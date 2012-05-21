#include "StdAfx.h"
#include "Resource.h"
#include "GameClientDlg.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//��������
#define MAX_COUNT					3									//�������

//ʱ�䶨��
#define IDI_WAITING					99									//ͣ����ʱ��

//���Ʊ���
#define SEND_PELS					30									//��������
#define SEND_SPACE					20									//���Ƽ��
#define IDI_SEND_CARD				100									//���ƶ�ʱ��

//���ձ���
#define RECLAIM_PELS				30									//��������
#define RECLAIM_SPACE				20									//���Ƽ��
#define IDI_RECLAIM_CARD			101									//���ƶ�ʱ��

//���ճ���
#define SCORE_PELS					30									//��������
#define SCORE_COUNT					10									//���Ƽ��
#define IDI_SCORE_CARD				102									//���ƶ�ʱ��

//���밴ť
#define IDC_BT_SCORE_1				100									//���밴ť
#define IDC_BT_SCORE_2				101									//���밴ť
#define IDC_BT_SCORE_3				102									//���밴ť
#define IDC_BT_SCORE_4				103									//���밴ť
#define IDC_BT_SCORE_5				104									//���밴ť

#define IDC_CONTROL_MAX				110

//���ܰ�ť
#define IDC_BT_START				200									//��ʼ��ť
#define IDC_BT_CLEAN				201									//�����ť
#define IDC_BT_CHIP_IN				202									//��ע��ť
#define IDC_BT_BANKER				203									//ׯ�Ұ�ť
#define IDC_BT_NO_BANKER			204									//������ť

//����λ��
#define MYSELF_CHAIR_ID				3									//�Լ�λ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)

	//ϵͳ��Ϣ
	ON_WM_TIMER()
	ON_WM_CREATE()

	//���ܰ�ť
	ON_BN_CLICKED(IDC_BT_START, OnStart)
	ON_BN_CLICKED(IDC_BT_CLEAN, OnClean)
	ON_BN_CLICKED(IDC_BT_BANKER, OnBanker)
	ON_BN_CLICKED(IDC_BT_CHIP_IN, OnChipin)
	ON_BN_CLICKED(IDC_BT_NO_BANKER, OnNoBanker)

	//���밴ť
	ON_BN_CLICKED(IDC_BT_SCORE_1, OnScore1)
	ON_BN_CLICKED(IDC_BT_SCORE_2, OnScore2)
	ON_BN_CLICKED(IDC_BT_SCORE_3, OnScore3)
	ON_BN_CLICKED(IDC_BT_SCORE_4, OnScore4)
	ON_BN_CLICKED(IDC_BT_SCORE_5, OnScore5)
	ON_BN_CLICKED(IDC_CONTROL_MAX,ControlMax)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView(void) : CGameFrameView(true,32)
{
	//�ȴ�����
	ZeroMemory(m_wUserPost,sizeof(m_wUserPost));

	//��ʷ��¼
	m_wDrawCount=0;
	m_lGameScore=0L;

	//��ע��Ϣ
	m_lMaxScore=0L;
	m_lLessScore=0L;

	//�������
	m_wMeChairID=INVALID_CHAIR;
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_wChairIndex,sizeof(m_wChairIndex));
	ZeroMemory(m_bBankerScore,sizeof(m_bBankerScore));

	//״̬����
	m_bLongBanker=false;
	m_bTakeScore=false;
	m_bShowType=false;
	m_bWaitBanker=false;
	m_bWaitPlayer=false;
	ZeroMemory(m_szCardType,sizeof(m_szCardType));

	//���Ʊ���
	m_wSendIndex=0;
	m_wSendCount=0;
	m_SendCardPos.SetPoint(0,0);

	//���Ʊ���
	m_wReclaimCount=0;
	m_wReclaimIndex=0;
	m_bReclaimIng=false;
	m_ReclaimCardPos.SetPoint(0,0);
	m_ReclaimStartPos.SetPoint(0,0);
	m_ReclaimEndingPos.SetPoint(0,0);

	//λ�ñ���
	ZeroMemory(m_GoldStation,sizeof(m_GoldStation));

	//������Դ
	m_ImageCard.SetLoadInfo(IDB_GAME_CARD,AfxGetInstanceHandle());
	m_ImageCards.SetLoadInfo(IDB_CARD_BACK,AfxGetInstanceHandle());
	m_ImageBanker.SetLoadInfo(IDB_BANKER_FLAG,AfxGetInstanceHandle());
	m_ImageLongBanker.SetLoadInfo(IDB_LONG_BANKER,AfxGetInstanceHandle());
	m_ImageDrawInfo.SetLoadInfo(IDB_DRAW_INFO,AfxGetInstanceHandle());
	m_ImageViewFill.SetLoadInfo(IDB_VIEW_FILL,AfxGetInstanceHandle());
	m_ImageWaitBanker.SetLoadInfo(IDB_WAIT_BANKER,AfxGetInstanceHandle());
	m_ImageWaitPlayer.SetLoadInfo(IDB_WAIT_PLAYER,AfxGetInstanceHandle());
	m_ImageWaitEnter.SetLoadInfo(IDB_WAIT_ENTER,AfxGetInstanceHandle());
	m_ImageWaitContinue.SetLoadInfo(IDB_WAIT_CONTINUE,AfxGetInstanceHandle());
	m_ImageViewBanker.SetLoadInfo(IDB_VIEW_BACK_BANKER,AfxGetInstanceHandle());
	m_ImageViewPlayer.SetLoadInfo(IDB_VIEW_BACK_PLAYER,AfxGetInstanceHandle());
	m_ImageWait.SetLoadInfo(IDB_WAITING_TIPS,AfxGetInstanceHandle());
	m_ImageUserFlag.SetLoadInfo(IDB_USER_FLAG,AfxGetInstanceHandle());

	return;
}

//��������
CGameClientView::~CGameClientView(void)
{
}

//��ʼ��ť
void CGameClientView::OnStart()
{
	AfxGetMainWnd()->SendMessage(IDM_START,0,0);
	return;
}

//���밴ť
void CGameClientView::OnScore1()
{
	AfxGetMainWnd()->SendMessage(IDM_SCORE,0,0);
	return;
}

//���밴ť
void CGameClientView::OnScore2()
{
	AfxGetMainWnd()->SendMessage(IDM_SCORE,1,1);
	return;
}

//���밴ť
void CGameClientView::OnScore3()
{
	AfxGetMainWnd()->SendMessage(IDM_SCORE,2,2);
	return;
}

