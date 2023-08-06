//
// Created by patri on 6/3/2018.
//

#include "patrick-lib.h"
#include <sqlite3.h>
#include "Database/DatabaseFunc.h"
#include "Debug/debug.h"
#include "Aptrans/MultiAptrans.h"
#include "Ctls/POSWave.h"
#include "Includes/POSSale.h"
#include "Ctls/POSCtls.h"
#include "Includes/POSVoid.h"
#include "Functionslist/POSFunctionsList.h"
#include "Includes/POSMain.h"
#include "Includes/CfgExpress.h"
#include "Includes/POSSetting.h"
#include "Includes/CardUtil.h"
#include "Includes/POSTrans.h"
#include "UIapi.h"
#include "Includes/POSReg.h"
#include "Ui/Display.h"
#include <ctosapi.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <dirent.h>
#include <vegaapi.h>
#include <android_jni_log.h>
#include <KMS2Client.h>

sqlite3 * db;
sqlite3_stmt *stmt;

extern JavaVM *jvm;
extern jclass activityClass;
extern jobject activityObj;
extern JNIEnv *g_env;


JavaVM *g_JavaVM;
extern jobject g_callback_obj_ctos;
extern jmethodID  g_callback_mid_bring2Front;

extern BOOL fEntryCardfromIDLE;
extern int inSelectedIdleAppsHost;

jmethodID g_callback_mid_lcd;
jmethodID  g_callback_mid_clear;
jmethodID  g_callback_mid_aligned;

jobject g_callback_obj_input;
jmethodID  g_callback_mid_showkeypad;
jmethodID  g_callback_mid_keypadmsg;
jmethodID  g_callback_mid_keypaddone;
jmethodID  g_callback_mid_InputAmountEx;

JavaVM *g_vm;

//ecr
jclass ecrClass;
jobject ecrObj;
JavaVM *ecrjvm;

int ing_KeyPressed = 0;
int printcopies_cntr = 0;

//settlement host
int inSettlementHost = 0;

static BOOL fUpdatedyear = 0;

JNIEXPORT jint
JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOSS_1TerminalStartUp(JNIEnv *env, jobject instance, jstring strSysYear) {
    // TODO


    BYTE bInBuf[40];
    BYTE bOutBuf[40];
    USHORT usInLen = 0;
    USHORT usOutLen = 0;

    char cwd[1024];
    int inRet=0;
	//for date saving
	BYTE szCurrDate[8] = {0};
	BYTE szBuf[8] = {0};
	CTOS_RTC SetRTC;
	int inReturn;
	
    //return d_OK;
    //inTCTRead(1);
    //strTCT.byRS232DebugPort = 8;
    //inTCTSave(1);

    vdDebug_LogPrintf("saturn TerminalStartUp");

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) strSysYear, NULL);
    if (str!=NULL)  {
        vdDebug_LogPrintf("set strSysYear env[%s],fUpdatedyear=%d", str, fUpdatedyear);
        //if(fUpdatedyear == 0) {
        //    inCTOSS_PutEnvDB("SYSYEAR",str);
        //    fUpdatedyear= 1;
        //}
    }

	
    if(activityClass != NULL)
        (*env)->DeleteGlobalRef(env, activityClass);
    if(activityObj != NULL)
        (*env)->DeleteGlobalRef(env, activityObj);
	
    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);


    //vdDebug_LogPrintf("inCTOSS_ProcessCfgExpress");
    //inCTOSS_ProcessCfgExpress();
    //vdDebug_LogPrintf("vdCTOS_InitWaveData");
    //vdCTOS_InitWaveData();

    inRet = inMultiAP_RunIPCCmdTypesEx("com.Source.SHARLS_EMV.SHARLS_EMV", 0x99, bInBuf, usInLen, bOutBuf, &usOutLen);
    vdDebug_LogPrintf("TerminalStartUp: SHARLS_EMV done - inRet = [%d]", inRet);
    inRet = inMultiAP_RunIPCCmdTypesEx("com.Source.SHARLS_COM.SHARLS_COM", 0x99, bInBuf, usInLen, bOutBuf, &usOutLen);
    vdDebug_LogPrintf("TerminalStartUp: SHARLS_COM done - inRet = [%d]", inRet);
    inRet = inMultiAP_RunIPCCmdTypesEx("com.Source.S1_MCCPAY.MCCPAY", 0x99, bInBuf, usInLen, bOutBuf, &usOutLen);
    vdDebug_LogPrintf("TerminalStartUp: MCCPAY done - inRet = [%d]", inRet);
    inRet = inMultiAP_RunIPCCmdTypesEx("com.Source.S1_BANCNET.BANCNET", 0x99, bInBuf, usInLen, bOutBuf, &usOutLen);
    vdDebug_LogPrintf("TerminalStartUp: BANCNET done - inRet = [%d]", inRet);

    vdDebug_LogPrintf("strTCT.fECR:%d", strTCT.fECR);
    if(strTCT.fECR)
    {
        if(inCallJAVA_checkAppRunning("com.persistent.app") == 0)
        {
            vdDebug_LogPrintf("1.Sharls ECR");
            inMultiAP_RunIPCCmdTypesEx("com.persistent.app", 0x99, bInBuf, usInLen, bOutBuf, &usOutLen);//Add to support ecr
        }
        else
            vdDebug_LogPrintf("2.Sharls ECR");
    }

    vdSetECRTransactionFlg(0);
    put_env_int("ECRPROCESS",0);
    put_env_int("APP_STARTED", 0);
	//TEMPORARY REMOVED

    srTransRec.byEntryMode = 0;
	//inCTLOS_Getpowrfail();//FOR TESTING
	inSettlementHost = 0;
	vdRemoveCard();//FOR TESTING

    vdReadUpdateMMT();

	inPrinterConfig(1);


	//if (get_env_int("CHECKYEAR") > 0)
		checkSystemDate();

    #ifdef CTMS_BACKGROUND_DL
	//put date comparison here
    CTOS_RTCGet(&SetRTC);
    memset(szCurrDate, 0x00, sizeof(szCurrDate));
    sprintf(szCurrDate,"%02d%02d%02d", SetRTC.bYear, SetRTC.bMonth, SetRTC.bDay);
    vdDebug_LogPrintf("s1f2 >> inCTOS_CTMSUPDATE_BackGround szCurrDate[%s]", szCurrDate);

	inCTOSS_GetEnvDB("CURRDATE", szBuf);
	vdDebug_LogPrintf("s1f2 szBuf %s", szBuf);
	vdDebug_LogPrintf("ENV CURDATE[%s],szCurrDate[%s]", szBuf, szCurrDate);
	//if (atoi(szCurrDate) != atoi(szBuf))
	if (strcmp (szCurrDate,szBuf) != 0)
	{
		inReturn = inCheckAllBatchEmtpy();
		put_env_char("CURRDATE",szCurrDate); //save current date
		vdDebug_LogPrintf("s1f2 - inCheckAllBatchEmtpy[%d]", inReturn);
        if(inReturn == 0){
        	inCTOS_CTMSUPDATE_BackGround();//check for update if batch not empty
        }
	}
	#endif

	 //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);
	
    return 0;
}

JNIEXPORT jint
JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOSS_1InitWaveData(JNIEnv *env, jobject instance) {
	// TODO

	int inRetVal = 0;
    BYTE outbuf[d_MAX_IPC_BUFFER];
    USHORT out_len = 0;

	BYTE bInBuf[40];
	BYTE bOutBuf[40];
	USHORT usInLen = 0;
	USHORT usOutLen = 0;


	char cwd[1024];
	BYTE szCurrDate[8] = {0};
	CTOS_RTC SetRTC;

	vdDebug_LogPrintf("saturn initwave");

	//inTCTRead(1);
	//strTCT.byRS232DebugPort = 8;
	//inTCTSave(1);

        if(activityClass != NULL)
               (*env)->DeleteGlobalRef(env, activityClass);
        if(activityObj != NULL)
               (*env)->DeleteGlobalRef(env, activityObj);

	jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);

    vdDebug_LogPrintf("inCTOSS_ProcessCfgExpress");
    inCTOSS_ProcessCfgExpress();
    vdDebug_LogPrintf("vdCTOS_InitWaveData");

	inTCTRead(1);

	//if(strTCT.fFirstInit == 1)
	//{
	//	strTCT.fFirstInit = 0;
	//	vdDebug_LogPrintf("fFirstInit inTCTSave 1");
	//	inTCTSave(1);
	//}

	vdSetECRTransactionFlg(0);
    put_env_int("ECRPROCESS",0);
	put_env_int("APP_STARTED", 0);

	vdReadUpdateMMT();

	vdCTOS_InitWaveData();

    inRetVal = inMultiAP_RunIPCCmdTypesEx("com.Source.SHARLS_EMV.SHARLS_EMV", 0x99, bInBuf, usInLen, bOutBuf, &usOutLen);
    vdDebug_LogPrintf("TerminalStartUp: SHARLS_EMV done - inRet = [%d]", inRetVal);
    inRetVal = inMultiAP_RunIPCCmdTypesEx("com.Source.SHARLS_COM.SHARLS_COM", 0x99, bInBuf, usInLen, bOutBuf, &usOutLen);
    vdDebug_LogPrintf("TerminalStartUp: SHARLS_COM done - inRet = [%d]", inRetVal);

    inRetVal = inMultiAP_RunIPCCmdTypesEx("com.Source.S1_MCCPAY.MCCPAY", 0x99, bInBuf, usInLen, bOutBuf, &usOutLen);
    vdDebug_LogPrintf("TerminalStartUp: MCCPAY done - inRet = [%d]", inRetVal);
    inRetVal = inMultiAP_RunIPCCmdTypesEx("com.Source.S1_BANCNET.BANCNET", 0x99, bInBuf, usInLen, bOutBuf, &usOutLen);
    vdDebug_LogPrintf("TerminalStartUp: BANCNET done - inRet = [%d]", inRetVal);

    srTransRec.byEntryMode = 0;

	//save first init date
    CTOS_RTCGet(&SetRTC);
    memset(szCurrDate, 0x00, sizeof(szCurrDate));
    sprintf(szCurrDate,"%02d%02d%02d", SetRTC.bYear, SetRTC.bMonth, SetRTC.bDay);
    vdDebug_LogPrintf("s1f2 >> inCTOS_CTMSUPDATE szCurrDate[%s]", szCurrDate);
    put_env_char("CURRDATE",szCurrDate);
	//end saving

	if(strTCT.fFirstInit == 1)
	{
		strTCT.fFirstInit = 0;
		vdDebug_LogPrintf("fFirstInit inTCTSave 1");
		inTCTSave(1);

		//Do auto-logon
    	inCTOS_Reg();

	}

	 //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);
	
    return 0;
}

JNIEXPORT jint
JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOSS_1Main(JNIEnv *env, jobject instance) {
	// TODO

	int inRetVal = 0;
    BYTE outbuf[d_MAX_IPC_BUFFER];
    USHORT out_len = 0;

	//inTCTRead(1);
	//strTCT.byRS232DebugPort = 8;
	//inTCTSave(1);

        if(activityClass != NULL)
             (*env)->DeleteGlobalRef(env, activityClass);
        if(activityObj != NULL)
             (*env)->DeleteGlobalRef(env, activityObj);

	jint rs = (*env)->GetJavaVM(env, &jvm);

	jclass cls = (*env)->GetObjectClass(env, instance);
	activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
	activityObj = (*env)->NewGlobalRef(env, instance);
	
	vdCTOS_InitWaveData();

	 //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);

    return 0;
}


int ePad_SignatureCaptureLibEex(DISPLAY_REC *szDisplayRec)
{
    unsigned char uszBuffer[100+1];
    int inRet = 0;

    vdDebug_LogPrintf("=====ePad_SignatureCaptureLibEex=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

	if (strlen(szDisplayRec)>0)
        strcpy(uszBuffer, szDisplayRec);

    //jstring jstr = (*env)->NewStringUTF(env, "NA");
	jstring jstr = (*env)->NewStringUTF(env, uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "ePad_SignatureCaptureLibEex_Java", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "ePad_SignatureCaptureLibEex_Java");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    if (str!=NULL)
    {
        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
    {
        vdDebug_LogPrintf("get nothing...");
    }

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end ePad_SignatureCaptureLibEex");

    //if(0 == strcmp(uszBuffer, "OK"))
    //    return d_OK;
    //else
    //    return d_NO;
    return d_OK;
}

JNIEXPORT jint
JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOSS_1Sale(JNIEnv *env, jobject instance, bool isEcr) {
    // TODO

    int inRetVal = 0;
    BYTE outbuf[d_MAX_IPC_BUFFER];
    USHORT out_len = 0;
    EMVCL_RC_DATA_EX stRCDataEx;

    char cwd[1024];
    //inRetVal = inTCTRead(1);
    vdDebug_LogPrintf("inTCTRead inRetVal[%d] strTCT.fDemo [%d]", inRetVal, strTCT.fDemo);
    //strTCT.byRS232DebugPort = 8;
    //strTCT.fDemo= 1;
    /*Here do test init, load TCT PRM*/
    //strTCT.fShareComEnable = 1;
    //strTCT.byERMMode  = 0;
	
    //inRetVal = inTCTSave(1);
    vdDebug_LogPrintf("inTCTSave inRetVal[%d]", inRetVal);
	
    if(activityClass != NULL)
        (*env)->DeleteGlobalRef(env, activityClass);
    if(activityObj != NULL)
        (*env)->DeleteGlobalRef(env, activityObj);

    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);

    vdDebug_LogPrintf("SATURN inCTOSS_Sale.............");

	if(isEcr == TRUE)
    {
		fEntryCardfromIDLE=VS_FALSE;
		vdSetECRTransactionFlg(VS_TRUE);
    }
	
     inCTOS_WAVE_SALE();
	 inCallJAVA_GetMenu();

     //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);
 
    return 0;
}

JNIEXPORT jint
JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOSS_1Void(JNIEnv *env, jobject instance, bool isEcr) {
    // TODO

    int inRetVal = 0;
    BYTE outbuf[d_MAX_IPC_BUFFER];
    USHORT out_len = 0;
    EMVCL_RC_DATA_EX stRCDataEx;

    char cwd[1024];

    //inRetVal = inTCTRead(1);
    //vdDebug_LogPrintf("inTCTRead inRetVal[%d] strTCT.fDemo [%d]", inRetVal, strTCT.fDemo);
    //strTCT.byRS232DebugPort = 8;
    //strTCT.fDemo= 0;
    /*Here do test init, load TCT PRM*/
    //strTCT.fShareComEnable = 1;

    inRetVal = inTCTSave(1);
    vdDebug_LogPrintf("inTCTSave inRetVal[%d]", inRetVal);

    if(activityClass != NULL)
        (*env)->DeleteGlobalRef(env, activityClass);
    if(activityObj != NULL)
        (*env)->DeleteGlobalRef(env, activityObj);

    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);


    vdDebug_LogPrintf("inCTOS_VOID.............");

	if(isEcr == TRUE)
    {
		fEntryCardfromIDLE=VS_FALSE;
		vdSetECRTransactionFlg(VS_TRUE);
    }
	
    inCTOS_VOID();
	inCallJAVA_GetMenu();

    //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);

    return 0;
}

JNIEXPORT jint
JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOSS_1Sale1(JNIEnv *env, jobject instance) {
    // TODO

    int inRetVal = 0;
    BYTE outbuf[d_MAX_IPC_BUFFER];
    USHORT out_len = 0;
    EMVCL_RC_DATA_EX stRCDataEx;

    char cwd[1024];

    //inRetVal = inTCTRead(1);
    vdDebug_LogPrintf("inTCTRead inRetVal[%d] strTCT.fDemo [%d]", inRetVal, strTCT.fDemo);
    //strTCT.byRS232DebugPort = 8;
    //strTCT.fDemo= 1;
    /*Here do test init, load TCT PRM*/
    //strTCT.fShareComEnable = 1;

    //inRetVal = inTCTSave(1);
    vdDebug_LogPrintf("saturn inTCTSave inRetVal[%d]", inRetVal);

    inCTOS_SALE();
    return 0;
}

JNIEXPORT jint
JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOSS_1Settle(JNIEnv *env, jobject instance) {
    // TODO

    int inRetVal = 0;
    BYTE outbuf[d_MAX_IPC_BUFFER];
    USHORT out_len = 0;
    EMVCL_RC_DATA_EX stRCDataEx;

    char cwd[1024];

    //inRetVal = inTCTRead(1);
    vdDebug_LogPrintf("inTCTRead inRetVal[%d] strTCT.fDemo [%d]", inRetVal, strTCT.fDemo);
    //strTCT.byRS232DebugPort = 8;
    //inRetVal = inTCTSave(1);
    vdDebug_LogPrintf("inTCTSave inRetVal[%d]", inRetVal);

    /*Here do test init, load TCT PRM*/
    //strTCT.fShareComEnable = 1;
    //strTCT.fDemo = 0;

    if(activityClass != NULL)
        (*env)->DeleteGlobalRef(env, activityClass);
    if(activityObj != NULL)
        (*env)->DeleteGlobalRef(env, activityObj);

    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);
	
    inCTOS_SETTLEMENT();

    //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);

    return 0;
}



JNIEXPORT jint
JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inDisplayMsg1(JNIEnv *env, jobject instance) {


    int inRetVal = 0;
    BYTE outbuf[d_MAX_IPC_BUFFER];
    USHORT out_len = 0;

    char cwd[1024];

    //inTCTRead(1);
    //strTCT.byRS232DebugPort = 8;
    //inTCTSave(1);

    if(activityClass != NULL)
        (*env)->DeleteGlobalRef(env, activityClass);
    if(activityObj != NULL)
        (*env)->DeleteGlobalRef(env, activityObj);

    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);

    vdDebug_LogPrintf("inDisplayMsg");
    inDisplayMsg1();

     //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);

    return 0;
}

JNIEXPORT jint
JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inDisplayImage(JNIEnv *env, jobject instance) {


    int inRetVal = 0;
    BYTE outbuf[d_MAX_IPC_BUFFER];
    USHORT out_len = 0;

    char cwd[1024];

    //inTCTRead(1);
    //strTCT.byRS232DebugPort = 8;
    //inTCTSave(1);

   if(activityClass != NULL)
        (*env)->DeleteGlobalRef(env, activityClass);
    if(activityObj != NULL)
        (*env)->DeleteGlobalRef(env, activityObj);

    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);

    vdDebug_LogPrintf("inDisplayImage");
    inDisplayImage();

     //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);

    return 0;
}


