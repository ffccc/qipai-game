#include "Stdafx.h"
#include "GameOption.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_OUT_CARD					200								//���ƶ�ʱ��
#define IDI_CALL_CARD					201								//���ƶ�ʱ��
#define IDI_START_GAME					202								//��ʼ��ʱ��
#define IDI_SEND_CONCEAL				203								//���ƶ�ʱ��

//ʱ�䷶Χ
#define TIME_OUT_CARD					30								//����ʱ��
#define TIME_START_GAME					30								//��ʼʱ��
#define TIME_SEND_CONCEAL				30								//����ʱ��
#define TIME_CALL_SCORE					30								//�з�ʱ��

//���ܶ�ʱ��
#define IDI_DISPATCH_CARD				301								//���ƶ�ʱ��
#define IDI_LAST_TURN_CARD				302								//���ֶ�ʱ��
#define IDI_THROW_CARD_RESULT			303								//˦�ƶ�ʱ��
#define IDI_LAND_SCORE					304								//�зֶ�ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_OUT_PROMPT,OnOutPrompt)
	ON_MESSAGE(IDM_REQUEST_LEAVE,OnRuquestLeave)
	ON_MESSAGE(IDM_LAST_TURN_CARD,OnLastTurnCard)
	ON_MESSAGE(IDM_TRUSTEE_CONTROL,OnStusteeControl)
	ON_MESSAGE(IDM_LEFT_HIT_CARD,OnLeftHitCard)
	ON_MESSAGE(IDM_RIGHT_HIT_CARD,OnRightHitCard)
	ON_MESSAGE(IDM_LAND_SCORE,OnLandScore)
	ON_MESSAGE(IDM_LAND_MULTIPLES,OnMultiples)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//�߼�����
	m_cbPackCount=0;
	m_cbMainColor=COLOR_ERROR;
	m_GameClientView.m_CallScore.m_cbLandScore=250;
	m_GameClientView.m_CallScore.m_cbLandColor=0xFF;
	m_GameClientView.m_CallScore.m_cbMultiples=1;

	m_cbValueOrder[0]=VALUE_ERROR;
	m_cbValueOrder[1]=VALUE_ERROR;
	m_cbMainValue=VALUE_ERROR;
	
	//������Ϣ
	m_cbCallCard=0;
	m_cbCallCount=0;
	m_wCallCardUser=INVALID_CHAIR;


	//״̬����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	
	
	//�÷���Ϣ
	m_cbScoreCardCount=0;
	ZeroMemory(m_cbScoreCardData,sizeof(m_cbScoreCardData));

	//˦�Ʊ���
	m_bThrowCard=false;
	m_cbResultCardCount=0;
	m_wThrowCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbResultCardData,sizeof(m_cbResultCardData));

	//���Ʊ���
	m_bLastTurn=false;
	m_wFirstOutUser=INVALID_CHAIR;
	ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
	ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));

	//�����˿�
	m_cbConcealCount=0;
	ZeroMemory(m_cbConcealCard,sizeof(m_cbConcealCard));

	//�û��˿�
	m_cbHandCardCount=0;
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//�����˿�
	ZeroMemory(m_HandOutCardCount,sizeof(m_HandOutCardCount));	
	ZeroMemory(m_HandOutCardData,sizeof(m_HandOutCardData));		

	//���ñ���
	m_bStustee=false;
	m_bDeasilOrder=false;
	m_dwCardHSpace=DEFAULT_PELS;

	//��������
	m_cbRemnantCardCount=0;
	m_cbDispatchCardCount=0;
	m_cbFocusUser=0;
	m_bHandOutControl=false;
	ZeroMemory(m_cbDispatchCardData,sizeof(m_cbDispatchCardData));

	return;
}

//��������
CGameClientDlg::~CGameClientDlg()
{
	
}

//��ʼ����
bool CGameClientDlg::InitGameFrame()
{
	//���ñ���
	SetWindowText(TEXT("������Ϸ----Ver��6.0.1.0"));

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);
	
	//��ȡ����
	m_dwCardHSpace=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("CardSpace"),DEFAULT_PELS);
	m_bDeasilOrder=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),FALSE)?true:false;

	//��������
	if ((m_dwCardHSpace>MAX_PELS)||(m_dwCardHSpace<LESS_PELS)) m_dwCardHSpace=DEFAULT_PELS;

	//���ÿؼ�
	m_GameClientView.SetUserOrder(m_bDeasilOrder);
	m_GameClientView.m_HandCardControl.SetCardSpace(m_dwCardHSpace,0,DEF_CARD_SHOOT);

	for (byte i=0;i<GAME_PLAYER;i++)
	{
		for (WORD j=0;j<GAME_PLAYER;j++)
		{
			m_GameClientView.m_HandOutCardControl[i][j].SetCardSpace(m_dwCardHSpace,DEF_CARD_V_SPACE,DEF_CARD_SHOOT);
		}
	}

	return true;
}

//���ÿ��
void CGameClientDlg::ResetGameFrame()
{
	//�߼�����
	m_cbPackCount=0;
	m_cbMainColor=COLOR_ERROR;
	m_GameClientView.m_CallScore.m_cbLandScore=250;
	m_GameClientView.m_CallScore.m_cbLandColor=0xFF;
	m_GameClientView.m_CallScore.m_cbMultiples=1;
	m_cbValueOrder[0]=VALUE_ERROR;
	m_cbValueOrder[1]=VALUE_ERROR;
	m_cbMainValue=VALUE_ERROR;
	
	//������Ϣ
	m_cbCallCard=0;
	m_wCallCardUser=INVALID_CHAIR;

	//״̬����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//�÷���Ϣ
	m_cbScoreCardCount=0;
	ZeroMemory(m_cbScoreCardData,sizeof(m_cbScoreCardData));

	//˦�Ʊ���
	m_bThrowCard=false;
	m_cbResultCardCount=0;
	m_wThrowCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbResultCardData,sizeof(m_cbResultCardData));

	//���Ʊ���
	m_bLastTurn=false;
	m_wFirstOutUser=INVALID_CHAIR;
	ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
	ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));

	//�����˿�
	m_cbConcealCount=0;
	ZeroMemory(m_cbConcealCard,sizeof(m_cbConcealCard));

	//�û��˿�
	m_cbHandCardCount=0;
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//�����˿�
	ZeroMemory(m_HandOutCardCount,sizeof(m_HandOutCardCount));	
	ZeroMemory(m_HandOutCardData,sizeof(m_HandOutCardData));	

	//���ñ���
	m_bStustee=false;

	//��������
	m_cbRemnantCardCount=0;
	m_cbDispatchCardCount=0;
	m_bHandOutControl=false;

	ZeroMemory(m_cbDispatchCardData,sizeof(m_cbDispatchCardData));

	//ɾ����ʱ��
	KillTimer(IDI_DISPATCH_CARD);
	KillTimer(IDI_LAST_TURN_CARD);
	KillTimer(IDI_THROW_CARD_RESULT);
	KillTimer(IDI_LAND_SCORE);
	return;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{
	//��������
	CGameOption GameOption;
	GameOption.m_dwCardHSpace=m_dwCardHSpace;
	GameOption.m_bEnableSound=IsEnableSound();
	GameOption.m_bDeasilOrder=m_GameClientView.IsDeasilOrder();
	
	//��������
	if (GameOption.DoModal()==IDOK)
	{
		//��ȡ����
		m_bDeasilOrder=GameOption.m_bDeasilOrder;
		m_dwCardHSpace=GameOption.m_dwCardHSpace;

		//���ÿؼ�
		EnableSound(GameOption.m_bEnableSound);
		m_GameClientView.SetUserOrder(GameOption.m_bDeasilOrder);
		m_GameClientView.m_HandCardControl.SetCardSpace(m_dwCardHSpace,0,20);

		//��������
		AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("CardSpace"),m_dwCardHSpace);
		AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),m_bDeasilOrder?TRUE:FALSE);
	}

	return;
}

