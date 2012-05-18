#include "StdAfx.h"
#include "..\���������\GameLogic.h"
#include "GameClient.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////
//�궨��
#define ACCOUNTS_HEIGHT 32
//��ը��ʶ
#define IDI_BOMB_EFFECT					101								//��ը��ʶ

//��ը��Ŀ
#define BOMB_EFFECT_COUNT				6								//��ը��Ŀ

//////////////////////////////////////////////////////////////////////////
//��ť��ʶ 

//���ư�ť
#define IDC_START						100								//��ʼ��ť
#define IDC_TRUSTEE						101								//�йܿ���
#define IDC_LAST_TURN					102								//�����˿�
#define IDC_SORT_CARD_ORDER				103								//�˿�����
#define IDC_SORT_CARD_COLOR				104								//�˿�����
#define IDC_SORT_CARD_COUNT				105								//�˿�����
#define IDC_SORT_CARD					106								//�˿�����

//��Ϸ��ť
#define IDC_OUT_CARD					120								//���ư�ť
#define IDC_PASS_CARD					121								//PASS��ť
#define IDC_OUT_PROMPT	                122                             //��ʾ��ť
#define IDC_TRUSTEE_CONTROL				203								//�йܿ���
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)

	//ϵͳ��Ϣ
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()

	//���ư�ť
	ON_BN_CLICKED(IDC_START, OnBnClickedStart)
	ON_BN_CLICKED(IDC_TRUSTEE, OnBnClickedStustee)
	ON_BN_CLICKED(IDC_LAST_TURN, OnBnClickedLastTurn)

	ON_BN_CLICKED(IDC_SORT_CARD, OnBnClickedSort)
	ON_BN_CLICKED(IDC_SORT_CARD_COLOR, OnBnClickedSortCardColor)
	ON_BN_CLICKED(IDC_SORT_CARD_COUNT, OnBnClickedSortCardCount)
	ON_BN_CLICKED(IDC_SORT_CARD_ORDER, OnBnClickedSortCardOrder)
	
	//��Ϸ��ť
	ON_BN_CLICKED(IDC_OUT_CARD, OnBnClickedOutCard)
	ON_BN_CLICKED(IDC_PASS_CARD, OnBnClickedPassCard)
	ON_BN_CLICKED(IDC_OUT_PROMPT, OnBnClickedOutPrompt)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
	//��Ϸ����
	m_lCellScore=0L;
	m_cbMainValue=0;
	ZeroMemory(m_cbValueOrder,sizeof(m_cbValueOrder));
	m_bShowFirstCard=true;
	m_nLButtonUpCount=0;

	//�÷ֱ���
	m_TurnScore=0;
	ZeroMemory(m_PlayerScore,sizeof(m_PlayerScore));

	//�û�״̬
	ZeroMemory(m_bUserPass,sizeof(m_bUserPass));
	ZeroMemory(m_bUserContinue,sizeof(m_bUserContinue));

	//��Ϸ״̬
	ZeroMemory(m_wWinOrder,sizeof(m_wWinOrder));
	ZeroMemory(m_wPersistInfo,sizeof(m_wPersistInfo));

	//��ը����
	m_bBombEffect=false;
	m_cbBombFrameIndex=0;

	//�ƶ�����
	m_bMoveMouse=false;
	m_bSelectCard=false;
	m_bSwitchCard=false;
	m_wHoverCardItem=INVALID_ITEM;
	m_wMouseDownItem=INVALID_ITEM;

	//״̬����
	m_bShowScore=false;
	m_bLastTurnCard=false;

	//������Դ
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_ImageNumber.SetLoadInfo(IDB_NUMBER,hResInstance);
	m_ImageViewFill.SetLoadInfo(IDB_VIEW_FILL,hResInstance);
	m_ImageViewBack.SetLoadInfo(IDB_VIEW_BACK,hResInstance);
	m_ImageUserPass.SetLoadInfo(IDB_USER_PASS,hResInstance);
	m_ImageStatusInfo.SetLoadInfo(IDB_STATUS_INFO,hResInstance);
	m_ImageBombEffect.SetLoadInfo(IDB_BOMB_EFFECT,hResInstance);
	m_ImageValueOrder.SetLoadInfo(IDB_VALUE_ORDER,hResInstance);
	m_ImageLastTurnTip.SetLoadInfo(IDB_LAST_TURN_TIP,hResInstance);
	m_ImageTurnScore.SetLoadInfo(IDB_TURN_SCORE,hResInstance);

	return;
}

//��������
CGameClientView::~CGameClientView()
{
}

//��Ϣ����
BOOL CGameClientView::PreTranslateMessage(MSG * pMsg)
{
	//��ʾ��Ϣ
	if (m_ToolTipCtrl.m_hWnd!=NULL) 
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}

	return __super::PreTranslateMessage(pMsg);
}

//���ý���
void CGameClientView::ResetGameView()
{
	//ɾ��ʱ��
	KillTimer(IDI_BOMB_EFFECT);

	//�÷ֱ���
	m_TurnScore=0;
	ZeroMemory(m_PlayerScore,sizeof(m_PlayerScore));

	//��Ϸ����
	m_lCellScore=0L;
	m_cbMainValue=0;
	ZeroMemory(m_cbValueOrder,sizeof(m_cbValueOrder));
	m_bShowFirstCard=true;
	m_nLButtonUpCount=0;

	//�û�״̬
	ZeroMemory(m_bUserPass,sizeof(m_bUserPass));
	ZeroMemory(m_bUserContinue,sizeof(m_bUserContinue));

	//��Ϸ״̬
	ZeroMemory(m_wWinOrder,sizeof(m_wWinOrder));
	ZeroMemory(m_wPersistInfo,sizeof(m_wPersistInfo));

	//�ƶ�����
	m_bMoveMouse=false;
	m_bSelectCard=false;
	m_bSwitchCard=false;
	m_wHoverCardItem=INVALID_ITEM;
	m_wMouseDownItem=INVALID_ITEM;

	//��ը����
	m_bBombEffect=false;
	m_cbBombFrameIndex=0;

	//״̬����
	m_bShowScore=false;
	m_bLastTurnCard=false;

	//���ư�ť
	m_btStart.ShowWindow(SW_HIDE);

	//��ֹ��ť
	m_btStustee.EnableWindow(FALSE);
	m_btLastTurn.EnableWindow(FALSE);
	m_btSort.EnableWindow(FALSE);
	m_btSortCardCount.EnableWindow(FALSE);
	m_btSortCardColor.EnableWindow(FALSE);
	m_btSortCardOrder.EnableWindow(FALSE);

	//��Ϸ��ť
	m_btOutCard.ShowWindow(SW_HIDE);
	m_btPassCard.ShowWindow(SW_HIDE);
	m_btOutPrompt.ShowWindow(SW_HIDE);

	//��ԭ��ť
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_btStustee.SetButtonImage(IDB_BT_START_TRUSTEE,hResInstance,false);

	//�û��˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_UserCardControl[i].SetCardData(NULL,0);
		m_HandCardControl[i].SetCardData(NULL,0);
		m_HandCardControl[i].SetDisplayItem(false);
	}

	//�˿˿ؼ�
	m_HandCardControl[MYSELF_VIEW_ID].SetPositively(false);

	return ;
}

