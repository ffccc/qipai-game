#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//����ʱ��
#define TIME_LESS					2									//����ʱ��
#define TIME_DISPATCH				5									//����ʱ��

//��Ϸʱ��
#define TIME_OUT_CARD				4									//����ʱ��
#define TIME_START_GAME				5									//��ʼʱ��
#define TIME_CALL_SCORE				2									//�з�ʱ��
#define TIME_CALL_BANKER			2									//ץ��ʱ��

//��Ϸʱ��
#define IDI_OUT_CARD				(IDI_ANDROID_ITEM_SINK+0)			//����ʱ��
#define IDI_START_GAME				(IDI_ANDROID_ITEM_SINK+1)			//��ʼʱ��
#define IDI_CALL_SCORE				(IDI_ANDROID_ITEM_SINK+2)			//�з�ʱ��
#define IDI_CALL_BANKER				(IDI_ANDROID_ITEM_SINK+3)			//ץ��ʱ��

//////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//�˿˱���
	m_cbTurnCardCount=0;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));
	m_wOutCardUser = INVALID_CHAIR ;
	m_wBankerUser = INVALID_CHAIR ;

	//�����˿�
	m_cbHandCardCount=0;
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	m_cbCurrentLandScore = 255 ;

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
	//�˿˱���
	m_cbTurnCardCount=0;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

	//�����˿�
	m_cbHandCardCount=0;
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

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
	case IDI_CALL_BANKER:	//���ץ��
		{
			//��ץ�ж�
			bool bBackCatch = rand() % 3 == 0 ? true : false;

			//��������
			CMD_C_LandScore LandScore;
			LandScore.bLandScore=(bBackCatch)?BACK_CATCH:LOOK_CARD;
			m_pIAndroidUserItem->SendSocketData(SUB_C_LAND_SCORE,&LandScore,sizeof(LandScore));

			return true;
		}
	case IDI_CALL_SCORE:	//�û��з�
		{
			//�������
			CMD_C_LandScore CallScore;
			ZeroMemory(&CallScore,sizeof(CallScore));

			//���ñ���
			CallScore.bLandScore=m_GameLogic.LandScore(m_pIAndroidUserItem->GetChairID(), m_cbCurrentLandScore);

			//��������
			m_pIAndroidUserItem->SendSocketData(SUB_C_LAND_SCORE,&CallScore,sizeof(CallScore));

			return true;
		}
	case IDI_OUT_CARD:		//�û�����
		{
			//�˿˷���
			tagOutCardResult OutCardResult;
			try
			{				
				m_GameLogic.SearchOutCard(m_cbHandCardData,m_cbHandCardCount,m_cbTurnCardData,m_cbTurnCardCount, m_wOutCardUser, m_pIAndroidUserItem->GetChairID(), OutCardResult);
			}
			catch(...)
			{
				//��������ã�ʹ�ý��������if����
				ZeroMemory(OutCardResult.cbResultCard, sizeof(OutCardResult.cbResultCard)) ;
				OutCardResult.cbCardCount = 10 ;
			}
			//���ͺϷ��ж�
			if(OutCardResult.cbCardCount>0 && CT_ERROR==m_GameLogic.GetCardType(OutCardResult.cbResultCard, OutCardResult.cbCardCount))
			{
				ASSERT(false) ;
#ifdef _DEBUG
				CString str ;
				for(BYTE i=0; i<OutCardResult.cbCardCount; ++i)
				{
					CString strTmp ;
					strTmp.Format("%d,", m_GameLogic.GetCardValue(OutCardResult.cbResultCard[i])) ;
					str += strTmp ;
				}
				str += "\n\n" ;
				OutputDebugString(str) ;
			
#endif
				ZeroMemory(&OutCardResult, sizeof(OutCardResult)) ;
			}

			//�ȳ��Ʋ���Ϊ��
			if(m_cbTurnCardCount==0)
			{
				ASSERT(OutCardResult.cbCardCount>0) ;
				if(OutCardResult.cbCardCount==0)
				{
					//��Сһ��
					OutCardResult.cbCardCount = 1 ;
					OutCardResult.cbResultCard[0]=m_cbHandCardData[m_cbHandCardCount-1] ;
				}
			}

			//����ж�
			if (OutCardResult.cbCardCount>0)
			{
				//�Ϸ��ж�
				if(m_cbTurnCardCount>0 && !m_GameLogic.CompareCard(m_cbTurnCardData, OutCardResult.cbResultCard, m_cbTurnCardCount, OutCardResult.cbCardCount))
				{
					ASSERT(false) ;

					//��������
					m_pIAndroidUserItem->SendSocketData(SUB_C_PASS_CARD);
					return true ;
				}

				//ɾ���˿�
				m_cbHandCardCount-=OutCardResult.cbCardCount;
				m_GameLogic.RemoveCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount,m_cbHandCardData,m_cbHandCardCount+OutCardResult.cbCardCount);

				//�������
				CMD_C_OutCard OutCard;
				ZeroMemory(&OutCard,sizeof(OutCard));

				//���ñ���
				OutCard.bCardCount=OutCardResult.cbCardCount;
				CopyMemory(OutCard.bCardData,OutCardResult.cbResultCard,OutCardResult.cbCardCount*sizeof(BYTE));

				//��������
				WORD wHeadSize=sizeof(OutCard)-sizeof(OutCard.bCardData);
				m_pIAndroidUserItem->SendSocketData(SUB_C_OUT_CART,&OutCard,wHeadSize+OutCard.bCardCount*sizeof(BYTE));
			}
			else
			{
				//��������
				m_pIAndroidUserItem->SendSocketData(SUB_C_PASS_CARD);
			}

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
	case SUB_S_SEND_CARD:	//��Ϸ��ʼ
		{
			return OnSubSendCard(pData,wDataSize);
		}
	case SUB_S_LAND_SCORE:	//�û��з�
		{
			return OnSubCallScore(pData,wDataSize);
		}
	case SUB_S_GAME_START:	//ׯ����Ϣ
		{
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_OUT_CARD:	//�û�����
		{
			return OnSubOutCard(pData,wDataSize);
		}
	case SUB_S_PASS_CARD:	//�û�����
		{
			return OnSubPassCard(pData,wDataSize);
		}
	case SUB_S_GAME_END:	//��Ϸ����
		{
			return OnSubGameEnd(pData,wDataSize);
		}
	case SUB_S_CHOICE_LOOK:		//�û���ׯ
		{
			return OnSubCallBanker(pData,wDataSize);
		}
	case SUB_C_TRUSTEE:			//����й�
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
	case GS_WK_FREE:			//����״̬
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
	case GS_WK_SCORE:		//�з�״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusScore)) return false;
			CMD_S_StatusScore * pStatusCall=(CMD_S_StatusScore *)pData;

			//�˿�����
			m_cbHandCardCount=NORMAL_COUNT;
			CopyMemory(m_cbHandCardData,pStatusCall->bCardData,sizeof(BYTE)*m_cbHandCardCount);

			//�з�����
			if (m_pIAndroidUserItem->GetChairID()==pStatusCall->wCurrentUser)
			{
				UINT nElapse=rand()%TIME_CALL_SCORE+TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_CALL_SCORE,nElapse+TIME_DISPATCH);
			}

			return true;
		}
	case GS_WK_PLAYING:		//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			//���Ʊ���
			m_cbTurnCardCount=pStatusPlay->bTurnCardCount;
			CopyMemory(m_cbTurnCardData,pStatusPlay->bTurnCardData,m_cbTurnCardCount*sizeof(BYTE));

			//�˿�����
			WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
			m_cbHandCardCount=pStatusPlay->bCardCount[wMeChairID];
			CopyMemory(m_cbHandCardData,pStatusPlay->bCardData,sizeof(BYTE)*m_cbHandCardCount);

			//�������
			if (pStatusPlay->wCurrentUser==m_pIAndroidUserItem->GetChairID())
			{
				UINT nElapse=rand()%TIME_OUT_CARD+TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_OUT_CARD,nElapse);
			}

			return true;
		}
	}

	//�������
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
bool CAndroidUserItemSink::OnSubSendCard(void * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_S_SendAllCard));
	if (wDataSize!=sizeof(CMD_S_SendAllCard)) return false;

	//��������
	CMD_S_SendAllCard * pSendCard=(CMD_S_SendAllCard *)pData;

	//����״̬
	m_pIAndroidUserItem->SetGameStatus(GS_WK_SCORE);

	//�˿˱���
	m_cbTurnCardCount=0;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));
	m_cbCurrentLandScore = 255 ;

	//�����˿�
	for(WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
		m_GameLogic.SetUserCard(wChairID, pSendCard->bCardData[wChairID], NORMAL_COUNT) ;

	//�����˿�
	BYTE cbLandScoreCardData[MAX_COUNT] ;
	CopyMemory(cbLandScoreCardData, pSendCard->bCardData[m_pIAndroidUserItem->GetChairID()], NORMAL_COUNT) ;
	CopyMemory(cbLandScoreCardData+NORMAL_COUNT, pSendCard->bBackCardData, BACK_COUNT) ;
	m_GameLogic.SetLandScoreCardData(cbLandScoreCardData, sizeof(cbLandScoreCardData)) ;
	CopyMemory(m_bBackCard, pSendCard->bBackCardData, BACK_COUNT) ;

	//�����˿�
	m_cbHandCardCount=NORMAL_COUNT;	
	CopyMemory(m_cbHandCardData, pSendCard->bCardData[m_pIAndroidUserItem->GetChairID()], NORMAL_COUNT) ; 

	//�����˿�
	m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount,ST_ORDER);

	//��Ҵ���
	//if (m_pIAndroidUserItem->GetChairID()==pSendCard->wCurrentUser)
	//{
	//	UINT nElapse=rand()%TIME_CALL_SCORE+TIME_LESS;
	//	m_pIAndroidUserItem->SetGameTimer(IDI_CALL_SCORE,nElapse+TIME_DISPATCH);
	//}

	return true;
}

