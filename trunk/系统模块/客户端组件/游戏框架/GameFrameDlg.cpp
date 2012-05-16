#include "Stdafx.h"
#include "Mmsystem.h"
#include "Resource.h"
#include "GameFrameDlg.h"
#include "GlobalOption.h"
#include "GameFrameControl.h"
#include "ClientKernelSink.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameFrameDlg, CSkinDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_GETMINMAXINFO()
	ON_WM_WINDOWPOSCHANGING()
	ON_BN_CLICKED(IDC_CONTROL, OnBnClickedControl)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameFrameDlg::CGameFrameDlg(CGameFrameView * pGameFrameView)  : CSkinDialog(IDD_GAME_FRAME), m_VorSplitter(VorSpliter)
{
	//����״̬
	m_bRectify=false;
	m_bHideList=false;
	m_rcNormalSize.SetRect(0,0,0,0);

	//���ñ���
	m_bReset=false;
	m_bGameSceen=false;
	m_bInitDialog=false;

	//����ӿ�
	m_pKernelSink=NULL;
	m_pGameFrameControl=NULL;
	m_pGameFrameView=pGameFrameView;

	//���ñ���
	m_bAllowSound=true;

	//��������
	m_pKernelSink=new CClientKernelSink(*this);

	return;
}

//��������
CGameFrameDlg::~CGameFrameDlg()
{
	SafeDelete(m_pKernelSink);
	SafeDelete(m_pGameFrameControl);
	return;
}

//��ť��Ϣ
bool CGameFrameDlg::OnSplitterButton(CSkinSplitter * pSkinSplitter, CSkinButton * pSplitterButton)
{
	return true;
}

//�������Ϣ
bool CGameFrameDlg::OnSplitterEvent(CSkinSplitter * pSkinSplitter, UINT uSplitID, int nXPos, int nYPos)
{
	//���ñ���
	m_bRectify=true;

	//��������
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());

	return true;
}

//��������
void CGameFrameDlg::RectifyControl(int nWidth, int nHeight)
{
	//״̬�ж�
	if (m_bInitDialog==false) return;

	//��������
	CRect rcVorSplitter;
	const int nSplitterWidth=6;
	const int iXExc=GetXExcursionPos();
	const int iYExc=GetYExcursionPos();
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;

	//��ȡλ��
	m_VorSplitter.GetWindowRect(&rcVorSplitter);
	ScreenToClient(&rcVorSplitter);

	//���������
	if (m_bRectify==false)
	{
		//���ñ���
		m_bRectify=true;

		//����λ��
		if (m_bHideList==false)
		{
			rcVorSplitter.left=nWidth*75/100;
			rcVorSplitter.right=rcVorSplitter.left+nSplitterWidth;
		}
		else
		{
			rcVorSplitter.right=nWidth;
			rcVorSplitter.left=nWidth-nSplitterWidth;
		}
	}

	//���ò�ַ�Χ
	if (m_bHideList==false)
	{
		int nLessPos=nWidth*3/5,nMaxPos=nWidth*75/100;
		m_VorSplitter.SetSplitterRange(nLessPos,nMaxPos);
		if ((nWidth!=0)&&(nHeight!=0))
		{
			if (rcVorSplitter.left<nLessPos) 
			{
				rcVorSplitter.left=nLessPos;
				rcVorSplitter.right=rcVorSplitter.left+nSplitterWidth;
			}
			if (rcVorSplitter.right>nMaxPos)
			{
				rcVorSplitter.right=nMaxPos;
				rcVorSplitter.left=rcVorSplitter.right-nSplitterWidth;
			}
		}
	}

	if (m_VorSplitter.GetSplitterType()==HorSpliter) rcVorSplitter.SetRect(nWidth,0,nWidth,nHeight);

	//������Ļ
	LockWindowUpdate();

	//�ƶ��ؼ�
	HDWP hDwp=BeginDeferWindowPos(32);
	DeferWindowPos(hDwp,m_pGameFrameView->GetSafeHwnd(),NULL,iXExc,iYExc,rcVorSplitter.left-iXExc,nHeight-iYExc,uFlags);
	DeferWindowPos(hDwp,m_VorSplitter,NULL,rcVorSplitter.left,iYExc,rcVorSplitter.Width(),nHeight-iYExc,uFlags);
	DeferWindowPos(hDwp,m_pGameFrameControl->GetSafeHwnd(),NULL,rcVorSplitter.right,iYExc,nWidth-rcVorSplitter.right,nHeight-iYExc,uFlags);
	EndDeferWindowPos(hDwp);

	//�ػ�����
	Invalidate(FALSE);
	UpdateWindow();

	//�������
	UnlockWindowUpdate();

	//���ư�ť
	CRect rcControl;
	m_btControl.GetClientRect(&rcControl);
	m_btControl.SetWindowPos(NULL,rcVorSplitter.left+(rcVorSplitter.Width()-rcControl.Width())/2,(nHeight-rcControl.Height())/2,0,0,uFlags|SWP_NOSIZE);

	//���ñ���
	m_bRectify=false;

	return;
}

