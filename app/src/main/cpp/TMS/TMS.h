#ifndef __CTOSS_TMS_H__
#define __CTOSS_TMS_H__

int inCTOSS_SettlementCheckTMSDownloadRequest(void);

int inCTOSS_CheckIfPendingTMSDownload(void);
int inCTOSS_TMSChkBatchEmptyProcess(void);
int inCTOSS_TMSChkBatchEmpty(void);
int inCTOSS_TMSDownloadRequest(void);
int inCTOSS_TMSPreConfig(int inComType, BOOL fPrompt);
int inCTOSS_TMSPreConfig2(int inComType, BOOL fPrompt);
int inCTOS_CTMSUPDATE(void);
int inCTOS_CTMSUPDATE_BackGround(void);

#endif //end __CTOSS_TMS_H__
