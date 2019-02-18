# Kubernetes Basic

## 1. What is Kubernetes

Kubernetes（即k8s）是容器集群管理系统，是一个开源的平台，可以实现容器集群的自动化部署、自动扩缩容、维护等功能。
Kubernetes是Google 2014年创建管理的，是Google 10多年大规模容器管理技术Borg的开源版本。

通过Kubernetes可以：

- 快速部署应用
- 快速扩展应用
- 无缝对接新的应用功能
- 节省资源，优化硬件资源的使用

Kubernetes 特点

- 可移植: 支持公有云，私有云，混合云，多重云（multi-cloud）
- 可扩展: 模块化, 插件化, 可挂载, 可组合
- 自动化: 自动部署，自动重启，自动复制，自动伸缩/扩展

## 2. Basic concepts

1. **节点（Nodes）**：Kubernetes 是由一组节点组成，这些节点可以是物理主机，也可以是虚拟机，
节点上运行 Docker，kubelet，kube-proxy 等服务。Kubernetes 平台运行这些节点之上，构成了集群。
Kubernetes集群分为两种Node：Master Node 和用于实际部署的 Pod 的工作 Node。
一个集群往往由几个 Master Node 控制着整个集群的所有Node。

2. **容器集（Pod）**：Pod 是 Kubernetes 中能够创建和部署的最小单元，是 Kubernetes 集群中的一个应用实例。
Pod中包含了一个或多个容器，还包括了存储、网络等各个容器共享的资源。
Pod 在单个节点上运行（但是一个节点可以运行许多Pod），这意味着Pod 中的所有容器将具有相同的IP地址，
并且可以通过 localhost 上的端口来相互通信。Pod 在部署后无法更新，只能删除或替换。

    除非容器之间耦合严重，一般使用单容器 Pod。


3. **服务（Services）**：Kubernetes Service 定义了一组 Pods 和它们的访问方式。

4. **部署（Deployments）**: Deployment 为 Pod 和 ReplicaSet 提供了一个声明式定义(declarative)方法，
来方便的管理应用。典型的应用场景包括：

    - 定义 Deployment 来创建 Pod 和 ReplicaSet
    - 滚动升级和回滚应用
    - 扩容和缩容
    - 暂停和继续 Deployment

## 使用YAML创建一个 Kubernetes Depolyment

YAML 是一个 Json 的超集, 使用YAML进行k8s配置具有便捷性、可维护性和灵活性。Yaml 有 List 和 Map 两种数据类型。



## ref:


https://www.kubernetes.org.cn/k8s

https://www.kubernetes.org.cn/1414.html  使用YAML创建一个 Kubernetes Depolyment

http://dockone.io/article/8108  Kubernetes basic