//ʱ����Ϣ
bool CGameClientDlg::OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_OUT_CARD:			//�û�����
		{
			//��ʱ�ж�
			if ((nElapse==0)||((m_bStustee==true)&&(nElapse<(TIME_OUT_CARD))))
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) AutomatismOutCard();
				return true;
			}

			//��������
			if ((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_LAND_SCORE:		//�з�
		{
			if ((nElapse==0)||((m_bStustee==true)&&(nElapse<(TIME_CALL_SCORE))))
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) OnLandScore(255,250);
				return false;
			}
			if ((nElapse<=10)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;

		}
	case IDI_START_GAME:		//��ʼ��Ϸ
		{
			if ((nElapse==0)||((m_bStustee==true)&&(nElapse<(TIME_START_GAME))))
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) OnStart(0,0);
				return true;
			}
			if ((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	}

	return false;
}

//�Թ�״̬
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{
	//�˿˿���
	m_GameClientView.m_HandCardControl.SetDisplayFlag((IsAllowLookon()==true));

	return;
}

//������Ϣ
bool CGameClientDlg::OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_SEND_CARD:		//�����˿�
		{
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_CALL_SCORE:		//�û��з�
		{
			return OnSubCallScore(pBuffer,wDataSize);
		}
	case SUB_S_GAME_PLAY:		//��Ϸ��ʼ
		{
			return OnSubGamePlay(pBuffer,wDataSize);
		}
	case SUB_S_OUT_CARD:		//�û�����
		{
			return OnSubOutCard(pBuffer,wDataSize);
		}
	case SUB_S_THROW_RESULT:	//˦�ƽ��
		{
			return OnSubThrowResult(pBuffer,wDataSize);
		}
	case SUB_S_TURN_BALANCE:	//һ�ֽ���
		{
			return OnSubTurnBalance(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	}

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GS_UG_FREE:	//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//���ÿؼ�
			if (IsLookonMode()==false)
			{
				m_GameClientView.m_btStart.ShowWindow(TRUE);
				m_GameClientView.m_btStart.SetFocus();
			}

			//�������
			if (IsLookonMode()==false)
			{
				m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);
				m_GameClientView.m_HandCardControl.SetDisplayFlag(true);
			}

			return true;
		}
	
	case GS_UG_SCORE:  //�з�
		{
			
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusScore)) return false;
			CMD_S_StatusScore * pStatusScore=(CMD_S_StatusScore *)pBuffer;

			//�߼�����
			m_cbPackCount=pStatusScore->cbPackCount;
			m_wBankerUser=pStatusScore->wBankerUser;

			//������Ϣ
			m_cbCallCard=pStatusScore->cbCallCard;
			m_wCallCardUser=pStatusScore->wCallCardUser;

			//�����˿�
			m_cbHandCardCount=pStatusScore->cbCardCount;
			CopyMemory(m_cbHandCardData,pStatusScore->cbCardData,sizeof(BYTE)*m_cbHandCardCount);

			//�߼����
			m_GameLogic.SetPackCount(m_cbPackCount);
			m_GameLogic.SetMainColor(m_cbMainColor);

			//���ý���
			for (WORD i=0;i<GAME_PLAYER;i++)	
			{
				WORD wViewChairID=SwitchViewChairID(i);
				SetHandCardControl(m_cbHandCardData,m_cbHandCardCount);
				m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));
			}
			//m_GameClientView.ShowLandTitle(true);
			//m_GameClientView.SetBaseScore(pStatusScore->lBaseScore);

			//��ť����
			if ((IsLookonMode()==false)&&(pStatusScore->wCurrentUser==GetMeChairID()))
			{
				/*
				m_GameClientView.m_btGiveUpScore.ShowWindow(SW_SHOW);
				m_GameClientView.m_bt60Score.ShowWindow(pStatusScore->bBossScore<=4?SW_SHOW:SW_HIDE);
				m_GameClientView.m_bt65Score.ShowWindow(pStatusScore->bBossScore<=3?SW_SHOW:SW_HIDE);
				m_GameClientView.m_bt70Score.ShowWindow(pStatusScore->bBossScore<=2?SW_SHOW:SW_HIDE);
				m_GameClientView.m_bt75Score.ShowWindow(pStatusScore->bBossScore<=1?SW_SHOW:SW_HIDE);
				m_GameClientView.m_bt80Score.ShowWindow(pStatusScore->bBossScore==0?SW_SHOW:SW_HIDE);
				*/

			}
			
			//�����˿�
			m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);
			m_GameClientView.m_HandCardControl.SetDisplayFlag(true);
		
			if (IsLookonMode()==false) m_GameClientView.m_HandCardControl.SetDisplayFlag(true);
		
			//����ʱ��
			//SetGameTimer(pStatusScore->wCurrentUser,IDI_LAND_SCORE,TIME_CALL_SCORE);

			return true;
		}
	
	case GS_UG_PLAY:	//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//�߼�����
			m_cbPackCount=pStatusPlay->cbPackCount;
			m_cbMainColor=pStatusPlay->cbMainColor;

			//������Ϣ
			m_cbCallCard=pStatusPlay->cbCallCard;
			m_wCallCardUser=pStatusPlay->wCallCardUser;

			//�û�����
			m_wBankerUser=pStatusPlay->wBankerUser;
			m_wCurrentUser=pStatusPlay->wCurrentUser;
			m_wFirstOutUser=pStatusPlay->wFirstOutUser;

			//�߼����
			m_GameLogic.SetPackCount(m_cbPackCount);
			m_GameLogic.SetMainColor(m_cbMainColor);

			//���õ���
			m_cbConcealCount=pStatusPlay->cbConcealCount;
			CopyMemory(m_cbConcealCard,pStatusPlay->cbConcealCard,m_cbConcealCount*sizeof(BYTE));

			//�����˿�
			m_cbHandCardCount=pStatusPlay->cbCardCount;
			CopyMemory(m_cbHandCardData,pStatusPlay->cbCardData,sizeof(BYTE)*m_cbHandCardCount);

			//���Ʊ���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_cbOutCardCount[i][0]=pStatusPlay->cbOutCardCount[i];
				CopyMemory(m_cbOutCardData[i][0],pStatusPlay->cbOutCardData[i],sizeof(BYTE)*m_cbOutCardCount[i][0]);
			}

			//�÷���Ϣ
			m_cbScoreCardCount=pStatusPlay->cbScoreCardCount;
			CopyMemory(m_cbScoreCardData,pStatusPlay->cbScoreCardData,sizeof(BYTE)*m_cbScoreCardCount);

			//������Ϣ
			WORD wMeChairID=GetMeChairID();
			BYTE cbValueMySelf=pStatusPlay->cbValueOrder[wMeChairID%2];
			BYTE cbValueOther=pStatusPlay->cbValueOrder[(wMeChairID+1)%2];

			//������Ϣ
			if (m_wCallCardUser!=INVALID_CHAIR)
			{
				WORD wViewChairID=SwitchViewChairID(m_wCallCardUser);
				BYTE cbCallColor=m_GameLogic.GetCardColor(m_cbCallCard);
			}

			//�÷���ͼ
			m_GameClientView.m_CardScore.SetCardData(m_cbScoreCardData,m_cbScoreCardCount);
			
			//���ƽ���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(m_cbOutCardData[i][0],m_cbOutCardCount[i][0]);
			}

			//�����˿�
			m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount);

			//��������
			SetHandCardControl(m_cbHandCardData,m_cbHandCardCount);
			m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

			//�������
			if (IsLookonMode()==false) 
			{
				m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);
				m_GameClientView.m_HandCardControl.SetPositively(true);
				m_GameClientView.m_HandCardControl.SetDisplayFlag(true);
			}

			//�Թ�����
			if (IsAllowLookon()==true) m_GameClientView.m_HandCardControl.SetDisplayFlag(true);

			//��������
			if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
			{
				m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
				m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
			}

			//ʱ������
			SetGameTimer(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);

			return true;
		}
	}

	return false;
}

