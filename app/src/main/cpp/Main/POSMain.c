/*******************************************************************************

*******************************************************************************/

#include <string.h>
#include <stdio.h>
#include <ctosapi.h>
#include <ctoserr.h>
#include <stdlib.h>
#include <stdarg.h>
#include <emv_cl.h>

#include "..\Includes\POSTypedef.h"
#include "..\Debug\Debug.h"

#include "..\FileModule\myFileFunc.h"
#include "..\DataBase\DataBaseFunc.h"

#include "..\Includes\showbmp.h"
#include "..\Includes\POSMain.h"
#include "..\Includes\POSTrans.h"
#include "..\Includes\POSHost.h"
#include "..\Includes\POSSale.h"
#include "..\Includes\POSConfig.h"
#include "..\ui\Display.h"
#include "..\Includes\Dmenu.h"
#include "..\Includes\POSVoid.h"
#include "..\powrfail\POSPOWRFAIL.h"
#include "..\Comm\V5Comm.h"
#include "..\Includes\CardUtil.h"

#include "..\Includes\MultiApLib.h"
#include "..\Aptrans\MultiAptrans.h"
#include "..\Includes\CfgExpress.h"
#include "..\Includes\Wub_lib.h"
#include "..\Aptrans\MultiShareECR.h"
#include "..\Aptrans\MultiShareCOM.h"
#include "..\TMS\TMS.h"

#include "..\Ctls\POSCtls.h"

//gcitra
#include "..\ctls\POSWave.h"
//gcitra

#include "..\Print\Print.h"
#include "..\Includes\PosDiag.h"
#include "../Debug/debug.h"
#include "../Includes/POSTrans.h"
#include "../Ctls/POSWave.h"
#include <stdbool.h>

extern void vdCTOSS_GetALLWaveSetting(void);

extern USHORT CTOS_VirtualFunctionKeyHit(BYTE *key);
extern USHORT CTOS_VirtualFunctionKeySet( CTOS_VIRTUAL_FUNCTION_KEY_SET_PARA* pPara, BYTE FuncKeyCount);

//issue:218 - mfl
BOOL fEntryCardfromIDLE;
//issue:218

static BYTE stgFirstIdleKey = 0x00;
static int gIdleDisplayICO = 0;

BYTE szPAN[PAN_SIZE+1];
BYTE szExpireDate[EXPIRY_DATE_BCD_SIZE+1];
BYTE szHoldNetworkName[128+1] = {0};
BYTE szHoldBmpSignal[50+1] = {0};
int inHoldGPRSSignal = 0;
int inTimes = 0;

extern BOOL fGPRSConnectOK;

extern USHORT usHWSupport;

int BATTERY_POSTION_X;
int BATTERY_POSTION_Y;

BYTE  bPercentage;
int inBattChargingCounter = 0;
BOOL fLowBattery = FALSE;
int inBatteryTimeOut = 0;
int inSignalTimeOut = 0;
static int inBatteryCounter = 0;
static int inSignalCounter = 0;

extern BOOL ErmTrans_Approved;
extern BOOL fPrinterStart;

static BYTE stgIdleEvent = 0;

BOOL fLowSignal = FALSE;

extern BOOL fERMTransaction; // Enhancement for printing speed
BOOL fISOLog = FALSE;
int inNotifyADEnable = 0;
int inIdleMenu = 0;

CTOS_VIRTUAL_FUNCTION_KEY_SET_PARA stKeyPara [1] = 
{
    //{0,400, 80, 480},  //F1
    //{80, 400, 160, 480}, //F2
    //{160, 400, 240, 480}, //F3
    //{240, 400, 320, 480} //F4
    
    {0, 400, 320, 480} //F4
};

BYTE chGetFirstIdleKey(void)
{
    return stgFirstIdleKey;
}

void vdSetFirstIdleKey(BYTE bFirstKey)
{
    stgFirstIdleKey = bFirstKey;
}

int inCTOS_ValidFirstIdleKey(void)
{
    if((chGetFirstIdleKey() >= d_KBD_1) && (chGetFirstIdleKey() <= d_KBD_9))
        return d_OK;
    else
        return d_NO;
}

int inCTOS_ECRTask(void)
{

    if (strTCT.fECR) // tct
    {
	    inMultiAP_ECRGetMainroutine(); 
    }

    return SUCCESS;
}

int inCTOS_IdleRunningTestFun4(void)
{    
    return SUCCESS;
}

int inCTOS_IdleRunningTestFun5(void)
{
    return SUCCESS;
}

int inCTOS_IdleRunningTestFun6(void)
{
    return SUCCESS;
}

int inCTOS_IdleRunningTestFun7(void)
{
    return SUCCESS;
}

int inCTOS_IdleRunningTestFun8(void)
{
    return SUCCESS;
}

// Clear extra image after bootup
int inCTOS_IdleVirtualFunctionKey(void)
{
	USHORT ret;

	vdDebug_LogPrintf("--inCTOS_IdleVirtualFunctionKey--");
	vdDebug_LogPrintf("inIdleMenu[%d]", inIdleMenu);
	
	if (inIdleMenu > 0)
	{
		if (strTCT.byTerminalType == 4)
		{
			BYTE Tkey = 0;	
			Tkey = (sizeof(stKeyPara) / sizeof(CTOS_VIRTUAL_FUNCTION_KEY_SET_PARA))%0x100;
			ret = CTOS_VirtualFunctionKeySet(stKeyPara, Tkey);
		}
	}

    return SUCCESS;
}

/*sidumili: added [patrick suggested]*/
int inCTOS_ConfigureCommDevice(void)
{
    BOOL BolDetachLANChange=TRUE;
    BYTE strOut[30],strtemp[17];

	if(BolDetachLANChange==TRUE)
	{
		inCPTRead(MCC_HOST);
		srTransRec.usTerminalCommunicationMode = strCPT.inCommunicationMode;
		vduiClearBelow(3);
		CTOS_LCDTPrintXY (1,7, "Please Wait 	"); 	  
		if(strCPT.inCommunicationMode == ETHERNET_MODE)
			CTOS_LCDTPrintXY(1, 8, "Init LAN... 	");
		else if(strCPT.inCommunicationMode == GPRS_MODE)
			CTOS_LCDTPrintXY(1, 8, "Init GPRS...	 ");
		else if(strCPT.inCommunicationMode == DIAL_UP_MODE)
			CTOS_LCDTPrintXY(1, 8, "Init Modem...	  ");

		if (inCTOS_InitComm(srTransRec.usTerminalCommunicationMode) != d_OK) 
		{
		   
			vdDisplayErrorMsg(1, 8, "COMM INIT ERR");
			return ST_ERROR;
		}
		
		srCommFuncPoint.inInitComm(&srTransRec, strCPT.inCommunicationMode);
		srCommFuncPoint.inGetCommConfig(&srTransRec);
		srCommFuncPoint.inSetCommConfig(&srTransRec);		
	
	}		   

	if(strCPT.inCommunicationMode == ETHERNET_MODE)
	{
	   if(strTCP.fDHCPEnable != IPCONFIG_STATIC)
	   {
			inEthernet_GetTerminalIP(strtemp);

			// set static IP			
			strTCP.fDHCPEnable = IPCONFIG_STATIC;
			strcpy(srEthernetRec.strLocalIP, strtemp);
			srCommFuncPoint.inSetCommConfig(&srTransRec); 

			// set DHCP
			strTCP.fDHCPEnable = IPCONFIG_DHCP;
			srCommFuncPoint.inSetCommConfig(&srTransRec); 

		   vduiClearBelow(3);
		   setLCDPrint(3, DISPLAY_POSITION_LEFT, "DHCP TERMINAL IP");
		   setLCDPrint(4, DISPLAY_POSITION_LEFT, strtemp);
	   }
	}

	return SUCCESS;
}


int inCTOS_ConfigureCTLSReader(void)
{
	inCTOSS_GetRAMMemorySize("FIRST IDLE");
	
    // patrick add code 20140903
    inTCTRead(1);

	if(inCTOSS_GetCtlsMode() == NO_CTLS)
	    return SUCCESS;
	
    if (strTCT.fFirstInit)
    {	
        //inCTOS_ConfigureCommDevice();
        
        if(NO_CTLS != inCTOSS_GetCtlsMode())
        {
            vdCTOS_InitWaveData();
            
            strTCT.fFirstInit = 0;
            inTCTSave(1);        
        }
       //CTOS_LCDTClearDisplay();
    }
    return SUCCESS;
}

