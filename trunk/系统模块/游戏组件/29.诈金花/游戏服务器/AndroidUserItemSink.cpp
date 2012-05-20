#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//����ʱ��
#define TIME_LESS					1									//����ʱ��

//��Ϸʱ��
#define IDI_START_GAME				(IDI_ANDROID_ITEM_SINK+0)			//��ʼ��ʱ��
#define IDI_USER_ADD_SCORE			(IDI_ANDROID_ITEM_SINK+1)			//��ע��ʱ��
#define IDI_USER_COMPARE_CARD		(IDI_ANDROID_ITEM_SINK+2)			//ѡ�����û���ʱ��

//ʱ���ʶ
#define TIME_START_GAME				3									//��ʼ��ʱ��
#define TIME_USER_ADD_SCORE			3									//��ע��ʱ��
#define	TIME_USER_COMPARE_CARD		3									//���ƶ�ʱ��

//////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//�ӿڱ���
	m_pIAndroidUserItem=NULL;

	return;
}

//��������
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//�ӿڲ�ѯ
void * __cdecl CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}

//��ʼ�ӿ�
bool __cdecl CAndroidUserItemSink::InitUserItemSink(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pIAndroidUserItem=GET_OBJECTPTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;

	return true;
}

//���ýӿ�
bool __cdecl CAndroidUserItemSink::RepositUserItemSink()
{
	return true;
}

//ʱ����Ϣ
bool __cdecl CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_START_GAME:		//��ʼ��ʱ��
		{
			//����׼��
			m_pIAndroidUserItem->SendUserReady(NULL,0);

			return true;
		}
	case IDI_USER_ADD_SCORE:	//��ע��ʱ��
		{
			//����
			//��ע
			//����
			//����
			//��ע
			//����

			//һΪ��ע
			WORD wTemp=rand()%2;

			//��ȡ����
			LONG lCurrentScore=(wTemp==0)?(��ע):(m_lCellScore*m_lCurrentTimes);
			WORD wMeChairID=GetMeChairID();

			//��ע�ӱ�
			if(m_bMingZhu)lCurrentScore*=2;

			//������Ϣ
			CMD_C_AddScore AddScore;
			AddScore.wState=0;
			AddScore.lScore=lCurrentScore;
			SendData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

			//�Զ�����
			SendData(SUB_C_GIVE_UP,NULL,0);

			////��ע��Ϣ
			//m_lCellScore=pStatusPlay->lCellScore;
			//m_lCurrentTimes=pStatusPlay->lCurrentTimes;
			//m_lUserMaxScore=pStatusPlay->lUserMaxScore;

			////���ñ���
			//m_wCurrentUser=pStatusPlay->wCurrentUser;
			//m_bMingZhu=pStatusPlay->bMingZhu[wMeChairID];
			//m_lTableScore=pStatusPlay->lTableScore[wMeChairID];

			return true;
		}
	case IDI_USER_COMPARE_CARD:	//ѡ��ʱ��
		{
			//�������
			CMD_C_CompareCard CompareCard;
			ZeroMemory(&CompareCard,sizeof(CompareCard));

			//�����ϼ�
			WORD wMeChairID=GetMeChairID();
			for (LONG i=wMeChairID-1;;i--)
			{
				if(i==-1)i=GAME_PLAYER-1;
				if(m_cbPlayStatus[i]==TRUE)
				{
					CompareCard.wCompareUser=(WORD)i;
					break;
				}
			}

			//������Ϣ
			m_pIAndroidUserItem->SendSocketData(SUB_C_COMPARE_CARD,&CompareCard,sizeof(CompareCard));

			return true;
		}
	}

	return false;
}

