package com.zoei.lola.bikesensor;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.TaskStackBuilder;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.CardView;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import org.apache.commons.lang3.StringUtils;

public class TestActivity extends AppCompatActivity {

    private static final int ALERT_TYPE = 0;
    private static final int BATTERY_TYPE = 1;
    private static final String ALERT_RECV = "alert";
    private static final String GOOD_BYE = "Back to all devices view";
    TextView deviceDetails;

    TextView mTextViewPercentage = null;
    ProgressBar mProgressBar = null;
    private final String EMPTY_STRING = "";
    private TextView alertWindow;
    private static final String ALERT_MSG = "Device is moving!";
    private static final int RECEIVE_MESSAGE = 1;

    private Handler handler;


    private NotificationManager manager;

    private void conNotify(){

        Intent intent = new Intent(this, TestActivity.class);
//        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        // Create the TaskStackBuilder and add the intent, which inflates the back stack
        TaskStackBuilder stackBuilder = TaskStackBuilder.create(this);
        stackBuilder.addNextIntentWithParentStack(intent);

        PendingIntent pendingIntent =
                //PendingIntent.getActivity(SecondActivity.this,
                //        11, intent, PendingIntent.FLAG_UPDATE_CURRENT);
                stackBuilder.getPendingIntent(0, PendingIntent.FLAG_UPDATE_CURRENT);


        Notification.Builder builder = new Notification.Builder(TestActivity.this);

        builder.setAutoCancel(true);
        builder.setTicker("this is ticker text");
        builder.setContentTitle("BikeSensor: Alert!");
        builder.setContentText("Bike is moving!");
        builder.setSmallIcon(R.drawable.bt_icon);
        builder.setContentIntent(pendingIntent);
        builder.setOngoing(true);
        builder.setSubText("Bike Alert");   //API level 16
        builder.setNumber(100);

        Notification myNotication = builder.build();
        manager.notify(11, myNotication);

    }



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_test);
        try{
            deviceDetails = (TextView)findViewById(R.id.device_details);
            deviceDetails.setText("In test mode...");

        }catch (Exception e){

        }

        CardView loginBtn = (CardView)findViewById(R.id.clean_alert_btn);

        loginBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                cleanAlertWindow();
            }
        });

        CardView disconnectBtn = (CardView)findViewById(R.id.disconnect);
        disconnectBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                disconnect();
            }
        });

        mTextViewPercentage = findViewById(R.id.tv_percentage);
        mProgressBar = findViewById(R.id.pb);

        execute();
        manager = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);



//        setBattery(90);

    }

    private void execute(){
        handler = new Handler() {
            public void handleMessage(android.os.Message msg) {
                switch (msg.what) {
                    case RECEIVE_MESSAGE:                                                   // if receive massage
                        byte[] readBuf = (byte[]) msg.obj;
                        String strIncom = new String(readBuf, 0, msg.arg1);        // create string from bytes array
                        handleMsg(strIncom);
                        break;
                }
            };
        };
        testConfiguration();
    }

    private void handleMsg(String strIncom) {
        TextView msgBox = (TextView) findViewById(R.id.msg_box);
        msgBox.setText(strIncom);
        if(msgType(strIncom) == ALERT_TYPE){
            //e.g "alert"
            setAlertToAlertWindow();
        }
        else if(msgType(strIncom) == BATTERY_TYPE){ //BATTERY_TYPE
            //e.g 83
            int precentage = getPrecentage(strIncom);
            setBattery(precentage);
        }
    }

    private int getPrecentage(String strIncom) {
        //need to define...
        return Integer.parseInt(strIncom);
    }

    private int msgType(String strIncom) {
        //need to define type of msg
        if(strIncom.equals(ALERT_RECV)){
            return ALERT_TYPE;
        }
        else if(StringUtils.isNumeric(strIncom) && strIncom.length() <= 3){
            return BATTERY_TYPE;
        }
        else{
            return -1;
        }
    }

    private void disconnect() {
        Toast.makeText(this,GOOD_BYE, Toast.LENGTH_SHORT).show();
        finish();

    }

    private void testConfiguration(){
        Button alertBtn = (Button)findViewById(R.id.alert_button);
        alertBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                setAlertToAlertWindow();
            }
        });

        Button precentageBtn = (Button)findViewById(R.id.precentage_btn);
        precentageBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                EditText precentage = (EditText)findViewById(R.id.test_precentage);
                setBattery(Integer.parseInt(precentage.getText().toString()));
            }
        });

        Button cleanMsgButton = (Button)findViewById(R.id.clean_msg);
        cleanMsgButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                TextView msgBox = (TextView)findViewById(R.id.msg_box);
                msgBox.setText("");
            }
        });
        Button testMockMsg = (Button)findViewById(R.id.go_btn);
        testMockMsg.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                EditText msgTest = (EditText)findViewById(R.id.msg_test);
                String msg = msgTest.getText().toString();
                byte[] buffer = msg.getBytes();
                handler.obtainMessage(RECEIVE_MESSAGE, msg.length(), -1, buffer).sendToTarget();
            }
        });

    }

    private void cleanAlertWindow() {
        alertWindow = (TextView)findViewById(R.id.alert_window);
        alertWindow.setText(EMPTY_STRING);
    }

    private void setAlertToAlertWindow(){
        alertWindow = (TextView)findViewById(R.id.alert_window);
        alertWindow.setText(ALERT_MSG);
        conNotify();
    }


    private void setBattery(int precentage){
        mProgressBar.setProgress(precentage);
        mTextViewPercentage.setText(precentage+"%");
    }
}
