#ifndef CLIENT_SHARE_HEAD_FILE
#define CLIENT_SHARE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////

//ƽ̨ͷ�ļ�
#include "Template.h"

//���ͷ�ļ�
#include "SkinControls.h"

//////////////////////////////////////////////////////////////////////////
//�����궨��

//��������
#ifndef CLIENT_SHARE_CLASS
	#ifdef  CLIENT_SHARE_DLL
		#define CLIENT_SHARE_CLASS _declspec(dllexport)
	#else
		#define CLIENT_SHARE_CLASS _declspec(dllimport)
	#endif
#endif

//ģ�鶨��
#ifdef _DEBUG
	#define CLIENT_SHARE_DLL_NAME	TEXT("ClientShareD.dll")			//��� DLL ����
#else
	#define CLIENT_SHARE_DLL_NAME	TEXT("ClientShare.dll")				//��� DLL ����
#endif

//////////////////////////////////////////////////////////////////////////
//�ṹ�嶨��

//��Ϣ����
struct tagMessageOption
{
	COLORREF							crName;							//������ɫ
	COLORREF							crSystemHead;					//ϵͳ��ɫ
	COLORREF							crSystemString;					//ϵͳ��ɫ
	TCHAR								szDefaultFont[32];				//Ĭ������
};

//��Ϣ��С
#define MS_NORMAL						0								//�����С
#define MS_MIDDLING						1								//�еȴ�С
#define MS_LARGE						2								//�������

//////////////////////////////////////////////////////////////////////////

#define VER_IMessageProxy INTERFACE_VERSION(1,1)
static const GUID IID_IMessageProxy={0xe8bd2387,0x9c2a,0x4346,0xb3,0x2c,0x17,0xb7,0xb3,0x51,0x9a,0x2f};

//��Ϣ����ӿ�
interface IMessageProxy : public IUnknownEx
{
	//���ýӿ�
public:
	//���þ��
	virtual bool __cdecl SetRichEditHwnd(CSkinRichEdit * pSkinRichEdit)=NULL;
	//������ɫ
	virtual bool __cdecl SetMessageOption(tagMessageOption & MessageOption)=NULL;

	//���ƽӿ�
public:
	//�����Ļ
	virtual bool __cdecl CleanScreen()=NULL;
	//���ù���
	virtual bool __cdecl SetAutoScroll(bool bScroll)=NULL;
	//������Ϣ
	virtual bool __cdecl LoadMessage(LPCTSTR pszFileName)=NULL;
	//������Ϣ
	virtual bool __cdecl SaveMessage(LPCTSTR pszFileName)=NULL;
	//��ȡָ��
	virtual CSkinRichEdit * __cdecl GetRichEdit()=NULL;

	//��Ϣ�ӿ�
public:
	//����ͼƬ
	virtual bool __cdecl InsertImage(CBitmap * pBitmap)=NULL;
	//����ʱ��
	virtual bool __cdecl InsertSystemTime(COLORREF crColor)=NULL;
	//�û�����
	virtual bool __cdecl InsertUserName(LPCTSTR pszUserName)=NULL;
	//ϵͳ��Ϣ
	virtual bool __cdecl InsertSystemString(LPCTSTR pszString, UINT uSize)=NULL;
	//��ɫ��Ϣ
	virtual bool __cdecl InsertShtickString(LPCTSTR pszString, COLORREF crColor, UINT uSize, bool bNewLine)=NULL;
	//��ͨ��Ϣ
	virtual bool __cdecl InsertGeneralString(LPCTSTR pszString, COLORREF crColor, UINT uSize, bool bNewLine)=NULL;
	//�û�����
	virtual bool __cdecl InsertUserChat(LPCTSTR pszSendName, LPCTSTR pszString, COLORREF crColor, UINT uSize)=NULL;
	//�û�����
	virtual bool __cdecl InsertUserChat(LPCTSTR pszSendName, LPCTSTR pszRecvName, LPCTSTR pszString, COLORREF crColor, UINT uSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////

//��Ϣ��������
class CMessageProxyHelper : public CTempldateHelper<IMessageProxy>
{
	//��������
public:
	//���캯��
	CMessageProxyHelper(void) : CTempldateHelper<IMessageProxy>(IID_IMessageProxy,
		VER_IMessageProxy,CLIENT_SHARE_DLL_NAME,TEXT("CreateMessageProxy")) { }
};

//////////////////////////////////////////////////////////////////////////
//������ͷ�ļ�

#include "Expression.h"
#include "HintMsgBox.h"

//////////////////////////////////////////////////////////////////////////

#endif