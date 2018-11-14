
# Discourse 笔记 2：开发搭建

2018-4-26

本节是关于 Discourse 的用于开发的搭建过程。搭建环境为 Ubuntu 16.04。

### 1. 安装Ruby和Rails运行环境

终端运行：

```shell
# Basics
whoami > /tmp/username
sudo add-apt-repository ppa:chris-lea/redis-server
sudo apt-get update
sudo apt-get install python-software-properties vim curl expect debconf-utils git-core build-essential zlib1g-dev libssl-dev openssl libcurl4-openssl-dev libreadline6-dev libpcre3 libpcre3-dev imagemagick postgresql postgresql-contrib-9.5 libpq-dev postgresql-server-dev-9.5 redis-server advancecomp gifsicle jhead jpegoptim libjpeg-turbo-progs optipng pngcrush pngquant gnupg2

# Ruby
curl -sSL https://rvm.io/mpapis.asc | gpg2 --import -
curl -sSL https://get.rvm.io | bash -s stable
echo 'gem: --no-document' >> ~/.gemrc

# exit the terminal and open it again to activate RVM

rvm install 2.3.4
rvm --default use 2.3.4 # If this error out check https://rvm.io/integration/gnome-terminal
gem install bundler mailcatcher

# Postgresql
sudo -u postgres -i
createuser --superuser -Upostgres $(cat /tmp/username)
psql -c "ALTER USER $(cat /tmp/username) WITH PASSWORD 'password';"
exit

# Node
curl -o- https://raw.githubusercontent.com/creationix/nvm/v0.33.2/install.sh | bash

# exit the terminal and open it again to activate NVM

nvm install node
nvm alias default node
npm install -g svgo
```

一些下载需要连接国外网站，可能下载速度很慢需要翻墙。

### 2. 从代码启动 Discourse

```shell
# install bundle and ruby if needed
sudo apt install ruby-bundler
sudo gem install bundler
sudo apt-get install ruby-dev

git clone https://github.com/discourse/discourse.git ~/discourse
cd ~/discourse
bundle install

# run this if there was a pre-existing database
# bundle exec rake db:drop
# RAILS_ENV=test bundle exec rake db:drop

# time to create the database and run migrations
bundle exec rake db:create db:migrate
RAILS_ENV=test bundle exec rake db:create db:migrate

# run the specs (optional)
bundle exec rake autospec # CTRL + C to stop
```

完成以上步骤后运行下面的命令启动 Discourse:

```shell
bundle exec rails s -b 0.0.0.0
```

启动完成后在本地的浏览器访问 http://localhost:3000 ，
即可看到欢迎页。此时由于未设置管理员账户，难以进行激活。运行下面的命令建立 admin 用户，并设置用户名、密码：

```shell
bundle exec rake admin:create
```

再启动 Discourse 即可进行访问主页、登录等操作，不需要激活。


## 参考:

官方指南

https://github.com/discourse/discourse/blob/master/docs/DEVELOPER-ADVANCED.md

</br></br>
