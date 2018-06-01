
# LLVM 笔记5：Link-Time Obfuscation

除了对单个编译单元（即 Module）的优化，LLVM 还提供了强大的模块间优化功能——连接时优化（Link-Time Obfuscation，LTO）。顾名思义 LTO 是发生在连接期的优化过程。

## 1. 设计哲学

LLVM LTO 在编译工具链中提供了全透明的模块间优化，通过与 linker 紧密集成，使得开发者能够不必过多变更 makefile 或 build system 便可使用模块间优化。

该模型中，linker 使用共享库 **libLTO** 处理 LLVM bitcode 文件。（因此系统的 linker 需要有支持 LTO 的功能。）

## 2. LTO 实现示例

下面通过实现一个简单的 LTO 功能（打印出所有函数名）示例 LTO 如何工作。

基本的思想是：1）实现一个 ModulePass；2）在 LTO 期间，多个编译单元被组装成一个单个的 Module，通过在这个 Module 上运行 pass 来实现 LTO。

#### 1. 实现 ModulePass

ModulePass 实现代码的 .cpp 与 .h 文件和 CMakeLists.txt 与一般的 pass 建立过程一样。

```c++
//TestLTO.h
#ifndef _TEST_LTO_H_
#define _TEST_LTO_H_
#include "llvm/Pass.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/PassRegistry.h"
using namespace std;
using namespace llvm;

namespace llvm {
  Pass * createTestLTOPass();
  Pass * createTestLTOPass(bool flag);
  void initializeTestLTOPass(PassRegistry &Registry);
}
#endif
```

```c++
//TestLTO.cpp
#include "llvm/Transforms/TestLTO/TestLTO.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include <iostream>

using namespace llvm;
using namespace std;

namespace {
    struct TestLTO : public ModulePass {
        static char ID;
        bool flag;

        TestLTO() : ModulePass(ID) { this->flag = true; }
        TestLTO(bool flag) : ModulePass(ID) { this->flag = flag; }
        StringRef getPassName() const override { return StringRef("TestLTO"); }
        bool runOnModule (Module &M) override;
    };
}

char TestLTO::ID = 0;
static RegisterPass<TestLTO> X("testLTO", "test LTO", false, false);
Pass *llvm::createTestLTOPass(bool flag) { return new TestLTO(flag); }

bool TestLTO::runOnModule(Module &M) {
  if (! flag )
    return false;
  errs() << "====================================\n";
  errs() << "Running TestLTO On Module " <<  M.getName() << "\n";

  //Print all the functions.
  for (Module::iterator iter = M.begin(); iter != M.end(); iter++) {
    Function *F = &(*iter);
    errs() << "  Function:" << F->getName() << "\n";
  }
  errs() << "====================================\n";
  return true;
}
```

#### 2. 注册 LTO Pass

修改 `llvm/Transforms/IPO/PassManagerBuild.cpp` 文件，添加：

```c++
static cl::opt<bool> TestLTO("test-lto", cl::init(false),
                            cl::desc("test LTO"));
```

在 `populateLTOPassManager()` 函数中添加：

```c++
  PM.add(createTestLTOPass(TestLTO));
```

#### 3. 运行 LTO Pass

准备用于测试的源代码文件。下面的例子中 `Test.h` 和 `Test.cpp` 中实现一个 `Test` 类，并在 `main.cpp` 中调用了 `Test` 类的方法。

build 修改后的 LLVM，通过下面的命令使用 clang++ 编译 `Test.cpp` 和 `main.cpp` 生成可执行文件，正常的情况下即可调用我们实现的 LTO pass 输出所有函数名：

```
$ LLVM_BUILD_PATH/bin/clang++ -v -flto -Wl,-mllvm,-test-lto ./Test.cpp ./classTest.cpp​
```

使用到的参数：

- `-flto`： 生成 LLVM bitcode 以便支持 LTO。

- `-Wl,<args>`：将逗号分隔的参数传递给 linker。我们需要向 linker 传参 `-mllvm -test-lto`，因此写为 `-Wl,-mllvm,-test-lto`

- `-v`：verbose output，显示 clang 运行中使用的命令。通过添加该参数我们可以看到 clang 是如何调用和向 linker 传递参数的。该例子中我们可以看到 linker 使用了 `LLVM_BUILD_PATH/lib/libLTO.dylib`.

（如果不能找到 libc++ 头文件则添加参数：`-I  PATH_TO_LIBCXX_HEADERS`）

## ref:

- LLVM Link Time Optimization: Design and Implementation

  https://llvm.org/docs/LinkTimeOptimization.html

- Clang 7 documentation

  https://clang.llvm.org/docs/CommandGuide/clang.html

<br/><br/>
