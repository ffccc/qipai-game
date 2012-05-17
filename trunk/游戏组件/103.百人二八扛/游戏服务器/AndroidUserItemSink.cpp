#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_FREE						99									//����ʱ��
#define IDI_PLACE_JETTON				2									//��עʱ��
#define IDI_ANDROID_USER_PLACE_JETTON	101									//��������ע
#define IDI_DISPATCH_CARD				301									//����ʱ��
//
//

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	m_lMeMaxScore = 0;						//�����ע
	m_lAreaLimitScore = 0;					//��������
	m_lApplyBankerCondition = 0;			//��������

	//��ע��Ϣ
	memset(m_lAllJettonScore, 0, sizeof(m_lAllJettonScore));//ȫ����ע
	memset(m_lMeUserJettonScore, 0, sizeof(m_lMeUserJettonScore));//������ע
	memset(m_cbTableCardArray, 0, sizeof(m_cbTableCardArray));//�����˿�
	m_cbLeftCardCount = 0;						//�˿���Ŀ

	//�ɼ�����
	m_lEndBankerScore = 0;					//ׯ�ҳɼ�
	m_lEndUserScore = 0;					//��ҳɼ�
	m_lEndUserReturnScore = 0;				//���ػ���
	m_lEndRevenue = 0;						//��Ϸ˰��

	//��ׯ��Ϣ
	m_nMeBankerCount = 0;					//��ׯ����
	m_nMePlayerCount = 0;					//�мҴ���

	//ׯ����Ϣ
	m_lBankerScore = 0;						//ׯ�һ���
	m_wCurrentBanker = INVALID_CHAIR;					//��ǰׯ��
	m_cbBankerTime = 0;						//ׯ�Ҿ���
	m_lBankerWinScore = 0;					//ׯ�ҳɼ�

	//�����б�
	//m_AppyBankerAdroidUserArray.RemoveAll();	//�������
	m_AppyBankerAdroidUserCount = 0;
	m_bAppyBanker = false;

	//���Ʊ���
	m_bEnableAndroidUserBanker = true;			//������ׯ
	m_bEnableSysBanker = true;					//����ϵͳ��ׯ

	m_lMeStatisticScore = 0L;					//�ۼƻ���
}

//��������
CAndroidUserItemSink::~CAndroidUserItemSink()
{

}

//�ӿڲ�ѯ
void * __cdecl CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink, Guid, dwQueryVer);
	return NULL;
}

//��ʼ�ӿ�
bool __cdecl CAndroidUserItemSink::InitUserItemSink(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pIAndroidUserItem = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, IAndroidUserItem);
	if (m_pIAndroidUserItem == NULL) return false;

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
	try
	{
		switch (nTimerID)
		{
				//���У���������ׯ��ת�ʵ�
			case IDI_FREE:
			{
				m_pIAndroidUserItem->KillGameTimer(IDI_FREE);
				break;
			}
			//����
			case IDI_DISPATCH_CARD:
			{
				return true;
				break;
			}
			//��ע
			case IDI_PLACE_JETTON:
			{
				return true;
				break;
			}
			//�������Զ���ע
			case IDI_ANDROID_USER_PLACE_JETTON:
			{
				m_pIAndroidUserItem->KillGameTimer(IDI_ANDROID_USER_PLACE_JETTON);
				if (m_wCurrentBanker == m_pIAndroidUserItem->GetChairID()) return true;

				if (m_pIAndroidUserItem->GetGameStatus() == GS_PLACE_JETTON)
				{
					LONGLONG llMaxScore = m_lMeMaxScore;//min(, m_pIAndroidUserItem->GetMeUserItem()->GetUserScore()->lGameGold / 2);
					const LONGLONG llScore[] = {10000L, 100000L, 1000000L, 5000000L, 10000000L, 50000000L};
					int index = -1;
					for (int i = 5; i >= 0; i--)
					{
						if (llMaxScore > llScore[i])
						{
							index = i;
							break;
						}
					}

					if (index >= 0 && rand() % 3 == 0)
					{
						int nAreaIndex = 1 + rand() % AREA_COUNT;
						int nScoreIndex = rand() % 2;
						if (nAreaIndex >= 0 && nAreaIndex <= 6 && nScoreIndex >= 0 && nScoreIndex <= 2)
							AndroidUserPlaceJetton(nAreaIndex, llScore[nScoreIndex]);
					}

					int nTimer = 1 + rand() % 2;
					while (nTimer <= 0)
					{
						nTimer = 1 + rand() % 2;
					}

					m_pIAndroidUserItem->SetGameTimer(IDI_ANDROID_USER_PLACE_JETTON, nTimer);
				}
				break;
			}
		}

		return true;
	}
	catch (...)
	{

	}
}

