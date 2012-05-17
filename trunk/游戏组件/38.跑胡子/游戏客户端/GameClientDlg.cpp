#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ʱ��
#define IDI_START_GAME				200									//��ʼ��ʱ��
#define IDI_OUT_CARD				201									//���ƶ�ʱ��
#define IDI_OPERATE_CARD			202									//������ʱ��
#define IDI_ASSISTANT_TIME			203									//������ʱ��

//������ʱ��
#define IDI_USER_ACTION				400									//������ʱ��
#define TIME_USER_ACTION			1000								//������ʱ��

//ʱ����
#define TIME_OUT_CARD				30									//����ʱ��
#define TIME_START_GAME				60									//��ʼʱ��
#define TIME_OPERATE_CARD			30									//����ʱ��
#define TIME_ASSISTANT_TIME			45									//����ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_CHOOSE_CARD,OnChooseCard)
	ON_MESSAGE(IDM_CARD_OPERATE,OnCardOperate)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_bUserCardCount,sizeof(m_bUserCardCount));
	ZeroMemory(m_bUserHuXiCount,sizeof(m_bUserHuXiCount));

	//��������
	m_bOutCard=false;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//����˿�
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_bWeaveItemCount,sizeof(m_bWeaveItemCount));

	//��������
	m_wActionCount=0;
	ZeroMemory(m_UserActionArray,sizeof(m_UserActionArray));

	//�˿˱���
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

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
	SetWindowText(TEXT("�����ܺ���  --  Ver��1.0.5.1"));

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
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_bUserCardCount,sizeof(m_bUserCardCount));
	ZeroMemory(m_bUserHuXiCount,sizeof(m_bUserHuXiCount));

	//��������
	m_bOutCard=false;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//����˿�
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_bWeaveItemCount,sizeof(m_bWeaveItemCount));

	//��������
	m_wActionCount=0;
	ZeroMemory(m_UserActionArray,sizeof(m_UserActionArray));

	//�˿˱���
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

	return;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{
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
			if (nElapse==5) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_OUT_CARD:			//�û�����
		{
			//��ʱ�ж�
			if ((nElapse==0)&&(m_wCurrentUser==GetMeChairID())&&(IsLookonMode()==false))
			{
				//Ѱ�ҵ���
				for (BYTE i=0;i<CountArray(m_cbCardIndex);i++)
				{
					if (m_cbCardIndex[i]==1)
					{
						OnOutCard(m_GameLogic.SwitchToCardData(i),0L);
						return false;
					}
				}

				//Ѱ�Ҷ���
				for (BYTE i=0;i<CountArray(m_cbCardIndex);i++)
				{
					if (m_cbCardIndex[i]==2)
					{
						OnOutCard(m_GameLogic.SwitchToCardData(i),0L);
						return false;
					}
				}

				return false;
			}

			//��������
			if ((nElapse==5)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_OPERATE_CARD:		//������ʱ��
		{
			//��ʱ�ж�
			if ((nElapse==0)&&(IsLookonMode()==false))
			{
				OnCardOperate(ACK_NULL,0);
				return false;
			}

			//��������
			if ((nElapse==5)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_ASSISTANT_TIME:	//������ʱ��
		{
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
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_USER_TI_CARD:	//�û�����
		{
			return OnSubUserTiCard(pBuffer,wDataSize);
		}
	case SUB_S_USER_PAO_CARD:	//�û�����
		{
			return OnSubUserPaoCard(pBuffer,wDataSize);
		}
	case SUB_S_USER_WEI_CARD:	//�û�����
		{
			return OnSubUserWeiCard(pBuffer,wDataSize);
		}
	case SUB_S_USER_PENG_CARD:	//�û�����
		{
			return OnSubUserPengCard(pBuffer,wDataSize);
		}
	case SUB_S_USER_CHI_CARD:	//�û�����
		{
			return OnSubUserChiCard(pBuffer,wDataSize);
		}
	case SUB_S_OPERATE_NOTIFY:	//������ʾ
		{
			return OnSubOperateNotify(pBuffer,wDataSize);
		}
	case SUB_S_OUT_CARD_NOTIFY:	//������ʾ
		{
			return OnSubOutCardNotify(pBuffer,wDataSize);
		}
	case SUB_S_OUT_CARD:		//������Ϣ
		{
			return OnSubOutCard(pBuffer,wDataSize);
		}
	case SUB_S_SEND_CARD:		//������Ϣ
		{
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	}

	return false;
}

//��Ϸ����
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GS_FREE:		//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//��Ϸ��Ϣ
			m_wBankerUser=pStatusFree->wBankerUser;
			m_GameClientView.SetBankerUser(m_wBankerUser);
			m_GameClientView.SetCellScore(pStatusFree->lCellScore);

			//�ؼ�����
			m_GameClientView.m_HandCardControl.SetDisplayItem(true);

			//���ÿؼ�
			if (IsLookonMode()==false)
			{
				m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
				m_GameClientView.m_btStart.SetFocus();
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

			//���ñ���
			m_wBankerUser=pStatusPlay->wBankerUser;
			m_wCurrentUser=pStatusPlay->wCurrentUser;
			m_wOutCardUser=pStatusPlay->wOutCardUser;
			m_cbOutCardData=pStatusPlay->cbOutCardData;
			m_bOutCard=(pStatusPlay->bOutCard==TRUE)?true:false;
			CopyMemory(m_cbCardIndex,pStatusPlay->cbCardIndex,sizeof(m_cbCardIndex));
			CopyMemory(m_bUserCardCount,pStatusPlay->bUserCardCount,sizeof(m_bUserCardCount));
			CopyMemory(m_WeaveItemArray,pStatusPlay->WeaveItemArray,sizeof(m_WeaveItemArray));
			CopyMemory(m_bWeaveItemCount,pStatusPlay->bWeaveItemCount,sizeof(m_bWeaveItemCount));

			//��Ϣ��Ϣ
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//���ú�Ϣ
				m_bUserHuXiCount[i]=0;

				//��Ϣ����
				for (BYTE j=0;j<m_bWeaveItemCount[i];j++) 
				{
					m_bUserHuXiCount[i]+=m_GameLogic.GetWeaveHuXi(m_WeaveItemArray[i][j]);
				}
			}

			//�˿���Ϣ
			BYTE cbCardData[MAX_COUNT];
			BYTE cbCardCount=m_GameLogic.SwitchToCardData(pStatusPlay->cbCardIndex,cbCardData,CountArray(cbCardData));

			//���ý���
			m_GameClientView.SetCellScore(pStatusPlay->lCellScore);
			m_GameClientView.SetLeftCardCount(pStatusPlay->bLeftCardCount);
			m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));
			m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount);

			//���ƽ���
			if (m_wOutCardUser!=INVALID_CHAIR) 
			{
				WORD wViewChairID=SwitchViewChairID(m_wOutCardUser);
				m_GameClientView.SetOutCardInfo(wViewChairID,m_cbOutCardData);
			}

			//��Ϸ����
			for (WORD i=0;i<GAME_PLAYER;i++) 
			{
				//��������
				WORD wViewChairID=SwitchViewChairID(i);

				//���ú�Ϣ
				m_GameClientView.SetUserHuXiCount(wViewChairID,m_bUserHuXiCount[i]);

				//�û��˿�
				if (i!=GetMeChairID())
				{
					WORD wIndex=(wViewChairID==0)?0:1;
					m_GameClientView.m_UserCard[wIndex].SetCardData(pStatusPlay->bUserCardCount[i]);
				}

				//��Ͻ���
				for (BYTE j=0;j<m_bWeaveItemCount[i];j++) 
				{
					m_GameClientView.m_WeaveCard[wViewChairID][j].SetCardData(m_WeaveItemArray[i][j]);
				}
			}

			//��������
			if ((pStatusPlay->bUserAction!=ACK_NULL)&&(pStatusPlay->bResponse==FALSE)&&(IsLookonMode()==false))
			{
				ASSERT(m_wCurrentUser==INVALID_CHAIR);
				SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
				m_GameClientView.m_ControlWnd.SetControlInfo(m_cbOutCardData,pStatusPlay->bUserAction);
			}

			//�ؼ�����
			m_GameClientView.m_HandCardControl.SetDisplayItem(true);
			m_GameClientView.m_HandCardControl.SetPositively(IsLookonMode()==false);

			//����ʱ��
			if (m_wCurrentUser!=INVALID_CHAIR) 
			{
				if (m_bOutCard==true) 
				{
					SetGameTimer(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);
					if ((IsLookonMode()==false)&&(GetMeChairID()==m_wCurrentUser)) m_GameClientView.SetStatusFlag(true,false);
				}
				else if ((GetMeChairID()==m_wCurrentUser)&&(IsLookonMode()==false)) SendData(SUB_C_CONTINUE_CARD);
			}

			return true;
		}
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//��ֹ����
bool CGameClientDlg::HangUserAction()
{
	//��ֹ�ж�
	if (m_wActionCount==0) return true;

	//ɾ����ʱ��
	KillTimer(IDI_USER_ACTION);

	//���ñ���
	m_wActionCount=0;
	ZeroMemory(m_UserActionArray,sizeof(m_UserActionArray));

	return true;
}

//ִ�ж���
bool CGameClientDlg::ExecuteActionPaoCard(WORD wChairID, BYTE cbCardData, BYTE cbRemoveCount)
{
	//�����ж�
	bool bExistWeave=false;
	for (BYTE cbIndex=0;cbIndex<m_bWeaveItemCount[wChairID];cbIndex++)
	{
		//��������
		BYTE cbWeaveKind=m_WeaveItemArray[wChairID][cbIndex].cbWeaveKind;
		BYTE cbWeaveCard=m_WeaveItemArray[wChairID][cbIndex].cbCardList[0];

		//�����ж�
		if ((cbCardData==cbWeaveCard)&&((cbWeaveKind==ACK_PENG)||(cbWeaveKind==ACK_WEI))) 
		{
			bExistWeave=true;
			break;
		}
	}

	//�˿���Ŀ
	m_bUserCardCount[wChairID]-=cbRemoveCount;

	//�˿�����
	if ((bExistWeave==false)&&(wChairID==GetMeChairID()))
	{
		BYTE cbCardList[3]={cbCardData,cbCardData,cbCardData};
		m_GameLogic.RemoveCard(m_cbCardIndex,cbCardList,cbRemoveCount);
	}

	//�������
	if (bExistWeave==false) m_bWeaveItemCount[wChairID]++;
	m_WeaveItemArray[wChairID][cbIndex].cbCardCount=4;
	m_WeaveItemArray[wChairID][cbIndex].cbWeaveKind=ACK_PAO;
	m_WeaveItemArray[wChairID][cbIndex].cbCenterCard=cbCardData;
	m_WeaveItemArray[wChairID][cbIndex].cbCardList[0]=cbCardData;
	m_WeaveItemArray[wChairID][cbIndex].cbCardList[1]=cbCardData;
	m_WeaveItemArray[wChairID][cbIndex].cbCardList[2]=cbCardData;
	m_WeaveItemArray[wChairID][cbIndex].cbCardList[3]=cbCardData;

	//���º�Ϣ
	UpdateUserHuXiCount(wChairID);

	//ɾ����ʱ��
	KillGameTimer(IDI_ASSISTANT_TIME);

	//��ʾ����
	tagActionTip ActionTip;
	ActionTip.wChairID=wChairID;
	ActionTip.cbActionFlags=ACK_PAO;
	ExecuteAction(ACTION_TIP,&ActionTip,sizeof(ActionTip));

	//���ƶ���
	tagActionPaoCard ActionPaoCard;
	ActionPaoCard.cbIndex=cbIndex;
	ActionPaoCard.wChairID=wChairID;
	ActionPaoCard.cbRemoveCount=cbRemoveCount;
	ActionPaoCard.bHuXiCount=m_bUserHuXiCount[wChairID];
	ExecuteAction(ACTION_PAO_CARD,&ActionPaoCard,sizeof(ActionPaoCard));

	return true;
}

//ִ�ж���
bool CGameClientDlg::ExecuteActionWeiCard(WORD wChairID, BYTE cbCardData)
{
	//�˿���Ŀ
	m_bUserCardCount[wChairID]-=2;

	//�˿�����
	if (wChairID==GetMeChairID())
	{
		BYTE cbCardList[]={cbCardData,cbCardData};
		m_GameLogic.RemoveCard(m_cbCardIndex,cbCardList,CountArray(cbCardList));
	}

	//�������
	BYTE cbIndex=m_bWeaveItemCount[wChairID]++;
	m_WeaveItemArray[wChairID][cbIndex].cbCardCount=3;
	m_WeaveItemArray[wChairID][cbIndex].cbWeaveKind=ACK_WEI;
	m_WeaveItemArray[wChairID][cbIndex].cbCenterCard=cbCardData;
	m_WeaveItemArray[wChairID][cbIndex].cbCardList[0]=cbCardData;
	m_WeaveItemArray[wChairID][cbIndex].cbCardList[1]=cbCardData;
	m_WeaveItemArray[wChairID][cbIndex].cbCardList[2]=cbCardData;

	//���º�Ϣ
	UpdateUserHuXiCount(wChairID);

	//ɾ����ʱ��
	KillGameTimer(IDI_ASSISTANT_TIME);

	//��ʾ����
	tagActionTip ActionTip;
	ActionTip.wChairID=wChairID;
	ActionTip.cbActionFlags=ACK_WEI;
	ExecuteAction(ACTION_TIP,&ActionTip,sizeof(ActionTip));

	//���ƶ���
	tagActionWeiCard ActionWeiCard;
	ActionWeiCard.cbIndex=cbIndex;
	ActionWeiCard.wChairID=wChairID;
	ActionWeiCard.bHuXiCount=m_bUserHuXiCount[wChairID];
	ExecuteAction(ACTION_WEI_CARD,&ActionWeiCard,sizeof(ActionWeiCard));

	return true;
}

//ִ�ж���
bool CGameClientDlg::ExecuteActionPengCard(WORD wChairID, BYTE cbCardData)
{
	//�˿���Ŀ
	m_bUserCardCount[wChairID]-=2;

	//�˿�����
	if (wChairID==GetMeChairID())
	{
		BYTE cbCardList[2]={cbCardData,cbCardData};
		m_GameLogic.RemoveCard(m_cbCardIndex,cbCardList,CountArray(cbCardList));
	}

	//ɾ����ʱ��
	KillGameTimer(IDI_OPERATE_CARD);
	KillGameTimer(IDI_ASSISTANT_TIME);

	//���ý���
	m_GameClientView.m_ChooseWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);

	//�������
	BYTE cbIndex=m_bWeaveItemCount[wChairID]++;
	m_WeaveItemArray[wChairID][cbIndex].cbCardCount=3;
	m_WeaveItemArray[wChairID][cbIndex].cbWeaveKind=ACK_PENG;
	m_WeaveItemArray[wChairID][cbIndex].cbCenterCard=cbCardData;
	m_WeaveItemArray[wChairID][cbIndex].cbCardList[0]=cbCardData;
	m_WeaveItemArray[wChairID][cbIndex].cbCardList[1]=cbCardData;
	m_WeaveItemArray[wChairID][cbIndex].cbCardList[2]=cbCardData;

	//���º�Ϣ
	UpdateUserHuXiCount(wChairID);

	//��ʾ����
	tagActionTip ActionTip;
	ActionTip.wChairID=wChairID;
	ActionTip.cbActionFlags=ACK_PENG;
	ExecuteAction(ACTION_TIP,&ActionTip,sizeof(ActionTip));

	//���ƶ���
	tagActionPengCard ActionPengCard;
	ActionPengCard.cbIndex=cbIndex;
	ActionPengCard.wChairID=wChairID;
	ActionPengCard.bHuXiCount=m_bUserHuXiCount[wChairID];
	ExecuteAction(ACTION_PENG_CARD,&ActionPengCard,sizeof(ActionPengCard));

	return true;
}

