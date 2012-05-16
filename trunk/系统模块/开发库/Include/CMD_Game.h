#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//��¼���ݰ�����

#define MDM_GR_LOGON				1									//�����¼

#define SUB_GR_LOGON_ACCOUNTS		1									//�ʻ���¼
#define SUB_GR_LOGON_USERID			2									//I D ��¼

#define SUB_GR_LOGON_SUCCESS		100									//��¼�ɹ�
#define SUB_GR_LOGON_ERROR			101									//��¼ʧ��
#define SUB_GR_LOGON_FINISH			102									//��¼���

//�����ʺŵ�¼
struct CMD_GR_LogonByAccounts
{
	DWORD							dwPlazaVersion;						//�㳡�汾
	DWORD							dwProcessVersion;					//���̰汾
	TCHAR							szAccounts[NAME_LEN];				//��¼�ʺ�
	TCHAR							szPassWord[PASS_LEN];				//��¼����
};

//���� ID ��¼
struct CMD_GR_LogonByUserID
{
	DWORD							dwPlazaVersion;						//�㳡�汾
	DWORD							dwProcessVersion;					//���̰汾
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassWord[PASS_LEN];				//��¼����
};

//��¼�ɹ���Ϣ
struct CMD_GR_LogonSuccess
{
	DWORD							dwUserID;							//�û� I D
};

//��¼ʧ��
struct CMD_GR_LogonError
{
	LONG							lErrorCode;							//�������
	TCHAR							szErrorDescribe[128];				//������Ϣ
};

//////////////////////////////////////////////////////////////////////////
//�û����ݰ�����

#define MDM_GR_USER					2									//�û���Ϣ

#define SUB_GR_USER_SIT_REQ			1									//��������
#define SUB_GR_USER_LOOKON_REQ		2									//�Թ�����
#define SUB_GR_USER_STANDUP_REQ		3									//��������
#define SUB_GR_USER_LEFT_GAME_REQ	4									//�뿪��Ϸ

#define SUB_GR_USER_COME			100									//�û�����
#define SUB_GR_USER_STATUS			101									//�û�״̬
#define SUB_GR_USER_SCORE			102									//�û�����
#define SUB_GR_SIT_FAILED			103									//����ʧ��
#define SUB_GR_USER_RIGHT			104									//�û�Ȩ��
#define SUB_GR_MEMBER_ORDER			105									//��Ա�ȼ�

#define SUB_GR_USER_CHAT			200									//������Ϣ
#define SUB_GR_USER_WISPER			201									//˽����Ϣ
#define SUB_GR_USER_RULE			202									//�û�����

#define SUB_GR_USER_INVITE			300									//������Ϣ
#define SUB_GR_USER_INVITE_REQ		301									//��������

//��Ա�ȼ�
struct CMD_GR_MemberOrder
{
	DWORD							dwUserID;							//���ݿ� ID
	BYTE							cbMemberOrder;						//��Ա�ȼ�
};

//�û�Ȩ��
struct CMD_GR_UserRight
{
	DWORD							dwUserID;							//���ݿ� ID
	DWORD							dwUserRight;						//�û�Ȩ��
};

//�û�״̬
struct CMD_GR_UserStatus
{
	DWORD							dwUserID;							//���ݿ� ID
	WORD							wTableID;							//����λ��
	WORD							wChairID;							//����λ��
	BYTE							cbUserStatus;						//�û�״̬
};

//�û�����
struct CMD_GR_UserScore
{
	LONG							lLoveliness;						//�û�����
	//LONG							lInsureScore;						//���ѽ��
	//LONG							lGameGold;							//��Ϸ���
	DWORD							dwUserID;							//�û� I D
	tagUserScore					UserScore;							//������Ϣ
};

//��������
struct CMD_GR_UserSitReq
{
	WORD							wTableID;							//����λ��
	WORD							wChairID;							//����λ��
	BYTE							cbPassLen;							//���볤��
	TCHAR							szTablePass[PASS_LEN];				//��������
};

//�����û� 
struct CMD_GR_UserInviteReq
{
	WORD							wTableID;							//���Ӻ���
	DWORD							dwUserID;							//�û� I D
};

