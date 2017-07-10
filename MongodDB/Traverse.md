对Mongodb中大量数据进行遍历。遇到的一些问题。


### 1. 循环写法

```python
for doc in col.find()
  ...
```

与

```python
docs = col.find()
for doc in docs
  ...
```

两种写法速度上有很大差异。后者需要一次获得所有结果，因此会有长时间卡顿。

### 2. 按页查找

Mongodb默认添加按照_id域的索引。数据量过大程序可能卡死的情况，可考虑按页进行遍历。python代码：

```python
PAGE_SIZE = 100

def find_page(col, index):
  '''
  :param col: Mongodb collection
  :param index: page index (starts from 0)
  :return: documents
  '''
  docs = col.find(limit=PAGE_SIZE, skip=PAGE_SIZE*index)
  return docs

...
for page_index in range(start, end):
  docs = find_page(col, page_index)
  for doc in docs:
    ...

```
