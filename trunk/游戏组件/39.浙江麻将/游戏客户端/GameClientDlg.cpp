#include "Stdafx.h"
#include "GameOption.h"
#include "GameClient.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ʱ��
#define IDI_START_GAME				200									//��ʼ��ʱ��
#define IDI_OUT_CARD				201									//���ƶ�ʱ��
#define IDI_OPERATE_CARD			202									//������ʱ��

//#define GENDER_GIRL					0									//�Ա�Ů

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_CONSIGN_CHANGED,OnConsignChanged)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_CARD_OPERATE,OnCardOperate)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg()
:	CGameFrameDlg(&m_GameClientView),
	m_GameClientView(this)
{
	//��Ϸ����
	m_wHeapHand=0;
	m_wHeapTail=0;
	m_bChiHu=true;
	m_bLeftCardCount=0;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_lScoreLeft, sizeof(m_lScoreLeft));
	m_lCellScore = 0;

	//������Ϣ
	m_bOutCardCount=0;
	ZeroMemory(m_wOutCardUser,sizeof(m_wOutCardUser));
	ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));

	//����˿�
	ZeroMemory(m_bWeaveCount,sizeof(m_bWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�˿˱���
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bHeapCardInfo,sizeof(m_bHeapCardInfo));

	//���Ա���
	m_bSelfReady = false;
	ZeroMemory(m_bReady, sizeof(m_bReady));
	m_cbOverTimeCount	= 0;
	m_bConsign			= false;

	m_wSiceCount = 0;

	return;
}

//��������
CGameClientDlg::~CGameClientDlg()
{
}

//��ʼ����
bool CGameClientDlg::InitGameFrame()
{
#ifdef MJ_DESPERATE
	//���ñ���
	SetWindowText(TEXT("����齫  --  Ver��6. 0. 9. 0"));
#else
	SetWindowText(TEXT("�����齫  --  Ver��6. 5. 1. 0"));
#endif
	

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//������Դ
	g_CardResource.LoadResource();

	return true;
}

//���ÿ��
void CGameClientDlg::ResetGameFrame()
{
	//��������
	KillGameTimer(IDI_START_GAME);

	//��Ϸ����
	m_wHeapHand=0;
	m_wHeapTail=0;
	m_bChiHu=true;
	m_bLeftCardCount=0;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_lScoreLeft, sizeof(m_lScoreLeft));
	m_lCellScore = 0;

	//������Ϣ
	m_bOutCardCount=0;
	ZeroMemory(m_wOutCardUser,sizeof(m_wOutCardUser));
	ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));

	//����˿�
	ZeroMemory(m_bWeaveCount,sizeof(m_bWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�˿˱���
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bHeapCardInfo,sizeof(m_bHeapCardInfo));

	return;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{
	//��������
	CGameOption GameOption;
	GameOption.m_bEnableSound = IsEnableSound();
	GameOption.m_bAllowLookon = IsAllowLookOn();

	//��������
	if (GameOption.DoModal()==IDOK)
	{
		//��������
		EnableSound(GameOption.m_bEnableSound);
		AllowUserLookon(0,GameOption.m_bAllowLookon);
	}

	return;
}

//ʱ����Ϣ
bool CGameClientDlg::OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_START_GAME:		//��ʼ��Ϸ
		{
			if (nElapse==0)
			{
				OnStart(0,0);
				return false;
			}
			if (nElapse<=5) 
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			}

			return true;
		}
	case IDI_OUT_CARD:			//�û�����
		{
			//��ʱ�ж�
			if (nElapse==0)
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) 
				{
					BYTE cbCardData = m_GameClientView.m_HandCardControl.GetCurrentCard();
#ifdef _AUTOPALY

					 cbCardData = GetIsolatedCard();
					if(cbCardData == 0xFF)
					{
						cbCardData = m_GameClientView.m_HandCardControl.GetCurrentCard();
					}
#endif
					if(cbCardData == CARD_KING_DATA_MASK)
					{
						return false;
					}
					OnOutCard(cbCardData,cbCardData);

#ifndef _AUTOPALY
					//�����й�
					if(IsConsign() == false)
					{
						m_cbOverTimeCount ++;
						if(m_cbOverTimeCount >= 3)
						{
							SetConsign(true);							
						}
						else
						{
							CString strTmp;
							strTmp.Format("����%d�γ��Ƴ�ʱ����ʱ�������κ�ϵͳ���Զ��йܣ�", m_cbOverTimeCount);
							InsertSystemString(strTmp);
						}
					}
#endif
				}
				return false;
			}

			//��������
			if ((nElapse<=5)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) 
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			}

			return true;
		}
	case IDI_OPERATE_CARD:		//������ʱ��
		{
			//��ʱ�ж�
			if (nElapse==0)
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) 
				{
					OnCardOperate(ACK_NULL,0);
				}
				return false;
			}

			//��������
			if ((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) 
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			}

			return true;
		}
	}

	return false;
}

//�Թ�״̬
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{
}

