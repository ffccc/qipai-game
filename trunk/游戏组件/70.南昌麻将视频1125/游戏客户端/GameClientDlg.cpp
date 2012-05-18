#include "Stdafx.h"
#include "GameClient.h"
#include "GameOption.h"
#include "GameClientDlg.h"
#include "cassert"
//////////////////////////////////////////////////////////////////////////

//��Ϸ��ʱ��
#define IDI_START_GAME				200									//��ʼ��ʱ��
#define IDI_OPERATE_CARD			202									//������ʱ��
#define IDI_BATCH					203									//ϴ�趨ʱ��

//��Ϸ��ʱ��
#define TIME_START_GAME				40									//��ʼ��ʱ��
#define TIME_OPERATE_CARD			15									//������ʱ��
#define TIME_BATCH					5									//ϴ�趨ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_CARD_OPERATE,OnCardOperate)
	ON_MESSAGE(IDM_GAME_START,OnGameStart)
	ON_MESSAGE(IDM_GAME_START2,OnGameStart2)
	ON_MESSAGE(IDM_SICE_TWO,OnSiceTwo)
	ON_MESSAGE(IDM_BATCH,OnBatch)
	ON_MESSAGE(IDM_TRUSTEE_CONTROL,OnStusteeControl)
	ON_MESSAGE(IDM_BATCH_END,OnBatchEnd)
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
	m_wKingHeapPos =-1;
	m_wKingPos=-1;
	FillMemory(m_cbKingCardIndex,sizeof(m_cbKingCardIndex),0xFF);

	//������Ϣ
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));
	m_bStart=true;
	m_bStart2=true;
	m_wUserAction=0;

	//���·�
	ZeroMemory(m_lUpDownScore,sizeof(m_lUpDownScore));
	m_cbFloor=0;
	m_cbUpDownKingCardData=0;
	m_wSiceCount3=0;


	//�йܱ���
	m_bStustee=false;
	m_wTimeOutCount =0;

	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�˿˱���
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));


	//�˿˱���
	m_GameClientView.m_HandCardControl.SetKingCardIndex(m_cbKingCardIndex);
	for (BYTE j=0;j<4;j++)
	{
		m_GameClientView.m_TableCard[j].SetKingCardIndex(m_cbKingCardIndex);
	}


}

//��������
CGameClientDlg::~CGameClientDlg()
{
}

//��ʼ����
bool CGameClientDlg::InitGameFrame()
{

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//������Դ
	g_CardResource.LoadResource();
		
	BYTE cbCardData[]={0x01,0x02,0x04,0x12,0x13,0x14,0x28};
	BYTE cbKingCardIndex[]={0,8};
	m_GameLogic.SetKingCardIndex(cbKingCardIndex);
	BYTE cbCardIndex[MAX_INDEX];
	m_GameLogic.SwitchToCardIndex(cbCardData,7,cbCardIndex);
	WORD wChihuRight=0;
	tagChiHuResult tagresult;
	ZeroMemory(&tagresult,sizeof(tagresult));
	tagWeaveItem weaveInfo[4];
	weaveInfo[0].cbCenterCard=0x18;
	weaveInfo[0].wWeaveKind=4;
	weaveInfo[0].wProvideUser=1;
	weaveInfo[0].cbCenterCard=0x15;
	weaveInfo[0].wWeaveKind=8;
	weaveInfo[0].wProvideUser=1;
	m_GameLogic.AnalyseChiHuCard(cbCardIndex,weaveInfo,2,0x28,wChihuRight,tagresult,true);
	

/*	BYTE cbBatchResult[]={1,1,1,1};

	m_GameClientView.SetBatchResult(cbBatchResult);
	m_GameClientView.SetBatchEffect(true);
*/
/*	BYTE cbCardData[]={0x18,0x19,0x17,0x22,0x22,0x13,0x23,0x24,0x01,0x02,0x03,0x04,0x35};
	//m_GameClientView.m_UserCard[0].SetCardData(CountArray(cbCardData)-6,0x01);
	//m_GameClientView.m_UserCard[1].SetCardData(CountArray(cbCardData)-12,0x01);
	//m_GameClientView.m_UserCard[2].SetCardData(CountArray(cbCardData)-12,0x01);
	m_GameClientView.m_HandCardControl.SetDisplayItem(true);

	//m_GameClientView.m_HandCardControl.SetCardData(cbCardData,CountArray(cbCardData)-9,0x01);
	m_GameClientView.m_DiscardCard[0].SetCardData(cbCardData,CountArray(cbCardData));
	m_GameClientView.m_DiscardCard[1].SetCardData(cbCardData,CountArray(cbCardData));
	m_GameClientView.m_DiscardCard[2].SetCardData(cbCardData,CountArray(cbCardData));
	m_GameClientView.m_DiscardCard[3].SetCardData(cbCardData,CountArray(cbCardData));
	m_GameClientView.m_TableCard[0].SetCardData(cbCardData,8);
	m_GameClientView.m_TableCard[1].SetCardData(cbCardData,2);
	m_GameClientView.m_TableCard[2].SetCardData(cbCardData,4);
	m_GameClientView.m_TableCard[3].SetCardData(cbCardData,2);
	m_GameClientView.m_WeaveCard[0][0].SetCardData(cbCardData,3);
	m_GameClientView.m_WeaveCard[0][1].SetCardData(cbCardData,3);
//	m_GameClientView.m_WeaveCard[0][2].SetCardData(cbCardData,3);
	m_GameClientView.m_WeaveCard[1][0].SetCardData(cbCardData,3);
	m_GameClientView.m_WeaveCard[1][1].SetCardData(cbCardData,3);
	m_GameClientView.m_WeaveCard[1][2].SetCardData(cbCardData,3);
	m_GameClientView.m_WeaveCard[1][3].SetCardData(cbCardData,3);
	m_GameClientView.m_WeaveCard[2][2].SetDisplayItem(2);
	m_GameClientView.m_WeaveCard[1][2].SetCardData(cbCardData,4);
	m_GameClientView.m_WeaveCard[2][0].SetCardData(cbCardData,3);
	m_GameClientView.m_WeaveCard[2][1].SetCardData(cbCardData,3);
	m_GameClientView.m_WeaveCard[2][2].SetCardData(cbCardData,4);
	m_GameClientView.m_WeaveCard[3][0].SetCardData(cbCardData,3);
	m_GameClientView.m_WeaveCard[3][1].SetCardData(cbCardData,3);
	m_GameClientView.m_WeaveCard[3][2].SetCardData(cbCardData,3);	
	m_GameClientView.m_WeaveCard[3][3].SetCardData(cbCardData,3);	
	m_wBankerUser=0;
	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_cbHeapCardInfo[i][0]=0;
		m_cbHeapCardInfo[i][1]=0;
		m_GameClientView.m_HeapCard[i].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1],HEAP_FULL_COUNT);
	tagGangCardResult tag;
	ZeroMemory(&tag,sizeof(tag));
	m_GameClientView.m_ControlWnd.SetControlInfo(0x31,WIK_LEFT|WIK_DNBL|WIK_DXBL,tag);
	}

	
	tagScoreInfo ScoreInfo;
	tagWeaveInfo WeaveInfo;
	ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
	ZeroMemory(&WeaveInfo,sizeof(WeaveInfo));

	for (BYTE j=0;j<1;j++)
	{
		WORD wWeaveKind=WIK_PENG;
		BYTE cbCenterCard=0x30;
		WeaveInfo.cbPublicWeave[j]=1;
		WeaveInfo.cbCardCount[j]=m_GameLogic.GetWeaveCard(wWeaveKind,cbCenterCard,WeaveInfo.cbCardData[j]);
	}

	//�ɼ�����
	ScoreInfo.wBankerUser=1;
	ScoreInfo.wProvideUser=2;
	ScoreInfo.cbProvideCard=255;
	WeaveInfo.cbWeaveCount=1;
	WeaveInfo.cbCardCount[0]=3;
	WeaveInfo.cbPublicWeave[0]=WIK_PENG;
	WeaveInfo.cbCardData[0][0]=0x01;
	WeaveInfo.cbCardData[0][1]=0x01;
	WeaveInfo.cbCardData[0][2]=0x01;
	CopyMemory(ScoreInfo.cbCardData,cbCardData,sizeof(BYTE)*5);

	ScoreInfo.cbCardCount=5;

	//���û���
	CString strTemp ,strEnd = "\n";


	for (WORD i=0;i<GAME_PLAYER;i++)
	{


		//���óɼ�
		ScoreInfo.lGameScore[i]=10000L;
		ScoreInfo.lUpDownScore[i]=100L;
		ScoreInfo.lGameHuScore[i]=1000L;
		ScoreInfo.lGameScoreEx[i]=100L;
		lstrcpyn(ScoreInfo.szUserName[i],TEXT("hongming6662"),CountArray(ScoreInfo.szUserName[i]));

	}
	m_GameClientView.m_GameScoreWnd.SetScoreInfo(ScoreInfo,WeaveInfo);
	*/
	
	
	

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
	m_wKingHeapPos =-1;
	m_wKingPos=-1;
	m_wSiceCount3=0;
	FillMemory(m_cbKingCardIndex,sizeof(m_cbKingCardIndex),0xFF);

	//������Ϣ
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));
	m_wUserAction=0;
	m_bStart=true;

	//���·�
	ZeroMemory(m_lUpDownScore,sizeof(m_lUpDownScore));
	m_cbFloor=0;
	m_cbUpDownKingCardData=0;

	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�йܱ���
	m_bStustee=false;
	m_wTimeOutCount =0;

	//�˿˱���
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_HeapCard[i].SetKingCardData(0xFF);
		m_GameClientView.m_HeapCard[i].SetKingPos(WORD(-1));
		m_GameClientView.m_DiscardCard[i].SetKingCardIndex(m_cbKingCardIndex);
		m_GameClientView.m_TableCard[i].SetKingCardIndex(m_cbKingCardIndex);
		for (WORD j=0;j<MAX_WEAVE;j++)
		{
			m_GameClientView.m_WeaveCard[i][j].SetKingCardIndex(m_cbKingCardIndex);
		}
	}
	m_GameClientView.m_HandCardControl.SetKingCardIndex(m_cbKingCardIndex);


	return;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{
	//��������
	CGameOption GameOption;
	GameOption.m_bEnableSound=IsEnableSound();

	//��������
	if (GameOption.DoModal()==IDOK)
	{
		EnableSound(GameOption.m_bEnableSound);
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
				PostMessage(WM_CLOSE);
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
			if (((nElapse==0)&&(IsLookonMode()==false))||(m_bStustee==true))
			{
				//��ȡλ��
				WORD wMeChairID=GetMeChairID();

				//��������
				if (wChairID==wMeChairID)
				{
					if((m_bStustee==false))
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
								if (m_cbCardIndex[2][i]==0) continue;
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
	case IDI_BATCH:   //ϴ�趨ʱ��
		{
			if ((nElapse==0)||(m_bStustee==true))
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID()))
				{
					OnBatch(0,0);
				}
				return true;
			}
			if ((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) 
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			return true;
		}

	}

	return false;
}

