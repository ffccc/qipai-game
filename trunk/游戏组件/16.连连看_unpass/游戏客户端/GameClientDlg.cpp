#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"
#include "GlobalOption.h"
#include "mmsystem.h"
#include ".\gameclientdlg.h"


//////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CSkinDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()

	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_D3D_LINK,OnLink)
	ON_MESSAGE(IDM_D3D_GUIDE,OnGuide)
	ON_MESSAGE(IDM_D3D_BOMB,OnBomb)
	ON_MESSAGE(IDM_D3D_REALIGN,OnRealign)
//	ON_MESSAGE(IDM_D3D_STOPIT,OnStopIt)
//	ON_MESSAGE(IDM_D3D_CLOG,OnClog)

	ON_MESSAGE(IDM_D3D_TIMER,OnTimerStep)

	ON_MESSAGE(IDM_QUIT,OnQuit)

	//��������
	//ON_MESSAGE(MM_MCINOTIFY,OnMusicNotify)

	//ѡ������
	ON_MESSAGE(IDM_D3D_ALLMUSIC, OnPlayMusic)

	ON_WM_TIMER()
	ON_WM_CREATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CSkinDialog(IDD_GAME_FRAME)
{
	//���ñ���
	m_bReset=false;
	m_bRectify=false;
	m_bInitDialog=false;

	//���ñ���
	m_bAllowSound=true;

	return;
}

//��������
CGameClientDlg::~CGameClientDlg()
{
	m_midi.Stop(FALSE);
	//m_AllSound.Release();
	//m_AllSound.ReleaseSound();
}

//�ؼ���
void CGameClientDlg::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
}

//��ʼ����
bool CGameClientDlg::InitGameFrame()
{
	//���ñ���
	SetWindowText(TEXT("��������Ϸ"));
	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);
	VERIFY(m_midi.Create(IDR_BGMIDI));

	//VERIFY(CMySound::InitDirectSound(m_hWnd));
	VERIFY(m_Bomb.Create(TEXT("WAVE_BOMB")));
	VERIFY(m_Link.Create(TEXT("WAVE_LINK")));
	VERIFY(m_FlyStar.Create(TEXT("WAVE_FLYSTAR")));
	VERIFY(m_Start.Create(TEXT("WAVE_START")));
	VERIFY(m_Select.Create(TEXT("WAVE_SELECT")));
	VERIFY(m_End.Create(TEXT("WAVE_END")));

	return true;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{
	return;
}

//ʱ����Ϣ
bool CGameClientDlg::OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	return true;
}

//�Թ�״̬
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{

}

//������Ϣ
bool CGameClientDlg::OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	switch(wSubCmdID)
	{
	case SUB_S_GROUP:		//�����Ϣ
		{
			return OnSubGroup(pBuffer,wDataSize);
		}
	case SUB_S_GAME_START:	//��Ϸ��ʼ
		{
			return OnSubStart(pBuffer,wDataSize);
		}
	case SUB_S_GAME_OVER:	//��Ϸ����
		{
			return OnSubEnd(pBuffer,wDataSize);
		}
	case SUB_S_LINK:		//���Ӵ���
		{
			return OnSubLink(pBuffer,wDataSize);
		}
	case SUB_S_LOST:		//ǿ�˻��߳�ʱ
		{
			return OnSubLost(pBuffer,wDataSize);
		}
	case SUB_S_REALIGN:		//���д���
		{
			return OnSubRealign(pBuffer,wDataSize);
		}
	case SUB_S_STOPIT:		//���ִ���
		{
			//return OnSubStopIt(pBuffer,wDataSize);
		}
	case SUB_S_CLOG:		//�ϰ�����
		{
			//return OnSubClog(pBuffer,wDataSize);
		}
	}
	return true;
}