//������Ϣ
bool CGameClientDlg::OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:			//��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_OUT_CARD:			//�û�����
		{
			return OnSubOutCard(pBuffer,wDataSize);
		}
	case SUB_S_OPERATE_CARD:		//�����˿�
		{
			return OnSubOperateCard(pBuffer,wDataSize);
		}
	case SUB_S_SEND_CARD:			//�����˿�
		{
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:			//��Ϸ����
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_SYS_MSG:				//ϵͳ��Ϣ
		{
			return OnSubSysMsg(pBuffer, wDataSize);
		}
	case SUB_S_USER_READY:			//׼���ź�
		{
			return OnSubUserReady(pBuffer, wDataSize);
		}
	case SUB_S_STAKE:				//ע��ı�
		{
			return OnSubStake(pBuffer, wDataSize);
		}
	}

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GS_FREE:		//����״̬
	case GS_FREE_FAKE:
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//fakeһ�����������Ϣ
			CMD_S_Stake Msg;
			Msg.lCellScore = pStatusFree->lCellScore;
			CopyMemory(Msg.lLeftScore, pStatusFree->lLeftScore, sizeof(Msg.lLeftScore));
			OnSubStake(&Msg, sizeof(CMD_S_Stake));

			//��������
			m_wBankerUser=pStatusFree->wBankerUser;
			m_GameClientView.m_HandCardControl.SetDisplayItem(true);

			//���ý���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_bHeapCardInfo[i][0]=0;
				m_bHeapCardInfo[i][1]=0;
				m_GameClientView.m_HeapCard[SwitchViewChairID(i)].SetCardData(m_bHeapCardInfo[i][0],m_bHeapCardInfo[i][1],HEAP_FULL_COUNT);
			}

			//���ÿؼ�
			if (IsLookonMode()==false)
			{
				m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
				m_GameClientView.m_btStart.SetFocus();
				m_GameClientView.m_btConsignEnter.ShowWindow(SW_SHOW);
				m_GameClientView.m_btConsignCancel.ShowWindow(SW_SHOW);

				SetGameTimer(GetMeChairID(),IDI_START_GAME,30);
			}

			//���½���
			m_GameClientView.UpdateGameView(NULL);

			return true;
		}
	case GS_PLAYING:	//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//fakeһ�����������Ϣ
			CMD_S_Stake Msg;
			Msg.lCellScore = pStatusPlay->lCellScore;
			CopyMemory(Msg.lLeftScore, pStatusPlay->lLeftScore, sizeof(Msg.lLeftScore));
			OnSubStake(&Msg, sizeof(CMD_S_Stake));

			//���ñ���
			m_wBankerUser=pStatusPlay->wBankerUser;
			m_wCurrentUser=pStatusPlay->wCurrentUser;
			m_bOutCardCount=pStatusPlay->cbOutCardCount;
			m_bLeftCardCount=pStatusPlay->cbLeftCardCount;
			m_bChiHu=(pStatusPlay->cbChihu>0)?true:false;
			CopyMemory(m_wOutCardUser,pStatusPlay->wOutCardUser,sizeof(m_wOutCardUser));
			CopyMemory(m_cbOutCardData,pStatusPlay->cbOutCardData,sizeof(m_cbOutCardData));
			CopyMemory(m_bWeaveCount,pStatusPlay->bWeaveCount,sizeof(m_bWeaveCount));
			CopyMemory(m_WeaveItemArray,pStatusPlay->WeaveItemArray,sizeof(m_WeaveItemArray));
			m_GameLogic.SetKingCardData(pStatusPlay->cbKingCardData);
			m_GameLogic.SwitchToCardIndex(pStatusPlay->cbCardData,pStatusPlay->cbCardCount,m_cbCardIndex);

			//��������
			WORD wViewChairID[GAME_PLAYER]={0,0,0,0};
			for (WORD i=0;i<GAME_PLAYER;i++) wViewChairID[i]=SwitchViewChairID(i);

			//��������
			m_GameClientView.SetBankerUser(wViewChairID[m_wBankerUser]);

			//����˿�
			BYTE cbGangCount=0;
			BYTE cbWeaveCard[4]={0,0,0,0};
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				WORD wOperateViewID = SwitchViewChairID(i);
				for (BYTE j=0;j<m_bWeaveCount[i];j++)
				{
					BYTE cbWeaveKind=m_WeaveItemArray[i][j].cbWeaveKind;
					BYTE cbCenterCard=m_WeaveItemArray[i][j].cbCenterCard;
					BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCardData(cbWeaveKind,cbCenterCard,cbWeaveCard);
					m_GameClientView.m_WeaveCard[wViewChairID[i]][j].SetCardData(cbWeaveCard,cbWeaveCardCount);
					if (cbWeaveKind==ACK_GANG)
					{
						cbGangCount++;
						if (m_WeaveItemArray[i][j].wProvideUser==i) m_GameClientView.m_WeaveCard[wViewChairID[i]][j].SetDisplayItem(false);
					}
					WORD wProviderViewID = SwitchViewChairID(m_WeaveItemArray[i][j].wProvideUser);
					m_GameClientView.m_WeaveCard[wOperateViewID][j].SetDirectionCardPos(3-(wOperateViewID-wProviderViewID+4)%4);
				}
			}

			//�û��˿�
			if (m_wCurrentUser==GetMeChairID())
			{
				BYTE cbCardCount=pStatusPlay->cbCardCount;
				//m_GameClientView.m_HandCardControl.SetCardData(pStatusPlay->cbCardData,cbCardCount-1,pStatusPlay->cbCardData[cbCardCount-cbCardCount]);
				SetHandCardData(pStatusPlay->cbCardData,cbCardCount-1,pStatusPlay->cbCardData[cbCardCount-1]);
			}
			else 
			{
				//m_GameClientView.m_HandCardControl.SetCardData(pStatusPlay->cbCardData,pStatusPlay->cbCardCount,0); 
				SetHandCardData(pStatusPlay->cbCardData,pStatusPlay->cbCardCount,0); 
			}

			//�û��˿�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if (i!=GetMeChairID())
				{
					BYTE cbCardCount=13-m_bWeaveCount[i]*3;
					WORD wUserCardIndex=(wViewChairID[i]<2)?wViewChairID[i]:2;
					m_GameClientView.m_UserCard[wUserCardIndex].SetCardData(cbCardCount,(m_wCurrentUser==i));
				}
			}

			//�����˿�
			for (WORD i=0;i<m_bOutCardCount;i++)
			{
				WORD wOutUser=m_wOutCardUser[i];
				BYTE cbCardData=m_cbOutCardData[i];
				m_GameClientView.m_DiscardCard[wViewChairID[wOutUser]].AddCardItem(cbCardData);
			}

			//��������
			if (IsLookonMode()==false)
			{
				m_GameClientView.m_HandCardControl.SetPositively(true);
				m_GameClientView.m_HandCardControl.SetDisplayItem(true);
			}

			//�����˿�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_bHeapCardInfo[i][0]=0;
				m_bHeapCardInfo[i][1]=0;
			}

			//�ַ��˿�
			m_wSiceCount = pStatusPlay->wSiceCount;
			BYTE bTakeCount=136-m_bLeftCardCount-cbGangCount;
			//BYTE bSiceFirst=HIBYTE(pGameStart->wSiceCount);
			//BYTE bSiceSecond=LOBYTE(pGameStart->wSiceCount);
			BYTE bSiceCount = HIBYTE(pStatusPlay->wSiceCount) + LOBYTE(pStatusPlay->wSiceCount);
			//WORD wTakeChairID=(m_wBankerUser+7-bSiceFirst)%GAME_PLAYER;
			WORD wTakeChairID = (m_wBankerUser + 3*bSiceCount - 3 - (bSiceCount-1)/GAME_PLAYER*GAME_PLAYER*3)%GAME_PLAYER;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//������Ŀ
				//BYTE bValidCount=HEAP_FULL_COUNT-m_bHeapCardInfo[wTakeChairID][1]-((i==0)?(bSiceSecond-1)*2:0);
				BYTE bValidCount=HEAP_FULL_COUNT-m_bHeapCardInfo[wTakeChairID][1]-((i==0)?(bSiceCount)*2:0);
				BYTE bRemoveCount=__min(bValidCount,bTakeCount);

				//��ȡ�˿�
				bTakeCount-=bRemoveCount;
				m_bHeapCardInfo[wTakeChairID][(i==0)?1:0]+=bRemoveCount;

				//����ж�
				if (bTakeCount==0)
				{
					m_wHeapHand=wTakeChairID;
					//m_wHeapTail=(m_wBankerUser+7-bSiceFirst)%GAME_PLAYER;
					m_wHeapTail = (m_wBankerUser + 3*bSiceCount - 3 - (bSiceCount-1)/GAME_PLAYER*GAME_PLAYER*3)%GAME_PLAYER;
					break;
				}

				//�л�����
				wTakeChairID=(wTakeChairID+1)%GAME_PLAYER;
			}

			//���Ʋ���
			for (BYTE i=0;i<cbGangCount;i++) DeductionTableCard(false);

			//���ò���ĵܵ�
			m_wKingBrotherHeapPos = (m_wHeapTail+3)%4;
			m_wKingBrotherCount = bSiceCount;
			WORD wHeapViewID=SwitchViewChairID(m_wKingBrotherHeapPos);
			for(i=0; i<GAME_PLAYER; i++)
			{
				if(i == wHeapViewID)
				{
					m_GameClientView.m_HeapCard[i].SetKingBrotherCount(m_wKingBrotherCount);
				}
				else
				{
					m_GameClientView.m_HeapCard[i].SetKingBrotherCount(0);
				}
			}
	
			//��������
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.m_HeapCard[wViewChairID[i]].SetCardData(m_bHeapCardInfo[i][0],m_bHeapCardInfo[i][1],HEAP_FULL_COUNT);
			}
			
			//ʱ������
			if ((m_wCurrentUser==INVALID_CHAIR)&&(IsLookonMode()==false)&&(m_bLeftCardCount>1))
			{
				//����ʱ��
				ASSERT(m_bOutCardCount > 0);
				if ((m_bOutCardCount>0)&&(pStatusPlay->cbResponse==FALSE))
				{
					//��������
					WORD wMeChairID=GetMeChairID();
					BYTE cbOutCardData = m_cbOutCardData[m_bOutCardCount-1];
					WORD wOutCardUser = m_wOutCardUser[m_bOutCardCount-1];

					//״̬�ж�
					BYTE cbActionMask=m_GameLogic.GetPengCardKind(m_cbCardIndex,cbOutCardData);
					if (m_bLeftCardCount>1)
					{
						cbActionMask|=m_GameLogic.GetGangCardKind(m_cbCardIndex,cbOutCardData);
					}

					//�����ж�
					WORD wEatUser=(wOutCardUser+GAME_PLAYER-1)%GAME_PLAYER;
					if (wEatUser == wMeChairID) 
					{
						cbActionMask|=m_GameLogic.GetEatCardKind(m_cbCardIndex,cbOutCardData);
					}

					//�����ж�
					if (m_bChiHu==true)
					{
						tagHuInput HuInput;
						HuInput.bJudgeHuOnly		= true;
						HuInput.bJudgeKingWait		= false;
						HuInput.bInsertCurrentCard	= true;
						HuInput.cbCurrentCard		= cbOutCardData;
						HuInput.pWeaveItemSeq		= m_WeaveItemArray[wMeChairID];
						HuInput.cbWeaveItemCount	= m_bWeaveCount[wMeChairID];
						HuInput.pCardIndexSeq		= m_cbCardIndex;
						tagHuCourt HuCourt;
						if(m_GameLogic.GetHuInfo(&HuInput, HuCourt))
						{
							cbActionMask |= ACK_CHIHU;
							m_bChiHu = false;
						}
						
					}

					//���ƽ���
					if (cbActionMask!=ACK_NULL)
					{

						SetGameTimer(wMeChairID,IDI_OPERATE_CARD,10);
						m_GameClientView.m_ControlWnd.SetControlInfo(cbOutCardData,cbActionMask,NULL);
					}
					else
					{
						InsertSystemString("OnGameSceneMessage : ��Ȼû���жϵ�");
					}
				}
				else
				{
					CString str;
					str.Format("OnGameSceneMessage : m_bOutCardCount == %d, pStatusPlay->cbResponse == %d", m_bOutCardCount, pStatusPlay->cbResponse);
					InsertSystemString(str);
				}
			}
			if (m_wCurrentUser!=INVALID_CHAIR) 
			{
				SetGameTimer(m_wCurrentUser,IDI_OUT_CARD,20);
			}

			//��������
			if (m_bOutCardCount>0)
			{
				WORD wOutUser=m_wOutCardUser[m_bOutCardCount-1];
				BYTE cbOutCardData=m_cbOutCardData[m_bOutCardCount-1];
				m_GameClientView.SetOutCardInfo(wViewChairID[wOutUser],cbOutCardData);
			}

			//���ÿؼ�
			if (IsLookonMode()==false)
			{
				m_GameClientView.m_btConsignEnter.ShowWindow(SW_SHOW);
				m_GameClientView.m_btConsignCancel.ShowWindow(SW_SHOW);
			}

			//���½���
			m_GameClientView.UpdateGameView(NULL);

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

	//��λ����
	ZeroMemory(m_bReady, sizeof(m_bReady));

	//��������
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//��������
	m_bChiHu=true;
	m_bOutCardCount=0;
	m_bLeftCardCount=55;
	m_wBankerUser=pGameStart->wBankerUser;
	m_wCurrentUser=pGameStart->wCurrentUser;

	//��������
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bWeaveCount,sizeof(m_bWeaveCount));

	ZeroMemory(m_wOutCardUser,sizeof(m_wOutCardUser));
	ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
	ZeroMemory(m_bHeapCardInfo,sizeof(m_bHeapCardInfo));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�����˿�
	m_GameLogic.SetKingCardData(pGameStart->cbKingCardData);
	BYTE cbCardCount=(GetMeChairID()==m_wBankerUser)?MAX_COUNT:(MAX_COUNT-1);
	m_GameLogic.SwitchToCardIndex(pGameStart->cbCardData,cbCardCount,m_cbCardIndex);

	//���ý���
	bool bPlayerMode=(IsLookonMode()==false);
	m_GameClientView.m_GameScoreWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_HandCardControl.SetPositively(bPlayerMode);
	m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

	//�Թ۽���
	if (bPlayerMode==false)
	{
		m_GameClientView.SetStatusFlag(false,false);
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
		m_GameClientView.m_GameScoreWnd.ShowWindow(SW_HIDE);
	}

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_bHeapCardInfo[i][0]=0;
		m_bHeapCardInfo[i][1]=0;
	}

	//�ַ��˿�
	m_wSiceCount = pGameStart->wSiceCount;
	BYTE bTakeCount=53;
	//BYTE bSiceFirst=HIBYTE(pGameStart->wSiceCount);
	//BYTE bSiceSecond=LOBYTE(pGameStart->wSiceCount);
	BYTE bSiceCount = HIBYTE(pGameStart->wSiceCount) + LOBYTE(pGameStart->wSiceCount);
	//WORD wTakeChairID=(m_wBankerUser+7-bSiceFirst)%GAME_PLAYER;
	WORD wTakeChairID = (m_wBankerUser + 3*bSiceCount - 3 - (bSiceCount-1)/GAME_PLAYER*GAME_PLAYER*3)%GAME_PLAYER;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//������Ŀ
		//BYTE bValidCount=HEAP_FULL_COUNT-m_bHeapCardInfo[wTakeChairID][1]-((i==0)?(bSiceSecond-1)*2:0);
		BYTE bValidCount=HEAP_FULL_COUNT-m_bHeapCardInfo[wTakeChairID][1]-((i==0)?(bSiceCount)*2:0);
		BYTE bRemoveCount=__min(bValidCount,bTakeCount);

		//��ȡ�˿�
		bTakeCount-=bRemoveCount;
		m_bHeapCardInfo[wTakeChairID][(i==0)?1:0]+=bRemoveCount;

		//����ж�
		if (bTakeCount==0)
		{
			m_wHeapHand=wTakeChairID;
			//m_wHeapTail=(m_wBankerUser+7-bSiceFirst)%GAME_PLAYER;
			m_wHeapTail = (m_wBankerUser + 3*bSiceCount - 3 - (bSiceCount-1)/GAME_PLAYER*GAME_PLAYER*3)%GAME_PLAYER;
			break;
		}

		//�л�����
		wTakeChairID=(wTakeChairID+1)%GAME_PLAYER;
	}

	//���ò���ĵܵ�
	m_wKingBrotherHeapPos = (m_wHeapTail+3)%4;
	m_wKingBrotherCount = bSiceCount;
	WORD wHeapViewID=SwitchViewChairID(m_wKingBrotherHeapPos);
	for(i=0; i<GAME_PLAYER; i++)
	{
		if(i == wHeapViewID)
		{
			m_GameClientView.m_HeapCard[i].SetKingBrotherCount(m_wKingBrotherCount);
		}
		else
		{
			m_GameClientView.m_HeapCard[i].SetKingBrotherCount(0);
		}
	}
	

	//CString str;
	//str.Format("Pos == %d, Count == %d , Banker == %d, bSiceFirst == %d, biSiceSecond == %d", 
	//	m_wKingBrotherHeapPos, m_wKingBrotherCount, m_wBankerUser, bSiceFirst, bSiceSecond);
	//AfxMessageBox(str);

	//�˿�����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wViewChairID=SwitchViewChairID(i);

		//��������
		m_GameClientView.m_WeaveCard[i][0].SetDisplayItem(true);
		m_GameClientView.m_WeaveCard[i][1].SetDisplayItem(true);
		m_GameClientView.m_WeaveCard[i][2].SetDisplayItem(true);
		m_GameClientView.m_WeaveCard[i][3].SetDisplayItem(true);
		m_GameClientView.m_HeapCard[wViewChairID].SetCardData(m_bHeapCardInfo[i][0],m_bHeapCardInfo[i][1],HEAP_FULL_COUNT);

		//�û��˿�
		if (wViewChairID!=2)
		{
			WORD wIndex=(wViewChairID>=3)?2:wViewChairID;
			m_GameClientView.m_UserCard[wIndex].SetCardData(CountArray(pGameStart->cbCardData),(i==m_wBankerUser));
		}
		else
		{
			BYTE cbBankerCard=(i==m_wBankerUser)?pGameStart->cbCardData[MAX_COUNT-1]:0;
			//m_GameClientView.m_HandCardControl.SetCardData(pGameStart->cbCardData,MAX_COUNT-1,cbBankerCard);
			SetHandCardData(pGameStart->cbCardData,MAX_COUNT-1,cbBankerCard);
		}

		//�Թ۽���
		if (bPlayerMode==false)
		{
			m_GameClientView.m_TableCard[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_DiscardCard[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[wViewChairID][0].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[wViewChairID][1].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[wViewChairID][2].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[wViewChairID][3].SetCardData(NULL,0);
		}
	}

	//�������
	if ((bPlayerMode==true)&&(GetMeChairID()==m_wCurrentUser))
	{
		//���ƽ���
		BYTE cbGangCard[4]={0,0,0,0};
		m_GameClientView.SetStatusFlag(true,false);

		tagHuInput HuInput;
		HuInput.bJudgeHuOnly		= true;
		HuInput.bJudgeKingWait		= false;
		HuInput.bInsertCurrentCard	= false;
		HuInput.cbCurrentCard		= 0;
		HuInput.pWeaveItemSeq		= m_WeaveItemArray[m_wCurrentUser];
		HuInput.cbWeaveItemCount	= m_bWeaveCount[m_wCurrentUser];
		HuInput.pCardIndexSeq		= m_cbCardIndex;
		tagHuCourt HuCourt;
		bool bHuCard = m_GameLogic.GetHuInfo(&HuInput, HuCourt);

		m_GameLogic.GetGangCardInfo(m_cbCardIndex,NULL,0,cbGangCard);

		//���ƽ���
		if ((cbGangCard[0]!=0)||(bHuCard==true)) 
		{
			BYTE cbActionMask=(bHuCard==true)?ACK_CHIHU:ACK_NULL;
			m_GameClientView.m_ControlWnd.SetControlInfo(0,cbActionMask,cbGangCard);
		}
	}

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	//������
	if (bPlayerMode==true)
	{
		ActiveGameFrame();
	}

	//��������
	SetGameTimer(m_wCurrentUser,IDI_OUT_CARD,20);
	
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	return true;
}

