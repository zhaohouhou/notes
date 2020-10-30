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


# ref

https://www.bookstack.cn/read/rocksdb-en/e4f46b8585612202.md

https://www.oracle.com/java/technologies/javase/6hotspotvm.html