int inCTOS_DisplayIdleBMP(void)
{    
	if (strTCT.fImageAnimated == TRUE)
	{
		if (strlen(strTCT.szIdleLogo1) > 0)
		{
			vdDisplayAnimateBmp(0, 30, strTCT.szIdleLogo1, strTCT.szIdleLogo2, strTCT.szIdleLogo3, NULL, NULL);
		}			
		else
		{
			if ((strTCT.byTerminalType % 2) == 0)
				vdDisplayAnimateBmp(0, 30, "Insert1.bmp", "Insert2.bmp", "Insert3.bmp", NULL, NULL);
			else
				vdDisplayAnimateBmp(0, 30, "Insert4.bmp", "Insert5.bmp", "Insert6.bmp", NULL, NULL);
		}
			
	}
	else
	{
		if (strlen(strTCT.szIdleLogo1) > 0)
		{
			displayAppbmpDataEx(0, 30, strTCT.szIdleLogo1, TRUE);
		}			
		else
		{
			if ((strTCT.byTerminalType % 2) == 0)
				displayAppbmpDataEx(0, 30, "Insert1.bmp", TRUE);
			else
				displayAppbmpDataEx(0, 30, "Insert4.bmp", TRUE);
		}
			
			
	}
			
    return SUCCESS;
}

int inCTOS_IdleDisplayDateAndTime(void)
{
	CTOS_RTC SetRTC;
	BYTE szCurrentTime[40 + 1];

	vdDebug_LogPrintf("--inCTOS_IdleDisplayDateAndTime--");
	memset(szCurrentTime, 0x00, sizeof(szCurrentTime));
	CTOS_RTCGet(&SetRTC);
	sprintf(szCurrentTime,"%d-%02d-%02d  %02d:%02d",SetRTC.bYear, SetRTC.bMonth, SetRTC.bDay, SetRTC.bHour,SetRTC.bMinute);
	vdDebug_LogPrintf("Len[%d].szCurrentTime[%s]", strlen(szCurrentTime), szCurrentTime);

	CTOS_LCDTSelectFontSize(d_FONT_16x16);

	if (isCheckTerminalMP200() == d_OK)
		CTOS_LCDTPrintXY (13,1, szCurrentTime);
	else
		CTOS_LCDTPrintXY (12,1, szCurrentTime);
	
	CTOS_LCDTSelectFontSize(d_FONT_16x30);
	
	return SUCCESS;
}


