/*******************************************************************************

*******************************************************************************/

#include <string.h>
#include <stdio.h>
#include <ctosapi.h>
#include <stdlib.h>
#include <stdarg.h>
#include <typedef.h>
#include <EMVAPLib.h>
#include <EMVLib.h>

#include "..\Includes\POSTypedef.h"
#include "..\Debug\Debug.h"

#include "..\Includes\POSMain.h"
#include "..\Includes\POSTrans.h"
#include "..\Includes\POSHost.h"
#include "..\Includes\POSReg.h"
#include "..\Includes\POSbatch.h"
#include "..\ui\Display.h"
#include "..\Includes\V5IsoFunc.h"
#include "..\Accum\Accum.h"
#include "..\print\Print.h"
#include "..\Comm\V5Comm.h"
#include "..\Includes\MultiApLib.h"
#include "..\Aptrans\MultiAptrans.h"
#include "..\Pinpad\Pinpad.h"
#include "..\Database\DatabaseFunc.h"
#include "../UIapi.h"
#include "../Includes/POSSetting.h"


int inCTOS_RegFlowProcess(void)
{
	int inRet = d_NO;

	USHORT ushEMVtagLen;
	BYTE   EMVtagVal[64];
	BYTE   szStr[64];
	char szOrigMenuFilename[FILENAME_SIZE+1], szSystemCmdPath[FILENAME_SIZE+1], szMenuFilename[FILENAME_SIZE+1];
    TRANS_DATA_TABLE* srTransPara;

    BYTE szTitle[25+1];

	vdCTOS_SetTransType(TERM_REG);

    if(get_env_int("MCCNH_TESTKEY") == 1)
	    vdSetDummyMKey(); //sidumili: Dummy Master Key (to be comment or remove, testing purpose only)

	memset(szTitle, 0x00, sizeof(szTitle));
	szGetTransTitle(srTransRec.byTransType, szTitle);

	//display title
	//vdDispTransTitle(TERM_REG);
	
	inRet = inCTOSS_CheckMemoryStatus();
	if(d_OK != inRet)
		return inRet;

	inRet = inCTOS_GetTxnPassword();
	if(d_OK != inRet)
		return inRet;
	
	inTCTRead(1);

	strCDT.HDTid=19;

	vdDebug_LogPrintf("saturn inCTOS_SelectHost");

	inRet = inCTOS_SelectHost();
	if(d_OK != inRet)
		return inRet;

	if (inMultiAP_CheckMainAPStatus() == d_OK)
	{
		inRet = inCTOS_MultiAPSaveData(d_IPC_CMD_LOGON);
		if(d_OK != inRet)
			return inRet;
	}
	else
	{
		if (inMultiAP_CheckSubAPStatus() == d_OK)
		{
			inRet = inCTOS_MultiAPGetData();
			if(d_OK != inRet)
				return inRet;
			
			inRet = inCTOS_MultiAPReloadTable();
			if(d_OK != inRet)
				return inRet;
		}
		inRet = inCTOS_MultiAPCheckAllowd();
		if(d_OK != inRet)
			return inRet;
	}

	vdDebug_LogPrintf("saturn befor pre-connect");
	inRet = inCTOS_PreConnect();
	if(d_OK != inRet)
		return inRet;

	/*
	inRet = inCTOS_PreConnectAndInit();
    if(d_OK != inRet)
        return inRet;
	*/

	inRet = inCTOS_CheckTranAllowd();
	if(d_OK != inRet)
		return inRet;

	inRet = inCTOS_CheckMustSettle();
	if(d_OK != inRet)
		return inRet;

	inRet = inCTOS_GetInvoice();
	if(d_OK != inRet)
		return inRet;

    if(strTCT.fRegUSD == 1) 
        srTransRec.fRegUSD=1; // set USD terminal serial number
    else
        srTransRec.fRegPHP=1; // set for PHP terminal serial number.
    
    //CTOS_LCDTClearDisplay();

    inRet=inProcessRegistration();
    if(d_OK != inRet)
        return inRet;

    if(strcmp(srTransRec.szRespCode, "00") == 0)
    {
       //CTOS_LCDTClearDisplay();  		
        if(strTCT.fRegUSD != 1)
        {
            srTransRec.fRegPHP = 1; // set for PHP terminal serial number.
            srTransRec.fRegUSD = 0; // reset USD terminal serial number	on Php registration.
            //setLCDPrint(7, DISPLAY_POSITION_CENTER, "PHP REGISTERED");  	

			vdDisplayMessageBox(1, 8, "", "PHP REGISTERED", "", MSG_TYPE_SUCCESS);
    		//usCTOSS_LCDDisplay(szDisMsg);
        }
        else
        {
            srTransRec.fRegUSD = 1;
            srTransRec.fRegPHP = 0; // reset USD terminal serial number	on Php registration.
            //setLCDPrint(7, DISPLAY_POSITION_CENTER, "USD REGISTERED");  	
            
            vdDisplayMessageBox(1, 8, "", "USD REGISTERED", "", MSG_TYPE_SUCCESS);
    		//usCTOSS_LCDDisplay(szDisMsg);
        }

		CTOS_Beep();
		WaitKey(2); 		   
        strTCT.fRegister=1;
		inTCTMenuSave(1);

	#if 0
        strTCT.inMenuid = strTCT.inMenuidNHProfileMenu;//original code default to 12;
        inTCTMenuSave(1);

		memset(szSystemCmdPath, 0x00, sizeof(szSystemCmdPath));
		sprintf(szSystemCmdPath, "rm %s", DB_APPSELECTMENU);
		system(szSystemCmdPath);

		memset(szOrigMenuFilename, 0, sizeof(szOrigMenuFilename));
	    sprintf(szOrigMenuFilename, "./fs_data/DYNAMICMENU%02d.S3DB", strTCT.inMenuid);
	    memset(szSystemCmdPath, 0x00, sizeof(szSystemCmdPath));
	    //sprintf(szMenuFilename, "./fs_data/DYNAMICMENU%02d.S3DB", 88);
	    sprintf(szSystemCmdPath, "cp %s %s", szOrigMenuFilename, DB_APPSELECTMENU);
	    system(szSystemCmdPath);
	#endif
    }
    else
        return ST_ERROR;

    if(get_env_int("MCCNH_TESTKEY") == 1)
	//vdSetDummyMKey(); //sidumili: Dummy Master Key (to be comment or remove, testing purpose only)
 
	return d_OK;
}



