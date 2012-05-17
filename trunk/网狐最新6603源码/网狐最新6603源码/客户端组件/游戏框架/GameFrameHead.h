#ifndef GAME_FRAME_HEAD_HEAD_FILE
#define GAME_FRAME_HEAD_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//�����ļ�

//MFC �ļ�
#include <Afxmt.h>
#include <AtlBase.h>

//ƽ̨�ļ�
#include "..\..\ȫ�ֶ���\Platform.h"

//��������
#include "..\..\��Ϣ����\CMD_Commom.h"
#include "..\..\��Ϣ����\CMD_GameServer.h"

//��������
#include "..\..\��Ϣ����\IPC_GameFrame.h"

//����ļ�
#include "..\..\�������\�������\ServiceCoreHead.h"
#include "..\..\�������\���̿���\ProcessControlHead.h"

//����ļ�
#include "..\..\�ͻ������\ͼ��ؼ�\WHImageHead.h"
#include "..\..\�ͻ������\�û�����\UserServiceHead.h"
#include "..\..\�ͻ������\����ؼ�\SkinControlHead.h"
#include "..\..\�ͻ������\��Ϸ�ؼ�\ShareControlHead.h"
#include "..\..\�ͻ������\ƽ̨����\PlatformDataHead.h"
#include "..\..\�ͻ������\��Ϸ����\GamePropertyHead.h"
#include "..\..\�ͻ������\�������\AvatarControlHead.h"

//��Ϸ����
#include "..\..\�ͻ������\��Ϸ����\GameEngineHead.h"

//////////////////////////////////////////////////////////////////////////////////
//��������

//��������
#ifndef GAME_FRAME_CLASS
	#ifdef  GAME_FRAME_DLL
		#define GAME_FRAME_CLASS _declspec(dllexport)
	#else
		#define GAME_FRAME_CLASS _declspec(dllimport)
	#endif
#endif

//ģ�鶨��
#ifndef _DEBUG
	#define GAME_FRAME_DLL_NAME		TEXT("GameFrame.dll")				//�������
#else
	#define GAME_FRAME_DLL_NAME		TEXT("GameFrameD.dll")				//�������
#endif

//////////////////////////////////////////////////////////////////////////////////
//�궨��

//Ԫ����Ŀ
#define MAX_CLOCK_ITEM				3									//ʱ����Ŀ

//���ִ�С
#define ACCOUNTS_CX					96									//�ʺſ��
#define ACCOUNTS_CY					12									//�ʺŸ߶�

//////////////////////////////////////////////////////////////////////////////////
//ö�ٶ���

//�滭ģʽ
#define DRAW_MODE_SPREAD			0									//ƽ��ģʽ
#define DRAW_MODE_CENTENT			1									//����ģʽ
#define DRAW_MODE_ELONGGATE			2									//����ģʽ

//////////////////////////////////////////////////////////////////////////////////
//�ṹ����

//�û�����
struct tagUserAttribute
{
	//�û�����
	DWORD							dwUserID;							//�û���ʶ
	WORD							wTableID;							//���Ӻ���
	WORD							wChairID;							//���Ӻ���

	//Ȩ������
	DWORD							dwUserRight;						//�û�Ȩ��
	DWORD							dwMasterRight;						//����Ȩ��
};

//��Ϸ����
struct tagGameAttribute
{
	WORD							wKindID;							//���ͱ�ʶ
	WORD							wChairCount;						//������Ŀ
	DWORD							dwClientVersion;					//��Ϸ�汾
	TCHAR							szGameName[LEN_KIND];				//��Ϸ����
};

//��������
struct tagServerAttribute
{
	WORD							wKindID;							//���ͱ�ʶ
	WORD							wServerID;							//�����ʶ
	WORD							wServerType;						//��Ϸ����
	DWORD							dwServerRule;						//�������
	TCHAR							szServerName[LEN_SERVER];			//��������
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IClientKernel INTERFACE_VERSION(1,1)
	static const GUID IID_IClientKernel={0xa6da55ae,0x784b,0x44fa,0x00be,0xad,0xd8,0xc4,0xab,0x2b,0x9c,0x27};
#else
	#define VER_IClientKernel INTERFACE_VERSION(1,1)
	static const GUID IID_IClientKernel={0xb6f72cb2,0x62a4,0x42cc,0x00b6,0x43,0x26,0x2f,0xda,0x74,0x11,0x5c};
#endif

//�ͻ����ں�
interface IClientKernel : public IUnknownEx
{
	//���ýӿ�
public:
	//�ں�����
	virtual bool InitClientKernel(LPCTSTR lpszComLine)=NULL;
	//���ýӿ�
	virtual bool SetUserEventSink(IUnknownEx * pIUnknownEx)=NULL;
	//���ýӿ�
	virtual bool SetStringMessage(IUnknownEx * pIUnknownEx)=NULL;
	//���ýӿ�
	virtual bool SetClientKernelSink(IUnknownEx * pIUnknownEx)=NULL;
	//��Ϸ����
	virtual bool SetGameAttribute(WORD wKindID, WORD wPlayerCount, DWORD dwClientVersion, HICON hGameIcon, LPCTSTR pszGameName)=NULL;