JNIEXPORT jint
JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inDisplayMsg(JNIEnv *env, jobject instance) {


    inDisplayMsg1();
    return 0;
}

int inCallJAVA_DisplayMultipleMessage(char *pszMsg)
{
    unsigned char uszBuffer[100+1];
    int inRet = 0;

    vdDebug_LogPrintf("=====inCallJAVA_DisplayMultipleMessage=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    vdDebug_LogPrintf("jstring[%s]", "This comes from inCallJAVA_DisplayMultipleMessage.");

    jstring jstr = (*env)->NewStringUTF(env, pszMsg);
    vdDebug_LogPrintf("jstring[%s][%s]", "This this Pass in string data to Java", pszMsg);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

//	jmethodID methodID = (*env)->GetMethodID(env, clazz, "rndGetNumStr", "(Ljava/lang/String;)Ljava/lang/String;");
    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "displayMultipleMsg", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "displayMultipleMsg");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
    //rs = (*jvm)->DetachCurrentThread(jvm);

    jbyte* str = NULL; //(*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        vdDebug_LogPrintf("strcpy");

        memset(uszBuffer, 0x00, sizeof(uszBuffer));
        strcpy(uszBuffer, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
    {
        vdDebug_LogPrintf("get nothing...");
    }

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_DisplayMultipleMessage");

    if(0 == strcmp(uszBuffer, "OK"))
        return d_OK;
    else
        return d_NO;
}


int inCallJAVA_DisplayUI(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[100+1];
    int inRet = 0;


    vdDebug_LogPrintf("saturn =====inCallJAVA_DisplayUI=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);



    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);


	vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "DisplayUI", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "DisplayUI");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    //(*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_DisplayUI");
    return d_OK;
}

int inCallJAVA_BackToProgress(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[100+1];
    int inRet = 0;


    vdDebug_LogPrintf("=====BackToProgress=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);



    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);


    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "BackToProgress", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "BackToProgress");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    //(*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end BackToProgress");
    return d_OK;
}

int inCallJAVA_GetMenu()
{
    vdDebug_LogPrintf("=====inCallJAVA_GetMenu=====");
    JNIEnv *env;

    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, clazz, "JNIGetMenu", "()V");

    (*env)->CallVoidMethod(env, activityObj, methodID);

    vdDebug_LogPrintf("=====inCallJAVA_GetMenu END=====");
    return d_OK;
}

int inCallJAVA_DisplayImage(int x, int y, char *pszMsg_Img)
{
    unsigned char uszBuffer[100+1];
    int inRet = 0;

    vdDebug_LogPrintf("=====inCallJAVA_DisplayImage=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    vdDebug_LogPrintf("jstring[%s]", "This comes from inCallJAVA_DisplayImage.");

    jstring jstr = (*env)->NewStringUTF(env, pszMsg_Img);
    vdDebug_LogPrintf("jstring[%s][%s]", "This this Pass in string data to Java", pszMsg_Img);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

//	jmethodID methodID = (*env)->GetMethodID(env, clazz, "rndGetNumStr", "(Ljava/lang/String;)Ljava/lang/String;");
    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "displayImage", "(IILjava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "inCallJAVA_DisplayImage");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, x, y, jstr);
    //rs = (*jvm)->DetachCurrentThread(jvm);

    jbyte* str = NULL; //(*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        vdDebug_LogPrintf("strcpy");

        memset(uszBuffer, 0x00, sizeof(uszBuffer));
        strcpy(uszBuffer, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
    {
        vdDebug_LogPrintf("get nothing...");
    }

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_DisplayImage");

    if(0 == strcmp(uszBuffer, "OK"))
        return d_OK;
    else
        return d_NO;
}

int inCallJAVA_MenuTransactions(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[512+1];
    int inRet = 0;


    vdDebug_LogPrintf("saturn =====inCallJAVA_MenuTransactions=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    vdDebug_LogPrintf("pbDispMsg[%s]", pbDispMsg);

    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);
    else
        strcpy(uszBuffer, "PASS IN MSG");

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

//	jmethodID methodID = (*env)->GetMethodID(env, clazz, "rndGetNumStr", "(Ljava/lang/String;)Ljava/lang/String;");
    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "MenuTransactions", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "MenuTransactions");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
    //rs = (*jvm)->DetachCurrentThread(jvm);

    jbyte* str = NULL; //(*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_MenuTransactions");
    return d_OK;
}

int inCallJAVA_MenuTransType(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[512+1];
    int inRet = 0;


    vdDebug_LogPrintf("saturn =====inCallJAVA_MenuTransType=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    vdDebug_LogPrintf("pbDispMsg[%s]", pbDispMsg);

    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);
    else
        strcpy(uszBuffer, "PASS IN MSG");

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

//	jmethodID methodID = (*env)->GetMethodID(env, clazz, "rndGetNumStr", "(Ljava/lang/String;)Ljava/lang/String;");
    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "MenuTransType", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "MenuTransType");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
    //rs = (*jvm)->DetachCurrentThread(jvm);

    jbyte* str = NULL; //(*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_MenuTransType");
    return d_OK;
}

int inCallJAVA_GetAmountString(BYTE *pbDispMsg, BYTE *pbAmtStr, BYTE *pbAmtLen)
{
    unsigned char uszBuffer[528+1];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_GetAmountString=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);
    //else
    //pbDispMsg = "Tine: UserConfirm activity";

    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", "This this Pass in string data to Java");

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

//	jmethodID methodID = (*env)->GetMethodID(env, clazz, "rndGetNumStr", "(Ljava/lang/String;)Ljava/lang/String;");
    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "GetAmountString", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "GetAmountString");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
    //rs = (*jvm)->DetachCurrentThread(jvm);

    jbyte* str = NULL; //(*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbAmtLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbAmtStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbAmtLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_GetAmountString");
    return d_OK;
}

int inCallJAVA_UserConfirmMenu(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[d_MAX_IPC_BUFFER];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_UserConfirmMenu=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);



    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);


	vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "UserConfirmMenu", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "UserConfirmMenu");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_UserConfirmMenu");
    return d_OK;
}

int inCallJAVA_UserConfirmMenu3(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[d_MAX_IPC_BUFFER];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_UserConfirmMenu3=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);



    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);


    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "UserConfirmMenu3", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "UserConfirmMenu3");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_UserConfirmMenu3");
    return d_OK;
}


int inCallJAVA_UserConfirmMenuTip(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[d_MAX_IPC_BUFFER];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_UserConfirmMenuTip=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);



    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);


    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "UserConfirmMenuTip", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "UserConfirmMenu");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_UserConfirmMenu");
    return d_OK;
}


int inCallJAVA_UserSelectUpDown(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[100+1];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_UserSelectUpDown=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);



    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);


	vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_NPX/NPX/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_NPX/NPX/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "UserSelectUpDown", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "inCallJAVA_UserSelectUpDown");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;

	if(result != NULL)
	    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_UserSelectUpDown");
    return d_OK;
}


int inCallJAVA_UserInputString(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[100+1];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_UserInputString=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    vdDebug_LogPrintf("pbDispMsg[%s]", pbDispMsg);

    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);
    else
        strcpy(uszBuffer, "PASS IN MSG");


	vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

//	jmethodID methodID = (*env)->GetMethodID(env, clazz, "rndGetNumStr", "(Ljava/lang/String;)Ljava/lang/String;");
    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "UserInputString", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "UserInputString");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
    //rs = (*jvm)->DetachCurrentThread(jvm);

    jbyte* str = NULL; //(*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_UserInputString");
    return d_OK;
}

int inCallJAVA_S1InputString(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[1024+1];
    int inRet = 0;


    vdDebug_LogPrintf("saturn =====inCallJAVA_S1InputString=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("SATURN jint[%d] *env[%x]", rs, *env);

    vdDebug_LogPrintf("saturn pbDispMsg[%s]", pbDispMsg);

    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);
    else
        strcpy(uszBuffer, "PASS IN MSG");


    vdDebug_LogPrintf("saturn uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("saturn jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("saturn jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

//	jmethodID methodID = (*env)->GetMethodID(env, clazz, "rndGetNumStr", "(Ljava/lang/String;)Ljava/lang/String;");
    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "InputStringUI", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("saturn test jstring[%s]", "InputStringUI");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
    //rs = (*jvm)->DetachCurrentThread(jvm);


	vdDebug_LogPrintf("saturn inCallJAVA_S1InputString test");

    jbyte* str = NULL; //(*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_S1InputString");
    return d_OK;
}

int inCallJAVA_EnterAnyNum(char *pbNumLen, char *pbaNum)
{
    unsigned char uszBuffer[100+1];
    int inRet = 0;

    char baPINBlk[8];

    vdDebug_LogPrintf("=====inCallJAVA_EnterAnyNum=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    vdDebug_LogPrintf("jstring[%s]", "This comes from CallJavaForNumString.");

    jstring jstr = (*env)->NewStringUTF(env, "This this Pass in string data to Java");
    vdDebug_LogPrintf("jstring[%s]", "This this Pass in string data to Java");

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

//	jmethodID methodID = (*env)->GetMethodID(env, clazz, "rndGetNumStr", "(Ljava/lang/String;)Ljava/lang/String;");
    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "getAnyNumStr", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "getAnyNumStr");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
    //rs = (*jvm)->DetachCurrentThread(jvm);

    jbyte* str = NULL; //(*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbNumLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbaNum, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);


        vdDebug_LogPrintf("Copy data back");
        if (0 == strcmp(str, "BYPASS"))
            *pbNumLen = 0;
    }
    else
        *pbNumLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_EnterAnyNum");
    return d_OK;
}

int inCallJAVA_DOptionMenuDisplay(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[100+1];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_DOptionMenuDisplay=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    vdDebug_LogPrintf("pbDispMsg[%s]", pbDispMsg);

    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);
    else
        strcpy(uszBuffer, "PASS IN MSG");

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

//	jmethodID methodID = (*env)->GetMethodID(env, clazz, "rndGetNumStr", "(Ljava/lang/String;)Ljava/lang/String;");
    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "DOptionMenuDisplay", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "DOptionMenuDisplay");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
    //rs = (*jvm)->DetachCurrentThread(jvm);

    jbyte* str = NULL; //(*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_DOptionMenuDisplay");
    return d_OK;
}

int inCallJAVA_DPopupMenuDisplay(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[1024+1];
    int inRet = 0;


    vdDebug_LogPrintf("saturn =====inCallJAVA_DPopupMenuDisplay=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    vdDebug_LogPrintf("pbDispMsg[%s]", pbDispMsg);

    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);
    else
        strcpy(uszBuffer, "PASS IN MSG");

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

//	jmethodID methodID = (*env)->GetMethodID(env, clazz, "rndGetNumStr", "(Ljava/lang/String;)Ljava/lang/String;");
    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "DPopupMenuDisplay", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "DPopupMenuDisplay");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
    //rs = (*jvm)->DetachCurrentThread(jvm);

    jbyte* str = NULL; //(*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_DPopupMenuDisplay");
    return d_OK;
}

int inCallJAVA_PrintReceiptUI(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[128+1];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_PrintReceiptUI=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);



    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);


    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "ReceiptOnScreen", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "ReceiptOnScreen");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_PrintReceiptUI");
    return d_OK;
}

int inCallJAVA_PrintFirstReceiptUI(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[128+1];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_PrintFirstReceiptUI=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);



    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);


    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "FirstReceiptOnScreen", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "FirstReceiptOnScreen");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_PrintFirstReceiptUI");
    return d_OK;
}

int inCallJAVA_EliteReceiptUI(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[128+1];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_EliteReceiptUI=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);



    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);


    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "EliteReceiptOnScreen", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "EliteReceiptOnScreen");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_EliteReceiptUI");
    return d_OK;
}

/*below is the new way to display UI*/
#if 0
USHORT CTOS_LCDTClearDisplay(void)
{
    JNIEnv *tenv;
    LOGD("---JNI AP CB--- CTOS_LCDTClearDisplay\n");
    if((*g_JavaVM)->AttachCurrentThread(g_JavaVM, &tenv, NULL) != JNI_OK)
    {
        // attachCurrentThread() failed.
    }
    else {
        (*tenv)->CallLongMethod(tenv, g_callback_obj_ctos, g_callback_mid_clear);
    }
    return 0;
}


USHORT CTOS_LCDTPrintXY(USHORT usX, USHORT usY, UCHAR* pbBuf)
{
    JNIEnv *tenv;
    jshort jsusX;
    jshort jsusY;

    LOGD("---JNI AP CB--- CTOS_LCDTPrintXY\n");
    if((*g_JavaVM)->AttachCurrentThread(g_JavaVM, &tenv, NULL) != JNI_OK)
    {
        // attachCurrentThread() failed.
    }
    else {
        jsusX = (jshort) usX;
        jsusY = (jshort) usY;
        jstring str = (*tenv)->NewStringUTF(tenv, (char*)pbBuf);
        (*tenv)->CallLongMethod(tenv, g_callback_obj_ctos, g_callback_mid_lcd, jsusX, jsusY, str);
	 (*tenv)->DeleteLocalRef(tenv, str);
    }



    return 0;
}


USHORT CTOS_LCDTPrintAligned(USHORT usY, UCHAR* pbBuf, BYTE bMode)
{
    JNIEnv *tenv;
    jshort jsusY;
    jbyte jbbMode;

    LOGD("---JNI AP CB--- CTOS_LCDTPrintAligned\n");
    if((*g_JavaVM)->AttachCurrentThread(g_JavaVM, &tenv, NULL) != JNI_OK)
    {
        // attachCurrentThread() failed.
    }
    else {
        jsusY = (jshort) usY;
        jbbMode = (jbyte) bMode;
        jstring str = (*tenv)->NewStringUTF(tenv, (char*)pbBuf);
        (*tenv)->CallLongMethod(tenv, g_callback_obj_ctos, g_callback_mid_aligned, jsusY, str, jbbMode);
    }
    return 0;
}
#endif

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
    JNIEnv *env;
    g_vm = vm;
    if ((*vm)->GetEnv(vm, (void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR; // JNI version not supported.
    }

    return JNI_VERSION_1_6;
}

JNIEXPORT jlong JNICALL Java_com_Source_S1_1MCC_MCC_Main_JNI_1Lib_REG_1CB_1CTOS(
        JNIEnv *env, jobject obj, jobject instance)
{
    int Rtn = 0;

    LOGD("SATURN ---JNI AP CB--- SET CALLBACK \n");

    if(activityClass != NULL)
        (*env)->DeleteGlobalRef(env, g_callback_obj_ctos);

    (*env)->GetJavaVM(env, &g_JavaVM);
    g_callback_obj_ctos = (*env)->NewGlobalRef(env,instance);
    jclass clz = (*env)->GetObjectClass(env,g_callback_obj_ctos);


    if(clz == NULL)
    {
        //failed to find class
        LOGD("SATURN ---JNI AP CB--- failed to find class \n");
    }

    g_callback_mid_lcd = (*env)->GetMethodID(env, clz, "CTOS_LCDTPrintXY", "(SSLjava/lang/String;)J");
    g_callback_mid_aligned = (*env)->GetMethodID(env, clz, "CTOS_LCDTPrintAligned", "(SLjava/lang/String;B)J");
    g_callback_mid_clear = (*env)->GetMethodID(env, clz, "CTOS_LCDTClearDisplay", "()J");
    g_callback_mid_bring2Front = (*env)->GetMethodID(env, clz, "CTOS_bring2Front", "()J");

    //g_callback_mid_pin_result = env->GetMethodID(clz, "CTOS_PINGetResult", "([B)J");

    JavaVM *javaVM = g_vm;
    jint res = (*javaVM)->GetEnv(javaVM, (void **) &env, JNI_VERSION_1_6);
    if (res != JNI_OK) {
        res = (*javaVM)->AttachCurrentThread(javaVM, &env, NULL);
        if (JNI_OK != res) {
            LOGE("Failed to AttachCurrentThread, ErrorCode = %d", res);

        }
    }

    if(clz != NULL)
        (*env)->DeleteLocalRef(env, clz);

    return Rtn;
}


int ShowVirtualKeypPad(OUT USHORT *pusKeyPadButtonNum, OUT BYTE *pbKeyPadButtonInfo, OUT USHORT *pusKeyPadButtonInfoLen)
{
    JNIEnv* env = NULL;

    LOGD("---JNI AP CB--- ShowVirtualKeypPad\n");

    //inCallJAVA_GetAmountString(pusKeyPadButtonNum, pbKeyPadButtonInfo, pusKeyPadButtonInfoLen);
    //return d_OK;

    if((*g_JavaVM)->AttachCurrentThread(g_JavaVM, &env, NULL) != JNI_OK)
    {
        // attachCurrentThread() failed.
    }
    else {

    }

    int buffsize = 1024;
    int i = 0;

    BYTE baInputbuffer[1024];
    BYTE baOutputbuffer[1024];

    //jbyte *by = (jbyte*)baInputbuffer;

    jbyteArray jarray = (*env)->NewByteArray(env, buffsize);

    (*env)->SetByteArrayRegion(env, jarray, 0, buffsize, (jbyte*)baInputbuffer);

    LOGE("buffsize= %d", buffsize);

    (*env)->CallLongMethod(env, g_callback_obj_input, g_callback_mid_showkeypad, jarray);

    //jsize len = env->GetArrayLength (jarray);
    //unsigned char* buf = new unsigned char[len];

    (*env)->GetByteArrayRegion (env, jarray, 0, buffsize, (jbyte*)(baOutputbuffer));

    *pusKeyPadButtonNum = baOutputbuffer[0];

    LOGE("baOutputbuffer[0]= %02X", baOutputbuffer[0]);
    LOGE("baOutputbuffer[1]= %02X", baOutputbuffer[1]);
    LOGE("baOutputbuffer[2]= %02X", baOutputbuffer[2]);

    *pusKeyPadButtonInfoLen = 0;
    *pusKeyPadButtonInfoLen += (baOutputbuffer[1]*256);
    *pusKeyPadButtonInfoLen += (baOutputbuffer[2]);

    LOGE("pusPINPadButtonNum= %d", *pusKeyPadButtonNum);
    LOGE("pusPINPadButtonInfoLen= %d", *pusKeyPadButtonInfoLen);


    //memcpy(pbKeyPadButtonInfo, &baOutputbuffer[3], *pusKeyPadButtonInfoLen);

    return 0;
}


int GetKeyPadDone(void)
{
    JNIEnv *tenv;
    jshort jsusX;
    jshort jsusY;

    LOGD("---JNI AP CB--- GetKeyPadDone\n");
    if((*g_JavaVM)->AttachCurrentThread(g_JavaVM, &tenv, NULL) != JNI_OK)
    {
        // attachCurrentThread() failed.
    }
    else {
        (*tenv)->CallLongMethod(tenv, g_callback_obj_input, g_callback_mid_keypaddone);
    }
    return 0;

}


int ShowKeyPadMsg(BYTE digitsNum, BYTE bPINType, BYTE bRemainingCounter)
{
    JNIEnv *tenv;
    jbyte digits;


    LOGD("---JNI AP CB--- ShowKeyPadMsg\n");
    if((*g_JavaVM)->AttachCurrentThread(g_JavaVM, &tenv, NULL) != JNI_OK)
    {
        // attachCurrentThread() failed.
    }
    else {
        digits = (jbyte) digitsNum;
        (*tenv)->CallLongMethod(g_callback_obj_input, g_callback_mid_keypadmsg, digits, bPINType, bRemainingCounter);
    }
    return 0;

}

JNIEXPORT jstring JNICALL
Java_com_Source_S1_1MCC_MCC_Trans_AmountEntryActivity_vdCTOSS_1FormatAmountUI(JNIEnv *env,
                                                                              jobject instance,
                                                                              jstring szFmt_,
                                                                              jstring szInAmt_) {
    const char *szFmt = (*env)->GetStringUTFChars(env, szFmt_, 0);
    const char *szInAmt = (*env)->GetStringUTFChars(env, szInAmt_, 0);
    char temp[24+1];

    // TODO
    memset(temp, 0, sizeof(temp));
    vdCTOSS_FormatAmount(szFmt, szInAmt, temp);

    (*env)->ReleaseStringUTFChars(env, szFmt_, szFmt);
    (*env)->ReleaseStringUTFChars(env, szInAmt_, szInAmt);

    return (*env)->NewStringUTF(env, temp);
}

int inCallJAVA_UserConfirmMenuInvandAmt(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[512+1];
    int inRet = 0;


    vdDebug_LogPrintf("saturn =====inCallJAVA_UserConfirmMenu=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);


  if (strlen(pbDispMsg)>0)
	  strcpy(uszBuffer, pbDispMsg);
    //else
		//pbDispMsg = "Tine: UserConfirm activity";

	vdDebug_LogPrintf("saturn uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("saturn jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("saturn jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "UserConfirmMenuInvandAmt", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("saturn jstring[%s]", "UserConfirmMenuInvandAmt");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_UserConfirmMenu");
    return d_OK;
}


JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOSS_1REPORTS(JNIEnv *env, jobject instance) {

    // TODO
    int inRetVal = 0;
    BYTE outbuf[d_MAX_IPC_BUFFER];
    USHORT out_len = 0;

    char cwd[1024];

    //inTCTRead(1);
    //strTCT.byRS232DebugPort = 8;
    //inTCTSave(1);

    if(activityClass != NULL)
        (*env)->DeleteGlobalRef(env, activityClass);
    if(activityObj != NULL)
        (*env)->DeleteGlobalRef(env, activityObj);

    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);

    vdDebug_LogPrintf("inReportSelecion");
    inReportSelecion();

     //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);

    return 0;
}

int inCallJAVA_LCDDisplay(BYTE *pbDispMsg, BYTE *pbOutStr_disp, BYTE *pbOutLen)
{
    unsigned char uszBuffer[528];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_LCDDisplay=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);


    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);

    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "LCDDisplay", "(Ljava/lang/String;)Ljava/lang/String;");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr_disp, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
    {
		*pbOutLen = 0;
		//strcpy(pbOutStr, "OK");
    }


    (*env)->DeleteLocalRef(env, jstr);
    //(*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_LCDDisplay");
    return d_OK;
}

