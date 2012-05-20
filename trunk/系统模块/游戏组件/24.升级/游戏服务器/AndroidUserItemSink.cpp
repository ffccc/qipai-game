#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//test
//#define TEST						

#ifndef	TEST
//����ʱ��
#define TIME_LESS_OUTCARD			1									//��������ʱ��
#define TIME_LESS_STARTGAME			1									//��ʼ����ʱ��
#define TIME_LESS_CALLCARD			2									//��������ʱ��
#define TIME_LESS_SENDCONCEAL		5									//���������ʱ��
#define TIME_LESS_CALLFINISH		1									//��ɽ�������ʱ��
//��Ϸʱ��
#define TIME_START_GAME				8									//��ʼʱ��
#define TIME_CALL_CARD				10									//����ʱ��
#define TIME_CALL_FINISH			5									//��ɽ���ʱ��
#define TIME_SEND_CONCEAL			12									//�����ʱ��
#define TIME_OUT_CARD				5									//����ʱ��
#define TIME_RESPONSE_LEAVE			4									//��Ӧ�뿪����
#else
//����ʱ��
#define TIME_LESS_OUTCARD			1									//��������ʱ��
#define TIME_LESS_STARTGAME			1									//��ʼ����ʱ��
#define TIME_LESS_CALLCARD			2									//��������ʱ��
#define TIME_LESS_SENDCONCEAL		5									//���������ʱ��
#define TIME_LESS_CALLFINISH		2									//��ɽ�������ʱ��
//��Ϸʱ��
#define TIME_START_GAME				1									//��ʼʱ��
#define TIME_CALL_CARD				1									//����ʱ��
#define TIME_CALL_FINISH			1									//��ɽ���ʱ��
#define TIME_SEND_CONCEAL			1									//�����ʱ��
#define TIME_OUT_CARD				1									//����ʱ��
#define TIME_RESPONSE_LEAVE			4									//��Ӧ�뿪����
#endif

//��Ϸʱ��
#define IDI_START_GAME				(IDI_ANDROID_ITEM_SINK+1)			//��ʼʱ��
#define IDI_CALL_CARD				(IDI_ANDROID_ITEM_SINK+2)			//����ʱ��
#define IDI_SEND_CONCEAL			(IDI_ANDROID_ITEM_SINK+3)			//�����ʱ��
#define IDI_OUT_CARD				(IDI_ANDROID_ITEM_SINK+4)			//����ʱ��
#define IDI_CALL_FINISH				(IDI_ANDROID_ITEM_SINK+5)			//����ʱ��
#define IDI_RESPONSE_LEAVE			(IDI_ANDROID_ITEM_SINK+6)			//��Ӧ�뿪����

