#修改内存权限（Linux）

在Linux中，mprotect()函数可以用来修改一段指定内存区域的保护属性。

函数原型：

		#include <unistd.h>  
    	#include <sys/mman.h>  
    	int mprotect(const void *start, size_t len, int prot);  

prot可以取以下几个值，并且可以用“|”将几个属性合起来使用：

1）PROT_READ：表示内存段内的内容可写；

2）PROT_WRITE：表示内存段内的内容可读；

3）PROT_EXEC：表示内存段中的内容可执行；

4）PROT_NONE：表示内存段中的内容根本没法访问。

	#define PROT_READ	0x1     /* Page can be read.  */
	#define PROT_WRITE	0x2     /* Page can be written.  */
	#define PROT_EXEC	0x4     /* Page can be executed.  */
	#define PROT_NONE	0x0     /* Page can not be accessed.  */

<font color = 'red'>注意：指定的内存区间必须包含整个内存页（4K）。区间开始的地址start必须是一个内存页的起始地址，并且区间长度len必须是页大小的整数倍。</font>

如果执行成功，则返回0；如果执行失败，则返回-1，并且设置errno变量(Linux错误代码常量，位于errno.h)

错误的原因主要有以下几个：

1）EACCES

该内存不能设置为相应权限。这是可能发生的，比如，如果你 mmap(2) 映射一个文件为只读的，接着使用 mprotect() 标志为 PROT_WRITE。

2）EINVAL

start 不是一个有效的指针，指向的不是某个内存页的开头。

3）ENOMEM

内核内部的结构体无法分配。

4）ENOMEM

进程的地址空间在区间 [start, start+len] 范围内是无效，或者有一个或多个内存页没有映射。 

如果调用进程内存访问行为侵犯了这些设置的保护属性，内核会为该进程产生 SIGSEGV （Segmentation fault，段错误）信号，并且终止该进程。

##附1 示例代码

```C
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>


int foo()
{
	int i = 10;
	int j = 200;
	return (i+j);

}

int main(){
	int pagesize = 1024 * 4;
	printf("pagesize %x\n", pagesize);
	int r = mprotect(foo - (int)foo%pagesize, 
				pagesize * 1, 
				PROT_READ | PROT_WRITE | PROT_EXEC);
	if(r < 0){
		printf("mprotect failed\n");
		if(errno == 13)//EACCES
			printf("EACCES: Permission denied\n");
		if(errno == 22)//EINVAL
			printf("EINVAL: wrong start pointer %x\n", foo - (int)foo%pagesize);
		if(errno == 12)//ENOMEM
			printf("ENOMEM: invalid scope\n");
		return 0;
	}
		
	printf("hello %d\n", foo());
	return 0;
}
```

##附2 Linux Error Code

