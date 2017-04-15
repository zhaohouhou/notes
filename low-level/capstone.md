##capstone使用


安装：下载、解压缩、make

学习使用：

1）写一小段测试程序、编译成可执行文件，再反汇编查看汇编代码（objdump -d）。

2）截取上面一段反汇编代码作为测试例。

使用capstone，编写程序如下：

	#include <stdio.h>
	#include <stdlib.h>
	#include <capstone/capstone.h>
	#include <inttypes.h>

	int main(){
		csh handle; 
		cs_arch arch = CS_ARCH_X86;
		cs_mode mode = CS_MODE_32;
		cs_insn *insn;
		char code[] = "\x53\x83\xec\x08\xe8\x97\x00\x00\x00";
		size_t size = sizeof(code) - 1;
	/*
	 80482b0:	53                   	push   %ebx
	 80482b1:	83 ec 08             	sub    $0x8,%esp
	 80482b4:	e8 97 00 00 00       	call   8048350 <__x86.get_pc_thunk.bx>
	*/

		uint64_t address = 0x1000; //(ASSUMED) address of the first instruction
		int err = cs_open(arch, mode, &handle);

		int v = cs_disasm(handle, code, size, address, 0, &insn);
		//memory of insn is allocated by cs_disasm()

		int j;
		if(v){
			printf("Disasm:\n");
			for (j = 0; j < v; j++) {
					printf("0x%"PRIx64":\t%s\t\t%s\n",insn[j].address, insn[j].mnemonic, insn[j].op_str);
			}
		}
		else{
			printf("Disasm failed.\n");
		}

		return 0;
	}

3）编译程序（其中-m32对应capstone库版本）：

	clang -m32  testCapstone.c -lcapstone

4）运行程序，输出：

	Disasm:
	0x1000:	push		ebx
	0x1001:	sub		esp, 8
	0x1004:	call		0x10a0