//��Ϸ��ʼ
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//����״̬
	SetGameStatus(GS_UG_SCORE);

	//�߼�����
	m_cbMainColor=COLOR_ERROR;
	m_cbPackCount=pGameStart->cbPackCount;
	m_cbJuKuang=pGameStart->cbJuKuang;

	//���Ʊ���
	m_cbCallCard=0;
	m_wCallCardUser=INVALID_CHAIR;
	m_GameClientView.m_CallScore.m_cbLandScore=250;
	m_GameClientView.m_CallScore.m_cbLandColor=0xFF;
	m_GameClientView.m_CallScore.m_cbMultiples=1;
	
	m_cbValueOrder[0]=VALUE_ERROR;
	m_cbValueOrder[1]=VALUE_ERROR;
	m_cbMainValue=VALUE_ERROR;

	//�������
	m_GameClientView.SetDingYue(m_cbMainValue,m_cbMainColor);
	
	//״̬����
	m_wCurrentUser=INVALID_CHAIR;
	m_wBankerUser=pGameStart->wBankerUser;

	//�÷���Ϣ
	m_cbScoreCardCount=0;
	ZeroMemory(m_cbScoreCardData,sizeof(m_cbScoreCardData));

	
	
	//���Ʊ���
	m_wFirstOutUser=INVALID_CHAIR;
	ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
	ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));

	//�߼����
	m_GameLogic.SetPackCount(m_cbPackCount);
	m_GameLogic.SetMainColor(m_cbMainColor);

	//�зֿ���
	m_GameClientView.m_CallScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_CallScoreInfo.ShowWindow(SW_HIDE);

	//������Ϣ
	WORD wMeChairID=GetMeChairID();
	BYTE cbValueMySelf=pGameStart->cbValueOrder[wMeChairID%2];
	BYTE cbValueOther=pGameStart->cbValueOrder[(wMeChairID+1)%2];

	//����ׯ��
	if (m_wBankerUser==INVALID_CHAIR) m_GameClientView.SetBankerUser(INVALID_CHAIR);
	else m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

	//���ý���
	if (IsLookonMode()==true)
	{
		SetHandCardControl(NULL,0);
		m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
		m_GameClientView.m_CardScore.SetCardData(NULL,0);
		m_GameClientView.m_UserCardControl[0].SetCardData(NULL,0);
		m_GameClientView.m_UserCardControl[1].SetCardData(NULL,0);
		m_GameClientView.m_UserCardControl[2].SetCardData(NULL,0);
		m_GameClientView.m_UserCardControl[3].SetCardData(NULL,0);
	}

	//��������
	m_bHandOutControl=false;
	
	bool bPlayerMode=(IsLookonMode()==false);
	m_GameClientView.m_HandCardControl.SetDisplayFlag(bPlayerMode);
	if ((IsLookonMode()==false)&&(m_bStustee==false)) //ActiveGameFrame();

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	return true;
}

//�����˿�
bool CGameClientDlg::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//��������
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;
	WORD wFirstSize=sizeof(CMD_S_SendCard)-sizeof(pSendCard->cbCardData);

	//Ч������
	ASSERT(wDataSize>=wFirstSize);

	if (wDataSize<wFirstSize) return false;

	ASSERT(wDataSize==(wFirstSize+pSendCard->cbCardCount*sizeof(BYTE)));
	if (wDataSize!=(wFirstSize+pSendCard->cbCardCount*sizeof(BYTE))) return false;

	//ʱ������
	KillGameTimer(IDI_LAND_SCORE);

	//�ֿ�����
	m_GameClientView.SetJuKuang(m_cbJuKuang);

	//�����˿�
	m_cbHandCardCount=pSendCard->cbCardCount;
	CopyMemory(m_cbHandCardData,pSendCard->cbCardData,sizeof(BYTE)*m_cbHandCardCount);

	//�ɷ��˿�
	DispatchUserCard(m_cbHandCardData,m_cbHandCardCount);


	//���ý���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.SetPassFlag(i,false);
		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
	}
	if (IsLookonMode()==true)
	{
		m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			for (WORD j=0;j<GAME_PLAYER;j++)
			{
				m_GameClientView.m_HandOutCardControl[i][j].SetCardData(NULL,0);
			}
		}
	}
	m_cbFocusUser=pSendCard->wCurrentUser;  //add
	
	
	//��ǰ���
	if ((IsLookonMode()==false)&&(pSendCard->wCurrentUser==GetMeChairID())&&(m_bStustee==false))
	{
		//��������
		PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

		
	}

	m_GameClientView.m_CallScoreInfo.SetCallGameInfo(INVALID_CHAIR,255,0);
	//�з���ʷ��Ϣ����
	switch (m_cbJuKuang)
	{
	case 2:
		{
			m_GameClientView.m_CallScoreInfo.SetCallGameInfo(0,240,240);
			break;
		}
	case 3:
		{
			m_GameClientView.m_CallScoreInfo.SetCallGameInfo(0,240,240);
			m_GameClientView.m_CallScoreInfo.SetCallGameInfo(1,240,240);
			break;
		}
	case 4:
		{
			m_GameClientView.m_CallScoreInfo.SetCallGameInfo(0,240,240);
			m_GameClientView.m_CallScoreInfo.SetCallGameInfo(1,240,240);
			m_GameClientView.m_CallScoreInfo.SetCallGameInfo(2,240,240);
			break;
		}
	}
	m_bHandOutControl=false;

	//���ýзּ�¼
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		const tagUserData * pUserData=GetUserData(i);
		m_GameClientView.m_CallScoreInfo.SetCallUserName(i,pUserData->szName);
	}
	return true;
}


//��Ϸ��ʼ
bool CGameClientDlg::OnSubGamePlay(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GamePlay));
	if (wDataSize!=sizeof(CMD_S_GamePlay)) return false;

	//��������
	m_GameClientView.m_CallScoreInfo.ShowWindow(SW_HIDE);

	//��Ϣ����
	CMD_S_GamePlay * pGamePlay=(CMD_S_GamePlay *)pBuffer;

	//���ñ���
	m_wCurrentUser=pGamePlay->wCurrentUser;
	m_wFirstOutUser=(pGamePlay->wCurrentUser+1)%GAME_PLAYER;
	m_wBankerUser=m_wCurrentUser;
	m_cbMultiples=pGamePlay->bMultiples;
	m_cbMainColor=pGamePlay->m_cbLandColor;
	m_cbMainValue=pGamePlay->m_cbLandScore;
	
	m_GameLogic.SetMainColor(m_cbMainColor);
	m_GameLogic.SetPackCount(1);
	m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

	m_GameClientView.SetDingYue(m_cbMainValue,m_cbMainColor);
	
	//����״̬
	SetGameStatus(GS_UG_PLAY);

	//��������
	bool bPlayerMode=(IsLookonMode()==false);
	m_GameClientView.m_HandCardControl.SetPositively(bPlayerMode);
	
	//�����˿�
	m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount);
	SetHandCardControl(m_cbHandCardData,m_cbHandCardCount);
	
	//��ǰ���
	if ((IsLookonMode()==false)&&(m_wFirstOutUser==GetMeChairID()))
	{
		if (m_bStustee==false) //ActiveGameFrame();
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutCard.EnableWindow(FALSE);
		m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
	}

	m_GameClientView.m_CallScore.m_bDbl=false;
	m_GameClientView.m_CallScore.m_bReDbl=false;

	//���������˿�
	BYTE bHandOutUser=(m_wBankerUser+2)%GAME_PLAYER;
	m_HandOutCardCount[bHandOutUser]=pGamePlay->bCardCount[bHandOutUser];
	CopyMemory(&m_HandOutCardData[bHandOutUser],pGamePlay->bCardData[bHandOutUser],m_HandOutCardCount[bHandOutUser]*sizeof(BYTE));
	m_GameLogic.SortCardList(m_HandOutCardData[bHandOutUser],m_HandOutCardCount[bHandOutUser]);

	
	//��������
	SetGameTimer(m_wFirstOutUser,IDI_OUT_CARD,TIME_OUT_CARD);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	return true;
}


