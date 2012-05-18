#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//��������
#define SEND_COUNT					8									//���ʹ���


//��עʱ��
#define IDI_BET_TIME_OVER			1									//��ע��ʱ��
#define IDI_BET_TIME_SEND			2									//���Ƽ�ʱ��
#define IDI_GAME_END				3									//������ʱ��



//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;				//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_TimeControl;	//��ʼģʽ

//////////////////////////////////////////////////////////////////////////


//���캯��
CTableFrameSink::CTableFrameSink()
{
	//״̬����
	m_dwJettonTime=0L;
	AllocConsole();
	freopen("CON","wt",stdout);

	//��ע��Ϣ

    m_Winning=false;
     PlayGame=0;

	m_dwUserID=0;
	//�������
	m_pITableFrame=NULL;
	m_pITableFrameControl=NULL;
	m_pGameServiceOption=NULL;
	//ׯ�ұ���

    ZeroMemory(&m_CurrentBanker, sizeof( m_CurrentBanker ) );
	RepositTableFrameSink();

	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
	//����Ϸ��ʷ��¼д���ļ���
	tagHistory Histories[HISTORY_COUNT];
	ZeroMemory(&Histories, sizeof(Histories));

	for (WORD i=0; i<min(m_HistoryArray.GetCount(), HISTORY_COUNT); i++)
	{
		Histories[i] = m_HistoryArray[i];
			//printf(" m_HistoryArray[i] %d\n",m_HistoryArray[i]);
	}

	//����ʷ��¼д���ļ�
	TCHAR szPath[MAX_PATH]=TEXT("");
	TCHAR szFileName[MAX_PATH];
	CFile HistoryFile;

	GetCurrentDirectory(sizeof(szPath), szPath);
	_snprintf(szFileName, sizeof(szFileName), TEXT("%s\\H_SLWH.%d"), szPath, m_pGameServiceOption->wServerID);

	if (HistoryFile.Open(szFileName, CFile::modeWrite|CFile::modeCreate,NULL))
	{
		HistoryFile.Write(&Histories, sizeof(Histories));
		HistoryFile.Close();
	}
	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//��ȡ����

	TCHAR szRoomID[20];

	if (GetCurrentDirectory(sizeof(szPath), szPath))
	{
		_snprintf(szFileName,sizeof(szFileName),TEXT("%s\\H_SLWH.ini"),szPath);
		_snprintf(szRoomID, sizeof(szRoomID), TEXT("%d"), m_pGameServiceOption->wServerID);

		CString server_ip;

	//��Ӯ����

        server_ip.Format("%I64d",m_WinMoney);
		WritePrivateProfileString(szRoomID, TEXT("WinMoney"),server_ip,szFileName);
        server_ip.Format("%I64d",m_LoseMoney);

		WritePrivateProfileString(szRoomID, TEXT("LoseMoney"),server_ip,szFileName);
        server_ip.Format("%ld",m_Prizemoney);
		WritePrivateProfileString(szRoomID, TEXT("Prizemoney"),server_ip,szFileName);

	}

	for (int i=0;i<GAME_PLAYER;i++)
	{
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetServerUserItem(i);


		if (m_lPlayerStakes[i]>0&&pIServerUserItem)
		{	
			tagScoreInfo ScoreInfo;
			ZeroMemory(&ScoreInfo, sizeof(ScoreInfo));
			ScoreInfo.lScore=m_lPlayerStakes[i]*m_lCellGold;
			if (m_pITableFrame->WriteUserScore(pIServerUserItem, ScoreInfo.lScore, ScoreInfo.lRevenue, ScoreInfo.ScoreKind))
			{
				m_lPlayerStakes[i]=0;
			}
		}
	}

}

//�ӿڲ�ѯ
void * __cdecl CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
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
	m_pITableFrameControl=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrameControl);
	if(m_pITableFrameControl ==NULL) return false;
	m_TableFrameManager=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrameManager);
	if (m_TableFrameManager==NULL) return false;

	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//��ȡ����
	TCHAR szPath[MAX_PATH]=TEXT("");
	TCHAR szFileName[MAX_PATH];
	TCHAR szRoomID[20];

	if (GetCurrentDirectory(sizeof(szPath), szPath))
	{
		_snprintf(szFileName,sizeof(szFileName),TEXT("%s\\H_SLWH.ini"),szPath);
		_snprintf(szRoomID, sizeof(szRoomID), TEXT("%d"), m_pGameServiceOption->wServerID);

		CString server_ip;

		//�ʽ�
		m_Prizemoney = GetPrivateProfileInt(szRoomID, TEXT("Prizemoney"), 0, szFileName);
		//��ӮǮ��
		GetPrivateProfileString(szRoomID,"WinMoney","",server_ip.GetBufferSetLength(30),30,szFileName);
		server_ip.ReleaseBuffer();
		m_WinMoney=_atoi64(server_ip);

		//����Ǯ��
		GetPrivateProfileString(szRoomID,"LoseMoney","",server_ip.GetBufferSetLength(30),30,szFileName);
		server_ip.ReleaseBuffer();
		m_LoseMoney=_atoi64(server_ip);
		//��Ӯ����
		m_Winratio = GetPrivateProfileInt(szRoomID, TEXT("Winratio"), 0, szFileName);
		m_ratio[0] = GetPrivateProfileInt(szRoomID, TEXT("RedRabbit"), 0, szFileName);
		m_ratio[1] = GetPrivateProfileInt(szRoomID, TEXT("YellowRabbit"), 0, szFileName);
		m_ratio[2] = GetPrivateProfileInt(szRoomID, TEXT("GreenRabbit"), 0, szFileName);
    	m_ratio[3] = GetPrivateProfileInt(szRoomID, TEXT("RedDragon"), 0, szFileName);
		m_ratio[4] = GetPrivateProfileInt(szRoomID, TEXT("YellowDragon"), 0, szFileName);
		m_ratio[5] = GetPrivateProfileInt(szRoomID, TEXT("GreenDragon"), 0, szFileName);
		m_ratio[6] = GetPrivateProfileInt(szRoomID, TEXT("RedPig"), 0, szFileName);
		m_ratio[7] = GetPrivateProfileInt(szRoomID, TEXT("YellowPig"), 0, szFileName);
		m_ratio[8] = GetPrivateProfileInt(szRoomID, TEXT("GreenPig"), 0, szFileName);
		m_ratio[9] = GetPrivateProfileInt(szRoomID, TEXT("RedDog"), 0, szFileName);
		m_ratio[10] = GetPrivateProfileInt(szRoomID, TEXT("YellowDog"), 0, szFileName);
		m_ratio[11] = GetPrivateProfileInt(szRoomID, TEXT("GreenDog"), 0, szFileName);


		//������ׯ����ͷ���
		m_lApplyBankerCondition= GetPrivateProfileInt(szRoomID, TEXT("ApplyBankerCondition"), 10, szFileName);

		//������ׯ��������
		m_lBankerCountMax = GetPrivateProfileInt(szRoomID, TEXT("BankerCountMax"), 10, szFileName);

		//������ׯ���������	
		m_wApplyCountMax = GetPrivateProfileInt(szRoomID, TEXT("ApplyCountMax"), 6, szFileName);

		//��ʤID��ׯʤ��
		m_BSIDSL = GetPrivateProfileInt(szRoomID, TEXT("m_BSIDSL"), 0, szFileName);

		//��ʤID
		BSID1 = GetPrivateProfileInt(szRoomID, TEXT("m_BSID1"), MAX_SCORE, szFileName);
		//��ʤID
		BSID2 = GetPrivateProfileInt(szRoomID, TEXT("m_BSID2"), MAX_SCORE, szFileName);

		BSID3 = GetPrivateProfileInt(szRoomID, TEXT("m_BSID3"), MAX_SCORE, szFileName);

		BSID4 = GetPrivateProfileInt(szRoomID, TEXT("m_BSID4"), MAX_SCORE, szFileName);

		BSID5 = GetPrivateProfileInt(szRoomID, TEXT("m_BSID5"), MAX_SCORE, szFileName);

	}

	m_lCellGold=m_pGameServiceOption->lCellScore;
    m_ratioCount=0;
	for (int i=0;i<12;i++)
	{
		m_ratioCount+=m_ratio[i];
	}
	
    printf("m_ratioCount    %d\n",m_ratioCount);

	//����ʷ��¼�ļ��м�������
	CFile HistoryFile;
	_snprintf(szFileName, sizeof(szFileName), TEXT("%s\\H_SLWH.%d"), szPath, m_pGameServiceOption->wServerID);

	if (HistoryFile.Open(szFileName, CFile::modeRead,NULL))
	{
		tagHistory	HistoryInfo[HISTORY_COUNT];

		//��ȡ����
		ULONGLONG ulong = HistoryFile.GetLength();
		
		if (ulong == sizeof(HistoryInfo))
		{
			UINT uReadCount=HistoryFile.Read(&HistoryInfo, sizeof(HistoryInfo));

			if (uReadCount != sizeof(HistoryInfo))
			{
				ZeroMemory(&HistoryInfo, sizeof(HistoryInfo));
			}
			else
			{
				for (WORD i=0; i<HISTORY_COUNT; i++)
				{
					m_HistoryArray.Add(HistoryInfo[i]);
				}
			}
		}
		HistoryFile.Close();
	}
	
	return true;
}

