#include "StdAfx.h"
#include "Resource.h"
#include "GameClientDlg.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��ť��ʶ
#define IDC_START						100								//��ʼ��ť
#define IDC_NO_ADD						102								//���Ӱ�ť
#define IDC_FOLLOW						103								//��ע��ť
#define IDC_GIVE_UP						104								//������ť
#define IDC_SHOW_HAND					105								//�����ť

//��ע��ť
#define IDC_ADD_SCORE					113								//��ע��ť

//��ʱ����ʶ
#define IDI_SEND_CARD					100								//���ƶ�ʱ��

//���ƶ���
#define SEND_STEP_COUNT					6								//����
#define SPEED_SEND_CARD					30								//�����ٶ�

//////////////////////////////////////////////////////////////////////////
																			  
BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)							  
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()		//test
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView() : CGameFrameView(true,24)
{
	//��������
	m_SendCardPos.SetPoint(0,0);
	m_SendCardCurPos = m_SendCardPos;
	m_nStepCount = SEND_STEP_COUNT;
	m_nXStep = 0;
	m_nYStep = 0;

	//���ݱ���
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	m_lCellScore = 0L;

	//������Դ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageCard.SetLoadInfo(IDB_CARD,hInstance);
	m_ImageViewBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_ImageViewFill.SetLoadInfo(IDB_VIEW_FILL,hInstance);
	m_PngNumber.LoadImage(hInstance,TEXT("NUMBER"));

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
	CRect rcCreate(0,0,0,0);
	m_ScoreView.Create(NULL,NULL,WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,10);
	m_GoldControl.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS,rcCreate,this,40);

	//������ť
	m_btStart.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,rcCreate,this,IDC_START);
	m_btNoAdd.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,rcCreate,this,IDC_NO_ADD);
	m_btFollow.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,rcCreate,this,IDC_FOLLOW);
	m_btGiveUp.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,rcCreate,this,IDC_GIVE_UP);
	m_btShowHand.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,rcCreate,this,IDC_SHOW_HAND);
	m_btAddScore.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,rcCreate,this,IDC_ADD_SCORE);

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btStart.SetButtonImage(IDB_BT_START,hInstance,false);
	m_btNoAdd.SetButtonImage(IDB_BT_BUJIA,hInstance,false);
	m_btFollow.SetButtonImage(IDB_BT_FOLLOW,hInstance,false);
	m_btGiveUp.SetButtonImage(IDB_BT_FANGQI,hInstance,false);
	m_btShowHand.SetButtonImage(IDB_BT_SOHA,hInstance,false);
	m_btAddScore.SetButtonImage(IDB_BT_JIAZHU,hInstance,false);

	//�����ؼ�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_CardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS,rcCreate,this,20+i);
	}
	//
	m_PlayerJeton[GAME_PLAYER].SetDrawMode(enDrawMode_Rand);

	//test
	//BYTE byCard[] = 
	//{
	//	0x01,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,							//���� A - K
	//	0x11,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,							//÷�� A - K
	//	0x21,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,							//���� A - K
	//	0x31,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,
	//};
	//for( WORD i = 0; i < GAME_PLAYER; i++ )
	//{
	//	m_CardControl[i].SetCardData(byCard,5);
	//	m_PlayerJeton[i].SetScore(123456L);
	//	m_lTableScore[i] = 10000L;
	//}
	//m_PlayerJeton[GAME_PLAYER].SetScore(123456L);
	////m_btStart.ShowWindow(SW_SHOW);
	//m_btNoAdd.ShowWindow(SW_SHOW);
	//m_btFollow.ShowWindow(SW_SHOW);
	//m_btGiveUp.ShowWindow(SW_SHOW);
	//m_btShowHand.ShowWindow(SW_SHOW);
	//m_btAddScore.ShowWindow(SW_SHOW);
	//for( i = 0; i < GAME_PLAYER; i++ )
	//{
	//	m_ScoreView.SetGameScore(i,TEXT("S"),(i%2!=0)?1L:-1L);
	//	m_ScoreView.SetGameTax(i,i%2==0?-1:1);
	//}
	//m_ScoreView.SetGameScore(2,TEXT("sssssssssssS"),0);
	//m_ScoreView.ShowWindow(SW_SHOW);
	//m_lCellScore = 93L;
	//m_GoldControl.SetCellScore(93L);
	//m_GoldControl.ShowWindow(SW_SHOW);
	////end test

	return 0;
}

