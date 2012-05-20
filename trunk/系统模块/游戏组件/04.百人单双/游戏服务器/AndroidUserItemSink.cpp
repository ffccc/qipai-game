#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_FREE					99									//����ʱ��
#define IDI_PLACE_JETTON			100									//��עʱ��
#define IDI_DISPATCH_CARD			301									//����ʱ��


//ʱ���ʶ
#define IDI_PLACE_JETTON			100									//��עʱ��

#ifdef _DEBUG
#define TIME_PLACE_JETTON			1									//��עʱ��
#define LESS_TIME					2									//��עʱ��
#else 
#define TIME_PLACE_JETTON			5									//��עʱ��
#define LESS_TIME					2									//��עʱ��
#endif


//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
WORD CAndroidUserItemSink::m_wAndroidApplyBankerCount = 0;			//������������ׯ����
BOOL CAndroidUserItemSink::m_blApply = false;
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
	m_nMeBankerCount=0;
	m_nMePlayerCount=0;
	m_nApplyUserCount=0;
	//m_AppyBankerAdroidUserArray.RemoveAll();

	//״̬����
	m_bMeApplyBanker=false;
	m_bCancelBanker=false;

	m_blInUserArray = false;


	//�����ļ���
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_snprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\OddEvenBattleConfig.ini"),szPath);

	
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
	switch (nTimerID)
	{
	case IDI_PLACE_JETTON:
		{
			if (m_wCurrentBanker!=m_pIAndroidUserItem->GetChairID())
			{
				//��������
				CMD_C_PlaceJetton PlaceJetton;
				ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));
				//�������
				PlaceJetton.cbJettonArea=(rand()%2)+1;
				
				LONG lPlaceScore[]={100L,1000L,10000L,100000L,500000L,1000000L,5000000L};
				int iCount = 0;

				TCHAR Session[255];
				memset(Session,0,255);
				_snprintf(Session,sizeof(Session),TEXT("Jetton%d"),m_wServerID);

				LONG Lmin = GetPrivateProfileInt(Session, TEXT("Min"), 10, m_szConfigFileName); 
				LONG Lmax = GetPrivateProfileInt(Session, TEXT("Max"), 10, m_szConfigFileName); 
				if(Lmax<Lmin||Lmin<100L||Lmax>5000000L)
				{
					m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON,rand()%TIME_PLACE_JETTON+LESS_TIME);
					return false;
				}
				INT nIndex = 0;//CountArray(lPlaceScore)-1;
				for (int n = 0;n<200;n++){

					nIndex=rand()%CountArray(lPlaceScore);
					if(Lmin<=lPlaceScore[nIndex] && lPlaceScore[nIndex]<=Lmax)
					{
						break;
					}
				}
				if (lPlaceScore[nIndex]<=m_pIAndroidUserItem->GetMeUserItem()->GetUserScore()->lScore)
					PlaceJetton.lJettonScore=lPlaceScore[nIndex];
				else
				{
					PlaceJetton.lJettonScore=0;
					return false;
				}
			
				//������Ϣ
				m_pIAndroidUserItem->SendSocketData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));

				//����ʱ��
				m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON,rand()%TIME_PLACE_JETTON+LESS_TIME);

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
	case SUB_S_APPLY_BANKER:	//������ׯ
		{
			return OnSubUserApplyBanker(pBuffer, wDataSize);
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


	return true;
}


//������ע
void CAndroidUserItemSink::SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
{
	
}

//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	SetBankerInfo(pGameStart->wCurBankerID,100);
	m_wServerID = pGameStart->wServerID;


	TCHAR Session[255];
	memset(Session,0,255);
	_snprintf(Session,sizeof(Session),TEXT("BankerCondition%d"),m_wServerID);


	//����ʱ��
	m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON,rand()%pGameStart->cbGameTime+1);

	BYTE cbEnableAndroidUserBanker=GetPrivateProfileInt(Session, TEXT("EnableAndroidUserBanker"), 1, m_szConfigFileName);
	m_bEnableAndroidUserBanker=cbEnableAndroidUserBanker?true:false;
	m_nAndroidUserBankerCount=GetPrivateProfileInt(Session, TEXT("AndroidUserBankerCount"), 1, m_szConfigFileName);
	m_nAndroidUserPlayerCount=GetPrivateProfileInt(Session, TEXT("AndroidUserPlayerCount"), 2, m_szConfigFileName);
	m_nMinApplyBankerCount=GetPrivateProfileInt(Session, TEXT("MinApplyBankerCount"), 1, m_szConfigFileName);
	m_lApplyBankerCondition=GetPrivateProfileInt(Session, TEXT("Score"), 100, m_szConfigFileName);


	return true;
}

//����ׯ��
void CAndroidUserItemSink::SetBankerInfo(WORD wBanker,LONGLONG lScore)
{
	m_wCurrentBanker=wBanker;
	//m_lBankerScore=lScore;
}



//�л�ׯ��
bool CAndroidUserItemSink::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return false;

	//��Ϣ����
	CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pBuffer;

	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;



	return true;
}


//������ׯ
bool CAndroidUserItemSink::OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return false;

	CMD_S_ApplyBanker *pApplyBanker  = (CMD_S_ApplyBanker *)pBuffer;

	
	return true;
}

//ȡ����ׯ
bool CAndroidUserItemSink::OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	
	return true;
}
//////////////////////////////////////////////////////////////////////////
