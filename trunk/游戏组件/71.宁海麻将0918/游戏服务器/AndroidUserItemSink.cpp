#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//test
//#define TEST

//����ʱ��
#define TIME_LESS					1									//����ʱ��

//��Ϸʱ��
#ifndef TEST
#define TIME_START_GAME				5									//��ʼʱ��
#define TIME_OPERATE_CARD			3									//������ʱ��
#define TIME_HEAR_STATUS			2									//����ʱ��
#else
#define TIME_START_GAME				3									//��ʼʱ��
#define TIME_OPERATE_CARD			3									//������ʱ��
#define TIME_HEAR_STATUS			3									//����ʱ��
#endif

//��Ϸʱ��
#define IDI_START_GAME				(IDI_ANDROID_ITEM_SINK+1)			//��ʼʱ��
#define IDI_OPERATE_CARD			(IDI_ANDROID_ITEM_SINK+11)			//������ʱ��

//////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//״̬����
	m_bHearStatus=false;
	m_bActionMask = WIK_NULL;
	m_bActionCard = 0;

	//������Ϣ
	m_cbEnjoinCardCount = 0;
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbEnjoinCardData,sizeof(m_cbEnjoinCardData));	
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�˿˱���
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));

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
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//״̬����
	m_bHearStatus=false;
	m_bActionMask = WIK_NULL;
	m_bActionCard = 0;

	//������Ϣ
	m_cbEnjoinCardCount = 0;
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbEnjoinCardData,sizeof(m_cbEnjoinCardData));	
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�˿˱���
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));

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
	case IDI_OPERATE_CARD:		//�û�����
		{
			tagOutCardResult OutCardResult;

			ASSERT( m_wCurrentUser == INVALID_CHAIR || m_pIAndroidUserItem->GetChairID() == m_wCurrentUser );
			if( m_wCurrentUser != INVALID_CHAIR && m_pIAndroidUserItem->GetChairID() != m_wCurrentUser )
				return true;
			if( m_GameLogic.SearchOutCard(m_pIAndroidUserItem->GetChairID(),m_wCurrentUser,
				m_cbCardIndex,m_WeaveItemArray,m_cbWeaveCount,m_cbDiscardCard,m_cbDiscardCount,m_bActionMask,m_bActionCard,
				m_cbEnjoinCardData,m_cbEnjoinCardCount,OutCardResult) )
			{
				if( OutCardResult.cbOperateCode != WIK_NULL )
				{
					OnOperateCard(OutCardResult.cbOperateCode,OutCardResult.cbOperateCard);
				}else
				{				
					ASSERT( m_cbCardIndex[m_pIAndroidUserItem->GetChairID()][m_GameLogic.SwitchToCardIndex(OutCardResult.cbOperateCard)] > 0 );
					OnOutCard(OutCardResult.cbOperateCard);
				}
			}else
			{
				OnOperateCard(WIK_NULL,0);
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
	case SUB_S_OUT_CARD:		//�û�����
		{
			return OnSubOutCard(pData,wDataSize);
		}
	case SUB_S_SEND_CARD:		//������Ϣ
		{
			return OnSubSendCard(pData,wDataSize);
		}
	case SUB_S_OPERATE_NOTIFY:	//������ʾ
		{
			return OnSubOperateNotify(pData,wDataSize);
		}
	case SUB_S_OPERATE_RESULT:	//�������
		{
			return OnSubOperateResult(pData,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pData,wDataSize);
		}
	case SUB_S_FORCE_OUT_CARD:	//ǿ�ȳ���
		{
			return OnSubForceOutCard(pData,wDataSize);
		}
	case SUB_S_FORCE_SEND_CARD:
		{
			return OnSubForceSendCard(pData,wDataSize);
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
	case GS_MJ_FREE:	//����״̬
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
	case GS_MJ_PLAY:		//��Ϸ״̬
		{
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
	if (wDataSize!=sizeof(CMD_S_GameStart))	return false;

	//��������
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pData;

	//����״̬
	m_pIAndroidUserItem->SetGameStatus(GS_MJ_PLAY);

	//���ñ���
	m_wBankerUser=pGameStart->wBankerUser;
	m_wCurrentUser=pGameStart->wCurrentUser;
	
	//�׷�����
	if(pGameStart->bFirst)
	{
		m_cbLeftCardCount=MAX_REPERTORY-GAME_PLAYER*(MAX_COUNT-1)-1;
		ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
		ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));
		//������Ϣ
		m_cbOutCardData=0;
		m_wOutCardUser=INVALID_CHAIR;
		m_cbEnjoinCardCount = 0;
		ZeroMemory(m_cbEnjoinCardData,sizeof(m_cbEnjoinCardData));
		//����˿�
		ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
		ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
		ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
		//״̬��Ϣ
		m_bHearStatus = false;
		m_bActionCard = 0;
		m_bActionMask = WIK_NULL;
	}

	//�����˿�
	if( pGameStart->bFirst )
	{
		WORD wMeChairId = m_pIAndroidUserItem->GetChairID();
		BYTE cbCardCount = (wMeChairId==m_wBankerUser)?MAX_COUNT:(MAX_COUNT-1);
		m_GameLogic.SwitchToCardIndex(pGameStart->cbCardData,cbCardCount,m_cbCardIndex[wMeChairId]);
		BYTE bIndex = 1;
		for( WORD i = 0; i < GAME_PLAYER; i++ )
		{
			if( i == wMeChairId ) continue;
			cbCardCount=(i==m_wBankerUser)?MAX_COUNT:(MAX_COUNT-1);
			m_GameLogic.SwitchToCardIndex(&pGameStart->cbCardData[MAX_COUNT*bIndex++],cbCardCount,m_cbCardIndex[i]);
		}
		//����˿�
		CopyMemory(m_cbRepertoryCard,pGameStart->cbRepertoryCard,sizeof(m_cbRepertoryCard));
	}
	

	//��������
	if ( m_wCurrentUser==m_pIAndroidUserItem->GetChairID() )
	{
		m_bActionMask = pGameStart->cbUserAction;
		m_bActionCard = 0;
		UINT nElapse = rand()%TIME_OPERATE_CARD+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_OPERATE_CARD,nElapse);
	}

	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(void * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//����״̬
	m_pIAndroidUserItem->SetGameStatus(GS_MJ_FREE);

	//ɾ����ʱ��
	m_pIAndroidUserItem->KillGameTimer(IDI_OPERATE_CARD);

	//����
	UINT nElapse = rand()%TIME_START_GAME+TIME_LESS;
	m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);

	return true;
}

