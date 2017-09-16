# Python日期和时间

## time模块

基本的时间处理模块，可以获得当前时间

```python
>>> import time

>>> time.time()   # 返回从1970年1月1日零时到现在时刻的秒数

>>> print time.localtime()   # 返回一个表示本地当前时间的结构体
time.struct_time(tm_year=2017, tm_mon=9, tm_mday=17, ...)
>>> print time.localtime(1e8)   # 也可以是其他时间
time.struct_time(tm_year=1973, tm_mon=3, tm_mday=3, ...)

>>> print time.strftime('%Y-%m-%d %H:%M:%S', time.localtime()) # pretty printing
2017-09-17 01:12:11
```

## calendar模块

可以获得日历

```python
>>> import calendar

>>> print calendar.month(2017, 7)
     July 2017
Mo Tu We Th Fr Sa Su
                1  2
 3  4  5  6  7  8  9
10 11 12 13 14 15 16
17 18 19 20 21 22 23
24 25 26 27 28 29 30
31

```

## datetime模块

datetime模块可以获得微秒级的时间。

```python
>>> import datetime
# There're some differences between python 2.7 and 3.4.
# Below are codes for python 2.7 .

>>> d = datetime.datetime.now()
>>> print d
2017-09-17 01:31:13.688000
>>> print d.second
13
>>> print d.microsecond
688000
```


<br/><br/>
