package com.Source.S1_MCC.MCC.Main;

import android.Manifest;
import android.animation.Animator;
import android.animation.AnimatorListenerAdapter;
import android.app.Activity;
import android.app.ActivityManager;
import android.app.AlertDialog;
import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import android.graphics.PorterDuffXfermode;
import android.graphics.Rect;
import android.graphics.drawable.AnimationDrawable;
import android.graphics.drawable.BitmapDrawable;
//import android.icu.text.SimpleDateFormat;
import android.graphics.drawable.Icon;
import android.net.ConnectivityManager;
import android.net.Network;
import android.net.NetworkCapabilities;
import android.net.NetworkInfo;
import android.net.NetworkRequest;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.Handler;
import android.os.Message;
import android.os.Process;
import android.preference.PreferenceManager;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.annotation.RequiresApi;
import android.support.v4.app.ActivityCompat;
import android.support.v4.app.NotificationCompat;
import android.support.v4.app.NotificationManagerCompat;
import android.support.v4.view.ViewPager;
import android.text.InputType;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.view.animation.LinearInterpolator;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.GridView;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.Source.S1_MCC.MCC.DMenu.Model;
import com.Source.S1_MCC.MCC.DMenu.GridViewAdapter;
import com.Source.S1_MCC.MCC.DMenu.ViewPagerAdapter;
import com.Source.S1_MCC.MCC.EcrCommandDefintion;
import com.Source.S1_MCC.MCC.EcrCommandReceiver;
import com.Source.S1_MCC.MCC.EditDatabaseActivity;
import com.Source.S1_MCC.MCC.MccApplication;
import com.Source.S1_MCC.MCC.OperatorHolder;
import com.Source.S1_MCC.MCC.SignaturePad.SignaturePadCastles;
import com.Source.S1_MCC.MCC.SysApplication;
import com.Source.S1_MCC.MCC.Trans.AmountEntryActivity;
import com.Source.S1_MCC.MCC.Trans.BatchReview;
import com.Source.S1_MCC.MCC.Trans.CardEntry;
import com.Source.S1_MCC.MCC.Trans.ClearScreen;
import com.Source.S1_MCC.MCC.Trans.CopyMenu;
import com.Source.S1_MCC.MCC.Trans.DCCFxRateUI;
import com.Source.S1_MCC.MCC.Trans.DCCFxRateUI2;
import com.Source.S1_MCC.MCC.Trans.EditListView;
import com.Source.S1_MCC.MCC.Trans.EditAdapter;
import com.Source.S1_MCC.MCC.Trans.GetCVVUI;
import com.Source.S1_MCC.MCC.Trans.GetCardOptionsActivity;
import com.Source.S1_MCC.MCC.Trans.GetExpiryDateUI;
import com.Source.S1_MCC.MCC.Trans.IdleMenuTransactionType;
import com.Source.S1_MCC.MCC.Trans.IdleMenuTransactionType2;
import com.Source.S1_MCC.MCC.Trans.InputAlpha;
import com.Source.S1_MCC.MCC.Trans.MenuTransactionType;
import com.Source.S1_MCC.MCC.Trans.MenuTransactionType2;


import com.Source.S1_MCC.MCC.R;
import com.Source.S1_MCC.MCC.Trans.S1InputString;
import com.Source.S1_MCC.MCC.Trans.Transactions;
import com.Source.S1_MCC.MCC.Trans.UserCancelUI;
import com.Source.S1_MCC.MCC.Trans.UserConfirmCard;
import com.Source.S1_MCC.MCC.Trans.UserConfirmVoid;
import com.Source.S1_MCC.MCC.Trans.UserManual;
import com.Source.S1_MCC.MCC.model.ConfirmMenu;
import com.Source.S1_MCC.MCC.model.ConfirmMenu2;
import com.Source.S1_MCC.MCC.model.ConfirmMenu3;
import com.Source.S1_MCC.MCC.model.ConfirmMenuTip;
import com.Source.S1_MCC.MCC.model.ConfirmOKMenu;
import com.Source.S1_MCC.MCC.model.ConfirmReboot;
import com.Source.S1_MCC.MCC.model.DOptionMenu;
import com.Source.S1_MCC.MCC.model.DPopupMenuActivity;
import com.Source.S1_MCC.MCC.model.DemoAppActivity;
import com.Source.S1_MCC.MCC.model.EliteReceiptonScreen;
import com.Source.S1_MCC.MCC.model.NormalKeyBoard;
import com.Source.S1_MCC.MCC.model.DPopupMenu;
import com.Source.S1_MCC.MCC.model.PrintFirstReceiptonScreen;
import com.Source.S1_MCC.MCC.model.PrintReceiptonScreen;
import com.Source.S1_MCC.MCC.model.QwertyUI;
import com.Source.S1_MCC.MCC.model.UserInputString;
import com.Source.S1_MCC.MCC.utility.GetAppInfor;
import com.Source.S1_MCC.MCC.utility.GetMemoryInfor;
import com.Source.S1_MCC.MCC.utility.TemporaryData;
import android.os.BatteryManager;
import android.content.IntentFilter;


import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.lang.reflect.Method;
import java.net.ServerSocket;
import java.net.Socket;
import java.text.ParseException;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import static android.content.ContentValues.TAG;
import static java.lang.Short.MAX_VALUE;

import CTOS.CtCtms;
import CTOS.CtSettings;
import pl.droidsonroids.gif.GifDrawable;
import pl.droidsonroids.gif.GifImageView;

import com.Source.S1_MCC.MCC.Kms.KMSUI;

import com.Source.S1_MCC.MCC.utility.GetNetworkInfor;
import com.Source.S1_MCC.MCC.wub_lib;

import java.text.SimpleDateFormat;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.ScheduledThreadPoolExecutor;
import java.util.concurrent.TimeUnit;


import CTOS.CtSystem;
import CTOS.CtSystemException;

//testlang-removelater
import CTOS.CtSystem;


import android.support.constraint.ConstraintLayout;

import android.os.PowerManager;
import android.widget.Toast;

import com.Source.S1_MCC.MCC.Trans.ObjSelected;

public class MainActivity extends DemoAppActivity implements View.OnClickListener {
    public static boolean Chinese = false;
    public static int Finish_Printf = 0;
    String Reboot="REBOOT_NO";
    public String AppHeaderBuff;
    String szMSR_status="MSR_NO";
    String szICC_status="ICC_NO";

	public int inMENUTYPE;


    TextView app_version;
    TextView merchant;
    TextView tprofile;
    TextView carrier_name;
    TextView idlemsgbox;
	TextView mcc_menu_title;
	
    Button btn_start;
    Button btn_main_here;

    private Button btn_reports;
    private Button btn_settle;
    private Button btn_settings;
	private Button btn_credit;
    private Button btn_bancnet;
	private Button btn_wallet;
	private Button btn_phqr;
	private Button btn_logon;
	private Button btn_settings2;
	
    private static Handler mHandler;
    private static Context mContext;

    private TextView title, result;
    private ImageView refresh, check;
    private ImageView[] food = new ImageView[6];
    private TextView[] food_count = new TextView[6];

    private int[] foodPickMount = new int[6];
    private int[] foodPrice = {45, 35, 160, 30, 40, 25};
    private int[] inPaddingLeft = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    private int total_price = 0;
    int ctlsdemo = 0;

    private String in_string="";
    //private boolean fSTART = false;
    private int idlecardentry_status = 0;

    public static boolean fSTART = false, fBlockTrans=false;

    int inKeySet = 0;
    int inKeyIndex= 0;
    int inKeyType = 0;
    int inPinBypassAllow = 0;
    String StrInData= "";
    public static String final_string;
    public static int Enter_Press;
    public static int inRet;
    public static String PinBlock;
    public static String KSN;

    public static String sPinBlock = "";
    public static String sKSN = "";
    public static String OutputStr = "";

    protected PowerManager.WakeLock mWakeLock;
    public ScheduledExecutorService mScheduledExecutorService = null;
    private boolean FLAG_IDLE_FUN = false;//if need idle task, only use FLAG_IDLE_FUN or FLAG_IDLE_TASK_FUN
    public int mCount = 0;
    private boolean isActivtyFront = false;

    Activity activity = this;
    //Button startButton;
    //Button startButton2;
    EditText PinDigitET;
    final KMSUI kmsui = new KMSUI();

    TextView tv;
    EditText edtLog;
    Thread test;
    String list[] = {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};
    ListView lsv_LCDTPrint;
    ArrayAdapter adapter;
    String displayMode[] = {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};
    int addPadding[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    boolean isInitialize = false;

    Double dAmount;
    EditText etAmount;
    EditText etAmount1;
    private String amt_string="";

    private int uiFlag = 0;

    private String pass_in_string="";
    private DPopupMenu popMenu;
    AdapterView mAdapter[];

    /* Database */
//    private Cursor employees;
//    private MyDatabase db;

    /*
    SD or flash card write
     */
    private EditText editname;
    private EditText editdetail;
    private Button btnsave;
    private Button btnclean;
    private Button btnread;
    private Button btnauto;

    int inGravity;


    TextView tvMsgLine01;
    TextView tvMsgLine02;
    TextView tvMsgLine03;
    TextView tvMsgLine04;
    TextView tvMsgLine05;
    TextView tvMsgLine06;
    TextView tvMsgLine07;
    TextView tvMsgLine08;
    TextView tvMsgLine09;
    TextView tvMsgLine10;
    TextView tvMsgLine11;
    TextView tvMsgLine12;
    TextView tvMsgLine13;
    TextView tvMsgLine14;
    TextView tvMsgLine15;
    TextView tvMsgLine16;

    ImageView imageView;


    /* Dmenu */
    //private String[] titles = {"INIT WAVE", "NETS Flash Pay", "NETS", "Credit Card/UPI", "NETS QR", "BCA Card", "NETS PP/Loyalty", "CEPAS", "NETS Cash Card", "Admin", "Setting",
            //"demo", "SALE", "VOID", "Comm Test", "EditDB", "DispMsg", "DispImg", "SETTLE"};
    //private String[] titles = {"SALE", "VOID", "SETTLE", "CARD_VER", "OFFLINE", "EditDB", "REPORTS"};
    //private String[] titles = {"SALE", "VOID", "INSTALLMENT", "SETTLEMENT", "BALANCE", "BATCH TOTAL", "BATCH REVIEW", "REPRINT", "REPORTS",  "REGISTRATION", "EMV TAGS", "SETUP", "Edit Table"};
    private String[] titles = {"START"};

    private ViewPager mPager;
    private List<View> mPagerList;
    private List<Model> mDatas;
    private LinearLayout mLlDot;
    private LayoutInflater inflater;
    Button button;
    int intransCount = 0;
    /**
     * 总的页数
     */
    private int pageCount;
    /**
     * 每一页显示的个数
     */
    private int pageSize = 35;
    /**
     * 当前显示的是第几页
     */
    private int curIndex = 0;

    private boolean isHide = false;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("crypto");
    }

    static {
        System.loadLibrary("ssl");
    }

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private JNI_Lib jni_api = new JNI_Lib();
    private JNICB_implements_CTOS jni_cb_ctos = new JNICB_implements_CTOS(this);
    private JNICB_implements_IPT jni_cb_ipt = new JNICB_implements_IPT(this);

    private JNI_offlinepin jni_api_offline_pin = new JNI_offlinepin();
	private JNICB_implements_EMV jni_cb_emv = new JNICB_implements_EMV(this);

	// Variable declaration
    public static ObjSelected objSelected;
    public static String sDefaultNull = "";
    public static int sDefaultInt = 0;
    public static double sDefaultDouble;
    public static boolean sDefaultBoolean;

	public static int inTermType;
    public static int inPrintConfirmTimeOut=10;
	
	public void vdClearFile(String fileName) {
	
		Log.i(TAG, "del file" + fileName);
	//			  String fileName = "/data/data/pub/TERMINAL.S3DB";
		try {
			File file = new File(fileName);
			if (!file.exists()) {
				Log.i(TAG,	"["  + fileName + "]" + "  Not exist");
			} else {
				if (file.isFile()) {
					Log.i(TAG, "file exist: " + fileName + "  Deleting...");
					boolean ret = false;
					ret = file.delete();
					Log.i(TAG, "del result:: " + ret);
	//											  file.deleteOnExit();
					File file1 = new File(fileName);
					if (!file1.exists()) {
						Log.i(TAG, "Del file:" +  "Not exist" + "  expected!!!");
					}
	//											  ret = deleteFile(fileName);
					Log.i(TAG, "del result: " + ret);
				}
			}
		} catch (Exception e) {
			Log.e(TAG, "exception: " + e.getMessage());
			e.printStackTrace();
		}
	
	}

    private void copyFileOrDir(String path) {
        AssetManager assetManager = this.getAssets();
        String assets[] = null;
        try {
            Log.i("Castles", "copyFileOrDir() " + path);
            assets = assetManager.list(path);
            if (assets.length == 0) {
                copyFile(path);
            } else {
                for (int i = 0; i < assets.length; ++i) {
                    String p;
                    if (path.equals(""))
                        p = "";
                    else
                        p = path + "/";

                    if (!path.startsWith("images") && !path.startsWith("sounds") && !path.startsWith("webkit"))
                        copyFileOrDir(p + assets[i]);
                }
            }
        } catch (IOException ex) {
            Log.e("Castles", "I/O Exception", ex);
        }
    }

    public int chmod(File path, int mode) throws Exception {
        Class fileUtils = Class.forName("android.os.FileUtils");
        Method setPermissions =
                fileUtils.getMethod("setPermissions", String.class, int.class, int.class, int.class);
        return (Integer) setPermissions.invoke(null, path.getAbsolutePath(), mode, -1, -1);
    }

	private void copyFile(String filename) {
			AssetManager assetManager = this.getAssets();
			int inFlagFs_dataSkip = 0;
	
			Log.i(TAG, "copyFile: run");
			InputStream in = null;
			OutputStream out = null;
			String newFileName = null;
	
			String strFilename = filename.substring(filename.lastIndexOf("/") + 1);
			Log.i("Castles", "strFilename() " + strFilename);
	
			try {
				Log.i("Castles", "copyFile() " + filename);
	
				if (filename.toLowerCase().startsWith("21"))
				{
					newFileName = "/data/data/pub/" + strFilename;
				}
				else if (filename.toLowerCase().startsWith("11"))
				{
					newFileName = "/data/data/font/" + strFilename;
				}
				else if (filename.toLowerCase().startsWith("55"))
				{
					newFileName = "/data/data/pub/" + strFilename;
				}
				else if (filename.toLowerCase().startsWith("56"))
				{
					newFileName = "/data/data/pub/" + strFilename;		
				}
				else if (filename.toLowerCase().startsWith("04"))
				{
					inCTOSS_CreatefsdataFolder();
	
					newFileName = "/data/data/" + this.getPackageName() + "/fs_data/" + strFilename;
					Log.i(TAG, "copyFile: strFilename " + strFilename  +  " newFileName: " + newFileName);
	
					//if (inCTOSS_fGetECRFlg() != 1)
					//{
					/*
						if(strFilename.equals("castleslogo.bmp"))
						{
							newFileName = "/data/data/" + this.getPackageName() + "/fs_data/" + "logo.bmp";
							Log.i(TAG, "copyFile: strFilename " + strFilename  +  " newFileName: " + newFileName);
						}
						else if(strFilename.equals("logo.bmp"))
						{
							Log.i(TAG, "copyFile: return strFilename " + strFilename  +  " newFileName: " + newFileName);
							return;
						}
					*/
					//}
	
					inFlagFs_dataSkip = 0;
				}
				else
				{
					 newFileName = "/data/data/" + this.getPackageName() + "/" + strFilename;
				}
				
				Log.i("Castles", "copyFile() Path " + newFileName);
				
				Log.i("Castles", "copyFile() Path " + newFileName);
	
				Log.i(TAG, "inFlagFs_dataSkip="+inFlagFs_dataSkip);
				File dbFile = new File(newFileName);
	
				// patrick testing code start 20190703
				if (dbFile.exists())
				{
					if(newFileName.equals("/data/data/pub/ENV.S3DB"))
						return;
					
					Log.i(TAG, "File: " + newFileName + " already exist!!!");
					if (inFlagFs_dataSkip == 0)
						dbFile.delete();
					else
						inFlagFs_dataSkip = 0;
				}
				// patrick testing code end 20190703
	
				//if (!dbFile.exists()) {
					//Log.i(TAG, "File: " + newFileName + " not exist!!!");
					//Log.i("Castles", "copyFile() !exists");
					in = assetManager.open(filename);
					out = new FileOutputStream(newFileName);
	
					byte[] buffer = new byte[2048];
					int read;
					while ((read = in.read(buffer)) != -1) {
						out.write(buffer, 0, read);
					}
					in.close();
					in = null;
					out.flush();
					out.close();
					out = null;
					chmod(new File(newFileName), 0777);
	
					Log.i(TAG, "File: " + newFileName + " copied complete");
				//}
			} catch (Exception e) {
				Log.e("Castles", "Exception in copyFile() of " + newFileName);
				Log.e("Castles", "Exception in copyFile() " + e.toString());
			}
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


    //public static final Lock LOCK = new Lock();
    public static Lock LOCK = new Lock();

    final Handler handler = new Handler(){
        @Override
        public  void handleMessage(Message msg){
            super.handleMessage(msg);
            Log.i("handleMessage", "Process");
            if(msg.what == 1) {
                    //GetMenu();
                    GetMenuEx();
            }
        }
    };

	private void CheckInstallFlagAndRunCTMS(SharedPreferences wmbPreference)
	{
		int ret = 0;
		boolean isCheckInstallFlag = wmbPreference.getBoolean("CHECKINSTALL", false);
		Log.i(TAG, "isCheckInstallFlag=: " + isCheckInstallFlag);
		if (isCheckInstallFlag )
		{
			CtCtms ctCtms = MccApplication.getCtCtmsObj();
			if(ctCtms != null) {
				ret = ctCtms.ResetFolder();
				Log.i(TAG, "ResetFolder=: " + ret);
			}
			wmbPreference.edit().remove("CHECKINSTALL").commit();
			Log.i(TAG, "call ctms udpate: ");
			Intent intent = new Intent();
			intent.putExtra("UPDATE_CONFIG_FLAG", false);
			intent.setClass(MainActivity.this, Activity_ctms_background.class);
			startActivity(intent);
	
		}
	}

    private void applyStatusBar(String iconTitle, int notificationId) {
        NotificationCompat.Builder mBuilder = new NotificationCompat.Builder(this)
                .setSmallIcon(R.mipmap.icon_batchreview)
                .setContentTitle(iconTitle);
        Intent resultIntent = new Intent(this, MainActivity.class);
        PendingIntent resultPendingIntent = PendingIntent.getActivity(this, 0, resultIntent, PendingIntent.FLAG_UPDATE_CURRENT);
        mBuilder.setContentIntent(resultPendingIntent);
        Notification notification = mBuilder.build();
        notification.flags |= Notification.FLAG_NO_CLEAR | Notification.FLAG_ONGOING_EVENT;

        NotificationManager mNotifyMgr = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
        mNotifyMgr.notify(notificationId, notification);
    }

    public String getSysYear() {
        String szCurrentYear = "";
        try {
            SimpleDateFormat sdf = new SimpleDateFormat("yyyy");
            szCurrentYear = sdf.format(new Date());
            Log.i("saturn", "getSysYear "+ szCurrentYear);
        } catch (Exception e) {
            Log.e(TAG, "getSysYear: " +e.getMessage() );
        } finally {

            return szCurrentYear;
        }

    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        TemporaryData.init(this);
        overridePendingTransition(0, 0);

		//testlang-removelater
		CtSystem ctSystem = new CtSystem();


        mHandler = new Handler(getMainLooper());
        mContext = this;


        SharedPreferences.Editor editorctls = TemporaryData.getSharedPreferences().edit();
        editorctls.putInt(TemporaryData.CTLSDEMO, 0);
        editorctls.apply();

        SharedPreferences sharedPreferences2 = PreferenceManager.getDefaultSharedPreferences(this);

        SharedPreferences.Editor editor2 = sharedPreferences2.edit();
        editor2.putInt("UIFLAG", 0);
        editor2.apply();

        SharedPreferences sharedPreferences = PreferenceManager.getDefaultSharedPreferences(this);
        uiFlag = sharedPreferences.getInt("UIFLAG", 0);

        Log.d("onCreate", uiFlag+"");



        Log.i("initDatas", "START!!!");

        //AppHeaderBuff = GetAppHeaderDetails();


        // Show status bar
        getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.dmenu_activity_main);

        //displayNotification("23/03");

        /* This code together with the one in onDestroy()
         * will make the screen be always on until this Activity gets destroyed.
        final PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        this.mWakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "S1_MCC:AAA-MainActivity>>WAKE_LOCK");
        this.mWakeLock.acquire();*/

        mPager = (ViewPager) findViewById(R.id.viewpager);
        mLlDot = (LinearLayout) findViewById(R.id.ll_dot);
        tvMsgLine01 = (TextView) findViewById(R.id.msg_text_01);
//        btnauto = (Button) findViewById(R.id.btn_auto);
        /*make it no sleep*/
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        SharedPreferences wmbPreference = PreferenceManager.getDefaultSharedPreferences(this);

        //boolean isFirstRun = wmbPreference.getBoolean("FIRSTRUN", true);

        //TINE:  mcc requirement
        //setContentView(R.layout.mcc_activity_main);
        //TINE:  -end

//        btnauto.setOnClickListener(this);
/*
        //test
        CtSystem system = new CtSystem(); 
		String setTimeZone = "Asia/Taipei";
        String server = "216.239.35.0";
        String port = "123";
        // setTimeZone
        Log.d(TAG, " return = " + Integer.toString(system.setTimeZone(setTimeZone)));
        // updataNTPSetting
        Log.d(TAG, " return = " + Integer.toString(system.updateNTPSetting(server, port, true, 1)));
*/

	  //ctms prm file exist
        //applyStatusBar("Statusbar Test", 10);

       if(isCTMSParameterFileExist()) {
            Log.i(TAG, "reset FIRSTRUN: ");
            wmbPreference.edit().remove("FIRSTRUN").commit();
       }

       int firstInit = inCTOSS_Get1stInitFlag();
       int DLNotFinished = inCTOSS_GetDLNotFinishedFlag();
       Log.i(TAG, "-inCTOSS_Get1stInitFlag: " + firstInit + " DLNotFinished:" + DLNotFinished);
       boolean isFirstRun = wmbPreference.getBoolean("FIRSTRUN", true);
       Log.i(TAG, "-isFirstRun =" + isFirstRun);

	   //CheckInstallFlagAndRunCTMS(wmbPreference);

        //DLNotFinished = 0;
        if(DLNotFinished == 1)
        {
            //terminal need to restart after call ctms in this case
            ScheduledExecutorService scheduledExecutorService = new ScheduledThreadPoolExecutor(1);
            scheduledExecutorService.schedule(new Runnable() {
                @Override
                public void run() {
                    Log.i(TAG, "run: ctms call");
                    Intent intent = new Intent();
                    intent.putExtra("REBOOT", true);
                    intent.setClass(MainActivity.this, Activity_ctms_update.class);
                    startActivity(intent);
                    Log.i(TAG, "run: ctms done");
                }
            }, 1000/2, TimeUnit.MILLISECONDS);
            Log.i(TAG, "return oncreate: ");
            return;
        }

        if(firstInit == 1) {
            isFirstRun = true;
            Log.i(TAG, "set isFirstRun true,after init set it to false: ");
        }

        if (isFirstRun)
        {
            //testlang-removelater
			//Reboot = Reboot.replace("REBOOT_NO", "REBOOT_YES");

            jni_api.REG_CB_CTOS(jni_cb_ctos);
			jni_api_offline_pin.REG_CB_EMV(jni_cb_emv);

            View decorView = getWindow().getDecorView();
            decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_LAYOUT_STABLE);
			decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY| View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);

            setContentView(R.layout.activity_disp);
            lsv_LCDTPrint = (ListView) findViewById(R.id.lsv_LCDTPrint);
            edtLog = (EditText) findViewById(R.id.edtLog);
            adapter = new ArrayAdapter(MainActivity.this, R.layout.listitem, list) {

                @NonNull
                @Override
                public View getView(int position, @Nullable View convertView, @NonNull ViewGroup parent) {
                    View view = super.getView(position, convertView, parent);

                    TextView textView = (TextView) view;

                    if (addPadding[position] != 0)
                        textView.setPadding(36 * addPadding[position], 0, 0, 0);


                    if (displayMode[position].equals("0")) {
                        textView.setGravity(Gravity.LEFT);
                    } else if (displayMode[position].equals("1")) {
                        textView.setGravity(Gravity.CENTER);
                    } else if (displayMode[position].equals("2")) {
                        textView.setGravity(Gravity.RIGHT);
                    }


                    return view;

                }
            };

