package com.zoei.lola.bikesensor;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.CardView;
import android.view.View;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    private EditText username;
    private EditText password;
    private CardView loginBtn;
    private CardView exitBtn;
    private CheckBox testCheck;
    private final String ADMIN = "admin";
    private final String PASS = "1234";


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        username = (EditText)findViewById(R.id.username);
        password = (EditText)findViewById(R.id.password);
        loginBtn = (CardView)findViewById(R.id.login_btn);
        exitBtn = (CardView)findViewById(R.id.exit_btn);
        testCheck = (CheckBox)findViewById(R.id.checkBox);

        loginBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                validate(username.getText().toString(), password.getText().toString());
            }
        });

        exitBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                finish();
            }
        });
    }

    private void validate(String username, String password){

        if(username.equals(ADMIN) && password.equals(PASS)){
            Boolean testMode = testCheck.isChecked();
            Intent intent;
            if(testMode){
                intent = new Intent(this, TestActivity.class);
            } else {
                intent = new Intent(this, Main2Activity.class);
            }

//        intent.putExtra("TEST_MODE",testMode);
            startActivity(intent);
//        finish();
        }else{
            Toast.makeText(this,"wrong details",Toast.LENGTH_SHORT).show();
        }
    }
}
