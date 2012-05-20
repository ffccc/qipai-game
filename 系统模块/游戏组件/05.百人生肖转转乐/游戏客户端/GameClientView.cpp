#include "StdAfx.h"
#include "Math.h"
#include "Resource.h"
#include "GameClientView.h"
#include ".\gameclientview.h"
#include "GameClientDlg.h"

//110,140,692,554

#define BET_AREA_LEFT				110
#define BET_AREA_TOP				135
#define BET_AREA_RIGHT				692
#define BET_AREA_BOTTOM				554
#define BET_AREA_WIDTH				( BET_AREA_RIGHT - BET_AREA_LEFT )
#define BET_AREA_HEIGH				( BET_AREA_BOTTOM - BET_AREA_TOP )

#define OFFSET_X		120
#define OFFSET_Y		150

#define TOTAL_WIDTH		586
#define TOTAL_HEIGHT	450

#define AREA_WIDTH		141
#define AREA_HEIGHT		108


//115 , 147
static	CRect rcLocalArea[MAX_AREA];
static	CRect rcArea[MAX_AREA];


CGameClientDlg * g_pGameClient;



//////////////////////////////////////////////////////////////////////////


//按钮标识
#define IDC_JETTON_BUTTON_100		200									//按钮标识
#define IDC_JETTON_BUTTON_1K		201									//按钮标识
#define IDC_JETTON_BUTTON_1W		202									//按钮标识
#define IDC_JETTON_BUTTON_10W		203									//按钮标识
#define IDC_JETTON_BUTTON_100W		204									//按钮标识
#define IDC_JETTON_BUTTON_500W  	205									//按钮标识
#define IDC_APPY_BANKER				206									//按钮标识
#define IDC_CANCEL_BANKER			207									//按钮标识
#define IDC_SCORE_MOVE_L			209									//按钮标识
#define IDC_SCORE_MOVE_R			210									//按钮标识
#define IDC_SAVE_SCORE              211                                 //按钮标识
#define IDC_FETCH_SCORE             212                                 //按钮标识
#define IDC_HISTORY                 213                                 //按钮标识
#define IDC_BANK_IN					214
#define IDC_BANK_OUT				215

#define IDC_PREVIOUS				216
#define IDC_NEXT					217	

#define IDC_BANKER_UP				218
#define IDC_BANKER_DOWN				219

//庄家信息
#define BANKER_INFO_LEN				150									//庄家信息


#define SETRECT(rc,l,t,w,h) \
	rc.left = l; \
    rc.top = t; \
	rc.right = rc.left + w; \
	rc.bottom = rc.top + h

#define    TIME_INTERVAL		80
#define	   INVALID_AREA			((DWORD)(-1))


//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_JETTON_BUTTON_100, OnJettonButton100)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_1K, OnJettonButton1K)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_1W, OnJettonButton1W)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_10W, OnJettonButton10W)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_100W, OnJettonButton100W)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_500W, OnJettonButton500W)
	ON_BN_CLICKED(IDC_APPY_BANKER, OnApplyBanker)
	ON_BN_CLICKED(IDC_CANCEL_BANKER, OnCancelBanker)

	ON_BN_CLICKED(IDC_BANK_IN, OnBankIn)
	ON_BN_CLICKED(IDC_BANK_OUT,OnBankOut)

	ON_BN_CLICKED(IDC_PREVIOUS,OnPrevious)
	ON_BN_CLICKED(IDC_NEXT,OnNext)

	ON_BN_CLICKED(IDC_BANKER_UP,OnBankUp)
	ON_BN_CLICKED(IDC_BANKER_DOWN,OnBankDown)

	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////



void CGameClientView::DrawChipNum(CDC * pDC, Graphics *pGraphics, int x, int y, INT64 num, int numtype, int nAlign/*=1*/)
{

	INT64 temp = num>=0?num:-num;

	TCHAR ch[50]={0};
	sprintf(ch,"%I64d",temp);

	CSkinImageEx *pImage = NULL;
	switch( numtype  )
	{
		case 0: pImage = &m_ImgChipNum;		break;
		case 1: pImage = &m_ImgChipNum1;	break;
		case 2: pImage = &m_ImgChipNum2;	break;
		default: return ;
	}

	const	int 	Width = pImage->GetWidth()/11;
	const	int		Height= pImage->GetHeight();

	if( nAlign == 1 )	x -= ( strlen(ch)*Width - Width ) /2;

	if( num<0 )
		m_ImageFuhao.Draw( pDC->m_hDC , x - m_ImageFuhao.GetWidth() - 1 , y + 2  );

	int pos;
	for (int i=0;i<strlen(ch);i++)
	{
		pos = ch[i]-48;
		pImage->Draw( pDC->m_hDC ,x + Width*i ,y ,Width,Height,pos*Width,0,Width,Height);
	}

}


