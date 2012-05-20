#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"
#include "GameClientDlg.h"
#include <MMSystem.h>
#include ".\gameclientview.h"

//////////////////////////////////////////////////////////////////////////

//��ť��ʶ 
#define IDC_START						100								//��ʼ��ť
#define IDC_GIVE_UP						102								//������ť
#define IDC_FOLLOW						103								//��ע��ť
#define IDC_SHOW_HAND					104								//�����ť
#define IDC_ADD_TIMES1					105								//��ְ�ť
#define IDC_ADD_TIMES2					106								//��ְ�ť
#define IDC_ADD_TIMES3					107								//��ְ�ť

//��ʱ����ʶ
#define IDI_SEND_CARD					107								//���ƶ�ʱ��

//���ƶ���
#define SEND_STEP_COUNT					6								//����
#define SPEED_SEND_CARD					10								//�����ٶ�

#define MAX_COUNT						5

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_FOLLOW, OnFollow)
	ON_BN_CLICKED(IDC_GIVE_UP, OnGiveUp)
	ON_BN_CLICKED(IDC_ADD_TIMES1, OnAddTimes1)
	ON_BN_CLICKED(IDC_ADD_TIMES2, OnAddTimes2)
	ON_BN_CLICKED(IDC_ADD_TIMES3, OnAddTimes3)
	ON_BN_CLICKED(IDC_SHOW_HAND, OnShowHand)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView(void) : CGameFrameView(true,24)
{
	//���ñ���
	m_lMaxGold=0L;
	m_lBasicGold=0L;
	m_bShowScoreControl = false;

	//��������
	m_SendCardPos.SetPoint(0,0);
	m_SendCardCurPos = m_SendCardPos;
	m_nStepCount = SEND_STEP_COUNT;
	m_nXStep = 0;
	m_nYStep = 0;

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageCardBack.SetLoadInfo(IDB_CARD_BACK,hInstance);
	m_ImageTitle.SetLoadInfo(IDB_TITLE,hInstance);
	m_ImageViewFill.SetLoadInfo(IDB_VIEW_FILL,hInstance);
	m_ImageViewCenter.SetLoadInfo(IDB_BACK_GROUND,hInstance);
	m_ImageScoreControlBack.SetLoadInfo( IDB_SCORE_CONTROL_BACK,hInstance );
	m_ImageScoreTable.SetLoadInfo( IDB_SCORE_TABLE,hInstance );
	m_PngBkTopLeft.LoadImage( hInstance,TEXT("BK_TOPLEFT") );
	m_PngBkTopRight.LoadImage( hInstance,TEXT("BK_TOPRIGHT") );
	m_PngBkBottomLeft.LoadImage( hInstance,TEXT("BK_BOTTOMLEFT") );
	m_PngBkBottomRight.LoadImage( hInstance,TEXT("BK_BOTTOMRIGHT") );

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
	for (int i=0;i<4;i++) m_CardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,10+i);

	//������ť
	m_btStart.Create(NULL,WS_CHILD,CreateRect,this,IDC_START);
	m_btFollow.Create(NULL,WS_CHILD,CreateRect,this,IDC_FOLLOW);
	m_btGiveUp.Create(NULL,WS_CHILD,CreateRect,this,IDC_GIVE_UP);
	m_btShowHand.Create(NULL,WS_CHILD,CreateRect,this,IDC_SHOW_HAND);
	m_btAddTimes1.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_ADD_TIMES1);
	m_btAddTimes2.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_ADD_TIMES2);
	m_btAddTimes3.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_ADD_TIMES3);

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btStart.SetButtonImage(IDB_RGN_START, hInstance, false);
	m_btFollow.SetButtonImage(IDB_RGN_FOLLOW, hInstance, false);
	m_btGiveUp.SetButtonImage(IDB_RGN_GIVEUP, hInstance, false);
	m_btShowHand.SetButtonImage(IDB_RGN_SHOWHAND, hInstance, false);
	m_btAddTimes1.SetButtonImage(IDB_RGN_ADD_GOLD, hInstance, false);
	m_btAddTimes2.SetButtonImage(IDB_RGN_ADD_GOLD, hInstance, false);
	m_btAddTimes3.SetButtonImage(IDB_RGN_ADD_GOLD, hInstance, false);

	//test