//�����
BOOL CGameClientView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_START:				//��ʼ��ť
		{
			//������Ϣ
			AfxGetMainWnd()->SendMessage(IDM_START,0,0);

			return TRUE;
		}
	case IDC_NO_ADD:			//���Ӱ�ť
		{
			//������Ϣ
			AfxGetMainWnd()->SendMessage(IDM_NO_ADD,0,0);

			return TRUE;
		}
	case IDC_FOLLOW:			//��ע��ť
		{
			//������Ϣ
			AfxGetMainWnd()->SendMessage(IDM_FOLLOW,0,0);

			return TRUE;
		}
	case IDC_GIVE_UP:			//������ť
		{
			//������Ϣ
			AfxGetMainWnd()->SendMessage(IDM_GIVE_UP,0,0);

			return TRUE;
		}
	case IDC_SHOW_HAND:			//�����ť
		{
			//������Ϣ
			AfxGetMainWnd()->SendMessage(IDM_SHOW_HAND,0,0);

			return TRUE;
		}
	case IDC_ADD_SCORE:			//��ע��ť
		{
			//��ʾ��ע��
			m_btAddScore.EnableWindow(FALSE);
			m_GoldControl.SetAddTimes(0L);
			m_GoldControl.ShowWindow(SW_SHOW);
			return TRUE;
		}
	}

	return __super::OnCommand(wParam, lParam);
}

//���ý���
void CGameClientView::ResetGameView()
{
	//��������
	m_SendCardCurPos = m_SendCardPos;
	m_nStepCount = 0;
	m_nXStep = 0;
	m_nYStep = 0;
	m_SendCardItemArray.RemoveAll();

	//���ݱ���
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	m_lCellScore = 0L;

	//���ó���
	for (WORD i=0;i<GAME_PLAYER+1;i++) 
	{
		m_PlayerJeton[i].SetScore(0L);
	}

	//ɾ����ʱ��
	KillTimer(IDI_SEND_CARD);

	//���ذ�ť
	m_btStart.ShowWindow(SW_HIDE);
	m_btNoAdd.ShowWindow(SW_HIDE);
	m_btGiveUp.ShowWindow(SW_HIDE);
	m_btFollow.ShowWindow(SW_HIDE);
	m_btShowHand.ShowWindow(SW_HIDE);
	m_btAddScore.ShowWindow(SW_HIDE);

	//���ؿؼ�
	m_ScoreView.ShowWindow(SW_HIDE);
	for (WORD i=0;i<GAME_PLAYER;i++) m_CardControl[i].SetCardData(NULL,0);

	return;
}

