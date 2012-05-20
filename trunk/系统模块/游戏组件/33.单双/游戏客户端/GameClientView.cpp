#include "StdAfx.h"
#include "Math.h"
#include "Resource.h"
#include "GameClientView.h"
#include "Mmsystem.h"
#include ".\gameclientview.h"
#include "GameClientDlg.h"


//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_FLASH_WINNER			100									//������ʶ
#define IDI_SHOW_CHANGE_BANKER		101									//�ֻ�ׯ��
#define IDI_OPEN_DICEBOX			102									//������
#define IDI_CLOSE_DICEBOX			103									//�ر�����
#define IDI_SHAKE_DICEBOX			104									//ҡ������
#define IDI_SHOW_GAME_SCOE			105									//��ʾ�ɼ�

//��ť��ʶ
#define IDC_JETTON_BUTTON_500		200									//��ť��ʶ
#define IDC_JETTON_BUTTON_50000		201									//��ť��ʶ
#define IDC_JETTON_BUTTON_100		202									//��ť��ʶ
#define IDC_JETTON_BUTTON_1000		203									//��ť��ʶ
#define IDC_JETTON_BUTTON_10000		204									//��ť��ʶ
#define IDC_JETTON_BUTTON_100000	205									//��ť��ʶ
#define IDC_APPY_BANKER				206									//��ť��ʶ
#define IDC_CANCEL_BANKER			207									//��ť��ʶ
#define IDC_JETTON_BUTTON_500000	208									//��ť��ʶ
#define IDC_SCORE_MOVE_L			209									//��ť��ʶ
#define IDC_SCORE_MOVE_R			210									//��ť��ʶ

//ׯ����Ϣ
#define BANKER_INFO_LEN				150									//ׯ����Ϣ

#define RECORD_COUNT				10									//��¼����


//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_JETTON_BUTTON_500, OnJettonButton500)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_50000, OnJettonButton50000)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_500000, OnJettonButton500000)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_100, OnJettonButton100)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_1000, OnJettonButton1000)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_10000, OnJettonButton10000)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_100000, OnJettonButton100000)
	ON_BN_CLICKED(IDC_APPY_BANKER, OnApplyBanker)
	ON_BN_CLICKED(IDC_CANCEL_BANKER, OnCancelBanker)
	ON_BN_CLICKED(IDC_SCORE_MOVE_L, OnScoreMoveL)
	ON_BN_CLICKED(IDC_SCORE_MOVE_R, OnScoreMoveR)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView() : CGameFrameView(true,24)
{
	//״̬��Ϣ
	m_lCurrentJetton=0L;
	m_cbWinnerSide=0xFF;
	m_cbWinnerFlash=0xFF;
	m_wMeChairID=INVALID_CHAIR;
	m_cbPreJettonArea = 255;

	m_bShowGameResult = false;
	m_lMeCurGameScore=0L;			
	m_lMeCurGameReturnScore=0L;	
	m_lBankerCurGameScore=0L;		

	m_lAreaLimitScore=0L;	
	m_bShowCloseDicebox = false;
	m_cbTimeIndex=0;

	//���ݱ���
	ZeroMemory(m_cbDice, sizeof(m_cbDice));

	//��ע��Ϣ
	m_lAllOddScore=0L;			
	m_lAllEvenScore=0L;		

	//λ����Ϣ
	m_nScoreHead = 0;
	m_nRecordFirst= 0;	
	m_nRecordLast= 0;

	//��ʷ�ɼ�
	m_wDrawCount=0;
	m_lMeResultCount=0;

	//��ע��Ϣ
	m_lMeMaxScore=0L;
	m_lMeOddScore=0L;
	m_lMeEvenScore=0L;

	//ׯ����Ϣ
	m_bShowChangeBanker = false;
	m_wCurrentBankerChairID = INVALID_CHAIR;
	m_cbBankerTime = 0;
	m_lBankerScore = 0;
	m_lBankerTreasure=0;

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageViewFill.SetLoadInfo(IDB_VIEW_FILL,hInstance);
	m_ImageViewBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_ImageWinFlags.SetLoadInfo(IDB_WIN_FLAGS,hInstance);
	m_ImageJettonView.SetLoadInfo(IDB_JETTOM_VIEW,hInstance);
	m_ImageScoreNumber.SetLoadInfo(IDB_SCORE_NUMBER,hInstance);
	m_ImageMeScoreNumber.SetLoadInfo(IDB_ME_SCORE_NUMBER,hInstance);

	m_ImageFrameOdd.SetLoadInfo( IDB_FRAME_ODD, hInstance );
	m_ImageFrameEven.SetLoadInfo( IDB_FRAME_EVEN, hInstance );

	m_ImageGameEnd.SetLoadInfo( IDB_GAME_END, hInstance );

	m_ImageMeBanker.SetLoadInfo( IDB_ME_BANKER, hInstance );
	m_ImageChangeBanker.SetLoadInfo( IDB_CHANGE_BANKER, hInstance );
	m_ImageNoBanker.SetLoadInfo( IDB_NO_BANKER, hInstance );	

	m_ImageTimeFlag.SetLoadInfo(IDB_TIME_FLAG, hInstance);

	m_ImageDicebox.SetLoadInfo(IDB_DICEBOX, hInstance);
	m_ImageDice.SetLoadInfo(IDB_DICE, hInstance);
	m_ImageCloseDicebox.SetLoadInfo(IDB_CLOSE_DICEBOX, hInstance);
	m_ImageWinFlag.SetLoadInfo(IDB_WIN_FLAG, hInstance);
	m_ImageEndSocre.SetLoadInfo(IDB_END_SCORE_NUMBER, hInstance);

	//��������
	m_cbOpenDiceboxIndex=255;
	m_cbCloseDiceboxIndex=255;
	m_cbShakeDiceboxIndex=255;

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
	CRect rcCreate(0,0,0,0);
	m_ApplyUser.Create( IDD_DLG_GAME_RECORD	, this );
	
	//������ť
	m_btJetton100.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_100);
	m_btJetton1000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_1000);
	m_btJetton10000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_10000);
	m_btJetton100000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_100000);
	m_btJetton500.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_500);
	m_btJetton50000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_50000);
	m_btJetton500000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_500000);	
	
	m_btApplyBanker.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_APPY_BANKER);
	m_btCancelBanker.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_CANCEL_BANKER);
	m_btApplyBanker.ShowWindow(SW_SHOW);
	m_btApplyBanker.EnableWindow(FALSE);
	m_btCancelBanker.ShowWindow(SW_HIDE);

	m_btScoreMoveL.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_L);
	m_btScoreMoveR.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_R);

	//���ð�ť
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_btJetton100.SetButtonImage(IDB_BT_JETTON_100,hResInstance,false);
	m_btJetton1000.SetButtonImage(IDB_BT_JETTON_1000,hResInstance,false);
	m_btJetton10000.SetButtonImage(IDB_BT_JETTON_10000,hResInstance,false);
	m_btJetton100000.SetButtonImage(IDB_BT_JETTON_100000,hResInstance,false);
	m_btJetton500.SetButtonImage(IDB_BT_JETTON_500,hResInstance,false);	
	m_btJetton50000.SetButtonImage(IDB_BT_JETTON_50000,hResInstance,false);
	m_btJetton500000.SetButtonImage(IDB_BT_JETTON_500000,hResInstance,false);	
	
	m_btApplyBanker.SetButtonImage(IDB_BT_APPLY_BANKER,hResInstance,false);
	m_btCancelBanker.SetButtonImage(IDB_BT_CANCEL_APPLY,hResInstance,false);

	m_btScoreMoveL.SetButtonImage(IDB_BT_SCORE_MOVE_L,hResInstance,false);
	m_btScoreMoveR.SetButtonImage(IDB_BT_SCORE_MOVE_R,hResInstance,false);

	return 0;
}

//���ý���
void CGameClientView::ResetGameView()
{
	//ׯ����Ϣ
	m_bShowChangeBanker = false;
	m_wCurrentBankerChairID = INVALID_CHAIR;
	m_cbBankerTime = 0;
	m_lBankerScore = 0;	
	m_lMeMaxScore=0L;
	m_lMeOddScore=0L;
	m_lMeEvenScore=0L;

	m_cbTimeIndex=0;

	//���ݱ���
	ZeroMemory(m_cbDice, sizeof(m_cbDice));

	m_bShowGameResult = false;
	m_lMeCurGameScore=0L;			
	m_lMeCurGameReturnScore=0L;	
	m_lBankerCurGameScore=0L;
	m_nScoreHead = 0;
	m_nRecordFirst= 0;	
	m_nRecordLast= 0;	
	m_lBankerTreasure=0;

	m_bShowCloseDicebox = false;
    
	m_lAreaLimitScore=0L;	

	//��ע��Ϣ
	m_lAllOddScore=0L;			
	m_lAllEvenScore=0L;		

	//����б�
	m_ApplyUser.ClearAll();

	//���ð�ť
	m_btApplyBanker.ShowWindow(SW_SHOW);
	m_btApplyBanker.EnableWindow(FALSE);
	m_btCancelBanker.ShowWindow(SW_HIDE);
	m_btCancelBanker.SetButtonImage(IDB_BT_CANCEL_APPLY,AfxGetInstanceHandle(),false);

	//��������
	m_cbOpenDiceboxIndex=255;
	m_cbCloseDiceboxIndex=255;
	m_cbShakeDiceboxIndex=255;

	SetCartoon(0, false);

	return;
}

//�����ؼ�
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	//λ����Ϣ
	m_nWinFlagsExcursionX = nWidth/2-182;
	m_nWinFlagsExcursionY = nHeight/2 + 253;

	//����λ��
	m_PointJetton[0].SetPoint(nWidth / 2 - 320,nHeight / 2 - 210);
	m_PointJetton[1].SetPoint(nWidth / 2 + 135,nHeight / 2 - 210);

	//�ƶ��ؼ�
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;

	//�б�ؼ�
	DeferWindowPos(hDwp,m_ApplyUser,NULL,nWidth/2 + 174,nHeight/2-314+566,201,92,uFlags);

	//���밴ť
	CRect rcJetton;
	m_btJetton100.GetWindowRect(&rcJetton);
	int nYPos = nHeight/2+148;
	int nXPos = nWidth/2-160+11;
	int nSpace = 2;
	DeferWindowPos(hDwp,m_btJetton100,NULL,nXPos,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton500,NULL,nXPos + nSpace + rcJetton.Width(),nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton1000,NULL,nXPos + nSpace * 2 + rcJetton.Width() * 2,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton10000,NULL,nXPos + nSpace * 3 + rcJetton.Width() * 3,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton50000,NULL,nXPos + nSpace * 4 + rcJetton.Width() * 4,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton100000,NULL,nXPos + nSpace * 5 + rcJetton.Width() * 5,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton500000,NULL,nXPos + nSpace * 6 + rcJetton.Width() * 6,nYPos,0,0,uFlags|SWP_NOSIZE);

	//��ׯ��ť
	DeferWindowPos(hDwp,m_btApplyBanker,NULL,nWidth/2+280,nHeight/2+217,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btCancelBanker,NULL,nWidth/2+280,nHeight/2+217,0,0,uFlags|SWP_NOSIZE);

	DeferWindowPos(hDwp,m_btScoreMoveL,NULL,nWidth/2-240,nHeight/2+272,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btScoreMoveR,NULL,nWidth/2+106,nHeight/2+268,0,0,uFlags|SWP_NOSIZE);

	//�����ƶ�
	EndDeferWindowPos(hDwp);

	return;
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//�滭����
	DrawViewImage(pDC,m_ImageViewFill,enMode_Spread);
	DrawViewImage(pDC,m_ImageViewBack,enMode_Centent);

	//ʤ����־
	DrawWinFlags(pDC);

	//�滭����
	DrawCartoon(pDC, nWidth, nHeight);

	//��ʾɫ��
	if ( m_cbDice[0] != 0 && m_cbDice[1] != 0 && m_cbOpenDiceboxIndex==255 && m_cbCloseDiceboxIndex==255 && m_cbShakeDiceboxIndex==255 )
	{
		//�滭ɫ��
		CImageHandle ImageHandleDicebox(&m_ImageDicebox);
		m_ImageDicebox.AlphaDrawImage(pDC, nWidth/2-200, nHeight/2-355, RGB(255,0,255));
	}
	if ( m_bShowCloseDicebox )
	{
		//�滭ɫ��
		CImageHandle ImageHandleCloseDicebox(&m_ImageCloseDicebox);
		m_ImageCloseDicebox.AlphaDrawImage(pDC, nWidth/2-200, nHeight/2-355, RGB(255,0,255));
	}

	//��ʾɫ��
	bool bShowDice = false;
	if ( m_cbDice[0] != 0 && m_cbDice[1] != 0 && 8 <= m_cbOpenDiceboxIndex && m_cbCloseDiceboxIndex == 255 ) bShowDice = true;
	else if ( m_cbDice[0] != 0 && m_cbDice[1] != 0 && 255 == m_cbOpenDiceboxIndex && m_cbCloseDiceboxIndex <= 6 ) bShowDice = true;
	if ( bShowDice )
	{
		//�滭ɫ��
		CImageHandle ImageHandleDice(&m_ImageDice);
		int nDiceWidth = m_ImageDice.GetWidth()/6;
		int nDiceHeight = m_ImageDice.GetHeight();

		bool bShowLeftDice = true, bShowRightDice = true;

		if ( m_cbCloseDiceboxIndex >= 6 && m_cbCloseDiceboxIndex != 255 ) bShowLeftDice = false;
		if ( m_cbOpenDiceboxIndex <= 8 && m_cbOpenDiceboxIndex != 255 ) bShowLeftDice = false;
		if ( bShowLeftDice )
			m_ImageDice.AlphaDrawImage(pDC, nWidth/2-38, nHeight/2-235, nDiceWidth, nDiceHeight, (m_cbDice[0]-1)*nDiceWidth, 0, RGB(255,0,255));
		if ( bShowRightDice )
			m_ImageDice.AlphaDrawImage(pDC, nWidth/2-38+5+nDiceWidth, nHeight/2-235, nDiceWidth, nDiceHeight, (m_cbDice[1]-1)*nDiceWidth, 0, RGB(255,0,255));
	}

	//��ʾ���
	ShowGameResult(pDC, nWidth, nHeight);

	//ʱ���ʶ
	bool bDispatchDice = (m_cbDice[0] + m_cbDice[1] != 0 ) ? true : false;
	CImageHandle ImageHandleTimeFlag(&m_ImageTimeFlag);
	int nTimeFlagWidth = m_ImageTimeFlag.GetWidth()/3;
	m_ImageTimeFlag.BitBlt(pDC->GetSafeHdc(), nWidth/2-230, nHeight/2+195, nTimeFlagWidth, m_ImageTimeFlag.GetHeight(),m_cbTimeIndex * nTimeFlagWidth, 0);
        
	//�����ע
	DrawNumberString(pDC,GetMaxOddScore(),nWidth/2+30, nHeight/2 + 80, true);
	DrawNumberString(pDC,GetMaxEvenScore(),nWidth/2+30, nHeight/2 + 110, true);

	//�滭�߿�
	int nXPos=0, nYPos=0;
	if ( m_lCurrentJetton == 0 ) m_cbPreJettonArea = 255;
	if ( m_cbPreJettonArea == ID_ODD )
	{
		CImageHandle ImageHandleOdd(&m_ImageFrameOdd);
		nXPos = nWidth / 2 - 380;
		nYPos = nHeight / 2 - 262;

		m_ImageFrameOdd.AlphaDrawImage(pDC,nXPos, nYPos , RGB(255,0,255));

	}
	else if ( m_cbPreJettonArea ==  ID_EVEN )
	{
		CImageHandle ImageHandleEven(&m_ImageFrameEven);

		nXPos = nWidth / 2 + 99;
		nYPos = nHeight / 2 - 262;
		m_ImageFrameEven.AlphaDrawImage(pDC,nXPos, nYPos , RGB(255,0,255));
	}

	//ʤ����־
	if ( m_cbWinnerFlash != 0xFF )
	{
		if ( m_cbWinnerSide == ID_ODD )
		{
			CImageHandle ImageHandleOdd(&m_ImageFrameOdd);

			nXPos = nWidth / 2 - 380;
			nYPos = nHeight / 2 - 262;
			m_ImageFrameOdd.AlphaDrawImage(pDC,nXPos, nYPos , RGB(255,0,255));
		}
		else if ( m_cbWinnerSide == ID_EVEN )
		{
			CImageHandle ImageHandleEven(&m_ImageFrameEven);

			nXPos = nWidth / 2 + 99;
			nYPos = nHeight / 2 - 262;
			m_ImageFrameEven.AlphaDrawImage(pDC,nXPos, nYPos , RGB(255,0,255));
		}
	}

	//�л�ׯ��
	if ( m_bShowChangeBanker )
	{
		int	nXPos = nWidth / 2 - 125;
		int	nYPos = nHeight / 2 - 260;

		//������ׯ
		if ( m_wMeChairID == m_wCurrentBankerChairID )
		{
			CImageHandle ImageHandleBanker(&m_ImageMeBanker);
			m_ImageMeBanker.AlphaDrawImage(pDC, nXPos, nYPos, RGB(255,0,255));
		}
		else if ( m_wCurrentBankerChairID != INVALID_CHAIR )
		{
			CImageHandle ImageHandleBanker(&m_ImageChangeBanker);
			m_ImageChangeBanker.AlphaDrawImage(pDC, nXPos, nYPos, RGB(255,0,255));
		}
		else
		{
			CImageHandle ImageHandleBanker(&m_ImageNoBanker);
			m_ImageNoBanker.AlphaDrawImage(pDC, nXPos, nYPos, RGB(255,0,255));
		}
	}
	//ׯ����Ϣ
	pDC->SetTextColor(RGB(255,0,0));
	if ( m_wCurrentBankerChairID != INVALID_CHAIR )
	{
		//��ȡ���
		tagUserData const *pUserData = GetUserInfo( m_wCurrentBankerChairID );
		if ( pUserData )
		{
			//λ����Ϣ
			CRect StrRect;
			StrRect.left = nWidth/2 - 314;
			StrRect.top = nHeight/2+223;
			StrRect.right = StrRect.left + 100;
			StrRect.bottom = StrRect.top + 21;

			//ׯ������
			pDC->DrawText( pUserData->szName, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP| DT_SINGLELINE );

			//ׯ���ܷ�
			CString strBankerTotalScore;
			strBankerTotalScore.Format( "%d", pUserData->lScore );
			StrRect.left = nWidth/2 - 167;
			StrRect.top = nHeight/2+223;
			StrRect.right = StrRect.left + 100;
			StrRect.bottom = StrRect.top + 21;
			pDC->DrawText( strBankerTotalScore, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP| DT_SINGLELINE );

			//ׯ�ҳɼ�
			CString strBankerTime;
			strBankerTime.Format( "%d", m_lBankerScore );
			StrRect.left = nWidth/2 - 20;
			StrRect.top = nHeight/2+223;
			StrRect.right = StrRect.left + 100;
			StrRect.bottom = StrRect.top + 21;
			pDC->DrawText( strBankerTime, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP| DT_SINGLELINE );

			//ׯ�Ҿ���
			CString strBankerScore;
			strBankerScore.Format( "%ld", m_cbBankerTime );
			StrRect.left = nWidth/2 + 127;
			StrRect.top = nHeight/2+223;
			StrRect.right = StrRect.left + 100;
			StrRect.bottom = StrRect.top + 21;
			pDC->DrawText( strBankerScore, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP| DT_SINGLELINE );
		}
	}
	else
	{
		//λ����Ϣ
		CRect StrRect;
		StrRect.left = nWidth/2 - 314;
		StrRect.top = nHeight/2+223;
		StrRect.right = StrRect.left + 100;
		StrRect.bottom = StrRect.top + 21;

		//ׯ������
		pDC->DrawText( TEXT("������ׯ"), StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP| DT_SINGLELINE );

		//ׯ�Ҿ���
		StrRect.left = nWidth/2 - 167;
		StrRect.top = nHeight/2+223;
		StrRect.right = StrRect.left + 100;
		StrRect.bottom = StrRect.top + 21;
		pDC->DrawText( "......", StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP| DT_SINGLELINE );

		//ׯ�ҳɼ�
		StrRect.left = nWidth/2 - 20;
		StrRect.top = nHeight/2+223;
		StrRect.right = StrRect.left + 100;
		StrRect.bottom = StrRect.top + 21;
		pDC->DrawText( "......", StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP| DT_SINGLELINE );

		//ׯ�ҳɼ�
		StrRect.left = nWidth/2 + 127;
		StrRect.top = nHeight/2+223;
		StrRect.right = StrRect.left + 100;
		StrRect.bottom = StrRect.top + 21;
		pDC->DrawText( "......", StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP| DT_SINGLELINE );
	}

	//������Դ
	CImageHandle HandleJettonView(&m_ImageJettonView);
	CSize SizeJettonItem(m_ImageJettonView.GetWidth()/JETTON_COUNT,m_ImageJettonView.GetHeight());

	//�滭����
	for (INT i=0;i<2;i++)
	{
		//��������
		LONG lScoreCount=0L;
		LONG lScoreJetton[JETTON_COUNT]={100L,500L,1000L,10000L,50000L,100000L,500000L};

		//�滭����
		for (INT_PTR j=0;j<m_JettonInfoArray[i].GetCount();j++)
		{
			//��ȡ��Ϣ
			tagJettonInfo * pJettonInfo=&m_JettonInfoArray[i][j];

			//�ۼ�����
			ASSERT(pJettonInfo->cbJettonIndex<JETTON_COUNT);
			lScoreCount+=lScoreJetton[pJettonInfo->cbJettonIndex];

			//�滭����
			m_ImageJettonView.AlphaDrawImage(pDC,pJettonInfo->nXPos+m_PointJetton[i].x-SizeJettonItem.cx/2,
				pJettonInfo->nYPos+m_PointJetton[i].y-SizeJettonItem.cy/2,SizeJettonItem.cx,SizeJettonItem.cy,
				pJettonInfo->cbJettonIndex*SizeJettonItem.cx,0,RGB(255,0,255));
		}

		//�滭����
		if (lScoreCount>0L) DrawNumberString(pDC,lScoreCount,m_PointJetton[i].x+90,m_PointJetton[i].y+160);
	}

	//�ҵ���ע
	DrawMeJettonNumber(pDC);

	//�滭ʱ��
	if (m_wMeChairID!=INVALID_CHAIR)
	{
		WORD wUserTimer=GetUserTimer(m_wMeChairID);
		if (wUserTimer!=0) DrawUserTimer(pDC,nWidth/2-260,nHeight/2-260+435,wUserTimer);
	}

	//�滭�û�
	if (m_wMeChairID!=INVALID_CHAIR)
	{
		const tagUserData *pMeUserData = GetUserInfo(m_wMeChairID);

		//��Ϸ��Ϣ
		if ( pMeUserData != NULL )
		{
			TCHAR szResultScore[16]=TEXT("");
			TCHAR szGameScore[16]=TEXT("");
			pDC->SetTextColor(RGB(0,0,0));
			LONG lMeJetton = m_lMeOddScore+m_lMeEvenScore;
			_sntprintf(szGameScore,CountArray(szGameScore),TEXT("%ld"),pMeUserData->lScore-lMeJetton);
			_sntprintf(szResultScore,CountArray(szResultScore),TEXT("%ld"),m_lMeResultCount);
			CRect rcAccount(CPoint(nWidth/2-315,nHeight/2+266),CPoint(nWidth/2-315+60,nHeight/2+268+14));
			CRect rcGameScore(CPoint(nWidth/2-315,nHeight/2+292),CPoint(nWidth/2-315+60,nHeight/2+292+10));
			CRect rcResultScore(CPoint(nWidth/2-315,nHeight/2+294+23),CPoint(nWidth/2-315+60,nHeight/2+294+10+23));
			pDC->DrawText(pMeUserData->szName,lstrlen(pMeUserData->szName),rcAccount,DT_VCENTER|DT_SINGLELINE|DT_CENTER|DT_END_ELLIPSIS);
			pDC->DrawText(szGameScore,lstrlen(szGameScore),rcGameScore,DT_VCENTER|DT_SINGLELINE|DT_CENTER|DT_END_ELLIPSIS);
			pDC->DrawText(szResultScore,lstrlen(szResultScore),rcResultScore,DT_VCENTER|DT_SINGLELINE|DT_CENTER|DT_END_ELLIPSIS);
		}
	}

	return;
}

//������Ϣ
void CGameClientView::SetMeMaxScore(LONG lMeMaxScore)
{
	if (m_lMeMaxScore!=lMeMaxScore)
	{
		//���ñ���
		m_lMeMaxScore=lMeMaxScore;

		//���½���
		UpdateGameView(NULL);
	}

	return;
}


//������Ϣ
void CGameClientView::SetMeOddScore(LONG lMeOddScore)
{
	if (m_lMeOddScore!=lMeOddScore)
	{
		//���ñ���
		m_lMeOddScore=lMeOddScore;

		//���½���
		UpdateGameView(NULL);
	}

	return;
}

//�����ע
void CGameClientView::SetAreaLimitScore(LONG lAreaLimitScore)
{
	if ( m_lAreaLimitScore!= lAreaLimitScore )
	{
		//���ñ���
		m_lAreaLimitScore=lAreaLimitScore;
	}
}

//������Ϣ
void CGameClientView::SetMeEvenScore(LONG lMeEvenScore)
{
	if (m_lMeEvenScore!=lMeEvenScore)
	{
		//���ñ���
		m_lMeEvenScore=lMeEvenScore;

		//���½���
		UpdateGameView(NULL);
	}

	return;
}

//���ó���
void CGameClientView::SetCurrentJetton(LONG lCurrentJetton)
{
	//���ñ���
	ASSERT(lCurrentJetton>=0L);
	m_lCurrentJetton=lCurrentJetton;

	return;
}


//��ʷ��¼
void CGameClientView::SetGameHistory(enOperateResult OperateResult, BYTE cbDiceCount)
{
	//��������
	tagClientGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	GameRecord.enOperateFlags = OperateResult;
	GameRecord.cbDiceCount = cbDiceCount;

	//�ƶ��±�
	m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst )
	{
		m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;
		if ( m_nScoreHead < m_nRecordFirst ) m_nScoreHead = m_nRecordFirst;
	}

	int nHistoryCount = (m_nRecordLast - m_nRecordFirst + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
	if ( RECORD_COUNT < nHistoryCount ) m_btScoreMoveR.EnableWindow(TRUE);

	//�Ƶ����¼�¼
	if ( RECORD_COUNT < nHistoryCount )
	{
		m_nScoreHead = (m_nRecordLast - RECORD_COUNT + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
		m_btScoreMoveL.EnableWindow(TRUE);
		m_btScoreMoveR.EnableWindow(FALSE);
	}

	//���½���
	UpdateGameView(NULL);

	return;
}

//��ʷ�ɼ�
void CGameClientView::SetHistoryScore(WORD wDrawCount,LONG lMeResultCount)
{
	//���ñ���
	m_wDrawCount=wDrawCount;
	m_lMeResultCount=lMeResultCount;

	//���½���
	UpdateGameView(NULL);

	return;
}

//�������
void CGameClientView::CleanUserJetton()
{
	//��������
	for (BYTE i=0;i<CountArray(m_JettonInfoArray);i++)
	{
		m_JettonInfoArray[i].RemoveAll();
	}

	//����ɫ��
	ZeroMemory(m_cbDice, sizeof(m_cbDice));

	//��ע��Ϣ
	m_lAllOddScore=0L;
	m_lAllEvenScore=0L;

	//���½���
	UpdateGameView(NULL);

	return;
}

//���ó���
void CGameClientView::PlaceUserJetton(BYTE cbViewIndex, LONG lScoreCount)
{
	//Ч�����
	ASSERT(cbViewIndex<=ID_EVEN);
	if (cbViewIndex>ID_EVEN) return;

	//��������
	bool bPlaceJetton=false;
	LONG lScoreIndex[JETTON_COUNT]={100L,500L,1000L,10000L,50000L,100000L,500000L};

	//�߿���
	int nFrameWidth = 200, nFrameHeight = 340;

	if ( cbViewIndex == ID_ODD ) m_lAllOddScore += lScoreCount;
	else if ( cbViewIndex == ID_EVEN ) m_lAllEvenScore += lScoreCount;


	//���ӳ���
	for (BYTE i=0;i<CountArray(lScoreIndex);i++)
	{
		//������Ŀ
		BYTE cbScoreIndex=JETTON_COUNT-i-1;
		LONG lCellCount=lScoreCount/lScoreIndex[cbScoreIndex];

		//�������
		if (lCellCount==0L) continue;

		//�������
		for (LONG j=0;j<lCellCount;j++)
		{
			//����λ��
			INT nXPos=rand()%JETTON_RADII;
			INT nYPos=(INT)sqrt((FLOAT)(rand()%(JETTON_RADII*JETTON_RADII-nXPos*nXPos)));

			//�������
			tagJettonInfo JettonInfo;
			JettonInfo.cbJettonIndex=cbScoreIndex;
			JettonInfo.nXPos=rand()%nFrameWidth;
			JettonInfo.nYPos=rand()%nFrameHeight;
			if ( JettonInfo.nXPos > 30 ) JettonInfo.nXPos -= 30;
			if ( JettonInfo.nYPos > 30 ) JettonInfo.nYPos -= 30;

			//��������
			bPlaceJetton=true;
			m_JettonInfoArray[cbViewIndex-1].Add(JettonInfo);
		}

		//������Ŀ
		lScoreCount-=lCellCount*lScoreIndex[cbScoreIndex];
	}

	//���½���
	if (bPlaceJetton==true) UpdateGameView(NULL);

	return;
}

//����ʤ��
void CGameClientView::SetWinnerSide(BYTE cbWinnerSide)
{
	//���ñ���
	m_cbWinnerSide=cbWinnerSide;
	m_cbWinnerFlash=cbWinnerSide;

	//����ʱ��
	if (cbWinnerSide!=0xFF)
	{
		SetTimer(IDI_FLASH_WINNER,500,NULL);
	}
	else KillTimer(IDI_FLASH_WINNER);

	//���½���
	UpdateGameView(NULL);

	return;
}

//��ȡ����
BYTE CGameClientView::GetJettonArea(CPoint MousePoint)
{
	//�����ж�
	if ( HitOddAreaTest(MousePoint) ) return ID_ODD;
	if ( HitEvenAreaTest(MousePoint) ) return ID_EVEN;

	return 0xFF;
}

//�滭����
void CGameClientView::DrawNumberString(CDC * pDC, LONG lNumber, INT nXPos, INT nYPos, bool bMeScore)
{
	//������Դ
	CImageHandle HandleScoreNumber(&m_ImageScoreNumber);
	CImageHandle HandleMeScoreNumber(&m_ImageMeScoreNumber);
	CSize SizeScoreNumber(m_ImageScoreNumber.GetWidth()/10,m_ImageScoreNumber.GetHeight());

	if ( bMeScore ) SizeScoreNumber.SetSize(m_ImageMeScoreNumber.GetWidth()/10, m_ImageMeScoreNumber.GetHeight());

	//������Ŀ
	LONG lNumberCount=0;
	LONGLONG lNumberTemp=lNumber;
	do
	{
		lNumberCount++;
		lNumberTemp/=10;
	} while (lNumberTemp>0);

	//λ�ö���
	INT nYDrawPos=nYPos-SizeScoreNumber.cy/2;
	INT nXDrawPos=nXPos+lNumberCount*SizeScoreNumber.cx/2-SizeScoreNumber.cx;

	//�滭����
	for (LONG i=0;i<lNumberCount;i++)
	{
		//�滭����
		LONG lCellNumber=(LONG)(lNumber%10);
		if ( bMeScore )
		{
			m_ImageMeScoreNumber.AlphaDrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
				lCellNumber*SizeScoreNumber.cx,0,RGB(255,0,255));
		}
		else
		{
			m_ImageScoreNumber.AlphaDrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
				lCellNumber*SizeScoreNumber.cx,0,RGB(255,0,255));
		}

		//���ñ���
		lNumber/=10;
		nXDrawPos-=SizeScoreNumber.cx;
	};

	return;
}

//�滭����
void CGameClientView::DrawEndNumberString(CDC * pDC, LONG lNumber, INT nXPos, INT nYPos)
{
	//������Դ
	CImageHandle HandleScoreNumber(&m_ImageEndSocre);
	CSize SizeScoreNumber(m_ImageEndSocre.GetWidth()/11,m_ImageEndSocre.GetHeight());

	//������Ŀ
	LONG lNumberCount=0;
	LONGLONG lNumberTemp=abs(lNumber);
	do
	{
		lNumberCount++;
		lNumberTemp/=10;
	} while (lNumberTemp>0);
	if ( lNumber < 0 ) lNumberCount += 1;

	//λ�ö���
	INT nYDrawPos=nYPos-SizeScoreNumber.cy/2;
	INT nXDrawPos=nXPos+lNumberCount*SizeScoreNumber.cx/2-SizeScoreNumber.cx;

	//�滭����
	int nIndex = 0;
	if ( lNumber < 0 )
	{
		lNumber *= -1;
		m_ImageEndSocre.AlphaDrawImage(pDC,nXPos-(lNumberCount*SizeScoreNumber.cx/2),nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
				10*SizeScoreNumber.cx,0,RGB(255,0,255));
		nIndex++;
	}

	//�滭����
	for (;nIndex<lNumberCount;nIndex++)
	{
		//�滭����
		LONG lCellNumber=(LONG)(lNumber%10);
		m_ImageEndSocre.AlphaDrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
				lCellNumber*SizeScoreNumber.cx,0,RGB(255,0,255));

		//���ñ���
		lNumber/=10;
		nXDrawPos-=SizeScoreNumber.cx;
	};

	return;
}

//���밴ť
void CGameClientView::OnJettonButton500()
{
	//���ñ���
	m_lCurrentJetton=500L;

	return;
}

//���밴ť
void CGameClientView::OnJettonButton50000()
{
	//���ñ���
	m_lCurrentJetton=50000L;

	return;
}

//���밴ť
void CGameClientView::OnJettonButton500000()
{
	//���ñ���
	m_lCurrentJetton=500000L;

	return;
}

//���밴ť
void CGameClientView::OnJettonButton100()
{
	//���ñ���
	m_lCurrentJetton=100L;

	return;
}

//���밴ť
void CGameClientView::OnJettonButton1000()
{
	//���ñ���
	m_lCurrentJetton=1000L;

	return;
}

//���밴ť
void CGameClientView::OnJettonButton10000()
{
	//���ñ���
	m_lCurrentJetton=10000L;

	return;
}

//���밴ť
void CGameClientView::OnJettonButton100000()
{
	//���ñ���
	m_lCurrentJetton=100000L;

	return;
}

//��ʱ����Ϣ
void CGameClientView::OnTimer(UINT nIDEvent)
{
	//����ʤ��
	if (nIDEvent==IDI_FLASH_WINNER)
	{
		//���ñ���
		if (m_cbWinnerFlash!=m_cbWinnerSide)
		{
			m_cbWinnerFlash=m_cbWinnerSide;
		}
		else 
		{
			m_cbWinnerFlash=0xFF;
		}

		//���½���
		UpdateGameView(NULL);

	}

	//�ֻ�ׯ��
	if ( nIDEvent == IDI_SHOW_CHANGE_BANKER )
	{
		ShowChangeBanker( false );
	}
	
	//������
	if ( nIDEvent == IDI_OPEN_DICEBOX )
	{
		++m_cbOpenDiceboxIndex;

		//��ֹ�ж�
		if ( 15 <= m_cbOpenDiceboxIndex )
		{
			m_cbOpenDiceboxIndex=255;
			KillTimer(IDI_OPEN_DICEBOX);
			SetTimer(IDI_SHOW_GAME_SCOE, 4*1000, NULL);
			m_bShowGameResult=true;
			SetWinnerSide(((m_cbDice[0] + m_cbDice[1]) % 2 == 0) ? ID_EVEN : ID_ODD);
		}

		//���½���
		UpdateGameView(NULL);
	}

	//��ʾ�ɼ�
	if ( nIDEvent == IDI_SHOW_GAME_SCOE )
	{
		//���ö���
		SetCartoon(INDEX_CLOSE_DICEBOX, true);
		KillTimer(IDI_SHOW_GAME_SCOE);
		m_bShowGameResult=false;
		SetWinnerSide(0xFF);
	}

	//�ر�����
	if ( nIDEvent == IDI_CLOSE_DICEBOX )
	{
		++m_cbCloseDiceboxIndex;

		//��ֹ�ж�
		if ( 15 <= m_cbCloseDiceboxIndex )
		{
			m_cbCloseDiceboxIndex=255;
			KillTimer(IDI_CLOSE_DICEBOX);

			//����ɫ��
			ZeroMemory(m_cbDice, sizeof(m_cbDice));

			//���ñ�ʶ
			ShowCloseDicebox(true);
		}

		//���½���
		UpdateGameView(NULL);
	}

	if ( nIDEvent == IDI_SHAKE_DICEBOX )
	{
		++m_cbShakeDiceboxIndex;

		//��ֹ�ж�
		if (  15 * 3 + 4 <= m_cbShakeDiceboxIndex )
		{
			m_cbShakeDiceboxIndex=255;
			KillTimer(IDI_SHAKE_DICEBOX);

			//���ñ�ʶ
			ShowCloseDicebox(true);

			AfxGetMainWnd()->SendMessage(IDM_UPDATE_CONTROL,0,0);
		}

		//ֹͣ����
		if ( m_cbShakeDiceboxIndex == 15 * 3 )
			::PlaySound(NULL, AfxGetInstanceHandle(), SND_ASYNC);

		//���½���
		UpdateGameView(NULL);
	}

	__super::OnTimer(nIDEvent);
}

//�����Ϣ
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	if (m_lCurrentJetton!=0L)
	{
		BYTE cbJettonArea=GetJettonArea(Point);

		//��С�ж�
		switch( cbJettonArea )
		{
		case ID_ODD:
			{
				LONG lMaxOddScore = GetMaxOddScore();
				if ( lMaxOddScore < m_lCurrentJetton )
					return ;
				break;
			}
		case ID_EVEN:
			{
				LONG lMaxEvenScore = GetMaxEvenScore();
				if ( lMaxEvenScore < m_lCurrentJetton )
					return ;
				break;
			}
		}

		if (cbJettonArea!=0xFF) AfxGetMainWnd()->SendMessage(IDM_PLACE_JETTON,cbJettonArea,m_lCurrentJetton);
	}

	__super::OnLButtonDown(nFlags,Point);
}

//�����Ϣ
void CGameClientView::OnRButtonDown(UINT nFlags, CPoint Point)
{
	//���ñ���
	m_lCurrentJetton=0L;

	__super::OnLButtonDown(nFlags,Point);
}

//�����Ϣ
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
/////////////////////////////////////////////////////////////////
	{
		//��ȡ����
		CPoint MousePoint;
		GetCursorPos(&MousePoint);
		ScreenToClient(&MousePoint);
		BYTE cbJettonArea=GetJettonArea(MousePoint);

		//���ñ���
		if ( m_cbPreJettonArea != cbJettonArea )
		{
			m_cbPreJettonArea = cbJettonArea;
			UpdateGameView(NULL);
		}
	}
	/////////////////////////////////////////////////////////////////////
	if (m_lCurrentJetton!=0L)
	{
		//��ȡ����
		CPoint MousePoint;
		GetCursorPos(&MousePoint);
		ScreenToClient(&MousePoint);
		BYTE cbJettonArea=GetJettonArea(MousePoint);

		//���ñ���
		if ( m_cbPreJettonArea != cbJettonArea )
		{
			m_cbPreJettonArea = cbJettonArea;
			UpdateGameView(NULL);
		}

		//�����ж�
		if (cbJettonArea==0xFF)
		{
			SetCursor(LoadCursor(NULL,IDC_NO));
			return TRUE;
		}

		//��С�ж�
		switch( cbJettonArea )
		{
		case ID_ODD:
			{
				LONG lMaxOddScore = GetMaxOddScore();
				if ( lMaxOddScore < m_lCurrentJetton )
				{
					SetCursor(LoadCursor(NULL,IDC_NO));
					return TRUE;
				}
				break;
			}
		case ID_EVEN:
			{
				LONG lMaxEvenScore = GetMaxEvenScore();
				if ( lMaxEvenScore < m_lCurrentJetton )
				{
					SetCursor(LoadCursor(NULL,IDC_NO));
					return TRUE;
				}
				break;
			}
		}

		//���ù��
		switch (m_lCurrentJetton)
		{
		case 500:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_500)));
				return TRUE;
			}
		case 50000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_50000)));
				return TRUE;
			}
		case 500000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_500000)));
				return TRUE;
			}
		case 100:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_100)));
				return TRUE;
			}
		case 1000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_1000)));
				return TRUE;
			}
		case 10000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_10000)));
				return TRUE;
			}
		case 100000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_100000)));
				return TRUE;
			}
		}
	}

	return __super::OnSetCursor(pWnd, nHitTest, uMessage);
}

//�ֻ�ׯ��
void CGameClientView::ShowChangeBanker( bool bChangeBanker )
{
	//�ֻ�ׯ��
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

	//���½���
	UpdateGameView(NULL);
}

//��ׯ��ť
void CGameClientView::OnApplyBanker()
{
	AfxGetMainWnd()->SendMessage(IDM_APPLY_BANKER,1,0);

}

//��ׯ��ť
void CGameClientView::OnCancelBanker()
{
	AfxGetMainWnd()->SendMessage(IDM_APPLY_BANKER,0,0);
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

//ׯ����Ϣ
void CGameClientView::SetBankerInfo( WORD wChairID, BYTE cbBankerTime, LONG lScore )
{
	//���ñ���
	m_wCurrentBankerChairID = wChairID;
	m_cbBankerTime = cbBankerTime;
	m_lBankerScore = lScore;

	//���½���
	UpdateGameView( NULL );
}

//ׯ�ҽ��
void CGameClientView::SetBankerTreasure(LONG lBankerTreasure)
{
	m_lBankerTreasure = lBankerTreasure;
	UpdateGameView(NULL);
}

//�滭��ʶ
void CGameClientView::DrawWinFlags(CDC * pDC)
{
	CImageHandle ImageHandleWinFlags(&m_ImageWinFlags);
	int nIndex = m_nScoreHead;
	COLORREF clrOld ;
	clrOld = pDC->SetTextColor(RGB(255,234,0));
	CString strPoint;
	
	int nDrawCount = 0;
	CFont font;
	VERIFY(font.CreateFont(
		25,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Arial"));                 // lpszFacename

	pDC->SetTextColor(RGB(255,255,255));
	CFont* def_font = pDC->SelectObject(&font);

	while ( nIndex != m_nRecordLast && ( m_nRecordLast!=m_nRecordFirst ) && nDrawCount < RECORD_COUNT )
	{
		tagClientGameRecord &ClientGameRecord = m_GameRecordArrary[nIndex];
		int nYPos = 0;
		if ( ClientGameRecord.cbDiceCount % 2 != 0 ) nYPos = m_nWinFlagsExcursionY ;
		else nYPos = m_nWinFlagsExcursionY + 33;

		int nXPos = m_nWinFlagsExcursionX + ((nIndex - m_nScoreHead + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY) * 28;

		int nFlagsIndex = 0;
		if ( ClientGameRecord.enOperateFlags == enOperateResult_NULL ) nFlagsIndex = 0;
		else if ( ClientGameRecord.enOperateFlags == enOperateResult_Win) nFlagsIndex = 1;
		else if ( ClientGameRecord.enOperateFlags == enOperateResult_Lost) nFlagsIndex = 2;

		m_ImageWinFlags.AlphaDrawImage( pDC, nXPos, nYPos, m_ImageWinFlags.GetWidth()/3 , 
			m_ImageWinFlags.GetHeight(),m_ImageWinFlags.GetWidth()/3 * nFlagsIndex, 0, RGB(255, 0, 255) );

		CRect rcDicePoint(nXPos, m_nWinFlagsExcursionY+66, nXPos+26, m_nWinFlagsExcursionY+66+33);
		CString strDicePoint;
		strDicePoint.Format(TEXT("%d"), ClientGameRecord.cbDiceCount);
		pDC->DrawText(strDicePoint, rcDicePoint, DT_VCENTER|DT_SINGLELINE|DT_CENTER|DT_END_ELLIPSIS);

		nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
		nDrawCount++;
	}

	//ͳ�Ƹ���
	nIndex = m_nRecordFirst;
	int nOddCount = 0, nEvenCount = 0;
	while ( nIndex != m_nRecordLast && ( m_nRecordLast!=m_nRecordFirst ))
	{
		tagClientGameRecord &ClientGameRecord = m_GameRecordArrary[nIndex];
		bool bEven = ClientGameRecord.cbDiceCount % 2 == 0 ? true : false;
		if ( bEven ) nEvenCount++;
		else nOddCount++;

		nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
	}

	int nWidth, nHeight;
	CRect rcClient;
	GetClientRect(&rcClient);
	nWidth = rcClient.Width();
	nHeight = rcClient.Height();

	strPoint.Format(TEXT("%d"), nOddCount);
	pDC->TextOut(nWidth/2-350+5*120+78-194, nHeight/2+257, strPoint);
	strPoint.Format(TEXT("%d"), nEvenCount);
	pDC->TextOut(nWidth/2-350+5*120+78-194, nHeight/2+288, strPoint);
	strPoint.Format(TEXT("%d"), nEvenCount+nOddCount);
	pDC->SetTextColor(RGB(254,186,0));
	pDC->TextOut(nWidth/2-350+5*120+78-194, nHeight/2+320, strPoint);
	pDC->SelectObject(def_font);
	font.DeleteObject();
}

//�ƶ���ť
void CGameClientView::OnScoreMoveL()
{
	if ( m_nRecordFirst == m_nScoreHead ) return;

	m_nScoreHead = (m_nScoreHead - 1 + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
	if ( m_nScoreHead == m_nRecordFirst ) m_btScoreMoveL.EnableWindow(FALSE);

	m_btScoreMoveR.EnableWindow(TRUE);

	//���½���
	UpdateGameView(NULL);
}

//�ƶ���ť
void CGameClientView::OnScoreMoveR()
{
	int nHistoryCount = ( m_nRecordLast - m_nScoreHead + MAX_SCORE_HISTORY ) % MAX_SCORE_HISTORY;
	if ( nHistoryCount == RECORD_COUNT ) return;

	m_nScoreHead = ( m_nScoreHead + 1 ) % MAX_SCORE_HISTORY;
	if ( nHistoryCount-1 == RECORD_COUNT ) m_btScoreMoveR.EnableWindow(FALSE);

	m_btScoreMoveL.EnableWindow(TRUE);

	//���½���
	UpdateGameView(NULL);
}

//��ʾ���
void CGameClientView::ShowGameResult(CDC *pDC, int nWidth, int nHeight)
{
	//��ʾ�ж�
	if ( m_bShowGameResult == false ) return;

	int	nXPos = nWidth / 2 - 161 +1;
	int	nYPos = nHeight / 2 - 353 +71;

	CImageHandle ImageHandleGameEnd(&m_ImageGameEnd);
	CRect rcAlpha(nXPos+2, nYPos+70, nXPos+2 + m_ImageGameEnd.GetWidth(), nYPos+70+m_ImageGameEnd.GetHeight());

	m_ImageGameEnd.AlphaDrawImage(pDC, nXPos, nYPos, RGB(255,0,255));
	DrawAlphaRect(pDC, &rcAlpha, RGB(74,70,73), 0.5f);

	DrawEndNumberString(pDC, m_lMeCurGameReturnScore, nXPos+202+62, nYPos + 21 + 16);
	DrawEndNumberString(pDC, m_lMeCurGameScore, nXPos + 75+62, nYPos+22 + 16);
	DrawEndNumberString(pDC, m_lBankerCurGameScore, nXPos + 75+62, nYPos + 54 + 16);

	if ( m_lMeCurGameScore != 0 )
	{
		CImageHandle ImageHandleWinFlag(&m_ImageWinFlag);
		int nWinFlagWidth = m_ImageWinFlag.GetWidth()/2;
		int nWinFlagXPos = nXPos + (m_ImageGameEnd.GetWidth()- nWinFlagWidth )/2;
		m_ImageWinFlag.AlphaDrawImage(pDC, nWinFlagXPos, nYPos + 87, nWinFlagWidth, m_ImageWinFlag.GetHeight(), m_lMeCurGameScore>0?0:nWinFlagWidth, 0, RGB(255,0,255));
	}
}

//͸���滭
bool CGameClientView::DrawAlphaRect(CDC* pDC, LPRECT lpRect, COLORREF clr, FLOAT fAlpha)
{
	ASSERT(pDC != 0 && lpRect != 0 && clr != CLR_NONE);
	if(pDC == 0 || lpRect == 0 || clr == CLR_NONE)
	{
		return false;
	}
	//ȫ͸��
	if( abs(fAlpha) <= 0.000001 )
	{
		return true;
	}

	for(LONG l=lpRect->top; l<lpRect->bottom; l++)
	{
		for(LONG k=lpRect->left; k<lpRect->right; k++)
		{
			COLORREF clrBk = pDC->GetPixel(k, l);
			if ( clrBk != RGB(74,70,73) ) continue;

			COLORREF clrBlend = RGB(GetRValue(clrBk)*(1-fAlpha)+GetRValue(clr)*fAlpha, 
								GetGValue(clrBk)*(1-fAlpha)+GetGValue(clr)*fAlpha,
								GetBValue(clrBk)*(1-fAlpha)+GetBValue(clr)*fAlpha);
			pDC->SetPixel(k, l, clrBlend);
		}
	}

	return true;
}

//�����ע
LONG CGameClientView::GetMaxOddScore()
{
	if ( m_wCurrentBankerChairID == INVALID_CHAIR ) return 0;

	//��������
	LONG lOtherAreaScore = m_lAllEvenScore;

	//�����ע
	LONG lMaxOddScore = lOtherAreaScore + m_lBankerTreasure;
	lMaxOddScore -= (m_lAllOddScore);
	lMaxOddScore = min(lMaxOddScore, (m_lAreaLimitScore-m_lAllOddScore));

	return lMaxOddScore;
}

//�����ע
LONG CGameClientView::GetMaxEvenScore()
{
	if ( m_wCurrentBankerChairID == INVALID_CHAIR ) return 0;

	//��������
	LONG lOtherAreaScore = m_lAllOddScore;

	//�����ע
	LONG lMaxEvenScore = lOtherAreaScore + m_lBankerTreasure;
	lMaxEvenScore -= (m_lAllEvenScore);
	lMaxEvenScore = min(lMaxEvenScore, (m_lAreaLimitScore-m_lAllEvenScore));

	return lMaxEvenScore;
}

//�ɼ�����
void CGameClientView::SetGameScore(LONG lMeCurGameScore, LONG lMeCurGameReturnScore, LONG lBankerCurGameScore)
{
	m_lMeCurGameScore=lMeCurGameScore;
	m_lMeCurGameReturnScore=lMeCurGameReturnScore;
	m_lBankerCurGameScore=lBankerCurGameScore;
}

//���ñ�ʶ
void CGameClientView::SetShowGameFlag(bool bShowGameResult)
{
	m_bShowGameResult = bShowGameResult;
	UpdateGameView(NULL);
}

//�滭����
void CGameClientView::DrawMeJettonNumber(CDC *pDC)
{
	//�滭����
	if ( 0 < m_lMeOddScore ) DrawNumberString(pDC,m_lMeOddScore,m_PointJetton[0].x+90,m_PointJetton[0].y+185, true);
	if ( 0 < m_lMeEvenScore) DrawNumberString(pDC,m_lMeEvenScore,m_PointJetton[1].x+90,m_PointJetton[1].y+185, true);
}


//�����ж�
bool CGameClientView::HitEvenAreaTest(CPoint &Point)
{
	//��ȡ��С
	CRect rcClient;
	GetClientRect(&rcClient);

	//��ʼλ��
	int nXPos = rcClient.Width() / 2 + 99;
	int nYPos = rcClient.Height()/ 2 - 264;

	//��������
	CImageHandle ImageHandleEvenArea(&m_ImageFrameEven);
	int nWidth = m_ImageFrameEven.GetWidth();
	int nHeight = m_ImageFrameEven.GetHeight();
	CRect rcEvenArea(nXPos, nYPos, nXPos+nWidth, nYPos+nHeight);

	//��ʼ�ж�
	if ( ! rcEvenArea.PtInRect(Point) ) return false;

	//���Ͻ�
	CRect rcLeftTop(nXPos, nYPos, nXPos+59, nYPos+76);
	if ( rcLeftTop.PtInRect(Point) )
	{
		//����б��
		float fSlope = 76.f / 59.f;
		if ( fSlope < ( nYPos + 76.f - Point.y ) / ( Point.x - nXPos ) ) return false;
	}

	//���Ͻ�
	CRect rcRightTop(nXPos+214, nYPos, nXPos+nWidth,nYPos+176);
	if ( rcRightTop.PtInRect(Point) )
	{
		//����б��
		float fSlope = 176.f / (nWidth-214.f);
		if ( fSlope < ( nYPos+176.f - Point.y ) / ( nXPos + nWidth - Point.x ) ) return false;
	}

	//���½�
	CRect rcRightBottom(nXPos+214, nYPos+235, nXPos+nWidth, nYPos+nHeight);
	if ( rcRightBottom.PtInRect(Point) )
	{
		//����б��
		float fSlope = (nHeight - 235.f) / (nWidth-214.f);
		if ( fSlope > ( nYPos+nHeight - Point.y ) / ( Point.x -  nXPos-214.f ) ) return false;
	}

	return true;
}

//�����ж�
bool CGameClientView::HitOddAreaTest(CPoint &Point)
{
	//��ȡ��С
	CRect rcClient;
	GetClientRect(&rcClient);

	//��ʼλ��
	int nXPos = rcClient.Width() / 2 - 380;
	int nYPos = rcClient.Height() / 2 - 264;

	//��������
	CImageHandle ImageHandleOddArea(&m_ImageFrameOdd);
	int nWidth = m_ImageFrameOdd.GetWidth();
	int nHeight = m_ImageFrameOdd.GetHeight();
	CRect rcOddArea(nXPos, nYPos, nXPos+nWidth, nYPos+nHeight);

	//��ʼ�ж�
	if ( ! rcOddArea.PtInRect(Point) ) return false;

	//���Ͻ�
	CRect rcLeftTop(nXPos, nYPos, nXPos+60, nYPos+157);
	if ( rcLeftTop.PtInRect(Point) )
	{
		//����б��
		float fSlope = 157.f / 60.f;
		if ( fSlope < ( nYPos+157.f - Point.y ) / ( Point.x - nXPos ) ) return false;
	}

	//���½�
	CRect rcLefttBottom(nXPos, nYPos+247, nXPos+60, nYPos+nHeight);
	if ( rcLefttBottom.PtInRect(Point) )
	{
		//����б��
		float fSlope = (nHeight - 247.f) / (60.f);
		if ( fSlope > ( nYPos+nHeight - Point.y ) / ( nXPos+60.f - Point.x ) ) return false;
	}

	//���Ͻ�
	CRect rcRightTop(nXPos+214, nYPos, nXPos+nWidth,nYPos+176);
	if ( rcRightTop.PtInRect(Point) )
	{
		//����б��
		float fSlope = 176.f / (nWidth-214.f);
		if ( fSlope < ( nYPos+176.f - Point.y ) / ( nXPos+nWidth - Point.x ) ) return false;
	}

	return true;
}

//���ö���
void CGameClientView::SetCartoon(BYTE cbCartoonIdex, bool bStart)
{
	//��������
	m_cbOpenDiceboxIndex=255;
	m_cbCloseDiceboxIndex=255;
	m_cbShakeDiceboxIndex=255;

	//�رն�ʱ��
	KillTimer(IDI_OPEN_DICEBOX);
	KillTimer(IDI_CLOSE_DICEBOX);
	KillTimer(IDI_SHAKE_DICEBOX);
	KillTimer(IDI_SHOW_GAME_SCOE);

	//ֹͣ����
	::PlaySound(NULL, AfxGetInstanceHandle(), SND_ASYNC);

	if ( bStart == true )
	{
		UINT nElapse = 70;
		if ( cbCartoonIdex == INDEX_OPEN_DICEBOX )
		{
			m_cbOpenDiceboxIndex = 0;
			SetTimer(IDI_OPEN_DICEBOX, nElapse, NULL);
		}
		else if ( cbCartoonIdex == INDEX_CLOSE_DICEBOX )
		{
			m_cbCloseDiceboxIndex = 0;
			SetTimer(IDI_CLOSE_DICEBOX, nElapse, NULL);
		}
		else if ( cbCartoonIdex == INDEX_SHAKE_DICEBOX ) 
		{
			//���ñ�ʶ
			ShowCloseDicebox(false);

			m_cbShakeDiceboxIndex = 0;
			SetTimer(IDI_SHAKE_DICEBOX, 40, NULL);

			//������Դ
			HINSTANCE hInstance = AfxGetInstanceHandle();
			HRSRC hResour=FindResource(hInstance,TEXT("SHAKE_DICE"),TEXT("WAVE"));
			if (hResour==NULL) return ;
			HGLOBAL hGlobal=LoadResource(hInstance,hResour);
			if (hGlobal==NULL) return ;

			//��������
			CGameClientDlg *pGameClientDlg = (CGameClientDlg *)GetParent();
			if ( pGameClientDlg->IsEnableSound() )
			{
				LPCTSTR pszMemory=(LPCTSTR)LockResource(hGlobal);
				::PlaySound(pszMemory,hInstance,SND_ASYNC|SND_MEMORY|SND_LOOP);
			}

			//������Դ
			UnlockResource(hGlobal);
			FreeResource(hGlobal);
		}
	}

	//���½���
	UpdateGameView(NULL);
}

//�滭����
void CGameClientView::DrawCartoon(CDC *pDC, int nWidth, int nHeight)
{
	//�����ж�
	if ( m_cbOpenDiceboxIndex < 15 )
	{
		//��ԴID
		UINT uResourceID[] = {IDB_OPEN_DICEBOX_1, IDB_OPEN_DICEBOX_2, IDB_OPEN_DICEBOX_3, IDB_OPEN_DICEBOX_4, IDB_OPEN_DICEBOX_5
			, IDB_OPEN_DICEBOX_6, IDB_OPEN_DICEBOX_7, IDB_OPEN_DICEBOX_8, IDB_OPEN_DICEBOX_9, IDB_OPEN_DICEBOX_10, IDB_OPEN_DICEBOX_11
			, IDB_OPEN_DICEBOX_12, IDB_OPEN_DICEBOX_13, IDB_OPEN_DICEBOX_14, IDB_OPEN_DICEBOX_15};

		//�ڴ���Դ
		CBitmap Bitmap;
		Bitmap.LoadBitmap(uResourceID[m_cbOpenDiceboxIndex]);
		BITMAP Bmp;
		Bitmap.GetBitmap(&Bmp);
		CDC dcMemory;
		dcMemory.CreateCompatibleDC(pDC);
		CBitmap *pOldBitmap = dcMemory.SelectObject(&Bitmap);

		pDC->TransparentBlt(nWidth/2-200, nHeight/2-355, Bmp.bmWidth, Bmp.bmHeight, &dcMemory, 0, 0, Bmp.bmWidth, Bmp.bmHeight,RGB(255,0,255));

		//�ͷ���Դ
		dcMemory.SelectObject(pOldBitmap);
		dcMemory.DeleteDC();
		Bitmap.DeleteObject();
	}
	if ( m_cbCloseDiceboxIndex < 15 )
	{
		//��ԴID
		UINT uResourceID[] = {IDB_CLOSE_DICEBOX_1, IDB_CLOSE_DICEBOX_2, IDB_CLOSE_DICEBOX_3, IDB_CLOSE_DICEBOX_4, IDB_CLOSE_DICEBOX_5
			, IDB_CLOSE_DICEBOX_6, IDB_CLOSE_DICEBOX_7, IDB_CLOSE_DICEBOX_8, IDB_CLOSE_DICEBOX_9, IDB_CLOSE_DICEBOX_10, IDB_CLOSE_DICEBOX_11
			, IDB_CLOSE_DICEBOX_12, IDB_CLOSE_DICEBOX_13, IDB_CLOSE_DICEBOX_14, IDB_CLOSE_DICEBOX_15};

		//�ڴ���Դ
		CBitmap Bitmap;
		Bitmap.LoadBitmap(uResourceID[m_cbCloseDiceboxIndex]);
		BITMAP Bmp;
		Bitmap.GetBitmap(&Bmp);
		CDC dcMemory;
		dcMemory.CreateCompatibleDC(pDC);
		CBitmap *pOldBitmap = dcMemory.SelectObject(&Bitmap);

		pDC->TransparentBlt(nWidth/2-200, nHeight/2-355, Bmp.bmWidth, Bmp.bmHeight, &dcMemory, 0, 0, Bmp.bmWidth, Bmp.bmHeight,RGB(255,0,255));

		//�ͷ���Դ
		dcMemory.SelectObject(pOldBitmap);
		dcMemory.DeleteDC();
		Bitmap.DeleteObject();
	}
	if ( m_cbShakeDiceboxIndex < ( 15 * 3 + 5 ) )
	{
		int nIndex = m_cbShakeDiceboxIndex % (15);

		if ( m_cbShakeDiceboxIndex > 15 * 3 )
		{
			nIndex = 15 + ( m_cbShakeDiceboxIndex - 15 * 3 ) % 5;
		}

		//��ԴID
		UINT uResourceID[] = {IDB_SHAKE_DICEBOX_1, IDB_SHAKE_DICEBOX_2, IDB_SHAKE_DICEBOX_3, IDB_SHAKE_DICEBOX_4, IDB_SHAKE_DICEBOX_5
			, IDB_SHAKE_DICEBOX_6, IDB_SHAKE_DICEBOX_7, IDB_SHAKE_DICEBOX_8, IDB_SHAKE_DICEBOX_9, IDB_SHAKE_DICEBOX_10, IDB_SHAKE_DICEBOX_11
			, IDB_SHAKE_DICEBOX_12, IDB_SHAKE_DICEBOX_13, IDB_SHAKE_DICEBOX_14, IDB_SHAKE_DICEBOX_15, IDB_SHAKE_DICEBOX_16, IDB_SHAKE_DICEBOX_17
		, IDB_SHAKE_DICEBOX_18, IDB_SHAKE_DICEBOX_19, IDB_SHAKE_DICEBOX_20, IDB_SHAKE_DICEBOX_21};

		//�ڴ���Դ
		CBitmap Bitmap;
		Bitmap.LoadBitmap(uResourceID[nIndex]);
		BITMAP Bmp;
		Bitmap.GetBitmap(&Bmp);
		CDC dcMemory;
		dcMemory.CreateCompatibleDC(pDC);
		CBitmap *pOldBitmap = dcMemory.SelectObject(&Bitmap);

		pDC->TransparentBlt(nWidth/2-200, nHeight/2-355, Bmp.bmWidth, Bmp.bmHeight, &dcMemory, 0, 0, Bmp.bmWidth, Bmp.bmHeight,RGB(255,0,255));

		//�ͷ���Դ
		dcMemory.SelectObject(pOldBitmap);
		dcMemory.DeleteDC();
		Bitmap.DeleteObject();
	}
}

//����ɫ��
void CGameClientView::SetDice(BYTE cbDice[2])
{
	m_cbDice[0] = cbDice[0];
	m_cbDice[1] = cbDice[1];
}

//״̬�ж�
bool CGameClientView::IsPlaceJettonStatus()
{
	if ( m_cbOpenDiceboxIndex != 255 ) return false;
	if ( m_cbCloseDiceboxIndex != 255 ) return false;
	if ( m_cbShakeDiceboxIndex != 255 ) return false;

	return true;
}
//////////////////////////////////////////////////////////////////////////

