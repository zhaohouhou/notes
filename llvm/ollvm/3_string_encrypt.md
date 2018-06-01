
# 混淆笔记3 - 基于 LLVM 的字符串加密

本节探讨如何基于 LLVM 对字符串进行加密。

### 基本知识

字符串在LLVM中间表示里用一个**常量数组**表示：

判断C字符串：`isa<ConstantDataSequential>`

Hikari 的作者提供的加密方案：搜索所有函数内对全局变量的调用,判断是否为常数数组，如是则在函数的起始位置插入解密函数，在函数的结尾插入加密函数。每个字符串采用不同的 key 加密（异或操作）

### 加密方案选择：

1. 最简陋的方案：在程序运行一开始解密所有字符串常量。这样加密之后，静态分析看不到字符串，动态运行容易获取原字符串。Armariris 采用了这种方案

下面的方案均需要为字符串添加状态量（例如新建相应的全局变量），以标志该字符串目前是明文或密文。

2. 在程序入口解密相关字符串（需首先判断是否需要解密）。保密性比方案 1 稍好。

3. 在程序入口解密并在结尾加密（均需要先进行判断）。存在的问题是，对于如下的情况，由于 `functionB()` 退出后又对 string 进行了加密，则 statement 3 使用 string 时面临错误的结果。

    ```c
    string = "123"

    functionA(){
      //statement 1
      print(string);

      functionB(){
        //statement 2
        print(string);
      }

      //statement 3
      print(string);
    }
    ```

    可能的一种解决方案：加密和解密不再通过 status 为 0 或 1 判断状态，采用计数的方式：

    - 每次解密若 status == 0 才执行真正的解密，之后无论是否解密 status 均 + 1。

    - 每次加密前 status 先 -1，若 status == 0 则执行加密。

4. 每次使用字符串均进行加解密：安全性较高、效率较低、实现较困难。

### 字符串识别

按照方案2和3实现字符串加密时，首先需要以 "基本块 -> Instruction -> 操作数" 的方式遍历需要处理的函数，并判断遍历到的 `Value` 是否是字符串常量。以下整理了一些要点。

- 字符串或者是 `GlobalVariable`，或者是一个指针变量 strip 后得到的 `GlobalVariable` 。

    > `stripPointerCasts()` - This method strips off any unneeded pointer casts from the specified value, returning the original uncasted value. Note that the returned value is guaranteed to have pointer type.

- C 字符串是 `ConstantDataSequential`，在 LLVM 表示中的名称一般为 `.str` 、 `.str.1` 、 `.str.2` ……

- Object C 字符串是一个 `struct.__NSConstantString_tag` 结构体，是 `ConstantAggregate` 类型。需要再从中将字符串常量取出来。


### 区分 ObjC string 和 c string

以下以一段 ObjectC 代码为例:

```c
static char* foo = "GlobalValue";

int main(int argc, char ** argv)
{
  printf("hello world\n");
  printf("%s\n", foo);
  NSLog(@"你好");
  return 0;
}

```

相应的 LLVM assembly：（命令 `clang -fobjc-arc -emit-llvm -S -c -o xxx.ll xxx.m`）

