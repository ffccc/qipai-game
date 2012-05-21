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
	//�߼�����
	m_cbPackCount=1;
	m_cbMainColor=COLOR_ERROR;
	m_wFirstUser=0;
	m_bLandScore=250;
	m_cbJuKuang=0;
	m_cbMultiples=1;

	//������Ϣ
	m_cbValueOrder[0]=VALUE_ERROR;
	m_cbValueOrder[1]=VALUE_ERROR;

	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//������Ϣ
	m_cbCallCard=0;
	m_cbCallCount=0;
	m_wCallCardUser=INVALID_CHAIR;

	//״̬����
	m_cbScoreCardCount=0;
	ZeroMemory(m_bCallCard,sizeof(m_bCallCard));
	ZeroMemory(m_cbScoreCardData,sizeof(m_cbScoreCardData));

	//���Ʊ���
	m_wTurnWinner=INVALID_CHAIR;
	m_wFirstOutUser=INVALID_CHAIR;
	ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
	ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));

	//�����˿�
	m_cbConcealCount=0;
	ZeroMemory(m_cbConcealCard,sizeof(m_cbConcealCard));

	//�û��˿�
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));

	//�����˿�
	ZeroMemory(m_cbTieCardData,sizeof(m_cbTieCardData));
	ZeroMemory(m_cbTieCardCount,sizeof(m_cbTieCardCount));

	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));

	//���ֿ���
	ZeroMemory(bHandOutFlag,sizeof(bHandOutFlag));

	//�зּ�¼
	memset(m_bColorInfo,1,sizeof(m_bScoreInfo));
	memset(m_bScoreInfo,0,sizeof(m_bScoreInfo));

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
	//�߼�����
	m_cbPackCount=1;
	m_cbMainColor=COLOR_ERROR;

	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	//m_wFirstUser=INVALID_CHAIR;

	//������Ϣ
	m_cbCallCard=0;
	m_cbCallCount=0;
	m_wCallCardUser=INVALID_CHAIR;
	m_bLandScore=250;
	m_cbJuKuang=0;
	m_cbMultiples=1;

	//״̬����
	m_cbScoreCardCount=0;
	ZeroMemory(m_bCallCard,sizeof(m_bCallCard));
	ZeroMemory(m_cbScoreCardData,sizeof(m_cbScoreCardData));

	//���Ʊ���
	m_wTurnWinner=INVALID_CHAIR;
	m_wFirstOutUser=INVALID_CHAIR;
	ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
	ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));

	//�����˿�
	m_cbConcealCount=0;
	ZeroMemory(m_cbConcealCard,sizeof(m_cbConcealCard));

	//�û��˿�
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));

	//�����˿�
	ZeroMemory(m_cbTieCardData,sizeof(m_cbTieCardData));
	ZeroMemory(m_cbTieCardCount,sizeof(m_cbTieCardCount));

	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));

	//���ֿ���
	ZeroMemory(bHandOutFlag,sizeof(bHandOutFlag));

	//�зּ�¼
	memset(m_bColorInfo,1,sizeof(m_bScoreInfo));
	memset(m_bScoreInfo,0,sizeof(m_bScoreInfo));

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
	//m_pITableFrame->SetGameStatus(GS_UG_CALL);
	m_pITableFrame->SetGameStatus(GS_UG_SCORE);

	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	
	//���ñ���
	m_cbMainColor=COLOR_ERROR;

	//���ֿ���
	ZeroMemory(bHandOutFlag,sizeof(bHandOutFlag));

	//�зּ�¼
	memset(m_bColorInfo,1,sizeof(m_bScoreInfo));
	memset(m_bScoreInfo,0,sizeof(m_bScoreInfo));

	//��������
	m_GameLogic.SetMainColor(m_cbMainColor);
	m_GameLogic.SetPackCount(m_cbPackCount);
	SetJuKuang(m_wFirstUser);
	m_cbMultiples=1;

	//������ʷ�з���Ϣ
	switch (m_cbJuKuang)
	{
	case 2:
		{
			m_bScoreInfo[0][0]=240;
			m_bColorInfo[0][0]=240;
			break;
		}
	case 3:
		{
			m_bScoreInfo[0][0]=240;
			m_bColorInfo[0][0]=240;
			m_bScoreInfo[1][0]=240;
			m_bColorInfo[1][0]=240;
			break;
		}
	case 4:
		{
			m_bScoreInfo[0][0]=240;
			m_bColorInfo[0][0]=240;
			m_bScoreInfo[1][0]=240;
			m_bColorInfo[1][0]=240;
			m_bScoreInfo[2][0]=240;
			m_bColorInfo[2][0]=240;
			break;
		}
	}

	//��������
	CMD_S_GameStart GameStart;
	GameStart.wBankerUser=m_wBankerUser;
	GameStart.cbPackCount=m_cbPackCount;
	GameStart.cbValueOrder[0]=0;
	GameStart.cbValueOrder[1]=0;
	GameStart.cbJuKuang=m_cbJuKuang;
	
	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));

	//�����˿�
	ZeroMemory(m_cbTieCardData,sizeof(m_cbTieCardData));
	ZeroMemory(m_cbTieCardCount,sizeof(m_cbTieCardCount));

	//�����˿�
	DispatchUserCard();

	return true;
}

