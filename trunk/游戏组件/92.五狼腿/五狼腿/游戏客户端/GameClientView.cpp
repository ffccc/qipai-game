#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////
//��ť��ʶ

#define IDC_START						100								//��ʼ��ť
#define IDC_OUT_CARD					101								//���ư�ť
#define IDC_PASS_CARD					102								//PASS��ť
#define IDC_OUT_PROMPT					103								//��ʾ��ť
#define IDC_LAST_TURN					104								//���ְ�ť
#define IDC_SORT_CARD					105								//���а�ť
#define IDC_TRUSTEE_CONTROL				106								//�йܿ���

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_CREATE()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_OUT_CARD, OnOutCard)
	ON_BN_CLICKED(IDC_PASS_CARD, OnPassCard)
	ON_BN_CLICKED(IDC_OUT_PROMPT, OnOutPrompt)
	ON_BN_CLICKED(IDC_LAST_TURN, OnBnClickedLastTurn)
	ON_BN_CLICKED(IDC_SORT_CARD, OnSortCard)
	ON_BN_CLICKED(IDC_TRUSTEE_CONTROL, OnStusteeControl)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView(void) : CGameFrameView(true,24)
{
	//���ñ���
	m_bDeasilOrder=true;
	m_bLastTurnCard=false;

	//״̬����
	memset(m_bPass,0,sizeof(m_bPass));
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	ZeroMemory(m_lGameScore,sizeof(m_lGameScore));
	ZeroMemory(m_cbWinTaxis,sizeof(m_cbWinTaxis));

	//������Դ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_ImageCenter.SetLoadInfo(IDB_VIEW_CENTER,hInstance);
	m_ImageUserPass.SetLoadInfo(IDB_USER_PASS,hInstance);
	m_ImageScoreInfo.SetLoadInfo(IDB_SCORE_INFO,hInstance);
	m_ImageWinTaxis.SetLoadInfo(IDB_WIN_TAXIS,hInstance);
	m_ImageLastTurnTip.SetLoadInfo(IDB_LAST_TURN_TIP,hInstance);
	m_ImageReady.SetLoadInfo(IDB_Ready,hInstance);          //׼��ͼ��

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
	for (WORD i=0;i<4;i++)
	{
		m_UserCardControl[i].SetDirection(true);
		m_UserCardControl[i].SetDisplayFlag(true);
		m_UserCardControl[i].SetCardSpace(16,0,0);
		m_UserCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,20+i);
	}
	for (WORD i=0;i<2;i++)
	{
		m_HandCardControl[i].SetDirection(true);
		m_HandCardControl[i].SetDisplayFlag(false);
		m_HandCardControl[i].SetCardSpace(16,20,0);
		m_HandCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,30+i);
	}
	for (WORD i=0;i<2;i++)
	{
		m_OtherCardControl[i].SetDirection(false);
		m_OtherCardControl[i].SetDisplayFlag(true);
		m_OtherCardControl[i].SetCardSpace(16,20,0);
		m_OtherCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,40+i);
	}

	//�����˿�
	m_HandCardControl[1].SetSinkWindow(AfxGetMainWnd());

	//������ť
	m_btStart.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_START);
	m_btOutCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_OUT_CARD);
	m_btPassCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_PASS_CARD);
	m_btOutPrompt.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_OUT_PROMPT);
	m_btSortCard.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CreateRect,this,IDC_SORT_CARD);
	m_btLastTurn.Create(TEXT(""),WS_CHILD|WS_DISABLED|WS_VISIBLE,CreateRect,this,IDC_LAST_TURN);
	m_btStusteeControl.Create(TEXT(""),WS_CHILD|WS_DISABLED|WS_VISIBLE,CreateRect,this,IDC_TRUSTEE_CONTROL);

	//���ð�ť
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btStart.SetButtonImage(IDB_START,hInstance,false);
	m_btOutCard.SetButtonImage(IDB_OUT_CARD,hInstance,false);
	m_btPassCard.SetButtonImage(IDB_PASS,hInstance,false);
	m_btOutPrompt.SetButtonImage(IDB_OUT_PROMPT,hInstance,false);
	m_btLastTurn.SetButtonImage(IDB_LAST_OUT_CARD,hInstance,false);
	m_btSortCard.SetButtonImage(IDB_BT_SORT,hInstance,false);
	m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,hInstance,false);

    //��ť��ʾ
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(&m_btLastTurn,TEXT("���ֳ���"));
	m_ToolTipCtrl.AddTool(&m_btStusteeControl,TEXT("�й�"));
	m_ToolTipCtrl.AddTool(&m_btSortCard,TEXT("����"));
	
	//��ȡ����
	m_bDeasilOrder=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),FALSE)?true:false;

	return 0;
}
//��Ϣ����
BOOL CGameClientView::PreTranslateMessage(MSG * pMsg)
{
	m_ToolTipCtrl.RelayEvent(pMsg);
	return __super::PreTranslateMessage(pMsg);
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

//��������
void CGameClientView::SetLastTurnCard(bool bLastTurnCard)
{
	//���ñ���
	if (bLastTurnCard!=m_bLastTurnCard)
	{
		//���ñ���
		m_bLastTurnCard=bLastTurnCard;

		//���½���
		UpdateGameView(NULL);
	}

	return;
}

//���ý���
void CGameClientView::ResetGameView()
{
	m_bLastTurnCard=false;

	//״̬����
	memset(m_bPass,0,sizeof(m_bPass));
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	ZeroMemory(m_lGameScore,sizeof(m_lGameScore));
	ZeroMemory(m_cbWinTaxis,sizeof(m_cbWinTaxis));

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
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_UserCardControl[i].SetCardData(NULL,0);
		m_HandCardControl[i].SetCardData(NULL,0);
		m_HandCardControl[i].SetPositively(false);
	}

	return;
}

