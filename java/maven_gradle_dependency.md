 
 
 A Maven `pom.xml` file or a Gradle `build.gradle` file specifies 
 the steps necessary to create a software artifact from our source code.
 
### dependency scopes

gradle 的 dependency scope 定义相比 maven 更加细粒度
  
| maven scope | gradle scope | when available  | Leaks to consumers? | Included in Artifact? | 
|---|---|---|---|---|
| **compile** | api | compile time, runtime, test compile time, test runtime | yes | yes |
| **provided** | compileOnly, testCompileOnly | compile time, runtime, test compile time, test runtime | yes | no |
| **runtime** | runtimeOnly, testRuntimeOnly | runtime, test runtime | runtime | yes |
| **test** | testCompile, testRuntime, testImplementation |  test compile time, test runtime | no | no |

此外 maven 还有 **system** 和 **import** (since 2.0.9) 范围

gradle 的 `implementation` scope 与 api 的区别是不暴露给使用者。（`implementation` 转换成 maven dependency 是 `runtime`）

### maven dependency further details

- `compile`: the default scope, used if none is specified. 
(Compile dependencies are available in all classpaths of a project)

- `provided`: (Expect the JDK or a container to provide the dependency at runtime)

- `runtime`: Indicates that the dependency is not required for compilation, but is for execution.
 It is in the runtime and test classpaths, but not the compile classpath.

- `system`: Similar to provided, except that you have to provide the JAR which contains it explicitly. 
The artifact is always available and is not looked up in a repository.

- `import`: Only supported on a dependency of type pom in the section. 
It indicates the dependency to be replaced with the effective list of dependencies in the specified POM’s section.
 Since they are replaced, dependencies with a scope of import do not actually participate in 
 limiting the transitivity of a dependency.
  
### Gradle Configurations

Gradle 要使用这些dependency配置，需要引入 Java Library Plugin:

```
plugins {
    id 'java-library'
}
```

###  ref

https://reflectoring.io/maven-scopes-gradle-configurations/

https://docs.gradle.org/current/userguide/java_library_plugin.html#sec:java_library_configurations_graph

https://juejin.im/post/5c1700f5f265da614312f794#heading-5  一文彻底搞清 Gradle 依赖

</br>
