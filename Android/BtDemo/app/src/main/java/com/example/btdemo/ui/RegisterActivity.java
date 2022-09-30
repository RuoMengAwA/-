package com.example.btdemo.ui;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;

import com.example.btdemo.R;
import com.example.btdemo.bean.RegisterBean;
import com.google.gson.Gson;
import com.gyf.immersionbar.ImmersionBar;

import java.io.IOException;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.FormBody;
import okhttp3.MediaType;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.RequestBody;
import okhttp3.Response;

public class RegisterActivity extends AppCompatActivity {



    private EditText mAccount;                        //用户名编辑
    private EditText mPwd;                            //密码编辑
    private EditText mEmail;
    private Button register;
    private RegisterBean registerBean;
    private static final String TAG = "6666666";
    private Handler handler;
    private Toolbar mToolbar;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        ImmersionBar.with(this)
                .statusBarDarkFont(true)   //状态栏字体是深色，不写默认为亮色
                .navigationBarDarkIcon(true) //导航栏图标是深色，不写默认为亮色
                .init();  //必须调用方可沉浸式
        setContentView(R.layout.activity_register);
        mAccount = findViewById(R.id.etAccount2);
        mPwd = findViewById(R.id.etPassword2);
        mEmail = findViewById(R.id.email);
        register = findViewById(R.id.btn_register1);
        register.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String user = mAccount.getText().toString().trim();//获取用户名
                String pwd = mPwd.getText().toString().trim();//获取密码
                String email = mEmail.getText().toString().trim();//获取邮箱;
                Log.d(TAG, user+pwd);
                    postData3(user, pwd , email);
            }
        });

        mToolbar = (Toolbar) this.findViewById(R.id.reg_toolbar);
        mToolbar.setTitle("");
        this.setSupportActionBar(mToolbar);
        if (getSupportActionBar() != null) {
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        }
        mToolbar.setNavigationOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                RegisterActivity.this.finish();
            }
        });


        handler = new Handler(Looper.getMainLooper()){
            @SuppressLint("HandlerLeak")
            @Override
            public void handleMessage(@NonNull Message msg) {
                RegisterBean registerBean = (RegisterBean ) msg.obj;
                if(registerBean.getErrors().equals("")) {
                    Toast.makeText(RegisterActivity.this, "注册成功！", Toast.LENGTH_SHORT).show();
                    Intent intent1 = new Intent(RegisterActivity.this, Login_Activity.class);
                    startActivity(intent1);
                    finish();
                }
                else{
                    Toast.makeText(RegisterActivity.this, registerBean.getErrors().toString(), Toast.LENGTH_SHORT).show();
                }
            }
        };
    }

    private void postData3(String username,String password,String email) {
        Register login = new Register();
        login.setUsername(username);
        login.setPassword(password);
        login.setEmail(email);
        Gson gson = new Gson();
        //使用Gson将对象转换为json字符串
        String json = gson.toJson(login);
        //MediaType  设置Content-Type 标头中包含的媒体类型值
        RequestBody requestBody = FormBody.create(MediaType.parse("application/json; charset=utf-8")
                , json);
        OkHttpClient client = new OkHttpClient();//创建OkHttpClient对象。
        Request request = new Request.Builder()//创建Request 对象。
                .url("https://tmonit.akasaki.space/api/user/register")
                .post(requestBody)//传递请求体
                .build();
        client.newCall(request).enqueue(new Callback() {
            @Override
            public void onFailure(Call call, IOException e) {
            }

            @Override
            public void onResponse(Call call, Response response) throws IOException {
                if (response.isSuccessful()) {//回调的方法执行在子线程。
                    Gson gson2 = new Gson();
                    registerBean = gson2.fromJson(response.body().string(),RegisterBean.class);

                    Message msg = new Message();
                    msg.obj = registerBean;
                    handler.sendMessage(msg);
                }
            }
        });
    }

    class Register{
        private String username;
        private String password ;
        private String email;


        public String getUsername() {
            return username;
        }

        public void setUsername(String username) {
            this.username = username;
        }

        public String getPassword() {
            return password;
        }

        public void setPassword(String password) {
            this.password = password;
        }

        public String getEmail() {
            return email;
        }

        public void setEmail(String email) {
            this.email = email;
        }

    }
}
