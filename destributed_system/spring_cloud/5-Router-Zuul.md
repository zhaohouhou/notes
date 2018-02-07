
# 路由：Zuul

路由（routing）是微服务架构的一部分。在微服务架构中，每一个微服务暴露一组细粒度的服务提供点，如果客户端直接与微服务通信，则会面临客户端代码过于复杂、微服务协议受限、难以重构微服务等问题；因此，更好的解决办法是采用API Gateway的方式。

#### API Gateway

API Gateway是一个服务器，用来封装系统内部的架构。API Gateway负责请求转发、合成和协议转换，所有来自客户端的请求都要先经过API Gateway，然后路由这些请求到对应的微服务。API Gateway通过调用多个微服务来处理一个请求以及聚合多个服务的结果。

Spring Cloud中使用 **Zuul** 作为API Gateway，提供了动态路由、监控、回退、安全等的边缘服务(edge service)。

## 1. 创建`service-zuul`工程

在pom.xml中添加依赖：

```xml
<dependencies>
  <dependency>
    <groupId>org.springframework.cloud</groupId>
    <artifactId>spring-cloud-starter-zuul</artifactId>
  </dependency>
  <dependency>
    <groupId>org.springframework.cloud</groupId>
    <artifactId>spring-cloud-starter-eureka</artifactId>
  </dependency>
</dependencies>
```

启动类加注解`@EnableZuulProxy`，开启Zuul功能：

```java
@SpringBootApplication
@EnableZuulProxy
public class ZuulApiGatewayApplication {
  public static void main(String[] args) {
    SpringApplication.run(ZuulApiGatewayApplication.class, args);
  }
}
```

配置项：

```python
# 指定服务注册中心
eureka.client.service-url.defaultZone=http://localhost:8001/eureka/
# 指定端口号
server.port=8401
# 服务名
spring.application.name=service-zuul
# 以'/ribbon/'开头的请求转发给service-ribbon服务
zuul.routes.ribbon.path=/ribbon/**
zuul.routes.ribbon.serviceId=service-ribbon
# 以'/feign/'开头的请求转发给service-feign服务
zuul.routes.feign.path=/feign/**
zuul.routes.feign.serviceId=service-feign
```

依次启动 eureka server项目、`service-hi`、`service-ribbon`、`service-feign`、`service-zuul`。浏览器访问 http://localhost:8401/ribbon/hi?name=xxx 和 http://localhost:8401/feign/hi?name=xxx ， 可分别使用`service-ribbon`和`service-feign`服务。这里，zuul起到了路由的作用。

## 2. 服务过滤

Zuul还可以对请求进行过滤，做一些安全验证。在项目中添加Filter类对请求进行过滤。下面的代码实现了ip地址过滤。

```java
@Component
public class MyFilter extends ZuulFilter{
    private static Logger log = LoggerFactory.getLogger(MyFilter.class);
    @Override
    public String filterType() {
        return "pre";
    }

    @Override
    public int filterOrder() {
        return 0;
    }

    @Override
    public boolean shouldFilter() {
        return true;
    }

    @Override
    public Object run() {
      RequestContext context = RequestContext.getCurrentContext();
      HttpServletRequest request = context.getRequest();
      String requestURL = request.getRequestURL().toString();
      String ip = getIpFromRequest(request);
      if(!MyConfig.isIpPertmitted(ip)){ //自定义配置类
          log.warn(String.format("Forbidden ip address: %s", ip));
          context.setSendZuulResponse(false);
          context.setResponseStatusCode(401);
          try {
              context.getResponse().getWriter().write("Forbidden ip address");
          }
          catch (Exception e){}
          return null;
        }
        log.info("ok");
        return null;
    }

    private String getIpFromRequest(HttpServletRequest request) {
        String ip;
        if ((ip = request.getHeader("x-forwarded-for")) != null) {
            StrTokenizer tokenizer = new StrTokenizer(ip, ",");
            while (tokenizer.hasNext()) {
                ip = tokenizer.nextToken().trim();
            }
        }
        else {
            ip = request.getRemoteAddr();
        }
        return ip;
    }
}
```

ZuulFilter类需要实现的函数：

  - filterType：返回一个字符串代表过滤器的类型。Zuul中定义了四种生命周期的过滤器类型：

    - pre：路由之前
    - routing：路由之时
    - post： 路由之后
    - error：发送错误调用


  - filterOrder：过滤的顺序

  - shouldFilter：是否需要过滤。可以在这里编写逻辑判断

  - run：过滤器的具体逻辑。可以比较复杂，例如查寻数据库判断请求是否有权限访问。

---

## ref

http://cloud.spring.io/spring-cloud-static/Edgware.RELEASE/single/spring-cloud.html#_router_and_filter_zuul

http://dockone.io/article/482 （API Gateway ）

http://book.itmuch.com/2%20Spring%20Cloud/2.6%20API%20Gateway.html （API Gateway：Zuul）

http://blog.csdn.net/forezp/article/details/69939114  路由网关(Zuul)

https://zhuanlan.zhihu.com/p/28376627  深入理解Zuul之源码解析

</br></br>
