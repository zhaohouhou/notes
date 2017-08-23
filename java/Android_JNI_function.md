# Android NDK 编程

ref: https://developer.android.com/studio/projects/add-native-code.html

## 0. 准备Java接口代码、本地接口代码和实现代码(c/cpp)

参考： http://www.cnblogs.com/sevenyuan/p/4202759.html


## 1. 工具

使用Android Studio，并安装LLDB、CMake 和 NDK组件。

- 打开Android项目， 从菜单栏选择 Tools > Android > SDK Manager => SDK Tools

- 选中LLDB、CMake 和 NDK并安装。

## 2. 创建CMakeLists.txt

模块目录下创建CMAKE脚本，需要添加 cmake_minimum_required() 和 add_library() 命令。例：

```
# Sets the minimum version of CMake required to build your native library.
cmake_minimum_required(VERSION 3.4.1)

# You can define multiple libraries by adding multiple add.library()
# commands.
add_library( # Specifies the name of the library.
            native-lib

            # Sets the library as a shared library.
            SHARED

            # Provides a relative path to your source file(s).
            src/main/cpp/native-lib.cpp )
```

如果本地代码包括头文件，还要将 include_directories() 命令添加到 CMake 构建脚本中并指定标头的路径：

```
# Specifies a path to native header files.
include_directories(src/main/cpp/include/)
```

在 Java 代码中加载此库时，使用您在 CMake 构建脚本中指定的名称：

```java
static {
    System.loadLibrary("native-lib");
}
```

(CMake 创建的库文件名称为 libnative-lib.so)


## 3. 关联Gradle到C++工程

在模块上右键，选择Link C++ Project with Gradle，选择 CMake 或 ndk-build，并关联配置文件（CMakeLists.txt或Android.mk脚本）

若设置正确，则应能够正常build项目和运行代码。


## 4. JNI编程

### 1）传参

本地代码中java数据结构的参数转换为本地表示：如JNI 基本数据类型 jint , jlong , jchar，引用数据类型jstring，jobject ，jobjectArray，jintArray等等。
JNI接口函数的第一个参数JNIEnv是接口指针，包括了一些jvm运行java语言的底层代码。

当传入或传出参数是字符串等，可以使用jbyteArray, 特别是有关的汉字问题。下面的代码显示了jbyteArray和char*间的类型转换。

```C
  //convert jbyteArray to unsigned char*
  //s: jbyteArray
  //env: JNIEnv *
  jbyte *jbyte_ptr = (*env)->GetByteArrayElements(env, s, 0);
  unsigned char *r = (unsigned char *)jbyte_ptr;
  ...
```

```C
// convert char* to jbyteArray
jbyteArray convertCharArrtoJByteArr(JNIEnv *env, char * in, int size){
    jbyte byte_array[size];
    int i = 0;
    for(;i < size; i++) {
        byte_array[i] = in[i];
    }
    jbyteArray jbyte_arr = (*env)->NewByteArray(env, size);
    (*env)->SetByteArrayRegion(env, jbyte_arr, 0, size, byte_array);
    return jbyte_arr;
}
```

### 2） 避免内存泄漏

JNI代码遵循本地语言的规则，例如C语言中malloc的空间需要调用free来释放，而不是由jvm自动管理，因此JNI编程中需要注意内存管理、以避免内存泄漏。

因此，上面使用NewByteArray创建的资源需要在使用后在native代码中进行释放（回调native函数释放）。

```C
//release jstring，jobject ，jobjectArray，jintArray, etc.
(*env)->DeleteLocalRef(env, arr);
```
