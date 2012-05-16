#ifndef GAME_SERVICE_EXPORT_HEAD_FILE
#define GAME_SERVICE_EXPORT_HEAD_FILE

//////////////////////////////////////////////////////////////////////////

//ƽ̨ͷ�ļ�
#include "..\..\ģ���\Template.h"
#include "..\..\�����ļ�\GlobalDef.h"
#include "..\..\�����ļ�\GlobalField.h"
#include "..\..\�����ļ�\GlobalFrame.h"
#include "..\..\�����ļ�\GlobalRight.h"
#include "..\..\�����ļ�\GlobalService.h"

//�������ļ�
#include "..\..\��Ϣ����\CMD_Game.h"
#include "..\..\��Ϣ����\CMD_Video.h"
#include "..\..\��Ϣ����\CMD_Plaza.h"
#include "..\..\��Ϣ����\CMD_Center.h"

//���ͷ�ļ�
#include "..\..\���������\�ں�����\KernelEngineHead.h"
#include "..\..\�ͻ������\�ʻ�����\PropertyModule.h"

//////////////////////////////////////////////////////////////////////////
//�����궨��

//��������
#ifndef GAME_SERVICE_CLASS
	#ifdef  GAME_SERVICE_DLL
		#define GAME_SERVICE_CLASS _declspec(dllexport)
	#else
		#define GAME_SERVICE_CLASS _declspec(dllimport)
	#endif
#endif

//ģ�鶨��
#ifdef _DEBUG
	#define GAME_SERVICE_DLL_NAME	TEXT("GameServiceD.dll")				//��� DLL ����
#else
	#define GAME_SERVICE_DLL_NAME	TEXT("GameService.dll")					//��� DLL ����
#endif

//////////////////////////////////////////////////////////////////////////

//�궨��
#define IDI_MAX_TIME_ID					30									//���޶�ʱ��

//����ԭ��
#define GER_NORMAL						0x00								//�������
#define GER_DISMISS						0x01								//��Ϸ��ɢ
#define GER_USER_LEFT					0x02								//�û��뿪

//��������
#define INDEX_SOCKET					(WORD)(0x0000)						//��������
#define INDEX_ANDROID					(WORD)(0x2000)						//��������
#define INVALID_WORD_INDEX				(WORD)(0xFFFF)						//��Ч����

//////////////////////////////////////////////////////////////////////////
//����ʱ��

//ʱ�䷶Χ
#define IDI_ATTEMPER_START				1									//������ʼ
#define IDI_ATTEMPER_CONCLUDE			99									//���Ƚ���

//ʱ���ʶ
#define IDI_CONNECT_CENTER_SERVER		(IDI_ATTEMPER_START+1)				//���ӷ�����
#define IDI_UPDATE_SERVER_LIST			(IDI_ATTEMPER_START+2)				//�����б�
#define IDI_UPDATE_ONLINE_COUNT			(IDI_ATTEMPER_START+3)				//��������
#define IDI_SEND_SYSTEM_MESSAGE			(IDI_ATTEMPER_START+4)				//ϵͳ��Ϣ

//������ʶ
#define IDI_LOAD_ANDROID_INFO			(IDI_ATTEMPER_START+5)				//�û���Ϣ
#define IDI_DISTRIBUTE_ANDROID			(IDI_ATTEMPER_START+6)				//�����û�

//ʱ�䶨��
#define TIME_RECONNECT					15000L								//����ʱ��
#define TIME_UPDATE_LIST				30000L								//����ʱ��
#define TIME_UPDATE_ONLINE				30000L								//��������

//////////////////////////////////////////////////////////////////////////
//����ʱ��

//ʱ�䷶Χ
#define IDI_ANDROID_START				100									//������ʼ
#define IDI_ANDROID_CONCLUDE			199									//��������

//ʱ���ʶ
#define IDI_ANDROID_INOUT				(IDI_ANDROID_START+1)				//����ʱ��
#define IDI_ANDROID_USER_PULSE			(IDI_ANDROID_START+2)				//����ʱ��

//����ʱ��
#define IDI_ANDROID_USER_TIEM			1									//��ʼ��ʶ
#define IDI_ANDROID_ITEM_SINK			500									//��ʼ��ʶ

//����ʱ��
#define TIME_LOAD_ANDROID_INFO			180L								//�����û�
#define TIME_DISTRIBUTE_ANDROID			15L									//�����û�

//�û�ʱ��
#define TIME_ANDROID_INOUT				10L									//��¼ʱ��
#define TIME_ANDROID_SERVICE			7200L								//����ʱ��
#define TIME_ANDROID_USER_PULSE			1L									//�û�����

//////////////////////////////////////////////////////////////////////////
//����ʱ��

//ʱ�䷶Χ
#define IDI_TABLE_FRAME_START			200									//������ʼ
#define IDI_TABLE_FRAME_CONCLUDE		5000								//���ӽ���

//ʱ�䳣��
#define RANGE_TABLE_TIME				300									//��ʶ��Χ

//////////////////////////////////////////////////////////////////////////
//ö�ٶ���

//��ʼģʽ
enum enStartMode
{
	enStartMode_FullReady,				//���˿�ʼ
	enStartMode_AllReady,				//����׼��
	enStartMode_Symmetry,				//�Գƿ�ʼ
	enStartMode_TimeControl,			//ʱ�����
};

//��������
enum enScoreKind
{
	enScoreKind_Win,					//ʤ
	enScoreKind_Lost,					//��
	enScoreKind_Draw,					//��
	enScoreKind_Flee,					//��
	enScoreKind_Service,				//����
	enScoreKind_Present,				//����
};

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//������Ϣ
struct tagScoreInfo
{
	LONG								lScore;								//��Ϸ����
	LONG								lRevenue;							//��Ϸ����
	enScoreKind							ScoreKind;							//��������
};

//�û�����
struct tagUserRule
{
	bool								bPassword;							//��������
	bool								bLimitWin;							//����ʤ��
	bool								bLimitFlee;							//��������
	bool								bLimitScore;						//���Ʒ���
	bool								bCheckSameIP;						//Ч���ַ
	WORD								wWinRate;							//ʤ������
	WORD								wFleeRate;							//��������
	LONG								lMaxScore;							//��߷��� 
	LONG								lLessScore;							//��ͷ���
	TCHAR								szPassword[PASS_LEN];				//��������
};

//������Ϣ
struct tagProperty
{
	INT									nPropertyID;						//����ID
	DWORD								dwPropCount;						//������Ŀ
	DWORD								dwConsumedCount;					//ʹ����Ŀ
};

//�û���Ϣ�ṹ
struct tagServerUserData
{
	//������Ϣ
	WORD								wFaceID;							//ͷ������
	DWORD								dwCustomFaceVer;					//�Զ�ͷ��
	DWORD								dwUserID;							//�û� I D
	DWORD								dwGameID;							//��Ϸ I D
	DWORD								dwGroupID;							//��������
	DWORD								dwUserRight;						//�û��ȼ�
	LONG								lLoveliness;						//�û�����
	DWORD								dwMasterRight;						//����Ȩ��
	TCHAR								szAccounts[NAME_LEN];				//�û��ʺ�
	TCHAR								szGroupName[GROUP_LEN];				//��������
	TCHAR								szUnderWrite[UNDER_WRITE_LEN];		//����ǩ��

	//�û�����
	BYTE								cbGender;							//�û��Ա�
	BYTE								cbMemberOrder;						//��Ա�ȼ�
	BYTE								cbMasterOrder;						//����ȼ�

	//״̬��Ϣ
	WORD								wTableID;							//���Ӻ���
	WORD								wChairID;							//����λ��
	BYTE								cbUserStatus;						//�û�״̬

	//������Ϣ
	LONG								lStorageScore;						//�洢����
	tagUserScore						UserScoreInfo;						//�û�����

	//��չ��Ϣ
	tagProperty							PropertyInfo[PROPERTY_COUNT];		//������Ϣ
};

//���Ӳ����ṹ
struct tagTableFrameParameter
{
	IUnknownEx							* pIGameServiceFrame;				//������
	IUnknownEx							* pIGameServiceManager;				//�������
	const tagGameServiceAttrib			* pGameServiceAttrib;				//��������
	const tagGameServiceOption			* pGameServiceOption;				//���ò���
};

//��������ṹ
struct tagGameServiceParameter
{
	TCHAR								szModule[32];						//ģ������
	DWORD								dwCenterAddr;						//���ĵ�ַ
	tagDataBaseInfo						GameUserDBInfo;						//��������
	tagDataBaseInfo						GameScoreDBInfo;					//��������
	tagGameServiceOption				GameServiceOption;					//��������
};

//////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IServerUserItem INTERFACE_VERSION(1,1)
	static const GUID IID_IServerUserItem={0x0400256a,0x6287,0x4a82,0x92,0xbf,0x53,0xcf,0x51,0x62,0x76,0x21};