            {
                //setContentView(R.layout.activity_keypad);
                Log.i("Castles", "INIT");
                new Thread() {
                    @Override
                    public void run() {
                        try {

                            //copy file or Dir
                            //LCDDisplay(" |Initializing...");
                            vdClearFile("/data/data/pub/TERMINAL.S3DB");  //for testing use only
                            vdClearFile("/data/data/pub/EMV.S3DB");
							vdClearFile("/data/data/pub/CFG.S3DB");
							vdClearFile("/data/data/com.Source.S1_MCC.MCC/S1_MCC.S3DB");
                            vdClearFile("/data/data/pub/mccpay.bmp");
                            vdClearFile("/data/data/pub/allcards.bmp");
							vdClearFile("/data/data/pub/supported0.bmp");
							vdClearFile("/data/data/pub/supported1.bmp");
							vdClearFile("/data/data/pub/supported2.bmp");
							vdClearFile("/data/data/pub/supported3.bmp");
							vdClearFile("/data/data/pub/supported4.bmp");
							vdClearFile("/data/data/pub/phqr.bmp");

                            for (int x = 1; x <= 122; ++x) {
                                if (x <= 9) {
                                    String s = "/data/data/com.Source.S1_MCC.MCC/DYNAMICMENU0" + x + ".S3DB";
                                    vdClearFile(s);
                                } else {
                                    String s = "/data/data/com.Source.S1_MCC.MCC/DYNAMICMENU" + x + ".S3DB";
                                    vdClearFile(s);
                                }
                            }

							vdClearFile("/data/data/com.Source.S1_MCCPAY.S1_MCCPAY/QRPAYMENU.S3DB");
							
                            //copy file or Dir
                            //testlang-removelater
                           	ctSystem.setPowerKey(false);

                            DisplayStatusBox("Initializing|Please wait...|wait");
                            //LCDDisplay(" |INITIALIZING...");
                            copyFileOrDir("");
                            inCTOSS_InitWaveData();
                            //inCTOSS_TerminalStartUp();

							//testlang-removelater
							ctSystem.setPowerKey(true);

                            //titles[0] = "REGISTER";

                            //AppRegister();

                            final Thread thread = new Thread(new Runnable() {

                                @Override
                                public void run() {
                                    Message message = new Message();
                                    message.what = 1;
                                    Log.d("handler.sendMessage", "test");
                                    handler.sendMessageDelayed(message, 100);
                                    //handler.sendMessage(message);
                                }

                            });

                            thread.start();

                        } catch (Exception e) {

                        }
                    }
                }.start();
            }
                //inCTOSS_ProcessCfgExpress();
                SharedPreferences.Editor editor = wmbPreference.edit();
                editor.putBoolean("FIRSTRUN", false);
                editor.commit();
        }
        else{
            Log.i(TAG, "run: firstboot else");
							
            Boolean isECR = false;
            Intent intent1=getIntent();
            if(intent1 != null) {
//                String strEcrCmd = null;
                final String strEcrCmd = intent1.getStringExtra(EcrCommandDefintion.ECR_REQ_CMD_NAME);
                //String strEcrCmd = intent1.getStringExtra(EcrCommandDefintion.ECR_REQ_CMD_NAME);
                Log.i(TAG, "strEcrCmd: " + strEcrCmd);

				//if(fSTART == true)
					//strEcrCmd=null;

				Log.i(TAG, "strEcrCmd fSTART:" + fSTART);
				
                if(strEcrCmd != null && !strEcrCmd.isEmpty()) {
                    isECR = true;
                    fBlockTrans = true;

                    try {
                        DisplayBox("ECR Transaction|Please Wait..| |info");
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
					
                    ScheduledExecutorService executorService = new ScheduledThreadPoolExecutor(1);
                    executorService.schedule(new Runnable() {
                        @Override
                        public void run() {
                            Log.i(TAG, "run: startEcrTxn");
							
                            startEcrTxn(strEcrCmd);
                            Log.i(TAG, "run: startEcrTxn done");
                        }
                    }, 1000/2, TimeUnit.MILLISECONDS);
                }
            }
			
            if (!isECR) {
                Log.i("Castles", "TerminalStartUp");
                //GetMenu();
                jni_api.REG_CB_CTOS(jni_cb_ctos);
                jni_api_offline_pin.REG_CB_EMV(jni_cb_emv);

                View decorView = getWindow().getDecorView();
                decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_LAYOUT_STABLE);
                decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);

                setContentView(R.layout.activity_disp);
                lsv_LCDTPrint = (ListView) findViewById(R.id.lsv_LCDTPrint);
                edtLog = (EditText) findViewById(R.id.edtLog);
                adapter = new ArrayAdapter(MainActivity.this, R.layout.listitem, list) {

                    @NonNull
                    @Override
                    public View getView(int position, @Nullable View convertView, @NonNull ViewGroup parent) {
                        View view = super.getView(position, convertView, parent);

                        TextView textView = (TextView) view;

                        if (addPadding[position] != 0)
                            textView.setPadding(36 * addPadding[position], 0, 0, 0);


                        if (displayMode[position].equals("0")) {
                            textView.setGravity(Gravity.LEFT);
                        } else if (displayMode[position].equals("1")) {
                            textView.setGravity(Gravity.CENTER);
                        } else if (displayMode[position].equals("2")) {
                            textView.setGravity(Gravity.RIGHT);
                        }


                        return view;

                    }
                };


                new Thread() {
                    @Override
                    public void run() {
                        try {
                            //jni_api.REG_CB_CTOS(jni_cb_ctos);
                            //jni_api_offline_pin.REG_CB_EMV(jni_cb_emv);
                            //LCDDisplay(" |Please wait...");
                            DisplayStatusBox("Loading application|Please wait...|wait");
                            //LCDDisplay(" |LOADING...");
                            //inCTOSS_ProcessCfgExpress();
                            //inCTOSS_InitWaveData();
                            inCTOSS_TerminalStartUp(getSysYear());

                            final Thread thread = new Thread(new Runnable() {

                                @Override
                                public void run() {
                                    Message message = new Message();
                                    message.what = 1;
                                    Log.d("handler.sendMessage", "test");
                                    handler.sendMessageDelayed(message, 100);
                                    //handler.sendMessage(message);
                                }

                            });

                            thread.start();

                        } catch (Exception e) {

                        }
                    }
                }.start();
                //what you do everytime goes here
            }
        }
}
    /**
     * 初始化数据源
     */
    private void initDatas(String szIdleMode) {

        mDatas = new ArrayList<Model>();
        for (int i = 0; i < titles.length; i++) {
            //动态获取资源ID，第一个参数是资源名，第二个参数是资源类型例如drawable，string等，第三个参数包名
            //icons for mcc
            if(szIdleMode.equals("1")) {
                int imageId = getResources().getIdentifier("ic_category_" + i, "mipmap", getPackageName());
                mDatas.add(new Model(titles[i], imageId));
            } else if(szIdleMode.equals("2")) {
                int imageId = getResources().getIdentifier("ic_menu_" + i, "mipmap", getPackageName());
                mDatas.add(new Model(titles[i], imageId));
            }

        }

    }


    public void startEcrTxn(String txnType) {
        Log.i(TAG, "****startEcrTxn: ****" + txnType);
        //scheduleTaskStop();
        //onIdlePause();
        //To test memory leak, no logic in C
        //fBlockTrans = true;

        if(false) {
            Log.i(TAG, "ignore ecr command test: ");
            wakeUpScreen();
            //setupMyViews("Credit Sale");
            Log.i("Castles", "SALE done");
            final Thread thread = new Thread(new Runnable() {
                @Override
                public void run() {
                    Log.i(TAG, "run txn: ");
                    jni_api.REG_CB_CTOS(jni_cb_ctos);
                    jni_api_offline_pin.REG_CB_EMV(jni_cb_emv);
                    inCTOSS_Sale(true);
                    Message message = new Message();
                    message.what = 1;
                    Log.d("handler.sendMessage", "sale");
                    handler.sendMessageDelayed(message, 200);
                }

            });

            thread.start();
            return;
        }

        if(txnType.equals(EcrCommandDefintion.ECR_REQ_CMD_SALE))
        {
            Log.i(TAG, "process ecr command: ");
            wakeUpScreen();
            //setupMyViews("Credit Sale");

            Log.i(TAG, "****Credit Sale: ****");
            new Thread() {
                @Override
                public void run() {
                    try {
						fSTART = true;
                        GetMemoryInfor.displayBriefMemory(mContext);
                        jni_api.REG_CB_CTOS(jni_cb_ctos);
                        jni_api_offline_pin.REG_CB_EMV(jni_cb_emv);
                        //inCTOSS_Sale(true);
                        inCTOSS_ECR_Menu(1);
                        //GetMemoryInfor.displayBriefMemory(mContext);
//									copyFileToLocal("/data/data/pub/Print_BMP.bmp");
                        Log.i("Castles", "ECR SALE done");
						fSTART = false;

						EcrCommandReceiver.isEcrProcessing=false;
						/*
                        final Thread thread = new Thread(new Runnable() {

                            @Override
                            public void run() {

                                if(false)//To handle ecr indenpendly as sub app
                                {
                                    Log.i(TAG, "not main app, so run ecr MainActivity finish: ");
                                    MainActivity.this.finish();
                                    int pid = Process.myPid();
                                    Log.i(TAG, "Process pid: " + pid);
//                                    Process.killProcess(pid);
                                    System.exit(0);
                                }
                                else
                                {
                                    Message message = new Message();
                                    message.what = 1;
                                    Log.d("handler.sendMessage", "sale");
                                    handler.sendMessageDelayed(message, 100);
                                }

                            }

                        });

                        thread.start();
						*/
                    } catch (Exception e) {

                    }
                }
            }.start();
        }
		else if (txnType.equals(EcrCommandDefintion.ECR_REQ_CMD_VOID)){ 
            Log.i("Castles", "Void");
            wakeUpScreen();
            //setupMyViews("Void");
            new Thread() {
                @Override
                public void run() {
                    try {
						fSTART = true;
                        jni_api.REG_CB_CTOS(jni_cb_ctos);
                        // patrick fix offline pin crash issue 20210123
                        jni_api_offline_pin.REG_CB_EMV(jni_cb_emv);
                        //inCTOSS_Void(true);
                        inCTOSS_ECR_Menu(2);
                        fSTART = false;

                        Log.i("Castles", "ECR Void done");
						
						EcrCommandReceiver.isEcrProcessing=false;
                      /* final Thread thread = new Thread(new Runnable(){

                            @Override
                            public void run() {
                                Message message = new Message();
                                message.what = 1;
                                Log.d("handler.sendMessage",  "test");
                                handler.sendMessageDelayed(message, 100);
                            }

                        });

                        thread.start(); */
                    } catch (Exception e) {

                    }
                }
            }.start();
           
        }
		/*
		else if (txnType.equals(EcrCommandDefintion.ECR_REQ_CMD_REFUND)){
            Log.i("Castles", "Refund");
            wakeUpScreen();
            setupMyViews("Refund");
            new Thread() {
                @Override
                public void run() {
                    try {

                        jni_api.REG_CB_CTOS(jni_cb_ctos);
                        // patrick fix offline pin crash issue 20210123
                        jni_api_offline_pin.REG_CB_EMV(jni_cb_emv);
                        inCTOSS_REFUND();

                        final Thread thread = new Thread(new Runnable(){

                            @Override
                            public void run() {
                                Message message = new Message();
                                message.what = 1;
                                Log.d("handler.sendMessage",  "test");
                                handler.sendMessageDelayed(message, 100);
                                //handler.sendMessage(message);
                            }

                        });

                        thread.start();
                    } catch (Exception e) {

                    }
                }
            }.start();
        } 
     else if (txnType.equals(EcrCommandDefintion.ECR_REQ_CMD_SETTLE_ALL)){
            Log.i("Castles", "SETTLE ALL");
            wakeUpScreen();
            setupMyViews("SETTLE ALL");
            new Thread() {
                @Override
                public void run() {
                    try {

                        jni_api.REG_CB_CTOS(jni_cb_ctos);
                        // patrick fix offline pin crash issue 20210123
                        jni_api_offline_pin.REG_CB_EMV(jni_cb_emv);
                        inCTOSS_SETTLE_ALL();

                        final Thread thread = new Thread(new Runnable(){

                            @Override
                            public void run() {
                                Message message = new Message();
                                message.what = 1;
                                Log.d("handler.sendMessage",  "settle all");
                                handler.sendMessageDelayed(message, 100);
                            }

                        });

                        thread.start();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            }.start();
        }
        else if (txnType.equals(EcrCommandDefintion.ECR_REQ_CMD_SETTLE))
        {
            Log.i("Castles", "SETTLEMENT");
            wakeUpScreen();
            setupMyViews("SETTLEMENT");
            new Thread() {
                @Override
                public void run() {
                    try {

                        jni_api.REG_CB_CTOS(jni_cb_ctos);
                        // patrick fix offline pin crash issue 20210123
                        jni_api_offline_pin.REG_CB_EMV(jni_cb_emv);
                        inCTOSS_SETTLEMENT(true);

                        final Thread thread = new Thread(new Runnable(){

                            @Override
                            public void run() {
                                Message message = new Message();
                                message.what = 1;
                                Log.d("handler.sendMessage",  "test");
                                handler.sendMessageDelayed(message, 100);
                            }

                        });

                        thread.start();
                    } catch (Exception e) {

                    }
                }
            }.start();
        }
        else if (txnType.equals(EcrCommandDefintion.ECR_REQ_CMD_ECHO_TEST))
        {
            Log.i("Castles", "ECHO TEST");
            wakeUpScreen();
            setupMyViews("ECHO TEST");
            new Thread() {
                @Override
                public void run() {
                    try {

                        jni_api.REG_CB_CTOS(jni_cb_ctos);
                        // patrick fix offline pin crash issue 20210123
                        jni_api_offline_pin.REG_CB_EMV(jni_cb_emv);
                        inCTOSS_Echo_Test(true);

                        final Thread thread = new Thread(new Runnable(){

                            @Override
                            public void run() {
                                Message message = new Message();
                                message.what = 1;
                                Log.d("handler.sendMessage",  "test");
                                handler.sendMessageDelayed(message, 100);
                            }

                        });

                        thread.start();
                    } catch (Exception e) {

                    }
                }
            }.start();
        }
        else if (txnType.equals(EcrCommandDefintion.ECR_REQ_CMD_RETRIEVAL))
        {
            Log.i("Castles", "RETRIEVAL");
            wakeUpScreen();
            setupMyViews("RETRIEVAL");
            new Thread() {
                @Override
                public void run() {
                    try {

                        jni_api.REG_CB_CTOS(jni_cb_ctos);
                        // patrick fix offline pin crash issue 20210123
                        jni_api_offline_pin.REG_CB_EMV(jni_cb_emv);
                        inCTOSS_Reprint_Any(true);

                        final Thread thread = new Thread(new Runnable(){

                            @Override
                            public void run() {
                                Message message = new Message();
                                message.what = 1;
                                Log.d("handler.sendMessage",  "test");
                                handler.sendMessageDelayed(message, 100);
                            }

                        });

                        thread.start();
                    } catch (Exception e) {

                    }
                }
            }.start();
        }
        else if (txnType.equals(EcrCommandDefintion.ECR_REQ_CMD_GET_TOTAL))
        {
            Log.i("Castles", "GET_TOTAL");
            wakeUpScreen();
            setupMyViews("GET_TOTAL");
            new Thread() {
                @Override
                public void run() {
                    try {

                        jni_api.REG_CB_CTOS(jni_cb_ctos);
                        // patrick fix offline pin crash issue 20210123
                        jni_api_offline_pin.REG_CB_EMV(jni_cb_emv);
                        inCTOSS_Summary_Report(true);

                        final Thread thread = new Thread(new Runnable(){

                            @Override
                            public void run() {
                                Message message = new Message();
                                message.what = 1;
                                Log.d("handler.sendMessage",  "test");
                                handler.sendMessageDelayed(message, 100);
                            }

                        });

                        thread.start();
                    } catch (Exception e) {

                    }
                }
            }.start();
        }
        else if (txnType.equals(EcrCommandDefintion.ECR_REQ_CMD_GET_LAST_TOTAL))
        {
            Log.i("Castles", "GET_LAST_TOTAL");
            wakeUpScreen();
            setupMyViews("GET_LAST_TOTAL");
            new Thread() {
                @Override
                public void run() {
                    try {

                        jni_api.REG_CB_CTOS(jni_cb_ctos);
                        // patrick fix offline pin crash issue 20210123
                        jni_api_offline_pin.REG_CB_EMV(jni_cb_emv);
                        inCTOSS_Last_Settle_Report(true);

                        final Thread thread = new Thread(new Runnable(){

                            @Override
                            public void run() {
                                Message message = new Message();
                                message.what = 1;
                                Log.d("handler.sendMessage",  "test");
                                handler.sendMessageDelayed(message, 100);
                            }

                        });

                        thread.start();
                    } catch (Exception e) {

                    }
                }
            }.start();
        }
        else if (txnType.equals(EcrCommandDefintion.ECR_REQ_CMD_BARCODE))
        {
            Log.i("Castles", "BARCODE");
            wakeUpScreen();
            setupMyViews("BARCODE");
            new Thread() {
                @Override
                public void run() {
                    try {

                        jni_api.REG_CB_CTOS(jni_cb_ctos);
                        // patrick fix offline pin crash issue 20210123
                        jni_api_offline_pin.REG_CB_EMV(jni_cb_emv);

                        Log.i("Castles", "1.inCTOSS_Barcode_Scanning");

                        //inCTOSS_Last_Settle_Report(true);

                        Intent intent = new Intent();
                        intent.putExtra("ECR_TRANS", "true");
                        intent.setClass(MainActivity.this, S1F2BarcodeScanActivity.class);
                        startActivity(intent);

                        //Log.i("Castles", "2.inCTOSS_Barcode_Scanning");
                        //inCTOSS_Barcode_Scanning(true);

                        final Thread thread = new Thread(new Runnable(){

                            @Override
                            public void run() {
                                Message message = new Message();
                                message.what = 1;
                                Log.d("handler.sendMessage",  "test");
                                handler.sendMessageDelayed(message, 100);
                            }

                        });

                        thread.start();
                    } catch (Exception e) {

                    }
                }
            }.start();
        }
        else if (txnType.equals(EcrCommandDefintion.ECR_REQ_CMD_PRINTING))
        {
            Log.i("Castles", "PRINTING");
            wakeUpScreen();
            setupMyViews("PRINTING");
            new Thread() {
                @Override
                public void run() {
                    try {

                        jni_api.REG_CB_CTOS(jni_cb_ctos);
                        // patrick fix offline pin crash issue 20210123
                        jni_api_offline_pin.REG_CB_EMV(jni_cb_emv);
                        inCTOSS_ECR_Printing(true);

                        final Thread thread = new Thread(new Runnable(){

                            @Override
                            public void run() {
                                Message message = new Message();
                                message.what = 1;
                                Log.d("handler.sendMessage",  "test");
                                handler.sendMessageDelayed(message, 100);
                            }

                        });

                        thread.start();
                    } catch (Exception e) {

                    }
                }
            }.start();
        }
        else if (txnType.equals(EcrCommandDefintion.ECR_REQ_CMD_READCARD))
        {
            Log.i("Castles", "READCARD");
            wakeUpScreen();
            setupMyViews("READCARD");
            new Thread() {
                @Override
                public void run() {
                    try {

                        jni_api.REG_CB_CTOS(jni_cb_ctos);
                        // patrick fix offline pin crash issue 20210123
                        jni_api_offline_pin.REG_CB_EMV(jni_cb_emv);
                        inCTOSS_ECR_ReadCard(true);

                        final Thread thread = new Thread(new Runnable(){

                            @Override
                            public void run() {
                                Message message = new Message();
                                message.what = 1;
                                Log.d("handler.sendMessage",  "test");
                                handler.sendMessageDelayed(message, 100);
                            }

                        });

                        thread.start();
                    } catch (Exception e) {

                    }
                }
            }.start();
        }
*/
        Log.i(TAG, "startEcrTxn: exit");
    }

    // dmenu start code
    /**
     * 设置圆点
     */
    public void setOvalLayout() {
        for (int i = 0; i < pageCount; i++) {
            mLlDot.addView(inflater.inflate(R.layout.dot, null));
        }
        // 默认显示第一页
        mLlDot.getChildAt(0).findViewById(R.id.v_dot)
                .setBackgroundResource(R.drawable.dot_selected);
        mPager.setOnPageChangeListener(new ViewPager.OnPageChangeListener() {
            public void onPageSelected(int position) {
                // 取消圆点选中
                mLlDot.getChildAt(curIndex)
                        .findViewById(R.id.v_dot)
                        .setBackgroundResource(R.drawable.dot_normal);
                // 圆点选中
                mLlDot.getChildAt(position)
                        .findViewById(R.id.v_dot)
                        .setBackgroundResource(R.drawable.dot_selected);
                curIndex = position;
            }

            public void onPageScrolled(int arg0, float arg1, int arg2) {
            }

            public void onPageScrollStateChanged(int arg0) {
            }
        });
    }
// dmenu end code
/*
    @Override
    protected void onDestroy() {
        super.onDestroy();
    }
*/
    private void bindViews() {
        editdetail = (EditText) findViewById(R.id.editdetail);
        editname = (EditText) findViewById(R.id.editname);
        btnclean = (Button) findViewById(R.id.btnclean);
        btnsave = (Button) findViewById(R.id.btnsave);
        btnread = (Button) findViewById(R.id.btnread);

        btnclean.setOnClickListener(this);
        btnsave.setOnClickListener(this);
        btnread.setOnClickListener(this);
    }

    @Override
    public void onClick(View v){
        switch (v.getId()) {
            case R.id.btnclean:
                editdetail.setText("");
                editname.setText("");
                break;
            case R.id.btnsave:
                break;
            case R.id.btnread:
                break;
            case R.id.viewpager:
                if(ctlsdemo == 1)
                {
                    intransCount++;
                //Toast.makeText(mContext, "Transaction Count "+intransCount, Toast.LENGTH_SHORT).show();
                jni_api.REG_CB_CTOS(jni_cb_ctos);
                setContentView(R.layout.activity_disp);
                lsv_LCDTPrint = (ListView) findViewById(R.id.lsv_LCDTPrint);
                edtLog = (EditText) findViewById(R.id.edtLog);
                adapter = new ArrayAdapter(MainActivity.this, R.layout.listitem, list) {

                    @NonNull
                    @Override
                    public View getView(int position, @Nullable View convertView, @NonNull ViewGroup parent) {
                        View view = super.getView(position, convertView, parent);


                        TextView textView = (TextView) view;

//                            Log.d("Position", String.valueOf(position));
//                            Log.d("Padding", String.valueOf(addPadding[position]));

                        if (addPadding[position] != 0)
                            textView.setPadding(36 * addPadding[position], 0, 0, 0);

//                            Log.d("DisplayMode", displayMode[position]);

                        if (displayMode[position].equals("0")) {
                            textView.setGravity(Gravity.LEFT);
                        } else if (displayMode[position].equals("1")) {
                            textView.setGravity(Gravity.CENTER);
                        } else if (displayMode[position].equals("2")) {
                            textView.setGravity(Gravity.RIGHT);
                        }


                        return view;

                    }
                };
            {
                Log.i("Castles", "SALE AUTO");
                new Thread() {
                    @Override
                    public void run() {
                        try {
                            inCTOSS_Sale(false);

                            final Thread thread = new Thread(new Runnable() {

                                @Override
                                public void run() {
                                    Message message = new Message();
                                    message.what = 1;
                                    Log.d("handler.sendMessage", "test");
                                    handler.sendMessageDelayed(message, 100);
                                    //handler.sendMessage(message);
                                }

                            });

                            thread.start();
                        } catch (Exception e) {

                        }
                    }
                }.start();
            }

        }
        }
    }

    public void showMsg( final String msg)
    {
        runOnUiThread(new Runnable()
                      {
                          @Override
                          public void run()
                          {
                              edtLog.append(msg + '\n');
                          }
                      }
        );
    }
    public String checkAppRunning(String msg)
    {
        if(isApplicationStarted(this, msg))
            return "1";
        return "0";
    }

