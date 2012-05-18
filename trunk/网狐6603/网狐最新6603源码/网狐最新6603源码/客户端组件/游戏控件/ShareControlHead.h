#ifndef SHARE_CONTROL_HEAD_FILE
#define SHARE_CONTROL_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////

//ƽ̨�ļ�
#include "..\..\ȫ�ֶ���\Platform.h"

//��������
#include "..\..\��Ϣ����\CMD_Commom.h"
#include "..\..\��Ϣ����\CMD_GameServer.h"
#include "..\..\��Ϣ����\CMD_LogonServer.h"

//����ļ�
#include "..\..\�ͻ������\ͼ��ؼ�\WHImageHead.h"
#include "..\..\�ͻ������\�������\DownLoadHead.h"
#include "..\..\�ͻ������\����ؼ�\SkinControlHead.h"
#include "..\..\�ͻ������\�û�����\UserServiceHead.h"
#include "..\..\�ͻ������\ƽ̨����\PlatformDataHead.h"
#include "..\..\�ͻ������\�������\AvatarControlHead.h"

//////////////////////////////////////////////////////////////////////////////////
//��������

//��������
#ifndef SHARE_CONTROL_CLASS
	#ifdef  SHARE_CONTROL_DLL
		#define SHARE_CONTROL_CLASS _declspec(dllexport)
	#else
		#define SHARE_CONTROL_CLASS _declspec(dllimport)
	#endif
#endif

//ģ�鶨��
#ifndef _DEBUG
	#define SHARE_CONTROL_DLL_NAME	TEXT("ShareControl.dll")			//�������
#else
	#define SHARE_CONTROL_DLL_NAME	TEXT("ShareControlD.dll")			//�������
#endif

//////////////////////////////////////////////////////////////////////////////////

//���澯��
#pragma warning(disable:4192)

//FALSH ����
#import "Flash.ocx" named_guids

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IFlashControlSink INTERFACE_VERSION(1,1)
	static const GUID IID_IFlashControlSink={0xb10c2bd0,0x1f03,0x431c,0xb2,0x4b,0xd9,0x9f,0x50,0xfb,0xff,0x70};
#else
	#define VER_IFlashControlSink INTERFACE_VERSION(1,1)
	static const GUID IID_IFlashControlSink={0x74d727ef,0x9847,0x4036,0x83,0xb8,0x9b,0xeb,0x1c,0x87,0xb9,0xa1};
#endif

//�����ӿ�
interface IFlashControlSink : public IUnknownEx
{
	//�������
	virtual VOID OnEventFlashPlayFinish()=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IRichEditMessageSink INTERFACE_VERSION(1,1)
	static const GUID IID_IRichEditMessageSink={0xd5956ee6,0x19e7,0x4567,0x87,0x31,0xf9,0x48,0x83,0x36,0x19,0x62};
#else
	#define VER_IRichEditMessageSink INTERFACE_VERSION(1,1)
	static const GUID IID_IRichEditMessageSink={0x79bfffe7,0xafef,0x4be6,0x84,0xad,0x07,0x79,0x3f,0x27,0x5d,0xec};
#endif

//��Ϣ�ӿ�
interface IRichEditMessageSink : public IUnknownEx
{
	//����ӿ�
	virtual bool OnEventStringLink(LPCTSTR pszString, UINT uMessage)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IStringMessage INTERFACE_VERSION(1,1)
	static const GUID IID_IStringMessage={0x2947cade,0xfd9a,0x40a2,0x9f,0x74,0xaa,0x7a,0xb4,0xfa,0x06,0x5d};
#else
	#define VER_IStringMessage INTERFACE_VERSION(1,1)
	static const GUID IID_IStringMessage={0xdd77eded,0x6b1e,0x4839,0xa6,0x05,0x92,0x9e,0xc6,0xec,0xd0,0x52};
#endif

//��Ϣ�ӿ�
interface IStringMessage : public IUnknownEx
{
	//�¼���Ϣ
public:
	//�����¼�
	virtual bool InsertUserEnter(LPCTSTR pszUserName)=NULL;
	//�뿪�¼�
	virtual bool InsertUserLeave(LPCTSTR pszUserName)=NULL;
	//�����¼�
	virtual bool InsertUserOffLine(LPCTSTR pszUserName)=NULL;

	//�ַ���Ϣ
public:
	//��ͨ��Ϣ
	virtual bool InsertNormalString(LPCTSTR pszString)=NULL;
	//ϵͳ��Ϣ
	virtual bool InsertSystemString(LPCTSTR pszString)=NULL;
	//��ʾ��Ϣ
	virtual bool InsertPromptString(LPCTSTR pszString)=NULL;
	//������Ϣ
	virtual bool InsertAfficheString(LPCTSTR pszString)=NULL;

	//������Ϣ
public:
	//������Ϣ
	virtual bool InsertCustomString(LPCTSTR pszString, COLORREF crColor)=NULL;
	//������Ϣ
	virtual bool InsertCustomString(LPCTSTR pszString, COLORREF crColor, COLORREF crBackColor)=NULL;

	//������Ϣ
public:
	//�û�����
	virtual bool InsertExpression(LPCTSTR pszSendUser, LPCTSTR pszImagePath)=NULL;
	//�û�����
	virtual bool InsertExpression(LPCTSTR pszSendUser, LPCTSTR pszImagePath, bool bMyselfString)=NULL;
	//�û�����
	virtual bool InsertExpression(LPCTSTR pszSendUser, LPCTSTR pszRecvUser, LPCTSTR pszImagePath)=NULL;

	//������Ϣ
public:
	//�û�����
	virtual bool InsertUserChat(LPCTSTR pszSendUser, LPCTSTR pszString, COLORREF crColor)=NULL;
	//�û�����
	virtual bool InsertUserChat(LPCTSTR pszSendUser, LPCTSTR pszRecvUser, LPCTSTR pszString, COLORREF crColor)=NULL;
	//�û�˽��
	virtual bool InsertWhisperChat(LPCTSTR pszSendUser, LPCTSTR pszString, COLORREF crColor, bool bMyselfString)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//�����ļ�

#ifndef SHARE_CONTROL_DLL
	#include "DlgBrowser.h"
	#include "WebBrowser.h"
	#include "WebMessage.h"
	#include "Expression.h"
	#include "Information.h"
	#include "WebPublicize.h"
	#include "FlashControl.h"
	#include "MissionManager.h"
	#include "WndChatControl.h"
	#include "SystemTrayIcon.h"
	#include "PasswordControl.h"
	#include "RichEditMessage.h"
	#include "UserInformation.h"
	#include "UserItemElement.h"
	#include "UserListControl.h"
	#include "DlgInsureCounter.h"
#endif

//////////////////////////////////////////////////////////////////////////////////

#endif