//构造函数
CGameClientView::CGameClientView() : CGameFrameView(true,24)
{
	//状态信息
	m_lCurrentJetton=0L;

	m_bShowGameResult = false;
	m_lMeCurGameScore=0L;
	m_lBankerCurGameScore=0L;		
	m_lTempCurGameScore=0;
	//下注信息
	m_lMeMaxScore=0L;
	ZeroMemory(m_nTotalUserBet,sizeof(m_nTotalUserBet));
	ZeroMemory(m_nUserBet,sizeof(m_nUserBet));

	//庄家信息
	m_bShowChangeBanker = false;
	m_wCurrentBankerChairID = INVALID_CHAIR;
	m_cbBankerTime = 0;			
	m_lBankerScore = 0;		
	m_lBankerTreasure=0;
	m_bFlashWinner=0;
	m_cbTimeCount=0;
	m_bDisPlayEnd = FALSE;
	m_wCursors = 0;
	m_nValidMoney = 0;
	m_wBankerCursors = 0;
	m_lBankerTotal = 0;

//////////////////////////////////////////////////////////////////////////

	//加载位图
	HINSTANCE hInstance=AfxGetInstanceHandle();
	for (int i=0;i<12;i++)
	{
		m_ImageAnimal[i].LoadFromResource( hInstance ,2001+i , TEXT("PNG") ,TRUE );
	}
	m_dwCurrentAniPos=0;
	m_dwCursorArea=INVALID_AREA;
	//m_ImageViewFill.LoadFromResource(hInstance,IDR_VIEW_FILL,TEXT("PNG"),FALSE);
	//m_ImageTimerBack.LoadFromResource(hInstance, IDR_TIMER_BACK,TEXT("PNG"),FALSE);
	m_ImageTimerNumber.LoadFromResource(hInstance,IDR_TIMER_NUMBER,TEXT("PNG"),true);
	m_ImageGameEnd.LoadFromResource(hInstance, IDR_GAME_END,TEXT("PNG"),true);
	m_ImageViewBack.LoadFromResource(hInstance, IDR_VIEW_BACK,TEXT("JPG"),FALSE);
	m_ImageBackBuf.LoadFromResource(hInstance, IDR_VIEW_BACK,TEXT("JPG"),FALSE);
	m_ImageJettonView.LoadFromResource(hInstance,IDR_JETTOM_VIEW,TEXT("PNG"),true);
	m_ImgChipNum.LoadFromResource(hInstance,TEXT("IDB_CHIP_NUM") ,TEXT("PNG"),true );
	m_ImgChipNum1.LoadFromResource(hInstance,TEXT("IDB_CHIP_NUM_1") ,TEXT("PNG"),true );
	m_ImgChipNum2.LoadFromResource(hInstance,TEXT("IDB_CHIP_NUM_2") ,TEXT("PNG"),true );
	m_ImageFuhao.LoadFromResource(hInstance,TEXT("IDB_HUHAO") ,TEXT("PNG"),true );
	m_ImageText.LoadFromResource(hInstance,TEXT("IDB_TEXT"),TEXT("PNG"),true);
	m_ImageBG.LoadFromResource(hInstance,TEXT("IDB_BG"),TEXT("JPG"),FALSE);
	m_TimerText.LoadFromResource(hInstance,TEXT("IDB_TIMER_TEXT"),TEXT("PNG"),TRUE);

	ZeroMemory(m_hCursorArray, sizeof(m_hCursorArray));

	const	int x = OFFSET_X;
	const	int y = OFFSET_Y;

	for ( int i=0;i<MAX_AREA;i++ )
	{
		if(i<4)
		{
			rcLocalArea[i].left = x + AREA_WIDTH*i;		
			rcLocalArea[i].top  = y;
		}

		if(i>=4&&i<8 )
		{
			rcLocalArea[i].left = x + AREA_WIDTH*(i%4);		
			rcLocalArea[i].top  = y + AREA_HEIGHT;
		}

		if(i>=8 )
		{
			rcLocalArea[i].left = x + AREA_WIDTH*(i%4);		
			rcLocalArea[i].top  = y + AREA_HEIGHT*2;
		}

		rcLocalArea[i].right = rcLocalArea[i].left + AREA_WIDTH;
		rcLocalArea[i].bottom = rcLocalArea[i].top + AREA_HEIGHT;
		if( i==12 )
		{
			rcLocalArea[i].left = x ;		
			rcLocalArea[i].top  = y + AREA_HEIGHT*3;

			rcLocalArea[i].right = rcLocalArea[i].left + AREA_WIDTH*2;
			rcLocalArea[i].bottom = rcLocalArea[i].top + AREA_HEIGHT - 34;
		}

		if( i==13 )
		{
			rcLocalArea[i].left = x + AREA_WIDTH*2;		
			rcLocalArea[i].top  = y + AREA_HEIGHT*3;

			rcLocalArea[i].right = rcLocalArea[i].left + AREA_WIDTH*2;
			rcLocalArea[i].bottom = rcLocalArea[i].top + AREA_HEIGHT - 34;
		}
	}

//////////////////////////////////////////////////////////////////////////

}

//析构函数

CGameClientView::~CGameClientView(void)
{
	for (int i = 0; i < JETTON_COUNT; i++)
	{
		DestroyCursor(m_hCursorArray[i]);
	}
}

