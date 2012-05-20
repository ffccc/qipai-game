#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////
//��ť��ʶ 

//��ť��ʶ 
#define IDC_DISTUOGUAN					109								//ȡ���йܰ�ť
#define IDC_TUOGUAN						105								//�йܰ�ť

#define IDC_START						100								//��ʼ��ť
#define IDC_CHANG_CARD					104                             //�鿴���ư�ť								
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_START, OnStart)

	
	ON_BN_CLICKED(IDC_DISTUOGUAN, OnDisTuoGuan)
	ON_BN_CLICKED(IDC_TUOGUAN, OnTuoGuan)

	ON_REGISTERED_MESSAGE(WM_DD_DRAGENTER,  OnDragEnter)
	ON_REGISTERED_MESSAGE(WM_DD_DRAGDROP,	OnDragDrop)
	ON_REGISTERED_MESSAGE(WM_DD_DRAGABORT,	OnDragAbort)

	ON_MESSAGE(IDM_MOUSE_MOVE,OnSonMouseMove)	
END_MESSAGE_MAP()


//���캯��
CGameClientView::CGameClientView()
{
	m_IsStartSendCard=false;
	//��־����
	m_bOutCard=false;
	m_bWaitOther=false;
	m_bHuangZhuang=false;
	m_bDecideBanker=false;

	//��Ϸ����
	m_lCellScore=0l;
	m_wBankerUser=INVALID_CHAIR;
	m_leaveHeapCardData=0;

	//�û�״̬
	m_cbCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbTongCount,sizeof(m_cbTongCount));

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageWait.SetLoadInfo(IDB_WAIT_TIP,hInstance);
	m_ImageBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_ImageUserFlag.SetLoadInfo(IDB_USER_FLAG,hInstance);
	m_ImageOutCard.SetLoadInfo(IDB_OUT_CARD_TIP,hInstance);
	m_ImageUserAction.SetLoadInfo(IDB_USER_ACTION,hInstance);
	m_ImageActionBack.SetLoadInfo(IDB_ACTION_BACK,hInstance);
	m_ImageHuangZhuang.SetLoadInfo(IDB_HUANG_ZHUANG,hInstance);
	
	ZeroMemory(m_DecideBankerCard,sizeof(m_UserDecideCardPos));
	ZeroMemory(m_UserDecideCardPos,sizeof(m_UserDecideCardPos));

	m_cbCurrentCard = 0;					//��յ�ǰ��

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

	enDirection Direction[]={Direction_East,Direction_South,Direction_West};

	//���ÿؼ�
	for (WORD i=0;i<3;i++)
	{

		m_TableCard[i].SetDirection(Direction[i]);
		m_DiscardCard[i].SetDirection(Direction[i]);

		//����˿�
		m_WeaveCard[i][0].SetDisplayItem(true);
		m_WeaveCard[i][1].SetDisplayItem(true);
		m_WeaveCard[i][2].SetDisplayItem(true);
		m_WeaveCard[i][3].SetDisplayItem(true);
		m_WeaveCard[i][4].SetDisplayItem(true);
		m_WeaveCard[i][5].SetDisplayItem(true);
		m_WeaveCard[i][6].SetDisplayItem(true);
		m_WeaveCard[i][7].SetDisplayItem(true);


		m_WeaveCard[i][0].SetDirection(Direction[i]);
		m_WeaveCard[i][1].SetDirection(Direction[i]);
		m_WeaveCard[i][2].SetDirection(Direction[i]);
		m_WeaveCard[i][3].SetDirection(Direction[i]);
		m_WeaveCard[i][4].SetDirection(Direction[i]);
		m_WeaveCard[i][5].SetDirection(Direction[i]);
		m_WeaveCard[i][6].SetDirection(Direction[i]);
		m_WeaveCard[i][7].SetDirection(Direction[i]);

	}

	m_UserCard[0].SetDirection(Direction_East);
	m_UserCard[2].SetDirection(Direction_West);

	//�����ؼ�
	CRect rcCreate(0,0,0,0);
	
	m_GameScoreWnd.Create(NULL,NULL,WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,100);
	m_ControlWnd.Create(NULL,NULL,WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,10,NULL);

	for(BYTE k=0;k<9;k++)
	{
		m_HandCard[k].Create(NULL,NULL,WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,1000+k,NULL);
	}

	//11-28
	m_HuCardDlg.Create(NULL,NULL,WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,11);

	m_btDisTuoGuan.Create(NULL,WS_CHILD,rcCreate,this,IDC_DISTUOGUAN);
	m_btTuoGuan.Create(NULL,WS_CHILD,rcCreate,this,IDC_TUOGUAN);


	//�����ؼ�
	m_btStart.Create(NULL,WS_CHILD,rcCreate,this,IDC_START);

	m_btDisTuoGuan.SetButtonImage(IDB_BT_DISTUOGUAN,AfxGetInstanceHandle(),false);
	m_btTuoGuan.SetButtonImage(IDB_BT_TUOGUAN,AfxGetInstanceHandle(),false);

	m_btStart.SetButtonImage(IDB_BT_START,AfxGetInstanceHandle(),false);

	//�����ϷŹ�����
	HWND hWnd[9];
	for(int i=0;i<9;i++)
	{
		hWnd[i] = m_HandCard[i].m_hWnd;
	}
	m_ddManager.InitDragDropManager(this,hWnd);

	return 0;
}