// 	SetGoldTitleInfo( 0,123456789 );
// 	m_btStart.ShowWindow( SW_SHOW );
// 	ShowScoreControl( true );
// 	BYTE byCard[] = {
// 		0x01,0x09,0x0a,0x0b,0x0c
// 	};
// 	for( WORD i = 0; i < GAME_PLAYER; i++ )
// 	{
// 		m_CardControl[i].SetCardData( byCard,5 );
// 		//SetUserGoldInfo( i,false,1 );
// 		//SetUserGoldInfo( i,true,456 );
// 	}
// 	m_CardControl[2].AllowPositively( true );
// 	BYTE cbCardData[] = {0x01,0x08,0x09,0x0A,0x0B};
// 	m_CardControl[2].SetCardData( cbCardData,5 );
// 	m_GoldView[0].SetGold( 123456789L );
// 	m_GoldView[1].SetGold( 123456789L );
// 	SetGoldTitleInfo( 1234567L,123456789L );
// 	m_btFollow.ShowWindow( SW_SHOW );
// 	m_btGiveUp.ShowWindow( SW_SHOW );
// 	m_btShowHand.ShowWindow( SW_SHOW );
// 	m_btAddTimes1.ShowWindow( SW_SHOW );
// 	m_btAddTimes2.ShowWindow( SW_SHOW );
// 	m_btAddTimes3.ShowWindow( SW_SHOW );
// 	ShowScoreControl( true );
// 	for( WORD i = 0; i < GAME_PLAYER; i++ )
// 	{
// 		m_GoldView[i*2].SetGoldIndex(2);
// 		m_GoldView[i*2].SetGold(12345678L);
// 		m_GoldView[i*2+1].SetGold(654321L);
// 	}
	//end test

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
void CGameClientView::OnAddTimes1()
{
	AfxGetMainWnd()->SendMessage(IDM_ADD_GOLD,1,0);
	return;
}

//��ע��ť
void CGameClientView::OnAddTimes2()
{
	AfxGetMainWnd()->SendMessage(IDM_ADD_GOLD,2,0);
	return;
}

//��ע��ť
void CGameClientView::OnAddTimes3()
{
	AfxGetMainWnd()->SendMessage(IDM_ADD_GOLD,3,0);
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
	m_bShowScoreControl = false;

	//��������
	m_SendCardCurPos = m_SendCardPos;
	m_nStepCount = 0;
	m_nXStep = 0;
	m_nYStep = 0;
	m_SendCardItemArray.RemoveAll();

	//���ÿؼ�
	m_btStart.ShowWindow(SW_HIDE);
	m_btFollow.ShowWindow(SW_HIDE);
	m_btGiveUp.ShowWindow(SW_HIDE);
	m_btShowHand.ShowWindow(SW_HIDE);
	m_ScoreView.ShowWindow(SW_HIDE);

	//���ó���
	for (int i=0;i<CountArray(m_GoldView);i++) m_GoldView[i].SetGold(0L);

	//�����˿�
	for (int i=0;i<CountArray(m_CardControl);i++)
	{
		m_CardControl[i].SetCardData(NULL,0);
		m_CardControl[i].ShowFirstCard(false);
		m_CardControl[i].AllowPositively(false);
	}

	return;
}