//���밴ť
void CGameClientView::OnScore4()
{
	AfxGetMainWnd()->SendMessage(IDM_SCORE,3,3);
	return;
}

//���밴ť
void CGameClientView::OnScore5()
{
	AfxGetMainWnd()->SendMessage(IDM_SCORE,4,4);
	return;
}

//�����ť
void CGameClientView::OnClean()
{
	AfxGetMainWnd()->SendMessage(IDM_CLEAN,0,0);
	return;
}

//��ע��ť
void CGameClientView::OnChipin()
{
	AfxGetMainWnd()->SendMessage(IDM_CHIP_IN,0,0);
	return;
}

//��ׯ��ť
void CGameClientView::OnBanker()
{
	AfxGetMainWnd()->SendMessage(IDM_BANKER,1,1);
	return;
}

//������ť
void CGameClientView::OnNoBanker()
{
	AfxGetMainWnd()->SendMessage(IDM_BANKER,0,0);
	return;
}

//ʱ����Ϣ
void CGameClientView::OnTimer(UINT nIDEvent)
{
	//���ʹ���
	if (nIDEvent==IDI_SEND_CARD)
	{
		//��������
		m_wSendIndex++;

		//����λ��
		tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];
		m_SendCardPos.x=m_SendStartPos.x+(m_SendEndingPos[pSendCardItem->wChairID].x-m_SendStartPos.x)*m_wSendIndex/m_wSendCount;
		m_SendCardPos.y=m_SendStartPos.y+(m_SendEndingPos[pSendCardItem->wChairID].y-m_SendStartPos.y)*m_wSendIndex/m_wSendCount;

		//���½���
		UpdateGameView(NULL);

		//ֹͣ�ж�
		if (m_wSendIndex>=m_wSendCount)
		{
			//��ȡ�˿�
			BYTE cbCardData[MAX_COUNT];
			WORD wChairID=pSendCardItem->wChairID;
			BYTE cbCardCount=(BYTE)m_CardControl[wChairID].GetCardData(cbCardData,CountArray(cbCardData));

			//�����˿�
			if (cbCardCount<MAX_COUNT)
			{
				cbCardData[cbCardCount++]=pSendCardItem->cbCardData;
				m_CardControl[wChairID].SetCardData(cbCardData,cbCardCount);
			}

			//ɾ������
			m_SendCardItemArray.RemoveAt(0);

			//��������
			if (m_SendCardItemArray.GetCount()>0)
			{
				//��������
				tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];

				//����λ��
				m_SendCardPos.x=m_SendStartPos.x;
				m_SendCardPos.y=m_SendStartPos.y;

				//λ�Ƽ���
				int nXCount=abs(m_SendEndingPos[pSendCardItem->wChairID].x-m_SendStartPos.x)/SEND_PELS;
				int nYCount=abs(m_SendEndingPos[pSendCardItem->wChairID].y-m_SendStartPos.y)/SEND_PELS;

				//���ô���
				m_wSendIndex=0;
				m_wSendCount=__max(nXCount,nYCount);

				//��������
				CGameClientDlg * pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);
				pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));

				return;
			}
			else 
			{
				//��ɴ���
				KillTimer(IDI_SEND_CARD);
				AfxGetMainWnd()->PostMessage(IDM_SEND_CARD_FINISH,0,0);

				return;
			}
		}

		return;
	}

	//�����˿�
	if (nIDEvent==IDI_RECLAIM_CARD)
	{
		//����λ��
		if (m_wReclaimIndex<m_wReclaimCount)
		{
			//��������
			m_wReclaimIndex++;

			//����λ��
			m_ReclaimCardPos.x=m_ReclaimStartPos.x+(m_ReclaimEndingPos.x-m_ReclaimStartPos.x)*m_wReclaimIndex/m_wReclaimCount;
			m_ReclaimCardPos.y=m_ReclaimStartPos.y+(m_ReclaimEndingPos.y-m_ReclaimStartPos.y)*m_wReclaimIndex/m_wReclaimCount;

			//���½���
			UpdateGameView(NULL);

			return;
		}

		//��ʼ����
		WORD wStartIndex=0;
		if (m_wBankerUser==m_wMeChairID) wStartIndex=3;

		//Ѱ���˿�
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//��ȡ����
			WORD wIndex=(wStartIndex+i)%GAME_PLAYER;

			//��ȡ�˿�
			BYTE cbCardData[MAX_COUNT];
			WORD wCardCount=m_CardControl[wIndex].GetCardData(cbCardData,CountArray(cbCardData));

			//�����ж�
			if (wCardCount>0)
			{
				//�����˿�
				m_CardControl[wIndex].SetCardData(NULL,0);

				//����λ��
				m_ReclaimCardPos.x=m_SendEndingPos[wIndex].x;
				m_ReclaimCardPos.y=m_SendEndingPos[wIndex].y;
				m_ReclaimStartPos.x=m_SendEndingPos[wIndex].x;
				m_ReclaimStartPos.y=m_SendEndingPos[wIndex].y;

				//λ�Ƽ���
				int nXCount=abs(m_ReclaimEndingPos.x-m_ReclaimStartPos.x)/RECLAIM_PELS;
				int nYCount=abs(m_ReclaimEndingPos.y-m_ReclaimStartPos.y)/RECLAIM_PELS;

				//���ô���
				m_wReclaimIndex=0;
				m_wReclaimCount=__max(nXCount,nYCount);

				//���½���
				UpdateGameView(NULL);

				return;
			}
		}

		//����ж�
		if (i==GAME_PLAYER) 
		{
			//���ñ���
			m_bReclaimIng=false;

			//ɾ��ʱ��
			KillTimer(IDI_RECLAIM_CARD);

			//���½���
			UpdateGameView(NULL);

			//������Ϣ
			AfxGetMainWnd()->PostMessage(IDM_RECLAIM_CARD_FINISH,0,0);
		}

		return;
	}

	//�������
	if (nIDEvent==IDI_SCORE_CARD)
	{
		if (m_wScoreIndex<SCORE_COUNT)
		{
			//��������
			m_wScoreIndex++;

			//ׯ��λ��
			WORD wBankerIndex=0;
			if (m_wBankerUser==m_wMeChairID) wBankerIndex=3;
			CPoint ScoreBanker=m_CardControl[wBankerIndex].GetBenchmarkPos();

			//�ƶ�λ��
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				LONG lScore=m_GoldView[i].GetGold();
				if (lScore==0L) continue;

				if (m_bBankerScore[i]==true)
				{
					m_GoldTarget[i].x=(ScoreBanker.x-m_GoldStation[i].x)*m_wScoreIndex/SCORE_COUNT+m_GoldStation[i].x;
					m_GoldTarget[i].y=(ScoreBanker.y-m_GoldStation[i].y)*m_wScoreIndex/SCORE_COUNT+m_GoldStation[i].y;
				}
				else
				{
					CPoint ScorePlayer=m_CardControl[i].GetBenchmarkPos();
					m_GoldTarget[i].x=(ScorePlayer.x-m_GoldStation[i].x)*m_wScoreIndex/SCORE_COUNT+m_GoldStation[i].x;
					m_GoldTarget[i].y=(ScorePlayer.y-m_GoldStation[i].y)*m_wScoreIndex/SCORE_COUNT+m_GoldStation[i].y;
				}
			}

			//���½���
			UpdateGameView(NULL);
		}
		else
		{
			m_wScoreIndex=0;
			m_bTakeScore=false;
			ZeroMemory(m_bBankerScore,sizeof(m_bBankerScore));
			KillTimer(IDI_SCORE_CARD);

			//���ó���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GoldView[i].SetGold(0);
			}

			//��ʾ�ɼ�
			m_ScoreView.ShowScore();

			//���½���
			UpdateGameView(NULL);
		}

		return;
	}

	//�ȴ���ʼ
	if(IDI_WAITING==nIDEvent)
	{
		m_bDrawTimes++;

		//���½���
		UpdateGameView(NULL);

		return ;
	}

	__super::OnTimer(nIDEvent);
}