//���ý���
void CGameClientView::ResetGameView()
{
	m_IsStartSendCard=false;

	//��־����
	m_bOutCard=false;
	m_bWaitOther=false;
	m_bHuangZhuang=false;
	m_bDecideBanker=true;

	//��Ϸ����
	m_lCellScore=0l;
	m_wBankerUser=INVALID_CHAIR;

	//�û�״̬
	m_cbCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	m_btDisTuoGuan.ShowWindow(SW_HIDE);
	m_btTuoGuan.ShowWindow(SW_HIDE);

	//��������
	m_btStart.ShowWindow(SW_HIDE);

	m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameScoreWnd.ShowWindow(SW_HIDE);

	//11-28
	m_HuCardDlg.ShowWindow(SW_HIDE);


	//�˿�����
	m_UserCard[0].SetCardData(0,false);
	m_UserCard[2].SetCardData(0,false);

	//11-30wan
	for (BYTE i=0; i<9; i++)
	{
		m_HandCard[i].SetCardData(NULL,0);
		m_HandCard[i].ShowWindow(SW_HIDE);
	}

	//�˿�����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_leaveHeapCardData=0;
		m_TableCard[i].SetCardData(NULL,0);
		m_DiscardCard[i].SetCardData(NULL,0);

		m_WeaveCard[i][0].SetCardData(NULL,0);
		m_WeaveCard[i][1].SetCardData(NULL,0);
		m_WeaveCard[i][2].SetCardData(NULL,0);
		m_WeaveCard[i][3].SetCardData(NULL,0);
		m_WeaveCard[i][4].SetCardData(NULL,0);
		m_WeaveCard[i][5].SetCardData(NULL,0);
		m_WeaveCard[i][6].SetCardData(NULL,0);
		m_WeaveCard[i][7].SetCardData(NULL,0);
	}

	////////////////////////////////////////////////////
	ZeroMemory(m_DecideBankerCard,sizeof(m_UserDecideCardPos));
	ZeroMemory(m_UserDecideCardPos,sizeof(m_UserDecideCardPos));
	ZeroMemory(m_cbTongCount,sizeof(m_cbTongCount));

	return;
}

