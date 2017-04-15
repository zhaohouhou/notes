# Get Started

## 0. 开始

### 0.1 启动  MongoDB 服务

运行安装目录下bin目录中的mongod程序，db目录需要手动建立。

      mongod  --dbpath \data\db

参数：

<table class="reference">
<tbody>
<tr>
<th>参数</th>
<th>描述</th>
</tr>
<tr>
<td>--bind_ip</td>
<td>绑定服务IP，若绑定127.0.0.1，则只能本机访问，不指定默认本地所有IP</td>
</tr>
<tr>
<td>--logpath</td>
<td>定MongoDB日志文件，注意是指定文件不是目录</td>
</tr>
<tr>
<td>--logappend</td>
<td>使用追加的方式写日志</td>
</tr>
<tr>
<td>--dbpath</td>
<td>指定数据库路径</td>
</tr>
<tr>
<td>--port</td>
<td>指定服务端口号，默认端口27017</td>
</tr>
<tr>
<td>--serviceName</td>
<td>指定服务名称</td>
</tr>
<tr>
<td>--serviceDisplayName</td>
<td>指定服务名称，有多个mongodb服务时执行。</td>
</tr>
<tr>
<td>--install</td>
<td>指定作为一个Windows服务安装。</td>
</tr>
</tbody>
</table>

### 0.2 连接

启动服务后可使用shell连接MongoDB服务。启动bin目录下mongo程序，默认连接至test数据库。

## 1.  MongoDB数据模型

概念对应:

<table class="reference">
<tbody><tr>
<th>RDBMS</th>
<th>MongoDB术语/概念</th>
<th>说明</th>
</tr>
<tr>
<td>database</td>
<td>database</td>
<td>数据库</td>
</tr>
<tr>
<td>table</td>
<td>collection</td>
<td>数据库表/集合</td>
</tr>
<tr>
<td>row</td>
<td>document</td>
<td>数据记录行/文档</td>
</tr>
<tr>
<td>column</td>
<td>field</td>
<td>数据字段/域</td>
</tr>
<tr>
<td>index</td>
<td>index</td>
<td>索引</td>
</tr>
<tr>
<td>table joins</td>
<td>&nbsp;</td>
<td>表连接,MongoDB不支持</td>
</tr>
<tr>
<td>primary key</td>
<td>primary key</td>
<td>MongoDB自动将_id字段设置为主键</td>
</tr>
</tbody></table>

### 1.1 数据库

MongoDB的单个实例可以容纳多个独立的数据库，每一个都有自己的集合和权限，不同的数据库也放置在不同的文件中。

"show dbs" 命令可以显示所有数据的列表。

执行 "db" 命令可以显示当前数据库对象或集合。

运行"use"命令，可以连接到一个指定的数据库。

#### 数据库名称要求：

- 不能是空字符串（"")。
- 不得含有' '（空格)、.、$、/、\和\0 (空宇符)。
- 应全部小写。
- 最多64字节。

### 1.2 文档 document

文档是若干键值(key-value)对构成的一条记录(BSON)。MongoDB 的文档不需要设置相同的字段，并且相同的字段不需要相同的数据类型。例如：`{"site":"www.google.com","name":"Google"}`

- 文档中的键/值对是有序的。
- 文档中的值可以是多种数据类型（可以是嵌入的文档)。
- MongoDB区分类型和大小写。
- MongoDB的文档不能有重复的键。
- 文档的键是字符串。除了少数例外情况，键可以使用任意UTF-8字符。

文档键命名规范：
- 键不能含有\0 (空字符)。这个字符用来表示键的结尾。
- .和$有特别的意义，只有在特定环境下才能使用。
- 以下划线"\_"开头的键是保留的(不是严格要求的)。

### 1.3 集合 collection

集合就是 MongoDB 文档组，类似于 RDBMS 中的表格。

在MongoDB数据库中名字空间 <dbname>.system.* 是包含多种系统信息的特殊集合(Collection)，如下:

<table class="reference">
<tbody>
<tr>
<th>集合命名空间</th>
<th>描述</th>
</tr>
<tr>
<td>dbname.system.namespaces</td>
<td>列出所有名字空间。</td>
</tr>
<tr>
<td>dbname.system.indexes</td>
<td>列出所有索引。</td>
</tr>
<tr>
<td>dbname.system.profile</td>
<td>包含数据库概要(profile)信息。</td>
</tr>
<tr>
<td>dbname.system.users</td>
<td>列出所有可访问数据库的用户。</td>
</tr>
<tr>
<td>dbname.local.sources</td>
<td>包含复制对端（slave）的服务器信息和状态。</td>
</tr>
</tbody>
</table>

### MongoDB 数据类型

<table class="reference">
<tbody><tr>
<th>数据类型</th>
<th>描述</th>
</tr>
<tr><td>String</td><td>字符串。存储数据常用的数据类型。在 MongoDB 中，UTF-8 编码的字符串才是合法的。   </td></tr>
<tr><td>Integer</td><td>整型数值。用于存储数值。根据你所采用的服务器，可分为 32 位或 64 位。  </td></tr>
<tr><td>Boolean</td><td>布尔值。用于存储布尔值（真/假）。  </td></tr>
<tr><td>Double</td><td>双精度浮点值。用于存储浮点值。  </td></tr>
<tr><td>Min/Max keys</td><td>将一个值与 BSON（二进制的 JSON）元素的最低值和最高值相对比。  </td></tr>
<tr><td>Arrays</td><td>用于将数组或列表或多个值存储为一个键。  </td></tr>
<tr><td>Timestamp</td><td>时间戳。记录文档修改或添加的具体时间。  </td></tr>
<tr><td>Object</td><td>用于内嵌文档。  </td></tr>
<tr><td>Null</td><td>用于创建空值。  </td></tr>
<tr><td>Symbol</td><td>符号。该数据类型基本上等同于字符串类型，但不同的是，它一般用于采用特殊符号类型的语言。</td></tr>
<tr><td>Date</td><td>日期时间。用 UNIX 时间格式来存储当前日期或时间。你可以指定自己的日期时间：创建 Date 对象，传入年月日信息。  </td></tr>
<tr><td>Object ID</td><td>对象 ID。用于创建文档的 ID。  </td></tr>
<tr><td>Binary Data</td><td>二进制数据。用于存储二进制数据。</td></tr>
<tr><td>Code</td><td>代码类型。用于在文档中存储 JavaScript 代码。</td></tr>
<tr><td>Regular expression</td><td>正则表达式类型。用于存储正则表达式。</td></tr>
</tbody></table>
