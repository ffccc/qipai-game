#ifndef DOWN_LOAD_HEAD_FILE
#define DOWN_LOAD_HEAD_FILE

//////////////////////////////////////////////////////////////////////////

//ƽ̨ͷ�ļ�
#include "..\..\ģ���\Template.h"

//���ͷ�ļ�
#include "..\..\�������\��������\ComService.h"
#include "..\..\�������\����ؼ�\SkinControls.h"

//////////////////////////////////////////////////////////////////////////
//�����궨��

//��������
#ifndef IDOWN_LOAD_CLASS
	#ifdef  IDOWN_LOAD_DLL
		#define IDOWN_LOAD_CLASS _declspec(dllexport)
	#else
		#define IDOWN_LOAD_CLASS _declspec(dllimport)
	#endif
#endif

//ģ�鶨��
#ifdef _DEBUG
	#define IDOWN_LOAD_DLL_NAME	TEXT("DownLoadD.dll")					//��� DLL ����
#else
	#define IDOWN_LOAD_DLL_NAME	TEXT("DownLoad.dll")					//��� DLL ����
#endif

//////////////////////////////////////////////////////////////////////////
//�ṹ�嶨��

//��������
struct tagDownLoadRequest
{
	bool							bDisplay;							//�Ƿ���ʾ
	TCHAR							szDescribe[128];					//������Ϣ
	TCHAR							szFileName[MAX_PATH];				//�����ļ�
	TCHAR							szLocalPath[MAX_PATH];				//����·��
	TCHAR							szDownLoadUrl[MAX_PATH];			//����·��
};

//////////////////////////////////////////////////////////////////////////

#define VER_IDownLoadService INTERFACE_VERSION(3,1)
static const GUID IID_IDownLoadService={0xe0b37184,0x6fbe,0x44ac,0xa4,0x83,0xab,0xa6,0x89,0x9b,0x33,0xd4};

//���ط���ӿ�
interface IDownLoadService : public IUnknownEx
{
	//��ȡ��Ŀ
	virtual INT_PTR __cdecl GetDownLoadMissionCount()=NULL;
	//���ýӿ�
	virtual bool __cdecl SetDownLoadServiceSink(IUnknownEx * pIUnknownEx)=NULL;
	//��������
	virtual DWORD __cdecl AddDownLoadRequest(DWORD dwDownLoadType, tagDownLoadRequest * pDownLoadRequest)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_IDownLoadServiceSink INTERFACE_VERSION(1,1)
static const GUID IID_IDownLoadServiceSink={0xb339b588,0xd328,0x4482,0x96,0xde,0xc4,0x12,0xdd,0x78,0x81,0xec};

//���ط���ص��ӿ�
interface IDownLoadServiceSink : public IUnknownEx
{
};

//////////////////////////////////////////////////////////////////////////

//���ط�������
class CDownLoadServiceHelper : public CTempldateHelper<IDownLoadService>
{
	//��������
public:
	//���캯��
	CDownLoadServiceHelper(void) : CTempldateHelper<IDownLoadService>(IID_IDownLoadService,
		VER_IDownLoadService,IDOWN_LOAD_DLL_NAME,TEXT("CreateDownLoadService"))
	{
	}
};

//////////////////////////////////////////////////////////////////////////

#endif