//�û�����
bool CAndroidUserItemSink::OnSubOutCard( const void *pBuffer,WORD wDataSize )
{
	//Ч����Ϣ
	ASSERT(wDataSize==sizeof(CMD_S_OutCard));
	if (wDataSize!=sizeof(CMD_S_OutCard)) return false;


	//��Ϣ����
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;

	//��������
	WORD wMeChairID=m_pIAndroidUserItem->GetChairID();

	//���ñ���
	m_wCurrentUser=INVALID_CHAIR;
	m_wOutCardUser=pOutCard->wOutCardUser;
	ASSERT( pOutCard->cbOutCardData != 0 );
	m_cbOutCardData=pOutCard->cbOutCardData;

	//��ǰ�û�
	if(pOutCard->wOutCardUser == wMeChairID)
	{
		m_cbEnjoinCardCount=0;
		ZeroMemory(m_cbEnjoinCardData,sizeof(m_cbEnjoinCardData));
	}

	//ɾ���˿�
	if( wMeChairID != pOutCard->wOutCardUser )
		m_GameLogic.RemoveCard(m_cbCardIndex[pOutCard->wOutCardUser],pOutCard->cbOutCardData);

	return true;
}


//�û�����
bool CAndroidUserItemSink::OnSubSendCard( const void *pBuffer,WORD wDataSize )
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//��������
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	//���ñ���
	WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
	m_wCurrentUser=pSendCard->wCurrentUser;

	//�����˿�
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		//�����˿�
		m_cbDiscardCard[m_wOutCardUser][m_cbDiscardCount[m_wOutCardUser]++] = m_cbOutCardData;
		//���ñ���
		m_cbOutCardData=0;
		m_wOutCardUser=INVALID_CHAIR;
	}

	//���ƴ���
	if (pSendCard->cbCardData!=0)
	{
		m_cbCardIndex[m_wCurrentUser][m_GameLogic.SwitchToCardIndex(pSendCard->cbCardData)]++;	
		//�۳��˿�
		m_cbLeftCardCount--;
	}

	//����ʱ��
	if( m_pIAndroidUserItem->GetChairID() == m_wCurrentUser )
	{
		m_bActionMask = pSendCard->cbActionMask;
		m_bActionCard = pSendCard->cbCardData;
		//����ʱ��
		UINT nElapse=rand()%TIME_OPERATE_CARD+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_OPERATE_CARD,nElapse);
	}

	return true;
}


