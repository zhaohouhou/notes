#6.824 lab 3 Paxos-based Key/Value Service

##Introduction 

实验2中依靠单个的master view server来选择primary，一旦view server不可用，那么key-value服务将不能进行。另一个缺陷是当primary或backup失效时需要进行全部database的传输，代价很高。

本实验中使用Paxos来管理

————————————————————

在这部分实验中使用主要/备用形式的副本进行容错，实现一个key/value服务。使用一种称为viewservice的主服务器进行管理。viewservice监控每个服务器是否可用。当某个主服务器（primary）或备用服务器（backup）失效，viewservice选择一个服务器进行取代。client通过查询viewservice来找到当前的primary。每个时刻最多只能有一个primary处于活动状态。

当一个primary失效，viewservice将会将backup提升为primary。当backup失效或被提升，并且存在一个空闲服务器，则它将成为backup。primary会将其完整的数据库传给新的backup，并且将后续的Puts操作传给backup，以保证backup的key/value数据库与primary保持一致。

此外primary也要将Gets操作发送给backup（若存在），并且在得到backup的回答后再回应客户端。这样做有利于避免两个服务器同时作为primary存在（即 "split brain"）。

发生故障的key/value服务器可以重启，但数据已经丢失（即数据只保存在内存中而非磁盘上）。客户机与服务器、服务器间的信息交换只能使用RPC，不能通过共享文件或变量。

以上设计的局限性：

1) viewservice本身没有副本，因此受故障影响较大。

2) primary 和 backup必须一次一个地进行操作，降低了性能。

3) 一个恢复工作的服务器需要从primary拷贝全部数据，降低了性能。

4) 由于服务器只把数据存放在内存中，系统无法挺过同时性的故障。

5) 如果一个primary在回复其所在的view之前发生故障，则viewservice只能停止更新view的操作。（避免split brain）

等等。

##Part A： The Viewservice

在这一部分要实现一个viewservice，处理一系列有序号的*view*，每个view包含一个primary和一个backup（如果有）。view由view的号码和primary与backup的id组成。

一个view的primary一定是前一个view的primary或者backup，这样有利于保存key/value service的状态。例外的情况是当viewservice启动时，任意服务器都可作为一开始的primary。backup可以是除了primary之外的任意服务器，如果没有空闲服务器也可以是空的（""表示）。

每个key/value server经过一个`PingInterval`向view service发送一个Ping RPC：（1）通知view service该服务器正常工作；（2）通知view service该服务器最进接收到的view的信息；（3）如果一个服务器在故障后重启，则向view service发送一个参数为0的Ping。

view service回复key/value server的Ping，再回复中包含当前view的信息。如果view service经过`DeadPings`个数的`PingInterval`仍未收到某个服务器的Ping，则认为该服务器宕机。

</br>
view service进入到一个新的view，当：

- view service没有收到临近的primary和（或？）backup的Ping；
- primary或backup宕机后重启；
- 当前没有backup而且有一个闲置的服务器。

但是，<font color=red>如果view X的primary没有对该view进行回复，则不能前进到view X+1。</font>

</br>
tips:

- Go的RPC server框架对于每个接收到的RPC请求启动一个新的线程，即服务器上可能会同时运行多个线程。实现功能时需注意线程安全性。

##Part B：The primary/backup key/value service

完成key/value service的客户端和服务器代码`pbservice/client.go`,`pbservice/server.go`.
客户端创建一个`Clerk`对象，并通过调用其方法来向服务发送RPC请求。

只要有可用的服务器，key/value service就应当能够工作。还要容忍网络隔离：临时网络问题、或服务器只能与一部分机器联络。如果服务只运行在一个服务器上，那么需要一个空闲服务器作为backup，以容忍服务器故障。<font color=red>(这段的意思？)</font>

正确的操作意味着所有对`Clerk.Get(k)`的调用返回最新的由`Clerk.Put(k,v)`或`Clerk.Append(k,v)`所成功设置的值，或者当该key不存在时返回空字符串。所有操作遵循at-most-once语义。

假设viewservice不会崩溃。此外客户端和服务器只能通过RPC通信，通过`client.go`中的`call()`方法发送RPC.

关键点：

- 同一时刻只能有一个活动的primary。
-  不是primary的服务器或者不能回复客户端，或者回复一个错误信息：GetReply.Err或PutReply.Err的值不是OK。
- Clerk.Get(), Clerk.Put()和Clerk.Append()方法应当仅在操作完成后返回，在操作未成功时不断尝试。服务器需要过滤掉重复的用于重试的RPC，以保证at-most-once语义。可以假设每个每个Clerk一次只有一个Put或Get调用。
- 为了避免viewservice位于性能与容错的关键路径，服务器不应当对每个Put/Get请求都查询viewservice服务，而应当周期性地发送Ping来获得新的view(pbservice/server.go中的 tick())。与此相似，客户端也不应当对每个要发送的RPC请求都查询viewservice，而是应该缓存当前的primary，并且仅当primary似乎故障是与viewservice联络。
- one-primary-at-a-time的实现部分基于viewservice仅提升view i的backup作为view i+1的primary。当旧的primary试图处理一个客户请求，将其转寄给backup：如果该backup尚未知悉view i+1，不会作为primary工作，则不会产生损害；如果此时旧backup已经作为新primary工作，那么它就会拒绝该转寄的请求。（设想如果提拔了其他服务器作为新primary，那么当旧primary没有获得最新view，新旧primary就可能同时工作。）
- 需要保证backup看到每个key/value数据库的更新：primary在backup初始化时提供整个key/value数据库，并且将后续端客户操作抄送给backup。







