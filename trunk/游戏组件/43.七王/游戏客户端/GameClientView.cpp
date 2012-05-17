#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////
//��ť��ʶ

#define IDC_START						100								//��ʼ��ť
#define IDC_OUT_CARD					101								//���ư�ť
#define IDC_PASS_CARD					102								//PASS��ť
#define IDC_OUT_PROMPT					103								//��ʾ��ť
#define IDC_BAIPAI                      104                             //���ư�ť
#define IDC_TOUXIANG                    105                             //Ͷ����ť
#define IDC_ABNEGATE                    106                             //������ť
#define IDC_JIU							107								//���˰�ť
#define IDC_DISPLAYTEN                  108                             //��ʮ

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_CREATE()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_OUT_CARD, OnOutCard)
	ON_BN_CLICKED(IDC_PASS_CARD, OnPassCard)
	ON_BN_CLICKED(IDC_OUT_PROMPT, OnOutPrompt)
	ON_BN_CLICKED(IDC_BAIPAI,OnBaiPai)
	ON_BN_CLICKED(IDC_TOUXIANG,OnTouXiang)
	ON_BN_CLICKED(IDC_ABNEGATE,OnAbnegate)
	ON_BN_CLICKED(IDC_JIU,OnJiu)
	ON_BN_CLICKED(IDC_DISPLAYTEN,OnDisplayTen)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView(void) : CGameFrameView(true,24)
{
	//���ñ���
	m_bDeasilOrder=true;

	//״̬����
	memset(m_bPass,0,sizeof(m_bPass));
	memset(m_bMost,0,sizeof(m_bMost));
	memset(m_wBanker,0,sizeof(m_wBanker));
	memset(m_bCardCount,0,sizeof(m_bCardCount));

	
	//��ʷ����
	memset(m_lAllTurnScore,0,sizeof(m_lAllTurnScore));
	memset(m_lLastTurnScore,0,sizeof(m_lLastTurnScore));

	//������Դ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_ImageBackGround.SetLoadInfo(IDB_BACK_GROUND,hInstance);
	m_ImageLc.SetLoadInfo(IDB_LC,hInstance);
	m_ImageRc.SetLoadInfo(IDB_RC,hInstance);
	
	m_ImageHeadBg.SetLoadInfo(IDB_HEAD_BG,hInstance);
	m_ImageUserPass.SetLoadInfo(IDB_USER_PASS,hInstance);
	m_RedT_Ten.SetLoadInfo(IDB_TAO_TEN,hInstance);
	m_Fang_Ten.SetLoadInfo(IDB_FANG_TEN,hInstance);
	//�����˿�
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		m_ScoreCardControl[i].SetCardImage(IDB_SCORE_CARD,true);
		m_ScoreCardControl[i].SetCardSpace(13,26,0);
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
	m_ScoreView.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,CreateRect,this,9);

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_UserCardControl[i].SetDirection(true);
		m_UserCardControl[i].SetDisplayFlag(true);
		m_UserCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,20+i);
		m_ScoreCardControl[i].SetDirection(true);
		m_ScoreCardControl[i].SetDisplayFlag(true);
		m_ScoreCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,41+i);
	}
	for (WORD i=0;i<GAME_PLAYER-1;i++)
	{
		m_LeaveCardControl[i].SetDisplayFlag(true);
		m_LeaveCardControl[i].SetCardSpace(20,18,0);
		m_LeaveCardControl[i].SetDirection(false);
		m_LeaveCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,30+i);
	}
	m_HandCardControl.SetSinkWindow(AfxGetMainWnd());
	m_HandCardControl.Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,40);

	//������ť
	m_btStart.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_START);
	m_btOutCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_OUT_CARD);
	m_btPassCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_PASS_CARD);
	m_btTouXiang.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_TOUXIANG);
	m_btBaiPai.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_BAIPAI);
	m_btAbnegate.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_ABNEGATE);
	//m_btOutPrompt.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_OUT_PROMPT);
	m_btJiu.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_JIU);
	m_btDisplayTen.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_DISPLAYTEN);

	//���ð�ť
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btStart.SetButtonImage(IDB_START,hInstance,false);
	m_btOutCard.SetButtonImage(IDB_OUT_CARD,hInstance,false);
	m_btPassCard.SetButtonImage(IDB_PASS,hInstance,false);
	//m_btOutPrompt.SetButtonImage(IDB_OUT_PROMPT,hInstance,false);
	m_btTouXiang.SetButtonImage(IDB_TOUXIANG,hInstance,false);
	m_btBaiPai.SetButtonImage(IDB_BAIPAI,hInstance,false);
	m_btAbnegate.SetButtonImage(IDB_ABNEGATE,hInstance,false);
	m_btJiu.SetButtonImage(IDB_JIU,hInstance,false);
	m_btDisplayTen.SetButtonImage(IDB_DISPLAYTEN,hInstance,false);

	//��ȡ����
	m_bDeasilOrder=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),FALSE)?true:false;

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
	m_HandCardControl.ShootAllCard(false);

	return;
}

