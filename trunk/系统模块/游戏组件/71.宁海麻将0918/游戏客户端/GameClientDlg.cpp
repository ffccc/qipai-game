#include "Stdafx.h"
#include "GameClient.h"
#include "GameOption.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ʱ��
#define IDI_START_GAME				200									//��ʼ��ʱ��
#define IDI_OPERATE_CARD			202									//������ʱ��

//��Ϸ��ʱ��
#define TIME_START_GAME				60									//��ʼ��ʱ��
#define TIME_OPERATE_CARD			30									//������ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_CARD_OPERATE,OnCardOperate)
	ON_MESSAGE(IDM_GAME_START,OnGameStart)
	ON_MESSAGE(IDM_TRUSTEE_CONTROL,OnStusteeControl)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//��������
	m_wHeapHand=0;
	m_wHeapTail=0;
	ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

	//�йܱ���
	m_bStustee=false;
	m_wTimeOutCount =0;

	//������Ϣ
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));
	ZeroMemory(m_cbUserWindCount,sizeof(m_cbUserWindCount));
	ZeroMemory(m_cbUserWindData,sizeof(m_cbUserWindData));
	m_bStart=true;
	m_cbUserAction=0;

	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�˿˱���
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_cbCardData,sizeof(m_cbCardData));

	//���ñ���
	m_cbManderin=0;


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
	SetWindowText(TEXT("�����齫  --  Ver��6.0.5.0"));

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//������Դ
	g_CardResource.LoadResource();
	//��ȡ����
	m_cbManderin = AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("Manderin"),2);


	return true;
}

