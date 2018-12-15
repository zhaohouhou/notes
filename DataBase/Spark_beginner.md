# Spark 笔记

2018-12-15, on Spark version 2.4.0

## Spark RDD

RDD（resilient distributed dataset，弹性分布式数据库？）是 Spark 提供的主要抽象：
a collection of elements partitioned across the nodes of the cluster that can be operated on in parallel. 

RDDs support two types of operations: *transformations*, which create a new dataset from an existing one, 
and *actions*, which return a value to the driver program after running a computation on the dataset.
For example, map is a transformation that passes each dataset element through a function and returns 
a new RDD representing the results. On the other hand, reduce is an action that aggregates all the
elements of the RDD using some function and returns the final result to the driver program.

ref: https://spark.apache.org/docs/latest/rdd-programming-guide.html

## Spark SQL

Spark 具有 Dataset 和 DataFrame 概念。DataFrame 概念上相当于关系型数据库的 table。
In Scala and Java, a DataFrame is represented by a Dataset of Rows.
