# C 语言纯汇编函数

（linux系统下）由于需要测试简单函数（仅含有add mov ret指令，没有push pop），因此需要手动编写汇编函数。

### 直接编写.s文件：

	.global foo

	foo:
		movl	$1, %eax
		movl	$2, %ecx
		addl	%ecx, %eax
		ret

结果能够正确运行，但`foo`无法被识别为一个函数，并且elf文件头没有大小信息。

## 使用纯汇编函数

为了保持栈平衡，需要阻止编译器生成prolog(`push ebp, ...`)，否则会segment fault。

### 使用clang编译（c文件）：

	__attribute__ ((naked)) void foo()
	{
    	__asm__("movl	$1, %eax\n"
        	    "movl	$2, %ecx\n"
            	"addl	%ecx, %eax\n"
            	"ret\n");
	}

成功运行（返回结果为3）

### VC/C++ 示例:

    __declspec(naked) int  add(int a,int b)  
    {  
       __asm mov eax,a  
       __asm add eax,b  
       __asm ret   
    }  

<br/>

    __declspec(naked) int __stdcall function(int a,int b)  
    {  
        __asm mov eax,a  
        __asm add eax,b  
        __asm ret 8        //注意后面的8  
    }  
