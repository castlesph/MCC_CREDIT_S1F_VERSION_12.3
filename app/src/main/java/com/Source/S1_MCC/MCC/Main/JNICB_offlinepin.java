package com.Source.S1_MCC.MCC.Main;

import android.util.Log;
import android.widget.Button;


import com.Source.S1_MCC.MCC.Kms.Convert;
import com.Source.S1_MCC.MCC.Kms.KMSUI;
import com.Source.S1_MCC.MCC.R;

import java.io.File;

import CTOS.CtEMVCusPINPadbyImg;

public class JNICB_offlinepin
{
    public JNICB_offlinepin() {
    }

    public interface  cb_EMV
	{
        long ShowVirtualPIN(byte[] pVirtualPinPadPara);
        long ShowPINDigit(byte digitsNum, final byte pinType, final byte remainingCounter);
        long GetPINDone();
    }
}


class JNICB_implements_EMV implements JNICB_offlinepin.cb_EMV
{
    MainActivity MainContext;
    CtEMVCusPINPadbyImg vpinpad = new CtEMVCusPINPadbyImg();
    private byte numOfOffPINDigits = 0;

    public JNICB_implements_EMV(MainActivity InContext)
    {
        MainContext = InContext;

        // (full screen resolution height: 1280 and width: 720)
        // top status bar height: 48 , bottom navigation bar height: 96
        // if both bar exist (not hide or layout_hide)
        // then the main layout will be from left top (0,48) to right bottom (720,1184) ,
        // sub layouts function or value below will set the margin value form (0,48) if top status bar exist,so margin(0,0) will be the(0,48) on screen
        // Show Pinpad by default :
        //f1f2f3
        // 0 1 2 back('R')
        // 3 4 5 enter('A')
        // 6 7 8 cancel('C')
        //   9

        vpinpad.VKBD_0.x = 50;
        vpinpad.VKBD_0.y = 545;
        vpinpad.VKBD_0.width = 140;
        vpinpad.VKBD_0.height = 140;
        vpinpad.VKBD_0.filepath = new File("/data/data/pub/0.bmp");
        vpinpad.VKBD_1.x = 210;
        vpinpad.VKBD_1.y = 545;
        vpinpad.VKBD_1.width = 140;
        vpinpad.VKBD_1.height = 140;
        vpinpad.VKBD_1.filepath = new File("/data/data/pub/1.bmp");
        vpinpad.VKBD_2.x = 370;
        vpinpad.VKBD_2.y = 545;
        vpinpad.VKBD_2.width = 140;
        vpinpad.VKBD_2.height = 140;
        vpinpad.VKBD_2.filepath = new File("/data/data/pub/2.bmp");
        vpinpad.VKBD_3.x = 50;
        vpinpad.VKBD_3.y = 695;
        vpinpad.VKBD_3.width = 140;
        vpinpad.VKBD_3.height = 140;
        vpinpad.VKBD_3.filepath = new File("/data/data/pub/3.bmp");
        vpinpad.VKBD_4.x = 210;
        vpinpad.VKBD_4.y = 695;
        vpinpad.VKBD_4.width = 140;
        vpinpad.VKBD_4.height = 140;
        vpinpad.VKBD_4.filepath = new File("/data/data/pub/4.bmp");
        vpinpad.VKBD_5.x = 370;
        vpinpad.VKBD_5.y = 695;
        vpinpad.VKBD_5.width = 140;
        vpinpad.VKBD_5.height = 140;
        vpinpad.VKBD_5.filepath = new File("/data/data/pub/5.bmp");
        vpinpad.VKBD_6.x = 50;
        vpinpad.VKBD_6.y = 845;
        vpinpad.VKBD_6.width = 140;
        vpinpad.VKBD_6.height = 140;
        vpinpad.VKBD_6.filepath = new File("/data/data/pub/6.bmp");
        vpinpad.VKBD_7.x = 210;
        vpinpad.VKBD_7.y = 845;
        vpinpad.VKBD_7.width = 140;
        vpinpad.VKBD_7.height = 140;
        vpinpad.VKBD_7.filepath = new File("/data/data/pub/7.bmp");
        vpinpad.VKBD_8.x = 370;
        vpinpad.VKBD_8.y = 845;
        vpinpad.VKBD_8.width = 140;
        vpinpad.VKBD_8.height = 140;
        vpinpad.VKBD_8.filepath = new File("/data/data/pub/8.bmp");
        vpinpad.VKBD_9.x = 210;
        vpinpad.VKBD_9.y = 995;
        vpinpad.VKBD_9.width = 140;
        vpinpad.VKBD_9.height = 140;
        vpinpad.VKBD_9.filepath = new File("/data/data/pub/9.bmp");
        vpinpad.VKBD_CLEAR.x = 530;
        vpinpad.VKBD_CLEAR.y = 695;
        vpinpad.VKBD_CLEAR.width = 140;
        vpinpad.VKBD_CLEAR.height = 140;
        vpinpad.VKBD_CLEAR.filepath = new File("/data/data/pub/backspace.bmp");
        vpinpad.VKBD_CANCEL.x = 530;
        vpinpad.VKBD_CANCEL.y = 845;
        vpinpad.VKBD_CANCEL.width = 140;
        vpinpad.VKBD_CANCEL.height = 140;
        vpinpad.VKBD_CANCEL.filepath = new File("/data/data/pub/cancel.bmp");
        vpinpad.VKBD_ENTER.x = 530;
        vpinpad.VKBD_ENTER.y = 545;
        vpinpad.VKBD_ENTER.width = 140;
        vpinpad.VKBD_ENTER.height = 140;
        vpinpad.VKBD_ENTER.filepath = new File("/data/data/pub/enter.bmp");
        vpinpad.VKBD_EX_FUNCTIONKEY1.x = 50;
        vpinpad.VKBD_EX_FUNCTIONKEY1.y = 445;
        vpinpad.VKBD_EX_FUNCTIONKEY1.width = 140;
        vpinpad.VKBD_EX_FUNCTIONKEY1.height = 90;
        vpinpad.VKBD_EX_FUNCTIONKEY1.value = 'S';// set 'S' ->f1 won't show on screen
        vpinpad.VKBD_EX_FUNCTIONKEY1.filepath = new File("/data/data/pub/f1.bmp");
        vpinpad.VKBD_EX_FUNCTIONKEY2.x = 210;
        vpinpad.VKBD_EX_FUNCTIONKEY2.y = 445;
        vpinpad.VKBD_EX_FUNCTIONKEY2.width = 140;
        vpinpad.VKBD_EX_FUNCTIONKEY2.height = 90;
        vpinpad.VKBD_EX_FUNCTIONKEY2.value = 'S';// set 'S' ->f2 won't show on screen
        vpinpad.VKBD_EX_FUNCTIONKEY2.filepath = new File("/data/data/pub/f2.bmp");
        vpinpad.VKBD_EX_FUNCTIONKEY3.x = 370;
        vpinpad.VKBD_EX_FUNCTIONKEY3.y = 445;
        vpinpad.VKBD_EX_FUNCTIONKEY3.width = 140;
        vpinpad.VKBD_EX_FUNCTIONKEY3.height = 90;
        vpinpad.VKBD_EX_FUNCTIONKEY3.value = 'S';// set 'S' ->f3 won't show on screen
        vpinpad.VKBD_EX_FUNCTIONKEY3.filepath = new File("/data/data/pub/f3.bmp");

        vpinpad.BtnRandom(true); // random pinpad (pin with random order) if true

        //vpinpad.InitAllVKBD_WidthHeight_and_Padding(130,130,20,360,765);//(width_of_img,heigh_of_img,padding between img,center X,center Y)
        //call InitAllVKBD_WidthHeight_and_Padding first when needed(after setting values of VKBD)
        //be ware of the size of the layout group,last two parameter are center(x,y) of the layout group,and (x,y)should not near edge

        /*
		  vpinpad.BtnRandom(true); // random pinpad (pin with random order) if true

        vpinpad.setMovingRange(true,0,150,720,1100,20,20);
		// set whole pinpad moving randomly in range of top left (x,y) to bottom right(x,y) with (x,y)distance unit

        int[] VKBD_position = { 1,2,3,
                                4,5,6,
                                7,8,9,
								   0,  'R', //back('R')
                                       'A', //enter('A')
									   'C'};//cancel('C')
        vpinpad.SetVKBD_Order(VKBD_position);
            //VKBD_position[x]
            //VKBD_position[0] = 1 ; -> put VKBD_1 in position 0
            // (position map below)
            //x= 0 1 2 10
            //   3 4 5 11
            //   6 7 8 12
            //     9

       	vpinpad.SetAllVKBD_WidthHeight(140,140); //Width Height of PIN
        vpinpad.SetAllVKBD_EX_WidthHeight(140,90); //Width Height of F1 F2 F3
        vpinpad.SetAllVKBD_and_EX_Offset(-5,50);  // move all PIN by +- its (x,y)
        vpinpad.SetPINDigits("*"); //set pin digit which show on screen

        //*/

		 /*
        vpinpad.SetScreenBackground("#FFFFFF");//main layout background

        vpinpad.SetTextViewBackground("#FFFFFF");//title text view background
		vpinpad.SetPinDigitText_Color_Size("#000000",20);//title text color and size

        vpinpad.SetPINDigitsBackground("#EEEEEE");//PIN Digits view background
        vpinpad.SetPinDigit_Color_Size("#000000",20);//PIN Digits color and size


        vpinpad.SetPinDigitTextView_width_height(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.WRAP_CONTENT);
        //set width and height parameter of title text view
        vpinpad.SetPinDigitTextView_leftMargin_topMargin(0,0);
        //if top status bar exist and not hide or layout_hide then set (0,0) will be (0,48) from screen most top left


        vpinpad.SetPinDigit_width_height(200,30);
        //set width and height parameter of Pin Digit view
        vpinpad.SetPinDigit_leftMargin_topMargin(160,60);
        //if top status bar exist and not hide or layout_hide then set (160,60) will be (160,108) from screen most top left
		// */


        //vpinpad.Set_hideSystembar(true); //don't use if it cause system bar change unexpected after removeAllView function
        //set true to hide system bar by using the flag that Set_SystemUiVisibility had set

		 /*
		vpinpad.Set_SystemUiVisibility( // hide system bar by value below
				// Set the content to appear under the system bars so that the
				// content doesn't resize when the system bars hide and show.
				View.SYSTEM_UI_FLAG_LAYOUT_STABLE
				| View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
				| View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
				// Hide the nav bar and status bar
				| View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
				| View.SYSTEM_UI_FLAG_FULLSCREEN
		);
		//*/
        //vpinpad.Set_Yoffset_to_kernel(48);//pass (position of img + Yoffset)  to kernel //this is a back up
        // if the actual press zone is higher(lower), set a positive(negative) number to make its y position lower(higher),

    }


