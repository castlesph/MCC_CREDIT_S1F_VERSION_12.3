package com.Source.S1_MCC.MCC;

import android.content.Context;
import android.content.Intent;
import android.os.PowerManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;

import com.Source.S1_MCC.MCC.Main.MainActivity;
import com.Source.S1_MCC.MCC.db.AndroidDatabaseManager;


public class EditDatabaseActivity extends AppCompatActivity implements View.OnClickListener{
    Button btnterminal;
    Button btnwave;
    Button btnemv;
    Button btnexit;
    Button btnenv;
    Button btnmccpay;
	Button btnpost;
	
    private static final String TAG = EditDatabaseActivity.class.getSimpleName();
    private String TerminalDb = "/data/data/pub/TERMINAL.S3DB";
    private String WaveDb = "/data/data/pub/WAVE.S3DB";
    private String EmvDb = "/data/data/pub/EMV.S3DB";
    private String EnvDb = "/data/data/pub/ENV.S3DB";
    private String MCCPayDb = "/data/data/pub/MCCPAY.S3DB";
	private String PostDb = "/data/data/pub/POST.S3DB";
	
    public static String final_string;

    protected PowerManager.WakeLock mWakeLock;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

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

        setContentView(R.layout.activity_edit_database);

        /* This code together with the one in onDestroy()
         * will make the screen be always on until this Activity gets destroyed. */
        final PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        this.mWakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "S1_MCC:EditDatabaseActivity>>WAKE_LOCK");
        this.mWakeLock.acquire();

        MainActivity.fSTART = true;

        btnterminal = (Button) findViewById(R.id.btn_edit_terminal);
        btnwave = (Button) findViewById(R.id.btn_edit_wave);
        btnemv = (Button) findViewById(R.id.btn_edit_emv);
        btnexit = (Button) findViewById(R.id.btn_exit);
        btnenv = (Button) findViewById(R.id.btn_edit_env);
        btnmccpay = (Button) findViewById(R.id.btn_edit_mccpay); 
		btnpost = (Button) findViewById(R.id.btn_edit_post); 
		
        btnterminal.setOnClickListener(this);
        btnwave.setOnClickListener(this);
        btnemv.setOnClickListener(this);
        btnexit.setOnClickListener(this);
        btnenv.setOnClickListener(this);
		btnmccpay.setOnClickListener(this);
        btnpost.setOnClickListener(this);

    }

    @Override
    public void onClick(View view) {
        Intent intent = new Intent();
        Bundle bundle = new Bundle();

        switch (view.getId())
        {
            case R.id.btn_edit_terminal:
                Log.d(TAG, "btn_edit_terminal");
                intent.setClass(EditDatabaseActivity.this, AndroidDatabaseManager.class);
                bundle.putString("dbname", TerminalDb);
                intent.putExtras(bundle);
                startActivity(intent);

                final_string = "";

                EditDatabaseActivity.this.finish();
                break;
            case R.id.btn_edit_wave:
                Log.d(TAG, "btn_edit_wave");
                intent.setClass(EditDatabaseActivity.this, AndroidDatabaseManager.class);
                bundle.putString("dbname", WaveDb);
                intent.putExtras(bundle);
                startActivity(intent);

                final_string = "";

                EditDatabaseActivity.this.finish();
                break;
            case R.id.btn_edit_emv:
                Log.d(TAG, "btn_edit_emv");
                intent.setClass(EditDatabaseActivity.this, AndroidDatabaseManager.class);
                bundle.putString("dbname", EmvDb);
                intent.putExtras(bundle);
                startActivity(intent);

                final_string = "";

                EditDatabaseActivity.this.finish();
                break;
				
            case R.id.btn_edit_env:
                Log.d(TAG, "btn_edit_env");
                intent.setClass(EditDatabaseActivity.this, AndroidDatabaseManager.class);
                bundle.putString("dbname", EnvDb);
                intent.putExtras(bundle);
                startActivity(intent);

                final_string = "";

                EditDatabaseActivity.this.finish();
                break;

            case R.id.btn_edit_mccpay:
                Log.d(TAG, "btn_edit_mccpay");
                intent.setClass(EditDatabaseActivity.this, AndroidDatabaseManager.class);
                bundle.putString("dbname", MCCPayDb);
                intent.putExtras(bundle);
                startActivity(intent);

                final_string = "";

                EditDatabaseActivity.this.finish();
                break;

            case R.id.btn_edit_post:
                Log.d(TAG, "btn_edit_post");
                intent.setClass(EditDatabaseActivity.this, AndroidDatabaseManager.class);
                bundle.putString("dbname", PostDb);
                intent.putExtras(bundle);
                startActivity(intent);

                final_string = "";

                EditDatabaseActivity.this.finish();
                break;
				
            case R.id.btn_exit:
                Log.d(TAG, "btn_exit");

                final_string = "";

                EditDatabaseActivity.this.finish();

                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();

                }
                //intent.setClass(EditDatabaseActivity.this, MainActivity.class);
                //startActivity(intent);
                //startActivity(new Intent(EditDatabaseActivity.this,MainActivity.class));
                break;

        }
        Log.i("EditDB", "===END===");

    }

    @Override
    protected void onDestroy() {
        this.mWakeLock.release();
        super.onDestroy();
        SysApplication.getInstance().removeActivity(this);
    }
}
