#include "StdAfx.h"
#include "Resource.h"
#include "GameClientDlg.h"
#include "GameClientView.h"
#include ".\gameclientview.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��ť��ʶ
#define IDC_START						100								//��ʼ��ť
#define IDC_MAX_SCORE					104								//���ť
#define IDC_MIN_SCORE					105								//���ٰ�ť
#define IDC_CONFIRM						106								//ȷ����ť
#define	IDC_CANCEL_ADD					119								//ȡ����ť
#define IDC_ADD_SCORE					110								//��ע��ť
#define IDC_LOOK_CARD					111								//������Ϣ
#define IDC_COMPARE_CARD				112								//������Ϣ
#define IDC_FOLLOW						113								//������Ϣ
#define IDC_GIVE_UP						114								//������Ϣ
#define	IDC_OPEN_CARD					115								//������Ϣ

//��ʱ����ʶ
#define IDI_CARTOON						99								//������ʱ��
#define	TIME_CARTOON					100								//����ʱ��

//�����ٶ�  //(SPEED)10 (TIME_CARTOON)70 /CPU UP
#define	SPEED							7								//�����ٶȲ���
#define	ROCK							10								//�񶯸���

//λ�ñ���
#define	MY_SELF_CHAIRID					2								//�Լ�λ��

//���Ʊ���
#define VSCARD_TIME						3								//���ƴ���
#define VSCARD_COUNT					6								//������Ŀ
#define COMPARE_RADII					115								//���ƿ��

//���Ʊ���
#define SEND_PELS						30								//��������
#define LESS_SEND_COUNT					2								//���ʹ���
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)							  
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView() : CGameFrameView(true,32)
{
	//��������
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_lDrawGold[i].RemoveAll();
		m_ptKeepJeton[i].RemoveAll();
		m_lStopUpdataScore[i] = 0;
		m_wFlashUser[i]=INVALID_CHAIR;	
	}
	m_bCartoon =false;
	m_SendCardPos.SetPoint(0,0);
	m_SendCardItemArray.RemoveAll();

	//���Ʊ���
	m_wConmareIndex=0;
	m_wConmareCount=0;
	m_wLoserUser=INVALID_CHAIR;
	ZeroMemory(m_wCompareChairID,sizeof(m_wCompareChairID));

	//���ݱ���
	ZeroMemory(m_tcBuffer,sizeof(m_tcBuffer));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_lCurrentScore,sizeof(m_lCurrentScore));
	ZeroMemory(m_bCompareUser,sizeof(m_bCompareUser));
	m_wBankerUser=INVALID_CHAIR;
	m_lMaxCellScore=0;
	m_lCellScore=0;	
	m_wFalshCount=0;
	m_bStopDraw=false;
	m_bFalsh=false;
	m_bInvest=false;		
	m_bShow=false;
	m_bCompareCard=false;
	m_wWaitUserChoice=INVALID_CHAIR;

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageJeton.SetLoadInfo(IDB_GOLD,hInstance);
	m_ImageCard.SetLoadInfo(IDB_CARD,hInstance);
	m_ImageTitle.SetLoadInfo(IDB_TITLE,hInstance);
	m_ImageViewBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_ImageViewCenter.SetLoadInfo(IDB_VIEW_FILL,hInstance);
	m_ImageBanker.SetLoadInfo(IDB_BANKER_FLAG,hInstance);
	m_ImageArrowhead.SetLoadInfo(IDB_ARROWHEAD,hInstance);
	m_ImageReady.SetLoadInfo(IDB_READY,hInstance);
	m_ImageNumber.SetLoadInfo(IDB_NUMBER,hInstance);
	m_ImagePocket.SetLoadInfo(IDB_POCKET,hInstance);
	m_ImageBalance.SetLoadInfo(IDB_SCORE_VIEW,hInstance);

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

	//�����ؼ�
	CRect rcCreate(0,0,0,0);
	m_GoldControl.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS,CRect(50,50,66,66),this,8);
	m_ScoreView.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,this,10);

	//������ť
	m_btStart.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,rcCreate,this,IDC_START);
	m_btMaxScore.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_MAX_SCORE);
	m_btMinScore.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_MIN_SCORE);
	m_btConfirm.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_CONFIRM);
	m_btCancel.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_CANCEL_ADD);
	m_btOpenCard.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_OPEN_CARD);

	m_btAddScore.Create(TEXT(""),WS_CHILD|WS_DISABLED,rcCreate,this,IDC_ADD_SCORE);
	m_btCompareCard.Create(TEXT(""),WS_CHILD|WS_DISABLED,rcCreate,this,IDC_COMPARE_CARD);
	m_btGiveUp.Create(TEXT(""),WS_CHILD|WS_DISABLED,rcCreate,this,IDC_GIVE_UP);
	m_btLookCard.Create(TEXT(""),WS_CHILD|WS_DISABLED,rcCreate,this,IDC_LOOK_CARD);
	m_btFollow.Create(TEXT(""),WS_CHILD|WS_DISABLED,rcCreate,this,IDC_FOLLOW);

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btStart.SetButtonImage(IDB_START,hInstance,false);
	m_btMaxScore.SetButtonImage(IDB_USERCONTROL_MAX,hInstance,false);
	m_btMinScore.SetButtonImage(IDB_USERCONTROL_MIN,hInstance,false);
	m_btAddScore.SetButtonImage(IDB_USERCONTROL_INVEST,hInstance,false);
	m_btConfirm.SetButtonImage(IDB_CONFIRM,hInstance,false);
	m_btCancel.SetButtonImage(IDB_USERCONTROL_CLEAR,hInstance,false);
	m_btCompareCard.SetButtonImage(IDB_COMPARE_CARD,hInstance,false);
	m_btOpenCard.SetButtonImage(IDB_OPEN_CARD,hInstance,false);
	m_btGiveUp.SetButtonImage(IDB_GIVE_UP,hInstance,false);
	m_btLookCard.SetButtonImage(IDB_LO0K_CARD,hInstance,false);
	m_btFollow.SetButtonImage(IDB_FOLLOW,hInstance,false);

	//�����ؼ�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_CardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,rcCreate,this,20+i);
		m_CardControl[i].SetCommandID(i,IDM_COMPARE_USER);
	}

	return 0;
}

