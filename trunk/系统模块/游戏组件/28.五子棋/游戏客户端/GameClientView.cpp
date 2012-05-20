#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//��ť��ʶ 
#define IDC_START					100									//��ʼ��ť
#define IDC_REGRET					101									//���尴ť
#define IDC_PEACE					102									//��Ͱ�ť
#define IDC_GIVEUP					103									//���䰴ť
#define IDC_SHOWID					104									//������ť
#define IDC_TRADE					105									//�Ի���ť
#define IDC_PRESERVE				106									//���水ť
#define IDC_STUDY					107									//�о���ť

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_REGRET, OnRegret)
	ON_BN_CLICKED(IDC_PEACE, OnPeace)
	ON_BN_CLICKED(IDC_GIVEUP, OnGiveUp)
	ON_BN_CLICKED(IDC_SHOWID, OnShowID)
	ON_BN_CLICKED(IDC_TRADE, OnTrade)
	ON_BN_CLICKED(IDC_PRESERVE, OnPreserve)
	ON_BN_CLICKED(IDC_STUDY, OnStudy)
	ON_MESSAGE(WM_HIT_CHESS_BORAD, OnHitChessBorad)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView() : CGameFrameView(true,16)
{
	//���ñ���
	m_wGameClock=0;
	m_wBlackUser=INVALID_CHAIR;
	memset(m_wLeftClock,0,sizeof(m_wLeftClock));
	
	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageClock.SetLoadInfo(IDB_CLOCK,hInstance);
	m_ImageBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_ImageFlag.SetLoadInfo(IDB_CHESS_FLAG,hInstance);
	m_ImageDecorate.SetLoadInfo(IDB_DECORATE,hInstance);
	m_ImageUserInfo.SetLoadInfo(IDB_USER_INFO,hInstance);
	m_ImageButtonBack.SetLoadInfo(IDB_BUTTON_BACK,hInstance);

	//��ȡ��С
	CImageHandle UserInfoHandle(&m_ImageUserInfo);
	CImageHandle ButtonBackHandle(&m_ImageButtonBack);
	m_UserInfoSize.cx=m_ImageUserInfo.GetWidth();
	m_UserInfoSize.cy=m_ImageUserInfo.GetHeight();
	m_ButtonBackSize.cx=m_ImageButtonBack.GetWidth();
	m_ButtonBackSize.cy=m_ImageButtonBack.GetHeight();

	return;
}

//��������
CGameClientView::~CGameClientView()
{
}

//������Ϣ
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//�����ؼ�
	CRect CreateRect(0,0,0,0);
	m_GameScoreWnd.Create(IDD_GAME_SCORE,this);
	m_ChessBorad.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,CreateRect,this,11);

	//���ð�ť
	m_btStart.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,CreateRect,this,IDC_START);
	m_btPeace.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,CreateRect,this,IDC_PEACE);
	m_btTrade.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,CreateRect,this,IDC_TRADE);
	m_btRegret.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,CreateRect,this,IDC_REGRET);
	m_btGiveUp.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,CreateRect,this,IDC_GIVEUP);
	m_btPreserve.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,CreateRect,this,IDC_PRESERVE);

	//���԰�ť
	m_btStudy.Create(NULL,WS_CHILD|WS_VISIBLE,CreateRect,this,IDC_STUDY);
	m_btShowID.Create(NULL,WS_CHILD|WS_VISIBLE,CreateRect,this,IDC_SHOWID);

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btStart.SetButtonImage(IDB_START,hInstance,false);
	m_btPeace.SetButtonImage(IDB_PEACE,hInstance,false);
	m_btTrade.SetButtonImage(IDB_TRADE,hInstance,false);
	m_btStudy.SetButtonImage(IDB_STUDY,hInstance,false);
	m_btGiveUp.SetButtonImage(IDB_GIVEUP,hInstance,false);
	m_btRegret.SetButtonImage(IDB_REGRET,hInstance,false);
	m_btShowID.SetButtonImage(IDB_SHOWID,hInstance,false);
	m_btPreserve.SetButtonImage(IDB_PRESERVE,hInstance,false);

	//����ؼ�
	m_PeaceRequest.InitRequest(IDM_PEACE_ANSWER,15,TEXT("�Լ����� [ ���� ] ���Ƿ�ͬ�⣿"));
	m_RegretRequest.InitRequest(IDM_REGRET_ANSWER,15,TEXT("�Լ����� [ ���� ] ���Ƿ�ͬ�⣿"));

	return 0;
}