//�����ؼ�
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	//��������
	m_ptFace[0].x=nWidth/2+330-m_nXFace;
	m_ptFace[0].y=nHeight/2-222;
	m_ptName[0].x=m_ptFace[0].x-5;
	m_ptName[0].y=m_ptFace[0].y+5;
	m_ptTimer[0].x=m_ptName[0].x - 100;
	m_ptTimer[0].y=m_ptName[0].y + 10;
	m_ptReady[0].x=m_ptName[0].x - 120;
	m_ptReady[0].y=m_ptName[0].y + 12;
	m_ptTableScore[0].x = nWidth/2+158;
	m_ptTableScore[0].y = nHeight/2-200;

	//��������
	m_ptFace[1].x=nWidth/2+330-m_nXFace;
	m_ptFace[1].y=nHeight/2-29;
	m_ptName[1].x=m_ptFace[1].x-5;
	m_ptName[1].y=m_ptFace[1].y+5;
	m_ptTimer[1].x=m_ptName[1].x - 100;
	m_ptTimer[1].y=m_ptName[1].y + 10;
	m_ptReady[1].x=m_ptName[1].x - 120;
	m_ptReady[1].y=m_ptName[1].y + 10;
	m_ptTableScore[1].x = nWidth/2+145;
	m_ptTableScore[1].y = nHeight/2+25;

	//��������
	m_ptFace[2].x=nWidth/2-m_nXFace/2;
	m_ptFace[2].y=nHeight/2+215;
	m_ptName[2].x=m_ptFace[2].x+m_nXFace+5;
	m_ptName[2].y=m_ptFace[2].y+5;
	m_ptTimer[2].x=m_ptFace[2].x-m_nXTimer-40;
	m_ptTimer[2].y=m_ptFace[2].y+m_nYFace/2;
	m_ptReady[2].x=nWidth/2;
	m_ptReady[2].y=nHeight/2+53;
	m_ptTableScore[2].x = nWidth/2-30;
	m_ptTableScore[2].y = nHeight/2+75;

	//��������
	m_ptFace[3].x=nWidth/2-340;
	m_ptFace[3].y=nHeight/2-29;
	m_ptName[3].x=m_ptFace[3].x+m_nXFace+5;
	m_ptName[3].y=m_ptFace[3].y+5;
	m_ptTimer[3].x=m_ptName[3].x + 100;
	m_ptTimer[3].y=m_ptName[3].y + 10;
	m_ptReady[3].x=m_ptName[3].x + 120;
	m_ptReady[3].y=m_ptName[3].y + 10;
	m_ptTableScore[3].x = nWidth/2-153;
	m_ptTableScore[3].y = nHeight/2+25;

	//��������
	m_ptFace[4].x=nWidth/2-340;
	m_ptFace[4].y=nHeight/2-222;
	m_ptName[4].x=m_ptFace[4].x+m_nXFace+5;
	m_ptName[4].y=m_ptFace[4].y+5;
	m_ptTimer[4].x=m_ptName[4].x + 100;
	m_ptTimer[4].y=m_ptName[4].y + 10;
	m_ptReady[4].x=m_ptName[4].x + 120;
	m_ptReady[4].y=m_ptName[4].y + 12;
	m_ptTableScore[4].x = nWidth/2-158;
	m_ptTableScore[4].y = nHeight/2-200;

	//�û��˿�
	m_CardControl[0].SetBenchmarkPos(nWidth/2+258,nHeight/2-183,enXCenter,enYTop);
	m_CardControl[1].SetBenchmarkPos(nWidth/2+225,nHeight/2+10,enXCenter,enYTop);
	m_CardControl[2].SetBenchmarkPos(nWidth/2,nHeight/2+200,enXCenter,enYBottom);
	m_CardControl[3].SetBenchmarkPos(nWidth/2-233,nHeight/2+10,enXCenter,enYTop);
	m_CardControl[4].SetBenchmarkPos(nWidth/2-268,nHeight/2-183,enXCenter,enYTop);

	//��ע��λ��
	INT nXGoldControl = nWidth/2+50;
	INT nYGoldControl = nHeight/2+50;
	m_GoldControl.SetBenchmarkPos(nXGoldControl,nYGoldControl);

	//��ť�ؼ�
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	
	//��ʼ��ť
	m_btStart.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btStart,NULL,nWidth/2+58,nHeight/2+264,0,0,uFlags);

	//���ư�ť
	m_btFollow.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btAddScore,NULL,nWidth/2+44,nHeight/2+277,0,0,uFlags);
	DeferWindowPos(hDwp,m_btFollow,NULL,nWidth/2+117,nHeight/2+261,0,0,uFlags);
	DeferWindowPos(hDwp,m_btNoAdd,NULL,nWidth/2+117,nHeight/2+261,0,0,uFlags);
	DeferWindowPos(hDwp,m_btShowHand,NULL,nWidth/2+186,nHeight/2+231,0,0,uFlags);
	DeferWindowPos(hDwp,m_btGiveUp,NULL,nWidth/2+248,nHeight/2+187,0,0,uFlags);

	//�����ƶ�
	EndDeferWindowPos(hDwp);

	//��ҳ���
	m_PlayerJeton[0].SetBenchmarkPos(nWidth/2+138,nHeight/2-140);
	m_PlayerJeton[1].SetBenchmarkPos(nWidth/2+125,nHeight/2-5);
	m_PlayerJeton[2].SetBenchmarkPos(nWidth/2,nHeight/2+55);
	m_PlayerJeton[3].SetBenchmarkPos(nWidth/2-135,nHeight/2-5);
	m_PlayerJeton[4].SetBenchmarkPos(nWidth/2-138,nHeight/2-140);
	//�������
	m_PlayerJeton[5].SetBenchmarkPos(nWidth/2-70,nHeight/2-130);

	//������ʼλ��
	m_SendCardPos.SetPoint(nWidth/2+150,nHeight/2-340);

	//������ͼ
	CRect rcControl;
	m_ScoreView.GetWindowRect(&rcControl);
	m_ScoreView.SetWindowPos(NULL,(nWidth-rcControl.Width())/2,nHeight/2-240,0,0,SWP_NOZORDER|SWP_NOSIZE);
	
	return;
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//�滭����
	DrawViewImage(pDC,m_ImageViewFill,enMode_Spread);
	DrawViewImage(pDC,m_ImageViewBack,enMode_Centent);

	//�滭�û�
	TCHAR szBuffer[64]=TEXT("");
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wUserTimer=GetUserTimer(i);
		const tagUserData * pUserData=GetUserInfo(i);
		//test
		//WORD wUserTimer = 30;
		//tagUserData *pUserData = new tagUserData;
		//lstrcpy(pUserData->szName,TEXT("�������"));
		//pUserData->lScore = 10000L;
		//pUserData->cbUserStatus = US_READY;

		//�滭�û�
		if (pUserData!=NULL)
		{
			//�û�����
			pDC->SetTextAlign(((i==0)||(i==1))?TA_RIGHT:TA_LEFT);
			DrawTextString(pDC,pUserData->szName,RGB(255,255,255),RGB(0,0,0),m_ptName[i].x,m_ptName[i].y);

			//�û����
			LONG lLeaveScore=pUserData->lScore-((pUserData->cbUserStatus==US_PLAY)?m_lTableScore[i]:0);
			_snprintf(szBuffer,CountArray(szBuffer),TEXT("%ld"),lLeaveScore);
			DrawTextString(pDC,szBuffer,RGB(255,255,255),RGB(0,0,0),m_ptName[i].x,m_ptName[i].y+16);
			//��ǰ��ע
			if( m_lTableScore[i] > 0L )
			{
				if( i == 2 ) pDC->SetTextAlign(TA_RIGHT);
				_snprintf(szBuffer,CountArray(szBuffer),TEXT("����ע%ld"),m_lTableScore[i]);
				DrawTextString(pDC,szBuffer,RGB(255,255,255),RGB(0,0,0),m_ptTableScore[i].x,m_ptTableScore[i].y);
			}

			//������Ϣ
			if (wUserTimer!=0) DrawUserTimer(pDC,m_ptTimer[i].x,m_ptTimer[i].y,wUserTimer);
			if (pUserData->cbUserStatus==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			DrawUserFace(pDC,pUserData->wFaceID,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);
		}
		//delete pUserData;

		//��ҳ���
		m_PlayerJeton[i].DrawJettonControl(pDC);
		//��������
		LONG lJetonScore = m_PlayerJeton[i].GetScore();
		if( lJetonScore > 0L )
		{
			CPoint pt = m_PlayerJeton[i].GetBenchmarkPos();
			CRect rcDraw(pt.x-50,pt.y+15,pt.x+50,pt.y+27);
			_snprintf(szBuffer,CountArray(szBuffer),TEXT("%ld"),lJetonScore);
			pDC->SetTextAlign(TA_LEFT);
			DrawTextString(pDC,szBuffer,RGB(255,255,85),RGB(0,0,0),&rcDraw);
		}
	}
	m_PlayerJeton[GAME_PLAYER].DrawJettonControl(pDC);

	//�滭�˿�
	if (m_SendCardItemArray.GetCount()>0)
	{
		//��������
		CImageHandle HandleCard(&m_ImageCard);
		tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];

		//��ȡ��С
		int nItemWidth=m_ImageCard.GetWidth()/13;
		int nItemHeight=m_ImageCard.GetHeight()/5;

		//�滭�˿�
		m_ImageCard.AlphaDrawImage(pDC,m_SendCardCurPos.x,m_SendCardCurPos.y,nItemWidth,nItemHeight,nItemWidth*2,nItemHeight*4,RGB(255,0,255));
	}
	//test
	//CImageHandle HandleCard(&m_ImageCard);
	//m_ImageCard.AlphaDrawImage(pDC,m_SendCardPos.x,m_SendCardPos.y,m_ImageCard.GetWidth()/13,m_ImageCard.GetHeight()/5,
	//	m_ImageCard.GetWidth()/13*2,m_ImageCard.GetHeight()/5*4,RGB(255,0,255));

	INT nNumberWidth = m_PngNumber.GetWidth()/10;
	INT nNumberHeight = m_PngNumber.GetHeight();
	//��ע��Ϣ
	if( m_lCellScore > 0L )
	{
		INT nXPos;
		INT nYPos = nHeight/2-325;
		int nCellCount = 0;
		BYTE byCell[10];
		LONG lCellScore = m_lCellScore;
		while( lCellScore > 0L )
		{
			byCell[nCellCount++] = (BYTE)(lCellScore-lCellScore/10*10);
			lCellScore /= 10;
			if( nCellCount == 10 ) break;
		}
		nXPos = nWidth/2-200-nCellCount*nNumberWidth/2;
		for( int j = nCellCount-1; j >= 0; j-- )
		{
			m_PngNumber.DrawImage(pDC,nXPos,nYPos,nNumberWidth,nNumberHeight,byCell[j]*nNumberWidth,0);
			nXPos += nNumberWidth;
		}
	}
	//������ע
	LONG lTableScore=0L;
	//ע:����������ע����
	for( i = 0; i < GAME_PLAYER; i++ )
		lTableScore += m_PlayerJeton[i].GetScore();
	lTableScore+=m_PlayerJeton[GAME_PLAYER].GetScore();
	//test
	//lTableScore = 1234567L;
	//��ע��Ϣ
	if (lTableScore>0L)
	{
		INT nXPos = nWidth/2+18;
		INT nYPos = nHeight/2-295;
		int nCellCount = 0;
		BYTE byCell[10];
		while( lTableScore > 0L )
		{
			byCell[nCellCount++] = (BYTE)(lTableScore-lTableScore/10*10);
			lTableScore /= 10;
			if( nCellCount == 10 ) break;
		}
		for( int j = nCellCount-1; j >= 0; j-- )
		{
			m_PngNumber.DrawImage(pDC,nXPos,nYPos,nNumberWidth,nNumberHeight,byCell[j]*nNumberWidth,0);
			nXPos += nNumberWidth;
		}
	}

	return;
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

		//
		UpdateGameView(NULL);

		//��������
		CGameClientDlg * pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);
		pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
	}

	return;
}