//�����ؼ�
VOID CGameClientView::RectifyGameView(INT nWidth, INT nHeight)
{

	//��ҽ���Ԫ������
	//�Լ����-0��ʾ
	m_ptFace[0].x=(nWidth-m_nXFace)/2;						//ͷ��ˮƽ����
	m_ptFace[0].y=5;										//ͷ������ϱ�5������
	m_ptName[0].x=m_ptFace[0].x+m_nXFace+10;				//���ֱ�ͷ����10������
	m_ptName[0].y=m_ptFace[0].y;							//���־����ϱ���ͷ����ͬ
	m_ptTimer[0].x=m_ptFace[0].x-m_nXTimer/2-5;				//
	m_ptTimer[0].y=m_ptFace[0].y+m_nYTimer/2;
	m_ptReady[0].x=nWidth/2;
	m_ptReady[0].y=200;
	m_ptAuto[0].x=nWidth/2+m_nXFace+m_nXTimer+35;
	m_ptAuto[0].y=m_nYBorder+5;

	//����Լ�-2��ʾ
	m_ptFace[2].x=(nWidth-m_nXFace)/2;						//ͷ��ˮƽ����
	m_ptFace[2].y=nHeight-m_nYTimer-5;						//ͷ�����ؾ���ױ�5������
	m_ptName[2].x=m_ptFace[2].x+m_nXFace+10;				//���ֱ�ͷ����10������
	m_ptName[2].y=m_ptFace[2].y;
	m_ptTimer[2].x=m_ptFace[2].x-m_nXTimer/2-5;
	m_ptTimer[2].y=m_ptFace[2].y+m_nYTimer/2;
	m_ptReady[2].x=nWidth/2;
	m_ptReady[2].y=nHeight-260;
	m_ptAuto[2].x=nWidth/2+m_nXFace+m_nXTimer+35;
	m_ptAuto[2].y=m_ptFace[2].y;//nHeight-m_nYBorder-m_nYFace-10;

	//�Ҳ����-1��ʾ
	m_ptFace[1].x=nWidth-m_nXFace*2-20;
	m_ptFace[1].y=(nHeight-m_nYFace)/2-50;
	m_ptName[1].x=m_ptFace[1].x;//nWidth-m_nXFace-10;
	m_ptName[1].y=m_ptFace[1].y+m_nYFace+10;
	m_ptTimer[1].x=m_ptFace[1].x+m_nXTimer/2;
	m_ptTimer[1].y=m_ptFace[1].y-m_nYTimer/2-5;
	m_ptReady[1].x=nWidth*7/10;
	m_ptReady[1].y=nHeight/2-50;
	m_ptAuto[1].x=m_ptFace[1].x+5;//nWidth-m_nXBorder-m_nXFace-5;
	m_ptAuto[1].y=nHeight/2-m_nYBorder-m_nYFace-m_nYTimer-83;
	
	//������-3��ʾ
	m_ptFace[3].x=5;
	m_ptFace[3].y=(nHeight-m_nYFace)/2-50;
	m_ptName[3].x=5;
	m_ptName[3].y=m_ptFace[3].y+m_nYFace+10;
	m_ptTimer[3].x=m_ptFace[3].x+m_nXTimer/2;
	m_ptTimer[3].y=m_ptFace[3].y-m_nYTimer/2-5;
	m_ptReady[3].x=nWidth*3/10;
	m_ptReady[3].y=nHeight/2-50;
	m_ptAuto[3].x=m_nXBorder+5;
	m_ptAuto[3].y=nHeight/2-m_nYBorder-m_nYFace-m_nYTimer-83;

	

	//�˿�λ��
	m_ptHeap[0].SetPoint(m_ptName[0].x+5,m_ptName[0].y+18);
	m_ptHeap[1].SetPoint(m_ptName[1].x+15,m_ptName[1].y+18);
	m_ptHeap[2].SetPoint(m_ptName[2].x-5,m_ptName[2].y+18);
	m_ptHeap[3].SetPoint(m_ptName[3].x+5,m_ptName[3].y+18);

	//����λ��
	m_ptPass[0].SetPoint(nWidth/2,180);
	m_ptPass[1].SetPoint(nWidth*3/10,nHeight/2-50);
	m_ptPass[2].SetPoint(nWidth/2,nHeight-260);
	m_ptPass[3].SetPoint(nWidth*7/10,nHeight/2-50);

	//�û��˿�
	m_HandCardControl[0].SetBenchmarkPos(nWidth/2,60,enXCenter,enYTop);
	m_HandCardControl[1].SetBenchmarkPos(75,nHeight/2-50,enXLeft,enYCenter);
	m_HandCardControl[2].SetBenchmarkPos(nWidth/2,nHeight-60,enXCenter,enYBottom);
	m_HandCardControl[3].SetBenchmarkPos(nWidth-75,nHeight/2-50,enXRight,enYCenter);

	//�����˿�
	m_UserCardControl[0].SetBenchmarkPos(nWidth/2,145,enXCenter,enYTop);
	m_UserCardControl[1].SetBenchmarkPos(nWidth*7/10,nHeight/2-50,enXCenter,enYCenter);
	m_UserCardControl[2].SetBenchmarkPos(nWidth/2,nHeight-300,enXCenter,enYCenter);
	m_UserCardControl[3].SetBenchmarkPos(nWidth*3/10,nHeight/2-50,enXCenter,enYCenter);

	m_5RecordCardControl.SetBenchmarkPos(5,75,enXLeft,enYTop);
	m_10RecordCardControl.SetBenchmarkPos(5,105,enXLeft,enYTop);
	m_KRecordCardControl.SetBenchmarkPos(5,135,enXLeft,enYTop);
	m_5RecordCardControl.SetDisplayItem(true);
	m_10RecordCardControl.SetDisplayItem(true);
	m_KRecordCardControl.SetDisplayItem(true);

	//�ƶ��ؼ�
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	//����λ��
	CSize CardSize=m_HandCardControl[MYSELF_VIEW_ID].GetCardSize();
	INT nYPosButton=nHeight-CardSize.cy-DEF_SHOOT_DISTANCE-63;

	//���ư�ť
	CRect rcStart;
	m_btStart.GetWindowRect(&rcStart);
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcStart.Width())/2,nYPosButton-rcStart.Height(),0,0,uFlags);

	//���ư�ť
	CRect rcControl;
	m_btStustee.GetWindowRect(&rcControl);
	DeferWindowPos(hDwp,m_btStustee,NULL,nWidth-rcControl.Width()-5,nHeight-rcControl.Height()*1-5,0,0,uFlags);
	DeferWindowPos(hDwp,m_btLastTurn,NULL,nWidth-rcControl.Width()-5,nHeight-rcControl.Height()*2-10,0,0,uFlags);
	DeferWindowPos(hDwp,m_btSortCardCount,NULL,nWidth-rcControl.Width()-5,nHeight-rcControl.Height()*3-15,0,0,uFlags);
	DeferWindowPos(hDwp,m_btSortCardColor,NULL,nWidth-rcControl.Width()-5,nHeight-rcControl.Height()*4-20,0,0,uFlags);
	DeferWindowPos(hDwp,m_btSort,		  NULL,nWidth-rcControl.Width()-5,nHeight-rcControl.Height()*5-25,0,0,uFlags);
	DeferWindowPos(hDwp,m_btSortCardOrder,NULL,nWidth-rcControl.Width()-5,nHeight-rcControl.Height()*6-30,0,0,uFlags);

	//��Ϸ��ť
	CRect rcOutCard;
	m_btOutCard.GetWindowRect(&rcOutCard);
	DeferWindowPos(hDwp,m_btOutCard,NULL,nWidth/2-rcOutCard.Width()*3/2-10,nYPosButton-rcOutCard.Height(),0,0,uFlags);
	DeferWindowPos(hDwp,m_btPassCard,NULL,nWidth/2+rcOutCard.Width()/2+10,nYPosButton-rcOutCard.Height(),0,0,uFlags);
	DeferWindowPos(hDwp,m_btOutPrompt,NULL,nWidth/2-rcOutCard.Width()/2,nYPosButton-rcOutCard.Height(),0,0,uFlags);
	//DeferWindowPos(hDwp,m_btOutPrompt,NULL,nWidth,nHeight,0,0,uFlags);

	//��Ƶ����
	CRect rcAVDlg;
	m_DlgVedioService[0].GetWindowRect(&rcAVDlg);
	DeferWindowPos(hDwp,m_DlgVedioService[1],NULL,nWidth-m_nXBorder-5-rcAVDlg.Width(),nHeight/2+30,0,0,uFlags);
	DeferWindowPos(hDwp,m_DlgVedioService[3],NULL,m_nXBorder+5,nHeight/2+30,0,0,uFlags);
	DeferWindowPos(hDwp,m_DlgVedioService[0],NULL,nWidth-m_nXBorder-5-rcAVDlg.Width(),5,0,0,uFlags);
	m_DlgVedioService[2].GetWindowRect(&rcAVDlg);
	DeferWindowPos(hDwp,m_DlgVedioService[2],NULL,m_nXBorder+5,nHeight-m_nYBorder-3-rcAVDlg.Height(),0,0,uFlags);
	//�����ƶ�
	EndDeferWindowPos(hDwp);

	return;
}