//���ý���
void CGameClientView::ResetGameView()
{
	//��������
	m_wGameClock=0;
	m_wBlackUser=INVALID_CHAIR;
	memset(m_wLeftClock,0,sizeof(m_wLeftClock));

	//��ť����
	m_btStart.EnableWindow(FALSE);
	m_btRegret.EnableWindow(FALSE);
	m_btPeace.EnableWindow(FALSE);
	m_btGiveUp.EnableWindow(FALSE);
	m_btTrade.EnableWindow(FALSE);
	m_btPreserve.EnableWindow(FALSE);

	//���̿���
	m_ChessBorad.CleanChess();
	m_ChessBorad.SetSelectMode(SM_NO_SELECT);

	//�ؼ�����
	m_GameScoreWnd.ShowWindow(SW_HIDE);
	if (m_PeaceRequest.GetSafeHwnd()) m_PeaceRequest.DestroyWindow();
	if (m_RegretRequest.GetSafeHwnd()) m_RegretRequest.DestroyWindow();

	return;
}

//�����ؼ�
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	//λ�ñ���
	int nXPos=m_nXBorder+5;
	int nYPos=(nHeight-m_UserInfoSize.cy)/2-30;

	//�û�����
	m_ptName[0].x=nXPos+18;
	m_ptName[0].y=nYPos+62;
	m_ptFace[0].x=nXPos+18;
	m_ptFace[0].y=nYPos+24;
	m_ptReady[0].x=nXPos+66;
	m_ptReady[0].y=nYPos-23;
	m_ptTimer[0].x=nXPos+65;
	m_ptTimer[0].y=nYPos+99;
	m_ChessFlagPoint[0].x=nXPos+85;
	m_ChessFlagPoint[0].y=nYPos+27;

	//�û�����
	m_ptFace[1].x=nXPos+18;
	m_ptFace[1].y=nYPos+194;
	m_ptName[1].x=nXPos+18;
	m_ptName[1].y=nYPos+233;
	m_ptReady[1].x=nXPos+66;
	m_ptReady[1].y=nYPos+375;
	m_ptTimer[1].x=nXPos+65;
	m_ptTimer[1].y=nYPos+271;
	m_ChessFlagPoint[1].x=nXPos+85;
	m_ChessFlagPoint[1].y=nYPos+197;

	//��������
	const CSize & BoradSize=m_ChessBorad.GetChessBoradSize();
	nYPos=(nHeight-BoradSize.cy)/2+5;
	nXPos=m_nXBorder+m_UserInfoSize.cx+(nWidth-m_UserInfoSize.cx-BoradSize.cx-2*m_nXBorder)/2;
	if ((nXPos+BoradSize.cx+m_nXBorder)>nWidth) nXPos=nWidth-BoradSize.cx-m_nXBorder;
	m_ChessBorad.MoveWindow(nXPos,nYPos,BoradSize.cx,BoradSize.cy);

	//�����ɼ�
	m_GameScoreWnd.CenterWindow(&m_ChessBorad);

	//������ť
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(8);
	m_btStart.GetWindowRect(&rcButton);
	nYPos=nHeight-m_nYBorder-m_ButtonBackSize.cy-5;
	nXPos=m_nXBorder+m_UserInfoSize.cx+(nWidth-m_UserInfoSize.cx-m_ButtonBackSize.cx)/2;
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	DeferWindowPos(hDwp,m_btStart,NULL,nXPos+4,nYPos+2,0,0,uFlags);
	DeferWindowPos(hDwp,m_btRegret,NULL,nXPos+rcButton.Width()+4,nYPos+2,0,0,uFlags);
	DeferWindowPos(hDwp,m_btPeace,NULL,nXPos+rcButton.Width()*2+4,nYPos+2,0,0,uFlags);
	DeferWindowPos(hDwp,m_btGiveUp,NULL,nXPos+rcButton.Width()*3+4,nYPos+2,0,0,uFlags);
	DeferWindowPos(hDwp,m_btShowID,NULL,nXPos+105,nYPos+45,0,0,uFlags);
	DeferWindowPos(hDwp,m_btTrade,NULL,nXPos+105+rcButton.Width(),nYPos+45,0,0,uFlags);
	DeferWindowPos(hDwp,m_btPreserve,NULL,nXPos+105+rcButton.Width()*2,nYPos+45,0,0,uFlags);
	DeferWindowPos(hDwp,m_btStudy,NULL,nXPos+105+rcButton.Width()*3,nYPos+45,0,0,uFlags);
	EndDeferWindowPos(hDwp);

	return;
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//�滭����
	DrawViewImage(pDC,m_ImageBack,enMode_Spread);

	//װ��λͼ
	CImageHandle DecorateHandle(&m_ImageDecorate);
	m_ImageDecorate.BitBlt(pDC->m_hDC,m_nXBorder+30,m_nYBorder+20);

	//�û�����
	CImageHandle UserInfoHandle(&m_ImageUserInfo);
	int nXPos=m_nXBorder+5;
	int nYPos=(nHeight-m_UserInfoSize.cy)/2-30;
	m_ImageUserInfo.BitBlt(pDC->m_hDC,nXPos,nYPos);

	//���ӱ�־
	if (m_wBlackUser!=INVALID_CHAIR)
	{
		CImageHandle FlagHandle(&m_ImageFlag);
		m_ImageFlag.BitBlt(pDC->m_hDC,m_ChessFlagPoint[0].x,m_ChessFlagPoint[0].y,m_ImageFlag.GetWidth()/2,m_ImageFlag.GetHeight(),
			(m_wBlackUser==0)?0:m_ImageFlag.GetWidth()/2,0);
		m_ImageFlag.BitBlt(pDC->m_hDC,m_ChessFlagPoint[1].x,m_ChessFlagPoint[1].y,m_ImageFlag.GetWidth()/2,m_ImageFlag.GetHeight(),
			(m_wBlackUser==1)?0:m_ImageFlag.GetWidth()/2,0);
	}

	//�滭�û�
	WORD wUserTimer=0;
	pDC->SetTextColor(RGB(255,255,255));
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wUserTimer=GetUserTimer(i);
		//��������
		CRect rcName;
		const tagUserData * pUserData=GetUserInfo(i);
		////////////////////////////////////////////
	//	tagUserData * pUserData=new tagUserData;
	//////	if ( i == 1 ) 
	//	pUserData->cbUserStatus = US_READY;
	//	wUserTimer = 60;
		//////////////////////////////////////////////
		if (pUserData!=NULL)
		{
			//�û�����
			rcName.top=m_ptName[i].y;
			rcName.left=m_ptName[i].x;
			rcName.right=m_ptName[i].x+102;
			rcName.bottom=m_ptName[i].y+12;
			pDC->DrawText(pUserData->szName,lstrlen(pUserData->szName),&rcName,DT_SINGLELINE|DT_END_ELLIPSIS);

			//�û�ʱ��
			DrawUserClock(pDC,m_ptTimer[i].x,m_ptTimer[i].y,m_wGameClock);
			DrawUserClock(pDC,m_ptTimer[i].x,m_ptTimer[i].y+27,m_wLeftClock[i]);

			
			if ( 0 < wUserTimer )
				DrawUserTimer( pDC, m_ptReady[i].x, m_ptReady[i].y, wUserTimer );

			//������Ϣ
			if (pUserData->cbUserStatus==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			DrawUserFace(pDC,pUserData,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);
		}
	}

	//��ť��ͼ
	CImageHandle ButtonBackHandle(&m_ImageButtonBack);
	nYPos=nHeight-m_nYBorder-m_ButtonBackSize.cy-5;
	nXPos=m_nXBorder+m_UserInfoSize.cx+(nWidth-m_UserInfoSize.cx-m_ButtonBackSize.cx)/2;
	m_ImageButtonBack.BitBlt(pDC->m_hDC,nXPos,nYPos);

	return;
}