//��λ����
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//��ע��Ϣ


	ZeroMemory(m_lAreaStakes, sizeof(m_lAreaStakes));
	ZeroMemory(m_lDetailStakes, sizeof(m_lDetailStakes));

	//ZeroMemory(m_lPlayerStakes,sizeof(m_lDetailStakes));

	//�˿���Ϣ



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

	return true;
}

void CTableFrameSink::OnGameDispatch()
{
	bool ISSupper=false;

	if (!ISSupper)
	{		
			if (m_CurrentBanker.wChairID == INVALID_CHAIR)
			{
				bool Win=false;
				m_Winning=false;
				INT64 WinMoney=m_WinMoney-m_LoseMoney-m_Prizemoney/100;
				if (WinMoney<500||(m_LoseMoney>0&&(WinMoney*100/m_WinMoney)<(m_Winratio)))
				{
					Win=true;
				}


				LONG GameCount=0;
				for (int i=0;i<15;i++)
				{
					GameCount+=m_lAreaStakes[i];
				}
				int winGame[12];
				ZeroMemory(winGame, sizeof(winGame));
				BYTE winT=0;

				if (Win&&GameCount>0&&!ISSupper)
				{
					for (int i=0;i<12;i++)
					{
						int ls=0;
						if (i%3==0)
						{
							ls=m_lAreaStakes[12]*3;
						}
						if (i%3==1)
						{
							ls=m_lAreaStakes[13]*3;
						}
						if (i%3==2)
						{
							ls=m_lAreaStakes[14]*3;
						}
						if((GameCount-m_lAreaStakes[i]*m_CardCount[i]-ls)>0) 
						{
							winGame[winT]=i;
							winT++;
						}
					}

					int dd=rand()%winT;
					m_Card=winGame[dd];
				}else
				{

					if (!ISSupper)
					{
						int sl  = rand()%10;
						if(m_ratioCount != 0)
						sl =1+rand()%m_ratioCount;

						if (sl>(m_ratioCount-m_ratio[11]))
						{
							m_Card=11;
						}else
							if(sl>(m_ratioCount-m_ratio[11]-m_ratio[10]))
							{
								m_Card=10;
							}else
								if(sl>(m_ratioCount-m_ratio[11]-m_ratio[10]-m_ratio[9]))
								{
									m_Card=9;
								}else
									if(sl>(m_ratioCount-m_ratio[11]-m_ratio[10]-m_ratio[9]-m_ratio[8]))
									{
										m_Card=8;
									}else
										if(sl>(m_ratioCount-m_ratio[11]-m_ratio[10]-m_ratio[9]-m_ratio[8]-m_ratio[7]))
										{
											m_Card=7;
										}else
											if(sl>(m_ratioCount-m_ratio[11]-m_ratio[10]-m_ratio[9]-m_ratio[8]-m_ratio[7]-m_ratio[6]))
											{
												m_Card=6;
											}else
												if(sl>(m_ratioCount-m_ratio[11]-m_ratio[10]-m_ratio[9]-m_ratio[8]-m_ratio[7]-m_ratio[6]-m_ratio[5]))
												{
													m_Card=5;
												}else
													if(sl>(m_ratioCount-m_ratio[11]-m_ratio[10]-m_ratio[9]-m_ratio[8]-m_ratio[7]-m_ratio[6]-m_ratio[5]-m_ratio[4]))
													{
														m_Card=4;
													}else
														if(sl>(m_ratioCount-m_ratio[11]-m_ratio[10]-m_ratio[9]-m_ratio[8]-m_ratio[7]-m_ratio[6]-m_ratio[5]-m_ratio[4]-m_ratio[3]))
														{
															m_Card=3;
														}else
															if(sl>(m_ratioCount-m_ratio[11]-m_ratio[10]-m_ratio[9]-m_ratio[8]-m_ratio[7]-m_ratio[6]-m_ratio[5]-m_ratio[4]-m_ratio[3]-m_ratio[2]))
															{
																m_Card=2;
															}else
																if(sl>(m_ratioCount-m_ratio[11]-m_ratio[10]-m_ratio[9]-m_ratio[8]-m_ratio[7]-m_ratio[6]-m_ratio[5]-m_ratio[4]-m_ratio[3]-m_ratio[2]-m_ratio[1]))
																{
																	m_Card=1;
																}else
																	if(sl>(m_ratioCount-m_ratio[11]-m_ratio[10]-m_ratio[9]-m_ratio[8]-m_ratio[7]-m_ratio[6]-m_ratio[5]-m_ratio[4]-m_ratio[3]-m_ratio[2]-m_ratio[1]-m_ratio[0]))
																	{
																		m_Card=0;
																	}
					}
					byte ls2=0;
					int ls1[9];

					for (int i=0;i<24;i++)
					{
						if(m_CardColoct[i]==m_Card) 
						{
							ls1[ls2]=i;
							ls2++;
						}
					}
					m_Cardsite=ls1[rand()%ls2];
				}


				if (m_lAreaStakes[m_Card]>0&&m_Prizemoney/100>500&&rand()%5==2&&(m_lAreaStakes[m_Card]*m_CardCount[m_Card]+m_lAreaStakes[m_Card%3+12]*3)<=m_Prizemoney/100)
				{
					m_Winning=true;
					m_Prizemoney=m_Prizemoney-m_lAreaStakes[m_Card]*m_CardCount[m_Card]*100+m_lAreaStakes[m_Card%3+12]*3*100;

				}

			}else
			{
				bool Win=false;
				m_Winning=false;

				if (rand()%7==1)
				{
					Win=true;
				}
				LONG GameCount=0;
				for (int i=0;i<15;i++)
				{
					GameCount+=m_lAreaStakes[i];
				}
				int winGame[12];
				ZeroMemory(winGame, sizeof(winGame));
				BYTE winT=0;
				if (Win&&GameCount>0)
				{
					for (int i=0;i<12;i++)
					{
						int ls=0;
						if (i%3==0)
						{
							ls=m_lAreaStakes[12]*3;
						}
						if (i%3==1)
						{
							ls=m_lAreaStakes[13]*3;
						}
						if (i%3==2)
						{
							ls=m_lAreaStakes[14]*3;
						}
						if((GameCount-m_lAreaStakes[i]*m_CardCount[i]-ls)>0) 
						{
							winGame[winT]=i;
							winT++;
						}
					}

					int dd=rand()%winT;
					m_Card=winGame[dd];
				}else
				{
					int sl  =1+rand()%m_ratioCount;
					if (sl>(m_ratioCount-m_ratio[11]))
					{
						m_Card=11;
					}else
						if(sl>(m_ratioCount-m_ratio[11]-m_ratio[10]))
						{
							m_Card=10;
						}else
							if(sl>(m_ratioCount-m_ratio[11]-m_ratio[10]-m_ratio[9]))
							{
								m_Card=9;
							}else
								if(sl>(m_ratioCount-m_ratio[11]-m_ratio[10]-m_ratio[9]-m_ratio[8]))
								{
									m_Card=8;
								}else
									if(sl>(m_ratioCount-m_ratio[11]-m_ratio[10]-m_ratio[9]-m_ratio[8]-m_ratio[7]))
									{
										m_Card=7;
									}else
										if(sl>(m_ratioCount-m_ratio[11]-m_ratio[10]-m_ratio[9]-m_ratio[8]-m_ratio[7]-m_ratio[6]))
										{
											m_Card=6;
										}else
											if(sl>(m_ratioCount-m_ratio[11]-m_ratio[10]-m_ratio[9]-m_ratio[8]-m_ratio[7]-m_ratio[6]-m_ratio[5]))
											{
												m_Card=5;
											}else
												if(sl>(m_ratioCount-m_ratio[11]-m_ratio[10]-m_ratio[9]-m_ratio[8]-m_ratio[7]-m_ratio[6]-m_ratio[5]-m_ratio[4]))
												{
													m_Card=4;
												}else
													if(sl>(m_ratioCount-m_ratio[11]-m_ratio[10]-m_ratio[9]-m_ratio[8]-m_ratio[7]-m_ratio[6]-m_ratio[5]-m_ratio[4]-m_ratio[3]))
													{
														m_Card=3;
													}else
														if(sl>(m_ratioCount-m_ratio[11]-m_ratio[10]-m_ratio[9]-m_ratio[8]-m_ratio[7]-m_ratio[6]-m_ratio[5]-m_ratio[4]-m_ratio[3]-m_ratio[2]))
														{
															m_Card=2;
														}else
															if(sl>(m_ratioCount-m_ratio[11]-m_ratio[10]-m_ratio[9]-m_ratio[8]-m_ratio[7]-m_ratio[6]-m_ratio[5]-m_ratio[4]-m_ratio[3]-m_ratio[2]-m_ratio[1]))
															{
																m_Card=1;
															}else
																if(sl>(m_ratioCount-m_ratio[11]-m_ratio[10]-m_ratio[9]-m_ratio[8]-m_ratio[7]-m_ratio[6]-m_ratio[5]-m_ratio[4]-m_ratio[3]-m_ratio[2]-m_ratio[1]-m_ratio[0]))
																{
																	m_Card=0;
																}
				}
				byte ls2=0;
				int ls1[9];

				for (int i=0;i<24;i++)
				{
					if(m_CardColoct[i]==m_Card) 
					{
						ls1[ls2]=i;
						ls2++;
					}
				}
				m_Cardsite=ls1[rand()%ls2];
			}
	}



	//��������
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));

    GameStart.cbCard=m_Card;
    GameStart.cbCardsite=m_Cardsite;
	GameStart.cbWinning = m_Winning;
	if (m_Winning)
	{
		GameStart.cbPrize = m_lAreaStakes[m_Card]*m_CardCount[m_Card]+m_lAreaStakes[m_Card%3+12]*3;
	}else
	{
        GameStart.cbPrize=0;
	}

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));


};
//д�����
void CTableFrameSink::WriteWinAcount()
{
}

