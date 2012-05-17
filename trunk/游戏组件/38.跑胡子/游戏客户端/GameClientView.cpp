#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//��ť��ʶ 
#define IDC_START						100								//��ʼ��ť

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_START, OnStart)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView(void) : CGameFrameView(true,24)
{
	//��Ϸ����
	m_lCellScore=0L;
	m_bLeftCardCount=0xFF;
	m_wBankerUser=INVALID_CHAIR;

	//���״̬
	m_bOutCard=false;
	m_bWaitOther=false;
	m_bHuangZhuang=false;

	//��������
	m_bMoveCard=false;
	m_bSwitchCard=false;
	m_wMouseDownItem=INVALID_ITEM;

	//�û�״̬
	m_cbCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_bUserAction,sizeof(m_bUserAction));
	for (BYTE i=0;i<CountArray(m_bUserHuXiCount);i++) m_bUserHuXiCount[i]=0xFF;

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageTitle.SetLoadInfo(IDB_TITLE,hInstance);
	m_ImageWait.SetLoadInfo(IDB_WAIT_TIP,hInstance);
	m_ImageBanker.SetLoadInfo(IDB_BANKER,hInstance);
	m_ImageViewBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_ImageOutCard.SetLoadInfo(IDB_OUT_CARD_TIP,hInstance);
	m_ImageCardFrame.SetLoadInfo(IDB_CARD_FRAME,hInstance);
	m_ImageUserAction.SetLoadInfo(IDB_USER_ACTION,hInstance);
	m_ImageActionBack.SetLoadInfo(IDB_ACTION_BACK,hInstance);
	m_ImageHuangZhuang.SetLoadInfo(IDB_HUANG_ZHUANG,hInstance);

	//��ȡ��С
	CImageHandle ImageHandleBanker(&m_ImageBanker);
	m_BankerSize.cx=m_ImageBanker.GetWidth();
	m_BankerSize.cy=m_ImageBanker.GetHeight();

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

	//��������
	enDirection Direction[]={Direction_East,Direction_South,Direction_West};

	//���ÿؼ�
	m_UserCard[0].SetDirection(Direction_East);
	m_UserCard[1].SetDirection(Direction_West);
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		//�����˿�
		m_DiscardCard[i].SetDirection(Direction[i]);

		//����˿�
		for (WORD j=0;j<CountArray(m_WeaveCard[i]);j++) m_WeaveCard[i][j].SetDirection(Direction[i]);
	}

	//������ť
	CRect rcCreate(0,0,0,0);
	m_btStart.Create(NULL,WS_CHILD,rcCreate,this,IDC_START);
	m_ChooseWnd.Create(NULL,NULL,WS_CHILD,rcCreate,this,100);
	m_ControlWnd.Create(NULL,NULL,WS_CHILD,rcCreate,this,101);
	m_ScoreView.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,this,102);

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btStart.SetButtonImage(IDB_BT_START,hInstance,false);

	return 0;
}

//���ý���
void CGameClientView::ResetGameView()
{
	//��Ϸ����
	m_lCellScore=0L;
	m_bLeftCardCount=0xFF;
	m_wBankerUser=INVALID_CHAIR;

	//���״̬
	m_bOutCard=false;
	m_bWaitOther=false;
	m_bHuangZhuang=false;

	//��������
	m_bMoveCard=false;
	m_bSwitchCard=false;
	m_wMouseDownItem=INVALID_ITEM;

	//�û�״̬
	m_cbCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_bUserAction,sizeof(m_bUserAction));
	for (BYTE i=0;i<CountArray(m_bUserHuXiCount);i++) m_bUserHuXiCount[i]=0xFF;

	//���ؿؼ�
	m_btStart.ShowWindow(SW_HIDE);
	m_ScoreView.ShowWindow(SW_HIDE);
	m_ChooseWnd.ShowWindow(SW_HIDE);
	m_ControlWnd.ShowWindow(SW_HIDE);

	//�����˿�
	m_UserCard[0].SetCardData(0);
	m_UserCard[1].SetCardData(0);
	m_HandCardControl.SetCardData(NULL,0);
	m_HandCardControl.SetPositively(false);
	m_HandCardControl.SetDisplayItem(false);

	//��������
	tagWeaveItem WeaveItem;
	WeaveItem.cbWeaveKind=ACK_NULL;

	//�����˿�
	for (int i=0;i<GAME_PLAYER;i++)
	{
		m_DiscardCard[i].SetCardData(NULL,0);
		for (int j=0;j<CountArray(m_WeaveCard[0]);j++) m_WeaveCard[i][j].SetCardData(WeaveItem);
	}

	return;
}