//��Ϸ����
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_DISMISS:		//��Ϸ��ɢ
		{
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			m_cbValueOrder[0]=VALUE_ERROR;
			m_cbValueOrder[1]=VALUE_ERROR;

			//������Ϸ
			m_pITableFrame->ConcludeGame();

			return true;
		}
	case GER_NORMAL:		//�������
		{
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//ͳ�Ʊ���
			LONG lBankerScore[2];
			BYTE bImp=0;

			//0Ϊׯ�� 1Ϊ�Լҷ�
			lBankerScore[0]=0;
			lBankerScore[1]=0;

			//-------------------------------------------------------------------------------
			//�ж��Ƿ���ɶ�Լ�� δ��ɶ�Լ
			if ((m_bLandScore+6)>m_cbValueOrder[0])
			{
				LONG lFirstDunValue=0;  //��һ��
				LONG lNextDunValue=0;   //1�պ�ÿ��
				LONG lFouthDunValue=0;  //��4�պ��Ժ�
				BYTE bHaveDun=((m_bLandScore+6)-m_cbValueOrder[0]);  //������
				
				//Ч�����
				ASSERT(m_cbJuKuang!=0);
				if (m_cbJuKuang==0) return false;
				if (m_cbJuKuang!=1) //˫�У��ϱ�������
				{
					switch (m_cbMultiples)
					{
					case 1:
						{
							lFirstDunValue=100;
							lNextDunValue=100;
							lFouthDunValue=0;
							break;
						}
					case 2:
						{
							lFirstDunValue=200;
							lNextDunValue=300;
							lFouthDunValue=0;
							break;
						}
					case 6:
						{
							lFirstDunValue=400;
							lNextDunValue=600;
							lFouthDunValue=0;
							break;
						}
					}
				}
				else  //˫��
				{
					switch (m_cbMultiples)
					{
					case 1:
						{
							lFirstDunValue=50;
							lNextDunValue=50;
							lFouthDunValue=0;
							break;
						}
					case 2:
						{
							lFirstDunValue=100;
							lNextDunValue=200;
							lFouthDunValue=100;
							break;
						}
					case 6:
						{
							lFirstDunValue=200;
							lNextDunValue=400;
							lFouthDunValue=200;
							break;
						}
					}
				}

				LONG lHaveValue=0; //���ռ����÷�
				if (bHaveDun<4)   //��������4��
				{
					lHaveValue=lFirstDunValue+(bHaveDun-1)*lNextDunValue;
				}
				else
				{
					BYTE bLostDun=bHaveDun-4+1;  
					lHaveValue=lFirstDunValue+2*lNextDunValue+bLostDun*lFouthDunValue;
				}

				//ͳ�Ƶ÷�
				lBankerScore[0]=-lHaveValue;
				lBankerScore[1]=lHaveValue;
			}
			//-------------------------------------------------------------------------------
			else  //��ɶ�Լ
			{
				LONG lFirstCardValue=0;
				LONG lNextCardValue=0;
				bool bIsFinishJu=false;  //�ɾֱ�־  false=���� true=�ɾ�
				
				//���㻨ɫ�ķ�ֵ
				switch (m_cbMainColor)
				{
				case COLOR_MEI_HUA:
				case COLOR_FANG_KUAI:
					{
						switch (m_cbMultiples)
						{
						case 1:
							{
								lFirstCardValue=20;
								lNextCardValue=20;
								break;
							}
						case 2:
							{
								lFirstCardValue=40;
								lNextCardValue=40;
								break;
							}
						case 6:
							{
								lFirstCardValue=80;
								lNextCardValue=80;
								break;
							}
						}
						break;
					}
				case COLOR_HONG_TAO:
				case COLOR_HEI_TAO:
					{
						switch (m_cbMultiples)
						{
						case 1:
							{
								lFirstCardValue=30;
								lNextCardValue=30;
								break;
							}
						case 2:
							{
								lFirstCardValue=40;
								lNextCardValue=40;
								break;
							}
						case 6:
							{
								lFirstCardValue=120;
								lNextCardValue=120;
								break;
							}
						}
						break;
					}
				case COLOR_NT:
					{
						switch (m_cbMultiples)
						{
						case 1:
							{
								lFirstCardValue=40;
								lNextCardValue=30;
								break;
							}
						case 2:
							{
								lFirstCardValue=80;
								lNextCardValue=60;
								break;
							}
						case 6:
							{
								lFirstCardValue=160;
								lNextCardValue=120;
								break;
							}
						}
						break;
					}
				}
				
				//�ж��Ƿ�ɾ�
				LONG bDingYueValue=lFirstCardValue+((m_bLandScore-1)*lNextCardValue);
				if (bDingYueValue>=100) bIsFinishJu=true;

				//��������
				if (m_bLandScore==6||m_bLandScore==7)
				{
					//Ч�����
					ASSERT(m_cbJuKuang==0);
					if (m_cbJuKuang==0) return false;
					if (m_cbJuKuang!=1) //˫�У��ϱ�������
					{
						lBankerScore[0]=(m_bLandScore==6)?750:1500;
					}
					else  //˫��
					{
						lBankerScore[0]=(m_bLandScore==6)?500:1000;
					}

					//����÷�
					lBankerScore[0]+=bDingYueValue;
					lBankerScore[1]=-lBankerScore[0];
				}
				else
				{
					//���㳬��
					BYTE bChaoDun=m_cbValueOrder[0]-(m_bLandScore+6);
					LONG bChaoDunValue=0;
					if (bChaoDun!=0)
					{
						//Ч�����
						//ASSERT(m_cbJuKuang==0);
						if (m_cbJuKuang==0) return false;
						if (m_cbJuKuang!=1) //˫�У��ϱ�������
						{
							switch (m_cbMultiples)
							{
							case 1:
								{
									switch (m_cbMainColor)
									{
										case COLOR_MEI_HUA:
										case COLOR_FANG_KUAI:
										{
											bChaoDunValue=20;	
											break;
										}
										case COLOR_HONG_TAO:
										case COLOR_HEI_TAO:
										{
											bChaoDunValue=30;		
											break;
										}
										case COLOR_NT:
										{
											bChaoDunValue=30;		
											break;
										}
									}
								}
							case 2:
								{
									bChaoDunValue=200;
									break;
								}
							case 6:
								{
									bChaoDunValue=400;
									break;
								}

							}
						}
						else  //˫��
						{
							switch (m_cbMultiples)
							{
							case 1:
								{
									switch (m_cbMainColor)
									{
										case COLOR_MEI_HUA:
										case COLOR_FANG_KUAI:
										{
											bChaoDunValue=20;	
											break;
										}
										case COLOR_HONG_TAO:
										case COLOR_HEI_TAO:
										{
											bChaoDunValue=30;		
											break;
										}
										case COLOR_NT:
										{
											bChaoDunValue=30;		
											break;
										}
									}
								}
							case 2:
								{
									bChaoDunValue=100;
									break;
								}
							case 6:
								{
									bChaoDunValue=200;
									break;
								}

							}
						}
					//------------------------------------------------
					//����÷�
					lBankerScore[0]+=(bDingYueValue+bChaoDun*bChaoDunValue);
					}
					
					//��Լ����
					LONG lDingYueScore=0;

					//Ч�����
					//ASSERT(m_cbJuKuang==0);
					if (m_cbJuKuang==0) return false;
					
					//�оֳɾ֣��޾ֳɾ֣����ֶ�Լ
					if (bIsFinishJu)
					{
						if (m_cbJuKuang!=1) //˫�У��ϱ�������
						{
							lDingYueScore=500;
						}
						else  //˫��
						{
							lDingYueScore=300;
						}
					}
					else
					{
						lDingYueScore=50;
					}
					
					//�ӱ����ټӱ�
					if (m_cbMultiples==2) lDingYueScore+=50;
					if (m_cbMultiples==6) lDingYueScore+=100;

					//�����ܷ�
					lBankerScore[0]+=lDingYueScore;
					lBankerScore[1]=-lBankerScore[0];
				}
			}


			//-------------------------------------------------------------------------------
			
			//IMP����ɻ���
			LONG lTempImp=0;
			BYTE bTiePoint=0;

			lTempImp=abs(lBankerScore[0]);
			if (lTempImp>=0&&lTempImp<50) bImp=1;
			if (lTempImp>=50&&lTempImp<90) bImp=2;
			if (lTempImp>=90&&lTempImp<130) bImp=3;
			if (lTempImp>=130&&lTempImp<170) bImp=4;
			if (lTempImp>=170&&lTempImp<220) bImp=5;
			if (lTempImp>=220&&lTempImp<270) bImp=6;
			if (lTempImp>=270&&lTempImp<320) bImp=7;
			if (lTempImp>=320&&lTempImp<370) bImp=8;
			if (lTempImp>=370&&lTempImp<430) bImp=9;
			if (lTempImp>=430&&lTempImp<500) bImp=10;
			if (lTempImp>=500&&lTempImp<600) bImp=11;
			if (lTempImp>=600&&lTempImp<750) bImp=12;
			if (lTempImp>=750&&lTempImp<900) bImp=13;
			if (lTempImp>=900&&lTempImp<1100) bImp=14;
			if (lTempImp>=1100&&lTempImp<1300) bImp=15;
			if (lTempImp>=1300&&lTempImp<1500) bImp=16;
			if (lTempImp>=1500&&lTempImp<1750) bImp=17;
			if (lTempImp>=1750&&lTempImp<2000) bImp=18;
			if (lTempImp>=2000&&lTempImp<2250) bImp=19;
			if (lTempImp>=2250&&lTempImp<2500) bImp=20;
			if (lTempImp>=2500&&lTempImp<3000) bImp=21;
			if (lTempImp>=3000&&lTempImp<3500) bImp=22;
			if (lTempImp>=3500&&lTempImp<4000) bImp=23;
			if (lTempImp>=4000) bImp=24;

			//����÷ַ��Ĵ���ֵ
			if (lBankerScore[0]>0)
			{
				bTiePoint=GetBigCardPoint(m_wBankerUser,(m_wBankerUser+2)%m_wPlayerCount);
			}
			else
			{
				bTiePoint=GetBigCardPoint((m_wBankerUser+1)%m_wPlayerCount,(m_wBankerUser+3)%m_wPlayerCount);
			}

			//���÷�
			BYTE bTotalScore=bImp+20-bTiePoint;
			LONG bNSScore=0;  //�ϱ��÷�
			LONG bEWScore=0;  //�����÷�
			if (lBankerScore[0]>0)
			{
				//��ҵ÷�
				GameEnd.lScore[m_wBankerUser]=bTotalScore*m_pGameServiceOption->lCellScore;
				GameEnd.lScore[(m_wBankerUser+2)%m_wPlayerCount]=bTotalScore*m_pGameServiceOption->lCellScore;
				GameEnd.lScore[(m_wBankerUser+1)%m_wPlayerCount]=-(bTotalScore*m_pGameServiceOption->lCellScore);
				GameEnd.lScore[(m_wBankerUser+3)%m_wPlayerCount]=-(bTotalScore*m_pGameServiceOption->lCellScore);
				bEWScore=-bTotalScore;
				bNSScore=bTotalScore;
			}
			else
			{
				//��ҵ÷�
				GameEnd.lScore[m_wBankerUser]=-(bTotalScore*m_pGameServiceOption->lCellScore);
				GameEnd.lScore[(m_wBankerUser+2)%m_wPlayerCount]=-(bTotalScore*m_pGameServiceOption->lCellScore);
				GameEnd.lScore[(m_wBankerUser+1)%m_wPlayerCount]=bTotalScore*m_pGameServiceOption->lCellScore;
				GameEnd.lScore[(m_wBankerUser+3)%m_wPlayerCount]=bTotalScore*m_pGameServiceOption->lCellScore;
				bEWScore=bTotalScore;
				bNSScore=-bTotalScore;
			}

			//����˰��
			LONG lGameTax=0L;
			if (m_pGameServiceOption->wServerType==GAME_GENRE_GOLD)
			{
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					if (GameEnd.lScore[i]>=10L)
					{
						lGameTax+=GameEnd.lScore[i]/10L;
						GameEnd.lScore[i]=GameEnd.lScore[i]*45L/50L;
					}
				}
			}

			
			//-------------------------------------------------------------------------------
			char cFangWei[10];
			if (m_wBankerUser==0) strcpy(cFangWei,"��");
			if (m_wBankerUser==1) strcpy(cFangWei,"��");
			if (m_wBankerUser==2) strcpy(cFangWei,"��");
			if (m_wBankerUser==3) strcpy(cFangWei,"��");
			char cColor[10];
			if (m_cbMainColor==COLOR_NT) strcpy(cColor,"NT");
			if (m_cbMainColor==COLOR_HEI_TAO) strcpy(cColor,"����");
			if (m_cbMainColor==COLOR_HONG_TAO) strcpy(cColor,"����");
			if (m_cbMainColor==COLOR_FANG_KUAI) strcpy(cColor,"����");
			if (m_cbMainColor==COLOR_MEI_HUA) strcpy(cColor,"÷��");

			//֪ͨ��Ϣ
			TCHAR szMessage[256]=TEXT("");
			_snprintf(szMessage,CountArray(szMessage),TEXT("�����[ %s%d%s ]IMP��[ %d ] �ϱ��÷֣�[ %d ] �����÷֣�[ %d ] ��Ϸ˰�ʣ�[ %d ]"),cFangWei,m_bLandScore,cColor,lTempImp,bNSScore,bEWScore,lGameTax);
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

			//-------------------------------------------------------------------------------

			//�´η���
			if (m_wFirstUser!=INVALID_CHAIR) m_wFirstUser=(m_wFirstUser+1)%GAME_PLAYER;
			
			//��Ϸ����
			GameEnd.wConcealTime=m_cbMultiples;

			//˫���÷�
			GameEnd.bBankerScore=m_cbValueOrder[0];
			GameEnd.bOtherScore=m_cbValueOrder[1];

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//�޸Ļ���
			tagScoreInfo ScoreInfo[m_wPlayerCount];
			ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				ScoreInfo[i].lScore=GameEnd.lScore[i];
				ScoreInfo[i].ScoreKind=(GameEnd.lScore[i]>0L)?enScoreKind_Win:enScoreKind_Lost;
			}
			m_pITableFrame->WriteTableScore(ScoreInfo,m_wPlayerCount,lGameTax);

			m_cbValueOrder[0]=VALUE_ERROR;
			m_cbValueOrder[1]=VALUE_ERROR;

			//ׯ���л�
			if (GameEnd.lScore[m_wBankerUser]>0L) m_wBankerUser=(m_wBankerUser+2)%m_wPlayerCount;
			else m_wBankerUser=(m_wBankerUser+1)%m_wPlayerCount;

			//������Ϸ
			m_pITableFrame->ConcludeGame();

			return true;
		}
	case GER_USER_LEFT:		//�û�ǿ��
		{
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//��ҵ÷�
			GameEnd.lScore[wChairID]=-8*m_pGameServiceOption->lCellScore;
			GameEnd.bBankerScore=0;
			GameEnd.bOtherScore=0;
			GameEnd.wConcealTime=0;

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//�޸Ļ���
			tagScoreInfo ScoreInfo;
			ScoreInfo.ScoreKind=enScoreKind_Flee;
			ScoreInfo.lScore=GameEnd.lScore[wChairID];
			m_pITableFrame->WriteUserScore(wChairID,ScoreInfo);

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

			m_cbValueOrder[0]=VALUE_ERROR;
			m_cbValueOrder[1]=VALUE_ERROR;
			//������Ϸ
			m_pITableFrame->ConcludeGame();

			return true;
		}
	}

	ASSERT(FALSE);

	return false;
}