#else
	#define VER_IServerUserItem INTERFACE_VERSION(1,1)
	static const GUID IID_IServerUserItem={0xe4ce78b7,0x9758,0x4b85,0xa5,0xa9,0x75,0x2f,0x66,0x63,0x32,0x5a};
#endif

//�������û��ӿ�
interface IServerUserItem : public IUnknownEx
{
	//��Ϣ�ӿ�
public:
	//�Ƿ񼤻�
	virtual bool __cdecl IsActive()=NULL;
	//�û�����
	virtual WORD __cdecl GetUserIndex()=NULL;
	//��ȡ��ַ
	virtual DWORD __cdecl GetClientIP()=NULL;
	//��ȡ����
	virtual LPCTSTR __cdecl GetPassword()=NULL;
	//��ȡ����
	virtual const tagUserRule * __cdecl GetUserRule()=NULL;
	//��ȡ����
	virtual const tagUserScore * __cdecl GetUserScore()=NULL;
	//��ȡ��Ϣ
	virtual tagServerUserData * __cdecl GetUserData()=NULL;

	//���Խӿ�
public:
	//�û�����
	virtual LPCTSTR __cdecl GetAccounts()=NULL;
	//�û� I D
	virtual DWORD __cdecl GetUserID()=NULL;
	//���Ӻ���
	virtual WORD __cdecl GetTableID()=NULL;
	//���Ӻ���
	virtual WORD __cdecl GetChairID()=NULL;
	//�û�״̬
	virtual BYTE __cdecl GetUserStatus()=NULL;
	//�û�ʤ��
	virtual WORD __cdecl GetUserWinRate()=NULL;
	//�û�����
	virtual WORD __cdecl GetUserLostRate()=NULL;
	//�û�����
	virtual WORD __cdecl GetUserDrawRate()=NULL;
	//�û�����
	virtual WORD __cdecl GetUserFleeRate()=NULL;

	//ͳ�ƽӿ�
public:
	//��ȡ˰��
	virtual LONG __cdecl GetUserRevenue()=NULL;
	//��Ϸʱ��
	virtual DWORD __cdecl GetPlayTimeCount()=NULL;
	//����ʱ��
	virtual DWORD __cdecl GetOnlineTimeCount()=NULL;

	//���ܽӿ�
public:
	//���ù���
	virtual bool __cdecl SetUserRule(tagUserRule & UserRule)=NULL;
	//������Ϣ
	virtual bool __cdecl GetUserScoreInfo(tagUserScore & ScoreModifyInfo)=NULL;
	//�޸Ļ���
    virtual bool __cdecl WriteScore(tagScoreInfo & ScoreInfo, DWORD dwPlayTimeCount)=NULL;
	//����״̬
	virtual bool __cdecl SetUserStatus(BYTE cbUserStatus, WORD wTableID, WORD wChairID)=NULL;
	//�޸Ľ��
	virtual bool __cdecl ModifyBankStorageGold(LONG lStorageCount)=NULL;
	//������Ϣ
	virtual tagUserScore* __cdecl GetUserScoreModifyInfo()=NULL;

	//���ܽӿ�
public:
	//����״̬
	virtual bool __cdecl IsAndroidUser()=NULL;
	//���ÿ���
	virtual VOID __cdecl SetAndroidUser(bool bAndroidUser)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IServerUserManager INTERFACE_VERSION(1,1)
	static const GUID IID_IServerUserManager={0x580b63b7,0x4b4f,0x4c6d,0xb7,0x49,0x5b,0x9f,0x46,0xc6,0x36,0xee};
#else
	#define VER_IServerUserManager INTERFACE_VERSION(1,1)
	static const GUID IID_IServerUserManager={0x8614dbc0,0xee75,0x4a82,0xb8,0xe1,0xdf,0x22,0x41,0x39,0x6f,0x49};
#endif

//�������û�����ӿ�
interface IServerUserManager : public IUnknownEx
{
	//����ӿ�
public:
	//�����û�
	virtual bool __cdecl ResetUserManager()=NULL;
	//�л��û�
	virtual bool __cdecl SwitchOnLineUserItem(IServerUserItem * pIServerUserItem, DWORD dwClientIP, WORD wUserIndex)=NULL;
	//�����û�
	virtual bool __cdecl ActiveOffLineUserItem(IServerUserItem * pIServerUserItem, DWORD dwClientIP, WORD wUserIndex)=NULL;
	//�����û�
	virtual IServerUserItem * __cdecl ActiveUserItem(tagServerUserData & ServerUserData, DWORD dwClientIP, WORD wUserIndex, TCHAR szPassword[PASS_LEN],WORD wServerType)=NULL;
	//ɾ���û�
	virtual bool __cdecl DeleteUserItem(IServerUserItem * pIServerUserItem)=NULL;
	//ע�����
	virtual bool __cdecl RegOffLineUserItem(IServerUserItem * pIServerUserItem)=NULL;
	//ɾ������
	virtual bool __cdecl UnRegOffLineUserItem(IServerUserItem * pIServerUserItem)=NULL;