	//���ӽӿ�
public:
	//���Ӵ���
	virtual bool CreateConnect()=NULL;
	//�ж�����
	virtual bool IntermitConnect()=NULL;

	//���Խӿ�
public:
	//�û�����
	virtual tagUserAttribute * GetUserAttribute()=NULL;
	//��Ϸ����
	virtual tagGameAttribute * GetGameAttribute()=NULL;
	//��������
	virtual tagServerAttribute * GetServerAttribute()=NULL;

	//ģʽ�ӿ�
public:
	//����ģʽ
	virtual bool IsSingleMode()=NULL;
	//�Թ�״̬
	virtual bool IsLookonMode()=NULL;
	//����״̬
	virtual bool IsAllowLookon()=NULL;
	//����״̬
	virtual bool IsServiceStatus()=NULL;

	//״̬�ӿ�
public:
	//��ȡ״̬
	virtual BYTE GetGameStatus()=NULL;
	//����״̬
	virtual VOID SetGameStatus(BYTE cbGameStatus)=NULL;

	//�û��ӿ�
public:
	//�Լ�λ��
	virtual WORD GetMeChairID()=NULL;
	//�Լ�λ��
	virtual IClientUserItem * GetMeUserItem()=NULL;
	//��Ϸ�û�
	virtual IClientUserItem * GetTableUserItem(WORD wChariID)=NULL;
	//�����û�
	virtual IClientUserItem * SearchUserByUserID(DWORD dwUserID)=NULL;
	//�����û�
	virtual IClientUserItem * SearchUserByGameID(DWORD dwGameID)=NULL;
	//�Թ��û�
	virtual IClientUserItem * EnumLookonUserItem(WORD wEnumIndex)=NULL;

	//����ӿ�
public:
	//���ͺ���
	virtual bool SendSocketData(WORD wMainCmdID, WORD wSubCmdID)=NULL;
	//���ͺ���
	virtual bool SendSocketData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;