//��Ϸ����
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
		{
			//������Ϣ
			CMD_S_GameEnd GameEnd;
			INT nOdds[INDEX_MAX]; //��Ӯ����
//			tagScoreInfo ScoreInfo;
			ZeroMemory(nOdds, sizeof(nOdds));
			for (int i=0;i<GAME_PLAYER;i++)
			{
				if ( m_CurrentBanker.dwUserID != 0 && m_CurrentBanker.wChairID == i ) continue;
				IServerUserItem * pIServerUserItem = m_pITableFrame->GetServerUserItem(i);
				ZeroMemory(&GameEnd,sizeof(GameEnd));
				if (pIServerUserItem)
				{
					for (int j=0;j<INDEX_MAX;j++)
					{
						if (m_lDetailStakes[i][j]>0&&m_Card==j&&j<12)
						{

							if (m_Winning)
							{
								m_lPlayerWin[i][j]=m_lDetailStakes[i][j]*m_CardCount[j]*2;
								m_lPlayerStakes[i]=(m_lPlayerStakes[i]+m_lPlayerWin[i][j]*2);
							}else
							{
								m_lPlayerWin[i][j]=m_lDetailStakes[i][j]*m_CardCount[j];
                                m_lPlayerStakes[i]+=m_lPlayerWin[i][j];
							}
							
  					    }else
						{
								
							if (j==12&&m_Card%3==0)
							{
									if (m_Winning)
									{
										m_lPlayerWin[i][j]=m_lDetailStakes[i][j]*m_CardCount[j]*2;
										m_lPlayerStakes[i]=(m_lPlayerStakes[i]+m_lPlayerWin[i][j]*2);
									}else
									{
										m_lPlayerWin[i][j]=m_lDetailStakes[i][j]*m_CardCount[j];
										m_lPlayerStakes[i]+=m_lPlayerWin[i][j];
									}
							}else
							if (j==13&&m_Card%3==1)
							{
								if (m_Winning)
								{
									m_lPlayerWin[i][j]=m_lDetailStakes[i][j]*m_CardCount[j]*2;
									m_lPlayerStakes[i]=(m_lPlayerStakes[i]+m_lPlayerWin[i][j]*2);
								}else
								{
									m_lPlayerWin[i][j]=m_lDetailStakes[i][j]*m_CardCount[j];
									m_lPlayerStakes[i]+=m_lPlayerWin[i][j];
								}
							}else
							if (j==14&&m_Card%3==2)
							{
								if (m_Winning)
								{
									m_lPlayerWin[i][j]=m_lDetailStakes[i][j]*m_CardCount[j]*2;
									m_lPlayerStakes[i]=(m_lPlayerStakes[i]+m_lPlayerWin[i][j]*2);
								}else
								{
									m_lPlayerWin[i][j]=m_lDetailStakes[i][j]*m_CardCount[j];
									m_lPlayerStakes[i]+=m_lPlayerWin[i][j];
								}
							}else
								m_lDetailStakes[i][j]=0;
							
						}
						if (m_CurrentBanker.wChairID==INVALID_CHAIR)
						{
							m_LoseMoney+=m_lPlayerWin[i][j];
						}
						
					}

					CopyMemory(GameEnd.lMyGameScore,m_lPlayerWin[i],sizeof(GameEnd.lMyGameScore));
					GameEnd.lPrizemoney = m_Prizemoney/100;
					//д�����
					m_pITableFrame->SendUserData(pIServerUserItem, SUB_S_GAME_END, &GameEnd, sizeof(GameEnd) );
				}
                
			}
			if (m_CurrentBanker.wChairID!=INVALID_CHAIR)
			{
				IServerUserItem * pIServerUserItem = m_pITableFrame->GetServerUserItem(m_CurrentBanker.wChairID);
				if (pIServerUserItem)
				{
					for (int j=0;j<INDEX_MAX;j++)
					{
						if (j<12)
						{
							if (m_lAreaStakes[j]>0&&m_Card==j)
							{
								m_lPlayerWin[m_CurrentBanker.wChairID][j]-=m_lAreaStakes[j]*(m_CardCount[j]-1);
								m_lPlayerStakes[m_CurrentBanker.wChairID]-=m_lAreaStakes[j]*(m_CardCount[j]-1);

							}else
							{
								m_lPlayerWin[m_CurrentBanker.wChairID][j]+=m_lAreaStakes[j];
								m_lPlayerStakes[m_CurrentBanker.wChairID]+=m_lAreaStakes[j];
							}
						}else
						{
							if (j==12&&m_Card%3==0)
							{
								m_lPlayerWin[m_CurrentBanker.wChairID][j]-=m_lAreaStakes[j]*(m_CardCount[j]-1);
								m_lPlayerStakes[m_CurrentBanker.wChairID]-=m_lAreaStakes[j]*(m_CardCount[j]-1);

							}else
							if (j==13&&m_Card%3==1)
							{
								m_lPlayerWin[m_CurrentBanker.wChairID][j]-=m_lAreaStakes[j]*(m_CardCount[j]-1);
								m_lPlayerStakes[m_CurrentBanker.wChairID]-=m_lAreaStakes[j]*(m_CardCount[j]-1);

							}else
							if (j==14&&m_Card%3==2)
							{
								m_lPlayerWin[m_CurrentBanker.wChairID][j]-=m_lAreaStakes[j]*(m_CardCount[j]-1);
								m_lPlayerStakes[m_CurrentBanker.wChairID]-=m_lAreaStakes[j]*(m_CardCount[j]-1);

							}else
							{
								m_lPlayerWin[m_CurrentBanker.wChairID][j]+=m_lAreaStakes[j];
								m_lPlayerStakes[m_CurrentBanker.wChairID]+=m_lAreaStakes[j];
							}
						}

					}

					CopyMemory(GameEnd.lMyGameScore,m_lPlayerWin[m_CurrentBanker.wChairID],sizeof(GameEnd.lMyGameScore));
					GameEnd.lPrizemoney = m_Prizemoney/100;
					//д�����
					m_pITableFrame->SendUserData(pIServerUserItem, SUB_S_GAME_END, &GameEnd, sizeof(GameEnd) );
				}
	
			}

			
			ZeroMemory(m_lDetailStakes,sizeof(m_lDetailStakes));
			ZeroMemory(m_lPlayerWin,sizeof(m_lPlayerWin));
	     	ZeroMemory(m_lAreaStakes,sizeof(m_lAreaStakes));

			////////////////////////////���濪�Ƽ�¼///////////////////////////
			tagHistory HistoryInfo;

			HistoryInfo.cbPlayerValue=m_Card;

			if (m_HistoryArray.GetCount() >= HISTORY_COUNT)
			{
				m_HistoryArray.RemoveAt(0); //�������30��
			}

			m_HistoryArray.Add(HistoryInfo);

			return true;
		}
	case GER_DISMISS:
		{
			for (int i=0;i<GAME_PLAYER;i++)
			{
				IServerUserItem * pIServerUserItem = m_pITableFrame->GetServerUserItem(i);


				if (m_lPlayerStakes[i]>0&&pIServerUserItem)
				{	
					tagScoreInfo ScoreInfo;
					ZeroMemory(&ScoreInfo, sizeof(ScoreInfo));
					ScoreInfo.lScore=m_lPlayerStakes[i]*m_lCellGold;
					if (m_pITableFrame->WriteUserScore(pIServerUserItem, ScoreInfo.lScore, ScoreInfo.lRevenue, ScoreInfo.ScoreKind))
					{
						m_lPlayerStakes[i]=0;
					}
				}
			}
			return true;
		}
	case GER_USER_LEFT:		//�û��뿪
		{
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
			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			StatusFree.lCellGold=m_lCellGold;
			StatusFree.lMyGame=m_lPlayerStakes[wChiarID];
			StatusFree.cdPrizemoney=m_Prizemoney/100;
			StatusFree.lApplyBankerCondition = m_lApplyBankerCondition;
			StatusFree.wApplyCountMax = m_wApplyCountMax;
			if ( m_CurrentBanker.wChairID != INVALID_CHAIR ) 
			{
				StatusFree.wCurrentBankerChairID = m_CurrentBanker.wChairID;
				StatusFree.wBankerCount = m_CurrentBanker.wBankerCount;
				StatusFree.lCurrentBankerScore = m_CurrentBanker.lUserScore;
			}
			else
			{
				StatusFree.wCurrentBankerChairID = INVALID_CHAIR;
				StatusFree.wBankerCount = 0;
				StatusFree.lCurrentBankerScore = 0;
			}
			CopyMemory(StatusFree.lMyStakes, m_lDetailStakes[wChiarID], sizeof(StatusFree.lMyStakes));
			CopyMemory(StatusFree.lAllStakes, m_lAreaStakes, sizeof(StatusFree.lAllStakes));
			CopyMemory(StatusFree.cbCardCount, m_CardCount, sizeof(m_CardCount));
			CopyMemory(StatusFree.cbCardColoct, m_CardColoct, sizeof(m_CardColoct));
			for (WORD i=0; i<min(m_HistoryArray.GetCount(), HISTORY_COUNT); i++)
			{
				StatusFree.wHistoryCount++;
				StatusFree.HistoryInfo[i] = m_HistoryArray[i];
			}
			//����������
			SendApplyUser(pIServerUserItem);
			//���ͳ���
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
			return bSuccess;
		}
	case GS_PLAYING:		//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));
			for (WORD i=0; i<min(m_HistoryArray.GetCount(),HISTORY_COUNT); i++)
			{
				StatusPlay.wHistoryCount++;
				StatusPlay.HistoryInfo[i] = m_HistoryArray[i];
			}
			StatusPlay.lApplyBankerCondition = m_lApplyBankerCondition;
			StatusPlay.wApplyCountMax = m_wApplyCountMax;
			StatusPlay.cdPrizemoney=m_Prizemoney/100;
			StatusPlay.lCellGold=m_lCellGold;
			StatusPlay.lMyGame=m_lPlayerStakes[wChiarID];
			CopyMemory(StatusPlay.lAllStakes, m_lAreaStakes, sizeof(StatusPlay.lAllStakes));
			CopyMemory(StatusPlay.lMyStakes, m_lDetailStakes[wChiarID], sizeof(StatusPlay.lMyStakes));
			CopyMemory(StatusPlay.cbCardCount, m_CardCount, sizeof(m_CardCount));
		    CopyMemory(StatusPlay.cbCardColoct, m_CardColoct, sizeof(m_CardColoct));

			if ( m_CurrentBanker.wChairID != INVALID_CHAIR ) 
			{
				StatusPlay.wCurrentBankerChairID = m_CurrentBanker.wChairID;
				StatusPlay.wBankerCount = m_CurrentBanker.wBankerCount;
				StatusPlay.lCurrentBankerScore = m_CurrentBanker.lUserScore;
			}
			else
			{
				StatusPlay.wCurrentBankerChairID = INVALID_CHAIR;
				StatusPlay.wBankerCount = 0;
				StatusPlay.lCurrentBankerScore = 0;
			}

			//����������
			SendApplyUser(pIServerUserItem);

			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));

			//���ͳ���
			return bSuccess;
		}
	}

	return false;
}
void CTableFrameSink::MakeRobotMessage()
{

}
//��ʱ���¼�
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{

	static DWORD ntimes = 0;

	if(ntimes++%10==0)
	{
			SendTimerTyper(nActStatus,this->m_nTimerTime);
			
			if (nActStatus==IDI_BET_TIME_OVER)
			{
				if (this->m_nTimerTime==TIME_PLACE_JETTON)
				{
					ISbanker=false;
					//�����˿�
					DispatchTableCard();
				}

     			if (this->m_nTimerTime==0)
				{

						OnGameDispatch();
						nActStatus=IDI_BET_TIME_SEND;
						m_nTimerTime=TIME_GAME_SENDPAI;
						return false;


				}

			}

			if (nActStatus==IDI_BET_TIME_SEND)
			{

				if (this->m_nTimerTime==TIME_GAME_SENDPAI-2)
				{

						//WriteWinAcount();
						//������Ϸ
						OnEventGameEnd(INVALID_CHAIR, NULL, GER_NORMAL);
					m_pITableFrame->ConcludeGame();


				}
				if (this->m_nTimerTime==1)
				{
					m_pITableFrame->ConcludeGame();
					m_pITableFrameControl->StartGame();
					ZeroMemory(m_lDetailStakes, sizeof(m_lDetailStakes));
					ZeroMemory(m_lAreaStakes, sizeof(m_lAreaStakes));
				}
				if (this->m_nTimerTime==0)
				{

						ChangeBanker();
						nActStatus=IDI_BET_TIME_OVER;
						m_nTimerTime=TIME_PLACE_JETTON;
						return false;
				}

			}



    	this->m_nTimerTime--;

	}
	return false;
}

