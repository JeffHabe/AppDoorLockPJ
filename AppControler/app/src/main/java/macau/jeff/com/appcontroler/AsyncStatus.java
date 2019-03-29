package macau.jeff.com.appcontroler;

import android.app.ProgressDialog;
import android.net.Uri;
import android.os.AsyncTask;
import android.util.Log;
import android.widget.Toast;

import java.io.BufferedWriter;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLEncoder;

import static android.content.ContentValues.TAG;

/**
 * Created by User on 2017/01/09.
 */

public class AsyncStatus extends AsyncTask<String,String,String > {
    private MainActivity mainActivity;
    private HttpURLConnection conn;
    private URL url;
    public AsyncStatus(MainActivity mainActivity){
        this.mainActivity = mainActivity;
    }
    @Override
    protected String doInBackground(String... params) {
        String data = null;
        try {
            data = "status=" + URLEncoder.encode(params[0],"utf-8");

            url = new URL("http://140.120.101.95/wifi/control_switch.php?"+data);
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        } catch (MalformedURLException e) {
            e.printStackTrace();
        }
        try {
            conn = (HttpURLConnection) url.openConnection();

            conn.setConnectTimeout(this.mainActivity.CONNECTION_TIMEOUT);
            conn.setReadTimeout(this.mainActivity.READ_TIMEOUT);
            conn.setRequestMethod("GET");

            int responseCode = conn.getResponseCode();
            if (responseCode == 200) {

                InputStream is = conn.getInputStream();
                String state = getStringFromInputStream(is);
                return state;
            } else {
                Log.i(TAG, "訪問失敗" + responseCode);
            }
            conn.connect();

        } catch (IOException e) {
            e.printStackTrace();
        }
        try {
            int response_code = conn.getResponseCode();

            if(response_code == HttpURLConnection.HTTP_OK){
                return ("successful");
            }
            else{
                return ("unsuccessful");
            }
        } catch (IOException e) {
            e.printStackTrace();
            return ("exception");
        } finally {
            conn.disconnect();
        }
    }

    protected void onPostExecute(String s) {
        if(s.equalsIgnoreCase("successful")){
            Toast.makeText(this.mainActivity, "Change Status Success", Toast.LENGTH_LONG).show();
        } else if(s.equalsIgnoreCase("unsuccessful") || s.equalsIgnoreCase("exception")){
            Toast.makeText(this.mainActivity, "Change fail", Toast.LENGTH_LONG).show();
        }
    }
    private static String getStringFromInputStream(InputStream is)
            throws IOException {
        ByteArrayOutputStream os = new ByteArrayOutputStream();
        // 模組代碼 必須熟練
        byte[] buffer = new byte[1024];
        int len = -1;
        // 一定要寫len=is.read(buffer)
        // 如果while((is.read(buffer))!=-1)則無法將數刻寫入buffer中
        while ((len = is.read(buffer)) != -1) {
            os.write(buffer, 0, len);
        }
        is.close();
        String state = os.toString();//把流中數刻轉換成字串,採用編碼utf-8
        os.close();
        return state;
    }
}