//�û�����
bool CGameClientDlg::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
	//Ч����Ϣ
	ASSERT(wDataSize==sizeof(CMD_S_OutCard));
	if (wDataSize!=sizeof(CMD_S_OutCard)) 
	{
		return false;
	}

	//��Ϣ����
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;

	//���ñ���
	WORD wMeChairID=GetMeChairID();
	m_wCurrentUser=pOutCard->wCurrentUser;
	m_wOutCardUser[m_bOutCardCount]=pOutCard->wOutCardUser;
	m_cbOutCardData[m_bOutCardCount++]=pOutCard->cbOutCardData;
	WORD wOutViewChairID=SwitchViewChairID(pOutCard->wOutCardUser);

	//��������
	if (pOutCard->wOutCardUser==wMeChairID) 
	{
		m_bChiHu=true;
	}

	//�����û�
	if ((pOutCard->wOutCardUser!=wMeChairID)||(IsLookonMode()==true))
	{
		//��������
		KillGameTimer(IDI_OUT_CARD); 
//		PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

		//���ƽ���
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetOutCardInfo(wOutViewChairID,pOutCard->cbOutCardData);

		//�����˿�
		if (wOutViewChairID==2)
		{
			BYTE cbCardData[MAX_COUNT];
			m_GameLogic.RemoveCard(m_cbCardIndex,pOutCard->cbOutCardData);
			BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData,CountArray(cbCardData));
			//m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount,0);
			SetHandCardData(cbCardData,cbCardCount,0);
		}
		else
		{
			WORD wUserIndex = (wOutViewChairID > 2) ? 2:wOutViewChairID;
			m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(false);
		}
	}

	//ȡ�ƴ���
	if (m_wCurrentUser!=INVALID_CHAIR)
	{
		//�û�ȡ��
		WORD wViewChairID=SwitchViewChairID(m_wCurrentUser);
		if (wViewChairID!=2)
		{
			WORD wUserIndex=(wViewChairID>2)?2:wViewChairID;
			m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(true);
		}
		else
		{
			m_cbCardIndex[m_GameLogic.SwitchToCardIndex(pOutCard->cbCurrentCard)]++;
			//m_GameClientView.m_HandCardControl.SetCurrentCard(pOutCard->cbCurrentCard);
			SetHandCurrentCard(pOutCard->cbCurrentCard);
		}

		//�۳��˿�
		DeductionTableCard(true);

		//�����˿�
		m_GameClientView.m_DiscardCard[wOutViewChairID].AddCardItem(pOutCard->cbOutCardData);

		//��ǰ�û�
		if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID))
		{
			//��������
			BYTE cbGangCard[4]={0,0,0,0};
			BYTE cbItemCount=m_bWeaveCount[m_wCurrentUser];
			tagWeaveItem * pWeaveItem=m_WeaveItemArray[m_wCurrentUser];

			//�����ж�
			if (m_bLeftCardCount>1) 
			{
				m_GameLogic.GetGangCardInfo(m_cbCardIndex,pWeaveItem,cbItemCount,cbGangCard);
			}

			tagHuInput HuInput;
			HuInput.bJudgeHuOnly		= true;
			HuInput.bJudgeKingWait		= false;
			HuInput.bInsertCurrentCard	= false;
			HuInput.cbCurrentCard		= 0;
			HuInput.pWeaveItemSeq		= m_WeaveItemArray[m_wCurrentUser];
			HuInput.cbWeaveItemCount	= m_bWeaveCount[m_wCurrentUser];
			HuInput.pCardIndexSeq		= m_cbCardIndex;
			tagHuCourt HuCourt;
			BYTE cbActionMask = ACK_NULL;
			if(m_GameLogic.GetHuInfo(&HuInput, HuCourt))
			{
				cbActionMask |= ACK_CHIHU;
			}

			if ((cbGangCard[0]!=0)||(cbActionMask == ACK_CHIHU) )
			{
				m_GameClientView.m_ControlWnd.SetControlInfo(0,cbActionMask,cbGangCard);
			}

			//������
			ActiveGameFrame();
		}

		//������ʾ
		m_GameClientView.SetStatusFlag((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID),false);

		//ʱ������
		SetGameTimer(m_wCurrentUser,IDI_OUT_CARD,20);
	}
	else
	{
		//��������
		if ((IsLookonMode()==false)&&(pOutCard->wOutCardUser!=wMeChairID))
		{
			BYTE cbActionMask = ACK_NULL;

#ifdef MJ_DESPERATE	

			//����齫���Ƴ���һ������������
			BYTE cbOperateSum = 0;
			for(WORD j=0; j<m_bWeaveCount[wMeChairID]; j++)
			{
				if(m_WeaveItemArray[wMeChairID][j].wProvideUser == pOutCard->wOutCardUser)
				{
					cbOperateSum ++;
				}
			}
			if(cbOperateSum < 2)
			{
				//�����ж�
				cbActionMask = m_GameLogic.GetPengCardKind(m_cbCardIndex,pOutCard->cbOutCardData);
				//�����ж�
				WORD wEatUser = (pOutCard->wOutCardUser+GAME_PLAYER-1)%GAME_PLAYER;
				if (wEatUser == wMeChairID)
				{
					cbActionMask|=m_GameLogic.GetEatCardKind(m_cbCardIndex,pOutCard->cbOutCardData);
				}
				//�����ж�
				if (m_bLeftCardCount > 1)
				{
					cbActionMask|=m_GameLogic.GetGangCardKind(m_cbCardIndex,pOutCard->cbOutCardData);
				}
			}
			else
			{
				CString str("weaveCount == %d ", m_bWeaveCount[wMeChairID]);
				InsertSystemString(str);
			}
#else

			//�����ж�
			cbActionMask = m_GameLogic.GetPengCardKind(m_cbCardIndex,pOutCard->cbOutCardData);
			//�����ж�
			WORD wEatUser = (pOutCard->wOutCardUser+GAME_PLAYER-1)%GAME_PLAYER;
			if (wEatUser == wMeChairID)
			{
				cbActionMask|=m_GameLogic.GetEatCardKind(m_cbCardIndex,pOutCard->cbOutCardData);
			}
			//�����ж�
			if (m_bLeftCardCount > 1)
			{
				cbActionMask|=m_GameLogic.GetGangCardKind(m_cbCardIndex,pOutCard->cbOutCardData);
			}

#endif
			//�����ж�
			if (m_bChiHu==true)
			{
				tagHuInput HuInput;
				HuInput.bJudgeHuOnly		= true;
				HuInput.bJudgeKingWait		= false;
				HuInput.bInsertCurrentCard	= true;
				HuInput.cbCurrentCard		= pOutCard->cbOutCardData;
				HuInput.pWeaveItemSeq		= m_WeaveItemArray[wMeChairID];
				HuInput.cbWeaveItemCount	= m_bWeaveCount[wMeChairID];
				HuInput.pCardIndexSeq		= m_cbCardIndex;
				tagHuCourt HuCourt;
				if(m_GameLogic.GetHuInfo(&HuInput, HuCourt))
				{
					cbActionMask |= ACK_CHIHU;
					m_bChiHu = false;
				}

			}
			else
			{
				BYTE cbWeaveCount=m_bWeaveCount[wMeChairID];
				tagHuInput HuInput;
				HuInput.bJudgeHuOnly		= true;
				HuInput.bJudgeKingWait		 = false;
				HuInput.bInsertCurrentCard	= true;
				HuInput.cbCurrentCard		= pOutCard->cbOutCardData;
				HuInput.pWeaveItemSeq		= m_WeaveItemArray[wMeChairID];
				HuInput.cbWeaveItemCount	= m_bWeaveCount[wMeChairID];
				HuInput.pCardIndexSeq		= m_cbCardIndex;
				tagHuCourt HuCourt;
				if(m_GameLogic.GetHuInfo(&HuInput, HuCourt))
				{
					InsertSystemString(TEXT("��Ϊ���������������޷������ƣ�"));
				}
			}

			//���ƽ���
			if (cbActionMask!=ACK_NULL)
			{
				ActiveGameFrame();
				SetGameTimer(wMeChairID,IDI_OPERATE_CARD,10);
				m_GameClientView.m_ControlWnd.SetControlInfo(pOutCard->cbOutCardData,cbActionMask,NULL);
			}
		}

		//������ʾ
		m_GameClientView.SetStatusFlag(false,false);
	}

	//��������
	PlayOutCardSound(pOutCard->wOutCardUser, pOutCard->cbOutCardData);

	//���½���
	m_GameClientView.UpdateGameView(NULL);


	//
	//_DebugAssert();

	return true;
}

