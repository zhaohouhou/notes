# R语言集合运算

1. 求并集 union

  ```r
  A<-1:10
  B<-seq(5,15,2)
  C<-1:5
  union(A,B)
  ```
  输出：`1  2  3  4  5  6  7  8  9 10 11 13 15`


2. 求交集 intersect

    ```r
    intersect(A,B)
    ```
    输出：`5 7 9`

3. 多个集合
  ```
  Reduce(intersect,  list(v1 = c("a","b","c","d"),
              v2 = c("a","b","e"),
              v3 = c("a","f","g")))
  ```