//�����
BOOL CGameClientView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_START:				//��ʼ��ť
		{
			//������Ϣ
			AfxGetMainWnd()->SendMessage(IDM_START,0,0);
			return TRUE;
		}
	case IDC_ADD_SCORE:			//��ע��ť
		{
			//������Ϣ
			AfxGetMainWnd()->SendMessage(IDM_ADD_SCORE,0,0);
			return TRUE;
		}
	case IDC_CONFIRM:			//ȷ����ť
		{
			//������Ϣ
			AfxGetMainWnd()->SendMessage(IDM_CONFIRM,0,0);
			return TRUE;
		}
	case IDC_CANCEL_ADD:		//ȡ����ť
		{
			//������Ϣ
			AfxGetMainWnd()->SendMessage(IDM_CANCEL_ADD,0,0);
			return TRUE;
		}
	case IDC_FOLLOW:			//��ע��ť
		{
			//������Ϣ
			AfxGetMainWnd()->SendMessage(IDM_CONFIRM,1,1);
			return TRUE;
		}
	case IDC_MIN_SCORE:			//���ٰ�ť
		{
			//������Ϣ
			AfxGetMainWnd()->SendMessage(IDM_MIN_SCORE,0,0);
			return TRUE;
		}
	case IDC_MAX_SCORE:			//���ť
		{
			//������Ϣ
			AfxGetMainWnd()->SendMessage(IDM_MAX_SCORE,0,0);
			return TRUE;
		}
	case IDC_COMPARE_CARD:		//���ư�ť
		{
			//������Ϣ
			AfxGetMainWnd()->SendMessage(IDM_COMPARE_CARD,0,0);
			return TRUE;
		}
	case IDC_LOOK_CARD:			//���ư�ť
		{
			//������Ϣ
			AfxGetMainWnd()->SendMessage(IDM_LOOK_CARD,0,0);
			return TRUE;
		}
	case IDC_OPEN_CARD:			//������Ϣ
		{
			//������Ϣ
			AfxGetMainWnd()->SendMessage(IDM_OPEN_CARD,0,0);
			return TRUE;
		}
	case IDC_GIVE_UP:			//������ť
		{
			//������Ϣ
			AfxGetMainWnd()->SendMessage(IDM_GIVE_UP,0,0);
			return TRUE;
		}
	}

	return __super::OnCommand(wParam, lParam);
}

//���ý���
void CGameClientView::ResetGameView()
{
	//ɾ��ʱ��
	KillTimer(IDI_CARTOON);

	//��������
	m_bCartoon =false;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_lDrawGold[i].RemoveAll();
		m_ptKeepJeton[i].RemoveAll();
		m_wFlashUser[i]=INVALID_CHAIR;	
		m_lStopUpdataScore[i] = 0;
	}
	m_SendCardPos.SetPoint(0,0);
	m_SendCardItemArray.RemoveAll();

	//���Ʊ���	
	m_wConmareIndex = 0;		
	m_wConmareCount = 0;		
	m_wLoserUser=INVALID_CHAIR;		
	ZeroMemory(m_wCompareChairID,sizeof(m_wCompareChairID));

	//���ݱ���
	ZeroMemory(m_tcBuffer,sizeof(m_tcBuffer));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	m_wBankerUser=INVALID_CHAIR;			
	ZeroMemory(m_lCurrentScore,sizeof(m_lCurrentScore));
	ZeroMemory(m_bCompareUser,sizeof(m_bCompareUser));
	m_lMaxCellScore=0;					
	m_lCellScore=0;		
	m_wFalshCount=0;
	m_bStopDraw=false;
	m_bFalsh=false;
	m_bInvest=false;		
	m_bShow=false;
	m_bCompareCard=false;
	m_wWaitUserChoice=INVALID_CHAIR;

	//����ť
	m_btStart.ShowWindow(SW_HIDE);
	m_btCancel.ShowWindow(SW_HIDE);
	m_btConfirm.ShowWindow(SW_HIDE);
	m_btMaxScore.ShowWindow(SW_HIDE);
	m_btMinScore.ShowWindow(SW_HIDE);
	m_btOpenCard.ShowWindow(SW_HIDE);
	m_GoldControl.ShowWindow(SW_HIDE);

	m_btAddScore.EnableWindow(FALSE);
	m_btCompareCard.EnableWindow(FALSE);
	m_btGiveUp.EnableWindow(FALSE);
	m_btLookCard.EnableWindow(FALSE);
	m_btFollow.EnableWindow(FALSE);

	//���ؿؼ�
	m_ScoreView.ShowWindow(SW_HIDE);
	for (WORD i=0;i<GAME_PLAYER;i++) m_CardControl[i].SetCardData(NULL,0);

	return;
}

