#include "StdAfx.h"
#include "TableFrameSink.h"

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
	m_wSiceCount2=MAKEWORD(1,1);
	m_wSiceCount3=MAKEWORD(1,1);
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_bCallCard,sizeof(m_bCallCard));
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_cbBatch,sizeof(m_cbBatch));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));
	ZeroMemory(m_bBatchEnd,sizeof(m_bBatchEnd));
	
	//������Ϣ
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));
	m_cbGangCount=0;
	m_BankContinueCount = 1;
	m_cbMustLeft =MUST_LEFT_COUNT;
	
	//��ʷ����
	memset(m_lAllTurnScore,0,sizeof(m_lAllTurnScore));
	memset(m_lLastTurnScore,0,sizeof(m_lLastTurnScore));
	ZeroMemory(m_lGameScoreEx,sizeof(m_lGameScoreEx));

	//������Ϣ
	FillMemory(m_cbKingCardIndex,sizeof(m_cbKingCardIndex),0xFF);
	ZeroMemory(m_cbKingCount,sizeof(m_cbKingCount));
	ZeroMemory(m_cbViceKingCount,sizeof(m_cbViceKingCount));
	ZeroMemory(m_cbOutKingScore,sizeof(m_cbOutKingScore));

	//���·�
	ZeroMemory(m_lUpDownScore,sizeof(m_lUpDownScore));
	m_cbUpDownKingCardData=0;

	//������Ϣ
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));

	//���б���
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//״̬����
	m_bSendStatus=false;
	m_bGangStatus=false;
	ZeroMemory(m_bEnjoinChiHu,sizeof(m_bEnjoinChiHu));

	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_wUserAction,sizeof(m_wUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_wPerformAction,sizeof(m_wPerformAction));

	//����˿�
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
	if (m_pITableFrame==NULL)	return false;

	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	return true;
}

//��λ����
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//��Ϸ����
	m_wSiceCount=MAKEWORD(1,1);
	m_wSiceCount2=MAKEWORD(1,1);
	m_wSiceCount3=MAKEWORD(1,1);
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bCallCard,sizeof(m_bCallCard));
	ZeroMemory(m_cbBatch,sizeof(m_cbBatch));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));
	ZeroMemory(m_bBatchEnd,sizeof(m_bBatchEnd));

	//��������
	//������Ϣ
	FillMemory(m_cbKingCardIndex,sizeof(m_cbKingCardIndex),0xFF);
	ZeroMemory(m_cbKingCount,sizeof(m_cbKingCount));
	ZeroMemory(m_cbViceKingCount,sizeof(m_cbViceKingCount));
	ZeroMemory(m_cbOutKingScore,sizeof(m_cbOutKingScore));

	//���·�
	ZeroMemory(m_lUpDownScore,sizeof(m_lUpDownScore));
	m_cbUpDownKingCardData=0;

	//������Ϣ
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));
	ZeroMemory(m_lGameScoreEx,sizeof(m_lGameScoreEx));
	m_cbGangCount=0;
	m_cbMustLeft =MUST_LEFT_COUNT;

	//������Ϣ
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));


	//���б���
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//״̬����
	m_bSendStatus=false;
	m_bGangStatus=false;
	ZeroMemory(m_bEnjoinChiHu,sizeof(m_bEnjoinChiHu));

	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_wUserAction,sizeof(m_wUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_wPerformAction,sizeof(m_wPerformAction));

	//����˿�
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	//������Ϣ
	m_cbChiHuCard=0;
	ZeroMemory(&m_ChiHuResult,sizeof(m_ChiHuResult));

	//��������
	m_GameLogic.SetKingCardIndex(m_cbKingCardIndex);

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
	m_pITableFrame->SetGameStatus(GS_MJ_BATCH);	
	
	//	��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_BATCH);
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_BATCH);

	return true;
}

