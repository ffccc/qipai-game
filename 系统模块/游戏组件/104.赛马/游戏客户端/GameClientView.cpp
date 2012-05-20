#include "StdAfx.h"
#include "Math.h"
#include "Resource.h"
#include "GameClientView.h"
#include "GameClientDlg.h"
#include "ch.h"
//////////////////////////////////////////////////////////////////////////

//时间标识
#define IDI_FLASH_WINNER			100									//闪动标识
#define IDI_SHOW_CHANGE_BANKER		101									//轮换庄家
#define IDI_HORSE_EFFECT			102									//赛马标识
#define IDI_HORSE_STATIC			103									//赛马标识
#define IDI_BOMB_EFFECT				200									//爆炸标识


//按钮标识
#define IDC_JETTON_BUTTON_100		200									//按钮标识
#define IDC_JETTON_BUTTON_1000		201									//按钮标识
#define IDC_JETTON_BUTTON_10000		202									//按钮标识
#define IDC_JETTON_BUTTON_100000	203									//按钮标识
#define IDC_JETTON_BUTTON_1000000	204									//按钮标识
#define IDC_JETTON_BUTTON_5000000	205									//按钮标识
#define IDC_JETTON_BUTTON_10000000	206									//按钮标识
#define IDC_APPY_BANKER				207									//按钮标识
#define IDC_CANCEL_BANKER			208									//按钮标识
#define IDC_SCORE_MOVE_L			209									//按钮标识
#define IDC_SCORE_MOVE_R			210									//按钮标识
#define IDC_VIEW_CHART				211									//按钮标识
#define IDC_BANK					214									//存钱

//爆炸数目
#define BOMB_EFFECT_COUNT			8									//爆炸数目
#define MAX_HISTORY_NUM				10                                  //历史记录数量

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_JETTON_BUTTON_100, OnJettonButton100)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_1000, OnJettonButton1000)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_10000, OnJettonButton10000)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_100000, OnJettonButton100000)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_1000000, OnJettonButton1000000)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_5000000, OnJettonButton5000000)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_10000000, OnJettonButton10000000)

	ON_BN_CLICKED(IDC_VIEW_CHART, OnViewChart)
	ON_BN_CLICKED(IDC_APPY_BANKER, OnApplyBanker)
	ON_BN_CLICKED(IDC_CANCEL_BANKER, OnCancelBanker)
	ON_BN_CLICKED(IDC_SCORE_MOVE_L, OnScoreMoveL)
	ON_BN_CLICKED(IDC_SCORE_MOVE_R, OnScoreMoveR)
	ON_BN_CLICKED(IDC_BANK,OnBank)
	//ON_BN_CLICKED(IDC_GET_SCORE,OnBankGet)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientView::CGameClientView() : CGameFrameView(true,24)
{
	//下注信息
	m_lMeScore2=0L;
	m_lMeScore3=0L;
	m_lMeScore1=0L;
	m_lMeScore6=0L;
	m_lMeScore5=0L;
	m_lMeScore4=0L;
	m_lMeScore7=0L;
	m_lMeScore8=0L;

	//发牌变量
	//ZeroMemory(m_cbSendCount,sizeof(m_cbSendCount));
	
	//全体下注
	m_lAllScore2=0L;
	m_lAllScore3=0L;
	m_lAllScore1=0L;
	m_lAllScore6=0L;
	m_lAllScore5=0L;
	m_lAllScore4=0L;
	m_lAllScore7=0L;
	m_lAllScore8=0L;

	//庄家信息
	m_wBankerUser=INVALID_CHAIR;		
	m_wBankerTime=0;
	m_lBankerScore=0L;	
	m_lBankerWinScore=0L;
	m_lTmpBankerWinScore=0;
	m_cbBlowIndex = 0;

	//当局成绩
	m_lMeCurGameScore=0L;	
	m_lMeCurGameReturnScore=0L;
	m_lBankerCurGameScore=0L;
	m_lGameRevenue=0L;
	//动画变量
	ZeroMemory(m_bBombEffect,sizeof(m_bBombEffect));
	ZeroMemory(m_cbBombFrameIndex,sizeof(m_cbBombFrameIndex));

	ZeroMemory(m_cbHorseSpeed,sizeof(m_cbHorseSpeed));	
	m_nSprint = 0;
	m_bFinish = false;
	m_bCutPhoto = false;
	m_Num = 0;
	InitHorseInfo();
	InitSignInfo();

	//状态信息
	m_lCurrentJetton=0L;
	m_cbWinnerSide=0xFF;
	m_cbAreaFlash=0xFF;
	m_wMeChairID=INVALID_CHAIR;
	m_bShowChangeBanker=false;

	m_lMeCurGameScore=0L;			
	m_lMeCurGameReturnScore=0L;	
	m_lBankerCurGameScore=0L;	
	m_lMeCurJettonScore=0L;

	m_lAreaLimitScore=0L;	

	m_nPosX = 0;
	m_nStartPosX = 0;
	m_nEndPosX = 782;

	//位置信息
	m_nScoreHead = 0;
	m_nRecordFirst= 0;	
	m_nRecordLast= 0;	

	//历史成绩
	m_lMeStatisticScore=0;

	//控件变量
	m_pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);

	//加载位图
	m_ImageViewBack.SetLoadInfo(IDB_VIEW_BACK);
	
	m_ImageJettonView.LoadImage(IDB_JETTOM_VIEW);
	m_ImageMeScoreNumber.LoadImage(IDB_ME_SCORE_NUMBER);

	m_ImageMeBanker.LoadImage( IDB_ME_BANKER );
	m_ImageChangeBanker.LoadImage( IDB_CHANGE_BANKER );
	m_ImageNoBanker.LoadImage( IDB_NO_BANKER );	

	m_ImageWinFlags.LoadImage(IDB_WIN_FLAGS);

	m_ImageBombEffect.LoadImage(FIRE_EFFECT);
	m_ImageGround.LoadImage(IDB_BG_MAP);
	m_ImageBgJetton.LoadImage(IDB_BG_JETTON);
	m_ImageCut.SetLoadInfo(IDB_BG_Cut);
	m_ImageSign.LoadImage(IDB_SIGN);
	m_ImageStartLine.LoadImage(IDB_START_LINE);
	m_ImageEndLine.LoadImage(IDB_END_LINE);
	m_ImageMeScore.LoadImage(IDB_SCORE_ME);
	m_ImageFirst.LoadImage(IDB_NO_1);
	m_ImageResult.LoadImage(IDB_GAME_RESULT);
	m_ImageFrame.LoadImage(IDB_GAME_FRAME);
	m_ImageBlow.LoadImage(IDB_GAME_BLOW);


	TCHAR str[200];
	for (int i=0; i< HORSE_COUNT; i++)
	{
		ZeroMemory(str,sizeof(str));
		sprintf(str,STATIC_HORSE,i+1);
		m_ImageSaticHorse[i].LoadImage(str);

		ZeroMemory(str,sizeof(str));
		sprintf(str,DYNAMIC_HORSE,i+1);
		m_ImageDynamicHorse[i].LoadImage(str);
	}
	
	return;
}

//析构函数
CGameClientView::~CGameClientView(void)
{
}

//建立消息
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//创建控件
	CRect rcCreate(0,0,0,0);
	m_ApplyUser.Create( IDD_DLG_GAME_RECORD	, this );
	
	//下注按钮
	m_btJetton100.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_100);
	m_btJetton1000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_1000);
	m_btJetton10000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_10000);
	m_btJetton100000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_100000);
	m_btJetton1000000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_1000000);
	m_btJetton5000000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_5000000);
	m_btJetton10000000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_10000000);
	//m_btSaveScore.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SAVE_SCORE);
	//m_btGetScore.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_GET_SCORE);
	//申请按钮
	m_btApplyBanker.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_APPY_BANKER);
	m_btCancelBanker.Create(NULL,WS_CHILD|WS_DISABLED,rcCreate,this,IDC_CANCEL_BANKER);

	m_btBank.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BANK);

	m_btScoreMoveL.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_L);
	m_btScoreMoveR.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_R);

	//设置按钮
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_btJetton100.SetButtonImage(IDB_BT_JETTON_100,false);
	m_btJetton1000.SetButtonImage(IDB_BT_JETTON_1000,false);
	m_btJetton10000.SetButtonImage(IDB_BT_JETTON_10000,false);
	m_btJetton100000.SetButtonImage(IDB_BT_JETTON_100000,false);
	m_btJetton1000000.SetButtonImage(IDB_BT_JETTON_1000000,false);
	m_btJetton5000000.SetButtonImage(IDB_BT_JETTON_5000000,false);
	m_btJetton10000000.SetButtonImage(IDB_BT_JETTON_10000000,false);

	//m_btSaveScore.SetButtonImage(IDB_BT_SAVE_SOCRE,false);
	//m_btGetScore.SetButtonImage(IDB_BT_GET_SOCRE,false);

	m_btApplyBanker.SetButtonImage(IDB_BT_APPLY_BANKER,false);
	m_btCancelBanker.SetButtonImage(IDB_BT_CANCEL_APPLY,false);

	m_btBank.SetButtonImage(IDB_GAME_BANK,false);

	m_btScoreMoveL.SetButtonImage(IDB_BT_SCORE_MOVE_L,false);
	m_btScoreMoveR.SetButtonImage(IDB_BT_SCORE_MOVE_R,false);


	//路单控件
	//if (m_DlgViewChart.m_hWnd==NULL) m_DlgViewChart.Create(IDD_VIEW_CHART,this);	
	SetTimer(IDI_HORSE_STATIC,100,0);
	return 0;
}

//马的信息初始化
void CGameClientView::InitHorseInfo()
{
	SYSTEMTIME time;
	GetLocalTime(&time);
	srand(time.wSecond);
	ZeroMemory(m_Horse,sizeof(m_Horse));

	for (int i=0; i < HORSE_COUNT; i++)
	{
		m_Horse[i].bStatic = true;
		m_Horse[i].bBlow = false;
		m_Horse[i].cbHorseIndex = rand()%12;
		m_Horse[i].cbHorseIndex1 = rand()%8;
		m_Horse[i].nSpeed = 0;
		m_Horse[i].nXPos = 13;
		m_Horse[i].nYPos = 125+i*47;
	}
	m_nPosX = 0;
	m_nStartPosX = 0;
	m_nEndPosX = 782;

	
}

void CGameClientView::InitSignInfo()
{
	ZeroMemory(m_Sign,sizeof(m_Sign));
	for (int i=0; i < HORSE_COUNT; i++)
	{
		m_Sign[i].fSpeed = 0.5f;
		m_Sign[i].fXPos = 180.f;
		m_Sign[i].fYPos = 12.f+i*12.f;
	}
}

//设置赛马速度
void CGameClientView::SetEveryHorseSpeed(BYTE cbSpeed)
{
	m_nSprint = cbSpeed;	
	for (int i=0; i < HORSE_COUNT; i++)
	{
		m_Horse[i].nSpeed = m_cbHorseSpeed[i][m_nSprint];
		m_Sign[i].fSpeed = 0.3f + m_Horse[i].nSpeed/8.f;

		if (m_nSprint > 3)
		{
			if (rand()%2 == 0 && m_cbHorseSpeed[i][m_nSprint-1] - m_cbHorseSpeed[i][m_nSprint] > 0)
			{
				m_Horse[i].bBlow = true;				
			}else
			{
				m_Horse[i].bBlow = false;
			}
		}else
		{
			m_Horse[i].bBlow = false;
		}
	}
}


//重置界面
void CGameClientView::ResetGameView()
{
	//清空列表
	m_ApplyUser.ClearAll();

	//清除桌面
	CleanUserJetton();

	//动画变量
	ZeroMemory(m_bBombEffect,sizeof(m_bBombEffect));
	ZeroMemory(m_cbBombFrameIndex,sizeof(m_cbBombFrameIndex));
	ZeroMemory(m_cbHorseSpeed,sizeof(m_cbHorseSpeed));	

	m_nSprint = 0;
	m_bFinish = false;
	m_bCutPhoto = false;
	InitHorseInfo();
	InitSignInfo();
	m_nEndPosX = 782;
	m_lMeCurJettonScore=0L;

	//设置按钮
	m_btApplyBanker.ShowWindow(SW_SHOW);
	m_btApplyBanker.EnableWindow(FALSE);
	m_btCancelBanker.ShowWindow(SW_HIDE);
	m_btCancelBanker.SetButtonImage(IDB_BT_CANCEL_APPLY,false);

	return;
}

