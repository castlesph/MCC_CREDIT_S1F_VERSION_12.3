#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <ctosapi.h>

#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>

#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <signal.h>
#include <pthread.h>
#include <sys/shm.h>
#include <linux/errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "..\ApTrans\MultiApTrans.h"
#include "..\Database\DatabaseFunc.h"

#include "..\Includes\POSTypedef.h"
#include "..\Includes\Wub_lib.h"

#include "..\Debug\Debug.h"

#include "..\ApTrans\MultiShareECR.h"
#include "..\Includes\MultiApLib.h"

USHORT usCTOSS_ECRInitialize(void)
{
    BYTE bInBuf[40];
    BYTE bOutBuf[40];
    BYTE *ptr = NULL;
    USHORT usInLen = 0;
    USHORT usOutLen = 0;
    USHORT usResult;
	char szAPName[50];
	int inAPPID;
	
	memset(bOutBuf,0x00,sizeof(bOutBuf));
	memset(szAPName,0x00,sizeof(szAPName));
	inMultiAP_CurrentAPNamePID(szAPName, &inAPPID);
    memset(bOutBuf, 0x00, sizeof(bOutBuf));    
	strcpy(bInBuf, szAPName);
	usInLen = strlen(szAPName);

    usResult = inMultiAP_RunIPCCmdTypes("com.persistent.app", d_IPC_CMD_ECR_Initialize, bInBuf, usInLen, bOutBuf, &usOutLen);
    //usResult = inMultiAP_RunIPCCmdTypes("SHARLS_ECR", d_IPC_CMD_ECR_Initialize, bInBuf, usInLen, bOutBuf, &usOutLen);
    vdDebug_LogPrintf("**usCTOSS_ECRInitialize [%d]**", usResult);
    if(d_OK == usResult)
    {
        //status
/*
        ptr = ptCTOSS_FindTagAddr(SHARE_EMV_RESP_STATU, bOutBuf, usOutLen);
        if (NULL == ptr)
        {
            usResult = d_NO;
        }
        else
        {
            usResult = ptr[SHARE_EMV_DEFINE_TAGS_LEN+SHARE_EMV_DEFINE_LEN];
        }
*/
    }
    
    return usResult;
}

USHORT usCTOSS_ECR_MultiDataGet(IN BYTE *pTagString, INOUT USHORT *pLen, OUT BYTE *pValue)
{
    BYTE bTagStringHex[256];
    BYTE bInBuf[256];
    BYTE bOutBuf[2048];
    BYTE *ptr = NULL;
    USHORT usTagStringLen = 0;
    USHORT usInLen = 0;
    USHORT usOutLen = 0;
    USHORT usResult;
    USHORT usDataLen = 0;

    inMultiAP_Database_EMVTransferDataInit();

    usTagStringLen = strlen(pTagString);
    wub_str_2_hex(pTagString, bTagStringHex, usTagStringLen);

    usInLen = 0;
    memset(bInBuf, 0x00, sizeof(bInBuf));
    //TagString
/*
    usInLen += usCTOSS_PackTagLenValue(&bInBuf[usInLen], SHARE_EMV_GET_MULTI_TAG, usTagStringLen, bTagStringHex);
    
    memset(bOutBuf, 0x00, sizeof(bOutBuf));    
    usResult = inMultiAP_RunIPCCmdTypes("SHARLS_ECR", d_IPC_CMD_ECR_GetPackageData, bInBuf, usInLen, bOutBuf, &usOutLen);
*/
     if(d_OK == usResult)
    {
        //status
/*        
        ptr = ptCTOSS_FindTagAddr(SHARE_EMV_RESP_STATU, bOutBuf, usOutLen);
        if(NULL == ptr)
        {
            usResult = d_NO;
        }
        else
        {
            usResult = ptr[SHARE_EMV_DEFINE_TAGS_LEN+SHARE_EMV_DEFINE_LEN];

            //pValue
            inMultiAP_Database_EMVTransferDataRead(&usDataLen, pValue);
            *pLen =  usDataLen; 
        }
*/
    }

    return usResult;
}

