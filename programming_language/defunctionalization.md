# Defunctionalization 函数消去#

**（Defunctionalization at Work, Olivier Danvy and Lasse R. Nielsen, 2001）**

---

**函数消去**技术是将程序整个由高阶（higher-order）到一阶（first-order）函数的变换。

一阶程序中，所有函数都被命名，所有对函数的调用都通过函数名对其进行引用。在高阶程序中，函数可以是无名的，并作为参数传递、作为返回值被返回。Strachey指出，在一阶程序中，函数是二等的可表示的值；在高阶函数中，函数是一等的可表示的值。

- 一等的函数通常表示为闭包--pair:(code, freevars)。--eager
functional programming
- 高阶程序还可以通过函数消除变为一阶程序（Reynolds,1970's）。
- 一等的函数还可以通过将函数式程序转化成*连接符(combinators)*并使用图归约（Turner, 1970's）。--lazy functional
programming

steps: 
 
1.**decode the dataflow**, that is, to isolate the expressions that create or use or pass on the value of higher-order arguments or results. The simplest way of doing this is to introduce a data type that encapsulates(封装) these values, together with a trivial function apply that unwraps them when necessary.

2.**look for lambdas**, that is, to identify the functions that get wrapped up in the new data type and passed as functional arguments. The point of doing this is so that we can find the **free variables** of each lambda-expression.
 
3.**create the constructors**: that is, redefine the data type to reflect the lamda-expressions we have found.

4.**write the worker**: that is, to redefine *apply* because its task is no longer trivial. 

<br /><br />