#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//��������
#define SEND_COUNT					300									//���ʹ���

//��������
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������

//��עʱ��
#define IDI_FREE					1									//����ʱ��
#define TIME_FREE					10									//����ʱ��

//��עʱ��
#define IDI_PLACE_JETTON			2									//��עʱ��
#define TIME_PLACE_JETTON			18									//��עʱ��

//����ʱ��
#define IDI_GAME_END				3									//����ʱ��
#define TIME_GAME_END				12									//����ʱ��


//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;				//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_TimeControl;	//��ʼģʽ

//////////////////////////////////////////////////////////////////////////
//����ṹ
struct tagReplaceCard
{
	BYTE							cbCardCount[2];						//�˿���Ŀ
	BYTE							cbTableCardArray[2][3];				//�����˿�
};

//ׯ����
tagReplaceCard CardBankerPairCard[]=
{
		3,3,0x1b,0x25,0x37,0x01,0x11,0x18,
		3,3,0x1c,0x24,0x37,0x01,0x11,0x14,
		3,3,0x25,0x39,0x3b,0x01,0x11,0x19,
		3,3,0x1c,0x2b,0x3a,0x01,0x11,0x18,
		3,2,0x1c,0x25,0x2a,0x03,0x13,0x00,
		3,2,0x34,0x36,0x3a,0x02,0x22,0x00,
		3,2,0x35,0x3a,0x3d,0x02,0x12,0x00,
		3,2,0x26,0x28,0x31,0x02,0x22,0x00,
		3,3,0x22,0x29,0x2c,0x06,0x16,0x1c,
		3,3,0x35,0x3b,0x3c,0x06,0x16,0x32,
		3,3,0x2b,0x34,0x3a,0x06,0x16,0x1d,
		3,2,0x32,0x33,0x39,0x07,0x17,0x00,
		3,3,0x2c,0x33,0x3c,0x0b,0x1b,0x2b,
		3,3,0x34,0x36,0x3c,0x0a,0x2a,0x2b,
		3,3,0x2d,0x32,0x3a,0x0a,0x1a,0x23,
		3,3,0x2d,0x34,0x37,0x0b,0x1b,0x24
};

//ׯ�Ҵ���
tagReplaceCard CardBankerWinCard[]=
{
		2,3,0x25,0x31,0x00,0x01,0x02,0x04,
		3,3,0x09,0x22,0x32,0x01,0x02,0x03,
		3,3,0x18,0x23,0x2c,0x01,0x02,0x0a,
		3,3,0x2b,0x2d,0x35,0x01,0x02,0x04,
		3,3,0x2d,0x32,0x39,0x03,0x09,0x13,
		3,3,0x1a,0x24,0x3d,0x03,0x0a,0x13,
		3,3,0x29,0x32,0x3c,0x03,0x09,0x13,
		3,3,0x2c,0x32,0x39,0x03,0x0a,0x12,
		3,2,0x21,0x24,0x39,0x06,0x0c,0x00,
		3,2,0x1b,0x31,0x3c,0x06,0x0c,0x00,
		3,3,0x1d,0x2b,0x36,0x06,0x0c,0x11,
		3,2,0x2c,0x32,0x3a,0x06,0x0b,0x00,
		3,2,0x1b,0x1d,0x3b,0x0b,0x16,0x00,
		2,3,0x25,0x31,0x00,0x0b,0x15,0x22,
		3,2,0x23,0x29,0x3a,0x0b,0x14,0x00,
		3,2,0x25,0x2d,0x35,0x0b,0x16,0x00
};

//ׯ����
tagReplaceCard CardBankerKingCard[]=
{
		2,3,0x25,0x32,0x00,0x01,0x02,0x16,
		3,3,0x23,0x2b,0x39,0x01,0x02,0x06,
		3,3,0x15,0x17,0x35,0x01,0x02,0x05,
		3,3,0x14,0x16,0x37,0x01,0x02,0x05,
		2,2,0x28,0x36,0x00,0x03,0x06,0x00,
		2,2,0x2c,0x34,0x00,0x03,0x06,0x00,
		2,2,0x16,0x27,0x00,0x03,0x06,0x00,
		2,2,0x14,0x1a,0x00,0x03,0x06,0x00,
		2,3,0x37,0x3a,0x00,0x07,0x13,0x29,
		2,2,0x2b,0x36,0x00,0x07,0x12,0x00,
		2,2,0x28,0x29,0x00,0x07,0x12,0x00,
		3,3,0x23,0x2c,0x39,0x07,0x13,0x18,
		2,2,0x1c,0x24,0x00,0x0a,0x19,0x00,
		2,2,0x34,0x3b,0x00,0x0a,0x19,0x00,
		2,2,0x25,0x27,0x00,0x0a,0x19,0x00,
		2,2,0x29,0x36,0x00,0x0a,0x18,0x00
};

//ƽ
tagReplaceCard CardEqualCard[]=
{
		2,2,0x1d,0x26,0x00,0x01,0x05,0x00,
		3,3,0x15,0x1c,0x29,0x01,0x02,0x11,
		2,2,0x15,0x21,0x00,0x01,0x05,0x00,
		2,2,0x1c,0x26,0x00,0x01,0x05,0x00,
		3,3,0x1a,0x25,0x39,0x03,0x0c,0x11,
		3,3,0x21,0x24,0x39,0x03,0x0c,0x11,
		3,3,0x2a,0x34,0x37,0x03,0x09,0x29,
		3,3,0x0b,0x32,0x3a,0x03,0x09,0x0a,
		2,2,0x1c,0x27,0x00,0x06,0x11,0x00,
		2,2,0x17,0x1a,0x00,0x06,0x11,0x00,
		2,2,0x21,0x25,0x00,0x06,0x1c,0x00,
		2,2,0x36,0x3b,0x00,0x06,0x0c,0x00,
		3,3,0x32,0x38,0x3a,0x0b,0x1c,0x1d,
		3,3,0x26,0x27,0x33,0x0b,0x14,0x22,
		3,3,0x14,0x19,0x1d,0x0c,0x11,0x12,
		3,3,0x2b,0x2d,0x34,0x0b,0x13,0x21
};

//ͬ��ƽ
tagReplaceCard CardSamePointCard[]=
{
		2,2,0x21,0x35,0x00,0x01,0x05,0x00,
		2,2,0x22,0x24,0x00,0x02,0x04,0x00,
		2,2,0x11,0x15,0x00,0x01,0x05,0x00,
		2,2,0x11,0x15,0x00,0x01,0x05,0x00,
		3,3,0x23,0x2b,0x3a,0x03,0x0b,0x1a,
		2,2,0x16,0x1a,0x00,0x06,0x0a,0x00,
		3,3,0x13,0x28,0x39,0x03,0x08,0x09,
		2,2,0x26,0x2c,0x00,0x06,0x0c,0x00,
		2,2,0x29,0x38,0x00,0x09,0x18,0x00,
		2,2,0x38,0x39,0x00,0x08,0x09,0x00,
		3,3,0x29,0x31,0x32,0x09,0x21,0x22,
		2,2,0x38,0x39,0x00,0x08,0x19,0x00,
		2,2,0x27,0x2b,0x00,0x17,0x1b,0x00,
		2,2,0x26,0x2c,0x00,0x16,0x1c,0x00,
		3,3,0x2b,0x32,0x35,0x1b,0x22,0x25,
		2,2,0x32,0x35,0x00,0x12,0x15,0x00
};

//�ж���
tagReplaceCard CardPlayerPairCard[]=
{
		3,3,0x05,0x25,0x3a,0x01,0x02,0x03,
		2,2,0x08,0x28,0x00,0x01,0x05,0x00,
		3,2,0x07,0x17,0x21,0x01,0x03,0x00,
		3,2,0x17,0x27,0x2d,0x01,0x06,0x00,
		3,3,0x25,0x35,0x3c,0x03,0x09,0x15,
		3,3,0x2a,0x3a,0x3d,0x03,0x08,0x14,
		3,3,0x1c,0x2c,0x34,0x03,0x07,0x12,
		3,3,0x21,0x31,0x34,0x03,0x11,0x13,
		2,2,0x23,0x33,0x00,0x06,0x0a,0x00,
		3,2,0x1a,0x2a,0x32,0x06,0x09,0x00,
		3,2,0x17,0x27,0x32,0x05,0x0d,0x00,
		3,2,0x25,0x35,0x3b,0x06,0x0a,0x00,
		2,3,0x28,0x38,0x00,0x0b,0x13,0x17,
		3,3,0x1b,0x2b,0x3c,0x0b,0x0c,0x13,
		3,3,0x21,0x31,0x3b,0x0a,0x13,0x14,
		3,3,0x1c,0x2c,0x32,0x0a,0x0b,0x0c
};

//��
tagReplaceCard CardPlayerWinCard[]=
{
		3,3,0x09,0x15,0x29,0x01,0x02,0x07,
		3,3,0x0c,0x23,0x2a,0x01,0x02,0x08,
		2,3,0x0d,0x16,0x00,0x01,0x02,0x07,
		2,3,0x11,0x26,0x00,0x01,0x02,0x07,
		3,3,0x26,0x27,0x2a,0x03,0x09,0x0c,
		2,3,0x32,0x34,0x00,0x03,0x08,0x21,
		3,3,0x19,0x32,0x36,0x03,0x08,0x15,
		2,3,0x17,0x19,0x00,0x03,0x08,0x13,
		2,3,0x29,0x38,0x00,0x05,0x18,0x21,
		3,3,0x2b,0x2c,0x36,0x05,0x1a,0x1d,
		2,3,0x2a,0x37,0x00,0x05,0x1b,0x28,
		2,3,0x2c,0x37,0x00,0x05,0x1c,0x1d,
		3,3,0x24,0x27,0x36,0x0b,0x16,0x18,
		3,3,0x2b,0x31,0x36,0x0b,0x16,0x18,
		3,2,0x23,0x32,0x3d,0x0b,0x14,0x00,
		2,2,0x2a,0x37,0x00,0x0b,0x16,0x00
};
//������
tagReplaceCard CardPlayerKingCard[]=
{
		2,2,0x14,0x25,0x00,0x01,0x02,0x00,
		2,2,0x14,0x35,0x00,0x01,0x02,0x00,
		3,3,0x0a,0x25,0x34,0x01,0x02,0x03,
		3,2,0x28,0x33,0x38,0x01,0x02,0x00,
		2,2,0x24,0x35,0x00,0x03,0x09,0x00,
		2,2,0x13,0x16,0x00,0x03,0x09,0x00,
		2,2,0x18,0x21,0x00,0x03,0x09,0x00,
		2,2,0x28,0x2d,0x00,0x03,0x08,0x00,
		2,2,0x19,0x1b,0x00,0x06,0x09,0x00,
		2,2,0x15,0x23,0x00,0x06,0x09,0x00,
		2,2,0x19,0x1a,0x00,0x06,0x09,0x00,
		2,2,0x1a,0x28,0x00,0x06,0x09,0x00,
		3,3,0x19,0x22,0x37,0x0b,0x14,0x16,
		3,2,0x1d,0x21,0x38,0x0b,0x13,0x00,
		3,3,0x34,0x36,0x39,0x0b,0x13,0x28,
		2,2,0x29,0x2d,0x00,0x0b,0x13,0x00
};

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//����ע��
	m_lTieScore=0L;
	m_lBankerScore=0L;
	m_lPlayerScore=0L;		
	m_lTieSamePointScore=0L;
	m_lBankerKingScore=0L;
	m_lPlayerKingScore=0L;	
	m_lPlayerTwoPair=0L;
	m_lBankerTwoPair=0L;
	m_cbExcuteTimes=0;

    //���Ʊ���
	m_cbWinSideControl=0;
	m_nSendCardCount=0;

	//������ע
	ZeroMemory(m_lUserTieScore,sizeof(m_lUserTieScore));
	ZeroMemory(m_lUserPlayerScore,sizeof(m_lUserPlayerScore));
	ZeroMemory(m_lUserTieSamePointScore,sizeof(m_lUserTieSamePointScore));
	ZeroMemory(m_lUserBankerKingScore,sizeof(m_lUserBankerKingScore));
	ZeroMemory(m_lUserPlayerKingScore,sizeof(m_lUserPlayerKingScore));
	ZeroMemory(m_lUserPlayerTwoPair,sizeof(m_lUserPlayerTwoPair));
	ZeroMemory(m_lUserBankerTwoPair,sizeof(m_lUserBankerTwoPair));
	ZeroMemory(m_lUserBankerScore,sizeof(m_lUserBankerScore));

	//��ҳɼ�	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	//�˿���Ϣ
	ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
	ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));

	//״̬����
	m_dwJettonTime=0L;

	//ׯ����Ϣ
	m_ApplyUserArray.RemoveAll();
	m_wCurrentBanker=INVALID_CHAIR;
	m_wBankerTime=0;
	m_lBankerWinScore=0L;		
	m_lBankerCurGameScore=0L;
	m_bEnableSysBanker=true;

	//��¼����
	ZeroMemory(m_GameRecordArrary,sizeof(m_GameRecordArrary));
	m_nRecordFirst=0;
	m_nRecordLast=0;
	m_dwRecordCount=0;

	//ׯ������
	m_lBankerMAX = 0l;
	m_lBankerAdd = 0l;							
	m_lBankerScoreMAX = 0l;
	m_lBankerScoreAdd = 0l;
	m_lPlayerBankerMAX = 0l;
	m_bExchangeBanker = true;

	//ʱ�����
	m_cbFreeTime = TIME_FREE;
	m_cbBetTime = TIME_PLACE_JETTON;
	m_cbEndTime = TIME_GAME_END;

	//�����˿���
	m_nChipRobotCount = 0;

	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
}

