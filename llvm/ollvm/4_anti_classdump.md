
# 混淆笔记4 - 基于 LLVM 的反 class dump

本节探讨如何实现基于 LLVM 实现对抗 classdump 工具的原理。

基本思想: 将原来的类的方法封装到自己实现的初始化函数。

Hikari的实现分两个版本：开源版本是 per-module 处理，结构体还保留在 module 中；私有版本包含 LTO（Link-time Optimization）过程的处理，会构建 dependency graph 并完全抹除结构体信息。

## 一、前置知识

### 1. 类方法、实例方法

Objective-C 代码中以 `-` 开头的方法是**实例方法**。它属于类的某一个或某几个实例对象，类对象必须实例化后才可以使用。

以 `+` 开头的方法是类方法。Objc中的类方法类似Java中的static方法，它是属于类本身的方法，不需要实例化类，用类名即可使用。

用 Hopper 查看反编译的 Object-C 项目，对照源代码和 `struct __objc_data`、 `struct __objc_method`、 `struct __objc_method_list` 这几个结构体，可以看到：

- 实例方法（`-`）对应 `_OBJC_CLASS_$_XXX` 中的data域里的 method_list；

- 类方法（`+`）对应 `_OBJC_METACLASS_$_XXX` 中的data域里的 method_list。

### 2. +initialize 和 +load 方法

+initialize 和 +load 方法都是 NSObject 类的初始化方法，调用顺序均为：基类->子类。区别是 +load 在类被添加到 runtime 时调用，+initialize 在类接收到第一条消息时调用。

### 3. Objective-C 的 Category

Category 的作用是为已经存在的类添加方法。Category 声明文件和实现文件统一采用“原类名+Category名”的方式命名。下面的代码定义了一个Category `MyAddition`，向类 `MyClass` 添加了函数 `printNameAddition`：

```objc
@interface MyClass : NSObject
- (void)printName;
@end

@interface MyClass(MyAddition)
- (void)printNameAddition;
@end
```

实现相应方法并编译为bitcode assembly查看，发现多了下面一些结构体：

```
@"\01l_OBJC_$_CATEGORY_INSTANCE_METHODS_MyClass_$_MyAddition" = ...
@"\01l_OBJC_$_CATEGORY_MyClass_$_MyAddition" =
    private global %struct._category_t { ... }, section "__DATA, __objc_const", align 8
...
@"OBJC_LABEL_CATEGORY_$" = private global ...
```

其中，全局变量 `@"\01l_OBJC_$_CATEGORY_MyClass_$_MyAddition"` 是一个结构体 `struct _category_t`，其定义如下：

```
struct _category_t {
  const char * const name; // CategoryName
  struct _class_t *const cls;  // the class to be extended
  const struct _method_list_t * const instance_methods;
  const struct _method_list_t * const class_methods;
  const struct _protocol_list_t * const protocols;
  const struct _prop_list_t * const properties;
  const struct _prop_list_t * const class_properties;
  const uint32_t size;
}
```

将程序编译为二进制后再查看，可以发现已经找不到 `MyAddition` 这一名称，而函数 `printNameAddition()` 已经在 `MyClass` 的方法列表中。Category的方法被放到了方法列表的前面。因此由于查找方法时是顺序查找的，category的方法会“覆盖”掉原来类的同名方法。

## 二、代码分析

下面分析 Hikari 的 per-module 反 class dump 处理。

#### 0. doInitialization()

在处理Module前进行一些初始化。

收集类型信息并插入需要用到的函数定义（ objc runtime 库自带的一些函数）。一些 ObjC 相关的结构体定义在 `CGObjCMac.cpp` 中可以找到，例如 `struct _ivar_t`，`struct _ivar_list_t` 等等。

#### 1. runOnModule()

函数的四个主要数据结构：

```c++
vector<string> readyclses; // 保存handleClass()需要处理的class
deque<string> tmpclses;    // 临时存储，辅助生成类初始化顺序
map<string /*class*/, string /*super class*/> dependency;
map<string /*Class*/, GlobalVariable *> GVMapping; // 类名 -> GlobalVariable
```

1. 通过 GlobalVariable `"OBJC_LABEL_CLASS_$"` 的 initializer 获得 class 列表。

2. 对每个 class 项：

    - 如果没有 super class 或 super class 是 external 的，则加入到 `readyclses`；否则加入到 `tmpclses` 待进一步处理。

    - 保存 super class 关系到 `dependency`，保存相应的GV到 `GVMapping`。

3. 循环从 `tmpclses` 开头取出 class 直到 `tmpclses` 为空：

    - 如果该 class 没有 super class 或其 super class 已经在 `readyclses`，则加入到 `readyclses`；

    - 否则入队 `tmpclses` 结尾

完成上面步骤之后 `readyclses` 中为正确的类初始化顺序，依次调用 `handleClass` 进行处理。

#### 2. handleClass()

-  `BasicBlock *EntryBB`: 遍历 class 的 method list，找到 +initialize 或 +load 方法，将该函数入口赋值给 `EntryBB`；如果未找到则新建一个 initializer 方法，新建其第一个 BasicBlock 赋值给 `EntryBB`。从 `EntryBB` 新建 IRBuilder 并添加 call objc_getClass()。

- 调用 handleMethods() 处理所有的实例方法，建立新的方法列表并替换原来的。

- 调用 handleMethods() 处理所有的类方法，建立新的方法列表并替换原来的。

#### 3. handleMethods()

函数原型：

```c++
void HandleMethods(ConstantStruct *class_ro, IRBuilder<> *IRB,
      Module *M, Value *Class, bool isMetaClass);
```

对每个列表中的函数，在IRB添加相应的初始化函数的调用。


### ref:

- Anti class-dump Implementation Notes

  https://mayuyu.io/2017/12/21/AntiClassDumpImplementationNotes/

- Hikari manual

  https://naville.gitbooks.io/hikaricn/content/Pass/AntiClassDump.html

- Objective-C 对象模型

  http://blog.leichunfeng.com/blog/2015/04/25/objective-c-object-model/

- Objective-C +load vs +initialize

  http://blog.leichunfeng.com/blog/2015/05/02/objective-c-plus-load-vs-plus-initialize/

- Objective-C：Category

  https://tech.meituan.com/DiveIntoCategory.html

  http://blog.leichunfeng.com/blog/2015/05/18/objective-c-category-implementation-principle/

</br></br>
