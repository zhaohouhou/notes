import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.Socket;

import net.sf.json.JSONObject;
import org.apache.http.Header;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClientBuilder;

import static java.net.HttpURLConnection.HTTP_OK;

/**
 * Created by zd on 4/20/17.
 */
public class HttpUtil
{
    private CloseableHttpClient httpClient;
    public final String requestUrl;

    public HttpUtil(String requestUrl)
    {
        httpClient = HttpClientBuilder
                          .create().build();
        this.requestUrl = requestUrl;
    }

    public static boolean testServerConnect(String ip, int port)
                      throws IOException
    {
        boolean result = false;
        Socket connect = new Socket();
        try
        {
            connect.connect(new InetSocketAddress(ip, port),
                              1000); //retry per second
            result = connect.isConnected();
            connect.close();
            return result;

        } catch (IOException e)
        {
            connect.close();
            return result;
        }
    }

    /**
     * Send a POST request and get the response data.
     *
     * @param params      - POST request parameter string.
     *                    e.g. param1=aaa&&param2=bbb
     * @param contentType - request content type.
     *                    e.g. "application/json", etc.
     * @param resultField - field name to be fetched from POST response.
     */
    public synchronized JSONObject postRequest(String params,
                                  String contentType,
                                  String resultField)
                      throws Exception
    {
        HttpPost request = new HttpPost(requestUrl);
        StringEntity paramEntity = new StringEntity(params);
        request.addHeader("content-type", contentType);
        request.setEntity(paramEntity);

        //parse result
        CloseableHttpResponse httpResponse = httpClient.execute(request);
        try
        {
            int httpCode = httpResponse.getStatusLine().getStatusCode();
            if (httpCode == HTTP_OK)
            {
                Header resultHeader = httpResponse.getFirstHeader(resultField);
                String result = resultHeader.getValue();
                return JSONObject.fromObject(result);
            } else
            {
                throw new Exception("POST not OK: " + httpCode + "\n");
            }
        } finally
        {
            httpResponse.close();
            //MUST be closed!! (to enable following requests)
        }

    }
}
