
# Java concurrency: synchronized wait notify

## synchronized 实现同步互斥

java synchronized 关键字可以作用乎方法或代码块。
java 中每个对象有一个 lock，synchronized 使用对象的锁实现同步互斥。

- `synchronized` 用在方法声明时，同一个对象的 synchronized 方法不会被多个线程同时使用。

- `synchronized` 代码块需要显示地指明获取哪个对象的锁，可以定义一个 Object 作为控制同步互斥的锁。

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





</br></br>
