#ifndef AVATAR_CONTROL_HEAD_HEAD_FILE
#define AVATAR_CONTROL_HEAD_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////

//ϵͳ�ļ�
#include <VFW.h>

//ƽ̨�ļ�
#include "..\..\ȫ�ֶ���\Platform.h"

//����ļ�
#include "..\..\�ͻ������\ͼ��ؼ�\WHImageHead.h"
#include "..\..\�ͻ������\�������\DownLoadHead.h"
#include "..\..\�ͻ������\����ؼ�\SkinControlHead.h"
#include "..\..\�ͻ������\��Ϸ�ؼ�\ShareControlHead.h"

//////////////////////////////////////////////////////////////////////////////////
//��������

//��������
#ifndef AVATAR_CONTROL_CLASS
	#ifdef  AVATAR_CONTROL_DLL
		#define AVATAR_CONTROL_CLASS _declspec(dllexport)
	#else
		#define AVATAR_CONTROL_CLASS _declspec(dllimport)
	#endif
#endif

//ģ�鶨��
#ifndef _DEBUG
	#define AVATAR_CONTROL_DLL_NAME	TEXT("AvatarControl.dll")			//�������
#else
	#define AVATAR_CONTROL_DLL_NAME	TEXT("AvatarControlD.dll")			//�������
#endif

//////////////////////////////////////////////////////////////////////////////////

//�������
#define ITEM_FRAME_NORMAL			0									//Ĭ�Ͽ��
#define ITEM_FRAME_HOVER			1									//Ĭ�Ͽ��
#define ITEM_FRAME_HITDOWN			2									//Ĭ�Ͽ��

//////////////////////////////////////////////////////////////////////////////////
//�ṹ����

//ͷ����Դ
struct tagAvatarResource
{
	LPCTSTR							pszResource;						//��Դ����
	HINSTANCE						hResInstance;						//��Դ���
};

//ͷ������
struct tagCustomFaceIndex
{
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwCustomID;							//�Զ���ʶ
};

//�༭���
struct tagCustomFaceResult
{
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwCustomID;							//�Զ���ʶ
	DWORD							dwCustomFace[FACE_CX*FACE_CY];		//ͼƬ��Ϣ
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IFaceItemControl INTERFACE_VERSION(1,1)
	static const GUID IID_IFaceItemControl={0xfc9067a9,0x162e,0x4e13,0xb0,0x01,0xf7,0x57,0x9e,0xa4,0x60,0x6b};
#else
	#define VER_IFaceItemControl INTERFACE_VERSION(1,1)
	static const GUID IID_IFaceItemControl={0xaab85fd0,0x2483,0x4384,0xbb,0xe0,0x7d,0x5d,0x82,0x30,0xa7,0x1c};
#endif

//ͷ�����
interface IFaceItemControl : public IUnknownEx
{
	//��Ϣ�ӿ�
public:
	//ͷ����Ŀ
	virtual WORD GetFaceCount()=NULL;
	//��ȡ�ߴ�
	virtual VOID GetFaceItemSize(CSize & SizeFace)=NULL;

	//��Դ�ӿ�
public:
	//��ȡ��Դ
	virtual VOID GetAvatarResource(tagAvatarResource & AvatarResource)=NULL;
	//�滭���
	virtual VOID DrawFaceItemFrame(CDC * pDC, INT nXPos, INT nYPos, BYTE cbFrameKind)=NULL;

	//�滭�ӿ�
public:
	//�滭ͷ��
	virtual VOID DrawFaceNormal(CDC * pDC, INT nXPos, INT nYPos, WORD wFaceID)=NULL;
	//�滭ͷ��
	virtual VOID DrawFaceOffLine(CDC * pDC, INT nXPos, INT nYPos, WORD wFaceID)=NULL;

	//�滭�ӿ�
public:
	//�滭ͷ��
	virtual VOID DrawFaceNormal(CDC * pDC, INT nXPos, INT nYPos, DWORD dwCustomFace[FACE_CX*FACE_CY])=NULL;
	//�滭ͷ��
	virtual VOID DrawFaceOffLine(CDC * pDC, INT nXPos, INT nYPos, DWORD dwCustomFace[FACE_CX*FACE_CY])=NULL;;