//�滭����
VOID CGameClientView::DrawGameView(CDC * pDC, INT nWidth, INT nHeight)
{
	//�滭����
	DrawViewImage(pDC,m_ImageViewFill,enMode_Spread);
	DrawViewImage(pDC,m_ImageViewBack,enMode_Centent);

	//״̬��Ϣ
	CImageHandle HandleStatusInfo(&m_ImageStatusInfo);
	m_ImageStatusInfo.BitBlt(pDC->m_hDC,5,5,SRCCOPY);
			
	pDC->SetTextColor(RGB(240,240,240));

	//�����ַ�
	TCHAR szBuffer[32]=TEXT("");

	_sntprintf(szBuffer,CountArray(szBuffer),TEXT(" %d ��"),m_PlayerScore[0]+m_PlayerScore[2]);
	pDC->TextOut(100,23,szBuffer,lstrlen(szBuffer));
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT(" %d ��"),m_PlayerScore[1]+m_PlayerScore[3]);
	pDC->TextOut(100,46,szBuffer,lstrlen(szBuffer));
	//���ַ�����Ϣ
	if(m_TurnScore>0)
	{
		CImageHandle HandleTurnScore(&m_ImageTurnScore);
		m_ImageTurnScore.AlphaDrawImage(pDC,nWidth/2-m_ImageTurnScore.GetWidth()/2,nHeight/2-m_ImageTurnScore.GetHeight()/2,RGB(255,0,255));
		DrawNumberString(pDC,m_TurnScore,nWidth/2+m_ImageTurnScore.GetWidth()/2+30,nHeight/2);
	}

	m_5RecordCardControl.DrawCardControl(pDC);
	m_10RecordCardControl.DrawCardControl(pDC);
	m_KRecordCardControl.DrawCardControl(pDC);

	//�滭�û�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		const tagUserData * pUserData=GetUserInfo(i);
		//tagUserData* pUserData = new tagUserData;
		WORD wUserTimer=GetUserTimer(i);
		if (pUserData==NULL) continue;

		////test
		/*
		wUserTimer = 30;
		
		//lstrcpy(pUserData->szName,TEXT("�������"));
		sprintf(pUserData->szName,"�������%02d",i);
		pUserData->lScore = 10000L;
		pUserData->cbUserStatus = US_PLAY;
		m_bAutoPlayer[i]=true;
		m_wPersistInfo[i][0]=1;
		*/
		//�滭����
		DrawUserFace(pDC,pUserData,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);

		if (wUserTimer!=0&&wUserTimer<=30) 
			DrawUserTimer(pDC,m_ptTimer[i].x,m_ptTimer[i].y,wUserTimer);

		//�滭����
		pDC->SetTextAlign(TA_LEFT|TA_TOP);
		pDC->SetTextColor(RGB(254,247,137));

		//�滭�û�
		//pDC->SetTextAlign((i==3)?TA_RIGHT|TA_TOP:TA_LEFT|TA_TOP);
		pDC->TextOut(m_ptName[i].x,m_ptName[i].y,pUserData->szName,lstrlen(pUserData->szName));
		/*	���˵÷�û��Ҫ��ʾ
		//�÷���Ϣ
		_sntprintf(szBuffer,CountArray(szBuffer),TEXT("�÷֣� %d ��"),m_PlayerScore[i]);
		if(i!=2)
		{
			if(i==3){ pDC->SetTextAlign(TA_LEFT|TA_TOP);pDC->TextOut(m_ptHeap[i].x,m_ptHeap[i].y+18,szBuffer,lstrlen(szBuffer));}
			else pDC->TextOut(m_ptHeap[i].x-5,m_ptHeap[i].y+18,szBuffer,lstrlen(szBuffer));
		}
		if(i==2) pDC->TextOut(5,nHeight-36,szBuffer,lstrlen(szBuffer));
		*/
		//�йܱ�־
		if(m_bAutoPlayer[i]==true)
		{
			CPngImage ImageScore;
			ImageScore.LoadImage(AfxGetInstanceHandle(),TEXT("TRUSTEE"));
			ImageScore.DrawImage(pDC,m_ptAuto[i].x,m_ptAuto[i].y);
		}
	}


	//�û��˿�
	bool bHaveCard=false,bHaveNoCard=false;
	for (BYTE i=0;i<GAME_PLAYER;i++)
	{
		if(m_HandCardControl[i].GetCardCount()==0) bHaveNoCard=true;
		if(m_HandCardControl[i].GetCardCount()>0)  bHaveCard=true;
	}
	for (BYTE i=0;i<GAME_PLAYER;i++)
	{
		//�û��˿�
		m_UserCardControl[i].DrawCardControl(pDC);

		//����˿�
		if (i==MYSELF_VIEW_ID)
		{
			//�滭�˿�
			m_HandCardControl[i].DrawCardControl(pDC);

			//�滭����
			if (m_HandCardControl[i].GetCardCount()>0)
			{
				//�����ַ�
				TCHAR szCardCount[32]=TEXT("");
				_sntprintf(szCardCount,CountArray(szCardCount),TEXT("������ʣ�� %d ��"),m_HandCardControl[i].GetCardCount());

				//����ַ�
				pDC->SetTextAlign(TA_LEFT|TA_TOP);
				pDC->TextOut(5,nHeight-18,szCardCount,lstrlen(szCardCount));
			}
		}

		//�û��˿�
		if ((i!=MYSELF_VIEW_ID)&&(m_HandCardControl[i].GetCardCount()>0))
		{
			if (m_HandCardControl[i].GetDisplayItem()==false)
			{
				//�滭����
				WORD wCardCount=m_HandCardControl[i].GetCardCount();
						
				//�����ַ�
				TCHAR szCardCount[32]=TEXT("");
				_sntprintf(szCardCount,CountArray(szCardCount),TEXT("ʣ��        ��"));

				//����ַ�
				pDC->SetTextAlign(TA_LEFT|TA_TOP);
				if(bHaveCard&&bHaveNoCard) pDC->TextOut(m_ptHeap[i].x-5,m_ptHeap[i].y,szCardCount,lstrlen(szCardCount));
				
				if(bHaveCard&&bHaveNoCard) DrawNumberString(pDC,wCardCount,m_ptHeap[i].x+42,m_ptHeap[i].y+5);
			}
			else
			{
				//�滭�˿�
				m_HandCardControl[i].DrawCardControl(pDC);
			}
		}
	}

	//�û��˿�
	for (BYTE i=0;i<GAME_PLAYER;i++)
	{
		m_UserCardControl[i].DrawCardControl(pDC);
	}

	//������Դ
	CPngImage ImageWinOrder;
	ImageWinOrder.LoadImage(AfxGetInstanceHandle(),TEXT("WIN_ORDER"));
	CSize SizeWinOrder(ImageWinOrder.GetWidth()/4,ImageWinOrder.GetHeight());

	//�滭����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (m_wWinOrder[i]!=0)
		{
			//��ȡλ��
			CPoint CenterPoint;
			m_HandCardControl[i].GetCenterPoint(CenterPoint);

			//�滭��Ϣ
			ImageWinOrder.DrawImage(pDC,CenterPoint.x-SizeWinOrder.cx/2,CenterPoint.y-SizeWinOrder.cy/2,SizeWinOrder.cx,SizeWinOrder.cy,
				(m_wWinOrder[i]-1)*SizeWinOrder.cx,0,SizeWinOrder.cx,SizeWinOrder.cy);
		}
	}

	//�û���־
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		const tagUserData * pUserData=GetUserInfo(i);
		if (pUserData==NULL) continue;

		//ͬ���־
		BYTE cbUserStatus=pUserData->cbUserStatus;
		if ((cbUserStatus==US_READY)||(m_bUserContinue[i]==true)) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
	}

	//������Դ
	CImageHandle HandleUserPass(&m_ImageUserPass);
	CSize SizeUserPass(m_ImageUserPass.GetWidth(),m_ImageUserPass.GetHeight());

	//�滭״̬
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (m_bUserPass[i]==true)
		{
			INT nXDrawPos=m_ptPass[i].x-SizeUserPass.cx/2;
			INT nYDrawPos=m_ptPass[i].y-SizeUserPass.cy/2;
			m_ImageUserPass.AlphaDrawImage(pDC,nXDrawPos,nYDrawPos,RGB(255,0,255));
		}
	}

	//���ֱ�־
	if (m_bLastTurnCard==true)
	{
		//������Դ
		CImageHandle HandleLastTurnTip(&m_ImageLastTurnTip);

		//�滭��־
		INT nImageWidth=m_ImageLastTurnTip.GetWidth();
		INT nImageHegith=m_ImageLastTurnTip.GetHeight();
		m_ImageLastTurnTip.AlphaDrawImage(pDC,(nWidth-nImageWidth)/2,(nHeight-nImageHegith)/2-50,RGB(255,0,255));
	}

	//�滭��ը
	if (m_bBombEffect==true)
	{
		//������Դ
		CImageHandle HandleBombEffect(&m_ImageBombEffect);

		//�滭Ч��
		INT nImageHeight=m_ImageBombEffect.GetHeight();
		INT nImageWidth=m_ImageBombEffect.GetWidth()/BOMB_EFFECT_COUNT;
		m_ImageBombEffect.AlphaDrawImage(pDC,(nWidth-nImageWidth)/2,0,nImageWidth,nImageHeight,
			nImageWidth*(m_cbBombFrameIndex%BOMB_EFFECT_COUNT),0,RGB(255,0,255));
	}

	//������ͼ
	//tagScoreInfoC fdde;
	//ZeroMemory(&fdde,sizeof(fdde));
	//m_ScoreControl.SetScoreInfo(fdde);
	m_ScoreControl.DrawScoreView(pDC,nWidth/2,nHeight*2/5);

	return;
}

