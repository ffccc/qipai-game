#include "StdAfx.h"
#include "TableFrameSink.h"
//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_FullReady;	//��ʼģʽ

//��ɫ����
const BYTE CTableFrameSink::m_bColorIndex[BALL_NUM]={0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,3,4,5,6,7};

//��������
const int CTableFrameSink::m_bPointIndex[BALL_NUM]={4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,3,4,5,6,7};
CTableFrameSink::CTableFrameSink(void)
{
	m_bHitColor=FALSE;
	m_dwMoney=0L;
	m_PlaerNum=1;
	m_iTimeDelay=30;
	m_bTargetBall=1;
	m_bNowHitUser=0;
	m_NowSetRule=0;
	bHitColor=1;
	::memset(m_bFinish,0,sizeof(m_bFinish));
	::memset(m_iUserPoint,0,sizeof(m_iUserPoint));
	::memset(m_bInBallIndex,0,sizeof(m_bInBallIndex));
	::memset(m_bBallAccess,0,sizeof(m_bBallAccess));
	m_bNextColor=0;
	m_bWhiteBallOver=false;
	m_bIsPlayColor=false;
	m_AgreeGoOn[0]=false;
	m_AgreeGoOn[1]=false;
	m_GameLayer=0;
	m_GameSetCount=0;
	bIsSetRule=FALSE;
//	m_pIGoldPresent=NULL;
}

CTableFrameSink::~CTableFrameSink(void)
{
}
void * __cdecl CTableFrameSink::QueryInterface(const IID & Guid, 
											DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}
