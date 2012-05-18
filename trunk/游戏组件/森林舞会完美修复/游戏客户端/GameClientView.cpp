#include "StdAfx.h"
#include "Math.h"
#include "Resource.h"
#include "GameClientView.h"
#include "Path.h"

//////////////////////////////////////////////////////////////////////////

//时间标识
#define IDI_FLASH_WINNER			100									//闪动标识
#define IDI_SHOW_CHANGE_BANKER		101									//轮换庄家
#define IDI_SEND_CARD_CARTOON		102									//发牌动画定时器
#define IDI_SHOW_WIN_AMOUNT			103									//显示输赢金额
#define IDI_FLASH_WIN               104                                 //声音间隔    
#define IDI_FLASH_Tips              105                                 //彩金鎑 
#define IDI_HY                      112                                 //HY时间
#define IDI_bank_show               106                                 //庄家列表显示

#define IDC_APPLY_BANKER			200									//申请坐庄
#define IDC_CANCEL_BANKER			201									//取消坐庄
#define IDC_BANKER_SHOW             214                                 //上庄下翻

#define IDC_GameBI		        	120									//申请坐庄
#define IDC_GameBI50		        	121									//申请坐庄
#define IDC_GameBI100		        	122									//申请坐庄
#define IDC_GameBI500		        	123									//申请坐庄
#define IDC_GameBI1000		        	124									//申请坐庄
#define IDC_GameBIClose		        	125									//申请坐庄

#define IDC_JETTON_BUTTON			300									//筹码按钮
#define IDC_CANCEL_FANG             451                                 //取分按钮
#define IDC_CANCEL_Settlement       452                                 //结算



#define IDC_INFO_UP                 216                                 //上庄下翻
#define IDC_INFO_DOWN               217                                 //上庄上翻


#define JETTON_AREA_BORDER			3									//筹码距离下注区域边缘的距离
#define JETTON_AREA_BORDER_W		5									//筹码距离下注区域边缘的距离
#define JETTON_AREA_BORDER_H		5									//筹码距离下注区域边缘的距离
#define IDC_JETTON_SUPPER			400									//筹码按钮
#define BG_WIDTH					730									//背景图高度
#define BG_HEIGHT					695									//背景图宽度
#define CARD_SPACE					20								//牌与牌的间距

//下注时间
#define IDI_BET_TIME_OVER			1									//下注计时器
#define IDI_BET_TIME_SEND			2									//下注计时器
#define IDI_GAME_END				3									//结束计时器


//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_MESSAGE(WM_GAME_BASE_CTRL_BUTTON_CLICK_UP, OnButtonClick)
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientView::CGameClientView()
{
	//设置开始时是不用转转盘的

	//状态信息
	m_lCurrentJetton=0L;
	m_dwWinnerFlash=NO_WINNER;
	m_nWinnerFlashIndex = 0 ;
	m_endsrok=false;
    m_bDrawWinArea=false;
	m_showPrizeBZ=false;
	m_lMywin=0;
	m_lBankerwin=0;
    ISDrawlight=true;
	m_timezt=-1;
	pShakeThread = NULL;

	//历史成绩
	m_wMyWinCount = 0;

	m_iSMyWin=false;


	//下注信息
	ZeroMemory(m_lAllMaxScore, sizeof(m_lAllMaxScore));
	ZeroMemory(m_lMyStakes, sizeof(m_lMyStakes));
	ZeroMemory(m_lAllStakes, sizeof(m_lAllStakes));
	ZeroMemory(m_showTX, sizeof(m_showTX));
	m_cbCardCount=0;

	//庄家信息
	m_pImageShowBanker = NULL;
	m_wCurrentBanker = INVALID_CHAIR;
	m_wMyChairID = INVALID_CHAIR;
	m_wBankerCount = 0;			
	m_lBankerWinScore = 0;			



	m_showrobst[0] = 1;
	m_showrobst[1] = 3;
	m_showrobst[2] = 5;
	m_showrobst[3] = 7;
	m_showrobst[4] = 9;
	m_showrobst[5] = 11;
	m_showrobst[6] = 13;
	m_showrobst[7] = 15;
	m_showrobst[8] = 17;
	m_showrobst[9] = 19;
	m_showrobst[10] = 21;
	m_showrobst[11] = 23;
	m_showrobst[12] = 22;
	m_showrobst[13] = 20;
	m_showrobst[14] = 18;
	m_showrobst[15] = 16;
	m_showrobst[16] = 14;
	m_showrobst[17] = 12;
	m_showrobst[18] = 10;
	m_showrobst[19] = 8;
	m_showrobst[20] = 6;
	m_showrobst[21] = 4;
	m_showrobst[22] = 2;
	m_showrobst[23] = 0;

	m_strResPath = g_pPath->StripFileExtension(g_pPath->GetPathExecutable()) + "\\";
	LoadRes();
	
	return;
}

//析构函数
CGameClientView::~CGameClientView()
{
}

void CGameClientView::LoadRes()
{
	CVIEWMANAGE* pViewManager = AfxGetViewManager();

	CString strFile = m_strResPath + "bg.psd";
	m_pImageViewBack = pViewManager->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);

	strFile = m_strResPath + "时间.psd";
	m_pImageTimer = pViewManager->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);

	strFile = m_strResPath + "history_win.psd";
	m_pImageHistoryWin = pViewManager->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);


	strFile = m_strResPath + "SmallTotalNum.psd";
	m_pImageStakeAmountTotal2 = pViewManager->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);

	strFile = m_strResPath + "SmallSingleNum.psd";
	m_pImageStakeAmountMine2 = pViewManager->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);

	strFile = m_strResPath + "下注时间.psd";//数字时间
	m_pImageplay = pViewManager->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);   

	strFile = m_strResPath + "发牌时间.psd";//数字时间
	m_pImagesend = pViewManager->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);   

	 strFile = m_strResPath + "History.psd";
	 m_pImageHistory = pViewManager->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);   

	 strFile = m_strResPath + "文字.psd";
	 m_WinTips = pViewManager->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);  

	 strFile = m_strResPath + "单组字-.psd";
	 m_WinTitle = pViewManager->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);  

	 strFile = m_strResPath + "彩灯动画.psd";
	 m_WinPrize = pViewManager->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);  
	
	 // 中彩金提示
	 strFile = m_strResPath + "提示框.psd";
     m_Winbg = pViewManager->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);  

	 strFile = m_strResPath + "动画\\11.psd";
	 m_show[0] = pViewManager->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);  

	 strFile = m_strResPath + "动画\\12.psd";
	 m_show[1] = pViewManager->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);  

	 strFile = m_strResPath + "动画\\13.psd";
	 m_show[2] = pViewManager->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);  

	 strFile = m_strResPath + "动画\\21.psd";
	 m_show[3] = pViewManager->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);  

	 strFile = m_strResPath + "动画\\22.psd";
	 m_show[4] = pViewManager->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);  

	 strFile = m_strResPath + "动画\\23.psd";
	 m_show[5] = pViewManager->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);  

	 strFile = m_strResPath + "动画\\31.psd";
	 m_show[6] = pViewManager->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);  

	 strFile = m_strResPath + "动画\\32.psd";
	 m_show[7] = pViewManager->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);  

	 strFile = m_strResPath + "动画\\33.psd";
	 m_show[8] = pViewManager->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);  

	 strFile = m_strResPath + "动画\\41.psd";
	 m_show[9] = pViewManager->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);  

	 strFile = m_strResPath + "动画\\42.psd";
	 m_show[10] = pViewManager->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);  

	 strFile = m_strResPath + "动画\\43.psd";
	 m_show[11] = pViewManager->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);  

	 strFile = m_strResPath + "框.psd";
     m_pImagekuan = pViewManager->CreateSurface(strFile.GetBuffer(), PSD_SURFACE); 

	 strFile = m_strResPath + "时钟.psd";
	 m_pImageTimerbg = pViewManager->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);

	 strFile = m_strResPath + "庄家.psd";
	 m_Bankerbg = pViewManager->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);

	 strFile = m_strResPath + "庄列表.psd";
	 m_pImageBankbg = pViewManager->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);  


	 strFile = m_strResPath + "no_banker.psd";
	 m_pImageNoBanker = pViewManager->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);

	 strFile = m_strResPath + "me_banker.psd";
	 m_pImageMeBanker = pViewManager->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);

	 strFile = m_strResPath + "change_banker.psd";
	 m_pImageChangeBanker = pViewManager->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);

	 strFile = m_strResPath + "kuang.psd";
	 m_kuang = pViewManager->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);

	 //数字时间

	CString strCursorPath;


		strCursorPath.Format("%s银币.cur", m_strResPath);
		HCURSOR Cursor = LoadCursorFromFile(strCursorPath);
		m_JettonCursors = (Cursor != NULL ? Cursor : LoadCursor(NULL, IDC_ARROW));

}