int inCTOS_DisplayComTypeICO(void)
{    

#define SMALL_ICON_LEN	24
#define SMALL_ICON_GAP	2
#define FIRST_SMALL_ICON	1
#define SMALL_ICON_LEN2		5

    USHORT usRet;
    USHORT usRet1;
    DWORD pdwStatus;
    //BYTE  bPercentage;
    static int inLoop = 0;
    static int inGPRSSignal = 0;
	static USHORT usNetworkType = 0;
	static USHORT usEthType = 1;
	static BYTE szNetworkName[128+1];
	static unsigned long linux_time = 0;
	unsigned long linux_crttime;

	static int gIdleDisplayIcon = 0;
	static BYTE szWIFISSID[50];

	BOOL fforceshtdwn = VS_FALSE;
	BOOL fdspchrg = VS_FALSE;
	char temp1[10], temp2[10];
	BOOL fPortable;
	BOOL fPCI;
	STRUCT_SHARLS_COM Sharls_COMData;
	static int inWifiSignal = 0;
	memset(&Sharls_COMData, 0x00, sizeof(STRUCT_SHARLS_COM));
	USHORT mkHWSupport; 	
	int inBatX = 60; // battery display x coordinate -- sidumili

	UCHAR chRet;
	char szBatteryDisplay[10];
	BOOL fBatteryCharge = FALSE;
	char szBuff[20] = {0};
	BOOL fGetBattery = FALSE;
	BOOL fGetSignal = FALSE;
	
	linux_crttime = CTOS_TickGet();

	// Set Battery Coordinates
	BATTERY_POSTION_X = 280;
	BATTERY_POSTION_Y = 15;

	vdDebug_LogPrintf("--inCTOS_DisplayComTypeICO--");

	vdDebug_LogPrintf("byTerminalType[%d] | byERMMode[%d] | byERMInit[%d]", strTCT.byTerminalType, strTCT.byERMMode, strTCT.byERMInit);
	vdDebug_LogPrintf("inCommunicationMode[%d]", strCPT.inCommunicationMode);
	
	// Display ERM bmp
	if (isCheckTerminalMP200() == d_OK)
	{
		if ((strTCT.byERMMode != 0) && (strTCT.byERMInit))		
			displayAppbmpDataEx(270, 200, "ERM.bmp", FALSE);		
	}
	
    if(strTCT.fDemo)
    {
        displayAppbmpDataEx(300, 90, "DEMO.bmp", FALSE);
    }

    if(strTCT.fECR)
    {
        //displayAppbmpDataEx(270, 180, "ECR1.bmp", FALSE);
        displayAppbmpDataEx(270, 350, "ECR1.bmp", FALSE);
    }

	//-------------------
	//sprintf(szBuff,"Icon: %d%s", chGetBatteryTimeOut(), "%");
	//CTOS_LCDTSelectFontSize(d_FONT_8x16);
	//CTOS_LCDTPrintXY (30, 4, szBuff);
	//CTOS_LCDTSelectFontSize(d_FONT_16x30);

	fGetBattery = FALSE;
	inBatteryCounter++;
	vdSetBatteryTimeOut(inBatteryCounter);
	if (isCheckTerminalMP200() == d_OK){

		if (chGetBatteryTimeOut() >= inBatteryTimeOut)
		{
			vdDebug_LogPrintf("Get New Battery Percentage!!!");			
			inBatteryCounter = 0;
		    vdSetBatteryTimeOut(inBatteryCounter);
			fGetBattery = TRUE;
		}
		else
		{
			vdDebug_LogPrintf("Do Not Get New Battery Percentage!!!");
		}		
			
		if (fGetBattery)
		{
			usRet1 = CTOS_PowerSource(&chRet);
			
			usRet1 = CTOS_BatteryGetCapacity(&bPercentage);
			
			vdDebug_LogPrintf("**CTOS_BatteryGetCapacity [%X] [%d]**", usRet1, bPercentage);
			
			if (usRet1 != d_BATTERY_NOT_EXIST) // Check if battery support
			{
				// Check for charging status
				if (chRet == d_PWRSRC_DCJACK)
				{
					fBatteryCharge = TRUE;
				}
				else
				{
					fBatteryCharge = FALSE;
					inBattChargingCounter = 1;
				}
					
				//displayAppbmpDataEx(BATTERY_POSTION_X, BATTERY_POSTION_Y, "blank.bmp", TRUE);

				vdBattChargingDisplay(fBatteryCharge, bPercentage);
			}
		}		
	}
	else
	{		
	    pdwStatus = 0x00;
	    usRet = CTOS_BatteryStatus(&pdwStatus);
	    //vdDebug_LogPrintf("**CTOS_BatteryStatus [%X] [%02X]**", usRet, pdwStatus);
	    usRet1 = CTOS_BatteryGetCapacity(&bPercentage);
	    //vdDebug_LogPrintf("**CTOS_BatteryGetCapacity [%X] [%d]**", usRet1, bPercentage);

		CTOS_HWSettingGet(&fPortable,&fPCI,&mkHWSupport);
		//sprintf(temp1,"%02d",fPortable);
		//sprintf(temp2,"%02x",mkHWSupport);
		//vdDebug_LogPrintf("AAA fPortable[%s] mkHWSupport[%s]",temp1,temp2); 

	    if(fPortable==VS_FALSE)
	    {
	        usRet1=d_BATTERY_NOT_EXIST;
	    }
		
	    if(d_BATTERY_NOT_EXIST == usRet1 || d_BATTERY_NOT_SUPPORT == usRet1)
	    {
	        displayAppbmpDataEx(BATTERY_POSTION_X, BATTERY_POSTION_Y, "blank.bmp", TRUE);
	    }
	    else if(d_OK == usRet1)
	    {	
	    	// Check for charging status
	    	if(d_OK == usRet && (pdwStatus & d_MK_BATTERY_CHARGE))
	    	{
				fBatteryCharge = TRUE;
	    	}
			else
			{
				fBatteryCharge = FALSE;
				inBattChargingCounter = 1;
			}
			
	        if(bPercentage <= 25)
	        {
				if (fBatteryCharge)
				{
					if(bPercentage <= 10)
	                    fdspchrg = VS_TRUE;
	                if(bPercentage <= 5)
	                    fforceshtdwn = VS_TRUE;	
				}
	        }

		    vdBattChargingDisplay(fBatteryCharge, bPercentage);
	    }
	}

    inLoop ++;
	if(strCPT.inCommunicationMode == ETHERNET_MODE)
	{	
	    displayAppbmpDataEx(1, 1, "LAN.bmp", FALSE);

		//aaronnino mcc v3 gprs fix on issue #0062 Blank Screen when battery is only 4% 3 of 6 start
		if (fdspchrg == VS_TRUE)
			{
				CTOS_LCDTSelectFontSize(d_FONT_16x16);
				CTOS_LCDTPrintXY (12,3, "Pls Charge");
				CTOS_LCDTSelectFontSize(d_FONT_16x30);
				if (fforceshtdwn == VS_TRUE)
					{
						CTOS_LCDTClearDisplay();
						CTOS_LCDTPrintXY (1,5,"POWERING OFF TERMINAL");
						CTOS_LCDTPrintXY (1,6,"PLEASE CHARGE");
						CTOS_Delay(5000);
						CTOS_PowerOff();
					}
			}
		//aaronnino mcc v3 gprs fix on issue #0062 Blank Screen when battery is only 4% 3 of 6 end	
	}
	else if(strCPT.inCommunicationMode == DIAL_UP_MODE)
	{
	    displayAppbmpDataEx(1, 1, "MODEM.bmp", FALSE);
		//aaronnino mcc v3 gprs fix on issue #0062 Blank Screen when battery is only 4% 4 of 6 start
		if (fdspchrg == VS_TRUE)
			{
				CTOS_LCDTSelectFontSize(d_FONT_16x16);
				CTOS_LCDTPrintXY (12,3, "Pls Charge");
				CTOS_LCDTSelectFontSize(d_FONT_16x30);
				if (fforceshtdwn == VS_TRUE)
					{
						CTOS_LCDTClearDisplay();
						CTOS_LCDTPrintXY (1,5,"POWERING OFF TERMINAL");
						CTOS_LCDTPrintXY (1,6,"PLEASE CHARGE");
						CTOS_Delay(5000);
						CTOS_PowerOff();
					}
			}
		//aaronnino mcc v3 gprs fix on issue #0062 Blank Screen when battery is only 4% 4 of 6 end		
	}		
	else if(strCPT.inCommunicationMode == GPRS_MODE)
	{
		CTOS_BackLightSetEx(d_BKLIT_KBD,d_ON,0xffffff);
		CTOS_BackLightSetEx(d_BKLIT_LCD,d_ON,0xffffff);

		fGetSignal = FALSE;
		inSignalCounter++;
		vdSetSignalTimeOut(inSignalCounter);

		if (chGetSignalTimeOut() >= inSignalTimeOut)
		{
			vdDebug_LogPrintf("Get New GPRS Signal!!!");
			
			inSignalCounter = 0;
		    vdSetSignalTimeOut(inSignalCounter);
			fGetSignal = TRUE;
		}
		else
		{
			vdDebug_LogPrintf("Do Not Get New GPRS Signal!!!");
		}
			
		//if (linux_crttime > linux_time + 500)
		if (fGetSignal)
		{	
				linux_time = CTOS_TickGet();	
				inGPRSSignal = inCTOSS_GetGPRSSignalEx(&usNetworkType, szNetworkName,&usEthType);
				vdDebug_LogPrintf("**inCTOSS_GetGPRSSignalEx inGPRSSignal=[%d]**usEthType=[%d]", inGPRSSignal,usEthType);
				vdDebug_LogPrintf("**usNetworkType [%2x][%2x][%d]**", d_MOBILE_NETWORK_UMTS , usNetworkType, (d_MOBILE_NETWORK_UMTS && usNetworkType));
		 
			if (inGPRSSignal > 0)
			{
				 strcpy(szHoldNetworkName, szNetworkName);
				 inHoldGPRSSignal = inGPRSSignal;
				 inTimes = 0;
			}
			else
			{
				 inTimes++;
				 if (inTimes == 3)
				 {
					 memset(szHoldNetworkName, 0x00,sizeof(szHoldNetworkName));
					 inHoldGPRSSignal = 0;
					 inTimes = 0;
				 }
			}
		}
		
		vdDebug_LogPrintf("DISPLAY Conditions %d %d %s", fGPRSConnectOK, inGPRSSignal, szHoldNetworkName);
		
		if(fGPRSConnectOK == TRUE)
		{ 
			CTOS_LCDTSelectFontSize(d_FONT_8x16);
			if (d_MOBILE_NETWORK_UMTS & usNetworkType)
				displayAppbmpDataEx(30, 15, "3G.bmp", FALSE);
			else if (d_MOBILE_NETWORK_GPRS & usNetworkType)
				displayAppbmpDataEx(30, 15, "G.bmp", FALSE);			
		}
		else
		{		
			CTOS_LCDTSelectFontSize(d_FONT_8x16);
			displayAppbmpDataEx(30, 15, "None.bmp", FALSE);
			CTOS_LCDTSelectFontSize(d_FONT_16x30);

		}

		vdDebug_LogPrintf("**inGPRSSignal =[%d]**szHoldNetworkName=[%s]**inHoldGPRSSignal[%d]**fGPRSConnectOK[%d]", inGPRSSignal,szHoldNetworkName,inHoldGPRSSignal,fGPRSConnectOK);
	
		if (inGPRSSignal > 0)
		{
			CTOS_LCDTSelectFontSize(d_FONT_8x16);
			CTOS_LCDTPrintXY (1, 3, szNetworkName);
			CTOS_LCDTSelectFontSize(d_FONT_16x30);
		}
		else
		{
			if (strlen(szHoldNetworkName) > 0)
			{
				vdDebug_LogPrintf("szHoldNetworkName has value %d",strlen(szHoldNetworkName));
				CTOS_LCDTSelectFontSize(d_FONT_8x16);
				if (d_MOBILE_NETWORK_UMTS & usNetworkType)
					displayAppbmpDataEx(30, 15, "3G.bmp", FALSE);
				else if (d_MOBILE_NETWORK_GPRS & usNetworkType)
					displayAppbmpDataEx(30, 15, "G.bmp", FALSE);
				CTOS_LCDTSelectFontSize(d_FONT_16x30);
					
				CTOS_LCDTSelectFontSize(d_FONT_8x16);
				CTOS_LCDTPrintXY (1, 3, szHoldNetworkName);
				CTOS_LCDTSelectFontSize(d_FONT_16x30);

			}
			else
			{
				vdDebug_LogPrintf("szHoldNetworkName has no value %d",strlen(szHoldNetworkName));
				CTOS_LCDTSelectFontSize(d_FONT_8x16);
				displayAppbmpDataEx(30, 15, "None.bmp", FALSE);				
				CTOS_LCDTSelectFontSize(d_FONT_16x30);
					
				CTOS_LCDTSelectFontSize(d_FONT_8x16);
				CTOS_LCDTPrintXY (1, 3, "No Service");
				CTOS_LCDTSelectFontSize(d_FONT_16x30);
			}
		}

		if (!fGPRSConnectOK) inHoldGPRSSignal = 0;
		
		if(1 == inHoldGPRSSignal)
			displayAppbmpDataEx(1, 1, "G1.bmp", FALSE);
		else if(2 == inHoldGPRSSignal)
			displayAppbmpDataEx(1, 1, "G2.bmp", FALSE);
		else if(3 == inHoldGPRSSignal)
			displayAppbmpDataEx(1, 1, "G3.bmp", FALSE);
		else if(4 == inHoldGPRSSignal)
			displayAppbmpDataEx(1, 1, "G4.bmp", FALSE);
		else
			displayAppbmpDataEx(1, 1, "G0.bmp", FALSE);
				
	}
    else if(strCPT.inCommunicationMode == WIFI_MODE)
    {
        CTOS_LCDTSelectFontSize(d_FONT_8x16);
        CTOS_LCDTPrintXY (10,2, "WIFI");
        CTOS_LCDTSelectFontSize(d_FONT_16x30);
        
        if (linux_crttime > linux_time + 500 || inLoop == 0)
        { 
            inLoop = 1;
            linux_time = CTOS_TickGet();
            inGPRSSignal = inCTOSS_GetGPRSSignalEx1(&usNetworkType, szNetworkName, &usEthType, &Sharls_COMData);
            inWifiSignal = Sharls_COMData.inReserved2;
            
            /*MCC: Hold the last GPRS Signal Data--sidumili/hubing*/
            if (inGPRSSignal > 0)
            {
                strcpy(szHoldNetworkName, Sharls_COMData.szWIFISSID);
                //vdDebug_LogPrintf("AAA-szHoldNetworkName[%s]", Sharls_COMData.szWIFISSID);
                inHoldGPRSSignal = inGPRSSignal;
                inTimes = 0;
                
                if ((usHWSupport & d_MK_HW_WIFI) == d_MK_HW_WIFI)
                {		
                    switch(inWifiSignal)
                    {
                        case 0:
                            strcpy(szHoldBmpSignal, "VFNone.bmp");
                        break;
    					
                        case 1:
                            strcpy(szHoldBmpSignal, "VF1.bmp");
                        break;
    					
                        case 2:
                            strcpy(szHoldBmpSignal, "VF2.bmp");
                        break;
    					
                        case 3:
                            strcpy(szHoldBmpSignal, "VF3.bmp");
                        break;
    					
                        case 4:
                            strcpy(szHoldBmpSignal, "VF4.bmp");
                        break;
    					
                        default:
                            strcpy(szHoldBmpSignal, "VF5.bmp");
                        break;
                    }
                }
                else
                {
                    if (0 == usHWSupport)
                        strcpy(szHoldBmpSignal, "VFInit.bmp");
                    else
                        strcpy(szHoldBmpSignal, "VFNone.bmp");
                }
            
            }
            else
            {
                inTimes++;
                if (inTimes == 3)
                {
                    memset(szHoldNetworkName, 0x00,sizeof(szHoldNetworkName));
                    inHoldGPRSSignal = 0;
                    inTimes = 0;
                }
                vdDebug_LogPrintf("**inTimes =[%d]**szHoldNetworkName=[%s]", inTimes,szHoldNetworkName);
            }
            /*MCC: Hold the last GPRS Signal Data--sidumili/hubing*/
        }

		vdDebug_LogPrintf("--WIFI--szHoldNetworkName[%s] | szHoldBmpSignal[%s]", szHoldNetworkName, szHoldBmpSignal);
		
        if (strlen(szHoldNetworkName) > 0)
        {
            CTOS_LCDTSelectFontSize(d_FONT_8x16);
            CTOS_LCDTPrintXY (1,3, szHoldNetworkName);
            CTOS_LCDTSelectFontSize(d_FONT_16x30);
            
            if (inWifiSignal > 0) 	
                displayAppbmpDataEx(1, 1, szHoldBmpSignal, FALSE);
        } 
        else
        {
            CTOS_LCDTSelectFontSize(d_FONT_8x16);
            CTOS_LCDTPrintXY (1,3, szHoldNetworkName);
            CTOS_LCDTSelectFontSize(d_FONT_16x30);
            
            if (0 == usHWSupport)
                strcpy(szHoldBmpSignal, "VFNone.bmp");
            else
                strcpy(szHoldBmpSignal, "VFNone.bmp");
            
            displayAppbmpDataEx(1, 1, szHoldBmpSignal, FALSE);
        }
        //vdDebug_LogPrintf("AAA - usHWSupport - %d inWifiSignal - %d", mkHWSupport, inWifiSignal);
    }
	
    return SUCCESS;
}