//�����˿�
bool CGameClientDlg::OnSubOperateCard(const void * pBuffer, WORD wDataSize)
{
	//Ч����Ϣ
	ASSERT(wDataSize==sizeof(CMD_S_OperateCard));
	if (wDataSize!=sizeof(CMD_S_OperateCard)) 
	{
		return false;
	}

	//��Ϣ����
	CMD_S_OperateCard * pOperateCard=(CMD_S_OperateCard *)pBuffer;

	//��������
	bool bPublicGang=false;
	WORD wOperateUser	= pOperateCard->wOperateUser;
	BYTE cbOperateCard	= pOperateCard->cbOperateCard;
	WORD wOperateViewID = SwitchViewChairID(wOperateUser);
	WORD wProviderViewID = SwitchViewChairID(pOperateCard->wProvideUser);

	//���ñ���
	m_wCurrentUser=pOperateCard->wCurrentUser;

	//�������
	if (pOperateCard->cbOperateCode==ACK_GANG)
	{
		//����˿�
		BYTE cbWeaveIndex=0xFF;
		for (BYTE i=0;i<m_bWeaveCount[wOperateUser];i++)
		{
			BYTE cbWeaveKind=m_WeaveItemArray[wOperateUser][i].cbWeaveKind;
			BYTE cbCenterCard=m_WeaveItemArray[wOperateUser][i].cbCenterCard;
			if ((cbCenterCard==cbOperateCard)&&(cbWeaveKind==ACK_PENG))
			{
				cbWeaveIndex=i;
				bPublicGang=true;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=ACK_GANG;
				break;
			}
		}

		//����˿�
		if (cbWeaveIndex==0xFF)
		{
			cbWeaveIndex=m_bWeaveCount[wOperateUser]++;
			bPublicGang=(pOperateCard->wProvideUser!=wOperateUser);
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=ACK_GANG;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterCard=cbOperateCard;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateCard->wProvideUser;
		}

		//��Ͻ���
		BYTE cbWeaveCard[4]={0,0,0,0},cbWeaveKind=pOperateCard->cbOperateCode;
		BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCardData(cbWeaveKind,cbOperateCard,cbWeaveCard);
		bool bShowWeaveItem=bPublicGang;//(bPublicGang==true)||((IsLookonMode()==false)&&(GetMeChairID()==wOperateUser));
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetDisplayItem(bShowWeaveItem);
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetCardData(cbWeaveCard,cbWeaveCardCount);

		//�˿�����
		if (GetMeChairID()==wOperateUser)
		{
			m_cbCardIndex[m_GameLogic.SwitchToCardIndex(pOperateCard->cbOperateCard)]=0;
			//m_cbCardIndex[m_GameLogic.SwitchToCardIndex(pOperateCard->cbCurrentCard)]++;
		}

		//�۳��˿�
		DeductionTableCard(false);
	}
	else if (pOperateCard->cbOperateCode!=ACK_NULL)
	{
		//�������
		BYTE cbWeaveIndex=m_bWeaveCount[wOperateUser]++;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterCard=cbOperateCard;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateCard->cbOperateCode;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateCard->wProvideUser;

		//��Ͻ���
		BYTE cbWeaveCard[4]={0,0,0,0},cbWeaveKind=pOperateCard->cbOperateCode;
		BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCardData(cbWeaveKind,cbOperateCard,cbWeaveCard);
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetCardData(cbWeaveCard,cbWeaveCardCount);
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetDirectionCardPos(3-(wOperateViewID-wProviderViewID+4)%4);

		//ɾ���˿�
		if (GetMeChairID()==wOperateUser)
		{
			m_GameLogic.RemoveCard(cbWeaveCard,cbWeaveCardCount,&cbOperateCard,1);
			m_GameLogic.RemoveCard(m_cbCardIndex,cbWeaveCard,cbWeaveCardCount-1);
		}
	}

	//�û��˿�
	if (pOperateCard->cbOperateCode!=ACK_NULL)
	{
		if (GetMeChairID()==wOperateUser)
		{
			BYTE cbCardData[MAX_COUNT];
			if (pOperateCard->cbCurrentCard!=0)
			{
				BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData,CountArray(cbCardData));
				m_cbCardIndex[m_GameLogic.SwitchToCardIndex(pOperateCard->cbCurrentCard)]++;
				//m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount,pOperateCard->cbCurrentCard);
				SetHandCardData(cbCardData,cbCardCount,pOperateCard->cbCurrentCard);
				
			}
			else
			{
				BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData,CountArray(cbCardData));
				//m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount-1,cbCardData[cbCardCount-1]);
				SetHandCardData(cbCardData,cbCardCount-1,cbCardData[cbCardCount-1]);
			}
		}
		else
		{
			WORD wUserIndex=(wOperateViewID>=3)?2:wOperateViewID;
			BYTE cbCardCount=MAX_COUNT-m_bWeaveCount[wOperateUser]*3;
			m_GameClientView.m_UserCard[wUserIndex].SetCardData(cbCardCount-1,true);
		}
	}

	//���ƽ���
	if ((IsLookonMode()==false)&&(GetMeChairID()==m_wCurrentUser))
	{
		//��������
		BYTE cbGangCard[4]={0,0,0,0};
		BYTE cbItemCount=m_bWeaveCount[m_wCurrentUser];
		tagWeaveItem * pWeaveItem=m_WeaveItemArray[m_wCurrentUser];

		//�����ж�
		bool bHuCard=false;
		if (pOperateCard->cbCurrentCard!=0) 
		{
			tagHuInput HuInput;
			HuInput.bJudgeHuOnly		= true;
			HuInput.bJudgeKingWait		= false;
			HuInput.bInsertCurrentCard	= false;
			HuInput.cbCurrentCard		= 0;
			HuInput.pWeaveItemSeq		= m_WeaveItemArray[m_wCurrentUser];
			HuInput.cbWeaveItemCount	= m_bWeaveCount[m_wCurrentUser];
			HuInput.pCardIndexSeq		= m_cbCardIndex;
			tagHuCourt HuCourt;
			bHuCard = m_GameLogic.GetHuInfo(&HuInput, HuCourt);

		}
		if (m_bLeftCardCount>1) 
		{
			m_GameLogic.GetGangCardInfo(m_cbCardIndex,pWeaveItem,cbItemCount,cbGangCard);
		}
		
		//���ƽ���
		if ((cbGangCard[0]!=0)||(bHuCard==true))
		{
			BYTE cbActionMask=(bHuCard==true)?ACK_CHIHU:ACK_NULL;
			m_GameClientView.m_ControlWnd.SetControlInfo(0,cbActionMask,cbGangCard);
		}
		else 
		{
			m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
		}
	}
	else 
	{
		m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	}

	//���ý���
	m_GameClientView.SetUserAction(wOperateViewID,pOperateCard->cbOperateCode);
	m_GameClientView.SetStatusFlag((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()),false);

	//��������
	switch(pOperateCard->cbOperateCode) 
	{
		case ACK_GANG:
			PlayChairSound(pOperateCard->wOperateUser, _T("COMMAND_KONG"));
			break;
		case ACK_PENG:
			PlayChairSound(pOperateCard->wOperateUser, _T("COMMAND_PONG"));
			break;
		case ACK_LEFT:
		case ACK_CENTER:
		case ACK_RIGHT:
			PlayChairSound(pOperateCard->wOperateUser, _T("COMMAND_CHOW"));
			break;
	}

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	//��������
//	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	SetGameTimer(m_wCurrentUser,IDI_OUT_CARD,20);

	//
	//_DebugAssert();

	return true;
}

