
# `@RequestMapping` 基本使用

`@RequestMapping`是Spring框架用来处理请求地址映射的注解。
`@RequestMapping`有以下几种属性：

- value：指定请求的实际地址；

- method：指定请求的method类型， GET、POST、PUT、DELETE等；

- consumes：指定处理请求的提交内容类型（Content-Type），例如application/json, text/html;

- produces: 指定返回的内容类型

- params： 指定request中必须包含的某些参数。

- headers：指定request中必须包含某些指定的header值

下面的示例中，函数`hi()`处理路径为`"/hi"`的GET请求，需要传入一个参数`name`：

```java
@RequestMapping(value = "/hi", method = RequestMethod.GET)
public String hi(@RequestParam String name){
    return "hello " + name;
}
```

### value的uri值可以有以下三类：

1. 具体的路径值

2. 含有变量的URI模板(URI Template Patterns with Path Variables)

    ```java
    @RequestMapping(value="/list{name}")    
    public void handleName(@PathVariable String name){
      return ("name: " + name);
    }
    ```

    ```java
    @RequestMapping(value="/departments/{departmentId}/employee/{employeeId}")
    public void handle(    
      @PathVariable String departmentId,    
      @PathVariable String employeeId){
        return ("ID: " + employeeId + ", dept.: " + departmentId);     
    }
    ```

3. 正则表达式表示的一类值(URI Template Patterns with Regular Expressions)

    ```java
    @RequestMapping(value="/path/{textualPart:[a-z-]+}.{numericPart:[\\d]+}")    
    public void handleRE(@PathVariable String textualPart,
        @PathVariable String numericPart){      
      return ("Textual part: " + textualPart + ", numeric part: " + numericPart);     
    }   
    ```

### 处理多个路径的请求

`@RequestMapping`的value可以是多个：

    @RequestMapping(value={"url1","url2","url3"....})



## ref:

https://www.cnblogs.com/qq78292959/p/3760560.html `@RequestMapping` 基本使用

http://blog.csdn.net/u012480620/article/details/52228393 可变value值

<br/><br/>
