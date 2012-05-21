#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//����ʱ��
#define TIME_LESS					1									//����ʱ��

//��Ϸʱ��
#define TIME_START_GAME				3									//��ʼʱ��

//��Ϸʱ��
#define IDI_BANKER_START			(IDI_ANDROID_ITEM_SINK+0)			//��ʼʱ��
#define IDI_PLAYER_START			(IDI_ANDROID_ITEM_SINK+1)			//��עʱ��
#define IDI_CHANGE_BANKER			(IDI_ANDROID_ITEM_SINK+2)			//��ׯʱ��
#define IDI_CONTINUE				(IDI_ANDROID_ITEM_SINK+3)			//����ʱ��

//////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//��ע��Ϣ
	m_lMaxScore=0L;
	m_lLessScore=0L;

	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;

	//��������
	m_bGameScene=false;

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
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;

	return true;
}

//���ýӿ�
bool __cdecl CAndroidUserItemSink::RepositUserItemSink()
{
	//��ע��Ϣ
	m_lMaxScore=0L;
	m_lLessScore=0L;

	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;

	//��������
	m_bGameScene=false;

	return true;
}

//ʱ����Ϣ
bool __cdecl CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_BANKER_START:		//ׯ�ҿ�ʼ
		{
			ASSERT(m_wBankerUser==m_pIAndroidUserItem->GetChairID());
			if (m_wBankerUser!=m_pIAndroidUserItem->GetChairID()) return true;

			//����׼��
			m_pIAndroidUserItem->SendUserReady(NULL,0);

			return true;
		}
	case IDI_PLAYER_START:		//�м���ע
		{
			ASSERT(m_wBankerUser!=m_pIAndroidUserItem->GetChairID());
			if (m_wBankerUser==m_pIAndroidUserItem->GetChairID()) return true;

			//��������
			CMD_C_UserChipin UserChipin;
			ZeroMemory(&UserChipin,sizeof(UserChipin));

			//�������
			LONG lMaxTime = m_lMaxScore/m_lLessScore;
			UserChipin.lScore=(rand()%lMaxTime+1)*m_lLessScore;

			//������Ϣ
			m_pIAndroidUserItem->SendSocketData(SUB_C_USER_CHIPIN,&UserChipin,sizeof(UserChipin));

			return true;
		}
	case IDI_CHANGE_BANKER:		//��ׯʱ��
		{
			ASSERT(m_wBankerUser!=m_pIAndroidUserItem->GetChairID());
			if (m_wBankerUser==m_pIAndroidUserItem->GetChairID()) return true;

			//�������
			CMD_C_ChangeInfo ChangeInfo;
			ZeroMemory(&ChangeInfo,sizeof(ChangeInfo));

			//���ñ���
			ChangeInfo.cbChange=TRUE;

			//������Ϣ
			m_pIAndroidUserItem->SendSocketData(SUB_C_CHANGE_INFO,&ChangeInfo,sizeof(ChangeInfo));

			return true;
		}
	case IDI_CONTINUE:			//������Ϸ
		{
			//������Ϣ
			m_pIAndroidUserItem->SendSocketData(SUB_C_CONTINUE_GAME);

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
	case SUB_S_TIMER_INFO:		//ʱ����Ϣ
		{
			if (m_bGameScene==false) return true;
			return OnSubTimeInfo(pData,wDataSize);
		}
	case SUB_S_SCORE_INFO:		//��ע��Ϣ
		{
			if (m_bGameScene==false) return true;
			return OnSubScoreInfo(pData,wDataSize);
		}
	case SUB_S_BANKER_USER:		//ׯ����Ϣ
		{
			if (m_bGameScene==false) return true;
			return OnSubBankerUser(pData,wDataSize);
		}
	case SUB_S_CHANGE_INFO:		//��ׯ��Ϣ
		{
			if (m_bGameScene==false) return true;
			return OnSubChangeInfo(pData,wDataSize);
		}
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			if (m_bGameScene==false) return true;
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_USER_CHIPIN:		//�û���ע
		{
			if (m_bGameScene==false) return true;
			return OnSubUserChipin(pData,wDataSize);
		}
	case SUB_S_SEND_CARD:		//�����˿�
		{
			if (m_bGameScene==false) return true;
			return OnSubSendCard(pData,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			if (m_bGameScene==false) return true;
			return OnSubGameEnd(pData,wDataSize);
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
bool __cdecl CAndroidUserItemSink::OnEventGameScene(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GS_FREE:		//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��������
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			//��������
			m_bGameScene=true;

			//���ñ���
			m_lMaxScore=pStatusFree->lMaxScore;
			m_lLessScore=pStatusFree->lLessScore;
			m_wBankerUser=pStatusFree->wBankerUser;

			//��ʼ����
			if (pStatusFree->wLeaveTime>0 && m_pIAndroidUserItem->GetChairID()==m_wBankerUser)
			{
				UINT nElapse=rand()%(TIME_START_GAME+3)+TIME_LESS*5;
				//nElapse=9;
				m_pIAndroidUserItem->SetGameTimer(IDI_BANKER_START,nElapse);
			}

			return true;
		}
	case GS_TK_PLAYING:	//��Ϸ״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//��������
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			//��������
			m_bGameScene=true;

			//���ñ���
			m_lMaxScore=pStatusPlay->lMaxScore;
			m_lLessScore=pStatusPlay->lLessScore;
			m_wBankerUser=pStatusPlay->wBankerUser;

			//�м�����
			if (m_pIAndroidUserItem->GetChairID()!=m_wBankerUser)
			{
				//����״̬
				if ((pStatusPlay->cbSendCard==FALSE)&&(pStatusPlay->lUserChipIn[m_pIAndroidUserItem->GetChairID()]==0))
				{
					//������עʱ��
					UINT nElapse=rand()%(TIME_START_GAME+2)+TIME_LESS*2;
					m_pIAndroidUserItem->SetGameTimer(IDI_PLAYER_START,nElapse);
				}
			}

			//���˷��ƶ���,ֱ�ӷ���"�������"
			if (pStatusPlay->cbSendCard==TRUE && pStatusPlay->cbNeedFinish==TRUE)
			{
				m_pIAndroidUserItem->SendSocketData(SUB_C_SEND_FINISH);
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

//ʱ����Ϣ
bool CAndroidUserItemSink::OnSubTimeInfo(const void * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_TimerInfo));
	if (wDataSize!=sizeof(CMD_S_TimerInfo)) return false;

	//��Ϣ����
	CMD_S_TimerInfo * pTimerInfo=(CMD_S_TimerInfo *)pData;

	//����ʱ��
	if (pTimerInfo->wLeaveTime!=0L && m_pIAndroidUserItem->GetChairID()==m_wBankerUser)
	{
		//����ʱ��
		UINT nElapse=rand()%(TIME_START_GAME+3)+TIME_LESS*5;
		//nElapse=9;
		m_pIAndroidUserItem->SetGameTimer(IDI_BANKER_START,nElapse);
	}
	else
	{
		//ɾ��ʱ��
		m_pIAndroidUserItem->KillGameTimer(IDI_BANKER_START);
	}

	return true;
}

//��ע��Ϣ
bool CAndroidUserItemSink::OnSubScoreInfo(const void * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ScoreInfo));
	if (wDataSize!=sizeof(CMD_S_ScoreInfo)) return false;

	//��Ϣ����
	CMD_S_ScoreInfo * pScoreInfo=(CMD_S_ScoreInfo *)pData;

	//���ñ���
	m_lMaxScore=pScoreInfo->lMaxScore;
	m_lLessScore=pScoreInfo->lLessScore;

	return true;
}