/*
    public String ForkSubAP(String msg)
    {
        int inMsgCnt = 0;
        int inIdx = 0;
	// String szSubAP = ".Main.MultiAP_SubAP";
	 String szSubAP = ".Main.MainActivity";

       // Toast.makeText(getApplicationContext(), msg, Toast.LENGTH_SHORT).show();
        Log.i("Castles ForkSubAP", msg);
        if(!isApplicationStarted(this, msg))
        {
            Log.i(TAG, "msg: not running");
        } else {
            Log.i(TAG, msg + " is running!!");
            return "OK";
        }

//        ComponentName apk2Component = new ComponentName("com.Source.S1_Amex.Amex", "com.Source.S1_Amex.Amex.Main.MultiAP_SubAP");
//        ComponentName apk2Component = new ComponentName("com.Source.SHARLS_CTLS.SHARLS_CTLS", "com.Source.SHARLS_CTLS.SHARLS_CTLS.Main.MultiAP_SubAP");
	 ComponentName apk2Component = new ComponentName(msg, msg+szSubAP);
	 Log.i("Castles", msg+szSubAP);
     

	 Intent intent = new Intent();
        Bundle bundle = new Bundle();

        bundle.putString("subAP", "1");
        bundle.putString("subAP1", "0");

        intent.putExtras(bundle);
        intent.setComponent(apk2Component);

        startActivity(intent);
        return "OK";
    }
*/
    public String ForkSubAP(String msg)
    {
        int inMsgCnt = 0;
        int inIdx = 0;
        String szSubAP = ".Main.MainActivity";

        Log.i("Castles", msg);

        try {
            if (isRun(this, msg)) {
                Log.i(TAG, msg + " app is running: ");

                Log.i(TAG, msg + " app is running END: ");
                return "OK";
            } else {
                Log.d(TAG, msg + " app is not running");
            }

            ComponentName apk2Component = new ComponentName(msg, msg+szSubAP);
            Log.i("Castles", msg+szSubAP);

            Intent intent = new Intent();
            Bundle bundle = new Bundle();

            bundle.putString("subAP", "1");
            bundle.putString("subAP1", "0");

            intent.putExtras(bundle);
            intent.setComponent(apk2Component);

            startActivity(intent);
        }catch (Exception e) {
            Log.e(TAG, e.getMessage());
        }
        return "OK";
    }

    public String displayMultipleMsg(final String disp_msg)
    {
        mHandler.post(new Runnable() {
            @Override
            public void run() {

                setContentView(R.layout.displaytextmessage);

                tvMsgLine01 = (TextView) findViewById(R.id.msg_text_01);
                tvMsgLine02 = (TextView) findViewById(R.id.msg_text_02);
                tvMsgLine03 = (TextView) findViewById(R.id.msg_text_03);
                tvMsgLine04 = (TextView) findViewById(R.id.msg_text_04);
                tvMsgLine05 = (TextView) findViewById(R.id.msg_text_05);
                tvMsgLine06 = (TextView) findViewById(R.id.msg_text_06);
                tvMsgLine07 = (TextView) findViewById(R.id.msg_text_07);
                tvMsgLine08 = (TextView) findViewById(R.id.msg_text_08);
                tvMsgLine09 = (TextView) findViewById(R.id.msg_text_09);
                tvMsgLine10 = (TextView) findViewById(R.id.msg_text_10);
                tvMsgLine11 = (TextView) findViewById(R.id.msg_text_11);
                tvMsgLine12 = (TextView) findViewById(R.id.msg_text_12);
                tvMsgLine13 = (TextView) findViewById(R.id.msg_text_13);
                tvMsgLine14 = (TextView) findViewById(R.id.msg_text_14);
                tvMsgLine15 = (TextView) findViewById(R.id.msg_text_15);
                tvMsgLine16 = (TextView) findViewById(R.id.msg_text_16);

                tvMsgLine01.setTag(new Date().getTime());

                System.out.println("start displayMultipleMsg");
                System.out.println(disp_msg);

                {
                    getMsg();

                    String[] parsemsg = disp_msg.split("\\|");

                    System.out.println("line:"+parsemsg[1]+"msg:"+parsemsg[2]+"mode:"+parsemsg[3]);

                    int inmsgcnt = parsemsg.length;
                    int line = Integer.parseInt(parsemsg[1]);
                    String msg = parsemsg[2];
                    int dispmode = Integer.parseInt(parsemsg[3]);

                    System.out.println("line:"+line+msg+dispmode);

                    if(dispmode == 0)
                        inGravity = Gravity.LEFT;
                    else if(dispmode == 1)
                        inGravity = Gravity.CENTER;
                    else if(dispmode == 2)
                        inGravity = Gravity.RIGHT;

                    System.out.printf("inGravity[%d]\n", inGravity);

                    switch (line) {
                        case 1:
                            tvMsgLine01.setGravity(inGravity);
                            tvMsgLine01.setText(parsemsg[2]);

                            break;
                        case 2:
                            tvMsgLine02.setGravity(inGravity);
                            tvMsgLine02.setText(parsemsg[2]);

                            break;
                        case 3:
                            tvMsgLine03.setGravity(inGravity);
                            tvMsgLine03.setText(parsemsg[2]);
                            break;
                        case 4:
                            tvMsgLine04.setGravity(inGravity);
                            tvMsgLine04.setText(parsemsg[2]);
                            break;
                        case 5:
                            tvMsgLine05.setGravity(inGravity);
                            tvMsgLine05.setText(parsemsg[2]);
                            break;
                        case 6:
                            tvMsgLine06.setGravity(inGravity);
                            tvMsgLine06.setText(parsemsg[2]);
                            break;
                        case 7:
                            tvMsgLine07.setGravity(inGravity);
                            tvMsgLine07.setText(parsemsg[2]);
                            break;
                        case 8:
                            tvMsgLine08.setGravity(inGravity);
                            tvMsgLine08.setText(parsemsg[2]);
                            break;
                        case 9:
                            tvMsgLine09.setGravity(inGravity);
                            tvMsgLine09.setText(parsemsg[2]);
                            break;
                        case 10:
                            tvMsgLine10.setGravity(inGravity);
                            tvMsgLine10.setText(parsemsg[2]);
                            break;
                        case 11:
                            tvMsgLine11.setGravity(inGravity);
                            tvMsgLine11.setText(parsemsg[2]);
                            break;
                        case 12:
                            tvMsgLine12.setGravity(inGravity);
                            tvMsgLine12.setText(parsemsg[2]);
                            break;
                        case 13:
                            tvMsgLine13.setGravity(inGravity);
                            tvMsgLine13.setText(parsemsg[2]);
                            break;
                        case 14:
                            tvMsgLine14.setGravity(inGravity);
                            tvMsgLine14.setText(parsemsg[2]);
                            break;
                        case 15:
                            tvMsgLine15.setGravity(inGravity);
                            tvMsgLine15.setText(parsemsg[2]);
                            break;
                        case 16:
                            tvMsgLine16.setGravity(inGravity);
                            tvMsgLine16.setText(parsemsg[2]);
                            break;
                    }
                }

                setMsg();

                System.out.println("end displayMultipleMsg");
            }
        });


        return "OK";
    }


    public String printbyXY(int x, int y, String disp_msg)
    {
        setContentView(R.layout.displaytextmessage);

        tvMsgLine01 = (TextView) findViewById(R.id.msg_text_01);
        tvMsgLine02 = (TextView) findViewById(R.id.msg_text_02);
        tvMsgLine03 = (TextView) findViewById(R.id.msg_text_03);
        tvMsgLine04 = (TextView) findViewById(R.id.msg_text_04);
        tvMsgLine05 = (TextView) findViewById(R.id.msg_text_05);
        tvMsgLine06 = (TextView) findViewById(R.id.msg_text_06);
        tvMsgLine07 = (TextView) findViewById(R.id.msg_text_07);
        tvMsgLine08 = (TextView) findViewById(R.id.msg_text_08);
        tvMsgLine09 = (TextView) findViewById(R.id.msg_text_09);
        tvMsgLine10 = (TextView) findViewById(R.id.msg_text_10);
        tvMsgLine11 = (TextView) findViewById(R.id.msg_text_11);
        tvMsgLine12 = (TextView) findViewById(R.id.msg_text_12);
        tvMsgLine13 = (TextView) findViewById(R.id.msg_text_13);
        tvMsgLine14 = (TextView) findViewById(R.id.msg_text_14);
        tvMsgLine15 = (TextView) findViewById(R.id.msg_text_15);
        tvMsgLine16 = (TextView) findViewById(R.id.msg_text_16);

        System.out.println("start printbyXY");
        System.out.println(disp_msg);

        inGravity = Gravity.NO_GRAVITY;
        x=x-1;


        getMsg();
        System.out.printf("inGravity[%d]\n", inGravity);

        switch (y) {
            case 1:
                tvMsgLine01.setGravity(inGravity);
                tvMsgLine01.setPadding(36*x,0,0,0);
                tvMsgLine01.setText(disp_msg);
                inPaddingLeft[1]=36*x;

                break;
            case 2:
                tvMsgLine02.setGravity(inGravity);
                tvMsgLine02.setPadding(36*x,0,0,0);
                tvMsgLine02.setText(disp_msg);
                inPaddingLeft[2]=36*x;
                break;
            case 3:
                tvMsgLine03.setGravity(inGravity);
                tvMsgLine03.setPadding(36*x,0,0,0);
                tvMsgLine03.setText(disp_msg);
                inPaddingLeft[3]=36*x;
                break;
            case 4:
                tvMsgLine04.setGravity(inGravity);
                tvMsgLine04.setPadding(36*x,0,0,0);
                tvMsgLine04.setText(disp_msg);
                inPaddingLeft[4]=36*x;
                break;
            case 5:
                tvMsgLine05.setGravity(inGravity);
                tvMsgLine05.setPadding(36*x,0,0,0);
                tvMsgLine05.setText(disp_msg);
                inPaddingLeft[5]=36*x;
                break;
            case 6:
                tvMsgLine06.setGravity(inGravity);
                tvMsgLine06.setPadding(36*x,0,0,0);
                tvMsgLine06.setText(disp_msg);
                inPaddingLeft[6]=36*x;
                break;
            case 7:
                tvMsgLine07.setGravity(inGravity);
                tvMsgLine07.setPadding(36*x,0,0,0);
                tvMsgLine07.setText(disp_msg);
                inPaddingLeft[7]=36*x;
                break;
            case 8:
                tvMsgLine08.setGravity(inGravity);
                tvMsgLine08.setPadding(36*x,0,0,0);
                tvMsgLine08.setText(disp_msg);
                inPaddingLeft[8]=36*x;
                break;
            case 9:
                tvMsgLine09.setGravity(inGravity);
                tvMsgLine09.setPadding(36*x,0,0,0);
                tvMsgLine09.setText(disp_msg);
                inPaddingLeft[9]=36*x;
                break;
            case 10:
                tvMsgLine10.setGravity(inGravity);
                tvMsgLine10.setPadding(36*x,0,0,0);
                tvMsgLine10.setText(disp_msg);
                inPaddingLeft[10]=36*x;
                break;
            case 11:
                tvMsgLine11.setGravity(inGravity);
                tvMsgLine11.setPadding(36*x,0,0,0);
                tvMsgLine11.setText(disp_msg);
                inPaddingLeft[11]=36*x;
                break;
            case 12:
                tvMsgLine12.setGravity(inGravity);
                tvMsgLine12.setPadding(36*x,0,0,0);
                tvMsgLine12.setText(disp_msg);
                inPaddingLeft[12]=36*x;
                break;
            case 13:
                tvMsgLine13.setGravity(inGravity);
                tvMsgLine13.setPadding(36*x,0,0,0);
                tvMsgLine13.setText(disp_msg);
                inPaddingLeft[13]=36*x;
                break;
            case 14:
                tvMsgLine14.setGravity(inGravity);
                tvMsgLine14.setPadding(36*x,0,0,0);
                tvMsgLine14.setText(disp_msg);
                inPaddingLeft[14]=36*x;
                break;
            case 15:
                tvMsgLine15.setGravity(inGravity);
                tvMsgLine15.setPadding(36*x,0,0,0);
                tvMsgLine15.setText(disp_msg);
                inPaddingLeft[15]=36*x;
                break;
            case 16:
                tvMsgLine16.setGravity(inGravity);
                tvMsgLine16.setPadding(36*x,0,0,0);
                tvMsgLine16.setText(disp_msg);
                inPaddingLeft[16]=36*x;
                break;
        }

        setMsg();

        System.out.println("end displayMultipleMsg");
        return "DISP_MSG_OK";
    }
/*
    public void GetMainScreen(){

        //TINE:  mcc requirement

        final String MenuBuff;//, AppHeaderBuff;
        String TrxMenuItems, AppDetail1="", AppDetail2="", TrxImageId, AppDetail;
        String MainHeader;
        String[] TrxTitle = new String[100];
        String[] AppHeader = new String[100];
        int inLoop=0;

        Log.i("initDatas", "START!!!");

        AppHeaderBuff = GetAppHeaderDetails();
        MainHeader = AppHeaderBuff;
        Log.i("TINE", AppHeaderBuff);
        AppHeader = MainHeader.split(" \n");
        int msgcnt = AppHeader.length;

        //MenuBuff = ReadTrxMenu();

        //MenuBuff = GetTrxTypes();

        setContentView(R.layout.mcc_activity_main);

        TextView app_version;
        TextView merchant;
        TextView tprofile;
        Button btn_start;

        // Show status bar
        getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);

        //String[] dispmsginfo = dispmsg.split("\\|");
        //int msgcnt = dispmsginfo.length;

        app_version = (TextView) findViewById(R.id.app_version);
        merchant = (TextView) findViewById(R.id.merchant);
        tprofile = (TextView) findViewById(R.id.tprofile);
        btn_start = (Button) findViewById(R.id.btn_start);

        for (int inIdx = 0; inIdx < msgcnt; inIdx++) {
            //System.out.println("split msg [" + inIdx + "][" + dispmsginfo[inIdx] + "]");
            switch (inIdx) {
                case 0:
                    app_version.setText(AppHeader[inIdx]);
                    break;
                case 1:
                    merchant.setText(AppHeader[inIdx]);
                    break;
                case 2:
                    AppDetail1 = "MID: "+AppHeader[inIdx];
                    Log.i("TINE", AppDetail1);
                    break;
                case 3:
                    AppDetail2 = "TID: "+AppHeader[inIdx];
                    Log.i("TINE", AppDetail2);
                    break;
            }
        }

        AppDetail = AppDetail1 + "  |  " + AppDetail2;
        tprofile.setText(AppDetail);


        btn_start.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Log.i("Castles", "GetMainScreen");
                new Thread() {
                    @Override
                    public void run() {
                        try {
                            AppStart();

                            final Thread thread = new Thread(new Runnable(){

                                @Override
                                public void run() {
                                    Message message = new Message();
                                    message.what = 1;
                                    Log.d("handler.sendMessage",  "TINE:  SALE FINAL THREAD test2");
                                    handler.sendMessageDelayed(message, 100);
                                    //handler.sendMessage(message);
                                }

                            });

                            thread.start();


                        } catch (Exception e) {

                        }
                    }
                }.start();

            }
        });

        Log.i("GetMainScreen", "---END---");

    }
*/
    public void JNIGetMenu(){
        final Thread thread = new Thread(new Runnable() {

            @Override
            public void run() {

				idlecardentry_status= 100;
                if(false)//To handle ecr indenpendly as sub app
                {
                    Log.i(TAG, "not main app, so run ecr MainActivity finish: ");
                    MainActivity.this.finish();
                    int pid = Process.myPid();
                    Log.i(TAG, "Process pid: " + pid);
//                                    Process.killProcess(pid);
                    System.exit(0);
                }
                else
                {
                    Message message = new Message();
                    message.what = 1;
                    Log.d("handler.sendMessage", "sale");
                    handler.sendMessageDelayed(message, 100);
                }

            }

        });

        thread.start();
		
        // Message message = new Message();
        //  message.what = 1;
        // Log.d("handler.sendMessage", "sale");
        // handler.sendMessageDelayed(message, 100);
	}

    public void GetMenu(){

        final String MenuBuff;//, AppHeaderBuff;
        String TrxMenuItems, AppDetail1="", AppDetail2="", TrxImageId, AppDetail, Networknm;
        String MainHeader;
        String[] TrxTitle = new String[100];
        String[] AppHeader = new String[100];
        int inLoop=0;
        String rstatus="0";
        String idleMode = "0";


        fSTART = false;
        idlecardentry_status = 0;
        szMSR_status="MSR_NO";


        //CtSystem system = new CtSystem();
        //system.setNavBarIcon(false,false,false);

		final PowerManager pm2 = (PowerManager) getSystemService(Context.POWER_SERVICE);

        if(receipt_bmp_file != null) {
            receipt_bmp_file = null;        //tine:  clear content
            Log.i("Saturn Main", "receipt_bmp_file cleared...");
        }

        if(myBitmap != null) {
            myBitmap.recycle();
            myBitmap = null;                //tine:  clear content
            Log.i("Saturn Main", "myBitmap cleared...");
        }

        Log.i("AAA shutdown", "Reboot " + Reboot);


        if(Reboot.equals("REBOOT_YES")) {
            new Thread() {
                @Override
                public void run() {
                    try {
                        Log.i("shutdown", "shutdown START");
                        CtSystem system = new CtSystem();
                        byte bmode = 0;// shutdown
                        Log.d("PATRICK", String.format("return = %X", system.shutdown(bmode)));
                        //MainActivity.this.finish();
                    } catch (Exception e) {
                        Log.d("Castles", "SHUTDOWN encountered ERROR");
                    }
                }
            }.start();
        } else {
            //vdClearFile("/data/data/com.Source.S1_MCC.MCC/DYNAMICMENU120.S3DB");
			
            Log.i("saturn ", "GetMenu - START");
            Log.i("saturn", "fStart " +fSTART + "idlecardentry_status " + idlecardentry_status);
            GetAppInfor getAppInfor = new GetAppInfor(mContext);
            String currentSignature = getAppInfor.getInstalledAppHashCode();
            Log.i("currentSignature", "" + currentSignature);
            //Toast.makeText(getApplicationContext(), "currentSignature " + currentSignature, Toast.LENGTH_SHORT).show();


            AppHeaderBuff = GetAppHeaderDetails();

            MainHeader = AppHeaderBuff;
            Log.i("TINE", AppHeaderBuff);
            AppHeader = MainHeader.split(" \n");
            int msgcnt = AppHeader.length;

            OperatorHolder operatorHolder = new OperatorHolder(this);
            WifiManager wifiManager = (WifiManager) getApplicationContext().getSystemService(Context.WIFI_SERVICE);
            WifiInfo info = wifiManager.getConnectionInfo ();
            String ssid  = info.getSSID();

            View decorView = getWindow().getDecorView();
            //decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_LAYOUT_STABLE);
            decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                 | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);

            inMENUTYPE = 0;
            if(msgcnt > 6) {
                if(AppHeader[6].equals("MCCPAY")){
					
                    setContentView(R.layout.dmenu_activity_main_2);
                }
                if (AppHeader[6].equals("CASHOUT")) {
                    setContentView(R.layout.dmenu_activity_main_3);
                }

				
                if (AppHeader[6].equals("COMPLETEQR")) {
					inMENUTYPE = 1;
                    setContentView(R.layout.dmenu_activity_main_4);
                }

                
                if (AppHeader[6].equals("QRPH")) {
                    setContentView(R.layout.dmenu_activity_main_5);
                }
				
            }
            else
                setContentView(R.layout.dmenu_activity_main);

            app_version = (TextView) findViewById(R.id.main_app_version);
            merchant = (TextView) findViewById(R.id.main_merchant);
            tprofile = (TextView) findViewById(R.id.main_tprofile);
            btn_start = (Button) findViewById(R.id.btn_start);
            carrier_name = (TextView) findViewById(R.id.main_NetworkCarrier);
            idlemsgbox = (TextView) findViewById(R.id.msg_main1);
            btn_main_here = (Button) findViewById(R.id.btn_here);


            for (int inIdx = 0; inIdx < msgcnt; inIdx++) {
                //System.out.println("split msg [" + inIdx + "][" + dispmsginfo[inIdx] + "]");
                switch (inIdx) {
                    case 0:
                        app_version.setText(AppHeader[inIdx]);
                        break;
                    case 1:
                        merchant.setText(AppHeader[inIdx]);
                        break;
                    case 2:
                        AppDetail1 = "MID: "+AppHeader[inIdx];
                        Log.i("TINE", AppDetail1);
                        break;
                    case 3:
                        AppDetail2 = "TID: "+AppHeader[inIdx];
                        Log.i("TINE", AppDetail2);
                        break;
                    case 4:
                        rstatus = AppHeader[inIdx];
                        break;
                    case 5:
                        idleMode = AppHeader[inIdx];
                        break;
                }
            }

            AppDetail = AppDetail1 + "  |  " + AppDetail2;
            tprofile.setText(AppDetail);

            if(idleMode.equals("1")) {
                btn_main_here.setVisibility(View.GONE);
                idlemsgbox.setText("Please insert/swipe card or \n press START button to begin");
            } else if(idleMode.equals("2")) {
                btn_main_here.setVisibility(View.VISIBLE);
                idlemsgbox.setText("Please insert/swipe card or \n press HERE for amount entry");
            }

			Log.i("AAA Networkname", "SSID: " + ssid);

            if (ssid.equals("<unknown ssid>") || (ssid.length() <=0))
                Networknm = "Network: " + operatorHolder.getOperatorName();
            else
                Networknm = "Network: " + ssid;

            if (Networknm.equals("Network: "))
                Networknm = "Network: NOT CONNECTED";

            Log.i("AAA Networkname", "Networknm: " + Networknm);
            carrier_name.setText(Networknm);

            mPager = (ViewPager) findViewById(R.id.viewpager);

            //初始化数据源
            initDatas(idleMode);
            inflater = LayoutInflater.from(this);
            //总的页数=总数/每页数量，并取整
            pageCount = (int) Math.ceil(mDatas.size() * 1.0 / pageSize);
            mPagerList = new ArrayList<View>();
            for (int i = 0; i < pageCount; i++) {
                //每个页面都是inflate出一个新实例
                GridView gridView = (GridView) inflater.inflate(R.layout.gridview, mPager, false);
                gridView.setAdapter(new GridViewAdapter(this, mDatas, i, pageSize));
                mPagerList.add(gridView);


                gridView.setOnItemClickListener(new AdapterView.OnItemClickListener() {


                    @Override
                    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                        int pos = position + curIndex * pageSize;
                        //Toast.makeText(MainActivity.this, mDatas.get(pos).getName(), Toast.LENGTH_SHORT).show();

                        jni_api.REG_CB_CTOS(jni_cb_ctos);
                        setContentView(R.layout.activity_disp);
                        lsv_LCDTPrint = (ListView) findViewById(R.id.lsv_LCDTPrint);
                        edtLog = (EditText) findViewById(R.id.edtLog);
                        adapter = new ArrayAdapter(MainActivity.this, R.layout.listitem, list){

                            @NonNull
                            @Override
                            public View getView(int position, @Nullable View convertView, @NonNull ViewGroup parent) {
                                View view =  super.getView(position, convertView, parent);


                                TextView textView = (TextView)view;

//                            Log.d("Position", String.valueOf(position));
//                            Log.d("Padding", String.valueOf(addPadding[position]));

                                if(addPadding[position] != 0)
                                    textView.setPadding(36*addPadding[position],0,0,0);

//                            Log.d("DisplayMode", displayMode[position]);

                                if (displayMode[position].equals("0")) {
                                    textView.setGravity(Gravity.LEFT);
                                } else if (displayMode[position].equals("1")) {
                                    textView.setGravity(Gravity.CENTER);
                                } else if (displayMode[position].equals("2")) {
                                    textView.setGravity(Gravity.RIGHT);
                                }


                                return view;

                            }
                        };

                        if (mDatas.get(pos).getName() == "START") {
                            Log.i("Castles", "set app to START");
                            idlecardentry_status = 3;
                        /*
                        new Thread() {
                            @Override
                            public void run() {
                                try {

                                    jni_api_offline_pin.REG_CB_EMV(jni_cb_emv);
                                    fSTART = true;
                                    AppStart();
                                    //inCTOSS_OnlinePinTest();

                                    final Thread thread = new Thread(new Runnable(){

                                        @Override
                                        public void run() {
                                            Message message = new Message();
                                            message.what = 1;
                                            Log.d("handler.sendMessage",  "test");
                                            handler.sendMessageDelayed(message, 100);
                                        }

                                    });

                                    thread.start();
                                } catch (Exception e) {

                                }
                            }
                        }.start();
                        */
                        }


                    }
                });

            }

            //设置适配器
            mPager.setAdapter(new ViewPagerAdapter(mPagerList));
    /*
           ctlsdemo = TemporaryData.getSharedPreferences().getInt(TemporaryData.CTLSDEMO, 0);
           if(ctlsdemo == 1) {
               mPager.setOnClickListener(this);

               mPager.performClick();
           }
    */
            //设置圆点
            setOvalLayout();