//�ӿڲ�ѯ
void * __cdecl CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
#ifdef __SPECIAL___
	QUERYINTERFACE(ITableUserActionEX,Guid,dwQueryVer);	
#endif
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//��ʼ��
bool __cdecl CTableFrameSink::InitTableFrameSink(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//���ƽӿ�
	m_pITableFrameControl=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrameControl);
	if (m_pITableFrameControl==NULL) return false;

	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//��ȡ����
	memcpy(m_szGameRoomName, m_pGameServiceOption->szGameRoomName, sizeof(m_szGameRoomName));

	//�����ļ���
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_snprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\BaccaratBattleConfig.ini"),szPath);

	ReadConfigInformation(true);	

	return true;
}

//��λ����
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//����ע��
	m_lTieScore=0L;
	m_lBankerScore=0L;
	m_lPlayerScore=0L;		
	m_lTieSamePointScore=0L;
	m_lBankerKingScore=0L;
	m_lPlayerKingScore=0L;	
	m_lPlayerTwoPair=0L;
	m_lBankerTwoPair=0L;

	//������ע
	ZeroMemory(m_lUserTieScore,sizeof(m_lUserTieScore));
	ZeroMemory(m_lUserPlayerScore,sizeof(m_lUserPlayerScore));
	ZeroMemory(m_lUserTieSamePointScore,sizeof(m_lUserTieSamePointScore));
	ZeroMemory(m_lUserBankerKingScore,sizeof(m_lUserBankerKingScore));
	ZeroMemory(m_lUserPlayerKingScore,sizeof(m_lUserPlayerKingScore));
	ZeroMemory(m_lUserPlayerTwoPair,sizeof(m_lUserPlayerTwoPair));
	ZeroMemory(m_lUserBankerTwoPair,sizeof(m_lUserBankerTwoPair));
	ZeroMemory(m_lUserBankerScore,sizeof(m_lUserBankerScore));

	//��ҳɼ�	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	//�����˿���
	m_nChipRobotCount = 0;

	return;
}

//��ʼģʽ
enStartMode __cdecl CTableFrameSink::GetGameStartMode()
{
	return m_GameStartMode;
}

//��Ϸ״̬
bool __cdecl CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	return true;
}

//��Ϸ��ʼ
bool __cdecl CTableFrameSink::OnEventGameStart()
{
	//��������
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));

	//��ȡׯ��
	IServerUserItem *pIBankerServerUserItem=NULL;
	if (INVALID_CHAIR!=m_wCurrentBanker) pIBankerServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);

	//���ñ���
	GameStart.cbTimeLeave=m_cbBetTime;
	GameStart.wBankerUser=m_wCurrentBanker;
	if (pIBankerServerUserItem!=NULL) GameStart.lBankerScore=pIBankerServerUserItem->GetUserScore()->lScore;

	//��ע����������
	int nChipRobotCount = 0;
	for (int i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem != NULL && pIServerUserItem->IsAndroidUser())
			nChipRobotCount++;
	}
	GameStart.nChipRobotCount = min(nChipRobotCount, m_nMaxChipRobot);

	//�����˿���
	m_nChipRobotCount = 0;

    //�Թ����
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	

	//��Ϸ���
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		if (pIServerUserItem==NULL) continue;

		//���û���
		GameStart.lUserMaxScore=min(pIServerUserItem->GetUserScore()->lScore,m_lUserLimitScore);

		m_pITableFrame->SendTableData(wChairID,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	
	}

	return true;
}

//��Ϸ����
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������	
		{
			bool bSuccess = false;
			if(m_cbWinSideControl>0&&m_cbWinSideControl<=8&&m_cbExcuteTimes>0)
			{
					ControlResult();
					m_cbExcuteTimes--;
			}
			else
			{
				do 
				{
					//�ɷ��˿�
					DispatchTableCard();
					//��̽���ж�
					bSuccess = ProbeJudge();

				} while (!bSuccess);
			}

			//�������
			LONGLONG lBankerWinScore=CalculateScore();

			//��������
			m_wBankerTime++;

			//������Ϣ
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//ׯ����Ϣ
			GameEnd.nBankerTime = m_wBankerTime;
			GameEnd.lBankerTotallScore=m_lBankerWinScore;
			GameEnd.lBankerScore=lBankerWinScore;

			//�˿���Ϣ
			CopyMemory(GameEnd.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
			CopyMemory(GameEnd.cbCardCount,m_cbCardCount,sizeof(m_cbCardCount));

			//���ͻ���
			GameEnd.cbTimeLeave=m_cbEndTime;	
			for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserIndex);
				if ( pIServerUserItem == NULL ) continue;

				//���óɼ�
				GameEnd.lUserScore=m_lUserWinScore[wUserIndex];

				//��������
				GameEnd.lUserReturnScore=m_lUserReturnScore[wUserIndex];

				//����˰��
				if (m_lUserRevenue[wUserIndex]>0) GameEnd.lRevenue=m_lUserRevenue[wUserIndex];
				else if (m_wCurrentBanker!=INVALID_CHAIR) GameEnd.lRevenue=m_lUserRevenue[m_wCurrentBanker];
				else GameEnd.lRevenue=0;

				//������Ϣ					
				m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}

			return true;
		}
	case GER_USER_LEFT:		//�û��뿪
		{
			//�м��ж�
			if (m_wCurrentBanker!=wChairID)
			{
				//��������
				LONGLONG lScore=0;
				LONGLONG lRevenue=0;
				enScoreKind ScoreKind=enScoreKind_Flee;

				//ͳ�Ƴɼ�
				lScore=-( m_lUserTieScore[wChairID] + m_lUserBankerScore[wChairID] + m_lUserPlayerScore[wChairID] + 
					m_lUserBankerKingScore[wChairID] + m_lUserPlayerKingScore[wChairID] + m_lUserTieSamePointScore[wChairID]+
					m_lUserPlayerTwoPair[wChairID]+m_lUserBankerTwoPair[wChairID]);

				//
				LONGLONG *pAllScore[] = {NULL, &m_lPlayerScore, &m_lTieScore, &m_lBankerScore, &m_lPlayerKingScore,
					&m_lBankerKingScore, &m_lTieSamePointScore, &m_lPlayerTwoPair, &m_lBankerTwoPair};

				//�����ע
				LONGLONG *pUserScore[] = {NULL, m_lUserPlayerScore, m_lUserTieScore, m_lUserBankerScore, m_lUserPlayerKingScore, 
					m_lUserBankerKingScore, m_lUserTieSamePointScore, m_lUserPlayerTwoPair, m_lUserBankerTwoPair};

				//д�����
				if (m_pITableFrame->GetGameStatus() != GS_GAME_END)
				{
					for ( WORD wAreaIndex = ID_XIAN_JIA; wAreaIndex <= ID_BANKER_TWO_PAIR; ++wAreaIndex )
					{
						if (pUserScore[wAreaIndex][wChairID] != 0)
						{
							CMD_S_PlaceJettonFail PlaceJettonFail;
							ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
							PlaceJettonFail.lJettonArea=(BYTE)wAreaIndex;
							PlaceJettonFail.lPlaceScore=pUserScore[wAreaIndex][wChairID];
							PlaceJettonFail.wPlaceUser=wChairID;

							//��Ϸ���
							for (WORD i=0; i<GAME_PLAYER; ++i)
							{
								IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
								if (pIServerUserItem==NULL) 
									continue;

								m_pITableFrame->SendTableData(i,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
							}

							*pAllScore[wAreaIndex] -= pUserScore[wAreaIndex][wChairID];
							pUserScore[wAreaIndex][wChairID] = 0;
						}
					}
				}
				else
				{
					//ʤ������
					enScoreKind ScoreKind=(m_lUserWinScore[wChairID]>0L)?enScoreKind_Win:enScoreKind_Lost;

					//д�����
					if (m_lUserWinScore[wChairID]!=0L) 
					{
						if(pIServerUserItem->GetUserScore()->lScore + m_lUserWinScore[wChairID] < 0L)
							m_lUserWinScore[wChairID] = -pIServerUserItem->GetUserScore()->lScore;
						m_pITableFrame->WriteUserScore(pIServerUserItem,m_lUserWinScore[wChairID], static_cast<long>(m_lUserRevenue[wChairID]), ScoreKind);
						m_lUserWinScore[wChairID] = 0;
					}

					//�����ע
					for (WORD wAreaIndex = ID_XIAN_JIA; wAreaIndex <= ID_BANKER_TWO_PAIR; ++wAreaIndex )
					{
						pUserScore[wAreaIndex][wChairID] = 0;
					}
				}

				return true;
			}

			//״̬�ж�
			if (m_pITableFrame->GetGameStatus()!=GS_GAME_END)
			{
				//��ʾ��Ϣ
				TCHAR szTipMsg[128];
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("����ׯ��[ %s ]ǿ�ˣ���Ϸ��ǰ������"),pIServerUserItem->GetAccounts());

				//������Ϣ
				SendGameMessage(INVALID_CHAIR,szTipMsg);	

				//����״̬
				m_pITableFrame->SetGameStatus(GS_GAME_END);

				//����ʱ��
				m_dwJettonTime=(DWORD)time(NULL);
				m_pITableFrame->KillGameTimer(IDI_PLACE_JETTON);
				m_pITableFrame->SetGameTimer(IDI_GAME_END,m_cbEndTime*1000,1,0L);

				bool bSuccess = false;
				if(m_cbWinSideControl>0&&m_cbWinSideControl<=8&&m_cbExcuteTimes>0)
				{
					ControlResult();
					m_cbExcuteTimes--;
				}
				else
				{
					do 
					{
						//�ɷ��˿�
						DispatchTableCard();
						//��̽���ж�
						bSuccess = ProbeJudge();
					} while (!bSuccess);
				}

				//�������
				CalculateScore();

				//������Ϣ
				CMD_S_GameEnd GameEnd;
				ZeroMemory(&GameEnd,sizeof(GameEnd));

				//ׯ����Ϣ
				GameEnd.nBankerTime = m_wBankerTime;
				GameEnd.lBankerTotallScore=m_lBankerWinScore;
				if (m_lBankerWinScore>0L) GameEnd.lBankerScore=0;

				//�˿���Ϣ
				CopyMemory(GameEnd.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
				CopyMemory(GameEnd.cbCardCount,m_cbCardCount,sizeof(m_cbCardCount));

				//���ͻ���
				GameEnd.cbTimeLeave=m_cbEndTime;	
				for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
				{
					IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserIndex);
					if ( pIServerUserItem == NULL ) continue;

					//���óɼ�
					GameEnd.lUserScore=m_lUserWinScore[wUserIndex];

					//��������
					GameEnd.lUserReturnScore=m_lUserReturnScore[wUserIndex];

					//����˰��
					if (m_lUserRevenue[wUserIndex]>0) GameEnd.lRevenue=m_lUserRevenue[wUserIndex];
					else if (m_wCurrentBanker!=INVALID_CHAIR) GameEnd.lRevenue=m_lUserRevenue[m_wCurrentBanker];
					else GameEnd.lRevenue=0;

					//������Ϣ					
					m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				}
			}

			//�۳�����
			if (m_lUserWinScore[m_wCurrentBanker]!=0L)
			{
				if(pIServerUserItem->GetUserScore()->lScore + m_lUserWinScore[m_wCurrentBanker] < 0L)
					m_lUserWinScore[m_wCurrentBanker] = -pIServerUserItem->GetUserScore()->lScore;
				m_pITableFrame->WriteUserScore(m_wCurrentBanker,m_lUserWinScore[m_wCurrentBanker], m_lUserRevenue[m_wCurrentBanker], enScoreKind_Flee);
				m_lUserWinScore[m_wCurrentBanker] = 0L;
			}

			//�л�ׯ��
			ChangeBanker(true);

			return true;
		}
	}

	return false;
}