//��Ϸ����
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	
	switch (cbGameStation)
	{
	case GS_FREE:		//����״̬
		{
			m_GameClientView.m_MyListView.m_btStart.EnableWindow(TRUE);

			return true;
		}
	case GS_PLAYING:	//��Ϸ״̬
		{
			if (wDataSize!=sizeof(CMD_S_GameState)) return false;
			CMD_S_GameState * psGameState = (CMD_S_GameState *)pBuffer;

			int id;
			//װ������
			for(int i=0; i<GAME_PLAYER; i++)
			{
				id = SwitchViewChairID(i);
				m_GameClientView.m_wndD3D.m_bPlayState[id] = psGameState->bPlayState[i];
				memcpy(m_GameClientView.m_wndD3D.m_Chess[id],psGameState->map[i],sizeof(psGameState->map[i]));
			}

			if(IsLookonMode() || !psGameState->bPlayState[GetMeChairID()])
			{
				m_GameClientView.m_wndD3D.m_bCanGame = false;
				m_GameClientView.m_wndD3D.m_bGameLose[2] = false;
			}
			else
			{
				memcpy(m_GameClientView.m_MyListView.m_ImplementCount,psGameState->bPropCnt,sizeof(psGameState->bPropCnt));
				//���趨ʱ��
				m_GameClientView.m_wndD3D.m_nTimeCounter = psGameState->nLeaveTime;
				m_GameClientView.m_wndD3D.m_bCanGame = true;
				if(psGameState->nLeaveTime>=30)
				m_GameClientView.m_wndD3D.m_bGameLose[2] = true;
			}
			m_GameClientView.m_wndD3D.UpdateBackSurface();
			m_GameClientView.m_MyListView.UpdateImplementState();

			//�ر���ϰ��ť
			m_GameClientView.m_MyListView.m_btStart.EnableWindow(FALSE);
			m_GameClientView.m_MyListView.m_btExercise.EnableWindow(FALSE);
			OnPlayMusic(IDM_MUSIC_BG,0);
			return true;
		}
	default:
		return true;
	}

	return true;
}



void CGameClientDlg::OnTimer(UINT nIDEvent)
{
	//CGameFrameDlg::OnTimer(nIDEvent);
}



BOOL CGameClientDlg::OnInitDialog()
{
	__super::OnInitDialog();

	//��������
	ModifyStyle(WS_CAPTION|WS_BORDER,0,0);

	m_bAllowSound=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("EnableSound"),TRUE)?true:false;

	//������Ϸ��ͼ
	CRect rcGameView;
	m_GameClientView.Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcGameView,this,10);

	//�������
	if (m_UserFaceResHelper.CreateInstance()==false) throw TEXT("ͷ���������ʧ��");
	if (m_ClientKernelHelper.CreateInstance()==false) throw TEXT("��Ϸ����ں�ģ�����ʧ��");

	//��ʼ���ں�
	CWinApp *p=AfxGetApp();
	LPCTSTR lpszCmdLine=AfxGetApp()->m_lpCmdLine;
	IUnknownEx * pIUnknownEx=GET_MYSELF_INTERFACE(IUnknownEx);
	bool bSuccess=m_ClientKernelHelper->InitClientKernel(lpszCmdLine,pIUnknownEx);
	if (bSuccess==false) throw TEXT("��Ϸ����߼�ģ���ʼ��ʧ��");

	IUnknownEx * pIMessageProxy = GET_OBJECT_INTERFACE(m_GameClientView.m_MyListView,IUnknownEx);
	bSuccess=m_ClientKernelHelper->SetMessageProxy(pIMessageProxy);
	if (bSuccess==false) throw TEXT("��ȡ��Ϣ���ģ��ʧ��");

	//��ʼ����Ϸ
	m_GameClientView.m_MyListView.SetClientKernel(m_ClientKernelHelper.GetInterface());
	if (InitGameFrame()==false) throw TEXT("��Ϸ��ܳ�ʼ��ʧ��");

	//��ʾ����
	m_bInitDialog=true;
	m_GameClientView.ShowWindow(SW_SHOW);

	//��ʾ����
	if (GetSystemMetrics(SM_CXSCREEN)>1024)
	{
		MoveWindow(0,0,1024,748,FALSE);
		CenterWindow(NULL);
	}
	else MaxSizeWindow();
	ShowWindow(SW_SHOW);

	return TRUE;
}
//�Ƿ���Ч
bool __cdecl CGameClientDlg::IsValid()
{
	return true;
}
//�ͷŶ���
bool __cdecl CGameClientDlg::Release()
{
	return true;
}
//�ӿڲ�ѯ
void * __cdecl CGameClientDlg::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IClientKernelSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IClientKernelSink,Guid,dwQueryVer);
	return NULL;
}

//�����ӿ�
//��ȡ����
HWND __cdecl CGameClientDlg::GetFrameWnd()
{
	return this->m_hWnd ;
}
//����״̬
void __cdecl CGameClientDlg::ResetGameFrame()
{
	//����û�
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.MySetUserInfo(i,NULL);
	}

	m_GameClientView.ResetGameView();
	return ;
}
//�رմ���
void __cdecl CGameClientDlg::CloseGameFrame(bool bForceClose)
{
	//��������
	g_GlobalOption.SaveOptionParameter();

	//�������
	m_UserFaceResHelper.CloseInstance();
	m_ClientKernelHelper.CloseInstance();


	//�رմ���
	DestroyWindow();
	return;
}

