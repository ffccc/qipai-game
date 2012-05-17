#include "StdAfx.h"
#include "TableFrameSink.h"

extern void WriteLog(CString strFileName, CString strText);
//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_FullReady;	//��ʼģʽ

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ����
	m_wSiceCount=MAKEWORD(1,1);

	m_wSiceCount2 =MAKEWORD(1,1);

	m_wBankerUser=INVALID_CHAIR;//û��ȷ��ׯ��
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

	//������Ϣ
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//������Ϣ
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));

	//���б���
	m_cbProvideCard=0;
	m_wTingUser = INVALID_CHAIR;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//״̬����
	m_bSendStatus=false;
	m_bGangStatus=false;
	ZeroMemory(m_bEnjoinChiHu,sizeof(m_bEnjoinChiHu));
	ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));

	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//����齫
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	//������Ϣ
	m_cbChiHuCard=0;
	ZeroMemory(&m_ChiHuResult,sizeof(m_ChiHuResult));

	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

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
	QUERYINTERFACE(ITableFrameEvent,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//��ʼ��
bool __cdecl CTableFrameSink::InitTableFrameSink(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=GET_OBJECTPTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	return true;
}

//��λ����
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//������	
	memset( (char *)m_bTingFlag, 0, sizeof(m_bTingFlag));

	//��Ϸ����
	m_wSiceCount=MAKEWORD(1,1);
	m_wSiceCount2 =MAKEWORD(1,1);

	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

	//������Ϣ
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//������Ϣ
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));

	//���б���
	m_cbProvideCard=0;
	m_wTingUser = INVALID_CHAIR;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//״̬����
	m_bSendStatus=false;
	m_bGangStatus=false;
	ZeroMemory(m_bEnjoinChiHu,sizeof(m_bEnjoinChiHu));
	ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));

	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//����齫
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	//������Ϣ
	m_cbChiHuCard=0;
	ZeroMemory(&m_ChiHuResult,sizeof(m_ChiHuResult));

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
	//����״̬
	m_pITableFrame->SetGameStatus(GS_MJ_PLAY);

	//�����齫
	m_wSiceCount=MAKEWORD(rand()%6+1,rand()%6+1);

	m_wSiceCount2=MAKEWORD(rand()%6+1,rand()%6+1);

	m_cbLeftCardCount=CountArray(m_cbRepertoryCard);
	m_GameLogic.RandCardData(m_cbRepertoryCard,CountArray(m_cbRepertoryCard));

	//�ַ��齫
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_cbLeftCardCount-=(MAX_COUNT-1);
		m_GameLogic.SwitchToCardIndex(&m_cbRepertoryCard[m_cbLeftCardCount],MAX_COUNT-1,m_cbCardIndex[i]);
	}

	//�����齫
	m_cbSendCardCount++;
	m_cbSendCardData=m_cbRepertoryCard[--m_cbLeftCardCount];
	m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

	//���ñ���
	m_cbProvideCard=0;
	m_wProvideUser=INVALID_CHAIR;
	m_wCurrentUser=m_wBankerUser;//��ǰ������

	//��������
	bool bAroseAction=false;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//�������
		//m_cbUserAction[i]|=m_GameLogic.EstimateChiHu(m_cbCardIndex[i]);

		//ׯ���ж�
		if (i==m_wBankerUser)
		{
			//�����ж�
			tagGangCardResult GangCardResult;
			m_cbUserAction[i]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[i],NULL,0,GangCardResult);

			//�����ж�
			tagChiHuResult ChiHuResult;
			m_cbUserAction[i]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],NULL,0,0,0,ChiHuResult);
		}

		//״̬����
		if ((bAroseAction==false)&&(i!=m_wBankerUser)&&(m_cbUserAction[i]!=WIK_NULL))
		{
			bAroseAction=true;

			m_wResumeUser=m_wCurrentUser;
			m_wCurrentUser=INVALID_CHAIR;
		}//End if
	}

	//��������
	CMD_S_GameStart GameStart;
	GameStart.wSiceCount=m_wSiceCount;
	GameStart.wSiceCount2=m_wSiceCount2;

	GameStart.wBankerUser=m_wBankerUser;//����ׯ��
	GameStart.wCurrentUser=m_wCurrentUser;

	//��������
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//���ñ���
		GameStart.cbUserAction=m_cbUserAction[i];
		m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameStart.cbCardData);

		//��������
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}

	return true;
}
/*
���Ƶ�������

��  �ƣ�1��

��  �ͣ�1�㣨����Ҳ���ڿ��ͣ�

��һɫ��5��

һ������5��

��С�ԣ�1�㣨��С��Ҳ�����������ۼӣ�

ȱһ�ţ�1��

ׯ�ң�(��Nׯ) N�㡣��ʼ1��

ע�⣺

1��  ������û�����ƶ��1�㣨������ͬ�������֧��ͬ��������

2��  ׯ������Ҫ�ӿ���ׯ��

3��  �������е���������

4��  ����1�㣬����2�㣬������Ч���������ӱ���

 

�÷־�����

1 ��ׯ�� ���ƣ����ͣ�ȱһ�ţ�������

ׯ�ң���ׯ2�Σ�֧��6+2*2=10�㣬�������֧��3*2=6�� ������1�㣬����1�㣬ȱһ��1�㣩��

2 ���˷���ׯ�Һ��ƣ���ׯ2�Σ�һ���������и����ܣ�

�����ߣ�δ���ƣ�֧��9�㣬�������֧��8�㣨����1�㣬ׯ��2�㣬һ����5�㣩

��Ϊ�и����ܣ����ٶึ2�㣨���������Ҳ��2�㣩��

*/
//��Ϸ����
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sOnEventGameEnd.log",strTime);

	strTemp.Format("OnEventGameEnd") ;
	WriteLog(strFile, strTemp);

	int baseInt = 1;//������

	switch (cbReason)
	{
	case GER_NORMAL:		//�������
		{
			strTemp.Format("�������") ;
			WriteLog(strFile, strTemp);

			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			LONG lCellScore=m_pGameServiceOption->lCellScore;
			BYTE cbBirdValue=(m_cbRepertoryCard[0]&MASK_VALUE);

			//������Ϣ
			GameEnd.wProvideUser=wChairID;
			GameEnd.cbChiHuCard=m_cbChiHuCard;
			for (WORD i=0;i<m_wPlayerCount;i++) GameEnd.wChiHuKind[i]=m_ChiHuResult[i].wChiHuKind;


			strTemp.Format("wProvideUser=%d,cbChiHuCard=%d,",
				//GameEnd.cbCardCount,
				//GameEnd.cbCardData,
				GameEnd.wProvideUser ,
				GameEnd.cbChiHuCard
				//GameEnd.lGameScore,
				//GameEnd.wChiHuKind,
				);
				
			WriteLog(strFile, strTemp);

			//�����齫
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
			}//End for


			//��������
			WORD wChiHuUserCount=0;

			//ͳ�ƻ���
			if (m_wProvideUser!=INVALID_CHAIR)
			{
				//��������
				if (m_ChiHuResult[m_wProvideUser].wChiHuKind!=CHK_NULL)
				{
					strTemp.Format("��������  �������е�������") ;
					WriteLog(strFile, strTemp);

					//��������
					BYTE cbChiHuOrder=m_GameLogic.GetChiHuActionRank(m_ChiHuResult[m_wProvideUser]);

					strTemp.Format("cbChiHuOrder=%d", cbChiHuOrder) ;
					WriteLog(strFile, strTemp);

					//ѭ���ۼ�
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						//Ӯ�ҹ���
						if (i==m_wProvideUser) 
						{
							GameEnd.lGameScore[i] = cbChiHuOrder;
							continue;
						}
						else
						{
							//����ͳ��
							GameEnd.lGameScore[i] = -cbChiHuOrder;
						}
					}

					//������Ŀ
					wChiHuUserCount=1;

					//ׯ������
					//
					//Del by 20080101 m_wBankerUser=m_wProvideUser;//ǰһ��

//��������
					WORD wNextBankerUser=INVALID_CHAIR;

					//ѭ���ۼ�
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						//��ҹ���
						if (m_ChiHuResult[i].wChiHuKind==CHK_NULL) continue;

						//���ñ���
						wNextBankerUser=i;

					}//End for
					//�����ׯ������
					if ( m_wProvideUser == m_wBankerUser)
					{
						//ׯ�Ҳ��䶯
					}
					else
					{
						m_wBankerUser = wNextBankerUser;//��һ��
					}
				}//End if (m_ChiHuResult[m_wProvideUser].wChiHuKind!=CHK_NULL)

				//׽������
				if (m_ChiHuResult[m_wProvideUser].wChiHuKind==CHK_NULL)
				{
					strTemp.Format("׽������") ;
					WriteLog(strFile, strTemp);

					//��������
					WORD wNextBankerUser=INVALID_CHAIR;

					//����״̬
					for (int k=0; k < m_wPlayerCount; k++)
					{
						//GameEnd.cbTingFlag[k] = 0;
						//����Ѿ�����
						if (m_bTingFlag[k])
						{
						//	GameEnd.cbTingFlag[k]=1;
							strTemp.Format("��=%d", k) ;
							WriteLog(strFile, strTemp);

						}
					}//End for

					//����÷�
					int mjResultInt = 0;//�÷�
					for(int i=0; i < m_wPlayerCount; i++)
					{
					//��������
						BYTE cbChiHuOrder=m_GameLogic.GetChiHuActionRank(m_ChiHuResult[i]);

						strTemp.Format("m_ChiHuResult[i]=0x%x", m_ChiHuResult[i]) ;
						WriteLog(strFile, strTemp);

						if (m_ChiHuResult[i].wChiHuKind != CHK_NULL)
						{
							//����
							int fanInt = m_GameLogic.GetWinMJNum(  m_cbCardIndex[i], MAX_INDEX);	

							//�Ƿ�Ϊ���� //CHK_JI_HU //CHK_PING_HU
							m_ChiHuResult[i].wChiHuKind &=0x0001;

							strTemp.Format("m_ChiHuResult[i]=0x%x", m_ChiHuResult[i]) ;
							WriteLog(strFile, strTemp);

							if ( (m_ChiHuResult[i].wChiHuKind == CHK_JI_HU) || (m_ChiHuResult[i].wChiHuKind == CHK_PING_HU) )
							{
								fanInt ++;
								strTemp.Format("Ϊ����") ;
								WriteLog(strFile, strTemp);

							}	

							strTemp.Format("m_ChiHuResult[i]=0x%x", m_ChiHuResult[i]) ;
							WriteLog(strFile, strTemp);

							mjResultInt = baseInt + fanInt;

							break;
						}//End if
					}//End for

					strTemp.Format("�÷� mjResultInt=%d", mjResultInt) ;
					WriteLog(strFile, strTemp);

					//ѭ���ۼ�
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						//��������
						BYTE cbChiHuOrder=m_GameLogic.GetChiHuActionRank(m_ChiHuResult[i]);

						//strTemp.Format("cbChiHuOrder=%d, %d", cbChiHuOrder, m_ChiHuResult[i].wChiHuKind) ;
						//WriteLog(strFile, strTemp);


						//���� 3��
						if (m_ChiHuResult[i].wChiHuKind==CHK_NULL)
						{
							//����ͳ��
							//���ڵ�
							if ( m_wProvideUser == i)
							{
								GameEnd.lGameScore[i] = -mjResultInt;

								if (!m_bTingFlag[i])
								{
									GameEnd.lGameScore[i]-= 1;//û�����ƣ�Ҫ���һ��

									strTemp.Format("û�����ƣ�Ҫ���һ�� GameEnd.lGameScore[i]=%d", GameEnd.lGameScore[i]) ;
									WriteLog(strFile, strTemp);

								}
							
								//ׯ�ҵ���
								if (m_wBankerUser == m_wProvideUser)
								{
									GameEnd.lGameScore[i] -= 1;

									strTemp.Format("ׯ�ҵ��� GameEnd.lGameScore[i]=%d", GameEnd.lGameScore[i]) ;
									WriteLog(strFile, strTemp);

								}
							}
							else
							{
								GameEnd.lGameScore[i] = -mjResultInt;
							}
							continue;
						}
						//Ӯ��
						else
						{
							GameEnd.lGameScore[i] = mjResultInt*3;

							//����ͳ��
							if (!m_bTingFlag[m_wProvideUser])
							{
								GameEnd.lGameScore[i] +=1;//
								strTemp.Format(" Ӯ�� ����δ���� GameEnd.lGameScore[i]=%d", GameEnd.lGameScore[i]) ;
								WriteLog(strFile, strTemp);
	
							}
		
						//ׯ�ҵ���
							if (m_wBankerUser == m_wProvideUser)
							{
								GameEnd.lGameScore[i] += 1;//

								strTemp.Format(" Ӯ�� ׯ�ҵ��� GameEnd.lGameScore[i]=%d", GameEnd.lGameScore[i]) ;
								WriteLog(strFile, strTemp);
							}

                         //ׯ��Ӯ��
							if (i == m_wBankerUser)
							{
								GameEnd.lGameScore[i] += 1;//

								strTemp.Format(" ׯ��Ӯ�� GameEnd.lGameScore[i]=%d", GameEnd.lGameScore[i]) ;
								WriteLog(strFile, strTemp);	
							}
							strTemp.Format("Ӯ�ҵķ�%d, i=%d", mjResultInt, i) ;
							WriteLog(strFile, strTemp);
						}//End if

						//���ñ���
						wNextBankerUser=i;
						wChiHuUserCount++;
					}//End for

					//ׯ������
					m_wBankerUser=(wChiHuUserCount>=2)?m_wProvideUser:wNextBankerUser;

					m_wBankerUser = wNextBankerUser;
				}//End if
			}
			else if ((m_cbOutCardCount==0)&&(m_cbSendCardCount==1))
			{
				strTemp.Format("m_cbOutCardCount==0)&&(m_cbSendCardCount==1") ;
				WriteLog(strFile, strTemp);

				//��������
				WORD wNextBankerUser=INVALID_CHAIR;

				//�������
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					//��ҹ���
					if (m_ChiHuResult[i].wChiHuKind==CHK_NULL) continue;

					//ѭ���ۼ�
					if ((m_ChiHuResult[i].wChiHuKind&0x00F0)!=0)
					{
						for (WORD j=0;j<m_wPlayerCount;j++)
						{
							if (m_ChiHuResult[j].wChiHuKind==CHK_NULL)
							{
							}
						}//End for (WORD j=0;j<m_wPlayerCount;j++)
					}
					else
					{
						for (WORD j=0;j<m_wPlayerCount;j++)
						{
							if (m_ChiHuResult[j].wChiHuKind==CHK_NULL)
							{
							}
						}//End for (WORD j=0;j<m_wPlayerCount;j++)
					}//End if

					//���ñ���
					wChiHuUserCount++;
					wNextBankerUser=i;
				}//End for (WORD i=0;i<m_wPlayerCount;i++)

				//ׯ������
				if (wChiHuUserCount==1) m_wBankerUser=wNextBankerUser;
			}
			else
			{

				strTemp.Format("��ׯ����") ;
				WriteLog(strFile, strTemp);
				//��ׯ����
				GameEnd.cbChiHuCard=0;
				//ׯ�Ҳ��䶯

			}//End if

			//�����齫
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
			}//End for

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//�޸Ļ���
			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				ScoreInfo[i].lScore=GameEnd.lGameScore[i];
				if (GameEnd.lGameScore[i]==0L) ScoreInfo[i].ScoreKind=enScoreKind_Draw;
				else ScoreInfo[i].ScoreKind=(GameEnd.lGameScore[i]>0L)?enScoreKind_Win:enScoreKind_Lost;
			}//End for
			m_pITableFrame->WriteTableScore(ScoreInfo,m_wPlayerCount,0L);

			//������Ϸ
			m_pITableFrame->ConcludeGame();

			return true;
		}
	case GER_USER_LEFT:		//�û�ǿ��
		{
		strTemp.Format("�û�ǿ��") ;
			WriteLog(strFile, strTemp);

			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//���ñ���
			GameEnd.wProvideUser=INVALID_CHAIR;
			GameEnd.lGameScore[wChairID]=-24*m_pGameServiceOption->lCellScore;

			//֪ͨ��Ϣ
			TCHAR szMessage[128]=TEXT("");
			_snprintf(szMessage,CountArray(szMessage),TEXT("���� [ %s ] �뿪��Ϸ����Ϸ����"),pIServerUserItem->GetAccounts());
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				IServerUserItem * pISendUserItem=m_pITableFrame->GetServerUserItem(i);
				if (pISendUserItem!=NULL) m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_INFO);
			}
			WORD wIndex=0;
			do
			{
				IServerUserItem * pISendUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
				if (pISendUserItem==NULL) break;
				m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_INFO);
			} while (true);

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//�޸Ļ���
			tagScoreInfo ScoreInfo;
			ScoreInfo.ScoreKind=enScoreKind_Flee;
			ScoreInfo.lScore=GameEnd.lGameScore[wChairID];
			m_pITableFrame->WriteUserScore(wChairID,ScoreInfo);

			//������Ϸ
			m_pITableFrame->ConcludeGame();

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
	case GS_MJ_FREE:	//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));

			//��������
			StatusFree.wBankerUser=m_wBankerUser;//
			StatusFree.lCellScore=m_pGameServiceOption->lCellScore;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_MJ_PLAY:
		{
		}//��Ϸ״̬
	case GS_MJ_HAI_DI:	//����״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//��Ϸ����
			StatusPlay.wSiceCount=m_wSiceCount;
			StatusPlay.wSiceCount2=m_wSiceCount2;

			StatusPlay.wBankerUser=m_wBankerUser;//
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.lCellScore=m_pGameServiceOption->lCellScore;

			//״̬����
			StatusPlay.cbActionCard=m_cbProvideCard;
			StatusPlay.cbLeftCardCount=m_cbLeftCardCount;
			StatusPlay.cbActionMask=(m_bResponse[wChiarID]==false)?m_cbUserAction[wChiarID]:WIK_NULL;

			//��ʷ��¼
			StatusPlay.wOutCardUser=m_wOutCardUser;
			StatusPlay.cbOutCardData=m_cbOutCardData;
			CopyMemory(StatusPlay.cbDiscardCard,m_cbDiscardCard,sizeof(StatusPlay.cbDiscardCard));
			CopyMemory(StatusPlay.cbDiscardCount,m_cbDiscardCount,sizeof(StatusPlay.cbDiscardCount));

			//����齫
			CopyMemory(StatusPlay.WeaveItemArray,m_WeaveItemArray,sizeof(m_WeaveItemArray));
			CopyMemory(StatusPlay.cbWeaveCount,m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

			//�齫����
			StatusPlay.cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[wChiarID],StatusPlay.cbCardData);

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	return false;
}