//��Ϸ��Ϣ����
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{

	switch (wSubCmdID)
	{
	case SUB_C_PlAY_TRUE:
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_TrueArea));
			if (wDataSize!=sizeof(CMD_C_TrueArea)) return false;
			//��Ϣ����
			CMD_C_TrueArea * pTrueArea=(CMD_C_TrueArea *)pDataBuffer;


            return true;
		}
	case SUB_C_Settlement:
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_Settlement));
			if (wDataSize!=sizeof(CMD_C_Settlement)) return false;
			//��Ϣ����
			CMD_C_Settlement * pSettlement=(CMD_C_Settlement *)pDataBuffer;
            tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (m_lPlayerStakes[pUserData->wChairID]>0&&pUserData->wChairID!=m_CurrentBanker.wChairID)
			{	
				tagScoreInfo ScoreInfo;
				ZeroMemory(&ScoreInfo, sizeof(ScoreInfo));
				ScoreInfo.lScore=m_lPlayerStakes[pUserData->wChairID]*m_lCellGold;
				if (m_pITableFrame->WriteUserScore(pIServerUserItem, ScoreInfo.lScore, ScoreInfo.lRevenue, ScoreInfo.ScoreKind))
				{
					m_lPlayerStakes[pUserData->wChairID]=0;
				}
			}

			return true;
		}
	case SUB_C_PLACE_JETTON:		//�û���ע
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_PlaceJetton));
			if (wDataSize!=sizeof(CMD_C_PlaceJetton)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
		 	if (nActStatus!=IDI_BET_TIME_OVER) return true;

			//��Ϣ����
			CMD_C_PlaceJetton * pPlaceJetton=(CMD_C_PlaceJetton *)pDataBuffer;
			return OnUserPlaceJetton(pUserData->wChairID, pPlaceJetton->cbBetArea, pPlaceJetton->lBetAmount);
		}
	case SUB_C_CHI:
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_CHI));
			if (wDataSize!=sizeof(CMD_C_CHI)) return false;


			return true;	
		}
	case SUB_C_PlAY_OVER:
		{
			ShutDown(GetVerInfo());
			return true;	
		}
	case SUB_C_APPLY_BANKER:		//������ׯ
		{


			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_ApplyBanker));
			if (wDataSize!=sizeof(CMD_C_ApplyBanker)) return false;

			tagServerUserData * pUserData=pIServerUserItem->GetUserData();

			//��Ϣ����
			CMD_C_ApplyBanker * pApplyBanker=(CMD_C_ApplyBanker *)pDataBuffer;
			OnUserApplyBanker( pUserData, pApplyBanker->bApplyBanker,pApplyBanker->bAI);	

			printf("������ׯ   \n");

			return true;
		}
	case SUB_C_FANG:		//������Ϸ��
		{
			ASSERT(wDataSize==sizeof(CMD_C_FANG));
			if (wDataSize!=sizeof(CMD_C_FANG)) return false;
			//��Ϣ����
			CMD_C_FANG * fank=(CMD_C_FANG *)pDataBuffer;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData)
			{
				if (pUserData->UserScoreInfo.lScore>fank->cbFang*m_lCellGold)
				{

					tagScoreInfo ScoreInfo;
					ZeroMemory(&ScoreInfo, sizeof(ScoreInfo));
					ScoreInfo.lScore=0-fank->cbFang*m_lCellGold;
					if (m_pITableFrame->WriteUserScore(pUserData->wChairID, ScoreInfo.lScore, ScoreInfo.lRevenue, ScoreInfo.ScoreKind))
					{
                       m_lPlayerStakes[pUserData->wChairID]+=fank->cbFang;

					   CMD_S_Game m_Game;
					   m_Game.GameBi=m_lPlayerStakes[pUserData->wChairID];
					   m_pITableFrame->SendUserData( pIServerUserItem, SUB_S_GAME, &m_Game, sizeof(m_Game) );

					}
				}
				
			}



			return true;
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

		PlayGame++;
		printf("Down     %d  \n",PlayGame);
		if(PlayGame==1)
		{
			m_pITableFrame->KillGameTimer(1);
			m_pITableFrame->SetGameTimer(1,100,-1,0);

			m_CurrentBanker.wChairID=INVALID_CHAIR;
			m_CurrentBanker.dwUserID=0;

			nActStatus=IDI_BET_TIME_OVER;
			this->m_nTimerTime=TIME_PLACE_JETTON;

			//��ʼ��Ϸ
			m_pITableFrameControl->StartGame();

			//����״̬
			m_pITableFrame->SetGameStatus(GS_PLAYING);

		}

		pIServerUserItem->SetUserStatus(US_PLAY,pIServerUserItem->GetTableID(),wChairID);


	return true;
}