//建立消息
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//创建控件
	CRect rcCreate(0,0,0,0);

	//m_btScoreMoveL.Create("",WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rcCreate,this,IDC_SCORE_MOVE_L);
	//m_btScoreMoveR.Create("",WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rcCreate,this, IDC_SCORE_MOVE_R);
	//m_btSaveScroe.Create("",WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rcCreate,this,IDC_SAVE_SCORE);
	//m_btFetchScore.Create("",WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rcCreate,this,IDC_FETCH_SCORE);
	m_btApplyBanker.Create("",WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rcCreate,this,IDC_APPY_BANKER);
	m_btCancelBanker.Create("",WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rcCreate,this,IDC_CANCEL_BANKER);
	m_btJetton100.Create("",WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rcCreate,this,IDC_JETTON_BUTTON_100);
	m_btJetton1K.Create("",WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rcCreate,this,IDC_JETTON_BUTTON_1K);
	m_btJetton1W.Create("",WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rcCreate,this,IDC_JETTON_BUTTON_1W);
	m_btJetton10W.Create("",WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rcCreate,this,IDC_JETTON_BUTTON_10W);
	m_btJetton100W.Create("",WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rcCreate,this,IDC_JETTON_BUTTON_100W);
	m_btJetton1000W.Create("",WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rcCreate,this,IDC_JETTON_BUTTON_500W);
	m_btBankerIn.Create("",WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rcCreate,this,IDC_BANK_IN);
	m_btBankerOut.Create("",WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rcCreate,this,IDC_BANK_OUT);
	m_btPrevious.Create("",WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rcCreate,this,IDC_PREVIOUS);
	m_btNext.Create("",WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rcCreate,this,IDC_NEXT);
	m_btBankerUp.Create("",WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rcCreate,this,IDC_BANKER_UP);
	m_btBankerDown.Create("",WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rcCreate,this,IDC_BANKER_DOWN);

	//设置按钮
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	//m_btScoreMoveL.SetButtonImage(IDR_BT_SCORE_MOVE_L,hResInstance,false);
	//m_btScoreMoveR.SetButtonImage(IDR_BT_SCORE_MOVE_R,hResInstance,false);
	//m_btSaveScroe.SetButtonImage(IDR_BT_SAVE_SCORE,hResInstance,false);
	//m_btFetchScore.SetButtonImage(IDR_BT_FETCH_SCORE,hResInstance,false);
	m_btApplyBanker.SetButtonImage(IDR_BT_APPLY_BANKER,hResInstance,false);
	m_btCancelBanker.SetButtonImage(IDR_BT_CANCEL_BANKER,hResInstance,false);
	m_btJetton100.SetButtonImage(IDR_BT_JETTON_100,hResInstance,false);
	m_btJetton1K.SetButtonImage(IDR_BT_JETTON_1000,hResInstance,false);
	m_btJetton1W.SetButtonImage(IDR_BT_JETTON_10000,hResInstance,false);
	m_btJetton10W.SetButtonImage(IDR_BT_JETTON_100000,hResInstance,false);
	m_btJetton100W.SetButtonImage(IDR_BT_JETTON_1000000,hResInstance,false);	
	m_btJetton1000W.SetButtonImage(IDR_BT_JETTON_10000000,hResInstance,false);
	m_btBankerIn.SetButtonImage(IDB_BT_BANK_IN,hResInstance,false);
	m_btBankerOut.SetButtonImage(IDB_BT_BANK_OUT,hResInstance,false);
	m_btPrevious.SetButtonImage(IDB_PREV_PAGE,hResInstance,false);
	m_btNext.SetButtonImage(IDB_NEXT_PAGE,hResInstance,false);
	m_btBankerUp.SetButtonImage(IDB_LIST_UP,hResInstance,false);
	m_btBankerDown.SetButtonImage(IDB_LIST_DOWN,hResInstance,false);

	m_btJetton100.EnableWindow(false);
	m_btJetton1K.EnableWindow(false);
	m_btJetton1W.EnableWindow(false);
	m_btJetton10W.EnableWindow(false);
	m_btJetton100W.EnableWindow(false);
	m_btJetton1000W.EnableWindow(false);


	//装入光标
	CString strCursorFile;
	TCHAR szModuleName[MAX_PATH];
	GetModuleFileName(AfxGetInstanceHandle(), szModuleName, MAX_PATH);
	PathRemoveExtension(szModuleName);
	
	strCursorFile = szModuleName;
	strCursorFile += TEXT("/*\\Cursor*/\\");
	m_hCursorArray[0] = LoadCursorFromFile(strCursorFile+TEXT("CurAddChip0.cur"));
	m_hCursorArray[1] = LoadCursorFromFile(strCursorFile+TEXT("CurAddChip1.cur"));
	m_hCursorArray[2] = LoadCursorFromFile(strCursorFile+TEXT("CurAddChip2.cur"));
	m_hCursorArray[3] = LoadCursorFromFile(strCursorFile+TEXT("CurAddChip3.cur"));
	m_hCursorArray[4] = LoadCursorFromFile(strCursorFile+TEXT("CurAddChip4.cur"));
	m_hCursorArray[5] = LoadCursorFromFile(strCursorFile+TEXT("CurAddChip5.cur"));

	//
	g_pGameClient = (CGameClientDlg*)AfxGetMainWnd();

	//StartMovingAnimal(34);
	

	return 0;
}

BOOL CGameClientView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

//重置界面
void CGameClientView::ResetGameView()
{

}

//调整控件
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	int x = (nWidth-m_ImageViewBack.GetWidth())/2 + OFFSET_X;
	int y = (nHeight-m_ImageViewBack.GetHeight())/2 + OFFSET_Y;

	RectifyArea( nWidth ,nHeight );

	int nXCenter = nWidth/2;
	int nYCenter = nHeight/2;

	//162,319
	m_btPrevious.SetWindowPos(NULL,nXCenter - 164 ,nYCenter+320,0,0,SWP_NOSIZE);
	m_btNext.SetWindowPos(NULL,nXCenter - 164 + 200 + 156 ,nYCenter+320,0,0,SWP_NOSIZE);

	//按钮控件
	//m_btFetchScore.ShowWindow(SW_HIDE);
	//m_btSaveScroe.ShowWindow(SW_HIDE);
	//m_btScoreMoveL.ShowWindow(SW_HIDE);
	//m_btScoreMoveR.ShowWindow(SW_HIDE);
	m_btApplyBanker.SetWindowPos(NULL,nXCenter+196-4,nYCenter-308,0,0,SWP_NOSIZE);
	m_btCancelBanker.SetWindowPos(NULL,nXCenter+196-4,nYCenter-308,0,0,SWP_NOSIZE);

	//282,305
	m_btBankerDown.SetWindowPos(NULL,nXCenter+282-2-2,nYCenter-307,0,0,SWP_NOSIZE);
	m_btBankerUp.SetWindowPos(NULL,nXCenter+282+38-2-2+5,nYCenter-307,0,0,SWP_NOSIZE);

	//255,263
	m_btBankerOut.SetWindowPos(NULL,nXCenter+255,nYCenter+263,0,0,SWP_NOSIZE);
	m_btBankerIn.SetWindowPos(NULL,nXCenter+255,nYCenter+263+42,0,0,SWP_NOSIZE);

	m_btBankerIn.ShowWindow(SW_SHOW);
	m_btBankerOut.ShowWindow(SW_SHOW);

	nXCenter -= 165;
	nYCenter += 260;
	//165,285
#define	 BTN_INTERVAL_DIS	(12+rcBtn.Width())
	CRect rcBtn;
	m_btJetton100.GetClientRect(&rcBtn);

	m_btJetton100.SetWindowPos(NULL,nXCenter,nYCenter,0,0,SWP_NOSIZE);
	m_btJetton1K.SetWindowPos(NULL,nXCenter+BTN_INTERVAL_DIS ,nYCenter,0,0,SWP_NOSIZE);
	m_btJetton1W.SetWindowPos(NULL,nXCenter + BTN_INTERVAL_DIS*2,nYCenter,0,0,SWP_NOSIZE);
	m_btJetton10W.SetWindowPos(NULL,nXCenter + BTN_INTERVAL_DIS*3,nYCenter,0,0,SWP_NOSIZE);
	m_btJetton100W.SetWindowPos(NULL,nXCenter + BTN_INTERVAL_DIS*4,nYCenter,0,0,SWP_NOSIZE);
	m_btJetton1000W.SetWindowPos(NULL,nXCenter + BTN_INTERVAL_DIS*5,nYCenter,0,0,SWP_NOSIZE);

	x=(nWidth-200)/2;
	y=(nHeight-200)/2;

	for (int i=0; i < MAX_CHAIR_NORMAL; i++)
	{
		SetFlowerControlInfo(i,x,y,200,200);
	}
}