int inCTOS_RegFlowProcessUSD(void)
{
	int inRet = d_NO;

	USHORT ushEMVtagLen;
	BYTE   EMVtagVal[64];
	BYTE   szStr[64];
    	TRANS_DATA_TABLE* srTransPara;

	vdCTOS_SetTransType(TERM_REG);
	
	//display title
	vdDispTransTitle(TERM_REG);
	
	inRet = inCTOSS_CheckMemoryStatus();
	if(d_OK != inRet)
		return inRet;

	inRet = inCTOS_GetTxnPassword();
	if(d_OK != inRet)
		return inRet;
	
	inTCTRead(1);
	
	//strCDT.HDTid=strTCT.inLogonHostIndex;
	strCDT.HDTid=19;
	
	inRet = inCTOS_SelectHost();
	if(d_OK != inRet)
		return inRet;

	if (inMultiAP_CheckMainAPStatus() == d_OK)
	{
		inRet = inCTOS_MultiAPSaveData(d_IPC_CMD_LOGON);
		if(d_OK != inRet)
			return inRet;
	}
	else
	{
		if (inMultiAP_CheckSubAPStatus() == d_OK)
		{
			inRet = inCTOS_MultiAPGetData();
			if(d_OK != inRet)
				return inRet;
			
			inRet = inCTOS_MultiAPReloadTable();
			if(d_OK != inRet)
				return inRet;
		}
		inRet = inCTOS_MultiAPCheckAllowd();
		if(d_OK != inRet)
			return inRet;
	}

	inRet = inCTOS_CheckAndSelectMutipleMID();
	if(d_OK != inRet)
		return inRet;

	/*
	inRet = inCTOS_PreConnectAndInit();
    if(d_OK != inRet)
        return inRet;
	*/
	
	inRet = inCTOS_PreConnect();
	if(d_OK != inRet)
		return inRet;

	//inRet = inCTOS_CheckIssuerEnable();
	//if(d_OK != inRet)
		//return inRet;

	inRet = inCTOS_CheckTranAllowd();
	if(d_OK != inRet)
		return inRet;

	inRet = inCTOS_CheckMustSettle();
	if(d_OK != inRet)
		return inRet;

	inRet = inCTOS_GetInvoice();
	if(d_OK != inRet)
		return inRet;

 	srTransRec.fRegUSD = 1; // set USD terminal serial number

	inRet = inBuildAndSendIsoData();
	if(d_OK != inRet)
		return inRet;


	// Change menu setup
	// TERM_REG
	if(strcmp(srTransRec.szRespCode, "00") == 0)
	 {
	 	strTCT.fRegister=1;
		strTCT.inMenuid = 12;
		inTCTMenuSave(1);
		//srTransRec.fRegUSD = 0; // set USD terminal serial number

	       //CTOS_LCDTClearDisplay();
	    	setLCDPrint(7, DISPLAY_POSITION_CENTER, "USD REGISTERED");  			
	    	WaitKey(2);
		inTCTRead(1);
	 }

	
	return d_OK;
}


