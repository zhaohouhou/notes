# Java向Process发送Ctrl+C

由于一些原因需要在java中启动一个进程执行命令，并通过向该进程发送Ctrl+C控制其运行。花费小一天时间搞定，做个记录

## 1. Ctrl+C

类似`Runtime.getRuntime().exec("ctrl+c");`是肯定不行的（因为没这个命令）。并且`Runtime.getRuntime().exec()`是启动新的进程执行。

ctrl+c 其实是给进程发送 SIGINT 信号。可以通过`kill -2 pid`来实现。因此问题转换为如何获得Process的pid。

**题外话**：`Runtime.getRuntime().exec("cd XXX");`这个命令是不能完成的（会报错找不到cd），因为cd不是一个单独的程序，并且启动一个新的进程并不能改变当前进程的working directory。参考http://stackoverflow.com/questions/4884681/how-to-use-cd-command-using-java-runtime


## 2.获得Process pid

由于`Runtime.getRuntime().exec("xxx");`会返回一个Process实例。保存这一结果并调试。Ubuntu下运行发现返回值为UNIXProcess类型。下面针对这个Process类型讨论。

http://bbs.it-home.org/thread-60006-1-1.html 提到可以利用Process.toString()并分析返回结果得到pid，但是调试发现UNIXProcess.toString()返回结果并不会包含pid。

尝试发现UNIXProcess的pid域是不可见的。搜到可以通过Java反射获取一个类的私有属性。

最后实现功能的代码可总结如下：

```java
	Process p = Runtime.getRuntime().exec(cmd);
	...
	String pid;
	try {
		Class clazz = Class.forName("java.lang.UNIXProcess");
		Field pidField = clazz.getDeclaredField("pid");
		pidField.setAccessible(true);
		Object value = pidField.get(process);
		pid = value.toString();

   	} catch (Throwable e) {
		e.printStackTrace();
   	}
	...
	Runtime.getRuntime().exec("kill -2 " + pid);
```

经测能够发送ctrl+c信号。

### 其他：

linux 下可使用**`ps -ef`**命令查看各进程和pid，配合进行测试。