//最大下注
void CGameClientView::SetAreaLimitScore(LONG lAreaLimitScore)
{
	if ( m_lAreaLimitScore!= lAreaLimitScore )
	{
		//设置变量
		m_lAreaLimitScore=lAreaLimitScore;
	}
}
//设置筹码
void CGameClientView::SetCurrentJetton(LONG lCurrentJetton)
{
	//设置变量
	ASSERT(lCurrentJetton>=0L);
	m_lCurrentJetton=lCurrentJetton;
}
//筹码按钮
void CGameClientView::OnJettonButton100()
{
	m_lCurrentJetton=100L;
}

//筹码按钮
void CGameClientView::OnJettonButton1K()
{
	m_lCurrentJetton=1000L;
}

//筹码按钮
void CGameClientView::OnJettonButton1W()
{
	m_lCurrentJetton=10000L;
}

//筹码按钮
void CGameClientView::OnJettonButton10W()
{
	m_lCurrentJetton=100000L;
}

//筹码按钮
void CGameClientView::OnJettonButton100W()
{
	m_lCurrentJetton=1000000L;
}

//筹码按钮
void CGameClientView::OnJettonButton500W()
{
	m_lCurrentJetton=5000000L;
}

//定时器消息
void CGameClientView::OnTimer(UINT nIDEvent)
{
	//闪动胜方
	if ( nIDEvent==IDI_FLASH_WINNER )
	{
	//	SetTimer( IDI_FLASH_WINNER , 400 , 0 );
		
		static DWORD nAnimalPos=INVALID_AREA;
		static int count=0;

		count++;
		m_bFlashWinner = TRUE;

		if ( count == 8 )
		{
			m_bDisPlayEnd=TRUE;
			m_lBankerTotal+=m_lBankerCurGameScore;
			m_lMeCurGameScore+=m_lTempCurGameScore;
		}

		if ( nAnimalPos == INVALID_AREA )
		{
			nAnimalPos = m_dwCurrentAniPos;
			m_dwCurrentAniPos = INVALID_AREA;
 		}
		else
		{
		    m_dwCurrentAniPos = nAnimalPos;
			nAnimalPos = INVALID_AREA;
		//	m_bFlashWinner = FALSE; 

			if ( count >= 16 )
			{
				count = 0;
				nAnimalPos = INVALID_AREA;
				m_dwCurrentAniPos=INVALID_AREA;
				m_bFlashWinner=FALSE;
				m_bDisPlayEnd=FALSE;
				m_ImageBackBuf.Destroy();
				m_ImageBackBuf.LoadFromResource(AfxGetInstanceHandle(), IDR_VIEW_BACK,TEXT("JPG"),FALSE);
				memset( m_nTotalUserBet,0,sizeof(m_nTotalUserBet) );
				memset( m_nUserBet,0,sizeof(m_nUserBet) );
				KillTimer(IDI_FLASH_WINNER);
			}

		}

		//更新界面
		UpdateGameView(NULL);
		
		return;
	}

	if (nIDEvent==IDT_COUNT_TIME)
	{
		if( m_cbTimeCount%3==0 )
			DeleteApplyList();
		if ( --m_cbTimeCount<0 )
		{
			m_cbTimeCount=0;
			KillTimer(IDT_COUNT_TIME);
		}
		UpdateGameView(NULL);
		return;
	}

	//轮换庄家
	if ( nIDEvent == IDI_SHOW_CHANGE_BANKER )
	{
		ShowChangeBanker(false);
		return;
	}

	if ( IDI_FLASH_ANIMAL == nIDEvent )
	{
#define COUNT_TIME	16

	//	static int m_cbAnimalPos=35;
		const	static int nInterVal[COUNT_TIME]={80,90,100,120,150,160,170,180,190,210,240,260,300,400,720,1400};
		static int i=-1;
		static int k=INVALID_CHAIR;
		static bool slow=FALSE;
		m_dwCurrentAniPos++;

		g_pGameClient->PlayGameSound( AfxGetInstanceHandle(),"IDC_SELGRID");

		if( m_dwCurrentAniPos>=36  )
		{
			m_dwCurrentAniPos=0;
		}

		if (!slow&&m_dwCurrentAniPos==m_cbRandCount)
		{
			i=0;
			if( m_cbAnimalPos<COUNT_TIME )
			{
				//36-12
				k=35+m_cbAnimalPos-COUNT_TIME + 1;
			}
			else
			{
				k=m_cbAnimalPos-COUNT_TIME;
			}
		}

		//
		if ( i>=0&&i<COUNT_TIME&&k!=INVALID_CHAIR&&m_dwCurrentAniPos == k )
		{
			slow=true;
		}

		//
		if( slow )
		{
			if( i==COUNT_TIME )
			{
				slow=FALSE;
				i=-1;
				KillTimer(IDI_FLASH_ANIMAL);
				SetTimer( IDI_FLASH_WINNER , 500 , 0 );
				UpdateGameView(NULL);
				return ;
			}
			KillTimer(IDI_FLASH_ANIMAL);
			SetTimer(IDI_FLASH_ANIMAL,nInterVal[i++],0);
			UpdateGameView(NULL);
			return ;
		}

		SetTimer(IDI_FLASH_ANIMAL,TIME_INTERVAL,0);
		UpdateGameView(NULL);
		
	}

	if( IDT_RAND_FLASH == nIDEvent )
	{
		m_dwCurrentAniPos = rand()%36;
		m_bFlashWinner=FALSE;
		m_bDisPlayEnd=FALSE;
		KillTimer(IDI_FLASH_WINNER);
		UpdateGameView(NULL);
		return ;
	}

	__super::OnTimer(nIDEvent);
}

//轮换庄家
void CGameClientView::ShowChangeBanker( bool bChangeBanker )
{
	//轮换庄家
	if ( bChangeBanker )
	{	
		m_lBankerTotal = 0;
	//	SetTimer( IDI_SHOW_CHANGE_BANKER, 3000, NULL );
		m_bShowChangeBanker = true;
	}
	else
	{
	//	KillTimer( IDI_SHOW_CHANGE_BANKER );
		m_bShowChangeBanker = false ;
	}

	//更新界面
	UpdateGameView(NULL);
}

//上庄按钮
void CGameClientView::OnApplyBanker()
{
	AfxGetMainWnd()->SendMessage(IDM_APPLY_BANKER,1,0);
}

