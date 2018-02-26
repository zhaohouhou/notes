# MACH-O可执行文件

MACH-O(Mach object file)是一种可执行文件格式，运行在Mach平台上。NeXTSTEP, OS X, and iOS are examples of systems that have used this format for native executables, libraries and object code.

## 基本结构

典型的Mach-O文件包含三个部分：

| **Header** |
| ------------- |
| **Load Commands** |
| **Data** |

- **Header**：基本信息，包括魔数、cpu类型、load command数量和大小等。

  魔数含有该可执行文件的类型信息。Fat文件魔数为cafebabe（32位）和cafebabf（64位）；Mach-O文件魔数为feedface（32位）和feedfacf（64位）。需要注意数据的大小端。

- **Load Commands**：紧接header区域，每一项包含command的类型、segment名称、位置等。

  使用`otool -lv`命令可以查看文件头和load commands信息。

- **Data**：包含代码和数据。

OS X上除了 Mach-O 文件还有一种目标文件：Universal Binaries，又称Fat文件。Mach-O 文件只包含一种体系结构的代码，而Fat文件包含多个体系结构的代码，例如i386、 x86_64、arm、arm64。

Fat文件的基本结构是一个fat header后接Mach-O文件。

| **Fat Header** |
| ------------- |
| **Mach-O Header** |
| **Load Commands** |
| **Data** |
| **Mach-O Header** |
| **Load Commands** |
| **Data** |

## ref

https://lowlevelbits.org/parsing-mach-o-files/

http://www.blogfshare.com/ioss-mach-o.html

</br></br>