//��ʱ���¼�
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
	return false;
}

//��Ϸ��Ϣ����
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sOnGameMessage.log",strTime);

//			strTemp.Format("OnGameMessage ");
//			WriteLog(strFile, strTemp);

	switch (wSubCmdID)
	{
//����
	case SUB_C_TING_CARD:
		{
		//	OnUserTingCard(WORD wChairID, BYTE cbCardData);
			//Ч����Ϣ
			ASSERT(wDataSize==sizeof(CMD_C_TingCard));
			if (wDataSize!=sizeof(CMD_C_TingCard)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			CMD_C_TingCard * pOutCard=(CMD_C_TingCard *)pDataBuffer;
			return OnUserTingCard(pUserData->wChairID,pOutCard->cbCardData);

		}
		break;
	case SUB_C_OUT_CARD:		//������Ϣ
		{
			//Ч����Ϣ
			ASSERT(wDataSize==sizeof(CMD_C_OutCard));
			if (wDataSize!=sizeof(CMD_C_OutCard)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pDataBuffer;
			return OnUserOutCard(pUserData->wChairID,pOutCard->cbCardData);
		}
	case SUB_C_OPERATE_CARD:	//������Ϣ
		{
			//Ч����Ϣ
			ASSERT(wDataSize==sizeof(CMD_C_OperateCard));
			if (wDataSize!=sizeof(CMD_C_OperateCard)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			CMD_C_OperateCard * pOperateCard=(CMD_C_OperateCard *)pDataBuffer;
			return OnUserOperateCard(pUserData->wChairID,pOperateCard->cbOperateCode,pOperateCard->cbOperateCard);
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
bool __cdecl CTableFrameSink::OnEventUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//ׯ������
	if ((bLookonUser==false)&&(m_wBankerUser==INVALID_CHAIR))
	{
		m_wBankerUser=pIServerUserItem->GetChairID();//��һ�����µĵ�ׯ
	}

	return true;
}

//�û�����
bool __cdecl CTableFrameSink::OnEventUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//ׯ������
	if ((bLookonUser==false)&&(wChairID==m_wBankerUser))
	{
		m_wBankerUser=INVALID_CHAIR;//����ߵ���ׯ��

		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if ((i!=wChairID)&&(m_pITableFrame->GetServerUserItem(i)!=NULL))
			{
				m_wBankerUser=i;
				break;
			}//End if
		}//End for
	}//End if ((bLookonUser

	return true;
}
//�û�TING��
bool CTableFrameSink::OnUserTingCard(WORD wChairID, BYTE cbCardData)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sOnUserTingCard.log",strTime);

	strTemp.Format("into OnUserTingCard(%d,%d", wChairID,  cbCardData) ;
	WriteLog(strFile, strTemp);


	WORD i=0 ;
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

	//�������
//	ASSERT(wChairID==m_wCurrentUser);
//	ASSERT(m_GameLogic.IsValidCard(cbCardData)==true);

	//Ч�����
//	if (wChairID!=m_wCurrentUser) return false;
//	if (m_GameLogic.IsValidCard(cbCardData)==false) return false;

	m_bTingFlag[wChairID] = true;
//�û�����
	//��������
	CMD_S_GameTingCard OutCard;
	OutCard.wTingCardUser=wChairID;


	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TING_CARD,&OutCard,sizeof(OutCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TING_CARD,&OutCard,sizeof(OutCard));



	return true;	
}

//�û�����
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData)
{
	WORD i=0 ;
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

	//�������
	ASSERT(wChairID==m_wCurrentUser);
	ASSERT(m_GameLogic.IsValidCard(cbCardData)==true);

	//Ч�����
	if (wChairID!=m_wCurrentUser) return false;
	if (m_GameLogic.IsValidCard(cbCardData)==false) return false;

	//ɾ���齫
	if (m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],cbCardData)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//���ñ���
	m_bSendStatus=true;
	m_bEnjoinChiHu[wChairID]=false;
	m_cbUserAction[wChairID]=WIK_NULL;
	m_cbPerformAction[wChairID]=WIK_NULL;

	//���Ƽ�¼
	m_cbOutCardCount++;
	m_wOutCardUser=wChairID;
	m_cbOutCardData=cbCardData;

	//��������
	CMD_S_OutCard OutCard;
	OutCard.wOutCardUser=wChairID;
	OutCard.cbOutCardData=cbCardData;

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));

	//�û��л�
	m_wProvideUser=wChairID;
	m_cbProvideCard=cbCardData;
	m_wCurrentUser=(wChairID+m_wPlayerCount-1)%m_wPlayerCount;

	//��Ӧ�ж�
	bool bAroseAction=EstimateUserRespond(wChairID,cbCardData,EstimatKind_OutCard);

	//��������
	if (m_bGangStatus==true)
	{
		for (i=0;i<m_wPlayerCount;i++)
		{
			if ((m_cbUserAction[i]&WIK_CHI_HU)!=0) break;
		}
		if (i==m_wPlayerCount) m_bGangStatus=false;
	}

	//�ɷ��齫
	if (bAroseAction==false) DispatchCardData(m_wCurrentUser);

	return true;
}

