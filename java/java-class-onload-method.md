# java类加载时执行方法

执行顺序：

1. 类加载时: static代码块。顺序：母 => 子

2. 实例化时：代码块 => 构造函数； 母 => 子

因此，若希望某个类加载时执行某些代码，可写在static块中。


**示例代码**：

```java
class HelloA {
    public HelloA() {
        System.out.print("A");
    }

    {
        System.out.print("B");
    }

    static {
        System.out.print("C");
    }
}

public class HelloB extends HelloA {

    public HelloB() {
        System.out.print("D");
    }

    {
        System.out.print("E");
    }

    static {
        System.out.print("F");
    }

    public static void main(String[] args) {
        System.out.print("G");
        new HelloB();
        new HelloB();
        System.out.print("H");
    }
}
```

结果：CFGBAEDBAEDH


<br/>

---------

ref:

http://blog.csdn.net/gdl116929569/article/details/49338011

<br/><br/>