//�����ؼ�
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	m_ptFace[0].x=nWidth-m_nXBorder-m_nXFace-25;//��
	m_ptFace[0].y=nHeight/2-m_nYFace-50;	
	m_ptName[0].x=nWidth-m_nXBorder-m_nXFace-60+5;
	m_ptName[0].y=nHeight/2-20;
	m_ptTimer[0].x=nWidth-m_nXBorder-m_nXFace-25+5+3+7;
	m_ptTimer[0].y=nHeight/2-m_nYFace-55-m_nYTimer-20;
	m_ptReady[0].x=nWidth-m_nXBorder-m_nXTimer-25;
	m_ptReady[0].y=nHeight/2-m_nYFace-55-m_nYTimer/2;
	m_UserFlagPos[0].x=nWidth-m_nXBorder-28-20-3-6;
	m_UserFlagPos[0].y=nHeight/2-m_nYFace-83-m_nYTimer+153-115;

	m_UserDecideCardPos[0].x=m_ptFace[0].x-5;
	m_UserDecideCardPos[0].y=nHeight/2;


	m_ptFace[1].x=nWidth/2-m_nXFace/2-5; //��
	m_ptFace[1].y=nHeight-100+43+15+5;
	m_ptName[1].x=nWidth/2-50-5+66;
	m_ptName[1].y=nHeight-40+40-3+3+5;	
	m_ptTimer[1].x=nWidth/2-m_nXFace-m_nXTimer*2+3+300;
	m_ptTimer[1].y=nHeight-80+40-3+15+5;	
	m_ptReady[1].x=nWidth/2-m_nXFace-5-30;
	m_ptReady[1].y=nHeight-80+40-3;
	m_UserFlagPos[1].x=nWidth/2-m_nXFace-m_nXTimer*2-29+5+182-100;
	m_UserFlagPos[1].y=nHeight-80+45-3;

	m_UserDecideCardPos[1].x=nWidth/2+15;
	m_UserDecideCardPos[1].y=m_ptFace[1].y-5;


	m_ptFace[2].x=m_nXBorder+30+10; //��
	m_ptFace[2].y=nHeight/2-m_nYFace-50;	
	m_ptName[2].x=m_nXBorder+10;
	m_ptName[2].y=nHeight/2-20;
	m_ptTimer[2].x=m_nXBorder+30+10+10+3;
	m_ptTimer[2].y=nHeight/2-m_nYFace-55-m_nYTimer-20;
	m_ptReady[2].x=m_nXBorder+25+m_nXTimer+5+5+10;
	m_ptReady[2].y=nHeight/2-m_nYFace-55-m_nYTimer/2;
	m_UserFlagPos[2].x=m_nXBorder+35+5+10-10;
	m_UserFlagPos[2].y=nHeight/2-m_nYFace-83-m_nYTimer+153-115;

	m_UserDecideCardPos[2].x=m_ptFace[2].x+5;
	m_UserDecideCardPos[2].y=nHeight/2;

	//�û��˿�
	m_UserCard[0].SetControlPoint(nWidth-172+24,nHeight/2-310);//�¼� ��
	m_UserCard[2].SetControlPoint(m_nXBorder+100+90-65-24,nHeight/2+160);       //�ϼ� ��

	//�����˿�
	m_TableCard[0].SetControlPoint(nWidth-m_nXBorder-m_nXFace-30-25-17,nHeight/2-197);//��
	m_TableCard[1].SetControlPoint(nWidth/2+185+40+32-6,nHeight-95+20-70); //��
	m_TableCard[2].SetControlPoint(m_nXBorder+m_nXFace+22+30+3,nHeight/2+100);//��

	//�ѻ��˿�
	int nXCenter=nWidth/2;
	int nYCenter=nHeight/2;
	m_LHeapCard.SetControlPoint(m_nXBorder+m_nXFace+50,m_nYBorder+30);
	
	//����λ�õ��������ұߣ������µ�������
	m_DiscardCard[0].SetControlPoint(nXCenter+36+70+40-50,nYCenter-96-100+26);//	�ұ�	
	m_DiscardCard[1].SetControlPoint(nXCenter+100-15,nYCenter+10);	 //	�±�
	m_DiscardCard[2].SetControlPoint(nXCenter-220+50+30,nYCenter+73-85);	 //���

	//����˿�  ��
	m_WeaveCard[0][0].SetControlPoint(nWidth-m_nXBorder-m_nXFace-83-16-42+30-64+24,nHeight/2);
	m_WeaveCard[0][1].SetControlPoint(nWidth-m_nXBorder-m_nXFace-83-16-42+30-64+24,nHeight/2-60);
	m_WeaveCard[0][2].SetControlPoint(nWidth-m_nXBorder-m_nXFace-83-16-42+30-64+24,nHeight/2-120);
	m_WeaveCard[0][3].SetControlPoint(nWidth-m_nXBorder-m_nXFace-83-16-42+30-64+24,nHeight/2-180);
	m_WeaveCard[0][4].SetControlPoint(nWidth-m_nXBorder-m_nXFace-83-16-42+30-64+24,nHeight/2-240);
	m_WeaveCard[0][5].SetControlPoint(nWidth-m_nXBorder-m_nXFace-83-16-42+30-64+24,nHeight/2-300);

	m_WeaveCard[0][6].SetControlPoint(nWidth-m_nXBorder-m_nXFace-83-16-42+30-64+64+24,nHeight/2);
	m_WeaveCard[0][7].SetControlPoint(nWidth-m_nXBorder-m_nXFace-83-16-42+30-64+64+24,nHeight/2-64);

	//����˿�  ��
	m_WeaveCard[1][0].SetControlPoint(nWidth/2-280,nHeight-150);
	m_WeaveCard[1][1].SetControlPoint(nWidth/2-220,nHeight-150);
	m_WeaveCard[1][2].SetControlPoint(nWidth/2-160,nHeight-150);
	m_WeaveCard[1][3].SetControlPoint(nWidth/2-100,nHeight-150);
	m_WeaveCard[1][4].SetControlPoint(nWidth/2-40,nHeight-150);
	m_WeaveCard[1][5].SetControlPoint(nWidth/2+20,nHeight-150);
	m_WeaveCard[1][6].SetControlPoint(nWidth/2+80,nHeight-150);
	m_WeaveCard[1][7].SetControlPoint(nWidth/2+140,nHeight-150);
	
	//����˿�  ��
	m_WeaveCard[2][0].SetControlPoint(m_nXBorder+m_nXFace+100+20+64-24,nHeight/2-150);
	m_WeaveCard[2][1].SetControlPoint(m_nXBorder+m_nXFace+100+20+64-24,nHeight/2-90);
	m_WeaveCard[2][2].SetControlPoint(m_nXBorder+m_nXFace+100+20+64-24,nHeight/2-30);
	m_WeaveCard[2][3].SetControlPoint(m_nXBorder+m_nXFace+100+20+64-24,nHeight/2+30);
	m_WeaveCard[2][4].SetControlPoint(m_nXBorder+m_nXFace+100+20+64-24,nHeight/2+90);
	m_WeaveCard[2][5].SetControlPoint(m_nXBorder+m_nXFace+100+20+64-24,nHeight/2+150);

	m_WeaveCard[2][6].SetControlPoint(m_nXBorder+m_nXFace+100+20+64-64-24,nHeight/2-150);
	m_WeaveCard[2][7].SetControlPoint(m_nXBorder+m_nXFace+100+20+64-64-24,nHeight/2-90);

	//���ƴ���
	m_ControlWnd.SetBenchmarkPos(nWidth/2+298,nHeight-m_nYBorder-200-m_nYFace);
	
	//fe11-30
	CRect clientRect;
	GetClientRect(&clientRect);
	m_nAreaLeftBorder = 115;
	m_nAreaYValue = nHeight-m_nYBorder-200-m_nYFace;
	m_nAreaWidth = (nWidth-255)/9;

	for(BYTE k=0;k<9;k++)
	{
		m_HandCard[k].SetBenchmarkPos(m_nAreaWidth*k+m_nAreaLeftBorder,m_nAreaYValue);
	}
	//fe11-30
	m_CurAreaPoiBench.x = m_nAreaLeftBorder+m_nAreaWidth*9;
	m_CurAreaPoiBench.y = m_nAreaYValue;
	
	//11-28
	CRect rcHuCard;
	m_HuCardDlg.GetWindowRect(&rcHuCard);
	m_HuCardDlg.SetBenchmarkPos((nWidth-rcHuCard.Width())/2-100,(nHeight-rcHuCard.Height())/2-150);

	//�����ɼ�
	CRect rcScore;
	m_GameScoreWnd.GetWindowRect(&rcScore);
	m_GameScoreWnd.MoveWindow((nWidth-rcScore.Width())/2,(nHeight-rcScore.Height())/2-60,rcScore.Width(),rcScore.Height());


	//�ƶ���ť
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);

	m_btDisTuoGuan.GetWindowRect(&rcButton);
	m_btTuoGuan.GetWindowRect(&rcButton);

	m_btStart.GetWindowRect(&rcButton);

	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;


	//�ƶ�����
	DeferWindowPos(hDwp,m_btTuoGuan,NULL,(nWidth-rcButton.Width())/2+120+200,nHeight-170-m_nYBorder+85+30+15,0,0,uFlags);
	DeferWindowPos(hDwp,m_btDisTuoGuan,NULL,(nWidth-rcButton.Width())/2+120+200,nHeight-170-m_nYBorder+85+30+15,0,0,uFlags);

	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2+120,nHeight-170-m_nYBorder+85+30,0,0,uFlags);

	//�����ƶ�
	EndDeferWindowPos(hDwp);

	return;
}