//���ÿ��
void CGameClientDlg::ResetGameFrame()
{
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//��������
	m_wHeapHand=0;
	m_wHeapTail=0;
	ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

	//�йܱ���
	m_bStustee=false;
	m_wTimeOutCount =0;

	//������Ϣ
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));
	ZeroMemory(m_cbUserWindCount,sizeof(m_cbUserWindCount));
	ZeroMemory(m_cbUserWindData,sizeof(m_cbUserWindData));
	m_bStart=true;
	m_cbUserAction=0;

	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�˿˱���
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_cbCardData,sizeof(m_cbCardData));

	return;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{
	//��������
	CGameOption GameOption;
	GameOption.m_bEnableSound=IsEnableSound();
	GameOption.m_cbManderin = m_cbManderin;


	//��������
	if (GameOption.DoModal()==IDOK)
	{
		EnableSound(GameOption.m_bEnableSound);
		m_cbManderin = GameOption.m_cbManderin;
		//��������
		AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("Manderin"),m_cbManderin);

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
				PostMessage(WM_CLOSE,0,0);
				return false;
			}
			if (nElapse<=5) 
			{
					PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			}
			return true;
		}
	case IDI_OPERATE_CARD:		//������ʱ��
		{
			if ((m_wTimeOutCount>=3)&&(m_bStustee==false))	OnStusteeControl(0,0);

			//��ʱ�ж�
			if ((nElapse==0 || m_bStustee==true)&&(IsLookonMode()==false))
			{
				//��ȡλ��
				WORD wMeChairID=GetMeChairID();

				//��������
				if (wChairID==wMeChairID)
				{
					if(m_bStustee==false)
					{
						m_wTimeOutCount++;
						//CString strTemp;
						//strTemp.Format(TEXT("���Ѿ���ʱ%d��"),m_wTimeOutCount);
						//InsertSystemString(LPCTSTR(strTemp));
						if(m_wTimeOutCount==3)
						{
							InsertSystemString(TEXT("��������γ�ʱ���л�Ϊ��ϵͳ�йܡ�ģʽ."));
						}
					}

					if (m_wCurrentUser==wMeChairID)
					{
						BYTE cbCardData=m_GameClientView.m_HandCardControl.GetCurrentCard();
						//����Ч��
						if (VerdictOutCard(cbCardData)==false)
						{
							for (BYTE i=0;i<MAX_INDEX;i++)
							{
								//����Ч��
								if (m_cbCardIndex[i]==0) continue;
								if (VerdictOutCard(m_GameLogic.SwitchToCardData(i))==false) 
									continue;

								//���ñ���
								cbCardData=m_GameLogic.SwitchToCardData(i);
							}
							InsertSystemString(TEXT("��������ʱ��ϵͳ�Ѱ������ƣ�"));
						}
						OnOutCard(cbCardData,cbCardData);
					}
					else
						OnCardOperate(WIK_NULL,0);
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
	//�˿˿���
	m_GameClientView.m_HandCardControl.SetDisplayItem((IsAllowLookon()==true));
	m_GameClientView.UpdateGameView(NULL);
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
	case SUB_S_OUT_CARD:		//�û�����
		{
			return OnSubOutCard(pBuffer,wDataSize);
		}
	case SUB_S_SEND_CARD:		//������Ϣ
		{
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_OPERATE_NOTIFY:	//������ʾ
		{
			return OnSubOperateNotify(pBuffer,wDataSize);
		}
	case SUB_S_OPERATE_RESULT:	//�������
		{
			return OnSubOperateResult(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_FORCE_OUT_CARD:	//ǿ�ȳ���
		{
			return OnSubForceOutCard(pBuffer,wDataSize);
		}
	case SUB_S_FORCE_SEND_CARD:
		{
			return OnSubForceSendCard(pBuffer,wDataSize);
		}
	case SUB_S_TRUSTEE:			//�û��й�
		{
			return OnSubTrustee(pBuffer,wDataSize);
		}
	}

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GS_MJ_FREE:	//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//��������
			m_wBankerUser=pStatusFree->wBankerUser;
			m_GameClientView.SetCellScore(pStatusFree->lCellScore);
			m_GameClientView.m_HandCardControl.SetDisplayItem(true);

			//�й�����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.SetTrustee(SwitchViewChairID(i),pStatusFree->bTrustee[i]);
			}

			//���ý���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_cbHeapCardInfo[i][0]=0;
				m_cbHeapCardInfo[i][1]=HEAP_FULL_COUNT-(((i==m_wBankerUser)||((i+2)%4==m_wBankerUser))?HEAP_FULL_COUNT:HEAP_FULL_COUNT);
				m_GameClientView.m_HeapCard[SwitchViewChairID(i)].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1],HEAP_FULL_COUNT);
			}

			//���ÿؼ�
			if (IsLookonMode()==false)
			{
				m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
				m_GameClientView.m_btStart.SetFocus();
				m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);
				SetGameTimer(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);

			}


			//���½���
			m_GameClientView.UpdateGameView(NULL);

			return true;
		}
	case GS_MJ_PLAY:	//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//���ñ���
			m_wBankerUser=pStatusPlay->wBankerUser;
			m_wCurrentUser=pStatusPlay->wCurrentUser;
			m_cbLeftCardCount=pStatusPlay->cbLeftCardCount;
			m_bStustee=pStatusPlay->bTrustee[GetMeChairID()];

			//�Թ�
			if( IsLookonMode()==true )
				m_GameClientView.m_HandCardControl.SetDisplayItem(IsAllowLookon()||IsAllowUserLookon());

			//�й�����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.SetTrustee(SwitchViewChairID(i),pStatusPlay->bTrustee[i]);
			}

			m_wTimeOutCount=0;
			if(pStatusPlay->bTrustee[GetMeChairID()])
				m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_STOP_TRUSTEE,AfxGetInstanceHandle(),false);
			else
				m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,AfxGetInstanceHandle(),false);


			//��ʷ����
			m_wOutCardUser=pStatusPlay->wOutCardUser;
			m_cbOutCardData=pStatusPlay->cbOutCardData;
			CopyMemory(m_cbDiscardCard,pStatusPlay->cbDiscardCard,sizeof(m_cbDiscardCard));
			CopyMemory(m_cbDiscardCount,pStatusPlay->cbDiscardCount,sizeof(m_cbDiscardCount));
			CopyMemory(m_cbUserWindData,pStatusPlay->cbUserWindCardData,sizeof(m_cbUserWindData));
			CopyMemory(m_cbUserWindCount,pStatusPlay->cbUserWindCount,sizeof(m_cbUserWindCount));

			//�˿˱���
			CopyMemory(m_cbWeaveCount,pStatusPlay->cbWeaveCount,sizeof(m_cbWeaveCount));
			CopyMemory(m_WeaveItemArray,pStatusPlay->WeaveItemArray,sizeof(m_WeaveItemArray));
			m_GameLogic.SwitchToCardIndex(pStatusPlay->cbCardData,pStatusPlay->cbCardCount,m_cbCardIndex);

			//��������
			WORD wViewChairID[GAME_PLAYER]={0,0,0,0};
			for (WORD i=0;i<GAME_PLAYER;i++)
				wViewChairID[i]=SwitchViewChairID(i);

			//��������
			m_GameClientView.SetCellScore(pStatusPlay->lCellScore);
			m_GameClientView.SetBankerUser(wViewChairID[m_wBankerUser]);

			//����˿�
			BYTE cbGangCount=0;
			BYTE cbWeaveCard[4]={0,0,0,0};
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				for (BYTE j=0;j<m_cbWeaveCount[i];j++)
				{
					BYTE cbWeaveKind=m_WeaveItemArray[i][j].cbWeaveKind;
					BYTE cbCenterCard=m_WeaveItemArray[i][j].cbCenterCard;
					BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(cbWeaveKind,cbCenterCard,cbWeaveCard);
					m_GameClientView.m_WeaveCard[wViewChairID[i]][j].SetCardData(cbWeaveCard,cbWeaveCardCount);
					if ((cbWeaveKind&WIK_GANG)&&(m_WeaveItemArray[i][j].wProvideUser==i))
					{
						m_GameClientView.m_WeaveCard[wViewChairID[i]][j].SetDisplayItem(false);
					}
					if(cbWeaveKind&WIK_GANG)
					{
						cbGangCount++;
					}
					WORD wOperateViewID = SwitchViewChairID(i);
					WORD wProviderViewID = SwitchViewChairID(m_WeaveItemArray[i][j].wProvideUser);
					m_GameClientView.m_WeaveCard[wOperateViewID][j].SetDirectionCardPos(3-(wOperateViewID-wProviderViewID+4)%4);

				}
			}

			//�û��˿�
			if (m_wCurrentUser==GetMeChairID())
			{
				BYTE cbCardCount=pStatusPlay->cbCardCount;
				m_GameClientView.m_HandCardControl.SetCardData(pStatusPlay->cbCardData,cbCardCount-1,pStatusPlay->cbCardData[cbCardCount-1]);
			}
			else 
				m_GameClientView.m_HandCardControl.SetCardData(pStatusPlay->cbCardData,pStatusPlay->cbCardCount,0); 

			//�˿�����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//�û��˿�
				if (i!=GetMeChairID())
				{
					BYTE cbCardCount=13-m_cbWeaveCount[i]*3;
					WORD wUserCardIndex=(wViewChairID[i]<2)?wViewChairID[i]:2;
					m_GameClientView.m_UserCard[wUserCardIndex].SetCardData(cbCardCount,(m_wCurrentUser==i));
				}
				//�����˿�
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.m_DiscardCard[wViewChairID].SetCardData(m_cbDiscardCard[i],m_cbDiscardCount[i]);
				m_GameClientView.m_WindCard[wViewChairID].SetCardData(m_cbUserWindData[i],m_cbUserWindCount[i]);

			}


			//��������
			if (IsLookonMode()==false) 
			{
				m_GameClientView.m_HandCardControl.SetPositively(true);
				m_GameClientView.m_HandCardControl.SetDisplayItem(true);
				m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);

			}

			//�����˿�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_cbHeapCardInfo[i][0]=0;
				m_cbHeapCardInfo[i][1]=HEAP_FULL_COUNT-(((i==m_wBankerUser)||((i+2)%4==m_wBankerUser))?HEAP_FULL_COUNT:HEAP_FULL_COUNT);
			}

			//�ַ��˿�
			BYTE cbTakeCount=MAX_REPERTORY-m_cbLeftCardCount+cbGangCount;
			BYTE cbSiceFirst=HIBYTE(pStatusPlay->wSiceCount);
			BYTE cbSiceSecond=LOBYTE(pStatusPlay->wSiceCount);
			BYTE cbMin=min(cbSiceFirst,cbSiceSecond)+1;

			if((cbSiceSecond+cbSiceFirst)%GAME_PLAYER==2 || (cbSiceSecond+cbSiceFirst)%GAME_PLAYER==0 )
				cbSiceFirst +=2;

			WORD wTakeChairID=(m_wBankerUser+cbSiceFirst+cbSiceSecond-1)%GAME_PLAYER;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//������Ŀ
				BYTE cbValidCount=HEAP_FULL_COUNT-m_cbHeapCardInfo[wTakeChairID][1]-((i==0)?(cbMin-1)*2:0);
				BYTE cbRemoveCount=__min(cbValidCount,cbTakeCount);

				//��ȡ�˿�
				cbTakeCount-=cbRemoveCount;
				m_cbHeapCardInfo[wTakeChairID][(i==0)?1:0]+=cbRemoveCount;

				//����ж�
				if (cbTakeCount==0)
				{
					m_wHeapHand=wTakeChairID;
					m_wHeapTail=(m_wBankerUser+cbSiceFirst+cbSiceSecond-1)%GAME_PLAYER;
					break;
				}

				//�л�����
				wTakeChairID=(wTakeChairID+1)%GAME_PLAYER;
			}
			//���Ʋ���
			for (BYTE i=0;i<cbGangCount;i++) DeductionTableCard(true);


			//��������
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.m_HeapCard[wViewChairID[i]].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1],HEAP_FULL_COUNT);
			}

			//��ʷ�˿�
			if (m_wOutCardUser!=INVALID_CHAIR)
			{
				WORD wOutChairID=SwitchViewChairID(m_wOutCardUser);
					m_GameClientView.SetOutCardInfo(wOutChairID,m_cbOutCardData);
			}
            
			//��������
			if ((IsLookonMode()==false)&&(pStatusPlay->cbActionMask!=WIK_NULL))
			{
				//��ȡ����
				BYTE cbActionMask=pStatusPlay->cbActionMask;
				BYTE cbActionCard=pStatusPlay->cbActionCard;

				//��������
				tagGangCardResult GangCardResult;
				ZeroMemory(&GangCardResult,sizeof(GangCardResult));

				//�����ж�
				if ((cbActionMask&WIK_GANG)!=0)
				{
					//�������
					if ((m_wCurrentUser==INVALID_CHAIR)&&(cbActionCard!=0))
					{
						GangCardResult.cbCardCount=1;
						GangCardResult.cbCardData[0]=cbActionCard;
					}

					//�Լ�����
					if ((m_wCurrentUser==GetMeChairID())||(cbActionCard==0))
					{
						WORD wMeChairID=GetMeChairID();
						m_GameLogic.AnalyseGangCard(m_cbCardIndex,m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID],GangCardResult);
					}
				}

				//���ý���
				m_GameClientView.m_ControlWnd.SetControlInfo(cbActionCard,cbActionMask,GangCardResult);
				if (m_wCurrentUser==INVALID_CHAIR) 
					SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
			}

			//����ʱ��
			if (m_wCurrentUser!=INVALID_CHAIR)
			{
				//����ʱ��
				WORD wTimeCount=TIME_OPERATE_CARD;
				SetGameTimer(m_wCurrentUser,TIME_OPERATE_CARD,wTimeCount);
			}
			
			//test
			//m_GameClientView.m_HandCardControl.SetDisplayItem(true);

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
	if (wDataSize!=sizeof(CMD_S_GameStart))	return false;

	//��������
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//����״̬
	SetGameStatus(GS_MJ_PLAY);
	m_GameClientView.SetCurrentUser(INVALID_CHAIR);

	//���ñ���
	m_wBankerUser=pGameStart->wBankerUser;
	m_wCurrentUser=pGameStart->wCurrentUser;
	
	//�׷�����
	if(pGameStart->bFirst)
	{
		m_cbLeftCardCount=MAX_REPERTORY-GAME_PLAYER*(MAX_COUNT-1)-1;
	}
	m_cbSumWindCount = pGameStart->cbSunWindCount;

	//������Ϣ
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	if(pGameStart->bFirst)
	{
		ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
		ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));
	}

	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�����˿�
	BYTE cbCardCount=(GetMeChairID()==m_wBankerUser)?MAX_COUNT:(MAX_COUNT-1);
	m_GameLogic.SwitchToCardIndex(pGameStart->cbCardData,cbCardCount,m_cbCardIndex);

