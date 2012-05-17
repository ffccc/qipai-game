#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//��ť��ʶ 
#define IDC_START						100								//��ʼ��ť
#define IDC_ADD_GOLD					101								//��ע��ť
#define IDC_GIVE_UP						102								//������ť
#define IDC_FOLLOW						103								//��ע��ť
#define IDC_SHOW_HAND					104								//�����ť

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_FOLLOW, OnFollow)
	ON_BN_CLICKED(IDC_GIVE_UP, OnGiveUp)
	ON_BN_CLICKED(IDC_ADD_GOLD, OnAddGold)
	ON_BN_CLICKED(IDC_SHOW_HAND, OnShowHand)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView(void) : CGameFrameView(true,24)
{
	//���ñ���
	m_lMaxGold=0L;
	m_lBasicGold=0L;

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	
	m_ImageBarMid.SetLoadInfo(IDB_BAR_MID,hInstance);
	m_ImageBarLeft.SetLoadInfo(IDB_BAR_LEFT,hInstance);
	m_ImageBarRight.SetLoadInfo(IDB_BAR_RIGHT,hInstance);
	m_ImageTitle.SetLoadInfo(IDB_TITLE,hInstance);
	m_ImageBackGround.SetLoadInfo(IDB_BACK_GROUND,hInstance);
	m_ImageHeadBg.SetLoadInfo(IDB_HEAD_BG,hInstance);
	
	if (GetSystemMetrics(SM_CXSCREEN)>=1024) 
	{	
		m_ImageGlodTable.SetLoadInfo(IDB_GLOD_TABLE,hInstance);
		m_ImageTotal.SetLoadInfo(IDB_TOTAL,hInstance);
	}else
	{
		m_ImageGlodTable.SetLoadInfo(IDB_GLOD_TABLE,hInstance);
		m_ImageTotal.SetLoadInfo(IDB_TOTAL_MINI,hInstance);
	}
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
	m_GoldControl.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS,CreateRect,this,8,NULL);
	m_ScoreView.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,CreateRect,this,9);
	for (int i=0;i<4;i++) m_CardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,10+i);

	//������ť
	m_btStart.Create(NULL,WS_CHILD,CreateRect,this,IDC_START);
	m_btFollow.Create(NULL,WS_CHILD,CreateRect,this,IDC_FOLLOW);
	m_btGiveUp.Create(NULL,WS_CHILD,CreateRect,this,IDC_GIVE_UP);
	m_btAddGold.Create(NULL,WS_CHILD,CreateRect,this,IDC_ADD_GOLD);
	m_btShowHand.Create(NULL,WS_CHILD,CreateRect,this,IDC_SHOW_HAND);

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btStart.SetButtonImage(IDB_BT_STAR,hInstance,false);
	m_btFollow.SetButtonImage(IDB_BT_FOLLOW,hInstance,false);
	m_btGiveUp.SetButtonImage(IDB_BT_GIVE_UP,hInstance,false);
	m_btAddGold.SetButtonImage(IDB_BT_ADD_GOLD,hInstance,false);
	m_btShowHand.SetButtonImage(IDB_BT_SHOW_HAND,hInstance,false);

	return 0;
}

//��ʼ��ť
void CGameClientView::OnStart()
{
	AfxGetMainWnd()->SendMessage(IDM_START,0,0);
	return;
}

//��ע��ť
void CGameClientView::OnFollow()
{
	AfxGetMainWnd()->SendMessage(IDM_FOLLOW,0,0);
	return;
}

//������ť
void CGameClientView::OnGiveUp()
{
	AfxGetMainWnd()->SendMessage(IDM_GIVE_UP,0,0);
	return;
}

//��ע��ť
void CGameClientView::OnAddGold()
{
	AfxGetMainWnd()->SendMessage(IDM_ADD_GOLD,0,0);
	return;
}

//�����ť
void CGameClientView::OnShowHand()
{
	AfxGetMainWnd()->SendMessage(IDM_SHOWHAND,0,0);
	return;
}