bool __cdecl CTableFrameSink::InitTableFrameSink(IUnknownEx * pIUnknownEx)
{
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if(m_pITableFrame==NULL) 
	{
		return false;
	}
//	m_pIGoldPresent=GET_OBJECTPTR_INTERFACE(pIUnknownEx,IGoldPresent);
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);
	return true;
}
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	InitData();
}
enStartMode __cdecl CTableFrameSink::GetGameStartMode()
{
	return enStartMode_FullReady;
}
bool __cdecl CTableFrameSink::OnEventGameStart()
{	
	//��������
	InitData();
	m_PlaerNum=2;
	m_pITableFrame->SetGameStatus(GS_GAME_PLAY);
	m_bNowHitUser=rand()%2;
	//������Ϣ
	tagGS_BeginStruct Begin;
	Begin.bFirstHit=m_bNowHitUser;
	Begin.bIsTrain=FALSE;
	for (int i=0;i<GAME_PLAYER;i++) 
	{
		m_pITableFrame->SendTableData(i, SUB_GAME_BEGIN, &Begin, sizeof(Begin));
		m_pITableFrame->SendLookonData(i, SUB_GAME_BEGIN, &Begin, sizeof(Begin));
	}
	return true;
}
bool __cdecl CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	return true;
}
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, 
											IServerUserItem * pIServerUserItem,
											BYTE cbReason)
{
	GameFinish(wChairID,0);
	return true;
}
bool __cdecl CTableFrameSink::SendGameScene(WORD wChiarID, 
											IServerUserItem * pIServerUserItem, 
											BYTE bGameStatus, 
											bool bSendSecret)
{
	switch (bGameStatus)
	{
	case GS_TQ_WAIT_BEGIN:	//��Ϸû�п�ʼ״̬
		{
			//InitData();
			tagGS_GameStation_1 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));
			 m_pITableFrame->SendGameScene(pIServerUserItem,
													&GameStation,
													sizeof(GameStation));
			return true;
		}
	case GS_GAME_PLAY:
		{
			tagGS_GameStation_2 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));
			for(int i=0;i<BALL_NUM;i++)
			{
				GameStation.bPlaceInfo[i].xPos=m_iXBallStation[i];	//�����Ϣ
				GameStation.bPlaceInfo[i].yPos=m_iYBallStation[i];
				GameStation.bPlaceInfo[i].bBallIndex=i;
				GameStation.bBallAccess[i]=m_bBallAccess[i];		//�ܷ����
				GameStation.bInBallIndex[i]=m_bInBallIndex[i];		//������������
			}
			GameStation.bNowHitUser=bIsHitFinish?(wChiarID+1)%2:wChiarID;				//��һ��������
			GameStation.bTargetBall=m_bTargetBall;					//Ŀ����
			GameStation.iUserPoint[0]=m_iUserPoint[0];				//��ǰ����
			GameStation.iUserPoint[1]=m_iUserPoint[1];
			GameStation.bIsTakeWhite=bIsSendTakeWhite;
			GameStation.mStarPos=m_StarPos;
			GameStation.mStarPoint=m_StarPoint;
		    m_pITableFrame->SendGameScene(pIServerUserItem,
													&GameStation,
													sizeof(GameStation));
			if(bIsHitFinish)
			{
				tagServerUserData *user = pIServerUserItem->GetUserData();
				if(user->cbUserStatus==US_LOOKON)
					m_pITableFrame->SendLookonData(wChiarID, ASS_TQ_HIT_BALL,&pNowHitInfo,sizeof(pNowHitInfo));
				else
					m_pITableFrame->SendTableData(wChiarID, ASS_TQ_HIT_BALL,&pNowHitInfo,sizeof(pNowHitInfo));
			}
			return true;
		}
	}
	return FALSE;
}
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
	return false;
}
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID,	
											const void * pDataBuffer, 
											WORD wDataSize, 
											IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case ASS_TQ_HIT_INFO:	//������Ϣ
		{
			tagServerUserData *user = pIServerUserItem->GetUserData();
			if(wDataSize != sizeof(tagGS_HitInfoStruct)) 
			{
				return false;
			}
			tagGS_HitInfoStruct *p=(tagGS_HitInfoStruct *)pDataBuffer;
			if(m_PlaerNum!=1)
			{
				m_pITableFrame->SendTableData((user->wChairID+1)%2, ASS_TQ_HIT_INFO,p,sizeof(*p));
				m_pITableFrame->SendLookonData((user->wChairID+1)%2, ASS_TQ_HIT_INFO,p,sizeof(*p));
			}
			return TRUE;
		}
	case ASS_TQ_HIT_BALL:	//������
		{
			tagServerUserData *user = pIServerUserItem->GetUserData();
			if(wDataSize != sizeof(tagGS_HitBallStruct)) 
			{
				return false;
			}
			tagGS_HitBallStruct *p=(tagGS_HitBallStruct *)pDataBuffer;
			if(m_PlaerNum!=1)
			{
				pNowHitInfo=*p;
				bIsHitFinish=TRUE;
				m_pITableFrame->SendTableData((user->wChairID+1)%2, ASS_TQ_HIT_BALL,p,sizeof(*p));
				for(int i=0;i<GAME_PLAYER;i++)
				{
					m_pITableFrame->SendLookonData(i, ASS_TQ_HIT_BALL,p,sizeof(*p));
				}
			}
			return TRUE;
		}
	case ASS_TQ_HIT_FINISH:	//�������
		{
			tagServerUserData *user = pIServerUserItem->GetUserData();
			if(wDataSize != sizeof(tagGS_HitFinishStruct)) 
			{
				return false;
			}
			memset(&pNowHitInfo,0,sizeof(pNowHitInfo));
			tagGS_HitFinishStruct * pHitFinish=(tagGS_HitFinishStruct *)pDataBuffer;
			return OnHitFinish(user->wChairID,pHitFinish);
		}
	case ASS_TQ_PLACE_WHITE:	//���ð���
		{
			tagServerUserData *user = pIServerUserItem->GetUserData();
			if(wDataSize != sizeof(tagCS_PlaceBallStruct)) 
			{
				return false;
			}
			bIsSendTakeWhite=FALSE;
			m_bBallAccess[0]=TRUE;
			//if (m_bBallAccess[0]==FALSE)
			//{
			tagCS_PlaceBallStruct * pPlaceWhite=(tagCS_PlaceBallStruct *)pDataBuffer;
			return OnPlaceWhite(user->wChairID,pPlaceWhite);
			//}
		}
	case ASS_TQ_PLACE_COLOR:		//GSQ:���ò���OK
		{
			//˫�������ú���
			tagServerUserData *user = pIServerUserItem->GetUserData();
			if(m_PlaerNum==1)
			{
				m_AgreeGoOn[(user->wChairID+1)%2]=true;
			}
			m_AgreeGoOn[user->wChairID]=true;
			if(m_AgreeGoOn[0] && m_AgreeGoOn[1])
			{
				//�Ѳ����µ�λ�ñ�������
				tagCS_PlaceBallStruct * pColorBall=(tagCS_PlaceBallStruct *)pDataBuffer;
				for(int i=0;i<pColorBall->bBallCount;i++)
				{
					m_iXBallStation[pColorBall->bPlaceInfo[i].bBallIndex]=pColorBall->bPlaceInfo[i].xPos;
					m_iYBallStation[pColorBall->bPlaceInfo[i].bBallIndex]=pColorBall->bPlaceInfo[i].yPos;
				}
				if(m_bWhiteBallOver)//�������Ͱ���������ȰѲ���Ż�ԭλ�ã��ٷŰ���
				{
					tagGS_CurrentPlayerStruct m_TakeWhiteUser;
					m_TakeWhiteUser.Seat=m_bNowHitUser;
					m_TakeWhiteUser.iPoint=m_GsqPoint;
					bIsSendTakeWhite=TRUE;
					for (int i=0;i<m_PlaerNum;i++)
					{
						//��������
						m_pITableFrame->SendTableData((user->wChairID+i)%2, ASS_TQ_TAKE_WHITE,&m_TakeWhiteUser,sizeof(m_TakeWhiteUser));
						m_pITableFrame->SendLookonData((user->wChairID+i)%2, ASS_TQ_TAKE_WHITE,&m_TakeWhiteUser,sizeof(m_TakeWhiteUser));
					}
					m_bWhiteBallOver=false;
				}
				else
				{
					tagGS_NextHitInfoStruct NextHit;
					NextHit.iPoint=m_GsqPoint;
					NextHit.bHitPeople=m_bNowHitUser;//�������		
					NextHit.bTargerBall=m_bTargetBall;//Ӧ�û�����ɫ
					for(int i=0;i<BALL_NUM;i++)
					{
						NextHit.bPlaceInfo[i].xPos=m_iXBallStation[i];	//�����Ϣ
						NextHit.bPlaceInfo[i].yPos=m_iYBallStation[i];
					}
					NextHit.mStarPoint=m_StarPoint;
					NextHit.mStarPos=m_StarPos;
					if(bIsCanAddStar&&m_GsqPoint>0)
					{
						NextHit.iPoint+=5;
						m_iUserPoint[m_bNowHitUser]+=5;
						NextHit.bIsGetStarPoint=TRUE;
					}
					else
					{
						NextHit.mStarPos=200;
						NextHit.bIsGetStarPoint=FALSE;
					}
					bIsCanAddStar=FALSE;
					for (int i=0;i<m_PlaerNum;i++)
					{
						//��������
						m_pITableFrame->SendTableData((user->wChairID+i)%2, ASS_TQ_NEXT_HIT, &NextHit,sizeof(NextHit));
						m_pITableFrame->SendLookonData((user->wChairID+i)%2, ASS_TQ_NEXT_HIT, &NextHit,sizeof(NextHit));
					}
				}
				m_AgreeGoOn[0]=false;
				m_AgreeGoOn[1]=false;
			}
			return TRUE;
		}
	case ASS_TQ_HIT_GIVEUP:
		{
			if(m_PlaerNum!=1)
			{
				tagServerUserData *user = pIServerUserItem->GetUserData();
				GameFinish(user->wChairID,3);
			}
			return TRUE;
		}

	case ASS_TQ_HIT_REPLACE:
		{
			if(m_PlaerNum!=1)
			{
				tagServerUserData *user = pIServerUserItem->GetUserData();
				m_pITableFrame->SendTableData((user->wChairID+1)%2, ASS_TQ_HIT_REPLACE);
			}
			return TRUE;
		}
	case ASS_TQ_HIT_CONESSION:
		{
			tagServerUserData *user = pIServerUserItem->GetUserData();
			tagGS_NextHitInfoStruct NextHit;
			NextHit.iPoint=0;
			if(m_PlaerNum!=1)
			{
				m_bNowHitUser=(m_bNowHitUser+1)%2;
			}
			NextHit.bHitPeople=m_bNowHitUser;//�������
			NextHit.bTargerBall=m_bTargetBall;//Ӧ�û�����ɫ
			NextHit.mStarPos=200;
			for(int i=0;i<BALL_NUM;i++)
			{
				NextHit.bPlaceInfo[i].xPos=m_iXBallStation[i];	//�����Ϣ
				NextHit.bPlaceInfo[i].yPos=m_iYBallStation[i];
			}
			bIsCanAddStar=FALSE;
			for (int i=0;i<m_PlaerNum;i++)
			{
				//��������
				m_pITableFrame->SendTableData((user->wChairID+i)%2, ASS_TQ_NEXT_HIT, &NextHit,sizeof(NextHit));
				m_pITableFrame->SendLookonData((user->wChairID+i)%2, ASS_TQ_NEXT_HIT, &NextHit,sizeof(NextHit));
			}
			return TRUE;
		}
	case ASS_TQ_AGREE:
		{
			tagServerUserData *user = pIServerUserItem->GetUserData();
			GameFinish(user->wChairID,2);
			return TRUE;
		}
	case ASS_TQ_NOAGREE:
		{
			tagServerUserData *user = pIServerUserItem->GetUserData();
			return m_pITableFrame->SendTableData((user->wChairID+1)%2, ASS_TQ_NOAGREE);
		}
		default:
		{
			return false;
		}
	}
	return false;
}
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, 
											const void * pDataBuffer, 
											WORD wDataSize, 
											IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_GF_USER_READY:		//�û�ͬ��
		{
			tagServerUserData *user = pIServerUserItem->GetUserData();
			InitData();
			if(!bIsSetRule)
			{
				m_PlaerNum=1;
				bIsSetRule=TRUE;
				tagGS_BeginStruct Begin;
				m_bNowHitUser=user->wChairID;
				Begin.bFirstHit=m_bNowHitUser;
				Begin.bIsTrain=TRUE;
				m_pITableFrame->SendTableData(user->wChairID, SUB_GAME_BEGIN, &Begin, sizeof(Begin));
			}
		}
	}
	return false;
}
BOOL CTableFrameSink::IsHaveRedBall()
{
	int temp=0;
	for(int i=0;i<15;i++)
	{
		if(m_bBallAccess[i+1])
			return TRUE;
	}
	return FALSE;
}
//���������
BOOL CTableFrameSink::OnHitFinish(WORD bDeskStation, tagGS_HitFinishStruct * pHitFinishInfo)
{
	BOOL b=false; //�Ƿ��в������
	BOOL tempBB=false;
	//����״̬ 
	if (bDeskStation==m_bNowHitUser)
	{
		bIsHitFinish=FALSE;
		if(pHitFinishInfo->bIsGetStar)
			bIsCanAddStar=TRUE;
		for(int i=0;i<22;i++)
		{
			m_bInBallIndex[i]=pHitFinishInfo->bInBallIndex[i];
			m_bBallAccess[i]=pHitFinishInfo->bBallAccess[i];
			m_iXBallStation[i]=pHitFinishInfo->iXBallStation[i];
			m_iYBallStation[i]=pHitFinishInfo->iYBallStation[i];
		}
	}
	m_bFinish[bDeskStation]=TRUE;
	if(m_PlaerNum == 1)
		m_bFinish[(bDeskStation+1)%2]=TRUE;	
	//�л�״̬
	if ((m_bFinish[0]==TRUE)&&(m_bFinish[1]==TRUE))
	{
		//�Ա�����
		::memset(m_bFinish,0,sizeof(m_bFinish));
		tagGS_NextHitInfoStruct NextHit;
		m_StarPos=rand()%6;
		//m_StarPos=255;
		m_StarPoint=1+rand()%4;
		NextHit.iPoint=0;
		NextHit.bHitPeople=m_bNowHitUser;
		//�ж��Ƿ������
		bHitColor=m_bColorIndex[pHitFinishInfo->bFirstHitBall];
		if (m_bBallAccess[0]==FALSE)
		{
			m_StarPos=255;
			if(bHitColor<4)
				NextHit.iPoint=-4;
			else
				NextHit.iPoint=-(bHitColor);
			m_bWhiteBallOver=true;
		}
		//����÷�
		if (pHitFinishInfo->bFirstHitBall!=255)	 //������
		{
			if ((pHitFinishInfo->bFirstHitBall>=BALL_NUM)||(pHitFinishInfo->bFirstHitBall==0)) return FALSE;

			//�������
			if (m_bTargetBall==1)	//����Ŀ����
			{
				if (bHitColor!=m_bTargetBall)
				{
					m_StarPos=255;
					//�������
					NextHit.iPoint=-max(m_bPointIndex[pHitFinishInfo->bFirstHitBall],4);
				}
			}
			else	//����Ŀ����
			{
				if ((m_bTargetBall==bHitColor||m_bTargetBall==8&&bHitColor>1)&&pHitFinishInfo->bInBallCount<2)
				{
					if(pHitFinishInfo->bInBallCount==1&&bHitColor!=m_bColorIndex[pHitFinishInfo->bInBallIndex[0]])
					{
						m_StarPos=255;
						//�������
						NextHit.iPoint=-max(m_bPointIndex[pHitFinishInfo->bFirstHitBall],4);
					}
				}
				else
				{
					m_StarPos=255;
					//�������
					NextHit.iPoint=-max(m_bPointIndex[pHitFinishInfo->bFirstHitBall],4);
				}
			}
			//��������
			tagGS_ColorBallPlaceInfoStruct PlaceColorBall;		//GSQ:�طŲ���ṹ��
			PlaceColorBall.bBallCount=0;
			bool m_bIsHaveError=false;
			if(m_bWhiteBallOver||NextHit.iPoint<0)
				m_bIsHaveError=true;
			if (m_bTargetBall==1)	//������
			{
				for (int i=0;i<pHitFinishInfo->bInBallCount;i++)
				{
					if(m_bColorIndex[pHitFinishInfo->bInBallIndex[i]]==0)
					{
						m_bIsHaveError=true;
						m_bWhiteBallOver=true;
						continue;
					}
					if (m_bColorIndex[pHitFinishInfo->bInBallIndex[i]]==1&&!m_bIsHaveError)
					{
						//������ȷ
						NextHit.iPoint=((NextHit.iPoint>=0)?NextHit.iPoint+1:NextHit.iPoint);
					}
					else
					{
						if(!IsHaveRedBall())
							tempBB=true;
						m_bIsHaveError=true;
						//�������
						m_StarPos=255;
						NextHit.iPoint=min(NextHit.iPoint,-max(m_bPointIndex[pHitFinishInfo->bInBallIndex[i]],4));
						if(pHitFinishInfo->bInBallIndex[i]>=16)//�Ѳ����ó���
						{
							b=true;
							PlaceColorBall.bPos=m_bNowHitUser;
							PlaceColorBall.BallError=true;
							PlaceColorBall.BallValue=NextHit.iPoint;
							PlaceColorBall.bInBallIndex[PlaceColorBall.bBallCount]=pHitFinishInfo->bInBallIndex[i];
							PlaceColorBall.bBallCount++;
						}
					}
				}
			}
			else	//������
			{
				if(!IsHaveRedBall())
					tempBB=true;
				for (int i=0;i<pHitFinishInfo->bInBallCount;i++)
				{
					if (((m_bTargetBall==m_bColorIndex[pHitFinishInfo->bInBallIndex[0]])||
						((m_bTargetBall==8)&&(m_bColorIndex[pHitFinishInfo->bInBallIndex[0]]>1)))&&
						NextHit.iPoint==0 && !m_bWhiteBallOver&&pHitFinishInfo->bInBallCount==1&&!m_bIsHaveError)
					{
						//������ȷ
						NextHit.iPoint=((NextHit.iPoint>=0)?NextHit.iPoint+m_bPointIndex[pHitFinishInfo->bInBallIndex[i]]:NextHit.iPoint);
						PlaceColorBall.BallError=false;
						if(m_bTargetBall==8)
							b=true;
						if(m_bNextColor==0)
						{
							PlaceColorBall.bInBallIndex[PlaceColorBall.bBallCount]=pHitFinishInfo->bInBallIndex[i];
							PlaceColorBall.bBallCount++;
						}
						if(m_bNextColor>0)
						{
							m_bNextColor++;
						}
						if(m_bNextColor==8)
						{
							m_iUserPoint[m_bNowHitUser]+=7;
							if(bIsCanAddStar)
								m_iUserPoint[m_bNowHitUser]+=5;
							GameFinish(m_bNowHitUser,1);
							return TRUE;
						}
					}
					else if(m_bColorIndex[pHitFinishInfo->bInBallIndex[i]]!=0)//�����������
					{
						//m_bIsHaveError=true;
						//�������
						m_StarPos=255;
						NextHit.iPoint=min(NextHit.iPoint,-max(m_bPointIndex[pHitFinishInfo->bInBallIndex[i]],4));
						b=true;
						PlaceColorBall.bPos=m_bNowHitUser;
						PlaceColorBall.BallError=true;
						PlaceColorBall.BallValue=(small)NextHit.iPoint;
						if(pHitFinishInfo->bInBallIndex[i]>=16)//�Ѳ����ó���
						{
							PlaceColorBall.bInBallIndex[PlaceColorBall.bBallCount]=pHitFinishInfo->bInBallIndex[i];
							PlaceColorBall.bBallCount++;
						}
					}
				}
			}
			if(tempBB && !m_bIsPlayColor)
				m_bIsPlayColor=true;

			if(m_bIsPlayColor && (m_bNextColor<2||m_bNextColor==8))
				m_bNextColor=2;
			//��¼����
			m_GsqPoint=NextHit.iPoint;
			if (NextHit.iPoint>0)
			{
				m_iUserPoint[m_bNowHitUser]+=NextHit.iPoint;
			}
			else
			{
				if(m_PlaerNum!=1)
				{
					m_bNowHitUser=(m_bNowHitUser+1)%2;
				}
				m_iUserPoint[m_bNowHitUser]-=NextHit.iPoint;
			}
			//�л�״̬
			if(m_bIsPlayColor)//�����׶�
			{
				m_bTargetBall=m_bNextColor;
			}
			else
			{
				if (m_bTargetBall==1)
				{
					if (NextHit.iPoint>0) 
						m_bTargetBall=8;
					else m_bTargetBall=1;
				}
				else 
				{
					m_bTargetBall=1;
				}
			}
			if(b)	//GSQ:�����ҽ��˲��򣬲��Ҳ����ڴ����׶Σ������ڴ����׶Σ����ǽ��˲���ָ������򣬷��͸�ÿһ����ң����·Ų����λ��
			{
				PlaceColorBall.Seat=m_bNowHitUser;
				for (int i=0;i<m_PlaerNum;i++)
				{
					//��������
					m_pITableFrame->SendTableData((bDeskStation+i)%2, ASS_TQ_PLACE_COLOR, &PlaceColorBall,sizeof(PlaceColorBall));
					m_pITableFrame->SendLookonData((bDeskStation+i)%2, ASS_TQ_PLACE_COLOR, &PlaceColorBall,sizeof(PlaceColorBall));
				}
				return TRUE;
			}
		}
		else 
		{
			m_StarPos=255;
			if(m_bNextColor<5)
				NextHit.iPoint=-4;
			else
				NextHit.iPoint=-(m_bNextColor);
			if(m_PlaerNum!=1)
			{
				m_bNowHitUser=(m_bNowHitUser+1)%2;
			}
			m_iUserPoint[m_bNowHitUser]-=NextHit.iPoint;

			if(!IsHaveRedBall())
			{
				if((m_bTargetBall<2||m_bTargetBall==8))
					m_bNextColor=2;
				m_bIsPlayColor=TRUE;
			}
			//�л�״̬
			if(m_bIsPlayColor)//�����׶�
			{
				m_bTargetBall=m_bNextColor;
			}
			else
			{
				if (m_bTargetBall==1)
				{
					if (NextHit.iPoint>0) 
						m_bTargetBall=8;
					else m_bTargetBall=1;
				}
				else 
				{
					m_bTargetBall=1;
				}
			}
		}
		if(m_bWhiteBallOver)//���û�в��������ͬʱ������˴�(����������ͬʱ����������Ĵ����ڲ���Ż�λ�ú����)
		{
			tagGS_CurrentPlayerStruct m_TakeWhiteUser;
			m_TakeWhiteUser.Seat=m_bNowHitUser;
			m_TakeWhiteUser.iPoint=NextHit.iPoint;
			bIsSendTakeWhite=TRUE;
			for (int i=0;i<m_PlaerNum;i++) 
			{
				//��������
				m_pITableFrame->SendTableData((bDeskStation+i)%2, ASS_TQ_TAKE_WHITE,&m_TakeWhiteUser,sizeof(m_TakeWhiteUser));
				m_pITableFrame->SendLookonData((bDeskStation+i)%2, ASS_TQ_TAKE_WHITE,&m_TakeWhiteUser,sizeof(m_TakeWhiteUser));
			}
			m_bWhiteBallOver=false;
			return TRUE;
		}
		for(int i=0;i<BALL_NUM;i++)
		{
			NextHit.bPlaceInfo[i].xPos=m_iXBallStation[i];	//�����Ϣ
			NextHit.bPlaceInfo[i].yPos=m_iYBallStation[i];
		}
		if(NextHit.iPoint<=0||NextHit.bHitPeople!=m_bNowHitUser)
			m_StarPos=255;
		NextHit.bTargerBall=m_bTargetBall;
		//NextHit.bNextHitPeople=m_bNextHitUser;
		NextHit.mStarPoint=m_StarPoint;
		NextHit.mStarPos=m_StarPos;
		if(bIsCanAddStar&&NextHit.iPoint>0&&NextHit.bHitPeople==m_bNowHitUser)
		{
			NextHit.bIsGetStarPoint=TRUE;
			NextHit.iPoint+=5;
			m_iUserPoint[m_bNowHitUser]+=5;
			bIsCanAddStar=FALSE;
			NextHit.bIsGetStarPoint=TRUE;
		}
		else
		{
			NextHit.bIsGetStarPoint=FALSE;
		}
		bIsCanAddStar=FALSE;
		NextHit.bHitPeople=m_bNowHitUser;
		for (int i=0;i<m_PlaerNum;i++)
		{
			//��������
			m_pITableFrame->SendTableData((bDeskStation+i)%2, ASS_TQ_NEXT_HIT, &NextHit,sizeof(NextHit));
			m_pITableFrame->SendLookonData((bDeskStation+i)%2, ASS_TQ_NEXT_HIT, &NextHit,sizeof(NextHit));
		}
		return TRUE;
	}
	return TRUE;
}
//���ð���
BOOL CTableFrameSink::OnPlaceWhite(WORD bDeskStation, tagCS_PlaceBallStruct * pPlaceWhite)
{
	//if (bDeskStation==m_bNowHitUser)
	//{
		//��������
		m_bBallAccess[0]=TRUE;
		m_iXBallStation[0]=pPlaceWhite->bPlaceInfo[0].xPos;
		m_iYBallStation[0]=pPlaceWhite->bPlaceInfo[0].yPos;

		//��������
		tagGS_PlaceInfoStruct PlaceResule;
		for(int i=0;i<BALL_NUM;i++)
		{
			PlaceResule.bPlaceInfo[i].xPos=m_iXBallStation[i];	//�����Ϣ
			PlaceResule.bPlaceInfo[i].yPos=m_iYBallStation[i];
		}
		PlaceResule.bHitPeople=m_bNowHitUser;
		PlaceResule.bTargerBall=m_bTargetBall;
		PlaceResule.bBallCount=1;
		PlaceResule.bPlaceInfo[0]=pPlaceWhite->bPlaceInfo[0];
		//int iSendSize=sizeof(PlaceResule)-sizeof(PlaceResule.bPlaceInfo)+sizeof(PlaceResule.bPlaceInfo[0]);
		for (int i=0;i<m_PlaerNum;i++) 
		{
			//��������
			m_pITableFrame->SendTableData((bDeskStation+i)%2, ASS_TQ_PLACE_WHITE, &PlaceResule, sizeof(PlaceResule));
			m_pITableFrame->SendLookonData((bDeskStation+i)%2, ASS_TQ_PLACE_WHITE, &PlaceResule, sizeof(PlaceResule));
		}
		
		return TRUE;
	//}
	//return FALSE;
}
//��Ϸ����
BOOL CTableFrameSink::GameFinish(WORD bDeskStation, int iCloseFlag)
{
	tagGS_GameFinishStruct Finish;
	tagScoreInfo	ScoreInfo_[GAME_PLAYER];
	Finish.Point[0]=0;
	Finish.Point[1]=0;
	Finish.FinishMode=0;
	if(m_PlaerNum==1)
	{
		if(iCloseFlag)
			Finish.FinishMode=2;
		iCloseFlag=-1;
		m_NowSetRule=bDeskStation;
	}
	Finish.m_NowHitUser=bDeskStation;
	int m_BasePoint=((m_iUserPoint[bDeskStation]-m_iUserPoint[(bDeskStation+1)%2])/10)*2;
	switch(iCloseFlag)
	{
	case 1:   //��������
		{
			Finish.FinishMode=2;
			m_NowSetRule=bDeskStation;
			if(m_iUserPoint[bDeskStation]>m_iUserPoint[(bDeskStation+1)%2])	
			{
				Finish.Point[bDeskStation]=2+m_BasePoint;
				Finish.Point[(bDeskStation+1)%2]=-2-m_BasePoint;
				ScoreInfo_[bDeskStation].ScoreKind = enScoreKind_Win;
				ScoreInfo_[(bDeskStation+1)%2].ScoreKind = enScoreKind_Lost;
			}
			else
			{
				if(m_iUserPoint[bDeskStation]==m_iUserPoint[(bDeskStation+1)%2])
				{
					Finish.Point[bDeskStation]=1;
					Finish.Point[(bDeskStation+1)%2]=1;
					ScoreInfo_[bDeskStation].ScoreKind = enScoreKind_Draw;
					ScoreInfo_[(bDeskStation+1)%2].ScoreKind = enScoreKind_Draw;
				}
				else
				{
					Finish.Point[bDeskStation]=m_BasePoint-2;
					Finish.Point[(bDeskStation+1)%2]=-m_BasePoint+2;
					ScoreInfo_[bDeskStation].ScoreKind = enScoreKind_Lost;
					ScoreInfo_[(bDeskStation+1)%2].ScoreKind = enScoreKind_Win;
				}
			}
			break;
		}
	case 0:      //����
		{
			m_NowSetRule=(bDeskStation+1)%2;
			Finish.Point[bDeskStation]=(m_BasePoint<0?m_BasePoint:0)-2;
			Finish.Point[(bDeskStation+1)%2]=(m_BasePoint<0?-m_BasePoint:0);
			ScoreInfo_[bDeskStation].ScoreKind = enScoreKind_Flee;
			ScoreInfo_[(bDeskStation+1)%2].ScoreKind = enScoreKind_Draw;
			break;
		}
	case 2:   //���
		{
			m_NowSetRule=bDeskStation;
			Finish.Point[bDeskStation]=m_BasePoint;
			Finish.Point[(bDeskStation+1)%2]=-m_BasePoint;
			ScoreInfo_[bDeskStation].ScoreKind = enScoreKind_Draw;
			ScoreInfo_[(bDeskStation+1)%2].ScoreKind = enScoreKind_Draw;
			break;
		}
	case 3:   //����
		{
			m_NowSetRule=(bDeskStation+1)%2;
			Finish.Point[bDeskStation]=(m_BasePoint<0?m_BasePoint:0)-2;
			Finish.FinishMode=1;
			Finish.Point[(bDeskStation+1)%2]=(m_BasePoint<0?-m_BasePoint:0);
			ScoreInfo_[bDeskStation].ScoreKind = enScoreKind_Lost;
			ScoreInfo_[(bDeskStation+1)%2].ScoreKind = enScoreKind_Win;
			break;
		}

	}
	if(m_PlaerNum!=1)
	{
		for (int i=0;i<GAME_PLAYER;i++)
		{
			if(iCloseFlag==1)
			{
				//������Ϸ��
////				if (m_pIGoldPresent!=NULL)
//				{
//					IServerUserItem *m_GoldPresentUser=m_pITableFrame->GetServerUserItem(i);
////					if(m_GoldPresentUser!=NULL)
////						m_pIGoldPresent->PresentGold(m_pITableFrame->GetServerUserItem(i),288,NULL);					
//				}
			}
			ScoreInfo_[i].lScore=Finish.Point[i];
		}
	}
	Finish.m_ReSetRule=m_NowSetRule;
	bIsSetRule=FALSE;
	if(m_PlaerNum!=1)
	{
			//ͳ�ƻ���
			LONG lScore=0;
			LONG lRevenue=0;
			enScoreKind ScoreKind;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				lScore = ScoreInfo_[i].lScore;
				ScoreKind = lScore > 0 ? enScoreKind_Win : enScoreKind_Lost;
				m_pITableFrame->WriteUserScore(i, lScore, lRevenue, ScoreKind );
			}
	}
	for(int i=0;i<m_PlaerNum;i++)
	{
		Finish.m_ReSetRule=(i+bDeskStation)%2;
		//��������
		m_pITableFrame->SendTableData((i+bDeskStation)%2, ASS_TQ_GAME_FINISH, &Finish,sizeof(Finish));
		m_pITableFrame->SendLookonData((i+bDeskStation)%2, ASS_TQ_GAME_FINISH, &Finish,sizeof(Finish));
	}
	m_PlaerNum=1;
	m_pITableFrame->ConcludeGame();
	m_pITableFrame->SetGameStatus(GS_TQ_WAIT_BEGIN);

	return true;
}
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if(!bLookonUser&&m_PlaerNum==1&&wChairID==m_bNowHitUser)
	{
		bIsSetRule=FALSE;
		InitData();
	}
	return true;
}
void CTableFrameSink::InitData()
{
	m_PlaerNum=1;
	//m_bNowHitUser=0;
	m_bTargetBall=1;
	::memset(m_bFinish,0,sizeof(m_bFinish));
	::memset(m_iUserPoint,0,sizeof(m_iUserPoint));
	::memset(m_bInBallIndex,0,sizeof(m_bInBallIndex));
	::memset(m_bBallAccess,0,sizeof(m_bBallAccess));
	memset(&pNowHitInfo,0,sizeof(pNowHitInfo));
	m_bWhiteBallOver=false;
	m_AgreeGoOn[0]=false;
	m_AgreeGoOn[1]=false;
	m_bIsPlayColor=false;
	m_bNextColor=0;
	m_bWhiteBallOver=false;
	bIsCanAddStar=FALSE;
	bIsHitFinish=FALSE;
	bIsSendTakeWhite=FALSE;
	m_StarPos=255;
	m_StarPoint=1;

	m_NowSetRule=0;
	m_bHitColor=FALSE;
	bHitColor=1;
	//��ʼ����
	m_iXBallStation[0]=140;	m_iYBallStation[0]=214;
	//��һ��
	m_iXBallStation[1]=535;	m_iYBallStation[1]=176;

	//�ڶ���
	m_iXBallStation[2]=556;	m_iYBallStation[2]=164;
	m_iXBallStation[3]=556;	m_iYBallStation[3]=188;

	//������
	m_iXBallStation[4]=577;	m_iYBallStation[4]=152;
	m_iXBallStation[5]=577;	m_iYBallStation[5]=176;
	m_iXBallStation[6]=577;	m_iYBallStation[6]=200;

	//������
	m_iXBallStation[7]=598;	m_iYBallStation[7]=140;
	m_iXBallStation[8]=598;	m_iYBallStation[8]=164;
	m_iXBallStation[9]=598;	m_iYBallStation[9]=188;
	m_iXBallStation[10]=598;	m_iYBallStation[10]=212;

	//������
	m_iXBallStation[11]=619;	m_iYBallStation[11]=128;
	m_iXBallStation[12]=619;	m_iYBallStation[12]=152;
	m_iXBallStation[13]=619;	m_iYBallStation[13]=176;
	m_iXBallStation[14]=619;	m_iYBallStation[14]=200;
	m_iXBallStation[15]=619;	m_iYBallStation[15]=224;

	//����
	m_iXBallStation[16]=140;	m_iYBallStation[16]=252;
	m_iXBallStation[17]=140;	m_iYBallStation[17]=110;
	m_iXBallStation[18]=140;	m_iYBallStation[18]=176;
	m_iXBallStation[19]=358;	m_iYBallStation[19]=176;
	m_iXBallStation[20]=506;	m_iYBallStation[20]=176;
	m_iXBallStation[21]=654;	m_iYBallStation[21]=176;
	for(int i=0;i<BALL_NUM;i++)
		m_bBallAccess[i]=TRUE;
}