/*	//���Դ���
	if(GetMeChairID() == m_wBankerUser &&pGameStart->cbSunWindCount>0)
	{
		CString str;
		for (int t=MAX_INDEX-8;t<MAX_INDEX;t++)
		{
			if(m_cbCardIndex[t]>0)
			{
				str.Format("%d=%d",t,m_cbCardIndex[t]);
				InsertSystemString((LPCTSTR)str);
			}
		}
	}*/

	//���ý���
	bool bPlayerMode=(IsLookonMode()==false);
	m_GameClientView.m_GameScoreWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_HandCardControl.SetPositively(bPlayerMode);
	m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

	//�Թ۽���
	if (bPlayerMode==false)
	{

		//m_GameClientView.SetHuangZhuang(false);
		m_GameClientView.SetStatusFlag(false,false);
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
		m_GameClientView.m_GameScoreWnd.ShowWindow(SW_HIDE);
	}

	//�׷�����
	if(pGameStart->bFirst)
	{
		//�����˿�
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			m_cbHeapCardInfo[i][0]=0;
			m_cbHeapCardInfo[i][1]=HEAP_FULL_COUNT-(((i==m_wBankerUser)||((i+2)%4==m_wBankerUser))?HEAP_FULL_COUNT:HEAP_FULL_COUNT);
		}

		//�ַ��˿�
		BYTE cbTakeCount=(MAX_COUNT-1)*GAME_PLAYER+1;
		BYTE cbSiceFirst=HIBYTE(pGameStart->wSiceCount);
		BYTE cbSiceSecond=LOBYTE(pGameStart->wSiceCount);
		BYTE cbMin=min(cbSiceFirst,cbSiceSecond)+1;

		if((cbSiceSecond+cbSiceFirst)%GAME_PLAYER==2 || (cbSiceSecond+cbSiceFirst)%GAME_PLAYER==0 )
			cbSiceFirst +=2;

		WORD wTakeChairID=(m_wBankerUser+cbSiceFirst+cbSiceSecond-1)%GAME_PLAYER;
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//������Ŀ
			BYTE cbValidCount=HEAP_FULL_COUNT-m_cbHeapCardInfo[wTakeChairID][1]-((i==0)?(cbMin-1)*2:0);
			BYTE cbRemoveCount=__min(cbValidCount,cbTakeCount);

			//��ȡ�˿�
			cbTakeCount-=cbRemoveCount;
			m_cbHeapCardInfo[wTakeChairID][(i==0)?1:0]+=cbRemoveCount;

			//����ж�
			if (cbTakeCount==0)
			{
				m_wHeapHand=wTakeChairID;                
				m_wHeapTail=(m_wBankerUser+cbSiceFirst+cbSiceSecond-1)%GAME_PLAYER;                
				break;                
			}

			//�л�����
			wTakeChairID=(wTakeChairID+1)%GAME_PLAYER;
		}
	}
	ASSERT(m_cbLeftCardCount == pGameStart->cbLeftCardCount);


	//��������
	CopyMemory(m_cbCardData,pGameStart->cbCardData,sizeof(m_cbCardData));
	m_cbUserAction=pGameStart->cbUserAction;


	BYTE cbSiceFirst=HIBYTE(pGameStart->wSiceCount);
	BYTE cbSiceSecond=LOBYTE(pGameStart->wSiceCount);
	ASSERT(cbSiceFirst<=6 &&cbSiceFirst>=1);
	ASSERT(cbSiceSecond<=6 && cbSiceSecond>=1);

	if(pGameStart->bFirst)
	{
		m_GameClientView.m_DrawSiceWnd.SetSiceInfo(m_GameClientView.GetDC(),250,cbSiceFirst,cbSiceSecond);
		m_GameClientView.m_DrawSiceWnd.ShowWindow(SW_SHOW);
	}

	//�˿�����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wViewChairID=SwitchViewChairID(i);


		//�Թ۽���
		if ((bPlayerMode==false)&&(pGameStart->bFirst))
		{
			m_GameClientView.m_TableCard[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_DiscardCard[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[wViewChairID][0].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[wViewChairID][1].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[wViewChairID][2].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[wViewChairID][3].SetCardData(NULL,0);
			m_GameClientView.m_WindCard[wViewChairID].SetCardData(NULL,0);
		}
	}


	//������
	if (bPlayerMode==true) 
		ActiveGameFrame();
	
	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));
	//�й�����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.SetTrustee(SwitchViewChairID(i),pGameStart->bTrustee[i]);
	}

	if(pGameStart->bFirst==false)	
	{
		//��������
		if ((bPlayerMode==true)&&(m_cbUserAction!=WIK_NULL))
		{
			ShowOperateControl(m_cbUserAction,0);
			SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD*2/3);
		}
		//����ʱ��
		if (m_wCurrentUser!=INVALID_CHAIR)
		{
			m_GameClientView.SetCurrentUser(SwitchViewChairID(m_wCurrentUser));
			SetGameTimer(m_wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
		}
	}
	return true;
}
//��Ϸ��ʼ
LRESULT CGameClientDlg::OnGameStart(WPARAM wParam, LPARAM lParam)
{
	bool bPlayerMode=(IsLookonMode()==false);
	m_bStart=true;
	PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
	m_GameClientView.m_DrawSiceWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_HandCardControl.SetPositively(bPlayerMode);
	m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

	//�˿�����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wViewChairID=SwitchViewChairID(i);

		//��Ͻ���
		m_GameClientView.m_WeaveCard[i][0].SetDisplayItem(true);
		m_GameClientView.m_WeaveCard[i][1].SetDisplayItem(true);
		m_GameClientView.m_WeaveCard[i][2].SetDisplayItem(true);
		m_GameClientView.m_WeaveCard[i][3].SetDisplayItem(true);
		m_GameClientView.m_HeapCard[wViewChairID].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1],HEAP_FULL_COUNT);

		//�û��˿�
		if (wViewChairID!=2)
		{
			WORD wIndex=(wViewChairID>=3)?2:wViewChairID;
			m_GameClientView.m_UserCard[wIndex].SetCardData(CountArray(m_cbCardData)-1,(i==m_wBankerUser));
		}
		else
		{
			BYTE cbBankerCard=(i==m_wBankerUser)?m_cbCardData[MAX_COUNT-1]:0;
			m_GameClientView.m_HandCardControl.SetCardData(m_cbCardData,MAX_COUNT-1,cbBankerCard);
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

	//������ʾ
	if ((bPlayerMode==true)&&(m_wCurrentUser!=INVALID_CHAIR))
	{
		WORD wMeChairID=GetMeChairID();
		if (m_wCurrentUser==wMeChairID) 
			m_GameClientView.SetStatusFlag(true,false);
	}

	SendData(SUB_C_FINISH);
	return 0;
}

