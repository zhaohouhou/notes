## Web安全之机器学习 -
# ch 2. KDD 99数据

************

本节主要研究KDD99数据集的使用。

## 1. KDD 99数据集介绍

KDD是数据挖掘与知识发现（Data Mining and Knowledge Discovery）的简称，KDD CUP是由ACM（Association for Computing Machiner）组织的年度竞赛。

”KDD CUP 99 dataset ”是KDD竞赛在1999年举行时采用的数据集。是一个模拟建立的网络环境中收集的网络连接数据，包含了多种不同的攻击手段。

KDDCUP历年题目：

    KDD-Cup 2008, Breast cancer
    KDD-Cup 2007, Consumer recommendations
    KDD-Cup 2006, Pulmonary embolisms detection from image data
    KDD-Cup 2005, Internet user search query categorization
    KDD-Cup 2004, Particle physics; plus Protein homology prediction
    KDD-Cup 2003, Network mining and usage log analysis
    KDD-Cup 2002, BioMed document; plus Gene role classification
    KDD-Cup 2001, Molecular bioactivity; plus Protein locale prediction.
    KDD-Cup 2000, Online retailer website clickstream analysis
    KDD-Cup 1999, Computer network intrusion detection
    KDD-Cup 1998, Direct marketing for profit optimization


下载地址：https://kdd.ics.uci.edu/databases/kddcup99/kddcup99.html

KDD99数据集每条数据包括42个元素，前41项为特征，第42项为数据标签（正常连接或网络攻击）。文件kddcup.names中描述了每个特征项的含义和类型（symbolic和continuous两种），以及攻击类型标签。

```
特征名称：
duration: continuous.
protocol_type: symbolic.
service: symbolic.
flag: symbolic.
src_bytes: continuous.
dst_bytes: continuous.
land: symbolic.
wrong_fragment: continuous.
...

特征向量：
0,tcp,http,SF,181,5450,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,8,8,0.00,0.00,0.00,0.00,1.00,0.00,0.00,9,9,1.00,0.00,0.11,0.00,0.00,0.00,0.00,0.00,normal.
0,tcp,http,SF,239,486,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,8,8,0.00,0.00,0.00,0.00,1.00,0.00,0.00,19,19,1.00,0.00,0.05,0.00,0.00,0.00,0.00,0.00,normal.
...
```

每个网络连接被标记为正常（normal）或异常（attack），异常类型被细分为4大类共39种攻击类型，其中22种攻击类型出现在训练集中，另有17种未知攻击类型出现在测试集中。

## 2. 数据预处理

对KDD99数据进行建模训练之前，首先要进行一些数据预处理。下面是一些可能需要进行的处理：

### 2. 1. 枚举类型转化为值型

枚举类型无法直接进行计算，需要转换为值型。
考虑要减小对于分类结果准确性的影响，可采用一串0/1值表示枚举值是否为枚举关键字中的某一项。

因此，可以首先从数据中统计一遍枚举关键字，形成每一个特征项的字典；在第二遍处理数据，将symbolic数据转换为0/1串。
（这种方式会增加特征的长度，因此关键字数量不能过多，否则需要考虑其他方式）


### 2. 2. 连续型特征

连续型特征的取值范围会影响一些算法的表现，最好训练前先对它们进行正规化（normalization）。

对于不同的特征，应该根据特征值的分布选择normalization formula。例如对接近高斯分布的特征值，可以使`X := (X-μ)/σ`

作为简单开始，这里我们使
`X := (X - X_min) / (X_max - X_min)`, 从而将特征取值范围变化到`[0, 1]`。因此，首先需要取得连续数据的最大值和最小值。

`sklearn.preprocessing`包含了一些有用的函数：
- `preprocessing.normalize()`: normalization
- `preprocessing.scale()`: scale
- `preprocessing.MinMaxScaler`: `fit_transform()`函数进行归一化

### 2. 3. 文本特征

文本特征提取可以采用词集模型或词袋模型（区别在于词袋模型统计了单词的出现次数）。

`sklearn.feature_extraction.text` 包含了处理工具类 `CountVectorizer`.

</br>

---

KDD99数据经过上面预处理，可得到含有118个特征的转换后数据。
在10%部分数据（约50万条数据）上进行实验，使用决策树模型可以训练得到准确率超过99.9%的准确率（区分攻击类型与否均可）。

另一个Web访问数据集HTTP DATASET CSIC 2010 (http://www.isi.csic.es/dataset/). Contains 36,000 normal requests and more than 25,000 anomalous requests.

---

####  ref:

https://en.wikipedia.org/wiki/Normalization_(statistics)







</br></br>