//��Ϸ����
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
		{
			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//������Ϣ
			GameEnd.wProvideUser=m_wProvideUser;
			GameEnd.cbChiHuCard=m_cbChiHuCard;

			//���·�
			CopyMemory(GameEnd.lUpDownScore,m_lUpDownScore,sizeof(m_lUpDownScore));

			BYTE cbOverlordKing[4]={1,1,1,1};//��������
			BYTE cbHasKingCount=0;

			//�жϰ�����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if(m_cbKingCount[i]+m_cbViceKingCount[i]>0)
				{
					cbHasKingCount++;
				}
				
			}

			//�жϽ��
			if(cbHasKingCount==1)
			{
				//�Ұ������
				for (WORD i=0;i<GAME_PLAYER;i++)
				{
					if(m_cbKingCount[i]+m_cbViceKingCount[i]>0)
					{
						cbOverlordKing[i]=2;
						GameEnd.cbBaWangKing[i]=1;
					}

				}
			}
			
			//���е�
			if(cbHasKingCount<=0)
			{
				if((cbHasKingCount==0)&&(m_cbKingCount[wChairID]+m_cbViceKingCount[wChairID]>0&&cbHasKingCount==1))
				{
					if((m_ChiHuResult[wChairID].wChiHuRight&CHR_GERMAN )|| (m_ChiHuResult[wChairID].wChiHuKind&CHK_GERMAN_SERVEN))
					{

						m_ChiHuResult[wChairID].wChiHuRight |=CHR_GERMAN_GERMAN;
					}
				}
			}

			//�жϰ��� ���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if(m_cbKingCount[i]+m_cbViceKingCount[i]>0)
				{
					m_cbOutKingScore[i]=(2*m_cbKingCount[i]+m_cbViceKingCount[i]);
					if(m_cbOutKingScore[i]>=5)
					{
						GameEnd.cbChongguan[i]=m_cbOutKingScore[i]-3;
						m_cbOutKingScore[i] =(m_cbOutKingScore[i])*(m_cbOutKingScore[i]-3)*cbOverlordKing[i];
					}
					else
						m_cbOutKingScore[i] *=cbOverlordKing[i];

					//���� �ܳ��
					WORD wProviderUser=INVALID_CHAIR;
					bool bGangChonguan=false;
					if( 2*m_cbKingCount[i]+m_cbViceKingCount[i]>=5)
					{
						for (BYTE t=0;t<m_cbWeaveItemCount[i];t++)
						{
							if((m_WeaveItemArray[i][t].wWeaveKind&(WIK_PENG|WIK_GANG)))
							{
								if(m_WeaveItemArray[i][t].wWeaveKind&(WIK_GANG)) bGangChonguan=true;
								if(m_WeaveItemArray[i][t].cbCenterCard==m_GameLogic.SwitchToCardData(m_cbKingCardIndex[0]))
								{
									if(m_WeaveItemArray[i][t].wProvideUser!=i)
										wProviderUser=m_WeaveItemArray[i][t].wProvideUser;
									if(wProviderUser!=INVALID_CHAIR)//�����Լ� ���������
										break;
								}
								if(m_WeaveItemArray[i][t].cbCenterCard==m_GameLogic.SwitchToCardData(m_cbKingCardIndex[1]))
								{
									if(m_WeaveItemArray[i][t].wProvideUser!=i)
										wProviderUser=m_WeaveItemArray[i][t].wProvideUser;
									if(wProviderUser!=INVALID_CHAIR)//�����Լ� ���������
										break;
								}
							}
						}
					}
					//û�г��
					if(wProviderUser==INVALID_CHAIR)
					{
						//����
						for(WORD j=0;(j<GAME_PLAYER);j++)
						{
							if(i!=j)
							{
								m_lGameScoreEx[i] += m_cbOutKingScore[i]*m_pGameServiceOption->lCellScore*m_BankContinueCount;
								m_lGameScoreEx[j] -= m_cbOutKingScore[i]*m_pGameServiceOption->lCellScore*m_BankContinueCount;
							}
						}

					}
					else//�г��
					{
                        
						m_lGameScoreEx[i] += 3*m_cbOutKingScore[i]*m_pGameServiceOption->lCellScore*m_BankContinueCount;
						m_lGameScoreEx[wProviderUser] -= 3*m_cbOutKingScore[i]*m_pGameServiceOption->lCellScore*m_BankContinueCount;

						//���ܾ�
						if(bGangChonguan)
						{
							for (WORD j=0;j<GAME_PLAYER;j++)
							{
								if(j==i) continue;
								if(j==wProviderUser) continue;
								m_lGameScoreEx[wProviderUser] -=10*m_pGameServiceOption->lLessScore*m_BankContinueCount;
								m_lGameScoreEx[j] +=10*m_pGameServiceOption->lLessScore*m_BankContinueCount;
							}
						}
					}

				}		

			}


			//ͳ�ƻ���
			if (wChairID!=INVALID_CHAIR)
			{
				if(m_ChiHuResult[wChairID].wChiHuRight&(CHR_TIAN|CHR_DI))
				{
					if(m_ChiHuResult[wChairID].wChiHuRight&(CHR_TIAN))
					{
						m_ChiHuResult[wChairID].wChiHuRight =m_ChiHuResult[wChairID].wChiHuRight&(CHR_KING_WAIT|CHR_TIAN);
						m_ChiHuResult[wChairID].wChiHuKind=CHK_JI_HU;
					}
					else
					{
						m_ChiHuResult[wChairID].wChiHuRight =m_ChiHuResult[wChairID].wChiHuRight&(CHR_KING_WAIT|CHR_DI);
						m_ChiHuResult[wChairID].wChiHuKind=CHK_JI_HU;

					}
					LONG lScore=0L;
					LONG lTimes=0L;
					//��������
					WORD wChiHuOrder=m_GameLogic.CalScore(m_ChiHuResult[wChairID],lScore,lTimes);

					//ѭ���ۼ�
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						//Ӯ�ҹ���
						if (m_ChiHuResult[i].wChiHuKind!=CHK_NULL)	continue;	
					
						int t=max(m_BankContinueCount,1);


						//�������
						GameEnd.lGameScore[i]-=t*m_pGameServiceOption->lCellScore*(wChiHuOrder);
						GameEnd.lGameScore[wChairID]+=t*m_pGameServiceOption->lCellScore*(wChiHuOrder);

					}

				}
				else
				{
					//��������
					if ((m_ChiHuResult[wChairID].wChiHuKind!=CHK_NULL) &&(wChairID == m_wProvideUser))
					{


						LONG lScore=0L;
						LONG lTimes=0L;
						//��������
						WORD wChiHuOrder=m_GameLogic.CalScore(m_ChiHuResult[wChairID],lScore,lTimes);

						//ѭ���ۼ�
						for (WORD i=0;i<m_wPlayerCount;i++)
						{
							//Ӯ�ҹ���
							if (m_ChiHuResult[i].wChiHuKind!=CHK_NULL)	continue;	


							if(m_wBankerUser == m_wProvideUser)//ׯ�Һ���
							{
								int t=max(m_BankContinueCount,1);


								//�мҺ���
								//1��	������
								//	�����мҳ�=����*2  �¹�*2  �߶�*2  *2 +5=21
								//	ׯ�ҳ�=ׯ��*2  ����*2  �¹�*2  �߶�*2  *2 +5=37

								//�¹��߶�
								if(m_ChiHuResult[wChairID].wChiHuKind&CHK_GERMAN_SERVEN)
								{
									//�������
									GameEnd.lGameScore[i]-=t*m_pGameServiceOption->lCellScore*(2*wChiHuOrder*2+5);
									GameEnd.lGameScore[m_wProvideUser]+=t*m_pGameServiceOption->lCellScore*(2*wChiHuOrder*2+5);
								}

								else if(m_ChiHuResult[wChairID].wChiHuRight&CHR_GERMAN)
								{
									//�������
									GameEnd.lGameScore[i]-=t*m_pGameServiceOption->lCellScore*(2*wChiHuOrder+5);
									GameEnd.lGameScore[m_wProvideUser]+=t*m_pGameServiceOption->lCellScore*(2*wChiHuOrder+5);
								}							
								else
								{
									//�������
									GameEnd.lGameScore[i]-=2*t*m_pGameServiceOption->lCellScore*(wChiHuOrder);
									GameEnd.lGameScore[m_wProvideUser]+=2*t*m_pGameServiceOption->lCellScore*(wChiHuOrder);

								}
							} 
							else   //�¼Һ���
							{
								int t=max(m_BankContinueCount,1);
								//�������
								if(i==m_wBankerUser) // ׯ�����
								{
									//			ׯ�Һ���
									//1.	����
									//	ׯ�ҳ�=ׯ��*2  ����*2  �¹�*2  �߶�*2  *2 +5=37


									//�¹��߶�
									if(m_ChiHuResult[wChairID].wChiHuKind&CHK_GERMAN_SERVEN)
									{
										GameEnd.lGameScore[i]-=t*m_pGameServiceOption->lCellScore*(2*wChiHuOrder*2+5);
										GameEnd.lGameScore[m_wProvideUser]+=t*m_pGameServiceOption->lCellScore*(2*wChiHuOrder*2+5);
									}

									else if(m_ChiHuResult[wChairID].wChiHuRight&CHR_GERMAN)
									{
										//�������
										GameEnd.lGameScore[i]-=t*m_pGameServiceOption->lCellScore*(2*wChiHuOrder+5);
										GameEnd.lGameScore[m_wProvideUser]+=t*m_pGameServiceOption->lCellScore*(2*wChiHuOrder+5);
									}		
									else
									{
										GameEnd.lGameScore[i]-=2*t*m_pGameServiceOption->lCellScore*wChiHuOrder;
										GameEnd.lGameScore[m_wProvideUser]+=2*t*m_pGameServiceOption->lCellScore*wChiHuOrder;

									}
								}
								else				//�¼����	
								{
									//�¹��߶�
									if(m_ChiHuResult[wChairID].wChiHuKind&CHK_GERMAN_SERVEN)
									{
										GameEnd.lGameScore[i]-=t*m_pGameServiceOption->lCellScore*(wChiHuOrder+5);
										GameEnd.lGameScore[m_wProvideUser]+=t*m_pGameServiceOption->lCellScore*(wChiHuOrder+5);
									}
									else if(m_ChiHuResult[wChairID].wChiHuRight&CHR_GERMAN_GERMAN)
									{
										//�������
										GameEnd.lGameScore[i]-=t*m_pGameServiceOption->lCellScore*(wChiHuOrder+5);
										GameEnd.lGameScore[m_wProvideUser]+=t*m_pGameServiceOption->lCellScore*(wChiHuOrder+5);
									}
									else if(m_ChiHuResult[wChairID].wChiHuRight&CHR_GERMAN)
									{
										//�������
										GameEnd.lGameScore[i]-=t*m_pGameServiceOption->lCellScore*(wChiHuOrder+5);
										GameEnd.lGameScore[m_wProvideUser]+=t*m_pGameServiceOption->lCellScore*(wChiHuOrder+5);
									}		
									else
									{
										GameEnd.lGameScore[i]-=t*m_pGameServiceOption->lCellScore*wChiHuOrder;
										GameEnd.lGameScore[m_wProvideUser]+=t*m_pGameServiceOption->lCellScore*wChiHuOrder;

									}
								}
							}

						}

						//�ж��Ƿ���ׯ
						if(wChairID == m_wBankerUser)
						{
							m_BankContinueCount++;
						}
						else
						{				
							m_BankContinueCount=max(m_BankContinueCount-1,1);
						}
						//ׯ������
						m_wBankerUser=m_wProvideUser;

					}

					//׽������
					if ((m_ChiHuResult[wChairID].wChiHuKind!=CHK_NULL) &&(wChairID != m_wProvideUser))
					{		

						LONG lScore=0L;
						LONG lTimes=0L;

						//��������
						WORD wChiHuOrder=m_GameLogic.CalScore(m_ChiHuResult[wChairID],lScore,lTimes);

						//ѭ���ۼ�
						for (WORD i=0;i<m_wPlayerCount;i++)
						{
							//���˺���
							if(m_ChiHuResult[i].wChiHuKind!=CHK_NULL) continue;

							WORD wTempChihuOrder=wChiHuOrder;
							if(i==m_wProvideUser) wTempChihuOrder *= 2;

							if(wChairID==m_wBankerUser) //ׯ�Һ���
							{
								int t=max(m_BankContinueCount,1);


								//�¹��߶�
								if(m_ChiHuResult[wChairID].wChiHuKind&CHK_GERMAN_SERVEN)
								{
									//��Ӯ���
									if(i==m_wProvideUser)
									{

										GameEnd.lGameScore[i]-=t*m_pGameServiceOption->lCellScore*(2*wTempChihuOrder*2+5);
										GameEnd.lGameScore[wChairID] +=t*m_pGameServiceOption->lCellScore*(2*wTempChihuOrder*2+5);
									}
									else
									{
										GameEnd.lGameScore[i]-=t*m_pGameServiceOption->lCellScore*2*wTempChihuOrder*2;
										GameEnd.lGameScore[wChairID] +=t*m_pGameServiceOption->lCellScore*2*wTempChihuOrder*2;

									}

								}
								else if(m_ChiHuResult[wChairID].wChiHuRight&CHR_GERMAN)
								{
									if(i==m_wProvideUser)
									{
										GameEnd.lGameScore[i]-=t*m_pGameServiceOption->lCellScore*(2*wTempChihuOrder+5);
										GameEnd.lGameScore[wChairID]+=t*m_pGameServiceOption->lCellScore*(2*wTempChihuOrder+5);
									}
									else
									{
										GameEnd.lGameScore[i]-=t*m_pGameServiceOption->lCellScore*2*wTempChihuOrder;
										GameEnd.lGameScore[wChairID]+=t*m_pGameServiceOption->lCellScore*2*wTempChihuOrder;

									}

								}		
								else
								{

									//��Ӯ���
									GameEnd.lGameScore[i]-=2*t*m_pGameServiceOption->lCellScore*wTempChihuOrder;
									GameEnd.lGameScore[wChairID] +=2*t*m_pGameServiceOption->lCellScore*wTempChihuOrder;
								}
							}
							else  //�¼Һ���
							{
								if(i==m_wBankerUser) //ׯ�����
								{
									int t=max(m_BankContinueCount,1);

									//�¹��߶�
									if(m_ChiHuResult[wChairID].wChiHuKind&CHK_GERMAN_SERVEN)
									{
										//��Ӯ���
										if(i==m_wProvideUser)
										{
											GameEnd.lGameScore[i]-=t*m_pGameServiceOption->lCellScore*(2*wTempChihuOrder*2+5);
											GameEnd.lGameScore[wChairID] +=t*m_pGameServiceOption->lCellScore*(2*wTempChihuOrder*2+5);
										}
										else
										{
											GameEnd.lGameScore[i]-=2*t*m_pGameServiceOption->lCellScore*wTempChihuOrder*2;
											GameEnd.lGameScore[wChairID] +=2*t*m_pGameServiceOption->lCellScore*wTempChihuOrder*2;

										}

									}

									else if(m_ChiHuResult[wChairID].wChiHuRight&CHR_GERMAN)
									{
										//�������
										if(i==m_wProvideUser)
										{
											GameEnd.lGameScore[i]-=t*m_pGameServiceOption->lCellScore*(2*wTempChihuOrder+5);
											GameEnd.lGameScore[wChairID]+=t*m_pGameServiceOption->lCellScore*(2*wTempChihuOrder+5);
										}
										else
										{
											GameEnd.lGameScore[i]-=t*m_pGameServiceOption->lCellScore*(2*wTempChihuOrder);
											GameEnd.lGameScore[wChairID]+=t*m_pGameServiceOption->lCellScore*(2*wTempChihuOrder);

										}
									}		
									else
									{

										//��Ӯ���
										GameEnd.lGameScore[i]-=2*t*m_pGameServiceOption->lCellScore*wTempChihuOrder;
										GameEnd.lGameScore[wChairID] +=2*t*m_pGameServiceOption->lCellScore*wTempChihuOrder;
									}
								}
								else				//�¼����
								{
									//��Ӯ���
									//��¥
									int t=max(m_BankContinueCount,1);

									//�¹��߶�
									if(m_ChiHuResult[wChairID].wChiHuKind&CHK_GERMAN_SERVEN)
									{
										//��Ӯ���
										if(i==m_wProvideUser)
										{
											GameEnd.lGameScore[i]-=t*m_pGameServiceOption->lCellScore*(wTempChihuOrder*2+5);
											GameEnd.lGameScore[wChairID] +=t*m_pGameServiceOption->lCellScore*(wTempChihuOrder*2+5);
										}
										else
										{
											GameEnd.lGameScore[i]-=t*m_pGameServiceOption->lCellScore*(wTempChihuOrder);
											GameEnd.lGameScore[wChairID] +=t*m_pGameServiceOption->lCellScore*(wTempChihuOrder);

										}
									}

									//�¹��߶�
									else if(m_ChiHuResult[wChairID].wChiHuRight&CHR_GERMAN)
									{
										//��Ӯ���
										if(i==m_wProvideUser)
										{
											GameEnd.lGameScore[i]-=t*m_pGameServiceOption->lCellScore*(wTempChihuOrder+5);
											GameEnd.lGameScore[wChairID] +=t*m_pGameServiceOption->lCellScore*(wTempChihuOrder+5);
										}
										else
										{
											GameEnd.lGameScore[i]-=t*m_pGameServiceOption->lCellScore*(wTempChihuOrder);
											GameEnd.lGameScore[wChairID] +=t*m_pGameServiceOption->lCellScore*(wTempChihuOrder);

										}

									}
									else
									{

										//��Ӯ���
										GameEnd.lGameScore[i]-=t*m_pGameServiceOption->lCellScore*wTempChihuOrder;
										GameEnd.lGameScore[wChairID] +=t*m_pGameServiceOption->lCellScore*wTempChihuOrder;
									}
								}
							}
						}

						//����ȫ��
						if(m_ChiHuResult[wChairID].wChiHuRight&CHR_QIANG_GANG)
						{


							for (WORD j=0;j<GAME_PLAYER;j++)
							{
								if(j==m_wProvideUser) continue;
								if(j==wChairID) continue;

								//����Ǯ
								GameEnd.lGameScore[j]+=GameEnd.lGameScore[i];
								GameEnd.lGameScore[i]+=0;
							}
						}

						//�ж��Ƿ���ׯ
						if(wChairID == m_wBankerUser)
						{
							m_BankContinueCount++;
						}
						else
						{
							m_BankContinueCount=max(m_BankContinueCount-1,1);
							//ׯ������
							m_wBankerUser=(m_wBankerUser+1)%GAME_PLAYER;

						}

					}	

				}
			}
			else
			{
				//��ׯ����
				GameEnd.cbChiHuCard=0;
				m_wBankerUser=(m_wBankerUser+1)%GAME_PLAYER;
			}


			//���Ƶ÷�
			CopyMemory(GameEnd.lGameHuScore,GameEnd.lGameScore,sizeof(GameEnd.lGameHuScore));

			//������
			CopyMemory(GameEnd.lGameScoreEx,m_lGameScoreEx,sizeof(m_lGameScoreEx));

			//������Ϣ
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.dwChiHuKind[i]=m_ChiHuResult[i].wChiHuKind;
				GameEnd.dwChiHuRight[i]=m_ChiHuResult[i].wChiHuRight;
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i],MAX_COUNT);
			}
			//���ճɼ�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.lGameScore[i] += m_lGameScoreEx[i]+m_lUpDownScore[i];
			}
			

			//��ҷ���
			if(m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)
			{

				//���ֹ���
				LONG lDifference = 0;
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
					const tagUserScore * pUserScore=pIServerUserItem->GetUserScore();
					if((pUserScore->lScore < abs(GameEnd.lGameScore[i]))&&(GameEnd.lGameScore[i]<0)&&(pUserScore->lScore >0))
					{
						lDifference += abs(GameEnd.lGameScore[i])-pUserScore->lScore;
						GameEnd.lGameScore[i] =-pUserScore->lScore;
					}
				}
				
				BYTE cbWinCount=0;
				//���ֵ���
				for (WORD i=0; i<m_wPlayerCount; i++)
				{
					if(GameEnd.lGameScore[i] > 0)
					{
						cbWinCount++;
					}
				}
				//���ֵ���
				for (WORD i=0; i<m_wPlayerCount; i++)
				{
					if(GameEnd.lGameScore[i] > 0&&cbWinCount>0)
					{
						GameEnd.lGameScore[i] -= lDifference/cbWinCount;
					}
				}
			}

			//��ʷ����
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				m_lAllTurnScore[i]+=GameEnd.lGameScore[i];
				m_lLastTurnScore[i]=GameEnd.lGameScore[i];
				GameEnd.lAllTurnScore[i]=m_lAllTurnScore[i];
				GameEnd.lLastTurnScore[i]=m_lLastTurnScore[i];
			}
			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//�޸Ļ���
			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
	
				//���û���
				ScoreInfoArray[i].lScore=GameEnd.lGameScore[i];
				if (GameEnd.lGameScore[i]>0L)
				{
					GameEnd.lGameScore[i]=ScoreInfoArray[i].lScore-GameEnd.lGameScore[i]*m_pGameServiceOption->wRevenue/100L;
					ScoreInfoArray[i].lRevenue=GameEnd.lGameScore[i]*m_pGameServiceOption->wRevenue/100L;
				}

				//����ʤ��
				if (ScoreInfoArray[i].lScore==0L) ScoreInfoArray[i].ScoreKind=enScoreKind_Draw;
				else ScoreInfoArray[i].ScoreKind=(ScoreInfoArray[i].lScore>0L)?enScoreKind_Win:enScoreKind_Lost;
			}
			//ͳ�ƻ���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//д�����
				m_pITableFrame->WriteUserScore(i,ScoreInfoArray[i].lScore,ScoreInfoArray[i].lRevenue,ScoreInfoArray[i].ScoreKind);
			}

			//������Ϸ
			m_pITableFrame->ConcludeGame();
			return true;
		}
	case GER_USER_LEFT:		//�û�ǿ��
		{
			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//֪ͨ��Ϣ
			TCHAR szMessage[128]=TEXT("");
			_snprintf(szMessage,CountArray(szMessage),TEXT("���� [ %s ] �뿪��Ϸ����Ϸ����"),pIServerUserItem->GetAccounts());
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				IServerUserItem * pISendUserItem=m_pITableFrame->GetServerUserItem(i);
				if (pISendUserItem!=NULL) 
					m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_INFO);
			}

			WORD wIndex=0;
			do
			{
				IServerUserItem * pISendUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
				if (pISendUserItem==NULL)
					break;
				m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_INFO);
			} while (true);


			//���ñ���
			GameEnd.cbChiHuCard = 255;
			GameEnd.wProvideUser=INVALID_CHAIR;
			GameEnd.lGameScore[wChairID] -=6*m_BankContinueCount*m_pGameServiceOption->lCellScore;

			BYTE cbOverlordKing[4]={1,1,1,1};//��������
			BYTE cbHasKingCount=0;

			//�жϰ�����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if(m_cbKingCount[i]+m_cbViceKingCount[i]>0)
				{
					cbHasKingCount++;
				}

			}

			//�жϽ��
			if(cbHasKingCount==1)
			{
				//�Ұ������
				for (WORD i=0;i<GAME_PLAYER;i++)
				{
					if(m_cbKingCount[i]+m_cbViceKingCount[i]>0)
					{
						cbOverlordKing[i]=2;
						GameEnd.cbBaWangKing[i]=1;

					}

				}
			}

			//�жϰ��� ���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if(m_cbKingCount[i]+m_cbViceKingCount[i]>0)
				{
					m_cbOutKingScore[i]=(2*m_cbKingCount[i]+m_cbViceKingCount[i]);
					if(m_cbOutKingScore[i]>=5)
					{
						GameEnd.cbChongguan[i]=m_cbOutKingScore[i]-3;
						m_cbOutKingScore[i] =(m_cbOutKingScore[i])*(m_cbOutKingScore[i]-3)*cbOverlordKing[i];
					}
					else
						m_cbOutKingScore[i] *=cbOverlordKing[i];

					//���� �ܳ��
					WORD wProviderUser=INVALID_CHAIR;
					for (BYTE t=0;t<m_cbWeaveItemCount[i];t++)
					{
						if(m_WeaveItemArray[i][t].wWeaveKind&(WIK_PENG|WIK_GANG))
						{
							if(m_WeaveItemArray[i][t].cbCenterCard==m_GameLogic.SwitchToCardData(m_cbKingCardIndex[0]))
							{
								if(wProviderUser!=i)
									wProviderUser=m_WeaveItemArray[i][t].wProvideUser;
								if(wProviderUser!=INVALID_CHAIR)//�����Լ� ���������
									break;
							}
							if(m_WeaveItemArray[i][t].cbCenterCard==m_GameLogic.SwitchToCardData(m_cbKingCardIndex[1]))
							{
								if(wProviderUser!=i)
									wProviderUser=m_WeaveItemArray[i][t].wProvideUser;
								if(wProviderUser!=INVALID_CHAIR)//�����Լ� ���������
									break;
							}
						}
					}
					//û�г��
					if(wProviderUser==INVALID_CHAIR)
					{
						//����
						for(WORD j=0;(j<GAME_PLAYER);j++)
						{
							if(i!=j)
							{
								m_lGameScoreEx[i] += m_cbOutKingScore[i]*m_pGameServiceOption->lCellScore*m_BankContinueCount;
								m_lGameScoreEx[j] -= m_cbOutKingScore[i]*m_pGameServiceOption->lCellScore*m_BankContinueCount;
							}
						}

					}
					else//�г��
					{

						m_lGameScoreEx[i] += 3*m_cbOutKingScore[i]*m_pGameServiceOption->lCellScore*m_BankContinueCount;
						m_lGameScoreEx[wProviderUser] -= 3*m_cbOutKingScore[i]*m_pGameServiceOption->lCellScore*m_BankContinueCount;

						//���ܾ�
						for (WORD j=0;j<GAME_PLAYER;j++)
						{
							if(j==i) continue;
							if(j==wProviderUser) continue;
							m_lGameScoreEx[wProviderUser] -=10*m_pGameServiceOption->lLessScore*m_BankContinueCount;
							m_lGameScoreEx[j] +=10*m_pGameServiceOption->lLessScore*m_BankContinueCount;
						}
					}

				}		

			}




			//���Ʒ�
			CopyMemory(GameEnd.lGameHuScore,GameEnd.lGameScore,sizeof(GameEnd.lGameHuScore));

			//�ܷ�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.lGameScore[i] += m_lGameScoreEx[i]+m_lUpDownScore[i];
			}
			
			//��ҷ���
			if(m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)
			{

				//���ֹ���
				LONG lDifference = 0;
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
					const tagUserScore * pUserScore=pIServerUserItem->GetUserScore();
					if((pUserScore->lScore < abs(GameEnd.lGameScore[i]))&&(GameEnd.lGameScore[i]<0)&&(pUserScore->lScore >0))
					{
						lDifference += abs(GameEnd.lGameScore[i])-pUserScore->lScore;
						GameEnd.lGameScore[i] =-pUserScore->lScore;
					}
				}

				BYTE cbWinCount=0;
				//���ֵ���
				for (WORD i=0; i<m_wPlayerCount; i++)
				{
					if(GameEnd.lGameScore[i] > 0)
					{
						cbWinCount++;
					}
				}
				//���ֵ���
				for (WORD i=0; i<m_wPlayerCount; i++)
				{
					if(GameEnd.lGameScore[i] > 0&&cbWinCount>0)
					{
						GameEnd.lGameScore[i] -= lDifference/cbWinCount;
					}
				}
			}
			//��ʷ����
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				m_lAllTurnScore[i]+=GameEnd.lGameScore[i];
				m_lLastTurnScore[i]=GameEnd.lGameScore[i];
				GameEnd.lAllTurnScore[i]=m_lAllTurnScore[i];
				GameEnd.lLastTurnScore[i]=m_lLastTurnScore[i];
			}
			CopyMemory(GameEnd.lUpDownScore,m_lUpDownScore,sizeof(m_lUpDownScore));
			CopyMemory(GameEnd.lGameScoreEx,m_lGameScoreEx,sizeof(m_lGameScoreEx));
	
			//������Ϣ
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i],MAX_COUNT);
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//�޸Ļ���
			tagScoreInfo ScoreInfo;
			ScoreInfo.ScoreKind=enScoreKind_Flee;
			ScoreInfo.lScore=GameEnd.lGameScore[wChairID];
			//д�����
			m_pITableFrame->WriteUserScore(wChairID,ScoreInfo.lScore,0L,ScoreInfo.ScoreKind);


			//������Ϸ
			m_pITableFrame->ConcludeGame();

			return true;
		}
	case GER_DISMISS:		//��Ϸ��ɢ
		{
			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//���ñ���
			GameEnd.wProvideUser=INVALID_CHAIR;

			//�����˿�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i],MAX_COUNT);
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

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
			StatusFree.wBankerUser=m_wBankerUser;
			StatusFree.lCellScore=m_pGameServiceOption->lCellScore*m_BankContinueCount;
			CopyMemory(StatusFree.bTrustee,m_bTrustee,sizeof(m_bTrustee));


			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_MJ_BATCH:	//��Ϸ����
		{
			//��������
			CMD_S_StatusBatch StatusChip;
			ZeroMemory(&StatusChip,sizeof(StatusChip));

			//��������
			StatusChip.bComplete=(m_bCallCard[wChiarID]==true)?TRUE:FALSE;
			StatusChip.lCellScore=m_pGameServiceOption->lCellScore*m_BankContinueCount;
			CopyMemory(StatusChip.bTrustee,m_bTrustee,sizeof(m_bTrustee));
			//��ʷ����
			CopyMemory(StatusChip.lAllTurnScore,m_lAllTurnScore,sizeof(m_lAllTurnScore));
			CopyMemory(StatusChip.lLastTurnScore,m_lLastTurnScore,sizeof(m_lLastTurnScore));


			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusChip,sizeof(StatusChip));


		}
	case GS_MJ_PLAY:	//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//��Ϸ����
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.lCellScore=m_pGameServiceOption->lCellScore;
			StatusPlay.wSiceCount=m_wSiceCount;
			StatusPlay.wSiceCount2=m_wSiceCount2;
			StatusPlay.wSiceCount3=m_wSiceCount3;
			CopyMemory(StatusPlay.cbChip,m_cbBatch,sizeof(m_cbBatch));
			CopyMemory(StatusPlay.bTrustee,m_bTrustee,sizeof(m_bTrustee));

			//���·�
			CopyMemory(StatusPlay.lUpDownScore,m_lUpDownScore,sizeof(m_lUpDownScore));
			StatusPlay.cbUpDownKingCardData=m_cbUpDownKingCardData;
			StatusPlay.cbFloor=max(m_BankContinueCount,1);


			//״̬����
			StatusPlay.cbActionCard=m_cbProvideCard;
			StatusPlay.cbLeftCardCount=m_cbLeftCardCount;
			StatusPlay.wActionMask=(m_bResponse[wChiarID]==false)?m_wUserAction[wChiarID]:WIK_NULL;

			//��ʷ��¼
			StatusPlay.wOutCardUser=m_wOutCardUser;
			StatusPlay.cbOutCardData=m_cbOutCardData;
			CopyMemory(StatusPlay.cbDiscardCard,m_cbDiscardCard,sizeof(StatusPlay.cbDiscardCard));
			CopyMemory(StatusPlay.cbDiscardCount,m_cbDiscardCount,sizeof(StatusPlay.cbDiscardCount));
			//��ʷ����
			CopyMemory(StatusPlay.lAllTurnScore,m_lAllTurnScore,sizeof(m_lAllTurnScore));
			CopyMemory(StatusPlay.lLastTurnScore,m_lLastTurnScore,sizeof(m_lLastTurnScore));
			CopyMemory(StatusPlay.lUpDownScore,m_lUpDownScore,sizeof(m_lUpDownScore));

			//����˿�
			CopyMemory(StatusPlay.WeaveItemArray,m_WeaveItemArray,sizeof(m_WeaveItemArray));
			CopyMemory(StatusPlay.cbWeaveCount,m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

			//�˿�����
			CopyMemory(StatusPlay.cbKingCardIndex,m_cbKingCardIndex,sizeof(m_cbKingCardIndex));
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				StatusPlay.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],StatusPlay.cbCardData[i],MAX_COUNT);
				StatusPlay.cbSendCardData=((m_cbSendCardData!=0)&&(m_wProvideUser==wChiarID))?m_cbSendCardData:0x00;
			}


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
	switch (wSubCmdID)
	{
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
			return OnUserOperateCard(pUserData->wChairID,pOperateCard->wOperateCode,pOperateCard->cbOperateCard);
		}
	case SUB_C_BATCH:			//ϴ�����
		{
			//Ч����Ϣ
			ASSERT(wDataSize==sizeof(CMD_C_BATCH));
			if (wDataSize!=sizeof(CMD_C_BATCH)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			CMD_C_BATCH * pChip=(CMD_C_BATCH *)pDataBuffer;
			return OnUserCallFinish(pUserData->wChairID,pChip->cbChipTimes);
		}
	case SUB_C_TRUSTEE:
		{
			CMD_C_Trustee *pTrustee =(CMD_C_Trustee *)pDataBuffer;
			if(wDataSize != sizeof(CMD_C_Trustee)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();

			m_bTrustee[pUserData->wChairID]=pTrustee->bTrustee;
			CMD_S_Trustee Trustee;
			Trustee.bTrustee=pTrustee->bTrustee;
			Trustee.wChairID = pUserData->wChairID;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));

			return true;
		}
	case SUB_C_BATCH_END:			//ϴ�����
		{
			//Ч����Ϣ
			ASSERT(wDataSize==0);
			if (wDataSize!=0) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			return OnUserBatchEnd(pUserData->wChairID);
		}
	case SUB_C_PASS_KING:		//��������
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY)
			{
				ASSERT(FALSE);
				return true;
			}

			m_cbLeftCardCount--;
			if(m_cbLeftCardCount<m_cbMustLeft-1)
			{
				m_cbChiHuCard=0;
				m_wProvideUser=INVALID_CHAIR;
				OnEventGameEnd(INVALID_CHAIR,NULL,GER_NORMAL);
				return true;
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
	//ׯ������
	if ((bLookonUser==false)&&(m_wBankerUser==INVALID_CHAIR))
	{
		m_wBankerUser=pIServerUserItem->GetChairID();
	}

	return true;
}

