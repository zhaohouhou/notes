# System F #

**（Universal Types, types and programming languages, chpt 23）**

---

**多态系统(polymorphic systems)** 指允许一段代码作用于多种类型上的类型系统。

现代编程语言中的多态有以下几种：

- *参数多态*：允许一段代码的类型是“一般性的”，用变量来代替具体的类型，并在需要时实例化为具体的类型。参数化的定义是一致（*uniform*）的:它们的实例都具有同样的行为。
 
 - 参数多态的最强形式是impredicative/first-class polymorphism。
 
 - 更常用的形式是ML风格的let-polymorphism（见类型推导）。
  
- *Ad-hoc polymorphism*则是允许一个多态的值表示对于不同类型的不同行为。

 - Ad-hoc polymorphism最常见的例子是*重载*：一个函数符号与多个实现相联系，并且对于一个具体的函数作用，根据参数的类型，（静态的：由编译器；动态的：由运行时）选择具体的实现。
 
 - Ad-hoc polymorphism的一个更强的形式是intensional polymorphism
(Harper and Morrisett, 1995; Crary, Weirich, and Morrisett, 1998)，允许在运行时对类型进行有限的计算。intensional polymorphism能使得一些多态语言的高级实现成为可能，例如：tag-free garbage
collection, “unboxed” function arguments, polymorphic marshaling, and
space-efficient “flattened” data structures.

 - 而一个更强的ad-hoc polymorphism的形式是通过**typecase**原语（在运行时对类型进行任意地模式匹配）实现。Java的**instanceof**测试可以看作typecase的一种形式。

- 子类型多态（subtype polymorphism）。

以上分类并不是互斥的：同一个语言中多种多态方式可能1混合出现。例如：Standard ML同时提供了参数多态和对内建的算术运算的重载。而Java包括子类型、重载和简单的Ad-hoc polymorphism（instanceof）。

---

###System F###

背景：The system we will be studying in this chapter, commonly called System F,
was first discovered by Jean-Yves Girard (1972), in the context of proof theory
in logic. A little later, a type system with essentially the same power was developed,
independently, by a computer scientist, John Reynolds (1974), who
called it the polymorphic lambda-calculus. This system has been used extensively
as a research vehicle for foundational work on polymorphism and
as the basis for numerous programming language designs. It is also sometimes
called the second-order lambda-calculus, because it corresponds, via
the Curry-Howard correspondence, to second-order intuitionistic logic, which
allows quantification not only over individuals [terms], but also over predicates
[types].




<br /><br />