//�ں��¼�
//ʱ����Ϣ
bool __cdecl CGameClientDlg::OnEventTimer(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	//��������
	WORD wViewChairID=SwitchViewChairID(wChairID);
	//m_GameClientView.SetUserTimer(wViewChairID,nElapse);
	//��Ҫ

	//�¼�֪ͨ
	return OnTimerMessage(wChairID,nElapse,nTimerID);

	return true;
}
//������Ϣ
bool CGameClientDlg::OnFrameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	return false;
}

//������Ϣ
bool __cdecl CGameClientDlg::OnEventSocket(const CMD_Command & Command, const void * pBuffer, WORD wDataSize)
{
	switch (Command.wMainCmdID)
	{
	case MDM_GF_FRAME:	//�����Ϣ
		{
			return OnFrameMessage(Command.wSubCmdID,pBuffer,wDataSize);
		}
	case MDM_GF_GAME:	//��Ϸ��Ϣ
		{
			return OnGameMessage(Command.wSubCmdID,pBuffer,wDataSize);
		}
	}
	return false;
}
//������Ϣ
bool __cdecl CGameClientDlg::OnEventProcess(const IPC_Head & Head, const void * pBuffer, WORD wDataSize, HWND hWndSend)
{
	return false;
}

//��Ϸ�¼�
//��Ϸ״̬
bool __cdecl CGameClientDlg::OnEventGameScene(BYTE cbGameStation, bool bLookonOther, void * pBuffer, WORD wDataSize)
{
	//Ҫ������ͨ������Ҫ��Ӧ�޸�
	/*
	m_pGameFrameControl->m_UserListView.m_wKindID=m_ClientKernelHelper->GetServerAttribute()->wKindID;
	m_pGameFrameControl->m_UserListView.m_wGameGenre=m_ClientKernelHelper->GetServerAttribute()->wGameGenre;
	*/
	return OnGameSceneMessage(cbGameStation,bLookonOther,pBuffer,wDataSize);
}
//������Ϣ
bool __cdecl CGameClientDlg::OnUserChatMessage(tagUserData * pSendUserData, tagUserData * pRecvUserData, LPCTSTR pszChatMessage, COLORREF crColor)
{
	return m_GameClientView.m_MyListView.InsertUserChat(pSendUserData->szName,pszChatMessage,crColor,MS_NORMAL);
}
//�û�����
void __cdecl CGameClientDlg::OnEventUserEnter(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
	//���ý���
	if (bLookonUser==false)
	{
		WORD wViewChairID=SwitchViewChairID(wChairID);
		m_GameClientView.MySetUserInfo(wViewChairID,pUserData);
		//��Ҫ
	}

	//test
	m_GameClientView.m_MyListView.InsertUserItem(pUserData);

	return;
}
//�û��뿪
void __cdecl CGameClientDlg::OnEventUserLeave(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
	//���ý���
	if (bLookonUser==false)
	{
		WORD wViewChairID=SwitchViewChairID(wChairID);
		m_GameClientView.MySetUserInfo(wViewChairID,NULL);
		//��Ҫ
	}

	//test
	m_GameClientView.m_MyListView.DeleteUserItem(pUserData);
	return;
}
//�û�����
void __cdecl CGameClientDlg::OnEventUserScore(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
	//���ý���
	if (bLookonUser==false) m_GameClientView.UpdateGameView(NULL);

	m_GameClientView.m_MyListView.UpdateUserItem(pUserData);

	return;
}
//�û�״̬
void __cdecl CGameClientDlg::OnEventUserStatus(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
	//���ý���
	if (bLookonUser==false) m_GameClientView.UpdateGameView(NULL);

	m_GameClientView.m_MyListView.UpdateUserItem(pUserData);

	return;
}

//ʱ�����
void __cdecl CGameClientDlg::OnEventTimerKilled(WORD wChairID, UINT nTimerID)
{
	//���ý���
	WORD wViewChairID=SwitchViewChairID(wChairID);
	//m_GameClientView.SetUserTimer(wViewChairID,0);
	//��Ҫ
	return;
}
//�Թ�״̬
void __cdecl CGameClientDlg::OnEventLookonChanged(bool bLookonUser, void * pBuffer, WORD wDataSize)
{
	return;
}

//��ť��Ϣ
bool __cdecl CGameClientDlg::OnSplitterButton(CSkinSplitter * pSkinSplitter, CSkinButton * pSplitterButton)
{
	return true;
}
//�������Ϣ
bool __cdecl CGameClientDlg::OnSplitterEvent(CSkinSplitter * pSkinSplitter, UINT uSplitID, int nXPos, int nYPos)
{
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());
	return true;
}
	
