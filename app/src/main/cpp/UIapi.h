#ifndef __UI_API_H__
#define __UI_API_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <ctosapi.h>

USHORT CTOS_LCDTSetReverse(BOOL boReverse);
USHORT CTOS_LCDTPrintAligned(USHORT usY, UCHAR* pbBuf, BYTE bMode);

USHORT CTOS_LCDSelectModeEx(BYTE bMode,BOOL fClear);
USHORT CTOS_LCDGShowBMPPic(USHORT usX, USHORT usY, BYTE *baFilename);
USHORT CTOS_LCDTPrintXY(USHORT usX, USHORT usY, UCHAR* pbBuf);
USHORT CTOS_LCDTClearDisplay(void);
USHORT CTOS_LCDTTFSwichDisplayMode(USHORT usMode);
USHORT CTOS_LCDTTFSelect(BYTE *baFilename, BYTE bIndex);
USHORT CTOS_LCDTPrintAligned(USHORT usY, UCHAR* pbBuf, BYTE bMode);
USHORT CTOS_LCDBackGndColor(ULONG ulColor);
USHORT CTOS_LCDForeGndColor(ULONG ulColor);
USHORT CTOS_LCDFontSelectMode(BYTE bMode);
USHORT CTOS_LCDGShowPic(USHORT usX, USHORT usY, BYTE* baPat, ULONG ulPatLen, USHORT usXSize);
USHORT CTOS_LCDTClear2EOL(void);
USHORT CTOS_LCDTGotoXY(USHORT usX,USHORT usY);
USHORT CTOS_LCDTPutchXY (USHORT usX, USHORT usY, UCHAR bChar);
USHORT CTOS_LCDTPrint(UCHAR* sBuf);
USHORT CTOS_LCDTSetReverse(BOOL boReverse);
USHORT CTOS_LCDSetContrast(BYTE bValue);
USHORT CTOS_LCDTSelectFontSize(USHORT usFontSize);
//USHORT usCTOSS_Confirm(void);
USHORT usCTOSS_Confirm(BYTE *szDispString);
USHORT usCTOSS_Confirm3(BYTE *szDispString);
//BYTE InputStringUI(BYTE bInputMode,  BYTE bShowAttr, BYTE *pbaStr, USHORT *usStrLen, USHORT usMinLen, USHORT usTimeOutMS, BYTE *password, BYTE *display);
BYTE InputStringUI(BYTE bInputMode,  BYTE bShowAttr, BYTE *pbaStr, USHORT *usStrLen, USHORT usMinLen, USHORT usTimeOutMS, BYTE *szInput);
USHORT usCTOSS_ConfirmInvAndAmt(BYTE *szDispString);

USHORT usCTOSS_LCDDisplay(BYTE *szStringMsg);
USHORT PrintReceiptUI(BYTE *szStringMsg);
USHORT PrintFirstReceiptUI(BYTE *szStringMsg);
USHORT EliteReceiptUI(BYTE *szStringMsg);

BYTE MenuTransType(BYTE *szTrxnMenu);
BYTE MenuTransactions(BYTE *szTrxnMenu, BYTE *szTransFunc);
BYTE IdleMenuTransType(BYTE *szTrxnMenu);
USHORT usEditDatabase(BYTE *szStringMsg);
BYTE InputExpiryDateUI(BYTE *pbaStr, USHORT *usStrLen, USHORT usMinLen, USHORT usTimeOutMS, BYTE *szInput);
USHORT usCARDENTRY(BYTE *szStringMsg);
USHORT ConfirmCardDetails(BYTE *szConfirmCardDetails);
BYTE inDCCFxRate(BYTE *szDCCFxRateDetails);
BYTE InputAlphaUI(BYTE bInputMode,  BYTE bShowAttr, BYTE *pbaStr, USHORT *usMaxLen, USHORT usMinLen, USHORT usTimeOutMS, BYTE *display);
USHORT getAppPackageInfo(BYTE *szAppName, BYTE *szStringMsg);
USHORT usCTOSS_DisplayBox(BYTE *szStringMsg);
USHORT usCTOSS_DisplayErrorMsg2(BYTE *szStringMsg);
BOOL usGetConnectionStatus(int inCommType);
USHORT AnimatePrint(void);
USHORT usCTOSS_DisplayStatusBox(BYTE *szStringMsg);

USHORT usCTOSS_EditInfoListViewUI(BYTE *szDispString, BYTE *szOutputBuf); // sidumili: added for edit host info

USHORT usCTOSS_GetMobileInfo(void);
BYTE InputQWERTY(BYTE bInputMode,  BYTE bShowAttr, BYTE *pbaStr, USHORT *usMaxLen, USHORT usMinLen, USHORT usTimeOutMS, BYTE *display);
USHORT usSetDateTime(BYTE *szDateTime);


USHORT usCTOSS_ClearScreen(void);


#ifdef __cplusplus
}
#endif

#endif

