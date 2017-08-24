# Python 时间序列预测

## 1. 时间序列 - 分解

### 组件

- 3个系统组件
	- 趋势: 长期上升或下降, 线性或非线性
	- 季节性:变量值以固定的时间区段(周期)上下起伏
	- 周期性: 变量值以不固定的时间区段(周期)上下起伏
- 非系统组件
	- 噪音

### 经典分解

- 相加分解
	- 适用条件: 周期性, 季节性和趋势的数量变化不随时间变化

- 相乘分解
	- 适用条件: 周期性, 季节性和趋势的数量变化随时间成比例变化
	- 通过对数转换变成相加分解

#### 例：季节性

- 在数据中与日期和时间相关
- 重复的, 周期性的模式
- 固定的周期
	- 周内效应  - 周内某几日的股票回报较其他几日高 (周期=5, 6, 或 7 天)
	- 季末效应 (周期=3个月)
	- 特例: 节假日

## 2. 平稳性(Stationarity)

在平稳的时间序列中,变量的统计属性(诸如<u>均值(mean)，方差(variance)，自相关(autocorrelation)，自协方差(autocovariance)</u> 等)不随时间改变

**严格平稳性**: 任意多个点的联合分布具有时间平移不变性。

**弱平稳**: 每个点的期望值以及相同距离两点之间的协方差不随时间变化。直觉上，满足弱平稳性的时间序列应该围绕相同的期望值做幅度不变的振荡。

如果均值和协方差都存在，则严格平稳性意味着弱平稳性。

#### 为什么需要时间序列的平稳性?

- 平稳的时间序列易于预测
- 得到有意义的变量间的统计属性

### 平稳性

现实的时间序列原始数据通常不稳定。如经济和商业数据中, 时间序列常存在趋势, 循环, random-walking或其他非平稳的行为。如果时间序列按照趋势调整调整后的变的平稳, 则称时间序列趋势平稳, 如按照差分调整后变的平稳称之为差分平稳。

### Unit root test(单位根检验)

决定时间需要的差分阶数的统计学测试
。序列中存在单位根，则为非平稳时间序列。

在计量经济学的自回归模型里，如果在 `y_t = a+ b*y_(t-1)+ epsilon _t`里，系数|b|=1，那么一个单位根是存在的。其中： y_t是在t 时刻的变量，b 是斜率系数，epsilon_t 是误差项。

Augmented Dickey–Fuller (ADF) 测试

- 虚无假设: 时间序列非平稳且非季节性

Kwiatkowski-Phillips-Schmidt-Shin (KPSS) 测试

- 虚无假设: 时间序列平稳且非季节性

#### Augmented Dickey-Fuller 测试
Test statistic显著小于critical value(5%) 则时间序列平稳

p-value 显著性概率值 <= 0.05

## 3. 常用调整时间序列的手段
- 对数转换 (logarithmic transformation)
- 季节调整 (seasonal adjustment)
- 第一差分 (first difference)
- 二阶差分 (second order difference)
- 季节差分 (seasonal difference)

### 3.1 差分

#### 第一差分:

	y'_t = y_t - y_(t-1)

#### 二阶差分:

```
y''_t = y'_t - y'_(t-1)
	= (y_t - y_(t-1)) - ( y_(t-1) - y_(t-2))
```

通常不超过二阶差分

### 3.2 差分

某一观测值与上一季的观测值的差分

	y'_t = y_t - y_(t-m)

m： 季节的数量

## 4.移动平均函数

移动平均函数 （Rolling Mean/Moving Average）： 前n天数值的均值。估计时间序列的趋势和周期。

#### 移动平均平滑

用数值前后n个时间点的值的平均值替代当值

### 移动均值Moving-average(MA)模型

`𝑌_𝑡` 时刻的预测值 `^𝑌_(𝑡+1)` 是最近𝑞个观测值的平均值

随机游走模型 (random walk): 𝑚=1. [如果在一个随机过程中，yt的每一次变化均来自于一个均值为零的独立同分布,称这个随机过程是随机游动。它是一个非平稳过程。]

### 自回归 (Auto-Regressive, AR)模型
- 假定变量与前期的值和白噪声具有线性关系
白噪声
- 不相关的随机变量序列, 是平稳序列
- 前提: 变量的自相关系数 >0.5

## 5. 建模

ARIMA模型步骤
- 平稳化时间序列 (指数转换, 差分, 等等)
- 决定差分的阶数 d
- 分析自相关函数和偏自相关函数来决定AR和MA项的数量 (p, q)

SARIMA模型（seasonal Autoregressive Integrated Moving Average），季节性差分自回归滑动平均模型。

SARIMAX function from statsmodels fits the corresponding Seasonal ARIMA model. Here, the order argument specifies the (p, d, q) parameters, while the seasonal_order argument specifies the (P, D, Q, S) seasonal component of the Seasonal ARIMA model. After fitting each SARIMAX()model, the code prints out its respective AIC score.

示例代码：

```python
import matplotlib.pyplot as plt
import numpy as np
from statsmodels.tsa.statespace.sarimax import SARIMAX


def getVariance(expect_y, predict_y):
    variance = 0
    for index in range(len(predict_y)):
        variance = variance + \
                   (expect_y[index] - predict_y[index]) ** 2
    return float(variance) / len(predict_y)

def getVarianceArr(expect_y, predict_y):
    var_arr = []
    for index in range(len(predict_y)):
        variance = abs(predict_y[index]-expect_y[index])/abs(expect_y[index])
        var_arr.append(variance)
    return var_arr

x_train = x[0:1000]
x_test = x[1000: len(x)]
npredict = 300
order = (1, 0, 1)
model = SARIMAX(np.asarray(x_train), order=order, seasonal_order=(2, 1, 0, 48), simple_differencing=False)
model_fit = model.fit(disp=0)
print(model_fit.summary())
output = model_fit.forecast(steps=npredict)
predictions = output
variants = getVarianceArr(x_test[0:npredict], predictions[0:npredict])
error = getVariance(x_test[0:npredict], predictions[0:npredict])

print('\n')
print('Printing Mean Squared Error of Predictions...')
print('Variance: %.6f' % error)
plt.subplot(211)
plt.plot(x_test, "blue")
plt.plot(predictions, "red")
plt.title("orig & predict")
plt.subplot(212)
plt.plot(variants, "black")
plt.title("diff rate")
plt.show()
```