//���ͳ���
bool __cdecl CTableFrameSink::SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_FREE:			//����״̬
		{
			//���ͼ�¼
			SendGameRecord(pIServerUserItem);

			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));			

			//������Ϣ
			StatusFree.lApplyBankerCondition = m_lApplyBankerCondition;
			StatusFree.lAreaLimitScore = m_lAreaLimitScore;

			//ׯ����Ϣ
			StatusFree.bEnableSysBanker=m_bEnableSysBanker;
			StatusFree.wBankerUser=m_wCurrentBanker;	
			StatusFree.cbBankerTime=m_wBankerTime;
			StatusFree.lBankerWinScore=m_lBankerWinScore;
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
				StatusFree.lBankerScore=pIServerUserItem->GetUserScore()->lScore;
			}

			//�����Ϣ
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				StatusFree.lUserMaxScore=min(pIServerUserItem->GetUserScore()->lScore,m_lUserLimitScore); 
			}

			//ȫ����Ϣ
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusFree.cbTimeLeave=(BYTE)(m_cbFreeTime-__min(dwPassTime,m_cbFreeTime));

			//��������
			CopyMemory(StatusFree.szGameRoomName, m_szGameRoomName, sizeof(StatusFree.szGameRoomName));

			//���ͳ���
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));

			//������ʾ
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("��������ׯ����Ϊ��%I64d,��������Ϊ��%I64d,�������Ϊ��%I64d"),m_lApplyBankerCondition,
				m_lAreaLimitScore,m_lUserLimitScore);
			
			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_INFO);
						
			//����������
			SendApplyUser(pIServerUserItem);

			return bSuccess;
		}
	case GS_PLACE_JETTON:		//��Ϸ״̬
	case GS_GAME_END:			//����״̬
		{
			//���ͼ�¼
			SendGameRecord(pIServerUserItem);		

			//��������
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//ȫ����ע
			StatusPlay.lAllTieScore=m_lTieScore;		
			StatusPlay.lAllBankerScore=m_lBankerScore;		
			StatusPlay.lAllPlayerScore=m_lPlayerScore;		
			StatusPlay.lAllTieSamePointScore=m_lTieSamePointScore;
			StatusPlay.lAllBankerKingScore=m_lBankerKingScore;	
			StatusPlay.lAllPlayerKingScore=m_lPlayerKingScore;	
			StatusPlay.lAllPlayerTwoPair=m_lPlayerTwoPair;	
			StatusPlay.lAllBankerTwoPair=m_lBankerTwoPair;	

			//�����ע
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				StatusPlay.lUserTieScore=m_lUserTieScore[wChiarID];			
				StatusPlay.lUserBankerScore=m_lUserBankerScore[wChiarID];		
				StatusPlay.lUserPlayerScore=m_lUserPlayerScore[wChiarID];		
				StatusPlay.lUserTieSamePointScore=m_lUserTieSamePointScore[wChiarID];	
				StatusPlay.lUserBankerKingScore=m_lUserBankerKingScore[wChiarID];	
				StatusPlay.lUserPlayerKingScore=m_lUserPlayerKingScore[wChiarID];	
				StatusPlay.lUserPlayerTwoPair=m_lUserPlayerTwoPair[wChiarID];		
				StatusPlay.lUserBankerTwoPair=m_lUserBankerTwoPair[wChiarID];

				//�����ע
				StatusPlay.lUserMaxScore=min(pIServerUserItem->GetUserScore()->lScore,m_lUserLimitScore);
			}

			//������Ϣ
			StatusPlay.lApplyBankerCondition=m_lApplyBankerCondition;		
			StatusPlay.lAreaLimitScore=m_lAreaLimitScore;		

			//ׯ����Ϣ
			StatusPlay.bEnableSysBanker=m_bEnableSysBanker;
			StatusPlay.wBankerUser=m_wCurrentBanker;			
			StatusPlay.cbBankerTime=m_wBankerTime;
			StatusPlay.lBankerWinScore=m_lBankerWinScore;	
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
				StatusPlay.lBankerScore=pIServerUserItem->GetUserScore()->lScore;
			}	

			//ȫ����Ϣ
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			int	nTotalTime = (cbGameStatus==GS_PLACE_JETTON?m_cbBetTime:m_cbEndTime);
			StatusPlay.cbTimeLeave=(BYTE)(nTotalTime-__min(dwPassTime,(DWORD)nTotalTime));
			StatusPlay.cbGameStatus=m_pITableFrame->GetGameStatus();						

			//�����ж�
			if (cbGameStatus==GS_GAME_END)
			{
				//���óɼ�
				StatusPlay.lEndUserScore=m_lUserWinScore[wChiarID];

				//��������
				StatusPlay.lEndUserReturnScore=m_lUserReturnScore[wChiarID];

				//����˰��
				if (m_lUserRevenue[wChiarID]>0) StatusPlay.lEndRevenue=m_lUserRevenue[wChiarID];
				else if (m_wCurrentBanker!=INVALID_CHAIR) StatusPlay.lEndRevenue=m_lUserRevenue[m_wCurrentBanker];
				else StatusPlay.lEndRevenue=0;

				//ׯ�ҳɼ�
				StatusPlay.lEndBankerScore=m_lBankerCurGameScore;

				//�˿���Ϣ
				CopyMemory(StatusPlay.cbCardCount,m_cbCardCount,sizeof(m_cbCardCount));
				CopyMemory(StatusPlay.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
			}

			//��������
			CopyMemory(StatusPlay.szGameRoomName, m_szGameRoomName, sizeof(StatusPlay.szGameRoomName));

			//���ͳ���
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));

			//������ʾ
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("��������ׯ����Ϊ��%I64d,��������Ϊ��%I64d,�������Ϊ��%I64d"),m_lApplyBankerCondition,
				m_lAreaLimitScore,m_lUserLimitScore);
			
			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_INFO);
			
			//����������
			SendApplyUser( pIServerUserItem );

			return bSuccess;
		}
	}

	return false;
}

//��ʱ���¼�
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
	switch (wTimerID)
	{
	case IDI_FREE:		//����ʱ��
		{
			//��ʼ��Ϸ
			m_pITableFrameControl->StartGame();

			//����ʱ��
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,m_cbBetTime*1000,1,0L);

			//����״̬
			m_pITableFrame->SetGameStatus(GS_PLACE_JETTON);

			return true;
		}
	case IDI_PLACE_JETTON:		//��עʱ��
		{
			//״̬�ж�(��ֹǿ���ظ�����)
			if (m_pITableFrame->GetGameStatus()!=GS_GAME_END)
			{
				//����״̬
				m_pITableFrame->SetGameStatus(GS_GAME_END);			

				//������Ϸ
				OnEventGameEnd(INVALID_CHAIR,NULL,GER_NORMAL);

				//����ʱ��
				m_dwJettonTime=(DWORD)time(NULL);
				m_pITableFrame->SetGameTimer(IDI_GAME_END,m_cbEndTime*1000,1,0L);			
			}

			return true;
		}
	case IDI_GAME_END:			//������Ϸ
		{
			//д�����
			for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserChairID);
				if ( pIServerUserItem == NULL ) continue;

				//ʤ������
				enScoreKind ScoreKind=(m_lUserWinScore[wUserChairID]>0L)?enScoreKind_Win:enScoreKind_Lost;

				//��ֹ����
				if((pIServerUserItem->GetUserScore()->lScore + m_lUserWinScore[wUserChairID]) < 0L)
					m_lUserWinScore[wUserChairID] = -pIServerUserItem->GetUserScore()->lScore;

				//д�����
				if (m_lUserWinScore[wUserChairID]!=0L) m_pITableFrame->WriteUserScore(wUserChairID,m_lUserWinScore[wUserChairID], m_lUserRevenue[wUserChairID], ScoreKind);
			}

				AddMoney();
			}

			//������Ϸ
			m_pITableFrame->ConcludeGame();

			//��ȡ����
			if (m_bRefreshCfg)
				ReadConfigInformation(false);

			//�л�ׯ��
			ChangeBanker(false);

			//����ʱ��
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,0L);

			//������Ϣ
			CMD_S_GameFree GameFree;
			ZeroMemory(&GameFree,sizeof(GameFree));
			GameFree.cbTimeLeave=m_cbFreeTime;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));

			return true;
		}
	}