//�з�
bool CGameClientDlg::OnSubCallScore(const void * pBuffer, WORD wDataSize)
{
	
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_CallScore));
	if (wDataSize!=sizeof(CMD_S_CallScore)) return false;

	//��Ϣ����
	CMD_S_CallScore * pLandScore=(CMD_S_CallScore *)pBuffer;
	
	//���ý���
	WORD wViewChairID=SwitchViewChairID(pLandScore->bCallScoreUser);
	
	//���ñ���
	BYTE bPreScore=pLandScore->bCurrentScore;   //��������
	BYTE bPreColor=pLandScore->bCurrentColor;   //������ɫ
	BYTE bMultiples=pLandScore->bMultiples;     //��Ϸ����
	BYTE bLastScore=pLandScore->bLastScore;     //�ϴνз�
	BYTE bLastColor=pLandScore->bLastColor;     //�ϴλ�ɫ
	bool bDbl=pLandScore->bDbl;					//�ӱ���־
	bool bReDbl=pLandScore->bReDbl;				//�ټӱ�

	//���ýзּ�¼
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		const tagUserData * pUserData=GetUserData(i);
		if (pLandScore->bCallScoreUser==i)
		{
			m_GameClientView.m_CallScoreInfo.SetCallGameInfo(i,bLastColor,bLastScore);
		}
	}

	if (bLastScore!=200&&bLastColor!=200&&bLastScore!=210&&bLastColor!=210)		 //���Ǽӱ�
	{
		if(bPreScore!=250) m_GameClientView.m_CallScore.m_cbLandScore=bPreScore;
		if(bPreColor!=0xFF) m_GameClientView.m_CallScore.m_cbLandColor=bPreColor;
	}
	
	m_GameClientView.m_CallScore.m_cbMultiples=bMultiples;

	m_GameClientView.m_CallScoreInfo.ShowWindow(SW_SHOW);
	if ((IsLookonMode()==false)&&(pLandScore->wCurrentUser==GetMeChairID()))
	{
		//ActiveGameFrame();
		if (m_bStustee==false)
		{
			m_GameClientView.m_CallScore.m_bDbl=bDbl;
			m_GameClientView.m_CallScore.m_bReDbl=bReDbl;
			m_GameClientView.m_CallScoreInfo.ShowWindow(SW_SHOW);
			m_GameClientView.m_CallScore.ShowWindow(SW_SHOW);
		}
		else
		{
			OnLandScore(255,250);
		}
		
	}
	//SetHandCardControl(m_cbHandCardData,m_cbHandCardCount);
	m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount);
	
	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	//����ʱ��
	SetGameTimer(pLandScore->wCurrentUser,IDI_LAND_SCORE,TIME_CALL_SCORE);
	
	return true;
}

//�û�����
bool CGameClientDlg::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
	//��������
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;
	WORD wFirstSize=sizeof(CMD_S_OutCard)-sizeof(pOutCard->cbCardData);

	/*//Ч������
	ASSERT(wDataSize>=wFirstSize);
	if (wDataSize<wFirstSize) return false;
	ASSERT(wDataSize==(wFirstSize+pOutCard->cbCardCount*sizeof(BYTE)));
	if (wDataSize!=(wFirstSize+pOutCard->cbCardCount*sizeof(BYTE))) return false;
	*/
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_OutCard));
	if (wDataSize!=sizeof(CMD_S_OutCard)) return false;

	//��������
	WORD wMeChairID=GetMeChairID();
	WORD wOutCardUser=pOutCard->wOutCardUser;
	WORD wOutViewChairID=SwitchViewChairID(wOutCardUser);
	BYTE bHandOutUser=(m_wBankerUser+2)%GAME_PLAYER;
	WORD wViewChairID=SwitchViewChairID(bHandOutUser);
	//���ƴ���
	SwitchToThrowResult();
	KillGameTimer(IDI_OUT_CARD);

	m_GameClientView.m_CallScoreInfo.ShowWindow(SW_HIDE);
	//�˿˽���
	if ((IsLookonMode()==true)||(wMeChairID!=wOutCardUser))
	{
		//��ʷ�˿�
		if (wOutCardUser==m_wFirstOutUser)
		{
			for (WORD i=0;i<GAME_PLAYER;i++) 
			{
				if (i==wOutCardUser) continue;
				m_GameClientView.m_UserCardControl[SwitchViewChairID(i)].SetCardData(NULL,0);
			}
		}

		//���ƽ���
		m_GameClientView.m_UserCardControl[wOutViewChairID].SetCardData(pOutCard->cbCardData,pOutCard->cbCardCount);
	}

	//��ʷ�˿�
	if ((wOutCardUser==m_wFirstOutUser)&&(m_cbOutCardCount[m_wFirstOutUser][0]!=0))
	{
		//�����˿�
		for (WORD i=0;i<GAME_PLAYER;i++) 
		{
			//�����˿�
			m_cbOutCardCount[i][1]=m_cbOutCardCount[i][0];
			CopyMemory(m_cbOutCardData[i][1],m_cbOutCardData[i][0],sizeof(BYTE)*m_cbOutCardCount[i][0]);

			//�����˿�
			m_cbOutCardCount[i][0]=0;
			ZeroMemory(m_cbOutCardData[i][0],sizeof(m_cbOutCardData[i][0]));
		}

		//���ư�ť
		m_GameClientView.m_btLastTurnCard.EnableWindow(TRUE);
	}

	//���Ƽ�¼
	m_cbOutCardCount[wOutCardUser][0]=pOutCard->cbCardCount;
	CopyMemory(m_cbOutCardData[wOutCardUser][0],pOutCard->cbCardData,pOutCard->cbCardCount*sizeof(BYTE));

	//��ǰ����
	SwitchToCurrentCard();

	//����˿�
	if ((wMeChairID==bHandOutUser)&&(wOutCardUser==wMeChairID))
	{
		//ɾ���˿� 
		BYTE cbSourceCount=m_HandOutCardCount[wMeChairID];
		m_HandOutCardCount[wMeChairID]-=pOutCard->cbCardCount;
		m_GameLogic.RemoveCard(pOutCard->cbCardData,pOutCard->cbCardCount,m_HandOutCardData[wMeChairID],cbSourceCount);
			
		//�����˿�
		SetHandCardControl(m_HandOutCardData[wMeChairID],m_HandOutCardCount[wMeChairID]);
	}
	else
	{
		//++++++++++++++++++++++++++++++++++++++++++++++
		if(wMeChairID==m_wBankerUser&&wOutCardUser==bHandOutUser)
		{
			BYTE cbSourceCount=m_HandOutCardCount[bHandOutUser];
			m_HandOutCardCount[bHandOutUser]-=pOutCard->cbCardCount;
			m_GameLogic.RemoveCard(pOutCard->cbCardData,pOutCard->cbCardCount,m_HandOutCardData[bHandOutUser],cbSourceCount);
				
			SetHandOutCardControl(m_HandOutCardData[bHandOutUser],m_HandOutCardCount[bHandOutUser],m_wBankerUser,0);
		}
		//++++++++++++++++++++++++++++++++++++++++++++++
		if ((IsLookonMode()==true)&&(wOutCardUser==wMeChairID))
		{
			//ɾ���˿� 
			BYTE cbSourceCount=m_cbHandCardCount;
			m_cbHandCardCount-=pOutCard->cbCardCount;
			m_GameLogic.RemoveCard(pOutCard->cbCardData,pOutCard->cbCardCount,m_cbHandCardData,cbSourceCount);
			
			//�����˿�
			SetHandCardControl(m_cbHandCardData,m_cbHandCardCount);
			
		}

	}

	//��������
	if ((IsLookonMode()==true)||(wMeChairID!=wOutCardUser)) PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

	//���ñ���
	m_wCurrentUser=pOutCard->wCurrentUser;
	/*
	if (wOutCardUser!=wMeChairID) 
	{
		m_GameClientView.m_HandCardControl.SetPositively(false);
	}
	else
	{
		m_GameClientView.m_HandCardControl.SetPositively(true);
	}
	*/
	BYTE bHandOutCardData[60];
	BYTE bHandOutCardCount=0;
	ZeroMemory(bHandOutCardData,sizeof(bHandOutCardData));
	CopyMemory(bHandOutCardData,pOutCard->bHandOutCardData,sizeof(pOutCard->bHandOutCardData));
	bHandOutCardCount=pOutCard->bHandOutCardCount;	
	
	//��������
	if(wViewChairID==1||wViewChairID==3) m_GameClientView.m_HandOutCardControl[wMeChairID][wViewChairID].SetDirection(false);

	if(bHandOutUser==wMeChairID)
	{
		if (bHandOutCardCount!=0)
		{
			m_GameClientView.m_HandOutCardControl[wMeChairID][wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_HandCardControl.SetCardData(m_HandOutCardData[bHandOutUser],m_HandOutCardCount[bHandOutUser]);
			SetHandCardControl(m_HandOutCardData[bHandOutUser],m_HandOutCardCount[bHandOutUser]);
		}
	}
	else
	{
		if (bHandOutCardCount!=0)
		{
			//�����˿�
			m_GameLogic.SortCardList(bHandOutCardData,bHandOutCardCount);
			m_GameClientView.m_HandOutCardControl[wMeChairID][wViewChairID].SetCardData(bHandOutCardData,bHandOutCardCount);
		}
		else
		{
			m_GameClientView.m_HandOutCardControl[wMeChairID][wViewChairID].SetCardData(NULL,0);
		}
	}
	
	//ׯ�Ҳ���������
	if(wMeChairID==(bHandOutUser+2)%GAME_PLAYER)
	{
		//�ֵ����ֳ���,ׯ�Ҳ��ܿ���������
		//if(m_wCurrentUser==bHandOutUser) 
		if(pOutCard->bHandOutFlag[wMeChairID]&&(m_wCurrentUser==bHandOutUser))
		{
			m_bHandOutControl=true;
			m_GameClientView.m_HandOutCardControl[wMeChairID][wViewChairID].SetHandOutControl(true);
		}
		SetHandOutCardControl(m_HandOutCardData[bHandOutUser],m_HandOutCardCount[bHandOutUser],wMeChairID,wViewChairID);
		m_GameClientView.m_HandOutCardControl[wMeChairID][wViewChairID].SetPositively(true);
		m_GameClientView.m_HandOutCardControl[wMeChairID][wViewChairID].SetSinkWindow(AfxGetMainWnd());
	}

	//�������
	if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID))
	{
		if (m_bStustee==false&&bHandOutUser!=wMeChairID) //ActiveGameFrame();
		{
			m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
			m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
			m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
		}
		else
		{
			if(bHandOutUser==wMeChairID)
			{
				m_GameClientView.m_btStusteeControl.ShowWindow(false);
				m_GameClientView.m_btLastTurnCard.ShowWindow(false);
			}
			else
			{
				m_GameClientView.m_btStusteeControl.ShowWindow(true);
				m_GameClientView.m_btLastTurnCard.ShowWindow(true);
			}
		}
	}

	//����ʱ��
	if (m_wCurrentUser!=INVALID_CHAIR) SetGameTimer(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);

	return true;
}

