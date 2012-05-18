#ifndef GAME_SERVICE_HEAD_HEAD_FILE
#define GAME_SERVICE_HEAD_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////

//ƽ̨����
#include "..\..\ȫ�ֶ���\Platform.h"

//��Ϣ����
#include "..\..\��Ϣ����\CMD_Commom.h"
#include "..\..\��Ϣ����\CMD_Correspond.h"
#include "..\..\��Ϣ����\CMD_GameServer.h"

//ƽ̨�ļ�
#include "..\..\�������\�������\ServiceCoreHead.h"
#include "..\..\���������\�ں�����\KernelEngineHead.h"

//////////////////////////////////////////////////////////////////////////////////
//��������

//��������
#ifndef GAME_SERVICE_CLASS
	#ifdef  GAME_SERVICE_DLL
		#define GAME_SERVICE_CLASS _declspec(dllexport)
	#else
		#define GAME_SERVICE_CLASS _declspec(dllimport)
	#endif
#endif

//ģ�鶨��
#ifndef _DEBUG
	#define GAME_SERVICE_DLL_NAME	TEXT("GameService.dll")				//��� DLL ����
#else
	#define GAME_SERVICE_DLL_NAME	TEXT("GameServiceD.dll")			//��� DLL ����
#endif

//////////////////////////////////////////////////////////////////////////////////
//�ӿ�˵��

//�û��ӿ�
interface IServerUserItem;
interface IServerUserManager;
interface IServerUserItemSink;

//����ӿ�
interface IAndroidUserItem;
interface IAndroidUserManager;
interface IAndroidUserItemSink;

//���ӽӿ�
interface ITableFrame;
interface ITableFrameSink;
interface ITableUserAction;

//����ӿ�
interface IMainServiceFrame;
interface IGameServiceManager;
interface IGameServiceCustomRule;

//////////////////////////////////////////////////////////////////////////////////
//��������

//Ⱥ������
#define BG_MOBILE					(BYTE)(0x01)						//�ֻ�Ⱥ��
#define BG_COMPUTER					(BYTE)(0x02)						//����Ⱥ��
#define BG_ALL_CLIENT				(BYTE)(0xFF)						//ȫ��Ⱥ��

//��������
#define INDEX_SOCKET				(WORD)(0x0000)						//��������
#define INDEX_ANDROID				(WORD)(0x2000)						//��������

//��������
#define GAME_SERVICE_CREATE_NAME	"CreateGameServiceManager"			//��������

//////////////////////////////////////////////////////////////////////////////////
//��������

//����ԭ��
#define GER_NORMAL					0x00								//�������
#define GER_DISMISS					0x01								//��Ϸ��ɢ
#define GER_USER_LEAVE				0x02								//�û��뿪
#define GER_NETWORK_ERROR			0x03								//�������

//�뿪ԭ��
#define LER_NORMAL					0x00								//�����뿪
#define LER_SYSTEM					0x01								//ϵͳԭ��
#define LER_NETWORK					0x02								//����ԭ��
#define LER_USER_IMPACT				0x03								//�û���ͻ
#define LER_SERVER_FULL				0x04								//����Ϊ��
#define LER_SERVER_CONDITIONS		0x05								//��������

//��������
#define SCORE_TYPE_NULL				0x00								//��Ч����
#define SCORE_TYPE_WIN				0x01								//ʤ�ֻ���
#define SCORE_TYPE_LOSE				0x02								//��ֻ���
#define SCORE_TYPE_DRAW				0x03								//�;ֻ���
#define SCORE_TYPE_FLEE				0x04								//�Ӿֻ���
#define SCORE_TYPE_PRESENT			0x10								//���ͻ���
#define SCORE_TYPE_SERVICE			0x11								//�������

//��ʼģʽ
#define START_MODE_ALL_READY		0x00								//����׼��
#define START_MODE_FULL_READY		0x01								//���˿�ʼ
#define START_MODE_PAIR_READY		0x02								//��Կ�ʼ
#define START_MODE_TIME_CONTROL		0x10								//ʱ�����
#define START_MODE_MASTER_CONTROL	0x11								//�������

//����ѡ��
#define DISTRIBUTE_ALLOW			0x01								//�������
#define DISTRIBUTE_IMMEDIATE		0x02								//����ѡ��
#define DISTRIBUTE_LAST_TABLE		0x04								//ͬ��ѡ��
#define DISTRIBUTE_SAME_ADDRESS		0x08								//��ַѡ��

//////////////////////////////////////////////////////////////////////////////////
//ʱ���ʶ

//���ȷ�Χ
#define IDI_MAIN_MODULE_START		1									//��ʼ��ʶ
#define IDI_MAIN_MODULE_FINISH		99									//��ֹ��ʶ

//������Χ
#define IDI_REBOT_MODULE_START		100									//��ʼ��ʶ
#define IDI_REBOT_MODULE_FINISH		199									//��ֹ��ʶ

//���ӷ�Χ
#define IDI_TABLE_MODULE_START		10000								//��ʼ��ʶ
#define IDI_TABLE_MODULE_FINISH		50000								//��ֹ��ʶ

//////////////////////////////////////////////////////////////////////////////////
//ʱ�䷶Χ

//��Ϸʱ��
#define TIME_TABLE_SINK_RANGE		20									//��ʶ��Χ
#define TIME_TABLE_KERNEL_RANGE		30									//��ʶ��Χ
#define TIME_TABLE_MODULE_RANGE		50									//��ʶ��Χ

