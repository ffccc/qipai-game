#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////
//����ʱ��
#define TIME_LESS					2									//����ʱ��
#define TIME_JETTON				    1									//��עʱ��
#define TIME_RAND				    5									//���ʱ��
#define TIME_APPLY_BANKER           20                                  // 20�����

//��Ϸʱ��
#define IDI_USER_JETTON			(IDI_ANDROID_ITEM_SINK+0)			    // ��ע��ʱ��ID
#define IDI_PLACE_JETTON        (IDI_ANDROID_ITEM_SINK+1)               // ��ע�׶�
#define IDI_APPLY_BANKER        (IDI_ANDROID_ITEM_SINK+2)               // ������ׯ
#define IDI_CANCEL_BANKER       (IDI_ANDROID_ITEM_SINK+3)               // ������ׯ
#define IDI_SELECT_SICBOTYPE    (IDI_ANDROID_ITEM_SINK+4)               // ѡ��ҡ���ӷ�ʽ

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	m_lMeMaxScore =0;						         // �����ע
	m_lAreaLimitScore=0;					         // ��������
	m_lApplyBankerCondition=0;			             // ��������
	ZeroMemory(m_lMeScore,sizeof(m_lMeScore));       // ÿ����λ��ע
	ZeroMemory(m_lAllScore,sizeof(m_lAllScore));	 // ÿ����λ��ע
	m_lBankerScore = 0;						         // ׯ�һ���
	m_wCurrentBanker = INVALID_CHAIR;	             // ��ǰׯ��
	m_pIAndroidUserItem = NULL;
	m_bEnableNote = false;                           // �Ƿ������ע
	m_bEnableSysBanker = false;                      // �Ƿ����ϵͳ��ׯ
	m_iMaxAndroidBanker = MAX_ANDROID_BANKER;
	m_lAllMaxScore=0L;                     // ���ֿ�����ע����ע
	m_arApplyBanker.RemoveAll();
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
	m_lMeMaxScore =0;						         // �����ע
	m_lAreaLimitScore=0;					         // ��������
	m_lApplyBankerCondition=0;			             // ��������
	ZeroMemory(m_lMeScore,sizeof(m_lMeScore));       // ÿ����λ��ע
	ZeroMemory(m_lAllScore,sizeof(m_lAllScore));	 // ÿ����λ��ע
	m_lBankerScore = 0;						         // ׯ�һ���
	m_wCurrentBanker = INVALID_CHAIR;	             // ��ǰׯ��
	m_bEnableNote = false;
	m_lAllMaxScore=0L;                     // ���ֿ�����ע����ע
	return true;
}

