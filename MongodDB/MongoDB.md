# MongoDB

## 启动服务

命令行运行： `mongod`

部分参数：
```
--dbpath arg                          指定数据库文件位置
--port arg                            指定端口号
--auth                                开启认证（run with security）
--noauth                              run without security
--master                              master mode
--slave                               slave mode
--shutdown                            kill a running server
```

## 数据库管理

**开启**：命令行运行`mongo`

**显示数据库**：

	show dbs

**显示表**：

	show tables


**切换到数据库**：

	use DB

**新建用户**：


	db.createUser(
	{
 		user: "root",
		pwd:"123",
 		roles: [
			{role: "readWrite", db: "DB" }
 			]
	})

**删除用户**：

db.dropUser("root")

显示用户：

	show users

## 导出数据

命令：`mongoexport`

参数：
```
--host arg                      server地址
--port arg                      端口号
--username arg                  用户名
--password                      密码
--db arg                        数据库名
-c                              collection name
-f arg1[, arg2, ...]            要输出的field-name
-o arg                          输出文件名
--csv                           输出为csv文件
-q query_string                 查询条件
```

其他问题：

- 报Authenticate failed：可能需要增加认证数据库参数例如：

	`--authenticationDatabase admin`

- 输出为空：需要查询条件，例如返回全部结果：`-q '{}'`

<br/><br/>
