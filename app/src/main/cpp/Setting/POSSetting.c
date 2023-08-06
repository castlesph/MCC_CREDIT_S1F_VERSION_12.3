/*******************************************************************************

*******************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctosapi.h>
#include <CTOSInput.h>

#include "../Includes/myEZLib.h"
#include "../Includes/msg.h"
#include "../Includes/wub_lib.h"


#include "..\Includes\CTOSInput.h"
#include "../Includes/POSTypedef.h"
#include "../FileModule/myFileFunc.h"

#include "../print/Print.h"
#include "../FileModule/myFileFunc.h"
#include "../Comm/V5Comm.h"


#include "..\Includes\CTOSInput.h"


#include "../Accum/accum.h"
#include "../DataBase/DataBaseFunc.h"
#include "..\Includes\POSSetting.h"
#include "..\ui\Display.h"
#include "..\Database\DatabaseFunc.h"
#include "..\debug\debug.h"
#include "..\Includes\POSTrans.h"
#include "..\Accum\Accum.h"
#include "..\filemodule\myFileFunc.h"
#include "..\Includes\MultiApLib.h"
#include "..\Aptrans\MultiAptrans.h"
#include "..\Aptrans\MultiShareEMV.h"
#include "..\PCI100\PCI100.h"
#include "../Debug/debug.h"
#include "../UIapi.h"


#define DISPLAY_POSITION_LEFT 0
#define DISPLAY_POSITION_CENTER 1
#define DISPLAY_POSITION_RIGHT 2
#define DISPLAY_LINE_SIZE 16


BOOL BolDetachGPRSChangeSetting=FALSE, BolDetachDIALChangeSetting=FALSE;

static int inSleepflag = 0;

extern int inEthernet_GetTerminalIP(char *szTerminalIP);
extern int inSettlementHost;


int inSelectedIdleAppsHost;

void vdCTOS_uiPowerOff(void)
{
    BYTE block[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
    USHORT ya,yb,xa,xb;
    unsigned char c;
        
   //CTOS_LCDTClearDisplay();

    vduiDisplayStringCenter(3,"ARE YOU SURE");
    vduiDisplayStringCenter(4,"WANT TO POWER");
    vduiDisplayStringCenter(5,"OFF TERMINAL");
    CTOS_LCDTPrintXY(1,7,"NO[X]   YES[OK] ");
    c=WaitKey(60);
    
    if(c!=d_KBD_ENTER)
    {
        return;
    }
    
    for(ya =1; ya<5; ya++)
    {
        CTOS_Delay(100);
        CTOS_LCDTGotoXY(1,ya);
        CTOS_LCDTClear2EOL();
    }
    for(yb=8; yb>4; yb--)
    {
        CTOS_Delay(100);
        CTOS_LCDTGotoXY(1,yb);
        CTOS_LCDTClear2EOL();
    }
    CTOS_LCDTPrintXY(1,4,"----------------");
    for(xa=1; xa<8; xa++)
    {
        CTOS_Delay(25);
        CTOS_LCDTPrintXY(xa,4," ");
    }
    for(xb=16; xb>7; xb--)
    {
        CTOS_Delay(25);
        CTOS_LCDTPrintXY(xb,4," ");
    }
            
    CTOS_LCDGShowPic(58, 6, block, 0, 6);
    CTOS_Delay(250);
    CTOS_LCDTGotoXY(7,4);
    CTOS_LCDTClear2EOL();
    CTOS_Delay(250);

	CTOS_BackLightSetEx(d_BKLIT_KBD,d_OFF,100);
	CTOS_BackLightSetEx(d_BKLIT_LCD,d_OFF,3000);

    CTOS_PowerOff(); //aaronnino mcc v3 gprs fix on issue #0020 Unable to shutdown terminal. Restart only. Need to remove battery .
    //CTOS_SystemReset();
}

void vdCTOS_uiRestart(void)
{
    BYTE block[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
    USHORT ya,yb,xa,xb;
    unsigned char c;
    int inResult;
	      
    inResult=vduiAskConfirmation("RESTART?");
    if(inResult == d_OK)
    {
		for(ya =1; ya<5; ya++)
		{
			CTOS_Delay(100);
			CTOS_LCDTGotoXY(1,ya);
			CTOS_LCDTClear2EOL();
		}
		for(yb=8; yb>4; yb--)
		{
			CTOS_Delay(100);
			CTOS_LCDTGotoXY(1,yb);
			CTOS_LCDTClear2EOL();
		}
		CTOS_LCDTPrintXY(1,4,"----------------");
		for(xa=1; xa<8; xa++)
		{
			CTOS_Delay(25);
			CTOS_LCDTPrintXY(xa,4," ");
		}
		for(xb=16; xb>7; xb--)
		{
			CTOS_Delay(25);
			CTOS_LCDTPrintXY(xb,4," ");
		}
				
		CTOS_LCDGShowPic(58, 6, block, 0, 6);
		CTOS_Delay(250);
		CTOS_LCDTGotoXY(7,4);
		CTOS_LCDTClear2EOL();
		CTOS_Delay(250);

		CTOS_SystemReset();		
    }
}

int inCTOS_SelectHostSetting(void)
{
    BYTE bHeaderAttr = 0x01+0x04, iCol = 1;
    BYTE  x = 1;
    BYTE key;
    char szHeaderString[50] = "SELECT HOST";
    char szHostMenu[1024];
    char szHostName[50][100];
    int inCPTID[50];
    int inLoop = 0;
    
    if (inMultiAP_CheckSubAPStatus() == d_OK)
        return d_OK;
    

    memset(szHostMenu, 0x00, sizeof(szHostMenu));
    memset(szHostName, 0x00, sizeof(szHostName));
    memset((char*)inCPTID, 0x00, sizeof(inCPTID));

    inHDTReadHostName(szHostName, inCPTID);

    for (inLoop = 0; inLoop < 50; inLoop++)
    {
        if (szHostName[inLoop][0]!= 0)
        {
			if(memcmp(szHostName[inLoop], "MCC ONE APP", strlen("MCC ONE APP")) == 0)
				strcat((char *)szHostMenu, "MC/VISA/JCB/UPI/AMEX");
			else	
                strcat((char *)szHostMenu, szHostName[inLoop]);
            if (szHostName[inLoop+1][0]!= 0)
                strcat((char *)szHostMenu, (char *)" \n");      
        }
        else
            break;
    }	
        
    key = MenuDisplay(szHeaderString, strlen(szHeaderString), bHeaderAttr, iCol, x, szHostMenu, TRUE);


    vdDebug_LogPrintf("saturn after seleting host from menu");

    if (key == 0xFF) 
    {
       //CTOS_LCDTClearDisplay();
        setLCDPrint(1, DISPLAY_POSITION_CENTER, "WRONG INPUT!!!");
        vduiWarningSound();
        return -1;  
    }

    if(key > 0)
    {
        if(d_KBD_CANCEL == key)
            return -2;
        
        vdDebug_LogPrintf("saturn key[%d] HostID[%d]", key, inCPTID[key-1]);
        srTransRec.HDTid = inCPTID[key-1];
        strHDT.inHostIndex = inCPTID[key-1];
        inHDTRead(inCPTID[key-1]);
        inCPTRead(inCPTID[key-1]);
        strcpy(srTransRec.szHostLabel, strHDT.szHostLabel);
    }
    
    return inCPTID[key-1];
}

/*configuration functions*/
void vdCTOS_IPConfig(void)
{
    BYTE bRet, key;
    BYTE szInputBuf[5];
    int inResult;
    BYTE strOut[30],strtemp[17];
    USHORT ret;
    USHORT usLen;
    BOOL BolDetachLANChange=FALSE;
    int shHostIndex = 1;
	int inCommMode = -1;
	BOOL fValid = TRUE;
	
    shHostIndex = inCTOS_SelectHostSetting();
    if (shHostIndex == -1)
        return;
                
    inResult = inCPTRead(shHostIndex);
    if(inResult != d_OK)
        return;

   //CTOS_LCDTClearDisplay();
    vdDispTitleString("IP SETTING");
    while(1)
    {
    	vdDispTitleString("IP SETTING"); //#00196 "Terminal settings" settings header disappears after entering incorrect input
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "Connection Type");
        if(strCPT.inCommunicationMode == ETHERNET_MODE)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "LAN");
        if(strCPT.inCommunicationMode == DIAL_UP_MODE)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "DIAL-UP");        
        if(strCPT.inCommunicationMode == GPRS_MODE)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "GPRS");
        if(strCPT.inCommunicationMode == WIFI_MODE)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "WIFI");

		if (isCheckTerminalMP200() == d_OK)
		{
			CTOS_LCDTPrintXY(1, 5, "1-GPRS");
	        CTOS_LCDTPrintXY(1, 6, "2-WIFI");
		}
		else
		{
			CTOS_LCDTPrintXY(1, 5, "1-DIAL-UP     2-LAN");
	        CTOS_LCDTPrintXY(1, 6, "3-GPRS          4-WIFI");
		}
                
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 1, 0, d_INPUT_TIMEOUT);
		
        if (ret == d_KBD_CANCEL || ret == 0)
            break;
        else
        {					
			if (isCheckTerminalMP200() == d_OK)
			{
				switch (strOut[0])
				{
					case 0x31:
						inCommMode = GPRS_MODE;
                        BolDetachGPRSChangeSetting = TRUE;
					break;
					case 0x32:
						inCommMode = WIFI_MODE;
                        BolDetachGPRSChangeSetting = TRUE;
					break;
					default:
						fValid = FALSE;
					break;
				}
			}
			else
			{
				switch (strOut[0])
				{
					case 0x31:
						inCommMode = DIAL_UP_MODE;
                        BolDetachDIALChangeSetting = TRUE;
					break;
					case 0x32:
						inCommMode = ETHERNET_MODE;
                        BolDetachLANChange = TRUE;
					break;
					case 0x33:
						inCommMode = GPRS_MODE;
                        BolDetachGPRSChangeSetting = TRUE;
					break;
					case 0x34:
						inCommMode = WIFI_MODE;
                        BolDetachGPRSChangeSetting = TRUE;
					break;
					default:
						fValid = FALSE;
					break;
				}
			}		            
        }

		// Check for valid input - sidumili
		if (!fValid)
		{
			CTOS_LCDTClearDisplay();
			vduiWarningSound();
			vduiClearBelow(6);
			vduiDisplayStringCenter(6,"PLEASE SELECT");
			vduiDisplayStringCenter(7,"A VALID");
			vduiDisplayStringCenter(8,"CONNECTION MODE");
			CTOS_Delay(2000);
		}
		else
			break; // exit while loop
		
    }

	// Check hardware
	inResult = inCheckPromptHW(inCommMode);
	if(inResult != d_OK)
		return;
			
    inResult = inCPTSave(shHostIndex);
    inResult = inCPTRead(shHostIndex);

    inUpdateCommsMode(inCommMode);
	
    if(inCommMode == ETHERNET_MODE || inCommMode == GPRS_MODE || inCommMode == WIFI_MODE)
    {
        inResult = inTCPRead(1);
        if(inResult != ST_SUCCESS)
            return;

        while(1)
        {
    	vdDispTitleString("IP SETTING"); //#00196 "Terminal settings" settings header disappears after entering incorrect input
            vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "IP Config");
            if (strTCP.fDHCPEnable == IPCONFIG_DHCP)
                setLCDPrint(4, DISPLAY_POSITION_LEFT, "1");
            if (strTCP.fDHCPEnable == IPCONFIG_STATIC)  
                setLCDPrint(4, DISPLAY_POSITION_LEFT, "0");
            
            CTOS_LCDTPrintXY(1, 5, "0-STATIC     1-DHCP");
            CTOS_LCDTPrintXY(1, 6, "                   ");
            
            
            strcpy(strtemp,"New:") ;
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strOut,0x00, sizeof(strOut));
            ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 1, 0, d_INPUT_TIMEOUT);
            if (ret == d_KBD_CANCEL )
                break;
            else if(0 == ret )
                break;
            else if(ret==1)
            {
                if (strOut[0]==0x30 || strOut[0]==0x31)
                {
                    BolDetachLANChange=TRUE;
                    
                    if(strOut[0] == 0x30)  
                            strTCP.fDHCPEnable = IPCONFIG_STATIC;
                    if(strOut[0] == 0x31)
                            strTCP.fDHCPEnable = IPCONFIG_DHCP;
                    break;
                }
                else
                {
		CTOS_LCDTClearDisplay();
                    vduiWarningSound();
                    vduiClearBelow(6);
                    vduiDisplayStringCenter(6,"PLEASE SELECT");
                    vduiDisplayStringCenter(7,"A VALID");
                    vduiDisplayStringCenter(8,"IP Config");
                    CTOS_Delay(2000);       
                }
            }
        }               
        inResult = inTCPSave(1);
        inResult = inTCPRead(1);

        inResult = inCPTRead(shHostIndex);
        if(inResult != ST_SUCCESS)
            return;
		
        while(1)
        {
    	vdDispTitleString("IP SETTING"); //#00196 "Terminal settings" settings header disappears after entering incorrect input
            vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "PRI HOST IP");
            setLCDPrint(4, DISPLAY_POSITION_LEFT, strCPT.szPriTxnHostIP);
            
            strcpy(strtemp,"New:") ;
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strtemp, 0x00, sizeof(strtemp));
            memset(strOut,0x00, sizeof(strOut));
            ret= struiApiGetStringSub(strtemp, 1,  8, strOut, MODE_IPADDRESS, 7, IP_LEN);
            if(ret==d_KBD_ENTER)
            {
                BolDetachLANChange=TRUE;
                memcpy(strCPT.szPriTxnHostIP,strOut,strlen(strOut));
                strCPT.szPriTxnHostIP[strlen(strOut)]=0;
                vdMyEZLib_LogPrintf("new host ip %s",strCPT.szPriTxnHostIP);
                break;
            }   
            if(ret == d_KBD_CANCEL)
                        break;
            }
        inResult = inCPTSave(shHostIndex);
        inResult = inCPTRead(shHostIndex);
        if(inResult != ST_SUCCESS)
            return;
        
        while(1)
        {   
    	vdDispTitleString("IP SETTING"); //#00196 "Terminal settings" settings header disappears after entering incorrect input
            vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "PRI HOST PORT");
            memset(szInputBuf, 0x00, sizeof(szInputBuf));
            sprintf(szInputBuf, "%d", strCPT.inPriTxnHostPortNum);
            setLCDPrint(4, DISPLAY_POSITION_LEFT, szInputBuf);
            
            strcpy(strtemp,"New:") ;
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strOut,0x00, sizeof(strOut));
            ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 5, 0, d_INPUT_TIMEOUT);
            if (ret == d_KBD_CANCEL )
                break;
            else if(0 == ret )
                break;
            else if(ret>=1)
            {
                BolDetachLANChange=TRUE;
                strCPT.inPriTxnHostPortNum = atoi(strOut);      
                vdMyEZLib_LogPrintf("new host port %d",strCPT.inPriTxnHostPortNum);
                break;
            }   
            if(ret == d_KBD_CANCEL)
                break;
        }
        inResult = inCPTSave(shHostIndex);

        inResult = inCPTRead(shHostIndex);
        if(inResult != ST_SUCCESS)
            return;
        while(1)
        {
    	vdDispTitleString("IP SETTING"); //#00196 "Terminal settings" settings header disappears after entering incorrect input
        
            vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "SEC HOST IP");
            setLCDPrint(4, DISPLAY_POSITION_LEFT, strCPT.szSecTxnHostIP);
            
            strcpy(strtemp,"New:") ;
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strtemp, 0x00, sizeof(strtemp));
            memset(strOut,0x00, sizeof(strOut));
            ret= struiApiGetStringSub(strtemp, 1,  8, strOut, MODE_IPADDRESS, 7, IP_LEN);
            if(ret==d_KBD_ENTER)
            {
                BolDetachLANChange=TRUE;
                memcpy(strCPT.szSecTxnHostIP,strOut,strlen(strOut));
                strCPT.szSecTxnHostIP[strlen(strOut)]=0;
                vdMyEZLib_LogPrintf("new host ip %s",strCPT.szSecTxnHostIP);
                break;
            }   
            if(ret == d_KBD_CANCEL)
                        break;
            }
        inResult = inCPTSave(shHostIndex);
        inResult = inCPTRead(shHostIndex);
        if(inResult != ST_SUCCESS)
            return;
        
        while(1)
        {   
    	vdDispTitleString("IP SETTING"); //#00196 "Terminal settings" settings header disappears after entering incorrect input
        
            vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "SEC HOST PORT");
            memset(szInputBuf, 0x00, sizeof(szInputBuf));
            sprintf(szInputBuf, "%d", strCPT.inSecTxnHostPortNum);
            setLCDPrint(4, DISPLAY_POSITION_LEFT, szInputBuf);
            
            strcpy(strtemp,"New:") ;
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strOut,0x00, sizeof(strOut));
            ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 5, 0, d_INPUT_TIMEOUT);
            if (ret == d_KBD_CANCEL )
                break;
            else if(0 == ret )
                break;
            else if(ret>=1)
            {
                BolDetachLANChange=TRUE;
                strCPT.inSecTxnHostPortNum = atoi(strOut);      
                vdMyEZLib_LogPrintf("new host port %d",strCPT.inSecTxnHostPortNum);
                break;
            }   
            if(ret == d_KBD_CANCEL)
                break;
        }
        inResult = inCPTSave(shHostIndex);

        inResult = inTCPRead(1);
        inResult = inCPTRead(shHostIndex);
        if(inResult != ST_SUCCESS)
            return;
        while(1)
        {
    	vdDispTitleString("IP SETTING"); //#00196 "Terminal settings" settings header disappears after entering incorrect input
        
            vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "SETTLE PRI HOST IP");
            setLCDPrint(4, DISPLAY_POSITION_LEFT, strCPT.szPriSettlementHostIP);
            
            strcpy(strtemp,"New:") ;
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strtemp, 0x00, sizeof(strtemp));
            memset(strOut,0x00, sizeof(strOut));
            ret= struiApiGetStringSub(strtemp, 1,  8, strOut, MODE_IPADDRESS, 7, IP_LEN);
            if(ret==d_KBD_ENTER)
            {
                BolDetachLANChange=TRUE;
                memcpy(strCPT.szPriSettlementHostIP,strOut,strlen(strOut));
                strCPT.szPriSettlementHostIP[strlen(strOut)]=0;
                vdMyEZLib_LogPrintf("new host ip %s",strCPT.szPriSettlementHostIP);
                break;
            }   
            if(ret == d_KBD_CANCEL)
                        break;
            }
        inResult = inCPTSave(shHostIndex);
        inResult = inCPTRead(shHostIndex);
        if(inResult != ST_SUCCESS)
            return;
        
        while(1)
        {   
            vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "SETTLE PRI HOST PORT");
            memset(szInputBuf, 0x00, sizeof(szInputBuf));
            sprintf(szInputBuf, "%d", strCPT.inPriSettlementHostPort);
            setLCDPrint(4, DISPLAY_POSITION_LEFT, szInputBuf);
            
            strcpy(strtemp,"New:") ;
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strOut,0x00, sizeof(strOut));
            ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 5, 0, d_INPUT_TIMEOUT);
            if (ret == d_KBD_CANCEL )
                break;
            else if(0 == ret )
                break;
            else if(ret>=1)
            {
                BolDetachLANChange=TRUE;
                strCPT.inPriSettlementHostPort = atoi(strOut);      
                vdMyEZLib_LogPrintf("new host port %d",strCPT.inPriSettlementHostPort);
                break;
            }   
            if(ret == d_KBD_CANCEL)
                break;
        }
        inResult = inCPTSave(shHostIndex);


        inResult = inCPTRead(shHostIndex);
        if(inResult != ST_SUCCESS)
            return;
        while(1)
        {
    	vdDispTitleString("IP SETTING"); //#00196 "Terminal settings" settings header disappears after entering incorrect input
        
            vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "SETTLE SEC HOST IP");
            setLCDPrint(4, DISPLAY_POSITION_LEFT, strCPT.szSecSettlementHostIP);
            
            strcpy(strtemp,"New:") ;
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strtemp, 0x00, sizeof(strtemp));
            memset(strOut,0x00, sizeof(strOut));
            ret= struiApiGetStringSub(strtemp, 1,  8, strOut, MODE_IPADDRESS, 7, IP_LEN);
            if(ret==d_KBD_ENTER)
            {
                BolDetachLANChange=TRUE;
                memcpy(strCPT.szSecSettlementHostIP,strOut,strlen(strOut));
                strCPT.szSecSettlementHostIP[strlen(strOut)]=0;
                vdMyEZLib_LogPrintf("new host ip %s",strCPT.szSecSettlementHostIP);
                break;
            }   
            if(ret == d_KBD_CANCEL)
                        break;
            }
        inResult = inCPTSave(shHostIndex);
        inResult = inCPTRead(shHostIndex);
        if(inResult != ST_SUCCESS)
            return;
        
        while(1)
        {   
    	vdDispTitleString("IP SETTING"); //#00196 "Terminal settings" settings header disappears after entering incorrect input
        
            vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "SETTLE SEC HOST PORT");
            memset(szInputBuf, 0x00, sizeof(szInputBuf));
            sprintf(szInputBuf, "%d", strCPT.inSecSettlementHostPort);
            setLCDPrint(4, DISPLAY_POSITION_LEFT, szInputBuf);
            
            strcpy(strtemp,"New:") ;
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strOut,0x00, sizeof(strOut));
            ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 5, 0, d_INPUT_TIMEOUT);
            if (ret == d_KBD_CANCEL )
                break;
            else if(0 == ret )
                break;
            else if(ret>=1)
            {
                BolDetachLANChange=TRUE;
                strCPT.inSecSettlementHostPort = atoi(strOut);      
                vdMyEZLib_LogPrintf("new host port %d",strCPT.inSecSettlementHostPort);
                break;
            }   
            if(ret == d_KBD_CANCEL)
                break;
        }
        inResult = inCPTSave(shHostIndex);

        inResult = inCPTRead(shHostIndex); /*add ip header*/
        if(inResult != ST_SUCCESS)
            return;
        while(1)
        {   
            vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "IP HEADER");
            memset(szInputBuf, 0x00, sizeof(szInputBuf));
            sprintf(szInputBuf, "%d", strCPT.inIPHeader);
            setLCDPrint(4, DISPLAY_POSITION_LEFT, szInputBuf);
            
            strcpy(strtemp,"New:") ;
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strOut,0x00, sizeof(strOut));
            ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 1, 0, d_INPUT_TIMEOUT);
            if (ret == d_KBD_CANCEL )
                break;
            else if(0 == ret )
                break;
            else if(ret>=1)
            {
                strCPT.inIPHeader= atoi(strOut);      
                vdMyEZLib_LogPrintf("new host port %d",strCPT.inSecTxnHostPortNum);
                break;
            }   
            if(ret == d_KBD_CANCEL)
                break;
        }
        inResult = inCPTSave(shHostIndex);
		
        inResult = inCPTRead(shHostIndex);      
        if(strTCP.fDHCPEnable == IPCONFIG_STATIC)
        {
            while(1)
            {
    	vdDispTitleString("IP SETTING"); //#00196 "Terminal settings" settings header disappears after entering incorrect input
                vduiClearBelow(3);
                setLCDPrint(3, DISPLAY_POSITION_LEFT, "TERMINAL IP");
                setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCP.szTerminalIP);
                
                strcpy(strtemp,"New:") ;     
                CTOS_LCDTPrintXY(1, 7, strtemp);
                memset(strtemp, 0x00, sizeof(strtemp));
                memset(strOut,0x00, sizeof(strOut));
                ret= struiApiGetStringSub(strtemp, 1,  8, strOut, MODE_IPADDRESS, 7, IP_LEN);
                if(ret==d_KBD_ENTER)
                {
                    BolDetachLANChange=TRUE;
                    memcpy(strTCP.szTerminalIP, strOut, strlen(strOut));
                    strTCP.szTerminalIP[strlen(strOut)]=0;
                    vdMyEZLib_LogPrintf("new Client ip %s",strTCP.szTerminalIP);
                    break;
                }   
                if(ret == d_KBD_CANCEL)
                    break;
            }
            inResult = inTCPSave(1);
            inResult = inTCPRead(1);    
            
            while(1)
            {
    	vdDispTitleString("IP SETTING"); //#00196 "Terminal settings" settings header disappears after entering incorrect input
                vduiClearBelow(3);
                setLCDPrint(3, DISPLAY_POSITION_LEFT, "Sub. Mask IP");
                setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCP.szSubNetMask);
                
                strcpy(strtemp,"New:") ;   
                CTOS_LCDTPrintXY(1, 7, strtemp);
                memset(strtemp, 0x00, sizeof(strtemp));
                memset(strOut,0x00, sizeof(strOut));
                ret= struiApiGetStringSub(strtemp, 1,  8, strOut, MODE_IPADDRESS, 7, IP_LEN);
                if(ret==d_KBD_ENTER)
                {
                    BolDetachLANChange=TRUE;
                    memcpy(strTCP.szSubNetMask, strOut,strlen(strOut));
                    strTCP.szSubNetMask[strlen(strOut)]=0;
                    vdMyEZLib_LogPrintf("new Sub. Mask ip %s", strTCP.szSubNetMask);
                    break;
                }   
                if(ret == d_KBD_CANCEL)
                    break;
            }               
                inResult = inTCPSave(1);
                inResult = inTCPRead(1);                
                while(1)
                {
                    vduiClearBelow(3);
                    setLCDPrint(3, DISPLAY_POSITION_LEFT, "Gateway IP");
                    setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCP.szGetWay);
                    
                    strcpy(strtemp,"New:") ;  
                    CTOS_LCDTPrintXY(1, 7, strtemp);
                    memset(strtemp, 0x00, sizeof(strtemp));
                    memset(strOut,0x00, sizeof(strOut));
                    ret= struiApiGetStringSub(strtemp, 1,  8, strOut, MODE_IPADDRESS, 7, IP_LEN);
                    if(ret==d_KBD_ENTER)
                    {
                        BolDetachLANChange=TRUE;
                        memcpy(strTCP.szGetWay, strOut,strlen(strOut));
                        strTCP.szGetWay[strlen(strOut)]=0;
                        vdMyEZLib_LogPrintf("new Gateway ip %s", strTCP.szGetWay);
                        break;
                    }   
                    if(ret == d_KBD_CANCEL)
                        break;
                }
                inResult = inTCPSave(1);
                inResult = inTCPRead(1);
                
                while(1)
                {
    	vdDispTitleString("IP SETTING"); //#00196 "Terminal settings" settings header disappears after entering incorrect input
                    vduiClearBelow(3);
                    setLCDPrint(3, DISPLAY_POSITION_LEFT, "DNS1 IP");
                    if (wub_strlen(strTCP.szHostDNS1)<=0)
                        setLCDPrint(4, DISPLAY_POSITION_LEFT, "[NOT SET]");
                    else
                        setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCP.szHostDNS1);
                    
                    strcpy(strtemp,"New:");     
                    CTOS_LCDTPrintXY(1, 7, strtemp);
                    memset(strtemp, 0x00, sizeof(strtemp));
                    memset(strOut,0x00, sizeof(strOut));
                    ret= struiApiGetStringSub(strtemp, 1,  8, strOut, MODE_IPADDRESS, 0, IP_LEN);
                    if(ret==d_KBD_ENTER)
                    {
                        if(strlen(strOut)>0)
                        {
                            BolDetachLANChange=TRUE;
                            memcpy(strTCP.szHostDNS1,strOut,strlen(strOut));
                            strTCP.szHostDNS1[strlen(strOut)]=0;
                            vdMyEZLib_LogPrintf("new DNS ip %s", strTCP.szHostDNS1);
                            break;
                        }
                        else
                        {
		CTOS_LCDTClearDisplay();
                            vduiClearBelow(3);
                            vduiDisplayStringCenter(4,"NO DNS IP");
                            vduiDisplayStringCenter(5,"ENTERED,DISABLE");
                            vduiDisplayStringCenter(6,"DNS IP?");
                            vduiDisplayStringCenter(8,"NO[X] YES[OK]");
                            key=struiGetchWithTimeOut();
                            if(key==d_KBD_ENTER)
                            {
                                memset(strTCP.szHostDNS1, 0, sizeof(strTCP.szHostDNS1));
                                break;
                            }
                        }
                    }                       
                    if(ret == d_KBD_CANCEL)
                        break;
                }
                inResult = inTCPSave(1);
                inResult = inTCPRead(1);

                while(1)
                {
    	vdDispTitleString("IP SETTING"); //#00196 "Terminal settings" settings header disappears after entering incorrect input
                    vduiClearBelow(3);
                    setLCDPrint(3, DISPLAY_POSITION_LEFT, "DNS2 IP");
                    if (wub_strlen(strTCP.szHostDNS2)<=0)
                        setLCDPrint(4, DISPLAY_POSITION_LEFT, "[NOT SET]");
                    else
                        setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCP.szHostDNS2);
                    
                    strcpy(strtemp,"New:");     
                    CTOS_LCDTPrintXY(1, 7, strtemp);
                    memset(strtemp, 0x00, sizeof(strtemp));
                    memset(strOut,0x00, sizeof(strOut));
                    ret= struiApiGetStringSub(strtemp, 1,  8, strOut, MODE_IPADDRESS, 0, IP_LEN);
                    if(ret==d_KBD_ENTER)
                    {
                        if(strlen(strOut)>0)
                        {
                            BolDetachLANChange=TRUE;
                            memcpy(strTCP.szHostDNS2,strOut,strlen(strOut));
                            strTCP.szHostDNS2[strlen(strOut)]=0;
                            vdMyEZLib_LogPrintf("new DNS ip %s", strTCP.szHostDNS2);
                            break;
                        }
                        else
                        {
		CTOS_LCDTClearDisplay();
                            vduiClearBelow(3);
                            vduiDisplayStringCenter(4,"NO DNS IP");
                            vduiDisplayStringCenter(5,"ENTERED,DISABLE");
                            vduiDisplayStringCenter(6,"DNS IP?");
                            vduiDisplayStringCenter(8,"NO[X] YES[OK]");
                            key=struiGetchWithTimeOut();
                            if(key==d_KBD_ENTER)
                            {
                                memset(strTCP.szHostDNS2, 0, sizeof(strTCP.szHostDNS2));
                                break;
                            }
                        }
                    }                       
                    if(ret == d_KBD_CANCEL)
                        break;
                }
                inResult = inTCPSave(1);
                inResult = inTCPRead(1);
            }
       
            if(BolDetachLANChange==TRUE)
            {
                srTransRec.usTerminalCommunicationMode = strCPT.inCommunicationMode;
                vduiClearBelow(3);
                CTOS_LCDTPrintXY (1,7, "Please Wait     ");       
                if(strCPT.inCommunicationMode == ETHERNET_MODE)
                    CTOS_LCDTPrintXY(1, 8, "Init LAN...     ");
                else if(strCPT.inCommunicationMode == GPRS_MODE)
                    CTOS_LCDTPrintXY(1, 8, "Init GPRS...     ");
                else if(strCPT.inCommunicationMode == WIFI_MODE)
                    CTOS_LCDTPrintXY(1, 8, "Init WIFI...     ");
                if (inCTOS_InitComm(srTransRec.usTerminalCommunicationMode) != d_OK) 
                {
                   
                    vdDisplayErrorMsg(1, 8, "COMM INIT ERR");
                    return;
                }
                
                srCommFuncPoint.inInitComm(&srTransRec,strCPT.inCommunicationMode);
                srCommFuncPoint.inGetCommConfig(&srTransRec);
                srCommFuncPoint.inSetCommConfig(&srTransRec);

				srCommFuncPoint.inDisconnect(&srTransRec);

            }          
    }
	
    if(strCPT.inCommunicationMode == ETHERNET_MODE)
    {
       if(strTCP.fDHCPEnable != IPCONFIG_STATIC)
       {
//           inEthernet_GetTerminalIP(strtemp);
//           if (strcmp(strtemp,"0.0.0.0") == 0)
           {
                srTransRec.usTerminalCommunicationMode = strCPT.inCommunicationMode;
                if (inCTOS_InitComm(srTransRec.usTerminalCommunicationMode) != d_OK) 
                {
                   
                    vdDisplayErrorMsg(1, 8, "COMM INIT ERR");
                    return;
                }
                srCommFuncPoint.inInitComm(&srTransRec,strCPT.inCommunicationMode);
                srCommFuncPoint.inGetCommConfig(&srTransRec);
                srCommFuncPoint.inSetCommConfig(&srTransRec); 
           }

			inEthernet_GetTerminalIP(strtemp);

			// set static IP			
			//strTCP.fDHCPEnable = IPCONFIG_STATIC;
			strcpy(srEthernetRec.strLocalIP, strtemp);
			srCommFuncPoint.inSetCommConfig(&srTransRec); 

			// set DHCP
			strTCP.fDHCPEnable = IPCONFIG_DHCP;
		    srCommFuncPoint.inSetCommConfig(&srTransRec); 

			srCommFuncPoint.inDisconnect(&srTransRec);
           vduiClearBelow(3);
           setLCDPrint(3, DISPLAY_POSITION_LEFT, "DHCP TERMINAL IP");
           setLCDPrint(4, DISPLAY_POSITION_LEFT, strtemp);
           WaitKey(30);
       }
    }
    return;
}