//ִ�ж���
bool CGameClientDlg::ExecuteActionChiCard(WORD wChairID, BYTE cbCardData, BYTE cbResultCount, BYTE cbResultData[][3])
{
	//��������
	BYTE cbFirstIndex=m_bWeaveItemCount[wChairID];

	//�˿���Ŀ
	m_bUserCardCount[wChairID]-=cbResultCount*3-1;

	//�������
	for (BYTE k=0;k<cbResultCount;k++)
	{
		BYTE cbIndex=m_bWeaveItemCount[wChairID]++;
		m_WeaveItemArray[wChairID][cbIndex].cbCardCount=3;
		m_WeaveItemArray[wChairID][cbIndex].cbWeaveKind=ACK_CHI;
		m_WeaveItemArray[wChairID][cbIndex].cbCenterCard=cbCardData;
		m_WeaveItemArray[wChairID][cbIndex].cbCardList[0]=cbResultData[k][0];
		m_WeaveItemArray[wChairID][cbIndex].cbCardList[1]=cbResultData[k][1];
		m_WeaveItemArray[wChairID][cbIndex].cbCardList[2]=cbResultData[k][2];
	}

	//ɾ����ʱ��
	KillGameTimer(IDI_OPERATE_CARD);
	KillGameTimer(IDI_ASSISTANT_TIME);

	//���ý���
	m_GameClientView.m_ChooseWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);

	//���º�Ϣ
	UpdateUserHuXiCount(wChairID);

	//ɾ���˿�
	if (wChairID==GetMeChairID())
	{
		BYTE cbDebarCard=cbCardData;
		for (BYTE k=0;k<cbResultCount;k++)
		{
			for (BYTE l=0;l<3;l++)
			{
				BYTE cbRemoveCard=cbResultData[k][l];
				if (cbRemoveCard==cbDebarCard) cbDebarCard=0;
				else m_cbCardIndex[m_GameLogic.SwitchToCardIndex(cbRemoveCard)]--;
			}
		}
	}

	//��ʾ����
	tagActionTip ActionChi;
	ActionChi.wChairID=wChairID;
	ActionChi.cbActionFlags=ACK_CHI;
	ExecuteAction(ACTION_TIP,&ActionChi,sizeof(ActionChi));

	//���ƶ���
	tagActionChiCard ActionChiCard;
	ActionChiCard.wChairID=wChairID;
	ActionChiCard.cbIndex=cbFirstIndex;
	ActionChiCard.cbActionCard=cbCardData;
	ActionChiCard.cbResultCount=cbResultCount;
	ActionChiCard.bHuXiCount=m_bUserHuXiCount[wChairID];
	CopyMemory(ActionChiCard.cbCardData,cbResultData,sizeof(BYTE)*3*cbResultCount);

	//ִ�ж���
	ExecuteAction(ACTION_CHI_CARD,&ActionChiCard,sizeof(ActionChiCard));

	return true;
}