//�����˿�
bool CGameClientDlg::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//Ч����Ϣ
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//��Ϣ����
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	//���ñ���
	WORD wMeChairID=GetMeChairID();
	m_wCurrentUser=pSendCard->wCurrentUser;

	//ȡ�ƽ���
	WORD wViewChairID=SwitchViewChairID(m_wCurrentUser);
	if (wViewChairID!=2)
	{
		WORD wUserIndex=(wViewChairID>2)?2:wViewChairID;
		m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(true);
	}
	else
	{
		m_cbCardIndex[m_GameLogic.SwitchToCardIndex(pSendCard->cbCardData)]++;
		//m_GameClientView.m_HandCardControl.SetCurrentCard(pSendCard->cbCardData);
		SetHandCurrentCard(pSendCard->cbCardData);
	}

	//�۳��˿�
	DeductionTableCard(true);

	//�����˿�
	ASSERT(m_bOutCardCount>0);
	WORD wOutViewChairID=SwitchViewChairID(m_wOutCardUser[m_bOutCardCount-1]);
	m_GameClientView.m_DiscardCard[wOutViewChairID].AddCardItem(m_cbOutCardData[m_bOutCardCount-1]);

	//��ǰ�û�
	if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID))
	{
		//��������
		BYTE cbGangCard[4]={0,0,0,0};
		BYTE cbItemCount=m_bWeaveCount[m_wCurrentUser];
		tagWeaveItem * pWeaveItem=m_WeaveItemArray[m_wCurrentUser];

		//�����ж�
		tagHuInput HuInput;
		HuInput.bJudgeHuOnly		= true;
		HuInput.bJudgeKingWait		= false;
		HuInput.bInsertCurrentCard	= false;
		HuInput.cbCurrentCard		= 0;
		HuInput.pWeaveItemSeq		= m_WeaveItemArray[m_wCurrentUser];
		HuInput.cbWeaveItemCount	= m_bWeaveCount[m_wCurrentUser];
		HuInput.pCardIndexSeq		= m_cbCardIndex;
		tagHuCourt HuCourt;
		bool bHuCard = m_GameLogic.GetHuInfo(&HuInput, HuCourt);

		if (m_bLeftCardCount>1) m_GameLogic.GetGangCardInfo(m_cbCardIndex,pWeaveItem,cbItemCount,cbGangCard);

		//���ƽ���
		if ((cbGangCard[0]!=0)||(bHuCard==true)) 
		{
			BYTE cbActionMask=(bHuCard==true)?ACK_CHIHU:ACK_NULL;
			m_GameClientView.m_ControlWnd.SetControlInfo(0,cbActionMask,cbGangCard);
		}

		//������
		ActiveGameFrame();

	}

	//������ʾ
	m_GameClientView.SetStatusFlag((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()),false);

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	//ʱ������
	//PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	SetGameTimer(m_wCurrentUser,IDI_OUT_CARD,20);

	//
	//_DebugAssert();
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

	//ɾ����ʱ��
	KillGameTimer(IDI_OUT_CARD);
	KillGameTimer(IDI_OPERATE_CARD);

	//���ÿؼ�
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_HandCardControl.SetPositively(false);

	//ready����
	m_bSelfReady = (pGameEnd->cbSelfReady == 0 ? false : true);

	//����ʣ��Ƹ�
#ifdef MJ_DESPERATE
	//m_lScoreLeft = pGameEnd->lLeftScore[GetMeChairID()];
	CopyMemory(m_lScoreLeft, pGameEnd->lLeftScore, sizeof(m_lScoreLeft));
#endif
    
	//��������
	tagGameScoreInfo GameScoreInfo;
	GameScoreInfo.wBankerUser=m_wBankerUser;
	GameScoreInfo.stHuCourt = pGameEnd->stHuCourt;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		const tagUserData * pUserData = GetUserData(i);
		GameScoreInfo.lScore[i]=pGameEnd->lGameScore[i];
		lstrcpyn(GameScoreInfo.szUserName[i],pUserData->szName,CountArray(GameScoreInfo.szUserName[i]));
	}
	m_GameClientView.m_GameScoreWnd.SetScoreInfo(GameScoreInfo);

	//�û��˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=SwitchViewChairID(i);
		if(i == pGameEnd->stHuCourt.wWinChair)
		{
			m_GameClientView.SetUserAction(wViewChairID,ACK_CHIHU);
		}
		m_GameClientView.m_TableCard[wViewChairID].SetCardData(pGameEnd->cbCardData[i],pGameEnd->cbCardCount[i]);
	}

	//�����˿�
	m_GameClientView.m_UserCard[0].SetCardData(0,false);
	m_GameClientView.m_UserCard[1].SetCardData(0,false);
	m_GameClientView.m_UserCard[2].SetCardData(0,false);
	//m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0);
	SetHandCardData(NULL,0,0);

	//��������
	//LONG lScore=pGameEnd->lGameScore[GetMeChairID()];
	/*if (lScore>0L) 
	{
		PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
	}
	else if (lScore<0L) 
	{
		PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
	}
	else 
	{
		PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_END"));
	}*/

	if(pGameEnd->stHuCourt.IsHu())
	{
		if(pGameEnd->stHuCourt.IsZimo())
		{
			PlayChairSound(pGameEnd->stHuCourt.wWinChair,TEXT("GAME_HU_ZIMO"));
		}
		else if(pGameEnd->stHuCourt.wHuAttachedType & EN_HU_ATTACHED_GANG_FLOWER)
		{
			PlayChairSound(pGameEnd->stHuCourt.wWinChair,TEXT("GAME_KONGF"));
		}
		else
		{
			PlayChairSound(pGameEnd->stHuCourt.wWinChair,TEXT("GAME_HU_CHI"));
		}
	}
	else
	{
		PlayChairSound(GetMeChairID(),TEXT("GAME_END"));
	}

	//���ý���
	m_GameClientView.SetStatusFlag(false, false);
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		SetGameTimer(GetMeChairID(),IDI_START_GAME,30);

	}

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	return true;
}

//ϵͳ��Ϣ
bool CGameClientDlg::OnSubSysMsg(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SysMsg));
	if (wDataSize!=sizeof(CMD_S_SysMsg))
	{
		return false;
	}

	//������Ϣ
	CMD_S_SysMsg* pMsg = (CMD_S_SysMsg*)pBuffer;
	pMsg->cbBuf[127] = 0;
	CString strMsg(pMsg->cbBuf);

	return InsertSystemString(strMsg);
}

//���׼��
bool CGameClientDlg::OnSubUserReady(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_UserReady));
	if (wDataSize!=sizeof(CMD_S_UserReady))
	{
		return false;
	}

	//������Ϣ
	CMD_S_UserReady* pMsg = (CMD_S_UserReady*)pBuffer;
	m_bReady[pMsg->wChairID] = true;

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	return true;
}

//ע��ı�
bool CGameClientDlg::OnSubStake(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_Stake))
	{
		ASSERT(0 && "ע����Ϣ���ȳ���");
		return false;
	}

	//������Ϣ
	CMD_S_Stake* pMsg = (CMD_S_Stake*)pBuffer;
	
	CopyMemory(m_lScoreLeft, pMsg->lLeftScore, sizeof(m_lScoreLeft));
	
	if(m_lCellScore != pMsg->lCellScore)
	{
		m_lCellScore = pMsg->lCellScore;
		CString strMsg;
		strMsg.Format(_T("��ǰ��עΪ%d��Ϸ�ҡ�"), m_lCellScore);
		InsertSystemString(strMsg);
	}

	return true;
}

