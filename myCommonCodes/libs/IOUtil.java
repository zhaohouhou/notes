import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

public class IOUtil
{
public void getExecOutput(String cmd)
{
        try
        {
                Process p = Runtime.getRuntime().exec(cmd);
                InputStream fis = p.getInputStream();
                InputStreamReader isr=new InputStreamReader(fis);
                BufferedReader br=new BufferedReader(isr);
                String line=null;
                while((line=br.readLine())!=null)
                {
                        System.out.println(line);
                }
        }
        catch (IOException e)
        {
                e.printStackTrace();
        }
}
}
