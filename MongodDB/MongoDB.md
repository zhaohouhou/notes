#MongoDB

**启动服务**： `mongod --dbpath yourDBPath`

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
		}
	 )
	
**删除用户**：

db.dropUser("root")

显示用户：

	show users