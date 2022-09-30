package com.example.btdemo.ui;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Typeface;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;
import android.widget.Toast;

import com.bumptech.glide.Glide;
import com.example.btdemo.R;
import com.example.btdemo.bean.DetailBean;
import com.example.btdemo.bean.ReportBean;
import com.example.btdemo.databinding.ActivityReportdetailBinding;
import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;
import com.gyf.immersionbar.ImmersionBar;

import java.io.IOException;
import java.util.List;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

public class ReportdetailActivity extends AppCompatActivity {

    private DetailBean detailBean;
    ActivityReportdetailBinding mBinding;
    private String image;
    static final int SUCCESS = 0;
    static final int FAILURE = 1;
    static final int EXCEPTION = 2;

    Handler handler = new Handler(Looper.getMainLooper()){
        @SuppressLint("HandlerLeak")
        @Override
        public void handleMessage(@NonNull Message msg) {

            switch (msg.what){
                case SUCCESS:
                    break;
                case FAILURE:
                    Toast.makeText(ReportdetailActivity.this, "网络连接失败，请重试", Toast.LENGTH_SHORT).show();
                    break;
                case EXCEPTION:
                    Toast.makeText(ReportdetailActivity.this, "网络连接出错，请检查", Toast.LENGTH_SHORT).show();
                    break;


            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mBinding = ActivityReportdetailBinding.inflate(getLayoutInflater());
        setContentView(mBinding.getRoot());
        Typeface mtypeface=Typeface.createFromAsset(getAssets(),"segoeuib.ttf");
        ImmersionBar.with(this).
                transparentStatusBar()  //透明状态栏，不写默认透明色
                .statusBarDarkFont(true)   //状态栏字体是深色，不写默认为亮色
                .navigationBarDarkIcon(true) //导航栏图标是深色，不写默认为亮色
                .init();  //必须调用方可沉浸式
        Intent intent = getIntent();
        String msg = intent.getStringExtra("data");
        String time = intent.getStringExtra("time");
        String dan = intent.getStringExtra("dan");
        image = intent.getStringExtra("image");
        Gson gson2 = new Gson();
        detailBean = gson2.fromJson(msg,DetailBean.class);
        String a[] =time.split("T");
        String b[] = a[1].split("\\.");
        mBinding.drivemsg.setText("驾驶人：xiaomai \n反馈时间："+ a[0]+" "+b[0]);
        mBinding.ac.setText(""+detailBean.getAc());
        mBinding.ac.setTypeface(mtypeface);
        mBinding.blp.setText(""+detailBean.getBlph()+"/"+detailBean.getBlpl());
        mBinding.blp.setTypeface(mtypeface);
        mBinding.hea.setText(""+detailBean.getHea());
        mBinding.hea.setTypeface(mtypeface);
        mBinding.tem.setText(""+36.5);
        mBinding.tem.setTypeface(mtypeface);
        mBinding.bls.setText(""+detailBean.getBls());
        mBinding.bls.setTypeface(mtypeface);
        mBinding.sdnn.setText(""+detailBean.getSdnn());
        mBinding.sdnn.setTypeface(mtypeface);
        int dan1 = Integer.parseInt(dan);
        Log.d("12321", "onCreate: " + dan1);
        switch (dan1){
            case 1:
                mBinding.illegal.setText("捕捉到危险行为: 喝酒");break;
            case 2:
                mBinding.illegal.setText("捕捉到危险行为: 喝水");break;
            case 3:
                mBinding.illegal.setText("捕捉到危险行为: 打电话");break;
            case 4:
                mBinding.illegal.setText("捕捉到危险行为: 驾驶员疲劳");break;
        }
        // 设置图片
        Glide.with(this).load("https://tmonit.akasaki.space" + image).into((ImageView)mBinding.illimage);
    }

}