//       btnauto.performClick();

            if(rstatus.equals("0"))
            {
                Log.i("Castles", "Pending Registration");
                new Thread() {
                    @Override
                    public void run() {
                        try {
                            //jni_api.REG_CB_CTOS(jni_cb_ctos);
                            boolean fInterActive = true;
                            while (idlecardentry_status == 0)
                            {
                                fSTART = false;

								fInterActive = pm2.isInteractive();
								Log.i("saturn", "saturn interactive =  "+fInterActive);

								//check if sleep node;
								//if (fInterActive == false)
									//inDoAutoReboot();
                            }
                            fSTART = true;
                            MCCAppEntry();
                        } catch (Exception e) {

                        }
                    }
                }.start();
            }
            else if(rstatus.equals("1")) {
                Log.i("Castles", "Clear MSR card data from invalid swipe");
                fBlockTrans = false;
                //EcrCommandReceiver.isEcrProcessing = false;
                //CTOSS_MSRRead_inJava(); 
                //szMSR_status = "MSR_NO";
                Log.i("Castles", "Waiting for IDLE INSERT/SWIPE");
                new Thread() {
                    @Override
                    public void run() {
                        try {
                            //jni_api.REG_CB_CTOS(jni_cb_ctos);
                            fSTART = false;
                            btn_main_here.setOnClickListener(new View.OnClickListener() {
                                @Override
                                public void onClick(View v) {
                                    Log.i("Castles", "set app to idle amount entry");
                                    idlecardentry_status = 4;
                                }
                            });

                            while (idlecardentry_status == 0) {

                                Thread.sleep(100); // a small delay to fix 2nd txn online PIN terminal hang issue. happen on S1F2. 2020/09/02

                                if(EcrCommandReceiver.isEcrProcessing){
                                    Log.i(TAG, "CTOSS_MSRRead_inJava isEcrProcessing " + EcrCommandReceiver.isEcrProcessing);
                                    return;
                                }

                                //Log.i("saturn", "saturn check for card entry");
                                //Log.d("AAA", "fBlockTrans: " + fBlockTrans);
                                if (fBlockTrans != true) {
                                    szMSR_status = CTOSS_MSRRead_inJava();
                                }
                                //Log.i("saturn", "saturn CTOSS_MSRRead_inJava"+szMSR_status);
                                if (szMSR_status.equals("MSR_OK")) {
                                    idlecardentry_status = 1;
                                    Log.i("saturn ", "saturn idle card swipe detected");
                                }

                                //szICC_status = CTOSS_SCStatus_inJava();
                                if (szMSR_status.equals("ICC_OK")) {
                                    idlecardentry_status = 1;
                                    // Log.i("saturn ", "saturn idle card insert detected");
                                }

                                if (szMSR_status.equals("SETTLE")) {
                                    idlecardentry_status = 2;
                                    //Log.i("saturn ", "trigger AutoSettlement");
                                }

                                if (fSTART) {
                                    break;
                                }
                            }

                            //Log.d("TINE", "szMSR_status: " + szMSR_status);
                            //Log.d("TINE", "idlecardentry_status: " + idlecardentry_status);
                            MCCAppEntry();

                        } catch (Exception e) {

                        }
                    }
                }.start();
            }
        }

    }


    @RequiresApi(api = Build.VERSION_CODES.M)
    public String displayImage(int x, int y, String msg_img)
    {
        setContentView(R.layout.displayimage);
        Log.i("Castles", "Start displayImage");
        Log.i("Castles", "x:"+x+"y:"+y+"name:"+msg_img);
        // System.out.println("Start displayImage");
        // System.out.println("x:"+x+"y:"+y+"name:"+msg_img);


        int resid = getResources().getIdentifier(msg_img,"drawable", getApplicationContext().getPackageName());

        imageView = (ImageView) findViewById(R.id.imageView);
        imageView.setImageResource(resid);

/*        LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(400, 400);
        imageView.setLayoutParams(params);
        imageView.setBackgroundColor(getColor(R.color.colorAccent));*/
        ViewGroup.MarginLayoutParams margin = new ViewGroup.MarginLayoutParams(imageView.getLayoutParams());
        margin.leftMargin = x;
        margin.topMargin = y;
        RelativeLayout.LayoutParams layoutParams = new RelativeLayout.LayoutParams(margin);
//        layoutParams.height = 400;//set the height
//        layoutParams.width = 400; //set the width
        imageView.setLayoutParams(layoutParams);
        //imageView.setScaleType(ImageView.ScaleType.FIT_XY);

        Log.i("Castles", "End displayImage");

        //System.out.println("End displayImage");
        return "OK";
    }


    public void getMsg() {
        tvMsgLine01.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine01, null));
        tvMsgLine02.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine02, null));
        tvMsgLine03.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine03, null));
        tvMsgLine04.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine04, null));
        tvMsgLine05.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine05, null));
        tvMsgLine06.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine06, null));
        tvMsgLine07.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine07, null));
        tvMsgLine08.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine08, null));
        tvMsgLine09.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine09, null));
        tvMsgLine10.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine10, null));
        tvMsgLine11.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine11, null));
        tvMsgLine12.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine12, null));
        tvMsgLine13.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine13, null));
        tvMsgLine14.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine14, null));
        tvMsgLine15.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine15, null));
        tvMsgLine16.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine16, null));

        tvMsgLine01.setGravity(TemporaryData.getSharedPreferences().getInt(TemporaryData.Gravity01, 0));
        tvMsgLine02.setGravity(TemporaryData.getSharedPreferences().getInt(TemporaryData.Gravity02, 0));
        tvMsgLine03.setGravity(TemporaryData.getSharedPreferences().getInt(TemporaryData.Gravity03, 0));
        tvMsgLine04.setGravity(TemporaryData.getSharedPreferences().getInt(TemporaryData.Gravity04, 0));
        tvMsgLine05.setGravity(TemporaryData.getSharedPreferences().getInt(TemporaryData.Gravity05, 0));
        tvMsgLine06.setGravity(TemporaryData.getSharedPreferences().getInt(TemporaryData.Gravity06, 0));
        tvMsgLine07.setGravity(TemporaryData.getSharedPreferences().getInt(TemporaryData.Gravity07, 0));
        tvMsgLine08.setGravity(TemporaryData.getSharedPreferences().getInt(TemporaryData.Gravity08, 0));
        tvMsgLine09.setGravity(TemporaryData.getSharedPreferences().getInt(TemporaryData.Gravity09, 0));
        tvMsgLine10.setGravity(TemporaryData.getSharedPreferences().getInt(TemporaryData.Gravity10, 0));
        tvMsgLine11.setGravity(TemporaryData.getSharedPreferences().getInt(TemporaryData.Gravity11, 0));
        tvMsgLine12.setGravity(TemporaryData.getSharedPreferences().getInt(TemporaryData.Gravity12, 0));
        tvMsgLine13.setGravity(TemporaryData.getSharedPreferences().getInt(TemporaryData.Gravity13, 0));
        tvMsgLine14.setGravity(TemporaryData.getSharedPreferences().getInt(TemporaryData.Gravity14, 0));
        tvMsgLine15.setGravity(TemporaryData.getSharedPreferences().getInt(TemporaryData.Gravity15, 0));
        tvMsgLine16.setGravity(TemporaryData.getSharedPreferences().getInt(TemporaryData.Gravity16, 0));

        tvMsgLine01.setPadding(TemporaryData.getSharedPreferences().getInt(TemporaryData.PaddingLeft01, 0), 0, 0, 0);
        tvMsgLine02.setPadding(TemporaryData.getSharedPreferences().getInt(TemporaryData.PaddingLeft02, 0), 0, 0, 0);
        tvMsgLine03.setPadding(TemporaryData.getSharedPreferences().getInt(TemporaryData.PaddingLeft03, 0), 0, 0, 0);
        tvMsgLine04.setPadding(TemporaryData.getSharedPreferences().getInt(TemporaryData.PaddingLeft04, 0), 0, 0, 0);
        tvMsgLine05.setPadding(TemporaryData.getSharedPreferences().getInt(TemporaryData.PaddingLeft05, 0), 0, 0, 0);
        tvMsgLine06.setPadding(TemporaryData.getSharedPreferences().getInt(TemporaryData.PaddingLeft06, 0), 0, 0, 0);
        tvMsgLine07.setPadding(TemporaryData.getSharedPreferences().getInt(TemporaryData.PaddingLeft07, 0), 0, 0, 0);
        tvMsgLine08.setPadding(TemporaryData.getSharedPreferences().getInt(TemporaryData.PaddingLeft08, 0), 0, 0, 0);
        tvMsgLine09.setPadding(TemporaryData.getSharedPreferences().getInt(TemporaryData.PaddingLeft09, 0), 0, 0, 0);
        tvMsgLine10.setPadding(TemporaryData.getSharedPreferences().getInt(TemporaryData.PaddingLeft10, 0), 0, 0, 0);
        tvMsgLine11.setPadding(TemporaryData.getSharedPreferences().getInt(TemporaryData.PaddingLeft11, 0), 0, 0, 0);
        tvMsgLine12.setPadding(TemporaryData.getSharedPreferences().getInt(TemporaryData.PaddingLeft12, 0), 0, 0, 0);
        tvMsgLine13.setPadding(TemporaryData.getSharedPreferences().getInt(TemporaryData.PaddingLeft13, 0), 0, 0, 0);
        tvMsgLine14.setPadding(TemporaryData.getSharedPreferences().getInt(TemporaryData.PaddingLeft14, 0), 0, 0, 0);
        tvMsgLine15.setPadding(TemporaryData.getSharedPreferences().getInt(TemporaryData.PaddingLeft15, 0), 0, 0, 0);
        tvMsgLine16.setPadding(TemporaryData.getSharedPreferences().getInt(TemporaryData.PaddingLeft16, 0), 0, 0, 0);

    }

    public void setMsg(){

        SharedPreferences.Editor editor = TemporaryData.getSharedPreferences().edit();
        editor.putString(TemporaryData.DispLine01, tvMsgLine01.getText().toString());
        editor.putString(TemporaryData.DispLine02, tvMsgLine02.getText().toString());
        editor.putString(TemporaryData.DispLine03, tvMsgLine03.getText().toString());
        editor.putString(TemporaryData.DispLine04, tvMsgLine04.getText().toString());
        editor.putString(TemporaryData.DispLine05, tvMsgLine05.getText().toString());
        editor.putString(TemporaryData.DispLine06, tvMsgLine06.getText().toString());
        editor.putString(TemporaryData.DispLine07, tvMsgLine07.getText().toString());
        editor.putString(TemporaryData.DispLine08, tvMsgLine08.getText().toString());
        editor.putString(TemporaryData.DispLine09, tvMsgLine09.getText().toString());
        editor.putString(TemporaryData.DispLine10, tvMsgLine10.getText().toString());
        editor.putString(TemporaryData.DispLine11, tvMsgLine11.getText().toString());
        editor.putString(TemporaryData.DispLine12, tvMsgLine12.getText().toString());
        editor.putString(TemporaryData.DispLine13, tvMsgLine13.getText().toString());
        editor.putString(TemporaryData.DispLine14, tvMsgLine14.getText().toString());
        editor.putString(TemporaryData.DispLine15, tvMsgLine15.getText().toString());
        editor.putString(TemporaryData.DispLine16, tvMsgLine16.getText().toString());

        editor.putInt(TemporaryData.Gravity01, tvMsgLine01.getGravity());
        editor.putInt(TemporaryData.Gravity02, tvMsgLine02.getGravity());
        editor.putInt(TemporaryData.Gravity03, tvMsgLine03.getGravity());
        editor.putInt(TemporaryData.Gravity04, tvMsgLine04.getGravity());
        editor.putInt(TemporaryData.Gravity05, tvMsgLine05.getGravity());
        editor.putInt(TemporaryData.Gravity06, tvMsgLine06.getGravity());
        editor.putInt(TemporaryData.Gravity07, tvMsgLine07.getGravity());
        editor.putInt(TemporaryData.Gravity08, tvMsgLine08.getGravity());
        editor.putInt(TemporaryData.Gravity09, tvMsgLine09.getGravity());
        editor.putInt(TemporaryData.Gravity10, tvMsgLine10.getGravity());
        editor.putInt(TemporaryData.Gravity11, tvMsgLine11.getGravity());
        editor.putInt(TemporaryData.Gravity12, tvMsgLine12.getGravity());
        editor.putInt(TemporaryData.Gravity13, tvMsgLine13.getGravity());
        editor.putInt(TemporaryData.Gravity14, tvMsgLine14.getGravity());
        editor.putInt(TemporaryData.Gravity15, tvMsgLine15.getGravity());
        editor.putInt(TemporaryData.Gravity16, tvMsgLine16.getGravity());

        editor.putInt(TemporaryData.PaddingLeft01, inPaddingLeft[1]);
        editor.putInt(TemporaryData.PaddingLeft02, inPaddingLeft[2]);
        editor.putInt(TemporaryData.PaddingLeft03, inPaddingLeft[3]);
        editor.putInt(TemporaryData.PaddingLeft04, inPaddingLeft[4]);
        editor.putInt(TemporaryData.PaddingLeft05, inPaddingLeft[5]);
        editor.putInt(TemporaryData.PaddingLeft06, inPaddingLeft[6]);
        editor.putInt(TemporaryData.PaddingLeft07, inPaddingLeft[7]);
        editor.putInt(TemporaryData.PaddingLeft08, inPaddingLeft[8]);
        editor.putInt(TemporaryData.PaddingLeft09, inPaddingLeft[9]);
        editor.putInt(TemporaryData.PaddingLeft10, inPaddingLeft[10]);
        editor.putInt(TemporaryData.PaddingLeft11, inPaddingLeft[11]);
        editor.putInt(TemporaryData.PaddingLeft12, inPaddingLeft[12]);
        editor.putInt(TemporaryData.PaddingLeft13, inPaddingLeft[13]);
        editor.putInt(TemporaryData.PaddingLeft14, inPaddingLeft[14]);
        editor.putInt(TemporaryData.PaddingLeft15, inPaddingLeft[15]);
        editor.putInt(TemporaryData.PaddingLeft16, inPaddingLeft[16]);
        editor.apply();

    }


    /**
     * 为了得到传回的数据，必须在前面的Activity中（指MainActivity类）重写onActivityResult方法
     *
     * requestCode 请求码，即调用startActivityForResult()传递过去的值
     * resultCode 结果码，结果码用于标识返回数据来自哪个新Activity
     */
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        String result = data.getExtras().getString("result");//得到新Activity 关闭后返回的数据
        Log.i("onActivityResult", result);
    }

    public void MCCAppEntry ()
    {
        if (idlecardentry_status == 1) {
            //idlecardentry_status = 0;
            //inCTOSS_Idle_Sale();

            Log.i("Castles", "Process WAVE_SALE - CardEntry:  MSR");

            new Thread() {
                @Override
                public void run() {
                    try {
                        jni_api.REG_CB_CTOS(jni_cb_ctos);
                        inCTOSS_Idle_Sale();

                        final Thread thread = new Thread(new Runnable() {

                            @Override
                            public void run() {
                                Message message = new Message();
                                message.what = 1;
                                Log.d("handler.sendMessage", "test");
                                handler.sendMessageDelayed(message, 100);
                            }

                        });

                        thread.start();
                    } catch (Exception e) {

                    }
                }
            }.start();

        }

        if (idlecardentry_status == 2) {
            //idlecardentry_status = 0;
            //inCTOSS_Idle_Sale();

            Log.i("Castles", "Process WAVE_SALE - CardEntry:  MSR");

            new Thread() {
                @Override
                public void run() {
                    try {
                        jni_api.REG_CB_CTOS(jni_cb_ctos);
						intAutoSettlement();

                        final Thread thread = new Thread(new Runnable() {

                            @Override
                            public void run() {
                                Message message = new Message();
                                message.what = 1;
                                Log.d("handler.sendMessage", "test");
                                handler.sendMessageDelayed(message, 100);
                            }

                        });
                        thread.start();
                    } catch (Exception e) {

                    }
                }
            }.start();

        }

        if(idlecardentry_status == 3)
        {
            Log.i("Castles", "START");
            new Thread() {
                @Override
                public void run() {
                    try {
                        jni_api.REG_CB_CTOS(jni_cb_ctos);
                        jni_api_offline_pin.REG_CB_EMV(jni_cb_emv);
                        fSTART = true;
                        AppStart();
                        //inCTOSS_OnlinePinTest();

                        final Thread thread = new Thread(new Runnable(){

                            @Override
                            public void run() {
                                Message message = new Message();
                                message.what = 1;
                                Log.d("handler.sendMessage",  "test");
                                handler.sendMessageDelayed(message, 100);
                            }

                        });

                        thread.start();
                    } catch (Exception e) {

                    }
                }
            }.start();
        }

        if(idlecardentry_status == 4)
        {
            Log.i("Castles", "Process WAVE_SALE - Idle Amount Entry");

            new Thread() {
                @Override
                public void run() {
                    try {
                        jni_api.REG_CB_CTOS(jni_cb_ctos);
                        inCTOSS_Idle_Sale();

                        final Thread thread = new Thread(new Runnable() {

                            @Override
                            public void run() {
                                Message message = new Message();
                                message.what = 1;
                                Log.d("handler.sendMessage", "test");
                                handler.sendMessageDelayed(message, 100);
                            }

                        });

                        thread.start();
                    } catch (Exception e) {

                    }
                }
            }.start();
        }

		if(idlecardentry_status == 5)
        {
            Log.i("Castles", "Process inCTOSS_Settings");

            new Thread() {
                @Override
                public void run() {
                    try {
                        jni_api.REG_CB_CTOS(jni_cb_ctos);
                        inCTOSS_Settings();
				
                        inSystemPanel=0;
						if(inCTOSS_GetEnvInt("SYSTEM_PANEL") == 1)
						{
							inCTOSS_PutEnvInt("SYSTEM_PANEL", 0);
						    inSystemPanel=1;
						}

                        inScanWIFI=0;
                        if(inCTOSS_GetEnvInt("SET_WIFI") == 1)
                        {
                            inCTOSS_PutEnvInt("SET_WIFI", 0);
                            inScanWIFI=1;
                        }
                        final Thread thread = new Thread(new Runnable() {

                            @Override
                            public void run() {
                                Message message = new Message();
                                message.what = 1;
                                Log.d("handler.sendMessage", "test");
                                handler.sendMessageDelayed(message, 100);
                            }

                        });

                        thread.start();
                    } catch (Exception e) {

                    }
                }
            }.start();
        }
		
		if(idlecardentry_status == 6)
        {
            /*
            String baEncryptedData;
			Encryption mEncryption = new Encryption();
            baEncryptedData=mEncryption.TLE_MAC("02003020078020C0020600000000000000010000028600520001002100375464981659924293D24092017120000000000F333032383530303730303030303130303032383530323101475F2A0206085F340101820239008407A0000000041010950580000080009A032305239C01009F02060000000001009F03060000000000009F090200029F10120110A00001220000000000000000000000FF9F1A0206089F1E0831323334353637389F2608C6B784D0D89D9B079F2701809F3303E008C89F34031F03029F3501229F360207EB9F370442DDA5C59F4104000000150006303030303730004700173138303030313538323033363131393834002631393030303030303030303030303030303030303030303030308000","6FB79CA8850EB389986E253018D92DB6");
            System.out.println("baEncryptedData TLE_MAC:" + baEncryptedData);
            Log.i("Encryption", "3.encrypted_data:");
            baEncryptedData=mEncryption.TLE_Decrypt("633EF7320E14C6A949FD1B59B206F7881B2A51707224C8FAF12B419B65A4087807AB658DE123B4454D6CB1C394ABEB8BCCFD83ACD577B33D917BF7B6A8DB7D04FF58A6B42655A8D26749E1F22A896B7A62861FC95D54FD9017830F274EA1CBBE7694B9C6150C9F207107EB15938C9812C1C733685AAD6EB5EFBE6620E4767C1916265890F9CCEFA95D751861B22E00D4", "986E253018D92DB66FB79CA8850EB389");
            System.out.println("baEncryptedData TLE_Decrypt:" + baEncryptedData);
            Log.i("Encryption", "4.encrypted_data:");
            */

            /*
            int inResult=1;
            Log.i("TcpClient", "1.Process TcpClient");
            TcpClient mTcpClient = new TcpClient();
            mTcpClient.Connect("192.168.1.172", 5041);
            Log.i("TcpClient", "2.Process inCTOSS_Reports:"+mTcpClient.bConnected);

            if(mTcpClient.bConnected == true)
            {
                inResult=mTcpClient.Send("0069600300000008002020010000C0000292000000020203005858585858585858585858585858585858585858585858001900173138303030313538323132343231383833");

                if(inResult == 0)
                {
                    Log.i("TcpClient", "RECEIVING");
                    //mTcpClient.Receive();
                }
            }
            mTcpClient.stopClient();
            Log.i("TcpClient", "2.Process TcpClient");

             */


            new Thread() {
                @Override
                public void run() {
                    try {
                        jni_api.REG_CB_CTOS(jni_cb_ctos);
                        inCTOSS_Reports();
                        //mTcpClient.stopClient();
                        inScanWIFI=0;
                        if(inCTOSS_GetEnvInt("SET_WIFI") == 1)
                        {
                            inCTOSS_PutEnvInt("SET_WIFI", 0);
						    inScanWIFI=1;	
                        }
						
                        final Thread thread = new Thread(new Runnable() {

                            @Override
                            public void run() {
                                Message message = new Message();
                                message.what = 1;
                                Log.d("handler.sendMessage", "test");
                                handler.sendMessageDelayed(message, 100);
                            }

                        });

                        thread.start();
                    } catch (Exception e) {

                    }
                }
            }.start();
        }

		if(idlecardentry_status == 7)
        {
            Log.i("Castles", "Process inCTOSS_Settlement");

            new Thread() {
                @Override
                public void run() {
                    try {
                        jni_api.REG_CB_CTOS(jni_cb_ctos);
                        inCTOSS_Settlement();

                        final Thread thread = new Thread(new Runnable() {

                            @Override
                            public void run() {
                                Message message = new Message();
                                message.what = 1;
                                Log.d("handler.sendMessage", "test");
                                handler.sendMessageDelayed(message, 100);
                            }

                        });

                        thread.start();
                    } catch (Exception e) {

                    }
                }
            }.start();
        }

		if(idlecardentry_status == 8)
        {
            Log.i("Castles", "Process inCTOSS_Credit");

            new Thread() {
                @Override
                public void run() {
                    try {
                        jni_api.REG_CB_CTOS(jni_cb_ctos);
                        inCTOSS_Credit();

                        final Thread thread = new Thread(new Runnable() {

                            @Override
                            public void run() {
                                Message message = new Message();
                                message.what = 1;
                                Log.d("handler.sendMessage", "test");
                                handler.sendMessageDelayed(message, 100);
                            }

                        });

                        thread.start();
                    } catch (Exception e) {

                    }
                }
            }.start();
        }
		
		if(idlecardentry_status == 9)
        {
            Log.i("Castles", "Process inCTOSS_Bancnet");

            new Thread() {
                @Override
                public void run() {
                    try {
                        jni_api.REG_CB_CTOS(jni_cb_ctos);
                        inCTOSS_Bancnet();

                        final Thread thread = new Thread(new Runnable() {

                            @Override
                            public void run() {
                                Message message = new Message();
                                message.what = 1;
                                Log.d("handler.sendMessage", "test");
                                handler.sendMessageDelayed(message, 100);
                            }

                        });

                        thread.start();
                    } catch (Exception e) {

                    }
                }
            }.start();
        }	

		if(idlecardentry_status == 10)
        {
            Log.i("Castles", "Process inCTOSS_PHQR");

            new Thread() {
                @Override
                public void run() {
                    try {
                        jni_api.REG_CB_CTOS(jni_cb_ctos);
                        inCTOSS_PHQR();

                        final Thread thread = new Thread(new Runnable() {

                            @Override
                            public void run() {
                                Message message = new Message();
                                message.what = 1;
                                Log.d("handler.sendMessage", "test");
                                handler.sendMessageDelayed(message, 100);
                            }

                        });

                        thread.start();
                    } catch (Exception e) {

                    }
                }
            }.start();
        }
		
		if(idlecardentry_status == 11)
        {
            Log.i("Castles", "Process inCTOSS_Wallet");

            new Thread() {
                @Override
                public void run() {
                    try {
                        jni_api.REG_CB_CTOS(jni_cb_ctos);
                        inCTOSS_Wallet();

                        final Thread thread = new Thread(new Runnable() {

                            @Override
                            public void run() {
                                Message message = new Message();
                                message.what = 1;
                                Log.d("handler.sendMessage", "test");
                                handler.sendMessageDelayed(message, 100);
                            }

                        });

                        thread.start();
                    } catch (Exception e) {

                    }
                }
            }.start();
        }

		if(idlecardentry_status == 12)
        {
            Log.i("Castles", "Process inCTOSS_Register");

            new Thread() {
                @Override
                public void run() {
                    try {
                        jni_api.REG_CB_CTOS(jni_cb_ctos);
                        inCTOSS_Register();

                        final Thread thread = new Thread(new Runnable() {

                            @Override
                            public void run() {
                                Message message = new Message();
                                message.what = 1;
                                Log.d("handler.sendMessage", "test");
                                handler.sendMessageDelayed(message, 100);
                            }

                        });

                        thread.start();
                    } catch (Exception e) {

                    }
                }
            }.start();
        }
    }

    public String GetAmountString(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;
			
        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();
                    //intent.setClass(MainActivity.this, GetAmount.class);
                    //intent.setClass(MainActivity.this, AmountEntryActivity.class);
                    intent.setClass(MainActivity.this, AmountEntryActivity.class);

                    //bundle.putString("amt_string", amt_string);
                    bundle.putString("pass_in_string", pass_in_string);
                    //bundle.putString("display", "AMOUNT  (PHP)");
                    bundle.putString("minlength", "1");
                    bundle.putString("maxlength", "9");
                    bundle.putString("type", "1");
                    //bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();


        /*
        Intent intent = new Intent();
        Bundle bundle = new Bundle();
        intent.setClass(MainActivity.this, GetAmount.class);

        bundle.putString("amt_string", amt_string);
        bundle.putString("amt_disp_msg", text);
        Log.i("amt_disp_msg", text);

        intent.putExtras(bundle);
        //startActivity(intent);
        startActivityForResult(intent, 0);

        Log.i("PATRICK", "startActivity");
        //finish();
        */


        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        //user_str = GetAmount.final_amt_string;
        user_str = AmountEntryActivity.final_amt_string;


        Log.i("PATRICK", user_str);
        Log.i("PATRICK", "FINISH 456");
        //pin_num = pin_num + "XXX";
//        Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
        return user_str;

    }

    public String GetPanString(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;

        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();
                    //intent.setClass(MainActivity.this, GetAmount.class);
                    intent.setClass(MainActivity.this, GetCardOptionsActivity.class);

                    //bundle.putString("amt_string", amt_string);
                    bundle.putString("pass_in_string", pass_in_string);
                    //bundle.putString("display", "AMOUNT  (PHP)");
                    //bundle.putString("minlength", "1");
                    //bundle.putString("maxlength", "10");
                    //bundle.putString("type", "1");
                    //bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        //user_str = GetAmount.final_amt_string;
        user_str = GetCardOptionsActivity.final_string;


        Log.i("PATRICK", user_str);
        Log.i("PATRICK", "FINISH 456");
        //pin_num = pin_num + "XXX";
//        Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
        return user_str;

    }

    public String UserInputString(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;


        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();
                    //intent.setClass(MainActivity.this, InputString.class);
                    intent.setClass(MainActivity.this, UserInputString.class);


                    //bundle.putString("PARAM_DISPLAY", "PASSWORD");
                    //bundle.putString("PARAM_MINLENGTH", "1");
                    //bundle.putString("PARAM_MAXLENGTH", "6");
                    //bundle.putString("PARAM_TYPE", "1");
                    //bundle.putString("PARAM_PASSWORD", "123456");

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        user_str = UserInputString.final_string;

        Log.i("PATRICK", user_str);
        Log.i("PATRICK", "FINISH 789");
        //pin_num = pin_num + "XXX";
//        Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
        return user_str;

    }
		public String UserConfirmMenu(String text) throws InterruptedException {
	
			String user_str="";
			pass_in_string = text;
	
			new Thread()
			{
				public void run()
				{
					try
					{
						Intent intent = new Intent();
						Bundle bundle = new Bundle();
						intent.setClass(MainActivity.this, ConfirmMenu.class);
	//					  intent.setClass(MainActivity.this, com.Source.S1_AMEX.AMEX.model.ConfirmMenu.class);
	
						bundle.putString("pass_in_string", pass_in_string);
						Log.i("pass_in_string", pass_in_string);
	
						intent.putExtras(bundle);
						startActivity(intent);
						//startActivityForResult(intent, 0);
					}
					catch(Exception e)
					{
						e.printStackTrace();
					}
				}
			}.start();
	
			MainActivity.LOCK.setCondition(false);
	
			synchronized (LOCK) {
				while (!LOCK.conditionMet()) {
					try {
						LOCK.wait();
					} catch (InterruptedException e) {
						Log.e(TAG, "Exception when waiting for condition", e);
						return "Exception";
					}
				}
			}
	
			user_str = ConfirmMenu.final_string;
	
			Log.i("PATRICK", user_str);
			Log.i("PATRICK", "FINISH 789");
			//pin_num = pin_num + "XXX";
	//		  Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
			return user_str;
	
		}

    public String UserConfirmMenu3(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;

        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();
                    intent.setClass(MainActivity.this, ConfirmMenu3.class);
                    //					  intent.setClass(MainActivity.this, com.Source.S1_AMEX.AMEX.model.ConfirmMenu.class);

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        user_str = ConfirmMenu3.final_string;

        Log.i("PATRICK", user_str);
        Log.i("PATRICK", "FINISH 789");
        //pin_num = pin_num + "XXX";
        //		  Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
        return user_str;

    }
    public String UserConfirmMenuTip(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;

        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();
                    intent.setClass(MainActivity.this, ConfirmMenuTip.class);
                    //intent.setClass(MainActivity.this, com.Source.S1_AMEX.AMEX.model.ConfirmMenu.class);

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        user_str = ConfirmMenuTip.final_string;

        Log.i("PATRICK", user_str);
        Log.i("PATRICK", "FINISH 789");
        //pin_num = pin_num + "XXX";
        //		  Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
        return user_str;

    }

    public String InputStringUI(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;


        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();
                    intent.setClass(MainActivity.this, S1InputString.class);
                    //intent.setClass(MainActivity.this, UserInputString.class);


                    //bundle.putString("PARAM_DISPLAY", "PASSWORD");
                    //bundle.putString("PARAM_MINLENGTH", "1");
                    //bundle.putString("PARAM_MAXLENGTH", "6");
                    //bundle.putString("PARAM_TYPE", "1");
                    //bundle.putString("PARAM_PASSWORD", "123456");

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        user_str = S1InputString.final_string;

        Log.i("PATRICK", user_str);
        Log.i("PATRICK", "FINISH S1InputString");
        //pin_num = pin_num + "XXX";
//        Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
        return user_str;

    }

    public String InputStringAlpha(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;


        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();
                    intent.setClass(MainActivity.this, InputAlpha.class);

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        user_str = InputAlpha.final_string;

        Log.i("TINE, user_str", user_str);
        Log.i("TINE, user_str", "FINISH InputStringAlpha");
        //pin_num = pin_num + "XXX";
//        Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
        return user_str;

    }



		public String UserConfirmMenu2(String text) throws InterruptedException {
	
			String user_str="";
			pass_in_string = text;
	
			new Thread()
			{
				public void run()
				{
					try
					{
						Intent intent = new Intent();
						Bundle bundle = new Bundle();
	
						intent.setClass(MainActivity.this, ConfirmMenu2.class);
	
						bundle.putString("pass_in_string", pass_in_string);
						Log.i("pass_in_string", pass_in_string);
	
						intent.putExtras(bundle);
						startActivity(intent);
						//startActivityForResult(intent, 0);
					}
					catch(Exception e)
					{
						e.printStackTrace();
					}
				}
			}.start();
	
			MainActivity.LOCK.setCondition(false);
	
			synchronized (LOCK) {
				while (!LOCK.conditionMet()) {
					try {
						LOCK.wait();
					} catch (InterruptedException e) {
						Log.e(TAG, "Exception when waiting for condition", e);
						return "Exception";
					}
				}
			}
	
			user_str = ConfirmMenu2.final_string;
	
			Log.i("PATRICK", user_str);
			Log.i("PATRICK", "FINISH 789");
			//pin_num = pin_num + "XXX";
	//		  Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
			return user_str;
		}

	public String BatchReviewUI(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;

        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();

                    intent.setClass(MainActivity.this, BatchReview.class);

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        user_str = BatchReview.final_string;

        Log.i("PATRICK", user_str);
        Log.i("PATRICK", "FINISH 789");
        //pin_num = pin_num + "XXX";
//        Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
        return user_str;
    }	

    public String UserConfirmOKMenu(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;

        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();

                    intent.setClass(MainActivity.this, ConfirmOKMenu.class);

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        user_str = ConfirmOKMenu.final_string;

        Log.i("PATRICK", user_str);
        Log.i("PATRICK", "FINISH 789");
        //pin_num = pin_num + "XXX";
//        Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
        return user_str;
    }

	  public String UserConfirmRebootMenu(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;

        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();

                    intent.setClass(MainActivity.this, ConfirmReboot.class);

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        user_str = ConfirmReboot.final_string;

        Log.i("PATRICK", user_str);
        Log.i("PATRICK", "FINISH 789");
        //pin_num = pin_num + "XXX";
//        Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
        return user_str;
    }
	
    public String UserConfirmCard(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;
        if (cardentrytimer != null)
            cardentrytimer.cancel();

        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();

                    intent.setClass(MainActivity.this, UserConfirmCard.class);

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        user_str = UserConfirmCard.final_string;
        Log.i("PATRICK", user_str);
        Log.i("PATRICK", "FINISH UserConfirmCard");
        return user_str;
    }

    public String ReceiptOnScreen(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;
        //int inTimeOut=0;

		inPrintConfirmTimeOut=inCTOSS_GetEnvInt("PRINT_CONF_TO");
		if(inPrintConfirmTimeOut <= 0)
			inPrintConfirmTimeOut=10;
		
        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();

                    intent.setClass(MainActivity.this, PrintReceiptonScreen.class);

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        user_str = PrintReceiptonScreen.final_string;

        Log.i("PATRICK", user_str);
        Log.i("PATRICK", "FINISH PrintReceiptonScreen");
        //pin_num = pin_num + "XXX";
//        Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
        return user_str;
    }


    public String FirstReceiptOnScreen(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;

        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();

                    intent.setClass(MainActivity.this, PrintFirstReceiptonScreen.class);

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        user_str = PrintFirstReceiptonScreen.final_string;

        Log.i("TINE", user_str);
        Log.i("TINE", "FINISH PrintFirstReceiptonScreen");
        //pin_num = pin_num + "XXX";
//        Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
        return user_str;
    }

    public String EliteReceiptOnScreen(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;

        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();

                    intent.setClass(MainActivity.this, EliteReceiptonScreen.class);

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        user_str = EliteReceiptonScreen.final_string;

        Log.i("TINE", user_str);
        Log.i("TINE", "FINISH EliteReceiptonScreen");
        //pin_num = pin_num + "XXX";
//        Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
        return user_str;
    }

    public String UserCancelUI(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;

        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();

                    intent.setClass(MainActivity.this, UserCancelUI.class);

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        user_str = UserCancelUI.final_string;

        Log.i("PATRICK", user_str);
        Log.i("PATRICK", "FINISH 789");
        //pin_num = pin_num + "XXX";
//        Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
        return user_str;
    }

    public String PrintCopy (String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;

        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();
                    intent.setClass(MainActivity.this, CopyMenu.class);

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        //user_str = CopyMenu.final_string;

        Log.i("PATRICK", user_str);
        Log.i("PATRICK", "FINISH 789");
        //pin_num = pin_num + "XXX";
//        Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
        return user_str;

    }

    public String getAnyNumStr(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;

        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();
                    intent.setClass(MainActivity.this, NormalKeyBoard.class);

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();


        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "IPT_ERR";
                }
            }
        }

        user_str = NormalKeyBoard.user_num_str;
        //Log.i("PATRICK123456", pin_num);
        Log.i("PATRICK", "FINISH 777");
        //pin_num = pin_num + "XXX";
