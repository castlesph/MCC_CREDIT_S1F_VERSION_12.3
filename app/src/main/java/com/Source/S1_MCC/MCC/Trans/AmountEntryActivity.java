package com.Source.S1_MCC.MCC.Trans;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.os.SystemClock;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.Source.S1_MCC.MCC.CloseActivityClass;
import com.Source.S1_MCC.MCC.Main.MainActivity;
import com.Source.S1_MCC.MCC.R;
import com.Source.S1_MCC.MCC.SysApplication;
import com.Source.S1_MCC.MCC.model.DemoAppActivity;
import com.Source.S1_MCC.MCC.wub_lib;

import android.os.PowerManager;

public class AmountEntryActivity extends DemoAppActivity {

    private Context mContext;

    private Button mButton;
    final Context c = this;

    private Handler aHandler;

    Button btnKey1, btnKey2, btnKey3,btnKey4,btnKey5,btnKey6,btnKey7,btnKey8,btnKey9,btnKey0,btnKeyEnter,btnKeyCancel,btnKeyClear;


    TextView txtViewAmount, txtViewTitle, textView_txn, textView1;
    int amountminlength=0, amountmaxlength=0;

    String Amount="";
    private String isTIP;
    public static String final_amt_string="";

    private int inTimeOut = 30;

    private String any_amt = "";
	private String txn_curr = "";
	private String amount_buff = "";

    protected PowerManager.WakeLock mWakeLock;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        CloseActivityClass.activityList.add(this);
        SysApplication.getInstance().addActivity(this);
        super.onCreate(savedInstanceState);

        aHandler = new Handler(Looper.getMainLooper());
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

        if(MainActivity.inTermType == 5)
            setContentView(R.layout.activity_amount_entry_s1em);
        else
            setContentView(R.layout.activity_amount_entry);