//��������
void CGameClientView::ShowPeaceRequest()
{
	if (m_PeaceRequest.m_hWnd==NULL) 
	{
		m_PeaceRequest.Create(IDD_USER_REQ,this);
		m_PeaceRequest.ShowWindow(SW_SHOW);
	}

	return;
}

//��������
void CGameClientView::ShowRegretRequest()
{
	if (m_RegretRequest.m_hWnd==NULL) 
	{
		m_RegretRequest.Create(IDD_USER_REQ,this);
		m_RegretRequest.ShowWindow(SW_SHOW);
	}

	return;
}

//���ú���
void CGameClientView::SetBlackUser(WORD wBlackUser)
{
	if (m_wBlackUser!=wBlackUser)
	{
		//���ñ���
		m_wBlackUser=wBlackUser;

		//���½���
		UpdateUserInfoArea();
	}

	return;
}

//����ʱ��
void CGameClientView::SetGameClock(WORD wGameClock)
{
	//���ñ���
	m_wGameClock=wGameClock;

	//���½���
	UpdateUserInfoArea();

	return;
}

//����ʱ��
void CGameClientView::SetUserClock(WORD wViewChairID, WORD wUserClock)
{
	//Ч�����
	ASSERT(wViewChairID<GAME_PLAYER);
	if (wViewChairID>=GAME_PLAYER) return;

	//����ʱ��
	m_wLeftClock[wViewChairID]=wUserClock;

	//���½���
	UpdateUserInfoArea();

	return;
}

