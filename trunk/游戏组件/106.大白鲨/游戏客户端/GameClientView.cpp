#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"
#include "GameClientDlg.h"
#include "GameClientDlg.h"
#include "math.h"

CGameClientDlg * g_pGameClient;

//////////////////////////////////////////////////////////////////////////

//��ť��ʶ 
#define IDC_APPY_BANKER				201									//������ׯ
#define IDC_CANCEL_BANKER			202									//ȡ����ׯ
#define	IDC_GAMERECORD_L			203									//�ɼ���ť
#define	IDC_GAMERECORD_R			204									//�ɼ��Ұ�ť
#define IDC_BANKERLIST_B			205									//������ׯ �°�ť
#define IDC_BANKERLIST_T			206									//������ׯ �ϰ�ť
#define IDC_JETTON_BUTTON_100		207									//��ť��ʶ
#define IDC_JETTON_BUTTON_1000		208									//��ť��ʶ
#define IDC_JETTON_BUTTON_10000		209									//��ť��ʶ
#define IDC_JETTON_BUTTON_100000	210									//��ť��ʶ
#define IDC_JETTON_BUTTON_1000000	211									//��ť��ʶ
#define IDC_JETTON_BUTTON_10000000	212									//��ť��ʶ
#define IDC_BANK_SAVE				213									//��ť
#define IDC_BANK_GET				214									//ȡ�ť
#define IDC_JETTON_BUTTON			215									//��ť��ʶ
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_JETTON_BUTTON_100, OnJettonButton100)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_1000, OnJettonButton1000)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_10000, OnJettonButton10000)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_100000, OnJettonButton100000)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_1000000, OnJettonButton1000000)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_10000000, OnJettonButton10000000)
	ON_BN_CLICKED(IDC_JETTON_BUTTON, OnJettonButton)
	ON_BN_CLICKED(IDC_GAMERECORD_L, OnScoreLeft)
	ON_BN_CLICKED(IDC_GAMERECORD_R, OnScoreRight)
	ON_BN_CLICKED(IDC_BANKERLIST_B, OnBankerBottom)
	ON_BN_CLICKED(IDC_BANKERLIST_T, OnBankerTop)
	ON_BN_CLICKED(IDC_APPY_BANKER, OnApplyBanker)
	ON_BN_CLICKED(IDC_CANCEL_BANKER, OnCancelBanker)
	ON_BN_CLICKED(IDC_BANK_GET, OnGetBank)
	ON_BN_CLICKED(IDC_BANK_SAVE, OnSaveBank)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////



//���캯��
CGameClientView::CGameClientView() : CGameFrameView(true,16)
{
	//������Ϣ
	m_lMeMaxScore=0L;			
	m_lAreaLimitScore=0L;		
	m_wMeChairID = INVALID_CHAIR;
	m_lCurrentJetton = 0L;
	m_cbJettonIndex = 0xff;
	m_cbSelectIndex = 0xff;
	m_cbMoveIndex = 0xff;
	m_bIsMove = false;
	m_bIsShowJetton = FALSE;
	m_cbGameState = GS_FREE;
	m_cbGameTimer = 0;
	m_lMeCanUseScore = 0L;
	m_cbSlectPos = 0xff;
	//ׯ����Ϣ
	m_wBankerUser=INVALID_CHAIR;		
	m_wBankerTime=0;
	m_lBankerScore=0L;	
	m_lTmpBankerWinScore=0L;
	//������ע��Ϣ

	m_lMeBigSharkScore=0L;
	m_lMeFeiQinScore=0L;
	m_lMeZouShouScore=0L;
	m_lMeYanZiScore=0L;				
	m_lMeTuZiScore=0L;				
	m_lMeGeZiScore=0L;					
	m_lMeXiongMaoScore=0L;					
	m_lMeKongQueScore=0L;					
	m_lMeHouZiScore=0L;				
	m_lMeLaoYingScore=0L;				
	m_lMeShiZiScore=0L;					

	//ȫ����ע
	m_lAllBigSharkScore=0L;
	m_lAllFeiQinScore=0L;
	m_lAllZouShouScore=0L;
	m_lALLYanZiScore=0L;				
	m_lALLTuZiScore=0L;				
	m_lALLGeZiScore=0L;					
	m_lALLXiongMaoScore=0L;					
	m_lALLKongQueScore=0L;					
	m_lALLHouZiScore=0L;					
	m_lALLLaoYingScore=0L;					
	m_lALLShiZiScore=0L;		

	m_lMeStatisticScore = 0L;
	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageViewBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_ImageScoreNumber.LoadImage(hInstance,TEXT("SCORE_NUMBER"));
	m_ImageMeScoreNumber.LoadImage(hInstance,TEXT("ME_SCORE_NUM"));
	m_ImageJettonView.LoadImage(hInstance,TEXT("CHART_PING"));
	m_ImageSelect.LoadImage(hInstance,TEXT("SEL_FRAME"));
	m_ImageGameState.LoadImage(hInstance,TEXT("GAME_SATATE"));
	m_ImageGameTimer.LoadImage(hInstance,TEXT("TIME_NUM"));
	m_ImageScoreHistory.LoadImage(hInstance,TEXT("SCORE_HISTORY"));
	m_ImageJettonBK.LoadImage(IDB_JETTON_BK);

	m_ImageYanZi.LoadImage(IDB_Image_YanZi);	
	m_ImageYanZi1.LoadImage(IDB_Image_YanZi1);
	m_ImageTuZi.LoadImage(IDB_Image_TuZi);	
	m_ImageTuZi1.LoadImage(IDB_Image_TuZi1);
	m_ImageGeZi.LoadImage(IDB_Image_GeZi);	
	m_ImageGeZi1.LoadImage(IDB_Image_GeZi1);
	m_ImageXiongMao.LoadImage(IDB_Image_XiongMao);
	m_ImageXiongMao1.LoadImage(IDB_Image_XiongMao1);
	m_ImageKongQue.LoadImage(IDB_Image_KongQue);	
	m_ImageKongQue1.LoadImage(IDB_Image_KongQue1);
	m_ImageHouZi.LoadImage(IDB_Image_HouZi);	
	m_ImageHouZi1.LoadImage(IDB_Image_HouZi1);
	m_ImageLaoYing.LoadImage(IDB_Image_LaoYing);	
	m_ImageLaoYing1.LoadImage(IDB_Image_LaoYing1);
	m_ImageShiZi.LoadImage(IDB_Image_ShiZi);	
	m_ImageShiZi1.LoadImage(IDB_Image_ShiZi1);
	m_ImageDaBaiSha.LoadImage(IDB_Image_DaBaiSha);
	m_ImageDaBaiSha1.LoadImage(IDB_Image_DaBaiSha1);
	m_ImageFeiQin.LoadImage(IDB_Image_FeiQin);
	m_ImageZouShou.LoadImage(IDB_Image_ZouShou);

	m_cbScoreHistroy.RemoveAll();
	m_cbScoreHistroyIndex = 0;
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

	//m_ApplyUser.Create( IDD_DLG_GAME_RECORD	, this );
	//�����ؼ�
	CRect rcCreate(0,0,0,0);
	//���밴ť
	//m_btBanker_T.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_BANKERLIST_T);
	//m_btBanker_B.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_BANKERLIST_B);
	m_btScore_L.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_GAMERECORD_L);
	m_btScore_R.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_GAMERECORD_R);
	
	//��ע��ť
	m_btJetton100.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_100);
	m_btJetton1000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_1000);
	m_btJetton10000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_10000);
	m_btJetton100000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_100000);
	m_btJetton1000000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_1000000);
	m_btJetton10000000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_10000000);
	m_btJetton.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON);
	
	//���ð�ť
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	//m_btBanker_T.SetButtonImage(IDB_BANKERLIST_T,hResInstance,false);
	//m_btBanker_B.SetButtonImage(IDB_BANKERLIST_B,hResInstance,false);
	m_btScore_L.SetButtonImage(IDB_GAMERECORD_L,hResInstance,false);
	m_btScore_R.SetButtonImage(IDB_GAMERECORD_R,hResInstance,false);
	
	m_btJetton100.SetButtonImage(IDB_BT_JETTON_100,5,false);
	m_btJetton1000.SetButtonImage(IDB_BT_JETTON_1000,5,false);
	m_btJetton10000.SetButtonImage(IDB_BT_JETTON_10000,5,false);
	m_btJetton100000.SetButtonImage(IDB_BT_JETTON_100000,5,false);
	m_btJetton1000000.SetButtonImage(IDB_BT_JETTON_1000000,5,false);
	m_btJetton10000000.SetButtonImage(IDB_BT_JETTON_10000000,5,false);
	m_btJetton.SetButtonImage(IDB_BT_JETTON,5,false);

	m_btJetton.EnableWindow(TRUE);

	m_btJetton100.ShowWindow(SW_HIDE);
	m_btJetton1000.ShowWindow(SW_HIDE);
	m_btJetton10000.ShowWindow(SW_HIDE);
	m_btJetton100000.ShowWindow(SW_HIDE);
	m_btJetton1000000.ShowWindow(SW_HIDE);
	m_btJetton10000000.ShowWindow(SW_HIDE);

	g_pGameClient = (CGameClientDlg*)AfxGetMainWnd();

	return 0;
}