//������ֵ
bool CGameClientView::SetMainValue(BYTE cbMainValue)
{
	//���ñ���
	m_cbMainValue=cbMainValue;

	//���½���
	UpdateGameView(NULL);

	return true;
}

//��Ԫ����
bool CGameClientView::SetCellScore(LONG lCellScore)
{
	//���ñ���
	m_lCellScore=lCellScore;

	//���½���
	UpdateGameView(NULL);

	return true;
}

//���ñ�ը
bool CGameClientView::SetBombEffect(bool bBombEffect)
{
	if (bBombEffect==true)
	{
		//���ñ���
		m_bBombEffect=true;
		m_cbBombFrameIndex=0;

		//����ʱ��
		SetTimer(IDI_BOMB_EFFECT,100,NULL);
	}
	else
	{
		//ֹͣ����
		if (m_bBombEffect==true)
		{
			//ɾ��ʱ��
			KillTimer(IDI_BOMB_EFFECT);

			//���ñ���
			m_bBombEffect=false;
			m_cbBombFrameIndex=0;

			//���½���
			UpdateGameView(NULL);
		}
	}

	return true;
}

//��������
VOID CGameClientView::SetLastTurnCard(bool bLastTurnCard)
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

//���÷���
bool CGameClientView::SetUserPass(WORD wChairID, bool bUserPass)
{
	//���ñ���
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<GAME_PLAYER;i++) m_bUserPass[i]=bUserPass;
	}
	else
	{
		ASSERT(wChairID<GAME_PLAYER);
		m_bUserPass[wChairID]=bUserPass;
	}

	//���½���
	UpdateGameView(NULL);

	return true;
}

