
# Java缓存框架（Ehcache）

开发大型系统时，缓存能够起到减少服务间请求次数、缓解数据访问瓶颈、提高响应速度等重要作用。Java语言有很多缓存框架，例如Ehcache、JCache API、JBoss Cache（Java object caching）、Redis（键值对存储）等。下面主要是EHCache的使用。

## EhCache 简介

Ehcache是一个纯Java的开源分布式缓存框架，能够与Spring、Hibernate良好整合。
Ehcache具有以下特点：

- 快速简单，容易集成。     
- 支持多种缓存策略。     
- 两级缓存：内存和磁盘。     
- 缓存数据会在虚拟机重启的过程中写入磁盘。
- 可以通过RMI、可插入API等方式进行分布式缓存。     
- 具有缓存和缓存管理器的侦听接口。
- 支持多缓存管理器实例，以及一个实例的多个缓存区域
- 提供 Hibernate 的缓存实现  ...

## EhCache 基本使用

### 1. maven配置

pom.xml文件中增加：

```xml
<dependency>
  <groupId>net.sf.ehcache</groupId>
  <artifactId>ehcache</artifactId>
</dependency>
```

（IDEA中可能需要右键项目：maven -> reimport ，不然无法`import net.sf.ehcache.*;`）

### 2. 配置文件

Ehcache支持多种配置方式，可以通过声明配置、在xml中配置、在程序里配置或者调用构造方法时传入不同的参数。常用的方式是使用xml文件进行配置。例如在src文件夹建立ehcache.xml文件:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<ehcache xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
   xsi:noNamespaceSchemaLocation="http://ehcache.org/ehcache.xsd">
    <diskStore path="java.io.tmpdir"/>
    <!-- 默认配置 -->
    <defaultCache
      maxElementsInMemory="10000"
      eternal="false"
      timeToIdleSeconds="30"
      timeToLiveSeconds="30"
      overflowToDisk="false"/>
    <!--  自定义配置 -->
    <cache name="testCache"
      maxElementsInMemory="10000"
      eternal="false"
      overflowToDisk="false"
      timeToIdleSeconds="900"
      timeToLiveSeconds="1800"
      memoryStoreEvictionPolicy="LFU" />
    <!-- 参数说明：
      maxElementsInMemory：缓存中允许创建的最大对象数
      eternal：缓存中对象是否为永久的. 如果是, 则超时设置被忽略
      timeToIdleSeconds：数据钝化时间(经过该时间未被访问则失效).
          非永久对象有效. 0=infinite
      timeToLiveSeconds：数据生存时间(无论被访问与否).
          非永久对象有效. 0=infinite
      overflowToDisk：内存不足时，是否启用磁盘缓存
      memoryStoreEvictionPolicy：淘汰算法。例如 LRU, LFU, FIFO
    -->
</ehcache>
```

## 3. 基本使用

测试代码：

```java
import junit.framework.Assert;
import junit.framework.TestCase;
import net.sf.ehcache.*;

/** Created by zd on 1/19/18. */
public class EhcacheTest extends TestCase
{
    public void testCache()
    {
        CacheManager manager = CacheManager.newInstance(
                          "src/main/resources/encache.xml");
        String[] cacheNames = manager.getCacheNames();  //["testCache"]
        Cache test = manager.getCache("testCache"); //前面配置文件里的名字
        test.put(new Element("key1", "value1"));
        Assert.assertTrue(test.isElementInMemory("key1"));  //pass
        Assert.assertEquals(test.get("key1").getObjectValue(),"value1");  //pass
        manager.shutdown();
    }
}
```

EhCache配置参数可以动态修改。此外官方文档中提到：

- Configuration lifecycle can be separated from application-code lifecycle.
- Configuration errors are checked at startup rather than causing an unexpected
runtime error.
- **If the configuration file is not provided, a default configuration is always loaded at runtime.**
- ...


## EhCache 集群方案

由于 EhCache 是进程中的缓存系统，在集群环境中，每一个节点维护各自的缓存数据，因此需要解决缓存数据同步的问题。

EhCache 支持多种集群方案，比较常用的有RMI、JGroups 以及 EhCache Server。
其中，EhCache Server 是使用一个独立的缓存服务器做为缓存系统，对外提供编程语言无关的基于 HTTP 的 RESTful 或者是 SOAP 的数据缓存操作接口。

## ref:

http://www.cnblogs.com/hoojo/archive/2012/07/12/2587556.html Ehcache使用

https://www.ibm.com/developerworks/cn/java/j-lo-ehcache/index.html EhCache集群

http://www.ehcache.org/generated/2.10.4/pdf/Ehcache_Configuration_Guide.pdf Ehcache Configuration

<br/><br/>