#if 0
int inCTOS_Reg(void)
{
    int inRet = d_NO;
    
    CTOS_LCDTClearDisplay();

	// Terminal send two consecutive logon during USD setup
	// 00114
	//Incorrect display using USD profile
	// 00139
    inTCTRead(1);

    if(strTCT.fRegUSD == 1)
   {
	srTransRec.fRegUSD = 1;
	//inRet = inCTOS_RegUSD();
	
	return inRet;
    }
    else
   {
	    	CTOS_LCDTClearDisplay();
		//display title
		vdDispTransTitle(TERM_REG);	   
		strCDT.HDTid=19;

		inRet = inCTOS_SelectHost();
		if(d_OK != inRet)
		return inRet;

		inRet = inCTOS_RegPerMerch();

	    if(d_OK == inRet)
	   {

		srTransRec.fRegPHP = 1; // set for PHP terminal serial number.
		srTransRec.fRegUSD = 0; // reset USD terminal serial number	on Php registration.

#if 1
		// Change menu setup
		// TERM_REG
		if(strcmp(srTransRec.szRespCode, "00") == 0)
		 {
		 	//srTransRec.fRegPHP = 1;
		 	if(strTCT.fRegUSD != 1)
			{
				strTCT.fRegister=1;
				strTCT.inMenuid = 12;
				inTCTMenuSave(1);
		 	}

		        CTOS_LCDTClearDisplay();
		    	setLCDPrint(7, DISPLAY_POSITION_CENTER, "PHP REGISTERED");  			
		    	WaitKey(2);
			//inTCTRead(1);
		 }
		else
			return ST_ERROR;		
#endif

	   		inTCTRead(1);
		     // if USD registration flag is ON -> TCT table.
		     if(strTCT.fRegUSD == 1)
		     {
			 	srTransRec.fRegUSD = 1;
			     	inRet = inCTOS_RegUSD();
		     }
	    }

    }
    return inRet;
}
//#else
int inCTOS_Reg(void)
{

   	 int inRet = d_NO;

	USHORT usTk1Len=TRACK_I_BYTES, usTk2Len=TRACK_II_BYTES, usTk3Len=TRACK_III_BYTES;
	BYTE szTk1Buf[TRACK_I_BYTES], szTk2Buf[TRACK_II_BYTES], szTk3Buf[TRACK_III_BYTES];
    
    	CTOS_LCDTClearDisplay();

	// Terminal send two consecutive logon during USD setup
	// 00114
	//Incorrect display using USD profile
	// 00139

//#00065 - Terminal able to process sale transaction on idle menu when MENU ID 1 (registration) is activated
	CTOS_MSRRead(szTk1Buf, &usTk1Len, szTk2Buf, &usTk2Len, szTk3Buf, &usTk3Len);	//clear buffer on idle swipe on term reg.				

        inTCTRead(1);	
	if(strTCT.fRegUSD == 1) 
 		srTransRec.fRegUSD = 1; // set USD terminal serial number
	else
		srTransRec.fRegPHP = 1; // set for PHP terminal serial number.

    CTOS_LCDTClearDisplay();

     //display title
     vdDispTransTitle(TERM_REG);	   
     strCDT.HDTid=19;

      inRet = inCTOS_SelectHost();
      if(d_OK != inRet)
	return inRet;

      inRet = inCTOS_RegPerMerch();

       if(d_OK == inRet)
      {
		// Change menu setup
		if(strcmp(srTransRec.szRespCode, "00") == 0)
		 {
		 
		 	if(strTCT.fRegUSD != 1)
			{
				srTransRec.fRegPHP = 1; // set for PHP terminal serial number.
				srTransRec.fRegUSD = 0; // reset USD terminal serial number	on Php registration.
			
				
		        	CTOS_LCDTClearDisplay();
		    		setLCDPrint(7, DISPLAY_POSITION_CENTER, "PHP REGISTERED");  			
		    		WaitKey(2);
				
		 	}
			else
			{
				srTransRec.fRegUSD = 1;
				srTransRec.fRegPHP = 0; // reset USD terminal serial number	on Php registration.

	        		CTOS_LCDTClearDisplay();
	    			setLCDPrint(7, DISPLAY_POSITION_CENTER, "USD REGISTERED");  			
	    			WaitKey(2);
				
			}
			
				strTCT.fRegister=1;
				strTCT.inMenuid = strTCT.inMenuidNHProfileMenu;//original code default to 12;
				//strTCT.inMenuidNewHost = strTCT.inMenuidNHProfileMenu; // send value of inMenuidNHProfileMenu for switch purposes.
				inTCTMenuSave(1);


		 }
		else
			return ST_ERROR;		
      
       }

    return inRet;

}
#endif


