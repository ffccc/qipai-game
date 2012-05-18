#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"
#include "CardControl.h"
#include ".\gameclientview.h"
#include "GameClient.h"
//////////////////////////////////////////////////////////////////////////
//��ť��ʶ
#define IDC_START                      100										//��ʼ��ť
#define IDC_OUT_CARD                   101										//���ư�ť
#define IDC_PASS_CARD                  102										//��ʾ��ť
#define IDC_PAY_TRIBUTE				   103										//������ť
#define IDC_CHANGE_CARD				   104										//������ť
#define IDC_RESET					   105										//����ť
#define IDC_COLOR_SORT				   106										//����ɫ��
#define IDC_BOMB_SORT				   107										//��ը����
#define IDC_AI						   108										//�йܰ�ť

#define SZ_TOU_JIA                     TEXT("��%-6s�� �ڣ�%2d�� ��ͷ��")				//����ͷ��
#define SZ_MO_YOU                      TEXT("��%-6s�� �ڣ�%2d�� ��ĩ��")				//����ĩ��
#define SZ_GAME_STATE                  TEXT("�ҷ���%s�� �� �Է���%s��")							//��Ϸ״̬
                                           
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_START,OnStart)
	ON_BN_CLICKED(IDC_OUT_CARD,OnOutCard)
	ON_BN_CLICKED(IDC_PASS_CARD,OnPassCard)
	ON_BN_CLICKED(IDC_PAY_TRIBUTE,OnPayTribute)
	ON_BN_CLICKED(IDC_CHANGE_CARD,OnChangeCard)
	ON_BN_CLICKED(IDC_RESET,OnReset)
	ON_BN_CLICKED(IDC_COLOR_SORT,OnColorSort)
	ON_BN_CLICKED(IDC_BOMB_SORT,OnBombSort)
	ON_BN_CLICKED(IDC_AI,OnAI)
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//������ʾ

//���캯��
CGameClientView::CGameClientView(void) : CGameFrameView(true,24)
{
	//����˳��
	m_bDeasilOrder=true;
    
	//״̬����
	ZeroMemory(m_bPass,sizeof(m_bPass));
	ZeroMemory(m_bCardCount,sizeof(m_bCardCount));
	ZeroMemory(m_bKanGong,sizeof(m_bKanGong));
	

	//��ʷ����
    ZeroMemory(m_lAllTurnScore,sizeof(m_lAllTurnScore));	
    ZeroMemory(m_lLastTurnScore,sizeof(m_lLastTurnScore));		
	
	//��ʼ����
	ZeroMemory(m_bGameResult,sizeof(m_bGameResult));
	ZeroMemory(m_bJinGong,sizeof(m_bJinGong));
	m_Series.bCurrentSeries = 2;
	m_Series.bOtherSeries   = 2;
	m_Series.bOurSeries     = 2;
	m_HongTao				= 0x22;
	m_bGamePlaying			= false;
	m_bFirstShow			= false;
	m_bAI					= false;

	//������Դ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_ImageUserPass.SetLoadInfo(IDB_USER_PASS,hInstance);
	m_ImageCard.SetLoadInfo(IDB_CARD,hInstance);
	m_ImageStick.SetLoadInfo(IDB_STICK,hInstance);
	m_ImageViewFill.SetLoadInfo(IDB_VIEW_FILL,hInstance);
}

//��������
CGameClientView::~CGameClientView(void)
{
}