//下庄按钮
void CGameClientView::OnCancelBanker()
{
	AfxGetMainWnd()->SendMessage(IDM_APPLY_BANKER,0,0);
}

//艺术字体
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos)
{
	//变量定义
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//绘画边框
	pDC->SetTextColor(crFrame);
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
	}

	//绘画字体
	pDC->SetTextColor(crText);
	pDC->TextOut(nXPos,nYPos,pszString,nStringLength);
}

//庄家信息
void CGameClientView::SetBankerInfo( WORD wChairID, BYTE cbBankerTime, LONG lScore )
{
	//设置变量
	m_wCurrentBankerChairID = wChairID;
	m_cbBankerTime = cbBankerTime;
	m_lBankerScore = lScore;

	//更新界面
	UpdateGameView( NULL );
}

//庄家金币
void CGameClientView::SetBankerTreasure(LONG lBankerTreasure)
{
	m_lBankerTreasure = lBankerTreasure;
	UpdateGameView(NULL);
}
//绘制定时
void CGameClientView::DrawTimer(CDC * pDC, int nXPos, int nYPos, WORD wTime, bool bAnimate)
{
	const int nNumberWidth = m_ImageTimerNumber.GetWidth() / 10;
	const int nNumberHeight = m_ImageTimerNumber.GetHeight();
	//const int nBackWidth = m_ImageTimerBack.GetWidth() / 3;
	//const int nBackHeight = m_ImageTimerBack.GetHeight();


	//计算数目
	LONG lNumberCount=2;

	//位置定义
	INT nYDrawPos=nYPos-nNumberHeight/2+3;
	INT nXDrawPos=nXPos+(lNumberCount*nNumberWidth)/2-nNumberWidth;

	int nWidth = m_TimerText.GetWidth()/3;
	int nHeight = m_TimerText.GetHeight();
		
	switch(g_pGameClient->GetGameStatus())
	{
	case  GS_BACCARAT_FREE:
		m_TimerText.Draw(pDC->m_hDC,nXDrawPos - 135 ,nYDrawPos ,  nWidth ,nHeight, 0,0,nWidth ,nHeight );
		break;
	case  GS_BACCARAT_JETTON:
		m_TimerText.Draw(pDC->m_hDC,nXDrawPos - 135 ,nYDrawPos ,  nWidth ,nHeight, nWidth,0,nWidth ,nHeight );
		break;
	case GS_BACCARAT_OPENCARD:
		m_TimerText.Draw(pDC->m_hDC,nXDrawPos - 135 ,nYDrawPos ,  nWidth ,nHeight, 2*nWidth,0,nWidth ,nHeight );
		break;
	}


	WORD temp=wTime;
	//绘画号码
	for (LONG i=0;i<lNumberCount;i++)
	{
		//绘画号码
		WORD wCellNumber=wTime%10;

		if( temp < 9 && i==1 )
		{
			wCellNumber = 0;
			m_ImageTimerNumber.Draw(pDC->GetSafeHdc(),nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,wCellNumber*nNumberWidth,0,
				nNumberWidth,nNumberHeight);
			nXDrawPos-=nNumberWidth;
			continue;
		}
		m_ImageTimerNumber.Draw(pDC->GetSafeHdc(),nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,wCellNumber*nNumberWidth,0,
			nNumberWidth,nNumberHeight);

		//设置变量
		wTime/=10;
		nXDrawPos-=nNumberWidth;
	}

}

//成绩设置
void CGameClientView::SetGameScore(LONG lMeCurGameScore, LONG lMeCurGameReturnScore, LONG lBankerCurGameScore)
{
	m_lTempCurGameScore = lMeCurGameScore;
	m_lBankerCurGameScore=lBankerCurGameScore;
}

//设置标识
void CGameClientView::SetShowGameFlag(bool bShowGameResult)
{
	m_bShowGameResult = bShowGameResult;
	UpdateGameView(NULL);
}

//
BOOL CGameClientView::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	return __super::OnWndMsg(message, wParam, lParam, pResult);
}
//
void CGameClientView::OnMouseMove(UINT nFlags, CPoint point)
{

	if( g_pGameClient->GetGameStatus()!=GS_BACCARAT_JETTON )
	{
		m_dwCursorArea=INVALID_AREA;
		return ;
	}
	
	Sleep(20);
	
	for ( int i=0;i<MAX_AREA;i++ )
	{
		if( rcArea[i].PtInRect(point) )
		{
			m_dwCursorArea = i;
			m_rcCurAreaRect = rcArea[i];
			UpdateGameView(NULL);
			return;
		}
	}
	m_dwCursorArea=INVALID_AREA;

	CGameFrameView::OnMouseMove(nFlags, point);
}

//
void CGameClientView::DrawRect( CDC *pDC , CRect *pRect )
{
	ASSERT( pDC!=NULL&&pRect!=NULL );

	CRect rc=*pRect;
	pDC->Draw3dRect( &rc , 0 , 0 );

	rc.left++,rc.top++;
	rc.right--,rc.bottom--;
	pDC->Draw3dRect( &rc , RGB(255,255,0),RGB(255,255,0) );

	rc.left++,rc.top++;
	rc.right--,rc.bottom--;
	pDC->Draw3dRect(&rc , RGB(255,255,0)/2,RGB(255,255,0)/2 );

}
//显示结果
void CGameClientView::DrawGameResult(CDC *pDC, int nXPos, int nYPos)
{
	//扑克点数
	nXPos = (nXPos - m_ImageGameEnd.GetWidth())/2;
	nYPos =	(nYPos - m_ImageGameEnd.GetHeight())/2 - 10;	

	//画结果框
	m_ImageGameEnd.Draw(pDC->GetSafeHdc(), nXPos, nYPos);
	
	DrawChipNum( pDC , 0 , nXPos + 212, nYPos + 91 - 6 ,m_lTempCurGameScore , 0 ,1 );
	DrawChipNum( pDC , 0 , nXPos + 212, nYPos + 130 - 6 ,m_lBankerCurGameScore , 0 ,1 );
}
//鼠标消息
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint point)
{	

	UpdateGameView(NULL);

	//PlaceUserJetton(12,1000);
	if( g_pGameClient->GetGameStatus()!=GS_BACCARAT_JETTON )
	{
		m_dwCursorArea=INVALID_AREA;
		return ;
	}

	if(m_lCurrentJetton<=0) return ;

	for ( int i=0;i<MAX_AREA;i++ )
	{
		if( rcArea[i].PtInRect(point) )
		{
			m_dwCursorArea = i;
			m_rcCurAreaRect = rcArea[i];
			CMD_C_PlaceJetton PlaceJetton;
			PlaceJetton.cbAnimal = i;
			PlaceJetton.lJettonScore = m_lCurrentJetton;
			PlaceJetton.cbViewJettonArea = i;

			if( i<=11 )
			{
				int k = i;
				int pos  = k%4==0?k:k-1;

				if(i%2==1)
				{
					if(k==3||k==7||k==11)
						pos=k;
					else
						pos=k+1;
				}
				PlaceJetton.cbViewJettonArea = pos;
			}

			g_pGameClient->SendData( SUB_C_PLACE_JETTON ,&PlaceJetton , sizeof(PlaceJetton) );
			break;
		}
	}
	__super::OnLButtonDown(nFlags,point);
}