//ʱ����Ϣ
bool __cdecl CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_USER_JETTON:	// ��������ע
		{
			return OnPlaceJetton();
		}
	case IDI_PLACE_JETTON:	// �ܵ���עʱ�䵽��
		{
			if (NULL != m_pIAndroidUserItem)
			{
				m_pIAndroidUserItem->KillGameTimer(IDI_USER_JETTON);
			}
			m_bEnableNote = false;
			return true;
		}
	case IDI_APPLY_BANKER:
		{
			if (m_arApplyBanker.GetCount() >= m_iMaxAndroidBanker)
			{
				return true;
			}
			//�Ϸ��ж�
			IServerUserItem *pMeUserItem = m_pIAndroidUserItem->GetMeUserItem();
			if (m_wCurrentBanker == m_pIAndroidUserItem->GetChairID())
			{
				return true;
			}

			// ������ׯ
			if (pMeUserItem->GetUserScore()->lScore < m_lApplyBankerCondition)
			{
				return true;
			}

			// �Լ�Ҫ���ٶ�����
			for (int i=0; i<m_arApplyBanker.GetCount(); ++i)
			{
				if (m_arApplyBanker[i] == m_pIAndroidUserItem->GetUserID())
				{
					return true;
				}
			}

			//������Ϣ
			WORD wChairID = INVALID_CHAIR;
			m_pIAndroidUserItem->SendSocketData(SUB_C_APPLY_BANKER, (void *)&wChairID, sizeof(wChairID));
			return true;
		}
	case IDI_CANCEL_BANKER:
		{
			if (m_arApplyBanker.GetCount() < m_iMaxAndroidBanker)
			{
				return true;
			}

			//�Ϸ��ж�
			IServerUserItem *pMeUserItem = m_pIAndroidUserItem->GetMeUserItem();
			if (m_wCurrentBanker == m_pIAndroidUserItem->GetChairID())
			{
				return true;
			}

			// �Լ��������ׯ�Ҷ�����
			for (int i=0; i<m_arApplyBanker.GetCount(); ++i)
			{
				if (m_arApplyBanker[i] == m_pIAndroidUserItem->GetUserID())
				{
					//������Ϣ
					WORD wChairID = INVALID_CHAIR;
					m_pIAndroidUserItem->SendSocketData(SUB_C_CANCEL_BANKER, (void *)&wChairID, sizeof(wChairID));
					return true;
				}
			}
			return true;
		}
	case IDI_SELECT_SICBOTYPE:
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_SELECT_SICBOTYPE);
			if (m_wCurrentBanker != m_pIAndroidUserItem->GetChairID())
			{
				return true;
			}
			// ����ҡ���ӷ�ʽ
			CMD_C_ChangeSicboType cmdChangeSicboType;
			ZeroMemory(&cmdChangeSicboType, sizeof(cmdChangeSicboType));
			cmdChangeSicboType.enSicboType = (E_SICBO_TYPE)(rand()%enSicboType_InTheAir);
			m_pIAndroidUserItem->SendSocketData(SUB_C_CHANGE_SICBO_TYPE, &cmdChangeSicboType, sizeof(cmdChangeSicboType));
			return true;
		}
	default: break;
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
			m_lMeMaxScore =0;						         // �����ע
			m_lAllMaxScore=0L;                     // ���ֿ�����ע����ע
			ZeroMemory(m_lMeScore,sizeof(m_lMeScore));       // ÿ����λ��ע
			ZeroMemory(m_lAllScore,sizeof(m_lAllScore));	 // ÿ����λ��ע
			m_bEnableNote = false;
			return  true;
		}
	case SUB_S_GAME_START:		// ��Ϸ��ʼ
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_GameStart));
			if (wDataSize!=sizeof(CMD_S_GameStart))
			{
				return false;
			}

			//��Ϣ����
			CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

			ZeroMemory(m_lMeScore, sizeof(m_lMeScore));
			ZeroMemory(m_lAllScore, sizeof(m_lAllScore));

			m_lMeMaxScore=pGameStart->lUserMaxScore;
			m_wCurrentBanker = pGameStart->wBankerUser;
			m_lBankerScore = pGameStart->lBankerScore;
			m_lAllMaxScore=pGameStart->lAllMaxScore;               // ���ֿ�����ע����ע
			if (NULL != m_pIAndroidUserItem)
			{
				m_bEnableNote = true;
				UINT nElapse=(rand()%TIME_RAND) +TIME_LESS;
				if (pGameStart->cbTimeLeave>(TIME_LESS+nElapse))
				{
					if (m_wCurrentBanker != m_pIAndroidUserItem->GetChairID())
					{
						m_pIAndroidUserItem->SetGameTimer(IDI_USER_JETTON,nElapse);
						m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON,pGameStart->cbTimeLeave-TIME_LESS);
					}
					else
					{
						//m_pIAndroidUserItem->SetGameTimer(IDI_SELECT_SICBOTYPE,nElapse);
					}
				}
			}

			//�����ļ���
			TCHAR szPath[MAX_PATH]=TEXT("");
			TCHAR szINIPath[MAX_PATH] = TEXT("");
			GetCurrentDirectory(sizeof(szPath),szPath);
			_snprintf(szINIPath,sizeof(szINIPath),TEXT("%s\\SicboShowHandConfig.ini"),szPath);
			iRandLimit[0]=GetPrivateProfileInt("RandLimit","Area0",200,szINIPath);
			iRandLimit[1]=GetPrivateProfileInt("RandLimit","Area1",200,szINIPath);
			iRandLimit[2]=GetPrivateProfileInt("RandLimit","Area2",-1,szINIPath);
			iRandLimit[3]=GetPrivateProfileInt("RandLimit","Area3",20,szINIPath);
			iRandLimit[4]=GetPrivateProfileInt("RandLimit","Area4",60,szINIPath);
			iRandLimit[5]=GetPrivateProfileInt("RandLimit","Area5",85,szINIPath);
			iRandLimit[6]=GetPrivateProfileInt("RandLimit","Area6",97,szINIPath);
			return true;
		}
	case SUB_S_PLACE_JETTON:	//�û���ע
		{
			return OnSubPlaceJetton(pBuffer,wDataSize);
		}
	case SUB_S_APPLY_BANKER:	//������ׯ
		{
			return OnSubUserApplyBanker(pBuffer,wDataSize);
		}
	case SUB_S_CANCEL_BANKER:	//ȡ����ׯ
		{
			return OnSubUserCancelBanker(pBuffer,wDataSize);
		}
	case SUB_S_CHANGE_BANKER:	// �л�ׯ��
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
			if (wDataSize!=sizeof(CMD_S_ChangeBanker))
			{
				return false;
			}

			//��Ϣ����
			CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pBuffer;

			//ׯ����Ϣ
			m_wCurrentBanker = pChangeBanker->wBankerUser;
			m_lBankerScore = pChangeBanker->lBankerScore;
			m_iMaxAndroidBanker = 1 + rand()%MAX_ANDROID_BANKER;

			// ɾ�����
			for (int i=0; i<m_arApplyBanker.GetCount(); ++i)
			{
				if (m_arApplyBanker[i] == pChangeBanker->dwUserID)
				{
					m_arApplyBanker.RemoveAt(i);
					if (m_arApplyBanker.GetCount() < m_iMaxAndroidBanker)
					{
						m_pIAndroidUserItem->SetGameTimer(IDI_APPLY_BANKER, (TIME_LESS+rand()%TIME_APPLY_BANKER));	
					}
					return true;
				}
			}
			return true;
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			m_bEnableNote = false;
			// �����ʱ��
			if (NULL != m_pIAndroidUserItem)
			{				
				m_pIAndroidUserItem->KillGameTimer(IDI_USER_JETTON);
				m_pIAndroidUserItem->KillGameTimer(IDI_SELECT_SICBOTYPE);
				if (INVALID_CHAIR == m_wCurrentBanker)
				{
					m_arApplyBanker.RemoveAll();
					m_pIAndroidUserItem->SetGameTimer(IDI_APPLY_BANKER, (TIME_LESS+rand()%TIME_APPLY_BANKER));
				}
			}
			return true;
		}
	case SUB_S_SEND_RECORD:		//��Ϸ��¼
	case SUB_C_CHANGE_SICBO_TYPE:
		{
			// �����˲���Ҫ����
			return true;
		}
	case SUB_S_PLACE_JETTON_FAIL:	//��עʧ��
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_PlaceJettonFail));
			if (wDataSize!=sizeof(CMD_S_PlaceJettonFail))
			{
				return false;
			}

			//��Ϣ����
			CMD_S_PlaceJettonFail * pPlaceJettonFail=(CMD_S_PlaceJettonFail *)pBuffer;

			//Ч�����
			BYTE cbViewIndex=pPlaceJettonFail->lJettonArea;
			ASSERT(cbViewIndex<COUNT_AZIMUTH);
			if (cbViewIndex>=COUNT_AZIMUTH)
			{
				return false;
			}

			__int64 lJettonCount=pPlaceJettonFail->lPlaceScore;
			m_lMeScore[cbViewIndex] -= lJettonCount;
			if (m_lMeScore[cbViewIndex] < 0)
			{
				m_lMeScore[cbViewIndex] = 0;
			}
			return true;
		}
	default:break;
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
	case GS_FREE:			//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��Ϣ����
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			//�����Ϣ
			m_lMeMaxScore=pStatusFree->lUserMaxScore;
			m_lAllMaxScore = 0;
			m_wCurrentBanker = pStatusFree->wBankerUser;
			m_lBankerScore = pStatusFree->lBankerScore;

			//������Ϣ
			m_lApplyBankerCondition=pStatusFree->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusFree->lAreaLimitScore;
			m_bEnableSysBanker = pStatusFree->bEnableSysBanker;
			if (INVALID_CHAIR == m_wCurrentBanker)
			{
				m_pIAndroidUserItem->SetGameTimer(IDI_APPLY_BANKER, TIME_LESS);	
			}
			else
			{
				m_pIAndroidUserItem->SetGameTimer(IDI_APPLY_BANKER, TIME_APPLY_BANKER);	
			}
			return true;
		}
	case GS_PLAYING:		//��Ϸ״̬
	case GS_GAME_END:		//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//��Ϣ����
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			// ������ע����			
			for (BYTE i=0; i<COUNT_AZIMUTH; ++i)
			{
				//ȫ����ע
				m_lAllScore[i] = pStatusPlay->lAllScore[i];
				m_lMeScore[i] = pStatusPlay->lUserScore[i];
			}

			//��һ���
			m_lMeMaxScore=pStatusPlay->lUserMaxScore;

			//������Ϣ
			m_lApplyBankerCondition=pStatusPlay->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusPlay->lAreaLimitScore;
			m_lAllMaxScore=pStatusPlay->lAllMaxScore;               // ���ֿ�����ע����ע
	
			//ׯ����Ϣ
			m_wCurrentBanker = pStatusPlay->wBankerUser;
			m_lBankerScore = pStatusPlay->lBankerScore;
			m_bEnableSysBanker = pStatusPlay->bEnableSysBanker;
			return true;
		}
	}

	return true;
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

