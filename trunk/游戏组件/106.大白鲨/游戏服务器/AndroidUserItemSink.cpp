#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_FREE					99									//����ʱ��
#define IDI_PLACE_JETTON			101									//��עʱ��
#define IDI_DISPATCH_CARD			301									//����ʱ��
#define IDI_DEDUCE_BANKER			302

#define TIME_JETTON_ELAPSE			2									//������עһ��	
#define TIME_BANKER_ELAPSE			10
#define TIME_BANKER_ELAPSE2			30

//////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	m_pIAndroidUserItem = NULL;
	m_iMeMaxScore = 0;
	m_iApplyBankerCondition = 0;
	ZeroMemory(m_iMeAreaScoreArray, sizeof(m_iMeAreaScoreArray));
	ZeroMemory(m_iAreaLimitScore, sizeof(m_iAreaLimitScore));
	ZeroMemory(m_iAreaTotalScore, sizeof(m_iAreaTotalScore));
	m_iBankerScore = 0;
	m_wCurrentBanker = INVALID_CHAIR;
	m_bEnableSysBanker = false;
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
	ZeroMemory(m_iMeAreaScoreArray, sizeof(m_iMeAreaScoreArray));
	ZeroMemory(m_iAreaLimitScore, sizeof(m_iAreaLimitScore));
	ZeroMemory(m_iAreaTotalScore, sizeof(m_iAreaTotalScore));
	return true;
}

//ʱ����Ϣ
bool __cdecl CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch(nTimerID)
	{
	case IDI_PLACE_JETTON:
		{
			OnPlaceJetton();
			return false;
		}
	case IDI_DEDUCE_BANKER:
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_DEDUCE_BANKER);
			WORD w = INVALID_CHAIR;
			m_pIAndroidUserItem->SendSocketData(SUB_C_APPLY_BANKER, &w,sizeof(w));
			m_pIAndroidUserItem->SetGameTimer(IDI_DEDUCE_BANKER, TIME_BANKER_ELAPSE2);
			return false;
		}
	}
	return false;
}

//��Ϸ��Ϣ
bool __cdecl CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_FREE:		//��Ϸ����
		{
			return OnSubGameFree(pBuffer,wDataSize);
		}
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_PLACE_JETTON:	//�û���ע
		{
			return OnSubPlaceJetton(pBuffer,wDataSize);
		}
	case SUB_S_APPLY_BANKER:	//������ׯ
		{
			return true;
		}
	case SUB_S_CANCEL_BANKER:	//ȡ����ׯ
		{
			return true;
		}
	case SUB_S_CHANGE_BANKER:	//�л�ׯ��
		{
			return OnSubChangeBanker(pBuffer, wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_SEND_RECORD:		//��Ϸ��¼
		{
			return true;
		}
	case SUB_S_PLACE_JETTON_FAIL:	//��עʧ��
		{
			return OnSubPlaceJettonFail(pBuffer,wDataSize);
		}
	}

	//�������
	//ASSERT(FALSE);

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
	case GS_FREE:			//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) 
				return false;

			//��Ϣ����
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;
			if( pStatusFree == NULL )
				return FALSE;

			//�����Ϣ
			m_iMeMaxScore=pStatusFree->lUserMaxScore;


			//ׯ����Ϣ
			SetBankerInfo(pStatusFree->wBankerUser,pStatusFree->lBankerScore);
			m_bEnableSysBanker=pStatusFree->bEnableSysBanker;

			//������Ϣ
			m_iApplyBankerCondition=pStatusFree->lApplyBankerCondition;
			for( int i=0; i<JETTON_AREA_COUNT; i++ )
				m_iAreaLimitScore[i] = pStatusFree->lAreaLimitScore;

			m_pIAndroidUserItem->SetGameTimer(IDI_DEDUCE_BANKER, TIME_BANKER_ELAPSE);
	
			//����״̬
			m_pIAndroidUserItem->SetGameStatus(GS_FREE);
			return true;
		}
	case GS_PLACE_JETTON:	//��ע״̬
	case GS_GAME_END:		//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) 
				return false;

			//��Ϣ����
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;
			if( pStatusPlay == NULL )
				return FALSE;

			//ȫ����ע
			for( int i=0; i<JETTON_AREA_COUNT; i++)
				m_iAreaTotalScore[i] = pStatusPlay->lAreaLimitScore;
			LONG  lUserAreaScore[JETTON_AREA_COUNT]={pStatusPlay->lUserBigSharkScore,pStatusPlay->lUserFeiQinScore,pStatusPlay->lUserZouShouScore,
													pStatusPlay->lUserYanZiScore,pStatusPlay->lUserTuZiScore,
													pStatusPlay->lUserGeZiScore,pStatusPlay->lUserXiongMaoScore,
													pStatusPlay->lUserKongQueScore,pStatusPlay->lUserHouZiScore,
													pStatusPlay->lUserLaoYingScore,pStatusPlay->lUserShiZiScore};
									
			//�����ע
			for( int i=0; i<JETTON_AREA_COUNT; i++ )
				SetMePlaceJetton(i, lUserAreaScore[i]);

			//��һ���
			m_iMeMaxScore=pStatusPlay->lUserMaxScore;			
		
			//������Ϣ
			m_iApplyBankerCondition=pStatusPlay->lApplyBankerCondition;
			for( int i=0; i<JETTON_AREA_COUNT; i++ )
				m_iAreaLimitScore[i] = pStatusPlay->lAreaLimitScore;


			//ׯ����Ϣ
			SetBankerInfo(pStatusPlay->wBankerUser,pStatusPlay->lBankerScore);
			m_bEnableSysBanker=pStatusPlay->bEnableSysBanker;
		
			//����״̬
			m_pIAndroidUserItem->SetGameStatus(pStatusPlay->cbGameStatus);

			//����ʱ��
			int cbTimeID = 0;
			int  nElapse = 0;
			if( pStatusPlay->cbGameStatus == GS_GAME_END )
			{
				cbTimeID = IDI_DISPATCH_CARD;
				nElapse = pStatusPlay->cbTimeLeave;
			}
			else
			{
				cbTimeID = IDI_PLACE_JETTON;
				nElapse = TIME_JETTON_ELAPSE;
			}
			m_pIAndroidUserItem->SetGameTimer(cbTimeID, nElapse);
			return true;
		}
	}

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

