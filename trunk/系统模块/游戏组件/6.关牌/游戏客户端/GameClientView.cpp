#include "StdAfx.h"
#include "Resource.h"
#include "GameClient.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////
//��ť��ʶ

#define IDC_START						100								//��ʼ��ť
#define IDC_OUT_CARD					101								//���ư�ť
#define IDC_PASS_CARD					102								//PASS��ť
#define IDC_OUT_PROMPT					103								//��ʾ��ť
#define IDC_TRUSTEE_CONTROL				104								//�йܿ���

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_CREATE()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_OUT_CARD, OnOutCard)
	ON_BN_CLICKED(IDC_PASS_CARD, OnPassCard)
	ON_BN_CLICKED(IDC_OUT_PROMPT, OnOutPrompt)
	ON_BN_CLICKED(IDC_TRUSTEE_CONTROL, OnStusteeControl)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//�Ϲܿ���
void CGameClientView::OnStusteeControl()
{
	AfxGetMainWnd()->SendMessage(IDM_TRUSTEE_CONTROL,0,0);
	return;
}

//���캯��
CGameClientView::CGameClientView(void) : CGameFrameView(true,24)
{
	//��Ϸ����
	m_lCellScore=0;
	m_cbBombCount=0;

	//״̬����
	memset(m_bPass,0,sizeof(m_bPass));
	memset(m_bCardCount,0,sizeof(m_bCardCount));

	//������Դ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageBomb.SetLoadInfo(IDB_BOMB,hInstance);
	m_ImageBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_ImageFill.SetLoadInfo(IDB_VIEW_FILL,hInstance);
	m_ImageUserPass.SetLoadInfo(IDB_USER_PASS,hInstance);

	return;
}

//��������
CGameClientView::~CGameClientView(void)
{
}

//������Ϣ
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//�����ؼ�
	CRect CreateRect(0,0,0,0);
	m_ScoreView.Create(IDD_GAME_SCORE,this);

	//�����˿�
	for (WORD i=0;i<2;i++)
	{
		m_UserCardControl[i].SetDirection(true);
		m_UserCardControl[i].SetDisplayFlag(true);
		m_UserCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,20+i);
	}

	//�û��˿�
	m_HandCardControl[0].SetCardSpace(20,19,0);
	m_HandCardControl[1].SetSinkWindow(AfxGetMainWnd());
	m_HandCardControl[0].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,40);
	m_HandCardControl[1].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,41);

	//������ť
	m_btStart.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_START);
	m_btOutCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_OUT_CARD);
	m_btPassCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_PASS_CARD);
	m_btOutPrompt.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_OUT_PROMPT);
	m_btStusteeControl.Create(TEXT(""),WS_CHILD|WS_DISABLED|WS_VISIBLE|WS_CLIPSIBLINGS,CreateRect,this,IDC_TRUSTEE_CONTROL);
	m_btStusteeControl.EnableWindow(FALSE);

	//���ð�ť
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btStart.SetButtonImage(IDB_START,hInstance,false);
	m_btOutCard.SetButtonImage(IDB_OUT_CARD,hInstance,false);
	m_btPassCard.SetButtonImage(IDB_PASS,hInstance,false);
	m_btOutPrompt.SetButtonImage(IDB_OUT_PROMPT,hInstance,false);
	m_btStusteeControl.SetButtonImage(IDB_START_TRUSTEE,hInstance,false);

#ifdef VIDEO_GAME
	//������Ƶ
	for (WORD i=0;i<2;i++)
	{
		//������Ƶ
		m_DlgVideoService[i].Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,CreateRect,this,200+i);
		m_DlgVideoService[i].InitVideoService(i==1,i==1);

		//������Ƶ
		g_VideoServiceManager.SetVideoServiceControl(i,&m_DlgVideoService[i]);
	}
#endif

	return 0;
}

//�����Ϣ
void CGameClientView::OnRButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnRButtonUp(nFlags, Point);

	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_OUT_CARD,1,1);

	return;
}