//�����ؼ�
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	CImageHandle ImageHandleBanker(&m_ImageBanker);

	//WORD w=0;
	//for(WORD i=0;i<GAME_PLAYER;i++)
	//{
	//	if(˳ʱ��)
	//	{
	//		if(i==0)w=...
	//	}
	//...
	//��������
	m_ptFace[0].x=nWidth/2+250;
	m_ptFace[0].y=nHeight/2-261;
	m_ptFace[1].x=nWidth/2+250;
	m_ptFace[1].y=nHeight/2-m_nYFace-10;
	m_ptFace[2].x=nWidth/2-10;
	m_ptFace[2].y=nHeight/2+204;
	m_ptFace[3].x=nWidth/2-275;
	m_ptFace[3].y=nHeight/2-m_nYFace-10;
	m_ptFace[4].x=nWidth/2-275;
	m_ptFace[4].y=nHeight/2-261;

	//ʱ��λ��
	m_ptTimer[0].x=nWidth/2+185;
	m_ptTimer[0].y=nHeight/2-230;
	m_ptTimer[1].x=nWidth/2+185;
	m_ptTimer[1].y=nHeight/2-15;
	m_ptTimer[2].x=nWidth/2+10;
	m_ptTimer[2].y=nHeight/2+86;
	m_ptTimer[3].x=nWidth/2-190;
	m_ptTimer[3].y=nHeight/2-15;
	m_ptTimer[4].x=nWidth/2-190;
	m_ptTimer[4].y=nHeight/2-230;

	//׼��λ��
	m_ptReady[0].x=nWidth/2+185;
	m_ptReady[0].y=nHeight/2-230;
	m_ptReady[1].x=nWidth/2+185;
	m_ptReady[1].y=nHeight/2-15;
	m_ptReady[2].x=nWidth/2+10;
	m_ptReady[2].y=nHeight/2+86;
	m_ptReady[3].x=nWidth/2-185;
	m_ptReady[3].y=nHeight/2-15;
	m_ptReady[4].x=nWidth/2-185;
	m_ptReady[4].y=nHeight/2-230;

	//�û��˿�
	m_CardControl[0].SetBenchmarkPos(nWidth/2+245,nHeight/2-150,enXCenter,enYCenter);
	m_CardControl[1].SetBenchmarkPos(nWidth/2+245,nHeight/2+69,enXCenter,enYCenter);
	m_CardControl[2].SetBenchmarkPos(nWidth/2+10,nHeight/2+203,enXCenter,enYBottom);
	m_CardControl[3].SetBenchmarkPos(nWidth/2-245,nHeight/2+69,enXCenter,enYCenter);
	m_CardControl[4].SetBenchmarkPos(nWidth/2-245,nHeight/2-150,enXCenter,enYCenter);

	//ׯ�ұ�־
	m_PointBanker[0].x=m_ptFace[0].x+45;
	m_PointBanker[0].y=m_ptFace[0].y+5;
	m_PointBanker[1].x=m_ptFace[1].x+45;
	m_PointBanker[1].y=m_ptFace[1].y+5;
	m_PointBanker[2].x=m_ptFace[2].x+45;
	m_PointBanker[2].y=m_ptFace[2].y+10;
	m_PointBanker[3].x=m_ptFace[3].x-55;
	m_PointBanker[3].y=m_ptFace[3].y+5;
	m_PointBanker[4].x=m_ptFace[4].x-55;
	m_PointBanker[4].y=m_ptFace[4].y+5;

	//����λ��
	//m_ptName[0].x=nWidth/2+5+m_nXFace/2+50;
	//m_ptName[0].y=5+m_nYBorder+5;
	//m_ptName[1].x=nWidth-m_nXBorder-m_nXFace-10-30;
	//m_ptName[1].y=nHeight/2-m_nYFace+3;
	//m_ptName[2].x=nWidth/2+5+m_nXFace/2+50;
	//m_ptName[2].y=nHeight-m_nYBorder-m_nYFace-2-25;
	//m_ptName[3].x=m_nXBorder+m_nXFace+10+75;
	//m_ptName[3].y=nHeight/2-m_nYFace+3+130;
	//m_ptName[4].x=m_nXBorder+m_nXFace+10+75;
	//m_ptName[4].y=nHeight/2-m_nYFace+3-110;

	//����λ��
	m_ptJeton[0].x=m_ptFace[0].x+50;
	m_ptJeton[0].y=m_PointBanker[0].y+210+15;
	m_ptJeton[1].x=m_PointBanker[1].x-60-30;
	m_ptJeton[1].y=m_ptFace[1].y+13;
	m_ptJeton[2].x=m_ptFace[0].x+50;
	m_ptJeton[2].y=m_PointBanker[2].y-210-30;
	m_ptJeton[3].x=m_PointBanker[3].x+130+75;
	m_ptJeton[3].y=m_ptFace[1].y+13+130;
	m_ptJeton[4].x=m_PointBanker[3].x+130+75;
	m_ptJeton[4].y=m_ptFace[1].y+13-110;

	m_ptUserCard[0].x=m_CardControl[0].GetCardPos().x;
	m_ptUserCard[0].y=m_CardControl[0].GetCardPos().y;
	m_ptUserCard[1].x=m_CardControl[1].GetCardPos().x-40;
	m_ptUserCard[1].y=m_CardControl[1].GetCardPos().y+60;
	m_ptUserCard[2].x=m_CardControl[2].GetCardPos().x;
	m_ptUserCard[2].y=m_CardControl[2].GetCardPos().y-20;
	m_ptUserCard[3].x=m_CardControl[3].GetCardPos().x+40;
	m_ptUserCard[3].y=m_CardControl[3].GetCardPos().y+39;
	m_ptUserCard[4].x=m_CardControl[4].GetCardPos().x+80;
	m_ptUserCard[4].y=m_CardControl[4].GetCardPos().y+20;

	//��ť�ؼ�
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	//��ʼ��ť
	m_btStart.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btStart,NULL,nWidth/2+53,nHeight/2+258,0,0,uFlags);

	//���ư�ť
	m_btAddScore.GetWindowRect(&rcButton);
	//INT n = m_nYBorder;
	DeferWindowPos(hDwp,m_btAddScore,NULL,nWidth/2+22,nHeight/2+281,0,0,uFlags);
	DeferWindowPos(hDwp,m_btFollow,NULL,nWidth/2+95,nHeight/2+268,0,0,uFlags);
	DeferWindowPos(hDwp,m_btLookCard,NULL,nWidth/2+165,nHeight/2+244,0,0,uFlags);
	DeferWindowPos(hDwp,m_btGiveUp,NULL,nWidth/2+284,nHeight/2+156,0,0,uFlags);
	DeferWindowPos(hDwp,m_btCompareCard,NULL,nWidth/2+229,nHeight/2+204,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOpenCard,NULL,nWidth/2+229,nHeight/2+204,0,0,uFlags);

	//����λ��
	m_ptUserCompare.SetPoint(nWidth/2,nHeight*2/5);

	int iX=nWidth/2+80;
	int iY=nHeight/2+200;

	m_btMaxScore.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btMinScore,NULL,iX+140,iY+3,0,0,uFlags);
	DeferWindowPos(hDwp,m_btMaxScore,NULL,iX+20,iY+80,0,0,uFlags);
	DeferWindowPos(hDwp,m_btCancel,NULL,iX+73,iY+80,0,0,uFlags);
	DeferWindowPos(hDwp,m_btConfirm,NULL,iX+125,iY+80,0,0,uFlags);

	//�����ƶ�
	EndDeferWindowPos(hDwp);

	//��ע����
	m_GoldControl.SetBasicPoint(iX,iY);

	//������ͼ
	CRect rcControl;
	m_ScoreView.GetWindowRect(&rcControl);
	m_ScoreView.SetWindowPos(NULL,(nWidth-rcControl.Width())/2,nHeight/2-190,0,0,SWP_NOZORDER|SWP_NOSIZE);

	return;
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//����Ч��
	if(m_bFalsh)
	{
		CImageHandle ImageHandleBack(&m_ImageViewBack);
		int xValue=rand()%ROCK;
		int yValue=rand()%ROCK;
		if(xValue%2==0)xValue*=(-1);
		if(yValue%2==0)yValue*=(-1);
		m_ImageViewBack.AlphaDrawImage(pDC,(nWidth/2-512)+xValue,(nHeight/2-397)+yValue,
			m_ImageViewBack.GetWidth(),m_ImageViewBack.GetHeight(),0,0,RGB(255,0,255));
	}
	else 
	{
		CImageHandle ImageHandleBack(&m_ImageViewBack);
		m_ImageViewBack.AlphaDrawImage(pDC,(nWidth/2-512+6),(nHeight/2-383),
			m_ImageViewBack.GetWidth(),m_ImageViewBack.GetHeight(),0,0,RGB(255,0,255));

		//�滭����
		//DrawViewImage(pDC,m_ImageViewCenter,enMode_Spread);//������󻯴���
		//DrawViewImage(pDC,m_ImageViewBack,enMode_Centent);
	}

	if(!m_bStopDraw)
	{
		//�滭�û�
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//��������
			WORD wUserTimer=GetUserTimer(i);
			const tagUserData * pUserData=GetUserInfo(i);

#ifdef _DEBUG
			////���Դ���
			//tagUserData Obj;
			//Obj.cbGender = 1;
			//_sntprintf(Obj.szName,sizeof(Obj.szName),TEXT("�û�������"));
			//Obj.cbUserStatus=US_READY;
			////Obj.wFaceID = 2;
			//pUserData = &Obj;

			////ׯ�ұ�־
			//CImageHandle ImageHandleBanker(&m_ImageBanker);
			//m_ImageBanker.AlphaDrawImage(pDC,m_PointBanker[i].x,m_PointBanker[i].y,m_ImageBanker.GetWidth(),m_ImageBanker.GetHeight(),0,0,RGB(255,0,255));
#endif
			//�滭�û�
			if (pUserData!=NULL)
			{
				//�û�����
				pDC->SetTextAlign(TA_CENTER);
				pDC->SetTextColor((wUserTimer>0)?RGB(250,250,250):RGB(220,220,220));
				DrawTextString(pDC,pUserData->szName,RGB(255,255,255),RGB(0,0,0),m_ptFace[i].x+15,m_ptFace[i].y+35);

				//�û����
				TCHAR szBuffer[64]=TEXT("");
				LONG lLeaveScore=pUserData->lScore-((m_ScoreView.IsWindowVisible()==TRUE)?(0):(m_lTableScore[i]));//;
				if(m_lStopUpdataScore[i]>0)lLeaveScore=m_lStopUpdataScore[i]-m_lTableScore[i];
				_snprintf(szBuffer,sizeof(szBuffer),TEXT("��%ld"),lLeaveScore);
				DrawTextString(pDC,szBuffer,RGB(255,255,255),RGB(0,0,0),m_ptFace[i].x+10,m_ptFace[i].y+48);

				//������Ϣ
				//wUserTimer =1;
				if (wUserTimer!=0) 
				{
					if(m_CardControl[MY_SELF_CHAIRID].GetCardCount()==0)
						DrawUserTimer(pDC,m_ptFace[i].x+m_nXFace/2,m_ptTimer[i].y+90,wUserTimer);
					else DrawUserTimer(pDC,m_ptTimer[i].x,m_ptTimer[i].y,wUserTimer);
				}

				//׼����־
				if (pUserData->cbUserStatus==US_READY) 
				{
					DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
				}
				DrawUserFace(pDC,pUserData,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);
			}
		}

		//������ע
		LONG lTableScore=0L;
		for (WORD i=0;i<GAME_PLAYER;i++) lTableScore+=m_lTableScore[i];

		//��ע��Ϣ
		if (lTableScore>0L)
		{	
			CImageHandle ImageHandle3(&m_ImageNumber);
			int iCount=0;
			LONG lCell[11];
			ZeroMemory(lCell,sizeof(lCell));
			while(lTableScore>0L)
			{
				LONG lScore=lTableScore%10;
				lCell[iCount++] = lScore;
				lTableScore=lTableScore/10;
			}
			int iTemp=0;
			int iX=m_ImageNumber.GetWidth();
			for (int i=iCount-1;i>=0;i--)
			{
				m_ImageNumber.AlphaDrawImage(pDC,nWidth/2+39+(iX/10)*(iTemp++),nHeight/2-296,
					iX/10,m_ImageNumber.GetHeight(),iX/10*lCell[i],0,RGB(255,0,255));
			}
		}

		//ׯ����Ϣ
		if (m_wBankerUser!=INVALID_CHAIR)
		{
			//ׯ�ұ�־
			CImageHandle ImageHandleBanker(&m_ImageBanker);
			m_ImageBanker.AlphaDrawImage(pDC,m_PointBanker[m_wBankerUser].x,m_PointBanker[m_wBankerUser].y,m_ImageBanker.GetWidth(),m_ImageBanker.GetHeight(),0,0,RGB(255,0,255));
		}

		//�����ڸ���
		for (BYTE i=0;i<GAME_PLAYER;i++)	
		{
			//�滭�ж�
			//m_lCurrentScore[i] = 100000;
			if (m_lCurrentScore[i]==0L) continue;
			LONG lTemp=m_lCurrentScore[i];
			LONG lTimes[]={10,20,50,100,200,500,1000,2000,5000,10000,20000,50000,100000,200000,500000};
			for (BYTE k=CountArray(lTimes)-1;;k--)
			{
				if(lTemp>=lTimes[k])
				{
					//�滭��Ϣ
					tagGoldMes tagTemp;
					tagTemp.lDrawCount = (lTemp/lTimes[k]);
					tagTemp.lGoldType = k;
					m_lDrawGold[i].Add(tagTemp);

					while((tagTemp.lDrawCount--)>0)
					{
						//���λ��
						int xTemp=rand();
						int yTemp=rand();
						int bRandX=nWidth/2-25+xTemp%60*((xTemp%2==0)?1:(-1));
						int bRandY=nHeight/2-70+yTemp%60*((yTemp%2==0)?1:(-1));

						//�������λ��
						CPoint ptTemp;
						ptTemp.SetPoint(bRandX,bRandY);
						m_ptKeepJeton[i].Add(ptTemp);
					}

					lTemp%=lTimes[k];
					if(lTemp==0)break;
				}
			}
			m_lCurrentScore[i]=0L;
		}

		//�滭����
		for (BYTE i=0;i<GAME_PLAYER;i++)
		{
			//m_lTableScore[i] = 100000;
			if (m_lTableScore[i]==0L ) continue;
			CImageHandle HandleJeton(&m_ImageJeton);

			//�̶�λ�����
			int Count=0;
			for(int j=0;j<(m_lDrawGold[i].GetCount());j++)
			{
				LONG lDrawCount = m_lDrawGold[i].GetAt(j).lDrawCount;
				while((lDrawCount--)>0)
				{
					CPoint cpTemp=m_ptKeepJeton[i].GetAt(Count++);
					m_ImageJeton.AlphaDrawImage(pDC,cpTemp.x,cpTemp.y,m_ImageJeton.GetWidth()/15,
						m_ImageJeton.GetHeight(),m_ImageJeton.GetWidth()/15*(m_lDrawGold[i].GetAt(j).lGoldType),0,RGB(255,0,255));
					if(Count==100)break;
				}
				if(Count==100)break;
			}

			//�滭��Ŀ
			TCHAR szBuffer[64];
			_snprintf(szBuffer,CountArray(szBuffer),TEXT("$:%ld"),m_lTableScore[i]);

			//����λ��
			CRect rcDrawRect;
			rcDrawRect.top=m_CardControl[i].GetCardPos().y;
			rcDrawRect.left=m_CardControl[i].GetCardPos().x+((i>2)?(135):(-50));
			rcDrawRect.right=rcDrawRect.left+30;
			rcDrawRect.bottom=rcDrawRect.top+12;

			//��������
			CFont ViewFont;
			ViewFont.CreateFont(-14,0,0,0,700,0,0,0,134,3,2,1,2,TEXT("����"));

			//��������
			CFont * pOldFont=pDC->SelectObject(&ViewFont);

			//�滭��Ŀ
			pDC->SetTextAlign(TA_LEFT);
			pDC->SetTextColor(RGB(250,200,40));
			pDC->DrawText(szBuffer,lstrlen(szBuffer),&rcDrawRect,DT_CENTER|DT_VCENTER|DT_NOCLIP|DT_SINGLELINE);

			//��������
			pDC->SelectObject(pOldFont);
		}

		//�滭��Ϣ
		if(m_lCellScore>0)
		{
			//��������
			CFont ViewFont;
			ViewFont.CreateFont(-12,0,0,0,700,0,0,0,134,3,2,1,2,TEXT("����"));

			//��������
			CFont * pOldFont=pDC->SelectObject(&ViewFont);

			pDC->SetTextAlign(TA_LEFT);
			pDC->SetTextColor(RGB(0,0,0));

			//����λ��
			CRect rcDrawRect;

			//�滭��Ŀ
			TCHAR szBuffer[64];
			_snprintf(szBuffer,CountArray(szBuffer),TEXT("�ⶥ��%ld"),m_lMaxCellScore);
			rcDrawRect.top=nHeight/2-338;
			rcDrawRect.left=nWidth/2-233;
			rcDrawRect.right=rcDrawRect.left+50;
			rcDrawRect.bottom=rcDrawRect.top+12;
			pDC->DrawText(szBuffer,lstrlen(szBuffer),&rcDrawRect,DT_CENTER|DT_VCENTER|DT_NOCLIP|DT_SINGLELINE);

			_snprintf(szBuffer,CountArray(szBuffer),TEXT("��ע��%ld"),m_lCellScore);
			rcDrawRect.top=nHeight/2-318;
			rcDrawRect.left=nWidth/2-230;
			rcDrawRect.right=rcDrawRect.left+50;
			rcDrawRect.bottom=rcDrawRect.top+12;
			pDC->DrawText(szBuffer,lstrlen(szBuffer),&rcDrawRect,DT_CENTER|DT_VCENTER|DT_NOCLIP|DT_SINGLELINE);

			//��������
			pDC->SelectObject(pOldFont);		
		}

		//ѡ�ȱ�־
		if(m_bCompareCard)
		{
			for (int i=0;i<GAME_PLAYER;i++)
			{
				if(m_bCompareUser[i]==TRUE)
				{
					CPoint cPost=m_CardControl[i].GetCardPos();
					CImageHandle ImageArrowhead(&m_ImageArrowhead);
					m_ImageArrowhead.AlphaDrawImage(pDC,cPost.x,cPost.y-m_ImageArrowhead.GetHeight(),
						m_ImageArrowhead.GetWidth(),m_ImageArrowhead.GetHeight(),0,0,RGB(255,0,255));
				}
			}
		}
	}

	//���ƻ滭
	if (m_SendCardItemArray.GetCount()>0)
	{
		//��������
		CImageHandle HandleCard(&m_ImageCard);
		tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];

		//��ȡ��С
		int nItemWidth=m_ImageCard.GetWidth()/13;
		int nItemHeight=m_ImageCard.GetHeight()/5;

		//�滭�˿�
		int nXPos=m_SendCardPos.x-nItemWidth/2;
		int nYPos=m_SendCardPos.y-nItemHeight/2+10;
		m_ImageCard.AlphaDrawImage(pDC,nXPos,nYPos,nItemWidth,nItemHeight,nItemWidth*2,nItemHeight*4,RGB(255,0,255));
	}

	//�����˿�
	if (m_wLoserUser!=INVALID_CHAIR)
	{
		//����״̬
		if (m_wConmareIndex<m_wConmareCount)
		{
			//�滭�˿�
			for (BYTE i=0;i<2;i++)
			{
				//��������
				CPoint PointCard;
				CPoint PointStart=m_ptUserCard[m_wCompareChairID[i]];

				//����λ��
				INT nQuotiety=(i==0)?-1:1;
				PointCard.y=PointStart.y+(m_ptUserCompare.y-PointStart.y)*m_wConmareIndex/m_wConmareCount;
				PointCard.x=PointStart.x+(m_ptUserCompare.x-PointStart.x+COMPARE_RADII*nQuotiety)*m_wConmareIndex/m_wConmareCount;

				//�滭�˿�
				DrawCompareCard(pDC,PointCard.x,PointCard.y,false);
			}
		}

		//����״̬
		if ((m_wConmareIndex>=m_wConmareCount)&&((m_wConmareIndex<m_wConmareCount+VSCARD_COUNT*VSCARD_TIME)))
		{
			//�滭�˿�
			for (BYTE i=0;i<2;i++)
			{
				//��������
				CPoint PointCard;
				CPoint PointStart=m_ptUserCard[m_wCompareChairID[i]];

				//����λ��
				INT nQuotiety=(i==0)?-1:1;
				PointCard.y=m_ptUserCompare.y;
				PointCard.x=m_ptUserCompare.x+COMPARE_RADII*nQuotiety;

				//�滭�˿�
				DrawCompareCard(pDC,PointCard.x,PointCard.y,false);
			}

			//���Ʊ�־
			CSize SizeVSCard;
			CPngImage ImageVSCard;
			ImageVSCard.LoadImage(AfxGetInstanceHandle(),TEXT("VS_CARD"));
			SizeVSCard.SetSize(ImageVSCard.GetWidth()/VSCARD_COUNT,ImageVSCard.GetHeight());

			//�滭��־
			INT nImageIndex=(m_wConmareIndex-m_wConmareCount)%VSCARD_COUNT;
			ImageVSCard.DrawImage(pDC,m_ptUserCompare.x-SizeVSCard.cx/2,m_ptUserCompare.y-SizeVSCard.cy/2,
				SizeVSCard.cx,SizeVSCard.cy,nImageIndex*SizeVSCard.cx,0);

			//����Ч��
			CGameClientDlg * pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);
			pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("COMPARE_CARD"));

		}

		//����״̬
		if (m_wConmareIndex>=m_wConmareCount+VSCARD_COUNT*VSCARD_TIME)
		{
			//��������
			WORD wConmareIndex=m_wConmareIndex-m_wConmareCount-VSCARD_COUNT*VSCARD_TIME;

			//�滭�˿�
			for (BYTE i=0;i<2;i++)
			{
				//��������
				CPoint PointCard;
				CPoint PointStart=m_ptUserCard[m_wCompareChairID[i]];

				//����λ��
				INT nQuotiety=(i==0)?-1:1;
				PointCard.y=m_ptUserCompare.y+(PointStart.y-m_ptUserCompare.y)*wConmareIndex/m_wConmareCount;
				PointCard.x=m_ptUserCompare.x+COMPARE_RADII*nQuotiety+(PointStart.x-m_ptUserCompare.x-COMPARE_RADII*nQuotiety)*wConmareIndex/m_wConmareCount;

				//�滭�˿�
				DrawCompareCard(pDC,PointCard.x,PointCard.y,(m_wLoserUser==m_wCompareChairID[i])?true:false);
			}

			//����Ч��
			if(m_wConmareIndex==m_wConmareCount+VSCARD_COUNT*VSCARD_TIME)
			{
				CGameClientDlg * pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);
				pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("CENTER_SEND_CARD"));
			}
		}
	}

	//�ȴ�ѡ��
	if(m_wWaitUserChoice!=INVALID_CHAIR)
	{
		TCHAR szBuffer[64]=TEXT("");

		//��������
		CFont ViewFont;
		ViewFont.CreateFont(-15,0,0,0,700,0,0,0,134,3,2,1,2,TEXT("����"));

		//��������
		CFont * pOldFont=pDC->SelectObject(&ViewFont);
		pDC->SetTextAlign(TA_CENTER);

		//�����Ϣ
		if(m_wWaitUserChoice==TRUE)_sntprintf(szBuffer,sizeof(szBuffer),TEXT("�ȴ����ѡ�����"));
		else _sntprintf(szBuffer,sizeof(szBuffer),TEXT("��ѡ����ҽ��б���"));
		DrawTextString(pDC,szBuffer,RGB(50,50,50),RGB(0,170,0),nWidth/2+10,nHeight/2+45);

		//��������
		pDC->SelectObject(pOldFont);
	}

	return;
}

