
# Spring Cloud 简介

Spring Cloud 为开发人员提供了快速构建分布式系统的一些工具，包括配置管理、服务发现、断路器（circuit breakers）、智能路由、微代理、事件总线、one-time tokens、全局锁、决策竞选、分布式会话等等。Spring Cloud能够减少分布式开发中出现的大量重复性功能代码，并且能够良好地运行在多种环境中。Spring Cloud 是基于 Spring Boot 的。

## Spring Boot

Spring Boot 致力于建立生产就绪（production-ready）的Spring程序，偏重“convention over configuration（惯例优先于配置）”，让开发者写更少的配置，程序能够更快的运行和启动。它是下一代javaweb框架，并且是spring cloud（微服务）的基础。

特性：

- 建立独立的Spring应用
- 直接集成了 Tomcat、Jetty 或 Undertow ，不再需要部署 WAR 包。
- 提供武断的初始POM来简化Maven配置
- 尽可能地自动配置Spring
- Provide production-ready features such as metrics, health checks and externalized configuration、
- no code generation and no requirement for XML configuration

## 微服务（microservices）

微服务架构：一种架构观念，旨在通过将功能分解到各个离散的服务中以实现对解决方案的解耦。微服务使得大型复杂应用能够连续地交付，提供更加灵活的服务支持。

与传统web开发方式的比较：

<table>
<tr>
<th></th>  <th>传统web开发</th>  <th>微服务架构</th>
</tr>
<tr>
<td> 服务</td>
<td> 所有的功能打包在一个WAR包里，基本没有外部依赖。部署在一个JEE容器（Tomcat，JBoss，WebLogic）里，包含了DO/DAO，Service，UI等所有逻辑。</td>
<td> 一些列的独立的服务单独部署，跑在自己的进程中；每个服务为独立的业务开发。分布式管理。</td>
</tr>
<tr>
<td> 优点 </td>
<td>
<li> 开发简单，集中式管理；</li>
<li> 基本无重复开发；</li>
<li> 功能都在本地，没有分布式的管理和调用消耗</li>
</td>
<td> 敏捷开发和部署、技术选择灵活。每个服务可独立开发、部署和调整。高度容错性和可用性。</td>
</tr>
<tr>
<td> 缺点 </td>
<td>
<li> 开发简单，集中式管理；</li>
<li> 基本无重复开发；</li>
<li> 功能都在本地，没有分布式的管理和调用消耗</li>
</td>
<td>
<li> 分布式系统而产生的复杂性。开发人员需要选择和实现基于消息传递或RPC的进程间通信机制。</li>
<li> 可能由于有多个数据库，需要考虑如何保证数据一致性。</li>
<li> 测试、更改、部署服务时的复杂性。</li>
</td>
</tr>
</table>

相关问题：

- 服务间通信： 微服务都是独立的Java进程跑在独立的虚拟机上，所以服务间的通信是IPC（inter process communication）。通用的方式：
  - 同步调用：简单、一致性强，但容易出现调用问题，特别是调用层次多时，性能体验较差。
    - REST（JAX-RS，Spring Boot）：一般基于HTTP，更容易实现、服务端实现技术更灵活，各个语言都能支持，能够跨客户端，因此使用广泛。
    - RPC（Thrift, Dubbo）：传输协议更高效，安全更可控，有统一的开发规范和服务框架时，开发效率更有优势。
  - 异步消息调用（Kafka, Notify, MetaQ）：异步消息方式在分布式系统中有广泛的应用。能够降低调用服务之间的耦合，使调用方无需停止等待，但解决数据一致性的问题，存在技术上的挑战。


- 实现： 一般每个服务有多个拷贝，用来做负载均衡。一个服务随时可能下线，也可能应对临时访问压力增加新的服务节点。
  - 服务发现：服务信息注册管理。


- 服务容错机制：重试、限流、熔断、负载均衡、降级（本地缓存）


## ref:

http://book.itmuch.com/ (sprint cloud)

http://blog.csdn.net/forezp/article/details/69696915 (sprint cloud)

http://projects.spring.io/spring-cloud/#quick-start

https://projects.spring.io/spring-boot/

http://www.cnblogs.com/wintersun/p/6219259.html 微服务架构设计

http://www.cnblogs.com/imyalost/p/6792724.html (microservice)

http://microservices.io/

</br></br>