//调整控件
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{	
	//区域位置
	int nCenterX=nWidth/2, nCenterY=nHeight/2;
	int iWidth = 160;//下注框的宽
	int iHeight = 130;//下注框的高
	int nOffX = 15;
	int nOffY = 10;

	m_rcFrame1.left=nCenterX - iWidth*2 - nOffX*3/2;
	m_rcFrame1.top=nCenterY-iHeight - 45;
	m_rcFrame1.right=m_rcFrame1.left+iWidth;
	m_rcFrame1.bottom=m_rcFrame1.top+iHeight;

	m_rcFrame2.left=nCenterX - iWidth - nOffX/2;
	m_rcFrame2.top=m_rcFrame1.top;
	m_rcFrame2.right=m_rcFrame2.left+iWidth;
	m_rcFrame2.bottom=m_rcFrame2.top+iHeight;

	m_rcFrame3.left=nCenterX+nOffX/2;
	m_rcFrame3.top=m_rcFrame1.top;
	m_rcFrame3.right=m_rcFrame3.left+iWidth;
	m_rcFrame3.bottom=m_rcFrame3.top+iHeight;

	m_rcFrame4.left=nCenterX + iWidth + nOffX*3/2;
	m_rcFrame4.top=m_rcFrame1.top;
	m_rcFrame4.right=m_rcFrame4.left+iWidth;
	m_rcFrame4.bottom=m_rcFrame4.top+iHeight;
	
	m_rcFrame5.left=m_rcFrame1.left;
	m_rcFrame5.top=nCenterY+iHeight/2 - 10;
	m_rcFrame5.right=m_rcFrame5.left+iWidth;
	m_rcFrame5.bottom=m_rcFrame5.top+iHeight;

	m_rcFrame6.left=m_rcFrame2.left;
	m_rcFrame6.top=m_rcFrame5.top;
	m_rcFrame6.right=m_rcFrame6.left+iWidth;
	m_rcFrame6.bottom=m_rcFrame6.top+iHeight;

	m_rcFrame7.left=m_rcFrame3.left;
	m_rcFrame7.top=m_rcFrame5.top;
	m_rcFrame7.right=m_rcFrame7.left+iWidth;
	m_rcFrame7.bottom=m_rcFrame7.top+iHeight;	

	m_rcFrame8.left=m_rcFrame4.left;
	m_rcFrame8.top=m_rcFrame5.top;
	m_rcFrame8.right=m_rcFrame8.left+iWidth;
	m_rcFrame8.bottom=m_rcFrame8.top+iHeight;

	//筹码数字
	int ExcursionY=10;
	m_PointJettonNumber[ID_HORSE1-1].SetPoint((m_rcFrame1.right+m_rcFrame1.left)/2, (m_rcFrame1.bottom+m_rcFrame1.top)/2-ExcursionY);
	m_PointJettonNumber[ID_HORSE2-1].SetPoint((m_rcFrame2.right+m_rcFrame2.left)/2, (m_rcFrame2.bottom+m_rcFrame2.top)/2-ExcursionY);
	m_PointJettonNumber[ID_HORSE3-1].SetPoint((m_rcFrame3.right+m_rcFrame3.left)/2, (m_rcFrame3.bottom+m_rcFrame3.top)/2-ExcursionY);
	m_PointJettonNumber[ID_HORSE4-1].SetPoint((m_rcFrame4.right+m_rcFrame4.left)/2, (m_rcFrame4.bottom+m_rcFrame4.top)/2-ExcursionY);
	m_PointJettonNumber[ID_HORSE5-1].SetPoint((m_rcFrame5.right+m_rcFrame5.left)/2, (m_rcFrame5.bottom+m_rcFrame5.top)/2-ExcursionY);
	m_PointJettonNumber[ID_HORSE6-1].SetPoint((m_rcFrame6.right+m_rcFrame6.left)/2, (m_rcFrame6.bottom+m_rcFrame6.top)/2-ExcursionY);
	m_PointJettonNumber[ID_HORSE7-1].SetPoint((m_rcFrame7.right+m_rcFrame7.left)/2, (m_rcFrame7.bottom+m_rcFrame7.top)/2-ExcursionY);
	m_PointJettonNumber[ID_HORSE8-1].SetPoint((m_rcFrame8.right+m_rcFrame8.left)/2, (m_rcFrame8.bottom+m_rcFrame8.top)/2-ExcursionY);
	
	//筹码位置
	m_PointJetton[ID_HORSE1-1].SetPoint(m_rcFrame1.left, m_rcFrame1.top);
	m_PointJetton[ID_HORSE2-1].SetPoint(m_rcFrame2.left, m_rcFrame2.top);
	m_PointJetton[ID_HORSE3-1].SetPoint(m_rcFrame3.left, m_rcFrame3.top);
	m_PointJetton[ID_HORSE4-1].SetPoint(m_rcFrame4.left, m_rcFrame4.top);
	m_PointJetton[ID_HORSE5-1].SetPoint(m_rcFrame5.left, m_rcFrame5.top);
	m_PointJetton[ID_HORSE6-1].SetPoint(m_rcFrame6.left, m_rcFrame6.top);
	m_PointJetton[ID_HORSE7-1].SetPoint(m_rcFrame7.left, m_rcFrame7.top);
	m_PointJetton[ID_HORSE8-1].SetPoint(m_rcFrame8.left, m_rcFrame8.top);


	//移动控件
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;

	//列表控件
	DeferWindowPos(hDwp,m_ApplyUser,NULL,nWidth/2 + 154,nHeight/2-315,217,75,uFlags);
	//筹码按钮
	CRect rcJetton;
	m_btJetton100.GetWindowRect(&rcJetton);

	int nYPos = nHeight/2+220;
	int nXPos = nWidth/2-180;
	int nSpace = 2;

	DeferWindowPos(hDwp,m_btJetton100,NULL,nXPos,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton1000,NULL,nXPos + nSpace + rcJetton.Width(),nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton10000,NULL,nXPos + nSpace * 2 + rcJetton.Width() * 2,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton100000,NULL,nXPos + nSpace * 3 + rcJetton.Width() * 3,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton1000000,NULL,nXPos + nSpace * 4 + rcJetton.Width() * 4,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton5000000,NULL,nXPos + nSpace * 5 + rcJetton.Width() * 5,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton10000000,NULL,nXPos + nSpace * 6 + rcJetton.Width() * 6,nYPos,0,0,uFlags|SWP_NOSIZE);

	//上庄按钮
	DeferWindowPos(hDwp,m_btApplyBanker,NULL,nWidth/2+300,nHeight/2-340,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btCancelBanker,NULL,nWidth/2+300,nHeight/2-340,0,0,uFlags|SWP_NOSIZE);

	DeferWindowPos(hDwp,m_btBank,NULL,nWidth/2+302,nHeight/2+282,0,0,uFlags|SWP_NOSIZE);

	DeferWindowPos(hDwp,m_btScoreMoveL,NULL,nWidth/2-154,nHeight/2+300,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btScoreMoveR,NULL,nWidth/2+252,nHeight/2+300,0,0,uFlags|SWP_NOSIZE);
	//结束移动
	EndDeferWindowPos(hDwp);

	m_nWinFlagsExcursionX = nWidth/2-111;	
	m_nWinFlagsExcursionY = nHeight/2 + 300;

	return;
}
#include "WINDOWSX.H"
//照相截图
void CGameClientView::CutPhoto(int nWidth, int nHeight, CDC *pDC)
{
	//CDC* pDeskDC =  GetDesktopWindow()->GetDC();		//获取桌面画布对象
	//CRect rc;
	//GetDesktopWindow()->GetClientRect(rc);				//获取屏幕的客户区域
	CDC  memDC;											//定义一个内存画布
	memDC.CreateCompatibleDC(pDC);					//创建一个兼容的画布
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC,770, m_ImageGround->GetHeight());	//创建兼容位图
	memDC.SelectObject(&bmp);							//选中位图对象	
	BITMAP bitmap;
	bmp.GetBitmap(&bitmap);								//获得图片信息
	memDC.BitBlt(0, -nHeight/2 + 452/2, bitmap.bmWidth,bitmap.bmHeight+130,pDC,0,0,SRCCOPY);//绘制图片

	//if(m_Cursor.GetCheck())								//选择抓取鼠标
	//{
	//	CPoint point;
	//	GetCursorPos(&point);							//鼠标位置
	//	HICON hicon = (HICON)GetCursor();				//获得鼠标图标句柄
	//	memDC.DrawIcon(point.x-10,point.y-10,hicon);					//绘制鼠标图标
	//}
	m_pGameClientDlg->PlayGameSound(RACE_Cut);
	CImageHandle ImageHandle(&m_ImageCut);
    m_ImageCut.BitBlt(pDC->GetSafeHdc(),0, 0, nWidth, nHeight,0,0,RGB(255,255,255));
	
	
	DWORD size=bitmap.bmWidthBytes*bitmap.bmHeight;		//图片数据大小
	LPSTR lpData=(LPSTR)GlobalAllocPtr(GPTR,size);
	int panelsize  = 0;									//记录调色板大小
	if(bitmap.bmBitsPixel<16)							//判断是否为真彩色位图
	{
		int bitPix = bitmap.bmBitsPixel;
		panelsize =  pow((double)2.0, (int)(bitmap.bmBitsPixel *sizeof(RGBQUAD)) );
	}
	BITMAPINFOHEADER *pBInfo = (BITMAPINFOHEADER*)LocalAlloc(LPTR,sizeof(BITMAPINFO)+panelsize);//位图头指针
	pBInfo->biBitCount       = bitmap.bmBitsPixel;		//位图像素
	pBInfo->biClrImportant   = 0;
	pBInfo->biCompression    = 0;
	pBInfo->biHeight         = bitmap.bmHeight;			//位图高
	pBInfo->biPlanes         = bitmap.bmPlanes;
	pBInfo->biSize           = sizeof(BITMAPINFO);
	pBInfo->biSizeImage      = bitmap.bmWidthBytes*bitmap.bmHeight;	//数据
	pBInfo->biWidth          = bitmap.bmWidth;			//位图宽
	pBInfo->biXPelsPerMeter  = 0;
	pBInfo->biYPelsPerMeter  = 0;
	GetDIBits(memDC.m_hDC,bmp,0,pBInfo->biHeight,lpData,
		(BITMAPINFO*)pBInfo,DIB_RGB_COLORS);

	TCHAR pathBuf[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, pathBuf);
	strcat(pathBuf, "\\Save");
	CreateDirectory(pathBuf, NULL);

	CString name,str;
	str.Format("%04d",++m_Num);
	name = "Save\\"+str+".bmp";//设置文件名及路径

	BITMAPFILEHEADER bfh;								//位图文件头
	bfh.bfReserved1 = bfh.bfReserved2 = 0;
	bfh.bfType      = ((WORD)('M'<< 8)|'B');
	bfh.bfSize      = 54+size;
	bfh.bfOffBits   = 54;
	CFile file;
	if(file.Open(name,CFile::modeCreate|CFile::modeWrite))//创建位图文件
	{
		file.Write(&bfh,sizeof(BITMAPFILEHEADER));	//写入位图文件头
		file.Write(pBInfo,sizeof(BITMAPINFOHEADER));//写入文件头
		file.Write(lpData,size);					//写入数据
		file.Close();									//关闭文件
	}

	m_nEndPosX += 8;
}