//////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//�߼�����
	m_cbPackCount=0;
	m_cbMainColor=COLOR_ERROR;
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

	//���Ʊ���
	m_wFirstOutUser=INVALID_CHAIR;
	ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
	ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));

	//�����˿�
	m_cbConcealCount=0;
	ZeroMemory(m_cbConcealCard,sizeof(m_cbConcealCard));

	//�û��˿�
	m_cbHandCardCount=0;
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

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
	//�߼�����
	m_cbPackCount=0;
	m_cbMainColor=COLOR_ERROR;
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

	//���Ʊ���
	m_wFirstOutUser=INVALID_CHAIR;
	ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
	ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));

	//�����˿�
	m_cbConcealCount=0;
	ZeroMemory(m_cbConcealCard,sizeof(m_cbConcealCard));

	//�û��˿�
	m_cbHandCardCount=0;
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//ɾ����ʱ��
	m_pIAndroidUserItem->KillGameTimer(0);

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
	case IDI_CALL_CARD:		//����
		{
			VERIFY( CallCard() );
			return true;
		}
	case IDI_CALL_FINISH:	//��ɽ���
		{
			m_pIAndroidUserItem->SendSocketData(SUB_C_CALL_FINISH);
			return true;
		}
	case IDI_SEND_CONCEAL:	//����
		{
			VERIFY( SendConceal() );
			return true;
		}	
	case IDI_OUT_CARD:		//����
		{
			VERIFY( OutCard() );
			return true;
		}
	case IDI_RESPONSE_LEAVE:	//�û������뿪
		{
			for( INT_PTR i = 0; i < m_RequestUserAry.GetCount(); i++ )
			{
				//����ж��Ƿ�ͬ��
				BOOL bAgree = (rand()%2==0)?TRUE:FALSE;

				//��������
				CMD_C_ResponsesLeave ResponsesLeave;
				ZeroMemory(&ResponsesLeave,sizeof(ResponsesLeave));
				//������Ϣ
				ResponsesLeave.wChairID=m_RequestUserAry[i];
				ResponsesLeave.cbAgreeLeave=bAgree;

				//������Ϣ
				m_pIAndroidUserItem->SendSocketData(SUB_C_RESPONSES_LEAVE,&ResponsesLeave,sizeof(ResponsesLeave));

				//ɾ���û�
				m_RequestUserAry.RemoveAt(i);
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
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_SEND_CARD:		//�����˿�
		{
			return OnSubSendCard(pData,wDataSize);
		}
	case SUB_S_CALL_CARD:		//�û�����
		{
			return OnSubCallCard(pData,wDataSize);
		}
	case SUB_S_SEND_CONCEAL:	//���͵���
		{
			return OnSubSendConceal(pData,wDataSize);
		}
	case SUB_S_GAME_PLAY:		//��Ϸ��ʼ
		{
			return OnSubGamePlay(pData,wDataSize);
		}
	case SUB_S_OUT_CARD:		//�û�����
		{
			return OnSubOutCard(pData,wDataSize);
		}
	case SUB_S_THROW_RESULT:	//˦�ƽ��
		{
			return OnSubThrowResult(pData,wDataSize);
		}
	case SUB_S_TURN_BALANCE:	//һ�ֽ���
		{
			return OnSubTurnBalance(pData,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pData,wDataSize);
		}
	case SUB_S_STRUSTEE:
		{
			return true;
		}
	case SUB_S_REQUEST_LEAVE:	//�����뿪
		{
			return OnSubRequestLeave(pData,wDataSize);
		}
	case SUB_S_BATCH_CARD:		//��������
		{
			return OnSubBatchCard(pData,wDataSize);
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
	case GS_UG_FREE:	//����״̬
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
				UINT nElapse=rand()%TIME_START_GAME+TIME_LESS_STARTGAME;
				m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
			}

			return true;
		}
	case GS_UG_CALL:
	case GS_UG_BACK:
	case GS_UG_PLAY:		//��Ϸ״̬
		{
			ASSERT( FALSE );
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

//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//����״̬
	m_pIAndroidUserItem->SetGameStatus(GS_UG_CALL);

	//�߼�����
	m_cbMainColor=COLOR_ERROR;
	m_cbMainValue=pGameStart->cbMainValue;
	m_cbPackCount=pGameStart->cbPackCount;

	//���Ʊ���
	m_cbCallCard=0;
	m_cbCallCount=0;
	m_wCallCardUser=INVALID_CHAIR;

	//���Ʊ���
	m_cbConcealCount=0;
	ZeroMemory(m_cbConcealCard,sizeof(m_cbConcealCard));

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
	m_GameLogic.SetMainValue(m_cbMainValue);

	return true;
}

//�����˿�
bool CAndroidUserItemSink::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//��������
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;
	WORD wFirstSize=sizeof(CMD_S_SendCard)-sizeof(pSendCard->cbCardData);

	//Ч������
	ASSERT(wDataSize>=wFirstSize);
	if (wDataSize<wFirstSize) return false;
	ASSERT(wDataSize==(wFirstSize+pSendCard->cbCardCount*sizeof(BYTE)));
	if (wDataSize!=(wFirstSize+pSendCard->cbCardCount*sizeof(BYTE))) return false;

	//�����˿�
	m_cbHandCardCount=pSendCard->cbCardCount;
	CopyMemory(m_cbHandCardData,pSendCard->cbCardData,sizeof(BYTE)*m_cbHandCardCount);

	m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount);

	//���ƶ�ʱ
	UINT nElapse = TIME_LESS_CALLCARD+rand()%TIME_CALL_CARD;
	m_pIAndroidUserItem->SetGameTimer(IDI_CALL_CARD,nElapse);

	return true;
}