// ������Ҷ���ʼ��
void __cdecl CAndroidUserItemSink::OnEventAllUserStart(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return ;
}


/*****************************************************************************************************************
*��Ϸ���к������忪ʼ
*****************************************************************************************************************/
//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) 
		return false;

	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;
	if( pGameStart == NULL )
		return FALSE;

	ZeroMemory(m_iMeAreaScoreArray, sizeof(m_iMeAreaScoreArray));
	ZeroMemory(m_iAreaLimitScore, sizeof(m_iAreaLimitScore));
	ZeroMemory(m_iAreaTotalScore, sizeof(m_iAreaTotalScore));

	//ׯ����Ϣ
	SetBankerInfo(pGameStart->wBankerUser,pGameStart->lBankerScore);

	//�����Ϣ
	m_iMeMaxScore=pGameStart->lUserMaxScore;
		

	//���ø���ע�����ʼ���·�

	//����ׯ�ҽ�����������������ĳ�ʼ�����·�
	int iOddsArray[JETTON_AREA_COUNT] = {40,5,30,5,20,5,10,5};
	
	for( int i=0; i<0; i++ )
		m_iAreaLimitScore[i] = __min(pGameStart->lBankerScore/(iOddsArray[i]+1), m_iAreaLimitScore[i] );

	//����ʱ��
	m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON, TIME_JETTON_ELAPSE);

	//����״̬
	m_pIAndroidUserItem->SetGameStatus(GS_PLACE_JETTON);
	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
	m_pIAndroidUserItem->KillGameTimer(IDI_PLACE_JETTON);
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) 
		return false;

	//��Ϣ����
	CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;
	if( pGameFree == NULL )
		return false;

	//����״̬
	m_pIAndroidUserItem->SetGameStatus(GS_FREE);
	return true;
}

//�û���ע�ɹ�
bool CAndroidUserItemSink::OnSubPlaceJetton(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJetton));
	if (wDataSize!=sizeof(CMD_S_PlaceJetton)) 
		return false;

	//��Ϣ����
	CMD_S_PlaceJetton * pPlaceJetton=(CMD_S_PlaceJetton *)pBuffer;
	if( pPlaceJetton == NULL )
		return false;
	if( pPlaceJetton->cbJettonArea>ID_SHI_ZI || pPlaceJetton->cbJettonArea<ID_BIG_SHARK )
		return false;

	//�޸���ע��¼����
	m_iAreaTotalScore[pPlaceJetton->cbJettonArea] += pPlaceJetton->lJettonScore;
	m_iAreaLimitScore[pPlaceJetton->cbJettonArea] -= pPlaceJetton->lJettonScore;

	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) 
		return false;

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	if( pGameEnd == NULL )
		return FALSE;
	
	m_pIAndroidUserItem->KillGameTimer(IDI_PLACE_JETTON);

	//ׯ����Ϣ
	m_iBankerScore = pGameEnd->lBankerTotallScore;

	//������Ϣ
	m_iMeMaxScore += pGameEnd->lUserScore;
	
	//����״̬
	m_pIAndroidUserItem->SetGameStatus(GS_GAME_END);
	return true;
}


//�л�ׯ��
bool CAndroidUserItemSink::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) 
		return false;

	//��Ϣ����
	CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pBuffer;
	if( pChangeBanker == NULL )
		return FALSE;


	//ׯ����
	SetBankerInfo(pChangeBanker->wBankerUser,pChangeBanker->lBankerScore);

	if( m_iMeMaxScore >= m_iApplyBankerCondition)
	{
		WORD w = INVALID_CHAIR;
		m_pIAndroidUserItem->SendSocketData(SUB_C_APPLY_BANKER, &w, sizeof(WORD));
	}

	return true;
}