//������Ϣ
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//���ô�С
	CRect CreateRect(0,0,0,0);

	//����������ͼ
	m_ScoreView.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,CreateRect,this,9);
	
	//�����˿�
	for (WORD i=0;i<3;i++)
	{
		m_LeaveCardControl[i].SetDisplayFlag(false);
		m_LeaveCardControl[i].SetCardSpace(15,14,0);
		m_LeaveCardControl[i].SetDirection((i==0)?true:false);
		m_LeaveCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,30+i);
	}
    
	for (WORD i=0;i<4;i++)
	{
		m_UserCardControl[i].SetDirection(true);
		m_UserCardControl[i].SetDisplayFlag(true);
		m_UserCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,20+i);
	}
	
	//������ť
	m_btStart.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_START);
	m_btOutCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_OUT_CARD);
	m_btPassCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_PASS_CARD);
	m_btPayTribute.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_PAY_TRIBUTE);
	m_btChangeCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_CHANGE_CARD);
	m_btResetCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_RESET);
	m_btColorSort.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_COLOR_SORT);
	m_btBombSort.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_BOMB_SORT);
	m_btAI.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_AI);
    
    //���ð�ť
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btStart.SetButtonImage(IDB_START,hInstance,false);
	m_btOutCard.SetButtonImage(IDB_OUT_CARD,hInstance,false);
	m_btPassCard.SetButtonImage(IDB_PASS,hInstance,false);
	m_btPayTribute.SetButtonImage(IDB_PAY_TRIBUTE,hInstance,false);
	m_btChangeCard.SetButtonImage(IDB_CHANGE_CARD,hInstance,false);
	m_btResetCard.SetButtonImage(IDB_RESET,hInstance,false);
	m_btColorSort.SetButtonImage(IDB_COLOR_SORT,hInstance,false);
	m_btBombSort.SetButtonImage(IDB_BOMB_SORT,hInstance,false);
	m_btAI.SetButtonImage(IDB_CANCELAI,hInstance,false);
	
	//�����˿�
	m_HandCardControl.SetSinkWindow(AfxGetMainWnd());
	m_HandCardControl.Create(NULL,NULL,WS_VISIBLE | WS_CHILD ,CreateRect,this,40);
	
	//������ʾ
	m_tip[0].Create(&m_btStart,TTS_ALWAYSTIP);
	m_tip[0].AddTool(&m_btStart);
	m_tip[1].Create(&m_btOutCard,TTS_ALWAYSTIP);
	m_tip[1].AddTool(&m_btOutCard);
	m_tip[2].Create(&m_btPassCard,TTS_ALWAYSTIP);
	m_tip[2].AddTool(&m_btPassCard);
	m_tip[3].Create(&m_btResetCard,TTS_ALWAYSTIP);
	m_tip[3].AddTool(&m_btResetCard);
	m_tip[4].Create(&m_btColorSort,TTS_ALWAYSTIP);
	m_tip[4].AddTool(&m_btColorSort);
	m_tip[5].Create(&m_btBombSort,TTS_ALWAYSTIP);
	m_tip[5].AddTool(&m_btBombSort);
	m_tip[6].Create(&m_btAI,TTS_ALWAYSTIP);
	m_tip[6].AddTool(&m_btAI);

#ifdef VIDEO_GAME
	CRect rcCreateRect(0,0,0,0);
	//������Ƶ
	for (WORD i=0;i<4;i++)
	{
		//������Ƶ
		m_DlgVideoService[i].Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rcCreateRect,this,200+i);
		m_DlgVideoService[i].InitVideoService(i==2,i==2);

		//������Ƶ
		g_VideoServiceManager.SetVideoServiceControl(i,&m_DlgVideoService[i]);
	}
#endif
	return 0;
}

//���ý���
void CGameClientView::ResetGameView()
{
	
	
	//״̬����
	ZeroMemory(m_bPass,sizeof(m_bPass));
	ZeroMemory(m_bCardCount,sizeof(m_bCardCount));
	ZeroMemory(m_bJinGong,sizeof(m_bJinGong));
	ZeroMemory(m_bKanGong,sizeof(m_bKanGong));

	//��ʷ����
	ZeroMemory(m_lAllTurnScore,sizeof(m_lAllTurnScore));
	ZeroMemory(m_lLastTurnScore,sizeof(m_lLastTurnScore));

	//���ؿؼ�
	m_ScoreView.ShowWindow(SW_HIDE);
	m_bFirstShow			= false;
	m_bAI					= false;
	m_btAI.SetButtonImage(IDB_CANCELAI,AfxGetInstanceHandle(),false);

	//���ذ�ť
	m_btStart.ShowWindow(SW_HIDE);
	m_btOutCard.ShowWindow(SW_HIDE);
	m_btPassCard.ShowWindow(SW_HIDE);
	m_btPayTribute.ShowWindow(SW_HIDE);
	m_btChangeCard.ShowWindow(SW_HIDE);
	m_btResetCard.ShowWindow(SW_HIDE);
	m_btColorSort.ShowWindow(SW_HIDE);
	m_btBombSort.ShowWindow(SW_HIDE);
	m_btAI.ShowWindow(SW_HIDE);

	//���ÿؼ�
	m_btOutCard.EnableWindow(FALSE);
	m_btPassCard.EnableWindow(FALSE);

	//�˿˿ؼ�
	m_HandCardControl.SetCardData(NULL,0);
	m_HandCardControl.SetPositively(false);
	m_HandCardControl.SetDisplayFlag(false);
	m_LeaveCardControl[0].SetCardData(NULL,0);
	m_LeaveCardControl[1].SetCardData(NULL,0);
	m_LeaveCardControl[2].SetCardData(NULL,0);
	for (WORD i=0;i<GAME_PLAYER;i++) m_UserCardControl[i].SetCardData(NULL,0);
}