//ִ�ж���
bool CGameClientDlg::ExecuteActionTiCard(WORD wChairID, BYTE cbCardData, BYTE cbRemoveCount)
{
	//�����ж�
	bool bExistWeave=false;
	for (BYTE cbIndex=0;cbIndex<m_bWeaveItemCount[wChairID];cbIndex++)
	{
		//��������
		BYTE cbWeaveKind=m_WeaveItemArray[wChairID][cbIndex].cbWeaveKind;
		BYTE cbWeaveCard=m_WeaveItemArray[wChairID][cbIndex].cbCardList[0];

		//�����ж�
		if ((cbCardData==cbWeaveCard)&&((cbWeaveKind==ACK_PENG)||(cbWeaveKind==ACK_WEI))) 
		{
			bExistWeave=true;
			break;
		}
	}

	//�˿���Ŀ
	m_bUserCardCount[wChairID]-=cbRemoveCount;

	//�˿�����
	if (wChairID==GetMeChairID())
	{
		BYTE cbCardList[4]={cbCardData,cbCardData,cbCardData,cbCardData};
		m_GameLogic.RemoveCard(m_cbCardIndex,cbCardList,cbRemoveCount);
	}

	//�������
	if (bExistWeave==false) m_bWeaveItemCount[wChairID]++;
	m_WeaveItemArray[wChairID][cbIndex].cbCardCount=4;
	m_WeaveItemArray[wChairID][cbIndex].cbWeaveKind=ACK_TI;
	m_WeaveItemArray[wChairID][cbIndex].cbCenterCard=cbCardData;
	m_WeaveItemArray[wChairID][cbIndex].cbCardList[0]=cbCardData;
	m_WeaveItemArray[wChairID][cbIndex].cbCardList[1]=cbCardData;
	m_WeaveItemArray[wChairID][cbIndex].cbCardList[2]=cbCardData;
	m_WeaveItemArray[wChairID][cbIndex].cbCardList[3]=cbCardData;

	//���º�Ϣ
	UpdateUserHuXiCount(wChairID);

	//ɾ����ʱ��
	KillGameTimer(IDI_ASSISTANT_TIME);

	//��ʾ����
	tagActionTip ActionTip;
	ActionTip.wChairID=wChairID;
	ActionTip.cbActionFlags=ACK_TI;
	ExecuteAction(ACTION_TIP,&ActionTip,sizeof(ActionTip));

	//���ƶ���
	tagActionTiCard ActionTiCard;
	ActionTiCard.cbIndex=cbIndex;
	ActionTiCard.wChairID=wChairID;
	ActionTiCard.cbRemoveCount=cbRemoveCount;
	ActionTiCard.bHuXiCount=m_bUserHuXiCount[wChairID];
	ExecuteAction(ACTION_TI_CARD,&ActionTiCard,sizeof(ActionTiCard));

	return true;
}

