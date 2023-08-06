package com.Source.S1_MCC.MCC.model;

import android.accessibilityservice.AccessibilityService;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.inputmethodservice.Keyboard;
import android.media.tv.TvView;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.Handler;
import android.os.PowerManager;
import android.support.annotation.NonNull;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewTreeObserver;
import android.view.WindowManager;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.Source.S1_MCC.MCC.CloseActivityClass;
import com.Source.S1_MCC.MCC.Main.MainActivity;
import com.Source.S1_MCC.MCC.R;
import com.Source.S1_MCC.MCC.SysApplication;
import com.Source.S1_MCC.MCC.Trans.InputAlpha;
import com.Source.S1_MCC.MCC.wub_lib;

import CTOS.CtSystem;
import CTOS.CtSystemException;

import static android.accessibilityservice.AccessibilityService.SHOW_MODE_HIDDEN;

public class QwertyUI extends DemoAppActivity {


    Button btnKeyEnter,btnKeyCancel,btnKeyClear;
    TextView textView_name, textView_msg;
    EditText editText_entry;
    LinearLayout layout_rootview;
    String userentry="", base="", inputchar="";

    boolean fNewChar=false,fNextSlot=false;
    boolean status = false;
    String szLastChar="", szNewChar=""; //1,2,3,..0

    public static String final_string;
    int invmaxlength=0, invminlength=0, typedef=0, count=0;



    private int inTimeOut = 0;
    String display, maxlength, minlength, type;

    CountDownTimer timer = null;

    final Context c = this;

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
                        |View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        //CtSystem system = new CtSystem();
        //system.setNavBarIcon(false,true,false);


        setContentView(R.layout.input_qwerty);

        /* This code together with the one in onDestroy()
         * will make the screen be always on until this Activity gets destroyed. */
        final PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        this.mWakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "S1_MCC:AAA-QwertyUI>>WAKE_LOCK");
        this.mWakeLock.acquire();

        layout_rootview = (LinearLayout) findViewById(R.id.qwerty_root_view);

        btnKeyCancel = (Button) findViewById(R.id.btn_can);
        btnKeyClear = (Button) findViewById(R.id.btn_clr);
        btnKeyEnter = (Button) findViewById(R.id.btn_ok);

        editText_entry = (EditText) findViewById(R.id.editText_1);

        textView_name = (TextView) findViewById(R.id.textView_name);
        textView_msg = (TextView) findViewById(R.id.textView_msg);

        Intent intent = getIntent();
        String value = intent.getStringExtra("pass_in_string");
        Log.i("InputAlpha::value", value);

        String[] dispmsginfo = value.split("\\|");
        int msgcnt = dispmsginfo.length;

        System.out.println("msgcnt [" + msgcnt + "]");
        //String[] dispmsginfo = dispmsg.split("\\|");
        //int msgcnt = dispmsginfo.length;

        //System.out.println("msgcnt [" + msgcnt + "]");

        for (int inIdx = 0; inIdx < msgcnt; inIdx++) {
            System.out.println("InputAlpha::split msg [" + inIdx + "][" + dispmsginfo[inIdx] + "]");
            switch (inIdx) {
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

        FuncKeyCancel();
        FuncKeyClear();
        FuncKeyEnter();

        KeyboardStatus();


        editText_entry.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            public boolean onEditorAction(TextView v, int actionId, KeyEvent event) {
                if (actionId == EditorInfo.IME_ACTION_DONE) {
                    Log.i("QwertyUI", "Action Done");

                    userentry = editText_entry.getText().toString();

                    if (userentry.length() != invminlength) {
                        //Toast.makeText(getApplicationContext(), "Minimum length must be " + String.valueOf(invminlength), Toast.LENGTH_SHORT).show();
                        invalidInput();
                    } else {

                        /*cancel timer first*/
                        getTimerCancel();

                        final_string = userentry;
                        QwertyUI.this.finish();

                        Log.i("Tine", "InputAlpha KeyBoard buOK");

                        // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                        synchronized (MainActivity.LOCK) {
                            MainActivity.LOCK.setCondition(true);
                            MainActivity.LOCK.notifyAll();


                        }
                    }

                }

                return false;
            }
        });


    }

    public void KeyboardStatus() {
        layout_rootview.getViewTreeObserver().addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {
            @Override
            public void onGlobalLayout() {
                int heightDiff = layout_rootview.getRootView().getHeight() - layout_rootview.getHeight();
                Log.i("Layout", "heightDiff = " + heightDiff);

                if (heightDiff > 600) {
                    Log.i("MyActivity", "keyboard opened");
                    status = true;
                } else {
                    Log.i("MyActivity", "keyboard closed");
                    status = false;

                    getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);    //Tine:  Show Status Bar
                    View decorView = getWindow().getDecorView();
                    decorView.setSystemUiVisibility(
                            View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                                    |View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                                    | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);

                    getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
                }
            }
        });
    }


    public void FuncKeyCancel(){
        btnKeyCancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                //wub_lib.ButtonPressed(c, btnKeyCancel); // sidumili: Press animation

                /*cancel timer first*/
                getTimerCancel();

                final_string = "CANCEL";
                //startActivity(new Intent(InputAlpha.this, MainActivity.class));
                QwertyUI.this.finish();

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

                userentry = editText_entry.getText().toString();

                if(userentry.length() > 0) {
                    userentry = userentry.substring(0, userentry.length()-1);
                    editText_entry.setText(userentry);
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
                userentry = editText_entry.getText().toString();

                if (userentry.length() != invminlength) {
                    //Toast.makeText(getApplicationContext(), "Minimum length must be " + String.valueOf(invminlength), Toast.LENGTH_SHORT).show();
                    invalidInput();
                } else {

                    /*cancel timer first*/
                    getTimerCancel();

                    final_string = userentry;
                    QwertyUI.this.finish();

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
                QwertyUI.this.finish();

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
        super.onBackPressed();
        getTimerCancel();

        final_string = "CANCEL";

        //startActivity(new Intent(QwertyUI.this, MainActivity.class));
        QwertyUI.this.finish();

        Log.i("PATRICK", "Back");

        // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
        synchronized (MainActivity.LOCK) {
            MainActivity.LOCK.setCondition(true);
            MainActivity.LOCK.notifyAll();
        }

    }

    @Override
    protected void onResume() {
        super.onResume();
        getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);    //Tine:  Show Status Bar
        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                        |View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);

        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
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
