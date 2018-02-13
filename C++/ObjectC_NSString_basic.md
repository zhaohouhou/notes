
## NSString 基本使用

### 1. 赋值

```objc
//init with constant string
NSString *string = @"This is a String!";

//init with empty string
NSString *string = [[NSString alloc] init];

//init with string
NSString *string = [[NSString alloc] initWithString:@"This is a String!"];

//init from C string
char *cString = "This is a String!";
NSString *string = [[NSString alloc] initWithCString:cString];

//init from format string
NSString *string = [[NSString alloc] initWithString:
  [NSString stringWithFormat:@"%d.This is %i string!",1,2]];
```

### 2. NSString和char*之间转换

- NSString转换为char*

  ```objc
  NSString *string = @"This is a String!";
  const char *cstring=[string UTF8String];
  ```

- char*转换为NSString

  ```objc
  //init with C string
  char *cString = "This is a String!";
  NSString *string = [[NSString alloc]
    initWithCString:cString];
  ```

### 3. NSString与NSInteger，int，float间转换

```objc
//NSString to NSInteger
NSString *string = @"123456";
NSInteger integer = [string integerValue];
NSLog(@"NSInt value: %ld", integer);  //123456

//NSInteger to NSString
NSString *s = [NSString stringWithFormat:@"%ld",integer];

//NSString to int
int value = [string intValue];

//int to NSString
NSString *stringInt = [NSString stringWithFormat:@"%d",value];

//NSString to float
float valuef = [string floatValue];

//float to NSString
NSString *stringFloat = [NSString stringWithFormat:@"%f",valuef];
```

### 4. 字符串拼接

```objc
NSString *a = @"123";
NSString *a = @"abc";

//1
NSString *conjunct = [NSString stringWithFormat:@"%@%@", a, b];

//2
NSString *conjunct = [[NSString alloc] initWithFormat:@"%@%@", a, b];
```

## 编译Objective-C程序

Objective-C编写的.m文件可以使用clang进行编译:

    $ clang -framework Foundation main.m -o main

`-framework Fundation`表示引用Fundation框架(`#import <Foundation/Foundation.h>`)。

## Objective-C 内存管理

Objective-C 中，对象通常是使用 alloc 方法在堆上创建的。 [NSObject alloc] 方法在对堆上分配一块内存，并按照NSObject的内部结构进行填充。

Objective-C中提供了两种内存管理机制：**MRC(MannulReference Counting)** 和 **ARC(Automatic Reference Counting)**，分别提供对内存的手动和自动管理，来满足不同的需求。现在苹果推荐使用 ARC 来进行内存管理。

<table>
<thead><tr>
<th>对象操作</th>
<th>OC中对应的方法</th>
<th>retainCount 变化</th>
</tr></thead>
<tbody>
<tr>
<td>生成并持有对象</td>
<td>alloc/new/copy/mutableCopy等</td>
<td>+1</td>
</tr>
<tr>
<td>持有对象</td>
<td>retain</td>
<td>+1</td>
</tr>
<tr>
<td>释放对象</td>
<td>release</td>
<td>-1</td>
</tr>
<tr>
<td>废弃对象</td>
<td>dealloc</td>
<td>-</td>
</tr>
</tbody>
</table>

## ref:

http://www.cnblogs.com/lovekarri/articles/2380033.html NSString的常用用法

https://segmentfault.com/a/1190000004943276 Objective-C 内存管理

<br/><br/>
