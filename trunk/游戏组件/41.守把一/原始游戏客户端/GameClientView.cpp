#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"

//#define DEBUG_GAME
//////////////////////////////////////////////////////////////////////////
//��ť��ʶ 

#define IDC_START						100								//��ʼ��ť
//#define IDC_ONE_SCORE					101								//1 �ְ�ť
//#define IDC_TWO_SCORE					102								//2 �ְ�ť
//#define IDC_THREE_SCORE					103								//3 �ְ�ť
#define IDC_GIVE_UP_SCORE				104								//������ť
#define IDC_OUT_CARD					105								//���ư�ť
#define IDC_PASS_CARD					106								//PASS��ť
#define IDC_AUTO_OUTCARD                107                             //��ʾ��Ŧ

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_CREATE()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_OUT_CARD, OnOutCard)
	ON_BN_CLICKED(IDC_PASS_CARD, OnPassCard)
	//ON_BN_CLICKED(IDC_ONE_SCORE, OnOneScore)
	//ON_BN_CLICKED(IDC_TWO_SCORE, OnTwoScore)
	//ON_BN_CLICKED(IDC_THREE_SCORE, OnThreeScore)
	ON_BN_CLICKED(IDC_AUTO_OUTCARD, OnAutoOutCard)
//	ON_BN_CLICKED(IDC_GIVE_UP_SCORE, OnGiveUpScore)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView(void) : CGameFrameView(true,24)
{
	//��Ϸ����
	m_lBaseScore=0;
//	m_wBombTime=1;
//	m_cbLandScore=0;
//	m_wLandUser=INVALID_CHAIR;

	//���ñ���
	m_bDeasilOrder=true;

	//״̬����
//	m_bLandTitle=false;
	memset(m_bPass,0,sizeof(m_bPass));
//	memset(m_bScore,0,sizeof(m_bScore));
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bEightOne,0,sizeof(m_bEightOne));

	//λ����Ϣ
//	memset(m_ptScore,0,sizeof(m_ptScore));
//	memset(m_ptLand,0,sizeof(m_ptLand));
//	memset(m_byZerodata,0,sizeof(m_byZerodata));

	//������Դ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageBomb.SetLoadInfo(IDB_BOMB,hInstance);
//	m_ImageScore.SetLoadInfo(IDB_SCORE,hInstance);
	m_ImageLand.SetLoadInfo(IDB_BANKER,hInstance);
	m_ImageBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_ImageCenter.SetLoadInfo(IDB_VIEW_CENTER,hInstance);
	m_ImageUserPass.SetLoadInfo(IDB_USER_PASS,hInstance);

	//����λ��
	CImageHandle ImageHandleLand(&m_ImageLand);
//	m_LandSize.cx=m_ImageLand.GetWidth();
//	m_LandSize.cy=m_ImageLand.GetHeight();

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
	for (WORD i=0;i<3;i++)
	{
		m_UserCardControl[i].SetDirection(true);
		m_UserCardControl[i].SetDisplayFlag(true);
		m_UserCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,20+i);
	}
	for (WORD i=0;i<2;i++) 
	{
		//m_FaceCardControl[i].SetDirection(false);
		//m_FaceCardControl[i].SetDisplayFlag(false);
		//m_FaceCardControl[i].SetCardSpace(0,18,0)
		m_LeaveCardControl[i].SetDirection(false);
		m_LeaveCardControl[i].SetDisplayFlag(false);
		m_LeaveCardControl[i].SetCardSpace(0,18,0);
		m_LeaveCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,30+i);
	}
//	m_BackCardControl.SetCardSpace(80,0,0);
//	m_BackCardControl.SetDisplayFlag(true);
	m_HandCardControl.SetSinkWindow(AfxGetMainWnd());
	m_HandCardControl.Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,40);
//	m_BackCardControl.Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,41);

	//������ť
	m_btStart.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_START);
	m_btOutCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_OUT_CARD);
	m_btPassCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_PASS_CARD);
