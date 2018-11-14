
# Discourse 搭建笔记 1：开始

2018-4-25

## 1. 什么是 Discourse

Discourse 是一个开源的论坛框架。Stack Overflow 的联合创始人 Jeff Atwood 在2013年推出了 Discourse 项目，致力于为未来的 Internet 打造交流平台。

特点：

- 开源

- 阅读优化：页面简洁，滚动屏幕代替翻页，等等

- 为移动设计：兼容移动设备和平板

- 方便管理：统计，安全，插件，升级，防灌水，等等

对比 Discuz ：

- Discourse 部署在 Docker 上，服务器至少 1 GB 内存（需要 swap），推荐 2 GB 以上内存

- 服务器后端为 Ruby on Rails，数据库为 Postgres，Redis cache。

- 入门难度较大。

## 2. Discourse 的本地简单部署

以下以 Ubuntu 环境为例介绍基本搭建过程。

1. 安装 Docker、Git，略。（若版本太低则需要升级）（Discourse 安装前需要先启动 Docker 服务）

2. 从源代码安装 Discourse

    - 切换至 root 用户

    - 新建文件夹：  `/var/discourse`

    - 获取代码：

          # git clone https://github.com/discourse/discourse_docker.git /var/discourse

3. 配置 Discourse

    安装命令：

        # cd /var/discourse
        # ./discourse-setup

    需要进行一些配置：

    <table>
      <tr>
        <th>配置项</th>
        <th>示例</th>
      </tr>
      <tr>
        <td>Hostname</td>
        <td>xxx.com</td>
      </tr>
      <tr>
        <td>Email address for admin account</td>
        <td>6666@qq.com</td>
      </tr>
      <tr>
        <td>SMTP server address</td>
        <td>smtp.qq.com</td>
      </tr>
      <tr>
        <td>SMTP port</td>
        <td>587</td>
      </tr>
      <tr>
        <td>SMTP user name</td>
        <td>6666@qq.com</td>
      </tr>
      <tr>
        <td>SMTP password</td>
        <td>"sdjgakfgsduyf"</td>
      </tr>
    </table>

    确认配置之后，脚本会生成 `app.yml` 配置文件，并开始 bootstrap 。这一过程需要一段时间（10分钟左右）。

4. 修改配置

    如果需要修改配置，可编辑 /containers/app.yml 文件。要使改动生效，需要运行：

        ./launcher rebuild app

    （关闭 docker 容器：`./launcher stop app` 或 `sudo docker stop app` ，会停止服务）

安装完成后，访问设定的 Hostname 会显示 Discourse 欢迎页面。现在可以根据提示发送激活邮件、激活管理员帐号，进行论坛的基本设置。

## 安装过程 Tips

安装过程中可能会遇到一些问题，需要针对具体情况来解决。下面是针对一些可能情况的解决方案。

- Docker 加速

    如果安装过程中 docker 连接不成功，可能需要使用国内 docker 镜像。在 /etc/docker/daemon.json （没有该文件的话，新建一个）添加：

    ```json
    {
      "registry-mirrors": ["https://docker.mirrors.ustc.edu.cn"]
    }
    ```

    重新启动 docker 后台服务：

        sudo service docker restart

- QQ 邮箱设置

    QQ 邮箱的 SMTP 密码是 “QQ 邮箱授权码”，可在 QQ 邮箱的账户设置中获取。

    如果接收不到激活邮件，可尝试修改 `app.yml`，在文件底部添加：

        - exec: rails r "SiteSetting.notification_email='xxx@qq.com'"

- Hostname

    Discourse 安装需要配置 Hostname，在本地尝试搭建时为了方便可以手动为 ip 地址添加域名。Ubuntu 中，修改 `/etc/hosts` 文件，添加：

        xxx.xxx.xx.xx     xxx.com

## 参考:

中文站 http://www.discoursecn.org/about/

Discourse安装及基本配置：

https://www.digitalocean.com/community/tutorials/how-to-install-discourse-on-ubuntu-16-04

https://meta.discoursecn.org/t/topic/26

https://blog.csdn.net/chris_mao/article/details/79398393

使用QQ邮箱可能遇到的一些问题

https://blog.csdn.net/dawn_moon/article/details/79004459

</br></br>