void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{

	DrawViewImage(pDC,m_ImageBack,enMode_ElongGate);

	CImageHandle HandleHeadBg(&m_ImageHeadBg);

	//�滭�û�
	pDC->SetTextColor(RGB(255,255,0));
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		const tagUserData * pUserData=GetUserInfo(i);
		if (pUserData!=NULL)
		{
			//�û�����
			pDC->SetTextAlign((i==1)?(TA_CENTER|TA_TOP):(TA_CENTER|TA_TOP));
			if(i==1)
			{
				DrawTextString(pDC,pUserData->szName,RGB(240,240,240),RGB(50,50,50),m_ptName[i].x+47,m_ptName[i].y-35);
			}
			else
			{
				DrawTextString(pDC,pUserData->szName,RGB(240,240,240),RGB(50,50,50),m_ptName[i].x+47,m_ptName[i].y-26);
			
			}
			
			//������Ϣ
			WORD wUserTimer=GetUserTimer(i);
			if (wUserTimer!=0) DrawUserTimer(pDC,m_ptTimer[i].x,m_ptTimer[i].y,wUserTimer);
			if (pUserData->cbUserStatus==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			DrawUserFace(pDC,pUserData,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);
		}
	}


	if(m_leaveHeapCardData!=0)
	{
		m_LHeapCard.DrawHeapCard(pDC,m_leaveHeapCardData,nWidth,m_nXFace);
		CRect rcDraw;
		
		CFont ft,*oldft;
		ft.CreatePointFont(220,"�����п�����",pDC);
		pDC->SetTextColor(RGB(81,109,144));
		oldft=pDC->SelectObject(&ft);		
		pDC->SetBkMode(TRANSPARENT);
	
		CString cDuiPaiInfo;
		cDuiPaiInfo.Format("��  ��  �� ");
		pDC->TextOut(nWidth/2-m_nXFace/2+10,0,cDuiPaiInfo);
		pDC->SelectObject(oldft);
	}


	//�滭���ƴ���
	CPoint TongPosition[GAME_PLAYER];
	TongPosition[0].SetPoint(m_ptName[0].x+30,m_ptName[0].y);
	TongPosition[1].SetPoint(m_ptName[1].x+60,m_ptName[1].y-24);
	TongPosition[2].SetPoint(m_ptName[2].x+25,m_ptName[2].y);

	//��ʾ���ƴ���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if(m_cbTongCount[i]!=0)
		{
			CString cTongCount;
			cTongCount.Format("���ƴ���:%d",m_cbTongCount[i]);

			DrawTextString(pDC,cTongCount,RGB(240,240,240),RGB(50,50,50),TongPosition[i].x,TongPosition[i].y);
			
		}
	}

	//�������ҷ��Ķ�ׯ����
	if(m_bDecideBanker)
	{
		if(m_DecideBankerCard[1]!=0)
		{
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if(((m_DecideBankerCard[i]&MASK_COLOR)>>4)!=1)
				{
					g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,m_DecideBankerCard[i],m_UserDecideCardPos[i].x,m_UserDecideCardPos[i].y);
				}
				else
				{
					g_CardResource.m_ImageTableBottom1.DrawCardItem(pDC,m_DecideBankerCard[i],m_UserDecideCardPos[i].x,m_UserDecideCardPos[i].y);
				}
			}
		}

	}		

	//�û���־
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		//����λͼ
		CImageHandle ImageHandle(&m_ImageUserFlag);
		int nImageWidth=m_ImageUserFlag.GetWidth()/4;
		int nImageHeight=m_ImageUserFlag.GetHeight();
	
		//�����־��ׯ�������������ϱ�ͼƬ
		m_ImageUserFlag.BitBlt(pDC->m_hDC,m_UserFlagPos[m_wBankerUser].x,m_UserFlagPos[m_wBankerUser].y,nImageWidth,nImageHeight,0,0);
	}

	//�����˿�
	for (WORD i=0;i<3;i++)
	{
		m_TableCard[i].DrawCardControl(pDC);
		m_DiscardCard[i].DrawCardControl(pDC);

		m_WeaveCard[i][0].DrawCardControl(pDC);
		m_WeaveCard[i][1].DrawCardControl(pDC);
		m_WeaveCard[i][2].DrawCardControl(pDC);
		m_WeaveCard[i][3].DrawCardControl(pDC);
		m_WeaveCard[i][4].DrawCardControl(pDC);
		m_WeaveCard[i][5].DrawCardControl(pDC);
		m_WeaveCard[i][6].DrawCardControl(pDC);
		m_WeaveCard[i][7].DrawCardControl(pDC);
	}
	//�û��˿�--�������û������ƣ�����ʾ����
	m_UserCard[0].DrawCardControl(pDC);
	m_UserCard[2].DrawCardControl(pDC);

	//������ʾ
	if (m_bOutCard==true)
	{
		CImageHandle HandleOutCard(&m_ImageOutCard);
		m_ImageOutCard.AlphaDrawImage(pDC,(nWidth-m_ImageOutCard.GetWidth())/2,nHeight-260,RGB(255,0,255));
	}

	//�ȴ���ʾ
	if (m_bWaitOther==true)
	{
		CImageHandle HandleWait(&m_ImageWait);
		m_ImageWait.AlphaDrawImage(pDC,(nWidth-m_ImageWait.GetWidth())/2,nHeight-260,RGB(255,0,255));
	}

	//��ׯ��־
	if (m_bHuangZhuang==true)
	{
		CImageHandle HandleHuangZhuang(&m_ImageHuangZhuang);
		m_ImageHuangZhuang.AlphaDrawImage(pDC,(nWidth-m_ImageHuangZhuang.GetWidth())/2,nHeight/2-103+45,RGB(255,0,255));
	}

	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if((m_cbUserAction[i]&WIK_TONG_GIVE_UP)==0)
		{
			if ((m_wOutCardUser==i)||(m_cbUserAction[i]!=0))
			{
				//����λ��
				int nXPos=0,nYPos=0;
				switch (i)
				{
				case 0:	//����
					{
						nXPos=nWidth-m_nXBorder-170-50-130;
						nYPos=nHeight/2-71+30;
						break;
					}
				case 1:	//����
					{
						nXPos=nWidth/2-32;
						nYPos=nHeight-m_nYBorder-220-20;
						break;
					}
				case 2:	//����
					{
						nXPos=m_nXBorder+115+20+150;
						nYPos=nHeight/2-71+30;
						break;
					}
				}

				
				//�滭����
				if (m_cbUserAction[i]!=WIK_NULL)
				{
					//��������
					int nXImagePos=0;
					CImageHandle ImageHandle(&m_ImageUserAction);
					int nItemWidth=m_ImageUserAction.GetWidth()/7;
					//��������
					CImageHandle ImageHandleAction(&m_ImageActionBack);
					m_ImageActionBack.AlphaDrawImage(pDC,nXPos,nYPos-100,m_ImageActionBack.GetWidth()/4,m_ImageActionBack.GetHeight(),
						(i+1)*m_ImageActionBack.GetWidth()/4,0,RGB(255,0,255));

					//����λ��
					if ((m_cbUserAction[i]&WIK_SAN_ZHAO)!=0||(m_cbUserAction[i]&WIK_SI_ZHAO)!=0) nXImagePos=nItemWidth;
					else if ((m_cbUserAction[i]&WIK_SI_TONG)!=0||(m_cbUserAction[i]&WIK_WU_TONG)!=0) nXImagePos=nItemWidth*2;
					//else if (m_cbUserAction[i]&WIK_GANG) nXImagePos=nItemWidth*3;
					else if (m_cbUserAction[i]&WIK_CHI_HU) nXImagePos=nItemWidth*4;

					//�滭����
					int nItemHeight=m_ImageUserAction.GetHeight();
					//m_ImageUserAction.BitBlt(pDC->m_hDC,nXPos+13,nYPos+15,nItemWidth,nItemHeight,nXImagePos,0);
					m_ImageUserAction.AlphaDrawImage(pDC,nXPos+13+13,nYPos+15+13-100,nItemWidth,nItemHeight,nXImagePos,0,RGB(106,203,255));
				}
				else
				{
					//�滭�˿�
					//g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,m_cbCardData,nXPos+18,nYPos+15);
					//�滭�˿�
					if(((m_cbCardData&MASK_COLOR)>>4)!=1)
					{			
						g_CardResource.m_ImageUserBottom.DrawCardItem(pDC,m_cbCardData,nXPos+33,nYPos+18-100);
					}
					else
					{
						g_CardResource.m_ImageUserBottom1.DrawCardItem(pDC,m_cbCardData,nXPos+33,nYPos+18-100);
					}
				}
				
			}
		}
	}
	return;
}