void CGameClientView::Create(RECT rc, CGameWnd *pWnd)
{
	CGameWnd::Create(rc, pWnd);
	Show(true);
}

bool CGameClientView::OnCreate()
{


	CRect rcCreate(0,0,0,0);

	CString strFile;

	//创建筹码按钮
	for (WORD i=0; i<CountArray(m_btnJettons); i++)
	{
		strFile.Format("%sbtn_%d.psd", m_strResPath, JETTON_AMOUNT[i]);
		m_btnJettons[i].Create(IDC_JETTON_BUTTON + i,  strFile.GetBuffer(),rcCreate, this);
		m_btnJettons[i].Show(true);
	}

	for (WORD i=0; i<8; i++)
	{
		strFile = m_strResPath + "列表下翻.psd";
		m_btnsupper[i].Create(IDC_JETTON_SUPPER + i,  strFile.GetBuffer(),rcCreate, this);
		m_btnsupper[i].Show(false);
	}

	strFile = m_strResPath + "上庄列表.psd";
	m_BankerShow.Create(IDC_BANKER_SHOW, strFile.GetBuffer(), rcCreate, this);
	m_BankerShow.Show(true);
	
	strFile = m_strResPath + "申请上庄.psd";
	m_btnApplyBanker.Create(IDC_APPLY_BANKER, strFile.GetBuffer(), rcCreate, this);
	m_btnApplyBanker.Show(true);

	strFile = m_strResPath + "申请下庄.psd";
	m_btnCancelBanker.Create(IDC_CANCEL_BANKER, strFile.GetBuffer(), rcCreate, this);
	m_btnCancelBanker.Show(false);

	strFile = m_strResPath + "银行.psd";
	m_pImagefang.Create(IDC_CANCEL_FANG, strFile.GetBuffer(), rcCreate, this);
	m_pImagefang.Show(false);

	strFile = m_strResPath + "结算-.psd";
	m_pSettlement.Create(IDC_CANCEL_Settlement, strFile.GetBuffer(), rcCreate, this);
	m_pSettlement.Show(true);

	strFile = m_strResPath + "列表上翻.psd";
	m_infoup.Create(IDC_INFO_UP, strFile.GetBuffer(), rcCreate, this);
	m_infoup.Show(true);

	strFile = m_strResPath + "列表下翻.psd";
	m_infodown.Create(IDC_INFO_DOWN, strFile.GetBuffer(), rcCreate, this);
	m_infodown.Show(true);

	strFile = m_strResPath + "购买游戏币.psd";
	m_pGameB.Create(IDC_GameBI, strFile.GetBuffer(), rcCreate, this);
	m_pGameB.Show(true);

	strFile = m_strResPath + "50.psd";
	m_pGame50.Create(IDC_GameBI50, strFile.GetBuffer(), rcCreate, this);
	strFile = m_strResPath + "100.psd";
	m_pGame100.Create(IDC_GameBI100, strFile.GetBuffer(), rcCreate, this);
	strFile = m_strResPath + "500.psd";
	m_pGame500.Create(IDC_GameBI500, strFile.GetBuffer(), rcCreate, this);
	strFile = m_strResPath + "1000.psd";
	m_pGame1000.Create(IDC_GameBI1000, strFile.GetBuffer(), rcCreate, this);
	strFile = m_strResPath + "关闭-.psd";
	m_pGameClose.Create(IDC_GameBIClose, strFile.GetBuffer(), rcCreate, this);

	return CGameWnd::OnCreate();
}

//建立消息
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	return 0;
}


//重置界面
void CGameClientView::ResetGameView()
{   
	//庄家信息
	m_pImageShowBanker = NULL;
	m_wCurrentBanker = INVALID_CHAIR;
	m_wBankerCount = 0;			
	m_lBankerWinScore = 0;	

	//清空列表
	//m_ApplyUser.ClearAll();


	return;
}

