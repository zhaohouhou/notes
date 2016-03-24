###1. 找出扩展用例的非显式步骤 nontrivial step

表格的方式，两列：用户和系统。以一个登录行为为例：

<table border="1" >
  <tr align="center">
    <th>Actor</th>
    <th>系统</th>
  </tr>
  <tr>
    <td> </td>
    <td>0. 系统显示主页面</td>
  </tr>
  <tr>
    <td>1. 顾客点击登录按钮</td>
    <td>2. 系统要求用户输入信息</td>
  </tr>
 <tr>
    <td>3. 顾客输入登录信息</td>
    <td>4. 系统进行验证</td>
  </tr>
  <tr>
    <td>5. 显示主页面</td>
    <td> </td>
  </tr>
</table>

第0步，总是从系统开始：当系统准备好才接受用户行为。应为由actor触发

包含多个过程的步骤（例如验证用户输入，需要与服务器交互）称为：**nontrivial step**

###2. scenario 情景建模

nontrivial step列出子步骤(语句描述)。
如对上表：

- 4.1 Login-controller从Login.jsp收到登陆请求数据
- 4.2 Login-controller从数据库更具相应id获取password（根据手机号码获取验证码，等）
- 4.3 比较
- 4.4.1 比较成功，...
	- 4.4.1.1 向message中添加成功信息...
- 4.4.2 比较失败，...
	- 4.4.2.1 向message中添加失败信息...
- 4.4.3 返回message

###3. scenario table

有了scenario描述，接下来列出scenario table。共有5列。举一部分为例：

<table border="1" >
  <tr align="center">
	<th>#</th>
	<td>Subject(动作的主体)</td>
	<td>Action(动作)</td>
	<td>Data</td>
	<td>Object(动作施加对象)</td>
  </tr>
  <tr>
    <th>3</th>
	<td>patron</td>
	<td>enter</td>
	<td>tel&verify code</td>
	<td>Login.jsp</td>
  </tr>
  <tr>
    <th>4.1</th>
	<td>Login-controller</td>
	<td>receive</td>
	<td>tel&verify code</td>
	<td>Login.jsp</td>
  </tr>
 <tr>
    <th>4.2</th>
	<td>Login-controller</td>
	<td>get</td>
	<td>tel(从数据库获得信息只用到电话号)</td>
	<td>database</td>
  </tr>
  <tr>
    <th>4.4</th>
	<td>if result is ture</td>
	
  </tr>
<tr>
    <th>4.4.1</th>
	<td>Login-controller</td>
	<td>add</td>
	<td>"success"</td>
	<td>message</td>
  </tr>
</table>

考虑过程的触发过程：由触发者进行调用，因此4.1可改为：

<table border="1" >
  <tr align="center">
	<th>#</th>
	<td>Subject(动作的主体)</td>
	<td>Action(动作)</td>
	<td>Data</td>
	<td>Object(动作施加对象)</td>
  </tr>
  
  <tr>
    <th>4.1</th>
	<td>Login.jsp</td>
	<td>sends</td>
	<td>tel&verify code</td>
	<td>Login-controller</td>
  </tr>
 
</table>

###4. sequence diagram（uml 时序图）

对象的表示(需要有下划线)：

<table border="1" >
  <tr align="center">
	<th><u>Name : Type</u></th>
	
  </tr>
</table>

