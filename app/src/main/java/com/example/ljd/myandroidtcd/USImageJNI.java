package com.example.ljd.myandroidtcd;

/**
 * Created by ljd-pc on 2016/5/3.
 */
public class USImageJNI {
    static {
        System.loadLibrary("USImageJNI");
    }
    public static native String InitALLThreads();
    //public static native String TestString();
}
