# Interface Stream<T>

## <U> U reduce(U identity, BiFunction<U,? super T,U> accumulator, BinaryOperator<U> combiner)

Performs a reduction on the elements of this stream, using the provided **identity**, **accumulation** and **combining functions**.

reduce 操作会将 T 类型的 stream 元素归并为一个 U 类型的值。三个主要概念:

- **accumulator** : 累加器，用于给 reduce 操作提供新的值。（将"右边"的 T 类型值累加到"左边"的 U 类型）

- **identity** : 提供给 accumulator 的初始值（accumulator "左边"的 U 的默认值）。

- **combiner** : 聚合 accumulator 的结果。combiner 只有并行模式会调用。

示例代码：


```java
String reducedParams = Stream.of(1, 2, 3)
  .reduce(
    "@", 
    (str, num) -> str + String.valueOf(num), 
    (str1, str2) -> {
     return str1 + "@" + str2;
  });
//=> "@123"

String reducedParams = Arrays.asList(1, 2, 3).parallelStream()
            .reduce(
                    "@",
                    (str, num) -> str + String.valueOf(num),
                    (str1, str2) -> {
                        return str1 + "@" + str2;
                    });
//=> "@1@@2@@3"
```



## ref:

https://www.baeldung.com/java-8-streams

</br></br>
