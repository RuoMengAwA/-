package com.example.btdemo.ui;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.graphics.Typeface;
import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;

import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.os.SystemClock;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.example.btdemo.R;
import com.example.btdemo.bean.DetailBean;
import com.example.btdemo.bean.ReportBean;
import com.example.btdemo.databinding.FragmentHomepageBinding;
import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;
import com.gyf.immersionbar.ImmersionBar;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

public class HomepageFragment extends Fragment {

    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";
    private View rootView;//避免重复解析
    private Button login;
    Typeface mtypeface;
    FragmentHomepageBinding mbinding;
    List<ReportBean> locallist = new ArrayList<>();
    private DetailBean detailBean;
    private int flag = 0;
    static final int SUCCESS = 0;
    static final int FAILURE = 1;
    static final int EXCEPTION = 2;

    // TODO: Rename and change types of parameters
    private String mParam1;
    private String mParam2;

    public HomepageFragment() {
        // Required empty public constructor
    }


    public static HomepageFragment newInstance(String param1) {
        HomepageFragment fragment = new HomepageFragment();
        Bundle args = new Bundle();
        args.putString(ARG_PARAM1, param1);
        fragment.setArguments(args);
        return fragment;
    }

    Handler handler = new Handler(Looper.getMainLooper()){
        @SuppressLint("HandlerLeak")
        @Override
        public void handleMessage(@NonNull Message msg) {
//            loading.setVisibility(View.GONE);

            switch (msg.what){
                case SUCCESS:
                    if(flag ==1)
                    setView(locallist);
                    Log.d("123445", "handleMessage: " + locallist.size());
                    break;
                case FAILURE:
                    Toast.makeText(getContext(), "网络连接失败，请重试", Toast.LENGTH_SHORT).show();
                    break;
                case EXCEPTION:
                    Toast.makeText(getContext(), "网络连接出错，请检查", Toast.LENGTH_SHORT).show();
                    break;


            }
        }
    };

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (getArguments() != null) {
            mParam1 = getArguments().getString(ARG_PARAM1);
            mParam2 = getArguments().getString(ARG_PARAM2);
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        mbinding = FragmentHomepageBinding.inflate(inflater, container, false);
        transparentStatusBar();
        mtypeface = Typeface.createFromAsset(getActivity().getAssets(),"segoeuib.ttf");
        getDataAsync();
        mbinding.testbutton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Toast.makeText(getContext(), "获取中", Toast.LENGTH_SHORT).show();
                getDataAsync();
                SystemClock.sleep(3000);
                Toast.makeText(getContext(), "最新数据已更新", Toast.LENGTH_SHORT).show();
            }
        });

         mbinding.login2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(getActivity(),Login_Activity.class);
                startActivityForResult(intent, 0);
            }
        });

        return mbinding.getRoot();
    }

    protected void transparentStatusBar() {
        Window window = getActivity().getWindow();
        window.clearFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
        window.getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                | View.SYSTEM_UI_FLAG_LIGHT_STATUS_BAR);//全屏布局|深色状态(黑色字体)
        window.addFlags(WindowManager.LayoutParams.FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS);
        window.setStatusBarColor(Color.TRANSPARENT);//设置透明色


    }

    public void LoginCallback(String name)
    {
        mbinding.login2.setVisibility(View.GONE);
        mbinding.name.setText(name);
        mbinding.login2.setVisibility(View.INVISIBLE);
        mbinding.jiujiadata.setText("1");
        mbinding.boringdata.setText("9");
        mbinding.dangertimes.setText("17");
        flag = 1;
    }

    private void getDataAsync() {

        SharedPreferences sharedPreferences = getContext().getSharedPreferences("token", Context.MODE_PRIVATE);
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

    @Override
    public void onResume() {
        super.onResume();
        getDataAsync();
    }

    public void setView(List<ReportBean> locallist){
        String msg = locallist.get(3).getDescription();
        Gson gson2 = new Gson();
        detailBean = gson2.fromJson(msg, DetailBean.class);
        mbinding.Alcoholdata.setText(""+detailBean.getAc());
        mbinding.Alcoholdata.setTypeface(mtypeface);
        if(detailBean.getAc()>20 && detailBean.getAc() < 80){
            mbinding.textView66.setText("注意，疑似酒驾行为！");
            mbinding.textView66.setTextColor(Color.parseColor("FFFF2A2A"));
        }
        if(detailBean.getAc() >= 80){
            mbinding.textView66.setText("注意，疑似醉驾行为！");
            mbinding.textView66.setTextColor(Color.parseColor("FFFF2A2A"));
        }
        mbinding.textView53.setText("体温正常");
        mbinding.bloodp.setText(""+detailBean.getBlph()+"/"+detailBean.getBlpl());
        mbinding.bloodp.setTypeface(mtypeface);
        mbinding.textView59.setText("血压正常");
        mbinding.beat.setText(""+detailBean.getHea());
        mbinding.beat.setTypeface(mtypeface);
        mbinding.textView62.setText("心率正常");
        mbinding.tempdata.setText(""+36.5);
        mbinding.tempdata.setTypeface(mtypeface);
        mbinding.textView53.setText("体温正常");
        mbinding.blfat.setText(""+detailBean.getBls());
        mbinding.blfat.setTypeface(mtypeface);
        mbinding.textView63.setText("血脂正常");
        String a[] =locallist.get(3).getDateTime().split("T");
        String b[] = a[1].split("\\.");
        mbinding.reflash.setText("最近一次更新时间: "+a[0]+" "+b[0]);
    }
}