//�����ؼ�
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	//��������
	m_ptFace[0].x=nWidth-m_nXBorder-m_nXFace-5;
	m_ptFace[0].y=nHeight/2-m_nYFace-50;
	m_ptName[0].x=nWidth-m_nXBorder-5;
	m_ptName[0].y=nHeight/2-45;
	m_ptTimer[0].x=nWidth-m_nXBorder-m_nXFace-5;
	m_ptTimer[0].y=nHeight/2-m_nYFace-55-m_nYTimer;
	m_ptReady[0].x=nWidth-m_nXBorder-m_nXTimer-5;
	m_ptReady[0].y=nHeight/2-m_nYFace-55-m_nYTimer/2;
	m_BankerPoint[0].x=nWidth-m_nXBorder-m_BankerSize.cx-3;
	m_BankerPoint[0].y=nHeight/2-10;

	m_ptFace[1].x=nWidth/2-m_nXFace/2;
	m_ptFace[1].y=nHeight-m_nYBorder-m_nYFace-5;
	m_ptName[1].x=nWidth/2+5+m_nXFace/2;
	m_ptName[1].y=nHeight-m_nYBorder-m_nYFace-2;
	m_ptTimer[1].x=nWidth/2-m_nXFace/2-m_nXTimer*2-5;
	m_ptTimer[1].y=nHeight-m_nYBorder-m_nYTimer-10;
	m_ptReady[1].x=nWidth/2-m_nXTimer-m_nXFace/2-5;
	m_ptReady[1].y=nHeight-m_nYBorder-m_nYTimer/2-10;
	m_BankerPoint[1].x=nWidth/2-m_nXFace/2-m_BankerSize.cx-m_nXTimer*2-10;
	m_BankerPoint[1].y=nHeight-m_nXBorder-m_BankerSize.cy-5;

	m_ptFace[2].x=m_nXBorder+5;
	m_ptFace[2].y=nHeight/2-m_nYFace-50;
	m_ptName[2].x=m_nXBorder+5;
	m_ptName[2].y=nHeight/2-45;
	m_ptTimer[2].x=m_nXBorder+5;
	m_ptTimer[2].y=nHeight/2-m_nYFace-55-m_nYTimer;
	m_ptReady[2].x=m_nXBorder+5+m_nXTimer;
	m_ptReady[2].y=nHeight/2-m_nYFace-55-m_nYTimer/2;
	m_BankerPoint[2].x=m_nXBorder+3;
	m_BankerPoint[2].y=nHeight/2-10;

	//�����˿�
	m_DiscardCard[0].SetControlPoint(nWidth-m_nXBorder-130,nHeight/2-290);
	m_DiscardCard[1].SetControlPoint(nWidth/2+295,nHeight-m_nYFace-m_nYBorder-105);
	m_DiscardCard[2].SetControlPoint(m_nXBorder+130,nHeight/2+170);

	//����˿�
	for (WORD i=0;i<CountArray(m_WeaveCard[0]);i++)
	{
		m_WeaveCard[0][i].SetControlPoint(nWidth-m_nXBorder-112,nHeight/2-i*30+53);
		m_WeaveCard[1][i].SetControlPoint(nWidth/2-110+i*35,nHeight-270);
		m_WeaveCard[2][i].SetControlPoint(m_nXBorder+112,nHeight/2-193+i*30);
	}

	//�û��˿�
	m_UserCard[0].SetControlPoint(nWidth-m_nXBorder-70,nHeight/2+140);
	m_UserCard[1].SetControlPoint(m_nXBorder+70,nHeight/2-260);
	m_HandCardControl.SetBenchmarkPos(nWidth/2,nHeight-m_nYFace-m_nYBorder-15,enXCenter,enYBottom);

	//�ƶ���ť
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(6);
	m_btStart.GetWindowRect(&rcButton);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	DeferWindowPos(hDwp,m_ControlWnd,NULL,nWidth-400-m_nXBorder,nHeight-215-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nHeight-195-m_nYBorder,0,0,uFlags);
	EndDeferWindowPos(hDwp);

	//���ƴ���
	m_ChooseWnd.SetBenchmarkPos(nWidth-200-m_nXBorder,nHeight-225-m_nYBorder);

	//������ͼ
	CRect rcControl;
	m_ScoreView.GetWindowRect(&rcControl);
	m_ScoreView.SetWindowPos(NULL,(nWidth-rcControl.Width())/2,130,0,0,SWP_NOZORDER|SWP_NOSIZE);

	return;
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//�滭����
	CImageHandle HandleTitle(&m_ImageTitle);
	DrawViewImage(pDC,m_ImageViewBack,enMode_Centent);
	m_ImageTitle.BitBlt(pDC->m_hDC,m_nXBorder+10,m_nYBorder+10);

	//�˿˱���
	CImageHandle HandleCardFrame(&m_ImageCardFrame);
	m_ImageCardFrame.BitBlt(pDC->m_hDC,(nWidth-m_ImageCardFrame.GetWidth())/2,m_nYBorder+10);

	//�滭�û�
	TCHAR szBuffer[32];
	pDC->SetTextColor(RGB(255,255,0));
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		const tagUserData * pUserData=GetUserInfo(i);
		if (pUserData!=NULL)
		{
			//�û�����
			pDC->SetTextAlign((i==0)?(TA_RIGHT|TA_TOP):(TA_LEFT|TA_TOP));
			pDC->TextOut(m_ptName[i].x,m_ptName[i].y,pUserData->szName,lstrlen(pUserData->szName));

			//ʣ���˿�
			if (m_bUserHuXiCount[i]!=0xFF)
			{
				_snprintf(szBuffer,sizeof(szBuffer),TEXT("��%ld ��Ϣ��"),m_bUserHuXiCount[i]);
				pDC->TextOut(m_ptName[i].x,m_ptName[i].y+18,szBuffer,lstrlen(szBuffer));
			}

			//������Ϣ
			WORD wUserTimer=GetUserTimer(i);
			if (wUserTimer!=0) DrawUserTimer(pDC,m_ptTimer[i].x,m_ptTimer[i].y,wUserTimer);
			if (pUserData->cbUserStatus==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			DrawUserFace(pDC,pUserData,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);
		}
	}

	//ׯ����Ϣ
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		//��ȡ�û�
		const tagUserData * pUserData=GetUserInfo(m_wBankerUser);
		if (pUserData!=NULL)
		{
			//��������
			CImageHandle ImageHandleBanker(&m_ImageBanker);

			//���� DC
			pDC->SetTextAlign(TA_LEFT|TA_TOP);
			pDC->SetTextColor(RGB(250,250,250));

			//ׯ������
			UINT uFormat=DT_SINGLELINE|DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS;
			CRect rcBanker(m_nXBorder+86,m_nYBorder+34,m_nXBorder+166,m_nYBorder+50);
			pDC->DrawText(pUserData->szName,lstrlen(pUserData->szName),&rcBanker,uFormat);

			//ׯ�ұ�־
			m_ImageBanker.AlphaDrawImage(pDC,m_BankerPoint[m_wBankerUser].x,m_BankerPoint[m_wBankerUser].y,RGB(255,0,255));
		}
	}

	//��������
	if (m_lCellScore!=0L)
	{
		pDC->SetTextColor(RGB(250,250,250));
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),m_lCellScore);
		CRect rcCellScore(m_nXBorder+86,m_nYBorder+14,m_nXBorder+166,m_nYBorder+29);
		pDC->DrawText(szBuffer,lstrlen(szBuffer),&rcCellScore,DT_SINGLELINE|DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS);
	}

	//ʣ���˿�
	if (m_bLeftCardCount!=0xFF)
	{
		//ʣ����Ϣ
		pDC->SetTextColor(RGB(250,250,250));
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld ��"),m_bLeftCardCount);
		CRect rcCellScore(m_nXBorder+86,m_nYBorder+53,m_nXBorder+166,m_nYBorder+68);
		pDC->DrawText(szBuffer,lstrlen(szBuffer),&rcCellScore,DT_SINGLELINE|DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS);

		//�˿���Ϣ
		if (m_bLeftCardCount>0)
		{
			int nYPos=m_nYBorder+19,nSpace=9;
			int nXPos=nWidth/2-((m_bLeftCardCount-1)*nSpace+g_CardResource.m_ImageTableTop.GetItemWidth())/2;
			for (BYTE i=0;i<m_bLeftCardCount;i++) g_CardResource.m_ImageTableTop.DrawCardItem(pDC,0,nXPos+i*nSpace,nYPos);
		}
	}

	//�û��˿�
	m_UserCard[0].DrawCardControl(pDC);
	m_UserCard[1].DrawCardControl(pDC);
	m_HandCardControl.DrawCardControl(pDC);

	//�滭�˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//�����˿�
		m_DiscardCard[i].DrawCardControl(pDC);

		//����˿�
		for (WORD j=0;j<CountArray(m_WeaveCard[i]);j++) m_WeaveCard[i][j].DrawCardControl(pDC);
	}

	//������ʾ
	if (m_bOutCard==true)
	{
		CImageHandle HandleOutCard(&m_ImageOutCard);
		m_ImageOutCard.AlphaDrawImage(pDC,(nWidth-m_ImageOutCard.GetWidth())/2,nHeight-300,RGB(255,0,255));
	}

	//�ȴ���ʾ
	if (m_bWaitOther==true)
	{
		CImageHandle HandleWait(&m_ImageWait);
		m_ImageWait.AlphaDrawImage(pDC,(nWidth-m_ImageWait.GetWidth())/2,nHeight-300,RGB(255,0,255));
	}

	//��ׯ��־
	if (m_bHuangZhuang==true)
	{
		CImageHandle HandleHuangZhuang(&m_ImageHuangZhuang);
		m_ImageHuangZhuang.AlphaDrawImage(pDC,(nWidth-m_ImageHuangZhuang.GetWidth())/2,nHeight/2-115,RGB(255,0,255));
	}

	//�滭����
	if (m_cbCardData!=0)
	{
		//����λ��
		switch (m_wOutCardUser)
		{
		case 0:	//����
			{
				int nYPos=nHeight/2-60-g_CardResource.m_ImageUserRight.GetItemHeight()/2;
				int nXPos=nWidth-m_nXBorder-290-g_CardResource.m_ImageUserRight.GetItemWidth()/2;
				g_CardResource.m_ImageUserRight.DrawCardItem(pDC,m_cbCardData,nXPos,nYPos);

				break;
			}
		case 1:	//����
			{
				int nXPos=nWidth/2-g_CardResource.m_ImageUserBottom.GetItemWidth()/2;;
				int nYPos=nHeight-m_nYBorder-335-g_CardResource.m_ImageUserBottom.GetItemHeight()/2;
				g_CardResource.m_ImageUserBottom.DrawCardItem(pDC,m_cbCardData,nXPos,nYPos);

				break;
			}
		case 2:	//����
			{
				int nXPos=m_nXBorder+290-g_CardResource.m_ImageUserLeft.GetItemWidth()/2;
				int nYPos=nHeight/2-60-g_CardResource.m_ImageUserLeft.GetItemHeight()/2;
				g_CardResource.m_ImageUserLeft.DrawCardItem(pDC,m_cbCardData,nXPos,nYPos);

				break;
			}
		}
	}

	//�滭����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (m_bUserAction[i]!=0)
		{
			//����λ��
			int nXPos=0,nYPos=0;
			int nActionWidth=129,nActionHeight=109;
			switch (i)
			{
			case 0:	//����
				{
					nXPos=nWidth-m_nXBorder-145-nActionWidth;
					nYPos=nHeight/2-103;
					break;
				}
			case 1:	//����
				{
					nXPos=(nWidth-nActionWidth)/2;
					nYPos=nHeight-m_nYBorder-150-nActionHeight;
					break;
				}
			case 2:	//����
				{
					nXPos=m_nXBorder+145;
					nYPos=nHeight/2-103;
					break;
				}
			}

			//��������
			int nXImagePos=0;
			CImageHandle HandleUserAction(&m_ImageUserAction);
			int nItemWidth=m_ImageUserAction.GetWidth()/6;

			//����λ��
			if (m_bUserAction[i]&ACK_TI) nXImagePos=0;
			else if (m_bUserAction[i]&ACK_PAO) nXImagePos=nItemWidth;
			else if (m_bUserAction[i]&ACK_WEI) nXImagePos=nItemWidth*2;
			else if (m_bUserAction[i]&ACK_PENG) nXImagePos=nItemWidth*4;
			else if (m_bUserAction[i]&ACK_CHI) nXImagePos=nItemWidth*3;

			//��������
			CImageHandle HandleActionBack(&m_ImageActionBack);
			m_ImageActionBack.AlphaDrawImage(pDC,nXPos,nYPos,m_ImageActionBack.GetWidth()/4,m_ImageActionBack.GetHeight(),
				(i+1)*m_ImageActionBack.GetWidth()/4,0,RGB(255,0,255));

			//�滭����
			int nItemHeight=m_ImageUserAction.GetHeight();
			m_ImageUserAction.AlphaDrawImage(pDC,nXPos+40,nYPos+28,nItemWidth,nItemHeight,nXImagePos,0,RGB(255,0,255));
		}
	}

	return;
}