//��Ϸ��Ϣ
bool __cdecl CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
		case SUB_S_GAME_FREE:		//��Ϸ����
		{
			return OnSubGameFree(pBuffer, wDataSize);
		}
		case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer, wDataSize);
		}
		case SUB_S_PLACE_JETTON:	//�û���ע
		{
			return OnSubPlaceJetton(pBuffer, wDataSize);
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
			return OnSubGameEnd(pBuffer, wDataSize);
		}
		case SUB_S_SEND_RECORD:		//��Ϸ��¼
		{
			return true;
		}
		case SUB_S_PLACE_JETTON_FAIL:	//��עʧ��
		{
			return OnSubPlaceJettonFail(pBuffer, wDataSize);
		}
// 	case SUB_S_CANCEL_CAN:
// 		{
// 			return true;
// 		}
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
	try
	{

		switch (cbGameStatus)
		{
			case GS_FREE:			//����״̬
			{
				//Ч������
				ASSERT(wDataSize == sizeof(CMD_S_StatusFree));
				if (wDataSize != sizeof(CMD_S_StatusFree)) return false;

				//��Ϣ����
				CMD_S_StatusFree * pStatusFree = (CMD_S_StatusFree *)pData;

				//����ʱ��
				//m_pIAndroidUserItem->SetGameTimer(IDI_FREE, pStatusFree->cbTimeLeave);

				//�����Ϣ
				m_lMeMaxScore = pStatusFree->lUserMaxScore;
				//ׯ����Ϣ
				m_wCurrentBanker = pStatusFree->wBankerUser;
				m_lBankerScore = pStatusFree->lBankerScore;
				m_cbBankerTime = pStatusFree->cbBankerTime;
				m_lBankerWinScore = pStatusFree->lBankerWinScore;
				m_bEnableSysBanker = pStatusFree->bEnableSysBanker;


				//������Ϣ
				m_lApplyBankerCondition = pStatusFree->lApplyBankerCondition;
				m_lAreaLimitScore = pStatusFree->lAreaLimitScore;

				//����״̬
				m_pIAndroidUserItem->SetGameStatus(GS_FREE);

				return true;
			}
			case GS_PLAYING:		//��Ϸ״̬
			case GS_GAME_END:		//����״̬
			{
				//Ч������
				ASSERT(wDataSize == sizeof(CMD_S_StatusPlay));
				if (wDataSize != sizeof(CMD_S_StatusPlay)) return false;

				//��Ϣ����
				CMD_S_StatusPlay * pStatusPlay = (CMD_S_StatusPlay *)pData;

				//��ע��Ϣ
				for (int nAreaIndex = 1; nAreaIndex <= AREA_COUNT; ++nAreaIndex)
				{
					m_lAllJettonScore[nAreaIndex] = pStatusPlay->lAllJettonScore[nAreaIndex];
					m_lMeUserJettonScore[nAreaIndex] = pStatusPlay->lUserJettonScore[nAreaIndex];
				}

				//��һ���
				m_lMeMaxScore = pStatusPlay->lUserMaxScore;


				//������Ϣ
				m_lApplyBankerCondition = pStatusPlay->lApplyBankerCondition;
				m_lAreaLimitScore = pStatusPlay->lAreaLimitScore;

				if (pStatusPlay->cbGameStatus == GS_GAME_END)
				{
					//�˿���Ϣ
					memcpy(m_cbTableCardArray, pStatusPlay->cbTableCardArray, sizeof(m_cbTableCardArray));

					//���óɼ�
					m_lEndBankerScore = pStatusPlay->lEndBankerScore;				//ׯ�ҳɼ�
					m_lEndUserScore = pStatusPlay->lEndUserScore;					//��ҳɼ�
					m_lEndUserReturnScore = pStatusPlay->lEndUserReturnScore;		//���ػ���
					m_lEndRevenue = pStatusPlay->lEndRevenue;						//��Ϸ˰��
				}

				//ׯ����Ϣ
				m_wCurrentBanker = pStatusPlay->wBankerUser;
				m_lBankerScore = pStatusPlay->lBankerScore;
				m_cbBankerTime = pStatusPlay->cbBankerTime;
				m_lBankerWinScore = pStatusPlay->lBankerWinScore;
				m_bEnableSysBanker = pStatusPlay->bEnableSysBanker;

				//����״̬
				m_pIAndroidUserItem->SetGameStatus(pStatusPlay->cbGameStatus);
				//����ʱ��
				//m_pIAndroidUserItem->SetGameTimer(pStatusPlay->cbGameStatus == GS_GAME_END ? IDI_DISPATCH_CARD : IDI_PLACE_JETTON, pStatusPlay->cbTimeLeave);

				return true;
			}
		}

		return true;
	}
	catch (...)
	{

	}
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

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
	try
	{
		//��Ϣ����
		CMD_S_GameFree * pGameFree = (CMD_S_GameFree *)pBuffer;

		//����ʱ��
		//m_pIAndroidUserItem->SetGameTimer(IDI_FREE, pGameFree->cbTimeLeave);

		//����״̬
		m_pIAndroidUserItem->SetGameStatus(GS_FREE);
		//������ׯ


		return true;
	}
	catch (...)
	{

	}
}