//绘画界面
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	
	//绘画背景
	DrawViewImage(pDC,m_ImageViewBack,enMode_Centent);
	m_ImageGround.DrawImage(pDC, nWidth/2-782/2, nHeight/2 - 452/2 , 782, m_ImageGround->GetHeight(), m_nPosX,0, 782,  m_ImageGround->GetHeight());

	//if (m_nStartPosX < 450)
	{
		m_ImageStartLine.DrawImage(pDC, nWidth/2-782/2, nHeight/2 - 452/2 , m_ImageStartLine.GetWidth(), m_ImageStartLine.GetHeight(), m_nStartPosX,0);
	}
	m_ImageEndLine.DrawImage(pDC, m_nEndPosX, nHeight/2 - 452/2 , m_ImageEndLine.GetWidth(), m_ImageEndLine.GetHeight(), 0,0);
	
	
	//绘制赛马
	DrawHorse(pDC);
	//绘制每个区域中我的下注
	DrawMeScore(pDC, nWidth, nHeight);
		
	//最大下注  闲可下分
	pDC->SetTextColor(RGB(255,234,0));

	//获取状态
	BYTE cbGameStatus=m_pGameClientDlg->GetGameStatus();

	//下注
	if (cbGameStatus==GS_PLACE_JETTON)
	{		
		int nImageWidth = m_ImageBgJetton.GetWidth();
		int nImageHeight = m_ImageBgJetton.GetHeight();
		m_ImageBgJetton.DrawImage(pDC,nWidth/2-nImageWidth/2, nHeight/2 - nImageHeight/2 - 10,nImageWidth,nImageHeight,0,0);
		
		//下注边框
		FlashJettonAreaFrame(nWidth,nHeight,pDC);

		//筹码资源
		CSize SizeJettonItem(m_ImageJettonView.GetWidth()/JETTON_COUNT,m_ImageJettonView.GetHeight());

		//绘画筹码
		for (INT i=0;i<8;i++)
		{
			//变量定义
			LONGLONG lScoreCount=0L;
			LONGLONG lScoreJetton[JETTON_COUNT]={100L,1000L,10000L,100000L,1000000L,5000000L,10000000L};
			
			//绘画筹码
			for (INT_PTR j=0;j<m_JettonInfoArray[i].GetCount();j++)
			{
				//获取信息
				tagJettonInfo * pJettonInfo=&m_JettonInfoArray[i][j];

				//累计数字
				ASSERT(pJettonInfo->cbJettonIndex<JETTON_COUNT);
				lScoreCount+=lScoreJetton[pJettonInfo->cbJettonIndex];

				//绘画界面
				m_ImageJettonView.DrawImage(pDC,pJettonInfo->nXPos+m_PointJetton[i].x,
					pJettonInfo->nYPos+m_PointJetton[i].y,SizeJettonItem.cx,SizeJettonItem.cy,
					pJettonInfo->cbJettonIndex*SizeJettonItem.cx,0);
			}

			//绘画数字
			if (lScoreCount>0L)	
			{				
				DrawNumberStringWithSpace(pDC,lScoreCount,m_PointJettonNumber[i].x-20,m_PointJettonNumber[i].y+87);
			}else
			{
				DrawNumberStringWithSpace(pDC,0,m_PointJettonNumber[i].x,m_PointJettonNumber[i].y+87);
			}
		}

		DrawMeJettonNumber(pDC);
	}

	
	//绘画时间
	if (m_wMeChairID!=INVALID_CHAIR && cbGameStatus!=GS_GAME_END)
	{
		WORD wUserTimer=GetUserTimer(m_wMeChairID);
		if (wUserTimer!=0) 
			DrawUserTimer(pDC,nWidth/2-60,nHeight/2-280,wUserTimer);
		else
			DrawUserTimer(pDC,nWidth/2-60,nHeight/2-280,0);

		TCHAR str[11];
		sprintf(str,"%d",wUserTimer);
		pDC->SetTextColor(RGB(255,0,0));
		pDC->TextOut(nWidth/2-60,nHeight/2-280,str);
	}
///////////////////////////////////////////////////////////////////////////////////
//调试马的速度
#ifdef _DEBUG
	int b = m_GameLogic.GetNumBigList(m_cbHorseSpeed,HORSE_COUNT);
	DrawNumberStringWithSpace(pDC,b,CRect(150,20 -10,300,30-5));
	for (INT i=0;i<8;i++)
	{	
		int a = m_GameLogic.GetNumListPip(m_cbHorseSpeed[i],8);
		DrawNumberStringWithSpace(pDC,a,CRect(120,20 +i*15,300,30+i*15));

		for (INT j=0;j<8;j++)
		{
			int c = m_cbHorseSpeed[i][j];
			DrawNumberStringWithSpace(pDC,c,CRect(150+j*15,20 +i*15,300+j*15,30+i*15));
		}
	}
//#else
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//庄家信息																											
	pDC->SetTextColor(RGB(255,234,0));

	//获取玩家
	tagUserData const *pUserData = m_wBankerUser==INVALID_CHAIR ? NULL : GetUserInfo(m_wBankerUser);

	//位置信息
	CRect StrRect;
	StrRect.left = nWidth/2-330;
	StrRect.top = nHeight/2 - 330;
	StrRect.right = StrRect.left + 190;
	StrRect.bottom = StrRect.top + 15;

	//庄家名字
	pDC->DrawText(pUserData==NULL?(m_bEnableSysBanker?TEXT("系统坐庄"):TEXT("无人坐庄")):pUserData->szName, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP| DT_SINGLELINE );

	//庄家总分
	CString strBankerTotalScore;
	StrRect.left = nWidth/2-330;
	StrRect.top = nHeight/2 - 310;
	StrRect.right = StrRect.left + 190;
	StrRect.bottom = StrRect.top + 15;	
	DrawNumberStringWithSpace(pDC,pUserData==NULL?0:pUserData->lScore, StrRect);

	//庄家局数
	CString strBankerTime;
	strBankerTime.Format( "%d", m_wBankerTime );

	StrRect.left = nWidth/2-330;
	StrRect.top = nHeight/2 - 260;
	StrRect.right = StrRect.left + 190;
	StrRect.bottom = StrRect.top + 15;	
	DrawNumberStringWithSpace(pDC,m_wBankerTime,StrRect);

	//庄家成绩
	CString strBankerScore;
	strBankerScore.Format( "%I64d", m_lBankerWinScore);

	StrRect.left = nWidth/2-330;
	StrRect.top = nHeight/2 - 284;
	StrRect.right = StrRect.left + 190;
	StrRect.bottom = StrRect.top + 15;	
	DrawNumberStringWithSpace(pDC,m_lBankerWinScore,StrRect);
//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//x-30  y-10
	//绘画用户
	if (m_wMeChairID!=INVALID_CHAIR)
	{
		const tagUserData *pMeUserData = GetUserInfo(m_wMeChairID);
		if ( pMeUserData != NULL )
		{
			m_lMeCurJettonScore = m_lMeScore2+m_lMeScore3+m_lMeScore1+m_lMeScore6+m_lMeScore5+m_lMeScore4+m_lMeScore7+m_lMeScore8;
			//游戏信息
			TCHAR szResultScore[16]=TEXT("");
			TCHAR szGameScore[16]=TEXT("");
			pDC->SetTextColor(RGB(255,255,255));
			
			CRect rcAccount(nWidth/2-310,nHeight/2+235,nWidth/2-310+90,nHeight/2+255);
			CRect rcGameScore(nWidth/2-310,nHeight/2+260,nWidth/2-310+90,nHeight/2+257+20);
			CRect rcResultScore(nWidth/2-310,nHeight/2+288,nWidth/2-310+90,nHeight/2+283+20);
			CRect rcMeJetton(nWidth/2-310,nHeight/2+313,nWidth/2-310+90,nHeight/2+308+20);
			DrawNumberStringWithSpace(pDC,m_lMeCurJettonScore,rcMeJetton);
			DrawNumberStringWithSpace(pDC,pMeUserData->lScore -m_lMeCurJettonScore,rcGameScore);
			DrawNumberStringWithSpace(pDC,m_lMeStatisticScore,rcResultScore);
			pDC->DrawText(pMeUserData->szName,lstrlen(pMeUserData->szName),rcAccount,DT_END_ELLIPSIS|DT_LEFT|DT_TOP|DT_SINGLELINE);
		}
	}

	//切换庄家
	if ( m_bShowChangeBanker )
	{
		int	nXPos = nWidth / 2 - 130;
		int	nYPos = nHeight / 2 - 160;

		//由我做庄
		if ( m_wMeChairID == m_wBankerUser )
		{
			//CImageHandle ImageHandleBanker(&m_ImageMeBanker);
			m_ImageMeBanker.DrawImage(pDC, nXPos, nYPos);
		}
		else if ( m_wBankerUser != INVALID_CHAIR )
		{
			//CImageHandle ImageHandleBanker(&m_ImageChangeBanker);
			m_ImageChangeBanker.DrawImage(pDC, nXPos, nYPos);
		}
		else
		{
			//CImageHandle ImageHandleBanker(&m_ImageNoBanker);
			m_ImageNoBanker.DrawImage(pDC, nXPos, nYPos);
		}
	}

	if (m_bCutPhoto)
		CutPhoto(nWidth ,nHeight,pDC);

	//胜利标志
	DrawWinFlags(pDC);

	//显示结果
	ShowGameResult(pDC, nWidth, nHeight);	

	//爆炸效果
	//DrawBombEffect(pDC);
	

	//CPoint nPoint;
	//GetCursorPos(&nPoint);
	//ScreenToClient(&nPoint);
	//TCHAR str[100];
	//sprintf(str,"x = %d;\ny = %d",nPoint.x,nPoint.y);
	//pDC->TextOut(20,100,str);

	return;
}

//设置信息
void CGameClientView::SetMeMaxScore(LONGLONG lMeMaxScore)
{
	if (m_lMeMaxScore!=lMeMaxScore)
	{
		//设置变量
		m_lMeMaxScore=lMeMaxScore;
	}

	return;
}

//最大下注
void CGameClientView::SetAreaLimitScore(LONGLONG lAreaLimitScore)
{
	if ( m_lAreaLimitScore!= lAreaLimitScore )
	{
		//设置变量
		m_lAreaLimitScore=lAreaLimitScore;
	}
}

//设置筹码
void CGameClientView::SetCurrentJetton(LONGLONG lCurrentJetton)
{
	//设置变量
	ASSERT(lCurrentJetton>=0L);
	m_lCurrentJetton=lCurrentJetton;

	return;
}

//历史记录
void CGameClientView::SetGameHistory(enOperateResult OperateResult, BYTE cbWinner)
{
	//设置数据
	tagClientGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	GameRecord.enOperateFlags = OperateResult;
	GameRecord.cbWinner = cbWinner;

	//移动下标
	m_nRecordLast = (m_nRecordLast+1) % MAX_C_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst )
	{
		m_nRecordFirst = (m_nRecordFirst+1) % MAX_C_SCORE_HISTORY;
		if ( m_nScoreHead < m_nRecordFirst ) m_nScoreHead = m_nRecordFirst;
	}

	int nHistoryCount = (m_nRecordLast - m_nRecordFirst + MAX_C_SCORE_HISTORY) % MAX_C_SCORE_HISTORY;
	if ( MAX_HISTORY_NUM < nHistoryCount ) m_btScoreMoveR.EnableWindow(TRUE);

	//移到最新记录
	if ( MAX_HISTORY_NUM < nHistoryCount )
	{
		m_nScoreHead = (m_nRecordLast - MAX_HISTORY_NUM + MAX_C_SCORE_HISTORY) % MAX_C_SCORE_HISTORY;
		m_btScoreMoveL.EnableWindow(TRUE);
		m_btScoreMoveR.EnableWindow(FALSE);
	}

	////更新路子
	//if (m_DlgViewChart.m_hWnd!=NULL)
	//{
	//	m_DlgViewChart.SetGameRecord(GameRecord);
	//}

	return;
}

