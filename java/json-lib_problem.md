# net.sf.json库问题

json-lib是一个操作json对象的库，官网 http://json-lib.sourceforge.net/

maven依赖：

```xml
<dependency>
  <groupid>net.sf.json-lib</groupid>
  <artifactid>json-lib</artifactid>
  <version>2.4</version>
  <classifier>jdk15</classifier>
</dependency>
```

实际使用中可能会遇到一些问题，列举部分如下。

## 1. FunctionHeader问题

（JSONObject.fromObject()可能会遇到）
JSONArray对字符串进行反序列化时会使用JSONUtils.isFunctionHeader()检查字符串是否以"function"开头，因此如果json字符串以"function"开头可能会引起意料之外的结果（通常会抛异常）。

这可能是json-lib试图支持的json的一些特性引起的，没有搜索到相应描述。

## 2. 尝试解析JSONObject/JSONArray失败问题

参考连接 https://mccxj.github.io/blog/20150108_net-sf-json-problem.html

问题描述为json字符串以”{“开头，”}”结束，或者”[“开头,”]”结束的时候，中间存在”]”等字符导致解析失败。

暂时还没有遇到这个问题（使用`"{\"id\":[\"ab]c\"]}"`进行测试）。

## 3. 箭头字符串

某些箭头字符串（类似于"===>"）进行解析时产生了意料之外的效果(字符串解析为JSONArray)。由于记忆不清无法找到例子。

## fix

一些问题由json库实现引起，如希望解决，可以尝试使用其他json库。例如Gson（可靠性高且速度较快）或Jackson（速度最快）库。

Gson解析器maven配置：

```xml
<dependency>
  <groupId>com.google.code.gson</groupId>
  <artifactId>gson</artifactId>
  <version>2.8.1</version>
</dependency>
```

## ref:

http://www.cnblogs.com/windlaughing/p/3241776.html

https://mccxj.github.io/blog/20150108_net-sf-json-problem.html

http://vickyqi.com/2015/10/19/几种常用JSON库性能比较/
