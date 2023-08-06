package com.Source.S1_MCC.MCC.SignaturePad;

import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.media.ThumbnailUtils;
import android.os.Bundle;
import android.os.PowerManager;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.Source.S1_MCC.MCC.CloseActivityClass;
import com.Source.S1_MCC.MCC.Main.MainActivity;
import com.Source.S1_MCC.MCC.R;
import com.Source.S1_MCC.MCC.SysApplication;
import com.Source.S1_MCC.MCC.model.DemoAppActivity;
import com.github.gcacace.signaturepad.monochrome.BitmapConvertor;
import com.github.gcacace.signaturepad.views.SignaturePad;

import static android.content.ContentValues.TAG;

public class SignaturePadCastles extends DemoAppActivity {

    private SignaturePad mSignaturePad;
    private Button mClearButton;
    private Button mSaveButton;

    protected PowerManager.WakeLock mWakeLock;

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        CloseActivityClass.activityList.add(this);
        SysApplication.getInstance().addActivity(this);

        super.onCreate(savedInstanceState);
        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);
        setContentView(R.layout.activity_signaturepad);

        /* This code together with the one in onDestroy()
         * will make the screen be always on until this Activity gets destroyed. */
        final PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        this.mWakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "S1_MCC:AAA-SignaturePadCastles>>WAKE_LOCK");
        this.mWakeLock.acquire();

		Intent intent = getIntent();
		String dispmsg = intent.getStringExtra("pass_in_string");

        Log.i(TAG, dispmsg);

        // Display Amount in Signature
        TextView text_msg;
        TextView detail_msg_01;

        String[] dispmsginfo = dispmsg.split("\\|");
        int msgcnt = dispmsginfo.length;

        // sidumili: For display amount in signature
        for (int inIdx = 0; inIdx < msgcnt; inIdx++) {
            System.out.println("split msg [" + inIdx + "][" + dispmsginfo[inIdx] + "]");
            switch (inIdx) {
                case 0: // TransType
                    // Do nothing...
                    break;
                case 1: // Message1
                    text_msg = (TextView) findViewById(R.id.msg_text_01);
                    text_msg.setText(dispmsginfo[inIdx]);
                    break;
                case 2: // Message2
                    detail_msg_01 = (TextView) findViewById(R.id.msg_detail_01);
                    detail_msg_01.setText(dispmsginfo[inIdx]);
                    break;
            }
        }

        mSignaturePad = (SignaturePad) findViewById(R.id.signature_pad);
        mSignaturePad.setOnSignedListener(new SignaturePad.OnSignedListener() {
            @Override
            public void onStartSigning() {
                //Toast.makeText(getApplicationContext(), "OnStartSigning", Toast.LENGTH_SHORT).show();
                Log.i("TINE", "OnStartSigning");
            }

            @Override
            public void onSigned() {
                mSaveButton.setEnabled(true);
                mClearButton.setEnabled(true);
            }

            @Override
            public void onClear() {
                mSaveButton.setEnabled(false);
                mClearButton.setEnabled(false);
            }
        });

        mClearButton = (Button) findViewById(R.id.clear_button);
        mSaveButton = (Button) findViewById(R.id.save_button);

        mClearButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                mSignaturePad.clear();
            }
        });

        mSaveButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Bitmap signatureBitmap = mSignaturePad.getSignatureBitmap();

                //Bitmap resizeBmp = ThumbnailUtils.extractThumbnail(signatureBitmap, 380, 460);
                Bitmap resizeBmp = ThumbnailUtils.extractThumbnail(signatureBitmap, 380, 200);

                BitmapConvertor convertor = new BitmapConvertor(SignaturePadCastles.this);
                //convertor.convertBitmap(signatureBitmap, "/data/data/pub/signtest.bmp");
                convertor.convertBitmap(resizeBmp, "/data/data/com.Source.S1_MCC.MCC/signtest.bmp");

                //startActivity(new Intent(SignaturePadCastles.this, MainActivity.class));
                finish();

                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();

                }
            }
        });
    }

    @Override
    protected void onDestroy() {
        this.mWakeLock.release();
        super.onDestroy();
        SysApplication.getInstance().removeActivity(this);
    }

}