int inCallJAVA_usCARDENTRY(BYTE *pbDispMsg, BYTE *pbOutStr_cardentry, BYTE *pbOutLen)
{
    unsigned char uszBuffer[512+1];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_usCARDENTRY=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);



    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);


    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    //jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    //vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "usCARDENTRY", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "usCARDENTRY");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr_cardentry, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
    {
        *pbOutLen = 0;
        //strcpy(pbOutStr, "OK");
    }


    (*env)->DeleteLocalRef(env, jstr);
    //(*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_usCARDENTRY");
    return d_OK;
}

int inCallJAVA_usEditDatabase(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[128+1];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_usEditDatabase=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);



    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);


    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "usEditDatabase", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "usEditDatabase");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_usEditDatabase");
    return d_OK;
}


//int inCallJAVA_usGetSerialNumber(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
int inCallJAVA_usGetSerialNumber(BYTE *pbOutStr, BYTE *pbOutLen)

{
    unsigned char uszBuffer[128+1];
    int inRet = 0;


    vdDebug_LogPrintf("saturn =====inCallJAVA_usGetSerialNumber=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);



    //if (strlen(pbDispMsg)>0)
    //    strcpy(uszBuffer, pbDispMsg);


    vdDebug_LogPrintf("saturn uszBuffer[%s]", uszBuffer);
	strcpy(uszBuffer, "test");

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "GetSerialNumber", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "GetSerialNumber");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_usGetSerialNumber");
    return d_OK;
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOSS_1BUTTONCANCEL(JNIEnv *env, jobject instance) {

    vdDebug_LogPrintf("saturn inCTOSS_BUTTONCANCEL.............");
    CTOS_KBDBufPut('C'); // patrick hit cancel 20190406
    ing_KeyPressed = 'C';
    vdDebug_LogPrintf("ing_KeyPressed = [%d]", ing_KeyPressed);
    CTOS_Delay(300);  
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOSS_1CARDENTRYTIMEOUT(JNIEnv *env,
                                                                        jobject instance) {
    vdDebug_LogPrintf("inCTOSS_CARDENTRYTIMEOUT.............");
    CTOS_KBDBufPut('T'); // patrick hit cancel 20190406
    ing_KeyPressed = 'T';
    vdDebug_LogPrintf("ing_KeyPressed = [%d]", ing_KeyPressed);
    CTOS_Delay(300);
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOSS_1PRINT_1EMVTAGS(JNIEnv *env,
                                                                      jobject instance) {

    // TODO

    vdDebug_LogPrintf("saturn print emv tags");
    vdPrintEMVTags();

}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOSS_1Registration(JNIEnv *env, jobject instance) {


	int inRetVal = 0;
	BYTE outbuf[d_MAX_IPC_BUFFER];
	USHORT out_len = 0;
	EMVCL_RC_DATA_EX stRCDataEx;

	char cwd[1024];

	//inRetVal = inTCTRead(1);
	vdDebug_LogPrintf("inTCTRead inRetVal[%d] strTCT.fDemo [%d]", inRetVal, strTCT.fDemo);
	//strTCT.byRS232DebugPort = 8;
	//strTCT.fDemo= 1;
	/*Here do test init, load TCT PRM*/
	//strTCT.fShareComEnable = 1;
	//strTCT.byERMMode  = 0;

	//inRetVal = inTCTSave(1);
	vdDebug_LogPrintf("inTCTSave inRetVal[%d]", inRetVal);

         if(activityClass != NULL)
              (*env)->DeleteGlobalRef(env, activityClass);
         if(activityObj != NULL)
              (*env)->DeleteGlobalRef(env, activityObj);

	jint rs = (*env)->GetJavaVM(env, &jvm);

	jclass cls = (*env)->GetObjectClass(env, instance);
	activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
	activityObj = (*env)->NewGlobalRef(env, instance);

	vdDebug_LogPrintf("SATURN inCTOS_Reg.............");
	inCTOS_Reg();

	 //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);

}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOSS_1INSTALLMENT(JNIEnv *env, jobject instance) {

    // TODO
    int inRetVal = 0;
    BYTE outbuf[d_MAX_IPC_BUFFER];
    USHORT out_len = 0;
    EMVCL_RC_DATA_EX stRCDataEx;

    char cwd[1024];

    //inRetVal = inTCTRead(1);
    vdDebug_LogPrintf("inTCTRead inRetVal[%d] strTCT.fDemo [%d]", inRetVal, strTCT.fDemo);
    //strTCT.byRS232DebugPort = 8;
    //strTCT.fDemo= 1;
    /*Here do test init, load TCT PRM*/
   // strTCT.fShareComEnable = 1;
    //strTCT.byERMMode  = 0;

    //inRetVal = inTCTSave(1);
    vdDebug_LogPrintf("inTCTSave inRetVal[%d]", inRetVal);

    if(activityClass != NULL)
        (*env)->DeleteGlobalRef(env, activityClass);
    if(activityObj != NULL)
        (*env)->DeleteGlobalRef(env, activityObj);

    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);

    vdDebug_LogPrintf("SATURN inCTOS_INSTALLMENT.............");
    inCTOS_INSTALLMENT();

     //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);

    return 0;
}

JNIEXPORT jstring JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_ReadTrxMenu(JNIEnv *env, jobject instance) {

    // TODO
   //inTCTRead(1);
    //vdDebug_LogPrintf("inTCTRead:  strTCT.inMenuid = [%d]", strTCT.inMenuid);
    //strTCT.byRS232DebugPort = 8;
    //inTCTSave(1);

    vdDebug_LogPrintf("TINE:  Get App Transactions from Dynamic Menu database");
    vdDebug_LogPrintf("TINE:  strTCT.inMenuid = [%d]", strTCT.inMenuid);
    char szTrxn[20][20];
    char szItemImage[20][40];
    char szTrxnMenu[256+1];
    int inTrxnID[20];
    int inRet;
    int inLoop=0;

    memset(szTrxn,0x00, sizeof(szTrxn));
    memset(szItemImage,0x00, sizeof(szItemImage));

    inRet = inReadTrxMenu(szTrxn, szItemImage, inTrxnID);
    if(inRet != d_OK)
        return "FAIL";

    memset(szTrxnMenu,0x00, sizeof(szTrxnMenu));

    for (inLoop = 0; inLoop < 150; inLoop++) {
        if (szTrxn[inLoop][0] != 0) {
            strcat((char *) szTrxnMenu, szTrxn[inLoop]);
            strcat((char *) szTrxnMenu, (char *) "|");
            strcat((char *) szTrxnMenu, szItemImage[inLoop]);
            if (szTrxn[inLoop + 1][0] != 0)
                strcat((char *) szTrxnMenu, (char *) " \n");

            vdDebug_LogPrintf("szCustomerMenu %s", szTrxnMenu);
        } else
            break;
    }

    vdDebug_LogPrintf("szCustomerMenu2 %s %d", szTrxnMenu, strlen(szTrxnMenu));

    return (*env)->NewStringUTF(env, szTrxnMenu);
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOSS_1Debit(JNIEnv *env, jobject instance) {

    int inRetVal = 0;
    BYTE outbuf[d_MAX_IPC_BUFFER];
    USHORT out_len = 0;
    EMVCL_RC_DATA_EX stRCDataEx;

    char cwd[1024];

    //inRetVal = inTCTRead(1);
    vdDebug_LogPrintf("inTCTRead inRetVal[%d] strTCT.fDemo [%d]", inRetVal, strTCT.fDemo);
    //strTCT.byRS232DebugPort = 8;
    //strTCT.fDemo= 1;
    /*Here do test init, load TCT PRM*/
    //strTCT.fShareComEnable = 1;
    //strTCT.byERMMode  = 0;

    //inRetVal = inTCTSave(1);
    vdDebug_LogPrintf("inTCTSave inRetVal[%d]", inRetVal);

   if(activityClass != NULL)
        (*env)->DeleteGlobalRef(env, activityClass);
    if(activityObj != NULL)
        (*env)->DeleteGlobalRef(env, activityObj);

    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);

    vdDebug_LogPrintf("SATURN debit sale.............");
     inCTOS_BancnetSale();

      //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);

    return 0;
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOSS_1Balance(JNIEnv *env, jobject instance) {


	int inRetVal = 0;
	BYTE outbuf[d_MAX_IPC_BUFFER];
	USHORT out_len = 0;
	EMVCL_RC_DATA_EX stRCDataEx;

	char cwd[1024];

	//inRetVal = inTCTRead(1);
	vdDebug_LogPrintf("inTCTRead inRetVal[%d] strTCT.fDemo [%d]", inRetVal, strTCT.fDemo);
	//strTCT.byRS232DebugPort = 8;
	//strTCT.fDemo= 1;
	/*Here do test init, load TCT PRM*/
	//strTCT.fShareComEnable = 1;
	//strTCT.byERMMode  = 0;

	//inRetVal = inTCTSave(1);
	vdDebug_LogPrintf("inTCTSave inRetVal[%d]", inRetVal);

        if(activityClass != NULL)
              (*env)->DeleteGlobalRef(env, activityClass);
        if(activityObj != NULL)
              (*env)->DeleteGlobalRef(env, activityObj);

	jint rs = (*env)->GetJavaVM(env, &jvm);

	jclass cls = (*env)->GetObjectClass(env, instance);
	activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
	activityObj = (*env)->NewGlobalRef(env, instance);

	vdDebug_LogPrintf("SATURN debit sale.............");
 	inCTOS_BancnetBalInq();

	 //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);

return 0;

}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOSS_1START(JNIEnv *env, jobject instance) {

    // TODO
    int inRetVal = 0;
    BYTE outbuf[d_MAX_IPC_BUFFER];
    USHORT out_len = 0;
    EMVCL_RC_DATA_EX stRCDataEx;

    char cwd[1024];

    //inRetVal = inTCTRead(1);
   // vdDebug_LogPrintf("inTCTRead inRetVal[%d] strTCT.fDemo [%d]", inRetVal, strTCT.fDemo);
    //strTCT.byRS232DebugPort = 8;
    //strTCT.fDemo= 1;
    /*Here do test init, load TCT PRM*/
    //strTCT.fShareComEnable = 1;
    //strTCT.byERMMode  = 0;

    //inRetVal = inTCTSave(1);
    vdDebug_LogPrintf("inTCTSave inRetVal[%d]", inRetVal);

     if(activityClass != NULL)
              (*env)->DeleteGlobalRef(env, activityClass);
     if(activityObj != NULL)
              (*env)->DeleteGlobalRef(env, activityObj);

    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);

    vdDebug_LogPrintf("inCTOS_WAVE_START.............");
    inCTOS_WAVE_START();

    //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);


    return 1;

}

JNIEXPORT jstring JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_GetTrxTypes(JNIEnv *env, jobject instance) {

    // TODO
    char szTrxn[20][20];
    char szItemImage[20][40];
    char szTrxnMenu[100 + 1];
    int inTrxnID[20];
    int inRet;
    int inLoop = 0;

    memset(szTrxn, 0x00, sizeof(szTrxn));
    memset(szItemImage, 0x00, sizeof(szItemImage));

    inRet = inReadTrxTypes(szTrxn, szItemImage, inTrxnID);
    if (inRet != d_OK)
        return "FAIL";

    memset(szTrxnMenu, 0x00, sizeof(szTrxnMenu));
    strcat(szTrxnMenu, "Select Transaction Type: |");

    for (inLoop = 0; inLoop < 150; inLoop++) {
        if (szTrxn[inLoop][0] != 0) {
            strcat((char *) szTrxnMenu, szTrxn[inLoop]);
            strcat((char *) szTrxnMenu, (char *) "|");
            strcat((char *) szTrxnMenu, szItemImage[inLoop]);
            if (szTrxn[inLoop + 1][0] != 0)
                strcat((char *) szTrxnMenu, (char *) " \n");

            vdDebug_LogPrintf("szCustomerMenu %s", szTrxnMenu);
        } else
            break;
    }

    vdDebug_LogPrintf("szCustomerMenu2 %s %d", szTrxnMenu, strlen(szTrxnMenu));

    return (*env)->NewStringUTF(env, szTrxnMenu);

}

JNIEXPORT jstring JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_GetAppHeaderDetails(JNIEnv *env, jobject instance) {

    // TODO
    char szTrxn[20][20];
    char szHeader[256+1];
    int inRet, inSeek, inLoop;
    char reg_status[2];
    char idleMode[2];
    char AppType[6] = "MCCAPP";

    if(activityClass != NULL)
        (*env)->DeleteGlobalRef(env, activityClass);
    if(activityObj != NULL)
        (*env)->DeleteGlobalRef(env, activityObj);

    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);

    inTCTRead(1);

    strGBLVar.inGBLvAppType= get_env_int("APPTYPE"); 

    vdDebug_LogPrintf("strTCT.fRegister=[%d]", strTCT.fRegister);
    sprintf(reg_status, "%d", strTCT.fRegister);
    vdDebug_LogPrintf("reg_status=[%s]", reg_status);
    sprintf(idleMode, "%d", strTCT.inIdleMode);
    strTCT.fShareComEnable = 1;
    inRet = inTCTSave(1);
    vdDebug_LogPrintf("inRet = [%d]", inRet);

    fEntryCardfromIDLE = FALSE;

    memset(szTrxn, 0x00, sizeof(szTrxn));

    inSeek = strTCT.inMMTid;
    vdDebug_LogPrintf("strTCT.inMMTid::inSeek = [%d]", inSeek);
    //inRet = inMMTRead(inSeek, szTrxn);

	// sidumili: AppHeaderDetails to load MMT record per apps
	switch (inSeek)
	{
		case 1: // credit
			inMMTReadRecord(19,1);
		break;
		case 2: // bancnet
			inMMTReadRecord(12,1);
		break;
		case 3: // mccpay
			inMMTReadRecord(20,1);
		break;
		default:
			inMMTReadRecord(19,1);
			break;
	}
	
    //if (inMMTReadRecord(19,1)!=d_OK)
	//     inMMTReadRecord(12,1);

#if 0
    for (inLoop = 0; inLoop < 150; inLoop++) {
        if (szTrxn[inLoop][0] != 0) {
            strcat((char *) szHeader, szTrxn[inLoop]);
            if (szTrxn[inLoop + 1][0] != 0)
                strcat((char *) szHeader, (char *) "|");

            vdDebug_LogPrintf("szHeader %s", szHeader);
        } else
            break;
    }
#endif
    memset(szHeader, 0x00, sizeof(szHeader));

    strcpy(szHeader, strTCT.szAppVersion);
    strcat(szHeader, " \n");
    strcat(szHeader, &strMMT[0].szMerchantName);
    strcat(szHeader, " \n");
    strcat(szHeader, &strMMT[0].szMID);
    strcat(szHeader, " \n");
    strcat(szHeader, &strMMT[0].szTID);
    strcat(szHeader, " \n");
    strcat(szHeader, reg_status);
    strcat(szHeader, " \n");
    strcat(szHeader, idleMode);

    if (strGBLVar.inGBLvAppType == 1)
    {
        strcat(szHeader, " \n");
        strcat(szHeader, "MCCPAY");
    }
	else if (strGBLVar.inGBLvAppType == 2)
    {
        strcat(szHeader, " \n");
        strcat(szHeader, "CASHOUT");
    }
	else if (strGBLVar.inGBLvAppType == 3)
    {
        strcat(szHeader, " \n");
        strcat(szHeader, "COMPLETEQR");
    }
	else if (strGBLVar.inGBLvAppType == 4)
    {
        strcat(szHeader, " \n");
        strcat(szHeader, "QRPH");
    }
	else
	{
        strcat(szHeader, " \n");
        strcat(szHeader, "OTHERS");
	}

	strcat(szHeader, " \n");
	strcat(szHeader, (strCPT.inCommunicationMode == GPRS_MODE ? "GPRS" : "WIFI"));
    //remove to fix hang
    //vdRemoveCard();

    return (*env)->NewStringUTF(env, szHeader);

}

JNIEXPORT jstring JNICALL
Java_com_Source_S1_1MCC_MCC_Trans_MenuTransactionType_ReadTrxMenu(JNIEnv *env, jobject instance) {

    // TODO
    //inTCTRead(1);
    //vdDebug_LogPrintf("inTCTRead:  strTCT.inMenuid = [%d]", strTCT.inMenuid);
    //strTCT.byRS232DebugPort = 8;
    //strTCT.inMenuid = 9;
    //inTCTSave(1);

    vdDebug_LogPrintf("TINE:  Get App Transactions from Dynamic Menu database");
    vdDebug_LogPrintf("TINE:  strTCT.inMenuid = [%d]", strTCT.inMenuid);
    char szTrxn[20][20];
    char szItemFunc[20][40];
    char szTrxnMenu[256+1];
    int inTrxnID[20];
    int inRet;
    int inLoop=0;

    memset(szTrxn,0x00, sizeof(szTrxn));
    memset(szItemFunc,0x00, sizeof(szItemFunc));

    inRet = inReadTrxMenu(szTrxn, szItemFunc, inTrxnID);
    if(inRet != d_OK)
        return "FAIL";

    memset(szTrxnMenu,0x00, sizeof(szTrxnMenu));

    for (inLoop = 0; inLoop < 150; inLoop++) {
        if (szTrxn[inLoop][0] != 0) {
            strcat((char *) szTrxnMenu, szTrxn[inLoop]);
            strcat((char *) szTrxnMenu, (char *) "|");
            strcat((char *) szTrxnMenu, szItemFunc[inLoop]);
            if (szTrxn[inLoop + 1][0] != 0)
                strcat((char *) szTrxnMenu, (char *) " \n");

            vdDebug_LogPrintf("szCustomerMenu %s", szTrxnMenu);
        } else
            break;
    }

    vdDebug_LogPrintf("szCustomerMenu2 %s %d", szTrxnMenu, strlen(szTrxnMenu));

    return (*env)->NewStringUTF(env, szTrxnMenu);
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Trans_Transactions_ExeFunction(JNIEnv *env, jobject instance,
                                                           jstring szFuncName_) {
    const char *szFuncName = (*env)->GetStringUTFChars(env, szFuncName_, 0);

    // TODO
    int inRetVal = 0;
    BYTE outbuf[d_MAX_IPC_BUFFER];
    USHORT out_len = 0;
    EMVCL_RC_DATA_EX stRCDataEx;

    char cwd[1024];

    //inRetVal = inTCTRead(1);
    vdDebug_LogPrintf("inTCTRead inRetVal[%d] strTCT.fDemo [%d]", inRetVal, strTCT.fDemo);
   //strTCT.byRS232DebugPort = 8;
    //strTCT.fDemo= 1;
    /*Here do test init, load TCT PRM*/
    //strTCT.fShareComEnable = 1;
    //strTCT.byERMMode  = 0;

    //inRetVal = inTCTSave(1);
    vdDebug_LogPrintf("inTCTSave inRetVal[%d]", inRetVal);

    if(activityClass != NULL)
         (*env)->DeleteGlobalRef(env, activityClass);
    if(activityObj != NULL)
         (*env)->DeleteGlobalRef(env, activityObj);

    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);

    inCTOSS_ExeFunction(szFuncName);

    (*env)->ReleaseStringUTFChars(env, szFuncName_, szFuncName);

     //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);

    return 0;
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Trans_Transactions_inCTOSS_1Sale(JNIEnv *env, jobject instance) {

    // TODO
    int inRetVal = 0;
    BYTE outbuf[d_MAX_IPC_BUFFER];
    USHORT out_len = 0;
    EMVCL_RC_DATA_EX stRCDataEx;

    char cwd[1024];

    //inRetVal = inTCTRead(1);
    //dDebug_LogPrintf("inTCTRead inRetVal[%d] strTCT.fDemo [%d]", inRetVal, strTCT.fDemo);
    //strTCT.byRS232DebugPort = 8;
    //strTCT.fDemo= 1;
    /*Here do test init, load TCT PRM*/
    //strTCT.fShareComEnable = 1;
    //strTCT.byERMMode  = 0;

    //inRetVal = inTCTSave(1);
    vdDebug_LogPrintf("inTCTSave inRetVal[%d]", inRetVal);

    if(activityClass != NULL)
         (*env)->DeleteGlobalRef(env, activityClass);
    if(activityObj != NULL)
         (*env)->DeleteGlobalRef(env, activityObj);

    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);

    vdDebug_LogPrintf("SATURN inCTOSS_Sale.............");
	
    inCTOS_WAVE_SALE();

    //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);

    return 0;

}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_AppStart(JNIEnv *env, jobject instance) {

    // TODO
    int inRetVal = 0;
    BYTE outbuf[d_MAX_IPC_BUFFER];
    USHORT out_len = 0;
    EMVCL_RC_DATA_EX stRCDataEx;

    char cwd[1024];

    inTCTRead(1);

    put_env_int("APP_STARTED", 1);
	
    vdDebug_LogPrintf("inCTOS_APP_START");

     if(activityClass != NULL)
              (*env)->DeleteGlobalRef(env, activityClass);
    if(activityObj != NULL)
          (*env)->DeleteGlobalRef(env, activityObj);

    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);

    vdDebug_LogPrintf("SATURN inCTOS_APP_START.............");
    //strTCT.fRegister = 1;
    if (strTCT.fRegister == 0) {
        vdDebug_LogPrintf("inRegister:  START");
        inRetVal = inRegister();
    }
	else if (strTCT.fRegister == 1 && strTCT.inMenuid == 11)
	{
		vdDebug_LogPrintf("inRegister:  START 2");
		inRetVal = inRegister();
    } else {
        inRetVal = inCTOS_APP_START();
    }

	put_env_int("APP_STARTED", 0);
	
    vdDebug_LogPrintf("SATURN inCTOS_APP_END............. inRetVal=[%d]", inRetVal);

     //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);

    return inRetVal;

}