//调整控件
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{

	WORD dwCenterX = nWidth/2;
	WORD dwCenterY = nHeight/2;





//	m_ptProp = CPoint(nWidth/2,nHeight/2-150);

	m_showXy[0].x=dwCenterX-94;
	m_showXy[0].y=dwCenterY-288;

	m_showXy[1].x=dwCenterX-25;
	m_showXy[1].y=dwCenterY-288;

	m_showXy[2].x=dwCenterX-170;
	m_showXy[2].y=dwCenterY-280;

	m_showXy[3].x=dwCenterX+52;
	m_showXy[3].y=dwCenterY-280;

	m_showXy[4].x=dwCenterX-247;
	m_showXy[4].y=dwCenterY-268;

	m_showXy[5].x=dwCenterX+132;
	m_showXy[5].y=dwCenterY-268;

	m_showXy[6].x=dwCenterX-320;
	m_showXy[6].y=dwCenterY-253;

	m_showXy[7].x=dwCenterX+205;
	m_showXy[7].y=dwCenterY-253;

	m_showXy[8].x=dwCenterX-383;
	m_showXy[8].y=dwCenterY-220;

	m_showXy[9].x=dwCenterX+268;
	m_showXy[9].y=dwCenterY-220;

	m_showXy[10].x=dwCenterX-443;
	m_showXy[10].y=dwCenterY-179;

	m_showXy[11].x=dwCenterX+328;
	m_showXy[11].y=dwCenterY-179;

	m_showXy[12].x=dwCenterX-445;
	m_showXy[12].y=dwCenterY-120;

	m_showXy[13].x=dwCenterX+330;
	m_showXy[13].y=dwCenterY-120;

	m_showXy[14].x=dwCenterX-385;
	m_showXy[14].y=dwCenterY-80;

	m_showXy[15].x=dwCenterX+270;
	m_showXy[15].y=dwCenterY-80;

	m_showXy[16].x=dwCenterX-323;
	m_showXy[16].y=dwCenterY-48;

	m_showXy[17].x=dwCenterX+208;
	m_showXy[17].y=dwCenterY-48;

	m_showXy[18].x=dwCenterX-250;
	m_showXy[18].y=dwCenterY-34;

	m_showXy[19].x=dwCenterX+135;
	m_showXy[19].y=dwCenterY-34;

	m_showXy[20].x=dwCenterX-175;
	m_showXy[20].y=dwCenterY-20;

	m_showXy[21].x=dwCenterX+60;
	m_showXy[21].y=dwCenterY-20;

	m_showXy[22].x=dwCenterX-97;
	m_showXy[22].y=dwCenterY-12;

	m_showXy[23].x=dwCenterX-18;
	m_showXy[23].y=dwCenterY-12;

	WORD dwPointX = dwCenterX+332; //横坐标起点
	WORD dwPointY = dwCenterY+232; //纵坐标起点


	CSize SizeUnit(75, 40);	//闲家下注区域矩形边长
	m_RectJetton[INDEX_PLAYER].SetRect(dwPointX, dwPointY, dwPointX+SizeUnit.cx, dwPointY + SizeUnit.cy);

	m_RectJetton[1].SetRect(dwPointX, dwPointY + SizeUnit.cy,dwPointX+SizeUnit.cx, dwPointY+SizeUnit.cy*2);

	m_RectJetton[2].SetRect(dwPointX, dwPointY+SizeUnit.cy*2,dwPointX+SizeUnit.cx, dwPointY+SizeUnit.cy*3);

	m_RectJetton[3].SetRect(dwPointX-128, dwPointY, dwPointX-128+SizeUnit.cx, dwPointY + SizeUnit.cy);

	m_RectJetton[4].SetRect(dwPointX-128, dwPointY + SizeUnit.cy,dwPointX-128+SizeUnit.cx, dwPointY+SizeUnit.cy*2);

	m_RectJetton[5].SetRect(dwPointX-128, dwPointY+SizeUnit.cy*2,dwPointX-128+SizeUnit.cx, dwPointY+SizeUnit.cy*3);

	m_RectJetton[6].SetRect(dwPointX-257, dwPointY, dwPointX-257+SizeUnit.cx, dwPointY + SizeUnit.cy);

	m_RectJetton[7].SetRect(dwPointX-257, dwPointY + SizeUnit.cy,dwPointX-257+SizeUnit.cx, dwPointY+SizeUnit.cy*2);

	m_RectJetton[8].SetRect(dwPointX-257, dwPointY+SizeUnit.cy*2,dwPointX-257+SizeUnit.cx, dwPointY+SizeUnit.cy*3);

	m_RectJetton[9].SetRect(dwPointX-385, dwPointY, dwPointX-385+SizeUnit.cx, dwPointY + SizeUnit.cy);

	m_RectJetton[10].SetRect(dwPointX-385, dwPointY + SizeUnit.cy,dwPointX-385+SizeUnit.cx, dwPointY+SizeUnit.cy*2);

	m_RectJetton[11].SetRect(dwPointX-385, dwPointY+SizeUnit.cy*2,dwPointX-385+SizeUnit.cx, dwPointY+SizeUnit.cy*3);

	m_RectJetton[12].SetRect(dwPointX-518, dwPointY, dwPointX-518+SizeUnit.cx, dwPointY + SizeUnit.cy);

	m_RectJetton[13].SetRect(dwPointX-518, dwPointY + SizeUnit.cy,dwPointX-518+SizeUnit.cx, dwPointY+SizeUnit.cy*2);

	m_RectJetton[14].SetRect(dwPointX-518, dwPointY+SizeUnit.cy*2,dwPointX-518+SizeUnit.cx, dwPointY+SizeUnit.cy*3);

	//移动控件
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;

	//筹码按钮
	CRect rcJetton = m_btnJettons[0].GetWndRect();
	int left = dwCenterX+412 ;
	int top = dwCenterY +195;
	CRect rcNew(left, top, left + rcJetton.Width() , top + rcJetton.Height());

	for (WORD i=0; i<CountArray(m_btnJettons); i++)
	{
        rcNew.MoveToY(rcNew.top+38);
		m_btnJettons[i].SetWndRect(rcNew);
	}

	rcJetton = m_pSettlement.GetWndRect();
	rcJetton.MoveToXY(dwCenterX-285, dwCenterY+330);

	m_pSettlement.SetWndRect(rcJetton);

	rcJetton = m_infoup.GetWndRect();
	rcJetton.MoveToXY(dwCenterX-438, dwCenterY+115);

	m_infoup.SetWndRect(rcJetton);
	rcJetton.MoveToXY(dwCenterX-438, dwCenterY+140);

	m_infodown.SetWndRect(rcJetton);

    m_pGameBZ.x = dwCenterX-285;
    m_pGameBZ.y = dwCenterY;

	rcJetton = m_pGameB.GetWndRect();
	rcJetton.MoveToXY(dwCenterX-285, dwCenterY+300);
	m_pGameB.SetWndRect(rcJetton);


	rcJetton = m_pGame50.GetWndRect();
	rcJetton.MoveToXY( m_pGameBZ.x+25, m_pGameBZ.y+15);
	m_pGame50.SetWndRect(rcJetton);

	rcJetton = m_pGame100.GetWndRect();
	rcJetton.MoveToXY( m_pGameBZ.x+25, m_pGameBZ.y+54);
	m_pGame100.SetWndRect(rcJetton);

	rcJetton = m_pGame500.GetWndRect();
	rcJetton.MoveToXY( m_pGameBZ.x+25, m_pGameBZ.y+93);
	m_pGame500.SetWndRect(rcJetton);

	rcJetton = m_pGame1000.GetWndRect();
	rcJetton.MoveToXY( m_pGameBZ.x+25, m_pGameBZ.y+132);
	m_pGame1000.SetWndRect(rcJetton);

	rcJetton = m_pGameClose.GetWndRect();
	rcJetton.MoveToXY( m_pGameBZ.x+109, m_pGameBZ.y+2);
	m_pGameClose.SetWndRect(rcJetton);


	m_time.x=dwCenterX-285;
	m_time.y=dwCenterY+225;

	rcJetton = m_btnApplyBanker.GetWndRect();
	rcJetton.MoveToXY(dwCenterX-502, dwCenterY+247);

	m_btnApplyBanker.SetWndRect(rcJetton);	//上庄按钮
	m_btnCancelBanker.SetWndRect(rcJetton);	//下庄按钮


	rcJetton = m_BankerShow.GetWndRect();

	rcJetton.MoveToXY(dwCenterX-420, dwCenterY+249);

	m_BankerShow.SetWndRect(rcJetton);	//上庄按钮

	//结束窗口调整
	EndDeferWindowPos(hDwp);

	return;
}

void CGameClientView::Drawlight(SURFACE* psurfTarget,BYTE x)
{
		
}
void CGameClientView::Animation(bool bClose)
{
	if(this->pShakeThread)
	{
		DWORD hStatus = ::WaitForSingleObject(this->pShakeThread->m_hThread,100);
		if(hStatus!=WAIT_OBJECT_0)
		{
			DWORD exitcode=0;
			if(GetExitCodeThread (this->pShakeThread->m_hThread,&exitcode))
			{
				TerminateThread(this->pShakeThread->m_hThread,exitcode);
			}
		}
		this->pShakeThread=NULL;
	}

	if(!bClose)
	{
		this->pShakeThread = AfxBeginThread(this->ShowAnimalFunction,this);
		this->pShakeThread->m_bAutoDelete = true;
	}
}