//�Թ�״̬
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{

	//�˿˿���
	m_GameClientView.m_HandCardControl.SetDisplayItem(IsAllowLookon()||IsLookonMode()==false);

	//��������
	IClientKernel *pIClientKenel=(IClientKernel *)GetClientKernel(IID_IClientKernel,VER_IClientKernel);
	ASSERT(pIClientKenel!=NULL);
	const tagUserData * pMeUserData=pIClientKenel->GetMeUserInfo();
	const tagServerAttribute * pServerAttribute=pIClientKenel->GetServerAttribute();

	//��������  ����Ա
	if(IsLookonMode()==true &&(pServerAttribute->wGameGenre&GAME_GENRE_MATCH)&&(pMeUserData->cbMasterOrder>0))
	{

		m_GameClientView.m_HandCardControl.SetDisplayItem(true);
		for (WORD i=0;i<GAME_PLAYER;i++)
		{

			//��ͨ�Թ�
			WORD wUserIndex=(SwitchViewChairID(i)>2)?2:SwitchViewChairID(i);
			m_GameClientView.m_UserCard[wUserIndex].SetCardData(0,false);

			//�����˿�
			if(SwitchViewChairID(i)==2) continue;
			BYTE cbCardData[MAX_COUNT];
			BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[SwitchViewChairID(i)],cbCardData,MAX_COUNT);
			m_GameClientView.m_TableCard[SwitchViewChairID(i)].SetCardData(cbCardData,cbCardCount);
		}
	}
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
	case SUB_S_BATCH:     //�û�ϴ��
		{
			return OnSubGameChip(pBuffer,wDataSize);
		}
	case SUB_S_TRUSTEE:			//�û��й�
		{
			return OnSubTrustee(pBuffer,wDataSize);
		}
	case SUB_S_BATCHRESULT:	//ϴ����
		{
			return OnSubBatchResult(pBuffer,wDataSize);
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
			m_GameClientView.m_lLastTurnScore[SwitchViewChairID(GetMeChairID())]=pStatusFree->lLastTurnScore;
			m_GameClientView.m_lAllTurnScore[SwitchViewChairID(GetMeChairID())]=pStatusFree->lAllTurnScore;


			//���ý���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_cbHeapCardInfo[i][0]=0;
				m_cbHeapCardInfo[i][1]=HEAP_FULL_COUNT-(((i==m_wBankerUser)||((i+2)%4==m_wBankerUser))?34:34);
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

			//����Ч��
			m_GameClientView.SetDiscUser(INVALID_CHAIR);
			m_GameClientView.SetTimer(IDI_DISC_EFFECT,250,NULL);
			
			//���½���
			//m_GameClientView.m_btScore.ShowWindow(SW_SHOW);
			m_GameClientView.UpdateGameView(NULL);

			return true;
		}
	case GS_MJ_BATCH:	//����״̬
		{
			if(wDataSize != sizeof(CMD_S_StatusBatch)) return false;
			CMD_S_StatusBatch * pStatusChip =(CMD_S_StatusBatch *)pBuffer;

			//���ñ���
			m_GameClientView.SetCellScore(pStatusChip->lCellScore);
			m_GameClientView.m_HandCardControl.SetDisplayItem(true);
			//m_GameClientView.m_btScore.ShowWindow(SW_SHOW);

			//��������
			IClientKernel *pIClientKenel=(IClientKernel *)GetClientKernel(IID_IClientKernel,VER_IClientKernel);
			ASSERT(pIClientKenel!=NULL);
			const tagUserData * pMeUserData=pIClientKenel->GetMeUserInfo();
			const tagServerAttribute * pServerAttribute=pIClientKenel->GetServerAttribute();

			//��������  ����Ա
			if(IsLookonMode()==true &&(pServerAttribute->wGameGenre&GAME_GENRE_MATCH)&&(pMeUserData->cbMasterOrder>0))
			{
				m_GameClientView.m_HandCardControl.SetDisplayItem(true);

			}

			//�й�����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.SetTrustee(SwitchViewChairID(i),pStatusChip->bTrustee[i]);
			}
			//���ý���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_cbHeapCardInfo[i][0]=0;
				m_cbHeapCardInfo[i][1]=HEAP_FULL_COUNT-(((i==m_wBankerUser)||((i+2)%4==m_wBankerUser))?34:34);
				m_GameClientView.m_HeapCard[SwitchViewChairID(i)].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1],HEAP_FULL_COUNT);
			}

			//���ý���
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewChairID=SwitchViewChairID(i);

				m_GameClientView.m_lAllTurnScore[wViewChairID]=pStatusChip->lAllTurnScore[i];
				m_GameClientView.m_lLastTurnScore[wViewChairID]=pStatusChip->lLastTurnScore[i];

			}

			m_wTimeOutCount=0;

			if(pStatusChip->bTrustee[GetMeChairID()])
				m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_STOP_TRUSTEE,AfxGetInstanceHandle(),false);
			else
				m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,AfxGetInstanceHandle(),false);


			//���ÿؼ�
			if (IsLookonMode()==false && pStatusChip->bComplete==FALSE)
			{
				//��ʾϴ��
				m_GameClientView.m_btBatch.ShowWindow(SW_SHOW);
				m_bStustee = false;
				m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,AfxGetInstanceHandle(),false);
				m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);


				//��ʾ����
				SetGameTimer(GetMeChairID(),IDI_BATCH,TIME_BATCH);
			}
			//����Ч��
			m_GameClientView.SetDiscUser(INVALID_CHAIR);
			m_GameClientView.SetTimer(IDI_DISC_EFFECT,250,NULL);


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
			m_cbLeftCardCount=pStatusPlay->cbLeftCardCount+1; //������Ԥ�ȼ�ȥ���� ���Կͻ���ҪԤ������

			//��ʷ����
			m_wOutCardUser=pStatusPlay->wOutCardUser;
			m_cbOutCardData=pStatusPlay->cbOutCardData;
			CopyMemory(m_cbDiscardCard,pStatusPlay->cbDiscardCard,sizeof(m_cbDiscardCard));
			CopyMemory(m_cbDiscardCount,pStatusPlay->cbDiscardCount,sizeof(m_cbDiscardCount));

			//��ʼ����
			m_bStart=true;
			m_bStart2=true;

			//����Ч��
			if(m_wOutCardUser != INVALID_CHAIR)
				m_GameClientView.SetDiscUser(SwitchViewChairID(m_wOutCardUser));
			m_GameClientView.SetTimer(IDI_DISC_EFFECT,250,NULL);

            //���·�
			m_GameClientView.m_cbUpDownKingCardData=pStatusPlay->cbUpDownKingCardData;
			m_GameClientView.m_cbFloor=pStatusPlay->cbFloor;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.m_lUpDownScore[SwitchViewChairID(i)]=pStatusPlay->lUpDownScore[i];
			}
		
			//���ý���
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.m_lAllTurnScore[wViewChairID]=pStatusPlay->lAllTurnScore[i];
				m_GameClientView.m_lLastTurnScore[wViewChairID]=pStatusPlay->lLastTurnScore[i];

			}
			//�˿˱���
			CopyMemory(m_cbWeaveCount,pStatusPlay->cbWeaveCount,sizeof(m_cbWeaveCount));
			CopyMemory(m_WeaveItemArray,pStatusPlay->WeaveItemArray,sizeof(m_WeaveItemArray));


			m_wTimeOutCount=0;
			if(pStatusPlay->bTrustee[GetMeChairID()])
				m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_STOP_TRUSTEE,AfxGetInstanceHandle(),false);
			else
				m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,AfxGetInstanceHandle(),false);

			
			//���þ���
			CopyMemory(m_cbKingCardIndex,pStatusPlay->cbKingCardIndex,sizeof(m_cbKingCardIndex));
			m_GameLogic.SetKingCardIndex(pStatusPlay->cbKingCardIndex);
			m_GameClientView.m_HandCardControl.SetKingCardIndex(pStatusPlay->cbKingCardIndex);
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.m_HeapCard[i].SetKingCardData(m_GameLogic.SwitchToCardData(pStatusPlay->cbKingCardIndex[0]));
				m_GameClientView.m_DiscardCard[i].SetKingCardIndex(pStatusPlay->cbKingCardIndex);
				m_GameClientView.m_TableCard[i].SetKingCardIndex(pStatusPlay->cbKingCardIndex);
				for (WORD j=0;j<MAX_WEAVE;j++)
				{
					m_GameClientView.m_WeaveCard[i][j].SetKingCardIndex(pStatusPlay->cbKingCardIndex);
				}
			}	
			m_GameClientView.SetKingCardIndex(pStatusPlay->cbKingCardIndex);
			
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameLogic.SwitchToCardIndex(pStatusPlay->cbCardData[i],pStatusPlay->cbCardCount[i],m_cbCardIndex[SwitchViewChairID(i)]);
			}

			//��������
			WORD wViewChairID[GAME_PLAYER]={0,0,0,0};
			for (WORD i=0;i<GAME_PLAYER;i++)
				wViewChairID[i]=SwitchViewChairID(i);

			//��������
			m_GameClientView.SetCellScore(pStatusPlay->lCellScore);
			m_GameClientView.SetBankerUser(wViewChairID[m_wBankerUser]);
			//m_GameClientView.m_btScore.ShowWindow(SW_SHOW);



			//����˿�
			BYTE cbGangCount=0;
			BYTE cbWeaveCard[4]={0,0,0,0};
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				for (BYTE j=0;j<m_cbWeaveCount[i];j++)
				{
					WORD wWeaveKind=m_WeaveItemArray[i][j].wWeaveKind;
					BYTE cbCenterCard=m_WeaveItemArray[i][j].cbCenterCard;
					BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(wWeaveKind,cbCenterCard,cbWeaveCard);
					m_GameClientView.m_WeaveCard[wViewChairID[i]][j].SetCardData(cbWeaveCard,cbWeaveCardCount);
					if ((wWeaveKind&WIK_GANG)&&(m_WeaveItemArray[i][j].wProvideUser==i))
					{
						m_GameClientView.m_WeaveCard[wViewChairID[i]][j].SetDisplayItem(2);
					}
					if(wWeaveKind&WIK_GANG)
					{
						cbGangCount++;
					}
				}
			}

			//�û��˿�
			if (m_wCurrentUser==GetMeChairID())
			{
				//�����˿�
				if (pStatusPlay->cbSendCardData!=0x00)
				{
					//��������
					BYTE cbCardCount=pStatusPlay->cbCardCount[GetMeChairID()];
					BYTE cbRemoveCard[]={pStatusPlay->cbSendCardData};

					//�����˿�
					m_GameLogic.RemoveCard(pStatusPlay->cbCardData[GetMeChairID()],cbCardCount,cbRemoveCard,1);
					pStatusPlay->cbCardData[GetMeChairID()][pStatusPlay->cbCardCount[GetMeChairID()]-1]=pStatusPlay->cbSendCardData;
				}
				//�����˿�
				BYTE cbCardCount=pStatusPlay->cbCardCount[GetMeChairID()];				//m_GameClientView.m_HandCardControl.SetCardData(pStatusPlay->cbCardData,cbCardCount-1,pStatusPlay->cbCardData[cbCardCount-cbCardCount]);
				SetHandCardData(pStatusPlay->cbCardData[GetMeChairID()],cbCardCount-1,pStatusPlay->cbCardData[GetMeChairID()][cbCardCount-1]);

			}
			else 
			{
				//m_GameClientView.m_HandCardControl.SetCardData(pStatusPlay->cbCardData,pStatusPlay->cbCardCount,0); 
				SetHandCardData(pStatusPlay->cbCardData[GetMeChairID()],pStatusPlay->cbCardCount[GetMeChairID()],0); 
			}


			//�˿�����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//�û��˿�
				if (i!=GetMeChairID())
				{
					BYTE cbCardCount=MAX_COUNT-m_cbWeaveCount[i]*3;
					WORD wUserCardIndex=(wViewChairID[i]<2)?wViewChairID[i]:2;
					m_GameClientView.m_UserCard[wUserCardIndex].SetCardData(cbCardCount,(m_wCurrentUser==i));
				}

				//�����˿�
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.m_DiscardCard[wViewChairID].SetCardData(m_cbDiscardCard[i],m_cbDiscardCount[i]);
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
				m_cbHeapCardInfo[i][1]=HEAP_FULL_COUNT-(((i==m_wBankerUser)||((i+2)%4==m_wBankerUser))?34:34);
			}

			//�ַ��˿�
			BYTE cbTakeCount=MAX_REPERTORY-m_cbLeftCardCount+cbGangCount;
			BYTE cbSiceFirst=HIBYTE(pStatusPlay->wSiceCount);
			BYTE cbSiceSecond=LOBYTE(pStatusPlay->wSiceCount);
			BYTE cbSiceUserFirst=HIBYTE(pStatusPlay->wSiceCount2);
			BYTE cbSiceUserSecond=LOBYTE(pStatusPlay->wSiceCount2);
			//����
			BYTE cbsicecount31=HIBYTE(pStatusPlay->wSiceCount3);
			BYTE cbsicecount32=LOBYTE(pStatusPlay->wSiceCount3);
			
			//�ܺ�
			WORD wSiceTotal=cbSiceUserFirst+cbSiceUserSecond;
			WORD wTakeChairID=(m_wBankerUser+cbSiceFirst+cbSiceSecond-1)%GAME_PLAYER;

			//���Ʊ���
			INT wTakeSizeTotal=17-cbsicecount31-cbsicecount32-(17-wSiceTotal);
			m_wKingHeapPos=wTakeChairID;



			if(wTakeSizeTotal<0)
			{
				m_wKingHeapPos=(m_wKingHeapPos-1+GAME_PLAYER)%GAME_PLAYER;
				wTakeSizeTotal=17-abs(wTakeSizeTotal);
			}

			m_wKingPos=wTakeSizeTotal+1;



			//���ƾ���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if(i== m_wKingHeapPos)
				{
					m_GameClientView.m_HeapCard[SwitchViewChairID(m_wKingHeapPos)].SetKingCardData(m_GameLogic.SwitchToCardData(pStatusPlay->cbKingCardIndex[0]));
					m_GameClientView.m_HeapCard[SwitchViewChairID(m_wKingHeapPos)].SetKingPos(m_wKingPos);
				}
				else
				{
					m_GameClientView.m_HeapCard[SwitchViewChairID(i)].SetKingCardData(0XFF);
					m_GameClientView.m_HeapCard[SwitchViewChairID(i)].SetKingPos(WORD(-1));
				}
			}

			if(wSiceTotal*2>HEAP_FULL_COUNT)
			{
				wTakeChairID=(wTakeChairID+1)%GAME_PLAYER;
				wSiceTotal=wSiceTotal%17;
			}

			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//������Ŀ
				BYTE cbValidCount=HEAP_FULL_COUNT-m_cbHeapCardInfo[wTakeChairID][1]-((i==0)?(wSiceTotal)*2:0);
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
			for (BYTE i=0;i<cbGangCount;i++) DeductionTableCard(false);

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
			if (cbGameStation==GS_MJ_PLAY)
			{
				//��������
				if ((IsLookonMode()==false)&&(pStatusPlay->wActionMask!=WIK_NULL))
				{
					//��ȡ����
					WORD wActionMask=pStatusPlay->wActionMask;
					BYTE cbActionCard=pStatusPlay->cbActionCard;

					//��������
					tagGangCardResult GangCardResult;
					ZeroMemory(&GangCardResult,sizeof(GangCardResult));

					//�����ж�
					if ((wActionMask&WIK_GANG)!=0)
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
							m_GameLogic.AnalyseGangCard(m_cbCardIndex[SwitchViewChairID(m_wCurrentUser)],m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID],GangCardResult);
						}
					}

					//���ý���
					m_GameClientView.m_ControlWnd.SetControlInfo(cbActionCard,wActionMask,GangCardResult);
					if (m_wCurrentUser==INVALID_CHAIR) 
						SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD*2/3);
				}

				//����ʱ��
				if (m_wCurrentUser!=INVALID_CHAIR)
				{
					//����ʱ��
					WORD wTimeCount=TIME_OPERATE_CARD;
					SetGameTimer(m_wCurrentUser,TIME_OPERATE_CARD,wTimeCount);
				}
			}

			//��������
			IClientKernel *pIClientKenel=(IClientKernel *)GetClientKernel(IID_IClientKernel,VER_IClientKernel);
			ASSERT(pIClientKenel!=NULL);
			const tagUserData * pMeUserData=pIClientKenel->GetMeUserInfo();
			const tagServerAttribute * pServerAttribute=pIClientKenel->GetServerAttribute();

			//��������  ����Ա
			if(IsLookonMode()==true &&(pServerAttribute->wGameGenre&GAME_GENRE_MATCH)&&(pMeUserData->cbMasterOrder>0))
			{
				m_GameClientView.m_HandCardControl.SetDisplayItem(true);

				for (WORD i=0;i<GAME_PLAYER;i++)
				{
					//��ͨ�Թ�
					WORD wUserIndex=(SwitchViewChairID(i)>2)?2:SwitchViewChairID(i);
					m_GameClientView.m_UserCard[wUserIndex].SetCardData(0,false);

					//�����˿�
					BYTE cbCardCount=(m_wBankerUser==i)?MAX_COUNT:(MAX_COUNT-1);
					if(SwitchViewChairID(i)==2) continue;
					m_GameClientView.m_TableCard[SwitchViewChairID(i)].SetCardData(pStatusPlay->cbCardData[SwitchViewChairID(i)],pStatusPlay->cbCardCount[SwitchViewChairID(i)]);
				}
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
	if (wDataSize!=sizeof(CMD_S_GameStart))	return false;

	//��������
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//����״̬
	SetGameStatus(GS_MJ_PLAY);
	m_bStart=false;

	//�Թ�����
	if(IsLookonMode()==true)
	{


		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			m_GameClientView.m_HeapCard[i].SetCardData(0,0,HEAP_FULL_COUNT);
			m_GameClientView.m_HeapCard[i].SetKingCardData(0xFF);
			m_GameClientView.m_HeapCard[i].SetKingPos(WORD(-1));
			m_GameClientView.m_DiscardCard[i].SetKingCardIndex(m_cbKingCardIndex);
			m_GameClientView.m_TableCard[i].SetKingCardIndex(m_cbKingCardIndex);
			for (WORD j=0;j<MAX_WEAVE;j++)
			{
				m_GameClientView.m_WeaveCard[i][j].SetKingCardIndex(m_cbKingCardIndex);
			}
		}
		m_GameClientView.m_HandCardControl.SetKingCardIndex(m_cbKingCardIndex);

	}

	//���ñ���
	m_wBankerUser=pGameStart->wBankerUser;
	m_wCurrentUser=pGameStart->wCurrentUser;
	m_cbLeftCardCount=MAX_REPERTORY-GAME_PLAYER*(MAX_COUNT-1)-1;
	CopyMemory(m_cbKingCardIndex,pGameStart->cbKingCardIndex,sizeof(m_cbKingCardIndex));

	//���þ���
	CopyMemory(m_cbKingCardIndex,pGameStart->cbKingCardIndex,sizeof(m_cbKingCardIndex));
	m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));
	
	//��������
	if(GetMeChairID()==m_wBankerUser)
		CopyMemory(m_cbCardData,pGameStart->cbCardData[GetMeChairID()],sizeof(BYTE)*14);
	else
		CopyMemory(m_cbCardData,pGameStart->cbCardData[GetMeChairID()],sizeof(BYTE)*13);
	m_wUserAction=pGameStart->wUserAction;

	//������Ϣ
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));


	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//�����˿�
		BYTE cbCardCount=(m_wBankerUser==i)?MAX_COUNT:(MAX_COUNT-1);


		m_GameLogic.SwitchToCardIndex(pGameStart->cbCardData[i],cbCardCount,m_cbCardIndex[SwitchViewChairID(i)]);
	}

	//���ý���
	bool bPlayerMode=(IsLookonMode()==false);
	m_GameClientView.m_GameScoreWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_HandCardControl.SetPositively(bPlayerMode);

	m_GameClientView.SetDiscUser(INVALID_CHAIR);


	//�Թ۽���
	if (bPlayerMode==false)
	{
		m_GameClientView.SetHuangZhuang(false);
		m_GameClientView.SetStatusFlag(false,false);
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
		m_GameClientView.m_GameScoreWnd.ShowWindow(SW_HIDE);
	}

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_cbHeapCardInfo[i][0]=0;
		m_cbHeapCardInfo[i][1]=HEAP_FULL_COUNT-(((i==m_wBankerUser)||((i+2)%4==m_wBankerUser))?34:34);
		//�Թ۽���
		if (bPlayerMode==false)
		{
			m_GameClientView.m_TableCard[i].SetCardData(NULL,0);
			m_GameClientView.m_DiscardCard[i].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[i][0].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[i][1].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[i][2].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[i][3].SetCardData(NULL,0);
		}
	}

	//�ַ��˿�
	BYTE cbTakeCount=(MAX_COUNT-1)*GAME_PLAYER+1;
	BYTE cbSiceFirst=HIBYTE(pGameStart->wSiceCount);
	BYTE cbSiceSecond=LOBYTE(pGameStart->wSiceCount);
	BYTE cbSiceUserFirst=HIBYTE(pGameStart->wSiceCount2);
	BYTE cbSiceUserSecond=LOBYTE(pGameStart->wSiceCount2);

	//CString str;
	//str.Format("%d %d %d",m_wBankerUser,cbSiceFirst,cbSiceSecond);
	//InsertSystemString(str);

	//str.Format("%d %d",cbSiceUserFirst,cbSiceUserSecond);
	//InsertSystemString(str);
	
	//����
	BYTE cbsicecount31=HIBYTE(pGameStart->wSiceCount3);
	BYTE cbsicecount32=LOBYTE(pGameStart->wSiceCount3);

	m_wSiceCount3=pGameStart->wSiceCount3;


	//�ܺ�
	WORD wSiceTotal=cbSiceUserFirst+cbSiceUserSecond;

	WORD wTakeChairID=(m_wBankerUser+cbSiceFirst+cbSiceSecond-1)%GAME_PLAYER;

	m_GameClientView.m_DrawSiceWnd.SetSiceInfo(m_GameClientView.GetDC(),250,cbSiceFirst,cbSiceSecond,cbSiceUserFirst,cbSiceUserSecond);
	m_GameClientView.m_DrawSiceWnd.ShowWindow(SW_SHOW);

	//���Ʊ���
	INT wTakeSizeTotal=17-cbsicecount31-cbsicecount32-(17-wSiceTotal);
	m_wKingHeapPos=wTakeChairID;

	//str.Format("%d %d",cbsicecount31,cbsicecount32);
	//InsertSystemString(str);


	if(wTakeSizeTotal<0)
	{
		m_wKingHeapPos=(m_wKingHeapPos-1+GAME_PLAYER)%GAME_PLAYER;
		wTakeSizeTotal=17-abs(wTakeSizeTotal);
	}

	m_wKingPos=wTakeSizeTotal+1;
	//str.Format("%d %d",m_wKingHeapPos,m_wKingPos);
	//InsertSystemString(str);

	if(wSiceTotal*2>HEAP_FULL_COUNT)
	{
		wTakeChairID=(wTakeChairID+1)%GAME_PLAYER;
		wSiceTotal=wSiceTotal%17;
	}

	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//������Ŀ
		BYTE cbValidCount=HEAP_FULL_COUNT-m_cbHeapCardInfo[wTakeChairID][1]-((i==0)?(wSiceTotal)*2:0);
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


	//���·���Ϣ
	m_cbUpDownKingCardData=pGameStart->cbUpDownKingCardData;
	m_cbFloor=pGameStart->cbFloor;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_lUpDownScore[SwitchViewChairID(i)]=pGameStart->lUpDownScore[i];
	}

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	//������
	if (bPlayerMode==true) 	ActiveGameFrame();


	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));
	return true;
}