//�����ؼ�
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	//��������
	m_ptFace[0].x=nWidth/2;
	m_ptFace[0].y=5+m_nYBorder;
	m_ptName[0].x=nWidth/2+m_nXFace+5;
	m_ptName[0].y=5+m_nYBorder;
	m_ptTimer[0].x=nWidth/2-m_nXFace/2-m_nXTimer/2-5;
	m_ptTimer[0].y=m_nYBorder+m_nYTimer/2+2;
	m_ptReady[0].x=nWidth/2-m_nXFace-15;
	m_ptReady[0].y=2+m_nYBorder+m_nYTimer/2;

	m_ptFace[1].x=nWidth-m_nXBorder-m_nXFace-5;
	m_ptFace[1].y=nHeight/2-m_nYFace-58;
	m_ptName[1].x=nWidth-m_nXBorder-m_nXFace-10;
	m_ptName[1].y=nHeight/2-m_nYFace-58;
	m_ptTimer[1].x=nWidth-m_nXBorder-m_nXTimer/2-2;
	m_ptTimer[1].y=nHeight/2-m_nYFace-83;
	m_ptReady[1].x=nWidth-(m_nXBorder+48);
	m_ptReady[1].y=nHeight/2-m_nYFace-83;

	m_ptFace[2].x=nWidth/2;
	m_ptFace[2].y=nHeight-m_nYBorder-m_nYFace-5;
	m_ptName[2].x=nWidth/2+m_nYFace+5;
	m_ptName[2].y=nHeight-m_nYBorder-m_nYFace-5;
	m_ptTimer[2].x=nWidth/2-m_nXFace/2-m_nXTimer/2-5;
	m_ptTimer[2].y=nHeight-m_nYBorder-m_nYTimer/2-2;
	m_ptReady[2].x=nWidth/2 + 10;
	m_ptReady[2].y=nHeight-217-m_nYBorder;

	m_ptFace[3].x=m_nXBorder+5;
	m_ptFace[3].y=nHeight/2-m_nYFace-58;
	m_ptName[3].x=m_nXBorder+m_nXFace+10;
	m_ptName[3].y=nHeight/2-m_nYFace-58;
	m_ptTimer[3].x=m_nXBorder+m_nXTimer/2;
	m_ptTimer[3].y=nHeight/2-m_nYFace-83;
	m_ptReady[3].x=m_nXBorder+45;
	m_ptReady[3].y=nHeight/2-m_nYFace-83;

	//����ؼ�
	int nXCenterPos=nWidth/2;
	int nYCenterPos=nHeight/2;
	m_GoldStation[0].SetPoint(nXCenterPos,nYCenterPos-90);
	m_GoldStation[1].SetPoint(nXCenterPos+79,nYCenterPos-90);
	m_GoldStation[2].SetPoint(nXCenterPos+91,nYCenterPos-3);
	m_GoldStation[3].SetPoint(nXCenterPos+91,nYCenterPos+65);
	m_GoldStation[4].SetPoint(nXCenterPos,nYCenterPos+82);
	m_GoldStation[5].SetPoint(nXCenterPos-79,nYCenterPos+82);
	m_GoldStation[6].SetPoint(nXCenterPos-90,nYCenterPos-5);
	m_GoldStation[7].SetPoint(nXCenterPos-90,nYCenterPos-71);

	//��ע��λ��
// 	m_ptScoreControl.SetPoint(nWidth/2+200,nHeight/2+200);
	m_ptScoreControl.SetPoint(nWidth+200,nHeight+200);
	INT nXControl = m_ptScoreControl.x+6;
	INT nYControl = m_ptScoreControl.y+7;

	//��ť�ؼ�
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	
	//��ʼ��ť
	CRect rcStart;
	m_btStart.GetWindowRect(&rcStart);
// 	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcStart.Width())/2+10,nHeight/2+156,0,0,uFlags);
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcStart.Width())/2+50,nHeight-50,0,0,uFlags);

	//���ư�ť
	m_btFollow.GetWindowRect(&rcButton);