UINT CGameClientView::ShowAnimalFunction( LPVOID pParam )
{

	CGameClientView *pThis = (CGameClientView*)pParam;
	if(pThis==NULL)
		return 0;

	if(pGlobalDlg->IsEnableSound())
	{
		pGlobalDlg->PlayGameSound("H_SLWH\\Sound\\start.wav");
	}

	int zcount;
	for (int i=0;i<24;i++)
	{
       if (pThis->m_showrobst[i]==pThis->m_cbCardCount)
       {
           zcount=48+i;
		   break;
       }
	}

	int m_lighttime=1000;
	int timecount=1;
	int timesum = 0;
	int m_Image[24];
	Sleep(200);
    int lstime = 0;
	while(1)
	{
		

		for (int i=0;i<24;i++)
		{
			if (m_Image[i]==1)
			{
				pThis->m_showTX[i]++;
			}

			if (m_Image[i]==2)
			{
				pThis->m_showTX[i]--;
			}
			if (pThis->m_showTX[i]>6)
			{
				m_Image[i]=2;
				pThis->m_showTX[i]=5;
			}
			if (pThis->m_showTX[i]<0)
			{
				m_Image[i]=0;
				pThis->m_showTX[i]=0;
			}
		}
		Sleep(50);
        pThis->UpdateGameView(NULL);

		timesum+=50;
		if (timesum==m_lighttime)
		{	
			if (pGlobalDlg->IsEnableSound())
			{
				pGlobalDlg->PlayGameSound("H_SLWH\\Sound\\light.wav");
			}
			timesum=0;
			if (timecount==zcount)
			{
				break;
			}
			m_Image[pThis->m_showrobst[timecount%24]]=1;
			if (m_lighttime>101&&timecount<10)
			{
				m_lighttime=m_lighttime-100;
			}
			if (timecount>zcount-8)
			{
				m_lighttime=m_lighttime+100;
			}
			timecount++;
		}
	}
	Sleep(100);
	if (pGlobalDlg->IsEnableSound())
	{
		PlaySound("H_SLWH\\Sound\\FreeBk.wav" ,NULL, SND_FILENAME | SND_ASYNC);
	}
	pThis->m_GameRecord.AddHistoryResult(pThis->m_Card);
	pThis->m_iSMyWin=true;
	pThis->StartWin();

    pThis->TimerFLASH();
	pThis->UpdateGameView(NULL);

}