JNIEXPORT jbyteArray JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOS_1GetBuffer(JNIEnv *env, jobject instance) {

	int size = 720*1280*4 + 54;
	BYTE* buffer = NULL;

	vdDebug_LogPrintf("SATURN CTOS_GETGBBuffer");
	while(1)
	{
		buffer = CTOS_GETGBBuffer();


		//vdDebug_LogPrintf("SATURN %s", buffer );
		if(buffer == NULL)
			continue;
		break;
	}
	jbyteArray array;
	array = (*env)->NewByteArray(env, size);
	(*env)->SetByteArrayRegion(env, array, 0, size, (jbyte*)buffer);
	buffer = NULL;
	return array;

}

int inCallJAVA_BatchReviewUI(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[250+1];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_BatchReviewUI=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

  if (strlen(pbDispMsg)>0)
	  strcpy(uszBuffer, pbDispMsg);

	vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "BatchReviewUI", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "BatchReviewUI");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_BatchReviewUI");
    return d_OK;
}

int inCallJAVA_UserConfirmOKMenu(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[250+1];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_UserConfirmOKMenu=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);
    //else
    //pbDispMsg = "Tine: UserConfirm activity";

    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "UserConfirmOKMenu", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "UserConfirmOKMenu");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_UserConfirmOKMenu");
    return d_OK;
}

int inCallJAVA_UserConfirmRebootMenu(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[250+1];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_UserConfirmRebootMenu=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);
    //else
    //pbDispMsg = "Tine: UserConfirm activity";

   //put_env_int("READCARDFAILEDCTR", 0);

    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "UserConfirmRebootMenu", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "UserConfirmRebootMenu");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_UserConfirmRebootMenu");
    return d_OK;
}



/***Start Offline Pin***/
jobject g_callback_obj_emv;
jmethodID g_callback_mid_pinpad;
jmethodID g_callback_mid_pindone;
jmethodID g_callback_mid_pindigi;




int ShowVirtualPIN(OUT USHORT *pusPINPadButtonNum, OUT BYTE *pbPINPadButtonInfo, OUT USHORT *pusPINPadButtonInfoLen)
{
	BYTE baInputbuffer[1024];
	BYTE baOutputbuffer[1024];

	JNIEnv* env = NULL;
	if((*g_JavaVM)->AttachCurrentThread(g_JavaVM, &env, NULL) != JNI_OK)
	{
		// attachCurrentThread() failed.
	}
	else {

	}

	int buffsize = 1024;
	int i = 0;

	//jbyte *by = (jbyte*)baInputbuffer;

	jbyteArray jarray = (*env)->NewByteArray(env, buffsize);

	(*env)->SetByteArrayRegion(env, jarray, 0, buffsize, (jbyte*)baInputbuffer);

	LOGE("buffsize= %d", buffsize);

	(*env)->CallLongMethod(env, g_callback_obj_emv, g_callback_mid_pinpad, jarray);

	//jsize len = env->GetArrayLength (jarray);
	//unsigned char* buf = new unsigned char[len];

	(*env)->GetByteArrayRegion (env, jarray, 0, buffsize, (jbyte*)(baOutputbuffer));

	*pusPINPadButtonNum = baOutputbuffer[0];

	LOGE("baOutputbuffer[0]= %02X", baOutputbuffer[0]);
	LOGE("baOutputbuffer[1]= %02X", baOutputbuffer[1]);
	LOGE("baOutputbuffer[2]= %02X", baOutputbuffer[2]);

	*pusPINPadButtonInfoLen = 0;
	*pusPINPadButtonInfoLen += (baOutputbuffer[1]*256);
	*pusPINPadButtonInfoLen += (baOutputbuffer[2]);

	LOGE("pusPINPadButtonNum= %d", *pusPINPadButtonNum);
	LOGE("pusPINPadButtonInfoLen= %d", *pusPINPadButtonInfoLen);


	memcpy(pbPINPadButtonInfo, &baOutputbuffer[3], *pusPINPadButtonInfoLen);

	return 0;
}



int GetPINDone(void)
{
    JNIEnv *tenv;
    jshort jsusX;
    jshort jsusY;

    //LOGD("---JNI AP CB--- GetPINDone\n");
    if((*g_JavaVM)->AttachCurrentThread(g_JavaVM, &tenv, NULL) != JNI_OK)
    {
        // attachCurrentThread() failed.
    }
    else {
        (*tenv)->CallLongMethod(tenv,g_callback_obj_emv, g_callback_mid_pindone);
    }
    return 0;

}


int ShowPINDigit(BYTE digitsNum, BYTE bPINType, BYTE bRemainingCounter)
{
    JNIEnv *tenv;
    jbyte digits;


    //LOGD("---JNI AP CB--- ShowPINDigit\n");
    if((*g_JavaVM)->AttachCurrentThread(g_JavaVM, &tenv, NULL) != JNI_OK)
    {
        // attachCurrentThread() failed.
    }
    else {
        digits = (jbyte) digitsNum;
        (*tenv)->CallLongMethod(tenv, g_callback_obj_emv, g_callback_mid_pindigi, digits, bPINType, bRemainingCounter);
    }
    return 0;

}


JNIEXPORT jlong JNICALL

Java_com_Source_S1_1MCC_MCC_Main_JNI_1offlinepin_REG_1CB_1EMV(JNIEnv *env,jobject obj,  jobject instance)
{
    int Rtn = 0;

    if(activityClass != NULL)
        (*env)->DeleteGlobalRef(env, g_callback_obj_emv);

    (*env)->GetJavaVM(env, &g_JavaVM);
    g_callback_obj_emv = (*env)->NewGlobalRef(env,instance);;
    jclass clz = (*env)->GetObjectClass(env, g_callback_obj_emv);

	LOGE("REG_1CB_1EMV");

    if(clz == NULL)
    {
        //failed to find class
    }

	LOGE("ShowVirtualPIN");
	LOGE("ShowPINDigit");
	LOGE("GetPINDone");

    g_callback_mid_pinpad = (*env)->GetMethodID(env,clz, "ShowVirtualPIN", "([B)J");
    g_callback_mid_pindigi = (*env)->GetMethodID(env, clz, "ShowPINDigit", "(BBB)J");
    g_callback_mid_pindone = (*env)->GetMethodID(env, clz, "GetPINDone", "()J");



    JavaVM *javaVM = g_vm;
    jint res = (*javaVM)->GetEnv(javaVM,(void **) &env, JNI_VERSION_1_6);
    if (res != JNI_OK) {
        res = (*javaVM)->AttachCurrentThread(javaVM, &env, NULL);
        if (JNI_OK != res) {
            LOGE("Failed to AttachCurrentThread, ErrorCode = %d", res);

        }
    }

    if(clz != NULL)
        (*env)->DeleteLocalRef(env, clz);

    return Rtn;
}

/***End Offline Pin***/



/* Analyze a string for str1,str2 */
int SplitString(char *str, char *str1, char *str2, char c)
{
    int i,j;

    for (j=0,i=0;str[i]!='\0'&&str[i]!=c;j++,i++)
        str1[j]=str[i];
    str1[j]='\0';

    if (str[i]=='\0')
		str2[0]='\0';
    else
    {
        for (j=0,i++;str[i]!='\0';j++,i++)
            str2[j]=str[i];
        str2[j]='\0';
    }

    return strlen(str2);
}


int inCallJAVA_inGetIPPPinEx(USHORT KeySet,  USHORT KeyIndex,  BYTE* pInData, BYTE* szPINBlock, BYTE* szKSN, BYTE* szIndex, USHORT pinBypassAllow)
{
    unsigned char uszpInData[20+1];
    unsigned char PinBlockKsn[20+1];
    unsigned char szPinBlockTemp[100+1];
    int inRet = 0;

	vdDebug_LogPrintf("inCallJAVA_inGetIPPPinEx Start KeySet[%d] KeyIndex[%d] pinBypassAllow[%d]", KeySet, KeyIndex, pinBypassAllow);

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);


	memset(uszpInData, 0x00, sizeof(uszpInData));
	vdDebug_LogPrintf("strlen(pInData)[%d]", strlen(pInData));


    if (strlen(pInData)>0)
        strcpy(uszpInData, pInData);

    vdDebug_LogPrintf("uszpInData[%s]", uszpInData);

    jstring Inputstr = (*env)->NewStringUTF(env, uszpInData);
    vdDebug_LogPrintf("jstring[%s]", uszpInData);

    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_HLB/HLB/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "GetIPPPinEx", "(IILjava/lang/String;I)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "GetIPPPinEx");

    jobject Result = (*env)->CallObjectMethod(env, activityObj, methodID, KeySet, KeyIndex, Inputstr, pinBypassAllow);

    jbyte* Outstr = NULL;


	if(NULL != Result)
	{
	   Outstr = (*env)->GetStringUTFChars(env,(jstring) Result, NULL);
	}
	else
	{
		vdDebug_LogPrintf("Result is NULL");
	}


    if (Outstr!=NULL)
    {

	    memset(uszpInData, 0x00, sizeof(uszpInData));

		vdDebug_LogPrintf("%s", Outstr);
		SplitString(Outstr, szIndex, PinBlockKsn, '*');
		vdDebug_LogPrintf("szIndex[%s]", szIndex);
		vdDebug_LogPrintf("PinBlockKsn[%s]", PinBlockKsn);
		vdDebug_LogPrintf("AAA before splitstring srTransRec.CDTid[%d], srTransRec.CDTid2[%d]", srTransRec.CDTid, srTransRec.CDTid2);
		memset(szPinBlockTemp, 0x00, sizeof(szPinBlockTemp));
                   SplitString(PinBlockKsn, szPinBlockTemp, szKSN, '|');
                   memcpy(szPINBlock, szPinBlockTemp, 16);
                   //SplitString(PinBlockKsn, szPINBlock, szKSN, '|');
		vdDebug_LogPrintf("AAA after splitstring srTransRec.CDTid[%d], srTransRec.CDTid2[%d]", srTransRec.CDTid, srTransRec.CDTid2);
		srTransRec.CDTid = srTransRec.CDTid2; //AAA fix on CDTid wrong value.
		vdDebug_LogPrintf("szPINBlock[%s] szPinBlockTemp[%s]", szPINBlock, szPinBlockTemp);  //corrupted szPINBlock dunno why opted to use szPinBlockTemp
		PackEx(szPinBlockTemp,16,srTransRec.szPINBlock);
		vdDebug_LogPrintf("szKSN[%s]", szKSN);
		DebugAddHEX("AAA",srTransRec.szPINBlock,8);
        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, Result, Outstr);
        if(strlen(szPINBlock) == 0)
        {
            vdDebug_LogPrintf("PINBYPASS");
	   vdDisplayMessageStatusBox(1, 8, "PROCESSING...", MSG_PLS_WAIT, MSG_TYPE_PROCESS);
	  
           inRet = d_NO;
        }
        else
            inRet = d_OK;

    }
    else {
        *szPINBlock = 0;
        *szKSN = 0;
        inRet = d_NO;
        vdDebug_LogPrintf("Outstr is NULL");
    }

    (*env)->DeleteLocalRef(env, Inputstr);
    (*env)->DeleteLocalRef(env, Result);

    vdDebug_LogPrintf("end inCallJAVA_inGetIPPPinEx [%d]", inRet);
    return inRet;
}