//���ý���
void CGameClientView::ResetGameView()
{
	//״̬����
	memset(m_bPass,0,sizeof(m_bPass));
	memset(m_bMost,0,sizeof(m_bMost));
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_wBanker,0,sizeof(m_wBanker));


	//��ʷ����
	memset(m_lAllTurnScore,0,sizeof(m_lAllTurnScore));
	memset(m_lLastTurnScore,0,sizeof(m_lLastTurnScore));

	//���ؿؼ�
	m_ScoreView.ShowWindow(SW_HIDE);

	//���ذ�ť
	m_btStart.ShowWindow(SW_HIDE);
	m_btOutCard.ShowWindow(SW_HIDE);
	m_btPassCard.ShowWindow(SW_HIDE);
	m_btBaiPai.ShowWindow(SW_HIDE);
	m_btAbnegate.ShowWindow(SW_HIDE);
	m_btTouXiang.ShowWindow(SW_HIDE);
	m_btJiu.ShowWindow(SW_HIDE);
	m_btDisplayTen.ShowWindow(SW_HIDE);
	//m_btOutPrompt.ShowWindow(SW_HIDE);

	//���ÿؼ�
	m_btOutCard.EnableWindow(FALSE);
	m_btPassCard.EnableWindow(FALSE);
	//m_btOutPrompt.EnableWindow(FALSE);

	//�˿˿ؼ�
	m_HandCardControl.SetCardData(NULL,0);
	m_HandCardControl.SetPositively(false);
	m_HandCardControl.SetDisplayFlag(false);
	for(WORD i=0;i<GAME_PLAYER-1;i++)m_LeaveCardControl[i].SetCardData(NULL,0);
	for (WORD i=0;i<GAME_PLAYER;i++) m_UserCardControl[i].SetCardData(NULL,0);

	return;
}