void CGameClientView::OnDraw(SURFACE* psurfTarget)
{
	if (!g_pGameApp)	return;

	try
	{


	TCHAR szBuffer[256];
	CRect rc = g_pGameApp->m_pMainGameWnd->GetWndRect();

	int nWidth = rc.Width();
	int nHeight = rc.Height();
	int dwCenterX = rc.Width() / 2;
	int dwCenterY = rc.Height() / 2;
	
	//绘制背景
	if (m_pImageViewBack)
	{
		m_pImageViewBack->Blit(psurfTarget, dwCenterX - m_pImageViewBack->GetWidth()/2, dwCenterY - m_pImageViewBack->GetHeight()/2-28);
	} 

	for (int i=0;i<24;i++)
	{
		m_show[	m_showT[i]]->Blit(psurfTarget,m_showTX[i]*140,0,m_showXy[i].x,m_showXy[i].y-10,140,200);
	}


	//m_pImageViewBack->Blit(psurfTarget,0,0,m_RectJetton[12].left,m_RectJetton[12].top,m_RectJetton[12].Width(),m_RectJetton[12].Height());

	//m_pImageViewBack->Blit(psurfTarget,0,0,m_RectJetton[13].left,m_RectJetton[13].top,m_RectJetton[13].Width(),m_RectJetton[13].Height());
	//	m_pImageViewBack->Blit(psurfTarget,0,0,m_RectJetton[14].left,m_RectJetton[14].top,m_RectJetton[14].Width(),m_RectJetton[14].Height());
	//提示信息
	if (!m_strGameHint.IsEmpty())
	{
		psurfTarget->DrawText(dwCenterX - 90, dwCenterY - 230, PIXEL(255,255,0), m_strGameHint.GetBuffer());
	}


		//绘画筹码
		for (INT i=0; i<INDEX_MAX; i++)
		{

			if (m_showPL[i]>9&&i<12)
			{
				m_GameRecord.DrawNumberString(psurfTarget, m_pImageStakeAmountTotal2, m_showPL[i], m_RectJetton[i].left-33,m_RectJetton[i].top+7);
			}
			if (m_showPL[i]>0&&m_showPL[i]<10&&i<12)
			{
				m_GameRecord.DrawNumberString(psurfTarget, m_pImageStakeAmountTotal2, m_showPL[i], m_RectJetton[i].left-27,m_RectJetton[i].top+7);
			}
		    //绘画下注金额
			if (m_lMyStakes[i]>0L&&m_lMyStakes[i]<10)
			{
				//画下注总额
				m_GameRecord.DrawNumberString(psurfTarget,  m_pImageStakeAmountTotal2, m_lMyStakes[i], m_RectJetton[i].right-27, m_RectJetton[i].top+7);
			}
			if (m_lMyStakes[i]>=10)
			{
				//画下注总额
				m_GameRecord.DrawNumberString(psurfTarget,  m_pImageStakeAmountTotal2, m_lMyStakes[i], m_RectJetton[i].right-33, m_RectJetton[i].top+7);
			}

		}



	char name[30];
	for (int i=0;i<INDEX_MAX;i++)
	{
		sprintf(name,"%ld",m_lAllStakes[i]);
        m_kuang->Blit(psurfTarget,m_RectJetton[i].left+32,m_RectJetton[i].top-5);
		psurfTarget->DrawText(m_RectJetton[i].left+32 +25-strlen(name)*3, m_RectJetton[i].top-3, PIXEL(255,255,0), name);
	}
            


	m_GameRecord.OnDraw(psurfTarget,m_pImageHistory,dwCenterX,dwCenterY);

	//绘画时间
	if (m_wMyChairID != INVALID_CHAIR&&m_pYP<=0)
	{
		m_pImageTimerbg->Blit(psurfTarget,m_time.x, m_time.y);
		DrawUserTimer(psurfTarget, m_time.x+27, m_time.y+22, m_timenum);
	}
	//绘画轮换庄家信息
	if (m_pImageShowBanker)
	{
		m_pImageShowBanker->Blit(psurfTarget, 0, 0, dwCenterX-m_pImageShowBanker->GetWidth()/2, dwCenterY-m_pImageShowBanker->GetHeight()/2, m_pImageShowBanker->GetWidth(), m_pImageShowBanker->GetHeight());
	}

	//庄家信息
	if ( m_wCurrentBanker != INVALID_CHAIR )
	{
		//获取玩家
		tagUserData const *pUserData = GetUserInfo( m_wCurrentBanker );

		if (pUserData )
		{
			m_Bankerbg->Blit(psurfTarget,dwCenterX-m_Bankerbg->GetWidth()/2,-30);
			//庄家
			_sntprintf(szBuffer, CountArray(szBuffer), TEXT("%s"), pUserData->szName);
			psurfTarget->DrawText(dwCenterX-5, dwCenterY - 337, PIXEL(255,0,0), szBuffer);

			//金币
			_sntprintf(szBuffer, CountArray(szBuffer), TEXT("%ld"), m_wBankerScore);
			psurfTarget->DrawText(dwCenterX-5 , dwCenterY - 317, PIXEL(255,0,0), szBuffer);

			//成绩
			_sntprintf(szBuffer, CountArray(szBuffer), TEXT("%s"),DrawNUM(m_wBankerCount<=20?m_wBankerCount:m_wBankerCount%20+1));
			psurfTarget->DrawText(dwCenterX-5 , dwCenterY - 296, PIXEL(255,0,0), szBuffer);

		}
	}


	const tagUserData * pUserData=GetUserInfo(m_wMyChairID);
	//昵称
	_sntprintf(szBuffer, CountArray(szBuffer), TEXT("%s"), pUserData->szName);

	psurfTarget->DrawText(dwCenterX -402-30, dwCenterY +260+20, PIXEL(255,0,0), szBuffer);
    //金币
	psurfTarget->DrawText(dwCenterX -402-30, dwCenterY +295+15, PIXEL(255,0,0), DrawNUM(pUserData->lScore));

	m_GameRecord.DrawNumberString(psurfTarget, m_pImageStakeAmountMine2, m_MyGame, 77,44);

	_sntprintf(szBuffer, CountArray(szBuffer), TEXT("上庄条件-> %ld 游戏币"),pGlobalDlg->m_lApplyBankerCondition);

	psurfTarget->DrawText(dwCenterX-495, dwCenterY+227, PIXEL(255,0,0), szBuffer);

	if (m_iSMyWin)
	{
			m_GameRecord.DrawNumberString(psurfTarget, m_pImageStakeAmountMine2, m_wMyWinCount, 60,65);
	}else
	{
		m_GameRecord.DrawNumberString(psurfTarget, m_pImageStakeAmountMine2, 0, 60,65);
	}

	m_GameRecord.DrawNumberString(psurfTarget, m_pImageStakeAmountMine2, m_Prizemoney,rc.Width()-92,46);

	if (m_showPrizeBZ)
	{ 
		m_Winbg->Blit(psurfTarget,-1,dwCenterY+180);

		m_WinTips->Blit(psurfTarget,m_pTipsX,dwCenterY+183);

		m_WinTitle->Blit(psurfTarget,0,m_Card*m_WinTitle->GetHeight()/12,m_pTipsX+403,dwCenterY+183,m_WinTitle->GetWidth(),m_WinTitle->GetHeight()/12);

		m_WinPrize->Blit(psurfTarget,m_showPrize*m_WinPrize->GetWidth()/5,0,840,30,m_WinPrize->GetWidth()/5,m_WinPrize->GetHeight());
	}

	if (m_GameBi)
	{
        m_pImagekuan->Blit(psurfTarget,m_pGameBZ.x, m_pGameBZ.y);
	}

	if (m_timezt==IDI_BET_TIME_OVER)
	{
		m_pImageplay->Blit(psurfTarget,m_time.x+5,m_time.y+50);

	}

	if (m_timezt==IDI_BET_TIME_SEND)
	{
		m_pImagesend->Blit(psurfTarget,m_time.x+5,m_time.y+50);
	}

	if (vBankerShow)
	{
		m_pImageBankbg->Blit(psurfTarget,dwCenterX-380 ,dwCenterY +100);
		m_AppList.DrawScore(psurfTarget,dwCenterX-340 ,dwCenterY +148);
	}

	if (m_wMyChairID==m_wCurrentBanker&&m_iSMyWin==true)
	{
		m_btnCancelBanker.EnableWindow(true);
	}
	if (m_wMyChairID==m_wCurrentBanker)
	{
		m_btnCancelBanker.EnableWindow(false);
	}




	}

	catch (...)
	{
	}

	return;
}
void CGameClientView::Drawumoney(SURFACE* psurfTarget,int g_money,int nX,int nXP,int nYP)
{
	if (nX==0)
	{
		return;
	}

	////加入筹码
	//for (int j=0; j<nX; j++)
	//{

	//	int nXPos,nYPos;
	//	nXPos=rand()%(200-m_pImageJettonView->GetHeight());
	//	nYPos=rand()%(80-m_pImageJettonView->GetHeight());
	//	m_pImageMoney->Blit(psurfTarget,g_money*45,0,nXP+nXPos,nYPos+nYP,45,45);
 //  	}
	
}
void CGameClientView::updateYazhumoney(SURFACE* psurfTarget,int nXPos,int nYPos)
{



}
char* CGameClientView::DrawNUM(INT64 num)
{
	TCHAR szBuffer[256];
	_sntprintf(szBuffer, CountArray(szBuffer), TEXT("%I64d"),num);
	int vf=1;
	if (num<0)
	{
		vf=2;
	}
	int cu = 0;
	s1="";
	s1=szBuffer;  
	for (int i=strlen(szBuffer)-1;i>=vf;i--)
	{
		cu++;
		if (cu%4 == 0)				
			s1.insert(i,",");		
	}
	return (char*)s1.c_str();

}
//绘画界面
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{  
	return;
}

void CGameClientView::DrawUserTimer(SURFACE* psurfTarget, int xPos, int yPos, int timer)
{
	int backWidth = 0;
	int backHeight = 0;

	if (m_pImageTimer)
	{
		int width = m_pImageTimer->GetWidth() / 10;
		int height = m_pImageTimer->GetHeight();
		int high = timer / 10;
		int low = timer% 10;
		m_pImageTimer->Blit(psurfTarget, width * high, 0, xPos - width,yPos -  height /2, width, height);
		m_pImageTimer->Blit(psurfTarget, width * low, 0, xPos +2, yPos- height/2, width, height);
	}

}

//设置最大下注额
void CGameClientView::SetMyMaxScore(LONG lMyMaxStake)
{
	//每个区域的最大下注
	//下注按钮
	if (m_wMyChairID==m_wCurrentBanker)
	{
		for (int i=CountArray(m_btnJettons)-1; i>=0; i--)
		{
			m_btnJettons[i].EnableWindow(false);
		}
		//更新界面
		UpdateGameView(NULL);
		return;
	}
	for (int i=CountArray(m_btnJettons)-1; i>=0; i--)
	{
            m_btnJettons[i].EnableWindow(lMyMaxStake+1 > JETTON_AMOUNT[i]);
	}

	//更新界面
	UpdateGameView(NULL);

	return;
}


//设置每个位置的最大下注额
void CGameClientView::SetAllMaxScore(const INT64 lMaxScore[])
{

	return;
}

//设置信息
void CGameClientView::SetMyStake(const INT64 plMyStakes[])
{


	//更新界面
	UpdateGameView(NULL);

	return;
}

void CGameClientView::InsertSystemString(LPCTSTR pszFormat, ...)
{
	ATLASSERT( AtlIsValidString( pszFormat ) );

	char szTemp[1024];   
	va_list argList;   
	va_start(argList, pszFormat);
	vsprintf(szTemp, pszFormat, argList);   
	va_end(argList);   
	AfxGetMainWnd()->SendMessage(IDM_INSERT_SYSTEM_STRING, (WPARAM)szTemp, 0);
}

void CGameClientView::SetMyStake(BYTE cbBetArea, LONG lBetAmount)
{
	ASSERT(cbBetArea < INDEX_MAX);

	if (m_lMyStakes[cbBetArea] != lBetAmount)
	{
		//设置变量
		m_lMyStakes[cbBetArea] = lBetAmount;

		//更新界面
		UpdateGameView(NULL);

		return;
	}

	return;

}

