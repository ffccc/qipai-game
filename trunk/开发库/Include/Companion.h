#ifndef COMPANION_HEAD_FILE
#define COMPANION_HEAD_FILE

//////////////////////////////////////////////////////////////////////////

//ƽ̨ͷ�ļ�
#include "Template.h"

//ȫ��ͷ�ļ�
#include "GlobalDef.h"

//���ͷ�ļ�
#include "SkinControls.h"

//////////////////////////////////////////////////////////////////////////
//�����궨��

//��������
#ifndef COMPANION_CLASS
	#ifdef  COMPANION_DLL
		#define COMPANION_CLASS _declspec(dllexport)
	#else
		#define COMPANION_CLASS _declspec(dllimport)
	#endif
#endif

//ģ�鶨��
#ifdef _DEBUG
	#define COMPANION_DLL_NAME	TEXT("CompanionD.dll")					//��� DLL ����
#else
	#define COMPANION_DLL_NAME	TEXT("Companion.dll")					//��� DLL ����
#endif

//////////////////////////////////////////////////////////////////////////
//ö�ٶ���

//��ϵ����
enum enCompanion
{
	enCompanion_UnKnow,				//δ֪��ϵ
	enCompanion_Friend,				//���ѹ�ϵ
	enCompanion_Detest,				//����ϵ
};

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//���ȶ���
#define CP_NOTE_LEN					128									//��ע����

//�û���¼�ṹ
struct tagCompanionItem
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	WORD							wFaceID;							//ͷ�� I D
	enCompanion						Companion;							//�û���ϵ
	TCHAR							szNote[CP_NOTE_LEN];				//�û���ע
	TCHAR							szAccounts[NAME_LEN];				//�û��˺�
};

//////////////////////////////////////////////////////////////////////////

#define VER_ICompanionManager INTERFACE_VERSION(2,1)
static const GUID IID_ICompanionManager={0x7bef6deb,0x5915,0x4021,0xa1,0x4f,0xa9,0xa3,0xec,0xad,0x74,0xf3};

//��ϵ����ӿ�
interface ICompanionManager : public IUnknownEx
{
	//����ӿ�
public:
	//������Ϣ
	virtual bool __cdecl LoadCompanion()=NULL;
	//�����ϵ
	virtual bool __cdecl SaveCompanion()=NULL;
	//��ʾ����
	virtual bool __cdecl ShowCompanionManager()=NULL;
	//��ϵ��Ŀ
	virtual INT_PTR __cdecl GetCompanionItemCount()=NULL;

	//���ܽӿ�
public:
	//�����ϵ
	virtual const tagCompanionItem * __cdecl InsertCompanionItem(tagCompanionItem & CompanionItem)=NULL;
	//���¹�ϵ
	virtual const tagCompanionItem * __cdecl UpdateCompanionItem(tagCompanionItem & CompanionItem)=NULL;
	//���¹�ϵ
	virtual const tagCompanionItem * __cdecl UpdateCompanionItem(DWORD dwUserID, LPCTSTR pszNote)=NULL;
	//���¹�ϵ
	virtual const tagCompanionItem * __cdecl UpdateCompanionItem(DWORD dwUserID, LPCTSTR pszAccounts, WORD wFaceID)=NULL;
	//ɾ����ϵ
	virtual bool __cdecl DeleteCompanionItem(DWORD dwUserID)=NULL;

	//���ҽӿ�
public:
	//ö�ٹ�ϵ
	virtual const tagCompanionItem * __cdecl EmunCompanionItem(WORD wIndex)=NULL;
	//Ѱ�ҹ�ϵ
	virtual const tagCompanionItem * __cdecl SearchCompanionItem(DWORD dwUserID)=NULL;
};

//////////////////////////////////////////////////////////////////////////

//��ϵ��������
class CCompanionManagerHelper : public CTempldateHelper<ICompanionManager>
{
	//��������
public:
	//���캯��
	CCompanionManagerHelper(void) : CTempldateHelper<ICompanionManager>(IID_ICompanionManager,
		VER_ICompanionManager,COMPANION_DLL_NAME,TEXT("CreateCompanionManager")) { }
};

//////////////////////////////////////////////////////////////////////////

#endif