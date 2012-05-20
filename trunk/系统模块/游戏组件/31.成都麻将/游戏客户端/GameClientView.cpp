#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////
//��ť��ʶ 

#define IDC_START						100								//��ʼ��ť
#define IDC_TRUSTEE_CONTROL				104								//�йܿ���
#define IDI_GANG_SOCRE					105								//

//������ʶ
#define IDI_BOMB_EFFECT					101								//������ʶ
//������Ŀ
#define BOMB_EFFECT_COUNT				12								//������Ŀ
#define DISC_EFFECT_COUNT				8								//����Ч��

#define TIME_GANG_SCORE					70								//
#define GANG_MOVE_COUNT					14								//

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_TRUSTEE_CONTROL,OnStusteeControl)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
	//��־����
	m_bOutCard=false;
	m_bWaitOther=false;
	ZeroMemory( m_lGangScore,sizeof(m_lGangScore) );
	m_wGangChairId = INVALID_CHAIR;

	//��Ϸ����
	m_lCellScore=0L;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory( m_cbWinOrder,sizeof(m_cbWinOrder) );

	//��������
	m_bBombEffect=false;
	m_cbBombFrameIndex=0;

	//����Ч��
	m_wDiscUser=INVALID_CHAIR;
	m_cbDiscFrameIndex=0;

	//�û�״̬
	m_cbCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageWait.SetLoadInfo(IDB_WAIT_TIP,hInstance);
	m_ImageBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_ImageUserFlag.SetLoadInfo(IDB_USER_FLAG,hInstance);
	m_ImageOutCard.SetLoadInfo(IDB_OUT_CARD_TIP,hInstance);
	m_ImageUserAction.SetLoadInfo(IDB_USER_ACTION,hInstance);
	m_ImageActionBack.SetLoadInfo(IDB_ACTION_BACK,hInstance);
	m_ImageTrustee.LoadImage(hInstance,TEXT("TRUSTEE"));
	m_ImageActionAni.LoadImage(AfxGetInstanceHandle(),TEXT("ActionAni"));
	m_ImageDisc.LoadImage(AfxGetInstanceHandle(),TEXT("DISC"));
	m_ImageArrow.LoadImage(AfxGetInstanceHandle(),TEXT("ARROW"));
	m_ImageCenter.SetLoadInfo(IDB_VIEW_CENTER,hInstance);
	m_ImageWinOrder.SetLoadInfo(IDB_WIN_ORDER,hInstance);
	m_ImageCellScore.LoadImage(AfxGetInstanceHandle(),TEXT("CELL_SCORE"));
	m_PngNumLost.LoadImage(AfxGetInstanceHandle(),TEXT("NUM_LOST"));
	m_PngNumWin.LoadImage(AfxGetInstanceHandle(),TEXT("NUM_WIN"));
	m_PngXiaYu.LoadImage(AfxGetInstanceHandle(),TEXT("XIAO_YU"));
	m_PngGuaFeng.LoadImage(AfxGetInstanceHandle(),TEXT("GUA_FENG"));

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
	enDirection Direction[]={Direction_North,Direction_East,Direction_South,Direction_West};

	//���ÿؼ�
	for (WORD i=0;i<4;i++)
	{
		//�û��˿�
		m_HeapCard[i].SetDirection(Direction[i]);
		m_TableCard[i].SetDirection(Direction[i]);
		m_DiscardCard[i].SetDirection(Direction[i]);

		//����˿�
		m_WeaveCard[i][0].SetDisplayItem(true);
		m_WeaveCard[i][1].SetDisplayItem(true);
		m_WeaveCard[i][2].SetDisplayItem(true);
		m_WeaveCard[i][3].SetDisplayItem(true);
		m_WeaveCard[i][0].SetDirection(Direction[i]);
		m_WeaveCard[i][1].SetDirection(Direction[i]);
		m_WeaveCard[i][2].SetDirection(Direction[i]);
		m_WeaveCard[i][3].SetDirection(Direction[i]);
	}

	//���ÿؼ�
	m_UserCard[0].SetDirection(Direction_North);
	m_UserCard[1].SetDirection(Direction_East);
	m_UserCard[2].SetDirection(Direction_West);


	//�����ؼ�
	CRect rcCreate(0,0,0,0);
	m_ControlWnd.Create(NULL,NULL,WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,10,NULL);
	m_DrawSiceWnd.Create(NULL,NULL,WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,11,NULL);
	m_DrawSiceWnd.SetWindowSink(AfxGetMainWnd());

	//�����ؼ�
	m_btStart.Create(NULL,WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,IDC_START);
	m_btStart.SetButtonImage(IDB_BT_START,AfxGetInstanceHandle(),false);

	//�йܰ�ť
	m_btStusteeControl.Create(TEXT(""),WS_CHILD|WS_DISABLED|WS_VISIBLE,rcCreate,this,IDC_TRUSTEE_CONTROL);
	m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,AfxGetInstanceHandle(),false);

	SetTimer(IDI_DISC_EFFECT,250,NULL);
	
	//test
	//m_btStart.ShowWindow( SW_SHOW );
	//BYTE byCard[] = {
	//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,					
	//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
	//	0x01,0x02,0x03,0x04,0x05,0x06
	//};
	//BYTE byCardCount = 14;
	//m_HandCardControl.SetDisplayItem(true);
	//m_HandCardControl.SetPositively(true);
	//m_HandCardControl.SetCardData(byCard,MAX_COUNT-1-3,byCard[MAX_COUNT-1]);
	//for( BYTE i = 0; i < GAME_PLAYER; i++ )
	//{
	//	m_HeapCard[i].SetCardData(0,0,HEAP_FULL_COUNT);
	//	m_TableCard[i].SetCardData(byCard,byCardCount);
	//	m_DiscardCard[i].SetCardData(byCard,byCardCount);
	//	m_WeaveCard[i][0].SetCardData(byCard,3);
	//	if( i < 3 )
	//		m_UserCard[i].SetCardData(MAX_COUNT-1,false);
	//	m_bTrustee[i] = true;
	//	m_cbUserAction[i] = WIK_PENG;
	//	m_cbWinOrder[i] = i+1;
	//}
	//m_bWaitOther = true;
	//m_bOutCard = true;
	//m_wOutCardUser = 0;
	//m_cbCardData = 0x01;
	//m_cbUserAction[0] = WIK_NULL;
	//m_wDiscUser = 1;
	//SetBombEffect(true);
	//tagScoreInfo si;
	//ZeroMemory( &si,sizeof(si) );
	//for( WORD i = 0; i < GAME_PLAYER; i++ )
	//	lstrcpy( si.szUserName[i],TEXT("�������") );
	//for( WORD i = 0; i < GAME_PLAYER; i++ )
	//{
	//	si.wMeChairId = i;
	//	si.cbGenCount = i+1;
	//	si.cbWinOrder= i+1;
	//	if( i%2==0 )
	//	{
	//		si.dwChiHuRight[0] = CHR_QING_LONG_QI_DUI;
	//		si.lGameScore = 1234L;
	//	}
	//	else si.lGameScore = -1234L;
	//	si.wLostFanShu[(i+1)%GAME_PLAYER] = 5;
	//	si.wLostFanShu[(i+2)%GAME_PLAYER] = 6;
	//	si.lGameTax = 1234L;
	//	si.lGangScore = 124L;
	//	si.wProvideUser = 3;
	//	m_ScoreControl[i].SetScoreInfo( si );
	//}
	//end test

	return 0;
}