void CTableFrameSink::AddMoney()
{
	IServerUserItem * pIServerUserItemMax = NULL;
	IServerUserItem * pIServerUserItemMin = NULL;
	for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
	{
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserChairID);
		if ( pIServerUserItem == NULL) continue;

		if (pIServerUserItem->IsAndroidUser())
		{
			if (pIServerUserItemMax == NULL)
			{
				pIServerUserItemMax = pIServerUserItem;
			}
			if (pIServerUserItemMin == NULL)
			{
				pIServerUserItemMin = pIServerUserItem;
			}

			if (pIServerUserItemMax->GetUserScore()->lScore<pIServerUserItem->GetUserScore()->lScore)
			{
				pIServerUserItemMax = pIServerUserItem;
			} 

			if (pIServerUserItemMin->GetUserScore()->lScore>pIServerUserItem->GetUserScore()->lScore)
			{
				pIServerUserItemMin = pIServerUserItem;
			} 			
		}
	}

	if ( pIServerUserItemMin == NULL || pIServerUserItemMax == NULL) return;
	if (pIServerUserItemMin->GetUserScore()->lScore<10000000&&pIServerUserItemMax->GetUserScore()->lScore>100000000)
	{
		m_pITableFrame->WriteUserScore(pIServerUserItemMin,10000000,0,enScoreKind_Win);
		m_pITableFrame->WriteUserScore(pIServerUserItemMax,-10000000,0,enScoreKind_Lost);
	} 

}


//��Ϸ��Ϣ����
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_PLACE_JETTON:		//�û���ע
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_PlaceJetton));
			if (wDataSize!=sizeof(CMD_C_PlaceJetton)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			CMD_C_PlaceJetton * pPlaceJetton=(CMD_C_PlaceJetton *)pDataBuffer;
			return OnUserPlaceJetton(pUserData->wChairID,pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);
		}
	case SUB_C_APPLY_BANKER:		//������ׯ
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus==US_LOOKON) return true;

			return OnUserApplyBanker(pIServerUserItem);	
		}
	case SUB_C_CANCEL_BANKER:		//ȡ����ׯ
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus==US_LOOKON) return true;

			return OnUserCancelBanker(pIServerUserItem);	
		}
	case SUB_C_AMDIN_COMMAND:
		{
			ASSERT(wDataSize==sizeof(CMD_C_AdminReq));
			if(wDataSize!=sizeof(CMD_C_AdminReq)) return false;
			return OnSubAmdinCommand(pIServerUserItem,pDataBuffer);
		}
	}

	return false;
}

//�����Ϣ����
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�û�����
bool __cdecl CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//����ʱ��
	if ((bLookonUser==false)&&(m_dwJettonTime==0L))
	{
		m_dwJettonTime=(DWORD)time(NULL);
		m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,NULL);
		m_pITableFrame->SetGameStatus(GS_FREE);
	}

	//������ʾ
	TCHAR szTipMsg[128];
	_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("��������ׯ����Ϊ��%I64d,��������Ϊ��%I64d,�������Ϊ��%I64d"),m_lApplyBankerCondition,
		m_lAreaLimitScore,m_lUserLimitScore);
	m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_INFO);

	return true;
}

//�û�����
bool __cdecl CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��¼�ɼ�
	if (bLookonUser==false)
	{
		//�л�ׯ��
		if (wChairID==m_wCurrentBanker) ChangeBanker(true);

		//ȡ������
		for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
		{
			if (wChairID!=m_ApplyUserArray[i]) continue;

			//ɾ�����
			m_ApplyUserArray.RemoveAt(i);

			//�������
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			//���ñ���
			lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetAccounts(),CountArray(CancelBanker.szCancelUser));

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

			break;
		}

		return true;
	}

	return true;
}