//�û�����
bool CTableFrameSink::OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sOnUserOperateCard.log",strTime);

	strTemp.Format("OnUserOperateCard") ;
	WriteLog(strFile, strTemp);

	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;
	//Ч���û�
	ASSERT((wChairID==m_wCurrentUser)||(m_wCurrentUser==INVALID_CHAIR));

	BYTE cbTargetAction=cbOperateCode;
	//�Ƿ����Ʋ���
	if (cbTargetAction==WIK_TING_CARD)
	{
		//m_bTingFlag[wChairID] = true;
		//֪ͨ���

		//��������
		CMD_S_GameTingCard SendCard;
		SendCard.wTingCardUser= m_wCurrentUser;

		//��������
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TING_CARD,&SendCard,sizeof(SendCard));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TING_CARD,&SendCard,sizeof(SendCard));

		return true;
	}//End if


	if ((wChairID!=m_wCurrentUser)&&(m_wCurrentUser!=INVALID_CHAIR)) return false;

	//��������
	if (m_wCurrentUser==INVALID_CHAIR)
	{
		//Ч��״̬
		ASSERT(m_bResponse[wChairID]==false);
		ASSERT((cbOperateCode==WIK_NULL)||((m_cbUserAction[wChairID]&cbOperateCode)!=0));

		//Ч��״̬
		if (m_bResponse[wChairID]==true) return false;
		if ((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)==0)) return false;

		//��������
		WORD wTargetUser=wChairID;
		BYTE cbTargetAction=cbOperateCode;

		//���ñ���
		m_bResponse[wChairID]=true;
		m_cbPerformAction[wChairID]=cbOperateCode;
		m_cbOperateCard[wChairID]=(cbOperateCard==0)?m_cbProvideCard:cbOperateCard;

		//ִ���ж�
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//��ȡ����
			BYTE cbUserAction=(m_bResponse[i]==false)?m_cbUserAction[i]:m_cbPerformAction[i];

			//���ȼ���
			BYTE cbUserActionRank=m_GameLogic.GetUserActionRank(cbUserAction);
			BYTE cbTargetActionRank=m_GameLogic.GetUserActionRank(cbTargetAction);

			//�����ж�
			if (cbUserActionRank>cbTargetActionRank)
			{
				wTargetUser=i;
				cbTargetAction=cbUserAction;
			}
		}//End for (WORD i=0;i<m_wPlayerCount;i++)
		if (m_bResponse[wTargetUser]==false) return true;

		//�Ժ��ȴ�
		if (cbTargetAction==WIK_CHI_HU)
		{
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if ((m_bResponse[i]==false)&&(m_cbUserAction[i]&WIK_CHI_HU)) return true;
			}
		}//End if (cbTargetAction==WIK_CHI_HU)

		//��������
		if (cbTargetAction==WIK_NULL)
		{
			//�û�״̬
			ZeroMemory(m_bResponse,sizeof(m_bResponse));
			ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
			ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
			ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

			//�����齫
			DispatchCardData(m_wResumeUser);

			return true;
		}//End if (cbTargetAction==WIK_NULL)

		//��������
		BYTE cbTargetCard=m_cbOperateCard[wTargetUser];

		//���Ʊ���
		m_cbOutCardData=0;
		m_bSendStatus=true;
		m_wOutCardUser=INVALID_CHAIR;

		//���Ʋ���
		if (cbTargetAction==WIK_CHI_HU)
		{
			//������Ϣ
			m_cbChiHuCard=cbTargetCard;
			m_wProvideUser=m_wProvideUser;

			//����Ȩλ
			WORD wChiHuRight=0;
			if (m_bGangStatus==true) wChiHuRight|=CHR_QIANG_GANG;
			if ((m_cbSendCardCount==1)&&(m_cbOutCardCount==1)) wChiHuRight|=CHR_DI;
			if ((m_cbSendCardCount==1)&&(m_cbOutCardCount==0)) wChiHuRight|=CHR_TIAN;

			//�����ж�
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//�����ж�
				if ((i==m_wProvideUser)||((m_cbPerformAction[i]&WIK_CHI_HU)==0)) continue;

				//��ͨ����
				if (m_cbChiHuCard!=0)
				{
					//�����ж�
					BYTE cbWeaveItemCount=m_cbWeaveItemCount[i];
					tagWeaveItem * pWeaveItem=m_WeaveItemArray[i];
					m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],pWeaveItem,cbWeaveItemCount,m_cbChiHuCard,wChiHuRight,m_ChiHuResult[i]);

					//�����齫
					if (m_ChiHuResult[i].wChiHuKind!=CHK_NULL) m_cbCardIndex[i][m_GameLogic.SwitchToCardIndex(m_cbChiHuCard)]++;
				}
			}//End for (WORD i=0;i<m_wPlayerCount;i++)

			//������Ϸ
			ASSERT(m_ChiHuResult[wTargetUser].wChiHuKind!=CHK_NULL);
			OnEventGameEnd(m_wProvideUser,NULL,GER_NORMAL);

			return true;
		}//End if (cbTargetAction==WIK_CHI_HU)

		//�û�״̬
		ZeroMemory(m_bResponse,sizeof(m_bResponse));
		ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
		ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
		ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

		//����齫
		ASSERT(m_cbWeaveItemCount[wTargetUser]<4);
		WORD wIndex=m_cbWeaveItemCount[wTargetUser]++;
		m_WeaveItemArray[wTargetUser][wIndex].cbPublicCard=TRUE;
		m_WeaveItemArray[wTargetUser][wIndex].cbCenterCard=cbTargetCard;
		m_WeaveItemArray[wTargetUser][wIndex].cbWeaveKind=cbTargetAction;
		m_WeaveItemArray[wTargetUser][wIndex].wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;

		//ɾ���齫
		switch (cbTargetAction)
		{
		case WIK_LEFT:		//���Ʋ���
			{
				//ɾ���齫
				BYTE cbRemoveCard[]={cbTargetCard+1,cbTargetCard+2};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

				break;
			}
		case WIK_RIGHT:		//���Ʋ���
			{
				//ɾ���齫
				BYTE cbRemoveCard[]={cbTargetCard-2,cbTargetCard-1};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

				break;
			}
		case WIK_CENTER:	//���Ʋ���
			{
				//ɾ���齫
				BYTE cbRemoveCard[]={cbTargetCard-1,cbTargetCard+1};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

				break;
			}
		case WIK_PENG:		//���Ʋ���
			{
				//ɾ���齫
				BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

				break;
			}
		case WIK_FILL:		//���Ʋ���
		case WIK_GANG:		//���Ʋ���
			{
				//ɾ���齫
				if ((m_cbSendCardCount==1)&&(m_cbOutCardData==0))
				{
					BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard,cbTargetCard,cbTargetCard};
					m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));
				}
				else
				{
					BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard,cbTargetCard};
					m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));
				}

				break;
			}
		}

		//������
		CMD_S_OperateResult OperateResult;
		OperateResult.wOperateUser=wTargetUser;
		OperateResult.cbOperateCard=cbTargetCard;
		OperateResult.cbOperateCode=cbTargetAction;
		OperateResult.wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;

		//����״̬
		if (cbTargetAction==WIK_GANG)
		{
			m_bGangStatus=true;
			m_bEnjoinChiPeng[wTargetUser]=true;
		}

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

		//�����û�
		m_wCurrentUser=wTargetUser;

		//���ƴ���
		if ((cbTargetAction==WIK_GANG)||(cbTargetAction==WIK_FILL))
		{
			//Ч�鶯��
			bool bAroseAction=EstimateUserRespond(wTargetUser,cbTargetCard,EstimatKind_GangCard);

			//�����齫
			if (bAroseAction==false) DispatchCardData(wTargetUser);

			return true;
		}

		//�����ж�
		if (m_cbLeftCardCount>1)
		{
			//�����ж�
			tagGangCardResult GangCardResult;
			m_cbUserAction[m_wCurrentUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[m_wCurrentUser],
				m_WeaveItemArray[m_wCurrentUser],m_cbWeaveItemCount[m_wCurrentUser],GangCardResult);

			//�������
			if (GangCardResult.cbCardCount>0)
			{
				//���ñ���
				m_cbUserAction[m_wCurrentUser]|=(WIK_GANG|WIK_FILL);

				//���Ͷ���
				m_wTingUser = wChairID;
				SendOperateNotify();
			}
		}

		return true;
	}//End if (m_wCurrentUser==INVALID_CHAIR)

	//��������
	if (m_wCurrentUser==wChairID)
	{
		//Ч�����
		ASSERT((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)!=0));
		if ((cbOperateCode==WIK_NULL)||((m_cbUserAction[wChairID]&cbOperateCode)==0)) return false;

		//�齫Ч��
		ASSERT((cbOperateCode==WIK_NULL)||(cbOperateCode==WIK_CHI_HU)||(m_GameLogic.IsValidCard(cbOperateCard)==true));
		if ((cbOperateCode!=WIK_NULL)&&(cbOperateCode!=WIK_CHI_HU)&&(m_GameLogic.IsValidCard(cbOperateCard)==false)) return false;

		//���ñ���
		m_bSendStatus=true;
		m_cbUserAction[wChairID]=WIK_NULL;
		m_cbPerformAction[wChairID]=WIK_NULL;

		//ִ�ж���
		switch (cbOperateCode)
		{
		case WIK_FILL:			//���Ʋ���
		case WIK_GANG:			//���Ʋ���
			{
				//��������
				BYTE cbWeaveIndex=0xFF;
				BYTE cbCardIndex=m_GameLogic.SwitchToCardIndex(cbOperateCard);

				//���ƴ���
				if (m_cbCardIndex[wChairID][cbCardIndex]==1)
				{
					//Ѱ�����
					for (BYTE i=0;i<m_cbWeaveItemCount[wChairID];i++)
					{
						BYTE cbWeaveKind=m_WeaveItemArray[wChairID][i].cbWeaveKind;
						BYTE cbCenterCard=m_WeaveItemArray[wChairID][i].cbCenterCard;
						if ((cbCenterCard==cbOperateCard)&&(cbWeaveKind==WIK_PENG))
						{
							cbWeaveIndex=i;
							break;
						}
					}

					//Ч�鶯��
					ASSERT(cbWeaveIndex!=0xFF);
					if (cbWeaveIndex==0xFF) return false;

					//����齫
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=TRUE;
					m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=cbOperateCode;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard;
				}
				else
				{
					//�齫Ч��
					ASSERT(m_cbCardIndex[wChairID][cbCardIndex]==4);
					if (m_cbCardIndex[wChairID][cbCardIndex]!=4) return false;

					//���ñ���
					cbWeaveIndex=m_cbWeaveItemCount[wChairID]++;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=FALSE;
					m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=cbOperateCode;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard;
				}

				//ɾ���齫
				m_cbCardIndex[wChairID][cbCardIndex]=0;

				//����״̬
				if (cbOperateCode==WIK_GANG)
				{
					m_bGangStatus=true;
					m_bEnjoinChiPeng[wChairID]=true;
				}

				//������
				CMD_S_OperateResult OperateResult;
				OperateResult.wOperateUser=wChairID;
				OperateResult.wProvideUser=wChairID;
				OperateResult.cbOperateCode=cbOperateCode;
				OperateResult.cbOperateCard=cbOperateCard;

				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

				//Ч�鶯��
				bool bAroseAction=EstimateUserRespond(wChairID,cbOperateCard,EstimatKind_GangCard);

				//�����齫
				if (bAroseAction==false) DispatchCardData(wChairID);

				return true;
			}
		case WIK_CHI_HU:		//�Ժ�����
			{
				//����Ȩλ
				WORD wChiHuRight=0;
				if (m_bGangStatus==true) wChiHuRight|=CHR_QIANG_GANG;
				if ((m_cbSendCardCount==1)&&(m_cbOutCardCount==1)) wChiHuRight|=CHR_DI;
				if ((m_cbSendCardCount==1)&&(m_cbOutCardCount==0)) wChiHuRight|=CHR_TIAN;

				//��ͨ����
				BYTE cbWeaveItemCount=m_cbWeaveItemCount[wChairID];
				tagWeaveItem * pWeaveItem=m_WeaveItemArray[wChairID];
				m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wChairID],pWeaveItem,cbWeaveItemCount,0,wChiHuRight,m_ChiHuResult[wChairID]);

				//������Ϣ
				m_cbChiHuCard=m_cbProvideCard;

				//������Ϸ
				OnEventGameEnd(m_wProvideUser,NULL,GER_NORMAL);

				return true;
			}
		}

		return true;
	}

	return false;
}

