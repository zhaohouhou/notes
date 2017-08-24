# Python basic

## 1. 基本数据类型

Python包含5种基本类型：

- Numbers 数字

- String 字符串

- List 列表

- Tuple 元组

- Dictionary 字典


## 2. 基本操作

### 赋值

变量的赋值决定变量类型。python可以同时为多个变量赋值：

```python
a, b, c = 1, 12.5, 'hello'
```

### 字符串操作

```python
string = "Hello"
print string      # print entire string
print string[0]   # print string-element by index
print string[1:4] # print string-slice from 2nd element to 4th
print string[3:]  # print string starting from the 4th element
print string * 3  # print string 3 times (string duplicates)
print string + "123"  # string concat
print u'汉字用unicode'  # Chinese charactor encoding
```

运行结果：

```
Hello
H
ell
lo
HelloHelloHello
Hello123
```

## 3. 基本运算

### 算数运算符

| Synbol | Operation |
| -- | -- |
|  `+, -, *`  |  加、减、乘运算  |
|  `/`  |  除法：整数相除返回取整后的结果。若要小数结果，则除数被除数至少有一个是浮点数。  |
| `%` | 取模（除法的整数部分） |
| `//` | 取除法的小数部分 |
| `**` | `x**y` 表示x的y次幂 |

示例代码：

```python
print 1 / 2
print 1.0 / 2
print 14 % 5
print 14 // 5
print 2 ** 3
```

运行结果：

```
0
0.5
4
2
8
```

### 逻辑运算符

| Synbol | Operation |
| -- | -- |
|  `==, !=`  |  相等、不等判断  |

</br></br>