	//�滭�ӿ�
public:
	//�滭ͷ��
	virtual VOID DrawFaceNormal(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight, WORD wFaceID)=NULL;
	//�滭ͷ��
	virtual VOID DrawFaceOffLine(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight, WORD wFaceID)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IFaceSelectControl INTERFACE_VERSION(1,1)
	static const GUID IID_IFaceSelectControl={0x8514020b,0x82a2,0x42d9,0x90,0x19,0x32,0xa4,0xea,0xce,0x9f,0x75};
#else
	#define VER_IFaceSelectControl INTERFACE_VERSION(1,1)
	static const GUID IID_IFaceSelectControl={0xf153d63f,0x8ed7,0x454c,0x8b,0x0c,0xcc,0x4b,0x12,0xc9,0xe1,0x2e};
#endif

//ѡ�����
interface IFaceSelectControl : public IUnknownEx
{
	//����ѡ��
public:
	//ѡ����Ŀ
	virtual VOID SetAllowItemFull()=NULL;
	//ѡ����Ŀ
	virtual VOID SetAllowItemCount(WORD wFaceCount)=NULL;

	//��ȡѡ��
public:
	//��ȡѡ��
	virtual bool GetSelectFaceID(WORD & wFaceID)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ICustomFaceEvent INTERFACE_VERSION(1,1)
	static const GUID IID_ICustomFaceEvent={0xaf4303d8,0x5cfd,0x4d62,0x82,0x78,0xd4,0xdf,0x1f,0xa9,0x73,0x2c};
#else
	#define VER_ICustomFaceEvent INTERFACE_VERSION(1,1)
	static const GUID IID_ICustomFaceEvent={0xfceb44d0,0x330e,0x4a9c,0xa6,0x42,0x38,0x82,0xd5,0x1b,0x0a,0x13};
#endif

//����ӿ�
interface ICustomFaceEvent : public IUnknownEx
{
	//ͷ������
	virtual VOID OnEventSystemFace(DWORD dwUserID, WORD wFaceID)=NULL;
	//ͷ������
	virtual VOID OnEventCustomFace(DWORD dwUserID, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ICustomFaceManager INTERFACE_VERSION(1,1)
	static const GUID IID_ICustomFaceManager={0xbcd7ff05,0x0a79,0x4482,0x9b,0xa6,0x79,0xdf,0x4b,0xd9,0x80,0x16};
#else
	#define VER_ICustomFaceManager INTERFACE_VERSION(1,1)
	static const GUID IID_ICustomFaceManager={0xe5694b07,0x888e,0x45aa,0x85,0xa7,0xfe,0x89,0x84,0x1b,0x79,0x20};
#endif

//ͷ�����
interface ICustomFaceManager : public IUnknownEx
{
	//���ýӿ�
	virtual bool SetCustomFaceEvent(IUnknownEx * pIUnknownEx)=NULL;
	//����ͷ��
	virtual bool LoadUserCustomFace(DWORD dwUserID, DWORD dwCustomID)=NULL;
	//����ͷ��
	virtual bool LoadUserCustomFace(DWORD dwUserID, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo)=NULL;
	//����ͷ��
	virtual bool SaveUserCustomFace(DWORD dwUserID, DWORD dwCustomID, DWORD dwCustomFace[FACE_CX*FACE_CY])=NULL;
};

//////////////////////////////////////////////////////////////////////////////////
//���������

DECLARE_MODULE_HELPER(FaceItemControl,AVATAR_CONTROL_DLL_NAME,"CreateFaceItemControl")
DECLARE_MODULE_HELPER(FaceSelectControl,AVATAR_CONTROL_DLL_NAME,"CreateFaceSelectControl")
DECLARE_MODULE_HELPER(CustomFaceManager,AVATAR_CONTROL_DLL_NAME,"CreateCustomFaceManager")

//////////////////////////////////////////////////////////////////////////////////

//����ļ�
#ifndef AVATAR_CONTROL_DLL
	#include "FaceItemView.h"
	#include "FaceItemControl.h"
	#include "CustomFaceManager.h"
	#include "FaceSelectControl.h"
	#include "FaceCustomControl.h"
	#include "ImageEditorControl.h"
#endif
//////////////////////////////////////////////////////////////////////////////////

#endif