//������Ϣ
bool CGameFrameDlg::OnFrameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	return false;
}

//�û�����
void __cdecl CGameFrameDlg::OnEventUserEnter(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
	return;
}

//�û��뿪
void __cdecl CGameFrameDlg::OnEventUserLeave(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
	return;
}

//�û�����
void __cdecl CGameFrameDlg::OnEventUserScore(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
	return;
}

//�û�״̬
void __cdecl CGameFrameDlg::OnEventUserStatus(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
	return;
}

//��ʼ����
void __cdecl CGameFrameDlg::OnEventInitProperty(tagPropertyInfo *pPropertyInfo, int nInfoCount)
{
	//��ȡ����
	CPropertyBar *pPropertyBar = CPropertyBar::GetInstance();
	ASSERT( pPropertyBar != NULL );
	if ( pPropertyBar == NULL ) return;

	//��ʼ�ṹ
	for ( int nIndex = 0; nIndex < nInfoCount; ++nIndex )
	{
		pPropertyBar->SetPropertyInfo((pPropertyInfo+nIndex)->nPropertyID, *(pPropertyInfo+nIndex));
	}

	return;
}

//��ʼ�ʻ�
void __cdecl CGameFrameDlg::OnEventInitFlower(tagFlowerInfo *pFlowerInfo, int nInfoCount)
{
	//��ȡ����
	CPropertyBar *pPropertyBar = CPropertyBar::GetInstance();
	ASSERT( pPropertyBar != NULL );
	if ( pPropertyBar == NULL ) return;

	//��ʼ�ṹ
	for ( int nIndex = 0; nIndex < nInfoCount; ++nIndex )
	{
		pPropertyBar->SetFlowerInfo((pFlowerInfo+nIndex)->nFlowerID, *(pFlowerInfo+nIndex));
	}

	return;
}

//�ʻ���Ϣ
void __cdecl CGameFrameDlg::OnEventFlower(const tagUserData * pSendUserData, const tagUserData * pRecvUserData, UINT uFlowerID, UINT uFlowerEffectID)
{
	//������ʾ
	m_pGameFrameView->ShowFlower(SwitchViewChairID(pSendUserData->wChairID), SwitchViewChairID(pRecvUserData->wChairID), uFlowerID, 
		uFlowerEffectID, m_ClientKernelHelper->GetMeChairID()==pRecvUserData->wChairID);

	return;
}

//�����б�
bool CGameFrameDlg::ShowViewControl(bool bShowControl)
{
	if (bShowControl!=m_bHideList)
	{
		//���ñ���
		m_bHideList=bShowControl;

		//��ȡλ��
		CRect rcClient;
		GetClientRect(&rcClient);

		//��������
		RectifyControl(rcClient.Width(),rcClient.Height());
	}

	return true;
}

//������Դ
bool CGameFrameDlg::UpdateSkinResource()
{
	//��ȡ��Դ
	HINSTANCE hInstance=g_GlobalOption.m_PlatformResourceModule->GetResInstance();
	tagGameFrameImage &GameFrameImage=g_GlobalOption.m_GameFrameImage;
	m_btControl.SetButtonImage(m_bHideList?GameFrameImage.uBtShowUserList:GameFrameImage.uBtHideUserList,hInstance,false);

	//���ư�ť
	UINT uImageID=m_bAllowSound?GameFrameImage.uBtGameSound:GameFrameImage.uBtGameUnSound;
	m_pGameFrameControl->m_btSound.SetButtonImage(uImageID,hInstance,false);

	//���ư�ť
	if (m_ClientKernelHelper.GetInterface()!=NULL) 
	{
		uImageID=m_ClientKernelHelper->IsAllowUserLookon()?GameFrameImage.uBtGameLookOn:GameFrameImage.uBtGameUnLookOn;
		m_pGameFrameControl->m_btLookOn.SetButtonImage(uImageID,hInstance,false);
	}

	return true;
}