void vdCTOS_TerminalConfig(void)
{
    int inResult;
    USHORT ret, inRet;
    USHORT usLen;
    int shHostIndex = 1;
    BYTE bRet,strOut[30],strtemp[17],key;
        
   //CTOS_LCDTClearDisplay();
    vdDispTitleString("TERMINAL SETTING");
    
    // fix for issue #00090 Can enable and disable USD currency even if batch is not empty
    //#00227 - Terminal allow to update any in TERMINAL SETTING SETUP even there are transaction on the existing batch.
    //inResult = inCTOSS_TMSChkBatchEmptyProcess();
    if(inCheckAllBatchEmtpy() > 0)
    {
        vdDisplayErrorMsg2(1, 8, "BATCH NOT", "EMPTY,SKIPPED.", MSG_TYPE_INFO);
        vdSetErrorMessage("");
        //return (d_FAIL)	;	
    }

    inResult = inTCTRead(1);
    //if (inResult != d_OK)
    //    return;

    while(1)
    {
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "BANK COPY");
		
        if(strTCT.fPrintBankCopy == 0)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "DISABLE");
        if(strTCT.fPrintBankCopy == 1)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "ENABLE");     
        
        CTOS_LCDTPrintXY(1, 5, "0-DISABLE      1-ENABLE");

        
        strcpy(strtemp,"New:") ; 
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        memset(strtemp, 0x00, sizeof(strtemp));
        usLen = 10;
        ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 0, d_INPUT_TIMEOUT);
        if(ret==d_KBD_ENTER)
        {
            if (strOut[0]==0x30 || strOut[0]==0x31)
            {
                 if(strOut[0] == 0x31)
                 {
                        strTCT.fPrintBankCopy = 1;
                 }
                 if(strOut[0] == 0x30)
                 {
                        strTCT.fPrintBankCopy = 0;
                 }
                 inTCTSave(1);
                 break;
             }
             else
             {
				 vduiClearBelow(2);
				 vduiWarningSound();
				 vduiDisplayStringCenter(7,"PLEASE SELECT");
				 vduiDisplayStringCenter(8,"A VALID VALUE");
				 CTOS_Delay(2000);		 
            }

        }
            if(ret == d_KBD_CANCEL)
                break;
   }

    inResult = inTCTRead(1);
    //if (inResult != d_OK)
    //    return;

    while(1)
    {
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "MERCH COPY");
		
        if(strTCT.fPrintMerchCopy == 0)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "DISABLE");
        if(strTCT.fPrintMerchCopy == 1)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "ENABLE");     
        
        CTOS_LCDTPrintXY(1, 5, "0-DISABLE      1-ENABLE");

        
        strcpy(strtemp,"New:") ; 
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        memset(strtemp, 0x00, sizeof(strtemp));
        usLen = 10;
        ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 0, d_INPUT_TIMEOUT);
        if(ret==d_KBD_ENTER)
        {
            if (strOut[0]==0x30 || strOut[0]==0x31)
            {
                 if(strOut[0] == 0x31)
                 {
                        strTCT.fPrintMerchCopy = 1;
                 }
                 if(strOut[0] == 0x30)
                 {
                        strTCT.fPrintMerchCopy = 0;
                 }
                 inTCTSave(1);
                 break;
             }
             else
             {
				 vduiClearBelow(2);
				 vduiWarningSound();
				 vduiDisplayStringCenter(7,"PLEASE SELECT");
				 vduiDisplayStringCenter(8,"A VALID VALUE");
				 CTOS_Delay(2000);		 
            }

        }
            if(ret == d_KBD_CANCEL)
                break;
   }

    inResult = inTCTRead(1);
    //if (inResult != d_OK)
    //    return;

    while(1)
    {
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "MANUAL ENTRY");
		
        if(strTCT.fManualEntry == 0)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "DISABLE");
        if(strTCT.fManualEntry == 1)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "ENABLE");     
        
        CTOS_LCDTPrintXY(1, 5, "0-DISABLE      1-ENABLE");

        
        strcpy(strtemp,"New:") ; 
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        memset(strtemp, 0x00, sizeof(strtemp));
        usLen = 10;
        ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 0, d_INPUT_TIMEOUT);
        if(ret==d_KBD_ENTER)
        {
            if (strOut[0]==0x30 || strOut[0]==0x31)
            {
                 if(strOut[0] == 0x31)
                 {
                        strTCT.fManualEntry = 1;
                 }
                 if(strOut[0] == 0x30)
                 {
                        strTCT.fManualEntry = 0;
                 }
                 inTCTSave(1);
                 break;
             }
             else
             {
				 vduiClearBelow(2);
				 vduiWarningSound();
				 vduiDisplayStringCenter(7,"PLEASE SELECT");
				 vduiDisplayStringCenter(8,"A VALID VALUE");
				 CTOS_Delay(2000);		 
            }

        }
            if(ret == d_KBD_CANCEL)
                break;
        }

    inResult = inTCTRead(1);
    //if (inResult != d_OK)
    //    return;

    while(1)
    {
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "TERMINAL USD REG");
		
        if(strTCT.fRegUSD == 0)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "DISABLE");
        if(strTCT.fRegUSD == 1)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "ENABLE");     
        
        CTOS_LCDTPrintXY(1, 5, "0-DISABLE      1-ENABLE");

        
        strcpy(strtemp,"New:") ; 
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        memset(strtemp, 0x00, sizeof(strtemp));
        usLen = 10;
        ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 0, d_INPUT_TIMEOUT);
        if(ret==d_KBD_ENTER)
        {
            if (strOut[0]==0x30 || strOut[0]==0x31)
            {
                 if(strOut[0] == 0x31)
                 {
                        strTCT.fRegUSD = 1;
                 }
                 if(strOut[0] == 0x30)
                 {
                        strTCT.fRegUSD = 0;
                 }
                 inTCTSave(1);
                 break;
             }
             else
             {
				 vduiClearBelow(2);
				 vduiWarningSound();
				 vduiDisplayStringCenter(7,"PLEASE SELECT");
				 vduiDisplayStringCenter(8,"A VALID VALUE");
				 CTOS_Delay(2000);		 
            }

        }
            if(ret == d_KBD_CANCEL)
                break;
    }
	
    inResult = inTCTRead(1);
   // if (inResult != d_OK)
    //    return;

    while(1)
    {
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "TERMINAL SERIAL NO");
		
        if(strTCT.fTSNEnable == 0)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "0");
        if(strTCT.fTSNEnable == 1)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "1");     
        
        CTOS_LCDTPrintXY(1, 5, "0-DISABLE      1-ENABLE");
        
        strcpy(strtemp,"New:") ; 
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        memset(strtemp, 0x00, sizeof(strtemp));
        usLen = 10;
        ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 0, d_INPUT_TIMEOUT);
        if(ret==d_KBD_ENTER)
        {
            if (strOut[0]==0x30 || strOut[0]==0x31)
            {
                 if(strOut[0] == 0x31)
                 {
                        strTCT.fTSNEnable = 1;
                 }
                 if(strOut[0] == 0x30)
                 {
                        strTCT.fTSNEnable = 0;
                 }
                 inTCTSave(1);
                 break;
             }
             else
             {
				 vduiClearBelow(2);
				 vduiWarningSound();
				 vduiDisplayStringCenter(7,"PLEASE SELECT");
				 vduiDisplayStringCenter(8,"A VALID VALUE");
				 CTOS_Delay(2000);		 
            }
        }
            if(ret == d_KBD_CANCEL)
                break;
    }

    inResult = inTCTRead(1);
    //if (inResult != d_OK)
     //   return;
	
    while(1)
    {
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "OFFLINE CEILING");	
		memset(strtemp, 0, sizeof(strtemp));
		wub_hex_2_str(strTCT.szOfflineCeilingAmt, strtemp,6); 
        memset(strOut,0x00, sizeof(strOut));
        sprintf(strOut,"%ld", atol(strtemp));
        CTOS_LCDTPrintXY(1, 4, strOut);
		
		memset(strtemp, 0, sizeof(strtemp));
        strcpy(strtemp,"New:") ; 
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        memset(strtemp, 0x00, sizeof(strtemp));
        usLen = 9;
        ret = InputString2(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
        if(ret==d_KBD_ENTER)
        {
			if(strlen(strOut))
			{
				sprintf(strtemp, "%012ld", atol(strOut));
				wub_str_2_hex(strtemp, strTCT.szOfflineCeilingAmt, 12);
				inTCTSave(1);
				break;
			}
        }
        if(ret == d_KBD_CANCEL || ret == 0xFF)
            break;
    }

    while(1)
    {
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "PRINT RECEIPT");
		
        if(strTCT.fPrintReceipt== 0)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "DISABLE");
        if(strTCT.fPrintReceipt == 1)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "ENABLE");     
        
        CTOS_LCDTPrintXY(1, 5, "0-DISABLE      1-ENABLE");
        
        strcpy(strtemp,"New:") ; 
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        memset(strtemp, 0x00, sizeof(strtemp));
        usLen = 10;
        ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 0, d_INPUT_TIMEOUT);
        if(ret==d_KBD_ENTER)
        {
            if (strOut[0]==0x30 || strOut[0]==0x31)
            {
                 if(strOut[0] == 0x31)
                 {
                        strTCT.fPrintReceipt = 1;
                 }
                 if(strOut[0] == 0x30)
                 {
                        strTCT.fPrintReceipt = 0;
                 }
                 inTCTSave(1);
                 break;
             }
             else
             {
				 vduiClearBelow(2);
				 vduiWarningSound();
				 vduiDisplayStringCenter(7,"PLEASE SELECT");
				 vduiDisplayStringCenter(8,"A VALID VALUE");
				 CTOS_Delay(2000);		 
            }
        }
            if(ret == d_KBD_CANCEL)
                break;
    }

    while(1)
    {
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "EMV ONLINE PIN");
		
        if(strTCT.fEMVOnlinePIN== 0)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "DISABLE");
        if(strTCT.fEMVOnlinePIN == 1)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "ENABLE");     
        
        CTOS_LCDTPrintXY(1, 5, "0-DISABLE      1-ENABLE");
        
        strcpy(strtemp,"New:") ; 
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        memset(strtemp, 0x00, sizeof(strtemp));
        usLen = 10;
        ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 0, d_INPUT_TIMEOUT);
        if(ret==d_KBD_ENTER)
        {
            if (strOut[0]==0x30 || strOut[0]==0x31)
            {
                 if(strOut[0] == 0x31)
                 {
                        strTCT.fEMVOnlinePIN = 1;
                 }
                 if(strOut[0] == 0x30)
                 {
                        strTCT.fEMVOnlinePIN = 0;
                 }
                 inTCTSave(1);
                 break;
             }
             else
             {
				 vduiClearBelow(2);
				 vduiWarningSound();
				 vduiDisplayStringCenter(7,"PLEASE SELECT");
				 vduiDisplayStringCenter(8,"A VALID VALUE");
				 CTOS_Delay(2000);		 
            }
        }
            if(ret == d_KBD_CANCEL)
                break;
    }

    inResult = inTCTRead(1);
    //if (inResult != d_OK)
    //    return;	

    while(1)
    {
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "IDLE SALE TYPE");
        if(strTCT.byIdleSaleType== 1)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "CREDIT");
        if(strTCT.byIdleSaleType == 2)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "DEBIT");        
        if(strTCT.byIdleSaleType == 3)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "INST");
        if(strTCT.byIdleSaleType == 4)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "LOYALTY");  
        if(strTCT.byIdleSaleType == 5)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "BANCNET");  
		
        CTOS_LCDTPrintXY(1, 5, "1-CREDIT 2-DEBIT");
        CTOS_LCDTPrintXY(1, 6, "3-INST 4-LOY 5-BANC");
        
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 1, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret==1)
        {
            if (strOut[0]==0x31 || strOut[0]==0x32 || strOut[0]==0x33 || strOut[0]==0x34)
            {
                 if(strOut[0] == 0x31)
                 {
                        strTCT.byIdleSaleType = 1;
                 }
                 if(strOut[0] == 0x32)
                 {
                        strTCT.byIdleSaleType = 2;
                 }
                 if(strOut[0] == 0x33)
                 {
                        strTCT.byIdleSaleType = 3;
                 }
                 if(strOut[0] == 0x34)
                 {
                        strTCT.byIdleSaleType = 4;
                 }
                
                 inRet = inTCTSave(1);
                 
                 vdDebug_LogPrintf(". inTCTSave(%d)",inRet);
                 break;
             }
             else
             {
				 vduiClearBelow(2);
				 vduiWarningSound();
				 vduiDisplayStringCenter(7,"PLEASE SELECT");
				 vduiDisplayStringCenter(8,"A VALID VALUE");
				 CTOS_Delay(2000);		 
            }
        }
        if (ret == d_KBD_CANCEL )
            break ;
    }	

    inResult = inTCTRead(1);
    //if (inResult != d_OK)
    //    return;	

    while(1)
    {
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "IDLE SWIPE");
        
        if(strTCT.fIdleSwipeAllow== 0)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "DISABLE");
        if(strTCT.fIdleSwipeAllow == 1)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "ENABLE"); 	
        
        CTOS_LCDTPrintXY(1, 5, "0-DISABLE      1-ENABLE");
        
        strcpy(strtemp,"New:") ; 
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        memset(strtemp, 0x00, sizeof(strtemp));
        usLen = 10;
        ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 0, d_INPUT_TIMEOUT);
        if(ret==d_KBD_ENTER)
        {
            if (strOut[0]==0x30 || strOut[0]==0x31)
            {
                if(strOut[0] == 0x31)
                {
                    strTCT.fIdleSwipeAllow= 1;
                }
                if(strOut[0] == 0x30)
                {
                    strTCT.fIdleSwipeAllow = 0;
                }
                inTCTSave(1);
                break;
            }
            else
            {
                vduiClearBelow(2);
                vduiWarningSound();
                vduiDisplayStringCenter(7,"PLEASE SELECT");
                vduiDisplayStringCenter(8,"A VALID VALUE");
                CTOS_Delay(2000);		
            }
        }
        if(ret == d_KBD_CANCEL || ret == 0xFF)
            break;
    }

    inResult = inTCTRead(1);
    //if (inResult != d_OK)
    //    return;	

    while(1)
    {
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "IDLE INSERT");
        
        if(strTCT.fIdleInsertAllow== 0)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "DISABLE");
        if(strTCT.fIdleInsertAllow == 1)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "ENABLE"); 	
        
        CTOS_LCDTPrintXY(1, 5, "0-DISABLE      1-ENABLE");
        
        strcpy(strtemp,"New:") ; 
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        memset(strtemp, 0x00, sizeof(strtemp));
        usLen = 10;
        ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 0, d_INPUT_TIMEOUT);
        if(ret==d_KBD_ENTER)
        {
            if (strOut[0]==0x30 || strOut[0]==0x31)
            {
                if(strOut[0] == 0x31)
                {
                    strTCT.fIdleInsertAllow= 1;
                }
                if(strOut[0] == 0x30)
                {
                    strTCT.fIdleInsertAllow = 0;
                }
                inTCTSave(1);
                break;
            }
            else
            {
                vduiClearBelow(2);
                vduiWarningSound();
                vduiDisplayStringCenter(7,"PLEASE SELECT");
                vduiDisplayStringCenter(8,"A VALID VALUE");
                CTOS_Delay(2000);		
            }
        }
        if(ret == d_KBD_CANCEL || ret == 0xFF)
            break;
    }

    inResult = inTCTRead(1);
    //if (inResult != d_OK)
    //    return;			
}

void vdCTOS_DialConfig(void)
{
    BYTE bRet,strOut[30],strtemp[17],key;
    BYTE szInputBuf[24+1];
    BYTE szIntComBuf[2];
    BYTE szPhNoBuf[9];
    BYTE szExtNoBuf[4];
    int inResult;
    USHORT ret;
    USHORT usLen;
    int shHostIndex = 1;
    
    shHostIndex = inCTOS_SelectHostSetting();
    if (shHostIndex == -1)
        return;
    
    inResult = inCPTRead(shHostIndex);  
    if (inResult != d_OK)
        return;
    
    inResult = inTCTRead(1);
    if (inResult != d_OK)
        return;
    
   //CTOS_LCDTClearDisplay();
    vdDispTitleString("DIALUP SETTING");
    
    while(1)
    {
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "PRIMARY PABX");
        if (wub_strlen(strTCT.szPabx)<=0)
            setLCDPrint(4,DISPLAY_POSITION_LEFT, "[DISABLED]");
        else
            setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCT.szPabx);

        
        strcpy(strtemp,"New:") ; 
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        memset(strtemp, 0x00, sizeof(strtemp));
        usLen = 10;
        ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 0, d_INPUT_TIMEOUT);
        if(ret==d_KBD_ENTER)
        {
                BolDetachDIALChangeSetting = TRUE;
                if(strlen(strOut)>0)
                {
                    memcpy(strTCT.szPabx,strOut,strlen(strOut));
                    strTCT.szPabx[strlen(strOut)]=',';
                    strTCT.szPabx[strlen(strOut)+1]=0;
                    break;
                }
                else
                {
                    vduiClearBelow(4);
                    vduiDisplayStringCenter(5,"NO PABX VALUE");
                    vduiDisplayStringCenter(6,"ENTERED,DISABLE");
                    vduiDisplayStringCenter(7,"PABX?");
                    vduiDisplayStringCenter(8,"NO[X] YES[OK]");
                    key=struiGetchWithTimeOut();
                    if(key==d_KBD_ENTER)
                    {
                        memset(strTCT.szPabx,0,sizeof(strTCT.szPabx));
                                                break;
                    }
                }
        }
            if(ret == d_KBD_CANCEL)
                break;
        }

        inTCTSave(1);
        
        while(1)
        {
            vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "PRIMARY LINE");
            setLCDPrint(4, DISPLAY_POSITION_LEFT, strCPT.szPriTxnPhoneNumber);

            strcpy(strtemp,"New:") ; 
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strOut,0x00, sizeof(strOut));
            usLen = 18;
            ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
            if (ret == d_KBD_CANCEL )
                break;
            else if(0 == ret )
                break;
            else if(ret>= 1)
            {
                BolDetachDIALChangeSetting = TRUE;
                memcpy(strCPT.szPriTxnPhoneNumber,strOut,strlen(strOut));
                strCPT.szPriTxnPhoneNumber[strlen(strOut)]=0;
                break;
            }
            if(ret == d_KBD_CANCEL)
                break;

        }
            
        while(1)
        {
            vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "SECONDARY LINE");
            setLCDPrint(4, DISPLAY_POSITION_LEFT, strCPT.szSecTxnPhoneNumber);
            
            strcpy(strtemp,"New:") ; 
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strOut,0x00, sizeof(strOut));
            usLen = 18;
            ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
            if (ret == d_KBD_CANCEL )
                break;
            else if(0 == ret )
                break;
            else if(ret>= 1)
            {
                BolDetachDIALChangeSetting = TRUE;
                memcpy(strCPT.szSecTxnPhoneNumber,strOut,strlen(strOut));
                strCPT.szSecTxnPhoneNumber[strlen(strOut)]=0;
                break;
            }
            if(ret == d_KBD_CANCEL)
                break;

        }
            
        while(1)
        {
            vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "SETTLEMENT PRI LINE");
            setLCDPrint(4, DISPLAY_POSITION_LEFT, strCPT.szPriSettlePhoneNumber);
            
            strcpy(strtemp,"New:") ; 
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strOut,0x00, sizeof(strOut));
            usLen = 18;
            ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
            if (ret == d_KBD_CANCEL )
                break;
            else if(0 == ret )
                break;
            else if(ret>= 1)
            {
                BolDetachDIALChangeSetting = TRUE;
                memcpy(strCPT.szPriSettlePhoneNumber,strOut,strlen(strOut));
                strCPT.szPriSettlePhoneNumber[strlen(strOut)]=0;
                break;
            }
            if(ret == d_KBD_CANCEL)
                break;

        }
            
        while(1)
        {
            vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "SETTLEMENT SEC LINE");
            setLCDPrint(4, DISPLAY_POSITION_LEFT, strCPT.szSecSettlePhoneNumber);
            
            strcpy(strtemp,"New:") ; 
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strOut,0x00, sizeof(strOut));
            usLen = 18;
            ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
            if (ret == d_KBD_CANCEL )
                break;
            else if(0 == ret )
                break;
            else if(ret>= 1)
            {
                BolDetachDIALChangeSetting = TRUE;
                memcpy(strCPT.szSecSettlePhoneNumber,strOut,strlen(strOut));
                strCPT.szSecSettlePhoneNumber[strlen(strOut)]=0;
                break;
            }
            if(ret == d_KBD_CANCEL)
                break;

        }

    inResult = inCPTSave(shHostIndex);      
    inResult = inCPTRead(shHostIndex);      
            
    if(BolDetachDIALChangeSetting==TRUE)
    {
        srTransRec.usTerminalCommunicationMode = strCPT.inCommunicationMode;
        vduiClearBelow(3);
        CTOS_LCDTPrintXY (1,7, "Please Wait     ");       
        CTOS_LCDTPrintXY(1, 8, "Init Modem...     ");

        if (inCTOS_InitComm(srTransRec.usTerminalCommunicationMode) != d_OK) 
        {
           
            vdDisplayErrorMsg(1, 8, "COMM INIT ERR");
            return;
        }
        
        srCommFuncPoint.inInitComm(&srTransRec,strCPT.inCommunicationMode);
        srCommFuncPoint.inGetCommConfig(&srTransRec);
        srCommFuncPoint.inSetCommConfig(&srTransRec); 

    }
}

void vdCTOS_ModifyEdcSetting(void)
{
    BYTE bRet;
    BYTE szInputBuf[15+1];
    int inResult,inResult1;
    TRANS_TOTAL stBankTotal;
    BYTE strOut[30],strtemp[17],key;
    USHORT ret;
    USHORT usLen;
    BYTE szTempBuf[12+1];
    BOOL isKey;
    int shHostIndex = 1;
    int inNum = 0, inRet;
    
    shHostIndex = inCTOS_SelectHostSetting();
    if (shHostIndex == -1)
        return;

	CTOS_LCDTClearDisplay();

    if (inMultiAP_CheckMainAPStatus() == d_OK)
    {
        inRet = inCTOS_MultiAPSaveData(d_IPC_CMD_EDC_SETTINGS);
        if(d_OK != inRet)
              return ;
    }
	else
	{
        if (inMultiAP_CheckSubAPStatus() == d_OK)
        {
            inRet = inCTOS_MultiAPGetData();
            if(d_OK != inRet)
                return ;
            
            inRet = inCTOS_MultiAPReloadHost();
            if(d_OK != inRet)
                return ;
        }
    }

    shHostIndex=strHDT.inHostIndex;

    inResult = inCTOS_CheckAndSelectMutipleMID();
    if(d_OK != inResult)
        return;
				
   //CTOS_LCDTClearDisplay();
    vdDispTitleString("HOST SETTING");

    if(inBatchNumRecord()>0)
    {
        vduiWarningSound();
        vduiClearBelow(5);
        setLCDPrint(7, DISPLAY_POSITION_LEFT,"BATCH NOT");
        setLCDPrint(8, DISPLAY_POSITION_LEFT,"EMPTY,SKIPPED.");
        CTOS_Delay(2000);
		return;
    }

#if 0
    while(1)
    {    
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "MMT ENABLE");
        if(strMMT[0].fMMTEnable== 0)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "0");
        else if(strMMT[0].fMMTEnable== 1)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "1");     
        
        CTOS_LCDTPrintXY(1, 5, "0-DISABLE      1-ENABLE");
        
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 1, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret==1)
        {
            if (strOut[0]==0x30 || strOut[0]==0x31)
            {
                 if(strOut[0] == 0x31)
                 {
                        strMMT[0].fMMTEnable= 1;
                 }
                 if(strOut[0] == 0x30)
                 {
                        strMMT[0].fMMTEnable = 0;
                 }

                 //inHDTSave(shHostIndex);
                inMMTSave(strMMT[0].MMTid);                 
                 break;
             }
             else
             {
                vduiWarningSound();
                vduiDisplayStringCenter(7,"PLEASE SELECT");
                vduiDisplayStringCenter(8,"A VALID INPUT");
                CTOS_Delay(2000);       
            }
        }
		
        if (ret == d_KBD_CANCEL )
            break ;
    }
#endif	
    while(1)
    {
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "TERMINAL ID");
        setLCDPrint(4, DISPLAY_POSITION_LEFT, strMMT[0].szTID);
    
        strcpy(strtemp,"New:");
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 8, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret>=1)
        {
            inNum = inBatchNumRecord();
            if (inNum == 0)
            {
                memset(strMMT[0].szTID, 0x00, sizeof(strMMT[0].szTID));
                memcpy(strMMT[0].szTID, strOut, TERMINAL_ID_BYTES);
                strMMT[0].szTID[TERMINAL_ID_BYTES]=0;
                inMMTSave(strMMT[0].MMTid);
                break;
            }
            else
            {
                vduiWarningSound();
                vduiClearBelow(5);
                vduiDisplayStringCenter(6,"BATCH NOT");
                vduiDisplayStringCenter(7,"EMPTY,SKIPPED.");
                CTOS_Delay(2000);
                break;
            }
       }
   if (ret == d_KBD_CANCEL )
        break ;
    }
    
    while(1)
    {
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "MERCHANT ID");
        setLCDPrint(4, DISPLAY_POSITION_LEFT, strMMT[0].szMID);
        
        strcpy(strtemp,"New:");
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 15, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret>=1)
        {
            inNum = inBatchNumRecord();
            if (inNum == 0)
            {
                memset(strMMT[0].szMID, 0x00, sizeof(strMMT[0].szMID));
                memcpy(strMMT[0].szMID, strOut, MERCHANT_ID_BYTES);
                strMMT[0].szMID[MERCHANT_ID_BYTES]=0;
                inMMTSave(strMMT[0].MMTid);
                break;
            }
            else
            {
                vduiWarningSound();
                vduiClearBelow(5);
                vduiDisplayStringCenter(6,"BATCH NOT");
                vduiDisplayStringCenter(7,"EMPTY,SKIPPED.");
                CTOS_Delay(2000);
                break;
            }
       }
       if (ret == d_KBD_CANCEL )
            break ;
    }

    while(1)
    {
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "TPDU");
        wub_hex_2_str(strHDT.szTPDU,szTempBuf,5);
        setLCDPrint(4, DISPLAY_POSITION_LEFT, szTempBuf);
        
        strcpy(strtemp,"New:");
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 10, 10, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret==10)
        {
            inNum = inBatchNumRecord();
            if (inNum == 0)
            {
                memset(strHDT.szTPDU, 0x00, sizeof(strHDT.szTPDU)); 
                wub_str_2_hex(strOut,strtemp,TPDU_BYTES);
                memcpy(strHDT.szTPDU, strtemp, TPDU_BYTES/2);
                inHDTSave(shHostIndex);
                break;
            }
            else
            {
                vduiWarningSound();
                vduiClearBelow(5);
                vduiDisplayStringCenter(6,"BATCH NOT");
                vduiDisplayStringCenter(7,"EMPTY,SKIPPED.");
                CTOS_Delay(2000);
                break;
            }
       }
       if (ret == d_KBD_CANCEL )
            break ;
    }
    
    while(1)
    {
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "NII");
        wub_hex_2_str(strHDT.szNII,szTempBuf,2);
        setLCDPrint(4, DISPLAY_POSITION_LEFT, szTempBuf);
        
        strcpy(strtemp,"New:");
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 4, 4, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret==4)
        {
            inNum = inBatchNumRecord();
            if (inNum == 0)
            {
                memset(strHDT.szNII, 0x00, sizeof(strHDT.szNII));
                wub_str_2_hex(strOut, strtemp, NII_BYTES);
                memcpy(strHDT.szNII, strtemp, NII_BYTES/2);
                inHDTSave(shHostIndex);
                break;
            }
            else
            {
                vduiWarningSound();
                vduiClearBelow(5);
                vduiDisplayStringCenter(6,"BATCH NOT");
                vduiDisplayStringCenter(7,"EMPTY,SKIPPED.");
                CTOS_Delay(2000);
                break;
            }
       }
       if (ret == d_KBD_CANCEL )
            break ;
        }

    inTCTRead(1);    
    while(1)
    {
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "System PWD");
        setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCT.szSystemPW);
        
        strcpy(strtemp,"New:");
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 6, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret>=1)
        {
            if (strlen(strOut)>0)
            {
                memset(strTCT.szSystemPW,0x00,PASSWORD_LEN);
                memcpy(strTCT.szSystemPW,strOut, strlen(strOut));
                strTCT.szSystemPW[strlen(strOut)]=0;
                inTCTSave(1);
                break;
            }
            else
            {
                vduiClearBelow(3);
                vduiDisplayStringCenter(4,"NO PASSWORD");
                vduiDisplayStringCenter(5,"ENTERED,DISABLE");
                vduiDisplayStringCenter(6,"SYSTEM PWD?");
                vduiDisplayStringCenter(8,"NO[X] YES[OK]");
                key=WaitKey(60);
                if(key==d_KBD_ENTER)
                {
                    memset(strTCT.szSystemPW,0x00,PASSWORD_LEN);
                    inTCTSave(1);
                    break;
                }
            }
    }
    if (ret == d_KBD_CANCEL )
        break ;
    }
  
    inTCTRead(1);
    while(1)
    {    
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "Engineer PWD");
        setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCT.szEngineerPW);
        
        strcpy(strtemp,"New:");
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 6, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret>=1)
        {
            if (strlen(strOut)>0)
            {
                memset(strTCT.szEngineerPW,0x00,PASSWORD_LEN);
                memcpy(strTCT.szEngineerPW,strOut, strlen(strOut));
                strTCT.szEngineerPW[strlen(strOut)]=0;
                inTCTSave(1);
                break;
            }
            else
            {
                vduiClearBelow(3);
                vduiDisplayStringCenter(4,"NO PASSWORD");
                vduiDisplayStringCenter(5,"ENTERED,DISABLE");
                vduiDisplayStringCenter(6,"ENGINEER PWD?");
                vduiDisplayStringCenter(8,"NO[X] YES[OK]");
                key=WaitKey(60);
                if(key==d_KBD_ENTER)
                {
                    memset(strTCT.szEngineerPW,0x00,PASSWORD_LEN);
                    inTCTSave(1);
                    break;
                }
            }
        }
        if (ret == d_KBD_CANCEL )
            break ;
        }

	inTCTRead(1);
    while(1)
	{    
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "PM password");
        setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCT.szPMpassword);
        
        strcpy(strtemp,"New:");
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 8, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret>=1)
        {
            if (strlen(strOut)>0)
            {
                memset(strTCT.szPMpassword,0x00,8);
                memcpy(strTCT.szPMpassword,strOut, strlen(strOut));
                strTCT.szPMpassword[strlen(strOut)]=0;
                inTCTSave(1);
                break;
            }
            else
            {
                vduiClearBelow(3);
                vduiDisplayStringCenter(4,"NO PASSWORD");
                vduiDisplayStringCenter(5,"ENTERED,DISABLE");
                vduiDisplayStringCenter(6,"ENGINEER PWD?");
                vduiDisplayStringCenter(8,"NO[X] YES[OK]");
                key=WaitKey(60);
                if(key==d_KBD_ENTER)
                {
                    memset(strTCT.szPMpassword,0x00,8);
                    inTCTSave(1);
                    break;
                }
            }
        }
        if (ret == d_KBD_CANCEL )
            break ;
	}

	inTCTRead(1);
    while(1)
	{    
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "FunKey PWD");
        setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCT.szFunKeyPW);
        
        strcpy(strtemp,"New:");
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 8, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret>=1)
        {
            if (strlen(strOut)>0)
            {
                memset(strTCT.szFunKeyPW,0x00,8);
                memcpy(strTCT.szFunKeyPW,strOut, strlen(strOut));
                strTCT.szFunKeyPW[strlen(strOut)]=0;
                inTCTSave(1);
                break;
            }
            else
            {
                vduiClearBelow(3);
                vduiDisplayStringCenter(4,"NO PASSWORD");
                vduiDisplayStringCenter(5,"ENTERED,DISABLE");
                vduiDisplayStringCenter(6,"ENGINEER PWD?");
                vduiDisplayStringCenter(8,"NO[X] YES[OK]");
                key=WaitKey(60);
                if(key==d_KBD_ENTER)
                {
                    memset(strTCT.szFunKeyPW,0x00,8);
                    inTCTSave(1);
                    break;
                }
            }
        }
        if (ret == d_KBD_CANCEL )
            break ;
	}

    inHDTRead(shHostIndex);
    while(1)
    {    
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "LAST 4 DIGIT");
        if(strHDT.fLast4Digit== 0)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "0");
        else if(strHDT.fLast4Digit == 1)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "1");     
        
        CTOS_LCDTPrintXY(1, 5, "0-DISABLE      1-ENABLE");
        
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 1, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret==1)
        {
            if (strOut[0]==0x30 || strOut[0]==0x31)
            {
                 if(strOut[0] == 0x31)
                 {
                        strHDT.fLast4Digit = 1;
                 }
                 if(strOut[0] == 0x30)
                 {
                        strHDT.fLast4Digit = 0;
                 }

                 inHDTSave(shHostIndex);
                 break;
             }
             else
             {
                vduiWarningSound();
                vduiDisplayStringCenter(7,"PLEASE SELECT");
                vduiDisplayStringCenter(8,"A VALID INPUT");
                CTOS_Delay(2000);       
            }
        }
		
        if (ret == d_KBD_CANCEL )
            break ;
    }

	inHDTRead(shHostIndex);
    while(1)
    {    
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "Tip Allowed");
        if(strHDT.fTipAllowFlag== 0)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "0");
        else if(strHDT.fTipAllowFlag == 1)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "1");     
        
        CTOS_LCDTPrintXY(1, 5, "0-DISABLE      1-ENABLE");
        
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 1, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret==1)
        {
            if (strOut[0]==0x30 || strOut[0]==0x31)
            {
                 if(strOut[0] == 0x31)
                 {
                        strHDT.fTipAllowFlag= 1;
                 }
                 if(strOut[0] == 0x30)
                 {
                        strHDT.fTipAllowFlag = 0;
                 }

                 inHDTSave(shHostIndex);
                 break;
             }
             else
             {
                vduiWarningSound();
                vduiDisplayStringCenter(7,"PLEASE SELECT");
                vduiDisplayStringCenter(8,"A VALID INPUT");
                CTOS_Delay(2000);       
            }
        }
		
        if (ret == d_KBD_CANCEL )
            break ;
    }