typedef struct 
{
	int  (*inCTOS_IdleRunningFun)(void);
} IDLE_FUNC;

IDLE_FUNC g_IdleRunningFun[] = 
{
    inCTOS_ConfigureCTLSReader,
    inCTOS_DisplayIdleBMP,
    inCTOS_DisplayComTypeICO,
    inCTOS_IdleDisplayDateAndTime,
    inCTOS_ECRTask,
    inCTOSS_JumpToSleepMode,
    inCTOS_IdleRunningTestFun4,
    inCTOS_IdleRunningTestFun5,
    inCTOS_IdleRunningTestFun6,
    inCTOS_IdleRunningTestFun7,
    inCTOS_IdleRunningTestFun8,
    inCTOS_AutoDiag_ParamCheck,
    inCTOS_ADC_Download,
    inCTOS_DisplayFSRMode,
    inCTOS_AutoSettlement,
    NULL       
};


int inCTOSS_InitAP(void)
{
	USHORT ret;
	BYTE szCurrDate[8] = {0};
	char buf[6+1];
	CTOS_RTC SetRTC;

         BYTE bInBuf[40];
         BYTE bOutBuf[40];
         USHORT usInLen = 0;
         USHORT usOutLen = 0;
	
	ret = CTOS_SetFunKeyPassword(strTCT.szFunKeyPW, 1);
	vdDebug_LogPrintf("CTOS_SetFunKeyPassword=[%s],ret=[%d]", strTCT.szFunKeyPW,ret);
	
	ret = CTOS_SetPMEnterPassword(strTCT.szPMpassword, 1);	
	vdDebug_LogPrintf("CTOS_SetPMEnterPassword=[%s],ret=[%d]", strTCT.szPMpassword,ret);

		/**********************
			CTOS_PrinterSetHeatLevel 
			default value is 2. 
			= 0 : Ultra light. 	= 1 : Very light. 
			= 2 : Light. 		= 3 : Medium. 
			= 4 : Dark. 		= 5 : Very dark. 
			= 6 : Ultra dark.
		***********************/
	ret = CTOS_PrinterSetHeatLevel(strTCT.inPrinterHeatLevel);	
	vdDebug_LogPrintf("CTOS_PrinterSetHeatLevel=[%d],ret=[%d]", strTCT.inPrinterHeatLevel,ret);
	if(get_env_int("CRCINIT") == 0)
	{
		vdPutEnvCRC(); // Get ALL CRC
		put_env_int("CRCINIT", 1);
	}
	
	inNotifyADEnable = get_env_int("NOTIFYADENABLE");

	inCTOSS_GetEnvDB("LASTAD", buf);

	if(strcmp(buf,"000000") == 0)		
	{
		memset(szCurrDate, 0x00, sizeof(szCurrDate));
		CTOS_RTCGet(&SetRTC);
		sprintf(szCurrDate,"%02d%02d%02d", SetRTC.bYear, SetRTC.bMonth, SetRTC.bDay);
		put_env_char("LASTAD",szCurrDate);
	}

	inMultiAP_RunIPCCmdTypesEx("com.Source.SHARLS_EMV.SHARLS_EMV", 0x99, bInBuf, usInLen, bOutBuf, &usOutLen);
         inMultiAP_RunIPCCmdTypesEx("com.Source.SHARLS_COM.SHARLS_COM", 0x99, bInBuf, usInLen, bOutBuf, &usOutLen);
         inMultiAP_RunIPCCmdTypesEx("com.Source.SHARLS_CTLS.SHARLS_CTLS", 0x99, bInBuf, usInLen, bOutBuf, &usOutLen);

	return SUCCESS;
}

