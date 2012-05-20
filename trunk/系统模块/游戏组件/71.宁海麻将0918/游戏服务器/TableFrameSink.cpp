#include "StdAfx.h"
#include "TableFrameSink.h"
#include  "math.h"
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
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));


	//������Ϣ
	m_cbOutCardData=0;
	m_cbOutCardCount=0;

	m_wOutCardUser=INVALID_CHAIR;
	m_cbLeftCardCount=0;
	m_cbRemainCardCount = GAME_REMAIN;
	m_cbGangCount = 0;
	ZeroMemory(m_cbEnjoinHuCard,sizeof(m_cbEnjoinHuCard));
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));
	ZeroMemory(m_bCallCard,sizeof(m_bCallCard));

	//������Ŀ
	ZeroMemory(m_cbWindCount,sizeof(m_cbWindCount));
	ZeroMemory(m_cbTempWinCount,sizeof(m_cbTempWinCount));
	ZeroMemory(m_cbWindData,sizeof(m_cbWindData));
	ZeroMemory(m_cbUserWindCount,sizeof(m_cbUserWindCount));
	ZeroMemory(m_cbUserWindData,sizeof(m_cbUserWindData));
	m_cbAllWindCount = 0;
	m_cbSumWindCount = 0;

	//������Ϣ
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
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
	ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));

	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

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
	if (m_pITableFrame==NULL) 
		return false;

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
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));

	//������Ϣ
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	m_cbLeftCardCount=0;
	m_cbRemainCardCount = GAME_REMAIN;
	m_cbGangCount = 0;
	ZeroMemory(m_cbEnjoinHuCard,sizeof(m_cbEnjoinHuCard));
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));
	ZeroMemory(m_bCallCard,sizeof(m_bCallCard));

	//������Ϣ
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));

	//���б���
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	ZeroMemory(m_cbWindCount,sizeof(m_cbWindCount));
	ZeroMemory(m_cbTempWinCount,sizeof(m_cbTempWinCount));
	ZeroMemory(m_cbWindData,sizeof(m_cbWindData));
	ZeroMemory(m_cbUserWindCount,sizeof(m_cbUserWindCount));
	ZeroMemory(m_cbUserWindData,sizeof(m_cbUserWindData));
	m_cbAllWindCount = 0;
	m_cbSumWindCount = 0;

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

	//����˿�
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

	//�����˿�
	m_wSiceCount=MAKEWORD(rand()%6+1,rand()%6+1);
	m_cbLeftCardCount=CountArray(m_cbRepertoryCard);
	m_GameLogic.RandCardData(m_cbRepertoryCard,CountArray(m_cbRepertoryCard));
	


	//�ַ��˿�
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
		//ׯ���ж�
		if (i==m_wBankerUser)
		{
			//�����ж�
			tagGangCardResult GangCardResult;
			m_cbUserAction[i]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[i],NULL,0,GangCardResult);

			//�����ж�
			tagChiHuResult ChiHuResult;
			BYTE cbHandTai=0;
			BYTE cbHandFeng =0;
			m_cbUserAction[i]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],NULL,0,0,0,ChiHuResult,i,true,true);

		}

		//״̬����
		if ((bAroseAction==false)&&(i!=m_wBankerUser)&&(m_cbUserAction[i]!=WIK_NULL))
		{
			bAroseAction=true;
			m_wResumeUser=m_wCurrentUser;
			m_wCurrentUser=INVALID_CHAIR;
		}
	}

	//��������
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));
	GameStart.wSiceCount=m_wSiceCount;
	GameStart.wBankerUser=m_wBankerUser;
	GameStart.wCurrentUser=INVALID_CHAIR;
	GameStart.cbLeftCardCount = m_cbLeftCardCount;
	GameStart.bFirst=true;
	CopyMemory(GameStart.bTrustee,m_bTrustee,sizeof(m_bTrustee));

	//��������
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		for (BYTE j=MAX_INDEX-8;j<MAX_INDEX;j++)
		{
			m_cbWindCount[i] += m_cbCardIndex[i][j];
		}
		m_cbSumWindCount += m_cbWindCount[i];
		m_cbAllWindCount += m_cbWindCount[i];
	}

	//��������
	IServerUserItem *pUserItem = NULL;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//���ñ���
		GameStart.cbUserAction=m_cbUserAction[i];
		pUserItem = m_pITableFrame->GetServerUserItem(i);
		m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameStart.cbCardData,CountArray(GameStart.cbCardData));
		if(i == m_wBankerUser)
			CopyMemory(m_cbWindData[i],&GameStart.cbCardData[MAX_COUNT-m_cbWindCount[i]],sizeof(BYTE)*m_cbWindCount[i]);
		else
			CopyMemory(m_cbWindData[i],&GameStart.cbCardData[MAX_COUNT-m_cbWindCount[i]-1],sizeof(BYTE)*m_cbWindCount[i]);
		if( pUserItem->IsAndroidUser() )
		{
			BYTE bIndex = 1;
			for( WORD j=0; j<GAME_PLAYER; j++ )
			{
				if( j == i ) continue;
				m_GameLogic.SwitchToCardData(m_cbCardIndex[j],&GameStart.cbCardData[MAX_COUNT*bIndex++]);
			}
			CopyMemory(GameStart.cbRepertoryCard,m_cbRepertoryCard,sizeof(m_cbRepertoryCard));
		}

		GameStart.cbSunWindCount = m_cbSumWindCount;

		//��������
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
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
			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			LONG lCellScore=m_pGameServiceOption->lCellScore;

			//������Ϣ
			GameEnd.wProvideUser=wChairID;
			GameEnd.cbProvideCard=m_cbChiHuCard;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//��������
				GameEnd.wChiHuKind[i]	=m_ChiHuResult[i].wChiHuKind;
				GameEnd.wChiHuRight[i]	=m_ChiHuResult[i].wChiHuRight;
			}

			//ͳ�ƻ���
			if (m_wProvideUser!=INVALID_CHAIR)
			{
				WORD  wWinUser =INVALID_CHAIR;

				//��������
				if (m_ChiHuResult[m_wProvideUser].wChiHuKind!=CHK_NULL)
				{

					wWinUser=m_wProvideUser;
					BYTE cbHua[4]={0,0,0,0};
					BYTE cbHu[4]={0,0,0,0};
					if(m_wProvideUser == m_wBankerUser)//����λ
					{
						if(m_cbUserWindCount[m_wProvideUser] == 8)
						{
							cbHua[m_wProvideUser] += 4;
							cbHu[m_wProvideUser] += 32;
						}
						else
						{
							cbHu[m_wProvideUser] += 4*m_cbUserWindCount[m_wProvideUser];
							m_GameLogic.SortCardList(m_cbUserWindData[m_wProvideUser],m_cbUserWindCount[m_wProvideUser]);
							
							for (INT bTemp = 0;bTemp<m_cbUserWindCount[m_wProvideUser];bTemp++)
							{
								//÷ �� ����
								if((m_cbUserWindData[m_wProvideUser][bTemp]==0x38)||(m_cbUserWindData[m_wProvideUser][bTemp]==0x3C))
								{
									cbHua[m_wProvideUser] +=1;
									if(bTemp+3<m_cbUserWindCount[m_wProvideUser])
									{
										if(m_cbUserWindData[m_wProvideUser][bTemp]==0x38 &&m_cbUserWindData[m_wProvideUser][bTemp+3]==0x3B)
											cbHua[m_wProvideUser]++;
										if(m_cbUserWindData[m_wProvideUser][bTemp]==0x3C &&m_cbUserWindData[m_wProvideUser][bTemp+3]==0x3F)
											cbHua[m_wProvideUser]++;

									}
								}
							}
						}
					}

					if(m_wProvideUser == (m_wBankerUser+m_wPlayerCount-1)%m_wPlayerCount)//�Ϸ�λ
					{
						if(m_cbUserWindCount[m_wProvideUser] == 8)
						{
							cbHua[m_wProvideUser] += 4;
							cbHu[m_wProvideUser] += 32;
						}
						else
						{
							cbHu[m_wProvideUser] += 4*m_cbUserWindCount[m_wProvideUser];
							m_GameLogic.SortCardList(m_cbUserWindData[m_wProvideUser],m_cbUserWindCount[m_wProvideUser]);
							for (INT bTemp = 0;bTemp<m_cbUserWindCount[m_wProvideUser];bTemp++)
							{
								//�� �� ����
								if((m_cbUserWindData[m_wProvideUser][bTemp]==0x39)||(m_cbUserWindData[m_wProvideUser][bTemp]==0x3D))
									cbHua[m_wProvideUser]++;
							
								if(bTemp+2<m_cbUserWindCount[m_wProvideUser]&&bTemp>=1)
								{
									if(m_cbUserWindData[m_wProvideUser][bTemp]==0x39 &&m_cbUserWindData[m_wProvideUser][bTemp+2]==0x3B&&m_cbUserWindData[m_wProvideUser][bTemp-1]==0x38)
										cbHua[m_wProvideUser]++;
									if(m_cbUserWindData[m_wProvideUser][bTemp]==0x3D &&m_cbUserWindData[m_wProvideUser][bTemp+2]==0x3F&&m_cbUserWindData[m_wProvideUser][bTemp-1]==0x3C)
										cbHua[m_wProvideUser]++;

								}
							}


						}
					}
					if(m_wProvideUser == (m_wBankerUser+m_wPlayerCount-2)%m_wPlayerCount)//����λ
					{
	
						if(m_cbUserWindCount[m_wProvideUser] == 8)
						{
							cbHua[m_wProvideUser] += 4;
							cbHu[m_wProvideUser] += 32;
						}
						else
						{
							cbHu[m_wProvideUser] += 4*m_cbUserWindCount[m_wProvideUser];
							m_GameLogic.SortCardList(m_cbUserWindData[m_wProvideUser],m_cbUserWindCount[m_wProvideUser]);
							
							for (INT bTemp = 0;bTemp<m_cbUserWindCount[m_wProvideUser];bTemp++)
							{

								//���
								if((m_cbUserWindData[m_wProvideUser][bTemp]==0x3A)||(m_cbUserWindData[m_wProvideUser][bTemp]==0x3E))
									cbHua[m_wProvideUser] +=1;
								if(bTemp+1<m_cbUserWindCount[m_wProvideUser]&&bTemp>=2)
								{
									if(m_cbUserWindData[m_wProvideUser][bTemp]==0x3A &&m_cbUserWindData[m_wProvideUser][bTemp+1]==0x3B &&m_cbUserWindData[m_wProvideUser][bTemp-2]==0x38)
										cbHua[m_wProvideUser]++;
									if(m_cbUserWindData[m_wProvideUser][bTemp]==0x3E &&m_cbUserWindData[m_wProvideUser][bTemp+1]==0x3F&&m_cbUserWindData[m_wProvideUser][bTemp-2]==0x3C)
										cbHua[m_wProvideUser]++;

								}
							}
						}
					}
					if(m_wProvideUser == (m_wBankerUser+m_wPlayerCount-3)%m_wPlayerCount)//����λ
					{

						if(m_cbUserWindCount[m_wProvideUser] == 8)
						{
							cbHua[m_wProvideUser] += 4;
							cbHu[m_wProvideUser] += 32;
						}
						else
						{
							cbHu[m_wProvideUser] += 4*m_cbUserWindCount[m_wProvideUser];
							m_GameLogic.SortCardList(m_cbUserWindData[m_wProvideUser],m_cbUserWindCount[m_wProvideUser]);
							
							for (INT bTemp = 0;bTemp<m_cbUserWindCount[m_wProvideUser];bTemp++)
							{

								//��÷
								if((m_cbUserWindData[m_wProvideUser][bTemp]==0x3B)||(m_cbUserWindData[m_wProvideUser][bTemp]==0x3F))
									cbHua[m_wProvideUser] +=1;
								if(bTemp-3<m_cbUserWindCount[m_wProvideUser])
								{
									if(m_cbUserWindData[m_wProvideUser][bTemp]==0x38 &&m_cbUserWindData[m_wProvideUser][bTemp-3]==0x3B)
										cbHua[m_wProvideUser]++;
									if(m_cbUserWindData[m_wProvideUser][bTemp]==0x3C &&m_cbUserWindData[m_wProvideUser][bTemp-3]==0x3F)
										cbHua[m_wProvideUser]++;

								}
							}


						}
					}
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(i==m_wProvideUser) continue;
						//���ͷ���
						for (BYTE j=0;j<m_cbWeaveItemCount[i];j++)
						{
							BYTE cbWeaveKind=m_WeaveItemArray[i][j].cbWeaveKind;
							if(cbWeaveKind==WIK_PENG)
							{
								if((m_WeaveItemArray[i][j].cbCenterCard&MASK_COLOR)==0x30)
								{
									cbHu[i] +=4;
								}
								else
								{
									if((m_WeaveItemArray[i][j].cbCenterCard&MASK_VALUE)==0x01 ||(m_WeaveItemArray[i][j].cbCenterCard&MASK_VALUE)==0x09)
									{
										cbHu[i] +=4;
									}
									else
										cbHu[i] +=2;
								}
							}
							if(cbWeaveKind==WIK_GANG)
							{
								if(m_WeaveItemArray[i][j].cbPublicCard==FALSE)
								{
									if((m_WeaveItemArray[i][j].cbCenterCard&MASK_COLOR)==0x30)
									{
										cbHu[i] +=32;
									}
									else
									{
										if((m_WeaveItemArray[i][j].cbCenterCard&MASK_VALUE)==0x01 ||(m_WeaveItemArray[i][j].cbCenterCard&MASK_VALUE)==0x09)
										{
											cbHu[i] +=32;
										}
										else
											cbHu[i] +=16;
									}
								}
								if(m_WeaveItemArray[i][j].cbPublicCard==TRUE)
								{
									if((m_WeaveItemArray[i][j].cbCenterCard&MASK_COLOR)==0x30)
									{
										cbHu[i] +=16;
									}
									else
									{
										if((m_WeaveItemArray[i][j].cbCenterCard&MASK_VALUE)==0x01 ||(m_WeaveItemArray[i][j].cbCenterCard&MASK_VALUE)==0x09)
										{
											cbHu[i] +=16;
										}
										else
											cbHu[i] +=8;
									}
								}
							}

							if(m_WeaveItemArray[i][j].cbCenterCard>=0x35 &&m_WeaveItemArray[i][j].cbCenterCard<=0x37)
							{
								cbHua[i]+=1;
							}
							if(i == m_wBankerUser && m_WeaveItemArray[i][j].cbCenterCard==0x31)
							{
								cbHua[i] += 1;
							}
							if((i == (m_wBankerUser+m_wPlayerCount-1)%m_wPlayerCount) && m_WeaveItemArray[i][j].cbCenterCard==0x32)
							{
								cbHua[i] += 1;
							}
							if((i == (m_wBankerUser+m_wPlayerCount-2)%m_wPlayerCount) && m_WeaveItemArray[i][j].cbCenterCard==0x33)
							{
								cbHua[i] += 1;
							}
							if((i == (m_wBankerUser+m_wPlayerCount-3)%m_wPlayerCount) && m_WeaveItemArray[i][j].cbCenterCard==0x34)
							{
								cbHua[i] += 1;
							}
						}

						//����
						if(i == m_wBankerUser)//����λ
						{
							if(m_cbUserWindCount[i] == 8)
							{
								cbHua[i] += 4;
								cbHu[i] += 32;
							}
							else
							{
								cbHu[i] += 4*m_cbUserWindCount[i];
								m_GameLogic.SortCardList(m_cbUserWindData[i],m_cbUserWindCount[i]);
								//���� Ұ��
								for (INT bTemp = 0;bTemp<m_cbUserWindCount[i];bTemp++)
								{
									//÷ �� ����
									if((m_cbUserWindData[i][bTemp]==0x38)||(m_cbUserWindData[i][bTemp]==0x3C))
									{
										cbHua[i] +=1;
										if(bTemp+3<m_cbUserWindCount[i])
										{
											if(m_cbUserWindData[i][bTemp]==0x38 &&m_cbUserWindData[i][bTemp+3]==0x3B)
												cbHua[i]++;
											if(m_cbUserWindData[i][bTemp]==0x3C &&m_cbUserWindData[i][bTemp+3]==0x3F)
												cbHua[i]++;

										}
									}
								}
							}
						}

						if(i == (m_wBankerUser+m_wPlayerCount-1)%m_wPlayerCount)//�Ϸ�λ
						{
							if(m_cbUserWindCount[i] == 8)
							{
								cbHua[i] += 4;
								cbHu[i] += 32;
							}
							else
							{
								cbHu[i] += 4*m_cbUserWindCount[i];
								m_GameLogic.SortCardList(m_cbUserWindData[i],m_cbUserWindCount[i]);
								for (INT bTemp = 0;bTemp<m_cbUserWindCount[i];bTemp++)
								{
									//�� �� ����
									if((m_cbUserWindData[i][bTemp]==0x39)||(m_cbUserWindData[i][bTemp]==0x3D))
										cbHua[i]++;

									if(bTemp+2<m_cbUserWindCount[i]&&bTemp>=1)
									{
										if(m_cbUserWindData[i][bTemp]==0x39 &&m_cbUserWindData[i][bTemp+2]==0x3B&&m_cbUserWindData[i][bTemp-1]==0x38)
											cbHua[i]++;
										if(m_cbUserWindData[i][bTemp]==0x3D &&m_cbUserWindData[i][bTemp+2]==0x3F&&m_cbUserWindData[i][bTemp-1]==0x3C)
											cbHua[i]++;

									}
								}


							}
						}
						if(i == (m_wBankerUser+m_wPlayerCount-2)%m_wPlayerCount)//����λ
						{

							if(m_cbUserWindCount[i] == 8)
							{
								cbHua[i] += 4;
								cbHu[i] += 32;
							}
							else
							{
								cbHu[i] += 4*m_cbUserWindCount[i];
								m_GameLogic.SortCardList(m_cbUserWindData[i],m_cbUserWindCount[i]);
								//���� Ұ��
								for (INT bTemp = 0;bTemp<m_cbUserWindCount[i];bTemp++)
								{

									//÷ �� ����
									if((m_cbUserWindData[i][bTemp]==0x3A)||(m_cbUserWindData[i][bTemp]==0x3E))
										cbHua[i] +=1;
									if(bTemp+1<m_cbUserWindCount[i]&&bTemp>=2)
									{
										if(m_cbUserWindData[i][bTemp]==0x3A &&m_cbUserWindData[i][bTemp+1]==0x3B &&m_cbUserWindData[i][bTemp-2]==0x38)
											cbHua[i]++;
										if(m_cbUserWindData[i][bTemp]==0x3E &&m_cbUserWindData[i][bTemp+1]==0x3F&&m_cbUserWindData[i][bTemp-2]==0x3C)
											cbHua[i]++;

									}
								}
							}

						}
						if(i == (m_wBankerUser+m_wPlayerCount-3)%m_wPlayerCount)//����λ
						{

							if(m_cbUserWindCount[i] == 8)
							{
								cbHua[i] += 4;
								cbHu[i] += 32;
							}
							else
							{
								cbHu[i] += 4*m_cbUserWindCount[i];
								m_GameLogic.SortCardList(m_cbUserWindData[i],m_cbUserWindCount[i]);
								//���� Ұ��
								for (INT bTemp = 0;bTemp<m_cbUserWindCount[i];bTemp++)
								{

									//÷ �� ����
									if((m_cbUserWindData[i][bTemp]==0x3B)||(m_cbUserWindData[i][bTemp]==0x3F))
										cbHua[i] +=1;
									if(bTemp-3<m_cbUserWindCount[i])
									{
										if(m_cbUserWindData[i][bTemp]==0x38 &&m_cbUserWindData[i][bTemp-3]==0x3B)
											cbHua[i]++;
										if(m_cbUserWindData[i][bTemp]==0x3C &&m_cbUserWindData[i][bTemp-3]==0x3F)
											cbHua[i]++;

									}
								}


							}
						}

						for(BYTE j=0;j<MAX_INDEX-8;j++)
						{
							if(m_cbCardIndex[i][j]>=3)
							{
								if((j%9==8 || j%9==0)&&j<MAX_INDEX-15)
								{
									cbHu[i] += 8;
								}
								else if((j%9!=8 || j%9!=0)&&j<MAX_INDEX-15)
								{
									cbHu[i] += 4;
								}
								else
								{
									cbHu[i] += 8;
								}
							}
						}

						for(BYTE j=MAX_INDEX-3-8;j<MAX_INDEX-8;j++)
						{
							if(m_cbCardIndex[i][j]==2)
							{
								cbHu[i] += 2;

							}
							if(m_cbCardIndex[i][j]>=3)
							{
								cbHua[i] += 1;

							}
						}
						//����
						if(i == m_wBankerUser)//����λ
						{
							if(m_cbCardIndex[i][27]==2)
								cbHu[i] += 2;

							if(m_cbCardIndex[i][27]>=3)
								cbHua[i] += 1;


						}

						if(i == (m_wBankerUser+m_wPlayerCount-1)%m_wPlayerCount)//�Ϸ�λ
						{
							if(m_cbCardIndex[i][28]==2)
								cbHu[i] += 2;
							if(m_cbCardIndex[i][28]>=3)
								cbHua[i] += 1;

	
						}
						if(i == (m_wBankerUser+m_wPlayerCount-2)%m_wPlayerCount)//����λ
						{
							if(m_cbCardIndex[i][29]==2)
								cbHu[i] += 2;
							if(m_cbCardIndex[i][29]>=3)
								cbHua[i] += 1;


						}
						if(i == (m_wBankerUser+m_wPlayerCount-3)%m_wPlayerCount)//����λ
						{
							if(m_cbCardIndex[i][30]==2)
								cbHu[i] += 2;	
							if(m_cbCardIndex[i][30]>=3)
								cbHua[i] += 1;

						}

					}

					LONG lTimes=0;
					m_GameLogic.CalScore(m_ChiHuResult[m_wProvideUser],lTimes);
					WORD  cbAllTai=(cbHu[m_wProvideUser]+m_ChiHuResult[m_wProvideUser].cbHuCount+60)*pow(2,cbHua[m_wProvideUser]+m_ChiHuResult[m_wProvideUser].cbHuTimes+lTimes);
					GameEnd.cbHua[m_wProvideUser]=cbHua[m_wProvideUser]+m_ChiHuResult[m_wProvideUser].cbHuTimes+lTimes;
					if(cbAllTai%10)
					{
						cbAllTai -= cbAllTai%10;
						cbAllTai += 10;
					}

					if(cbAllTai>500)
						cbAllTai=500;

					GameEnd.wHuCount[m_wProvideUser]=cbAllTai;

					//ѭ���ۼ�
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						//Ӯ�ҹ���
						if (m_ChiHuResult[i].wChiHuKind!=CHK_NULL)	continue;	

						//�������
						GameEnd.lGameScore[i]-=2*m_pGameServiceOption->lCellScore*cbAllTai;					
						GameEnd.lGameScore[m_wProvideUser] +=2*m_pGameServiceOption->lCellScore*cbAllTai;
					}
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						//Ӯ�ҹ���
						if (m_ChiHuResult[i].wChiHuKind!=CHK_NULL)	continue;
						WORD cbAllTai=cbHu[i]*pow(2,cbHua[i]);
						if(cbAllTai%10)
						{
							cbAllTai -= cbAllTai%10;
							cbAllTai += 10;
						}
						if(cbAllTai>500)
							cbAllTai=500;
						GameEnd.cbHua[i]=cbHua[i]+m_ChiHuResult[i].cbHuTimes;
						GameEnd.wHuCount[i]=cbAllTai;

						if(i==m_wBankerUser)
						{
							for (WORD j=0;j<m_wPlayerCount;j++)
							{
								//Ӯ�ҹ���
								if (m_ChiHuResult[j].wChiHuKind!=CHK_NULL)	continue;
								if(i==j) continue;

								GameEnd.lGameScore[i] += 2*m_pGameServiceOption->lCellScore*cbAllTai;
								GameEnd.lGameScore[j] -= 2*m_pGameServiceOption->lCellScore*cbAllTai;

							}

						}
						else
						{
							for (WORD j=0;j<m_wPlayerCount;j++)
							{
								//Ӯ�ҹ���
								if (m_ChiHuResult[j].wChiHuKind!=CHK_NULL)	continue;

								if(i==j) continue;

								if(j==m_wBankerUser)
								{
									GameEnd.lGameScore[i] += 2*m_pGameServiceOption->lCellScore*cbAllTai;
									GameEnd.lGameScore[j] -= 2*m_pGameServiceOption->lCellScore*cbAllTai;
								}
								else
								{
									GameEnd.lGameScore[i] += m_pGameServiceOption->lCellScore*cbAllTai;
									GameEnd.lGameScore[j] -= m_pGameServiceOption->lCellScore*cbAllTai;
								}

							}

						}

						
					}
					//��ׯ��ʽ
					if(m_wProvideUser==m_wBankerUser)
					{
						m_wBankerUser=m_wBankerUser;
					}
					else
					{
						m_wBankerUser=(m_wBankerUser-1+m_wPlayerCount)%m_wPlayerCount;
					}

				}

				//׽������
				if (m_ChiHuResult[m_wProvideUser].wChiHuKind==CHK_NULL)
				{
					//ѭ���ۼ�
					BYTE cbHua[4]={0,0,0,0};
					BYTE cbHu[4]={0,0,0,0};

					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						//��ҹ���
						if (m_ChiHuResult[i].wChiHuKind==CHK_NULL) 		continue;

						wWinUser=i;
						if( i== m_wBankerUser) //����λ
						{
							if(m_cbUserWindCount[m_wBankerUser] == 8)
							{
								cbHua[m_wBankerUser] += 4;
								cbHu[m_wBankerUser] += 32;
							}
							else
							{
								cbHu[m_wBankerUser] += 4*m_cbUserWindCount[m_wBankerUser];
								m_GameLogic.SortCardList(m_cbUserWindData[m_wBankerUser],m_cbUserWindCount[m_wBankerUser]);
								
								for (INT bTemp = 0;bTemp<m_cbUserWindCount[m_wBankerUser];bTemp++)
								{
									//÷ �� 
									if((m_cbUserWindData[m_wBankerUser][bTemp]==0x38)||(m_cbUserWindData[m_wBankerUser][bTemp]==0x3C))
									{
										cbHua[m_wBankerUser] +=1;
										if(bTemp+3<m_cbUserWindCount[m_wBankerUser])
										{
											if(m_cbUserWindData[m_wBankerUser][bTemp]==0x38 &&m_cbUserWindData[m_wBankerUser][bTemp+3]==0x3B)
												cbHua[m_wBankerUser]++;
											if(m_cbUserWindData[m_wBankerUser][bTemp]==0x3C &&m_cbUserWindData[m_wBankerUser][bTemp+3]==0x3F)
												cbHua[m_wBankerUser]++;

										}
									}
								}
							}
						}

						if(i == (m_wBankerUser+m_wPlayerCount-1)%m_wPlayerCount)//�Ϸ�λ
						{
							if(m_cbUserWindCount[i] == 8)
							{
								cbHua[i] += 4;
								cbHu[i] += 32;
							}
							else
							{
								cbHu[i] += 4*m_cbUserWindCount[i];
								m_GameLogic.SortCardList(m_cbUserWindData[i],m_cbUserWindCount[i]);
								for (INT bTemp = 0;bTemp<m_cbUserWindCount[i];bTemp++)
								{
									//�� �� 
									if((m_cbUserWindData[i][bTemp]==0x39)||(m_cbUserWindData[i][bTemp]==0x3D))
										cbHua[i]++;

									if(bTemp+2<m_cbUserWindCount[i]&&bTemp>=1)
									{
										if(m_cbUserWindData[i][bTemp]==0x39 &&m_cbUserWindData[i][bTemp+2]==0x3B&&m_cbUserWindData[i][bTemp-1]==0x38)
											cbHua[i]++;
										if(m_cbUserWindData[i][bTemp]==0x3D &&m_cbUserWindData[i][bTemp+2]==0x3F&&m_cbUserWindData[i][bTemp-1]==0x3C)
											cbHua[i]++;

									}
								}


							}
						}
						if(i == (m_wBankerUser+m_wPlayerCount-2)%m_wPlayerCount)//����λ
						{
							if(m_cbUserWindCount[i] == 8)
							{
								cbHua[i] += 4;
								cbHu[i] += 32;
							}
							else
							{
								cbHu[i] += 4*m_cbUserWindCount[i];
								m_GameLogic.SortCardList(m_cbUserWindData[i],m_cbUserWindCount[i]);
								
								for (INT bTemp = 0;bTemp<m_cbUserWindCount[i];bTemp++)
								{

									//���
									if((m_cbUserWindData[i][bTemp]==0x3A)||(m_cbUserWindData[i][bTemp]==0x3E))
										cbHua[i] +=1;
									if(bTemp+1<m_cbUserWindCount[i]&&bTemp>=2)
									{
										if(m_cbUserWindData[i][bTemp]==0x3A &&m_cbUserWindData[i][bTemp+1]==0x3B &&m_cbUserWindData[i][bTemp-2]==0x38)
											cbHua[i]++;
										if(m_cbUserWindData[i][bTemp]==0x3E &&m_cbUserWindData[i][bTemp+1]==0x3F&&m_cbUserWindData[i][bTemp-2]==0x3C)
											cbHua[i]++;

									}
								}
							}

						}
						if(i == (m_wBankerUser+m_wPlayerCount-3)%m_wPlayerCount)//����λ
						{
							if(m_cbUserWindCount[i] == 8)
							{
								cbHua[i] += 4;
								cbHu[i] += 32;
							}
							else
							{
								cbHu[i] += 4*m_cbUserWindCount[i];
								m_GameLogic.SortCardList(m_cbUserWindData[i],m_cbUserWindCount[i]);
								
								for (INT bTemp = 0;bTemp<m_cbUserWindCount[i];bTemp++)
								{

									//����
									if((m_cbUserWindData[i][bTemp]==0x3B)||(m_cbUserWindData[i][bTemp]==0x3F))
										cbHua[i] +=1;
									if(bTemp-3<m_cbUserWindCount[i])
									{
										if(m_cbUserWindData[i][bTemp]==0x38 &&m_cbUserWindData[i][bTemp-3]==0x3B)
											cbHua[i]++;
										if(m_cbUserWindData[i][bTemp]==0x3C &&m_cbUserWindData[i][bTemp-3]==0x3F)
											cbHua[i]++;

									}
								}


							}
						}
						LONG lTimes=0;
						m_GameLogic.CalScore(m_ChiHuResult[i],lTimes);

						WORD  cbAllTai=(cbHu[i]+60+m_ChiHuResult[i].cbHuCount)*pow(2,cbHua[i]+m_ChiHuResult[i].cbHuTimes+lTimes);
						if(cbAllTai%10)
						{
							cbAllTai -= cbAllTai%10;
							cbAllTai += 10;
						}

						if(cbAllTai>500)
							cbAllTai=500;
						GameEnd.cbHua[i]=cbHua[i]+m_ChiHuResult[i].cbHuTimes+lTimes;
						GameEnd.wHuCount[i]=cbAllTai;

						for (WORD j=0;j<m_wPlayerCount;j++)
						{
							//Ӯ�ҹ���
							if (m_ChiHuResult[j].wChiHuKind!=CHK_NULL) 		continue;
							//�������
							if(j==m_wProvideUser)
							{
								GameEnd.lGameScore[j]-=2*m_pGameServiceOption->lCellScore*cbAllTai;
								GameEnd.lGameScore[i]+=2*m_pGameServiceOption->lCellScore*cbAllTai;
							}
							else
							{
								GameEnd.lGameScore[j]-=1*m_pGameServiceOption->lCellScore*cbAllTai;
								GameEnd.lGameScore[i]+=1*m_pGameServiceOption->lCellScore*cbAllTai;

							}
						}
						WORD wChiHuRight=0;
						if((!m_GameLogic.AnalyseTingCard(m_cbCardIndex[m_wProvideUser],m_WeaveItemArray[m_wProvideUser],m_cbWeaveItemCount[m_wProvideUser],wChiHuRight))&&(IsAfford(m_wProvideUser,wWinUser)>1))
						{
							for (WORD j=0;j<m_wPlayerCount;j++)
							{
								//��ҹ���
								if (m_ChiHuResult[j].wChiHuKind!=CHK_NULL) 		continue;
								if(j==m_wProvideUser) continue;
								//�������
								GameEnd.lGameScore[m_wProvideUser]+=GameEnd.lGameScore[j];
								GameEnd.lGameScore[j]=0;

							}

						}
					}
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						//Ӯ�ҹ���
						if (m_ChiHuResult[i].wChiHuKind!=CHK_NULL) 		continue;	//���ͷ���
						for (BYTE j=0;j<m_cbWeaveItemCount[i];j++)
						{
							BYTE cbWeaveKind=m_WeaveItemArray[i][j].cbWeaveKind;
							if(cbWeaveKind==WIK_PENG)
							{
								if((m_WeaveItemArray[i][j].cbCenterCard&MASK_COLOR)==0x30)
								{
									cbHu[i] +=4;
								}
								else
								{
									if((m_WeaveItemArray[i][j].cbCenterCard&MASK_VALUE)==0x01 ||(m_WeaveItemArray[i][j].cbCenterCard&MASK_VALUE)==0x09)
									{
										cbHu[i] +=4;
									}
									else
										cbHu[i] +=2;
								}
							}
							if(cbWeaveKind==WIK_GANG)
							{
								if(m_WeaveItemArray[i][j].cbPublicCard==FALSE)
								{
									if((m_WeaveItemArray[i][j].cbCenterCard&MASK_COLOR)==0x30)
									{
										cbHu[i] +=32;
									}
									else
									{
										if((m_WeaveItemArray[i][j].cbCenterCard&MASK_VALUE)==0x01 ||(m_WeaveItemArray[i][j].cbCenterCard&MASK_VALUE)==0x09)
										{
											cbHu[i] +=32;
										}
										else
											cbHu[i] +=16;
									}
								}
								if(m_WeaveItemArray[i][j].cbPublicCard==TRUE)
								{
									if((m_WeaveItemArray[i][j].cbCenterCard&MASK_COLOR)==0x30)
									{
										cbHu[i] +=16;
									}
									else
									{
										if((m_WeaveItemArray[i][j].cbCenterCard&MASK_VALUE)==0x01 ||(m_WeaveItemArray[i][j].cbCenterCard&MASK_VALUE)==0x09)
										{
											cbHu[i] +=16;
										}
										else
											cbHu[i] +=8;
									}
								}
							}



							if(m_WeaveItemArray[i][j].cbCenterCard>=0x35 &&m_WeaveItemArray[i][j].cbCenterCard<=0x37)
							{
								cbHua[i]+=1;
							}
							if(i == m_wBankerUser && m_WeaveItemArray[i][j].cbCenterCard==0x31)
							{
								cbHua[i] += 1;
							}
							if((i == (m_wBankerUser+m_wPlayerCount-1)%m_wPlayerCount) && m_WeaveItemArray[i][j].cbCenterCard==0x32)
							{
								cbHua[i] += 1;
							}
							if((i == (m_wBankerUser+m_wPlayerCount-2)%m_wPlayerCount) && m_WeaveItemArray[i][j].cbCenterCard==0x33)
							{
								cbHua[i] += 1;
							}
							if((i == (m_wBankerUser+m_wPlayerCount-3)%m_wPlayerCount) && m_WeaveItemArray[i][j].cbCenterCard==0x34)
							{
								cbHua[i] += 1;
							}
						}

						//����
						if(i == m_wBankerUser)//����λ
						{
							if(m_cbUserWindCount[m_wBankerUser] == 8)
							{
								cbHua[i] += 4;
								cbHu[i] += 32;
							}
							else
							{
								cbHu[i] += 4*m_cbUserWindCount[i];
								m_GameLogic.SortCardList(m_cbUserWindData[i],m_cbUserWindCount[i]);
								//���� Ұ��
								for (INT bTemp = 0;bTemp<m_cbUserWindCount[i];bTemp++)
								{
									//÷ �� ����
									if((m_cbUserWindData[i][bTemp]==0x38)||(m_cbUserWindData[i][bTemp]==0x3C))
									{
										cbHua[i] +=1;
										if(bTemp+3<m_cbUserWindCount[i])
										{
											if(m_cbUserWindData[i][bTemp]==0x38 &&m_cbUserWindData[i][bTemp+3]==0x3B)
												cbHua[i]++;
											if(m_cbUserWindData[i][bTemp]==0x3C &&m_cbUserWindData[i][bTemp+3]==0x3F)
												cbHua[i]++;

										}
									}
								}
							}
						}

						if(i == (m_wBankerUser+m_wPlayerCount-1)%m_wPlayerCount)//�Ϸ�λ
						{
							if(m_cbUserWindCount[i] == 8)
							{
								cbHua[i] += 4;
								cbHu[i] += 32;
							}
							else
							{
								cbHu[i] += 4*m_cbUserWindCount[i];
								m_GameLogic.SortCardList(m_cbUserWindData[i],m_cbUserWindCount[i]);
								for (INT bTemp = 0;bTemp<m_cbUserWindCount[i];bTemp++)
								{
									//�� �� ����
									if((m_cbUserWindData[i][bTemp]==0x39)||(m_cbUserWindData[i][bTemp]==0x3D))
										cbHua[i]++;

									if(bTemp+2<m_cbUserWindCount[i]&&bTemp>=1)
									{
										if(m_cbUserWindData[i][bTemp]==0x39 &&m_cbUserWindData[i][bTemp+2]==0x3B&&m_cbUserWindData[i][bTemp-1]==0x38)
											cbHua[i]++;
										if(m_cbUserWindData[i][bTemp]==0x3D &&m_cbUserWindData[i][bTemp+2]==0x3F&&m_cbUserWindData[i][bTemp-1]==0x3C)
											cbHua[i]++;

									}
								}


							}
						}
						if(i == (m_wBankerUser+m_wPlayerCount-2)%m_wPlayerCount)//����λ
						{

							if(m_cbUserWindCount[i] == 8)
							{
								cbHua[i] += 4;
								cbHu[i] += 32;
							}
							else
							{
								cbHu[i] += 4*m_cbUserWindCount[i];
								m_GameLogic.SortCardList(m_cbUserWindData[i],m_cbUserWindCount[i]);
								//���� Ұ��
								for (INT bTemp = 0;bTemp<m_cbUserWindCount[i];bTemp++)
								{

									//÷ �� ����
									if((m_cbUserWindData[i][bTemp]==0x3A)||(m_cbUserWindData[i][bTemp]==0x3E))
										cbHua[i] +=1;
									if(bTemp+1<m_cbUserWindCount[i]&&bTemp>=2)
									{
										if(m_cbUserWindData[i][bTemp]==0x3A &&m_cbUserWindData[i][bTemp+1]==0x3B &&m_cbUserWindData[i][bTemp-2]==0x38)
											cbHua[i]++;
										if(m_cbUserWindData[i][bTemp]==0x3E &&m_cbUserWindData[i][bTemp+1]==0x3F&&m_cbUserWindData[i][bTemp-2]==0x3C)
											cbHua[i]++;

									}
								}


							}

						}
						if(i == (m_wBankerUser+m_wPlayerCount-3)%m_wPlayerCount)//����λ
						{

							if(m_cbUserWindCount[i] == 8)
							{
								cbHua[i] += 4;
								cbHu[i] += 32;
							}
							else
							{
								cbHu[i] += 4*m_cbUserWindCount[i];
								m_GameLogic.SortCardList(m_cbUserWindData[i],m_cbUserWindCount[i]);
								//���� Ұ��
								for (INT bTemp = 0;bTemp<m_cbUserWindCount[i];bTemp++)
								{

									//÷ �� ����
									if((m_cbUserWindData[i][bTemp]==0x3B)||(m_cbUserWindData[i][bTemp]==0x3F))
										cbHua[i] +=1;
									if(bTemp-3<m_cbUserWindCount[i])
									{
										if(m_cbUserWindData[i][bTemp]==0x38 &&m_cbUserWindData[i][bTemp-3]==0x3B)
											cbHua[i]++;
										if(m_cbUserWindData[i][bTemp]==0x3C &&m_cbUserWindData[i][bTemp-3]==0x3F)
											cbHua[i]++;

									}
								}


							}
						}

						for(BYTE j=0;j<MAX_INDEX-8;j++)
						{
							if(m_cbCardIndex[i][j]>=3)
							{
								if((j%9==8 || j%9==0)&&j<MAX_INDEX-15)
								{
									cbHu[i] += 8;
								}
								else if((j%9!=8 || j%9!=0)&&j<MAX_INDEX-15)
								{
									cbHu[i] += 4;
								}
								else
								{
									cbHu[i] += 8;
								}
							}
						}

						for(BYTE j=MAX_INDEX-3-8;j<MAX_INDEX-8;j++)
						{
							if(m_cbCardIndex[i][j]==2)
							{
								cbHu[i] += 2;

							}
							if(m_cbCardIndex[i][j]>=3)
							{
								cbHua[i] += 1;

							}
						}
						//����
						if(i == m_wBankerUser)//����λ
						{
							if(m_cbCardIndex[i][27]==2)
								cbHu[i] += 2;
							if(m_cbCardIndex[i][27]>=3)
								cbHua[i] += 1;


						}

						if(i == (m_wBankerUser+m_wPlayerCount-1)%m_wPlayerCount)//�Ϸ�λ
						{
							if(m_cbCardIndex[i][28]==2)
								cbHu[i] += 2;
							if(m_cbCardIndex[i][28]>=3)
								cbHua[i] += 1;


						}
						if(i == (m_wBankerUser+m_wPlayerCount-2)%m_wPlayerCount)//����λ
						{
							if(m_cbCardIndex[i][29]==2)
								cbHu[i] += 2;
							if(m_cbCardIndex[i][29]>=3)
								cbHua[i] += 1;


						}
						if(i == (m_wBankerUser+m_wPlayerCount-3)%m_wPlayerCount)//����λ
						{
							if(m_cbCardIndex[i][30]==2)
								cbHu[i] += 2;
							if(m_cbCardIndex[i][30]>=3)
								cbHua[i] += 1;

						}
						WORD  cbAllTai=cbHu[i]*pow(2,cbHua[i]);
						if(cbAllTai%10)
						{
							cbAllTai -= cbAllTai%10;
							cbAllTai += 10;
						}

						if(cbAllTai>500)
							cbAllTai=500;

						GameEnd.cbHua[i]=cbHua[i]+m_ChiHuResult[i].cbHuTimes;
						GameEnd.wHuCount[i]=cbAllTai;

						if(i==m_wBankerUser)
						{
							for (WORD j=0;j<m_wPlayerCount;j++)
							{
								//��ҹ���
								if (m_ChiHuResult[j].wChiHuKind!=CHK_NULL) 		continue;
								if(j==i) continue;
								GameEnd.lGameScore[j]-=2*m_pGameServiceOption->lCellScore*cbAllTai;
								GameEnd.lGameScore[i]+=2*m_pGameServiceOption->lCellScore*cbAllTai;

							}
						}
						else
						{
							for (WORD j=0;j<m_wPlayerCount;j++)
							{
								//��ҹ���
								if (m_ChiHuResult[j].wChiHuKind!=CHK_NULL) 		continue;
								if(j==i) continue;
								if(j==m_wBankerUser)
								{
									GameEnd.lGameScore[j]-=2*m_pGameServiceOption->lCellScore*cbAllTai;
									GameEnd.lGameScore[i]+=2*m_pGameServiceOption->lCellScore*cbAllTai;

								}
								else
								{
									GameEnd.lGameScore[j]-=1*m_pGameServiceOption->lCellScore*cbAllTai;
									GameEnd.lGameScore[i]+=1*m_pGameServiceOption->lCellScore*cbAllTai;

								}
							}
						}
					}


					
					//��ׯ��ʽ
					if(wWinUser==m_wBankerUser)
					{
						m_wBankerUser=m_wBankerUser;
					}
					else
					{
						m_wBankerUser=(m_wBankerUser-1+m_wPlayerCount)%m_wPlayerCount;
					}
				}
			}
			else
			{
				//��ׯ����
				GameEnd.cbProvideCard=0;
				GameEnd.cbHaiDiCard = m_cbProvideCard;	
				m_wBankerUser = (m_wBankerUser+m_wPlayerCount-1)%m_wPlayerCount;
			}
			//�����˿�
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
			}

			//����˰��
			if (m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)
			{
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					if (GameEnd.lGameScore[i]>=1000L)
					{
						LONG lTempTax = GameEnd.lGameScore[i]*m_pGameServiceOption->wRevenue/1000;
						GameEnd.lGameTax+=lTempTax;
						GameEnd.lGameScore[i] -= lTempTax;
					}
				}
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//�޸Ļ���
			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				ScoreInfo[i].lScore=GameEnd.lGameScore[i];
				if (GameEnd.lGameScore[i]==0L) 
					ScoreInfo[i].ScoreKind=enScoreKind_Draw;
				else
					ScoreInfo[i].ScoreKind=(GameEnd.lGameScore[i]>0L)?enScoreKind_Win:enScoreKind_Lost;
			}
			//ͳ�ƻ���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//д�����
				m_pITableFrame->WriteUserScore(i,ScoreInfo[i].lScore,ScoreInfo[i].lRevenue,ScoreInfo[i].ScoreKind);
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

			//���ñ���
			GameEnd.cbProvideCard = 255;
			GameEnd.wProvideUser=INVALID_CHAIR;
			GameEnd.lGameScore[wChairID]=-24*m_pGameServiceOption->lCellScore;
			
			//�����˿�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
			}
			//���ֹ���
			IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
			const tagUserScore * pUserScore=pIServerUserItem->GetUserScore();


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

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//�޸Ļ���
			tagScoreInfo ScoreInfo;
			ScoreInfo.ScoreKind=enScoreKind_Flee;
			ScoreInfo.lScore=GameEnd.lGameScore[wChairID];
			m_pITableFrame->WriteUserScore(wChairID,ScoreInfo.lScore,0,ScoreInfo.ScoreKind);

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
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
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
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//��������
			StatusFree.wBankerUser=m_wBankerUser;
			StatusFree.lCellScore=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusFree.bTrustee,m_bTrustee,sizeof(m_bTrustee));

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_MJ_PLAY:	//��Ϸ״̬

		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//��Ϸ����
			StatusPlay.wSiceCount=m_wSiceCount;
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.lCellScore=m_pGameServiceOption->lCellScore;

			//״̬����
			StatusPlay.cbActionCard=m_cbProvideCard;
			StatusPlay.cbLeftCardCount=m_cbLeftCardCount;
			StatusPlay.cbActionMask=(m_bResponse[wChiarID]==false)?m_cbUserAction[wChiarID]:WIK_NULL;
			CopyMemory(StatusPlay.bTrustee,m_bTrustee,sizeof(m_bTrustee));
			
			//��ʷ��¼
			StatusPlay.wOutCardUser=m_wOutCardUser;
			StatusPlay.cbOutCardData=m_cbOutCardData;
			CopyMemory(StatusPlay.cbDiscardCard,m_cbDiscardCard,sizeof(StatusPlay.cbDiscardCard));
			CopyMemory(StatusPlay.cbDiscardCount,m_cbDiscardCount,sizeof(StatusPlay.cbDiscardCount));
			CopyMemory(StatusPlay.cbUserWindCount,m_cbUserWindCount,sizeof(m_cbUserWindCount));
			CopyMemory(StatusPlay.cbUserWindCardData,m_cbUserWindData,sizeof(m_cbUserWindData));

			//����˿�
			CopyMemory(StatusPlay.WeaveItemArray,m_WeaveItemArray,sizeof(m_WeaveItemArray));
			CopyMemory(StatusPlay.cbWeaveCount,m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

			//�˿�����
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
	switch (wSubCmdID)
	{
	case SUB_C_OUT_CARD:		//������Ϣ
		{
			//Ч����Ϣ
			ASSERT(wDataSize==sizeof(CMD_C_OutCard));
			if (wDataSize!=sizeof(CMD_C_OutCard)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) 	return true;

			//��Ϣ����
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pDataBuffer;
			return OnUserOutCard(pUserData->wChairID,pOutCard->cbCardData);
		}
	case SUB_C_OPERATE_CARD:	//������Ϣ
		{
			//Ч����Ϣ
			ASSERT(wDataSize==sizeof(CMD_C_OperateCard));
			if (wDataSize!=sizeof(CMD_C_OperateCard))	return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) 	return true;

			//��Ϣ����
			CMD_C_OperateCard * pOperateCard=(CMD_C_OperateCard *)pDataBuffer;
			return OnUserOperateCard(pUserData->wChairID,pOperateCard->cbOperateCode,pOperateCard->cbOperateCard);
		}
	case SUB_C_FINISH:
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) 	return true;

			return OnUserCallFinish(pUserData->wChairID);

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
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY)
		return true;

	//�������
	ASSERT(wChairID==m_wCurrentUser);
	ASSERT(m_GameLogic.IsValidCard(cbCardData)==true);

	//Ч�����
	if (wChairID!=m_wCurrentUser) 	return false;
	if (m_GameLogic.IsValidCard(cbCardData)==false)	return false;

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],cbCardData)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//���ñ���
	m_bSendStatus=true;
	m_bEnjoinChiHu[wChairID]=true;
	m_cbUserAction[wChairID]=WIK_NULL;
	m_cbPerformAction[wChairID]=WIK_NULL;


	//�Ժ����
	for (WORD i =0;i<m_wPlayerCount;i++)
	{
		if (i==wChairID) continue;
		else
			m_bEnjoinChiHu[i] = false;
	}


	//�������
	m_cbEnjoinHuCard[wChairID]=0;
	
	//���Ƽ�¼
	m_cbOutCardCount++;
	m_wOutCardUser=wChairID;
	m_cbOutCardData=cbCardData;

	//��������
	CMD_S_OutCard OutCard;
	OutCard.wOutCardUser=wChairID;
	OutCard.cbOutCardData=cbCardData;

	//���ñ���
	m_bSendStatus=true;
	m_cbUserAction[wChairID]=WIK_NULL;
	m_cbPerformAction[wChairID]=WIK_NULL;

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));

	//�û��л�
	m_wProvideUser=wChairID;
	m_cbProvideCard=cbCardData;
	m_wCurrentUser=(wChairID+m_wPlayerCount-1)%m_wPlayerCount;


	//��Ӧ�ж�
	bool bAroseAction=EstimateUserRespond(wChairID,cbCardData,EstimatKind_OutCard);

	//�ɷ��˿�
	if (bAroseAction==false)
		DispatchCardData(m_wCurrentUser,false);

	return true;
}