inHDTRead(shHostIndex);
    while(1)
    {
    	char temp[3+1];
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "TIP PERCENT");

	    memset(temp,0x00,sizeof(temp));
		sprintf(temp, "%ld", (int)strHDT.ulTipPercent);
        setLCDPrint(4, DISPLAY_POSITION_LEFT, temp);
        
        strcpy(strtemp,"New:");
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 3, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret>=1)
        {
            //if ((strlen(strOut)>0) && (atol(strOut)<=100)) // original code
			if (strlen(strOut)>0)	 // MCC Change Request -- sidumili
            {
               ULONG ulTipPercent=0;
               strHDT.ulTipPercent = 0;
               strHDT.ulTipPercent = atol(strOut);
               inHDTSave(shHostIndex);
               break;
            }
            else
            {
                vduiClearBelow(3);
                vduiDisplayStringCenter(7,"PLEASE INPUT");
                vduiDisplayStringCenter(8,"A VALID VALUE");
                key=WaitKey(60);
                if(key==d_KBD_ENTER)
                {
                    strHDT.ulTipPercent = 0;
                    inHDTSave(shHostIndex);
                    continue;
                }
            }
        }
    if (ret == d_KBD_CANCEL )
        break ;
    }

    inHDTRead(shHostIndex);
    while(1)
    {    
        char temp[3+1];		
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "DELETE REV RETRY");
		
	    memset(temp,0x00,sizeof(temp));
		sprintf(temp, "%ld", (int)strHDT.inDeleteREV);
        setLCDPrint(4, DISPLAY_POSITION_LEFT, temp);    
        
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 1, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret==1)
        {
            strHDT.inDeleteREV=atoi(strOut);
            inHDTSave(shHostIndex);
            break;
        }
		
        if (ret == d_KBD_CANCEL )
            break ;
    }
    inHDTRead(shHostIndex);

		
	return;
}

//void vdCTOS_DeleteBatch(void)
int vdCTOS_DeleteBatch(void)
{
    int         shHostIndex = 1;
    int         inResult,inRet;
    ACCUM_REC srAccumRec;
    STRUCT_FILE_SETTING strFile;

    vdDebug_LogPrintf("--vdCTOS_DeleteBatch--");
	vdDebug_LogPrintf("byERMMode[%d]", strTCT.byERMMode);
	
    vduiLightOn();                

    shHostIndex = inCTOS_SelectHostSetting();
    if (shHostIndex == -1)
        return -1;
	else if(shHostIndex == -2)
        return -2;

   //CTOS_LCDTClearDisplay();
	
    if (inMultiAP_CheckMainAPStatus() == d_OK)
    {
        inRet = inCTOS_MultiAPSaveData(d_IPC_CMD_DELETE_BATCH);
        if(d_OK != inRet)
            return -1;
    }
    else
    {
        if (inMultiAP_CheckSubAPStatus() == d_OK)
        {
            inRet = inCTOS_MultiAPGetData();
            if(d_OK != inRet)
                return -1;

            inRet = inCTOS_MultiAPReloadHost();
            if(d_OK != inRet)
                return -1;
        }
    }

    inResult = inCTOS_CheckAndSelectMutipleMID();
    if(d_OK != inResult)
        return -1;
          
    //inResult = vduiAskConfirmContinue();
    inResult=vduiAskConfirmation("DELETE BATCH?");
	vdDebug_LogPrintf("inResult[%d]", inResult);
    if(inResult == d_OK)
    {
        char szBcd[INVOICE_BCD_SIZE+1];
		
        if(CN_TRUE == strMMT[0].fMustSettFlag)
        {
            strMMT[0].fMustSettFlag = CN_FALSE;
            inMMTSave(strMMT[0].MMTid);
        }

        memset(szBcd, 0x00, sizeof(szBcd));
        memcpy(szBcd, strMMT[0].szBatchNo, INVOICE_BCD_SIZE);    
        inBcdAddOne(szBcd, strMMT[0].szBatchNo, INVOICE_BCD_SIZE);
        strMMT[0].fMustSettFlag = CN_FALSE;
        inMMTSave(strMMT[0].MMTid);

		strMMT[0].fBatchNotEmpty = 0;	
		inMMTBatchNotEmptySave(strMMT[0].MMTid);	   

		// Check for batch empty
		inRet = inCTOS_ChkBatchEmpty();
		if(d_OK != inRet)
			return -1;		
	
        // delete batch where hostid and mmtid is matcj
        inDatabase_BatchDelete();

        memset(&srAccumRec, 0x00, sizeof(ACCUM_REC));
        memset(&strFile,0,sizeof(strFile));
        vdCTOS_GetAccumName(&strFile, &srAccumRec);
    
        if((inResult = CTOS_FileDelete(strFile.szFileName)) != d_OK)
        {
            vdDebug_LogPrintf("[inMyFile_SettleRecordDelete]---Delete Record error[%04x]", inResult);
        }
        
        inCTOS_DeleteBKAccumTotal(&srAccumRec,strHDT.inHostIndex,srTransRec.MITid);

        inDatabase_InvoiceNumDelete(srTransRec.HDTid, srTransRec.MITid);
		
        inMyFile_ReversalDelete();

        inMyFile_AdviceDelete();
        
        inMyFile_TCUploadDelete();

		// For ERM
		if (strTCT.byERMMode != 0)
		{
			vdLinuxCommandClearERMBitmap();
			vdClearERMBatch(FALSE);
		}
        			
       //CTOS_LCDTClearDisplay();
		if (isCheckTerminalMP200() == d_OK || isCheckTerminalNonTouch() == d_OK)
		{
			//setLCDPrint27(5, d_LCD_ALIGNCENTER, "CLEAR BATCH");
			//setLCDPrint27(6, d_LCD_ALIGNCENTER, "RECORD DONE");
			
			vdDisplayMessageBox(1, 8, "CLEAR BATCH", "RECORD DONE", "", MSG_TYPE_INFO);
		}
		else
		{
	        //setLCDPrint(5, DISPLAY_POSITION_CENTER, "CLEAR BATCH");
	        //setLCDPrint(6, DISPLAY_POSITION_CENTER, "RECORD DONE");
			
			vdDisplayMessageBox(1, 8, "CLEAR BATCH", "RECORD DONE", "", MSG_TYPE_INFO);
		}
		
        CTOS_Delay(1000); 
		return d_OK;
    } 
	return -1;
}

int vdCTOS_DeleteReversal(void)
{
    CHAR szFileName[d_BUFF_SIZE];
    int         shHostIndex = 1;
    int         inResult,inRet;
    ACCUM_REC srAccumRec;
    STRUCT_FILE_SETTING strFile;

    
    vduiLightOn();                

    shHostIndex = inCTOS_SelectHostSetting();
    if (shHostIndex == -1)
        return -1;
	else if(shHostIndex == -2)
        return -2;
	

   //CTOS_LCDTClearDisplay();
		
    if (inMultiAP_CheckMainAPStatus() == d_OK)
    {
        inRet = inCTOS_MultiAPSaveData(d_IPC_CMD_DELETE_REVERSAL);
        if(d_OK != inRet)
            return d_NO;
    }
    else
    {
        if (inMultiAP_CheckSubAPStatus() == d_OK)
        {
            inRet = inCTOS_MultiAPGetData();
            if(d_OK != inRet)
                return d_NO;

            inRet = inCTOS_MultiAPReloadHost();
            if(d_OK != inRet)
                return d_NO;
        }
    }

    inResult = inCTOS_CheckAndSelectMutipleMID();    
    if(d_OK != inResult)
        return d_NO;

    memset(szFileName,0,sizeof(szFileName));
    sprintf(szFileName, "%s%02d%02d.rev"
                    , strHDT.szHostLabel
                    , strHDT.inHostIndex
                    , srTransRec.MITid);

    vdDebug_LogPrintf("Rever Name %s",szFileName);
    
    if((inResult = inMyFile_CheckFileExist(szFileName)) < 0)
    {
        vdDebug_LogPrintf("inMyFile_CheckFileExist <0");
        vduiClearBelow(2);
        //vdDisplayErrorMsg(1, 8, "NO REVERSAL EXIST");
          /*strcpy(szDisMsg, "CLEAR REVERSAL");
          strcat(szDisMsg, "|");
          strcat(szDisMsg, "NO REVERSAL EXISTS");
          usCTOSS_LCDDisplay(szDisMsg);*/

	vdDisplayMessageBox(1, 8, "", "NO REVERSAL EXIST", "", MSG_TYPE_INFO);
	CTOS_Beep();
	CTOS_Delay(1000);
		return d_NO;
        
    }
            
    //inResult = vduiAskConfirmContinue();    
    inResult=vduiAskConfirmation("CLEAR REVERSAL?");
	vdDebug_LogPrintf("inResult[%d]", inResult);
    if(inResult == d_OK)
    {
        inMyFile_ReversalDelete();
    
        //CTOS_LCDTClearDisplay();
		//strcpy(szDisMsg, "CLEAR REVERSAL");
		//strcat(szDisMsg, "|");
		//strcat(szDisMsg, "RECORD DONE");
		//usCTOSS_LCDDisplay(szDisMsg);	
		
		vdDisplayMessageBox(1, 8, "CLEAR REVERSAL", "RECORD DONE", "", MSG_TYPE_INFO);
		CTOS_Beep();
		CTOS_Delay(1000);
    }   

	return d_OK;
}

void vdCTOS_PrintEMVTerminalConfig(void)
{
   //CTOS_LCDTClearDisplay();
    vdPrintEMVTags();
    
    return;
}

void vdCTOS_Debugmode(void)
{
    BYTE bRet;
    BYTE szInputBuf[15+1];
    int inResult,inResult1;
    TRANS_TOTAL stBankTotal;
    BYTE strOut[30],strtemp[17],key;
    USHORT ret;
    USHORT usLen;
    BYTE szTempBuf[12+1];
    BOOL isKey;
    int shHostIndex = 1;
    int inNum = 0;
    int inRet = 0;

    inRet = inTCTRead(1);  
    vdDebug_LogPrintf(". inTCTRead(%d)",inRet);

   //CTOS_LCDTClearDisplay();
    vdDispTitleString("DEBUG MODE");
    while(1)
    {
        clearLine(3);
        clearLine(4);
        clearLine(5);
        clearLine(6);
        clearLine(7);
        clearLine(8);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "DEBUG MODE");
        if(strTCT.byRS232DebugPort == 0)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "0");
        if(strTCT.byRS232DebugPort == 1)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "1");        
        if(strTCT.byRS232DebugPort == 2)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "2");
        if(strTCT.byRS232DebugPort == 8)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "8");  
        
        CTOS_LCDTPrintXY(1, 5, "0-NO        1-COM1");
        CTOS_LCDTPrintXY(1, 6, "2-COM2    8-USB");
        
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 1, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret==1)
        {
            if (strOut[0]==0x30 || strOut[0]==0x31 || strOut[0]==0x32 || strOut[0]==0x38)
            {
                 if(strOut[0] == 0x31)
                 {
                        strTCT.byRS232DebugPort = 1;
                 }
                 if(strOut[0] == 0x30)
                 {
                        strTCT.byRS232DebugPort = 0;
                 }
                 if(strOut[0] == 0x32)
                 {
                        strTCT.byRS232DebugPort = 2;
                 }
                 if(strOut[0] == 0x38)
                 {
                        strTCT.byRS232DebugPort = 8;
                 }
                
                 inRet = inTCTSave(1);
                 
                 vdDebug_LogPrintf(". inTCTSave(%d)",inRet);
                 break;
             }
             else
             {
                vduiWarningSound();
                vduiDisplayStringCenter(6,"PLEASE SELECT");
                vduiDisplayStringCenter(7,"A VALID");
                vduiDisplayStringCenter(8,"DEBUG MODE");
                CTOS_Delay(2000);       
            }
        }
        if (ret == d_KBD_CANCEL )
            break ;
    }
       
    return ;
}
#if 0
void vdCTOSS_InjectMKKey(void)
{
	BYTE bInBuf[250];
	BYTE bOutBuf[250];
	BYTE *ptr = NULL;
	USHORT usInLen = 0;
	USHORT usOutLen = 0;
	USHORT usResult;


	CTOS_LCDTClearDisplay();
	CTOS_LCDTPrintXY(1, 1, "INJECT MK");

	memset(bOutBuf, 0x00, sizeof(bOutBuf));
	memset(bInBuf, 0x00, sizeof(bInBuf));
	
	vdDebug_LogPrintf("saturn d_IPC_CMD_EMV_INJECTMK usInLen[%d] ",usInLen);
	
	usResult = inMultiAP_RunIPCCmdTypes("com.Source.SHARLS_EMV.SHARLS_EMV", d_IPC_CMD_EMV_INJECTMK, bInBuf, usInLen, bOutBuf, &usOutLen);

    if (usResult != d_OK)
      vdDisplayErrorMsg(1, 8, "Save MK Error");
	else
	  vdDisplayErrorMsg(1, 8, "Save MK OK");

			
	CTOS_LCDTPrintXY(1, 1, "INJECT MK");
	vdDebug_LogPrintf("saturn d_IPC_CMD_EMV_INJECTMK usOutLen[%d] ",usOutLen);
	DebugAddHEX("MK",bOutBuf,usOutLen);
	
	inTCTRead(1);
	vdDebug_LogPrintf("saturn InjectMKKey..byPinPadType=[%d],byPinPadPort=[%d]",strTCT.byPinPadType,strTCT.byPinPadPort);
	if (1 == strTCT.byPinPadType)
	{
		if (usOutLen == 8)
			inPCI100_SaveMKProcess(MK_DES_KEY,usOutLen,bOutBuf);
		if (usOutLen == 16)
			inPCI100_SaveMKProcess(MK_3DES_DOUBLE_KEY,usOutLen,bOutBuf);
		if (usOutLen == 24)
			inPCI100_SaveMKProcess(MK_3DES_TRIPLE_KEY,usOutLen,bOutBuf);

		if (usResult != d_OK)
		  vdDisplayErrorMsg(1, 8, "Save MK Error");
		else
		  vdDisplayErrorMsg(1, 8, "Save MK OK");	    
	}

	return;
}
#else
void vdCTOSS_InjectMKKey(void)
{
	BYTE bInBuf[250];
	BYTE bOutBuf[250];
	BYTE *ptr = NULL;
	USHORT usInLen = 0;
	USHORT usOutLen = 0;
	USHORT usResult;
	 USHORT usKeySet;
	USHORT usKeyIndex;

	BYTE bHeaderAttr = 0x01+0x04, iCol = 1;
  BYTE	x = 1;
  BYTE key;
	char szHeaderString[50] = "SELECT TO LOAD KEY";
	char szInjectKeyMenu[1024] = {0};


	CTOS_LCDTClearDisplay();
	CTOS_LCDTPrintXY(1, 1, "INJECT MK");
	put_env_int("KEYSET",0);
	put_env_int("KEYINDEX",0);

	/*********************************************/
	//sidumili: [Select Pinpad Type for Injection]

	memset(szInjectKeyMenu, 0x00, sizeof(szInjectKeyMenu));
	strcpy((char*)szInjectKeyMenu, "IPP \nPCi100 \nIPP/PCi100 \nHKLM DUKPT \nCANCEL");
	key = MenuDisplay(szHeaderString, strlen(szHeaderString), bHeaderAttr, iCol, x, szInjectKeyMenu, TRUE);

	if (key == 0xFF) 
	{	
		CTOS_LCDTClearDisplay();
		setLCDPrint(1, DISPLAY_POSITION_CENTER, "WRONG INPUT!!!");
		vduiWarningSound();
		return; 
	}

	if (key > 0){
		
		if(d_KBD_CANCEL == key)
		return;

		memset(bOutBuf, 0x00, sizeof(bOutBuf));
		memset(bInBuf, 0x00, sizeof(bInBuf));

		// --->> IPP
		if (key == 1){
	
			// --- Inject to IPP
			CTOS_LCDTClearDisplay();
			CTOS_LCDTPrintXY(1, 1, "INJECT MK");
		
			usResult = inMultiAP_RunIPCCmdTypes("SHARLS_EMV", d_IPC_CMD_EMV_INJECTMK, bInBuf, usInLen, bOutBuf, &usOutLen);

			// -- sidumili: debugging of key injected
			//vdPrintPCIDebug_HexPrintf(TRUE, "INJECT IN", bInBuf, usInLen);
			//vdPrintPCIDebug_HexPrintf(TRUE, "INJECT OUT", bOutBuf, usOutLen);
			
			if (usResult != d_OK)		
				vdDisplayErrorMsg(1, 8, "Save MK error");
			else
				vdDisplayErrorMsg(1, 8, "Save MK OK");
	
			return ;
		}
			
		// --->> PCi100 / BOTH
		if ((key == 2) || (key == 3)){
			
			// --- Inject to IPP
			CTOS_LCDTClearDisplay();
			CTOS_LCDTPrintXY(1, 1, "INJECT MK");
		
			usResult = inMultiAP_RunIPCCmdTypes("SHARLS_EMV", d_IPC_CMD_EMV_INJECTMK, bInBuf, usInLen, bOutBuf, &usOutLen);

			// -- sidumili: debugging of key injected
			//vdPrintPCIDebug_HexPrintf(TRUE, "INJECT IN", bInBuf, usInLen);
			//vdPrintPCIDebug_HexPrintf(TRUE, "INJECT OUT", bOutBuf, usOutLen);
			
			if (usResult != d_OK)
			return ;

			// --- Inject to PCi100
			CTOS_LCDTClearDisplay();
			CTOS_LCDTPrintXY(1, 1, "INJECT MK");
 			usKeySet = get_env_int("KEYSET");
			usKeyIndex = get_env_int("KEYINDEX");
			vdDebug_LogPrintf("usKeySet=[%x]",usKeySet);
			vdDebug_LogPrintf("usKeyIndex=[%x]",usKeyIndex);
			
			if (usOutLen == 8)
			usResult = inPCI100_SaveMKProcess(MK_DES_KEY,usOutLen,bOutBuf);
			if (usOutLen == 16)
			usResult = inPCI100_SaveMKProcess(MK_3DES_DOUBLE_KEY,usOutLen,bOutBuf);
			if (usOutLen == 24)
			usResult = inPCI100_SaveMKProcess(MK_3DES_TRIPLE_KEY,usOutLen,bOutBuf);

			if (usResult != d_OK)
			return ;

		}
		if (key == 4){
			inCTOSS_HKLMInjectDUKPTKey(0xc000,0x8888);
			return;
		}

		// --->> CANCEL
		if (key == 5){
			vdDisplayErrorMsg(1, 8, "Inject cancelled");
			return;
		}
	}
	/*********************************************/

	
#if 0
	memset(bOutBuf, 0x00, sizeof(bOutBuf));
	memset(bInBuf, 0x00, sizeof(bInBuf));
	
	vdDebug_LogPrintf("d_IPC_CMD_EMV_INJECTMK usInLen[%d] ",usInLen);
	
	usResult = inMultiAP_RunIPCCmdTypes("SHARLS_EMV", d_IPC_CMD_EMV_INJECTMK, bInBuf, usInLen, bOutBuf, &usOutLen);

	if (usResult != d_OK)
		return ;

	CTOS_LCDTPrintXY(1, 1, "INJECT MK");
	vdDebug_LogPrintf("d_IPC_CMD_EMV_INJECTMK usOutLen[%d] ",usOutLen);
	DebugAddHEX("MK",bOutBuf,usOutLen);
	
	inTCTRead(1);
	vdDebug_LogPrintf("InjectMKKey..byPinPadType=[%d],byPinPadPort=[%d]",strTCT.byPinPadType,strTCT.byPinPadPort);
	if (1 == strTCT.byPinPadType)
	{
		if (usOutLen == 8)
			inPCI100_SaveMKProcess(MK_DES_KEY,usOutLen,bOutBuf);
		if (usOutLen == 16)
			inPCI100_SaveMKProcess(MK_3DES_DOUBLE_KEY,usOutLen,bOutBuf);
		if (usOutLen == 24)
			inPCI100_SaveMKProcess(MK_3DES_TRIPLE_KEY,usOutLen,bOutBuf);
	}
#endif

	return;
}

#endif



void vdCTOSS_SelectPinpadType(void)
{
    BYTE bRet;
    BYTE szInputBuf[15+1];
    int inResult,inResult1;
    TRANS_TOTAL stBankTotal;
    BYTE strOut[30],strtemp[17],key;
    USHORT ret;
    USHORT usLen;
    BYTE szTempBuf[12+1];
    BOOL isKey;
    int shHostIndex = 1;
    int inNum = 0;
    int inRet = 0;

    inRet = inTCTRead(1);  
    vdDebug_LogPrintf(". inTCTRead(%d)",inRet);

   //CTOS_LCDTClearDisplay();
    vdDispTitleString("SETTING");
    while(1)
    {
        clearLine(3);
        clearLine(4);
        clearLine(5);
        clearLine(6);
        clearLine(7);
        clearLine(8);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "PINPAD TYPE");
        if(strTCT.byPinPadType == 0)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "0");
        if(strTCT.byPinPadType == 1)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "1");        
        if(strTCT.byPinPadType == 2)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "2");
  
        
        CTOS_LCDTPrintXY(1, 5, "0-None	1-PCI100");
        CTOS_LCDTPrintXY(1, 6, "2-OTHER    ");
        
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 1, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret==1)
        {
            if (strOut[0]==0x30 || strOut[0]==0x31 || strOut[0]==0x32)
            {
                 if(strOut[0] == 0x31)
                 {
                        strTCT.byPinPadType = 1;
                 }
                 if(strOut[0] == 0x30)
                 {
                        strTCT.byPinPadType = 0;
                 }
                 if(strOut[0] == 0x32)
                 {
                        strTCT.byPinPadType = 2;
                 }
 
                
                 inRet = inTCTSave(1);
                 
                 vdDebug_LogPrintf(". inTCTSave(%d)",inRet);
                 break;
             }
             else
             {
                vduiWarningSound();
                vduiDisplayStringCenter(6,"PLEASE SELECT");
                vduiDisplayStringCenter(7,"A VALID");
                vduiDisplayStringCenter(8,"DEBUG MODE");
                CTOS_Delay(2000);       
            }
        }
        if (ret == d_KBD_CANCEL )
            break ;
    }
       
    return ;
}

int inCTOSS_GetCtlsMode(void)
{
	//inTCTRead(1);

	return strTCT.byCtlsMode;
}

void vdCTOSS_CtlsMode(void)
{
    BYTE bRet;
    BYTE szInputBuf[15+1];
    int inResult,inResult1;
    TRANS_TOTAL stBankTotal;
    BYTE strOut[30],strtemp[17],key;
    USHORT ret;
    USHORT usLen;
    BYTE szTempBuf[12+1];
    BOOL isKey;
    int shHostIndex = 1;
    int inNum = 0;
    int inRet = 0;

    inRet = inTCTRead(1);  
    vdDebug_LogPrintf(". inTCTRead(%d)",inRet);

   //CTOS_LCDTClearDisplay();
    vdDispTitleString("SETTING");
    while(1)
    {
        clearLine(3);
        clearLine(4);
        clearLine(5);
        clearLine(6);
        clearLine(7);
        clearLine(8);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "CTLS MODE");
        if(strTCT.byCtlsMode == 0)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "0");
        if(strTCT.byCtlsMode == 1)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "1");        
        if(strTCT.byCtlsMode == 2)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "2");
		if(strTCT.byCtlsMode == 3)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "3");
 
        
        CTOS_LCDTPrintXY(1, 5, "0-Disable 1-Internal");
        CTOS_LCDTPrintXY(1, 6, "2-External 3-V3 CTLS");
        
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 1, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret==1)
        {
            if (strOut[0]==0x30 || strOut[0]==0x31 || strOut[0]==0x32 || strOut[0]==0x33)
            {
                 if(strOut[0] == 0x31)
                 {
                        strTCT.byCtlsMode = 1;
                 }
                 if(strOut[0] == 0x30)
                 {
                        strTCT.byCtlsMode = 0;
                 }
                 if(strOut[0] == 0x32)
                 {
                        strTCT.byCtlsMode = 2;
                 }
				 if(strOut[0] == 0x33)
                 {
                        strTCT.byCtlsMode = 3;
                 }
                
                 inRet = inTCTSave(1);
                 
                 vdDebug_LogPrintf(". inTCTSave(%d)",inRet);
                 break;
             }
             else
             {
                vduiWarningSound();
                vduiDisplayStringCenter(6,"PLEASE SELECT");
                vduiDisplayStringCenter(7,"A VALID");
                vduiDisplayStringCenter(8,"DEBUG MODE");
                CTOS_Delay(2000);       
            }
        }
        if (ret == d_KBD_CANCEL )
            break ;
    }
       
    return ;
}


void vdCTOS_DemoMode(void)
{
    BYTE bRet;
    BYTE szInputBuf[15+1];
    int inResult,inResult1;
    TRANS_TOTAL stBankTotal;
    BYTE strOut[30],strtemp[17],key;
    USHORT ret;
    USHORT usLen;
    BYTE szTempBuf[12+1];
    BOOL isKey;
    int shHostIndex = 1;
    int inNum = 0;
    int inNumOfHost = 0;
    int inNumOfMerchant = 0;
    int inLoop =0 ;
    //int inResult = 0;
    ACCUM_REC srAccumRec;
    STRUCT_FILE_SETTING strFile;

    inTCTRead(1);   
    
   //CTOS_LCDTClearDisplay();
    vdDispTitleString("DEMO MODE");
    while(1)
    {
        clearLine(3);
        clearLine(4);
        clearLine(5);
        clearLine(6);
        clearLine(7);
        clearLine(8);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "DEMO MODE");
        if(strTCT.fDemo == 0)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "0");
        if(strTCT.fDemo == 1)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "1");     
        
        CTOS_LCDTPrintXY(1, 5, "0-DISABLE      1-ENABLE");
        
   
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 1, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret==1)
        {
            //check host num
            inNumOfHost = inHDTNumRecord();
            
            vdDebug_LogPrintf("[inNumOfHost]-[%d]", inNumOfHost);
            for(inNum =1 ;inNum <= inNumOfHost; inNum++)
            {
                if(inHDTRead(inNum) == d_OK)
                {
                    inMMTReadNumofRecords(strHDT.inHostIndex,&inNumOfMerchant);
                
                    vdDebug_LogPrintf("[inNumOfMerchant]-[%d]strHDT.inHostIndex[%d]", inNumOfMerchant,strHDT.inHostIndex);
                    for(inLoop=1; inLoop <= inNumOfMerchant;inLoop++)
                    {
                        if((inResult = inMMTReadRecord(strHDT.inHostIndex,strMMT[inLoop-1].MITid)) !=d_OK)
                        {
                            vdDebug_LogPrintf("[read MMT fail]-Mitid[%d]strHDT.inHostIndex[%d]inResult[%d]", strMMT[inLoop-1].MITid,strHDT.inHostIndex,inResult);
                            continue;
                            //break;
                        }
                        else    // delete batch where hostid and mmtid is match  
                        {
                            strMMT[0].HDTid = strHDT.inHostIndex;
                            strMMT[0].MITid = strMMT[inLoop-1].MITid;
                            inDatabase_BatchDeleteHDTidMITid();
                            vdDebug_LogPrintf("[inDatabase_BatchDelete]-Mitid[%d]strHDT.inHostIndex[%d]", strMMT[inLoop-1].MITid,strHDT.inHostIndex);
                            
                            memset(&srAccumRec, 0x00, sizeof(ACCUM_REC));
                            memset(&strFile,0,sizeof(strFile));
                            srTransRec.MITid = strMMT[inLoop-1].MITid;
                            vdCTOS_GetAccumName(&strFile, &srAccumRec);
                            
                            if((inResult = CTOS_FileDelete(strFile.szFileName)) != d_OK)
                            {
                                vdDebug_LogPrintf("[inMyFile_SettleRecordDelete]---Delete Record error[%04x]", inResult);
                            }

                            inCTOS_DeleteBKAccumTotal(&srAccumRec,strHDT.inHostIndex,srTransRec.MITid);

                            inDatabase_InvoiceNumDelete(srTransRec.HDTid, srTransRec.MITid);
							
                            inMyFile_ReversalDelete();

                            inMyFile_AdviceDelete();
                            
                            inMyFile_TCUploadDelete();

                        }
                    }
                }
                else
                    continue;

            }
            
            if (strOut[0]==0x30 || strOut[0]==0x31)
            {
                 if(strOut[0] == 0x31)
                 {
                        strTCT.fDemo = 1;
                 }
                 if(strOut[0] == 0x30)
                 {
                        strTCT.fDemo = 0;
                 }
                 inTCTSave(1);
                 break;
             }
             else
             {
                vduiWarningSound();
                vduiDisplayStringCenter(6,"PLEASE SELECT");
                vduiDisplayStringCenter(7,"A VALID");
                vduiDisplayStringCenter(8,"DEBUG MODE");
                CTOS_Delay(2000);       
            }
        }
        if (ret == d_KBD_CANCEL )
            break ;
    }
       
    return ;
}


void vdCTOS_TipAllowd(void)
{
    BYTE bRet;
    BYTE szInputBuf[15+1];
    int inResult,inResult1;
    TRANS_TOTAL stBankTotal;
    BYTE strOut[30],strtemp[17],key;
    USHORT ret;
    USHORT usLen;
    BYTE szTempBuf[12+1];
    BOOL isKey;
    int shHostIndex = 1;
    int inNum = 0;

    inTCTRead(1);    
    
   //CTOS_LCDTClearDisplay();
    vdDispTitleString("SETTING");
    
    while(1)
    {
        clearLine(3);
        clearLine(4);
        clearLine(5);
        clearLine(6);
        clearLine(7);
        clearLine(8);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "Tip Allowed");
        if(strTCT.fTipAllowFlag == 0)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "0");
        if(strTCT.fTipAllowFlag == 1)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "1");     
        
        CTOS_LCDTPrintXY(1, 5, "0-DISABLE      1-ENABLE");
        
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 1, 0, d_INPUT_TIMEOUT);

		if(inCTOS_ChkBatchEmpty() == d_OK)
		{
            vdDisplayErrorMsg(1, 8, "BATCH NOT EMPTY");	
			break;
		}
		
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret==1)
        {
            if (strOut[0]==0x30 || strOut[0]==0x31)
            {
                 if(strOut[0] == 0x31)
                 {
                        strTCT.fTipAllowFlag = 1;
                 }
                 if(strOut[0] == 0x30)
                 {
                        strTCT.fTipAllowFlag = 0;
                 }

                 inTCTSave(1);
                 break;
             }
             else
             {
                vduiWarningSound();
                vduiDisplayStringCenter(6,"PLEASE SELECT");
                vduiDisplayStringCenter(7,"A VALID");
                vduiDisplayStringCenter(8,"DEBUG MODE");
                CTOS_Delay(2000);       
            }
        }
        if (ret == d_KBD_CANCEL )
            break ;
    }
       
    return ;
}