//清理筹码
void CGameClientView::CleanUserJetton()
{
	//清理数组
	for (BYTE i=0;i<CountArray(m_JettonInfoArray);i++)
	{
		m_JettonInfoArray[i].RemoveAll();
	}

	m_bFinish = false;
	m_bCutPhoto = false;

	//下注信息
	m_lMeScore2=0L;
	m_lMeScore3=0L;
	m_lMeScore1=0L;
	m_lMeScore6=0L;
	m_lMeScore5=0L;
	m_lMeScore4=0L;
	m_lMeScore7=0L;
	m_lMeScore8=0L;
	
	//全体下注
	m_lAllScore2=0L;
	m_lAllScore3=0L;
	m_lAllScore1=0L;
	m_lAllScore6=0L;
	m_lAllScore5=0L;
	m_lAllScore4=0L;
	m_lAllScore7=0L;
	m_lAllScore8=0L;
	
	//更新界面
	UpdateGameView(NULL);

	return;
}

//个人下注
void CGameClientView::SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
{
	//效验参数
	ASSERT(cbViewIndex<=ID_HORSE8);
	if (cbViewIndex>ID_HORSE8) return;

	if (cbViewIndex==ID_HORSE1) m_lMeScore1=lJettonCount;
	else if (cbViewIndex==ID_HORSE2) m_lMeScore2=lJettonCount;
	else if (cbViewIndex==ID_HORSE3) m_lMeScore3=lJettonCount;
	else if (cbViewIndex==ID_HORSE4) m_lMeScore4=lJettonCount;
	else if (cbViewIndex==ID_HORSE5) m_lMeScore5=lJettonCount;
	else if (cbViewIndex==ID_HORSE6) m_lMeScore6=lJettonCount;
	else if (cbViewIndex==ID_HORSE7) m_lMeScore7=lJettonCount;
	else if (cbViewIndex==ID_HORSE8) m_lMeScore8=lJettonCount;
	else {ASSERT(FALSE);return;}

	
	//更新界面
	UpdateGameView(NULL);
}

void CGameClientView::SetFreeTime()
{
	SetTimer(IDI_HORSE_STATIC,100,0);
	InitHorseInfo();
	InitSignInfo();
} 

//设置
void CGameClientView::SetHorseSpeed(BYTE cbTableCardArray[8][8])
{
	SetTimer(IDI_HORSE_EFFECT,80,NULL);
	CopyMemory(m_cbHorseSpeed,cbTableCardArray,sizeof(m_cbHorseSpeed));

	//推断赢家
	BYTE cbWinner = m_GameLogic.GetNumBigList(m_cbHorseSpeed,HORSE_COUNT);
	//TCHAR a[111];
	//sprintf(a,"%d",cbWinner);
	//MessageBox(a);
	for (int i=0; i< HORSE_COUNT; i++)
	{
		m_Horse[i].bStatic = false;
		m_Horse[i].nSpeed = m_cbHorseSpeed[i][0];

	}
}

//结束赛跑
void CGameClientView::FinishRun()
{
	//删除定时器
	KillTimer(IDI_HORSE_EFFECT);
	SetTimer(IDI_HORSE_STATIC,100,0);

	//设置记录
	if (m_pGameClientDlg->GetGameStatus()==GS_GAME_END)
	{
		//操作类型
		enOperateResult OperateResult = enOperateResult_NULL;
		if (0 < m_lMeCurGameScore) OperateResult = enOperateResult_Win;
		else if (m_lMeCurGameScore < 0) OperateResult = enOperateResult_Lost;
		else OperateResult = enOperateResult_NULL;

		//推断赢家
		BYTE cbWinner = m_GameLogic.GetNumBigList(m_cbHorseSpeed,HORSE_COUNT);		

		//保存记录
		SetGameHistory(OperateResult, cbWinner);

		//累计积分
		m_lMeStatisticScore+=m_lMeCurGameScore;
		m_lBankerWinScore=m_lTmpBankerWinScore;

		LONGLONG lScore[] = {m_lMeScore1,m_lMeScore2,m_lMeScore3,m_lMeScore4,m_lMeScore5,m_lMeScore6,m_lMeScore7,m_lMeScore8};
		LONGLONG lResult[8] = {0};
		for (int i=0;i<8;i++)
		{
			if (i == cbWinner-1)
			{
				continue;
			}
			lResult[i] = -lScore[i];
		}
		lResult[cbWinner-1] = lScore[cbWinner-1]*7;

		TCHAR szTipMsg[1024];
		_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("\n  1号区域：%I64d \n  2号区域：%I64d   \n  3号区域：%I64d \n  4号区域：%I64d \n  5号区域：%I64d \n  6号区域：%I64d \n  7号区域：%I64d \n  8号区域：%I64d\n  返还下注：%I64d"),lResult[0],lResult[1],lResult[2],lResult[3],lResult[4],lResult[5],lResult[6],lResult[7],lScore[cbWinner-1]);
		//消息积分
		m_pGameClientDlg->InsertSystemString(szTipMsg);
	}

}

//设置筹码
void CGameClientView::PlaceUserJetton(BYTE cbViewIndex, LONGLONG lScoreCount)
{
	//效验参数
	ASSERT(cbViewIndex<=ID_HORSE8);
	if (cbViewIndex>ID_HORSE8) return;

	//设置炸弹
	if (lScoreCount==5000000L || lScoreCount==10000000L) SetBombEffect(true,cbViewIndex);	

	//变量定义
	bool bPlaceJetton=false;
	LONGLONG lScoreIndex[JETTON_COUNT]={100L,1000L,10000L,100000L,1000000L,5000000L,10000000L};

	//边框宽度
	int nFrameWidth=0, nFrameHeight=0;
	int nBorderWidth=6;

	switch (cbViewIndex)
	{
	case ID_HORSE1:
		{ 
			m_lAllScore1 += lScoreCount;
			nFrameWidth = m_rcFrame1.right-m_rcFrame1.left;
			nFrameHeight = m_rcFrame1.bottom-m_rcFrame1.top;
			break;
		}
	case ID_HORSE2:
		{
			m_lAllScore2 += lScoreCount;
			nFrameWidth = m_rcFrame2.right-m_rcFrame2.left;
			nFrameHeight = m_rcFrame2.bottom-m_rcFrame2.top;
			break;
		}
	case ID_HORSE3:
		{ 
			m_lAllScore3 += lScoreCount;
			nFrameWidth = m_rcFrame3.right-m_rcFrame3.left;
			nFrameHeight = m_rcFrame3.bottom-m_rcFrame3.top;
			break;
		}
	case ID_HORSE4:
		{ 
			m_lAllScore4 += lScoreCount;
			nFrameWidth = m_rcFrame4.right-m_rcFrame4.left;
			nFrameHeight = m_rcFrame4.bottom-m_rcFrame4.top;
			break;
		}
	case ID_HORSE5:
		{ 
			m_lAllScore5 += lScoreCount;
			nFrameWidth = m_rcFrame5.right-m_rcFrame5.left;
			nFrameHeight = m_rcFrame5.bottom-m_rcFrame5.top;
			break;
		}
	case ID_HORSE6:
		{ 
			m_lAllScore6 += lScoreCount;
			nFrameWidth = m_rcFrame6.right-m_rcFrame6.left;
			nFrameHeight = m_rcFrame6.bottom-m_rcFrame6.top;
			break;
		}
	case ID_HORSE7:
		{ 
			m_lAllScore7 += lScoreCount;
			nFrameWidth = m_rcFrame7.right-m_rcFrame7.left;
			nFrameHeight = m_rcFrame7.bottom-m_rcFrame7.top;
			break;
		}
	case ID_HORSE8:
		{ 
			m_lAllScore8 += lScoreCount;
			nFrameWidth = m_rcFrame8.right-m_rcFrame8.left;
			nFrameHeight = m_rcFrame8.bottom-m_rcFrame8.top;
			break;
		}
	default:
		{
			ASSERT(FALSE);
			return;
		}
	}

	nFrameWidth += nBorderWidth;
	nFrameHeight += nBorderWidth;

	//增加筹码
	for (BYTE i=0;i<CountArray(lScoreIndex);i++)
	{
		//计算数目
		BYTE cbScoreIndex=JETTON_COUNT-i-1;
		LONGLONG lCellCount=lScoreCount/lScoreIndex[cbScoreIndex];

		//插入过虑
		if (lCellCount==0L) continue;

		//加入筹码
		for (LONGLONG j=0;j<lCellCount;j++)
		{
			//构造变量
			tagJettonInfo JettonInfo;
			int nJettonSize=68;
			JettonInfo.cbJettonIndex=cbScoreIndex;
			JettonInfo.nXPos=rand()%(nFrameWidth-nJettonSize);
			JettonInfo.nYPos=rand()%(nFrameHeight-nJettonSize);

			//插入数组
			bPlaceJetton=true;
			m_JettonInfoArray[cbViewIndex-1].Add(JettonInfo);
		}

		//减少数目
		lScoreCount-=lCellCount*lScoreIndex[cbScoreIndex];
	}

	//更新界面
	if (bPlaceJetton==true) UpdateGameView(NULL);

	return;
}

//当局成绩
void CGameClientView::SetCurGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore, LONGLONG lGameRevenue)
{
	m_lMeCurGameScore=lMeCurGameScore;			
	m_lMeCurGameReturnScore=lMeCurGameReturnScore;			
	m_lBankerCurGameScore=lBankerCurGameScore;			
	m_lGameRevenue=lGameRevenue;					
}

//设置胜方
void CGameClientView::SetWinnerSide(BYTE cbWinnerSide)
{
	//设置变量
	m_cbWinnerSide=cbWinnerSide;
	m_cbAreaFlash=cbWinnerSide;

	//设置时间
	if (cbWinnerSide!=0xFF)
	{
		SetTimer(IDI_FLASH_WINNER,500,NULL);
	}
	else KillTimer(IDI_FLASH_WINNER);

	//更新界面
	UpdateGameView(NULL);

	return;
}

//获取区域
BYTE CGameClientView::GetJettonArea(CPoint MousePoint)
{
	if (m_rcFrame1.PtInRect(MousePoint)) return ID_HORSE1;
	if (m_rcFrame2.PtInRect(MousePoint)) return ID_HORSE2;
	if (m_rcFrame3.PtInRect(MousePoint)) return ID_HORSE3;
	if (m_rcFrame4.PtInRect(MousePoint)) return ID_HORSE4;	
	if (m_rcFrame5.PtInRect(MousePoint)) return ID_HORSE5;	
	if (m_rcFrame6.PtInRect(MousePoint)) return ID_HORSE6;
	if (m_rcFrame7.PtInRect(MousePoint)) return ID_HORSE7;
	if (m_rcFrame8.PtInRect(MousePoint)) return ID_HORSE8;

	return 0xFF;
}

//绘画数字
void CGameClientView::DrawNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos, bool bMeScore)
{
	//加载资源
	CSize SizeScoreNumber(m_ImageMeScoreNumber.GetWidth()/10, m_ImageMeScoreNumber.GetHeight());

	//计算数目
	LONGLONG lNumberCount=0;
	LONGLONG lNumberTemp=lNumber;
	do
	{
		lNumberCount++;
		lNumberTemp/=10;
	} while (lNumberTemp>0);

	//位置定义
	INT nYDrawPos=nYPos-SizeScoreNumber.cy/2;
	INT nXDrawPos=nXPos+(INT)lNumberCount*SizeScoreNumber.cx/2-SizeScoreNumber.cx;

	//绘画桌号
	for (LONGLONG i=0;i<lNumberCount;i++)
	{
		//绘画号码
		LONGLONG lCellNumber=(LONGLONG)(lNumber%10);

		m_ImageMeScoreNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,(INT)lCellNumber*SizeScoreNumber.cx,0);

		//设置变量
		lNumber/=10;
		nXDrawPos-=SizeScoreNumber.cx;
	};

	return;
}

//绘画数字
void CGameClientView::DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos)
{
	CString strNumber=TEXT(""), strTmpNumber1,strTmpNumber2;
	if (lNumber==0) strNumber=TEXT("0");
	int nNumberCount=0;
	LONGLONG lTmpNumber=lNumber;
	if (lNumber<0) lNumber=-lNumber;
	while (lNumber>0)
	{
		strTmpNumber1.Format(TEXT("%I64d"),lNumber%10);
		nNumberCount++;
		strTmpNumber2 = strTmpNumber1+strTmpNumber2;

		if (nNumberCount==4)
		{
			strTmpNumber2 += (TEXT(" ") +strNumber);
			strNumber=strTmpNumber2;
			nNumberCount=0;
			strTmpNumber2=TEXT("");
		}
		lNumber/=10;
	}

	if (strTmpNumber2.IsEmpty()==FALSE)
	{
		strTmpNumber2 += (TEXT(" ") +strNumber);
		strNumber=strTmpNumber2;
	}

	if (lTmpNumber<0) strNumber=TEXT("-")+strNumber;

	//输出数字
	pDC->TextOut(nXPos,nYPos,strNumber);
}