USHORT usCTOSS_ECR_MultiDataSet(IN USHORT usLen, IN BYTE *pValue)
{
    BYTE bInBuf[2048];
    BYTE bOutBuf[64];
    BYTE *ptr = NULL;
    USHORT usInLen = 0;
    USHORT usOutLen = 0;
    USHORT usResult = 0;

    inMultiAP_Database_EMVTransferDataInit();
    
    usInLen = 0;
    memset(bInBuf, 0x00, sizeof(bInBuf));
    //Tags TLV data
    inMultiAP_Database_EMVTransferDataWrite(usLen, pValue);

    memset(bOutBuf, 0x00, sizeof(bOutBuf));    
    //usResult = inMultiAP_RunIPCCmdTypes("SHARLS_ECR", d_IPC_CMD_ECR_SetPackageData, bInBuf, usInLen, bOutBuf, &usOutLen);
    usResult = inMultiAP_RunIPCCmdTypes("com.persistent.app", d_IPC_CMD_ECR_SetPackageData, bInBuf, usInLen, bOutBuf, &usOutLen);
    if(d_OK == usResult)
    {
        //status
/*        
        ptr = ptCTOSS_FindTagAddr(SHARE_EMV_RESP_STATU, bOutBuf, usOutLen);
        if(NULL == ptr)
        {
            usResult = d_NO;
        }
        else
        {
            usResult = ptr[SHARE_EMV_DEFINE_TAGS_LEN+SHARE_EMV_DEFINE_LEN];
        }
*/        
    }

    return usResult;
}

int inSelectECRTrans(void)
{
	int inRet;
	char szDebug[40 + 1]={0};
    char szSaleType[12][19+1];
    char szSaleMenu[1024];
	char szHeaderString[24+1];
	int bHeaderAttr = 0x01+0x04, key=0; 
	int inLoop=0;
	int inIndex=0;

	vdDebug_LogPrintf("inCTOS_SelectAccountType[START]");

	vdDebug_LogPrintf("-->>SELECT ACNT byTransType[%d]", srTransRec.byTransType);

    memset(szHeaderString, 0x00, sizeof(szHeaderString));
	memset(szSaleType, 0x00, sizeof(szSaleType));
	memset(szSaleMenu, 0x00, sizeof(szSaleMenu));

    if(strTCT.fECRSale == TRUE)   
    {
        strcat(&szSaleType[inIndex][0],"CREDIT SALE"); 
		inIndex++;
    }

    if(strTCT.fECRDebit== TRUE)   
    {
        strcat(&szSaleType[inIndex][0],"DEBIT SALE"); 
		inIndex++;
    }

    if(strTCT.fECRBancnet== TRUE)   
    {
        strcat(&szSaleType[inIndex][0],"BANCNET"); 
		inIndex++;
    }
	
    
    if(strTCT.fECRDiners== TRUE)   
    {
        strcat(&szSaleType[inIndex][0],"DINERS"); 
		inIndex++;
    }

    for (inLoop = 0; inLoop < 50; inLoop++)
    {
        if (szSaleType[inLoop][0]!= 0)
        {
			//inIndex=inLoop;
            strcat((char *)szSaleMenu, szSaleType[inLoop]);
            if (szSaleType[inLoop+1][0]!= 0)
                strcat((char *)szSaleMenu, (char *)" \n"); 
			vdDebug_LogPrintf("inLoop:(%d), szTransType[%s]", inLoop, szSaleType[inLoop]);
        }
        else
            break;
    }

    if(inIndex > 1)
    {
        strcpy(szHeaderString, "SALE MENU");
        key = MenuDisplay(szHeaderString,strlen(szHeaderString), bHeaderAttr, 1, 1, szSaleMenu, TRUE);
    }
    else if(inIndex == 1)
        key=1;    
	
    if (key > 0)
    {
      if (key == d_KBD_CANCEL)
      {
          return -1;
      }
    
      if(memcmp(szSaleType[key-1], "CREDIT SALE", strlen(szSaleType[key-1])) == 0)
      {
           return 1;           	   	
      }
      else if(memcmp(szSaleType[key-1], "DEBIT SALE", strlen(szSaleType[key-1])) == 0)
      {
           return 2;	   	
      }
      else if(memcmp(szSaleType[key-1], "DINERS", strlen(szSaleType[key-1])) == 0)
      {
           return 3;	   	
      }
      else if(memcmp(szSaleType[key-1], "BANCNET", strlen(szSaleType[key-1])) == 0)
      {
           return 4;	   	
      }
	  else
	  	vdDisplayErrorMsg(1, 8, "INVALID TRANS");
    } 
	else
	   vdDisplayErrorMsg(1, 8, "INVALID TRANS");

   return -1;	
}

