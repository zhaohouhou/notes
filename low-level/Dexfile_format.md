# DEX文件格式

## 1. dex文件结构

dex header结构定义：

```c
struct DexHeader {
    u1  magic[8];           /* includes version number */
    u4  checksum;           /* adler32 checksum */
    u1  signature[kSHA1DigestLen]; /* SHA-1 hash (len=20) */
    u4  fileSize;           /* length of entire file */
    u4  headerSize;         /* offset to start of next section */
    u4  endianTag;
    u4  linkSize;
    u4  linkOff;
    u4  mapOff;
    u4  stringIdsSize;
    u4  stringIdsOff;
    u4  typeIdsSize;
    u4  typeIdsOff;
    u4  protoIdsSize;
    u4  protoIdsOff;
    u4  fieldIdsSize;
    u4  fieldIdsOff;
    u4  methodIdsSize;
    u4  methodIdsOff;
    u4  classDefsSize;
    u4  classDefsOff;
    u4  dataSize;
    u4  dataOff;
};
```

各Ids作用：

<table>
<thead>
<tr>
  <th>Ids类型</th>
  <th align="center">作用</th>
  <th align="right">对应数据结构</th>
  <th>说明</th>
</tr>
</thead>
<tbody><tr>
  <td>stringsIds</td>
  <td align="center">保存所有用到的字符串的索引</td>
  <td align="right">DexStringId</td>
  <td>DexStringId包含 u4 stringDataOff; 该成员指出字符串在常量池中偏移</td>
</tr>
<tr>
  <td>typeIds</td>
  <td align="center">保存class, 基础类型的表</td>
  <td align="right">DexTypeId</td>
  <td>DexTypeId 只包含u4  descriptorIdx; 这是一个DexStringId的索引</td>
</tr>
<tr>
  <td>protoIds</td>
  <td align="center">保存一个method参数及返回值类型的数据的索引</td>
  <td align="right">DexProtoId</td>
  <td>DexProtoId包含3个成员：shortyIdx: 返回值参数的短格式, 是一个stringsId值; returnTypeIdx：返回值类型，是一个typeId值; parametersOff: 相对baseAddr的DexTypeList对象的偏移，DexTypeList是描述每个参数类型的列表对象</td>
</tr>
<tr>
  <td>fieldId</td>
  <td align="center">描述field信息的索引</td>
  <td align="right">DexFieldId</td>
  <td>DexFieldId包含3个成员：classIdx: field所属类的typeId值；typeIdx：field类型的typeId值；nameIdx: 名字的stringId值</td>
</tr>
<tr>
  <td>methodId</td>
  <td align="center">描述一个方法的信息索引</td>
  <td align="right">DexMethodId</td>
  <td>DexMethodId包含3个成员：classIdx: method所属类的typeId值; protoIdx；method的参数及返回值索引，protoId索引； nameIdx: 名字的stringId值</td>
</tr>
<tr>
  <td>classDefs</td>
  <td align="center">一个类的定义信息</td>
  <td align="right">DexClassDef</td>
  <td>定义了一个类</td>
</tr>
</tbody></table>

从上面可以看到：

  - field_ids_size: 体现了所有类的field的总数
  - method_ids_size:体现了所有类的方法的总数
  - class_defs_size: 类的总数

## 2. dexdump工具

dexdump 是安卓提供的一个dex文件查看工具。可以输入压缩文件（apk/zip/jar）或直接输入dex文件，查看dex信息。

在adb shell中使用（将apk文件上传到虚拟机/安卓设备中）：

    adb shell dexdump -f /data/app/com.tencent.mobileqq-1.apk

PC端安装Android sdk之后可以在 build-tools 中找到dexdemp可执行文件。

    $ .../android-sdk-linux/build-tools/23.0.1/dexdump -f classes.dex > 1.txt

下边是dexdump输出dex信息的示例，包含了dex header信息：

```
Processing 'classes.dex'...
Opened 'classes.dex', DEX version '035'
DEX file header:
magic               : 'dex\n035\0'
checksum            : 852891c2
signature           : a023...3474
file_size           : 455516
header_size         : 112
link_size           : 0
link_off            : 0 (0x000000)
string_ids_size     : 4072
string_ids_off      : 112 (0x000070)
type_ids_size       : 727
type_ids_off        : 16400 (0x004010)
proto_ids_size       : 882
proto_ids_off        : 19308 (0x004b6c)
field_ids_size      : 1333
field_ids_off       : 29892 (0x0074c4)
method_ids_size     : 3854
method_ids_off      : 40556 (0x009e6c)
class_defs_size     : 437
class_defs_off      : 71388 (0x0116dc)
data_size           : 370144
data_off            : 85372 (0x014d7c)

Class #0            -
  Class descriptor  : 'Lbeckham/a;'
  ...
```

---

### ref:

http://blog.csdn.net/doon/article/details/51691627

</br></br>