//ִ�ж���
bool CGameClientDlg::ExecuteActionOutCardNotify(WORD wCurrentUser, BYTE bOutCard)
{
	//ɾ����ʱ��
	KillGameTimer(IDI_ASSISTANT_TIME);

	//������ʾ
	tagActionOutCardNotify ActionOutCardNotify;
	ActionOutCardNotify.bOutCard=bOutCard;
	ActionOutCardNotify.wCurrentUser=wCurrentUser;
	ExecuteAction(ACTION_OUT_CARD_NOTIFY,&ActionOutCardNotify,sizeof(ActionOutCardNotify));

	return true;
}

//ִ�ж���
bool CGameClientDlg::ExecuteActionOperateNotify(BYTE cbOperateCode, BYTE cbOperateCard, WORD wResumeUser)
{
	//������ʾ
	tagActionOperateNotify ActionOperateNotify;
	ActionOperateNotify.wResumeUser=wResumeUser;
	ActionOperateNotify.cbOperateCode=cbOperateCode;
	ActionOperateNotify.cbCurrentCard=cbOperateCard;
	ExecuteAction(ACTION_OPERATE_NOTIFY,&ActionOperateNotify,sizeof(ActionOperateNotify));

	return true;
}

//ִ�ж���
bool CGameClientDlg::ExecuteActionOutCard(WORD wOutCardUser, BYTE cbOutCardData)
{
	//ɾ���˿�
	if ((GetMeChairID()!=wOutCardUser)||(IsLookonMode()==true))
	{
		m_bUserCardCount[wOutCardUser]--;
		if (GetMeChairID()==wOutCardUser) m_GameLogic.RemoveCard(m_cbCardIndex,cbOutCardData);
	}

	//���ƶ���
	tagActionOutCard ActionOutCard;
	ActionOutCard.wOutCardUser=wOutCardUser;
	ActionOutCard.cbOutCardData=cbOutCardData;
	ExecuteAction(ACTION_OUT_CARD,&ActionOutCard,sizeof(ActionOutCard));

	return true;
}

//ִ�ж���
bool CGameClientDlg::ExecuteActionSendCard(BYTE cbCardData, WORD wAttachUser)
{
	//ɾ����ʱ��
	KillGameTimer(IDI_ASSISTANT_TIME);

	//���ƶ���
	tagActionSendCard ActionSendCard;
	ActionSendCard.wAttachUser=wAttachUser;
	ActionSendCard.cbSendCardData=cbCardData;
	ExecuteAction(ACTION_SEND_CARD,&ActionSendCard,sizeof(ActionSendCard));

	return true;
}

//ִ�ж���
bool CGameClientDlg::ExecuteAction(BYTE cbActionKind, void * pActionInfo, WORD wDataSize)
{
	//Ч��״̬
	ASSERT(m_wActionCount<CountArray(m_UserActionArray));
	ASSERT(wDataSize<=sizeof(m_UserActionArray[m_wActionCount].cbActionBuffer));
	if (m_wActionCount>=CountArray(m_UserActionArray)) return false;
	if (wDataSize>sizeof(m_UserActionArray[m_wActionCount].cbActionBuffer)) return false;

	//������ʾ
	m_UserActionArray[m_wActionCount].cbActionKind=cbActionKind;
	CopyMemory(m_UserActionArray[m_wActionCount++].cbActionBuffer,pActionInfo,wDataSize);

	//���ö�ʱ��
	if (m_wActionCount==1) SetTimer(IDI_USER_ACTION,TIME_USER_ACTION,NULL);

	return true;
}

//���º�Ϣ
bool CGameClientDlg::UpdateUserHuXiCount(WORD wChairID)
{
	//��Ϣ����
	BYTE bUserHuXiCount=0;
	for (BYTE i=0;i<m_bWeaveItemCount[wChairID];i++)
	{
		bUserHuXiCount+=m_GameLogic.GetWeaveHuXi(m_WeaveItemArray[wChairID][i]);
	}
	
	//���ú�Ϣ
	m_bUserHuXiCount[wChairID]=bUserHuXiCount;

	//�Լ���Ϣ
	if (wChairID==GetMeChairID())
	{
	}

	return true;
}

//ɾ���˿�
bool CGameClientDlg::RemoveControlCard(WORD wChairID, BYTE cbRemoveCard[], WORD wCardCount)
{
	if (wChairID==GetMeChairID())
	{
		//��ȡ�˿�
		BYTE cbHandCardList[MAX_COUNT];
		WORD wHandCardCount=m_GameClientView.m_HandCardControl.GetCardData(cbHandCardList,CountArray(cbHandCardList));

		//ɾ���˿�
		m_GameLogic.RemoveCard(cbHandCardList,(BYTE)(wHandCardCount),cbRemoveCard,(BYTE)wCardCount);

		//�����˿�
		WORD wResultCount=wHandCardCount-wCardCount;
		m_GameClientView.m_HandCardControl.SetCardData(cbHandCardList,wResultCount);
	}
	else
	{
		//������Ŀ
		WORD wUserIndex=(SwitchViewChairID(wChairID)==2)?1:0;
		WORD wHandCardCount=m_GameClientView.m_UserCard[wUserIndex].GetCardCount();

		//�����˿�
		WORD wResultCount=wHandCardCount-wCardCount;
		m_GameClientView.m_UserCard[wUserIndex].SetCardData(wResultCount);
	}

	return true;
}

//��Ϸ��ʼ
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//��������
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//��������
	m_wResumeUser=INVALID_CHAIR;
	m_wBankerUser=pGameStart->wBankerUser;
	m_wCurrentUser=pGameStart->wCurrentUser;
	for (WORD i=0;i<GAME_PLAYER;i++) m_bUserCardCount[i]=(m_wBankerUser==i)?21:20;

	//��������
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_bWeaveItemCount,sizeof(m_bWeaveItemCount));

	//�����˿�
	BYTE cbCardCount=(GetMeChairID()==m_wBankerUser)?MAX_COUNT:(MAX_COUNT-1);
	m_GameLogic.SwitchToCardIndex(pGameStart->cbCardData,cbCardCount,m_cbCardIndex);

	//���ý���
	bool bPlayerMode=(IsLookonMode()==false);
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
	m_GameClientView.m_HandCardControl.SetPositively(bPlayerMode);
	m_GameClientView.m_HandCardControl.SetDisplayItem(bPlayerMode);

	//״̬����
	m_GameClientView.SetLeftCardCount(19);
	m_GameClientView.SetUserHuXiCount(INVALID_CHAIR,0);
	m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

	//�Թ۽���
	if (bPlayerMode==false)
	{
		m_GameClientView.SetHuangZhuang(false);
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
	}

	//�˿�����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wViewChairID=SwitchViewChairID(i);

		//�û��˿�
		if (wViewChairID!=1)
		{
			WORD wIndex=(wViewChairID==0)?0:1;
			m_GameClientView.m_UserCard[wIndex].SetCardData((i==m_wBankerUser)?MAX_COUNT:(MAX_COUNT-1));
		}
		else
		{
			BYTE cbCardCount=(i==m_wBankerUser)?MAX_COUNT:(MAX_COUNT-1);
			m_GameClientView.m_HandCardControl.SetCardData(pGameStart->cbCardData,cbCardCount);
		}

		//�Թ۽���
		if (bPlayerMode==false)
		{
			WORD wMeChair=GetMeChairID();
			m_GameClientView.m_DiscardCard[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[wViewChairID][0].SetCardData(m_WeaveItemArray[wMeChair][0]);
			m_GameClientView.m_WeaveCard[wViewChairID][1].SetCardData(m_WeaveItemArray[wMeChair][1]);
			m_GameClientView.m_WeaveCard[wViewChairID][2].SetCardData(m_WeaveItemArray[wMeChair][2]);
			m_GameClientView.m_WeaveCard[wViewChairID][3].SetCardData(m_WeaveItemArray[wMeChair][3]);
			m_GameClientView.m_WeaveCard[wViewChairID][4].SetCardData(m_WeaveItemArray[wMeChair][4]);
			m_GameClientView.m_WeaveCard[wViewChairID][5].SetCardData(m_WeaveItemArray[wMeChair][5]);
			m_GameClientView.m_WeaveCard[wViewChairID][6].SetCardData(m_WeaveItemArray[wMeChair][6]);
		}
	}

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	//������ʾ
	if ((m_wCurrentUser==GetMeChairID())&&(IsLookonMode()==false)) 
	{
		m_bOutCard=true;
		m_GameClientView.SetStatusFlag(true,false);
	}

	//����ʱ��
	if (m_wCurrentUser!=INVALID_CHAIR) SetGameTimer(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);

	//��������
	if (bPlayerMode==true) ActiveGameFrame();
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	return true;
}