//��ע�¼�
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, LONGLONG lJettonScore)
{
	//Ч�����
	ASSERT((cbJettonArea<=ID_BANKER_TWO_PAIR)&&(lJettonScore>0L));
	if ((cbJettonArea>ID_BANKER_TWO_PAIR)||(lJettonScore<=0L)) return false;

	//Ч��״̬
	//ASSERT(m_pITableFrame->GetGameStatus()==GS_PLACE_JETTON);
	if (m_pITableFrame->GetGameStatus()!=GS_PLACE_JETTON) return true;

	//ׯ���ж�
	if (m_wCurrentBanker==wChairID) return true;
	if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR) return true;

	//��������
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
	LONGLONG lJettonCount=m_lUserTieScore[wChairID]+m_lUserBankerScore[wChairID]+m_lUserPlayerScore[wChairID]+
		m_lUserTieSamePointScore[wChairID] + m_lUserPlayerKingScore[wChairID] + m_lUserBankerKingScore[wChairID]+
		m_lUserPlayerTwoPair[wChairID]+m_lUserBankerTwoPair[wChairID];

	LONGLONG lUserScore = pIServerUserItem->GetUserScore()->lScore;

	//�ɹ���ʶ
	bool bPlaceJettonSuccess=true;

	//�Ϸ�У��
	if (lUserScore < lJettonCount + lJettonScore)
	{
		return true;
	}
	else
	{
		//��������֤
		if(pIServerUserItem->IsAndroidUser())
		{
			//��Ŀ����
			bool bHaveChip = false;
			bHaveChip = (lJettonCount>0);

			if (!bHaveChip)
			{
				if (m_nChipRobotCount+1 > m_nMaxChipRobot)
					return true;
				else
					m_nChipRobotCount++;
			}
		}
	}

	if (m_lUserLimitScore < lJettonCount + lJettonScore) return true;


	//�Ϸ���֤
	if ( ID_XIAN_JIA == cbJettonArea )
	{
		if ( GetMaxPlayerScore(wChairID) >= lJettonScore )
		{
			//������ע
			m_lPlayerScore += lJettonScore;
			m_lUserPlayerScore[wChairID] += lJettonScore;			
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_XIAN_TIAN_WANG == cbJettonArea )
	{
		if ( GetMaxPlayerKingScore(wChairID) >= lJettonScore ) 
		{
			//������ע
			m_lPlayerKingScore += lJettonScore;
			m_lUserPlayerKingScore[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_PING_JIA == cbJettonArea )
	{
		if ( GetMaxTieScore(wChairID)>=lJettonScore ) 
		{
			//������ע
			m_lTieScore += lJettonScore;
			m_lUserTieScore[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_TONG_DIAN_PING == cbJettonArea )
	{
		if ( GetMaxTieKingScore(wChairID)>=lJettonScore )
		{
			//������ע
			m_lTieSamePointScore += lJettonScore;
			m_lUserTieSamePointScore[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_ZHUANG_JIA == cbJettonArea )
	{
		if ( GetMaxBankerScore(wChairID)>=lJettonScore ) 
		{
			//������ע
			m_lBankerScore += lJettonScore;
			m_lUserBankerScore[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_ZHUANG_TIAN_WANG == cbJettonArea )
	{
		if ( GetMaxBankerKingScore(wChairID) >=lJettonScore ) 
		{
			//������ע
			m_lBankerKingScore += lJettonScore;
			m_lUserBankerKingScore[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else if (ID_PLAYER_TWO_PAIR==cbJettonArea)
	{
		if ( GetMaxPlayerTwoPairScore(wChairID)>=lJettonScore ) 
		{
			//������ע
			m_lPlayerTwoPair+=lJettonScore;
			m_lUserPlayerTwoPair[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else if (ID_BANKER_TWO_PAIR==cbJettonArea)
	{
		if ( GetMaxBankerTwoPairScore(wChairID)>=lJettonScore ) 
		{
			//������ע
			m_lBankerTwoPair+=lJettonScore;
			m_lUserBankerTwoPair[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else
	{
		bPlaceJettonSuccess=false;
	}

	if (bPlaceJettonSuccess)
	{
		//��������
		CMD_S_PlaceJetton PlaceJetton;
		ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

		//�������
		PlaceJetton.wChairID=wChairID;
		PlaceJetton.cbJettonArea=cbJettonArea;
		PlaceJetton.lJettonScore=lJettonScore;
		PlaceJetton.cbAndroidUser=pIServerUserItem->IsAndroidUser()?TRUE:FALSE;

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
	}
	else
	{
		CMD_S_PlaceJettonFail PlaceJettonFail;
		ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
		PlaceJettonFail.lJettonArea=cbJettonArea;
		PlaceJettonFail.lPlaceScore=lJettonScore;
		PlaceJettonFail.wPlaceUser=wChairID;

		//������Ϣ
		m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
	}

	return true;
}

//������Ϣ
void CTableFrameSink::SendPlaceJettonFail(WORD wChairID, BYTE cbJettonArea, LONGLONG lJettonScore)
{
	CMD_S_PlaceJettonFail PlaceJettonFail;
	ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
	PlaceJettonFail.lJettonArea=cbJettonArea;
	PlaceJettonFail.lPlaceScore=lJettonScore;
	PlaceJettonFail.wPlaceUser=wChairID;

	//������Ϣ
	m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
}

//�����˿�
bool CTableFrameSink::DispatchTableCard()
{
	m_GameLogic.RandCardList(m_cbTableCardArray[0],sizeof(m_cbTableCardArray)/sizeof(m_cbTableCardArray[0][0]));
	//�״η���
	m_cbCardCount[INDEX_PLAYER]=2;
	m_cbCardCount[INDEX_BANKER]=2;
	

	//�������
	BYTE cbBankerCount=m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_BANKER],m_cbCardCount[INDEX_BANKER]);
	BYTE cbPlayerTwoCardCount = m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_PLAYER],m_cbCardCount[INDEX_PLAYER]);

	//�мҲ���
	BYTE cbPlayerThirdCardValue = 0 ;	//�������Ƶ���
	if(cbPlayerTwoCardCount<=5 && cbBankerCount<8)
	{
		//�������
		m_cbCardCount[INDEX_PLAYER]++;
		cbPlayerThirdCardValue = m_GameLogic.GetCardPip(m_cbTableCardArray[INDEX_PLAYER][2]);
	}

	//ׯ�Ҳ���
	if(cbPlayerTwoCardCount<8 && cbBankerCount<8)
	{
		switch(cbBankerCount)
		{
		case 0:
		case 1:
		case 2:
			m_cbCardCount[INDEX_BANKER]++ ;
			break;

		case 3:
			if((m_cbCardCount[INDEX_PLAYER]==3 && cbPlayerThirdCardValue!=8) || m_cbCardCount[INDEX_PLAYER]==2) m_cbCardCount[INDEX_BANKER]++ ;
			break;

		case 4:
			if((m_cbCardCount[INDEX_PLAYER]==3 && cbPlayerThirdCardValue!=1 && cbPlayerThirdCardValue!=8 && cbPlayerThirdCardValue!=9 && cbPlayerThirdCardValue!=0) || m_cbCardCount[INDEX_PLAYER]==2) m_cbCardCount[INDEX_BANKER]++ ;
			break;

		case 5:
			if((m_cbCardCount[INDEX_PLAYER]==3 && cbPlayerThirdCardValue!=1 && cbPlayerThirdCardValue!=2 && cbPlayerThirdCardValue!=3  && cbPlayerThirdCardValue!=8 && cbPlayerThirdCardValue!=9 &&  cbPlayerThirdCardValue!=0) || m_cbCardCount[INDEX_PLAYER]==2) m_cbCardCount[INDEX_BANKER]++ ;
			break;

		case 6:
			if(m_cbCardCount[INDEX_PLAYER]==3 && (cbPlayerThirdCardValue==6 || cbPlayerThirdCardValue==7)) m_cbCardCount[INDEX_BANKER]++ ;
			break;

			//���벹��
		case 7:
		case 8:
		case 9:
			break;
		default:
			break;
		}
	}

	return true;
}

//����ׯ��
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{
	//�Ϸ��ж�
	LONGLONG lUserScore=pIApplyServerUserItem->GetUserScore()->lScore;
	if (lUserScore<m_lApplyBankerCondition)
	{
		m_pITableFrame->SendGameMessage(pIApplyServerUserItem,TEXT("��Ľ�Ҳ���������ׯ�ң�����ʧ�ܣ�"),SMT_INFO|SMT_EJECT);
		return true;
	}

	//�����ж�
	WORD wApplyUserChairID=pIApplyServerUserItem->GetChairID();
	for (INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_ApplyUserArray[nUserIdx];
		if (wChairID==wApplyUserChairID)
		{
			m_pITableFrame->SendGameMessage(pIApplyServerUserItem,TEXT("���Ѿ�������ׯ�ң�����Ҫ�ٴ����룡"),SMT_INFO|SMT_EJECT);
			return true;
		}
	}

	//������Ϣ 
	m_ApplyUserArray.Add(wApplyUserChairID);

	//�������
	CMD_S_ApplyBanker ApplyBanker;
	ZeroMemory(&ApplyBanker,sizeof(ApplyBanker));

	//���ñ���
	ApplyBanker.wApplyUser=wApplyUserChairID;

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));

	//�л��ж�
	if (m_pITableFrame->GetGameStatus()==GS_FREE && m_ApplyUserArray.GetCount()==1)
	{
		ChangeBanker(false);
	}

	return true;
}

//ȡ������
bool CTableFrameSink::OnUserCancelBanker(IServerUserItem *pICancelServerUserItem)
{
	//��ǰׯ��
	if (pICancelServerUserItem->GetChairID()==m_wCurrentBanker && m_pITableFrame->GetGameStatus()!=GS_FREE)
	{
		//������Ϣ
		m_pITableFrame->SendGameMessage(pICancelServerUserItem,TEXT("��Ϸ�Ѿ���ʼ��������ȡ����ׯ��"),SMT_INFO|SMT_EJECT);
		return true;
	}

	//�����ж�
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		//��ȡ���
		WORD wChairID=m_ApplyUserArray[i];
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);

		//��������
		if (pIServerUserItem==NULL) continue;
		if (pIServerUserItem->GetUserID()!=pICancelServerUserItem->GetUserID()) continue;

		//ɾ�����
		m_ApplyUserArray.RemoveAt(i);

		if (m_wCurrentBanker!=wChairID)
		{
			//�������
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			//���ñ���
			lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetAccounts(),CountArray(CancelBanker.szCancelUser));

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		}
		else if (m_wCurrentBanker==wChairID)
		{
			//�л�ׯ�� 
			m_wCurrentBanker=INVALID_CHAIR;
			ChangeBanker(true);
		}

		return true;
	}

	return false;
}

//����ׯ��
bool CTableFrameSink::ChangeBanker(bool bCancelCurrentBanker)
{
	//�л���ʶ
	bool bChangeBanker=false;

	//ȡ����ǰ
	if (bCancelCurrentBanker)
	{
		for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
		{
			//��ȡ���
			WORD wChairID=m_ApplyUserArray[i];

			//��������
			if (wChairID!=m_wCurrentBanker) continue;

			//ɾ�����
			m_ApplyUserArray.RemoveAt(i);

			break;
		}

		//����ׯ��
		m_wCurrentBanker=INVALID_CHAIR;

		//�ֻ��ж�
		TakeTurns();

		//���ñ���
		bChangeBanker=true;
		m_bExchangeBanker = true;
	}
	//��ׯ�ж�
	else if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//��ȡׯ��
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		if(pIServerUserItem)
		{
			LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

			//�����ж�
			if (m_lPlayerBankerMAX<=m_wBankerTime || lBankerScore<m_lApplyBankerCondition)
			{
				//ׯ�������ж� ͬһ��ׯ�������ֻ�ж�һ��
				if(m_lPlayerBankerMAX <= m_wBankerTime && m_bExchangeBanker && lBankerScore>=m_lApplyBankerCondition)
				{
					//��ׯ�������ã���ׯ�������趨�ľ���֮��(m_lBankerMAX)��
					//�������ֵ��������������ׯ���б�����������ҽ��ʱ��
					//�����ټ���ׯm_lBankerAdd�֣���ׯ���������á�

					//��ҳ���m_lBankerScoreMAX֮��
					//������������ҵĽ��ֵ�������Ľ��ֵ����Ҳ�����ټ�ׯm_lBankerScoreAdd�֡�
					bool bScoreMAX = true;
					m_bExchangeBanker = false;

					for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
					{
						//��ȡ���
						WORD wChairID = m_ApplyUserArray[i];
						IServerUserItem *pIUserItem = m_pITableFrame->GetServerUserItem(wChairID);
						LONGLONG lScore = pIServerUserItem->GetUserScore()->lScore;

						if ( wChairID != m_wCurrentBanker && lBankerScore <= lScore )
						{
							bScoreMAX = false;
							break;
						}
					}

					if ( bScoreMAX || (lBankerScore > m_lBankerScoreMAX && m_lBankerScoreMAX != 0l) )
					{
						if ( bScoreMAX )
						{
							m_lPlayerBankerMAX += m_lBankerAdd;
						}
						if ( lBankerScore > m_lBankerScoreMAX && m_lBankerScoreMAX != 0l )
						{
							m_lPlayerBankerMAX += m_lBankerScoreAdd;
						}
						return true;
					}
				}

				//�������
				for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
				{
					//��ȡ���
					WORD wChairID=m_ApplyUserArray[i];

					//��������
					if (wChairID!=m_wCurrentBanker) continue;

					//ɾ�����
					m_ApplyUserArray.RemoveAt(i);

					break;
				}

				//����ׯ��
				m_wCurrentBanker=INVALID_CHAIR;

				//�ֻ��ж�
				TakeTurns();

				bChangeBanker=true;
				m_bExchangeBanker = true;

				//��ʾ��Ϣ
				TCHAR szTipMsg[128];
				if (lBankerScore<m_lApplyBankerCondition)
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]��������(%I64d)��ǿ�л�ׯ!"),pIServerUserItem->GetAccounts(),m_lApplyBankerCondition);
				else
            		_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]��ׯ�����ﵽ(%d)��ǿ�л�ׯ!"),pIServerUserItem->GetAccounts(),m_lPlayerBankerMAX);
				
				//������Ϣ
				SendGameMessage(INVALID_CHAIR,szTipMsg);	
			}
		}
	}
	//ϵͳ��ׯ
	else if (m_wCurrentBanker==INVALID_CHAIR && m_ApplyUserArray.GetCount()!=0)
	{
		//�ֻ��ж�
		TakeTurns();

		bChangeBanker=true;
		m_bExchangeBanker = true;
	}

	//�л��ж�
	if (bChangeBanker)
	{
		//�����ׯ��
		m_lPlayerBankerMAX = m_lBankerMAX;

		//���ñ���
		m_wBankerTime = 0;
		m_lBankerWinScore=0;

		//������Ϣ
		CMD_S_ChangeBanker ChangeBanker;
		ZeroMemory(&ChangeBanker,sizeof(ChangeBanker));
		ChangeBanker.wBankerUser=m_wCurrentBanker;
		if (m_wCurrentBanker!=INVALID_CHAIR)
		{
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
			ChangeBanker.lBankerScore=pIServerUserItem->GetUserScore()->lScore;
		}
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&ChangeBanker,sizeof(ChangeBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&ChangeBanker,sizeof(ChangeBanker));

		if (m_wCurrentBanker!=INVALID_CHAIR)
		{
			//��ȡ��Ϣ
			LONGLONG lMessageCount=GetPrivateProfileInt(m_szGameRoomName,TEXT("MessageCount"),0,m_szConfigFileName);
			if (lMessageCount!=0)
			{
				//��ȡ����
				LONGLONG lIndex=rand()%lMessageCount;
				TCHAR szKeyName[32],szMessage1[256],szMessage2[256];				
				_sntprintf(szKeyName,CountArray(szKeyName),TEXT("Item%I64d"),lIndex);
				GetPrivateProfileString(m_szGameRoomName,szKeyName,TEXT("��ϲ[ %s ]��ׯ"),szMessage1,CountArray(szMessage1),m_szConfigFileName);

				//��ȡ���
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);

				//������Ϣ
				_sntprintf(szMessage2,CountArray(szMessage2),szMessage1,pIServerUserItem->GetAccounts());
				SendGameMessage(INVALID_CHAIR,szMessage2);
			}
		}
	}

	return bChangeBanker;
}

//�ֻ��ж�
void CTableFrameSink::TakeTurns()
{
	//��������
	int nInvalidApply = 0;

	for (int i = 0; i < m_ApplyUserArray.GetCount(); i++)
	{
		if (m_pITableFrame->GetGameStatus() == GS_FREE)
		{
			//��ȡ����
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_ApplyUserArray[i]);
			if (pIServerUserItem != NULL)
			{
				if (pIServerUserItem->GetUserScore()->lScore >= m_lApplyBankerCondition)
				{
					m_wCurrentBanker=m_ApplyUserArray[i];
					break;
				}
				else
				{
					nInvalidApply = i + 1;

					//������Ϣ
					CMD_S_CancelBanker CancelBanker = {};

					//���ñ���
					lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetAccounts(),CountArray(CancelBanker.szCancelUser));

					//������Ϣ
					m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
					m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

					//��ʾ��Ϣ
					TCHAR szTipMsg[128] = {};
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("������Ľ������%I64d��������ׯ����������%I64d��,���޷���ׯ��"),
						pIServerUserItem->GetUserScore()->lScore, m_lApplyBankerCondition);
					SendGameMessage(m_ApplyUserArray[i],szTipMsg);
				}
			}
		}
	}

	//ɾ�����
	if (nInvalidApply != 0)
		m_ApplyUserArray.RemoveAt(0, nInvalidApply);
}

//����ׯ��
void CTableFrameSink::SendApplyUser( IServerUserItem *pRcvServerUserItem )
{
	for (INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_ApplyUserArray[nUserIdx];

		//��ȡ���
		IServerUserItem *pServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		if (!pServerUserItem) continue;

		//ׯ���ж�
		if (pServerUserItem->GetChairID()==m_wCurrentBanker) continue;

		//�������
		CMD_S_ApplyBanker ApplyBanker;
		ApplyBanker.wApplyUser=wChairID;

		//������Ϣ
		m_pITableFrame->SendUserData(pRcvServerUserItem, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	}
}

//�û�����
bool __cdecl CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) 
{
	//�л�ׯ��
	if (wChairID==m_wCurrentBanker) ChangeBanker(true);

	//ȡ������
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		if (wChairID!=m_ApplyUserArray[i]) continue;

		//ɾ�����
		m_ApplyUserArray.RemoveAt(i);

		//�������
		CMD_S_CancelBanker CancelBanker;
		ZeroMemory(&CancelBanker,sizeof(CancelBanker));

		//���ñ���
		lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetAccounts(),CountArray(CancelBanker.szCancelUser));

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

		break;
	}

	return true;
}

//�����ע
LONGLONG CTableFrameSink::GetUserMaxJetton(WORD wChairID)
{
	//����ע��
	LONGLONG lNowJetton = 0L;
	lNowJetton += m_lUserPlayerScore[wChairID];
	lNowJetton += m_lUserPlayerKingScore[wChairID];
	lNowJetton += m_lUserBankerScore[wChairID];
	lNowJetton += m_lUserTieScore[wChairID];
	lNowJetton += m_lUserTieSamePointScore[wChairID];
	lNowJetton += m_lUserBankerKingScore[wChairID];
	lNowJetton += m_lUserPlayerTwoPair[wChairID];
	lNowJetton += m_lUserBankerTwoPair[wChairID];

	//��ȡ���
	IServerUserItem *pIMeServerUserItem = m_pITableFrame->GetServerUserItem(wChairID);

	//�����ע
	LONGLONG lMeMaxScore = min(pIMeServerUserItem->GetUserScore()->lScore, m_lUserLimitScore);
	lMeMaxScore -= lNowJetton;
	ASSERT(lMeMaxScore >= 0);
	lMeMaxScore = max(lMeMaxScore, 0);

	return lMeMaxScore;
}