//��������
void CGameClientDlg::RectifyControl(int nWidth, int nHeight)
{
	const int iXExc=GetXExcursionPos();
	const int iYExc=GetYExcursionPos();

	//������Ļ
	//LockWindowUpdate();

	//�ƶ��ؼ�
	HDWP hDwp=BeginDeferWindowPos(1);
	DeferWindowPos(hDwp,m_GameClientView,NULL,iXExc,iYExc,nWidth,nHeight,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS);
	EndDeferWindowPos(hDwp);

	//�ػ�����
	Invalidate(FALSE);
	UpdateWindow();

	//�������
	//UnlockWindowUpdate();

	return;
}

void CGameClientDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	//�����ؼ�
	RectifyControl(cx,cy);

	return;
}

void CGameClientDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� __super::OnPaint()
}

void CGameClientDlg::OnCancel()
{
	/*
	//�˳���ʾ
	if (IsLookonMode()==false)
	{
		WORD wChairID=GetMeChairID();
		const tagUserData * pUserData=GetUserData(wChairID);
		if ((pUserData!=NULL)&&(pUserData->cbUserStatus==US_PLAY))
		{
			int iRet=AfxMessageBox(TEXT("��������Ϸ�У�ǿ���˳������۷֣�ȷʵҪǿ����"),MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2);
			if (iRet!=IDYES) return;
		}
	}
	*/
	//�رտ��
	CloseGameFrame(true);

	return;
}

//���ͺ���
bool CGameClientDlg::SendData(WORD wSubCmdID)
{
	ASSERT(m_ClientKernelHelper.GetInterface()!=NULL);
	return m_ClientKernelHelper->SendSocketData(MDM_GF_GAME,wSubCmdID);
}

//���ͺ���
bool CGameClientDlg::SendData(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	ASSERT(m_ClientKernelHelper.GetInterface()!=NULL);
	//if(m_ClientKernelHelper.GetInterface()==NULL)
	//	return true;
	return m_ClientKernelHelper->SendSocketData(MDM_GF_GAME,wSubCmdID,pData,wDataSize);
}

//����׼��
bool CGameClientDlg::SendUserReady(void * pBuffer, WORD wDataSize)
{
	ASSERT(m_ClientKernelHelper.GetInterface()!=NULL);
	return m_ClientKernelHelper->SendUserReady(pBuffer,wDataSize);
}

//���ö�ʱ��
bool CGameClientDlg::SetGameTimer(WORD wChairID, UINT nTimerID, UINT nElapse)
{
	ASSERT(m_ClientKernelHelper.GetInterface()!=NULL);
	return m_ClientKernelHelper->SetGameTimer(wChairID,nTimerID,nElapse);
}

//ɾ����ʱ��
bool CGameClientDlg::KillGameTimer(UINT nTimerID)
{
	ASSERT(m_ClientKernelHelper.GetInterface()!=NULL);
	return m_ClientKernelHelper->KillGameTimer(nTimerID);
}


//����ͼƬ
bool CGameClientDlg::InsertImage(CBitmap * pBitmap)
{
	return m_GameClientView.m_MyListView.InsertImage(pBitmap);
}

//��������
bool CGameClientDlg::InsertUserName(LPCTSTR pszUserName)
{
	return m_GameClientView.m_MyListView.InsertUserName(pszUserName);
}

//ϵͳ��Ϣ
bool CGameClientDlg::InsertSystemString(LPCTSTR pszString)
{
	return m_GameClientView.m_MyListView.InsertSystemString(pszString,MS_NORMAL);
}

//��ɫ��Ϣ
bool CGameClientDlg::InsertShtickString(LPCTSTR pszString, COLORREF crColor, bool bNewLine)
{
	return m_GameClientView.m_MyListView.InsertShtickString(pszString,crColor,MS_NORMAL,bNewLine);
}

//��ͨ��Ϣ
bool CGameClientDlg::InsertGeneralString(LPCTSTR pszString, COLORREF crColor, bool bNewLine)
{
	return m_GameClientView.m_MyListView.InsertGeneralString(pszString,crColor,MS_NORMAL,bNewLine);
}

//�������
void CGameClientDlg::ActiveGameFrame()
{
	BringWindowToTop(); 
	SetForegroundWindow();
	return;
}

//�л�����
WORD CGameClientDlg::SwitchViewChairID(WORD wChairID)
{
	//��������
	ASSERT(m_ClientKernelHelper.GetInterface()!=NULL);
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
bool CGameClientDlg::IsEnableSound()
{
	return m_bAllowSound;
}

//��������
void CGameClientDlg::EnableSound(bool bEnable)
{
	//���ñ���
	m_bAllowSound=bEnable;
	
	//��дע��
	AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("EnableSound"),m_bAllowSound);

	return;
}

