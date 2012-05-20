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

	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	return true;
}

//��λ����
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	TRACE("CTableFrameSink::RepositTableFrameSink");
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
	TRACE("OnEventGameStart");
	//����״̬
	m_pITableFrame->SetGameStatus(GS_PLAYING);
	int nIndex=m_GameLogic.StartANewGame();
	for(int i=0;i<m_wPlayerCount;i++)
	{
		struct tagSetInitPisa tInit;
		m_GameLogic.GetInitPisa(i,&tInit);
		//��ʼ��ÿ����ҵ���
//		m_pITableFrame->SendUserData(pUser,MSG_SET_INITPISA,(void *)&tInit,sizeof(struct tagSetInitPisa));
		m_pITableFrame->SendTableData(i,MSG_SET_INITPISA,(void *)&tInit,sizeof(struct tagSetInitPisa));
		m_pITableFrame->SendLookonData(i,MSG_SET_INITPISA,(void *)&tInit,sizeof(struct tagSetInitPisa));
	}
	//�����к���3�����ȳ���
	struct tagSetFocus focus;
	focus.bClean=TRUE;
	focus.nFoucsId=nIndex;
	SendUserDataToAll(MSG_SET_FOCUS,(void *)&focus,sizeof(focus));
	//�������ĳ��Ƽ�¼
	m_GameLogic.EmptyLastCardInfo();
	return true;
}

//��Ϸ���� wChairIDΪ���ȳ������
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
		{
			//�������
			struct tagGameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));

			//��������
			LONG lCellScore=m_pGameServiceOption->lCellScore;
			tagScoreInfo ScoreInfo[m_wPlayerCount];
			memset(&ScoreInfo,0,sizeof(ScoreInfo));

			//ͳ�ƻ���
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (i!=wChairID)
				{
					int LeftCard=m_GameLogic.GetPlayerLeftCard(i);
					GameEnd.lGameScore[i]-=LeftCard*lCellScore;
					ScoreInfo[i].lScore-=LeftCard*lCellScore;
					GameEnd.lGameScore[wChairID]+=LeftCard*lCellScore;
					ScoreInfo[i].lScore += LeftCard*lCellScore;
				}
			}

			//����˰��
			if (m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)
			{
				if (GameEnd.lGameScore[wChairID]>=100L)
				{
					LONG lTax=GameEnd.lGameScore[wChairID]*m_pGameServiceOption->wRevenue/100L;
					GameEnd.lGameTax+=lTax;
					ScoreInfo[wChairID].lRevenue=lTax;
					GameEnd.lGameScore[wChairID]=GameEnd.lGameScore[wChairID]-lTax;
				}
			}

			//������Ϣ
			SendUserDataToAll(MSG_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,MSG_GAME_END,&GameEnd,sizeof(GameEnd));

			for (WORD i=0;i<m_wPlayerCount;i++)
			{				
				ScoreInfo[i].ScoreKind=(GameEnd.lGameScore[i]>0L)?enScoreKind_Win:enScoreKind_Lost;
				m_pITableFrame->WriteUserScore(i,ScoreInfo[i].lScore,ScoreInfo[i].lRevenue,ScoreInfo[i].ScoreKind);
			}
			//m_pITableFrame->WriteTableScore(ScoreInfo,m_wPlayerCount,GameEnd.lGameTax);

			//������Ϸ
			m_pITableFrame->ConcludeGame();
			return true;
		}
	case GER_USER_LEFT:		//�û�ǿ��
		{
			//Ч�����
			ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<m_wPlayerCount);

			//��������
			struct tagGameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));
			GameEnd.lGameScore[wChairID]=-20*m_pGameServiceOption->lCellScore;//���ܿ�20������

			//������Ϣ
			SendUserDataToAll(MSG_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,MSG_GAME_END,&GameEnd,sizeof(GameEnd));

			//�޸Ļ���
			tagScoreInfo ScoreInfo;
			memset(&ScoreInfo,0,sizeof(ScoreInfo));
			ScoreInfo.ScoreKind=enScoreKind_Draw;
			ScoreInfo.lScore=GameEnd.lGameScore[wChairID];
			m_pITableFrame->WriteUserScore(wChairID,ScoreInfo.lScore,0L,ScoreInfo.ScoreKind);

			//������Ϸ
			m_pITableFrame->ConcludeGame();
			return true;
		}
	}
	ASSERT(FALSE);
	return false;
}

//���ͳ���
bool __cdecl CTableFrameSink::SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret)
{
	//TRACE("CTableFrameSink::SendGameScene:ChairID:%d,GameStatus:%d",wChiarID,bGameStatus);
	return m_pITableFrame->SendGameScene(pIServerUserItem,NULL,0);
	//Ч����
	//ASSERT(FALSE);
	return true;
	//return false;
}

//��ʱ���¼�
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
	return false;
}

