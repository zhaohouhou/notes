# java枚举类型（enum）

## 1. 定义

枚举类型使用关键字enum定义，隐含了所创建的类是 java.lang.Enum 类的子类（java.lang.Enum 是一个抽象类）。枚举类型的每一个值都将映射到 protected Enum(String name, int ordinal) 构造函数中，这里，每个值的名称都被转换成一个字符串，并且序数设置表示了此设置被创建的顺序。

```java
public enum EnumTest {
    MON, TUE, WED, THU, FRI, SAT, SUN;
}
```

这段代码实际上调用了7次 Enum(String name, int ordinal)：

```java
new Enum<EnumTest>("MON",0);
new Enum<EnumTest>("TUE",1);
new Enum<EnumTest>("WED",2);
    ... ...

```

## 2. 遍历和switch

- 使用Enum.values()获得包含所有值的Collection。


- switch Enum类型：

```java
EnumTest test = EnumTest.TUE;
switch (test) {
        case MON:
            System.out.println("今天是星期一");
            break;
        case TUE:
            System.out.println("今天是星期二");
            break;
        // ... ...
        default:
            System.out.println(test);
            break;
        }
```

## 3. Enum常用方法介绍

- `int compareTo(E o)`
          比较此枚举与指定对象的顺序。

- `Class<E> getDeclaringClass()`
          返回与此枚举常量的枚举类型相对应的 Class 对象。

- `String name()`
          返回此枚举常量的名称，在其枚举声明中对其进行声明。

- `int ordinal()`
          返回枚举常量的序数（它在枚举声明中的位置，其中初始常量序数为零）。

- `String toString()` 返回枚举常量的名称。

- `static <T extends Enum<T>> T valueOf(Class<T> enumType, String name)`
          返回带指定名称的指定枚举类型的枚举常量。

可以自定义一些属性和构造方法。

<br/>
------------


ref:

http://www.cnblogs.com/hemingwang0902/archive/2011/12/29/2306263.html

<br/>  
<br/>