//        Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
        return user_str;

    }

    public String DOptionMenuDisplay(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;

        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();
                    intent.setClass(MainActivity.this, DOptionMenu.class);

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();


        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "IPT_ERR";
                }
            }
        }

        user_str = DOptionMenu.select_item;
        //Log.i("PATRICK123456", pin_num);
        Log.i("PATRICK", "FINISH 777");
        //pin_num = pin_num + "XXX";
//        Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
        return user_str;

    }

    public String DPopupMenuDisplay(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;

        new Thread()
        {
            public void run()
            {
                try
                {
                    EditListView.isResume = false;
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();
                    intent.setClass(MainActivity.this, DPopupMenuActivity.class);

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();


        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "IPT_ERR";
                }
            }
        }

        user_str = DPopupMenuActivity.select_item;

        //Log.i("PATRICK123456", pin_num);
        Log.i("PATRICK", "FINISH DPopupMenuActivity");
        //pin_num = pin_num + "XXX";
        //Toast.makeText(this, user_str, Toast.LENGTH_LONG).show();

        return user_str;

	}

	public String CTMSUPDATE(String text) throws InterruptedException {
	
		String user_str="";
		pass_in_string = text;
	
		new Thread()
		{
			public void run()
			{
				try
				{
					Intent intent = new Intent();
					Bundle bundle = new Bundle();
	
					//intent.setClass(MainActivity.this, BatchReview.class);
					intent.setClass(MainActivity.this, Activity_ctms_update.class);
	
					bundle.putString("pass_in_string", pass_in_string);
					Log.i("pass_in_string", pass_in_string);
	
					intent.putExtras(bundle);
					startActivity(intent);
					//startActivityForResult(intent, 0);
				}
				catch(Exception e)
				{
					e.printStackTrace();
				}
			}
		}.start();
	
		MainActivity.LOCK.setCondition(false);
	
		synchronized (LOCK) {
		while (!LOCK.conditionMet()) {
			try {
				LOCK.wait();
				} catch (InterruptedException e) {
					Log.e(TAG, "Exception when waiting for condition", e);
					return "Exception";
				}
			}
		}
	
		//user_str = BatchReview.final_string;
	
		Log.i("PATRICK", user_str);
		Log.i("PATRICK", "CTMS 789");
		//pin_num = pin_num + "XXX";
	//	Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
		return user_str;
	}

		public String CTMSUPDATEBackground(String text) throws InterruptedException {
	
			String user_str = "000";
			pass_in_string = text;
	
			new Thread() {
				public void run() {
					try {
						Intent intent = new Intent();
						Bundle bundle = new Bundle();
	
						//intent.setClass(MainActivity.this, BatchReview.class);
	//					  intent.setClass(MainActivity.this, Activity_ctms_update.class);
						intent.setClass(MainActivity.this, Activity_ctms_background.class);
	
						//remark this line if no need to do check install result after boot up install done
						bundle.putBoolean("CHECKINSTALL_FLAG", true);
						bundle.putString("pass_in_string", pass_in_string);
						Log.i("pass_in_string", pass_in_string);
	
						intent.putExtras(bundle);
						startActivity(intent);
						//startActivityForResult(intent, 0);
					} catch (Exception e) {
						e.printStackTrace();
					}
				}
			}.start();
	
			MainActivity.LOCK.setCondition(false);
	
			synchronized (LOCK) {
				while (!LOCK.conditionMet()) {
					try {
						LOCK.wait();
					} catch (InterruptedException e) {
						Log.e(TAG, "Exception when waiting for condition", e);
						return "Exception";
					}
				}
			}
	
			//user_str = "";
	
			Log.i("PATRICK", user_str);
			Log.i("PATRICK", "CTMS 789");
			//pin_num = pin_num + "XXX";
			//	Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
			return user_str;
		}


    public String UserConfirmMenuInvandAmt(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;

        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();
                    intent.setClass(MainActivity.this, UserConfirmVoid.class);

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        user_str = UserConfirmVoid.final_string;

        Log.i("PATRICK", user_str);
        Log.i("PATRICK", "FINISH 789");
        //pin_num = pin_num + "XXX";
//        Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
        return user_str;

    }

    public static Bitmap drawTextToLeftTop(Context context, Bitmap bitmap, String text,

                                           int size, int color, int paddingLeft, int paddingTop) {

        Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);

        paint.setColor(color);

        paint.setTextSize(dp2px(context, size));

        Rect bounds = new Rect();

        paint.getTextBounds(text, 0, text.length(), bounds);

        return drawTextToBitmap(context, bitmap, text, paint, bounds,

                dp2px(context, paddingLeft),

                dp2px(context, paddingTop) + bounds.height());

    }

    public static Bitmap drawTextToRightBottom(Context context, Bitmap bitmap, String text,

                                               int size, int color, int paddingRight, int paddingBottom) {

        Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);

        paint.setColor(color);

        paint.setTextSize(dp2px(context, size));

        Rect bounds = new Rect();

        paint.getTextBounds(text, 0, text.length(), bounds);

        return drawTextToBitmap(context, bitmap, text, paint, bounds,

                bitmap.getWidth() - bounds.width() - dp2px(context, paddingRight),

                bitmap.getHeight() - dp2px(context, paddingBottom));

    }

    public static Bitmap drawTextToRightTop(Context context, Bitmap bitmap, String text,

                                            int size, int color, int paddingRight, int paddingTop) {

        Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);

        paint.setColor(color);

        paint.setTextSize(dp2px(context, size));

        Rect bounds = new Rect();

        paint.getTextBounds(text, 0, text.length(), bounds);

        return drawTextToBitmap(context, bitmap, text, paint, bounds,

                bitmap.getWidth() - bounds.width() - dp2px(context, paddingRight),

                dp2px(context, paddingTop) + bounds.height());

    }

    public static Bitmap drawTextToLeftBottom(Context context, Bitmap bitmap, String text,

                                              int size, int color, int paddingLeft, int paddingBottom) {

        Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);

        paint.setColor(color);

        paint.setTextSize(dp2px(context, size));

        Rect bounds = new Rect();

        paint.getTextBounds(text, 0, text.length(), bounds);

        return drawTextToBitmap(context, bitmap, text, paint, bounds,

                dp2px(context, paddingLeft),

                bitmap.getHeight() - dp2px(context, paddingBottom));

    }

    public static Bitmap drawTextToCenter(Context context, Bitmap bitmap, String text,

                                          int size, int color) {

        Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);

        paint.setColor(color);

        paint.setTextSize(dp2px(context, size));

        Rect bounds = new Rect();

        paint.getTextBounds(text, 0, text.length(), bounds);

        return drawTextToBitmap(context, bitmap, text, paint, bounds,

                (bitmap.getWidth() - bounds.width()) / 2,

                (bitmap.getHeight() + bounds.height()) / 2);

    }

    private static Bitmap drawTextToBitmap(Context context, Bitmap bitmap, String text,

                                           Paint paint, Rect bounds, int paddingLeft, int paddingTop) {

        android.graphics.Bitmap.Config bitmapConfig = bitmap.getConfig();



        paint.setDither(true); // 获取跟清晰的图像采样

        paint.setFilterBitmap(true);// 过滤一些

        if (bitmapConfig == null) {

            bitmapConfig = android.graphics.Bitmap.Config.ARGB_8888;

        }

        bitmap = bitmap.copy(bitmapConfig, true);

        Canvas canvas = new Canvas(bitmap);



        canvas.drawText(text, paddingLeft, paddingTop, paint);

        return bitmap;

    }

    public static Bitmap scaleWithWH(Bitmap src, double w, double h) {

        if (w == 0 || h == 0 || src == null) {

            return src;

        } else {

            // 记录src的宽高

            int width = src.getWidth();

            int height = src.getHeight();

            // 创建一个matrix容器

            Matrix matrix = new Matrix();

            // 计算缩放比例

            float scaleWidth = (float) (w / width);

            float scaleHeight = (float) (h / height);

            // 开始缩放

            matrix.postScale(scaleWidth, scaleHeight);

            // 创建缩放后的图片

            return Bitmap.createBitmap(src, 0, 0, width, height, matrix, true);

        }

    }

    public static int dp2px(Context context, float dp) {

        final float scale = context.getResources().getDisplayMetrics().density;

        return (int) (dp * scale + 0.5f);

    }

    public	ArrayList<Bitmap> getBitmapArrayByGif(int inResId) {
        try {
            ArrayList<Bitmap> BitmapList = new ArrayList<>();
            int i=0;
            //GifDrawable gifDrawable = new GifDrawable(getResources(), R.drawable.cardidle);//加载一个动态图gif
            GifDrawable gifDrawable = new GifDrawable(getResources(), inResId);//加载一个动态图gif
            int totalCount = gifDrawable.getNumberOfFrames();
            for(i=0;i<totalCount;i++)
            {
                BitmapList.add(gifDrawable.seekToFrameAndGet(i));
            }
            return BitmapList;
        } catch (Exception e) {
            return null;
        }
    }

    public int	getResource(String imageName) {
        Context ctx = getBaseContext();
        int resId = getResources().getIdentifier(imageName, "drawable", ctx.getPackageName());
        return resId;
    }


    public String DisplayUI(String text) throws InterruptedException {

        String user_str="";
        in_string = text;

        runOnUiThread(new Runnable() {
            @RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN)
            @Override
            public void run() {
                setContentView(R.layout.display_cardidle);

                //Intent intent=getIntent();
                //String dispmsg=intent.getStringExtra("pass_in_string");
                //Log.i("dispmsg", dispmsg);

                String dispmsg=in_string;
                Log.i(TAG, dispmsg);

                // Show status bar
                getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);


                String[] dispmsginfo = dispmsg.split("\\|");
                int msgcnt = dispmsginfo.length;


                GifImageView gifImageView1 = (GifImageView) findViewById(R.id.gif1);
                try {// 如果加载的是gif动图，第一步需要先将gif动图资源转化为GifDrawable
                    // 将gif图资源转化为GifDrawable
                    ArrayList<Bitmap> BitmapList = new ArrayList<>();
                    ArrayList<Bitmap> BitmapListNew = new ArrayList<>();
                    ArrayList<Integer> Fontcolor = new ArrayList<>();
                    BitmapDrawable bd = null;

                    Fontcolor.add(Color.RED);
                    Fontcolor.add(Color.BLUE);
                    Fontcolor.add(Color.GREEN);
                    Fontcolor.add(Color.YELLOW);
                    Fontcolor.add(Color.WHITE);
                    Fontcolor.add(Color.BLACK);

                    Bitmap newbp = null;
                    Bitmap tempbp;
                    byte[] gifbyte = null;

                    AnimationDrawable animation = new AnimationDrawable();

                    int i = 0;

                    int ResId =getResource(dispmsginfo[0]);
                    BitmapList = getBitmapArrayByGif(ResId);

                    Log.i("BitmapList.size()", BitmapList.size()+"");

                    //String input = "TEST第一次";

                    for(i=0;i<BitmapList.size();i++) {
                        Log.i("BitmapList.size()", i+"");
                        //Thread.sleep(5000);
                        if(msgcnt == 2)
                        {
                            newbp = BitmapList.get(i);
                        }
                        else if(msgcnt == 3)
                        {
                            newbp = drawTextToRightTop(getApplicationContext(), BitmapList.get(i), dispmsginfo[2], 16, Color.BLACK, 6, 6);
                        }
                        else if(msgcnt == 4)
                        {
                            tempbp = drawTextToRightTop(getApplicationContext(), BitmapList.get(i), dispmsginfo[2], 16, Color.BLACK, 6, 6);
                            newbp = drawTextToRightTop(getApplicationContext(), tempbp, dispmsginfo[3], 28, Color.BLACK, 10, 100);
                        }
                        else if(msgcnt == 5)
                        {
                            tempbp = drawTextToRightTop(getApplicationContext(), BitmapList.get(i), dispmsginfo[2], 16, Color.BLACK, 6, 6);
                            tempbp = drawTextToRightTop(getApplicationContext(), tempbp, dispmsginfo[3], 28, Color.BLACK, 10, 100);
                            newbp = drawTextToLeftTop(getApplicationContext(), tempbp, dispmsginfo[4], 28, Color.BLACK, 10, 100);
                        }
                        bd = new BitmapDrawable(getResources(), newbp);
                        animation.addFrame(bd, 200);

                    }
                    animation.setOneShot(false);


                    if(dispmsginfo[1].equals("front")) {
                        gifImageView1.setImageDrawable(animation);
                    }
                    else {
                        gifImageView1.setBackground(animation);
                    }
                    // start the animation!
                    animation.start();

                } catch(Exception e){
                    e.printStackTrace();
                }
            }
        });
        return user_str;
    }

    Bitmap myBitmap = null;
    ImageView imageView_receipt_bmp;
    File receipt_bmp_file = null;

    public String LCDDisplay(String text) throws InterruptedException {

        String user_str="";
        in_string = text;
        if(cardentrytimer != null)
            cardentrytimer.cancel();

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                overridePendingTransition(0, 0); // disable the animation, faster

                View decorView = getWindow().getDecorView();
                decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);

                setContentView(R.layout.lcddisplay_message);

                //Intent intent=getIntent();
                //String dispmsg=intent.getStringExtra("pass_in_string");
                //Log.i("dispmsg", dispmsg);

                String dispmsg = in_string;
                Log.i(TAG, dispmsg);

                TextView textViewmsg;
                TextView textView_dtxn;
                TextView textView_cur;
                TextView textView_opt1;
                TextView textView_opt2;

                // For BMP Display
                ConstraintLayout linearLayout;
                LinearLayout linearLayout_bmp;

                // For Status Display
                LinearLayout linearLayout_status;
                TextView textView_status1;
                TextView textView_status2;
                ImageView imageView_status;

                LinearLayout lcd_header;
                LinearLayout lcd_txntitle;
                LinearLayout lcd_txn_cur;

                // Show status bar
                getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);


                String[] dispmsginfo = dispmsg.split("\\|");
                int msgcnt = dispmsginfo.length;
                Log.i("sidumili", "LCDDisplay: msgcnt="+msgcnt+",dispmsginfo="+dispmsginfo);

                textViewmsg = (TextView) findViewById(R.id.textView_msg);
                textView_dtxn = (TextView) findViewById(R.id.textView_dtxn);
                textView_cur = (TextView) findViewById(R.id.textView_cur);
                textView_opt1 = (TextView) findViewById(R.id.textView6);
                textView_opt2 = (TextView) findViewById(R.id.textView7);

                linearLayout = (ConstraintLayout) findViewById(R.id.linearLayout);
                linearLayout_bmp= (LinearLayout) findViewById(R.id.linearLayout_bmp);

                linearLayout_status = (LinearLayout) findViewById(R.id.linearLayout_status);

                lcd_header = (LinearLayout) findViewById(R.id.lcd_header);
                lcd_txntitle = (LinearLayout) findViewById(R.id.lcd_txntitle);
                lcd_txn_cur = (LinearLayout) findViewById(R.id.lcd_txn_cur);

                try
                {

                    for (int inIdx = 0; inIdx < msgcnt; inIdx++) {
                        System.out.println("LCDDisplay::split msg [" + inIdx + "][" + dispmsginfo[inIdx] + "]");
                        switch (inIdx) {
                            case 0:
                                textView_dtxn.setText(dispmsginfo[inIdx]);
                                break;
                            case 1:
                                textViewmsg.setText(dispmsginfo[inIdx]);

                                // ---------------------------------------------------------------------------------------------------------------------------
                                /*
                                if (dispmsginfo[1].toString().equals("PROCESSING..."))
                                {
                                    linearLayout.setVisibility(View.GONE);
                                    linearLayout_status.setVisibility(View.VISIBLE);

                                    imageView_status = (ImageView) findViewById(R.id.imageViewStatus);
                                    imageView_status.setImageDrawable(getResources().getDrawable(R.drawable.icon_process));

                                    textView_status1 = (TextView) findViewById(R.id.box_msg_status1);
                                    textView_status1.setText("Processing");

                                    textView_status2 = (TextView) findViewById(R.id.box_msg_status2);
                                    textView_status2.setText("Please wait...");

                                    // declare animation
                                    final Animation stb = AnimationUtils.loadAnimation(getApplicationContext(), R.anim.stb);
                                    final Animation ttb = AnimationUtils.loadAnimation(getApplicationContext(), R.anim.ttb);

                                    imageView_status = (ImageView) findViewById(R.id.imageViewStatus);
                                    textView_status1 = (TextView) findViewById(R.id.box_msg_status1);
                                    textView_status2 = (TextView) findViewById(R.id.box_msg_status2);

                                    // set animation
                                    imageView_status.startAnimation(stb);

                                    textView_status1.startAnimation(ttb);
                                    textView_status2.startAnimation(ttb);

                                }
                                else if (dispmsginfo[1].toString().equals("LOADING..."))
                                {
                                    linearLayout.setVisibility(View.GONE);
                                    linearLayout_status.setVisibility(View.VISIBLE);

                                    imageView_status = (ImageView) findViewById(R.id.imageViewStatus);
                                    imageView_status.setImageDrawable(getResources().getDrawable(R.drawable.icon_process));

                                    textView_status1 = (TextView) findViewById(R.id.box_msg_status1);
                                    textView_status1.setText("Loading application");

                                    textView_status2 = (TextView) findViewById(R.id.box_msg_status2);
                                    textView_status2.setText("Please wait...");

                                    // declare animation
                                    final Animation stb = AnimationUtils.loadAnimation(getApplicationContext(), R.anim.stb);
                                    final Animation ttb = AnimationUtils.loadAnimation(getApplicationContext(), R.anim.ttb);

                                    imageView_status = (ImageView) findViewById(R.id.imageViewStatus);
                                    textView_status1 = (TextView) findViewById(R.id.box_msg_status1);
                                    textView_status2 = (TextView) findViewById(R.id.box_msg_status2);

                                    // set animation
                                    imageView_status.startAnimation(stb);

                                    textView_status1.startAnimation(ttb);
                                    textView_status2.startAnimation(ttb);
                                }
                                else if (dispmsginfo[1].toString().equals("INITIALIZING..."))
                                {
                                    linearLayout.setVisibility(View.GONE);
                                    linearLayout_status.setVisibility(View.VISIBLE);

                                    imageView_status = (ImageView) findViewById(R.id.imageViewStatus);
                                    imageView_status.setImageDrawable(getResources().getDrawable(R.drawable.icon_process));

                                    textView_status1 = (TextView) findViewById(R.id.box_msg_status1);
                                    textView_status1.setText("Initializing");

                                    textView_status2 = (TextView) findViewById(R.id.box_msg_status2);
                                    textView_status2.setText("Please wait...");

                                    // declare animation
                                    final Animation stb = AnimationUtils.loadAnimation(getApplicationContext(), R.anim.stb);
                                    final Animation ttb = AnimationUtils.loadAnimation(getApplicationContext(), R.anim.ttb);

                                    imageView_status = (ImageView) findViewById(R.id.imageViewStatus);
                                    textView_status1 = (TextView) findViewById(R.id.box_msg_status1);
                                    textView_status2 = (TextView) findViewById(R.id.box_msg_status2);

                                    // set animation
                                    imageView_status.startAnimation(stb);

                                    textView_status1.startAnimation(ttb);
                                    textView_status2.startAnimation(ttb);
                                }
                                else if (dispmsginfo[1].toString().equals("SENDING..."))
                                {
                                    linearLayout.setVisibility(View.GONE);
                                    linearLayout_status.setVisibility(View.VISIBLE);

                                    imageView_status = (ImageView) findViewById(R.id.imageViewStatus);
                                    imageView_status.setImageDrawable(getResources().getDrawable(R.drawable.icon_process));

                                    textView_status1 = (TextView) findViewById(R.id.box_msg_status1);
                                    textView_status1.setText("Sending");

                                    textView_status2 = (TextView) findViewById(R.id.box_msg_status2);
                                    textView_status2.setText("Please wait...");

                                    // declare animation
                                    final Animation stb = AnimationUtils.loadAnimation(getApplicationContext(), R.anim.stb);
                                    final Animation ttb = AnimationUtils.loadAnimation(getApplicationContext(), R.anim.ttb);

                                    imageView_status = (ImageView) findViewById(R.id.imageViewStatus);
                                    textView_status1 = (TextView) findViewById(R.id.box_msg_status1);
                                    textView_status2 = (TextView) findViewById(R.id.box_msg_status2);

                                    // set animation
                                    imageView_status.startAnimation(stb);

                                    textView_status1.startAnimation(ttb);
                                    textView_status2.startAnimation(ttb);
                                }
                                else if (dispmsginfo[1].toString().equals("RECEIVING..."))
                                {
                                    linearLayout.setVisibility(View.GONE);
                                    linearLayout_status.setVisibility(View.VISIBLE);

                                    imageView_status = (ImageView) findViewById(R.id.imageViewStatus);
                                    imageView_status.setImageDrawable(getResources().getDrawable(R.drawable.icon_process));

                                    textView_status1 = (TextView) findViewById(R.id.box_msg_status1);
                                    textView_status1.setText("Receiving");

                                    textView_status2 = (TextView) findViewById(R.id.box_msg_status2);
                                    textView_status2.setText("Please wait...");

                                    // declare animation
                                    final Animation stb = AnimationUtils.loadAnimation(getApplicationContext(), R.anim.stb);
                                    final Animation ttb = AnimationUtils.loadAnimation(getApplicationContext(), R.anim.ttb);

                                    imageView_status = (ImageView) findViewById(R.id.imageViewStatus);
                                    textView_status1 = (TextView) findViewById(R.id.box_msg_status1);
                                    textView_status2 = (TextView) findViewById(R.id.box_msg_status2);

                                    // set animation
                                    imageView_status.startAnimation(stb);

                                    textView_status1.startAnimation(ttb);
                                    textView_status2.startAnimation(ttb);
                                }
                                 */
                                if(dispmsginfo[1].toString().equals("PRINTING...")) // Printing animation
                                {
                                    linearLayout.setVisibility(View.GONE);
                                    linearLayout_bmp.setVisibility(View.VISIBLE);

                                    receipt_bmp_file = new File("/data/data/pub/Print_BMP.bmp");

                                    myBitmap = BitmapFactory.decodeFile(receipt_bmp_file.getAbsolutePath());
                                    imageView_receipt_bmp = (ImageView) findViewById(R.id.receipt_bmp);
                                    imageView_receipt_bmp.setImageBitmap(myBitmap);

                                    Animation animation = AnimationUtils.loadAnimation(getApplicationContext(),R.anim.move_up);
                                    animation.setDuration(10000);
                                    imageView_receipt_bmp.startAnimation(animation);
                                }
                                //----------------------------------------------------------------------------------------------------------------------------
                                break;
                            case 2:
                                textView_cur.setText(dispmsginfo[inIdx]);
                                break;
                            case 3:
                                textView_opt1.setText(dispmsginfo[inIdx]);
                                break;
                            case 4:
                                textView_opt2.setText(dispmsginfo[inIdx]);
                                break;
                        }
                    }

					// sidumili: Set visibility to GONE when passing no value. Fix on press cancel during printing preview
                    if (dispmsginfo[0].trim().length() <= 0 || dispmsginfo[1].trim().length() <= 0)
                        isHide = true;
                    else
                        isHide = false;

                    Log.i("sidumili", "run: isHide="+isHide+",msgcnt="+msgcnt);
                    if (isHide)
                    {
                        Log.i("sidumili", "run: Hide me now");
                        lcd_header.setVisibility(View.GONE);
                        lcd_txntitle.setVisibility(View.GONE);
                        lcd_txn_cur.setVisibility(View.GONE);
                    }

                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }

            }

        });

        //Log.i("Main", "sync app at LCDDisplay");
        //synchronized (MainActivity.LOCK) {
        //    MainActivity.LOCK.setCondition(true);
        //    MainActivity.LOCK.notifyAll();

        //}

        user_str = "";
        Log.i("LCDDisplay", "user_str = " + user_str);
        return user_str;
    }

    final CountDownTimer cardentrytimer;
    int inTimeOut=30;

    {
        cardentrytimer = new CountDownTimer(inTimeOut * 1000, 1000) {

            @Override
            public void onTick(long millisUntilFinished) {
                Log.i("Timer", "cardentrytimer onTick");
            }

            @Override
            public void onFinish() {
                Log.i("Timer", "cardentrytimer onFinish");

                inCTOSS_CARDENTRYTIMEOUT();
                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                /*
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();
                }

                 */
            }
        };
    }

	


    public String usCARDENTRY(String text) throws InterruptedException {

        String user_str = "";
        in_string = text;
        if(cardentrytimer != null)
            cardentrytimer.cancel();

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                try {
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

                    setContentView(R.layout.cardentry);

                    //Intent intent=getIntent();
                    //String dispmsg=intent.getStringExtra("pass_in_string");
                    //Log.i("dispmsg", dispmsg);
                    int inTimeOut = 30;

                    String man_entry_flag = "";
                    String dispmsg = in_string;
                    Log.i(TAG, dispmsg);

                    final TextView textViewmsg;
                    TextView textView_dtxn;
                    TextView textView_cur;
                    Button btn_can;
                    Button man_entry;

                    // Show status bar
                    getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);


                    String[] dispmsginfo = dispmsg.split("\\|");
                    int msgcnt = dispmsginfo.length;

                    textViewmsg = (TextView) findViewById(R.id.textView_msg);
                    textView_dtxn = (TextView) findViewById(R.id.textView_dtxn);
                    textView_cur = (TextView) findViewById(R.id.textView_cur);
                    btn_can = (Button) findViewById(R.id.btn_can);
                    man_entry = (Button) findViewById(R.id.man_entry);

                    //to do
                    for (int inIdx = 0; inIdx < msgcnt; inIdx++) {
                        System.out.println("split msg [" + inIdx + "][" + dispmsginfo[inIdx] + "]");
                        switch (inIdx) {
                            case 0:
                                textView_dtxn.setText(dispmsginfo[inIdx]);
                                break;
                            case 1:
                                textViewmsg.setText(dispmsginfo[inIdx]);
                                break;
                            case 2:
                                textView_cur.setText(dispmsginfo[inIdx]);
                                break;
                            case 3:
                                man_entry_flag = dispmsginfo[inIdx];
                                if(man_entry_flag.equals("0"))
                                    man_entry.setVisibility(View.GONE);
                                if(man_entry_flag.equals("1"))
                                    man_entry.setVisibility(View.VISIBLE);
                                break;
                        }
                    }
                    //cardentrytimer.start();

                    btn_can.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View view) {
                            //startActivity(new Intent(CardEntry.this, MainActivity.class));
                            Log.i("PATRICK", "Cancel buOK");
                            //cardentrytimer.cancel();
                            if(cardentrytimer != null)
                                cardentrytimer.cancel();

                            inCTOSS_BUTTONCANCEL();
                        }
                    });

                    man_entry.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View view) {
                            //startActivity(new Intent(CardEntry.this, MainActivity.class));
                            Log.i("PATRICK", "Manual Entry");
                            //cardentrytimer.cancel();
                            if(cardentrytimer != null)
                                cardentrytimer.cancel();

                            inCTOSS_MANUALENTRY();
                        }
                    });

                } catch (Exception e) {
                    e.printStackTrace();
                }
            }

        });

        synchronized (MainActivity.LOCK) {
            MainActivity.LOCK.setCondition(true);
            MainActivity.LOCK.notifyAll();
        }

		user_str = "";
        return user_str;
    }

    private String usECRCARDENTRY(String text) throws InterruptedException {

        String user_str = "";
        in_string = text;
        if(cardentrytimer != null)
            cardentrytimer.cancel();

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                try {
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

                    setContentView(R.layout.cardentry);

                    //Intent intent=getIntent();
                    //String dispmsg=intent.getStringExtra("pass_in_string");
                    //Log.i("dispmsg", dispmsg);
                    int inTimeOut = 30;

                    String man_entry_flag = "";
                    String dispmsg = in_string;
                    Log.i(TAG, dispmsg);

                    final TextView textViewmsg;
                    TextView textView_dtxn;
                    TextView textView_cur;
                    Button btn_can;
                    Button man_entry;

                    // Show status bar
                    getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);


                    String[] dispmsginfo = dispmsg.split("\\|");
                    int msgcnt = dispmsginfo.length;

                    textViewmsg = (TextView) findViewById(R.id.textView_msg);
                    textView_dtxn = (TextView) findViewById(R.id.textView_dtxn);
                    textView_cur = (TextView) findViewById(R.id.textView_cur);
                    btn_can = (Button) findViewById(R.id.btn_can);
                    man_entry = (Button) findViewById(R.id.man_entry);

                    //to do
                    for (int inIdx = 0; inIdx < msgcnt; inIdx++) {
                        System.out.println("split msg [" + inIdx + "][" + dispmsginfo[inIdx] + "]");
                        switch (inIdx) {
                            case 0:
                                textView_dtxn.setText(dispmsginfo[inIdx]);
                                break;
                            case 1:
                                textViewmsg.setText(dispmsginfo[inIdx]);
                                break;
                            case 2:
                                textView_cur.setText(dispmsginfo[inIdx]);
                                break;
                            case 3:
                                man_entry_flag = dispmsginfo[inIdx];
                                if(man_entry_flag.equals("0"))
                                    man_entry.setVisibility(View.GONE);
                                if(man_entry_flag.equals("1"))
                                    man_entry.setVisibility(View.VISIBLE);
                                break;
                        }
                    }
                    //cardentrytimer.start();

                    btn_can.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View view) {
                            //startActivity(new Intent(CardEntry.this, MainActivity.class));
                            Log.i("PATRICK", "Cancel buOK");
                            //cardentrytimer.cancel();
                            if(cardentrytimer != null)
                                cardentrytimer.cancel();

                            inCTOSS_BUTTONCANCEL();
                        }
                    });

                    man_entry.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View view) {
                            //startActivity(new Intent(CardEntry.this, MainActivity.class));
                            Log.i("PATRICK", "Manual Entry");
                            //cardentrytimer.cancel();
                            if(cardentrytimer != null)
                                cardentrytimer.cancel();

                            inCTOSS_MANUALENTRY();
                        }
                    });

                } catch (Exception e) {
                    e.printStackTrace();
                }
            }

        });

        synchronized (MainActivity.LOCK) {
            MainActivity.LOCK.setCondition(true);
            MainActivity.LOCK.notifyAll();
        }

        user_str = "";
        return user_str;
    }


    public String usSetDateAndTime(String text) {
        Log.i(TAG, "usSetDateAndTime: run");

        //int iYear=0,iMonth=0, iDay=0, iHour=0, iMinutes=0, iSeconds=0;
        String sYear="",sMonth="", sDay="", sHour="", sMinutes="", sSeconds="", sSavedYear = "";

        String user_str="";
        in_string = text;
        String dispmsg = in_string;
        String szCurrentYear ="";

        String szCompleteDateTime ="";

        String[] dispmsginfo = dispmsg.split("\\|");
        int msgcnt = dispmsginfo.length;

        for (int inIdx = 0; inIdx < msgcnt; inIdx++) {
            System.out.println("usSetDateAndTime->saturn split msg [" + inIdx + "][" + dispmsginfo[inIdx] + "]");
            switch (inIdx) {
                case 0:
                    //iMonth = Integer.parseInt(dispmsginfo[inIdx]);
                    sMonth = dispmsginfo[inIdx];
                    break;
                case 1:
                    //iDay= Integer.parseInt(dispmsginfo[inIdx]);
                    sDay = dispmsginfo[inIdx];
                    break;
                case 2:
                    //iYear = Integer.parseInt(dispmsginfo[inIdx]);
                    sYear = dispmsginfo[inIdx];
                    break;
                case 3:
                    //iHour = Integer.parseInt(dispmsginfo[inIdx]);
                    sHour = dispmsginfo[inIdx];
                    break;
                case 4:
                    //iMinutes = Integer.parseInt(dispmsginfo[inIdx]);
                    sMinutes = dispmsginfo[inIdx];
                    break;
                case 5:
                    //iSeconds = Integer.parseInt(dispmsginfo[inIdx]);
                    sSeconds = dispmsginfo[inIdx];
                    break;
                case 6:
                    //iSeconds = Integer.parseInt(dispmsginfo[inIdx]);
                    sSavedYear = dispmsginfo[inIdx];
                    Log.i(TAG, "sSavedYear: " + sSavedYear);
                    break;
                default:
                    Log.i(TAG, "default: ");
                    break;
            }
        }


        if(sYear.equals("x")){
            SimpleDateFormat sdf = new SimpleDateFormat("yyyy");
            szCurrentYear = sdf.format(new Date());
            Log.i("saturn", "saturn szCurrentYear "+ szCurrentYear);
            sYear = szCurrentYear;

        }

        int num1 = Integer.parseInt(sYear);
        int num2 = Integer.parseInt(sSavedYear);
        Log.w(TAG, "num1: " + num1 + " num2: " + num2);
        if(num1 < num2) {
            sYear = sSavedYear;
            Log.w(TAG, "usSetDateAndTime: " + sSavedYear);
        }

        CtSystem system = new CtSystem();

        try{
            SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyyMMddHHmmss");

            szCompleteDateTime = sYear+sMonth+sDay+sHour+sMinutes+sSeconds;

            Date date = simpleDateFormat.parse(szCompleteDateTime);

            Log.d(TAG, "saturn setSystemTime return = " + Integer.toString(system.setSystemTime(date)));
        } catch (ParseException e) {
            Log.e("Castles", "Date and Time parse exception", e);
        }

        user_str = "time set";

        return user_str;

    }

    public String usEditDatabase(String text) throws InterruptedException {

        Log.i("Castles", "Edit Table");
        String user_str="";
        pass_in_string = text;

        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();

                    intent.setClass(MainActivity.this, EditDatabaseActivity.class);

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);
                    bundle.putString("AppHeaderBuff", AppHeaderBuff);
                    Log.i("AppHeaderBuff", AppHeaderBuff);

                    intent.putExtras(bundle);
                    startActivity(intent);
                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        user_str = "";

        Log.i("TINE", "FINISH EditDatabaseActivity "+user_str);

        //Toast.makeText(this, user_str, Toast.LENGTH_LONG).show();

        return user_str;

    }

    public String ePad_SignatureCaptureLibEex_Java(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;

        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();
                    intent.setClass(MainActivity.this, SignaturePadCastles.class);

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "IPT_ERR";
                }
            }
        }

        Log.i("PATRICK", "FINISH 888");

        return user_str;
    }

    public String usCARDENTRY2_Java(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;

        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();
                    intent.setClass(MainActivity.this, CardEntry.class);

                    bundle.putString("pass_in_string", pass_in_string);

                    //String result = bundle.getString("result");
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                }
                catch(Exception e)
                {
                    //e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        Log.i("TINE", "FINISH usCARDENTRY_Java");
        user_str = CardEntry.cardentry_final_string;
        return user_str;

    }

    @Override
    public void onBackPressed() {
        super.onBackPressed();
        Log.i("TINE", "BackPressed MainActivity");

        synchronized (MainActivity.LOCK) {
            MainActivity.LOCK.setCondition(true);
            MainActivity.LOCK.notifyAll();
        }

    }
    
    public String MenuTransactions(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;

        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();

                    intent.setClass(MainActivity.this, Transactions.class);

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);
                    bundle.putString("AppHeaderBuff", AppHeaderBuff);
                    Log.i("AppHeaderBuff", AppHeaderBuff);

                    intent.putExtras(bundle);
                    startActivity(intent);
                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        user_str = Transactions.select_item;

        Log.i("TINE", "FINISH MenuTransactionType "+user_str);

        //Toast.makeText(this, user_str, Toast.LENGTH_LONG).show();

        return user_str;
    }

    public String MenuTransType(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;

        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();

                   //intent.setClass(MainActivity.this, MenuTransactionType.class);
                    Log.i("MenuTransType", "inMENUTYPE:"+inMENUTYPE);
				   
                    if (inMENUTYPE == 1)
						intent.setClass(MainActivity.this, MenuTransactionType2.class);
					else
						intent.setClass(MainActivity.this, MenuTransactionType.class);
					

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);
                    bundle.putString("AppHeaderBuff", AppHeaderBuff);
                    Log.i("AppHeaderBuff", AppHeaderBuff);

                    intent.putExtras(bundle);
                    startActivity(intent);
                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

		if (inMENUTYPE == 1)
        	user_str = MenuTransactionType2.select_item2;
		else
        	user_str = MenuTransactionType.select_item;

        Log.i("TINE", "FINISH MenuTransactionType "+user_str);

        //Toast.makeText(this, user_str, Toast.LENGTH_LONG).show();

        return user_str;
    }

    public String IdleMenuTransType(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;

        /*
        new Thread()
        {
            public void run()
            {
                try
                {

                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();
        */
        Intent intent = new Intent();
        Bundle bundle = new Bundle();

        intent.setClass(MainActivity.this, IdleMenuTransactionType.class);
        intent.addFlags(Intent.FLAG_ACTIVITY_NO_HISTORY);

        AppHeaderBuff = GetAppHeaderDetails();

        bundle.putString("pass_in_string", pass_in_string);
        Log.i("pass_in_string", pass_in_string);
        bundle.putString("AppHeaderBuff", AppHeaderBuff);
        Log.i("AppHeaderBuff", AppHeaderBuff);

        intent.putExtras(bundle);
        startActivity(intent);

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        user_str = IdleMenuTransactionType.select_item;

        Log.i("TINE", "FINISH MenuTransactionType "+user_str);

        //Toast.makeText(this, user_str, Toast.LENGTH_LONG).show();

        return user_str;
    }
    public String IdleMenuTransType2(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;

        /*
        new Thread()
        {
            public void run()
            {
                try
                {

                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();
        */
        Intent intent = new Intent();
        Bundle bundle = new Bundle();

        intent.setClass(MainActivity.this, IdleMenuTransactionType2.class);
        intent.addFlags(Intent.FLAG_ACTIVITY_NO_HISTORY);

        AppHeaderBuff = GetAppHeaderDetails();

        bundle.putString("pass_in_string", pass_in_string);
        Log.i("pass_in_string", pass_in_string);
        bundle.putString("AppHeaderBuff", AppHeaderBuff);
        Log.i("AppHeaderBuff", AppHeaderBuff);

        intent.putExtras(bundle);
        startActivity(intent);

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        user_str = IdleMenuTransactionType2.select_item;

        Log.i("TINE", "FINISH MenuTransactionType "+user_str);

        //Toast.makeText(this, user_str, Toast.LENGTH_LONG).show();

        return user_str;
    }

    public String BackToProgress(String text) throws InterruptedException {
	
		in_string = text;
	
		runOnUiThread(new Runnable() {
			@Override
			public void run() {
	
				jni_api.REG_CB_CTOS(jni_cb_ctos);
				setContentView(R.layout.activity_disp);
				lsv_LCDTPrint = (ListView) findViewById(R.id.lsv_LCDTPrint);
				edtLog = (EditText) findViewById(R.id.edtLog);
				adapter = new ArrayAdapter(MainActivity.this, R.layout.listitem, list) {
	
					@NonNull
					@Override
					public View getView(int position, @Nullable View convertView, @NonNull ViewGroup parent) {
						View view = super.getView(position, convertView, parent);
	
						TextView textView = (TextView) view;
	
						if (addPadding[position] != 0)
							textView.setPadding(36 * addPadding[position], 0, 0, 0);
	
	
						if (displayMode[position].equals("0")) {
							textView.setGravity(Gravity.LEFT);
						} else if (displayMode[position].equals("1")) {
							textView.setGravity(Gravity.CENTER);
						} else if (displayMode[position].equals("2")) {
							textView.setGravity(Gravity.RIGHT);
						}
	
	
						return view;
	
					}
				};
			}
		});
	
		return in_string;
	}

	

   

    public String GetIPPPinEx(int KeySet,  int KeyIndex,  String pInData, int pinBypassAllow) throws InterruptedException {


        inKeySet = KeySet;
        inKeyIndex = KeyIndex;
        StrInData = pInData;
        inPinBypassAllow = pinBypassAllow;
        Enter_Press = 0;
        inKeyType = 2;


        Log.i("GetIPPPinEx", inKeySet+"");
        Log.i("GetIPPPinEx", inKeyIndex+"");
        Log.i("GetIPPPinEx", StrInData+"");
        Log.i("GetIPPPinEx", inPinBypassAllow+"");

        kmsui.init(MainActivity.this, inKeySet, inKeyIndex, StrInData, inKeyType, inPinBypassAllow);


        while(MainActivity.Enter_Press==0)
        {
            //Log.d(TAG_A, MainActivity.Enter_Press+"");
            Thread.sleep(500);
        }

        Log.d("GetIPPPinEx", MainActivity.Enter_Press+"");




       
        Log.i("GetIPPPinEx", MainActivity.Enter_Press+"");
        OutputStr = String.valueOf(MainActivity.inRet)+'*'+sPinBlock+'|'+sKSN;

        Log.i("GetIPPPinEx", OutputStr);

        return OutputStr;

    }
	
	
		public String GetPIN_With_3DESDUKPTEx(int KeySet,  int KeyIndex,  String pInData, int pinBypassAllow) throws InterruptedException {
	
	
			inKeySet = KeySet;
			inKeyIndex = KeyIndex;
			StrInData = pInData;	
			inPinBypassAllow = pinBypassAllow;
			Enter_Press = 0;
			inKeyType = 1;

	
			Log.i("GetPIN_With_3DESDUKPTEx", inKeySet+"");
			Log.i("GetPIN_With_3DESDUKPTEx", inKeyIndex+"");
			Log.i("GetPIN_With_3DESDUKPTEx", StrInData+"");
	
			kmsui.init(MainActivity.this, inKeySet, inKeyIndex, StrInData, inKeyType, inPinBypassAllow);
	
	
			while(MainActivity.Enter_Press==0)
			{
				//Log.d(TAG_A, MainActivity.Enter_Press+"");
				Thread.sleep(500);
			}
	
			Log.d("GetPIN_With_3DESDUKPTEx", MainActivity.Enter_Press+"");
	
	  
			OutputStr = String.valueOf(MainActivity.inRet)+'*'+sPinBlock+'|'+sKSN;
	
			Log.i("GetPIN_With_3DESDUKPTEx", OutputStr);
		  
			return OutputStr;
	
		}

