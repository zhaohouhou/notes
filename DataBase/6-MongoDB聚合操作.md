# MongoDB 聚合

ref:  http://www.runoob.com/mongodb/mongodb-aggregate.html

MongoDB中聚合(aggregate)主要用于处理数据(诸如统计平均值,求和等)，并返回计算后的数据结果。类似sql语句中的 count(\*)。

### 基本语法


    >db.COLLECTION_NAME.aggregate(AGGREGATE_OPERATION)

语法：

    aggregate([{$group:{_id:"$fieldName",alias:{分组函数:"$fieldName"}}}]) 

例如，如下命令统计集合中文档个数：

```shell
db.COLLECTION_NAME.aggregate(
   [
      { $group: { _id: null, count: { $sum: 1 } } }
   ]
)
```

计算平均值：

```shell
db.COLLECTION_NAME.aggregate(
  [{$group : {_id : null, avg_value : {$avg : "$FIELD_NAME"}}}]
)
```

注意，聚合的FIELD_NAME前加'$'。