//���ͳ���
bool __cdecl CTableFrameSink::SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_UG_FREE:		//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//���ñ���
			StatusFree.lBaseScore=m_pGameServiceOption->lCellScore;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_UG_SCORE:		//�з�״̬
		{
			//��������
			CMD_S_StatusScore StatusScore;
			ZeroMemory(&StatusScore,sizeof(StatusScore));
			
        	//���ñ���
			StatusScore.bBossScore=m_bLandScore;
			StatusScore.wCurrentUser=m_wCurrentUser;
			
			//StatusScore.lBaseScore=m_pGameServiceOption->lCellScore;
	
			//CopyMemory(StatusScore.bScoreInfo,m_bScoreInfo,sizeof(m_bScoreInfo));
			//CopyMemory(StatusScore.bCardData,m_bHandCardData[wChiarID],m_bCardCount[wChiarID]);

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusScore,sizeof(StatusScore));
		}
	
	
	case GS_UG_PLAY:		//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//��Ϸ����
			StatusPlay.cbPackCount=m_cbPackCount;
			StatusPlay.cbMainColor=m_cbMainColor;
			StatusPlay.cbValueOrder[0]=m_cbValueOrder[0];
			StatusPlay.cbValueOrder[1]=m_cbValueOrder[1];

			//���Ʊ���
			StatusPlay.cbCallCard=m_cbCallCard;
			StatusPlay.cbCallCount=m_cbCallCount;
			StatusPlay.wCallCardUser=m_wCallCardUser;

			//�û�����
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.wFirstOutUser=m_wFirstOutUser;

			//�˿˱���
			StatusPlay.cbCardCount=m_cbHandCardCount[wChiarID];
			CopyMemory(StatusPlay.cbCardData,m_cbHandCardData[wChiarID],sizeof(BYTE)*m_cbHandCardCount[wChiarID]);

			//������Ϣ
			if (wChiarID==m_wBankerUser)
			{
				StatusPlay.cbConcealCount=m_cbConcealCount;
				CopyMemory(StatusPlay.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);
			}

			//���Ʊ���
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				StatusPlay.cbOutCardCount[i]=m_cbOutCardCount[i];
				CopyMemory(StatusPlay.cbOutCardData[i],m_cbOutCardData[i],sizeof(BYTE)*m_cbOutCardCount[i]);
			}

			//�÷ֱ���
			StatusPlay.cbScoreCardCount=m_cbScoreCardCount;
			CopyMemory(StatusPlay.cbScoreCardData,m_cbScoreCardData,sizeof(StatusPlay.cbScoreCardData));

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
	
	case SUB_C_CALL_SCORE:		//�û��з�
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_CallScore));
			if (wDataSize!=sizeof(CMD_C_CallScore)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;
			
			//��Ϣ����
			CMD_C_CallScore * pLandScore=(CMD_C_CallScore *)pDataBuffer;
			return OnUserLandScore(pUserData->wChairID,pLandScore->bBossScore,pLandScore->bBossColor);
			
		}
	case SUB_C_MULTIPLES:      //�û��ӱ�
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_OnMultiples));
			if (wDataSize!=sizeof(CMD_C_OnMultiples)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;
			
			//��Ϣ����
			CMD_C_OnMultiples * pMultiples=(CMD_C_OnMultiples *)pDataBuffer;
			return OnUserMultiples(pUserData->wChairID,pMultiples->bMultiples);
			
		}

	case SUB_C_OUT_CARD:		//�û�����
		{
			/*//��������
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pDataBuffer;
			WORD wHeadSize=sizeof(CMD_C_OutCard)-sizeof(pOutCard->cbCardData);

			//Ч������
			ASSERT(wDataSize>=wHeadSize);
			if (wDataSize<wHeadSize) return false;
			ASSERT(wDataSize==(wHeadSize+pOutCard->cbCardCount*sizeof(pOutCard->cbCardData[0])));
			if (wDataSize!=(wHeadSize+pOutCard->cbCardCount*sizeof(pOutCard->cbCardData[0]))) return false;
			*/

			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_OutCard));
			if (wDataSize!=sizeof(CMD_C_OutCard)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;
			
			//��Ϣ����
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pDataBuffer;
			return OnUserOutCard(pUserData->wChairID,pOutCard->cbCardData,pOutCard->cbCardCount,pOutCard->bIsHandOut,pOutCard->bIsAutoOut);
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
bool __cdecl CTableFrameSink::OnEventUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//���ñ���
	if (bLookonUser==false)
	{
		//�߼�����
		m_cbPackCount=1;
		m_cbMainColor=COLOR_ERROR;
		m_wFirstUser=INVALID_CHAIR;
		m_bLandScore=250;
		m_cbJuKuang=0;
		m_cbMultiples=1;

		//������Ϣ
		m_cbValueOrder[0]=VALUE_ERROR;
		m_cbValueOrder[1]=VALUE_ERROR;

		//��Ϸ����
		m_wBankerUser=INVALID_CHAIR;
		m_wCurrentUser=INVALID_CHAIR;

		//������Ϣ
		m_cbCallCard=0;
		m_cbCallCount=0;
		m_wCallCardUser=INVALID_CHAIR;

		//״̬����
		m_cbScoreCardCount=0;
		ZeroMemory(m_bCallCard,sizeof(m_bCallCard));
		ZeroMemory(m_cbScoreCardData,sizeof(m_cbScoreCardData));

		//���Ʊ���
		m_wTurnWinner=INVALID_CHAIR;
		m_wFirstOutUser=INVALID_CHAIR;
		ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
		ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));

		//�����˿�
		m_cbConcealCount=0;
		ZeroMemory(m_cbConcealCard,sizeof(m_cbConcealCard));

		//�û��˿�
		ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
		ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));

		//�����˿�
		ZeroMemory(m_cbTieCardData,sizeof(m_cbTieCardData));
		ZeroMemory(m_cbTieCardCount,sizeof(m_cbTieCardCount));

		memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));

		//���ֿ���
		ZeroMemory(bHandOutFlag,sizeof(bHandOutFlag));

		//�зּ�¼
		memset(m_bColorInfo,1,sizeof(m_bScoreInfo));
		memset(m_bScoreInfo,0,sizeof(m_bScoreInfo));

	}

	return true;
}