	//���ҽӿ�
public:
	//ö���û�
	virtual IServerUserItem * __cdecl EnumOnLineUser(WORD wEnumIndex)=NULL;
	//ö���û�
	virtual IServerUserItem * __cdecl EnumOffLineUser(WORD wEnumIndex)=NULL;
	//�����û�
	virtual IServerUserItem * __cdecl SearchOnLineUser(DWORD dwUserID)=NULL;
	//�����û�
	virtual IServerUserItem * __cdecl SearchOffLineUser(DWORD dwUserID)=NULL;

	//��Ϣ�ӿ�
public:
	//��������
	virtual DWORD __cdecl GetOnLineCount()=NULL;
	//��������
	virtual DWORD __cdecl GetOffLineCount()=NULL;
};

//////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ITableFrame INTERFACE_VERSION(1,1)
	static const GUID IID_ITableFrame={0x2b4e7113,0xdf01,0x4398,0xac,0xe0,0x51,0x7f,0x0a,0x60,0xea,0xfd};
#else
	#define VER_ITableFrame INTERFACE_VERSION(1,1)
	static const GUID IID_ITableFrame={0xe65d2d8c,0xe55a,0x4b7c,0x8d,0xde,0x43,0xcc,0x69,0xdf,0x0f,0x9f};
#endif

//��Ϸ���ӽӿ�
interface ITableFrame : public IUnknownEx
{
	//��Ϣ�ӿ�
public:
	//���Ӻ���
	virtual WORD __cdecl GetTableID()=NULL;
	//��Ϸ����
	virtual WORD __cdecl GetChairCount()=NULL;
	//��Ϸʱ��
	virtual DWORD __cdecl GetPlayTimeCount()=NULL;

	//�û��ӿ�
public:
	//ö���û�
	virtual IServerUserItem * __cdecl EnumLookonUserItem(WORD wIndex)=NULL;
	//��ȡ�û�
	virtual IServerUserItem * __cdecl GetServerUserItem(WORD wChairID)=NULL;

	//״̬�ӿ�
public:
	//��ȡ״̬
	virtual BYTE __cdecl GetGameStatus()=NULL;
	//����״̬
	virtual void __cdecl SetGameStatus(BYTE bGameStatus)=NULL;
	//��ȡ����
	virtual const tagGameServiceOption * __cdecl GetGameServiceOption()=NULL;

	//��ʱ���ӿ�
public:
	//���ö�ʱ��
	virtual bool __cdecl SetGameTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM wBindParam)=NULL;
	//ɾ����ʱ��
	virtual bool __cdecl KillGameTimer(DWORD dwTimerID)=NULL;

	//����ӿ�
public:
	//��������
	virtual bool __cdecl SendUserData(IServerUserItem * pIServerUserItem, WORD wSubCmdID)=NULL;
	//��������
	virtual bool __cdecl SendUserData(IServerUserItem * pIServerUserItem, WORD wSubCmdID, void * pData, WORD wDataSize)=NULL;
	//��������
	virtual bool __cdecl SendTableData(WORD wChairID, WORD wSubCmdID)=NULL;
	//��������
	virtual bool __cdecl SendTableData(WORD wChairID, WORD wSubCmdID, void * pData, WORD wDataSize)=NULL;
	//��������
	virtual bool __cdecl SendLookonData(WORD wChairID, WORD wSubCmdID)=NULL;
	//��������
	virtual bool __cdecl SendLookonData(WORD wChairID, WORD wSubCmdID, void * pData, WORD wDataSize)=NULL;

	//��Ϣ�ӿ�
public:
	//���ͷ�����Ϣ
	virtual bool __cdecl SendRoomMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType)=NULL;
	//������Ϸ��Ϣ
	virtual bool __cdecl SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType)=NULL;

	//��Ϸ�ӿ�
