# Using Mockito with JUnit

Mock 是单元测试中常见的一种技术，主要作用是模拟一些在应用中不容易构造或者比较复杂（例如消耗的资源或时间较多）的对象，从而隔离地测试不同的 class。Mockito 是一个基于 Java 的 mocking framework，用于高效地进行单元测试。

## Mockito

Mockito 使用 java 反射来对指定的 interface 生成 mock objects。Mockito 支持返回值、exceptions 、检查方法的调用顺序，支持通过 annotation 创建 mock。

Requirement: JDK 1.5 or above.

## An Example

以一个最简单的例子说明 Mockito 的使用

### Step 1: Impl. in Dependency-Injection manner

定义 interface：

```java
public interface ServiceA {
  public int doTheThing(int input);
}
```

（接口的实现省略。）要测试的类：

```java
public class Application {
  private ServiceA serviceA;

  public void setServiceA(ServiceA serviceA) {
    this.serviceA = serviceA;
  }

  public int doSomething(int input) {
    return this.serviceA.doTheThing(input) + 1;
  }
}
```

### Step 2: create a JUnit test class

```java
import org.junit.Assert;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.runners.MockitoJUnitRunner;

import static org.mockito.Mockito.*;

// @RunWith attaches a runner with the test class to initialize the test data
// Alternatively, we can activate annotations by invoking MockitoAnnotations.initMocks()
@RunWith(MockitoJUnitRunner.class)
public class MathApplicationTester {

   //@InjectMocks annotation is used to create and inject the mock object
   @InjectMocks
   Application application = new Application();

   //@Mock annotation is used to create the mock object to be injected
   @Mock
   ServiceA serviceA;

   @Before
   public void init() {
     //
      MockitoAnnotations.initMocks(this);
   }

   @Test
   public void test(){
      //add the behavior of calc service to add two numbers
      when(serviceA.doSomething(1).thenReturn(1);

      //test the add functionality
      assert(application.doSomething(1) == 2);
   }
}
```

## Create Mock

创建 Mock 的两种方式：

1. call `Mockito.mock`

    ```java
    List mockList = Mockito.mock(ArrayList.class);
    ```

2. using the `@Mock` annotation:

    ```java
    @Mock
    List<String> mockedList;
    ```

Mockito Annotations 需要通过上面例子中的 `@RunWith(MockitoJUnitRunner.class)` 来激活；或者调用 `MockitoAnnotations.initMocks()`：

```java
@Before
public void init() {
    MockitoAnnotations.initMocks(this);
}
```

## @Spy

使用 Mokito.spy() 可以在把持该对象的所有正常方法调用的同时，像mock对象一样对其配置一些行为。Example：

```java
@Spy
List<String> spyList = new ArrayList<String>();
// 或
// List<String> spyList = Mockito.spy(new ArrayList<String>());

@Test
public void test() {
    spyList.add("one");
    spyList.add("two");

    Mockito.verify(spyList).add("one");
    Mockito.verify(spyList).add("two");
    assertEquals(2, spyList.size());

    // 打桩
    doReturn(100).when(spyList).size();
    assertEquals(100, spyList.size());
}
```

Mock vs. Spy：
- Mock 是从 class 创建
- Spy 是从一个实例创建，会真正调用对象的方法（产生影响）


## @Captor

todo


## Adding Behavior

1. `when` & `then` pattern

  ```java
  when(dao.save(customer)).thenReturn(true);
  when(dao.save(any(Customer.class))).thenThrow(RuntimeException.class);
  ```

2. `do` & `when`

  ```java
  doReturn(1).when(foo).bar();
  doNothing().when(foo).bar(1, 2);
  doThrow(RuntimeException.class).when(foo).bar(any(String.class));
  ```

##  Verifying Behavior

Mockito.verify 可以对性为进行验证：

1. `verify(mockObj)`:  we can use this if the method is supposed to be invoked only once.

  ```java
  List mockedList = mock(List.class);

  mockedList.add("one");

  //verification
  verify(mockedList).add("one");
  ```
 
2. `verify(mockObj, VerificationMode)`:
    - `times(int wantedNumberOfInvocations)`
    - `atLeast( int wantedNumberOfInvocations )`
    - `atMost( int wantedNumberOfInvocations )`
    - `calls( int wantedNumberOfInvocations )`
    - `only( int wantedNumberOfInvocations )`
    - `atLeastOnce()`
    - `never()`

    ```java
    Customer customer = new Customer();
    assertThat(service.addCustomer(customer), is(false));
    verify(daoSpy).save(any(Customer.class));
    verify(daoSpy, times(1)).exists(anyString());
    verify(daoSpy, never()).delete(any(Customer.class));
    ```

Note: `verify()` 不会重置方法的状态计数，可以使用 `reset(mockObject)` 重置状态。 (尽量避免)


## ref:

https://www.tutorialspoint.com/mockito/index.htm

https://javacodehouse.com/blog/mockito-tutorial/


<br/><br/>