//���ü���
bool CGameClientView::SetUserContinue(WORD wChairID, bool bContinue)
{
	//���ñ���
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<GAME_PLAYER;i++) m_bUserContinue[i]=bContinue;
	}
	else
	{
		ASSERT(wChairID<GAME_PLAYER);
		m_bUserContinue[wChairID]=bContinue;
	}

	//���½���
	UpdateGameView(NULL);

	return true;
}

//��������
bool CGameClientView::SetUserWinOrder(WORD wChairID, WORD wWinOrder)
{
	//���ñ���
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<GAME_PLAYER;i++) m_wWinOrder[i]=wWinOrder;
	}
	else
	{
		ASSERT(wChairID<GAME_PLAYER);
		m_wWinOrder[wChairID]=wWinOrder;
	}

	//���½���
	UpdateGameView(NULL);

	return true;
}

//������Ϣ
bool CGameClientView::SetUserPersist(WORD wChairID, WORD wPersistInfo[2])
{
	//���ñ���
	if (wChairID<GAME_PLAYER)
	{
		m_wPersistInfo[wChairID][0]=wPersistInfo[0];
		m_wPersistInfo[wChairID][1]=wPersistInfo[1];
	}
	else
	{
		ZeroMemory(m_wPersistInfo,sizeof(m_wPersistInfo));
	}

	//���½���
	UpdateGameView(NULL);

	return true;
}

//���õȼ�
bool CGameClientView::SetUserValueOrder(BYTE cbMySelfOrder, BYTE cbEnemyOrder)
{
	//���ñ���
	m_cbValueOrder[0]=cbMySelfOrder;
	m_cbValueOrder[1]=cbEnemyOrder;

	//���½���
	UpdateGameView(NULL);

	return true;
}

//������Ϸ����
bool CGameClientView::SetScoreInfo(int TurnScore,int PlayerScore[4])
{
	m_TurnScore=TurnScore;
	CopyMemory(m_PlayerScore,PlayerScore,sizeof(m_PlayerScore));
	UpdateGameView(NULL);
	return true;
}