public:
	//��ɢ��Ϸ
	virtual bool __cdecl DismissGame()=NULL;
	//������Ϸ
	virtual bool __cdecl ConcludeGame()=NULL;
	//���ͳ���
	virtual bool __cdecl SendGameScene(IServerUserItem * pIServerUserItem, void * pData, WORD wDataSize)=NULL;

	//д�ֽӿ�
public:
	//д�����
	virtual bool __cdecl WriteUserScore(WORD wChairID, LONG lScore, LONG lRevenue, enScoreKind ScoreKind, LONG lPlayTimeCount=-1)=NULL;
	//д�����
	virtual bool __cdecl WriteUserScore(IServerUserItem * pIServerUserItem, LONG lScore, LONG lRevenue, enScoreKind ScoreKind, LONG lPlayTimeCount=-1)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ITableFrameManager INTERFACE_VERSION(1,1)
	static const GUID IID_ITableFrameManager={0xab394013,0x708c,0x4cbe,0xa4,0xa7,0xe1,0x08,0x24,0x3c,0x91,0xdd};
#else
	#define VER_ITableFrameManager INTERFACE_VERSION(1,1)
	static const GUID IID_ITableFrameManager={0x4dba0eb4,0xd92b,0x471f,0xa5,0x50,0xb4,0xea,0x2a,0x6b,0xca,0x74};
#endif

//��ܹ���ӿ�
interface ITableFrameManager : public ITableFrame
{
	//���ýӿ�
public:
	//��ʼ��
	virtual bool __cdecl InitTableFrame(WORD wTableID, tagTableFrameParameter * pTableFrameParameter)=NULL;

	//��Ϣ�ӿ�
public:
	//����״̬
	virtual bool __cdecl IsTableLocked()=NULL;
	//��Ϸ״̬
	virtual bool __cdecl IsGameStarted()=NULL;
	//��Ϸ״̬
	virtual bool __cdecl IsUserPlaying(IServerUserItem * pIServerUserItem)=NULL;

	//ִ�нӿ�
public:
	//���¶���
	virtual bool __cdecl PerformSitDownAction(WORD wChairID, IServerUserItem * pIServerUserItem,LPCTSTR szPassword)=NULL;
	//�Թ۶���
	virtual bool __cdecl PerformLookonAction(WORD wChairID, IServerUserItem * pIServerUserItem,LPCTSTR szPassword)=NULL;
	//�뿪����
	virtual bool __cdecl PerformStandUpAction(IServerUserItem * pIServerUserItem)=NULL;

	//�û��¼�
public:
	//�����¼�
	virtual bool __cdecl OnUserOffLine(WORD wChairID)=NULL;
	//�ؽ��¼�
	virtual bool __cdecl OnUserReConnect(WORD wChairID)=NULL;
	//�������
	virtual bool __cdecl OnUserReqOffLine(WORD wChairID)=NULL;