//��Ϸ��Ϣ
bool __cdecl CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			//��Ϣ����
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_ADD_SCORE:		//�û���ע
		{
			//��Ϣ����
			return OnSubAddScore(pData,wDataSize);
		}
	case SUB_S_LOOK_CARD:		//������Ϣ
		{
			//��Ϣ����
			return OnSubLookCard(pData,wDataSize);
		}
	case SUB_S_COMPARE_CARD:	//������Ϣ
		{
			//��Ϣ����
			return OnSubCompareCard(pData,wDataSize);
		}
	case SUB_S_OPEN_CARD:		//������Ϣ
		{
			//��Ϣ����
			return OnSubOpenCard(pData,wDataSize);
		}
	case SUB_S_GIVE_UP:			//�û�����
		{
			//��Ϣ����
			return OnSubGiveUp(pData,wDataSize);
		}
	case SUB_S_PLAYER_EXIT:		//�û�ǿ��
		{
			//��Ϣ����
			return OnSubPlayerExit(pData,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			//��Ϣ����
			return OnSubGameEnd(pData,wDataSize);
		}
	case SUB_S_WAIT_COMPARE:	//�ȴ�����
		{
			//��Ϣ����
			if (wDataSize!=sizeof(CMD_S_WaitCompare)) return false;
			CMD_S_WaitCompare * pWaitCompare=(CMD_S_WaitCompare *)pData;	
			ASSERT(pWaitCompare->wCompareUser==m_wCurrentUser);

			//���¶�ʱ
			if(GetMeChairID()==m_wCurrentUser)
			{
				//����ʱ��
				UINT nElapse=rand()%(TIME_USER_COMPARE_CARD+3)+TIME_LESS*3;
				m_pIAndroidUserItem->SetGameTimer(IDI_USER_COMPARE_CARD,nElapse);
			}

			return true;
		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//��Ϸ��Ϣ
bool __cdecl CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	return true;
}

//������Ϣ
bool __cdecl CAndroidUserItemSink::OnEventGameScene(BYTE cbGameStatus, bool bLookonOther, void * pBuffer, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GS_FREE:		//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��ʼʱ��
			UINT nElapse=rand()%(TIME_USER_START_GAME+3)+TIME_LESS*3;
			m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);

			return true;
		}
	case GS_PLAYING:	//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;
			WORD wMeChairID=GetMeChairID();

			//��ע��Ϣ
			m_lCellScore=pStatusPlay->lCellScore;
			m_lCurrentTimes=pStatusPlay->lCurrentTimes;
			m_lUserMaxScore=pStatusPlay->lUserMaxScore;

			//���ñ���
			m_wCurrentUser=pStatusPlay->wCurrentUser;
			m_bMingZhu=pStatusPlay->bMingZhu[wMeChairID];
			CopyMemory(m_lTableScore,pStatusPlay->lTableScore,sizeof(pStatusPlay->lTableScore));
			CopyMemory(m_cbPlayStatus,pStatusPlay->cbPlayStatus,sizeof(pStatusPlay->cbPlayStatus));

			if((!IsLookonMode()) && wMeChairID==m_wCurrentUser)
			{
				//�жϿؼ�
				if(!(pStatusPlay->bCompareState))
				{
					//����ʱ��
					UINT nElapse=rand()%(TIME_USER_ADD_SCORE+3)+TIME_LESS*3;
					m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE,nElapse);
				}
				else 
				{
					//�ȴ�����
					SendData(SUB_C_WAIT_COMPARE,NULL,0);

					//����ʱ��
					UINT nElapse=rand()%(TIME_USER_COMPARE_CARD+3)+TIME_LESS*3;
					m_pIAndroidUserItem->SetGameTimer(IDI_USER_COMPARE_CARD,nElapse);
				}
			}
			return true;
		}
	}

	ASSERT(FALSE);

	return false;
}

//�û�����
void __cdecl CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û��뿪
void __cdecl CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�����
void __cdecl CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�״̬
void __cdecl CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û���λ
void __cdecl CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//������Ϣ
	m_lCellScore=pGameStart->lCellScore;
	m_lCurrentTimes=pGameStart->lCurrentTimes;
	m_wCurrentUser=pGameStart->wCurrentUser;
	m_lUserMaxScore=pGameStart->lUserMaxScore;

	//���ñ���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		if (m_pIAndroidUserItem->GetTableUserItem(i)!=NULL) 
		{
			//������Ϣ
			m_lTableScore[i]=m_lCellScore;
			m_cbPlayStatus[i]=TRUE;
		}
		else 
		{
			m_lTableScore[i]=0;
			m_cbPlayStatus[i]=0;
		}
	}

	//����ʱ��
	if(m_wCurrentUser==GetMeChairID())
	{
		//��עʱ��
		UINT nElapse=rand()%(TIME_USER_ADD_SCORE+3)+TIME_LESS*8;
		m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE,nElapse);
	}

	return true;
}

