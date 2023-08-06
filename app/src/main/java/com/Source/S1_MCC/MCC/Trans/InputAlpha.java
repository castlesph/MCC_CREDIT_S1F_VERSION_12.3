package com.Source.S1_MCC.MCC.Trans;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.PowerManager;
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

public class InputAlpha extends DemoAppActivity {


    Button btnKey1, btnKey2, btnKey3,btnKey4,btnKey5,btnKey6,btnKey7,btnKey8,btnKey9,
            btnKey0,btnKeyEnter,btnKeyCancel,btnKeyClear;
    TextView textView_value, textView_name, textView_msg;
    String userentry="", base="", inputchar="";

    boolean fNewChar=false,fNextSlot=false;
    String szLastChar="", szNewChar=""; //1,2,3,..0

    public static String final_string;
    int invmaxlength=0, invminlength=0, typedef=0, count=0;

    protected PowerManager.WakeLock mWakeLock;


    private int inTimeOut = 0;
    String display, maxlength, minlength, type;

    CountDownTimer timer = null;

    final Context c = this;

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

        setContentView(R.layout.activity_key_alpha);

        /* This code together with the one in onDestroy()
         * will make the screen be always on until this Activity gets destroyed. */
        final PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        this.mWakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "S1_MCC:AAA-InputAlpha>>WAKE_LOCK");
        this.mWakeLock.acquire();

        btnKey1=(Button) findViewById(R.id.btn_qz);
        btnKey2=(Button) findViewById(R.id.btn_abc);
        btnKey3=(Button) findViewById(R.id.btn_def);
        btnKey4=(Button) findViewById(R.id.btn_ghi);
        btnKey5=(Button) findViewById(R.id.btn_jkl);
        btnKey6=(Button) findViewById(R.id.btn_mno);
        btnKey7=(Button) findViewById(R.id.btn_prs);
        btnKey8=(Button) findViewById(R.id.btn_tuv);
        btnKey9=(Button) findViewById(R.id.btn_wxy);
        btnKey0=(Button) findViewById(R.id.btn_00);
        btnKeyCancel=(Button) findViewById(R.id.btn_can);
        btnKeyClear=(Button) findViewById(R.id.btn_clr);
        btnKeyEnter=(Button) findViewById(R.id.btn_ok);

        textView_value=(TextView) findViewById(R.id.textView_value);
        textView_value.setText("");
        textView_name = (TextView) findViewById(R.id.textView_name);
        textView_msg = (TextView) findViewById(R.id.textView_msg);

        Intent intent=getIntent();
        String value=intent.getStringExtra("pass_in_string");
        Log.i("InputAlpha::value", value);

        String[] dispmsginfo = value.split("\\|");
        int msgcnt = dispmsginfo.length;

        System.out.println("msgcnt [" + msgcnt + "]");
        //String[] dispmsginfo = dispmsg.split("\\|");
        //int msgcnt = dispmsginfo.length;

        //System.out.println("msgcnt [" + msgcnt + "]");

        for (int inIdx = 0 ; inIdx < msgcnt; inIdx++)
        {
            System.out.println("InputAlpha::split msg [" + inIdx + "][" + dispmsginfo[inIdx] + "]");
            switch (inIdx)
            {
                case 0:
                    invminlength = Integer.valueOf(dispmsginfo[inIdx]);
                    break;
                case 1:
                    textView_name.setText(dispmsginfo[inIdx]);
                    break;
                case 2:
                    textView_msg.setText(dispmsginfo[inIdx]);
                    break;
            }
        }
        //typedef = Integer.valueOf(type.toString());

        //Toast.makeText(getApplicationContext(), "display: "+display, Toast.LENGTH_SHORT).show();
        //Toast.makeText(getApplicationContext(), "minlength: "+minlength, Toast.LENGTH_SHORT).show();
        //Toast.makeText(getApplicationContext(), "maxlength: "+maxlength, Toast.LENGTH_SHORT).show();
        //Toast.makeText(getApplicationContext(), "type: "+type, Toast.LENGTH_SHORT).show();

        Log.i("pass in value", value);

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

                wub_lib.ButtonPressed(c, btnKey1); // sidumili: Press animation
				
                //timer.start();
                fNextSlot=false;
                base = "1QZ";
                inputchar="";
                szNewChar="1";
                if(szLastChar.equals(szNewChar)) {
					
                    count++;
                    if(count>=base.length())
                        count=0;
                    userentry = userentry.substring(0, userentry.length()-1);
                }
                else {
                    count = 0;
                    //go to next array                    
                    fNextSlot=true;
					
					if (userentry.length() >= 6)
					return;
                }

                inputchar = base.substring(count,count+1);
                userentry = userentry + inputchar;
                //Toast.makeText(getApplicationContext(), "inputchar: "+inputchar, Toast.LENGTH_SHORT).show();

                textView_value.setText(userentry);
                szLastChar=szNewChar;
            }
        });
    }

    public void FuncKey2() {
        btnKey2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                wub_lib.ButtonPressed(c, btnKey2); // sidumili: Press animation

                //timer.start();
                fNextSlot=false;
                base = "2ABC";
                inputchar="";
                szNewChar="2";
                if(szLastChar.equals(szNewChar)) {
                    count++;
                    if(count>=base.length())
                        count=0;
                    userentry = userentry.substring(0, userentry.length()-1);
                }
                else {
                    count = 0;
                    //go to next array
                    fNextSlot=true;
					
					if (userentry.length() >= 6)
					return;
                }
                inputchar = base.substring(count,count+1);
                userentry = userentry + inputchar;
                textView_value.setText(userentry);
                szLastChar=szNewChar;
            }
        });
    }

    public void FuncKey3() {
        btnKey3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                wub_lib.ButtonPressed(c, btnKey3); // sidumili: Press animation

                //timer.start();
                fNextSlot=false;
                base = "3DEF";
                inputchar="";
                szNewChar="3";
                if(szLastChar.equals(szNewChar)) {
                    count++;
                    if(count>=base.length())
                        count=0;
                    userentry = userentry.substring(0, userentry.length()-1);
                }
                else {
                    count = 0;
                    //go to next array
                    fNextSlot=true;
					
					if (userentry.length() >= 6)
					return;
                }
                inputchar = base.substring(count,count+1);
                userentry = userentry + inputchar;
                textView_value.setText(userentry);
                szLastChar=szNewChar;
            }
        });
    }

    public void FuncKey4() {
        btnKey4.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                wub_lib.ButtonPressed(c, btnKey4); // sidumili: Press animation

                //timer.start();
                fNextSlot=false;
                base = "4GHI";
                inputchar="";
                szNewChar="4";
                if(szLastChar.equals(szNewChar)) {
                    count++;
                    if(count>=base.length())
                        count=0;
                    userentry = userentry.substring(0, userentry.length()-1);
                }
                else {
                    count = 0;
                    //go to next array
                    fNextSlot=true;
					
					if (userentry.length() >= 6)
					return;
                }
                inputchar = base.substring(count,count+1);
                userentry = userentry + inputchar;
                textView_value.setText(userentry);
                szLastChar=szNewChar;
            }
        });
    }

    public void FuncKey5() {
        btnKey5.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                wub_lib.ButtonPressed(c, btnKey5); // sidumili: Press animation

                //timer.start();
                fNextSlot=false;
                base = "5JKL";
                inputchar="";
                szNewChar="5";
                if(szLastChar.equals(szNewChar)) {
                    count++;
                    if(count>=base.length())
                        count=0;
                    userentry = userentry.substring(0, userentry.length()-1);
                }
                else {
                    count = 0;
                    //go to next array
                    fNextSlot=true;
					
					if (userentry.length() >= 6)
					return;
                }
                inputchar = base.substring(count,count+1);
                userentry = userentry + inputchar;
                textView_value.setText(userentry);
                szLastChar=szNewChar;
            }
        });
    }

    public void FuncKey6() {
        btnKey6.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                wub_lib.ButtonPressed(c, btnKey6); // sidumili: Press animation

                //timer.start();
                fNextSlot=false;
                base = "6MNO";
                inputchar="";
                szNewChar="6";
                if(szLastChar.equals(szNewChar)) {
                    count++;
                    if(count>=base.length())
                        count=0;
                    userentry = userentry.substring(0, userentry.length()-1);
                }
                else {
                    count = 0;
                    //go to next array
                    fNextSlot=true;
					
					if (userentry.length() >= 6)
					return;
                }
                inputchar = base.substring(count,count+1);
                userentry = userentry + inputchar;
                textView_value.setText(userentry);
                szLastChar=szNewChar;
            }
        });
    }

    public void FuncKey7() {
        btnKey7.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                wub_lib.ButtonPressed(c, btnKey7); // sidumili: Press animation

                //timer.start();
                fNextSlot=false;
                base = "7PRS";
                inputchar="";
                szNewChar="7";
                if(szLastChar.equals(szNewChar)) {
                    count++;
                    if(count>=base.length())
                        count=0;
                    userentry = userentry.substring(0, userentry.length()-1);
                }
                else {
                    count = 0;
                    //go to next array
                    fNextSlot=true;
					
					if (userentry.length() >= 6)
					return;
                }
                inputchar = base.substring(count,count+1);
                userentry = userentry + inputchar;
                textView_value.setText(userentry);
                szLastChar=szNewChar;
            }
        });
    }

    public void FuncKey8() {
        btnKey8.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                wub_lib.ButtonPressed(c, btnKey8); // sidumili: Press animation

                //timer.start();
                fNextSlot=false;
                base = "8TUV";
                inputchar="";
                szNewChar="8";
                if(szLastChar.equals(szNewChar)) {
                    count++;
                    if(count>=base.length())
                        count=0;
                    userentry = userentry.substring(0, userentry.length()-1);
                }
                else {
                    count = 0;
                    //go to next array
                    fNextSlot=true;
					
					if (userentry.length() >= 6)
					return;
                }
                inputchar = base.substring(count,count+1);
                userentry = userentry + inputchar;
                textView_value.setText(userentry);
                szLastChar=szNewChar;
            }
        });
    }

    public void FuncKey9() {
        btnKey9.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                wub_lib.ButtonPressed(c, btnKey9); // sidumili: Press animation

                //timer.start();
                fNextSlot=false;
                base = "9WXY";
                inputchar="";
                szNewChar="9";
                if(szLastChar.equals(szNewChar)) {
                    count++;
                    if(count>=base.length())
                        count=0;
                    userentry = userentry.substring(0, userentry.length()-1);
                }
                else {
                    count = 0;
                    //go to next array
                    fNextSlot=true;
					
					if (userentry.length() >= 6)
					return;
                }
                inputchar = base.substring(count,count+1);
                userentry = userentry + inputchar;
                textView_value.setText(userentry);
                szLastChar=szNewChar;
            }
        });
    }


    public void FuncKey0() {
        btnKey0.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                wub_lib.ButtonPressed(c, btnKey0); // sidumili: Press animation

                //timer.start();
                fNextSlot=false;
                base = "0";
                inputchar="";
                szNewChar="0";
                if(szLastChar.equals(szNewChar)) {
                    count++;
                    if(count>=base.length())
                        count=0;
                    //userentry = userentry.substring(0, userentry.length()-1);
                }
                else {
                    count = 0;
                    //go to next array
                    fNextSlot=true;

					if (userentry.length() >= 6)
					return;
                }

                if (userentry.length() >= 6)
                    return;
                
                inputchar = base.substring(count,count+1);
                userentry = userentry + inputchar;
                textView_value.setText(userentry);
                szLastChar=szNewChar;
            }
        });
    }

    public void FuncKeyCancel(){
        btnKeyCancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                wub_lib.ButtonPressed(c, btnKeyCancel); // sidumili: Press animation

                /*cancel timer first*/
                getTimerCancel();

                //if(userentry.length() > 0) {
                //    userentry = "";
                //    textView_value.setText(userentry);
                //    count=0;
                //    szLastChar="";
                //    szNewChar="";
                //    return;
                //}

                final_string = "CANCEL";
                //startActivity(new Intent(InputAlpha.this, MainActivity.class));
                InputAlpha.this.finish();

                Log.i("PATRICK", "Get Amount KeyBoard buCancel");

                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();

                }
            }
        });

    }

    public void FuncKeyClear() {
        btnKeyClear.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                wub_lib.ButtonPressed(c, btnKeyClear); // sidumili: Press animation

                if(userentry.length() > 0) {
                    userentry = userentry.substring(0, userentry.length()-1);
                    textView_value.setText(userentry);
                }
                count=0;
                szLastChar="";
                szNewChar="";
            }
        });
    }

    public void FuncKeyEnter() {
        btnKeyEnter.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                wub_lib.ButtonPressed(c, btnKeyEnter); // sidumili: Press animation

                Log.i("sidumili", "onClick: FuncKeyEnter, userentry="+userentry);
                /*cancel timer first*/
                //getTimerCancel();

                if (userentry.length() < invminlength) {
                    //Toast.makeText(getApplicationContext(), "Minimum length must be " + String.valueOf(invminlength), Toast.LENGTH_SHORT).show();
                    invalidInput();
                } else {

                    /*cancel timer first*/
                    getTimerCancel();

                    final_string = userentry;
                    InputAlpha.this.finish();

                    Log.i("Tine", "InputAlpha KeyBoard buOK");

                    // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                    synchronized (MainActivity.LOCK) {
                        MainActivity.LOCK.setCondition(true);
                        MainActivity.LOCK.notifyAll();


                    }
                }


            }

        });
    }

    private void invalidInput() {
        AlertDialog.Builder dialog=new AlertDialog.Builder(this);
        dialog.setMessage("Invalid input");
        //dialog.setTitle("Dialog Box");
        dialog.setPositiveButton("OK",
                new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog,
                                        int which) {
                        //Toast.makeText(getApplicationContext(),"Yes is clicked",Toast.LENGTH_LONG).show();

                    }
                });
        /*
        dialog.setNegativeButton("cancel",new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                Toast.makeText(getApplicationContext(),"cancel is clicked",Toast.LENGTH_LONG).show();
            }
        });
        */
        AlertDialog alertDialog=dialog.create();

		alertDialog.getWindow().setFlags(WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
             WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE);
		
        alertDialog.show();
    }

    /**
     * 取消倒计时
     */
    public void getTimerCancel() {
        if(timer != null)
            timer.cancel();
    }

    /**
     * 开始倒计时
     */
    public void getTimerRestart(int cTimeOut)
    {
        Log.i("ctimer", "Timer set at: " + cTimeOut);
        timer = new CountDownTimer(cTimeOut*1000, 1000) {

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
                InputAlpha.this.finish();

                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();
                }
            }
        }.start();

    }

    @Override
    public void onBackPressed() {
        getTimerCancel();

        final_string = "TO";

        startActivity(new Intent(InputAlpha.this, MainActivity.class));
        InputAlpha.this.finish();

        Log.i("PATRICK", "Back");

        // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
        synchronized (MainActivity.LOCK) {
            MainActivity.LOCK.setCondition(true);
            MainActivity.LOCK.notifyAll();
        }

    }

    @Override
    protected void onDestroy() {
        this.mWakeLock.release();
        super.onDestroy();
        final_string = null;
        timer = null;
        SysApplication.getInstance().removeActivity(this);
    }

}
