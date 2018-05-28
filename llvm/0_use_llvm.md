# LLVM 使用

总结一些 LLVM (相关工具)的使用、以及一些对 LLVM 开发有帮助的命令。

## 查看程序 CFG 图

1. 生成 bytecode 文件

        $LLVM_BUILD_PATH/bin/clang add.c -emit-llvm -c -o add.bc

2. 使用 opt 工具生成 dot 格式的 CFG。

        $LLVM_BUILD_PATH/bin/opt add.bc -dot-cfg

3. 使用 dot 工具能够可视化地查看 CFG。例如 Graphviz

    Mac 上安装 Graphviz：

        $brew install Graphviz

    从 dot 文件生成图片文件：

        $GRAPHVIZ_PATH/bin/dot -Tjpg xxx.dot -o xxx.jpg


## LLVM Debug Info

如果 LLVM 的 build 模式为 debug 模式，那么使用 `-debug` 参数可以输出 LLVM 编译过程中的调试信息：

    $clang -mllvm -debug test.c

在 LLVM 开发中，若需要添加 debug info，可以使用宏 `DEBUG_WITH_TYPE` (include/llvm/Support/Debug.h)。例如：

```c++
DEBUG_WITH_TYPE("test",
    errs() << "TEST: some info: " << value << "\n");
```

输出类似于：

    TEST: some info: 123


## 打印 IR

通过 LLVM 的 `-print-after-all` 选项可以打印出每个 pass 之后的程序的中间表示，可以帮助我们查看 pass 对程序所作的改变，对于自己编写的 Pass 便于定位问题。使用 Clang：

    $clang -mllvm -print-after-all xxx.c

可以通过 `-filter-print-funcs` 选项指定一个或几个需要打印的函数：

    $clang -mllvm -filter-print-funcs=foo -mllvm -print-after-all xxx.c


## LLVM testing

LLVM 测试框架由回归测试（regression tests）和 whole programs 两部分组成。

- Regression tests 是一些测试 LLVM 特定功能的代码片段，由 `lit` 驱动，测试例位于 llvm/test 目录。

- test-suite 模块包含一些完整项目：由一些代码编译链接成一个可执行程序。需要单独从SVN库上下载。

#### 回归测试

运行所有的 LLVM 回归测试：

    $make check-llvm

测试 LLVM 和 Clang

    $make check-all

要进行单个或部分回归测试，可使用LLVM 集成测试工具 - lit。

    $llvm-lit XXX/XXX.ll

    $llvm-lit XXX(directory)

通过的测试例显示 `PASS`。


## libc path

如果 clang 在编译时找不到 libc 头文件，可以修改环境变量使 libc 查找路径指向正确的目录下（libc.a 所在目录）。环境变量 `LD_LIBRARY_PATH` (`DYLD_LIBRARY_PATH` on OS X)。

编译出的 clang++ 可能找不到 libc++ 的基本头文件。可以使用 `-v` 参数查看 include 文件的 search 路径。如果安装了 libc++ 还是不能找到相应头文件，可以编译时通过 `-I` 手动包含正确的 search 路径。


## Ref:

http://llvm.org/docs/TestingGuide.html

http://llvm.org/docs/TestSuiteMakefileGuide.html

</br></br>