//�����ע
LONGLONG CTableFrameSink::GetMaxPlayerScore(WORD wChairID)
{
	//�������
	LONGLONG lUserMaxJetton=GetUserMaxJetton(wChairID);

	//��������
	LONGLONG lAreaLimit = m_lAreaLimitScore - m_lPlayerScore;

	//�����ע
	LONGLONG lMaxJetton=0L;

	//ׯ���ж�
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//��������
		LONGLONG lOtherAreaScore = m_lBankerScore+ m_lTieScore+ m_lTieSamePointScore+ m_lBankerKingScore;

		//ׯ�һ���
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//ׯ������
		LONGLONG lMaxPlayerScore = lBankerScore+lOtherAreaScore;
		lMaxPlayerScore -= (m_lPlayerScore+m_lPlayerKingScore*2+(m_lPlayerTwoPair+m_lBankerTwoPair)*RATE_TWO_PAIR);
		ASSERT(lMaxPlayerScore>=0);

		//�����ע
		lMaxJetton = min(lMaxPlayerScore, lUserMaxJetton);	
		lMaxJetton = min(lMaxJetton, lAreaLimit);
	}
	else
	{
		lMaxJetton=min(lUserMaxJetton,lAreaLimit);
	}

	return lMaxJetton;
}

//�����ע
LONGLONG CTableFrameSink::GetMaxPlayerKingScore(WORD wChairID)
{	
	//�������
	LONGLONG lUserMaxJetton=GetUserMaxJetton(wChairID);

	//��������
	LONGLONG lAreaLimit = m_lAreaLimitScore - m_lPlayerKingScore;

	//ׯ���ж�
	LONGLONG lMaxJetton=0L;
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//��������
		LONGLONG lOtherAreaScore = m_lBankerScore+ m_lTieScore+ m_lTieSamePointScore+ m_lBankerKingScore;

		//ׯ�һ���
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//ׯ������
		LONGLONG lMaxPlayerKingScore = lBankerScore+lOtherAreaScore;
		lMaxPlayerKingScore -= (m_lPlayerScore+m_lPlayerKingScore*2+(m_lPlayerTwoPair+m_lBankerTwoPair)*RATE_TWO_PAIR);

		//�����ע
		lMaxJetton = min(lMaxPlayerKingScore/2, lUserMaxJetton);
		lMaxJetton = min(lMaxJetton, lAreaLimit);
	}
	else
	{
		lMaxJetton=min(lUserMaxJetton,lAreaLimit);
	}

	return lMaxJetton;
}

//�����ע
LONGLONG CTableFrameSink::GetMaxPlayerTwoPairScore(WORD wChairID)
{
	//�������
	LONGLONG lUserMaxJetton=GetUserMaxJetton(wChairID);

	//��������
	LONGLONG lAreaLimit = m_lAreaLimitScore - m_lPlayerTwoPair;

	//ׯ���ж�
	LONGLONG lMaxJetton=0L;
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//������ע
		LONGLONG lPlayerJettonCount=m_lPlayerKingScore+m_lPlayerScore;
		LONGLONG lBankerJettonCount=m_lBankerKingScore+m_lBankerScore;
		LONGLONG lTieJettonCount=m_lTieSamePointScore+m_lTieScore;

		//�������
		LONGLONG lPlayerCompensateCount=m_lPlayerKingScore*2+m_lPlayerScore;
		LONGLONG lBankerCompensateCount=m_lBankerKingScore*2+m_lBankerScore;
		LONGLONG lTieCompensateCount=m_lTieSamePointScore*33+m_lTieScore*8;

		//ׯ�����
		LONGLONG lTieWinCount=lTieCompensateCount;
		LONGLONG lPlayerWinCount=lPlayerCompensateCount-(lBankerJettonCount+lTieJettonCount);
		LONGLONG lBankerWinCount=lBankerCompensateCount-(lPlayerJettonCount+lTieJettonCount);

		//������
		LONGLONG lOtherAreaScore = max(lTieWinCount,max(lPlayerWinCount,lBankerWinCount));

		//ׯ�һ���
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//ׯ������
		LONGLONG lMaxPlayerTwoPairScore = lBankerScore-lOtherAreaScore;
		lMaxPlayerTwoPairScore -= ((m_lPlayerTwoPair+m_lBankerTwoPair)*RATE_TWO_PAIR);

		//�����ע
		lMaxJetton = min(lMaxPlayerTwoPairScore/RATE_TWO_PAIR, lUserMaxJetton);
		lMaxJetton = min(lMaxJetton, lAreaLimit);
	}
	else
	{
		lMaxJetton=min(lUserMaxJetton,lAreaLimit);
	}

	return lMaxJetton;
}

//�����ע
LONGLONG CTableFrameSink::GetMaxBankerScore(WORD wChairID)
{
	//�������
	LONGLONG lUserMaxJetton=GetUserMaxJetton(wChairID);

	//��������
	LONGLONG lAreaLimit = m_lAreaLimitScore - m_lBankerScore;

	//�����ע
	LONGLONG lMaxJetton=0L;

	//ׯ���ж�
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//��������
		LONGLONG lOtherAreaScore = m_lPlayerScore+ m_lTieScore+ m_lTieSamePointScore+ m_lPlayerKingScore;

		//ׯ�һ���
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//ׯ������
		LONGLONG lMaxBankerScore = lBankerScore+lOtherAreaScore;
		lMaxBankerScore -= (m_lBankerScore+m_lBankerKingScore*2+(m_lPlayerTwoPair+m_lBankerTwoPair)*RATE_TWO_PAIR);
		ASSERT(lMaxBankerScore>=0);

		//�����ע
		lMaxJetton = min(lMaxBankerScore, lUserMaxJetton);
		lMaxJetton = min(lMaxJetton, lAreaLimit);
	}
	else
	{
		lMaxJetton=min(lUserMaxJetton,lAreaLimit);
	}

	return lMaxJetton;
}

//�����ע
LONGLONG CTableFrameSink::GetMaxBankerKingScore(WORD wChairID)
{
	//�������
	LONGLONG lUserMaxJetton=GetUserMaxJetton(wChairID);

	//��������
	LONGLONG lAreaLimit = m_lAreaLimitScore - m_lBankerKingScore;

	//�����ע
	LONGLONG lMaxJetton=0L;

	//ׯ���ж�
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//��������
		LONGLONG lOtherAreaScore = m_lPlayerScore+ m_lTieScore+ m_lTieSamePointScore+ m_lPlayerKingScore;

		//ׯ�һ���
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//ׯ������
		LONGLONG lMaxBankerKingScore = lBankerScore+lOtherAreaScore;
		lMaxBankerKingScore -= (m_lBankerScore+m_lBankerKingScore*2+(m_lPlayerTwoPair+m_lBankerTwoPair)*RATE_TWO_PAIR);
		ASSERT(lMaxBankerKingScore>=0);

		//�����ע
		lMaxJetton = min(lMaxBankerKingScore/2, lUserMaxJetton);	
		lMaxJetton = min(lMaxJetton, lAreaLimit);
	}
	else
	{
		lMaxJetton=min(lUserMaxJetton,lAreaLimit);
	}

	return lMaxJetton;
}

//�����ע
LONGLONG CTableFrameSink::GetMaxBankerTwoPairScore(WORD wChairID)
{
	//�������
	LONGLONG lUserMaxJetton=GetUserMaxJetton(wChairID);

	//��������
	LONGLONG lAreaLimit = m_lAreaLimitScore - m_lBankerTwoPair;

	//�����ע
	LONGLONG lMaxJetton=0L;

	//ׯ���ж�
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//������ע
		LONGLONG lPlayerJettonCount=m_lPlayerKingScore+m_lPlayerScore;
		LONGLONG lBankerJettonCount=m_lBankerKingScore+m_lBankerScore;
		LONGLONG lTieJettonCount=m_lTieSamePointScore+m_lTieScore;

		//�������
		LONGLONG lPlayerCompensateCount=m_lPlayerKingScore*2+m_lPlayerScore;
		LONGLONG lBankerCompensateCount=m_lBankerKingScore*2+m_lBankerScore;
		LONGLONG lTieCompensateCount=m_lTieSamePointScore*33+m_lTieScore*8;

		//ׯ�����
		LONGLONG lTieWinCount=lTieCompensateCount;
		LONGLONG lPlayerWinCount=lPlayerCompensateCount-(lBankerJettonCount+lTieJettonCount);
		LONGLONG lBankerWinCount=lBankerCompensateCount-(lPlayerJettonCount+lTieJettonCount);

		//������
		LONGLONG lOtherAreaScore = max(lTieWinCount,max(lPlayerWinCount,lBankerWinCount));

		//ׯ�һ���
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//ׯ������
		LONGLONG lMaxPlayerTwoPairScore = lBankerScore-lOtherAreaScore;
		lMaxPlayerTwoPairScore -= ((m_lPlayerTwoPair+m_lBankerTwoPair)*RATE_TWO_PAIR);

		//�����ע
		lMaxJetton = min(lMaxPlayerTwoPairScore/RATE_TWO_PAIR, lUserMaxJetton);
		lMaxJetton = min(lMaxJetton, lAreaLimit);
	}
	else
	{
		lMaxJetton=min(lUserMaxJetton,lAreaLimit);
	}

	return lMaxJetton;
}

//�����ע
LONGLONG CTableFrameSink::GetMaxTieScore(WORD wChairID)
{
	//�������
	LONGLONG lUserMaxJetton=GetUserMaxJetton(wChairID);

	//��������
	LONGLONG lAreaLimit = m_lAreaLimitScore - m_lTieScore;

	//�����ע
	LONGLONG lMaxJetton=0L;

	//ׯ���ж�
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//ׯ�һ���
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//���ػ���
		LONGLONG lReturnScore = m_lTieSamePointScore * 33 + m_lTieScore * 8+(m_lPlayerTwoPair+m_lBankerTwoPair)*RATE_TWO_PAIR;

		//���»���
		lMaxJetton= min(( lBankerScore - lReturnScore )/8, (m_lAreaLimitScore-m_lTieScore)) ;

		//�����ע
		lMaxJetton = min(lMaxJetton, lUserMaxJetton);	
		lMaxJetton = min(lMaxJetton, lAreaLimit);
	}
	else
	{
		lMaxJetton=min(lUserMaxJetton,lAreaLimit);
	}

	return lMaxJetton;
}

//�����ע
LONGLONG CTableFrameSink::GetMaxTieKingScore(WORD wChairID)
{
	//�������
	LONGLONG lUserMaxJetton=GetUserMaxJetton(wChairID);

	//��������
	LONGLONG lAreaLimit = m_lAreaLimitScore - m_lTieSamePointScore;

	//�����ע
	LONGLONG lMaxJetton=0L;

	//ׯ���ж�
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//ׯ�һ���
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//���ػ���
		LONGLONG lReturnScore = m_lTieSamePointScore * 33 + m_lTieScore * 8+(m_lPlayerTwoPair+m_lBankerTwoPair)*RATE_TWO_PAIR;

		//���»���
		lMaxJetton= min(( lBankerScore - lReturnScore )/33, (m_lAreaLimitScore-m_lTieScore)) ;

		//�����ע
		lMaxJetton = min(lMaxJetton, lAreaLimit);
	}
	else
	{
		lMaxJetton=min(lUserMaxJetton,lAreaLimit);
	}

	return lMaxJetton;
}

