import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Properties;

/**
 * 配置文件工具类
 */
public class ResourcesUtil
{
   /**
    * 读取配置文件中指定key对应的value
    * @param propertiesName
    * @param key
    * @return
    */
   public String read(String propertiesName, String key)
   {
      String value = null;
      InputStreamReader reader = new InputStreamReader(
         ResourcesUtil.class.getResourceAsStream(propertiesName));
      Properties ps = new Properties();
      try
      {
         ps.load(reader);
         value = ps.getProperty(key);
         reader.close();
      }
      catch (IOException e)
      {
         e.printStackTrace();
      }
      return value;
   }
}