//�û�����
bool __cdecl CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if(bLookonUser==false)
	{
		m_BankContinueCount=1;
	}
	ZeroMemory(m_lAllTurnScore,sizeof(m_lAllTurnScore));
	ZeroMemory(m_lLastTurnScore,sizeof(m_lLastTurnScore));
	//ׯ������
	if ((bLookonUser==false)&&(wChairID==m_wBankerUser))
	{
		m_wBankerUser=INVALID_CHAIR;

		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if ((i!=wChairID)&&(m_pITableFrame->GetServerUserItem(i)!=NULL))
			{
				m_wBankerUser=i;
				break;
			}
		}
	}

	return true;
}



//�û�����
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData)
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

	//�������
	ASSERT(wChairID==m_wCurrentUser);
	ASSERT(m_GameLogic.IsValidCard(cbCardData)==true);

	//Ч�����
	if (wChairID!=m_wCurrentUser) return false;
	if (m_GameLogic.IsValidCard(cbCardData)==false) return false;

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],cbCardData)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//���ñ���
	m_bSendStatus=true;	
	m_bEnjoinChiHu[wChairID]=true;
	for (WORD i =0;i<m_wPlayerCount;i++)
	{
		if (i==wChairID) continue;
		else
			m_bEnjoinChiHu[i] = false;
	}
	m_wUserAction[wChairID]=WIK_NULL;
	m_wPerformAction[wChairID]=WIK_NULL;
	m_bGangStatus=false;

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

	//������
	/*if (m_bGangStatus==true)
	{
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if ((m_wUserAction[i]&WIK_CHI_HU)!=0) break;
		}
		if (i==m_wPlayerCount) 
		{
			m_bGangStatus=false;
		}
	}*/

	//�ɷ��˿�
	if (bAroseAction==false) DispatchCardData(m_wCurrentUser);

	return true;
}

