#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//����ʱ��
#define TIME_LESS					1									//����ʱ��

//��Ϸʱ��
#define TIME_OUT_CARD				8									//����ʱ��
#define TIME_START_GAME				8									//��ʼʱ��

#define TIME_PASS_CARD				3									//PASSʱ��

//��Ϸʱ��
#define IDI_OUT_CARD				(IDI_ANDROID_ITEM_SINK+0)			//����ʱ��
#define IDI_START_GAME				(IDI_ANDROID_ITEM_SINK+1)			//��ʼʱ��
#define IDI_PASS_CARD				(IDI_ANDROID_ITEM_SINK+2)			//PASSʱ��

//////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	m_wOutCardUser = INVALID_CHAIR;
	m_wMeChairId =  INVALID_CHAIR;

	//��Ϸ����
	m_bOutCard3=TRUE;
	m_wBankerUser=INVALID_CHAIR;
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bCardData,0,sizeof(m_bCardData));
	ZeroMemory(&m_AnalysedCard,sizeof(m_AnalysedCard));

	//���Ʊ���
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//���Ƽ�¼
	ZeroMemory( m_bOutCardRecord,sizeof(m_bOutCardRecord) );
	m_bOutCardCount = 0;

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
	m_wOutCardUser = INVALID_CHAIR;
	m_wMeChairId = INVALID_CHAIR;

	//��Ϸ����
	m_bOutCard3=TRUE;
	m_wBankerUser=INVALID_CHAIR;
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bCardData,0,sizeof(m_bCardData));
	ZeroMemory(&m_AnalysedCard,sizeof(m_AnalysedCard));

	//���Ʊ���
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//���Ƽ�¼
	ZeroMemory( m_bOutCardRecord,sizeof(m_bOutCardRecord) );
	m_bOutCardCount = 0;

	return true;
}