//�û�����
bool CGameClientDlg::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
	//Ч����Ϣ
	ASSERT(wDataSize==sizeof(CMD_S_OutCard));
	if (wDataSize!=sizeof(CMD_S_OutCard))	return false;

	//��Ϣ����
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;

	if(!m_bStart)	OnGameStart(0,0);

	if(!m_bStart2) OnGameStart2(0,0);

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
		PlayCardSound(pOutCard->wOutCardUser,pOutCard->cbOutCardData);

		//���ƽ���
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetOutCardInfo(wOutViewChairID,pOutCard->cbOutCardData);

		//�����˿�
		if (wOutViewChairID==2)
		{
			//ɾ���˿�
			BYTE cbCardData[MAX_COUNT];
			m_GameLogic.RemoveCard(m_cbCardIndex[2],pOutCard->cbOutCardData);

			//�����˿�
			BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[2],cbCardData,MAX_COUNT);
			//m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount,0);
			SetHandCardData(cbCardData,cbCardCount,0);

		}
		else
		{
			//��ͨ�Թ�
			WORD wUserIndex=(wOutViewChairID>2)?2:wOutViewChairID;
			m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(false);

			//����Ա�����Թ�
			m_GameLogic.RemoveCard(m_cbCardIndex[wOutViewChairID],pOutCard->cbOutCardData);

			//��������
			IClientKernel *pIClientKenel=(IClientKernel *)GetClientKernel(IID_IClientKernel,VER_IClientKernel);
			ASSERT(pIClientKenel!=NULL);
			const tagUserData * pMeUserData=pIClientKenel->GetMeUserInfo();
			const tagServerAttribute * pServerAttribute=pIClientKenel->GetServerAttribute();

			//��������  ����Ա
			if(IsLookonMode()==true &&(pServerAttribute->wGameGenre&GAME_GENRE_MATCH)&&(pMeUserData->cbMasterOrder>0))
			{
				m_GameClientView.m_HandCardControl.SetDisplayItem(true);
				BYTE cbCardData[MAX_COUNT];
				BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[wOutViewChairID],cbCardData,MAX_COUNT);
				m_GameClientView.m_TableCard[wOutViewChairID].SetCardData(cbCardData,cbCardCount);
			}
		}
	}

	return true;
}

