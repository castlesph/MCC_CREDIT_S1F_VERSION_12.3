
typedef struct
{
	int PRMid;
	int HDTid;
	char szPromoLabel[21];
	char szPromoCode[3];
	int inGroup;
	//fix for crash- add +1 ro all sizes (null terminator)
	char szTermsEX[4+1];	
	char szOffsetPeriod[4+1];
	char szFundIndicator[1+1];
	char szPlanID1[10+1];
	char szPlanGroup[10+1];
	char szPlanGroupIDX[5+1];
	//end of fix for crash
	int INSHDTid;
} STRUCT_PRM;

STRUCT_PRM strPRM;
STRUCT_PRM strMultiPRM[20];

typedef struct
{
	int PRMid2;
	int HDTid2;
	//crash fix add 1 for null terminantor
	char szPromoLabel2[50+1];
	char szPromoCode2[3+1];
	int inGroup2;
	char szTermsEX2[4+1];		
	char szOffsetPeriod2[4+1];
	char szFundIndicator2[1+1];
	char szPlanID12[10+1];
	char szPlanGroup2[10+1];
	char szPlanGroupIDX2[5+1];
	//end of fix
} STRUCT_PRM2;

STRUCT_PRM2 strPRM2;
STRUCT_PRM2 strMultiPRM2[50]; //[20] - fix system on cash2go and installment if added cash2go data to PRM2 table .


int inPRMReadbyHDTid(int inHDTid, int *inNumRecs);
int inPRMReadbyPRMid(int inPRMid);
int inPRMSave(int inSeekCnt);
int inPRMNumRecord(void);
int inPRMReadbyinInstGroup(int inGroup, int *inNumRecs);
int inPRMReadbyinInstGroup2(int inGroup, int *inNumRecs);