//设置筹码
void CGameClientView::SetCurrentJetton(LONG lCurrentJetton)
{
	//设置变量
	ASSERT(lCurrentJetton>=0L);

	m_lCurrentJetton = lCurrentJetton;


	return;
}


//开启游戏以来的累计数据
void CGameClientView::SetHistoryScore(WORD wWinCount, WORD wLossCount, INT64 lMyWinAmountSum)
{

	//更新界面
	UpdateGameView(NULL);

	return;
}

//清理筹码
void CGameClientView::CleanUserJetton()
{
	//清理数组
	//for (BYTE i=0; i<CountArray(m_JettonInfoArray); i++)
	//{
	//	m_JettonInfoArray[i].RemoveAll();
	//}

	ZeroMemory(m_lAllStakes, sizeof(m_lAllStakes));

	ZeroMemory(m_lMyStakes, sizeof(m_lMyStakes));

	//更新界面
	UpdateGameView(NULL);

	return;
}

//设置筹码
void CGameClientView::PlaceUserJetton(BYTE cbBetArea, LONG lBetAmount,int cbchar)
{
	//变量定义
	bool bHasChange = false;
	m_lAllStakes[cbBetArea] += lBetAmount;
	m_lMyStakes[cbBetArea] += lBetAmount;

	////增加筹码
	//for (int i=CountArray(JETTON_AMOUNT)-1; i>=0 && lBetAmount > 0; i--)
	//{
	//	//计算数目
	//	LONG lCellCount= lBetAmount / JETTON_AMOUNT[i];

	//	//插入过虑
	//	if (lCellCount==0L) continue;

	//	//加入筹码
	//	for (int j=0; j<lCellCount; j++)
	//	{
	//		//构造变量
	//		tagJettonInfo JettonInfo;
	//		JettonInfo.cbJettonIndex = i;
	//		JettonInfo.nXPos=JETTON_AREA_BORDER_W + rand()%(m_RectJetton[cbBetArea].Width()-m_pImageJettonView->GetHeight()-2*JETTON_AREA_BORDER_W);
	//		JettonInfo.nYPos=JETTON_AREA_BORDER_H + rand()%(m_RectJetton[cbBetArea].Height()-m_pImageJettonView->GetHeight()-5*JETTON_AREA_BORDER_H);

	//		//插入数组
	//		m_JettonInfoArray[cbBetArea].Add(JettonInfo);

	//		bHasChange = true;  //数据已改变SetTimer
	//	}

	//	//待处理金额减少
	//	lBetAmount -= lCellCount*JETTON_AMOUNT[i];
	//}

	////更新界面
	//if (bHasChange/*&&cbchar==m_wMyChairID*/)
	//{
	//	//UpdateGameView(NULL);
	//}

	return;
}

//设置筹码
void CGameClientView::PlaceUserJetton(const INT64 plBetAmounts[])
{
	for (BYTE cbBetArea = 0; cbBetArea < INDEX_MAX; cbBetArea++)
	{
		LONG lBetAmount = plBetAmounts[cbBetArea];
		
		if (lBetAmount > 0)
		{
			PlaceUserJetton(cbBetArea, lBetAmount,0);
		}
	}

	return;
}

void CGameClientView::SetWinAmount(LONG lWinAmount)
{

	SetTimer(IDI_SHOW_WIN_AMOUNT, 4000, NULL);
}

//设置胜方
void CGameClientView::SetWinner(BYTE cbPlayerValue, BYTE cbBankerValue, DWORD dwWinner)
{
	//设置变量
	m_dwWinnerFlash=dwWinner;
	m_cbPlayerValue = cbPlayerValue;
	m_cbBankerValue = cbBankerValue;
	m_nWinnerFlashIndex = 0 ;

	//设置时间
	if (dwWinner != NO_WINNER)
	{

		SetTimer(IDI_FLASH_WINNER, 500, NULL);

	}
	else
	{
		

	}

	//更新界面
	UpdateGameView(NULL);

	return;
}

void CGameClientView::SetGameHint(LPCTSTR pszFormat, ...)
{
	CString strNewHint;

	if (pszFormat == NULL)
	{
		strNewHint = pszFormat;
	}
	else
	{
		ATLASSERT( AtlIsValidString( pszFormat ) );
		va_list argList;
		va_start( argList, pszFormat );
		strNewHint.FormatV(pszFormat, argList);
		va_end( argList );
	}

	//防止多余的刷新操作
	if (strNewHint != m_strGameHint)
	{
		m_strGameHint = strNewHint;
		UpdateGameView(NULL);
	}
}

//获取区域
BYTE CGameClientView::GetJettonArea(CPoint MousePoint)
{
	for (BYTE i=0; i<CountArray(m_RectJetton); i++)
	{
		//判断当前点是否在矩形区域内, 5
		if (MousePoint.x >= m_RectJetton[i].left && MousePoint.x < m_RectJetton[i].right&&
			MousePoint.y >= m_RectJetton[i].top && MousePoint.y < m_RectJetton[i].bottom)

			return i;
	}


	return 0xFF;

}
//定时器消息
void CGameClientView::KillTimerFLASH()
{
     CGameFrameView::KillTimer(IDI_FLASH_WINNER);
	 for (int i=0;i<24;i++)
	 {
		 m_showTX[i]=0;
	 }
	 m_showPrizeBZ=false;
	 ISDrawlight=true;
	 m_bDrawWinArea=FALSE;
}

