package com.example.ljd.myandroidtcd;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

/**
 * Created by ljd-pc on 2016/5/6.
 */
class MyView extends SurfaceView implements SurfaceHolder.Callback
{
    //private Paint paint;
    public int WIDTH=200;
    public int HEIGHT=200;//单位为像素不是dp，与布局文件不同
    public int STRIDE=WIDTH;//must be >=WIDTH
    private SurfaceHolder holder;
    private MyThread myThread;
    int[] s = null;

    public MyView(Context context) {
        super(context);
        // TODO Auto-generated constructor stub
        holder = this.getHolder();
        holder.addCallback(this);
        myThread = new MyThread(holder);//创建一个绘图线程
    }

    public MyView(Context context, AttributeSet attrs) {
        super(context, attrs);
        holder = this.getHolder();
        holder.addCallback(this);
        myThread = new MyThread(holder);//创建一个绘图线程
    }

    public MyView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        holder = this.getHolder();
        holder.addCallback(this);
        myThread = new MyThread(holder);//创建一个绘图线程
    }

    public MyView(Context context, int HEIGHT, int STRIDE, int WIDTH) {
        super(context);
        this.HEIGHT = HEIGHT;
        this.STRIDE = STRIDE;
        this.WIDTH = WIDTH;
        holder = this.getHolder();
        holder.addCallback(this);
        myThread = new MyThread(holder);//创建一个绘图线程
    }

    public MyView(Context context, AttributeSet attrs, int HEIGHT, int STRIDE, int WIDTH) {
        super(context, attrs);
        this.HEIGHT = HEIGHT;
        this.STRIDE = STRIDE;
        this.WIDTH = WIDTH;
        holder = this.getHolder();
        holder.addCallback(this);
        myThread = new MyThread(holder);//创建一个绘图线程
    }

    public MyView(Context context, AttributeSet attrs, int defStyleAttr, int HEIGHT, int STRIDE, int WIDTH) {
        super(context, attrs, defStyleAttr);
        this.HEIGHT = HEIGHT;
        this.STRIDE = STRIDE;
        this.WIDTH = WIDTH;
        holder = this.getHolder();
        holder.addCallback(this);
        myThread = new MyThread(holder);//创建一个绘图线程
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width,
                               int height) {
        // TODO Auto-generated method stub
        Log.i("SurfaceView","surfaceChanged");

    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        // TODO Auto-generated method stub
        Log.i("SurfaceView","surfaceCreated");
        myThread.isRun = true;
        myThread.start();
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        // TODO Auto-generated method stub
        Log.i("SurfaceView","surfaceDestroyed");
        myThread.isRun = false;
    }
    public int[] initColors(int myCount,int[] colors) {
        //int[] colors=new int[STRIDE*HEIGHT];
        int k=0;
        for (int y = 0; y < HEIGHT; y++) {//use of x,y is legible then the use of i,j
            for (int x = 0; x < WIDTH; x++) {
                if(x>=myCount){
                    k=x-myCount;
                }else{
                    k=myCount-x;
                }
                int r = y * 255 / (HEIGHT - 1)-k;
                int g=r;
                int b=r;
                int a=255;

                colors[y*WIDTH+x]=(a<<24)|(r<<16)|(g<<8)|(b);//the shift operation generates the color ARGB
                //Log.i("ARGB",""+colors[y*WIDTH+x]);
            }
        }
        return colors;
    }
    class MyThread extends Thread
    {
        private SurfaceHolder holder;
        public boolean isRun ;
        public  MyThread(SurfaceHolder holder)
        {
            this.holder =holder;
            isRun = true;
        }
        @Override
        public void run()
        {
            int count = 0;
            int  myCount=0;
            int[] mColors= new int[WIDTH*HEIGHT];
            //Bitmap mBitmaps= Bitmap.createBitmap(mColors, 0, STRIDE, WIDTH, HEIGHT, Bitmap.Config.ARGB_8888);
            Canvas c = null;
            while(isRun)
            {
                //s = MainActivity.getCount();
                try
                {
                    synchronized (holder){

                        mColors = initColors(myCount,mColors);
                        c = holder.lockCanvas();//锁定画布，一般在锁定后就可以通过其返回的画布对象Canvas，在其上面画图等操作了。
                        c.drawColor(Color.BLACK);//设置画布背景颜色
                        Paint p = new Paint(); //创建画笔
                        p.setColor(Color.WHITE);
                        c.drawBitmap(mColors, 0, STRIDE, 0, 0, WIDTH, HEIGHT, true, p);
                        //c.translate(0, HEIGHT);
                        //c.drawText("这是第"+(count++)+"秒"+s[0]+" "+s[1]+" "+s[2]+" "+s[3]+" "+s[4], 50, 100, p);
                        //count++;
                        //Log.i("MyMultiDeepTCDView","mColors[5]:"+mColors[5*WIDTH]);
                        myCount++;
                        if(myCount == WIDTH){
                            myCount = 0;
                        }
                        Thread.sleep(10);//睡眠时间为1秒
                    }
                }
                catch (Exception e) {
                    e.printStackTrace();
                }
                finally
                {
                    if(c!= null)
                    {
                        holder.unlockCanvasAndPost(c);//结束锁定画图，并提交改变。
                    }
                }
            }
        }
    }
}



