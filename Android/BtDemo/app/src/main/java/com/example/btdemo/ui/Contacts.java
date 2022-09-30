package com.example.btdemo.ui;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import com.example.btdemo.R;
import com.gyf.immersionbar.ImmersionBar;

public class Contacts extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_contacts);
        ImmersionBar.with(this)
                .statusBarDarkFont(true)   //状态栏字体是深色，不写默认为亮色
                .navigationBarDarkIcon(true) //导航栏图标是深色，不写默认为亮色
                .init();  //必须调用方可沉浸式
    }
}