// ��ע����
bool CAndroidUserItemSink::OnPlaceJetton(void)
{
	if (NULL == m_pIAndroidUserItem)
	{
		m_bEnableNote = false;
		return true;
	}
	//ׯ���ж�
	if ( m_pIAndroidUserItem->GetChairID() == m_wCurrentBanker ) 
	{
		m_bEnableNote = false;
		return true;
	}
	if ((INVALID_CHAIR == m_wCurrentBanker)
		&&(!m_bEnableSysBanker)) 
	{
		m_bEnableNote = false;
		return true;
	}

	if (m_bEnableNote)
	{
		__int64 lUserNote[7]={100L,1000L,10000L,100000L,500000L,1000000L,5000000L};
		 
		int iRandArea[COUNT_AZIMUTH] = {-1,114,228,340,                   // ��С������˫ 58/1000
			452,453,455,460,470,482,494,506,518,530,535,540,543,545,     //����
			546,576,606,636,666,696,                                     // �� 30/1000
			726,741,756,771,786,801,816,831,846,861,876,881,896,911,926, // ����15/1000
			941,949,957,965,973,981,                                     // ����8/1000 *12
			989,990,991,992,993,994,995};                                // ��ͬ1/1000

		// ��ѡ������Ȼ���ٶ��¶���ע
		int iRand = rand()%1000;  // ��ע����
		BYTE byArea = enCardType_Big;
		for (BYTE i=(COUNT_AZIMUTH-1); i>=0; --i)
		{
			if (iRand > iRandArea[i])
			{
				byArea = i;
				break;
			}
		}

		// ��ȡ������������ע��
		__int64 lMaxPlayerScore = GetMaxAndroidScore(byArea);
		if (lMaxPlayerScore >= lUserNote[0])
		{
			__int64 lJettonScore = lUserNote[0];
			iRand = rand()%100;
			for (int i=6; i>=0; --i)
			{
				if (iRand > iRandLimit[i])
				{
					if (lMaxPlayerScore >=lUserNote[i])
					{
						lJettonScore = lUserNote[i];
					}
					break;
				}
			}

			//���ñ���
			m_lMeScore[byArea] += lJettonScore;

			//��������
			CMD_C_PlaceJetton PlaceJetton;
			ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

			// �������
			PlaceJetton.cbJettonArea=byArea;
			PlaceJetton.lJettonScore=lJettonScore;

			//������Ϣ
			m_pIAndroidUserItem->SendSocketData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));

			// �����´���ע
			UINT nElapse=rand()%TIME_RAND+TIME_JETTON;
			m_pIAndroidUserItem->SetGameTimer(IDI_USER_JETTON,nElapse);
		}
		else
		{
			m_bEnableNote = false;
		}				
	}
	return true;
}