//������Ϣ
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//�����ؼ�
	CRect rcCreate(0,0,0,0);
	for (int i=0;i<GAME_PLAYER;i++) m_CardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,rcCreate,this,20+i);

	//��ʼ��ť
	m_btStart.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_BT_START);
	m_btStart.SetButtonImage(IDB_BT_START,AfxGetInstanceHandle(),false);

	//�����ť
	m_btClean.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_BT_CLEAN);
	m_btClean.SetButtonImage(IDB_BT_CLEAN,AfxGetInstanceHandle(),false);

	//��ע��ť
	m_btChipIn.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_BT_CHIP_IN);
	m_btChipIn.SetButtonImage(IDB_BT_CHIP_IN,AfxGetInstanceHandle(),false);

	//��ׯ��ť
	m_btBanker.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_BT_BANKER);
	m_btBanker.SetButtonImage(IDB_BT_BANKER,AfxGetInstanceHandle(),false);

	//������ť
	m_btNoBanker.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_BT_NO_BANKER);
	m_btNoBanker.SetButtonImage(IDB_BT_NO_BANKER,AfxGetInstanceHandle(),false);

	//���밴ť
	m_btScore1.Create(TEXT(""),WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_BT_SCORE_1);
	m_btScore2.Create(TEXT(""),WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_BT_SCORE_2);
	m_btScore3.Create(TEXT(""),WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_BT_SCORE_3);
	m_btScore4.Create(TEXT(""),WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_BT_SCORE_4);
	m_btScore5.Create(TEXT(""),WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_BT_SCORE_5);

	//���ð�ť
	m_btScore1.SetButtonImage(IDB_BT_SCORE_1,AfxGetInstanceHandle(),false);
	m_btScore2.SetButtonImage(IDB_BT_SCORE_2,AfxGetInstanceHandle(),false);
	m_btScore3.SetButtonImage(IDB_BT_SCORE_3,AfxGetInstanceHandle(),false);
	m_btScore4.SetButtonImage(IDB_BT_SCORE_4,AfxGetInstanceHandle(),false);
	m_btScore5.SetButtonImage(IDB_BT_SCORE_5,AfxGetInstanceHandle(),false);

	m_btControlMax.Create(TEXT("�������"),WS_CHILD|WS_DISABLED,CRect(10,10,70,30),this,IDC_CONTROL_MAX);
	m_btControlMax.ShowWindow(SW_HIDE);

	return 0;
}

//���ý���
void CGameClientView::ResetGameView()
{
	ZeroMemory(m_wUserPost,sizeof(m_wUserPost));

	//��ע��Ϣ
	m_lMaxScore=0L;
	m_lLessScore=0L;

	//��ʷ��¼
	m_wDrawCount=0;
	m_lGameScore=0L;

	//״̬����
	m_bLongBanker=false;
	m_bShowType=false;
	m_bTakeScore=false;
	m_bWaitBanker=false;
	m_bWaitPlayer=false;
	ZeroMemory(m_szCardType,sizeof(m_szCardType));
	ZeroMemory(m_bBankerScore,sizeof(m_bBankerScore));

	//�������
	m_wMeChairID=INVALID_CHAIR;
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_wChairIndex,sizeof(m_wChairIndex));

	//���Ʊ���
	m_wSendIndex=0;
	m_wSendCount=0;
	m_SendCardPos.SetPoint(0,0);
	m_SendCardItemArray.RemoveAll();

	//���Ʊ���
	m_wReclaimCount=0;
	m_wReclaimIndex=0;
	m_bReclaimIng=false;
	m_ReclaimCardPos.SetPoint(0,0);
	m_ReclaimStartPos.SetPoint(0,0);
	m_ReclaimEndingPos.SetPoint(0,0);

	//���ÿؼ�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GoldView[i].SetGold(0L);
		m_CardControl[i].SetCardData(NULL,0);
	}

	//���ؿؼ�
	m_btStart.ShowWindow(SW_HIDE);
	m_btClean.ShowWindow(SW_HIDE);
	m_btChipIn.ShowWindow(SW_HIDE);
	m_btBanker.ShowWindow(SW_HIDE);
	m_btNoBanker.ShowWindow(SW_HIDE);

	//���ÿؼ�
	m_btScore1.EnableWindow(FALSE);
	m_btScore2.EnableWindow(FALSE);
	m_btScore3.EnableWindow(FALSE);
	m_btScore4.EnableWindow(FALSE);
	m_btScore5.EnableWindow(FALSE);

	//���ش���
	m_ScoreView.ResetScore();

	//ɾ��ʱ��
	KillTimer(IDI_SEND_CARD);
	KillTimer(IDI_SCORE_CARD);
	KillTimer(IDI_RECLAIM_CARD);

	return;
}