//�û�����
bool CAndroidUserItemSink::OnSubCallCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_CallCard));
	if (wDataSize!=sizeof(CMD_S_CallCard)) return false;

	//��Ϣ����
	CMD_S_CallCard * pCallCard=(CMD_S_CallCard *)pBuffer;

	//���ñ���
	m_cbCallCard=pCallCard->cbCallCard;
	m_cbCallCount=pCallCard->cbCallCount;
	m_wCallCardUser=pCallCard->wCallCardUser;

	return true;
}

//���͵���
bool CAndroidUserItemSink::OnSubSendConceal(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SendConceal));
	if (wDataSize!=sizeof(CMD_S_SendConceal)) return false;

	//��Ϣ����
	CMD_S_SendConceal * pSendConceal=(CMD_S_SendConceal *)pBuffer;

	//���ñ���
	m_cbMainColor=pSendConceal->cbMainColor;
	m_wBankerUser=pSendConceal->wBankerUser;
	m_wCurrentUser=pSendConceal->wCurrentUser;

	//״̬����
	m_pIAndroidUserItem->SetGameStatus(GS_UG_BACK);
	m_GameLogic.SetMainColor(m_cbMainColor);

	//�˿�����
	if (m_wCurrentUser==m_pIAndroidUserItem->GetChairID())
	{
		//���õ���
		m_cbConcealCount=pSendConceal->cbConcealCount;
		CopyMemory(m_cbConcealCard,pSendConceal->cbConcealCard,m_cbConcealCount*sizeof(BYTE));

		//�����˿�
		CopyMemory(&m_cbHandCardData[m_cbHandCardCount],m_cbConcealCard,m_cbConcealCount*sizeof(BYTE));
		m_cbHandCardCount+=m_cbConcealCount;

		//�����˿�
		m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount);

		//���ö�ʱ
		UINT nElapse = TIME_LESS_SENDCONCEAL + rand()%TIME_SEND_CONCEAL;
		m_pIAndroidUserItem->SetGameTimer(IDI_SEND_CONCEAL,nElapse);
	}
	else
	{
		//�����˿�
		m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount);
	}

	return true;
}

//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGamePlay(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GamePlay));
	if (wDataSize!=sizeof(CMD_S_GamePlay)) return false;

	//��Ϣ����
	CMD_S_GamePlay * pGamePlay=(CMD_S_GamePlay *)pBuffer;

	//���ñ���
	m_wCurrentUser=pGamePlay->wCurrentUser;
	m_wFirstOutUser=pGamePlay->wCurrentUser;
	m_cbConcealCount=pGamePlay->cbConcealCount;
	CopyMemory(m_cbConcealCard,pGamePlay->cbConcealCard,m_cbConcealCount*sizeof(BYTE));

	//����״̬
	m_pIAndroidUserItem->SetGameStatus(GS_UG_PLAY);

	//��ǰ���
	WORD wMeChairid = m_pIAndroidUserItem->GetChairID();
	if (m_wCurrentUser==wMeChairid)
	{
		UINT uElapse = TIME_LESS_OUTCARD + rand()%TIME_OUT_CARD;
		m_pIAndroidUserItem->SetGameTimer(IDI_OUT_CARD,uElapse);
	}

	return true;
}