int inCTOS_RegUSD(void)
{
    int inRet = d_NO;
    
   //CTOS_LCDTClearDisplay();
    
    vdCTOS_TxnsBeginInit();

    inRet = inCTOS_RegFlowProcessUSD();

    inCTOS_inDisconnect();

    vdCTOS_TransEndReset();

    return inRet;
}

// register multi merchant
int inCTOS_RegPerMerch(void)
{
	int inRet = d_NO;
	int inNumOfMit = 0,inNum;
	char szErrMsg[30+1];

	memset(szErrMsg,0x00,sizeof(szErrMsg));
	sprintf(szErrMsg,"%s",strHDT.szHostLabel);
	CTOS_LCDTPrintXY(1, 8, szErrMsg);
	inMMTReadNumofRecords(strHDT.inHostIndex,&inNumOfMit);
	vdDebug_LogPrintf("inNumOfMit=[%d]-----",inNumOfMit);

    inRet = inCTOS_PreConnect();
    if(d_OK != inRet)
        return inRet;
        
	for(inNum =0 ;inNum < inNumOfMit; inNum++)
	{
		memcpy(&strMMT[0],&strMMT[inNum],sizeof(STRUCT_MMT));
		srTransRec.MITid = strMMT[0].MITid;
	    strcpy(srTransRec.szTID, strMMT[0].szTID);
	    strcpy(srTransRec.szMID, strMMT[0].szMID);
	    memcpy(srTransRec.szBatchNo, strMMT[0].szBatchNo, 4);
	    strcpy(srTransRec.szHostLabel, strHDT.szHostLabel);

        vdCTOS_SetTransType(TERM_REG);

	    inRet = inCTOS_RegPerMerch2();
		if (d_OK != inRet)
		{
			memset(szErrMsg,0x00,sizeof(szErrMsg));
		    if (inGetErrorMessage(szErrMsg) > 0)
		    {
		        vdDisplayErrorMsg(1, 8, szErrMsg);
		    }
			vdSetErrorMessage("");
		}
    }

	return ST_SUCCESS;
}