	//ϵͳ�¼�
public:
	//��ʱ���¼�
	virtual bool __cdecl OnEventTimer(DWORD dwTimerID, WPARAM wBindParam)=NULL;
	//��Ϸ�¼�
	virtual bool __cdecl OnEventSocketGame(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)=NULL;
	//����¼�
	virtual bool __cdecl OnEventSocketFrame(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)=NULL;
	//��Ƶ�¼�����
	virtual bool __cdecl OnEventSocketVideo(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ITableFrameControl INTERFACE_VERSION(1,2)
	static const GUID IID_ITableFrameControl={0x3d1bbba0,0x9311,0x4dc9,0x97,0x04,0x62,0x36,0xd7,0xa9,0x29,0xb5};
#else
	#define VER_ITableFrameControl INTERFACE_VERSION(1,2)
	static const GUID IID_ITableFrameControl={0xb2590d93,0xe8fc,0x48fa,0x94,0x44,0x73,0xd8,0x98,0x96,0x88,0x82};
#endif

//���ƽӿ�
interface ITableFrameControl : public IUnknownEx
{
	//��ʼ��Ϸ
	virtual bool __cdecl StartGame()=NULL;
	//��ʼ�ж�
	virtual bool __cdecl StartVerdict()=NULL;
	//����λ��
	virtual bool __cdecl SwitchUserChair(WORD wSourceID[], WORD wTargetID[], WORD wSwitchCount)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ITableFrameSink INTERFACE_VERSION(1,1)
	static const GUID IID_ITableFrameSink={0xc9aea5dc,0xb7a8,0x47bf,0xb5,0x48,0xcb,0x4b,0xbb,0x3a,0x39,0x6a};
#else
	#define VER_ITableFrameSink INTERFACE_VERSION(1,1)
	static const GUID IID_ITableFrameSink={0x2b4a4b35,0x837a,0x4e84,0x86,0xcb,0xa0,0x01,0x09,0xa8,0xe9,0x9f};
#endif

//��Ϸ���ӻص��ӿ�
interface ITableFrameSink : public IUnknownEx
{
	//����ӿ�
public:
	//��ʼ��
	virtual bool __cdecl InitTableFrameSink(IUnknownEx * pIUnknownEx)=NULL;
	//��λ����
	virtual void __cdecl RepositTableFrameSink()=NULL;

	//��Ϣ�ӿ�
public:
	//��ʼģʽ
	virtual enStartMode __cdecl GetGameStartMode()=NULL;
	//��Ϸ״̬
	virtual bool __cdecl IsUserPlaying(WORD wChairID)=NULL;

	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool __cdecl OnEventGameStart()=NULL;
	//��Ϸ����
	virtual bool __cdecl OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)=NULL;
	//���ͳ���
	virtual bool __cdecl SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret)=NULL;

	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool __cdecl OnTimerMessage(WORD wTimerID, WPARAM wBindParam)=NULL;
	//��Ϸ��Ϣ
	virtual bool __cdecl OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)=NULL;
	//�����Ϣ
	virtual bool __cdecl OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ITableUserAction INTERFACE_VERSION(1,1)
	static const GUID IID_ITableUserAction={0x72c11395,0x1ee9,0x4dc2,0xa8,0x66,0x0e,0x3f,0xfb,0xca,0xbb,0xaf};
#else
	#define VER_ITableUserAction INTERFACE_VERSION(1,1)
	static const GUID IID_ITableUserAction={0xfaf49c38,0x0d59,0x4ab8,0xb2,0x3b,0x03,0xde,0x56,0x7d,0x00,0xd1};
#endif

//�����¼�
interface ITableUserAction : public IUnknownEx
{
	//�û�����
	virtual bool __cdecl OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)=NULL;
	//�û�����
	virtual bool __cdecl OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem)=NULL;
	//�û�����
	virtual bool __cdecl OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)=NULL;
	//�û�����
	virtual bool __cdecl OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)=NULL;
	//�û�ͬ��
	virtual bool __cdecl OnActionUserReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IAndroidUserItem INTERFACE_VERSION(1,1)
	static const GUID IID_IAndroidUserItem={0x8eab7240,0xd355,0x4b93,0x9f,0xb3,0x03,0x00,0xac,0xde,0x50,0xfb};
#else
	#define VER_IAndroidUserItem INTERFACE_VERSION(1,1)
	static const GUID IID_IAndroidUserItem={0xc83ac2b5,0x9e18,0x4909,0xb0,0x7e,0xd5,0x7c,0x56,0x9b,0x16,0x4b};
#endif

//�����˽ӿ�
interface IAndroidUserItem : public IUnknownEx
{
	//״̬����
public:
	//��ȡ״̬
	virtual BYTE __cdecl GetGameStatus()=NULL;
	//����״̬
	virtual VOID __cdecl SetGameStatus(BYTE cbGameStatus)=NULL;

	//��Ϣ�ӿ�
public:
	//��ȡ I D
	virtual DWORD __cdecl GetUserID()=NULL;
	//���Ӻ���
	virtual WORD __cdecl GetTableID()=NULL;
	//���Ӻ���
	virtual WORD __cdecl GetChairID()=NULL;

	//���ܽӿ�
public:
	//��ȡ�Լ�
	virtual IServerUserItem * __cdecl GetMeUserItem()=NULL;
	//��Ϸ�û�
	virtual IServerUserItem * __cdecl GetTableUserItem(WORD wChariID)=NULL;

