# references (引用) #

**(In topic: Type and Programming Languages. chpt 13)**

---

函数抽象、基本类型、结构化类型（records、variants）都属于纯粹的语言特征，它们成了纯函数式语言（Haskell）、基本函数式的语言（ML）、命令式语言和面向对象语言的基本部分。

大部分实际的编程语言还包括各种非纯粹的部分。例如除了返回结果、对term进行求值，一些语言还可以向可变的变量赋值（例如引用单元、数组、可变的record field）、进行输入输出、进行非本地的控制转移（exceptions、jumps、continuations、进程间同步和通信等）。这些操作产生的影响可称为side effects或computational effects.

在ML类语言中，名字绑定和赋值语句的机制是分离的。值（value）x可以（与数字等）进行运算、但不能对x进行赋值；引用y可以通过value进行赋值，但不能直接进行加法等运算，而需要显式地进行解引用（dereference）、通过写成`!y`来获得当前的值。而在C类语言中（包括java），每一个变量名都关联一个可变单元，而解引用一个变量来获得其当前值的操作是隐式的。

### Basics ###

对于引用的基本操作有：分配allocation，解引用dereferencing，赋值assignment。

要分配一个reference，我们使用`ref`操作，为新单元提供一个初始值。

    r = ref 5;
    ▶ r : Ref Nat

则类型检查使得r的值是一个指向内存单元的引用，而该单元总是存放一个整数。要获得该单元的当前值，使用解引用操作符`!`。

    !r;
    ▶ 5 : Nat

要改变该单元中存放的值，使用赋值操作符。

    r := 7; 
    ▶ unit : Unit
    !r;
    ▶ 7 : Nat

### References and Aliasing(别名使用) ###

注意引用值与被引用的单元的区别. 如果通过绑定将一个引用值复制给另一个值，则只有引用被复制，而存储单元则没有。例如在上面的基础上：

    s = r;
    ▶ s : Ref Nat
    s := 82;
    ▶ unit : Unit
    !r;
    ▶ 82 : Nat

则引用`r`和`s`称为同一个单元的别名（*aliases*）。

### Garbage Collection ###

注意到尽管有引用的分配操作，然而我们并未提供释放不再使用的单元这一操作。 取而代之地，想很多现代语言一样，我们依赖运行时系统进行垃圾回收。这种做法并非仅仅考虑语言设计：当存在明确的deallocation操作时，想获得类型安全性是极其困难的。

### Typing 定型 ###

增加的定型规则（"=>"左边代表前提，右边代表结果）：

<span style="font-family: courier;">(T-Ref)  t1: T1 => ref t1 : Ref T1
</span>

<span style="font-family: courier;">(T-Deref)  t1: Ref T1 => !t1 : T1</span>

<span style="font-family: courier;">(T-Assign)  t1: Ref T1, t2 : T1 => t1 := t2 : Unit
</span>

### Evaluation 求值 ###

考虑到存储，我们需要扩展操作语义。由于求值结果会依赖于该求值所处的存储内容，因此求值规则不仅需要被求表达式、还需要存储（store）作为另一个参数。并且，由于求值过程可能会对存储产生side effect, 因此求值规则还需要返回一个store。所以求值关系的形式从 `t → t1`变为 `t | μ → t1 | μ1`。

对于原有的求值规则，在一步求值中存储不发生改变（因为没有side effect）。例如：

<span style="font-family: courier;">
(E-AppAbs)  (λx.t) v | μ → replace x with v in t | μ
</span>


<span style="font-family: courier;">
(E-App1)  t1 | μ → t1' | μ' => t1 t2 | μ → t1' t2 | μ'
</span>

<span style="font-family: courier;">
(E-App2)  t2 | μ → t2' | μ' => v1 t2 | μ → v1 t2' | μ'
</span>

除此之外，我们还需在表达式的语法上做一点改变。`ref`表达式的求值结果是一个新的存储中的location，因此值的类型中加入location类型的值：

<span style="font-family: courier;">
v ::=  &nbsp; &nbsp;&nbsp; &nbsp; values:<br />
&nbsp; &nbsp; λx:T.t &nbsp; abstraction value<br />
&nbsp; &nbsp; unit &nbsp; &nbsp;  &nbsp;unit value<br />
&nbsp; &nbsp; l &nbsp; &nbsp; &nbsp; &nbsp; store location<br />
</span>

需要注意的是，一个具体的location `l`只能是一个求值的中间结果，而并不直接暴露给程序员。

对dereference表达式`!t1`求值前，需要先将`t1`规约至一个value:

<span style="font-family: courier;">
(E-Deref)  t1 | μ → t1' | μ' => ! t1 | μ → ! t1' | μ'
</span>

`t1`的最终规约结果应当是一个location，否则如果试图对其他类型的值进行dereference操作，视为错误的。

<span style="font-family: courier;">
(E-DerefLoc)  μ(l) = v => !l | μ → v | μ
</span>

对于赋值表达式`t1:=t2`,我们要先将`t1`求值至一个值，然后再将`t2`求值至一个值：

<span style="font-family: courier;">
(E-Assign1)  t1 | μ → t1' | μ' => t1:=t2 | μ → t1':=t2 | μ' <br />
(E-Assign2)  t2 | μ → t2' | μ' => v1:=t2 | μ → v1:=t2' | μ'
</span>

当我们完成了对`t1`和`t2`的求值之后，我们得到了形如：`l:=v2`的表达式。对其进行求值就是使存储中位置`l`存放值`v2`:

<span style="font-family: courier;">
(E-Assign)  l:=v2 | μ → unit | assign l with v2 in μ
</span>

为求值形如`ref t1`的表达式，首先要将`t1`求值至一个值：

<span style="font-family: courier;">
(E-Ref)  t1 | μ → t1' | μ' => ref t1 | μ → ref t1' | μ'
</span>

接着要对`ref v1`进行求值，选择一个新的location `l`（不属于`μ`中已分配的部分），并生成一个新的store，在`μ`的基础上增加了将`l`映射到`v1`的绑定。最后整个表达式返回`l`这一新分配的location名字。

<span style="font-family: courier;">
(E-RefV)  l not in dom(μ) => ref v1 | μ → l | (μ, l to v1) 
</span>

<font color="#FF0000">注意到，</font>目前的求值规则中没有进行垃圾回收，而只是随着表达式求值的过程使存储一直增长。但这并不影响求值过程的正确性（毕竟，“垃圾”的含义就是存储中不可达进而不能再对求值过程产生影响的部分）。一个简单的求值器实现可能会耗尽内存。精致一些的求值器应当能够在某个location `l`成为垃圾后对其进行复用。

<!--
插入公式latex
<img align=center src="http://www.forkosh.com/mathtex.cgi?  公式">
-->

<br /><br /><br />