//定时器消息
void CGameClientView::TimerFLASH()
{
	CGameFrameView::KillTimer(IDI_FLASH_WINNER);
	m_showForT=true;
	if (g_Tips)
	{
		m_showPrizeBZ=true;
	}
	m_pTipsX=0;
	SetTimer(IDI_FLASH_WINNER, 50, NULL);
}
//定时器消息
void CGameClientView::CloseWin()
{
    CGameFrameView::KillTimer(IDI_FLASH_WIN);
	m_MyGame+=m_wMyWinCount;
    m_wMyWinCount=0;
	m_iSMyWin=false;

}
void CGameClientView::StartWin()
{
	CGameFrameView::KillTimer(IDI_FLASH_WIN);
	SetTimer(IDI_FLASH_WIN, 50, NULL);
}
void CGameClientView::StartHyMovie()
{
	CGameFrameView::KillTimer(IDI_HY);
	SetTimer(IDI_HY,100, NULL);
}
//定时器消息
void CGameClientView::OnTimer(UINT nIDEvent)
{



	if (nIDEvent == IDI_FLASH_WIN)
	{
		if (m_wMyWinCount<0)
		{
			m_wMyWinCount++;
			m_MyGame--;
			pGlobalDlg->PlayGameSound("H_SLWH\\Sound\\light.wav");
		}
		if (m_wMyWinCount>0)
		{
			m_wMyWinCount--;
			m_MyGame++;
		    pGlobalDlg->PlayGameSound("H_SLWH\\Sound\\light.wav");
		}
		
		if (m_wMyWinCount==0)
		{
			m_iSMyWin=false;
			CGameFrameView::KillTimer(IDI_FLASH_WIN);
		}
		UpdateGameView(NULL);
	    return;
	}
	if (nIDEvent == IDI_bank_show)
	{

		CGameFrameView::KillTimer(IDI_bank_show);
		vBankerShow=false;
		UpdateGameView(NULL);
		return;
	}
	//火焰
	if(nIDEvent == IDI_HY)
	{

		if (m_showForT)
		{
			for (int i=0;i<24;i++)
			{
				m_showTX[i]++;
			}
			//更新界面
			UpdateGameView(NULL);
		}else
		{
			for (int i=0;i<24;i++)
			{
				m_showTX[i]--;
			}
			//更新界面
			UpdateGameView(NULL);
		}

		if (m_showTX[23]==6&&m_showForT)
		{
			m_showForT=false;
		}
		if (m_showTX[23]==0&&!m_showForT)
		{
            CGameFrameView::KillTimer(IDI_HY);
		}
		return;
	}
	//闪动胜方
	if (nIDEvent==IDI_FLASH_WINNER)
	{
		if (m_showForT)
		{
			m_showTX[m_cbCardCount]++;
			UpdateGameView(NULL);
		}else
		{
			m_showTX[m_cbCardCount]--;
			//更新界面
			UpdateGameView(NULL);
		}

		if (m_showTX[m_cbCardCount]==6&&m_showForT)
		{
			m_showForT=false;
		}
		if (m_showTX[m_cbCardCount]==0&&!m_showForT)
		{
			m_showForT=true;
		}

		if (g_Tips)
		{
			m_showPrize++;

			if (cdPrize>0)
			{
				cdPrize--;
				m_Prizemoney--;
			}
			if (m_showPrize>=5)
			{
				m_showPrize=0;
			}
			if (g_Title)
			{
				m_pTipsX+=5;
			}
			else
			{
				m_pTipsX-=5;
			}
			if (m_pTipsX+700>=1020)
			{
				g_Title=false;
			}
			if (m_pTipsX<=0)
			{
				g_Title=true;
			}
		}
		//更新界面
		UpdateGameView(NULL);

		return;
	}

	//轮换庄家
	if ( nIDEvent == IDI_SHOW_CHANGE_BANKER )
	{
		m_pImageShowBanker = NULL;

		UpdateGameView(NULL);

		return;
	}

	if ( nIDEvent == IDI_SHOW_WIN_AMOUNT)
	{

		UpdateGameView(NULL);

		return;
	}



}

void CGameClientView::ShowWinnerFlash(SURFACE* psurfTarget, int nWidth, int nHeight)
{
	
}
void CGameClientView::OnMouseMove(UINT nFlags, CPoint point)
{
	__super::OnMouseMove(nFlags, point);
}

//鼠标消息
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	if (m_lCurrentJetton != 0L)
	{
		BYTE cbBetArea=GetJettonArea(Point);
		if (m_MyGame<m_lCurrentJetton)
		{
            return;
		}
         printf("====%d\n",m_lCurrentJetton);

		if (cbBetArea < INDEX_MAX && m_timezt==1&&m_lMyStakes[cbBetArea]<100)
		{
			AfxGetMainWnd()->SendMessage(IDM_PLACE_JETTON, cbBetArea, m_lCurrentJetton);
		}
	}

	__super::OnLButtonDown(nFlags,Point);

}

//鼠标消息
void CGameClientView::OnRButtonDown(UINT nFlags, CPoint Point)
{
	//设置变量
	m_lCurrentJetton=0L;
	if (m_iSMyWin)
	{
		CloseWin();
	}

	__super::OnLButtonDown(nFlags,Point);
}

//光标消息
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	if (m_lCurrentJetton == 0L)
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		return true;
	}

	//获取区域
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);
	BYTE m_MouseArea = GetJettonArea(MousePoint);


	//区域判断
	if (m_MouseArea == 0xFF)
	{
		SetGameHint(NULL);
	    SetCursor(LoadCursor(NULL, IDC_ARROW));
		return __super::OnSetCursor(pWnd, nHitTest, uMessage);
	}
	else if (m_MouseArea >= INDEX_MAX)
	{
		SetGameHint(NULL);
		SetCursor(LoadCursor(NULL, IDC_NO));
		return true;
	}	

	for (BYTE i=0; i<CountArray(JETTON_AMOUNT); i++)
	{
		if (m_lCurrentJetton ==JETTON_AMOUNT[i])
		{

			SetCursor(m_JettonCursors);
			break;
		}
	}

	return TRUE;
}


//庄家信息
void CGameClientView::SetBankerInfo( WORD wChairID, WORD wBankerCount, LONG lBankerWinScore ,LONG lBankerScore )
{
	//轮换庄家
	if (m_wCurrentBanker == wChairID && wBankerCount > 0)
	{
		CGameFrameView::KillTimer( IDI_SHOW_CHANGE_BANKER );
		m_pImageShowBanker = NULL;
	}
	else
	{
		if (wChairID == INVALID_CHAIR)
		{
			m_pImageShowBanker = m_pImageNoBanker;
			ZeroMemory(m_lAllMaxScore, sizeof(m_lAllMaxScore));
			ZeroMemory(m_lMyStakes, sizeof(m_lMyStakes));
			ZeroMemory(m_lAllStakes, sizeof(m_lAllStakes));

			ZeroMemory(m_JettonInfoArray, sizeof(m_JettonInfoArray));
			m_cbCardCount=0;
		}
		else if (wChairID == m_wMyChairID)
			m_pImageShowBanker = m_pImageMeBanker;
		else
			m_pImageShowBanker = m_pImageChangeBanker;

		SetTimer(IDI_SHOW_CHANGE_BANKER, 3000, NULL );
	}


	//设置变量
	m_wCurrentBanker = wChairID;
	m_wBankerCount = wBankerCount;			
	m_lBankerWinScore = lBankerWinScore;
	m_wBankerScore = lBankerScore;


	//更新界面
	UpdateGameView(NULL);

}

LRESULT CGameClientView::OnButtonClick(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	UINT nCommandID = LOWORD(lParam);
	if (nCommandID >= IDC_JETTON_SUPPER && nCommandID < (IDC_JETTON_SUPPER + 8))
	{
		CMD_C_TrueArea vchu;
		vchu.cbBetArea=nCommandID-IDC_JETTON_SUPPER;
		pGlobalDlg->SendData(SUB_C_PlAY_TRUE, &vchu, sizeof(vchu));
		return TRUE;
	}

	if (nCommandID == IDC_INFO_UP)
	{
		if (m_GameRecord.m_wPageNO > HISTORY_RECORD_SHOW)
		{
			m_GameRecord.m_wPageNO--;
		}
	}
	if (nCommandID == IDC_INFO_DOWN)
	{
		if (m_GameRecord.m_wPageNO < HISTORY_COUNT)
		{
			m_GameRecord.m_wPageNO++;
		}
	}
	if (nCommandID == IDC_APPLY_BANKER)
	{
		AfxGetMainWnd()->SendMessage(IDM_APPLY_BANKER, 1, 0);
		return TRUE;
	}

	if (nCommandID == IDC_CANCEL_BANKER)
	{
		AfxGetMainWnd()->SendMessage(IDM_APPLY_BANKER, 0, 0);
		return TRUE;
	}
	if(nCommandID==IDC_BANKER_SHOW)
	{
		vBankerShow=true;
		SetTimer(IDI_bank_show,3000, NULL);
		if (m_GameBi)
		{
			m_GameBi=false;
			m_pGame50.Show(false);					    //购买游戏币
			m_pGame100.Show(false);					    //购买游戏币
			m_pGame500.Show(false);					    //购买游戏币
			m_pGame1000.Show(false);					//购买游戏币
			m_pGameClose.Show(false);
		}
	}
	//如果点击的是游戏列表按钮
	if (nCommandID >= IDC_JETTON_BUTTON && nCommandID < (IDC_JETTON_BUTTON + CountArray(JETTON_AMOUNT)))
	{
		m_lCurrentJetton = JETTON_AMOUNT[nCommandID - IDC_JETTON_BUTTON];
	}

	if (nCommandID == IDC_CANCEL_Settlement)
	{
		if (m_iSMyWin)
		{
			CloseWin();
		}
		
		m_MyGame=0;
		CMD_C_Settlement vSettlement;
		vSettlement.Settlement=true;
		pGlobalDlg->SendData(SUB_C_Settlement, &vSettlement, sizeof(vSettlement));
	}

	if (nCommandID == IDC_CANCEL_FANG)
	{
		AfxGetMainWnd()->SendMessage(IDM_CANCEL_FANG, 0, 0);
	}

	if(nCommandID==IDC_GameBI)
	{
       m_GameBi=true;
	   m_pGame50.Show(true);					    //购买游戏币
	   m_pGame100.Show(true);					    //购买游戏币
	   m_pGame500.Show(true);					    //购买游戏币
	   m_pGame1000.Show(true);					    //购买游戏币
	   m_pGameClose.Show(true);
	
	}
	if(nCommandID==IDC_GameBI50)
	{
		CMD_C_FANG vchu;
		vchu.cbFang=50;
		pGlobalDlg->SendData(SUB_C_FANG, &vchu, sizeof(vchu));
	}
	if(nCommandID==IDC_GameBI100)
	{
		CMD_C_FANG vchu;
		vchu.cbFang=100;
		pGlobalDlg->SendData(SUB_C_FANG, &vchu, sizeof(vchu));
	}
	if(nCommandID==IDC_GameBI500)
	{
		CMD_C_FANG vchu;
		vchu.cbFang=500;
		pGlobalDlg->SendData(SUB_C_FANG, &vchu, sizeof(vchu));
	}
	if(nCommandID==IDC_GameBI1000)
	{
		CMD_C_FANG vchu;
		vchu.cbFang=1000;
		pGlobalDlg->SendData(SUB_C_FANG, &vchu, sizeof(vchu));
	}
	if(nCommandID==IDC_GameBIClose)
	{
		m_GameBi=false;
		m_pGame50.Show(false);					    //购买游戏币
		m_pGame100.Show(false);					    //购买游戏币
		m_pGame500.Show(false);					    //购买游戏币
		m_pGame1000.Show(false);					    //购买游戏币
		m_pGameClose.Show(false);
	}



	UpdateGameView(NULL);

   return true;
}

LRESULT CGameClientView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (g_pGameApp == NULL)
	{
		return DefWindowProc( message, wParam, lParam );
	}

	if (g_pGameApp->m_pMainGameWnd == NULL)
	{
		return DefWindowProc( message, wParam, lParam );
	}

	int flag = 0;

	//先处理应用程序相关的消息，不再转发给各个窗体
	switch(message)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;
	//case WM_SETCURSOR:
	//	{

	//	}
	//	break;
	case WM_PAINT:
		{
			RECT rc;

			if (GetUpdateRect(&rc, FALSE))
			{
				if (AfxGetViewManager())
				{
					g_pGameApp->m_pMainGameWnd->SelectUpdateClipRect(g_pGameApp->m_pMainGameWnd->GetWndRect());
					g_pGameApp->m_pMainGameWnd->Update(false);
					AfxGetViewManager()->UpdateScreen();
				}
			} 
			//UpdateGameView(NULL);
			return DefWindowProc(message, wParam, lParam);
		}
		break;
	case WM_SIZE:
		{
			CRect rc;
			GetWindowRect(&rc);
			if(rc.right - rc.left < g_pGameApp->m_rcMinApp.Width() + 1 
				&& rc.bottom - rc.top < g_pGameApp->m_rcMinApp.Height() + 1)
			{//防止窗口尺寸过小造成错误
				rc.left = 0;
				rc.top = 0;
				rc.right = g_pGameApp->m_rcMinApp.Width();
				rc.bottom = g_pGameApp->m_rcMinApp.Height();
			}
			else
			{
				rc.right--;
				rc.bottom--;
			}

			g_pGameApp->m_rcApp = rc;

			if (AfxGetViewManager())
			{
				rc.right = rc.right - rc.left;
				rc.left = 0;
				rc.bottom = rc.bottom - rc.top;
				rc.top = 0;
				g_pGameApp->m_pMainGameWnd->SetWndRect(rc);
				g_pGameApp->m_pMainGameWnd->SelectUpdateClipRect(g_pGameApp->m_pMainGameWnd->GetWndRect());
				g_pGameApp->m_pMainGameWnd->Update();
				AfxGetViewManager()->UpdateScreen();
			}
			//UpdateGameView(NULL);
		}   

		break;	
		/*case WM_TIMER:
		OnTimer(wParam);
		break;*/
	case WM_GETMINMAXINFO:
		{
			LPMINMAXINFO lpmmi = (LPMINMAXINFO)lParam;
			lpmmi->ptMaxPosition.x = 0;
			lpmmi->ptMaxPosition.y = 0;
			lpmmi->ptMaxSize.x = g_pGameApp->m_rcMaxApp.Width() + 1;
			lpmmi->ptMaxSize.y = g_pGameApp->m_rcMaxApp.Height() + 1;
		}
		break;
	default:
		{
			try
			{
				//if (g_pGameApp->m_pMainGameWnd->DispatchWndMsg(m_hWnd, message, wParam, lParam))
				//{
				if(g_pGameApp != NULL && g_pGameApp->m_pMainGameWnd != NULL)
				{
					POINTS pts = MAKEPOINTS(lParam);
					switch(message) 
					{
					case WM_LBUTTONDOWN:
					case WM_MBUTTONDOWN:
					case WM_RBUTTONDOWN:
						//case WM_LBUTTONUP:
						//case WM_MBUTTONUP:
						//case WM_RBUTTONUP:
					case WM_MOUSEMOVE:
						{

							g_pGameApp->m_ptCurMouse.x = pts.x;
							g_pGameApp->m_ptCurMouse.y = pts.y;
						}
						break;
					default:
						break;
					}
					flag = g_pGameApp->m_pMainGameWnd->DispatchWndMsg(m_hWnd, message, wParam, lParam);
					switch(message) 
					{
					case WM_LBUTTONDOWN:
					case WM_MBUTTONDOWN:
					case WM_RBUTTONDOWN:
						//case WM_LBUTTONUP:
						//case WM_MBUTTONUP:
						//case WM_RBUTTONUP:
					case WM_MOUSEMOVE:
						{
							//修改鼠标最后座标位置
							g_pGameApp->m_ptLastMouse.x = g_pGameApp->m_ptCurMouse.x;
							g_pGameApp->m_ptLastMouse.y = g_pGameApp->m_ptCurMouse.y;

						}
						break;
					default:
						break;
					}
				}
				//}
			}

			catch (...)
			{
			}
		}
		break;
	}
	if(flag == 1)
	{
		return 1;
	}

	return __super::WindowProc(message, wParam, lParam);


}	

//////////////////////////////////////////////////////////////////////////
