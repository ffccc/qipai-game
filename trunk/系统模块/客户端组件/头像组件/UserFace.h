#ifndef USER_FACE_HEAD_FILE
#define USER_FACE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////

//ƽ̨ͷ�ļ�
#include "..\..\ģ���\Template.h"

//////////////////////////////////////////////////////////////////////////
//�����궨��

//�궨��
#define LIT_FACE_WIDTH						16							//ͷ����
#define LIT_FACE_HEIGHT						16							//ͷ��߶�
#define NOR_FACE_WIDTH						32							//ͷ����
#define NOR_FACE_HEIGHT						32							//ͷ��߶�
#define BIG_FACE_WIDTH						100							//ͷ����
#define BIG_FACE_HEIGHT						100							//ͷ��߶�

//ģ�鶨��
#ifdef _DEBUG
	#define USER_FACE_DLL_NAME	TEXT("UserFaceD.dll")					//��� DLL ����
#else
	#define USER_FACE_DLL_NAME	TEXT("UserFace.dll")					//��� DLL ����
#endif

//////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IUserFaceRes INTERFACE_VERSION(1,1)
	static const GUID IID_IUserFaceRes={0x5ac6a7f0,0x1e38,0x49ef,0x91,0x2c,0x4f,0x7a,0x5b,0x03,0xdf,0xca};
#else
	#define VER_IUserFaceRes INTERFACE_VERSION(1,1)
	static const GUID IID_IUserFaceRes={0x567fb4df,0xd5b6,0x488b,0x90,0x40,0x90,0x17,0x15,0x10,0xf7,0x41};
#endif

//ͷ������ӿ�
interface IUserFaceRes : public IUnknownEx
{
	//��ȡ��Ŀ
	virtual WORD __cdecl GetFaceCount()=NULL;
	//��ȡ��С
	virtual const CSize & __cdecl GetNormalFaceSize()=NULL;
	//����б�
	virtual WORD __cdecl FillImageList(CImageList & m_ImageList)=NULL;
	//�滭ͷ��
	virtual void __cdecl DrawNormalFace(CDC * pDC, int nXPos, int nYPos, WORD wFaceID, DWORD dwUserID,DWORD dwCustomFace)=NULL;
	//�滭ͷ��
	virtual void __cdecl DrawCustomBigFace(CDC * pDC, int nXPos, int nYPos, DWORD dwUserID,DWORD dwCustomFace,UINT nWidth=BIG_FACE_WIDTH,UINT nHeight=BIG_FACE_HEIGHT)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IFaceControl INTERFACE_VERSION(1,1)
	static const GUID IID_IFaceControl={0xa728d07d,0xe453,0x46bd,0xb4,0x27,0x25,0x7e,0x2a,0xa0,0xc4,0xc4};
#else
	#define VER_IFaceControl INTERFACE_VERSION(1,1)
	static const GUID IID_IFaceControl={0x6b0e8af2,0x0e5b,0x4024,0xb8,0x2c,0x1e,0x8e,0x17,0xac,0x6b,0x2e};
#endif

//ͷ��ؼ��ӿ�
interface IFaceControl : public IUnknownEx
{
	//��Ϣ�ӿ�
public:
	//ͷ����
	virtual int __cdecl GetFaceWidth()=NULL;
	//ͷ��߶�
	virtual int __cdecl GetFaceHeight()=NULL;
	//ͷ����Ŀ
	virtual WORD __cdecl GetFaceItemCount()=NULL;

	//�滭�ӿ�
public:
	//�滭ͷ��
	virtual void __cdecl DrawFaceItem(CDC * pDC, int nXPos, int nYPos, WORD wFaceID)=NULL;
};

//////////////////////////////////////////////////////////////////////////

//ͷ�����������
class CUserFaceResHelper : public CTempldateHelper<IUserFaceRes>
{
	//��������
public:
	//���캯��
	CUserFaceResHelper(void) : CTempldateHelper<IUserFaceRes>(IID_IUserFaceRes,
		VER_IUserFaceRes,USER_FACE_DLL_NAME,TEXT("CreateUserFaceRes"))
	{
	}
};

//////////////////////////////////////////////////////////////////////////

//ͷ��ؼ�������
class CFaceControlHelper : public CTempldateHelper<IFaceControl>
{
	//��������
public:
	//���캯��
	CFaceControlHelper(void) : CTempldateHelper<IFaceControl>(IID_IFaceControl,
		VER_IFaceControl,USER_FACE_DLL_NAME,TEXT("CreateFaceControl"))
	{
	}
};

//////////////////////////////////////////////////////////////////////////

#endif