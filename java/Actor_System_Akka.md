
## Actor模型

## Akka for JAVA

Akka中有一个Actor System. Actor System统管了Actor,是Actor的系统工厂或管理者,掌控了Actor的生命周期.

### Actor路径

Akka 中的 Actor 具有 parent -> child 层级结构，并且引入了 Actor path 来管理树形结构。
Actor路径使用类似于URL的方式来描述一个Actor,Actor Path在一个Actor System中是唯一的。
通过路径,可以很明确的看出某个Actor的继承关系是怎样的。

当我们创建一个ActorSystem的时候,AKKA会为该System默认的创建三个Actor：
- root guardian：所有Actor的 pairent.
- user actor:所有用户创建的Actor的 pairent，路径是 `/user` 。通过 `system.actorOf()` 创建出来的Actor都是 user actor 的 child.
- system actor:所有系统创建的Actor的 pairent,路径是 `/system` ,主要的作用是提供了一系列的系统的功能.

- actorOf：创建一个新的Actor。创建的Actor为调用该方法时所属的Context下的直接子Actor；
- actorSelection：当消息传递来时，只查找现有的Actor，而不会创建新的Actor；在创建了selection时，也不会验证目标Actors是否存在；
- actorFor（已经被actorSelection所deprecated）：只会查找现有的Actor，而不会创建新的Actor。

## Practice

### 创建Actor: 

（推荐的方式：） 定义 Actor class，并通过一个静态工场方法创建 Props instance，

```java
import akka.actor.UntypedAbstractActor;

//akka 2.5版本之后推荐使用AbstractActor并实现 createReceive() 方法，
public class MyActor extends UntypedAbstractActor {
    private int x;

    public MyActor(int x) {
        this.x = x;
    }
    
    /**
     * Create Props for an actor of this type.
     */
    public static Props props(final int x) {
        return Props.create(new Creator<MyActor>() {
            private static final long serialVersionUID = 1L;

            @Override
            public MyActor create() throws Exception {
                return new MyActor(x);
            }
        });
    }
    
    @Override
    public void onReceive(Object message) throws Throwable {
      xxx
    }
}
```

创建 Actor 例子:

```java
public class HelloWorld {
    public static void main(String args[]) {
        ActorSystem system = ActorSystem.create("myActorSystem");
        ActorRef actor = system.actorOf(MyActor.props(4), "newActor");
        actor.tell("good morning", ActorRef.noSender());
    }
}
```

### Stopping Actors

Actors are stopped by invoking the stop method of a `ActorRefFactory`, i.e. `ActorContext` or `ActorSystem`. Typically the context is used for stopping the actor itself or child actors and the system for stopping top level actors. The actual termination of the actor is performed asynchronously, i.e. stop may return before the actor is stopped.

Actor 会在处理完当前的的message之后停止，且不再处理之后的message。

停止的具体过程：
1. actor停止处理 mailbox，并且向所有的children发送 stop； 
2. 收集到所有 children 的停止信号之后，再终止自身。(调用`postStop()` 方法，dumping mailbox, publishing Terminated on the DeathWatch, telling its supervisor)

通过重载 `postStop()` 方法可以对资源进行清理, `postStop()` 方法会在actor彻底停止后被调用。

## ref:

https://doc.akka.io/docs/akka/2.5/actors.html