//��עʧ��
bool CAndroidUserItemSink::OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJettonFail));
	if (wDataSize!=sizeof(CMD_S_PlaceJettonFail)) 
		return false;

	//��Ϣ����
	CMD_S_PlaceJettonFail * pPlaceJettonFail=(CMD_S_PlaceJettonFail *)pBuffer;
	if( pPlaceJettonFail == NULL )
		return FALSE;

	//Ч�����
	BYTE cbViewIndex=pPlaceJettonFail->lJettonArea;	//ȡ����ע����
	ASSERT(cbViewIndex<=ID_SHI_ZI);
	if (cbViewIndex>ID_SHI_ZI) 

		return false;

	//��ע����
	LONG iJettonCount=pPlaceJettonFail->lPlaceScore;
	
	m_iMeAreaScoreArray[cbViewIndex] -= iJettonCount;
	return true;
}

//����ׯ��
void CAndroidUserItemSink::SetBankerInfo(WORD wBanker,LONG iScore)
{
	m_wCurrentBanker=wBanker;
	m_iBankerScore=iScore;
}

//������ע
void CAndroidUserItemSink::SetMePlaceJetton(BYTE cbViewIndex, LONG iJettonCount)
{
	//Ч�����
	ASSERT(cbViewIndex<= ID_SHI_ZI);
	if (cbViewIndex>ID_SHI_ZI) 
		return;

	m_iMeAreaScoreArray[cbViewIndex] = iJettonCount;
}

//��ע��Ϣ
void CAndroidUserItemSink::OnPlaceJetton( )
{
	//��������
	BYTE cbJettonArea=0xFF;			//��ע����
	bool bCanJettonArray[6]={false,false,false,false,false,false};	//��Щ��������ע
	LONG iCurJetton = 0;			//������ע�ĳ����С
	int	nCanJettonNum = 0;			//����ע���������
	int nIndex = 0;					//�����������ע���������±�
	LONG iLeaveScore = 0;		//���˻�ʣ���ٳ���
	LONG	iTemp = 0;
	int nLoop = 0;

	//���㱾��ʣ��Ľ��
	for( int i=0; i<JETTON_AREA_COUNT; i++ )
		iTemp += m_iMeAreaScoreArray[i];
	iLeaveScore = m_iMeMaxScore - iTemp;


	//���ҵ�ǰ����������������¶��ĳ���
	LONG iScoreJetton[]={100L,1000L,10000L,100000L,1000000L,5000000L};

	//�����ϵͳ��ׯ���û����˳�������ע�����������ֻ����ǰ���ֳ���
	if( m_wCurrentBanker == INVALID_CHAIR && m_bEnableSysBanker )
		nLoop = 6;
	else		
		nLoop = 3;

	for( int i=0; i<nLoop; i++)
	{
		if( iLeaveScore >= iScoreJetton[i] )
		{
			bCanJettonArray[i] = true;
			nCanJettonNum++;
		}
	}

	//���ȡ��һ������
	if(nCanJettonNum <= 0 || nCanJettonNum >nLoop)	//ûǮ��
	{
		return;
	}
	nIndex = rand()%nCanJettonNum;
	if( ! bCanJettonArray[nIndex])
	{
		return ;
	}
	iCurJetton = iScoreJetton[rand()%nCanJettonNum];

	//���ȡ��һ����ע����
	cbJettonArea = rand()%JETTON_AREA_COUNT+1;
	if( cbJettonArea>ID_SHI_ZI || cbJettonArea<ID_BIG_SHARK )
	{
		return;
	}

	IServerUserItem * pUserItem = m_pIAndroidUserItem->GetMeUserItem();
	if( pUserItem == NULL )
	{
		return;
	}

	//ׯ�Ҳ�����ע
	if ( pUserItem->GetChairID() == m_wCurrentBanker )
	{
		return;
	}

	//������ע�������ñ�����ע����
	m_iMeAreaScoreArray[cbJettonArea] += iCurJetton;

	//��������
	CMD_C_PlaceJetton PlaceJetton;
	ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

	//�������
	PlaceJetton.cbJettonArea=cbJettonArea;
	PlaceJetton.lJettonScore=iCurJetton;

	//����������Ϣ
	m_pIAndroidUserItem->SendSocketData(SUB_C_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));

	if( m_pIAndroidUserItem->GetGameStatus() == GS_PLACE_JETTON)
	{
		int nElapse = rand()%5;
		nElapse = __max(nElapse, 1);
		m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON, nElapse);
	}
	return;
}
/*****************************************************************************************************************
*��Ϸ���к����������
*****************************************************************************************************************/


//////////////////////////////////////////////////////////////////////////