//���˫��
void CGameClientView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	__super::OnLButtonDblClk(nFlags, point);

	//�����˿�
	m_btOutCard.EnableWindow(FALSE);
	m_HandCardControl[1].ShootAllCard(false);

	return;
}

//���ý���
void CGameClientView::ResetGameView()
{
	//��Ϸ����
	m_lCellScore=0L;
	m_cbBombCount=0;

	//״̬����
	memset(m_bPass,0,sizeof(m_bPass));
	memset(m_bCardCount,0,sizeof(m_bCardCount));

	//���ؿؼ�
	m_ScoreView.ShowWindow(SW_HIDE);

	//���ذ�ť
	m_btStart.ShowWindow(SW_HIDE);
	m_btOutCard.ShowWindow(SW_HIDE);
	m_btPassCard.ShowWindow(SW_HIDE);
	m_btOutPrompt.ShowWindow(SW_HIDE);

	//���ÿؼ�
	m_btOutCard.EnableWindow(FALSE);
	m_btPassCard.EnableWindow(FALSE);

	//�˿˿ؼ�
	m_HandCardControl[0].SetCardData(NULL,0);
	m_HandCardControl[1].SetCardData(NULL,0);
	m_HandCardControl[0].SetPositively(false);
	m_HandCardControl[1].SetPositively(false);
	m_HandCardControl[1].SetDisplayFlag(false);
	for (WORD i=0;i<GAME_PLAYER;i++) m_UserCardControl[i].SetCardData(NULL,0);

	return;
}

//�����ؼ�
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	m_ptFace[0].x=nWidth/2-m_nXFace/2;
	m_ptFace[0].y=m_nYBorder+5;
	m_ptName[0].x=nWidth/2+5+m_nXFace/2;
	m_ptName[0].y=m_nYBorder+5;
	m_ptTimer[0].x=nWidth/2-m_nXFace/2-m_nXTimer/2-5;
	m_ptTimer[0].y=m_nYBorder+m_nYTimer/2+5;
	m_ptReady[0].x=nWidth/2-m_nXTimer-m_nXFace/2-5;
	m_ptReady[0].y=m_nYBorder+m_nYTimer/2+10;
	m_PointUserPass[0].x=nWidth/2-21;
	m_PointUserPass[0].y=m_nYBorder+m_nYFace+210;
	m_PointUserBomb[0].x=nWidth/2-m_nXFace/2-150;
	m_PointUserBomb[0].y=m_nYBorder+m_nYFace+3;
	m_ptAuto[0].x=nWidth/2+m_nXFace+m_nXTimer+15;
	m_ptAuto[0].y=m_nYBorder+5;

	m_ptFace[1].x=nWidth/2-m_nXFace/2;
	m_ptFace[1].y=nHeight-m_nYBorder-m_nYFace-5;
	m_ptName[1].x=nWidth/2+5+m_nXFace/2;
	m_ptName[1].y=nHeight-m_nYBorder-m_nYFace-2;
	m_ptTimer[1].x=nWidth/2-m_nXFace/2-m_nXTimer/2-5;
	m_ptTimer[1].y=nHeight-m_nYTimer+20;
	m_ptReady[1].x=nWidth/2;
	m_ptReady[1].y=nHeight-m_nYBorder-m_nYTimer/2-130;
	m_PointUserPass[1].x=nWidth/2-21;
	m_PointUserPass[1].y=nHeight-m_nYBorder-m_nYFace-210;
	m_PointUserBomb[1].x=nWidth/2-m_nXFace/2-150;
	m_PointUserBomb[1].y=nHeight-m_nYBorder-m_nYFace-3;
	m_ptAuto[1].x=nWidth/2+m_nXFace+m_nXTimer+15;
	m_ptAuto[1].y=nHeight-m_nYBorder-m_nYFace-10;

	//�ƶ���ť
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	m_btStart.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nHeight-223-m_nYBorder,0,0,uFlags);

	m_btOutCard.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btOutCard,NULL,nWidth/2-rcButton.Width()*3/2-10,nHeight-223-m_nYBorder,0,0,uFlags);
