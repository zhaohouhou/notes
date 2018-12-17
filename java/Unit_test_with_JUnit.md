
# 用 junit 进行单元测试

## 傻瓜版

1） 首先添加项目依赖，例如在 gradle 中， build.gradle 中添加：

```
dependencies {
    testCompile group: 'junit', name: 'junit', version: '4.12'
}
```

2） code

```java
import static org.junit.Assert.*;
import org.junit.Test;

class TestSampleClass{

@Test
void sampleTestFunction(){
  ...
  assertEquals(result, expect);
}

...
}

```

## Basic annotations

- Test - Marks the method as a test method.
- `@Before` and `@After` sandwiches each test method in the class.
- `@BeforeClass` and `@AfterClass` sandwiches all of the test methods in a JUnit test class.

execution order:
1. Method annotated with `@BeforeClass`
2. Method annotated with `@Before`
3. First method annotated with `@Test`
4. Method annotated with `@After`
5. Method annotated with `@Before`
6. Second method annotated with `@Test`
7. Method annotated with `@After`
8. Method annotated with `@AfterClass`

## Assertions、Exceptions testing、 Parameterized tests

## 实现 SetUp / TearDown manner

通过 annotation 可以很方便地实现 SetUp/TearDown 模式， （`@BeforeClass`，`@AfterClass`）。

another way：

```java
import junit.framework.TestCase;
	public class JUnitTestCaseWOAnnotation extends TestCase {
    private AccountService accountService = new AccountService();
    private Account dummyAccount;
    
    @Override
    protected void setUp() throws Exception {
        System.out.println("Setting it up!");
        dummyAccount = accountService.getAccountDetails();
    }

    public void testDummyAccount() {
        System.out.println("Running: testDummyAccount");
        assertNotNull(dummyAccount.getAccountCode());
    }

    @Override
    protected void tearDown() throws Exception {
        System.out.println("Running: tearDown");
        dummyAccount = null;
        assertNull(dummyAccount);
    }
}
```