//�û�����
bool CGameClientDlg::OnSubUserTiCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_UserTiCard));
	if (wDataSize!=sizeof(CMD_S_UserTiCard)) return false;

	//��������
	CMD_S_UserTiCard * pUserTiCard=(CMD_S_UserTiCard *)pBuffer;

	//ִ�ж���
	ExecuteActionTiCard(pUserTiCard->wActionUser,pUserTiCard->cbActionCard,pUserTiCard->cbRemoveCount);

	return true;
}

//�û�����
bool CGameClientDlg::OnSubUserPaoCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_UserPaoCard));
	if (wDataSize!=sizeof(CMD_S_UserPaoCard)) return false;

	//��������
	CMD_S_UserPaoCard * pUserPaoCard=(CMD_S_UserPaoCard *)pBuffer;

	//ִ�ж���
	ExecuteActionPaoCard(pUserPaoCard->wActionUser,pUserPaoCard->cbActionCard,pUserPaoCard->cbRemoveCount);

	return true;
}

//�û�����
bool CGameClientDlg::OnSubUserWeiCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_UserWeiCard));
	if (wDataSize!=sizeof(CMD_S_UserWeiCard)) return false;

	//��������
	CMD_S_UserWeiCard * pUserWeiCard=(CMD_S_UserWeiCard *)pBuffer;

	//ִ�ж���
	ExecuteActionWeiCard(pUserWeiCard->wActionUser,pUserWeiCard->cbActionCard);

	return true;
}

//�û�����
bool CGameClientDlg::OnSubUserPengCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_UserPengCard));
	if (wDataSize!=sizeof(CMD_S_UserPengCard)) return false;

	//��������
	CMD_S_UserPengCard * pUserPengCard=(CMD_S_UserPengCard *)pBuffer;

	//ִ�ж���
	ExecuteActionPengCard(pUserPengCard->wActionUser,pUserPengCard->cbActionCard);

	return true;
}

//�û�����
bool CGameClientDlg::OnSubUserChiCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_UserChiCard));
	if (wDataSize!=sizeof(CMD_S_UserChiCard)) return false;

	//��������
	CMD_S_UserChiCard * pUserChiCard=(CMD_S_UserChiCard *)pBuffer;

	//ִ�ж���
	ExecuteActionChiCard(pUserChiCard->wActionUser,pUserChiCard->cbActionCard,pUserChiCard->cbResultCount,pUserChiCard->cbCardData);

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

	//ִ�ж���
	ExecuteActionOperateNotify(pOperateNotify->cbOperateCode,pOperateNotify->cbActionCard,pOperateNotify->wResumeUser);

	return true;
}

//������ʾ
bool CGameClientDlg::OnSubOutCardNotify(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_OutCardNotify));
	if (wDataSize!=sizeof(CMD_S_OutCardNotify)) return false;

	//��������
	CMD_S_OutCardNotify * pOutCardNotify=(CMD_S_OutCardNotify *)pBuffer;

	//ִ�ж���
	ExecuteActionOutCardNotify(pOutCardNotify->wCurrentUser,pOutCardNotify->bOutCard);

	return true;
}

//�û�����
bool CGameClientDlg::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_OutCard));
	if (wDataSize!=sizeof(CMD_S_OutCard)) return false;

	//��������
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;

	//ִ�ж���
	ExecuteActionOutCard(pOutCard->wOutCardUser,pOutCard->cbOutCardData);

	return true;
}