//�û��ӱ�
bool CTableFrameSink::OnUserMultiples(WORD wChairID,BYTE bMultiples)
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_UG_SCORE);
	if (m_pITableFrame->GetGameStatus()!=GS_UG_SCORE) return true;
	if (wChairID!=m_wCurrentUser) return false;
	if (bMultiples<1||bMultiples>6) return false;

	//���ñ���
	switch (m_cbMultiples)
	{
	case 1:
		{
			m_cbMultiples=2;
			break;
		}
	case 2:
		{
			m_cbMultiples=6;
			break;
		}
	}

	//�зּ�¼
	for (byte i=0;i<50;i++)
	{
		if (m_bScoreInfo[wChairID][i]==0)
		{
			m_bScoreInfo[wChairID][i]=(m_cbMultiples==2)?200:210;
			m_bColorInfo[wChairID][i]=(m_cbMultiples==2)?200:210;
			break;
		}
	}

	//�����û�
	ZeroMemory(m_bPlayStatus,sizeof(m_bPlayStatus));

	//˳ʱ��
	m_wCurrentUser=(wChairID+1)%m_wPlayerCount;

	//������Ϣ
	CMD_S_CallScore LandScore;
	LandScore.bCallScoreUser=wChairID;
	LandScore.wCurrentUser=m_wCurrentUser;
	LandScore.bCurrentScore=m_bLandScore;
	LandScore.bCurrentColor=m_cbMainColor;
	LandScore.bMultiples=m_cbMultiples;

	//�ж�m_wCurrentUser�Ƿ��ܼӱ����ټӱ�
	bool bDbl=false;
	bool bRedbl=false;
	//m_wCurrentUser�ܷ��ټӱ�
	if (m_cbMultiples!=6)  //����Ѿ����ټӱ������¼�ֻ��PASS������
	{
		byte bNum1=GetScoreNum(wChairID);
		byte bNum2=GetScoreNum((wChairID+2)%m_wPlayerCount);
		//���wChairIDû�ټӱ���m_wCurrentUser�����ټӱ�
		if (m_bScoreInfo[wChairID][bNum1-1]!=210&&m_bScoreInfo[wChairID][bNum1-1]!=240)
		{
			if (m_bScoreInfo[wChairID][bNum1-1]==200) 
			{
				bRedbl=true;
			}

			if (bNum2!=50)
			{
				if (m_bScoreInfo[(wChairID+2)%m_wPlayerCount][bNum2-1]==200) 
				{
					bRedbl=true;
				}
			}
		}
	}

	if (m_cbMultiples==2)
	{
		//�ӱ�
		LandScore.bLastColor=200;
		LandScore.bLastScore=200;
	}
	else
	{
		//�ټӱ�
		LandScore.bLastColor=210;
		LandScore.bLastScore=210;
	}
	
	LandScore.bDbl=bDbl;
	LandScore.bReDbl=bRedbl;

	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CALL_SCORE,&LandScore,sizeof(LandScore));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CALL_SCORE,&LandScore,sizeof(LandScore));
	return true;
}



