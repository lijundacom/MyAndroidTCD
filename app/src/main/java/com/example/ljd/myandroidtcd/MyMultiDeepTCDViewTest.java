package com.example.ljd.myandroidtcd;

import android.content.Context;
import android.util.AttributeSet;
import android.util.Log;

import static com.example.ljd.myandroidtcd.USImageJNI.GetDeepPoint;
import static com.example.ljd.myandroidtcd.USImageJNI.GetMultiDeepModeTCDARGBArray;

/**
 * Created by ljd-pc on 2016/5/10.
 */
public class MyMultiDeepTCDViewTest extends MyView {
    private static final int HEIGHT = GetDeepPoint();
    private static final int STRIDE = 300;
    private static final int WIDTH = STRIDE;
    public MyMultiDeepTCDViewTest(Context context) {
        super(context,HEIGHT,STRIDE,WIDTH);
    }

    public MyMultiDeepTCDViewTest(Context context, AttributeSet attrs) {
        super(context, attrs,HEIGHT,STRIDE,WIDTH);
    }

    public MyMultiDeepTCDViewTest(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr,HEIGHT,STRIDE,WIDTH);
    }

    @Override
    public int[] initColors(int myCount, int[] colors) {
        int[] colorOneDeep = null;
        //colorOneDeep = GetMultiDeepModeTCDARGBArray();

        for(int i=0;i<HEIGHT;i++){
            for(myCount = 0;myCount<50;myCount++){
               // colors[myCount*WIDTH+i]= colorOneDeep[i];
                colors[myCount*WIDTH+i]=0;
            }
        }
        //Log.i("MyMultiDeepTCDView","initColors colors :"+colors[myCount*WIDTH+5]);
        //Log.i("MyMultiDeepTCDView","initColors colorOneDeep :"+colorOneDeep[HEIGHT/2]);
        return colors;
    }
}