//�û�����
bool CGameClientDlg::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
	//Ч����Ϣ
	ASSERT(wDataSize==sizeof(CMD_S_OutCard));
	if (wDataSize!=sizeof(CMD_S_OutCard))	return false;

	//��Ϣ����
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;

	//��������
	WORD wMeChairID=GetMeChairID();
	WORD wOutViewChairID=SwitchViewChairID(pOutCard->wOutCardUser);

	//���ñ���
	m_wCurrentUser=INVALID_CHAIR;
	m_wOutCardUser=pOutCard->wOutCardUser;
	m_cbOutCardData=pOutCard->cbOutCardData;


	//�����û�
	if ((IsLookonMode()==true)||(pOutCard->wOutCardUser!=wMeChairID))
	{
		//��������
		KillGameTimer(IDI_OPERATE_CARD); 
		PlayOutCardSound(wOutViewChairID,pOutCard->cbOutCardData);

		//���ƽ���
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetOutCardInfo(wOutViewChairID,pOutCard->cbOutCardData);

		//�����˿�
		if (wOutViewChairID==2)
		{
			//ɾ���˿�
			BYTE cbCardData[MAX_COUNT];
			m_GameLogic.RemoveCard(m_cbCardIndex,pOutCard->cbOutCardData);

			//�����˿�
			BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
			m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount,0);
		}
		else
		{
			WORD wUserIndex=(wOutViewChairID>2)?2:wOutViewChairID;
			m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(false);
		}
	}

	return true;
}

//������Ϣ
bool CGameClientDlg::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) 	return false;

	//��������
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	//���ñ���
	WORD wMeChairID=GetMeChairID();
	m_wCurrentUser=pSendCard->wCurrentUser;
	
	//���ƴ���
	if (pSendCard->cbCardData!=0)
	{
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
			m_GameClientView.m_HandCardControl.SetCurrentCard(pSendCard->cbCardData);
		}
		
		//�۳��˿�
		DeductionTableCard(pSendCard->bGang);
	}

	//�����˿�
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		//�����˿�
		WORD wOutViewChairID=SwitchViewChairID(m_wOutCardUser);
		m_GameClientView.m_DiscardCard[wOutViewChairID].AddCardItem(m_cbOutCardData);

		//�������
		m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);

		//���ñ���
		m_cbOutCardData=0;
		m_wOutCardUser=INVALID_CHAIR;
	}


	//��ǰ�û�
	if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID))
	{
		//������
		ActiveGameFrame();

		//��������
		if (pSendCard->cbActionMask!=WIK_NULL)
		{
			//��ȡ����
			BYTE cbActionCard=pSendCard->cbCardData;
			BYTE cbActionMask=pSendCard->cbActionMask;

			//��������
			tagGangCardResult GangCardResult;
			ZeroMemory(&GangCardResult,sizeof(GangCardResult));

			//�����ж�
			if ((cbActionMask&WIK_GANG)!=0)
			{
				WORD wMeChairID=GetMeChairID();
				m_GameLogic.AnalyseGangCard(m_cbCardIndex,m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID],GangCardResult);
			}

			//���ý���
			m_GameClientView.m_ControlWnd.SetControlInfo(cbActionCard,cbActionMask,GangCardResult);
		}
	}

	//������ʾ
	m_GameClientView.SetStatusFlag((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID),false);

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	//����ʱ��
	m_GameClientView.SetCurrentUser(SwitchViewChairID(m_wCurrentUser));
	SetGameTimer(m_wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);

	return true;
}

//������ʾ
bool CGameClientDlg::OnSubOperateNotify(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_OperateNotify));
	if (wDataSize!=sizeof(CMD_S_OperateNotify)) return false;

	//��������
	CMD_S_OperateNotify * pOperateNotify=(CMD_S_OperateNotify *)pBuffer;

	//�û�����
	if ((IsLookonMode()==false)&&(pOperateNotify->cbActionMask!=WIK_NULL))
	{
		//��ȡ����
		WORD wMeChairID=GetMeChairID();
		BYTE cbActionMask=pOperateNotify->cbActionMask;
		BYTE cbActionCard=pOperateNotify->cbActionCard;

		//��������
		tagGangCardResult GangCardResult;
		ZeroMemory(&GangCardResult,sizeof(GangCardResult));

		//�����ж�
		if ((cbActionMask&WIK_GANG)!=0)
		{
			//�������
			if ((m_wCurrentUser==INVALID_CHAIR)&&(cbActionCard!=0))
			{
				GangCardResult.cbCardCount=1;
				GangCardResult.cbCardData[0]=cbActionCard;
			}

			//�Լ�����
			if ((m_wCurrentUser==wMeChairID)||(cbActionCard==0))
			{
				WORD wMeChairID=GetMeChairID();
				m_GameLogic.AnalyseGangCard(m_cbCardIndex,m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID],GangCardResult);
			}
		}

		//���ý���
		ActiveGameFrame();
		m_GameClientView.SetCurrentUser(INVALID_CHAIR);
		SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
		m_GameClientView.m_ControlWnd.SetControlInfo(cbActionCard,cbActionMask,GangCardResult);
	}

	return true;
}