//������Ϣ
bool CGameClientDlg::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) 		return false;

	//��������
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	//���ñ���
	WORD wMeChairID=GetMeChairID();
	m_wCurrentUser=pSendCard->wCurrentUser;

	//�����˿�
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		//�����˿�
		WORD wOutViewChairID=SwitchViewChairID(m_wOutCardUser);
		m_GameClientView.m_DiscardCard[wOutViewChairID].AddCardItem(m_cbOutCardData);
		m_GameClientView.SetDiscUser(wOutViewChairID);

		//���ñ���
		//m_cbOutCardData=0;
		m_wOutCardUser=INVALID_CHAIR;
	}

	//���ƴ���
	if (pSendCard->cbCardData!=0)
	{
		//ȡ�ƽ���
		WORD wViewChairID=SwitchViewChairID(m_wCurrentUser);
		if (wViewChairID!=2)
		{
			WORD wUserIndex=(wViewChairID>2)?2:wViewChairID;
			m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(true);

			//����Ա�����Թ�
			m_cbCardIndex[wViewChairID][m_GameLogic.SwitchToCardIndex(pSendCard->cbCardData)]++;
		
			//��������
			IClientKernel *pIClientKenel=(IClientKernel *)GetClientKernel(IID_IClientKernel,VER_IClientKernel);
			ASSERT(pIClientKenel!=NULL);
			const tagUserData * pMeUserData=pIClientKenel->GetMeUserInfo();
			const tagServerAttribute * pServerAttribute=pIClientKenel->GetServerAttribute();

			//��������  ����Ա
			if(IsLookonMode()==true &&(pServerAttribute->wGameGenre&GAME_GENRE_MATCH)&&(pMeUserData->cbMasterOrder>0))
			{
				m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(false);
				m_GameClientView.m_HandCardControl.SetDisplayItem(true);
				BYTE cbCardData[MAX_COUNT];
				BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[wViewChairID],cbCardData,MAX_COUNT);
				m_GameClientView.m_TableCard[wViewChairID].SetCardData(cbCardData,cbCardCount);
			}
			

		}
		else
		{
			m_cbCardIndex[2][m_GameLogic.SwitchToCardIndex(pSendCard->cbCardData)]++;
			//m_GameClientView.m_HandCardControl.SetCurrentCard(pSendCard->cbCardData);
			SetHandCurrentCard(pSendCard->cbCardData);
		}
		
		//�۳��˿�
		DeductionTableCard(pSendCard->cbIsNotGang);
	}

	//��ǰ�û�
	if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID))
	{
		//������
		ActiveGameFrame();

		//��������
		if (pSendCard->wActionMask!=WIK_NULL)
		{
			//��ȡ����
			BYTE cbActionCard=pSendCard->cbCardData;
			WORD wActionMask=pSendCard->wActionMask;

			//��������
			tagGangCardResult GangCardResult;
			ZeroMemory(&GangCardResult,sizeof(GangCardResult));

			//�����ж�
			if ((wActionMask&WIK_GANG)!=0)
			{
				WORD wMeChairID=GetMeChairID();
				m_GameLogic.AnalyseGangCard(m_cbCardIndex[SwitchViewChairID(m_wCurrentUser)],m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID],GangCardResult);
			}

			//���ý���
			m_GameClientView.m_ControlWnd.SetControlInfo(cbActionCard,wActionMask,GangCardResult);
		}
	}

	//������ʾ
	m_GameClientView.SetStatusFlag((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID),false);

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	//����ʱ��
	SetGameTimer(m_wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);

	return true;
}

