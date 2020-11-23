# Docker基本使用

## 1. 安装

https://docs.docker.com/install/overview/

以下是 Ubuntu 系统上的安装(非手动)：

### SET UP THE REPOSITORY

1. Update the apt package index: `$ sudo apt-get update `

2. Install packages to allow apt to use a repository over HTTPS:
    ```
    $ sudo apt-get install \
        apt-transport-https \
        ca-certificates \
        curl \
        software-properties-common
    ```

3. Add Docker’s official GPG key:
    ```
    $ curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
    ```

4. set up the stable repository
    ```
    sudo add-apt-repository \
    "deb [arch=amd64] https://download.docker.com/linux/ubuntu \
    $(lsb_release -cs) \
    stable"
    ```

### INSTALL DOCKER CE

1. Update the apt package index.
    ```
    sudo apt-get update
    ```

2. Install the latest version of Docker CE:
    ```
    sudo apt-get install docker-ce
    ```

3. You can verify that Docker CE is installed correctly by running the hello-world image.
    ```
    sudo docker run hello-world
    ```

## 2.基本概念

- 镜像（Image）：是一个文件系统，包含容器运行所需要的数据
- 容器（Container）：docker运行实例
- 仓库（Repository）：docker镜像的托管仓库（类似github）

## 3. 基本使用

### 1） Build an Image

1. 通过编写 Dockerfile 可以建立一个新的 docker image。Dockerfile 包含了一系列命令，来告诉 docker 如何 build 一个 image。

2. 执行命令 `docker build -t $tag $dir`

    其中，-t 参数用于指定镜像的名字，最后的参数是 build 目录（即 dockerfile 位于的目录）

3. 启动这个image的一个container：

    ```
    docker run $image_name $command
    ```

###  2） 常用命令

|  cmd |  |
| - | - |
| service docker start | 启动docker服务 |
| service docker stop | 关闭docker服务 |
| docker images | 查看本地镜像 |
| docker ps -a	| list all containers |
| docker rmi IMAGE_ID	| 删除本地镜像 |
| docker rm CONTAINER_ID | 删除容器 |
| docker run IMAGE_NAME	| 建立image的一个容器并启动它 |
| docker start/stop/restart/kill | CONTAINER_NAME	启动/关闭/重启/杀掉容器 |
| docker save IMAGE_ID -o FILE_NAME.tar <REPOSITORY:TAG> | image打包（本地需要有这个image） |
| docker rename OLD_NAME NEW_NAME	| 重命名容器 |
| docker attach CONTAINER_NAME	| 进入一个后台运行的容器终端（退出后容器会停止）.退出容器使用exit命令 |
| docker cp [OPTIONS] $SRC $DEST | 拷贝文件. src和dest前加<containerID/containerName:>来指明是对container中的目标做操作. e.g.:  `docker cp containerId:/foo.txt foo.txt` |
| docker exec -it CONTAINER_NAME /bin/bash | 在container中启动一个bash shell .<br> -t: flag assigns a pseudo-tty or terminal inside the new container.(进入终端).<br> -i: flag allows you to make an interactive connection by grabbing the standard in (STDIN) |

docker run 常用参数:

|  option | desc |
| - | - |
| -d | 以后台模式启动一个容器(detached) |
| --rm  | 退出运行后删除容器（不能与-d同时使用，不能自动清理detached容器） |
| -v $path_container:$path_host[:$option] | 映射宿主机文件到container. $option 选项例如只读(ro) |
| --restart $restart_option | 自动重启. restart_option 有 always/unless_stopped等. https://docs.docker.com/config/containers/start-containers-automatically/ |
| -p $host_port:$container_port | 端口映射 |
| --name=$container_name | 指定container的名字 |


## 4. some debug practice with docker

### 在容器中使用 gdb attach 到进程

直接在 docker 容器中用 gdb attach 运行中进程可能会报错:
`Could not attach to process. If your uid matches the uid of the target process, check the setting of /proc/sys/kernel/yama/ptrace_scope, or try again as the root user...`. 
但是直接修改 /proc/sys/kernel/yama/ptrace_scope 会遇到 "read only file system".

solution:

1. `docker exec --privileged -ti <container> bash` 可以直接在 bash 中使用 gdb attach.

2. 比较旧的 docker 版本的 docker exec 可能没上面的功能, 需要在 docker run 时添加 --privileged 参数,
然后再 docker exec bash 就可以 attach 到进程.

ref: 

https://docs.docker.com/engine/reference/commandline/exec/#options

https://bitworks.software/en/2017-07-24-docker-ptrace-attach.html

### docker container 停止原因

- docker log 定位原因。
- 根据 exit code 判断原因。（docker ps 可以查看到已经停止了的container的 exit code。前提是容器没被删掉。） 

### 收集 docker 中的 core dump 文件

有 core dump 文件， 可以在 debugger 中还原程序崩溃时的状态（`gdb $prog $core_file`）。对解决 segment fault 这种问题很好用。

- 需要确保 container 中存在写 core dump 文件的路径。core 文件生成使用的是host的配置。配置文件在 /proc/sys/kernel/core_pattern . 可以添加一个路径：

    ```
    echo '/tmp/core.%e.%p' | sudo tee /proc/sys/kernel/core_pattern
    ```
    并且在Dockerfile中添加 `RUN mkdir /tmp`

- enable core dump

    - 宿主机执行 `ulimit -c unlimited` 去除 Core Dump 文件大小限制。（`ulimit -a` 查看现有权限）
    - docker run 的时候加上参数： `--ulimit core=-1`.