//������ע
void CAndroidUserItemSink::AndroidUserPlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
{
	try
	{
		//��������
		CMD_C_PlaceJetton PlaceJetton;
		ZeroMemory(&PlaceJetton, sizeof(PlaceJetton));

		//�������
		PlaceJetton.cbJettonArea = cbViewIndex;
		PlaceJetton.lJettonScore = lJettonCount;

		//������Ϣ
		m_pIAndroidUserItem->SendSocketData(SUB_C_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));

	}
	catch (...)
	{

	}
}

//������ׯ
void CAndroidUserItemSink::RandomApplyBanker()
{
	try
	{
		if (m_bEnableAndroidUserBanker && m_pIAndroidUserItem && m_pIAndroidUserItem->GetMeUserItem() && m_pIAndroidUserItem->GetMeUserItem()->GetUserScore())
		{
			if (m_wCurrentBanker != m_pIAndroidUserItem->GetChairID())
			{
				if (m_pIAndroidUserItem->GetGameStatus() == GS_FREE &&
				        //m_AppyBankerAdroidUserArray.GetCount() <= 0 &&
				        m_AppyBankerAdroidUserCount < 1 &&
				        m_pIAndroidUserItem->GetMeUserItem()->GetUserScore()->lGameGold > m_lApplyBankerCondition &&
				        !m_bAppyBanker && (rand() % 10 == 0 || m_wCurrentBanker == INVALID_CHAIR))
				{
					//������ׯ
					m_pIAndroidUserItem->SendSocketData(SUB_C_APPLY_BANKER, NULL, 0);
					m_bAppyBanker = true;
				}
			}
			else if (m_cbBankerTime >= 3 && rand() % 3 == 0)
			{
				//��ׯ
				m_pIAndroidUserItem->SendSocketData(SUB_C_CANCEL_BANKER, NULL, 0);
				m_bAppyBanker = false;
			}
		}
	}
	catch (...)
	{

	}
}