//���ͺ���
bool CGameFrameDlg::SendData(WORD wSubCmdID)
{
	ASSERT(m_ClientKernelHelper.GetInterface()!=NULL);
	return m_ClientKernelHelper->SendSocketData(MDM_GF_GAME,wSubCmdID);
}

//���ͺ���
bool CGameFrameDlg::SendData(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	ASSERT(m_ClientKernelHelper.GetInterface()!=NULL);
	return m_ClientKernelHelper->SendSocketData(MDM_GF_GAME,wSubCmdID,pData,wDataSize);
}

//����׼��
bool CGameFrameDlg::SendUserReady(void * pBuffer, WORD wDataSize)
{
	ASSERT(m_ClientKernelHelper.GetInterface()!=NULL);
	return m_ClientKernelHelper->SendUserReady(pBuffer,wDataSize);
}

//���ö�ʱ��
bool CGameFrameDlg::SetGameTimer(WORD wChairID, UINT nTimerID, UINT nElapse)
{
	ASSERT(m_ClientKernelHelper.GetInterface()!=NULL);
	return m_ClientKernelHelper->SetGameTimer(wChairID,nTimerID,nElapse);
}

//ɾ����ʱ��
bool CGameFrameDlg::KillGameTimer(UINT nTimerID)
{
	ASSERT(m_ClientKernelHelper.GetInterface()!=NULL);
	return m_ClientKernelHelper->KillGameTimer(nTimerID);
}

//��������
bool CGameFrameDlg::PlayGameSound(LPCTSTR pszSoundName)
{
	if ((m_bAllowSound==true)&&(IsWindowVisible()))
	{
		::PlaySound(pszSoundName,AfxGetInstanceHandle(),SND_ASYNC|SND_NODEFAULT);
		return true;
	}

	return false;
}

//��������
bool CGameFrameDlg::PlayGameSound(HINSTANCE hInstance, LPCTSTR pszSoundName)
{
	if ((m_bAllowSound==true)&&(IsWindowVisible()))
	{
		//������Դ
		HRSRC hResour=FindResource(hInstance,pszSoundName,TEXT("WAVE"));
		if (hResour==NULL) return false;
		HGLOBAL hGlobal=LoadResource(hInstance,hResour);
		if (hGlobal==NULL) return false;

		//��������
		LPCTSTR pszMemory=(LPCTSTR)LockResource(hGlobal);
		::PlaySound(pszMemory,hInstance,SND_ASYNC|SND_MEMORY);

		//������Դ
		UnlockResource(hGlobal);
		FreeResource(hGlobal);
	}

	return true;
}

//����ͼƬ
bool CGameFrameDlg::InsertImage(CBitmap * pBitmap)
{
	if (m_MessageProxyHelper.GetInterface()==NULL) return false;
	return m_MessageProxyHelper->InsertImage(pBitmap);
}

//��������
bool CGameFrameDlg::InsertUserName(LPCTSTR pszUserName)
{
	if (m_MessageProxyHelper.GetInterface()==NULL) return false;
	return m_MessageProxyHelper->InsertUserName(pszUserName);
}

//ϵͳ��Ϣ
bool CGameFrameDlg::InsertSystemString(LPCTSTR pszString)
{
	if (m_MessageProxyHelper.GetInterface()==NULL) return false;
	return m_MessageProxyHelper->InsertSystemString(pszString,MS_NORMAL);
}

//��ɫ��Ϣ
bool CGameFrameDlg::InsertShtickString(LPCTSTR pszString, COLORREF crColor, bool bNewLine)
{
	if (m_MessageProxyHelper.GetInterface()==NULL) return false;
	return m_MessageProxyHelper->InsertShtickString(pszString,crColor,MS_NORMAL,bNewLine);
}