    boolean isViewCreated = false;
    public long ShowVirtualPIN(byte[] pVirtualPinPadPara)
    {
        Log.i("OFF PIN", "ShowVirtualPIN: ");
        if(GlobalPara.isPINPadAlone == true)
        {
            Log.d("pVirtualPinPadPara IN", Convert.ByteArrayTohexString(pVirtualPinPadPara));
         

            vpinpad.showPINPAD(MainContext,pVirtualPinPadPara);

			Log.d("pVirtualPinPadPara : ", Convert.ByteArrayTohexString(pVirtualPinPadPara));
        }
       
        return 0;
    }


    public long ShowPINDigit(byte digitsNum, final byte pinType, final byte remainingCounter)
    {
       
        String title;
        Log.i("OFF PIN", "ShowPINDigit pinType: " + pinType + " remainingCounter" + remainingCounter + "  digitsNum:" + digitsNum);
        if (pinType == 0)
            title="Enter Online Pin :";
        else {
        	if (remainingCounter == 1)
				title = "ReEnter Offline Pin :";
        	else
			title = "Enter Offline Pin :";
		}
        numOfOffPINDigits = digitsNum;
        if(digitsNum > 6) {
            Log.i("OFF PIN", "digitsNum > 6: ");
        }
        vpinpad.showPINDigits(digitsNum,title);

        return 0;
    }

    public long GetPINDone()
    {
        Log.i("OFFLINE PIN", "GetPINDone numOfOffPINDigits: " + numOfOffPINDigits);
        if((numOfOffPINDigits > 6) || (numOfOffPINDigits < 4)){
            if(0 != numOfOffPINDigits)
            {
                Log.i("OFF PIN", "not in the range...");
            }

        }
        if(MainContext == null) {
            Log.e("OFF PIN", "MainContext null: ");
            return 0;
        }
        if(vpinpad == null) {
            Log.e("OFF PIN", "vpinpad null: ");
            return 0;
        }
		vpinpad.removeAllView(MainContext);//already in ui thread
		
		try
        {
          //  Thread.sleep(300);//if ui tread take too long
        }
        catch(Exception e)
        {
            e.printStackTrace();
        }
        Log.i("OFF PIN", "GetPINDone exit: ");
        return 0;
    }
}
