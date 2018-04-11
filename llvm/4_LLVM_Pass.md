
# LLVM 笔记4：LLVM 的 Pass

## Pass API

通过实现 `Pass` 类的子类可以实现具体的优化过程。
`Pass` 类的不同子类具有不同的优化粒度：

- `ModulePass`：allows an entire module to
be analyzed at once, without any specific function order。要实现该类，需要重载`runOnModule()`方法。

- `FunctionPass`：handle one function at a
time, without any particular order。要实现该类，需要重载`runOnFunction()`方法。

- `BasicBlockPass`: basic block 粒度的优化. 要实现该类，需要重载`runOnBasicBlock()`方法。

## 实现一个定制化的 Pass

基本步骤：

1. 根据需要优化的粒度，选择的 `Pass` 的子类进行实现，重载相应的方法。

    在 lib/Transforms 目录下建立 pass 代码目录，实现具体的 pass 逻辑。LLVM 项目中提供了示例的 "Hello World" pass, 位于 lib/Transforms/Hello 目录（下面一些代码以此为例）。

2. 注册 Pass

  ```c++    
  static RegisterPass<FnArgCnt> X("hello",
      "inserting Hello World Pass");
  ```

3. 将 Pass 代码与 LLVM 一起 build：

  - 在代码目录建立 Makefile 文件。
  - 修改 lib/Transforms/Makefile 文件的 PARALLEL_DIRS 变量


#### Build pass with Cmake：

下面的移植过程在 llvm 4.0 版本能够成功。

- 在代码目录建立 CmakeLists.txt 文件。示例：

  ```
  add_llvm_library( LLVMHello
    Hello.cpp

    ADDITIONAL_HEADER_DIRS
    ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms

    DEPENDS
    intrinsics_gen
    )
  ```

  代码目录 LLVMBuild.txt 示例：

  ```
  [component_0]
  type = Library
  name = Hello
  parent = Transforms
  Library_name = Hello
  ```

- 上层 Transforms 目录 CmakeLists.txt 文件，添加：

  ```
  add_subdirectory(Hello)
  ```

  LLVMBuild.txt `common` 下的 `subdirectories`添加：`Hello`.

- 为了在 Clang 中通过命令行参数使用 pass，在 lib/Transforms/IPO/PassManagerBuilder.cpp 添加：

  ```c++
   static cl::opt<bool> Hello("hello", cl::int(false),
      cl::desc("Enable Hello World pass"),
  ```

  在函数 `void PassManagerBuilder::populateModulePassManager` 中添加：
  ```c++
  ...
  // Allow forcing function attributes as a debugging and tuning aid.
  MPM.add(createForceFunctionAttrsLegacyPass());

  //添加的代码
  MPM.add(createHello(Hello));
  ...
  ```

  以上需要引用相应的头文件例如 Hello.h，可以在 include/llvm/Transforms/ 目录下建立头文件。`createHello`的实现示例：

  ```c++
  Pass *llvm::createHello(bool flag){return new Hello(flag);}
  ```

  lib/Transforms/IPO/LLVMBuild.txt `required_libraries` 添加 Hello（否则 make 时会出错，找不到函数实现）。

完成上面步骤后再使用 `-mllvm -hello` 参数运行 Clang 编译程序，可以驱动 Hello pass。

#### 添加 annotate

为了编程时控制在哪些函数上使用 pass、在哪些函数上禁止使用 pass，
可以在函数定义通过 annotation 声明。Clang 能够解析 annotation，不需要再修改前端代码。在 pass 实现中可以获取 annotation 内容并判断是否要进行处理。

具体实现可参考 ollvm 的 Utils.cpp 中 readAnnotate 函数。

## ref:

参考书：《Getting Started with LLVM Core Libraries》

http://llvm.org/docs/WritingAnLLVMPass.html

https://llvm.org/docs/CommandLine.html

<br/><br/>