```
...
%struct.__NSConstantString_tag = type { i32*, i32, i8*, i64 }

@.str = private unnamed_addr constant [13 x i8] c"hello world\0A\00", align 1
@.str.1 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@foo = internal global i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str.5, i32 0, i32 0), align 8
@"OBJC_CLASS_$_NSString" = external global %struct._class_t
@"OBJC_CLASSLIST_REFERENCES_$_" = private global %struct._class_t* @"OBJC_CLASS_$_NSString", section "__DATA,__objc_classrefs,regular,no_dead_strip", align 8
@__CFConstantStringClassReference = external global [0 x i32]
@.str.2 = private unnamed_addr constant [3 x i16] [i16 20320, i16 22909, i16 0], section "__TEXT,__ustring", align 2
@_unnamed_cfstring_ = private global %struct.__NSConstantString_tag { i32* getelementptr inbounds ([0 x i32], [0 x i32]* @__CFConstantStringClassReference, i32 0, i32 0), i32 2000, i8* bitcast ([3 x i16]* @.str.2 to i8*), i64 2 }, section "__DATA,__cfstring", align 8
@OBJC_METH_VAR_NAME_ = private unnamed_addr constant [18 x i8] c"stringWithFormat:\00", section "__TEXT,__objc_methname,cstring_literals", align 1
@OBJC_SELECTOR_REFERENCES_ = private externally_initialized global i8* getelementptr inbounds ([18 x i8], [18 x i8]* @OBJC_METH_VAR_NAME_, i32 0, i32 0), section "__DATA,__objc_selrefs,literal_pointers,no_dead_strip", align 8
@OBJC_METH_VAR_NAME_.3 = private unnamed_addr constant [11 x i8] c"UTF8String\00", section "__TEXT,__objc_methname,cstring_literals", align 1
@OBJC_SELECTOR_REFERENCES_.4 = private externally_initialized global i8* getelementptr inbounds ([11 x i8], [11 x i8]* @OBJC_METH_VAR_NAME_.3, i32 0, i32 0), section "__DATA,__objc_selrefs,literal_pointers,no_dead_strip", align 8
@.str.5 = private unnamed_addr constant [12 x i8] c"GlobalValue\00", align 1

...

; Function Attrs: noinline optnone ssp uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %retval = alloca i32, align 4
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca i8**, align 8
  store i32 0, i32* %retval, align 4
  store i32 %argc, i32* %argc.addr, align 4
  store i8** %argv, i8*** %argv.addr, align 8
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @.str, i32 0, i32 0))
  %0 = load i8*, i8** @foo, align 8
  %call1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i8* %0)
  %1 = load %struct._class_t*, %struct._class_t** @"OBJC_CLASSLIST_REFERENCES_$_", align 8
  %2 = load i8*, i8** @OBJC_SELECTOR_REFERENCES_, align 8, !invariant.load !8
  %3 = bitcast %struct._class_t* %1 to i8*
  %call2 = call i8* (i8*, i8*, %0*, ...) bitcast (i8* (i8*, i8*, ...)* @objc_msgSend to i8* (i8*, i8*, %0*, ...)*)(i8* %3, i8* %2, %0* bitcast (%struct.__NSConstantString_tag* @_unnamed_cfstring_ to %0*))
  %4 = call i8* @objc_retainAutoreleasedReturnValue(i8* %call2)
  ...
}
```


### 其他问题

0) 字符串常量位于静态区，对应程序的 `__TEXT` 段，默认是不可写的，因此需要进行更改。可将 `__TEXT` 段更改为 `__DATA,__const` 段。

1) 全局变量初始化

```c
char a[] = "123";
int* b = (int*)a;

main(){
    ....
}
```

在 llvm.global_ctors 中解密。Hikari 没有对这种情况的字符串进行加密。

2) 全局变量引用到字符串全局变量

```c
byte* a;
char b[] = "123";

functionA(){
  a = b;
}

functionB(){
  do sth. with a
}

main(){
  functionA();  functionB();
}
```

1. analyze the GV's def-use chain and locate any instructions referencing it。或者一旦某字符串被全局变量引用，则进行标记，运行中再也不对其进行加密。

2. 采用不进行再加密的方案。经过反汇编混淆后的代码和lldb调试可以看到，Hikari开源版本采用的是这种方案。

3) ObjC 类名

对于 ObjC 代码，dyld 在 main executable 之前执行，因此如果 ObjC 类名被加密、则启动时注册的类名是错误的。因此，需要加以区分。ObjC 类名、方法明等所属的 section 有：`__objc_methname`、`__objc_classname` 等，全局变量名含有 "OBJC_" 。

4) 多线程

对于多线程的情况，几个线程可能都需要访问某字符串，需要避免 race-condition 的情况发生，例如重复对字符串进行解密。因此，至少访问字符串加密状态变量时需要进入临界区。

## ref:

http://bbs.iosre.com/t/llvm/10610  基于llvm的字符串加密


<br/><br/>