//�����ؼ�
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	//�ƶ��ؼ�
	if (m_wMeChairID!=m_wBankerUser)
	{
		//�û�λ��
		m_ptName[0].x=nWidth/2+5;
		m_ptName[0].y=nHeight/2-244;
		m_ptFace[0].x=nWidth/2-m_nXFace;
		m_ptFace[0].y=nHeight/2-248;

		//�û�λ��
		m_ptName[1].x=nWidth/2+370+m_nXFace;
		m_ptName[1].y=nHeight/2+37;
		m_ptFace[1].x=nWidth/2+365;
		m_ptFace[1].y=nHeight/2+33;

		//�û�λ��
		m_ptName[2].x=nWidth/2+185+m_nXFace;
		m_ptName[2].y=nHeight/2+146;
		m_ptFace[2].x=nWidth/2+180;
		m_ptFace[2].y=nHeight/2+142;

		//�û�λ��
		m_ptName[3].x=nWidth/2+5;
		m_ptName[3].y=nHeight/2+177;
		m_ptFace[3].x=nWidth/2-m_nXFace;
		m_ptFace[3].y=nHeight/2+173;

		//�û�λ��
		m_ptName[4].x=nWidth/2-260+m_nXFace;
		m_ptName[4].y=nHeight/2+146;
		m_ptFace[4].x=nWidth/2-265;
		m_ptFace[4].y=nHeight/2+142;

		//�û�λ��
		m_ptName[5].x=nWidth/2-445+m_nXFace;
		m_ptName[5].y=nHeight/2+37;
		m_ptFace[5].x=nWidth/2-450;
		m_ptFace[5].y=nHeight/2+33;

		//�˿�λ��
		m_SendStartPos.SetPoint(nWidth/2+370,nHeight/2-260);
		m_ReclaimEndingPos.SetPoint(nWidth/2-410,nHeight/2-290);

		//����λ��
		m_SendEndingPos[0].SetPoint(nWidth/2,nHeight/2-158);
		m_SendEndingPos[1].SetPoint(nWidth/2+406,nHeight/2-33);
		m_SendEndingPos[2].SetPoint(nWidth/2+223,nHeight/2+73);
		m_SendEndingPos[3].SetPoint(nWidth/2,nHeight/2+108);
		m_SendEndingPos[4].SetPoint(nWidth/2-223,nHeight/2+73);
		m_SendEndingPos[5].SetPoint(nWidth/2-406,nHeight/2-33);

		//�ƶ�׼��
		HDWP hDwp=BeginDeferWindowPos(32);
		const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

		//�ƶ�����
		DeferWindowPos(hDwp,m_btScore1,NULL,nWidth/2-489,nHeight/2+128,0,0,uFlags|SWP_SHOWWINDOW);
		DeferWindowPos(hDwp,m_btScore2,NULL,nWidth/2-420,nHeight/2+170,0,0,uFlags|SWP_SHOWWINDOW);
		DeferWindowPos(hDwp,m_btScore3,NULL,nWidth/2-350,nHeight/2+200,0,0,uFlags|SWP_SHOWWINDOW);
		DeferWindowPos(hDwp,m_btScore4,NULL,nWidth/2-280,nHeight/2+224,0,0,uFlags|SWP_SHOWWINDOW);
		DeferWindowPos(hDwp,m_btScore5,NULL,nWidth/2-210,nHeight/2+240,0,0,uFlags|SWP_SHOWWINDOW);

		//��ʼ��ť
		DeferWindowPos(hDwp,m_btStart,NULL,nWidth/2+160,nHeight/2+130,0,0,uFlags);

		//��ע��ť
		DeferWindowPos(hDwp,m_btClean,NULL,nWidth/2+60,nHeight/2+267,0,0,uFlags);
		DeferWindowPos(hDwp,m_btChipIn,NULL,nWidth/2-30,nHeight/2+267,0,0,uFlags);

		//��ׯ��ť
		DeferWindowPos(hDwp,m_btBanker,NULL,nWidth/2-30,nHeight/2+267,0,0,uFlags);
		DeferWindowPos(hDwp,m_btNoBanker,NULL,nWidth/2+60,nHeight/2+267,0,0,uFlags);

		//�����ƶ�
		EndDeferWindowPos(hDwp);

		//�ƶ�����
		m_GoldStation[0].SetPoint(nWidth/2,nHeight/2-158);
		m_GoldStation[1].SetPoint(nWidth/2+315,nHeight/2-150);
		m_GoldStation[2].SetPoint(nWidth/2+170,nHeight/2-58);
		m_GoldStation[3].SetPoint(nWidth/2,nHeight/2-33);
		m_GoldStation[4].SetPoint(nWidth/2-170,nHeight/2-58);
		m_GoldStation[5].SetPoint(nWidth/2-315,nHeight/2-150);

		//�ƶ��˿�
		m_CardControl[0].SetBenchmarkPos(nWidth/2,nHeight/2-158,enXCenter,enYCenter);
		m_CardControl[1].SetBenchmarkPos(nWidth/2+406,nHeight/2-33,enXCenter,enYCenter);
		m_CardControl[2].SetBenchmarkPos(nWidth/2+223,nHeight/2+73,enXCenter,enYCenter);
		m_CardControl[3].SetBenchmarkPos(nWidth/2,nHeight/2+108,enXCenter,enYCenter);
		m_CardControl[4].SetBenchmarkPos(nWidth/2-223,nHeight/2+73,enXCenter,enYCenter);
		m_CardControl[5].SetBenchmarkPos(nWidth/2-406,nHeight/2-33,enXCenter,enYCenter);
	}
	else
	{
		//�û�λ��
		m_ptName[0].x=nWidth/2+5;
		m_ptName[0].y=nHeight/2-200;
		m_ptFace[0].x=nWidth/2-m_nXFace;
		m_ptFace[0].y=nHeight/2-204;

		//�û�λ��
		m_ptName[1].x=nWidth/2+185+m_nXFace;
		m_ptName[1].y=nHeight/2-166;
		m_ptFace[1].x=nWidth/2+180;
		m_ptFace[1].y=nHeight/2-170;

		//�û�λ��
		m_ptName[2].x=nWidth/2+370+m_nXFace;
		m_ptName[2].y=nHeight/2-61;
		m_ptFace[2].x=nWidth/2+365;
		m_ptFace[2].y=nHeight/2-65;

		//�û�λ��
		m_ptName[3].x=nWidth/2+5;
		m_ptName[3].y=nHeight/2+219;
		m_ptFace[3].x=nWidth/2-m_nXFace;
		m_ptFace[3].y=nHeight/2+215;

		//�û�λ��
		m_ptName[4].x=nWidth/2-445+m_nXFace;
		m_ptName[4].y=nHeight/2-61;
		m_ptFace[4].x=nWidth/2-450;
		m_ptFace[4].y=nHeight/2-65;

		//�û�λ��
		m_ptName[5].x=nWidth/2-260+m_nXFace;
		m_ptName[5].y=nHeight/2-166;
		m_ptFace[5].x=nWidth/2-265;
		m_ptFace[5].y=nHeight/2-170;

		//�˿�λ��
		m_SendStartPos.SetPoint(nWidth/2-370,nHeight/2+260);
		m_ReclaimEndingPos.SetPoint(nWidth/2+410,nHeight/2+290);

		//����λ��
		m_SendEndingPos[0].SetPoint(nWidth/2,nHeight/2-108);
		m_SendEndingPos[1].SetPoint(nWidth/2+223,nHeight/2-73);
		m_SendEndingPos[2].SetPoint(nWidth/2+406,nHeight/2+33);
		m_SendEndingPos[3].SetPoint(nWidth/2,nHeight/2+158);
		m_SendEndingPos[4].SetPoint(nWidth/2-406,nHeight/2+33);
		m_SendEndingPos[5].SetPoint(nWidth/2-223,nHeight/2-73);

		//�ƶ�׼��
		HDWP hDwp=BeginDeferWindowPos(32);
		const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

		//�ƶ�����
		DeferWindowPos(hDwp,m_btScore1,NULL,0,0,0,0,uFlags|SWP_HIDEWINDOW);
		DeferWindowPos(hDwp,m_btScore2,NULL,0,0,0,0,uFlags|SWP_HIDEWINDOW);
		DeferWindowPos(hDwp,m_btScore3,NULL,0,0,0,0,uFlags|SWP_HIDEWINDOW);
		DeferWindowPos(hDwp,m_btScore4,NULL,0,0,0,0,uFlags|SWP_HIDEWINDOW);
		DeferWindowPos(hDwp,m_btScore5,NULL,0,0,0,0,uFlags|SWP_HIDEWINDOW);

		//��ʼ��ť
		DeferWindowPos(hDwp,m_btStart,NULL,nWidth/2+160,nHeight/2+130,0,0,uFlags);

		//��ע��ť
		DeferWindowPos(hDwp,m_btClean,NULL,nWidth/2-10,nHeight/2+150,0,0,uFlags);
		DeferWindowPos(hDwp,m_btChipIn,NULL,nWidth/2-100,nHeight/2+150,0,0,uFlags);

		//��ׯ��ť
		DeferWindowPos(hDwp,m_btBanker,NULL,nWidth/2-30,nHeight/2+267,0,0,uFlags);
		DeferWindowPos(hDwp,m_btNoBanker,NULL,nWidth/2+60,nHeight/2+267,0,0,uFlags);

		//�����ƶ�
		EndDeferWindowPos(hDwp);

		//�ƶ�����
		m_GoldStation[0].SetPoint(nWidth/2,nHeight/2+40);
		m_GoldStation[1].SetPoint(nWidth/2+170,nHeight/2+61);
		m_GoldStation[2].SetPoint(nWidth/2+315,nHeight/2+150);
		m_GoldStation[3].SetPoint(nWidth/2,nHeight/2+158);
		m_GoldStation[4].SetPoint(nWidth/2-315,nHeight/2+150);
		m_GoldStation[5].SetPoint(nWidth/2-170,nHeight/2+61);

		//�ƶ��˿�
		m_CardControl[0].SetBenchmarkPos(nWidth/2,nHeight/2-108,enXCenter,enYCenter);
		m_CardControl[1].SetBenchmarkPos(nWidth/2+223,nHeight/2-73,enXCenter,enYCenter);
		m_CardControl[2].SetBenchmarkPos(nWidth/2+406,nHeight/2+33,enXCenter,enYCenter);
		m_CardControl[3].SetBenchmarkPos(nWidth/2,nHeight/2+158,enXCenter,enYCenter);
		m_CardControl[4].SetBenchmarkPos(nWidth/2-406,nHeight/2+33,enXCenter,enYCenter);
		m_CardControl[5].SetBenchmarkPos(nWidth/2-223,nHeight/2-73,enXCenter,enYCenter);
	}

	//��������
	SetFlowerControlInfo(0,m_ptFace[0].x-BIG_FACE_WIDTH/2,m_ptFace[0].y);
	SetFlowerControlInfo(1,m_ptFace[1].x-BIG_FACE_WIDTH/2,m_ptFace[1].y);
	SetFlowerControlInfo(2,m_ptFace[2].x-BIG_FACE_WIDTH/2,m_ptFace[2].y);
	SetFlowerControlInfo(3,m_ptFace[3].x-BIG_FACE_WIDTH/2,m_ptFace[3].y);
	SetFlowerControlInfo(4,m_ptFace[4].x,m_ptFace[4].y					);
	SetFlowerControlInfo(5,m_ptFace[5].x-BIG_FACE_WIDTH/2,m_ptFace[5].y);
	return;
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//�滭����
	DrawViewImage(pDC,m_ImageViewFill,enMode_Spread);

	//�滭����
	if (m_wMeChairID!=INVALID_CHAIR)
	{
		if (m_wMeChairID==m_wBankerUser) DrawViewImage(pDC,m_ImageViewBanker,enMode_Centent);
		if (m_wMeChairID!=m_wBankerUser) DrawViewImage(pDC,m_ImageViewPlayer,enMode_Centent);
	}

	//�ܹ���ע
	LONG lSumScore=0L;
	for (WORD i=0;i<GAME_PLAYER;i++) lSumScore+=m_GoldView[i].GetGold();

	//��ע��Ϣ
	if (m_lMaxScore!=0L)
	{
		//������Ϣ
		TCHAR szBuffer[128]=TEXT("");
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("�����ע����%ld"),m_lMaxScore);

		//�滭��Ϣ
		if (m_wMeChairID!=m_wBankerUser)
		{
			pDC->SetTextColor(RGB(0,0,0));
			pDC->TextOut(nWidth/2-350,nHeight/2-335,szBuffer,lstrlen(szBuffer));
		}
		else
		{
			pDC->SetTextColor(RGB(0,0,0));
			pDC->TextOut(nWidth/2+243,nHeight/2+295,szBuffer,lstrlen(szBuffer));
		}
	}
	if (m_lLessScore!=0L)
	{
		//������Ϣ
		TCHAR szBuffer[128]=TEXT("");
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("��С��ע����%ld"),m_lLessScore);

		//�滭��Ϣ
		if (m_wMeChairID!=m_wBankerUser)
		{
			pDC->SetTextColor(RGB(0,0,0));
			pDC->TextOut(nWidth/2-350,nHeight/2-310,szBuffer,lstrlen(szBuffer));
		}
		else
		{
			pDC->SetTextColor(RGB(0,0,0));
			pDC->TextOut(nWidth/2+243,nHeight/2+312,szBuffer,lstrlen(szBuffer));
		}
	}
	if ((m_wMeChairID==m_wBankerUser)&&((m_lLessScore!=0L)||(m_lMaxScore!=0L)))
	{
		//������Ϣ
		TCHAR szBuffer[128]=TEXT("");
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("�ܹ���ע����%ld"),lSumScore);

		//�滭��Ϣ
		pDC->SetTextColor(RGB(0,0,0));
		pDC->TextOut(nWidth/2+243,nHeight/2+330,szBuffer,lstrlen(szBuffer));
	}

	//�滭����
	for (DWORD i=0;i<CountArray(m_GoldView);i++)
	{
		if (m_bTakeScore==false)
		{
			m_GoldView[i].DrawGoldView(pDC,m_GoldStation[i].x,m_GoldStation[i].y,true);
		}
		else
		{
			m_GoldView[i].DrawGoldView(pDC,m_GoldTarget[i].x,m_GoldTarget[i].y,false);
		}
	}

	//ׯ�ұ�־
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		CImageHandle HandleBanker(&m_ImageBanker);
		CImageHandle HandleLongBanker(&m_ImageLongBanker);
		if (m_wBankerUser==m_wMeChairID) 
		{
			if(!m_bLongBanker)	//��ׯ
			{
				m_ImageBanker.BitBlt(pDC->m_hDC,nWidth/2-110,nHeight/2+217);
			}
			else				//��ׯ
			{
				m_ImageBanker.BitBlt(pDC->m_hDC,nWidth/2-110,nHeight/2+217);
				//m_ImageLongBanker.BitBlt(pDC->m_hDC,nWidth/2-110,nHeight/2+217);
			}
		}
		else 
		{
			if(!m_bLongBanker)	//��ׯ
			{
				m_ImageBanker.BitBlt(pDC->m_hDC,nWidth/2-110,nHeight/2-245);
			}
			else				//��ׯ
			{
				m_ImageBanker.BitBlt(pDC->m_hDC,nWidth/2-110,nHeight/2-245);
				//m_ImageLongBanker.BitBlt(pDC->m_hDC,nWidth/2-110,nHeight/2-245);
			}
		}
	}

	//�û�ͳ��
	WORD wUserCount=0;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		const tagUserData * pUserData=GetUserInfo(i);
		if (pUserData!=NULL) wUserCount++;
	}

	//�����־
	if ((wUserCount==1)&&(m_wBankerUser==m_wMeChairID))
	{
		CImageHandle HandleWaitEnter(&m_ImageWaitEnter);
		m_ImageWaitEnter.BitBlt(pDC->m_hDC,(nWidth-m_ImageWaitEnter.GetWidth())/2,nHeight/2-10);
	}

	//�ȴ���־
	if ((m_bWaitBanker==true)&&(wUserCount>1))
	{
		if (m_wBankerUser==m_wMeChairID) 
		{
			if (m_btStart.IsWindowVisible())
			{
				CImageHandle HandleWaitBanker(&m_ImageWaitBanker);
				m_ImageWaitBanker.BitBlt(pDC->m_hDC,(nWidth-m_ImageWaitBanker.GetWidth())/2,nHeight/2+80);
			}
			else
			{
				CImageHandle HandleWaitBanker(&m_ImageWaitContinue);
				m_ImageWaitContinue.BitBlt(pDC->m_hDC,(nWidth-m_ImageWaitContinue.GetWidth())/2,nHeight/2+80);
			}
		}
		else
		{
			CImageHandle HandleWaitBanker(&m_ImageWaitBanker);
			m_ImageWaitBanker.BitBlt(pDC->m_hDC,(nWidth-m_ImageWaitBanker.GetWidth())/2,nHeight/2-120);
		}
	}

	//�ȴ���־
	if ((m_bWaitPlayer==true)&&(wUserCount>1))
	{
		//�滭��־
		CImageHandle HandleWaitPlayer(&m_ImageWaitPlayer);
		if (m_wBankerUser==m_wMeChairID) m_ImageWaitPlayer.BitBlt(pDC->m_hDC,(nWidth-m_ImageWaitPlayer.GetWidth())/2,nHeight/2+80);
		else m_ImageWaitPlayer.BitBlt(pDC->m_hDC,(nWidth-m_ImageWaitPlayer.GetWidth())/2,nHeight/2-120);

		//�滭ʱ��
		if (m_wBankerUser==m_wMeChairID)
		{
			WORD wUserTimer=GetUserTimer(m_wBankerUser);
			if (wUserTimer!=0L) DrawUserTimer(pDC,(nWidth+m_ImageWaitPlayer.GetWidth())/2-30,nHeight/2+94,wUserTimer);
		}
	}

	//�滭�û�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		const tagUserData * pUserData=GetUserInfo(i);

		//�滭�û�
		if (pUserData!=NULL)
		{
			//��ȡλ��
			WORD wViewFaceID=m_wChairIndex[i];
			pDC->SetTextAlign(TA_LEFT);
			DrawTextString(pDC,pUserData->szName,RGB(255,255,255),RGB(0,0,0),m_ptName[wViewFaceID].x,m_ptName[wViewFaceID].y);

			//�û����
			TCHAR szBuffer[64]=TEXT("");
			LONG lLeaveScore=pUserData->lScore-m_GoldView[wViewFaceID].GetGold();
			_snprintf(szBuffer,CountArray(szBuffer),TEXT("��%ld"),lLeaveScore);
			DrawTextString(pDC,szBuffer,RGB(255,255,255),RGB(0,0,0),m_ptName[wViewFaceID].x,m_ptName[wViewFaceID].y+15);

			//�û�ͷ��
			DrawUserFace(pDC,pUserData,m_ptFace[wViewFaceID].x,m_ptFace[wViewFaceID].y,false);

			//���ʱ��
			if ((m_wBankerUser!=m_wMeChairID)||(m_bWaitBanker==true))
			{
				WORD wUserTimer=GetUserTimer(i);
				if (wUserTimer!=0L) DrawUserTimer(pDC,m_ptFace[wViewFaceID].x-33+10,m_ptFace[wViewFaceID].y+6,wUserTimer);
			}
		}
	}

	//�滭����
	if (m_bShowType==true)
	{
		//��������
		CFont TypeFont;
		TypeFont.CreateFont(-14,0,0,0,700,0,0,0,134,3,2,1,2,TEXT("����"));

		//���� DC
		pDC->SelectObject(&TypeFont);
		//pDC->SetTextColor(RGB(250,200,40));

		//�滭��Ϣ
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if (m_szCardType[i][0]!=0)
			{
				CPoint Point=m_CardControl[i].GetBenchmarkPos();
				DrawTextString(pDC,m_szCardType[i],RGB(255,255,255),RGB(0,0,0),Point.x+68,Point.y-45);
			}
		}

		//���� DC
		pDC->SelectObject(CSkinResourceManager::GetDefaultFont());
	}

	//�����˿�
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
		int nYPos=m_SendCardPos.y-nItemHeight/2;
		m_ImageCard.AlphaDrawImage(pDC,nXPos,nYPos,nItemWidth,nItemHeight,nItemWidth*2,nItemHeight*4,RGB(255,0,255));
	}

	//�����˿�
	if (m_bReclaimIng==true)
	{
		//������Դ
		CImageHandle HandleCard(&m_ImageCards);

		//��ȡ��С
		int nItemWidth=m_ImageCards.GetWidth();
		int nItemHeight=m_ImageCards.GetHeight();

		//�滭�˿�
		int nXPos=m_ReclaimCardPos.x-nItemWidth/2;
		int nYPos=m_ReclaimCardPos.y-nItemHeight/2;
		m_ImageCards.AlphaDrawImage(pDC,nXPos,nYPos,RGB(255,0,255));
	}

	//��ʷ�ɼ�
	if (m_wDrawCount!=0)
	{
		//������Ϣ
		TCHAR szDrawCount[32]=TEXT(""),szGameScore[32]=TEXT("");
		_sntprintf(szDrawCount,CountArray(szDrawCount),TEXT("%ld"),m_wDrawCount);
		_sntprintf(szGameScore,CountArray(szGameScore),TEXT("%ld"),m_lGameScore);

		//�滭����
		if (m_wMeChairID==m_wBankerUser)
		{
			CImageHandle HandleDrawInfo(&m_ImageDrawInfo);
			m_ImageDrawInfo.AlphaDrawImage(pDC,m_nXBorder,m_nYBorder,RGB(255,0,255));
		}
		else
		{
			CImageHandle HandleDrawInfo(&m_ImageDrawInfo);
			m_ImageDrawInfo.AlphaDrawImage(pDC,nWidth-m_nXBorder-m_ImageDrawInfo.GetWidth(),nHeight-m_nYBorder-m_ImageDrawInfo.GetHeight(),RGB(255,0,255));
		}

		//���� DC
		pDC->SetTextColor(RGB(10,10,10));
		pDC->SetTextAlign(TA_LEFT|TA_TOP);

		//�滭��Ϣ
		if (m_wMeChairID==m_wBankerUser)
		{
			CRect rcDrawCount(m_nXBorder+10,m_nYBorder+44,m_nXBorder+91,m_nYBorder+66);
			CRect rcGameScore(m_nXBorder+94,m_nYBorder+44,m_nXBorder+188,m_nYBorder+66);
			pDC->DrawText(szDrawCount,lstrlen(szDrawCount),&rcDrawCount,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_SINGLELINE);
			pDC->DrawText(szGameScore,lstrlen(szGameScore),&rcGameScore,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_SINGLELINE);
		}
		else
		{
			CRect rcDrawCount(nWidth-m_nXBorder-188,nHeight-m_nYBorder-31,nWidth-m_nXBorder-106,nHeight-m_nYBorder-8);
			CRect rcGameScore(nWidth-m_nXBorder-101,nHeight-m_nYBorder-31,nWidth-m_nXBorder-10,nHeight-m_nYBorder-8);
			pDC->Draw3dRect(&rcDrawCount,0,0);
			pDC->Draw3dRect(&rcGameScore,0,0);
			pDC->DrawText(szDrawCount,lstrlen(szDrawCount),&rcDrawCount,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_SINGLELINE);
			pDC->DrawText(szGameScore,lstrlen(szGameScore),&rcGameScore,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_SINGLELINE);
		}
	}

	return;
}

