package com.example.btdemo.ui;

import static android.view.View.VISIBLE;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.fragment.app.Fragment;
import androidx.recyclerview.widget.RecyclerView;
import androidx.recyclerview.widget.StaggeredGridLayoutManager;

import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Toast;

import com.chad.library.adapter.base.BaseQuickAdapter;
import com.chad.library.adapter.base.listener.OnItemClickListener;
import com.example.btdemo.R;
import com.example.btdemo.adapter.Drivereport_Adapter;
import com.example.btdemo.bean.ReportBean;
import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;


public class DrivereportFragment extends Fragment {


    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";
    private View rootView;//避免重复解析
    private RecyclerView recyclerView;
    private Drivereport_Adapter adapter;
    private List<ReportBean> locallist= new ArrayList<>();
    static final int SUCCESS = 0;
    static final int FAILURE = 1;
    static final int EXCEPTION = 2;
    private boolean isload = true;
    private ConstraintLayout loading;
    private ConstraintLayout nodata;
    private ReportBean reportBean;
    private int flag = 0;


    // TODO: Rename and change types of parameters
    private String mParam1;
    private String mParam2;

    public DrivereportFragment() {
        // Required empty public constructor
    }


    public static DrivereportFragment newInstance(String param1, String param2) {
        DrivereportFragment fragment = new DrivereportFragment();
        Bundle args = new Bundle();
        args.putString(ARG_PARAM1, param1);
        args.putString(ARG_PARAM2, param2);
        fragment.setArguments(args);
        return fragment;
    }

    Handler handler = new Handler(Looper.getMainLooper()){
        @SuppressLint("HandlerLeak")
        @Override
        public void handleMessage(@NonNull Message msg) {
            loading.setVisibility(View.GONE);
            switch (msg.what){
                case SUCCESS:
                    nodata.setVisibility(View.INVISIBLE);
                    Log.d("123445", "handleMessage: " + locallist.size());
                    recyclerView.setVisibility(VISIBLE);
                    adapter.notifyDataSetChanged();
                    break;
                case FAILURE:
                    recyclerView.setVisibility(View.GONE);
                    Toast.makeText(getContext(), "网络连接失败，请重试", Toast.LENGTH_SHORT).show();
                    adapter.notifyDataSetChanged();
                    break;
                case EXCEPTION:
                    recyclerView.setVisibility(View.GONE);
                    Toast.makeText(getContext(), "网络连接出错，请检查", Toast.LENGTH_SHORT).show();
                    adapter.notifyDataSetChanged();
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
        if (rootView == null ) {
            rootView = inflater.inflate(R.layout.fragment_drivereport, container, false);
        }
        loading = rootView.findViewById(R.id.loading);
        recyclerView = rootView.findViewById(R.id.fragment_view);
        recyclerView.setLayoutManager(new StaggeredGridLayoutManager(1, StaggeredGridLayoutManager.VERTICAL));
        adapter = new Drivereport_Adapter(R.layout.drivefilercy_item, locallist);
        nodata = rootView.findViewById(R.id.nodata);
        if(locallist.isEmpty()){
            nodata.setVisibility(VISIBLE);
        }
        adapter.setEmptyView(R.layout.nodata);
        recyclerView.setAdapter(adapter);
        adapter.setAnimationWithDefault(BaseQuickAdapter.AnimationType.SlideInBottom);
        adapter.setAnimationFirstOnly(false);

        //设置Item点击事件
        adapter.setOnItemClickListener(new OnItemClickListener() {
            @Override
            public void onItemClick(@NonNull BaseQuickAdapter<?, ?> adapter, @NonNull View view, int position) {
                Intent intent = new Intent(getActivity(), ReportdetailActivity.class);
                intent.putExtra("data",locallist.get(position).getDescription());
                intent.putExtra("image",locallist.get(position).getImage());
                intent.putExtra("time",locallist.get(position).getDateTime());
                intent.putExtra("dan",locallist.get(position).getDangerousLevel()+"");
                Log.d("66666",locallist.get(position).getDangerousLevel()+"" );
                startActivity(intent);
            }
        });
        return rootView;
    }

    public void onLCallback(String name)
    {
        flag = 1;
    }

        private void getDataAsync() {
        nodata.setVisibility(View.INVISIBLE);
        loading.setVisibility(VISIBLE);
        recyclerView.setVisibility(View.INVISIBLE);
        SharedPreferences sharedPreferences = getContext().getSharedPreferences("token",Context.MODE_PRIVATE);
        String token = sharedPreferences.getString("token", "");
        Log.d("66666", "getDataAsync: " + token);
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
                isload = false;
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
                    locallist.clear();
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
        if(flag == 1)
        getDataAsync();
    }
}