//��������
void CGameClientView::SetCellScore(LONG lCellScore)
{
	//�����˿�
	if (lCellScore!=m_lCellScore)
	{
		//���ñ���
		m_lCellScore=lCellScore;

		//���½���
		UpdateGameView(NULL);
	}

	return;
}

//ׯ���û�
void CGameClientView::SetBankerUser(WORD wBankerUser)
{
	//�����û�
	if (wBankerUser!=m_wBankerUser)
	{
		//���ñ���
		m_wBankerUser=wBankerUser;

		//���½���
		UpdateGameView(NULL);
	}

	return;
}

//���û�ׯ
void CGameClientView::SetHuangZhuang(bool bHuangZhuang)
{
	//�����û�
	if (bHuangZhuang!=m_bHuangZhuang)
	{
		//���ñ���
		m_bHuangZhuang=bHuangZhuang;

		//���½���
		UpdateGameView(NULL);
	}

	return;
}

//ʣ���˿�
void CGameClientView::SetLeftCardCount(BYTE bLeftCardCount)
{
	//�����˿�
	if (bLeftCardCount!=m_bLeftCardCount)
	{
		//���ñ���
		m_bLeftCardCount=bLeftCardCount;

		//���½���
		UpdateGameView(NULL);
	}

	return;
}