//���ý���
void CGameClientView::ResetGameView()
{
	m_wMeChairID = INVALID_CHAIR;
	m_cbSelectIndex = 0xff;
	m_cbMoveIndex = 0xff;
	m_bIsMove = false;
	m_cbGameState = GS_FREE;
	m_cbGameTimer=0;
	m_lMeCanUseScore = 0L;
	m_cbSlectPos = 0xff;
	//���ð�ť
	return;
}

//�����ؼ�
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	int CenterXLine = nWidth/2;
	int CenterYLine = nHeight/2;
	//λ�ö�λ
	m_ptUserInfo.x = CenterXLine + 308;
	m_ptUserInfo.y = CenterYLine + 262;
	//����λ��
	m_rcBigShark.top = CenterYLine+185;
	m_rcBigShark.left =CenterXLine-63;
	m_rcBigShark.bottom = m_rcBigShark.top +SIZE_Y_SHARK;
	m_rcBigShark.right = m_rcBigShark.left +SIZE_X_SHARK;

	
	m_rcFeiQin.top = CenterYLine+115;
	m_rcFeiQin.left =CenterXLine-404;
	m_rcFeiQin.bottom = m_rcFeiQin.top +SIZE_Y_2;
	m_rcFeiQin.right = m_rcFeiQin.left +SIZE_X_2;

	m_rcZouShou.top = CenterYLine+120;
	m_rcZouShou.left =CenterXLine+206;
	m_rcZouShou.bottom = m_rcZouShou.top +SIZE_Y_2;
	m_rcZouShou.right = m_rcZouShou.left +SIZE_X_2;

	int nPosX=nWidth/2-400;
	int nPosY=nHeight/2+175;
	CSize SizeAnimal(m_ImageYanZi.GetWidth(),m_ImageYanZi.GetHeight());

	m_rcYanZi.top = nPosY;
	m_rcYanZi.left = nPosX;
	m_rcYanZi.bottom =m_rcYanZi.top + SIZE_Y;  
	m_rcYanZi.right = m_rcYanZi.left + SIZE_X;

	m_rcTuZi.top = nPosY;
	m_rcTuZi.left = nPosX+SizeAnimal.cx*3+286;
	m_rcTuZi.bottom =m_rcTuZi.top + SIZE_Y;  
	m_rcTuZi.right = m_rcTuZi.left + SIZE_X;

	m_rcGeZi.top = m_rcYanZi.top;
	m_rcGeZi.left = nPosX+SizeAnimal.cx;
	m_rcGeZi.bottom =m_rcGeZi.top + SIZE_Y;  
	m_rcGeZi.right = m_rcGeZi.left + SIZE_X;

	m_rcXiongMao.top = nPosY;
	m_rcXiongMao.left = nPosX+SizeAnimal.cx*4+286;
	m_rcXiongMao.bottom =m_rcXiongMao.top + SIZE_Y;  
	m_rcXiongMao.right = m_rcXiongMao.left + SIZE_X;

	m_rcKongQue.top = m_rcYanZi.top;
	m_rcKongQue.left = nPosX+SizeAnimal.cx*2;
	m_rcKongQue.bottom =m_rcKongQue.top + SIZE_Y;  
	m_rcKongQue.right = m_rcKongQue.left + SIZE_X;
	
	m_rcHouZi.top = nPosY;
	m_rcHouZi.left = nPosX+SizeAnimal.cx*5+286;
	m_rcHouZi.bottom =m_rcHouZi.top + SIZE_Y;  
	m_rcHouZi.right = m_rcHouZi.left + SIZE_X;

	m_rcLaoYing.top = m_rcYanZi.top;
	m_rcLaoYing.left = nPosX+SizeAnimal.cx*3;
	m_rcLaoYing.bottom =m_rcLaoYing.top + SIZE_Y;  
	m_rcLaoYing.right = m_rcLaoYing.left + SIZE_X;
	
	m_rcShiZi.top = nPosY;
	m_rcShiZi.left = nPosX+SizeAnimal.cx*6+286;
	m_rcShiZi.bottom =m_rcShiZi.top + SIZE_Y;  
	m_rcShiZi.right = m_rcShiZi.left + SIZE_X;
	
	//��λ��
	m_ptBaseTop.x = CenterXLine - 379;
	m_ptBaseTop.y = CenterYLine - 294;

	m_ptBaseBottom.x = CenterXLine + 262;
	m_ptBaseBottom.y = CenterYLine + 163;

	m_ptBaseLeft.x = CenterXLine - 379;
	m_ptBaseLeft.y = CenterYLine + 162;

	m_ptBaseRight.x = CenterXLine + 262;
	m_ptBaseRight.y = CenterYLine - 294;

	//��Ϸ״̬
	m_ptGameState.x = CenterXLine - m_ImageGameState.GetWidth()/3 + 6;
	m_ptGameState.y = CenterYLine - 203;
	
	//ʱ��
	m_ptGameTimer.x = CenterXLine + 30;
	m_ptGameTimer.y = CenterYLine -187; 

	//��ʷ�ɼ�
	m_ptScoreHistory.x = CenterXLine - 116;
	m_ptScoreHistory.y = CenterYLine + 308;

	//��������
	int ExcursionY=25;
	m_PointJettonNumber[ID_BIG_SHARK-1].SetPoint((m_rcBigShark.right+m_rcBigShark.left)/2-15, (m_rcBigShark.bottom+m_rcBigShark.top)/2-ExcursionY+40);
	m_PointJettonNumber[ID_FEI_QIN-1].SetPoint((m_rcFeiQin.right+m_rcFeiQin.left)/2-50, (m_rcFeiQin.bottom+m_rcFeiQin.top)/2-ExcursionY-15);
	m_PointJettonNumber[ID_ZOU_SHOU-1].SetPoint((m_rcZouShou.right+m_rcZouShou.left)/2+35, (m_rcZouShou.bottom+m_rcZouShou.top)/2-ExcursionY-12);
	m_PointJettonNumber[ID_YAN_ZI-1].SetPoint((m_rcYanZi.right+m_rcYanZi.left)/2-10, (m_rcYanZi.bottom+m_rcYanZi.top)/2-ExcursionY);
	m_PointJettonNumber[ID_TU_ZI-1].SetPoint((m_rcTuZi.right+m_rcTuZi.left)/2-10, (m_rcTuZi.bottom+m_rcTuZi.top)/2-ExcursionY);
	m_PointJettonNumber[ID_GE_ZI-1].SetPoint((m_rcGeZi.right+m_rcGeZi.left)/2-10, (m_rcGeZi.bottom+m_rcGeZi.top)/2-ExcursionY);
	m_PointJettonNumber[ID_XIONG_MAO-1].SetPoint((m_rcXiongMao.right+m_rcXiongMao.left)/2-10, (m_rcXiongMao.bottom+m_rcXiongMao.top)/2-ExcursionY);
	m_PointJettonNumber[ID_KONG_QUE-1].SetPoint((m_rcKongQue.right+m_rcKongQue.left)/2-10, (m_rcKongQue.bottom+m_rcKongQue.top)/2-ExcursionY);
	m_PointJettonNumber[ID_HOU_ZI-1].SetPoint((m_rcHouZi.right+m_rcHouZi.left)/2-10, (m_rcHouZi.bottom+m_rcHouZi.top)/2-ExcursionY);
	m_PointJettonNumber[ID_LAO_YING-1].SetPoint((m_rcLaoYing.right+m_rcLaoYing.left)/2-10, (m_rcLaoYing.bottom+m_rcLaoYing.top)/2-ExcursionY);
	m_PointJettonNumber[ID_SHI_ZI-1].SetPoint((m_rcShiZi.right+m_rcShiZi.left)/2-10, (m_rcShiZi.bottom+m_rcShiZi.top)/2-ExcursionY);

	//����λ��
	//m_PointJetton[ID_BIG_SHARK-1].SetPoint(m_rcBigShark.left, m_rcBigShark.top);
	//m_PointJetton[ID_FEI_QIN-1].SetPoint(m_rcFeiQin.left, m_rcFeiQin.top);
	//m_PointJetton[ID_ZOU_SHOU-1].SetPoint(m_rcZouShou.left, m_rcZouShou.top);
	//m_PointJetton[ID_YAN_ZI-1].SetPoint(m_rcYanZi.left, m_rcYanZi.top);
	//m_PointJetton[ID_TU_ZI-1].SetPoint(m_rcTuZi.left, m_rcTuZi.top);
	//m_PointJetton[ID_GE_ZI-1].SetPoint(m_rcGeZi.left, m_rcGeZi.top);
	//m_PointJetton[ID_XIONG_MAO-1].SetPoint(m_rcXiongMao.left, m_rcXiongMao.top);
	//m_PointJetton[ID_KONG_QUE-1].SetPoint(m_rcKongQue.left, m_rcKongQue.top);
	//m_PointJetton[ID_HOU_ZI-1].SetPoint(m_rcHouZi.left, m_rcHouZi.top);
	//m_PointJetton[ID_LAO_YING-1].SetPoint(m_rcLaoYing.left, m_rcLaoYing.top);
	//m_PointJetton[ID_SHI_ZI-1].SetPoint(m_rcShiZi.left, m_rcShiZi.top);
	//�ƶ��ؼ�
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;
	
	//������ť
	//DeferWindowPos(hDwp,m_btBanker_T,NULL,nWidth/2+280,nHeight/2-352+46,0,0,uFlags|SWP_NOSIZE);
	//DeferWindowPos(hDwp,m_btBanker_B,NULL,nWidth/2+280+43,nHeight/2-352+46,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btScore_L,NULL,nWidth/2-134,nHeight/2,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btScore_R,NULL,nWidth/2+158,nHeight/2,0,0,uFlags|SWP_NOSIZE);
	
	//���밴ť
	CRect rcJetton;
	m_btJetton100.GetWindowRect(&rcJetton);
	int nYPos = nHeight/2-30;
	int nXPos = nWidth/2-190+534;
	int nSpace = 0;
	DeferWindowPos(hDwp,m_btJetton100,NULL,nXPos,nYPos+rcJetton.Height(),0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton1000,NULL,nXPos,nYPos+rcJetton.Height()*2,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton10000,NULL,nXPos,nYPos+rcJetton.Height()*3,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton100000,NULL,nXPos,nYPos+rcJetton.Height()*4,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton1000000,NULL,nXPos,nYPos+rcJetton.Height()*5,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton10000000,NULL,nXPos,nYPos+rcJetton.Height()*6,0,0,uFlags|SWP_NOSIZE);

	DeferWindowPos(hDwp,m_btJetton,NULL,nXPos-1,nYPos-3,0,0,uFlags|SWP_NOSIZE);
	//�б�ؼ�
	//DeferWindowPos(hDwp,m_ApplyUser,NULL,nWidth/2 + 192,nHeight/2-349,180,43,uFlags);

	//�����ƶ�
	EndDeferWindowPos(hDwp);
	return;
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//�滭����
	DrawViewImage(pDC,m_ImageViewBack,enMode_Centent);
	m_ImageJettonBK.DrawImage(pDC,nWidth/2-190+535-6,nHeight/2-30-5);

	int nPosX=nWidth/2-400;
	int nPosY=nHeight/2+175;
	CSize SizeAnimal(m_ImageYanZi.GetWidth(),m_ImageYanZi.GetHeight());
	m_ImageYanZi1.DrawImage(pDC,nPosX,nPosY);
	m_ImageGeZi1.DrawImage(pDC,nPosX+SizeAnimal.cx,nPosY);
	m_ImageKongQue1.DrawImage(pDC,nPosX+SizeAnimal.cx*2,nPosY);
	m_ImageLaoYing1.DrawImage(pDC,nPosX+SizeAnimal.cx*3,nPosY);
	m_ImageTuZi1.DrawImage(pDC,nPosX+SizeAnimal.cx*3+286,nPosY);
	m_ImageXiongMao1.DrawImage(pDC,nPosX+SizeAnimal.cx*4+286,nPosY);
	m_ImageHouZi1.DrawImage(pDC,nPosX+SizeAnimal.cx*5+286,nPosY);
	m_ImageShiZi1.DrawImage(pDC,nPosX+SizeAnimal.cx*6+286,nPosY);
	m_ImageDaBaiSha1.DrawImage(pDC,nWidth/2-107,nPosY-44);



		//�滭�û�
	if (m_wMeChairID!=INVALID_CHAIR)
	{
		const tagUserData *pMeUserData = GetUserInfo(m_wMeChairID);
	//	TCHAR szBuffer[1024];
	//	_snprintf(szBuffer,sizeof(szBuffer),TEXT("%d"),pMeUserData->lScore);
	//	TCHAR szBuffer1[1024];
		LONG lMeJetton = pMeUserData->lScore-(m_lMeBigSharkScore+m_lMeFeiQinScore+m_lMeZouShouScore+m_lMeYanZiScore+m_lMeTuZiScore+m_lMeGeZiScore+m_lMeXiongMaoScore+m_lMeKongQueScore+
			m_lMeHouZiScore+m_lMeLaoYingScore+m_lMeShiZiScore);
	//	_snprintf(szBuffer1,sizeof(szBuffer1),TEXT("%d"),lMeJetton);
	//	TCHAR szBuffer2[1024];
	//	_snprintf(szBuffer2,sizeof(szBuffer2),TEXT("%d"),m_lMeStatisticScore);
	//	//�滭�û���Ϣ
		CFont font;
		font.CreatePointFont(180,"����");
		CFont* pOldFont = pDC->SelectObject(&font);
		pDC->SelectObject(&font);
		pDC->SetTextAlign(TA_CENTER|TA_TOP);
		pDC->SetTextColor(RGB(255,0,0));
	//	pDC->TextOut(m_ptUserInfo.x,m_ptUserInfo.y,pMeUserData->szName,lstrlen(pMeUserData->szName));
		DrawNumberStringWithSpace(pDC,lMeJetton,m_ptUserInfo.x-50,m_ptUserInfo.y+48);
	//	DrawNumberStringWithSpace(pDC,lMeJetton,m_ptUserInfo.x,m_ptUserInfo.y+34);
	//	DrawNumberStringWithSpace(pDC,m_lMeStatisticScore,m_ptUserInfo.x,m_ptUserInfo.y+51);
	//	pDC->SelectObject(&pOldFont);
	}
	
	////ׯ����Ϣ																										
	//pDC->SetTextColor(RGB(255,234,0));
	////��ȡ���
	//tagUserData const *pUserData = m_wBankerUser==INVALID_CHAIR ? NULL : GetUserInfo(m_wBankerUser);
	////λ����Ϣ
	//CRect StrRect;
	//StrRect.left = nWidth/2-308;  
	//StrRect.top = nHeight/2-346;  
	//StrRect.right = StrRect.left + 190;
	//StrRect.bottom = StrRect.top + 15;
	////ׯ������
	//pDC->DrawText(pUserData==NULL?(m_bEnableSysBanker?TEXT("ϵͳ��ׯ"):TEXT("������ׯ")):pUserData->szName, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP| DT_SINGLELINE );
	////ׯ���ܷ�
	//CString strBankerTotalScore;
	//strBankerTotalScore.Format( "%d", pUserData==NULL?0:pUserData->lScore );
	//StrRect.left = nWidth/2-308;
	//StrRect.top = nHeight/2-317;
	//StrRect.right = StrRect.left + 190;
	//StrRect.bottom = StrRect.top + 15;
	////pDC->DrawText(strBankerTotalScore, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP| DT_SINGLELINE );
	//DrawNumberStringWithSpace(pDC, pUserData==NULL?0:pUserData->lScore , StrRect.left, StrRect.top);

	////ׯ�Ҿ���
	//CString strBankerTime;
	//strBankerTime.Format( "%d", m_wBankerTime );
	//StrRect.left = nWidth/2-308;
	//StrRect.top = nHeight/2-331;
	//StrRect.right = StrRect.left + 190;
	//StrRect.bottom = StrRect.top + 15;
	//pDC->DrawText(strBankerTime, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP| DT_SINGLELINE );

	////ׯ�ҳɼ�
	//CString strBankerScore;
	//strBankerScore.Format( "%ld", pUserData==NULL?0:m_lTmpBankerWinScore);
	//StrRect.left = nWidth/2-308;
	//StrRect.top = nHeight/2-303;
	//StrRect.right = StrRect.left + 190;
	//StrRect.bottom = StrRect.top + 15; 
	////pDC->DrawText(strBankerScore,StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP| DT_SINGLELINE );
	//DrawNumberStringWithSpace(pDC, pUserData==NULL?0:m_lTmpBankerWinScore, StrRect.left, StrRect.top);

	//����ѡ��滭
	//CRect crRect[]={m_rcBigShark,m_rcFeiQin,m_rcZouShou,m_rcYanZi,m_rcTuZi,m_rcGeZi,m_rcXiongMao,m_rcKongQue,m_rcHouZi,m_rcLaoYing,m_rcShiZi};
	//CBrush brush(RGB(240, 240, 0));
	//CBrush *oldBrush = pDC->SelectObject(&brush);
	//for (int i=0;i<11;i++)
	//{
	//	pDC->FrameRect(crRect[i],oldBrush);
	//	pDC->SelectObject(&oldBrush);
	//}
	if (m_cbGameState==GS_PLACE_JETTON)
	{
		m_ImageYanZi.DrawImage(pDC,nPosX,nPosY);
		m_ImageGeZi.DrawImage(pDC,nPosX+SizeAnimal.cx,nPosY);
		m_ImageKongQue.DrawImage(pDC,nPosX+SizeAnimal.cx*2,nPosY);
		m_ImageLaoYing.DrawImage(pDC,nPosX+SizeAnimal.cx*3,nPosY);
		m_ImageTuZi.DrawImage(pDC,nPosX+SizeAnimal.cx*3+286,nPosY);
		m_ImageXiongMao.DrawImage(pDC,nPosX+SizeAnimal.cx*4+286,nPosY);
		m_ImageHouZi.DrawImage(pDC,nPosX+SizeAnimal.cx*5+286,nPosY);
		m_ImageShiZi.DrawImage(pDC,nPosX+SizeAnimal.cx*6+286,nPosY);
		m_ImageDaBaiSha.DrawImage(pDC,nWidth/2-107,nPosY-44);
		m_ImageFeiQin.DrawImage(pDC,nWidth/2-404,nHeight/2+115);
		m_ImageZouShou.DrawImage(pDC,nWidth/2+206,nHeight/2+120);
	}
	//�滭����
	CSize SizeJettonItem(m_ImageJettonView.GetWidth()/JETTON_COUNT,m_ImageJettonView.GetHeight());
	for (INT i=0;i<11;i++)
	{
		//��������
		LONG lScoreCount=0L;
		LONG lScoreJetton[JETTON_COUNT]={100L,1000L,10000L,100000L,1000000L,10000000L};

		for (INT_PTR j=0;j<m_JettonInfoArray[i].GetCount();j++)
		{
			//��ȡ��Ϣ
			tagJettonInfo * pJettonInfo=&m_JettonInfoArray[i][j];

			//�ۼ�����
			ASSERT(pJettonInfo->cbJettonIndex<JETTON_COUNT);
			lScoreCount+=lScoreJetton[pJettonInfo->cbJettonIndex];
		}

		//�滭����
		if (lScoreCount>0L)	DrawNumberString(pDC,lScoreCount,m_PointJettonNumber[i].x,m_PointJettonNumber[i].y);
	}
	//�ҵ���ע
	DrawMeJettonNumber(pDC);
	//�滭ѡ��״̬
	//if (m_cbSelectIndex!=0xff)
	//{
	//	CPoint pt;
	//	GetSelectPt(pt);
	//	m_ImageSelect.DrawImage(pDC,pt.x,
	//							pt.y,m_ImageSelect.GetWidth()/8,m_ImageSelect.GetHeight(),
	//							m_cbSelectIndex*119,0);
	//}
	//���ֶ���
	if (m_bIsMove)
	{
		CPoint pt;
		GetSelectPt(pt, m_cbMoveIndex);
		m_ImageSelect.DrawImage(pDC,pt.x,
			pt.y,m_ImageSelect.GetWidth()/3,m_ImageSelect.GetHeight(),
			0,0);
	}
	//�滭״̬ʱ��
	if (m_cbGameState==GS_FREE) m_ImageGameState.DrawImage(pDC, m_ptGameState.x, m_ptGameState.y,
														   m_ImageGameState.GetWidth()/3,m_ImageGameState.GetHeight(),
														   0,0);
	if (m_cbGameState==GS_PLACE_JETTON) m_ImageGameState.DrawImage(pDC, m_ptGameState.x, m_ptGameState.y,
															m_ImageGameState.GetWidth()/3,m_ImageGameState.GetHeight(),
															108,0);
	if (m_cbGameState==GS_GAME_END) m_ImageGameState.DrawImage(pDC, m_ptGameState.x, m_ptGameState.y,
															   m_ImageGameState.GetWidth()/3,m_ImageGameState.GetHeight(),
															   216,0);
	//�滭ʱ��
	DrawNumberString(pDC,m_cbGameTimer,m_ptGameTimer.x,m_ptGameTimer.y,false,true);

	//�滭��ʷ�ɼ�
	if (m_cbScoreHistroy.GetCount()>0)
	{
		if (m_cbScoreHistroy.GetCount()<8)
		{
			for (BYTE i=0; i<m_cbScoreHistroy.GetCount(); i++)
			{
				m_ImageScoreHistory.DrawImage(pDC, m_ptScoreHistory.x+i*(m_ImageScoreHistory.GetWidth()/8+10)+i*10+5, m_ptScoreHistory.y-306,
											  m_ImageScoreHistory.GetWidth()/8,m_ImageScoreHistory.GetHeight(),
											  (m_cbScoreHistroy[i]-1)*m_ImageScoreHistory.GetWidth()/8,0);
			}
		}
		else
		{
			for (BYTE i=m_cbScoreHistroyIndex, j=0; i<m_cbScoreHistroyIndex+7; i++,j++)
			{
				m_ImageScoreHistory.DrawImage(pDC, m_ptScoreHistory.x+(j*m_ImageScoreHistory.GetWidth()/8+10)+j*10+5, m_ptScoreHistory.y-306,
											  m_ImageScoreHistory.GetWidth()/8,m_ImageScoreHistory.GetHeight(),
											  (m_cbScoreHistroy[i]-1)*m_ImageScoreHistory.GetWidth()/8,0);
			}
		}
	}
}

//�����Լ�λ��
void CGameClientView::SetMeChairID(WORD wMeChair)
{
	m_wMeChairID = wMeChair;
	UpdateGameView(NULL);
}

//������Ϣ
void CGameClientView::SetMeMaxScore(LONG lMeMaxScore)
{
	if (m_lMeMaxScore!=lMeMaxScore)
	{
		//���ñ���
		m_lMeMaxScore=lMeMaxScore;
	}

	return;
}

//ׯ����Ϣ
void CGameClientView::SetBankerInfo(WORD wBankerUser, LONG lBankerScore) 
{
	//�л��ж�
	if (m_wBankerUser!=wBankerUser)
	{
		m_wBankerUser=wBankerUser;
		m_wBankerTime=0L;
		m_lTmpBankerWinScore=0L;
	}
	m_lBankerScore=lBankerScore;
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

//������ť
void CGameClientView::OnScoreLeft()
{
	m_cbScoreHistroyIndex--;
	if (m_cbScoreHistroyIndex>0)
		m_btScore_L.EnableWindow(TRUE);
	else
		m_btScore_L.EnableWindow(FALSE);
	if (m_cbScoreHistroyIndex<m_cbScoreHistroy.GetCount()-7)
		m_btScore_R.EnableWindow(TRUE);
	else
		m_btScore_R.EnableWindow(FALSE);
	//������Ϣ
	//AfxGetMainWnd()->SendMessage(IDM_SCORE_L,0,0);
	return;
}

//������ť
void CGameClientView::OnScoreRight()
{
	m_cbScoreHistroyIndex++;
	if (m_cbScoreHistroyIndex>0)
		m_btScore_L.EnableWindow(TRUE);
	else
		m_btScore_L.EnableWindow(FALSE);
	if (m_cbScoreHistroyIndex<m_cbScoreHistroy.GetCount()-7)
		m_btScore_R.EnableWindow(TRUE);
	else
		m_btScore_R.EnableWindow(FALSE);
		
	//������Ϣ
	//AfxGetMainWnd()->SendMessage(IDM_SCORE_R,0,0);
	return;
}

//������ť
void CGameClientView::OnBankerTop()
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_BANKER_T,0,0);
	return;
}

//������ť
void CGameClientView::OnBankerBottom()
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_BANKER_B,0,0);
	return;
}

//���밴ť
void CGameClientView::OnJettonButton100()
{
	//���ñ���
	m_lCurrentJetton=100L;
	m_btJetton100.ShowWindow(SW_HIDE);
	m_btJetton1000.ShowWindow(SW_HIDE);
	m_btJetton10000.ShowWindow(SW_HIDE);
	m_btJetton100000.ShowWindow(SW_HIDE);
	m_btJetton1000000.ShowWindow(SW_HIDE);
	m_btJetton10000000.ShowWindow(SW_HIDE);
	return;
}

//���밴ť
void CGameClientView::OnJettonButton1000()
{
	//���ñ���
	m_lCurrentJetton=1000L;
	m_btJetton100.ShowWindow(SW_HIDE);
	m_btJetton1000.ShowWindow(SW_HIDE);
	m_btJetton10000.ShowWindow(SW_HIDE);
	m_btJetton100000.ShowWindow(SW_HIDE);
	m_btJetton1000000.ShowWindow(SW_HIDE);
	m_btJetton10000000.ShowWindow(SW_HIDE);
	return;
}

//���밴ť
void CGameClientView::OnJettonButton10000()
{
	//���ñ���
	m_lCurrentJetton=10000L;
	m_btJetton100.ShowWindow(SW_HIDE);
	m_btJetton1000.ShowWindow(SW_HIDE);
	m_btJetton10000.ShowWindow(SW_HIDE);
	m_btJetton100000.ShowWindow(SW_HIDE);
	m_btJetton1000000.ShowWindow(SW_HIDE);
	m_btJetton10000000.ShowWindow(SW_HIDE);
	return;
}

//���밴ť
void CGameClientView::OnJettonButton100000()
{
	//���ñ���
	m_lCurrentJetton=100000L;
	m_btJetton100.ShowWindow(SW_HIDE);
	m_btJetton1000.ShowWindow(SW_HIDE);
	m_btJetton10000.ShowWindow(SW_HIDE);
	m_btJetton100000.ShowWindow(SW_HIDE);
	m_btJetton1000000.ShowWindow(SW_HIDE);
	m_btJetton10000000.ShowWindow(SW_HIDE);
	return;
}

//���밴ť
void CGameClientView::OnJettonButton1000000()
{
	//���ñ���
	m_lCurrentJetton=1000000L;
	m_btJetton100.ShowWindow(SW_HIDE);
	m_btJetton1000.ShowWindow(SW_HIDE);
	m_btJetton10000.ShowWindow(SW_HIDE);
	m_btJetton100000.ShowWindow(SW_HIDE);
	m_btJetton1000000.ShowWindow(SW_HIDE);
	m_btJetton10000000.ShowWindow(SW_HIDE);
	return;
}
//���밴ť
void CGameClientView::OnJettonButton10000000()
{
	//���ñ���
	m_lCurrentJetton=10000000L;
	m_btJetton100.ShowWindow(SW_HIDE);
	m_btJetton1000.ShowWindow(SW_HIDE);
	m_btJetton10000.ShowWindow(SW_HIDE);
	m_btJetton100000.ShowWindow(SW_HIDE);
	m_btJetton1000000.ShowWindow(SW_HIDE);
	m_btJetton10000000.ShowWindow(SW_HIDE);
	return;
}
//�����Ϣ
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	if (m_lCurrentJetton!=0L)
	{
		BYTE cbJettonArea=GetJettonArea(Point);
		//PlaceUserJetton(cbJettonArea, m_lCurrentJetton);
		//SetSelectIndex(cbJettonArea-1);
		//��С�ж�

		LONG lMaxBankerKingScore = GetMaxScore(m_cbJettonIndex);
		if ( lMaxBankerKingScore < 0)
		{
			return ;
		}

 		if (cbJettonArea!=0xFF) 
			AfxGetMainWnd()->SendMessage(IDM_PLACE_JETTON,cbJettonArea,m_lCurrentJetton);
	
	}

	__super::OnLButtonDown(nFlags,Point);
}

//�����Ϣ
void CGameClientView::OnRButtonDown(UINT nFlags, CPoint Point)
{
	//���ñ���
 	m_lCurrentJetton=0L;
	
	if (m_cbGameState!=GS_GAME_END)
	{
		m_cbJettonIndex = 0xff;
		UpdateGameView(NULL);
	}

	__super::OnLButtonDown(nFlags,Point);
}

//�����Ϣ
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	if (m_lCurrentJetton!=0L)
	{
		//��ȡ����
		CPoint MousePoint;
		GetCursorPos(&MousePoint);
		ScreenToClient(&MousePoint);
	    m_cbJettonIndex = GetJettonArea(MousePoint);
		if (m_cbJettonIndex!=0xFF&&m_cbGameState==GS_PLACE_JETTON)
		{
			UpdateGameView(NULL);
		}

		LONG lMaxPlayerScore = GetMaxScore(m_cbJettonIndex);
		if ( lMaxPlayerScore <0 )
		{
			SetCursor(LoadCursor(NULL,IDC_NO));
			return TRUE;
		}
	}

	return __super::OnSetCursor(pWnd, nHitTest, uMessage);
}
//��ȡ����
BYTE CGameClientView::GetJettonArea(CPoint MousePoint)
{
	if (m_rcBigShark.PtInRect(MousePoint)) return ID_BIG_SHARK;
	if (m_rcFeiQin.PtInRect(MousePoint)) return ID_FEI_QIN;
	if (m_rcZouShou.PtInRect(MousePoint)) return ID_ZOU_SHOU;
	if (m_rcYanZi.PtInRect(MousePoint)) return ID_YAN_ZI;
 	if (m_rcTuZi.PtInRect(MousePoint)) return ID_TU_ZI;
	if (m_rcGeZi.PtInRect(MousePoint)) return ID_GE_ZI;
	if (m_rcXiongMao.PtInRect(MousePoint)) return ID_XIONG_MAO;	
	if (m_rcKongQue.PtInRect(MousePoint)) return ID_KONG_QUE;	
	if (m_rcHouZi.PtInRect(MousePoint)) return ID_HOU_ZI;
	if (m_rcLaoYing.PtInRect(MousePoint)) return ID_LAO_YING;
	if (m_rcShiZi.PtInRect(MousePoint)) return ID_SHI_ZI;

	return 0xFF;
}
//���ó���
void CGameClientView::PlaceUserJetton(BYTE cbViewIndex, LONG lScoreCount)
{
	//Ч�����
	//ASSERT(cbViewIndex<=ID_SHI_ZI);
	if (cbViewIndex>ID_SHI_ZI) return;

	//����ը��
	//if (lScoreCount==10000000L) SetBombEffect(true,cbViewIndex);

	//��������
	bool bPlaceJetton=false;
	LONG lScoreIndex[JETTON_COUNT]={100,1000L,10000L,100000L,1000000L,10000000L};

	//�߿���
	int nFrameWidth=0, nFrameHeight=0;
	int nBorderWidth=6;

	switch (cbViewIndex)
	{
	case ID_BIG_SHARK:
		{
			m_lAllBigSharkScore += lScoreCount;
			nFrameWidth = m_rcBigShark.right-m_rcBigShark.left;
			nFrameHeight = m_rcBigShark.bottom-m_rcBigShark.top;
			break;
		}
	case ID_FEI_QIN:
		{ 
			m_lAllFeiQinScore += lScoreCount;
			nFrameWidth = m_rcFeiQin.right-m_rcFeiQin.left;
			nFrameHeight = m_rcFeiQin.bottom-m_rcFeiQin.top;
			break;
		}
	case ID_ZOU_SHOU:
		{
			m_lAllZouShouScore += lScoreCount;
			nFrameWidth = m_rcZouShou.right-m_rcZouShou.left;
			nFrameHeight = m_rcZouShou.bottom-m_rcZouShou.top;
			break;
		}
	case ID_YAN_ZI:
		{ 
			m_lALLYanZiScore += lScoreCount;
			nFrameWidth = m_rcYanZi.right-m_rcYanZi.left;
			nFrameHeight = m_rcYanZi.bottom-m_rcYanZi.top;
			break;
		}
	case ID_TU_ZI:
		{
			m_lALLTuZiScore += lScoreCount;
			nFrameWidth = m_rcTuZi.right-m_rcTuZi.left;
			nFrameHeight = m_rcTuZi.bottom-m_rcTuZi.top;
			break;
		}
	case ID_GE_ZI:
		{ 
			m_lALLGeZiScore += lScoreCount;
			nFrameWidth = m_rcGeZi.right-m_rcGeZi.left;
			nFrameHeight = m_rcGeZi.bottom-m_rcGeZi.top;
			break;
		}
	case ID_XIONG_MAO:
		{ 
			m_lALLXiongMaoScore += lScoreCount;
			nFrameWidth = m_rcXiongMao.right-m_rcXiongMao.left;
			nFrameHeight = m_rcXiongMao.bottom-m_rcXiongMao.top;
			break;
		}
	case ID_KONG_QUE:
		{ 
			m_lALLKongQueScore += lScoreCount;
			nFrameWidth = m_rcKongQue.right-m_rcKongQue.left;
			nFrameHeight = m_rcKongQue.bottom-m_rcKongQue.top;
			break;
		}
	case ID_HOU_ZI:
		{ 
			m_lALLHouZiScore += lScoreCount;
			nFrameWidth = m_rcHouZi.right-m_rcHouZi.left;
			nFrameHeight = m_rcHouZi.bottom-m_rcHouZi.top;
			break;
		}
	case ID_LAO_YING:
		{ 
			m_lALLLaoYingScore += lScoreCount;
			nFrameWidth = m_rcLaoYing.right-m_rcLaoYing.left;
			nFrameHeight = m_rcLaoYing.bottom-m_rcLaoYing.top;
			break;
		}
	case ID_SHI_ZI:
		{ 
			m_lALLShiZiScore += lScoreCount;
			nFrameWidth = m_rcShiZi.right-m_rcShiZi.left;
			nFrameHeight = m_rcShiZi.bottom-m_rcShiZi.top;
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
			//�������
			tagJettonInfo JettonInfo;
			int nJettonSize=68;
			JettonInfo.cbJettonIndex=cbScoreIndex;
			JettonInfo.nXPos=rand()%(nFrameWidth-nJettonSize);
			JettonInfo.nYPos=rand()%(nFrameHeight-nJettonSize);

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

//�������
void CGameClientView::CleanUserJetton()
{
	//��������
	for (BYTE i=0;i<CountArray(m_JettonInfoArray);i++)
	{
		m_JettonInfoArray[i].RemoveAll();
	}

	//������ע��Ϣ
	m_lMeBigSharkScore=0L;
	m_lMeFeiQinScore=0L;
	m_lMeZouShouScore=0L;
	m_lMeYanZiScore=0L;				
	m_lMeTuZiScore=0L;				
	m_lMeGeZiScore=0L;					
	m_lMeXiongMaoScore=0L;					
	m_lMeKongQueScore=0L;					
	m_lMeHouZiScore=0L;				
	m_lMeLaoYingScore=0L;				
	m_lMeShiZiScore=0L;					

	//ȫ����ע
	m_lAllBigSharkScore=0L;
	m_lAllFeiQinScore=0L;
	m_lAllZouShouScore=0L;
	m_lALLYanZiScore=0L;				
	m_lALLTuZiScore=0L;				
	m_lALLGeZiScore=0L;					
	m_lALLXiongMaoScore=0L;					
	m_lALLKongQueScore=0L;					
	m_lALLHouZiScore=0L;					
	m_lALLLaoYingScore=0L;					
	m_lALLShiZiScore=0L;

	//���½���
	UpdateGameView(NULL);

	return;
}

//�滭����
void CGameClientView::DrawNumberString(CDC * pDC, LONG lNumber, INT nXPos, INT nYPos, bool bMeScore,bool bTimer)
{
	CSize SizeScoreNumber(m_ImageScoreNumber.GetWidth()/11,m_ImageScoreNumber.GetHeight());

	if (bMeScore) 
		SizeScoreNumber.SetSize(m_ImageMeScoreNumber.GetWidth()/11, m_ImageMeScoreNumber.GetHeight());
	else if (bTimer)
		SizeScoreNumber.SetSize(m_ImageGameTimer.GetWidth()/10, m_ImageGameTimer.GetHeight());

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
	INT nXDrawPos=nXPos+lNumberCount*SizeScoreNumber.cx/2-SizeScoreNumber.cx+10;
	if (bTimer)
		nXDrawPos = nXPos + SizeScoreNumber.cx;

	INT nSpaceCOunt = lNumberCount/4;
	nSpaceCOunt = (lNumberCount==4)?0:nSpaceCOunt;
	nSpaceCOunt =(lNumberCount>5&&lNumberCount<9)?1:nSpaceCOunt;

	for (LONG i=0;i<lNumberCount;i++)
	{
		//�滭����
		LONG lCellNumber=(LONG)(lNumber%10);
		if (bMeScore)
		{
			m_ImageMeScoreNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
				lCellNumber*SizeScoreNumber.cx,0);
			////��������ж�
			//if ((nSpaceCOunt!=0))
			//{
			//	if (((i+1)%4)==0)
			//	{
			//		nSpaceCOunt--;
			//		nXDrawPos-=SizeScoreNumber.cx;
			//		m_ImageMeScoreNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
			//			10*SizeScoreNumber.cx,0);		
			//	}
			//}
		}
		else if(bTimer)
		{
			if (lNumberCount==1)
			{
				m_ImageGameTimer.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
					lCellNumber*SizeScoreNumber.cx,0);
				nXDrawPos-=SizeScoreNumber.cx;
				m_ImageGameTimer.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
					0,0);
			}
			else
			{
				m_ImageGameTimer.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
					lCellNumber*SizeScoreNumber.cx,0);
			}
		}
		//else
		//{
		//	m_ImageScoreNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
		//		lCellNumber*SizeScoreNumber.cx,0);
		//	//��������ж�
		//	if ((nSpaceCOunt!=0))
		//	{
		//		if (((i+1)%4)==0)
		//		{
		//			nSpaceCOunt--;
		//			nXDrawPos-=SizeScoreNumber.cx;
		//			m_ImageScoreNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
		//				10*SizeScoreNumber.cx,0);	
		//		}

		//	}
		//}
		//���ñ���
		lNumber/=10;
		nXDrawPos-=SizeScoreNumber.cx;
	};

	return;
}

//�滭����
void CGameClientView::DrawMeJettonNumber(CDC *pDC)
{
	//�滭����
	if (0 < m_lMeBigSharkScore) DrawNumberString(pDC,m_lMeBigSharkScore,m_PointJettonNumber[ID_BIG_SHARK-1].x,m_PointJettonNumber[ID_BIG_SHARK-1].y+40, true);
	if (0 < m_lMeFeiQinScore) DrawNumberString(pDC,m_lMeFeiQinScore,m_PointJettonNumber[ID_FEI_QIN-1].x,m_PointJettonNumber[ID_FEI_QIN-1].y+40, true);
	if (0 < m_lMeZouShouScore) DrawNumberString(pDC,m_lMeZouShouScore,m_PointJettonNumber[ID_ZOU_SHOU-1].x,m_PointJettonNumber[ID_ZOU_SHOU-1].y+40, true);
	if (0 < m_lMeYanZiScore) DrawNumberString(pDC,m_lMeYanZiScore,m_PointJettonNumber[ID_YAN_ZI-1].x,m_PointJettonNumber[ID_YAN_ZI-1].y+40, true);
	if (0 < m_lMeTuZiScore) DrawNumberString(pDC,m_lMeTuZiScore,m_PointJettonNumber[ID_TU_ZI-1].x,m_PointJettonNumber[ID_TU_ZI-1].y+40, true);
	if (0 < m_lMeGeZiScore) DrawNumberString(pDC,m_lMeGeZiScore,m_PointJettonNumber[ID_GE_ZI-1].x,m_PointJettonNumber[ID_GE_ZI-1].y+40, true);
	if (0 < m_lMeXiongMaoScore) DrawNumberString(pDC,m_lMeXiongMaoScore,m_PointJettonNumber[ID_XIONG_MAO-1].x,m_PointJettonNumber[ID_XIONG_MAO-1].y+40, true);
	if (0 < m_lMeKongQueScore) DrawNumberString(pDC,m_lMeKongQueScore,m_PointJettonNumber[ID_KONG_QUE-1].x,m_PointJettonNumber[ID_KONG_QUE-1].y+40, true);
	if (0 < m_lMeHouZiScore) DrawNumberString(pDC,m_lMeHouZiScore,m_PointJettonNumber[ID_HOU_ZI-1].x,m_PointJettonNumber[ID_HOU_ZI-1].y+40, true);
	if (0 < m_lMeLaoYingScore) DrawNumberString(pDC,m_lMeLaoYingScore,m_PointJettonNumber[ID_LAO_YING-1].x,m_PointJettonNumber[ID_LAO_YING-1].y+40, true);
	if (0 < m_lMeShiZiScore) DrawNumberString(pDC,m_lMeShiZiScore,m_PointJettonNumber[ID_SHI_ZI-1].x,m_PointJettonNumber[ID_SHI_ZI-1].y+40, true);
}

//�滭����
void CGameClientView::DrawNumberStringWithSpace(CDC * pDC, LONG lNumber, INT nXPos, INT nYPos)
{
	CString strNumber=TEXT("");
	strNumber.Format(TEXT("%ld"),lNumber>0?lNumber:-lNumber);
	int p=strNumber.GetLength()-4;
	while(p>0)
	{
		strNumber.Insert(p,",");
		p-=4;
	}
	if(lNumber<0)
	{
		strNumber=TEXT("-")+strNumber;
	}
	//�������
	pDC->TextOut(nXPos,nYPos,strNumber);
}
//������ע
void CGameClientView::SetMePlaceJetton(BYTE cbViewIndex, LONG lJettonCount)
{
	//Ч�����
	//ASSERT(cbViewIndex<=ID_BANKER_TWO_PAIR);
	if (cbViewIndex>ID_SHI_ZI) return;

	if (cbViewIndex==ID_BIG_SHARK) m_lMeBigSharkScore=lJettonCount;
	else if (cbViewIndex==ID_FEI_QIN) m_lMeFeiQinScore=lJettonCount;
	else if (cbViewIndex==ID_ZOU_SHOU) m_lMeZouShouScore=lJettonCount;
	else if (cbViewIndex==ID_YAN_ZI) m_lMeYanZiScore=lJettonCount;
	else if (cbViewIndex==ID_TU_ZI) m_lMeTuZiScore=lJettonCount;
	else if (cbViewIndex==ID_GE_ZI) m_lMeGeZiScore=lJettonCount;
	else if (cbViewIndex==ID_XIONG_MAO) m_lMeXiongMaoScore=lJettonCount;
	else if (cbViewIndex==ID_KONG_QUE) m_lMeKongQueScore=lJettonCount;
	else if (cbViewIndex==ID_HOU_ZI) m_lMeHouZiScore=lJettonCount;
	else if (cbViewIndex==ID_LAO_YING) m_lMeLaoYingScore=lJettonCount;
	else if (cbViewIndex==ID_SHI_ZI) m_lMeShiZiScore=lJettonCount;
	else {ASSERT(FALSE);return;}

	//���½���
	UpdateGameView(NULL);
}

//����ѡ������
void CGameClientView::SetSelectIndex(BYTE cbSelectIndex)
{
	m_cbSelectIndex = cbSelectIndex;
	m_cbSlectPos = rand()%4;
	UpdateGameView(NULL);
}
#define PI	3.1415926543
//��ȡѡ�е�
void CGameClientView::GetSelectPt(CPoint &ptSelect,BYTE cbMoveIndex)
{
	double dstep = 2*PI/360;
	double dAngle =dstep*GetAngle(cbMoveIndex);
	ptSelect.x = 310*cos(dAngle)+482+GetErrorPoint(cbMoveIndex).x-87;
	ptSelect.y = 200*sin(dAngle)+220+GetErrorPoint(cbMoveIndex).y;
}

//��ȡ�Ƕ�
double CGameClientView::GetAngle(BYTE cbMoveIndex)
{
	double dAngle[28] ={0};
	for (int i=0;i<28;i++)
	{
		dAngle[i]=(i*90)/7;
	}

	return dAngle[cbMoveIndex%28];
}

//�������
POINT CGameClientView::GetErrorPoint(BYTE cbMoveIndex)
{
	POINT pt[28];
	ZeroMemory(pt,sizeof(pt));
	pt[1].x=-4;
	pt[1].y=12;
	pt[2].x=-13;
	pt[2].y=17;
	pt[3].x=-21;
	pt[3].y=14;
	pt[4].x=-24;
	pt[4].y=9;
	pt[5].x=-22;
	pt[5].y=4;
	pt[6].x=-16;
	pt[6].y=-1;
	pt[7].x=-3;
	pt[7].y=-3;
	pt[8].x=3;
	pt[8].y=-3;
	pt[9].x=15;
	pt[9].y=0;
	pt[10].x=19;
	pt[10].y=6;
	pt[11].x=18;
	pt[11].y=12;
	pt[12].x=12;
	pt[12].y=15;
	pt[13].x=5;
	pt[13].y=10;
	pt[14].x=2;
	pt[14].y=0;
	pt[15].x=5;
	pt[15].y=-13;
	pt[16].x=12;
	pt[16].y=-18;
	pt[17].x=23;
	pt[17].y=-14;
	pt[18].x=23;
	pt[18].y=-10;
	pt[19].x=18;
	pt[19].y=-3;
	pt[20].x=8;
	pt[20].y=3;
	pt[21].x=-3;
	pt[21].y=3;
	pt[22].x=-10;
	pt[22].y=4;
	pt[23].x=-19;
	pt[23].y=0;
	pt[24].x=-21;
	pt[24].y=-7;
	pt[25].x=-19;
	pt[25].y=-13;
	pt[26].x=-11;
	pt[26].y=-15;
	pt[27].x=-3;
	pt[27].y=-11;

	return pt[cbMoveIndex%28];
}

void CGameClientView::SetCanMove(bool bMove)
{
	m_bIsMove = bMove;
	//UpdateGameView(NULL);
}

//���ö�������
void CGameClientView::SetMoveIndex(BYTE cbMoveIndex)
{
	m_cbMoveIndex = cbMoveIndex;
	UpdateGameView(NULL);
}

//���ó���
void CGameClientView::SetCurrentJetton(LONG lCurrentJetton)
{
	//���ñ���
	ASSERT(lCurrentJetton>=0L);
	m_lCurrentJetton=lCurrentJetton;

	return;
}

//������Ϸ״̬
void CGameClientView::SetGameState(BYTE cbState)
{
	m_cbGameState = cbState;
	UpdateGameView(NULL);
}

//������Ϸʱ��
void CGameClientView::SetGameTimerNum(BYTE cbTimerNum)
{
	m_cbGameTimer = cbTimerNum;
	UpdateGameView(NULL);
}

//���ÿ��ý��
void CGameClientView::SetMeCanUseSocre(LONG lCanUseScore)
{
	m_lMeCanUseScore = lCanUseScore;
	UpdateGameView(NULL);
}

//���óɼ�
void CGameClientView::SetMeScore(LONG lMeScore)
{
	m_lMeStatisticScore = lMeScore;
	UpdateGameView(NULL);
}

//����ѡ������
void CGameClientView::SetSelectJettonIndex(BYTE cbSelectIndex)
{
	m_cbJettonIndex = cbSelectIndex;
	UpdateGameView(NULL);
}

//���ø��˳ɼ�
void CGameClientView::SetMeHisScore(LONG lHisScore)
{
	m_lMeStatisticScore = lHisScore;
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

//������ʷ��Ϣ
void CGameClientView::SetScoreHistory(BYTE cbScoreHistory[MAX_SCORE_HISTORY])
{
	m_cbScoreHistroy.RemoveAll();
	for (BYTE i=0; i<MAX_SCORE_HISTORY; i++)
	{
		if (cbScoreHistory[i]!=0)
		{
			m_cbScoreHistroy.Add(cbScoreHistory[i]);
		}
	}
	if (m_cbScoreHistroy.GetCount()>6)
		m_cbScoreHistroyIndex = (BYTE)m_cbScoreHistroy.GetCount()-7;
	else
		m_cbScoreHistroyIndex = 0;
	if (m_cbScoreHistroyIndex>0)
		m_btScore_L.EnableWindow(TRUE);
	else
		m_btScore_L.EnableWindow(FALSE);
	if (m_cbScoreHistroyIndex<m_cbScoreHistroy.GetCount()-7)
		m_btScore_R.EnableWindow(TRUE);
	else
		m_btScore_R.EnableWindow(FALSE);
	UpdateGameView(NULL);
}

//�����ע
LONG CGameClientView::GetMaxScore(BYTE cbArea)
{
	LONG lAreaScore[]={0,m_lAllBigSharkScore,m_lAllFeiQinScore,m_lAllZouShouScore,
					m_lALLYanZiScore,m_lALLTuZiScore,m_lALLGeZiScore,m_lALLXiongMaoScore,
					m_lALLKongQueScore,m_lALLHouZiScore,m_lALLLaoYingScore,m_lALLShiZiScore};
	LONG lMaxPlayerScore = 0;
	lMaxPlayerScore=m_lAreaLimitScore-lAreaScore[cbArea];
	return lMaxPlayerScore;
}

//��ȡ��ť
void CGameClientView::OnSaveBank()
{
	//IClientKernel *pIClientKernel = (IClientKernel *)g_pGameClient->GetClientKernel(IID_IClientKernel,VER_IClientKernel);
	//ASSERT(pIClientKernel!=NULL);
	//if (pIClientKernel==NULL) return ;
	//pIClientKernel->ShowBankDialog( 0,true );
	//return ;
}

//ȡǮ��ť
void CGameClientView::OnGetBank()
{
	//IClientKernel *pIClientKernel = (IClientKernel *)g_pGameClient->GetClientKernel(IID_IClientKernel,VER_IClientKernel);
	//ASSERT(pIClientKernel!=NULL);
	//if (pIClientKernel==NULL) return ;
	//pIClientKernel->ShowBankDialog( 1,true );
	//return ;
}

void CGameClientView::OnJettonButton()
{
	m_btJetton100.ShowWindow(m_bIsShowJetton);	
	m_btJetton1000.ShowWindow(m_bIsShowJetton);	
	m_btJetton10000.ShowWindow(m_bIsShowJetton);
	m_btJetton100000.ShowWindow(m_bIsShowJetton);
	m_btJetton1000000.ShowWindow(m_bIsShowJetton);
	m_btJetton10000000.ShowWindow(m_bIsShowJetton);
	m_bIsShowJetton=!m_bIsShowJetton;
}


/////////////////////////////////////////////////////////////////
/////////