	//���̽ӿ�
public:
	//��������
	virtual bool SendProcessData(WORD wMainCmdID, WORD wSubCmdID)=NULL;
	//��������
	virtual bool SendProcessData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//ִ������
	virtual bool SendProcessCommand(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;

	//���ܽӿ�
public:
	//����׼��
	virtual bool SendUserReady(VOID * pData, WORD wDataSize)=NULL;
	//�Թۿ���
	virtual bool SendUserLookon(DWORD dwUserID, bool bAllowLookon)=NULL;
	//���ͱ���
	virtual bool SendUserExpression(DWORD dwTargetUserID, WORD wItemIndex)=NULL;
	//��������
	virtual bool SendUserChatMessage(DWORD dwTargetUserID, LPCTSTR pszChatString, COLORREF crColor)=NULL;

	//�����ӿ�
public:
	//������
	virtual VOID ActiveGameFrame()=NULL;
	//�л�����
	virtual WORD SwitchViewChairID(WORD wChairID)=NULL;

	//ʱ�ӽӿ�
public:
	//ʱ�ӱ�ʶ
	virtual UINT GetClockID()=NULL;
	//ʱ��λ��
	virtual WORD GetClockChairID()=NULL;
	//ɾ��ʱ��
	virtual VOID KillGameClock(WORD wClockID)=NULL;
	//����ʱ��
	virtual VOID SetGameClock(WORD wChairID, WORD wClockID, UINT nElapse)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IClientKernelSink INTERFACE_VERSION(1,1)
	static const GUID IID_IClientKernelSink={0xfec3bd90,0x2ba4,0x4dc4,0x00ae,0x2c,0xd7,0x5b,0x0c,0xe1,0x59,0xed};
#else
	#define VER_IClientKernelSink INTERFACE_VERSION(1,1)
	static const GUID IID_IClientKernelSink={0xae401d54,0xaf7f,0x4b46,0x00ab,0x5c,0x59,0x8b,0x8c,0x4a,0x31,0x7e};
#endif

//�ں˻ص�
interface IClientKernelSink : public IUnknownEx
{
	//���ƽӿ�
public:
	//������Ϸ
	virtual VOID ResetGameClient()=NULL;
	//�ر���Ϸ
	virtual VOID CloseGameClient()=NULL;

	//���ýӿ�
public:
	//�������
	virtual bool OnSchemeFinish()=NULL;
	//�б�����
	virtual bool SchemeColumnItem(tagColumnItem ColumnItem[], BYTE cbColumnCount)=NULL;
	//��������
	virtual bool SchemePropertyInfo(tagPropertyInfo PropertyInfo[], BYTE cbPropertyCount)=NULL;

	//�ں��¼�
public:
	//��Ϸ��Ϣ
	virtual bool OnEventSocket(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//������Ϣ
	virtual bool OnEventProcess(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;

	//��Ϸ�¼�
public:
	//�Թ���Ϣ
	virtual bool OnEventLookonMode(VOID * pData, WORD wDataSize)=NULL;
	//������Ϣ
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize)=NULL;

	//ʱ���¼�
public:
	//ʱ��ɾ��
	virtual bool OnEventGameClockKill(WORD wChairID)=NULL;
	//ʱ����Ϣ
	virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IUserEventSink INTERFACE_VERSION(1,1)
	static const GUID IID_IUserEventSink={0x6cfedee7,0x4acb,0x4ae1,0x84,0x92,0xe8,0x49,0x87,0x60,0xff,0x6f};
#else
	#define VER_IUserEventSink INTERFACE_VERSION(1,1)
	static const GUID IID_IUserEventSink={0x3719eae7,0xd137,0x4036,0x97,0xa0,0x72,0x67,0x26,0x25,0x43,0x01};
#endif

//�û��¼�
interface IUserEventSink : public IUnknownEx
{
	//�û��¼�
public:
	//�û�����
	virtual VOID OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser)=NULL;
	//�û��뿪
	virtual VOID OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser)=NULL;
	//�û�����
	virtual VOID OnEventUserScore(IClientUserItem * pIClientUserItem, bool bLookonUser)=NULL;
	//�û�״̬
	virtual VOID OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser)=NULL;
	//�û�ͷ��
	virtual VOID OnEventCustomFace(IClientUserItem * pIClientUserItem, bool bLookonUser)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IGameFrameWnd INTERFACE_VERSION(1,1)
	static const GUID IID_IGameFrameWnd={0xb6ac5ec5,0x26f9,0x47dd,0xb5,0x0f,0x4f,0x73,0xa1,0x3e,0xbc,0xc9};
#else
	#define VER_IGameFrameWnd INTERFACE_VERSION(1,1)
	static const GUID IID_IGameFrameWnd={0x4224b444,0x7ed3,0x44d3,0xa9,0x77,0x0b,0xdc,0xae,0xe2,0x56,0x08};
#endif

//��Ϸ���
interface IGameFrameWnd : public IUnknownEx
{
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IGameFrameView INTERFACE_VERSION(1,1)
	static const GUID IID_IGameFrameView={0x8b8011d9,0x2e83,0x419c,0xa5,0x90,0x2f,0xe5,0xcf,0xb6,0xdb,0x58};
#else
	#define VER_IGameFrameView INTERFACE_VERSION(1,1)
	static const GUID IID_IGameFrameView={0xf940a4c3,0x926d,0x4b01,0x82,0xc9,0xaf,0x1b,0xab,0x3f,0x1e,0x1f};
#endif

//��ͼ�ӿ�
interface IGameFrameView : public IUnknownEx
{
	//���ڽӿ�
public:
	//���ھ��
	virtual HWND GetGameViewHwnd()=NULL;
	//��������
	virtual bool CreateGameViewWnd(CWnd * pParentWnd, UINT nWndID)=NULL;

	//���ƽӿ�
public:
	//�л�״̬
	virtual VOID SwitchToReadyStatus()=NULL;

	//ʱ�ӹ���
public:
	//��ȡʱ��
	virtual WORD GetUserClock(WORD wChairID)=NULL;
	//����ʱ��
	virtual VOID SetUserClock(WORD wChairID, WORD wUserClock)=NULL;

	//�û�����
public:
	//��ȡ�û�
	virtual IClientUserItem * GetClientUserItem(WORD wChairID)=NULL;
	//�����û�
	virtual VOID SetClientUserItem(WORD wChairID, IClientUserItem * pIClientUserItem)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IGameFrameControl INTERFACE_VERSION(1,1)
	static const GUID IID_IGameFrameControl={0xf908a318,0x3729,0x4fbc,0x97,0x09,0xc1,0x7b,0x3c,0x04,0x66,0xcc};
#else
	#define VER_IGameFrameControl INTERFACE_VERSION(1,1)
	static const GUID IID_IGameFrameControl={0xab9c1739,0x510e,0x4ef3,0x84,0xe3,0xb7,0xe0,0x82,0xf1,0x2b,0x84};
#endif

//���ƿ��
interface IGameFrameControl : public IUnknownEx
{
	//���ƽӿ�
public:
	//���ÿؼ�
	virtual VOID ResetControl()=NULL;
	//���ù��
	virtual VOID SetGameBillInfo(WORD wKindID, WORD wServerID)=NULL;
	//�����û�
	virtual VOID SetUserViewInfo(IClientUserItem * pIClientUserItem)=NULL;
	//�б�����
	virtual VOID SetColumnDescribe(tagColumnItem ColumnItem[], BYTE cbColumnCount)=NULL;
	//��������
	virtual VOID SetPropertyDescribe(tagPropertyInfo PropertyInfo[], BYTE cbPropertyCount)=NULL;

	//�˵��ӿ�
public:
	//��ʾ�˵�
	virtual VOID TrackUserItemMenu(IClientUserItem * pIClientUserItem)=NULL;
	//��ʾ�˵�
	virtual VOID TrackUserItemMenu(IClientUserItem * pIClientUserItem[], WORD wItemCount)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IGameFrameToolBar INTERFACE_VERSION(1,1)
	static const GUID IID_IGameFrameToolBar={0x42f11860,0x4f2b,0x4d09,0xa1,0x4a,0xbd,0x44,0x5a,0xd7,0xbd,0xa5};
#else
	#define VER_IGameFrameToolBar INTERFACE_VERSION(1,1)
	static const GUID IID_IGameFrameToolBar={0xa51ecb9c,0x2a15,0x4243,0xa7,0x6d,0x5d,0x45,0x73,0x62,0xd1,0x35};
#endif

//���߿��
interface IGameFrameToolBar : public IUnknownEx
{
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IGameFrameEngine INTERFACE_VERSION(1,1)
	static const GUID IID_IGameFrameEngine={0xaea43c61,0x98e5,0x429c,0x88,0x7b,0x07,0x94,0xec,0x19,0x4e,0x1a};
#else
	#define VER_IGameFrameEngine INTERFACE_VERSION(1,1)
	static const GUID IID_IGameFrameEngine={0x917a4fe1,0x9754,0x4dad,0x86,0x82,0xdb,0x30,0x55,0x6f,0xb3,0x8e};
#endif

//�������
interface IGameFrameEngine : public  IUnknownEx
{
	//ϵͳ�¼�
public:
	//��������
	virtual bool OnInitGameEngine()=NULL;
	//���ú���
	virtual bool OnResetGameEngine()=NULL;

	//ʱ���¼�
public:
	//ʱ��ɾ��
	virtual bool OnEventGameClockKill(WORD wChairID)=NULL;
	//ʱ����Ϣ
	virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID)=NULL;

	//��Ϸ�¼�
public:
	//�Թ���Ϣ
	virtual bool OnEventLookonMode(VOID * pData, WORD wDataSize)=NULL;
	//��Ϸ��Ϣ
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//������Ϣ
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IGameFrameService INTERFACE_VERSION(1,1)
	static const GUID IID_IGameFrameService={0x75f319b5,0xcfa5,0x49b0,0x9a,0x30,0x92,0x6a,0xa7,0x23,0x5e,0x8c};
#else
	#define VER_IGameFrameService INTERFACE_VERSION(1,1)
	static const GUID IID_IGameFrameService={0x06d5798d,0x6cc1,0x45c2,0xb6,0xe3,0x4d,0x4a,0xfa,0xa0,0x5a,0x83};
#endif

//��ܷ���
interface IGameFrameService : public IUnknownEx
{
};

//////////////////////////////////////////////////////////////////////////////////
//���ģ��

DECLARE_MODULE_HELPER(ClientKernel,GAME_FRAME_DLL_NAME,"CreateClientKernel")

//////////////////////////////////////////////////////////////////////////////////
//�����ļ�

#ifndef GAME_FRAME_DLL
	#include "GlobalUnits.h"
	#include "GameFrameApp.h"
	#include "GameFrameWnd.h"
	#include "GameFrameView.h"
	#include "GameFrameEngine.h"
	#include "GameFrameAvatar.h"
	#include "GameFrameService.h"
	#include "GameFrameControl.h"
	#include "GameFrameViewD3D.h"
	#include "GameFrameViewGDI.h"
	#include "GameFrameUserView.h"
	#include "GameFrameMedalView.h"
	#include "GameFramePublicize.h"
#endif

//////////////////////////////////////////////////////////////////////////////////

#endif