void vdCTOS_GPRSSetting(void)
{
    BYTE bRet,strOut[30],strtemp[17],key;
    BYTE szInputBuf[24+1];
    BYTE szIntComBuf[2];
    BYTE szPhNoBuf[9];
    BYTE szExtNoBuf[4];
    int inResult;
    USHORT ret;
    short inRtn;
    USHORT shMaxLen=0;
    
    inResult = inTCPRead(1);
    if (inResult != d_OK)
        return;
        
   //CTOS_LCDTClearDisplay();
    vdDispTitleString("GPRS SETUP");
    
    while(1)
    {
        clearLine(3);
        clearLine(4);
        clearLine(5);
        clearLine(6);
        clearLine(7);
        clearLine(8);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "APN");
        setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCP.szAPN);
        if(strlen(strTCP.szAPN) > 19)
		    setLCDPrint(5, DISPLAY_POSITION_LEFT, &strTCP.szAPN[19]);
		
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        shMaxLen=30;
        //ret= InputStringAlpha(1, 8, 0x00, 0x02, strOut, &shMaxLen, 1, d_INPUT_TIMEOUT);
        ret= InputStringAlpha2(1, 8, 0x04, 0x02, strOut, &shMaxLen, 0, d_INPUT_TIMEOUT);
        if(ret==d_KBD_ENTER)
        {
            if(strlen(strOut) <= 0)
                strcpy(strOut,""); 
            BolDetachGPRSChangeSetting=TRUE;
            memcpy(strTCP.szAPN, strOut,strlen(strOut));
            strTCP.szAPN[strlen(strOut)]=0;
            inResult = inTCPSave(1);
            break;
        }   
        if(ret == d_KBD_CANCEL)
            break;
    }               

    while(1)
    {
        clearLine(3);
        clearLine(4);
        clearLine(5);
        clearLine(6);
        clearLine(7);
        clearLine(8);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "GPRS USER NAME");
        setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCP.szUserName);

        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        shMaxLen=30;
        //ret= InputStringAlpha(1, 8, 0x00, 0x02, strOut, &shMaxLen, 0, d_INPUT_TIMEOUT);
        ret= InputStringAlpha2(1, 8, 0x04, 0x02, strOut, &shMaxLen, 0, d_INPUT_TIMEOUT);
        if(ret==d_KBD_ENTER)
        {
			if(strlen(strOut) <= 0)
			    strcpy(strOut,"");    	
            BolDetachGPRSChangeSetting=TRUE;
            memcpy(strTCP.szUserName, strOut,strlen(strOut));
            strTCP.szUserName[strlen(strOut)]=0;
            inResult = inTCPSave(1);
            break;
        }   
        if(ret == d_KBD_CANCEL)
            break;
    }               

    while(1)
    {
        clearLine(3);
        clearLine(4);
        clearLine(5);
        clearLine(6);
        clearLine(7);
        clearLine(8);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "GPRS PASSWORD");
        setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCP.szPassword);

        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
		shMaxLen=30;
        //ret= InputStringAlpha(1, 8, 0x00, 0x02, strOut, &shMaxLen, 0, d_INPUT_TIMEOUT);
        ret= InputStringAlpha2(1, 8, 0x04, 0x02, strOut, &shMaxLen, 0, d_INPUT_TIMEOUT);
        if(ret==d_KBD_ENTER)
        {
			if(strlen(strOut) <= 0)
			    strcpy(strOut,"");
			BolDetachGPRSChangeSetting=TRUE;
            memcpy(strTCP.szPassword, strOut,strlen(strOut));
            strTCP.szPassword[strlen(strOut)]=0;
            inResult = inTCPSave(1);
            break;
        }   
        if(ret == d_KBD_CANCEL)
            break;
    }   

    if(BolDetachGPRSChangeSetting==TRUE)
    {
            srTransRec.usTerminalCommunicationMode = strCPT.inCommunicationMode;
            clearLine(3);
            clearLine(4);
            clearLine(5);
            clearLine(6);
            clearLine(7);
            clearLine(8);
            CTOS_LCDTPrintXY (1,7, "Please Wait     ");       
            CTOS_LCDTPrintXY(1, 8, "Init GPRS...     ");
            if (inCTOS_InitComm(srTransRec.usTerminalCommunicationMode) != d_OK) 
            {
               
                vdDisplayErrorMsg(1, 8, "COMM INIT ERR");
                return;
            }
                          
            srCommFuncPoint.inInitComm(&srTransRec,GPRS_MODE);
            srCommFuncPoint.inGetCommConfig(&srTransRec);
            srCommFuncPoint.inSetCommConfig(&srTransRec);

    }

    return ;
}

/* delete a Char in string */
void DelCharInStr(char *str, char c, int flag)
{
    int i,l;

    l=strlen(str);

    if (flag == STR_HEAD)
    {
        for (i=0;i<l&&str[i]==c;i++);
        if (i>0) strcpy(str,str+i);
    }

    if (flag == STR_BOTTOM)
    {
        for (i=l-1;i>=0&&str[i]==c;i--);
        str[i+1]='\0';
    }

    if (flag == STR_ALL)
    {
        for (i=0;i<l;i++)
            if (str[i]==c)
            {
                strcpy(str+i,str+i+1);
                i--;
            }
    }
}



void vdConfigEditAddHeader(void)
{
    USHORT inRet, inSelIndex=7;
    BYTE strBuff[50];
    BYTE strActualValue[50];
    USHORT inResult;
    
    STR *keyboardLayoutEnglish[]={" 0", "qzQZ1", "abcABC2", "defDEF3", "ghiGHI4",
    "jklJKL5", "mnoMNO6", "prsPRS7", "tuvTUV8", "wxyWXY9", ":;/\\|?,.<>_",
    ".!@#$%^&*()-"};
    //numeric keyboard (0123456789) and radix point '.'
    STR *keyboardLayoutNumberWithRadixPoint[]={"0", "1", "2", "3", "4", "5", "6",
    "7", "8", "9", "", "."};
    //numeric keyboard (0123456789) without radix point
    STR *keyboardLayoutNumber[]={"0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
    "", ""};
    
    //Parameter for CTOS_UIKeypad:
    //1. usX: 4 => Cursor horizontal position at 4
    //2. usY: 2 => Cursor vertical position at 2
    //3. *pcaKeyboardLayout[]: keyboardLayoutEnglish => use english keyboard
    //layout defined at the beginning of the program
    //4. ucCursorBlinkInterval: 40 => Cursor blink at 400ms interval
    //5. ucDelayToNextChar: 80 => Move Cursor to next character if user did not
    //press any key within 800ms
    //6. boEnableCursorMove: d_TRUE => Allow user to move cursor by F3 and F4 key
    //7. boOneRadixPointOnly: d_FALSE => Do not check radix point
    //8. ucDigitAfterRadixPoint: 0 => Do not limit digits after radix point
    //9. bPasswordMask: 0 => Do not mask user data.
    //10. caData[]: baBuff => Pointer to store the user data retrieved
    //11. ucDataLen: 9 => baBuff has 9 bytes. User may input 8 bytes max.
    
    inResult = inPCTRead(1);
     
//---------HEADER LINE 1---------------------//
    vduiClearBelow(2);  
    vduiDisplayStringCenter(1, "ADDRESS HEADER");
    CTOS_LCDTPrintXY(1, 2, "OLD HEADER LINE1:           ");
    CTOS_LCDTPrintXY(1, 3, strPCT.szRctHdr1);
    CTOS_LCDTPrintXY(1, 4, &strPCT.szRctHdr1[16]);
    CTOS_LCDTPrintXY(1, 5, &strPCT.szRctHdr1[32]);
    CTOS_LCDTPrintXY(1, 6, "ENTR NEW LINE1:");
    memset(strBuff,0x00, sizeof(strBuff));  
    memset(strActualValue,0x00, sizeof(strActualValue));
    
    inRet=CTOS_UIKeypad(1, 8, keyboardLayoutEnglish, 40, 80, d_TRUE, d_FALSE, 0, 0,strBuff, 16);

    if(inRet==d_OK)
    {
        if (strlen(strBuff)>0)
        {
            memcpy(strActualValue,strBuff,strlen(strBuff));

            if (strlen(strBuff)>=15)
            {
                vduiClearBelow(2);  
                vduiDisplayStringCenter(4,"CONTINUE INPUT");
                vduiDisplayStringCenter(5,"MORE TEXT?");
                vduiDisplayStringCenter(8,"NO[X] YES[OK]");

                while(1)
                {
                    inRet=struiGetchWithTimeOut();
                    if (inRet==d_KBD_ENTER)
                    {
                        vduiClearBelow(2);  
                        CTOS_LCDTPrintXY(1, 6, "ENTR NEW LINE1:");
                        memset(strBuff,0x00, sizeof(strBuff));
                        
                        vduiClearBelow(7);  
                        CTOS_LCDTPrintXY(1, 7, strActualValue);
                        inRet=CTOS_UIKeypad(1, 8, keyboardLayoutEnglish, 40, 80, d_TRUE, d_FALSE, 0, 0,strBuff, 16);
                        if(inRet==d_OK)
                        {
                            if (strlen(strBuff)>0)
                            {
                                memcpy(&strActualValue[15],strBuff,strlen(strBuff));
                                if (strlen(strBuff)>=15)
                                {
                                    vduiClearBelow(2);  
                                    vduiDisplayStringCenter(4,"CONTINUE INPUT");
                                    vduiDisplayStringCenter(5,"MORE TEXT?");
                                    vduiDisplayStringCenter(8,"NO[X] YES[OK]");
                                    inRet=struiGetchWithTimeOut();
                                    if (inRet==d_KBD_ENTER)
                                    {
                                        vduiClearBelow(2);
                                        CTOS_LCDTPrintXY(1, 5, "ENTR NEW LINE1:");
                                        memset(strBuff,0x00, sizeof(strBuff));                                                                  
                                        CTOS_LCDTPrintXY(1, 6, strActualValue);
                                        CTOS_LCDTPrintXY(1, 7, &strActualValue[16]);
                                        inRet=CTOS_UIKeypad(1, 8, keyboardLayoutEnglish, 40, 80, d_TRUE, d_FALSE, 0, 0,strBuff, 16);
                                        if(inRet==d_OK)
                                        {    
                                           memcpy(&strActualValue[30],strBuff,strlen(strBuff));
                                        }
                                        break;
                                    }
                                    else if (inRet==d_KBD_CANCEL)
                                        break;
                                    else
                                            vduiWarningSound();
                                }
                            }
                            else
                                break;
                                                
                        }
                        break;
                    }
                    else if (inRet==d_KBD_CANCEL)
                        break;
                    else
                        vduiWarningSound();
                }
            }
            memset(strPCT.szRctHdr1,0x00, sizeof(strPCT.szRctHdr1));    
            memcpy(strPCT.szRctHdr1,strActualValue,strlen(strActualValue));
            
        }
        else
        {
            vduiClearBelow(3);  
            vduiDisplayStringCenter(4,"NO VALUE ENTERED");
            vduiDisplayStringCenter(5,"SAVE EMPTY?");
            vduiDisplayStringCenter(8,"NO[X] YES[OK]");
            while(1)
            {
                inRet=struiGetchWithTimeOut();
                if (inRet==d_KBD_ENTER)
                {
                    memset(strPCT.szRctHdr1,0x00, sizeof(strPCT.szRctHdr1));    
                    break;
                }
                else if (inRet==d_KBD_CANCEL)
                    break;
                else
                    vduiWarningSound();
            }
        }
    }
        
   
    //---------HEADER LINE 2---------------------//    
    vduiClearBelow(2);  
    vduiDisplayStringCenter(1, "ADDRESS HEADER");
    CTOS_LCDTPrintXY(1, 2, "OLD HEADER LINE2:           ");
    CTOS_LCDTPrintXY(1, 3, strPCT.szRctHdr2);
    CTOS_LCDTPrintXY(1, 4, &strPCT.szRctHdr2[16]);
    CTOS_LCDTPrintXY(1, 5, &strPCT.szRctHdr2[32]);
    CTOS_LCDTPrintXY(1, 6, "ENTR NEW LINE2:");
    memset(strBuff,0x00, sizeof(strBuff));  
    memset(strActualValue,0x00, sizeof(strActualValue));
    
    inRet=CTOS_UIKeypad(1, 8, keyboardLayoutEnglish, 40, 80, d_TRUE, d_FALSE, 0, 0,strBuff, 16);

    if(inRet==d_OK)
    {
        if (strlen(strBuff)>0)
        {
            memcpy(strActualValue,strBuff,strlen(strBuff));

            if (strlen(strBuff)>=15)
            {
                vduiClearBelow(2);  
                vduiDisplayStringCenter(4,"CONTINUE INPUT");
                vduiDisplayStringCenter(5,"MORE TEXT?");
                vduiDisplayStringCenter(8,"NO[X] YES[OK]");

                while(1)
                {
                    inRet=struiGetchWithTimeOut();
                    if (inRet==d_KBD_ENTER)
                    {
                        vduiClearBelow(2);  
                        CTOS_LCDTPrintXY(1, 6, "ENTR NEW LINE2:");
                        memset(strBuff,0x00, sizeof(strBuff));
                        
                        vduiClearBelow(7);  
                        CTOS_LCDTPrintXY(1, 7, strActualValue);
                        inRet=CTOS_UIKeypad(1, 8, keyboardLayoutEnglish, 40, 80, d_TRUE, d_FALSE, 0, 0,strBuff, 16);
                        if(inRet==d_OK)
                        {
                            if (strlen(strBuff)>0)
                            {
                                memcpy(&strActualValue[15],strBuff,strlen(strBuff));
                                if (strlen(strBuff)>=15)
                                {
                                    vduiClearBelow(2);  
                                    vduiDisplayStringCenter(4,"CONTINUE INPUT");
                                    vduiDisplayStringCenter(5,"MORE TEXT?");
                                    vduiDisplayStringCenter(8,"NO[X] YES[OK]");
                                    inRet=struiGetchWithTimeOut();
                                    if (inRet==d_KBD_ENTER)
                                    {
                                        vduiClearBelow(2);
                                        CTOS_LCDTPrintXY(1, 5, "ENTR NEW LINE2:");
                                        memset(strBuff,0x00, sizeof(strBuff));                                                                  
                                        CTOS_LCDTPrintXY(1, 6, strActualValue);
                                        CTOS_LCDTPrintXY(1, 7, &strActualValue[16]);
                                        inRet=CTOS_UIKeypad(1, 8, keyboardLayoutEnglish, 40, 80, d_TRUE, d_FALSE, 0, 0,strBuff, 16);
                                        if(inRet==d_OK)
                                        {    
                                           memcpy(&strActualValue[30],strBuff,strlen(strBuff));
                                        }
                                        break;
                                    }
                                    else if (inRet==d_KBD_CANCEL)
                                        break;
                                    else
                                        vduiWarningSound();
                                }
                            }
                            else
                                break;
                                                
                        }
                        break;
                    }
                    else if (inRet==d_KBD_CANCEL)
                        break;
                    else
                        vduiWarningSound();
                }
            }
            memset(strPCT.szRctHdr2,0x00, sizeof(strPCT.szRctHdr2));    
            memcpy(strPCT.szRctHdr2,strActualValue,strlen(strActualValue));
            
        }
        else
        {
            vduiClearBelow(3);  
            vduiDisplayStringCenter(4,"NO VALUE ENTERED");
            vduiDisplayStringCenter(5,"SAVE EMPTY?");
            vduiDisplayStringCenter(8,"NO[X] YES[OK]");
            while(1)
            {
                inRet=struiGetchWithTimeOut();
                if (inRet==d_KBD_ENTER)
                {
                    memset(strPCT.szRctHdr2,0x00, sizeof(strPCT.szRctHdr2));    
                    break;
                }
                else if (inRet==d_KBD_CANCEL)
                    break;
                else
                    vduiWarningSound();
            }
        }
    }    
        
        
   //---------HEADER LINE 3---------------------//    
    vduiClearBelow(2);  
    vduiDisplayStringCenter(1, "ADDRESS HEADER");
    CTOS_LCDTPrintXY(1, 2, "OLD HEADER LINE3:           ");
    CTOS_LCDTPrintXY(1, 3, strPCT.szRctHdr3);
    CTOS_LCDTPrintXY(1, 4, &strPCT.szRctHdr3[16]);
    CTOS_LCDTPrintXY(1, 5, &strPCT.szRctHdr3[32]);
    CTOS_LCDTPrintXY(1, 6, "ENTR NEW LINE3:");
    memset(strBuff,0x00, sizeof(strBuff));  
    memset(strActualValue,0x00, sizeof(strActualValue));
    
    inRet=CTOS_UIKeypad(1, 8, keyboardLayoutEnglish, 40, 80, d_TRUE, d_FALSE, 0, 0,strBuff, 16);

    if(inRet==d_OK)
    {
        if (strlen(strBuff)>0)
        {
            memcpy(strActualValue,strBuff,strlen(strBuff));

            if (strlen(strBuff)>=15)
            {
                vduiClearBelow(2);  
                vduiDisplayStringCenter(4,"CONTINUE INPUT");
                vduiDisplayStringCenter(5,"MORE TEXT?");
                vduiDisplayStringCenter(8,"NO[X] YES[OK]");

                while(1)
                {
                    inRet=struiGetchWithTimeOut();
                    if (inRet==d_KBD_ENTER)
                    {
                        vduiClearBelow(2);  
                        CTOS_LCDTPrintXY(1, 6, "ENTR NEW LINE3:");
                        memset(strBuff,0x00, sizeof(strBuff));
                        
                        vduiClearBelow(7);  
                        CTOS_LCDTPrintXY(1, 7, strActualValue);
                        inRet=CTOS_UIKeypad(1, 8, keyboardLayoutEnglish, 40, 80, d_TRUE, d_FALSE, 0, 0,strBuff, 16);
                        if(inRet==d_OK)
                        {
                            if (strlen(strBuff)>0)
                            {
                                memcpy(&strActualValue[15],strBuff,strlen(strBuff));
                                if (strlen(strBuff)>=15)
                                {
                                    vduiClearBelow(2);  
                                    vduiDisplayStringCenter(4,"CONTINUE INPUT");
                                    vduiDisplayStringCenter(5,"MORE TEXT?");
                                    vduiDisplayStringCenter(8,"NO[X] YES[OK]");
                                    inRet=struiGetchWithTimeOut();
                                    if (inRet==d_KBD_ENTER)
                                    {
                                        vduiClearBelow(2);
                                        CTOS_LCDTPrintXY(1, 5, "ENTR NEW LINE3:");
                                        memset(strBuff,0x00, sizeof(strBuff));                                                                  
                                        CTOS_LCDTPrintXY(1, 6, strActualValue);
                                        CTOS_LCDTPrintXY(1, 7, &strActualValue[16]);
                                        inRet=CTOS_UIKeypad(1, 8, keyboardLayoutEnglish, 40, 80, d_TRUE, d_FALSE, 0, 0,strBuff, 16);
                                        if(inRet==d_OK)
                                        {    
                                           memcpy(&strActualValue[30],strBuff,strlen(strBuff));
                                        }
                                        break;
                                    }
                                    else if (inRet==d_KBD_CANCEL)
                                        break;
                                    else
                                            vduiWarningSound();
                                }
                            }
                            else
                                break;
                                                
                        }
                        break;
                    }
                    else if (inRet==d_KBD_CANCEL)
                        break;
                    else
                        vduiWarningSound();
                }
            }
            memset(strPCT.szRctHdr3,0x00, sizeof(strPCT.szRctHdr3));    
            memcpy(strPCT.szRctHdr3,strActualValue,strlen(strActualValue));
            
        }
        else
        {
            vduiClearBelow(3);  
            vduiDisplayStringCenter(4,"NO VALUE ENTERED");
            vduiDisplayStringCenter(5,"SAVE EMPTY?");
            vduiDisplayStringCenter(8,"NO[X] YES[OK]");
            while(1)
            {
                inRet=struiGetchWithTimeOut();
                if (inRet==d_KBD_ENTER)
                {
                    memset(strPCT.szRctHdr3,0x00, sizeof(strPCT.szRctHdr3));    
                    break;
                }
                else if (inRet==d_KBD_CANCEL)
                    break;
                else
                    vduiWarningSound();
            }
        }
    }         
        
      
        
    //---------HEADER LINE 4---------------------//    
    vduiClearBelow(2);  
    vduiDisplayStringCenter(1, "ADDRESS HEADER");
    CTOS_LCDTPrintXY(1, 2, "OLD HEADER LINE4:           ");
    CTOS_LCDTPrintXY(1, 3, strPCT.szRctHdr4);
    CTOS_LCDTPrintXY(1, 4, &strPCT.szRctHdr4[16]);
    CTOS_LCDTPrintXY(1, 5, &strPCT.szRctHdr4[32]);
    CTOS_LCDTPrintXY(1, 6, "ENTR NEW LINE4:");
    memset(strBuff,0x00, sizeof(strBuff));  
    memset(strActualValue,0x00, sizeof(strActualValue));
    
    inRet=CTOS_UIKeypad(1, 8, keyboardLayoutEnglish, 40, 80, d_TRUE, d_FALSE, 0, 0,strBuff, 16);

    if(inRet==d_OK)
    {
        if (strlen(strBuff)>0)
        {
            memcpy(strActualValue,strBuff,strlen(strBuff));

            if (strlen(strBuff)>=15)
            {
                vduiClearBelow(2);  
                vduiDisplayStringCenter(4,"CONTINUE INPUT");
                vduiDisplayStringCenter(5,"MORE TEXT?");
                vduiDisplayStringCenter(8,"NO[X] YES[OK]");

                while(1)
                {
                    inRet=struiGetchWithTimeOut();
                    if (inRet==d_KBD_ENTER)
                    {
                        vduiClearBelow(2);  
                        CTOS_LCDTPrintXY(1, 6, "ENTR NEW LINE4:");
                        memset(strBuff,0x00, sizeof(strBuff));
                        
                        vduiClearBelow(7);  
                        CTOS_LCDTPrintXY(1, 7, strActualValue);
                        inRet=CTOS_UIKeypad(1, 8, keyboardLayoutEnglish, 40, 80, d_TRUE, d_FALSE, 0, 0,strBuff, 16);
                        if(inRet==d_OK)
                        {
                            if (strlen(strBuff)>0)
                            {
                                memcpy(&strActualValue[15],strBuff,strlen(strBuff));
                                if (strlen(strBuff)>=15)
                                {
                                    vduiClearBelow(2);  
                                    vduiDisplayStringCenter(4,"CONTINUE INPUT");
                                    vduiDisplayStringCenter(5,"MORE TEXT?");
                                    vduiDisplayStringCenter(8,"NO[X] YES[OK]");
                                    inRet=struiGetchWithTimeOut();
                                    if (inRet==d_KBD_ENTER)
                                    {
                                        vduiClearBelow(2);
                                        CTOS_LCDTPrintXY(1, 5, "ENTR NEW LINE4:");
                                        memset(strBuff,0x00, sizeof(strBuff));                                                                  
                                        CTOS_LCDTPrintXY(1, 6, strActualValue);
                                        CTOS_LCDTPrintXY(1, 7, &strActualValue[16]);
                                        inRet=CTOS_UIKeypad(1, 8, keyboardLayoutEnglish, 40, 80, d_TRUE, d_FALSE, 0, 0,strBuff, 16);
                                        if(inRet==d_OK)
                                        {    
                                           memcpy(&strActualValue[30],strBuff,strlen(strBuff));
                                        }
                                        break;
                                    }
                                    else if (inRet==d_KBD_CANCEL)
                                        break;
                                    else
                                            vduiWarningSound();
                                }
                            }
                            else
                                break;
                                                
                        }
                        break;
                    }
                    else if (inRet==d_KBD_CANCEL)
                        break;
                    else
                        vduiWarningSound();
                }
            }
            memset(strPCT.szRctHdr4,0x00, sizeof(strPCT.szRctHdr4));    
            memcpy(strPCT.szRctHdr4,strActualValue,strlen(strActualValue));
            
        }
        else
        {
            vduiClearBelow(3);  
            vduiDisplayStringCenter(4,"NO VALUE ENTERED");
            vduiDisplayStringCenter(5,"SAVE EMPTY?");
            vduiDisplayStringCenter(8,"NO[X] YES[OK]");
            while(1)
            {
                inRet=struiGetchWithTimeOut();
                if (inRet==d_KBD_ENTER)
                {
                    memset(strPCT.szRctHdr4,0x00, sizeof(strPCT.szRctHdr4));    
                    break;
                }
                else if (inRet==d_KBD_CANCEL)
                    break;
                else
                    vduiWarningSound();
            }
        }
    }   
        
        
        
   //---------HEADER LINE 5---------------------//    
    vduiClearBelow(2);  
    vduiDisplayStringCenter(1, "ADDRESS HEADER");
    CTOS_LCDTPrintXY(1, 2, "OLD HEADER LINE5:           ");
    CTOS_LCDTPrintXY(1, 3, strPCT.szRctHdr5);
    CTOS_LCDTPrintXY(1, 4, &strPCT.szRctHdr5[16]);
    CTOS_LCDTPrintXY(1, 5, &strPCT.szRctHdr5[32]);
    CTOS_LCDTPrintXY(1, 6, "ENTR NEW LINE5:");
    memset(strBuff,0x00, sizeof(strBuff));  
    memset(strActualValue,0x00, sizeof(strActualValue));
    
    inRet=CTOS_UIKeypad(1, 8, keyboardLayoutEnglish, 40, 80, d_TRUE, d_FALSE, 0, 0,strBuff, 16);

    if(inRet==d_OK)
    {
        if (strlen(strBuff)>0)
        {
            memcpy(strActualValue,strBuff,strlen(strBuff));

            if (strlen(strBuff)>=15)
            {
                vduiClearBelow(2);  
                vduiDisplayStringCenter(4,"CONTINUE INPUT");
                vduiDisplayStringCenter(5,"MORE TEXT?");
                vduiDisplayStringCenter(8,"NO[X] YES[OK]");

                while(1)
                {
                    inRet=struiGetchWithTimeOut();
                    if (inRet==d_KBD_ENTER)
                    {
                        vduiClearBelow(2);  
                        CTOS_LCDTPrintXY(1, 6, "ENTR NEW LINE5:");
                        memset(strBuff,0x00, sizeof(strBuff));
                        
                        vduiClearBelow(7);  
                        CTOS_LCDTPrintXY(1, 7, strActualValue);
                        inRet=CTOS_UIKeypad(1, 8, keyboardLayoutEnglish, 40, 80, d_TRUE, d_FALSE, 0, 0,strBuff, 16);
                        if(inRet==d_OK)
                        {
                            if (strlen(strBuff)>0)
                            {
                                memcpy(&strActualValue[15],strBuff,strlen(strBuff));
                                if (strlen(strBuff)>=15)
                                {
                                    vduiClearBelow(2);  
                                    vduiDisplayStringCenter(4,"CONTINUE INPUT");
                                    vduiDisplayStringCenter(5,"MORE TEXT?");
                                    vduiDisplayStringCenter(8,"NO[X] YES[OK]");
                                    inRet=struiGetchWithTimeOut();
                                    if (inRet==d_KBD_ENTER)
                                    {
                                        vduiClearBelow(2);
                                        CTOS_LCDTPrintXY(1, 5, "ENTR NEW LINE5:");
                                        memset(strBuff,0x00, sizeof(strBuff));                                                                  
                                        CTOS_LCDTPrintXY(1, 6, strActualValue);
                                        CTOS_LCDTPrintXY(1, 7, &strActualValue[16]);
                                        inRet=CTOS_UIKeypad(1, 8, keyboardLayoutEnglish, 40, 80, d_TRUE, d_FALSE, 0, 0,strBuff, 16);
                                        if(inRet==d_OK)
                                        {    
                                           memcpy(&strActualValue[30],strBuff,strlen(strBuff));
                                        }
                                        break;
                                    }
                                    else if (inRet==d_KBD_CANCEL)
                                        break;
                                    else
                                            vduiWarningSound();
                                }
                            }
                            else
                                break;
                                                
                        }
                        break;
                    }
                    else if (inRet==d_KBD_CANCEL)
                        break;
                    else
                        vduiWarningSound();
                }
            }
            memset(strPCT.szRctHdr5,0x00, sizeof(strPCT.szRctHdr5));    
            memcpy(strPCT.szRctHdr5,strActualValue,strlen(strActualValue));
            
        }
        else
        {
            vduiClearBelow(3);  
            vduiDisplayStringCenter(4,"NO VALUE ENTERED");
            vduiDisplayStringCenter(5,"SAVE EMPTY?");
            vduiDisplayStringCenter(8,"NO[X] YES[OK]");
            while(1)
            {
                inRet=struiGetchWithTimeOut();
                if (inRet==d_KBD_ENTER)
                {
                    memset(strPCT.szRctHdr5,0x00, sizeof(strPCT.szRctHdr5));    
                    break;
                }
                else if (inRet==d_KBD_CANCEL)
                    break;
                else
                    vduiWarningSound();
            }
        }
    }         
        
    inPCTSave(1);
}

#if 1
void vdCTOSS_DownloadMode(void)
{

	CTOS_LCDTClearDisplay();
	CTOS_EnterDownloadMode();

	inCTOS_KillALLSubAP();
	
	exit(0);
	return ;
	
}
#else
void vdCTOSS_DownloadMode(void)
{
#define ITEMS_PER_PAGE          4

    char szMMT[50];
    char szDisplay[50];
    int inNumOfRecords = 0;
    short shCount = 0;
    short shTotalPageNum;
    short shCurrentPageNum;
    short shLastPageItems = 0;
    short shPageItems = 0;
    short shLoop;
    short shFalshMenu = 1;
     BYTE isUP = FALSE, isDOWN = FALSE;
	 BYTE bHeaderAttr = 0x01+0x04, iCol = 1;
    BYTE  x = 1;
    BYTE key;
    char szHeaderString[50] = "SELECT MERCHANT";
    char szMitMenu[1024];
    int inLoop = 0;
	char temp[40+1];

    unsigned char bstatus = 0; 

    memset(szMitMenu, 0x00, sizeof(szMitMenu));
    	CTOS_PrinterPutString("vdCTOSS_DownloadMode");

	strHDT.inHostIndex = 1;
	
    //get the index , then get all MID from the MMT list and prompt to user to select
    inMMTReadNumofRecords(strHDT.inHostIndex,&inNumOfRecords);
	CTOS_KBDBufFlush();//cleare key buffer

    memset(temp, 0x00, sizeof(temp));
	sprintf(temp, "&&&&&::%d:%d", strHDT.inHostIndex, inNumOfRecords);
    	CTOS_PrinterPutString(temp);

    if(inNumOfRecords > 1)
	{
	    for (inLoop = 0; inLoop < inNumOfRecords; inLoop++)
	    {
            strcat((char *)szMitMenu, strMMT[inLoop].szMerchantName);
			if(inLoop + 1 != inNumOfRecords)
            	strcat((char *)szMitMenu, (char *)" \n");
	    }
	        
	    key = MenuDisplay(szHeaderString, strlen(szHeaderString), bHeaderAttr, iCol, x, szMitMenu, TRUE);

		if (key == 0xFF) 
		{
		    CTOS_LCDTClearDisplay();
		    setLCDPrint(1, DISPLAY_POSITION_CENTER, "WRONG INPUT!!!");
		    vduiWarningSound();
		    return -1;  
		}

    	if(key > 0)
	    {
	        if(d_KBD_CANCEL == key)
	            return -1;
	        
	        vdDebug_LogPrintf("key[%d]-------", key);		
		memcpy(&strMMT[0],&strMMT[key-1],sizeof(STRUCT_MMT));
	    }
	}

#if 0	
    srTransRec.MITid = strMMT[0].MITid;
    strcpy(srTransRec.szTID, strMMT[0].szTID);
    strcpy(srTransRec.szMID, strMMT[0].szMID);
    memcpy(srTransRec.szBatchNo, strMMT[0].szBatchNo, 4);
    strcpy(srTransRec.szHostLabel, strHDT.szHostLabel);

    vdDebug_LogPrintf("szATCMD1=[%s] [%s] [%s] [%s] [%s]",strMMT[0].szATCMD1,strMMT[0].szATCMD2,strMMT[0].szATCMD3,strMMT[0].szATCMD4,strMMT[0].szATCMD5);
#endif    

    return SUCCESS;
    

}
#endif
void vdCTOSS_CheckMemory(void)
{
	vdCTOSS_GetMemoryStatus("MEMORY");
	return ;
}

/* ==========================================================================
 *
 * FUNCTION NAME: SetRTC
 *
 * DESCRIPTION:   Use this function to set the real-time clock's data and time.
 *
 * RETURN:        none.
 *
 * NOTES:         none.
 *
 * ========================================================================== */