//鼠标消息
void CGameClientView::OnRButtonDown(UINT nFlags, CPoint Point)
{
	//设置变量
	m_lCurrentJetton=0L;

	//m_vecHistory.push_back(rand()%12);
	//if ( m_vecHistory.size() > MAX_HISTORY_LENTH )
	//{
	//	std::vector<DWORD>::iterator iter =  m_vecHistory.begin() + m_vecHistory.size() - MAX_HISTORY_LENTH;
	//	m_vecHistory.erase(m_vecHistory.begin(),iter);
	//	m_wCursors = m_vecHistory.size() - MAX_HISTORY_DISPLAY;
	//}
	//if( m_vecHistory.size() > MAX_HISTORY_DISPLAY )
	//{
	//	m_wCursors++;
	//}

	//UpdateGameView(NULL);
	__super::OnLButtonDown(nFlags,Point);
}

//光标消息
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	if (m_lCurrentJetton!=0L)
	{
		//获取区
		switch (m_lCurrentJetton)
		{
		case 100:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_100)));
				return TRUE;
			}
		case 1000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_1K)));
				return TRUE;
			}
		case 10000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_1W)));
				return TRUE;
			}
		case 100000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_10W)));
				return TRUE;
			}
		case 1000000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_100W)));
				return TRUE;
			}
		case 5000000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_500W)));
				return TRUE;
			}
		}
	}

	return __super::OnSetCursor(pWnd, nHitTest, uMessage);
}
//绘画界面
int TOP_OFFSET_X;
int LEFT_OFFSET_Y;
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	m_ImageBG.BitBlt( pDC->m_hDC , (nWidth-m_ImageBG.GetWidth())/2 ,(nHeight-m_ImageBG.GetHeight())/2 );

	//绘画背景
	const int nXPos = (nWidth - m_ImageViewBack.GetWidth()) / 2;
	const int nYPos = (nHeight - m_ImageViewBack.GetHeight()) / 2;
	
	m_ImageViewBack.BitBlt(pDC->GetSafeHdc(), nXPos ,nYPos, SRCCOPY);

	m_ImageBackBuf.BitBlt(pDC->m_hDC, nXPos + BET_AREA_LEFT , nYPos + BET_AREA_TOP , BET_AREA_WIDTH , BET_AREA_HEIGH, 
		BET_AREA_LEFT , BET_AREA_TOP ,SRCCOPY);

	//for ( int i=0;i<MAX_AREA;i++)
	//{
	//	DrawRect(pDC,&rcArea[i]);
	//}

	//闪动动画
	if( m_bFlashWinner && m_dwCurrentAniPos != INVALID_AREA ) 
	{
		int i = m_dwCurrentAniPos%12;
		int pos  = i%4==0?i:i-1;

		if(m_dwCurrentAniPos%2==1)
		{
			if(i==3||i==7||i==11)
				pos=i;
			else
				pos=i+1;
		}

		int pos2 = 12+(m_dwCurrentAniPos)%2;
		DrawRect( pDC , &rcArea[pos] );
		DrawRect( pDC , &rcArea[pos2] );
	}
	else
	{
		if( m_dwCursorArea>=0 && m_dwCursorArea < MAX_AREA && m_bFlashWinner == FALSE )
		{
			DrawRect(pDC,&m_rcCurAreaRect);
		}
	}

	DrawHighLightAnimal(pDC ,nWidth ,nHeight);
	DrawUerBet(pDC);

	if( m_bDisPlayEnd )
	{
		DrawGameResult( pDC , nWidth , nHeight );
	}
	DrawUserInfo(pDC , nWidth , nHeight);

	int x = nWidth/2;
	int y = nHeight/2;
	
	if( m_cbTimeCount>=0 )
	{
		DrawTimer(pDC,x+40+7,y-300,m_cbTimeCount,0);
	}
}
//
void CGameClientView::StartMovingAnimal( BYTE animalpos )
{	
	srand(time(NULL));
	m_cbAnimalPos = animalpos;
	m_dwCurrentAniPos = INVALID_AREA;
	m_cbRandCount=rand()%18+18;
	m_bFlashWinner=FALSE;
	m_bDisPlayEnd=FALSE;
	KillTimer(IDI_FLASH_WINNER);
	KillTimer(IDT_RAND_FLASH);
	SetTimer(IDI_FLASH_ANIMAL,TIME_INTERVAL,0);
}
//
void CGameClientView::StartRandAnimal()
{
	ZeroMemory(m_nTotalUserBet,sizeof(m_nTotalUserBet));
	ZeroMemory(m_nUserBet,sizeof(m_nUserBet));

	SetTimer( IDT_RAND_FLASH , 250 , 0 );
	SetTimer(IDT_COUNT_TIME	,1000,0);
}
//
void CGameClientView::DrawUerBet(CDC *pDC)
{
	for (int i=0;i<MAX_AREA;i++)
	{	
		if( m_nTotalUserBet[i]>0 )
		{
			DrawChipNum( pDC , 0 , rcArea[i].left + rcArea[i].Width()/2   , rcArea[i].top + 10 , m_nTotalUserBet[i],0 ,1 );
		}
		if( m_nUserBet[i]>0 )
		{
			DrawChipNum( pDC , 0 , rcArea[i].left + rcArea[i].Width()/2  , rcArea[i].top + rcArea[i].Height()/2 + 5 , m_nUserBet[i],1 ,1 );
		}
	}
}
//
void CGameClientView::DrawHighLightAnimal(CDC *pDC ,int nWidth ,int nHeight )
{
	if ( m_dwCurrentAniPos>=0 && m_dwCurrentAniPos<36 )
	{
		TOP_OFFSET_X = ((nWidth - m_ImageViewBack.GetWidth()) / 2) + 27+8;
		LEFT_OFFSET_Y =	((nHeight - m_ImageViewBack.GetHeight()) / 2) + 77+8;
#define ANIMAL_W		((m_ImageAnimal[0].GetWidth())-18)
#define ANIMAL_H		(m_ImageAnimal[0].GetHeight()-17)	

		int nAnimal=m_dwCurrentAniPos%12;
		int nDestX,nDestY;

		//0-8
		if( m_dwCurrentAniPos < 9 )
		{
			nDestX = TOP_OFFSET_X + m_dwCurrentAniPos*ANIMAL_W+m_dwCurrentAniPos*12;
			nDestY = LEFT_OFFSET_Y;
		}
		//9-17
		if ( m_dwCurrentAniPos >= 9 && m_dwCurrentAniPos < 18 )
		{
			nDestX = TOP_OFFSET_X + 9 * ANIMAL_W+12*9;
			nDestY = LEFT_OFFSET_Y + (m_dwCurrentAniPos-9)*ANIMAL_H+12*(m_dwCurrentAniPos-9);
		}
		//18-26
		if( m_dwCurrentAniPos >= 18 && m_dwCurrentAniPos < 27 )
		{
			nDestX = TOP_OFFSET_X + (27-m_dwCurrentAniPos)*ANIMAL_W+12*(27-m_dwCurrentAniPos);
			nDestY = LEFT_OFFSET_Y + 9*ANIMAL_H+12*9;
		}
		//27-35
		if ( m_dwCurrentAniPos>=27 && m_dwCurrentAniPos<36 )
		{
			nDestX = TOP_OFFSET_X;
			nDestY = LEFT_OFFSET_Y - (m_dwCurrentAniPos-36)*(ANIMAL_H+12);
		}

		m_ImageAnimal[nAnimal].Draw(pDC->m_hDC,nDestX,nDestY,
			m_ImageAnimal[0].GetWidth(),m_ImageAnimal[0].GetHeight(), 0 ,0,m_ImageAnimal[0].GetWidth(),m_ImageAnimal[0].GetHeight() );
	}

}

