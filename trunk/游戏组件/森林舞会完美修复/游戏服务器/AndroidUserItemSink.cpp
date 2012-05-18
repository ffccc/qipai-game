#include "Stdafx.h"
#include "AndroidUserItemSink.h"
#include ".\androiduseritemsink.h"

//////////////////////////////////////////////////////////////////////////

//����ʱ��
#define TIME_LESS					3									//����ʱ��
#define TIME_DISPATCH				8									//����ʱ��

//��Ϸʱ��
#define TIME_OUT_CARD				5									//����ʱ��
#define TIME_START_GAME				5									//��ʼʱ��
#define TIME_CALL_SCORE				5									//�з�ʱ��

//��Ϸʱ��
#define IDI_TIMER_JG				(IDI_ANDROID_ITEM_SINK+0)			//��Ϣʱ��
#define IDI_START_GAME				(IDI_ANDROID_ITEM_SINK+1)			//��ʼʱ��
#define IDI_ADD_JEETOND				(IDI_ANDROID_ITEM_SINK+2)			//�з�ʱ��
#define IDI_TIMER_FREE				(IDI_ANDROID_ITEM_SINK+4)			//����ʱ��


#define	IDI_TIMER_JEETTON			18									//��ע��ʱ��
#define IDI_TIMER_WAIBAO			6									//�����ʱ��
#define	IDI_TIMER_SENDCARD			22									//�����㷢ʱ��
#define IDI_TIMER_APPLYBANKER       23                                  //������ׯ
//////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink():
m_cbTurnCardCount(0),
m_first(true)
{
	srand((unsigned)time(0));
	m_wCurrentBanker = INVALID_CHAIR;
	m_lMyMaxStake=0;
	ZeroMemory(m_ShakeType,sizeof(m_ShakeType));
	//const tagUserScore* taUser =m_pIAndroidUserItem->GetMeUserItem()->GetUserScore();
	//m_Money = taUser->lGold;
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
	//m_cbTurnCardCount=0;
	//m_bCardTempCount=0;
	//ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));
	//m_bTurnOutType=CT_INVALID;

	////�����˿�
	//m_cbHandCardCount=0;
	//ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//memset(m_bCardTempData,0,sizeof(m_bCardTempData));
	//m_wCurrentBanker = INVALID_CHAIR;
	//m_first = true;
	//m_lMyMaxStake=0;
	//ZeroMemory(m_ShakeType,sizeof(m_ShakeType));
	return true;
}

//ʱ����Ϣ
bool __cdecl CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	//switch (nTimerID)
	//{
	//case IDI_START_GAME:	//��ʼ��Ϸ
	//	{
	//		//��ʼ�ж�
	//		m_pIAndroidUserItem->SendUserReady(NULL,0);

	//		return true;
	//	}
	//case IDI_TIMER_JG:	//��Ϣһ����
	//	{			
	//		//const tagUserScore* taUser =m_pIAndroidUserItem->GetMeUserItem()->GetUserScore();
	//		//m_Money = taUser->lGold;
	//		//m_cbTurnCardCount = 0;
	//		////�������
	//		//m_pIAndroidUserItem->KillGameTimer(IDI_TIMER_JG);
	//		//m_pIAndroidUserItem->SetGameTimer(IDI_ADD_JEETOND,100);

	//		printf("��Ϣһ����\n");

	//		return true;
	//	}
	//case IDI_ADD_JEETOND:		//�û���ע
	//	{

	//		//if(m_Money>100000)
	//		//{ 
	//		//	OnAddJeetton();
	//		//}
	//		//m_pIAndroidUserItem->KillGameTimer(IDI_ADD_JEETOND);

	//		//if (++m_cbTurnCardCount < 4)
	//		//{
	//		//	m_pIAndroidUserItem->SetGameTimer(IDI_ADD_JEETOND,100+rand()%4*100);
	//		//}
	//		return true;
	//	}
	//case IDI_TIMER_APPLYBANKER://������ׯ
	//	{
	//			//m_pIAndroidUserItem->KillGameTimer(IDI_TIMER_APPLYBANKER);
	//			//if (m_wCurrentBanker==INVALID_CHAIR)
	//			//{
	//			//	OnAppyBanker();
	//			//}
	//	}
	//	//case IDI_SEND_OUT_CARD:		//�û�����
	//	//	{
	//	//		//AutoOutCard(true);
	//	//	}
	//	return true;
	//}

	return false;
}
int CAndroidUserItemSink::AutoOutCard(bool bOutCard)
{

	return 0;
}