//��ȡ����
LONG CGameClientView::GetUserClipInInfo(WORD wChairID)
{
	//��ȡ����
	WORD wChairIndex=m_wChairIndex[wChairID];
	LONG lScore=m_GoldView[wChairIndex].GetGold();

	return lScore;
}

//��ʾ����
void CGameClientView::SetCardType(bool bShowType)
{
	//���ñ���
	m_bShowType=bShowType;

	//���½���
	UpdateGameView(NULL);

	return;
}

//�ȴ�ׯ��
void CGameClientView::SetWaitBanker(bool bWaitBanker)
{
	//���ñ���
	m_bWaitBanker=bWaitBanker;

	//���½���
	UpdateGameView(NULL);

	return;
}

//�ȴ��м�
void CGameClientView::SetWaitPlayer(bool bWaitPlayer)
{
	//���ñ���
	m_bWaitPlayer=bWaitPlayer;

	//���½���
	UpdateGameView(NULL);

	return;
}

//��ע��Ϣ
void CGameClientView::SetChipInInfo(LONG lMaxScore, LONG lLessScore)
{
	//���ñ���
	m_lMaxScore=lMaxScore;
	m_lLessScore=lLessScore;

	//���½���
	UpdateGameView(NULL);

	return;
}

//������Ϣ
void CGameClientView::SetUserDrawInfo(WORD wDrawCount, LONG lGameScore)
{
	//���ñ���
	m_wDrawCount=wDrawCount;
	m_lGameScore=lGameScore;

	//���½���
	UpdateGameView(NULL);

	return;
}