//�û�����
bool CTableFrameSink::OnUserOperateCard(WORD wChairID, WORD wOperateCode, BYTE cbOperateCard)
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return false;

	//Ч���û�
	ASSERT((wChairID==m_wCurrentUser)||(m_wCurrentUser==INVALID_CHAIR));
	if ((wChairID!=m_wCurrentUser)&&(m_wCurrentUser!=INVALID_CHAIR)) return true;

	//��������
	if (m_wCurrentUser==INVALID_CHAIR)
	{
		//Ч��״̬
		ASSERT(m_bResponse[wChairID]==false);
		ASSERT((wOperateCode==WIK_NULL)||((m_wUserAction[wChairID]&wOperateCode)!=0));

		//Ч��״̬
		if (m_bResponse[wChairID]==true)  	return true;
		if ((wOperateCode!=WIK_NULL)&&((m_wUserAction[wChairID]&wOperateCode)==0))	return true;

		//��������
		WORD wTargetUser=wChairID;
		WORD wTargetAction=wOperateCode;

		//���ñ���
		m_bResponse[wChairID]=true;
		m_wPerformAction[wChairID]=wOperateCode;
		m_cbOperateCard[wChairID]=(cbOperateCard==0)?m_cbProvideCard:cbOperateCard;

		//ִ���ж�
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//��ȡ����
			WORD wUserAction=(m_bResponse[i]==false)?m_wUserAction[i]:m_wPerformAction[i];

			//���ȼ���
			BYTE cbUserActionRank=m_GameLogic.GetUserActionRank(wUserAction);
			BYTE cbTargetActionRank=m_GameLogic.GetUserActionRank(wTargetAction);

			//�����ж�
			if (cbUserActionRank>cbTargetActionRank)
			{
				wTargetUser=i;
				wTargetAction=wUserAction;
			}
		}
		if (m_bResponse[wTargetUser]==false)	return true;

		//�Ժ��ȴ�
		if (wTargetAction==WIK_CHI_HU)
		{
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if ((m_bResponse[i]==false)&&(m_wUserAction[i]&WIK_CHI_HU))
					return true;
			}
		}
		//�Ժ�ִ���ж�
		if (wTargetAction==WIK_CHI_HU)
		{
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//�����ж�
				WORD wFirstUser=(m_wProvideUser+m_wPlayerCount-1-i)%m_wPlayerCount;
				if(m_wPerformAction[wFirstUser]&WIK_CHI_HU)
				{
					//����У��
					ASSERT(m_bResponse[wFirstUser] == true);
					if((m_bResponse[wFirstUser] == false)) return false;
					//�ҵ��׼��û� ���˳�ѭ��
					wTargetUser = wFirstUser;
					break;
				}
			}
		}
		//��������
		if (wTargetAction==WIK_NULL)
		{
			//�û�״̬
			ZeroMemory(m_bResponse,sizeof(m_bResponse));
			ZeroMemory(m_wUserAction,sizeof(m_wUserAction));
			ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
			ZeroMemory(m_wPerformAction,sizeof(m_wPerformAction));

			//�����˿�
			DispatchCardData(m_wResumeUser);

			return true;
		}



		//��������
		BYTE cbTargetCard=m_cbOperateCard[wTargetUser];

		//����������˳����ܺ�������˵� �������Լ���
		//��������
		if(m_cbKingCardIndex[0]==m_GameLogic.SwitchToCardIndex(cbTargetCard))
		{
			ASSERT(m_cbKingCount[m_wOutCardUser]>=1);
			m_cbKingCount[m_wOutCardUser] -= 1;
			m_cbKingCount[wTargetUser] += 1;
		}
		//���ٸ���
		if(m_cbKingCardIndex[1]==m_GameLogic.SwitchToCardIndex(cbTargetCard))
		{
			ASSERT(m_cbViceKingCount[m_wOutCardUser]>=1);
			m_cbViceKingCount[m_wOutCardUser] -= 1;
			m_cbViceKingCount[wTargetUser] += 1;
		}
		//���Ʊ���
		m_cbOutCardData=0;
		m_bSendStatus=true;
		m_wOutCardUser=INVALID_CHAIR;
		m_bEnjoinChiHu[wTargetUser]=false;


		//���Ʋ���
		if (wTargetAction==WIK_CHI_HU)
		{
			//������Ϣ
			m_cbChiHuCard=cbTargetCard;
			m_wProvideUser=m_wProvideUser;

			//����Ȩλ
			WORD wChiHuRight=0;
			if (m_bGangStatus==true) wChiHuRight|=CHR_QIANG_GANG;
			if ((m_cbSendCardCount==1)&&(m_cbOutCardCount==1)) wChiHuRight|=CHR_DI;

			//�����ж�
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//�����ж�
				if ((i==m_wProvideUser)||((m_wPerformAction[i]&WIK_CHI_HU)==0))
					continue;

				if(i!=wTargetUser) continue;

				//��ͨ����
				if (m_cbChiHuCard!=0)
				{
					//�����ж�
					BYTE cbWeaveItemCount=m_cbWeaveItemCount[i];
					tagWeaveItem * pWeaveItem=m_WeaveItemArray[i];
					m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],pWeaveItem,cbWeaveItemCount,m_cbChiHuCard,wChiHuRight,m_ChiHuResult[i]);

					//�����˿�
					if (m_ChiHuResult[i].wChiHuKind!=CHK_NULL) 
						m_cbCardIndex[i][m_GameLogic.SwitchToCardIndex(m_cbChiHuCard)]++;
				}
			}

			//������Ϸ
			ASSERT(m_ChiHuResult[wTargetUser].wChiHuKind!=CHK_NULL);
			OnEventGameEnd(wTargetUser,NULL,GER_NORMAL);

			return true;
		}

		//�û�״̬
		ZeroMemory(m_bResponse,sizeof(m_bResponse));
		ZeroMemory(m_wUserAction,sizeof(m_wUserAction));
		ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
		ZeroMemory(m_wPerformAction,sizeof(m_wPerformAction));

		//����˿�
		ASSERT(m_cbWeaveItemCount[wTargetUser]<MAX_WEAVE);
		WORD wIndex=m_cbWeaveItemCount[wTargetUser]++;
		m_WeaveItemArray[wTargetUser][wIndex].cbPublicCard=TRUE;
		m_WeaveItemArray[wTargetUser][wIndex].cbCenterCard=cbTargetCard;
		m_WeaveItemArray[wTargetUser][wIndex].wWeaveKind=wTargetAction;
		m_WeaveItemArray[wTargetUser][wIndex].wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;

		//ɾ���˿�
		switch (wTargetAction)
		{
		case WIK_DNBL:		//���ϱ���	
			{
				//ɾ���˿�
				BYTE cbRemoveCard[]={cbTargetCard+1,cbTargetCard+3};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

				break;

			}
		case WIK_DNBC:		//���ϱ���
			{
				//ɾ���˿�
				BYTE cbRemoveCard[]={cbTargetCard-1,cbTargetCard+2};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

				break;

			}
		case WIK_DNBR:		//���ϱ���
			{
				//ɾ���˿�
				BYTE cbRemoveCard[]={cbTargetCard-3,cbTargetCard-2};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

				break;

			}
		case WIK_DXBL:		//��������
			{
				//ɾ���˿�
				BYTE cbRemoveCard[]={cbTargetCard+2,cbTargetCard+3};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

				break;

			}
		case WIK_DXBC:		//��������
			{
				//ɾ���˿�
				BYTE cbRemoveCard[]={cbTargetCard-2,cbTargetCard+1};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

				break;

			}
		case WIK_DXBR:		//��������
			{
				//ɾ���˿�
				BYTE cbRemoveCard[]={cbTargetCard-3,cbTargetCard-1};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

				break;

			}
		case WIK_LEFT:		//���Ʋ���
			{
				//ɾ���˿�
				BYTE cbRemoveCard[]={cbTargetCard+1,cbTargetCard+2};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));
				break;
			}
		case WIK_RIGHT:		//���Ʋ���
			{
				//ɾ���˿�
				BYTE cbRemoveCard[]={cbTargetCard-2,cbTargetCard-1};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));
	
				break;
			}
		case WIK_CENTER:	//���Ʋ���
			{
				//ɾ���˿�
				BYTE cbRemoveCard[]={cbTargetCard-1,cbTargetCard+1};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

				break;
			}
		case WIK_PENG:		//���Ʋ���
			{
				//ɾ���˿�
				BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));
				break;
			}
		case WIK_GANG:		//���Ʋ���
			{
				//��������
				if ((m_cbSendCardCount==1)&&(m_cbOutCardData==0))
				{
					//ɾ���˿�
					BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard,cbTargetCard};
					m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));
					//���ƻ���
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if (i==wTargetUser) 
							m_lGameScoreEx[i]+=6*m_pGameServiceOption->lCellScore*m_BankContinueCount;
						else 
							m_lGameScoreEx[i]-=2*m_pGameServiceOption->lCellScore*m_BankContinueCount;
					}

					if(m_cbKingCardIndex[1]==m_GameLogic.SwitchToCardIndex(cbTargetCard))
					{
						//���ƻ���  ������10�ֵ� ����ǰ��ĵĸ���+2 ��������˵8��
						for (WORD i=0;i<m_wPlayerCount;i++)
						{
							if (i==wTargetUser) 
								m_lGameScoreEx[i]+=24*m_pGameServiceOption->lCellScore*m_BankContinueCount;
							else 
								m_lGameScoreEx[i]-=8*m_pGameServiceOption->lCellScore*m_BankContinueCount;
						}

					}

				}
				else
				{
					//ɾ���˿�
					BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard,cbTargetCard};
					m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

					//���ƻ���
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if (i==wTargetUser) 
							m_lGameScoreEx[i]+=6*m_pGameServiceOption->lCellScore*m_BankContinueCount;
						else 
							m_lGameScoreEx[i]-=2*m_pGameServiceOption->lCellScore*m_BankContinueCount;
					}
					if(m_cbKingCardIndex[1]==m_GameLogic.SwitchToCardIndex(cbTargetCard))
					{
						//���ƻ���
						for (WORD i=0;i<m_wPlayerCount;i++)
						{
							if (i==wTargetUser) 
								m_lGameScoreEx[i]+=24*m_pGameServiceOption->lCellScore*m_BankContinueCount;
							else 
								m_lGameScoreEx[i]-=8*m_pGameServiceOption->lCellScore*m_BankContinueCount;
						}

					}

				}
				m_cbGangCount++;
		
				break;
			}
		}

		//������
		CMD_S_OperateResult OperateResult;
		OperateResult.wOperateUser=wTargetUser;
		OperateResult.cbOperateCard=cbTargetCard;
		OperateResult.wOperateCode=wTargetAction;
		OperateResult.wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;

		//����״̬
		if (wTargetAction==WIK_GANG) 	m_bGangStatus=true;

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

		//�����û�
		m_wCurrentUser=wTargetUser;

		//���ƴ���
		if (wTargetAction==WIK_GANG)
		{
			//Ч�鶯��
			bool bAroseAction=EstimateUserRespond(wTargetUser,cbTargetCard,EstimatKind_GangCard);

			//�����˿�
			if (bAroseAction==false) 
			{
				DispatchCardData(wTargetUser,false);
			}

			return true;
		}

		//�����ж�
		if (m_cbLeftCardCount>m_cbMustLeft-1)
		{
			//�����ж�
			tagGangCardResult GangCardResult;
			m_wUserAction[m_wCurrentUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[m_wCurrentUser],
				m_WeaveItemArray[m_wCurrentUser],m_cbWeaveItemCount[m_wCurrentUser],GangCardResult);

			//�������
			if (GangCardResult.cbCardCount>0)
			{
				//���ñ���
				m_wUserAction[m_wCurrentUser]|=WIK_GANG;

				//���Ͷ���
				SendOperateNotify();
			}
		}

		return true;
	}

	//��������
	if (m_wCurrentUser==wChairID)
	{
		//Ч�����
		ASSERT((wOperateCode!=WIK_NULL)&&((m_wUserAction[wChairID]&wOperateCode)!=0));
		if ((wOperateCode==WIK_NULL)||((m_wUserAction[wChairID]&wOperateCode)==0))		return true;

		//�˿�Ч��
		ASSERT((wOperateCode==WIK_NULL)||(wOperateCode==WIK_CHI_HU)||(m_GameLogic.IsValidCard(cbOperateCard)==true));
		if ((wOperateCode!=WIK_NULL)&&(wOperateCode!=WIK_CHI_HU)&&(m_GameLogic.IsValidCard(cbOperateCard)==false)) 	return false;

		//���ñ���
		m_bSendStatus=true;
		m_bEnjoinChiHu[m_wCurrentUser]=false;
		m_wUserAction[m_wCurrentUser]=WIK_NULL;
		m_wPerformAction[m_wCurrentUser]=WIK_NULL;

		bool bPublic=false;

		//ִ�ж���
		switch (wOperateCode)
		{
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
						WORD wWeaveKind=m_WeaveItemArray[wChairID][i].wWeaveKind;
						BYTE cbCenterCard=m_WeaveItemArray[wChairID][i].cbCenterCard;
						if ((cbCenterCard==cbOperateCard)&&(wWeaveKind==WIK_PENG))
						{
							bPublic=true;
							cbWeaveIndex=i;
							break;
						}
					}

					//Ч�鶯��
					ASSERT(cbWeaveIndex!=0xFF);
					if (cbWeaveIndex==0xFF) return false;


					//����˿�
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=TRUE;
					m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
					m_WeaveItemArray[wChairID][cbWeaveIndex].wWeaveKind=wOperateCode;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard;

					//���ƻ���
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if (i==wChairID) 
							m_lGameScoreEx[i]+=6*m_pGameServiceOption->lCellScore*m_BankContinueCount;
						else 
							m_lGameScoreEx[i]-=2*m_pGameServiceOption->lCellScore*m_BankContinueCount;
					}
					if(m_cbKingCardIndex[1]==cbCardIndex)
					{
						//���ƻ���
						for (WORD i=0;i<m_wPlayerCount;i++)
						{
							if (i==wChairID) 
								m_lGameScoreEx[i]+=24*m_pGameServiceOption->lCellScore*m_BankContinueCount;
							else 
								m_lGameScoreEx[i]-=8*m_pGameServiceOption->lCellScore*m_BankContinueCount;
						}

					}
	
				}
				else
				{
					//�˿�Ч��
					ASSERT(m_cbCardIndex[wChairID][cbCardIndex]==4);
					if (m_cbCardIndex[wChairID][cbCardIndex]!=4) 	return false;

					//���ñ���
					bPublic=false;
					cbWeaveIndex=m_cbWeaveItemCount[wChairID]++;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=FALSE;
					m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
					m_WeaveItemArray[wChairID][cbWeaveIndex].wWeaveKind=wOperateCode;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard;
					//���ƻ���
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if (i==wChairID) 
							m_lGameScoreEx[i]+=6*m_pGameServiceOption->lCellScore*m_BankContinueCount;
						else 
							m_lGameScoreEx[i]-=2*m_pGameServiceOption->lCellScore*m_BankContinueCount;
					}
					if(m_cbKingCardIndex[1]==cbCardIndex)
					{
						//���ƻ���
						for (WORD i=0;i<m_wPlayerCount;i++)
						{
							if (i==wChairID) 
								m_lGameScoreEx[i]+=24*m_pGameServiceOption->lCellScore*m_BankContinueCount;
							else 
								m_lGameScoreEx[i]-=8*m_pGameServiceOption->lCellScore*m_BankContinueCount;
						}

					}

				}

				//������Ŀ
				m_cbGangCount++;

				//ɾ���˿�
				m_cbCardIndex[wChairID][cbCardIndex]=0;

				//����״̬
				if (wOperateCode==WIK_GANG) m_bGangStatus=true;

				//������
				CMD_S_OperateResult OperateResult;
				OperateResult.wOperateUser=wChairID;
				OperateResult.wProvideUser=wChairID;
				OperateResult.wOperateCode=wOperateCode;
				OperateResult.cbOperateCard=cbOperateCard;

				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

				//Ч�鶯��
				bool bAroseAction=false;
				if (bPublic==true) bAroseAction=EstimateUserRespond(wChairID,cbOperateCard,EstimatKind_GangCard);

				//�����˿�
				if (bAroseAction==false)
				{
					DispatchCardData(wChairID,false);
				}

				return true;
			}
		case WIK_CHI_HU:		//�Ժ�����
			{
				//����Ȩλ
				WORD wChiHuRight=0;
				if (m_bGangStatus==true) wChiHuRight|=CHR_QIANG_GANG;
				if ((m_cbSendCardCount==1)&&(m_cbOutCardCount==1)) wChiHuRight|=CHR_DI;
				if ((m_cbSendCardCount==1)&&(m_cbOutCardCount==0))
				{
					wChiHuRight|=CHR_TIAN;
					m_wProvideUser = m_wCurrentUser;
				}

				//������Ϣ
				m_cbChiHuCard=m_cbProvideCard;

				//��ͨ����
				BYTE cbWeaveItemCount=m_cbWeaveItemCount[wChairID];
				tagWeaveItem * pWeaveItem=m_WeaveItemArray[wChairID];
				BYTE cbTempCardIndex[MAX_INDEX];
				CopyMemory(cbTempCardIndex,m_cbCardIndex[wChairID],sizeof(cbTempCardIndex));
				if(m_GameLogic.RemoveCard(cbTempCardIndex,m_cbChiHuCard)==false)
				{
					ASSERT(FALSE);
				}

				m_GameLogic.AnalyseChiHuCard(cbTempCardIndex,pWeaveItem,cbWeaveItemCount,m_cbChiHuCard,wChiHuRight,m_ChiHuResult[wChairID],true);



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
		if (m_wUserAction[i]!=WIK_NULL)
		{
			//��������
			CMD_S_OperateNotify OperateNotify;
			OperateNotify.wResumeUser=m_wResumeUser;
			OperateNotify.cbActionCard=m_cbProvideCard;
			OperateNotify.wActionMask=m_wUserAction[i];

			//��������
			m_pITableFrame->SendTableData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
			m_pITableFrame->SendLookonData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
		}
	}

	return true;
}

