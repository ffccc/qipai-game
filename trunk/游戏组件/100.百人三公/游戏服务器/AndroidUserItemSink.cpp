#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_FREE					99									//����ʱ��
#define IDI_PLACE_JETTON			100									//��עʱ��
#define IDI_DISPATCH_CARD			301									//����ʱ��

//////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//������Ϣ
	m_lMeMaxScore=0L;			
	m_lAreaLimitScore=0L;		
	m_lApplyBankerCondition=0L;	

	//������ע
	m_lMeTieScore=0L;
	m_lMeBankerScore=0L;
	m_lMePlayerScore=0L;
	m_lMeTieSamePointScore=0L;
	m_lMePlayerKingScore=0L;
	m_lMeBankerKingScore=0L;
	m_lMePlayerTwoPair=0L;
	m_lMeBankerTwoPair=0L;

	//ׯ����Ϣ
	m_lBankerScore=0L;
	m_wCurrentBanker=0L;

	//״̬����
	m_bMeApplyBanker=false;

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

	return true;
}

//ʱ����Ϣ
bool __cdecl CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	//switch (nTimerID)
	//{
	//case IDI_PLACE_JETTON:
	//	{
	//		//��������
	//		CMD_C_PlaceJetton PlaceJetton;
	//		ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

	//		//�������
	//		PlaceJetton.cbJettonArea=(rand()%2)==0?ID_XIAN_JIA:ID_ZHUANG_JIA;
	//		PlaceJetton.lJettonScore=100;

	//		//������Ϣ
	//		m_pIAndroidUserItem->SendSocketData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));

	//		return false;
	//	}
	//default:
	//	{
	//		break;
	//	}
	//}

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
	case SUB_S_CHANGE_BANKER:	//�л�ׯ��
		{
			return OnSubChangeBanker(pBuffer, wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	}

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


//��Ϸ����
bool CAndroidUserItemSink::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) return false;

	//��������
//	for (int nAreaIndex=ID_XIAN_JIA; nAreaIndex<=ID_BANKER_TWO_PAIR; ++nAreaIndex) SetMePlaceJetton(nAreaIndex,0);

	return true;
}


//������ע
void CAndroidUserItemSink::SetMePlaceJetton(BYTE cbViewIndex, LONG lJettonCount)
{
	//Ч�����
	//ASSERT(cbViewIndex<=ID_BANKER_TWO_PAIR);
	//if (cbViewIndex>ID_BANKER_TWO_PAIR) return;

	//if (cbViewIndex==ID_XIAN_JIA) m_lMePlayerScore=lJettonCount;
	//else if (cbViewIndex==ID_PING_JIA) m_lMeTieScore=lJettonCount;
	//else if (cbViewIndex==ID_ZHUANG_JIA) m_lMeBankerScore=lJettonCount;
	//else if (cbViewIndex==ID_XIAN_TIAN_WANG) m_lMePlayerKingScore=lJettonCount;
	//else if (cbViewIndex==ID_ZHUANG_TIAN_WANG) m_lMeBankerKingScore=lJettonCount;
	//else if (cbViewIndex==ID_TONG_DIAN_PING) m_lMeTieSamePointScore=lJettonCount;
	//else if (cbViewIndex==ID_BANKER_TWO_PAIR) m_lMeBankerTwoPair=lJettonCount;
	//else if (cbViewIndex==ID_PLAYER_TWO_PAIR) m_lMePlayerTwoPair=lJettonCount;
	//else {ASSERT(FALSE);return;}
}

//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	////Ч������
	//ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	//if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	////��Ϣ����
	//CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	////ׯ����Ϣ
	//SetBankerInfo(pGameStart->wBankerUser,pGameStart->lBankerScore);

	////�����Ϣ
	//m_lMeMaxScore=pGameStart->lUserMaxScore;

	////����ʱ��
	//m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON,rand()%pGameStart->cbTimeLeave+1);

	return true;
}

//����ׯ��
void CAndroidUserItemSink::SetBankerInfo(WORD wBanker,LONG lScore)
{
	//m_wCurrentBanker=wBanker;
	//m_lBankerScore=lScore;
}



//�л�ׯ��
bool CAndroidUserItemSink::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	//ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	//if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return false;

	////��Ϣ����
	//CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pBuffer;

	////ׯ����
	//SetBankerInfo(pChangeBanker->wBankerUser,pChangeBanker->lBankerScore);

	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	////Ч������
	//ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	//if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////