//�û��з�
bool CTableFrameSink::OnUserLandScore(WORD wChairID, BYTE bBossScore,BYTE bBossColor)
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_UG_SCORE);
	if (m_pITableFrame->GetGameStatus()!=GS_UG_SCORE) return true;
	if (wChairID!=m_wCurrentUser) return false;
	//Ч�����
	if ((bBossScore>7)&&(bBossScore!=250)) return false;

	//���ñ��� ��Լ��������Ϸ��������
	if (bBossScore!=250)
	{
		m_bLandScore=bBossScore;
		m_wBankerUser=m_wCurrentUser;
		m_cbMainColor=bBossColor;
		m_cbMultiples=1;
	}

	//�зּ�¼
	for (byte i=0;i<50;i++)
	{
		if (m_bScoreInfo[wChairID][i]==0) 
		{
			m_bScoreInfo[wChairID][i]=bBossScore;
			m_bColorInfo[wChairID][i]=bBossColor;
			break;
		}
	}

	//��ʼ�ж�
	//if ((m_bLandScore==5)||(m_wFirstUser==(wChairID+1)%m_wPlayerCount))
	if (bBossScore==250) 
	{
		m_bPlayStatus[wChairID]=1;
	}
	else
	{
		ZeroMemory(m_bPlayStatus,sizeof(m_bPlayStatus));
	}

	WORD PassCount=0;	  //�жϷ�������
	WORD PassCountUser=INVALID_CHAIR;  
	WORD wNextUser=INVALID_CHAIR;	  

	for(WORD i=0;i<m_wPlayerCount;i++)													  
	{
		if (m_bPlayStatus[i]==1) 
		{
			PassCount++;
		}
		else
		{
			PassCountUser=i;
		}
	}    
	
	if ((PassCount==3&&m_wBankerUser!=INVALID_CHAIR)||(m_bLandScore==7&&m_cbMainColor==COLOR_NT))
	{
		if (m_wBankerUser==INVALID_CHAIR) m_wBankerUser=0;
		//����״̬
		m_pITableFrame->SetGameStatus(GS_UG_PLAY);

		if (PassCount==3) m_wCurrentUser=PassCountUser;
		
		//�ж�ׯ��
		//------------------------------------------------------------------------------------
		//�ų��������(���׽ж�Լ�������˶�PASSʱ��ׯ�����׽���)
		if (GetScoreNum(0)!=1&&GetScoreNum(1)!=1&&GetScoreNum(2)!=1&&GetScoreNum(3)!=1) 
		{
			BYTE bNum1=0;
			BYTE bNum2=0;
			for (byte j=0;j<GetScoreNum(m_wCurrentUser);j++)
			{
				if (m_bColorInfo[m_wCurrentUser][j]!=m_cbMainColor)
				{
					bNum1++;
				}
				else
				{
					break;
				}
			}
			for (byte j=0;j<GetScoreNum((m_wCurrentUser+2)%m_wPlayerCount);j++)
			{
				if (m_bColorInfo[(m_wCurrentUser+2)%m_wPlayerCount][j]!=m_cbMainColor)
				{
					bNum2++;
				}
				else
				{
					break;
				}
			}
			if (bNum2<=bNum1) 
			{
				//���ñ���
				m_wCurrentUser=(m_wCurrentUser+2)%m_wPlayerCount;
				m_wBankerUser=m_wCurrentUser;
			}
		}
		//------------------------------------------------------------------------------------

		//��������
		CMD_S_GamePlay GamePlay;
		GamePlay.wCurrentUser=m_wCurrentUser;
		GamePlay.m_cbLandColor=m_cbMainColor;
		GamePlay.m_cbLandScore=m_bLandScore;
		GamePlay.bMultiples=m_cbMultiples;
		
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
				GamePlay.bCardCount[i]=m_cbHandCardCount[i];
				CopyMemory(&GamePlay.bCardData[i],m_cbHandCardData[i],m_cbHandCardCount[i]*sizeof(BYTE));
		}

		m_GameLogic.SetMainColor(m_cbMainColor);
		m_GameLogic.SetPackCount(1);

		//�׳�����
		m_wCurrentUser=(m_wCurrentUser+1)%GAME_PLAYER;
		m_wFirstOutUser=m_wCurrentUser;
		
		//��������
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//������Ϣ
			m_pITableFrame->SendTableData(i,SUB_S_GAME_PLAY,&GamePlay,sizeof(GamePlay));
			m_pITableFrame->SendLookonData(i,SUB_S_GAME_PLAY,&GamePlay,sizeof(GamePlay));
		}

		return true;
	}

	if (PassCount==4&&m_wBankerUser==INVALID_CHAIR)
	{
		//���·���
		memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
		DispatchUserCard();
		return true;
	}

	//�����û�
	//˳ʱ��
	m_wCurrentUser=(wChairID+1)%m_wPlayerCount;

	//�ж�m_wCurrentUser�Ƿ��ܼӱ����ټӱ�
	bool bDbl=false;
	bool bRedbl=false;
	
	switch (m_cbMultiples)
	{
		//m_wCurrentUser�Ƿ��ܼӱ�
	case 1:
		{
			byte bNum=GetScoreNum(wChairID);
			byte bNum1=GetScoreNum((wChairID+2)%m_wPlayerCount);
			if (bNum==50) return false;

			//�����û�ж�Լ
			if (m_bLandScore==250) break;

			//���m_wCurrentUser���ѼҽеĶ�Լ��Ŀǰ��Լ
			if (m_bScoreInfo[(m_wCurrentUser+2)%m_wPlayerCount][bNum-1]==m_bLandScore&&m_bColorInfo[(m_wCurrentUser+2)%m_wPlayerCount][bNum-1]==m_cbMainColor) break;

			if (bNum1!=50)
			{
				if ((m_bScoreInfo[wChairID][bNum-1]!=250||m_bScoreInfo[(wChairID+2)%m_wPlayerCount][bNum1-1]!=250)&&(m_bScoreInfo[wChairID][bNum-1]!=210&&m_bScoreInfo[(wChairID+2)%m_wPlayerCount][bNum1-1]!=210)&&(m_bScoreInfo[wChairID][bNum-1]!=240&&m_bScoreInfo[(wChairID+2)%m_wPlayerCount][bNum1-1]!=240)) 
				{
					bDbl=true;
					break;
				}
			}
			else
			{
				if (m_bScoreInfo[wChairID][bNum-1]!=250&&m_bScoreInfo[wChairID][bNum-1]!=210&&m_bScoreInfo[wChairID][bNum-1]!=240&&m_bScoreInfo[wChairID][bNum-1]!=240)
				{
					bDbl=true;
					break;
				}
			}
			
			if (m_bScoreInfo[wChairID][bNum-1]!=250&&m_bScoreInfo[wChairID][bNum-1]!=240)
			{
				bDbl=true;
			}
			else
			{
				bNum=GetScoreNum((wChairID+2)%m_wPlayerCount);
				if (bNum==50) break;
				if (m_bScoreInfo[(wChairID+2)%m_wPlayerCount][bNum-1]!=250&&m_bScoreInfo[(wChairID+2)%m_wPlayerCount][bNum-1]!=240) bDbl=true;
			}
			
			break;
		}
		//m_wCurrentUser�Ƿ����ټӱ�
	case 2:
		{
			byte bNum1=GetScoreNum(wChairID);
			byte bNum2=GetScoreNum((wChairID+2)%m_wPlayerCount);
			//��������Լ�ͬ���ټӱ�
			if (m_bScoreInfo[wChairID][bNum1-1]!=210&&m_bScoreInfo[wChairID][bNum1-1]!=240)
			{
				if (m_bScoreInfo[wChairID][bNum1-1]==200) 
				{
					bRedbl=true;
					break;
				}
				
				if (bNum2!=50)
				{
					if (m_bScoreInfo[(wChairID+2)%m_wPlayerCount][bNum2-1]==200) 
					{
						bRedbl=true;
						break;
					}
				}
			}
			break;
		}
	}
	
	//�����û�
	//��ʱ��
	/*
	for(WORD i=m_wPlayerCount-1;i>=0;i--)													  
	{
		wNextUser=(m_wCurrentUser+i-1)%m_wPlayerCount;	
		if (m_bPlayStatus[wNextUser]==0) break;         
	}
	m_wCurrentUser=wNextUser;
	*/

	//������Ϣ
	CMD_S_CallScore LandScore;
	LandScore.bCallScoreUser=wChairID;
	LandScore.wCurrentUser=m_wCurrentUser;
	LandScore.bCurrentScore=m_bLandScore;
	LandScore.bCurrentColor=m_cbMainColor;
	LandScore.bMultiples=m_cbMultiples;
	LandScore.bLastColor=bBossColor;
	LandScore.bLastScore=bBossScore;
	LandScore.bDbl=bDbl;
	LandScore.bReDbl=bRedbl;

	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CALL_SCORE,&LandScore,sizeof(LandScore));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CALL_SCORE,&LandScore,sizeof(LandScore));
	

	return true;
}

