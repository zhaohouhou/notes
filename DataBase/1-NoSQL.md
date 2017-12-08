# NoSQL

> MongoDB教程--1
>
> ref： http://www.runoob.com/mongodb/nosql.html

NoSQL = Not Only SQL，即"不仅仅是SQL"。

## 1. 关系型数据库

1970年 E.F.Codd's提出的关系模型的论文 "A relational model of data for large shared data banks"，这使得数据建模和应用程序编程更加简单。

事务在英文中是transaction，和现实世界中的交易很类似，它有如下四个特性：

#### 1. A (Atomicity) 原子性

原子性是说事务里的所有操作要么全部做完，要么都不做，事务成功的条件是事务里的所有操作都成功，只要有一个操作失败，整个事务就失败，需要回滚。

比如银行转账，从A账户转100元至B账户，分为两个步骤：1）从A账户取100元；2）存入100元至B账户。这两步要么一起完成，要么一起不完成，如果只完成第一步，第二步失败，钱会莫名其妙少了100元。

#### 2. C (Consistency) 一致性

一致性，也就是说数据库要一直处于一致的状态，事务的运行不会改变数据库原本的一致性约束。

#### 3. I (Isolation) 独立性

独立性是指并发的事务之间不会互相影响，如果一个事务要访问的数据正在被另外一个事务修改，只要另外一个事务未提交，它所访问的数据就不受未提交事务的影响。

#### 4. D (Durability) 持久性

持久性是指一旦事务提交后，它所做的修改将会永久的保存在数据库上，即使出现宕机也不会丢失。

## 2. NoSQL

NoSQL，指的是非关系型的数据库。NoSQL用于超大规模数据的存储。（例如谷歌或Facebook每天为他们的用户收集万亿比特的数据）。这些类型的数据存储不需要固定的模式，无需多余操作就可以横向扩展。

#### RDBMS vs NoSQL

RDBMS
- 高度组织化结构化数据
- 结构化查询语言（SQL） (SQL)
- 数据和关系都存储在单独的表中。
- 数据操纵语言，数据定义语言
- 严格的一致性
- 基础事务

NoSQL
- 代表着不仅仅是SQL
- 没有声明性查询语言
- 没有预定义的模式
- 键 - 值对存储，列存储，文档存储，图形数据库
- 最终一致性，而非ACID属性
- 非结构化和不可预知的数据
- CAP定理
- 高性能，高可用性和可伸缩性

#### CAP定理

CAP定理（CAP theorem）, 又被称作 布鲁尔定理（Brewer's theorem）, 它指出对于一个分布式计算系统来说，不可能同时满足以下三点:

- 一致性(Consistency) (所有节点在同一时间具有相同的数据)
- 可用性(Availability) (保证每个请求不管成功或者失败都有响应)
- 分隔容忍(Partition tolerance) (系统中任意信息的丢失或失败不会影响系统的继续运作)

CAP理论的核心是：一个分布式系统不可能同时很好的满足一致性，可用性和分区容错性这三个需求，最多只能同时较好的满足两个。

因此，根据 CAP 原理将 NoSQL 数据库分成了满足 CA 原则、满足 CP 原则和满足 AP 原则三大类：

- CA - 单点集群，满足一致性，可用性的系统，通常在可扩展性上不太强大。
- CP - 满足一致性，分区容忍性的系统，通常性能不是特别高。
- AP - 满足可用性，分区容忍性的系统，通常可能对一致性要求低一些。

## 3. NoSQL 数据库分类

