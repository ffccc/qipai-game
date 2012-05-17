#include "Stdafx.h"
#include "AndroidUserItemSink.h"
#include <math.h>

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
	m_lMeScore2=0L;
	m_lMeScore3=0L;
	m_lMeScore1=0L;
	m_lMeScore6=0L;
	m_lMeScore5=0L;
	m_lMeScore4=0L;
	m_lMeScore7=0L;
	m_lMeScore8=0L;

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
	////������ע
	//m_lMeScore2=0L;
	//m_lMeScore3=0L;
	//m_lMeScore1=0L;
	//m_lMeScore6=0L;
	//m_lMeScore5=0L;
	//m_lMeScore4=0L;
	//m_lMeScore7=0L;
	//m_lMeScore8=0L;
	return true;
}

//ʱ����Ϣ
bool __cdecl CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_PLACE_JETTON:
		{
			//��������
			CMD_C_PlaceJetton PlaceJetton;
			ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

			//CMD_APlaceJetton aPlaceJetton;
			//ZeroMemory(&aPlaceJetton,sizeof(aPlaceJetton));

			BYTE TempTimes = rand()%10 + 1;
			if (m_wCurrentBanker != 0)
			{
				//�������
				for (BYTE i=0; i<TempTimes; i++ )
				{
					PlaceJetton.cbJettonArea = rand()%8+1;
					int TemNum = rand()%1000;
					if (TemNum <= 400)
					{
						PlaceJetton.lJettonScore = 0;
						continue;
					}
					else if (TemNum>400 && TemNum<=600)
					{
						PlaceJetton.lJettonScore = 100;
					}
					else if (TemNum>600 && TemNum<=750)
					{
						PlaceJetton.lJettonScore = 1000;
					}
					else if (TemNum>750 && TemNum<=850)
					{
						PlaceJetton.lJettonScore = 10000;
					}
					else if (TemNum>850 && TemNum<=900)
					{
						PlaceJetton.lJettonScore = 50000;
					}
					else if (TemNum>900 && TemNum<=940)
					{
						PlaceJetton.lJettonScore = 100000;
					}
					else if (TemNum>940 && TemNum<=970)
					{
						PlaceJetton.lJettonScore = 500000;
					}
					else if (TemNum>970 && TemNum<=990)
					{
						PlaceJetton.lJettonScore = 1000000;
					}
					else if (TemNum>990 && TemNum<=1000) 
					{
						PlaceJetton.lJettonScore = 5000000;
					}

					//������ע��Ϣ
					m_pIAndroidUserItem->SendSocketData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
					
				}
			}
			
			return false;
		}
	default:
		{
			break;
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

	//AfxMessageBox("��Ϸ����");
	//��������
	for (BYTE nAreaIndex=ID_HORSE1; nAreaIndex<=ID_HORSE8; ++nAreaIndex) 
		SetMePlaceJetton(nAreaIndex,0);
	return true;
}

////������ע
//void CAndroidUserItemSink::SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
//{
//	//Ч�����
//	ASSERT(cbViewIndex<=ID_HUANG_MARK);
//	if (cbViewIndex>ID_HUANG_MARK) return;
//
//	if (cbViewIndex==ID_TIAN_MARK) m_lMeTianScore=lJettonCount;
//	else if (cbViewIndex==ID_DI_MARK) m_lMeDiScore=lJettonCount;
//	else if (cbViewIndex==ID_XUAN_MARK) m_lMeXuanScore=lJettonCount;
//	else if (cbViewIndex==ID_HUANG_MARK) m_lMeHuangScore=lJettonCount;
//	else {ASSERT(FALSE);return;}
//}

//������ע
void CAndroidUserItemSink::SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
{
	//Ч�����
	ASSERT(cbViewIndex<=ID_HORSE8);
	if (cbViewIndex>ID_HORSE8) return;

	if (cbViewIndex==ID_HORSE1) m_lMeScore1=lJettonCount;
	else if (cbViewIndex==ID_HORSE2) m_lMeScore2=lJettonCount;
	else if (cbViewIndex==ID_HORSE3) m_lMeScore3=lJettonCount;
	else if (cbViewIndex==ID_HORSE4) m_lMeScore4=lJettonCount;
	else if (cbViewIndex==ID_HORSE5) m_lMeScore5=lJettonCount;
	else if (cbViewIndex==ID_HORSE6) m_lMeScore6=lJettonCount;
	else if (cbViewIndex==ID_HORSE7) m_lMeScore7=lJettonCount;
	else if (cbViewIndex==ID_HORSE8) m_lMeScore8=lJettonCount;
	else {ASSERT(FALSE);return;}
}

//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//ׯ����Ϣ
	SetBankerInfo(pGameStart->wBankerUser,pGameStart->lBankerScore);

	//�����Ϣ
	m_lMeMaxScore=pGameStart->lUserMaxScore;

	//����ʱ��
	m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON,0);
	return true;
}

//����ׯ��
void CAndroidUserItemSink::SetBankerInfo(WORD wBanker,LONGLONG lScore)
{
	m_wCurrentBanker=wBanker;
	m_lBankerScore=lScore;
	if (m_wCurrentBanker == INVALID_CHAIR)
	{
		WORD bTempNum = rand()%100+1;
		if (m_pIAndroidUserItem->GetChairID()>=(bTempNum-5) && m_pIAndroidUserItem->GetChairID()<=(bTempNum+5))
		{
			m_pIAndroidUserItem->SendSocketData(SUB_C_APPLY_BANKER,NULL,0);
		}
	}
	else
	{
		WORD bTempNum = rand()%100+1;
		if (bTempNum <= 10)
		{
			m_pIAndroidUserItem->SendSocketData(SUB_C_APPLY_BANKER,NULL,0);
		}
	}
}

//�л�ׯ��
bool CAndroidUserItemSink::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return false;

	//��Ϣ����
	CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pBuffer;

	//ׯ����
	SetBankerInfo(pChangeBanker->wBankerUser,pChangeBanker->lBankerScore);
	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	////Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
