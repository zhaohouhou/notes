
# 用 junit 进行单元测试

## 傻瓜版

1） 首先添加项目依赖，例如在 gradle 中， build.gradle 中添加：

```
dependencies {
    testCompile group: 'junit', name: 'junit', version: '4.12'
}
```

2） code

```
import static org.junit.Assert.*;
import org.junit.Test;

class TestSampleClass{

@Test
void sampleTestFunction(){
  ...
  assertEquals(result, expect);
}

...
}

```
