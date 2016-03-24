##RPC的使用

使用包`import "net/rpc"`

1）定义Server类型：`type Server struct{...}`，并定义一些成员函数作为用于被远程调用的服务。

2）
	
	server:= new(Server)
		
	...	//一些初始化

	rpcServer := rpc.NewServer()
	rpcServer.Register(server)

3）`net.Listen()`函数返回一个listener `l`。

4）`l.Accept()`返回一个连接`conn`;
`rpcServer.ServeConn(conn)`处理该请求。(或`conn.Close()`关闭连接)。一个不断处理请求的服务器就是重复该步骤。。

5）结束服务时调用`l.Close()`关闭监听器。