//�û���ע
bool CAndroidUserItemSink::OnSubPlaceJetton(const void * pBuffer, WORD wDataSize)
{
	if (NULL == m_pIAndroidUserItem)
	{
		return true;
	}
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJetton));
	if (wDataSize!=sizeof(CMD_S_PlaceJetton)) return false;

	//��Ϣ����
	CMD_S_PlaceJetton * pPlaceJetton=(CMD_S_PlaceJetton *)pBuffer;
	if ((pPlaceJetton->cbJettonArea>=COUNT_AZIMUTH) || (pPlaceJetton->lJettonScore<0))
	{
		return true;
	}

	// ���������ע����
	m_lAllScore[pPlaceJetton->cbJettonArea] += pPlaceJetton->lJettonScore;
	return true;
}

//������ׯ
bool CAndroidUserItemSink::OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return false;

	//��Ϣ����
	CMD_S_ApplyBanker * pApplyBanker=(CMD_S_ApplyBanker *)pBuffer;

	// ����һ���Ƿ��ظ�
	for (int i=0; i<m_arApplyBanker.GetCount(); ++i)
	{
		if (pApplyBanker->dwUserID == m_arApplyBanker[i])
		{
			return true;
		}
	}
	m_arApplyBanker.Add(pApplyBanker->dwUserID);

	// ��������ҹ���ʱ������ׯ
	if ((m_arApplyBanker.GetCount() > m_iMaxAndroidBanker)
		&& (m_wCurrentBanker != m_pIAndroidUserItem->GetChairID()))
	{
		//�Ϸ��ж�
		IServerUserItem *pMeUserItem = m_pIAndroidUserItem->GetMeUserItem();
		if (NULL == pMeUserItem)
		{
			return true;
		}
		m_pIAndroidUserItem->SetGameTimer(IDI_CANCEL_BANKER, (TIME_LESS+rand()%TIME_APPLY_BANKER));
	}
	return true;
}