// 	DeferWindowPos(hDwp,m_btFollow,NULL,nXControl,nYControl,0,0,uFlags);
// 	DeferWindowPos(hDwp,m_btAddTimes2,NULL,nXControl,nYControl+rcButton.Height()+5,0,0,uFlags);
// 	DeferWindowPos(hDwp,m_btAddTimes1,NULL,nXControl,nYControl+rcButton.Height()*2+10,0,0,uFlags);
// 	DeferWindowPos(hDwp,m_btShowHand,NULL,nXControl+rcButton.Width()+5,nYControl,0,0,uFlags);
// 	DeferWindowPos(hDwp,m_btAddTimes3,NULL,nXControl+rcButton.Width()+5,nYControl+rcButton.Height()+5,0,0,uFlags);
// 	DeferWindowPos(hDwp,m_btGiveUp,NULL,nXControl+rcButton.Width()+5,nYControl+rcButton.Height()*2+10,0,0,uFlags);

	DeferWindowPos(hDwp,m_btGiveUp,NULL,10,nHeight-50,0,0,uFlags);
	DeferWindowPos(hDwp,m_btFollow,NULL,nWidth-rcButton.Width()-10,nHeight-50,0,0,uFlags);
	DeferWindowPos(hDwp,m_btShowHand,NULL,nWidth-rcButton.Width()-10,nHeight-50-100,0,0,uFlags);
	DeferWindowPos(hDwp,m_btAddTimes1,NULL,nWidth-rcButton.Width()-130,nHeight-50-100,0,0,uFlags);
	DeferWindowPos(hDwp,m_btAddTimes2,NULL,nWidth-rcButton.Width()-130,nHeight-50-50,0,0,uFlags);
	DeferWindowPos(hDwp,m_btAddTimes3,NULL,nWidth-rcButton.Width()-130,nHeight-50,0,0,uFlags);

	//�����ƶ�
	EndDeferWindowPos(hDwp);

	//�ɼ�����
	CRect rcScoreView;
	m_ScoreView.GetWindowRect(&rcScoreView);
	m_ScoreView.MoveWindow((nWidth-rcScoreView.Width())/2,(nHeight-rcScoreView.Height())/2-10,rcScoreView.Width(),rcScoreView.Height());

	//�˿˿ؼ�
	m_CardControl[0].SetBasicStation(CPoint(nWidth/2,52+m_nYBorder),enXCenter,enYTop);
	m_CardControl[1].SetBasicStation(CPoint(nWidth-m_nXBorder-5,nHeight/2),enXRight,enYCenter);
	m_CardControl[2].SetBasicStation(CPoint(nWidth/2,nHeight-55-m_nYBorder),enXCenter,enYBottom);
	m_CardControl[3].SetBasicStation(CPoint(m_nXBorder+5,nHeight/2),enXLeft,enYCenter);

	//������ʼλ��
	m_SendCardPos.SetPoint(nWidth/2-40,nHeight/2-52);

	return;
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//�滭����
	CImageHandle HandleTitle(&m_ImageTitle);
	DrawViewImage(pDC,m_ImageViewFill,enMode_Centent);
	//DrawViewImage(pDC,m_ImageViewCenter,enMode_Centent);
	m_ImageTitle.BitBlt(pDC->m_hDC,m_nXBorder+80,m_nYBorder);
	//�Ľ�
	m_PngBkTopLeft.DrawImage( pDC,0,0 );
	m_PngBkTopRight.DrawImage( pDC,nWidth-m_PngBkTopRight.GetWidth(),0 );
	m_PngBkBottomLeft.DrawImage( pDC,0,nHeight-m_PngBkBottomLeft.GetHeight() );
	m_PngBkBottomRight.DrawImage( pDC,nWidth-m_PngBkBottomRight.GetWidth(),nHeight-m_PngBkBottomRight.GetHeight() );

	//
	CImageHandle HandleScoreTable(&m_ImageScoreTable);
	m_ImageScoreTable.AlphaDrawImage( pDC,nWidth/2-m_ImageScoreTable.GetWidth()/2,nHeight/2-m_ImageScoreTable.GetHeight()/2,
		RGB(255,0,255) );

	//�滭�û�
	WORD wUserTimer=0;
	TCHAR szBuffer[64];
	pDC->SetTextColor(RGB(255,255,0));
	for (WORD i=0;i<4;i++)
	{
		//��������
		const tagUserData * pUserData=GetUserInfo(i);
		
		////////////////////////////////////////////////////////
		//tagUserData *pUserData = new tagUserData;
		//strcpy( pUserData->szName, "dkfjdkfj" );
		//pUserData->cbUserStatus = US_READY ;
		//wUserTimer = 50;
		//////////////////////////////////////////////////////////
		

		if (pUserData!=NULL)
		{
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
			DrawUserFace(pDC,pUserData,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);
		}
	}

	//�滭����
	LONG lTotalGold=0L;
	for (DWORD i=0;i<CountArray(m_GoldView);i++)
	{
		lTotalGold+=m_GoldView[i].GetGold();
		m_GoldView[i].DrawGoldView(pDC,m_GoldStation[i].x,m_GoldStation[i].y);
	}

	//��Ϸ��Ϣ
	pDC->SetTextAlign(TA_LEFT|TA_TOP);
	pDC->SetTextColor(RGB(250,250,250));
	const UINT uDrawFormat=DT_END_ELLIPSIS|DT_CENTER|DT_SINGLELINE|DT_VCENTER;

	//��Ϸ��ע
	if (lTotalGold!=0L)
	{
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),lTotalGold);
		DrawText(pDC->m_hDC,szBuffer,lstrlen(szBuffer),CRect(m_nXBorder+80+80,m_nYBorder+48,m_nXBorder+140+80,m_nYBorder+60),uDrawFormat);
		//m_TotalGoldView.SetGold( lTotalGold );
		//m_TotalGoldView.DrawGoldView(pDC,nWidth/2-1,nHeight/2+2);
		//��������
		CFont ViewFont;
		ViewFont.CreateFont(-12,0,0,0,700,0,0,0,134,3,2,1,1,TEXT("����"));
		CFont * pOldFont=pDC->SelectObject(&ViewFont);
		
		DrawText( pDC->m_hDC,szBuffer,lstrlen(szBuffer),CRect(nWidth/2-40,nHeight/2-10,nWidth/2+40,nHeight/2+10),uDrawFormat );
		//������Դ
		pDC->SelectObject(pOldFont);
	}

	//�����ע
	if (m_lMaxGold!=0)
	{
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),m_lMaxGold);
		DrawText(pDC->m_hDC,szBuffer,lstrlen(szBuffer),CRect(m_nXBorder+80+80,m_nYBorder+10,m_nXBorder+140+80,m_nYBorder+22),uDrawFormat);
	}

	//������ע
	if (m_lBasicGold!=0)
	{
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),m_lBasicGold);
		DrawText(pDC->m_hDC,szBuffer,lstrlen(szBuffer),CRect(m_nXBorder+80+80,m_nYBorder+29,m_nXBorder+140+80,m_nYBorder+41),uDrawFormat);
	}

	//��ʾ��ע���Ʊ���
	if( m_bShowScoreControl )
	{
		CImageHandle HandleScoreControl(&m_ImageScoreControlBack);
		m_ImageScoreControlBack.AlphaDrawImage( pDC,m_ptScoreControl.x,m_ptScoreControl.y,RGB(255,0,255) );
	}

	//�滭�˿�
	if (m_SendCardItemArray.GetCount()>0)
	{
		//��������
		CImageHandle HandleCard(&m_ImageCardBack);
		tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];

		//�滭�˿�
		m_ImageCardBack.AlphaDrawImage(pDC,m_SendCardCurPos.x,m_SendCardCurPos.y,RGB(255,0,255));
	}

	return;
}

