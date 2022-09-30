package com.example.btdemo.ui;

import android.content.Intent;
import android.graphics.Rect;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.recyclerview.widget.RecyclerView;

import com.example.btdemo.R;
import com.franmontiel.persistentcookiejar.ClearableCookieJar;
import com.franmontiel.persistentcookiejar.PersistentCookieJar;
import com.franmontiel.persistentcookiejar.cache.SetCookieCache;
import com.franmontiel.persistentcookiejar.persistence.SharedPrefsCookiePersistor;
import com.google.android.material.button.MaterialButton;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.Cookie;
import okhttp3.HttpUrl;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

/**
 * A simple {@link Fragment} subclass.
 * Activities that contain this fragment must implement the
 * to handle interaction events.
 */
public class My_Fragment extends Fragment {

    private static final String ARG_TEXT = "param2";
    private static final String TAG = "66666";
    private List<Cookie> validCookies;
    private Button carbind;
    private Button login;
    private Button mycollect;
    private TextView textView;
    private boolean iSload = false;
    private MaterialButton bind;
    private MaterialButton contract;



    // TODO: Rename and change types of parameters
    private String mTextString;
    private View rootView;//避免重复解析
    private RecyclerView recyclerView;


    public My_Fragment() {
        // Required empty public constructor
    }

    // TODO: Rename and change types and number of parameters
    public static My_Fragment newInstance(String param2) {
        My_Fragment fragment = new My_Fragment();
        Bundle args = new Bundle();
        args.putString(ARG_TEXT, param2);
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (getArguments() != null) {
            mTextString = getArguments().getString(ARG_TEXT);
        }

    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        if (rootView == null ) {
            rootView = inflater.inflate(R.layout.fragment_my, container, false);
        }
       textView = rootView.findViewById(R.id.loadname);
       login = rootView.findViewById(R.id.login);
       bind = rootView.findViewById(R.id.carbinding);
       contract = rootView.findViewById(R.id.contacts);
       login.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
               Intent intent = new Intent(getActivity(),Login_Activity.class);
               startActivityForResult(intent, 0);
            }
        });
        contract.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(getActivity(),Contacts.class);
                startActivityForResult(intent, 0);
            }
        });

        return rootView;
    }

    public void onLoginCallback(String name)
    {
        login.setVisibility(View.INVISIBLE);
        textView.setText("欢迎您:"+" "+name);
        bind.setText("    车辆绑定  (已绑定)");
    }

    @Override
    public void onResume() {
        //这是 Fragment 从创建到显示的最后一个回调的方法
        super.onResume();

    }

}