//ʱ����Ϣ
bool __cdecl CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_START_GAME:	//��ʼ��Ϸ
		{
			//��ʼ�ж�
			m_pIAndroidUserItem->SendUserReady(NULL,0);

			return true;
		}
	case IDI_OUT_CARD:		//�û�����
		{
			tagOutCardResult OutCardResult;

			ASSERT( m_bOutCardCount <= FULL_COUNT );
			//�ж�һ�ֽ���
			for( int i = 0; i < GAME_PLAYER; i++ )
			{
				if( m_bCardCount[i]==0 ) return true;
			}

			//�״γ���
			if((m_bOutCard3==TRUE)&&(m_wBankerUser==m_pIAndroidUserItem->GetChairID())&&(m_bCardCount[m_wBankerUser]==13) )
			{
				if( m_GameLogic.AutomatismPlay( m_AnalysedCard,m_pIAndroidUserItem->GetChairID(),m_wOutCardUser,m_bTurnCardData,m_bTurnCardCount,
					m_bCardData,m_bCardCount,m_bOutCardRecord,m_bOutCardCount,OutCardResult,true ) )
				{
					//ɾ���˿�
					BYTE bSourceCount=m_bCardCount[m_wMeChairId];
					m_bCardCount[m_wMeChairId] -=OutCardResult.cbCardCount;
					m_GameLogic.RemoveCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount,m_bCardData[m_wMeChairId],bSourceCount);

					//��������
					CMD_C_OutCard OutCard;
					OutCard.bCardCount=OutCardResult.cbCardCount;
					CopyMemory( OutCard.bCardData,OutCardResult.cbResultCard,sizeof(BYTE)*OutCardResult.cbCardCount );
					m_pIAndroidUserItem->SendSocketData( SUB_C_OUT_CART,&OutCard,sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE) );

					return true;
				}
				ASSERT(FALSE);
				return false;
			}

			//���״γ���
			if( m_GameLogic.AutomatismPlay( m_AnalysedCard,m_pIAndroidUserItem->GetChairID(),m_wOutCardUser,m_bTurnCardData,m_bTurnCardCount,
				m_bCardData,m_bCardCount,m_bOutCardRecord,m_bOutCardCount,OutCardResult) )
			{
				//ɾ���˿�
				BYTE bSourceCount=m_bCardCount[m_wMeChairId];
				m_bCardCount[m_wMeChairId] -=OutCardResult.cbCardCount;
				m_GameLogic.RemoveCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount,m_bCardData[m_wMeChairId],bSourceCount);

				//��������
				CMD_C_OutCard OutCard;
				OutCard.bCardCount=OutCardResult.cbCardCount;
				CopyMemory( OutCard.bCardData,OutCardResult.cbResultCard,sizeof(BYTE)*OutCardResult.cbCardCount );
				m_pIAndroidUserItem->SendSocketData( SUB_C_OUT_CART,&OutCard,sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE) );

				return true;
			}

			m_pIAndroidUserItem->SendSocketData( SUB_C_PASS_CARD );
			return true;
		}
	case IDI_PASS_CARD:
		{
			m_pIAndroidUserItem->SendSocketData( SUB_C_PASS_CARD );
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
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_OUT_CARD:		//�û�����
		{
			return OnSubOutCard(pData,wDataSize);
		}
	case SUB_S_PASS_CARD:		//��������
		{
			return OnSubPassCard(pData,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pData,wDataSize);
		}
	case SUB_S_STRUSTEE:
		{
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
bool __cdecl CAndroidUserItemSink::OnEventGameScene(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GS_RF_FREE:	//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��������
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;
			IServerUserItem * pIServerUserItem=m_pIAndroidUserItem->GetMeUserItem();

			//�������
			if (pIServerUserItem->GetUserStatus()!=US_READY)
			{
				UINT nElapse=rand()%TIME_START_GAME+TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
			}

			return true;
		}
	case GS_RF_PLAYING:		//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			//���ñ���
			m_wBankerUser=pStatusPlay->wBankerUser;
			m_bTurnCardCount=pStatusPlay->bTurnCardCount;
			m_bTurnOutType=m_GameLogic.GetCardType(pStatusPlay->bTurnCardData,m_bTurnCardCount);

			CopyMemory(m_bCardData,pStatusPlay->bCardData,sizeof(m_bCardData));

			CopyMemory(m_bTurnCardData,pStatusPlay->bTurnCardData,pStatusPlay->bTurnCardCount);
			CopyMemory(m_bCardCount,pStatusPlay->bCardCount,sizeof(BYTE)*GAME_PLAYER);

			m_wOutCardUser = pStatusPlay->wLastOutUser;


			//���ö�ʱ��
			if (pStatusPlay->wCurrentUser==m_pIAndroidUserItem->GetChairID())
			{
				UINT nElapse=rand()%TIME_OUT_CARD+TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_OUT_CARD,nElapse);
			}

			return true;
		}
	}

	ASSERT(false);
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

//ׯ����Ϣ
bool CAndroidUserItemSink::OnSubGameStart(void * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pData;

	//���ñ���
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	m_bOutCard3=pGameStart->cbOutCard3;
	m_wBankerUser=pGameStart->wBankerUser;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	m_bOutCardCount = 0;
	ZeroMemory( m_bOutCardRecord,sizeof(m_bOutCardRecord) );

	//�����˿�
	CopyMemory(m_bCardData,pGameStart->cbCardData,sizeof(m_bCardData));

	//�˿���Ŀ
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_bCardCount[i]=13;
	}

	m_wMeChairId = m_pIAndroidUserItem->GetChairID();
	//�����ֲ�
	m_GameLogic.AnalyseCardData( m_bCardData[m_wMeChairId],m_bCardCount[m_wMeChairId],m_AnalysedCard );

	//�������
	if (pGameStart->wCurrentUser==m_pIAndroidUserItem->GetChairID())
	{
		UINT nElapse=rand()%TIME_OUT_CARD+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_OUT_CARD,nElapse);
	}

	return true;
}