//���ý���
void CGameClientView::ResetGameView()
{
	//��־����
	m_bOutCard=false;
	m_bWaitOther=false;
	ZeroMemory( m_lGangScore,sizeof(m_lGangScore) );
	m_wGangChairId = INVALID_CHAIR;

	//��Ϸ����
	m_lCellScore=0L;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory( m_cbWinOrder,sizeof(m_cbWinOrder) );

	//��������
	m_bBombEffect=false;
	m_cbBombFrameIndex=0;

	//����Ч��
	m_wDiscUser=INVALID_CHAIR;
	m_cbDiscFrameIndex=0;

	//�û�״̬
	m_cbCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));

	//��������
	m_btStart.ShowWindow(SW_HIDE);
	m_ControlWnd.ShowWindow(SW_HIDE);
	for( WORD i = 0; i < GAME_PLAYER; i++ )
		m_ScoreControl[i].RestorationData();
	
	//���ÿؼ�
	m_btStusteeControl.EnableWindow(FALSE);

	//�˿�����
	m_UserCard[0].SetCardData(0,false);
	m_UserCard[1].SetCardData(0,false);
	m_UserCard[2].SetCardData(0,false);
	m_HandCardControl.SetPositively(false);
	m_HandCardControl.SetDisplayItem(false);
	m_HandCardControl.SetCardData(NULL,0,0);

	//�˿�����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_HeapCard[i].SetCardData(0,0,0);
		m_TableCard[i].SetCardData(NULL,0);
		m_DiscardCard[i].SetCardData(NULL,0);
		m_WeaveCard[i][0].SetCardData(NULL,0);
		m_WeaveCard[i][1].SetCardData(NULL,0);
		m_WeaveCard[i][2].SetCardData(NULL,0);
		m_WeaveCard[i][3].SetCardData(NULL,0);
	}

	//���ٶ�ʱ��
	KillTimer(IDI_DISC_EFFECT);
	KillTimer(IDI_BOMB_EFFECT);

	return;
}

