package com.example.btdemo.ui;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Toast;


import com.example.btdemo.R;
import com.example.btdemo.bean.ReportBean;
import com.google.android.material.bottomnavigation.BottomNavigationView;
import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;
import com.gyf.immersionbar.ImmersionBar;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;
import androidx.viewpager2.widget.ViewPager2;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

public class MainActivity extends AppCompatActivity
{


    private ViewPager2 viewPager;
    private static final String TAG = "66666";
    ArrayList<Fragment>fragments = new ArrayList<>();
    private Fragment[] fragment;
    private String[] mTitles = {"1","2","3"};
    private FragmentManager manager;
    private FragmentTransaction transaction;
    private int lastFragemtntIndex;
    private My_Fragment my_fragment;
    private HomepageFragment homepageFragment;
    private DrivereportFragment drivereportFragment;
    private List<ReportBean> locallist= new ArrayList<>();
    static final int SUCCESS = 0;
    static final int FAILURE = 1;
    static final int EXCEPTION = 2;



    Handler handler = new Handler(Looper.getMainLooper()){
        @SuppressLint("HandlerLeak")
        @Override
        public void handleMessage(@NonNull Message msg) {
//            loading.setVisibility(View.GONE);

            switch (msg.what){
                case SUCCESS:
                    Log.d("123445", "handleMessage: " + locallist.size());
                    break;
                case FAILURE:
                    Toast.makeText(MainActivity.this, "网络连接失败，请重试", Toast.LENGTH_SHORT).show();
                    break;
                case EXCEPTION:
                    Toast.makeText(MainActivity.this, "网络连接出错，请检查", Toast.LENGTH_SHORT).show();
                    break;


            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
       transparentStatusBar();
        setContentView(R.layout.activity_main);
        my_fragment = new My_Fragment();
        homepageFragment =new HomepageFragment();
        drivereportFragment = new DrivereportFragment();
        fragment = new Fragment[]{
                homepageFragment,
                drivereportFragment,
                my_fragment
        };
        lastFragemtntIndex = 0;
        getSupportFragmentManager().beginTransaction()
                .add(R.id.main_fra,fragment[0])
                .commit();


        //BottomNavigationView 点击事件监听
        final BottomNavigationView navView = findViewById(R.id.nav_view);
        navView.setOnNavigationItemSelectedListener(menuItem -> {
            int menuId = menuItem.getItemId();
            // 跳转指定页面：Fragment
            switch (menuId) {
                case R.id.navigation_home:
                    navView.getMenu().getItem(0).setChecked(true);
                    switchFragemnet(0);
                    break;
                case R.id.navigation_dashboard:
                    switchFragemnet(1);
                    navView.getMenu().getItem(1).setChecked(true);
                    break;
                case R.id.navigation_notifications:
                    switchFragemnet(2);
                    navView.getMenu().getItem(2).setChecked(true);
                    break;
            }
            return false;
        });

    }

    protected void transparentStatusBar() {
        Window window = getWindow();
        window.clearFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
        window.getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                | View.SYSTEM_UI_FLAG_LIGHT_STATUS_BAR);//全屏布局|深色状态(黑色字体)
        window.addFlags(WindowManager.LayoutParams.FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS);
        window.setStatusBarColor(Color.TRANSPARENT);//设置透明色


    }


    private void switchFragemnet(int to) {

        if(lastFragemtntIndex == to){
            return;
        }
        FragmentTransaction fragmentTransaction =
                getSupportFragmentManager().beginTransaction();
        if(!fragment[to].isAdded()){
            fragmentTransaction.add(R.id.main_fra,fragment[to]);
        }else {
            fragmentTransaction.show(fragment[to]);
        }
        fragmentTransaction.hide(fragment[lastFragemtntIndex])
                .commitAllowingStateLoss();//一定要commit！
        lastFragemtntIndex = to;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (resultCode == RESULT_OK)
        {
            String name = data.getStringExtra("name1");
            my_fragment.onLoginCallback(name);
            drivereportFragment.onLCallback(name);
            homepageFragment.LoginCallback(name);
        }
    }

    private void getDataAsync() {
        SharedPreferences sharedPreferences = this.getSharedPreferences("token",Context.MODE_PRIVATE);
        String token = sharedPreferences.getString("token", "");
        Log.d("6666611", "getDataAsync: " + token);
        //创建OkHttpClient对象
        OkHttpClient client = new OkHttpClient();
        //创建Request 对象
        Request request = new Request.Builder()
                .url("https://tmonit.akasaki.space/api/events")
                .addHeader("Authorization","Bearer "+token)
                .build();
        client.newCall(request).enqueue(new Callback() {
            @Override
            public void onFailure(Call call, IOException e) {
                handler.sendEmptyMessage(FAILURE);
            }
            @Override
            public void onResponse(Call call, Response response) throws IOException {
                Log.d("6666", "onResponse: " + response.body().toString() + "   " + response.code());
                if (response.code() == 200) {//回调的方法执行在子线程。
                    Log.d("6666", "onResponse " + response.body().toString());
                    Gson gson = new Gson();
                    List<ReportBean> reportBeans = gson.fromJson(response.body().string(), new TypeToken<List<ReportBean>>() {
                    }.getType());
                    Log.d("6666", "获取数据成功了");
                    Log.d("6666", "response.code()==" + reportBeans.get(0).getDescription());
                    locallist.addAll(reportBeans);
                    Log.d("666", "onResponse1: " +locallist.size());
                    handler.sendEmptyMessage(SUCCESS);

                }
            }
        });
    }

    /**
     * 活动被销毁回调，中断所有的监听线程，清理资源
     */
    @Override
    protected void onDestroy()
    {
        super.onDestroy();
    }

}