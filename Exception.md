# Exceptions (异常) #

**(In topic: Type and Programming Languages. chpt 14)**

---

在这一章节，我们继续扩展语言的计算模型：加入异常的产生和处理。

在实际编程中，我们可能会遇到某个函数因为某些原因不能正常执行任务、从而需要通知其调用者的情况。这样的情况有事可以通过使函数返回某些*variant*来解决。但另一些情况下，异常情况可能更加“异常”，因而我们并不希望每个调用者都要应对所有可能出现的情况；而是更希望异常情况能引起一个直接的控制转移、交由某个异常处理者，或简单地退出程序执行。

### Raising Exceptions ###

下面对简单的λ表达式语言进行拓展。引入语法项`error`，进行求值时，退出它所在的语法项的求值过程。新加入的规则如下：

<span style="font-family: courier;">(E-AppErr1)  error t1 -> error
</span>

<span style="font-family: courier;">(E-AppErr2)  v1 error -> error
</span>

<span style="font-family: courier;">(T-Error)  Γ┣ error: T
</span>

（由于`error`并不是一个值，因此
<span style="font-family: courier;">E-AppErr1</span>和
<span style="font-family: courier;">E-AppErr2</span>
所覆盖的情况并不会有所重合。保证了求值规则的确定性。）

<span style="font-family: courier;">T-Error</span>
决定了`error`可以作为任意类型出现。对于有
<span style="TEXT-DECORATION: underline">子类型</span>
的语言，可以为`error`赋予一个最小类型`Bot`，可根据需要提升（promoted）至任意类型。
对于具有<span style="TEXT-DECORATION: underline">参数多态</span>的语言，可为`error`赋予多态类型：`forall X.X`，可被实例化为任意类型。

这种扩充之后，well-type的语法项不再一定会求值为一个value，而也可能得到`error`（`error`不是value）。

### Handling Exceptions ###

对`error`的求值规则可看作“unwinding the call stack”。在引入了异常的语言中通常采取这样的做法，当异常发生时，连续丢弃栈上的活动记录，直到遇到一个*exception handler*（或直到栈为空）。即异常的作用是产生一个非局部的控制转移，转移的目标是最近的exception handler。

语法项中新增：

<span style="font-family: courier;">t ::= ... | try t with t</span>

新增求值规则：

<span style="font-family: courier;">(E-TryV)  try v1 with t2 -> v1
</span>

<span style="font-family: courier;">(E-TryError)  try error with t2 -> t2
</span>

<span style="font-family: courier;">(E-Try)  t1 -> t1' =>
try t1 with t2 -> try t1' with t2
</span>

新增定型规则：

<span style="font-family: courier;">(T-Try)  Γ┣  t1 : T, Γ┣  t1 : T =>
Γ┣ try t1 with t2 : T
</span>

### Exceptions and Continuations ###

#### 1. control stack ####

考虑下面的语法定义：

<span style="font-family: courier;">
 v -> n
</span>

<span style="font-family: courier;">
e -> v | e+e
</span>

以及对应的求值规则：

<span style="font-family: courier;">
 e1 -> e1' => e1+e2 -> e1'+e2
</span>

<span style="font-family: courier;">
 e2 -> e2' => v1+e2  -> v1+e2'
</span>

<span style="font-family: courier;">
  v1+v2  -> v      (where v=v1+v2)
</span>

上述规则实现简单，然而而每次的求值过程都需要传递尚未被求值的子表达式。可以考虑对一长串表达式进行求值的过程。为解决这一问题，我们可使用一个辅助性的栈结构来存放未来的求值结果，进而采用懒惰式的求值策略。一个栈帧`F`定义为：

<span style="font-family: courier;">
  F -> []+e | v+[]
</span>

其中<span style="font-family: courier;">[]</span>表示栈帧上的一个洞。为了得到一个完整的表达式，将`F`作用于表达式`e`：
<span style="font-family: courier;"> F[e]
</span>，则得到把`e`填入`F`的洞中得到的表达式。

栈`S`定义为栈帧组成的列表（<span style="font-family: courier;">.</span>
代表空）：

<span style="font-family: courier;">
 S -> . | F::S
</span>

则机器模型可表示为<span style="font-family: courier;"> M = (S, e)
</span>。其中`S`为当前的栈，`e`为正在被求值的表达式。

那么small-step的操作语义有：

<span style="font-family: courier;">
 (S, v1+v2) -> (S, v)    (where v=v1+v2)
</span>

<span style="font-family: courier;">
 (F::S, v) -> (S, F[v])
</span>

<span style="font-family: courier;">
 (S, e1+e2) -> ([]+e2::S, e1)
</span>

<span style="font-family: courier;">
 (S, v1+e2) -> (v1+[]::S, e2)
