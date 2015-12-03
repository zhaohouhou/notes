#Google APAC test 2016-round D 总结

这次记得时间参加上了，晚了几分钟。但是时间安排出了问题结果很悲剧。

##Problem A Dynamic Grid

R行C列的矩阵，每个元素值为0或1.有两种操作：更改某个元素的值为0或1；查询值为1的联通域的个数（只能经过边联通、不能经过顶点联通）。

**分析：**

使用递归的染色来区分联通域。初始先计算联通域个数。对某个位置进行更改时只需根据与该单元相连通的联通域个数（即把该单元作为0后，四周的联通域个数），再根据修改的境况增加或减少联通域总数。

这题比较容易（处理输入时略烦），分值少，比赛时没做。

##Problem B gBallon

一个高塔（看作y轴）收集一些空中的气球。每个气球的横坐标为Pi,高度为Hi。有M个不同的高度，对应不同的风速（水平）。正的风速表示每个单位时间使气球像正方向移动，反之亦然。

工程师可以将气球上下移动，所花费的能量为|H_original-H_new|（不花费时间）。总共的能量值为Q。

给定气球个数N、所有气球的初始位置，所有高度的个数M、各层风速，以及总能量值Q。求最快的能够收集所有气球的时间(如果不能则输出IMPOSSIBLE)。

**分析：**

很容易想到是个背包问题。背包总容量为Q。dp[i][j]表示容量为j的情况下、要收集前i个气球所需要的最少时间。

首先要计算每个气球要被收集有哪些option，如果没有可用的option(对横坐标不为零的气球，风速与横坐标符号相反且花费小于Q)则直接输出IMPOSSIBLE。动态规划计算dp[i][j]时每步找到气球i的option，使得：j>=option_cost，并且max(dp[i-1][j-option_cost],option_time)最小。

<font color="#FF0000">**存在问题：**</font>为了便于计算对每个气球的option_time做了排序，然而调用的位置写错了。并且其实没必要排序，或者只对第一个气球排序就可以了。这题写起来其实比较容易，然而比赛时没写。

##Problem C IP Address Summarization

给出一堆IPv4的子网地址，输出排序后的且正规化（即掩码之外的位都为0）的**最短的**列表，使得：某个地址属于输入的子网**当且仅当**它属于输出的某个子网。

**分析：**

这个本来并不难，但是没理解题意，以至于好久都没做对，时间都花在上面了。关键点其实就是如果两个子网地址能合并的时候要合并，这样才能叫做“最短的”列表。

<font color="#FF0000">**存在问题：**</font>关键问题是由于采用的是数组，合并的时候应该是多趟合并，因为这个大数据没过。中且途判断子网从属关系的时候条件少写了。

##Problem D. Virtual Rabbit

一个虚拟宠物兔子，如果时间X没有被喂就会饿死（可正好在该时间喂）。主任起床时间为Q，开始工作时间为W，回家时间为H，睡觉时间为B。即[W, H)和[B, G)时间不能喂食。

兔子在第0天00:00:00递交主人（这时肚子是饱的），问要使兔子在第D天00:00:00依旧存活，所需要的最少投喂次数。输入为hh:mm:ss形式的G, W, H, B, X，以及正整数D。

**分析：**

就是分情况计算就行了。需要注意的地方是最后一天的情况不同，应当另外计算（采用往回退的方式处理最后一天结果不对）。可采用一个全局变量die表示剩余存活时间（最晚下次要喂的时间）。

对大数据情况（1 ≤ D ≤ 10^14）需要提高速度。具体可以采用一个24*3600长度的数组保存不同的die值对应的投喂次数快照与剩余天数的快照，当下次遇到相同的die即可直接计算。

<font color="#FF0000">**存在问题：**</font>还是有语句没注意位置写错了。并且对于大数据，D要用long long型存储，表示<u>**与其直接计算的中间变量、输入输出**</u>都要用long long型！


##总结

就这样惨淡地结束了。

</br></br>