//////////////////////////////////////////////////////////////////////////////////
//��������

//��������
#define RESERVE_USER_COUNT			40L									//������Ŀ

//ϵͳʱ��
#define TIME_DISTRIBUTE_ANDROID		15L									//�����û�
#define TIME_REPORT_SERVER_INFO		60L									//�ϱ�ʱ��

//����ʱ��
#define TIME_LOAD_SYSTEM_MESSAGE	900L								//ϵͳ��Ϣ
#define TIME_SEND_SYSTEM_MESSAGE	300L								//ϵͳ��Ϣ

//����ʱ��
#define TIME_ANDROID_INOUT			5L									//��¼ʱ��
#define TIME_ANDROID_PULSE			1L									//����ʱ��
#define TIME_LOAD_ANDROID_INFO		900L								//�����û�
#define TIME_ANDROID_REPOSE_TIME	1800L								//����ʱ��

//////////////////////////////////////////////////////////////////////////////////
//�ֻ�����

//��ͼģʽ
#define	VIEW_MODE_ALL				0x0001								//ȫ������
#define	VIEW_MODE_PART				0x0002								//���ֿ���

//��Ϣģʽ
#define VIEW_INFO_LEVEL_1			0x0010								//������Ϣ
#define VIEW_INFO_LEVEL_2			0x0020								//������Ϣ
#define VIEW_INFO_LEVEL_3			0x0040								//������Ϣ
#define VIEW_INFO_LEVEL_4			0x0080								//������Ϣ

//��������
#define RECVICE_GAME_CHAT			0x0100								//��������
#define RECVICE_ROOM_CHAT			0x0200								//��������
#define RECVICE_ROOM_WHISPER		0x0400								//����˽��

//////////////////////////////////////////////////////////////////////////////////
//�ṹ����

//���ò���
struct tagGameParameter
{
	WORD							wMedalRate;							//���ƻ���
	WORD							wRevenueRate;						//����˰��
};

//��������
struct tagGameServiceAttrib
{
	//�ں�����
	WORD							wKindID;							//���ƺ���
	WORD							wChairCount;						//������Ŀ
	WORD							wSupporType;						//֧������
	TCHAR							szGameName[LEN_KIND];				//��Ϸ����

	//���ܱ�־
	BYTE							cbAndroidUser;						//������־
	BYTE							cbDynamicJoin;						//��̬����
	BYTE							cbOffLineTrustee;					//���ߴ���

	//��������
	DWORD							dwServerVersion;					//��Ϸ�汾
	DWORD							dwClientVersion;					//��Ϸ�汾
	TCHAR							szDataBaseName[32];					//��Ϸ����
	TCHAR							szServerDLLName[LEN_PROCESS];		//��������
	TCHAR							szClientEXEName[LEN_PROCESS];		//��������
};

//��������
struct tagGameServiceOption
{
	//�ҽ�����
	WORD							wKindID;							//�ҽ�����
	WORD							wNodeID;							//�ҽӽڵ�
	WORD							wSortID;							//���б�ʶ
	WORD							wServerID;							//�����ʶ

	//˰������
	BYTE							cbRevenueRatio;						//˰�ձ���

	//��������
	LONG							lCellScore;							//��λ����
	SCORE							lRestrictScore;						//���ƻ���
	SCORE							lMinTableScore;						//��ͻ���
	SCORE							lMinEnterScore;						//��ͻ���
	SCORE							lMaxEnterScore;						//��߻���

	//��Ա����
	BYTE							cbMinEnterMember;					//��ͻ�Ա
	BYTE							cbMaxEnterMember;					//��߻�Ա

	//��������
	DWORD							dwServerRule;						//�������
	DWORD							dwAttachUserRight;					//����Ȩ��

	//��������
	WORD							wMaxPlayer;							//�����Ŀ
	WORD							wTableCount;						//������Ŀ
	WORD							wServerPort;						//����˿�
	WORD							wServerType;						//��������
	TCHAR							szServerName[LEN_SERVER];			//��������

	//��������
	BYTE							cbDistributeRule;					//�������
	WORD							wMinDistributeUser;					//��������
	WORD							wMaxDistributeUser;					//�������
	WORD							wDistributeTimeSpace;				//������
	WORD							wDistributeDrawCount;				//�������
	WORD							wDistributeStartDelay;				//��ʼ��ʱ

	//��������
	TCHAR							szDataBaseAddr[16];					//���ӵ�ַ
	TCHAR							szDataBaseName[32];					//���ݿ���

	//�Զ�����
	BYTE							cbCustomRule[1024];					//�Զ�����
};

//���Ӳ���
struct tagTableFrameParameter
{
	//�ں����
	ITimerEngine *					pITimerEngine;						//ʱ������
	IDataBaseEngine *				pIRecordDataBaseEngine;				//��������
	IDataBaseEngine *				pIKernelDataBaseEngine;				//��������

	//�������
	IMainServiceFrame *				pIMainServiceFrame;					//������
	IGameServiceManager *			pIGameServiceManager;				//�������

	//���ñ���
	tagGameParameter *				pGameParameter;						//���ò���
	tagGameServiceAttrib *			pGameServiceAttrib;					//��������
	tagGameServiceOption *			pGameServiceOption;					//���ò���
};

//��������
struct tagAndroidUserParameter
{
	//���ñ���
	tagGameParameter *				pGameParameter;						//���ò���
	tagGameServiceAttrib *			pGameServiceAttrib;					//��������
	tagGameServiceOption *			pGameServiceOption;					//���ò���

