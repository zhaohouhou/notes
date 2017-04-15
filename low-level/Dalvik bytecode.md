# Dalvik bytecode

## 1 基础设计

- 采用基于寄存器的机器模型。寄存器大小为32位；两个相邻寄存器可结为64位寄存器对。单个寄存器被认为足够存储一个Object的引用。
- 对于按位的的表示，`(Object) null == (int) 0`。
- Frames are fixed in size upon creation. Each frame consists of a particular number of registers (specified by the method) as well as any adjunct data needed to execute the method, such as the program counter and a reference to the .dex file that contains the method. The N arguments to a method land in the last N registers of the method's invocation frame, in order. Wide arguments consume two registers. Instance methods are passed a *this* reference as their first argument.
- **The instructions must be located on even-numbered bytecode offsets**. In order to meet this requirement, dex generation tools must emit an extra nop instruction as a spacer if such an instruction would otherwise be unaligned.
- When installed on a running system, some instructions may be altered, changing their format, as an install-time static linking optimization. This is to allow for faster execution once linkage is known.


**关于助记符**：

- 参数顺序：目的-源 （Dest-then-source）
- 一些指令带有解释性的后缀，用于表明操作数类型：
	- Type-general 32-bit opcodes are unmarked.
	- Type-general 64-bit opcodes are suffixed with -wide.
	- Type-specific opcodes are suffixed with their type, one of: `-boolean -byte -char -short -int -long -float -double -object -string -class -void`.