//	m_btOneScore.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_ONE_SCORE);
//	m_btTwoScore.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_TWO_SCORE);
//	m_btThreeScore.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_THREE_SCORE);
//	m_btGiveUpScore.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_GIVE_UP_SCORE);
	m_btAutoOutCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_AUTO_OUTCARD);

	//���ð�ť
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btStart.SetButtonImage(IDB_START,hInstance,false);
	m_btOutCard.SetButtonImage(IDB_OUT_CARD,hInstance,false);
	m_btPassCard.SetButtonImage(IDB_PASS,hInstance,false);
//	m_btOneScore.SetButtonImage(IDB_ONE_SCORE,hInstance,false);
//	m_btTwoScore.SetButtonImage(IDB_TWO_SCORE,hInstance,false);
//	m_btThreeScore.SetButtonImage(IDB_THREE_SCORE,hInstance,false);
//	m_btGiveUpScore.SetButtonImage(IDB_GIVE_UP,hInstance,false);
	m_btAutoOutCard.SetButtonImage(IDB_AUTO_OUT_CARD,hInstance,false);

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
	//��Ϸ����
	m_lBaseScore=0;
	//m_wBombTime=1;
	//m_cbLandScore=0;
	//m_wLandUser=INVALID_CHAIR;

	//״̬����
//	m_bLandTitle=false;
	memset(m_bPass,0,sizeof(m_bPass));
//	memset(m_bScore,0,sizeof(m_bScore));
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bEightOne,0,sizeof(m_bEightOne));

	//���ؿؼ�
	m_ScoreView.ShowWindow(SW_HIDE);

	//���ذ�ť
	m_btStart.ShowWindow(SW_SHOW);
	m_btOutCard.ShowWindow(SW_HIDE);
	m_btPassCard.ShowWindow(SW_HIDE);
//	m_btThreeScore.ShowWindow(SW_HIDE);
//	m_btGiveUpScore.ShowWindow(SW_HIDE);
	m_btAutoOutCard.ShowWindow(SW_HIDE);

	//���ÿؼ�
	m_btOutCard.EnableWindow(FALSE);
	m_btPassCard.EnableWindow(FALSE);
	m_btAutoOutCard.EnableWindow(FALSE);

	//�˿˿ؼ�
//	m_BackCardControl.SetCardData(NULL,0);
	m_HandCardControl.SetCardData(NULL,0);
	m_HandCardControl.SetPositively(false);
	m_HandCardControl.SetDisplayFlag(false);
	m_LeaveCardControl[0].SetCardData(NULL,0);
	m_LeaveCardControl[1].SetCardData(NULL,0);
	for (WORD i=0;i<GAME_PLAYER;i++) m_UserCardControl[i].SetCardData(NULL,0);

	return;
}