void CTOSS_SetRTC(void)
{
	//Declare Local Variable //
	CTOS_RTC SetRTC;
	USHORT i;
	BYTE isSet = FALSE;
	BYTE baYear[4+1],baMonth[2+1],baDay[2+1],baHour[2+1],baMinute[2+1],baSecond[2+1];
	BYTE key;
	BYTE babuff[d_BUFF_SIZE];

	CTOS_LCDTClearDisplay();
	CTOS_LCDTSetReverse(TRUE);
	CTOS_LCDTPrintXY(1, 1, "     Set RTC     ");
	CTOS_LCDTSetReverse(FALSE);

	//Read the date and the time //
	CTOS_RTCGet(&SetRTC);

	//Show on the LCD Dispaly //
	CTOS_LCDTPrintXY(1, 2, "   Get   Set");
	sprintf(babuff,"YY:%04d",SetRTC.bYear + 2000);
	CTOS_LCDTPrintXY(1, 3, babuff);
	sprintf(babuff,"MM:%02d",SetRTC.bMonth);
	CTOS_LCDTPrintXY(1, 4, babuff);
	sprintf(babuff,"DD:%02d",SetRTC.bDay);
	CTOS_LCDTPrintXY(1, 5, babuff);
	sprintf(babuff,"hh:%02d",SetRTC.bHour);
	CTOS_LCDTPrintXY(1, 6, babuff);
	sprintf(babuff,"mm:%02d",SetRTC.bMinute);
	CTOS_LCDTPrintXY(1, 7, babuff);
	sprintf(babuff,"ss:%02d",SetRTC.bSecond);
	CTOS_LCDTPrintXY(1, 8, babuff);

	sprintf(babuff,"%02d",SetRTC.MCCW);
	CTOS_LCDTPrintXY(15, 8, babuff);

	//Input data for the setting //
	i = sizeof(baYear);
  	if (InputString(10,3,FALSE, 0x02, baYear, &i, i, d_INPUT_TIMEOUT) == 1){    //Input Year //
    	SetRTC.bYear = (wub_str_2_long(baYear) - 2000);
    	isSet = TRUE;
  	}
  	i = sizeof(baMonth);
  	if (InputString(10,4,FALSE, 0x02, baMonth, &i, i, d_INPUT_TIMEOUT) == 1){   //Input Month //
    	SetRTC.bMonth = wub_str_2_long(baMonth);
    	isSet = TRUE;
  	}
  	i = sizeof(baDay);
  	if (InputString(10,5,FALSE, 0x02, baDay, &i, i, d_INPUT_TIMEOUT) == 1){	  //Input Day //
    	SetRTC.bDay = wub_str_2_long(baDay);
    	isSet = TRUE;
  	}
  	i = sizeof(baHour);
  	if (InputString(10,6,FALSE, 0x02, baHour, &i, i, d_INPUT_TIMEOUT) == 1){  //Input Hour //
    	SetRTC.bHour = wub_str_2_long(baHour);
    	isSet = TRUE;
  	}
  	i = sizeof(baMinute);
  	if (InputString(10,7,FALSE, 0x02, baMinute, &i, i, d_INPUT_TIMEOUT) == 1){  //Input Minute //
    	SetRTC.bMinute = wub_str_2_long(baMinute);
    	isSet = TRUE;
  	}
  	i = sizeof(baSecond);
  	if (InputString(10,8,FALSE, 0x02, baSecond, &i, i, d_INPUT_TIMEOUT) == 1){  //Input Second //
    	SetRTC.bSecond = wub_str_2_long(baSecond);
    	isSet = TRUE;
  	}
  
  	if (isSet){
    	//Set the date and time //
    	if (CTOS_RTCSet(&SetRTC) == d_OK) CTOS_LCDTPrintXY(15, 2, "OK");
   	 	else CTOS_LCDTPrintXY(13, 2, "Fail");
    	isSet = FALSE;
  	}
  
//  	CTOS_KBDGet ( &key );
  	return;
}
void vdCTOSS_FileUpload(void)
{
    int st, uintRet;
	unsigned char szDisplay[40];
    unsigned char temp[1024];
    unsigned char public_path[50] = "/home/ap/pub/";
    unsigned char private_path[50] = "./fs_data";
    FILE  *fPubKey;
    unsigned char szFileName1[50];
	char szFileName[50];
	ULONG ulSize;
	int iExp;
    STR *keyboardLayoutEnglish[]={" 0", "qzQZ1", "abcABC2", "defDEF3", "ghiGHI4",
    "jklJKL5", "mnoMNO6", "prsPRS7", "tuvTUV8", "wxyWXY9", ":;/\\|?,.<>_",
    ".!@#$%^&*()-"};
    int inRet;

	CTOS_LCDTClearDisplay ();                                                                                
                                                                                                           
                               
    CTOS_LCDTSetReverse(FALSE);    
    CTOS_USBOpen();
    
    CTOS_LCDTPrintXY(1,1,"ENTER FILENAME:");
    inRet=CTOS_UIKeypad(1, 8, keyboardLayoutEnglish, 40, 80, d_TRUE, d_FALSE, 0, 0,szFileName1, 50);
    if(inRet==d_OK)
    {
        if (strlen(szFileName1)>0)
		{
            memset(szFileName, 0x00, sizeof(szFileName));
            sprintf(szFileName,"%s%s", public_path,szFileName1);
        }
        else
        {
             CTOS_LCDTPrintXY(1,1,"TRANSFER CANCEL");
             WaitKey(3);
             return;
        }
    }
    else
    {
        CTOS_LCDTPrintXY(1,1,"TRANSFER CANCEL");
        WaitKey(3);
        return;
    }
    //strcpy(szFileName1,"V5S_VISAMASTER.prm");
    //sprintf(szFileName,"%s%s", public_path,szFileName1);
    
    ulSize = 0;
	fPubKey = fopen( (char*)szFileName1, "rb");
	if (fPubKey == NULL)
	{
        CTOS_LCDTPrintXY(1,1,"OPEN FILE ERROR");
        CTOS_LCDTPrintXY(1,2,szFileName1);
        //CTOS_LCDTPrintXY(1,3,&szFileName[19]);
        WaitKey(5);
		//memset(szFileName, 0x00, sizeof(szFileName));
        //sprintf(szFileName,"%s%s", private_path,szFileName1);
        fPubKey = fopen( (char*)szFileName, "rb");
        if (fPubKey == NULL)
        {
            CTOS_LCDTPrintXY(1,1,"OPEN FILE ERROR");
            CTOS_LCDTPrintXY(1,2,szFileName);
            CTOS_LCDTPrintXY(1,3,&szFileName[19]);
            
            memset(szFileName, 0x00, sizeof(szFileName));
            sprintf(szFileName,"%s%s", private_path,szFileName1);
            fPubKey = fopen( (char*)szFileName, "rb");
            if (fPubKey == NULL)
            {
                CTOS_LCDTPrintXY(1,1,"OPEN FILE ERROR");
                CTOS_LCDTPrintXY(1,2,szFileName);
                CTOS_LCDTPrintXY(1,3,&szFileName[19]);
                WaitKey(5);
                return;
            }
        }
		
	}
    
    CTOS_LCDTPrintXY(1,1,"                 ");
    CTOS_LCDTPrintXY(1,2,"                 ");
    CTOS_LCDTPrintXY(1,3,"                 ");
	CTOS_LCDTPrintXY(1,1,"Transfer Size");
	while(1)
	{
		uintRet = fread( temp, 1, sizeof(temp), fPubKey);
		ulSize += uintRet;
		sprintf(szDisplay,"%ld", ulSize);
		CTOS_LCDTPrintXY(1,2,szDisplay);
		if(uintRet > 0)
			CTOS_USBTxData(temp, uintRet);
		if(uintRet <=0)
			break;
		CTOS_Delay(5);
	}
	fclose(fPubKey); 
	CTOS_LCDTPrintXY(1,3,"File Transmitted");
	WaitKey(3); 
    /*if(remove(szFileName1) == -1)
    {
        CTOS_LCDTPrintXY(1,3,"File Delete Error");
   
    }
    else CTOS_LCDTPrintXY(1,3,"File Deleted");
    WaitKey(3);*/
}

int inCTOS_PromptPassword(void)
{
    int inRet = d_NO;
    
   //CTOS_LCDTClearDisplay();
    vduiClearBelow(7);
    vdCTOS_SetTransType(SETUP);
    
    vdDispTransTitle(SETUP);
    
    inRet = inCTOSS_CheckMemoryStatus();
    if(d_OK != inRet)
        return inRet;
    
    inRet = inCTOS_GetTxnPassword();

   //CTOS_LCDTClearDisplay();
	
    if(d_OK != inRet)
    {
        inCTOS_IdleEventProcess();
        return inRet;
    }

    return d_OK;
}

void vdCTOS_TMSSetting(void)
{
	BYTE strOut[30],strtemp[17];
	BYTE szInputBuf[5],count;
    int inResult;
    int ret;
	USHORT usLen;
    BYTE szSerialNum[17 + 1];
	int bSIMSlot=0;
	USHORT usRes=0;

	int inCommMode = -1;
	BOOL fValid = TRUE;

	vdDebug_LogPrintf("--vdCTOS_TMSSetting--");

	// Input library function call -- sidumili
	vdCTOS_InputTMSSetting();
	return;

   //CTOS_LCDTClearDisplay();
    vdDispTitleString("TMS SETTINGS");

	inTCPRead(1);
	
    memset(szSerialNum, 0x00, sizeof(szSerialNum));
	CTOS_CTMSGetConfig(d_CTMS_SERIALNUM, szSerialNum);
    while(1)
    {   
        vduiClearBelow(3);
		setLCDPrint(3, DISPLAY_POSITION_LEFT, "SERIAL NO.");
		setLCDPrint(4, DISPLAY_POSITION_LEFT, szSerialNum);
		
		strcpy(strtemp,"New:") ;
		CTOS_LCDTPrintXY(1, 7, strtemp);
		memset(strOut,0x00, sizeof(strOut));
		usLen = 32;
		//ret= InputStringAlpha(1, 8, 0x00, 0x02, strOut, &usLen, 16, d_INPUT_TIMEOUT);
		ret = shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 16, 0, d_INPUT_TIMEOUT);
		if(ret==d_KBD_ENTER)
		{
			//len = strlen(szSerialNum);
			strcpy(szSerialNum,"0000000000000000");
			memcpy(&szSerialNum[16-usLen],strOut,usLen);
			vdDebug_LogPrintf("szSerialNum=[%s].tmpbuf=[%s].lenght[%d]",szSerialNum,strOut, strlen(strOut));

			//memcpy(strTCP.szAPN, strOut,strlen(strOut));
			//strTCP.szAPN[strlen(strOut)]=0;
			CTOS_CTMSSetConfig(d_CTMS_SERIALNUM, szSerialNum);
            CTOS_CTMSSetConfig(d_CTMS_RECOUNT, &count);
			
			break;
		}	
		if(ret == d_KBD_CANCEL)
			break;
    }	

    inResult = inTCTRead(1);
    if(inResult != d_OK)
        return;	
    while(1)
    {   
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "TMS GAP");
        memset(szInputBuf, 0x00, sizeof(szInputBuf));
        sprintf(szInputBuf, "%d", strTCT.usTMSGap);
        setLCDPrint(4, DISPLAY_POSITION_LEFT, szInputBuf);
        
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 3, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret>=1)
        {
            strTCT.usTMSGap=atoi(strOut);      
            vdMyEZLib_LogPrintf("strTCT.usTMSGap %d",strTCT.usTMSGap);
			inResult = inTCTSave(1);
            break;
        }   
        if(ret == d_KBD_CANCEL)
            break;
    }	

    inResult = inTCTRead(1);
    if(inResult != d_OK)
        return;	

    while(1)
    {
    	vduiClearBelow(3);
    	setLCDPrint(3, DISPLAY_POSITION_LEFT, "AUTO DOWNLOAD");
    	
    	if(strTCT.fSettleAutoDL== 0)
    		setLCDPrint(4, DISPLAY_POSITION_LEFT, "DISABLE");
    	if(strTCT.fSettleAutoDL == 1)
    		setLCDPrint(4, DISPLAY_POSITION_LEFT, "ENABLE"); 	
    	
    	CTOS_LCDTPrintXY(1, 5, "0-DISABLE      1-ENABLE");
    	
    	strcpy(strtemp,"New:") ; 
    	CTOS_LCDTPrintXY(1, 7, strtemp);
    	memset(strOut,0x00, sizeof(strOut));
    	memset(strtemp, 0x00, sizeof(strtemp));
    	usLen = 10;
    	ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 0, d_INPUT_TIMEOUT);
    	if(ret==d_KBD_ENTER)
    	{
    		if (strOut[0]==0x30 || strOut[0]==0x31)
    		{
    			if(strOut[0] == 0x31)
    			{
    				strTCT.fSettleAutoDL=1;
    			}
    			if(strOut[0] == 0x30)
    			{
    				strTCT.fSettleAutoDL=0;
    			}
    			inTCTSave(1);
    			break;
    		}
    		else
    		{
                vduiClearBelow(2);
                vduiWarningSound();
                vduiDisplayStringCenter(7,"PLEASE SELECT");
                vduiDisplayStringCenter(8,"A VALID VALUE");
                CTOS_Delay(2000);		
    		}
    	}
    	if(ret == d_KBD_CANCEL || ret == 0xFF)
    		break;
    }

    inResult = inTCTRead(1);
    if(inResult != d_OK)
        return;	


	vduiClearBelow(3);
	setLCDPrint(3, DISPLAY_POSITION_LEFT, "TMS COMM MODE");
	switch(strTCT.inTMSComMode)
	{
		case 0:
			setLCDPrint(4, DISPLAY_POSITION_LEFT, "DIALUP");
			break;
		case 1:
			setLCDPrint(4, DISPLAY_POSITION_LEFT, "LAN");
			break;
		case 2:
			setLCDPrint(4, DISPLAY_POSITION_LEFT, "GPRS");
			break;
		case 4:
			setLCDPrint(4, DISPLAY_POSITION_LEFT, "WIFI");
			break;
	}
	
    while(1)
    {   
	    fValid = TRUE;

		if (isCheckTerminalMP200() == d_OK)
		{
			setLCDPrint(5, DISPLAY_POSITION_LEFT, "1-GPRS");
			setLCDPrint(6, DISPLAY_POSITION_LEFT, "2-WIFI");
		}
		else
		{
			setLCDPrint(5, DISPLAY_POSITION_LEFT, "0-DIALUP 1-LAN");
			setLCDPrint(6, DISPLAY_POSITION_LEFT, "2-GPRS   3-WIFI");
		}
		        
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 1, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL || ret == 0)
            break;
        else
        {

			if (isCheckTerminalMP200() == d_OK)
			{
				switch (strOut[0])
				{
					case 0x31:
						inCommMode = GPRS_MODE;
					break;
					case 0x32:
						inCommMode = WIFI_MODE;
					break;
					default:
						fValid = FALSE;
					break;	
				}
			}
			else
			{
				switch (strOut[0])
				{
					case 0x30:
						inCommMode = DIAL_UP_MODE;
					break;
					case 0x31:
						inCommMode = ETHERNET_MODE;
					break;
					case 0x32:
						inCommMode = GPRS_MODE;
					break;
					case 0x33:
						inCommMode = WIFI_MODE;
					break;
					default:
						fValid = FALSE;
					break;	
				}
			}
			            
        }   
        
		if (!fValid)
			vdDisplayErrorMsg(1, 8, "INVALID INPUT");
		else					
			break; // exit while loop
		
    }	

	// Check hardware
	inResult = inCheckPromptHW(inCommMode);
	if(inResult != d_OK)
	return;

	// Save
	strTCT.inTMSComMode = inCommMode;      
	vdMyEZLib_LogPrintf("strTCT.inTMSComMode %d -- strOut[%d]",strTCT.inTMSComMode, strOut[0]);
	inResult = inTCTSave(1);
			
	// Read
    inResult = inTCTRead(1);
    if(inResult != d_OK)
        return;	
	
    while(1)
    {
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "TMS REMOTE IP");
        setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCT.szTMSRemoteIP);
        
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strtemp, 0x00, sizeof(strtemp));
        memset(strOut,0x00, sizeof(strOut));
        ret= struiApiGetStringSub(strtemp, 1,  8, strOut, MODE_IPADDRESS, 7, IP_LEN);
        if(ret==d_KBD_ENTER)
        {
            memcpy(strTCT.szTMSRemoteIP,strOut,strlen(strOut));
            strTCT.szTMSRemoteIP[strlen(strOut)]=0;
            vdMyEZLib_LogPrintf("strTCT.szTMSRemoteIP %s",strTCT.szTMSRemoteIP);
			inResult = inTCTSave(1);
            break;
        }	
        if(ret == d_KBD_CANCEL)
            break;
    }
	
    inResult = inTCTRead(1);
    if(inResult != d_OK)
        return;	
    while(1)
    {   
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "TMS REMOTE PORT");
        memset(szInputBuf, 0x00, sizeof(szInputBuf));
        sprintf(szInputBuf, "%d", strTCT.usTMSRemotePort);
        setLCDPrint(4, DISPLAY_POSITION_LEFT, szInputBuf);
        
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 5, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret>=1)
        {
            strTCT.usTMSRemotePort=atoi(strOut);      
            vdMyEZLib_LogPrintf("strTCT.usTMSRemotePort %d",strTCT.usTMSRemotePort);
			inResult = inTCTSave(1);
            break;
        }   
        if(ret == d_KBD_CANCEL)
            break;
    }	

    if(strTCT.inTMSComMode == DIAL_UP_MODE)
    {
    	vdDebug_LogPrintf("--TMS COM MODE - DIAL_UP_MODE--");
		
	    inResult = inTCTRead(1);
	    if(inResult != d_OK)
	        return;
		while(1)
		{
			vduiClearBelow(3);
			setLCDPrint(3, DISPLAY_POSITION_LEFT, "TMS REMOTE PHONE");
			setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCT.szTMSRemotePhone);
		
			strcpy(strtemp,"New:") ; 
			CTOS_LCDTPrintXY(1, 7, strtemp);
			memset(strOut,0x00, sizeof(strOut));
			usLen = 32;
			ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
			if (ret == d_KBD_CANCEL )
				break;
			else if(0 == ret )
				break;
			else if(ret>= 1)
			{
				memcpy(strTCT.szTMSRemotePhone,strOut,strlen(strOut));
				strTCT.szTMSRemotePhone[strlen(strOut)]=0;
				inResult = inTCTSave(1);
				break;
			}
			if(ret == d_KBD_CANCEL)
				break;
		}

	    inResult = inTCTRead(1);
	    if(inResult != d_OK)
	        return;
		while(1)
		{
			vduiClearBelow(3);
			setLCDPrint(3, DISPLAY_POSITION_LEFT, "TMS REMOTE ID");
			setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCT.szTMSRemoteID);
		
			strcpy(strtemp,"New:") ; 
			CTOS_LCDTPrintXY(1, 7, strtemp);
			memset(strOut,0x00, sizeof(strOut));
			usLen = 32;
			//ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
			ret= InputStringAlpha(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
			if (ret == d_KBD_CANCEL )
				break;
			else if(0 == ret )
				break;
			else if(ret>= 1)
			{
				memcpy(strTCT.szTMSRemoteID,strOut,strlen(strOut));
				strTCT.szTMSRemoteID[strlen(strOut)]=0;
				inResult = inTCTSave(1);
				break;
			}
			if(ret == d_KBD_CANCEL)
				break;
		}
	
	    inResult = inTCTRead(1);
	    if(inResult != d_OK)
	        return;
		while(1)
		{
			vduiClearBelow(3);
			setLCDPrint(3, DISPLAY_POSITION_LEFT, "TMS REMOTE PASSWORD");
			setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCT.szTMSRemotePW);
		
			strcpy(strtemp,"New:") ; 
			CTOS_LCDTPrintXY(1, 7, strtemp);
			memset(strOut,0x00, sizeof(strOut));
			usLen = 32;
			//ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
			ret= InputStringAlpha(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
			if (ret == d_KBD_CANCEL )
				break;
			else if(0 == ret )
				break;
			else if(ret>= 1)
			{
				memcpy(strTCT.szTMSRemotePW,strOut,strlen(strOut));
				strTCT.szTMSRemotePW[strlen(strOut)]=0;
				inResult = inTCTSave(1);
				break;
			}
			if(ret == d_KBD_CANCEL)
				break;
		
		}
    }
    else if(strTCT.inTMSComMode == GPRS_MODE)
    {
		CTMS_GPRSInfo stgprs;

		vdDebug_LogPrintf("--TMS COM MODE - GPRS--");
		
		memset(&stgprs, 0x00, sizeof (CTMS_GPRSInfo));
		usRes=CTOS_CTMSGetConfig(d_CTMS_GPRS_CONFIG, &stgprs);
		vdDebug_LogPrintf("CTOS_CTMSGetConfig - usRes[%d]", usRes);
		/*
        if(strlen(stgprs.strAPN) > 0)
            strcpy(strTCP.szAPN, stgprs.strAPN);

        if(strlen(stgprs.strID) > 0)
            strcpy(strTCP.szUserName, stgprs.strID);

        if(strlen(stgprs.strPW) > 0)
		    strcpy(strTCP.szPassword, stgprs.strPW);
		*/
		
		while(1)
		{	
			vduiClearBelow(3);
			setLCDPrint(3, DISPLAY_POSITION_LEFT, "SIM SLOT");
			setLCDPrint(4, DISPLAY_POSITION_LEFT, "1~2");
			memset(szInputBuf, 0x00, sizeof(szInputBuf));
			sprintf(szInputBuf, "%d", stgprs.bSIMSlot);
			//sprintf(szInputBuf, "%d", strTCP.inSIMSlot);
			setLCDPrint(6, DISPLAY_POSITION_LEFT, szInputBuf);
			
			strcpy(strtemp,"New:") ;
			CTOS_LCDTPrintXY(1, 7, strtemp);
			memset(strOut,0x00, sizeof(strOut));
			ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 1, 0, d_INPUT_TIMEOUT);
			if (ret == d_KBD_CANCEL )
				break;
			else if(0 == ret )
				break;
			else if(ret>=1)
			{
				if(atoi(strOut) == 1 || atoi(strOut) == 2)
				{
					if(atoi(strOut) == 1)
					    bSIMSlot=1;
					else
					    bSIMSlot=2;	
					break;
				}
				else
					vdDisplayErrorMsg(1, 8, "INVALID INPUT");
			}	
			if(ret == d_KBD_CANCEL)
				break;
		}	
						
        while(1)
        {
			vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "APN");
            setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCP.szAPN);
            
            strcpy(strtemp,"New:") ;
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strOut,0x00, sizeof(strOut));
			usLen = 32;
            ret= InputStringAlpha(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
            if(ret==d_KBD_ENTER)
            {
                memcpy(strTCP.szAPN, strOut,strlen(strOut));
                strTCP.szAPN[strlen(strOut)]=0;
                break;
            }   
            if(ret == d_KBD_CANCEL)
                break;
        }               
    
        while(1)
        {
			vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "GPRS USER NAME");
            setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCP.szUserName);
    
            strcpy(strtemp,"New:") ;
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strOut,0x00, sizeof(strOut));
			usLen = 32;
            ret= InputStringAlpha(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
            if(ret==d_KBD_ENTER)
            {
                //BolDetachGPRSChangeSetting=TRUE;
                memcpy(strTCP.szUserName, strOut,strlen(strOut));
                strTCP.szUserName[strlen(strOut)]=0;
                //inResult = inTCPSave(1);
                break;
            }   
            if(ret == d_KBD_CANCEL)
                break;
        }               
    
        while(1)
        {
			vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "GPRS PASSWORD");
            setLCDPrint(4, DISPLAY_POSITION_CENTER, strTCP.szPassword);
    
            strcpy(strtemp,"New:") ;
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strOut,0x00, sizeof(strOut));
			usLen = 32;
            ret= InputStringAlpha(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
            if(ret==d_KBD_ENTER)
            {
                //BolDetachGPRSChangeSetting=TRUE;
                memcpy(strTCP.szPassword, strOut,strlen(strOut));
                strTCP.szPassword[strlen(strOut)]=0;
                //inResult = inTCPSave(1);
                break;
            }   
            if(ret == d_KBD_CANCEL)
                break;
        } 

        memset(&stgprs, 0x00, sizeof (CTMS_GPRSInfo));
		bSIMSlot = 1;
		stgprs.bSIMSlot = bSIMSlot;
        strcpy(stgprs.strAPN, strTCP.szAPN);
        strcpy(stgprs.strID, strTCP.szUserName);
        strcpy(stgprs.strPW, strTCP.szPassword);
 
        strcpy(stgprs.strRemoteIP, strTCT.szTMSRemoteIP);
        stgprs.usRemotePort = strTCT.usTMSRemotePort;
        stgprs.ulSIMReadyTimeout = 10000;
        stgprs.ulGPRSRegTimeout = 10000;
        stgprs.usPPPRetryCounter = 5;
        stgprs.ulPPPTimeout = 10000;
        stgprs.ulTCPConnectTimeout = 10000;

		stgprs.ulTCPTxTimeout=10000;
		stgprs.ulTCPRxTimeout=10000;		

		usRes=CTOS_CTMSSetConfig(d_CTMS_GPRS_CONFIG, &stgprs);

		vdDebug_LogPrintf("CTOS_CTMSSetConfig - usRes[%d]", usRes);
		vdDebug_LogPrintf("stgprs.strAPN[%s]", stgprs.strAPN);
		vdDebug_LogPrintf("stgprs.strID[%s]", stgprs.strID);
		vdDebug_LogPrintf("stgprs.strPW[%s]", stgprs.strPW);
		vdDebug_LogPrintf("stgprs.strRemoteIP[%s]", stgprs.strRemoteIP);
		vdDebug_LogPrintf("stgprs.usRemotePort[%d]", stgprs.usRemotePort);
		
		vdDebug_LogPrintf("strTCP.szAPN[%s]", strTCP.szAPN);
		vdDebug_LogPrintf("strTCP.szUserName[%s]", strTCP.szUserName);
		vdDebug_LogPrintf("strTCP.szPassword[%s]", strTCP.szPassword);
		
    }
	else if (strTCT.inTMSComMode == WIFI_MODE)
	{
		CTMS_WIFIInfo stWF;
	
		memset(&stWF, 0x00, sizeof (d_CTMS_WIFI_CONFIG));
		CTOS_CTMSGetConfig(d_CTMS_WIFI_CONFIG, &stWF);
	
		strcpy(stWF.strRemoteIP, strTCT.szTMSRemoteIP);
		stWF.usRemotePort = strTCT.usTMSRemotePort;
		//stgprs.ulSIMReadyTimeout = 10000;
		//stgprs.ulGPRSRegTimeout = 10000;
		//stgprs.usPPPRetryCounter = 5;
		//stgprs.ulPPPTimeout = 10000;
		//stgprs.ulTCPConnectTimeout = 10000;
	
		CTOS_CTMSSetConfig(d_CTMS_WIFI_CONFIG , &stWF);

    }
	
}

int  inCTOS_TMSPreConfigSetting(void)
{
	BYTE strOut[30],strtemp[17];
	BYTE szInputBuf[5];
    int inResult;
    int ret;
	USHORT usLen;

	int inCommMode = -1;
	BOOL fValid = TRUE;

	vdDebug_LogPrintf("--inCTOS_TMSPreConfigSetting--");
	
   //CTOS_LCDTClearDisplay();
    vdDispTitleString("TMS SETTINGS");

    inResult = inTCTRead(1);
    if(inResult != d_OK)
        return d_NO;	

	vduiClearBelow(3);
	setLCDPrint(3, DISPLAY_POSITION_LEFT, "TMS COMM MODE");
	
	switch(strTCT.inTMSComMode)
	{
		case 0:
			setLCDPrint(4, DISPLAY_POSITION_LEFT, "DIALUP");
			break;
		case 1:
			setLCDPrint(4, DISPLAY_POSITION_LEFT, "LAN");
			break;
		case 2:
			setLCDPrint(4, DISPLAY_POSITION_LEFT, "GPRS");
			break;
		case 4:
			setLCDPrint(4, DISPLAY_POSITION_LEFT, "WIFI");
			break;
	}
	
    while(1)
    {   
	    fValid = TRUE;

		if (isCheckTerminalMP200() == d_OK)
		{
			setLCDPrint(5, DISPLAY_POSITION_LEFT, "1-GPRS");
			setLCDPrint(6, DISPLAY_POSITION_LEFT, "2-WIFI");
		}
		else
		{
			setLCDPrint(5, DISPLAY_POSITION_LEFT, "0-DIALUP 1-LAN");
			setLCDPrint(6, DISPLAY_POSITION_LEFT, "2-GPRS   4-WIFI");
		}
		        
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
		ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 1, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL || ret == 0)
            return d_NO;
        else
        {			
			if (isCheckTerminalMP200() == d_OK)
			{
				switch (strOut[0])
				{
					case 0x31:
						inCommMode = GPRS_MODE;
					break;
					case 0x32:
						inCommMode = WIFI_MODE;
					break;
					default:
						fValid = FALSE;
					break;	
				}
			}
			else
			{
				switch (strOut[0])
				{
					case 0x30:
						inCommMode = DIAL_UP_MODE;
					break;
					case 0x31:
						inCommMode = ETHERNET_MODE;
					break;
					case 0x32:
						inCommMode = GPRS_MODE;
					break;
					case 0x33:
						inCommMode = WIFI_MODE;
					break;
					default:
						fValid = FALSE;
					break;	
				}
			}						
        } 

		// Check for valid input - sidumili
		if (!fValid)
			vdDisplayErrorMsg(1, 8, "INVALID INPUT");
		else				
			break;	// exit while loop
		
			
    }	

	// Check hardware
	inResult = inCheckPromptHW(inCommMode);
	if(inResult != d_OK)
	return d_NO;
	
	strTCT.inTMSComMode = inCommMode;      
	vdMyEZLib_LogPrintf("strTCT.inTMSComMode %d -- strOut[%d]",strTCT.inTMSComMode, strOut[0]);
	inResult = inTCTSave(1);
			
    return d_OK;
}

int inCTOS_CommsFallback(int shHostIndex)
{
	int inResult = 0;

	vdDebug_LogPrintf("inCTOS_CommsFallback, Host Index [%d]", shHostIndex);
	vdDebug_LogPrintf("srTransRec.usTerminalCommunicationMode [%d]", srTransRec.usTerminalCommunicationMode);
	vdDebug_LogPrintf("strCPT.inCommunicationMode [%d]", strCPT.inCommunicationMode);

	vduiClearBelow(3);
	CTOS_LCDTPrintXY (1,6, "Comms Fallback");
	CTOS_LCDTPrintXY (1,7, "Please Wait 	");

	if ((srTransRec.usTerminalCommunicationMode == ETHERNET_MODE) || (srTransRec.usTerminalCommunicationMode == GPRS_MODE))
	{
		CTOS_LCDTPrintXY(1, 8, "Init Modem... 	");
		srTransRec.usTerminalCommunicationMode = DIAL_UP_MODE;
	}
	//else
	//{
		//CTOS_LCDTPrintXY(1, 8, "Init IP...	  ");
		//srTransRec.usTerminalCommunicationMode = ETHERNET_MODE;
	//}
	
	if (inCTOS_InitComm(srTransRec.usTerminalCommunicationMode) != d_OK) 
	{
		vdDebug_LogPrintf("inCTOS_CommsFallback, COMM INIT ERR");
		vdDisplayErrorMsg(1, 8, "COMM INIT ERR");
		return d_NO;
	}

	inCTOS_CheckInitComm(srTransRec.usTerminalCommunicationMode);

	vdDebug_LogPrintf("vdCTOS_DialBackupConfig, inCTOS_CheckInitComm");

	if (srTransRec.byOffline == CN_FALSE)
	{
		inResult = srCommFuncPoint.inCheckComm(&srTransRec);
	}

	vduiClearBelow(3);

	return d_OK;	
}

//hubing enhance ECR
int get_env_int (char *tag)
{
	int     ret = -1;
	char    buf[64];

    memset (buf, 0, sizeof (buf));
    if ( inCTOSS_GetEnvDB (tag, buf) == d_OK )
    {
        ret = atoi (buf);
    }

	vdDebug_LogPrintf("get_env_int [%s]=[%d]", tag, ret);

    return ret;
}
//hubing enhance ECR
void put_env_int(char *tag, int value)
{
	int     ret = -1;
	char    buf[64];

    memset (buf, 0, sizeof (buf));
    //int2str (buf, value);
    sprintf(buf, "%d", value);
    ret = inCTOSS_PutEnvDB (tag, buf);

	vdDebug_LogPrintf("put_env_int [%s]=[%d] ret[%d]", tag, value, ret);
}


void vdCTOSS_EditTable(void)
{
	BYTE strOut[100];
	BYTE szdatabase[100];
	BYTE sztable[100];
    USHORT usLen;
	USHORT ret;
	char szDispay[50];

	memset(szDispay,0x00,sizeof(szDispay));
	memset(szdatabase,0x00,sizeof(szdatabase));
	memset(sztable,0x00,sizeof(sztable));
	sprintf(szDispay,"EDIT DATABASE");
	CTOS_LCDTClearDisplay();
	
    vdDispTitleString(szDispay);            
	CTOS_LCDTPrintXY(1, 3, "DATABASE NAME:");
	
	usLen = 20;
	CTOS_LCDFontSelectMode(d_FONT_FNT_MODE);
    ret = InputStringAlphaEx(1, 7, 0x00, 0x02, szdatabase, &usLen, 1, d_INPUT_TIMEOUT);
//	if (strTCT.inThemesType == 1)
//	{
//		CTOS_LCDTTFSelect("tahoma.ttf", 0);
//		CTOS_LCDTTFSwichDisplayMode(d_TTF_MODE_VIETNAM);
//	}
//	else
	{
		CTOS_LCDTTFSelect(d_FONT_DEFAULT_TTF, 0);
		CTOS_LCDTTFSwichDisplayMode(d_TTF_MODE_DEFAULT);
	}

	if (ret == d_KBD_CANCEL || 0 == ret )
	{
		return ;
	}
	if(ret>= 1)
    {
    	vduiClearBelow(2);
        vdDebug_LogPrintf("szdatabase[%s].usLen=[%d].",szdatabase,usLen);
		CTOS_LCDTPrintXY(1, 3, "TABLE NAME:");
	
		usLen = 20;
		CTOS_LCDFontSelectMode(d_FONT_FNT_MODE);
	    ret = InputStringAlphaEx(1, 7, 0x00, 0x02, sztable, &usLen, 1, d_INPUT_TIMEOUT);
//		if (strTCT.inThemesType == 1)
//		{
//			CTOS_LCDTTFSelect("tahoma.ttf", 0);
///			CTOS_LCDTTFSwichDisplayMode(d_TTF_MODE_VIETNAM);
//		}
//		else
		{
			CTOS_LCDTTFSelect(d_FONT_DEFAULT_TTF, 0);
			CTOS_LCDTTFSwichDisplayMode(d_TTF_MODE_DEFAULT);
		}
		
		if (ret == d_KBD_CANCEL || 0 == ret )
		{
			return ;
		}
		if(ret>= 1)
	    {
	        vdDebug_LogPrintf("sztable[%s].usLen=[%d].szdatabase[%s]",sztable,usLen,szdatabase);
			inCTOSS_EditTalbe(sztable,szdatabase);
	    }
	
    }
	
}