//�û�����
bool CAndroidUserItemSink::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
	//��������
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;
	WORD wFirstSize=sizeof(CMD_S_OutCard)-sizeof(pOutCard->cbCardData);

	//Ч������
	ASSERT(wDataSize>=wFirstSize);
	if (wDataSize<wFirstSize) return false;
	ASSERT(wDataSize==(wFirstSize+pOutCard->cbCardCount*sizeof(BYTE)));
	if (wDataSize!=(wFirstSize+pOutCard->cbCardCount*sizeof(BYTE))) return false;

	//��������
	WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
	WORD wOutCardUser=pOutCard->wOutCardUser;

	//���Ƽ�¼
	m_cbOutCardCount[wOutCardUser]=pOutCard->cbCardCount;
	CopyMemory(m_cbOutCardData[wOutCardUser],pOutCard->cbCardData,pOutCard->cbCardCount*sizeof(BYTE));

	//���ñ���
	m_wCurrentUser=pOutCard->wCurrentUser;

	//�������
	if (m_wCurrentUser==wMeChairID)
	{
		UINT uElapse = TIME_LESS_OUTCARD + rand()%TIME_OUT_CARD;
		m_pIAndroidUserItem->SetGameTimer(IDI_OUT_CARD,uElapse);
	}

	return true;
}

//˦�ƽ��
bool CAndroidUserItemSink::OnSubThrowResult(const void * pBuffer, WORD wDataSize)
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
	WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
	WORD wOutCardUser=pThrowResult->wOutCardUser;

	//�˿˱���
	BYTE cbThrowCardCount=pThrowResult->cbThrowCardCount;
	BYTE cbResultCardCount=pThrowResult->cbResultCardCount;
	BYTE * pcbThrowCardData=&pThrowResult->cbCardDataArray[0];
	BYTE * pcbResultCardData=&pThrowResult->cbCardDataArray[pThrowResult->cbThrowCardCount];

	//���Ƽ�¼
	m_cbOutCardCount[wOutCardUser]=cbResultCardCount;
	CopyMemory(m_cbOutCardData[wOutCardUser],pcbResultCardData,cbResultCardCount*sizeof(BYTE));

	//����˿�
	if (wOutCardUser==wMeChairID)
	{
		//�����˿�
		BYTE cbSourceCount=m_cbHandCardCount;
		m_cbHandCardCount+=cbThrowCardCount;
		CopyMemory(&m_cbHandCardData[cbSourceCount],pcbThrowCardData,sizeof(BYTE)*cbThrowCardCount);

		//�����˿�
		m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount);

		//ɾ���˿�
		cbSourceCount=m_cbHandCardCount;
		m_cbHandCardCount-=cbResultCardCount;
		m_GameLogic.RemoveCard(pcbResultCardData,cbResultCardCount,m_cbHandCardData,cbSourceCount);
	}

	//���ñ���
	m_wCurrentUser=pThrowResult->wCurrentUser;

	//����ʱ��
	if( m_wCurrentUser == wMeChairID )
	{
		UINT uElapse = TIME_LESS_OUTCARD + rand()%TIME_OUT_CARD;
		m_pIAndroidUserItem->SetGameTimer(IDI_OUT_CARD,uElapse);
	}

	return true;
}

//һ�ֽ���
bool CAndroidUserItemSink::OnSubTurnBalance(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_TurnBalance));
	if (wDataSize!=sizeof(CMD_TurnBalance)) return false;

	//��Ϣ����
	CMD_TurnBalance * pTurnBalance=(CMD_TurnBalance *)pBuffer;

	//����÷�
	WORD wWinnerUser=pTurnBalance->wTurnWinner;
	if ((wWinnerUser!=m_wBankerUser)&&((wWinnerUser+2)%GAME_PLAYER!=m_wBankerUser))
	{
		//��ȡ����
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			BYTE * pcbScoreCard=&m_cbScoreCardData[m_cbScoreCardCount];
			BYTE cbMaxCount=CountArray(m_cbScoreCardData)-m_cbScoreCardCount;
			m_cbScoreCardCount+=m_GameLogic.GetScoreCard(m_cbOutCardData[i],m_cbOutCardCount[i],pcbScoreCard,cbMaxCount);
		}
	}

	//�����û�
	m_wCurrentUser=pTurnBalance->wCurrentUser;
	m_wFirstOutUser=pTurnBalance->wCurrentUser;

	//�������
	if (m_wCurrentUser==m_pIAndroidUserItem->GetChairID())
	{
		UINT uElapse = TIME_LESS_OUTCARD + rand()%TIME_OUT_CARD;
		m_pIAndroidUserItem->SetGameTimer(IDI_OUT_CARD,uElapse);
	}

	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//��Ϸ��ʱ��
	m_pIAndroidUserItem->KillGameTimer(0);

	UINT uElapse = TIME_LESS_STARTGAME + rand()%TIME_START_GAME;
	m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,uElapse);

	return true;
}