//�����ؼ�
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	//��������
	if (m_bDeasilOrder==true)
	{
		m_ptFace[0].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptFace[0].y=nHeight/2-m_nYFace;
		m_ptName[0].x=m_ptFace[2].x+m_nXFace;
		m_ptName[0].y=nHeight/2+10;
		m_ptTimer[0].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptTimer[0].y=nHeight/2-m_nYFace-m_nYTimer-5;
		m_ptReady[0].x=nWidth-m_nXBorder-m_nXTimer-5;
		m_ptReady[0].y=nHeight/2-m_nYFace-m_nYTimer/2-5;
		m_PointUserPass[0].x=nWidth-m_nXBorder-m_nXFace-200;
		m_PointUserPass[0].y=nHeight/2-m_nYFace+5;
		//m_ptScore[0].x=nWidth-m_nXBorder-m_nXFace-200;
		//m_ptScore[0].y=nHeight/2-m_nYFace+5;
		//m_ptLand[0].x=nWidth-m_nXBorder-m_LandSize.cx-3;
		//m_ptLand[0].y=m_ptName[0].y+35;

		m_ptFace[2].x=m_nXBorder+25;
		m_ptFace[2].y=nHeight/2-m_nYFace;
		m_ptName[2].x=m_ptFace[2].x;
		m_ptName[2].y=nHeight/2+10;
		m_ptTimer[2].x=m_nXBorder+25;
		m_ptTimer[2].y=nHeight/2-m_nYFace-5-m_nYTimer;
		m_ptReady[2].x=m_nXBorder+25+m_nXTimer;
		m_ptReady[2].y=nHeight/2-m_nYFace-m_nYTimer/2-5;
		
		m_PointUserPass[2].x=m_nXBorder+m_nXFace+150;
		m_PointUserPass[2].y=nHeight/2-m_nYFace+5;
		//m_ptLand[2].x=m_nXBorder+3;
		//m_ptLand[2].y=m_ptName[2].y+35;
	}
	else
	{
		m_ptFace[2].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptFace[2].y=nHeight/2-m_nYFace;
		m_ptName[2].x=m_ptFace[2].x+m_nXFace;
		m_ptName[2].y=nHeight/2+10;
		m_ptTimer[2].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptTimer[2].y=nHeight/2-m_nYFace-m_nYTimer-5;
		m_ptReady[2].x=nWidth-m_nXBorder-m_nXTimer-5;
		m_ptReady[2].y=nHeight/2-m_nYFace-m_nYTimer/2-5;
		m_PointUserPass[2].x=nWidth-m_nXBorder-m_nXFace-200;
		m_PointUserPass[2].y=nHeight/2-m_nYFace+5;
		//m_ptLand[2].x=nWidth-m_nXBorder-m_LandSize.cx-3;
		//m_ptLand[2].y=m_ptName[2].y+35;

		m_ptFace[0].x=m_nXBorder+25;
		m_ptFace[0].y=nHeight/2-m_nYFace;
		m_ptName[0].x=m_ptFace[0].x;
		m_ptName[0].y=nHeight/2+10;
		m_ptTimer[0].x=m_nXBorder+25;
		m_ptTimer[0].y=nHeight/2-m_nYFace-5-m_nYTimer;
		m_ptReady[0].x=m_nXBorder+25+m_nXTimer;
		m_ptReady[0].y=nHeight/2-m_nYFace-m_nYTimer/2-5;
		m_PointUserPass[0].x=m_nXBorder+m_nXFace+150;
		m_PointUserPass[0].y=nHeight/2-m_nYFace+5;
		//m_ptLand[0].x=m_nXBorder+3;
		//m_ptLand[0].y=m_ptName[0].y+35;
	}

	m_ptFace[1].x=nWidth/2-m_nXFace/2;
	m_ptFace[1].y=nHeight*13/14-m_nYBorder-m_nYFace-5;
	m_ptName[1].x=nWidth/2+5+m_nXFace/2;
	m_ptName[1].y=nHeight*13/14-m_nYBorder-m_nYFace-2;
	m_ptTimer[1].x=nWidth/2-m_nXFace/2-m_nXTimer*2-5;
	m_ptTimer[1].y=nHeight*13/14-m_nYBorder-m_nYTimer-10;
	m_ptReady[1].x=nWidth/2-m_nXTimer-m_nXFace/2-5;
	m_ptReady[1].y=nHeight*13/14-m_nYBorder-m_nYTimer/2-10;
	m_PointUserPass[1].x=nWidth/2-21;
	m_PointUserPass[1].y=nHeight-m_nYBorder-m_nYFace-210;
	//m_ptLand[1].x=nWidth/2-m_nXFace/2-m_LandSize.cx-m_nXTimer*2-10;
	//m_ptLand[1].y=nHeight-m_nXBorder-m_LandSize.cy-20;

	//�ƶ���ť
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	m_btStart.GetWindowRect(&rcButton);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nHeight*13/14-195-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOutCard,NULL,nWidth/2-rcButton.Width()*3/2-10,nHeight*13/14-195-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btAutoOutCard,NULL,nWidth/2-rcButton.Width()/2,nHeight*13/14-195-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btPassCard,NULL,nWidth/2+rcButton.Width()/2+10,nHeight*13/14-195-m_nYBorder,0,0,uFlags);
//	DeferWindowPos(hDwp,m_btOneScore,NULL,nWidth/2-rcButton.Width()*2-30,nHeight-195-m_nYBorder,0,0,uFlags);
	//DeferWindowPos(hDwp,m_btTwoScore,NULL,nWidth/2-rcButton.Width()-10,nHeight-195-m_nYBorder,0,0,uFlags);
	//DeferWindowPos(hDwp,m_btThreeScore,NULL,nWidth/2+10,nHeight-195-m_nYBorder,0,0,uFlags);
