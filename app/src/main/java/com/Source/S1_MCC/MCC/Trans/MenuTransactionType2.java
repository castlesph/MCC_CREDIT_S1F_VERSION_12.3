package com.Source.S1_MCC.MCC.Trans;

import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.Handler;
import android.os.Message;
import android.os.PowerManager;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.TextView;

import com.Source.S1_MCC.MCC.EditDatabaseActivity;
import com.Source.S1_MCC.MCC.Main.MainActivity;
import com.Source.S1_MCC.MCC.OperatorHolder;
import com.Source.S1_MCC.MCC.R;
import com.Source.S1_MCC.MCC.SysApplication;
import com.Source.S1_MCC.MCC.model.DPopupMenu;
import com.Source.S1_MCC.MCC.model.DPopupMenuActivity;
import com.Source.S1_MCC.MCC.model.DemoAppActivity;
import com.Source.S1_MCC.MCC.model.TxnPopupMenu;

import CTOS.CtSystem;

public class MenuTransactionType2 extends DemoAppActivity {

    private boolean isFirstTrigger=true;
    private int flag=1, iHeader=0;
    private Button btn_title=null;
    private String TAG="MCC_Menu";

    private static Handler mHandler;

    public static String select_item2;
    public String TrxList, AppHeaderBuff;
    public String[] transItems = new String[20];
    public int inCnt=0;


    private int inTimeOut = 60;

    //private TxnPopupMenu mccMenu;
    private String MenuTitle, FullMenuItems;
    private String AppDetail1, AppDetail2, AppDetail, Networknm;
    private String[] MenuItems = new String[100];
    private String[] AppHeader = new String[100];
    private String[] TrxMenuItems = new String[100];
    private String[] TrxTitle = new String[100];

    private Button btn_nav_main;
    private Button btn_reports;
    private Button btn_settle;
    private Button btn_settings;
	private Button btn_credit;
	
	private Button btn_bancnet;
	
	private Button btn_wallet;
	
	private Button btn_phqr;

    private TextView textView_appversion;
    private TextView textView_merchant;
    private TextView textView_tprofile;
    private TextView textView_carrier_name;

    protected PowerManager.WakeLock mWakeLock;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.i("MenuTransactionType", "Popup MCC Menu onCreate");

        overridePendingTransition(0, 0); // disable the animation, faster
        OperatorHolder operatorHolder = new OperatorHolder(this);
        WifiManager wifiManager = (WifiManager) getApplicationContext().getSystemService(Context.WIFI_SERVICE);
        WifiInfo info = wifiManager.getConnectionInfo ();
        String ssid  = info.getSSID();

        getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);    //Tine:  Show Status Bar

        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                        // Set the content to appear under the system bars so that the
                        // content doesn't resize when the system bars hide and show.
                        //| View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        //| View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                        //| View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                        // Hide the nav bar and status bar
                        | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);
        //| View.SYSTEM_UI_FLAG_FULLSCREEN);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        setContentView(R.layout.mcc_popup_menu2);

        /* This code together with the one in onDestroy()
         * will make the screen be always on until this Activity gets destroyed. */
        final PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        this.mWakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "S1_MCC:AAA-MenuTransactionType>>WAKE_LOCK");
        this.mWakeLock.acquire();

        btn_nav_main = (Button) findViewById(R.id.btn_nav_main);
        btn_reports = (Button) findViewById(R.id.btn_reports);	
        btn_settle = (Button) findViewById(R.id.btn_settle);
        btn_settings = (Button) findViewById(R.id.btn_settings);
		btn_credit = (Button) findViewById(R.id.btn_credit);
		btn_bancnet = (Button) findViewById(R.id.btn_bancnet);
		btn_wallet = (Button) findViewById(R.id.btn_wallet);
		btn_phqr = (Button) findViewById(R.id.btn_phqr);

        textView_appversion = (TextView) findViewById(R.id.app_version);
        textView_merchant = (TextView) findViewById(R.id.merchant);
        textView_tprofile = (TextView) findViewById(R.id.tprofile);
        textView_carrier_name = (TextView) findViewById(R.id.NetworkCarrier);
        //btn_can.setText("hello");
        Intent intent = getIntent();
        String MenuBuff = intent.getStringExtra("pass_in_string");
        Log.i("2.main MenuBuff", MenuBuff);
        AppHeaderBuff = intent.getStringExtra("AppHeaderBuff");
        Log.i("main AppHeaderBuff", AppHeaderBuff);


        MenuTitle = MenuBuff.substring(0, MenuBuff.indexOf("|"));
        FullMenuItems = MenuBuff.substring(MenuBuff.indexOf("|") + 1);
        Log.i("MenuTitle", MenuTitle);
        Log.i("FullMenuItems", FullMenuItems);

        /*split out the menu items*/
        //MenuItems = FullMenuItems.split(" \n");
        TrxMenuItems = FullMenuItems.split(" \n");
        AppHeader = AppHeaderBuff.split(" \n");
        iHeader = AppHeader.length;

        for(int indx=0; indx<iHeader; indx++)
        {
            switch (indx) {
                case 0:
                    textView_appversion.setText(AppHeader[indx]);
                    break;
                case 1:
                    textView_merchant.setText(AppHeader[indx]);
                    break;
                case 2:
                    AppDetail1 = "MID: "+AppHeader[indx];
                    break;
                case 3:
                    AppDetail2 = "TID: "+AppHeader[indx];
                    break;
            }
        }
        AppDetail = AppDetail1 + "  |  " + AppDetail2;
        textView_tprofile.setText(AppDetail);

        if (ssid.equals("<unknown ssid>") || (ssid.length() <=0))
            Networknm = "Network: " + operatorHolder.getOperatorName();
        else
            Networknm = "Network: " + ssid;

        if (Networknm.equals("Network: "))
            Networknm = "Network: NOT CONNECTED";

        Log.i("AAA Networkname", "Networknm: " + Networknm);
        textView_carrier_name.setText(Networknm);


        btn_title = (Button) findViewById(R.id.mcc_menu_title);
        btn_title.setText(MenuTitle);

        /*Start timer*/
        getTimerRestart();

        FuncKeyCancel();
        FuncKeyReports();
        FuncKeySettings();
        FuncKeySettle();
		FuncCredit();
		FuncBancnet();
		FuncMCCPay();
		FuncMPHQR();


    }

    public void FuncCredit()
    {
        btn_credit.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                getTimerCancel();
                //mccMenu.dismiss();

                select_item2 = "1";
                //startActivity(new Intent(AmountEntryActivity.this, MainActivity.class));
                MenuTransactionType2.this.finish();

                Log.i("PATRICK", "MenuTransactionType credit"+select_item2);
                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();
                }

            }
        });
    }


    public void FuncBancnet()
    {
        btn_bancnet.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                getTimerCancel();
                //mccMenu.dismiss();

                select_item2 = "2";
                //startActivity(new Intent(AmountEntryActivity.this, MainActivity.class));
                MenuTransactionType2.this.finish();

                Log.i("PATRICK", "MenuTransactionType bancnet"+select_item2);
                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();
                }

            }
        });
    }

    public void FuncMCCPay()
    {
        btn_wallet.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                getTimerCancel();
                //mccMenu.dismiss();

                select_item2 = "3";
                //startActivity(new Intent(AmountEntryActivity.this, MainActivity.class));
                MenuTransactionType2.this.finish();

                Log.i("PATRICK", "MenuTransactionType bancnet"+select_item2);
                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();
                }

            }
        });
    }


    public void FuncMPHQR()
    {
        btn_phqr.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                getTimerCancel();
                //mccMenu.dismiss();

                select_item2 = "4";
                //startActivity(new Intent(AmountEntryActivity.this, MainActivity.class));
                MenuTransactionType2.this.finish();

                Log.i("PATRICK", "MenuTransactionType bancnet"+select_item2);
                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();
                }

            }
        });
    }


    public void FuncKeyReports()
    {
        btn_reports.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                getTimerCancel();
                //mccMenu.dismiss();

                select_item2 = "REPORTS";
                //startActivity(new Intent(AmountEntryActivity.this, MainActivity.class));
                MenuTransactionType2.this.finish();

                Log.i("PATRICK", "MenuTransactionType REPORTS"+select_item2);
                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();
                }

            }
        });
    }

    public void FuncKeySettle()
    {
        btn_settle.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                getTimerCancel();
                //mccMenu.dismiss();

                select_item2 = "SETTLE";
                //startActivity(new Intent(AmountEntryActivity.this, MainActivity.class));
                MenuTransactionType2.this.finish();

                Log.i("PATRICK", "MenuTransactionType SETTLE");
                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();
                }

            }
        });
    }
	

    public void FuncKeySettings()
    {
        btn_settings.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                getTimerCancel();
                //mccMenu.dismiss();

                //select_item = "50";
                //startActivity(new Intent(MenuTransactionType.this, EditDatabaseActivity.class));
                select_item2 = "SETTINGS";
                MenuTransactionType2.this.finish();

                Log.i("PATRICK", "MenuTransactionType SETTINGS");
                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                //CtSystem system = new CtSystem();
                //system.setNavBarIcon(true,true,true);
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();
                }

            }
        });
    }


    public void FuncKeyCancel() {
        btn_nav_main.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                getTimerCancel();
                //mccMenu.dismiss();
                //Toast.makeText(getApplicationContext(), "Cancelling...", Toast.LENGTH_SHORT).show();

                select_item2 = "CANCEL";
                //startActivity(new Intent(AmountEntryActivity.this, MainActivity.class));
                MenuTransactionType2.this.finish();

                Log.i("PATRICK", "DPopupMenuActivity KeyBoard btn_can");
                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();
                }

            }
        });
    }



    /**
     * å–æ¶ˆå€’è®¡æ—¶
     */
    public void getTimerCancel() {
        timer.cancel();
    }

    /**
     * å¼€å§‹å€’è®¡æ—¶
     */
    public void getTimerRestart()
    {
        timer.start();
    }

    private CountDownTimer timer = new CountDownTimer(inTimeOut*1000, 1000) {

        @Override
        public void onTick(long millisUntilFinished) {
            Log.i("Timer", "Timer onTick");
        }

        @Override
        public void onFinish() {
            Log.i("Timer", "Timer onFinish");

            //mccMenu.dismiss();
            select_item2 = "TO";

            //startActivity(new Intent(DPopupMenuActivity.this,MainActivity.class));


            Log.i("TINE", "Timeout DPopupMenuActivity");
            MenuTransactionType2.this.finish();

            // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
            synchronized (MainActivity.LOCK) {
                MainActivity.LOCK.setCondition(true);
                MainActivity.LOCK.notifyAll();
            }
        }
    };

    @Override
    public void onBackPressed() {
        super.onBackPressed();
        getTimerCancel();
        //mccMenu.dismiss();
        select_item2 = "CANCEL";
        //startActivity(new Intent(DPopupMenuActivity.this,MainActivity.class));
        MenuTransactionType2.this.finish();
        Log.i("PATRICK", "BackPressed DPopupMenuActivity");

        // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
        synchronized (MainActivity.LOCK) {
            MainActivity.LOCK.setCondition(true);
            MainActivity.LOCK.notifyAll();
        }
    }

    //tine - to maintain fullscreen after screen is interrupted by warnings
    @Override
    protected void onResume() {
        super.onResume();
        getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);    //Tine:  Show Status Bar

        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                        | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);

        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
    }

    @Override
    protected void onDestroy() {
        this.mWakeLock.release();
        super.onDestroy();
        select_item2 = null;
        AppHeaderBuff = null;
        mHandler = null;
        SysApplication.getInstance().removeActivity(this);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String ReadTrxMenu();

}