//�滭����
VOID CGameClientView::DrawNumberString(CDC * pDC, LONG lNumber, INT nXPos, INT nYPos)
{
	//������Դ
	CImageHandle HandleNumber(&m_ImageNumber);
	INT nNumberHeight=m_ImageNumber.GetHeight();
	INT nNumberWidth=m_ImageNumber.GetWidth()/10;

	//������Ŀ
	LONG lNumberCount=0;
	LONG lNumberTemp=lNumber;
	do
	{
		lNumberCount++;
		lNumberTemp/=10;
	} while (lNumberTemp>0);

	//λ�ö���
	INT nYDrawPos=nYPos-nNumberHeight/2;
	INT nXDrawPos=nXPos+lNumberCount*nNumberWidth/2-nNumberWidth;

	//�滭����
	for (LONG i=0;i<lNumberCount;i++)
	{
		//�滭����
		LONG lCellNumber=lNumber%10;
		m_ImageNumber.AlphaDrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,lCellNumber*nNumberWidth,0,RGB(255,0,255));

		//���ñ���
		lNumber/=10;
		nXDrawPos-=nNumberWidth;
	};

	return;
}

//��ʼ��ť
VOID CGameClientView::OnBnClickedStart()
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_START,0,0);

	return;
}

//���ư�ť
VOID CGameClientView::OnBnClickedOutCard()
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_OUT_CARD,0,0);

	return;
}

//PASS��ť
VOID CGameClientView::OnBnClickedPassCard()
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_PASS_CARD,0,0);

	return;
}

//��ʾ��ť
VOID CGameClientView::OnBnClickedOutPrompt()
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_OUT_PROMPT,0,0);

	return;
}

//�Ϲܰ�ť
VOID CGameClientView::OnBnClickedStustee()
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_TRUSTEE_CONTROL,0,0);

	return;
}
//���ư�ť
VOID CGameClientView::OnBnClickedSort()
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_SORT_HAND_CARD,255,255);

	return;
}
//����ť
VOID CGameClientView::OnBnClickedSortCardColor()
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_SORT_HAND_CARD,ST_TONGHUA,ST_TONGHUA);

	return;
}

//����ť
VOID CGameClientView::OnBnClickedSortCardCount()
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_SORT_HAND_CARD,ST_510K,ST_510K);

	return;
}

//����ť
VOID CGameClientView::OnBnClickedSortCardOrder()
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_SORT_HAND_CARD,ST_ORDER,ST_ORDER);

	return;
}

//���ְ�ť
VOID CGameClientView::OnBnClickedLastTurn()
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_LAST_TURN_CARD,0,0);

	return;
}

//ʱ����Ϣ
VOID CGameClientView::OnTimer(UINT nIDEvent)
{
	//��ը����
	if (nIDEvent==IDI_BOMB_EFFECT)
	{
		//ֹͣ�ж�
		if (m_bBombEffect==false)
		{
			KillTimer(IDI_BOMB_EFFECT);
			return;
		}

		//���ñ���
		if ((m_cbBombFrameIndex+1)>=BOMB_EFFECT_COUNT)
		{
			//ɾ��ʱ��
			KillTimer(IDI_BOMB_EFFECT);

			//���ñ���
			m_bBombEffect=false;
			m_cbBombFrameIndex=0;
		}
		else m_cbBombFrameIndex++;

		//���½���
		UpdateGameView(NULL);

		return;
	}

	__super::OnTimer(nIDEvent);
}

//��������
INT CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//��������
	CRect rcCreate(0,0,0,0);
	HINSTANCE hResInstance=AfxGetInstanceHandle();

	//���ư�ť
	m_btStart.Create(NULL,WS_CHILD,rcCreate,this,IDC_START);
	m_btStustee.Create(NULL,WS_CHILD|WS_DISABLED|WS_VISIBLE,rcCreate,this,IDC_TRUSTEE);
	m_btLastTurn.Create(NULL,WS_CHILD|WS_DISABLED|WS_VISIBLE,rcCreate,this,IDC_LAST_TURN);
	m_btSort.Create(NULL,WS_CHILD|WS_DISABLED|WS_VISIBLE,rcCreate,this,IDC_SORT_CARD);
	m_btSortCardColor.Create(NULL,WS_CHILD|WS_DISABLED|WS_VISIBLE,rcCreate,this,IDC_SORT_CARD_COLOR);
	m_btSortCardCount.Create(NULL,WS_CHILD|WS_DISABLED|WS_VISIBLE,rcCreate,this,IDC_SORT_CARD_COUNT);
	m_btSortCardOrder.Create(NULL,WS_CHILD|WS_DISABLED|WS_VISIBLE,rcCreate,this,IDC_SORT_CARD_ORDER);
	//m_btStusteeControl.Create(TEXT(""),WS_CHILD|WS_DISABLED|WS_VISIBLE,rcCreate,this,IDC_TRUSTEE_CONTROL);
	//m_btStusteeControl.EnableWindow(FALSE);

	//��Ϸ��ť
	m_btOutCard.Create(NULL,WS_CHILD,rcCreate,this,IDC_OUT_CARD);
	m_btPassCard.Create(NULL,WS_CHILD,rcCreate,this,IDC_PASS_CARD);
	m_btOutPrompt.Create(NULL,WS_CHILD,rcCreate,this,IDC_OUT_PROMPT);

	//���ư�ť
	m_btStart.SetButtonImage(IDB_BT_START,hResInstance,false);
	m_btLastTurn.SetButtonImage(IDB_BT_LAST_TURN,hResInstance,false);
	m_btStustee.SetButtonImage(IDB_BT_START_TRUSTEE,hResInstance,false);
	m_btSort.SetButtonImage(IDB_BT_SORT,hResInstance,false);
	m_btSortCardColor.SetButtonImage(IDB_BT_SORT_CARD_COLOR,hResInstance,false);
	m_btSortCardCount.SetButtonImage(IDB_BT_SORT_CARD_COUNT,hResInstance,false);
	m_btSortCardOrder.SetButtonImage(IDB_BT_SORT_CARD_ORDER,hResInstance,false);

	//m_btStusteeControl.SetButtonImage(IDB_START_TRUSTEE,hInstance,false);

	//��Ϸ��ť
	m_btOutCard.SetButtonImage(IDB_BT_OUT_CARD,hResInstance,false);
	m_btPassCard.SetButtonImage(IDB_BT_PASS_CARD,hResInstance,false);
	m_btOutPrompt.SetButtonImage(IDB_BT_OUT_PROMPT,hResInstance,false);

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//�û��˿�
		if (i==MYSELF_VIEW_ID)
		{
			m_HandCardControl[i].SetDirection(true);
			m_HandCardControl[i].SetCardDistance(DEF_X_DISTANCE,DEF_Y_DISTANCE,DEF_SHOOT_DISTANCE);
		}

		//����˿�
		if (i==0)
		{
			m_HandCardControl[i].SetCardMode(true);
			m_HandCardControl[i].SetDirection(true);
			m_HandCardControl[i].SetCardDistance(DEF_X_DISTANCE_SMALL,DEF_Y_DISTANCE_SMALL,0);
		}

		//����˿�
		if ((i==1)||(i==3))
		{
			m_HandCardControl[i].SetCardMode(true);
			m_HandCardControl[i].SetDirection(false);
			m_HandCardControl[i].SetCardDistance(DEF_X_DISTANCE_SMALL,DEF_Y_DISTANCE_SMALL,0);
		}

		//�����˿�
		m_UserCardControl[i].SetDisplayItem(true);
		m_UserCardControl[i].SetCardDistance(DEF_X_DISTANCE,0,0);
	}

	//������ʾ
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(&m_btStart,TEXT("��ʼ��Ϸ"));

	//������Ƶ
	for (WORD i=0;i<4;i++)
	{
		//������Ƶ
		m_DlgVedioService[i].Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,200+i);
		m_DlgVedioService[i].InitVideoService(i==2,true);

		//������Ƶ
		g_VedioServiceManager.SetVideoServiceControl(i,&m_DlgVedioService[i]);
	}

	return 0;
}