//�����ؼ�
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{

	//�û���Ϣ
	if(m_bDeasilOrder)
	{
		m_ptFace[1].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptFace[1].y=nHeight/2-m_nYFace-55;
		m_ptName[1].x=nWidth-m_nXBorder-10;
		m_ptName[1].y=nHeight/2-50;
		m_ptTimer[1].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptTimer[1].y=nHeight/2-m_nYFace-m_nYTimer-35;
		m_ptReady[1].x=nWidth-m_nXBorder-m_nXTimer-35;
		m_ptReady[1].y=nHeight/2-m_nYFace-m_nYTimer/2-15;
		m_PointUserPass[1].x=nWidth-m_nXBorder-m_nXFace-200;
		m_PointUserPass[1].y=nHeight/2-m_nYFace+5;
		m_PointGameResult[1].x=nWidth-m_nXBorder-10;
	    m_PointGameResult[1].y=m_ptName[1].y+45;

		m_ptFace[3].x=m_nXBorder+5;
		m_ptFace[3].y=nHeight/2-m_nYFace-55;
		m_ptName[3].x=m_nXBorder+10;
		m_ptName[3].y=nHeight/2-50;
		m_ptTimer[3].x=m_nXBorder+m_nXFace+5;
		m_ptTimer[3].y=nHeight/2-m_nYFace-m_nYTimer-35;
		m_ptReady[3].x=m_nXBorder+35+m_nXTimer;
		m_ptReady[3].y=nHeight/2-m_nYFace-m_nYTimer/2-15;
		m_PointUserPass[3].x=m_nXBorder+m_nXFace+135;
		m_PointUserPass[3].y=nHeight/2-m_nYFace+5;
		m_PointGameResult[3].x=m_nXBorder+5;
	    m_PointGameResult[3].y=m_ptName[3].y+45;
	}
	else
	{
		m_ptFace[3].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptFace[3].y=nHeight/2-m_nYFace-55;
		m_ptName[3].x=nWidth-m_nXBorder-10;
		m_ptName[3].y=nHeight/2-50;
		m_ptTimer[3].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptTimer[3].y=nHeight/2-m_nYFace-m_nYTimer-35;
		m_ptReady[3].x=nWidth-m_nXBorder-m_nXTimer-35;
		m_ptReady[3].y=nHeight/2-m_nYFace-m_nYTimer/2-15;
		m_PointUserPass[3].x=nWidth-m_nXBorder-m_nXFace-200;
		m_PointUserPass[3].y=nHeight/2-m_nYFace+5;
		m_PointGameResult[3].x=nWidth-m_nXBorder-10;
	    m_PointGameResult[3].y=m_ptName[3].y+45;

		m_ptFace[1].x=m_nXBorder+5;
		m_ptFace[1].y=nHeight/2-m_nYFace-55;
		m_ptName[1].x=m_nXBorder+10;
		m_ptName[1].y=nHeight/2-50;
		m_ptTimer[1].x=m_nXBorder+m_nXFace+5;
		m_ptTimer[1].y=nHeight/2-m_nYFace-m_nYTimer-35;
		m_ptReady[1].x=m_nXBorder+35+m_nXTimer;
		m_ptReady[1].y=nHeight/2-m_nYFace-m_nYTimer/2-15;
		m_PointUserPass[1].x=m_nXBorder+m_nXFace+135;
		m_PointUserPass[1].y=nHeight/2-m_nYFace+5;
		m_PointGameResult[1].x=m_nXBorder+5;
	    m_PointGameResult[1].y=m_ptName[1].y+45;
	}

	m_ptFace[0].x=nWidth/2-m_nXFace/2;
	m_ptFace[0].y=m_nYBorder+5;
	m_ptName[0].x=nWidth/2+5+m_nXFace/2;
	m_ptName[0].y=m_nYBorder+2;
	m_ptTimer[0].x=nWidth/2-m_nXFace/2-m_nXTimer*2-25;
	m_ptTimer[0].y=m_nYBorder+30;
	m_ptReady[0].x=nWidth/2-m_nXTimer-m_nXFace/2-5;
	m_ptReady[0].y=m_nYBorder+m_nYTimer/2+10;
	m_PointUserPass[0].x=nWidth/2-21;
	m_PointUserPass[0].y=nHeight/2-140;
	m_PointGameResult[0].x=m_ptName[0].x+50;
	m_PointGameResult[0].y=m_nYBorder+20;

	m_ptFace[2].x=nWidth/2-m_nXFace/2;
	m_ptFace[2].y=nHeight-m_nYBorder-m_nYFace-5;
	m_ptName[2].x=nWidth/2+5+m_nXFace/2;
	m_ptName[2].y=nHeight-m_nYBorder-m_nYFace-2;
	m_ptTimer[2].x=nWidth/2-m_nXFace/2-m_nXTimer*2-25;
	m_ptTimer[2].y=nHeight-m_nYBorder-m_nYTimer+10;
	m_ptReady[2].x=nWidth/2-m_nXTimer-m_nXFace/2-5;
	m_ptReady[2].y=nHeight-m_nYBorder-m_nYTimer/2-10;
	m_PointUserPass[2].x=nWidth/2-21;
	m_PointUserPass[2].y=nHeight-m_nYBorder-m_nYFace-210;
	m_PointGameResult[2].x=m_ptName[2].x+50;
	m_PointGameResult[2].y=m_ptName[2].y+18;
	
	for(BYTE i=0;i<4;i++)
	{
		m_PointStick[i].x=m_ptReady[i].x;
		if(i%2==0)
			m_PointStick[i].x-=(m_nXTimer+15);
		m_PointStick[i].y=m_ptReady[i].y;
		if(i%2==1)
			m_PointStick[i].y-=(m_nYTimer+15);
    }

	//�ƶ���ť
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(11);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	m_btStart.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nHeight-193-m_nYBorder,0,0,uFlags);

	m_btOutCard.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btOutCard,	NULL,nWidth/2-rcButton.Width()*3/2-10,	nHeight-193-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btPayTribute,	NULL,nWidth/2-rcButton.Width()/2,		nHeight-193-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btChangeCard,	NULL,nWidth/2-rcButton.Width()/2,		nHeight-193-m_nYBorder,0,0,uFlags);
    DeferWindowPos(hDwp,m_btPassCard,	NULL,nWidth/2+rcButton.Width()/2+10,	nHeight-193-m_nYBorder,0,0,uFlags);
	
    
	m_btResetCard.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btResetCard,	NULL,nWidth-3*rcButton.Width()-20-m_nXBorder,			nHeight*15/16,		   0,0,uFlags);
	DeferWindowPos(hDwp,m_btColorSort,	NULL,nWidth-2*rcButton.Width()-10-m_nXBorder,			nHeight*15/16,		   0,0,uFlags);
	DeferWindowPos(hDwp,m_btAI,			NULL,nWidth-rcButton.Width()-m_nXBorder,				nHeight*15/16,		   0,0,uFlags);
	EndDeferWindowPos(hDwp);

	//�ƶ��˿�
	m_HandCardControl.SetBenchmarkPos(nWidth/2,nHeight-m_nYFace-m_nYBorder-10,enXCenter,enYBottom);
    m_UserCardControl[0].SetBenchmarkPos(nWidth/2,m_nYBorder+m_nYFace+m_nYBorder+50,enXCenter,enYTop);
	m_UserCardControl[2].SetBenchmarkPos(nWidth/2,nHeight-198-m_nYBorder,enXCenter,enYBottom);
	m_LeaveCardControl[0].SetBenchmarkPos(nWidth/2,m_nXBorder+m_nYFace+m_nYBorder+10,enXCenter,enYTop);

	//�û��˿�
	if (m_bDeasilOrder==true)
	{
		m_UserCardControl[1].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-60,nHeight/2-30,enXRight,enYCenter);
		m_UserCardControl[3].SetBenchmarkPos(m_nXBorder+m_nXFace+60,nHeight/2-30,enXLeft,enYCenter);
		m_LeaveCardControl[1].SetBenchmarkPos(nWidth-m_nXBorder-m_nXFace-50,nHeight/2-50,enXRight,enYCenter);
		m_LeaveCardControl[2].SetBenchmarkPos(m_nXBorder+m_nXFace+50,nHeight/2-50,enXLeft,enYCenter);
	}
	else 
	{
		m_UserCardControl[3].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-60,nHeight/2-30,enXRight,enYCenter);
		m_UserCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+60,nHeight/2-30,enXLeft,enYCenter);
		m_LeaveCardControl[2].SetBenchmarkPos(nWidth-m_nXBorder-m_nXFace-50,nHeight/2-50,enXRight,enYCenter);
		m_LeaveCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+50,nHeight/2-50,enXLeft,enYCenter);
	}
	
	//������ͼ
	CRect rcControl;
	m_ScoreView.GetWindowRect(&rcControl);
	m_ScoreView.SetWindowPos(NULL,(nWidth-rcControl.Width())/2,nHeight/2-130,0,0,SWP_NOZORDER|SWP_NOSIZE);
	
		//��Ƶλ������
#ifdef VIDEO_GAME
	RectifyVideoView(nWidth,nHeight);
#endif
	return;
}

#ifdef VIDEO_GAME
//������Ƶλ��
void CGameClientView::RectifyVideoView(int nWidth, int nHeight)
{
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	//��Ƶ����
	CRect rcAVDlg;
	m_DlgVideoService[0].GetWindowRect(&rcAVDlg);
	if( m_bDeasilOrder==false)
	{
		DeferWindowPos(hDwp,m_DlgVideoService[3],NULL,nWidth-m_nXBorder-5-rcAVDlg.Width(),nHeight/2-10,0,0,uFlags);
		DeferWindowPos(hDwp,m_DlgVideoService[1],NULL,m_nXBorder+5,nHeight/2-10,0,0,uFlags);
	}else
	{
		DeferWindowPos(hDwp,m_DlgVideoService[1],NULL,nWidth-m_nXBorder-5-rcAVDlg.Width(),nHeight/2-10,0,0,uFlags);
		DeferWindowPos(hDwp,m_DlgVideoService[3],NULL,m_nXBorder+5,nHeight/2-10,0,0,uFlags);
	}
	DeferWindowPos(hDwp,m_DlgVideoService[0],NULL,nWidth-rcAVDlg.Width()-5,5,0,0,uFlags);
	m_DlgVideoService[2].GetWindowRect(&rcAVDlg);
	DeferWindowPos(hDwp,m_DlgVideoService[2],NULL,m_nXBorder+5,nHeight-m_nYBorder-103-rcAVDlg.Height(),10,0,uFlags);
	EndDeferWindowPos(hDwp);
}