//��ͨ��Ϣ
bool CGameFrameDlg::InsertGeneralString(LPCTSTR pszString, COLORREF crColor, bool bNewLine)
{
	if (m_MessageProxyHelper.GetInterface()==NULL) return false;
	return m_MessageProxyHelper->InsertGeneralString(pszString,crColor,MS_NORMAL,bNewLine);
}

//�������
void CGameFrameDlg::ActiveGameFrame()
{
	//��ȡ����
	CWnd * pFocusWnd=GetFocus();
	CWnd * pMainWnd=AfxGetMainWnd();

	//�����ж�
	if ((pFocusWnd==NULL)||(pMainWnd->IsChild(pFocusWnd)==FALSE)) pMainWnd->FlashWindow(TRUE);

	return;
}

//�л�����
WORD CGameFrameDlg::SwitchViewChairID(WORD wChairID)
{
	//Ч��״̬
	if (m_ClientKernelHelper.GetInterface()==NULL) return INVALID_CHAIR;

	//��������
	const tagUserData * pMeUserData=m_ClientKernelHelper->GetMeUserInfo();
	const tagServerAttribute * pServerAttribute=m_ClientKernelHelper->GetServerAttribute();

	//ת������
	WORD wViewChairID=(wChairID+pServerAttribute->wChairCount-pMeUserData->wChairID);
	switch (pServerAttribute->wChairCount)
	{
	case 2: { wViewChairID+=1; break; }
	case 3: { wViewChairID+=1; break; }
	case 4: { wViewChairID+=2; break; }
	case 5: { wViewChairID+=2; break; }
	case 6: { wViewChairID+=3; break; }
	case 7: { wViewChairID+=3; break; }
	case 8: { wViewChairID+=4; break; }
	}
	return wViewChairID%pServerAttribute->wChairCount;
}

//�Ƿ�����
bool CGameFrameDlg::IsEnableSound()
{
	return m_bAllowSound;
}

//��������
void CGameFrameDlg::EnableSound(bool bEnable)
{
	//��ȡ��Դ
	HINSTANCE hInstance=g_GlobalOption.m_PlatformResourceModule->GetResInstance();
	tagGameFrameImage &GameFrameImage=g_GlobalOption.m_GameFrameImage;

	//���ñ���
	m_bAllowSound=bEnable;

	//���ư�ť
	UINT uImageID=m_bAllowSound?GameFrameImage.uBtGameSound:GameFrameImage.uBtGameUnSound;
	m_pGameFrameControl->m_btSound.SetButtonImage(uImageID,hInstance,false);
	
	//��дע��
	AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("EnableSound"),m_bAllowSound);

	return;
}

//��Ϸ״̬
BYTE CGameFrameDlg::GetGameStatus()
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return GS_FREE;
	return m_ClientKernelHelper->GetGameStatus();
}

//�Թ�ģʽ
bool CGameFrameDlg::IsLookonMode()
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return true;
	return m_ClientKernelHelper->IsLookonMode();
}

//�����Թ�
bool CGameFrameDlg::IsAllowLookon()
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return false;
	return m_ClientKernelHelper->IsAllowLookon();
}

//�����Թ�
bool CGameFrameDlg::IsAllowUserLookon()
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return false;
	return m_ClientKernelHelper->IsAllowUserLookon();
}

//�Թۿ���
bool CGameFrameDlg::AllowUserLookon(DWORD dwUserID, bool bAllowLookon)
{
	//״̬Ч��
	if (m_ClientKernelHelper.GetInterface()==NULL) return false;

	//���ð�ť
	if (dwUserID==0L)
	{
		//��ȡ��Դ
		HINSTANCE hInstance=g_GlobalOption.m_PlatformResourceModule->GetResInstance();
		tagGameFrameImage &GameFrameImage=g_GlobalOption.m_GameFrameImage;

		//���ư�ť
		UINT uImageID=m_ClientKernelHelper->IsAllowUserLookon()?GameFrameImage.uBtGameLookOn:GameFrameImage.uBtGameUnLookOn;
		m_pGameFrameControl->m_btLookOn.SetButtonImage(uImageID,hInstance,false);
	}

	//������Ϣ
	return m_ClientKernelHelper->AllowUserLookon(dwUserID,bAllowLookon);
}