//设置筹码
void CGameClientView::PlaceUserJetton(BYTE cbViewIndex, LONG lScoreCount)
{
	HDC hdc	= m_ImageBackBuf.GetDC();

	const int w = m_ImageJettonView.GetWidth()/6;
	const int h = m_ImageJettonView.GetHeight();


	int index=0;
	switch ( lScoreCount )
	{
	case 100:
		index=0;
		break;
	case 1000:
		index=1;
		break;
	case 10000:
		index=2;
		break;
	case 100000:
		index=3;
		break;
	case 1000000:
		index=4;
		break;
	case 5000000:
		index=5;
		break;
	}

	//127,72
	if( cbViewIndex<=11 )
	{	
		//40
		m_ImageJettonView.Draw( hdc , rcLocalArea[cbViewIndex].left + 18 + rand()%60  ,rcLocalArea[cbViewIndex].top + (rand()%60) ,
			w , h , index*w ,0 , w ,h );
	}
	else
	{
		m_ImageJettonView.Draw( hdc , rcLocalArea[cbViewIndex].left + 30 +  (rand()%190) ,rcLocalArea[cbViewIndex].top + (rand()%27) + 5  ,
			w , h , index*w ,0 , w ,h );		
	}

	
	m_ImageBackBuf.ReleaseDC();
	UpdateGameView(NULL);
}