int inCallJAVA_inGetPIN_With_3DESDUKPTEx(USHORT KeySet,  USHORT KeyIndex,  BYTE* pInData, BYTE* szPINBlock, BYTE* szKSN, BYTE* szIndex, USHORT pinBypassAllow)
{
	unsigned char uszpInData[20+1];
	unsigned char PinBlockKsn[20+1];
	int inRet = 0;

	vdDebug_LogPrintf("inCallJAVA_inGetPIN_With_3DESDUKPTEx Start KeySet[%d] KeyIndex[%d]", KeySet, KeyIndex);

	JNIEnv *env;
	jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
	// Use the env pointer...
	vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);


	memset(uszpInData, 0x00, sizeof(uszpInData));
	vdDebug_LogPrintf("strlen(pInData)[%d]", strlen(pInData));


	if (strlen(pInData)>0)
		strcpy(uszpInData, pInData);

	vdDebug_LogPrintf("uszpInData[%s]", uszpInData);

	jstring Inputstr = (*env)->NewStringUTF(env, uszpInData);
	vdDebug_LogPrintf("jstring[%s]", uszpInData);

	vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_HLB/HLB/Main/MainActivity");

	jmethodID methodID = (*env)->GetMethodID(env, activityClass, "GetPIN_With_3DESDUKPTEx", "(IILjava/lang/String;I)Ljava/lang/String;");

	vdDebug_LogPrintf("jstring[%s]", "GetIPPPinEx");

	jobject Result = (*env)->CallObjectMethod(env, activityObj, methodID, KeySet, KeyIndex, Inputstr, pinBypassAllow);

	jbyte* Outstr = NULL;


	if(NULL != Result)
	{
	   Outstr = (*env)->GetStringUTFChars(env,(jstring) Result, NULL);
	}
	else
	{
		vdDebug_LogPrintf("Result is NULL");
	}


	if (Outstr!=NULL)
	{

		memset(uszpInData, 0x00, sizeof(uszpInData));

		vdDebug_LogPrintf("%s", Outstr);
		SplitString(Outstr, szIndex, PinBlockKsn, '*');
		vdDebug_LogPrintf("szIndex[%s]", szIndex);
		vdDebug_LogPrintf("PinBlockKsn[%s]", PinBlockKsn);
		SplitString(PinBlockKsn, szPINBlock, szKSN, '|');
		vdDebug_LogPrintf("szPINBlock[%s]", szPINBlock);
		vdDebug_LogPrintf("szKSN[%s]", szKSN);
		vdDebug_LogPrintf("ReleaseStringUTFChars");
		(*env)->ReleaseStringUTFChars(env, Result, Outstr);
        if(strlen(szPINBlock) == 0)
            inRet = d_NO;
        else
            inRet = d_OK;
	}
	else {
		*szPINBlock = 0;
		*szKSN = 0;
        vdDebug_LogPrintf("PINBYPASS");
	vdDisplayMessageStatusBox(1, 8, "PROCESSING...", MSG_PLS_WAIT, MSG_TYPE_PROCESS);
        inRet = d_NO;
	}

	(*env)->DeleteLocalRef(env, Inputstr);
	(*env)->DeleteLocalRef(env, Result);

	vdDebug_LogPrintf("end inCallJAVA_inGetPIN_With_3DESDUKPTEx[%d]", inRet);
	return inRet;
}


#define d_EXTERNAL_PINPAD           0

BYTE g_bPINType;
BYTE g_bRemainingCounter;
BOOL ispinpadalone=true;





USHORT OnShowVirtualPIN(OUT USHORT *pusPINPadButtonNum, OUT BYTE *pbPINPadButtonInfo, OUT USHORT *pusPINPadButtonInfoLen)
{
	vdDebug_LogPrintf((BYTE*)"OnShowVirtualPIN() is triggered -->");
	USHORT usPINPadButtonNum;
	BYTE bPINPadButtonInfo[1024];
	USHORT usPINPadButtonInfoLen;

	ShowVirtualPIN(&usPINPadButtonNum, bPINPadButtonInfo, &usPINPadButtonInfoLen);

	*pusPINPadButtonNum= usPINPadButtonNum;
	*pusPINPadButtonInfoLen = usPINPadButtonInfoLen;
	memcpy(pbPINPadButtonInfo, bPINPadButtonInfo, usPINPadButtonInfoLen);

    DebugAddINT((BYTE*)"  pusPINPadButtonNum",  *pusPINPadButtonNum);
    DebugAddINT((BYTE*)"  pusPINPadButtonInfoLen", *pusPINPadButtonInfoLen);
    DebugAddHEX((BYTE*)"  pbPINPadButtonInfo", pbPINPadButtonInfo, *pusPINPadButtonInfoLen);
	return 0;
}

USHORT OnGetPINDone(void)
{
	vdDebug_LogPrintf((BYTE*)"OnGetPINDone() is triggered -->");
	if(ispinpadalone == true)
	{
		GetPINDone();
	}
	//CTOS_LCDTPrintXY(1, 3, (BYTE*)"Get PIN Done");

	return 0;
}

void OnShowPinDigit(IN BYTE bDigits,int reEnterPin)
{
	BYTE baStr[21];
	vdDebug_LogPrintf((BYTE*)"OnShowPinDigit() is triggered -->");
    DebugAddINT((BYTE*)"  bDigits", bDigits);

    DebugAddINT((BYTE*)"  ispinpadalone", ispinpadalone);
	if(ispinpadalone == true)
	{
		ShowPINDigit(bDigits, 0x1, reEnterPin/*0xF*/);
	}
	else
	{
		memset(baStr, 0x20, 20);
		baStr[20] = 0;

		memset(baStr, '*', bDigits);
		CTOS_LCDTPrintXY(1, 2, baStr);
	}
}


JNIEXPORT jint
JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOSS_1OnlinePinTest(JNIEnv *env, jobject instance) {
    // TODO

    int inRetVal = 0;

    strTCT.fShareComEnable = 1;
    strTCT.byERMMode  = 0;
	//strTCT.byRS232DebugPort = 8;

    inRetVal = inTCTSave(1);
    vdDebug_LogPrintf("inTCTSave inRetVal[%d]", inRetVal);

  if(activityClass != NULL)
           (*env)->DeleteGlobalRef(env, activityClass);
  if(activityObj != NULL)
           (*env)->DeleteGlobalRef(env, activityObj);

    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);

    vdDebug_LogPrintf("inCTOSS_OnlinePinTest.............[%d]", strTCT.byCtlsMode);


	int KeySet = 0xC000;
    int KeyIndex = 0x04;
    BYTE pInData[19+1] = {0};
    BYTE szPINBlock[60] = {0};
    BYTE szKSN[50] = {0};

    //inCTOS_InjectKeyTest();
    vdCTOSS_EFT_TestHardcodeKey();
    vdDebug_LogPrintf("Before");
    vdCTOSS_GetKSN();

    strcpy(pInData, "22222222222222222");

	vdDebug_LogPrintf("inCTOSS_Sale.............KeySet[%d] KeyIndex[%d]", KeySet, KeyIndex);
	vdDebug_LogPrintf("inCTOSS_Sale.............[%d]", strTCT.byCtlsMode);
	vdDebug_LogPrintf("pInData[%s]", pInData);
    inCTOS_KMS2PINGetExDukpt(KeySet, KeyIndex, pInData, szPINBlock, szKSN, 1);

    vdDebug_LogPrintf("After");
    vdCTOSS_GetKSN();

    //inCTOS_KMS2PINGetEx3Des(KeySet, KeyIndex, pInData, szPINBlock, szKSN, 1);
	vdDebug_LogPrintf("inCTOSS_Sale  szPINBlock[%s]  szKSN[%s] ", szPINBlock, szKSN);

     //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);

    return 0;
}


JNIEXPORT jstring JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_CTOSS_1MSRRead_1inJava(JNIEnv *env,
                                                                     jobject instance) {

    // TODO
    USHORT usTk1Len, usTk2Len, usTk3Len;
    BYTE szTk1Buf[TRACK_I_BYTES], szTk2Buf[TRACK_II_BYTES], szTk3Buf[TRACK_III_BYTES];
    usTk1Len = TRACK_I_BYTES ;
    usTk2Len = TRACK_II_BYTES ;
    usTk3Len = TRACK_III_BYTES ;

    int byMSR_status= d_NO;
    int shReturn= d_NO;

    int bySC_status = d_NO;
    //DWORD dwWait=0, dwWakeup=0;
	int inRet;

    //vdSetFirstIdleKey(0x00);

/*
    dwWait = d_EVENT_KBD | d_EVENT_MSR | d_EVENT_SC;
    CTOS_SystemWait(20, dwWait, &dwWakeup);
    if ((dwWakeup & d_EVENT_MSR) == d_EVENT_MSR)
    {
        vdDebug_LogPrintf("MSR Reading...");
    }
*/
    //if(activityClass != NULL)
        //(*env)->DeleteGlobalRef(env, activityClass);
    //if(activityObj != NULL)
        //(*env)->DeleteGlobalRef(env, activityObj);

   // jint rs = (*env)->GetJavaVM(env, &jvm);

    //jclass cls = (*env)->GetObjectClass(env, instance);
   // activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    //activityObj = (*env)->NewGlobalRef(env, instance);

   inRet = inCTOS_CheckAutoSettlement();
   vdDebug_LogPrintf("saturn check auto settle result2 %d", inRet);
   if (inRet == TRUE){
   		return (*env)->NewStringUTF(env, "SETTLE");
   }

    vdDebug_LogPrintf("MSRRead");
    byMSR_status = CTOS_MSRRead(szTk1Buf, &usTk1Len, szTk2Buf, &usTk2Len, szTk3Buf, &usTk3Len);
    CTOS_Delay(100);
    if (byMSR_status == 0) {
        CTOS_Beep();
        //shReturn = shCTOS_SetMagstripCardTrackData(szTk1Buf, usTk1Len, szTk2Buf, usTk2Len, szTk3Buf, usTk3Len);  //BDO Code
        //vdCTOS_SetTransEntryMode(CARD_ENTRY_MSR);
        vdCTOS_SetMagstripCardTrackData(szTk1Buf, usTk1Len, szTk2Buf, usTk2Len, szTk3Buf, usTk3Len);
        fEntryCardfromIDLE = TRUE;

        vdDebug_LogPrintf("on MSR NewStringUTF");
        return (*env)->NewStringUTF(env, "MSR_OK");
    }
    //else {
    //    return (*env)->NewStringUTF(env, "MSR_NO");
    //}

    vdDebug_LogPrintf("ICCRead");

    CTOS_SCStatus(d_SC_USER, &bySC_status);
    CTOS_Delay(100);
    if(bySC_status & d_MK_SC_PRESENT) {
        vdDebug_LogPrintf("chip entry");
        //inCTOSS_CLMCancelTransaction();
        vdCTOS_SetTransEntryMode(CARD_ENTRY_ICC);
        fEntryCardfromIDLE = TRUE;
        //shReturn = inCTOS_EMVCardReadProcess();
        vdDebug_LogPrintf("on ICC NewStringUTF");
        CTOS_Beep();
        return (*env)->NewStringUTF(env, "ICC_OK");
    }
    //else {
    //    return (*env)->NewStringUTF(env, "ICC_NO");
    //}

    return (*env)->NewStringUTF(env, "MSR_ICC_NO");


}

JNIEXPORT jstring JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_CTOSS_1SCStatus_1inJava(JNIEnv *env,
                                                                      jobject instance) {

    // TODO
    int bySC_status = d_NO;
    int shReturn= d_NO;

    vdDebug_LogPrintf("ICCRead");

    CTOS_SCStatus(d_SC_USER, &bySC_status);
    CTOS_Delay(1000);
    if(bySC_status & d_MK_SC_PRESENT) {
        vdDebug_LogPrintf("chip entry");
        //inCTOSS_CLMCancelTransaction();
        vdCTOS_SetTransEntryMode(CARD_ENTRY_ICC);
        //shReturn = inCTOS_EMVCardReadProcess();
        vdDebug_LogPrintf("on ICC NewStringUTF");
        CTOS_Beep();
        return (*env)->NewStringUTF(env, "ICC_OK");
    } else {
        return (*env)->NewStringUTF(env, "ICC_NO");
    }

}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOSS_1Idle_1Sale(JNIEnv *env, jobject instance) {

    // TODO
    int inRetVal = 0;
    BYTE outbuf[d_MAX_IPC_BUFFER];
    USHORT out_len = 0;
    EMVCL_RC_DATA_EX stRCDataEx;

    char cwd[1024];

    inRetVal = inTCTRead(1);
    vdDebug_LogPrintf("inTCTRead inRetVal[%d] strTCT.fDemo [%d]", inRetVal, strTCT.fDemo);
    //strTCT.byRS232DebugPort = 8;
    //strTCT.fDemo= 1;
    /*Here do test init, load TCT PRM*/
    strTCT.fShareComEnable = 1;
    strTCT.byERMMode  = 0;

    inRetVal = inTCTSave(1);
    vdDebug_LogPrintf("inTCTSave inRetVal[%d]", inRetVal);

    vdDebug_LogPrintf("srTransRec.byEntryMode = [%d]", srTransRec.byEntryMode);
    vdDebug_LogPrintf("srTransRec.szPAN = [%d]", strlen(srTransRec.szPAN));

 if(activityClass != NULL)
              (*env)->DeleteGlobalRef(env, activityClass);
 if(activityObj != NULL)
              (*env)->DeleteGlobalRef(env, activityObj);

    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);

    vdDebug_LogPrintf("SATURN inCTOSS_Sale.............");
    //inCTOS_WAVE_SALE();
    //tine: android - set ui thread to display nothing
    DisplayStatusLine(" ");
    vdGetGlobalVariable();

    put_env_int("APP_STARTED", 1);
    if(strTCT.byIdleSaleType == IDLE_CREDIT_SALE || fEntryCardfromIDLE == FALSE)
    {
        inSelectedIdleAppsHost = MCC_HOST;
        inCTOS_WAVE_SALE();
        //inRetVal = inCTOS_APP_START();

    }
    else if(strTCT.byIdleSaleType == IDLE_BANCNET)
    {
        //inCTOS_BancnetSale();
    }
    else if (strTCT.byIdleSaleType == IDLE_CREDIT_AND_BANCNET)
    {
        //inSelectIdleApps();
        //inCTOS_APP_START();
        //usCTOSS_LCDDisplay("SALE|PROCESSING...");
        inSelectIdleTrxn();
    }

	put_env_int("APP_STARTED", 0);
	
    vdCTOS_ResetMagstripCardData();
    vdCTOS_TransEndReset();
    vdRemoveCard();

     //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);

    return 0;
}

int inCallJAVA_IdleMenuTransType(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[512+1];
    int inRet = 0;


    vdDebug_LogPrintf("saturn =====inCallJAVA_IdleMenuTransType=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    vdDebug_LogPrintf("pbDispMsg[%s]", pbDispMsg);

    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);
    else
        strcpy(uszBuffer, "PASS IN MSG");

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

//	jmethodID methodID = (*env)->GetMethodID(env, clazz, "rndGetNumStr", "(Ljava/lang/String;)Ljava/lang/String;");
    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "IdleMenuTransType", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "IdleMenuTransType");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
    //rs = (*jvm)->DetachCurrentThread(jvm);

    jbyte* str = NULL; //(*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    if (str!=NULL)
    {
        vdDebug_LogPrintf("AAA<<>>>%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_IdleMenuTransType");
    return d_OK;
}

int inCallJAVA_IdleMenuTransType2(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[512+1];
    int inRet = 0;


    vdDebug_LogPrintf("saturn =====inCallJAVA_IdleMenuTransType2=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    vdDebug_LogPrintf("pbDispMsg[%s]", pbDispMsg);

    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);
    else
        strcpy(uszBuffer, "PASS IN MSG");

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

//	jmethodID methodID = (*env)->GetMethodID(env, clazz, "rndGetNumStr", "(Ljava/lang/String;)Ljava/lang/String;");
    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "IdleMenuTransType2", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "IdleMenuTransType");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
    //rs = (*jvm)->DetachCurrentThread(jvm);

    jbyte* str = NULL; //(*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    if (str!=NULL)
    {
        vdDebug_LogPrintf("AAA<<>>>%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_IdleMenuTransType");
    return d_OK;
}


