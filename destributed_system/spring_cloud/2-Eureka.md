
#  服务的注册与发现（Eureka）

在微服务架构中，服务发现（Service Discovery）是关键原则之一。手动配置是十分困难的。Spring Cloud提供了多种服务发现的实现方式，例如：Eureka、Consul、Zookeeper，其中支持得最好的是Eureka。

demo简要步骤：

### 1. 首先创建一个Maven主工程

### 2. 创建Eureka Server

首先建立Eureka Server的model工程：

- intelij：右键工程 -> 创建model -> spring initializr。下一步 -> cloud discovery -> eureka server

- 启动一个服务注册中心：在spring boot工程的启动application类(`@SpringBootApplication`)上加注解：`@EnableEurekaServer`。

- 修改eureka server配置文件，使

        server:
          port: 8001

        eureka:
          instance:
            hostname: localhost
          client:
            registerWithEureka：false
            fetchRegistry：false
            ...

  （指明其为一个eureka server，而不是一个eureka client）

- 启动后可以浏览器访问eureka server的界面：
http://localhost:8001

###  3. 创建一个服务提供者 (eureka client)：

- Eureka server 从每个client实例接收心跳消息，如果心跳超时，则通常将该实例从注册server中删除。创建过程同server类似。    

- application类(`@SpringBootApplication`)上注解`@EnableEurekaClient`，表明自身是一个eureka client。代码示例：

  ```java
  @SpringBootApplication
  @EnableEurekaClient
  @RestController
  public class ServiceHiApplication {

      public static void main(String[] args) {
          SpringApplication.run(ServiceHiApplication.class, args);
      }

      @Value("${server.port}")
      String port;

      @RequestMapping("/hi")
      public String home(@RequestParam String name) {
          return "hi "+name+",i am from port:" +port;
      }

  }
  ```

- 在配置文件中注明的地址：

      eureka:
        client:
          serviceUrl:
            defaultZone: http://localhost:8001/eureka/
        server:
        port: 8101
        spring:
        application:
          name: service-hi

- 启动client后，访问 eureka server 网址，可以看到Application中已经有一个注册的服务。此时访问 http://localhost:8101/hi?name=xxx ，可以看到返回信息：`hi xxx,i am from port:8101`.



## ref:

http://book.itmuch.com

http://blog.csdn.net/forezp/article/details/69696915

</br></br>
