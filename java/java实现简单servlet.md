# java+maven实现简单servlet

目标：通过maven工程建立一个http服务，处理简单的post和get请求。

## 1. servlet

实现`javax.servlet.http.HttpServlet`类，并实现`doGet`和`doPost`函数。

```java
package com.mytest;
public class MyTestServlet extends HttpServlet
{
    @Override
    public void doGet(HttpServletRequest request, HttpServletResponse response)
                      throws
                      ServletException, IOException
    {
        response.setCharacterEncoding("utf8");
        response.setContentType("text/json");
        PrintWriter out = response.getWriter();
        out.print("test-connect");
        out.close();
    }

    @Override
    public void doPost(HttpServletRequest request, HttpServletResponse response)
                      throws ServletException, IOException
    {
        request.setCharacterEncoding("utf8");
        response.setCharacterEncoding("utf8");
        response.setContentType("application/json");
        PrintWriter out = response.getWriter();
        //处理接收到的json
        String message = request.getParameter("message");
        System.out.println(message);

        //返回json数据
        JSONObject obj = new JSONObject();
        obj.put("message", "hello from server");
        out.print(obj);
        out.close();	//不要忘记！
    }
}
```

ref： http://stackoverflow.com/questions/9645647/return-json-from-servlet

## 2. maven配置

配置pom依赖导入需要的jar包，packaging方式为war包。此外，配置插件：

```xml
<build>
  ...
    </resources>
        <plugins>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-war-plugin</artifactId>
                <version>2.2</version>
                <configuration>
                  ...
                </configuration>
            </plugin>
            <plugin>
                <groupId>org.apache.tomcat.maven</groupId>
                <artifactId>tomcat7-maven-plugin</artifactId>
                <version>2.2</version>
                <configuration>
                    <path>/MY_APP</path>
                    <port>8080</port>
                    <useTestClasspath>false</useTestClasspath>
                    <warSourceDirectory>src/main/webapp</warSourceDirectory>
                    <additionalClasspathDirs>
                        <additionalClasspathDir></additionalClasspathDir>
                    </additionalClasspathDirs>
                </configuration>
            </plugin>
        </plugins>
</build>
```

## 3. web.xml

建立web.xml配置文件。web.xml要指明Servlet的路径。web.xml存放在src/main/webapp/WEB-INF目录。

为了使上面的servlet能够运行，配置web.xml:


```xml
<?xml version="1.0" encoding="UTF-8"?>
<web-app ...（模板）>
	<display-name>MyApp/display-name>
	<servlet>
		<servlet-name>MyServlet</servlet-name>
		<servlet-class>com.mytest.MyTestServlet</servlet-class>
		<load-on-startup>1</load-on-startup>
	</servlet>

	<servlet-mapping>
		<servlet-name>MyServlet</servlet-name>
		<!-- default servlet -->
		<url-pattern>/test</url-pattern>
	</servlet-mapping>

</web-app>

```

通过servlet-name绑定路径和类。url-pattern为servlet访问路径。如果为'/'，则项目根目录为访问地址。

## 4. 启动tomcat

官网下载Tomcat7.0.77, 并解压。增加环境变量：

```
#tomcat7
export TOMCAT_HOME=/opt/apache-tomcat-7.0.77
export PATH=$PATH:$TOMCAT_HOME/bin
```

工程目录运行`maven: mvn clean install package`.  启动服务：`mvn tomcat7:run`


上面的配置，本地浏览器访问`http://localhost:8080/MY_APP/test`，显示："hello from server"。说明连接成功。

使用curl工具发送请求并显示回复：

  `curl -i -d "message=hello" http://localhost:8080/MY_APP/test`

#### ref:

模板： http://blog.csdn.net/u013816347/article/details/50435296