//��������
bool CGameClientDlg::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//��������
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	//ִ�ж���
	ExecuteActionSendCard(pSendCard->cbCardData,pSendCard->wAttachUser);

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

	//��Ϸ��ʱ��
	KillTimer(IDI_USER_ACTION);
	KillGameTimer(IDI_OUT_CARD);
	KillGameTimer(IDI_OPERATE_CARD);
	KillGameTimer(IDI_ASSISTANT_TIME);

	//��ֹ����
	HangUserAction();

	//���ñ���
	m_bOutCard=false;
	m_wBankerUser=INVALID_CHAIR;

	//���ý���
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.m_ChooseWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);

	//�����˿�
	BYTE cbPostion=0;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//���ý���
		WORD wViewChairID=SwitchViewChairID(i);
		if (wViewChairID==0) m_GameClientView.m_UserCard[0].SetCardData(&pGameEnd->bCardData[cbPostion],pGameEnd->bCardCount[i]);
		else if (wViewChairID==2) m_GameClientView.m_UserCard[1].SetCardData(&pGameEnd->bCardData[cbPostion],pGameEnd->bCardCount[i]);

		//��������
		cbPostion+=pGameEnd->bCardCount[i];
	}

	//������ʾ
	switch (pGameEnd->cbReason)
	{
	case GER_DISMISS:			//��Ϸ��ɢ
	case GER_USER_LEFT:			//�û�ǿ��
		{
			//��������
			tagScoreViewInfo ScoreViewInfo;
			ZeroMemory(&ScoreViewInfo,sizeof(ScoreViewInfo));

			//�û���Ϣ
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//��������
				const tagUserData * pUserData=GetUserData(i);

				//������Ϣ
				ScoreViewInfo.lGameScore[i]=pGameEnd->lGameScore[i];
				lstrcpyn(ScoreViewInfo.szUserName[i],pUserData->szName,CountArray(ScoreViewInfo.szUserName[i]));
			}

			//������Ϣ
			ScoreViewInfo.cbHuCard=0;
			ScoreViewInfo.lGameTax=pGameEnd->lGameTax;
			ScoreViewInfo.wWinUser=pGameEnd->wWinUser;
			ScoreViewInfo.wProvideUser=pGameEnd->wProvideUser;

			//��ʾ�ɼ�
			m_GameClientView.m_ScoreView.SetScoreViewInfo(ScoreViewInfo,true);

			break;
		}
	case GER_NORMAL:	//�������
		{
			if (pGameEnd->wWinUser!=INVALID_CHAIR)
			{
				//��������
				tagScoreViewInfo ScoreViewInfo;
				ZeroMemory(&ScoreViewInfo,sizeof(ScoreViewInfo));

				//������Ϣ
				BYTE cbPostion=0;
				for (WORD i=0;i<GAME_PLAYER;i++)
				{
					//���Ʒ���
					if (i==pGameEnd->wWinUser)
					{
						//ת������
						BYTE cbCardIndex[MAX_INDEX];
						m_GameLogic.SwitchToCardIndex(&pGameEnd->bCardData[cbPostion],pGameEnd->bCardCount[i],cbCardIndex);

						//������Ϣ
						m_GameLogic.GetHuCardInfo(cbCardIndex,pGameEnd->cbHuCard,m_bUserHuXiCount[i],ScoreViewInfo.HuCardInfo);

						//�������
						for (BYTE j=0;j<m_bWeaveItemCount[i];j++)
						{
							BYTE cbIndex=ScoreViewInfo.HuCardInfo.cbWeaveCount++;
							ScoreViewInfo.HuCardInfo.WeaveItemArray[cbIndex]=m_WeaveItemArray[i][j];
							ScoreViewInfo.HuCardInfo.cbHuXiCount+=m_GameLogic.GetWeaveHuXi(m_WeaveItemArray[i][j]);
						}

						break;
					}

					//��������
					cbPostion+=pGameEnd->bCardCount[i];
				}

				//�û���Ϣ
				for (WORD i=0;i<GAME_PLAYER;i++)
				{
					//��������
					const tagUserData * pUserData=GetUserData(i);

					//������Ϣ
					ScoreViewInfo.lGameScore[i]=pGameEnd->lGameScore[i];
					lstrcpyn(ScoreViewInfo.szUserName[i],pUserData->szName,CountArray(ScoreViewInfo.szUserName[i]));
				}

				//������Ϣ
				ScoreViewInfo.lGameTax=pGameEnd->lGameTax;
				ScoreViewInfo.cbHuCard=pGameEnd->cbHuCard;
				ScoreViewInfo.wWinUser=pGameEnd->wWinUser;
				ScoreViewInfo.wProvideUser=pGameEnd->wProvideUser;

				//��ʾ�ɼ�
				m_GameClientView.m_ScoreView.SetScoreViewInfo(ScoreViewInfo,true);
			}
			else m_GameClientView.SetHuangZhuang(true);

			break;
		}
	}

	//�˿˿ؼ�
	m_GameClientView.m_HandCardControl.SetPositively(false);
	if (IsLookonMode()==true) m_GameClientView.m_HandCardControl.SetDisplayItem(true);

	//��������
	WORD wMeChairID=GetMeChairID();
	LONG lMeScore=pGameEnd->lGameScore[GetMeChairID()];
	if (lMeScore>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
	else if (lMeScore<0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
	else PlayGameSound(GetModuleHandle(NULL),TEXT("GAME_END"));

	//���ý���
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		SetGameTimer(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
	}

	return true;
}

//ʱ����Ϣ
void CGameClientDlg::OnTimer(UINT nIDEvent)
{
	//������ʱ��
	if (nIDEvent==IDI_USER_ACTION)
	{
		//ɾ��ʱ��
		if (m_wActionCount==0)
		{
			KillTimer(IDI_USER_ACTION);
			return;
		}

		//��������
		switch (m_UserActionArray[0].cbActionKind)
		{
		case ACTION_TIP:				//��ʾ����
			{
				//��������
				tagActionTip * pActionTip=(tagActionTip *)(m_UserActionArray[0].cbActionBuffer);

				//���ý���
				WORD wViewChairID=SwitchViewChairID(pActionTip->wChairID);
				m_GameClientView.SetUserAction(wViewChairID,pActionTip->cbActionFlags);

				break;
			}
		case ACTION_TI_CARD:			//���ƶ���
			{
				//��������
				tagActionTiCard * pActionTiCard=(tagActionTiCard *)(m_UserActionArray[0].cbActionBuffer);

				//��������
				BYTE cbIndex=pActionTiCard->cbIndex;
				WORD wChairID=pActionTiCard->wChairID;
				WORD wViewChairID=SwitchViewChairID(wChairID);

				//���ñ���
				m_cbOutCardData=0;
				m_wOutCardUser=INVALID_CHAIR;

				//��Ͻ���
				m_GameClientView.m_WeaveCard[wViewChairID][cbIndex].SetCardData(m_WeaveItemArray[wChairID][cbIndex]);
				RemoveControlCard(wChairID,m_WeaveItemArray[wChairID][cbIndex].cbCardList,pActionTiCard->cbRemoveCount);

				//���ý���
				m_GameClientView.SetStatusFlag(false,false);
				m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
				m_GameClientView.SetUserAction(INVALID_CHAIR,ACK_NULL);
				m_GameClientView.SetUserHuXiCount(wViewChairID,pActionTiCard->bHuXiCount);

				break;
			}
		case ACTION_PAO_CARD:			//���ƶ���
			{
				//��������
				tagActionPaoCard * pActionPaoCard=(tagActionPaoCard *)(m_UserActionArray[0].cbActionBuffer);

				//��������
				BYTE cbIndex=pActionPaoCard->cbIndex;
				WORD wChairID=pActionPaoCard->wChairID;
				WORD wViewChairID=SwitchViewChairID(wChairID);

				//���ñ���
				m_cbOutCardData=0;
				m_wOutCardUser=INVALID_CHAIR;

				//��Ͻ���
				m_GameClientView.m_WeaveCard[wViewChairID][cbIndex].SetCardData(m_WeaveItemArray[wChairID][cbIndex]);
				RemoveControlCard(wChairID,m_WeaveItemArray[wChairID][cbIndex].cbCardList,pActionPaoCard->cbRemoveCount);

				//���ý���
				m_GameClientView.SetStatusFlag(false,false);
				m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
				m_GameClientView.SetUserAction(INVALID_CHAIR,ACK_NULL);
				m_GameClientView.SetUserHuXiCount(wViewChairID,pActionPaoCard->bHuXiCount);

				break;
			}
		case ACTION_WEI_CARD:			//���ƶ���
			{
				//��������
				tagActionWeiCard * pActionWeiCard=(tagActionWeiCard *)(m_UserActionArray[0].cbActionBuffer);

				//��������
				BYTE cbIndex=pActionWeiCard->cbIndex;
				WORD wChairID=pActionWeiCard->wChairID;
				WORD wViewChairID=SwitchViewChairID(wChairID);

				//��Ͻ���
				if(wChairID==GetMeChairID())
					m_GameClientView.m_WeaveCard[wViewChairID][cbIndex].SetDisplayStyle(true);
				else
					m_GameClientView.m_WeaveCard[wViewChairID][cbIndex].SetDisplayStyle(false);
				RemoveControlCard(wChairID,m_WeaveItemArray[wChairID][cbIndex].cbCardList,2);
				m_GameClientView.m_WeaveCard[wViewChairID][cbIndex].SetCardData(m_WeaveItemArray[wChairID][cbIndex]);

				//���Ӻ�Ϣ
				BYTE cbUserHuXi=m_GameLogic.GetWeaveHuXi(m_WeaveItemArray[wChairID][cbIndex]);
				m_GameClientView.SetUserHuXiCount(wViewChairID,m_GameClientView.m_bUserHuXiCount[wViewChairID]+cbUserHuXi);

				//������Ϣ
				m_cbOutCardData=0;
				m_wOutCardUser=INVALID_CHAIR;

				//��������
				m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
				m_GameClientView.SetUserAction(INVALID_CHAIR,ACK_NULL);
				m_GameClientView.SetUserHuXiCount(wViewChairID,pActionWeiCard->bHuXiCount);

				break;
			}
		case ACTION_PENG_CARD:			//���ƶ���
			{
				//��������
				tagActionPengCard * pActionPengCard=(tagActionPengCard *)(m_UserActionArray[0].cbActionBuffer);

				//��������
				BYTE cbIndex=pActionPengCard->cbIndex;
				WORD wChairID=pActionPengCard->wChairID;
				WORD wViewChairID=SwitchViewChairID(wChairID);

				//������Ϣ
				m_cbOutCardData=0;
				m_wOutCardUser=INVALID_CHAIR;

				//��Ͻ���
				RemoveControlCard(wChairID,m_WeaveItemArray[wChairID][cbIndex].cbCardList,2);
				m_GameClientView.m_WeaveCard[wViewChairID][cbIndex].SetCardData(m_WeaveItemArray[wChairID][cbIndex]);

				//��������
				m_GameClientView.SetStatusFlag(false,false);
				m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
				m_GameClientView.SetUserAction(INVALID_CHAIR,ACK_NULL);
				m_GameClientView.SetUserHuXiCount(wViewChairID,pActionPengCard->bHuXiCount);

				break;
			}
		case ACTION_CHI_CARD:			//�û�����
			{
				//��������
				tagActionChiCard * pActionChiCard=(tagActionChiCard *)(m_UserActionArray[0].cbActionBuffer);

				//��������
				BYTE cbIndex=pActionChiCard->cbIndex;
				WORD wChairID=pActionChiCard->wChairID;
				WORD wViewChairID=SwitchViewChairID(wChairID);

				//������Ϣ
				m_cbOutCardData=0;
				m_wOutCardUser=INVALID_CHAIR;

				//��Ͻ���
				BYTE cbUserHuXi=0;
				for (BYTE i=0;i<pActionChiCard->cbResultCount;i++)
				{
					cbUserHuXi+=m_GameLogic.GetWeaveHuXi(m_WeaveItemArray[wChairID][cbIndex+i]);
					m_GameClientView.m_WeaveCard[wViewChairID][cbIndex+i].SetCardData(m_WeaveItemArray[wChairID][cbIndex+i]);
				}

				//ɾ��׼��
				BYTE cbRemoveCount=0;
				BYTE cbRemoveCard[MAX_COUNT];
				BYTE cbDebarCard=pActionChiCard->cbActionCard;
				for (BYTE j=0;j<pActionChiCard->cbResultCount;j++)
				{
					for (BYTE l=0;l<3;l++)
					{
						BYTE cbCurrentCard=pActionChiCard->cbCardData[j][l];
						if (cbCurrentCard==cbDebarCard) cbDebarCard=0;
						else cbRemoveCard[cbRemoveCount++]=cbCurrentCard;
					}
				}

				//ɾ���˿�
				ASSERT(cbRemoveCount==(pActionChiCard->cbResultCount*3-1));
				RemoveControlCard(wChairID,cbRemoveCard,pActionChiCard->cbResultCount*3-1);

				//��������
				m_GameClientView.SetStatusFlag(false,false);
				m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
				m_GameClientView.SetUserAction(INVALID_CHAIR,ACK_NULL);
				m_GameClientView.SetUserHuXiCount(wViewChairID,pActionChiCard->bHuXiCount);

				break;
			}
		case ACTION_OUT_CARD:			//�û�����
			{
				//��������
				tagActionOutCard * pActionOutCard=(tagActionOutCard *)(m_UserActionArray[0].cbActionBuffer);

				//���ñ���
				m_bOutCard=false;
				m_wCurrentUser=INVALID_CHAIR;

				//�˿��ռ�
				if (m_wOutCardUser!=INVALID_CHAIR)
				{
					WORD wViewChairID=SwitchViewChairID(m_wOutCardUser);
					m_GameClientView.m_DiscardCard[wViewChairID].AddCardItem(m_cbOutCardData);
				}

				//������Ϣ
				m_wOutCardUser=pActionOutCard->wOutCardUser;
				m_cbOutCardData=pActionOutCard->cbOutCardData;

				//��������
				if ((GetMeChairID()!=m_wOutCardUser)||(IsLookonMode()==true))
				{
					//�����˿�
					BYTE cbRemoveCard[]={m_cbOutCardData};
					RemoveControlCard(pActionOutCard->wOutCardUser,cbRemoveCard,CountArray(cbRemoveCard));

					//���ý���
					KillGameTimer(IDI_OUT_CARD);
					m_GameClientView.SetUserAction(INVALID_CHAIR,0);
					m_GameClientView.SetOutCardInfo(SwitchViewChairID(m_wOutCardUser),m_cbOutCardData);

					//��������
					PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
				}

				break;
			}
		case ACTION_SEND_CARD:			//���ƴ���
			{
				//��������
				tagActionSendCard * pActionSendCard=(tagActionSendCard *)(m_UserActionArray[0].cbActionBuffer);

				//�˿��ռ�
				if (m_wOutCardUser!=INVALID_CHAIR)
				{
					WORD wViewChairID=SwitchViewChairID(m_wOutCardUser);
					m_GameClientView.m_DiscardCard[wViewChairID].AddCardItem(m_cbOutCardData);
				}

				//������Ϣ
				m_wOutCardUser=pActionSendCard->wAttachUser;
				m_cbOutCardData=pActionSendCard->cbSendCardData;

				//���ƽ���
				WORD wViewChairID=SwitchViewChairID(m_wOutCardUser);
				m_GameClientView.SetOutCardInfo(wViewChairID,m_cbOutCardData);
				m_GameClientView.SetLeftCardCount(m_GameClientView.m_bLeftCardCount-1);

				//��������
				PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

				break;
			}
		case ACTION_OPERATE_NOTIFY:		//������ʾ
			{
				//��������
				tagActionOperateNotify * pActionOperateNotify=(tagActionOperateNotify *)(m_UserActionArray[0].cbActionBuffer);

				//���ñ���
				m_bOutCard=false;
				m_wCurrentUser=INVALID_CHAIR;
				m_wResumeUser=pActionOperateNotify->wResumeUser;

				//�û�����
				if ((IsLookonMode()==false)&&(pActionOperateNotify->cbOperateCode!=ACK_NULL))
				{
					//��ȡ����
					BYTE cbOperateCode=pActionOperateNotify->cbOperateCode;
					BYTE cbCurrentCard=pActionOperateNotify->cbCurrentCard;

					//���ý���
					ActiveGameFrame();
					SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
					m_GameClientView.m_ControlWnd.SetControlInfo(cbCurrentCard,cbOperateCode);
				}
				else SetGameTimer(m_wResumeUser,IDI_ASSISTANT_TIME,TIME_ASSISTANT_TIME);

				break;
			}
		case ACTION_OUT_CARD_NOTIFY:	//������ʾ
			{
				//��������
				tagActionOutCardNotify * pActionOutCardNotify=(tagActionOutCardNotify *)(m_UserActionArray[0].cbActionBuffer);

				//���ñ���
				m_bOutCard=false;
				m_wCurrentUser=pActionOutCardNotify->wCurrentUser;

				//���ý���
				if ((pActionOutCardNotify->bOutCard==TRUE)&&(m_wCurrentUser==GetMeChairID())&&(IsLookonMode()==false))
				{
					m_bOutCard=true;
					ActiveGameFrame();
					m_GameClientView.SetStatusFlag(true,false);
				}

				//����ʱ��
				if (pActionOutCardNotify->bOutCard==FALSE)
				{
					if ((m_wCurrentUser==GetMeChairID())&&(IsLookonMode()==false)) SendData(SUB_C_CONTINUE_CARD);
				}
				else SetGameTimer(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);

				break;
			}
		}

		//ɾ������
		m_wActionCount--;
		if (m_wActionCount>0)
		{
			LONG lMoveSize=m_wActionCount*sizeof(m_UserActionArray[0]);
			MoveMemory(&m_UserActionArray[0],&m_UserActionArray[1],lMoveSize);
		}

		return;
	}

	__super::OnTimer(nIDEvent);
}

//��ʼ��ť
LRESULT CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	//��������
	m_bOutCard=false;
	m_wCurrentUser=INVALID_CHAIR;

	//��Ϸ����
	ZeroMemory(m_bUserCardCount,sizeof(m_bUserCardCount));
	ZeroMemory(m_bUserHuXiCount,sizeof(m_bUserHuXiCount));

	//����˿�
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_bWeaveItemCount,sizeof(m_bWeaveItemCount));

	//��������
	m_wActionCount=0;
	ZeroMemory(m_UserActionArray,sizeof(m_UserActionArray));

	//�˿˱���
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

	//���ؿؼ�
	m_GameClientView.m_btStart.ShowWindow(FALSE);
	m_GameClientView.m_ScoreView.ShowWindow(FALSE);
	m_GameClientView.m_ChooseWnd.ShowWindow(FALSE);
	m_GameClientView.m_ControlWnd.ShowWindow(FALSE);

	//���ý���
	m_GameClientView.SetHuangZhuang(false);
	m_GameClientView.SetLeftCardCount(0xFF);
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
	m_GameClientView.SetUserHuXiCount(INVALID_CHAIR,0xFF);
	m_GameClientView.SetUserAction(INVALID_CHAIR,ACK_NULL);

	//�����˿�
	m_GameClientView.m_UserCard[0].SetCardData(0);
	m_GameClientView.m_UserCard[1].SetCardData(0);
	m_GameClientView.m_HandCardControl.SetCardData(NULL,0);

	//�����˿�
	for (int i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_DiscardCard[i].SetCardData(NULL,0);
		for (int j=0;j<7;j++) m_GameClientView.m_WeaveCard[i][j].SetCardData(m_WeaveItemArray[i][j]);
	}

	//���½���
	KillGameTimer(IDI_START_GAME);
	m_GameClientView.UpdateGameView(NULL);

	//������Ϣ
	SendUserReady(NULL,0);

	return 0;
}

