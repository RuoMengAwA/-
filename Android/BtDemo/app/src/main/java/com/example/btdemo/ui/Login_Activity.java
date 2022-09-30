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
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;

import com.example.btdemo.R;
import com.example.btdemo.bean.LoginBean;
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

public class Login_Activity extends AppCompatActivity {

    private EditText mAccount;                        //用户名编辑
    private EditText mPwd;                            //密码编辑
    private Button login;
    private Button register;
    private Button quit;
    private LoginBean loginBean2;
    private static final String TAG = "6666666";
    private Context mContext;
    private Handler handler;
    private Toolbar mToolbar;
    private TextView login_text;
    private SharedPreferences pref;
    private SharedPreferences.Editor editor;
    private String user;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);
        ImmersionBar.with(this)
                .statusBarDarkFont(true)   //状态栏字体是深色，不写默认为亮色
                .navigationBarDarkIcon(true) //导航栏图标是深色，不写默认为亮色
                .init();  //必须调用方可沉浸式
        mAccount = findViewById(R.id.etAccount2);
        mPwd = findViewById(R.id.etPassword2);
        login = findViewById(R.id.register);
        register = findViewById(R.id.btn_register1);
        login.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
               user = mAccount.getText().toString().trim();//获取用户名
               String pwd = mPwd.getText().toString().trim();//获取密码
               Log.d(TAG, user+pwd);
               postData2(user,pwd);
            }
        });
       register.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent2 = new Intent(Login_Activity.this,RegisterActivity.class);
                startActivity(intent2);

            }
        });

        mToolbar = (Toolbar) this.findViewById(R.id.log_toolbar);
        mToolbar.setTitle("");
        login_text = this.findViewById(R.id.toolbarlog_title);
        this.setSupportActionBar(mToolbar);
        if (getSupportActionBar() != null) {
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        }


        mToolbar.setNavigationOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                Login_Activity.this.finish();

            }
        });

        if (login_text != null) {
            register.setTextColor(Color.parseColor("#5A5656"));
        }

        handler = new Handler(Looper.getMainLooper()){
            @SuppressLint("HandlerLeak")
            @Override
            public void handleMessage(@NonNull Message msg) {
                LoginBean login = (LoginBean ) msg.obj;
                if(!login.isSuccess()) {
                    Toast.makeText(Login_Activity.this, "账号密码不匹配", Toast.LENGTH_SHORT).show();
                }
                else{
                    SharedPreferences sharedPreferences = getSharedPreferences("token", Context.MODE_PRIVATE);
                    SharedPreferences.Editor editor = sharedPreferences.edit();//获取编辑器
                    editor.putString("token", loginBean2.getToken());
                    editor.commit();//提交修改
                    Toast.makeText(Login_Activity.this, "登录成功", Toast.LENGTH_SHORT).show();
                    Intent intent = new Intent(Login_Activity.this, MainActivity.class);
                    intent.putExtra("name1","xiaomai");
                    setResult(RESULT_OK, intent);
                    finish();
                    }
            }
        };
    }

    private void postData2(String email,String password) {

        Login login = new Login();
        login.setPassword("135246wdMM!");
        login.setEmail("xiaomai");
        Gson gson = new Gson();
        //使用Gson将对象转换为json字符串
        String json = gson.toJson(login);
        //MediaType  设置Content-Type 标头中包含的媒体类型值
        RequestBody requestBody = FormBody.create(MediaType.parse("application/json; charset=utf-8")
                , json);
        OkHttpClient client = new OkHttpClient();//创建OkHttpClient对象。
        Request request = new Request.Builder()//创建Request 对象。
                .url("https://tmonit.akasaki.space/api/user/login")
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
                    loginBean2 = gson2.fromJson(response.body().string(), LoginBean.class);
                    Message msg = new Message();
                    msg.obj = loginBean2;
                    handler.sendMessage(msg);
                }
            }
        });
    }

    class Login{
        private String password ;
        private String email;

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