</span>

则计算的初始状态为<span style="font-family: courier;">(., e)</span>，结束时为<span style="font-family: courier;">(., v)</span>，其中`v`为`e`的求值结果。

#### 2. exceptions ####

考虑扩展的语法定义：

<span style="font-family: courier;">
 v -> n
</span>

<span style="font-family: courier;">
 e -> v | e+e | throw | try e catch e
</span>

其中`throw`意为便抛出一个无名字并且不包含值的异常。由于`throw`应当能出现在任意表达式的位置，因而将类型系统定义如下：

<span style="font-family: courier;">
 T -> Int | Any
</span>

`throw`具有`Any`类型，并且`Any`应可以视作其它任意类型。

相应地扩展帧的定义：

<span style="font-family: courier;">
 F -> []+e | v+[] | try [] catch e
</span>

那么形式化的操作语义也进行相应的扩展;

<span style="font-family: courier;">
 (E-Try) (S, try e1 catch e2) -> (try [] catch e2::S, e1)
</span>

<span style="font-family: courier;">
 (E-Throw) (S1@((try [] catch e2)::S2)), throw) -> (S2, e2)
(S1 does not contain a try-catch)
</span>

那么对于规则<span style="font-family: courier;">(E-Throw)</span>，表示在遇到`throw`时在栈上找到第一个try-catch结构，并对其catch子表达式进行求值。这在编译器文献称作*stack unwinding*。

#### 3. value carrying exceptions ####

异常也可以携带值。我们将语法扩展如下：

<span style="font-family: courier;">
  v -> n
</span>

<span style="font-family: courier;">
  e -> v | e+e | throw e | try e catch e
</span>

那么现在`throw`语句携带了一个表达式`e`，即异常。相应的类型系统和定型规则为：

<span style="font-family: courier;">
  T -> Int | Any | Exn
</span>

<span style="font-family: courier;">
 (T-Throw) ┣ e: Exn =>
  ┣ throw e: Any
</span>

<span style="font-family: courier;">
 (T-Try) ┣ e1: T    ┣ e2: Exn->T =>
  ┣ try e1 catch e2: T
</span>

其中，(T-Try)规则说明catch子语句具有Exn->T的函数类型，即以一个异常为参数返回T类型的表达式。

#### 4. stack as function compositions ####

对于控制栈的另一种观点则是函数复合，即：

<span style="font-family: courier;">
 F::S = F o S
</span>

（`F o S`表示函数复合形成的新函数，注意其含义。不一定与`fn x => G (F x)`相同？eg.对于case)

那么，求值规则将变为下面的形式：

<span style="font-family: courier;">
 (S, e1+e2) -> ((λx.x+e2) o S, e1)
</span>

<span style="font-family: courier;">
 (S, v1+e2) -> ((λx.v1+x) o S, e2)
</span>

<span style="font-family: courier;">
 (S, v1+v2) -> (S, v)  (where v=v1+v2)
</span>

<span style="font-family: courier;">
 (S, v) -> (., S(v))   (stack application rule)
</span>

#### 4. continuations ####

我们扩展语义引入continuation：

<span style="font-family: courier;">
 v -> n | cont(S)
</span>

<span style="font-family: courier;">
 e -> v | e+e | callcc (λk.e) | appcc e e
</span>

类型系统和定型规则：

<span style="font-family: courier;">
 T -> Int | Cont (T)
</span>

<span style="font-family: courier;">
 G ┣ S: T =>
  G ┣ cont(S): Cont(T)
</span>

<span style="font-family: courier;">
 G, x: Cont(T) ┣ e: T =>
  G ┣ callcc (λx.e): T
</span>

<span style="font-family: courier;">
 G ┣ S: T =>
  G ┣ cont(S): Cont(T)
</span>

扩充帧的定义：

<span style="font-family: courier;">
  F -> ... | appcc [] e2 | appcc v []
</span>

操作语义如下：

<span style="font-family: courier;">
  (E-Cont)
  (S, callcc (λx.e)) -> (S, [x|->cont(S)]e)
</span>

<span style="font-family: courier;">
  (E-Appcc1)
  (S, appcc (cont(S')) v2) -> (S', v2)
</span>

<span style="font-family: courier;">
  (E-Appcc2)
  (S, appcc v1 e2) -> (app v1 []::S, e2)
</span>

<span style="font-family: courier;">
  (E-Appcc3)
  (S, appcc e1 e2) -> (appcc [] e2::S, e1)
</span>

Essentially, `callcc` will duplicate and pass the current
control stack to e, whereas `appcc` destroy the current stack
and restore the saved old stack (thus transfer control
to some alternate universe).

<br /><br /><br />