//��������
void CGameClientView::SetCellScore(LONG lCellScore)
{
	//�����˿�
	if (lCellScore!=m_lCellScore)//��������
	{
		//���ñ���
		m_lCellScore=lCellScore;

		//���½���
		UpdateGameView(NULL);
	}

	return;
}

//����
void CGameClientView::SetHuangZhuang(bool bHuangZhuang)
{
	//�����˿�
	if (bHuangZhuang!=m_bHuangZhuang)
	{
		//���ñ���
		m_bHuangZhuang=bHuangZhuang;

		//���½���
		UpdateGameView(NULL);
	}

	return;
}


//�����˿�
void CGameClientView::SetHD(bool bHD)
{
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

//������Ϣ
void CGameClientView::SetUserAction(WORD wViewChairID, BYTE bUserAction)
{
	//���ñ���
	if (wViewChairID<GAME_PLAYER) m_cbUserAction[wViewChairID]=bUserAction;
	else ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));

	//���½���
	UpdateGameView(NULL);

	return;
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

//�����Ϣ
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//������
	__super::OnSetCursor(pWnd,nHitTest,uMessage);

	return TRUE;
}

//��������������������ƶ�ʱ���ͻὫ����ƶ���Ϣת������ͼ��
LRESULT CGameClientView::OnSonMouseMove(WPARAM wParam, LPARAM lParam)
{
	//��õ�ǰ����Ӵ��������Ļλ��
	CPoint MousePoint;
	MousePoint.x = (LONG)wParam;
	MousePoint.y = (LONG)lParam;

	ScreenToClient(&MousePoint);	

	CWnd *pWnd = ChildWindowFromPoint(MousePoint);
	CHandCard *pHandCardTarget = NULL; 

	for(int i=0;i<9;i++)
	{
		//�ҵ���ǰ����
		if(m_HandCard[i].m_bCurrentCardArea==true)
		{
			m_HandCard[i].SetCurrentCardArea(false);
		}
		//�ҵ�Ŀ������
		if(pWnd!=NULL && pWnd==&m_HandCard[i])
		{
			pHandCardTarget=&m_HandCard[i];
		}
	}

	if(pHandCardTarget!=NULL)
	{
		pHandCardTarget->SetCurrentCardArea(true);
	}
	return 0;
}
//��ʼ��ť
void CGameClientView::OnStart()
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_START,0,0);

	return;
}