//����֪ͨ
bool CAndroidUserItemSink::OnSubOperateNotify( const void *pBuffer,WORD wDataSize )
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_OperateNotify));
	if (wDataSize!=sizeof(CMD_S_OperateNotify)) return false;

	//��������
	CMD_S_OperateNotify * pOperateNotify=(CMD_S_OperateNotify *)pBuffer;

	//�û�����
	if ((pOperateNotify->cbActionMask!=WIK_NULL))
	{
		ASSERT( m_wCurrentUser == INVALID_CHAIR );
		//��ȡ����
		WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
		m_bActionMask = pOperateNotify->cbActionMask;
		m_bActionCard = pOperateNotify->cbActionCard;

		//����ʱ��
		UINT nElapse = rand()%TIME_OPERATE_CARD+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_OPERATE_CARD,nElapse);
	}

	return true;
}


//�������֪ͨ
bool CAndroidUserItemSink::OnSubOperateResult( const void *pBuffer,WORD wDataSize )
{
	//Ч����Ϣ
	ASSERT(wDataSize==sizeof(CMD_S_OperateResult));
	if (wDataSize!=sizeof(CMD_S_OperateResult)) return false;

	//��Ϣ����
	CMD_S_OperateResult * pOperateResult=(CMD_S_OperateResult *)pBuffer;

	//��������
	BYTE cbPublicCard=TRUE;
	WORD wOperateUser=pOperateResult->wOperateUser;
	BYTE cbOperateCard=pOperateResult->cbOperateCard;

	//ɾ����ʱ��
	m_pIAndroidUserItem->KillGameTimer(IDI_OPERATE_CARD);

	//���Ʊ���
	if (pOperateResult->cbOperateCode!=WIK_NULL)
	{
		m_cbOutCardData=0;
		m_wOutCardUser=INVALID_CHAIR;
	}

	//���ñ���
	m_bActionMask = WIK_NULL;
	m_bActionCard = 0;

	//
	WORD wMeChairID=m_pIAndroidUserItem->GetChairID();

	//�������
	if ((pOperateResult->cbOperateCode&WIK_GANG)!=0)
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

		//�˿�����
		m_cbCardIndex[wOperateUser][m_GameLogic.SwitchToCardIndex(pOperateResult->cbOperateCard)]=0;

		if ((wMeChairID == wOperateUser)&&(m_cbWeaveCount[wMeChairID]>2))
		{
			m_cbEnjoinCardCount=0;
			ZeroMemory(m_cbEnjoinCardData,sizeof(m_cbEnjoinCardData));
		}

	}
	else if (pOperateResult->cbOperateCode!=WIK_NULL&&(pOperateResult->cbOperateCode&WIK_CHI_HU)==0)
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

		//ɾ���˿�
		m_GameLogic.RemoveCard(cbWeaveCard,cbWeaveCardCount,&cbOperateCard,1);
		m_GameLogic.RemoveCard(m_cbCardIndex[wOperateUser],cbWeaveCard,cbWeaveCardCount-1);

		//���ƴ���
		if ((m_cbWeaveCount[wMeChairID]>2)&&(wMeChairID == wOperateUser))
		{
			m_cbEnjoinCardCount=0;
			ZeroMemory(m_cbEnjoinCardData,sizeof(m_cbEnjoinCardData));
		}
		else if((m_cbWeaveCount[wMeChairID]<3)&&(wMeChairID == wOperateUser))
		{
			//��ֹ����
			if(cbWeaveKind == WIK_LEFT)  //���
			{
				//��ֹ��������
				m_cbEnjoinCardData[m_cbEnjoinCardCount] =cbOperateCard;
				m_cbEnjoinCardCount++;
				if((cbOperateCard&MASK_VALUE)<7)
				{
					m_cbEnjoinCardData[m_cbEnjoinCardCount] =cbOperateCard+3;
					m_cbEnjoinCardCount++;
				}
			}
			else if (cbWeaveKind == WIK_CENTER) //�г�
			{
				m_cbEnjoinCardData[m_cbEnjoinCardCount] =cbOperateCard;
				m_cbEnjoinCardCount++;
			}
			else  if(cbWeaveKind == WIK_RIGHT)//�ҳ�
			{
				m_cbEnjoinCardData[m_cbEnjoinCardCount] =cbOperateCard;
				m_cbEnjoinCardCount++;
				if((cbOperateCard&MASK_VALUE)>3)
				{
					m_cbEnjoinCardData[m_cbEnjoinCardCount] =cbOperateCard-3;
					m_cbEnjoinCardCount++;
				}
			}
		}
	}

	//����ʱ��
	if (m_wCurrentUser==m_pIAndroidUserItem->GetChairID())
	{
		//����ʱ��
		UINT nElapse=rand()%TIME_OPERATE_CARD+TIME_LESS;
		//����ʱ��
		m_pIAndroidUserItem->SetGameTimer(IDI_OPERATE_CARD,nElapse);
	}

	return true;
}