//����ʧ��
struct CMD_GR_SitFailed
{
	TCHAR							szFailedDescribe[256];				//��������
};

//����ṹ 
struct CMD_GR_UserChat
{
	WORD							wChatLength;						//��Ϣ����
	COLORREF						crFontColor;						//��Ϣ��ɫ
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szChatMessage[MAX_CHAT_LEN];		//������Ϣ
};

//˽��ṹ 
struct CMD_GR_Wisper
{
	WORD							wChatLength;						//��Ϣ����
	COLORREF						crFontColor;						//��Ϣ��ɫ
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szChatMessage[MAX_CHAT_LEN];		//������Ϣ
};

//�û�����
struct CMD_GR_UserRule
{
	bool							bPassword;							//��������
	bool							bLimitWin;							//����ʤ��
	bool							bLimitFlee;							//���ƶ���
	bool							bLimitScore;						//���Ʒ���
	bool							bCheckSameIP;						//Ч���ַ
	WORD							wWinRate;							//����ʤ��
	WORD							wFleeRate;							//��������
	LONG							lMaxScore;							//��߷��� 
	LONG							lLessScore;							//��ͷ���
	TCHAR							szPassword[PASS_LEN];				//��������
};

//�����û� 
struct CMD_GR_UserInvite
{
	WORD							wTableID;							//���Ӻ���
	DWORD							dwUserID;							//�û� I D
};

//////////////////////////////////////////////////////////////////////////
//������Ϣ���ݰ�

#define MDM_GR_INFO					3									//������Ϣ

#define SUB_GR_SERVER_INFO			100									//��������
#define SUB_GR_ORDER_INFO			101									//�ȼ�����
#define SUB_GR_MEMBER_INFO			102									//��Ա����
#define SUB_GR_COLUMN_INFO			103									//�б�����
#define SUB_GR_CONFIG_FINISH		104									//�������

//��Ϸ������Ϣ
struct CMD_GR_ServerInfo
{
	//��������
	WORD							wKindID;							//���� I D
	WORD							wTableCount;						//������Ŀ
	WORD							wChairCount;						//������Ŀ
	DWORD							dwVideoAddr;						//��Ƶ��ַ

	//��չ����
	WORD							wGameGenre;							//��Ϸ����
	BYTE							cbHideUserInfo;						//������Ϣ
};

//����������Ϣ
struct CMD_GR_ScoreInfo
{
	WORD							wDescribeCount;						//������Ŀ
	WORD							wDataDescribe[16];					//���ݱ�־
};

//�ȼ������ṹ
struct tagOrderItem
{
	LONG							lScore;								//�ȼ�����
	WORD							wOrderDescribe[16];					//�ȼ�����
};

//�ȼ�������Ϣ
struct CMD_GR_OrderInfo
{
	WORD							wOrderCount;						//�ȼ���Ŀ
	tagOrderItem					OrderItem[128];						//�ȼ�����
};

//�б��������ṹ
struct tagColumnItem
{
	WORD							wColumnWidth;						//�б���
	WORD							wDataDescribe;						//�ֶ�����
	TCHAR							szColumnName[16];					//�б�����
};

//�б�������Ϣ
struct CMD_GR_ColumnInfo
{
	WORD							wColumnCount;						//�б���Ŀ
	tagColumnItem					ColumnItem[32];						//�б�����
};

//////////////////////////////////////////////////////////////////////////
//����״̬���ݰ�

#define MDM_GR_STATUS				4									//״̬��Ϣ

#define SUB_GR_TABLE_INFO			100									//������Ϣ
#define SUB_GR_TABLE_STATUS			101									//����״̬

//����״̬�ṹ
struct tagTableStatus
{
	BYTE							bTableLock;							//����״̬
	BYTE							bPlayStatus;						//��Ϸ״̬
};

//����״̬����
struct CMD_GR_TableInfo
{
	WORD							wTableCount;						//������Ŀ
	tagTableStatus					TableStatus[512];					//״̬����
};