int inCTOS_IdleEventProcess(void)
{
#define CTOS_ILDE_TIMER  60

    DWORD dwWait=0, dwWakeup=0;
    USHORT ret;
    USHORT byMSR_status;
    BYTE key;
    BYTE bySC_status;
    BYTE babuff[128];
    BYTE sT1Buf[128] ,sT2Buf[128] ,sT3Buf[128];
    USHORT bT1Len=128 ,bT2Len=128 ,bT3Len=128;
    int i = 0;
    int i1=0, i2=0;
    BYTE baTemp[128];
	char szErrorMessage[30];
    int inSaleType=0;
USHORT usTk1Len=TRACK_I_BYTES, usTk2Len=TRACK_II_BYTES, usTk3Len=TRACK_III_BYTES;
BYTE szTk1Buf[TRACK_I_BYTES], szTk2Buf[TRACK_II_BYTES], szTk3Buf[TRACK_III_BYTES];
  int inRetVal;

    CTOS_SCStatus(d_SC_USER, &bySC_status);
    if(bySC_status & d_MK_SC_PRESENT)
    {
        inSetTextMode();
        
        vdRemoveCard();
    }

   inTCTRead(1);	
   if(strTCT.inMenuid <= 11)
   {
      strTCT.fRegister=0;
      strTCT.inMenuid=11; /*need to do registration*/		
      inTCTMenuSave(1);
      inTCTRead(1);
   }
    
    CTOS_TimeOutSet(TIMER_ID_4, CTOS_ILDE_TIMER);  //Only IDLE loop use TIMER_ID_4, please don't use this TIMER_ID_4 in other place in application
    vduiKeyboardBackLight(VS_FALSE);

    vdSetIdleEvent(1);
	
    vdDebug_LogPrintf("**strTCT.fECR[%d]**", strTCT.fECR);
    if (strTCT.fECR) // tct
    {
        usCTOSS_ECRInitialize();	
    }

	inCTOSS_InitAP();

	// seet value for icon display
	memset(szErrorMessage,0x00,sizeof(szErrorMessage));
	sprintf(szErrorMessage,"%d",strCPT.inCommunicationMode);
	inCTOSS_PutEnv("PRICOMMMODE", szErrorMessage);
	inCTOSS_PutEnv("COMMBACK", "0");
	//memset(szErrorMessage,0x00,sizeof(szErrorMessage));
	//sprintf(szErrorMessage,"%d",strCPT.inSecCommunicationMode);
	//inCTOSS_PutEnv("SECCOMMMODE", szErrorMessage);

	inCTOS_IdleVirtualFunctionKey();
	
    while(1)
    {	
					
        fEntryCardfromIDLE=FALSE;
		fLowBattery = FALSE;
	    ErmTrans_Approved = FALSE;
		fPrinterStart = FALSE;
		fLowSignal = FALSE;
        
        if(CTOS_TimeOutCheck(TIMER_ID_4) == d_YES)
        {
            i = 0;
            while(g_IdleRunningFun[i].inCTOS_IdleRunningFun != NULL)
            g_IdleRunningFun[i++].inCTOS_IdleRunningFun();
        }

		// Check if low battery, unable to use terminal, call function after displayComTypeICO wherein battery percentage get.
		if (strTCT.inMinBattery > 0)
		{
			if (inCheckBatteryStatus() > 0)
			continue;
		}

		/*
		if (isCheckTerminalMP200() == d_OK)
		{
			inCheckSignalStatus();
			if (fLowSignal) continue;
		}
		*/
		
        dwWait = d_EVENT_KBD | d_EVENT_MSR | d_EVENT_SC;

		// Idle menu button
		if (strTCT.byTerminalType == 4)
		{
			do{
				ret = CTOS_VirtualFunctionKeyHit(&key);
				if (key == d_KBD_INVALID)
				{
					CTOS_KBDHit(&key);
					break;
				}

			 	CTOS_KBDBufPut(key);				
			}while(key != d_KBD_INVALID);

			if (key != d_KBD_INVALID) // patrick, please do not remove this part of code
			{
				CTOS_KBDBufPut(key);
			}			
		}
		
        //System waits the device the be set acts.   //System waits for target devices to be triggered and will not return until any target device is triggered //
        ret = CTOS_SystemWait(20, dwWait, &dwWakeup);

        vdSetFirstIdleKey(0x00);
        CTOS_SCStatus(d_SC_USER, &bySC_status);
        if ((dwWakeup & d_EVENT_KBD) == d_EVENT_KBD)
        {

            vduiKeyboardBackLight(VS_TRUE);

			vdCTOS_uiIDLEWakeUpSleepMode();
			
            //Detect if any key is pressed and get which key, but not get actual key.
            //If you get actual key, use to "CTOS_KBDGet" function//
            CTOS_KBDGet(&key);

			vdDebug_LogPrintf("idle key press key[%d]", key);
			
            switch(key)
            {
                case d_KBD_DOT:
                    CTOS_PrinterFline(24); // LineFeed
                break;
				
                case d_KBD_F1:                     
                    inF1KeyEvent();
                   //CTOS_LCDTClearDisplay();
                    break;

                case d_KBD_F2:                     
					inF2KeyEvent();
                   //CTOS_LCDTClearDisplay();
                    break;

                case d_KBD_F3:                     
                   //CTOS_LCDTClearDisplay();
                    break;

                case d_KBD_F4:                    
					inF4KeyEvent();
                   //CTOS_LCDTClearDisplay();
                    break;

                case d_KBD_ENTER:                    
					inEnterKeyEvent();
					CTOS_MSRRead(szTk1Buf, &usTk1Len, szTk2Buf, &usTk2Len, szTk3Buf, &usTk3Len);
					inSetTextMode();
					vdRemoveCard();
                   //CTOS_LCDTClearDisplay();
                    break;

                case d_KBD_CANCEL:
                    break;

                case d_KBD_0:
                    memset(szPAN, 0, sizeof(szPAN));//#00238 - Card No not cleared once cancel after expiry date
                    memset(szExpireDate, 0, sizeof(szExpireDate));
                    inF1KeyEvent();
					CTOS_MSRRead(szTk1Buf, &usTk1Len, szTk2Buf, &usTk2Len, szTk3Buf, &usTk3Len);
					inSetTextMode();
                    vdRemoveCard();
                   //CTOS_LCDTClearDisplay();				  
                break;

                case d_KBD_1:
                case d_KBD_2:
                case d_KBD_3:
                case d_KBD_4:
                case d_KBD_5:
                case d_KBD_6:
                case d_KBD_7:
                case d_KBD_8:
                case d_KBD_9: 
                    if(strTCT.fRegister == 0)
                    {
                    	vdDisplayErrorMsg(1, 8, "TERMINAL NOT REGISTER");
                        break;
                    }

                    vdSetFirstIdleKey(key);					
                    vduiKeyboardBackLight(VS_TRUE);
                    vduiLightOn();
					vdSetIdleEvent(1); //set stgIdleEvent to 1 to reset sleep timer.

					//fEntryCardfromIDLE = TRUE;
                    if(strTCT.byIdleSaleType == IDLE_CREDIT_SALE || strTCT.byIdleSaleType == IDLE_CREDIT_AND_BANCNET)
                    {
                        inCTOS_WAVE_SALE();
                    }
                    else if(strTCT.byIdleSaleType == IDLE_DEBIT_SALE)
                    {
                        inCTOS_DebitSale();
                    }
                    else if(strTCT.byIdleSaleType == IDLE_INSTALLMENT)
                    {
                        inCTOS_INSTALLMENT();
                    }
                    else if(strTCT.byIdleSaleType == IDLE_LOYALTY)
                    {
                        inCTOS_RedeemVariable();
                    }
                    else if(strTCT.byIdleSaleType == IDLE_BANCNET)
                    {
                        inCTOS_BancnetSale();
                    }
                    inSetTextMode();
					CTOS_LCDTClearDisplay();
				break;	
 
                case 80: // up botton
                case 87: // MP200
                    //CTOS_PrinterFline(d_LINE_DOT * 3); // space after printing trans title.
                    //vduiPowerOff(); //aaronnino mcc v3 gprs added enhancement for #0029 power off button start
                    vdCTOS_ShutdownRestart();
                break;
                
                default:
                break;
            }
			
			CTOS_LCDTClearDisplay();
			vdCTOSS_ClearTouchPanelTest(); // Idle menu button
            vduiKeyboardBackLight(VS_FALSE);
						
			//vdSetIdleEvent(1); // Move to vdCTOS_TransEndReset
            
        }
        else if ((dwWakeup & d_EVENT_MSR) == d_EVENT_MSR) // if enable swipe card on idle mode - activate script
        {
            if(strTCT.fRegister == 0 || strTCT.fIdleSwipeAllow == 0)
            {
                CTOS_MSRRead(szTk1Buf, &usTk1Len, szTk2Buf, &usTk2Len, szTk3Buf, &usTk3Len);	//clear buffer on idle swipe on term reg.
                vdDisplayErrorMsg(1, 8, "TERMINAL NOT REGISTER");
                continue;
            }
			
			//if(strTCT.fIdleSwipeAllow == 0)
			//{
				//continue;
			//}

            vduiKeyboardBackLight(VS_TRUE);
            vduiLightOn();
            vdCTOS_SetTransEntryMode(CARD_ENTRY_MSR);
			fEntryCardfromIDLE = TRUE;
            if(strTCT.byIdleSaleType == IDLE_CREDIT_SALE)
            {
                inCTOS_WAVE_SALE();
            }
            else if(strTCT.byIdleSaleType == IDLE_DEBIT_SALE)
            {
                inCTOS_DebitSale();
            }
            else if(strTCT.byIdleSaleType == IDLE_INSTALLMENT)
            {
                inCTOS_INSTALLMENT();
            }
            else if(strTCT.byIdleSaleType == IDLE_LOYALTY)
            {
                inCTOS_RedeemVariable();	
            }
			else if(strTCT.byIdleSaleType == IDLE_BANCNET)
			{
				inCTOS_BancnetSale();
			}
			else if (strTCT.byIdleSaleType == IDLE_CREDIT_AND_BANCNET)
			{
				inSelectIdleApps();
				vdRemoveCard();
				CTOS_MSRRead(szTk1Buf, &usTk1Len, szTk2Buf, &usTk2Len, szTk3Buf, &usTk3Len);
			}
			inSetTextMode();
			CTOS_LCDTClearDisplay();
			
			vdCTOSS_ClearTouchPanelTest(); // Idle menu button
			vduiKeyboardBackLight(VS_FALSE);
        }
        else if (((dwWakeup & d_EVENT_SC) == d_EVENT_SC) || (bySC_status & d_MK_SC_PRESENT)) // if enable insert card on idle mode - activate
        {
            if(strTCT.fRegister == 0)
            {
                CTOS_MSRRead(szTk1Buf, &usTk1Len, szTk2Buf, &usTk2Len, szTk3Buf, &usTk3Len);		//clear buffer on idle swipe on term reg.
                vdDisplayErrorMsg(1, 8, "TERMINAL NOT REGISTER");
                continue;
            }

            if(strTCT.fIdleInsertAllow== 0)
                continue;

            fEntryCardfromIDLE = TRUE;
            vduiKeyboardBackLight(VS_TRUE);
            vduiLightOn();
            vdCTOS_SetTransEntryMode(CARD_ENTRY_ICC);	
            if(strTCT.byIdleSaleType == IDLE_CREDIT_SALE)
            {
                inCTOS_WAVE_SALE();
            }
            else if(strTCT.byIdleSaleType == IDLE_DEBIT_SALE)
            {
                inCTOS_DebitSale();
            }
            else if(strTCT.byIdleSaleType == IDLE_INSTALLMENT)
            {
                inCTOS_INSTALLMENT();
            }
            else if(strTCT.byIdleSaleType == IDLE_LOYALTY)
            {
                inCTOS_RedeemVariable();	
            }
			else if(strTCT.byIdleSaleType == IDLE_BANCNET)
			{
				inCTOS_BancnetSale();
			}			
			else if (strTCT.byIdleSaleType == IDLE_CREDIT_AND_BANCNET)
			{				
				inSelectIdleApps();
				vdRemoveCard();
			}			
			inSetTextMode();
			CTOS_LCDTClearDisplay();

			vdCTOSS_ClearTouchPanelTest(); // Idle menu button
			vduiKeyboardBackLight(VS_FALSE);
			
        }

    }


	
}