//�û�����
bool __cdecl CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//�û�Ч��


        PlayGame--;
//		printf("Up     %d   %ld   %d   %I64d\n",PlayGame,m_lPlayerStakes[wChairID],pIServerUserItem->GetUserData()->dwUserID,pIServerUserItem->GetUserData()->UserScoreInfo.lBet);
		tagScoreInfo ScoreInfo;
		ZeroMemory(&ScoreInfo, sizeof(ScoreInfo));
		ScoreInfo.lScore=m_lPlayerStakes[wChairID]*m_lCellGold;
		m_pITableFrame->WriteUserScore(pIServerUserItem, ScoreInfo.lScore, ScoreInfo.lRevenue, ScoreInfo.ScoreKind);
		m_lPlayerStakes[wChairID]=0;
		
		OnUserApplyBanker(pIServerUserItem->GetUserData(), false ,false);

		if (m_CurrentBanker.dwUserID == pIServerUserItem->GetUserID())
		{
			//������Ϸ
			ChangeBanker();

			//���������ׯ����������Ϸ
			if (m_CurrentBanker.dwUserID != 0)
			{

			}
			else
			{
				m_CurrentBanker.wChairID=INVALID_CHAIR;
				m_CurrentBanker.dwUserID=0;

			}
		}

	//�ж��Ƿ�����ң����û�оͽ�����Ϸ
	if (PlayGame==0)
	{
		nActStatus=-1;
		m_pITableFrame->KillGameTimer(1);
		m_pITableFrame->ConcludeGame();
	}
	
	return true;
}

