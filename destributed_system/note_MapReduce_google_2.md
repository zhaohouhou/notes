###<center>阅读笔记： MapReduce: Simplified Data Processing on Large Clusters (google)(2)</center>

## 4. 精化

尽管提供Map和Reduce函数的功能就可以应对大多数情况，还有一些扩展是十分有用的。

### 4.1 分割函数 （Partitioning Function）

MapReduce的用户指定希望得到的输出文件个数（R）。在这些任务中，数据由中间key上的分割函数分割。提供的默认分割函数使用哈希（`hash(key) mod R`），使得通常分割比较均衡。在某些情况下，使用其他中间key上的分割函数更有帮助。例如，有时输出的key是URL地址，并且我们希望同一个host下的条目都在同一文件中。为了支持这样的情况，用户可以使用特殊的分割函数。例如对上面的例子，可以使用函数`hash(Hostname(urlkey)) mod R`。


### 4.2 排序保证

<u>对于一个给定的分割，保证中间key/value对的处理顺序是按照key递增的顺序。这样每个分割会产生一个有序的输出文件。</u>对于希望按照key值随机访问的情形比较有帮助。

### 4.3 合并函数 （Combiner Function）

在某些情形下，每个map任务产生的中间key具有大量重复，而用户指定的Reduce函数具有交换性和结合性（commutative and associative）。一个很好的例子就是2.1节中的单词计数。由于单词频率倾向于符合Zipf分布，每个map任务将产生成百上千的<the, 1>形式的记录。所有这些记录将在网络上传送给单个reduce任务并由Reduce函数相加产生一个数字。我们允许用户指定一个可选的合并函数，在数据向网络传送之前进行部分的合并。

合并函数 （Combiner Function）在每个执行map任务的机器上执行。通常使用同一份代码来实现合并函数和reduce函数。两者间的唯一区别在于MapReduce库如何处理函数的输出。reduce函数的输出写在一个最终的输出文件；合并函数的输出写在即将传送的reduce任务的的中间文件里。

部分合并对于特定种类的MapReduce任务具有显著的加速作用。

###4.4 输入输出类型

MapReduce库支持几种不同格式的输入数据。例如“text”模式将每行作为一个key/value对，key为偏移量，value为该行内容。每种输入类型的实现需要知道如何将输入切分为有意义的块来交给不同的map任务处理。用户可以通过实现reader接口增加对新类型的支持。

一个reader不一定需要从文件提供输入数据，例如也可定义reader从数据库和内存中读取数据。

输出类型的支持也类似上面。

###4.5 Side-effects （附加影响）

在一些情况下，MapReduce的用户会希望产生一些额外的输出文件。那么使这种附加影响具有原子性和幂等性的任务要依靠应用程序的作者。通常应用向一个临时文件写操作，并在完成时进行原子地重命名。我们没有提供同一任务多个输出文件的原子的二相commit操作。

###4.6 Skipping Bad Records

有时用户代码中的bug导致在一些记录上会产生确定性的失败。由于这些bug可能存在于第三方的库，并且对一些情况而言越过个别记录是可以接受的（例如统计分析大量数据），所以我们提供了一个可选的模式，使得MapReduce库探测哪些记录会产生确定性的失败并跳过这些记录。

每个worker进程装载一个signal handler，来捕捉段冲突和总线错误。在调用一个用户Map或Reduce操作之前，MapReduce库在一个全局的变量中保存参数的序号。如果用户代码产生了一个signal，则signal handler向master发送一个“最后一击（last gasp）”UDP包，包含序号。当master观察到某个特定记录上发生超过一次的故障，则指示下一次Map或Reduce任务的重执行跳过该记录。

###4.7 本地执行

由于计算发生在分布式的系统、通常在上千个机器上、并且由master进行动态的任务分配，调试Map或Reduce函数是一个棘手的事情。MapReduce库提供了另一种实现，顺序化地在本地机器上执行所有任务。

###4.8 状态信息

master运行一个内部的HTTP server并导出一些状态页提供给人类。用户可以得知进展情况、是否需要更多计算资源，失败任务的信息还可以用于错误诊断。

###4.9 计数器

MapReduce库提供了计数器功能，来对各种事件进行计数。为了使用该功能，用户需要建立一个有名的counter对象，并在Map和/或Reduce函数中恰当地增加计数。例如：

	Counter* uppercase;
	uppercase = GetCounter("uppercase");
	
	map(String name, String contents):
		for each word w in contents:
			if (IsCapitalized(w)):
				uppercase->Increment();
			EmitIntermediate(w, "1");

单个worker机器的计数值周期性地传送给master（由ping response携带）。master从成功的map或reduce任务中聚合这些计数值并在MapReduce操作完成后返回给用户代码。进行中的计数值同样显示在状态页。重复（由后备任务和重执行引起）的值被去掉。

MapReduce库自动地维护一些计数值，例如已处理的输入值对、已产生的输出值对。

##5. 性能（略）

举了两个例子。说明了有备用任务的情况比不使用备用任务要快很多（尽管大部分任务完成情况相近，但不使用备用任务会有长尾部）。还说明对于很多机器失效的情况能够快速恢复，总运行时间仅增长几个百分比。

##6. 经验

Google在2003年实现了第一版的MapReduce库，此后发现MapReduce库能够应用于极其广泛的各类问题，包括：

- 大规模机器学习问题；
- Google News和Froogle产品的聚类问题；
- 用于大众查询报告的数据抽取；
- 用于新实验和产品的web页面属性抽取；
- 大规模的图计算。

MapReduce库建立后在谷歌的应用迅速地增长。

###大规模索引

至文章截止，MapReduce的最重要应用就是重写了索引生成系统——用于产生谷歌网页搜索服务所需的数据结构。
索引系统的输入为爬虫系统获得的大量的文档，保存为GFS文件。每个文件大小超过20TB。索引过程是5到10个MapReduce操作的序列。相比于上一版本使用ad-hoc，使用MapReduce有以下优点：




<br/><br/><br/>