	//�ں����
	ITimerEngine *					pITimerEngine;						//ʱ������
	IServerUserManager *			pIServerUserManager;				//�û�����
	IGameServiceManager *			pIGameServiceManager;				//�������
	ITCPNetworkEngineEvent *		pITCPNetworkEngineEvent;			//�¼��ӿ�
};

//////////////////////////////////////////////////////////////////////////////////
//�����ṹ

//������Ϣ
struct tagScoreInfo
{
	BYTE							cbType;								//��������
	SCORE							lScore;								//�û�����
	SCORE							lGrade;								//�û��ɼ�
	SCORE							lRevenue;							//��Ϸ˰��
};

//�����Ϣ
struct tagVariationInfo
{
	//������Ϣ
	SCORE							lScore;								//�û�����
	SCORE							lGrade;								//�û��ɼ�
	SCORE							lInsure;							//�û�����
	SCORE							lRevenue;							//��Ϸ˰��	

	//��Ϸ��Ϣ
	DWORD							dwWinCount;							//ʤ������
	DWORD							dwLostCount;						//ʧ������
	DWORD							dwDrawCount;						//�;�����
	DWORD							dwFleeCount;						//��������
	DWORD							dwPlayTimeCount;					//��Ϸʱ��

	//ȫ����Ϣ
	DWORD							dwUserMedal;						//�û�����
	DWORD							dwExperience;						//�û�����
	DWORD							dwLoveLiness;						//�û�����
};

//�û�����
struct tagUserRule
{
	//�����־
	bool							bLimitSameIP;						//Ч���ַ
	bool							bLimitWinRate;						//����ʤ��
	bool							bLimitFleeRate;						//���ƶ���
	bool							bLimitGameScore;					//���Ʒ���

	//��������
	WORD							wMinWinRate;						//���ʤ��
	WORD							wMaxFleeRate;						//�������
	LONG							lMaxGameScore;						//��߷��� 
	LONG							lMinGameScore;						//��ͷ���
	TCHAR							szPassword[LEN_PASSWORD];			//��������
};

//�û���Ϣ
struct tagUserInfoPlus
{
	//��¼��Ϣ
	DWORD							dwLogonTime;						//��¼ʱ��
	DWORD							dwInoutIndex;						//������ʶ

	//������Ϣ
	WORD							wBindIndex;							//������
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ

	//���ӱ���
	DWORD							dwUserRight;						//�û�Ȩ��
	DWORD							dwMasterRight;						//����Ȩ��
	SCORE							lRestrictScore;						//���ƻ���

	//��������
	bool							bMobileUser;						//�ֻ��û�
	bool							bAndroidUser;						//�����û�
	TCHAR							szPassword[LEN_MD5];				//��������
};

//������
struct tagRequestResult
{
	BYTE							cbFailureCode;						//ʧ��ԭ��
	TCHAR							szFailureReason[128];				//ʧ��ԭ��
};

//����״��
struct tagTableUserInfo
{
	WORD							wMinUserCount;						//��������
	WORD							wTableUserCount;					//�û���Ŀ
	WORD							wTableReadyCount;					//׼����Ŀ
	WORD							wTableAndroidCount;					//������Ŀ
};

//����״��
struct tagAndroidUserInfo
{
	WORD							wFreeUserCount;						//�û���Ŀ
	WORD							wPlayUserCount;						//�û���Ŀ
	WORD							wSitdownUserCount;					//�û���Ŀ
	IAndroidUserItem *				pIAndroidUserFree[MAX_ANDROID];		//�����ӿ�
	IAndroidUserItem *				pIAndroidUserPlay[MAX_ANDROID];		//�����ӿ�
	IAndroidUserItem *				pIAndroidUserSitdown[MAX_ANDROID];	//�����ӿ�
};

//��������
struct tagAndroidParameter
{
	DWORD							dwUserID;							//�û���ʶ
	SCORE							lMinTakeScore;						//Я������
	SCORE							lMaxTakeScore;						//Я������
	DWORD							dwMinPlayDraw;						//��Ϸ����
	DWORD							dwMaxPlayDraw;						//��Ϸ����
	DWORD							dwMinReposeTime;					//����ʱ��
	DWORD							dwMaxReposeTime;					//����ʱ��
	DWORD							dwServiceGender;					//��������
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IGameServiceManager INTERFACE_VERSION(1,1)
	static const GUID IID_IGameServiceManager={0xa975cceb,0x0331,0x4553,0xa1,0xe0,0xa7,0xc7,0x7a,0x7c,0x4e,0xfd};
#else
	#define VER_IGameServiceManager INTERFACE_VERSION(1,1)
	static const GUID IID_IGameServiceManager={0x4b2b9d8f,0xce1b,0x44f3,0xa5,0x22,0x65,0x1a,0x65,0xc9,0x0a,0x25};
#endif

//��Ϸ�ӿ�
interface IGameServiceManager : public IUnknownEx
{
	//�����ӿ�
public:
	//��������
	virtual VOID * CreateTableFrameSink(REFGUID Guid, DWORD dwQueryVer)=NULL;
	//��������
	virtual VOID * CreateAndroidUserItemSink(REFGUID Guid, DWORD dwQueryVer)=NULL;
	//��������
	virtual VOID * CreateGameDataBaseEngineSink(REFGUID Guid, DWORD dwQueryVer)=NULL;