//���Ͳ���
bool CTableFrameSink::SendOperateNotify()
{
	//������ʾ
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbUserAction[i]!=WIK_NULL)
		{
			//��������
			CMD_S_OperateNotify OperateNotify;
			OperateNotify.wOperateUser = m_wTingUser;
			OperateNotify.wResumeUser=m_wResumeUser;
			OperateNotify.cbActionCard=m_cbProvideCard;
			OperateNotify.cbActionMask=m_cbUserAction[i];

			//��������
			m_pITableFrame->SendTableData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
			m_pITableFrame->SendLookonData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
		}
	}

	return true;
}

//�ɷ��齫
bool CTableFrameSink::DispatchCardData(WORD wCurrentUser)
{
	//״̬Ч��
	ASSERT(wCurrentUser!=INVALID_CHAIR);
	if (wCurrentUser==INVALID_CHAIR) return false;

	//�����齫
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		m_cbDiscardCount[m_wOutCardUser]++;
		m_cbDiscardCard[m_wOutCardUser][m_cbDiscardCount[m_wOutCardUser]-1]=m_cbOutCardData;
	}

	//��ׯ����
    if (m_cbLeftCardCount==0)
	{
		m_cbChiHuCard=0;
		m_wProvideUser=INVALID_CHAIR;
		OnEventGameEnd(m_wProvideUser,NULL,GER_NORMAL);

		return true;
	}//End if

	//���ƴ���
	if (m_bSendStatus==true)
	{
		//�����齫
		m_cbSendCardCount++;
		m_cbSendCardData=m_cbRepertoryCard[--m_cbLeftCardCount];
		m_cbCardIndex[wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

		//���ñ���
		m_wProvideUser=wCurrentUser;
		m_cbProvideCard=m_cbSendCardData;

		//�����ж�
		if (m_cbLeftCardCount>1)
		{
			tagGangCardResult GangCardResult;
			m_cbUserAction[wCurrentUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[wCurrentUser],
				m_WeaveItemArray[wCurrentUser],m_cbWeaveItemCount[wCurrentUser],GangCardResult);
		}

		//����Ȩλ
		WORD wChiHuRight=0;
		if (m_bGangStatus==true) wChiHuRight|=CHR_QIANG_GANG;

		//�����ж�
		tagChiHuResult ChiHuResult;
		m_cbUserAction[wCurrentUser]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wCurrentUser],
			m_WeaveItemArray[wCurrentUser],m_cbWeaveItemCount[wCurrentUser],0,wChiHuRight,ChiHuResult);
	}

	//���ñ���
	m_cbOutCardData=0;
	m_wCurrentUser=wCurrentUser;
	m_wOutCardUser=INVALID_CHAIR;

	//��������
	CMD_S_SendCard SendCard;
	SendCard.wCurrentUser=wCurrentUser;
	SendCard.cbActionMask=m_cbUserAction[wCurrentUser];
	SendCard.cbCardData=(m_bSendStatus==true)?m_cbSendCardData:0x00;

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

	return true;
}