int inCTOS_RegPerMerch2(void)
{
    int inRet = d_NO;
    char szDisplayMsg[50];
    
    memset(szDisplayMsg,0x00,sizeof(szDisplayMsg));
    sprintf(szDisplayMsg,"%s",strMMT[0].szMerchantName);
    CTOS_LCDTPrintXY(1, 8, "                   ");
    CTOS_LCDTPrintXY(1, 8, szDisplayMsg);
    
    inRet = inCTOS_PreConnect();
    if(d_OK != inRet)
        return inRet;
    
    inRet = inBuildAndSendIsoData();
    if(d_OK != inRet)
        return inRet;
    
    inRet = inCTOS_inDisconnect();
    if(d_OK != inRet)
        return inRet;
    else
        vdSetErrorMessage("");
    
    return ST_SUCCESS;
}

int inCTOS_Reg(void)
{
    int inRet = d_NO;

	vdDebug_LogPrintf("saturn inCTOS_Reg");
    //CTOS_LCDTClearDisplay();
    
    vdCTOS_TxnsBeginInit();

	inRet = inCheckBattery();	
	if(d_OK != inRet)
		return inRet;

//	vdCTOSS_GetAmt();

    inRet = inCTOS_RegFlowProcess();

    inCTOS_inDisconnect();

    vdCTOS_TransEndReset();

    return inRet;
}

int inRegister(void)
{
	char szTrxnListMenu[512 + 1], szTransFunc[256 + 1];
	int key;
    int inRet;
    int inSaleType = 0;
	char trxlogo[20];

	
	//TINE:  android
	memset(szTrxnListMenu, 0x00, sizeof(szTrxnListMenu));
	memset(szTransFunc, 0x00, sizeof(szTransFunc));
	//if (strGBLVar.inGBLvAppType == 3)
	//	strcpy(szTrxnListMenu, "allcards4.bmp");
	//else
	//	strcpy(szTrxnListMenu, "allcards.bmp");

	sprintf(szTrxnListMenu,"supported%d.bmp", strGBLVar.inGBLvAppType);
	strcat(szTrxnListMenu, "|");
	
	strcat((char *) szTrxnListMenu, "LOG ON|inCTOS_Reg \n");
    strcat((char *) szTrxnListMenu, "SETTINGS|getSettingsMenu");
	
	key = MenuTransactions(szTrxnListMenu, szTransFunc);
	if (key == d_OK) {
		if(strcmp(szTransFunc, "getSettingsMenu") == 0)
        {
			srTransRec.byTransType = SETUP;
			inRet = inCTOS_GetTxnPassword();
			if (d_OK != inRet)
				return inRet;

			inSaleType = 0;
			memset(trxlogo, 0x00, sizeof(trxlogo));
			strcpy(trxlogo, "allcards.bmp");
			vdDebug_LogPrintf("trxlogo [%s], inSaleType [%d]", trxlogo, inSaleType);
			inRet = vdDisplayTrxn(inSaleType, trxlogo);
        }
		else
		{
		    inRet = inCTOSS_ExeFunction(szTransFunc);
			if (inRet != d_OK)
				return -1;
		}
	}

	return inRet;
}