//�۳��˿�
void CGameClientDlg::DeductionTableCard(bool bHeadCard)
{
	if (bHeadCard==true)
	{
		//�л�����
		BYTE cbHeapCount=m_bHeapCardInfo[m_wHeapHand][0]+m_bHeapCardInfo[m_wHeapHand][1];
		if (cbHeapCount==HEAP_FULL_COUNT) m_wHeapHand=(m_wHeapHand+1)%4;//CountArray(m_bHeapCardInfo);

		//�����˿�
		m_bLeftCardCount--;
		m_bHeapCardInfo[m_wHeapHand][0]++;

		if(m_wHeapHand == m_wKingBrotherHeapPos &&
			m_bHeapCardInfo[m_wHeapHand][0] == (HEAP_FULL_COUNT/2-m_wKingBrotherCount+1)*2-1)
		{
			m_bLeftCardCount--;
			m_bHeapCardInfo[m_wHeapHand][0]++;
		}

		//�����˿�
		WORD wHeapViewID=SwitchViewChairID(m_wHeapHand);
		WORD wMinusHeadCount=m_bHeapCardInfo[m_wHeapHand][0];
		WORD wMinusLastCount=m_bHeapCardInfo[m_wHeapHand][1];
		m_GameClientView.m_HeapCard[wHeapViewID].SetCardData(wMinusHeadCount,wMinusLastCount,HEAP_FULL_COUNT);
	}
	else
	{
		//�л�����
		BYTE cbHeapCount=m_bHeapCardInfo[m_wHeapTail][0]+m_bHeapCardInfo[m_wHeapTail][1];
		if (cbHeapCount==HEAP_FULL_COUNT) m_wHeapTail=(m_wHeapTail+3)%4;

		//�����˿�
		m_bLeftCardCount--;
		m_bHeapCardInfo[m_wHeapTail][1]++;

		if(m_wHeapTail == m_wKingBrotherHeapPos &&
			m_bHeapCardInfo[m_wHeapTail][1] == (m_wKingBrotherCount*2-1))
		{
			m_bLeftCardCount--;
			m_bHeapCardInfo[m_wHeapTail][1]++;
		}

		//�����˿�
		WORD wHeapViewID=SwitchViewChairID(m_wHeapTail);
		WORD wMinusHeadCount=m_bHeapCardInfo[m_wHeapTail][0];
		WORD wMinusLastCount=m_bHeapCardInfo[m_wHeapTail][1];
		m_GameClientView.m_HeapCard[wHeapViewID].SetCardData(wMinusHeadCount,wMinusLastCount,HEAP_FULL_COUNT);
	}

	return;
}

//��ʼ��ť
LRESULT CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	//��������
	KillGameTimer(IDI_START_GAME);
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_GameScoreWnd.ShowWindow(SW_HIDE);

	//���ý���
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.SetBankerUser(INVALID_CHAIR);
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);

	//�˿�����
	m_GameClientView.m_UserCard[0].SetCardData(0,false);
	m_GameClientView.m_UserCard[1].SetCardData(0,false);
	m_GameClientView.m_UserCard[2].SetCardData(0,false);
	//m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0);
	SetHandCardData(NULL,0,0);

	//�˿�����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_TableCard[i].SetCardData(NULL,0);
		m_GameClientView.m_DiscardCard[i].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[i][0].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[i][1].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[i][2].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[i][3].SetCardData(NULL,0);
	}

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_bHeapCardInfo[i][0]=0;
		m_bHeapCardInfo[i][1]=0;
		m_GameClientView.m_HeapCard[SwitchViewChairID(i)].SetCardData(m_bHeapCardInfo[i][0],m_bHeapCardInfo[i][1],HEAP_FULL_COUNT);
	}

	//��Ϸ����
	m_wHeapHand=0;
	m_wHeapTail=0;
	m_bLeftCardCount=0;
	m_wCurrentUser=INVALID_CHAIR;

	//������Ϣ
	m_bOutCardCount=0;
	ZeroMemory(m_wOutCardUser,sizeof(m_wOutCardUser));
	ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));

	//����˿�
	ZeroMemory(m_bWeaveCount,sizeof(m_bWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�˿˱���
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bHeapCardInfo,sizeof(m_bHeapCardInfo));

	//������Ϣ
	if(m_bSelfReady)
	{
		SendData(SUB_C_READY, 0, 0);
	}
	//ʹ���Զ����ready�ź�
	else
	{
		SendUserReady(NULL,0);
	}
	
	return 0;
}

//���Ʋ���
LRESULT CGameClientDlg::OnOutCard(WPARAM wParam, LPARAM lParam)
{
	//�����ж�
	if ((IsLookonMode()==true)||(m_wCurrentUser!=GetMeChairID())) return 0;

	//���ñ���
	BYTE cbOutCard=(BYTE)wParam;
	m_wCurrentUser=INVALID_CHAIR;
	m_GameLogic.RemoveCard(m_cbCardIndex,cbOutCard);

	//�����˿�
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData,CountArray(cbCardData));
	//m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount,0);
	SetHandCardData(cbCardData,cbCardCount,0);

	//���ý���
	KillGameTimer(IDI_OUT_CARD);
	m_GameClientView.UpdateGameView(NULL);
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.SetOutCardInfo(2,cbOutCard);
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);

	//��������
	//PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

	//��������
	CMD_C_OutCard OutCard;
	OutCard.cbCardData=cbOutCard;
	SendData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard));

	return 0;
}

//�˿˲���
LRESULT CGameClientDlg::OnCardOperate(WPARAM wParam, LPARAM lParam)
{
	//��������
	BYTE cbOperateCode=(BYTE)(wParam);
	BYTE cbOperateCard=(BYTE)(LOWORD(lParam));

	//״̬�ж�
	if ((m_wCurrentUser==GetMeChairID())&&(cbOperateCode==ACK_NULL))
	{
		m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
		return 0;
	};

	//��������
	m_GameClientView.SetStatusFlag(false,true);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	KillGameTimer((GetMeChairID()==m_wCurrentUser)?IDI_OUT_CARD:IDI_OPERATE_CARD);

	//��������
	CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=cbOperateCode;
	OperateCard.cbOperateCard=cbOperateCard;
	SendData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	return 0;
}

//�йܲ���
LRESULT CGameClientDlg::OnConsignChanged(WPARAM wParam, LPARAM lParam)
{
	SetConsign((wParam == 1 ? true : false));
	return 0;
}

//������Ϣ
void CGameClientDlg::SetHandCardData(BYTE cbCardData[], WORD wCardCount, BYTE cbCurrentCard)
{
	//��������
	if(cbCurrentCard == CARD_KING_DATA_MASK)
	{
//		AfxMessageBox("SetHandCardData");
		cbCurrentCard = cbCardData[wCardCount-1];
		BYTE cbCardIndex[MAX_INDEX];
		CopyMemory(cbCardIndex, m_cbCardIndex, sizeof(cbCardIndex));
		m_GameLogic.RemoveCard(cbCardIndex, cbCurrentCard);
		m_GameLogic.SwitchToCardData(cbCardIndex, cbCardData, (BYTE)wCardCount);	
		ASSERT(m_GameLogic.IsValidCommonCard(cbCurrentCard));
	}
	ASSERT(m_GameLogic.IsValidCommonCard(cbCurrentCard)||cbCurrentCard==0);
	m_GameClientView.m_HandCardControl.SetCardData(cbCardData,wCardCount, cbCurrentCard);

}

//����֮��ǰ��
void CGameClientDlg::SetHandCurrentCard(BYTE cbCurrentCard)
{
	//��������
	if(cbCurrentCard == CARD_KING_DATA_MASK)
	{
//		AfxMessageBox("SetHandCurrentCard");
		BYTE cbCardIndex[MAX_INDEX];
		CopyMemory(cbCardIndex, m_cbCardIndex, sizeof(cbCardIndex));
		for(int i=CARD_KING_INDEX-1; i>=0; i--)
		{
			if(cbCardIndex[i] > 0)
			{
				cbCardIndex[i]--;
				cbCurrentCard = m_GameLogic.SwitchToCardData(i);
				break;
			}
		}
		if(i < 0)	//ֻʣ������
		{
			m_GameClientView.m_HandCardControl.SetCurrentCard(cbCurrentCard);
			return;
		}
		
		BYTE cbCardData[MAX_COUNT];
		BYTE cbCardCount = m_GameLogic.SwitchToCardData(cbCardIndex,cbCardData,CountArray(cbCardData));
		ASSERT(m_GameLogic.IsValidCommonCard(cbCurrentCard));
		m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount, cbCurrentCard);
		return;
	}
	ASSERT(m_GameLogic.IsValidCommonCard(cbCurrentCard)||cbCurrentCard==0);
	m_GameClientView.m_HandCardControl.SetCurrentCard(cbCurrentCard);

}

//�Ƿ�׼��
bool CGameClientDlg::IsUserReady(WORD wChairID)
{
	//��ȡ�û�
	const tagUserData * pUserData = GetUserData(wChairID);
	if(pUserData->cbUserStatus == US_READY || m_bReady[wChairID])
	{
		return true;
	}
	return false;
}