int main(int argc,char *argv[])
{
	USHORT inSoundKBD = get_env_int("SOUNDKBD");
	USHORT inSoundFrequency = get_env_int("SOUNDFREQ"); 
	USHORT inSoundDuration = get_env_int("SOUNDDUR"); 

	char szvalue[1+1];
	
	inSetTextMode();
	
	CTOS_BackLightSetBrightness(d_BKLIT_LCD, get_env_int("BRIGHTNESS"));

	// Handle enable / disable of keyboard sound. And frequency setting -- sidumili
	if (inSoundKBD > 0)
	{
		CTOS_KBDSetSound(1);
		//android-reoved
		//CTOS_KBDSetFrequence(inSoundFrequency, inSoundDuration);
	}
	else
	{
		CTOS_KBDSetSound(0);
	}
	
	inCTOSS_ProcessCfgExpress();
	
    inDatabase_TerminalOpenDatabase();
    inTCTReadEx(1);
    inCPTReadEx(MCC_HOST);
    inPCTReadEx(1);
    inCSTReadEx(1);
    inTCPReadEx(1);
    inDatabase_TerminalCloseDatabase();
	
    vdThreadRunAppColdInit();

	if (get_env_int("AUTORECOVER") != 8)
	{
		inMultiAP_ForkTask();
		if(d_OK == inMultiAP_CheckSubAPStatus())
		{
			vdDebug_LogPrintf("inWaitTime =[%d]",strTCT.inWaitTime);
			if (strTCT.inWaitTime <= 0)
				strTCT.inWaitTime = 100;
			
			while (1)
			{
				inMultiAP_GetMainroutine();			
				//CTOS_Delay(strTCT.inWaitTime);
				
				msgEnd();
				return 0;
			}
		}
		else
		{
			inMultiAP_ForkSharlsAp();
		}
	}
	else
	{
		inMultiAP_ForkTaskEx();
	}
	
   //CTOS_LCDTClearDisplay();
	inCTOS_DisplayIdleBMP();
	vdDisplayLoadingApps();	
	CTOS_Delay(1000);

	inAutoUpdateComms(); // Auto Update Comms
	CTOS_Delay(1000);
	
	if (get_env_int("AUTORECOVER") != 8)
		inCTLOS_Getpowrfail();
	
	put_env_int("AUTORECOVER", 0);
	
	if (get_env_int("MAINRESET") != 1){
		put_env_int("CRDTCTR",0);
		put_env_int("INSTCTR",0);
		put_env_int("DEBTCTR",0);
		put_env_int("DINCTR",0);
		put_env_int("DININSTCTR",0);
	}
	
	put_env_int("MAINRESET",0);

	vdInitUSCDCBMODE(); //FOR USB CDC MODE [ECR] -- sidumili

	//android-removed
	//vdReFork_SHARLS_ERM(); // Make sure sharls_erm is will run in background -- sidumili		
    //end
    
	//android-removed
	//inCTOS_InitWIFI(); // Transfer here init wifi to init/connect successfully - sidumili
	//end

	// Idle Menu 
	inIdleMenu = get_env_int("IDLEMENU");	
	
	vdCTOSS_ClearTouchPanelTest();

	inCTOSS_GetCtlsMode();

	vdSetCtlsEntryImage();

	// For battery
	inBatteryTimeOut = get_env_int("IDLETIMEOUT");
	if (inBatteryTimeOut < 0) inBatteryTimeOut = 5;
	vdSetBatteryTimeOut(inBatteryTimeOut);

	// For signal
	inSignalTimeOut = get_env_int("IDLETIMEOUT");
	if (inSignalTimeOut < 0) inSignalTimeOut = 5;
	vdSetSignalTimeOut(inSignalTimeOut);

	vdSetGSMAuthType(); // Set GSM Authentication tyep to 0, by default if no value set

	vdSetGSMNetworkType(); // Set GSM Network Type
	
	vdCheckERMMode(); // Enhancement for printing speed -- sidumili	

	vdCheckISOLog(); // Check ofor ISO Logging
	
	CTOS_LCDTClearDisplay();

	vdSetADiagParams();

	vdSetADLParams();
	
	if ( inCTOSS_GetEnvDB ("TEST_INSTALLMENT", szvalue) != d_OK )
	                   inCTOSS_PutEnvDB("TEST_INSTALLMENT", "0");
	if ( inCTOSS_GetEnvDB ("TEST_BANCNET", szvalue) != d_OK )
	                   inCTOSS_PutEnvDB("TEST_BANCNET", "0");
                      
    inCTOS_IdleEventProcess();

}

// Clear extra image after bootup
void vdCTOSS_ClearTouchPanelTest(void)
{
	BYTE str[256],size = 5;
	ULONG rb;
	BYTE count = 0;

	int pen_up = 0;
	int pen_down = 0;
	int bXpen_up = 0, bXpen_down = 0;
	int bXCoordinate = 0, bYCoordinate = 0;
	int temp_iX = 0,temp_iY = 0;
	USHORT ret;
    BYTE key;

	vdDebug_LogPrintf("--vdCTOSS_ClearTouchPanelTest--");
	vdDebug_LogPrintf("inIdleMenu[%d]", inIdleMenu);
	
	if (inIdleMenu > 0)
	{
		if (strTCT.byTerminalType == 4)
		{
			do{
				ret = CTOS_VirtualFunctionKeyHit(&key);
				vdDebug_LogPrintf("CTOS_VirtualFunctionKeyHit.key=[%d][%c]",key,key);
				if (key == d_KBD_INVALID)
				{
					CTOS_KBDHit(&key);
					break;
				}

			}while(key != d_KBD_INVALID);
		}
	}
		
}