//���Ʋ���
LRESULT CGameClientDlg::OnOutCard(WPARAM wParam, LPARAM lParam)
{
	//�����ж�
	if ((IsLookonMode()==true)||(m_bOutCard==false)||(m_wCurrentUser!=GetMeChairID())) return 0;

	//��������
	BYTE cbOutCard=(BYTE)wParam;
	WORD wMeChairID=GetMeChairID();

	//����Ч��
	BYTE cbCardIndex=m_GameLogic.SwitchToCardIndex(cbOutCard);
	if (m_cbCardIndex[cbCardIndex]==3) return 0;

	//���ñ���
	m_bOutCard=false;
	m_wCurrentUser=INVALID_CHAIR;

	//ɾ���˿�
	m_bUserCardCount[wMeChairID]--;
	m_GameLogic.RemoveCard(m_cbCardIndex,cbOutCard);

	//���ƽ���
	BYTE cbRemoveCard[]={cbOutCard};
	RemoveControlCard(wMeChairID,cbRemoveCard,CountArray(cbRemoveCard));
	m_GameClientView.SetOutCardInfo(SwitchViewChairID(wMeChairID),cbOutCard);

	//���ý���
	KillGameTimer(IDI_OUT_CARD);
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

	//��������
	CMD_C_OutCard OutCard;
	OutCard.cbCardData=cbOutCard;
	SendData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard));

	return 0;
}