        /* This code together with the one in onDestroy()
         * will make the screen be always on until this Activity gets destroyed. */
        final PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        this.mWakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "S1_MCC:AAA-AmountEntryActivity>>WAKE_LOCK");
        this.mWakeLock.acquire();

        Intent intent=getIntent();
        String maxlength=intent.getStringExtra("maxlength");
        String minlength=intent.getStringExtra("minlength");
        String type=intent.getStringExtra("type");

        String dispmsg = intent.getStringExtra("pass_in_string");
        Log.i("dispmsg", dispmsg);



        btnKeyCancel=(Button) findViewById(R.id.ibtnCancel);
        btnKeyEnter=(Button) findViewById(R.id.ibtnEnter);

        btnKey1=(Button) findViewById(R.id.ibtnOne);
        btnKey2=(Button) findViewById(R.id.ibtnTwo);
        btnKey3=(Button) findViewById(R.id.ibtnThree);
        btnKey4=(Button) findViewById(R.id.ibtnFour);
        btnKey5=(Button) findViewById(R.id.ibtnFive);
        btnKey6=(Button) findViewById(R.id.ibtnSix);
        btnKey7=(Button) findViewById(R.id.ibtnSeven);
        btnKey8=(Button) findViewById(R.id.ibtnEight);
        btnKey9=(Button) findViewById(R.id.ibtnNine);
        btnKey0=(Button) findViewById(R.id.ibtnZero);
        btnKeyCancel=(Button) findViewById(R.id.ibtnCancel);
        btnKeyClear=(Button) findViewById(R.id.ibtnClear);
        btnKeyEnter=(Button) findViewById(R.id.ibtnEnter);

        textView_txn=(TextView) findViewById(R.id.textView_txn) ;
        textView1=(TextView) findViewById(R.id.textView_cur);

        txtViewAmount=(TextView) findViewById(R.id.textViewAmount);

        amountmaxlength = Integer.valueOf(maxlength.toString());
        amountminlength = Integer.valueOf(minlength.toString());

        String[] dispmsginfo = dispmsg.split("\\|");
        int msgcnt = dispmsginfo.length;

        System.out.println("msgcnt [" + msgcnt + "]");

        for (int inIdx = 0; inIdx < msgcnt; inIdx++) {
            System.out.println("split msg [" + inIdx + "][" + dispmsginfo[inIdx] + "]");
            switch (inIdx) {
                case 0:
                    textView_txn.setText(dispmsginfo[inIdx]);
                    break;
                case 1:
                    textView1.setText(dispmsginfo[inIdx]);
                    isTIP = dispmsginfo[inIdx].substring(0, dispmsginfo[inIdx].indexOf(" "));
					Log.i("AmountEntry", "isTIP: " + isTIP);
                    break;
				case 2:
					txn_curr = (dispmsginfo[inIdx] + " ");
					amount_buff = txn_curr.concat("0.00");
                    txtViewAmount.setText(amount_buff);
                    break;
            }
        }

        /*Start timer*/
        getTimerRestart();

        FuncKey1();
        FuncKey2();
        FuncKey3();
        FuncKey4();
        FuncKey5();
        FuncKey6();
        FuncKey7();
        FuncKey8();
        FuncKey9();
        FuncKey0();
        FuncKeyCancel();
        FuncKeyClear();
        FuncKeyEnter();
    }

    public void FuncKey1() {
        btnKey1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                getTimerRestart();

                if(Amount.length() >= amountmaxlength) {
                    Toast.makeText(getApplicationContext(), "Maximum of " + amountmaxlength, Toast.LENGTH_SHORT).show();
                    return;
                }
                Amount=Amount+"1";
                //txtViewAmount.setText(tools.getFormatAmount(Amount).toString());
                //txtViewAmount.setText(Amount);
                amount_buff = txn_curr.concat(vdCTOSS_FormatAmountUI("NN,NNN,NNN,NNn.nn", Amount).toString());
                txtViewAmount.setText(amount_buff);

                // sidumili: Press animation
                wub_lib.ButtonPressed(c, btnKey1);
            }
        });
    }

    public void FuncKey2() {
        btnKey2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                getTimerRestart();

                if(Amount.length() >= amountmaxlength) {
                    Toast.makeText(getApplicationContext(), "Maximum of " + amountmaxlength, Toast.LENGTH_SHORT).show();
                    return;
                }
                Amount=Amount+"2";
                //txtViewAmount.setText(tools.getFormatAmount(Amount).toString());
                //txtViewAmount.setText(Amount);
                amount_buff = txn_curr.concat(vdCTOSS_FormatAmountUI("NN,NNN,NNN,NNn.nn", Amount).toString());
                txtViewAmount.setText(amount_buff);

                // sidumili: Press animation
                wub_lib.ButtonPressed(c, btnKey2);
            }
        });
    }

    public void FuncKey3() {
        btnKey3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                getTimerRestart();

                if(Amount.length() >= amountmaxlength) {
                    Toast.makeText(getApplicationContext(), "Maximum of " + amountmaxlength, Toast.LENGTH_SHORT).show();
                    return;
                }
                Amount=Amount+"3";
                //txtViewAmount.setText(tools.getFormatAmount(Amount).toString());
                //txtViewAmount.setText(Amount);
                amount_buff = txn_curr.concat(vdCTOSS_FormatAmountUI("NN,NNN,NNN,NNn.nn", Amount).toString());
                txtViewAmount.setText(amount_buff);

                // sidumili: Press animation
                wub_lib.ButtonPressed(c, btnKey3);
            }
        });
    }

    public void FuncKey4() {
        btnKey4.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                getTimerRestart();

                if(Amount.length() >= amountmaxlength) {
                    Toast.makeText(getApplicationContext(), "Maximum of " + amountmaxlength, Toast.LENGTH_SHORT).show();
                    return;
                }
                Amount=Amount+"4";
                //txtViewAmount.setText(tools.getFormatAmount(Amount).toString());
                //txtViewAmount.setText(Amount);
                amount_buff = txn_curr.concat(vdCTOSS_FormatAmountUI("NN,NNN,NNN,NNn.nn", Amount).toString());
                txtViewAmount.setText(amount_buff);

                // sidumili: Press animation
                wub_lib.ButtonPressed(c, btnKey4);
            }
        });
    }

    public void FuncKey5() {
        btnKey5.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                getTimerRestart();

                if(Amount.length() >= amountmaxlength) {
                    Toast.makeText(getApplicationContext(), "Maximum of " + amountmaxlength, Toast.LENGTH_SHORT).show();
                    return;
                }
                Amount=Amount+"5";
                //txtViewAmount.setText(tools.getFormatAmount(Amount).toString());
                //txtViewAmount.setText(Amount);
                amount_buff = txn_curr.concat(vdCTOSS_FormatAmountUI("NN,NNN,NNN,NNn.nn", Amount).toString());
                txtViewAmount.setText(amount_buff);

                // sidumili: Press animation
                wub_lib.ButtonPressed(c, btnKey5);

            }
        });
    }

    public void FuncKey6() {
        btnKey6.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                getTimerRestart();

                if(Amount.length() >= amountmaxlength) {
                    Toast.makeText(getApplicationContext(), "Maximum of " + amountmaxlength, Toast.LENGTH_SHORT).show();
                    return;
                }
                Amount=Amount+"6";
                //txtViewAmount.setText(tools.getFormatAmount(Amount).toString());
                //txtViewAmount.setText(Amount);
                amount_buff = txn_curr.concat(vdCTOSS_FormatAmountUI("NN,NNN,NNN,NNn.nn", Amount).toString());
                txtViewAmount.setText(amount_buff);

                // sidumili: Press animation
                wub_lib.ButtonPressed(c, btnKey6);
            }
        });
    }

    public void FuncKey7() {
        btnKey7.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                getTimerRestart();

                if(Amount.length() >= amountmaxlength) {
                    Toast.makeText(getApplicationContext(), "Maximum of " + amountmaxlength, Toast.LENGTH_SHORT).show();
                    return;
                }
                Amount=Amount+"7";
                //txtViewAmount.setText(tools.getFormatAmount(Amount).toString());
                //txtViewAmount.setText(Amount);
                amount_buff = txn_curr.concat(vdCTOSS_FormatAmountUI("NN,NNN,NNN,NNn.nn", Amount).toString());
                txtViewAmount.setText(amount_buff);

                // sidumili: Press animation
                wub_lib.ButtonPressed(c, btnKey7);
            }
        });
    }

    public void FuncKey8() {
        btnKey8.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                getTimerRestart();

                if(Amount.length() >= amountmaxlength) {
                    Toast.makeText(getApplicationContext(), "Maximum of " + amountmaxlength, Toast.LENGTH_SHORT).show();
                    return;
                }
                Amount=Amount+"8";
                //txtViewAmount.setText(tools.getFormatAmount(Amount).toString());
                //txtViewAmount.setText(Amount);
                amount_buff = txn_curr.concat(vdCTOSS_FormatAmountUI("NN,NNN,NNN,NNn.nn", Amount).toString());
                txtViewAmount.setText(amount_buff);

                // sidumili: Press animation
                wub_lib.ButtonPressed(c, btnKey8);
            }
        });
    }

    public void FuncKey9() {
        btnKey9.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                getTimerRestart();

                if(Amount.length() >= amountmaxlength) {
                    Toast.makeText(getApplicationContext(), "Maximum of " + amountmaxlength, Toast.LENGTH_SHORT).show();
                    return;
                }
                Amount=Amount+"9";
                //txtViewAmount.setText(tools.getFormatAmount(Amount).toString());
                //txtViewAmount.setText(Amount);
                amount_buff = txn_curr.concat(vdCTOSS_FormatAmountUI("NN,NNN,NNN,NNn.nn", Amount).toString());
                txtViewAmount.setText(amount_buff);

                // sidumili: Press animation
                wub_lib.ButtonPressed(c, btnKey9);
            }
        });
    }

    public void FuncKey0() {
        btnKey0.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                getTimerRestart();

                if(Amount.length() <= 0)
                    return;

                if(Amount.length() >= amountmaxlength) {
                    Toast.makeText(getApplicationContext(), "Maximum of " + amountmaxlength, Toast.LENGTH_SHORT).show();
                    return;
                }

                Amount = Amount + "0";
                //txtViewAmount.setText(tools.getFormatAmount(Amount).toString());
                //txtViewAmount.setText(Amount);
                amount_buff = txn_curr.concat(vdCTOSS_FormatAmountUI("NN,NNN,NNN,NNn.nn", Amount).toString());
                txtViewAmount.setText(amount_buff);

                // sidumili: Press animation
                wub_lib.ButtonPressed(c, btnKey0);
            }
        });
    }

    public void FuncKeyCancel() {
        btnKeyCancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                if(Amount.length() > 0){
                    getTimerRestart();
                    Amount = "";
                    amount_buff = txn_curr.concat("0.00");
                    txtViewAmount.setText(amount_buff);

                } else {
                    getTimerCancel();
                    //Toast.makeText(getApplicationContext(), "Cancelling...", Toast.LENGTH_SHORT).show();

                    final_amt_string = "CANCEL";

                    // sidumili: Press animation
                    wub_lib.ButtonPressed(c, btnKeyCancel);

                    //startActivity(new Intent(AmountEntryActivity.this, MainActivity.class));
                    AmountEntryActivity.this.finish();

                    Log.i("PATRICK", "Get Amount KeyBoard buCancel");
                    // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                    synchronized (MainActivity.LOCK) {
                        MainActivity.LOCK.setCondition(true);
                        MainActivity.LOCK.notifyAll();
                    }


                }

            }
        });
    }

    public void FuncKeyClear() {
        btnKeyClear.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                getTimerRestart();

                if(Amount.length() > 0) {
                    Amount = Amount.substring(0, Amount.length()-1);
                }
                //txtViewAmount.setText(tools.getFormatAmount(Amount).toString());
                //txtViewAmount.setText(Amount);
                amount_buff = txn_curr.concat(vdCTOSS_FormatAmountUI("NN,NNN,NNN,NNn.nn", Amount).toString());
                txtViewAmount.setText(amount_buff);

                // sidumili: Press animation
                wub_lib.ButtonPressed(c, btnKeyClear);
            }
        });
    }

    private long mLastClickTime = 0;
	
    public void FuncKeyEnter() {
        btnKeyEnter.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                /*cancel timer first*/
                getTimerCancel();

                if (SystemClock.elapsedRealtime() - mLastClickTime < 1000){
                    return;
                }
                mLastClickTime = SystemClock.elapsedRealtime();

                //dAmount = Double.parseDouble(etAmount.getText().toString());
                //final_amt_string = etAmount.getText().toString();
                //final_amt_string="12345678";

                if(Amount.length() <= 0) {
                    //Toast.makeText(getApplicationContext(), "Please Enter Amount", Toast.LENGTH_SHORT).show();
                    //return;
                    if(isTIP.equals("Tip"))
                    {
                        Amount = "0";
                    }
                    else
                    {
                        Toast.makeText(getApplicationContext(), "Please Enter Amount", Toast.LENGTH_SHORT).show();
                        getTimerRestart();
                        return;
                    }
                }
                else if(Amount.length() < amountminlength) {
                    Toast.makeText(getApplicationContext(), "Minimum length is " + amountminlength, Toast.LENGTH_SHORT).show();
                    return;
                }

                final_amt_string=Amount;

                // sidumili: Press animation
                wub_lib.ButtonPressed(c, btnKeyEnter);

                //final_amt_string = final_amt_string.replaceAll("[$,.]", "");

                //startActivity(new Intent(AmountEntryActivity.this,MainActivity.class));
                //use this code for S1F
                AmountEntryActivity.this.finish();
                //use for S1F2
                //new Handler().postDelayed(new Runnable() {
                //@Override
                //    public void run() {
                //         AmountEntryActivity.this.finish();
                //    }
                //}, 100);   
                //end-s1f2
                
                Log.i("Tine", "Get Amount KeyBoard buOK");

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
            final_amt_string = "TIME OUT";

            //startActivity(new Intent(AmountEntryActivity.this, MainActivity.class));

            Log.i("PATRICK", "Timeout UserInputString");
            AmountEntryActivity.this.finish();

            // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
            synchronized (MainActivity.LOCK) {
                MainActivity.LOCK.setCondition(true);
                MainActivity.LOCK.notifyAll();
            }
        }
    };

    @Override
    public void onBackPressed() {
        getTimerCancel();

        final_amt_string = "CANCEL";
        //startActivity(new Intent(AmountEntryActivity.this, MainActivity.class));
        AmountEntryActivity.this.finish();

        Log.i("PATRICK", "Get Amount KeyBoard buCancel");
        // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
        synchronized (MainActivity.LOCK) {
            MainActivity.LOCK.setCondition(true);
            MainActivity.LOCK.notifyAll();
        }

    }

    @Override
    protected void onDestroy() {
        this.mWakeLock.release();
        getTimerCancel();

        final_amt_string = null;
        aHandler = null;
        mContext = null;
        super.onDestroy();
        SysApplication.getInstance().removeActivity(this);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String vdCTOSS_FormatAmountUI(String szFmt, String szInAmt);
}