//�û�����
bool CAndroidUserItemSink::OnSubGiveUp(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_GiveUp)) return false;
	CMD_S_GiveUp * pGiveUp=(CMD_S_GiveUp *)pBuffer;

	//���ñ���
	m_cbPlayStatus[pGiveUp->wGiveUpUser]=FALSE;

	return true;
}

//�û���ע
bool CAndroidUserItemSink::OnSubAddScore(const void * pBuffer, WORD wDataSize)
{
	//��������
	WORD wMeChairID=GetMeChairID();
	CMD_S_AddScore * pAddScore=(CMD_S_AddScore *)pBuffer;

	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_AddScore));
	if (wDataSize!=sizeof(CMD_S_AddScore)) return false;
	ASSERT(pAddScore->wCurrentUser < GAME_PLAYER);
	ASSERT(pAddScore->lCurrentTimes<=m_lMaxCellScore/m_lCellScore);

	//��ǰ�û�
	m_lCurrentTimes = pAddScore->lCurrentTimes;
	m_wCurrentUser=pAddScore->wCurrentUser;
	m_lTableScore[pAddScore->wAddScoreUser]+=pAddScore->lAddScoreCount;

	//����ʱ��
	if(pAddScore->wCompareState==FALSE && m_wCurrentUser==wMeChairID)
	{
		//��עʱ��
		UINT nElapse=rand()%(TIME_USER_ADD_SCORE+3)+TIME_LESS*3;
		m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE,nElapse);
	}

	return true;
}

//�û�����
bool CAndroidUserItemSink::OnSubLookCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_LookCard)) return false;

	//���¶�ʱ
	if(m_wCurrentUser==GetMeChairID())
	{
		//��עʱ��
		UINT nElapse=rand()%(TIME_USER_ADD_SCORE+3)+TIME_LESS*3;
		m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE,nElapse);
	}

	return true;
}

//�û�����
bool CAndroidUserItemSink::OnSubCompareCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_CompareCard)) return false;
	CMD_S_CompareCard * pCompareCard=(CMD_S_CompareCard *)pBuffer;

	//���ñ���
	m_cbPlayStatus[pCompareCard->wLostUser]=FALSE;

	//��ǰ�û�
	m_wCurrentUser=pCompareCard->wCurrentUser;

	//�������
	BYTE bCount=0;
	for (WORD i=0;i<GAME_PLAYER;i++)if(m_cbPlayStatus[i]==TRUE)bCount++;

	//�жϽ���
	if(bCount>1)
	{
		//�ؼ���Ϣ
		if(!IsLookonMode() && wMeChairID==m_wCurrentUser)
		{
			//��עʱ��
			UINT nElapse=rand()%(TIME_USER_ADD_SCORE+3)+TIME_LESS*3;
			m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE,nElapse);
		}
	}

	return true;
}

//�û�����
bool CAndroidUserItemSink::OnSubOpenCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_OpenCard)) return false;

	return true;
}

//�û�ǿ��
bool CAndroidUserItemSink::OnSubPlayerExit(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_PlayerExit)) return false;
	CMD_S_PlayerExit * pPlayerExit=(CMD_S_PlayerExit *)pBuffer;

	//��Ϸ��Ϣ
	ASSERT(m_cbPlayStatus[pPlayerExit->wPlayerID]==TRUE);
	m_cbPlayStatus[pPlayerExit->wPlayerID]=FALSE;

	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//��ʼ��ť
	if (IsLookonMode()==false)
	{
		//��עʱ��
		UINT nElapse=rand()%(TIME_START_GAME+3)+TIME_LESS*3;
		m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