//ϴ����
bool CGameClientDlg::OnSubBatchResult(const void *pBuffer,WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize == sizeof(CMD_S_BatchResult));
	if(wDataSize != sizeof(CMD_S_BatchResult)) return false;
	
	//��������
	CMD_S_BatchResult *pBatchResult=(CMD_S_BatchResult *)pBuffer;
	BYTE cbBatchResult[GAME_PLAYER];
	for (BYTE i=0;i<GAME_PLAYER;i++)
	{
		cbBatchResult[SwitchViewChairID(i)]=pBatchResult->cbBatchResult[i];
	}

	//InsertSystemString(TEXT("ϴ�迪ʼ"));
	m_GameClientView.SetBatchResult(cbBatchResult);

	//�Ƿ��ж���
	if(cbBatchResult[0]+cbBatchResult[1]+cbBatchResult[2]+cbBatchResult[3]>0)
		m_GameClientView.SetBatchEffect(true);
	return true;
}

//������ʾ
bool CGameClientDlg::OnSubOperateNotify(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_OperateNotify));
	if (wDataSize!=sizeof(CMD_S_OperateNotify))		return false;

	//��������
	CMD_S_OperateNotify * pOperateNotify=(CMD_S_OperateNotify *)pBuffer;

	//�û�����
	if ((IsLookonMode()==false)&&(pOperateNotify->wActionMask!=WIK_NULL))
	{
		//��ȡ����
		WORD wMeChairID=GetMeChairID();
		WORD wActionMask=pOperateNotify->wActionMask;
		BYTE cbActionCard=pOperateNotify->cbActionCard;

		//��������
		tagGangCardResult GangCardResult;
		ZeroMemory(&GangCardResult,sizeof(GangCardResult));

		//�����ж�
		if ((wActionMask&WIK_GANG)!=0)
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
				m_GameLogic.AnalyseGangCard(m_cbCardIndex[SwitchViewChairID(m_wCurrentUser)],m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID],GangCardResult);
			}
		}

		//���ý���
		ActiveGameFrame();
		SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD*2/3);
		m_GameClientView.m_ControlWnd.SetControlInfo(cbActionCard,wActionMask,GangCardResult);
		//CString str;
		//str.Format("%d",wActionMask);
		//InsertSystemString(str);

	}

	return true;
}

