# maven发布可运行jar包

### 1. pom打包方式配置

```xml
	<packaging>jar</packaging>
```

`<packaging>war</packaging>`为发布war包，用于网络应用。


工程目录运行`mvn clean install package`打包。生成的jar包在target目录。


### 2. 引用jar包

pom文件添加引用。例如：

```xml
<dependencies>
		<dependency>
			<groupId>org.xxx</groupId>
			<artifactId>xxx</artifactId>
			<version>1.0</version>
		</dependency>
</dependencies>
```

可在http://mvnrepository.com 搜索所需包的dependency。

### 3. 包含jar包

使用shade可以方便的将应用到的jar包都打包进发布的jar包中，方便移动和运行。pom配置：

```xml
<plugin>
	<groupId>org.apache.maven.plugins</groupId>
	<artifactId>maven-shade-plugin</artifactId>
	<version>2.3</version>
	<executions>
		<execution>
			<phase>package</phase>
			<goals>
				<goal>shade</goal>
			</goals>
			<configuration>
				<transformers>
					<transformer
						implementation="org.apache.maven.plugins.shade.resource.ManifestResourceTransformer">
						<mainClass>com.xxx.MainClass</mainClass>
					</transformer>
				</transformers>
				<filters>
					<filter>
						<artifact>*:*</artifact>
						<excludes>
							<exclude>META-INF/*.SF</exclude>
							<exclude>META-INF/*.DSA</exclude>
							<exclude>META-INF/*.RSA</exclude>
						</excludes>
					</filter>
				</filters>
			</configuration>
		</execution>
	</executions>
</plugin>
```

其中`<mainClass>`标识了程序入口。

### 4. 本地jar包

一些需要引用的jar包可能无法在maven库找到，有几种方式添加引用：

- pom中添加dependency：

```xml
	<dependency>
		<groupId>xxx</groupId>
		<artifactId>xxx</artifactId>
		<version>1.0</version>
		<scope>system</scope>
		<systemPath>${basedir}/lib/xxx.jar</systemPath>
	</dependency>
```

其中`<scope>system</scope>`表示假设该jar包一直存在于文件系统。因此这种方式不利于协同开发。并且shade也不能将这些jar包打包。

-  使用maven的install命令手动添加至本地仓库

适用于在单独一台机器上开发，不利于协同开发。

-  使用/上传至自己的远程仓库

增强了可移动性，有利于协同开发。

-  使用addjars插件

适用于maven 3.0.3以上版本，可以将项目目录中一个文件夹下的所有jar文件添加至classpath。亲测好用。

```xml
<plugin>
	<groupId>com.googlecode.addjars-maven-plugin</groupId>
	<artifactId>addjars-maven-plugin</artifactId>
	<version>1.0.5</version>
	<executions>
		<execution>
			<goals>
				<goal>add-jars</goal>
			</goals>
			<configuration>
				<resources>
					<resource>
						<directory>${basedir}/lib</directory>
					</resource>
				</resources>
			</configuration>
		</execution>
	</executions>
</plugin>
```


### ref:

http://hemika-kodikara.blogspot.com/2015/03/creating-runnable-jar-using-maven.html

http://stackoverflow.com/questions/3642023/having-a-3rd-party-jar-included-in-maven-shaded-jar-without-adding-it-to-local-r


https://code.google.com/archive/p/addjars-maven-plugin/wikis/UsagePage.wiki