void vdCTOS_ERMConfig(void)
{
    int inResult;
    USHORT ret, inRet;
    USHORT usLen;
    int shHostIndex = 1;
    BYTE bRet,strOut[30],strtemp[17],key;
    
    inResult = inTCTRead(1);
    if (inResult != d_OK)
        return;
    
   //CTOS_LCDTClearDisplay();
    vdDispTitleString("ERM SETTINGS");
    
    while(1)
    {
    	vduiClearBelow(3);
    	setLCDPrint(3, DISPLAY_POSITION_LEFT, "SIGNATURE CAPTURE");
    	
    	if(strTCT.fSignatureFlag == 0)
    		setLCDPrint(4, DISPLAY_POSITION_LEFT, "DISABLE");
    	if(strTCT.fSignatureFlag == 1)
    		setLCDPrint(4, DISPLAY_POSITION_LEFT, "ENABLE"); 	
    	
    	CTOS_LCDTPrintXY(1, 5, "0-DISABLE      1-ENABLE");
    	
    	strcpy(strtemp,"New:") ; 
    	CTOS_LCDTPrintXY(1, 7, strtemp);
    	memset(strOut,0x00, sizeof(strOut));
    	memset(strtemp, 0x00, sizeof(strtemp));
    	usLen = 10;
    	ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 0, d_INPUT_TIMEOUT);
    	if(ret==d_KBD_ENTER)
    	{
    		if (strOut[0]==0x30 || strOut[0]==0x31)
    		{
    			if(strOut[0] == 0x31)
    			{
    				strTCT.fSignatureFlag= 1;
    			}
    			if(strOut[0] == 0x30)
    			{
    				strTCT.fSignatureFlag = 0;
    			}
    			inTCTSave(1);
    			break;
    		}
    		else
    		{
                vduiClearBelow(2);
                vduiWarningSound();
                vduiDisplayStringCenter(7,"PLEASE SELECT");
                vduiDisplayStringCenter(8,"A VALID VALUE");
                CTOS_Delay(2000);		
    		}
    	}
    	if(ret == d_KBD_CANCEL || ret == 0xFF)
    		break;
    }

    inResult = inTCTRead(1);
    if (inResult != d_OK)
        return;

    while(1)
    {
    	vduiClearBelow(3);
    	setLCDPrint(3, DISPLAY_POSITION_LEFT, "ERM");
    	
    	if(strTCT.byERMMode== 0)
    		setLCDPrint(4, DISPLAY_POSITION_LEFT, "NO");
    	else if(strTCT.byERMMode == 2)
    		setLCDPrint(4, DISPLAY_POSITION_LEFT, "ERM2"); 	
    	else if(strTCT.byERMMode == 3)
    		setLCDPrint(4, DISPLAY_POSITION_LEFT, "ERM3");
		else if(strTCT.byERMMode == 4)
    		setLCDPrint(4, DISPLAY_POSITION_LEFT, "ERM4");
    	
        CTOS_LCDTPrintXY(1, 5, "0-NO        2-ERM2");
        CTOS_LCDTPrintXY(1, 6, "3-ERM3      4-ERM4");
    	
    	strcpy(strtemp,"New:") ; 
    	CTOS_LCDTPrintXY(1, 7, strtemp);
    	memset(strOut,0x00, sizeof(strOut));
    	memset(strtemp, 0x00, sizeof(strtemp));
    	usLen = 10;
    	ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 0, d_INPUT_TIMEOUT);
    	if(ret==d_KBD_ENTER)
    	{
    		if (strOut[0]==0x30 || strOut[0]==0x32 || strOut[0]==0x33 || strOut[0]==0x34)
    		{
    			if(strOut[0] == 0x30)
    			{
    				strTCT.byERMMode=0;
    			}
    			else if(strOut[0] == 0x32)
    			{
    				strTCT.byERMMode=2;
    			}
    			else if(strOut[0] == 0x33)
    			{
    				strTCT.byERMMode=3;
    			}
				else if(strOut[0] == 0x34)
    			{
    				strTCT.byERMMode=4;
    			}
    			inTCTSave(1);
    			break;
    		}
    		else
    		{
                vduiClearBelow(2);
                vduiWarningSound();
                vduiDisplayStringCenter(7,"PLEASE SELECT");
                vduiDisplayStringCenter(8,"A VALID VALUE");
                CTOS_Delay(2000);		
    		}
    	}
    	if(ret == d_KBD_CANCEL || ret == 0xFF)
    		break;
    }	
}

void vdCTOS_ECRConfig(void)
{
    int inResult;
    USHORT ret, inRet;
    USHORT usLen;
    int shHostIndex = 1;
    BYTE bRet,strOut[30],strtemp[17],key;

    vdCTOS_ECRSettings();
	return;
	
   //CTOS_LCDTClearDisplay();
    vdDispTitleString("ECR SETTINGS");
    
    //inResult = inCTOSS_TMSChkBatchEmptyProcess();
    //if(inResult != d_OK)
        //return;
    if(inCheckAllBatchEmtpy() > 0)
    {
        vdDisplayErrorMsg2(1, 8, "BATCH NOT", "EMPTY,SKIPPED.", MSG_TYPE_INFO);
        vdSetErrorMessage("");
        //return (d_FAIL);		
    }

    inResult = inTCTRead(1);
    //if (inResult != d_OK)
    //    return;

    while(1)
    {
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "ECR");
        
        if(strTCT.fECR == 0)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "DISABLE");
        if(strTCT.fECR == 1)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "ENABLE"); 	
        
        CTOS_LCDTPrintXY(1, 5, "0-DISABLE      1-ENABLE");
        
        strcpy(strtemp,"New:") ; 
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        memset(strtemp, 0x00, sizeof(strtemp));
        usLen = 10;
        ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 0, d_INPUT_TIMEOUT);
        if(ret==d_KBD_ENTER)
        {
            if (strOut[0]==0x30 || strOut[0]==0x31)
            {
                if(strOut[0] == 0x31)
                {
                    strTCT.fECR= 1;
                }
                if(strOut[0] == 0x30)
                {
                    strTCT.fECR = 0;
                }
                inTCTSave(1);
                break;
            }
            else
            {
                vduiClearBelow(2);
                vduiWarningSound();
                vduiDisplayStringCenter(7,"PLEASE SELECT");
                vduiDisplayStringCenter(8,"A VALID VALUE");
                CTOS_Delay(2000);		
            }
        }
        if(ret == d_KBD_CANCEL || ret == 0xFF)
            break;
    }

    inResult = inTCTRead(1);
   // if (inResult != d_OK)
   //     return;

    while(1)
    {
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "ECR PORT");
        if(strTCT.byRS232ECRPort == 0)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "NO");
        if(strTCT.byRS232ECRPort == 1)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "COM1");        
        if(strTCT.byRS232ECRPort == 2)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "COM2");
        if(strTCT.byRS232ECRPort == 8)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "USB");  
        
        CTOS_LCDTPrintXY(1, 5, "0-NO        1-COM1");
        CTOS_LCDTPrintXY(1, 6, "2-COM2    8-USB");
        
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 1, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret==1)
        {
            if (strOut[0]==0x30 || strOut[0]==0x31 || strOut[0]==0x32 || strOut[0]==0x38)
            {
                 if(strOut[0] == 0x31)
                 {
                        strTCT.byRS232ECRPort = 1;
                 }
                 if(strOut[0] == 0x30)
                 {
                        strTCT.byRS232ECRPort = 0;
                 }
                 if(strOut[0] == 0x32)
                 {
                        strTCT.byRS232ECRPort = 2;
                 }
                 if(strOut[0] == 0x38)
                 {
                        strTCT.byRS232ECRPort = 8;
                 }
                
                 inRet = inTCTSave(1);
                 
                 vdDebug_LogPrintf(". inTCTSave(%d)",inRet);
                 break;
             }
             else
             {
				 vduiClearBelow(2);
				 vduiWarningSound();
				 vduiDisplayStringCenter(7,"PLEASE SELECT");
				 vduiDisplayStringCenter(8,"A VALID VALUE");
				 CTOS_Delay(2000);		 
            }
        }
        if (ret == d_KBD_CANCEL )
            break ;
    }	

    inResult = inTCTRead(1);
    //if (inResult != d_OK)
    //    return;

    while(1)
    {
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "ECR BAUD RATE");
        if(strTCT.byRS232ECRBaud == 1)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "9600");
        if(strTCT.byRS232ECRBaud == 2)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "115200");         
        
        CTOS_LCDTPrintXY(1, 5, "1-9600      2-115200");
        
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 1, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret==1)
        {
            if (strOut[0]==0x31 || strOut[0]==0x32)
            {
                 if(strOut[0] == 0x31)
                 {
                        strTCT.byRS232ECRBaud = 1;
                 }
                 if(strOut[0] == 0x32)
                 {
                        strTCT.byRS232ECRBaud = 2;
                 }
                
                 inRet = inTCTSave(1);
                 
                 vdDebug_LogPrintf(". inTCTSave(%d)",inRet);
                 break;
             }
             else
             {
				 vduiClearBelow(2);
				 vduiWarningSound();
				 vduiDisplayStringCenter(7,"PLEASE SELECT");
				 vduiDisplayStringCenter(8,"A VALID VALUE");
				 CTOS_Delay(2000);		 
            }
        }
        if (ret == d_KBD_CANCEL )
            break ;
    }	

    inResult = inTCTRead(1);
    //if (inResult != d_OK)
     //   return;

    while(1)
    {
    	vduiClearBelow(3);
    	setLCDPrint(3, DISPLAY_POSITION_LEFT, "ECR CREDIT");
    	
    	if(strTCT.fECRSale == 0)
    		setLCDPrint(4, DISPLAY_POSITION_LEFT, "DISABLE");
    	if(strTCT.fECRSale == 1)
    		setLCDPrint(4, DISPLAY_POSITION_LEFT, "ENABLE");	
    	
    	CTOS_LCDTPrintXY(1, 5, "0-DISABLE      1-ENABLE");
    	
    	strcpy(strtemp,"New:") ; 
    	CTOS_LCDTPrintXY(1, 7, strtemp);
    	memset(strOut,0x00, sizeof(strOut));
    	memset(strtemp, 0x00, sizeof(strtemp));
    	usLen = 10;
    	ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 0, d_INPUT_TIMEOUT);
    	if(ret==d_KBD_ENTER)
    	{
    		if (strOut[0]==0x30 || strOut[0]==0x31)
    		{
    			if(strOut[0] == 0x31)
    			{
    				strTCT.fECRSale= 1;
    			}
    			if(strOut[0] == 0x30)
    			{
    				strTCT.fECRSale = 0;
    			}
    			inTCTSave(1);
    			break;
    		}
    		else
    		{
    			vduiClearBelow(2);
    			vduiWarningSound();
    			vduiDisplayStringCenter(7,"PLEASE SELECT");
    			vduiDisplayStringCenter(8,"A VALID VALUE");
    			CTOS_Delay(2000);		
    		}
    	}
    	if(ret == d_KBD_CANCEL || ret == 0xFF)
    		break;
    }
    
    inResult = inTCTRead(1);
    //if (inResult != d_OK)
    //	return;

    while(1)
    {
    	vduiClearBelow(3);
    	setLCDPrint(3, DISPLAY_POSITION_LEFT, "ECR DEBIT");
    	
    	if(strTCT.fECRDebit== 0)
    		setLCDPrint(4, DISPLAY_POSITION_LEFT, "DISABLE");
    	if(strTCT.fECRDebit== 1)
    		setLCDPrint(4, DISPLAY_POSITION_LEFT, "ENABLE");	
    	
    	CTOS_LCDTPrintXY(1, 5, "0-DISABLE      1-ENABLE");
    	
    	strcpy(strtemp,"New:") ; 
    	CTOS_LCDTPrintXY(1, 7, strtemp);
    	memset(strOut,0x00, sizeof(strOut));
    	memset(strtemp, 0x00, sizeof(strtemp));
    	usLen = 10;
    	ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 0, d_INPUT_TIMEOUT);
    	if(ret==d_KBD_ENTER)
    	{
    		if (strOut[0]==0x30 || strOut[0]==0x31)
    		{
    			if(strOut[0] == 0x31)
    			{
    				strTCT.fECRDebit= 1;
    			}
    			if(strOut[0] == 0x30)
    			{
    				strTCT.fECRDebit = 0;
    			}
    			inTCTSave(1);
    			break;
    		}
    		else
    		{
    			vduiClearBelow(2);
    			vduiWarningSound();
    			vduiDisplayStringCenter(7,"PLEASE SELECT");
    			vduiDisplayStringCenter(8,"A VALID VALUE");
    			CTOS_Delay(2000);		
    		}
    	}
    	if(ret == d_KBD_CANCEL || ret == 0xFF)
    		break;
    }
    
    inResult = inTCTRead(1);
    //if (inResult != d_OK)
    //	return;

    while(1)
    {
    	vduiClearBelow(3);
    	setLCDPrint(3, DISPLAY_POSITION_LEFT, "ECR DINERS");
    	
    	if(strTCT.fECRDiners== 0)
    		setLCDPrint(4, DISPLAY_POSITION_LEFT, "DISABLE");
    	if(strTCT.fECRDiners== 1)
    		setLCDPrint(4, DISPLAY_POSITION_LEFT, "ENABLE");	
    	
    	CTOS_LCDTPrintXY(1, 5, "0-DISABLE      1-ENABLE");
    	
    	strcpy(strtemp,"New:") ; 
    	CTOS_LCDTPrintXY(1, 7, strtemp);
    	memset(strOut,0x00, sizeof(strOut));
    	memset(strtemp, 0x00, sizeof(strtemp));
    	usLen = 10;
    	ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 0, d_INPUT_TIMEOUT);
    	if(ret==d_KBD_ENTER)
    	{
    		if (strOut[0]==0x30 || strOut[0]==0x31)
    		{
    			if(strOut[0] == 0x31)
    			{
    				strTCT.fECRDiners= 1;
    			}
    			if(strOut[0] == 0x30)
    			{
    				strTCT.fECRDiners = 0;
    			}
    			inTCTSave(1);
    			break;
    		}
    		else
    		{
    			vduiClearBelow(2);
    			vduiWarningSound();
    			vduiDisplayStringCenter(7,"PLEASE SELECT");
    			vduiDisplayStringCenter(8,"A VALID VALUE");
    			CTOS_Delay(2000);		
    		}
    	}
    	if(ret == d_KBD_CANCEL || ret == 0xFF)
    		break;
    }
    
    inResult = inTCTRead(1);
   // if (inResult != d_OK)
    //	return;

    while(1)
    {
    	vduiClearBelow(3);
    	setLCDPrint(3, DISPLAY_POSITION_LEFT, "ECR BANCNET");
    	
    	if(strTCT.fECRBancnet== 0)
    		setLCDPrint(4, DISPLAY_POSITION_LEFT, "DISABLE");
    	if(strTCT.fECRBancnet== 1)
    		setLCDPrint(4, DISPLAY_POSITION_LEFT, "ENABLE");	
    	
    	CTOS_LCDTPrintXY(1, 5, "0-DISABLE      1-ENABLE");
    	
    	strcpy(strtemp,"New:") ; 
    	CTOS_LCDTPrintXY(1, 7, strtemp);
    	memset(strOut,0x00, sizeof(strOut));
    	memset(strtemp, 0x00, sizeof(strtemp));
    	usLen = 10;
    	ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 0, d_INPUT_TIMEOUT);
    	if(ret==d_KBD_ENTER)
    	{
    		if (strOut[0]==0x30 || strOut[0]==0x31)
    		{
    			if(strOut[0] == 0x31)
    			{
    				strTCT.fECRBancnet= 1;
    			}
    			if(strOut[0] == 0x30)
    			{
    				strTCT.fECRBancnet = 0;
    			}
    			inTCTSave(1);
    			break;
    		}
    		else
    		{
    			vduiClearBelow(2);
    			vduiWarningSound();
    			vduiDisplayStringCenter(7,"PLEASE SELECT");
    			vduiDisplayStringCenter(8,"A VALID VALUE");
    			CTOS_Delay(2000);		
    		}
    	}
    	if(ret == d_KBD_CANCEL || ret == 0xFF)
    		break;
    }
    
    inResult = inTCTRead(1);
    //if (inResult != d_OK)
    //	return;

	vdCTOS_uiRestart(); // Need to restart
}


void vdDisplayEnvMenuOption(char *pszTag, char *pszValue)
{
	/*display current tag & value*/
	vduiClearBelow(3);
	setLCDPrint(3, DISPLAY_POSITION_LEFT, pszTag);
	setLCDPrint(4, DISPLAY_POSITION_LEFT, pszValue);
		
	/*F1-F4 func display*/
	CTOS_LCDTPrintXY(1, 5, "1 - NEW");
	CTOS_LCDTPrintXY(20-10, 5, "2 - FIND");

	CTOS_LCDTPrintXY(1, 6, "3 - EDIT");
	CTOS_LCDTPrintXY(20-9, 6, "4 - DEL");

	CTOS_LCDTPrintXY(1, 8, "7 - PREV");
	CTOS_LCDTPrintXY(20-9, 8, "9 - NEXT");
}

void vdCTOS_EditEnvParamDB(void)
{
    BYTE bRet,strOut[30],strtemp[17],key;
    BYTE szInputBuf[24+1];
    BYTE szIntComBuf[2];
    BYTE szPhNoBuf[9];
    BYTE szExtNoBuf[4];
    int inResult;
    USHORT ret;
    USHORT usLen;


	unsigned char keyval;
    BOOL isKey;

	char szCurrTag[64+1];
	char szCurrValue[128+1];

	char szNewTag[64+1];
	char szNewValue[128+1];

	int inEnvTotal = 0;
	int inIdx = 0;

	int inFindIdx = 0;
    char szFindTag[64+1];

	/*title*/
   //CTOS_LCDTClearDisplay();
    vdDispTitleString("EDIT EMV PARAM");

	vdDebug_LogPrintf("=====vdCTOS_EditEnvParamDB=====");
	
	inEnvTotal = inCTOSS_EnvTotalDB();

	vdDebug_LogPrintf("inCTOSS_EnvTotal inEnvTotal[%d]", inEnvTotal);
	
	//if (0 == inEnvTotal)
		//return;
	
	inIdx = 1;

	/*retrive Env data by index*/
	memset(szCurrTag, 0, sizeof(szCurrTag));
	memset(szCurrValue, 0, sizeof(szCurrValue));
	if (inEnvTotal > 0)
	{
		inCTOSS_GetEnvByIdxDB(inIdx, szCurrTag, szCurrValue);
	}
	else
	{
		strcpy(szCurrTag, "__________");
		strcpy(szCurrValue, "__________");
	}
	//vdDebug_LogPrintf("inIdx %d szCurrTag [%s] szCurrValue [%s]", inIdx, szCurrTag, szCurrValue);

	vdDisplayEnvMenuOption(szCurrTag, szCurrValue);

	/*set idle timout*/
	CTOS_TimeOutSet(TIMER_ID_3, 3000);
	
    while(1)
    {
    	/*check timeout*/
		if (CTOS_TimeOutCheck(TIMER_ID_3) == d_YES)
        {      
            return;
        }

		keyval = 0;
		
		/*wait for user*/
		CTOS_KBDInKey(&isKey);
        if (isKey)
		{ //If isKey is TRUE, represent key be pressed //
			vduiLightOn();
            //Get a key from keyboard //
            CTOS_KBDGet(&keyval);

			/*set idle timout agian*/
			CTOS_TimeOutSet(TIMER_ID_3, 3000);
        }

		switch (keyval)
		{
			case d_KBD_1: //New
				memset(szNewTag, 0, sizeof(szNewTag));
				memset(szNewValue, 0, sizeof(szNewValue));
						
				strcpy(strtemp,"TAG:") ; 
				CTOS_LCDTPrintXY(1, 7, strtemp);
				memset(strOut,0x00, sizeof(strOut));
				memset(strtemp, 0x00, sizeof(strtemp));
				usLen = 20;
				ret = InputStringAlpha(1, 8, 0x00, 0x02, strOut, &usLen, 0, d_INPUT_TIMEOUT);
				if(ret==d_KBD_ENTER)
				{
					if(strlen(strOut)>0)
					{
						strcpy(szNewTag, strOut);
								
						//vdDebug_LogPrintf("inIdx %d szNewTag [%s] szNewValue [%s]", inIdx, szNewTag, szNewValue);
						//inCTOSS_PutEnv(szNewTag, szNewValue);
						//break;
					}
					else
					{
					}
				}

				/*new tag not set, break*/
				if (strlen(szNewTag) <= 0)
					break;

				vduiClearBelow(7);
				strcpy(strtemp,"VAL:") ; 
				CTOS_LCDTPrintXY(1, 7, strtemp);
				memset(strOut,0x00, sizeof(strOut));
				memset(strtemp, 0x00, sizeof(strtemp));
				usLen = 20;
				ret = InputStringAlpha(1, 8, 0x00, 0x02, strOut, &usLen, 0, d_INPUT_TIMEOUT);
				if(ret==d_KBD_ENTER)
				{
					if(strlen(strOut)>0)
					{
						strcpy(szNewValue, strOut);
								
						//vdDebug_LogPrintf("inIdx %d szNewTag [%s] szNewValue [%s]", inIdx, szNewTag, szNewValue);
						//inCTOSS_PutEnv(szNewTag, szNewValue);
						//break;
					}
					else
					{
					}
				}

				/*new tag not set, break*/
				if (strlen(szNewValue) <= 0)
					break;

				
				//vdDebug_LogPrintf("inIdx %d szNewTag [%s] szNewValue [%s]", inIdx, szNewTag, szNewValue);
				inCTOSS_PutEnvDB(szNewTag, szNewValue);
				inEnvTotal = inCTOSS_EnvTotalDB();
				inIdx = inEnvTotal;

				/*update current display*/
				memset(szCurrTag, 0, sizeof(szCurrTag));
				memset(szCurrValue, 0, sizeof(szCurrValue));
				if (inEnvTotal > 0)
				{
    				inCTOSS_GetEnvByIdxDB(inIdx, szCurrTag, szCurrValue);
				}
				else
				{
					strcpy(szCurrTag, "__________");
					strcpy(szCurrValue, "__________");
				}

				vdDisplayEnvMenuOption(szCurrTag, szCurrValue);
				
				break;
				
			case d_KBD_2: //Find
				memset(szNewTag, 0, sizeof(szNewTag));
				memset(szNewValue, 0, sizeof(szNewValue));
						
				strcpy(strtemp,"TAG:") ; 
				CTOS_LCDTPrintXY(1, 7, strtemp);
				memset(strOut,0x00, sizeof(strOut));
				memset(strtemp, 0x00, sizeof(strtemp));
				usLen = 20;
				ret = InputStringAlpha(1, 8, 0x00, 0x02, strOut, &usLen, 0, d_INPUT_TIMEOUT);
				if(ret==d_KBD_ENTER)
				{
					if(strlen(strOut)>0)
					{
						//strcpy(szNewValue, strOut);
						//strcpy(szNewTag, szCurrTag);
						strcpy(szFindTag, strOut);

						inCTOSS_GetEnvIdxDB(szFindTag, &inFindIdx);
						//vdDebug_LogPrintf("inIdx %d szFindTag [%s]", inFindIdx, szFindTag);
						inIdx = inFindIdx;
					}
					else
					{

					}
				}
				/*update current display*/
				memset(szCurrTag, 0, sizeof(szCurrTag));
				memset(szCurrValue, 0, sizeof(szCurrValue));
				if (inEnvTotal > 0)
				{
    				inCTOSS_GetEnvByIdxDB(inIdx, szCurrTag, szCurrValue);
				}
				else
				{
					strcpy(szCurrTag, "__________");
					strcpy(szCurrValue, "__________");
				}

				vdDisplayEnvMenuOption(szCurrTag, szCurrValue);
				break;
				
			case d_KBD_3: //Edit
				
				memset(szNewTag, 0, sizeof(szNewTag));
				memset(szNewValue, 0, sizeof(szNewValue));
				
				strcpy(strtemp,"New:") ; 
				CTOS_LCDTPrintXY(1, 7, strtemp);
				memset(strOut,0x00, sizeof(strOut));
				memset(strtemp, 0x00, sizeof(strtemp));
				usLen = 20;
				ret = InputStringAlpha(1, 8, 0x00, 0x02, strOut, &usLen, 0, d_INPUT_TIMEOUT);
				if(ret==d_KBD_ENTER)
				{
					if(strlen(strOut)>0)
					{
						strcpy(szNewValue, strOut);
						strcpy(szNewTag, szCurrTag);
								
						//vdDebug_LogPrintf("inIdx %d szNewTag [%s] szNewValue [%s]", inIdx, szNewTag, szNewValue);
						inCTOSS_PutEnvDB(szNewTag, szNewValue);
						//break;
					}
					else
					{
						vduiClearBelow(4);
				
						vduiDisplayStringCenter(5,"NO VALUE SET");
						//vduiDisplayStringCenter(7,"PREV[UP] NEXT[DOWN]");
						//vduiDisplayStringCenter(7,"PABX?");
						//vduiDisplayStringCenter(8,"NO[X] YES[OK]");
						//key=struiGetchWithTimeOut();
					}
				}

				/*update current display*/
				memset(szCurrTag, 0, sizeof(szCurrTag));
				memset(szCurrValue, 0, sizeof(szCurrValue));
				if (inEnvTotal > 0)
				{
    				inCTOSS_GetEnvByIdxDB(inIdx, szCurrTag, szCurrValue);
				}
				else
				{
					strcpy(szCurrTag, "__________");
					strcpy(szCurrValue, "__________");
				}

				vdDisplayEnvMenuOption(szCurrTag, szCurrValue);
				break;
				
			case d_KBD_4: //Del
				inCTOSS_DelEnvDB(szCurrTag);
				inEnvTotal = inCTOSS_EnvTotalDB();
				inIdx = 1;
				/*retrive Env data by index*/
				memset(szCurrTag, 0, sizeof(szCurrTag));
				memset(szCurrValue, 0, sizeof(szCurrValue));
				if (inEnvTotal > 0)
				{
					inCTOSS_GetEnvByIdxDB(inIdx, szCurrTag, szCurrValue);
				}
				else
				{
					strcpy(szCurrTag, "__________");
					strcpy(szCurrValue, "__________");
				}
				//vdDebug_LogPrintf("inIdx %d szCurrTag [%s] szCurrValue [%s]", inIdx, szCurrTag, szCurrValue);

				vdDisplayEnvMenuOption(szCurrTag, szCurrValue);
				break;
				
			case d_KBD_7: //prev
				inIdx--;
				if (inIdx < 1)
					inIdx = inEnvTotal;
				/*retrive Env data by index*/
    			memset(szCurrTag, 0, sizeof(szCurrTag));
				memset(szCurrValue, 0, sizeof(szCurrValue));
				if (inEnvTotal > 0)
				{
    				inCTOSS_GetEnvByIdxDB(inIdx, szCurrTag, szCurrValue);
				}
				else
				{
					strcpy(szCurrTag, "__________");
					strcpy(szCurrValue, "__________");
				}
				//vdDebug_LogPrintf("inIdx %d szCurrTag [%s] szCurrValue [%s]", inIdx, szCurrTag, szCurrValue);

				vdDisplayEnvMenuOption(szCurrTag, szCurrValue);
				break;
				
			case d_KBD_9: //next
				inIdx++;
				if (inIdx > inEnvTotal)
					inIdx = 1;
				memset(szCurrTag, 0, sizeof(szCurrTag));
				memset(szCurrValue, 0, sizeof(szCurrValue));
				if (inEnvTotal > 0)
				{
    				inCTOSS_GetEnvByIdxDB(inIdx, szCurrTag, szCurrValue);
				}
				else
				{
					strcpy(szCurrTag, "__________");
					strcpy(szCurrValue, "__________");
				}
				//vdDebug_LogPrintf("inIdx %d szCurrTag [%s] szCurrValue [%s]", inIdx, szCurrTag, szCurrValue);

				vdDisplayEnvMenuOption(szCurrTag, szCurrValue);
				break;
				
			case d_KBD_CANCEL: //exit
				return ;
				//break;
			default:
				break;
		}
	}

}

void vdCTOS_AuthenticationType(void)
{
	int inKeySelected = 0;
	char szHeaderString[50] = {0};
	char szChoiceMsg[1024] = {0};
	int bHeaderAttr = 0x01+0x04, key=0; 
	BYTE szTemp1[20 + 1] = {0};
	int inValue = 0;
	int inAuthType;

   //CTOS_LCDTClearDisplay();
    vdDispTitleString("SET GSM AUTH TYPE");

	inAuthType = get_env_int("GSMAUTHTYPE");
	
	memset(szHeaderString, 0x00, sizeof(szHeaderString));
	memset(szChoiceMsg, 0x00, sizeof(szChoiceMsg));
	memset(szTemp1, 0x00, sizeof(szTemp1));
	
	switch(inAuthType)
	{
		case AUTH_TYPE_AUTO:
			strcpy(szTemp1, "AUTO");
			break;
		case AUTH_TYPE_PAP:
			strcpy(szTemp1, "PAP");
			break;
		case AUTH_TYPE_CHAP:
			strcpy(szTemp1, "CHAP");
			break;
		default:
			strcpy(szTemp1, "NONE");
			break;
	}

	sprintf(szHeaderString, "AUTH TYPE: %s", szTemp1);	
	strcpy((char*)szChoiceMsg, "AUTO \nPAP \nCHAP ");
	inKeySelected = MenuDisplay(szHeaderString,strlen(szHeaderString), bHeaderAttr, 1, 1, szChoiceMsg, TRUE);
	
	vdDebug_LogPrintf("::AUTH TYPE::inSetMenuSelection::inKeySelected[%d]", inKeySelected);
	
	if (inKeySelected != d_KBD_CANCEL)
	{
		inValue = inKeySelected - 1; // (0-AUTO, 1-PAP, 2-CHAP)
		put_env_int("GSMAUTHTYPE", inValue);		
	}
	
	vdDebug_LogPrintf("::AUTH TYPE::inKeySelected[%d]::szHeaderString[%s]::inValue[%d]", inKeySelected, szHeaderString, inValue);
       
    return ;
}

void vdCTOS_PingIPAddress(void)
{
	int inCPTNumRecs = 0,
	i = 0,
	inInput = 0,
	inCommMode =-1;
	BYTE strtemp[17];
	BYTE strOut[30] = {0};
	USHORT usLen = 0,
	inResult = 0,
	ret = 0;
	int x = 0;
	int inRetry = 3;

	inTCTRead(1);
	inCPTRead(MCC_HOST);

	CTOS_LCDTClearDisplay();
	vdDispTitleString("PING IP ADDRESS");
	
	while(1)
	{
		vduiClearBelow(3);
		setLCDPrint(3, DISPLAY_POSITION_LEFT, "ENTER IP ADDRESS:");
		
		strcpy(strtemp,"IP:") ;
		CTOS_LCDTPrintXY(1, 7, strtemp);
		memset(strtemp, 0x00, sizeof(strtemp));
		memset(strOut,0x00, sizeof(strOut));
		ret= struiApiGetStringSub(strtemp, 1,  8, strOut, MODE_IPADDRESS, 7, IP_LEN);		
		if(ret==d_KBD_ENTER)
		{
				if ( strCPT.inCommunicationMode == ETHERNET_MODE)
				{
					inResult = CTOS_EthernetOpenEx();
					vdDebug_LogPrintf(". CTOS_EthernetOpenEx Ret=(%04x)",inResult);
					{//fix power on, first time connot connect
						CTOS_Delay(100);
						inResult = CTOS_EthernetClose();
						vdDebug_LogPrintf(". CTOS_EthernetClose Ret=(%04x)",inResult);
						CTOS_Delay(100);
						inResult = CTOS_EthernetOpenEx();
						vdDebug_LogPrintf(". CTOS_EthernetOpenEx2 Ret=(%04x)",inResult);
					}	
					
					while(x < inRetry)
					{
						inResult = 0;
						setLCDPrint(5, DISPLAY_POSITION_LEFT, "PINGING IP...");
						CTOS_Delay(500);
						//android-removed
						//inResult = CTOS_EthernetPing(strOut,strlen(strOut));
						//end
						vdDebug_LogPrintf("AAA - ETHERNENT inResult[%05x][%s]", inResult, strOut);
						x++;
					}
					
					CTOS_EthernetClose();
				
				}
				else if ( strCPT.inCommunicationMode == WIFI_MODE)
				{
					while(x < inRetry)
					{
						inResult = 0;
						setLCDPrint(5, DISPLAY_POSITION_LEFT, "PINGING IP...");
						CTOS_Delay(500);
						//android-removed
						//inResult = CTOS_WifiPing(strOut,strlen(strOut));
						//end
						vdDebug_LogPrintf("AAA - WIFI inResult[%05x][%s]", inResult, strOut);
						x++;
					}
				}
				else if ( strCPT.inCommunicationMode == GPRS_MODE)
				{
				while(x < inRetry)
					{
						inResult = 0;
						setLCDPrint(5, DISPLAY_POSITION_LEFT, "PINGING IP...");
						CTOS_Delay(500);
						//Result = inCTOSS_GPRSPing(strOut);
						vdDebug_LogPrintf("AAA - GPRS inResult[%05x][%s] | Retry[%d]", inResult, strOut, x);
						x++;
					}        
				}
				else
					break;

				if (inResult == 00000)
				{
					setLCDPrint(8, DISPLAY_POSITION_LEFT, "PING IP SUCCESS");  
					break;
				}
				else
				{
					setLCDPrint(8, DISPLAY_POSITION_LEFT, "PING IP FAILED"); 
					break;
				}
				
				
		} 	
		
		if(ret == d_KBD_CANCEL)
			break;
	}
	
	CTOS_Delay(3000);
	CTOS_Beep();
	CTOS_Beep();
}

void vdCTOS_uiIDLESleepMode(void)
{
    USHORT usRet1;
    BYTE  bPercentage ;
	UCHAR usRet;
    DWORD pdwStatus ;

	usRet1 = CTOS_PowerSource(&usRet);
	vdDebug_LogPrintf("CTOS_PowerSource=[%x]..usRet=[%d]..inSleepflag=[%d]....",usRet1,usRet,inSleepflag);

	if ((d_OK == usRet1) && (usRet == d_PWRSRC_BATTERY))
	{
		if (inSleepflag == 0)
		{
			vdDebug_LogPrintf("vdSetsysPowerLowSpeed......");
			//CTOS_BackLightSet (d_BKLIT_LCD, d_OFF);
			//CTOS_BackLightSet (d_BKLIT_KBD, d_OFF);
			///vdSetsysPowerLowSpeed();
			//CTOS_PowerMode(d_PWR_SLEEP_MODE);
			CTOS_PowerMode(d_PWR_STANDBY_MODE);
			inSleepflag = 1;

			vdCTOS_uiIDLEWakeUpSleepMode();
		}
	}
	else
		vdCTOS_uiIDLEWakeUpSleepMode();
	return;
	
}