//�û�����
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData[], BYTE cbCardCount, bool bIsHandOut[],bool bIsAutoOut)
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_UG_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_UG_PLAY) return true;

	//Ч���û�
	if(bIsHandOut[wChairID]==false)
	{
		ASSERT((wChairID==m_wCurrentUser)&&(cbCardCount<=MAX_COUNT));
		if ((wChairID!=m_wCurrentUser)||(cbCardCount>MAX_COUNT)) return false;
	}
	else
	{
		ASSERT((cbCardCount<=MAX_COUNT));
		if (cbCardCount>MAX_COUNT) return false;
		if (!bIsAutoOut) wChairID=(m_wBankerUser+2)%m_wPlayerCount;
	}
	//�׳�����
	if (m_wFirstOutUser==wChairID)
	{
		//�����ж�
		BYTE cbCardType=m_GameLogic.GetCardType(cbCardData,cbCardCount);
		if (cbCardType==CT_ERROR||cbCardType!=CT_SINGLE) return false;
	}
	else
	{
		//��Ŀ�ж�
		ASSERT(cbCardCount==m_cbOutCardCount[m_wFirstOutUser]);
		if (cbCardCount!=m_cbOutCardCount[m_wFirstOutUser]) return false;

		//����Ч��
		if (m_GameLogic.EfficacyOutCard(cbCardData,cbCardCount,m_cbOutCardData[m_wFirstOutUser],
			m_cbOutCardCount[m_wFirstOutUser],m_cbHandCardData[wChairID],m_cbHandCardCount[wChairID])==false)
		{
			ASSERT(FALSE);
			return false;
		}
	}

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(cbCardData,cbCardCount,m_cbHandCardData[wChairID],m_cbHandCardCount[wChairID])==false) return false;
	m_cbHandCardCount[wChairID]-=cbCardCount;
	
	//���Ƽ�¼
	m_cbOutCardCount[wChairID]=cbCardCount;
	CopyMemory(m_cbOutCardData[wChairID],cbCardData,cbCardCount);

	//�л��û�
	m_wCurrentUser=(wChairID+1)%m_wPlayerCount;
	if (m_wCurrentUser==m_wFirstOutUser) m_wCurrentUser=INVALID_CHAIR;

	

	if (m_wCurrentUser!=INVALID_CHAIR)
	{
		//�ֵ����ֳ���
		if(m_wCurrentUser==(m_wBankerUser+2)%GAME_PLAYER)
		{
			bHandOutFlag[m_wBankerUser]=true;
		}
	}

	//��������
	CMD_S_OutCard OutCard;
	OutCard.wOutCardUser=wChairID;
	OutCard.cbCardCount=cbCardCount;
	OutCard.wCurrentUser=m_wCurrentUser;
	OutCard.bHandOutCardCount=m_cbHandCardCount[(m_wBankerUser+2)%m_wPlayerCount];
	//OutCard.bHandOutCardData=m_cbHandCardData[(m_wBankerUser+2)%m_wPlayerCount];
	CopyMemory(OutCard.bHandOutCardData,m_cbHandCardData[(m_wBankerUser+2)%m_wPlayerCount],sizeof(m_cbHandCardData[(m_wBankerUser+2)%m_wPlayerCount]));
	CopyMemory(OutCard.cbCardData,cbCardData,cbCardCount*sizeof(BYTE));
	CopyMemory(OutCard.bHandOutFlag,bHandOutFlag,sizeof(bHandOutFlag));

	//��������
	//WORD wSendSize=sizeof(OutCard)-sizeof(OutCard.cbCardData)+OutCard.cbCardCount*sizeof(BYTE);
	//m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);
	//m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));

	//һ�ֽ���
	if (m_wCurrentUser==INVALID_CHAIR)
	{
		//�ж�ʤ��
		m_wTurnWinner=m_GameLogic.CompareCardArray(m_cbOutCardData,m_cbOutCardCount[0],m_wFirstOutUser);

		//����÷�
		if ((m_wTurnWinner!=m_wBankerUser)&&((m_wTurnWinner+2)%m_wPlayerCount!=m_wBankerUser)) //ׯ�ҺͶԼ�
		{
			m_cbValueOrder[1]+=1;
		}
		else
		{
			m_cbValueOrder[0]+=1;
		}

		//���ñ���
		if (m_cbHandCardCount[m_wTurnWinner]!=0)
		{
			m_wFirstOutUser=m_wTurnWinner;
			ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
			ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));
		}

		ZeroMemory(bHandOutFlag,sizeof(bHandOutFlag));
		//�û��л�
		m_wCurrentUser=(m_cbHandCardCount[m_wTurnWinner]==0)?INVALID_CHAIR:m_wTurnWinner;

		//��������
		if (m_wCurrentUser!=INVALID_CHAIR)
		{
			//�ֵ����ֳ���
			if(m_wCurrentUser==(m_wBankerUser+2)%GAME_PLAYER)
			{
				bHandOutFlag[m_wBankerUser]=true;
			}
		}

		//��������
		CMD_TurnBalance TurnBalance;
		TurnBalance.wTurnWinner=m_wTurnWinner;
		TurnBalance.wCurrentUser=m_wCurrentUser;
		TurnBalance.bBankerScore=m_cbValueOrder[0];
		TurnBalance.bOtherScore=m_cbValueOrder[1];
		CopyMemory(TurnBalance.bHandOutFlag,bHandOutFlag,sizeof(bHandOutFlag));


		//��������
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TURN_BALANCE,&TurnBalance,sizeof(TurnBalance));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TURN_BALANCE,&TurnBalance,sizeof(TurnBalance));
	}

	//�����ж�
	if (m_wCurrentUser==INVALID_CHAIR) OnEventGameEnd(m_wTurnWinner,NULL,GER_NORMAL);

	return true;
}