//�йܰ�ť
void CGameClientView::OnTuoGuan()
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_TUOGUAN,0,0);
	return;
}
//ȡ���йܰ�ť
void CGameClientView::OnDisTuoGuan()
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_DISTUOGUAN,0,0);
	return;
}

//////////////////////////////////////////////////////////////////////////

//fe11-29
//�õ���ǰ��Ч��������
int CGameClientView::GetCardAreaCount()
{
	return 9;
}

BOOL CGameClientView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_F12)
		{
			MessageBox("������翪����Ա����ܰ�ϼ������ܣ���ܰ���������廪�����С�������С�ܣ����С�ƣ����С������ܺ���");
		}
		return true;
	}
	return m_ddManager.PreTranslateMessage(pMsg)?true:CGameFrameView::PreTranslateMessage(pMsg);
}

//���÷��ƴ�������Ӧ����¼�
void CGameClientView::EnableOutCard()
{

	for(BYTE i=0;i<9;i++)
	{
		m_HandCard[i].m_bCanOutCard=true;
	}
	
}
//���÷��ƴ��ڲ���Ӧ����¼�
void CGameClientView::DisableOutCard()
{
	for(BYTE i=0;i<9;i++)
	{
		m_HandCard[i].m_bCanOutCard=false;
	}
}