//	DeferWindowPos(hDwp,m_btGiveUpScore,NULL,nWidth/2+30+rcButton.Width(),nHeight-195-m_nYBorder,0,0,uFlags);
	EndDeferWindowPos(hDwp);

	//�ƶ��˿�
//	m_BackCardControl.SetBenchmarkPos(nWidth/2,m_nYBorder+10,enXCenter,enYTop);
	m_HandCardControl.SetBenchmarkPos(nWidth/2,nHeight*13/14-m_nYFace-m_nYBorder-10,enXCenter,enYBottom);
	m_UserCardControl[1].SetBenchmarkPos(nWidth/2,nHeight-190-m_nYBorder,enXCenter,enYBottom);

	//�û��˿�
	if (m_bDeasilOrder==true)
	{
		m_UserCardControl[0].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-150,nHeight/2-30,enXRight,enYCenter);
		m_UserCardControl[2].SetBenchmarkPos(m_nXBorder+m_nXFace+150,nHeight/2-30,enXLeft,enYCenter);
		m_LeaveCardControl[0].SetBenchmarkPos(nWidth-m_nXBorder-m_nXFace-30,nHeight/2-50,enXRight,enYCenter);
		m_LeaveCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+50,nHeight/2-55,enXLeft,enYCenter);
	}
	else 
	{
		m_UserCardControl[2].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-150,nHeight/2-30,enXRight,enYCenter);
		m_UserCardControl[0].SetBenchmarkPos(m_nXBorder+m_nXFace+150,nHeight/2-30,enXLeft,enYCenter);
		m_LeaveCardControl[1].SetBenchmarkPos(nWidth-m_nXBorder-m_nXFace-30,nHeight/2-50,enXRight,enYCenter);
		m_LeaveCardControl[0].SetBenchmarkPos(m_nXBorder+m_nXFace+50,nHeight/2-55,enXLeft,enYCenter);
	}

	//������ͼ
	CRect rcControl;
	m_ScoreView.GetWindowRect(&rcControl);
	m_ScoreView.SetWindowPos(NULL,(nWidth-rcControl.Width())/2,(nHeight-rcControl.Height()-100)/2,0,0,SWP_NOZORDER|SWP_NOSIZE);

	return;
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//�滭����
	//pDC->FillSolidRect(10,10,800,650,RGB(0,56,132));
	DrawViewImage(pDC,m_ImageBack,enMode_Spread);
	DrawViewImage(pDC,m_ImageCenter,enMode_Centent);

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
			pDC->SetTextAlign(((i==0)&&(m_bDeasilOrder==true))||((i==2)&&(m_bDeasilOrder==false))?TA_RIGHT:TA_LEFT);
			pDC->TextOut(m_ptName[i].x,m_ptName[i].y,pUserData->szName,lstrlen(pUserData->szName));

			//ʣ���˿�
			if (pUserData->cbUserStatus>=US_PLAY)
			{
				_snprintf(szBuffer,sizeof(szBuffer),TEXT("��%ld �ţ�"),m_bCardCount[i]);
				pDC->TextOut(m_ptName[i].x,m_ptName[i].y+18,szBuffer,lstrlen(szBuffer));
			}

			//������Ϣ
			if (wUserTimer!=0) DrawUserTimer(pDC,m_ptTimer[i].x,m_ptTimer[i].y,wUserTimer);
			if (pUserData->cbUserStatus==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			DrawUserFace(pDC,pUserData->wFaceID,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);
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
//	//�зֱ�־
////	CImageHandle ImageHnadleScore(&m_ImageScore);
//	int nXImagePos,nImageWidth=m_ImageScore.GetWidth(),nImageHeight=m_ImageScore.GetHeight();
//	for (WORD i=0;i<3;i++)
//	{
//		if ((m_bScore[i]!=0)||(m_bPass[i]==true))
//		{
//			if ((m_bPass[i]==true)||(m_bScore[i]==255)) nXImagePos=m_ImageScore.GetWidth()*3/4;
//			else nXImagePos=(m_bScore[i]-1)*m_ImageScore.GetWidth()/4;
//			m_ImageScore.AlphaDrawImage(pDC,m_ptScore[i].x,m_ptScore[i].y,nImageWidth/4,nImageHeight,nXImagePos,0,RGB(255,0,255));
//		}
//	}

	////ׯ��λ��
	//if (m_wLandUser!=INVALID_CHAIR)
	//{
	//	//��ȡ�û�
	//	const tagUserData * pUserData=GetUserInfo(m_wLandUser);
	//	if (pUserData!=NULL)
	//	{
	//		//��������
	//		TCHAR szBuffer[64];
	//		CImageHandle ImageHandleLand(&m_ImageLand);

	//		//���� DC
	//		pDC->SetTextAlign(TA_LEFT|TA_TOP);
	//		pDC->SetTextColor(RGB(250,250,250));

	//		//�滭��Ϣ
	//		m_ImageLand.AlphaDrawImage(pDC,m_ptLand[m_wLandUser].x,m_ptLand[m_wLandUser].y,RGB(255,0,255));
	//		_snprintf(szBuffer,sizeof(szBuffer),TEXT("������%s"),pUserData->szName);
	//		pDC->TextOut(m_nXBorder+m_nXFace+10,m_nYBorder+8,szBuffer,lstrlen(szBuffer));
	//		_snprintf(szBuffer,sizeof(szBuffer),TEXT("������%d ��"),m_cbLandScore);
	//		pDC->TextOut(m_nXBorder+m_nXFace+10,m_nYBorder+25,szBuffer,lstrlen(szBuffer));
	//		DrawUserFace(pDC,pUserData->wFaceID,m_nXBorder+5,m_nYBorder+5,false);
	//	}
	//}

	////ը������
	//if (m_wBombTime>1)
	//{
	//	//ը����־
	//	CImageHandle ImageHandle(&m_ImageBomb);
	//	m_ImageBomb.AlphaDrawImage(pDC,m_nXBorder+5,nHeight-m_nYBorder-m_ImageBomb.GetHeight()-5,RGB(255,0,255));

	//	//���� DC
	//	pDC->SetTextColor(RGB(250,250,250));
	//	pDC->SetTextAlign(TA_LEFT|TA_TOP);

	//	//ը������
	//	TCHAR szBuffer[64];
	//	_snprintf(szBuffer,sizeof(szBuffer),TEXT("ը��������%d"),m_wBombTime);
	//	pDC->TextOut(m_nXBorder+m_ImageBomb.GetWidth()+10,nHeight-m_nYBorder-20,szBuffer,lstrlen(szBuffer));
	//}

	////������Ϣ
	//if (m_bLandTitle==true)
	//{
	//	//������Դ
	//	CFont InfoFont;
	//	InfoFont.CreateFont(-36,0,0,0,400,0,0,0,134,3,2,1,2,TEXT("����"));

	//	//���� DC
	//	pDC->SetTextColor(RGB(201,229,133));
	//	pDC->SetTextAlign(TA_CENTER|TA_TOP);
	//	CFont * pOldFont=pDC->SelectObject(&InfoFont);

	//	//�滭��Ϣ
	//	pDC->TextOut(nWidth/2,200,"���ȴ��û��з�");

	//	//������Դ
	//	pDC->SelectObject(pOldFont);
	//	InfoFont.DeleteObject();
	//}
#ifdef DEBUG_GAME
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		////��������
		//WORD wUserTimer=GetUserTimer(i);
		//const tagUserData * pUserData=GetUserInfo(i);

		//�滭�û�
		//if (pUserData!=NULL)
		{
			//�û�����
			pDC->SetTextColor(RGB(250,250,250));
			pDC->SetTextAlign(((i==0)&&(m_bDeasilOrder==true))||((i==2)&&(m_bDeasilOrder==false))?TA_RIGHT:TA_LEFT);
			pDC->TextOut(m_ptName[i].x,m_ptName[i].y,"��Ϸ���");

			//ʣ���˿�
			/*if (pUserData->cbUserStatus>=US_PLAY)*/
			{
				//_snprintf(szBuffer,sizeof(szBuffer),TEXT("�� �ţ�"),15);
				pDC->TextOut(m_ptName[i].x,m_ptName[i].y+18,"15��");
			}

			//������Ϣ
			/*if (wUserTimer!=0)*/ DrawUserTimer(pDC,m_ptTimer[i].x,m_ptTimer[i].y,15);
			/*if (pUserData->cbUserStatus==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);*/
			DrawUserFace(pDC,5,m_ptFace[i].x,m_ptFace[i].y,true);
		}
	}