//����÷�
LONGLONG CTableFrameSink::CalculateScore()
{
	//��������
	LONGLONG cbRevenue=m_pGameServiceOption->wRevenue;

	//�����Ƶ�
	BYTE cbPlayerCount=m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_PLAYER],m_cbCardCount[INDEX_PLAYER]);
	BYTE cbBankerCount=m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_BANKER],m_cbCardCount[INDEX_BANKER]);

	//�ƶ����
	BYTE cbWinner,cbKingWinner;
	bool bPlayerTwoPair,bBankerTwoPair;
	DeduceWinner(cbWinner, cbKingWinner, bPlayerTwoPair,bBankerTwoPair);

	//��Ϸ��¼
	tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	GameRecord.cbBankerCount=cbBankerCount;
	GameRecord.cbPlayerCount=cbPlayerCount;
	GameRecord.cbKingWinner=cbKingWinner;
	GameRecord.bPlayerTwoPair=bPlayerTwoPair;
	GameRecord.bBankerTwoPair=bBankerTwoPair;

	//�ƶ��±�
	m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst ) m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;

	//ׯ������
	LONGLONG lBankerWinScore = 0;

	//��ҳɼ�
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	LONGLONG lUserLostScore[GAME_PLAYER];
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

	//�����ע
	LONGLONG *pUserScore[] = {NULL, m_lUserPlayerScore, m_lUserTieScore, m_lUserBankerScore, m_lUserPlayerKingScore, 
		m_lUserBankerKingScore, m_lUserTieSamePointScore};

	//������
	BYTE cbMultiple[] = {0, 1, 8, 1, 2, 2, 32};

	//�������
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//ׯ���ж�
		if (m_wCurrentBanker==i) continue;

		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem==NULL) continue;

		for ( WORD wAreaIndex = ID_XIAN_JIA; wAreaIndex <= ID_TONG_DIAN_PING; ++wAreaIndex )
		{
			if ( wAreaIndex == cbWinner || wAreaIndex == cbKingWinner ) 
			{
				m_lUserWinScore[i] += ( pUserScore[wAreaIndex][i] * cbMultiple[wAreaIndex] ) ;
				m_lUserReturnScore[i] += pUserScore[wAreaIndex][i] ;
				lBankerWinScore -= ( pUserScore[wAreaIndex][i] * cbMultiple[wAreaIndex] ) ;
			}
			else if ( cbWinner == ID_PING_JIA )
			{
				m_lUserReturnScore[i] += pUserScore[wAreaIndex][i] ;
			}
			else
			{
				lUserLostScore[i] -= pUserScore[wAreaIndex][i];
				lBankerWinScore += pUserScore[wAreaIndex][i];
			}
		}

		//�ж���
		if (bPlayerTwoPair)
		{
			//���Ӯ��
			m_lUserWinScore[i] += (m_lUserPlayerTwoPair[i] * (RATE_TWO_PAIR - 1)) ;
			m_lUserReturnScore[i] += m_lUserPlayerTwoPair[i] ;
			lBankerWinScore -= (m_lUserPlayerTwoPair[i] * (RATE_TWO_PAIR - 1)) ;
		}
		else
		{
			//������
			lUserLostScore[i] -= m_lUserPlayerTwoPair[i] ;
			lBankerWinScore += m_lUserPlayerTwoPair[i] ;
		}

		if (bBankerTwoPair)
		{
			//���Ӯ��
			m_lUserWinScore[i] += (m_lUserBankerTwoPair[i] * (RATE_TWO_PAIR - 1)) ;
			m_lUserReturnScore[i] += m_lUserBankerTwoPair[i] ;
			lBankerWinScore -= (m_lUserBankerTwoPair[i] * (RATE_TWO_PAIR - 1)) ;
		}
		else
		{
			//������
			lUserLostScore[i] -= m_lUserBankerTwoPair[i];
			lBankerWinScore += m_lUserBankerTwoPair[i];
		}

		//����˰��
		if (0 < m_lUserWinScore[i])
		{
			float fRevenuePer=float(cbRevenue/1000.0);
			m_lUserRevenue[i]  = LONGLONG(m_lUserWinScore[i]*fRevenuePer);
			m_lUserWinScore[i] -= m_lUserRevenue[i];
		}

		//�ܵķ���
		m_lUserWinScore[i] += lUserLostScore[i];
	}

	//ׯ�ҳɼ�
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		m_lUserWinScore[m_wCurrentBanker] = lBankerWinScore;

		//����˰��
		if (0 < m_lUserWinScore[m_wCurrentBanker])
		{
			float fRevenuePer=float(cbRevenue/1000.0);
			m_lUserRevenue[m_wCurrentBanker]  = LONGLONG(m_lUserWinScore[m_wCurrentBanker]*fRevenuePer);
			m_lUserWinScore[m_wCurrentBanker] -= m_lUserRevenue[m_wCurrentBanker];
			lBankerWinScore = m_lUserWinScore[m_wCurrentBanker];
		}		
	}

	//�ۼƻ���
	m_lBankerWinScore += lBankerWinScore;

	//��ǰ����
	m_lBankerCurGameScore=lBankerWinScore;

	return lBankerWinScore;
}

//�ƶ�Ӯ��
void CTableFrameSink::DeduceWinner(BYTE &cbWinner, BYTE &cbKingWinner, bool &bPlayerTwoPair, bool &bBankerTwoPair)
{
	cbWinner = 0;
	cbKingWinner = 0;

	//�����Ƶ�
	BYTE cbPlayerCount=m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_PLAYER],m_cbCardCount[INDEX_PLAYER]);
	BYTE cbBankerCount=m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_BANKER],m_cbCardCount[INDEX_BANKER]);

	//ʤ�����
	if (cbPlayerCount==cbBankerCount)
	{
		cbWinner=ID_PING_JIA;

		//ͬ��ƽ�ж�
		bool bAllPointSame = false;
		if ( m_cbCardCount[INDEX_PLAYER] == m_cbCardCount[INDEX_BANKER] )
		{
			for (WORD wCardIndex = 0; wCardIndex < m_cbCardCount[INDEX_PLAYER]; ++wCardIndex )
			{
				BYTE cbBankerValue = m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_BANKER][wCardIndex]);
				BYTE cbPlayerValue = m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_PLAYER][wCardIndex]);
				if ( cbBankerValue != cbPlayerValue ) break;
			}
			if ( wCardIndex == m_cbCardCount[INDEX_PLAYER] ) bAllPointSame = true;
		}
		if ( bAllPointSame ) cbKingWinner = ID_TONG_DIAN_PING;
	}
	else if (cbPlayerCount<cbBankerCount) 
	{
		cbWinner=ID_ZHUANG_JIA;

		//�����ж�
		if ( cbBankerCount == 8 || cbBankerCount == 9 ) cbKingWinner = ID_ZHUANG_TIAN_WANG;
	}
	else 
	{
		cbWinner=ID_XIAN_JIA;

		//�����ж�
		if ( cbPlayerCount == 8 || cbPlayerCount == 9 ) cbKingWinner = ID_XIAN_TIAN_WANG;
	}

	//�����ж�
	bPlayerTwoPair=false;
	bBankerTwoPair=false;
	if (m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_PLAYER][0])==m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_PLAYER][1]))
		bPlayerTwoPair=true;
	if (m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_BANKER][0])==m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_BANKER][1]))
		bBankerTwoPair=true;
}


//��̽���ж�
bool CTableFrameSink::ProbeJudge()
{
	//��������
	LONGLONG cbRevenue=m_pGameServiceOption->wRevenue;

	//�����Ƶ�
	BYTE cbPlayerCount=m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_PLAYER],m_cbCardCount[INDEX_PLAYER]);
	BYTE cbBankerCount=m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_BANKER],m_cbCardCount[INDEX_BANKER]);

	//�ƶ����
	BYTE cbWinner,cbKingWinner;
	bool bPlayerTwoPair,bBankerTwoPair;
	DeduceWinner(cbWinner, cbKingWinner, bPlayerTwoPair,bBankerTwoPair);

	//ׯ������
	LONGLONG lBankerWinScore = 0;

	//��ҳɼ�
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	LONGLONG lUserLostScore[GAME_PLAYER];
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

	//�����ע
	LONGLONG *pUserScore[] = {NULL, m_lUserPlayerScore, m_lUserTieScore, m_lUserBankerScore, m_lUserPlayerKingScore, 
		m_lUserBankerKingScore, m_lUserTieSamePointScore};

	//������
	BYTE cbMultiple[] = {0, 1, 8, 1, 2, 2, 32};

	//ϵͳ��Ӯ
	LONGLONG lSystemScore = 0l;

	//ׯ���ǲ��ǻ�����
	bool bIsBankerAndroidUser = false;
	if ( m_wCurrentBanker != INVALID_CHAIR )
	{
		IServerUserItem * pIBankerUserItem = m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		if (pIBankerUserItem != NULL) 
		{
			bIsBankerAndroidUser = pIBankerUserItem->IsAndroidUser();
		}
	}

	//�������
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//ׯ���ж�
		if (m_wCurrentBanker==i) continue;

		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem==NULL) continue;

		//
		bool bIsAndroidUser = pIServerUserItem->IsAndroidUser();

		for ( WORD wAreaIndex = ID_XIAN_JIA; wAreaIndex <= ID_TONG_DIAN_PING; ++wAreaIndex )
		{
			if ( wAreaIndex == cbWinner || wAreaIndex == cbKingWinner ) 
			{
				if(bIsAndroidUser)
					lSystemScore += (pUserScore[wAreaIndex][i] * cbMultiple[wAreaIndex]);

				if(m_wCurrentBanker==INVALID_CHAIR || bIsBankerAndroidUser)
					lSystemScore -= (pUserScore[wAreaIndex][i] * cbMultiple[wAreaIndex]);
			}
			else if ( cbWinner == ID_PING_JIA )
			{
			}
			else
			{
				if(bIsAndroidUser)
					lSystemScore -= pUserScore[wAreaIndex][i];

				if(m_wCurrentBanker==INVALID_CHAIR || bIsBankerAndroidUser)
					lSystemScore += pUserScore[wAreaIndex][i];
			}
		}

		//�ж���
		if (bPlayerTwoPair)
		{
			if(bIsAndroidUser)
				lSystemScore += (m_lUserPlayerTwoPair[i] * (RATE_TWO_PAIR - 1));
			
			if(m_wCurrentBanker==INVALID_CHAIR || bIsBankerAndroidUser)
				lSystemScore -= (m_lUserPlayerTwoPair[i] * (RATE_TWO_PAIR - 1));
		}
		else
		{
			//������
			if(bIsAndroidUser)
				lSystemScore -= m_lUserPlayerTwoPair[i];

			if(m_wCurrentBanker==INVALID_CHAIR || bIsBankerAndroidUser)
				lSystemScore += m_lUserPlayerTwoPair[i];
		}

		if (bBankerTwoPair)
		{
			//���Ӯ��
			if(bIsAndroidUser)
				lSystemScore += (m_lUserBankerTwoPair[i] * (RATE_TWO_PAIR - 1));
			
			if(m_wCurrentBanker==INVALID_CHAIR || bIsBankerAndroidUser)
				lSystemScore -= (m_lUserBankerTwoPair[i] * (RATE_TWO_PAIR - 1));
		}
		else
		{
			//������
			if(bIsAndroidUser)
				lSystemScore -= m_lUserBankerTwoPair[i];

			if(m_wCurrentBanker==INVALID_CHAIR || bIsBankerAndroidUser)
				lSystemScore += m_lUserBankerTwoPair[i];
		}
	}

	//ϵͳ��ֵ����
	if ((lSystemScore + m_StorageStart) < 0l)
	{
		return false;
	}
	else
	{
		m_StorageStart += lSystemScore;
		m_StorageStart = m_StorageStart - (m_StorageStart * m_StorageDeduct / 1000);
		return true;
	}
}