//ǿ�ȳ���
bool CAndroidUserItemSink::OnSubForceOutCard(const void *pBuffer,WORD wDataSize)
{
	//Ч����Ϣ
	ASSERT(wDataSize==sizeof(CMD_S_OutCard));
	if (wDataSize!=sizeof(CMD_S_OutCard))		
		return false;

	//��Ϣ����
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;

	//���ñ���
	m_wCurrentUser=INVALID_CHAIR;
	m_wOutCardUser=pOutCard->wOutCardUser;
	m_cbOutCardData=pOutCard->cbOutCardData;

	VERIFY( m_GameLogic.RemoveCard(m_cbCardIndex[m_wOutCardUser],m_cbOutCardData) );

	return true;
}

//ǿ�Ʒ���
bool CAndroidUserItemSink::OnSubForceSendCard(const void *pBuffer,WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard))		return false;

	//��������
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	//���ñ���
	WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
	m_wCurrentUser=pSendCard->wCurrentUser;

	//�����˿�
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		//�����˿�
		m_cbDiscardCard[m_wOutCardUser][m_cbDiscardCount[m_wOutCardUser]++] = m_cbOutCardData;
		//���ñ���
		m_cbOutCardData=0;
		m_wOutCardUser=INVALID_CHAIR;
	}
	//���ƴ���
	if (pSendCard->cbCardData!=0)
	{
		m_cbCardIndex[m_wCurrentUser][m_GameLogic.SwitchToCardIndex(pSendCard->cbCardData)]++;	
		//�۳��˿�
		m_cbLeftCardCount--;
	}

	return true;
}


//����
void CAndroidUserItemSink::OnOutCard( BYTE cbOutCard )
{
	//���ñ���
	m_wCurrentUser=INVALID_CHAIR;
	m_GameLogic.RemoveCard(m_cbCardIndex[m_pIAndroidUserItem->GetChairID()],cbOutCard);

	//���ý���
	m_pIAndroidUserItem->KillGameTimer(IDI_OPERATE_CARD);

	//��������
	CMD_C_OutCard OutCard;
	OutCard.cbCardData=cbOutCard;
	if( !m_pIAndroidUserItem->SendSocketData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard)) )
	{
		ASSERT( FALSE );
		return ;
	}

	//���ñ���
	m_bActionMask = WIK_NULL;
	m_bActionCard = 0;
	
	return ;
}



//������
void CAndroidUserItemSink::OnOperateCard( BYTE cbOperateCode,BYTE cbOperateCard )
{
	//ɾ��ʱ��
	m_pIAndroidUserItem->KillGameTimer(IDI_OPERATE_CARD);

	//��������
	CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=cbOperateCode;
	OperateCard.cbOperateCard=cbOperateCard;
	if( !m_pIAndroidUserItem->SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard)) )
	{
		ASSERT( FALSE );
		return ;
	}

	//���ñ���
	m_bActionMask = WIK_NULL;
	m_bActionCard = 0;

	return ;
}


//////////////////////////////////////////////////////////////////////////
