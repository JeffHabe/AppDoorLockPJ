package macau.jeff.com.appcontroler;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {
    Button btn;
    public static final int READ_TIMEOUT = 15000;
    public static final int CONNECTION_TIMEOUT = 10000;
    private boolean isLight=false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        findview();

    }
    public void findview()
    {
        btn = (Button)findViewById(R.id.button);
    }
    public void onClick(View view) {
        if (!isLight) {
            isLight=true;
            Log.e("isLight;",Boolean.toString(isLight));
            new AsyncStatus(this).execute("1");
        }else{
            isLight=false;
            Log.e("isLight;",Boolean.toString(isLight));
            new AsyncStatus(this).execute("0");
        }
    }
}