<table class="reference">
	<tbody>
		<tr>
			<td>

					类型

			</td>
			<td>

					部分代表
				<p></p>
			</td>
			<td>
				特点
			</td>
		</tr>
		<tr>
			<td>
				列存储
			</td>
			<td>
				<p>
					<span style="font-size:14px;font-family:宋体;">Hbase</span><span style="font-size:12.0000pt;font-family:'宋体';"></span>
				</p>
				<p>
					<span style="font-size:14px;font-family:宋体;">Cassandra</span><span style="font-size:12.0000pt;font-family:'宋体';"></span>
				</p>
				<p>
					<span style="font-size:14px;font-family:宋体;">Hypertable</span><span style="font-size:12.0000pt;font-family:'宋体';"></span>
				</p>
			</td>
			<td>
				<p>
					<span style="font-size:14px;font-family:宋体;">顾名思义，是按列存储数据的。最大的特点是方便存储结构化和半结构化数据，方便做数据压缩，对针对某一列或者某几列的查询有非常大的IO优势。</span><span style="font-size:12.0000pt;font-family:'宋体';"></span>
				</p>
			</td>
		</tr>
		<tr>
			<td>
				<p>
					<span style="font-size:14px;font-family:宋体;">文档存储</span><span style="font-size:12.0000pt;font-family:'宋体';"></span>
				</p>
			</td>
			<td>
				<p>
					<span style="font-size:14px;font-family:宋体;">MongoDB</span><span style="font-size:12.0000pt;font-family:'宋体';"></span>
				</p>
				<p>
					<span style="font-size:14px;font-family:宋体;">CouchDB</span><span style="font-size:12.0000pt;font-family:'宋体';"></span>
				</p>
			</td>
			<td>
				<p>
					<span style="font-size:14px;font-family:宋体;">文档存储一般用类似json的格式存储，存储的内容是文档型的。这样也就有有机会对某些字段建立索引，实现关系数据库的某些功能。</span><span style="font-size:12.0000pt;font-family:'宋体';"></span>
				</p>
			</td>
		</tr>
		<tr>
			<td>
				<p>
					<span style="font-size:14px;font-family:宋体;">key-value存储</span><span style="font-size:12.0000pt;font-family:'宋体';"></span>
				</p>
			</td>
			<td>
				<p>
					<span style="font-size:14px;font-family:宋体;">Tokyo&nbsp;Cabinet&nbsp;/&nbsp;Tyrant</span><span style="font-size:12.0000pt;font-family:'宋体';"></span>
				</p>
				<p>
					<span style="font-size:14px;font-family:宋体;">Berkeley&nbsp;DB</span><span style="font-size:12.0000pt;font-family:'宋体';"></span>
				</p>
				<p>
					<span style="font-size:14px;font-family:宋体;">MemcacheDB</span><span style="font-size:12.0000pt;font-family:'宋体';"></span>
				</p>
				<p>
					<span style="font-size:14px;font-family:宋体;">Redis</span><span style="font-size:12.0000pt;font-family:'宋体';"></span>
				</p>
			</td>
			<td>
				<p>
					<span style="font-size:14px;font-family:宋体;">可以通过key快速查询到其value。一般来说，存储不管value的格式，照单全收。（Redis包含了其他功能）</span><span style="font-size:12.0000pt;font-family:'宋体';"></span>
				</p>
			</td>
		</tr>
		<tr>
			<td>
				<p>
					<span style="font-size:14px;font-family:宋体;">图存储</span><span style="font-size:12.0000pt;font-family:'宋体';"></span>
				</p>
			</td>
			<td>
				<p>
					<span style="font-size:14px;font-family:宋体;">Neo4J</span><span style="font-size:12.0000pt;font-family:'宋体';"></span>
				</p>
				<p>
					<span style="font-size:14px;font-family:宋体;">FlockDB</span><span style="font-size:12.0000pt;font-family:'宋体';"></span>
				</p>
			</td>
			<td>
				<p>
					<span style="font-size:14px;font-family:宋体;">图形关系的最佳存储。使用传统关系数据库来解决的话性能低下，而且设计使用不方便。</span><span style="font-size:12.0000pt;font-family:'宋体';"></span>
				</p>
			</td>
		</tr>
		<tr>
			<td>
				<p>
					<span style="font-size:14px;font-family:宋体;">对象存储</span><span style="font-size:12.0000pt;font-family:'宋体';"></span>
				</p>
			</td>
			<td>
				<p>
					<span style="font-size:14px;font-family:宋体;">db4o</span><span style="font-size:12.0000pt;font-family:'宋体';"></span>
				</p>
				<p>
					<span style="font-size:14px;font-family:宋体;">Versant</span><span style="font-size:12.0000pt;font-family:'宋体';"></span>
				</p>
			</td>
			<td>
				<p>
					<span style="font-size:14px;font-family:宋体;">通过类似面向对象语言的语法操作数据库，通过对象的方式存取数据。</span><span style="font-size:12.0000pt;font-family:'宋体';"></span>
				</p>
			</td>
		</tr>
		<tr>
			<td>
				<p>
					<span style="font-size:14px;font-family:宋体;">xml数据库</span><span style="font-size:12.0000pt;font-family:'宋体';"></span>
				</p>
			</td>
			<td>
				<p>
					<span style="font-size:14px;font-family:宋体;">Berkeley&nbsp;DB&nbsp;XML</span><span style="font-size:12.0000pt;font-family:'宋体';"></span>
				</p>
				<p>
					<span style="font-size:14px;font-family:宋体;">BaseX</span><span style="font-size:12.0000pt;font-family:'宋体';"></span>
				</p>
			</td>
			<td>
				<p>
					<span style="font-size:14px;font-family:宋体;">高效的存储XML数据，并支持XML的内部查询语法，比如XQuery,Xpath。</span><span style="font-size:12.0000pt;font-family:'宋体';"></span>
				</p>
			</td>
		</tr>
	</tbody>
</table>

<br/>
