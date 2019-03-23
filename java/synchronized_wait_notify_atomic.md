
# Java concurrency: synchronized wait notify

## synchronized 实现同步互斥

java synchronized 关键字可以作用乎方法或代码块。
java 中每个对象有一个 lock，synchronized 使用对象的锁实现同步互斥。

- Synchronized Instance Methods： 用在方法声明时，同一个对象实例的 synchronized 方法在
一个时刻只会被一个线程进入。

- Synchronized Static Methods：synchronized on the class object。Only one
 thread can execute inside a static synchronized method in the same class.

- Synchronized Blocks in Instance Methods:
`synchronized` 代码块需要显示地指明 monitor object，可以定义一个 Object 作为锁。
Only one thread can execute inside a Java code block synchronized on the same monitor object.

- Synchronized Blocks in Static Methods

- `wait()`：the calling thread will give up the monitor and sleep until
some other thread enters the same monitor and calls `notify()`.

- `notify()`: wakes up the a thread that is waiting on the same object.

## an Example

```java
import java.util.Date;
import java.util.Vector;

public class TestConsumer {

    static class Producer extends Thread {

        static final int MAXQUEUE = 5;
        private Vector messages;

        public Producer(Vector messages) {
            this.messages = messages;
        }

        @Override
        public void run() {
            try {
                while (true) {
                    putMessage();
                    sleep(1000);
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        private synchronized void putMessage() throws InterruptedException {
            while (messages.size() == MAXQUEUE) {
                wait();
            }
            String message = new Date().toString();
            messages.addElement(message);
            System.out.println("put message" + message);
            notify();
            //notify() threads that are blocking on the same object.
        }

        public synchronized String getMessage() throws InterruptedException {
            while (messages.size() == 0) {
                //By executing wait() from a synchronized block, a thread gives up its hold on the lock and goes to sleep.
                wait();
            }
            String message = (String) messages.firstElement();
            messages.removeElement(message);
            notify();
            return message;
        }

    }

    static class Consumer extends Thread {

        private Producer producer;

        public Consumer(Producer producer) {
            this.producer = producer;
        }

        @Override
        public void run() {
            try {
                while (true) {
                    String message = this.producer.getMessage();
                    System.out.println("Got message: " + message);
                    sleep(200);
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public static void main(String[] args) {
        Vector messages = new Vector();

        Producer producer = new Producer(messages);
        Consumer consumer = new Consumer(producer);
        producer.start();
        consumer.start();
    }
}
```

## volatile & Atomic

### volatile 关键字（after Java 5）

The Java `volatile` keyword is used to mark a Java variable as "being stored
in main memory" (see java-memory-model). All writes to the variable will be
written back to main memory immediately; all reads of the variable will be read
directly from main memory. Declaring a variable volatile guarantees the
 *visibility* for other threads of writes to that variable (otherwise different
  reader thread may see different values.）

除了 visibility 保证, volatile 还通知 java compiler 在编译优化（会发生指令重排）的过程中
提供 "happens-before（有序性）" 保证:

1. 对其他变量的读写如果发生在写 volatile 变量之前，则不能被移动到写 volatile 变量操作之后。
反方向的移动是可以的。

2. 对其他变量的读写如果发生在读 volatile 变量之后，则不能被移动到读 volatile 变量操作之前。
反方向的移动是可以的。

（写 volatile 变量之前需保证所有前面的写操作已经生效；
而读取 volatile 变量的操作生效后才可以进行后面的读操作。）

```Java
int a, b, c;
volatile int v;

a = c; v = b;     =>   v = b; a = c;   // ×
v = b; a = c;     =>   a = c; v = b;   // √

a = b; b = v;     =>   b = v; a = b;   // √
b = v; a = c;     =>   a = c; b = v;   // ×
```

其他：
- 在Java中，对基本数据类型的变量的读取和赋值操作是原子性操作；
而 `volatile` 可以用于 32 和 64 位数据类型的变量。

- `volatile` 不能提供 "get-and-set" 的原子性，例如 `v++`。此时可使用 `Atomic` classes,
例如 `AtomicInteger` 和 `AtomicReference`。（如果只是分别用 get 和 set，则
`AtomicReference` reference 和 `volatile` 一样。）

- 由于每次读写 volatile 变量都需要访问 main memory，代价比访问 CPU cache 高很多。
此外限制指令重排会影响编译优化。

### Atomic classes

package: java.util.concurrent.atomic.* 。提供原子的读写操作和 `compareAndSet()` 等
更多的原子操作。包括 `AtomicBoolean`、`AtomicInteger`、`AtomicLong`、`AtomicReference`
等。

## ref:

http://tutorials.jenkov.com/java-concurrency/volatile.html

http://tutorials.jenkov.com/java-concurrency/java-memory-model.html

</br></br>