//�û�����
bool CTableFrameSink::OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard)
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY)	return true;

	//Ч���û�
	//ASSERT((wChairID==m_wCurrentUser)||(m_wCurrentUser==INVALID_CHAIR));
	if ((wChairID!=m_wCurrentUser)&&(m_wCurrentUser!=INVALID_CHAIR)) return true;
	

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
		}
		if (m_bResponse[wTargetUser]==false)	return true;

		//�Ժ��ȴ�
		if (cbTargetAction==WIK_CHI_HU)
		{
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if ((m_bResponse[i]==false)&&(m_cbUserAction[i]&WIK_CHI_HU)) 
					return true;
			}
		}

		//�Ժ�ִ���ж�
		if (cbTargetAction==WIK_CHI_HU)
		{
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//�����ж�
				WORD wFirstUser=(m_wProvideUser+m_wPlayerCount-1-i)%m_wPlayerCount;
				if(m_cbPerformAction[wFirstUser]&WIK_CHI_HU)
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
		if (cbTargetAction==WIK_NULL)
		{
			//�û�״̬
			ZeroMemory(m_bResponse,sizeof(m_bResponse));
			ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
			ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
			ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

			//�����˿�
			DispatchCardData(m_wResumeUser,false);

			return true;
		}

		//��������
		BYTE cbTargetCard=m_cbOperateCard[wTargetUser];

		//���Ʊ���
		m_bSendStatus=true;

		//���Ʋ���
		if (cbTargetAction==WIK_CHI_HU)
		{
			//������Ϣ
			m_cbChiHuCard=cbTargetCard;

			//����Ȩλ
			WORD wChiHuRight=0;
			//�����ж�
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//�����ж�
				if ((i==m_wProvideUser)||((m_cbPerformAction[i]&WIK_CHI_HU)==0))		continue;
				
				if(i!=wTargetUser) continue;

				//��ͨ����
				if (m_cbChiHuCard!=0)
				{
					//�����ж�
					BYTE cbWeaveItemCount=m_cbWeaveItemCount[i];
					tagWeaveItem * pWeaveItem=m_WeaveItemArray[i];
					m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],pWeaveItem,cbWeaveItemCount,m_cbChiHuCard,wChiHuRight,m_ChiHuResult[i],(m_wBankerUser-i+GAME_PLAYER)%GAME_PLAYER,false,false);
	

					//�����˿�
					if (m_ChiHuResult[i].wChiHuKind!=CHK_NULL)
						m_cbCardIndex[i][m_GameLogic.SwitchToCardIndex(m_cbChiHuCard)]++;
				}
			}
			//������
			CMD_S_OperateResult OperateResult;
			OperateResult.wOperateUser=wTargetUser;
			OperateResult.wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;
			OperateResult.cbOperateCode=WIK_CHI_HU;
			OperateResult.cbOperateCard=cbOperateCard;

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));


			//������Ϸ
			ASSERT(m_ChiHuResult[wTargetUser].wChiHuKind!=CHK_NULL);
			OnEventGameEnd(m_wProvideUser,NULL,GER_NORMAL);

			return true;
		}

		//�û�״̬
		ZeroMemory(m_bResponse,sizeof(m_bResponse));
		ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
		ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
		ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

		//����˿�
		ASSERT(m_cbWeaveItemCount[wTargetUser]<4);
		WORD wIndex=m_cbWeaveItemCount[wTargetUser]++;
		m_WeaveItemArray[wTargetUser][wIndex].cbPublicCard=TRUE;
		m_WeaveItemArray[wTargetUser][wIndex].cbCenterCard=cbTargetCard;
		m_WeaveItemArray[wTargetUser][wIndex].cbWeaveKind=cbTargetAction;
		m_WeaveItemArray[wTargetUser][wIndex].wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;

		//ɾ���˿�
		switch (cbTargetAction)
		{
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
				}
				else
				{
					//ɾ���˿�
					BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard,cbTargetCard};
					m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));
				}

				//���ƴ���
				m_cbGangCount++;
				m_cbRemainCardCount += 2;


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
			//m_bEnjoinChiPeng[wTargetUser]=true;
		}

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

		//�����û�
		m_wCurrentUser=wTargetUser;

		//���ƴ���
		if (cbTargetAction==WIK_GANG)
		{
			//Ч�鶯��
			bool bAroseAction=EstimateUserRespond(wTargetUser,cbTargetCard,EstimatKind_GangCard);

			//�����˿�
			if (bAroseAction==false) 
				DispatchCardData(wTargetUser,true);

			return true;
		}

		//�����ж�
		if (m_cbLeftCardCount>=m_cbRemainCardCount+1)
		{
			//�����ж�
			tagGangCardResult GangCardResult;
			m_cbUserAction[m_wCurrentUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[m_wCurrentUser],
				m_WeaveItemArray[m_wCurrentUser],m_cbWeaveItemCount[m_wCurrentUser],GangCardResult);

			//�������
			if (GangCardResult.cbCardCount>0)
			{
				//���ñ���
				m_cbUserAction[m_wCurrentUser]|=WIK_GANG;

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
		ASSERT((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)!=0));
		if ((cbOperateCode==WIK_NULL)||((m_cbUserAction[wChairID]&cbOperateCode)==0)) return false;

		//�˿�Ч��
		ASSERT((cbOperateCode==WIK_NULL)||(cbOperateCode==WIK_CHI_HU)||(m_GameLogic.IsValidCard(cbOperateCard)==true));
		if ((cbOperateCode!=WIK_NULL)&&(cbOperateCode!=WIK_CHI_HU)&&(m_GameLogic.IsValidCard(cbOperateCard)==false)) return false;

		//���ñ���
		m_bSendStatus=true;
		m_cbUserAction[wChairID]=WIK_NULL;
		m_cbPerformAction[wChairID]=WIK_NULL;

		//ִ�ж���
		switch (cbOperateCode)
		{
		case WIK_GANG:			//���Ʋ���
			{
				//��������
				BYTE cbWeaveIndex=0xFF;
				BYTE cbCardIndex=m_GameLogic.SwitchToCardIndex(cbOperateCard);
				bool bpublic=true;

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

					//����˿�
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=TRUE;
					//m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=cbOperateCode;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard;
					bpublic=true;
				}
				else
				{
					//�˿�Ч��
					ASSERT(m_cbCardIndex[wChairID][cbCardIndex]==4);
					if (m_cbCardIndex[wChairID][cbCardIndex]!=4) return false;

					//���ñ���
					cbWeaveIndex=m_cbWeaveItemCount[wChairID]++;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=FALSE;
					m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=cbOperateCode;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard;
					bpublic=false;
				}


				//���ƴ���
				m_cbGangCount++;
				m_cbRemainCardCount += 2;
	

				//ɾ���˿�
				m_cbCardIndex[wChairID][cbCardIndex]=0;

				//����״̬
				if (cbOperateCode==WIK_GANG)
				{
					m_bGangStatus=true;
					//m_bEnjoinChiPeng[wChairID]=true;
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

				bool bAroseAction=false;
				//Ч�鶯��
				if(bpublic)
					 bAroseAction=EstimateUserRespond(wChairID,cbOperateCard,EstimatKind_GangCard);

				//�����˿�
				if (bAroseAction==false)
					DispatchCardData(wChairID,true);

				return true;
			}
		case WIK_CHI_HU:		//�Ժ�����
			{
				//����Ȩλ
				WORD wChiHuRight=0;

				//��ͨ����
				BYTE cbWeaveItemCount=m_cbWeaveItemCount[wChairID];
				tagWeaveItem * pWeaveItem=m_WeaveItemArray[wChairID];
				
				//Ϊ���ж�����
				//��ɾ��
				m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],m_cbSendCardData);
				m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wChairID],pWeaveItem,cbWeaveItemCount,m_cbSendCardData,wChiHuRight,m_ChiHuResult[wChairID],(m_wBankerUser-wChairID+GAME_PLAYER)%GAME_PLAYER,false,true);
				m_cbCardIndex[wChairID][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

				//������Ϣ
				m_cbChiHuCard=m_cbProvideCard;
			
				//������
				CMD_S_OperateResult OperateResult;
				OperateResult.wOperateUser=wChairID;
				OperateResult.wProvideUser=wChairID;
				OperateResult.cbOperateCode=WIK_CHI_HU;
				OperateResult.cbOperateCard=cbOperateCard;

				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));


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

