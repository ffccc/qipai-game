#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_FREE					99									//����ʱ��
#define IDI_PLACE_JETTON			100									//��עʱ��
#define IDI_DISPATCH_CARD			301									//����ʱ��

#define IDI_GAME_ACTION				105									//��Ϸʱ��

//////////////////////////////////////////////////////////////////////////

static const LONGLONG lScoreArray[]={100L,1000L,10000L,100000L,1000000L,5000000L};

//////////////////////////////////////////////////////////////////////////


BOOL CAndroidUserItemSink::m_bAllowApplyBanker=TRUE;
WORD CAndroidUserItemSink::m_wMaxBankerTime=0;
INT CAndroidUserItemSink::m_nMaxJettonRange=0;
CString CAndroidUserItemSink::m_strConfigFile;
BOOL CAndroidUserItemSink::m_bLoadConfig=TRUE;

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	m_lAreaLimitScore=0L;
	m_lUserLimitScore=0L;
	m_lApplyBankerCondition=0L;
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	m_lMeMaxScore=0L;
	m_lBankerScore=0L;
	m_wBankerUser=INVALID_CHAIR;
	m_bEnableSysBanker=false;

	m_cbJettonArea=0;

	m_bMeCurrentBanker=false;
	m_bApplyingBanker=false;
	m_bCancelingBanker=false;

	m_wRandBankerTime=0;

	LoadConfig();
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
	m_bMeCurrentBanker=false;
	m_bApplyingBanker=false;
	m_bCancelingBanker=false;

	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));
	return true;
}

//ʱ����Ϣ
bool __cdecl CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (m_pIAndroidUserItem->GetGameStatus())
	{
	case GS_FREE:
		{
			if (m_bMeCurrentBanker==false)	//������ׯ
			{
				if (m_bApplyingBanker==true) return false;

				const tagUserScore *pUserScore=m_pIAndroidUserItem->GetMeUserItem()->GetUserScore();
				ASSERT(pUserScore!=NULL);
				if (pUserScore->lScore < m_lApplyBankerCondition) return false;

				m_pIAndroidUserItem->SendSocketData(SUB_C_APPLY_BANKER,NULL,0);
				m_bApplyingBanker=true;
			}
			else if (m_bMeCurrentBanker==true)
			{
				if (m_bCancelingBanker==true) return false;
				m_pIAndroidUserItem->SendSocketData(SUB_C_CANCEL_BANKER,NULL,0);
				m_bCancelingBanker=true;
			}
			return true;
		}
	case GS_PLACE_JETTON:
		{
			if (m_wJettonCount--==0) return false;

			int nMaxIndex=0;
			for (int i=0; i < CountArray(lScoreArray); i++)
				if (m_lMeMaxScore>lScoreArray[i]) nMaxIndex=i;

			//��������
			if (nMaxIndex>m_nMaxJettonRange) nMaxIndex=m_nMaxJettonRange;

			LONGLONG lJettonScore=lScoreArray[rand()%nMaxIndex];
			ASSERT(lJettonScore<=m_lMeMaxScore);

			if (lJettonScore>GetUserMaxJetton()) return false;
			TRACE("Jetton Count=%d Area=%d Score=%I64d\n", m_wJettonCount,m_cbJettonArea,lJettonScore);

			//���ñ���
			m_lUserJettonScore[m_cbJettonArea]+=lJettonScore;

			//������Ϣ
			CMD_C_PlaceJetton PlaceJetton;
			PlaceJetton.cbJettonArea=m_cbJettonArea;
			PlaceJetton.lJettonScore=lJettonScore;

			m_pIAndroidUserItem->SendSocketData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));

			UINT nElapse=rand()%4+1;
			m_pIAndroidUserItem->SetGameTimer(IDI_GAME_ACTION,nElapse);

			return true;
		}
	case GS_GAME_END:
		{
			return true;
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
			return OnSubUserApplyBanker(pBuffer, wDataSize);
		}
	case SUB_S_CANCEL_BANKER:	//ȡ����ׯ
		{
			return OnSubUserCancelBanker(pBuffer, wDataSize);
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
			return OnSubGameRecord(pBuffer,wDataSize);
		}
	case SUB_S_PLACE_JETTON_FAIL:	//��עʧ��
		{
			return OnSubPlaceJettonFail(pBuffer,wDataSize);
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
	switch(cbGameStatus)
	{
	case GS_FREE:
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��Ϣ����
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;
			m_lApplyBankerCondition=pStatusFree->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusFree->lAreaLimitScore;
			m_wBankerUser=pStatusFree->wBankerUser;
			m_wBankerTime=pStatusFree->cbBankerTime;
			m_lBankerScore=pStatusFree->lBankerScore;
			m_lBankerWinScore=pStatusFree->lBankerWinScore;
			m_bEnableSysBanker=pStatusFree->bEnableSysBanker;

			if (bLookonOther==false)
			{
				m_lMeMaxScore=pStatusFree->lUserMaxScore;
			}

			return true;

		}
	case GS_PLAYING:
	case GS_GAME_END:
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//��Ϣ����
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;
			m_lApplyBankerCondition=pStatusPlay->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusPlay->lAreaLimitScore;
			m_wBankerUser=pStatusPlay->wBankerUser;
			m_wBankerTime=pStatusPlay->cbBankerTime;
			m_lBankerScore=pStatusPlay->lBankerScore;
			m_lBankerWinScore=pStatusPlay->lBankerWinScore;
			m_bEnableSysBanker=pStatusPlay->bEnableSysBanker;

			CopyMemory(m_lAllJettonScore,pStatusPlay->lAllJettonScore,sizeof(m_lAllJettonScore));

			if (bLookonOther==false)
			{
				m_lMeMaxScore=pStatusPlay->lUserMaxScore;
				CopyMemory(m_lUserJettonScore,pStatusPlay->lUserJettonScore,sizeof(m_lUserJettonScore));
			}

			return true;
		}
	}
	return true;
}

