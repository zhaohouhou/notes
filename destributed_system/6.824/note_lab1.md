#6.824 Lab 1: MapReduce
结构：

 1. 完成一个简单的MapReduce程序
 2. 编写Master程序，向worker分派任务。
 3. 使Master程序能处理错误。

##Getting started

需要安装git（git clone git://g.csail.mit.edu/6.824-golabs-2015 6.824）和Go语言。

Windows下：安装完成后需要设置用户环境变量。PATH中增加Go的安装目录（bin文件夹）；增加一个GOPATH指向Go的工作目录。

##工作过程：
1）MakeMapReduce()函数初始化一个MapReduce结构体，初始化注册服务器，并调用MapReduce的Run()函数启动MapReduce过程。

2）MapReduce的Run()函数首先调用Split()将文件分块（每块按序号赋予不同文件名）；接着调用RunMaster()启动任务调度；最后调用进行Merge()进行合并。

3）由应用程序（这里为test测试例）调用RunWorker()：为worker指明Map与Reduce函数，用rpc发布worker，并向MapReduce注册Worker。于是MapReduce结构体的registerChannel接收到注册信息（Worker名）。

4）RunMaster()进行任务调度与错误处理。首先分配Map任务，当Map任务都完成后开始分配Reduce任务，最后Reduce任务均完成后返回。任务的分配是并行的，从MapReduce结构体的registerChannel接收Worker信息，并通过远程调用call来启动Worker的DoJob()函数。