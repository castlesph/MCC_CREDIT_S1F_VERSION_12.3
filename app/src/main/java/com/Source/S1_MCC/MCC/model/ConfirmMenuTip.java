package com.Source.S1_MCC.MCC.model;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.PowerManager;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.Source.S1_MCC.MCC.CloseActivityClass;
import com.Source.S1_MCC.MCC.Main.MainActivity;
import com.Source.S1_MCC.MCC.R;
import com.Source.S1_MCC.MCC.SysApplication;

public class ConfirmMenuTip extends DemoAppActivity {

    private Context mContext;

    private Button mButton;
    final Context c = this;

    EditText etInputStr;
    Button buOK;
    Button buCancel;

    TextView textView_d1;
    TextView textView_d2;
    TextView textView_d3;
    TextView textView_d4;
    TextView textView_1;
    TextView textView_2;
    TextView textView_3;
    TextView textView_4;
    TextView textView_txn;
    TextView textView_cur;

    Double dAmount;
    int inResult;
    String stResult;

    boolean androidThinking;
    public static String final_string;
    public static String input_type;

    private int inTimeOut = 30;

    protected PowerManager.WakeLock mWakeLock;

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

        setContentView(R.layout.confirm_menu_tip);

        /* This code together with the one in onDestroy()
         * will make the screen be always on until this Activity gets destroyed. */
        final PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        this.mWakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "S1_MCC:AAA-ConfirmMenuTip>>WAKE_LOCK");
        this.mWakeLock.acquire();

        Intent intent=getIntent();
        String dispmsg=intent.getStringExtra("pass_in_string");
        Log.i("dispmsg", dispmsg);

        textView_txn = (TextView) findViewById(R.id.textView_txn);
        textView_cur = (TextView) findViewById(R.id.textView_cur);
        textView_d1 = (TextView) findViewById(R.id.tipadj_d1);
        textView_d2 = (TextView) findViewById(R.id.tipadj_d3);
        textView_d3 = (TextView) findViewById(R.id.tipadj_d7);
        textView_d4 = (TextView) findViewById(R.id.tipadj_d9);
        textView_1 = (TextView) findViewById(R.id.tipadj_d6);
        textView_2 = (TextView) findViewById(R.id.tipadj_d8);
        textView_3 = (TextView) findViewById(R.id.tipadj_d12);
        textView_4 = (TextView) findViewById(R.id.tipadj_d11);

        buOK = (Button) findViewById(R.id.IPT_OKButton);
        buCancel = (Button) findViewById(R.id.IPT_CancelButton);


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
                    textView_d1.setText(dispmsginfo[inIdx]);
                    break;
                case 2:
                    textView_d2.setText(dispmsginfo[inIdx]);
                    break;
                case 3:
                    textView_1.setText(dispmsginfo[inIdx]);
                    break;
                case 4:
                    textView_d3.setText(dispmsginfo[inIdx]);
                    break;
                case 5:
                    textView_2.setText(dispmsginfo[inIdx]);
                    break;
                case 6:
                    textView_d4.setText(dispmsginfo[inIdx]);
                    break;
                case 7:
                    textView_3.setText(dispmsginfo[inIdx]);
                    break;
                case 8:
                    textView_4.setText(dispmsginfo[inIdx]);
                    if(dispmsginfo[inIdx].equals("TOTAL AMOUNT"))
                        buOK.setText("PROCEED");
                    break;
                default:
                    break;
            }
        }


        /*Start timer*/
        getTimerRestart();

        buOK.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                /*cancel timer first*/
                getTimerCancel();

                //dAmount = Double.parseDouble(etAmount.getText().toString());
                final_string = "CONFIRM";
                //final_string = final_string.replaceAll("[$,.]", "");

                //startActivity(new Intent(ConfirmMenu.this,MainActivity.class));
                ConfirmMenuTip.this.finish();

                Log.i("PATRICK", "Get Amount KeyBoard buOK");

                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();

                }

            }
        });

        buCancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                /*cancel timer first*/
                getTimerCancel();

                final_string = "CANCEL";

                //startActivity(new Intent(ConfirmMenu.this,MainActivity.class));
                ConfirmMenuTip.this.finish();

                Log.i("PATRICK", "Get Amount KeyBoard buCancel");

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
    }

    /**
     * 开始倒计时
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

            //etInputStr.clear();
            final_string = "TIME OUT";

            //startActivity(new Intent(ConfirmMenu.this,MainActivity.class));

            Log.i("PATRICK", "Timeout UserInputString");
            ConfirmMenuTip.this.finish();

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

    //tine - to maintain fullscreen after screen is interrupted by warnings
    @Override
    protected void onResume() {
        super.onResume();
        getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);

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
        final_string = null;
        mContext = null;
        SysApplication.getInstance().removeActivity(this);
    }

}

