#ifndef CMD_PLAZA_HEAD_FILE
#define CMD_PLAZA_HEAD_FILE

//////////////////////////////////////////////////////////////////////////

//�㳡�汾
#define VER_PLAZA_LOW					1								//�㳡�汾
#define VER_PLAZA_HIGH					2								//�㳡�汾
#define VER_PLAZA_FRAME					MAKELONG(VER_PLAZA_LOW,VER_PLAZA_HIGH)

//////////////////////////////////////////////////////////////////////////
//��¼�����ʶ

#define ERC_GP_LOGON_SUCCESS			0								//��½�ɹ�
#define ERC_GP_ACCOUNTS_NOT_EXIST		1								//�ʺŲ�����
#define ERC_GP_LONG_NULLITY				2								//��ֹ��¼
#define ERC_GP_PASSWORD_ERCOR			3								//�������

//////////////////////////////////////////////////////////////////////////
//��¼������

#define MDM_GP_LOGON					1								//�㳡��¼

#define SUB_GP_LOGON_ACCOUNTS			1								//�ʺŵ�¼
#define SUB_GP_LOGON_USERID				2								//I D ��¼
#define SUB_GP_REGISTER_ACCOUNTS		3								//ע���ʺ�
#define SUB_GP_UPLOAD_CUSTOM_FACE		4								//����ͷ��

#define SUB_GP_LOGON_SUCCESS			100								//��½�ɹ�
#define SUB_GP_LOGON_ERROR				101								//��½ʧ��
#define SUB_GP_LOGON_FINISH				102								//��½���

//�ʺŵ�¼
struct CMD_GP_LogonByAccounts
{
	DWORD								dwPlazaVersion;					//�㳡�汾
	TCHAR								szAccounts[NAME_LEN];			//��¼�ʺ�
	TCHAR								szPassWord[PASS_LEN];			//��¼����
};

//I D ��¼
struct CMD_GP_LogonByUserID
{
	DWORD								dwPlazaVersion;					//�㳡�汾
	DWORD								dwUserID;						//�û� I D
	TCHAR								szPassWord[PASS_LEN];			//��¼����
};

//ע���ʺ�
struct CMD_GP_RegisterAccounts
{
	WORD								wFaceID;						//ͷ���ʶ
	BYTE								cbGender;						//�û��Ա�
	DWORD								dwPlazaVersion;					//�㳡�汾
	TCHAR								szSpreader[NAME_LEN];			//�ƹ�����
	TCHAR								szAccounts[NAME_LEN];			//��¼�ʺ�
	TCHAR								szPassWord[PASS_LEN];			//��¼����
	TCHAR								szPassWordBank[PASS_LEN];		//��¼����
};

//��½�ɹ�
struct CMD_GP_LogonSuccess
{
	WORD								wFaceID;						//ͷ������
	BYTE								cbGender;						//�û��Ա�
	BYTE								cbMember;						//��Ա�ȼ�
	DWORD								dwUserID;						//�û� I D
	DWORD								dwGameID;						//��Ϸ I D
	DWORD								dwExperience;					//�û�����
	
	
	//��չ��Ϣ
	DWORD								dwCustomFaceVer;				//ͷ��汾
};

//��½ʧ��
struct CMD_GP_LogonError
{
	LONG								lErrorCode;						//�������
	TCHAR								szErrorDescribe[128];			//������Ϣ
};

//////////////////////////////////////////////////////////////////////////
//��Ϸ�б�������

#define MDM_GP_SERVER_LIST				2								//�б���Ϣ

#define SUB_GP_LIST_TYPE				100								//�����б�
#define SUB_GP_LIST_KIND				101								//�����б�
#define SUB_GP_LIST_STATION				102								//վ���б�
#define SUB_GP_LIST_SERVER				103								//�����б�
#define SUB_GP_LIST_FINISH				104								//�������
#define SUB_GP_LIST_CONFIG				105								//�б�����

//�б�����
struct CMD_GP_ListConfig
{
	BYTE								bShowOnLineCount;				//��ʾ����
};

//////////////////////////////////////////////////////////////////////////
//ϵͳ������

#define MDM_GP_SYSTEM					3								//ϵͳ����

#define SUB_GP_VERSION					100								//�汾֪ͨ
#define SUB_SP_SYSTEM_MSG				101								//ϵͳ��Ϣ

//�汾֪ͨ
struct CMD_GP_Version
{
	BYTE								bNewVersion;					//���°汾
	BYTE								bAllowConnect;					//��������
};

//////////////////////////////////////////////////////////////////////////

#define MDM_GP_USER						4								//�û���Ϣ

#define SUB_GP_USER_UPLOAD_FACE			100								//�ϴ�ͷ��
#define SUB_GP_USER_DOWNLOAD_FACE		101								//����ͷ��
#define SUB_GP_UPLOAD_FACE_RESULT		102								//�ϴ����
#define SUB_GP_DELETE_FACE_RESULT		103								//ɾ�����
#define SUB_GP_CUSTOM_FACE_DELETE		104								//ɾ��ͷ��
#define SUB_GP_MODIFY_INDIVIDUAL		105								//��������
#define SUB_GP_MODIFY_INDIVIDUAL_RESULT	106								//�޸Ľ��

//��������
struct CMD_GP_ModifyIndividual
{
	DWORD							dwUserID;							//��� ID
	TCHAR							szNickname[NAME_LEN];				//����ǳ�
	int								nGender;							//����Ա�
	int								nAge;								//�������
	TCHAR							szAddress[64];						//��ҵ�ַ
	TCHAR							szUnderWrite[32];					//����ǩ��
	TCHAR							szPassword[33];						//�������
};

//����ͷ��
struct CMD_GP_UploadCustomFace
{
	DWORD								dwUserID;						//��� ID
	WORD								wCurrentSize;					//��ǰ��С
	bool								bLastPacket;					//����ʶ
	bool								bFirstPacket;					//��һ����ʶ
	BYTE								bFaceData[2048];				//ͷ������
};

//���سɹ�
struct CMD_GP_DownloadFaceSuccess
{
	DWORD							dwToltalSize;						//�ܹ���С
	DWORD							dwCurrentSize;						//��ǰ��С
	DWORD							dwUserID;							//��� ID
	BYTE							bFaceData[2048];					//ͷ������
};

//����ͷ��
struct CMD_GP_DownloadFace
{
	DWORD							dwUserID;							//��� ID
};

//�ϴ����
struct CMD_GP_UploadFaceResult
{
	TCHAR							szDescribeMsg[128];					//������Ϣ
	DWORD							dwFaceVer;							//ͷ��汾
	bool							bOperateSuccess;					//�ɹ���ʶ
};

//ɾ�����
struct CMD_GP_DeleteFaceResult
{
	TCHAR							szDescribeMsg[128];					//������Ϣ
	DWORD							dwFaceVer;							//ͷ��汾
	bool							bOperateSuccess;					//�ɹ���ʶ
};

//ɾ����Ϣ
struct CMD_GP_CustomFaceDelete
{
	DWORD							dwUserID;							//��� ID
	DWORD							dwFaceVer;							//ͷ��汾
};

//�޸Ľ��
struct CMD_GP_ModifyIndividualResult
{
	TCHAR							szDescribeMsg[128];					//������Ϣ
	bool							bOperateSuccess;					//�ɹ���ʶ
};

//////////////////////////////////////////////////////////////////////////

#endif