//�����ؼ�
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	//��������
	if (m_bDeasilOrder==true)
	{
		m_ptFace[1].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptFace[1].y=nHeight/2-m_nYFace;
		m_ptName[1].x=nWidth-m_nXBorder-m_nXFace-10;
		m_ptName[1].y=nHeight/2-m_nYFace+3;
		m_ptTimer[1].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptTimer[1].y=nHeight/2-m_nYFace-m_nYTimer-5;
		m_ptReady[1].x=nWidth-m_nXBorder-m_nXTimer-5;
		m_ptReady[1].y=nHeight/2-m_nYFace-m_nYTimer/2-5;
		m_PointUserPass[1].x=nWidth-m_nXBorder-m_nXFace-150;
		m_PointUserPass[1].y=nHeight/2-m_nYFace+5;
		m_PointWin[1].x=nWidth-m_nXBorder-m_nXFace-20;
		m_PointWin[1].y=nHeight/2-m_nYFace-m_nYTimer-45;

		m_ptFace[3].x=m_nXBorder+5;
		m_ptFace[3].y=nHeight/2-m_nYFace;
		m_ptName[3].x=m_nXBorder+m_nXFace+10;
		m_ptName[3].y=nHeight/2-m_nYFace+3;
		m_ptTimer[3].x=m_nXBorder+5;
		m_ptTimer[3].y=nHeight/2-m_nYFace-5-m_nYTimer;
		m_ptReady[3].x=m_nXBorder+5+m_nXTimer;
		m_ptReady[3].y=nHeight/2-m_nYFace-m_nYTimer/2-5;
		m_PointUserPass[3].x=m_nXBorder+m_nXFace+107;
		m_PointUserPass[3].y=nHeight/2-m_nYFace+5;
		m_PointWin[3].x=m_nXBorder+5;
		m_PointWin[3].y=nHeight/2-m_nYFace-45-m_nYTimer;
	}
	else
	{
		m_ptFace[3].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptFace[3].y=nHeight/2-m_nYFace;
		m_ptName[3].x=nWidth-m_nXBorder-m_nXFace-10;
		m_ptName[3].y=nHeight/2-m_nYFace+3;
		m_ptTimer[3].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptTimer[3].y=nHeight/2-m_nYFace-m_nYTimer-5;
		m_ptReady[3].x=nWidth-m_nXBorder-m_nXTimer-5;
		m_ptReady[3].y=nHeight/2-m_nYFace-m_nYTimer/2-5;
		m_PointUserPass[3].x=nWidth-m_nXBorder-m_nXFace-150;
		m_PointUserPass[3].y=nHeight/2-m_nYFace+5;
		m_PointWin[3].x=nWidth-m_nXBorder-m_nXFace-20;
		m_PointWin[3].y=nHeight/2-m_nYFace-m_nYTimer-45;

		m_ptFace[1].x=m_nXBorder+5;
		m_ptFace[1].y=nHeight/2-m_nYFace;
		m_ptName[1].x=m_nXBorder+m_nXFace+10;
		m_ptName[1].y=nHeight/2-m_nYFace+3;
		m_ptTimer[1].x=m_nXBorder+5;
		m_ptTimer[1].y=nHeight/2-m_nYFace-5-m_nYTimer;
		m_ptReady[1].x=m_nXBorder+5+m_nXTimer;
		m_ptReady[1].y=nHeight/2-m_nYFace-m_nYTimer/2-5;
		m_PointUserPass[1].x=m_nXBorder+m_nXFace+107;
		m_PointUserPass[1].y=nHeight/2-m_nYFace+5;
		m_PointWin[1].x=m_nXBorder+5;
		m_PointWin[1].y=nHeight/2-m_nYFace-45-m_nYTimer;
	}

	m_ptFace[0].x=nWidth/2-m_nXFace/2;
	m_ptFace[0].y=m_nYBorder+5;
	m_ptName[0].x=nWidth/2+5+m_nXFace/2;
	m_ptName[0].y=m_nYBorder+5;
	m_ptTimer[0].x=nWidth/2-m_nXFace/2-m_nXTimer*2-5;
	m_ptTimer[0].y=m_nYBorder+10;
	m_ptReady[0].x=nWidth/2-m_nXTimer-m_nXFace/2-5;
	m_ptReady[0].y=m_nYBorder+m_nYTimer/2+10;
	m_PointUserPass[0].x=nWidth/2-21;
	m_PointUserPass[0].y=nHeight/2-140;//���ö��Ų�����ʾ��λ��
	m_PointWin[0].x=nWidth/2-m_nXFace/2-m_nXTimer*2-65;
	m_PointWin[0].y=m_nYBorder+10;

	m_ptFace[2].x=nWidth/2-m_nXFace/2;
	m_ptFace[2].y=nHeight-m_nYBorder-m_nYFace-5;
	m_ptName[2].x=nWidth/2+5+m_nXFace/2;
	m_ptName[2].y=nHeight-m_nYBorder-m_nYFace-2;
	m_ptTimer[2].x=nWidth/2-m_nXFace/2-m_nXTimer*2-5;
	m_ptTimer[2].y=nHeight-m_nYBorder-m_nYTimer-10;
	m_ptReady[2].x=nWidth/2-m_nXTimer-m_nXFace/2-5;
	m_ptReady[2].y=nHeight-m_nYBorder-m_nYTimer/2-10;
	m_PointUserPass[2].x=nWidth/2-21;
	m_PointUserPass[2].y=nHeight-m_nYBorder-m_nYFace-210;
	m_PointWin[2].x=nWidth/2-m_nXFace/2-m_nXTimer*2-65;
	m_PointWin[2].y=nHeight-m_nYBorder-m_nYTimer-10;

	//�ƶ���ť
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(11);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	//��ʼ��ť
	m_btStart.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nHeight-196-m_nYBorder-4,0,0,uFlags);

	//���ư�ť
	m_btOutCard.GetWindowRect(&rcButton); //���ó��� ���� ��ʾ��ť�ĸ߶�
	DeferWindowPos(hDwp,m_btOutCard,NULL,nWidth/2-rcButton.Width()*3/2-10,nHeight-196-m_nYBorder-4,0,0,uFlags);
	DeferWindowPos(hDwp,m_btPassCard,NULL,nWidth/2-rcButton.Width()/2,nHeight-196-m_nYBorder-4,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOutPrompt,NULL,nWidth/2+rcButton.Width()/2+10,nHeight-196-m_nYBorder-4,0,0,uFlags);

	//���ܰ�ť
	m_btStusteeControl.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btSortCard,NULL,nWidth-m_nXBorder-(rcButton.Width()*3+15),nHeight-5-rcButton.Height()-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btLastTurn,NULL,nWidth-m_nXBorder-(rcButton.Width()*2+10),nHeight-5-rcButton.Height()-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btStusteeControl,NULL,nWidth-m_nXBorder-(rcButton.Width()+5),nHeight-5-rcButton.Height()-m_nYBorder,0,0,uFlags);

	EndDeferWindowPos(hDwp);

	//�ƶ��˿�
	m_HandCardControl[0].SetBenchmarkPos(nWidth/2,m_nXBorder+66,enXCenter,enYTop);
	m_UserCardControl[0].SetBenchmarkPos(nWidth/2,nHeight/2-175,enXCenter,enYTop);
	m_HandCardControl[1].SetBenchmarkPos(nWidth/2,nHeight-m_nYFace-m_nYBorder-10,enXCenter,enYBottom);
	m_UserCardControl[2].SetBenchmarkPos(nWidth/2,nHeight-205-m_nYBorder,enXCenter,enYBottom);

	//�û��˿�
	if (m_bDeasilOrder==true)
	{
		m_OtherCardControl[0].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-50,nHeight/2-30,enXRight,enYCenter);
		m_OtherCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+50,nHeight/2-30,enXLeft,enYCenter);

		m_UserCardControl[1].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-130,nHeight/2-30,enXRight,enYCenter);
		m_UserCardControl[3].SetBenchmarkPos(m_nXBorder+m_nXFace+130,nHeight/2-30,enXLeft,enYCenter);
	}
	else 
	{
		m_OtherCardControl[0].SetBenchmarkPos(m_nXBorder+m_nXFace+50,nHeight/2-30,enXLeft,enYCenter);
		m_OtherCardControl[1].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-50,nHeight/2-30,enXRight,enYCenter);

		m_UserCardControl[3].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-130,nHeight/2-30,enXRight,enYCenter);
		m_UserCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+130,nHeight/2-30,enXLeft,enYCenter);
	}

	//������ͼ
	CRect rcControl;
	m_ScoreView.GetWindowRect(&rcControl);
	m_ScoreView.SetWindowPos(NULL,(nWidth-rcControl.Width())/2,nHeight/2-160,0,0,SWP_NOZORDER|SWP_NOSIZE);

	return;
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//�滭����
	DrawViewImage(pDC,m_ImageBack,enMode_Spread);
	DrawViewImage(pDC,m_ImageCenter,enMode_Centent);

	//�÷���Ϣ
	CImageHandle ScoreInfoHandle(&m_ImageScoreInfo);
	m_ImageScoreInfo.BitBlt(pDC->m_hDC,nWidth-m_ImageScoreInfo.GetWidth()-2-m_nXBorder,m_nYBorder+2);

	//�滭��Ϣ
	for (int i=0;i<CountArray(m_lGameScore);i++)
	{
		//����λ��
		CRect rcScore;
		rcScore.left=nWidth-m_nXBorder-95;
		rcScore.top=m_nXBorder+6+19*i;
		rcScore.right=nWidth-m_nXBorder-8;
		rcScore.bottom=rcScore.top+16;

		//�滭��Ϣ
		TCHAR szBuffer[16]=TEXT("");
		_sntprintf(szBuffer,CountArray(szBuffer),TEXT("%ld"),m_lGameScore[i]);
		pDC->SetTextAlign(TA_LEFT|TA_TOP);
		pDC->SetTextColor(RGB(255,255,255));
		pDC->DrawText(szBuffer,lstrlen(szBuffer),rcScore,DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	}

	//�滭�û�
	TCHAR szBuffer[32];
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wUserTimer=GetUserTimer(i);
		const tagUserData * pUserData=GetUserInfo(i);

		//�滭�û�
		if (pUserData!=NULL)
		{
			//�û�����
			pDC->SetTextColor((wUserTimer>0)?RGB(250,250,250):RGB(220,220,220));
			pDC->SetTextAlign(((i==1)&&(m_bDeasilOrder==true))||((i==3)&&(m_bDeasilOrder==false))?TA_RIGHT:TA_LEFT);
			pDC->TextOut(m_ptName[i].x,m_ptName[i].y,pUserData->szName,lstrlen(pUserData->szName));

			//ʣ���˿�
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("��%ld �ţ�"),m_bCardCount[i]);
			pDC->TextOut(m_ptName[i].x,m_ptName[i].y+18,szBuffer,lstrlen(szBuffer));

			//�û�����
			if (m_cbWinTaxis[i]!=0)
			{
				CImageHandle HandleWinTaxis(&m_ImageWinTaxis);
				int nImageWidth=m_ImageWinTaxis.GetWidth()/3;
				int nImageHeight=m_ImageWinTaxis.GetHeight();
				m_ImageWinTaxis.BitBlt(pDC->m_hDC,m_PointWin[i].x,m_PointWin[i].y,nImageWidth,nImageHeight,nImageWidth*(m_cbWinTaxis[i]-1),0);
			}

			//������Ϣ
			if (wUserTimer!=0) DrawUserTimer(pDC,m_ptTimer[i].x,m_ptTimer[i].y,wUserTimer);
			//if (pUserData->cbUserStatus==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
            if (pUserData->cbUserStatus==US_READY)//�Զ���׼����ʶ
			{  
			  CImageHandle ImageHnadleReady(&m_ImageReady);
			  //m_ImageReady.AlphaDrawImage(pDC,((-1*i)?(m_ptFace[i].x-(i%2?-20:180)):(m_ptFace[i].x+(i%2?0:120))),m_ptFace[i].y-150*(i%2),m_ImageReady.GetWidth(),m_ImageReady.GetHeight(),0,0,RGB(255,0,255));
			  if(i%2==0)
			  {
				  m_ImageReady.AlphaDrawImage(pDC,m_ptFace[i].x,m_ptFace[i].y-((i==0)?-60:60),m_ImageReady.GetWidth(),m_ImageReady.GetHeight(),0,0,RGB(255,0,255));
			  }
			  else if(i%2==1)
			  {
				  m_ImageReady.AlphaDrawImage(pDC,m_ptFace[i].x-(i%3?0:50),m_ptFace[i].y-60,m_ImageReady.GetWidth(),m_ImageReady.GetHeight(),0,0,RGB(255,0,255));
			  }
			  //m_ImageReady.AlphaDrawImage(pDC,m_PointUserPass[i].x,m_PointUserPass[i].y,m_ImageReady.GetWidth(),m_ImageReady.GetHeight(),0,0,RGB(255,0,255));
			}
			DrawUserFace(pDC,pUserData,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);
		}
	}

	//������־
	CImageHandle ImageHnadleUserPass(&m_ImageUserPass);
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (m_bPass[i]==true) 
		{
			m_ImageUserPass.AlphaDrawImage(pDC,m_PointUserPass[i].x,m_PointUserPass[i].y,RGB(255,0,255));
		}
	}

	//���ֱ�־
	if (m_bLastTurnCard==true)
	{
		//������Դ
		CImageHandle HandleLastTurnTip(&m_ImageLastTurnTip);

		//�滭��־
		int nImageWidth=m_ImageLastTurnTip.GetWidth();
		int nImageHegith=m_ImageLastTurnTip.GetHeight();
		m_ImageLastTurnTip.AlphaDrawImage(pDC,(nWidth-nImageWidth)/2,(nHeight-nImageHegith)/2-50,RGB(255,0,255));
	}

	return;
}