#endif
//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//�滭����
	DrawViewImage(pDC,m_ImageViewFill,enMode_Spread);
	DrawViewImage(pDC,m_ImageBack,enMode_Centent);
    
	//�滭�û�
	TCHAR    szBuffer[50];
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wUserTimer=GetUserTimer(i);
		const tagUserData * pUserData=GetUserInfo(i);

		//////////////////////////////////////////////////////////

		////test
		//WORD wUserTimer = 30;
		//tagUserData *pUserData = new tagUserData;
		//lstrcpy(pUserData->szName,TEXT("�������"));
		//pUserData->lScore = 10000L;
		//pUserData->cbUserStatus = US_READY;

		//////////////////////////////////////////////////////////
		CImageHandle ImageHnadleUserPass(&m_ImageUserPass);
		//�滭�û�
		if (pUserData!=NULL)
		{
			//�û�����
			pDC->SetTextColor((wUserTimer>0)?RGB(250,250,250):RGB(220,220,220));
			pDC->SetTextAlign(((i==1)&&(m_bDeasilOrder==true))||((i==3)&&(m_bDeasilOrder==false))?TA_RIGHT:TA_LEFT);
			pDC->TextOut(m_ptName[i].x,m_ptName[i].y,pUserData->szName,lstrlen(pUserData->szName));

			
		    //ʣ���˿�
			CFont font,*oldfont;
			oldfont=pDC->GetCurrentFont();
			font.CreatePointFont(120,"����");
			pDC->SelectObject(font);
			pDC->SetTextColor(RGB(220,220,0));
			_sntprintf(szBuffer,sizeof(szBuffer),TEXT("[%ld ��]"),m_bCardCount[i]);
			pDC->TextOut(m_ptName[i].x,m_ptName[i].y+18,szBuffer,lstrlen(szBuffer));
			pDC->SelectObject(oldfont);

			//������Ϣ
			if (wUserTimer!=0) DrawUserTimer(pDC,m_ptTimer[i].x,m_ptTimer[i].y,wUserTimer);
			if (pUserData->cbUserStatus==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			DrawUserFace(pDC,pUserData,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);

        }
		//������־
		if (m_bPass[i]==true) 
		{
			m_ImageUserPass.AlphaDrawImage(pDC,m_PointUserPass[i].x,m_PointUserPass[i].y,RGB(255,0,255));
		}
	}
    
	if(m_bGamePlaying)
	{
		for(WORD i=0;i<GAME_PLAYER;i++)
		{
			if(m_bKanGong[i])
				DrawStick(pDC,m_PointStick[i]);
		}
		
		//��������
		CFont font,*oldfont;
		oldfont=pDC->GetCurrentFont();
		font.CreatePointFont(120,"����");
		pDC->SelectObject(font);
	
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(220,220,220));
		pDC->SetTextAlign(TA_LEFT);
	    
		//��ʾͷ�Σ�ĩ��
		BYTE     bCount=1;
		for(WORD i=0;i<GAME_PLAYER;i++)
		{
			const tagUserData * pUserData=GetUserInfo(i);
			if(pUserData!=NULL)
			{
				if(m_bGameResult[i]>0)
				{   
					bCount++;
					_sntprintf(szBuffer,sizeof(szBuffer),SZ_TOU_JIA,pUserData->szName,m_bGameResult[i]);
					 pDC->TextOut(20,m_nYBorder+14*bCount,szBuffer,lstrlen(szBuffer));
					 break;
				}
			}
		}
		for(WORD i=0;i<GAME_PLAYER;i++)
		{
			const tagUserData * pUserData=GetUserInfo(i);
			if(pUserData!=NULL)
			{
				if(m_bGameResult[i]<0) 
				{  
					bCount++;
					_sntprintf(szBuffer,sizeof(szBuffer),SZ_MO_YOU,pUserData->szName,m_bGameResult[i]*(-1));
					pDC->TextOut(20,m_nYBorder+14*bCount,szBuffer,lstrlen(szBuffer));

				}
			}
		}
		
		TCHAR * m_bShowSeries[13]={"2","3","4","5","6","7",
								 "8","9","10","J","Q","K","A"};
		//��ʾ��Ϸ����
	//	_sntprintf(szBuffer,sizeof(szBuffer),_T("�۱��֣�"));
    //	pDC->TextOut(20,581+m_nYBorder,szBuffer,lstrlen(szBuffer));
	//	_sntprintf(szBuffer,sizeof(szBuffer),_T("�۱��֣�"));
	//	pDC->TextOut(124,411+m_nYBorder,szBuffer,lstrlen(szBuffer));
		pDC->SetTextColor(RGB(220,220,0));
		if(m_Series.bCurrentSeries==m_Series.bOurSeries)
			_sntprintf(szBuffer,sizeof(szBuffer),SZ_GAME_STATE,
					m_bShowSeries[m_Series.bOurSeries-2],m_bShowSeries[m_Series.bOtherSeries-2]);
		else
			_sntprintf(szBuffer,sizeof(szBuffer),SZ_GAME_STATE,
					m_bShowSeries[m_Series.bOtherSeries-2],m_bShowSeries[m_Series.bOurSeries-2]);
        
		pDC->TextOut(40,11+m_nYBorder,szBuffer,lstrlen(szBuffer));
		//������Դ
		pDC->SelectObject(oldfont);
		font.DeleteObject();

    //����������
	//����λͼ
	CImageHandle HandleCard(&m_ImageCard);
	
	
	DWORD dwXImagePos,dwYImagePos;
	DWORD cx,cy;
	cx=m_ImageCard.GetWidth()/13;
	cy=m_ImageCard.GetHeight()/5;


	//����λͼ
	CBitmap BufferImage;
	BufferImage.CreateCompatibleBitmap(pDC,cx,cy);

	//���� DC
	CDC BufferDC;
	BufferDC.CreateCompatibleDC(pDC);
	BufferDC.SelectObject(&BufferImage);
    
	dwXImagePos=((m_HongTao&0x0f)-1)*cx;
	dwYImagePos=2*cy;
	m_ImageCard.BitBlt(BufferDC,0,0,cx,cy,dwXImagePos,dwYImagePos);
    
	pDC->BitBlt(m_nXBorder+10,nHeight-m_nYBorder-cy-10,cx,cy,&BufferDC,0,0,SRCCOPY);



	//��ȡ�û���Ϣ
	if(m_bFirstShow)
	{
		const tagUserData * pUserData=GetUserInfo(m_wFirstID);
		if(pUserData!=NULL)
		{
			//��ȡ���ڴ�С
			CRect rcClient;
			GetClientRect(&rcClient);
			//�޸�����
			CFont font;
			font.CreatePointFont(150,"����");
			pDC->SelectObject(font);
			pDC->SetTextColor(RGB(220,220,220));
			if(m_bLiangCard!=0)
			{
				TCHAR * m_bCard[15]={"A","2","3","4","5","6","7",
								 "8","9","10","J","Q","K","С��","����"};
				BYTE bIndex;
			
				if((m_bLiangCard&0xf0)==0x40)
					bIndex=(m_bLiangCard&0x0f+12);
				else
					bIndex=(m_bLiangCard&0x0f-1);

				_sntprintf(szBuffer,sizeof(szBuffer),_T("����Ϊ %-s , %-s �õ�����,��һ������"),m_bCard[bIndex],
											pUserData->szName);
				pDC->TextOut(rcClient.Width()/2-140,rcClient.Height()/2-20,szBuffer,lstrlen(szBuffer));
			
			}
			else
			{
				_sntprintf(szBuffer,sizeof(szBuffer),_T("%-s ��һ������"),pUserData->szName);
				pDC->TextOut(rcClient.Width()/2-60,rcClient.Height()/2-20,szBuffer,lstrlen(szBuffer));
			}
		}
	}

	//��ʾ������
	bCount=0;
	//��ȡ���ڴ�С
	CRect rcClient;
	GetClientRect(&rcClient);
	//�޸�����
	CFont nfont;
	nfont.CreatePointFont(150,"����");
	pDC->SelectObject(nfont);
	pDC->SetTextColor(RGB(220,220,220));
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		const tagUserData * pUserData=GetUserInfo(i);
		if(m_bJinGong[i])
		{	
			bCount++;
			_sntprintf(szBuffer,sizeof(szBuffer),_T("Ӧ�ع� %-s һ����"),pUserData->szName);
			pDC->TextOut(rcClient.Width()/2-60,rcClient.Height()/2-20+bCount*17,szBuffer,lstrlen(szBuffer));
		}
	}

	}
}