//	DeferWindowPos(hDwp,m_btOutPrompt,NULL,nWidth/2-rcButton.Width()/2,nHeight-193-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btPassCard,NULL,nWidth/2+rcButton.Width()/2+10,nHeight-223-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOutPrompt,NULL,nWidth,nHeight,0,0,uFlags);
	m_btStusteeControl.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btStusteeControl,NULL,nWidth-m_nXBorder-(rcButton.Width()+5)*1,nHeight-m_nYBorder-5-rcButton.Height(),0,0,uFlags);

#ifdef VIDEO_GAME
	//��Ƶ����
	CRect rcAVDlg;
	m_DlgVideoService[0].GetWindowRect(&rcAVDlg);
	DeferWindowPos(hDwp,m_DlgVideoService[0],NULL,nWidth-m_nXBorder-5-rcAVDlg.Width(),m_nYBorder+5,0,0,uFlags);

	m_DlgVideoService[1].GetWindowRect(&rcAVDlg);
	DeferWindowPos(hDwp,m_DlgVideoService[1],NULL,m_nXBorder+5,nHeight-m_nYBorder-3-rcAVDlg.Height(),0,0,uFlags);
#endif

	EndDeferWindowPos(hDwp);

	//�ƶ��˿�
	m_HandCardControl[0].SetBenchmarkPos(nWidth/2,m_nYBorder+m_nYFace+30,enXCenter,enYTop);
	m_HandCardControl[1].SetBenchmarkPos(nWidth/2,nHeight-m_nYFace-m_nYBorder-30,enXCenter,enYBottom);

	//�û��˿�
	m_UserCardControl[0].SetBenchmarkPos(nWidth/2,m_nYBorder+180,enXCenter,enYTop);
	m_UserCardControl[1].SetBenchmarkPos(nWidth/2,nHeight-228-m_nYBorder,enXCenter,enYBottom);

	//������ͼ
	CRect rcControl;
	m_ScoreView.GetWindowRect(&rcControl);
	m_ScoreView.SetWindowPos(NULL,(nWidth-rcControl.Width())/2,nHeight/2-80,0,0,SWP_NOZORDER|SWP_NOSIZE);

	return;
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//�滭����
	DrawViewImage(pDC,m_ImageFill,enMode_Spread);
	DrawViewImage(pDC,m_ImageBack,enMode_Centent);

	//�滭�û�
	TCHAR szBuffer[32];
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wUserTimer=GetUserTimer(i);
		const tagUserData * pUserData=GetUserInfo(i);

		///////////////////////////////////////////////
		//test
		//WORD wUserTimer = 30;
		//tagUserData *pUserData = new tagUserData;
		//lstrcpy(pUserData->szName,TEXT("�������"));
		//pUserData->lScore = 10000L;
		//pUserData->cbUserStatus = US_PLAY;
		//m_bPass[i]=true;
		//m_bCardCount[i]=30;
		///////////////////////////////////////////////

		//�滭�û�
		if (pUserData!=NULL)
		{
			//�û�����
			pDC->SetTextAlign(TA_LEFT|TA_TOP);
			pDC->SetTextColor((wUserTimer>0)?RGB(250,250,250):RGB(220,220,220));
			pDC->TextOut(m_ptName[i].x,m_ptName[i].y,pUserData->szName,lstrlen(pUserData->szName));

			//ʣ���˿�
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("��%ld �ţ�"),m_bCardCount[i]);
			pDC->TextOut(m_ptName[i].x,m_ptName[i].y+18,szBuffer,lstrlen(szBuffer));

			//������Ϣ
			if (wUserTimer!=0) DrawUserTimer(pDC,m_ptTimer[i].x,m_ptTimer[i].y,wUserTimer);
			if (pUserData->cbUserStatus==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			DrawUserFace(pDC,pUserData,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);
		}
			//�йܱ�־
			if(m_bAutoPlayer[i]==true)
			{
				CPngImage ImageScore;
				ImageScore.LoadImage(AfxGetInstanceHandle(),TEXT("TRUSTEE"));
				ImageScore.DrawImage(pDC,m_ptAuto[i].x,m_ptAuto[i].y);
			}
	}

	//ը������
	if (m_cbBombCount>1)
	{
		//ը����־
		CImageHandle ImageHandle(&m_ImageBomb);
		m_ImageBomb.AlphaDrawImage(pDC,m_nXBorder+5,m_nYBorder+m_ImageBomb.GetHeight()+5,RGB(255,0,255));

		//���� DC
		pDC->SetTextColor(RGB(250,250,250));
		pDC->SetTextAlign(TA_LEFT|TA_TOP);

		//ը������
		TCHAR szBuffer[64]=TEXT("");
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("ը����Ŀ��%d"),m_cbBombCount);
		pDC->TextOut(m_nXBorder+m_ImageBomb.GetWidth()+10,m_nYBorder+20,szBuffer,lstrlen(szBuffer));
	}

	//������־
	CImageHandle ImageHnadleUserPass(&m_ImageUserPass);
	for (WORD i=0;i<2;i++)
	{
		if (m_bPass[i]==true) 
		{
			CSize sizeImage;
			sizeImage.SetSize(m_ImageUserPass.GetWidth(),m_ImageUserPass.GetHeight());
			m_ImageUserPass.AlphaDrawImage(pDC,m_PointUserPass[i].x-sizeImage.cx/2,m_PointUserPass[i].y-sizeImage.cy/2,RGB(255,0,255));
		}
	}

	//��Ԫ����
	if (m_lCellScore>0L)
	{
		TCHAR szBuffer[128]=TEXT("");
		_snprintf(szBuffer,CountArray(szBuffer),TEXT("��Ϸ��ע��%ld"),m_lCellScore);
		pDC->SetTextColor(RGB(250,250,250));
		pDC->SetTextAlign(TA_LEFT|TA_BOTTOM);
		pDC->TextOut(m_nXBorder+5,nHeight-5-m_nYBorder,szBuffer,lstrlen(szBuffer));
	}

	return;
}