//������ע
void CGameClientView::SetUserTableScore(WORD wChairID, LONG lTableScore)
{
	//��������
	if (wChairID!=INVALID_CHAIR) m_lTableScore[wChairID]=lTableScore;
	else ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//���½���
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

		//���½���
		UpdateGameView(NULL);

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
				CGameClientDlg * pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);
				pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));

				return;
			}
			else 
			{
				//��ɴ���
				KillTimer(IDI_SEND_CARD);
				AfxGetMainWnd()->PostMessage(IDM_SEND_CARD_FINISH,0,0);

				return;
			}
		}

		return;
	}

	__super::OnTimer(nIDEvent);
}

//��������
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos)
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	pDC->SetTextColor(crFrame);
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
	}

	//�滭����
	pDC->SetTextColor(crText);
	pDC->TextOut(nXPos,nYPos,pszString,nStringLength);

	return;
}

//��������
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect)
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	pDC->SetTextColor(crFrame);
	CRect rcDraw;
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		rcDraw.CopyRect(lpRect);
		rcDraw.OffsetRect(nXExcursion[i],nYExcursion[i]);
		pDC->DrawText(pszString,nStringLength,&rcDraw,DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	}

	//�滭����
	rcDraw.CopyRect(lpRect);
	pDC->SetTextColor(crText);
	pDC->DrawText(pszString,nStringLength,&rcDraw,DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	return;
}

//���õ�Ԫע
void CGameClientView::SetCellScore( LONG lCellScore )
{
	if( m_lCellScore == lCellScore ) return;
	m_lCellScore = lCellScore;
	m_GoldControl.SetCellScore(m_lCellScore);
	UpdateGameView(NULL);
}

//test
void CGameClientView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	//test
	//for( WORD i = 0; i < GAME_PLAYER; i++ )
	//{
	//	DispatchUserCard(i,0x01);
	//}
	//CGameClientDlg * pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);
	//pGameClientDlg->InsertGeneralString(TEXT("\nSSSSSSS"),RGB(255,0,255),true);
	CGameFrameView::OnLButtonDblClk(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////