//绘画数字
void CGameClientView::DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, CRect rcRect, INT nFormat)
{
	CString strNumber=TEXT(""), strTmpNumber1,strTmpNumber2;
	if (lNumber==0) strNumber=TEXT("0");
	int nNumberCount=0;
	LONGLONG lTmpNumber=lNumber;
	if (lNumber<0) lNumber=-lNumber;
	while (lNumber>0)
	{
		strTmpNumber1.Format(TEXT("%I64d"),lNumber%10);
		nNumberCount++;
		strTmpNumber2 = strTmpNumber1+strTmpNumber2;

		if (nNumberCount==4)
		{
			strTmpNumber2 += (TEXT(" ") +strNumber);
			strNumber=strTmpNumber2;
			nNumberCount=0;
			strTmpNumber2=TEXT("");
		}
		lNumber/=10;
	}

	if (strTmpNumber2.IsEmpty()==FALSE)
	{
		strTmpNumber2 += (TEXT(" ") +strNumber);
		strNumber=strTmpNumber2;
	}

	if (lTmpNumber<0) strNumber=TEXT("-")+strNumber;

	pDC->SetTextColor(RGB(255,234,0));
	//输出数字
	if (nFormat==-1) pDC->DrawText(strNumber,rcRect,DT_END_ELLIPSIS|DT_LEFT|DT_TOP|DT_SINGLELINE);
	else pDC->DrawText(strNumber,rcRect,nFormat);
}

//筹码按钮
void CGameClientView::OnJettonButton100()
{
	//设置变量
	m_lCurrentJetton=100L;

	return;
}

//筹码按钮
void CGameClientView::OnJettonButton1000()
{
	//设置变量
	m_lCurrentJetton=1000L;

	return;
}

//筹码按钮
void CGameClientView::OnJettonButton10000()
{
	//设置变量
	m_lCurrentJetton=10000L;

	return;
}

//筹码按钮
void CGameClientView::OnJettonButton100000()
{
	//设置变量
	m_lCurrentJetton=100000L;

	return;
}

//筹码按钮
void CGameClientView::OnJettonButton1000000()
{
	//设置变量
	m_lCurrentJetton=1000000L;

	return;
}
//筹码按钮
void CGameClientView::OnJettonButton5000000()
{
	//设置变量
	m_lCurrentJetton=5000000L;

	return;
}

//筹码按钮
void CGameClientView::OnJettonButton10000000()
{
	//设置变量
	m_lCurrentJetton=10000000L;

	return;
}

//查看路子
void CGameClientView::OnViewChart()
{
	//if (m_DlgViewChart.m_hWnd==NULL) m_DlgViewChart.Create(IDD_VIEW_CHART,this);

	////显示判断
	//if (!m_DlgViewChart.IsWindowVisible())
	//{
	//	m_DlgViewChart.ShowWindow(SW_SHOW);
	//	m_DlgViewChart.CenterWindow();
	//}
	//else
	//{
	//	m_DlgViewChart.ShowWindow(SW_HIDE);
	//}
}

//定时器消息
void CGameClientView::OnTimer(UINT nIDEvent)
{
	CRect rtClient;
	GetClientRect(&rtClient);
	
	switch(nIDEvent)
	{
	
	case IDI_SHOW_CHANGE_BANKER:
		{
			ShowChangeBanker( false );
			break;
		}	
	case IDI_HORSE_STATIC:
		{
			
			//KillTimer(IDI_HORSE_EFFECT);
			for (int i=0; i < HORSE_COUNT; i++)
			{
				m_Horse[i].bStatic = true;
				if (m_Horse[i].bStatic)
				{
					m_Horse[i].cbHorseIndex ++;
					if (m_Horse[i].cbHorseIndex > 11)
					{
						m_Horse[i].cbHorseIndex = 0;
					}

				}
			}
			break;
		}
	case IDI_HORSE_EFFECT:
		{
			KillTimer(IDI_HORSE_STATIC);

			BYTE cbMinIndex = GetFastOrLater(false);//最后一名的下标				

			//最后一名到达就结束
			if (m_Horse[cbMinIndex].nXPos > 780)
			{
				m_bFinish = true;
				FinishRun();
			}	

			BYTE cbMaxIndex = GetFastOrLater(true);//第一名的下标
			BYTE cbTemp = 0;
			if (m_nSprint <= 6&&!m_Horse[cbMaxIndex].bStatic&&(m_Horse[cbMaxIndex].nXPos >=rtClient.Width()/2-130))
			{	
				cbTemp = m_Horse[cbMaxIndex].nSpeed;

				for (int i=0; i < HORSE_COUNT; i++)
					m_Horse[i].nSpeed -= cbTemp;
			}

			float fPosX = 0.f;
			
			if (m_Horse[cbMaxIndex].nXPos >= 600-140)
			{				
				fPosX = m_Sign[cbMaxIndex].fXPos - 1.5f;
				
				for (int i=0; i < HORSE_COUNT; i++)
				{					
					m_Horse[i].nSpeed = 15;
					m_Sign[i].fSpeed = m_Horse[i].nSpeed/10.f;
				}
				if (m_Horse[cbMaxIndex].nXPos < 600-140 + 10)
					m_bCutPhoto = true;
				else
					m_bCutPhoto = false;				
			}			

			if (m_nSprint<2)
			{
				m_nPosX += 10;
				m_nStartPosX += 10;
			}else if (m_nSprint<3)
			{
				m_nPosX += 15;
				m_nStartPosX += 15;
			}else if (m_nSprint<4)
			{
				m_nPosX += 20;
			}
			else if (m_nSprint<5)
			{
				m_nPosX += 16;
			}
			else if (m_nSprint<=6)
			{
				m_nPosX += 10;	
				m_nEndPosX -= 10; 			
			}
			else if (m_nSprint<=6)
			{
				m_nPosX += 10;	
				m_nEndPosX -= 10; 			
			}


			if (m_nPosX > 1500-780)
			{
				m_nPosX = 0;
			}

			//暂停1S
			if (m_Horse[cbMaxIndex].nXPos >= (600-130 + 25) && m_Horse[cbMaxIndex].nXPos < 600-130 + 40)
			{				
				UpdateGameView(NULL);
				Sleep(1000);
				m_pGameClientDlg->PlayGameSound(RACE_OVER);
			}

			for (int i=0; i < HORSE_COUNT; i++)
			{
				if (m_Horse[i].nXPos >= 780)
					m_Sign[i].fXPos = fPosX;
				m_Sign[i].fXPos += m_Sign[i].fSpeed;

				m_Horse[i].bStatic = false;
				m_Horse[i].nXPos += m_Horse[i].nSpeed;

				//动画帧序列
				m_Horse[i].cbHorseIndex1 ++;		
				if (m_Horse[i].cbHorseIndex1 > 7)
				{
					m_Horse[i].cbHorseIndex1 = 0;
				}

				if (m_Horse[i].nXPos > 780)
				{
					m_Horse[i].nSpeed = 0;
				}				

			}

			if (m_cbBlowIndex++>=7)
			{
				m_cbBlowIndex = 0;
			}
			break;
		}
	}

	

	//爆炸动画
	if (nIDEvent<=IDI_BOMB_EFFECT+ID_HORSE8 && IDI_BOMB_EFFECT+ID_HORSE1<=nIDEvent)
	{
		WORD wIndex=nIDEvent-IDI_BOMB_EFFECT;
		//停止判断
		if (m_bBombEffect[wIndex]==false)
		{
			KillTimer(nIDEvent);
			return;
		}

		//设置变量
		if ((m_cbBombFrameIndex[wIndex]+1)>=BOMB_EFFECT_COUNT)
		{
			//删除时间
			KillTimer(nIDEvent);

			//设置变量
			m_bBombEffect[wIndex]=false;
			m_cbBombFrameIndex[wIndex]=0;
		}
		else m_cbBombFrameIndex[wIndex]++;
	}
	
	UpdateGameView(NULL);
	
	__super::OnTimer(nIDEvent);
}

//void Standardization()
//{
//
//}

//鼠标消息
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	if (m_lCurrentJetton!=0L)
	{
		BYTE cbJettonArea=GetJettonArea(Point);

		//大小判断
		switch( cbJettonArea )
		{
		case ID_HORSE1:
			{
				LONGLONG lMaxPlayerScore = GetMaxHorse1();
				if ( lMaxPlayerScore < m_lCurrentJetton )
					return ;
				break;
			}
		case ID_HORSE2:
			{
				LONGLONG lMaxTieScore  = GetMaxHorse2();
				if ( lMaxTieScore < m_lCurrentJetton )
					return ;
				break;
			}
		case ID_HORSE3:
			{
				LONGLONG lMaxBankerScore = GetMaxHorse3();
				if ( lMaxBankerScore < m_lCurrentJetton )
					return ;
				break;
			}
		case ID_HORSE6:
			{
				LONGLONG lMaxTieKingScore= GetMaxHorse6();
				if ( lMaxTieKingScore < m_lCurrentJetton )
					return ;
				break;
			}
		case ID_HORSE4:
			{
				LONGLONG lMaxPlayerKingScore = GetMaxHorse4();
				if ( lMaxPlayerKingScore < m_lCurrentJetton )
					return ;
				break;
			}
		case ID_HORSE5:
			{
				LONGLONG lMaxBankerKingScore = GetMaxHorse5();
				if ( lMaxBankerKingScore < m_lCurrentJetton )
					return ;
				break;
			}
		case ID_HORSE7:
			{
				LONGLONG lMaxPlayerTwoPair=GetMaxHorse7();
				if (lMaxPlayerTwoPair<m_lCurrentJetton)
					return ;
				break;
			}
		case ID_HORSE8:
			{
				LONGLONG lMaxBankerTwoPair=GetMaxHorse8();
				if (lMaxBankerTwoPair<m_lCurrentJetton)
					return ;
				break;
			}
		}

		if (cbJettonArea!=0xFF) AfxGetMainWnd()->SendMessage(IDM_PLACE_JETTON,cbJettonArea,(long)m_lCurrentJetton);
	}

	__super::OnLButtonDown(nFlags,Point);
}

//鼠标消息
void CGameClientView::OnRButtonDown(UINT nFlags, CPoint Point)
{
	//设置变量
	m_lCurrentJetton=0L;

	if (m_pGameClientDlg->GetGameStatus()!=GS_GAME_END && m_cbAreaFlash!=0xFF)
	{
		m_cbAreaFlash=0xFF;
		UpdateGameView(NULL);
	}

	__super::OnLButtonDown(nFlags,Point);
}