//�����ؼ�
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	//��������
	m_ptFace[0].x=nWidth/2-m_nXFace;
	m_ptFace[0].y=nHeight/2 - 280;
	m_ptName[0].x=nWidth/2+5;
	m_ptName[0].y=nHeight/2 - 280 + 15;
	m_ptTimer[0].x=nWidth/2-m_nXFace-m_nXTimer-2;
	m_ptTimer[0].y=17+m_nYBorder;

	m_ptReady[0].x=nWidth/2-m_nXFace-m_nXTimer*2-5 + 130;
	m_ptReady[0].y=nHeight/2 - 280+m_nYTimer/2 + 40;

	m_UserFlagPos[0].x=nWidth/2-m_nXFace-m_nXTimer-32;
	m_UserFlagPos[0].y=nHeight/2 - 280;
	m_PointTrustee[0].x=nWidth/2-m_nXFace-20-m_nXFace/2;
	m_PointTrustee[0].y=nHeight/2 - 280;
	m_ptWinOrder[0].x=nWidth/2-m_nXFace/2-120;
	m_ptWinOrder[0].y=nHeight/2 - 280+18;
	SetFlowerControlInfo( 0,m_ptFace[0].x+m_nXFace/2-BIG_FACE_WIDTH,m_ptFace[0].y+m_nYFace );
	m_ptGangScore[0].x = m_ptFace[0].x+16;
	m_ptGangScore[0].y = m_ptFace[0].y+100;
	m_ScoreControl[0].SetBenchmarkPos( nWidth/2-120,nHeight/2-250 );

	m_ptFace[1].x=nWidth-m_nXBorder-m_nXFace-5;
	m_ptFace[1].y=nHeight/2-m_nYFace-50;
	m_ptName[1].x=nWidth-m_nXBorder-5;
	m_ptName[1].y=nHeight/2-45;
	m_ptTimer[1].x=nWidth-m_nXBorder-m_nXFace+5;
	m_ptTimer[1].y=nHeight/2-m_nYFace-27-m_nYTimer;
	m_ptReady[1].x=nWidth-m_nXBorder-m_nXTimer+3;
	m_ptReady[1].y=nHeight/2-m_nYFace-55-m_nYTimer/2+m_nYFace*2+40;
	m_UserFlagPos[1].x=nWidth-m_nXBorder-m_nXTimer+8;
	m_UserFlagPos[1].y=nHeight/2-m_nYFace-83-m_nYTimer;
	m_PointTrustee[1].x=nWidth-m_nXBorder-m_nXTimer+8;
	m_PointTrustee[1].y=nHeight/2-m_nYFace-50-35;
	m_ptWinOrder[1].x=nWidth-m_nXBorder-5-18;
	m_ptWinOrder[1].y=nHeight/2+m_nYFace/2-20;
	SetFlowerControlInfo( 1,m_ptFace[1].x-BIG_FACE_WIDTH,m_ptFace[1].y+m_nYFace/2-BIG_FACE_HEIGHT );
	m_ptGangScore[1].x = m_ptFace[1].x - 100;
	m_ptGangScore[1].y = m_ptFace[1].y + 16;
	m_ScoreControl[1].SetBenchmarkPos( nWidth/2+100,nHeight/2-90 );

	m_ptFace[2].x=nWidth/2-m_nXFace;
	m_ptFace[2].y=nHeight/2+350-m_nYFace;
	m_ptName[2].x=nWidth/2+5+m_nXFace/2;
	m_ptName[2].y=nHeight/2+350-m_nYFace+8;
	m_ptTimer[2].x=nWidth/2-m_nXFace/2-m_nXTimer-2;
	m_ptTimer[2].y=nHeight/2+350-m_nYTimer-8+40;

	m_ptReady[2].x=nWidth/2-m_nXTimer*2-m_nXFace/2-5 +130;
	m_ptReady[2].y=nHeight/2+350-m_nYTimer/2-10 - 40;

	m_UserFlagPos[2].x=nWidth/2-m_nXFace-m_nXTimer-32;
	m_UserFlagPos[2].y=nHeight/2+350-35;
	m_PointTrustee[2].x=nWidth/2-m_nXFace-20-m_nXFace/2;
	m_PointTrustee[2].y=nHeight/2+350-m_nYFace-5;
	m_ptWinOrder[2].x=nWidth/2-m_nXFace/2-120;
	m_ptWinOrder[2].y=nHeight/2+350-5-16;
	SetFlowerControlInfo( 2,m_ptFace[2].x+m_nXFace/2-BIG_FACE_WIDTH,m_ptFace[2].y-BIG_FACE_HEIGHT );
	m_ptGangScore[2].x = m_ptFace[2].x + 16;
	m_ptGangScore[2].y = m_ptFace[2].y - 100;
	m_ScoreControl[2].SetBenchmarkPos( nWidth/2-120,nHeight/2+70 );

	m_ptFace[3].x=m_nXBorder+5;
	m_ptFace[3].y=nHeight/2-m_nYFace-50;
	m_ptName[3].x=m_nXBorder+5;
	m_ptName[3].y=nHeight/2-45;
	m_ptTimer[3].x=m_nXBorder+22;
	m_ptTimer[3].y=nHeight/2-m_nYFace-27-m_nYTimer;
	m_ptReady[3].x=m_nXBorder-2+m_nXTimer;
	m_ptReady[3].y=nHeight/2-m_nYFace-55-m_nYTimer/2+m_nYFace*2+40;
	m_UserFlagPos[3].x=m_nXBorder+8;
	m_UserFlagPos[3].y=nHeight/2-m_nYFace-83-m_nYTimer;
	m_PointTrustee[3].x=m_nXBorder+5;
	m_PointTrustee[3].y=nHeight/2-m_nYFace-50-35;
	m_ptWinOrder[3].x=m_nXBorder+5+18;
	m_ptWinOrder[3].y=nHeight/2+m_nYFace/2-20;
	SetFlowerControlInfo( 3,m_ptFace[3].x+m_nXBorder,m_ptFace[3].y+m_nYFace/2-BIG_FACE_HEIGHT );
	m_ptGangScore[3].x = m_ptFace[3].x + 100;
	m_ptGangScore[3].y = m_ptFace[3].y + 16;
	m_ScoreControl[3].SetBenchmarkPos( nWidth/2-260,nHeight/2-90 );

	int nXCenter=nWidth/2;
	int nYCenter=nHeight/2-40;
	//�û��˿�
	m_UserCard[0].SetControlPoint(nXCenter-200 - 23,nYCenter-213 - 60 + 96);	//��
	m_UserCard[1].SetControlPoint(nXCenter+238+60 - 32 ,nHeight/2-255 + 135 - 10);//��
	m_UserCard[2].SetControlPoint(nXCenter-276-80 + 32,nHeight/2+177 + 7);//��
	m_HandCardControl.SetBenchmarkPos(nWidth/2 - 5,nHeight-m_nYFace-m_nYBorder-5 - 16,enXCenter,enYBottom);

	//�����˿�
	m_TableCard[0].SetControlPoint(nWidth/2-179 - 36,nYCenter-213 - 60 + 94);//��
	m_TableCard[1].SetControlPoint(nXCenter+238+60 - 6,nHeight/2-125 + 34);//��
	m_TableCard[2].SetControlPoint(nWidth/2+185 + 16,nHeight-m_nYFace-m_nYBorder-15 - 26);//��
	m_TableCard[3].SetControlPoint(nXCenter-276-80 + 5,nHeight/2+125);//��

	//�ѻ��˿�
	m_HeapCard[0].SetControlPoint(nXCenter-200 - 2,nYCenter-213 + 87 - 15);	//��
	m_HeapCard[1].SetControlPoint(nXCenter+238 - 52 + 5,nYCenter-140 + 89);	//��
	m_HeapCard[2].SetControlPoint(nXCenter-189 - 52 + 19,nYCenter+213-5 - 4);	//��
	m_HeapCard[3].SetControlPoint(nXCenter-276 + 35 - 5,nYCenter-135 + 84 );	//��

	//�����˿�
	m_DiscardCard[0].SetControlPoint(nXCenter-175 + 55 - 2,nYCenter-100+12 + 49);	//��
	m_DiscardCard[1].SetControlPoint(nXCenter+103 - 15,nYCenter-64+12 + 35);		//��
	m_DiscardCard[2].SetControlPoint(nXCenter+150 - 60 + 12,nYCenter+112+12 + 6);			//��
	m_DiscardCard[3].SetControlPoint(nXCenter-137 - 24 - 2,nYCenter+76+12 + 21);	//��

	//����˿�			��
	m_WeaveCard[0][0].SetControlPoint(nWidth/2+195 - 50 - 97*0,nYCenter-213 - 60 + 96 + 60);
	m_WeaveCard[0][1].SetControlPoint(nWidth/2+195 - 50 - 97*1,nYCenter-213 - 60 + 96 + 60);
	m_WeaveCard[0][2].SetControlPoint(nWidth/2+195 - 50 - 97*2,nYCenter-213 - 60 + 96 + 60);
	m_WeaveCard[0][3].SetControlPoint(nWidth/2+195 - 50 - 97*3,nYCenter-213 - 60 + 96 + 60);

	//����˿�			��
	m_WeaveCard[1][0].SetControlPoint(nXCenter+238+44 + 33 - 26 - 10*0,nHeight/2+130 - 30 + 23 - 57*0);
	m_WeaveCard[1][1].SetControlPoint(nXCenter+238+44 + 33 - 26 - 10*1,nHeight/2+130 - 30 + 23 - 57*1);
	m_WeaveCard[1][2].SetControlPoint(nXCenter+238+44 + 33 - 26 - 10*2,nHeight/2+130 - 30 + 23 - 57*2);
	m_WeaveCard[1][3].SetControlPoint(nXCenter+238+44 + 33 - 26 - 10*3,nHeight/2+130 - 30 + 23 - 57*3);

	//����˿�			��
	m_WeaveCard[2][0].SetControlPoint(nWidth/2-240 + 43 + 108*0,nHeight-m_nYFace-m_nYBorder-73 - 4);
	m_WeaveCard[2][1].SetControlPoint(nWidth/2-240 + 43 + 108*1,nHeight-m_nYFace-m_nYBorder-73 - 4);
	m_WeaveCard[2][2].SetControlPoint(nWidth/2-240 + 43 + 108*2,nHeight-m_nYFace-m_nYBorder-73 - 4);
	m_WeaveCard[2][3].SetControlPoint(nWidth/2-240 + 43 + 108*3,nHeight-m_nYFace-m_nYBorder-73 - 4);

	//����˿�		��
	m_WeaveCard[3][0].SetControlPoint(nXCenter-276-23 + 33 + 31 - 10*0,nHeight/2-210 +30 + 124 + 57*0);
	m_WeaveCard[3][1].SetControlPoint(nXCenter-276-23 + 33 + 31 - 10*1,nHeight/2-210 +30 + 124 + 57*1);
	m_WeaveCard[3][2].SetControlPoint(nXCenter-276-23 + 33 + 31 - 10*2,nHeight/2-210 +30 + 124 + 57*2);
	m_WeaveCard[3][3].SetControlPoint(nXCenter-276-23 + 33 + 31 - 10*3,nHeight/2-210 +30 + 124 + 57*3);

	//���ƴ���
	m_ControlWnd.SetBenchmarkPos(nWidth/2+298,nHeight-m_nYBorder-120);

	//ɫ�Ӵ���
	CRect rcSice;
	m_DrawSiceWnd.GetWindowRect(&rcSice);
	m_DrawSiceWnd.SetBenchmarkPos((nWidth-rcSice.Width())/2,(nHeight-rcSice.Height())/2);
	m_DrawSiceWnd.MoveWindow((nWidth-rcSice.Width())/2,(nHeight-rcSice.Height())/2,rcSice.Width(),rcSice.Height());

	//�ƶ���ť
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	m_btStart.GetWindowRect(&rcButton);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	//�ƶ�����
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nHeight-115-m_nYBorder-15,0,0,uFlags);
	//DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nHeight/2+70,0,0,uFlags);
	//�ƶ�����
	DeferWindowPos(hDwp,m_btStusteeControl,NULL,nWidth-m_nXBorder-(rcButton.Width()+5),nHeight-m_nYBorder-rcButton.Height(),0,0,uFlags);

	//�����ƶ�
	EndDeferWindowPos(hDwp);

	return;
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//�滭����
	DrawViewImage(pDC,m_ImageBack,enMode_Spread);
	DrawViewImage(pDC,m_ImageCenter,enMode_Centent);

	//��Ԫ����
	m_ImageCellScore.DrawImage( pDC,m_nXBorder+5,m_nYBorder+5 );
	TCHAR szBuffer[32] = TEXT("");
	_snprintf( szBuffer,CountArray(szBuffer),TEXT("%ld"),m_lCellScore );
	DrawTextString( pDC,szBuffer,RGB(255,255,255),RGB(0,0,0),m_nXBorder+5+56,m_nYBorder+5+12,true );

	//�滭�û�
	pDC->SetTextColor(RGB(255,255,0));
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		const tagUserData * pUserData=GetUserInfo(i);
		//test
		//tagUserData *pUserData = new tagUserData;
		//lstrcpy(pUserData->szName,TEXT("SSSSSssssssssssss"));
		//pUserData->cbUserStatus = US_READY;
		//end test
		if (pUserData!=NULL)
		{
			//�û�����
			pDC->SetTextAlign((i==1)?(TA_RIGHT|TA_TOP):(TA_LEFT|TA_TOP));
			DrawTextString(pDC,pUserData->szName,RGB(255,255,255),RGB(0,0,0),m_ptName[i].x,m_ptName[i].y,false);

			//�����Ⱥ�
			if( m_cbWinOrder[i] != 0 )
			{
				CImageHandle HandleWinOrder(&m_ImageWinOrder);
				INT nWinOrderWidth = m_ImageWinOrder.GetWidth()/4;
				INT nWinOrderHeight = m_ImageWinOrder.GetHeight();
				m_ImageWinOrder.AlphaDrawImage( pDC,m_ptWinOrder[i].x-nWinOrderWidth/2,m_ptWinOrder[i].y-nWinOrderHeight/2,
					nWinOrderWidth,nWinOrderHeight,(m_cbWinOrder[i]-1)*nWinOrderWidth,0,RGB(255,0,255) );
			}

			//������Ϣ
			WORD wUserTimer=GetUserTimer(i);
			//test
			//wUserTimer = 30;
			//end test
			if ((wUserTimer!=0)&&(m_wCurrentUser!=INVALID_CHAIR))
			{
				DrawUserTimer(pDC,nWidth/2,nHeight/2 + 24,wUserTimer);
				if(m_wCurrentUser==0)
					m_ImageArrow.DrawImage(pDC,
									nWidth/2-15,	//X����
									nHeight/2-m_ImageArrow.GetHeight()*2 + 24,	//Y����
									m_ImageArrow.GetWidth()/4,				//��
									m_ImageArrow.GetHeight(),				//��
									m_ImageArrow.GetWidth()/4*m_wCurrentUser,//��ʼλ��X
									0);
				if(m_wCurrentUser==1)
					m_ImageArrow.DrawImage(pDC,nWidth/2+m_ImageArrow.GetWidth()/4,nHeight/2-15 + 24,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*m_wCurrentUser,0);
				if(m_wCurrentUser==2)
					m_ImageArrow.DrawImage(pDC,nWidth/2-15,nHeight/2+m_ImageArrow.GetHeight() + 24,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*m_wCurrentUser,0);
				if(m_wCurrentUser==3)
					m_ImageArrow.DrawImage(pDC,nWidth/2-m_ImageArrow.GetWidth()/4*2,nHeight/2-15 + 24,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*m_wCurrentUser,0);
			}
			if((wUserTimer!=0)&&(m_wCurrentUser==INVALID_CHAIR))
			{
				DrawUserTimer(pDC,nWidth/2,nHeight/2,wUserTimer);
				if(i==0)
					m_ImageArrow.DrawImage(pDC,nWidth/2-15,nHeight/2-m_ImageArrow.GetHeight()*2,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*i,0);
				if(i==1)
					m_ImageArrow.DrawImage(pDC,nWidth/2+m_ImageArrow.GetWidth()/4,nHeight/2-15,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*i,0);
				if(i==2)
					m_ImageArrow.DrawImage(pDC,nWidth/2-15,nHeight/2+m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*i,0);
				if(i==3)
					m_ImageArrow.DrawImage(pDC,nWidth/2-m_ImageArrow.GetWidth()/4*2,nHeight/2-15,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*i,0);

			}

			if (pUserData->cbUserStatus==US_READY) 
				DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			DrawUserFace(pDC,pUserData,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);

			//�й�
			if(m_bTrustee[i])
			{
				m_ImageTrustee.DrawImage(pDC,m_PointTrustee[i].x,m_PointTrustee[i].y);
			}
		}
		//test
		//delete pUserData;
		//end test
	}

	//�û���־
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		//����λͼ
		CImageHandle ImageHandle(&m_ImageUserFlag);
		int nImageWidth=m_ImageUserFlag.GetWidth()/4;
		int nImageHeight=m_ImageUserFlag.GetHeight();

		//�滭��־
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			WORD wIndex=((i+GAME_PLAYER)-m_wBankerUser)%GAME_PLAYER;
			m_ImageUserFlag.AlphaDrawImage(pDC,m_UserFlagPos[i].x,m_UserFlagPos[i].y,nImageWidth,nImageHeight,nImageWidth*wIndex,0,RGB(255,0,255));
		}
	}
	
	//�û��˿�
	m_UserCard[0].DrawCardControl(pDC);
	m_TableCard[0].DrawCardControl(pDC);
	
	//���
	m_WeaveCard[0][0].DrawCardControl(pDC);	
	m_WeaveCard[0][1].DrawCardControl(pDC);
	m_WeaveCard[0][2].DrawCardControl(pDC);
	m_WeaveCard[0][3].DrawCardControl(pDC);

	m_HeapCard[0].DrawCardControl(pDC);	//��

	//�����˿�
	m_TableCard[1].DrawCardControl(pDC);
	m_TableCard[2].DrawCardControl(pDC);
	m_TableCard[3].DrawCardControl(pDC);
	//for (WORD i=0;i<4;i++)
	//{
	//	m_TableCard[i].DrawCardControl(pDC); //�����ϵ�
	//	//m_DiscardCard[i].DrawCardControl(pDC);	//����

	//}

	m_UserCard[1].DrawCardControl(pDC);
	//���
	m_WeaveCard[1][3].DrawCardControl(pDC);	
	m_WeaveCard[1][2].DrawCardControl(pDC);
	m_WeaveCard[1][1].DrawCardControl(pDC);
	m_WeaveCard[1][0].DrawCardControl(pDC);

	//���
	m_WeaveCard[2][0].DrawCardControl(pDC);	
	m_WeaveCard[2][1].DrawCardControl(pDC);
	m_WeaveCard[2][2].DrawCardControl(pDC);
	m_WeaveCard[2][3].DrawCardControl(pDC);
	//���
	m_WeaveCard[3][0].DrawCardControl(pDC);	
	m_WeaveCard[3][1].DrawCardControl(pDC);
	m_WeaveCard[3][2].DrawCardControl(pDC);
	m_WeaveCard[3][3].DrawCardControl(pDC);

	//������
	m_DiscardCard[0].DrawCardControl(pDC);	//����
	m_DiscardCard[1].DrawCardControl(pDC);	//����
	m_DiscardCard[3].DrawCardControl(pDC);	//����
	m_DiscardCard[2].DrawCardControl(pDC);	//����

	//�ѻ��˿�
	m_HeapCard[3].DrawCardControl(pDC);	//��
	//m_HeapCard[0].DrawCardControl(pDC);	//��
	m_HeapCard[1].DrawCardControl(pDC);	//��
	m_HeapCard[2].DrawCardControl(pDC);	//��

	//�û��˿�
	//m_UserCard[0].DrawCardControl(pDC);
	//m_UserCard[1].DrawCardControl(pDC);
	m_UserCard[2].DrawCardControl(pDC);
	m_HandCardControl.DrawCardControl(pDC);

	//������ʾ
	if (m_bOutCard==true)
	{
		CImageHandle HandleOutCard(&m_ImageOutCard);
		m_ImageOutCard.AlphaDrawImage(pDC,(nWidth-m_ImageOutCard.GetWidth())/2,nHeight-145,RGB(255,0,255));
	}

	//�ȴ���ʾ
	if (m_bWaitOther==true)
	{
		CImageHandle HandleWait(&m_ImageWait);
		m_ImageWait.AlphaDrawImage(pDC,(nWidth-m_ImageWait.GetWidth())/2,nHeight-145,RGB(255,0,255));
	}

	//�û�״̬
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if ((m_wOutCardUser==i)||(m_cbUserAction[i]!=0))
		{
			//����λ��
			int nXPos=0,nYPos=0;
			switch (i)
			{
			case 0:	//����	��
				{
					nXPos=nWidth/2 - 22;
					nYPos=nHeight/2 - 100 - 100 - 20;
					break;
				}
			case 1:	//����   ��
				{
					nXPos=nWidth/2 - 52 + 100 + 200;
					nYPos=nHeight/2 - 45;
					break;
				}
			case 2:	//����	��
				{
					nXPos=nWidth/2 - 22;
					nYPos=nHeight/2 + 100 + 100 - 50;
					break;
				}
			case 3:	//����	��
				{
					nXPos=nWidth/2 - 22 - 100 - 200;
					nYPos=nHeight/2 - 45;
					break;
				}
			}

			//�滭����
			if (m_cbUserAction[i]!=WIK_NULL)
			{	

				//�滭����
				if (m_bBombEffect==true)
				{

					//�滭Ч��
					INT nItemHeight=m_ImageActionAni.GetHeight()/6;
					INT nItemWidth=m_ImageActionAni.GetWidth()/BOMB_EFFECT_COUNT;

					//�滭����
					int nYImagePos=0;
					if (m_cbUserAction[i]&WIK_PENG) nYImagePos=nItemHeight;
					else if (m_cbUserAction[i]&WIK_GANG) nYImagePos=nItemHeight*2;
					else if (m_cbUserAction[i]==WIK_CHI_HU) nYImagePos=nItemHeight*4;
					else if (m_cbUserAction[i]&WIK_ZI_MO) nYImagePos=nItemHeight*5;
					else nYImagePos=0;
					m_ImageActionAni.DrawImage(pDC,nXPos-nItemWidth/2+54,nYPos+42-nItemHeight/2,nItemWidth,nItemHeight,
						nItemWidth*(m_cbBombFrameIndex%BOMB_EFFECT_COUNT),nYImagePos,nItemWidth,nItemHeight);
				}
			}
			else
			{	
				//��������
				CImageHandle ImageHandle(&m_ImageActionBack);
				m_ImageActionBack.BlendDrawImage(pDC,nXPos,nYPos,m_ImageActionBack.GetWidth(),m_ImageActionBack.GetHeight(),
					0,0,RGB(255,0,255),180);


				//�滭�˿�
				g_CardResource.m_ImageUserBottom.DrawCardItem(pDC,m_cbCardData,nXPos+15,nYPos+13);
			}

		}
	}

	//����Ч��
	if(m_wDiscUser!=INVALID_CHAIR)
	{
		CSize SizeDisc(m_ImageDisc.GetWidth()/DISC_EFFECT_COUNT,m_ImageDisc.GetHeight());
		CPoint pt=m_DiscardCard[m_wDiscUser].GetLastCardPosition();
		pt.Offset(-SizeDisc.cx/2,-SizeDisc.cy);
		//�滭��Ϣ
		m_ImageDisc.DrawImage(pDC,pt.x,pt.y,SizeDisc.cx,SizeDisc.cy,
			m_cbDiscFrameIndex*SizeDisc.cx,0,SizeDisc.cx,SizeDisc.cy);
	}

	//�滭�ɼ���
	for( WORD i = 0; i < GAME_PLAYER; i++ )
		m_ScoreControl[i].DrawScoreControl( pDC );

	//���Ƶ÷ֶ���
	if( m_wGangChairId != INVALID_CHAIR )
	{
		//�η����궯��
		if( m_cbXiaYu )
		{
			int nImageWidth = m_PngXiaYu.GetWidth()/4;
			int nImageHeight = m_PngXiaYu.GetHeight();
			m_PngXiaYu.DrawImage( pDC,m_ptGangScore[m_wGangChairId].x,m_ptGangScore[m_wGangChairId].y,
				nImageWidth,nImageHeight,nImageWidth*m_cbGangIndex,0,nImageWidth,nImageHeight );
		}
		else
		{
			int nImageWidth = m_PngGuaFeng.GetWidth()/4;
			int nImageHeight = m_PngGuaFeng.GetHeight();
			m_PngGuaFeng.DrawImage( pDC,m_ptGangScore[m_wGangChairId].x,m_ptGangScore[m_wGangChairId].y,
				nImageWidth,nImageHeight,nImageWidth*m_cbGangIndex,0,nImageWidth,nImageHeight );
		}
		for( WORD i = 0; i < GAME_PLAYER; i++ )
		{
			if( m_lGangScore[i] > 0 )
				DrawNumberString( pDC,m_PngNumWin,m_ptGangScore[i].x,m_ptGangScore[i].y-m_nYGangStep*3,m_lGangScore[i],true,true );
			else DrawNumberString( pDC,m_PngNumLost,m_ptGangScore[i].x,m_ptGangScore[i].y-m_nYGangStep*3,m_lGangScore[i],true,true );
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
	if (wViewChairID<GAME_PLAYER)
	{
		m_cbUserAction[wViewChairID]=bUserAction;
		SetBombEffect(true);
	}
	else 
	{
		ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
		if(m_bBombEffect)
			SetBombEffect(false);
	}

	//���½���
	UpdateGameView(NULL);

	return;
}

//���ö���
bool CGameClientView::SetBombEffect(bool bBombEffect)
{
	if (bBombEffect==true)
	{
		//���ñ���
		m_bBombEffect=true;
		m_cbBombFrameIndex=0;

		//����ʱ��
		SetTimer(IDI_BOMB_EFFECT,250,NULL);
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
//�����û�
void CGameClientView::SetDiscUser(WORD wDiscUser)
{
	if(m_wDiscUser != wDiscUser)
	{
		//���±���
		m_wDiscUser=wDiscUser;

		//���½���
		UpdateGameView(NULL);
	}
	return;
}
//��ʱ���
void CGameClientView::SetCurrentUser(WORD wCurrentUser)
{
	if (m_wCurrentUser != wCurrentUser)
	{
		//���±��� 
		m_wCurrentUser=wCurrentUser;
		
		//���½���
		UpdateGameView(NULL);
	}
	return;
}
//�����й�
void CGameClientView::SetTrustee(WORD wTrusteeUser,bool bTrustee)
{
	//У������ 
	ASSERT(wTrusteeUser>=0&&wTrusteeUser<GAME_PLAYER);

	if(m_bTrustee[wTrusteeUser] !=bTrustee)	
	{
		//��������
		m_bTrustee[wTrusteeUser]=bTrustee;

		//���½���
		UpdateGameView(NULL);
	}
	return;
}

//��������
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos, bool bDrawCenter)
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	if( bDrawCenter )
	{
		TEXTMETRIC tm;
		ZeroMemory( &tm,sizeof(tm) );
		GetTextMetrics(pDC->m_hDC,&tm );
		nXPos -= tm.tmAveCharWidth*nStringLength/2;
		nYPos -= tm.tmHeight/2;
	}

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
	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//�������
	bool bRePaint=false;
	bool bHandle=m_HandCardControl.OnEventSetCursor(MousePoint,bRePaint);

	//�ػ�����
	if (bRePaint==true)
		UpdateGameView(NULL);

	//������
	if (bHandle==false)
		__super::OnSetCursor(pWnd,nHitTest,uMessage);

	return TRUE;
}

//�����Ϣ
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);

	//��ȡ�˿�
	BYTE cbHoverCard=m_HandCardControl.GetHoverCard();
	if (cbHoverCard!=0) 
		AfxGetMainWnd()->PostMessage(IDM_OUT_CARD,cbHoverCard,cbHoverCard);

	//test
	//LONG lGangScore[GAME_PLAYER] = { -12345,-12345,-12345,12345 };
	//SetGangScore(2,0, lGangScore );
	//end test

	return;
}