int inCallJAVA_GetExpiryDate(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[1024+1];
    int inRet = 0;


    vdDebug_LogPrintf("saturn =====inCallJAVA_GetExpiryDate=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("SATURN jint[%d] *env[%x]", rs, *env);

    vdDebug_LogPrintf("saturn pbDispMsg[%s]", pbDispMsg);

    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);
    else
        strcpy(uszBuffer, "PASS IN MSG");


    vdDebug_LogPrintf("saturn uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("saturn jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("saturn jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

//	jmethodID methodID = (*env)->GetMethodID(env, clazz, "rndGetNumStr", "(Ljava/lang/String;)Ljava/lang/String;");
    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "GetExpiryDate", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("saturn test jstring[%s]", "GetExpiryDate");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
    //rs = (*jvm)->DetachCurrentThread(jvm);


    vdDebug_LogPrintf("saturn inCallJAVA_GetExpiryDate test");

    jbyte* str = NULL; //(*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_GetExpiryDate");
    return d_OK;
}


int inCallJAVA_GetCVV(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[1024+1];
    int inRet = 0;


    vdDebug_LogPrintf("saturn =====inCallJAVA_GetCVV=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("SATURN jint[%d] *env[%x]", rs, *env);

    vdDebug_LogPrintf("saturn pbDispMsg[%s]", pbDispMsg);

    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);
    else
        strcpy(uszBuffer, "PASS IN MSG");


    vdDebug_LogPrintf("saturn uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("saturn jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("saturn jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

//	jmethodID methodID = (*env)->GetMethodID(env, clazz, "rndGetNumStr", "(Ljava/lang/String;)Ljava/lang/String;");
    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "GetCVV", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("saturn test jstring[%s]", "GetCVV");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
    //rs = (*jvm)->DetachCurrentThread(jvm);


    vdDebug_LogPrintf("saturn inCallJAVA_GetCVV test");

    jbyte* str = NULL; //(*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    if (str!=NULL)
    {
        vdDebug_LogPrintf("AAA - CVV%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_GetCVV");
    return d_OK;
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOSS_1MANUALENTRY(JNIEnv *env, jobject instance) {

    // TODO

    vdDebug_LogPrintf("saturn inCTOSS_MANUALENTRY............");
//    if(activityClass != NULL)
//        (*env)->DeleteGlobalRef(env, activityClass);
//    if(activityObj != NULL)
//        (*env)->DeleteGlobalRef(env, activityObj);

//    jint rs = (*env)->GetJavaVM(env, &jvm);

//    jclass cls = (*env)->GetObjectClass(env, instance);
//    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
//    activityObj = (*env)->NewGlobalRef(env, instance);

    CTOS_KBDBufPut('M');
    ing_KeyPressed = 'M';
    vdDebug_LogPrintf("ing_KeyPressed = [%d]", ing_KeyPressed);

//    if(cls != NULL)
//        (*env)->DeleteLocalRef(env, cls);

    return 0;
}

int inCallJAVA_UserConfirmCard(BYTE *pbCardDisplay, BYTE *pbOutBuf, BYTE *pbLen)
{
    unsigned char uszBuffer[512+1];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_UserConfirmCard=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    if (strlen(pbCardDisplay)>0)
        strcpy(uszBuffer, pbCardDisplay);

    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "UserConfirmCard", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "UserConfirmCard");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
    vdDebug_LogPrintf("jstring[%s]", "CallObjectMethod");
    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutBuf, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_UserConfirmCard");
    return d_OK;
}

int inCallJAVA_inDCCFxRateUI(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[1028+1];
    int inRet = 0;

    vdDebug_LogPrintf("=====inCallJAVA_inDCCFxRateUI=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);

    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "inDCCFxRateUI", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "inDCCFxRateUI");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_inDCCFxRateUI");
    return d_OK;
}

int inCallJAVA_inDCCFxRateUI2(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[1028+1];
    int inRet = 0;

    vdDebug_LogPrintf("=====inCallJAVA_inDCCFxRateUI2=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);

    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "inDCCFxRateUI2", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "inDCCFxRateUI2");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_inDCCFxRateUI2");
    return d_OK;
}


int is_utf8(const char * string)
{

    vdDebug_LogPrintf("AAA -is_utf8 start ");
    if(!string)
        return 0;
    vdDebug_LogPrintf("AAA -is_utf8 1 ");
    const unsigned char * bytes = (const unsigned char *)string;
    while(*bytes)
    {
        if( (// ASCII
             // use bytes[0] <= 0x7F to allow ASCII control characters
                bytes[0] == 0x09 ||
                bytes[0] == 0x0A ||
                bytes[0] == 0x0D ||
                (0x20 <= bytes[0] && bytes[0] <= 0x7E)
            )
        ) {
            bytes += 1;
            continue;
        }

        if( (// non-overlong 2-byte
                (0xC2 <= bytes[0] && bytes[0] <= 0xDF) &&
                (0x80 <= bytes[1] && bytes[1] <= 0xBF)
            )
        ) {
            bytes += 2;
            continue;
        }

        if( (// excluding overlongs
                bytes[0] == 0xE0 &&
                (0xA0 <= bytes[1] && bytes[1] <= 0xBF) &&
                (0x80 <= bytes[2] && bytes[2] <= 0xBF)
            ) ||
            (// straight 3-byte
                ((0xE1 <= bytes[0] && bytes[0] <= 0xEC) ||
                    bytes[0] == 0xEE ||
                    bytes[0] == 0xEF) &&
                (0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
                (0x80 <= bytes[2] && bytes[2] <= 0xBF)
            ) ||
            (// excluding surrogates
                bytes[0] == 0xED &&
                (0x80 <= bytes[1] && bytes[1] <= 0x9F) &&
                (0x80 <= bytes[2] && bytes[2] <= 0xBF)
            )
        ) {
            bytes += 3;
            continue;
        }

        if( (// planes 1-3
                bytes[0] == 0xF0 &&
                (0x90 <= bytes[1] && bytes[1] <= 0xBF) &&
                (0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
                (0x80 <= bytes[3] && bytes[3] <= 0xBF)
            ) ||
            (// planes 4-15
                (0xF1 <= bytes[0] && bytes[0] <= 0xF3) &&
                (0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
                (0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
                (0x80 <= bytes[3] && bytes[3] <= 0xBF)
            ) ||
            (// plane 16
                bytes[0] == 0xF4 &&
                (0x80 <= bytes[1] && bytes[1] <= 0x8F) &&
                (0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
                (0x80 <= bytes[3] && bytes[3] <= 0xBF)
            )
        ) {
            bytes += 4;
            continue;
        }
         vdDebug_LogPrintf("AAA -is_utf8 start return 0");
        return 0;
    }
     vdDebug_LogPrintf("AAA -is_utf8 return 1 ");
    return 1;
}


int inCallJAVA_S1InputStringAlpha(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[512+1];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_S1InputStringAlpha=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    vdDebug_LogPrintf("pbDispMsg[%s]", pbDispMsg);

    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);
    else
        strcpy(uszBuffer, "PASS IN MSG");


    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

//	jmethodID methodID = (*env)->GetMethodID(env, clazz, "rndGetNumStr", "(Ljava/lang/String;)Ljava/lang/String;");
    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "InputStringAlpha", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "InputStringAlpha");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
    //rs = (*jvm)->DetachCurrentThread(jvm);

    jbyte* str = NULL; //(*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_S1InputStringAlpha");
    return d_OK;
}

int inCallJAVA_getAppPackageInfo(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
	unsigned char uszBuffer[512+1];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_getAppPackageInfo=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    vdDebug_LogPrintf("pbDispMsg[%s]", pbDispMsg);

    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);
    else
        strcpy(uszBuffer, "PASS IN MSG");


    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

//	jmethodID methodID = (*env)->GetMethodID(env, clazz, "rndGetNumStr", "(Ljava/lang/String;)Ljava/lang/String;");
    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "AppPackageInfo", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "AppPackageInfo");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
    //rs = (*jvm)->DetachCurrentThread(jvm);

    jbyte* str = NULL; //(*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_getAppPackageInfo");
    return d_OK;
}

int inCallJAVA_DisplayBox(BYTE *pbDispMsg, BYTE *pbOutStr_dispbox, BYTE *pbOutLen)
{
    unsigned char uszBuffer[528];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_DisplayBox=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);


    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);

    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "DisplayBox", "(Ljava/lang/String;)Ljava/lang/String;");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr_dispbox, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    //(*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_DisplayBox");
    return d_OK;
}

int inCallJAVA_DisplayErrorMsg2(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[528];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_DisplayErrorMsg2=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);


    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);

    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "DisplayErrorMsg2", "(Ljava/lang/String;)Ljava/lang/String;");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    //(*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_DisplayErrorMsg2");
    return d_OK;
}


int inCallJAVA_usGetConnectionStatus(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)

{
    unsigned char uszBuffer[128+1];
    int inRet = 0;


    vdDebug_LogPrintf("saturn =====inCallJAVA_usGetSerialNumber=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);


    memset(uszBuffer, 0x00, sizeof(uszBuffer));
    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);


    //vdDebug_LogPrintf("saturn uszBuffer[%s]", uszBuffer);
	//strcpy(uszBuffer, "WIFI");

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("saturn jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("saturn jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "fGetConnectStatus", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("saturn jstring[%s]", "fGetConnectStatus");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("saturn end fGetConnectStatus");
    return d_OK;
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_AppRegister(JNIEnv *env, jobject instance) {

    // TODO
    int inRetVal = 0;
    BYTE outbuf[d_MAX_IPC_BUFFER];
    USHORT out_len = 0;
    EMVCL_RC_DATA_EX stRCDataEx;

    char cwd[1024];

    //inRetVal = inTCTRead(1);
    //vdDebug_LogPrintf("inTCTRead inRetVal[%d] strTCT.fDemo [%d]", inRetVal, strTCT.fDemo);
    //strTCT.byRS232DebugPort = 8;
    //strTCT.fDemo= 1;
    /*Here do test init, load TCT PRM*/
    //strTCT.fShareComEnable = 1;
    //strTCT.byERMMode  = 0;

    //inRetVal = inTCTSave(1);
    //vdDebug_LogPrintf("inTCTSave inRetVal[%d]", inRetVal);

     if(activityClass != NULL)
              (*env)->DeleteGlobalRef(env, activityClass);
     if(activityObj != NULL)
              (*env)->DeleteGlobalRef(env, activityObj);

    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);

    vdDebug_LogPrintf("SATURN inCTOS_Reg.............");
    inRetVal = inRegister();

     //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);

    return inRetVal;

}

int inCallJAVA_UserConfirmMenu2(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[d_MAX_IPC_BUFFER];
    int inRet = 0;


    vdDebug_LogPrintf("saturn =====inCallJAVA_UserConfirmMenu=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);



    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);


	vdDebug_LogPrintf("saturn uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "UserConfirmMenu2", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "UserConfirmMenu2");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("saturn ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("saturn end inCallJAVA_UserConfirmMenu2");
    return d_OK;
}


int inCallJAVA_GetWifiInfo(BYTE *pbOutStr)
{

    unsigned char uszBuffer[528];
    int inRet = 0;


    vdDebug_LogPrintf("saturn =====inCallJAVA_GetWifiInfo=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);


    strcpy(uszBuffer, "test");

    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "GetWIFISettings", "(Ljava/lang/String;)Ljava/lang/String;");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("saturn wifi settings %s", str);
        //*pbOutLen = strlen(str);
        //vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);
        (*env)->ReleaseStringUTFChars(env, result, str);



    }
    //else
        //*pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    //(*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("saturn end inCallJAVA_GetWifiInfo");
    return d_OK;
	}



int inCallJAVA_GetConnectionType(BYTE *pbOutStr)
{

    unsigned char uszBuffer[528];
    int inRet = 0;


    vdDebug_LogPrintf("saturn =====inCallJAVA_GetConnectionType=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);


    strcpy(uszBuffer, "test");

    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "GetConnectionType", "(Ljava/lang/String;)Ljava/lang/String;");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("saturn wifi settings %s", str);
        //*pbOutLen = strlen(str);
        //vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);
        (*env)->ReleaseStringUTFChars(env, result, str);



    }
    //else
        //*pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    //(*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("saturn end inCallJAVA_GetConnectionType");
    return d_OK;
	}


JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_intAutoSettlement(JNIEnv *env, jobject thiz) {
    // TODO: implement intAutoSettlement()

    vdDebug_LogPrintf("saturn do autosettle");
    if(activityClass != NULL)
        (*env)->DeleteGlobalRef(env, activityClass);
    if(activityObj != NULL)
        (*env)->DeleteGlobalRef(env, activityObj);

    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, thiz);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, thiz);

	if(get_env_int("ECRPROCESS")==0)
	     inCTOS_AutoSettlement();
	
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);
	return d_OK;
}

JNIEXPORT jint
JNICALL
Java_com_Source_S1_1MCC_MCC_db_AndroidDatabaseManager_vdSetDbJournalMode(JNIEnv *env, jobject thiz) {
    vdDebug_LogPrintf("saturn vdSetDbJournalMode");
    vdSetJournalModeOff();
    return d_OK;
}

int inCallJAVA_AnimatePrint(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[528];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_AnimatePrint=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);


    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);

    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "AnimateReceiptPrint", "(Ljava/lang/String;)Ljava/lang/String;");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    //(*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_AnimatePrint");
    return d_OK;
}

int inCallJAVA_GetBatteryLevel(BYTE *pbOutStr)
{

    unsigned char uszBuffer[528];
    int inRet = 0;


    vdDebug_LogPrintf("saturn =====inCallJAVA_GetBatteryLevel=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);


    strcpy(uszBuffer, "test");

    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "GetBatteryLevel", "(Ljava/lang/String;)Ljava/lang/String;");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("saturn wifi settings %s", str);
        //*pbOutLen = strlen(str);
        //vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);
        (*env)->ReleaseStringUTFChars(env, result, str);



    }
    //else
        //*pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    //(*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("saturn end inCallJAVA_GetBatteryLevel");
    return d_OK;
}

int inCallJAVA_DisplayStatusBox(BYTE *pbDispMsg, BYTE *pbOutStr_dsbox, BYTE *pbOutLen)
{
    unsigned char uszBuffer[528];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_DisplayStatusBox=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);


    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);

    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "DisplayStatusBox", "(Ljava/lang/String;)Ljava/lang/String;");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr_dsbox, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    //(*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_DisplayStatusBox");
    return d_OK;
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOSS_1ProcessCfgExpress(JNIEnv *env,
                                                                         jobject thiz) {
    // TODO: implement inCTOSS_ProcessCfgExpress()
    BYTE bInBuf[40];
    BYTE bOutBuf[40];
    USHORT usInLen = 0;
    USHORT usOutLen = 0;

    char cwd[1024];
    int inRet = 0;

    //return d_OK;
    //inTCTRead(1);
    //strTCT.byRS232DebugPort = 8;
    //inTCTSave(1);

    vdDebug_LogPrintf("saturn TerminalStartUp");

    if(activityClass != NULL)
        (*env)->DeleteGlobalRef(env, activityClass);
    if(activityObj != NULL)
        (*env)->DeleteGlobalRef(env, activityObj);

    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, thiz);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, thiz);

    vdDebug_LogPrintf("inCTOSS_ProcessCfgExpress");
    inCTOSS_ProcessCfgExpress();

    inDatabase_TerminalOpenDatabase();
    inTCTReadEx(1);
    inCPTReadEx(MCC_HOST);
    inPCTReadEx(1);
    inCSTReadEx(1);
    //inTCPReadEx(1);
    inDatabase_TerminalCloseDatabase();

    //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);

    return 0;
}

// sidumili: added for edit host info
int inCallJAVA_EditInfoListViewUI(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[512];
    int inRet = 0;

	vdDebug_LogPrintf("=====inCallJAVA_EditInfoListViewUI=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);
  
  if (strlen(pbDispMsg)>0)
	  strcpy(uszBuffer, pbDispMsg);
	
	vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "EditInfoListViewUI", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "EditInfoListViewUI");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL; 
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); 
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_EditInfoListViewUI");
    return d_OK;
}

int inCallJAVA_GetMobileInfo(BYTE *pbOutStr)
{

    unsigned char uszBuffer[528];
    int inRet = 0;


    vdDebug_LogPrintf("saturn =====inCallJAVA_GetMobileInfo=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);


    strcpy(uszBuffer, "test");

    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "GetMOBILESettings", "(Ljava/lang/String;)Ljava/lang/String;");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("saturn mobile settings %s", str);
        //*pbOutLen = strlen(str);
        //vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);
        (*env)->ReleaseStringUTFChars(env, result, str);



    }
    //else
        //*pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    //(*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("saturn end inCallJAVA_GetMobileInfo");
    return d_OK;
}

int inCallJAVA_InputQWERTY(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[512+1];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_InputQWERTY=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    vdDebug_LogPrintf("pbDispMsg[%s]", pbDispMsg);

    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);
    else
        strcpy(uszBuffer, "PASS IN MSG");


    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

//	jmethodID methodID = (*env)->GetMethodID(env, clazz, "rndGetNumStr", "(Ljava/lang/String;)Ljava/lang/String;");
    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "InputQWERTYUI", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "InputQWERTYUI");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
    //rs = (*jvm)->DetachCurrentThread(jvm);

    jbyte* str = NULL; //(*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_InputQWERTY");
    return d_OK;
}


JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inDoAutoReboot(JNIEnv *env, jobject thiz) {

	CTOS_RTC rtcClock;
	BYTE szCurrTime[7] = {0};
	int inCurrTime = 0;
	int inAutoRebooTime;

	
    vdDebug_LogPrintf("saturn check autoreboot");
	// Current Time
	memset(szCurrTime, 0x00, sizeof(szCurrTime));
	CTOS_RTCGet(&rtcClock);
	sprintf(szCurrTime,"%02d%02d", rtcClock.bHour, rtcClock.bMinute);
	inCurrTime = wub_str_2_long(szCurrTime);

	//put_env_int("AUTOREBOOTTIME",1200);

	inAutoRebooTime = get_env_int("AUTOREBOOTTIME");

	vdDebug_LogPrintf("saturn autoreboot time %d", inAutoRebooTime);
	
	if(inCurrTime == inAutoRebooTime){
		
		vdDebug_LogPrintf("saturn check autoreboot RETURN ONE");
		
		inDisplayMessageBoxWithButtonReboot(1,8,"SLEEP","MODE","REBOOT", 4);
		return 1;
	}else{
	
	vdDebug_LogPrintf("saturn check autoreboot RETURN ZERO");
	    return 0;
	}}

	int inCallJAVA_usSetDateTime(BYTE *pbDispMsg, BYTE *pbOutStr_datetime, BYTE *pbOutLen)
	{
		unsigned char uszBuffer[512+1];
		int inRet = 0;
	
	
		vdDebug_LogPrintf("=====inCallJAVA_usSetDateTime=====");
	
		JNIEnv *env;
		jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
		// Use the env pointer...
		vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);
	
	
	
		if (strlen(pbDispMsg)>0)
			strcpy(uszBuffer, pbDispMsg);
	
	
		vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);
	
		jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
		vdDebug_LogPrintf("jstring[%s]", uszBuffer);
	
		//jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
		//vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");
	
		jmethodID methodID = (*env)->GetMethodID(env, activityClass, "usSetDateAndTime", "(Ljava/lang/String;)Ljava/lang/String;");
	
		vdDebug_LogPrintf("jstring[%s]", "usSetDateTime");
	
		jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
	
		jbyte* str = NULL;
		str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
		if (str!=NULL)
		{
			vdDebug_LogPrintf("%s", str);
			*pbOutLen = strlen(str);
			vdDebug_LogPrintf("strcpy");
			strcpy(pbOutStr_datetime, str);
	
			vdDebug_LogPrintf("ReleaseStringUTFChars");
			(*env)->ReleaseStringUTFChars(env, result, str);
	
		}
		else
		{
			*pbOutLen = 0;
			//strcpy(pbOutStr, "OK");
		}
	
	
		(*env)->DeleteLocalRef(env, jstr);
		//(*env)->DeleteLocalRef(env, clazz);
		(*env)->DeleteLocalRef(env, result);
	
		vdDebug_LogPrintf("end inCallJAVA_usSetDateTime");
		return d_OK;
}