//����״̬
void CGameClientView::UpdateUserInfoArea()
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//���ñ���
	CRect rcUserArea;
	rcUserArea.left=m_nXBorder+5;
	rcUserArea.top=(rcClient.Height()-m_UserInfoSize.cy)/2-30;
	rcUserArea.right=rcUserArea.left+m_UserInfoSize.cx;
	rcUserArea.bottom=rcUserArea.top+m_UserInfoSize.cy;

	//���½���
	UpdateGameView(&rcUserArea);

	return;
}

//�û�ʱ��
void CGameClientView::DrawUserClock(CDC * pDC, int nXPos, int nYPos, WORD wClockCount)
{
	//������Դ
	CImageHandle ClockImageHandle(&m_ImageClock);

	//��������
	WORD wMinute=wClockCount/60;
	WORD wSecond=wClockCount%60;
	int nImageWidth=m_ImageClock.GetWidth()/12;
	int nImageHeight=m_ImageClock.GetHeight();
	
	//�滭����
	m_ImageClock.AlphaDrawImage(pDC,nXPos,nYPos,nImageWidth,nImageHeight,wMinute/10*nImageWidth,0,RGB(255,0,255));
	m_ImageClock.AlphaDrawImage(pDC,nXPos+nImageWidth+3,nYPos,nImageWidth,nImageHeight,wMinute%10*nImageWidth,0,RGB(255,0,255));

	//�滭ð��
	m_ImageClock.AlphaDrawImage(pDC,nXPos+(nImageWidth+3)*2,nYPos,nImageWidth,nImageHeight,10*nImageWidth,0,RGB(255,0,255));

	//�滭����
	m_ImageClock.AlphaDrawImage(pDC,nXPos+(nImageWidth+3)*3,nYPos,nImageWidth,nImageHeight,wSecond/10*nImageWidth,0,RGB(255,0,255));
	m_ImageClock.AlphaDrawImage(pDC,nXPos+(nImageWidth+3)*4,nYPos,nImageWidth,nImageHeight,wSecond%10*nImageWidth,0,RGB(255,0,255));

	return;
}

//��ʼ��ť
void CGameClientView::OnStart()
{
	AfxGetMainWnd()->SendMessage(IDM_START,0,0);
	return;
}

//���尴ť
void CGameClientView::OnRegret()
{
	AfxGetMainWnd()->SendMessage(IDM_REGRET,0,0);
	return;
}

//��Ͱ�ť
void CGameClientView::OnPeace()
{
	AfxGetMainWnd()->SendMessage(IDM_PEACE,0,0);
	return;
}

//���䰴ť
void CGameClientView::OnGiveUp()
{
	AfxGetMainWnd()->SendMessage(IDM_GIVEUP,0,0);
	return;
}

//������ť
void CGameClientView::OnShowID()
{
	AfxGetMainWnd()->SendMessage(IDM_SHOW_ID,0,0);
	return;
}

//�Ի���ť
void CGameClientView::OnTrade()
{
	AfxGetMainWnd()->SendMessage(IDM_TRADE,0,0);
	return;
}

//���水ť
void CGameClientView::OnPreserve()
{
	AfxGetMainWnd()->SendMessage(IDM_PRESERVE,0,0);
	return;
}

//�о���ť
void CGameClientView::OnStudy()
{
	AfxGetMainWnd()->SendMessage(IDM_STUDY,0,0);
	return;
}

//�������
LRESULT CGameClientView::OnHitChessBorad(WPARAM wParam, LPARAM lParam)
{
	AfxGetMainWnd()->SendMessage(WM_HIT_CHESS_BORAD,wParam,lParam);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