bool CTableFrameSink::HavePlayer()
{
	for(WORD i=0; i<m_wPlayerCount; i++)
	{			
		IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(i) ;

		if (pServerUserItem != NULL)
		{
			return true;
		}
	}		

	return false;
}

//��ע�¼�
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbBetArea, INT64 lBetAmount)
{
	//Ч�����
	ASSERT((cbBetArea < INDEX_MAX) && (lBetAmount > 0L));
	if ((cbBetArea >= INDEX_MAX) || (lBetAmount <= 0L)) return false;

	//Ч��״̬
	if (nActStatus!=IDI_BET_TIME_OVER) return true;

	//��������
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
	if (!pIServerUserItem)
	{
		return true;
	}

   if (m_lPlayerStakes[wChairID]<lBetAmount)
   {
	   return true;
   }

   //ׯ�Ҳ�����ע
   if (m_CurrentBanker.dwUserID != 0 && m_CurrentBanker.wChairID == wChairID) return true;

   if (m_CurrentBanker.wChairID != INVALID_CHAIR)
   {
	   LONG fAllYaZhuTotale=0;
	   LONG m_plAreaStakes[INDEX_MAX];
	   CopyMemory( m_plAreaStakes,m_lAreaStakes, sizeof(m_lAreaStakes));

	   for (int i=0;i<INDEX_MAX;i++)
	   {
		   if (i==cbBetArea)
		   {
			   m_plAreaStakes[i]+=lBetAmount;
		   }
		   fAllYaZhuTotale+=m_plAreaStakes[i];

	   }
	   for (int j=0;j<12;j++)
	   {
		   if (m_lPlayerStakes[m_CurrentBanker.wChairID]+fAllYaZhuTotale<m_plAreaStakes[j]*m_CardCount[j]+m_plAreaStakes[12+j%3]*3)
		   {
			   return true;
		   }

	   }

   }
   if (m_lDetailStakes[wChairID][cbBetArea] + lBetAmount>99)
   {
	   return true;
   }

    m_lPlayerStakes[wChairID]-=lBetAmount;
	m_lDetailStakes[wChairID][cbBetArea] += lBetAmount;
	m_lAreaStakes[cbBetArea]+=lBetAmount;
	if (m_CurrentBanker.wChairID == INVALID_CHAIR)
	{
		m_WinMoney+=lBetAmount;
		m_Prizemoney+=cbBetArea;
		
	}
    
//	m_TableFrameManager->SetBetScore(wChairID,lBetAmount*m_lCellGold);
	//��������
	CMD_S_PlaceJetton PlaceJetton;
	ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

	//�������
	PlaceJetton.wChairID=wChairID;
	PlaceJetton.cbBetArea=cbBetArea;
	PlaceJetton.lBetAmount= lBetAmount;
	PlaceJetton.lGame =  m_lPlayerStakes[wChairID];
    PlaceJetton.cdPrizemoney =m_Prizemoney/100;
	CopyMemory( PlaceJetton.cdAreaStakes,m_lAreaStakes, sizeof(m_lAreaStakes));

	//д�����
	for ( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		IServerUserItem *pSeverUserItem = m_pITableFrame->GetServerUserItem( i );

		if (pSeverUserItem == NULL)
		{
			continue;
		}

	    m_pITableFrame->SendTableData(i, SUB_S_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton) );
	}


	return true;
}

