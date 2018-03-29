
# LLVM 笔记2：前端 Frontend

编译器系统中，前端的作用是将源代码转换为编译器的中间表示。
前端的实现是和源语言相关的，一个前端可能针对某一个几个相似的编程语言，例如 Clang 面向 C、 C++ 和 objective-C。
本节以 Clang 为例讨论 LLVM 的前端。

## 前端的功能（略）

- 词法分析

- 语法分析

- 语义分析

- 生成 LLVM IR

## Libraries

Clang 的设计是模块化的，由几个库组成。
`libclang` 是最主要的接口，提供C语言的前端API，由几个库组成。一些相关的库：

<table border="2" align="left">
  <tr>
    <th>`libclangLex`</th>
    <td> 词法分析 </td>
  </tr>
  <tr>
    <th>`libclangAST`</th>
    <td> 构造和操作抽象语法树</td>
  </tr>
  <tr>
    <th>`libclangParse`</th>
    <td> parse logic </td>
  </tr>
  <tr>
    <th>`libclangSema`</th>
    <td> semantic analysis (AST verification) </td>
  </tr>
  <tr>
    <th>`libclangCodeGen`</th>
    <td> LLVM IR code generation </td>
  </tr>
  <tr>
    <th>`libclangAnalysis`</th>
    <td> static analysis </td>
  </tr>
  <tr>
    <th>`libclangRewrite`</th>
    <td> code rewriting; code-refactoring utilities </td>
  </tr>
  <tr>
    <th>`libclangBasic`</th>
    <td> utilities: memory allocation
abstractions, source locations, diagnostics, etc. </td>
  </tr>
</table>

## ref:

参考书：《Getting Started with LLVM Core Libraries》

<br/><br/>