//��Ϸ״̬
BYTE CGameClientDlg::GetGameStatus()
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return GS_FREE;
	return m_ClientKernelHelper->GetGameStatus();
}

//�Թ�ģʽ
bool CGameClientDlg::IsLookonMode()
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return true;
	return m_ClientKernelHelper->IsLookonMode();
}

//�����Թ�
bool CGameClientDlg::IsAllowLookon()
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return false;
	return m_ClientKernelHelper->IsAllowLookon();
}

//��Ϸ״̬
void CGameClientDlg::SetGameStatus(BYTE bGameStatus)
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return;
	m_ClientKernelHelper->SetGameStatus(bGameStatus);


	return;
}

//�Լ�λ��
WORD CGameClientDlg::GetMeChairID()
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return INVALID_CHAIR;
	return m_ClientKernelHelper->GetMeChairID();
}

//ʱ��λ��
WORD CGameClientDlg::GetTimeChairID()
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return INVALID_CHAIR;
	return m_ClientKernelHelper->GetTimeChairID();
}

//��ȡ�û�
const tagUserData * CGameClientDlg::GetUserData(WORD wChairID)
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return NULL;
	return m_ClientKernelHelper->GetUserInfo(wChairID);
}

//�ں˽ӿ�
void * CGameClientDlg::GetClientKernel(const IID & Guid, DWORD dwQueryVer)
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return NULL;
	return m_ClientKernelHelper->QueryInterface(Guid,dwQueryVer);
}

int CGameClientDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������

	return 0;
}

void CGameClientDlg::OnOK()
{
	m_GameClientView.m_MyListView.OnBnClickedSendChat();
	return;
}

LRESULT	CGameClientDlg::OnQuit(WPARAM wParam, LPARAM lParam)
{
	OnCancel();
	return true;
}

// ��������λ��ת��
int CGameClientDlg::ChairSwitch(int chair)
{
	int otherChair = SwitchViewChairID(chair);

	if(otherChair == 3)
		return 2;

	return otherChair;
}

LRESULT CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	m_midi.Pause();
	m_GameClientView.ResetGameView();
	SendUserReady(NULL,0);

	m_GameClientView.m_wndD3D.m_bGameOver = false;
	m_GameClientView.m_wndD3D.m_nTimeCounter = 0;
	return true;
}

LRESULT CGameClientDlg::OnTimerStep(WPARAM wParam, LPARAM lParam)
{
	if(m_GameClientView.m_wndD3D.m_ExerciseMode)
		return true;
	CMD_C_Timer c_timer;
	c_timer.nTime = (BYTE)(LINKALLTIME - wParam);

	SendData(SUB_C_TIMER,&c_timer,sizeof(c_timer));
	return true;
}

//��Ϸ��ʼ
bool CGameClientDlg::OnSubStart(const void * pBuffer, WORD wDataSize)
{
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;
	CMD_S_GameStart * psGameStart = (CMD_S_GameStart *)pBuffer;

	m_GameClientView.m_wndD3D.InitChess();

	int id;
	for(int i=0; i<GAME_PLAYER; i++)
	{
		id = SwitchViewChairID(i);
		m_GameClientView.m_wndD3D.m_bPlayState[id] = psGameStart->bPlayState[i];
		if(m_GameClientView.m_wndD3D.m_bPlayState[id] == GS_PLAYING)
			memcpy(m_GameClientView.m_wndD3D.m_Chess[id], psGameStart->bChess, sizeof(psGameStart->bChess));
	}

	if(!IsLookonMode())
	{
		//��ʼ����
		for(int i=0; i<5;i++)
		m_GameClientView.m_MyListView.m_ImplementCount[i] = psGameStart->bPropCnt[i];
		//��ʱ��
		m_GameClientView.m_wndD3D.m_nTimeCounter = 0;
		m_GameClientView.m_wndD3D.m_bCanGame = true;
	}
	m_GameClientView.m_wndD3D.AccountChessCount();
	m_GameClientView.m_wndD3D.UpdateBackSurface();
	m_GameClientView.m_MyListView.UpdateImplementState();

	//�ر���ϰ��ť
	m_GameClientView.m_MyListView.m_btExercise.EnableWindow(FALSE);

	//��������
	OnPlayMusic(IDM_MUSIC_START,0);
	OnPlayMusic(IDM_MUSIC_BG, 0);
	return true;
}