//�ɷ��˿�
bool CTableFrameSink::DispatchUserCard()
{
	//�����˿�
	BYTE cbRandCard[CELL_PACK*2];
	m_GameLogic.RandCardList(cbRandCard,CELL_PACK*m_cbPackCount);

	//�û��˿�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_cbHandCardCount[i]=m_GameLogic.GetDispatchCount();
		CopyMemory(&m_cbHandCardData[i],&cbRandCard[i*m_cbHandCardCount[i]],sizeof(BYTE)*m_cbHandCardCount[i]);
		
		//�����˿�
		m_cbTieCardCount[i]=m_GameLogic.GetDispatchCount();
		CopyMemory(&m_cbTieCardData[i],&cbRandCard[i*m_cbTieCardCount[i]],sizeof(BYTE)*m_cbTieCardCount[i]);
	}
	
	//���Ϳ�ʼ
	if(m_wFirstUser==INVALID_CHAIR) m_wFirstUser=0;
	m_wCurrentUser=m_wFirstUser;
	CMD_S_SendCard SendCard;
	SendCard.wCurrentUser=m_wCurrentUser;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//Ч��״̬
		ASSERT(m_cbHandCardCount[i]<=CountArray(SendCard.cbCardData));
		if (m_cbHandCardCount[i]>CountArray(SendCard.cbCardData)) return false;

		//�����˿�
		SendCard.cbCardCount=m_cbHandCardCount[i];
		CopyMemory(SendCard.cbCardData,m_cbHandCardData[i],sizeof(BYTE)*m_cbHandCardCount[i]);

		//����ͳ��
		WORD wHeadSize=sizeof(SendCard)-sizeof(SendCard.cbCardData);
		WORD wSendSize=wHeadSize+sizeof(SendCard.cbCardData[0])*SendCard.cbCardCount;

		//��������
		m_pITableFrame->SendTableData(i,SUB_S_SEND_CARD,&SendCard,wSendSize);
		m_pITableFrame->SendLookonData(i,SUB_S_SEND_CARD,&SendCard,wSendSize);
	}

	return true;
}