//��������
void CGameClientView::SetCardTypeInfo(WORD wChairID, LPCTSTR pszCardType)
{
	//���ñ���
	lstrcpyn(m_szCardType[wChairID],pszCardType,CountArray(m_szCardType[wChairID]));

	//���½���
	if (m_bShowType==true) UpdateGameView(NULL);

	return;
}

//����λ��
void CGameClientView::SetUserChairInfo(WORD wBankerUser, WORD wMeChairID)
{
	//���ñ���
	m_wMeChairID=wMeChairID;
	m_wBankerUser=wBankerUser;

	//λ�ø���
	if ((m_wBankerUser!=INVALID_CHAIR)&&(m_wMeChairID!=INVALID_CHAIR))
	{
		if (m_wBankerUser!=m_wMeChairID)
		{
			//Ĭ��λ��
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_wChairIndex[i]=i;
			}

			//ʵ��λ��
			WORD wTemp = m_wUserPost[m_wBankerUser];
			m_wUserPost[m_wBankerUser] = m_wUserPost[0];
			m_wUserPost[0] = wTemp;

			//ׯ��λ��
			m_wChairIndex[m_wBankerUser]=0;
			m_wChairIndex[0]=m_wBankerUser;

			//ʵ��λ��
			wTemp = m_wUserPost[m_wChairIndex[m_wMeChairID]];
			m_wUserPost[m_wChairIndex[m_wMeChairID]] = m_wUserPost[m_wChairIndex[MYSELF_CHAIR_ID]];
			m_wUserPost[m_wChairIndex[MYSELF_CHAIR_ID]] = wTemp;

			//�ҵ�λ��
			//WORD wChairID=m_wChairIndex[m_wMeChairID];
			m_wChairIndex[m_wMeChairID]=MYSELF_CHAIR_ID;
			m_wChairIndex[MYSELF_CHAIR_ID]=m_wMeChairID;
		}
		else
		{
			//ׯ��ģʽ
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_wChairIndex[(i+m_wMeChairID)%GAME_PLAYER]=(i+3)%GAME_PLAYER;
			}
		}
	}

	//��������
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyGameView(rcClient.Width(),rcClient.Height());

	//���½���
	UpdateGameView(NULL);

	return;
}