int inCallJAVA_CTMSUPDATE(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[512+1];
    int inRet = 0;

    memset(uszBuffer, 0x00, sizeof(uszBuffer));

    vdDebug_LogPrintf("=====inCallJAVA_CTMSUPDATE=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);

    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "CTMSUPDATE", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "CTMSUPDATE");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_CTMSUPDATE");
    return d_OK;
}


JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOSS_1Get1stInitFlag(JNIEnv *env, jobject thiz) {
		int ret = 0;
		inTCTRead(1);
		ret = strTCT.fFirstInit;
		vdDebug_LogPrintf("inCTOSS_1Get1stInitFlag[%d].............", ret);
	
		return ret;
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOSS_1GetDLNotFinishedFlag(JNIEnv *env,
                                                                            jobject thiz) {
    int ret = 0;
    inTCTRead(1);
    ret = strTCT.byDLNotFinished;
    vdDebug_LogPrintf("inCTOSS_1Get1stInitFlag[%d].............", ret);

    return ret;
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Main_Activity_1ctms_1update_inCTOSS_1BackupDataScript(JNIEnv *env,
                                                                                  jobject thiz) {
    // TODO: implement inCTOSS_BackupDataScript()
    
		vdDebug_LogPrintf("inCTOSS_BackupDataScript.............");
		char szOutPrmFile[100+1];
	
		memset(szOutPrmFile, 0, sizeof(szOutPrmFile));
	//	  inCTOSS_BackupDataScript("/data/data/pub/com.Source.S1_UOB.UOB.tbd", szOutPrmFile);
	//	  inCTOSS_BackupDataScript("/data/data/com.Source.S1_UOB.UOB/com.Source.S1_UOB.UOB.tbd", szOutPrmFile);
		inCTOSS_BackupDataScript("/data/data/com.Source.S1_MCC.MCC/com.Source.S1_MCC.MCC.tbd", szOutPrmFile);
		return 0;
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Main_Activity_1ctms_1update_inCTOSS_1SetDownloadedNotFinishedFlag(
        JNIEnv *env, jobject thiz, jint value) {
    // TODO: implement inCTOSS_SetDownloadedNotFinishedFlag()
    vdDebug_LogPrintf("inCTOSS_SetDownloadedNotFinishedFlag,byDLNotFinished[%d],value[%d].............",strTCT.byDLNotFinished, value);
    strTCT.byDLNotFinished = value;

    //if(value == 0)
    //    strTCT.fFirstInit = 1;

    inTCTSave(1);
    return 0;
}

int inCallJAVA_UserManual(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[250+1];
    int inRet = 0;

    vdDebug_LogPrintf("=====inCallJAVA_UserManual=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);
    //else
    //pbDispMsg = "Tine: UserConfirm activity";

    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "UserManual", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "UserManual");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_UserManual");
    return d_OK;
}

int inCallJAVA_SendEcrResponse()
{
    vdDebug_LogPrintf("AAA<<>>> =====inCallJAVA_SendEcrResponse=====");
    JNIEnv *env;
    if(FALSE)
    {
        vdDebug_LogPrintf("=====Test code, don't send ecr resp=====");
        return d_OK;
    }

    jint rs = (*ecrjvm)->AttachCurrentThread(ecrjvm, &env, NULL);
    jstring jstr = (*env)->NewStringUTF(env, "dummy");

    jmethodID methodID = (*env)->GetMethodID(env, ecrClass, "sendBackEcrResponse", "(Ljava/lang/String;)V");

    (*env)->CallVoidMethod(env, ecrObj, methodID, jstr);
    vdDebug_LogPrintf("Clear buff");
    (*env)->DeleteLocalRef(env, jstr);

    vdDebug_LogPrintf("=====inCallJAVA_SendEcrResponse exit=====");
    return d_OK;
}

int inCallJAVA_checkAppRunning(char *pAppame)
{
    unsigned char uszBuffer[500+1];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_checkAppRunning=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);

    if (strlen(pAppame)>0)
        strcpy(uszBuffer, pAppame);
    else
        strcpy(uszBuffer, "PASS IN MSG");

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "checkAppRunning", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "checkAppRunning");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
    //rs = (*jvm)->DetachCurrentThread(jvm);

    jbyte* str = NULL; //(*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)

    if(result != NULL)
        str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        if(memcmp(str, "1", 1) == 0)
            inRet = 1;
        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);


    }
    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end checkAppRunning", inRet);

//	ProcessingUI();
    return inRet;
}

JNIEXPORT jint
JNICALL
Java_com_Source_S1_1MCC_MCC_EcrCommandReceiver_vdEcrSetupInstance(JNIEnv *env, jobject instance) {
    vdDebug_LogPrintf("=====vdEcrSetupInstance=====");
    if(ecrClass != NULL)
        (*env)->DeleteGlobalRef(env, ecrClass);
    if(ecrObj != NULL)
        (*env)->DeleteGlobalRef(env, ecrObj);

    jint rs = (*env)->GetJavaVM(env, &ecrjvm);
    jclass cls = (*env)->GetObjectClass(env, instance);
    ecrClass = (jclass) (*env)->NewGlobalRef(env, cls);
    ecrObj = (*env)->NewGlobalRef(env, instance);
    vdDebug_LogPrintf("=====vdEcrSetupInstance exit=====");
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);

    return 0;
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOSS_1ECR_1Menu(JNIEnv *env, jobject instance, int inECRTxnType) {

// TODO
    int inRetVal = 0, inRet = 0;
    BYTE outbuf[d_MAX_IPC_BUFFER];
    USHORT out_len = 0;
    char cwd[1024];
    
    if(activityClass != NULL)
        (*env)->DeleteGlobalRef(env, activityClass);
    if(activityObj != NULL)
        (*env)->DeleteGlobalRef(env, activityObj);
    
    jint rs = (*env)->GetJavaVM(env, &jvm);
    
    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);
    
    fEntryCardfromIDLE=VS_FALSE;
    vdSetECRTransactionFlg(VS_TRUE);
    vdGetGlobalVariable();

    inRetVal = inTCTRead(1);
    vdDebug_LogPrintf("AAA>>...[inECRTxnType[%d], strTCT.fECRSale[%d], strTCT.fECRBancnet[%d]", inECRTxnType, strTCT.fECRSale, strTCT.fECRBancnet);

switch (inECRTxnType)
{
     case 1:
          if((strTCT.fECRSale == TRUE) && (strTCT.fECRBancnet == FALSE)&& (strTCT.fECRQRPay == FALSE)&& (strTCT.fECRQRPh == FALSE))
          {
               inCTOS_WAVE_SALE();
          }
          else if((strTCT.fECRSale == FALSE) && (strTCT.fECRBancnet == TRUE)&& (strTCT.fECRQRPay == FALSE) && (strTCT.fECRQRPh == FALSE))
          {
               inCTOS_BancnetSale();
          }
		  else if((strTCT.fECRSale == FALSE) && (strTCT.fECRBancnet == FALSE) && (strTCT.fECRQRPay == TRUE) && (strTCT.fECRQRPh == FALSE))
          {
               inQRPAY();
          }
		  else if((strTCT.fECRSale == FALSE) && (strTCT.fECRBancnet == FALSE) && (strTCT.fECRQRPay == FALSE) && (strTCT.fECRQRPh == TRUE))
          {
               inPHQRSale();
          }
          else
          	{
               inRet=inSelectECRTrxn();
			   vdDebug_LogPrintf("SAA>> srTransRec.HDTid[%d] inRet[%d] srTransRec.szECRRespCode[%s], srTransRec.szRespCode[%s], srTransRec.szResponseText[%s] strlen(srTransRec.szECRRespCode)[%d]",srTransRec.HDTid, inRet, srTransRec.szECRRespCode, srTransRec.szRespCode, srTransRec.szResponseText, strlen(srTransRec.szECRRespCode));
               if(inRet!=d_OK)
               {
                    inMultiAP_ECRSendSuccessResponse();
               }
			   if((srTransRec.HDTid!=MCC_HOST) && (srTransRec.HDTid!=BANCNET_HOST) && (inRet==d_OK) && (strlen(srTransRec.szECRRespCode)> 0))
			   	{
			   	    inMultiAP_ECRSendSuccessResponse();
			   	}
			   if((srTransRec.HDTid!=MCC_HOST) && (srTransRec.HDTid!=BANCNET_HOST) && (inRet==d_OK) && (strlen(srTransRec.szECRRespCode) <= 0))
			   	{
			   	    //memcpy(srTransRec.szECRRespCode, ECR_DECLINED_ERR,2);
					//memcpy(srTransRec.szResponseText, ECR_OPER_CANCEL_RESP,2);
			   	    inMultiAP_ECRSendSuccessResponse();
			   	}
			   	
          	}
     	  break;

     case 2:
          inCTOS_VOID();
          break;
}

inCallJAVA_GetMenu();

    vdSetECRTransactionFlg(0);
    put_env_int("ECRPROCESS",0);
    put_env_int("APP_STARTED", 0);
//release memory
if(cls != NULL)
  (*env)->DeleteLocalRef(env, cls);

return 0;
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOSS_1Reports(JNIEnv *env, jobject instance) {
    char trxlogo[20+1];
	int inSaleType=0, inRet=0;

	char szTemp[2048];
	char szTemp2[2048];
	char szTemp3[2048];
	
    put_env_int("APP_STARTED", 1);
	vdGetGlobalVariable();
	
	if(activityClass != NULL)
         (*env)->DeleteGlobalRef(env, activityClass);
    if(activityObj != NULL)
         (*env)->DeleteGlobalRef(env, activityObj);

    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);

	/*
    vdDebug_LogPrintf("1.TLE_ENCRYPTION");
    //02003020078020C0020600000000000000010000020900520001002100375464980026130014D23052010790000000000F333032383530303530303030303130303032383530323101475F2A0206085F340101820239008407A0000000041010950504000080009A032302169C01009F02060000000001009F03060000000000009F090200029F10120210A00001220000000000000000000000FF9F1A0206089F1E0831323334353637389F26084833C0439E7206499F2701809F3303E008C89F34031F03029F3501229F360210899F37043EDEC6699F4104000000030006303030303038004700173138303030313538323033363131363838002631393030303030303030303030303030303030303030303030308000
    memset(szTemp, 0x00, sizeof(szTemp));
	memset(szTemp2, 0x00, sizeof(szTemp2));
	memset(szTemp3, 0x00, sizeof(szTemp3));
    strcpy(szTemp, "02003020078020C0020600000000000000010000020900520001002100375464980026130014D23052010790000000000F333032383530303530303030303130303032383530323101475F2A0206085F340101820239008407A0000000041010950504000080009A032302169C01009F02060000000001009F03060000000000009F090200029F10120210A00001220000000000000000000000FF9F1A0206089F1E0831323334353637389F26084833C0439E7206499F2701809F3303E008C89F34031F03029F3501229F360210899F37043EDEC6699F4104000000030006303030303038004700173138303030313538323033363131363838002631393030303030303030303030303030303030303030303030308000");
    int len=wub_str_2_hex(szTemp, szTemp3 ,strlen(szTemp));
	inGetMACEx(szTemp3, len, "\x41\x1A\xBE\xE6\x4B\x73\xA6\xB3\x6F\xB7\x9C\xA8\x85\x0E\xB3\x89", szTemp2);

	vdDebug_LogPrintf("2.TLE_ENCRYPTION");
    memset(szTemp, 0x00, sizeof(szTemp));
	memset(szTemp2, 0x00, sizeof(szTemp2));
	memset(szTemp3, 0x00, sizeof(szTemp3));
    strcpy(szTemp, "600021000002003020078020C0020600000000000000010000020900520001002100375464980026130014D23052010790000000000F333032383530303530303030303130303032383530323101475F2A0206085F340101820239008407A0000000041010950504000080009A032302169C01009F02060000000001009F03060000000000009F090200029F10120210A00001220000000000000000000000FF9F1A0206089F1E0831323334353637389F26084833C0439E7206499F2701809F3303E008C89F34031F03029F3501229F360210899F37043EDEC6699F410400000003000630303030303800470017313830303031353832303336313136383800263139303030303030303030303030303030303030303030303030127A5267A9C6FEA28000000000");
    len=wub_str_2_hex(szTemp, szTemp3 ,strlen(szTemp));
	vdTLE_Encrypt(szTemp3, "\x41\x1A\xBE\xE6\x4B\x73\xA6\xB3\x6F\xB7\x9C\xA8\x85\x0E\xB3\x89", szTemp2, len);
	vdDebug_LogPrintf("3.TLE_ENCRYPTION");

	memset(szTemp, 0x00, sizeof(szTemp));
	memset(szTemp2, 0x00, sizeof(szTemp2));
	memset(szTemp3, 0x00, sizeof(szTemp3));
    strcpy(szTemp, "271E9D9A3CCFE9C073FFD319A986F250524D756ADF737DA1160536495EE0F5DF53DCAC059997C6C1D124F363F51AE5394440D45A1E28FA0EAD777F37B3801CCE072847593FB54E63F2BF795276D7282A48DCB33D0CAB8510BAA4E113689996E01FED1CD901D62E22571F1D570DABAE66ECF6BA8F8968D5453E96455B91F3D3FE92BB04D49F63D18EFA8B6D01F1CA655D");
    len=wub_str_2_hex(szTemp, szTemp3 ,strlen(szTemp));
	vdTLE_Decrypt(szTemp3, "\x41\x1A\xBE\xE6\x4B\x73\xA6\xB3\x6F\xB7\x9C\xA8\x85\x0E\xB3\x89", szTemp2, len);
	vdDebug_LogPrintf("4.TLE_ENCRYPTION");
	*/
	
	DisplayStatusLine(" ");
	//usCTOSS_ClearScreen();

	inSaleType = 'R';
	memset(trxlogo, 0x00, sizeof(trxlogo));
	strcpy(trxlogo, "allcards.bmp");
	vdDisplayTrxn(inSaleType, trxlogo);

    //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);

	put_env_int("APP_STARTED", 0);
    return 0;
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOSS_1Settlement(JNIEnv *env, jobject instance) {
    char trxlogo[20+1];
	int inSaleType=0, inRet=0;
    int inAppType=get_env_int("APPTYPE");
	
    put_env_int("APP_STARTED", 1);
	vdGetGlobalVariable();
	
	if(activityClass != NULL)
         (*env)->DeleteGlobalRef(env, activityClass);
    if(activityObj != NULL)
         (*env)->DeleteGlobalRef(env, activityObj);

    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);

	DisplayStatusLine(" ");
	//usCTOSS_ClearScreen();

    #if 0
	srTransRec.byTransType = SETTLE;
	inRet = inCTOS_GetTxnPassword();
	if (d_OK != inRet)
		return inRet;
	#endif
	
	inSaleType = 'P';
	memset(trxlogo, 0x00, sizeof(trxlogo)); 	
	sprintf(trxlogo,"supported%d.bmp", inAppType);
	vdDebug_LogPrintf("trxlogo [%s], inSaleType [%d]", trxlogo, inSaleType);
	vdDisplayTrxn(inSaleType, trxlogo);
	
    //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);

    put_env_int("APP_STARTED", 0);
	
    return 0;
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOSS_1Settings(JNIEnv *env, jobject instance) {
    char trxlogo[20+1];
	int inSaleType=0, inRet=0;

    put_env_int("APP_STARTED", 1);
	vdGetGlobalVariable();
	
	if(activityClass != NULL)
         (*env)->DeleteGlobalRef(env, activityClass);
    if(activityObj != NULL)
         (*env)->DeleteGlobalRef(env, activityObj);

    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);

	DisplayStatusLine(" ");
	//usCTOSS_ClearScreen();

	srTransRec.byTransType = SETUP;
	inRet = inCTOS_GetTxnPassword();
	if (d_OK != inRet)
		return inRet;
	
	inSaleType = 0;
	memset(trxlogo, 0x00, sizeof(trxlogo));
	strcpy(trxlogo, "allcards.bmp");
	vdDebug_LogPrintf("trxlogo [%s], inSaleType [%d]", trxlogo, inSaleType);
	vdDisplayTrxn(inSaleType, trxlogo);
	
    //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);

    put_env_int("APP_STARTED", 0);
	
    return 0;
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOSS_1Credit(JNIEnv *env, jobject instance) {
    char trxlogo[20+1];
	int inSaleType=0, inRet=0;

    put_env_int("APP_STARTED", 1);
	vdGetGlobalVariable();
	
	if(activityClass != NULL)
         (*env)->DeleteGlobalRef(env, activityClass);
    if(activityObj != NULL)
         (*env)->DeleteGlobalRef(env, activityObj);

    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);

	DisplayStatusLine(" ");
	//usCTOSS_ClearScreen();

	inSaleType = 1;
	memset(trxlogo, 0x00, sizeof(trxlogo));
	strcpy(trxlogo, "credit.bmp");
	vdDebug_LogPrintf("trxlogo [%s], inSaleType [%d]", trxlogo, inSaleType);
	vdDisplayTrxn(inSaleType, trxlogo);

    //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);

    put_env_int("APP_STARTED", 0);
	
    return 0;
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOSS_1Bancnet(JNIEnv *env, jobject instance) {
    char trxlogo[20+1];
	int inSaleType=0, inRet=0;

    put_env_int("APP_STARTED", 1);
	vdGetGlobalVariable();
	
	if(activityClass != NULL)
         (*env)->DeleteGlobalRef(env, activityClass);
    if(activityObj != NULL)
         (*env)->DeleteGlobalRef(env, activityObj);

    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);

	DisplayStatusLine(" ");
	//usCTOSS_ClearScreen();

	inSaleType = 2;
	memset(trxlogo, 0x00, sizeof(trxlogo));
	strcpy(trxlogo, "bancnet.bmp");
	vdDebug_LogPrintf("trxlogo [%s], inSaleType [%d]", trxlogo, inSaleType);
	vdDisplayTrxn(inSaleType, trxlogo);

    //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);

    put_env_int("APP_STARTED", 0);
	
    return 0;
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOSS_1PHQR(JNIEnv *env, jobject instance) {
    char trxlogo[20+1];
	int inSaleType=0, inRet=0;

	put_env_int("APP_STARTED", 1);
	vdGetGlobalVariable();
	
    if(activityClass != NULL)
         (*env)->DeleteGlobalRef(env, activityClass);
    if(activityObj != NULL)
         (*env)->DeleteGlobalRef(env, activityObj);

    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);

	DisplayStatusLine(" ");
	//usCTOSS_ClearScreen();

    inRet = inCheckBattery();	
    if(inRet == d_OK)
    {
        inSaleType = 4;
        memset(trxlogo, 0x00, sizeof(trxlogo));
        strcpy(trxlogo, "phqr.bmp");
        vdDebug_LogPrintf("trxlogo [%s], inSaleType [%d]", trxlogo, inSaleType);
        vdDisplayTrxn(inSaleType, trxlogo);
    }
	
    //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);

	put_env_int("APP_STARTED", 0);
	
    return 0;
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOSS_1Wallet(JNIEnv *env, jobject instance) {
    char trxlogo[20+1];
	int inSaleType=0, inRet=0;

    put_env_int("APP_STARTED", 1);
	vdGetGlobalVariable();
	
	if(activityClass != NULL)
         (*env)->DeleteGlobalRef(env, activityClass);
    if(activityObj != NULL)
         (*env)->DeleteGlobalRef(env, activityObj);

    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);

	DisplayStatusLine(" ");
	//usCTOSS_ClearScreen();

    inRet = inCheckBattery();	
    if(inRet == d_OK)
    {
        inSaleType = 3;
        memset(trxlogo, 0x00, sizeof(trxlogo));
        strcpy(trxlogo, "mccpay.bmp");
        vdDebug_LogPrintf("trxlogo [%s], inSaleType [%d]", trxlogo, inSaleType);
        vdDisplayTrxn(inSaleType, trxlogo);
    }
	
    //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);

	put_env_int("APP_STARTED", 0);
	
    return 0;
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOSS_1Register(JNIEnv *env, jobject instance) {
    char trxlogo[20+1];
	int inSaleType=0, inRet=1;

	put_env_int("APP_STARTED", 1);
	vdGetGlobalVariable();
	
	if(activityClass != NULL)
         (*env)->DeleteGlobalRef(env, activityClass);
    if(activityObj != NULL)
         (*env)->DeleteGlobalRef(env, activityObj);

    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);

	DisplayStatusLine(" ");
	//usCTOSS_ClearScreen();
	
    inTCTRead(1);

    if(strTCT.fRegister == 0) {
        vdDebug_LogPrintf("inRegister:  START");
        inCTOS_Reg();
    }
	else if(strTCT.fRegister == 1 && strTCT.inMenuid == 11)
	{
		vdDebug_LogPrintf("inRegister:  START 2");
		inCTOS_Reg();
    }
	
    //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);

	put_env_int("APP_STARTED", 0);
	
    return 0;
}