void vdBattChargingDisplay(BOOL fCharging, int inCurrentPercentage)
{
	int inCount = 0;
	int inPercentage = 0;
	BYTE szBatteryPercentage[128 + 1] = {0};
	int inCtr = 0;

	vdDebug_LogPrintf("--vdBattChargingDisplay--");
	vdDebug_LogPrintf("inCurrentPercentage[%d] -- fCharging[%d] -- inBattChargingCounter[%d]", inCurrentPercentage, fCharging, inBattChargingCounter);

	if (fCharging)
	{
		if (inCurrentPercentage >= 100)
		{
			displayAppbmpDataEx(BATTERY_POSTION_X, BATTERY_POSTION_Y, "100.bmp", TRUE);
		}
		else
		{
			switch (inBattChargingCounter)
			{
				case 1:
					displayAppbmpDataEx(BATTERY_POSTION_X, BATTERY_POSTION_Y, "25N.bmp", TRUE);
				break;
				case 2:
					displayAppbmpDataEx(BATTERY_POSTION_X, BATTERY_POSTION_Y, "50N.bmp", TRUE);
				break;
				case 3:
					displayAppbmpDataEx(BATTERY_POSTION_X, BATTERY_POSTION_Y, "75N.bmp", TRUE);
				break;
				case 4:
					displayAppbmpDataEx(BATTERY_POSTION_X, BATTERY_POSTION_Y, "100N.bmp", TRUE);
				break;
				case 5:
					displayAppbmpDataEx(BATTERY_POSTION_X, BATTERY_POSTION_Y, "10N.bmp", TRUE);
				break;
				default:
					inBattChargingCounter = 0;
				break;	
			}

			inBattChargingCounter++;
		}
	}
	else
	{
		if (inCurrentPercentage <= 25)
			inPercentage = 25;
		else if (inCurrentPercentage <= 50)
			inPercentage = 50;
		else if (inCurrentPercentage <= 75)
			inPercentage = 75;
		else if (inCurrentPercentage <= 100)
			inPercentage = 100;
		else
			inPercentage = 0;
	
		switch (inPercentage)
		{
			case 0:
				displayAppbmpDataEx(BATTERY_POSTION_X, BATTERY_POSTION_Y, "0.bmp", TRUE);
			break;
			case 25:
				displayAppbmpDataEx(BATTERY_POSTION_X, BATTERY_POSTION_Y, "25.bmp", TRUE);
			break;
			case 50:
				displayAppbmpDataEx(BATTERY_POSTION_X, BATTERY_POSTION_Y, "50.bmp", TRUE);
			break;
			case 75:
				displayAppbmpDataEx(BATTERY_POSTION_X, BATTERY_POSTION_Y, "75.bmp", TRUE);
			break;
			case 100:
				displayAppbmpDataEx(BATTERY_POSTION_X, BATTERY_POSTION_Y, "100.bmp", TRUE);
			break;		
		}
	}

	// Display battery percentage
	if (inCurrentPercentage > 0)
	{
		memset(szBatteryPercentage, 0x00, sizeof(szBatteryPercentage));
		sprintf(szBatteryPercentage, "%d%s", inCurrentPercentage, "%");
		CTOS_LCDTSelectFontSize(d_FONT_8x16);
		CTOS_LCDTPrintXY (72, 3, szBatteryPercentage);
		CTOS_LCDTSelectFontSize(d_FONT_16x30);
	}

	if (!fCharging)
	{
		if ((inCurrentPercentage < strTCT.inMinBattery) && (inCurrentPercentage > 0))
			fLowBattery = TRUE;
		else
			fLowBattery = FALSE;	
	}
	else
	{
		fLowBattery = FALSE;
	}
		

}

int inCheckBatteryStatus(void)
{
	int inResult = d_NO;

	vdDebug_LogPrintf("--inCheckBatteryStatus--");
	vdDebug_LogPrintf("fLowBattery[%d]", fLowBattery);

	if (fLowBattery)
	{
		CTOS_KBDBufFlush();
		vdDisplayAnimateBmp(245, 150, "Low_battery.bmp", NULL, NULL, NULL, NULL);
		CTOS_Beep();
		CTOS_Delay(2000);
		return(1);
	}
	
	return (0);
}

int inCTOSS_JumpToSleepMode(void)
{  
	char szMessage[30+1];
	static int inSleepTime = 2;
	static int Sleepflag = 0;
	static int offonflag = 0;
	static unsigned long linux_time = 0;
	unsigned long linux_crttime;


	if(get_env_int("POWERSAVE") != 1)
		return SUCCESS;

	linux_crttime = CTOS_TickGet();

	vdDebug_LogPrintf("inCTOSS_JumpToSleepMode");
    	
	if (chGetIdleEvent() == 1)
	{
		inSleepTime = get_env_int("SLEEPTIME");
		if (inSleepTime < 0)
			inSleepTime = 90;

		if (inSleepTime == 0)
			Sleepflag = 1;
				
		vdDebug_LogPrintf("inCTOSS_JumpToSleepMode,inSleepTime=[%d],Sleepflag=[%d]", inSleepTime,Sleepflag);
		vdSetIdleEvent(0);
		
		linux_time = CTOS_TickGet();
	}

    vdDebug_LogPrintf("TEST linux_crttime = %d %d",linux_crttime, linux_time + (inSleepTime*100));
	if((linux_crttime > linux_time + (inSleepTime*100)) && Sleepflag != 1)
	{
		vdCTOS_uiIDLESleepMode();
	}

	if (inCTOSS_CheckBatteryChargeStatus() == d_OK)
	{
	    vdDebug_LogPrintf("TEST CTOS_TickGet"); 
		linux_time = CTOS_TickGet();
	}

    return SUCCESS;
}

BYTE chGetIdleEvent(void)
{
    vdDebug_LogPrintf("chGetIdleEvent %d %c",stgIdleEvent, stgIdleEvent);
    return stgIdleEvent;
}

void vdSetIdleEvent(BYTE bIdleEvent)
{
    stgIdleEvent = bIdleEvent;	
}

//android-removed
#if 0
int inCTOS_InitWIFI(void)
{
	int inRet = d_OK;

	if (strCPT.inCommunicationMode != WIFI_MODE) 
	return;
		
	vdDebug_LogPrintf("--inCTOS_InitWIFI--[START]");
	
	inCPTRead(MCC_HOST);
	inTCPRead(1);

	vdDebug_LogPrintf("szWifiSSID[%s] | szWifiPassword[%s]", strTCP.szWifiSSID, strTCP.szWifiPassword);
	
	srTransRec.usTerminalCommunicationMode = strCPT.inCommunicationMode;
	vduiClearBelow(3);

	vdDebug_LogPrintf("AAA - WIFI INIT ON IDLE");

	if (strCPT.inCommunicationMode == WIFI_MODE)
	{
		if ((strcmp(strTCP.szWifiSSID, "X") != 0) && (strlen(strTCP.szWifiSSID) > 0))
		{
			CTOS_LCDTPrintXY(1, 8, "Init WIFI...	  ");

			inFirstInitConnectFuncPoint();
			
			inRet = inCTOS_InitComm(srTransRec.usTerminalCommunicationMode);
			vdDebug_LogPrintf("inCTOS_InitComm..inRet[%d]", inRet);
			
			if (inRet != d_OK) 
			{
				vdCTOS_DispStatusMessage("WIFI INIT FAILED");
				return d_NO;
			}
			
			inRet = srCommFuncPoint.inInitComm(&srTransRec, strCPT.inCommunicationMode);				
			inRet = srCommFuncPoint.inGetCommConfig(&srTransRec);
			inRet = srCommFuncPoint.inSetCommConfig(&srTransRec);			
		}
		
	}	

	vdDebug_LogPrintf("--inCTOS_InitWIFI--[END]");
	
	return inRet;
}
#endif

