#  Java 8 CompletableFuture 使用

## 创建

```java
// 1.
CompletableFuture<Integer> future = new CompletableFuture<>();
// do sth. with "future"
return future;

// 2. 返回已经计算好的future
public static <U> CompletableFuture<U> completedFuture(U value)

// 3. 创建异步过程
public static CompletableFuture<Void> 	runAsync(Runnable runnable)
public static CompletableFuture<Void> 	runAsync(Runnable runnable, Executor executor)
public static <U> CompletableFuture<U> 	supplyAsync(Supplier<U> supplier)
public static <U> CompletableFuture<U> 	supplyAsync(Supplier<U> supplier, Executor executor)
```

## 完成计算

```java
// 正常完成
future.complete(100);

// 异常完成
future.completeExceptionally(new Exception());
```

## 阻塞地获取结果（转同步）

```java
public T 	get()
public T 	get(long timeout, TimeUnit unit)
public T 	getNow(T valueIfAbsent)
public T 	join()
```

## 异步地处理结果（正常和异常）

```java
public CompletableFuture<T> 	whenComplete(BiConsumer<? super T,? super Throwable> action)
public CompletableFuture<T> 	whenCompleteAsync(BiConsumer<? super T,? super Throwable> action)
public CompletableFuture<T> 	whenCompleteAsync(BiConsumer<? super T,? super Throwable> action, Executor executor)
public CompletableFuture<T>     exceptionally(Function<Throwable,? extends T> fn)
```

## 进一步处理 future 的结果（正常）

### 转换

```java
public <U> CompletableFuture<U> 	thenApply(Function<? super T,? extends U> fn)
public <U> CompletableFuture<U> 	thenApplyAsync(Function<? super T,? extends U> fn)
public <U> CompletableFuture<U> 	thenApplyAsync(Function<? super T,? extends U> fn, Executor executor)
```

### 消费

```java
public CompletableFuture<Void> 	thenAccept(Consumer<? super T> action)
public CompletableFuture<Void> 	thenAcceptAsync(Consumer<? super T> action)
public CompletableFuture<Void> 	thenAcceptAsync(Consumer<? super T> action, Executor executor)
```

### 不依赖结果

```java
public CompletableFuture<Void> 	thenRun(Runnable action)
public CompletableFuture<Void> 	thenRunAsync(Runnable action)
public CompletableFuture<Void> 	thenRunAsync(Runnable action, Executor executor)
```

### 组合

```java
public <U> CompletableFuture<U> 	thenCompose(Function<? super T,? extends CompletionStage<U>> fn)
public <U> CompletableFuture<U> 	thenComposeAsync(Function<? super T,? extends CompletionStage<U>> fn)
public <U> CompletableFuture<U> 	thenComposeAsync(Function<? super T,? extends CompletionStage<U>> fn, Executor executor)
```



</br></br>