```
C Name    Value    Description
EPERM    1    Operation not permitted
ENOENT    2    No such file or directory
ESRCH    3    No such process
EINTR    4    Interrupted system call
EIO    5    I/O error
ENXIO    6    No such device or address
E2BIG    7    Arg list too long
ENOEXEC    8    Exec format error
EBADF    9    Bad file number
ECHILD    10    No child processes
EAGAIN    11    Try again
ENOMEM    12    Out of memory
EACCES    13    Permission denied
EFAULT    14    Bad address
ENOTBLK    15    Block device required
EBUSY    16    Device or resource busy
EEXIST    17    File exists
EXDEV    18    Cross-device link
ENODEV    19    No such device
ENOTDIR    20    Not a directory
EISDIR    21    Is a directory
EINVAL    22    Invalid argument
ENFILE    23    File table overflow
EMFILE    24    Too many open files
ENOTTY    25    Not a tty device
ETXTBSY    26    Text file busy
EFBIG    27    File too large
ENOSPC    28    No space left on device
ESPIPE    29    Illegal seek
EROFS    30    Read-only file system
EMLINK    31    Too many links
EPIPE    32    Broken pipe
EDOM    33    Math argument out of domain
ERANGE    34    Math result not representable
EDEADLK    35    Resource deadlock would occur
ENAMETOOLONG    36    Filename too long
ENOLCK    37    No record locks available
ENOSYS    38    Function not implemented
ENOTEMPTY    39    Directory not empty
ELOOP    40    Too many symbolic links encountered
EWOULDBLOCK    41    Same as EAGAIN
ENOMSG    42    No message of desired type
EIDRM    43    Identifier removed
ECHRNG    44    Channel number out of range
EL2NSYNC    45    Level 2 not synchronized
EL3HLT    46    Level 3 halted
EL3RST    47    Level 3 reset
ELNRNG    48    Link number out of range
EUNATCH    49    Protocol driver not attached
ENOCSI    50    No CSI structure available
EL2HLT    51    Level 2 halted
EBADE    52    Invalid exchange
EBADR    53    Invalid request descriptor
EXFULL    54    Exchange full
ENOANO    55    No anode
EBADRQC    56    Invalid request code
EBADSLT    57    Invalid slot
EDEADLOCK     -    Same as EDEADLK
EBFONT    59    Bad font file format
ENOSTR    60    Device not a stream
ENODATA    61    No data available
ETIME    62    Timer expired
ENOSR    63    Out of streams resources
ENONET    64    Machine is not on the network
ENOPKG    65    Package not installed
EREMOTE    66    Object is remote
ENOLINK    67    Link has been severed
EADV    68    Advertise error
ESRMNT    69    Srmount error
ECOMM    70    Communication error on send
EPROTO    71    Protocol error
EMULTIHOP    72    Multihop attempted
EDOTDOT    73    RFS specific error
EBADMSG    74    Not a data message
EOVERFLOW    75    Value too large for defined data type
ENOTUNIQ    76    Name not unique on network
EBADFD    77    File descriptor in bad state
EREMCHG    78    Remote address changed
ELIBACC    79    Cannot access a needed shared library
ELIBBAD    80    Accessing a corrupted shared library
ELIBSCN    81    A .lib section in an .out is corrupted
ELIBMAX    82    Linking in too many shared libraries
ELIBEXEC    83    Cannot exec a shared library directly
EILSEQ    84    Illegal byte sequence
ERESTART    85    Interrupted system call should be restarted
ESTRPIPE    86    Streams pipe error
EUSERS    87    Too many users
ENOTSOCK    88    Socket operation on non-socket
EDESTADDRREQ    89    Destination address required
EMSGSIZE    90    Message too long
EPROTOTYPE    91    Protocol wrong type for socket
ENOPROTOOPT    92    Protocol not available
EPROTONOSUPPORT    93    Protocol not supported
ESOCKTNOSUPPORT    94    Socket type not supported
EOPNOTSUPP    95    Operation not supported on transport
EPFNOSUPPORT    96    Protocol family not supported
EAFNOSUPPORT    97    Address family not supported by protocol
EADDRINUSE    98    Address already in use
EADDRNOTAVAIL    99    Cannot assign requested address
ENETDOWN    100    Network is down
ENETUNREACH    101    Network is unreachable
ENETRESET     102    Network dropped
ECONNABORTED    103    Software caused connection
ECONNRESET    104    Connection reset by
ENOBUFS    105    No buffer space available
EISCONN    106    Transport endpoint
ENOTCONN    107    Transport endpoint
ESHUTDOWN     108    Cannot send after transport
ETOOMANYREFS    109    Too many references
ETIMEDOUT     110    Connection timed
ECONNREFUSED    111    Connection refused
EHOSTDOWN     112    Host is down
EHOSTUNREACH    113    No route to host
EALREADY    114    Operation already
EINPROGRESS    115    Operation now in
ESTALE    116    Stale NFS file handle
EUCLEAN    117    Structure needs cleaning
ENOTNAM    118    Not a XENIX-named
ENAVAIL    119    No XENIX semaphores
EISNAM    120    Is a named type file
EREMOTEIO     121    Remote I/O error
EDQUOT    122    Quota exceeded
ENOMEDIUM     123    No medium found
EMEDIUMTYPE    124    Wrong medium type
```