#endif
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

////ը������
//void CGameClientView::SetBombTime(WORD wBombTime)
//{
//	//���ñ���
////	m_wBombTime=wBombTime;
//
//	//ˢ�½���
//	UpdateGameView(NULL);
//
//	return;
//}

//��������
void CGameClientView::SetBaseScore(LONG lBaseScore)
{
	//���ñ���
	m_lBaseScore=lBaseScore;

	//ˢ�½���
	UpdateGameView(NULL);

	return;
}

////��ʾ��ʾ
//void CGameClientView::ShowLandTitle(bool bLandTitle)
//{
//	//���ñ���
//	m_bLandTitle=bLandTitle;
//
//	//ˢ�½���
//	UpdateGameView(NULL);
//
//	return;
//}

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

////�ڿӷ���
//void CGameClientView::SetLandUser(WORD wChairID, BYTE bLandScore)
//{
//	//���ñ���
//	m_wLandUser=wChairID;
//	m_cbLandScore=bLandScore;
//
//	//���½���
//	UpdateGameView(NULL);
//
//	return;
//}

////�ڿӷ���
//void CGameClientView::SetLandScore(WORD wChairID, BYTE bLandScore)
//{
//	//���ñ���
//	if (wChairID!=INVALID_CHAIR) m_bScore[wChairID]=bLandScore;
//	else memset(m_bScore,0,sizeof(m_bScore));
//
//	//���½���
//	UpdateGameView(NULL);
//
//	return;
//}

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

////�зְ�ť
//void CGameClientView::OnOneScore()
//{
//	AfxGetMainWnd()->SendMessage(IDM_LAND_SCORE,1,1);
//	return;
//}
//
////�зְ�ť
//void CGameClientView::OnTwoScore()
//{
//	AfxGetMainWnd()->SendMessage(IDM_LAND_SCORE,2,2);
//	return;
//}
//
////�зְ�ť
//void CGameClientView::OnThreeScore()
//{
//	AfxGetMainWnd()->SendMessage(IDM_LAND_SCORE,3,3);
//	return;
//}

//������ť
//void CGameClientView::OnGiveUpScore()
//{
//	AfxGetMainWnd()->SendMessage(IDM_LAND_SCORE,255,255);
//	return;
//}

//������ʾ
void CGameClientView::OnAutoOutCard()
{
	AfxGetMainWnd()->SendMessage(IDM_AUTO_OUTCARD,0,0);
	return;
}

//////////////////////////////////////////////////////////////////////////