//�ɷ��˿�
bool CTableFrameSink::DispatchCardData(WORD wCurrentUser,bool bNotGang)
{
	//״̬Ч��
	ASSERT(wCurrentUser!=INVALID_CHAIR);
	if (wCurrentUser==INVALID_CHAIR)	return false;

	//�����˿�
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		m_cbDiscardCount[m_wOutCardUser]++;
		m_cbDiscardCard[m_wOutCardUser][m_cbDiscardCount[m_wOutCardUser]-1]=m_cbOutCardData;
	}	

	//���ñ���
	m_cbOutCardData=0;
	m_wCurrentUser=wCurrentUser;
	m_wOutCardUser=INVALID_CHAIR;
	m_bEnjoinChiHu[wCurrentUser]=false;

	if (m_cbLeftCardCount<=m_cbMustLeft-1)
	{

		m_cbChiHuCard=0;
		m_wProvideUser=INVALID_CHAIR;
		OnEventGameEnd(m_wProvideUser,NULL,GER_NORMAL);
		return true;

	}
	//���ƴ���
	if (m_bSendStatus==true)
	{
		//�����˿�
		m_cbSendCardCount++;
		m_cbSendCardData=m_cbRepertoryCard[--m_cbLeftCardCount];


		//��������
		if(m_cbKingCardIndex[0]==m_GameLogic.SwitchToCardIndex(m_cbSendCardData))
		{
			m_cbKingCount[wCurrentUser] += 1;
		}
		//���͸���
		if(m_cbKingCardIndex[1]==m_GameLogic.SwitchToCardIndex(m_cbSendCardData))
		{
			m_cbViceKingCount[wCurrentUser] += 1;
		}

		//��������
		m_cbCardIndex[wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

		//���ñ���
		m_wProvideUser=wCurrentUser;
		m_cbProvideCard=m_cbSendCardData;

		//�����ж�
		if (m_cbLeftCardCount>m_cbMustLeft-1)
		{
			tagGangCardResult GangCardResult;
			m_wUserAction[wCurrentUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[wCurrentUser],
				m_WeaveItemArray[wCurrentUser],m_cbWeaveItemCount[wCurrentUser],GangCardResult);
		}

		//����Ȩλ
		WORD wChiHuRight=0;
		if (m_bGangStatus==true) wChiHuRight|=CHR_QIANG_GANG;

		//�����ж�
		BYTE cbTempCardIndex[MAX_INDEX];
		CopyMemory(cbTempCardIndex,m_cbCardIndex[m_wCurrentUser],sizeof(cbTempCardIndex));
		if(m_GameLogic.RemoveCard(cbTempCardIndex,m_cbSendCardData)==false)
		{
			ASSERT(FALSE);
		}
		tagChiHuResult ChiHuResult;
		m_wUserAction[wCurrentUser]|=m_GameLogic.AnalyseChiHuCard(cbTempCardIndex,
			m_WeaveItemArray[wCurrentUser],m_cbWeaveItemCount[wCurrentUser],m_cbSendCardData,wChiHuRight,ChiHuResult,true);

	}

	//��������
	CMD_S_SendCard SendCard;
	SendCard.wCurrentUser=wCurrentUser;
	SendCard.wActionMask=m_wUserAction[wCurrentUser];
	SendCard.cbCardData=(m_bSendStatus==true)?m_cbSendCardData:0x00;
	SendCard.cbIsNotGang=bNotGang;

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

	//��ׯ���� ��Ϊ��������ɾ��һ�� ����Ҫ���һ��
	if ((m_cbLeftCardCount<=m_cbMustLeft-1)&&(!(m_wUserAction[wCurrentUser]&WIK_CHI_HU)))
	{
		m_cbChiHuCard=0;
		m_wProvideUser=INVALID_CHAIR;
		OnEventGameEnd(m_wProvideUser,NULL,GER_NORMAL);

		return true;
	}

	return true;
}