//������ע
void CGameClientView::SetUserTableScore(WORD wChairID, LONG lTableScore,LONG lCurrentScore)
{
	//��������
	if (wChairID!=INVALID_CHAIR) 
	{
		m_lCurrentScore[wChairID]=lCurrentScore;
		m_lTableScore[wChairID]=lTableScore;
	}
	else 
	{
		ZeroMemory(m_tcBuffer,sizeof(m_tcBuffer));
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			m_lDrawGold[i].RemoveAll();
			m_ptKeepJeton[i].RemoveAll();
		}
		ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
		ZeroMemory(m_lCurrentScore,sizeof(m_lCurrentScore));
	}

	//���½���
	UpdateGameView(NULL);

	return;
}

//�ȴ�ѡ��
void CGameClientView::SetWaitUserChoice(WORD wChoice)
{
	m_wWaitUserChoice = wChoice;

	//���½���
	UpdateGameView(NULL);

	return;
}

//ׯ�ұ�־
void CGameClientView::SetBankerUser(WORD wBankerUser)
{
	m_wBankerUser=wBankerUser;

	//���½���
	UpdateGameView(NULL);
	return;
}

////�ȴ���־
//void CGameClientView::SetWaitInvest(bool bInvest)
//{
//	if(m_bInvest!=bInvest)
//	{
//		m_bInvest=bInvest;
//
//		//���½���
//		UpdateGameView(NULL);
//	}
//	return;
//}

