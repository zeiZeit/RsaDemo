package com.example.liaoxiaomin.mycmakejnitest;


import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Handler;
import android.os.IBinder;
import android.os.RemoteException;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import com.wind.emode.atci.IOpenAtci;


public class ClientMainActivity extends AppCompatActivity {
    private String TAG="ClientMainActivity";
    private Button encrytoRSA;
    private String SignText;
    private MyConn conn;
    private IOpenAtci iOpenAtciService;
    private String mCode;

    private Handler mhandler ;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mhandler = new Handler();
        encrytoRSA = (Button) findViewById(R.id.encrytoRSAbutton);
        encrytoRSA.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                myBindService();

            }
        });
    }


    private boolean myBindService() {
        final Intent service = new Intent();
        service.setAction("com.wind.emode.atci.action.IOpenAtci");
        service.setPackage("com.wind.emode");
        service.setComponent(new ComponentName("com.wind.emode","com.wind.emode.atci.OpenAtci"));
        conn = new MyConn();

        Runnable runnable=new Runnable(){

            @Override
            public void run() {
                // TODO Auto-generated method stub
                //要做的事情，这里再次调用此Runnable对象，以实现每两秒实现一次的定时器操作
                bindService(service,conn,BIND_AUTO_CREATE);

                if(iOpenAtciService!=null){
                    Log.i(TAG,"ClientMainActivity:: bindService success!!");
                    mhandler.removeCallbacks(this);
                    getCode();
                    getMyAction();
                }else {
                    Log.i(TAG,"ClientMainActivity::  bindService failed!!");
                    mhandler.postDelayed(this,1000);
                }
            }
        };

        if (iOpenAtciService!=null){
            getCode();
            getMyAction();
            return true;
        }else {
            mhandler.post(runnable);
            return false;
        }

    }

    // 这个类很重要！实现了一个接口，在这个类里边取到远程服务。
    private class MyConn implements ServiceConnection {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            Log.i(TAG, "in onServiceConnected and service ="+service.toString());
            iOpenAtciService = IOpenAtci.Stub.asInterface(service);
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {

        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (conn!=null){
            unbindService(conn);
        }
    }


    private void getMyAction() {
        if (iOpenAtciService!=null){
            try {
                SignText=JniUtil.SignByRSA(mCode);
                iOpenAtciService.returnCode(SignText);
            } catch (RemoteException e) {
                e.printStackTrace();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
    private void getCode() {
        if (iOpenAtciService != null) {
            try {
                mCode = iOpenAtciService.getCode();
            } catch (RemoteException e) {
                e.printStackTrace();
            }
        } else {
            Log.i(TAG,"ClientMainActivity::  getcode Failed!!");
        }
    }
}