//�������
bool CGameClientDlg::OnSubOperateResult(const void * pBuffer, WORD wDataSize)
{
	//Ч����Ϣ
	ASSERT(wDataSize==sizeof(CMD_S_OperateResult));
	if (wDataSize!=sizeof(CMD_S_OperateResult))  		return false;

	//��Ϣ����
	CMD_S_OperateResult * pOperateResult=(CMD_S_OperateResult *)pBuffer;

	//��������
	BYTE cbPublicCard=TRUE;
	WORD wOperateUser=pOperateResult->wOperateUser;
	BYTE cbOperateCard=pOperateResult->cbOperateCard;
	WORD wOperateViewID=SwitchViewChairID(wOperateUser);
	WORD wProviderViewID = SwitchViewChairID(pOperateResult->wProvideUser);

	//���Ʊ���
	if (pOperateResult->cbOperateCode!=WIK_NULL)
	{
		m_cbOutCardData=0;
		m_wOutCardUser=INVALID_CHAIR;
	}
	if((pOperateResult->cbOperateCode&WIK_CHI_HU)!=0)
	{
		//�ж��Ա�
		const tagUserData* pUserData = GetUserData(wOperateViewID);
		if(pUserData == 0)
		{
			assert(0 && "�ò��������Ϣ");
			return true;
		}
		if(wOperateViewID < 0 || wOperateViewID > 3)
		{
			return true;
		}
		bool bBoy = (pUserData->cbGender == 1 ? false : true);
		if(bBoy)
			PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_CHI_HU"));
		else
			PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_CHI_HU"));
			
	}	

	else if ((pOperateResult->cbOperateCode&WIK_GANG)!=0)	//�������
	{
		//���ñ���
		m_wCurrentUser=INVALID_CHAIR;

		//����˿�
		BYTE cbWeaveIndex=0xFF;
		for (BYTE i=0;i<m_cbWeaveCount[wOperateUser];i++)
		{
			BYTE cbWeaveKind=m_WeaveItemArray[wOperateUser][i].cbWeaveKind;
			BYTE cbCenterCard=m_WeaveItemArray[wOperateUser][i].cbCenterCard;
			if ((cbCenterCard==cbOperateCard)&&(cbWeaveKind==WIK_PENG))
			{
				cbWeaveIndex=i;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=TRUE;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;
				break;
			}
		}

		//����˿�
		if (cbWeaveIndex==0xFF)
		{
			//�����ж�
			cbPublicCard=(pOperateResult->wProvideUser==wOperateUser)?FALSE:TRUE;

			//�����˿�
			cbWeaveIndex=m_cbWeaveCount[wOperateUser]++;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=cbPublicCard;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterCard=cbOperateCard;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;
		}

		//��Ͻ���
		BYTE cbWeaveCard[4]={0,0,0,0},cbWeaveKind=pOperateResult->cbOperateCode;
		BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(cbWeaveKind,cbOperateCard,cbWeaveCard);
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetCardData(cbWeaveCard,cbWeaveCardCount);
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetDisplayItem((cbPublicCard==TRUE)?true:false);

		//�˿�����
		if (GetMeChairID()==wOperateUser)
		{
			m_cbCardIndex[m_GameLogic.SwitchToCardIndex(pOperateResult->cbOperateCard)]=0;
		}

		//�����˿�
		if (GetMeChairID()==wOperateUser)
		{
			BYTE cbCardData[MAX_COUNT];
			BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
			m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount,0);
		}
		else
		{
			WORD wUserIndex=(wOperateViewID>=3)?2:wOperateViewID;
			BYTE cbCardCount=MAX_COUNT-m_cbWeaveCount[wOperateUser]*3;
			m_GameClientView.m_UserCard[wUserIndex].SetCardData(cbCardCount-1,false);
		}

	}
	else if (pOperateResult->cbOperateCode!=WIK_NULL)
	{
		//���ñ���
		m_wCurrentUser=pOperateResult->wOperateUser;

		//�������
		BYTE cbWeaveIndex=m_cbWeaveCount[wOperateUser]++;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=TRUE;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterCard=cbOperateCard;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;

		//��Ͻ���
		BYTE cbWeaveCard[4]={0,0,0,0},cbWeaveKind=pOperateResult->cbOperateCode;
		BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(cbWeaveKind,cbOperateCard,cbWeaveCard);
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetCardData(cbWeaveCard,cbWeaveCardCount);
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetDirectionCardPos(3-(wOperateViewID-wProviderViewID+4)%4);



		//ɾ���˿�
		if (GetMeChairID()==wOperateUser)
		{
			m_GameLogic.RemoveCard(cbWeaveCard,cbWeaveCardCount,&cbOperateCard,1);
			m_GameLogic.RemoveCard(m_cbCardIndex,cbWeaveCard,cbWeaveCardCount-1);
		}



		//�����˿�
		if (GetMeChairID()==wOperateUser)
		{
			BYTE cbCardData[MAX_COUNT];
			BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
			m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount-1,cbCardData[cbCardCount-1]);
		}
		else
		{
			WORD wUserIndex=(wOperateViewID>=3)?2:wOperateViewID;
			BYTE cbCardCount=MAX_COUNT-m_cbWeaveCount[wOperateUser]*3;
			m_GameClientView.m_UserCard[wUserIndex].SetCardData(cbCardCount-1,true);
		}
	}

	//���ý���
	m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.SetUserAction(wOperateViewID,pOperateResult->cbOperateCode);
	m_GameClientView.SetStatusFlag((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()),false);

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	//��������
	PlayActionSound(wOperateViewID,pOperateResult->cbOperateCode);

	//����ʱ��
	if (m_wCurrentUser!=INVALID_CHAIR)
	{
		m_GameClientView.SetCurrentUser(SwitchViewChairID(m_wCurrentUser));
		SetGameTimer(m_wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
	}

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


	//����״̬
	SetGameStatus(GS_MJ_FREE);
	m_GameClientView.SetStatusFlag(false,false);

	//ɾ����ʱ��
	KillGameTimer(IDI_OPERATE_CARD);

	//���ÿؼ�
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_HandCardControl.SetPositively(false);



		//��������
		tagScoreInfo ScoreInfo;
		tagWeaveInfo WeaveInfo;
		ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
		ZeroMemory(&WeaveInfo,sizeof(WeaveInfo));

		//�ɼ�����
		ScoreInfo.wBankerUser=m_wBankerUser;
		ScoreInfo.wProvideUser=pGameEnd->wProvideUser;
		ScoreInfo.cbProvideCard=pGameEnd->cbProvideCard;

		//���û���
		CString strTemp ,strEnd = "\n";


		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			const tagUserData * pUserData=GetUserData(i);
			//��������
			ScoreInfo.dwChiHuKind[i]=pGameEnd->wChiHuKind[i];
			ScoreInfo.dwChiHuRight[i]=pGameEnd->wChiHuRight[i];

			//���óɼ�
			ScoreInfo.lGameScore[i]=pGameEnd->lGameScore[i];
			lstrcpyn(ScoreInfo.szUserName[i],pUserData->szName,CountArray(ScoreInfo.szUserName[i]));
			if(pGameEnd->lGameScore[i]>0)
				strTemp.Format("%s: %d �� %d �� %+ld\n",pUserData->szName,pGameEnd->wHuCount[i],pGameEnd->cbHua[i],pGameEnd->lGameScore[i]);
			else
				strTemp.Format("%s: %d �� %d �� %ld\n",pUserData->szName,pGameEnd->wHuCount[i],pGameEnd->cbHua[i],pGameEnd->lGameScore[i]);
			strEnd += strTemp;


			//�����˿�
			if ((ScoreInfo.cbCardCount==0)&&(pGameEnd->wChiHuKind[i]!=CHK_NULL))
			{
				//����˿�
				WeaveInfo.cbWeaveCount=m_cbWeaveCount[i];
				for (BYTE j=0;j<WeaveInfo.cbWeaveCount;j++)
				{
					BYTE cbWeaveKind=m_WeaveItemArray[i][j].cbWeaveKind;
					BYTE cbCenterCard=m_WeaveItemArray[i][j].cbCenterCard;
					WeaveInfo.cbPublicWeave[j]=m_WeaveItemArray[i][j].cbPublicCard;
					WeaveInfo.cbCardCount[j]=m_GameLogic.GetWeaveCard(cbWeaveKind,cbCenterCard,WeaveInfo.cbCardData[j]);
				}

				//�����˿�
				ScoreInfo.cbCardCount=pGameEnd->cbCardCount[i];
				CopyMemory(ScoreInfo.cbCardData,&pGameEnd->cbCardData[i],ScoreInfo.cbCardCount*sizeof(BYTE));

				//��ȡ����
				for (BYTE j=0;j<ScoreInfo.cbCardCount;j++)
				{
					if (ScoreInfo.cbCardData[j]==pGameEnd->cbProvideCard)
					{
						MoveMemory(&ScoreInfo.cbCardData[j],&ScoreInfo.cbCardData[j+1],(ScoreInfo.cbCardCount-j-1)*sizeof(BYTE));
						ScoreInfo.cbCardData[ScoreInfo.cbCardCount-1]=pGameEnd->cbProvideCard;
						break;
					}
				}
			}
		}

		//��Ϣ����
		InsertSystemString((LPCTSTR)strEnd);

		//�ɼ�����
		m_GameClientView.m_GameScoreWnd.SetScoreInfo(ScoreInfo,WeaveInfo);;


	
	
	//��������
	WORD wViewChairID[GAME_PLAYER]={0,0,0,0};
	for (WORD i=0;i<GAME_PLAYER;i++)
		wViewChairID[i]=SwitchViewChairID(i);

	//����˿�
	BYTE cbGangCount=0;
	BYTE cbWeaveCard[4]={0,0,0,0};
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		for (BYTE j=0;j<m_cbWeaveCount[i];j++)
		{
			BYTE cbWeaveKind=m_WeaveItemArray[i][j].cbWeaveKind;
			BYTE cbCenterCard=m_WeaveItemArray[i][j].cbCenterCard;
			BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(cbWeaveKind,cbCenterCard,cbWeaveCard);
			m_GameClientView.m_WeaveCard[wViewChairID[i]][j].SetCardData(cbWeaveCard,cbWeaveCardCount);
			if ((cbWeaveKind&WIK_GANG)&&(m_WeaveItemArray[i][j].wProvideUser==i))
			{
				m_GameClientView.m_WeaveCard[wViewChairID[i]][j].SetDisplayItem(true);
				WORD wOperateViewID = SwitchViewChairID(i);
				WORD wProviderViewID = SwitchViewChairID(m_WeaveItemArray[i][j].wProvideUser);
				m_GameClientView.m_WeaveCard[wOperateViewID][j].SetDirectionCardPos(3-(wOperateViewID-wProviderViewID+4)%4);
			}
		}
	}

	//�û��˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{

		if (pGameEnd->wChiHuKind[i]!=CHK_NULL)
		{	
			m_GameClientView.SetUserAction(SwitchViewChairID(i),WIK_CHI_HU);
		}
		m_GameClientView.m_TableCard[SwitchViewChairID(i)].SetCardData(pGameEnd->cbCardData[i],pGameEnd->cbCardCount[i]);
	}

	//�����˿�
	m_GameClientView.m_UserCard[0].SetCardData(0,false);
	m_GameClientView.m_UserCard[1].SetCardData(0,false);
	m_GameClientView.m_UserCard[2].SetCardData(0,false);
	m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0);

	//���ý���
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		SetGameTimer(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
	}
	m_GameClientView.SetCurrentUser(INVALID_CHAIR);

	//ȡ���й�
	if(m_bStustee)
		OnStusteeControl(0,0);


	//���½���
	m_GameClientView.UpdateGameView(NULL);
	//��������
	LONG lScore=pGameEnd->lGameScore[GetMeChairID()];
	if (lScore>0L)
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
	}
	return true;
}
//�û��й�
bool CGameClientDlg::OnSubTrustee(const void * pBuffer,WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_Trustee));
	if (wDataSize!=sizeof(CMD_S_Trustee)) return false;

	//��Ϣ����
	CMD_S_Trustee * pTrustee=(CMD_S_Trustee *)pBuffer;
	m_GameClientView.SetTrustee(SwitchViewChairID(pTrustee->wChairID),pTrustee->bTrustee);
	if ((IsLookonMode()==true)||(pTrustee->wChairID!=GetMeChairID()))
	{
		const tagUserData * pUserData=GetUserData(pTrustee->wChairID);
		TCHAR szBuffer[256];
		if(pTrustee->bTrustee==true)
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("���[%s]ѡ�����йܹ���."),pUserData->szName);
		else
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("���[%s]ȡ�����йܹ���."),pUserData->szName);
		InsertSystemString(szBuffer);
	}

	return true;
}
//��������
void CGameClientDlg::PlayActionSound(WORD wChairID,BYTE cbAction)
{
	//�ж��Ա�
	const tagUserData* pUserData = GetUserData(wChairID);
	if(pUserData == 0)
	{
		assert(0 && "�ò��������Ϣ");
		return;
	}
	if(wChairID < 0 || wChairID > 3)
	{
		return;
	}
	bool bBoy = (pUserData->cbGender == 1 ? false : true);

	switch (cbAction)
	{
	case WIK_NULL:		//ȡ��
		{
			if(bBoy)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_OUT_CARD"));
			else
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_OUT_CARD"));
			break;
		}
	case WIK_LEFT:
	case WIK_CENTER:
	case WIK_RIGHT:		//����
		{
			if(bBoy)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_CHI"));
			else
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_CHI"));		
			break;
		}
	case WIK_PENG:		//����
		{
			if(bBoy)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_PENG"));
			else
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_PENG"));	
			break;
		}
	case WIK_GANG:		//����
		{
			if(bBoy)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_GANG"));
			else
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_GANG"));		
			break;
		}
	}

	return;
}

