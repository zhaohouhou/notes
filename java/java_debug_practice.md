# 实用 Java debug tips

## 1. jdk tools

### 1.1 jdk tools 安装

```
apt-get update
apt install -y --no-install-recommends openjdk-11-jdk
```

### 1.2 visualvm 工具

visualvm 可以查看运行中java程序状态，可以新建和加载heap dump。超级好用。


### 1.3 查看内存状态

- Object 统计： `jmap -histo[:live] $pid`

  会打印 heap 上存活的对象。如果指定了live，会强制程序进行一次 full gc。

- dump map： `jmap -dump:live,format=b,file=$dump_file $pid`
  
  上面的命令将当前堆状态dump为bit文件格式。可以用visualvm加载查看。
visualvm可以查看每个object被哪些东西引用，还可以计算retained大小（object引用的东西整个占多少内存），
查找内存泄露很有帮助。

  'live' 选项同样会引发full gc。

- gc以及各个区的状态： `jstat -gc -t $pid [$print_interval_ms] [$times_to_print]`

  各个区域内存大小的时间单位为 KB
  
  加上时间一起输出日志: `watch -n 1 'date "+%Y-%m-%d %H:%M:%S" >> gc_log.txt; jstat -gc $PID >> gc_log.txt'`

### 1.4 查看程序运行状态

**jstack** 可以打印运行中的java线程状态（call stack, locks, ...）: `jstack -l $pid`. 对于debug死锁问题很有帮助。

option `-m`： Prints a mixed mode stack trace that has both Java and native C/C++ frames.

**Note: 类似于full gc，jstack 也会造成程序停止，尤其是线程很多的情况下，卡顿时间可能会到秒级。
因此需要考虑对程序行为可能的影响。**

### 其他

force full gc: `jcmd <pid> GC.run`

jps: Lists the instrumented Java Virtual Machines (JVMs) on the target system. Examples:

```
# List all JVM processes:
jps
# List all JVM processes with only PID:
jps -q
# Display the arguments passed to the processes:
# jps -m
# Display the full package name of all processes:
jps -l
# Display the arguments passed to the JVM:
jps -v
```

## 2. JNI debugging

### hs_err_pid 日志

java 程序挂在 c++ 代码里，例如 segment fault，会自动生成 hs_err_pid 文件。
包括：导致 crash 的线程信息、寄存器状态，所有线程信息，gc 相关记录，jvm 内存映射，jvm 启动参数 等。

hs_err_pid 日志默认生成在 /tmp 目录或者 working dir。可以通过 jvm 参数指定路径：

```
-XX:ErrorFile=/var/log/hs_err_pid<pid>.log
```

stack 信息中， C开头的是 native frame， J 开头的是 java frames。有这个日志基本可以确定挂在哪里了。


### gdb（没试过）

gdb 也可以 attach 到运行中 java app。
还可以启动 java 程序时加参数 `-XX:OnError="gdb — %p" ` 在运行出错时自动启动 gdb。

## 3. Debug leak in direct memory

Java direct 内存泄露是泄露在 heap 之外,现象是 htop 查看程序所占内存比 heap 要大很多(正常heap之外大概200M左右).
pmap 可以查看进程内存 memory map 状况.

### jemalloc

jemalloc 是 malloc 的一个替代实现, 能够trace malloc的调用, 可以用来做 heap profiling.

1. get jemalloc: https://github.com/jemalloc/jemalloc.git 获取源码.运行:

  ```
  ./configure --enable-prof   # profile需要这个. 不然Invalid conf pair
  make
  make install
  ```

2. 运行要profile的程序之前,设置环境变量(设置为临时的,不然一直用这个malloc了):

  ```
  export LD_PRELOAD=$JEMALLOC_INSTALL_PATH/lib/libjemalloc.so
  export MALLOC_CONF="prof:true,lg_prof_sample:17,lg_prof_interval:30"
  ```
  其中:
  - lg_prof_sample: 分配多少(2^lg_prof_sample)byte数据进行一次采样.默认为512 KiB (2^19 B).
  - lg_prof_interval: 分配多少(2^lg_prof_sample)byte数据进行一次快照.默认为-1 (disabled).
  
3. 运行程序,生成一堆 jeprof.pid.x.x.heap 文件.

### jeprof

jeprof 是 jemalloc 工具的一部分, 可以将 profile 结果可视化. 下面的命令可以把生成的heap dump(和trace信息)画成图:

```
jeprof --show_bytes --pdf `which $PROGRAM` jeprof.xxxxx.x.x.heap > out.pdf
```
notes:

- $PROGRAM 是程序运行命令。例如c程序"./a.out"，java 程序 "java -jar app.jar".
- 画 pdf 需要系统里有graphviz(for dot) 和 ghostscript(for ps2pdf). (sudo apt-get install ...)
- 这都是C程序了, 所以画出来的也是C函数, 还得再分析对应到 java 哪里
- 程序可以不停, 按trace信息 attach 上去 debug.

###  其他工具: gperftools

# ref

- https://www.bookstack.cn/read/rocksdb-en/e4f46b8585612202.md
- https://github.com/jemalloc/jemalloc/blob/dev/INSTALL.md
- https://github.com/jemalloc/jemalloc/wiki/Use-Case:-Leak-Checking

https://www.oracle.com/java/technologies/javase/6hotspotvm.html