//������Ϣ
void CGameClientView::SetScoreInfo(LONG lTurnMaxScore,LONG lTurnLessScore)
{
	m_lMaxCellScore=lTurnMaxScore;
	m_lCellScore=lTurnLessScore;

	//���½���
	UpdateGameView(NULL);
	return;
}

////��ʾ����
//void CGameClientView::DisplayType(bool bShow)
//{
//	if(m_bShow!=bShow)
//	{
//		m_bShow=bShow;
//
//		//���½���
//		UpdateGameView(NULL);
//	}
//}

//���Ʊ�־
void CGameClientView::SetCompareCard(bool bCompareCard,BOOL bCompareUser[])
{
	m_bCompareCard=bCompareCard;
	if(bCompareUser!=NULL)
	{
		CopyMemory(m_bCompareUser,bCompareUser,sizeof(m_bCompareUser));
		for(int i=0;i<GAME_PLAYER;i++)
		{
			if(m_bCompareUser[i]==TRUE)m_CardControl[i].SetCompareCard(true);
			else m_CardControl[i].SetCompareCard(false);
		}
	}
	//���½���
	UpdateGameView(NULL);

	if(!bCompareCard)
	{
		for(int i=0;i<GAME_PLAYER;i++)
		{
			m_CardControl[i].SetCompareCard(false) ;
		}
	}

	return;
}