//�����Ϣ
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//�˿˲���
	if (m_HandCardControl[MYSELF_VIEW_ID].OnEventSetCursor(MousePoint)==true) return TRUE;

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}
//�����Ϣ
VOID CGameClientView::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags, Point);

	//��������
	WORD wHoverCardItem=INVALID_ITEM;

	//�ƶ��ж�
	if (m_wHoverCardItem!=INVALID_ITEM)
	{
		//��ȡ���
		CPoint MousePoint;
		GetCursorPos(&MousePoint);
		ScreenToClient(&MousePoint);

		//�˿˴�С
		CSize ControlSize;
		m_HandCardControl[MYSELF_VIEW_ID].GetControlSize(ControlSize);

		//�˿�λ��
		CPoint OriginPoint;
		m_HandCardControl[MYSELF_VIEW_ID].GetOriginPoint(OriginPoint);

		//���е���
		if (MousePoint.x<OriginPoint.x) MousePoint.x=OriginPoint.x;
		if (MousePoint.x>(OriginPoint.x+ControlSize.cx)) MousePoint.x=(OriginPoint.x+ControlSize.cx);

		//��ȡ����
		MousePoint.y=OriginPoint.y+DEF_SHOOT_DISTANCE;
		wHoverCardItem=m_HandCardControl[MYSELF_VIEW_ID].SwitchCardPoint(MousePoint);

		//�ƶ�����
		if (wHoverCardItem!=m_wHoverCardItem) m_bMoveMouse=true;
	}

	//�����˿�
	if ((m_bSwitchCard==true)&&(wHoverCardItem!=m_wHoverCardItem))
	{
		//�����˿�
		m_HandCardControl[MYSELF_VIEW_ID].MoveCardItem(wHoverCardItem);

		//���½���
		UpdateGameView(NULL);
	}

	//ѡ���˿�
	if ((m_bSelectCard==true)&&(wHoverCardItem!=m_wHoverCardItem))
	{
		//�����˿�
		if (wHoverCardItem>m_wMouseDownItem)
		{
			m_HandCardControl[MYSELF_VIEW_ID].SetSelectIndex(m_wMouseDownItem,wHoverCardItem);
		}
		else
		{
			m_HandCardControl[MYSELF_VIEW_ID].SetSelectIndex(wHoverCardItem,m_wMouseDownItem);
		}

		//���½���
		UpdateGameView(NULL);
	}

	//���ñ���
	m_wHoverCardItem=wHoverCardItem;

	return;
}

//�����Ϣ
VOID CGameClientView::OnRButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnRButtonUp(nFlags, Point);

	//״̬�ж�
	if (m_HandCardControl[MYSELF_VIEW_ID].GetPositively()==true)
	{
		//�����ж�
		if (m_HandCardControl[MYSELF_VIEW_ID].GetCardFromPoint(Point)==NULL)
		{
			//��ȡ�˿�
			BYTE cbCardData[MAX_COUNT];
			WORD wCardCount=m_HandCardControl[MYSELF_VIEW_ID].GetShootCard(cbCardData,CountArray(cbCardData));

			//���½���
			if (wCardCount>=0)
			{
				//�����˿�
				m_HandCardControl[MYSELF_VIEW_ID].SetShootCard(NULL,0);

				//������Ϣ
				AfxGetMainWnd()->SendMessage(IDM_LEFT_HIT_CARD,0,0);

				//���½���
				UpdateGameView(NULL);
			}
		}
		else
		{
			//������Ϣ
			if (m_btOutCard.IsWindowVisible()&&m_btOutCard.IsWindowEnabled())
			{
				AfxGetMainWnd()->SendMessage(IDM_OUT_CARD,0,0);
			}

		}
	}

	return;
}