- 一些指令带有解释性的后缀，用于区分指令布局或操作之间的不同。这些后缀由一个斜杠("/")与主名称分隔开。
- 值的长度由字母标示出，每个字母代表4 bits。例如指令`move-wide/from16 vAA, vBBBB`:
	- `"move"` is the base opcode, indicating the base operation (move a register's value).
	- `"wide"` is the name suffix, indicating that it operates on wide (64 bit) data.
	- `"from16"` is the opcode suffix, indicating a variant that has a 16-bit register reference as a source.
	- `"vAA"` is the destination register (implied by the operation; again, the rule is that destination arguments always come first), which must be in the range v0 – v255.
	- `"vBBBB"` is the source register, which must be in the range v0 – v65535.


部分字节码集编码如下:

<table class="instruc">
<thead>
<tr>
  <th>Op &amp; Format</th>
  <th>Mnemonic / Syntax</th>
  <th>Arguments</th>
  <th>Description</th>
</tr>
</thead>
<tbody>
<tr>
  <td>00 10x</td>
  <td>nop</td>
  <td>&nbsp;</td>
  <td>Waste cycles.
    <p class="note"><strong>Note:</strong>
    Data-bearing pseudo-instructions are tagged with this opcode, in which
    case the high-order byte of the opcode unit indicates the nature of
    the data. See "<code>packed-switch-payload</code> Format",
    "<code>sparse-switch-payload</code> Format", and
    "<code>fill-array-data-payload</code> Format" below.</p>
  </td>
</tr>
<tr>
  <td>01 12x</td>
  <td>move vA, vB</td>
  <td><code>A:</code> destination register (4 bits)<br>
    <code>B:</code> source register (4 bits)</td>
  <td>Move the contents of one non-object register to another.</td>
</tr>
<tr>
  <td>02 22x</td>
  <td>move/from16 vAA, vBBBB</td>
  <td><code>A:</code> destination register (8 bits)<br>
    <code>B:</code> source register (16 bits)</td>
  <td>Move the contents of one non-object register to another.</td>
</tr>
<tr>
  <td>03 32x</td>
  <td>move/16 vAAAA, vBBBB</td>
  <td><code>A:</code> destination register (16 bits)<br>
    <code>B:</code> source register (16 bits)</td>
  <td>Move the contents of one non-object register to another.</td>
</tr>
<tr>
  <td>...</td>
  <td>..</td>
  <td></td>
  <td></td>
</tr>
</tbody>
</table>




<br/>


## 2 Instruction format

类型编码：

<table class="letters">
<thead>
<tr>
  <th>Mnemonic</th>
  <th>Bit Sizes</th>
  <th>Meaning</th>
</tr>
</thead>
<tbody>
<tr>
  <td>b</td>
  <td>8</td>
  <td>immediate signed <b>b</b>yte</td>
</tr>
<tr>
  <td>c</td>
  <td>16, 32</td>
  <td><b>c</b>onstant pool index</td>
</tr>
<tr>
  <td>f</td>
  <td>16</td>
  <td>inter<b>f</b>ace constants (only used in statically linked formats)
  </td>
</tr>
<tr>
  <td>h</td>
  <td>16</td>
  <td>immediate signed <b>h</b>at (high-order bits of a 32- or 64-bit
    value; low-order bits are all <code>0</code>)
  </td>
</tr>
<tr>
  <td>i</td>
  <td>32</td>
  <td>immediate signed <b>i</b>nt, or 32-bit float</td>
</tr>
<tr>
  <td>l</td>
  <td>64</td>
  <td>immediate signed <b>l</b>ong, or 64-bit double</td>
</tr>
<tr>
  <td>m</td>
  <td>16</td>
  <td><b>m</b>ethod constants (only used in statically linked formats)</td>
</tr>
<tr>
  <td>n</td>
  <td>4</td>
  <td>immediate signed <b>n</b>ibble</td>
</tr>
<tr>
  <td>s</td>
  <td>16</td>
  <td>immediate signed <b>s</b>hort</td>
</tr>
<tr>
  <td>t</td>
  <td>8, 16, 32</td>
  <td>branch <b>t</b>arget</td>
</tr>
<tr>
  <td>x</td>
  <td>0</td>
  <td>no additional data</td>
</tr>
</tbody>
</table>

**指令格式 Format ID**: Most format IDs consist of three characters, two digits followed by a letter. The first digit indicates the number of 16-bit code units in the format. The second digit indicates the maximum number of registers that the format contains. 具体如下表:

<table class="format">
<thead>
<tr>
  <th>Format</th>
  <th>ID</th>
  <th>Syntax</th>
  <th>Notable Opcodes Covered</th>
</tr>
</thead>
<tbody>
<tr>
  <td><i>N/A</i></td>
  <td>00x</td>
  <td><i><code>N/A</code></i></td>
  <td><i>pseudo-format used for unused opcodes; suggested for use as the
    nominal format for a breakpoint opcode</i></td>
</tr>
<tr>
  <td>ØØ|<i>op</i></td>
  <td>10x</td>
  <td><i><code>op</code></i></td>
  <td>&nbsp;</td>
</tr>
<tr>
  <td rowspan="2">B|A|<i>op</i></td>
  <td>12x</td>
  <td><i><code>op</code></i> vA, vB</td>
  <td>&nbsp;</td>
</tr>
<tr>
  <td>11n</td>
  <td><i><code>op</code></i> vA, #+B</td>
  <td>&nbsp;</td>
</tr>
<tr>
  <td rowspan="2">AA|<i>op</i></td>
  <td>11x</td>
  <td><i><code>op</code></i> vAA</td>
  <td>&nbsp;</td>
</tr>
<tr>
  <td>10t</td>
  <td><i><code>op</code></i> +AA</td>
  <td>goto</td>
</tr>
<tr>
  <td>ØØ|<i>op</i> AAAA</td>
  <td>20t</td>
  <td><i><code>op</code></i> +AAAA</td>
  <td>goto/16</td>
</tr>
<tr>
  <td>AA|<i>op</i> BBBB</td>
  <td>20bc</td>
  <td><i><code>op</code></i> AA, kind@BBBB</td>
  <td><i>suggested format for statically determined verification errors;
    A is the type of error and B is an index into a type-appropriate
    table (e.g. method references for a no-such-method error)</i></td>
</tr>
<tr>
  <td rowspan="5">AA|<i>op</i> BBBB</td>
  <td>22x</td>
  <td><i><code>op</code></i> vAA, vBBBB</td>
  <td>&nbsp;</td>
</tr>
<tr>
  <td>21t</td>
  <td><i><code>op</code></i> vAA, +BBBB</td>
  <td>&nbsp;</td>
</tr>
<tr>
  <td>21s</td>
  <td><i><code>op</code></i> vAA, #+BBBB</td>
  <td>&nbsp;</td>
</tr>
<tr>
  <td>21h</td>
  <td><i><code>op</code></i> vAA, #+BBBB0000<br>
    <i><code>op</code></i> vAA, #+BBBB000000000000
  </td>
  <td>&nbsp;</td>
</tr>
<tr>
  <td>21c</td>
  <td><i><code>op</code></i> vAA, type@BBBB<br>
    <i><code>op</code></i> vAA, field@BBBB<br>
    <i><code>op</code></i> vAA, string@BBBB
  </td>
  <td>check-cast<br>
    const-class<br>
    const-string
  </td>
</tr>
<tr>
  <td rowspan="2">AA|<i>op</i> CC|BB</td>
  <td>23x</td>
  <td><i><code>op</code></i> vAA, vBB, vCC</td>
  <td>&nbsp;</td>
</tr>
<tr>
  <td>22b</td>
  <td><i><code>op</code></i> vAA, vBB, #+CC</td>
  <td>&nbsp;</td>
</tr>
<tr>
  <td rowspan="4">B|A|<i>op</i> CCCC</td>
  <td>22t</td>
  <td><i><code>op</code></i> vA, vB, +CCCC</td>
  <td>&nbsp;</td>
</tr>
<tr>
  <td>22s</td>
  <td><i><code>op</code></i> vA, vB, #+CCCC</td>
  <td>&nbsp;</td>
</tr>
<tr>
  <td>22c</td>
  <td><i><code>op</code></i> vA, vB, type@CCCC<br>
    <i><code>op</code></i> vA, vB, field@CCCC
  </td>
  <td>instance-of</td>
</tr>
<tr>
  <td>22cs</td>
  <td><i><code>op</code></i> vA, vB, fieldoff@CCCC</td>
  <td><i>suggested format for statically linked field access instructions of
    format 22c</i>
  </td>
</tr>
<tr>
  <td>ØØ|<i>op</i> AAAA<sub>lo</sub> AAAA<sub>hi</sub></td>
  <td>30t</td>
  <td><i><code>op</code></i> +AAAAAAAA</td>
  <td>goto/32</td>
</tr>
<tr>
  <td>ØØ|<i>op</i> AAAA BBBB</td>
  <td>32x</td>
  <td><i><code>op</code></i> vAAAA, vBBBB</td>
  <td>&nbsp;</td>
</tr>
<tr>
  <td rowspan="3">AA|<i>op</i> BBBB<sub>lo</sub> BBBB<sub>hi</sub></td>
  <td>31i</td>
  <td><i><code>op</code></i> vAA, #+BBBBBBBB</td>
  <td>&nbsp;</td>
</tr>
<tr>
  <td>31t</td>
  <td><i><code>op</code></i> vAA, +BBBBBBBB</td>
  <td>&nbsp;</td>
</tr>
<tr>
  <td>31c</td>
  <td><i><code>op</code></i> vAA, string@BBBBBBBB</td>
  <td>const-string/jumbo</td>
</tr>
<tr>
  <td rowspan="3">A|G|<i>op</i> BBBB F|E|D|C</td>
  <td>35c</td>
  <td><i>[<code>A=5</code>] <code>op</code></i> {vC, vD, vE, vF, vG},
    meth@BBBB<br>
    <i>[<code>A=5</code>] <code>op</code></i> {vC, vD, vE, vF, vG},
    site@BBBB<br>
    <i>[<code>A=5</code>] <code>op</code></i> {vC, vD, vE, vF, vG},
    type@BBBB<br>
    <i>[<code>A=4</code>] <code>op</code></i> {vC, vD, vE, vF},
    <i><code>kind</code></i>@BBBB<br>
    <i>[<code>A=3</code>] <code>op</code></i> {vC, vD, vE},
    <i><code>kind</code></i>@BBBB<br>
    <i>[<code>A=2</code>] <code>op</code></i> {vC, vD},
    <i><code>kind</code></i>@BBBB<br>
    <i>[<code>A=1</code>] <code>op</code></i> {vC},
    <i><code>kind</code></i>@BBBB<br>
    <i>[<code>A=0</code>] <code>op</code></i> {},
    <i><code>kind</code></i>@BBBB<br>
    <p><i>The unusual choice in lettering here reflects a desire to make
    the count and the reference index have the same label as in format
    3rc.</i></p>
  </td>
  <td>&nbsp;</td>
</tr>
<tr>
  <td>35ms</td>
  <td><i>[<code>A=5</code>] <code>op</code></i> {vC, vD, vE, vF, vG},
    vtaboff@BBBB<br>
    <i>[<code>A=4</code>] <code>op</code></i> {vC, vD, vE, vF},
    vtaboff@BBBB<br>
    <i>[<code>A=3</code>] <code>op</code></i> {vC, vD, vE},
    vtaboff@BBBB<br>
    <i>[<code>A=2</code>] <code>op</code></i> {vC, vD},
    vtaboff@BBBB<br>
    <i>[<code>A=1</code>] <code>op</code></i> {vC},
    vtaboff@BBBB<br>
    <p><i>The unusual choice in lettering here reflects a desire to make
    the count and the reference index have the same label as in format
    3rms.</i></p>
  </td>
  <td><i>suggested format for statically linked <code>invoke-virtual</code>
    and <code>invoke-super</code> instructions of format 35c</i>
  </td>
</tr>
<tr>
  <td>35mi</td>
  <td><i>[<code>A=5</code>] <code>op</code></i> {vC, vD, vE, vF, vG},
    inline@BBBB<br>
    <i>[<code>A=4</code>] <code>op</code></i> {vC, vD, vE, vF},
    inline@BBBB<br>
    <i>[<code>A=3</code>] <code>op</code></i> {vC, vD, vE},
    inline@BBBB<br>
    <i>[<code>A=2</code>] <code>op</code></i> {vC, vD},
    inline@BBBB<br>
    <i>[<code>A=1</code>] <code>op</code></i> {vC},
    inline@BBBB<br>
    <p><i>The unusual choice in lettering here reflects a desire to make
    the count and the reference index have the same label as in format
    3rmi.</i></p>
  </td>
  <td><i>suggested format for inline linked <code>invoke-static</code>
    and <code>invoke-virtual</code> instructions of format 35c</i>
  </td>
</tr>
<tr>
  <td rowspan="3">AA|<i>op</i> BBBB CCCC</td>
  <td>3rc</td>
  <td><i><code>op</code></i> {vCCCC .. vNNNN}, meth@BBBB<br>
    <i><code>op</code></i> {vCCCC .. vNNNN}, site@BBBB<br>
    <i><code>op</code></i> {vCCCC .. vNNNN}, type@BBBB<br>
    <p><i>where <code>NNNN = CCCC+AA-1</code>, that is <code>A</code>
    determines the count <code>0..255</code>, and <code>C</code>
    determines the first register</i></p>
  </td>
  <td>&nbsp;</td>
</tr>
<tr>
  <td>3rms</td>
  <td><i><code>op</code></i> {vCCCC .. vNNNN}, vtaboff@BBBB<br>
    <p><i>where <code>NNNN = CCCC+AA-1</code>, that is <code>A</code>
    determines the count <code>0..255</code>, and <code>C</code>
    determines the first register</i></p>
  </td>
  <td><i>suggested format for statically linked <code>invoke-virtual</code>
    and <code>invoke-super</code> instructions of format <code>3rc</code></i>
  </td>
</tr>
<tr>
  <td>3rmi</td>
  <td><i><code>op</code></i> {vCCCC .. vNNNN}, inline@BBBB<br>
    <p><i>where <code>NNNN = CCCC+AA-1</code>, that is <code>A</code>
    determines the count <code>0..255</code>, and <code>C</code>
    determines the first register</i></p>
  </td>
  <td><i>suggested format for inline linked <code>invoke-static</code>
    and <code>invoke-virtual</code> instructions of format 3rc</i>
  </td>
</tr>
<tr>
  <td>A|G|<i>op</i> BBBB F|E|D|C HHHH
  </td><td>45cc</td>
  <td>
    <i>[<code>A=5</code>] <code>op</code></i> {vC, vD, vE, vF, vG}, meth@BBBB, proto@HHHH<br>
    <i>[<code>A=4</code>] <code>op</code></i> {vC, vD, vE, vF}, meth@BBBB, proto@HHHH<br>
    <i>[<code>A=3</code>] <code>op</code></i> {vC, vD, vE}, meth@BBBB, proto@HHHH<br>
    <i>[<code>A=2</code>] <code>op</code></i> {vC, vD}, meth@BBBB, proto@HHHH<br>
    <i>[<code>A=1</code>] <code>op</code></i> {vC}, meth@BBBB, proto@HHHH
  </td>
  <td>invoke-polymorphic
  </td>
</tr>
<tr>
  <td>AA|<i>op</i> BBBB CCCC HHHH
  </td><td>4rcc</td>
  <td>
    <code>op&gt;</code> {vCCCC .. vNNNN}, meth@BBBB, proto@HHHH
    <p><i>wheere <code>NNNN = CCCC+AA-1</code>, that is <code>A</code>
    determines the count <code>0..255</code>, and <code>C</code>
    determines the first register</i></p>
  </td>
  <td>invoke-polymorphic/range
  </td>
</tr>
<tr>
  <td>AA|<i>op</i> BBBB<sub>lo</sub> BBBB BBBB BBBB<sub>hi</sub></td>
  <td>51l</td>
  <td><i><code>op</code></i> vAA, #+BBBBBBBBBBBBBBBB</td>
  <td>const-wide</td>
</tr>
</tbody>
</table>


## 3 IDA中查看

用IDA打开一个apk的classes.dex文件，可以看到反编译出的代码。例如某个函数PhoneInfo_Init:

```
CODE:00052A14 public void com.zhone.PhoneInfo.<init>(
CODE:00052A14       android.content.Context ArrayVectorUtils.java)
CODE:00052A14 this = v3                               # CODE XREF: MainTask_getStr@LL+6j
CODE:00052A14 ArrayVectorUtils.java = v4
CODE:00052A14                 invoke-direct                   {this}, <void Object.<init>() imp. @ _def_Object__init_@V>
CODE:00052A1A                 new-instance                    v0, <t: UserInfo>
CODE:00052A1E                 invoke-direct                   {v0}, <void UserInfo.<init>() UserInfo__init_@V>
CODE:00052A24                 iput-object                     v0, this, PhoneInfo_userInfo
CODE:00052A28                 iput-object                     ArrayVectorUtils.java, this, PhoneInfo_context
CODE:00052A2C                 new-instance                    v0, <t: BuildInfo>
CODE:00052A30                 iget-object                     v1, this, PhoneInfo_userInfo
CODE:00052A34                 invoke-direct                   {v0, v1}, <void BuildInfo.<init>(ref) BuildInfo__init_@VL>
CODE:00052A3A                 invoke-virtual                  {v0}, <ref BuildInfo.getFromPhone() BuildInfo_getFromPhone@L>
...
CODE:00052B1E                 iput-object                     v1, v0, UserInfo_ua
CODE:00052B22                 invoke-virtual                  {this}, <void PhoneInfo.getProp() PhoneInfo_getProp@V>
CODE:00052B28
CODE:00052B28 locret:
CODE:00052B28                 return-void
CODE:00052B28 Method End
CODE:00052B28 # ---------------------------------------------------------------------------
```

在HexView中可以看到对应的字节码：

```
00052A14  70 10 49 11 03 00 22 00  59 02 70 10 6F 10 00 00  p.I...".Y.p.o...
00052A24  5B 30 11 07 5B 34 10 07  22 00 4D 02 54 31 11 07  [0..[4..".M.T1..
00052A34  70 20 51 10 10 00 6E 10  52 10 00 00 22 00 50 02  p Q...n.R...".P.
00052A44  70 10 55 10 00 00 54 31  11 07 6E 30 57 10 40 01  p.U...T1..n0W.@.
00052A54  22 00 5C 02 54 31 11 07  70 20 77 10 10 00 6E 20  ".\.T1..p w...n
00052A64  78 10 40 00 22 00 5B 02  54 31 11 07 70 20 74 10  x.@.".[.T1..p t.
00052A74  10 00 6E 20 76 10 40 00  54 30 10 07 70 20 63 10  ..n v.@.T0..p c.
00052A84  03 00 54 30 11 07 22 01  B2 02 6E 10 62 10 03 00  ..T0.."...n.b...
00052A94  0C 02 71 10 74 11 02 00  0C 02 70 20 7C 11 21 00  ..q.t.....p |.!.
...
```

查表可得0x00052A14处指令码0x70为：

| Op & Format | Mnemonic / Syntax | Arguments | Description |
|--|--|--|--|
|||||
| 6e..72 35c |invoke-kind {vC, vD, vE, vF, vG}, meth@BBBB　<br/>6e: invoke-virtual<br/>6f: invoke-super<br/>70: invoke-direct<br/>71: invoke-static<br/>72: invoke-interface | A: argument word count (4 bits)<br/>B: method reference index (16 bits)<br/>C..G: argument registers (4 bits each) | Call the indicated method.

则该指令为`invoke-direct`，操作数为4+16+4*5 = 40 bits，加上操作码总指令长度为48 bits = 6字节。因此下一条指令为0x00052A1A: 22 00 ...，为`new-instance`指令。

Format 35c的格式为`A|G|op BBBB F|E|D|C`，注意为了使35c与3rc形式一致，参数没有按照Syntax顺序排列。因此指令编码`70 10 49 11 03 00`中，参数word count = 1， 唯一的参数为v3，method reference index为`49 11`。

### 另一个例子：

ida中看到指令：`invoke-direct/range             {v0..v7}, <xxxx>`

对应的十六进制字节码为:`76 08 B6 01 00 00`

**分析**：

`76`指令编码对应`invoke-direct/range`;其格式（Format）为

| Op & Format | Mnemonic / Syntax | Arguments | Description |
|--|--|--|--|
|||||
| 76 3rc | invoke-direct/range | A: argument word count (8 bits)<br/>B: method reference index (16 bits)<br/>C: first argument register (16 bits)<br/>N = A + C - 1 | Call the indicated method.

则指令长度为：8(opcode)+8(A)+16(B)+16( C) = 48 bits = 6 bytes.

具体的指令编码中，argument word count为`08`， method reference index = `B6 01`， first argument register = `00 00`。则最后一个使用的寄存器序号为：8+0-1 = 7，即参数为`{v0..v7}`。

<br/>

## 4 payload format

存放数据的伪指令以`0x00`开头，与nop的编码相同，随后的两位指明具体的数据类型。

**例如**：`fill-array-data`指令从指明的数据表位置将数据装入数组，所需要的数据表类型为"fill-array-data-payload Format"。地址`0x00155EBA`除指令编码` 26 00 71 01 00 00`（IDA中显示：`CODE:  fill-array-data  v0, arraydata_15619C`）的offset为`71 01 00 00`即0x171，则找到的数据表位置为:0x00155EBA + 0x717*2 = 0x0015619C。

0x0015619C处编码：`00 03 04 00 1B 00 00 00 ....`，其中`00 03`表示数据类型为“fill-array-data-payload”，`00 04`表示每个元素大小为4 bytes，`1B 00 00 00`表示元素个数为0x1B = 27个。


#### packed-switch-payload format

<table class="supplement">
<thead>
<tr>
  <th>Name</th>
  <th>Format</th>
  <th>Description</th>
</tr>
</thead>
<tbody>
<tr>
  <td>ident</td>
  <td>ushort = 0x0100</td>
  <td>identifying pseudo-opcode</td>
</tr>
<tr>
  <td>size</td>
  <td>ushort</td>
  <td>number of entries in the table</td>
</tr>
<tr>
  <td>first_key</td>
  <td>int</td>
  <td>first (and lowest) switch case value</td>
</tr>
<tr>
  <td>targets</td>
  <td>int[]</td>
  <td>list of <code>size</code> relative branch targets. The targets are
    relative to the address of the switch opcode, not of this table.
  </td>
</tr>
</tbody>
</table>

> Note: The total number of code units for an instance of this table is (size * 2) + 4.

#### sparse-switch-payload format

<table class="supplement">
<thead>
<tr>
  <th>Name</th>
  <th>Format</th>
  <th>Description</th>
</tr>
</thead>
<tbody>
<tr>
  <td>ident</td>
  <td>ushort = 0x0200</td>
  <td>identifying pseudo-opcode</td>
</tr>
<tr>
  <td>size</td>
  <td>ushort</td>
  <td>number of entries in the table</td>
</tr>
<tr>
  <td>keys</td>
  <td>int[]</td>
  <td>list of <code>size</code> key values, sorted low-to-high</td>
</tr>
<tr>
  <td>targets</td>
  <td>int[]</td>
  <td>list of <code>size</code> relative branch targets, each corresponding
    to the key value at the same index. The targets are
    relative to the address of the switch opcode, not of this table.
  </td>
</tr>
</tbody>
</table>

> The total number of code units for an instance of this table is (size * 4) + 2.


#### fill-array-data-payload format

<table class="supplement">
<thead>
<tr>
  <th>Name</th>
  <th>Format</th>
  <th>Description</th>
</tr>
</thead>
<tbody>
<tr>
  <td>ident</td>
  <td>ushort = 0x0300</td>
  <td>identifying pseudo-opcode</td>
</tr>
<tr>
  <td>element_width</td>
  <td>ushort</td>
  <td>number of bytes in each element</td>
</tr>
<tr>
  <td>size</td>
  <td>uint</td>
  <td>number of elements in the table</td>
</tr>
<tr>
  <td>data</td>
  <td>ubyte[]</td>
  <td>data values</td>
</tr>
</tbody>
</table>

> Note: The total number of code units for an instance of this table is (size * element_width + 1) / 2 + 4.



------------

ref:

https://source.android.com/devices/tech/dalvik/dalvik-bytecode.html

https://source.android.com/devices/tech/dalvik/instruction-formats.html

https://github.com/JesusFreke/smali
