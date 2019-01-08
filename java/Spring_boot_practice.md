
## 小白的 Spring boot practice

### @Autowired

Spring 的注入是基于实例化的 Object 的，因此 static fields 无法注入（结果会是null pointer）。
Autowired 和 static method/block 配合使用很容易出错。
（静态的域可以用 properties 文件配置。）

可以为 base class/interface 引用注入子类对象。如果存在多个实现可以使用 Qualifier 注解。
要被注入的类需要添加 Component 注解(Controller、Service都是 Component)。

Component 的查找路径是从 SpringBootApplication 类所处目录的子目录查找。
如果需要注入的类不在相应目录中，需要在 SpringBootApplication 的类上添加 ComponentScan 注释莱指明要进行查找的目录。

### 404

测试请求如果返回意外的 404 错误（hostname和改变端口号产生的是不一样的错误），可能原因：

- controller 路经配置错误、或请求的路经输错了。

- controller 类注解应为 @RestController;或者是@Controller+@ResponseBody

- controller 类没成功注入（没找到这个 Component）

### ref:

SpringBoot URL 映射: 

- https://segmentfault.com/a/1190000015761606

testing with mvc: 

- https://docs.spring.io/spring-boot/docs/current/reference/html/boot-features-testing.html

- https://docs.spring.io/spring/docs/current/spring-framework-reference/testing.html#spring-mvc-test-framework