//�û�����
bool CAndroidUserItemSink::OnSubOutCard(void * pData, WORD wDataSize)
{
	//��������
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pData;
	WORD wHeadSize=sizeof(CMD_S_OutCard)-sizeof(pOutCard->bCardData);

	//Ч������
	ASSERT(wDataSize>=wHeadSize);
	if (wDataSize<wHeadSize) return false;
	ASSERT(wDataSize==(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData[0])));
	if (wDataSize!=(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData[0]))) return false;

	//���ñ���
	if( pOutCard->wOutCardUser != m_wMeChairId )
	{
		m_GameLogic.RemoveCard(pOutCard->bCardData,pOutCard->bCardCount,
			m_bCardData[pOutCard->wOutCardUser],m_bCardCount[pOutCard->wOutCardUser]);
		m_bCardCount[pOutCard->wOutCardUser]-=pOutCard->bCardCount;
	}


	//��¼����
	m_bTurnCardCount=pOutCard->bCardCount;
	m_bTurnOutType=m_GameLogic.GetCardType(pOutCard->bCardData,pOutCard->bCardCount);
	CopyMemory(m_bTurnCardData,pOutCard->bCardData,sizeof(BYTE)*pOutCard->bCardCount);

	m_wOutCardUser = pOutCard->wOutCardUser;

	//��������¼
	for( int i = 0; i < m_bTurnCardCount; i++ )
	{
		m_bOutCardRecord[m_bOutCardCount++] = m_bTurnCardData[i];
	}


	//����ж�
	if (pOutCard->wCurrentUser==pOutCard->wOutCardUser)
	{
		//���ñ���
		m_bTurnCardCount=0;
		m_bTurnOutType=CT_ERROR;
		memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	}

	//�������
	if (m_pIAndroidUserItem->GetChairID()==pOutCard->wCurrentUser)
	{
		UINT nElapse;
		//����С�ڳ������Զ�PASS
		if( m_bTurnCardCount > m_bCardCount[pOutCard->wCurrentUser] )
		{
			nElapse=rand()%TIME_PASS_CARD+TIME_LESS;
			m_pIAndroidUserItem->SetGameTimer(IDI_PASS_CARD,nElapse);
			return true;
		}
		nElapse=rand()%TIME_OUT_CARD+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_OUT_CARD,nElapse);
	}

	return true;
}

//�û�����
bool CAndroidUserItemSink::OnSubPassCard(void * pData, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_PassCard)) return false;
	CMD_S_PassCard * pPassCard=(CMD_S_PassCard *)pData;

	//һ���ж�
	if (pPassCard->bNewTurn==TRUE)
	{
		m_bTurnCardCount=0;
		m_bTurnOutType=CT_ERROR;
		memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	}

	//�������
	if (m_pIAndroidUserItem->GetChairID()==pPassCard->wCurrentUser)
	{
		UINT nElapse;
		//����С�ڳ������Զ�PASS
		if( m_bTurnCardCount > m_bCardCount[pPassCard->wCurrentUser] )
		{
			nElapse=rand()%TIME_PASS_CARD+TIME_LESS;
			m_pIAndroidUserItem->SetGameTimer(IDI_PASS_CARD,nElapse);
			return true;
		}
		nElapse=rand()%TIME_OUT_CARD+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_OUT_CARD,nElapse);
	}

	return true;
}


//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(void * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pData;

	//����״̬
	m_pIAndroidUserItem->SetGameStatus(GS_RF_FREE);

	//ɾ��ʱ��
	m_pIAndroidUserItem->KillGameTimer(IDI_OUT_CARD);

	//��ʼ����
	UINT nElapse=rand()%TIME_START_GAME+TIME_LESS;
	m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);

	return true;
}

//////////////////////////////////////////////////////////////////////////