//��Ϸ״̬
void CGameFrameDlg::SetGameStatus(BYTE bGameStatus)
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return;
	m_ClientKernelHelper->SetGameStatus(bGameStatus);
	return;
}

//�Լ�λ��
WORD CGameFrameDlg::GetMeChairID()
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return INVALID_CHAIR;
	return m_ClientKernelHelper->GetMeChairID();
}

//ʱ��λ��
WORD CGameFrameDlg::GetTimeChairID()
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return INVALID_CHAIR;
	return m_ClientKernelHelper->GetTimeChairID();
}

//������Ϸ
VOID CGameFrameDlg::JoinInGameTable(WORD wChairID)
{
	//״̬Ч��
	ASSERT(m_ClientKernelHelper.GetInterface()!=NULL);
	if (m_ClientKernelHelper.GetInterface()==NULL) return;

	//��ȡ��Ϣ
	const tagUserData * pUserData=m_ClientKernelHelper->GetMeUserInfo();
	if (pUserData==NULL) return;

	//��������
	IPC_JoinInGame JoinInGame;
	ZeroMemory(&JoinInGame,sizeof(JoinInGame));

	//���ñ���
	JoinInGame.wChairID=wChairID;
	JoinInGame.wTableID=pUserData->wTableID;

	//������Ϣ
	m_ClientKernelHelper->SendProcessData(IPC_MAIN_CONCTROL,IPC_SUB_JOIN_IN_GAME,&JoinInGame,sizeof(JoinInGame));

	return;
}

//��ȡ�û�
const tagUserData * CGameFrameDlg::GetUserData(WORD wChairID)
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return NULL;
	return m_ClientKernelHelper->GetUserInfo(wChairID);
}

//�ں˽ӿ�
void * CGameFrameDlg::GetClientKernel(const IID & Guid, DWORD dwQueryVer)
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return NULL;
	return m_ClientKernelHelper->QueryInterface(Guid,dwQueryVer);
}

//�ؼ���
void CGameFrameDlg::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONTROL, m_btControl);
	DDX_Control(pDX, IDC_VOR_SPLITER, m_VorSplitter);
}

//��ʼ������
BOOL CGameFrameDlg::OnInitDialog()
{
	__super::OnInitDialog();

	//��������
	ModifyStyle(WS_CAPTION|WS_BORDER,0,0);

	//��ȡ����
	m_bAllowSound=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("EnableSound"),TRUE)?true:false;

	//��������
	m_pGameFrameControl=new CGameFrameControl(this);
	if (m_pGameFrameControl==NULL) throw TEXT("��Ϸ��ܴ���ʧ��");

	//�������
	if (m_MessageProxyHelper.CreateInstance()==false) throw TEXT("��Ϣ�������ʧ��");
	m_MessageProxyHelper->SetRichEditHwnd(&m_pGameFrameControl->m_ChatMessage);

	//���ý���
	m_VorSplitter.SetSplitterSink(this);

	//�������
	if (m_UserFaceResHelper.CreateInstance()==false) throw TEXT("ͷ���������ʧ��");
	if (m_ClientKernelHelper.CreateInstance()==false) throw TEXT("��Ϸ����ں�ģ�����ʧ��");

	//������Ϸ��ͼ
	CRect rcGameView(0,0,0,0);
	m_pGameFrameControl->Create(IDD_FRAME_CONTROL,this);
	if (m_pGameFrameControl->SetUserFaceRes(m_UserFaceResHelper.GetInterface())==false) throw TEXT("ͷ����Դ�ӿ�����ʧ��");
	m_pGameFrameView->Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcGameView,this,10);
	if (m_pGameFrameView->SetUserFaceRes(m_UserFaceResHelper.GetInterface())==false) throw TEXT("ͷ����Դ�ӿ�����ʧ��");
	CVideoServiceManager * pVideoServiceManager=CVideoServiceManager::GetInstance();
	if (pVideoServiceManager!=NULL && pVideoServiceManager->SetUserFaceRes(m_UserFaceResHelper.GetInterface())==false) throw TEXT("ͷ����Դ�ӿ�����ʧ��");

	//��ʼ���ں�
	LPCTSTR lpszCmdLine=AfxGetApp()->m_lpCmdLine;
	bool bSuccess=m_ClientKernelHelper->InitClientKernel(lpszCmdLine,m_pKernelSink);
	if (bSuccess==false) throw TEXT("��Ϸ����߼�ģ���ʼ��ʧ��");
	bSuccess=m_ClientKernelHelper->SetMessageProxy(m_MessageProxyHelper.GetInterface());
	if (bSuccess==false) throw TEXT("��ȡ��Ϣ���ģ��ʧ��");

	//��ʼ����Ϸ
	m_pGameFrameControl->SetClientKernel(m_ClientKernelHelper.GetInterface());
	if (InitGameFrame()==false) throw TEXT("��Ϸ��ܳ�ʼ��ʧ��");

	//������Դ
	UpdateSkinResource();

	//�ƶ�����
	INT nXScreen=GetSystemMetrics(SM_CXSCREEN);
	INT nYScreen=GetSystemMetrics(SM_CYSCREEN);
	SetWindowPos(NULL,0,0,__min(nXScreen,1024),__min(nYScreen,730),SWP_NOZORDER|SWP_NOMOVE);

	//��ʾ����
	m_bInitDialog=true;
	m_pGameFrameView->ShowWindow(SW_SHOW);
	m_pGameFrameControl->ShowWindow(SW_SHOW);

	//��ʾ����
	CenterWindow();
	MaxSizeWindow();
	ShowWindow(SW_SHOW);
	
	return TRUE;
}