//��Ϸ��Ϣ
bool __cdecl CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	//case SUB_S_TIME_MODE:  //ʱ��״̬
	//	{
	//		return OnSubTimerType(pBuffer,wDataSize);
	//	}
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_PLACE_JETTON:	//�û���ע
		{
			return OnSubPlaceJetton(pBuffer,wDataSize);
		}
	case SUB_S_APPLY_BANKER:	//������ׯ
		{
			return OnSubApplyBanker( pBuffer, wDataSize );
		}
	case SUB_S_CHANGE_BANKER:	//�л�ׯ��
		{
			return OnSubChangeBanker( pBuffer, wDataSize );
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
bool __cdecl CAndroidUserItemSink::OnEventGameScene(BYTE cbGameStatus, bool bLookonOther, void * pBuffer, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GS_FREE:			//����״̬
		{


			return true;
		}
	case GS_PLAYING:		//��Ϸ״̬
		{
			return true;
		}
	}

	return false;
}

//�û�����
void __cdecl CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
#ifdef _DEBUG
	TRACE( pIAndroidUserItem->GetMeUserItem()->GetAccounts());
#endif
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

//�л�ׯ��
bool CAndroidUserItemSink::OnSubChangeBanker(void * pData, WORD wDataSize)
{
	//Ч������
	//ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	//if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return false;

	////��Ϣ����
	//CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pData;

	//m_wCurrentBanker = pChangeBanker->wChairID;

	//OnAppyBanker();


	return true;
}
//ʱ��״̬
bool CAndroidUserItemSink::OnSubChangeCard(void * pData, WORD wDataSize)
{
	return true;
}
//ʱ��״̬
bool CAndroidUserItemSink::OnSubTimerType(void * pData, WORD wDataSize)
{
	//Ч������
	//ASSERT(wDataSize==sizeof(CMD_S_GameTimeMode));
	//if (wDataSize!=sizeof(CMD_S_GameTimeMode)) return false;
	////��Ϣ����
	//CMD_S_GameTimeMode * pTimerType=(CMD_S_GameTimeMode *)pData;
	//switch(pTimerType->TimeMode)
	//{
	//case 1: //����ʱ��
	//	{
	//		if(m_wCurrentBanker==m_pIAndroidUserItem->GetChairID())
	//		{
	//			CMD_C_ShakeType ShakeType;
	//			ZeroMemory(&ShakeType,sizeof(ShakeType));
	//			ZeroMemory(m_ShakeType,sizeof(m_ShakeType));
	//			int nTpye=0;
	//			int nShakeType=rand()%3;
	//			if(nTpye==0)
	//			{
	//				m_ShakeType[0]=true;
	//				ShakeType.nShakeDiceType=1;
	//			}
	//			else if(nTpye==1)
	//			{
	//				m_ShakeType[1]=true;
	//				ShakeType.nShakeDiceType=2;
	//			}
	//			else
	//			{
	//				m_ShakeType[2]=true;
	//				ShakeType.nShakeDiceType=3;
	//			}
	//			m_pIAndroidUserItem->SendSocketData(SUB_C_SHAKETYPE,&ShakeType,sizeof(ShakeType));
	//		}
	//		break;
	//	}
	//case 2://��עʱ��
	//	{
	//		m_pIAndroidUserItem->SetGameTimer(IDI_TIMER_JG,100/*+rand()%4*/);//��Ϣһ���Ӻ���ע
	//		break;
	//	}
	//case 3://ҡ�������ʱ��
	//	{
	//		if(m_wCurrentBanker==m_pIAndroidUserItem->GetChairID())
	//		{
	//			Sleep(1);
	//			CMD_C_ShakeDice   ShakeDice;
	//			ZeroMemory(&ShakeDice,sizeof(ShakeDice));
	//			CopyMemory(ShakeDice.bDrawDiceAnimation,m_ShakeType,sizeof(ShakeDice.bDrawDiceAnimation));
	//			m_pIAndroidUserItem->SendSocketData(SUB_C_SHAKEDICE,&ShakeDice,sizeof(ShakeDice));
	//		}
	//		break;
	//	}
	//default:
	//	break;
	//}

	return true;
}

//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(void * pData, WORD wDataSize)
{


	return true;
}

//�û���ע
bool CAndroidUserItemSink::OnSubPlaceJetton(void * pData, WORD wDataSize)
{

	return true;
}

//������ׯ
bool CAndroidUserItemSink::OnSubApplyBanker(void * pData, WORD wDataSize)
{


	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(void * pData, WORD wDataSize)
{


	return true;
}


void CAndroidUserItemSink::OnAddJeetton(void)
{

}

// �Լ�������ׯ
void CAndroidUserItemSink::OnAppyBanker(void)
{
	
	return;	
}
