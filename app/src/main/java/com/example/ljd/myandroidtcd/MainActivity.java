package com.example.ljd.myandroidtcd;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import static com.example.ljd.myandroidtcd.USImageJNI.InitALLThreads;
//import static com.example.ljd.myandroidtcd.USImageJNI.TestString;

public class MainActivity extends Activity {
    static final String TAG = "Handler";
    TextView tv;
    Button bt;
    Handler handler = new Handler(){
        public void handleMessage(Message msg){
            switch (msg.what)
            {
                case UPDATE:
                    Log.d(TAG, "The handler thread id = " + Thread.currentThread().getId() + "\n");
                    //tv.setText(InitALLThreads());
                    tv.setText("ljd");
                    break;
            }
        }
    };
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        tv = (TextView)findViewById(R.id.TextView1);
        bt = (Button)findViewById(R.id.bt);
        bt.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v){
                new myThread().start();
            }
        });
    }
    static final int UPDATE = 1;
    class myThread extends Thread{
        public void run(){
            Message msg = new Message();
            msg.what = UPDATE;
            handler.sendMessage(msg);
            InitALLThreads();
            Log.d(TAG, "The worker thread id = " + Thread.currentThread().getId() + "\n");

        }
    }
}