//ֹͣ����
void CGameClientView::StopUpdataScore(bool bStop)
{
	//��������
	ZeroMemory(m_lStopUpdataScore,sizeof(m_lStopUpdataScore));

	//������Ŀ
	if(bStop)
	{
		//��ȡ�û�
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//��������
			const tagUserData * pUserData=GetUserInfo(i);
			if(pUserData!=NULL)
			{
				m_lStopUpdataScore[i]=pUserData->lScore;
			}
		}
	}

	//���½���
	UpdateGameView(NULL);

	return;
}

//�滭�˿�
void CGameClientView::DrawCompareCard(CDC * pDC, INT nXPos, INT nYPos, bool bChapped)
{
	//������Դ
	CImageHandle HandleGameCard(&m_ImageCard);
	CSize SizeGameCard(m_ImageCard.GetWidth()/13,m_ImageCard.GetHeight()/5);

	//����λ��
	nYPos-=SizeGameCard.cy/2;
	nXPos-=(SizeGameCard.cx+DEFAULT_PELS*(MAX_COUNT-1))/2;

	//�滭�˿�
	for (BYTE i=0;i<MAX_COUNT;i++)
	{
		INT nXDrawPos=nXPos+DEFAULT_PELS*i;
		m_ImageCard.AlphaDrawImage(pDC,nXDrawPos,nYPos,SizeGameCard.cx,SizeGameCard.cy,
			SizeGameCard.cx*((bChapped==true)?4:2),SizeGameCard.cy*4,RGB(255,0,255));
	}

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

//��������
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect)
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	pDC->SetTextColor(crFrame);
	CRect rcDraw;
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		rcDraw.CopyRect(lpRect);
		rcDraw.OffsetRect(nXExcursion[i],nYExcursion[i]);
		pDC->DrawText(pszString,nStringLength,&rcDraw,DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	}

	//�滭����
	rcDraw.CopyRect(lpRect);
	pDC->SetTextColor(crText);
	pDC->DrawText(pszString,nStringLength,&rcDraw,DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	return;
}