int inCTOSS_CheckBatteryChargeStatus(void)
{
	USHORT usRet1;
	BYTE  bPercentage ;
	UCHAR usRet;
	DWORD pdwStatus ;

	usRet1 = CTOS_PowerSource(&usRet);
	//vdDebug_LogPrintf("CTOS_PowerSource=[%x]..usRet=[%d]......",usRet1,usRet);
	if ((d_OK == usRet1) && (usRet == d_PWRSRC_BATTERY))
	{
		vdDebug_LogPrintf("inCTOSS_CheckBatteryChargeStatus NO"); 
		return d_NO;
	}
	
	vdDebug_LogPrintf("inCTOSS_CheckBatteryChargeStatus OK"); 
	return d_OK;
	
}

void vdCTOS_uiIDLEWakeUpSleepMode(void)
{
	//vdDebug_LogPrintf("vdCTOS_uiIDLEWakeUpSleepMode,inSleepflag=[%d]......",inSleepflag);
	if (inSleepflag == 1)
	{
		//vdDebug_LogPrintf("vdSetsysPowerHighSpeed......");
		//vdSetsysPowerHighSpeed();
		CTOS_BackLightSet (d_BKLIT_LCD, d_ON);
		CTOS_BackLightSet (d_BKLIT_KBD, d_ON);
		inSleepflag = 0;
		vdSetIdleEvent(1);
	}
	return;
}

int vdDisplayIdleEMVApp(int inIdleSaleType)
{
	int inMenuNumRecs = 0,
		inCtr = 0,
		inKey = 0,
		inHeaderAttr = 0x01+0x04;

	char szMenuItems[9000] = {0},
		szBuf[200] = {0},
		szHeader[20] = {0};

	vdDebug_LogPrintf("--vdDisplayIdleEMVApp--");
	vdDebug_LogPrintf("inIdleSaleType[%d]", inIdleSaleType);

	inMenuNumRecs = inReadDynamicMenu(inIdleSaleType);

	vdDebug_LogPrintf("inMenuNumRecs[%d]", inMenuNumRecs);
	
	strcpy(szHeader, "SELECT TRANSACTION");
	memset(szMenuItems, 0, sizeof(szMenuItems));
	for(inCtr = 0; inCtr < inMenuNumRecs; ++inCtr)
	{
		strcat(szMenuItems, strDynamicMenu[inCtr].szButtonItemLabel);
		if((strlen(strDynamicMenu[inCtr].szButtonItemLabel) > 0) &&
			(inCtr != inMenuNumRecs))
			strcat(szMenuItems, "\n");

	}

	vdDebug_LogPrintf("szMenuItems[%s]", szMenuItems);
	
	inCtr = strlen(szMenuItems) - 1;
	szMenuItems[inCtr] = 0x00;
	
	inKey = MenuDisplay(szHeader, strlen(szHeader), inHeaderAttr, 1, 1, szMenuItems, TRUE);

	vdDebug_LogPrintf("inKey[%d]", inKey);
	
	if(inKey == 0xFF)
	{
		CTOS_LCDTClearDisplay();
		setLCDPrint(1, DISPLAY_POSITION_CENTER, "WRONG INPUT!!!");
		vduiWarningSound();
		return -1;
	}

	if(inKey > 0)
	{
		if(inKey == d_KBD_CANCEL)
			return -1;

        vdDebug_LogPrintf("strDynamicMenu.szButtonItemFunc[%s]", strDynamicMenu[inKey-1].szButtonItemFunc);
        vdDebug_LogPrintf("strDynamicMenu.szSubMenuName[%s]", strDynamicMenu[inKey-1].szSubMenuName);
        
        if(strlen(strDynamicMenu[inKey-1].szSubMenuName) > 0)
        {
            inMenuNumRecs = inReadDynamicSubMenu(inIdleSaleType); 
            strcpy(szHeader, "SALE MENU");
            memset(szMenuItems, 0, sizeof(szMenuItems));
            for(inCtr = 0; inCtr < inMenuNumRecs; ++inCtr)
            {
                strcat(szMenuItems, strDynamicMenu[inCtr].szButtonItemLabel);
                if((strlen(strDynamicMenu[inCtr].szButtonItemLabel) > 0) && (inCtr != inMenuNumRecs))
                strcat(szMenuItems, "\n");
            }
            
            inCtr = strlen(szMenuItems) - 1;
            szMenuItems[inCtr] = 0x00;
            
            inKey = MenuDisplay(szHeader, strlen(szHeader), inHeaderAttr, 1, 1, szMenuItems, TRUE);
            	vdDebug_LogPrintf("inKey[%d]", inKey);
	
            if(inKey == 0xFF)
            {
               //CTOS_LCDTClearDisplay();
                setLCDPrint(1, DISPLAY_POSITION_CENTER, "WRONG INPUT!!!");
                vduiWarningSound();
                return -1;
            }

            if(inKey > 0)
            {
                if(inKey == d_KBD_CANCEL)
                    return -1;
                
                inCTOSS_ExeFunction(strDynamicMenu[inKey-1].szButtonItemFunc);
                inKey = d_OK;
            }
        }
        else
            inCTOSS_ExeFunction(strDynamicMenu[inKey-1].szButtonItemFunc);
	}
}

int inSelectIdleApps(void)
{
	int inRet;
	char szDebug[40 + 1]={0};
    char szSaleType[12][19+1];
    char szSaleMenu[1024];
	char szHeaderString[24+1];
	int bHeaderAttr = 0x01+0x04, key=0; 
	int inLoop=0;
	int inIndex=0;
	int inSaleType = 0;
	BOOL isValid = TRUE;
    int inResult=0x0000;
	int inNumRecs =0;
	int i = 0;
	char szSelectedMenu[100];
	
	vdDebug_LogPrintf("--inSelectIdleApps--");
	vdDebug_LogPrintf("inMenuid[%d]", strTCT.inMenuid);
	vdDebug_LogPrintf("inMenuidNHProfileMenu[%d]", strTCT.inMenuidNHProfileMenu);

    memset(szHeaderString, 0x00, sizeof(szHeaderString));
	memset(szSaleType, 0x00, sizeof(szSaleType));
	memset(szSaleMenu, 0x00, sizeof(szSaleMenu));

	inDeleteToSelectionMenu();
	
	inProcessSelectionMenu();
	
	inGetSelectionMenu(&inNumRecs);	

	vdDebug_LogPrintf("inGetSelectionMenu.inNumRecs[%d]", inNumRecs);

	for (i=0; i<inNumRecs; i++)
	{
		if (strTrxMenu[i].szButtonItemLabel[0] != 0)
		{
			inIndex=i;
			strcat((char *)szSaleMenu, strTrxMenu[i].szButtonItemLabel);

			
			if (strTrxMenu[i+1].szButtonItemLabel[0] != 0)
				strcat((char *)szSaleMenu, (char *)" \n");			
		}
	}

	vdDebug_LogPrintf("inGetSelectionMenu.szSaleMenu[%s]", szSaleMenu);
		
    strcpy(szHeaderString, "SELECT APP"); // Header	
	
	vdDebug_LogPrintf("inIndex[%d] | szHeaderString[%s] | szSaleMenu[%s]", inIndex, szHeaderString, szSaleMenu);

	if (inNumRecs > 0)
	{
		
		key = MenuDisplay(szHeaderString,strlen(szHeaderString), bHeaderAttr, 1, 1, szSaleMenu, TRUE);
		
		vdDebug_LogPrintf("key[%d]", key);

		if (key == d_KBD_CANCEL)
		{
			//vdDisplayErrorMsg(1, 8, "USER CANCEL");
	        return d_NO;
		}
		
	    if (key > 0)
	    {

	      memset(szSelectedMenu, 0x00, sizeof(szSelectedMenu));
		  strcpy(szSelectedMenu, strTrxMenu[key-1].szButtonItemLabel);
	      vdDebug_LogPrintf("szSelectedMenu", strTrxMenu[key-1]);
		
	      if (key == d_KBD_CANCEL)
	      {
	      	  //vdDisplayErrorMsg(1, 8, "USER CANCEL");
	          return d_NO;
	      }
		  
	      if((memcmp(szSelectedMenu, "MC/VISA/JCB/UPI", strlen(szSelectedMenu)) == 0) ||
		  	(memcmp(szSelectedMenu, "MC/VISA", strlen(szSelectedMenu)) == 0) ||
		  	(memcmp(szSelectedMenu, "SALE", strlen(szSelectedMenu)) == 0) ||
		  	(memcmp(szSelectedMenu, "INSTALLMENT", strlen(szSelectedMenu)) == 0))
	      {
	           inSaleType = 1;	   	
	      }		  
	      else if(memcmp(szSelectedMenu, "BANCNET", strlen(szSelectedMenu)) == 0)
	      {
	           inSaleType = 2;	   	
	      }
		  else
		  {
		  	isValid = FALSE;
		  	vdDisplayErrorMsg(1, 8, "INVALID TRANS");
		  }
	    } 
		else
		{
			isValid = FALSE;
		   vdDisplayErrorMsg(1, 8, "INVALID TRANS");
		}
		
		switch (inSaleType)
		{		
			case 1: // Credit
				inSelectedIdleAppsHost = MCC_HOST;
				vdDisplayIdleEMVApp(IDLE_CREDIT_AND_BANCNET);			
				break;
			case 2: // Bancnet
				inSelectedIdleAppsHost = BANCNET_HOST;
				vdDisplayIdleEMVApp(IDLE_BANCNET);			
				break;
			default:
				isValid = FALSE;
				vdDisplayErrorMsg(1, 8, "INVALID TRANS");
				break;
		}

		vdDebug_LogPrintf("isValid[%d]", isValid);
		vdDebug_LogPrintf("inSaleType[%d]", inSaleType);
		vdDebug_LogPrintf("inSelectedIdleAppsHost[%d]", inSelectedIdleAppsHost);
	}
	else
	{
		isValid = FALSE;
		vdDisplayErrorMsg(1, 8, "NO APP MENU CREATED");
	}
		
	
	if (!isValid)
		return d_NO;
	else
		return d_OK;
}

void vdCTOS_TMSSetting2(void)
{
	BYTE strOut[30],strtemp[17];
	BYTE szInputBuf[5],count;
    int inResult;
    int ret;
	USHORT usLen;
    BYTE szSerialNum[17 + 1];
	int bSIMSlot=0;
	USHORT usRes=0;

	int inCommMode = -1;
	BOOL fValid = TRUE;

	vdDebug_LogPrintf("--vdCTOS_TMSSetting--");
/*
	// Input library function call -- sidumili
	vdCTOS_InputTMSSetting();
	return;
*/
   //CTOS_LCDTClearDisplay();
    vdDispTitleString("TMS SETTINGS");

	inTCPRead(1);
	
    memset(szSerialNum, 0x00, sizeof(szSerialNum));
	CTOS_CTMSGetConfig(d_CTMS_SERIALNUM, szSerialNum);
    while(1)
    {   
        vduiClearBelow(3);
		setLCDPrint(3, DISPLAY_POSITION_LEFT, "SERIAL NO.");
		setLCDPrint(4, DISPLAY_POSITION_LEFT, szSerialNum);
		
		strcpy(strtemp,"New:") ;
		CTOS_LCDTPrintXY(1, 7, strtemp);
		memset(strOut,0x00, sizeof(strOut));
		usLen = 32;
		//ret= InputStringAlpha(1, 8, 0x00, 0x02, strOut, &usLen, 16, d_INPUT_TIMEOUT);
		ret = shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 16, 0, d_INPUT_TIMEOUT);
		if(ret>=1)
		{
			//len = strlen(szSerialNum);
			strcpy(szSerialNum,"0000000000000000");
			memcpy(&szSerialNum[16-usLen],strOut,usLen);
			vdDebug_LogPrintf("szSerialNum=[%s].tmpbuf=[%s].lenght[%d]",szSerialNum,strOut, strlen(strOut));

			//memcpy(strTCP.szAPN, strOut,strlen(strOut));
			//strTCP.szAPN[strlen(strOut)]=0;
			CTOS_CTMSSetConfig(d_CTMS_SERIALNUM, szSerialNum);
            CTOS_CTMSSetConfig(d_CTMS_RECOUNT, &count);
			
			break;
		}	
		if(ret == d_KBD_CANCEL)
			break;
    }	

	/*

    inResult = inTCTRead(1);
    if(inResult != d_OK)
        return;	
    while(1)
    {   
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "TMS GAP");
        memset(szInputBuf, 0x00, sizeof(szInputBuf));
        sprintf(szInputBuf, "%d", strTCT.usTMSGap);
        setLCDPrint(4, DISPLAY_POSITION_LEFT, szInputBuf);
        
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 3, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret>=1)
        {
            strTCT.usTMSGap=atoi(strOut);      
            vdMyEZLib_LogPrintf("strTCT.usTMSGap %d",strTCT.usTMSGap);
			inResult = inTCTSave(1);
            break;
        }   
        if(ret == d_KBD_CANCEL)
            break;
    }	

    inResult = inTCTRead(1);
    if(inResult != d_OK)
        return;	

    while(1)
    {
    	vduiClearBelow(3);
    	setLCDPrint(3, DISPLAY_POSITION_LEFT, "AUTO DOWNLOAD");
    	
    	if(strTCT.fSettleAutoDL== 0)
    		setLCDPrint(4, DISPLAY_POSITION_LEFT, "DISABLE");
    	if(strTCT.fSettleAutoDL == 1)
    		setLCDPrint(4, DISPLAY_POSITION_LEFT, "ENABLE"); 	
    	
    	CTOS_LCDTPrintXY(1, 5, "0-DISABLE      1-ENABLE");
    	
    	strcpy(strtemp,"New:") ; 
    	CTOS_LCDTPrintXY(1, 7, strtemp);
    	memset(strOut,0x00, sizeof(strOut));
    	memset(strtemp, 0x00, sizeof(strtemp));
    	usLen = 10;
    	ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 0, d_INPUT_TIMEOUT);
    	if(ret==d_KBD_ENTER)
    	{
    		if (strOut[0]==0x30 || strOut[0]==0x31)
    		{
    			if(strOut[0] == 0x31)
    			{
    				strTCT.fSettleAutoDL=1;
    			}
    			if(strOut[0] == 0x30)
    			{
    				strTCT.fSettleAutoDL=0;
    			}
    			inTCTSave(1);
    			break;
    		}
    		else
    		{
                vduiClearBelow(2);
                vduiWarningSound();
                vduiDisplayStringCenter(7,"PLEASE SELECT");
                vduiDisplayStringCenter(8,"A VALID VALUE");
                CTOS_Delay(2000);		
    		}
    	}
    	if(ret == d_KBD_CANCEL || ret == 0xFF)
    		break;
    }
*/
    inResult = inTCTRead(1);
    if(inResult != d_OK)
        return;	


	vduiClearBelow(3);
	setLCDPrint(3, DISPLAY_POSITION_LEFT, "TMS COMM MODE");
	inCommMode=strTCT.inTMSComMode;
	switch(strTCT.inTMSComMode)
	{
		case 0:
			setLCDPrint(4, DISPLAY_POSITION_LEFT, "DIALUP");
			break;
		case 1:
			setLCDPrint(4, DISPLAY_POSITION_LEFT, "LAN");
			break;
		case 2:
			setLCDPrint(4, DISPLAY_POSITION_LEFT, "GPRS");
			break;
		case 4:
			setLCDPrint(4, DISPLAY_POSITION_LEFT, "WIFI");
			break;
	}
	
    while(1)
    {   
	    fValid = TRUE;

		if (isCheckTerminalMP200() == d_OK)
		{
			setLCDPrint(5, DISPLAY_POSITION_LEFT, "1-GPRS");
			setLCDPrint(6, DISPLAY_POSITION_LEFT, "2-WIFI");
		}
		else
		{
			setLCDPrint(5, DISPLAY_POSITION_LEFT, "0-DIALUP 1-LAN");
			setLCDPrint(6, DISPLAY_POSITION_LEFT, "2-GPRS   3-WIFI");
		}
		        
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 1, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL || ret == 0)
            break;
        else
        {

			if (isCheckTerminalMP200() == d_OK)
			{
				switch (strOut[0])
				{
					case 0x31:
						inCommMode = GPRS_MODE;
					break;
					case 0x32:
						inCommMode = WIFI_MODE;
					break;
					default:
						fValid = FALSE;
					break;	
				}
			}
			else
			{
				switch (strOut[0])
				{
					case 0x30:
						inCommMode = DIAL_UP_MODE;
					break;
					case 0x31:
						inCommMode = ETHERNET_MODE;
					break;
					case 0x32:
						inCommMode = GPRS_MODE;
					break;
					case 0x33:
						inCommMode = WIFI_MODE;
					break;
					default:
						fValid = FALSE;
					break;	
				}
			}
			            
        }   
        
		if (!fValid)
			vdDisplayErrorMsg(1, 8, "INVALID INPUT");
		else					
			break; // exit while loop
		
    }	

	// Check hardware
	inResult = inCheckPromptHW(inCommMode);
	if(inResult != d_OK)
	return;

	// Save
	strTCT.inTMSComMode = inCommMode;      
	vdMyEZLib_LogPrintf("strTCT.inTMSComMode %d -- strOut[%d]",strTCT.inTMSComMode, strOut[0]);
	inResult = inTCTSave(1);
			
	// Read
    inResult = inTCTRead(1);
    if(inResult != d_OK)
        return;	
	
    while(1)
    {
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "TMS REMOTE IP");
        setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCT.szTMSRemoteIP);
        
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strtemp, 0x00, sizeof(strtemp));
        memset(strOut,0x00, sizeof(strOut));
        ret= struiApiGetStringSub(strtemp, 1,  8, strOut, MODE_IPADDRESS, 7, IP_LEN);
        if(ret==d_KBD_ENTER)
        {
            memcpy(strTCT.szTMSRemoteIP,strOut,strlen(strOut));
            strTCT.szTMSRemoteIP[strlen(strOut)]=0;
            vdMyEZLib_LogPrintf("strTCT.szTMSRemoteIP %s",strTCT.szTMSRemoteIP);
			inResult = inTCTSave(1);
            break;
        }	
        if(ret == d_KBD_CANCEL)
            break;
    }
	
    inResult = inTCTRead(1);
    if(inResult != d_OK)
        return;	
    while(1)
    {   
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "TMS REMOTE PORT");
        memset(szInputBuf, 0x00, sizeof(szInputBuf));
        sprintf(szInputBuf, "%d", strTCT.usTMSRemotePort);
        setLCDPrint(4, DISPLAY_POSITION_LEFT, szInputBuf);
        
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 5, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret>=1)
        {
            strTCT.usTMSRemotePort=atoi(strOut);      
            vdMyEZLib_LogPrintf("strTCT.usTMSRemotePort %d",strTCT.usTMSRemotePort);
			inResult = inTCTSave(1);
            break;
        }   
        if(ret == d_KBD_CANCEL)
            break;
    }	

    if(strTCT.inTMSComMode == DIAL_UP_MODE)
    {
    	vdDebug_LogPrintf("--TMS COM MODE - DIAL_UP_MODE--");
		
	    inResult = inTCTRead(1);
	    if(inResult != d_OK)
	        return;
		while(1)
		{
			vduiClearBelow(3);
			setLCDPrint(3, DISPLAY_POSITION_LEFT, "TMS REMOTE PHONE");
			setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCT.szTMSRemotePhone);
		
			strcpy(strtemp,"New:") ; 
			CTOS_LCDTPrintXY(1, 7, strtemp);
			memset(strOut,0x00, sizeof(strOut));
			usLen = 32;
			ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
			if (ret == d_KBD_CANCEL )
				break;
			else if(0 == ret )
				break;
			else if(ret>= 1)
			{
				memcpy(strTCT.szTMSRemotePhone,strOut,strlen(strOut));
				strTCT.szTMSRemotePhone[strlen(strOut)]=0;
				inResult = inTCTSave(1);
				break;
			}
			if(ret == d_KBD_CANCEL)
				break;
		}

	    inResult = inTCTRead(1);
	    if(inResult != d_OK)
	        return;
		while(1)
		{
			vduiClearBelow(3);
			setLCDPrint(3, DISPLAY_POSITION_LEFT, "TMS REMOTE ID");
			setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCT.szTMSRemoteID);
		
			strcpy(strtemp,"New:") ; 
			CTOS_LCDTPrintXY(1, 7, strtemp);
			memset(strOut,0x00, sizeof(strOut));
			usLen = 32;
			//ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
			ret= InputStringAlpha(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
			if (ret == d_KBD_CANCEL )
				break;
			else if(0 == ret )
				break;
			else if(ret>= 1)
			{
				memcpy(strTCT.szTMSRemoteID,strOut,strlen(strOut));
				strTCT.szTMSRemoteID[strlen(strOut)]=0;
				inResult = inTCTSave(1);
				break;
			}
			if(ret == d_KBD_CANCEL)
				break;
		}
	
	    inResult = inTCTRead(1);
	    if(inResult != d_OK)
	        return;
		while(1)
		{
			vduiClearBelow(3);
			setLCDPrint(3, DISPLAY_POSITION_LEFT, "TMS REMOTE PASSWORD");
			setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCT.szTMSRemotePW);
		
			strcpy(strtemp,"New:") ; 
			CTOS_LCDTPrintXY(1, 7, strtemp);
			memset(strOut,0x00, sizeof(strOut));
			usLen = 32;
			//ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
			ret= InputStringAlpha(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
			if (ret == d_KBD_CANCEL )
				break;
			else if(0 == ret )
				break;
			else if(ret>= 1)
			{
				memcpy(strTCT.szTMSRemotePW,strOut,strlen(strOut));
				strTCT.szTMSRemotePW[strlen(strOut)]=0;
				inResult = inTCTSave(1);
				break;
			}
			if(ret == d_KBD_CANCEL)
				break;
		
		}
    }
    else if(strTCT.inTMSComMode == GPRS_MODE)
    {
		CTMS_GPRSInfo stgprs;

		vdDebug_LogPrintf("--TMS COM MODE - GPRS--");
		
		memset(&stgprs, 0x00, sizeof (CTMS_GPRSInfo));
		usRes=CTOS_CTMSGetConfig(d_CTMS_GPRS_CONFIG, &stgprs);
		vdDebug_LogPrintf("CTOS_CTMSGetConfig - usRes[%d]", usRes);
		/*
        if(strlen(stgprs.strAPN) > 0)
            strcpy(strTCP.szAPN, stgprs.strAPN);

        if(strlen(stgprs.strID) > 0)
            strcpy(strTCP.szUserName, stgprs.strID);

        if(strlen(stgprs.strPW) > 0)
		    strcpy(strTCP.szPassword, stgprs.strPW);
		*/
		
		while(1)
		{	
			vduiClearBelow(3);
			setLCDPrint(3, DISPLAY_POSITION_LEFT, "SIM SLOT");
			setLCDPrint(4, DISPLAY_POSITION_LEFT, "1~2");
			memset(szInputBuf, 0x00, sizeof(szInputBuf));
			sprintf(szInputBuf, "%d", stgprs.bSIMSlot);
			//sprintf(szInputBuf, "%d", strTCP.inSIMSlot);
			setLCDPrint(6, DISPLAY_POSITION_LEFT, szInputBuf);
			
			strcpy(strtemp,"New:") ;
			CTOS_LCDTPrintXY(1, 7, strtemp);
			memset(strOut,0x00, sizeof(strOut));
			ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 1, 0, d_INPUT_TIMEOUT);
			if (ret == d_KBD_CANCEL )
				break;
			else if(0 == ret )
				break;
			else if(ret>=1)
			{
				if(atoi(strOut) == 1 || atoi(strOut) == 2)
				{
					if(atoi(strOut) == 1)
					    bSIMSlot=1;
					else
					    bSIMSlot=2;	
					break;
				}
				else
					vdDisplayErrorMsg(1, 8, "INVALID INPUT");
			}	
			if(ret == d_KBD_CANCEL)
				break;
		}	
						
        while(1)
        {
			vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "APN");
            setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCP.szAPN);
            
            strcpy(strtemp,"New:") ;
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strOut,0x00, sizeof(strOut));
			usLen = 32;
            ret= InputStringAlpha(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
            if(ret==d_KBD_ENTER)
            {
                memcpy(strTCP.szAPN, strOut,strlen(strOut));
                strTCP.szAPN[strlen(strOut)]=0;
                break;
            }   
            if(ret == d_KBD_CANCEL)
                break;
        }               
    
        while(1)
        {
			vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "GPRS USER NAME");
            setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCP.szUserName);
    
            strcpy(strtemp,"New:") ;
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strOut,0x00, sizeof(strOut));
			usLen = 32;
            ret= InputStringAlpha(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
            if(ret==d_KBD_ENTER)
            {
                //BolDetachGPRSChangeSetting=TRUE;
                memcpy(strTCP.szUserName, strOut,strlen(strOut));
                strTCP.szUserName[strlen(strOut)]=0;
                //inResult = inTCPSave(1);
                break;
            }   
            if(ret == d_KBD_CANCEL)
                break;
        }               
    
        while(1)
        {
			vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "GPRS PASSWORD");
            setLCDPrint(4, DISPLAY_POSITION_CENTER, strTCP.szPassword);
    
            strcpy(strtemp,"New:") ;
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strOut,0x00, sizeof(strOut));
			usLen = 32;
            ret= InputStringAlpha(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
            if(ret==d_KBD_ENTER)
            {
                //BolDetachGPRSChangeSetting=TRUE;
                memcpy(strTCP.szPassword, strOut,strlen(strOut));
                strTCP.szPassword[strlen(strOut)]=0;
                //inResult = inTCPSave(1);
                break;
            }   
            if(ret == d_KBD_CANCEL)
                break;
        } 

        memset(&stgprs, 0x00, sizeof (CTMS_GPRSInfo));
		bSIMSlot = 1;
		stgprs.bSIMSlot = bSIMSlot;
        strcpy(stgprs.strAPN, strTCP.szAPN);
        strcpy(stgprs.strID, strTCP.szUserName);
        strcpy(stgprs.strPW, strTCP.szPassword);
 
        strcpy(stgprs.strRemoteIP, strTCT.szTMSRemoteIP);
        stgprs.usRemotePort = strTCT.usTMSRemotePort;
        stgprs.ulSIMReadyTimeout = 10000;
        stgprs.ulGPRSRegTimeout = 10000;
        stgprs.usPPPRetryCounter = 5;
        stgprs.ulPPPTimeout = 10000;
        stgprs.ulTCPConnectTimeout = 10000;

		stgprs.ulTCPTxTimeout=10000;
		stgprs.ulTCPRxTimeout=10000;		

		usRes=CTOS_CTMSSetConfig(d_CTMS_GPRS_CONFIG, &stgprs);

		vdDebug_LogPrintf("CTOS_CTMSSetConfig - usRes[%d]", usRes);
		vdDebug_LogPrintf("stgprs.strAPN[%s]", stgprs.strAPN);
		vdDebug_LogPrintf("stgprs.strID[%s]", stgprs.strID);
		vdDebug_LogPrintf("stgprs.strPW[%s]", stgprs.strPW);
		vdDebug_LogPrintf("stgprs.strRemoteIP[%s]", stgprs.strRemoteIP);
		vdDebug_LogPrintf("stgprs.usRemotePort[%d]", stgprs.usRemotePort);
		
		vdDebug_LogPrintf("strTCP.szAPN[%s]", strTCP.szAPN);
		vdDebug_LogPrintf("strTCP.szUserName[%s]", strTCP.szUserName);
		vdDebug_LogPrintf("strTCP.szPassword[%s]", strTCP.szPassword);
		
    }
	else if (strTCT.inTMSComMode == WIFI_MODE)
	{
		CTMS_WIFIInfo stWF;
	
		memset(&stWF, 0x00, sizeof (d_CTMS_WIFI_CONFIG));
		CTOS_CTMSGetConfig(d_CTMS_WIFI_CONFIG, &stWF);
	
		strcpy(stWF.strRemoteIP, strTCT.szTMSRemoteIP);
		stWF.usRemotePort = strTCT.usTMSRemotePort;
		//stgprs.ulSIMReadyTimeout = 10000;
		//stgprs.ulGPRSRegTimeout = 10000;
		//stgprs.usPPPRetryCounter = 5;
		//stgprs.ulPPPTimeout = 10000;
		//stgprs.ulTCPConnectTimeout = 10000;
	
		CTOS_CTMSSetConfig(d_CTMS_WIFI_CONFIG , &stWF);

    }
	
}

void put_env_char(char *tag, char *value)
{
	int     ret = -1;
	char    buf[12];

    memset (buf, 0, sizeof (buf));
    //int2str (buf, value);
    sprintf(buf, "%s", value);
    ret = inCTOSS_PutEnvDB (tag, buf);

	vdDebug_LogPrintf("put_env_char [%s]=[%s] ret[%d]", tag, value, ret);
}

int inCTOS_PromptReprintPassword(void)
{
    int inRet = d_NO;
    
   //CTOS_LCDTClearDisplay();
    vduiClearBelow(7);
    vdCTOS_SetTransType(REPRINT_ANY);
    
    vdDispTransTitle(REPRINT_ANY);
    
    inRet = inCTOSS_CheckMemoryStatus();
    if(d_OK != inRet)
        return inRet;
    
    inRet = inCTOS_GetTxnPassword();

   //CTOS_LCDTClearDisplay();
	
    if(d_OK != inRet)
    {
        inCTOS_IdleEventProcess();
        return inRet;
    }

    return d_OK;
}

int inSelectIdleTrxn(void)
{
    int inRet;
    char szDebug[40 + 1]={0};
    char szSaleType[12][19+1];
    char szSaleMenu[1024];
    char szHeaderString[24+1];
    int bHeaderAttr = 0x01+0x04, key=0;
    int inLoop=0;
    int inIndex=0;
    int inSaleType = 0;
    BOOL isValid = TRUE;
    int inResult=0x0000;
    int inNumRecs =0;
    int i = 0;
    char szSelectedMenu[100];
    char szTrxFunc[100];

    vdDebug_LogPrintf("--inSelectIdleTrxn--");
    vdDebug_LogPrintf("inMenuid[%d]", strTCT.inMenuid);

    memset(szHeaderString, 0x00, sizeof(szHeaderString));
    memset(szSaleType, 0x00, sizeof(szSaleType));
    memset(szSaleMenu, 0x00, sizeof(szSaleMenu));

    inReadBaseDMTrxMenu(&inNumRecs);

    vdDebug_LogPrintf("inGetSelectionMenu.inNumRecs[%d]", inNumRecs);

    strcat(szSaleMenu,  "SELECT APPLICATION|");

    for (i=0; i<inNumRecs; i++)
    {
        if (strTrxMenu[i].szButtonItemLabel[0] != 0)
        {
            inIndex=i;
            strcat((char *)szSaleMenu, strTrxMenu[i].szButtonItemLabel);
            strcat((char *) szSaleMenu, (char *) "|");
            strcat((char *) szSaleMenu, strTrxMenu[i].szButtonItemImage);


            if (strTrxMenu[i+1].szButtonItemLabel[0] != 0)
                strcat((char *)szSaleMenu, (char *)" \n");
        }
    }

    vdDebug_LogPrintf("inSelectIdleTrxn.szSaleMenu[%s] len [%d]", szSaleMenu, strlen(szSaleMenu));

    if (inNumRecs > 0)
    {
        key = IdleMenuTransType(szSaleMenu);
        vdDebug_LogPrintf("SATURN inSelectIdleTrxn - MenuTransType KEY %d", key);

        if (key == 'C')
        {
            //vdDisplayErrorMsg(1, 8, "USER CANCEL");
            return d_NO;
        }

        if (key > 0)
        {
            memset(szSelectedMenu, 0x00, sizeof(szSelectedMenu));
            memset(szTrxFunc, 0x00, sizeof(szTrxFunc));
            strcpy(szSelectedMenu, strTrxMenu[key-1].szButtonItemLabel);
            //check if need to read submenu
            strcpy(szTrxFunc, strTrxMenu[key-1].szButtonItemFunc);
            vdDebug_LogPrintf("szSelectedMenu", strTrxMenu[key-1]);

            if((memcmp(szSelectedMenu, "MC/VISA/JCB/UPI", strlen(szSelectedMenu)) == 0) ||
               (memcmp(szSelectedMenu, "MC/VISA", strlen(szSelectedMenu)) == 0) ||
               (memcmp(szSelectedMenu, "SALE", strlen(szSelectedMenu)) == 0) ||
               (memcmp(szSelectedMenu, "INSTALLMENT", strlen(szSelectedMenu)) == 0))
            {
                inSaleType = 1;
            }
            else if(memcmp(szSelectedMenu, "BANCNET", strlen(szSelectedMenu)) == 0)
            {
                inSaleType = 2;
            }
            else
            {
                isValid = FALSE;
                vdDisplayErrorMsg(1, 8, "INVALID TRANS");
            }
        }
        else
        {
            isValid = FALSE;
            vdDisplayErrorMsg(1, 8, "INVALID TRANS");
        }

        switch (inSaleType)
        {
            case 1: // Credit
                inSelectedIdleAppsHost = MCC_HOST;
                if(strTrxMenu[key-1].szButtonItemFunc[0] != 0)
                {
                    inCTOSS_ExeFunction(strTrxMenu[key-1].szButtonItemFunc);
                    break;
                }
                else
                {
                    //vdDisplayIdleEMVApp(IDLE_CREDIT_AND_BANCNET);
                    vdDisplayIdleEMVTrxn2(IDLE_CREDIT_AND_BANCNET);
                    break;
                }
            case 2: // Bancnet
                inSelectedIdleAppsHost = BANCNET_HOST;
                if(strTrxMenu[key-1].szButtonItemFunc[0] != 0)
                {
                    inCTOSS_ExeFunction(strTrxMenu[key-1].szButtonItemFunc);
                    break;
                }
                else
                {
                    //vdDisplayIdleEMVApp(IDLE_BANCNET);
                    //vdDisplayIdleEMVTrxn(IDLE_BANCNET);
                    vdDisplayIdleEMVTrxn2(IDLE_BANCNET);
                    break;
                }

            default:
                isValid = FALSE;
                vdDisplayErrorMsg(1, 8, "INVALID TRANS");
                break;
        }

        vdDebug_LogPrintf("isValid[%d]", isValid);
        vdDebug_LogPrintf("inSaleType[%d]", inSaleType);
        vdDebug_LogPrintf("inSelectedIdleAppsHost[%d]", inSelectedIdleAppsHost);
    }
    else
    {
        isValid = FALSE;
        vdDisplayErrorMsg(1, 8, "NO APP MENU CREATED");
    }


    if (!isValid)
        return d_NO;
    else
        return d_OK;
}