//��ұ���
void CGameClientView::SetGoldTitleInfo(DWORD lMaxGold, DWORD lBasicGold)
{
	//���ñ���
	m_lMaxGold=lMaxGold;
	m_lBasicGold=lBasicGold;

	//���°�ť����
	TCHAR szNum[16];
	_snprintf(szNum,CountArray(szNum),TEXT("%ld"),m_lBasicGold);
	m_btAddTimes1.SetWindowText(szNum);
	_snprintf(szNum,CountArray(szNum),TEXT("%ld"),m_lBasicGold*2);
 	m_btAddTimes2.SetWindowText(szNum);
	_snprintf(szNum,CountArray(szNum),TEXT("%ld"),m_lBasicGold*3);
 	m_btAddTimes3.SetWindowText(szNum);

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

//��ʱ����Ϣ
void CGameClientView::OnTimer(UINT nIDEvent)
{
	//���ʹ���
	if (nIDEvent==IDI_SEND_CARD)
	{
		//����λ��
		m_SendCardCurPos.x += m_nXStep;
		m_SendCardCurPos.y += m_nYStep;


		//ֹͣ�ж�
		if ( --m_nStepCount == 0 )
		{
			//��ȡ�˿�
			BYTE cbCardData[MAX_COUNT];
			WORD wChairID=m_SendCardItemArray[0].wChairID;
			BYTE cbCardCount=(BYTE)m_CardControl[wChairID].GetCardData(cbCardData,CountArray(cbCardData));

			//�����˿�
			if (cbCardCount<MAX_COUNT)
			{
				cbCardData[cbCardCount++]=m_SendCardItemArray[0].cbCardData;
				m_CardControl[wChairID].SetCardData(cbCardData,cbCardCount);
			}

			//ɾ������
			m_SendCardItemArray.RemoveAt(0);

			//��������
			if (m_SendCardItemArray.GetCount()>0)
			{
				//
				wChairID = m_SendCardItemArray[0].wChairID;
				m_nStepCount = SEND_STEP_COUNT;
				m_SendCardCurPos = m_SendCardPos;
				m_nXStep = (m_CardControl[wChairID].GetTailPos().x-m_SendCardPos.x)/m_nStepCount;
				m_nYStep = (m_CardControl[wChairID].GetTailPos().y-m_SendCardPos.y)/m_nStepCount;

				//��������
				PlaySound(TEXT("GET_CARD"), AfxGetInstanceHandle(), SND_ASYNC|SND_RESOURCE|SND_NODEFAULT);

				//���½���
				UpdateGameView(NULL);

				return;
			}
			else 
			{
				//��ɴ���
				KillTimer(IDI_SEND_CARD);
				AfxGetMainWnd()->PostMessage(IDM_SEND_CARD_FINISH,0,0);

				//���½���
				UpdateGameView(NULL);

				return;
			}
		}

		//���½���
		UpdateGameView(NULL);

		return;

	}
	__super::OnTimer(nIDEvent);
}

//�����ж�
bool CGameClientView::IsDispatchCard()
{
	return m_SendCardItemArray.GetCount()>0;
}

//��ɷ���
void CGameClientView::FinishDispatchCard()
{
	//����ж�
	if (m_SendCardItemArray.GetCount()==0) return;

	//��ɶ���
	for (INT_PTR i=0;i<m_SendCardItemArray.GetCount();i++)
	{
		//��ȡ����
		tagSendCardItem * pSendCardItem=&m_SendCardItemArray[i];

		//��ȡ�˿�
		BYTE cbCardData[MAX_COUNT];
		WORD wChairID=pSendCardItem->wChairID;
		BYTE cbCardCount=(BYTE)m_CardControl[wChairID].GetCardData(cbCardData,CountArray(cbCardData));

		//�����˿�
		if (cbCardCount<MAX_COUNT)
		{
			cbCardData[cbCardCount++]=pSendCardItem->cbCardData;
			m_CardControl[wChairID].SetCardData(cbCardData,cbCardCount);
		}
	}

	//ɾ������
	KillTimer(IDI_SEND_CARD);
	m_SendCardItemArray.RemoveAll();

	//���½���
	UpdateGameView(NULL);

	return;
}

//�����˿�
void CGameClientView::DispatchUserCard(WORD wChairID, BYTE cbCardData)
{
	//Ч�����
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return;

	//���ñ���
	tagSendCardItem SendCardItem;
	SendCardItem.wChairID=wChairID;
	SendCardItem.cbCardData=cbCardData;
	m_SendCardItemArray.Add(SendCardItem);

	//��������
	if (m_SendCardItemArray.GetCount()==1) 
	{
		//���ö�ʱ��
		SetTimer(IDI_SEND_CARD,SPEED_SEND_CARD,NULL);

		//
		m_nStepCount = SEND_STEP_COUNT;
		m_SendCardCurPos = m_SendCardPos;
		m_nXStep = (m_CardControl[wChairID].GetTailPos().x-m_SendCardPos.x)/m_nStepCount;
		m_nYStep = (m_CardControl[wChairID].GetTailPos().y-m_SendCardPos.y)/m_nStepCount;

		//��������
		PlaySound(TEXT("GET_CARD2"), AfxGetInstanceHandle(), SND_ASYNC|SND_RESOURCE|SND_NODEFAULT);

		UpdateGameView(NULL);

	}

	return;
}

//
VOID CGameClientView::ShowScoreControl( bool bShow )
{
	int nCmdShow = bShow?SW_SHOW:SW_HIDE;
	//���ý���
	m_btFollow.ShowWindow(nCmdShow);
	m_btGiveUp.ShowWindow(nCmdShow);
	m_btShowHand.ShowWindow(nCmdShow);
	m_btAddTimes1.ShowWindow(nCmdShow);
	m_btAddTimes2.ShowWindow(nCmdShow);
	m_btAddTimes3.ShowWindow(nCmdShow);

	m_bShowScoreControl = bShow;
	UpdateGameView( NULL );
}

//////////////////////////////////////////////////////////////////////////