//���ƶ���
void CGameClientView::PerformCompareCard(WORD wCompareUser[2], WORD wLoserUser)
{
	//Ч��״̬
	ASSERT(m_wLoserUser==INVALID_CHAIR);
	if (m_wLoserUser!=INVALID_CHAIR) return;

	//���ñ���
	m_wConmareIndex=0;
	m_wLoserUser=wLoserUser;
	m_wConmareCount=LESS_SEND_COUNT;

	//�Ա��û�
	m_wCompareChairID[0]=wCompareUser[0];
	m_wCompareChairID[1]=wCompareUser[1];

	//��������
	for (BYTE i=0;i<2;i++)
	{
		//λ�Ƽ���
		INT nXCount=abs(m_ptUserCard[wCompareUser[i]].x-m_ptUserCompare.x)/SEND_PELS;
		INT nYCount=abs(m_ptUserCard[wCompareUser[i]].y-m_ptUserCompare.y)/SEND_PELS;

		//��������
		m_wConmareCount=__max(m_wConmareCount,__max(nXCount,nYCount));
	}

	//�����˿�
	m_CardControl[wCompareUser[0]].SetCardData(NULL,0);
	m_CardControl[wCompareUser[1]].SetCardData(NULL,0);

	//����ʱ��
	if (!m_bCartoon)
	{
		m_bCartoon=true;
		SetTimer(IDI_CARTOON,TIME_CARTOON,NULL);
	}

	//����Ч��
	CGameClientDlg * pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);
	pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("CENTER_SEND_CARD"));
	return;
}

//���ƶ���
void CGameClientView::DispatchUserCard(WORD wChairID, BYTE cbCardData)
{
	//Ч�����
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return;

	//���ñ���
	tagSendCardItem SendCardItem;
	SendCardItem.wChairID=wChairID;
	SendCardItem.cbCardData=cbCardData;
	m_SendCardItemArray.Add(SendCardItem);

	//��������
	if (m_SendCardItemArray.GetCount()==1) 
	{
		//��ȡλ��
		CRect rcClient;
		GetClientRect(&rcClient);

		//����λ�� /2
		m_SendCardPos.x=rcClient.Width()/2+200;
		m_SendCardPos.y=rcClient.Height()/2-308;
		m_KeepPos.x =m_SendCardPos.x;
		m_KeepPos.y=m_SendCardPos.y;

		if(!m_bCartoon)
		{
			m_bCartoon = true;
			//���ö�ʱ��
			SetTimer(IDI_CARTOON,TIME_CARTOON,NULL);
		}

		//��������
		CGameClientDlg * pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);
		pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
	}

	return;
}

//���ƶ���
bool CGameClientView::bFalshCard(WORD wFalshUser[])
{
	for(int i=0;i<GAME_PLAYER;i++)
	{
		if(wFalshUser[i]<GAME_PLAYER)m_wFlashUser[i] = wFalshUser[i];
	}

	//���ö�ʱ��
	m_bStopDraw=true;

	if(!m_bCartoon)
	{
		m_bCartoon = true;
		SetTimer(IDI_CARTOON,TIME_CARTOON,NULL);
	}

	return true;
}