//��Ϣ����
BOOL CGameFrameDlg::PreTranslateMessage(MSG * pMsg)
{
	//������Ϣ
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE)) return TRUE;
	return __super::PreTranslateMessage(pMsg);
}

//ȷ����Ϣ
void CGameFrameDlg::OnOK()
{
	return;
}

//ȡ����Ϣ
void CGameFrameDlg::OnCancel()
{
	//�˳���ʾ
	if ((m_ClientKernelHelper.GetInterface()!=NULL)&&(m_ClientKernelHelper->GetExitFlags()==false))
	{
		WORD wChairID=GetMeChairID();
		const tagUserData * pUserData=GetUserData(wChairID);
		if (pUserData!=NULL)
		{
			int iRet=ShowInformation(TEXT("��������Ϸ�У�ǿ���˳������۷֣�ȷʵҪǿ����"),0,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2);
			if (iRet!=IDYES) return;
		}
	}

	//��������
	g_GlobalOption.SaveOptionParameter();

	//ע���ӿ�
	m_ClientKernelHelper->DestroyClientKernel();

	//�������
	SafeDelete(m_pKernelSink);
	m_UserFaceResHelper.CloseInstance();
	m_ClientKernelHelper.CloseInstance();

	//�رմ���
	DestroyWindow();

	return;
}

//���ư�ť
VOID CGameFrameDlg::OnBnClickedControl()
{
	//���ñ���
	m_bRectify=false;
	m_bHideList=!m_bHideList;

	//���ý���
	HINSTANCE hInstance=g_GlobalOption.m_PlatformResourceModule->GetResInstance();
	tagGameFrameImage &GameFrameImage=g_GlobalOption.m_GameFrameImage;
	m_btControl.SetButtonImage(m_bHideList?GameFrameImage.uBtShowUserList:GameFrameImage.uBtHideUserList,hInstance,false);

	//���ý���
	m_VorSplitter.EnableSplitter(m_bHideList==false);

	//��������
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());

	return;
}

//λ����Ϣ
void CGameFrameDlg::OnGetMinMaxInfo(MINMAXINFO * lpMMI)
{
	__super::OnGetMinMaxInfo(lpMMI);

	//����λ��
	lpMMI->ptMinTrackSize.x=1024;
	lpMMI->ptMinTrackSize.y=740;

	return;
}

//λ�øı�
void CGameFrameDlg::OnWindowPosChanging(WINDOWPOS * lpwndpos)
{
	if (m_bInitDialog==false) lpwndpos->flags&=~SWP_SHOWWINDOW;
	__super::OnWindowPosChanging(lpwndpos);
}

//λ�øı�
void CGameFrameDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	//�����ؼ�
	RectifyControl(cx,cy);

	return;

}

//////////////////////////////////////////////////////////////////////////