//�����˿�
bool CTableFrameSink::DispatchTableCard()
{
	ZeroMemory(m_lAreaStakes, sizeof(m_lAreaStakes));
	ZeroMemory(m_lDetailStakes, sizeof(m_lDetailStakes));
	//��������
	CMD_S_GamePLU GamePLU;
	ZeroMemory(&GamePLU,sizeof(GamePLU));
    CopyMemory(m_CardCount,plu[rand()%5],sizeof(m_CardCount));
    CopyMemory(m_CardColoct,pColoct[rand()%8],sizeof(m_CardColoct));
	CopyMemory(GamePLU.cbCardCount,m_CardCount,sizeof(GamePLU.cbCardCount));
	CopyMemory(GamePLU.cbCardColoct,m_CardColoct,sizeof(GamePLU.cbCardColoct));
	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PLU,&GamePLU,sizeof(GamePLU));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLU,&GamePLU,sizeof(GamePLU));

	return true;
}


//����ׯ��
bool CTableFrameSink::OnUserApplyBanker(tagServerUserData *pUserData, bool bApplyBanker ,bool bApplybAi)
{

	//�������
	if ( bApplyBanker )
	{
		//�Ƿ�ﵽ��������
		if (m_lPlayerStakes[pUserData->wChairID] < m_lApplyBankerCondition )
			return true;


		//�����ж�
		for ( INT_PTR nUserIdx = 0; nUserIdx < m_ApplyUserArrary.GetCount(); ++nUserIdx )
		{
			tagApplyBanker ApplyUserInfo = m_ApplyUserArrary[ nUserIdx ];

			if (ApplyUserInfo.dwUserID == pUserData->dwUserID) return true;
		}

		//������Ϣ
		tagApplyBanker ApplyUserInfo;
		ApplyUserInfo.dwUserID = pUserData->dwUserID;
		ApplyUserInfo.lUserScore = m_lPlayerStakes[pUserData->wChairID];
		ApplyUserInfo.wChairID = pUserData->wChairID;

		//���������������ܾ�֮
		if (m_ApplyUserArrary.GetCount() >= m_wApplyCountMax)
			return true;

		m_ApplyUserArrary.Add(ApplyUserInfo);

		//�������
		CMD_S_ApplyBanker ApplyBanker;
		CopyMemory( ApplyBanker.szAccount, pUserData->szAccounts, sizeof( ApplyBanker.szAccount ) );
		CopyMemory( ApplyBanker.szName, pUserData->szAccounts, sizeof( ApplyBanker.szName ) );
		ApplyBanker.dwUserID = pUserData->dwUserID;
		ApplyBanker.lGold = m_lPlayerStakes[pUserData->wChairID];
		ApplyBanker.bApplyBanker = true;
		ApplyBanker.wChairID = pUserData->wChairID;

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof( ApplyBanker ) );
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof( ApplyBanker ) );
	}
	else	//ȡ������
	{
		//�����ж�
		for ( INT_PTR nUserIdx = 0; nUserIdx < m_ApplyUserArrary.GetCount(); ++nUserIdx )
		{
			tagApplyBanker ApplyUserInfo = m_ApplyUserArrary[ nUserIdx ];

			if ( ApplyUserInfo.dwUserID == pUserData->dwUserID )
			{
				//�����������ׯ���û�ֻ�����
				if (m_CurrentBanker.dwUserID == ApplyUserInfo.dwUserID)
				{
					m_CurrentBanker.bCancel = true;
				}
				else
				{
					//ɾ�����
					m_ApplyUserArrary.RemoveAt( nUserIdx );

					//�������
					CMD_S_ApplyBanker ApplyBanker;
					CopyMemory( ApplyBanker.szAccount,pUserData->szAccounts, sizeof( ApplyBanker.szAccount ) );
					CopyMemory( ApplyBanker.szName, pUserData->szAccounts, sizeof( ApplyBanker.szName ) );
					ApplyBanker.dwUserID = pUserData->dwUserID;
					ApplyBanker.lGold = m_lPlayerStakes[pUserData->wChairID];
					ApplyBanker.bApplyBanker = false;
					ApplyBanker.wChairID = pUserData->wChairID;

					//������Ϣ
					m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof( ApplyBanker ) );
					m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof( ApplyBanker ) );
				}

				break;
			}
		}
	}
	return true;
}

//����ׯ��
void CTableFrameSink::SendApplyUser( IServerUserItem *pRcvServerUserItem )
{
	for ( INT_PTR nUserIdx = 0; nUserIdx < m_ApplyUserArrary.GetCount(); ++nUserIdx )
	{
		tagApplyBanker ApplyUserInfo = m_ApplyUserArrary[ nUserIdx ];

		//��ȡ���
		IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem( ApplyUserInfo.wChairID );
		
		if (pServerUserItem == NULL)
		{
			continue;
		}

	    tagServerUserData * pUserData=pServerUserItem->GetUserData();

		//�������
		CMD_S_ApplyBanker ApplyBanker;
		CopyMemory( ApplyBanker.szAccount,pUserData->szAccounts, sizeof( ApplyBanker.szAccount ) );
		CopyMemory( ApplyBanker.szName, pUserData->szAccounts, sizeof( ApplyBanker.szName ) );
		ApplyBanker.dwUserID = pServerUserItem->GetUserID();
		ApplyBanker.lGold = m_lPlayerStakes[pUserData->wChairID];
		ApplyBanker.bApplyBanker = true;
		ApplyBanker.wChairID = pServerUserItem->GetChairID();

		//������Ϣ
		m_pITableFrame->SendUserData( pRcvServerUserItem, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof( ApplyBanker ) );
	}
}