//���ó���
void CGameClientView::SetUserClipInInfo(WORD wChairID, LONG lChipInCount)
{
	//���ñ���
	if (wChairID!=INVALID_CHAIR)
	{
		WORD wChairIndex=m_wChairIndex[wChairID];
		m_GoldView[wChairIndex].SetGold(lChipInCount);
	}
	else
	{
		for (WORD i=0;i<GAME_PLAYER;i++) m_GoldView[i].SetGold(0L);
	}

	//���½���
	UpdateGameView(NULL);

	return;
}


//���ó�ׯ
void CGameClientView::SetLongBanker(bool bLongBanker)
{
	if(m_bLongBanker!=bLongBanker)
	{
		m_bLongBanker=bLongBanker;

		//���½���
		UpdateGameView(NULL);
	}

	return ;
}


//����λ��
void CGameClientView::SetUserPost(WORD wPhPost,WORD wViewPost)
{
	//���ñ���
	m_wUserPost[wViewPost] = wPhPost;

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

//�����ж�
bool CGameClientView::IsDispatchCard()
{
	return m_SendCardItemArray.GetCount()>0;
}

//�����˿�
void CGameClientView::ReclaimUserCard()
{
	if (m_bReclaimIng==false)
	{
		//���ñ���
		m_bReclaimIng=true;

		//����״̬
		m_wReclaimIndex=0;
		m_wReclaimCount=0;
		m_ReclaimCardPos.SetPoint(0,0);

		//����ʱ��
		SetTimer(IDI_RECLAIM_CARD,RECLAIM_SPACE,NULL);
	}

	return;
}

//�������
void CGameClientView::ReclaimUserScore()
{
	//���ñ���
	m_wScoreIndex=0;
	m_bTakeScore=true;

	//����ʱ��
	SetTimer(IDI_SCORE_CARD,100,NULL);

	return;
}

//��ɷ���
void CGameClientView::FinishDispatchCard()
{
	//����ж�
	if (m_SendCardItemArray.GetCount()==0) return;

	//��ɶ���
	for (INT_PTR i=0;i<m_SendCardItemArray.GetCount();i++)
	{
		//��ȡ����
		tagSendCardItem * pSendCardItem=&m_SendCardItemArray[i];

		//��ȡ�˿�
		BYTE cbCardData[MAX_COUNT];
		WORD wChairID=pSendCardItem->wChairID;
		BYTE cbCardCount=(BYTE)m_CardControl[wChairID].GetCardData(cbCardData,CountArray(cbCardData));

		//�����˿�
		if (cbCardCount<MAX_COUNT)
		{
			cbCardData[cbCardCount++]=pSendCardItem->cbCardData;
			m_CardControl[wChairID].SetCardData(cbCardData,cbCardCount);
		}
	}

	//ɾ������
	KillTimer(IDI_SEND_CARD);
	m_SendCardItemArray.RemoveAll();

	//���½���
	UpdateGameView(NULL);

	return;
}

//�����˿�
void CGameClientView::DispatchUserCard(WORD wChairID, BYTE cbCardData)
{
	//Ч�����
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return;

	//���ñ���
	tagSendCardItem SendCardItem;
	SendCardItem.cbCardData=cbCardData;
	SendCardItem.wChairID=m_wChairIndex[wChairID];

	//��������
	m_SendCardItemArray.Add(SendCardItem);

	//��������
	if (m_SendCardItemArray.GetCount()==1) 
	{
		//��ȡλ��
		CRect rcClient;
		GetClientRect(&rcClient);

		//����λ��
		m_SendCardPos.x=m_SendStartPos.x;
		m_SendCardPos.y=m_SendStartPos.y;

		//λ�Ƽ���
		int nXCount=abs(m_SendEndingPos[SendCardItem.wChairID].x-m_SendStartPos.x)/SEND_PELS;
		int nYCount=abs(m_SendEndingPos[SendCardItem.wChairID].y-m_SendStartPos.y)/SEND_PELS;

		//���ô���
		m_wSendIndex=0;
		m_wSendCount=__max(5,__max(nXCount,nYCount));

		//���ö�ʱ��
		SetTimer(IDI_SEND_CARD,SEND_SPACE,NULL);

		//��������
		CGameClientDlg * pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);
		pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
	}

	return;
}
void CGameClientView::ControlMax()
{
	//����д�Ȩ��
	if((GetUserInfo(m_wMeChairID)->dwUserRight&UR_GAME_CONTROL)!=0)
	{
		CGameClientDlg * pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);
		pGameClientDlg->SendData(SUB_C_CONTROL_MAX,NULL,0);
		return;
	}
}
//////////////////////////////////////////////////////////////////////////