	//����ӿ�
public:
	//���ͺ���
	virtual bool __cdecl SendSocketData(WORD wSubCmdID)=NULL;
	//���ͺ���
	virtual bool __cdecl SendSocketData(WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;

	//���ܽӿ�
public:
	//ɾ��ʱ��
	virtual bool __cdecl KillGameTimer(UINT nTimerID)=NULL;
	//����ʱ��
	virtual bool __cdecl SetGameTimer(UINT nTimerID, UINT nElapse)=NULL;
	//����׼��
	virtual bool __cdecl SendUserReady(VOID * pData, WORD wDataSize)=NULL;
	//��������
	virtual bool __cdecl SendChatMessage(DWORD dwTargetUserID, LPCTSTR pszChatMessage, COLORREF crFontColor)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IAndroidUserItemSink INTERFACE_VERSION(1,1)
	static const GUID IID_IAndroidUserItemSink={0xf87ebc8b,0xb80d,0x4a6d,0x9b,0x7b,0x40,0x2a,0x3e,0xd6,0x30,0x42};
#else
	#define VER_IAndroidUserItemSink INTERFACE_VERSION(1,1)
	static const GUID IID_IAndroidUserItemSink={0x43ef04c6,0x04e9,0x4603,0x92,0xff,0x97,0xda,0x1f,0x70,0x31,0x62};
#endif

//�����˽ӿ�
interface IAndroidUserItemSink : public IUnknownEx
{
	//���ƽӿ�
public:
	//��ʼ�ӿ�
	virtual bool __cdecl InitUserItemSink(IUnknownEx * pIUnknownEx)=NULL;
	//���ýӿ�
	virtual bool __cdecl RepositUserItemSink()=NULL;

	//��Ϸ�¼�
public:
	//ʱ����Ϣ
	virtual bool __cdecl OnEventTimer(UINT nTimerID)=NULL;
	//��Ϸ��Ϣ
	virtual bool __cdecl OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//��Ϸ��Ϣ
	virtual bool __cdecl OnEventFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//������Ϣ
	virtual bool __cdecl OnEventGameScene(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize)=NULL;

	//�û��¼�
public:
	//�û�����
	virtual VOID __cdecl OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)=NULL;
	//�û��뿪
	virtual VOID __cdecl OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)=NULL;
	//�û�����
	virtual VOID __cdecl OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)=NULL;
	//�û�״̬
	virtual VOID __cdecl OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)=NULL;
	//�û���λ
	virtual VOID __cdecl OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IAndroidUserManager INTERFACE_VERSION(1,1)
	static const GUID IID_IAndroidUserManager={0xbf347008,0x96d7,0x4ca7,0xb2,0x25,0xe9,0xbe,0x0e,0x04,0x94,0xfd};
#else
	#define VER_IAndroidUserManager INTERFACE_VERSION(1,1)
	static const GUID IID_IAndroidUserManager={0xe79d8438,0xe020,0x4bf4,0x80,0x5e,0x8b,0x7a,0x10,0xdc,0xd0,0xad};
#endif

//�����˽ӿ�
interface IAndroidUserManager : public IUnknownEx
{
	//���ƽӿ�
public:
	//ֹͣ����
	virtual bool __cdecl StopService()=NULL;
	//��������
	virtual bool __cdecl StartService()=NULL;

	//���ýӿ�
public:
	//���ÿ��
	virtual bool __cdecl SetAndroidStock(DWORD dwAndroidStock[], WORD wStockCount)=NULL;

	//����ӿ�
public:
	//ɾ��������
	virtual bool __cdecl DeleteAndroidUserItem(DWORD dwAndroidID)=NULL;
	//�л�������
	virtual IAndroidUserItem * __cdecl SwitchAndroidUserItem(IServerUserItem * pIServerUserItem)=NULL;
	//����������
	virtual IAndroidUserItem * __cdecl CreateAndroidUserItem(DWORD dwUserID, TCHAR szPassword[PASS_LEN])=NULL;

	//�¼��ӿ�
public:
	//�����¼�
	virtual bool __cdecl OnEventTimerPulse(DWORD dwTimerID, WPARAM dwBindParameter)=NULL;