//�����ؼ�
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	//��������


	m_ptFace[0].x=nWidth/5-m_nXFace*3;//����
	m_ptFace[0].y=nHeight/5+5;
	m_ptName[0].x=m_ptFace[0].x+m_nXFace/2;
	m_ptName[0].y=m_ptFace[0].y+m_nYFace;
	m_ptTimer[0].x=m_ptFace[0].x;
	m_ptTimer[0].y=m_ptFace[0].y-m_nYFace;
	m_ptReady[0].x=m_ptFace[0].x+3*m_nXFace/2;
	m_ptReady[0].y=m_ptFace[0].y+m_nYFace/2;
	m_PointUserPass[0].x=m_ptFace[0].x;
	m_PointUserPass[0].y=m_ptFace[0].y-2*m_nYFace;

	m_ptFace[1].x=nWidth/5-m_nXFace*3;//����
	m_ptFace[1].y=3*nHeight/5+5;
	m_ptName[1].x=m_ptFace[1].x+m_nXFace/2;
	m_ptName[1].y=m_ptFace[1].y+m_nYFace;
	m_ptTimer[1].x=m_ptFace[1].x;
	m_ptTimer[1].y=m_ptFace[1].y-m_nYFace;
	m_ptReady[1].x=m_ptFace[1].x+3*m_nXFace/2;
	m_ptReady[1].y=m_ptFace[1].y+m_nYFace/2;
	m_PointUserPass[1].x=m_ptFace[1].x;
	m_PointUserPass[1].y=m_ptFace[1].y-2*m_nYFace;

	m_ptFace[2].x=nWidth/2-m_nXFace/2;//�Լ�
	m_ptFace[2].y=nHeight-90;
	m_ptName[2].x=nWidth/2+5+m_nXFace/2;
	m_ptName[2].y=nHeight-80;
	m_ptTimer[2].x=nWidth/2-m_nXFace/2-m_nXTimer*2-5;
	m_ptTimer[2].y=nHeight-90;
	m_ptReady[2].x=nWidth/2-m_nXTimer-m_nXFace/2-5;
	m_ptReady[2].y=nHeight-80;
	m_PointUserPass[2].x=m_ptFace[2].x-2*m_nXFace;
	m_PointUserPass[2].y=m_ptFace[2].y;

	m_ptFace[3].x=4*nWidth/5+m_nXFace*3;//����
	m_ptFace[3].y=3*nHeight/5+5;
	m_ptName[3].x=m_ptFace[3].x+m_nXFace/2;
	m_ptName[3].y=m_ptFace[3].y+m_nYFace;
	m_ptTimer[3].x=m_ptFace[3].x;
	m_ptTimer[3].y=m_ptFace[3].y-m_nYFace;
	m_ptReady[3].x=m_ptFace[3].x-m_nXFace/2;
	m_ptReady[3].y=m_ptFace[3].y+m_nYFace/2;
	m_PointUserPass[3].x=m_ptFace[3].x;
	m_PointUserPass[3].y=m_ptFace[3].y-2*m_nYFace;

	m_ptFace[4].x=4*nWidth/5+m_nXFace*3;//����
	m_ptFace[4].y=nHeight/5+5;
	m_ptName[4].x=m_ptFace[4].x+m_nXFace/2;
	m_ptName[4].y=m_ptFace[4].y+m_nYFace;
	m_ptTimer[4].x=m_ptFace[4].x;
	m_ptTimer[4].y=m_ptFace[4].y-m_nYFace;
	m_ptReady[4].x=m_ptFace[4].x-m_nXFace/2;
	m_ptReady[4].y=m_ptFace[4].y+m_nYFace/2;
	m_PointUserPass[4].x=m_ptFace[4].x;
	m_PointUserPass[4].y=m_ptFace[4].y-2*m_nYFace;

	

	//�ƶ���ť
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(11);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	m_btStart.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nHeight-220-m_nYBorder,0,0,uFlags);

	m_btOutCard.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btOutCard,NULL,nWidth/2-rcButton.Width(),nHeight-240-m_nYBorder,0,0,uFlags);
	m_btTouXiang.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btTouXiang,NULL,nWidth/2-rcButton.Width()*3/2+30-60,nHeight-240-m_nYBorder,0,0,uFlags);
	m_btBaiPai.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btBaiPai,NULL,nWidth/2+rcButton.Width()/2-30-30-100,nHeight-240-m_nYBorder,0,0,uFlags);
	m_btAbnegate.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btAbnegate,NULL,nWidth/2+rcButton.Width()/2+3*m_nXFace,nHeight-220-m_nYBorder+4*m_nYFace,0,0,uFlags);
	DeferWindowPos(hDwp,m_btJiu,NULL,nWidth/2+rcButton.Width()/2-30-30-100,nHeight-240-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btDisplayTen,NULL,nWidth/2+rcButton.Width()/2+3*m_nXFace,nHeight-m_nYBorder-300+4*m_nYFace,0,0,uFlags);
	//DeferWindowPos(hDwp,m_btOutPrompt,NULL,nWidth/2-rcButton.Width()/2,nHeight-193-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btPassCard,NULL,nWidth/2+10,nHeight-240-m_nYBorder,0,0,uFlags);
	EndDeferWindowPos(hDwp);

	//�ƶ��˿�
	m_HandCardControl.SetBenchmarkPos(nWidth/2,nHeight-100,enXCenter,enYBottom);
	

	//�û��˿�
	m_UserCardControl[0].SetBenchmarkPos(m_ptFace[0].x+3*m_nXFace,m_ptFace[0].y,enXCenter,enYCenter);
	m_UserCardControl[1].SetBenchmarkPos(m_ptFace[1].x+3*m_nXFace,m_ptFace[1].y,enXCenter,enYCenter);
	m_UserCardControl[2].SetBenchmarkPos(nWidth/2,nHeight-250-m_nYBorder,enXCenter,enYBottom);
	m_UserCardControl[3].SetBenchmarkPos(m_ptFace[3].x-2*m_nXFace,m_ptFace[3].y,enXCenter,enYCenter);
	m_UserCardControl[4].SetBenchmarkPos(m_ptFace[4].x-2*m_nXFace,m_ptFace[4].y,enXCenter,enYCenter);
	//�����˿�
	m_ScoreCardControl[0].SetBenchmarkPos(nWidth/2+326,nHeight-198-m_nYBorder,enXRight,enYCenter);
	m_ScoreCardControl[1].SetBenchmarkPos(nWidth/2+326,nHeight-198-m_nYBorder+26,enXRight,enYCenter);
	m_ScoreCardControl[2].SetBenchmarkPos(nWidth/2+326,nHeight-198-m_nYBorder+2*26,enXRight,enYCenter);
	m_ScoreCardControl[3].SetBenchmarkPos(nWidth/2+326,nHeight-198-m_nYBorder+3*26,enXRight,enYCenter);
	m_ScoreCardControl[4].SetBenchmarkPos(nWidth/2+326,nHeight-198-m_nYBorder+4*26,enXRight,enYCenter);

	
	
	m_LeaveCardControl[0].SetBenchmarkPos(m_ptFace[0].x+3*m_nXFace,m_ptFace[0].y,enXCenter,enYCenter);
	m_LeaveCardControl[1].SetBenchmarkPos(m_ptFace[1].x+3*m_nXFace,m_ptFace[1].y,enXCenter,enYCenter);
	m_LeaveCardControl[2].SetBenchmarkPos(m_ptFace[3].x-2*m_nXFace,m_ptFace[3].y,enXCenter,enYCenter);
	m_LeaveCardControl[3].SetBenchmarkPos(m_ptFace[4].x-2*m_nXFace,m_ptFace[4].y,enXCenter,enYCenter);

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
	DrawViewImage(pDC,m_ImageBackGround,enMode_Spread);
	DrawViewImage(pDC,m_ImageBack,enMode_Centent);
	CImageHandle HandleLc(&m_ImageLc);
	CImageHandle HandleRc(&m_ImageRc);
	
	CImageHandle HandleHeadBg(&m_ImageHeadBg);

	
	m_ImageLc.AlphaDrawImage(pDC,m_nXBorder,nHeight-m_ImageLc.GetHeight()-m_nYBorder,RGB(255,0,255));
	m_ImageRc.AlphaDrawImage(pDC,nWidth-m_ImageRc.GetWidth()-m_nXBorder,m_nYBorder,RGB(255,0,255));



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
			//m_ImageHeadBg.BitBlt(pDC->m_hDC,m_ptFace[i].x-2,m_ptFace[i].y-2);
       
			//�û�����
			pDC->SetTextColor((wUserTimer>0)?RGB(250,250,250):RGB(220,220,220));
			pDC->SetTextAlign(i==2?TA_TOP:TA_CENTER);
			pDC->TextOut(m_ptName[i].x,m_ptName[i].y,pUserData->szName,lstrlen(pUserData->szName));

			//ʣ���˿�
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("��%ld �ţ�"),m_bCardCount[i]);
			pDC->TextOut(m_ptName[i].x,m_ptName[i].y+18,szBuffer,lstrlen(szBuffer));

			//������Ϣ
			if (wUserTimer!=0) DrawUserTimer(pDC,m_ptTimer[i].x,m_ptTimer[i].y,wUserTimer);
			if (pUserData->cbUserStatus==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			DrawUserFace(pDC,pUserData,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);
		}
	}



	//������־
	CImageHandle ImageHnadleUserPass(&m_ImageUserPass);
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (m_bPass[i]==true) 
		{
			m_ImageUserPass.AlphaDrawImage(pDC,m_PointUserPass[i].x-3*m_nXBorder,m_PointUserPass[i].y,RGB(255,0,255));
		}
	}
	////����־
	//for (WORD i=0;i<GAME_PLAYER;i++)
	//{
	//	//������Ϣ
	//	if (m_bMost[i])
	//	{
	//		//������Դ
	//		CFont InfoFont;
	//		InfoFont.CreateFont(-36,0,0,0,400,0,0,0,134,3,2,1,2,TEXT("����"));

	//		//���� DC
	//		pDC->SetTextColor(RGB(201,229,133));
	//		pDC->SetTextAlign(TA_CENTER|TA_TOP);
	//		CFont * pOldFont=pDC->SelectObject(&InfoFont);

	//		//�滭��Ϣ
	//		pDC->TextOut(m_PointUserPass[i].x-2*m_nXBorder,m_PointUserPass[i].y-m_nYBorder,"���");

	//		//������Դ
	//		pDC->SelectObject(pOldFont);
	//		InfoFont.DeleteObject();
	//	}
	//}
	////����־
	//for (WORD i=0;i<GAME_PLAYER;i++)
	//{
	//	//������Ϣ
	//	if (m_bMost[i])
	//	{
	//		//������Դ
	//		CFont InfoFont;
	//		InfoFont.CreateFont(-36,0,0,0,400,0,0,0,134,3,2,1,2,TEXT("����"));

	//		//���� DC
	//		pDC->SetTextColor(RGB(201,229,133));
	//		pDC->SetTextAlign(TA_CENTER|TA_TOP);
	//		CFont * pOldFont=pDC->SelectObject(&InfoFont);

	//		//�滭��Ϣ
	//		pDC->TextOut(m_PointUserPass[i].x-3*m_nXBorder,m_PointUserPass[i].y,"ׯ��");

	//		//������Դ
	//		pDC->SelectObject(pOldFont);
	//		InfoFont.DeleteObject();
	//	}
	//}



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
//��������־
void CGameClientView::SetMostFlag(WORD wChairID,bool bMost)
{
		//���ñ���
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<GAME_PLAYER;i++) m_bMost[i]=bMost;
	}
	else m_bMost[wChairID]=bMost;

	//���½���
	UpdateGameView(NULL);

	return;

}
//����ׯ�ұ�־
void CGameClientView::SetBankerFlag(WORD wChairID,bool wBanker)
{
		//���ñ���
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<GAME_PLAYER;i++) m_wBanker[i]=wBanker;
	}
	else m_wBanker[wChairID]=wBanker;

	//���½���
	UpdateGameView(NULL);

	return;


}
//һ��һ����
void CGameClientView::ClearOneTime()
{
	//״̬����
	memset(m_bPass,0,sizeof(m_bPass));
	memset(m_bMost,0,sizeof(m_bMost));
	memset(m_wBanker,0,sizeof(m_wBanker));
	for(BYTE i=0;i<GAME_PLAYER;i++)
		m_ScoreCardControl[i].SetCardData(NULL,0);

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
void CGameClientView::OnBaiPai()
{
	//�ؼ�����
	m_btAbnegate.ShowWindow(false);
	m_btBaiPai.ShowWindow(false);
	m_btTouXiang.ShowWindow(false);

	AfxGetMainWnd()->SendMessage(IDM_BAIPAI,0,1);
	return;
}
void CGameClientView::OnAbnegate()
{
	//�ؼ�����
	m_btAbnegate.ShowWindow(false);
	m_btBaiPai.ShowWindow(false);
	m_btTouXiang.ShowWindow(false);
	AfxGetMainWnd()->SendMessage(IDM_ABNEGATE,0,1);
	return;
}
void CGameClientView::OnJiu()
{
	//�ؼ�����
	m_btJiu.ShowWindow(SW_HIDE);
	m_btAbnegate.ShowWindow(SW_HIDE);
	AfxGetMainWnd()->SendMessage(IDM_JIU,0,2);

}
void CGameClientView::OnDisplayTen()
{
	m_btDisplayTen.ShowWindow(SW_HIDE);
	m_btAbnegate.ShowWindow(SW_HIDE);
	AfxGetMainWnd()->SendMessage(IDM_DISPLAYTEN,0,3);
}
//Ͷ����ť
void CGameClientView::OnTouXiang()
{
	//�ؼ�����
	m_btAbnegate.ShowWindow(false);
	m_btBaiPai.ShowWindow(false);
	m_btTouXiang.ShowWindow(false);
	AfxGetMainWnd()->SendMessage(IDM_TOUXIANG,0,2);
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

//LRESULT CALLBACK BProc(HWND	hwnd,UINT message,WPARAM wParam,LPARAM lParam)
//{
//	if(message==WM_RBUTTONDOWN)
//		MessageBox(NULL,"adsf",NULL,NULL);
//	return CallWindowProc(OldProc,hwnd,message,wParam,lParam);
//}



//////////////////////////////////////////////////////////////////////////

