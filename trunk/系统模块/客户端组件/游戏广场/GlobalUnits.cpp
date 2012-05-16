#include "StdAfx.h"
#include "Mmsystem.h"
#include "GlobalUnits.h"


//////////////////////////////////////////////////////////////////////////

//��������
#define DTP_GAME_CLIENT			1									//��������

//״̬��Ϣ
struct tagAstatInfo
{
	ADAPTER_STATUS				AdapterStatus;						//����״̬
	NAME_BUFFER					NameBuff[16];						//���ֻ���
};

//ȫ�ֱ���
CGlobalUnits					g_GlobalUnits;						//��Ϣ���			
CGlobalAttemper					g_GlobalAttemper;					//ȫ�ֵ���

//////////////////////////////////////////////////////////////////////////

//���캯��
CGlobalUnits::CGlobalUnits(void)
{
	m_szDirSkin[0]=0;
	m_szDirWork[0]=0;
	m_dwPlazaVersion=VER_PLAZA_FRAME;
	memset(&m_GloblaUserData,0,sizeof(m_GloblaUserData));
	lstrcpyn(m_szStationPage,szStationPage,sizeof(m_szStationPage));

	//��Դ����
	ZeroMemory(&m_PlazaViewImage,sizeof(m_PlazaViewImage));
	ZeroMemory(&m_ServerViewImage,sizeof(m_ServerViewImage));
	ZeroMemory(&m_WhisperViewImage,sizeof(m_WhisperViewImage));
	ZeroMemory(&m_ControlViewImage,sizeof(m_ControlViewImage));
	ZeroMemory(&m_PlatformFrameImage,sizeof(m_PlatformFrameImage));
	ZeroMemory(&m_ChatExpViewImage,sizeof(m_ChatExpViewImage));
	ZeroMemory(&m_PropertyViewImage,sizeof(m_PropertyViewImage));

	return;
}

//��������
CGlobalUnits::~CGlobalUnits(void)
{
}

//��ʼ������
bool CGlobalUnits::InitGlobalUnits()
{
	//�����������
	srand((UINT)time(NULL));

	//���ù���Ŀ¼
	GetModuleFileName(AfxGetInstanceHandle(),m_szDirWork,sizeof(m_szDirWork));
	int nModuleLen=lstrlen(m_szDirWork);
	int nProcessLen=lstrlen(AfxGetApp()->m_pszExeName)+lstrlen(TEXT(".EXE"));
	m_szDirWork[nModuleLen-nProcessLen]=0;
	SetCurrentDirectory(m_szDirWork);

	//���ý���Ŀ¼
	_snprintf(m_szDirSkin,sizeof(m_szDirSkin),TEXT("%s\\Skin"),m_szDirWork);

	//�������
	if (m_UserFaceRes.CreateInstance()==false) return false;
	if (m_DownLoadService.CreateInstance()==false) return false;
	if (m_CompanionManager.CreateInstance()==false) return false;
	if (m_PlatformResourceModule.CreateInstance()==false) return false;

	//��ȡ��Դ
	m_PlatformResourceModule->GetPlazaViewImage(&m_PlazaViewImage);
	m_PlatformResourceModule->GetServerViewImage(&m_ServerViewImage);
	m_PlatformResourceModule->GetWhisperViewImage(&m_WhisperViewImage);
	m_PlatformResourceModule->GetControlViewImage(&m_ControlViewImage);
	m_PlatformResourceModule->GetPlatformFrameImage(&m_PlatformFrameImage);
	m_PlatformResourceModule->GetChatExpViewImage(&m_ChatExpViewImage);
	m_PlatformResourceModule->GetPropertyViewImage(&m_PropertyViewImage);

	//�ؼ���Դ
	//IPlatformResource * pIPlatformResource=m_PlatformResourceModule.GetInterface();
	ISkinResource * pISkinResource=(ISkinResource *)m_PlatformResourceModule->GetSkinResource(IID_ISkinResource,VER_ISkinResource);
	if (CSkinResourceManager::LoadSkinResource(pISkinResource)==false) return false;

	//��ʼ�����
	m_CompanionManager->LoadCompanion();

	return true;
}

//�����ַ�
bool CGlobalUnits::CopyToClipboard(LPCTSTR pszString)
{
	//��������
	HWND hWnd=AfxGetMainWnd()->GetSafeHwnd();

	//�򿪼��а�
	if (OpenClipboard(hWnd)==FALSE) return false;
	if (EmptyClipboard()==FALSE) { CloseClipboard(); return false; }

	//��������
	HANDLE hData=GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT,CountStringBuffer(pszString));
	if (hData==NULL) 
	{
		CloseClipboard();
		return false;
	}

	//��������
	LPSTR pszMemString=(LPSTR)GlobalLock(hData);
	lstrcpy(pszMemString,pszString);
	SetClipboardData(CF_TEXT,hData);

	//��������
	GlobalUnlock(hData);
	CloseClipboard();

	return true;
}