//光标消息
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	if (m_lCurrentJetton!=0L)
	{
		//获取区域
		CPoint MousePoint;
		GetCursorPos(&MousePoint);
		ScreenToClient(&MousePoint);
		BYTE cbJettonArea=GetJettonArea(MousePoint);

		//设置变量
		if ( m_cbAreaFlash!= cbJettonArea )
		{
			m_cbAreaFlash = cbJettonArea;
			UpdateGameView(NULL);
		}

		//区域判断
		if (cbJettonArea==0xFF)
		{
			SetCursor(LoadCursor(NULL,IDC_NO));
			return TRUE;
		}

		//大小判断
		switch( cbJettonArea )
		{
		case ID_HORSE1:
			{
				LONGLONG lMaxPlayerScore = GetMaxHorse1();
				if ( lMaxPlayerScore < m_lCurrentJetton )
				{
					SetCursor(LoadCursor(NULL,IDC_NO));
					return TRUE;
				}
				break;
			}
		case ID_HORSE2:
			{
				LONGLONG lMaxTieScore  = GetMaxHorse2();
				if ( lMaxTieScore < m_lCurrentJetton )
				{
					SetCursor(LoadCursor(NULL,IDC_NO));
					return TRUE;
				}
				break;
			}
		case ID_HORSE3:
			{
				LONGLONG lMaxBankerScore = GetMaxHorse3();
				if ( lMaxBankerScore < m_lCurrentJetton  )
				{
					SetCursor(LoadCursor(NULL,IDC_NO));
					return TRUE;
				};
				break;
			}
		case ID_HORSE4:
			{
				LONGLONG lMaxPlayerKingScore = GetMaxHorse4();
				if ( lMaxPlayerKingScore < m_lCurrentJetton )
				{
					SetCursor(LoadCursor(NULL,IDC_NO));
					return TRUE;
				}
				break;
			}
		case ID_HORSE5:
			{
				LONGLONG lMaxBankerKingScore = GetMaxHorse5();
				if ( lMaxBankerKingScore < m_lCurrentJetton )
				{
					SetCursor(LoadCursor(NULL,IDC_NO));
					return TRUE;
				}
				break;
			}
		case ID_HORSE6:
			{
				LONGLONG lMaxTieKingScore= GetMaxHorse6();
				if ( lMaxTieKingScore < m_lCurrentJetton )
				{
					SetCursor(LoadCursor(NULL,IDC_NO));
					return TRUE;
				}
				break;
			}
		

		case ID_HORSE7:
			{
				LONGLONG lMaxHorse7Score = GetMaxHorse7();
				if ( lMaxHorse7Score < m_lCurrentJetton )
				{
					SetCursor(LoadCursor(NULL,IDC_NO));
					return TRUE;
				}
				break;
			}
		case ID_HORSE8:
			{
				LONGLONG lMaxHorse8Score = GetMaxHorse8();
				if ( lMaxHorse8Score < m_lCurrentJetton )
				{
					SetCursor(LoadCursor(NULL,IDC_NO));
					return TRUE;
				}
				break;
			}
		}


		//设置光标
		switch (m_lCurrentJetton)
		{
		case 100:
			{
				SetCursor(LoadCursorFromFile(IDC_SCORE_100));
				return TRUE;
			}
		case 1000:
			{
				SetCursor(LoadCursorFromFile(IDC_SCORE_1000));
				return TRUE;
			}
		case 10000:
			{
				SetCursor(LoadCursorFromFile(IDC_SCORE_10000));
				return TRUE;
			}
		case 100000:
			{
				SetCursor(LoadCursorFromFile(IDC_SCORE_100000));
				return TRUE;
			}
		case 1000000:
			{
				SetCursor(LoadCursorFromFile(IDC_SCORE_1000000));
				return TRUE;
			}
		case 5000000:
			{
				SetCursor(LoadCursorFromFile(IDC_SCORE_5000000));
				return TRUE;
			}	
		case 10000000:
			{
				SetCursor(LoadCursorFromFile(IDC_SCORE_10000000));
				return TRUE;
			}
		}
	}

	return __super::OnSetCursor(pWnd, nHitTest, uMessage);
}

