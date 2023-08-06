package com.Source.S1_MCC.MCC.Trans;

import android.content.Context;
import android.content.Intent;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.os.Build;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.PowerManager;
import android.support.annotation.RequiresApi;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.TextView;

import com.Source.S1_MCC.MCC.CloseActivityClass;
import com.Source.S1_MCC.MCC.Main.MainActivity;
import com.Source.S1_MCC.MCC.R;
import com.Source.S1_MCC.MCC.SysApplication;
import com.Source.S1_MCC.MCC.model.DemoAppActivity;
import com.Source.S1_MCC.MCC.wub_lib;

import java.io.File;

public class DCCFxRateUI extends DemoAppActivity {

    Button dcc_currency;
    Button local_currency;
    Button cancel_button;

    TextView textView01;
    TextView textView02;
    TextView textView03;
    TextView textView04;
    TextView textView_txn;
    TextView textView_timer;
    TextView textView_dcc;
    TextView textView_local;

    String dcc_image_path;
    String dcc_filename;
    String local_image_path;
    String local_filename;
    File image_file;
    Bitmap myimage;

    public static String final_string;

    private int inTimeOut = 0;

    protected PowerManager.WakeLock mWakeLock;


    CountDownTimer ctimer = null;

    @RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN)
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        CloseActivityClass.activityList.add(this);
        SysApplication.getInstance().addActivity(this);
        super.onCreate(savedInstanceState);
        Bundle bundle = getIntent().getExtras();

        overridePendingTransition(0, 0); // disable the animation, faster

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

        setContentView(R.layout.dcc_fxrate);

        /* This code together with the one in onDestroy()
         * will make the screen be always on until this Activity gets destroyed. */
        final PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        this.mWakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "S1_MCC:DCCFxRateUI>>WAKE_LOCK");
        this.mWakeLock.acquire();

        Intent intent=getIntent();
        String dispmsg=intent.getStringExtra("pass_in_string");
        Log.i("dispmsg", dispmsg);

        textView_txn = (TextView) findViewById(R.id.textView_txn);
        textView01 = (TextView) findViewById(R.id.msg_text_01);
        textView02 = (TextView) findViewById(R.id.msg_text_02);
        textView03 = (TextView) findViewById(R.id.msg_text_03);
        textView04 = (TextView) findViewById(R.id.msg_text_04);
        textView_dcc = (TextView) findViewById(R.id.dcc_msg_curr);
        textView_local = (TextView) findViewById(R.id.local_msg_curr);
        textView_timer = (TextView) findViewById(R.id.textViewtimer);

        dcc_currency = (Button) findViewById(R.id.IPT_OKButton);
        local_currency = (Button) findViewById(R.id.btn_local_curr);
        cancel_button = (Button) findViewById(R.id.IPT_CANCELButton);

        String[] dispmsginfo = dispmsg.split("\\|");
        int msgcnt = dispmsginfo.length;

        System.out.println("msgcnt [" + msgcnt + "]");

        for (int inIdx = 0 ; inIdx < msgcnt; inIdx++)
        {
            System.out.println("split msg [" + inIdx + "][" + dispmsginfo[inIdx] + "]");
            switch (inIdx)
            {
                case 0:
                    textView_txn.setText(dispmsginfo[inIdx]);
                    break;
                case 1:
                    textView01.setText(dispmsginfo[inIdx]);
                    break;
                case 2:
                    textView02.setText(dispmsginfo[inIdx]);
                    break;
                case 3:
                    textView03.setText(dispmsginfo[inIdx]);
                    break;
                case 4:
                    textView04.setText(dispmsginfo[inIdx]);
                    break;
                case 5:
                    //dcc_currency.setText(dispmsginfo[inIdx]);

                    dcc_filename = dispmsginfo[inIdx].substring(0, 3);
                    System.out.println("dcc_filename [" + dcc_filename + "]");

                    // Diplay image
                    dcc_image_path = "/data/data/pub/"+dcc_filename.toLowerCase()+".png";
                    System.out.println("dcc_image_path [" + dcc_image_path + "]");
                    image_file = new File(dcc_image_path);
                    myimage = BitmapFactory.decodeFile(image_file.getAbsolutePath());
                    dcc_currency.setBackground(new BitmapDrawable(getResources(), myimage));

                    // Display message
                    textView_dcc.setText(dispmsginfo[inIdx]);
                    break;
                case 6:
                    //local_currency.setText(dispmsginfo[inIdx]);

                    local_filename = dispmsginfo[inIdx].substring(0, 3);
                    System.out.println("local_filename [" + local_filename + "]");

                    // Diplay image
                    local_image_path = "/data/data/pub/"+local_filename.toLowerCase()+".png";
                    System.out.println("local_image_path [" + local_image_path + "]");
                    image_file = new File(local_image_path);
                    myimage = BitmapFactory.decodeFile(image_file.getAbsolutePath());
                    local_currency.setBackground(new BitmapDrawable(getResources(), myimage));

                    // Display message
                    textView_local.setText(dispmsginfo[inIdx]);
                    break;

                case 7: //UI Timer
                    textView_timer.setText(dispmsginfo[inIdx]);
                    String textViewTime = textView_timer.getText().toString();
                    inTimeOut = Integer.parseInt(textViewTime);
                    break;
            }
        }


        /*Start timer*/
        getTimerRestart(inTimeOut);


        dcc_currency.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                /*cancel timer first*/
                getTimerCancel();

                //dAmount = Double.parseDouble(etAmount.getText().toString());
                final_string = "DCC_CURR";
                //final_string = final_string.replaceAll("[$,.]", "");

                //startActivity(new Intent(ConfirmMenu.this,MainActivity.class));
                DCCFxRateUI.this.finish();

                Log.i("Tine", "Get Amount KeyBoard buOK");

                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();

                }

            }
        });

        local_currency.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                /*cancel timer first*/
                getTimerCancel();

                //dAmount = Double.parseDouble(etAmount.getText().toString());
                final_string = "LOCAL_CURR";
                //final_string = final_string.replaceAll("[$,.]", "");

                //startActivity(new Intent(ConfirmMenu.this,MainActivity.class));
                DCCFxRateUI.this.finish();

                Log.i("Tine", "Get Amount KeyBoard buOK");

                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();

                }

            }
        });

        cancel_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                /*cancel timer first*/
                getTimerCancel();

                final_string = "CANCEL";
                DCCFxRateUI.this.finish();

                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();

                }
            }
        });
    }

    /**
     * 取消倒计时
     */
    public void getTimerCancel() {
        timer.cancel();
        if(ctimer != null)
            ctimer.cancel();
    }

    /**
     * 开始倒计时
     */
    public void getTimerRestart(int cTimeOut)
    {
        Log.i("ctimer", "Timer set at: " + cTimeOut);
        ctimer = new CountDownTimer(cTimeOut*1000, 1000) {

            @Override
            public void onTick(long millisUntilFinished) {
                Log.i("Timer", "Timer onTick");
            }

            @Override
            public void onFinish() {
                Log.i("Timer", "Timer onFinish");

                //etInputStr.clear();
                final_string = "TO";

                //startActivity(new Intent(S1InputString.this,MainActivity.class));

                Log.i("PATRICK", "Timeout UserInputString");
                DCCFxRateUI.this.finish();

                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();
                }
            }
        }.start();

    }

    private CountDownTimer timer = new CountDownTimer(inTimeOut*1000, 1000) {

        @Override
        public void onTick(long millisUntilFinished) {
            Log.i("Timer", "Timer onTick");
        }

        @Override
        public void onFinish() {
            Log.i("Timer", "Timer onFinish");

            //etInputStr.clear();
            final_string = "TIME OUT";

            //startActivity(new Intent(ConfirmMenu.this,MainActivity.class));

            Log.i("PATRICK", "Timeout UserInputString");
            DCCFxRateUI.this.finish();

            // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
            synchronized (MainActivity.LOCK) {
                MainActivity.LOCK.setCondition(true);
                MainActivity.LOCK.notifyAll();
            }
        }
    };

    @Override
    public void onBackPressed() {

    }

    @Override
    protected void onDestroy() {
        this.mWakeLock.release();
        super.onDestroy();
        final_string = null;
        ctimer = null;
        SysApplication.getInstance().removeActivity(this);
    }

}
