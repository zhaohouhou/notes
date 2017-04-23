
import net.sf.json.JSONArray;
import net.sf.json.JSONObject;
import org.apache.http.Header;
import org.apache.http.HttpResponse;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClientBuilder;

import java.io.*;
import java.net.InetSocketAddress;
import java.net.Socket;

public class SignatureScanner extends Scanner
{

  public int port = 8080;
  public int ok = 200;

  //send POST request
  public void send(){
    HttpPost request = new HttpPost("http://localhost:" + this.port);
    StringEntity params = new StringEntity("username=test&password=test");
    request.addHeader("content-type", "text/json");
    request.setEntity(params);
    CloseableHttpClient httpClient = HttpClientBuilder
                      .create().build();

    //parse result
    HttpResponse httpResponse = httpClient.execute(request);
    int httpCode = httpResponse.getStatusLine().getStatusCode();
    if (httpCode == this.ok)
    {
        Header resultHeader = httpResponse.getFirstHeader("result");
        String result = resultHeader.getValue();
        JSONObject jsonResult = JSONObject
                          .fromObject(result);

        addToReport(jsonResult);
        System.out.println("result: " + jsonResult);
        this.finished = true;
    } else
  }
    
}