bool CAndroidUserItemSink::OnSubGameStart( const void * pBuffer, WORD wDataSize )
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//����״̬
	m_pIAndroidUserItem->SetGameStatus(GS_PLACE_JETTON);

	//���ñ���
	m_lMeMaxScore=pGameStart->lUserMaxScore;
	m_wBankerUser=pGameStart->wBankerUser;
	m_lBankerScore=pGameStart->lBankerScore;

	//��ע����
	if (m_lMeMaxScore>2000L && m_wBankerUser!=INVALID_CHAIR && m_wBankerUser!=m_pIAndroidUserItem->GetChairID())
	{
		//�������
		static DWORD dwRandCount=0;
		srand((unsigned)time(NULL)+dwRandCount);
		dwRandCount++;

		m_cbJettonArea=rand()%AREA_COUNT+1;
		m_wJettonCount=rand()%10+3;
		
		ASSERT(m_cbJettonArea<=ID_JIAO_R && m_cbJettonArea>=ID_SHUN_MEN);
		TRACE("Android begin jetton area %d count %d........\n", m_cbJettonArea, m_wJettonCount);

		UINT nElapse=rand()%2+1;
		m_pIAndroidUserItem->SetGameTimer(IDI_GAME_ACTION,nElapse);
	}
	

	m_bLoadConfig=TRUE;

	return true;
}

bool CAndroidUserItemSink::OnSubGameFree( const void * pBuffer, WORD wDataSize )
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) return false;

	//��Ϣ����
	CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;

	m_pIAndroidUserItem->SetGameStatus(GS_FREE);

	IServerUserItem *pServerUserItem=m_pIAndroidUserItem->GetMeUserItem();
	ASSERT(pServerUserItem!=NULL);

	if (m_wBankerUser==INVALID_CHAIR||m_wBankerUser!=pServerUserItem->GetChairID())
		m_bMeCurrentBanker=false;

	
	if (pServerUserItem->GetUserStatus()!=US_LOOKON)
	{
		//��ׯ����
		if (m_bApplyingBanker==false && m_bMeCurrentBanker==false && m_bAllowApplyBanker==TRUE && 
			pServerUserItem->GetUserScore()->lScore > m_lApplyBankerCondition)
		{
			m_wRandBankerTime=rand()%m_wMaxBankerTime+1;
			UINT nElapse=rand()%3+1;
			m_pIAndroidUserItem->SetGameTimer(IDI_GAME_ACTION, nElapse);
			TRACE0("Android apply banker........\n");
		}
		else if (m_bMeCurrentBanker==true && m_bCancelingBanker==false)
		{
			if (m_wBankerTime>=m_wRandBankerTime)
			{
				UINT nElapse=rand()%3+1;
				m_pIAndroidUserItem->SetGameTimer(IDI_GAME_ACTION, nElapse);
				TRACE0("Android cancel banker........\n");
			}
		}
	}

	//װ������
	CAndroidUserItemSink::LoadConfig();

	return true;
}

bool CAndroidUserItemSink::OnSubPlaceJetton( const void * pBuffer, WORD wDataSize )
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJetton));
	if (wDataSize!=sizeof(CMD_S_PlaceJetton)) return false;

	//��Ϣ����
	CMD_S_PlaceJetton * pPlaceJetton=(CMD_S_PlaceJetton *)pBuffer;

	ASSERT(pPlaceJetton->cbJettonArea<=ID_JIAO_R && pPlaceJetton->cbJettonArea>=ID_SHUN_MEN);
	ASSERT(pPlaceJetton->wChairID!=INVALID_CHAIR);
	ASSERT(pPlaceJetton->lJettonScore>=lScoreArray[0] && pPlaceJetton->lJettonScore<=lScoreArray[5]);

	//���ñ���
	m_lAllJettonScore[pPlaceJetton->cbJettonArea]+=pPlaceJetton->lJettonScore;

	return true;
}