//����״̬��Ϣ
struct CMD_GR_TableStatus
{
	WORD							wTableID;							//���Ӻ���
	BYTE							bTableLock;							//����״̬
	BYTE							bPlayStatus;						//��Ϸ״̬
};

//////////////////////////////////////////////////////////////////////////
//�������ݰ�

#define MDM_GR_MANAGER				5									//��������

#define SUB_GR_SEND_WARNING			1									//���;���
#define SUB_GR_SEND_MESSAGE			2									//������Ϣ
#define SUB_GR_LOOK_USER_IP			3									//�鿴��ַ
#define SUB_GR_KILL_USER			4									//�߳��û�
#define SUB_GR_LIMIT_ACCOUNS		5									//�����ʻ�
#define SUB_GR_SET_USER_RIGHT		6									//Ȩ������
#define SUB_GR_OPTION_SERVER		7									//��������

//���;���
struct CMD_GR_SendWarning
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szWarningMessage[MAX_CHAT_LEN];		//������Ϣ
};

//ϵͳ��Ϣ
struct CMD_GR_SendMessage
{
	BYTE							cbGame;								//��Ϸ��Ϣ
	BYTE							cbRoom;								//��Ϸ��Ϣ
	WORD							wChatLength;						//��Ϣ����
	TCHAR							szSystemMessage[MAX_CHAT_LEN];		//ϵͳ��Ϣ
};

//�鿴��ַ
struct CMD_GR_LookUserIP
{
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�߳��û�
struct CMD_GR_KillUser
{
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�����ʻ�
struct CMD_GR_LimitAccounts
{
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//Ȩ������
struct CMD_GR_SetUserRight
{
	//Ŀ���û�
	DWORD							dwTargetUserID;						//Ŀ���û�

	//�󶨱���
	BYTE							cbGameRight;						//�ʺ�Ȩ��
	BYTE							cbAccountsRight;					//�ʺ�Ȩ��

	//Ȩ�ޱ仯
	BYTE							cbLimitRoomChat;					//��������
	BYTE							cbLimitGameChat;					//��Ϸ����
	BYTE							cbLimitPlayGame;					//��ϷȨ��
	BYTE							cbLimitSendWisper;					//������Ϣ
	BYTE							cbLimitLookonGame;					//�Թ�Ȩ��
};

//���ñ�־
#define OSF_ROOM_CHAT				1									//��������
#define OSF_GAME_CHAT				2									//��Ϸ����
#define OSF_ROOM_WISPER				3									//����˽��
#define OSF_ENTER_GAME				4									//������Ϸ
#define OSF_ENTER_ROOM				5									//���뷿��
#define OSF_SHALL_CLOSE				6									//�����ر�

//��������
struct CMD_GR_OptionServer
{
	BYTE							cbOptionFlags;						//���ñ�־
	BYTE							cbOptionValue;						//���ñ�־
};

//////////////////////////////////////////////////////////////////////////
//ϵͳ���ݰ�

#define MDM_GR_SYSTEM				10									//ϵͳ��Ϣ

#define SUB_GR_MESSAGE				100									//ϵͳ��Ϣ

//��Ϣ����
#define SMT_INFO					0x0001								//��Ϣ��Ϣ
#define SMT_EJECT					0x0002								//������Ϣ
#define SMT_GLOBAL					0x0004								//ȫ����Ϣ
#define SMT_CLOSE_ROOM				0x1000								//�رշ���
#define SMT_INTERMIT_LINE			0x4000								//�ж�����

//��Ϣ���ݰ�
struct CMD_GR_Message
{
	WORD							wMessageType;						//��Ϣ����
	WORD							wMessageLength;						//��Ϣ����
	TCHAR							szContent[1024];					//��Ϣ����
};

//////////////////////////////////////////////////////////////////////////
//�������ݰ�

#define MDM_GR_SERVER_INFO			11									//������Ϣ

#define SUB_GR_ONLINE_COUNT_INFO	100									//������Ϣ

//������Ϣ
struct tagOnLineCountInfo
{
	WORD							wKindID;							//���ͱ�ʶ
	DWORD							dwOnLineCount;						//��������
};

//////////////////////////////////////////////////////////////////////////

#endif