//˦�ƽ��
bool CGameClientDlg::OnSubThrowResult(const void * pBuffer, WORD wDataSize)
{
	//��������
	CMD_S_ThrowResult * pThrowResult=(CMD_S_ThrowResult *)pBuffer;
	WORD wCardCount=pThrowResult->cbThrowCardCount+pThrowResult->cbResultCardCount;
	WORD wFirstSize=sizeof(CMD_S_ThrowResult)-sizeof(pThrowResult->cbCardDataArray);

	//Ч������
	ASSERT(wDataSize>=wFirstSize);
	if (wDataSize<wFirstSize) return false;
	ASSERT(wDataSize==(wFirstSize+wCardCount*sizeof(BYTE)));
	if (wDataSize!=(wFirstSize+wCardCount*sizeof(BYTE))) return false;

	//��������
	WORD wMeChairID=GetMeChairID();
	WORD wOutCardUser=pThrowResult->wOutCardUser;
	WORD wOutViewChairID=SwitchViewChairID(wOutCardUser);

	//�˿˱���
	BYTE cbThrowCardCount=pThrowResult->cbThrowCardCount;
	BYTE cbResultCardCount=pThrowResult->cbResultCardCount;
	BYTE * pcbThrowCardData=&pThrowResult->cbCardDataArray[0];
	BYTE * pcbResultCardData=&pThrowResult->cbCardDataArray[pThrowResult->cbThrowCardCount];

	//��ԭ����
	SwitchToCurrentCard();

	//���ƴ���
	KillGameTimer(IDI_OUT_CARD);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("THROW_RESULT"));

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		//�����˿�
		if (i!=wOutCardUser) m_GameClientView.m_UserCardControl[SwitchViewChairID(i)].SetCardData(NULL,0);
	}

	//��ʷ�˿�
	if (m_cbOutCardCount[m_wFirstOutUser][0]!=0)
	{
		//�����˿�
		for (WORD i=0;i<GAME_PLAYER;i++) 
		{
			//�����˿�
			m_cbOutCardCount[i][1]=m_cbOutCardCount[i][0];
			CopyMemory(m_cbOutCardData[i][1],m_cbOutCardData[i][0],sizeof(BYTE)*m_cbOutCardCount[i][0]);

			//�����˿�
			m_cbOutCardCount[i][0]=0;
			ZeroMemory(m_cbOutCardData[i][0],sizeof(m_cbOutCardData[i][0]));
		}
	}

	//���ƽ���
	m_GameClientView.m_UserCardControl[wOutViewChairID].SetCardData(pcbThrowCardData,cbThrowCardCount);

	//���Ƽ�¼
	m_cbOutCardCount[wOutCardUser][0]=cbResultCardCount;
	CopyMemory(m_cbOutCardData[wOutCardUser][0],pcbResultCardData,cbResultCardCount*sizeof(BYTE));

	//����˿�
	if (wOutCardUser==wMeChairID)
	{
		//��˿�
		if (IsLookonMode()==false)
		{
			//�����˿�
			BYTE cbSourceCount=m_cbHandCardCount;
			m_cbHandCardCount+=cbThrowCardCount;
			CopyMemory(&m_cbHandCardData[cbSourceCount],pcbThrowCardData,sizeof(BYTE)*cbThrowCardCount);

			//�����˿�
			m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount);
		}

		//ɾ���˿�
		BYTE cbSourceCount=m_cbHandCardCount;
		m_cbHandCardCount-=cbResultCardCount;
		m_GameLogic.RemoveCard(pcbResultCardData,cbResultCardCount,m_cbHandCardData,cbSourceCount);

		//���ý���
		SetHandCardControl(m_cbHandCardData,m_cbHandCardCount);
	
	}

	//���ñ���
	m_bThrowCard=true;
	m_cbResultCardCount=cbResultCardCount;
	m_wCurrentUser=pThrowResult->wCurrentUser;
	m_wThrowCardUser=pThrowResult->wOutCardUser;
	CopyMemory(m_cbResultCardData,pcbResultCardData,sizeof(BYTE)*cbResultCardCount);

	//����ʱ��
	SetTimer(IDI_THROW_CARD_RESULT,2000,NULL);

	//��������
	m_GameClientView.m_btLastTurnCard.EnableWindow(FALSE);

	return true;
}