//��ʼ��ť
void CGameClientView::OnStart()
{
	AfxGetMainWnd()->SendMessage(IDM_START,0,0);
}

//���ư�ť
void CGameClientView::OnOutCard()
{
	AfxGetMainWnd()->SendMessage(IDM_OUT_CARD,1,1);
} 

//������ť
void CGameClientView::OnPassCard()
{
	AfxGetMainWnd()->SendMessage(IDM_PASS_CARD,1,1);
}

//������ť
void CGameClientView::OnPayTribute()
{
	AfxGetMainWnd()->SendMessage(IDM_PAY_TRIBUTE,0,0);
}

//������ť
void CGameClientView::OnChangeCard()
{
  AfxGetMainWnd()->SendMessage(IDM_PAY_TRIBUTE,0,0);
}

//��������
void CGameClientView::OnReset()
{
	AfxGetMainWnd()->SendMessage(IDM_SORT_CARD,0,0);
}

//����ɫ��
void CGameClientView::OnColorSort()
{
	AfxGetMainWnd()->SendMessage(IDM_SORT_CARD,0,1);
}

//��ը����
void CGameClientView::OnBombSort()
{
	AfxGetMainWnd()->SendMessage(IDM_SORT_CARD,0,2);
}
//����һ�
void CGameClientView::OnRButtonUp(UINT nFlags, CPoint point)
{
	CGameFrameView::OnRButtonUp(nFlags, point);
	m_HandCardControl.ShootAllCard(false);
	if(m_btOutCard.IsWindowVisible())	
		m_btOutCard.EnableWindow(false);
	if(m_btPayTribute.IsWindowVisible())
		m_btPayTribute.EnableWindow(false);
	if(m_btChangeCard.IsWindowVisible())
		m_btChangeCard.EnableWindow(false);
}
//����й�
void CGameClientView::OnAI()
{
	if(!m_bAI)
		m_btAI.SetButtonImage(IDB_AI,AfxGetInstanceHandle(),false);
	else
		m_btAI.SetButtonImage(IDB_CANCELAI,AfxGetInstanceHandle(),false);
	m_bAI=!m_bAI;
	AfxGetMainWnd()->SendMessage(IDM_AI,0,0);
}
//����˳��
void CGameClientView::SetUserOrder(bool bDeasilOrder)
{
	//״̬�ж�
	if(m_bDeasilOrder==bDeasilOrder) return ;
	
	//�޸�˳��
    m_bDeasilOrder=bDeasilOrder;
	AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),m_bDeasilOrder?TRUE:FALSE);

	//���ý���
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyGameView(rcClient.Width(),rcClient.Height());

	//ˢ�½���
	UpdateGameView(NULL);

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

