
# LLVM 笔记4：LLVM 的 Pass

Pass 框架是 LLVM 系统的重要部分，LLVM 的优化和转换就是由多个 pass 一起完成的。本节将示例如何在 LLVM 系统中实现一个简单的 pass 。

## 1. pass API

通过实现 `Pass` 类的子类可以实现具体的优化过程。
`Pass` 类的不同子类具有不同的优化粒度：

- `ModulePass`：allows an entire module to
be analyzed at once, without any specific function order。要实现该类，需要重载`runOnModule()`方法。

- `FunctionPass`：handle one function at a
time, without any particular order。要实现该类，需要重载`runOnFunction()`方法。

- `BasicBlockPass`: basic block 粒度的优化。要实现该类，需要重载`runOnBasicBlock()`方法。

## 2. 实现一个定制化的 Pass

### 2.1. 编写代码实现 Pass 逻辑

首先根据需要优化的粒度，选择合适的 `Pass` 的子类进行实现，重载相应的方法。

在 LLVM 的 lib/Transforms 目录下建立 pass 代码目录，实现具体的 pass 逻辑。LLVM 项目中提供了示例的 "Hello World" pass, 位于 lib/Transforms/Hello 目录（下面一些代码以此为例）。Hello.cpp 的内容：

```c++
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#define DEBUG_TYPE "hello"

STATISTIC(HelloCounter, "Counts number of functions greeted");

namespace {
  struct Hello : public FunctionPass {
    static char ID; // Pass identification, replacement for typeid
    Hello() : FunctionPass(ID) {}

    bool runOnFunction(Function &F){ //重载接口方法
      ++HelloCounter;
      errs() << "Hello: ";
      errs().write_escaped(F.getName()) << '\n';
      //由于该Pass没有对IR进行修改，因此返回 fase
      return false;
    }
  };
}
// ID由LLVM内部赋值，用来区分Pass
char Hello::ID = 0;
//注册Pass
static RegisterPass<Hello> X("hello", "Hello World Pass");
```

上面实现了一个 `FunctionPass`，作用是输出每个函数的名称。LLVM 运行时，由 pass manager 对 pass 进行加载。
`static RegisterPass<FnArgCnt> X("hello", ...)` 的第一个参数可以被 opt 工具用来识别 pass。

<!--
 将 Pass 代码与 LLVM 一起 build：
  - 在代码目录建立 Makefile 文件。
  - 修改 lib/Transforms/Makefile 文件的 PARALLEL_DIRS 变量
   (这是资料里写的，经测llvm 4.0无法成功build？)
-->

### 2.2. Build Pass with CMake：

编写好 pass 的具体实现代码后，需要修改配置项使得 pass 能够与 LLVM 一起 build。下面的操作过程适用于 llvm 4.0 版本。

- 在代码目录建立 CMakeLists.txt 文件。示例：

  ```
  add_llvm_library( LLVMHello
    Hello.cpp

    ADDITIONAL_HEADER_DIRS
    ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms

    DEPENDS
    intrinsics_gen
    )
  ```

  代码目录新建 LLVMBuild.txt 文件，示例：

  ```
  [component_0]
  type = Library
  name = Hello
  parent = Transforms
  Library_name = Hello
  ```

- 修改上层 Transforms 目录的 CMakeLists.txt 文件，添加：

  ```
  add_subdirectory(Hello)
  ```

  修改 Transforms 目录的 LLVMBuild.txt，在 `[common]` 下的 `subdirectories` 添加：`Hello`。

## 3. 添加到命令行参数

使用 LLVM 的 CommandLine library 可以轻松地为我们实现的 pass 添加对应的命令行参数，方便调用 pass。下面举例说明如何添加命令行参数。

在 lib/Transforms/IPO/PassManagerBuilder.cpp 中添加：

```c++
 static cl::opt<bool> Hello("hello", cl::int(false),
    cl::desc("Enable Hello World pass"),
```

仍旧是该文件，在函数 `void PassManagerBuilder::populateModulePassManager` 中添加：

```c++
...
// Allow forcing function attributes as a debugging and tuning aid.
MPM.add(createForceFunctionAttrsLegacyPass());

//添加的代码
MPM.add(createHello(Hello));
...
```

以上需要引用相应的头文件（例如 Hello.h），可以在 include/llvm/Transforms/ 目录下建立头文件。函数 `createHello()` 的实现示例：

```c++
Pass *llvm::createHello(bool flag){return new Hello(flag);}
```

最后修改 lib/Transforms/IPO/LLVMBuild.txt，在 `required_libraries` 添加 `Hello`（否则 make 时会出错，找不到函数实现）。

<br/>

完成上面步骤后重新编译 LLVM 和 clang。此时再运行：

    $build目录/bin/clang -mllvm -hello test.c

即可驱动 Hello pass。添加 `-mllvm -debug` 参数可以查看 LLVM 的详细运行信息。

## 4. 添加 annotation

通过在函数定义上使用 annotation（注解），可以在编写源语言程序时控制 LLVM 在某个函数上使用、或禁止使用某些 pass。这种方式为编程人员提供了很大的灵活性。例如：

```c
int foo(int x) __attribute((__annotate__(("hello"))))
{
   return x + 1;
}
```

Clang 能够解析 annotation，不需要修改前端代码。LLVM IR 中的 `llvm.global.annotations` 含有 annotation 的信息，该变量是在 `Module` 中定义的。在 pass 实现中可以先获取 annotation 内容并判断是否要进行处理。

下面的代码获取与函数 f 相关的 annotations，拼接为一个字符串返回。

```c++
std::string readAnnotate(Function *f) {
  std::string annotation = "";

  // Get annotation variable
  GlobalVariable *glob =
      f->getParent()->getGlobalVariable("llvm.global.annotations");

  if (glob != NULL) {
    // Get the array
    if (ConstantArray *ca = dyn_cast<ConstantArray>(glob->getInitializer())) {
      for (unsigned i = 0; i < ca->getNumOperands(); ++i) {
        // Get the struct
        if (ConstantStruct *structAn =
                dyn_cast<ConstantStruct>(ca->getOperand(i))) {
          if (ConstantExpr *expr =
                  dyn_cast<ConstantExpr>(structAn->getOperand(0))) {
            // If it's a bitcast we can check if the annotation is concerning
            // the current function
            if (expr->getOpcode() == Instruction::BitCast &&
                expr->getOperand(0) == f) {
              ConstantExpr *note = cast<ConstantExpr>(structAn->getOperand(1));
              // If it's a GetElementPtr, that means we found the variable
              // containing the annotations
              if (note->getOpcode() == Instruction::GetElementPtr) {
                if (GlobalVariable *annoteStr =
                        dyn_cast<GlobalVariable>(note->getOperand(0))) {
                  if (ConstantDataSequential *data =
                          dyn_cast<ConstantDataSequential>(
                              annoteStr->getInitializer())) {
                    if (data->isString()) {
                      annotation += data->getAsString().lower() + " ";
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return annotation;
}
```

<!--
  ollvm 的 Utils.cpp 中 readAnnotate 函数
-->

## ref:

参考书：《Getting Started with LLVM Core Libraries》

http://llvm.org/docs/WritingAnLLVMPass.html

https://llvm.org/docs/CommandLine.html

本节例子参考了 ollvm 的实现

<br/><br/>