bool CAndroidUserItemSink::OnSubGameEnd( const void * pBuffer, WORD wDataSize )
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	m_pIAndroidUserItem->SetGameStatus(GS_GAME_END);

	//���ñ���
	m_lBankerScore=pGameEnd->lBankerScore;
	m_wBankerTime=pGameEnd->nBankerTime;

	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	return true;

}

bool CAndroidUserItemSink::OnSubUserApplyBanker( const void * pBuffer, WORD wDataSize )
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return false;

	//��Ϣ����
	CMD_S_ApplyBanker * pApplyBanker=(CMD_S_ApplyBanker *)pBuffer;

	if (pApplyBanker->wApplyUser==m_pIAndroidUserItem->GetChairID())
	{
	}

	return true;
}

bool CAndroidUserItemSink::OnSubUserCancelBanker( const void * pBuffer, WORD wDataSize )
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_CancelBanker));
	if (wDataSize!=sizeof(CMD_S_CancelBanker)) return false;

	//��Ϣ����
	CMD_S_CancelBanker * pCancelBanker=(CMD_S_CancelBanker *)pBuffer;

	return true;
}

bool CAndroidUserItemSink::OnSubChangeBanker( const void * pBuffer, WORD wDataSize )
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return false;

	//��Ϣ����
	CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pBuffer;

	WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
	if (m_wBankerUser==wMeChairID && pChangeBanker->wBankerUser!=wMeChairID)
	{
		m_bMeCurrentBanker=false;
		m_bCancelingBanker=false;
		m_wRandBankerTime=0;
	}
	else if (pChangeBanker->wBankerUser==wMeChairID)
	{
		m_bMeCurrentBanker=true;
		m_bApplyingBanker=false;
	}

	TRACE0("Change banker user........\n");

	m_wBankerUser=pChangeBanker->wBankerUser;
	m_lBankerScore=pChangeBanker->lBankerScore;

	return true;

}

bool CAndroidUserItemSink::OnSubGameRecord( const void * pBuffer, WORD wDataSize )
{
	return true;
}

bool CAndroidUserItemSink::OnSubPlaceJettonFail( const void * pBuffer, WORD wDataSize )
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJettonFail));
	if (wDataSize!=sizeof(CMD_S_PlaceJettonFail)) return false;

	//��Ϣ����
	CMD_S_PlaceJettonFail * pPlaceJettonFail=(CMD_S_PlaceJettonFail *)pBuffer;

	m_pIAndroidUserItem->KillGameTimer(IDI_GAME_ACTION);

	if (pPlaceJettonFail->wPlaceUser==m_pIAndroidUserItem->GetChairID())
	{
		ASSERT(pPlaceJettonFail->lJettonArea<=ID_JIAO_R && pPlaceJettonFail->lJettonArea>=ID_SHUN_MEN);
		m_lUserJettonScore[pPlaceJettonFail->lJettonArea]-=pPlaceJettonFail->lPlaceScore;
	}
	return true;
}

LONGLONG CAndroidUserItemSink::GetUserMaxJetton()
{
	//����ע��
	LONGLONG lNowJetton = 0;
	ASSERT(AREA_COUNT<=CountArray(m_lUserJettonScore));
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) 
		lNowJetton += m_lUserJettonScore[nAreaIndex];

	//ׯ�ҽ��
	LONGLONG lBankerScore=2147483647;
	if (m_wBankerUser!=INVALID_CHAIR) 
		lBankerScore=m_lBankerScore;

	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
		lBankerScore-=m_lAllJettonScore[nAreaIndex];

	//��������
	LONGLONG lMeMaxScore=min(m_lMeMaxScore-lNowJetton,m_lAreaLimitScore);

	//ׯ������
	lMeMaxScore=min(lMeMaxScore,lBankerScore);

	//��������
	ASSERT(lMeMaxScore >= 0);
	lMeMaxScore = max(lMeMaxScore, 0);

	return lMeMaxScore;
}

void CAndroidUserItemSink::LoadConfig()
{
	if (m_bLoadConfig)
	{
		if (m_strConfigFile.GetLength()==0)
		{
			TCHAR szPath[MAX_PATH]=TEXT("");
			GetModuleFileName(AfxGetInstanceHandle(),szPath,MAX_PATH);
			PathRemoveFileSpec(szPath);
			m_strConfigFile.Format(TEXT("%s\\RedNine100Config.ini"),szPath);
		}

		m_nMaxJettonRange=GetPrivateProfileInt(TEXT("AndroidUser"),TEXT("JettonRange"),4,m_strConfigFile);
		m_wMaxBankerTime=GetPrivateProfileInt(TEXT("AndroidUser"),TEXT("BankerTime"),8,m_strConfigFile);
		m_bAllowApplyBanker=GetPrivateProfileInt(TEXT("AndroidUser"),TEXT("AllowApplyBanker"),1,m_strConfigFile);
		m_bLoadConfig=FALSE;
	}
}
//////////////////////////////////////////////////////////////////////////
