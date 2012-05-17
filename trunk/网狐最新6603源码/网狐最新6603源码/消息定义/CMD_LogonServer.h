#ifndef CMD_LONGON_HEAD_FILE
#define CMD_LONGON_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//��¼����

#define MDM_GP_LOGON				1									//�㳡��¼

//��¼ģʽ
#define SUB_GP_LOGON_GAMEID			1									//I D ��¼
#define SUB_GP_LOGON_ACCOUNTS		2									//�ʺŵ�¼
#define SUB_GP_REGISTER_ACCOUNTS	3									//ע���ʺ�

//��¼���
#define SUB_GP_LOGON_SUCCESS		100									//��¼�ɹ�
#define SUB_GP_LOGON_FAILURE		101									//��¼ʧ��
#define SUB_GP_LOGON_FINISH			102									//��¼���

//������ʾ
#define SUB_GP_UPDATE_NOTIFY		200									//������ʾ

//////////////////////////////////////////////////////////////////////////////////

//I D ��¼
struct CMD_GP_LogonGameID
{
	//ϵͳ��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//��¼��Ϣ
	DWORD							dwGameID;							//��Ϸ I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
};

//�ʺŵ�¼
struct CMD_GP_LogonAccounts
{
	//ϵͳ��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
};

//ע���ʺ�
struct CMD_GP_RegisterAccounts
{
	//ϵͳ��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szInsurePass[LEN_MD5];				//��������

	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szSpreader[LEN_ACCOUNTS];			//�Ƽ��ʺ�
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//֤������
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����
};

//��¼�ɹ�
struct CMD_GP_LogonSuccess
{
	//��������
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwGroupID;							//���ű�ʶ
	DWORD							dwCustomID;							//�Զ���ʶ
	DWORD							dwUserMedal;						//�û�����
	DWORD							dwExperience;						//������ֵ
	DWORD							dwLoveLiness;						//�û�����

	//�û���Ϣ
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMoorMachine;						//��������
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������
};

//��¼ʧ��
struct CMD_GP_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��½���
struct CMD_GP_LogonFinish
{
	WORD							wIntermitTime;						//�ж�ʱ��
	WORD							wOnLineCountTime;					//����ʱ��
};

//������ʾ
struct CMD_GP_UpdateNotify
{
	BYTE							cbMustUpdate;						//ǿ������
	BYTE							cbAdviceUpdate;						//��������
	DWORD							dwCurrentVersion;					//��ǰ�汾
};

//////////////////////////////////////////////////////////////////////////////////
//Я����Ϣ CMD_GP_LogonSuccess

#define DTP_GP_GROUP_INFO			1									//������Ϣ
#define DTP_GP_MEMBER_INFO			2									//��Ա��Ϣ
#define	DTP_GP_UNDER_WRITE			3									//����ǩ��
#define DTP_GP_STATION_URL			4									//��ҳ��Ϣ

//������Ϣ
struct DTP_GP_GroupInfo
{
	DWORD							dwGroupID;							//��������
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������
};

//��Ա��Ϣ
struct DTP_GP_MemberInfo
{
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	SYSTEMTIME						MemberOverDate;						//����ʱ��
};

//////////////////////////////////////////////////////////////////////////////////
//�б�����

#define MDM_GP_SERVER_LIST			2									//�б���Ϣ

//��ȡ����
#define SUB_GP_GET_LIST				1									//��ȡ�б�
#define SUB_GP_GET_SERVER			2									//��ȡ����
#define SUB_GP_GET_ONLINE			3									//��ȡ����
#define SUB_GP_GET_COLLECTION		4									//��ȡ�ղ�

//�б���Ϣ
#define SUB_GP_LIST_TYPE			100									//�����б�
#define SUB_GP_LIST_KIND			101									//�����б�
#define SUB_GP_LIST_NODE			102									//�ڵ��б�
#define SUB_GP_LIST_PAGE			103									//�����б�
#define SUB_GP_LIST_SERVER			104									//�����б�

//�����Ϣ
#define SUB_GP_LIST_FINISH			200									//�������
#define SUB_GP_SERVER_FINISH		201									//�������

//������Ϣ
#define SUB_GR_KINE_ONLINE			300									//��������
#define SUB_GR_SERVER_ONLINE		301									//��������

//////////////////////////////////////////////////////////////////////////////////

//��ȡ����
struct CMD_GP_GetOnline
{
	WORD							wServerCount;						//������Ŀ
	WORD							wOnLineServerID[MAX_SERVER];		//�����ʶ
};

