# LLVM 使用

总结一些 LLVM (相关工具)的使用。

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

通过的测试例显示`PASS`。

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


## Ref:

http://llvm.org/docs/TestingGuide.html

http://llvm.org/docs/TestSuiteMakefileGuide.html

</br></br>
