## Java多线程

### 1. 使用java多线程编程

#### 方式1：继承Thread类

#### 方式2：实现Runnable接口

通过重写(override)  `public void run()` 方法，实现能够被多线程地调用的函数。调用者创建一个该类的实例，并调用start()方法启动新线程运行run函数。也可以以该实例作为参数建立一个Thread的实例，并调用start启动线程执行run函数。例如：

```java
class Dog extends Thread {

    @Override
    public void run() {
        System.out.println("嗷");
    }
}

public class Test {
    public static void main(String[] args) {
        Dog dog = new Dog();
        dog.start();

		Dog dog2 = new Dog();
        Thread thread = new Thread(dog2);
        thread.start();
    }
}
```

如果直接调用run方法，则并不会启动新线程执行。

重载（overload）的run方法不能被多线程调用（函数类型不一致）。


### 2. 进程状态

Thread类的`Thread.State getState()`方法返回线程状态。相关的oracle文档中描述如下：（https://docs.oracle.com/javase/8/docs/api/java/lang/Thread.html ）


> public static enum Thread.State
> extends Enum<Thread.State>
>
> A thread state. A thread can be in one of the following states:
>
>   - **NEW**
>	A thread that has not yet started is in this state.
>   - **RUNNABLE**
>    A thread executing in the Java virtual machine is in this state.
>   - **BLOCKED**
>    A thread that is blocked waiting for a monitor lock is in this state.
>   - **WAITING**
>    A thread that is waiting indefinitely for another thread to perform a particular action is in this state.
>   - **TIMED_WAITING**
>    A thread that is waiting for another thread to perform an action for up to a specified waiting time is in this state.
>   - **TERMINATED**
>    A thread that has exited is in this state.
>
> A thread can be in only one state at a given point in time. These states are virtual machine states which do not reflect any operating system thread states.
>
> Since:
>    1.5



通过调用`boolean isAlive()`方法可以判断线程是否执行完成。



### 3. 线程调度

#### 等待线程执行完成：

希望等待线程结束，可以调用线程的join方法。

#### 终止线程执行：

`Thread.stop()`被声明不赞成使用，因为可能会引发不可预料的后果。

`Thread.interrupt()`中断线程。IO过程中可能无法中断。

获得当前当前的线程并停止希望停止的线程(或特定group的线程)：

```java
Thread[] array = new Thread[20];
Thread.enumerate(array); //将目前的线程序列填入array
for (Thread thread : array)
{
	if (thread == null) break; //array size更大，不停止循环会引发null pointer
	if (thread.getName().equals("xxx") ||
		thread.getThreadGroup().getName().equals("ooo"))
	{
		thread.stop();
	}
}

```

### 4. 线程池

#### 基本使用

固定大小线程池： Executors.newFixedThreadPool(n) => submit() => shutdownNow()

周期任务、延时任务： Executors.newScheduledThreadPool(n) => schedule()/scheduleAtFixedRate() => shutdownNow()

#### 基本原理

ThreadPoolExecutor: BlockingQueue 任务队列。

ScheduledThreadPoolExecutor (extends ThreadPoolExecutor): 

1. 包装类：如果task执行后需要重新schedule，再放入到workQueue里面。
2. 优先队列：workQueue是一个DelayedWorkQueue，实现了堆的数据结构。

#### 其他 topic

1. reject policy: AbortPolicy，CallerRunsPolicy，DiscardPolicy, DiscardOldestPolicy...
2. hooker: beforeExecute, afterExecute
3. 等待队列：避免使用无线长度的队列以占用太多内存。。。