//��Ϸ����
bool CGameClientDlg::OnSubEnd(const void * pBuffer, WORD wDataSize)
{
	if (wDataSize!=sizeof(CMD_S_GameOver)) return false;
	CMD_S_GameOver * psGameOver = (CMD_S_GameOver *)pBuffer;

	m_midi.Pause();

	for(int i=0; i<GAME_PLAYER; i++)
	{
		int id = SwitchViewChairID(i);
		m_GameClientView.m_wndD3D.m_UserTaxis[id] = psGameOver->bOrder[i];
		m_GameClientView.m_wndD3D.m_UserGrade[id] = psGameOver->nScores[i];
		if(psGameOver->bOrder[i]!= 1)m_GameClientView.m_wndD3D.m_bGameLose[id] = true;
		else m_GameClientView.m_wndD3D.m_bGameLose[id] = false;
		
		this->InsertSystemString(TEXT("���ֽ������ɼ�ͳ�ƣ�"));
		if(m_ClientKernelHelper->GetUserInfo(i))
		{
			CString msg;
			msg.Format("%s : ����: %d, ���÷�: %d",GetUserData(i)?GetUserData(i)->szName:"����", psGameOver->bOrder[i],psGameOver->nScores[i]);
			this->InsertSystemString(msg);
		}
	}

	if(m_GameClientView.m_wndD3D.m_bGameLose[2] == false)
	{
		//��������
		OnPlayMusic(IDM_MUSIC_END,0);
	}

	m_GameClientView.m_wndD3D.m_bCanGame = false;
	m_GameClientView.m_wndD3D.m_bGameOver = true;
	m_GameClientView.m_MyListView.PropZero();
	m_GameClientView.m_MyListView.UpdateImplementState();


	if(!IsLookonMode())
	{
		m_GameClientView.m_MyListView.m_btStart.EnableWindow(TRUE);

		//����ϰ��ť
		m_GameClientView.m_MyListView.m_btExercise.EnableWindow(TRUE);
	}

	BYTE cbGameStatus = GetGameStatus() ;

	return true;
}

//��Ϸʧ��
bool CGameClientDlg::OnSubLost(const void * pBuffer, WORD wDataSize)
{
	if (wDataSize!=sizeof(CMD_S_Lost)) return false;
	CMD_S_Lost * psGameLost = (CMD_S_Lost *)pBuffer;

	int id = SwitchViewChairID(psGameLost->wSeat);
	if(id == 2)
	{
		m_GameClientView.m_MyListView.PropZero();
		m_GameClientView.m_MyListView.UpdateImplementState();
		m_GameClientView.m_wndD3D.m_bCanGame = false;
		m_GameClientView.m_wndD3D.m_bGameLose[id] = true;
		//��������
		OnPlayMusic(IDM_MUSIC_END,0);
	}
	else
	{
		m_GameClientView.m_wndD3D.m_bGameLose[id] = true;
	}
	return true;
}

//�����Ϣ
bool CGameClientDlg::OnSubGroup(const void * pBuffer, WORD wDataSize)
{
	return true;
}

//����������Ϣ
LRESULT CGameClientDlg::OnLink(WPARAM wParam, LPARAM lParam)
{
	//��������
	OnPlayMusic(IDM_MUSIC_LINK,0);
	OnPlayMusic(IDM_MUSIC_BOMB,0);
	
	m_GameClientView.m_wndD3D.m_nTimeCounter = 0;

	if(m_GameClientView.m_wndD3D.m_ExerciseMode)
		return true;

	CMD_C_Link clink;
	clink.nFirst_X = m_GameClientView.m_wndD3D.m_ptLinkFirst.x;
	clink.nFirst_Y = m_GameClientView.m_wndD3D.m_ptLinkFirst.y;
	clink.nEnd_X = m_GameClientView.m_wndD3D.m_ptLinkSecond.x;
	clink.nEnd_Y = m_GameClientView.m_wndD3D.m_ptLinkSecond.y;
	clink.bLinkKind =(BYTE) wParam;

	SendData(SUB_C_LINK,&clink,sizeof(clink));

	

	return true;
}


//���Ӵ���
bool CGameClientDlg::OnSubLink(const void * pBuffer, WORD wDataSize)
{
	if (wDataSize!=sizeof(CMD_S_Link)) return false;
	CMD_S_Link * pslink=(CMD_S_Link *)pBuffer;

	WORD id = SwitchViewChairID(pslink->nCurrentPlayer);
	if(id ==2)
	{
		if(IsLookonMode())
		{
			
			m_GameClientView.LinkPro(CPoint(pslink->nFirst_X,pslink->nFirst_Y),
				CPoint(pslink->nEnd_X,pslink->nEnd_Y),id);

			//��������
			OnPlayMusic(IDM_MUSIC_LINK,0);
			OnPlayMusic(IDM_MUSIC_BOMB,0);
		}
		else
		{
			if(pslink->nPropType != ERROR_ALL)
			{
				m_GameClientView.m_MyListView.m_ImplementCount[pslink->nPropType] = pslink->nPropCount;
				m_GameClientView.m_MyListView.UpdateImplementState();
			}
		}
	}
	else
	{
		m_GameClientView.OtherPlayLinkPro(CPoint(pslink->nFirst_X,pslink->nFirst_Y),
				CPoint(pslink->nEnd_X,pslink->nEnd_Y),id);
	}
	return true;
}