//�ɷ��˿�
bool CTableFrameSink::DispatchCardData(WORD wCurrentUser,bool bGang)
{
	//״̬Ч��
	ASSERT(wCurrentUser!=INVALID_CHAIR);
	if (wCurrentUser==INVALID_CHAIR) return false;	
	
	//�����˿�
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		m_cbDiscardCount[m_wOutCardUser]++;
		m_cbDiscardCard[m_wOutCardUser][m_cbDiscardCount[m_wOutCardUser]-1]=m_cbOutCardData;
	}
	//�����ж�
	if ((m_cbLeftCardCount<=m_cbRemainCardCount+1) && bGang)
	{
		//�����˿�
		m_cbSendCardCount++;
		m_cbSendCardData=m_cbRepertoryCard[--m_cbLeftCardCount];

		//�жϻ���
		if(m_cbLeftCardCount>0)  
		{

			//�жϻ���
			while ((m_cbSendCardData>=0x38)&&(m_cbSendCardData<=0x3F))
			{
				ASSERT(m_cbSendCardData>=0x38&&m_cbSendCardData<=0x3F);

				//ǿ�Ƴ���
				if (m_GameLogic.IsValidCard(m_cbSendCardData)==false) return false;

				//��������
				m_cbCardIndex[wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

				//��������
				CMD_S_SendCard SendCard;
				SendCard.wCurrentUser=wCurrentUser;
				SendCard.cbActionMask=m_cbUserAction[wCurrentUser];
				SendCard.cbCardData=m_cbSendCardData;
				SendCard.bGang =true;

				//��������
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_FORCE_SEND_CARD,&SendCard,sizeof(SendCard));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_FORCE_SEND_CARD,&SendCard,sizeof(SendCard));

				//ɾ���˿�
				if (m_GameLogic.RemoveCard(m_cbCardIndex[wCurrentUser],m_cbSendCardData)==false)
				{
					ASSERT(FALSE);
					return false;
				}

				//���Ƽ�¼
				m_cbOutCardCount++;
				m_wOutCardUser=wCurrentUser;
				m_cbOutCardData=m_cbSendCardData;
				m_cbAllWindCount +=1;			
				//m_cbWindData[wCurrentUser][m_cbTempWinCount[wCurrentUser]++] = m_cbSendCardData;
				m_cbUserWindData[wCurrentUser][m_cbUserWindCount[wCurrentUser]++] = m_cbSendCardData;


				//��������
				CMD_S_OutCard OutCard;
				OutCard.wOutCardUser=wCurrentUser;
				OutCard.cbOutCardData=m_cbSendCardData;

				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_FORCE_OUT_CARD,&OutCard,sizeof(OutCard));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_FORCE_OUT_CARD,&OutCard,sizeof(OutCard));	

				//���·���
				m_cbSendCardCount++;
				m_cbSendCardData=m_cbRepertoryCard[--m_cbLeftCardCount];
			}
		}
	}
	
	if((m_cbLeftCardCount<=m_cbRemainCardCount+1) &&bGang==false)
	{
		//���ñ���
		m_wResumeUser=wCurrentUser;
		m_wCurrentUser=wCurrentUser;
		m_wProvideUser=INVALID_CHAIR;
		m_cbProvideCard=m_cbSendCardData;
		OnEventGameEnd(m_wProvideUser,NULL,GER_NORMAL);
		return true;
	}

	//��ׯ����
	if (m_cbLeftCardCount==0)
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
		
		//�жϻ���
		if(m_cbLeftCardCount>0)  ////��ʱ���ܻ��и�
		{

			//�жϻ���
			while ((m_cbSendCardData>=0x38)&&(m_cbSendCardData<=0x3F))
			{
				ASSERT(m_cbSendCardData>=0x38&&m_cbSendCardData<=0x3F);

				//ǿ�Ƴ���
				if (m_GameLogic.IsValidCard(m_cbSendCardData)==false) return false;

				//��������
				m_cbCardIndex[wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

				//��������
				CMD_S_SendCard SendCard;
				SendCard.wCurrentUser=wCurrentUser;
				SendCard.cbActionMask=m_cbUserAction[wCurrentUser];
				SendCard.cbCardData=m_cbSendCardData;
				SendCard.bGang=true;

				//��������
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_FORCE_SEND_CARD,&SendCard,sizeof(SendCard));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_FORCE_SEND_CARD,&SendCard,sizeof(SendCard));

				//ɾ���˿�
				if (m_GameLogic.RemoveCard(m_cbCardIndex[wCurrentUser],m_cbSendCardData)==false)
				{
					ASSERT(FALSE);
					return false;
				}

				//���Ƽ�¼
				m_cbOutCardCount++;
				m_wOutCardUser=wCurrentUser;
				m_cbOutCardData=m_cbSendCardData;
				m_cbAllWindCount +=1;
				m_cbUserWindData[wCurrentUser][m_cbUserWindCount[wCurrentUser]++] = m_cbSendCardData;
	
				//״̬�ж�
				if (m_pITableFrame->GetGameStatus()== GS_MJ_PLAY)
				{
					//�����ж�
					if(m_cbAllWindCount<8)
						m_cbRemainCardCount = 14 + (m_cbGangCount+m_cbAllWindCount)%2+m_cbGangCount*2;
					else
						m_cbRemainCardCount = 16 + (m_cbGangCount+m_cbAllWindCount)%2+m_cbGangCount*2;
				}

				//��������
				CMD_S_OutCard OutCard;
				OutCard.wOutCardUser=wCurrentUser;
				OutCard.cbOutCardData=m_cbSendCardData;

				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_FORCE_OUT_CARD,&OutCard,sizeof(OutCard));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_FORCE_OUT_CARD,&OutCard,sizeof(OutCard));

				//���·���
				m_cbSendCardCount++;
				m_cbSendCardData=m_cbRepertoryCard[--m_cbLeftCardCount];
			}

		}

		m_cbCardIndex[wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

		//���ñ���
		m_wProvideUser=wCurrentUser;
		m_cbProvideCard=m_cbSendCardData;
		
		tagGangCardResult GangCardResult;
		m_cbUserAction[wCurrentUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[wCurrentUser],
				m_WeaveItemArray[wCurrentUser],m_cbWeaveItemCount[wCurrentUser],GangCardResult);

		//����Ȩλ
		WORD wChiHuRight=0;

		//�����ж�
		tagChiHuResult ChiHuResult;
		m_cbUserAction[wCurrentUser]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wCurrentUser],m_WeaveItemArray[wCurrentUser],m_cbWeaveItemCount[wCurrentUser],0,wChiHuRight,ChiHuResult,(m_wBankerUser-wCurrentUser+GAME_PLAYER)%GAME_PLAYER,true,true);
	}

	//���ñ���
	m_wCurrentUser=wCurrentUser;

	//��������
	CMD_S_SendCard SendCard;
	SendCard.wCurrentUser=wCurrentUser;
	SendCard.cbActionMask=m_cbUserAction[wCurrentUser];
	SendCard.cbCardData=(m_bSendStatus==true)?m_cbSendCardData:0x00;

	SendCard.bGang = bGang;

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
		if (wCenterUser==i) 		continue;

		//��������
		if (EstimatKind==EstimatKind_OutCard)
		{
			//�����ж�
			if (m_bEnjoinChiPeng[i]==false&&m_cbLeftCardCount>=m_cbRemainCardCount+1)
			{
				//�����ж�
				m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard);

	

				//�����ж�
				WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
				if (wEatUser==i)
					m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard);
			}
			m_cbUserAction[i]|=m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard);
		}

		//�����ж�
		if (m_bEnjoinChiHu[i]==false)
		{
			//����Ȩλ
			WORD wChiHuRight=0;

			//�Ժ��ж�
			tagChiHuResult ChiHuResult;
			BYTE cbWeaveCount=m_cbWeaveItemCount[i];
			if(m_cbUserWindCount[i] == 8)
			{
				m_cbUserAction[i]|=WIK_CHI_HU;
			}
			BYTE cbHandTai=0;
			BYTE cbHandFeng = 0;
			if(m_cbEnjoinHuCard[i] != cbCenterCard)
			{
				m_cbUserAction[i]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],m_WeaveItemArray[i],cbWeaveCount,cbCenterCard,wChiHuRight,ChiHuResult,(m_wBankerUser-i+GAME_PLAYER)%GAME_PLAYER,true,false);
			}

           
			//��ֹ����
			if(m_cbUserAction[i]&WIK_CHI_HU)
			{
				m_cbEnjoinHuCard[i]=cbCenterCard;
			}
            
		}

		//����ж�
		if (m_cbUserAction[i]!=WIK_NULL) 
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
//�ɷ��˿�
bool CTableFrameSink::ForceDispatchCardData(WORD wCurrentUser)
{
	//״̬Ч��
	ASSERT(wCurrentUser!=INVALID_CHAIR);
	if (wCurrentUser==INVALID_CHAIR) return false;
	
	//�����˿�
	m_cbSendCardCount++;
	m_cbSendCardData=m_cbRepertoryCard[--m_cbLeftCardCount];

	//�жϻ���
	if (m_cbSendCardData>=0x38&&m_cbSendCardData<=0x3F)
	{
		ASSERT(m_cbSendCardData>=0x38&&m_cbSendCardData<=0x3F);
		m_cbSumWindCount +=1;
		m_cbWindData[wCurrentUser][m_cbTempWinCount[wCurrentUser]++] = m_cbSendCardData;
		m_cbAllWindCount += 1;


	}
	m_cbCardIndex[wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

	//���ñ���
	m_wProvideUser=wCurrentUser;
	m_cbProvideCard=m_cbSendCardData;

	//���ñ���
	m_wCurrentUser=wCurrentUser;

	//��������
	CMD_S_SendCard SendCard;
	SendCard.wCurrentUser=wCurrentUser;
	SendCard.cbActionMask=m_cbUserAction[wCurrentUser];
	SendCard.cbCardData=m_cbSendCardData;
	SendCard.bGang = true;

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_FORCE_SEND_CARD,&SendCard,sizeof(SendCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_FORCE_SEND_CARD,&SendCard,sizeof(SendCard));

	return true;
}
//ǿ�ȳ���
bool CTableFrameSink::ForceOutCardData(WORD wCurrentUser,BYTE cbCardData)
{
	ASSERT(m_wCurrentUser != wCurrentUser);

	if(m_wCurrentUser != wCurrentUser) return false;
	return true;

}

//���3�� ����
BYTE CTableFrameSink::IsAfford(WORD wUserProvider,WORD wUserAccept)
{
	//У������
	ASSERT(wUserProvider<4);
	ASSERT(wUserProvider>=0);
	ASSERT(wUserAccept<4);
	ASSERT(wUserAccept>=0);

	if(m_cbWeaveItemCount[wUserAccept]<3) return 1;

	BYTE cbAfford=0;//1�����ǰ���ϵ 2������ϵ
	BYTE cbCount[4] = {0,0,0,0};
	//������
	for (BYTE cbTemp =0;cbTemp<m_cbWeaveItemCount[wUserAccept];cbTemp++)
	{
			cbCount[(m_WeaveItemArray[wUserAccept][cbTemp].cbCenterCard&MASK_COLOR)>>4]++;
	}
	if((cbCount[0]+cbCount[3]>=3)||(cbCount[1]+cbCount[3]>=3) ||(cbCount[2]+cbCount[3]>=3))
		cbAfford+=2;

	return cbAfford>1?cbAfford:1;
}

//�������
bool CTableFrameSink::OnUserCallFinish(WORD wChairID)
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

	//�û�Ч��
	ASSERT(m_bCallCard[wChairID]==false);
	if (m_bCallCard[wChairID]==true) return false;

	//��������
	m_bCallCard[wChairID]=true;

	//����ж�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_bCallCard[i]==false) break;
	}
	if (i!=m_wPlayerCount) return true;
	//�����滻
	do 
	{
		for (WORD i=0;i<m_wPlayerCount;i++)
		{	
			//�û�����
			WORD wCurrentUser = (m_wBankerUser-i+m_wPlayerCount)%m_wPlayerCount;

			//����i����
			for(BYTE j=0;j<m_cbWindCount[wCurrentUser];j++)
			{
				BYTE cbCardData = m_cbWindData[wCurrentUser][j];

				if (m_GameLogic.IsValidCard(cbCardData)==false)	return false;

				ASSERT(cbCardData>=0x38&&cbCardData<=0x3F);

				//ɾ���˿�
				if (m_GameLogic.RemoveCard(m_cbCardIndex[wCurrentUser],cbCardData)==false)
				{
					ASSERT(FALSE);
					return false;
				}

				//���Ƽ�¼
				m_cbOutCardCount++;
				m_wOutCardUser=wCurrentUser;
				m_cbOutCardData=cbCardData;

				//��������
				CMD_S_OutCard OutCard;
				OutCard.wOutCardUser=wCurrentUser;
				OutCard.cbOutCardData=cbCardData;

				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_FORCE_OUT_CARD,&OutCard,sizeof(OutCard));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_FORCE_OUT_CARD,&OutCard,sizeof(OutCard));
				m_cbUserWindData[wCurrentUser][m_cbUserWindCount[wCurrentUser]++] = cbCardData;

				//�滻����
				if(ForceDispatchCardData(wCurrentUser) ==false)
				{
					ASSERT(FALSE);
					return false;
				}
			}
			m_cbSumWindCount -= m_cbWindCount[wCurrentUser];
			m_cbWindCount[wCurrentUser] = m_cbTempWinCount[wCurrentUser];
			m_cbTempWinCount[wCurrentUser]=0;

		}
	} while(m_cbSumWindCount>0);

	//��������
	bool bAroseAction=false;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//ׯ���ж�
		if (i==m_wBankerUser)
		{
			//�����ж�
			tagGangCardResult GangCardResult;
			m_cbUserAction[i]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[i],NULL,0,GangCardResult);

			//�����ж�
			tagChiHuResult ChiHuResult;
			m_cbUserAction[i]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],NULL,0,0,0,ChiHuResult,0,true,true);
		}

		//״̬����
		if ((bAroseAction==false)&&(i!=m_wBankerUser)&&(m_cbUserAction[i]!=WIK_NULL))
		{
			bAroseAction=true;
			m_wResumeUser=m_wCurrentUser;
			m_wCurrentUser=INVALID_CHAIR;
		}
	}


	//��ǰ�û�
	m_wCurrentUser=m_wBankerUser;

	CMD_S_GameStart GameStart;

	//��������
	ZeroMemory(&GameStart,sizeof(GameStart));
	GameStart.wSiceCount=m_wSiceCount;
	GameStart.wBankerUser=m_wBankerUser;
	GameStart.wCurrentUser=m_wCurrentUser;
	GameStart.cbSunWindCount=m_cbSumWindCount;
	GameStart.cbLeftCardCount = m_cbLeftCardCount;
	GameStart.bFirst=false;
	CopyMemory(GameStart.bTrustee,m_bTrustee,sizeof(m_bTrustee));

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

//////////////////////////////////////////////////////////////////////////