//�����뿪
bool CAndroidUserItemSink::OnSubRequestLeave(const void *pBuffer,WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_RequestLeave));
	if (wDataSize!=sizeof(CMD_S_RequestLeave)) return false;

	//��������
	CMD_S_RequestLeave * pRequestLeave=(CMD_S_RequestLeave *)pBuffer;

	m_RequestUserAry.Add(pRequestLeave->wChairID);

	UINT nElapse = 1+rand()%TIME_RESPONSE_LEAVE;
	m_pIAndroidUserItem->SetGameTimer(IDI_RESPONSE_LEAVE,nElapse);

	return true;
}

//����
bool CAndroidUserItemSink::CallCard()
{
	//�������
	bool bCallCard = rand()%2 == 0 ? true:false;
	if( bCallCard )
	{
		//��������
		BYTE cbCallColor=m_GameLogic.GetCardColor(m_cbCallCard);
		BYTE cbCardCount[6]={0,0,0,0,0,0};
		BYTE cbCallColorArray[6]={COLOR_FANG_KUAI,COLOR_MEI_HUA,COLOR_HONG_TAO,COLOR_HEI_TAO,COLOR_NT,COLOR_NT};
		BYTE cbColorCount[6] = {0,0,0,0,0,0};
		//�˿�ͳ��
		for (BYTE i=0;i<m_cbHandCardCount;i++)
		{
			//��ȡ����
			BYTE cbCardColor=m_GameLogic.GetCardColor(m_cbHandCardData[i]);
			BYTE cbCardValue=m_GameLogic.GetCardValue(m_cbHandCardData[i]);

			//����ͳ��
			if (cbCardColor==COLOR_NT) 
			{
				cbCardCount[4+cbCardValue-0x0E]++;
				cbColorCount[4+cbCardValue-0x0E]++;
			}
			else if (cbCardValue==m_cbMainValue)
			{
				cbCardCount[cbCardColor>>4]++;
				cbColorCount[cbCardColor>>4]++;
			}
		}
		//���ƿ���
		for (BYTE i=0;i<CountArray(cbCardCount);i++)
		{
			//�����ж�
			if ( cbCardCount[i]==0 ) continue;

			//��Ŀ�ж�
			if ((i<4)&&(cbCardCount[i]>m_cbCallCount))
				continue;

			//�����ж�
			if ((i>=4)&&(cbCardCount[i]>=2)&&(cbCardCount[i]>m_cbCallCount||
				(m_cbCallCard&LOGIC_MASK_VALUE)==m_cbMainValue||i==5))
				continue;

			////��ɫ�ж�
			//BYTE cbPackCount=m_GameLogic.GetPackCount();
			//if ((cbCardCount[i]==cbPackCount)&&(cbCallColorArray[i]>cbCallColor))
			//	continue;

			cbCardCount[i] = 0;
		}
		//�����������
		BYTE byIndex = 0;
		for( BYTE i = 1; i < CountArray(cbCardCount); i++ )
		{
			if( cbCardCount[i] == 0 ) continue;

			if( cbColorCount[i] > cbColorCount[byIndex] )
				byIndex = i;
		}
		if( cbCardCount[byIndex] > 0 )
		{
			BYTE cbCallCard;
			if( byIndex == 4 ) cbCallCard = 0x4E;
			else if( byIndex == 5 ) cbCallCard = 0x4F;
			else cbCallCard = m_cbMainValue|cbCallColorArray[byIndex];

			//��������
			CMD_C_CallCard CallCard;
			CallCard.cbCallCard=cbCallCard;
			CallCard.cbCallCount=cbCardCount[byIndex];

			//��������
			m_pIAndroidUserItem->SendSocketData(SUB_C_CALL_CARD,&CallCard,sizeof(CallCard));
		}
	}
	UINT nElapse = TIME_LESS_CALLFINISH + rand()%TIME_CALL_FINISH;
	m_pIAndroidUserItem->SetGameTimer(IDI_CALL_FINISH,nElapse);
	return true;
}