//��ʼ��ť
void CGameClientView::OnStart()
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_START,0,0);

	return;
}
//�Ϲܿ���
void CGameClientView::OnStusteeControl()
{
	AfxGetMainWnd()->SendMessage(IDM_TRUSTEE_CONTROL,0,0);
	return;
}

//��ʱ��
void CGameClientView::OnTimer(UINT nIDEvent)
{
	//��������
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
	if (nIDEvent==IDI_DISC_EFFECT)
	{
		//���ñ���
		if ((m_cbDiscFrameIndex+1)>=DISC_EFFECT_COUNT)
		{
			m_cbDiscFrameIndex=0;
		}
		else m_cbDiscFrameIndex++;

		//���½���
		UpdateGameView(NULL);

		return;

	}
	else if( nIDEvent == IDI_GANG_SOCRE )
	{
		m_cbGangIndex = (m_cbGangIndex+1)%4;
		if( ++m_nYGangStep == GANG_MOVE_COUNT )
		{
			KillTimer( IDI_GANG_SOCRE );
			m_wGangChairId = INVALID_CHAIR;
			ZeroMemory( m_lGangScore,sizeof(m_lGangScore) );
		}

		UpdateGameView( NULL );
		return;
	}

	__super::OnTimer(nIDEvent);
}