int inCallJAVA_ClearScreen(BYTE *pbDispMsg, BYTE *pbOutStr_disp, BYTE *pbOutLen)
{
    unsigned char uszBuffer[528];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_ClearScreen=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);


    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);

    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "ClearScreen", "(Ljava/lang/String;)Ljava/lang/String;");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr_disp, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
    {
		*pbOutLen = 0;
		//strcpy(pbOutStr, "OK");
    }


    (*env)->DeleteLocalRef(env, jstr);
    //(*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_LCDDisplay");
    return d_OK;
}

int inCallJAVA_TLE_MAC(BYTE *pbInStr, BYTE *pbOutStr, int inLen)
{
	unsigned char uszBuffer[2048+1];
	int inRet = 0;

	vdDebug_LogPrintf("=====inCallJAVA_GetMAC=====");

	JNIEnv *env;
	jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
	// Use the env pointer...
	vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    vdDebug_LogPrintf("inCallJAVA_GetMAC.pbInStr[%s]", pbInStr);
	//vdDebug_LogPrintf("inCallJAVA_GetMAC.pbInStrKey[%s]", pbInStrKey);
	
	if (strlen(pbInStr)>0)
	{
		strcpy(uszBuffer, pbInStr);
	}
	//else
	//pbDispMsg = "Tine: UserConfirm activity";

	vdDebug_LogPrintf("1.uszBuffer[%s]", uszBuffer);

	jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
	vdDebug_LogPrintf("jstring[%s]", "This this Pass in string data to Java");

	jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
	vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

//	jmethodID methodID = (*env)->GetMethodID(env, clazz, "rndGetNumStr", "(Ljava/lang/String;)Ljava/lang/String;");
	jmethodID methodID = (*env)->GetMethodID(env, activityClass, "TLE_MAC", "(Ljava/lang/String;)Ljava/lang/String;");

	vdDebug_LogPrintf("jstring[%s]", "GetAmountString");

	jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
	//rs = (*jvm)->DetachCurrentThread(jvm);

	jbyte* str = NULL; //(*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
	str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
	if (str!=NULL)
	{
		vdDebug_LogPrintf("%s", str);
		//*pbAmtLen = strlen(str);
		vdDebug_LogPrintf("strcpy");
		strcpy(pbOutStr, str);

		vdDebug_LogPrintf("ReleaseStringUTFChars");
		(*env)->ReleaseStringUTFChars(env, result, str);

	}
	//else
		//*pbAmtLen = 0;

	(*env)->DeleteLocalRef(env, jstr);
	(*env)->DeleteLocalRef(env, clazz);
	(*env)->DeleteLocalRef(env, result);

	vdDebug_LogPrintf("end inCallJAVA_GetMAC");
	return d_OK;
}

int inCallJAVA_TLE_Encrypt(BYTE *pbInStr, BYTE *pbOutStr, int inLen)
{
	unsigned char uszBuffer[2048+1];
	int inRet = 0;

	vdDebug_LogPrintf("=====inCallJAVA_TLE_Encrypt=====");

	JNIEnv *env;
	jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
	// Use the env pointer...
	vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    vdDebug_LogPrintf("inCallJAVA_GetMAC.pbInStr[%s]", pbInStr);
	//vdDebug_LogPrintf("inCallJAVA_GetMAC.pbInStrKey[%s]", pbInStrKey);
	
	if (strlen(pbInStr)>0)
	{
		strcpy(uszBuffer, pbInStr);
	}
	//else
	//pbDispMsg = "Tine: UserConfirm activity";

	vdDebug_LogPrintf("1.uszBuffer[%s]", uszBuffer);

	jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
	vdDebug_LogPrintf("jstring[%s]", "This this Pass in string data to Java");

	jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
	vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

//	jmethodID methodID = (*env)->GetMethodID(env, clazz, "rndGetNumStr", "(Ljava/lang/String;)Ljava/lang/String;");
	jmethodID methodID = (*env)->GetMethodID(env, activityClass, "TLE_Encrypt", "(Ljava/lang/String;)Ljava/lang/String;");

	vdDebug_LogPrintf("jstring[%s]", "GetAmountString");

	jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
	//rs = (*jvm)->DetachCurrentThread(jvm);

	jbyte* str = NULL; //(*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
	str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
	if (str!=NULL)
	{
		vdDebug_LogPrintf("%s", str);
		//*pbAmtLen = strlen(str);
		vdDebug_LogPrintf("strcpy");
		strcpy(pbOutStr, str);

		vdDebug_LogPrintf("ReleaseStringUTFChars");
		(*env)->ReleaseStringUTFChars(env, result, str);

	}
	//else
		//*pbAmtLen = 0;

	(*env)->DeleteLocalRef(env, jstr);
	(*env)->DeleteLocalRef(env, clazz);
	(*env)->DeleteLocalRef(env, result);

	vdDebug_LogPrintf("end inCallJAVA_TLE_Encrypt");
	return d_OK;
}

int inCallJAVA_TLE_Decrypt(BYTE *pbInStr, BYTE *pbOutStr, int inLen)
{
	unsigned char uszBuffer[2048+1];
	int inRet = 0;

	vdDebug_LogPrintf("=====inCallJAVA_TLE_Decrypt=====");

	JNIEnv *env;
	jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
	// Use the env pointer...
	vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    vdDebug_LogPrintf("inCallJAVA_GetMAC.pbInStr[%s]", pbInStr);
	//vdDebug_LogPrintf("inCallJAVA_GetMAC.pbInStrKey[%s]", pbInStrKey);
	
	if (strlen(pbInStr)>0)
	{
		strcpy(uszBuffer, pbInStr);
	}
	//else
	//pbDispMsg = "Tine: UserConfirm activity";

	vdDebug_LogPrintf("1.uszBuffer[%s]", uszBuffer);

	jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
	vdDebug_LogPrintf("jstring[%s]", "This this Pass in string data to Java");

	jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
	vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

//	jmethodID methodID = (*env)->GetMethodID(env, clazz, "rndGetNumStr", "(Ljava/lang/String;)Ljava/lang/String;");
	jmethodID methodID = (*env)->GetMethodID(env, activityClass, "TLE_Decrypt", "(Ljava/lang/String;)Ljava/lang/String;");

	vdDebug_LogPrintf("jstring[%s]", "GetAmountString");

	jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
	//rs = (*jvm)->DetachCurrentThread(jvm);

	jbyte* str = NULL; //(*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
	str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
	if (str!=NULL)
	{
		vdDebug_LogPrintf("%s", str);
		//*pbAmtLen = strlen(str);
		vdDebug_LogPrintf("strcpy");
		strcpy(pbOutStr, str);

		vdDebug_LogPrintf("ReleaseStringUTFChars");
		(*env)->ReleaseStringUTFChars(env, result, str);

	}
	//else
		//*pbAmtLen = 0;

	(*env)->DeleteLocalRef(env, jstr);
	(*env)->DeleteLocalRef(env, clazz);
	(*env)->DeleteLocalRef(env, result);

	vdDebug_LogPrintf("end inCallJAVA_TLE_Decrypt");
	return d_OK;
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOSS_1GetEnvInt(JNIEnv *env, jobject instance, jstring tag) {
    int ret = 0;

    const char *szFmt = (*env)->GetStringUTFChars(env, tag, 0);

	ret=get_env_int(szFmt);
    //inTCTRead(1);
    //ret = strTCT.fFirstInit;
    vdDebug_LogPrintf("inCTOSS_1GetEnvInt[%d].............", ret);

    return ret;
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Main_Activity_1ctms_1update_inCTOSS_1GetEnvInt(JNIEnv *env,
                                                                           jobject thiz,
                                                                           jstring sztag) {
    // TODO: implement inCTOSS_GetEnvInt()
    int ret = 0;

    const char *szFmt = (*env)->GetStringUTFChars(env, sztag, 0);

    ret = get_env_int(szFmt);
    vdDebug_LogPrintf("inCTOSS_1GetEnvInt[%d].............", ret);

    return ret;
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Main_Activity_1ctms_1update_inCTOSS_1SetEnvInt(JNIEnv *env,
                                                                           jobject thiz,
                                                                           jstring sztag,
                                                                           jint invalue) {
    // TODO: implement inCTOSS_SetEnvInt()
     int     ret = -1;
    char    buf[64];
	const char *szFmt = (*env)->GetStringUTFChars(env, sztag, 0);

    memset (buf, 0, sizeof (buf));
    sprintf(buf, "%d", invalue);
    ret = inCTOSS_PutEnvDB (szFmt, buf);

    vdDebug_LogPrintf("inCTOSS_SetEnvInt [%s]=[%d] ret[%d]", szFmt, invalue, ret);
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOSS_1CreatefsdataFolder(JNIEnv *env,
																		 jobject thiz) {
   // TODO: implement inCTOSS_CreatefsdataFolder()
   #define d_FILE_EXIST 0x01

   BYTE pbaFileName[20] = {"PatTesting.txt"};
   BYTE bStorageType = 0;
   ULONG ulFileSize = 0, ulHandle;
   int ret = 0;

   vdDebug_LogPrintf("CreatefsdataFolder");
   
   /* Get the file size with specific file name.
	* If Get file size > 0, the file is already existed 	 */
   ret = CTOS_FileGetSize(pbaFileName,&ulFileSize);

   if (ulFileSize > 0)
	   return d_FILE_EXIST;

   if ((ret != d_OK) && (ret != d_FS_FILE_NOT_FOUND))
	   return ret;

   /* Open a file and return a number called a file handle.
	* If the specified file name does not exist , it will be created first. */
   ret =CTOS_FileOpen(pbaFileName , bStorageType , &ulHandle);

   if (ret == d_OK)
	   CTOS_FileClose(ulHandle);
}

 
 int inCallJAVA_CTMSUPDATEBackGround(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
 {
	 unsigned char uszBuffer[1024+1];
	 int inRet = 0;
 
	 memset(uszBuffer, 0x00, sizeof(uszBuffer));
 
	 vdDebug_LogPrintf("=====inCallJAVA_CTMSUPDATEBackGround=====");
 
	 JNIEnv *env;
	 jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
	 // Use the env pointer...
	 vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);
 
	 if (strlen(pbDispMsg)>0)
		 strcpy(uszBuffer, pbDispMsg);
 
	 vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);
 
	 jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
	 vdDebug_LogPrintf("jstring[%s]", uszBuffer);
 
	 jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
	 vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/BDO/MCC/MainActivity");
 
	 jmethodID methodID = (*env)->GetMethodID(env, activityClass, "CTMSUPDATEBackground", "(Ljava/lang/String;)Ljava/lang/String;");
 
	 vdDebug_LogPrintf("jstring[%s]", "CTMSUPDATE");
 
	 jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
 
	 jbyte* str = NULL;
	 str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
	 if (str!=NULL)
	 {
		 vdDebug_LogPrintf("%s", str);
		 *pbOutLen = strlen(str);
		 vdDebug_LogPrintf("strcpy");
		 strcpy(pbOutStr, str);
 
		 vdDebug_LogPrintf("ReleaseStringUTFChars");
		 (*env)->ReleaseStringUTFChars(env, result, str);
 
	 }
	 else
		 *pbOutLen = 0;
 
	 (*env)->DeleteLocalRef(env, jstr);
	 (*env)->DeleteLocalRef(env, clazz);
	 (*env)->DeleteLocalRef(env, result);
 
	 vdDebug_LogPrintf("end inCallJAVA_CTMSUPDATEBackGround");
	 return d_OK;
 }

 
 JNIEXPORT jint JNICALL
 Java_com_Source_S1_1MCC_MCC_Main_Activity_1ctms_1background_inCTOSS_1GetEnvInt(JNIEnv *env,
																			jobject thiz,
																			jstring sztag) {
	 // TODO: implement inCTOSS_GetEnvInt()
	 int ret = 0;
 
	 const char *szFmt = (*env)->GetStringUTFChars(env, sztag, 0);
 
	 ret = get_env_int(szFmt);
	 vdDebug_LogPrintf("inCTOSS_1GetEnvInt[%d].............", ret);
 
	 return ret;
 }
 
 JNIEXPORT jint JNICALL
 Java_com_Source_S1_1MCC_MCC_Main_Activity_1ctms_1background_inCTOSS_1SetEnvInt(JNIEnv *env,
																			jobject thiz,
																			jstring sztag,
																			jint invalue) {
	 // TODO: implement inCTOSS_SetEnvInt()
	 int	 ret = -1;
	 char	 buf[64];
	 const char *szFmt = (*env)->GetStringUTFChars(env, sztag, 0);
 
	 memset (buf, 0, sizeof (buf));
	 sprintf(buf, "%d", invalue);
	 ret = inCTOSS_PutEnvDB (szFmt, buf);
 
	 vdDebug_LogPrintf("inCTOSS_SetEnvInt [%s]=[%d] ret[%d]", szFmt, invalue, ret);
 }
																			
 JNIEXPORT jint JNICALL
 Java_com_Source_S1_1MCC_MCC_Main_Activity_1ctms_1background_inCTOSS_1BackupDataScript(JNIEnv *env,
																				   jobject thiz) {
	 // TODO: implement inCTOSS_BackupDataScript()
 
	 vdDebug_LogPrintf("inCTOSS_BackupDataScript.............");
	 char szOutPrmFile[100+1];
 
	 memset(szOutPrmFile, 0, sizeof(szOutPrmFile));
	 //    inCTOSS_BackupDataScript("/data/data/pub/com.Source.S1_UOB.UOB.tbd", szOutPrmFile);
	 //    inCTOSS_BackupDataScript("/data/data/com.Source.S1_UOB.UOB/com.Source.S1_UOB.UOB.tbd", szOutPrmFile);
	 inCTOSS_BackupDataScript("/data/data/com.Source.S1_MCC.MCC/com.Source.S1_MCC.MCC.tbd", szOutPrmFile);
	 return 0;
 }

int inCallJAVA_switchGPRS()
{
   int inRet = 0;

   vdDebug_LogPrintf("=====inCallJAVA_switchGPRS=====");

   JNIEnv *env;
   jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
   // Use the env pointer...
   vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

   jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
   vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

   jmethodID methodID = (*env)->GetMethodID(env, activityClass, "switchGPRS", "()V");

   vdDebug_LogPrintf("jstring[%s]", "switchGPRS");
   if (methodID)
   {
	   vdDebug_LogPrintf("GetMethodID[%s]", "switchGPRS");
	   (*env)->CallVoidMethod(env, activityObj, methodID);
   }

   if (clazz)
	   (*env)->DeleteLocalRef(env, clazz);

   return inRet;
}


JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCC_MCC_Main_MainActivity_inCTOSS_1PutEnvInt(JNIEnv *env, jobject instance, jstring tag, int inValue) {
    int ret = 0;

    const char *szFmt = (*env)->GetStringUTFChars(env, tag, 0);

	put_env_int(szFmt, inValue);
    //inTCTRead(1);
    //ret = strTCT.fFirstInit;
    vdDebug_LogPrintf("inCTOSS_1PutEnvInt[%d].............", ret);

    return ret;
}