//�û�˳��
void CGameClientView::SetUserOrder(bool bDeasilOrder)
{
	//״̬�ж�
	if (m_bDeasilOrder==bDeasilOrder) return;

	//���ñ���
	m_bDeasilOrder=bDeasilOrder;
	AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),m_bDeasilOrder?TRUE:FALSE);

	//���ý���
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyGameView(rcClient.Width(),rcClient.Height());

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

//���õ÷�
void CGameClientView::SetUserScore(WORD wIndex, LONG lScore)
{
	//���ñ���
	if (wIndex>=CountArray(m_lGameScore))
	{
		ZeroMemory(m_lGameScore,sizeof(m_lGameScore));
	}
	else m_lGameScore[wIndex]=lScore;

	//���½���
	UpdateGameView(NULL);

	return;
}

//��������
void CGameClientView::SetUserTaxis(WORD wChairID, BYTE cbTaxis)
{
	//���ñ���
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<GAME_PLAYER;i++) m_cbWinTaxis[i]=cbTaxis;
	}
	else m_cbWinTaxis[wChairID]=cbTaxis;

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
	AfxGetMainWnd()->SendMessage(IDM_OUT_CARD,0,0);
	return;
}

//������ť
void CGameClientView::OnPassCard()
{
	AfxGetMainWnd()->SendMessage(IDM_PASS_CARD,0,0);
	return;
}

//��ʾ��ť
void CGameClientView::OnOutPrompt()
{
	AfxGetMainWnd()->SendMessage(IDM_OUT_PROMPT,0,0);
	return;
}

//�����˿�
void CGameClientView::OnSortCard()
{
	AfxGetMainWnd()->SendMessage(IDM_SORT_CARD,0,0);
	return;
}

//�й��˿�
void CGameClientView::OnStusteeControl()
{
	AfxGetMainWnd()->SendMessage(IDM_TRUSTEE_CONTROL,0,0);
	return;
}

//���ְ�ť
void CGameClientView::OnBnClickedLastTurn()
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_LAST_TURN_CARD,0,0);

	return;
}

//////////////////////////////////////////////////////////////////////////