//�۳��˿�
void CGameClientDlg::DeductionTableCard(bool bHeadCard)
{
	if (bHeadCard==false)
	{
		//�л�����
		BYTE cbHeapCount=m_cbHeapCardInfo[m_wHeapHand][0]+m_cbHeapCardInfo[m_wHeapHand][1];
		if (cbHeapCount==HEAP_FULL_COUNT) 
			m_wHeapHand=(m_wHeapHand+1)%CountArray(m_cbHeapCardInfo);

		//�����˿�
		m_cbLeftCardCount--;
		m_cbHeapCardInfo[m_wHeapHand][0]++;

		//�����˿�
		WORD wHeapViewID=SwitchViewChairID(m_wHeapHand);
		WORD wMinusHeadCount=m_cbHeapCardInfo[m_wHeapHand][0];
		WORD wMinusLastCount=m_cbHeapCardInfo[m_wHeapHand][1];
		m_GameClientView.m_HeapCard[wHeapViewID].SetCardData(wMinusHeadCount,wMinusLastCount,HEAP_FULL_COUNT);
	}
	else
	{
		//InsertSystemString(TEXT("��β����!"));
		//�л�����
		BYTE cbHeapCount=m_cbHeapCardInfo[m_wHeapTail][0]+m_cbHeapCardInfo[m_wHeapTail][1];
		if (cbHeapCount==HEAP_FULL_COUNT) 
			m_wHeapTail=(m_wHeapTail+3)%CountArray(m_cbHeapCardInfo);

		//�����˿�
		m_cbLeftCardCount--;
		m_cbHeapCardInfo[m_wHeapTail][1]++;
		//CString str;
		//str.Format(TEXT("ʣ������:%d"),m_cbLeftCardCount);
		//InsertSystemString((LPCTSTR)str);

		//�����˿�
		WORD wHeapViewID=SwitchViewChairID(m_wHeapTail);
		WORD wMinusHeadCount=m_cbHeapCardInfo[m_wHeapTail][0];
		WORD wMinusLastCount=m_cbHeapCardInfo[m_wHeapTail][1];
		m_GameClientView.m_HeapCard[wHeapViewID].SetCardData(wMinusHeadCount,wMinusLastCount,HEAP_FULL_COUNT);
	}

	return;
}