//�û��з�
bool CAndroidUserItemSink::OnSubCallScore(void * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_S_LandScore));
	if (wDataSize!=sizeof(CMD_S_LandScore)) return false;

	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_LandScore));
	if (wDataSize!=sizeof(CMD_S_LandScore)) return false;

	//��Ϣ����
	CMD_S_LandScore * pLandScore=(CMD_S_LandScore *)pData;

	//���õ���
	if(pLandScore->wBankerUser!=INVALID_CHAIR)
	{
		m_wBankerUser=pLandScore->wBankerUser;
		m_GameLogic.SetBanker(pLandScore->bLandUser) ;
	}

	//���״̬
	m_GameLogic.SetCurrentStatus(pLandScore->bCurrentState&0x0F);

	//�û�����
	if (m_pIAndroidUserItem->GetChairID()==pLandScore->wCurrentUser)
	{
		UINT nElapse=rand()%TIME_CALL_SCORE+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_CALL_SCORE,nElapse);
	}

	//����״̬
	m_GameLogic.SetCallScore(pLandScore->bLandUser, pLandScore->bLandScore == PASS_ACTION ? false : true);

	return true;
}

//ׯ����Ϣ
bool CAndroidUserItemSink::OnSubGameStart(void * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//��������
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pData;

	//����״̬
	m_pIAndroidUserItem->SetGameStatus(GS_WK_PLAYING);

	
	//�������
	ASSERT(m_bBackCard[0]!=0);
	m_GameLogic.SetBackCard(m_wBankerUser, m_bBackCard, BACK_COUNT);

	//��������
	if (m_wBankerUser==m_pIAndroidUserItem->GetChairID() && m_cbHandCardCount<20)
	{
		BYTE bCardCound=m_cbHandCardCount;
		m_cbHandCardCount+=CountArray(m_bBackCard);
		CopyMemory(&m_cbHandCardData[bCardCound],m_bBackCard,sizeof(m_bBackCard));
		m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount,ST_ORDER);
	}

	//�������
	if (pGameStart->wCurrentUser==m_pIAndroidUserItem->GetChairID())
	{
		UINT nElapse=rand()%TIME_OUT_CARD+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_OUT_CARD,nElapse);
	}

	ASSERT(m_GameLogic.m_cbUserCardCount[0] + m_GameLogic.m_cbUserCardCount[1] + m_GameLogic.m_cbUserCardCount[2] == 54 );
	if ( m_GameLogic.m_cbUserCardCount[0] + m_GameLogic.m_cbUserCardCount[1] + m_GameLogic.m_cbUserCardCount[2] != 54 )
		return true;

	return true;
}