//������Ϸ״̬
void CGameClientView::SetGamePlay(bool bPlaying)
{
	m_bGamePlaying=bPlaying;
}

BOOL CGameClientView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	for(BYTE i=0;i<7;i++)
	{
		if(m_tip[i].m_hWnd!=NULL)
			m_tip[i].RelayEvent(pMsg);
	}

	return CGameFrameView::PreTranslateMessage(pMsg);
}


void CGameClientView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_tip[0].UpdateTipText(_T("��Ϸ��ʼ"),&m_btStart);
	m_tip[1].UpdateTipText(_T("����"),	&m_btOutCard);
	m_tip[2].UpdateTipText(_T("����"),	&m_btPassCard);
	m_tip[3].UpdateTipText(_T("�����˿�"),&m_btResetCard);
	m_tip[4].UpdateTipText(_T("����ɫ��"),&m_btColorSort);
	m_tip[5].UpdateTipText(_T("��ը����"),&m_btBombSort);
	if(m_bAI)
		m_tip[6].UpdateTipText(_T("ȡ���й�"),&m_btAI);
	else
		m_tip[6].UpdateTipText(_T("�й�"),&m_btAI);

	CGameFrameView::OnMouseMove(nFlags, point);
	
}

//��ʾ��һ�����
void CGameClientView::SetFirstUser(WORD wChairID,bool bShow,BYTE bCard)
{
	m_wFirstID=wChairID;
	m_bFirstShow=bShow;
	m_bLiangCard=bCard;
	//���½���
	UpdateGameView(NULL);

	
}


//��ʾ������
void CGameClientView::ShowJinGong(WORD wChairID,bool bShow)
{
	if(wChairID==INVALID_CHAIR)
		ZeroMemory(m_bJinGong,sizeof(m_bJinGong));
	else
		m_bJinGong[wChairID]=bShow;

	//���½���
	UpdateGameView(NULL);
}

// ������ͼ
void CGameClientView::DrawStick(CDC * pDC, POINT x)
{
	CImageHandle ImageHnadleStick(&m_ImageStick);
	m_ImageStick.AlphaDrawImage(pDC,x.x,x.y,RGB(255,255,255));
}
