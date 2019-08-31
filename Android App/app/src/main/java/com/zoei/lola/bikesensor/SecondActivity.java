package com.zoei.lola.bikesensor;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.TaskStackBuilder;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.SystemClock;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.CardView;
import android.view.View;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import org.apache.commons.lang3.StringUtils;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;

public class SecondActivity extends AppCompatActivity {

    private static final int ALERT_TYPE = 0;
    private static final int BATTERY_TYPE = 1;
    private static final String ALERT_RECV = "alert";
    private static final String GOOD_BYE = "Thanks for using the app. Bye!";
    public static final String HC_05 = "HC-05";
    TextView deviceDetails;
    String address = null, name = null;

    BluetoothAdapter myBluetooth = null;
    BluetoothSocket btSocket = null;
    Set<BluetoothDevice> pairedDevices;
    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    TextView mTextViewPercentage = null;
    ProgressBar mProgressBar = null;
    private final String EMPTY_STRING = "";
    private TextView alertWindow;
    private static final String ALERT_MSG = "Device is moving!";
    private static final int RECEIVE_MESSAGE = 1;

    private Handler handler;


    private NotificationManager manager;



    private void conNotify(){

        Intent intent = new Intent(this, SecondActivity.class);
//        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        // Create the TaskStackBuilder and add the intent, which inflates the back stack
        TaskStackBuilder stackBuilder = TaskStackBuilder.create(this);
        stackBuilder.addNextIntentWithParentStack(intent);

        PendingIntent pendingIntent =
                //PendingIntent.getActivity(SecondActivity.this,
                //        11, intent, PendingIntent.FLAG_UPDATE_CURRENT);
                stackBuilder.getPendingIntent(0, PendingIntent.FLAG_UPDATE_CURRENT);


        Notification.Builder builder = new Notification.Builder(SecondActivity.this);

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
        setContentView(R.layout.activity_second);
        try{
            createConnection();
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
        if (btSocket != null){
            Receive receiveThread = new Receive(btSocket);
            receiveThread.start();
        }
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


    private class Receive extends Thread{
        private final BluetoothSocket bluetoothSocket;
        private final InputStream inputStream;
        private final OutputStream outputStream;

        public Receive(BluetoothSocket socket){
            bluetoothSocket = socket;
            InputStream tempInputStream = null;
            OutputStream tempOutputStream = null;

            try {
                tempInputStream = bluetoothSocket.getInputStream();
                tempOutputStream = bluetoothSocket.getOutputStream();
            } catch (IOException e) {
                e.printStackTrace();
            }
            inputStream = tempInputStream;
            outputStream = tempOutputStream;
        }

        public void run(){
            byte[] buffer = new byte[1024];
            int bytes;
            while(true){
                try {
                    // Read from the InputStream
                    bytes = inputStream.available();
                    if(bytes != 0) {
                        SystemClock.sleep(100); //pause and wait for rest of data. Adjust this depending on your sending speed.
                        bytes = inputStream.available(); // how many bytes are ready to be read?
                        bytes = inputStream.read(buffer, 0, bytes); // record how many bytes we actually read
                        handler.obtainMessage(RECEIVE_MESSAGE, bytes, -1, buffer)
                                .sendToTarget(); // Send the obtained bytes to the UI activity
                    }
                    //TODO: send Ack message or something??
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }

        public void write(byte[] bytes){
            try {
                outputStream.write(bytes);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }


    }

    private void disconnect() {
        try {
            if(btSocket != null){
                btSocket.close();
            }

            Toast.makeText(this,GOOD_BYE, Toast.LENGTH_SHORT).show();
            finish();
            System.exit(0);


        } catch (IOException e) {
            e.printStackTrace();
        }

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

    private void createConnection() {
        try{
            myBluetooth = BluetoothAdapter.getDefaultAdapter();
            address = myBluetooth.getAddress();
            pairedDevices = myBluetooth.getBondedDevices();
            if(pairedDevices.size() > 0 ){
                for(BluetoothDevice bt: pairedDevices){
                    name = bt.getName().toString();
                    if(name.equals(HC_05)){
                        address = bt.getAddress().toString(); //"98:D3:31:F9:53:A2"
                        Toast.makeText(this, "device is connected", Toast.LENGTH_SHORT).show();
                    }

                }
            }
        } catch (Exception e){
            TextView msgBox = (TextView) findViewById(R.id.msg_box);
            msgBox.setText("No Bluetooth...");
            return;
        }
        myBluetooth = BluetoothAdapter.getDefaultAdapter();
        BluetoothDevice dispositivo = myBluetooth.getRemoteDevice(address);
        try {
            btSocket = dispositivo.createInsecureRfcommSocketToServiceRecord(myUUID);
            btSocket.connect();
            deviceDetails = (TextView)findViewById(R.id.device_details);
            deviceDetails.setText("Device name: " + name + "\nDevice address: "+address);
        } catch (IOException e) {
            e.printStackTrace();
        }

    }
}
