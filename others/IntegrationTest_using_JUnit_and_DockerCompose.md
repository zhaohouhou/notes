# 使用 Docker Compose 和 JUnit 编写集成测试

场景：

- 测试依赖外部组件
- 微服务间调用的测试

**依赖：测试机器上能够运行 Docker**

以下为以 Junit 5 为例的集成方法。

## 1. 修改 `build.gradle` 配置

添加 dependency:

```
testCompile group: 'org.testcontainers', name: 'testcontainers', version: '1.12.4'
```

可以结合 @Tag 和 gradle 命令, 分别运行集成测试和单元测试。
定义 `IntegrationTest` 接口（详见 Junit @Tag 的使用），并在`build.gradle`中添加

```
test {
    useJUnitPlatform {
		excludeTags 'IntegrationTest'
	}
}

task integrationTest(type: Test) {
    useJUnitPlatform {
        includeTags 'IntegrationTest'
    }
}
```

## 2. 使用

下面的例子通过 Docker Compose 的方式连接mongo数据服务进行集成测试。

准备docker-compose文件：

```
version: '3'

services:
  mongo:
    image: 'mongo:4.2.0'
    ports:
      - '27017:27017'
```

UT代码：

```java
static MongoClient mongoClient;

public static void setUp () {
    // set up mongo docker
    DockerComposeContainer compose = new DockerComposeContainer(
            new File("src/test/resources/docker-compose.yml")).withExposedService("mongo", 27017);
    compose.start();
    String host = compose.getServiceHost("mongo", 27017);
    int port = compose.getServicePort("mongo", 27017);
    MongoClient mongoClient = ...
}

```

此外 testcontainers 提供了一些模板可以直接使用，不需要 Docker Compose 文件。例如简单的启动后执行一条命令的 `GenericContainer`，以及一些数据库服务（需要添加相应的依赖，可在mvn仓库中查询）。

## ref

https://www.baeldung.com/docker-test-containers#

## other option

'com.palantir.docker.compose:docker-compose-junit-jupiter'

ref:

- https://blog.codecentric.de/en/2017/03/writing-integration-tests-docker-compose-junit/

- https://github.com/palantir/docker-compose-rule