// ��ʼ�϶����������û�ѡ�е��ƶ�����һ���ϷŶ���
LRESULT CGameClientView::OnDragEnter(WPARAM wp, LPARAM lp)
{
	DRAGDROPINFO& ddi = *(DRAGDROPINFO*)lp;
	CHandCard* pHandCard = DYNAMIC_DOWNCAST(CHandCard, GetDlgItem((int)wp));
	ASSERT(pHandCard != NULL);  
	if(pHandCard != NULL)
	{
		BYTE cbCardIndex=pHandCard->m_cbHoverItem;
		if(cbCardIndex!=0xFF)
		{
			BYTE cbCardData = pHandCard->m_CardItemArray[cbCardIndex].cbCardData;
			if (cbCardData != 0xFF) 
			{	
				ddi.data = new CDragDropData(cbCardData);
				return TRUE; 
			}
		}
	}
	return FALSE;
}

//////////////////
// �����϶�����: �����϶����Ʒŵ�Ŀ���Ӵ�����
//
LRESULT CGameClientView::OnDragDrop(WPARAM wp, LPARAM lp)
{
	DRAGDROPINFO& ddi = *(DRAGDROPINFO*)lp;
	BYTE cbCardData = ddi.data->GetDragData();

	//�õ�Դ��Ŀ�괰�ڵľ��
	CWnd* pWndSource = CWnd::FromHandle(ddi.hwndSource);
	CWnd* pWndTarget = CWnd::FromHandle(ddi.hwndTarget);

	//�õ�Դ��Ŀ�괰�ڵĶ���ָ��
	CHandCard* pHandCardSource = DYNAMIC_DOWNCAST(CHandCard, pWndSource);
	CHandCard* pHandCardTarget = DYNAMIC_DOWNCAST(CHandCard, pWndTarget);

	//���ͷŴ�����Ŀ������ʱ��ʲôҲ����
	ASSERT((pHandCardSource != NULL) && (pHandCardTarget != NULL));
	if((pHandCardTarget == NULL) || (pHandCardSource == NULL)) 	return 0;

	//��Ŀ��������ԭ����ʱ�����ԭ������ɾ���ƣ���Ŀ��������������
	if(pHandCardSource != pHandCardTarget)
	{
	
		if((pHandCardTarget->GetCardCount())<9)
		{		
			pHandCardSource->DeleteCardData(cbCardData);		
			pHandCardTarget->AddCardData(cbCardData);
		}
	}
	return 0;
}

//////////////////
// Drag aborted (Eg, user pressed Esc).
//
LRESULT CGameClientView::OnDragAbort(WPARAM wp, LPARAM lp)
{
	return 0;
}