//
void CGameClientView::SetWinOrder( WORD wChairId, BYTE cbWinOrder )
{
	if( wChairId == INVALID_CHAIR )
		ZeroMemory( m_cbWinOrder,sizeof(m_cbWinOrder) );
	else m_cbWinOrder[wChairId] = cbWinOrder;
	UpdateGameView( NULL );
}

//
void CGameClientView::SetGangScore( WORD wChairId, BYTE cbXiaYu, LONG lGangScore[GAME_PLAYER] )
{
	CopyMemory( m_lGangScore,lGangScore,sizeof(m_lGangScore) );
	m_wGangChairId = wChairId;
	m_cbXiaYu = cbXiaYu;
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		if( m_lGangScore[i] != 0 )
		{
			m_nYGangStep = 0;
			m_cbGangIndex = 0;
			SetTimer( IDI_GANG_SOCRE,TIME_GANG_SCORE,NULL );
			UpdateGameView( NULL );
			break;
		}
	}
}

//�滭����
void CGameClientView::DrawNumberString( CDC *pDC, CPngImage &PngNumber, int nXPos, int nYPos, LONG lNumber, bool bDrawCenter, bool bDrawSign )
{
	int nWidthNumber = PngNumber.GetWidth()/12;
	int nHeightNumber = PngNumber.GetHeight();

	int nXDrawPos = nXPos, nYDrawPos = nYPos;
	nYDrawPos -= nHeightNumber/2;

	CHAR szNum[256] = {0};
	_snprintf(szNum,CountArray(szNum),"%+ld",lNumber);

	if( bDrawCenter )
	{
		nXDrawPos -= (lstrlen(szNum)+(bDrawSign?0:-1))*nWidthNumber/2;
	}

	//������
	if( bDrawSign )
	{
		PngNumber.DrawImage( pDC,nXDrawPos,nYDrawPos,nWidthNumber,nHeightNumber,(lNumber<0?1:0)*nWidthNumber,0,
			nWidthNumber,nHeightNumber );
		nXDrawPos += nWidthNumber;
	}

	for( int i = 1; i < lstrlen(szNum); i++ )
	{
		if( ::isdigit(szNum[i]) )
		{
			PngNumber.DrawImage( pDC,nXDrawPos,nYDrawPos,nWidthNumber,nHeightNumber,(((int)szNum[i]-48)+2)*nWidthNumber,
				0,nWidthNumber,nHeightNumber );
		}
		else
		{
			break;
		}
		nXDrawPos += nWidthNumber;
	}
}

//////////////////////////////////////////////////////////////////////////