//ѡ��������Ϣ
LRESULT CGameClientDlg::OnPlayMusic(WPARAM wParam, LPARAM lParam)
{
	static bool BgnFirst = true;
	switch(wParam)
	{
	case IDM_MUSIC_BG:
		{
			if(m_GameClientView.m_GameOption.m_bEnableMusic || lParam){
				if(BgnFirst){
					m_midi.Play(true);
					BgnFirst = false;
				}
				else{
					if(lParam){
						m_midi.Continue();
					} else {
						m_midi.Pause();
						m_midi.Rewind();
						m_midi.Continue();
					}
				}
			}
		}
		return true;
	case IDM_MUSIC_STP:
		{
			//RetailOutput("IDM_MUSIC_STP begin m_midi.m_bIsReplay = %d\n",m_midi.m_bIsReplay);
			if(m_GameClientView.m_GameOption.m_bEnableMusic || lParam)m_midi.Pause();
			//RetailOutput("IDM_MUSIC_STP end m_midi.m_bIsReplay = %d\n",m_midi.m_bIsReplay);
			///AfxMessageBox("m_midi.Stop();");
		}
		return true;
	case IDM_MUSIC_START:
		{
			//PlayGameSound(AfxGetInstanceHandle(),TEXT("WAVE_START"));
			if(m_GameClientView.m_GameOption.m_bEnableSound)m_Start.Play();
		}
		return true;
	case IDM_MUSIC_LINK:
		{
			if(m_GameClientView.m_GameOption.m_bEnableSound)m_Link.Play();
		}
		return true;
	case IDM_MUSIC_BOMB:
		{
			if(m_GameClientView.m_GameOption.m_bEnableSound)m_Bomb.Play();
		}
		return true;
	case IDM_MUSIC_FLYSTAR:
		{
			//PlayGameSound(AfxGetInstanceHandle(),TEXT("WAVE_FLYSTAR"));
			if(m_GameClientView.m_GameOption.m_bEnableSound)m_FlyStar.Play();
		}
		return true;
	case IDM_MUSIC_END:
		{
			//PlayGameSound(AfxGetInstanceHandle(),TEXT("WAVE_END"));
			if(m_GameClientView.m_GameOption.m_bEnableSound)m_End.Play();
		}
		return true;
	case IDM_MUSIC_PROP:
		{
			//PlayGameSound(AfxGetInstanceHandle(),TEXT("WAVE_PROP"));
			if(m_GameClientView.m_GameOption.m_bEnableSound)m_Prop.Play();
		}
		return true;
	case IDM_MUSIC_SEL:
		{
			//PlayGameSound(AfxGetInstanceHandle(),TEXT("WAVE_SELECT"));
			if(m_GameClientView.m_GameOption.m_bEnableSound)m_Select.Play();
		}
		return true;
	}
	
	return true;
}


//����ָ������Ϣ
LRESULT CGameClientDlg::OnGuide(WPARAM wParam, LPARAM lParam)
{
	OnPlayMusic(IDM_MUSIC_SEL,0);
	if(m_GameClientView.m_wndD3D.m_ExerciseMode)
		return true;
	SendData(SUB_C_GUIDE);
	return true;
}

//����ָ������Ϣ
LRESULT CGameClientDlg::OnBomb(WPARAM wParam, LPARAM lParam)
{
	if(m_GameClientView.m_wndD3D.m_ExerciseMode)
		return true;
	SendData(SUB_C_BOMB);
	return true;
}


//����������Ϣ
LRESULT CGameClientDlg::OnRealign(WPARAM wParam, LPARAM lParam)
{
	//��������
	OnPlayMusic(IDM_MUSIC_FLYSTAR,0);
	if(m_GameClientView.m_wndD3D.m_ExerciseMode)
		return true;
	CMD_C_Realign	cRealign;
	memcpy(cRealign.nChess, m_GameClientView.m_wndD3D.m_Chess[2], sizeof(m_GameClientView.m_wndD3D.m_Chess[2]));

	SendData(SUB_C_REALIGN,&cRealign,sizeof(cRealign));


	return true;
}