//轮换庄家
void CGameClientView::ShowChangeBanker( bool bChangeBanker )
{
	//轮换庄家
	if ( bChangeBanker )
	{
		SetTimer( IDI_SHOW_CHANGE_BANKER, 3000, NULL );
		m_bShowChangeBanker = true;
	}
	else
	{
		KillTimer( IDI_SHOW_CHANGE_BANKER );
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

	return;
}
//庄家信息
void CGameClientView::SetBankerInfo(WORD wBankerUser, LONGLONG lBankerScore) 
{
	//切换判断
	if (m_wBankerUser!=wBankerUser)
	{
		m_wBankerUser=wBankerUser;
		m_wBankerTime=0L;
		m_lBankerWinScore=0L;	
		m_lTmpBankerWinScore=0L;
	}
	m_lBankerScore=lBankerScore;
}

//绘画标识
void CGameClientView::DrawWinFlags(CDC * pDC)
{
	//CImageHandle ImageHandleWinFlags(&m_ImageWinFlags);
	int nIndex = m_nScoreHead;
	COLORREF clrOld ;
	clrOld = pDC->SetTextColor(RGB(255,234,0));
	CString strPoint;
	int nDrawCount = 0;
	while ( nIndex != m_nRecordLast && ( m_nRecordLast!=m_nRecordFirst ) && nDrawCount < MAX_HISTORY_NUM )
	{
		tagClientGameRecord &ClientGameRecord = m_GameRecordArrary[nIndex];
		int nYPos = m_nWinFlagsExcursionY;
		
		int nXPos = m_nWinFlagsExcursionX + ((nIndex - m_nScoreHead + MAX_C_SCORE_HISTORY) % MAX_C_SCORE_HISTORY) * 36 + 3;
		int nFlagsIndex = 0;
		nFlagsIndex = ClientGameRecord.cbWinner;

		m_ImageWinFlags.DrawImage( pDC, nXPos, nYPos, m_ImageWinFlags.GetWidth()/8 , m_ImageWinFlags.GetHeight(),m_ImageWinFlags.GetWidth()/8 * (nFlagsIndex-1), 0);

		nDrawCount++;
		nIndex = (nIndex+1) % MAX_C_SCORE_HISTORY;
	}
	pDC->SetTextColor(clrOld);

	////统计个数
	//nIndex = m_nRecordFirst;
	//int nPlayerCount = 0, nBankerCount = 0, nTieCount = 0;
	//while ( nIndex != m_nRecordLast && ( m_nRecordLast!=m_nRecordFirst ))
	//{
	//	//tagClientGameRecord &ClientGameRecord = m_GameRecordArrary[nIndex];
	//	//if ( ClientGameRecord.cbBankerCount < ClientGameRecord.cbWinner ) nPlayerCount++;
	//	//else if ( ClientGameRecord.cbBankerCount == ClientGameRecord.cbWinner ) nTieCount++;
	//	//else nBankerCount++;

	//	nIndex = (nIndex+1) % MAX_C_SCORE_HISTORY;
	//}

	//int nWidth, nHeight;
	//CRect rcClient;
	//GetClientRect(&rcClient);
	//nWidth = rcClient.Width();
	//nHeight = rcClient.Height();
	//CFont font;
	//VERIFY(font.CreateFont(25,0,0,0,FW_NORMAL,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS,"Arial"));                

	//pDC->SetTextColor(RGB(255,255,255));
	//CFont* def_font = pDC->SelectObject(&font);
	//strPoint.Format(TEXT("%d"), nBankerCount);
	//pDC->TextOut(nWidth/2-350+5*120+78, nHeight/2+257, strPoint);
	//strPoint.Format(TEXT("%d"), nPlayerCount);
	//pDC->TextOut(nWidth/2-350+5*120+78, nHeight/2+288, strPoint);
	//strPoint.Format(TEXT("%d"), nTieCount);
	//pDC->TextOut(nWidth/2-350+5*120+78, nHeight/2+320, strPoint);
	//pDC->SelectObject(def_font);
	//font.DeleteObject();
}

//移动按钮
void CGameClientView::OnScoreMoveL()
{
	if ( m_nRecordFirst == m_nScoreHead ) return;

	m_nScoreHead = (m_nScoreHead - 1 + MAX_C_SCORE_HISTORY) % MAX_C_SCORE_HISTORY;
	if ( m_nScoreHead == m_nRecordFirst ) m_btScoreMoveL.EnableWindow(FALSE);

	m_btScoreMoveR.EnableWindow(TRUE);

	//更新界面
	UpdateGameView(NULL);
}

//移动按钮
void CGameClientView::OnScoreMoveR()
{
	int nHistoryCount = ( m_nRecordLast - m_nScoreHead + MAX_C_SCORE_HISTORY ) % MAX_C_SCORE_HISTORY;
	if ( nHistoryCount == MAX_HISTORY_NUM ) return;

	m_nScoreHead = ( m_nScoreHead + 1 ) % MAX_C_SCORE_HISTORY;
	if ( nHistoryCount-1 == MAX_HISTORY_NUM ) m_btScoreMoveR.EnableWindow(FALSE);

	m_btScoreMoveL.EnableWindow(TRUE);

	//更新界面
	UpdateGameView(NULL);
}

//显示结果
void CGameClientView::ShowGameResult(CDC *pDC, int nWidth, int nHeight)
{
	//显示判断
	if (m_pGameClientDlg->GetGameStatus()!=GS_GAME_END) return;
	if (!m_bFinish) return;

	int	nXPos = nWidth / 2 + 50;
	int	nYPos = nHeight / 2 - 100;
//////////////////////////////////////////////////////////////////////////
	BYTE cbWinner = m_GameLogic.GetNumBigList(m_cbHorseSpeed,HORSE_COUNT);
	int iHeight = 0;
	if (cbWinner < 3)
	{
		iHeight = 0;
	}else
	{
		iHeight = 40 * (cbWinner-2);
	}
	//绘制胜利截图
	CString name,str;
	str.Format("%04d",m_Num);
	name = "Save\\"+str+".bmp";//文件名及路径
	m_ImageWin.SetLoadInfo(name);
	CImageHandle Handle(&m_ImageWin);
	m_ImageWin.BitBlt(pDC->GetSafeHdc(),nWidth/2 -270,nHeight/2-90,302,169,400,iHeight);


	//-----------------------------------------------------------------------
	//m_ImageGround.DrawImage(pDC, nWidth/2-782/2, nHeight/2 - 452/2 , 782, m_ImageGround->GetHeight(), m_nPosX,0, 782,  m_ImageGround->GetHeight());
	//全屏背景
	m_ImageResult.DrawImage(pDC,nWidth/2-782/2, nHeight/2 - 452/2 ,782, m_ImageGround->GetHeight(),nWidth/2-782/2, nHeight/2 - 452/2);
	
////////////////////////////////////////////////////////////////////////////////////////////
	//设置字体颜色
	pDC->SetTextColor(RGB(255,234,0));

	//计算绘制字体的矩形区域
	CRect rcMeWinScore, rcMeReturnScore;
	rcMeWinScore.left = nXPos+2 + 40;
	rcMeWinScore.top = nYPos+70 + 35;
	rcMeWinScore.right = rcMeWinScore.left + 111;
	rcMeWinScore.bottom = rcMeWinScore.top + 34;

	rcMeReturnScore.left = rcMeWinScore.left + 90;
	rcMeReturnScore.top = rcMeWinScore.top;
	rcMeReturnScore.right = rcMeReturnScore.left + 110;
	rcMeReturnScore.bottom = rcMeWinScore.bottom;

	CString strMeGameScore, strMeReturnScore;
	DrawNumberStringWithSpace(pDC,m_lMeCurGameScore,rcMeWinScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	DrawNumberStringWithSpace(pDC,m_lMeCurGameReturnScore,rcMeReturnScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);

	CRect rcBankerWinScore;
	rcBankerWinScore.left = rcMeWinScore.left;
	rcBankerWinScore.top = rcMeWinScore.top + 25;
	rcBankerWinScore.right = rcMeWinScore.right;
	rcBankerWinScore.bottom = rcBankerWinScore.top + 34;

	CString strBankerCurGameScore;
	strBankerCurGameScore.Format(TEXT("%I64d"), m_lBankerCurGameScore);
	DrawNumberStringWithSpace(pDC,m_lBankerCurGameScore,rcBankerWinScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);

//////////////////////////////////////////////////////////////////////////////////////////////////////
	

	//结算框上面的马
	int nImageHeight=m_ImageSaticHorse[cbWinner-1].GetHeight();
	int nImageWidth=m_ImageSaticHorse[cbWinner-1].GetWidth()/12;
	m_ImageSaticHorse[cbWinner-1].DrawImage(pDC,nWidth/2 + 100,nHeight/2-140,nImageWidth,nImageHeight,nImageWidth*m_Horse[cbWinner-1].cbHorseIndex,0);
}

//透明绘画
bool CGameClientView::DrawAlphaRect(CDC* pDC, LPRECT lpRect, COLORREF clr, FLOAT fAlpha)
{
	ASSERT(pDC != 0 && lpRect != 0 && clr != CLR_NONE);
	if(pDC == 0 || lpRect == 0 || clr == CLR_NONE)
	{
		return false;
	}
	//全透明
	if( abs(fAlpha) <= 0.000001 )
	{
		return true;
	}

	for(int l=lpRect->top; l<lpRect->bottom; l++)
	{
		for(int k=lpRect->left; k<lpRect->right; k++)
		{
			COLORREF clrBk = pDC->GetPixel(k, l);
			COLORREF clrBlend = RGB(GetRValue(clrBk)*(1-fAlpha)+GetRValue(clr)*fAlpha, 
								GetGValue(clrBk)*(1-fAlpha)+GetGValue(clr)*fAlpha,
								GetBValue(clrBk)*(1-fAlpha)+GetBValue(clr)*fAlpha);
			pDC->SetPixel(k, l, clrBlend);
		}
	}

	return true;
}
//================================================================
//最大下注
LONGLONG CGameClientView::GetMaxHorse1()
{	
	LONGLONG lMaxScore = 0;

	//庄家判断
	if (m_wBankerUser!=INVALID_CHAIR) 
	{
		//其他区域
		LONGLONG lOtherAreaScore = m_lAllScore2 + m_lAllScore3 + m_lAllScore4 + m_lAllScore5 + m_lAllScore6 + m_lAllScore7 + m_lAllScore8;

		//最大下注
		lMaxScore = m_lBankerScore + lOtherAreaScore;
		lMaxScore -= m_lAllScore3 * 7;
		lMaxScore = min(lMaxScore, (m_lAreaLimitScore-m_lAllScore1));
	}
	else
	{
		lMaxScore=m_lAreaLimitScore-m_lAllScore1;
	}

	return lMaxScore;
}



//最大下注
LONGLONG CGameClientView::GetMaxHorse2()
{
	LONGLONG lMaxScore = 0;

	//庄家判断
	if (m_wBankerUser!=INVALID_CHAIR) 
	{
		//其他区域
		LONGLONG lOtherAreaScore = m_lAllScore1 + m_lAllScore3 + m_lAllScore4 + m_lAllScore5 + m_lAllScore6 + m_lAllScore7 + m_lAllScore8;

		//最大下注
		lMaxScore = m_lBankerScore + lOtherAreaScore;
		lMaxScore -= m_lAllScore2 * 7;
		lMaxScore = min(lMaxScore, (m_lAreaLimitScore-m_lAllScore2));
	}
	else
	{
		lMaxScore=m_lAreaLimitScore-m_lAllScore2;
	}

	return lMaxScore;
}

//最大下注
LONGLONG CGameClientView::GetMaxHorse3()
{
	LONGLONG lMaxScore = 0;

	//庄家判断
	if (m_wBankerUser!=INVALID_CHAIR) 
	{
		//其他区域
		LONGLONG lOtherAreaScore = m_lAllScore1 + m_lAllScore2 + m_lAllScore4 + m_lAllScore5 + m_lAllScore6 + m_lAllScore7 + m_lAllScore8;

		//最大下注
		lMaxScore = m_lBankerScore + lOtherAreaScore;
		lMaxScore -= m_lScore3 * 7;
		lMaxScore = min(lMaxScore, (m_lAreaLimitScore-m_lAllScore3));
	}
	else
	{
		lMaxScore=m_lAreaLimitScore-m_lAllScore3;
	}

	return lMaxScore;
}

//最大下注
LONGLONG CGameClientView::GetMaxHorse4()
{
	LONGLONG lMaxScore = 0;

	//庄家判断
	if (m_wBankerUser!=INVALID_CHAIR) 
	{
		//其他区域
		LONGLONG lOtherAreaScore = m_lAllScore1 + m_lAllScore2 + m_lAllScore3 + m_lAllScore5 + m_lAllScore6 + m_lAllScore7 + m_lAllScore8;

		//最大下注
		lMaxScore = m_lBankerScore + lOtherAreaScore;
		lMaxScore -= m_lScore4 * 7;
		lMaxScore = min(lMaxScore, (m_lAreaLimitScore-m_lAllScore4));
	}
	else
	{
		lMaxScore=m_lAreaLimitScore-m_lAllScore4;
	}

	return lMaxScore;
}

//最大下注
LONGLONG CGameClientView::GetMaxHorse5()
{
	LONGLONG lMaxScore = 0;

	//庄家判断
	if (m_wBankerUser!=INVALID_CHAIR) 
	{
		//其他区域
		LONGLONG lOtherAreaScore = m_lAllScore1 + m_lAllScore2 + m_lAllScore3 + m_lAllScore4 + m_lAllScore6 + m_lAllScore7 + m_lAllScore8;

		//最大下注
		lMaxScore = m_lBankerScore + lOtherAreaScore;
		lMaxScore -= m_lScore5 * 7;
		lMaxScore = min(lMaxScore, (m_lAreaLimitScore-m_lAllScore5));
	}
	else
	{
		lMaxScore=m_lAreaLimitScore-m_lAllScore5;
	}

	return lMaxScore;
}



//最大下注
LONGLONG CGameClientView::GetMaxHorse6()
{
	LONGLONG lMaxScore = 0;

	//庄家判断
	if (m_wBankerUser!=INVALID_CHAIR) 
	{
		//其他区域
		LONGLONG lOtherAreaScore = m_lAllScore1 + m_lAllScore2 + m_lAllScore3 + m_lAllScore4 + m_lAllScore5 + m_lAllScore7 + m_lAllScore8;

		//最大下注
		lMaxScore = m_lBankerScore + lOtherAreaScore;
		lMaxScore -= m_lScore6 * 7;
		lMaxScore = min(lMaxScore, (m_lAreaLimitScore-m_lAllScore6));
	}
	else
	{
		lMaxScore=m_lAreaLimitScore-m_lAllScore6;
	}

	return lMaxScore;
}


//最大下注
LONGLONG CGameClientView::GetMaxHorse7()
{
	LONGLONG lMaxScore = 0;

	//庄家判断
	if (m_wBankerUser!=INVALID_CHAIR) 
	{
		//其他区域
		LONGLONG lOtherAreaScore = m_lAllScore1 + m_lAllScore2 + m_lAllScore3 + m_lAllScore4 + m_lAllScore5 + m_lAllScore6 + m_lAllScore8;

		//最大下注
		lMaxScore = m_lBankerScore + lOtherAreaScore;
		lMaxScore -= m_lScore7 * 7;
		lMaxScore = min(lMaxScore, (m_lAreaLimitScore-m_lAllScore7));
	}
	else
	{
		lMaxScore=m_lAreaLimitScore-m_lAllScore7;
	}

	return lMaxScore;
}


//最大下注
LONGLONG CGameClientView::GetMaxHorse8()
{
	LONGLONG lMaxScore = 0;

	//庄家判断
	if (m_wBankerUser!=INVALID_CHAIR) 
	{
		//其他区域
		LONGLONG lOtherAreaScore = m_lAllScore1 + m_lAllScore2 + m_lAllScore3 + m_lAllScore4 + m_lAllScore5 + m_lAllScore6 + m_lAllScore7;

		//最大下注
		lMaxScore = m_lBankerScore + lOtherAreaScore;
		lMaxScore -= m_lScore8 * 7;
		lMaxScore = min(lMaxScore, (m_lAreaLimitScore-m_lAllScore8));
	}
	else
	{
		lMaxScore=m_lAreaLimitScore-m_lAllScore8;
	}

	return lMaxScore;
}

//绘画数字
void CGameClientView::DrawMeJettonNumber(CDC *pDC)
{
	//绘画数字
	if (0 < m_lMeScore1) DrawNumberString(pDC,m_lMeScore1,m_PointJettonNumber[ID_HORSE1-1].x,m_PointJettonNumber[ID_HORSE1-1].y+25, true);
	if (0 < m_lMeScore2) DrawNumberString(pDC,m_lMeScore2,m_PointJettonNumber[ID_HORSE2-1].x,m_PointJettonNumber[ID_HORSE2-1].y+25, true);
	if (0 < m_lMeScore3) DrawNumberString(pDC,m_lMeScore3,m_PointJettonNumber[ID_HORSE3-1].x,m_PointJettonNumber[ID_HORSE3-1].y+25, true);
	if (0 < m_lMeScore4) DrawNumberString(pDC,m_lMeScore4,m_PointJettonNumber[ID_HORSE4-1].x,m_PointJettonNumber[ID_HORSE4-1].y+25, true);
	if (0 < m_lMeScore5) DrawNumberString(pDC,m_lMeScore5,m_PointJettonNumber[ID_HORSE5-1].x,m_PointJettonNumber[ID_HORSE5-1].y+25, true);
	if (0 < m_lMeScore6) DrawNumberString(pDC,m_lMeScore6,m_PointJettonNumber[ID_HORSE6-1].x,m_PointJettonNumber[ID_HORSE6-1].y+25, true);
	if (0 < m_lMeScore7) DrawNumberString(pDC,m_lMeScore7,m_PointJettonNumber[ID_HORSE7-1].x,m_PointJettonNumber[ID_HORSE7-1].y+25, true);
	if (0 < m_lMeScore8) DrawNumberString(pDC,m_lMeScore8,m_PointJettonNumber[ID_HORSE8-1].x,m_PointJettonNumber[ID_HORSE8-1].y+25, true);

}


//下注边框
void CGameClientView::FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC)
{
	if (m_cbAreaFlash==0xFF) return;

	//CImageHandle ImageHandleFrame(&m_ImageFrame);

	int nImageWidth = m_ImageFrame.GetWidth();
	int nImageHeight = m_ImageFrame.GetHeight();
	//闲家标志
	switch (m_cbAreaFlash)
	{
	case ID_HORSE1:	
		{			
			//m_ImageFrame.AlphaDrawImage(pDC,m_rcFrame1.left, m_rcFrame1.top, RGB(255,0,255));
			m_ImageFrame.DrawImage(pDC, m_rcFrame1.left, m_rcFrame1.top, nImageWidth, nImageHeight, 0,0);
			break;
		}
	case ID_HORSE2:
		{		
			//m_ImageFrame.AlphaDrawImage(pDC,m_rcFrame2.left, m_rcFrame2.top, RGB(255,0,255));
			m_ImageFrame.DrawImage(pDC, m_rcFrame2.left, m_rcFrame2.top, nImageWidth, nImageHeight, 0,0);
			break;
		}
	case ID_HORSE3:
		{
			//m_ImageFrame.AlphaDrawImage(pDC,m_rcFrame3.left, m_rcFrame3.top, RGB(255,0,255));
			m_ImageFrame.DrawImage(pDC, m_rcFrame3.left, m_rcFrame3.top, nImageWidth, nImageHeight, 0,0);
			break;
		}
	case ID_HORSE4:
		{
			//m_ImageFrame.AlphaDrawImage(pDC,m_rcFrame4.left, m_rcFrame4.top, RGB(255,0,255));
			m_ImageFrame.DrawImage(pDC, m_rcFrame4.left, m_rcFrame4.top, nImageWidth, nImageHeight, 0,0);
			break;
		}
	case ID_HORSE5:
		{
			//m_ImageFrame.AlphaDrawImage(pDC,m_rcFrame5.left, m_rcFrame5.top, RGB(255,0,255));
			m_ImageFrame.DrawImage(pDC, m_rcFrame5.left, m_rcFrame5.top, nImageWidth, nImageHeight, 0,0);
			break;
		}
	case ID_HORSE6:
		{		
			//m_ImageFrame.AlphaDrawImage(pDC,m_rcFrame6.left, m_rcFrame6.top, RGB(255,0,255));
			m_ImageFrame.DrawImage(pDC, m_rcFrame6.left, m_rcFrame6.top, nImageWidth, nImageHeight, 0,0);
			break;
		}
	case ID_HORSE7:
		{		
			//m_ImageFrame.AlphaDrawImage(pDC,m_rcFrame7.left, m_rcFrame7.top, RGB(255,0,255));
			m_ImageFrame.DrawImage(pDC, m_rcFrame7.left, m_rcFrame7.top, nImageWidth, nImageHeight, 0,0);
			break;
		}	
	
	case ID_HORSE8:
		{
			//m_ImageFrame.AlphaDrawImage(pDC,m_rcFrame8.left, m_rcFrame8.top, RGB(255,0,255));
			m_ImageFrame.DrawImage(pDC, m_rcFrame8.left, m_rcFrame8.top, nImageWidth, nImageHeight, 0,0);
			break;
		}
	}
}


//设置爆炸
bool CGameClientView::SetBombEffect(bool bBombEffect, WORD wAreaIndex)
{
	if (bBombEffect==true)
	{
		//设置变量
		m_bBombEffect[wAreaIndex]=true;
		m_cbBombFrameIndex[wAreaIndex]=0;

		//启动时间
		//SetTimer(IDI_BOMB_EFFECT+wAreaIndex,100,NULL);
	}
	else
	{
		//停止动画
		if (m_bBombEffect[wAreaIndex]==true)
		{
			//删除时间
			//KillTimer(IDI_BOMB_EFFECT+wAreaIndex);

			//设置变量
			m_bBombEffect[wAreaIndex]=false;
			m_cbBombFrameIndex[wAreaIndex]=0;

			//更新界面
			UpdateGameView(NULL);
		}
	}

	return true;
}


//绘画爆炸
void CGameClientView::DrawBombEffect(CDC *pDC)
{
	//绘画爆炸
	INT nImageHeight=m_ImageBombEffect.GetHeight();
	INT nImageWidth=m_ImageBombEffect.GetWidth()/BOMB_EFFECT_COUNT;
	if (m_bBombEffect[ID_HORSE1]==true)
	{
		m_ImageBombEffect.DrawImage(pDC,m_PointJettonNumber[ID_HORSE1-1].x-nImageWidth/2,m_PointJettonNumber[ID_HORSE1-1].y,nImageWidth,nImageHeight,
			nImageWidth*(m_cbBombFrameIndex[ID_HORSE1]%BOMB_EFFECT_COUNT),0);
	}
	if (m_bBombEffect[ID_HORSE2]==true)
	{
		m_ImageBombEffect.DrawImage(pDC,m_PointJettonNumber[ID_HORSE2-1].x-nImageWidth/2,m_PointJettonNumber[ID_HORSE2-1].y,nImageWidth,nImageHeight,
			nImageWidth*(m_cbBombFrameIndex[ID_HORSE2]%BOMB_EFFECT_COUNT),0);
	}
	if (m_bBombEffect[ID_HORSE3]==true)
	{
		m_ImageBombEffect.DrawImage(pDC,m_PointJettonNumber[ID_HORSE3-1].x-nImageWidth/2,m_PointJettonNumber[ID_HORSE3-1].y,nImageWidth,nImageHeight,
			nImageWidth*(m_cbBombFrameIndex[ID_HORSE3]%BOMB_EFFECT_COUNT),0);
	}
	if (m_bBombEffect[ID_HORSE4]==true)
	{
		m_ImageBombEffect.DrawImage(pDC,m_PointJettonNumber[ID_HORSE4-1].x-nImageWidth/2,m_PointJettonNumber[ID_HORSE4-1].y,nImageWidth,nImageHeight,
			nImageWidth*(m_cbBombFrameIndex[ID_HORSE4]%BOMB_EFFECT_COUNT),0);
	}
	if (m_bBombEffect[ID_HORSE5]==true)
	{
		m_ImageBombEffect.DrawImage(pDC,m_PointJettonNumber[ID_HORSE5-1].x-nImageWidth/2,m_PointJettonNumber[ID_HORSE5-1].y,nImageWidth,nImageHeight,
			nImageWidth*(m_cbBombFrameIndex[ID_HORSE5]%BOMB_EFFECT_COUNT),0);
	}
	if (m_bBombEffect[ID_HORSE6]==true)
	{
		m_ImageBombEffect.DrawImage(pDC,m_PointJettonNumber[ID_HORSE6-1].x-nImageWidth/2,m_PointJettonNumber[ID_HORSE6-1].y,nImageWidth,nImageHeight,
			nImageWidth*(m_cbBombFrameIndex[ID_HORSE6]%BOMB_EFFECT_COUNT),0);
	}
	if (m_bBombEffect[ID_HORSE7]==true)
	{
		m_ImageBombEffect.DrawImage(pDC,m_PointJettonNumber[ID_HORSE7-1].x-nImageWidth/2,m_PointJettonNumber[ID_HORSE7-1].y,nImageWidth,nImageHeight,
			nImageWidth*(m_cbBombFrameIndex[ID_HORSE7]%BOMB_EFFECT_COUNT),0);
	}
	if (m_bBombEffect[ID_HORSE8]==true)
	{
		m_ImageBombEffect.DrawImage(pDC,m_PointJettonNumber[ID_HORSE8-1].x-nImageWidth/2,m_PointJettonNumber[ID_HORSE8-1].y,nImageWidth,nImageHeight,
			nImageWidth*(m_cbBombFrameIndex[ID_HORSE8]%BOMB_EFFECT_COUNT),0);
	}

}

//绘画马
void CGameClientView::DrawHorse(CDC *pDC)
{
	INT nImageHeight = 0;
	INT nImageWidth = 0;
	INT nSignHeight = m_ImageSign->GetHeight();
	INT nSignWidth = m_ImageSign->GetWidth()/8;

	BYTE bFirst = GetFastOrLater();
	for (int i=0; i< HORSE_COUNT; i++)
	{		
		m_ImageSign.DrawImage(pDC, (int)(m_Sign[i].fXPos + 0.9f), (int)m_Sign[i].fYPos, nSignWidth, nSignHeight, nSignWidth * i, 0, nSignWidth, nSignHeight); 

		if (m_Horse[i].bStatic)
		{
			nImageHeight=m_ImageSaticHorse[i].GetHeight();
			nImageWidth=m_ImageSaticHorse[i].GetWidth()/12;
			m_ImageSaticHorse[i].DrawImage(pDC,m_Horse[i].nXPos,m_Horse[i].nYPos,nImageWidth,nImageHeight,nImageWidth*m_Horse[i].cbHorseIndex,0);
		} 
		else
		{    
			nImageHeight=m_ImageDynamicHorse[0].GetHeight();
			nImageWidth=m_ImageDynamicHorse[0].GetWidth()/8;
			m_ImageDynamicHorse[i].DrawImage(pDC,m_Horse[i].nXPos,m_Horse[i].nYPos,nImageWidth,nImageHeight,nImageWidth*m_Horse[i].cbHorseIndex1,0);

			if (bFirst == i)
			m_ImageFirst.DrawImage(pDC,m_Horse[i].nXPos + nImageWidth-10,m_Horse[i].nYPos,m_ImageFirst.GetWidth(),m_ImageFirst.GetHeight(),0,0);

			if (m_Horse[i].bBlow)
			{
				int nImageW = m_ImageBlow.GetWidth()/7;
				int nImageH = m_ImageBlow.GetHeight();
				m_ImageBlow.DrawImage(pDC,m_Horse[i].nXPos + nImageWidth-15,m_Horse[i].nYPos+30,nImageW,nImageH,m_cbBlowIndex*nImageW,0);
			}			
		}
	}
}

//绘制我的下注
void CGameClientView::DrawMeScore(CDC * pDC, int nWidth, int nHeight)
{
	if (m_pGameClientDlg->GetGameStatus() != GS_GAME_END) return;
	m_ImageMeScore.DrawImage(pDC, 10, nHeight/2 + 452/2 - 60, m_ImageMeScore.GetWidth(), m_ImageMeScore.GetHeight(), 0,0);
	pDC->SetTextColor(RGB(0,0,0));

	LONGLONG lMeScore[] = {m_lMeScore1, m_lMeScore2, m_lMeScore3, m_lMeScore4, m_lMeScore5, m_lMeScore6, m_lMeScore7, m_lMeScore8};

	for (int i=0;i<8; i++)
	{
		LONGLONG lNumber = lMeScore[i];
		CString strNumber=TEXT(""), strTmpNumber1,strTmpNumber2;
		if (lNumber==0) strNumber=TEXT("0");
		int nNumberCount=0;
		LONGLONG lTmpNumber=lNumber;
		if (lNumber<0) lNumber=-lNumber;
		while (lNumber>0)
		{
			strTmpNumber1.Format(TEXT("%I64d"),lNumber%10);
			nNumberCount++;
			strTmpNumber2 = strTmpNumber1+strTmpNumber2;

			if (nNumberCount==4)
			{
				strTmpNumber2 += (TEXT(" ") +strNumber);
				strNumber=strTmpNumber2;
				nNumberCount=0;
				strTmpNumber2=TEXT("");
			}
			lNumber/=10;
		}

		if (strTmpNumber2.IsEmpty()==FALSE)
		{
			strTmpNumber2 += (TEXT(" ") +strNumber);
			strNumber=strTmpNumber2;
		}

		if (lTmpNumber<0) strNumber=TEXT("-")+strNumber;

		pDC->SetTextColor(RGB(0,0,200));
		//输出数字
		//pDC->DrawText(strNumber,CRect(10 + 93*i,nHeight/2 + 452/2 -25,105 + 93*i,nHeight/2 + 452/2-10),DT_VCENTER|DT_SINGLELINE|DT_CENTER|DT_END_ELLIPSIS);
		pDC->DrawText(strNumber,CRect(10 + 93*i,nHeight/2 + 452/2 -25,105 + 93*i,nHeight/2 + 452/2-10),DT_VCENTER|DT_SINGLELINE|DT_CENTER|DT_END_ELLIPSIS);
	}
		//DrawNumberStringWithSpace(pDC,lMeScore[i],CRect(10 + 93*i,nHeight/2 + 452/2 -25,105 + 93*i,nHeight/2 + 452/2-10),DT_VCENTER|DT_SINGLELINE|DT_CENTER|DT_END_ELLIPSIS);
}

BYTE CGameClientView::GetFastOrLater(bool bFast)
{
	int nMinIndex = 0;//最后一名的下标
	int nMaxIndex = 0;//最前一名的下标			
	int MinPos = m_Horse[0].nXPos;
	int MaxPos = m_Horse[0].nXPos;

	for (int i=0; i < HORSE_COUNT; i++)
	{
		if (m_Horse[i].nXPos < MinPos)
		{
			MinPos = m_Horse[i].nXPos;
			nMinIndex = i;
		}
		if (m_Horse[i].nXPos > MaxPos)
		{
			MaxPos = m_Horse[i].nXPos;
			nMaxIndex = i;
		}
	}

	if (bFast)
	{
		return nMaxIndex;
	} 
	else
	{
		return nMinIndex;
	}

}

//绘画时间
void CGameClientView::DrawUserTimer(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea)
{
	//加载资源
	CPngImage ImageTimeBack;
	CPngImage ImageTimeNumber;
	ImageTimeNumber.LoadImage("RaceHorse\\SCORE_NUMBER.png");

	//获取属性
	const INT nNumberHeight=ImageTimeNumber.GetHeight();
	const INT nNumberWidth=ImageTimeNumber.GetWidth()/10;

	//计算数目
	LONG lNumberCount=0;
	WORD wNumberTemp=wTime;
	do
	{
		lNumberCount++;
		wNumberTemp/=10;
	} while (wNumberTemp>0L);

	//位置定义
	INT nYDrawPos=nYPos-nNumberHeight/2+1;
	INT nXDrawPos=nXPos+(lNumberCount*nNumberWidth)/2-nNumberWidth;

	//绘画号码
	for (LONG i=0;i<lNumberCount;i++)
	{
		//绘画号码
		WORD wCellNumber=wTime%10;
		ImageTimeNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,wCellNumber*nNumberWidth,0);

		//设置变量
		wTime/=10;
		nXDrawPos-=nNumberWidth;
	};

	return;
}

//存钱
void CGameClientView::OnBank()
{
	AfxGetMainWnd()->SendMessage(IDM_BANK,0,0);
	
}
//
////取钱
//void CGameClientView::OnBankGet()
//{
//	AfxGetMainWnd()->SendMessage(IDM_BANK,1,1);
//}
//////////////////////////////////////////////////////////////////////////