//ѡ�Ʋ���
LRESULT CGameClientDlg::OnChooseCard(WPARAM wParam, LPARAM lParam)
{
	//��������
	BYTE cbChiKind=(BYTE)lParam;

	//��������
	m_GameClientView.SetStatusFlag(false,true);
	m_GameClientView.m_ChooseWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	KillGameTimer((GetMeChairID()==m_wCurrentUser)?IDI_OUT_CARD:IDI_OPERATE_CARD);

	//��������
	CMD_C_OperateCard OperateCard;
	OperateCard.cbChiKind=cbChiKind;
	OperateCard.cbOperateCode=ACK_CHI;
	SendData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	//���ö�ʱ��
	SetGameTimer(m_wResumeUser,IDI_ASSISTANT_TIME,TIME_ASSISTANT_TIME);

	return 0;
}

//�˿˲���
LRESULT CGameClientDlg::OnCardOperate(WPARAM wParam, LPARAM lParam)
{
	//��������
	BYTE cbOperateCode=(BYTE)(wParam);

	//״̬�ж�
	if ((m_wCurrentUser==GetMeChairID())&&(cbOperateCode==ACK_NULL))
	{
		//��������
		m_GameClientView.m_ChooseWnd.ShowWindow(SW_HIDE);
		m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);

		return 0;
	};

	//���Ʋ���
	if (cbOperateCode==ACK_CHI)
	{
		//��ȡ����
		tagChiCardInfo ChiCardInfo[6];
		BYTE cbWeaveCount=m_GameLogic.GetActionChiCard(m_cbCardIndex,m_cbOutCardData,ChiCardInfo);

		//���ÿ���
		m_GameClientView.m_ChooseWnd.SetChooseWeave(ChiCardInfo,cbWeaveCount);

		return 0;
	}

	//��������
	m_GameClientView.m_ChooseWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	if (cbOperateCode!=ACK_NULL) m_GameClientView.SetStatusFlag(false,true);
	KillGameTimer((GetMeChairID()==m_wCurrentUser)?IDI_OUT_CARD:IDI_OPERATE_CARD);

	//��������
	CMD_C_OperateCard OperateCard;
	OperateCard.cbChiKind=CK_NULL;
	OperateCard.cbOperateCode=cbOperateCode;
	SendData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	//���ö�ʱ��
	SetGameTimer(m_wResumeUser,IDI_ASSISTANT_TIME,TIME_ASSISTANT_TIME);

	return 0;
}

//////////////////////////////////////////////////////////////////////////