//TINE:  for android terminal
int vdDisplayIdleEMVTrxn(int inIdleSaleType) {
    int inMenuNumRecs = 0,
            inCtr = 0,
            inKey = 0,
            inHeaderAttr = 0x01 + 0x04;

    char szMenuItems[9000] = {0},
            szBuf[200] = {0},
            szHeader[20] = {0};

    vdDebug_LogPrintf("--vdDisplayIdleEMVTrxn--");
    vdDebug_LogPrintf("inIdleSaleType[%d]", inIdleSaleType);

    inMenuNumRecs = inReadDMTrxAppMenu(inIdleSaleType);

    vdDebug_LogPrintf("inMenuNumRecs[%d]", inMenuNumRecs);

    if (strlen(strDynamicMenu[0].szButtonItemFunc) > 0) {
        inCTOSS_ExeFunction(strDynamicMenu[0].szButtonItemFunc);
        return d_OK;
    } else if (strlen(strDynamicMenu[0].szSubMenuName) > 0) {
        inMenuNumRecs = inReadDMTrxSubMenu(inIdleSaleType);

        if (inMenuNumRecs > 1) {
            strcpy(szHeader, "SALE MENU");
            memset(szMenuItems, 0, sizeof(szMenuItems));
            vdDebug_LogPrintf("inMenuNumRecs = [%d]", inMenuNumRecs);


            strcpy(szHeader, "SELECT TRANSACTION");
            memset(szMenuItems, 0, sizeof(szMenuItems));
            for (inCtr = 0; inCtr < inMenuNumRecs; ++inCtr) {
                strcat(szMenuItems, strDynamicMenu[inCtr].szButtonItemLabel);
                if ((strlen(strDynamicMenu[inCtr].szButtonItemLabel) > 0) &&
                    (inCtr != inMenuNumRecs))
                    strcat(szMenuItems, " \n");

            }

            vdDebug_LogPrintf("szMenuItems[%s]", szMenuItems);
            inCtr = strlen(szMenuItems);
            szMenuItems[inCtr] = 0x00;

            inKey = MenuDisplay(szHeader, strlen(szHeader), inHeaderAttr, 1, 1, szMenuItems, TRUE);
            vdDebug_LogPrintf("inKey[%d]", inKey);

            if (inKey == 0xFF) {
               //CTOS_LCDTClearDisplay();
                setLCDPrint(1, DISPLAY_POSITION_CENTER, "WRONG INPUT!!!");
                vduiWarningSound();
                return -1;
            }

            if (inKey > 0) {
                if (inKey == d_KBD_CANCEL)
                    return -1;

                inCTOSS_ExeFunction(strDynamicMenu[inKey - 1].szButtonItemFunc);
                return d_OK;
            }

        } else {
            inCTOSS_ExeFunction(strDynamicMenu[0].szButtonItemFunc);
            return d_OK;
        }
    }

}

int vdDisplayIdleEMVTrxn2(int inIdleSaleType) {
     int inMenuNumRecs = 0,
     inCtr = 0,
     inKey = 0,
     inHeaderAttr = 0x01 + 0x04;
     
     char szMenuItems[9000] = {0},
     szBuf[200] = {0},
     szHeader[20] = {0};

    vdDebug_LogPrintf("--vdDisplayIdleEMVTrxn2--");
    vdDebug_LogPrintf("inIdleSaleType[%d]", inIdleSaleType);

    inMenuNumRecs = inReadDMTrxAppMenu2(inIdleSaleType);
	
    if (inMenuNumRecs > 1) 
    {
         strcpy(szHeader, "SALE MENU");
         memset(szMenuItems, 0, sizeof(szMenuItems));
         vdDebug_LogPrintf("inMenuNumRecs = [%d]", inMenuNumRecs);
         
         
         strcpy(szHeader, "SELECT TRANSACTION");
         memset(szMenuItems, 0, sizeof(szMenuItems));
         for (inCtr = 0; inCtr < inMenuNumRecs; ++inCtr) 
	{
              strcat(szMenuItems, strDynamicMenu[inCtr].szButtonItemLabel);
              if ((strlen(strDynamicMenu[inCtr].szButtonItemLabel) > 0) && (inCtr != inMenuNumRecs))
                   strcat(szMenuItems, " \n");
         }
         
         vdDebug_LogPrintf("szMenuItems[%s]", szMenuItems);
         inCtr = strlen(szMenuItems);
         szMenuItems[inCtr] = 0x00;
         
         inKey = MenuDisplay(szHeader, strlen(szHeader), inHeaderAttr, 1, 1, szMenuItems, TRUE);
         vdDebug_LogPrintf("inKey[%d]", inKey);
         
         
         if (inKey == 0xFF) 
	{
             //CTOS_LCDTClearDisplay();
              setLCDPrint(1, DISPLAY_POSITION_CENTER, "WRONG INPUT!!!");
              vduiWarningSound();
              return -1;
         }
         if (inKey > 0)
         {
              if (inKey == d_KBD_CANCEL)
                   return -1;
              
              if(strlen(strDynamicMenu[inKey-1].szSubMenuName) > 0)
              {
                   inMenuNumRecs = inReadDynamicSubMenu(inIdleSaleType); 
                   strcpy(szHeader, "SALE MENU");
                   memset(szMenuItems, 0, sizeof(szMenuItems));
                   
                   vdDebug_LogPrintf("AAA - inMenuNumRecs[%d]", inMenuNumRecs);
                   
                   for(inCtr = 0; inCtr < inMenuNumRecs; ++inCtr)
                   {
                        vdDebug_LogPrintf("AAA - szMenuItems[%s]", szMenuItems);
                        strcat(szMenuItems, strDynamicMenu[inCtr].szButtonItemLabel);
                        
                        if(strlen(strDynamicMenu[inCtr+1].szButtonItemLabel) > 0)
                             strcat(szMenuItems, " \n");
                   }
                   
                   inCtr = strlen(szMenuItems) - 1;
                   szMenuItems[inCtr] = 0x00;
                   
                   inKey = MenuDisplay(szHeader, strlen(szHeader), inHeaderAttr, 1, 1, szMenuItems, TRUE);
                   vdDebug_LogPrintf("inKey[%d]", inKey);
                   
                   if(inKey == 0xFF)
                   {
                       //CTOS_LCDTClearDisplay();
                        setLCDPrint(1, DISPLAY_POSITION_CENTER, "WRONG INPUT!!!");
                        vduiWarningSound();
                        return -1;
                   }
                   
                   if(inKey > 0)
                   {
                        if(inKey == d_KBD_CANCEL)
                             return -1;
                        
                        inCTOSS_ExeFunction(strDynamicMenu[inKey-1].szButtonItemFunc);
                        inKey = d_OK;
                   }
              }		
              
              inCTOSS_ExeFunction(strDynamicMenu[inKey - 1].szButtonItemFunc);
              return d_OK;
         }
    
    } 
    else 
    {
         inCTOSS_ExeFunction(strDynamicMenu[0].szButtonItemFunc);
         return d_OK;
    }


}


int vdDisplayTrxn(int inSaleType, char *sztrxlogo)
{
    int inMenuNumRecs = 0,
            inCtr = 0,
            inKey = 0,
            inHeaderAttr = 0x01+0x04;

    char szMenuItems[9000] = {0},
            szBuf[200] = {0},
            szTransFunc[100] = {0},
            szHeader[20] = {0},
            szMenuItemTemp[100] = {0};

	char szSetupItem[400][400];
	char szSetupItemFunc[400][400];
	int setup_item_cnt = 0;
	int inRet = 0;

    inTCTRead(1);
	
	vdDebug_LogPrintf("saturn  %d ", strTCT.inMenuid);
    vdDebug_LogPrintf("saturn --vdDisplayTrxn-- ");
    vdDebug_LogPrintf("saturn inSaleType[%d]", inSaleType);

    GET_LIST:
    inMenuNumRecs = inReadDMTrx(inSaleType);

    vdDebug_LogPrintf("inMenuNumRecs[%d]", inMenuNumRecs);

    memset(szMenuItems, 0, sizeof(szMenuItems));
	if(inSaleType == 0 || inSaleType == 'R' || inSaleType == 'P' || inSaleType == 'N')
	{
		//strcpy(szHeader, "SELECT TRANSACTION");
		memset(szHeader, 0x00, sizeof(szHeader));

		if(inSaleType == 0)
		{
            strcpy(szHeader, "SETTINGS");
		    for(inCtr = 0; inCtr < inMenuNumRecs; ++inCtr)
	        {
				//code to handle conflicting details on DynamicMenu and Android Terminal item display
	            if(strcmp(strDynamicMenu[inCtr].szButtonItemLabel, "IP SETTING") == 0)
	            {
	                memset(szMenuItemTemp, 0x00, sizeof(szMenuItemTemp));
	                strcpy(szMenuItemTemp, "CHANGE COMM");
	                strcat(szMenuItems, szMenuItemTemp);

					//TINE:  save item in new array of setup transactions
					strcpy(szSetupItem[setup_item_cnt], szMenuItemTemp);
					strcpy(szSetupItemFunc[setup_item_cnt], strDynamicMenu[inCtr].szButtonItemFunc);

					if(strlen(strDynamicMenu[inCtr+1].szButtonItemLabel) > 0)
	                strcat(szMenuItems, " \n");

					setup_item_cnt++;
	            }
				else if(strcmp(strDynamicMenu[inCtr].szButtonItemLabel, "CLEAR BATCH") == 0 
					|| strcmp(strDynamicMenu[inCtr].szButtonItemLabel, "CLEAR REVERSAL") == 0
					|| strcmp(strDynamicMenu[inCtr].szButtonItemLabel, "EDIT TABLE") == 0
					|| strcmp(strDynamicMenu[inCtr].szButtonItemLabel, "SYSTEM PANEL") == 0
					|| strcmp(strDynamicMenu[inCtr].szButtonItemLabel, "SET WIFI") == 0)
				{
					//TINE:  include item in transaction list
					strcat(szMenuItems, strDynamicMenu[inCtr].szButtonItemLabel);

					//TINE:  save item in new array of setup transactions
					strcpy(szSetupItem[setup_item_cnt], szMenuItemTemp);
					strcpy(szSetupItemFunc[setup_item_cnt], strDynamicMenu[inCtr].szButtonItemFunc);

					if(strlen(strDynamicMenu[inCtr+1].szButtonItemLabel) > 0)
	                strcat(szMenuItems, " \n");

					setup_item_cnt++;
				}
				else if(strcmp(strDynamicMenu[inCtr].szButtonItemLabel, "CHANGE TELCO") == 0)
				{
					//TINE:  include item in transaction list
					strcat(szMenuItems, strDynamicMenu[inCtr].szButtonItemLabel);

					//TINE:  save item in new array of setup transactions
					strcpy(szSetupItem[setup_item_cnt], szMenuItemTemp);
					strcpy(szSetupItemFunc[setup_item_cnt], strDynamicMenu[inCtr].szButtonItemFunc);

					if(strlen(strDynamicMenu[inCtr+1].szButtonItemLabel) > 0)
	                strcat(szMenuItems, " \n");

					setup_item_cnt++;
				}
				else if(strcmp(strDynamicMenu[inCtr].szButtonItemLabel, "EDIT HOST") == 0) // sidumili: added for edit host
				{
                    memset(szMenuItemTemp, 0x00, sizeof(szMenuItemTemp));
                    strcpy(szMenuItemTemp, "HOST SETTINGS");
                    strcat(szMenuItems, szMenuItemTemp);

                    //TINE:  save item in new array of setup transactions
                    strcpy(szSetupItem[setup_item_cnt], szMenuItemTemp);
                    strcpy(szSetupItemFunc[setup_item_cnt], strDynamicMenu[inCtr].szButtonItemFunc);

					if(strlen(strDynamicMenu[inCtr+1].szButtonItemLabel) > 0)
	                strcat(szMenuItems, " \n");

					setup_item_cnt++;
				}
				else if(strcmp(strDynamicMenu[inCtr].szButtonItemLabel, "EDIT IP") == 0) // sidumili: added for edit ip
				{
                    memset(szMenuItemTemp, 0x00, sizeof(szMenuItemTemp));
                    strcpy(szMenuItemTemp, "IP SETTINGS");
                    strcat(szMenuItems, szMenuItemTemp);

                    //TINE:  save item in new array of setup transactions
                    strcpy(szSetupItem[setup_item_cnt], szMenuItemTemp);
                    strcpy(szSetupItemFunc[setup_item_cnt], strDynamicMenu[inCtr].szButtonItemFunc);

					if(strlen(strDynamicMenu[inCtr+1].szButtonItemLabel) > 0)
	                strcat(szMenuItems, " \n");

					setup_item_cnt++;
				}
				else if(strcmp(strDynamicMenu[inCtr].szButtonItemLabel, "EDIT TERMINAL") == 0) // sidumili: added for edit terminal
				{
                    memset(szMenuItemTemp, 0x00, sizeof(szMenuItemTemp));
                    strcpy(szMenuItemTemp, "TERMINAL SETTINGS");
                    strcat(szMenuItems, szMenuItemTemp);

                    //TINE:  save item in new array of setup transactions
                    strcpy(szSetupItem[setup_item_cnt], szMenuItemTemp);
                    strcpy(szSetupItemFunc[setup_item_cnt], strDynamicMenu[inCtr].szButtonItemFunc);

					if(strlen(strDynamicMenu[inCtr+1].szButtonItemLabel) > 0)
	                strcat(szMenuItems, " \n");

					setup_item_cnt++;
				}
				else if(strcmp(strDynamicMenu[inCtr].szButtonItemLabel, "EDIT PRINTER") == 0) // sidumili: added for edit printer
				{
                    memset(szMenuItemTemp, 0x00, sizeof(szMenuItemTemp));
                    strcpy(szMenuItemTemp, "EDIT HEADERS");
                    strcat(szMenuItems, szMenuItemTemp);

                    //TINE:  save item in new array of setup transactions
                    strcpy(szSetupItem[setup_item_cnt], szMenuItemTemp);
                    strcpy(szSetupItemFunc[setup_item_cnt], strDynamicMenu[inCtr].szButtonItemFunc);

					if(strlen(strDynamicMenu[inCtr+1].szButtonItemLabel) > 0)
	                strcat(szMenuItems, " \n");

					setup_item_cnt++;
				}
                else if(strcmp(strDynamicMenu[inCtr].szButtonItemLabel, "INIT WAVE") == 0) // sidumili: added for edit printer
                {
                    //TINE:  include item in transaction list
                    strcat(szMenuItems, strDynamicMenu[inCtr].szButtonItemLabel);

                    //TINE:  save item in new array of setup transactions
                    strcpy(szSetupItem[setup_item_cnt], szMenuItemTemp);
                    strcpy(szSetupItemFunc[setup_item_cnt], strDynamicMenu[inCtr].szButtonItemFunc);

                    if(strlen(strDynamicMenu[inCtr+1].szButtonItemLabel) > 0)
                        strcat(szMenuItems, " \n");

                    setup_item_cnt++;
                }
	            else if((strcmp(strDynamicMenu[inCtr].szButtonItemLabel, "DELETE ISO LOGS") == 0) && (strTCT.inISOLogger==1)) 
				{
                    //memset(szMenuItemTemp, 0x00, sizeof(szMenuItemTemp));
                    //strcpy(szMenuItemTemp, "ISO LOGGER");
                    strcat(szMenuItems, strDynamicMenu[inCtr].szButtonItemLabel);

                    //TINE:  save item in new array of setup transactions
                    strcpy(szSetupItem[setup_item_cnt], szMenuItemTemp);
                    strcpy(szSetupItemFunc[setup_item_cnt], strDynamicMenu[inCtr].szButtonItemFunc);

					if(strlen(strDynamicMenu[inCtr+1].szButtonItemLabel) > 0)
	                strcat(szMenuItems, " \n");

					setup_item_cnt++;
				}
				else if(strcmp(strDynamicMenu[inCtr].szButtonItemLabel, "USER MANUAL") == 0) // sidumili: added for user manual
                {
                    //TINE:  include item in transaction list
                    strcat(szMenuItems, strDynamicMenu[inCtr].szButtonItemLabel);

                    //TINE:  save item in new array of setup transactions
                    strcpy(szSetupItem[setup_item_cnt], szMenuItemTemp);
                    strcpy(szSetupItemFunc[setup_item_cnt], strDynamicMenu[inCtr].szButtonItemFunc);

                    if(strlen(strDynamicMenu[inCtr+1].szButtonItemLabel) > 0)
                        strcat(szMenuItems, " \n");

                    setup_item_cnt++;
                }
				else if(strcmp(strDynamicMenu[inCtr].szButtonItemLabel, "ECR SETTINGS") == 0)
				{
					//TINE:  include item in transaction list
					strcat(szMenuItems, strDynamicMenu[inCtr].szButtonItemLabel);

					//TINE:  save item in new array of setup transactions
					strcpy(szSetupItem[setup_item_cnt], szMenuItemTemp);
					strcpy(szSetupItemFunc[setup_item_cnt], strDynamicMenu[inCtr].szButtonItemFunc);

					if(strlen(strDynamicMenu[inCtr+1].szButtonItemLabel) > 0)
	                strcat(szMenuItems, " \n");

					setup_item_cnt++;
				}
	        }
		}
		else
		{		

		    if(inSaleType == 'P')
                strcpy(szHeader, "SETTLEMENT");
			else if(inSaleType == 'R')
                strcpy(szHeader, "REPORTS");
			else if(inSaleType == 'N')
			    strcpy(szHeader, "REPRINT");

		    for(inCtr = 0; inCtr < inMenuNumRecs; ++inCtr)
			{
				strcat(szMenuItems, strDynamicMenu[inCtr].szButtonItemLabel);
			
				if(strlen(strDynamicMenu[inCtr+1].szButtonItemLabel) > 0)
				strcat(szMenuItems, " \n");
			}
			vdDebug_LogPrintf("AAA - szMenuItems[%s]", szMenuItems);
		}     
	}
    else
    {
		strcpy(szMenuItems, sztrxlogo);
    	strcat(szMenuItems, "|");

        for(inCtr = 0; inCtr < inMenuNumRecs; ++inCtr)
        {
            strcat(szMenuItems, strDynamicMenu[inCtr].szButtonItemLabel);
            strcat(szMenuItems, "|");
            strcat(szMenuItems, strDynamicMenu[inCtr].szButtonItemFunc);
            //if((strlen(strDynamicMenu[inCtr+1].szButtonItemLabel) > 0) && (inCtr != inMenuNumRecs))
            if(strlen(strDynamicMenu[inCtr+1].szButtonItemLabel) > 0)
                strcat(szMenuItems, " \n");

        }
    }
    



    vdDebug_LogPrintf("szMenuItems[%s]", szMenuItems);

    //inCtr = strlen(szMenuItems);
    //szMenuItems[inCtr] = 0x00;
    SETTINGS_MENU:
    //inKey = MenuDisplay(szHeader, strlen(szHeader), inHeaderAttr, 1, 1, szMenuItems, TRUE);
    if(inSaleType == 0 || inSaleType == 'R' || inSaleType == 'P' || inSaleType == 'N')
    {
		inKey = MenuDisplay(szHeader, strlen(szHeader), inHeaderAttr, 1, 1, szMenuItems, TRUE);
    }
	else
	{
		inKey = MenuTransactions(szMenuItems, szTransFunc);
	}
    

    vdDebug_LogPrintf("inKey[%d]", inKey);

    if(inKey == 0xFF)
    {
        //CTOS_LCDTClearDisplay();
        //setLCDPrint(1, DISPLAY_POSITION_CENTER, "WRONG INPUT!!!");
        //vduiWarningSound();
        //vdDisplayErrorMsg(1, 8, "TIME OUT");
        return d_NO;
    }

    if(inKey == d_KBD_CANCEL)
    {
		//vdDisplayErrorMsg(1, 8, "USER CANCEL");
		return -1;
    }
        

	if(inSaleType == 0)
    {
        vdDebug_LogPrintf("szSetupItemFunc[%s]", szSetupItemFunc[inKey-1]);
        inRet = inCTOSS_ExeFunction(szSetupItemFunc[inKey-1]);
        if (inRet == -2)
            goto SETTINGS_MENU;

        return d_OK;
    }
	else if(inSaleType == 'R' || inSaleType == 'P' || inSaleType == 'N')
    {
        vdDebug_LogPrintf("strDynamicMenu.szButtonItemFunc[%s]", strDynamicMenu[inKey-1].szButtonItemFunc);
        vdDebug_LogPrintf("strDynamicMenu[inKey-1].szButtonItemLabel [%s]", strDynamicMenu[inKey-1].szButtonItemLabel);
        if (strcmp(strDynamicMenu[inKey-1].szButtonItemLabel, "REPRINT") == 0) {
            vdDebug_LogPrintf("inSaleType N");
            inSaleType = 'N';
            goto GET_LIST;
        }
        
	    //if ((strcmp(strDynamicMenu[inKey-1].szButtonItemLabel, "SETTLE ONE") == 0) && (get_env_int("APPTYPE")==1 || get_env_int("APPTYPE")==3)){
        if (strcmp(strDynamicMenu[inKey-1].szButtonItemLabel, "SETTLE ONE") == 0){
            inSaleType = 'S';
            srTransRec.byTransType=SETTLE;
            inRet = inCTOS_GetTxnPassword();
            if(d_OK != inRet)
                 return inRet;	
            goto GET_LIST;
        }
        else
            inCTOSS_ExeFunction(strDynamicMenu[inKey-1].szButtonItemFunc);

        return d_OK;
    }
    else
    {
        if(inKey == d_OK)
        {
			if(strcmp(szTransFunc, "inPHQRSale") == 0 || strcmp(szTransFunc, "inCTOS_PHQRRetreive") == 0 || strcmp(szTransFunc, "inPHQRLogin") == 0
				|| strcmp(szTransFunc, "inQRPAY") == 0 || strcmp(szTransFunc, "inCTOS_Retrieve") == 0)
			    vdDisplayMessageStatusBox(1, 8, "Processing  ", MSG_PLS_WAIT, MSG_TYPE_PROCESS);
			
            inCTOSS_ExeFunction(szTransFunc);
            return d_OK;
        }
    }

}

int inCTOS_AutoSelectHostSetting(void)
{
    BYTE bHeaderAttr = 0x01+0x04, iCol = 1;
    BYTE  x = 1;
    BYTE key;
    char szHeaderString[50] = "SELECT HOST";
    char szHostMenu[1024];
    char szHostName[50][100];
    int inCPTID[50];
    int inLoop = 0;

	vdDebug_LogPrintf("--inCTOS_AutoSelectHostSetting--");
	vdDebug_LogPrintf("inSettlementHost=[%d]", inSettlementHost);
	
    if (inMultiAP_CheckSubAPStatus() == d_OK)
        return d_OK;
    

    memset(szHostMenu, 0x00, sizeof(szHostMenu));
    memset(szHostName, 0x00, sizeof(szHostName));
    memset((char*)inCPTID, 0x00, sizeof(inCPTID));

	switch (inSettlementHost)
	{
		case 1: // Credit
			inHDTReadHostNameCredit(szHostName, inCPTID);
		break;
		case 2: // Bancnet
			inHDTReadHostNameBancnet(szHostName, inCPTID);
		break;
		case 3: // Digital Wallet
			inHDTReadHostNameDigitalWallet(szHostName, inCPTID);
		break;
	}
    
    for (inLoop = 0; inLoop < 50; inLoop++)
    {
        if (szHostName[inLoop][0]!= 0)
        {
			if(memcmp(szHostName[inLoop], "MCC ONE APP", strlen("MCC ONE APP")) == 0)
				strcat((char *)szHostMenu, "MC/VISA/JCB/UPI");			
			else	
                strcat((char *)szHostMenu, szHostName[inLoop]);
            if (szHostName[inLoop+1][0]!= 0)
                strcat((char *)szHostMenu, (char *)" \n");      
        }
        else
            break;
    }
        
    key = MenuDisplay(szHeaderString, strlen(szHeaderString), bHeaderAttr, iCol, x, szHostMenu, TRUE);

	/*
   if (inSettlementHost == 1)
	   key = 1;
   else if (inSettlementHost == 2)
	   key = 2;
	*/

    vdDebug_LogPrintf("saturn after seleting host from menu %d", inSettlementHost);

    //if (key == 0xFF) 
    //{
    //    CTOS_LCDTClearDisplay();
    //    setLCDPrint(1, DISPLAY_POSITION_CENTER, "WRONG INPUT!!!");
    //    vduiWarningSound();
    //    return -1;  
    //}

   //if(key > 0)
    //{
        //if(d_KBD_CANCEL == key)
        //    return -1;
        
        vdDebug_LogPrintf("saturn key[%d] HostID[%d]", key, inCPTID[key-1]);
        srTransRec.HDTid = inCPTID[key-1];
        strHDT.inHostIndex = inCPTID[key-1];
        inHDTRead(inCPTID[key-1]);
        inCPTRead(inCPTID[key-1]);
        strcpy(srTransRec.szHostLabel, strHDT.szHostLabel);
   // }
    
    return inCPTID[key-1];

}


/*configuration functions*/
int vdCTOS_ChangeComm(void)
{
	char szChoiceMsg[30 + 1];
    int bHeaderAttr = 0x01+0x04, key=0;
    char szHeaderMsg2[30 + 1];	
	int inCommMode = -1;

   //CTOS_LCDTClearDisplay();
	memset(szChoiceMsg, 0x00, sizeof(szChoiceMsg));
	memset(szHeaderMsg2, 0x00, sizeof(szHeaderMsg2));
	strcpy(szHeaderMsg2, "SELECT COMM");

    strcat(szChoiceMsg,"GPRS \n");
    strcat(szChoiceMsg,"WIFI ");
	
    key = MenuDisplay(szHeaderMsg2,strlen(szHeaderMsg2), bHeaderAttr, 1, 1, szChoiceMsg, TRUE);
    
	if(key == 'C')
	{
		//vdDisplayErrorMsg(1, 8, "USER CANCEL");
        return -2;
	}
	else if (key == 0xFF)
	{
		vdDisplayMessageBox(1, 8, "", "TIME OUT", "", MSG_TYPE_TIMEOUT);
		CTOS_Beep();
		CTOS_Delay(1500);
		return -1;
	}
	else if (key > 0)
	{
		if (key == 1)   
		{
			inCommMode = GPRS_MODE;
		}
		else if (key == 2)   
		{
			inCommMode = WIFI_MODE;
		}

		//usCTOSS_LCDDisplay(" |Configuring Comms\nPlease wait...");
		vdDisplayMessageStatusBox(1, 8, "Configuring comms", MSG_PLS_WAIT, MSG_TYPE_PROCESS);		
		CTOS_Delay(1000);
		
		inUpdateCommsMode(inCommMode);

		vdDisplayMessageBox(1, 8, "Configuring Comms", "Done!", "", MSG_TYPE_SUCCESS);
		
		CTOS_Beep();
	    CTOS_Delay(1000);
	}	

    return d_OK;
}

int vdCTOS_ChangeTelco(void)
{
	int inRet;
	
	inRet = inCTOS_SelectTelco();

	return inRet;

}

int inSelectECRTrxn(void)
{
    int inRet;
    char szDebug[40 + 1]={0};
    char szSaleType[12][19+1];
    char szSaleMenu[1024];
    char szHeaderString[24+1];
    int bHeaderAttr = 0x01+0x04, key=0;
    int inLoop=0;
    int inIndex=0;
    int inSaleType = 0;
    BOOL isValid = TRUE;
    int inResult=0x0000;
    int inNumRecs =0;
    int i = 0;
    char szSelectedMenu[100];
    char szTrxFunc[100];

    vdDebug_LogPrintf("--inSelectECRTrxn--");
    vdDebug_LogPrintf("inMenuid[%d]", strTCT.inMenuid);

    memset(szHeaderString, 0x00, sizeof(szHeaderString));
    memset(szSaleType, 0x00, sizeof(szSaleType));
    memset(szSaleMenu, 0x00, sizeof(szSaleMenu));

    inReadBaseDMTrxMenu(&inNumRecs);

    vdDebug_LogPrintf("inGetSelectionMenu.inNumRecs[%d]", inNumRecs);

    strcat(szSaleMenu,  "SELECT TRANSACTION TYPE:|");

    for (i=0; i<4; i++)
    {
        if (strTrxMenu[i].szButtonItemLabel[0] != 0)
        {
            inIndex=i;
            strcat((char *)szSaleMenu, strTrxMenu[i].szButtonItemLabel);
            strcat((char *) szSaleMenu, (char *) "|");
            strcat((char *) szSaleMenu, strTrxMenu[i].szButtonItemImage);


            if (strTrxMenu[i+1].szButtonItemLabel[0] != 0)
                strcat((char *)szSaleMenu, (char *)" \n");
        }
    }

    vdDebug_LogPrintf("inSelectIdleTrxn.szSaleMenu[%s] len [%d]", szSaleMenu, strlen(szSaleMenu));

    if (inNumRecs > 0)
    {
        key = IdleMenuTransType(szSaleMenu);
		//key = MenuTransType(szSaleMenu);
        vdDebug_LogPrintf("SATURN inSelectIdleTrxn - MenuTransType KEY %d", key);

        if (key == 'C')
        {
            //vdDisplayErrorMsg(1, 8, "USER CANCEL");
            return d_NO;
        }
        else if (key == 0xFF)
        {
            vdDisplayMessageBox(1, 8, "", "TIME OUT", "", MSG_TYPE_TIMEOUT);
            CTOS_Beep();
            CTOS_Delay(1500);
            return -1;
        }

        if (key > 0)
        {
            memset(szSelectedMenu, 0x00, sizeof(szSelectedMenu));
            memset(szTrxFunc, 0x00, sizeof(szTrxFunc));
            strcpy(szSelectedMenu, strTrxMenu[key-1].szButtonItemLabel);
            //check if need to read submenu
            strcpy(szTrxFunc, strTrxMenu[key-1].szButtonItemFunc);
            vdDebug_LogPrintf("szSelectedMenu", strTrxMenu[key-1]);

            if((memcmp(szSelectedMenu, "MC/VISA/JCB/UPI", strlen(szSelectedMenu)) == 0) ||
               (memcmp(szSelectedMenu, "MC/VISA", strlen(szSelectedMenu)) == 0) ||
               (memcmp(szSelectedMenu, "SALE", strlen(szSelectedMenu)) == 0) ||
               (memcmp(szSelectedMenu, "INSTALLMENT", strlen(szSelectedMenu)) == 0))
            {
               inCTOS_WAVE_SALE();
            }
            else if(memcmp(szSelectedMenu, "BANCNET", strlen(szSelectedMenu)) == 0)
            {
                inCTOS_BancnetSale();
            }
			else if(memcmp(szSelectedMenu, "MCCPAY", strlen(szSelectedMenu)) == 0)
            {
                put_env_int("MCCPAYECRTRANS",1);
                inQRPAY();
				srTransRec.HDTid = get_env_int("ECRMCCPAYHOST");
				//inMultiAP_ECRSendSuccessResponse();
				//vdDebug_LogPrintf("AAA<><><> - inCTOS_MultiAPGetData 3");
            }
			else if (memcmp(szSelectedMenu, "PHQR", strlen(szSelectedMenu)) == 0) 
            {
                 put_env_int("MCCPAYECRTRANS",1);
                 inPHQRSale();
                 srTransRec.HDTid = get_env_int("ECRMCCPAYHOST");
            }
        }
        else
        {
            isValid = FALSE;
            vdDisplayErrorMsg(1, 8, "INVALID TRANS");
        }

    }

        return d_OK;
}

void vdCTOS_DeleteBatchPassword(void)
{
    int inRet=0;

	vdDebug_LogPrintf("vdCTOS_DeleteBatchPassword");
	
	srTransRec.byTransType = CLEAR_BATCH;
	inRet = inCTOS_GetTxnPassword();
	if (d_OK != inRet)
		return;

	vdCTOS_DeleteBatch();
}

void vdCTOS_SetWIFI(void)
{
	put_env_int("SET_WIFI", 1);
	
}

int inSystemPanel(void)
{
    int inResult;
	
	vdCTOS_SetTransType(SYSTEM_PANEL);
	inResult = inCTOS_GetTxnPassword();
    if(d_OK != inResult)
		return inResult;

    put_env_int("SYSTEM_PANEL", 1);
	
    vdDisplayMessageBox(1, 8, "Accesing System Panel", "", MSG_PLS_WAIT, MSG_TYPE_PROCESS);
    CTOS_Delay(1000);
	
	return 0;
}