//ׯ����Ϣ
bool CAndroidUserItemSink::OnSubBankerUser(const void * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_BankerUser));
	if (wDataSize!=sizeof(CMD_S_BankerUser)) return false;

	//��Ϣ����
	CMD_S_BankerUser * pBankerUser=(CMD_S_BankerUser *)pData;

	//���ñ���
	m_wBankerUser=pBankerUser->wBankerUser;

	//ɾ��ʱ��
	m_pIAndroidUserItem->KillGameTimer(IDI_BANKER_START);

	//��ׯ����  ǿ����ׯ
	if (m_pIAndroidUserItem->GetChairID()==m_wBankerUser)
	{
		//ɾ��ʱ��
		m_pIAndroidUserItem->KillGameTimer(IDI_CHANGE_BANKER);
	}

	return true;
}

//��ׯ��Ϣ
bool CAndroidUserItemSink::OnSubChangeInfo(const void * pData, WORD wDataSize)
{
	//������ׯʱ��
	UINT nElapse=rand()%(TIME_START_GAME+5)+TIME_LESS*2;
	m_pIAndroidUserItem->SetGameTimer(IDI_CHANGE_BANKER,nElapse);

	return true;
}

//��ע��Ϣ
bool CAndroidUserItemSink::OnSubUserChipin(const void * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_UserChipin));
	if (wDataSize!=sizeof(CMD_S_UserChipin)) return false;

	return true;
}

//�����˿�
bool CAndroidUserItemSink::OnSubSendCard(const void * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//���˷��ƶ���,ֱ�ӷ���"�������"
	m_pIAndroidUserItem->SendSocketData(SUB_C_SEND_FINISH);

	//ɾ��ʱ��
	//m_pIAndroidUserItem->KillGameTimer(0);

	return true;
}

//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(const void * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	CMD_S_GameStart *pGameStart=(CMD_S_GameStart *)pData;

	//���ñ���
	m_lMaxScore=pGameStart->lMaxScore;
	m_lLessScore=pGameStart->lLessScore;

	m_pIAndroidUserItem->KillGameTimer(IDI_BANKER_START);

	//������עʱ��
	if (m_pIAndroidUserItem->GetChairID()!=m_wBankerUser)
	{
		UINT nElapse=rand()%(TIME_START_GAME*2)+TIME_LESS*2;
		m_pIAndroidUserItem->SetGameTimer(IDI_PLAYER_START,nElapse);
	}

	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(const void * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//������Ϸ
	UINT nElapse=rand()%(TIME_START_GAME+2)+TIME_LESS*2;
	m_pIAndroidUserItem->SetGameTimer(IDI_CONTINUE,nElapse);

	//ɾ��ʱ��

	return true;
}


//////////////////////////////////////////////////////////////////////////
