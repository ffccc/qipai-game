#ifndef GAME_FRAME_HEAD_FILE
#define GAME_FRAME_HEAD_FILE

//////////////////////////////////////////////////////////////////////////

//ȫ��ͷ�ļ�
#include "..\..\�����ļ�\GlobalDef.h"
#include "..\..\�����ļ�\GlobalField.h"
#include "..\..\�����ļ�\GlobalFrame.h"
#include "..\..\�����ļ�\GlobalRight.h"

//����ͷ�ļ�
#include "..\..\��Ϣ����\CMD_Game.h"
#include "..\..\��Ϣ����\CMD_Plaza.h"
#include "..\..\��Ϣ����\CMD_Video.h"

//ģ���
#include "..\..\ģ���\Template.h"

//���ͷ�ļ�
#include "..\��Ϸ�ȼ�\GameRank.h"
#include "..\ͷ�����\UserFace.h"
#include "..\��ϵ����\Companion.h"
#include "..\�ŵ�ģ��\ChannelModule.h"
#include "..\�ͻ��˹���\ClientShare.h"
#include "..\..\�������\����ؼ�\SkinControls.h"
#include "..\..\�������\������Դ\SkinResourceModule.h"

//////////////////////////////////////////////////////////////////////////
//�����궨��

//��������
#ifndef GAME_FRAME_CLASS
	#ifdef  GAME_FRAME_DLL
		#define GAME_FRAME_CLASS _declspec(dllexport)
	#else
		#define GAME_FRAME_CLASS _declspec(dllimport)
	#endif
#endif

//ģ�鶨��
#ifdef _DEBUG
	#define GAME_FRAME_DLL_NAME	TEXT("GameFrameD.dll")					//��� DLL ����
#else
	#define GAME_FRAME_DLL_NAME	TEXT("GameFrame.dll")					//��� DLL ����
#endif

//////////////////////////////////////////////////////////////////////////
//�ṹ�嶨��

//��������
struct tagServerAttribute
{
	WORD							wKindID;							//���ͱ�ʶ
	WORD							wServerID;							//�����ʶ
	WORD							wGameGenre;							//��Ϸ����
	WORD							wChairCount;						//������Ŀ
	DWORD							dwVideoAddr;						//��Ƶ��ַ
	BYTE							cbHideUserInfo;						//������Ϣ
	TCHAR							szKindName[KIND_LEN];				//��������
	TCHAR							szServerName[SERVER_LEN];			//��������
};

//////////////////////////////////////////////////////////////////////////

#define VER_IClientKernel INTERFACE_VERSION(5,1)
static const GUID IID_IClientKernel={0x850581d,0x1d7a,0x4137,0xa4,0xb7,0xf0,0x3,0x36,0x1c,0x0,0xed};

//�ͻ����ں�
interface IClientKernel : public IUnknownEx
{
	//�����ӿ�
public:
	//���ýӿ�
	virtual bool __cdecl SetMessageProxy(IUnknownEx * pIUnknownEx)=NULL;
	//���ýӿ�
	virtual bool __cdecl DestroyClientKernel()=NULL;
	//�ں�����
	virtual bool __cdecl InitClientKernel(LPCTSTR lpszComLine, IUnknownEx * pIUnknownEx)=NULL;

	//��Ϣ�ӿ�
public:
	//�뿪״̬
	virtual bool __cdecl GetExitFlags()=NULL;
	//�Թ��û�
	virtual bool __cdecl IsLookonMode()=NULL;
	//�����Թ�
	virtual bool __cdecl IsAllowLookon()=NULL;
	//��������
	virtual const tagServerAttribute * __cdecl GetServerAttribute()=NULL;

	//�Թۿ���
public:
	//�Թ�����
	virtual bool __cdecl IsAllowUserLookon()=NULL;
	//�Թۿ���
	virtual bool __cdecl AllowUserLookon(DWORD dwUserID, bool bAllowLookon)=NULL;

	//״̬�ӿ�
public:
	//��Ϸ״̬
	virtual BYTE __cdecl GetGameStatus()=NULL;
	//��Ϸ״̬
	virtual void __cdecl SetGameStatus(BYTE bGameStatus)=NULL;

	//�û��ӿ�
public:
	//�Լ�λ��
	virtual WORD __cdecl GetMeChairID()=NULL;
	//��ȡ�Լ�
	virtual const tagUserData * __cdecl GetMeUserInfo()=NULL;
	//��ȡ���
	virtual const tagUserData * __cdecl GetUserInfo(WORD wChairID)=NULL;

	//���ͽӿ�
public:
	//��ʾ��Ϣ
	virtual int __cdecl ShowMessageBox(LPCTSTR pszMessage, UINT nType)=NULL;
	//���ͺ���
	virtual bool __cdecl SendSocketData(WORD wMainCmdID, WORD wSubCmdID)=NULL;
	//���ͺ���
	virtual bool __cdecl SendSocketData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)=NULL;
	//��������
	virtual bool __cdecl SendProcessData(WORD wMainCmdID, WORD wSubCmdID)=NULL;
	//��������
	virtual bool __cdecl SendProcessData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize)=NULL;

	//���ܽӿ�
public:
	//����׼��
	virtual bool __cdecl SendUserReady(void * pBuffer, WORD wDataSize)=NULL;
	//��������
	virtual bool __cdecl SendChatMessage(DWORD dwTargetUserID, LPCTSTR pszChatMessage, COLORREF crFontColor)=NULL;

	//��ʱ���ӿ�
public:
	//��ʱ��λ��
	virtual WORD __cdecl GetTimeChairID()=NULL;
	//���ö�ʱ��
	virtual bool __cdecl SetGameTimer(WORD wChairID, UINT nTimerID, UINT nElapse)=NULL;
	//ɾ����ʱ��
	virtual bool __cdecl KillGameTimer(UINT nTimerID)=NULL;

	//�û�����
public:
	//�����û�
	virtual tagUserData * __cdecl SearchUserItem(DWORD dwUserID)=NULL;
	//ö�����
	virtual tagUserData * __cdecl EnumLookonUser(WORD wIndex)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_IClientKernelSink INTERFACE_VERSION(1,1)
static const GUID IID_IClientKernelSink={0xebda1f6e,0x3ce2,0x448d,0x99,0x5,0x87,0x6e,0x52,0x71,0x6b,0x7f};

//�ں˹��ӽӿ�
interface IClientKernelSink : public IUnknownEx
{
	//�����ӿ�
public:
	//��ȡ����
	virtual HWND __cdecl GetFrameWnd()=NULL;
	//����״̬
	virtual void __cdecl ResetGameFrame()=NULL;
	//�رմ���
	virtual void __cdecl CloseGameFrame(bool bForceClose)=NULL;

	//�ں��¼�
public:
	//ʱ����Ϣ
	virtual bool __cdecl OnEventTimer(WORD wChairID, UINT nElapse, UINT nTimerID)=NULL;
	//������Ϣ
	virtual bool __cdecl OnEventSocket(const CMD_Command & Command, const void * pBuffer, WORD wDataSize)=NULL;
	//������Ϣ
	virtual bool __cdecl OnEventProcess(const IPC_Head & Head, const void * pBuffer, WORD wDataSize, HWND hWndSend)=NULL;

	//��Ϸ�¼�
public:
	//��Ϸ״̬
	virtual bool __cdecl OnEventGameScene(BYTE cbGameStation, bool bLookonOther, void * pBuffer, WORD wDataSize)=NULL;
	//������Ϣ
	virtual bool __cdecl OnUserChatMessage(tagUserData * pSendUserData, tagUserData * pRecvUserData, LPCTSTR pszChatMessage, COLORREF crColor)=NULL;
	//ʱ�����
	virtual void __cdecl OnEventTimerKilled(WORD wChairID, UINT nTimerID)=NULL;
	//�Թ�״̬
	virtual void __cdecl OnEventLookonChanged(bool bLookonUser, void * pBuffer, WORD wDataSize)=NULL;

	//�û��¼�
public:
	//�û�����
	virtual void __cdecl OnEventUserEnter(tagUserData * pUserData, WORD wChairID, bool bLookonUser)=NULL;
	//�û��뿪
	virtual void __cdecl OnEventUserLeave(tagUserData * pUserData, WORD wChairID, bool bLookonUser)=NULL;
	//�û�����
	virtual void __cdecl OnEventUserScore(tagUserData * pUserData, WORD wChairID, bool bLookonUser)=NULL;
	//�û�״̬
	virtual void __cdecl OnEventUserStatus(tagUserData * pUserData, WORD wChairID, bool bLookonUser)=NULL;
	//��Ա�ȼ�
	virtual void __cdecl OnEventUserMemberOrder(tagUserData * pUserData, WORD wChairID, bool bLookonUser)=NULL;
	
	//�����¼�
public:
	//��ʼ����
	virtual void __cdecl OnEventInitProperty(tagPropertyInfo *pPropertyInfo, int nInfoCount)=NULL;
	//��ʼ�ʻ�
	virtual void __cdecl OnEventInitFlower(tagFlowerInfo *pFlowerInfo, int nInfoCount)=NULL;
	//�ʻ���Ϣ
	virtual void __cdecl OnEventFlower(const tagUserData * pSendUserData, const tagUserData * pRecvUserData, UINT uFlowerID, UINT uFlowerEffectID)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_IGameFrameView INTERFACE_VERSION(1,1)
static const GUID IID_IGameFrameView={0x22d2598,0x624f,0x46f9,0xa6,0xde,0xbc,0x85,0xf4,0xc8,0x12,0x11};

//��Ϸ��ͼ�ӿ�
interface IGameFrameView : public IUnknownEx
{
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ�ں˶�������
class CClientKernelHelper : public CTempldateHelper<IClientKernel>
{
	//��������
public:
	//���캯��
	CClientKernelHelper(void) : CTempldateHelper<IClientKernel>(IID_IClientKernel,
		VER_IClientKernel,GAME_FRAME_DLL_NAME,TEXT("CreateClientKernel"))
	{
	}
};

//////////////////////////////////////////////////////////////////////////

//��չ���
#include "..\..\�ͻ������\��Ƶ���\VideoServiceModule.h"

//������ͷ�ļ�
#ifndef GAME_FRAME_DLL
	#include "ThreadDraw.h"
	#include "GameFrameApp.h"
	#include "GameFrameDlg.h"
	#include "GameFrameView.h"
#endif

//////////////////////////////////////////////////////////////////////////

#endif