//��Ϸ��Ϣ����
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	TRACE("CTableFrameSink::OnGameMessage,SubCmdID:%d",wSubCmdID);
	//����ύ��pass���߳��Ʋ���
	if (wSubCmdID==MSG_SET_OUTPISA)
	{
		ASSERT(wDataSize==sizeof(struct tagSetOutPisa));
		if (wDataSize!=sizeof(struct tagSetOutPisa)) return false;
		struct tagSetOutPisa *pOut=(struct tagSetOutPisa *)pDataBuffer;
		ASSERT(pOut!=NULL);
		pOut->nChairId=pIServerUserItem->GetChairID();
		TRACE("�յ��û���%d�ĳ�����Ϣ\n",pOut->nChairId);

		//������pass
		if (pOut->nCount==0)
		{
			//�������ҿ�ʼ�����ƣ���Ҳ�����pass
			if (m_GameLogic.IsUserBegin(pOut->nChairId))
			{
				//Pass�������س�����Ϣ
				struct tagOutPisaMsg out;
				memset(&out,0,sizeof(out));
				out.nResult=1;
				strcpy(out.chErrMsg,"����Pass�Լ������ƣ�");
				m_pITableFrame->SendUserData(pIServerUserItem,MSG_OUTPISA_MSG,(void *)&out,sizeof(out));
			}
			else
			{
				//֪ͨ������Ҹ����pass
				struct tagSetOtherPisa pisa;
				pisa.nChairId=pOut->nChairId;
				pisa.nCount=0;
				SendUserDataToAll(MSG_SET_OTHERPISA,(void *)&pisa,sizeof(pisa));
				//������һ�����Ϊ����
				int index=(pOut->nChairId+1)%m_wPlayerCount;
				TRACE("������һ��%dΪ����\n",index);
				struct tagSetFocus focus;
				//����Ѿ�Pass�ֻص�������
				focus.bClean=m_GameLogic.IsUserBegin(index)?TRUE:FALSE;
				focus.nFoucsId=index;
				SendUserDataToAll(MSG_SET_FOCUS,(void *)&focus,sizeof(focus));
			}
		}
		else
		{
			if (m_GameLogic.IsUserBegin(pOut->nChairId))
				m_GameLogic.EmptyLastCardInfo();
			if (m_GameLogic.IsBiggerThanLastCard(pOut))
			{
				//֪ͨ��ҳ���ͨ��
				struct tagOutPisaMsg out;
				memset(&out,0,sizeof(out));
				out.nResult=0;
				m_pITableFrame->SendUserData(pIServerUserItem,MSG_OUTPISA_MSG,(void *)&out,sizeof(out));
				//֪ͨ������ҳ���ʲô��
				struct tagSetOtherPisa pisa;
				pisa.nChairId=pOut->nChairId;
				pisa.nCount=pOut->nCount;
				for(int i=0;i<pOut->nCount;i++)
					pisa.tOutPisa[i]=pOut->tOutPisa[i];
				SendUserDataToAll(MSG_SET_OTHERPISA,(void *)&pisa,sizeof(pisa));
				//������������Ϣ
				if (!m_GameLogic.SaveLastCard(pOut))
				{
					//��ʾ������Ѿ�������������,�����Ϸ
					OnEventGameEnd(pOut->nChairId,NULL,GER_NORMAL);
				}
				else
				{
					//������һ�����Ϊ����
					int index=(pOut->nChairId+1)%m_wPlayerCount;
					TRACE("������һ��%dΪ����\n",index);
					struct tagSetFocus focus;
					focus.bClean=FALSE;
					focus.nFoucsId=index;
					SendUserDataToAll(MSG_SET_FOCUS,(void *)&focus,sizeof(focus));
				}
			}
			else
			{
				//���Ƴ������س�����Ϣ
				struct tagOutPisaMsg out;
				memset(&out,0,sizeof(out));
				out.nResult=1;
				strncpy(out.chErrMsg,m_GameLogic.GetCompareError(),sizeof(out.chErrMsg)-1);
				m_pITableFrame->SendUserData(pIServerUserItem,MSG_OUTPISA_MSG,(void *)&out,sizeof(out));
			}
		}
		return true;
	}
	else
	{
		TRACE("Warning:receive other gamemessage.");
	}
	return true;
}

//�����Ϣ����
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	//m_GameLogic.StartANewGame();
	//TRACE("CTableFrameSink::OnFrameMessage:SubCmdID:%d",wSubCmdID);
	//return true;
	return false;
}

//////////////////////////////////////////////////////////////////////////

//��������ҷ��͵���Ϣ
void CTableFrameSink::SendUserDataToAll(WORD wSubCmdId,void *pBuffer,int nLen)
{
	m_pITableFrame->SendTableData(INVALID_CHAIR,wSubCmdId,pBuffer,nLen);
//	IServerUserItem *pUserItem;
/*	for(int i=0;i<m_wPlayerCount;i++)
	{
		//pUserItem=m_pITableFrame->GetServerUserItem(i);
		//ASSERT(pUserItem!=NULL);
		//m_pITableFrame->SendUserData(pUserItem,wSubCmdId,pBuffer,nLen);
		m_pITableFrame->SendTableData(i,wSubCmdId,pBuffer,nLen);
	}*/
}