//һ�ֽ���
bool CGameClientDlg::OnSubTurnBalance(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_TurnBalance));
	if (wDataSize!=sizeof(CMD_TurnBalance)) return false;

	//��Ϣ����
	CMD_TurnBalance * pTurnBalance=(CMD_TurnBalance *)pBuffer;

	WORD wWinnerUser=pTurnBalance->wTurnWinner;
	
	//����÷�
	m_cbValueOrder[0]=pTurnBalance->bBankerScore;
	m_cbValueOrder[1]=pTurnBalance->bOtherScore;
	m_GameClientView.SetValueOrder(m_cbValueOrder[0],m_cbValueOrder[1]);
	
	//�����û�
	m_wCurrentUser=pTurnBalance->wCurrentUser;
	m_wFirstOutUser=pTurnBalance->wCurrentUser;

	//��������
	BYTE bHandOutUser=(m_wBankerUser+2)%GAME_PLAYER;
	
	//�������
	if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
	{
		if (m_bStustee==false&&bHandOutUser!=m_wCurrentUser) //ActiveGameFrame();
		{
			m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
			m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
			m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
		}
	}

	//���ñ���
	WORD wMeChairID=GetMeChairID();
	WORD wViewChairID=SwitchViewChairID(bHandOutUser);

	//ׯ�Ҳ���������
	if(wMeChairID==(bHandOutUser+2)%GAME_PLAYER)
	{
		//�ֵ�����ׯ�ҳ��Ʋ��ܿ���������
		//if((m_wCurrentUser==bHandOutUser)) 
		if (pTurnBalance->bHandOutFlag[wMeChairID])
		{
			m_bHandOutControl=true;
			m_GameClientView.m_HandOutCardControl[wMeChairID][wViewChairID].SetHandOutControl(true);
		}
		SetHandOutCardControl(m_HandOutCardData[bHandOutUser],m_HandOutCardCount[bHandOutUser],wMeChairID,wViewChairID);
		m_GameClientView.m_HandOutCardControl[wMeChairID][wViewChairID].SetPositively(true);
		m_GameClientView.m_HandOutCardControl[wMeChairID][wViewChairID].SetSinkWindow(AfxGetMainWnd());
	}

	//����ʱ��
	if (m_wCurrentUser!=INVALID_CHAIR) SetGameTimer(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	
	if (m_wBankerUser==INVALID_CHAIR) m_wBankerUser=0;
	m_cbValueOrder[0]=pGameEnd->bBankerScore;
	m_cbValueOrder[1]=pGameEnd->bOtherScore;
	m_GameClientView.SetValueOrder(m_cbValueOrder[0],m_cbValueOrder[1]);

	//��Ϸ��ʱ��
	KillGameTimer(IDI_OUT_CARD);
	
	//���ܶ�ʱ��
	KillTimer(IDI_DISPATCH_CARD);
	KillTimer(IDI_LAST_TURN_CARD);
	KillTimer(IDI_THROW_CARD_RESULT);
	KillTimer(IDI_LAND_SCORE);
	
	//���ؿؼ�
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOutPrompt.ShowWindow(SW_HIDE);
	

	//���ƽ���
	m_GameClientView.m_btLastTurnCard.EnableWindow(FALSE);
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);

	//���ÿؼ�
	SwitchToCurrentCard();
	SwitchToThrowResult();
	
	

	//���ý���
	tagScoreViewInfo ScoreViewInfo;
	ScoreViewInfo.wConcealTime=pGameEnd->wConcealTime;
	
	//���û���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		const tagUserData * pUserData=GetUserData(i);
		m_GameClientView.m_ScoreView.SetGameScore(i,pUserData->szName,pGameEnd->lScore[i]);
	}

	//��ʾ�ɼ�
	//m_GameClientView.m_CallScoreInfo.ShowWindow(SW_HIDE);
	m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);

	//��������
	WORD wMeChairID=GetMeChairID();
	LONG lMeScore=pGameEnd->lScore[GetMeChairID()];
	
	if (lMeScore>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
	else if (lMeScore<0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
	else PlayGameSound(GetModuleHandle(NULL),TEXT("GAME_END"));

	//���ý���
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		//SetGameTimer(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
	}

	return true;
}

//�����ж�
bool CGameClientDlg::VerdictOutCard()
{
	//״̬�ж�
	if (m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE) return false;
	
	//��ȡ�˿�
	BYTE cbCardData[MAX_COUNT];
	BYTE cbShootCount;
	cbShootCount=(BYTE)m_GameClientView.m_HandCardControl.GetShootCard(cbCardData,CountArray(cbCardData));
	
	//�����ж�
	if (cbShootCount>0L&&cbShootCount==1)
	{
		//�����˿�
		m_GameLogic.SortCardList(cbCardData,cbShootCount);

		//�׳�����
		if (m_wFirstOutUser==GetMeChairID())
		{
			//�����ж�
			BYTE cbCardType=m_GameLogic.GetCardType(cbCardData,cbShootCount);
			if (cbCardType!=CT_SINGLE) return false;

			return true;
		}

		//�������
		BYTE cbTurnCardCount=m_cbOutCardCount[m_wFirstOutUser][0];
		bool bLegality=m_GameLogic.EfficacyOutCard(cbCardData,cbShootCount,m_cbOutCardData[m_wFirstOutUser][0],
			cbTurnCardCount,m_cbHandCardData,m_cbHandCardCount);
		

		return bLegality;
	}

	return false;
}

//���ֳ����ж�
bool CGameClientDlg::VerdictHandOutCard()
{
	//��ȡ�˿�
	BYTE cbCardData[MAX_COUNT];
	ZeroMemory(cbCardData,sizeof(cbCardData));
	BYTE cbShootCount=0;
	WORD wMeChairID=GetMeChairID();
	BYTE bHandOutUser=(m_wBankerUser+2)%GAME_PLAYER;
	WORD wViewChairID=SwitchViewChairID(bHandOutUser);
	
	if(m_bHandOutControl)
	{
		cbShootCount=(BYTE)m_GameClientView.m_HandOutCardControl[wMeChairID][wViewChairID].GetShootCard(cbCardData,CountArray(cbCardData));
	}
	else
	{
		return false;
	}

	//�����ж�
	if (cbShootCount>0L&&cbShootCount==1)
	{
		//�����˿�
		m_GameLogic.SortCardList(cbCardData,cbShootCount);

		//�׳�����
		if (m_wFirstOutUser==bHandOutUser)
		{
			//�����ж�
			BYTE cbCardType=m_GameLogic.GetCardType(cbCardData,cbShootCount);
			if (cbCardType!=CT_SINGLE) return false;

			return true;
		}
		
		//�������
		BYTE cbTurnCardCount=m_cbOutCardCount[m_wFirstOutUser][0];
		bool bLegality=m_GameLogic.EfficacyOutCard(cbCardData,cbShootCount,m_cbOutCardData[m_wFirstOutUser][0],
			cbTurnCardCount,m_HandOutCardData[bHandOutUser],m_HandOutCardCount[bHandOutUser]);
		
		return bLegality;
	}
	

	return false;
}


//�Զ�����
bool CGameClientDlg::AutomatismOutCard()
{
	//��������
	tagOutCardResult OutCardResult;
	ZeroMemory(&OutCardResult,sizeof(OutCardResult));

	//��������
	BYTE cbTurnCardCount=(m_wCurrentUser==m_wFirstOutUser)?0:m_cbOutCardCount[m_wFirstOutUser][0];
	
	m_GameLogic.SearchOutCard(m_cbHandCardData,m_cbHandCardCount,m_cbOutCardData[m_wFirstOutUser][0],cbTurnCardCount,OutCardResult);

	//���ñ���
	KillGameTimer(IDI_OUT_CARD);
	m_wCurrentUser=INVALID_CHAIR;

	//���ÿؼ�
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btOutPrompt.ShowWindow(SW_HIDE);

	//�����˿�
	if (m_wFirstOutUser==GetMeChairID())
	{
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if (i!=2) m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
		}
	}

	WORD bHandOutUser=(m_wBankerUser+2)%GAME_PLAYER;
	WORD wViewChairID=SwitchViewChairID(bHandOutUser);
	
	//��������
	CMD_C_OutCard OutCard;
	OutCard.bIsHandOut[GetMeChairID()]=false;
	OutCard.bIsAutoOut=true;
	OutCard.cbCardCount=OutCardResult.cbCardCount;
	CopyMemory(OutCard.cbCardData,OutCardResult.cbResultCard,sizeof(BYTE)*OutCard.cbCardCount);
	SendData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard));

	//Ԥ����ʾ
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	m_GameClientView.m_UserCardControl[2].SetCardData(OutCard.cbCardData,OutCard.cbCardCount);

	//���ֿ���ʧЧ
	m_bHandOutControl=false;
	m_GameClientView.m_HandOutCardControl[m_wBankerUser][0].SetHandOutControl(m_bHandOutControl);
	/*if(m_bHandOutControl)
	{
		ASSERT(1>2);
		BYTE cbSourceCount=m_HandOutCardCount[bHandOutUser];
		m_HandOutCardCount[bHandOutUser]-=OutCard.cbCardCount;
		m_GameLogic.RemoveCard(OutCard.cbCardData,OutCard.cbCardCount,m_HandOutCardData[bHandOutUser],cbSourceCount);
			
		SetHandOutCardControl(m_HandOutCardData[bHandOutUser],m_HandOutCardCount[bHandOutUser],m_wBankerUser,0);
		//���ֿ���ʧЧ
		m_bHandOutControl=false;
	}
	else
	{*/
		
		//Ԥ��ɾ��
		BYTE cbSourceCount=m_cbHandCardCount;
		m_cbHandCardCount-=OutCard.cbCardCount;
		m_GameLogic.RemoveCard(OutCard.cbCardData,OutCard.cbCardCount,m_cbHandCardData,cbSourceCount);

		//�����˿�
		SetHandCardControl(m_cbHandCardData,m_cbHandCardCount);
	//}

	return true;
}

//��ǰ�˿�
bool CGameClientDlg::SwitchToCurrentCard()
{
	//�����ж�
	if (m_bLastTurn==false) return false;

	//��������
	m_bLastTurn=false;
	KillTimer(IDI_LAST_TURN_CARD);
	m_GameClientView.SetLastTurnCard(false);

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=SwitchViewChairID(i);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(m_cbOutCardData[i][0],m_cbOutCardCount[i][0]);
	}

	return true;
}

//˦�ƽ��
bool CGameClientDlg::SwitchToThrowResult()
{
	//�����ж�
	if (m_bThrowCard==false) return false;
	if (m_wThrowCardUser==INVALID_CHAIR) return false;

	//ʱ������
	KillTimer(IDI_THROW_CARD_RESULT);

	//���ý���
	WORD wViewChairID=SwitchViewChairID(m_wThrowCardUser);
	m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(m_cbResultCardData,m_cbResultCardCount);

	//��������
	if (m_cbOutCardCount[0][1]!=0) m_GameClientView.m_btLastTurnCard.EnableWindow(TRUE);

	//���ñ���
	m_bThrowCard=false;
	m_cbResultCardCount=0;
	m_wThrowCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbResultCardData,sizeof(m_cbResultCardData));

	return true;
}



//�ɷ��˿�
bool CGameClientDlg::DispatchUserCard(BYTE cbCardData[], BYTE cbCardCount)
{
	//���ñ���
	m_cbDispatchCardCount=0;
	m_cbRemnantCardCount=cbCardCount;
	CopyMemory(m_cbDispatchCardData,cbCardData,sizeof(BYTE)*cbCardCount);


	//���ö�ʱ��
	SetTimer(IDI_DISPATCH_CARD,100,NULL);

	return true;
}



//�����˿�
bool CGameClientDlg::SetHandCardControl(BYTE cbCardData[], BYTE cbCardCount)
{
	//���ù���
	if (cbCardCount>0)
	{
		//��������
		BYTE cbRectifyCount=1;
		BYTE cbRectifyCard[MAX_COUNT+4]={cbCardData[0]};

		//��ȡ����
		BYTE cbLogicColor=m_GameLogic.GetCardLogicColor(cbCardData[0]);

		//�任�˿�
		for (BYTE i=1;i<cbCardCount;i++)
		{
			//��ȡ����
			BYTE cbCurrentColor=m_GameLogic.GetCardLogicColor(cbCardData[i]);

			//�����ж�
			if (cbCurrentColor!=cbLogicColor)
			{
				cbLogicColor=cbCurrentColor;
				cbRectifyCard[cbRectifyCount++]=255;
				cbRectifyCard[cbRectifyCount++]=cbCardData[i];
			}
			else cbRectifyCard[cbRectifyCount++]=cbCardData[i];
		}

		//�����˿�
		m_GameClientView.m_HandCardControl.SetCardData(cbRectifyCard,cbRectifyCount);

		return true;
	}
	else m_GameClientView.m_HandCardControl.SetCardData(NULL,0);
	
	return true;
}

//���������˿�
bool CGameClientDlg::SetHandOutCardControl(BYTE cbCardData[], BYTE cbCardCount, WORD wMeChairID, WORD wViewChairID)
{
	//���ù���
	if (cbCardCount>0)
	{
		//��������
		BYTE cbRectifyCount=1;
		BYTE cbRectifyCard[MAX_COUNT+4]={cbCardData[0]};

		//��ȡ����
		BYTE cbLogicColor=m_GameLogic.GetCardLogicColor(cbCardData[0]);

		//�任�˿�
		for (BYTE i=1;i<cbCardCount;i++)
		{
			//��ȡ����
			BYTE cbCurrentColor=m_GameLogic.GetCardLogicColor(cbCardData[i]);

			//�����ж�
			if (cbCurrentColor!=cbLogicColor)
			{
				cbLogicColor=cbCurrentColor;
				cbRectifyCard[cbRectifyCount++]=255;
				cbRectifyCard[cbRectifyCount++]=cbCardData[i];
			}
			else cbRectifyCard[cbRectifyCount++]=cbCardData[i];
		}

		//�����˿�
		m_GameClientView.m_HandOutCardControl[wMeChairID][wViewChairID].SetCardData(cbRectifyCard,cbRectifyCount);

		return true;
	}
	else m_GameClientView.m_HandOutCardControl[wMeChairID][wViewChairID].SetCardData(NULL,0);
	
	return true;
}

//��ʱ����Ϣ
void CGameClientDlg::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
	case IDI_DISPATCH_CARD:		//�ɷ��˿�
		{
			//�ɷ�����
			if (m_cbRemnantCardCount>0)
			{
				//��ȡ����
				BYTE cbCardColor=m_GameLogic.GetCardColor(m_cbDispatchCardData[m_cbDispatchCardCount]);
				BYTE cbCardValue=m_GameLogic.GetCardValue(m_cbDispatchCardData[m_cbDispatchCardCount]);

				//�ɷ��˿�
				m_cbRemnantCardCount--;
				m_cbDispatchCardCount++;

				m_GameLogic.SortCardList(m_cbDispatchCardData,m_cbDispatchCardCount);
				SetHandCardControl(m_cbDispatchCardData,m_cbDispatchCardCount);
				
				//��������
				PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
			}
			
			//��ֹ�ж�
			if (m_cbRemnantCardCount==0) 
			{
				//ɾ����ʱ��
				KillTimer(IDI_DISPATCH_CARD);
				
				//��������
				
				if ((IsLookonMode()==false)&&(m_cbFocusUser==GetMeChairID())) 
				{
					//ActiveGameFrame();
					//����ʱ��
					SetGameTimer(m_cbFocusUser,IDI_LAND_SCORE,TIME_CALL_SCORE);
					m_GameClientView.m_CallScoreInfo.ShowWindow(SW_HIDE);
					m_GameClientView.m_CallScore.ShowWindow(SW_SHOW);
				
				}
			}

			return;
		}
	case IDI_LAST_TURN_CARD:	//�����˿�
		{
			//��ԭ����
			SwitchToCurrentCard();

			return;
		}

	
	case IDI_THROW_CARD_RESULT:	//˦�ƽ��
		{
			//��ԭ����
			SwitchToThrowResult();

			//��ǰ���
			if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
			{
				if (m_bStustee==false) //ActiveGameFrame();
				m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
				m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
			}

			//���ö�ʱ��
			SetGameTimer(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);

			return;
		}
	}

//Doctor	__super::OnTimer(nIDEvent);
}

//��ʼ��ť
LRESULT CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	//���û���
	KillGameTimer(IDI_START_GAME);
	
	//���ý���
	m_GameClientView.SetBankerUser(INVALID_CHAIR);
	m_cbValueOrder[0]=VALUE_ERROR;
	m_cbValueOrder[1]=VALUE_ERROR;
	m_cbMainValue=VALUE_ERROR;
	m_GameClientView.SetValueOrder(m_cbValueOrder[0],m_cbValueOrder[1]);
	m_GameClientView.SetDingYue(m_cbMainValue,VALUE_ERROR);
	m_GameClientView.SetJuKuang(VALUE_ERROR);

	//���ؿؼ�
	m_GameClientView.m_btStart.ShowWindow(FALSE);
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);

	//�зֿ���
	m_GameClientView.m_CallScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_CallScoreInfo.ShowWindow(SW_HIDE);

	//�����˿�
	SetHandCardControl(NULL,0);
	m_GameClientView.m_CardScore.SetCardData(NULL,0);
	m_GameClientView.m_HandCardControl.SetPositively(false);
	m_GameClientView.SetMeChair(GetMeChairID());

	//�����˿�
	ZeroMemory(m_HandOutCardCount,sizeof(m_HandOutCardCount));	
	ZeroMemory(m_HandOutCardData,sizeof(m_HandOutCardData));
	m_bHandOutControl=false;

	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		for (WORD j=0;j<GAME_PLAYER;j++)
		{
			m_GameClientView.m_HandOutCardControl[i][j].SetCardData(NULL,0);
		}
	}
	for (WORD i=0;i<GAME_PLAYER;i++) m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);

	//������Ϣ
	SendUserReady(NULL,0);

	return 0;
}