//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	try
	{
		//Ч������
		ASSERT(wDataSize == sizeof(CMD_S_GameStart));
		if (wDataSize != sizeof(CMD_S_GameStart)) return false;

		//��Ϣ����
		CMD_S_GameStart * pGameStart = (CMD_S_GameStart *)pBuffer;

		//ׯ����Ϣ
		m_lBankerScore = pGameStart->lBankerScore;					//ׯ�һ���
		m_wCurrentBanker = pGameStart->wBankerUser;					//��ǰׯ��

		//�����Ϣ
		m_lMeMaxScore = pGameStart->lUserMaxScore;

		//����ʱ��
		//m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON, pGameStart->cbTimeLeave);
		int nTimer = 1 + rand() % 2;
		while (nTimer <= 0)
		{
			nTimer = 1 + rand() % 2;
		}
		m_pIAndroidUserItem->SetGameTimer(IDI_ANDROID_USER_PLACE_JETTON, nTimer);
		RandomApplyBanker();

		//����״̬
		m_pIAndroidUserItem->SetGameStatus(GS_PLACE_JETTON);

		return true;
	}
	catch (...)
	{

	}
}

//�û���ע
bool CAndroidUserItemSink::OnSubPlaceJetton(const void * pBuffer, WORD wDataSize)
{
	try
	{
		//Ч������
		ASSERT(wDataSize == sizeof(CMD_S_PlaceJetton));
		if (wDataSize != sizeof(CMD_S_PlaceJetton)) return false;

		//��Ϣ����
		CMD_S_PlaceJetton * pPlaceJetton = (CMD_S_PlaceJetton *)pBuffer;

		if (m_pIAndroidUserItem->GetChairID() != pPlaceJetton->wChairID/* || IsLookonMode()*/)
		{
			m_lAllJettonScore[pPlaceJetton->cbJettonArea] += pPlaceJetton->lJettonScore;
		}
	}
	catch (...)
	{

	}
	return true;
}

//�л�ׯ��
bool CAndroidUserItemSink::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize == sizeof(CMD_S_ChangeBanker));
	if (wDataSize != sizeof(CMD_S_ChangeBanker)) return false;

	//��Ϣ����
	CMD_S_ChangeBanker * pChangeBanker = (CMD_S_ChangeBanker *)pBuffer;

	//ׯ����Ϣ
	m_lBankerScore = pChangeBanker->lBankerScore;						//ׯ�һ���
	m_wCurrentBanker = pChangeBanker->wBankerUser;					//��ǰׯ��

	//ɾ�����
//	if (m_wCurrentBanker != INVALID_CHAIR)
//	{
// 		//ɾ�����
// 		for (int i = 0; i < m_AppyBankerAdroidUserArray.GetCount(); i++)
// 		{
// 			if (m_AppyBankerAdroidUserArray[i] == pChangeBanker->wBankerUser)
// 				m_AppyBankerAdroidUserArray.RemoveAt(i);
// 		}
	m_AppyBankerAdroidUserCount--;
//	}
	if (m_wCurrentBanker == m_pIAndroidUserItem->GetChairID())
		m_bAppyBanker = false;

	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	try
	{
		//Ч������
		ASSERT(wDataSize == sizeof(CMD_S_GameEnd));
		if (wDataSize != sizeof(CMD_S_GameEnd)) return false;

		//��Ϣ����
		CMD_S_GameEnd * pGameEnd = (CMD_S_GameEnd *)pBuffer;


		//����ʱ��
		//m_pIAndroidUserItem->SetGameTimer(IDI_DISPATCH_CARD, pGameEnd->cbTimeLeave);

		//�˿���Ϣ
		memcpy(m_cbTableCardArray, pGameEnd->cbTableCardArray, sizeof(m_cbTableCardArray));

		//����״̬
		m_pIAndroidUserItem->SetGameStatus(GS_GAME_END);

		m_lEndBankerScore = pGameEnd->lBankerTotallScore;
		m_cbBankerTime = pGameEnd->nBankerTime;
		m_cbLeftCardCount = pGameEnd->cbLeftCardCount;

		//�ɼ���Ϣ
		m_lEndBankerScore = pGameEnd->lBankerScore;				//ׯ�ҳɼ�
		m_lEndUserScore = pGameEnd->lUserScore;					//��ҳɼ�
		m_lEndUserReturnScore = pGameEnd->lUserReturnScore;		//���ػ���
		m_lEndRevenue = pGameEnd->lRevenue;						//��Ϸ˰��
		//ׯ����Ϣ
		if (m_wCurrentBanker == m_pIAndroidUserItem->GetChairID())
			m_lMeStatisticScore += m_lEndBankerScore;
		else
			m_lMeStatisticScore += m_lEndUserScore;

		return true;
	}
	catch (...)
	{

	}
}

