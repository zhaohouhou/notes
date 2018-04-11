
# LLVM 笔记3：LLVM 中间表示

编译器的 IR 设计理念：

- high-level IR： 容易优化

- low-level IR： 容易生成目标相关代码、针对特定的目标机器进行优化；一般性较差，不利于对多个不同架构生成目标代码。

GCC 和 LLVM 这样支持多种平台的编译器称为 *retargetable compilers* ，需要具有通用性的 IR 以控制开发成本。在通用性 IR 上可以进行目标无关优化。另一方面，一个好的 retargetable compiler 为了实现目标相关的优化，还应该同时采用其他的 lower level IR。

LLVM IR 指 LLVM 中最具有一般性的中间表示，使用 `Function` 和 `Instruction` 等类表示。除此以外 LLVM 还使用了其他层次的 IR，但官方上，LLVM IR 仅指 `Instruction` 类等。LLVM IR 有三种等价形式：内存表示，bitcode file，LLVM assembly file。

编译为LLVM字节码：

    $ clang sum.c -emit-llvm -c -o sum.bc

生成 assembly file：

    $ clang sum.c -emit-llvm -S -c -o sum.ll

下图展示了 LLVM IR 的基本模型。模块（module）是最顶层结构，由若干 function 组成，此外还包含 global variables, the target data layout, and external
function prototypes as well as data structure declarations. LLVM local value 相当于寄存器。

![](llvm_IR_model.png)

LLVM 语法的一些基本性质：

- 采用单静态赋值形式 （Static Single Assignment, SSA），易于分析和优化。

- 指令为三地址码
（two source operands and a destination operand）

- 寄存器的个数没有上限。
LLVM local value 的命名以 `%` 开头，global value 以 `@` 开头。

## LLVM IR Detail

### Iterators - 在 IR 层迭代

- Module::iterator

  Iterates through functions in the module

- Function::iterator

  Iterates through a function's basic blocks

- BasicBlock::iterator

  Iterates through instructions in a block

- Value::use_iterator

  Iterates through **uses**。指令和常量都使用 Value 表示

- User::op_iterator

  Iterates over operands

### Instructions

Instruction 有 LoadInst, StoreInst, CmpInst, BranchInst 等。 BranchInst 只能出现在基本块结尾；Phi instruction 只能出现在基本块开头。

（个人理解，Phi/φ 指令用于实现 branching、保存前继节点和 value 的生存期间的关系）

### Types

Primitive types：

  - Integers (iN)
  - Floating point (half, float, double, ...)
  - others (x86mmx, void, ...)

Derived types：

  - Arrays  ([# elements (>= 0) x element type])
  - Functions (returntype (paramlist))
  - Pointers (type\*, type addrspace(N)\*)
  - Vectors (<# elements (> 0) x element type>)
  - Structures ({ typelist }) ...

## IR 层优化

程序被翻译为LLVM IR之后，会经过一系列目标无关优化。

`opt`工具可以对 bytecode file 进行优化，并且与 Clang 一样接受 `-O0, -O1, -O2, -O3, -Os` 和 `-Oz` 参数。此外 Clang 还支持 `-O4`。每个 flag 表示一种优化流程（/等级）:

- `-O0`：no optimization

- `-O1`：between `-O0` and `-O2`

- `-O2`：enables most optimizations

- `-Os`：considered as `-O2` with extra optimizations to reduce code size

- `-Oz`：like `-Os` (and thus `-O2`), but
reduces code size further

- `-O3`：在`-O2`的基础上进行一些可能耗时长或增加代码大小的优化，以求更高的程序运行速度

- `-O4`：enables link-time optimization (On supported platforms). object files are stored as LLVM bitcode file.

使用 opt 工具对 bytecode file 进行优化：

    $ opt -O3 sum.bc -o sum-O3.bc -stats

`-stats`参数表示打印每个pass的统计信息，`-time-passes`参数显示每个优化过程所耗时间，等等。


## ref:

参考书：《Getting Started with LLVM Core Libraries》

https://llvm.org/docs/CommandLine.html

<br/><br/>