//��ʾ����
bool CGameClientDlg::ShowOperateControl(BYTE cbUserAction, BYTE cbActionCard)
{
	//��������
	tagGangCardResult GangCardResult;
	ZeroMemory(&GangCardResult,sizeof(GangCardResult));

	//�����ж�
	if ((cbUserAction&WIK_GANG)!=0)
	{
		//�������
		if (cbActionCard!=0)
		{
			GangCardResult.cbCardCount=1;
			GangCardResult.cbCardData[0]=cbActionCard;
		}

		//�Լ�����
		if (cbActionCard==0)
		{
			WORD wMeChairID=GetMeChairID();
			m_GameLogic.AnalyseGangCard(m_cbCardIndex,m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID],GangCardResult);
		}
	}

	//��ʾ����
	m_GameClientView.m_ControlWnd.SetControlInfo(cbActionCard,cbUserAction,GangCardResult);

	return true;
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
	//m_GameClientView.SetHuangZhuang(false);
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.SetBankerUser(INVALID_CHAIR);
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);

	//�˿�����
	m_GameClientView.m_UserCard[0].SetCardData(0,false);
	m_GameClientView.m_UserCard[1].SetCardData(0,false);
	m_GameClientView.m_UserCard[2].SetCardData(0,false);
	m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0);

	//�˿�����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_TableCard[i].SetCardData(NULL,0);
		m_GameClientView.m_DiscardCard[i].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[i][0].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[i][1].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[i][2].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[i][3].SetCardData(NULL,0);
		m_GameClientView.m_WindCard[i].SetCardData(NULL,0);
	}

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_cbHeapCardInfo[i][0]=0;
		m_cbHeapCardInfo[i][1]=HEAP_FULL_COUNT-(((i==m_wBankerUser)||((i+2)%4==m_wBankerUser))?HEAP_FULL_COUNT:HEAP_FULL_COUNT);
		m_GameClientView.m_HeapCard[SwitchViewChairID(i)].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1],HEAP_FULL_COUNT);
	}

	//��Ϸ����
	m_wCurrentUser=INVALID_CHAIR;

	//������Ϣ
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));


	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�����˿�
	m_wHeapHand=0;
	m_wHeapTail=0;
	ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

	//�˿˱���
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

	//������Ϣ
	SendUserReady(NULL,0);

	return 0;
}

//���Ʋ���
LRESULT CGameClientDlg::OnOutCard(WPARAM wParam, LPARAM lParam)
{
	//�����ж�
	if ((IsLookonMode()==true)||(m_wCurrentUser!=GetMeChairID())) return 0;

	//�����ж�
	if (m_cbSumWindCount>0) 	return 0;
	BYTE cbOutCardData=(BYTE)wParam;
	if(VerdictOutCard(cbOutCardData) == false)
	{
		InsertSystemString(TEXT("�����������ƣ�ϵͳ��ֹ�����Թ�����!"));
		return 0;
	}

	//���ñ���
	m_wCurrentUser=INVALID_CHAIR;

	m_GameLogic.RemoveCard(m_cbCardIndex,cbOutCardData);
	m_cbUserAction =0;

	//�����˿�
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
	m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount,0);

	//���ý���
	KillGameTimer(IDI_OPERATE_CARD);
	m_GameClientView.UpdateGameView(NULL);
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetOutCardInfo(2,cbOutCardData);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);

	//��������
	PlayOutCardSound(GetMeChairID(),cbOutCardData);

	//��������
	CMD_C_OutCard OutCard;
	OutCard.cbCardData=cbOutCardData;
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
	if ((m_wCurrentUser==GetMeChairID())&&(cbOperateCode==WIK_NULL))
	{
		m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
		return 0;
	};

	//��������
	KillGameTimer(IDI_OPERATE_CARD);
	m_GameClientView.SetStatusFlag(false,true);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);

	//��������
	CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=cbOperateCode;
	OperateCard.cbOperateCard=cbOperateCard;
	SendData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	return 0;
}
//ǿ�ȳ���
bool CGameClientDlg::OnSubForceOutCard(const void * pBuffer, WORD wDataSize)
{
	//Ч����Ϣ
	ASSERT(wDataSize==sizeof(CMD_S_OutCard));
	if (wDataSize!=sizeof(CMD_S_OutCard))		return false;

	//��Ϣ����
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;

	//��������
	WORD wMeChairID=GetMeChairID();
	WORD wOutViewChairID=SwitchViewChairID(pOutCard->wOutCardUser);

	//���ñ���
	m_wCurrentUser=INVALID_CHAIR;
	m_wOutCardUser=pOutCard->wOutCardUser;
	m_cbOutCardData=pOutCard->cbOutCardData;

/*	if(GetMeChairID()==m_wBankerUser)
	{
		CString str;
		str.Format("OnSubForceOutCard :�û�%d:��%d",wOutViewChairID,pOutCard->cbOutCardData);
		InsertSystemString(LPCTSTR(str));
	}*/


	//�û��Լ�
	if(pOutCard->wOutCardUser == wMeChairID)
	{
		//�����˿�
		bool bRet=m_GameLogic.RemoveCard(m_cbCardIndex,pOutCard->cbOutCardData);
		
		//���Դ���
		/*if(wMeChairID == m_wBankerUser )
		{
			bool bRet=m_GameLogic.RemoveCard(m_cbCardIndex,pOutCard->cbOutCardData);
			if(bRet == false)
				AfxMessageBox("ׯ�ҳ���!");
		}
		else
		{
			bool bRet=m_GameLogic.RemoveCard(m_cbCardIndex,pOutCard->cbOutCardData);
			if(bRet == false)
				AfxMessageBox("�мҳ���!");
		}*/
		BYTE cbCardData[MAX_COUNT];
		ZeroMemory(cbCardData,sizeof(cbCardData));

		BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData,CountArray(cbCardData));
		if(cbCardCount == MAX_COUNT)
		{
			m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount-1,cbCardData[MAX_COUNT-1]);


		}
		else
			m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount,0);


		//���ý���
		m_GameClientView.UpdateGameView(NULL);
		m_GameClientView.SetStatusFlag(false,false);
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		//m_GameClientView.SetOutCardInfo(2,m_cbOutCardData);
		m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);

		//�������
		m_cbSumWindCount = 0;
	}

	//��������
	PlayOutCardSound(wOutViewChairID,pOutCard->cbOutCardData);


	//�����û�
	if ((IsLookonMode()==true)||(pOutCard->wOutCardUser!=wMeChairID))
	{
		//��������
	//	KillGameTimer(IDI_OPERATE_CARD); 

		//���ƽ���
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		
		WORD wUserIndex=(wOutViewChairID>2)?2:wOutViewChairID;
		m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(false);
	}

	//�����˿�
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		//�����˿�
		WORD wOutViewChairID=SwitchViewChairID(m_wOutCardUser);

		m_GameClientView.m_WindCard[wOutViewChairID].AddCardItem(m_cbOutCardData);

		//���ñ���
		m_cbOutCardData=0;
		m_wOutCardUser=INVALID_CHAIR;
	}
	return true;
}