//�������
bool CGameClientDlg::OnSubOperateResult(const void * pBuffer, WORD wDataSize)
{
	//Ч����Ϣ
	ASSERT(wDataSize==sizeof(CMD_S_OperateResult));
	if (wDataSize!=sizeof(CMD_S_OperateResult)) return false;

	//��Ϣ����
	CMD_S_OperateResult * pOperateResult=(CMD_S_OperateResult *)pBuffer;
	if(!m_bStart)
		OnGameStart(0,0);

	//��������
	BYTE cbPublicCard=TRUE;
	WORD wOperateUser=pOperateResult->wOperateUser;
	BYTE cbOperateCard=pOperateResult->cbOperateCard;
	WORD wOperateViewID=SwitchViewChairID(wOperateUser);

	//���Ʊ���
	if (pOperateResult->wOperateCode!=WIK_NULL)
	{
		//m_cbOutCardData=0;
		m_wOutCardUser=INVALID_CHAIR;
	}

	//�������
	if ((pOperateResult->wOperateCode&WIK_GANG)!=0)
	{
		//���ñ���
		m_wCurrentUser=INVALID_CHAIR;

		//����˿�
		BYTE cbWeaveIndex=0xFF;
		for (BYTE i=0;i<m_cbWeaveCount[wOperateUser];i++)
		{
			WORD wWeaveKind=m_WeaveItemArray[wOperateUser][i].wWeaveKind;
			BYTE cbCenterCard=m_WeaveItemArray[wOperateUser][i].cbCenterCard;
			if ((cbCenterCard==cbOperateCard)&&(wWeaveKind==WIK_PENG))
			{
				cbWeaveIndex=i;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=TRUE;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].wWeaveKind=pOperateResult->wOperateCode;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;
				break;
			}
		}

		//����˿�
		if (cbWeaveIndex==0xFF)
		{
			//�����ж�
			cbPublicCard=(pOperateResult->wProvideUser==wOperateUser)?FALSE:TRUE;
			if(cbPublicCard==FALSE &&(wOperateUser==GetMeChairID()))
			{
				cbPublicCard=2;
			}


			//�����˿�
			cbWeaveIndex=m_cbWeaveCount[wOperateUser]++;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=cbPublicCard;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterCard=cbOperateCard;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].wWeaveKind=pOperateResult->wOperateCode;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;
		}

		//��Ͻ���
		BYTE cbWeaveCard[4]={0,0,0,0};
		WORD wWeaveKind=pOperateResult->wOperateCode;
		BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(wWeaveKind,cbOperateCard,cbWeaveCard);
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetCardData(cbWeaveCard,cbWeaveCardCount);
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetDisplayItem(cbPublicCard);

		//�˿�����
		if (GetMeChairID()==wOperateUser)
		{
			m_cbCardIndex[SwitchViewChairID(wOperateUser)][m_GameLogic.SwitchToCardIndex(pOperateResult->cbOperateCard)]=0;
		}
		else //����Ա�Թ�
		{
			m_cbCardIndex[SwitchViewChairID(wOperateUser)][m_GameLogic.SwitchToCardIndex(pOperateResult->cbOperateCard)]=0;
		}

		//�����˿�
		if (GetMeChairID()==wOperateUser)
		{
			BYTE cbCardData[MAX_COUNT];
			BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[SwitchViewChairID(wOperateUser)],cbCardData,MAX_COUNT);
			//m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount,0);
			SetHandCardData(cbCardData,cbCardCount,0);

		}
		else
		{
			WORD wUserIndex=(wOperateViewID>=3)?2:wOperateViewID;
			BYTE cbCardCount=MAX_COUNT-m_cbWeaveCount[wOperateUser]*3;
			m_GameClientView.m_UserCard[wUserIndex].SetCardData(cbCardCount-1,false);
		}
	}
	else if (pOperateResult->wOperateCode!=WIK_NULL)
	{
		//���ñ���
		m_wCurrentUser=pOperateResult->wOperateUser;

		//�������
		BYTE cbWeaveIndex=m_cbWeaveCount[wOperateUser]++;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=TRUE;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterCard=cbOperateCard;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].wWeaveKind=pOperateResult->wOperateCode;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;

		//��Ͻ���
		BYTE cbWeaveCard[4]={0,0,0,0};
		WORD wWeaveKind=pOperateResult->wOperateCode;
		BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(wWeaveKind,cbOperateCard,cbWeaveCard);
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetCardData(cbWeaveCard,cbWeaveCardCount);

		//ɾ���˿�
		if (GetMeChairID()==wOperateUser)
		{
			m_GameLogic.RemoveCard(cbWeaveCard,cbWeaveCardCount,&cbOperateCard,1);
			m_GameLogic.RemoveCard(m_cbCardIndex[SwitchViewChairID(wOperateUser)],cbWeaveCard,cbWeaveCardCount-1);
		}
		else  //����Ա�Թ�
		{
			m_GameLogic.RemoveCard(cbWeaveCard,cbWeaveCardCount,&cbOperateCard,1);
			m_GameLogic.RemoveCard(m_cbCardIndex[SwitchViewChairID(wOperateUser)],cbWeaveCard,cbWeaveCardCount-1);
		}

		//�����˿�
		if (GetMeChairID()==wOperateUser)
		{
			BYTE cbCardData[MAX_COUNT];
			BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[SwitchViewChairID(wOperateUser)],cbCardData,MAX_COUNT);
			//m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount-1,cbCardData[cbCardCount-1]);
			SetHandCardData(cbCardData,cbCardCount-1,cbCardData[cbCardCount-1]);
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
	m_GameClientView.SetUserAction(wOperateViewID,pOperateResult->wOperateCode);
	m_GameClientView.SetStatusFlag((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()),false);

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	//��������
	PlayActionSound(pOperateResult->wOperateUser,pOperateResult->wOperateCode);

	//����ʱ��
	if (m_wCurrentUser!=INVALID_CHAIR)
	{
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
	m_GameClientView.m_DrawSiceWnd.ShowWindow(SW_HIDE);

	//���ý���
	m_GameClientView.m_btBatch.ShowWindow(SW_HIDE);
	
	//�û��˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=SwitchViewChairID(i);
		if (pGameEnd->dwChiHuKind[i]!=CHK_NULL)
			m_GameClientView.SetUserAction(wViewChairID,WIK_CHI_HU);
		m_GameClientView.m_TableCard[wViewChairID].SetCardData(pGameEnd->cbCardData[i],pGameEnd->cbCardCount[i]);
	}
	//ɾ����ʱ��
	KillGameTimer(IDI_OPERATE_CARD);
	KillGameTimer(IDI_BATCH);

	//���ÿؼ�
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_HandCardControl.SetPositively(false);
	//��������
	tagScoreInfo ScoreInfo;
	tagWeaveInfo WeaveInfo;
	ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
	ZeroMemory(&WeaveInfo,sizeof(WeaveInfo));
	CopyMemory(ScoreInfo.lUpDownScore,pGameEnd->lUpDownScore,sizeof(ScoreInfo.lUpDownScore));
	CopyMemory(ScoreInfo.lGameScoreEx,pGameEnd->lGameScoreEx,sizeof(ScoreInfo.lGameScoreEx));
	CopyMemory(ScoreInfo.cbChongGuang,pGameEnd->cbChongguan,sizeof(ScoreInfo.cbChongGuang));
	CopyMemory(ScoreInfo.cbBaWangKing,pGameEnd->cbBaWangKing,sizeof(ScoreInfo.cbBaWangKing));
	//��������
	if (pGameEnd->cbChiHuCard!=0)
	{
			

		//�ɼ�����
		ScoreInfo.wBankerUser=m_wBankerUser;
		ScoreInfo.wProvideUser=pGameEnd->wProvideUser;
		ScoreInfo.cbProvideCard=pGameEnd->cbChiHuCard;

		//���û���
		CString strTemp ,strEnd = "\n";


		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			const tagUserData * pUserData=GetUserData(i);
			//��������
			ScoreInfo.dwChiHuKind[i]=pGameEnd->dwChiHuKind[i];
			ScoreInfo.dwChiHuRight[i]=pGameEnd->dwChiHuRight[i];

			//���óɼ�
			ScoreInfo.lGameScore[i]=pGameEnd->lGameScore[i];
			ScoreInfo.lGameHuScore[i]=pGameEnd->lGameHuScore[i];
			lstrcpyn(ScoreInfo.szUserName[i],pUserData->szName,CountArray(ScoreInfo.szUserName[i]));
			if(pGameEnd->lGameScore[i]>0)
				strTemp.Format("%s: %+ld\n",pUserData->szName,pGameEnd->lGameScore[i]);
			else
				strTemp.Format("%s: %ld\n",pUserData->szName,pGameEnd->lGameScore[i]);
			strEnd += strTemp;

			//�����˿�
			if ((ScoreInfo.cbCardCount==0)&&(pGameEnd->dwChiHuKind[i]!=CHK_NULL))
			{
				PlayHuSound(SwitchViewChairID(i),pGameEnd->dwChiHuKind[i],pGameEnd->dwChiHuRight[i]);
				//����˿�
				WeaveInfo.cbWeaveCount=m_cbWeaveCount[i];
				for (BYTE j=0;j<WeaveInfo.cbWeaveCount;j++)
				{
					WORD wWeaveKind=m_WeaveItemArray[i][j].wWeaveKind;
					BYTE cbCenterCard=m_WeaveItemArray[i][j].cbCenterCard;
					WeaveInfo.cbPublicWeave[j]=m_WeaveItemArray[i][j].cbPublicCard;
					WeaveInfo.cbCardCount[j]=m_GameLogic.GetWeaveCard(wWeaveKind,cbCenterCard,WeaveInfo.cbCardData[j]);
				}

				//�����˿�
				ScoreInfo.cbCardCount=pGameEnd->cbCardCount[i];
				CopyMemory(ScoreInfo.cbCardData,&pGameEnd->cbCardData[i],ScoreInfo.cbCardCount*sizeof(BYTE));

				//��ȡ����
				for (BYTE j=0;j<ScoreInfo.cbCardCount;j++)
				{
					if (ScoreInfo.cbCardData[j]==pGameEnd->cbChiHuCard)
					{
						MoveMemory(&ScoreInfo.cbCardData[j],&ScoreInfo.cbCardData[j+1],(ScoreInfo.cbCardCount-j-1)*sizeof(BYTE));
						ScoreInfo.cbCardData[ScoreInfo.cbCardCount-1]=pGameEnd->cbChiHuCard;
						break;
					}
				}
			}
		}

		//��Ϣ����
		InsertSystemString((LPCTSTR)strEnd);
		m_GameClientView.m_GameScoreWnd.SetScoreInfo(ScoreInfo,WeaveInfo);
	}
	else 
	{
		//�ɼ�����
		ScoreInfo.wBankerUser=m_wBankerUser;
		ScoreInfo.wProvideUser=pGameEnd->wProvideUser;
		ScoreInfo.cbProvideCard=pGameEnd->cbChiHuCard;

		//���û���
		CString strTemp ,strEnd = "\n";


		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			const tagUserData * pUserData=GetUserData(i);
			//��������
			ScoreInfo.dwChiHuKind[i]=pGameEnd->dwChiHuKind[i];
			ScoreInfo.dwChiHuRight[i]=pGameEnd->dwChiHuRight[i];

			//���óɼ�
			ScoreInfo.lGameScore[i]=pGameEnd->lGameScore[i];
			lstrcpyn(ScoreInfo.szUserName[i],pUserData->szName,CountArray(ScoreInfo.szUserName[i]));
			if(pGameEnd->lGameScore[i]>0)
				strTemp.Format("%s: %+ld\n",pUserData->szName,pGameEnd->lGameScore[i]);
			else
				strTemp.Format("%s: %ld\n",pUserData->szName,pGameEnd->lGameScore[i]);
			strEnd += strTemp;
		}

		//��Ϣ����
		InsertSystemString((LPCTSTR)strEnd);
		m_GameClientView.m_GameScoreWnd.SetScoreInfo(ScoreInfo,WeaveInfo);
		DeductionTableCard(true);
		m_GameClientView.SetHuangZhuang(true);
		InsertSystemString("��ׯ!");

	}



	//�����˿�
	m_GameClientView.m_UserCard[0].SetCardData(0,false);
	m_GameClientView.m_UserCard[1].SetCardData(0,false);
	m_GameClientView.m_UserCard[2].SetCardData(0,false);
	//m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0);
	SetHandCardData(NULL,0,0);


	SleepEx(1000,TRUE);
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

	//���ý���
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		SetGameTimer(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
	}


	//���ý���
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		SetGameTimer(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
	}
	//ȡ���й�
	if(m_bStustee)
		OnStusteeControl(0,0);

	for (BYTE i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=SwitchViewChairID(i);
		m_GameClientView.m_lAllTurnScore[wViewChairID]=pGameEnd->lAllTurnScore[i];
		m_GameClientView.m_lLastTurnScore[wViewChairID]=pGameEnd->lLastTurnScore[i];

	}

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	return true;
}

//��������
/*void CGameClientDlg::PlayCardSound(BYTE cbCardData,BYTE cbManderin)
{
	//��������
	TCHAR szSoundName[16]=TEXT("");
	TCHAR szKind[]={TEXT('W'),TEXT('S'),TEXT('T'),TEXT('F')};
	TCHAR szManderin[]={TEXT('P'),TEXT('G')};

	//��������
	BYTE cbValue=(cbCardData&MASK_VALUE);
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;
	_snprintf(szSoundName,CountArray(szSoundName),TEXT("%c%c_%d"),szManderin[m_cbManderin],szKind[cbColor],cbValue);

	//��������
	PlayGameSound(AfxGetInstanceHandle(),szSoundName);

	return;
}*/