//�û�����
bool CAndroidUserItemSink::OnSubOutCard(void * pData, WORD wDataSize)
{
	//��������
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pData;
	WORD wHeadSize=sizeof(CMD_S_OutCard)-sizeof(pOutCard->bCardData);

	//Ч������
	ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pOutCard->bCardCount*sizeof(BYTE))));
	if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pOutCard->bCardCount*sizeof(BYTE)))) return false;

	//���Ʊ���
	if (pOutCard->wCurrentUser==pOutCard->wOutCardUser)
	{
		m_cbTurnCardCount=0;
		ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));
	}
	else
	{
		m_cbTurnCardCount=pOutCard->bCardCount;
		m_wOutCardUser = pOutCard->wOutCardUser ;
		CopyMemory(m_cbTurnCardData,pOutCard->bCardData,pOutCard->bCardCount*sizeof(BYTE));
	}

	//�������
	if (m_pIAndroidUserItem->GetChairID()==pOutCard->wCurrentUser)
	{
		UINT nElapse=rand()%TIME_OUT_CARD+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_OUT_CARD,nElapse);
	}

	//���ñ���
	m_GameLogic.RemoveUserCardData(pOutCard->wOutCardUser, pOutCard->bCardData, pOutCard->bCardCount) ;

	return true;
}