//ȡ����ׯ
bool CAndroidUserItemSink::OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_CancelBanker));
	if (wDataSize!=sizeof(CMD_S_CancelBanker)) return false;

	//��Ϣ����
	CMD_S_CancelBanker * pCancelBanker=(CMD_S_CancelBanker *)pBuffer;

	// ɾ�����
	for (int i=0; i<m_arApplyBanker.GetCount(); ++i)
	{
		if (m_arApplyBanker[i] == pCancelBanker->dwUserID)
		{
			m_arApplyBanker.RemoveAt(i);
			if (m_arApplyBanker.GetCount() < m_iMaxAndroidBanker)
			{
				m_pIAndroidUserItem->SetGameTimer(IDI_APPLY_BANKER, (TIME_LESS+rand()%TIME_APPLY_BANKER));				
			}
			return true;
		}
	}
	return true;
}

// �����ע
__int64 CAndroidUserItemSink::GetMaxAndroidScore(int iArea)
{
	if ((iArea < 0) || (iArea>=COUNT_AZIMUTH))
	{
		return 0L;
	}
	__int64 i64UserAllScore = 0; // ����Լ��Ѿ���ע�ķ�
	__int64 i64AllUserAllScore=0;// ���������ע�� 
	__int64 i64AllLossScore=0;   // ׯ��������
	__int64 i64LoseAreaScore=0;  // ׯ�����������ע��
	for (int i=0; i<COUNT_AZIMUTH; ++i)
	{
		i64UserAllScore += m_lMeScore[i];
		i64AllUserAllScore += m_lAllScore[i];
	}
	// ����������
	if (i64AllUserAllScore > 350000000L)
	{
		m_bEnableNote = false;
		return 0L;
	}
	for (int i=1; i<=MAX_SICBO_NUMBER; ++i)
	{
		for (int j=i; j<=MAX_SICBO_NUMBER; ++j)
		{
			for (int h=j; h<=MAX_SICBO_NUMBER; ++h)
			{
				E_SICBO_NUMBER enBuffer[]={(E_SICBO_NUMBER)i,(E_SICBO_NUMBER)j,(E_SICBO_NUMBER)h};
				E_CARD_TYPE enCardType[COUNT_AZIMUTH];
				int iCount = 0;
				m_GameLogic.GetCardType(enBuffer,enCardType, iCount);
				__int64 i64AllLossScoreTemp=0;
				__int64 i64LoseAreaScoreTemp=0;  // ׯ�����������ע��
				bool bFind = false;
				for (int l=0; l<iCount; ++l)
				{
					if (enCardType_Illegal == enBuffer[l])
					{
						continue;
					}
					if (iArea == enCardType[l])
					{
						bFind = true;
					}
				}
				if (!bFind)
				{
					continue;
				}

				// ��������������Ǯ
				for (int l=0; l<iCount; ++l)
				{
					if (enCardType_Illegal == enCardType[l])
					{
						continue;
					}
					i64LoseAreaScoreTemp +=m_lAllScore[enCardType[l]];  // ׯ�����������ע��
					if ((enCardType[l]<enCardType_SicboOne) ||(enCardType[l]>enCardType_SicboSix))
					{
						i64AllLossScoreTemp += (m_lAllScore[enCardType[l]] * m_GameLogic.m_i64Loss_Percent[enCardType[l]]);
					}
					else
					{
						// ��Ҫ�������
						E_SICBO_NUMBER enSicboNum = (E_SICBO_NUMBER)(enCardType[l]-enCardType_SicboOne+1);
						int iCountNum = m_GameLogic.GetSicboCountByNumber(enBuffer, enSicboNum);
						int iMuti[]={0,2,3,4};
						i64AllLossScoreTemp += (m_lAllScore[enCardType[l]] * iMuti[iCountNum]);
					}
				}

				if (i64AllLossScoreTemp > i64AllLossScore)
				{
					i64AllLossScore = i64AllLossScoreTemp;
				}
				if (i64LoseAreaScoreTemp>i64LoseAreaScore)
				{
					i64LoseAreaScore = i64LoseAreaScoreTemp;
				}
			}
		}
	}
	//OUTPUT_DEBUG_STRING(_T("client %d������עׯ������� %I64d"),iArea, i64AllLossScore);

	// ��ȡ��������
	__int64 lAllMaxScore = m_lAllMaxScore;
	__int64 lUserMaxScore = m_lMeMaxScore;

	// ��ȥ�Ѿ���ע��
	lUserMaxScore -= i64UserAllScore;
	if (lUserMaxScore<=0)
	{
		return 0L;
	}

	OUTPUT_DEBUG_STRING(_T("adroid �����ע %I64d, ���������ע %I64d,  ׯ�ҵ�ǰ�� %I64d  �����ע����%d"),
		lAllMaxScore, i64AllUserAllScore, i64AllLossScore, iArea);

	// ׯ�ҿ������Ǯ
	lAllMaxScore = lAllMaxScore - i64LoseAreaScore + i64AllUserAllScore;
	lAllMaxScore -= i64AllLossScore;
	if ((iArea<enCardType_SicboOne) ||(iArea>enCardType_SicboSix))
	{
		lAllMaxScore = lAllMaxScore/(m_GameLogic.m_i64Loss_Percent[iArea] -1);
	}
	else
	{
		lAllMaxScore = lAllMaxScore/3; // ���������
	}
	if (lAllMaxScore<=0)
	{
		return 0L;
	}
	lUserMaxScore = min(lAllMaxScore, lUserMaxScore);
	return lUserMaxScore;
}

//////////////////////////////////////////////////////////////////////////