//��Ӧ�ж�
bool CTableFrameSink::EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind)
{
	//��������
	bool bAroseAction=false;

	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//�����ж�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//�û�����
		if (wCenterUser==i) continue;

		//��������
		if (EstimatKind==EstimatKind_OutCard)
		{
			//�����ж�
			if (m_bEnjoinChiPeng[i]==false)
			{
				//�����ж�
				m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard);

				//�����ж�
				/*Del by doctor 20071202
				WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
				if (wEatUser==i) m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard);
				*/
			}

			//�����ж�
			if (m_cbLeftCardCount>1) 
			{
				m_cbUserAction[i]|=m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard);
			}
		}

		//�����ж�
		if (m_bEnjoinChiHu[i]==false)
		{
			//����Ȩλ
			WORD wChiHuRight=0;
			if (m_bGangStatus==true) wChiHuRight|=CHR_QIANG_GANG;
			if ((m_cbSendCardCount==1)&&(m_cbOutCardCount==1)) wChiHuRight|=CHR_DI;
			if ((m_cbSendCardCount==1)&&(m_cbOutCardCount==0)) wChiHuRight|=CHR_TIAN;

			//�Ժ��ж�
			tagChiHuResult ChiHuResult;
			BYTE cbWeaveCount=m_cbWeaveItemCount[i];
			m_cbUserAction[i]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],m_WeaveItemArray[i],cbWeaveCount,cbCenterCard,wChiHuRight,ChiHuResult);

			//�Ժ�����
			if ((m_cbUserAction[i]&WIK_CHI_HU)!=0) m_bEnjoinChiHu[i]=true;
		}

		//����ж�
		if (m_cbUserAction[i]!=WIK_NULL) bAroseAction=true;
	}

	//�������
	if (bAroseAction==true) 
	{
		//���ñ���
		m_wProvideUser=wCenterUser;
		m_cbProvideCard=cbCenterCard;
		m_wResumeUser=m_wCurrentUser;
		m_wCurrentUser=INVALID_CHAIR;

		//������ʾ
		m_wTingUser = wCenterUser;
		SendOperateNotify();

		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