	//�����ӿ�
public:
	//�������
	virtual bool GetServiceAttrib(tagGameServiceAttrib & GameServiceAttrib)=NULL;
	//��������
	virtual bool RectifyParameter(tagGameServiceOption & GameServiceOption)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IGameServiceCustomRule INTERFACE_VERSION(1,1)
	static const GUID IID_IGameServiceCustomRule={0x74a43b7d,0x7c29,0x4fb8,0xa5,0x00,0x8e,0x8d,0xff,0x6e,0x2a,0xdd};
#else
	#define VER_IGameServiceCustomRule INTERFACE_VERSION(1,1)
	static const GUID IID_IGameServiceCustomRule={0xc7ace01d,0x75f8,0x4af7,0xb1,0x80,0xa8,0x53,0xcd,0x2e,0x0a,0xb6};
#endif

//���ýӿ�
interface IGameServiceCustomRule : public IUnknownEx
{
	//��ȡ����
	virtual bool SaveCustomRule(LPBYTE pcbCustomRule, WORD wCustonSize)=NULL;
	//Ĭ������
	virtual bool DefaultCustomRule(LPBYTE pcbCustomRule, WORD wCustonSize)=NULL;
	//��������
	virtual HWND CreateCustomRule(CWnd * pParentWnd, CRect rcCreate, LPBYTE pcbCustomRule, WORD wCustonSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IGameDataBaseEngine INTERFACE_VERSION(1,1)
	static const GUID IID_IGameDataBaseEngine={0x47dcd531,0x2a19,0x4c0a,0x89,0xb6,0x9f,0xd8,0xe0,0xa7,0x85,0xee};
#else
	#define VER_IGameDataBaseEngine INTERFACE_VERSION(1,1)
	static const GUID IID_IGameDataBaseEngine={0x4310e733,0xc49b,0x4592,0xa3,0xb1,0x73,0x18,0xd1,0x53,0x2f,0x3e};
#endif

//��Ϸ����
interface IGameDataBaseEngine : public IUnknownEx
{
	//���ò���
public:
	//�Զ�����
	virtual VOID * GetCustomRule()=NULL;
	//��������
	virtual tagGameServiceAttrib * GetGameServiceAttrib()=NULL;
	//��������
	virtual tagGameServiceOption * GetGameServiceOption()=NULL;

	//��ȡ����
public:
	//��ȡ����
	virtual VOID * GetDataBase(REFGUID Guid, DWORD dwQueryVer)=NULL;
	//��ȡ����
	virtual VOID * GetDataBaseEngine(REFGUID Guid, DWORD dwQueryVer)=NULL;

	//���ܽӿ�
public:
	//Ͷ�ݽ��
	virtual bool PostGameDataBaseResult(WORD wRequestID, VOID * pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IGameDataBaseEngineSink INTERFACE_VERSION(1,1)
	static const GUID IID_IGameDataBaseEngineSink={0x9b9111d9,0x7a71,0x41a1,0xae,0x78,0xd4,0xf3,0x20,0x08,0x24,0xdf};
#else
	#define VER_IGameDataBaseEngineSink INTERFACE_VERSION(1,1)
	static const GUID IID_IGameDataBaseEngineSink={0xa6c5e2cc,0x34c1,0x422c,0xa0,0x1b,0x54,0xab,0x68,0xfa,0xe6,0x81};
#endif

//���ݽӿ�
interface IGameDataBaseEngineSink : public IUnknownEx
{
	//���ýӿ�
	virtual bool InitializeSink(IUnknownEx * pIUnknownEx)=NULL;
	//���ݴ���
	virtual bool OnGameDataBaseRequest(DWORD dwUserID, WORD wTableID, WORD wChairID);
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IServerUserItem INTERFACE_VERSION(1,1)
	static const GUID IID_IServerUserItem={0xcd43dce8,0x1e12,0x43be,0x8b,0x4f,0x94,0x95,0x92,0xa4,0xf6,0x19};
#else
	#define VER_IServerUserItem INTERFACE_VERSION(1,1)
	static const GUID IID_IServerUserItem={0xb5ce01a7,0x5cd1,0x4788,0x94,0x6c,0xa1,0xef,0x5b,0x30,0x2c,0xb7};
#endif

//�û��ӿ�
interface IServerUserItem : public IUnknownEx
{
	//������Ϣ
public:
	//�û�����
	virtual WORD GetBindIndex()=NULL;
	//�û���ַ
	virtual DWORD GetClientAddr()=NULL;
	//������ʶ
	virtual LPCTSTR GetMachineID()=NULL;

	//��¼��Ϣ
public:
	//��¼ʱ��
	virtual DWORD GetLogonTime()=NULL;
	//��¼����
	virtual DWORD GetInoutIndex()=NULL;

	//�û���Ϣ
public:
	//�û���Ϣ
	virtual tagUserInfo * GetUserInfo()=NULL;
	//�û�����
	virtual tagUserRule * GetUserRule()=NULL;

	//������Ϣ
public:
	//�û��Ա�
	virtual BYTE GetGender()=NULL;
	//�û���ʶ
	virtual DWORD GetUserID()=NULL;
	//��Ϸ��ʶ
	virtual DWORD GetGameID()=NULL;
	//�û��ǳ�
	virtual LPCTSTR GetNickName()=NULL;

	//״̬�ӿ�
public:
	//���Ӻ���
	virtual WORD GetTableID()=NULL;
	//���Ӻ���
	virtual WORD GetChairID()=NULL;
	//�û�״̬
	virtual BYTE GetUserStatus()=NULL;

	//Ȩ����Ϣ
public:
	//�û�Ȩ��
	virtual DWORD GetUserRight()=NULL;
	//����Ȩ��
	virtual DWORD GetMasterRight()=NULL;

	//Ȩ����Ϣ
public:
	//�û�Ȩ��
	virtual BYTE GetMemberOrder()=NULL;
	//����Ȩ��
	virtual BYTE GetMasterOrder()=NULL;

	//������Ϣ
public:
	//�û�����
	virtual SCORE GetUserScore()=NULL;
	//�û��ɼ�
	virtual SCORE GetUserGrade()=NULL;
	//�û�����
	virtual SCORE GetUserInsure()=NULL;

	//�й���Ϣ
public:
	//�йܻ���
	virtual SCORE GetTrusteeScore()=NULL;
	//��������
	virtual SCORE GetFrozenedScore()=NULL;

	//������Ϣ
public:
	//�û�ʤ��
	virtual WORD GetUserWinRate()=NULL;
	//�û�����
	virtual WORD GetUserLostRate()=NULL;
	//�û�����
	virtual WORD GetUserDrawRate()=NULL;
	//�û�����
	virtual WORD GetUserFleeRate()=NULL;
	//��Ϸ����
	virtual DWORD GetUserPlayCount()=NULL;

	//Ч��ӿ�
public:
	//�Ա��ʺ�
	virtual bool ContrastNickName(LPCTSTR pszNickName)=NULL;
	//�Ա�����
	virtual bool ContrastLogonPass(LPCTSTR pszPassword)=NULL;

	//��Ϸ״̬
public:
	//����״̬
	virtual bool IsClientReady()=NULL;
	//��������
	virtual VOID SetClientReady(bool bClientReady)=NULL;

	//�ֻ��û�
public:
	//����״̬
	virtual bool IsMobileUser()=NULL;
	//���ÿ���
	virtual VOID SetMobileUser(bool bMobileUser)=NULL;

	//�����û�
public:
	//����״̬
	virtual bool IsAndroidUser()=NULL;
	//���ÿ���
	virtual VOID SetAndroidUser(bool bbMachineUser)=NULL;

	//��¼�ӿ�
public:
	//����ж�
	virtual bool IsVariation()=NULL;
	//��ѯ��¼
	virtual bool QueryRecordInfo(tagVariationInfo & UserRecordInfo)=NULL;
	//��ȡ���
	virtual bool DistillVariation(tagVariationInfo & UserVariationInfo)=NULL;

	//����ӿ�
public:
	//����״̬
	virtual bool SetUserStatus(BYTE cbUserStatus, WORD wTableID, WORD wChairID)=NULL;
	//д�����
    virtual bool WriteUserScore(SCORE lScore, SCORE lGrade, SCORE lRevenue, DWORD dwUserMedal, BYTE cbScoreType, DWORD dwPlayTimeCount)=NULL;

	//����ӿ�
public:
	//�������
	virtual bool FrozenedUserScore(SCORE lScore)=NULL;
	//�ⶳ����
	virtual bool UnFrozenedUserScore(SCORE lScore)=NULL;

	//�߼��ӿ�
public:
	//�����
	virtual bool DetachBindStatus()=NULL;
	//���в���
	virtual bool ModifyUserInsure(SCORE lScore, SCORE lInsure, SCORE lRevenue)=NULL;
	//���ò���
	virtual bool SetUserParameter(DWORD dwClientAddr, WORD wBindIndex, TCHAR szMachineID[LEN_MACHINE_ID], bool bAndroidUser, bool bClientReady)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IServerUserService INTERFACE_VERSION(1,1)
	static const GUID IID_IServerUserService={0x2f4e25a9,0xad87,0x4a37,0x98,0x96,0x5c,0x50,0xf9,0x91,0x05,0x31};
#else
	#define VER_IServerUserService INTERFACE_VERSION(1,1)
	static const GUID IID_IServerUserService={0xcc4e885f,0x3357,0x4d68,0xb2,0x6b,0x78,0x8f,0x4b,0x3e,0x64,0xc3};
#endif

//�û�����
interface IServerUserService : public IUnknownEx
{
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IServerUserItemSink INTERFACE_VERSION(1,1)
	static const GUID IID_IServerUserItemSink={0x415be3e4,0xd48d,0x4a77,0x94,0xb7,0xd1,0x05,0xcd,0xa3,0x82,0x81};
#else
	#define VER_IServerUserItemSink INTERFACE_VERSION(1,1)
	static const GUID IID_IServerUserItemSink={0x9d0cfe02,0x0fe9,0x4a8b,0x97,0x95,0xac,0x32,0x67,0x5a,0xf8,0xb1};
#endif

//״̬�ӿ�
interface IServerUserItemSink : public IUnknownEx
{
	//�û�����
	virtual bool OnEventUserItemScore(IServerUserItem * pIServerUserItem)=NULL;
	//�û�״̬
	virtual bool OnEventUserItemStatus(IServerUserItem * pIServerUserItem)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IServerUserManager INTERFACE_VERSION(1,1)
	static const GUID IID_IServerUserManager={0xeb413ed6,0x185b,0x4ceb,0xa8,0xbd,0x54,0x74,0x19,0x89,0x6a,0x53};
#else
	#define VER_IServerUserManager INTERFACE_VERSION(1,1)
	static const GUID IID_IServerUserManager={0x77a3c4df,0x1d95,0x48c6,0xac,0x9d,0x75,0xd7,0x6c,0x2a,0x3c,0x0e};
#endif

//�û�����
interface IServerUserManager : public IUnknownEx
{
	//���ýӿ�
public:
	//���ýӿ�
	virtual bool SetServerUserItemSink(IUnknownEx * pIUnknownEx)=NULL;

	//���ҽӿ�
public:
	//ö���û�
	virtual IServerUserItem * EnumUserItem(WORD wEnumIndex)=NULL;
	//�����û�
	virtual IServerUserItem * SearchUserItem(DWORD dwUserID)=NULL;
	//�����û�
	virtual IServerUserItem * SearchUserItem(LPCTSTR pszNickName)=NULL;

	//ͳ�ƽӿ�
public:
	//��������
	virtual DWORD GetUserItemCount()=NULL;

	//����ӿ�
public:
	//ɾ���û�
	virtual bool DeleteUserItem()=NULL;
	//ɾ���û�
	virtual bool DeleteUserItem(IServerUserItem * pIServerUserItem)=NULL;
	//�����û�
	virtual bool InsertUserItem(IServerUserItem * * pIServerUserResult, tagUserInfo & UserInfo, tagUserInfoPlus & UserInfoPlus)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ITableFrame INTERFACE_VERSION(1,1)
	static const GUID IID_ITableFrame={0x2e577d5f,0x1e01,0x44ff,0x9f,0xf4,0x01,0x16,0x23,0x1b,0x76,0x15};
#else
	#define VER_ITableFrame INTERFACE_VERSION(1,1)
	static const GUID IID_ITableFrame={0x860cd1cf,0x1a4f,0x4e35,0xb8,0x0c,0xd2,0x46,0xa4,0xef,0xfd,0xfb};
#endif

//���ӽӿ�
interface ITableFrame : public IUnknownEx
{
	//���Խӿ�
public:
	//���Ӻ���
	virtual WORD GetTableID()=NULL;
	//��Ϸ����
	virtual WORD GetChairCount()=NULL;

	//���ò���
public:
	//�Զ�����
	virtual VOID * GetCustomRule()=NULL;
	//��������
	virtual tagGameServiceAttrib * GetGameServiceAttrib()=NULL;
	//��������
	virtual tagGameServiceOption * GetGameServiceOption()=NULL;

	//���ýӿ�
public:
	//��ʼģʽ
	virtual BYTE GetStartMode()=NULL;
	//��ʼģʽ
	virtual VOID SetStartMode(BYTE cbStartMode)=NULL;

	//��Ԫ����
public:
	//��Ԫ����
	virtual LONG GetCellScore()=NULL;
	//��Ԫ����
	virtual VOID SetCellScore(LONG lCellScore)=NULL;

	//��Ϣ�ӿ�
public:
	//����״̬
	virtual bool IsTableLocked()=NULL;
	//��Ϸ״̬
	virtual bool IsGameStarted()=NULL;
	//��Ϸ״̬
	virtual bool IsDrawStarted()=NULL;
	//��Ϸ״̬
	virtual bool IsTableStarted()=NULL;

	//״̬�ӿ�
public:
	//��ȡ״̬
	virtual BYTE GetGameStatus()=NULL;
	//����״̬
	virtual VOID SetGameStatus(BYTE bGameStatus)=NULL;

	//���ƽӿ�
public:
	//��ʼ��Ϸ
	virtual bool StartGame()=NULL;
	//��ɢ��Ϸ
	virtual bool DismissGame()=NULL;
	//������Ϸ
	virtual bool ConcludeGame(BYTE cbGameStatus)=NULL;

	//д�ֽӿ�
public:
	//д�����
	virtual bool WriteUserScore(WORD wChairID, SCORE lScore, BYTE cbScoreType)=NULL;
	//д�����
	virtual bool WriteTableScore(tagScoreInfo ScoreInfoArray[], WORD wScoreCount)=NULL;

	//����ӿ�
public:
	//����˰��
	virtual SCORE CalculateRevenue(WORD wChairID, SCORE lScore)=NULL;
	//�����޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem)=NULL;

	//�û��ӿ�
public:
	//Ѱ���û�
	virtual IServerUserItem * SearchUserItem(DWORD dwUserID)=NULL;
	//��Ϸ�û�
	virtual IServerUserItem * GetTableUserItem(WORD wChairID)=NULL;
	//�Թ��û�
	virtual IServerUserItem * EnumLookonUserItem(WORD wEnumIndex)=NULL;

	//ʱ��ӿ�
public:
	//����ʱ��
	virtual bool SetGameTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter)=NULL;
	//ɾ��ʱ��
	virtual bool KillGameTimer(DWORD dwTimerID)=NULL;

	//����ӿ�
public:
	//��������
	virtual bool SendTableData(WORD wChairID, WORD wSubCmdID)=NULL;
	//��������
	virtual bool SendTableData(WORD wChairID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//��������
	virtual bool SendLookonData(WORD wChairID, WORD wSubCmdID)=NULL;
	//��������
	virtual bool SendLookonData(WORD wChairID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//��������
	virtual bool SendUserItemData(IServerUserItem * pIServerUserItem, WORD wSubCmdID)=NULL;
	//��������
	virtual bool SendUserItemData(IServerUserItem * pIServerUserItem, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;

	//���ܽӿ�
public:
	//������Ϣ
	virtual bool SendGameMessage(LPCTSTR lpszMessage, WORD wType)=NULL;
	//��Ϸ��Ϣ
	virtual bool SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType)=NULL;
	//������Ϣ
	virtual bool SendRoomMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType)=NULL;

	//���ܽӿ�
public:
	//���ͳ���
	virtual bool SendGameScene(IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ITableFrameSink INTERFACE_VERSION(1,1)
	static const GUID IID_ITableFrameSink={0x9476b154,0x8beb,0x4f7e,0xaf,0x64,0xd2,0xb1,0x1a,0xda,0x5e,0xc4};
#else
	#define VER_ITableFrameSink INTERFACE_VERSION(1,1)
	static const GUID IID_ITableFrameSink={0x38a74df5,0x6245,0x46c7,0xb6,0xce,0x53,0xf9,0xd5,0xbf,0x6d,0xe6};
#endif

//�ص��ӿ�
interface ITableFrameSink : public IUnknownEx
{
	//����ӿ�
public:
	//��λ�ӿ�
	virtual VOID RepositionSink()=NULL;
	//���ýӿ�
	virtual bool Initialization(IUnknownEx * pIUnknownEx)=NULL;

	//��ѯ�ӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem)=NULL;
	//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem)=NULL;

	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool OnEventGameStart()=NULL;
	//��Ϸ����
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)=NULL;
	//���ͳ���
	virtual bool OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)=NULL;

	//�¼��ӿ�
public:
	//ʱ���¼�
	virtual bool OnTimerMessage(DWORD dwTimerID, WPARAM dwBindParameter)=NULL;
	//�����¼�
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize)=NULL;
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)=NULL;

	//����ӿ�
public:
	//��Ϸ��Ϣ
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)=NULL;
	//�����Ϣ
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ITableUserAction INTERFACE_VERSION(1,1)
	static const GUID IID_ITableUserAction={0x0f9aa3f9,0xdba4,0x49cb,0x88,0x4f,0xd9,0x11,0xaf,0x24,0xfb,0x8d};
#else
	#define VER_ITableUserAction INTERFACE_VERSION(1,1)
	static const GUID IID_ITableUserAction={0xc97c060b,0xcf0e,0x40b7,0x93,0x30,0x97,0xa4,0xf6,0x8c,0xca,0x84};
#endif

//�û�����
interface ITableUserAction : public IUnknownEx
{
	//�û�����
	virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)=NULL;
	//�û�����
	virtual bool OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem)=NULL;
	//�û�����
	virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)=NULL;
	//�û�����
	virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)=NULL;
	//�û�ͬ��
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ITableUserRequest INTERFACE_VERSION(1,1)
	static const GUID IID_ITableUserRequest={0x7a810ebe,0x3835,0x41b5,0xba,0x7e,0x02,0x97,0x8c,0x13,0x73,0x50};
#else
	#define VER_ITableUserRequest INTERFACE_VERSION(1,1)
	static const GUID IID_ITableUserRequest={0x7ad17e89,0xcb5b,0x472a,0xac,0xeb,0x84,0x4d,0x4f,0xa1,0x4c,0x38};
#endif

//�û�����
interface ITableUserRequest : public IUnknownEx
{
	//�Թ�����
	virtual bool OnUserRequestLookon(WORD wChairID, IServerUserItem * pIServerUserItem, tagRequestResult & RequestResult)=NULL;
	//��������
	virtual bool OnUserRequestSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, tagRequestResult & RequestResult)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IMainServiceFrame INTERFACE_VERSION(1,1)
	static const GUID IID_IMainServiceFrame={0xef3efa64,0x788b,0x4299,0x80,0x99,0xdd,0xed,0x08,0xde,0x57,0xc1};
#else
	#define VER_IMainServiceFrame INTERFACE_VERSION(1,1)
	static const GUID IID_IMainServiceFrame={0xbaaf5584,0xf9b4,0x41b6,0xae,0x6b,0xef,0x4d,0x54,0x41,0xf8,0x32};
#endif

//������
interface IMainServiceFrame : public IUnknownEx
{
	//��Ϣ�ӿ�
public:
	//������Ϣ
	virtual bool SendRoomMessage(LPCTSTR lpszMessage, WORD wType)=NULL;
	//��Ϸ��Ϣ
	virtual bool SendGameMessage(LPCTSTR lpszMessage, WORD wType)=NULL;
	//������Ϣ
	virtual bool SendRoomMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType)=NULL;
	//��Ϸ��Ϣ
	virtual bool SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType)=NULL;

	//����ӿ�
public:
	//��������
	virtual bool SendData(BYTE cbSendMask, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//��������
	virtual bool SendData(DWORD dwContextID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//��������
	virtual bool SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IAndroidUserItem INTERFACE_VERSION(1,1)
	static const GUID IID_IAndroidUserItem={0xf6856fe1,0xc93e,0x4166,0xbc,0x92,0xf7,0x43,0xad,0x97,0x1c,0xa8};
#else
	#define VER_IAndroidUserItem INTERFACE_VERSION(1,1)
	static const GUID IID_IAndroidUserItem={0xb1faa2f4,0x9804,0x4c6f,0x9d,0xfc,0xb1,0x0a,0x08,0x8a,0x22,0x69};
#endif

//�����˽ӿ�
interface IAndroidUserItem : public IUnknownEx
{
	//��Ϣ�ӿ�
public:
	//��ȡ I D
	virtual DWORD GetUserID()=NULL;
	//���Ӻ���
	virtual WORD GetTableID()=NULL;
	//���Ӻ���
	virtual WORD GetChairID()=NULL;

	//״̬����
public:
	//��ȡ״̬
	virtual BYTE GetGameStatus()=NULL;
	//����״̬
	virtual VOID SetGameStatus(BYTE cbGameStatus)=NULL;

	//������Ϣ
public:
	//��ȡ����
	virtual tagAndroidParameter * GetAndroidParameter()=NULL;

	//���ܽӿ�
public:
	//��ȡ�Լ�
	virtual IServerUserItem * GetMeUserItem()=NULL;
	//��Ϸ�û�
	virtual IServerUserItem * GetTableUserItem(WORD wChariID)=NULL;

	//���нӿ�
public:
	//������
	virtual bool PerformSaveScore(SCORE lScore)=NULL;
	//��ȡ���
	virtual bool PerformTakeScore(SCORE lScore)=NULL;

	//����ӿ�
public:
	//���ͺ���
	virtual bool SendSocketData(WORD wSubCmdID)=NULL;
	//���ͺ���
	virtual bool SendSocketData(WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;

	//���ܽӿ�
public:
	//ɾ��ʱ��
	virtual bool KillGameTimer(UINT nTimerID)=NULL;
	//����ʱ��
	virtual bool SetGameTimer(UINT nTimerID, UINT nElapse)=NULL;
	//����׼��
	virtual bool SendUserReady(VOID * pData, WORD wDataSize)=NULL;
	//��������
	virtual bool SendChatMessage(DWORD dwTargetUserID, LPCTSTR pszChatString, COLORREF crFontColor)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IAndroidUserItemSink INTERFACE_VERSION(1,1)
	static const GUID IID_IAndroidUserItemSink={0x0967632c,0x93da,0x4f7f,0x98,0xe4,0x6f,0x9f,0xf2,0xca,0x7b,0xc4};
#else
	#define VER_IAndroidUserItemSink INTERFACE_VERSION(1,1)
	static const GUID IID_IAndroidUserItemSink={0x1e8a1918,0x572b,0x453b,0xbc,0x0b,0x6b,0x61,0x70,0xa3,0x3c,0xca};
#endif

//�����˽ӿ�
interface IAndroidUserItemSink : public IUnknownEx
{
	//���ƽӿ�
public:
	//���ýӿ�
	virtual bool RepositionSink()=NULL;
	//��ʼ�ӿ�
	virtual bool Initialization(IUnknownEx * pIUnknownEx)=NULL;

	//��Ϸ�¼�
public:
	//ʱ����Ϣ
	virtual bool OnEventTimer(UINT nTimerID)=NULL;
	//��Ϸ��Ϣ
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//��Ϸ��Ϣ
	virtual bool OnEventFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//������Ϣ
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize)=NULL;

	//�û��¼�
public:
	//�û�����
	virtual VOID OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)=NULL;
	//�û��뿪
	virtual VOID OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)=NULL;
	//�û�����
	virtual VOID OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)=NULL;
	//�û�״̬
	virtual VOID OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IAndroidUserManager INTERFACE_VERSION(1,1)
	static const GUID IID_IAndroidUserManager={0x0c963240,0xa798,0x4e33,0x81,0x28,0x97,0x3d,0x05,0xe7,0x7f,0x89};
#else
	#define VER_IAndroidUserManager INTERFACE_VERSION(1,1)
	static const GUID IID_IAndroidUserManager={0xba43054d,0x924b,0x4013,0xb2,0x6d,0xa6,0x91,0xb6,0x20,0x23,0xb2};
#endif

//�����˽ӿ�
interface IAndroidUserManager : public IUnknownEx
{
	//���ƽӿ�
public:
	//��������
	virtual bool StartService()=NULL;
	//ֹͣ����
	virtual bool ConcludeService()=NULL;

	//״̬�ӿ�
public:
	//�û�״��
	virtual WORD GetAndroidUserInfo(tagAndroidUserInfo & AndroidUserInfo)=NULL;

	//���ýӿ�
public:
	//�������
	virtual bool InitAndroidUser(tagAndroidUserParameter & AndroidUserParameter)=NULL;
	//���ÿ��
	virtual bool SetAndroidStock(tagAndroidParameter AndroidParameter[], WORD wStockCount)=NULL;

	//����ӿ�
public:
	//ɾ������
	virtual bool DeleteAndroidUserItem(DWORD dwAndroidID)=NULL;
	//���һ���
	virtual IAndroidUserItem * SearchAndroidUserItem(DWORD dwUserID, DWORD dwContextID)=NULL;
	//��������
	virtual IAndroidUserItem * CreateAndroidUserItem(tagAndroidParameter & AndroidParameter)=NULL;

	//�¼��ӿ�
public:
	//�����¼�
	virtual bool OnEventTimerPulse(DWORD dwTimerID, WPARAM dwBindParameter)=NULL;

	//����ӿ�
public:
	//��������
	virtual bool SendDataToClient(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//��������
	virtual bool SendDataToClient(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//��������
	virtual bool SendDataToServer(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//�����ļ�
#ifndef GAME_SERVICE_DLL
	#include "AndroidUserItem.h"
	#include "ServerUserManager.h"
	#include "AndroidUserManager.h"
	#include "GamePropertyManager.h"
#endif

//��Ϸ����
DECLARE_MODULE_DYNAMIC(GameServiceManager)
DECLARE_MODULE_DYNAMIC(AndroidUserItemSink)

//////////////////////////////////////////////////////////////////////////////////

#endif