//״̬��־
void CGameClientView::SetStatusFlag(bool bOutCard, bool bWaitOther)
{
	//���ñ���
	m_bOutCard=bOutCard;
	m_bWaitOther=bWaitOther;

	//���½���
	UpdateGameView(NULL);

	return;
}

//��Ϣ��Ŀ
void CGameClientView::SetUserHuXiCount(WORD wChairID, BYTE bHuXiCount)
{
	//���ñ���
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<CountArray(m_bUserHuXiCount);i++) m_bUserHuXiCount[i]=bHuXiCount;
	}
	else m_bUserHuXiCount[wChairID]=bHuXiCount;

	//���½���
	UpdateGameView(NULL);

	return;
}

//������Ϣ
void CGameClientView::SetUserAction(WORD wViewChairID, BYTE bUserAction)
{
	//���ñ���
	if (wViewChairID<GAME_PLAYER) m_bUserAction[wViewChairID]=bUserAction;
	else ZeroMemory(m_bUserAction,sizeof(m_bUserAction));

	//���½���
	UpdateGameView(NULL);

	return;
}

//������Ϣ
void CGameClientView::SetOutCardInfo(WORD wViewChairID, BYTE cbCardData)
{
	//���ñ���
	m_cbCardData=cbCardData;
	m_wOutCardUser=wViewChairID;

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

//�����Ϣ
void CGameClientView::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags, Point);

	//���ƴ���
	if (m_bMoveCard==false)
	{
		BYTE cbHoverCard=m_HandCardControl.GetHoverCard();
		if (cbHoverCard!=0) AfxGetMainWnd()->PostMessage(IDM_OUT_CARD,cbHoverCard,cbHoverCard);
	}

	//��������
	if (m_bSwitchCard==true)
	{
		//�ͷ����
		ReleaseCapture();

		//���ñ���
		m_bMoveCard=false;
		m_bSwitchCard=false;
		m_wMouseDownItem=INVALID_ITEM;
	}

	return;
}