//��ȡ����
void CGlobalUnits::GetClientSerial(tagClientSerial & ClientSerial)
{
	//��ȡ�汾
	OSVERSIONINFO OSVerInfo;
	OSVerInfo.dwOSVersionInfoSize=sizeof(OSVerInfo);
	GetVersionEx(&OSVerInfo);
	ClientSerial.dwSystemVer=MAKELONG(OSVerInfo.dwMajorVersion,OSVerInfo.dwMinorVersion);

	//������ʶ
	ZeroMemory(ClientSerial.dwComputerID,sizeof(ClientSerial.dwComputerID));
	WORD wMacSize=GetMACAddress((BYTE *)ClientSerial.dwComputerID,sizeof(ClientSerial.dwComputerID));

	//Ӳ�̱�ʶ
	WORD wIndex=(wMacSize+sizeof(DWORD)-1)/sizeof(DWORD);
	LPCTSTR pszHardDisk[]={TEXT("C:\\"),TEXT("D:\\"),TEXT("E:\\")};
	for (WORD i=wIndex;i<CountArray(ClientSerial.dwComputerID);i++)
	{
		ASSERT(CountArray(pszHardDisk)>(i-wIndex));
		GetVolumeInformation(pszHardDisk[i-wIndex],NULL,0,&ClientSerial.dwComputerID[i],NULL,NULL,0,NULL);
	}

	return;
}

//������վ��ַ
LPCTSTR CGlobalUnits::SetStationPage(LPCTSTR pszStationPage)
{
	ASSERT(pszStationPage!=NULL);
	lstrcpyn(m_szStationPage,pszStationPage,sizeof(m_szStationPage));
	return m_szStationPage;
}

//��¼���
bool CGlobalUnits::WriteUserCookie()
{
	//��������
	TCHAR szTokenData[128]=TEXT("");
	INT nCharCount=sizeof(szTokenData)/sizeof(szTokenData[0]);

	//�û� I D
	_sntprintf(szTokenData,nCharCount,TEXT("%ld"),m_GloblaUserData.dwUserID);
	CInternetSession::SetCookie(szCookieUrl,TEXT("UserID"),szTokenData);

	//��Ϸ I D
	_sntprintf(szTokenData,nCharCount,TEXT("%ld"),m_GloblaUserData.dwGameID);
	CInternetSession::SetCookie(szCookieUrl,TEXT("GameID"),szTokenData);

	//�û��ʺ�
	CInternetSession::SetCookie(szCookieUrl,TEXT("Accounts"),m_GloblaUserData.szAccounts);

	//�û�����
	CInternetSession::SetCookie(szCookieUrl,TEXT("Password"),m_GloblaUserData.szPassWord);

	return true;
}

//ɾ�����
bool CGlobalUnits::DeleteUserCookie()
{
	//ɾ����Ϣ
	CInternetSession::SetCookie(szCookieUrl,TEXT("UserID"),TEXT(""));
	CInternetSession::SetCookie(szCookieUrl,TEXT("GameID"),TEXT(""));
	CInternetSession::SetCookie(szCookieUrl,TEXT("Accounts"),TEXT(""));
	CInternetSession::SetCookie(szCookieUrl,TEXT("Password"),TEXT(""));
	CInternetSession::SetCookie(szCookieUrl,TEXT("UserToken"),TEXT(""));

	return true;
}

