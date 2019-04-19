## Java 反射的使用

### 通过反射 get/set Java 类的 private field 

```java
// Class declaration
class MyClass {
    private String instanceField = "A";
    
    public String getInstanceField() {
        return instanceField;
    }
}

// Get field instance
Field field = MyClass.class.getDeclaredField("instanceField");
field.setAccessible(true); // 更改Field的access
 
// Get field value of a class's instance 
MyClass object = new MyClass();
String fieldValue = (String) field.get();
System.out.println(fieldValue); // -> A
 
// Set value
field.set(object, "B");
System.out.println(object.()); // -> B
```

</br></br>