BYTE chGetBatteryTimeOut(void)
{
    vdDebug_LogPrintf("chGetBatteryTimeOut %d",inBatteryCounter);
	
    return inBatteryCounter;
}

void vdSetBatteryTimeOut(BYTE bTimeOut)
{
    inBatteryCounter = bTimeOut;
}

BYTE chGetSignalTimeOut(void)
{
    vdDebug_LogPrintf("chGetSignalTimeOut %d",inSignalCounter);
	
    return inSignalCounter;
}

void vdSetSignalTimeOut(BYTE bTimeOut)
{
    inSignalCounter = bTimeOut;
}

int inCheckSignalStatus(void)
{
	int inResult = d_NO;
	int inSignal = 0;

	// For GPRS only
	if (strCPT.inCommunicationMode != GPRS_MODE) return (0);

	inSignal = inHoldGPRSSignal;
	
	vdDebug_LogPrintf("--inCheckSignalStatus--");
	vdDebug_LogPrintf("inSignal[%d]", inSignal);

	if (inSignal < 1)
	{		
		vdDebug_LogPrintf("initfail.bmp");
		vdDisplayAnimateBmp(245, 150, "initfail.bmp", NULL, NULL, NULL, NULL);

		CTOS_KBDBufFlush();
		CTOS_Beep();
		fLowSignal = TRUE;
		
	}
	else
	{
		fLowSignal = FALSE;
	}
		
	return (d_OK);
}

int inAutoUpdateComms(void)
{
	int inResult;
	int inCommMode = 1;
	USHORT ret = 0;
	int inIPHeader = 1;

	vdDebug_LogPrintf("--inAutoUpdateComms--");
	vdDebug_LogPrintf("inCommunicationMode[%d]", strCPT.inCommunicationMode);
	
	vduiClearBelow(2);

	if (get_env_int("AUTORECOVER") != 8)
	{		
		CTOS_LCDTPrintXY(1, 7, "Configuring Comms");
		CTOS_LCDTPrintXY(1, 8, "Please wait...");
	}
	else
	{
		CTOS_LCDTPrintXY(1, 8, "Please wait...");	
	}
	
	switch(strCPT.inCommunicationMode)
	{
		case 0:
			inCommMode = DIAL_UP_MODE;
			break;
		case 1:
			inCommMode = ETHERNET_MODE;
			break;
		case 2:
			inCommMode = GPRS_MODE;
			break;
		case 4:
			inCommMode = WIFI_MODE;
			break;
        default:
			inCommMode = ETHERNET_MODE;
			break;
	}

	// Check hardware
	inResult = inCheckPromptHW(inCommMode);
	if(inResult != d_OK)
	return ST_ERROR;

	/*
	if ((inCommMode == GPRS_MODE) || (inCommMode == WIFI_MODE))
		inIPHeader = 1;
	else
		inIPHeader = 0;
	*/
	
	strCPT.inCommunicationMode = inCommMode;
	//strCPT.inIPHeader = inIPHeader;		
	inCPT_Update(inCommMode, inIPHeader);
		
	inTCTRead(1);

	#if 0
	if ((inCommMode == GPRS_MODE) || (inCommMode == WIFI_MODE))
		strTCT.fShareComEnable = 1;
	else
		strTCT.fShareComEnable = 0;
	#endif

	strTCT.inTMSComMode = inCommMode;
	
	inTCTSave(1);

	#if 0
	/*MCC: Update fDHCPEnable for GPRS/WIFI*/
	inTCPRead(1);
	if ((inCommMode == GPRS_MODE) || (inCommMode == WIFI_MODE))
		strTCP.fDHCPEnable = IPCONFIG_DHCP;
	else	
		strTCP.fDHCPEnable = IPCONFIG_STATIC;

	inTCPSave(1);
	/*MCC: Update fDHCPEnable for GPRS/WIFI*/
	#endif

	// Update inCommMode for ERM -- sidumili
	if (strTCT.byERMMode != 0)
	{
		inERMTCT_Update(inCommMode, 1);
	}		

	vduiClearBelow(2);

	if (get_env_int("AUTORECOVER") != 8)
	{
		CTOS_LCDTPrintXY(1, 7, "Configuring Comms");
		CTOS_LCDTPrintXY(1, 8, "Done!            ");
	}
	else
	{
		CTOS_LCDTPrintXY(1, 8, "Please wait...");	
	}
	
	CTOS_Beep();
    CTOS_Delay(500);
	
return d_OK;
	
}

// Enhancement for printing speed
void vdCheckERMMode(void)
{
	vdDebug_LogPrintf("--vdCheckERMMode--");
	
	if(strTCT.byERMMode != 0)
		fERMTransaction = TRUE;
	else
		fERMTransaction = FALSE;

	//vdDebug_LogPrintf("byERMMode[%d] | fERMTransaction[%d]", strTCT.byERMMode, fERMTransaction);
}

void vdSetGSMAuthType(void)
{
	int inType;

	vdDebug_LogPrintf("--vdSetGSMAuthType--");

	// 0 - Auto
	// 1 - PAP
	// 2 - CHAP
	
	inType = get_env_int("GSMAUTHTYPE");
	if (inType < 0)
	{
		put_env_int("GSMAUTHTYPE", 0); //Set Auto Authentication Type
	}	
}

void vdSetGSMNetworkType(void)
{
	int inType;

	vdDebug_LogPrintf("--vdSetGSMNetworkType--");

	// 0 - Auto
	// 1 - GPRS / 2G
	// 2 - UMTS
	// 3 - CDMA
	// 4 - LTE
	
	inType = get_env_int("NETWORKTYPE");
	if (inType < 0)
	{
		put_env_int("NETWORKTYPE", 0); //Set Default Network Type
	}
}

void vdDisplayLoadingApps(void)
{
	vduiClearBelow(2);

	if (get_env_int("AUTORECOVER") != 8)
	{				
		CTOS_LCDTPrintXY(1, 7, "Loading Application");
		CTOS_LCDTPrintXY(1, 8, "Please wait...");
	}
	else
	{
		CTOS_LCDTPrintXY(1, 8, "Please wait...");
	}	
}

void vdInitUSCDCBMODE(void)
{
	USHORT ret;

	vdDebug_LogPrintf("--vdInitUSCDCBMODE--");
	vdDebug_LogPrintf("byRS232ECRPort[%d]", strTCT.byRS232ECRPort);
	vdDebug_LogPrintf("byTerminalType[%d]", strTCT.byTerminalType);

//android-removed
#if 0
	if (strTCT.byRS232ECRPort == ECR_USB_BASE)
		ret =  CTOS_BaseUSBSetCDCMode(); // SYSTEM VALUE [BASE USB CDC = Y]

	if (strTCT.byRS232ECRPort == ECR_USB_TERMINAL)
		ret = CTOS_BaseUSBSetSTDMode();  // SYSTEM VALUE [BASE USB CDC = N]
#endif
//end

	vdDebug_LogPrintf("vdInitUSCDCBMODE::CTOS_BaseUSBSetCDCMode[%d]", ret);

}

void vdCheckISOLog(void)
{
	int inLog = get_env_int("ISOLOG");	
	
	vdDebug_LogPrintf("--vdCheckISOLog--");
	
	if (inLog > 0)
		fISOLog = TRUE;
	else
		fISOLog = FALSE;

	vdDebug_LogPrintf("inLog[%d] | fISOLog[%d]", inLog, fISOLog);
}

int inCTOS_APP_START(void)
{
	int inRet=d_NO;
	int inKey=0;
	char szTransType;
	USHORT usTk1Len, usTk2Len, usTk3Len;
	BYTE szTk1Buf[TRACK_I_BYTES], szTk2Buf[TRACK_II_BYTES], szTk3Buf[TRACK_III_BYTES];
	usTk1Len = TRACK_I_BYTES ;
	usTk2Len = TRACK_II_BYTES ;
	usTk3Len = TRACK_III_BYTES ;

	vdDebug_LogPrintf("saturn --APPSTART--");
	vdCTOS_ResetMagstripCardData();

	vdGetGlobalVariable(); // sidumili: For global varibles
	
	inKey = GetMenuTransTypes();

    vdRemoveCard();
	CTOS_MSRRead(szTk1Buf, &usTk1Len, szTk2Buf, &usTk2Len, szTk3Buf, &usTk3Len);
	//GetTransactions();
	return inRet;
}