void CGameClientView::OnBankIn()
{
	IClientKernel *pIClientKernel = (IClientKernel *)g_pGameClient->GetClientKernel(IID_IClientKernel,VER_IClientKernel);
	ASSERT(pIClientKernel!=NULL);
	if (pIClientKernel==NULL) return ;
	pIClientKernel->ShowBankDialog( 0,0 );
	return ;
}
//
void CGameClientView::OnBankOut()
{
	IClientKernel *pIClientKernel = (IClientKernel *)g_pGameClient->GetClientKernel(IID_IClientKernel,VER_IClientKernel);
	ASSERT(pIClientKernel!=NULL);
	if (pIClientKernel==NULL) return ;
	pIClientKernel->ShowBankDialog( 1,0 );
	return ;
}
void CGameClientView::OnBankDown()
{
	//if( m_vecHistory.size() - m_wCursors < MAX_HISTORY_DISPLAY + 1 )	return ;
	//m_wCursors++;

	//int count = m_ApplyUserList.GetCount() - 1;

	//if (  m_wBankerCursors + 3  >= count )
	//{
	//	return ;
	//}

	if( m_ApplyUserList.GetCount() - m_wBankerCursors < 3 + 1 ) return ;

	m_wBankerCursors++;
	UpdateGameView(NULL);

}
//
void CGameClientView::OnBankUp()
{
	if( m_wBankerCursors == 0 )	return ;

	m_wBankerCursors--;
	UpdateGameView(NULL);
}
//
void CGameClientView::OnNext()
{
	if( m_vecHistory.size() - m_wCursors < MAX_HISTORY_DISPLAY + 1 )	return ;
	m_wCursors++;
	UpdateGameView(NULL);
}
//
void CGameClientView::OnPrevious()
{
	if( m_wCursors == 0 )	return;
	m_wCursors--;
	UpdateGameView(NULL);
}
void CGameClientView::DrawUserInfo( CDC *pDC , int nWidth , int nHeight )
{
	TCHAR num[30]={0};
	//
	const tagUserData * pUserInfo = g_pGameClient->GetUserData(g_pGameClient->GetMeChairID());
	pDC->SelectObject(CSkinEditAttribute::GetDefaultFont());
	pDC->SetTextColor(0xffffff);
	int x = nWidth/2;
	int y = nHeight/2;
	TCHAR ch[50];

	if ( pUserInfo )
	{
		//昵称
		//喜币
		//成绩
		pDC->SetTextColor(0);

		sprintf(ch,"昵称：%s",pUserInfo->szName);
		pDC->TextOut(x-320-38,y+274,ch);

		TCHAR temp[30];
		FormatNum( temp , pUserInfo->lGameGold );
		sprintf(ch,"当前：%s",temp);
		pDC->TextOut(x-320-38,y+274+15,ch);

		FormatNum( temp , m_nValidMoney );
		sprintf(ch,"可用：%s",temp);
		pDC->TextOut(x-320-38,y+274+15*2,ch);

		FormatNum( temp , m_lMeCurGameScore );
		sprintf(ch,"成绩：%s",temp );
		pDC->TextOut(x-320-38,y+274+15*3,ch);		

	}

	if( m_wCurrentBankerChairID != INVALID_CHAIR )
	{
		//当前庄家名字
		//连盘庄数
		//庄家金币
		//庄家成绩
		const tagUserData * pUserInfo = g_pGameClient->GetUserData(m_wCurrentBankerChairID);

		if( pUserInfo )
		{			
			pDC->SetTextColor(0x00ffff);
			sprintf(ch,"当前庄家：%s",pUserInfo->szName);
			pDC->TextOut(x-366,y-332-14,ch);

			sprintf(ch,"连庄盘数：%d",m_cbBankerTime);
			pDC->TextOut(x-366,y-332-14+15,ch);
	
			TCHAR temp[30];
			FormatNum( temp ,pUserInfo->lGameGold );
			sprintf(ch,"庄家金币：%s",temp);
			pDC->TextOut(x-366,y-332-14+15*2,ch);
	
			FormatNum( temp ,m_lBankerTotal );
			sprintf(ch,"庄家成绩：%s",temp);
			pDC->TextOut(x-366,y-332-14+15*3,ch);
		}

	}

	if( m_ApplyUserList.GetCount() )
	{
#define  MAX_DISPLAY_COUNT 3
		pDC->SetTextColor(0x00ffff);

		for ( int i=0;i<MAX_DISPLAY_COUNT;i++ )
		{
			if( i+m_wBankerCursors >= m_ApplyUserList.GetCount() ) break;

			pDC->TextOut(x+200,y-350+13*(i),m_ApplyUserList[i+m_wBankerCursors].strUserName);
			pDC->SetTextAlign(TA_RIGHT);

			TCHAR temp[30];
			FormatNum( temp ,m_ApplyUserList[i+m_wBankerCursors].lUserScore );
			pDC->TextOut(x+200+160,y-350+13*(i),temp);
			pDC->SetTextAlign(TA_LEFT);
		}
	}
	for (int i=0;i<m_vecHistory.size()&&i<MAX_HISTORY_DISPLAY;i++)
	{
		int nPos=0;
		if ( i+m_wCursors<m_vecHistory.size() )
			nPos=m_vecHistory[i+m_wCursors];
		else
			break;
		//14,13
		//162,319
		m_ImageText.Draw( pDC->m_hDC ,x - 162 + 35 + i*35 ,y + 319 + 3 ,14,13,nPos*14,0,14,13 );
	}
}

void CGameClientView::FormatNum( TCHAR ch[] , INT64 n )
{	
	if( n == 0 ) 
	{	
		sprintf(ch ,"%d", n);
		return;
	}

	INT64 m = n>0?n:-n;
	TCHAR temp[30];
	TCHAR num[30]={0};
	sprintf(temp ,"%I64d" ,m);
	
	int k=strlen(temp)/4;
	int j=strlen(temp)/4 + strlen(temp)-1;
	if( strlen(temp)%4==0 ) k-=1,j-=1;

	int count=0;
	for (int i=strlen(temp)-1;;i--)
	{
		num[j--] = temp[i];
		count++;
		if( (count)%4 == 0 && k > 0 )
		{	
			k--;
			num[j--]=',';
			count=0;
		}
  
		if( j == -1 ) break;
	}
	
	if ( n < 0 )
	{	
		sprintf( ch,"-%s",num);
	}
	else
	{
		sprintf( ch,"%s",num);
	}
}

void CGameClientView::RectifyArea( const int nWidth , const int nHeight )
{
	int x = (nWidth-m_ImageViewBack.GetWidth())/2 + OFFSET_X;
	int y = (nHeight-m_ImageViewBack.GetHeight())/2 + OFFSET_Y;

	for ( int i=0;i<MAX_AREA;i++ )
	{
		if(i<4)
		{
			rcArea[i].left = x + AREA_WIDTH*i;		
			rcArea[i].top  = y;
		}

		if(i>=4&&i<8 )
		{
			rcArea[i].left = x + AREA_WIDTH*(i%4);		
			rcArea[i].top  = y + AREA_HEIGHT;
		}

		if(i>=8 )
		{
			rcArea[i].left = x + AREA_WIDTH*(i%4);		
			rcArea[i].top  = y + AREA_HEIGHT*2;
		}

		rcArea[i].right = rcArea[i].left + AREA_WIDTH;
		rcArea[i].bottom = rcArea[i].top + AREA_HEIGHT;

		if( i==12 )
		{
			rcArea[i].left = x ;		
			rcArea[i].top  = y + AREA_HEIGHT*3;

			rcArea[i].right = rcArea[i].left + AREA_WIDTH*2;
			rcArea[i].bottom = rcArea[i].top + AREA_HEIGHT - 34;
		}

		if( i==13 )
		{
			rcArea[i].left = x + AREA_WIDTH*2;		
			rcArea[i].top  = y + AREA_HEIGHT*3;

			rcArea[i].right = rcArea[i].left + AREA_WIDTH*2;
			rcArea[i].bottom = rcArea[i].top + AREA_HEIGHT - 34;
		}

	}
	m_dwCursorArea=INVALID_AREA;
}

void CGameClientView::DeleteApplyList()
{
	if( m_wCurrentBankerChairID == INVALID_CHAIR )
	{
		return;
	}

	const tagUserData *pUserDate =g_pGameClient->GetUserData(m_wCurrentBankerChairID);

	if( !pUserDate )
	{
	//	g_pGameClient->InsertSystemString("错误");
	}

	int count = m_ApplyUserList.GetSize();
	for (int i=0;i<count;i++)
	{
		if( pUserDate &&  0 == lstrcmp( pUserDate->szName , m_ApplyUserList[i].strUserName ) )
		{
			m_ApplyUserList.RemoveAt(i);
			UpdateGameView(NULL);
			return;
		}
	}
}