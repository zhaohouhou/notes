
# 配置中心(Spring Cloud Config)

分布式系统中服务数量多，各个服务的配置也随之增多，因此需要考虑配置管理的问题。
各个服务可能采用不同的语言、具有不同类型的配置文件，如果各自管理，会对维护和更新造成困难，配置中心是一个比较好的解决方案。

Spring Cloud Config 为外部化配置分布式系统的服务器和客户端提供了支持。

- Config Server 能够集中式地为各种环境的应用管理外部属性。
- 可与Spring应用良好结合，也能够用于任何语言开发的程序。
- 后端存储默认基于git，因此可以标记配置版本。
- 可选择其他存储实现方式，与Spring configuration结合。

Spring Cloud Config 组件中，有 config server 和 config client 两种角色。

## 1. 建立 Config Server

1. 创建一个spring-boot项目，取名为`config-server`, 添加pom依赖:

  ```xml
  <dependencies>
    <dependency>
      <groupid>org.springframework.cloud</groupid>
      <artifactid>spring-cloud-config-server</artifactid>
    </dependency>
    <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-starter-web</artifactId>
    </dependency>
  </dependencies>
  ```

2. 在程序的入口Application类加上`@EnableConfigServer`注解，开启配置服务器的功能：

3. 配置文件：

  ```python
  spring.application.name=config-server
  server.port=8501

  # git仓库地址
  spring.cloud.config.server.git.uri=https://github.com/XXX
  # 仓库路径
  spring.cloud.config.server.git.searchPaths=respo
  # 仓库分支
  spring.cloud.config.label=master
  # 用户名(私有仓库)
  spring.cloud.config.server.git.username=your username
  # 用户密码(私有仓库)
  spring.cloud.config.server.git.password=your password
  ```

  使config server能够从git仓库中的文件读取配置信息。

## 2. 建立 Config Client

1. 新建`config-client`项目，pom依赖：

  ```xml    
  <dependencies>
     <dependency>
       <groupid>org.springframework.cloud</groupid>
       <artifactid>spring-cloud-starter-config</artifactid>
     </dependency>
     <dependency>
       <groupId>org.springframework.boot</groupId>
       <artifactId>spring-boot-starter-web</artifactId>
     </dependency>
  </dependencies>
  ```

2. 项目的配置文件
<font color="red"><b>bootstrap.properties</b></font>：

  ```python
  spring.application.name=config-client
  # 远程仓库分支
  spring.cloud.config.label=master
  # 配置：dev-开发环境，test-测试环境，pro-正式环境
  spring.cloud.config.profile=dev
  spring.cloud.config.uri= http://localhost:8501/
  server.port=8601
  ```

  > Spring Cloud项目可以使用yml或properties文件进行配置，原理相同。
  >
  > **application.yml** or **application.properties** contains standard application configuration.
  >
  > **bootstrap.yml** or **bootstrap.properties** is only used/needed if you're using Spring Cloud and your application's configuration is stored on a remote configuration server (e.g. Spring Cloud Config Server).
  >
  > **bootstrap.yml** is loaded before **application.yml**.

3. 程序的入口类，API接口“／hi”，返回从配置中心读取的foo变量的值：

  ```java
  @SpringBootApplication
  @RestController
  public class ConfigClientApplication {
      public static void main(String[] args) {
          SpringApplication.run(ConfigClientApplication.class, args);
      }

      @Value("${foo}")
      String foo;
      @RequestMapping(value = "/hi")
      public String hi(){
          return foo;
      }
  }
  ```

  若git仓库中config-client-dev.properties文件保存了foo变量的配置，则程序可以通过config server读取配置信息（访问 http://localhost:8601/hi ）。

  **git仓库 => Config Server => Config Client**

## 3. 高可用的分布式配置中心

当系统中有很多服务需要从被指中心读取信息时，可将配置中心设为微服务集群，从而达到高可用。

可以使用一个 eureka server 作为 config server 和 config client 的服务注册中心，并可增加负载均衡等供能。

---

## ref

http://cloud.spring.io/spring-cloud-static/Edgware.RELEASE/single/spring-cloud.html#_spring_cloud_config

https://blog.coding.net/blog/spring-cloud-config （**Spring Cloud Config**）

http://book.itmuch.com/2%20Spring%20Cloud/2.5%20%E9%85%8D%E7%BD%AE%E4%B8%AD%E5%BF%83.html （配置中心）

http://blog.csdn.net/forezp/article/details/70037291 分布式配置中心

http://blog.csdn.net/forezp/article/details/70037513 高可用的分布式配置中心(Spring Cloud Config)

</br></br>
