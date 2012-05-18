#ifndef DATA_BASE_PACKET_HEAD_FILE
#define DATA_BASE_PACKET_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//�������ݰ�

//�û��¼�
#define	DBR_GR_LOGON_USERID			100									//I D ��¼
#define	DBR_GR_LOGON_MOBILE			101									//�ֻ���¼
#define	DBR_GR_LOGON_ACCOUNTS		102									//�ʺŵ�¼

//ϵͳ�¼�
#define DBR_GR_WRITE_GAME_SCORE		200									//��Ϸ����
#define DBR_GR_LEAVE_GAME_SERVER	201									//�뿪����
#define DBR_GR_GAME_SCORE_RECORD	202									//���ּ�¼

//�����¼�
#define DBR_GR_LOAD_PARAMETER		300									//���ز���
#define DBR_GR_LOAD_GAME_COLUMN		301									//�����б�
#define DBR_GR_LOAD_ANDROID_USER	302									//���ػ���
#define DBR_GR_LOAD_GAME_PROPERTY	303									//���ص���

//�����¼�
#define DBR_GR_USER_SAVE_SCORE		400									//������
#define DBR_GR_USER_TAKE_SCORE		401									//��ȡ���
#define DBR_GR_QUERY_INSURE_INFO	402									//��ѯ����

//��Ϸ�¼�
#define DBR_GR_GAME_FRAME_REQUEST	400									//��Ϸ����

//////////////////////////////////////////////////////////////////////////////////
//�����Ϣ

//�߼��¼�
#define DBO_GR_LOGON_SUCCESS		100									//��¼�ɹ�
#define DBO_GR_LOGON_FAILURE		101									//��¼ʧ��

//�����¼�
#define DBO_GR_GAME_PARAMETER		200									//������Ϣ
#define DBO_GR_GAME_COLUMN_INFO		201									//�б���Ϣ
#define DBR_GR_GAME_ANDROID_INFO	202									//������Ϣ
#define DBO_GR_GAME_PROPERTY_INFO	203									//������Ϣ

//��������
#define DBO_GR_USER_INSURE_INFO		300									//��������
#define DBO_GR_USER_INSURE_SUCCESS	301									//���гɹ�
#define DBO_GR_USER_INSURE_FAILURE	302									//����ʧ��

//��Ϸ�¼�
#define DBO_GR_GAME_FRAME_RESULT	400									//��Ϸ���

//////////////////////////////////////////////////////////////////////////////////

//ID ��¼
struct DBR_GR_LogonUserID
{		
	//��¼��Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//ID ��¼
struct DBR_GR_LogonMobile
{		
	//��¼��Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//�ʺŵ�¼
struct DBR_GR_LogonAccounts
{
	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//��Ϸ����
struct DBR_GR_WriteGameScore
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwClientAddr;						//���ӵ�ַ

	//�ɼ�����
	tagVariationInfo				VariationInfo;						//��ȡ��Ϣ
};

//�뿪����
struct DBR_GR_LeaveGameServer
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwInoutIndex;						//��¼����
	DWORD							dwLeaveReason;						//�뿪ԭ��
	DWORD							dwOnLineTimeCount;					//����ʱ��

	//�ɼ�����
	tagVariationInfo				RecordInfo;							//��¼��Ϣ
	tagVariationInfo				VariationInfo;						//��ȡ��Ϣ

	//ϵͳ��Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//��Ϸ��¼
struct DBR_GR_GameScoreRecord
{
	//��Ϸ��Ϣ
	WORD							wTableID;							//���Ӻ���
	DWORD							dwPlayTimeCount;					//��Ϸʱ��
	DWORD							dwUserID[MAX_CHAIR];				//�û���ʶ

	//������Ϣ
	SCORE							lWasteCount;						//�����Ŀ
	SCORE							lRevenueCount;						//˰����Ŀ
	SCORE							lGameScore[MAX_CHAIR];				//��Ϸ����
	SCORE							lGameGrade[MAX_CHAIR];				//��Ϸ����
	SCORE							lGameRevenue[MAX_CHAIR];			//��Ϸ˰��
};

//������
struct DBR_GR_UserSaveScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lSaveScore;							//������
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ȡ�����
struct DBR_GR_UserTakeScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lTakeScore;							//��ȡ���
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��ѯ����
struct DBR_GR_QueryInsureInfo
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
};

//////////////////////////////////////////////////////////////////////////////////

//��¼�ɹ�
struct DBO_GR_LogonSuccess
{
	//��������
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwGroupID;							//���� I D
	DWORD							dwCustomID;							//�Զ�ͷ��
	TCHAR							szNickName[LEN_NICKNAME];			//�ʺ��ǳ�
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������

	//�û�����
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	BYTE							cbMasterOrder;						//����ȼ�
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��

	//������Ϣ
	SCORE							lScore;								//�û�����
	SCORE							lGrade;								//�û��ɼ�
	SCORE							lInsure;							//�û�����

	//��Ϸ��Ϣ
	DWORD							dwWinCount;							//ʤ������
	DWORD							dwLostCount;						//ʧ������
	DWORD							dwDrawCount;						//�;�����
	DWORD							dwFleeCount;						//��������
	DWORD							dwUserMedal;						//�û�����
	DWORD							dwExperience;						//�û�����
	DWORD							dwLoveLiness;						//�û�����

	//�û�Ȩ��
	DWORD							dwUserRight;						//�û�Ȩ��
	DWORD							dwMasterRight;						//����Ȩ��

	//��������
	DWORD							dwInoutIndex;						//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ

	//������Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��¼ʧ��
struct DBO_GR_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//������Ϣ
struct DBO_GR_GameParameter
{
	WORD							wMedalRate;							//���ƻ���
	WORD							wRevenueRate;						//����˰��
};

//�б���Ϣ
struct DBO_GR_GameColumnInfo
{
	LONG							lResultCode;						//�������
	BYTE							cbColumnCount;						//�б���Ŀ
	tagColumnItem					ColumnItemInfo[MAX_COLUMN];			//�б���Ϣ
};

//������Ϣ
struct DBO_GR_GameAndroidInfo
{
	LONG							lResultCode;						//�������
	WORD							wAndroidCount;						//�û���Ŀ
	tagAndroidParameter				AndroidParameter[MAX_ANDROID];		//������Ϣ
};

//������Ϣ
struct DBO_GR_GamePropertyInfo
{
	LONG							lResultCode;						//�������
	BYTE							cbPropertyCount;					//������Ŀ
	tagPropertyInfo					PropertyInfo[MAX_PROPERTY];			//������Ϣ
};

//��������
struct DBO_GR_UserInsureInfo
{
	WORD							wServerID;							//�����ʶ
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
};

//���гɹ�
struct DBO_GR_UserInsureSuccess
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lSourceScore;						//ԭ�����
	SCORE							lSourceInsure;						//ԭ�����
	SCORE							lInsureRevenue;						//����˰��
	SCORE							lFrozenedScore;						//�������
	SCORE							lVariationScore;					//��ұ仯
	SCORE							lVariationInsure;					//���б仯
	TCHAR							szDescribeString[128];				//������Ϣ
};

//����ʧ��
struct DBO_GR_UserInsureFailure
{
	LONG							lResultCode;						//��������
	SCORE							lFrozenedScore;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//////////////////////////////////////////////////////////////////////////////////

#endif