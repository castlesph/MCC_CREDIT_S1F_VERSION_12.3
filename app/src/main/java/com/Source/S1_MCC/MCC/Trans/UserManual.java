package com.Source.S1_MCC.MCC.Trans;

import android.content.Context;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.support.annotation.Nullable;
import android.support.annotation.RequiresApi;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.TextClock;
import android.widget.TextView;

import com.Source.S1_MCC.MCC.CloseActivityClass;
import com.Source.S1_MCC.MCC.Main.MainActivity;
import com.Source.S1_MCC.MCC.R;
import com.Source.S1_MCC.MCC.SysApplication;
import com.Source.S1_MCC.MCC.model.DemoAppActivity;
import com.github.barteksc.pdfviewer.PDFView;
import com.github.barteksc.pdfviewer.util.Constants;
import com.github.barteksc.pdfviewer.util.FitPolicy;

import static com.Source.S1_MCC.MCC.Main.MainActivity.objSelected;
import static com.Source.S1_MCC.MCC.Main.MultiAP_SubAP.TAG;

// sidumili:
public class UserManual extends DemoAppActivity {

    final Context c = this;
    public static String final_string;

    public static int inTimeOut = 0;

    RecyclerView recyclerView;
    Button btn_can;
    TextView tv_title;
    TextView tv_title_header;

    String final_selected_string;

    PDFView pdfView;
    Constants constants;

    @RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN)
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        Log.i(TAG, "UserManual onCreate: run");
        CloseActivityClass.activityList.add(this);
        SysApplication.getInstance().addActivity(this);
        super.onCreate(savedInstanceState);
        Bundle bundle = getIntent().getExtras();

        overridePendingTransition(0, 0); // disable the animation, faster

        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON); // Set always ON screen

        setContentView(R.layout.activity_usermanual);

        Intent intent=getIntent();
        String dispmsg=intent.getStringExtra("pass_in_string");
        Log.i("saturn dispmsg", dispmsg);

        btn_can = (Button) findViewById(R.id.btn_can);
        tv_title = (TextView) findViewById(R.id.textView_txn);
        //tv_title_header = (TextView) findViewById(R.id.tv_title_header);

        String[] pass_user_str = dispmsg.split("\\|");
        String sFileName = "";

        for(int indx = 0; indx < pass_user_str.length; indx++)
        {
            System.out.println("UserManual, split msg [" + indx + "][" + pass_user_str[indx] + "]");

            switch (indx)
            {
                case 0:
                    tv_title.setText( pass_user_str[indx]);
                    break;
                case 1:
                    //tv_title_header.setText( pass_user_str[indx]);
                    sFileName = pass_user_str[indx];
                    break;
            }
        }

        Log.i(TAG, "sFileName="+sFileName);

        /*Start timer*/
        //getTimerRestart();

        // View PDF (User Manual) -- sidumili
        pdfView = (PDFView) findViewById(R.id.pdfview);
        pdfView.fromAsset(sFileName)
                .defaultPage(1)
                .enableAnnotationRendering(true)
                .enableAntialiasing(false)
                .enableDoubletap(true)
                .enableSwipe(true)
                .spacing(10)
                .pageFitPolicy(FitPolicy.BOTH)
                .swipeHorizontal(false)
                .autoSpacing(false)
                .pageSnap(false)
                .pageFling(false)
                .load();

        btn_can.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                /*cancel timer first*/
                //getTimerCancel();

                final_string = "0|000000|CANCEL";

                objSelected.setCancelled(true);
                objSelected.setCompleted(false);
                objSelected.setContinue(false);

                UserManual.this.finish();

                Log.i("TINE", "CANCEL BUTTON");

                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();

                }
            }
        });

    }

    public static class Lock {
        private boolean condition;

        public boolean conditionMet() {
            return condition;
        }

        public void setCondition(boolean condition) {
            this.condition = condition;


        }

    }

    public static UserManual.Lock LOCK = new UserManual.Lock();

    public void getTimerCancel() {
        timer.cancel();
    }

    public void getTimerRestart()
    {
        timer.start();
    }

    private CountDownTimer timer = new CountDownTimer(inTimeOut*1000, 1000) {

        @Override
        public void onTick(long millisUntilFinished) {
            Log.i("OrderPayment Timer", "Timer onTick");
        }

        @Override
        public void onFinish() {
            Log.i("Timer", "OrderPayment Timer onFinish");

            //etInputStr.clear();
            final_string = "0|000000|TIME OUT";

            Log.i("PATRICK", "Timeout OrderPayment");
            UserManual.this.finish();

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
    protected void onResume() {
        super.onResume();

        getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);    //Tine:  Show Status Bar
        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY |View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        final_string = null;
        final_selected_string = null;

        pdfView = null;
        SysApplication.getInstance().removeActivity(this);
    }

}
