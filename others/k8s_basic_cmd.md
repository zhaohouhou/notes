### 查看已有pods：

```bash
kubectl get pods
kubectl get pods -n $namespace
```
 
###  进入运行的pod（开启一个bash）

```bash
kubectl exec -it $pod_name -- /bin/bash
kubectl exec -it $pod_name -n $namespace -- /bin/bash
```

###  重启 pod
```bash
kubectl [-n NAME_SPACE] rollout restart deployment POD_NAME
```

###  k8s查看容器日志
---查看运行中指定pod以及指定pod中容器的日志

#### 1、查看指定pod的日志
kubectl logs <pod_name>
kubectl logs -f <pod_name> #类似tail -f的方式查看(tail -f 实时查看日志文件 tail -f 日志文件log)

#### 2、查看指定pod中指定容器的日志

  kubectl logs <pod_name> -c <container_name>

PS：查看Docker容器日志 

  docker logs <container_id>

###  config map

查看： kubectl get cm
修改：kubectl edit cm {config_name}
kubectl decribe cm {config_name}

### ref：
https://k8smeetup.github.io/docs/home/