//��������
struct CMD_GP_KindOnline
{
	WORD							wKindCount;							//������Ŀ
	tagOnLineInfoKind				OnLineInfoKind[MAX_KIND];			//��������
};

//��������
struct CMD_GP_ServerOnline
{
	WORD							wServerCount;						//������Ŀ
	tagOnLineInfoServer				OnLineInfoServer[MAX_SERVER];		//��������
};

//////////////////////////////////////////////////////////////////////////////////
//��������

#define MDM_GP_USER_SERVICE			3									//�û�����

//�˺ŷ���
#define SUB_GP_MODIFY_MACHINE		100									//�޸Ļ���
#define SUB_GP_MODIFY_LOGON_PASS	101									//�޸�����
#define SUB_GP_MODIFY_INSURE_PASS	102									//�޸�����
#define SUB_GP_MODIFY_UNDER_WRITE	103									//�޸�ǩ��

//�޸�ͷ��
#define SUB_GP_USER_FACE_INFO		200									//ͷ����Ϣ
#define SUB_GP_SYSTEM_FACE_INFO		201									//ϵͳͷ��
#define SUB_GP_CUSTOM_FACE_INFO		202									//�Զ�ͷ��

//��������
#define SUB_GP_USER_INDIVIDUAL		301									//��������
#define	SUB_GP_QUERY_INDIVIDUAL		302									//��ѯ��Ϣ
#define SUB_GP_MODIFY_INDIVIDUAL	303									//�޸�����

//���з���
#define SUB_GP_USER_SAVE_SCORE		400									//������
#define SUB_GP_USER_TAKE_SCORE		401									//ȡ�����
#define SUB_GP_USER_INSURE_INFO		402									//��������
#define SUB_GP_QUERY_INSURE_INFO	403									//��ѯ����
#define SUB_GP_USER_INSURE_SUCCESS	404									//���гɹ�
#define SUB_GP_USER_INSURE_FAILURE	405									//����ʧ��

//�������
#define SUB_GP_OPERATE_SUCCESS		900									//�����ɹ�
#define SUB_GP_OPERATE_FAILURE		901									//����ʧ��

//////////////////////////////////////////////////////////////////////////////////

//�޸�����
struct CMD_GP_ModifyLogonPass
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
};

//�޸�����
struct CMD_GP_ModifyInsurePass
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
};

//�޸�ǩ��
struct CMD_GP_ModifyUnderWrite
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��
};

//////////////////////////////////////////////////////////////////////////////////

//�û�ͷ��
struct CMD_GP_UserFaceInfo
{
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwCustomID;							//�Զ���ʶ
};

//�޸�ͷ��
struct CMD_GP_SystemFaceInfo
{
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�޸�ͷ��
struct CMD_GP_CustomFaceInfo
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	DWORD							dwCustomFace[FACE_CX*FACE_CY];		//ͼƬ��Ϣ
};

//////////////////////////////////////////////////////////////////////////////////

//�󶨻���
struct CMD_GP_ModifyMachine
{
	BYTE							cbBind;								//�󶨱�־
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//////////////////////////////////////////////////////////////////////////////////

//��������
struct CMD_GP_UserIndividual
{
	DWORD							dwUserID;							//�û� I D
};

//��ѯ��Ϣ
struct CMD_GP_QueryIndividual
{
	DWORD							dwUserID;							//�û� I D
};

//�޸�����
struct CMD_GP_ModifyIndividual
{
	BYTE							cbGender;							//�û��Ա�
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
};

//////////////////////////////////////////////////////////////////////////////////
//Я����Ϣ CMD_GP_UserIndividual

#define DTP_GP_UI_NICKNAME			1									//�û��ǳ�
#define DTP_GP_UI_USER_NOTE			2									//�û�˵��
#define DTP_GP_UI_UNDER_WRITE		3									//����ǩ��
#define DTP_GP_UI_QQ				4									//Q Q ����
#define DTP_GP_UI_EMAIL				5									//�����ʼ�
#define DTP_GP_UI_SEAT_PHONE		6									//�̶��绰
#define DTP_GP_UI_MOBILE_PHONE		7									//�ƶ��绰
#define DTP_GP_UI_COMPELLATION		8									//��ʵ����
#define DTP_GP_UI_DWELLING_PLACE	9									//��ϵ��ַ

//////////////////////////////////////////////////////////////////////////////////

//��������
struct CMD_GP_UserInsureInfo
{
	WORD							wServerID;							//�����ʶ
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
};

//������
struct CMD_GP_UserSaveScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lSaveScore;							//������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��ȡ���
struct CMD_GP_UserTakeScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lTakeScore;							//��ȡ���
	TCHAR							szPassword[LEN_MD5];				//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//���гɹ�
struct CMD_GP_UserInsureSuccess
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
	TCHAR							szDescribeString[128];				//������Ϣ
};