//��������
//��������
void CGameClientDlg::PlayActionSound(WORD wChairID,WORD wAction)
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
	bool bBoy = (pUserData->cbGender == 2 ? false : true);

	switch (wAction)
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
	case WIK_DNBL:
	case WIK_DNBC:
	case WIK_DNBR:
	case WIK_DXBL:
	case WIK_DXBC:
	case WIK_DXBR:
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
	case WIK_CHI_HU:	//�Ժ�
		{

			if(bBoy)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_CHI_HU"));
			else
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_CHI_HU"));		
			break;
		}
	}

	return;
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
//�۳��˿�
void CGameClientDlg::DeductionTableCard(bool bHeadCard)
{
	if (bHeadCard==true)
	{
		//�л�����
		BYTE cbHeapCount=m_cbHeapCardInfo[m_wHeapHand][0]+m_cbHeapCardInfo[m_wHeapHand][1];
		if (cbHeapCount==HEAP_FULL_COUNT) 
			m_wHeapHand=(m_wHeapHand+1)%CountArray(m_cbHeapCardInfo);

		//�����˿�
		m_cbLeftCardCount--;
		m_cbHeapCardInfo[m_wHeapHand][0]++;

		//��������
		if((m_wHeapHand == m_wKingHeapPos)
			&&(m_cbHeapCardInfo[m_wHeapHand][0] == (HEAP_FULL_COUNT/2-m_wKingPos+1)*2-1))
		{
			m_cbLeftCardCount--;
			m_cbHeapCardInfo[m_wHeapHand][0]++;
			if((GetMeChairID() == m_wCurrentUser)&&( false == IsLookonMode() ))
			{
				//InsertSystemString(_T("��������"));
				SendData(SUB_C_PASS_KING);
			}
		}

		//�����˿�
		WORD wHeapViewID=SwitchViewChairID(m_wHeapHand);
		WORD wMinusHeadCount=m_cbHeapCardInfo[m_wHeapHand][0];
		WORD wMinusLastCount=m_cbHeapCardInfo[m_wHeapHand][1];
		m_GameClientView.m_HeapCard[wHeapViewID].SetCardData(wMinusHeadCount,wMinusLastCount,HEAP_FULL_COUNT);
	}
	else
	{
		//�л�����
		BYTE cbHeapCount=m_cbHeapCardInfo[m_wHeapTail][0]+m_cbHeapCardInfo[m_wHeapTail][1];
		if (cbHeapCount==HEAP_FULL_COUNT) 
			m_wHeapTail=(m_wHeapTail+3)%CountArray(m_cbHeapCardInfo);

		//�����˿�
		m_cbLeftCardCount--;
		m_cbHeapCardInfo[m_wHeapTail][1]++;
		if((m_wHeapTail == m_wKingHeapPos) 
			&&(m_cbHeapCardInfo[m_wHeapTail][1] == (m_wKingPos*2-1)))
		{
			m_cbLeftCardCount--;
			m_cbHeapCardInfo[m_wHeapTail][1]++;
			if((GetMeChairID() == m_wCurrentUser)&&( false == IsLookonMode() ))
			{
				//InsertSystemString(_T("��������"));
				SendData(SUB_C_PASS_KING);
			}
		}

		//�����˿�
		WORD wHeapViewID=SwitchViewChairID(m_wHeapTail);
		WORD wMinusHeadCount=m_cbHeapCardInfo[m_wHeapTail][0];
		WORD wMinusLastCount=m_cbHeapCardInfo[m_wHeapTail][1];
		m_GameClientView.m_HeapCard[wHeapViewID].SetCardData(wMinusHeadCount,wMinusLastCount,HEAP_FULL_COUNT);
	}

	return;
}

//��ʾ����
bool CGameClientDlg::ShowOperateControl(WORD wUserAction, BYTE cbActionCard)
{
	//��������
	tagGangCardResult GangCardResult;
	ZeroMemory(&GangCardResult,sizeof(GangCardResult));

	//�����ж�
	if ((wUserAction&WIK_GANG)!=0)
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
			m_GameLogic.AnalyseGangCard(m_cbCardIndex[2],m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID],GangCardResult);
		}
	}

	//��ʾ����
	m_GameClientView.m_ControlWnd.SetControlInfo(cbActionCard,wUserAction,GangCardResult);

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
	m_GameClientView.SetDiscUser(INVALID_CHAIR);
	m_GameClientView.SetHuangZhuang(false);
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
	m_GameClientView.SetKingCardIndex(m_cbKingCardIndex);
	m_GameClientView.m_cbUpDownKingCardData=0;
	m_GameClientView.m_cbFloor=0;




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
		m_cbHeapCardInfo[i][0]=0;
		m_cbHeapCardInfo[i][1]=HEAP_FULL_COUNT-(((i==m_wBankerUser)||((i+2)%4==m_wBankerUser))?34:34);
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
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_HeapCard[i].SetKingCardData(0xFF);
		m_GameClientView.m_HeapCard[i].SetKingPos(WORD(-1));
		m_GameClientView.m_DiscardCard[i].SetKingCardIndex(m_cbKingCardIndex);
		m_GameClientView.m_TableCard[i].SetKingCardIndex(m_cbKingCardIndex);
		for (WORD j=0;j<MAX_WEAVE;j++)
		{
			m_GameClientView.m_WeaveCard[i][j].SetKingCardIndex(m_cbKingCardIndex);
		}
	}
	m_GameClientView.m_HandCardControl.SetKingCardIndex(m_cbKingCardIndex);

	return 0;
}

//���Ʋ���
LRESULT CGameClientDlg::OnOutCard(WPARAM wParam, LPARAM lParam)
{
	//�����ж�
	if ((IsLookonMode()==true)||(m_wCurrentUser!=GetMeChairID()))	return 0;
	
	BYTE cbOutCardData=(BYTE)wParam;
	if(VerdictOutCard(cbOutCardData) == false)
	{
		return 0;
	}

	//���ñ���
	m_wCurrentUser=INVALID_CHAIR;
	m_GameLogic.RemoveCard(m_cbCardIndex[2],cbOutCardData);
	m_wUserAction =0;

	//�����˿�
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[2],cbCardData,MAX_COUNT);
	//m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount,0);
	SetHandCardData(cbCardData,cbCardCount,0);


	//���ý���
	KillGameTimer(IDI_OPERATE_CARD);
	m_GameClientView.UpdateGameView(NULL);
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetOutCardInfo(2,cbOutCardData);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);

	//��������
	PlayCardSound(GetMeChairID(),cbOutCardData);

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
	WORD wOperateCode=(WORD)(wParam);
	BYTE cbOperateCard=(BYTE)(LOWORD(lParam));

	//״̬�ж�
	if ((m_wCurrentUser==GetMeChairID())&&(wOperateCode==WIK_NULL))
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
	OperateCard.wOperateCode=wOperateCode;
	OperateCard.cbOperateCard=cbOperateCard;
	SendData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	return 0;
}
//ɸ�Ӷ�����ʾ���
LRESULT CGameClientDlg::OnSiceTwo(WPARAM wParam, LPARAM lParam)
{
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));
	return 0;
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
	m_GameClientView.SetKingCardIndex(m_cbKingCardIndex);
	BYTE cbKingIndex[]={0xFF,0xFF};
	m_GameClientView.m_HandCardControl.SetKingCardIndex(cbKingIndex);
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_HeapCard[i].SetKingCardData(m_GameLogic.SwitchToCardData(m_cbKingCardIndex[0]));
		m_GameClientView.m_DiscardCard[i].SetKingCardIndex(m_cbKingCardIndex);
		m_GameClientView.m_TableCard[i].SetKingCardIndex(m_cbKingCardIndex);
		for (WORD j=0;j<MAX_WEAVE;j++)
		{
			m_GameClientView.m_WeaveCard[i][j].SetKingCardIndex(m_cbKingCardIndex);
		}
	}

	//�˿�����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wViewChairID=SwitchViewChairID(i);

		//��Ͻ���
		m_GameClientView.m_WeaveCard[i][0].SetDisplayItem(1);
		m_GameClientView.m_WeaveCard[i][1].SetDisplayItem(1);
		m_GameClientView.m_WeaveCard[i][2].SetDisplayItem(1);
		m_GameClientView.m_WeaveCard[i][3].SetDisplayItem(1);
		m_GameClientView.m_HeapCard[wViewChairID].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1],HEAP_FULL_COUNT);

		//�û��˿�
		if (wViewChairID!=2)
		{
			WORD wIndex=(wViewChairID>=3)?2:wViewChairID;
			m_GameClientView.m_UserCard[wIndex].SetCardData(CountArray(m_cbCardData)-1,(i==m_wBankerUser));
		}
		else
		{


			BYTE cbCardData[MAX_COUNT];
			CopyMemory(cbCardData,m_cbCardData,sizeof(cbCardData));
			for (BYTE j=0;j<CountArray(cbCardData)/2-1;j++)
			{
				BYTE cbTempData=cbCardData[j];
				cbCardData[j]=cbCardData[j+7];
				cbCardData[j+7]=cbTempData;
			}

			BYTE cbBankerCard=(i==m_wBankerUser)?cbCardData[MAX_COUNT-1]:0;
			SetHandCardData(cbCardData,MAX_COUNT-1,cbBankerCard);
			m_GameClientView.UpdateWindow();
			SleepEx(500,FALSE);

			ZeroMemory(cbCardData,sizeof(cbCardData));
			SetHandCardData(NULL,0,0);

			m_GameClientView.m_TableCard[2].SetCardData(cbCardData,(i==m_wBankerUser)?MAX_COUNT:MAX_COUNT-1);
			m_GameClientView.UpdateWindow();

			m_GameClientView.m_DrawSiceWnd2.SetSiceInfo(m_GameClientView.GetDC(),250,HIBYTE(m_wSiceCount3),LOBYTE(m_wSiceCount3));
			m_GameClientView.m_DrawSiceWnd2.ShowWindow(SW_SHOW);

		}	
	}
	m_bStart2=false;
	return 0;
}

