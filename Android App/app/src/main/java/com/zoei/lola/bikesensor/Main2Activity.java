package com.zoei.lola.bikesensor;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

import static com.zoei.lola.bikesensor.SecondActivity.HC_05;

public class Main2Activity extends AppCompatActivity {

    private BluetoothAdapter myBluetooth;
    private String address;
    private Set<BluetoothDevice> pairedDevices;
    private List<String> deviceToShow;
    private List<String> deviceToShowAddresses;
    ListView connectedDevices;
    private String TEST_MODE_ADDR = "testModeAddr";
    private String TEST_MODE = "testMode";
    private int TEST_MODE_POS = 0;
    private Map<Integer, Class> appFactory = new HashMap<>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main2);

        connectedDevices = findViewById(R.id.d_list);
        createFactory();
        getPairedDevices();
        createList();
    }

    private void createFactory() {
        appFactory.put(TEST_MODE_POS, TestActivity.class);
        appFactory.put(1,Device1Activity.class);
        appFactory.put(2,Device2Activity.class);
        appFactory.put(3,Device3Activity.class);
    }

    private void getPairedDevices() {
        myBluetooth = BluetoothAdapter.getDefaultAdapter();
        address = myBluetooth.getAddress();
        pairedDevices = myBluetooth.getBondedDevices();
    }

    private void createList() {
        deviceToShow = new ArrayList<>();
        deviceToShowAddresses = new ArrayList<>();
        deviceToShow.add(TEST_MODE);
        deviceToShowAddresses.add(TEST_MODE_ADDR);
        try{
            if(pairedDevices.size() > 0 ){
                for(BluetoothDevice bt: pairedDevices){
                    String name = bt.getName();
                    if(name.contains(HC_05)){
                        deviceToShow.add(name);
                        deviceToShowAddresses.add(bt.getAddress());
//                        address = bt.getAddress().toString(); //"98:D3:31:F9:53:A2"
//                        Toast.makeText(this, "device is connected", Toast.LENGTH_SHORT).show();
                    }
                }
            }
        } catch (Exception e){
            TextView msgBox = (TextView) findViewById(R.id.msg_box);
            msgBox.setText("No Bluetooth...");
            return;
        }
//        android.R.layout.simple_list_item_1
        ArrayAdapter adapter = new ArrayAdapter(this, R.layout.mtextview, deviceToShow);
        connectedDevices.setAdapter(adapter);
        connectedDevices.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                Intent intent = new Intent(Main2Activity.this, appFactory.get(position));

//                intent.putExtra("devName", getDeviceName(position));
                intent.putExtra("devAddress", getDeviceAddress(position));
                startActivity(intent);


            }
        });
    }

    private String getDeviceName(int position) {
        return deviceToShow.get(position);
    }

    private String getDeviceAddress(int position) {

        return deviceToShowAddresses.get(position);
    }

}