//���ý���
void CGameClientView::ResetGameView()
{
	//���ñ���
	m_lMaxGold=0L;
	m_lBasicGold=0L;

	//���ÿؼ�
	m_btStart.ShowWindow(SW_HIDE);
	m_btFollow.ShowWindow(SW_HIDE);
	m_btGiveUp.ShowWindow(SW_HIDE);
	m_btAddGold.ShowWindow(SW_HIDE);
	m_btShowHand.ShowWindow(SW_HIDE);
	m_ScoreView.ShowWindow(SW_HIDE);
	m_GoldControl.ShowWindow(SW_HIDE);
	for (int i=0;i<CountArray(m_GoldView);i++) m_GoldView[i].SetGold(0L);
	for (int i=0;i<CountArray(m_CardControl);i++) m_CardControl[i].SetCardData(NULL,0);

	return;
}

//�����ؼ�
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	//��������
	m_ptFace[0].x=nWidth/2+75;
	m_ptFace[0].y=20+m_nYBorder;
	m_ptName[0].x=nWidth/2+75+m_nXFace;
	m_ptName[0].y=20+m_nYBorder;
	m_ptTimer[0].x=nWidth/2+75;
	m_ptTimer[0].y=20+m_nYBorder;
	m_ptReady[0].x=nWidth/2+55+m_nXTimer;
	m_ptReady[0].y=30+m_nYBorder;

	m_ptFace[1].x=nWidth-m_nXBorder-m_nXFace-5;
	m_ptFace[1].y=nHeight/2-m_nYFace-50;
	m_ptName[1].x=nWidth-m_nXBorder-m_nXFace-10;
	m_ptName[1].y=nHeight/2-m_nYFace-50;
	m_ptTimer[1].x=nWidth-m_nXBorder-m_nXFace-5;
	m_ptTimer[1].y=nHeight/2-m_nYFace-55-m_nYTimer;
	m_ptReady[1].x=nWidth-m_nXBorder-m_nXTimer-5;
	m_ptReady[1].y=nHeight/2-m_nYFace-55-m_nYTimer/2;

	m_ptFace[2].x=nWidth/2+72;
	m_ptFace[2].y=nHeight-m_nYBorder-m_nYFace-50;
	m_ptName[2].x=nWidth/2+77+m_nXFace;
	m_ptName[2].y=nHeight-m_nYBorder-m_nYFace-50;
	m_ptTimer[2].x=nWidth/2+72;
	m_ptTimer[2].y=nHeight-m_nYBorder-m_nYFace-55-m_nYTimer;
	m_ptReady[2].x=nWidth/2+72+m_nXTimer;
	m_ptReady[2].y=nHeight-m_nYBorder-m_nYFace-50-m_nYTimer/2;

	m_ptFace[3].x=m_nXBorder+30;
	m_ptFace[3].y=nHeight/2-m_nYFace-50;
	m_ptName[3].x=m_nXBorder+m_nXFace+40;
	m_ptName[3].y=nHeight/2-m_nYFace-50;
	m_ptTimer[3].x=m_nXBorder+30;
	m_ptTimer[3].y=nHeight/2-m_nYFace-55-m_nYTimer;
	m_ptReady[3].x=m_nXBorder+30+m_nXTimer;
	m_ptReady[3].y=nHeight/2-m_nYFace-55-m_nYTimer/2;

	//����ؼ�
	int nXCenterPos=nWidth/2;
	int nYCenterPos=nHeight/2;
	int nXmove=52;
	int nYmove=145;
	if (GetSystemMetrics(SM_CXSCREEN)>=1024)
	{
		m_GoldStation[0].SetPoint(nXCenterPos-nXmove,nYCenterPos-nYmove);
		m_GoldStation[1].SetPoint(nXCenterPos+nXmove,nYCenterPos-nYmove);

		m_GoldStation[2].SetPoint(nXCenterPos+nYmove,nYCenterPos-nXmove);
		m_GoldStation[3].SetPoint(nXCenterPos+nYmove,nYCenterPos+nXmove);

		m_GoldStation[4].SetPoint(nXCenterPos+nXmove,nYCenterPos+nYmove);
		m_GoldStation[5].SetPoint(nXCenterPos-nXmove,nYCenterPos+nYmove);

		m_GoldStation[6].SetPoint(nXCenterPos-nYmove,nYCenterPos+nXmove);
		m_GoldStation[7].SetPoint(nXCenterPos-nYmove,nYCenterPos-nXmove);
	}
	else
	{
		m_GoldStation[0].SetPoint(nXCenterPos-32,nYCenterPos-92);
		m_GoldStation[1].SetPoint(nXCenterPos+28,nYCenterPos-92);
		m_GoldStation[2].SetPoint(nXCenterPos+96,nYCenterPos-27);
		m_GoldStation[3].SetPoint(nXCenterPos+96,nYCenterPos+33);
		m_GoldStation[4].SetPoint(nXCenterPos+28,nYCenterPos+93);
		m_GoldStation[5].SetPoint(nXCenterPos-32,nYCenterPos+93);
		m_GoldStation[6].SetPoint(nXCenterPos-94,nYCenterPos+33);
		m_GoldStation[7].SetPoint(nXCenterPos-94,nYCenterPos-27);
	}

	//��ť�ؼ�
	HDWP hDwp=BeginDeferWindowPos(5);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	//DeferWindowPos(hDwp,m_btStart,NULL,nWidth-m_nXBorder-110,nHeight-m_nYBorder-105,0,0,uFlags);
	//DeferWindowPos(hDwp,m_btStart,NULL,nWidth/2-39,nHeight/2-39,0,0,uFlags);
	//DeferWindowPos(hDwp,m_btAddGold,NULL,nWidth-m_nXBorder-75,nHeight-m_nYBorder-143,0,0,uFlags);
	//DeferWindowPos(hDwp,m_btFollow,NULL,nWidth-m_nXBorder-85,nHeight-m_nYBorder-80,0,0,uFlags);
	//DeferWindowPos(hDwp,m_btGiveUp,NULL,nWidth-m_nXBorder-43,nHeight-m_nYBorder-38,0,0,uFlags);
	//DeferWindowPos(hDwp,m_btShowHand,NULL,nWidth-m_nXBorder-155,nHeight-m_nYBorder-71,0,0,uFlags);
	DeferWindowPos(hDwp,m_btStart,NULL,nWidth-m_nXBorder-100,nHeight-m_nYBorder-105,0,0,uFlags);
	DeferWindowPos(hDwp,m_btAddGold,NULL,nWidth-m_nXBorder-65,nHeight-m_nYBorder-140,0,0,uFlags);
	DeferWindowPos(hDwp,m_btFollow,NULL,nWidth-m_nXBorder-65,nHeight-m_nYBorder-110,0,0,uFlags);
	DeferWindowPos(hDwp,m_btGiveUp,NULL,nWidth-m_nXBorder-65,nHeight-m_nYBorder-80,0,0,uFlags);
	//DeferWindowPos(hDwp,m_btShowHand,NULL,nWidth-m_nXBorder-155,nHeight-m_nYBorder-71,0,0,uFlags);
	EndDeferWindowPos(hDwp);

	//�ɼ�����
	CRect rcScoreView;
	m_ScoreView.GetWindowRect(&rcScoreView);
	m_ScoreView.MoveWindow((nWidth-rcScoreView.Width())/2,(nHeight-rcScoreView.Height())/2-10,rcScoreView.Width(),rcScoreView.Height());
	//m_ScoreView.MoveWindow	(		
	//							(nWidth-rcScoreView.Width())/2,(nHeight-rcScoreView.Height())/2-115,
	//							rcScoreView.Width(),rcScoreView.Height()						
	//							);

	//��ע����
	m_GoldControl.SetBasicPoint(nWidth-80-m_nXBorder,nHeight-85-m_nYBorder);

	//�˿˿ؼ�
	m_CardControl[0].SetBasicStation(CPoint(nWidth/2,22+m_nYBorder),enXCenter,enYTop);
	m_CardControl[1].SetBasicStation(CPoint(nWidth-m_nXBorder-5,nHeight/2),enXRight,enYCenter);
	m_CardControl[2].SetBasicStation(CPoint(nWidth/2,nHeight-40-m_nYBorder),enXCenter,enYBottom);
	m_CardControl[3].SetBasicStation(CPoint(m_nXBorder+30,nHeight/2),enXLeft,enYCenter);

	return;
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//��������
	CImageHandle HandleTitle(&m_ImageTitle);
	//CImageHandle HandleBarMid(&m_ImageBarMid);
	CImageHandle HandleBarLeft(&m_ImageBarLeft);
	CImageHandle HandleBarRight(&m_ImageBarRight);
	CImageHandle HandleGlodTable(&m_ImageGlodTable);
	CImageHandle HandleHeadBg(&m_ImageHeadBg);
	CImageHandle HandleTotal(&m_ImageTotal);
	//�滭����
	//DrawViewImage(pDC,m_ImageBackGround,enMode_Centent);

	DrawViewImage(pDC,m_ImageBackGround,enMode_Spread);
	DrawViewImage(pDC,m_ImageGlodTable,enMode_Centent);
	
	
	//m_ImageGlodTable.BitBlt(pDC->m_hDC,(nWidth-m_ImageGlodTable.GetWidth())/2,(nHeight-m_ImageGlodTable.GetHeight())/2);

	//��������
	//m_ImageBarLeft.BitBlt(pDC->m_hDC,m_nXBorder,nHeight-m_ImageBarLeft.GetHeight()-m_nYBorder);
	//m_ImageBarMid.StretchBlt(pDC->m_hDC,m_nXBorder+m_ImageBarLeft.GetWidth(),nHeight-m_ImageBarMid.GetHeight()-m_nYBorder,nWidth-2*m_nXBorder-m_ImageBarLeft.GetWidth()-m_ImageBarRight.GetWidth(),m_ImageBarMid.GetHeight());
	//m_ImageBarRight.BitBlt(pDC->m_hDC,nWidth-m_nXBorder-m_ImageBarRight.GetWidth(),/*m_ImageBarRight.GetHeight()+*/m_nYBorder);
	m_ImageBarRight.AlphaDrawImage(pDC,nWidth-m_nXBorder-m_ImageBarRight.GetWidth(),m_nYBorder,RGB(255,0,255));
	m_ImageBarLeft.AlphaDrawImage(pDC,m_nXBorder,nHeight-m_ImageBarLeft.GetHeight()-m_nYBorder,RGB(255,0,255));
	//���Ͻ�
	//m_ImageTitle.BitBlt(pDC->m_hDC,m_nXBorder,m_nYBorder);
	m_ImageTitle.AlphaDrawImage(pDC,m_nXBorder+20,m_nYBorder+10,RGB(255,0,255));

	m_ImageTotal.AlphaDrawImage(pDC,nWidth/2-m_ImageTotal.GetWidth()/2,nHeight/2-m_ImageTotal.GetHeight()/2,RGB(255,0,255));
	//�滭�û�
	WORD wUserTimer=0;
	TCHAR szBuffer[64];

	pDC->SetTextColor(RGB(255,255,0));
	for (WORD i=0;i<4;i++)
	{
		//��������
		const tagUserData * pUserData=GetUserInfo(i);
		if (pUserData!=NULL)
		{
			//m_ImageHeadBg.BitBlt(pDC->m_hDC,m_ptFace[i].x-2,m_ptFace[i].y-2);
			//�û�����
			pDC->SetTextAlign((i==1)?(TA_RIGHT|TA_TOP):(TA_LEFT|TA_TOP));
			pDC->TextOut(m_ptName[i].x,m_ptName[i].y,pUserData->szName,lstrlen(pUserData->szName));

			//�û����
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("��%ld"),pUserData->lScore);
			pDC->TextOut(m_ptName[i].x,m_ptName[i].y+16,szBuffer,lstrlen(szBuffer));

			//������Ϣ
			wUserTimer=GetUserTimer(i);
			if (wUserTimer!=0) DrawUserTimer(pDC,m_ptTimer[i].x,m_ptTimer[i].y,wUserTimer);
			if (pUserData->cbUserStatus==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			DrawUserFace(pDC,pUserData->wFaceID,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);

		}
	}

	//��Ϸ��Ϣ
	const UINT uDrawFormat=DT_END_ELLIPSIS|DT_CENTER|DT_SINGLELINE|DT_VCENTER;
	pDC->SetTextAlign(TA_LEFT|TA_TOP);
	pDC->SetTextColor(RGB(255,255,255));
	if (m_lMaxGold!=0) 
	{
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("��%ld"),m_lMaxGold);
		DrawText(pDC->GetSafeHdc(),szBuffer,lstrlen(szBuffer),CRect(91,25,167,40),uDrawFormat);
		//DrawText(pDC->GetSafeHdc(),szBuffer,lstrlen(szBuffer),CRect(86,28,162,43),uDrawFormat);
	}
	if (m_lBasicGold!=0) 
	{
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("��%ld"),m_lBasicGold);
		//DrawText(pDC->GetSafeHdc(),szBuffer,lstrlen(szBuffer),CRect(86,35,162,50),uDrawFormat);
		DrawText(pDC->GetSafeHdc(),szBuffer,lstrlen(szBuffer),CRect(91,47,167,62),uDrawFormat);
	}

	//�滭����
	LONG lTotalGold=0L;
	for (DWORD i=0;i<CountArray(m_GoldView);i++)
	{
		lTotalGold+=m_GoldView[i].GetGold();
		m_GoldView[i].DrawGoldView(pDC,m_GoldStation[i].x,m_GoldStation[i].y);
	}

	//��ע��Ϣ
	if (lTotalGold!=0L)
	{
		CFont ViewFont;
		ViewFont.CreateFont(-24,0,0,0,700,0,0,0,134,3,2,1,1,TEXT("����_GB2312"));
		CFont * pOldFont=pDC->SelectObject(&ViewFont);
		pDC->SetTextColor(RGB(0,0,250));
		pDC->SetTextAlign(TA_CENTER|TA_TOP);
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),lTotalGold);
		pDC->TextOut(nWidth/2,nHeight/2-12,szBuffer,lstrlen(szBuffer));

		pDC->SetTextColor(RGB(255,0,0));
		pDC->TextOut(nWidth/2+1,nHeight/2-12+1,szBuffer,lstrlen(szBuffer));
		pDC->TextOut(nWidth/2+1,nHeight/2-12-1,szBuffer,lstrlen(szBuffer));
		pDC->TextOut(nWidth/2+1,nHeight/2-12+0,szBuffer,lstrlen(szBuffer));
		pDC->TextOut(nWidth/2-1,nHeight/2-12+1,szBuffer,lstrlen(szBuffer));
		pDC->TextOut(nWidth/2-1,nHeight/2-12-1,szBuffer,lstrlen(szBuffer));
		pDC->TextOut(nWidth/2-1,nHeight/2-12+0,szBuffer,lstrlen(szBuffer));
		pDC->TextOut(nWidth/2+0,nHeight/2-12+1,szBuffer,lstrlen(szBuffer));
		pDC->TextOut(nWidth/2+0,nHeight/2-12-1,szBuffer,lstrlen(szBuffer));

		pDC->SelectObject(pOldFont);
		ViewFont.DeleteObject();
	}
	return;
}

//��ұ���
void CGameClientView::SetGoldTitleInfo(DWORD lMaxGold, DWORD lBasicGold)
{
	//���ñ���
	m_lMaxGold=lMaxGold;
	m_lBasicGold=lBasicGold;

	//������ͼ
	UpdateGameView(NULL);

	return;
}

//�û�����
void CGameClientView::SetUserGoldInfo(WORD wViewChairID, bool bTableGold, DWORD dwGold)
{
	//�����ж�
	WORD wGoldPos=wViewChairID*2+((bTableGold==true)?1:0);
	if (wGoldPos>=CountArray(m_GoldStation)) return;

	//���ñ���
	DWORD m_dwAllGold=0L;
	m_GoldView[wGoldPos].SetGold(dwGold);
	for (int i=0;i<CountArray(m_GoldStation);i++) m_dwAllGold+=m_GoldView[i].GetGold();

	//������ͼ
	UpdateGameView(NULL);

	return;
}

//////////////////////////////////////////////////////////////////////////