//�û�����
bool CAndroidUserItemSink::OnSubPassCard(void * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PassCard));
	if (wDataSize!=sizeof(CMD_S_PassCard)) return false;

	//��������
	CMD_S_PassCard * pPassCard=(CMD_S_PassCard *)pData;

	//һ���ж�
	if (pPassCard->bNewTurn==TRUE)
	{
		m_cbTurnCardCount=0;
		ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));
	}

	//�������
	if (m_pIAndroidUserItem->GetChairID()==pPassCard->wCurrentUser)
	{
		UINT nElapse=rand()%TIME_OUT_CARD+TIME_LESS;
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

	//��������
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pData;

	//����״̬
	m_pIAndroidUserItem->SetGameStatus(GS_WK_FREE);

	//���ñ���
	m_cbTurnCardCount=0;
	m_cbHandCardCount=0;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//ɾ��ʱ��
	m_pIAndroidUserItem->KillGameTimer(IDI_OUT_CARD);
	m_pIAndroidUserItem->KillGameTimer(IDI_CALL_SCORE);

	//��ʼ����
	UINT nElapse=rand()%TIME_START_GAME+TIME_LESS;
	m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);

	return true;
}

//�û���ׯ
bool CAndroidUserItemSink::OnSubCallBanker(void * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_CallBanker));
	if (wDataSize!=sizeof(CMD_S_CallBanker)) return false;

	//��������
	CMD_S_CallBanker * pCallBanker=(CMD_S_CallBanker *)pData;

	//�ؼ��ж�
	if (m_pIAndroidUserItem->GetChairID()==pCallBanker->wCurrentUser)
	{
		UINT nElapse=rand()%TIME_CALL_BANKER+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_CALL_BANKER,nElapse);
	}

	//���ý���
	m_cbHandCardCount=17;
	m_bMingCard = pCallBanker->bMingCard;

	//����״̬
	m_GameLogic.SetCallScore(INVALID_CHAIR, false);

	return true;
}

//////////////////////////////////////////////////////////////////////////