//���ƴ���
bool CGameClientView::SendCard()
{
	//�����ж�
	if (m_SendCardItemArray.GetCount()==0) return false;

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	int nXExcursion[GAME_PLAYER]={-1,1,-1,-1,-1};
	int nYExcursion[GAME_PLAYER]={0,1,1,1,1};

	//����λ��
	tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];
	WORD bID=pSendCardItem->wChairID;
	BYTE bTimes;
	if(bID==0)bTimes=4;
	else if(bID==1)bTimes=3;
	else if(bID==2)bTimes=2;
	else if(bID>=3)bTimes=2;

	m_SendCardPos.x+=nXExcursion[bID]*(abs(m_ptUserCard[bID].x-m_KeepPos.x)/SPEED*bTimes);
	m_SendCardPos.y+=nYExcursion[bID]*(abs(m_ptUserCard[bID].y-m_KeepPos.y)/SPEED*bTimes);

	//���½���
	UpdateGameView(&rcClient);

	//ֹͣ�ж�
	int bTempX=0,bTempY=0;
	bTempX=m_ptUserCard[bID].x;
	bTempY=m_ptUserCard[bID].y;
	if((pSendCardItem->wChairID==0 && m_SendCardPos.x<=bTempX)||(pSendCardItem->wChairID>=1 && m_SendCardPos.y>=bTempY))
	{
		//��ȡ�˿�
		BYTE cbCardData[MAX_COUNT];
		WORD wChairID=pSendCardItem->wChairID;
		BYTE cbCardCount=(BYTE)m_CardControl[wChairID].GetCardData(cbCardData,CountArray(cbCardData));

		//�����˿�&& pSendCardItem->cbCardData!=0
		if (cbCardCount<=MAX_COUNT)
		{
			cbCardData[cbCardCount++]=pSendCardItem->cbCardData;
			m_CardControl[wChairID].SetCardData(cbCardData,cbCardCount);
		}

		//ɾ������
		m_SendCardItemArray.RemoveAt(0);

		//��������
		if (m_SendCardItemArray.GetCount()>0)
		{
			//����λ�� /2
			GetClientRect(&rcClient);
			m_SendCardPos.x=rcClient.Width()/2+200;
			m_SendCardPos.y=rcClient.Height()/2-308;

			//��������
			CGameClientDlg * pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);
			pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));

			return true;
		}
		else
		{
			//��ɴ���
			AfxGetMainWnd()->PostMessage(IDM_SEND_CARD_FINISH,0,0);

			return false;
		}
	}

	return true;
}

//���ƴ���
bool CGameClientView::FlashCard()
{	
	//�����ж�
	for(int i=0;i<GAME_PLAYER;i++) if(m_wFlashUser[i]<GAME_PLAYER)break;
	if(i==GAME_PLAYER)return false;

	//���ñ���
	m_wFalshCount++;
	WORD wTimes=m_wFalshCount*TIME_CARTOON;

	//���½���
	UpdateGameView(NULL);
	m_bFalsh=!m_bFalsh;

	//�˿˱���
	WORD wValue=(rand()%2==0)?((m_wFalshCount+1)%6):((m_wFalshCount+2)%6);
	for(int i=0;i<GAME_PLAYER;i++)
	{
		if(m_wFlashUser[i]<GAME_PLAYER)m_CardControl[m_wFlashUser[i]].SetCardColor(wValue);
	}

	//ֹͣ����
	if(2200 <= wTimes)
	{
		//���ò���
		m_bFalsh=false;
		m_bStopDraw=false;
		m_wFalshCount=0;
		for(int i=0;i<GAME_PLAYER;i++)
		{
			if(m_wFlashUser[i]<GAME_PLAYER)
			{
				m_CardControl[m_wFlashUser[i]].SetCardColor(INVALID_CHAIR);
				m_wFlashUser[i]=INVALID_CHAIR;
			}
		}

		//��ɴ���
		AfxGetMainWnd()->PostMessage(IDM_FALSH_CARD_FINISH,0,0);

		//���½���
		UpdateGameView(NULL);

		return false; 
	}

	return true; 
}

//���ƴ���
bool CGameClientView::CompareCard()
{
	//�����ж�
	if (m_wLoserUser==INVALID_CHAIR) return false;

	//��������
	m_wConmareIndex++;

	//���½���
	UpdateGameView(NULL);

	//ֹͣ�ж�
	if (m_wConmareIndex>=((m_wConmareCount*2)+VSCARD_COUNT*VSCARD_TIME))
	{
		//�����˿�
		BYTE bTemp[]={0,0,0};
		m_CardControl[m_wLoserUser].SetCardColor(2);
		m_CardControl[m_wCompareChairID[0]].SetCardData(bTemp,MAX_COUNT);
		m_CardControl[m_wCompareChairID[1]].SetCardData(bTemp,MAX_COUNT);

		//���ñ���
		m_wConmareIndex=0;
		m_wConmareCount=0;
		m_wLoserUser=INVALID_CHAIR;
		ZeroMemory(m_wCompareChairID,sizeof(m_wCompareChairID));

		//������Ϣ
		AfxGetMainWnd()->PostMessage(IDM_FALSH_CARD_FINISH,0,0);

		return false;
	}

	return true;
}

//��ʱ����Ϣ
void CGameClientView::OnTimer(UINT nIDEvent)
{
	if (nIDEvent==IDI_CARTOON)		//������ʱ
	{
		//��������
		bool bKillTimer=true;

		//���ƶ���
		if(m_bStopDraw && (FlashCard()==true))
		{
			bKillTimer=false;
		}

		//���ƶ���
		if ((m_wLoserUser!=INVALID_CHAIR)&&(CompareCard()==true))
		{
			bKillTimer=false;
		}

		//���ƶ���
		if ((m_SendCardItemArray.GetCount()>0)&&(SendCard()==true))
		{
			bKillTimer=false;
		}

		//ɾ��ʱ��
		if (bKillTimer==true)
		{
			m_bCartoon=false;
			KillTimer(nIDEvent);
		}

		return;
	}
	

	__super::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////

