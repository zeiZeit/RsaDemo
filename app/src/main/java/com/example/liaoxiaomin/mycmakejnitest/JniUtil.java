package com.example.liaoxiaomin.mycmakejnitest;


/**
 * Created by liaoxiaomin on 2018/1/12.
 */

public class JniUtil {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }
    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public static native String SignByRSA(String text);
}