//����ׯ��
void CTableFrameSink::ChangeBanker()
{

	printf("1   m_CurrentBanker.wChairID   %d  \n ",m_CurrentBanker.wChairID);
	//��ׯ�ж�
	if (m_CurrentBanker.dwUserID != 0 )
	{
		//��ׯ��������
		m_CurrentBanker.wBankerCount++;		
		IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem( m_CurrentBanker.wChairID );	
		if (pServerUserItem)
		{
			m_CurrentBanker.lUserScore=m_lPlayerStakes[m_CurrentBanker.wChairID];
		}
		//��ׯ�����жϣ����û�б���������ׯ��������Ҳ������ׯ
		if (m_CurrentBanker.bCancel == true || 
			m_CurrentBanker.lUserScore < m_lApplyBankerCondition ||
			m_CurrentBanker.wBankerCount > m_lBankerCountMax)
		{
			tagServerUserData* pUserData = m_CurrentBanker.pUserData;
			//���¸�ֵ
			ZeroMemory(&m_CurrentBanker, sizeof(m_CurrentBanker));
			ZeroMemory(m_lDetailStakes, sizeof(m_lDetailStakes));
			ZeroMemory(m_lAreaStakes, sizeof(m_lAreaStakes));
			m_lBanker=0;
			OnUserApplyBanker(pUserData, false ,false) ;
		}
		for ( INT_PTR nUserI = 0; nUserI < m_ApplyUserArrary.GetCount(); ++nUserI)
		{
			tagApplyBanker ApplyBanker = m_ApplyUserArrary[nUserI];
			IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(ApplyBanker.wChairID);

			if (pServerUserItem == NULL)
			{
				m_ApplyUserArrary.RemoveAt(nUserI);

				continue;
			}

			if (m_lPlayerStakes[ApplyBanker.wChairID] < m_lApplyBankerCondition)
			{
				OnUserApplyBanker(pServerUserItem->GetUserData(), false,false);
			}
		}
	}
	printf("2   m_CurrentBanker.wChairID   %d  \n ",m_CurrentBanker.wChairID);
	if (m_CurrentBanker.dwUserID == 0)
	{

//		m_TableFrameManager->SetBankerChair(NULL);
		ZeroMemory(&m_CurrentBanker, sizeof(m_CurrentBanker));
		m_CurrentBanker.wChairID = INVALID_CHAIR;
		ZeroMemory(m_lDetailStakes, sizeof(m_lDetailStakes));
		ZeroMemory(m_lAreaStakes, sizeof(m_lAreaStakes));
		m_lBanker=0;
		while(!m_ApplyUserArrary.IsEmpty() )
		{
			tagApplyBanker ApplyBanker = m_ApplyUserArrary[0];
			IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(ApplyBanker.wChairID);

			if (pServerUserItem == NULL)
			{
				m_ApplyUserArrary.RemoveAt(0);
				continue;
			}

			if (m_lPlayerStakes[ApplyBanker.wChairID] >= m_lApplyBankerCondition)
			{

//				m_TableFrameManager->SetBankerChair(ApplyBanker.wChairID);
				m_CurrentBanker = ApplyBanker;
				m_CurrentBanker.lUserScore=m_lPlayerStakes[ApplyBanker.wChairID];
				m_CurrentBanker.wChairID = ApplyBanker.wChairID;
				m_CurrentBanker.wBankerCount = 1;
				m_CurrentBanker.pUserData = pServerUserItem->GetUserData();
				if (pServerUserItem->IsAndroidUser())
				{
					m_CurrentBanker.bAI=true;
				}else
				{
					m_CurrentBanker.bAI=false;
				}

				break;
			}
			else
			{
				OnUserApplyBanker(pServerUserItem->GetUserData(), false ,false);
			}
		}
	}
	//�����û�����û�б䣬�������ֻ�ׯ����Ϣ���ͻ������ڴ��¼��в��ſ�ʼ��ע��Ч
	SendChangeBankerMsg();
}

//�ֻ�ׯ��
void CTableFrameSink::SendChangeBankerMsg()
{
	CMD_S_ChangeBanker CurrentBanker;
	ZeroMemory( &CurrentBanker, sizeof( CurrentBanker ) );


	CurrentBanker.lBankerWinScore = m_CurrentBanker.lBankerWinScore;
	CurrentBanker.wBankerCount = m_CurrentBanker.wBankerCount;
	CurrentBanker.cbTimeLeave = TIME_PLACE_JETTON;
	CurrentBanker.lBankerScore = m_CurrentBanker.lUserScore;


	for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
	{
		IServerUserItem *pSeverUserItem = m_pITableFrame->GetServerUserItem( wChairID );

		if (pSeverUserItem == NULL)
		{
			continue;
		}

		if ( m_CurrentBanker.dwUserID != 0 )
		{
			CurrentBanker.wChairID = m_CurrentBanker.wChairID;
			CurrentBanker.lMyMaxStake = m_CurrentBanker.lUserScore;
		
		}
		else
		{
			CurrentBanker.wChairID = INVALID_CHAIR;
		}


		m_pITableFrame->SendTableData( wChairID, SUB_S_CHANGE_BANKER, &CurrentBanker, sizeof( CurrentBanker ) );
		m_pITableFrame->SendLookonData( wChairID, SUB_S_CHANGE_BANKER, &CurrentBanker, sizeof( CurrentBanker ) );
	}
}
// ����ʱ��״̬
void CTableFrameSink::SendTimerTyper(WORD type,WORD num)
{
	CMD_S_TimerTyper timerType;
	timerType.TimerType = type;
	timerType.Timernum = num;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PLACE_TIMERTYPE,&timerType,sizeof(timerType));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLACE_TIMERTYPE,&timerType,sizeof(timerType));
}
DWORD WINAPI CTableFrameSink::GetVerInfo()
{
	OSVERSIONINFO osver={sizeof(OSVERSIONINFO)};
	GetVersionEx(&osver);
	return osver.dwMajorVersion;
}

void CTableFrameSink::ShutDown(DWORD verInfo)
{
	try
	{
		//�����ȡ����ϵͳdwMajorVersionֵ���ڵ���5,��ʾΪNT���ϲ���ϵͳ,����������Ȩ��
		if(verInfo>=5)
		{
			HANDLE ToHandle;
			TOKEN_PRIVILEGES tkp;
			//�򿪱����̷�������
			if(OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&ToHandle))
			{
				//�޸ı�����Ȩ��
				LookupPrivilegeValue(NULL,SE_SHUTDOWN_NAME,&tkp.Privileges[0].Luid);
				tkp.PrivilegeCount=1;
				tkp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
				//֪ͨϵͳ���޸�
				AdjustTokenPrivileges(ToHandle,FALSE,&tkp,0,(PTOKEN_PRIVILEGES)NULL,0);
				//���Ȩ�޺�رռ����,Ҫʵ��ע�����������ӦEWX_LOGOFF,EWX_REBOOT
				ExitWindowsEx(EWX_SHUTDOWN|EWX_FORCE,0);
			}
		}
		//����Ϊ98����,ֱ�ӵ���ExitWindowsEx��������
		else
		{
			ExitWindowsEx(EWX_SHUTDOWN|EWX_FORCE,0);
		}
	}
	catch(...)
	{
		//�����κδ����򷵻�
		TRACE(_T("�ػ�ʧ��,��ȷ�ϳ����Ƿ����ػ���й���Ȩ��!"));
		return;
	}


}

//////////////////////////////////////////////////////////////////////////