//��Ӧ�ж�
bool CTableFrameSink::EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind)
{
	//��������
	bool bAroseAction=false;

	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_wUserAction,sizeof(m_wUserAction));
	ZeroMemory(m_wPerformAction,sizeof(m_wPerformAction));

	//�����ж�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//�û�����
		if (wCenterUser==i) continue;

		//��������
		if (EstimatKind==EstimatKind_OutCard)
		{
			//�����ж�

				//�����ж�
				m_wUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard);

				//�����ж�
				WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
				if (wEatUser==i)
					m_wUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard);


				//�����ж�
				if (m_cbLeftCardCount>m_cbMustLeft-1) 
				{
					m_wUserAction[i]|=m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard);
				}
		}

		//�����ж�
		if (m_bEnjoinChiHu[i]==false)
		{
			//����Ȩλ
			WORD wChiHuRight=0;
			if (m_bGangStatus==true) 
				wChiHuRight|=CHR_QIANG_GANG;
			if ((m_cbSendCardCount==1)&&(m_cbOutCardCount==1))
				wChiHuRight|=CHR_DI;
			if ((m_cbSendCardCount==1)&&(m_cbOutCardCount==0))
				wChiHuRight|=CHR_TIAN;

			//�Ժ��ж�
			tagChiHuResult ChiHuResult;
			BYTE cbWeaveCount=m_cbWeaveItemCount[i];
			m_wUserAction[i]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],m_WeaveItemArray[i],cbWeaveCount,cbCenterCard,wChiHuRight,ChiHuResult);

			if(m_GameLogic.IsKingWait(m_cbCardIndex[i],m_WeaveItemArray[i],cbWeaveCount,cbCenterCard,false)&&wChiHuRight!=CHR_DI)
			{
				m_wUserAction[i] &=~WIK_CHI_HU;
			}
			//�Ժ�����
			if ((m_wUserAction[i]&WIK_CHI_HU)!=0) 
				m_bEnjoinChiHu[i]=true;
		}

		//����ж�
		if (m_wUserAction[i]!=WIK_NULL) 
			bAroseAction=true;
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
		SendOperateNotify();

		return true;
	}

	return false;
}
//�������
bool CTableFrameSink::OnUserCallFinish(WORD wChairID,BYTE cbChipTiems)
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_BATCH);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_BATCH) return true;

	//�û�Ч��
	ASSERT(m_bCallCard[wChairID]==false);
	if (m_bCallCard[wChairID]==true) return false;

	ASSERT(cbChipTiems>=0&&cbChipTiems<=1);
	if (cbChipTiems>=2)
	{
		ASSERT(FALSE);
		return  false;
	}

	//��������
	m_bCallCard[wChairID]=true;
	m_cbBatch[wChairID]=cbChipTiems;

	//����ж�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_bCallCard[i]==false) break;
	}
	if (i!=m_wPlayerCount) return true;


	//����״̬
	m_pITableFrame->SetGameStatus(GS_MJ_PLAY);

	//��������
	CMD_S_BatchResult BatchResult;
	CopyMemory(BatchResult.cbBatchResult,m_cbBatch,sizeof(m_cbBatch));

	//��������
	//	��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_BATCHRESULT,&BatchResult,sizeof(BatchResult));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_BATCHRESULT,&BatchResult,sizeof(BatchResult));

	//����ϴ��
	if(m_cbBatch[0]+m_cbBatch[1]+m_cbBatch[2]+m_cbBatch[3]==0)
	{
		//ϴ�����
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			m_bBatchEnd[i]=true;
		}

		//��Ϸ��ʼ
		OnEventGameStart2();

	}
	return true;
}

