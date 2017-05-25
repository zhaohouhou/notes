import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

/**
 * Created by zd on 5/25/17.
 *
 * Thread Scheduler codes.
 */

public class WorkThreadFactory implements ThreadFactory
{
private long counter;

public WorkThreadFactory() {
        counter = 0;
}
@Override
public Thread newThread(Runnable run)
{
        String name = "Thread_" + counter;
        Thread t = new Thread(run, name);
        counter++;
        System.out.printf("Created thread %d with name %s on %s\n",
                          t.getId(),t.getName(),new Date());
        return t;
}
}

public class WorkRejectPolicy implements RejectedExecutionHandler
{
    /**Rejected handler.*/
    @Override
    public void rejectedExecution(Runnable r, ThreadPoolExecutor executor)
    {
        //Do things...
        System.out.println("Job rejected (Busy)");
    }
}

public class Scheduler implements Runnable
{
private static Scheduler instance=null;
private ThreadPoolExecutor executor=null;
private WorkRejectPolicy rejectHandler;
private LinkedBlockingQueue<Runnable> workQueue;
private ThreadFactory threadFactory;

private Scheduler(){
        this.threadFactory = new WorkThreadFactory();
        this.workQueue = new LinkedBlockingQueue<Runnable>();
        this.rejectHandler = new WorkRejectPolicy();
        this.executor = new ThreadPoolExecutor(
                THREAD_CORE_POOL_SIZE,
                THREAD_MAX_POOL_SIZE,
                THREAD_ALIVE_TIME,
                TimeUnit.SECONDS,
                workQueue,
                threadFactory,
                rejectHandler);

        new Thread(this).start(); //start printing schedule status.
}

public synchronized static Scheduler getInstance(){
        if(instance==null) {
                instance=new Scheduler();
        }
        return instance;
}

public synchronized void submit(Runnable run)
{
        this.executor.execute(run);
}

@Override
public void run()
{
        while (true) {
                System.out.println("线程池中线程数：" + executor.getPoolSize()
                                   + "，队列中等待执行的任务数："
                                   + executor.getQueue().size()
                                   + "，已完成任务数："
                                   + executor.getCompletedTaskCount());
                try {
                        Thread.sleep(60000);
                } catch (InterruptedException e) {
                        e.printStackTrace();
                }
        }

}
}