//��Ϸ��ʼ
LRESULT CGameClientDlg::OnGameStart2(WPARAM wParam, LPARAM lParam)
{

	//InsertSystemString(TEXT("OnGameStart2()"));
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));
	m_GameClientView.m_TableCard[2].SetCardData(NULL,0);
	bool bPlayerMode=(IsLookonMode()==false);
	m_GameClientView.m_DrawSiceWnd2.ShowWindow(SW_HIDE);
	m_GameClientView.m_HandCardControl.SetPositively(bPlayerMode);
	m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));
	m_GameClientView.SetKingCardIndex(m_cbKingCardIndex);
	m_GameClientView.m_HandCardControl.SetKingCardIndex(m_cbKingCardIndex);

	//�˿�����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wViewChairID=SwitchViewChairID(i);

		if (wViewChairID==2)
		{
			m_GameLogic.SetKingCardIndex(m_cbKingCardIndex);    

			BYTE cbBankerCard=(i==m_wBankerUser)?m_cbCardData[MAX_COUNT-1]:0;

			SetHandCardData(m_cbCardData,MAX_COUNT-1,cbBankerCard);

			//������ʾ
			if ((bPlayerMode==true)&&(m_wCurrentUser!=INVALID_CHAIR))
			{
				WORD wMeChairID=GetMeChairID();
				if (m_wCurrentUser==wMeChairID) 
					m_GameClientView.SetStatusFlag(true,false);
			}

			//��������
			if ((bPlayerMode==true)&&(m_wUserAction!=WIK_NULL))
			{
				ShowOperateControl(m_wUserAction,0);
				SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD*2/3);
			}
			//����ʱ��
			if (m_wCurrentUser!=INVALID_CHAIR)
				SetGameTimer(m_wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
	
		}
	}

	//���ƾ���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if(i== m_wKingHeapPos)
		{
			m_GameClientView.m_HeapCard[SwitchViewChairID(m_wKingHeapPos)].SetKingCardData(m_GameLogic.SwitchToCardData(m_cbKingCardIndex[0]));
			m_GameClientView.m_HeapCard[SwitchViewChairID(m_wKingHeapPos)].SetKingPos(m_wKingPos);
		}
		else
		{
			m_GameClientView.m_HeapCard[SwitchViewChairID(i)].SetKingCardData(0XFF);
			m_GameClientView.m_HeapCard[SwitchViewChairID(i)].SetKingPos(WORD(-1));

		}
	}

	//���û���
	CString strTemp ,strEnd="\n���·��÷�:\n";
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		const tagUserData * pUserData=GetUserData(i);
		if (!pUserData)
		{
			continue;
		}

		strTemp.Format("%s: %ld\n",pUserData->szName,m_lUpDownScore[SwitchViewChairID(i)]);
		strEnd += strTemp;

	}
	//��Ϣ����
	InsertSystemString((LPCTSTR)strEnd);
	m_GameClientView.m_cbUpDownKingCardData=m_cbUpDownKingCardData;
	m_GameClientView.m_cbFloor=m_cbFloor;
	CopyMemory(m_GameClientView.m_lUpDownScore,m_lUpDownScore,sizeof(m_lUpDownScore));
	//��������
	IClientKernel *pIClientKenel=(IClientKernel *)GetClientKernel(IID_IClientKernel,VER_IClientKernel);
	ASSERT(pIClientKenel!=NULL);
	const tagUserData * pMeUserData=pIClientKenel->GetMeUserInfo();
	const tagServerAttribute * pServerAttribute=pIClientKenel->GetServerAttribute();

	//��������  ����Ա
	if(IsLookonMode()==true &&(pServerAttribute->wGameGenre&GAME_GENRE_MATCH)&&(pMeUserData->cbMasterOrder>0))
	{
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//��ͨ�Թ�
			WORD wUserIndex=(SwitchViewChairID(i)>2)?2:SwitchViewChairID(i);
			m_GameClientView.m_UserCard[wUserIndex].SetCardData(0,false);

			//�����˿�
			if(SwitchViewChairID(i)==2) continue;
			BYTE cbCardData[MAX_COUNT];
			BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[SwitchViewChairID(i)],cbCardData,MAX_COUNT);
			m_GameClientView.m_TableCard[SwitchViewChairID(i)].SetCardData(cbCardData,cbCardCount);
		}
	}



	m_GameClientView.UpdateWindow();
	m_bStart2=true;

	return 0;
}


//ϴ�趯�����
LRESULT CGameClientDlg::OnBatchEnd(WPARAM wParam,LPARAM lParam)
{
	//�����û�
	if(IsLookonMode()==false)
	{
		SendData(SUB_C_BATCH_END);
	}
	
	return 0;
}

//������Ϣ
void CGameClientDlg::SetHandCardData(BYTE cbCardData[], WORD wCardCount, BYTE cbCurrentCard)
{

	m_GameClientView.m_HandCardControl.SetCardData(cbCardData,wCardCount, cbCurrentCard);

}

//����֮��ǰ��
void CGameClientDlg::SetHandCurrentCard(BYTE cbCurrentCard)
{

	m_GameClientView.m_HandCardControl.SetCurrentCard(cbCurrentCard);
}

//����У��
bool CGameClientDlg::VerdictOutCard(BYTE cbPreCardData)
{

	return true;
}
//ϴ��
LRESULT CGameClientDlg::OnBatch(WPARAM wParam,LPARAM lParam)
{	
	//ɾ����ʱ��
	KillGameTimer(IDI_BATCH);

	//���ý���
	m_GameClientView.m_btBatch.ShowWindow(SW_HIDE);

	//��������
	CMD_C_BATCH Chiper;
	Chiper.cbChipTimes=(BYTE)wParam;
	Chiper.wCurrentUser=GetMeChairID();
	if(Chiper.cbChipTimes==1)
		PlayGameSound(AfxGetInstanceHandle(),TEXT("ONE_SCORE"));

	
	SendData(SUB_C_BATCH,&Chiper,sizeof(Chiper));
	//InsertSystemString(TEXT("OK"));
	return 0;
}

//��Ϸϴ��
bool CGameClientDlg::OnSubGameChip(const void *pBuffer,WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==0);
	if (wDataSize!=0) return false;

	//����״̬
	SetGameStatus(GS_MJ_BATCH);

	//�������
	 if(IsLookonMode()==false)
	 {
		 m_GameClientView.m_btBatch.ShowWindow(SW_SHOW);

	 }
	 //���ö�ʱ��
	 SetGameTimer(GetMeChairID(),IDI_BATCH,TIME_BATCH);
	 return true;
}
//�Ϲܿ���
LRESULT CGameClientDlg::OnStusteeControl(WPARAM wParam, LPARAM lParam)
{
	//���ñ���
	m_wTimeOutCount=0;

	//����״̬
	if (m_bStustee==true)
	{
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

//���ų�������
void CGameClientDlg::PlayCardSound(WORD wChairID, BYTE cbCardData)
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

	if(bBoy)
	{
		strSoundName = _T("BOY_") +strSoundName;
	}
	else
	{
		strSoundName = _T("GIRL_") + strSoundName;
	}
	PlayGameSound(AfxGetInstanceHandle(), strSoundName);
}


//��������
void CGameClientDlg::PlayHuSound(WORD wChairID,DWORD dwChiHuKind,DWORD dwCHiHuRight)
{
	//�ж��Ա�
	const tagUserData* pUserData = GetUserData(wChairID);
	if(pUserData == 0)
	{
		//assert(0 && "�ò��������Ϣ");
		return;
	}
	if(wChairID < 0 || wChairID > 3)
	{
		return;
	}
	if(dwChiHuKind == 0) return;
	bool bGirl = pUserData->cbGender == 2 ? true : false;
	///bool bGirl=false;
	CString strSoundName;
	if(dwCHiHuRight&CHR_TIAN)
	{
		strSoundName=TEXT("TIANHU");
	}
	else if(dwCHiHuRight&CHR_DI)
	{
		strSoundName=TEXT("DIHU");
	}
	
	else if(dwCHiHuRight&CHR_GERMAN_GERMAN)
	{
		strSoundName=TEXT("DEZHONGDE");
	}
	else if(dwCHiHuRight&CHR_GERMAN)
	{
		strSoundName=TEXT("DEGUO");
	}
	else if(dwCHiHuRight&CHR_KING_WAIT)
	{
		strSoundName=TEXT("JINDAO");
	}
	else if(dwCHiHuRight&CHR_GANG_FLOWER)
	{
		strSoundName=TEXT("GSKH");
	}
	else
	{
	
		if(dwChiHuKind&CHK_GERMAN_SERVEN)
		{
			strSoundName=TEXT("QIXING");
		}
		else if(dwChiHuKind&CHK_QI_DUI)
		{
			strSoundName=TEXT("QIDUI");
		}
		else if(dwChiHuKind&CHK_PENG_PENG)
		{
			strSoundName=TEXT("DAQIDUI");
		}
		else if(dwChiHuKind&CHK_THIRTEEN)
		{
			strSoundName=TEXT("SHISANLAN");
		}
		else
		{
			strSoundName+TEXT("PINHU");
		}
	}
	


	if(!bGirl)
	{
		strSoundName = _T("BOY_") +strSoundName;
	}
	else
	{
		strSoundName = _T("GIRL_") + strSoundName;
	}
	PlayGameSound(AfxGetInstanceHandle(), strSoundName);
	return;
}

//////////////////////////////////////////////////////////////////////////