//�����Ϣ
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);

	//��ȡ����
	m_wMouseDownItem=m_HandCardControl.SwitchCardPoint(Point);

	//�����ж�
	if ((m_wMouseDownItem!=INVALID_ITEM)&&(m_HandCardControl.IsPositively()==true))
	{
		//�������
		SetCapture();

		//���ñ���
		m_bMoveCard=false;
		m_bSwitchCard=true;
	}

	return;
}

//�����Ϣ
void CGameClientView::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags, Point);

	//�����˿�
	if (m_bSwitchCard==true)
	{
		//��ȡ���
		CPoint MousePoint;
		GetCursorPos(&MousePoint);
		ScreenToClient(&MousePoint);

		//��ȡ����
		WORD wMouseDownItem=m_wMouseDownItem;
		m_wMouseDownItem=m_HandCardControl.SwitchCardPoint(MousePoint);

		//�����ж�
		if (wMouseDownItem!=m_wMouseDownItem)
		{
			//�����˿�
			m_bMoveCard=true;
			m_HandCardControl.SwitchCardItem(wMouseDownItem,m_wMouseDownItem);

			//���½���
			UpdateGameView(NULL);
		}
	}

	return;
}

//�����Ϣ
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//�������
	bool bRePaint=false;
	bool bHandle=m_HandCardControl.OnEventSetCursor(MousePoint,bRePaint);

	//�ػ�����
	if (bRePaint==true) UpdateGameView(NULL);

	//������
	if (bHandle==false) __super::OnSetCursor(pWnd,nHitTest,uMessage);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