//ǿ�ȷ���
bool CGameClientDlg::OnSubForceSendCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard))		return false;

	//��������
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	//���ñ���
	WORD wMeChairID=GetMeChairID();
	m_wCurrentUser=pSendCard->wCurrentUser;

	if(m_wCurrentUser == wMeChairID)
	{
		//��׼����
		m_cbSumWindCount = 1;

	}
	//���ƴ���
	if (pSendCard->cbCardData!=0)
	{
		//ȡ�ƽ���
		WORD wViewChairID=SwitchViewChairID(m_wCurrentUser);
		if (wViewChairID!=2)
		{
			WORD wUserIndex=(wViewChairID>2)?2:wViewChairID;
			m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(false);
		}
		else
		{
			m_cbCardIndex[m_GameLogic.SwitchToCardIndex(pSendCard->cbCardData)]++;
			BYTE cbCardData[MAX_COUNT];
			ZeroMemory(cbCardData,sizeof(cbCardData));
			//m_GameClientView.m_HandCardControl.SetCurrentCard(pSendCard->cbCardData);
			BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData,CountArray(cbCardData));
			if(cbCardCount == MAX_COUNT)
			{
				m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount-1,cbCardData[MAX_COUNT-1]);
	
			}
			else
				m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount,0);
	
		}

		//�۳��˿�
		DeductionTableCard(pSendCard->bGang);
	}

	//��ǰ�û�
	if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID))
	{
		//������
		ActiveGameFrame();

		//��������
		if (pSendCard->cbActionMask!=WIK_NULL)
		{
			//��ȡ����
			BYTE cbActionCard=pSendCard->cbCardData;
			BYTE cbActionMask=pSendCard->cbActionMask;

			//��������
			tagGangCardResult GangCardResult;
			ZeroMemory(&GangCardResult,sizeof(GangCardResult));

			//�����ж�
			if ((cbActionMask&WIK_GANG)!=0)
			{
				WORD wMeChairID=GetMeChairID();
				m_GameLogic.AnalyseGangCard(m_cbCardIndex,m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID],GangCardResult);
			}

			//���ý���
			m_GameClientView.m_ControlWnd.SetControlInfo(cbActionCard,cbActionMask,GangCardResult);
		}
	}
	//�����˿�
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		//�����˿�
		WORD wOutViewChairID=SwitchViewChairID(m_wOutCardUser);

		m_GameClientView.m_DiscardCard[wOutViewChairID].AddCardItem(m_cbOutCardData);

		//���ñ���
		m_cbOutCardData=0;
		m_wOutCardUser=INVALID_CHAIR;
	}
	//���½���
	m_GameClientView.UpdateGameView(NULL);
	return true;
}
bool CGameClientDlg::VerdictOutCard(BYTE cbCardData)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
//���ų�������
void CGameClientDlg::PlayOutCardSound(WORD wChairID, BYTE cbCardData)
{
	if(m_GameLogic.IsValidCard(cbCardData) == false)
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
	bool bBoy = (pUserData->cbGender == 2 ? false : true);
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
					strSoundName = _T("F_1");
					break;
				}
			case 2:
				{
					strSoundName = _T("F_2");
					break;
				}
			case 3:
				{
					strSoundName = _T("F_3");
					break;
				}
			case 4:
				{
					strSoundName = _T("F_4");
					break;
				}
			case 5:
				{
					strSoundName = _T("F_5");
					break;
				}
			case 6:
				{
					strSoundName = _T("F_6");
					break;
				}
			case 7:
				{
					strSoundName = _T("F_7");
					break;
				}
			default:
				{
					strSoundName=_T("BU_HUA");
				}

			}
			break;
		}		
	case 0X20:	//Ͳ
		{
			strSoundName.Format(_T("T_%d"), cbValue);
			break;
		}

	case 0X10:	//��
		{
			strSoundName.Format(_T("S_%d"), cbValue);
			break;
		}
	case 0X00:	//��
		{
			strSoundName.Format(_T("W_%d"), cbValue);
			break;
		}
	}	
	if(m_cbManderin<=0 || m_cbManderin>2)
		m_cbManderin=2;
	CString strManderin;
	strManderin.Format("%d_",m_cbManderin);

	if(bBoy)
	{
		strSoundName = _T("BOY_") + strManderin+strSoundName;
	}
	else
	{
		strSoundName = _T("GIRL_") + strManderin+strSoundName;
	}
	PlayGameSound(AfxGetInstanceHandle(), strSoundName);
}

//�Ϲܿ���
LRESULT CGameClientDlg::OnStusteeControl(WPARAM wParam, LPARAM lParam)
{

	//����״̬
	if (m_bStustee==true)
	{
		//���ñ���
		m_wTimeOutCount=0;

		m_bStustee=false;
		m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,AfxGetInstanceHandle(),false);
		InsertSystemString(_T("��ȡ�����йܹ���."));
		CMD_C_Trustee Trustee;
		Trustee.bTrustee = false;
		SendData(SUB_C_TRUSTEE,&Trustee,sizeof(Trustee));

	}
	else
	{
		m_bStustee=true;
		m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_STOP_TRUSTEE,AfxGetInstanceHandle(),false);
		InsertSystemString(_T("��ѡ�����йܹ���."));
		CMD_C_Trustee Trustee;
		Trustee.bTrustee = true;
		SendData(SUB_C_TRUSTEE,&Trustee,sizeof(Trustee));

	}

	return 0;
}