//�µ���
bool CAndroidUserItemSink::SendConceal()
{
	//ɾ���˿�
	m_cbHandCardCount-=m_cbConcealCount;
	VERIFY( m_GameLogic.RemoveCard(m_cbConcealCard,m_cbConcealCount,m_cbHandCardData,m_cbHandCardCount+m_cbConcealCount) );

	//��������
	CMD_C_ConcealCard ConcealCard;
	ConcealCard.cbConcealCount=m_cbConcealCount;
	CopyMemory(ConcealCard.cbConcealCard,m_cbConcealCard,m_cbConcealCount*sizeof(BYTE));

	//��������
	m_pIAndroidUserItem->SendSocketData(SUB_C_CONCEAL_CARD,&ConcealCard,sizeof(ConcealCard));
	return true;
}

//����
bool CAndroidUserItemSink::OutCard()
{
	//��������
	tagOutCardResult OutCardResult;
	ZeroMemory(&OutCardResult,sizeof(OutCardResult));

	//��������
	BYTE cbTurnCardCount=(m_wCurrentUser==m_wFirstOutUser)?0:m_cbOutCardCount[m_wFirstOutUser];
	m_GameLogic.SearchOutCard(m_cbHandCardData,m_cbHandCardCount,m_cbOutCardData[m_wFirstOutUser],cbTurnCardCount,OutCardResult);
	ASSERT( m_wFirstOutUser==m_pIAndroidUserItem->GetChairID() ||
		m_GameLogic.EfficacyOutCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount,m_cbOutCardData[m_wFirstOutUser],
		m_cbOutCardCount[m_wFirstOutUser],m_cbHandCardData,m_cbHandCardCount) );

	//���ñ���
	m_wCurrentUser=INVALID_CHAIR;

	//��������
	CMD_C_OutCard OutCard;
	OutCard.cbCardCount=OutCardResult.cbCardCount;
	CopyMemory(OutCard.cbCardData,OutCardResult.cbResultCard,sizeof(BYTE)*OutCard.cbCardCount);
	m_pIAndroidUserItem->SendSocketData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard)-sizeof(OutCard.cbCardData)+OutCard.cbCardCount*sizeof(BYTE));

	//Ԥ��ɾ��
	BYTE cbSourceCount=m_cbHandCardCount;
	m_cbHandCardCount-=OutCard.cbCardCount;
	VERIFY( m_GameLogic.RemoveCard(OutCard.cbCardData,OutCard.cbCardCount,m_cbHandCardData,cbSourceCount) );

	return true;
}

//��������
bool CAndroidUserItemSink::OnSubBatchCard(const void *pBuffer,WORD wDataSize)
{
	//��������
	CMD_S_BatchCard * pBatchCard=(CMD_S_BatchCard *)pBuffer;
	WORD wFirstSize=sizeof(CMD_S_BatchCard)-sizeof(pBatchCard->cbCardData);

	//Ч������
	ASSERT(wDataSize>=wFirstSize);
	if (wDataSize<wFirstSize) return false;
	ASSERT(wDataSize==(wFirstSize+pBatchCard->cbCardCount*GAME_PLAYER*sizeof(BYTE)));
	if (wDataSize!=(wFirstSize+pBatchCard->cbCardCount*GAME_PLAYER*sizeof(BYTE))) return false;

	//��Ϣ����
	BYTE cbCardCount=pBatchCard->cbCardCount;
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		//��ʷ�˿�
		m_cbOutCardCount[i]=cbCardCount;
		CopyMemory(m_cbOutCardData[i],&pBatchCard->cbCardData[i*cbCardCount],cbCardCount*sizeof(BYTE));
	}

	//���ñ���
	m_wCurrentUser=INVALID_CHAIR;

	return true;
}

//////////////////////////////////////////////////////////////////////////