//�����Ϣ
VOID CGameClientView::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags, Point);

	m_nLButtonUpCount++;
	if ( m_nLButtonUpCount == 2 )
	{
		return __super::OnLButtonUp(nFlags, Point);
	}

	//Ĭ�ϴ���
	if (m_bMoveMouse==false)
	{
		//��ȡ�˿�
		tagCardItem * pCardItem=NULL;
		WORD wMouseDownItem=m_HandCardControl[MYSELF_VIEW_ID].SwitchCardPoint(Point);
		if (wMouseDownItem==m_wMouseDownItem) pCardItem=m_HandCardControl[MYSELF_VIEW_ID].GetCardFromPoint(Point);

		//�����˿�
		if (pCardItem!=NULL)
		{
			//�����˿�
			pCardItem->bShoot=!pCardItem->bShoot;

			//������Ϣ
			AfxGetMainWnd()->SendMessage(IDM_LEFT_HIT_CARD,0,0);

			//���½���
			UpdateGameView(NULL);
		}
	}
		//ѡ����
	if ((m_bSelectCard==true)&&(m_bMoveMouse==true))
	{
		//�����˿�
		if (m_wHoverCardItem>m_wMouseDownItem)
		{
			m_HandCardControl[MYSELF_VIEW_ID].SetShootIndex(m_wMouseDownItem,m_wHoverCardItem);
		}
		else
		{
			m_HandCardControl[MYSELF_VIEW_ID].SetShootIndex(m_wHoverCardItem,m_wMouseDownItem);
		}

		//������Ϣ
		AfxGetMainWnd()->SendMessage(IDM_LEFT_HIT_CARD,0,0);

		//���½���
		UpdateGameView(NULL);
	}

	//��������
	if ((m_bSwitchCard==true)&&(m_bMoveMouse==true))
	{
		//�����˿�
		m_HandCardControl[MYSELF_VIEW_ID].SetShootCard(NULL,0);

		//������Ϣ
		AfxGetMainWnd()->SendMessage(IDM_LEFT_HIT_CARD,0,0);
		AfxGetMainWnd()->SendMessage(IDM_SORT_HAND_CARD,ST_CUSTOM,ST_CUSTOM);

		//���½���
		UpdateGameView(NULL);
	}

	//�ͷ����
	ReleaseCapture();

	//���ñ���
	m_bMoveMouse=false;
	m_bSwitchCard=false;
	m_bSelectCard=false;

	//��������
	m_wMouseDownItem=INVALID_ITEM;
	m_wHoverCardItem=INVALID_ITEM;
	
	return;
}

//�����Ϣ
VOID CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);
	m_nLButtonUpCount=0;

	//״̬�ж�
	if (m_HandCardControl[MYSELF_VIEW_ID].GetPositively()==true)
	{
		//��ȡ�˿�
		m_wMouseDownItem=m_HandCardControl[MYSELF_VIEW_ID].SwitchCardPoint(Point);

		//�����˿�
		if (m_wMouseDownItem!=INVALID_ITEM)
		{
			//��ȡ�˿�
			tagCardItem * pCardItem=m_HandCardControl[MYSELF_VIEW_ID].GetCardFromIndex(m_wMouseDownItem);

			//���ñ���
			m_bMoveMouse=false;
			m_wHoverCardItem=m_wMouseDownItem;

			//��������
			if (pCardItem->bShoot==false) 
			{
				m_bSelectCard=true;
			}
			else
			{
				m_bSwitchCard=true;
			}

			//�������
			SetCapture();
		}
	}

	return;
}

//�����Ϣ
VOID CGameClientView::OnLButtonDblClk(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDblClk(nFlags, Point);

	//�����˿�
	if (m_HandCardControl[MYSELF_VIEW_ID].GetCardFromPoint(Point)==NULL)
	{
		//��ȡ�˿�
		BYTE cbCardData[MAX_COUNT];
		WORD wCardCount=m_HandCardControl[MYSELF_VIEW_ID].GetShootCard(cbCardData,CountArray(cbCardData));

		//���½���
		if (wCardCount>=0)
		{
			//��ֹ��ť
			m_btOutCard.EnableWindow(FALSE);

			//�����˿�
			m_HandCardControl[MYSELF_VIEW_ID].SetShootCard(NULL,0);

			//���½���
			UpdateGameView(NULL);
		}
	}
	else
	{
		//״̬�ж�
		if (m_HandCardControl[MYSELF_VIEW_ID].GetPositively()==true) m_wMouseDownItem=m_HandCardControl[MYSELF_VIEW_ID].SwitchCardPoint(Point);
		
		//��ȡ�˿�
		tagCardItem * pCardItem=m_HandCardControl[MYSELF_VIEW_ID].GetCardFromIndex(m_wMouseDownItem);
		BYTE bSelectCard=pCardItem->cbCardData;
		pCardItem->bShoot=false;
		
		//���Ҹ�����
		WORD wStartIndex=0;
		WORD wEndIndex=0;
		for(int i=m_wMouseDownItem;i>=0;i--)
		{
			if((m_HandCardControl[MYSELF_VIEW_ID].GetCardFromIndex(i)->cbCardData&0x0F)==(bSelectCard&0x0F)) wStartIndex=i;
			else	break;
		}
		for(int i=m_wMouseDownItem;i<m_HandCardControl[MYSELF_VIEW_ID].GetCardCount();i++)
		{
			if((m_HandCardControl[MYSELF_VIEW_ID].GetCardFromIndex(i)->cbCardData&0x0F)==(bSelectCard&0x0F))		wEndIndex=i;
			else	break;
		}

		//����������
		m_HandCardControl[MYSELF_VIEW_ID].SetShootIndex(wStartIndex,wEndIndex);

		AfxGetMainWnd()->SendMessage(IDM_LEFT_HIT_CARD,0,0);

		UpdateGameView(NULL);
	}
	return;
}
void CGameClientView::SetAutoUser(bool bAutoUser[4])
{
	CopyMemory(m_bAutoPlayer,bAutoUser,sizeof(m_bAutoPlayer));
		
	//���½���
	UpdateGameView(NULL);
	return;
}
//////////////////////////////////////////////////////////////////////////
//�Ƽ�¼
void CGameClientView::Set510KRecord(BYTE bRecord[3][8])
{
	for(BYTE i=0;i<8;i++) if(bRecord[0][i]==0) break;
	m_5RecordCardControl.SetCardData(bRecord[0],i);
	for(BYTE i=0;i<8;i++) if(bRecord[1][i]==0) break;
	m_10RecordCardControl.SetCardData(bRecord[1],i);
	for(BYTE i=0;i<8;i++) if(bRecord[2][i]==0) break;
	m_KRecordCardControl.SetCardData(bRecord[2],i);
		
	//���½���
	UpdateGameView(NULL);
	return;
}