	//����ӿ�
public:
	//��������
	virtual bool __cdecl SendDataToClient(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//��������
	virtual bool __cdecl SendDataToClient(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//��������
	virtual bool __cdecl SendDataToServer(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IGameServiceFrame INTERFACE_VERSION(1,1)
	static const GUID IID_IGameServiceFrame={0x1a009277,0x069d,0x42a2,0xb9,0xde,0x5b,0x39,0x29,0x0b,0xf9,0x9c};
#else
	#define VER_IGameServiceFrame INTERFACE_VERSION(1,1)
	static const GUID IID_IGameServiceFrame={0x4d4395f9,0xb18e,0x4b30,0xad,0xc0,0xfb,0x09,0x02,0xd6,0xf9,0x40};
#endif

//��Ϸ������
interface IGameServiceFrame : public IUnknownEx
{
	//״̬�ӿ�
public:
	//�رղ�ѯ
	virtual bool __cdecl IsShallClose()=NULL;
	//˽�Ĳ�ѯ
	virtual bool __cdecl IsAllowWisper()=NULL;
	//�����ѯ
	virtual bool __cdecl IsAllowRoomChat()=NULL;
	//�����ѯ
	virtual bool __cdecl IsAllowGameChat()=NULL;
	//�����ѯ
	virtual bool __cdecl IsAllowEnterRoom()=NULL;
	//�����ѯ
	virtual bool __cdecl IsAllowEnterGame()=NULL;

	//״̬֪ͨ
public:
	//����״̬
	virtual bool __cdecl SendTableStatus(WORD wTableID)=NULL;
	//���ͷ���
	virtual bool __cdecl SendUserScore(IServerUserItem * pIServerUserItem)=NULL;
	//����״̬
	virtual bool __cdecl SendUserStatus(IServerUserItem * pIServerUserItem)=NULL;
	//���͵���
	virtual bool __cdecl SendResidualProperty(IServerUserItem * pIServerUserItem)=NULL;

	//����ӿ�
public:
	//��������
	virtual bool __cdecl SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID)=NULL;
	//��������
	virtual bool __cdecl SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)=NULL;

	//��ʱ���ӿ�
public:
	//���ö�ʱ��
	virtual bool __cdecl SetTableTimer(WORD wTableID, DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM wBindParam)=NULL; 
	//ɾ����ʱ��
	virtual bool __cdecl KillTableTimer(WORD wTableID, DWORD dwTimerID)=NULL;

	//����ӿ�
public:
	//ɾ���û�
	virtual bool __cdecl DeleteUserItem(IServerUserItem * pIServerUserItem)=NULL;
	//�����Ϣ
	virtual void __cdecl ExportInformation(LPCTSTR pszString, enTraceLevel TraceLevel)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IGameServiceManager INTERFACE_VERSION(1,1)
	static const GUID IID_IGameServiceManager={0x5843d536,0xc6b7,0x4c9d,0x00aa,0xbc,0xce,0x05,0x2f,0x17,0x56,0x84};
#else
	#define VER_IGameServiceManager INTERFACE_VERSION(1,1)
	static const GUID IID_IGameServiceManager={0x1a17e988,0xaed4,0x4bcd,0x0082,0x4d,0x77,0xf7,0x1a,0x4d,0x62,0x21};
#endif

//��Ϸ����������ӿ�
interface IGameServiceManager : public IUnknownEx
{
	//�����ӿ�
public:
	//��ȡ����
	virtual void __cdecl GetGameServiceAttrib(tagGameServiceAttrib & GameServiceAttrib)=NULL;
	//�޸Ĳ���
	virtual bool __cdecl RectifyServiceOption(tagGameServiceOption * pGameServiceOption)=NULL;

	//�����ӿ�
public:
	//��������
	virtual VOID * __cdecl CreateTableFrameSink(REFGUID Guid, DWORD dwQueryVer)=NULL;
	//��������
	virtual VOID * __cdecl CreateAndroidUserItemSink(REFGUID Guid, DWORD dwQueryVer)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IGameService INTERFACE_VERSION(1,1)
	static const GUID IID_IGameService={0xcefa9ee9,0xb11f,0x47f9,0x8b,0xe1,0x92,0x70,0x25,0xfe,0x5d,0x57};
#else
	#define VER_IGameService INTERFACE_VERSION(1,1)
	static const GUID IID_IGameService={0x93376fd3,0xc55d,0x45d4,0xbd,0x43,0x3d,0x20,0x15,0x72,0x2e,0xb9};
#endif

//��Ϸ����ӿ�
interface IGameService : public IUnknownEx
{
	//����״̬
	virtual bool __cdecl IsService()=NULL;
	//��ʼ����
	virtual bool __cdecl StartService(tagGameServiceParameter * pGameServiceParameter)=NULL;
	//ֹͣ����
	virtual bool __cdecl StopService()=NULL;
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ�������������
class CGameServiceHelper : public CTempldateHelper<IGameService>
{
	//��������
public:
	//���캯��
	CGameServiceHelper(void) : CTempldateHelper<IGameService>(IID_IGameService,
		VER_IGameService,GAME_SERVICE_DLL_NAME,TEXT("CreateGameService")) { }
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ�������������
class CGameServiceManagerHelper : public CTempldateHelper<IGameServiceManager>
{
	//��������
public:
	//���캯��
	CGameServiceManagerHelper(void) : CTempldateHelper<IGameServiceManager>(IID_IGameServiceManager,
		VER_IGameServiceManager,GAME_SERVICE_DLL_NAME,TEXT("CreateGameServiceManager")) { }
};

//////////////////////////////////////////////////////////////////////////

#endif