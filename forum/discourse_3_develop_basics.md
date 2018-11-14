
# Discourse 笔记 3：Rails 开发入门

2018-4-26

本节是关于 Discourse 、Rails 框架开发的基础。

### 1. Rails 项目代码文件结构

<table id="table-1-1" class="tabular">
	<tbody>
		<tr>
			<th class="align_left"><strong>文件/文件夹</strong></th>
			<th class="align_left"><strong>说明</strong></th>
		</tr>
		<tr class="top_bar">
			<td class="align_left">`app/`</td>
			<td class="align_left">程序的核心文件，包含模型、视图、控制器和帮助方法</td>
		</tr>
		<tr>
			<td class="align_left">`app/assets`</td>
			<td class="align_left">程序的资源文件，如 CSS、JavaScript 和图片</td>
		</tr>
		<tr>
			<td class="align_left">`config/`</td>
			<td class="align_left">程序的设置</td>
		</tr>
		<tr>
			<td class="align_left">`db/`</td>
			<td class="align_left">数据库文件</td>
		</tr>
		<tr>
			<td class="align_left">`doc/`</td>
			<td class="align_left">程序的文档</td>
		</tr>
		<tr>
			<td class="align_left">`lib/`</td>
			<td class="align_left">代码库文件</td>
		</tr>
		<tr>
			<td class="align_left">`lib/assets`</td>
			<td class="align_left">代码库包含的资源文件，如 CSS、JavaScript 和 图片</td>
		</tr>
		<tr>
			<td class="align_left">`log/`</td>
			<td class="align_left">程序的日志文件</td>
		</tr>
		<tr>
			<td class="align_left">`public/`</td>
			<td class="align_left">公共（例如浏览器）可访问的数据，如出错页面</td>
		</tr>
		<tr>
			<td class="align_left">`script/rails`</td>
			<td class="align_left">生成代码、打开终端会话或开启本地服务器的脚本</td>
		</tr>
		<tr>
			<td class="align_left">`test/`</td>
			<td class="align_left">程序的测试文件</td>
		</tr>
		<tr>
			<td class="align_left">`tmp/`</td>
			<td class="align_left">临时文件</td>
		</tr>
		<tr>
			<td class="align_left">`vendor/`</td>
			<td class="align_left">第三方代码，如插件和 gem</td>
		</tr>
		<tr>
			<td class="align_left">`vendor/assets`</td>
			<td class="align_left">第三方代码包含的资源文件，如 CSS、JavaScript 和图片</td>
		</tr>
		<tr>
			<td class="align_left">`Rakefile`</td>
			<td class="align_left">`rake` 命令包含的任务</td>
		</tr>
		<tr>
			<td class="align_left">`Gemfile`</td>
			<td class="align_left">该程序所需的 gem</td>
		</tr>
		<tr>
			<td class="align_left">`Gemfile.lock`</td>
			<td class="align_left">一个 gem 的列表，确保本程序的复制版使用相同版本的 gem</td>
		</tr>
		<tr>
			<td class="align_left">`config.ru`</td>
			<td class="align_left">基于 Rack 的服务器所需的 Rack 配置，用于启动应用。</td>
		</tr>
	</tbody>
</table>

`app/` 下有三个子文件夹：`models`、`views` 和 `controllers`。Rails 采用了 MVC 架构，即采用业务逻辑、数据、界面显示分离的方法组织代码。

- **Model（模型）**：应用程序中用于处理应用程序数据逻辑的部分。通常模型对象负责在数据库中存取数据。

- **View（视图）**：应用程序中处理数据显示的部分。
通常视图是依据模型数据创建的。

- **Controller（控制器）**：应用程序中处理用户交互的部分。通常控制器负责从视图读取数据，控制用户输入，并向模型发送数据。

## 2. model

动作 - app/controllers/xxx.rb

||

V

视图生成 - app/views/xxx.html.erb

render => 回传预设的 template


## 参考:

官方指南

《Ruby on Rails Tutorial, Third Edition》

</br></br>