//�з���Ϣ
LRESULT CGameClientDlg::OnLandScore(WPARAM wParam, LPARAM lParam)
{
	//���ý���
	m_GameClientView.m_CallScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_CallScoreInfo.ShowWindow(SW_SHOW);

	//��������
	CMD_C_CallScore LandScore;
	
	LandScore.bBossScore=(BYTE)lParam;
	LandScore.bBossColor=(BYTE)wParam;
	
	SendData(SUB_C_CALL_SCORE,&LandScore,sizeof(LandScore));
	return 0;
}

//�ӱ���Ϣ
LRESULT CGameClientDlg::OnMultiples(WPARAM wParam, LPARAM lParam)
{
	//���ý���
	m_GameClientView.m_CallScore.ShowWindow(SW_HIDE);


	//��������
	CMD_C_OnMultiples Multiples;
	Multiples.bMultiples=(BYTE)lParam;

	SendData(SUB_C_MULTIPLES,&Multiples,sizeof(Multiples));
	return 0;
}


//������Ϣ
LRESULT CGameClientDlg::OnOutCard(WPARAM wParam, LPARAM lParam)
{
	//״̬�ж�
	if (m_GameClientView.m_btOutCard.IsWindowEnabled()==FALSE) return 0;
	if (m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE) return 0;

	//���ñ���
	KillGameTimer(IDI_OUT_CARD);
	m_wCurrentUser=INVALID_CHAIR;

	//���ÿؼ�
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btOutPrompt.ShowWindow(SW_HIDE);

	//�������ֱ���
	WORD wMeChairID=GetMeChairID();
	BYTE bHandOutUser=(m_wBankerUser+2)%GAME_PLAYER;
	WORD wViewChairID=SwitchViewChairID(bHandOutUser);

	//�����˿�
	if(m_bHandOutControl==true)
	{
		if (m_wFirstOutUser==bHandOutUser)
		{
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if (i!=wViewChairID) m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
			}
		}
		//��������
		CMD_C_OutCard OutCard;
		OutCard.bIsHandOut[GetMeChairID()]=m_bHandOutControl;
		OutCard.bIsAutoOut=false;
		OutCard.cbCardCount=(BYTE)m_GameClientView.m_HandOutCardControl[wMeChairID][wViewChairID].GetShootCard(OutCard.cbCardData,CountArray(OutCard.cbCardData));
		SendData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard));
		
		PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

		//Ԥ��ɾ��
		//BYTE cbSourceCount=m_HandOutCardCount[bHandOutUser];
		//m_HandOutCardCount[bHandOutUser]-=OutCard.cbCardCount;
		//m_GameLogic.RemoveCard(OutCard.cbCardData,OutCard.cbCardCount,m_HandOutCardData[bHandOutUser],cbSourceCount);
		//
		//SetHandOutCardControl(m_HandOutCardData[bHandOutUser],m_HandOutCardCount[bHandOutUser],wMeChairID,wViewChairID);
		
	}
	else
	{
		if (m_wFirstOutUser==GetMeChairID())
		{
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if (i!=2) m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
			}
		}
		//��������
		CMD_C_OutCard OutCard;
		OutCard.bIsAutoOut=false;
		OutCard.bIsHandOut[GetMeChairID()]=m_bHandOutControl;
		OutCard.cbCardCount=(BYTE)m_GameClientView.m_HandCardControl.GetShootCard(OutCard.cbCardData,CountArray(OutCard.cbCardData));
		//m_GameLogic.SortCardList(OutCard.cbCardData,OutCard.cbCardCount);
		SendData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard));

		//Ԥ����ʾ
		PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
		m_GameClientView.m_UserCardControl[2].SetCardData(OutCard.cbCardData,OutCard.cbCardCount);

		//Ԥ��ɾ��
		BYTE cbSourceCount=m_cbHandCardCount;
		m_cbHandCardCount-=OutCard.cbCardCount;
		m_GameLogic.RemoveCard(OutCard.cbCardData,OutCard.cbCardCount,m_cbHandCardData,cbSourceCount);

		//�����˿�
		SetHandCardControl(m_cbHandCardData,m_cbHandCardCount);
	}
	
	//���ֿ���ʧЧ
	m_bHandOutControl=false;
	m_GameClientView.m_HandOutCardControl[wMeChairID][wViewChairID].SetHandOutControl(m_bHandOutControl);
	return 0;
}

//������ʾ
LRESULT CGameClientDlg::OnOutPrompt(WPARAM wParam, LPARAM lParam)
{
	//��������
	tagOutCardResult OutCardResult;
	ZeroMemory(&OutCardResult,sizeof(OutCardResult));

	//��������
	BYTE cbTurnCardCount=(m_wCurrentUser==m_wFirstOutUser)?0:m_cbOutCardCount[m_wFirstOutUser][0];
	bool bSuccess=m_GameLogic.SearchOutCard(m_cbHandCardData,m_cbHandCardCount,m_cbOutCardData[m_wFirstOutUser][0],cbTurnCardCount,OutCardResult);

	//ʧ���ж�
	if (bSuccess==false)
	{
		InsertSystemString(TEXT("�Զ���ʾ���ƴ�������Ѵ�״̬��Ϣ��ͼ���������ṩ��лл��"));
		return false;
	}

	//���ý���
	m_GameClientView.m_btOutCard.EnableWindow(TRUE);
	m_GameClientView.m_HandCardControl.SetShootCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount);

	return 0;
}


//�����뿪
LRESULT CGameClientDlg::OnRuquestLeave(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

//�����˿�
LRESULT CGameClientDlg::OnLastTurnCard(WPARAM wParam, LPARAM lParam)
{
	if (m_bLastTurn==false)
	{
		//�����˿�
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			WORD wViewChairID=SwitchViewChairID(i);
			m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(m_cbOutCardData[i][1],m_cbOutCardCount[i][1]);
		}

		//��������
		m_bLastTurn=true;
		m_GameClientView.SetLastTurnCard(true);
		SetTimer(IDI_LAST_TURN_CARD,3000,NULL);
	}
	else SwitchToCurrentCard();

	return 0;
}

//�Ϲܿ���
LRESULT CGameClientDlg::OnStusteeControl(WPARAM wParam, LPARAM lParam)
{
	//����״̬
	if (m_bStustee==true)
	{
		m_bStustee=false;
		m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_START_TRUSTEE,AfxGetInstanceHandle(),false);
	}
	else
	{
		m_bStustee=true;
		m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_STOP_TRUSTEE,AfxGetInstanceHandle(),false);
	}

	return 0;
}

//����˿�
LRESULT CGameClientDlg::OnLeftHitCard(WPARAM wParam, LPARAM lParam)
{
	if (m_bHandOutControl==false)
	{
		//��ȡ״̬
		BYTE cbGameStatue=GetGameStatus();
		
		//����״̬
		switch (cbGameStatue)
		{
		
		case GS_UG_PLAY:	//��Ϸ״̬
			{
				//���ÿؼ�
				bool bOutCard=VerdictOutCard();
				m_GameClientView.m_btOutCard.EnableWindow(bOutCard?TRUE:FALSE);

				return 0;
			}
		}
	}
	return 0;
}

//�Ҽ��˿�
LRESULT CGameClientDlg::OnRightHitCard(WPARAM wParam, LPARAM lParam)
{
	if (m_bHandOutControl==true)
	{
		//��ȡ״̬
		BYTE cbGameStatue=GetGameStatus();
		
		//����״̬
		switch (cbGameStatue)
		{
		
		case GS_UG_PLAY:	//��Ϸ״̬
			{
				//���ÿؼ�
				bool bOutCard=VerdictHandOutCard();
				
				m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btOutCard.EnableWindow(bOutCard==true?TRUE:FALSE);

				return 0;
			}
		}
	//OnOutCard(0,0);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