//���ܴ���
BYTE CGameClientDlg::GetIsolatedCard()
{
	//�з���
	for(int i=27; i<27+7; i++)
	{
		if(m_cbCardIndex[i] == 1)
		{
			return m_GameLogic.SwitchToCardData(i);
		}
	}
	//�й��ƴ����
	//��19����
	for(i=0; i<3; i++)
	{
		if(m_cbCardIndex[i*9] == 1)
		{
			if(m_cbCardIndex[i*9+1] == 0
				&& m_cbCardIndex[i*9+2] == 0)
			{
				return m_GameLogic.SwitchToCardData(i*9);
			}
		}
		if(m_cbCardIndex[i*9+8] == 1)
		{
			if(m_cbCardIndex[i*9+7] == 0
				&&  m_cbCardIndex[i*9+6] == 0)
			{
				return m_GameLogic.SwitchToCardData(i*9+8);
			}
		}
	}
	//����ͨ����
	for( i=0; i<27; i++)
	{
		if(m_cbCardIndex[i] == 1)
		{
			BYTE cbTmp = i%9;
			switch(cbTmp)
			{
			case 0:
				if(m_cbCardIndex[i+1] == 0
					&& m_cbCardIndex[i+2] == 0)
				{
					return m_GameLogic.SwitchToCardData(i);
				}
				break;
			case 1:
				if(m_cbCardIndex[i+1] == 0
					&& m_cbCardIndex[i+2] == 0
					&& m_cbCardIndex[i-1] == 0)
				{
					return m_GameLogic.SwitchToCardData(i);
				}
				break;
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
				if(m_cbCardIndex[i+1] == 0
					&& m_cbCardIndex[i+2] == 0
					&& m_cbCardIndex[i-1] == 0
					&& m_cbCardIndex[i-2] == 0)
				{
					return m_GameLogic.SwitchToCardData(i);
				}
				break;
			case 7:
				if(m_cbCardIndex[i-1] == 0
					&& m_cbCardIndex[i-2] == 0
					&& m_cbCardIndex[i+1] == 0)
				{
					return m_GameLogic.SwitchToCardData(i);
				}
				break;
			case 8:
				if(m_cbCardIndex[i-1] == 0
					&& m_cbCardIndex[i-2] == 0)
				{
					return m_GameLogic.SwitchToCardData(i);
				}
				break;
			}
			if(cbTmp < 3)
			{
				i += 5;
			}
			else
			{
				i = (i/9+1)*9 - 1;
			}
		}	
	}

	//����
	//��19����
	for(i=0; i<3; i++)
	{
		if(m_cbCardIndex[i*9] == 1)
		{
			if(m_cbCardIndex[i*9+1] == 0
				|| m_cbCardIndex[i*9+2] == 0)
			{
				return m_GameLogic.SwitchToCardData(i*9);
			}
		}
		if(m_cbCardIndex[i*9+8] == 1)
		{
			if(m_cbCardIndex[i*9+7] == 0
				|| m_cbCardIndex[i*9+6] == 0)
			{
				return m_GameLogic.SwitchToCardData(i*9+8);
			}
		}
	}

	//����ͨ����
	for(i=0; i<27; i++)
	{
		if(m_cbCardIndex[i] == 1)
		{
			if((i%9) == 0)
			{
				i += 3;
				continue;
			}
			if((i+1)%9 == 0)
			{
				continue;
			}
			if(m_cbCardIndex[i+1] == 0
				&& m_cbCardIndex[i-1] == 0)
			{
				return m_GameLogic.SwitchToCardData(i);
			}
			i += 1;
		}	
	}

	return  0XFF;
}

//�����й�
void CGameClientDlg::SetConsign(bool bEnter)
{
	if(bEnter == m_bConsign)
	{
		return;
	}
	//��ʱ�������¼���
	m_cbOverTimeCount	= 0;	
	m_bConsign = bEnter;
	m_GameClientView.SetConsign(m_bConsign);
	if(m_bConsign)
	{
		InsertSystemString(_T("ϵͳ�й��У�������½ǰ�ť��ȡ���йܣ�"));
	}
	else
	{
		InsertSystemString(_T("���Ѿ�ȡ���йܣ�"));
	}
	
}

//���ö�ʱ��
bool CGameClientDlg::SetGameTimer(WORD wChairID, UINT nTimerID, UINT nElapse)
{
	if(nTimerID == IDI_OUT_CARD || nTimerID == IDI_START_GAME)
	{
#ifdef _AUTOPALY
		return CGameFrameDlg::SetGameTimer(wChairID,nTimerID,1);
#else
		if(m_bConsign)
		{
			return CGameFrameDlg::SetGameTimer(wChairID,nTimerID,1);
		}
#endif
	}

	return CGameFrameDlg::SetGameTimer(wChairID, nTimerID, nElapse);
	
}

//���ų�������
void CGameClientDlg::PlayOutCardSound(WORD wChairID, BYTE cbCardData)
{
	if(m_GameLogic.IsValidCommonCard(cbCardData) == false)
	{
		return;
	}
	if(wChairID < 0 || wChairID > 3)
	{
		return;
	}

	//�ж��Ա�
	const tagUserData* pUserData = GetUserData(wChairID);
	if(pUserData == 0)
	{
		assert(0 && "�ò��������Ϣ");
		return;
	}
	bool bBoy = (pUserData->cbGender == GENDER_GIRL ? false : true);

	//BYTE cbCardIndex = m_GameLogic.SwitchToCardIndex(cbCardData);
	BYTE cbType= (cbCardData & MASK_COLOR);
	BYTE cbValue= (cbCardData & MASK_VALUE);
	CString strSoundName;
	switch(cbType)
	{
	case 0X30:	//��
		{
			switch(cbValue) 
			{
			case 1:
				{
					strSoundName = _T("WIND_EAST");
					break;
				}
			case 2:
				{
					strSoundName = _T("WIND_SOUTH");
					break;
				}
			case 3:
				{
					strSoundName = _T("WIND_WEST");
					break;
				}
			case 4:
				{
					strSoundName = _T("WIND_NORTH");
					break;
				}
			case 5:
				{
					strSoundName = _T("DRAGON_RED");
					break;
				}
			case 6:
				{
					strSoundName = _T("DRAGON_GREEN");
					break;
				}
			case 7:
				{
					strSoundName = _T("DRAGON_WHITE");
					break;
				}
			}
			break;
		}		
	case 0X20:	//Ͳ
		{
			strSoundName.Format(_T("CIRCLE_%d"), cbValue);
			break;
		}
		
	case 0X10:	//��
		{
			strSoundName.Format(_T("BAMBOO_%d"), cbValue);
			break;
		}
	case 0X00:	//��
		{
			strSoundName.Format(_T("CHARACTER_%d"), cbValue);
			break;
		}
	}	
	
	if(bBoy)
	{
		strSoundName = _T("BOY1_") + strSoundName;
	}
	else
	{
		strSoundName = _T("GIRL1_") + strSoundName;
	}
	//strSoundName.Format("Gire")
	PlayGameSound(AfxGetInstanceHandle(), strSoundName);
}


//������������
void CGameClientDlg::PlayChairSound(WORD wChairID, LPCSTR lpstrName)
{
	if(lpstrName == 0)
	{
		return;
	}
	//�ж��Ա�
	const tagUserData* pUserData = GetUserData(wChairID);
	if(pUserData == 0)
	{
		assert(0 && "�ò��������Ϣ");
		return;
	}
	bool bBoy = (pUserData->cbGender == GENDER_GIRL ? false : true);

	CString strSoundName = lpstrName;
	if(bBoy)
	{
		strSoundName = _T("BOY1_") + strSoundName;
	}
	else
	{
		strSoundName = _T("GIRL1_") + strSoundName;
	}
	//strSoundName.Format("Gire")
	PlayGameSound(AfxGetInstanceHandle(), strSoundName);
}

void CGameClientDlg::_DebugAssert()
{
	int iSum = 0;
	for(int i=0; i<MAX_INDEX; i++)
	{
		if( m_cbCardIndex[i] > 0)
		{
			iSum = m_cbCardIndex[i];
			BYTE cbCardData = m_GameLogic.SwitchToCardData(i);
			for(int j=0; j<108; j++)
			{
				if(m_cbOutCardData[j] == cbCardData)
				{
					iSum ++;
				}
			}
			if(iSum > 4)
			{
				assert(0 && "��Ȼ�����ˣ�");
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