//����ʧ��
struct CMD_GP_UserInsureFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��ȡ���
struct CMD_GP_UserTakeResult
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
};

//��ѯ����
struct CMD_GP_QueryInsureInfo
{
	DWORD							dwUserID;							//�û� I D
};

//////////////////////////////////////////////////////////////////////////////////

//����ʧ��
struct CMD_GP_OperateFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//�����ɹ�
struct CMD_GP_OperateSuccess
{
	LONG							lResultCode;						//��������
	TCHAR							szDescribeString[128];				//�ɹ���Ϣ
};

//////////////////////////////////////////////////////////////////////////////////
//Զ�̷���

#define MDM_GP_REMOTE_SERVICE		4									//Զ�̷���

//���ҷ���
#define SUB_GP_C_SEARCH_DATABASE	100									//���ݲ���
#define SUB_GP_C_SEARCH_CORRESPOND	101									//Э������

//���ҷ���
#define SUB_GP_S_SEARCH_DATABASE	200									//���ݲ���
#define SUB_GP_S_SEARCH_CORRESPOND	201									//Э������

//////////////////////////////////////////////////////////////////////////////////

//Э������
struct CMD_GP_C_SearchCorrespond
{
	DWORD							dwGameID;							//��Ϸ��ʶ
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
};

//Э������
struct CMD_GP_S_SearchCorrespond
{
	WORD							wUserCount;							//�û���Ŀ
	tagUserRemoteInfo				UserRemoteInfo[16];					//�û���Ϣ
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//��¼����
#define MDM_MB_LOGON				100									//�㳡��¼

//��¼ģʽ
#define SUB_MB_LOGON_GAMEID			1									//I D ��¼
#define SUB_MB_LOGON_ACCOUNTS		2									//�ʺŵ�¼
#define SUB_MB_REGISTER_ACCOUNTS	3									//ע���ʺ�

//��¼���
#define SUB_MB_LOGON_SUCCESS		100									//��¼�ɹ�
#define SUB_MB_LOGON_FAILURE		101									//��¼ʧ��

//������ʾ
#define SUB_MB_UPDATE_NOTIFY		200									//������ʾ

//////////////////////////////////////////////////////////////////////////////////

//I D ��¼
struct CMD_MB_LogonGameID
{
	//ϵͳ��Ϣ
	WORD							wModuleID;							//ģ���ʶ
	DWORD							dwPlazaVersion;						//�㳡�汾

	//��¼��Ϣ
	DWORD							dwGameID;							//��Ϸ I D
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
};

//�ʺŵ�¼
struct CMD_MB_LogonAccounts
{
	//ϵͳ��Ϣ
	WORD							wModuleID;							//ģ���ʶ
	DWORD							dwPlazaVersion;						//�㳡�汾

	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
};

//ע���ʺ�
struct CMD_MB_RegisterAccounts
{
	//ϵͳ��Ϣ
	WORD							wModuleID;							//ģ���ʶ
	DWORD							dwPlazaVersion;						//�㳡�汾

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szInsurePass[LEN_MD5];				//��������

	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
};

//��¼�ɹ�
struct CMD_MB_LogonSuccess
{
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwExperience;						//������ֵ
	DWORD							dwLoveLiness;						//�û�����
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
};

//��¼ʧ��
struct CMD_MB_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//������ʾ
struct CMD_MB_UpdateNotify
{
	BYTE							cbMustUpdate;						//ǿ������
	BYTE							cbAdviceUpdate;						//��������
	DWORD							dwCurrentVersion;					//��ǰ�汾
};

//////////////////////////////////////////////////////////////////////////////////
//�б�����

#define MDM_MB_SERVER_LIST			101									//�б���Ϣ

//�б���Ϣ
#define SUB_MB_LIST_KIND			100									//�����б�
#define SUB_MB_LIST_SERVER			101									//�����б�
#define SUB_MB_LIST_FINISH			200									//�б����

//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif