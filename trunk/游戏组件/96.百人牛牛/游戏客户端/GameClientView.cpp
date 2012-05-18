#include "StdAfx.h"
#include "Math.h"
#include "Resource.h"
#include "GameClientView.h"
#include "GameClientDlg.h"
#include ".\gameclientview.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_FLASH_WINNER			100									//������ʶ
#define IDI_SHOW_CHANGE_BANKER		101									//�ֻ�ׯ��
#define IDI_DISPATCH_CARD			102									//���Ʊ�ʶ
#define DISPATCH_CARD_TIP	        103									//������ʾ
#define IDI_OPENCARD				104									//������ʾ
#define IDI_MOVECARD_END			105									//�Ƅ��ƽY��
#define IDI_POSTCARD				106									//������ʾ

//��ť��ʶ
#define IDC_JETTON_BUTTON_100		200									//��ť��ʶ
#define IDC_JETTON_BUTTON_1000		201									//��ť��ʶ
#define IDC_JETTON_BUTTON_10000		202									//��ť��ʶ
#define IDC_JETTON_BUTTON_100000	203									//��ť��ʶ
#define IDC_JETTON_BUTTON_1000000	204									//��ť��ʶ
#define IDC_JETTON_BUTTON_5000000	205									//��ť��ʶ
#define IDC_APPY_BANKER				206									//��ť��ʶ
#define IDC_CANCEL_BANKER			207									//��ť��ʶ
#define IDC_SCORE_MOVE_L			209									//��ť��ʶ
#define IDC_SCORE_MOVE_R			210									//��ť��ʶ
#define IDC_VIEW_CHART				211									//��ť��ʶ
#define IDC_JETTON_BUTTON_50000	    219									//��ť��ʶ
#define IDC_JETTON_BUTTON_500000	212									//��ť��ʶ
#define IDC_AUTO_OPEN_CARD			213									//��ť��ʶ
#define IDC_OPEN_CARD				214									//��ť��ʶ
#define IDC_BANK					215									//��ť��ʶ
#define IDC_CONTINUE_CARD			216									//��ť��ʶ
#define IDC_UP						223									//��ť��ʶ
#define IDC_DOWN					224									//��ť��ʶ

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_UP,OnUp)
	ON_BN_CLICKED(IDC_DOWN,OnDown)
	ON_BN_CLICKED(IDC_OPEN_CARD,OnOpenCard)
	ON_BN_CLICKED(IDC_BANK_DRAW,OnBankDraw)
	ON_BN_CLICKED(IDC_BT_ADMIN,OpenAdminWnd)
	ON_BN_CLICKED(IDC_APPY_BANKER,OnApplyBanker)
	ON_BN_CLICKED(IDC_SCORE_MOVE_L,OnScoreMoveL)
	ON_BN_CLICKED(IDC_SCORE_MOVE_R,OnScoreMoveR)
	ON_BN_CLICKED(IDC_BANK_STORAGE,OnBankStorage)
	ON_BN_CLICKED(IDC_CANCEL_BANKER,OnCancelBanker)
	ON_BN_CLICKED(IDC_AUTO_OPEN_CARD,OnAutoOpenCard)
	ON_MESSAGE(WM_VIEWLBTUP,OnViLBtUp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView() : CGameFrameView(true,24)
{
	//��ע��Ϣ
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//ȫ����ע
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	//ׯ����Ϣ
	m_wBankerUser=INVALID_CHAIR;		
	m_wBankerTime=0;
	m_lBankerScore=0L;	
	m_lBankerWinScore=0L;
	m_lTmpBankerWinScore=0;

	//���ֳɼ�
	m_lMeCurGameScore=0L;	
	m_lMeCurGameReturnScore=0L;
	m_lBankerCurGameScore=0L;
	m_lGameRevenue=0L;

	//״̬��Ϣ
	m_lCurrentJetton=0L;
	m_cbAreaFlash=0xFF;
	m_wMeChairID=INVALID_CHAIR;
	m_bShowChangeBanker=false;
	m_bNeedSetGameRecord=false;
	m_bWinTianMen=false;
	m_bWinHuangMen=false;
	m_bWinXuanMen=false;
	m_bFlashResult=false;
	m_blMoveFinish = false;
	m_blAutoOpenCard = true;
	m_enDispatchCardTip=enDispatchCardTip_NULL;

	m_lMeCurGameScore=0L;			
	m_lMeCurGameReturnScore=0L;	
	m_lBankerCurGameScore=0L;		

	m_lAreaLimitScore=0L;	

	//λ����Ϣ
	m_nScoreHead = 0;
	m_nRecordFirst= 0;	
	m_nRecordLast= 0;	

	//��ʷ�ɼ�
	m_lMeStatisticScore=0;

	//�ؼ�����
	m_pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageViewFill.SetLoadInfo(IDB_VIEW_FILL,hInstance);
	m_ImageViewBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_ImageWinFlags.SetLoadInfo(IDB_WIN_FLAGS,hInstance);
	m_ImageJettonView.SetLoadInfo(IDB_JETTOM_VIEW,hInstance);
	m_ImageScoreNumber.SetLoadInfo(IDB_SCORE_NUMBER,hInstance);
	m_ImageMeScoreNumber.SetLoadInfo(IDB_ME_SCORE_NUMBER,hInstance);

	//�߿���Դ
	m_ImageFrameTianMen.SetLoadInfo(IDB_FRAME_TIAN_MEN_EX, hInstance);
	m_ImageFrameDiMen.SetLoadInfo(IDB_FRAME_DI_MEN_EX, hInstance);
	m_ImageFrameQiao.SetLoadInfo(IDB_FRAME_QIAO_EX, hInstance);
	m_ImageFrameHuangMen.SetLoadInfo(IDB_FRAME_HUANG_MEN_EX, hInstance);
	m_ImageFrameXuanMen.SetLoadInfo(IDB_FRAME_XUAN_MEN_EX, hInstance);
	m_ImageFrameJiaoR.SetLoadInfo(IDB_FRAME_JIAO_R_EX, hInstance);
	for (int i = 0;i<5;i++)m_ImageCardType[i].SetLoadInfo(IDB_CARDTYPE,hInstance);
	m_ImageGameEnd.SetLoadInfo( IDB_GAME_END, hInstance );

	m_ImageMeBanker.SetLoadInfo( IDB_ME_BANKER, hInstance );
	m_ImageChangeBanker.SetLoadInfo( IDB_CHANGE_BANKER, hInstance );
	m_ImageNoBanker.SetLoadInfo( IDB_NO_BANKER, hInstance );	

	m_ImageTimeFlag.SetLoadInfo(IDB_TIME_FLAG, hInstance);

	/*const BYTE CGameLogic::m_cbCardListData[CARD_COUNT]=
	{
			0x0C,0x2C,
			0x0B,0x2B,
			0x0A,0x1A,0x2A,0x3A,
			0x09,0x29,
			0x07,0x17,0x27,0x37,
			0x08,0x18,0x28,0x38,
			0x06,0x16,0x26,0x36,
			0x05,0x25,
			0x04,0x14,0x24,0x34,
			0x02,0x22,
			0x42,
			0x23

			QQ��22��������3����8�ԣ���4�ԣ���10�ԡ���6�ԡ���4�ԣ�����3��Ϊһ����
			����10�� ��6�ԡ���7�ԡ���J�� ������4��һ���󣩺�8�ԡ���7�ԡ���5�ԡ���Ŷԣ�����4��һ����
			����һ������� ��ô���ܵ�����Ϊׯ�Ҵ� Q+ 9��϶��㣾  Q+8 ��϶���(������8����) ��2��8��϶��㣨������8�� 
			>Q��2����8����4����10=��6=��4=��10=��6=��7=��J=��8=��7=��5=���>����==3
	};*/
	//m_cbTableCardArray[SHUN_MEN_INDEX ][0]=0x0A;
	//m_cbTableCardArray[SHUN_MEN_INDEX ][1]=0x2A;
	//m_cbTableCardArray[ BANKER_INDEX][0]=0x1A;
	//m_cbTableCardArray[BANKER_INDEX][1]=0x3A;

	BYTE bcTmp[5]={0x2B,0x2C,0x0D,0x3B,0x1D};
	////	BYTE bcTmp1[5]={0x05,0x35,0x25,0x15,0x13};
	emCardType itype = (emCardType)m_GameLogic.GetCardType(bcTmp,5);

	m_lUserCardType[0] = itype;
	//BYTE Byte;
	//bool bWinShunMen=m_GameLogic.CompareCard(bcTmp,5,bcTmp1,5,Byte)==1?true:false;

	return;
}

//��������
CGameClientView::~CGameClientView(void)
{
}

void CGameClientView::SetMoveCardTimer()
{
	KillTimer(IDI_POSTCARD);
	m_OpenCardIndex = 1;
	m_bcShowCount = 0;
	m_PostStartIndex =  m_GameLogic.GetCardValue(m_bcfirstShowCard);
	if(m_PostStartIndex>1)
		m_PostStartIndex  = (m_PostStartIndex-1)%5;
	m_PostCardIndex=m_PostStartIndex;
	for (int i = 0;i<5;i++)
	{
		m_CardControl[i].m_blAdd = true;
		m_CardControl[i].m_blAuto = m_blAutoOpenCard;
		m_CardControl[i].m_blGameEnd = true;
		m_CardControl[i].m_blShowResult = false;
		m_CardControl[i].m_blShowLineResult = false;
	}
	SetTimer(IDI_POSTCARD,300,NULL);
	SetTimer(IDI_DISPATCH_CARD,25000,NULL);

}
//������Ϣ
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//�����ؼ�
	CRect rcCreate(0,0,0,0);
	m_GameRecord.Create(IDD_DLG_GAME_RECORD,this);
	m_ApplyUser.Create( IDD_DLG_GAME_RECORD	, this );
	m_GameRecord.ShowWindow(SW_HIDE);
	
	//��ע��ť
	m_btJetton100.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_100);
	m_btJetton1000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_1000);
	m_btJetton10000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_10000);
	m_btJetton50000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_50000);
	m_btJetton100000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_100000);
	
	m_btJetton500000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_500000);
	m_btJetton1000000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_1000000);
	m_btJetton5000000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_5000000);
		
	//���밴ť
	m_btApplyBanker.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_APPY_BANKER);
	m_btCancelBanker.Create(NULL,WS_CHILD|WS_DISABLED,rcCreate,this,IDC_CANCEL_BANKER);

	m_btScoreMoveL.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_L);
	m_btScoreMoveR.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_R);

	m_btAutoOpenCard.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_AUTO_OPEN_CARD);
	m_btOpenCard.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_OPEN_CARD);

	m_btBank.Create(NULL,WS_CHILD,rcCreate,this,IDC_BANK);
	m_btContinueCard.Create(NULL,WS_CHILD,rcCreate,this,IDC_CONTINUE_CARD);

	m_btUp.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_UP);
	m_btDown.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_DOWN);

//	m_btBankerStorage.EnableWindow(TRUE);
	m_btOpenAdmin.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(4,4,11,11),this,IDC_BT_ADMIN);
	m_btOpenAdmin.ShowWindow(SW_HIDE);

	m_AdminDlg.Create(IDD_DIALOG_ADMIN,this);
	m_AdminDlg.ShowWindow(SW_HIDE);

	//���ð�ť
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_btJetton100.SetButtonImage(IDB_BT_JETTON_100,hResInstance,false);
	m_btJetton1000.SetButtonImage(IDB_BT_JETTON_1000,hResInstance,false);
	m_btJetton50000.SetButtonImage(IDB_BT_JETTON_50000,hResInstance,false);
	m_btJetton10000.SetButtonImage(IDB_BT_JETTON_10000,hResInstance,false);
	m_btJetton100000.SetButtonImage(IDB_BT_JETTON_100000,hResInstance,false);
	m_btJetton500000.SetButtonImage(IDB_BT_JETTON_500000,hResInstance,false);
	m_btJetton1000000.SetButtonImage(IDB_BT_JETTON_1000000,hResInstance,false);
	m_btJetton5000000.SetButtonImage(IDB_BT_JETTON_5000000,hResInstance,false);

	m_btApplyBanker.SetButtonImage(IDB_BT_APPLY_BANKER,hResInstance,false);
	m_btCancelBanker.SetButtonImage(IDB_BT_CANCEL_APPLY,hResInstance,false);

	m_btUp.SetButtonImage(IDB_BT_BT_S,hResInstance,false);
	m_btDown.SetButtonImage(IDB_BT_BT_X,hResInstance,false);

	m_btScoreMoveL.SetButtonImage(IDB_BT_SCORE_MOVE_L,hResInstance,false);
	m_btScoreMoveR.SetButtonImage(IDB_BT_SCORE_MOVE_R,hResInstance,false);

	m_btAutoOpenCard.SetButtonImage(IDB_BT_AUTO_OPEN_CARD,hResInstance,false);
	m_btOpenCard.SetButtonImage(IDB_BT_OPEN_CARD,hResInstance,false);

	m_btBank.SetButtonImage(IDB_BT_BANK,hResInstance,false);
	m_btContinueCard.SetButtonImage(IDB_BT_CONTINUE_CARD,hResInstance,false);

	m_btBankerStorage.SetButtonImage(IDB_BT_STORAGE,hResInstance,false);
	m_btBankerDraw.SetButtonImage(IDB_BT_DRAW,hResInstance,false);

	//tagApplyUser ApplyUser;
	//ApplyUser.strUserName="fdsfds12333333333333333333333";
	//m_ApplyUser.InserUser(ApplyUser);
	//ApplyUser.strUserName="fdsfds1";
	//m_ApplyUser.InserUser(ApplyUser);
	//ApplyUser.strUserName="fdsfds2";
	//m_ApplyUser.InserUser(ApplyUser);
	//ApplyUser.strUserName="fdsfds3";
	//m_ApplyUser.InserUser(ApplyUser);
	//ApplyUser.strUserName="fdsfds4";
	//m_ApplyUser.InserUser(ApplyUser);
	//ApplyUser.strUserName="fdsfds5";
	//m_ApplyUser.InserUser(ApplyUser);

#ifdef __BANKER___
	if(m_DlgBank.m_hWnd==NULL) m_DlgBank.Create(IDD_BANK_STORAGE,this);

	m_btBankerStorage.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BANK_STORAGE);
	m_btBankerDraw.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BANK_DRAW);

	m_btBankerStorage.SetButtonImage(IDB_BT_STORAGE,hResInstance,false);
	m_btBankerDraw.SetButtonImage(IDB_BT_DRAW,hResInstance,false);
#endif

	//�˿˿ؼ�
	for (int i=0; i<CountArray(m_CardControl); ++i) m_CardControl[i].SetDisplayFlag(true);
	return 0;
}

//���ý���
void CGameClientView::ResetGameView()
{
	//��ע��Ϣ
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//ȫ����ע
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	//ׯ����Ϣ
	m_wBankerUser=INVALID_CHAIR;		
	m_wBankerTime=0;
	m_lBankerScore=0L;	
	m_lBankerWinScore=0L;
	m_lTmpBankerWinScore=0;

	//���ֳɼ�
	m_lMeCurGameScore=0L;	
	m_lMeCurGameReturnScore=0L;
	m_lBankerCurGameScore=0L;
	m_lGameRevenue=0L;

	//״̬��Ϣ
	m_lCurrentJetton=0L;
	m_cbAreaFlash=0xFF;
	m_wMeChairID=INVALID_CHAIR;
	m_bShowChangeBanker=false;
	m_bNeedSetGameRecord=false;
	m_bWinTianMen=false;
	m_bWinHuangMen=false;
	m_bWinXuanMen=false;
	m_bFlashResult=false;
	m_bShowGameResult=false;
	m_enDispatchCardTip=enDispatchCardTip_NULL;

	m_lMeCurGameScore=0L;			
	m_lMeCurGameReturnScore=0L;	
	m_lBankerCurGameScore=0L;		

	m_lAreaLimitScore=0L;	

	//λ����Ϣ
	m_nScoreHead = 0;
	m_nRecordFirst= 0;	
	m_nRecordLast= 0;	

	//��ʷ�ɼ�
	m_lMeStatisticScore=0;

	//����б�
	m_ApplyUser.ClearAll();

	//�������
	CleanUserJetton();

	//���ð�ť
	m_btApplyBanker.ShowWindow(SW_SHOW);
	m_btApplyBanker.EnableWindow(FALSE);
	m_btCancelBanker.ShowWindow(SW_HIDE);
	m_btCancelBanker.SetButtonImage(IDB_BT_CANCEL_APPLY,AfxGetInstanceHandle(),false);

	return;
}

//�����ؼ�
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	//λ����Ϣ
	m_nWinFlagsExcursionX = nWidth/2-260-41;	
	m_nWinFlagsExcursionY = nHeight/2 + 246-21;

	CSize Size;
	m_CardControl[0].GetControlSize(Size,3);
	Size.cy = Size.cy/2;

	CImageHandle ImageHandle(&m_ImageViewBack);
	int iWidth = m_ImageViewBack.GetWidth();

	int LifeWidth = nWidth/2-iWidth/2;
	int TopHeight = nHeight/2-m_ImageViewBack.GetHeight()/2;

	//����λ��
	int nCenterX=nWidth/2, nCenterY=nHeight/2, nBorderWidth=4;
	m_rcTianMen.left=LifeWidth+33+22;
	m_rcTianMen.top=TopHeight+257+7;
	m_rcTianMen.right=m_rcTianMen.left+167;
	m_rcTianMen.bottom=m_rcTianMen.top+138;

	m_rcDimen.left=LifeWidth+33+25+172;
	m_rcDimen.top=TopHeight+257+7;
	m_rcDimen.right=m_rcDimen.left+167;
	m_rcDimen.bottom=m_rcDimen.top+138;

	m_rcXuanMen.left=LifeWidth+33+25+177+170;
	m_rcXuanMen.top=TopHeight+257+7;
	m_rcXuanMen.right=m_rcXuanMen.left+167;
	m_rcXuanMen.bottom=m_rcXuanMen.top+138;


	m_rcHuangMen.left=LifeWidth+33+25+175+175+173;
	m_rcHuangMen.top=TopHeight+257+7;
	m_rcHuangMen.right=m_rcHuangMen.left+167;
	m_rcHuangMen.bottom=m_rcHuangMen.top+138;

	//��������
	int ExcursionY=10;
	m_PointJettonNumber[ID_TIAN_MEN-1].SetPoint((m_rcTianMen.right+m_rcTianMen.left)/2, (m_rcTianMen.bottom+m_rcTianMen.top)/2-ExcursionY);
	m_PointJettonNumber[ID_DI_MEN -1].SetPoint((m_rcDimen.right+m_rcDimen.left)/2, (m_rcDimen.bottom+m_rcDimen.top)/2-ExcursionY);
	m_PointJettonNumber[ID_HUANG_MEN-1].SetPoint((m_rcHuangMen.right+m_rcHuangMen.left)/2, (m_rcHuangMen.bottom+m_rcHuangMen.top)/2-ExcursionY);
	m_PointJettonNumber[ID_XUAN_MEN-1].SetPoint((m_rcXuanMen.right+m_rcXuanMen.left)/2, (m_rcXuanMen.bottom+m_rcXuanMen.top)/2-ExcursionY);

	//����λ��
	m_PointJetton[ID_TIAN_MEN-1].SetPoint(m_rcTianMen.left, m_rcTianMen.top);
	m_PointJetton[ID_DI_MEN-1].SetPoint(m_rcDimen.left, m_rcDimen.top);
	m_PointJetton[ID_HUANG_MEN -1].SetPoint(m_rcHuangMen.left, m_rcHuangMen.top);
	m_PointJetton[ID_XUAN_MEN-1].SetPoint(m_rcXuanMen.left, m_rcXuanMen.top);


	//�˿˿ؼ�
	m_CardControl[0].SetBenchmarkPos(CPoint(nWidth/2,nHeight/2-355+135),enXCenter,enYTop);
	m_CardTypePoint[0] =CPoint(nWidth/2+125,nHeight/2-355+135+62); 

	m_CardControl[1].SetBenchmarkPos(CPoint(LifeWidth+Size.cy+Size.cy/6,nHeight/2+60-20),enXCenter,enYTop);
	m_CardTypePoint[1] =CPoint(LifeWidth+Size.cy+Size.cy/6,nHeight/2+60+112); 

	m_CardControl[2].SetBenchmarkPos(CPoint(nWidth/2-Size.cy+Size.cy/4-10,nHeight/2+60-20),enXCenter,enYTop);
	m_CardTypePoint[2] =CPoint(nWidth/2-Size.cy+Size.cy/4,nHeight/2+60+112); 

	m_CardControl[3].SetBenchmarkPos(CPoint(nWidth/2+Size.cy-Size.cy/4-10,nHeight/2+60-20),enXCenter,enYTop);
	m_CardTypePoint[3] =CPoint(nWidth/2+Size.cy-Size.cy/4,nHeight/2+60+112); 

	m_CardControl[4].SetBenchmarkPos(CPoint(nWidth-LifeWidth-Size.cy-Size.cy/6-20,nHeight/2+60-20),enXCenter,enYTop);
	m_CardTypePoint[4] =CPoint(nWidth-LifeWidth-Size.cy-Size.cy/6,nHeight/2+60+112); 

	m_CardControl[5].SetBenchmarkPos(CPoint(nWidth/2,nHeight/2-100),enXCenter,enYTop);

	//�ƶ��ؼ�
	HDWP hDwp=BeginDeferWindowPos(33);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;

	m_ApplyUser.m_viewHandle = m_hWnd;

	//�б�ؼ�
	DeferWindowPos(hDwp,m_ApplyUser,NULL,LifeWidth+245+20,TopHeight+10+9,311,92,uFlags);
	DeferWindowPos(hDwp,m_btUp,NULL,LifeWidth+245+20+310,TopHeight+10+10,75/5,16,uFlags);
	DeferWindowPos(hDwp,m_btDown,NULL,LifeWidth+245+20+310,TopHeight+10+12+88-20,75/5,16,uFlags);

	//JJWillDo ɾ������ ������true��false
	/*tagApplyUser ap;
	for (int i = 0; i < 8 ; i++)
	{
		ap.lUserScore = i+1;
		ap.strUserName.Format(_T("a%d"), i+1);
		m_ApplyUser.InserUser(ap);
	}*/
	
	m_btUp.ShowWindow(SW_SHOW);
	m_btUp.EnableWindow(false);
	m_btDown.ShowWindow(SW_SHOW);
	m_btDown.EnableWindow(false);

	m_MeInfoRect.top = TopHeight+10+12;
	m_MeInfoRect.left = LifeWidth+245+20+389;

	//���밴ť
	CRect rcJetton;
	m_btJetton100.GetWindowRect(&rcJetton);
	int nYPos = nHeight/2+265-42;
	int nXPos = nWidth/2+80+25;
	int nSpace = 0;

	DeferWindowPos(hDwp,m_btJetton100,NULL,nXPos,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton1000,NULL,nXPos + nSpace + rcJetton.Width(),nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton10000,NULL,nXPos + nSpace * 2 + rcJetton.Width() * 2,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton50000,NULL,nXPos + nSpace * 3 + rcJetton.Width() * 3,nYPos,0,0,uFlags|SWP_NOSIZE);

	DeferWindowPos(hDwp,m_btJetton100000,NULL,nXPos + nSpace * 0 + rcJetton.Width() * 0,nYPos+rcJetton.Height()-5,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton500000,NULL,nXPos + nSpace * 1 + rcJetton.Width() * 1,nYPos+rcJetton.Height()-5,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton1000000,NULL,nXPos + nSpace * 2 + rcJetton.Width() * 2,nYPos+rcJetton.Height()-5,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton5000000,NULL,nXPos + nSpace * 3 + rcJetton.Width() *3,nYPos+rcJetton.Height()-5,0,0,uFlags|SWP_NOSIZE);

	//��ׯ��ť
	DeferWindowPos(hDwp,m_btApplyBanker,NULL,nWidth/2+277-80,nHeight/2-340+74,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btCancelBanker,NULL,nWidth/2+277-80,nHeight/2-340+74,0,0,uFlags|SWP_NOSIZE);

	DeferWindowPos(hDwp,m_btScoreMoveL,NULL,LifeWidth+58+23+4,TopHeight+594+9,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btScoreMoveR,NULL,LifeWidth+58+28+403+2,TopHeight+594+9,0,0,uFlags|SWP_NOSIZE);

	//���ư�ť
	DeferWindowPos(hDwp,m_btAutoOpenCard,NULL,LifeWidth+624+20,TopHeight+198,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btOpenCard,NULL,LifeWidth+624+20,TopHeight+198+30,0,0,uFlags|SWP_NOSIZE);

	//������ť
	DeferWindowPos(hDwp,m_btBank,NULL,nWidth/2+290,nHeight/2-340+74,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btContinueCard,NULL,LifeWidth+624+20,TopHeight+198-30,0,0,uFlags|SWP_NOSIZE);

	//���а�ť
#ifdef __BANKER___
	m_btBankerDraw.GetWindowRect(&rcJetton);
	DeferWindowPos(hDwp,m_btBankerStorage,NULL,nWidth/2+313,nHeight/2+228,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btBankerDraw,NULL,nWidth/2+313,nHeight/2+228+53,0,0,uFlags|SWP_NOSIZE);
#endif
	
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

	//��ȡ״̬
	BYTE cbGameStatus=m_pGameClientDlg->GetGameStatus();

	//״̬��ʾ
	CFont static InfoFont;
	InfoFont.CreateFont(-16,0,0,0,400,0,0,0,134,3,2,ANTIALIASED_QUALITY,2,TEXT("����"));
	CFont * pOldFont=pDC->SelectObject(&InfoFont);
	pDC->SetTextColor(RGB(255,234,0));
	
	pDC->SelectObject(pOldFont);
	InfoFont.DeleteObject();

	//ʱ����ʾ
	CImageHandle ImageHandleTimeFlag(&m_ImageTimeFlag);
	int nTimeFlagWidth = m_ImageTimeFlag.GetWidth()/3;
	int nFlagIndex=0;
	if (cbGameStatus==GS_FREE) nFlagIndex=0;
	else if (cbGameStatus==GS_PLACE_JETTON) nFlagIndex=1;
	else if (cbGameStatus==GS_GAME_END) nFlagIndex=2;
	m_ImageTimeFlag.BitBlt(pDC->GetSafeHdc(), nWidth/2-348+49, nHeight/2+225-372, nTimeFlagWidth, m_ImageTimeFlag.GetHeight(),
		nFlagIndex * nTimeFlagWidth, 0);

	//Dlg�����SetGameTimer(GetMeChairID()��������õ�SwitchViewChairID��������������8ʱʵ����ViewIDת��Ϊ0��
	WORD wUserTimer=GetUserTimer(0);
	if (wUserTimer!=0) DrawUserTimer(pDC,nWidth/2-323+49,nHeight/2+197-372,wUserTimer);

	//ʤ���߿�
	FlashJettonAreaFrame(nWidth,nHeight,pDC);

	//������Դ
	CImageHandle HandleJettonView(&m_ImageJettonView);
	CSize static SizeJettonItem(m_ImageJettonView.GetWidth()/9,m_ImageJettonView.GetHeight());

	//�滭����
	for (INT i=0;i<AREA_COUNT;i++)
	{
		//��������
		LONGLONG lScoreCount=0L;
		LONGLONG static lScoreJetton[JETTON_COUNT]={100L,1000L,10000L,50000L,100000L,500000L,1000000L,5000000L};
		int static nJettonViewIndex=0;

		//�滭����
		for (INT_PTR j=0;j<m_JettonInfoArray[i].GetCount();j++)
		{
			//��ȡ��Ϣ
			tagJettonInfo * pJettonInfo=&m_JettonInfoArray[i][j];

			//�ۼ�����
			ASSERT(pJettonInfo->cbJettonIndex<JETTON_COUNT);
			lScoreCount+=lScoreJetton[pJettonInfo->cbJettonIndex];

			//ͼƬ����
			if (1==pJettonInfo->cbJettonIndex || 2==pJettonInfo->cbJettonIndex) nJettonViewIndex=pJettonInfo->cbJettonIndex+1;
			else if (0==pJettonInfo->cbJettonIndex) nJettonViewIndex=pJettonInfo->cbJettonIndex;
			else nJettonViewIndex=pJettonInfo->cbJettonIndex+1;

			//�滭����
			m_ImageJettonView.AlphaDrawImage(pDC,pJettonInfo->nXPos+m_PointJetton[i].x,
				pJettonInfo->nYPos+m_PointJetton[i].y,SizeJettonItem.cx,SizeJettonItem.cy,
				nJettonViewIndex*SizeJettonItem.cx,0,RGB(255,0,255));
		}

		//�滭����
		if (lScoreCount>0L)	DrawNumberString(pDC,lScoreCount,m_PointJettonNumber[i].x,m_PointJettonNumber[i].y);
	}

	//�滭ׯ��
	DrawBankerInfo(pDC,nWidth,nHeight);

	//�滭�û�
	DrawMeInfo(pDC,nWidth,nHeight);

	//�л�ׯ��
	if ( m_bShowChangeBanker )
	{
		int	nXPos = nWidth / 2 - 130;
		int	nYPos = nHeight / 2 - 160;

		//������ׯ
		if ( m_wMeChairID == m_wBankerUser )
		{
			CImageHandle ImageHandleBanker(&m_ImageMeBanker);
			m_ImageMeBanker.BitBlt(pDC->GetSafeHdc(), nXPos, nYPos);
		}
		else if ( m_wBankerUser != INVALID_CHAIR )
		{
			CImageHandle ImageHandleBanker(&m_ImageChangeBanker);
			m_ImageChangeBanker.BitBlt(pDC->GetSafeHdc(), nXPos, nYPos);
		}
		else
		{
			CImageHandle ImageHandleBanker(&m_ImageNoBanker);
			m_ImageNoBanker.BitBlt(pDC->GetSafeHdc(), nXPos, nYPos);
		}
	}

    //������ʾ
	if (enDispatchCardTip_NULL!=m_enDispatchCardTip)
	{
		if (m_ImageDispatchCardTip.IsNull()==false) m_ImageDispatchCardTip.Destroy();
		if (enDispatchCardTip_Dispatch==m_enDispatchCardTip)
			m_ImageDispatchCardTip.SetLoadInfo(IDB_DISPATCH_CARD,AfxGetInstanceHandle(),false);
		else
			m_ImageDispatchCardTip.SetLoadInfo(IDB_CONTINUE_CARD,AfxGetInstanceHandle(),false);
		CImageHandle ImageHandle(&m_ImageDispatchCardTip);
		m_ImageDispatchCardTip.BitBlt(pDC->GetSafeHdc(), (nWidth-m_ImageDispatchCardTip.GetWidth())/2, nHeight/2);
	}

	//�ҵ���ע
	DrawMeJettonNumber(pDC);

	/*test*/
	if (0)
	{
		//�滭�˿�
		for (int i=0; i<CountArray(m_CardControl); ++i) m_CardControl[i].DrawCardControl(pDC);
	}

	//ʤ����־
	DrawWinFlags(pDC);

	/*if(1)
	{
		m_bShowGameResult= 1;
		DrawType(pDC,0);
	}*/
	if(m_blMoveFinish)
	{
		//��ʾ���
		DrawType(pDC,0);
		
	}

	//����״̬
	if (cbGameStatus==GS_GAME_END)
	{
		//�滭�˿�
		for (int i=0; i<CountArray(m_CardControl); ++i) m_CardControl[i].DrawCardControl(pDC);

	}
	if(m_blMoveFinish)
	{

		ShowGameResult(pDC, nWidth, nHeight);
	}

	

	

	return;
}

//������Ϣ
void CGameClientView::SetMeMaxScore(LONGLONG lMeMaxScore)
{
	if (m_lMeMaxScore!=lMeMaxScore)
	{
		//���ñ���
		m_lMeMaxScore=lMeMaxScore;
	}

	return;
}

//�����ע
void CGameClientView::SetAreaLimitScore(LONGLONG lAreaLimitScore)
{
	if ( m_lAreaLimitScore!= lAreaLimitScore )
	{
		//���ñ���
		m_lAreaLimitScore=lAreaLimitScore;
	}
}

//���ó���
void CGameClientView::SetCurrentJetton(LONGLONG lCurrentJetton)
{
	//���ñ���
	ASSERT(lCurrentJetton>=0L);
	m_lCurrentJetton=lCurrentJetton;

	return;
}

//��ʷ��¼
void CGameClientView::SetGameHistory(bool bWinTian, bool bWinDi, bool bWinXuan,bool bWinHuang)
{
	//��������
	tagClientGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	GameRecord.bWinTianMen=bWinTian;
	GameRecord.bWinDiMen =bWinDi;
	GameRecord.bWinXuanMen=bWinXuan;
	GameRecord.bWinHuangMen=bWinHuang;

	//��������
	if (0==m_lUserJettonScore[ID_TIAN_MEN]) GameRecord.enOperateTianMen=enOperateResult_NULL;
	else if (m_lUserJettonScore[ID_TIAN_MEN] > 0 && true==bWinTian) GameRecord.enOperateTianMen=enOperateResult_Win;
	else if (m_lUserJettonScore[ID_TIAN_MEN] > 0 && false==bWinTian) GameRecord.enOperateTianMen=enOperateResult_Lost;

	if (0==m_lUserJettonScore[ID_DI_MEN]) GameRecord.enOperateDiMen=enOperateResult_NULL;
	else if (m_lUserJettonScore[ID_DI_MEN] > 0 && true==bWinDi) GameRecord.enOperateDiMen=enOperateResult_Win;
	else if (m_lUserJettonScore[ID_DI_MEN] >0 && false==bWinDi) GameRecord.enOperateDiMen=enOperateResult_Lost;

	if (0==m_lUserJettonScore[ID_XUAN_MEN]) GameRecord.enOperateXuanMen=enOperateResult_NULL;
	else if (m_lUserJettonScore[ID_XUAN_MEN] > 0 && true==bWinXuan) GameRecord.enOperateXuanMen=enOperateResult_Win;
	else if (m_lUserJettonScore[ID_XUAN_MEN] >0 && false==bWinXuan) GameRecord.enOperateXuanMen=enOperateResult_Lost;


	if (0==m_lUserJettonScore[ID_HUANG_MEN]) GameRecord.enOperateHuangMen=enOperateResult_NULL;
	else if (m_lUserJettonScore[ID_HUANG_MEN] > 0 && true==bWinHuang) GameRecord.enOperateHuangMen=enOperateResult_Win;
	else if (m_lUserJettonScore[ID_HUANG_MEN] >0 && false==bWinHuang) GameRecord.enOperateHuangMen=enOperateResult_Lost;

	//�ƶ��±�
	m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst )
	{
		m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;
		if ( m_nScoreHead < m_nRecordFirst ) m_nScoreHead = m_nRecordFirst;
	}

	int nHistoryCount = (m_nRecordLast - m_nRecordFirst + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
	if ( MAX_FALG_COUNT < nHistoryCount ) m_btScoreMoveR.EnableWindow(TRUE);

	//�Ƶ����¼�¼
	if ( MAX_FALG_COUNT < nHistoryCount )
	{
		m_nScoreHead = (m_nRecordLast - MAX_FALG_COUNT + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
		m_btScoreMoveL.EnableWindow(TRUE);
		m_btScoreMoveR.EnableWindow(FALSE);
	}

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

	//��ע��Ϣ
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//ȫ����ע
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	//���½���
	UpdateGameView(NULL);

	return;
}

//������ע
void CGameClientView::SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
{
	//Ч�����
	ASSERT(cbViewIndex<=ID_HUANG_MEN);
	if (cbViewIndex>ID_HUANG_MEN) return;

	m_lUserJettonScore[cbViewIndex]=lJettonCount;

	//���½���
	UpdateGameView(NULL);
}

//�����˿�
void CGameClientView::SetCardInfo(BYTE cbTableCardArray[5][5])
{
	if (cbTableCardArray!=NULL)
	{
		CopyMemory(m_cbTableCardArray,cbTableCardArray,sizeof(m_cbTableCardArray));

		//��ʼ����
		DispatchCard();
	}
	else
	{
		ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));
	}
}

//���ó���
void CGameClientView::PlaceUserJetton(BYTE cbViewIndex, LONGLONG lScoreCount)
{
	//Ч�����
	ASSERT(cbViewIndex<=ID_HUANG_MEN);
	if (cbViewIndex>ID_HUANG_MEN) return;

	//��������
	bool bPlaceJetton=false;
	LONGLONG lScoreIndex[JETTON_COUNT]={100L,1000L,10000L,50000L,100000L,500000L,1000000L,5000000L};

	//�߿���
	int nFrameWidth=0, nFrameHeight=0;
	int nBorderWidth=6;

	switch (cbViewIndex)
	{
	case ID_TIAN_MEN:
		{ 
			m_lAllJettonScore[ID_TIAN_MEN] += lScoreCount;
			nFrameWidth = m_rcTianMen.right-m_rcTianMen.left;
			nFrameHeight = m_rcTianMen.bottom-m_rcTianMen.top;
			break;
		}
	case ID_DI_MEN:
		{
			m_lAllJettonScore[ID_DI_MEN] += lScoreCount;
			nFrameWidth = m_rcDimen.right-m_rcDimen.left;
			nFrameHeight = m_rcDimen.bottom-m_rcDimen.top;
			break;
		}
	case ID_HUANG_MEN :
		{ 
			m_lAllJettonScore[ID_HUANG_MEN] += lScoreCount;
			nFrameWidth = m_rcHuangMen.right-m_rcHuangMen.left;
			nFrameHeight = m_rcHuangMen.bottom-m_rcHuangMen.top;
			break;
		}
	case ID_XUAN_MEN:
		{ 
			m_lAllJettonScore[ID_XUAN_MEN] += lScoreCount;
			nFrameWidth = m_rcXuanMen.right-m_rcXuanMen.left;
			nFrameHeight = m_rcXuanMen.bottom-m_rcXuanMen.top;
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

	//�����ж�
	bool bAddJetton=lScoreCount>0?true:false;
	lScoreCount= (lScoreCount>0?lScoreCount:-lScoreCount);

	//���ӳ���
	for (BYTE i=0;i<CountArray(lScoreIndex);i++)
	{
		//������Ŀ
		BYTE cbScoreIndex=JETTON_COUNT-i-1;
		LONGLONG lCellCount=lScoreCount/lScoreIndex[cbScoreIndex];

		//�������
		if (lCellCount==0L) continue;

		//�������
		for (LONGLONG j=0;j<lCellCount;j++)
		{
			if (true==bAddJetton)
			{
				//�������
				tagJettonInfo JettonInfo;
				int nJettonSize=46;
				JettonInfo.cbJettonIndex=cbScoreIndex;
				JettonInfo.nXPos=rand()%(nFrameWidth-nJettonSize-20)+6;
				JettonInfo.nYPos=rand()%(nFrameHeight-nJettonSize-23-14)+6;

				//��������
				bPlaceJetton=true;
				m_JettonInfoArray[cbViewIndex-1].Add(JettonInfo);
			}
			else
			{
				for (int nIndex=0; nIndex<m_JettonInfoArray[cbViewIndex-1].GetCount(); ++nIndex)
				{
					//�Ƴ��ж�
					tagJettonInfo &JettonInfo=m_JettonInfoArray[cbViewIndex-1][nIndex];
					if (JettonInfo.cbJettonIndex==cbScoreIndex)
					{
						m_JettonInfoArray[cbViewIndex-1].RemoveAt(nIndex);
						break;
					}
				}
			}
		}

		//������Ŀ
		lScoreCount-=lCellCount*lScoreIndex[cbScoreIndex];
	}

	//���½���
	if (bPlaceJetton==true) UpdateGameView(NULL);

	return;
}

//���ֳɼ�
void CGameClientView::SetCurGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore, LONGLONG lGameRevenue)
{
	m_lMeCurGameScore=lMeCurGameScore;			
	m_lMeCurGameReturnScore=lMeCurGameReturnScore;			
	m_lBankerCurGameScore=lBankerCurGameScore;			
	m_lGameRevenue=lGameRevenue;					
}

//����ʤ��
void CGameClientView::SetWinnerSide(bool bWinTian, bool bWinDi, bool bWinXuan,bool bWinHuang, bool bSet)
{	
	//����ʱ��
	if (true==bSet)
	{
		//���ö�ʱ��
		SetTimer(IDI_FLASH_WINNER,500,NULL);

		//ȫʤ�ж�
		if (false==bWinTian && false==bWinDi && false==bWinXuan &&false ==bWinHuang )
		{
			//�����Դ
			m_ImageFrameTianMen.Destroy();
			m_ImageFrameDiMen.Destroy();
			m_ImageFrameHuangMen.Destroy();
			m_ImageFrameXuanMen.Destroy();
			//������Դ
			HINSTANCE hInstance=AfxGetInstanceHandle();
			m_ImageFrameTianMen.SetLoadInfo(IDB_FRAME_TIAN_EMN, hInstance);
			m_ImageFrameDiMen.SetLoadInfo(IDB_FRAME_DI_EMN, hInstance);
			m_ImageFrameHuangMen.SetLoadInfo(IDB_FRAME_HUANG_EMN, hInstance);
			m_ImageFrameXuanMen.SetLoadInfo(IDB_FRAME_XUAN_EMN, hInstance);

		}
	}
	else 
	{
		//�����ʱ��
		KillTimer(IDI_FLASH_WINNER);

		//ȫʤ�ж�
		if (false==m_bWinXuanMen && false==m_bWinTianMen && false==m_bWinHuangMen)
		{
			//�����Դ
			m_ImageFrameTianMen.Destroy();
			m_ImageFrameDiMen.Destroy();
			m_ImageFrameHuangMen.Destroy();
			m_ImageFrameXuanMen.Destroy();


			//������Դ
			HINSTANCE hInstance=AfxGetInstanceHandle();
			m_ImageFrameTianMen.SetLoadInfo(IDB_FRAME_TIAN_MEN_EX, hInstance);
			m_ImageFrameDiMen.SetLoadInfo(IDB_FRAME_DI_MEN_EX, hInstance);
			m_ImageFrameHuangMen.SetLoadInfo(IDB_FRAME_HUANG_MEN_EX, hInstance);
			m_ImageFrameXuanMen.SetLoadInfo(IDB_FRAME_XUAN_MEN_EX, hInstance);
		}
	}

	//���ñ���
	m_bWinTianMen=bWinTian;
	m_bWinHuangMen=bWinHuang;
	m_bWinXuanMen=bWinXuan;
	m_bWinDiMen=bWinDi;
	m_bFlashResult=bSet;
	m_bShowGameResult=bSet;
	m_cbAreaFlash=0xFF;

	//���½���
	UpdateGameView(NULL);

	return;
}

//��ȡ����
BYTE CGameClientView::GetJettonArea(CPoint MousePoint)
{
	if (m_rcTianMen.PtInRect(MousePoint))
	{
		return ID_TIAN_MEN;

	}else
	{
		if (m_rcDimen.PtInRect(MousePoint))
		{
			return ID_DI_MEN;

		}

	}
	if (m_rcHuangMen.PtInRect(MousePoint)) return ID_HUANG_MEN;	
	if (m_rcXuanMen.PtInRect(MousePoint)) return ID_XUAN_MEN ;	
	return 0xFF;
}
void CGameClientView::DrawType(CDC* pDC,WORD wChairID)
{
	if (false==m_bShowGameResult) return;

	CImageHandle HandleImageCardType[5];
	for (int i = 0;i<5;i++)	
	{
		int iIndex = 0;
		//iIndex = CT_SPECIAL_NIUNIUXW-2;
		//m_lUserCardType[IDC_BANK]
		if(m_lUserCardType[i]!= CT_POINT)
		{
			iIndex = m_lUserCardType[i]-2;
			if(iIndex>=13)
			{
				if(iIndex == 13)
				{
					iIndex = 12;
					//break;
				}
				if(iIndex == 14)
				{
					iIndex = 11;
					//break;
				}
				if(iIndex == 15)
				{
					iIndex = 13;
					//	break;
				}
			}
			else
			{
				if(iIndex>10)
				{
					iIndex = 10;
				}
				if(iIndex<0)
				{
					iIndex = 0;
				}
			}
			//iIndex = 11;
		}
		
		CImageHandle HandleImageCardType(&m_ImageCardType[i]);
		//HandleImageCardType[i](&m_ImageCardType[i]);
		m_ImageCardType[i].AlphaDrawImage(pDC,m_CardTypePoint[i].x,m_CardTypePoint[i].y,m_ImageCardType[i].GetWidth()/14,m_ImageCardType[i].GetHeight(),m_ImageCardType[i].GetWidth()/14*iIndex,0,RGB(255,0,255));
	}
}

//�滭����
void CGameClientView::DrawNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos, bool bMeScore)
{
	//������Դ
	CImageHandle HandleScoreNumber(&m_ImageScoreNumber);
	CImageHandle HandleMeScoreNumber(&m_ImageMeScoreNumber);
	CSize SizeScoreNumber(m_ImageScoreNumber.GetWidth()/10,m_ImageScoreNumber.GetHeight());

	if ( bMeScore ) SizeScoreNumber.SetSize(m_ImageMeScoreNumber.GetWidth()/10, m_ImageMeScoreNumber.GetHeight());

	//������Ŀ
	int lNumberCount=0;
	LONGLONG lNumberTemp=lNumber;
	do
	{
		lNumberCount++;
		lNumberTemp/=10;
	} while (lNumberTemp>0);

	//λ�ö���
	int nYDrawPos=nYPos-SizeScoreNumber.cy/2;
	int nXDrawPos=nXPos+lNumberCount*SizeScoreNumber.cx/2-SizeScoreNumber.cx;

	//�滭����
	for (int i=0;i<lNumberCount;i++)
	{
		//�滭����
		int lCellNumber=(int)(lNumber%10);
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
void CGameClientView::DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos)
{
	CString strNumber, strTempNum;
	strTempNum.Format(_T("%I64d"), (lNumber>0?lNumber:-lNumber));

	int nLength = strTempNum.GetLength();
	if (nLength <= 3)
		strNumber.Format(_T("%I64d"), lNumber);
	else
	{
		for (int i = 0; i < int((nLength-1)/3); i++)
			strTempNum.Insert(nLength - 3*(i+1), _T(","));
		strNumber.Format(_T("%s%s"), (lNumber<0?_T("-"):_T("")), strTempNum);
	}

	//�������
	pDC->TextOut(nXPos,nYPos,strNumber);
}

//�滭����
void CGameClientView::DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, CRect rcRect, INT nFormat)
{
	CString strNumber, strTempNum;
	strTempNum.Format(_T("%I64d"), (lNumber>0?lNumber:-lNumber));

	int nLength = strTempNum.GetLength();
	if (nLength <= 3)
		strNumber.Format(_T("%I64d"), lNumber);
	else
	{
		for (int i = 0; i < int((nLength-1)/3); i++)
			strTempNum.Insert(nLength - 3*(i+1), _T(","));
		strNumber.Format(_T("%s%s"), (lNumber<0?_T("-"):_T("")), strTempNum);
	}

	//�������
	if (nFormat==-1) pDC->DrawText(strNumber,rcRect,DT_END_ELLIPSIS|DT_LEFT|DT_TOP|DT_SINGLELINE);
	else pDC->DrawText(strNumber,rcRect,nFormat);
}
void CGameClientView::KillCardTime()
{
	KillTimer(IDI_FLASH_WINNER);
	KillTimer(IDI_POSTCARD);
	KillTimer(IDI_OPENCARD);
	KillTimer(IDI_DISPATCH_CARD);
	KillTimer(DISPATCH_CARD_TIP);
	for (int i = 0;i<5;i++)
	{
	  m_CardControl[i].m_blShowResult = false;
	  m_CardControl[i].m_blShowLineResult = false;
	}
}
//��ʱ����Ϣ
void CGameClientView::OnTimer(UINT nIDEvent)
{
	//����ʤ��
	if (nIDEvent==IDI_FLASH_WINNER)
	{
		//���ñ���
		m_bFlashResult=!m_bFlashResult;

		//���½���
		UpdateGameView(NULL);

		return;
	}
	if(IDI_POSTCARD ==nIDEvent)
	{

		if(m_bcShowCount<5)
		{
			if(m_CardControl[5].m_CardTempItemArray.GetCount()>0)
			{
				m_CardControl[5].m_blhideOneCard = false;
				m_CardControl[5].m_blGameEnd = true;
				m_CardControl[5].SetDisplayFlag(true);
				m_CardControl[5].ShowCardControl(true);
				m_CardControl[5].m_CardItemArray.SetSize(1);
				m_CardControl[5].m_CardItemArray[0]=m_CardControl[5].m_CardTempItemArray[0];
				m_CardControl[5].m_CardItemArray[0].cbCardData = m_bcfirstShowCard;
				m_CardControl[5].m_CardItemArray[0].bShoot = false;
			}
		
		}else
		{
			m_CardControl[5].m_CardItemArray.SetSize(0);

			bool blKillTimer = true;
			for (int i = 0;i<5;i++)
			{
				if(m_CardControl[i].m_CardItemArray.GetCount()!=m_CardControl[i].m_CardTempItemArray.GetCount())
				{
					blKillTimer = false;
					break;
				}
			}
			m_PostCardIndex = m_PostCardIndex%5;
			m_CardControl[m_PostCardIndex].OnCopyCard();
			m_PostCardIndex++;
			if(blKillTimer)
			{
					
				KillTimer(IDI_POSTCARD);
				SetTimer(IDI_OPENCARD,25,NULL);
			}
			UpdateGameView(NULL);

		}
		m_bcShowCount++;
	
	
	}
	if(IDI_OPENCARD==nIDEvent)
	{
		if(m_blAutoOpenCard == false)
		{
			while (1)
			{
				bool blCopy = false;
				for (int i = 0;i<5;i++)
				{
					if(m_CardControl[i].m_CardItemArray.GetCount()!=5)
					{
						blCopy = true;
						break;
					}
					if(m_CardControl[i].m_CardTempItemArray.GetCount()==0)
					{
						blCopy = false;
						break;
					}
				}
				if(false == blCopy)
				{
					break;
				}else
				{
					m_CardControl[i].OnCopyCard();
				}
			}

			for (int i = 0;i<5;i++)
			{
				m_CardControl[i].m_blGameEnd = false;
				m_CardControl[i].m_blhideOneCard = false;
			}
			//���½���
			UpdateGameView(NULL);
			KillTimer(IDI_OPENCARD);

			SetTimer(IDI_DISPATCH_CARD,10000,NULL);

			return ;

		}
		{
			if(m_CardControl[m_OpenCardIndex%5].m_MovePoint.x<40&&m_CardControl[m_OpenCardIndex%5].m_blAdd)
			{
				m_CardControl[m_OpenCardIndex%5].m_blGameEnd = false;
				m_CardControl[m_OpenCardIndex%5].m_MovePoint.x+=2;
				m_CardControl[m_OpenCardIndex%5].m_blhideOneCard = false;
				//���½���
				UpdateGameView(NULL);


			}else
			{

				m_CardControl[m_OpenCardIndex%5].m_blAdd = false;
				m_CardControl[m_OpenCardIndex%5].m_blGameEnd = false;
				if(m_CardControl[m_OpenCardIndex%5].m_MovePoint.x>10)
				m_CardControl[m_OpenCardIndex%5].m_MovePoint.x-=2;
				m_CardControl[m_OpenCardIndex%5].m_blhideOneCard = false;
				UpdateGameView(NULL);
				if(m_CardControl[m_OpenCardIndex%5].m_MovePoint.x<=10)
				{
					m_CardControl[m_OpenCardIndex%5].m_tMoveCard.m_blMoveCard = true;
					m_CardControl[m_OpenCardIndex%5].m_tMoveCard.m_blMoveFinish = true;
					m_CardControl[m_OpenCardIndex%5].m_MovePoint.x = 0;
					m_CardControl[m_OpenCardIndex%5].m_MovePoint.y = 0;
					m_CardControl[m_OpenCardIndex%5].m_blAdd = true;
					m_CardControl[m_OpenCardIndex%5].m_blGameEnd = true;

					if(m_lUserCardType[m_OpenCardIndex%5]==CT_POINT&&m_lUserCardType[m_OpenCardIndex%5]==CT_SPECIAL_BOMEBOME)
					{
						m_CardControl[m_OpenCardIndex%5].m_blShowLineResult = true;
					}else
					{
						m_CardControl[m_OpenCardIndex%5].m_blShowLineResult = false;

					}
					//BYTE bcTmp[5];
					//BYTE bcCardDataTmp[5];
					//m_CardControl[m_OpenCardIndex%5].GetCardData(bcCardDataTmp,5);
				/*	int iType = m_GameLogic.GetCardType((const BYTE*)bcCardDataTmp,5,bcTmp);
					
					BYTE bcResult[5];
					if(iType==CT_POINT)
					{
						m_CardControl[m_OpenCardIndex%5].m_blShowLineResult = true;
						CopyMemory(bcResult,bcTmp,5);
					}else
					{
						CopyMemory(bcResult,bcTmp+3,2);
						CopyMemory(bcResult+2,bcTmp,3);

					}*/
					
					m_CardControl[m_OpenCardIndex%5].SetCardData(m_cbTableSortCardArray[m_OpenCardIndex%5],5,false);
					m_CardControl[m_OpenCardIndex%5].m_blShowResult = true;
					m_OpenCardIndex++;

					UpdateGameView(NULL);

					if(m_OpenCardIndex==6)
					{
						/*for (int i = 0;i<5;i++)
						{
							m_CardControl[i].m_blGameEnd = true;
							BYTE bcTmp[5];
							BYTE bcCardDataTmp[5];
							m_CardControl[i].GetCardData(bcCardDataTmp,5);
							m_GameLogic.GetCardType((const BYTE*)bcCardDataTmp,5,bcTmp);
							m_CardControl[i].SetCardData(bcTmp,5,false);
						}*/
						KillTimer(IDI_OPENCARD);
						UpdateGameView(NULL);

						//������ʾ�ƶ�ʱ��
						SetTimer(IDI_DISPATCH_CARD,30,NULL);
					}

				}

			}
		}
	}
	//�ֻ�ׯ��
	else if ( nIDEvent == IDI_SHOW_CHANGE_BANKER )
	{
		ShowChangeBanker( false );

		return;
	}
	else if (nIDEvent==IDI_DISPATCH_CARD)
	{

		//��ɷ���
		FinishDispatchCard(true);
		
	/*	while (1)
		{
			bool blCopy = false;
			for (int i = 0;i<5;i++)
			{
				if(m_CardControl[i].m_CardItemArray.GetCount()!=5)
				{
					blCopy = true;
					break;
				}
				if(m_CardControl[i].m_CardTempItemArray.GetCount()==0)
				{
					blCopy = false;
					break;
				}
			}
			if(false == blCopy)
			{
				break;
			}else
			{
				m_CardControl[i].OnCopyCard();
			}
		}*/

		{
			for (int i = 0;i<5;i++)
			{
				m_CardControl[i].m_blGameEnd = true;
				m_CardControl[i].m_blhideOneCard = false;

				if(m_lUserCardType[i]==CT_POINT||m_lUserCardType[i]==CT_SPECIAL_BOMEBOME)
				{
					m_CardControl[i].m_blShowLineResult = true;
				}else
				{
					m_CardControl[i].m_blShowLineResult = false;

				}
			//	if (m_blAutoOpenCard==false)
				{
				//	BYTE bcTmp[5];
					//BYTE bcCardDataTmp[5];
				/*	m_CardControl[i].GetCardData(bcCardDataTmp,5);
					int iType = m_GameLogic.GetCardType((const BYTE*)bcCardDataTmp,5,bcTmp);
					BYTE bcResult[5];

					if(iType==CT_POINT)
					{
						m_CardControl[i].m_blShowLineResult = true;
						CopyMemory(bcResult,bcTmp,5);
					}else
					{
						CopyMemory(bcResult,bcTmp+3,2);
						CopyMemory(bcResult+2,bcTmp,3);
					}*/
					m_CardControl[i].SetCardData(m_cbTableSortCardArray[i],5,false);
					//m_CardControl[i].SetCardData(bcResult,5,false);
					m_CardControl[i].m_blShowResult = true;
				}
			}
			KillTimer(IDI_OPENCARD);
			UpdateGameView(NULL);

		}
		m_blMoveFinish = true;
		//���½���
		UpdateGameView(NULL);

		return;
	}
	else if (DISPATCH_CARD_TIP==nIDEvent)
	{
		SetDispatchCardTip(enDispatchCardTip_NULL);

	}else if(nIDEvent==IDI_MOVECARD_END)
	{
		KillTimer(IDI_MOVECARD_END);
	}

	__super::OnTimer(nIDEvent);
}

//�����Ϣ
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{	//JJWillDo ɾ������
	/*int static a = 1;
	tagApplyUser ta;
	ta.strUserName.Format(_T("a%d"), a);
	m_ApplyUser.DeleteUser(ta);
	a++;*/

	for (int i = 0;i<5;i++)
	{
		if(!m_CardControl[i].m_blAuto)
		{
			m_CardControl[i].OnLButtonDown(nFlags, Point);
		}
	}
		
	if (m_lCurrentJetton!=0L)
	{
		int iTimer = 1;
		//��ע����
		BYTE cbJettonArea=GetJettonArea(Point);
	
		//�����ע
		LONGLONG lMaxJettonScore=GetUserMaxJetton();

		if((m_lAllJettonScore[cbJettonArea]+m_lCurrentJetton)>m_lAreaLimitScore)
		{		
			return ;
		}
		//�Ϸ��ж�
		//if (lMaxJettonScore < m_lCurrentJetton*iTimer) return ;

		//������Ϣ
		if (cbJettonArea!=0xFF) AfxGetMainWnd()->SendMessage(IDM_PLACE_JETTON,cbJettonArea,0);
	}
	UpdateGameView(NULL);

	__super::OnLButtonDown(nFlags,Point);
}
void CGameClientView::OnLButtonUp(UINT nFlags, CPoint Point)
{
	
	for (int i = 0;i<5;i++)
	{
		if(!m_CardControl[i].m_blAuto)
		{
			m_CardControl[i].OnLButtonUp(nFlags, Point);
			UpdateGameView(NULL);
		}
		
	}
	__super::OnLButtonUp(nFlags,Point);
}
//���������ؼ���������Ϣ
LRESULT CGameClientView::OnViLBtUp(WPARAM wParam, LPARAM lParam)
{
	CPoint *pPoint = (CPoint*)lParam;
	ScreenToClient(pPoint);
    OnLButtonUp(1,*pPoint);
	return 1;

}
//����ƶ���Ϣ
void CGameClientView::OnMouseMove(UINT nFlags, CPoint point)
{
	for (int i = 0;i<5;i++)
	{
		if(!m_CardControl[i].m_blAuto)
		{
			m_CardControl[i].OnMouseMove(nFlags, point);
			UpdateGameView(NULL);

		}

	}
	__super::OnMouseMove(nFlags,point);
}
//�����Ϣ
void CGameClientView::OnRButtonDown(UINT nFlags, CPoint Point)
{
	//���ñ���
	m_lCurrentJetton=0L;

	if (m_pGameClientDlg->GetGameStatus()!=GS_GAME_END && m_cbAreaFlash!=0xFF)
	{
		m_cbAreaFlash=0xFF;
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
		BYTE cbJettonArea=GetJettonArea(MousePoint);

		//���ñ���
		if ( m_cbAreaFlash!= cbJettonArea )
		{
			m_cbAreaFlash = cbJettonArea;
			UpdateGameView(NULL);
		}

		//�����ж�
		if (cbJettonArea==0xFF)
		{
			SetCursor(LoadCursor(NULL,IDC_ARROW));
			return TRUE;
		}

		//�����ע
		LONGLONG lMaxJettonScore=GetUserMaxJetton();

		//�Ϸ��ж�
		int iTimer = 1;
		if((m_lAllJettonScore[cbJettonArea]+m_lCurrentJetton)>m_lAreaLimitScore)
		{
			LONGLONG lScoreIndex[JETTON_COUNT]={100L,1000L,10000L,50000L,100000L,500000L,1000000L,5000000L};
			CButton *pButton[JETTON_COUNT]={&m_btJetton100,&m_btJetton1000,&m_btJetton10000,&m_btJetton50000,&m_btJetton100000,&m_btJetton500000,&m_btJetton1000000,&m_btJetton5000000};
			for (int i = 0; i < JETTON_COUNT; i++)
			{
				if (lMaxJettonScore >= lScoreIndex[i] && pButton[i]->IsWindowEnabled())
					m_lCurrentJetton = lScoreIndex[i];
			}

			if (lMaxJettonScore < 100)
			{
				SetCursor(LoadCursor(NULL,IDC_NO));
				return TRUE;	
			}
		}
	
		//���ù��
		switch (m_lCurrentJetton)
		{
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
		case 50000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_50000)));
				return TRUE;
			}
		case 100000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_100000)));
				return TRUE;
			}
		case 500000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_500000)));
				return TRUE;
			}
		case 1000000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_1000000)));
				return TRUE;
			}
		case 5000000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_5000000)));
				return TRUE;
			}		
		}
	}
	else
	{
		ClearAreaFlash();
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
void CGameClientView::SetBankerInfo(DWORD dwBankerUserID, LONGLONG lBankerScore) 
{
	//ׯ�����Ӻ�
	WORD wBankerUser=INVALID_CHAIR;

	//�������Ӻ�
	if (0!=dwBankerUserID)
	{
		for (WORD wChairID=0; wChairID<MAX_CHAIR; ++wChairID)
		{
			tagUserData const *pUserData=GetUserInfo(wChairID);
			if (NULL!=pUserData && dwBankerUserID==pUserData->dwUserID)
			{
				wBankerUser=wChairID;
				break;
			}
		}
	}

	//�л��ж�
	if (m_wBankerUser!=wBankerUser)
	{
		m_wBankerUser=wBankerUser;
		m_wBankerTime=0L;
		m_lBankerWinScore=0L;	
		m_lTmpBankerWinScore=0L;
	}
	m_lBankerScore=lBankerScore;
}

//�滭��ʶ
void CGameClientView::DrawWinFlags(CDC * pDC)
{
	//�ǿ��ж�
	//m_nRecordLast = 14;
	if (m_nRecordLast==m_nRecordFirst) return;

	//��Դ����
	CImageHandle ImageHandleWinFlags(&m_ImageWinFlags);
	int nIndex = m_nScoreHead;
	COLORREF static clrOld ;

	clrOld = pDC->SetTextColor(RGB(255,234,0));
	int nDrawCount=0;
	while ( nIndex != m_nRecordLast && ( m_nRecordLast!=m_nRecordFirst ) && nDrawCount < MAX_FALG_COUNT )
	{
		//ʤ����ʶ
		tagClientGameRecord &ClientGameRecord = m_GameRecordArrary[nIndex];
		bool  static bWinMen[4];
		bWinMen[0]=ClientGameRecord.bWinTianMen;
		bWinMen[1]=ClientGameRecord.bWinDiMen ;
		bWinMen[2]=ClientGameRecord.bWinXuanMen;
		bWinMen[3]=ClientGameRecord.bWinHuangMen;

		//�������
		enOperateResult static OperateResult[4];
		OperateResult[0]=ClientGameRecord.enOperateTianMen;
		OperateResult[1]=ClientGameRecord.enOperateDiMen;
		OperateResult[2]=ClientGameRecord.enOperateXuanMen;
		OperateResult[3]=ClientGameRecord.enOperateHuangMen;

		for (int i=0; i<4; ++i)
		{
			//λ�ñ���
			int static nYPos=0,nXPos=0;
			nYPos=m_nWinFlagsExcursionY+i*27;
			nXPos=m_nWinFlagsExcursionX + ((nIndex - m_nScoreHead + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY) * 27;

			//ʤ����ʶ
			int static nFlagsIndex=0;
			if (false==bWinMen[i])
			{
				nFlagsIndex=2;
			}
			else
			{
				nFlagsIndex=0;
			}
			if (OperateResult[i] != enOperateResult_NULL)
			{			
				nFlagsIndex+=1;

				if(nFlagsIndex>=4)
				{
					nFlagsIndex=3;
				}
			}
			//�滭��ʶ
			m_ImageWinFlags.AlphaDrawImage( pDC, nXPos, nYPos, m_ImageWinFlags.GetWidth()/4 , 
				m_ImageWinFlags.GetHeight(),m_ImageWinFlags.GetWidth()/4 * nFlagsIndex, 0, RGB(255, 0, 255) );
		}

		//�ƶ��±�
		nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
		nDrawCount++;
	}
	pDC->SetTextColor(clrOld);
}
//�ֹ�����
afx_msg void CGameClientView::OnOpenCard()
{
	::AfxGetMainWnd()->SendMessage(IDM_OPEN_CARD,0,0);

}
//�Զ�����
afx_msg void CGameClientView::OnAutoOpenCard()
{
	::AfxGetMainWnd()->SendMessage(IDM_AUTO_OPEN_CARD,0,0);

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
	if ( nHistoryCount == MAX_FALG_COUNT ) return;

	m_nScoreHead = ( m_nScoreHead + 1 ) % MAX_SCORE_HISTORY;
	if ( nHistoryCount-1 == MAX_FALG_COUNT ) m_btScoreMoveR.EnableWindow(FALSE);

	m_btScoreMoveL.EnableWindow(TRUE);

	//���½���
	UpdateGameView(NULL);
}

//��ʾ���
void CGameClientView::ShowGameResult(CDC *pDC, int nWidth, int nHeight)
{
	//��ʾ�ж�
	//if (m_pGameClientDlg->GetGameStatus()!=GS_GAME_END) return;
	if (false==m_bShowGameResult) return;

	int	nXPos = nWidth / 2 - 129;
	int	nYPos = nHeight / 2 - 148;
	CImageHandle ImageHandleGameEnd(&m_ImageGameEnd);

	CRect rcAlpha(nXPos+2, nYPos+70, nXPos+2 + m_ImageGameEnd.GetWidth(), nYPos+70+m_ImageGameEnd.GetHeight());
	DrawAlphaRect(pDC, &rcAlpha, RGB(74,70,73), 0.8f);
	m_ImageGameEnd.AlphaDrawImage(pDC, nXPos+2, nYPos+70, RGB(255,0,255));

	pDC->SetTextColor(RGB(255,234,0));
	CRect rcMeWinScore, rcMeReturnScore;
	rcMeWinScore.left = nXPos+2 + 40;
	rcMeWinScore.top = nYPos+70 + 32;
	rcMeWinScore.right = rcMeWinScore.left + 111;
	rcMeWinScore.bottom = rcMeWinScore.top + 34;

	rcMeReturnScore.left = nXPos+2 + 150;
	rcMeReturnScore.top = nYPos+70 + 32;
	rcMeReturnScore.right = rcMeReturnScore.left + 111;
	rcMeReturnScore.bottom = rcMeReturnScore.top + 34;

	CString strMeGameScore, strMeReturnScore;
	DrawNumberStringWithSpace(pDC,m_lMeCurGameScore,rcMeWinScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	DrawNumberStringWithSpace(pDC,m_lMeCurGameReturnScore,rcMeReturnScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);

	CRect rcBankerWinScore;
	rcBankerWinScore.left = nXPos+2 + 40;
	rcBankerWinScore.top = nYPos+70 + 69;
	rcBankerWinScore.right = rcBankerWinScore.left + 111;
	rcBankerWinScore.bottom = rcBankerWinScore.top + 34;

	CString strBankerCurGameScore;
	strBankerCurGameScore.Format(TEXT("%I64d"), m_lBankerCurGameScore);
	DrawNumberStringWithSpace(pDC,m_lBankerCurGameScore,rcBankerWinScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);
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

//�����ע
LONGLONG CGameClientView::GetUserMaxJetton()
{
	int iTimer = 10;
	//����ע��
	LONGLONG lNowJetton = 0;
	ASSERT(AREA_COUNT<=CountArray(m_lUserJettonScore));
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lNowJetton += m_lUserJettonScore[nAreaIndex]*iTimer;

	//ׯ�ҽ��
	LONGLONG lBankerScore=2147483647;
	if (m_wBankerUser!=INVALID_CHAIR) lBankerScore=m_lBankerScore;
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lBankerScore-=m_lAllJettonScore[nAreaIndex]*iTimer;

	//��������
	LONGLONG lMeMaxScore;
	if((m_lMeMaxScore-lNowJetton)/iTimer>m_lAreaLimitScore)
	{
		lMeMaxScore= m_lAreaLimitScore*iTimer;

	}else
	{
		lMeMaxScore = m_lMeMaxScore-lNowJetton;
		lMeMaxScore = lMeMaxScore;
	}

	//ׯ������
	lMeMaxScore=min(lMeMaxScore,lBankerScore);

	lMeMaxScore /= iTimer; 

	//��������
	//ASSERT(lMeMaxScore >= 0);
	lMeMaxScore = max(lMeMaxScore, 0);

	return lMeMaxScore;
}

//�ɼ�����
void CGameClientView::SetGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore)
{
	m_lMeCurGameScore=lMeCurGameScore;
	m_lMeCurGameReturnScore=lMeCurGameReturnScore;
	m_lBankerCurGameScore=lBankerCurGameScore;	
}

//�滭����
void CGameClientView::DrawMeJettonNumber(CDC *pDC)
{
	//�滭����
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
	{
		if (m_lUserJettonScore[nAreaIndex] > 0 )
			DrawNumberString(pDC,m_lUserJettonScore[nAreaIndex],m_PointJettonNumber[nAreaIndex-1].x,m_PointJettonNumber[nAreaIndex-1].y+25, true);
	}
}

//��ʼ����
void CGameClientView::DispatchCard()
{
	//���ý���
	for (int i=0; i<CountArray(m_CardControl); ++i) m_CardControl[i].m_CardItemArray.SetSize(0);
	//���ñ�ʶ
	m_bNeedSetGameRecord=true;
}

//��������
void CGameClientView::FinishDispatchCard(bool bNotScene)
{
	//����ж�
	if (m_bNeedSetGameRecord==false) return;

	//���ñ�ʶ
	m_bNeedSetGameRecord=false;

	//ɾ����ʱ��
	KillTimer(IDI_DISPATCH_CARD);

	////�����˿�
	//for (int i=0; i<CountArray(m_CardControl); ++i) m_CardControl[i].SetCardData(m_cbTableCardArray[i],2);

	//�ƶ�Ӯ��
	bool bWinTianMen, bWinDiMen, bWinXuanMen,bWinHuang;

	BYTE TianMultiple,diMultiple,TianXuanltiple,HuangMultiple;

	DeduceWinner(bWinTianMen, bWinDiMen, bWinXuanMen,bWinHuang,TianMultiple,diMultiple,TianXuanltiple,HuangMultiple);

	//�����¼
	if (bNotScene)
		SetGameHistory(bWinTianMen, bWinDiMen, bWinXuanMen,bWinHuang);

	//�ۼƻ���
	m_lMeStatisticScore+=m_lMeCurGameScore;
	m_lBankerWinScore=m_lTmpBankerWinScore;

	//����Ӯ��
	SetWinnerSide(bWinTianMen, bWinDiMen, bWinXuanMen, bWinHuang, true);

	//��������
	if (m_lMeCurGameScore>0) m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("END_WIN"));
	else if (m_lMeCurGameScore<0) m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("END_LOST"));
	else m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("END_DRAW"));
}

//��ֹ����
void CGameClientView::StopAnimal()
{
	//��ʱ��
	KillTimer(IDI_POSTCARD);
	KillTimer(IDI_DISPATCH_CARD);
	KillTimer(IDI_FLASH_WINNER);
	KillTimer(IDI_OPENCARD);

	FinishDispatchCard(true);

	for (int i = 0;i<4;i++)
	{
		m_CardControl[i].m_blGameEnd = true;
		m_CardControl[i].m_blhideOneCard = false;
	}
	m_blMoveFinish = true;
	m_bFlashResult = false;
}

//ʤ���߿�
void CGameClientView::FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC)
{
	//�Ϸ��ж�
	if (m_cbAreaFlash==0xFF && false==m_bFlashResult) return;

	//��ע�ж�
	if (false==m_bFlashResult)
	{
		if (m_pGameClientDlg->GetGameStatus()==GS_PLACE_JETTON)
		{
			switch (m_cbAreaFlash)
			{
			case ID_TIAN_MEN:	
				{
					//�滭ͼƬ
					CImageHandle ImageHandle(&m_ImageFrameTianMen);
					m_ImageFrameTianMen.AlphaDrawImage(pDC,m_rcTianMen.left, m_rcTianMen.top, RGB(255,0,255));
					break;
				}
			case ID_DI_MEN:
				{
					//�滭ͼƬ
					CImageHandle ImageHandle(&m_ImageFrameDiMen);
					m_ImageFrameDiMen.AlphaDrawImage(pDC,m_rcDimen.left, m_rcDimen.top, RGB(255,0,255));
					break;
				}
			case ID_XUAN_MEN:
				{	
					//�滭ͼƬ
					CImageHandle ImageHandle(&m_ImageFrameXuanMen);	
					m_ImageFrameXuanMen.AlphaDrawImage(pDC,m_rcXuanMen.left, m_rcXuanMen.top, RGB(255,0,255));
					break;
				}
			case ID_HUANG_MEN:
				{
					//�滭ͼƬ
					CImageHandle ImageHandle(&m_ImageFrameHuangMen);	
					m_ImageFrameHuangMen.AlphaDrawImage(pDC,m_rcHuangMen.left, m_rcHuangMen.top, RGB(255,0,255));
					break;
				}
			}
		}
		
	}
	else
	{
		//ׯȫʤ�ж�
		if (false==m_bWinXuanMen && false==m_bWinTianMen && false==m_bWinHuangMen&&m_bWinDiMen == false)
		{
			//������Դ
			CImageHandle ImageHandleTianMen(&m_ImageFrameTianMen);
			CImageHandle ImageHandleDimen(&m_ImageFrameDiMen);
			CImageHandle ImageHandleHuangMen(&m_ImageFrameHuangMen);	
			CImageHandle ImageHandleXuanMen(&m_ImageFrameXuanMen);	

			//�滭ͼƬ
			m_ImageFrameTianMen.AlphaDrawImage(pDC,m_rcTianMen.left, m_rcTianMen.top, RGB(255,0,255));
			m_ImageFrameDiMen.AlphaDrawImage(pDC,m_rcDimen.left, m_rcDimen.top, RGB(255,0,255));	
			m_ImageFrameHuangMen.AlphaDrawImage(pDC,m_rcHuangMen.left, m_rcHuangMen.top, RGB(255,0,255));
			m_ImageFrameXuanMen.AlphaDrawImage(pDC,m_rcXuanMen.left, m_rcXuanMen.top, RGB(255,0,255));
		}
		else
		{
			//���ж�
			if (true==m_bWinXuanMen)
			{
				CImageHandle ImageHandleXuanMen(&m_ImageFrameXuanMen);
				m_ImageFrameXuanMen.AlphaDrawImage(pDC,m_rcXuanMen.left, m_rcXuanMen.top, RGB(255,0,255));
			}
			if (true==m_bWinTianMen)
			{
				CImageHandle ImageHandleTianMen(&m_ImageFrameTianMen);
				m_ImageFrameTianMen.AlphaDrawImage(pDC,m_rcTianMen.left, m_rcTianMen.top, RGB(255,0,255));
			}
			if (true==m_bWinHuangMen)
			{
				CImageHandle ImageHandleHuangMen(&m_ImageFrameHuangMen);
				m_ImageFrameHuangMen.AlphaDrawImage(pDC,m_rcHuangMen.left, m_rcHuangMen.top, RGB(255,0,255));
			}
			if (true==m_bWinDiMen)
			{
				CImageHandle ImageHandleDiMen(&m_ImageFrameDiMen);
				m_ImageFrameDiMen.AlphaDrawImage(pDC,m_rcDimen.left, m_rcDimen.top, RGB(255,0,255));
			}
		}
	}
}


//�ƶ�Ӯ��
void CGameClientView::DeduceWinner(bool &bWinTian, bool &bWinDi, bool &bWinXuan,bool &bWinHuan,BYTE &TianMultiple,BYTE &diMultiple,BYTE &TianXuanltiple,BYTE &HuangMultiple )
{
	//��С�Ƚ�
	bWinTian=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],5,m_cbTableCardArray[SHUN_MEN_INDEX],5,TianMultiple)==1?true:false;
	bWinDi=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],5,m_cbTableCardArray[DUI_MEN_INDEX],5,diMultiple)==1?true:false;
	bWinXuan=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],5,m_cbTableCardArray[DAO_MEN_INDEX],5,TianXuanltiple)==1?true:false;
	bWinHuan=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],5,m_cbTableCardArray[HUAN_MEN_INDEX],5,HuangMultiple)==1?true:false;
}

//�ؼ�����
BOOL CGameClientView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��ȡID
	WORD wControlID=LOWORD(wParam);

	//�ؼ��ж�
	switch (wControlID)
	{
	case IDC_JETTON_BUTTON_100:
		{
			//���ñ���
			m_lCurrentJetton=100L;
			break;
		}
	case IDC_JETTON_BUTTON_1000:
		{
			//���ñ���
			m_lCurrentJetton=1000L;
			break;
		}
	case IDC_JETTON_BUTTON_10000:
		{
			//���ñ���
			m_lCurrentJetton=10000L;
			break;
		}
	case IDC_JETTON_BUTTON_50000:
		{
			//���ñ���
			m_lCurrentJetton=50000L;
			break;
		}
	case IDC_JETTON_BUTTON_100000:
		{
			//���ñ���
			m_lCurrentJetton=100000L;
			break;
		}
	case IDC_JETTON_BUTTON_500000:
		{
			//���ñ���
			m_lCurrentJetton=500000L;
			break;
		}
	case IDC_JETTON_BUTTON_1000000:
		{
			//���ñ���
			m_lCurrentJetton=1000000L;
			break;
		}
	case IDC_JETTON_BUTTON_5000000:
		{
			//���ñ���
			m_lCurrentJetton=5000000L;
			break;
		}
	case IDC_AUTO_OPEN_CARD:
		{
			break;
		}
	case IDC_OPEN_CARD:
		{
			break;
		}
	case IDC_BANK:
		{
			break;
		}
	case IDC_CONTINUE_CARD:
		{
			AfxGetMainWnd()->SendMessage(IDM_CONTINUE_CARD,0,0);
			break;
		}
	}

	return CGameFrameView::OnCommand(wParam, lParam);
}

//�ҵ�λ��
void CGameClientView::SetMeChairID(DWORD dwMeUserID)
{
	//�������Ӻ�
	for (WORD wChairID=0; wChairID<MAX_CHAIR; ++wChairID)
	{
		tagUserData const *pUserData=GetUserInfo(wChairID);
		if (NULL!=pUserData && dwMeUserID==pUserData->dwUserID)
		{
			m_wMeChairID=wChairID;
			break;
		}
	}
}

//������ʾ
void CGameClientView::SetDispatchCardTip(enDispatchCardTip DispatchCardTip)
{
	//���ñ���
	m_enDispatchCardTip=DispatchCardTip;

	//���ö�ʱ��
	if (enDispatchCardTip_NULL!=DispatchCardTip) SetTimer(DISPATCH_CARD_TIP,2*1000,NULL);		
	else KillTimer(DISPATCH_CARD_TIP);

	//���½���
	UpdateGameView(NULL);
}

//�滭ׯ��
void CGameClientView::DrawBankerInfo(CDC *pDC,int nWidth,int nHeight)
{
	//ׯ����Ϣ																											
	pDC->SetTextColor(RGB(255,234,0));

	//��ȡ���
	tagUserData const *pUserData = m_wBankerUser==INVALID_CHAIR ? NULL : GetUserInfo(m_wBankerUser);

	//λ����Ϣ
	CRect static StrRect;
	StrRect.left = nWidth/2-293;
	StrRect.top = nHeight/2 - 328;
	StrRect.right = StrRect.left + 104;
	StrRect.bottom = StrRect.top + 15;

	//ׯ������
	pDC->DrawText(pUserData==NULL?(m_bEnableSysBanker?TEXT("ϵͳ��ׯ"):TEXT("������ׯ")):pUserData->szName, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP| DT_SINGLELINE );

	//ׯ���ܷ�
	StrRect.left = nWidth/2-293;
	StrRect.top = nHeight/2 - 306;
	StrRect.right = StrRect.left + 104;
	StrRect.bottom = StrRect.top + 15;
	DrawNumberStringWithSpace(pDC,pUserData==NULL?0:pUserData->lScore, StrRect);

	//ׯ�ҳɼ�
	StrRect.left = nWidth/2-293;
	StrRect.top = nHeight/2 - 255;
	StrRect.right = StrRect.left + 104;
	StrRect.bottom = StrRect.top + 15;
	DrawNumberStringWithSpace(pDC,m_lBankerWinScore,StrRect);

	//ׯ�Ҿ���
	StrRect.left = nWidth/2-293;
	StrRect.top = nHeight/2 - 280;
	StrRect.right = StrRect.left + 104;
	StrRect.bottom = StrRect.top + 15;
	DrawNumberStringWithSpace(pDC,m_wBankerTime,StrRect);
}

void CGameClientView::SetFirstShowCard(BYTE bcCard)
{
	m_bcfirstShowCard =  bcCard;
	m_CardControl[5].SetCardData(NULL,0);
	m_CardControl[5].SetCardData(&bcCard,1);
	m_CardControl[5].m_blGameEnd = true;
	m_CardControl[5].SetDisplayFlag(true);
	m_CardControl[5].ShowCardControl(true);
}

//�滭���
void CGameClientView::DrawMeInfo(CDC *pDC,int nWidth,int nHeight)
{
	//�Ϸ��ж�
	if (INVALID_CHAIR==m_wMeChairID) return;

	const tagUserData *pMeUserData = GetUserInfo(m_wMeChairID);

	if ( pMeUserData != NULL )
	{
		//������ɫ
		pDC->SetTextColor(RGB(255,234,0));

		//�ܹ���ע
		LONGLONG lMeJetton=0L;
		for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lMeJetton += m_lUserJettonScore[nAreaIndex];

		//����λ��
		CRect static rcDraw;
		rcDraw.left=m_MeInfoRect.left;
		rcDraw.top=m_MeInfoRect.top;
		rcDraw.right=rcDraw.left+114;
		rcDraw.bottom=rcDraw.top+15;

		//�滭�ʺ�
		pDC->DrawText(pMeUserData->szName,lstrlen(pMeUserData->szName),rcDraw,DT_VCENTER|DT_SINGLELINE|DT_LEFT|DT_END_ELLIPSIS);

		//�滭����
		rcDraw.top=m_MeInfoRect.top+21;
		rcDraw.bottom=rcDraw.top+15;
		DrawNumberStringWithSpace(pDC,pMeUserData->lScore-lMeJetton,rcDraw,DT_VCENTER|DT_SINGLELINE|DT_LEFT|DT_END_ELLIPSIS);

		//�滭�ɼ�
		rcDraw.top=m_MeInfoRect.top+2*21;
		rcDraw.bottom=rcDraw.top+15;
		DrawNumberStringWithSpace(pDC,m_lMeStatisticScore,rcDraw,DT_VCENTER|DT_SINGLELINE|DT_LEFT|DT_END_ELLIPSIS);
	}
}

//���д��
void CGameClientView::OnBankStorage()
{
#ifdef __BANKER___
	//��ȡ�ӿ�
	CGameClientDlg *pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);
	IClientKernel *pIClientKernel=(IClientKernel *)pGameClientDlg->GetClientKernel(IID_IClientKernel,VER_IClientKernel);

	if (NULL!=pIClientKernel)
	{
		if (NULL==m_DlgBank.m_hWnd) m_DlgBank.Create(IDD_BANK_STORAGE,this);

		if (!m_DlgBank.IsWindowVisible() || !m_DlgBank.m_bBankStorage)
		{
			m_DlgBank.SetSendInfo(pIClientKernel,pIClientKernel->GetMeUserInfo());
			m_DlgBank.SetBankerActionType(true);
			pGameClientDlg->ReSetGameCtr();
			CRect btRect;
			m_btBankerStorage.GetWindowRect(&btRect);
			m_DlgBank.SetPostPoint(CPoint(btRect.right,btRect.top));
			m_DlgBank.ShowWindow(SW_SHOW);
		}
		else
			m_DlgBank.ShowWindow(SW_HIDE);
	}
#endif
}

//����ȡ��
void CGameClientView::OnBankDraw()
{
#ifdef __BANKER___
	//��ȡ�ӿ�
	CGameClientDlg *pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);
	IClientKernel *pIClientKernel=(IClientKernel *)pGameClientDlg->GetClientKernel(IID_IClientKernel,VER_IClientKernel);

	if (NULL!=pIClientKernel)
	{
		if (NULL==m_DlgBank.m_hWnd) m_DlgBank.Create(IDD_BANK_STORAGE,this);

		if (!m_DlgBank.IsWindowVisible() || m_DlgBank.m_bBankStorage)
		{
			m_DlgBank.SetSendInfo(pIClientKernel,pIClientKernel->GetMeUserInfo());
			m_DlgBank.SetBankerActionType(false);
			pGameClientDlg->ReSetGameCtr();
			CRect btRect;
			m_btBankerDraw.GetWindowRect(&btRect);
			m_DlgBank.SetPostPoint(CPoint(btRect.right,btRect.top));
			m_DlgBank.ShowWindow(SW_SHOW);
		}
		else
			m_DlgBank.ShowWindow(SW_HIDE);
	}
#endif
}

void CGameClientView::OnUp()
{
	m_ApplyUser.m_AppyUserList.SendMessage(WM_VSCROLL, MAKELONG(SB_LINEUP,0),NULL);
	m_ApplyUser.m_AppyUserList.Invalidate(TRUE);
	double nPos = m_ApplyUser.m_AppyUserList.GetScrollPos(SB_VERT);
	double nMax = m_ApplyUser.m_AppyUserList.GetScrollLimit(SB_VERT);

}

void CGameClientView::OnDown()
{
	double nPos = m_ApplyUser.m_AppyUserList.GetScrollPos(SB_VERT);
	if(m_ApplyUser.m_AppyUserList.GetItemCount()>MAX_APPLY_DISPLAY)
	{
		if(nPos>m_ApplyUser.m_AppyUserList.GetItemCount()-5)
		{
			return ;
		}
	}
	m_ApplyUser.m_AppyUserList.SendMessage(WM_VSCROLL, MAKELONG(SB_LINEDOWN,0),NULL);
	m_ApplyUser.m_AppyUserList.Invalidate(TRUE);
	double nMax = m_ApplyUser.m_AppyUserList.GetScrollLimit(SB_VERT);
};

void CGameClientView::OpenAdminWnd()
{
	//����д�Ȩ��
	if((GetUserInfo(m_wMeChairID)->dwUserRight&UR_GAME_CONTROL)!=0)
	{
		if(!m_AdminDlg.IsWindowVisible()) m_AdminDlg.ShowWindow(SW_SHOW);
		else m_AdminDlg.ShowWindow(SW_HIDE);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////