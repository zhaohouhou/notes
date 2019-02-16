
## 小白的 Spring boot practice

### @Autowired

Spring 的注入是基于实例化的 Object 的，因此 static fields 无法注入（结果会是null pointer）。
Autowired 和 static method/block 配合使用很容易出错。
（静态的域可以用 properties 文件配置。）

可以为 base class/interface 引用注入子类对象。如果存在多个实现可以使用 Qualifier 注解。
要被注入的类需要添加 Component 注解(Controller、Service都是 Component)。

Component 的查找路径是从 SpringBootApplication 类所处目录的子目录查找。
如果需要注入的类不在相应目录中，需要在 SpringBootApplication 的类上添加 ComponentScan 注释莱指明要进行查找的目录。

### `@ResponseBody` and `ResponseEntity`

- `@ResponseBody` 注解：在方法上添加该注解，Spring 会将函数返回的对象转换成 http response body。

- `ResponseEntity`：和 `@ResponseBody` 作用类似。通过 `ResponseEntity` 可以灵活地定义其他 response 信息，
例如 header 和 status code 等。

- 如果使用了 `ResponseEntity` 作为返回对象，则不需要载使用 `@ResponseBody` 注解。

例子：

```java
@RequestMapping(value = "/message")
@ResponseBody
public Message get() {
    return new Message("AAA");
}

@RequestMapping(value = "/message")
ResponseEntity<Message> get() {
    Message message = new Message("BBB");
    return new ResponseEntity<Message>(message, HttpStatus.OK);
}
```

### 404

测试请求如果返回意外的 404 错误（hostname和改变端口号产生的是不一样的错误），可能原因：

- controller 路经配置错误、或请求的路经输错了。

- controller 类注解应为 @RestController;或者是@Controller+@ResponseBody

- controller 类没成功注入（没找到这个 Component）


### BeanFactory 创建 bean

下面的方法可以在运行时建立单例的 bean，并且设置一些参数。

1. bean class （with a 'name' field）

```java
public class PrototypeBean {
    private String name;
     
    public PrototypeBean(String name) {
        this.name = name;
        logger.info("Prototype instance " + name + " created");
    }
 
    //...
}
```

2. inject a bean factory into a singleton bean by making use of the `java.util.Function` interface

```java
public class SingletonFunctionBean {
     
    @Autowired
    private Function<String, PrototypeBean> beanFactory;
     
    public PrototypeBean getPrototypeInstance(String name) {
        PrototypeBean bean = beanFactory.apply(name);
        return bean;
    }
}
```

3. define the factory bean, prototype and singleton beans in configuration:

```java
@Configuration
public class AppConfig {
    @Bean
    public Function<String, PrototypeBean> beanFactory() {
        return name -> prototypeBeanWithParam(name);
    } 
 
    @Bean
    @Scope(value = "prototype")
    //singleton：单例模式，在整个Spring IoC容器中，使用singleton定义的Bean将只有一个实例
    //prototype：原型模式，每次通过容器的getBean方法获取prototype定义的Bean时，都将产生一个新的Bean实例
    //request：对每次HTTP请求，使用request定义的Bean都将产生一个新实例。在Web应用中使用Spring时该作用域才有效
    public PrototypeBean prototypeBeanWithParam(String name) {
       return new PrototypeBean(name);
    }
     
    @Bean
    public SingletonFunctionBean singletonFunctionBean() {
        return new SingletonFunctionBean();
    }
    //...
}
```


### ref:

SpringBoot URL 映射: 

- https://segmentfault.com/a/1190000015761606

testing with mvc: 

- https://docs.spring.io/spring-boot/docs/current/reference/html/boot-features-testing.html

- https://docs.spring.io/spring/docs/current/spring-framework-reference/testing.html#spring-mvc-test-framework

https://www.baeldung.com/spring-inject-prototype-bean-into-singleton