//������ַ
WORD CGlobalUnits::GetMACAddress(BYTE cbMacBuffer[], WORD wBufferSize)
{
	//��������
	WORD wMacSize=0;
	HINSTANCE hInstance=NULL;
	typedef BYTE __stdcall NetBiosProc(NCB * Ncb);

	try
	{
		//���� DLL
		hInstance=LoadLibrary(TEXT("NetApi32.dll"));
		if (hInstance==NULL) throw TEXT("���� NetApi32.dll ʧ��");

		//��ȡ����
		NetBiosProc * pNetBiosProc=(NetBiosProc *)GetProcAddress(hInstance,"Netbios");
		if (pNetBiosProc==NULL) throw TEXT("�޷��ҵ� NetBios ����");

		//��������
		NCB Ncb;
		LANA_ENUM LanaEnum;
		ZeroMemory(&Ncb,sizeof(Ncb));
		ZeroMemory(&LanaEnum,sizeof(LanaEnum));

		//ö������
		Ncb.ncb_command=NCBENUM;
		Ncb.ncb_length=sizeof(LanaEnum);
		Ncb.ncb_buffer=(BYTE *)&LanaEnum;
		if (pNetBiosProc(&Ncb)!=NRC_GOODRET) throw TEXT("ö������ʧ��");
		if (LanaEnum.length==0) throw TEXT("�Ҳ����κ�����");

		//��ȡ��ַ
		tagAstatInfo Adapter;
		for (BYTE i=0;i<LanaEnum.length;i++)
		{
			//��������
			ZeroMemory(&Ncb,sizeof(Ncb));
			Ncb.ncb_command=NCBRESET;
			Ncb.ncb_lana_num=LanaEnum.lana[i];
			if (pNetBiosProc(&Ncb)!=NRC_GOODRET) throw TEXT("��������ʧ��");

			//��ȡ״̬
			ZeroMemory(&Ncb,sizeof(Ncb));
			Ncb.ncb_command=NCBASTAT;
			Ncb.ncb_length=sizeof(Adapter);
			Ncb.ncb_buffer=(BYTE *)&Adapter;
			Ncb.ncb_lana_num=LanaEnum.lana[i];
			strcpy((char *)Ncb.ncb_callname,"*");
			if (pNetBiosProc(&Ncb)!=NRC_GOODRET) throw TEXT("��ȡ����״̬ʧ��");

			//��ȡ��ַ
			if (wMacSize<wBufferSize)
			{
				//������ַ
				WORD wCopySize=__min((wBufferSize-wMacSize),sizeof(BYTE)*6);
				CopyMemory(cbMacBuffer,Adapter.AdapterStatus.adapter_address,wCopySize);

				//����ж�
				wMacSize+=wCopySize;
				if (wMacSize==wBufferSize) break;
			}
		}

		//�ͷ���Դ
		if (hInstance!=NULL)
		{
			FreeLibrary(hInstance);
			hInstance=NULL;
		}

		return wMacSize;
	}
	catch (...) 
	{ 
		//�ͷ���Դ
		if (hInstance!=NULL)
		{
			FreeLibrary(hInstance);
			hInstance=NULL;
		}
	}

	return wMacSize;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CGlobalAttemper::CGlobalAttemper(void)
{
}

//��������
CGlobalAttemper::~CGlobalAttemper(void)
{
}

//��ʾ��ʾ
bool CGlobalAttemper::ShowStatusMessage(LPCTSTR pszMessage, CWnd * pNotifyWnd)
{
	return m_DlgStatus.ShowStatusMessage(pszMessage,pNotifyWnd);
}

//�ر�״̬
bool CGlobalAttemper::DestroyStatusWnd(CWnd * pNotifyWnd)
{
	return m_DlgStatus.DestroyStatusWnd(pNotifyWnd);
}

//������Ϸ
bool CGlobalAttemper::DownLoadClient(LPCTSTR pszKindName, WORD wKindID, bool bDisplay)
{
	//��������
	tagDownLoadRequest DownLoadRequest;
	memset(&DownLoadRequest,0,sizeof(DownLoadRequest));
	DownLoadRequest.bDisplay=bDisplay;
	lstrcpyn(DownLoadRequest.szDescribe,pszKindName,CountArray(DownLoadRequest.szDescribe));
	_snprintf(DownLoadRequest.szFileName,sizeof(DownLoadRequest.szFileName),TEXT("%s.EXE"),pszKindName);
	_snprintf(DownLoadRequest.szLocalPath,sizeof(DownLoadRequest.szLocalPath),TEXT("%s\\DownLoad"),g_GlobalUnits.GetWorkDirectory());
	_snprintf(DownLoadRequest.szDownLoadUrl,sizeof(DownLoadRequest.szDownLoadUrl),TEXT("http://221.234.25.72/Download.asp?KindID=%ld&LocalVersion=0&PlazaVersion=%ld"),wKindID,VER_PLAZA_FRAME);

	//Ͷ������
	DWORD dwDownLoadID=g_GlobalUnits.m_DownLoadService->AddDownLoadRequest(DTP_GAME_CLIENT,&DownLoadRequest);

	return true;
}

//ע���ȼ�
bool CGlobalUnits::RegisterHotKey(HWND hWnd, UINT uKeyID, WORD wHotKey)
{
	//��������
	BYTE cbModifiers=0;
	if (HIBYTE(wHotKey)&HOTKEYF_ALT) cbModifiers|=MOD_ALT;
	if (HIBYTE(wHotKey)&HOTKEYF_SHIFT) cbModifiers|=MOD_SHIFT;
	if (HIBYTE(wHotKey)&HOTKEYF_CONTROL) cbModifiers|=MOD_CONTROL;

	//ע���ȼ�
	BOOL bSuccess=::RegisterHotKey(hWnd,uKeyID,cbModifiers,LOBYTE(wHotKey));

	return (bSuccess==TRUE)?true:false;
}


//////////////////////////////////////////////////////////////////////////