//��עʧ��
bool CAndroidUserItemSink::OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize == sizeof(CMD_S_PlaceJettonFail));
	if (wDataSize != sizeof(CMD_S_PlaceJettonFail)) return false;
//
// 	//��Ϣ����
// 	CMD_S_PlaceJettonFail * pPlaceJettonFail = (CMD_S_PlaceJettonFail *)pBuffer;
//
// 	//Ч�����
// 	ASSERT(pPlaceJettonFail->lJettonArea <= ID_JIAO_R && pPlaceJettonFail->lJettonArea >= ID_SHUN_MEN);
// 	if (!(pPlaceJettonFail->lJettonArea <= ID_JIAO_R && pPlaceJettonFail->lJettonArea >= ID_SHUN_MEN)) return false;
//
// 	//�Լ��ж�
// 	if (m_pIAndroidUserItem->GetChairID() == pPlaceJettonFail->wPlaceUser)
// 	{
// 		//�Ϸ�У��
// 		ASSERT(m_lMeUserJettonScore[cbViewIndex] >= pPlaceJettonFail->lPlaceScore);
// 		if (pPlaceJettonFail->lPlaceScore > m_lMeUserJettonScore[pPlaceJettonFail->lJettonArea]) return false;
//
// 		//������ע
// 		m_lMeUserJettonScore[pPlaceJettonFail->lJettonArea] -= pPlaceJettonFail->lPlaceScore;
// 	}

	return true;
}

//������ׯ
bool CAndroidUserItemSink::OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
	try
	{
		//Ч������
		ASSERT(wDataSize == sizeof(CMD_S_ApplyBanker));
		if (wDataSize != sizeof(CMD_S_ApplyBanker)) return false;

		//��Ϣ����
		CMD_S_ApplyBanker * pApplyBanker = (CMD_S_ApplyBanker *)pBuffer;

		//�������
// 	if (m_wCurrentBanker != pApplyBanker->wApplyUser && !IsApplyedBanker(pApplyBanker->wApplyUser))
// 	{
// 		m_AppyBankerAdroidUserArray.Add(pApplyBanker->wApplyUser);
// 	}
		if (pApplyBanker->wApplyUser == m_pIAndroidUserItem->GetChairID())
			m_bAppyBanker = true;

		m_AppyBankerAdroidUserCount++;

		return true;
	}
	catch (...)
	{

	}
}

//ȡ������
bool CAndroidUserItemSink::OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{
	try
	{
		//Ч������
		ASSERT(wDataSize == sizeof(CMD_S_CancelBanker));
		if (wDataSize != sizeof(CMD_S_CancelBanker)) return false;

		//��Ϣ����
		CMD_S_CancelBanker * pCancelBanker = (CMD_S_CancelBanker *)pBuffer;


// 	//ɾ�����
// 	WORD wCancelUserID = INVALID_CHAIR;
// 	for (int i = 1; i < GAME_PLAYER; i++)
// 	{
// 		IServerUserItem *pServerUserItem = m_pIAndroidUserItem->GetTableUserItem(i);
// 		if (pServerUserItem)
// 		{
// 			tagServerUserData *pUserData = pServerUserItem->GetUserData();
// 			if (pUserData)
// 			{
// 				if (lstrcmp(pUserData->szAccounts, pCancelBanker->szCancelUser) == 0)
// 				{
// 					wCancelUserID = i;
// 					break;
// 				}
// 			}
// 		}
// 	}

// 	if (wCancelUserID != INVALID_CHAIR)
// 	{
// 		for (int i = 0; i < m_AppyBankerAdroidUserArray.GetCount(); i++)
// 		{
// 			if (m_AppyBankerAdroidUserArray[i] == wCancelUserID)
// 				m_AppyBankerAdroidUserArray.RemoveAt(i);
// 		}
// 	}

// 	if (pCancelBanker->szCancelUser == m_pIAndroidUserItem->GetChairID())
// 		m_bAppyBanker = false;

		m_AppyBankerAdroidUserCount--;
	}
	catch (...)
	{

	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