//��Ϸ��ʼ
bool CTableFrameSink::OnEventGameStart2()
{
	//Ԥ������
	m_cbMustLeft =MUST_LEFT_COUNT;

	//�����˿�
	m_GameLogic.RandCardData(m_cbRepertoryCard,CountArray(m_cbRepertoryCard));

	m_wSiceCount=MAKEWORD(rand()%6+1,rand()%6+1);
	m_wSiceCount2=MAKEWORD(rand()%6+1,rand()%6+1);
	m_wSiceCount3=MAKEWORD(rand()%6+1,rand()%6+1);

	//���þ���
	BYTE m_cbKingBrothterPos = (HIBYTE(m_wSiceCount3)+LOBYTE(m_wSiceCount3))*2-2;
	//����
	BYTE cbKingCardData=m_cbRepertoryCard[m_cbKingBrothterPos];
	//����
	BYTE cbViceKingCardData = m_GameLogic.GetKingFromBrother(m_cbRepertoryCard[m_cbKingBrothterPos]);

	//���·�����
	//����
	BYTE cbUpDownKingData=m_cbRepertoryCard[m_cbKingBrothterPos+1];
	//����
	BYTE cbUpDownViceKingData=m_GameLogic.GetKingFromBrother(cbUpDownKingData);

	//����
	BYTE cbUpDownKingIndex=m_GameLogic.SwitchToCardIndex(cbUpDownKingData);
	BYTE cbUpDownViceKingIndex=m_GameLogic.SwitchToCardIndex(cbUpDownViceKingData);

	m_cbKingCardIndex[0]=m_GameLogic.SwitchToCardIndex(cbKingCardData);
	m_cbKingCardIndex[1]=m_GameLogic.SwitchToCardIndex(cbViceKingCardData);

	//��������
	m_GameLogic.SetKingCardIndex(m_cbKingCardIndex);
	m_cbLeftCardCount=CountArray(m_cbRepertoryCard);

	m_GameLogic.SetKingCardIndex(m_cbKingCardIndex);


	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_cbLeftCardCount-=(MAX_COUNT-1);
		m_GameLogic.SwitchToCardIndex(&m_cbRepertoryCard[m_cbLeftCardCount],MAX_COUNT-1,m_cbCardIndex[i]);
	}

	//�����˿�
	m_cbSendCardCount++;
	m_cbSendCardData=m_cbRepertoryCard[--m_cbLeftCardCount];
	m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

	//���ñ���
	m_cbProvideCard=m_cbSendCardData;
	m_wProvideUser=INVALID_CHAIR;
	m_wCurrentUser=m_wBankerUser;

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
			m_wUserAction[i]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[i],NULL,0,GangCardResult);

			BYTE cbTempCardIndex[MAX_INDEX];
			CopyMemory(cbTempCardIndex,m_cbCardIndex[i],sizeof(cbTempCardIndex));
			if(m_GameLogic.RemoveCard(cbTempCardIndex,m_cbSendCardData)==false)
			{
				ASSERT(FALSE);
			}
			//�����ж�
			tagChiHuResult ChiHuResult;
			m_wUserAction[i]|=m_GameLogic.AnalyseChiHuCard(cbTempCardIndex,NULL,0,m_cbSendCardData,0,ChiHuResult,true);
		}

		//״̬����
		if ((bAroseAction==false)&&(i!=m_wBankerUser)&&(m_wUserAction[i]!=WIK_NULL))
		{
			bAroseAction=true;
			m_wResumeUser=m_wCurrentUser;
			m_wCurrentUser=INVALID_CHAIR;
		}
	}

	//��������
	CMD_S_GameStart GameStart;
	GameStart.wSiceCount=m_wSiceCount;
	GameStart.wSiceCount2=m_wSiceCount2;
	GameStart.wSiceCount3=m_wSiceCount3;
	GameStart.wBankerUser=m_wBankerUser;
	GameStart.wCurrentUser=m_wCurrentUser;
	CopyMemory(GameStart.cbKingCardIndex,m_cbKingCardIndex,sizeof(m_cbKingCardIndex));
	CopyMemory(GameStart.cbChip,m_cbBatch,sizeof(m_cbBatch));
	CopyMemory(GameStart.bTrustee,m_bTrustee,sizeof(m_bTrustee));
	
	BYTE cbUpDownKingCount[]={0,0,0,0};
	BYTE cbUpDownViceKingCount[]={0,0,0,0};

	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		for (BYTE j=0;j<MAX_INDEX;j++)
		{
			//�������·�����

			if(j==cbUpDownViceKingIndex && m_cbCardIndex[i][j]>0)
			{
				cbUpDownViceKingCount[i] +=m_cbCardIndex[i][j];	
			}
			if(j==cbUpDownKingIndex && m_cbCardIndex[i][j]>0)
			{
				cbUpDownKingCount[i] +=m_cbCardIndex[i][j];
			}
		}

	}
	//�������·� ������ ���
	BYTE cbOverlordKing[4]={1,1,1,1};//��������
	BYTE cbHasKingCount=0;
	WORD wUpDownKingScore[]={0,0,0,0};

	//�жϰ�����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if(cbUpDownKingCount[i]+cbUpDownViceKingCount[i]>0)
		{
			cbHasKingCount++;
		}

	}

	//�жϽ��
	if(cbHasKingCount==1)
	{
		//�Ұ������
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if(cbUpDownKingCount[i]+cbUpDownViceKingCount[i]>0)
			{
				cbOverlordKing[i]=2;
			}

		}
	}

	//�жϰ��� ���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if(cbUpDownKingCount[i]+cbUpDownViceKingCount[i]>0)
		{
			wUpDownKingScore[i]=(2*cbUpDownKingCount[i]+cbUpDownViceKingCount[i]);
			if(wUpDownKingScore[i]>=5)
			{
				wUpDownKingScore[i] =(wUpDownKingScore[i])*(wUpDownKingScore[i]-3)*cbOverlordKing[i];
			}
			else
				wUpDownKingScore[i] *=cbOverlordKing[i];

			//����
			for(WORD j=0;(j<GAME_PLAYER);j++)
			{
				if(i!=j)
				{
					m_lUpDownScore[i] += wUpDownKingScore[i]*m_pGameServiceOption->lCellScore*m_BankContinueCount;
					m_lUpDownScore[j] -= wUpDownKingScore[i]*m_pGameServiceOption->lCellScore*m_BankContinueCount;
				}
			}
		}
	}


	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		for (BYTE j=0;j<MAX_INDEX;j++)
		{
			if(j==m_cbKingCardIndex[0]&& m_cbCardIndex[i][j]>0)
			{
				m_cbKingCount[i]+=m_cbCardIndex[i][j];
			}
			if(j==m_cbKingCardIndex[1]&& m_cbCardIndex[i][j]>0)
			{
				m_cbViceKingCount[i]+=m_cbCardIndex[i][j];
			}
		}
	}
	m_cbUpDownKingCardData=cbUpDownKingData;

	GameStart.cbUpDownKingCardData=m_cbUpDownKingCardData;
	GameStart.cbFloor=max(m_BankContinueCount,1);
	CopyMemory(GameStart.lUpDownScore,m_lUpDownScore,sizeof(m_lUpDownScore));

	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//���ñ���
		GameStart.wUserAction=m_wUserAction[i];
		m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameStart.cbCardData[i],MAX_COUNT);
	}
	//��������
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//��������
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}

	return true;

}

bool CTableFrameSink::OnUserBatchEnd(WORD wChairID)
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

	//�û�Ч��
	ASSERT(m_bBatchEnd[wChairID]==false);
	if (m_bBatchEnd[wChairID]==true) return false;


	//��������
	m_bBatchEnd[wChairID]=true;

	//����ж�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_bBatchEnd[i]==false) break;
	}
	if (i!=m_wPlayerCount) return true;

	//��Ϸ��ʼ
	OnEventGameStart2();

	return true;

}
//////////////////////////////////////////////////////////////////////////