//���ͼ�¼
void CTableFrameSink::SendGameRecord(IServerUserItem *pIServerUserItem)
{
	WORD wBufferSize=0;
	BYTE cbBuffer[SOCKET_BUFFER];
	int nIndex = m_nRecordFirst;
	while ( nIndex != m_nRecordLast )
	{
		if ((wBufferSize+sizeof(tagServerGameRecord))>sizeof(cbBuffer))
		{
			m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);
			wBufferSize=0;
		}
		CopyMemory(cbBuffer+wBufferSize,&m_GameRecordArrary[nIndex],sizeof(tagServerGameRecord));
		wBufferSize+=sizeof(tagServerGameRecord);

		nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
	}
	if (wBufferSize>0) m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);
}

//������Ϣ
void CTableFrameSink::SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg)
{
	if (wChairID==INVALID_CHAIR)
	{
		//��Ϸ���
		for (WORD i=0; i<GAME_PLAYER; ++i)
		{
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
			if (pIServerUserItem==NULL) continue;
			m_pITableFrame->SendGameMessage(pIServerUserItem,pszTipMsg,SMT_INFO);
		}

		//�Թ����
		WORD wIndex=0;
		do {
			IServerUserItem *pILookonServerUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
			if (pILookonServerUserItem==NULL) break;

			m_pITableFrame->SendGameMessage(pILookonServerUserItem,pszTipMsg,SMT_INFO);

		}while(true);
	}
	else
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		if (pIServerUserItem!=NULL) m_pITableFrame->SendGameMessage(pIServerUserItem,pszTipMsg,SMT_INFO|SMT_EJECT);
	}
}

//��ȡ����
void CTableFrameSink::ReadConfigInformation(bool bReadFresh)
{
	//���Ʊ���
	LONGLONG lMaxScore = 0x7fffffffffffffff;
	TCHAR OutBuf[255];
	TCHAR szMaxScore[255] = {0};

	//ÿ��ˢ��
	if (bReadFresh)
	{
		//ÿ��ˢ��
		BYTE cbRefreshCfg = GetPrivateProfileInt(m_szGameRoomName, TEXT("Refresh"), 0, m_szConfigFileName);
		m_bRefreshCfg = cbRefreshCfg?true:false;
	}

	_snprintf(szMaxScore,sizeof(szMaxScore),TEXT("%I64d"),lMaxScore);
	m_lApplyBankerCondition=GetPrivateProfileInt(m_szGameRoomName, TEXT("Score"), 100, m_szConfigFileName);

	memset(OutBuf,0,sizeof(OutBuf));
	GetPrivateProfileString(m_szGameRoomName, TEXT("AreaLimitScore"), szMaxScore, OutBuf, 255, m_szConfigFileName);
	_snscanf(OutBuf, strlen(OutBuf), TEXT("%I64d"), &m_lAreaLimitScore);

	memset(OutBuf,0,sizeof(OutBuf));
	GetPrivateProfileString(m_szGameRoomName, TEXT("UserLimitScore"), szMaxScore, OutBuf, 255, m_szConfigFileName);
	_snscanf(OutBuf, strlen(OutBuf), TEXT("%I64d"), &m_lUserLimitScore);

	BYTE cbEnableSysBanker=GetPrivateProfileInt(m_szGameRoomName, TEXT("EnableSysBanker"), 1, m_szConfigFileName);
	m_bEnableSysBanker=cbEnableSysBanker?true:false;

	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("StorageStart"),"0",OutBuf,255,m_szConfigFileName);
	_snscanf(OutBuf,strlen(OutBuf),"%I64d",&m_StorageStart);

	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("StorageDeduct"),"1",OutBuf,255,m_szConfigFileName);
	_snscanf(OutBuf,strlen(OutBuf),"%I64d",&m_StorageDeduct);

	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("Time"),TEXT("10"),OutBuf,255,m_szConfigFileName);
	_snscanf(OutBuf,strlen(OutBuf),"%I64d",&m_lBankerMAX);

	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("TimeAdd"),TEXT("10"),OutBuf,255,m_szConfigFileName);
	_snscanf(OutBuf,strlen(OutBuf),"%I64d",&m_lBankerAdd);

	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("BankerScore"),TEXT("100000000"),OutBuf,255,m_szConfigFileName);
	_snscanf(OutBuf,strlen(OutBuf),"%I64d",&m_lBankerScoreMAX);

	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("BankerAdd"),TEXT("10"),OutBuf,255,m_szConfigFileName);
	_snscanf(OutBuf,strlen(OutBuf),"%I64d",&m_lBankerScoreAdd);

	m_nMaxChipRobot = GetPrivateProfileInt(m_szGameRoomName, TEXT("RobotBetCount"), 10, m_szConfigFileName);
	if (m_nMaxChipRobot < 0)	m_nMaxChipRobot = 10;

	if ( m_lBankerScoreMAX <= m_lApplyBankerCondition)
		m_lBankerScoreMAX = 0l;

	m_lPlayerBankerMAX = m_lBankerMAX;

	//ʱ�����
	m_cbFreeTime = GetPrivateProfileInt(m_szGameRoomName, TEXT("FreeTime"), TIME_FREE, m_szConfigFileName);
	m_cbBetTime = GetPrivateProfileInt(m_szGameRoomName, TEXT("BetTime"), TIME_PLACE_JETTON, m_szConfigFileName);
	m_cbEndTime = GetPrivateProfileInt(m_szGameRoomName, TEXT("EndTime"), TIME_GAME_END, m_szConfigFileName);
}

//////////////////////////////////////////////////////////////////////////
//���в���
#ifdef __SPECIAL___
bool __cdecl CTableFrameSink::OnActionUserBank(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	return true;
}

#endif
//////////////////////////////////////////////////////////////////////////

bool CTableFrameSink::OnSubAmdinCommand(IServerUserItem*pIServerUserItem,const void*pDataBuffer)
{
	//��������й���ԱȨ�� �򷵻ش���
	if((pIServerUserItem->GetUserData()->dwUserRight&UR_GAME_CONTROL)==0)
	{
		return false;
	}

	const CMD_C_AdminReq* AdminReq=static_cast<const CMD_C_AdminReq*>(pDataBuffer);
	switch(AdminReq->cbReqType)
	{
	case RQ_RESET_CONTROL:
		{
			CMD_S_CommandResult cResult;
			cResult.cbAckType=ACK_RESET_CONTROL;
			cResult.cbResult=CR_ACCEPT;
			m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));
			break;
		}
	case RQ_SET_WIN_AREA:
		{
			if(AdminReq->cbExtendData[0]>0&&AdminReq->cbExtendData[0]<=8)
			{
				m_cbWinSideControl=AdminReq->cbExtendData[0];
				m_cbExcuteTimes=AdminReq->cbExtendData[1];
				CMD_S_CommandResult cResult;
				cResult.cbAckType=ACK_SET_WIN_AREA;
				cResult.cbResult=CR_ACCEPT;
				m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));
			}
			else
			{
				CMD_S_CommandResult cResult;
				cResult.cbAckType=ACK_SET_WIN_AREA;
				cResult.cbResult=CR_REFUSAL;
				m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));

			}
			break;
		}
	case RQ_PRINT_SYN:
		{
			CMD_S_CommandResult cResult;
			cResult.cbAckType=ACK_PRINT_SYN;
			cResult.cbResult=CR_ACCEPT;
			cResult.cbExtendData[0]=m_cbWinSideControl;
			cResult.cbExtendData[1]=m_cbExcuteTimes;
			m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));
			break;
		}

	}
	
	return true;
}

void CTableFrameSink::ControlResult()
{
	switch(m_cbWinSideControl)
	{
	case 1:
		{
			//�ж���
			BYTE cbIndex=rand()%CountArray(CardPlayerPairCard);
			CopyMemory(m_cbCardCount,CardPlayerPairCard[cbIndex].cbCardCount,sizeof(m_cbCardCount));
			CopyMemory(m_cbTableCardArray,CardPlayerPairCard[cbIndex].cbTableCardArray,sizeof(m_cbTableCardArray));
			break;
		}
	case 2:
		{
			//��
			BYTE cbIndex=rand()%CountArray(CardPlayerWinCard);
			CopyMemory(m_cbCardCount,CardPlayerWinCard[cbIndex].cbCardCount,sizeof(m_cbCardCount));
			CopyMemory(m_cbTableCardArray,CardPlayerWinCard[cbIndex].cbTableCardArray,sizeof(m_cbTableCardArray));
			break;
		}
	case 3:
		{
			//������
			BYTE cbIndex=rand()%CountArray(CardPlayerKingCard);
			CopyMemory(m_cbCardCount,CardPlayerKingCard[cbIndex].cbCardCount,sizeof(m_cbCardCount));
			CopyMemory(m_cbTableCardArray,CardPlayerKingCard[cbIndex].cbTableCardArray,sizeof(m_cbTableCardArray));
			break;
		}
	case 4:
		{
			//ƽ��ʤ��
			BYTE cbIndex=rand()%CountArray(CardEqualCard);
			CopyMemory(m_cbCardCount,CardEqualCard[cbIndex].cbCardCount,sizeof(m_cbCardCount));
			CopyMemory(m_cbTableCardArray,CardEqualCard[cbIndex].cbTableCardArray,sizeof(m_cbTableCardArray));
			break;
		}
	case 5:
		{
			//ͬ��ƽ
			BYTE cbIndex=rand()%CountArray(CardSamePointCard);
			CopyMemory(m_cbCardCount,CardSamePointCard[cbIndex].cbCardCount,sizeof(m_cbCardCount));
			CopyMemory(m_cbTableCardArray,CardSamePointCard[cbIndex].cbTableCardArray,sizeof(m_cbTableCardArray));
			break;
		}
	case 6:
		{
			//ׯ����
			BYTE cbIndex=rand()%CountArray(CardBankerPairCard);
			CopyMemory(m_cbCardCount,CardBankerPairCard[cbIndex].cbCardCount,sizeof(m_cbCardCount));
			CopyMemory(m_cbTableCardArray,CardBankerPairCard[cbIndex].cbTableCardArray,sizeof(m_cbTableCardArray));
			break;
		}
	case 7:
		{
			//ׯ��ʤ��
			BYTE cbIndex=rand()%CountArray(CardBankerWinCard);
			CopyMemory(m_cbCardCount,CardBankerWinCard[cbIndex].cbCardCount,sizeof(m_cbCardCount));
			CopyMemory(m_cbTableCardArray,CardBankerWinCard[cbIndex].cbTableCardArray,sizeof(m_cbTableCardArray));
			break;
		}
	case 8:
		{
			//ׯ����
			BYTE cbIndex=rand()%CountArray(CardBankerKingCard);
			CopyMemory(m_cbCardCount,CardBankerKingCard[cbIndex].cbCardCount,sizeof(m_cbCardCount));
			CopyMemory(m_cbTableCardArray,CardBankerKingCard[cbIndex].cbTableCardArray,sizeof(m_cbTableCardArray));
			break;
		}
	default:ASSERT(FALSE); return ;
	}
}