//�ֿ�����
bool CTableFrameSink::SetJuKuang(WORD wFirstUser)
{
	if(wFirstUser==INVALID_CHAIR) wFirstUser=0;
	m_cbJuKuang=(wFirstUser%GAME_PLAYER)+1;
	if (m_cbJuKuang<=0||m_cbJuKuang>4) m_cbJuKuang=1;
	/*����ֿ�
	m_cbJuKuang=1+(int)(4.0*rand()/(RAND_MAX+1.0)); 
	if(m_cbJuKuang<=0||m_cbJuKuang>=5) SetJuKuang();
	*/
	return true;
}

//���Ƶ���
BYTE CTableFrameSink::GetBigCardPoint(WORD wFirstUser,WORD wNextUser)
{
	BYTE bBigCardPoint=0;
	for (byte i=0;i<m_cbTieCardCount[wFirstUser];i++)
	{
		//1=A 13=K 12=Q 11=J
		if (m_GameLogic.GetCardValue(m_cbTieCardData[wFirstUser][i])==1) bBigCardPoint+=4;
		if (m_GameLogic.GetCardValue(m_cbTieCardData[wFirstUser][i])==13) bBigCardPoint+=3;
		if (m_GameLogic.GetCardValue(m_cbTieCardData[wFirstUser][i])==12) bBigCardPoint+=2;
		if (m_GameLogic.GetCardValue(m_cbTieCardData[wFirstUser][i])==11) bBigCardPoint+=1;

		if (m_GameLogic.GetCardValue(m_cbTieCardData[wNextUser][i])==1) bBigCardPoint+=4;
		if (m_GameLogic.GetCardValue(m_cbTieCardData[wNextUser][i])==13) bBigCardPoint+=3;
		if (m_GameLogic.GetCardValue(m_cbTieCardData[wNextUser][i])==12) bBigCardPoint+=2;
		if (m_GameLogic.GetCardValue(m_cbTieCardData[wNextUser][i])==11) bBigCardPoint+=1;
	}
	return bBigCardPoint;
}

//�õ����д���
BYTE CTableFrameSink::GetScoreNum(WORD wChariID)
{
	byte bNum=0;
	for (byte i=0;i<50;i++)
	{
		if (m_bScoreInfo[wChariID][i]!=0) bNum++;
	}
	return bNum;
}
//////////////////////////////////////////////////////////////////////////