//���д���
bool CGameClientDlg::OnSubRealign(const void * pBuffer, WORD wDataSize)
{
	if (wDataSize!=sizeof(CMD_S_Realign)) return false;
	CMD_S_Realign * psRealign=(CMD_S_Realign *)pBuffer;

	WORD id = SwitchViewChairID(psRealign->nCurrentPlayer);
	if(id ==2)
	{
		if(IsLookonMode())
		{
			memcpy(m_GameClientView.m_wndD3D.m_Chess[id],psRealign->nChess , sizeof(psRealign->nChess));
			m_GameClientView.m_wndD3D.UpdateBackSurface();

			//Ч��
			m_GameClientView.m_wndD3D.m_PointSprite_Star.On();

			//��������
			OnPlayMusic(IDM_MUSIC_FLYSTAR,0);
		}
		else
		{
			m_GameClientView.m_MyListView.m_ImplementCount[PROP_RESET] = psRealign->nPropCount;
			m_GameClientView.m_MyListView.UpdateImplementState();
		}
	}
	else
	{
		memcpy(m_GameClientView.m_wndD3D.m_Chess[id],psRealign->nChess , sizeof(psRealign->nChess));
		m_GameClientView.m_wndD3D.UpdateChessSmall(id);
	}
	return true;
}
//���ͽ�����Ϣ
/*LRESULT CGameClientDlg::OnStopIt(WPARAM wParam, LPARAM lParam)
{
	if(m_GameClientView.m_wndD3D.m_ExerciseMode)
		return true;
	CMD_C_StopIt cStopIt;
	cStopIt.nDesUserId = (BYTE)wParam;

	SendData(SUB_C_STOPIT,&cStopIt,sizeof(cStopIt));
	return true;
}

//���ִ���
bool CGameClientDlg::OnSubStopIt(const void * pBuffer, WORD wDataSize)
{
	if (wDataSize!=sizeof(CMD_S_StopIt)) return false;
	CMD_S_StopIt * psStopIt=(CMD_S_StopIt *)pBuffer;

	//������Ϣ
	CString msg;
	msg.Format("%s �� %s ʹ�����ϰ�����",GetUserData(psStopIt->nSrcUserId)->szName,GetUserData(psStopIt->nDesUserId)->szName);
	this->InsertSystemString(msg);

	if(psStopIt->nSrcUserId == GetMeChairID() && IsLookonMode() == false)
	{
		m_GameClientView.m_MyListView.m_ImplementCount[PROP_STOPIT] = psStopIt->nPropCount;
			m_GameClientView.m_MyListView.UpdateImplementState();
	}
	else
	if(psStopIt->nDesUserId == GetMeChairID() && IsLookonMode() == false)
	{
		m_GameClientView.m_wndD3D.StopIt();
	}

	//��������
	if(psStopIt->nDesUserId == GetMeChairID())
	{
		//��������
		OnPlayMusic(IDM_MUSIC_PROP,0);
	}

	return true;
}

//�����ϰ���Ϣ
LRESULT CGameClientDlg::OnClog(WPARAM wParam, LPARAM lParam)
{
	if(m_GameClientView.m_wndD3D.m_ExerciseMode)
		return true;
	CMD_C_Clog cClog;
	cClog.nDesUserId = (BYTE)wParam;

	SendData(SUB_C_CLOG,&cClog,sizeof(cClog));
	return true;
}

//�����ϰ���Ϣ
bool CGameClientDlg::OnSubClog(const void * pBuffer, WORD wDataSize)
{
	if (wDataSize!=sizeof(CMD_S_Clog)) return false;
	CMD_S_Clog * psClog=(CMD_S_Clog *)pBuffer;

	//������Ϣ
	CString msg;
	msg.Format("%s �� %s ʹ�����ϰ�����",GetUserData(psClog->nSrcUserId)->szName,GetUserData(psClog->nDesUserId)->szName);
	this->InsertSystemString(msg);

	if(psClog->nDesUserId == GetMeChairID() && IsLookonMode() == false || psClog->nDesUserId != GetMeChairID())
	{
		int id = SwitchViewChairID(psClog->nDesUserId);
		m_GameClientView.m_wndD3D.Clog(psClog->nChessPosX,psClog->nChessPosY,psClog->nChessFace,id);
	}
	else
	if(psClog->nSrcUserId == GetMeChairID()  && IsLookonMode() == false)
	{
		m_GameClientView.m_MyListView.m_ImplementCount[PROP_CLOG] = psClog->nPropCount;
		m_GameClientView.m_MyListView.UpdateImplementState();
	}

	//��������
	if(psClog->nDesUserId == GetMeChairID())
	{
		//��������
		OnPlayMusic(IDM_MUSIC_PROP,0);
	}

	return true;
}

LRESULT CGameClientDlg::OnMusicNotify(WPARAM wParam, LPARAM lParam)
{
	m_midi.Replay();
	return true;
}*/