//ը����Ŀ
void CGameClientView::SetBombCount(BYTE cbBombCount)
{
	//���ñ���
	m_cbBombCount=cbBombCount;

	//ˢ�½���
	UpdateGameView(NULL);

	return;
}

//��������
void CGameClientView::SetCellScore(LONG lCellScore)
{
	//���ñ���
	m_lCellScore=lCellScore;

	//ˢ�½���
	UpdateGameView(NULL);

	return;
}

//���÷���
void CGameClientView::SetPassFlag(WORD wChairID, bool bPass)
{
	//���ñ���
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<GAME_PLAYER;i++) m_bPass[i]=bPass;
	}
	else m_bPass[wChairID]=bPass;

	//���½���
	UpdateGameView(NULL);

	return;
}

//�˿���Ŀ
void CGameClientView::SetCardCount(WORD wChairID, BYTE bCardCount)
{
	//���ñ���
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<GAME_PLAYER;i++) m_bCardCount[i]=bCardCount;
	}
	else m_bCardCount[wChairID]=bCardCount;

	//���½���
	UpdateGameView(NULL);

	return;
}

//��ʼ��ť
void CGameClientView::OnStart()
{
	AfxGetMainWnd()->SendMessage(IDM_START,0,0);
	return;
}

//���ư�ť
void CGameClientView::OnOutCard()
{
	AfxGetMainWnd()->SendMessage(IDM_OUT_CARD,1,1);
	return;
}

//������ť
void CGameClientView::OnPassCard()
{
	AfxGetMainWnd()->SendMessage(IDM_PASS_CARD,1,1);
	return;
}

//��ʾ��ť
void CGameClientView::OnOutPrompt()
{
	AfxGetMainWnd()->SendMessage(IDM_OUT_PROMPT,0,0);
	return;
}
void CGameClientView::SetAutoUser(bool bAutoUser[2])
{
	CopyMemory(m_bAutoPlayer,bAutoUser,sizeof(m_bAutoPlayer));
		
	//���½���
	UpdateGameView(NULL);
	return;
}
//////////////////////////////////////////////////////////////////////////