//test
   
   public String GetSerialNumber(String text) throws InterruptedException {
	CtSystem system = new CtSystem(); 

	
	Log.i("saturn GetSerialNumber", OutputStr);
	// getFactorySN 	
	byte buf[] = new byte[17]; 
	
	try {			  
	buf = system.getFactorySN();			   
	Log.d(TAG, String.format("saturn FactorySN = %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", buf[0], buf[1], buf[2], 
		                       buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], buf[9] , 
		                       buf[10], buf[11], buf[12], buf[13], buf[14], buf[15]));		

   OutputStr = String.format("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", buf[0], buf[1], buf[2], 
									  buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], buf[9] , 
									  buf[10], buf[11], buf[12], buf[13], buf[14], buf[15]);	   
	
	} catch (CtSystemException e) 
	{e.showStatus();		  
	}   

    return OutputStr;

   }

//test
    public String GetExpiryDate(String text) throws InterruptedException {


        String user_str="";
        pass_in_string = text;

        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();
                    intent.setClass(MainActivity.this, GetExpiryDateUI.class);
                    //intent.setClass(MainActivity.this, UserInputString.class);


                    //bundle.putString("PARAM_DISPLAY", "PASSWORD");
                    //bundle.putString("PARAM_MINLENGTH", "1");
                    //bundle.putString("PARAM_MAXLENGTH", "6");
                    //bundle.putString("PARAM_TYPE", "1");
                    //bundle.putString("PARAM_PASSWORD", "123456");

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        user_str = GetExpiryDateUI.final_string;

        Log.i("TINE", user_str);
        Log.i("TINE", "FINISH GetExpiryDateUI");
        //pin_num = pin_num + "XXX";
//        Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
        return user_str;

    }

		public String GetCVV(String text) throws InterruptedException {
	
	
			String user_str="";
			pass_in_string = text;
	
			new Thread()
			{
				public void run()
				{
					try
					{
						Intent intent = new Intent();
						Bundle bundle = new Bundle();
						intent.setClass(MainActivity.this, GetCVVUI.class);
						//intent.setClass(MainActivity.this, UserInputString.class);
	
	
						//bundle.putString("PARAM_DISPLAY", "PASSWORD");
						//bundle.putString("PARAM_MINLENGTH", "1");
						//bundle.putString("PARAM_MAXLENGTH", "6");
						//bundle.putString("PARAM_TYPE", "1");
						//bundle.putString("PARAM_PASSWORD", "123456");
	
						bundle.putString("pass_in_string", pass_in_string);
						Log.i("pass_in_string", pass_in_string);
	
						intent.putExtras(bundle);
						startActivity(intent);
						//startActivityForResult(intent, 0);
					}
					catch(Exception e)
					{
						e.printStackTrace();
					}
				}
			}.start();
	
			MainActivity.LOCK.setCondition(false);
	
			synchronized (LOCK) {
				while (!LOCK.conditionMet()) {
					try {
						LOCK.wait();
					} catch (InterruptedException e) {
						Log.e(TAG, "Exception when waiting for condition", e);
						return "Exception";
					}
				}
			}
	
			user_str = GetCVVUI.final_string;
	
			Log.i("TINE", user_str);
			Log.i("TINE", "FINISH GetCVVUI");
			//pin_num = pin_num + "XXX";
	//		  Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
			return user_str;
	
		}


    public String inDCCFxRateUI(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;
        if(cardentrytimer != null)
            cardentrytimer.cancel();

        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();

                    intent.setClass(MainActivity.this, DCCFxRateUI.class);

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        user_str = DCCFxRateUI.final_string;

        Log.i("PATRICK", user_str);
        Log.i("PATRICK", "FINISH inDCCFxRateUI");
        //pin_num = pin_num + "XXX";
//        Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
        return user_str;
    }

    public String inDCCFxRateUI2(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;
        if(cardentrytimer != null)
            cardentrytimer.cancel();

        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();

                    intent.setClass(MainActivity.this, DCCFxRateUI2.class);

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        user_str = DCCFxRateUI2.final_string;

        Log.i("PATRICK", user_str);
        Log.i("PATRICK", "FINISH inDCCFxRateUI");
        //pin_num = pin_num + "XXX";
//        Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
        return user_str;
    }

	public String AppPackageInfo (String text) {

        int x=0;
        String szCRC = "";
		String szAppCRC = "";
        szAppCRC = text;
        Log.i("TINE", "getAppInfor - START");
		Log.i("szAppCRC", ""+szAppCRC);
        GetAppInfor getAppInfor = new GetAppInfor(mContext);
        //String currentSignature = getAppInfor.getInstalledAppHashCode();
        String currentSignature = getAppInfor.getAppHashCode(szAppCRC);
        Log.i("currentSignature", "" + currentSignature);
        //Toast.makeText(getApplicationContext(), "currentSignature " + currentSignature, Toast.LENGTH_SHORT).show();
        x=0;
        for(int i=0; i<9; i++)
        {
            szCRC = szCRC + currentSignature.substring(x, x+1);
            x = x+4;
        }

        Log.i("szCRC", ""+szCRC);
        return szCRC;
	}

    /*sidumili: modification to display text and image*/
	public String DisplayBox(String text) throws InterruptedException {

        String user_str="";
        in_string = text;
        if(cardentrytimer != null)
            cardentrytimer.cancel();

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                overridePendingTransition(0, 0); // disable the animation, faster

                if(MainActivity.inTermType == 5)
                    setContentView(R.layout.displaybox_message_s1em);
				else
					setContentView(R.layout.displaybox_message);

                //Intent intent=getIntent();
                //String dispmsg=intent.getStringExtra("pass_in_string");
                //Log.i("dispmsg", dispmsg);

                String dispmsg = in_string;
                Log.i(TAG, dispmsg);

                TextView box_msg;
                TextView box_msg2;
                TextView box_msg3;
                ImageView imageView;

                String[] dispmsginfo = dispmsg.split("\\|");
                int msgcnt = dispmsginfo.length;
                String image_str = "";
                String rebootRCF = "";

                // Show status bar
                getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);

                for (int inIdx = 0; inIdx < msgcnt; inIdx++) {
                    System.out.println("split msg [" + inIdx + "][" + dispmsginfo[inIdx] + "]");
                    switch (inIdx) {
                        case 0: // Message 1
                            box_msg = (TextView) findViewById(R.id.box_msg);
                            box_msg.setText(dispmsginfo[inIdx]);
                            break;
                        case 1: // Message 2
                            box_msg2 = (TextView) findViewById(R.id.box_msg2);
                            box_msg2.setText(dispmsginfo[inIdx]);
                            break;
                        case 2: // Message 3
                            box_msg3 = (TextView) findViewById(R.id.box_msg3);
                            box_msg3.setText(dispmsginfo[inIdx]);
                            break;
                        case 3: // Image Icon
                            image_str = dispmsginfo[inIdx];
                            System.out.println("image_str [" + image_str + "]");
                            imageView = (ImageView) findViewById(R.id.imageView);
                            wub_lib.ViewImageResourcesByType(image_str, imageView); // sidumili: added to call function in any java, code optimization
                            break;
                        /*case 4: //AAA restart feature
                            rebootRCF = dispmsginfo[inIdx];
                            if(rebootRCF.equals("READ CARD FAILED")) {
                                Toast.makeText(getApplicationContext(), "Terminal will restart please wait...", Toast.LENGTH_SHORT).show();
                                //AAA
                                int secs = 3;
                                DelayCode mDelayCode = new DelayCode();

                                mDelayCode.delay(secs, new DelayCode.DelayCallback() {
                                    @Override
                                    public void afterDelay() {

                                        // AAA after delay do this code
                                        CtSystem system = new CtSystem();
                                        byte bmode = 0;// reboots
                                        Log.d("PATRICK", String.format("return = %X", system.shutdown(bmode)));

                                    }
                                });
                            }
                            break;*/
                    }
                }

                if(image_str.equals("timeout")) {
                    try {
                        AnimateTimeout();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
                else {
                    try {
                        AnimationMessageBox();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        });
        /*
        synchronized (MainActivity.LOCK) {
            MainActivity.LOCK.setCondition(true);
            MainActivity.LOCK.notifyAll();

        }

         

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }
		*/

        return user_str;
    }

    public String DisplayStatusBox(String text) throws InterruptedException {

        String user_str="";
        in_string = text;
        if(cardentrytimer != null)
            cardentrytimer.cancel();

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                overridePendingTransition(0, 0); // disable the animation, faster

                
                if(MainActivity.inTermType == 5)
                    setContentView(R.layout.displaystatusbox_message_s1em);
                else
                    setContentView(R.layout.displaystatusbox_message);
		
                //Intent intent=getIntent();
                //String dispmsg=intent.getStringExtra("pass_in_string");
                //Log.i("dispmsg", dispmsg);

                String dispmsg = in_string;
                Log.i(TAG, dispmsg);

                TextView box_msg;
                TextView box_msg2;
                ImageView imageView;

                String[] dispmsginfo = dispmsg.split("\\|");
                int msgcnt = dispmsginfo.length;
                String image_str = "";

                // Show status bar
                getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
                
                for (int inIdx = 0; inIdx < msgcnt; inIdx++) {
                    System.out.println("split msg [" + inIdx + "][" + dispmsginfo[inIdx] + "]");
                    switch (inIdx) {
                        case 0: // Message 1
                            box_msg = (TextView) findViewById(R.id.box_msg);
							System.out.println("Processing    yyyy");
                            box_msg.setText(dispmsginfo[inIdx]);
							
						    if(box_msg.getText().toString().equals("Processing  "))
					        {
					        	//getWindow().setFlags(WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE, WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE);
                                System.out.println("Processing    xxxx");
					        }
							System.out.println("Processing    zzzz");
                            break;
                        case 1: // Message 2
                            box_msg2 = (TextView) findViewById(R.id.box_msg2);
                            box_msg2.setText(dispmsginfo[inIdx]);
                            break;
                        case 2: // Image Icon
                            image_str = dispmsginfo[inIdx];
                            System.out.println("image_str [" + image_str + "]");
                            imageView = (ImageView) findViewById(R.id.imageView);
                            wub_lib.ViewImageResourcesByType(image_str, imageView); // sidumili: added to call function in any java, code optimization
                            break;
                    }
                }

                if(image_str.equals("process") || image_str.equals("wait") || image_str.equals("init"))
                    AnimateProcessing();
                else
                    AnimationMessageStatusBox();
            }
        });
        /*
        synchronized (MainActivity.LOCK) {
            MainActivity.LOCK.setCondition(true);
            MainActivity.LOCK.notifyAll();

        }

         */

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        return user_str;
    }

    public void AnimationMessageBox() throws InterruptedException {
        TextView box_msg;
        TextView box_msg2;
        TextView box_msg3;
        ImageView imageView;

        // declare animation
        final Animation stb = AnimationUtils.loadAnimation(this, R.anim.stb);
        final Animation ttb = AnimationUtils.loadAnimation(this, R.anim.ttb);

        imageView = (ImageView) findViewById(R.id.imageView);
        box_msg = (TextView) findViewById(R.id.box_msg);
        box_msg2 = (TextView) findViewById(R.id.box_msg2);
        box_msg3 = (TextView) findViewById(R.id.box_msg3);

        // set animaiton
        imageView.startAnimation(stb);

        box_msg.startAnimation(ttb);
        box_msg2.startAnimation(ttb);
        box_msg3.startAnimation(ttb);
		//Thread.sleep(50);

        synchronized (MainActivity.LOCK) {
            MainActivity.LOCK.setCondition(true);
            MainActivity.LOCK.notifyAll();

        }

    }

    public void AnimationMessageStatusBox()
    {
        TextView box_msg;
        TextView box_msg2;
        ImageView imageView;

        // declare animation
        final Animation stb = AnimationUtils.loadAnimation(this, R.anim.stb);
        final Animation ttb = AnimationUtils.loadAnimation(this, R.anim.ttb);

        imageView = (ImageView) findViewById(R.id.imageView);
        box_msg = (TextView) findViewById(R.id.box_msg);
        box_msg2 = (TextView) findViewById(R.id.box_msg2);

        // set animaiton
        imageView.startAnimation(stb);

        box_msg.startAnimation(ttb);
        box_msg2.startAnimation(ttb);

        synchronized (MainActivity.LOCK) {
            MainActivity.LOCK.setCondition(true);
            MainActivity.LOCK.notifyAll();

        }

    }

    public void AnimateProcessing()
    {
        TextView box_msg;
        TextView box_msg2;
        ImageView imageView;

        // declare animation
        final Animation stb = AnimationUtils.loadAnimation(this, R.anim.clockwise);
        final Animation ttb = AnimationUtils.loadAnimation(this, R.anim.ttb);

        imageView = (ImageView) findViewById(R.id.imageView);
        box_msg = (TextView) findViewById(R.id.box_msg);
        box_msg2 = (TextView) findViewById(R.id.box_msg2);

        // set animaiton
        imageView.startAnimation(stb);

        box_msg.startAnimation(ttb);
        box_msg2.startAnimation(ttb);

        synchronized (MainActivity.LOCK) {
            MainActivity.LOCK.setCondition(true);
            MainActivity.LOCK.notifyAll();

        }

    }

    public void AnimateTimeout() throws InterruptedException {
        TextView box_msg;
        TextView box_msg2;
        TextView box_msg3;
        ImageView imageView;

        // declare animation
        final Animation stb = AnimationUtils.loadAnimation(this, R.anim.halfturn);
        final Animation ttb = AnimationUtils.loadAnimation(this, R.anim.ttb);

        imageView = (ImageView) findViewById(R.id.imageView);
        box_msg = (TextView) findViewById(R.id.box_msg);
        box_msg2 = (TextView) findViewById(R.id.box_msg2);
        box_msg3 = (TextView) findViewById(R.id.box_msg3);

        // set animaiton
        imageView.startAnimation(stb);

        box_msg.startAnimation(ttb);
        box_msg2.startAnimation(ttb);
        box_msg3.startAnimation(ttb);

		//Thread.sleep(50);

        synchronized (MainActivity.LOCK) {
            MainActivity.LOCK.setCondition(true);
            MainActivity.LOCK.notifyAll();

        }


    }

	public String DisplayErrorMsg2(String text) throws InterruptedException {

        String user_str="";
        in_string = text;

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                overridePendingTransition(0, 0); // disable the animation, faster

                setContentView(R.layout.display_errorbox2);

                //Intent intent=getIntent();
                //String dispmsg=intent.getStringExtra("pass_in_string");
                //Log.i("dispmsg", dispmsg);

                String dispmsg = in_string;
                Log.i(TAG, dispmsg);

                TextView error_msg1;
                TextView error_msg2;
                TextView error_msg3;

                // Show status bar
                getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);

                String[] dispmsginfo = dispmsg.split("\\|");
                int msgcnt = dispmsginfo.length;

                error_msg1 = (TextView) findViewById(R.id.error_msg1);
                error_msg2 = (TextView) findViewById(R.id.error_msg2);
                error_msg3 = (TextView) findViewById(R.id.error_msg3);

                error_msg1.setText("");
                error_msg2.setText("");
                error_msg3.setText("");

                try
                {
                    //to do

                    for (int inIdx = 0; inIdx < msgcnt; inIdx++) {
                        System.out.println("split msg [" + inIdx + "][" + dispmsginfo[inIdx] + "]");
                        switch (inIdx) {
                            case 0:
                                error_msg1.setText(dispmsginfo[inIdx]);
                                break;
                            case 1:
                                error_msg2.setText(dispmsginfo[inIdx]);
                                break;
                            case 2:
                                error_msg3.setText(dispmsginfo[inIdx]);
                                break;
                        }
                    }
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }

            }

        });
        synchronized (MainActivity.LOCK) {
            MainActivity.LOCK.setCondition(true);
            MainActivity.LOCK.notifyAll();

        }
        return user_str;
    }

    public String fGetConnectStatus(String text) {

        String user_str="";
        //in_string = text;
        ConnectivityManager cm = (ConnectivityManager) mContext.getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo activeNetwork = cm.getActiveNetworkInfo();
        if (activeNetwork != null) {
            Log.i("saturn", "AAA activeNetwork is active");
            Log.i("saturn", "AAA text.equals " + text);
            // connected to the internet
            if (text.equals("4")) {
                if (activeNetwork.getType() == ConnectivityManager.TYPE_WIFI) {
                    // connected to wifi
                    Log.i("saturn", "AAA active getType is WIFI");
                    user_str = "YES";
                } else if (activeNetwork.getType() == ConnectivityManager.TYPE_MOBILE) {
                    // connected to mobile data
                    Log.i("saturn", "AAA active getType is mobile");
                    user_str = "FALLBACK";
                }
            }
            if (text.equals("2"))
            {
                Log.i("saturn", "Albert forced ye");
                //if (activeNetwork.getType() == ConnectivityManager.TYPE_MOBILE)
                    user_str = "YES";
                //else
                    //user_str = "NO";
            }
        } else {
            // not connected to the internet
            Log.i("saturn", "AAA active getType is no connectivity");
            user_str = "NONETWORK";
        }

 /*
			Log.i("saturn", "saturn GetConnectionStatus text " + text);
		GetNetworkInfor getNetworkInfor = new GetNetworkInfor(mContext);


        if (text.equals("4")){

			Log.i("saturn", "saturn wifi mode");
			String wifiSSID = getNetworkInfor.getWifiSSID();

			if (wifiSSID != null)
				user_str = "YES";
			else
				user_str = "NO";


        }else{
        Log.i("saturn", "saturn gprs  mode");
			Boolean isonline = getNetworkInfor.isOnline();
			if (isonline == true)
				user_str = "YES";
			else
				user_str = "NO";
		}

		Log.i("saturn", "saturn test GetConnectionStatus: " + user_str);
	*/
        return user_str;

    }

	
	public String GetWIFISettings(String text) {

		String user_str="";
		GetNetworkInfor getNetworkInfor = new GetNetworkInfor(mContext);


		Log.i("saturn", "saturn GetWIFISettings");

		user_str = getNetworkInfor.GetIPSettings();
		
		Log.i("saturn", "saturn GetWIFISettings: " + user_str);
	
	 	return user_str;
	
	}

	
	public String GetConnectionType(String text) {

		String user_str="";
		GetNetworkInfor getNetworkInfor = new GetNetworkInfor(mContext);


		Log.i("saturn", "saturn GetConnectionType");


		user_str = getNetworkInfor.getNetworkType();

		
		Log.i("saturn", "saturn GetConnectionType: " + user_str);
	
	 	return user_str;
	
	}

	public String GetBatteryLevel(String text) {

		String user_str="";
		String bat_level="";
		String charging="";

		BatteryManager bm = (BatteryManager)getSystemService(BATTERY_SERVICE);
		


		IntentFilter ifilter = new IntentFilter(Intent.ACTION_BATTERY_CHANGED);
		Intent batteryStatus = mContext.registerReceiver(null, ifilter);

        int status = batteryStatus.getIntExtra(BatteryManager.EXTRA_STATUS, -1);
		boolean isCharging = status == BatteryManager.BATTERY_STATUS_CHARGING;
		
		if (isCharging == true)
			charging = "1";
		else
			charging = "0";

		Log.i("saturn", "saturn isCharging: " + isCharging);

		//if (android.os.Build.VERSION.SDK_INT > = android.os.Build.VERSION_CODES.LOLLIPOP) {
		int percentage = bm.getIntProperty(BatteryManager.BATTERY_PROPERTY_CAPACITY);
		bat_level = String.valueOf(percentage);
		//}

		user_str = charging+bat_level;

		
		Log.i("saturn", "saturn GetBatteryLevel: " + user_str);
	
	 	return user_str;
	
	}

	public String AnimateReceiptPrint(String text) throws InterruptedException {

        String user_str="";
        in_string = text;

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                overridePendingTransition(0, 0); // disable the animation, faster

                setContentView(R.layout.lcddisplay_message);

                //Intent intent=getIntent();
                //String dispmsg=intent.getStringExtra("pass_in_string");
                //Log.i("dispmsg", dispmsg);

                String dispmsg = in_string;
                Log.i(TAG, dispmsg);

                TextView textViewmsg;
                TextView textView_dtxn;
                TextView textView_cur;
                TextView textView_opt1;
                TextView textView_opt2;

                // Show status bar
                getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);


                String[] dispmsginfo = dispmsg.split("\\|");
                int msgcnt = dispmsginfo.length;

                textViewmsg = (TextView) findViewById(R.id.textView_msg);
                textView_dtxn = (TextView) findViewById(R.id.textView_dtxn);
                textView_cur = (TextView) findViewById(R.id.textView_cur);
                textView_opt1 = (TextView) findViewById(R.id.textView6);
                textView_opt2 = (TextView) findViewById(R.id.textView7);

                try
                {
                    //to do

                    for (int inIdx = 0; inIdx < msgcnt; inIdx++) {
                        System.out.println("split msg [" + inIdx + "][" + dispmsginfo[inIdx] + "]");
                        switch (inIdx) {
                            case 0:
                                textView_dtxn.setText(dispmsginfo[inIdx]);
                                break;
                            case 1:
                                textViewmsg.setText(dispmsginfo[inIdx]);
                                break;
                            case 2:
                                textView_cur.setText(dispmsginfo[inIdx]);
                                break;
                            case 3:
                                textView_opt1.setText(dispmsginfo[inIdx]);
                                break;
                            case 4:
                                textView_opt2.setText(dispmsginfo[inIdx]);
                                break;
                        }
                    }
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }

            }

        });
        synchronized (MainActivity.LOCK) {
            MainActivity.LOCK.setCondition(true);
            MainActivity.LOCK.notifyAll();

        }
		user_str = "AnimateReceipt_DONE";
        return user_str;
    }

    public String GetMOBILESettings(String text) {

        String user_str="";
        String sInfoPart1 = "";
        String sInfoPart2 = "";
        String sMobileInfo = "";

        GetNetworkInfor getNetworkInfor = new GetNetworkInfor(mContext);
        Log.i("saturn", "saturn GetMOBILESettings");

        //sidumili: Get Terminal IP, Gateway, SubNetMask
        sInfoPart1 = getNetworkInfor.GetDeviceipMobileData();
        Log.i("saturn", "saturn GetDeviceipMobileData: sInfoPart1=" + sInfoPart1);

        //sidumili: Get DNS 1, DNS 2
        sInfoPart2 = getNetworkInfor.GetDNSServer(this);
        Log.i("saturn", "saturn GetDNSServer: sInfoPart2=" + sInfoPart2);

        sMobileInfo = sInfoPart1 + sInfoPart2;
        Log.i("saturn", "saturn GetMOBILESettings: sMobileInfo=" + sMobileInfo);

        user_str = sMobileInfo;

        return user_str;

    }

    // sidumili: added for edit host
    public String EditInfoListViewUI(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;
        Log.i("EditInfoListViewUI:", "1");
        new Thread()
        {
            public void run()
            {
                try
                {
                    EditListView.isResume = false;
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();

                    Log.i("EditInfoListViewUI:", "2");
                    intent.setClass(MainActivity.this, EditListView.class);
                    Log.i("EditInfoListViewUI:", "3");
                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        user_str = EditListView.final_string;

        Log.i("PATRICK", user_str);
        Log.i("PATRICK", "FINISH EditInfoListViewUI");
        //pin_num = pin_num + "XXX";
//        Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
        return user_str;
    }

    public String InputQWERTYUI(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;


        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();
                    intent.setClass(MainActivity.this, QwertyUI.class);

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        user_str = QwertyUI.final_string;

        Log.i("TINE, user_str", user_str);
        Log.i("TINE, user_str", "FINISH InputQWERTYUI");
        //pin_num = pin_num + "XXX";
//        Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
        return user_str;

    }

    private void wakeUpScreen() {
        PowerManager pm = (PowerManager) this.getSystemService(Context.POWER_SERVICE);
        boolean screenOn = pm.isScreenOn();
        Log.i(TAG, "screenOn: " + screenOn);
        if(!screenOn) {
            // 获取PowerManager.WakeLock对象,后面的参数|表示同时传入两个值,最后的是LogCat里用的Tag
            PowerManager.WakeLock wl = pm.newWakeLock(PowerManager.ACQUIRE_CAUSES_WAKEUP | PowerManager.SCREEN_BRIGHT_WAKE_LOCK, "bright");
            wl.acquire(); // 点亮屏幕
            wl.release(); // 释放
        }
    }

    // -----------------------------------------------------------------------------------------------
    // UserManual -- sidumili
    // -----------------------------------------------------------------------------------------------
    public String UserManual(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;

        // Init object
        objSelected = new ObjSelected(sDefaultInt, sDefaultNull, false, text, sDefaultNull, false, 0, false, sDefaultNull);
        objSelected.setCompleted(false);
        objSelected.setCancelled(false);
        objSelected.setContinue(true);

        boolean isExitLoop = false;
        boolean isCancelled = false;
        boolean isCompleted = false;
        boolean isContinue = true;

        do
        {
            isContinue = objSelected.isContinue();
            Log.i(TAG, "UserManual: Loop, isContinue="+ objSelected.isContinue());

            if (isContinue)
            {
                Log.i("saturn UserManual:", "run");
                new Thread()
                {
                    public void run()
                    {
                        try
                        {
                            Intent intent = new Intent();
                            Bundle bundle = new Bundle();
                            intent.setClass(MainActivity.this, UserManual.class);
                            bundle.putString("pass_in_string", pass_in_string);
                            Log.i("pass_in_string", pass_in_string);

                            intent.putExtras(bundle);
                            startActivity(intent);
                        }
                        catch(Exception e)
                        {
                            e.printStackTrace();
                        }
                    }
                }.start();

                MainActivity.LOCK.setCondition(false);

                synchronized (LOCK) {
                    while (!LOCK.conditionMet()) {
                        try {
                            LOCK.wait();
                        } catch (InterruptedException e) {
                            Log.e(TAG, "Exception when waiting for condition", e);
                            return "Exception";
                        }
                    }
                }

                Log.i(TAG, "UserManual: isCancelled="+objSelected.isCancelled());
                if (objSelected.isCancelled())
                {
                    user_str = "0|000000|CANCEL";
                }

            }
            else
            {
                isExitLoop = true;
            }

        }
        while (!isExitLoop);


        Log.i("saturn", "saturn user_str ="+user_str);
        Log.i("saturn", "saturn FINISH UserManual");

        return user_str;
    }

    /**
     * To check whether app is running or not
     * @param context
     * @param PackageName, eg:com.persistent.app
     * @return
     */
    public boolean isApplicationStarted(Context context,String PackageName) {
        //<uses-permission android:name="android.permission.GET_TASKS"/>
        boolean isStarted =false;
        Log.i(TAG, "isApplicationStarted: " + PackageName);
        try {
            ActivityManager mActivityManager = (ActivityManager)context.getSystemService(Context.ACTIVITY_SERVICE);
            int intGetTastCounter = 1000;
            List<ActivityManager.RunningServiceInfo> mRunningService = mActivityManager.getRunningServices(intGetTastCounter );
            for (ActivityManager.RunningServiceInfo amService : mRunningService) {
                if(0 == amService.service.getPackageName().compareTo(PackageName)) {
                    isStarted = true;
                    break;
                }
            }
        }
        catch(SecurityException e) {
            e.printStackTrace();
        }
        Log.i(TAG, "isStarted: " + isStarted);
        return isStarted;
    }

	public boolean isRun(Context context,String PackageName) {
		 ActivityManager am = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
		 List<ActivityManager.RunningTaskInfo> list = am.getRunningTasks(MAX_VALUE);
		 boolean isAppRunning = false;
		 String MY_PKG_NAME = PackageName;
		 Log.i(TAG, "isRun: " + MY_PKG_NAME);
	
		 final List<ActivityManager.RunningServiceInfo> services = am.getRunningServices(Integer.MAX_VALUE);
		 for (ActivityManager.RunningServiceInfo runningServiceInfo : services) {
			 // Log.d(TAG, String.format("Service:%s", runningServiceInfo.service.getClassName()));
			 if (runningServiceInfo.service.getClassName().contains(MY_PKG_NAME)){
				 isAppRunning = true;
				 Log.i("ActivityService isRun()", runningServiceInfo.service.getClassName());
				 break;
			 }
		 }
	
		 Log.i("ActivityService isRun()", "com.ad ...isAppRunning..... " + isAppRunning);
		 if (isAppRunning == true)
			 return isAppRunning;
	
		 MY_PKG_NAME = PackageName;
		 Log.i(TAG, "isRun: " + MY_PKG_NAME);
	
		 for (ActivityManager.RunningTaskInfo info : list) {
			 // Log.d(TAG, String.format("Activity:%s", info.topActivity.getPackageName()));
			 // Log.d(TAG, String.format("Activity:%s", info.baseActivity.getPackageName()));
			 if (info.topActivity.getPackageName().equals(MY_PKG_NAME) || info.baseActivity.getPackageName().equals(MY_PKG_NAME)) {
				 isAppRunning = true;
				 Log.i("Activity isRun()", info.topActivity.getPackageName() + " info.baseActivity.getPackageName()=" + info.baseActivity.getPackageName());
				 break;
			 }
		 }
	
		 return isAppRunning;
	 }

    private boolean isCTMSParameterFileExist() {
        String packageName = getPackageName();
        String packagePath =  "/data/data/" + getPackageName() + "/files/" + packageName.toUpperCase() + ".prm";
        Log.i(TAG, "packagePath: " + packagePath);
        File file = new File(packagePath);
        if(file.exists()) {
            Log.i(TAG, "reset FIRSTRUN: ");
            return true;
        }
        return false;
    }

    public void scheduleTaskStop()
    {
        Log.i(TAG, "scheduleTaskStop1: " + (mScheduledExecutorService == null) + " mCount:" + mCount + "  isActivtyFront:" + isActivtyFront + "FLAG_IDLE_FUN:" + FLAG_IDLE_FUN);

        if(FLAG_IDLE_FUN)
        {
            mCount = 0;
            if(mScheduledExecutorService != null) {

                Log.i(TAG, "isShutdown: " + mScheduledExecutorService.isShutdown() + " isTerminated:" + mScheduledExecutorService.isTerminated());
                try {
//                mScheduledExecutorService.shutdown();
                    mScheduledExecutorService.shutdownNow();
                    Log.i(TAG, "isShutdown: " + mScheduledExecutorService.isShutdown() + " isTerminated:" + mScheduledExecutorService.isTerminated());

                    while(!mScheduledExecutorService.awaitTermination(3, TimeUnit.MILLISECONDS)){
                        mScheduledExecutorService.shutdownNow();
                    }
                } catch (InterruptedException e) {
                    // awaitTermination方法被中断的时候也中止线程池中全部的线程的执行。
                    Log.i(TAG, "awaitTermination interrupted: ");
                    mScheduledExecutorService.shutdownNow();
                }
                Log.i(TAG, "Thread pool not shut down yet");
            }
            if(mScheduledExecutorService != null) {
                Log.i(TAG, "scheduleTaskStop isShutdown: " + mScheduledExecutorService.isShutdown() + " isTerminated:" + mScheduledExecutorService.isTerminated());
            }
            //        mScheduledExecutorService = null;
        }
        return ;
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
        //this.mWakeLock.release();
        super.onDestroy();
        releaseAll();
        Log.i(TAG, "****S1_MCC onDestroy: ****");
        mHandler = null;
        mContext = null;
        SysApplication.getInstance().exit();
    }

    private void releaseAll() {
        Log.i(TAG, "MCC releaseAll: ");
      /*  if(myTimer != null)
            myTimer.cancel();
        myTimer = null;
        if(myTimerTask != null)
            myTimerTask.cancel();
        myTimerTask = null;
        if(mScheduledExecutorService != null)
            mScheduledExecutorService.shutdown();
        mScheduledExecutorService = null;*/
        AppHeaderBuff = null;
        final_string = null;
//        Enter_Press = 0;
//        inRet = 0;
        PinBlock = null;
        KSN = null;

        sPinBlock = null;
        sKSN = null;
        OutputStr = null;

        mContext = null;
        mHandler = null;

        LOCK = null;
        jni_api = null;
        jni_api_offline_pin = null;
        jni_cb_ctos = null;
        jni_cb_emv = null;

        lsv_LCDTPrint = null;
        adapter = null;
        mPager = null;
        //mDatas.clear();
        mDatas = null;
        //mPagerList.clear();GetAmountString
        mPagerList = null;
        mLlDot = null;
        inflater = null;

/*        jni_api.REG_CB_CTOS(jni_cb_ctos);
        jni_api_offline_pin.REG_CB_EMV(jni_cb_emv);*/
        Log.i(TAG, "releaseAll done.: ");
    }

    int inScanWIFI=0;
	int inSystemPanel=0;
	
	public void GetMenuEx(){

		final String MenuBuff;//, AppHeaderBuff;
		String TrxMenuItems, AppDetail1="", AppDetail2="", TrxImageId, AppDetail, Networknm;
		String MainHeader;
		String[] TrxTitle = new String[100];
		String[] AppHeader = new String[100];
		int inLoop=0;
		String rstatus="0";
		String idleMode = "0";
        String sCommMode = "";
			
        File image_file;
        Bitmap myimage;
		String dcc_image_path;
		
		fSTART = false;
		idlecardentry_status = 0;
		szMSR_status="MSR_NO";

		//displayNotification("11");
		//displayNotification("33");
		inTermType=inCTOSS_GetEnvInt("TERM_TYPE");;
		
		//CtSystem system = new CtSystem();
		//system.setNavBarIcon(false,false,false);

		final PowerManager pm2 = (PowerManager) getSystemService(Context.POWER_SERVICE);

		if(receipt_bmp_file != null) {
			receipt_bmp_file = null;		//tine:  clear content
			Log.i("Saturn Main", "receipt_bmp_file cleared...");
		}

		if(myBitmap != null) {
			myBitmap.recycle();
			myBitmap = null;				//tine:  clear content
			Log.i("Saturn Main", "myBitmap cleared...");
		}

		Log.i("AAA shutdown", "Reboot " + Reboot);


		if(Reboot.equals("REBOOT_YES")) {
			new Thread() {
				@Override
				public void run() {
					try {
						Log.i("shutdown", "shutdown START");
						CtSystem system = new CtSystem();
						byte bmode = 0;// shutdown
						Log.d("PATRICK", String.format("return = %X", system.shutdown(bmode)));
						//MainActivity.this.finish();
					} catch (Exception e) {
						Log.d("Castles", "SHUTDOWN encountered ERROR");
					}
				}
			}.start();
		} else {
			//vdClearFile("/data/data/com.Source.S1_MCC.MCC/DYNAMICMENU120.S3DB");
			
			Log.i("saturn ", "GetMenu - START");
			Log.i("saturn", "fStart " +fSTART + "idlecardentry_status " + idlecardentry_status);
			/*
			GetAppInfor getAppInfor = new GetAppInfor(mContext);
			String currentSignature = getAppInfor.getInstalledAppHashCode();
			Log.i("currentSignature", "" + currentSignature);
			*/
			//Toast.makeText(getApplicationContext(), "currentSignature " + currentSignature, Toast.LENGTH_SHORT).show();


			AppHeaderBuff = GetAppHeaderDetails();

			MainHeader = AppHeaderBuff;
			Log.i("TINE", AppHeaderBuff);
			AppHeader = MainHeader.split(" \n");
			int msgcnt = AppHeader.length;
/*
			OperatorHolder operatorHolder = new OperatorHolder(this);
			WifiManager wifiManager = (WifiManager) getApplicationContext().getSystemService(Context.WIFI_SERVICE);
			WifiInfo info = wifiManager.getConnectionInfo ();
			String ssid  = info.getSSID();
*/
			View decorView = getWindow().getDecorView();
			//decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_LAYOUT_STABLE);
			decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
				 | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);

			inMENUTYPE = 0;
            if (AppHeader[6].equals("COMPLETEQR")) {
                //inMENUTYPE = 1;
                if(inTermType == 5)
					setContentView(R.layout.dmenu_activity_main_complete_s1em);
				else
                    setContentView(R.layout.dmenu_activity_main_complete);
            }
            else{
				if(inTermType == 5)
					setContentView(R.layout.dmenu_activity_main_incomplete_s1em);
				else
                    setContentView(R.layout.dmenu_activity_main_incomplete);
            }

			app_version = (TextView) findViewById(R.id.main_app_version);
			merchant = (TextView) findViewById(R.id.main_merchant);
			tprofile = (TextView) findViewById(R.id.main_tprofile);
			btn_start = (Button) findViewById(R.id.btn_start);
			carrier_name = (TextView) findViewById(R.id.main_NetworkCarrier);
			idlemsgbox = (TextView) findViewById(R.id.msg_main1);
			btn_main_here = (Button) findViewById(R.id.btn_here);

			mcc_menu_title = (TextView) findViewById(R.id.mcc_menu_title);
			btn_reports = (Button) findViewById(R.id.btn_reports);	
			btn_settle = (Button) findViewById(R.id.btn_settle);
			btn_settings = (Button) findViewById(R.id.btn_settings);

			btn_credit = (Button) findViewById(R.id.btn_credit);
			btn_bancnet = (Button) findViewById(R.id.btn_bancnet);
			btn_wallet = (Button) findViewById(R.id.btn_wallet);
			btn_phqr = (Button) findViewById(R.id.btn_phqr);
			
			btn_logon = (Button) findViewById(R.id.btn_logon);
            btn_settings2 = (Button) findViewById(R.id.btn_settings2);
			
			for (int inIdx = 0; inIdx < msgcnt; inIdx++) {
				System.out.println("split msg [" + inIdx + "][" + AppHeader[inIdx] + "]");
				switch (inIdx) {
					case 0:
						app_version.setText(AppHeader[inIdx]);
						break;
					case 1:
						merchant.setText(AppHeader[inIdx]);
						break;
					case 2:
						AppDetail1 = "MID: "+AppHeader[inIdx];
						Log.i("TINE", AppDetail1);
						break;
					case 3:
						AppDetail2 = "TID: "+AppHeader[inIdx];
						Log.i("TINE", AppDetail2);
						break;
					case 4:
						rstatus = AppHeader[inIdx];
						break;
					case 5:
						idleMode = AppHeader[inIdx];
						break;
					case 7:
                         sCommMode = AppHeader[inIdx];
                         break;
				}
			}

            if(rstatus.equals("0")) {
                btn_credit.setVisibility(View.GONE);
                btn_bancnet.setVisibility(View.GONE);
                btn_wallet.setVisibility(View.GONE);
                btn_phqr.setVisibility(View.GONE);

				btn_reports.setVisibility(View.GONE);
				btn_settle.setVisibility(View.GONE);
				btn_settings.setVisibility(View.GONE);

				btn_logon.setVisibility(View.VISIBLE);
                btn_settings2.setVisibility(View.VISIBLE);

				mcc_menu_title.setVisibility(View.INVISIBLE);
            }
            else{
				if(AppHeader[6].equals("MCCPAY")) //APPTYPE=1
				{
					btn_phqr.setVisibility(View.GONE);
				}
				else if(AppHeader[6].equals("CASHOUT")) //APPTYPE=2
				{
					btn_credit.setVisibility(View.GONE);
					btn_wallet.setVisibility(View.GONE);
					btn_phqr.setVisibility(View.GONE);
				}
				else if(AppHeader[6].equals("COMPLETEQR"))	//APPTYPE=3
				{
					
				}		
				else if(AppHeader[6].equals("QRPH")) //APPTYPE=4
				{
					btn_wallet.setVisibility(View.GONE);
				}
				else
				{
					btn_wallet.setVisibility(View.GONE);
					btn_phqr.setVisibility(View.GONE);
				}
				
				btn_logon.setVisibility(View.GONE);
                btn_settings2.setVisibility(View.GONE);

				mcc_menu_title.setVisibility(View.VISIBLE);
            }
			AppDetail = AppDetail1 + "	|  " + AppDetail2;
			tprofile.setText(AppDetail);

			if(idleMode.equals("1")) {
				btn_main_here.setVisibility(View.GONE);
				idlemsgbox.setText("Please insert/swipe card or \n press START button to begin");
			} else if(idleMode.equals("2")) {
				btn_main_here.setVisibility(View.VISIBLE);
				idlemsgbox.setText("Please insert/swipe card or \n press HERE for amount entry");
			}
/*
			Log.i("AAA Networkname", "SSID: " + ssid);

			if (ssid.equals("<unknown ssid>") || (ssid.length() <=0))
				Networknm = "Network: " + operatorHolder.getOperatorName();
			else
				Networknm = "Network: " + ssid;

			if (Networknm.equals("Network: "))
				Networknm = "Network: NOT CONNECTED";

			Log.i("AAA Networkname", "Networknm: " + Networknm);
*/
            GetNetworkInfor getNetworkInfor = new GetNetworkInfor(MainActivity.this);

            Networknm = "";
            boolean isValid = false;
            OperatorHolder operatorHolder = new OperatorHolder(this);
            Log.i(TAG, "getOperatorName=" + operatorHolder.getOperatorName());
            Log.i("GetMenu", "sCommMode=" + sCommMode);
			if (sCommMode.equals("GPRS")) 
			{
				Networknm = operatorHolder.getOperatorName();
			} 
			else if(sCommMode.equals("WIFI")) {
                getNetworkInfor.SetWIFIEnabled(true);
                SetLocationEnabled(true);

                ActivityCompat.requestPermissions(MainActivity.this,
                        new String[]{Manifest.permission.ACCESS_WIFI_STATE,
                                Manifest.permission.ACCESS_COARSE_LOCATION,
                                Manifest.permission.ACCESS_FINE_LOCATION},
                        PackageManager.PERMISSION_GRANTED);

				if(inScanWIFI == 1)
				{
                    getSSIDList();

					SetWIFI();
					inScanWIFI=0;

                    //getSSIDList2();
				}
			
				String ssid = getNetworkInfor.getSSID().replace("\"", "");
				Log.i(TAG, "wifi ssid=" + ssid);
				
				if (ssid.equals("<unknown ssid>") || (ssid.length() <= 0))
					Networknm = "Not connected";
				else
					Networknm = ssid;
			}

			if(inSystemPanel == 1)
			{
				new CtSystem().setNavigationbar(true);
				finishAffinity();
				System.exit(0);
			}

			carrier_name.setText("Network: " + Networknm);

			mPager = (ViewPager) findViewById(R.id.viewpager);

			//初始化数据源
			initDatas(idleMode);
			inflater = LayoutInflater.from(this);
			//总的页数=总数/每页数量，并取整
			pageCount = (int) Math.ceil(mDatas.size() * 1.0 / pageSize);
			mPagerList = new ArrayList<View>();
			for (int i = 0; i < pageCount; i++) {
				//每个页面都是inflate出一个新实例
				GridView gridView = (GridView) inflater.inflate(R.layout.gridview, mPager, false);
				gridView.setAdapter(new GridViewAdapter(this, mDatas, i, pageSize));
				mPagerList.add(gridView);


				gridView.setOnItemClickListener(new AdapterView.OnItemClickListener() {


					@Override
					public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
						int pos = position + curIndex * pageSize;
						//Toast.makeText(MainActivity.this, mDatas.get(pos).getName(), Toast.LENGTH_SHORT).show();

						jni_api.REG_CB_CTOS(jni_cb_ctos);
						setContentView(R.layout.activity_disp);
						lsv_LCDTPrint = (ListView) findViewById(R.id.lsv_LCDTPrint);
						edtLog = (EditText) findViewById(R.id.edtLog);
						adapter = new ArrayAdapter(MainActivity.this, R.layout.listitem, list){

							@NonNull
							@Override
							public View getView(int position, @Nullable View convertView, @NonNull ViewGroup parent) {
								View view =  super.getView(position, convertView, parent);


								TextView textView = (TextView)view;

//							  Log.d("Position", String.valueOf(position));
//							  Log.d("Padding", String.valueOf(addPadding[position]));

								if(addPadding[position] != 0)
									textView.setPadding(36*addPadding[position],0,0,0);

//							  Log.d("DisplayMode", displayMode[position]);

								if (displayMode[position].equals("0")) {
									textView.setGravity(Gravity.LEFT);
								} else if (displayMode[position].equals("1")) {
									textView.setGravity(Gravity.CENTER);
								} else if (displayMode[position].equals("2")) {
									textView.setGravity(Gravity.RIGHT);
								}


								return view;

							}
						};

						if (mDatas.get(pos).getName() == "START") {
							Log.i("Castles", "set app to START");
							idlecardentry_status = 3;
						/*
						new Thread() {
							@Override
							public void run() {
								try {

									jni_api_offline_pin.REG_CB_EMV(jni_cb_emv);
									fSTART = true;
									AppStart();
									//inCTOSS_OnlinePinTest();

									final Thread thread = new Thread(new Runnable(){

										@Override
										public void run() {
											Message message = new Message();
											message.what = 1;
											Log.d("handler.sendMessage",  "test");
											handler.sendMessageDelayed(message, 100);
										}

									});

									thread.start();
								} catch (Exception e) {

								}
							}
						}.start();
						*/
						}


					}
				});

			}

			//设置适配器
			mPager.setAdapter(new ViewPagerAdapter(mPagerList));
	/*
		   ctlsdemo = TemporaryData.getSharedPreferences().getInt(TemporaryData.CTLSDEMO, 0);
		   if(ctlsdemo == 1) {
			   mPager.setOnClickListener(this);

			   mPager.performClick();
		   }
	*/
			//设置圆点
			setOvalLayout();
//		 btnauto.performClick();

            if(rstatus.equals("0"))
            {
                Log.i("Castles", "Pending Registration");
                new Thread() {
                    @Override
                    public void run() {
                        try {
                            //jni_api.REG_CB_CTOS(jni_cb_ctos);
                            boolean fInterActive = true;

							btn_logon.setOnClickListener(new View.OnClickListener() {
								@Override
								public void onClick(View v) {
									Log.i("Castles", "set app to idle amount entry");
									idlecardentry_status = 12;
								}
							});

							btn_settings2.setOnClickListener(new View.OnClickListener() {
								@Override
								public void onClick(View v) {
									Log.i("Castles", "settings menu");
									idlecardentry_status = 5;
								}
							});
								
                            while (idlecardentry_status == 0)
                            {
                                fSTART = false;

								fInterActive = pm2.isInteractive();
								Log.i("saturn", "saturn interactive =  "+fInterActive);

								//check if sleep node;
								//if (fInterActive == false)
									//inDoAutoReboot();
                            }
                            fSTART = true;
                            MCCAppEntry();
                        } catch (Exception e) {

                        }
                    }
                }.start();
            }
			else if(rstatus.equals("1")) {
				Log.i("Castles", "Clear MSR card data from invalid swipe");
				fBlockTrans = false;
				//EcrCommandReceiver.isEcrProcessing = false;
				//CTOSS_MSRRead_inJava(); 
				//szMSR_status = "MSR_NO";
				Log.i("Castles", "Waiting for IDLE INSERT/SWIPE");
				new Thread() {
					@Override
					public void run() {
						try {
							//jni_api.REG_CB_CTOS(jni_cb_ctos);
							fSTART = false;
							btn_main_here.setOnClickListener(new View.OnClickListener() {
								@Override
								public void onClick(View v) {
									Log.i("Castles", "set app to idle amount entry");
									idlecardentry_status = 4;
								}
							});

							btn_settings.setOnClickListener(new View.OnClickListener() {
								@Override
								public void onClick(View v) {
									Log.i("Castles", "settings menu");
									idlecardentry_status = 5;
								}
							});
							
							btn_reports.setOnClickListener(new View.OnClickListener() {
								@Override
								public void onClick(View v) {
									Log.i("Castles", "reports menu");
									idlecardentry_status = 6;
								}
							});

							btn_settle.setOnClickListener(new View.OnClickListener() {
								@Override
								public void onClick(View v) {
									Log.i("Castles", "settlement menu");
									idlecardentry_status = 7;
								}
							});

							btn_credit.setOnClickListener(new View.OnClickListener() {
								@Override
								public void onClick(View v) {
									Log.i("Castles", "credit menu");
									idlecardentry_status = 8;
								}
							});

							btn_bancnet.setOnClickListener(new View.OnClickListener() {
								@Override
								public void onClick(View v) {
									Log.i("Castles", "bancnet menu");
									idlecardentry_status = 9;
								}
							});

							btn_phqr.setOnClickListener(new View.OnClickListener() {
								@Override
								public void onClick(View v) {
									Log.i("Castles", "phqr menu");
									idlecardentry_status = 10;
								}
							});

							btn_wallet.setOnClickListener(new View.OnClickListener() {
								@Override
								public void onClick(View v) {
									Log.i("Castles", "wallet menu");
									idlecardentry_status = 11;
								}
							});								

							while (idlecardentry_status == 0) {

								Thread.sleep(100); // a small delay to fix 2nd txn online PIN terminal hang issue. happen on S1F2. 2020/09/02

								if(EcrCommandReceiver.isEcrProcessing){
									Log.i(TAG, "CTOSS_MSRRead_inJava isEcrProcessing " + EcrCommandReceiver.isEcrProcessing);
									return;
								}

								//Log.i("saturn", "saturn check for card entry");
								//Log.d("AAA", "fBlockTrans: " + fBlockTrans);
								if (fBlockTrans != true) {
									szMSR_status = CTOSS_MSRRead_inJava();
								}
								//Log.i("saturn", "saturn CTOSS_MSRRead_inJava"+szMSR_status);
								if (szMSR_status.equals("MSR_OK")) {
									idlecardentry_status = 1;
									Log.i("saturn ", "saturn idle card swipe detected");
								}

								//szICC_status = CTOSS_SCStatus_inJava();
								if (szMSR_status.equals("ICC_OK")) {
									idlecardentry_status = 1;
									// Log.i("saturn ", "saturn idle card insert detected");
								}

								if (szMSR_status.equals("SETTLE")) {
									idlecardentry_status = 2;
									//Log.i("saturn ", "trigger AutoSettlement");
								}

								if (fSTART) {
									break;
								}
							}

							//Log.d("TINE", "szMSR_status: " + szMSR_status);
							//Log.d("TINE", "idlecardentry_status: " + idlecardentry_status);
							MCCAppEntry();

						} catch (Exception e) {

						}
					}
				}.start();
			}
		}

	}

	public String ClearScreen(String text) throws InterruptedException {

		String user_str="";
		pass_in_string = text;
			
		new Thread()
		{
			public void run()
			{
				try
				{
					Intent intent = new Intent();
					Bundle bundle = new Bundle();
					//intent.setClass(MainActivity.this, GetAmount.class);
					//intent.setClass(MainActivity.this, AmountEntryActivity.class);
					intent.setClass(MainActivity.this, ClearScreen.class);

					//bundle.putString("amt_string", amt_string);
					bundle.putString("pass_in_string", pass_in_string);
					//bundle.putString("display", "AMOUNT  (PHP)");
					bundle.putString("minlength", "1");
					bundle.putString("maxlength", "9");
					bundle.putString("type", "1");
					//bundle.putString("pass_in_string", pass_in_string);
					Log.i("pass_in_string", pass_in_string);

					intent.putExtras(bundle);
					startActivity(intent);
					//startActivityForResult(intent, 0);
				}
				catch(Exception e)
				{
					e.printStackTrace();
				}
			}
		}.start();


		/*
		Intent intent = new Intent();
		Bundle bundle = new Bundle();
		intent.setClass(MainActivity.this, GetAmount.class);

		bundle.putString("amt_string", amt_string);
		bundle.putString("amt_disp_msg", text);
		Log.i("amt_disp_msg", text);

		intent.putExtras(bundle);
		//startActivity(intent);
		startActivityForResult(intent, 0);

		Log.i("PATRICK", "startActivity");
		//finish();
		*/


		MainActivity.LOCK.setCondition(false);

		synchronized (LOCK) {
			while (!LOCK.conditionMet()) {
				try {
					LOCK.wait();
				} catch (InterruptedException e) {
					Log.e(TAG, "Exception when waiting for condition", e);
					return "Exception";
				}
			}
		}

		user_str = "Hello";
		//user_str = AmountEntryActivity.final_amt_string;


		Log.i("PATRICK", user_str);
		Log.i("PATRICK", "FINISH 456");
		//pin_num = pin_num + "XXX";
//		  Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
		return user_str;

	}

	public String TLE_MAC(String text) {
	    String user_str="hello";
        String szData = "";
		String szKey = "";

		System.out.println("TLE_MAC:"+text);
		
        String[] parsemsg = text.split("\\|");
		szData=parsemsg[0];
		szKey=parsemsg[1];
		
		System.out.println("szData:"+szData);
        System.out.println("szKey:"+szKey);

		Log.i("TLE_MAC", "1.encrypted_data:"+szData);
        Encryption mEncryption = new Encryption();
        user_str = mEncryption.TLE_MAC(szData, szKey);
        text="";
        System.out.println("TLE_MAC: user_str:"+user_str);
        
        return user_str;
	
	}
	
	public String TLE_Encrypt(String text) {
		String user_str="hello";
		String szData = "";
		String szKey = "";

		System.out.println("TLE_Encypt:"+text);
		
		String[] parsemsg = text.split("\\|");
		szData=parsemsg[0];
		szKey=parsemsg[1];
		
		System.out.println("szData:"+szData);
		System.out.println("szKey:"+szKey);
		
		//byte[] baEncryptedData = new byte[text.length()];
		Log.i("TLE_Encypt", "1.encrypted_data:"+szData);
		Encryption mEncryption = new Encryption();
		user_str = mEncryption.TLE_Encrypt(szData, szKey);
		text="";
		System.out.println("TLE_Encypt user_str:"+user_str);
		
		return user_str;
	}

	public String TLE_Decrypt(String text) {
		String user_str="hello";
		String szData = "";
		String szKey = "";
	
		System.out.println("TLE_Decrypt:"+text);
		
		String[] parsemsg = text.split("\\|");
		szData=parsemsg[0];
		szKey=parsemsg[1];
		
		System.out.println("szData:"+szData);
		System.out.println("szKey:"+szKey);
		
		//byte[] baEncryptedData = new byte[text.length()];
		Log.i("TLE_Encypt", "1.encrypted_data:"+szData);
		Encryption mEncryption = new Encryption();
		user_str = mEncryption.TLE_Decrypt(szData, szKey);
		text="";
		System.out.println("TLE_Decrypt user_str:"+user_str);
		
		return user_str;
	}

    final String CHANNEL_ID = "CHANNEL_ID";
    final String CHANNEL_NAME = "CHANNEL_NAME";


    private Bitmap createBitmapFromString(String inputNumber) {

        Paint paint = new Paint();
        paint.setAntiAlias(true);
        paint.setTextSize(80);
        paint.setTextAlign(Paint.Align.CENTER);

        Rect textBounds = new Rect();
        paint.getTextBounds(inputNumber, 0, inputNumber.length(), textBounds);

        Bitmap bitmap = Bitmap.createBitmap(textBounds.width() + 10, 90,
                Bitmap.Config.ARGB_8888);

        Canvas canvas = new Canvas(bitmap);
        canvas.drawText(inputNumber, textBounds.width() / 2 + 5, 70, paint);
        return bitmap;
    }

    private void createNotificationChannel() {

        {
            CharSequence name = "testing";
            String description = "i'm testing this notification";
            int importance = NotificationManager.IMPORTANCE_DEFAULT;
            NotificationChannel channel = new NotificationChannel(CHANNEL_ID, name, importance);
            channel.setDescription(description);
            // Register the channel with the system; you can't change the importance
            // or other notification behaviors after this
            NotificationManager notificationManager = getSystemService(NotificationManager.class);
            assert notificationManager != null;
            notificationManager.createNotificationChannel(channel);
        }
    }

    public void displayNotification(String text) {

        Notification.Builder builder = null;
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.O) {
            builder = new Notification.Builder(this, CHANNEL_ID);
        }

        //convert text to bitmap
        Bitmap bitmap = createBitmapFromString(text.trim());

        //setting bitmap to staus bar icon.
        builder.setSmallIcon(Icon.createWithBitmap(bitmap));

        builder.setContentTitle("Simple Notification");
        builder.setContentText("This is a simple notification");
        builder.setPriority(Notification.PRIORITY_MAX);

        NotificationManagerCompat notificationManagerCompat = NotificationManagerCompat.from(this);
        notificationManagerCompat.notify(01, builder.build());

        createNotificationChannel();
    }

	ConnectivityManager.NetworkCallback mGPRSCallback = null;
    public void switchGPRS() {
        Log.d(TAG, "switchGPRS");
		
        try {
	        final ConnectivityManager connection_manager = (ConnectivityManager) this.getSystemService(CONNECTIVITY_SERVICE);

	        NetworkRequest.Builder request = new NetworkRequest.Builder();
	        request.addCapability(NetworkCapabilities.NET_CAPABILITY_INTERNET);
	        request.addTransportType(NetworkCapabilities.TRANSPORT_CELLULAR);

	        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) 
			{
			    if(mGPRSCallback == null)
			    {
			            //Init only once
	        			Log.d(TAG, "mGPRSCallback");
			            mGPRSCallback = new ConnectivityManager.NetworkCallback() {
			                @Override
			                public void onAvailable(final Network network) {
			                    try {
			                        Log.d(TAG, "mGPRSCallback onAvailable");

			                        try {
			                            if (Build.VERSION.SDK_INT < Build.VERSION_CODES.M) {
			                                Log.d(TAG, "setProcessDefaultNetwork");
			                                ConnectivityManager.setProcessDefaultNetwork(network);
			                            } else {
			                                Log.d(TAG, "bindProcessToNetwork");
			                                connection_manager.bindProcessToNetwork(network);
			                            }
			                        } catch (IllegalStateException e) {
			                            Log.e(TAG, "ConnectivityManager fail: ", e);
			                        }
			                    } catch (NullPointerException npe) {
			                        npe.printStackTrace();
			                    }
			                }

	                        @Override
	                        public void onUnavailable() {
	                            super.onUnavailable();
	                            // do remove callback
	                            connection_manager.unregisterNetworkCallback(this);
	                        }
			            };
					}
					else
					{
						Log.d(TAG, "mGPRSCallback EXITS");
					}
			
			        connection_manager.requestNetwork(request.build(), mGPRSCallback);
			        Log.d(TAG, "mGPRSCallback requestNetwork");

	        }
        }catch(Exception e){
            e.printStackTrace();
        }
		
    }

    String[] Items = new String[100];
    String szPassword="";
    String szSSID="";
    public void SetWIFI()
    {

        //userTimerCancel();
        final PowerManager powerManager = (PowerManager) getSystemService(Context.POWER_SERVICE);
        PowerManager.WakeLock  wakeLock = powerManager.newWakeLock(PowerManager.FULL_WAKE_LOCK|PowerManager.ACQUIRE_CAUSES_WAKEUP | PowerManager.ON_AFTER_RELEASE, "appname::WakeLock");

        GetNetworkInfor getNetworkInfor = new GetNetworkInfor(MainActivity.this);

        Log.i("SetWIFI", "wakeLock.acaquire");
        wakeLock.acquire();
        //getNetworkInfor.SetWIFIEnabled(true);

        String szSSIDList=getNetworkInfor.getSSIDList();

        AlertDialog.Builder builder = new AlertDialog.Builder(MainActivity.this);
        builder.setTitle("Networks");
        builder.setCancelable(false);
        Log.i("SetWIFI", "szSSIDList: " + szSSIDList);

        Items = szSSIDList.split("\\|");
        int msgcnt = Items.length;

        Log.i("SetWIFI", "msgcnt: " + msgcnt);

        // add a list
        //String[] animals = {"horse", "cow", "camel", "sheep", "goat", "cat", "rat", "horse", "cow", "camel", "sheep", "goat", "cat", "rat"};
        builder.setItems(Items, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                szSSID=Items[which];
                Log.i("SetWIFI", "szSSID: " + szSSID);

                /**/
                AlertDialog.Builder builder = new AlertDialog.Builder(MainActivity.this);
                builder.setTitle("Enter WIFI Password");
                builder.setCancelable(false);
                // Set up the input
                final EditText input = new EditText(MainActivity.this);
                // Specify the type of input expected; this, for example, sets the input as a password, and will mask the text
                input.setInputType(InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_VARIATION_NORMAL);
                builder.setView(input);

                // Set up the buttons
                builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        szPassword = input.getText().toString();
                        Log.i("SetWIFI", "szPassword: " + szPassword);

                        getNetworkInfor.SelectSSID(szSSID, szPassword);
                        if(wakeLock.isHeld() == true) {
                            Log.i("SetWIFI", "inScanWIFI.wakeLock.isHeld() is true");
                            wakeLock.release();
                        }
                    }
                });
                builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        if(wakeLock.isHeld() == true) {
                            Log.i("SetWIFI", "inScanWIFI.wakeLock.isHeld() is true");
                            wakeLock.release();
                        }
                        dialog.cancel();
                    }
                });

                builder.show();
                /**/

            }
        });
        builder.setNegativeButton("Cancel",
                new DialogInterface.OnClickListener()
                {
                    public void onClick(DialogInterface dialog, int id)
                    {
                        if(wakeLock.isHeld() == true) {
                            Log.i("SetWIFI", "Cancel.wakeLock.isHeld() is true");
                            wakeLock.release();
                        }
                        dialog.cancel();
                    }
                });

        // create and show the alert dialog
        AlertDialog dialog = builder.create();
        dialog.show();
    }

    public void SetLocationEnabled(boolean status)
    {
        CtSettings ctSettings = new CtSettings();
        try{
            ctSettings.setLocation(status);
            //ctSettings.setNotificationVolume(7);
            //ctSettings.setMediaVolume(15);
            //ctSettings.openBatteryPercentage(true);
            //ctSettings.screenLock(false);
        } catch (Exception e) {
            Log.e(TAG, e.getMessage() );
        }
    }

    public String getSSIDList() {
        String ssid = "";
        WifiManager wifiManager = (WifiManager) getApplicationContext().getSystemService(Context.WIFI_SERVICE);
        wifiManager.startScan();

        List<ScanResult> scanResults = wifiManager.getScanResults();
        Log.i(TAG, "scanResults.size():"+scanResults.size());

        for (ScanResult result : scanResults) {
            ssid = result.SSID;
            Log.i(TAG, "getSSID: "+ssid);
            // Do something with the SSID
        }

        return ssid;
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native int inCTOSS_TerminalStartUp(String strSysYear);
	public native int inCTOSS_InitWaveData();
	public native int inCTOSS_Main();
	//public native int inCTOSS_Sale();
	public native int inCTOSS_Sale(boolean isECR);
    public native int inCTOSS_Void(boolean isECR);
    public native int inCTOSS_Settle();
	public native int inDisplayMsg();
	public native int inDisplayImage();
    public native int inCTOSS_REPORTS();
    public native int inCTOSS_PREAUTH();
    public native int inCTOSS_INSTALLMENT();
    public native int inCTOSS_SALE_OFFLINE();
    public native int inCTOSS_TIP_ADJUST();
    public native int inCTOSS_REPRINT();
    public native int inCTOSS_Settings();
    public native int inCTOSS_BUTTONCANCEL();
    public native int inCTOSS_CARDENTRYTIMEOUT();
    public native int inCTOSS_Balance();
	public native byte[] inCTOS_GetBuffer();
    public native int inCTOSS_SETUP();
    public native int inCTOSS_Batch_Total();
    public native int inCTOSS_Batch_Review();
	public native int inCTOSS_PRINT_EMVTAGS();
	public native int inCTOSS_Registration();
	public native String ReadTrxMenu();
	public native int inCTOSS_START();
	public native String GetTrxTypes();
	public native String GetAppHeaderDetails();
	public native int AppStart();
    public native String CTOSS_MSRRead_inJava();
    public native String CTOSS_SCStatus_inJava();
    public native int inCTOSS_Idle_Sale();
    public native int inCTOSS_OnlinePinTest();
    public native int inCTOSS_MANUALENTRY();
    public native int AppRegister();
	public native int intAutoSettlement();
    public native int inCTOSS_ProcessCfgExpress();
    public native int inDoAutoReboot();
	public native int inCTOSS_Get1stInitFlag();
	public native int inCTOSS_GetDLNotFinishedFlag();
    public native int inCTOSS_ECR_Menu(int inECRTxnType);
	public native int inCTOSS_Reports();
	public native int inCTOSS_Settlement();

	public native int inCTOSS_Credit();
	public native int inCTOSS_Bancnet();
	public native int inCTOSS_PHQR();
	public native int inCTOSS_Wallet();
	public native int inCTOSS_Register();
	
    public native int inCTOSS_GetEnvInt(String sztag);
	public native int inCTOSS_PutEnvInt(String sztag, int inValue);
	
	public native int inCTOSS_CreatefsdataFolder();
}

