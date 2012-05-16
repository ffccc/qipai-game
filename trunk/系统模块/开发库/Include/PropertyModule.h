#ifndef PROPERTY_MODULE_HEAD_FILE1
#define PROPERTY_MODULE_HEAD_FILE1
//////////////////////////////////////////////////////////////////////////

//ȫ��ͷ�ļ�
#include "GlobalProperty.h"

//ģ���
#include "Template.h"
#include "Module.h"

//���ͷ�ļ�
#include "ClientShare.h"
#include "NetworkServiceHead.h"

//�ӿ�ͷ�ļ�
#include "ComService.h"
#include "SkinControls.h"
#include "SkinResourceModule.h"

//////////////////////////////////////////////////////////////////////////

//��������
#ifndef PROPERTY_MODULE_CLASS
	#ifdef PROPERTY_MODULE_DLL
		#define PROPERTY_MODULE_CLASS _declspec(dllexport)
	#else
		#define PROPERTY_MODULE_CLASS _declspec(dllimport)
	#endif
#endif

//ģ�鶨��
#ifdef _DEBUG
	#define PROPERTY_MODULE_DLL_NAME TEXT("PropertyModuleD.dll")
#else
	#define PROPERTY_MODULE_DLL_NAME TEXT("PropertyModule.dll")
#endif

//////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IPurchaseInfo INTERFACE_VERSION(1,1)
	static const GUID IID_IPurchaseInfo={0xaa45290b,0xb9f1,0x4469,0x00a1,0x7b,0x04,0xcc,0xb1,0x77,0x61,0x7d};
#else
	#define VER_IPurchaseInfo INTERFACE_VERSION(1,1)
	static const GUID IID_IPurchaseInfo={0xdc5ae116,0x48a5,0x48f7,0x008e,0x18,0xfd,0xb5,0x46,0x2f,0xbc,0xc4};
#endif

//////////////////////////////////////////////////////////////////////////

//�ʻ��ӿ�
interface IPurchaseInfo: public IUnknownEx
{
	//��������
public:
	//������
	virtual BYTE __cdecl GetLocation()=NULL;
	//��������
	virtual WORD __cdecl GetGameGenre()=NULL;
	//ö�����
	virtual tagUserData * __cdecl EnumLookonUser(WORD wIndex)=NULL;
	//�����û�
	virtual tagUserData * __cdecl SearchUserItem(DWORD dwUserID)=NULL;
	//�����û�
	virtual tagUserData * __cdecl SearchUserItem(LPCTSTR pszUserName)=NULL;
	//��ȡ���
	virtual const tagUserData * __cdecl GetUserInfo(WORD wChairID)=NULL;
	//��ȡ�Լ�
	virtual const tagUserData * __cdecl GetMeUserInfo()=NULL;
	//������Ϣ
	virtual void __cdecl SendData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////

//������ͷ�ļ� 
#include "SingleProItem.h"
#include "CharmValueExchange.h"
#include "BankStorage.h"
#include "BankServiceDlg.h"

//////////////////////////////////////////////////////////////////////////
#endif