#include "StdAfx.h"
#include "TableFrame.h"

//////////////////////////////////////////////////////////////////////////

//��ʱ�� ID
#define IDI_OFFLINE					(IDI_MAX_TIME_ID+1)					//���߶�ʱ��

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrame::CTableFrame()
{
	//���Ա���
	m_wChairCount=0;
	m_wTableID=INVALID_TABLE;

	//״̬����
	m_bTableLocked=false;
	m_bGameStarted=false;

	//״̬����
	m_dwTimeStart=0L;
	m_bGameStatus=GS_FREE;
	ZeroMemory(m_szPassword,sizeof(m_szPassword));

	//������Ϣ
	m_lGameTaxScore=0L;
	ZeroMemory(m_ScoreInfo,sizeof(m_ScoreInfo));
	ZeroMemory(m_dwPlayerID,sizeof(m_dwPlayerID));

	//�û���Ϣ
	ZeroMemory(m_pIUserItem,sizeof(m_pIUserItem));
	ZeroMemory(m_bAllowLookon,sizeof(m_bAllowLookon));
	ZeroMemory(m_wOffLineCount,sizeof(m_wOffLineCount));

	//���ñ���
	m_pGameServiceAttrib=NULL;
	m_pGameServiceOption=NULL;

	//�ӿڱ���
	m_pITableFrameSink=NULL;
	m_pITableUserAction=NULL;
	m_pIGameServiceFrame=NULL;

	return;
}

//��������
CTableFrame::~CTableFrame()
{
	//�ͷŶ���
	if (m_pITableFrameSink!=NULL)
	{
		m_pITableFrameSink->Release();
		m_pITableFrameSink=NULL;
	}

	return;
}

//�ӿڲ�ѯ
void * __cdecl CTableFrame::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrame,Guid,dwQueryVer);
	QUERYINTERFACE(ITableFrameManager,Guid,dwQueryVer);
	QUERYINTERFACE(ITableFrameControl,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameManager,Guid,dwQueryVer);
	return NULL;
}

//��ʼ��
bool __cdecl CTableFrame::InitTableFrame(WORD wTableID, tagTableFrameParameter * pTableFrameParameter)
{
	//Ч�����
	ASSERT(pTableFrameParameter!=NULL);

	//��������
	m_ClientReadyUser.RemoveAll();

	//���ñ���
	m_wTableID=wTableID;
	m_pGameServiceAttrib=pTableFrameParameter->pGameServiceAttrib;
	m_pGameServiceOption=pTableFrameParameter->pGameServiceOption;
	m_wChairCount=pTableFrameParameter->pGameServiceAttrib->wChairCount;

	//��ѯ�ӿ�
	m_pIGameServiceFrame=QUERY_OBJECT_PTR_INTERFACE(pTableFrameParameter->pIGameServiceFrame,IGameServiceFrame);

	//�����ж�
	if (m_pIGameServiceFrame==NULL)
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	IGameServiceManager * pIGameServiceManager=QUERY_OBJECT_PTR_INTERFACE(pTableFrameParameter->pIGameServiceManager,IGameServiceManager);
	m_pITableFrameSink=(ITableFrameSink *)pIGameServiceManager->CreateTableFrameSink(IID_ITableFrameSink,VER_ITableFrameSink);

	//�����ж�
	if (m_pITableFrameSink==NULL)
	{
		ASSERT(FALSE);
		return false;
	}

	//���ýӿ�
	IUnknownEx * pIUnknownEx=QUERY_ME_INTERFACE(IUnknownEx);
	if (m_pITableFrameSink->InitTableFrameSink(pIUnknownEx)==false) return false;

	//��չ�ӿ�
	m_pITableUserAction=QUERY_OBJECT_PTR_INTERFACE(m_pITableFrameSink,ITableUserAction);

	return true;
}

//��Ϸ״̬
bool __cdecl CTableFrame::IsUserPlaying(IServerUserItem * pIServerUserItem)
{
	//��Ϸ״̬
	if (m_bGameStarted==false) return false;

	//�û�״̬
	BYTE cbUserStatus=pIServerUserItem->GetUserStatus();
	if ((cbUserStatus!=US_PLAY)&&(cbUserStatus!=US_OFFLINE)) return false;

	//�߼��ж�
	WORD wChairID=pIServerUserItem->GetChairID();
	return m_pITableFrameSink->IsUserPlaying(wChairID);
}

//�뿪����
bool __cdecl CTableFrame::PerformStandUpAction(IServerUserItem * pIServerUserItem)
{
	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	ASSERT(pIServerUserItem->GetTableID()==m_wTableID);
	ASSERT(pIServerUserItem->GetChairID()<m_wChairCount);

	//��������
	WORD wChairID=pIServerUserItem->GetChairID();
	BYTE cbUserStatus=pIServerUserItem->GetUserStatus();
	IServerUserItem * pITableUserItem=GetServerUserItem(wChairID);

	//�û�����
	if (pITableUserItem==pIServerUserItem)
	{
		//��������
		bool bTableLocked=IsTableLocked();
		bool bGameStarted=IsGameStarted();

		//���ñ���
		m_bAllowLookon[wChairID]=false;
		m_ClientReadyUser.RemoveKey(pIServerUserItem->GetUserID());

		//������Ϸ
		if (IsUserPlaying(pIServerUserItem)==true)
		{
			//������Ϸ
			m_pITableFrameSink->OnEventGameEnd(wChairID,pIServerUserItem,GER_USER_LEFT);

			//�뿪�ж�
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if ((pUserData->wTableID==INVALID_TABLE)||(pUserData->wChairID==INVALID_CHAIR)) return true;
		}

		//��������
		CMD_GF_LookonControl LookonControl;
		ZeroMemory(&LookonControl,sizeof(LookonControl));

		//���ñ���
		LookonControl.dwUserID=0L;
		LookonControl.bAllowLookon=FALSE;

		//������Ϣ
		for (INT_PTR i=0;i<m_LookonUserItemPtr.GetCount();i++)
		{
			//��ȡ�û�
			IServerUserItem * pILookonUserItem=m_LookonUserItemPtr[i];

			//������Ϣ
			if ((pILookonUserItem->GetChairID()==wChairID)&&(IsClientReady(pILookonUserItem)==true))
			{
				m_pIGameServiceFrame->SendData(pILookonUserItem,MDM_GF_FRAME,SUB_GF_LOOKON_CONTROL,&LookonControl,sizeof(LookonControl));
			}
		}

		//�����û�
		m_pIUserItem[wChairID]=NULL;
		pIServerUserItem->SetUserStatus(US_FREE,INVALID_TABLE,INVALID_CHAIR);
		m_pIGameServiceFrame->SendUserStatus(pIServerUserItem);

		//ͳ������
		WORD wUserCount=0;
		for (WORD i=0;i<m_wChairCount;i++)
		{
			if (m_pIUserItem[i]!=NULL) wUserCount++;
		}

		//��������
		if (wUserCount==0)
		{
			m_bTableLocked=false;
			ZeroMemory(m_szPassword,sizeof(m_szPassword));
		}

		//�����Թ�
		if (wUserCount==0)
		{
			for (INT_PTR i=0;i<m_LookonUserItemPtr.GetCount();i++)
			{
				SendGameMessage(m_LookonUserItemPtr[i],TEXT("����Ϸ������������Ѿ��뿪�ˣ�"),SMT_CLOSE_GAME|SMT_EJECT|SMT_INFO);
			}
		}

		//����״̬
		if ((bTableLocked!=IsTableLocked())||(bGameStarted!=IsGameStarted()))
		{
			m_pIGameServiceFrame->SendTableStatus(m_wTableID);
		}

		//��������
		if (m_pITableUserAction!=NULL) m_pITableUserAction->OnActionUserStandUp(wChairID,pIServerUserItem,false);

		//��������
		bool bMatchServer=((m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)!=0);
		bool bControlStart=((bMatchServer==true)&&m_pGameServiceOption->cbControlStart==TRUE);

		//��ʼ�ж�
		if ((bControlStart==false)&&(StartVerdict()==true))
		{
			StartGame();
			return true;
		}
	}
	else
	{
		//�Թ��û�
		for (INT_PTR i=0;i<m_LookonUserItemPtr.GetCount();i++)
		{
			if (pIServerUserItem==m_LookonUserItemPtr[i])
			{
				//���ñ���
				m_ClientReadyUser.RemoveKey(pIServerUserItem->GetUserID());

				//�����û�
				m_LookonUserItemPtr.RemoveAt(i);
				pIServerUserItem->SetUserStatus(US_FREE,INVALID_TABLE,INVALID_CHAIR);
				m_pIGameServiceFrame->SendUserStatus(pIServerUserItem);

				//��������
				if (m_pITableUserAction!=NULL) m_pITableUserAction->OnActionUserStandUp(wChairID,pIServerUserItem,true);

				return true;
			}
		}
	}

	return true;
}

//�Թ۶���
bool __cdecl CTableFrame::PerformLookonAction(WORD wChairID, IServerUserItem * pIServerUserItem,LPCTSTR szPassword)
{
	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	ASSERT(wChairID<m_wChairCount);
	ASSERT(pIServerUserItem->GetTableID()==INVALID_TABLE);
	ASSERT(pIServerUserItem->GetChairID()==INVALID_CHAIR);

	//��������
	DWORD dwUserRight=pIServerUserItem->GetUserData()->dwUserRight;
	BYTE cbMasterOrder=pIServerUserItem->GetUserData()->cbMasterOrder;

	//Ȩ���ж�
	if (CUserRight::CanLookon(dwUserRight)==false)
	{
		//������Ϣ
		SendSitFailedPacket(pIServerUserItem,TEXT("��Ǹ����û�н����Թ���Ϸ��Ȩ�ޣ�����Ҫ����������ϵ��Ϸ�ͷ���ѯ��"));

		return false;
	}

	//�رղ�ѯ
	if ((cbMasterOrder==0L)&&(m_pIGameServiceFrame->IsShallClose()==true))
	{
		SendSitFailedPacket(pIServerUserItem,TEXT("���ڴ���Ϸ���伴����ͣ������Ҳ������ٽ�����Ϸ����"));
		return false;
	}

	//��ֹ��ѯ
	if ((cbMasterOrder==0L)&&(m_pIGameServiceFrame->IsAllowEnterGame()==false))
	{
		SendSitFailedPacket(pIServerUserItem,TEXT("��Ǹ������Ϸ�����ڲ�������ҽ��룡"));
		return false;
	}

	//λ���ж�
	if (m_pIUserItem[wChairID]==NULL)
	{
		SendSitFailedPacket(pIServerUserItem,TEXT("�������Թ۵�λ���Ѿ�û������ˣ������Թۣ�"));
		return false;
	}

	//����Ч��
	if ((m_bTableLocked==true)&&(cbMasterOrder==0L))
	{
		//����Ч��
		if (szPassword==NULL || lstrcmp(m_szPassword,szPassword)!=0)
		{
			LPCTSTR pszDescribe=TEXT("������󣬲��ܼ�����Ϸ��");
			SendSitFailedPacket(pIServerUserItem,pszDescribe);
			return false;
		}
	}

	//״̬�ж�
	if ((m_bGameStarted==false)&&(cbMasterOrder==0L))
	{
		SendSitFailedPacket(pIServerUserItem,TEXT("��Ϸ��û�п�ʼ����ʱ�����Թۣ�"));
		return false;
	}

	//�������
	m_LookonUserItemPtr.Add(pIServerUserItem);
	pIServerUserItem->SetUserStatus(US_LOOKON,m_wTableID,wChairID);
	m_pIGameServiceFrame->SendUserStatus(pIServerUserItem);

	//�Թ۴���
	if (m_pITableUserAction!=NULL) m_pITableUserAction->OnActionUserSitDown(wChairID,pIServerUserItem,true);

	return true;
}

//���¶���
bool __cdecl CTableFrame::PerformSitDownAction(WORD wChairID, IServerUserItem * pIServerUserItem, LPCTSTR szPassword)
{
	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	ASSERT(wChairID<m_wChairCount);
	ASSERT(pIServerUserItem->GetTableID()==INVALID_TABLE);
	ASSERT(pIServerUserItem->GetChairID()==INVALID_CHAIR);

	//��������
	const tagUserScore * pUserScore=pIServerUserItem->GetUserScore();
	const tagUserRule * pUserRule=pIServerUserItem->GetUserRule(),* pTableUserRule=NULL;
	tagServerUserData * pUserData=pIServerUserItem->GetUserData(),* pTableUserData=NULL;

	//�رղ�ѯ
	if ((pUserData->dwMasterRight==0L)&&(m_pIGameServiceFrame->IsShallClose()==true))
	{
		SendSitFailedPacket(pIServerUserItem,TEXT("���ڴ���Ϸ���伴����ͣ������Ҳ������ٽ�����Ϸ����"));
		return false;
	}

	//��ֹ��ѯ
	if ((pUserData->dwMasterRight==0L)&&(m_pIGameServiceFrame->IsAllowEnterGame()==false))
	{
		SendSitFailedPacket(pIServerUserItem,TEXT("��Ǹ������Ϸ�����ڲ�������ҽ��룡"));
		return false;
	}

	//λ���ж�
	if (m_pIUserItem[wChairID]!=NULL)
	{
		//�����ж�
		if ((m_pIUserItem[wChairID]->GetChairID()==wChairID)&&(m_pIUserItem[wChairID]->GetTableID()==m_wTableID))
		{
			//������Ϣ
			TCHAR szDescribe[128]=TEXT("");
			pTableUserData=m_pIUserItem[wChairID]->GetUserData();
			_snprintf(szDescribe,sizeof(szDescribe),TEXT("�����Ѿ��� [ %s ] �����ȵ��ˣ��´ζ���Ҫ����ˣ�"),pTableUserData->szAccounts);
			SendSitFailedPacket(pIServerUserItem,szDescribe);
		}
		else
		{
			ASSERT(FALSE);
		}

		return false;
	}

	//��Ϸ״̬
	if ((m_bGameStarted==true)&&(m_pGameServiceAttrib->cbJoinInGame==FALSE))
	{
		SendSitFailedPacket(pIServerUserItem,TEXT("��Ϸ�Ѿ���ʼ�ˣ���ʱ���ܽ�����Ϸ����"));
		return false;
	}

	//�����ж�
	if (m_pGameServiceOption->wServerType==GAME_GENRE_MATCH)
	{
		/*if(m_pGameServiceOption->wKindID==)
		{

		}*/
		//����Ȩ��
		if (CUserRight::IsMatchUser(pUserData->dwUserRight)==false)
		{
			TCHAR szDescribe[128]=TEXT("");
			lstrcpyn(szDescribe,TEXT("������Ϸ�������䣬�㲻�Ǳ���ѡ�֣�����������λ���ϣ� "),CountArray(szDescribe));
			SendSitFailedPacket(pIServerUserItem,szDescribe);
			return false;
		}

		//��ַЧ��
		DWORD dwUserIP=pIServerUserItem->GetClientIP();
		for (WORD i=0;i<m_wChairCount;i++)
		{
			if ((m_pIUserItem[i]!=NULL)&&(m_pIUserItem[i]->GetClientIP()==dwUserIP))
			{
				if (pUserRule->bCheckSameIP)
				{
					LPCTSTR pszDescribe=TEXT("�������˲�����ͬ IP ��ַ�������Ϸ������Ϸ���������� IP ��ַ��ͬ����ң����ܼ�����Ϸ��");
					SendSitFailedPacket(pIServerUserItem,pszDescribe);
					return false;
				}
				else
				{
					LPCTSTR pszDescribe=TEXT("���������˲�����ͬ IP ��ַ�������Ϸ������Ϸ���������� IP ��ַ��ͬ����ң����ܼ�����Ϸ��");
					SendSitFailedPacket(pIServerUserItem,pszDescribe);
					return false;
				}
			}
		}
		for (WORD i=0;i<m_wChairCount-1;i++)
		{
			if (m_pIUserItem[i]!=NULL)
			{
				for (WORD j=i+1;j<m_wChairCount;j++)
				{
					if ((m_pIUserItem[j]!=NULL)&&(m_pIUserItem[i]->GetClientIP()==m_pIUserItem[j]->GetClientIP()))
					{
						LPCTSTR pszDescribe=TEXT("�������˲�����ͬ IP ��ַ�������Ϸ������Ϸ������ IP ��ַ��ͬ����ң����ܼ�����Ϸ��");
						SendSitFailedPacket(pIServerUserItem,pszDescribe);
						return false;
					}
				}
			}
		}

		//�Ծ־���
		if (m_pGameServiceOption->lMatchDraw>0)
		{
			LONG lPlayCount=pUserData->UserScoreInfo.lWinCount+pUserData->UserScoreInfo.lLostCount+pUserData->UserScoreInfo.lDrawCount;
			if (lPlayCount>=m_pGameServiceOption->lMatchDraw)
			{
				TCHAR szDescribe[128]=TEXT("");
				lstrcpyn(szDescribe,TEXT("��ϲ�������ı��������Ѿ�����ˣ�����Ҫ�ټ��������������ĵȴ����������� "),CountArray(szDescribe));
				SendSitFailedPacket(pIServerUserItem,szDescribe);
				return false;
			}
		}
	}
	else
	{
		//Ȩ���ж�
		if (CUserRight::CanPlay(pUserData->dwUserRight)==false)
		{
			//������Ϣ
			LPCTSTR pszMessage=TEXT("��Ǹ����û�н�����Ϸ��Ȩ�ޣ�����Ҫ����������ϵ��Ϸ�ͷ���ѯ��");
			SendSitFailedPacket(pIServerUserItem,pszMessage);

			return true;
		}

		//��������//�޸�
		LONG lMaxScore=m_pGameServiceOption->lMaxScore;
		LONG lLessScore=m_pGameServiceOption->lLessScore;
		if (lLessScore!=0L || lMaxScore>lLessScore)
		{
			if (pUserData->UserScoreInfo.lScore<lLessScore)
			{
				//������Ϣ
				TCHAR szDescribe[128]=TEXT("");
				if (m_pGameServiceOption->wServerType==GAME_GENRE_GOLD)
				{
					_snprintf(szDescribe,sizeof(szDescribe),TEXT("������Ϸ������Ҫ %ld ����ң����Ľ�Ҳ��������ܼ��룡"),lLessScore);
				}
				else
				{
					_snprintf(szDescribe,sizeof(szDescribe),TEXT("������Ϸ������Ҫ %ld ����Ϸ���֣����Ļ��ֲ��������ܼ��룡"),lLessScore);
				}
				SendSitFailedPacket(pIServerUserItem,szDescribe);

				return false;
			}

			if (lMaxScore>lLessScore && lMaxScore<pUserData->UserScoreInfo.lScore)
			{
				//������Ϣ
				TCHAR szDescribe[128]=TEXT("");
				if (m_pGameServiceOption->wServerType==GAME_GENRE_GOLD)
				{
					_snprintf(szDescribe,sizeof(szDescribe),TEXT("���Ľ�ҳ����������������ƶ��%ld,�������Ϸ����!"),lMaxScore);
				}
				else
				{
					_snprintf(szDescribe,sizeof(szDescribe),TEXT("������Ϸ���ֳ����������������ƶ��%ld,�������Ϸ����!"),lMaxScore);
				}
				SendSitFailedPacket(pIServerUserItem,szDescribe);

				return false;
			}
		}

		//����Ч��
		if ((m_bTableLocked==true)&&(pUserData->dwMasterRight==0L))
		{
			if (szPassword==NULL || lstrcmp(m_szPassword,szPassword)!=0)
			{
				LPCTSTR pszDescribe=TEXT("������󣬲��ܼ�����Ϸ��");
				SendSitFailedPacket(pIServerUserItem,pszDescribe);
				return false;
			}
		}

		//���ַ�Χ
		WORD wWinRate=pIServerUserItem->GetUserWinRate();
		WORD wFleeRate=pIServerUserItem->GetUserFleeRate();
		for (WORD i=0;i<m_wChairCount;i++)
		{
			if (m_pIUserItem[i]!=NULL)
			{
				//��ȡ����
				pTableUserRule=m_pIUserItem[i]->GetUserRule();

				//����Ч��
				if ((pTableUserRule->bLimitFlee)&&(wFleeRate>pTableUserRule->wFleeRate))
				{
					TCHAR szDescribe[128]=TEXT("");
					_snprintf(szDescribe,sizeof(szDescribe),TEXT("���������̫�ߣ��� %s ���õ����ò��������ܼ�����Ϸ��"),m_pIUserItem[i]->GetAccounts());
					SendSitFailedPacket(pIServerUserItem,szDescribe);
					return false;
				}

				//ʤ��Ч��
				if ((pTableUserRule->bLimitWin)&&(wWinRate<pTableUserRule->wWinRate))
				{
					TCHAR szDescribe[128]=TEXT("");
					_snprintf(szDescribe,sizeof(szDescribe),TEXT("���ʤ��̫�ͣ��� %s ���õ����ò��������ܼ�����Ϸ��"),m_pIUserItem[i]->GetAccounts());
					SendSitFailedPacket(pIServerUserItem,szDescribe);
					return false;
				}

				//����Ч��
				if (pTableUserRule->bLimitScore==true)
				{
					if (pUserScore->lScore>pTableUserRule->lMaxScore)
					{
						TCHAR szDescribe[128]=TEXT("");
						_snprintf(szDescribe,sizeof(szDescribe),TEXT("��Ļ���̫�ߣ��� %s ���õ����ò��������ܼ�����Ϸ��"),m_pIUserItem[i]->GetAccounts());
						SendSitFailedPacket(pIServerUserItem,szDescribe);
						return false;
					}
					if (pUserScore->lScore<pTableUserRule->lLessScore)
					{
						TCHAR szDescribe[128]=TEXT("");
						_snprintf(szDescribe,sizeof(szDescribe),TEXT("��Ļ���̫�ͣ��� %s ���õ����ò��������ܼ�����Ϸ��"),m_pIUserItem[i]->GetAccounts());
						SendSitFailedPacket(pIServerUserItem,szDescribe);
						return false;
					}
				}
			}
		}

		//�����ж�
		if (m_pGameServiceOption->cbUnSameIPPlay==TRUE)
		{
			bool bPlay = true;
			DWORD dwUserIP=pIServerUserItem->GetClientIP();
			for (WORD i=0;i<m_wChairCount;i++)
			{
				if ((m_pIUserItem[i]!=NULL)&&(m_pIUserItem[i]->GetClientIP()==dwUserIP))
				{
					bPlay = false;
				}
			}
			if(bPlay)
			{
				for (WORD i=0;i<m_wChairCount-1;i++)
				{
					if (m_pIUserItem[i]==NULL)continue;

					for (WORD j=i+1;j<m_wChairCount;j++)
					{
						if ((m_pIUserItem[j]!=NULL)&&(m_pIUserItem[i]->GetClientIP()==m_pIUserItem[j]->GetClientIP()))
						{
							bPlay = false;
						}
					}
				}
			}

			if(!bPlay)
			{
				LPCTSTR pszDescribe=TEXT("��������������ͬIP��������ͬһ��,��ѡ��������λ!");
				SendSitFailedPacket(pIServerUserItem,pszDescribe);
				return false;
			}
		}

		//��ַЧ��
		bool bCheckSameIP=pUserRule->bCheckSameIP;
		for (WORD i=0;i<m_wChairCount;i++)
		{
			if (m_pIUserItem[i]!=NULL)
			{
				pTableUserRule=m_pIUserItem[i]->GetUserRule();
				if (pTableUserRule->bCheckSameIP==true) 
				{
					bCheckSameIP=true;
					break;
				}
			}
		}
		if (bCheckSameIP==true)
		{
			DWORD dwUserIP=pIServerUserItem->GetClientIP();
			for (WORD i=0;i<m_wChairCount;i++)
			{
				if ((m_pIUserItem[i]!=NULL)&&(m_pIUserItem[i]->GetClientIP()==dwUserIP))
				{
					if (pUserRule->bCheckSameIP)
					{
						LPCTSTR pszDescribe=TEXT("�������˲�����ͬ IP ��ַ�������Ϸ������Ϸ���������� IP ��ַ��ͬ����ң����ܼ�����Ϸ��");
						SendSitFailedPacket(pIServerUserItem,pszDescribe);
						return false;
					}
					else
					{
						LPCTSTR pszDescribe=TEXT("���������˲�����ͬ IP ��ַ�������Ϸ������Ϸ���������� IP ��ַ��ͬ����ң����ܼ�����Ϸ��");
						SendSitFailedPacket(pIServerUserItem,pszDescribe);
						return false;
					}
				}
			}
			for (WORD i=0;i<m_wChairCount-1;i++)
			{
				if (m_pIUserItem[i]!=NULL)
				{
					for (WORD j=i+1;j<m_wChairCount;j++)
					{
						if ((m_pIUserItem[j]!=NULL)&&(m_pIUserItem[i]->GetClientIP()==m_pIUserItem[j]->GetClientIP()))
						{
							LPCTSTR pszDescribe=TEXT("�������˲�����ͬ IP ��ַ�������Ϸ������Ϸ������ IP ��ַ��ͬ����ң����ܼ�����Ϸ��");
							SendSitFailedPacket(pIServerUserItem,pszDescribe);
							return false;
						}
					}
				}
			}
		}
	}

	//��������
	WORD wUserCount=0;
	for (WORD i=0;i<m_wChairCount;i++)
	{
		if (m_pIUserItem[i]!=NULL) wUserCount++;
	}
	if (wUserCount==0 && m_pGameServiceAttrib->wChairCount <= MAX_CHAIR_NORMAL)
	{
		m_bTableLocked=pUserRule->bPassword;
		if (m_bTableLocked==true) lstrcpyn(m_szPassword,pUserRule->szPassword,CountArray(m_szPassword));
	}

	//�������
	m_bAllowLookon[wChairID]=false;
	m_pIUserItem[wChairID]=pIServerUserItem;
	pIServerUserItem->SetUserStatus(US_SIT,m_wTableID,wChairID);

	//����״̬
	m_pIGameServiceFrame->SendTableStatus(m_wTableID);
	m_pIGameServiceFrame->SendUserStatus(pIServerUserItem);

	//���´���
	if (m_pITableUserAction!=NULL) m_pITableUserAction->OnActionUserSitDown(wChairID,pIServerUserItem,false);

	return true;
}

//�����¼�
bool __cdecl CTableFrame::OnUserOffLine(WORD wChairID)
{
	//Ч��״̬
	ASSERT(wChairID<m_wChairCount);
	ASSERT(m_pIUserItem[wChairID]!=NULL);

	//����״̬
	m_wOffLineCount[wChairID]++;
	m_pIUserItem[wChairID]->SetUserStatus(US_OFFLINE,m_wTableID,wChairID);
	m_pIGameServiceFrame->SendUserStatus(m_pIUserItem[wChairID]);

	//�����û�
	m_ClientReadyUser.RemoveKey(m_pIUserItem[wChairID]->GetUserID());

	//���ߴ���
	if (m_pITableUserAction!=NULL) m_pITableUserAction->OnActionUserOffLine(wChairID,m_pIUserItem[wChairID]);

	//���ö�ʱ��
	DWORD dwTimerID=IDI_OFFLINE+wChairID;
	WPARAM wBindParam=m_pIUserItem[wChairID]->GetUserID();
	m_pIGameServiceFrame->SetTableTimer(m_wTableID,dwTimerID,90000L,1,wBindParam);

	//������Ϣ
	TCHAR szMessage[512]=TEXT("");
	_sntprintf(szMessage,sizeof(szMessage),TEXT("[ %s ] �����ˣ������ĵȴ�90�롣"),m_pIUserItem[wChairID]->GetAccounts());

	//������Ϣ
	for (WORD i=0;i<MAX_CHAIR;i++)
	{
		if ((i!=wChairID)&&(m_pIUserItem[i]!=NULL))
		{
			SendGameMessage(m_pIUserItem[i],szMessage,SMT_INFO);
		}
	}

	return true;
}

//�ؽ��¼�
bool __cdecl CTableFrame::OnUserReConnect(WORD wChairID)
{
	//Ч��״̬
	ASSERT(wChairID<m_wChairCount);
	ASSERT(m_pIUserItem[wChairID]!=NULL);

	//ɾ����ʱ��
	m_pIGameServiceFrame->KillTableTimer(m_wTableID,IDI_OFFLINE+wChairID);

	//�����û�
	m_pIUserItem[wChairID]->SetUserStatus(US_PLAY,m_wTableID,wChairID);
	m_pIGameServiceFrame->SendUserStatus(m_pIUserItem[wChairID]);

	//���봦��
	if (m_pITableUserAction!=NULL) m_pITableUserAction->OnActionUserReConnect(wChairID,m_pIUserItem[wChairID]);

	return true;
}

//�������
bool __cdecl CTableFrame::OnUserReqOffLine(WORD wChairID)
{
	//Ч�����
	ASSERT(wChairID<m_wChairCount);
	ASSERT(m_pIUserItem[wChairID]!=NULL);
	if (wChairID>=m_wChairCount) return false;
	if (m_pIUserItem[wChairID]==NULL) return false;

	//����ģʽ
	if (m_pGameServiceOption->wServerType==GAME_GENRE_MATCH)
	{
		return m_wOffLineCount[wChairID]<5;
	}

	//����ģʽ
	return m_wOffLineCount[wChairID]<3;
}

//��ʱ���¼�
bool __cdecl CTableFrame::OnEventTimer(DWORD dwTimerID, WPARAM wBindParam)
{
	//���Ӷ�ʱ��
	if (dwTimerID>IDI_MAX_TIME_ID)
	{
		//���ߴ���
		if ((dwTimerID>=IDI_OFFLINE)&&(dwTimerID<(DWORD)(IDI_OFFLINE+m_wChairCount)))
		{
			//Ч��״̬
			ASSERT(m_bGameStarted==true);
			if (m_bGameStarted==false) return false;

			//��������
			WORD wChairID=(WORD)(dwTimerID-IDI_OFFLINE);
			IServerUserItem * pIServerUserItem=m_pIUserItem[wChairID];

			//���ߴ���
			if (pIServerUserItem!=NULL)
			{
				//״̬�ж�
				if (pIServerUserItem->GetUserID()!=wBindParam) return false;
				if (pIServerUserItem->GetUserStatus()!=US_OFFLINE) return false;

				//�û�����
				PerformStandUpAction(pIServerUserItem);

				//�����û�
				if (pIServerUserItem->IsActive()==true)
				{
					m_pIGameServiceFrame->DeleteUserItem(pIServerUserItem);
				}

				return true;
			}

			return false;
		}

		return false;
	}

	//��Ϸ��ʱ��
	return m_pITableFrameSink->OnTimerMessage((WORD)dwTimerID,wBindParam);
}

//��Ϸ�¼�����
bool __cdecl CTableFrame::OnEventSocketGame(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	ASSERT(m_pITableFrameSink!=NULL);

	//��Ϣ����
	return m_pITableFrameSink->OnGameMessage(wSubCmdID,pDataBuffer,wDataSize,pIServerUserItem);
}

//����¼�����
bool __cdecl CTableFrame::OnEventSocketFrame(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	ASSERT(m_pITableFrameSink!=NULL);

	//��Ϣ����
	bool bSuccess=m_pITableFrameSink->OnFrameMessage(wSubCmdID,pDataBuffer,wDataSize,pIServerUserItem);
	if (bSuccess==false)
	{
		switch (wSubCmdID)
		{
		case SUB_GF_INFO:	//��Ϸ��Ϣ
			{
				//Ч�����
				ASSERT(wDataSize==sizeof(CMD_GF_Info));
				if (wDataSize<sizeof(CMD_GF_Info)) return false;

				//��������
				CMD_GF_Info * pInfo=(CMD_GF_Info *)pDataBuffer;
				tagServerUserData * pUserData=pIServerUserItem->GetUserData();
				bool bLookonUser=(pUserData->cbUserStatus==US_LOOKON);

				//Ч��״̬
				ASSERT(pUserData->wChairID<m_wChairCount);
				if (pUserData->wChairID>=m_wChairCount) return false;

				//�����û�
				m_ClientReadyUser[pUserData->dwUserID]=pUserData->dwUserID;

				//���ñ���
				if (bLookonUser==false) m_bAllowLookon[pUserData->wChairID]=pInfo->bAllowLookon?true:false;

				//��������
				CMD_GF_Option Option;
				Option.bGameStatus=m_bGameStatus;
				Option.bAllowLookon=m_bAllowLookon[pUserData->wChairID]?TRUE:FALSE;
				m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_OPTION,&Option,sizeof(Option));

				//���ͳ���
				bool bSendSecret=((bLookonUser==false)||(m_bAllowLookon[pUserData->wChairID]==true));
				return m_pITableFrameSink->SendGameScene(pUserData->wChairID,pIServerUserItem,m_bGameStatus,bSendSecret);
			}
		case SUB_GF_USER_READY:	//�û�ͬ��
			{
				//��������
				tagServerUserData * pUserData=pIServerUserItem->GetUserData();
				bool bLookonUser=(pUserData->cbUserStatus==US_LOOKON);

				//״̬Ч��
				ASSERT(bLookonUser==false);
				ASSERT(m_pIUserItem[pUserData->wChairID]==pIServerUserItem);
				if (bLookonUser==true) return false;
				if (pUserData->cbUserStatus>=US_PLAY) return true;

				//���ñ���
				pUserData->cbUserStatus=US_READY;

				//ͬ�⴦��
				if (m_pITableUserAction!=NULL) 
				{
					m_pITableUserAction->OnActionUserReady(pUserData->wChairID,m_pIUserItem[pUserData->wChairID],(VOID *)pDataBuffer,wDataSize);
				}

				//��������
				bool bMatchServer=((m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)!=0);
				bool bControlStart=((bMatchServer==true)&&m_pGameServiceOption->cbControlStart==TRUE);

				//��ʼ�ж�
				if ((bControlStart==false)&&(StartVerdict()==true))
				{
					StartGame();
					return true;
				}

				//����״̬
				m_pIGameServiceFrame->SendUserStatus(pIServerUserItem);

				return true;
			}
		case SUB_GF_USER_CHAT:	//�û�����
			{
				//Ч�����
				CMD_GF_UserChat * pUserChat=(CMD_GF_UserChat *)pDataBuffer;
				ASSERT(wDataSize>=(sizeof(CMD_GF_UserChat)-sizeof(pUserChat->szChatMessage)));
				ASSERT(wDataSize==(sizeof(CMD_GF_UserChat)-sizeof(pUserChat->szChatMessage)+pUserChat->wChatLength));
				if (wDataSize<(sizeof(CMD_GF_UserChat)-sizeof(pUserChat->szChatMessage))) return false;
				if (wDataSize!=(sizeof(CMD_GF_UserChat)-sizeof(pUserChat->szChatMessage)+pUserChat->wChatLength)) return false;

				//��������
				bool bMatchServer=((m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)!=0);
				bool bControlStart=((bMatchServer==true)&&m_pGameServiceOption->cbControlStart==TRUE);

				//�������
				const tagServerUserData * pUserData=pIServerUserItem->GetUserData();
				if (pUserData->dwMasterRight!=0L)
				{
					//������ʼ
					if ((bMatchServer==true)&&(bControlStart=true)&&(lstrcmp(pUserChat->szChatMessage,TEXT("/:StartGame"))==0))
					{
						if (StartVerdict()==true) StartGame();
						return true;
					}

					//��Ϸ��ɢ
					if (lstrcmp(pUserChat->szChatMessage,TEXT("/:DismissGame"))==0)
					{
						DismissGame();
						return true;
					}
				}

				//�����Թ�
				if (pUserData->dwMasterRight==0L)
				{
					if ((bMatchServer==true)&&(pIServerUserItem->GetUserStatus()==US_LOOKON))
					{
						LPCTSTR pszMessage=TEXT("Ϊ�˲�Ӱ�����ѡ�ֱ������Թ۵��û���ֹ���ԣ�");
						SendGameMessage(pIServerUserItem,pszMessage,SMT_EJECT|SMT_INFO);
						return true;
					}
				}

				//Ȩ���ж�
				if (CUserRight::CanGameChat(pIServerUserItem->GetUserData()->dwUserRight)==false)
				{
					LPCTSTR pszMessage=TEXT("����ʱû�з��ͷ�����Ϣ��Ȩ�ޣ�ֻ�������Ա˽�ģ�");
					SendGameMessage(pIServerUserItem,pszMessage,SMT_EJECT|SMT_INFO);
					return true;
				}

				//״̬��ѯ
				if ((pUserData->dwMasterRight==0L)&&(m_pIGameServiceFrame->IsAllowGameChat()==false))
				{
					LPCTSTR pszMessage=TEXT("��Ǹ������Ϸ���䲻������������Ϣ��");
					SendGameMessage(pIServerUserItem,pszMessage,SMT_EJECT|SMT_INFO);
					return true;
				}

				//��Ϸ���
				for (WORD i=0;i<m_wChairCount;i++)
				{
					if ((m_pIUserItem[i]!=NULL)&&(IsClientReady(m_pIUserItem[i])==true))
					{
						m_pIGameServiceFrame->SendData(m_pIUserItem[i],MDM_GF_FRAME,SUB_GF_USER_CHAT,pUserChat,wDataSize);
					}
				}

				//�Թ����
				for (INT_PTR i=0;i<m_LookonUserItemPtr.GetCount();i++)
				{
					if (IsClientReady(m_LookonUserItemPtr[i])==true)
					{
						m_pIGameServiceFrame->SendData(m_LookonUserItemPtr[i],MDM_GF_FRAME,SUB_GF_USER_CHAT,pUserChat,wDataSize);
					}
				}

				return true;
			}
		case SUB_GF_LOOKON_CONTROL:	//�Թۿ���
			{
				//Ч�����
				ASSERT(wDataSize==sizeof(CMD_GF_LookonControl));
				if (wDataSize<sizeof(CMD_GF_LookonControl)) return false;

				//�û�Ч��
				tagServerUserData * pUserData=pIServerUserItem->GetUserData();
				if (pUserData->cbUserStatus==US_LOOKON) return false;

				//��������
				CMD_GF_LookonControl * pLookonControl=(CMD_GF_LookonControl *)pDataBuffer;

				//�Թ۴���
				if (pLookonControl->dwUserID!=0L)
				{
					for (INT_PTR i=0;i<m_LookonUserItemPtr.GetCount();i++)
					{
						//��ȡ�û�
						IServerUserItem * pILookonUserItem=m_LookonUserItemPtr[i];
						if (pILookonUserItem->GetUserID()!=pLookonControl->dwUserID) continue;
						if (pILookonUserItem->GetChairID()!=pIServerUserItem->GetChairID()) continue;

						//������Ϣ
						CMD_GF_LookonControl LookonControl;
						LookonControl.dwUserID=pLookonControl->dwUserID;
						LookonControl.bAllowLookon=pLookonControl->bAllowLookon;

						//������Ϣ
						if (IsClientReady(pILookonUserItem)==true)
						{
							m_pIGameServiceFrame->SendData(pILookonUserItem,MDM_GF_FRAME,SUB_GF_LOOKON_CONTROL,&LookonControl,sizeof(LookonControl));
						}

						break;
					}
				}
				else
				{
					//�����ж�
					bool bAllowLookon=(pLookonControl->bAllowLookon==TRUE)?true:false;
					if (bAllowLookon==m_bAllowLookon[pUserData->wChairID]) return true;
					
					//���ñ���
					m_bAllowLookon[pUserData->wChairID]=bAllowLookon;

					//������Ϣ
					CMD_GF_LookonControl LookonControl;
					LookonControl.dwUserID=pLookonControl->dwUserID;
					LookonControl.bAllowLookon=pLookonControl->bAllowLookon;

					//������Ϣ
					for (INT_PTR i=0;i<m_LookonUserItemPtr.GetCount();i++)
					{
						//��ȡ�û�
						IServerUserItem * pILookonUserItem=m_LookonUserItemPtr[i];
						if (pILookonUserItem->GetChairID()!=pIServerUserItem->GetChairID()) continue;

						//������Ϣ
						if (IsClientReady(pILookonUserItem)==true)
						{
							m_pIGameServiceFrame->SendData(pILookonUserItem,MDM_GF_FRAME,SUB_GF_LOOKON_CONTROL,&LookonControl,sizeof(LookonControl));
						}
					}
				}

				return true;
			}
		case SUB_GF_KICK_TABLE_USER:	//�����û�
			{
				//Ч�����
				ASSERT(wDataSize==sizeof(CMD_GF_KickTableUser));
				if (wDataSize!=sizeof(CMD_GF_KickTableUser)) return false;                
			
				//Ч���û�
				tagServerUserData * pUserData=pIServerUserItem->GetUserData();
				if((pUserData->cbMemberOrder==0)&&(pUserData->cbMasterOrder==0)) return false;

				//��Ϸ�ж�
				if ( pUserData->cbMasterOrder==0 && m_pGameServiceAttrib->wChairCount > MAX_CHAIR_NORMAL )
				{
					SendGameMessage(pIServerUserItem, TEXT("�Բ��𣬴˷��䲻�������ˣ�"), SMT_EJECT|SMT_INFO);
					return true;
				}

				//��������
				CMD_GF_KickTableUser * pKickTableUser=(CMD_GF_KickTableUser *)pDataBuffer;

				//�����û�
				IServerUserItem * pIKickUserItem=NULL;
				for (WORD i=0;i<m_wChairCount;i++)
				{
					if ((m_pIUserItem[i]!=NULL)&&(m_pIUserItem[i]->GetUserID()==pKickTableUser->dwUserID))
					{
						pIKickUserItem=m_pIUserItem[i];
						break;
					}
				}
				if (pIKickUserItem==NULL)
				{
					IServerUserItem * pTempUserItem=NULL;
					for (INT_PTR i=0;i<m_LookonUserItemPtr.GetCount();i++)
					{
						pTempUserItem=m_LookonUserItemPtr[i];
						if (pTempUserItem->GetUserID()==pKickTableUser->dwUserID)
						{
							pIKickUserItem=pTempUserItem;
							break;
						}
					}
				}

				//�����û�
				if (pIKickUserItem!=NULL)
				{	
					//��������
					tagServerUserData * pKickUserData=pIKickUserItem->GetUserData();
					tagServerUserData * pActionUserData=pIServerUserItem->GetUserData();

					//״̬�ж�
					if (pKickUserData->cbUserStatus>=US_PLAY) return true;

					//Ȩ���ж�
					bool bKickUser=false;
					if (bKickUser==false) bKickUser=(pActionUserData->cbMasterOrder>pKickUserData->cbMasterOrder);
					if (m_pGameServiceAttrib->wChairCount<=MAX_CHAIR_NORMAL && bKickUser==false)
						bKickUser=(pActionUserData->cbMemberOrder>pKickUserData->cbMemberOrder);
					if ( pActionUserData->cbMasterOrder<pKickUserData->cbMasterOrder ) bKickUser= false; 

					//�����û�
					if (bKickUser==true)
					{
						//�����ж�
						tagServerUserData *pKickUserData = pIKickUserItem->GetUserData();
						DWORD dwPassTime = pKickUserData->PropertyInfo[PROP_KICK].dwConsumedCount ;
						DWORD dwUseableTime = pKickUserData->PropertyInfo[PROP_KICK].dwPropCount;
						if ( 0 < dwUseableTime && dwPassTime <= dwUseableTime && pUserData->cbMasterOrder==0 )
						{
							TCHAR szDescribe[128]=TEXT("");
							_snprintf(szDescribe,sizeof(szDescribe),TEXT("[ %s ] ʹ���˷��߿�������ʧ�ܣ�"),pKickUserData->szAccounts);
							SendGameMessage(pIServerUserItem,szDescribe,SMT_EJECT|SMT_INFO);

							_snprintf(szDescribe,sizeof(szDescribe),TEXT("������ʹ���˷��߿����ɹ���ֹ��[ %s ]���������˶�����"),pUserData->szAccounts);
							SendGameMessage(pIKickUserItem,szDescribe,SMT_EJECT|SMT_INFO);

							return true;
						}

						TCHAR szDescribe[128]=TEXT("");
						_snprintf(szDescribe,sizeof(szDescribe),TEXT("���� [ %s ] �߳���Ϸ����"),pUserData->szAccounts);
						SendGameMessage(pIKickUserItem,szDescribe,SMT_EJECT|SMT_INFO|SMT_CLOSE_GAME);									
					}
				}    

				return true;
			}
		}
	}

	return bSuccess;
}

//��Ƶ�¼�����
bool CTableFrame::OnEventSocketVideo(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_GF_C_VIDEO_CONFIG:		//��Ƶ����
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_VideoConfig));
			if (wDataSize!=sizeof(CMD_C_VideoConfig)) return false;

			//��������
			CMD_C_VideoConfig * pVideoConfig=(CMD_C_VideoConfig *)pDataBuffer;

			//�����û�
			IServerUserItem * pIRemoteUserItem=NULL;
			for (WORD i=0;i<m_wChairCount;i++)
			{
				if ((m_pIUserItem[i]!=NULL)&&(m_pIUserItem[i]->GetUserID()==pVideoConfig->dwRemoteUserID))
				{
					pIRemoteUserItem=m_pIUserItem[i];
					break;
				}
			}

			//��Ϣ����
			if (pIRemoteUserItem!=NULL)
			{
				//��������
				CMD_S_VideoConfig VideoConfig;
				ZeroMemory(&VideoConfig,sizeof(VideoConfig));

				//���ñ���
				VideoConfig.wNatPort=pVideoConfig->wNatPort;
				VideoConfig.dwNatAddr=pVideoConfig->dwNatAddr;
				VideoConfig.wLocalPort=pVideoConfig->wLocalPort;
				VideoConfig.dwLocalAddr=pVideoConfig->dwLocalAddr;
				VideoConfig.dwLocalUserID=pIServerUserItem->GetUserID();

				//������Ϣ
				m_pIGameServiceFrame->SendData(pIRemoteUserItem,MDM_GF_VIDEO,SUB_GF_S_VIDEO_CONFIG,&VideoConfig,sizeof(VideoConfig));
			}

			return true;
		}
	case SUB_GF_C_VIDEO_OPEN:		//����Ƶ
		{
			//�Ϸ���֤
			WORD wOpenUserChairID=pIServerUserItem->GetChairID();
			if (pIServerUserItem->GetUserStatus()==US_LOOKON) return false;

			//��������
			CMD_S_VideoOpen VideoOpen;
			ZeroMemory(&VideoOpen,sizeof(VideoOpen));

			//���ñ���
			VideoOpen.wOpenUser=wOpenUserChairID;

			//������Ϣ
			for (WORD i=0;i<m_wChairCount;i++)
			{
				//��ȡ���
				IServerUserItem * pTargetServerUserItem=GetServerUserItem(i);
				if ((pTargetServerUserItem==NULL)||(pIServerUserItem==pTargetServerUserItem)) continue;

				//������Ϣ
				m_pIGameServiceFrame->SendData(pTargetServerUserItem,MDM_GF_VIDEO,SUB_GF_S_VIDEO_OPEN,&VideoOpen,sizeof(VideoOpen));
			}

			return true;
		}
	case SUB_GF_C_VIDEO_CLOSE:		//�ر���Ƶ
		{			
			//�Ϸ���֤
			WORD wCloseUserChairID=pIServerUserItem->GetChairID();
			if (pIServerUserItem->GetUserStatus()==US_LOOKON) return false;

			//��������
			CMD_S_VideoClose VideoClose;
			ZeroMemory( &VideoClose,sizeof(VideoClose));

			//���ñ���
			VideoClose.wCloseUser=wCloseUserChairID;

			//������Ϣ
			for (WORD i=0;i<m_wChairCount;i++)
			{
				//��ȡ���
				IServerUserItem * pTargetServerUserItem=GetServerUserItem(i);
				if ((pTargetServerUserItem==NULL)||(pIServerUserItem==pTargetServerUserItem)) continue;

				//������Ϣ
				m_pIGameServiceFrame->SendData(pTargetServerUserItem,MDM_GF_VIDEO,SUB_GF_S_VIDEO_CLOSE,&VideoClose,sizeof(VideoClose));
			}

			return true;
		}
	}

	return true;
}

//ö���û�
IServerUserItem * __cdecl CTableFrame::EnumLookonUserItem(WORD wIndex)
{
	if (wIndex>=m_LookonUserItemPtr.GetCount()) return NULL;
	return m_LookonUserItemPtr[wIndex];
}

//��ȡ�û�
IServerUserItem * __cdecl CTableFrame::GetServerUserItem(WORD wChairID)
{
	ASSERT(wChairID<m_wChairCount);
	if (wChairID>=m_wChairCount) return NULL;
	return m_pIUserItem[wChairID];
}

//���ö�ʱ��
bool __cdecl CTableFrame::SetGameTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM wBindParam)
{
	//Ч�����
	ASSERT(dwTimerID<=IDI_MAX_TIME_ID);
	if (dwTimerID>IDI_MAX_TIME_ID) return false;

	//���ö�ʱ��
	return m_pIGameServiceFrame->SetTableTimer(m_wTableID,dwTimerID,dwElapse,dwRepeat,wBindParam);
}

//ɾ����ʱ��
bool __cdecl CTableFrame::KillGameTimer(DWORD dwTimerID)
{
	//Ч�����
	ASSERT(dwTimerID<=IDI_MAX_TIME_ID);
	if (dwTimerID>IDI_MAX_TIME_ID) return false;

	//ɾ����ʱ��
	return m_pIGameServiceFrame->KillTableTimer(m_wTableID,dwTimerID);
}

//��������
bool __cdecl CTableFrame::SendUserData(IServerUserItem * pIServerUserItem, WORD wSubCmdID)
{
	//�û��ж�
	if (IsClientReady(pIServerUserItem)==false) return false;

	//������Ϣ
	m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID);

	return true;
}

//��������
bool __cdecl CTableFrame::SendUserData(IServerUserItem * pIServerUserItem, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	//�û��ж�
	if (IsClientReady(pIServerUserItem)==false) return false;

	//������Ϣ
	m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,pData,wDataSize);

	return true;
}

//��������
bool __cdecl CTableFrame::SendTableData(WORD wChairID, WORD wSubCmdID)
{
	//Ⱥ���û�
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<m_wChairCount;i++)
		{
			if ((m_pIUserItem[i]!=NULL)&&(IsClientReady(m_pIUserItem[i])==true))
			{
				m_pIGameServiceFrame->SendData(m_pIUserItem[i],MDM_GF_GAME,wSubCmdID);
			}
		}

		return true;
	}

	//��һ����
	if ((wChairID<m_wChairCount)&&(m_pIUserItem[wChairID]!=NULL))
	{
		if (IsClientReady(m_pIUserItem[wChairID])==true)
		{
			m_pIGameServiceFrame->SendData(m_pIUserItem[wChairID],MDM_GF_GAME,wSubCmdID);
			return true;
		}
	}

	return false;
}

//��������
bool __cdecl CTableFrame::SendTableData(WORD wChairID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	//Ⱥ���û�
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<m_wChairCount;i++)
		{
			if ((m_pIUserItem[i]!=NULL)&&(IsClientReady(m_pIUserItem[i])==true))
			{
				m_pIGameServiceFrame->SendData(m_pIUserItem[i],MDM_GF_GAME,wSubCmdID,pData,wDataSize);
			}
		}

		return true;
	}

	//��һ����
	if ((wChairID<m_wChairCount)&&(m_pIUserItem[wChairID]!=NULL))
	{
		if (IsClientReady(m_pIUserItem[wChairID])==true)
		{
			m_pIGameServiceFrame->SendData(m_pIUserItem[wChairID],MDM_GF_GAME,wSubCmdID,pData,wDataSize);
			return true;
		}
	}

	return false;
}

//��������
bool __cdecl CTableFrame::SendLookonData(WORD wChairID, WORD wSubCmdID)
{
	for (INT_PTR i=0;i<m_LookonUserItemPtr.GetCount();i++)
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_LookonUserItemPtr[i];

		//������Ϣ
		if (IsClientReady(pIServerUserItem)==true)
		{
			//��һ����
			if (wChairID<m_wChairCount)
			{
				tagServerUserData * pUserData=pIServerUserItem->GetUserData();
				if (pUserData->wChairID==wChairID) m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID);
			}

			//��������
			if (wChairID==INVALID_CHAIR) m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID);
		}
	}

	return true;
}

//��������
bool __cdecl CTableFrame::SendLookonData(WORD wChairID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	for (INT_PTR i=0;i<m_LookonUserItemPtr.GetCount();i++)
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_LookonUserItemPtr[i];

		//������Ϣ
		if (IsClientReady(pIServerUserItem)==true)
		{
			//��һ����
			if (wChairID<m_wChairCount)
			{
				tagServerUserData * pUserData=pIServerUserItem->GetUserData();
				if (pUserData->wChairID==wChairID) m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,pData,wDataSize);
			}

			//��������
			if (wChairID==INVALID_CHAIR) m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,pData,wDataSize);
		}
	}

	return true;
}

//���ͷ�����Ϣ
bool __cdecl CTableFrame::SendRoomMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType)
{
	//�û��ж�
	if (IsClientReady(pIServerUserItem)==false) return false;

	//�������ݰ�
	CMD_GR_Message Message;
	Message.wMessageType=wMessageType;
	lstrcpyn(Message.szContent,lpszMessage,CountArray(Message.szContent));
	Message.wMessageLength=CountStringBuffer(Message.szContent);

	//��������
	WORD wSendSize=sizeof(Message)-sizeof(Message.szContent)+Message.wMessageLength*sizeof(TCHAR);
	m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GR_SYSTEM,SUB_GR_MESSAGE,&Message,wSendSize);

	return true;
}

//������Ϸ��Ϣ
bool __cdecl CTableFrame::SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType)
{
	//�û��ж�
	if (IsClientReady(pIServerUserItem)==false) return false;

	//�������ݰ�
	CMD_GF_Message Message;
	Message.wMessageType=wMessageType;
	lstrcpyn(Message.szContent,lpszMessage,CountArray(Message.szContent));
	Message.wMessageLength=CountStringBuffer(Message.szContent);

	//��������
	WORD wSendSize=sizeof(Message)-sizeof(Message.szContent)+Message.wMessageLength*sizeof(TCHAR);
	m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_MESSAGE,&Message,wSendSize);

	return true;
}

//��ɢ��Ϸ
bool __cdecl CTableFrame::DismissGame()
{
	//״̬�ж�
	ASSERT(m_bGameStarted==true);
	if (m_bGameStarted==false) return false;

	//������Ϸ
	if ((m_bGameStarted==true)&&(m_pITableFrameSink->OnEventGameEnd(INVALID_CHAIR,NULL,GER_DISMISS)==false))
	{
		ASSERT(FALSE);
		return false;
	}

	//����״̬
	if (m_bGameStarted==false)
	{
		//���ñ���
		m_bGameStarted=false;

		//����״̬
		m_pIGameServiceFrame->SendTableStatus(m_wTableID);
	}

	return true;
}

//������Ϸ
bool __cdecl CTableFrame::ConcludeGame()
{
	//����״̬
	m_bGameStarted=false;
	m_bGameStatus=GS_FREE;

	//���ñ���
	m_dwTimeStart=0L;
	ZeroMemory(m_wOffLineCount,sizeof(m_wOffLineCount));

	//������Ϣ
	m_lGameTaxScore=0L;
	ZeroMemory(m_ScoreInfo,sizeof(m_ScoreInfo));
	ZeroMemory(m_dwPlayerID,sizeof(m_dwPlayerID));

	//��������
	m_pITableFrameSink->RepositTableFrameSink();

	//�����û�
	for (WORD i=0;i<m_wChairCount;i++)
	{
		if (m_pIUserItem[i]!=NULL)
		{
			tagServerUserData * pUserData=m_pIUserItem[i]->GetUserData();
			if (pUserData->cbUserStatus!=US_OFFLINE) pUserData->cbUserStatus=US_SIT;
		}
	}

	//���ߴ���
	for (WORD i=0;i<m_wChairCount;i++)
	{
		if (m_pIUserItem[i]!=NULL)
		{
			//��ȡ�û�
			IServerUserItem * pIServerUserItem=m_pIUserItem[i];
			if (pIServerUserItem->GetUserStatus()!=US_OFFLINE) continue;
			
			//ɾ����ʱ��
			DWORD dwTimerID=IDI_OFFLINE+i;
			m_pIGameServiceFrame->KillTableTimer(m_wTableID,dwTimerID);

			//�û��뿪
			PerformStandUpAction(pIServerUserItem);

			//�����û�
			if (pIServerUserItem->IsActive()==true)
			{
				m_pIGameServiceFrame->DeleteUserItem(pIServerUserItem);
			}
		}
	}

	//�����ж�
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)&&(m_pGameServiceOption->lMatchDraw>0))
	{
		for (WORD i=0;i<m_wChairCount;i++)
		{
			if (m_pIUserItem[i]!=NULL)
			{
				//��������
				IServerUserItem * pIServerUserItem=m_pIUserItem[i];
				tagServerUserData * pUserData=pIServerUserItem->GetUserData();
				LONG lPlayCount=pUserData->UserScoreInfo.lWinCount+pUserData->UserScoreInfo.lLostCount+pUserData->UserScoreInfo.lDrawCount;

				//�����ж�
				if (lPlayCount>=m_pGameServiceOption->lMatchDraw)
				{
					//������Ϣ
					TCHAR szDescribe[256]=TEXT("");
					lstrcpyn(szDescribe,TEXT("��ϲ�㣬��ı��������Ѿ�����ˣ�����Ҫ�ټ��������������ĵȴ����������� "),CountArray(szDescribe));
					SendGameMessage(pIServerUserItem,szDescribe,SMT_EJECT|SMT_INFO|SMT_CLOSE_GAME);

					//�û�����
					PerformStandUpAction(pIServerUserItem);
				}
			}
		}
	}

	//������//�޸�
	LONG lMaxScore=m_pGameServiceOption->lMaxScore;
	LONG lLessScore=m_pGameServiceOption->lLessScore;
	if (lLessScore!=0L || lMaxScore>lLessScore)
	{
		for (WORD i=0;i<m_wChairCount;i++)
		{
			if (m_pIUserItem[i]!=NULL)
			{
				//��ȡ�û�
				IServerUserItem * pIServerUserItem=m_pIUserItem[i];
				tagServerUserData * pUserData=pIServerUserItem->GetUserData();

				//�����ж�
				if (pUserData->UserScoreInfo.lScore<lLessScore)
				{
					//������Ϣ
					TCHAR szDescribe[256]=TEXT("");
					if (m_pGameServiceOption->wServerType==GAME_GENRE_GOLD)
					{
						_snprintf(szDescribe,sizeof(szDescribe),TEXT("���Ľ������ %ld�����ܼ�����Ϸ��"),lLessScore);
					}
					else 
					{
						_snprintf(szDescribe,sizeof(szDescribe),TEXT("������Ϸ�������� %ld�����ܼ�����Ϸ��"),lLessScore);
					}
					SendGameMessage(pIServerUserItem,szDescribe,SMT_EJECT|SMT_INFO|SMT_CLOSE_GAME);
					
					//�����û�
					PerformStandUpAction(pIServerUserItem);
				}
				//����ж�
				else if (lMaxScore>lLessScore && lMaxScore<pUserData->UserScoreInfo.lScore)
				{
					//������Ϣ
					TCHAR szDescribe[256]=TEXT("");
					if (m_pGameServiceOption->wServerType==GAME_GENRE_GOLD)
					{
						_snprintf(szDescribe,sizeof(szDescribe),TEXT("���Ľ�ҳ����������������ƶ��%ld,�������Ϸ����!"),lMaxScore);
					}
					else 
					{
						_snprintf(szDescribe,sizeof(szDescribe),TEXT("������Ϸ���ֳ����������������ƶ��%ld,�������Ϸ����!"),lMaxScore);
					}
					SendGameMessage(pIServerUserItem,szDescribe,SMT_EJECT|SMT_INFO|SMT_CLOSE_GAME);
					
					//�����û�
					PerformStandUpAction(pIServerUserItem);
				}
			}
		}
	}

	//�ر��ж�
	if ((m_pIGameServiceFrame->IsAllowEnterGame()==false)||(m_pIGameServiceFrame->IsShallClose()==true))
	{
		for (WORD i=0;i<m_wChairCount;i++)
		{
			if (m_pIUserItem[i]!=NULL)
			{
				//��ȡ�û�
				IServerUserItem * pIServerUserItem=m_pIUserItem[i];
				tagServerUserData * pUserData=pIServerUserItem->GetUserData();

				//�û��ж�
				if (pUserData->dwMasterRight==0L)
				{
					//�û�����
					LPCTSTR pszMessage=TEXT("��ע�⣬��Ϸ���伴���رջ��߲�������ҽ��룬�����뿪��Ϸ���ӣ�");
					SendGameMessage(pIServerUserItem,pszMessage,SMT_EJECT|SMT_INFO|SMT_CLOSE_GAME);
					PerformStandUpAction(pIServerUserItem);

					continue;
				}
			}
		}
	}

	//�û�״̬
	for (WORD i=0;i<m_wChairCount;i++)
	{
		if (m_pIUserItem[i]!=NULL)
		{
			m_pIGameServiceFrame->SendUserStatus(m_pIUserItem[i]);
		}
	}

	//����״̬
	m_pIGameServiceFrame->SendTableStatus(m_wTableID);

	return true;
}

//д�����
bool __cdecl CTableFrame::WriteUserScore(WORD wChairID, LONG lScore, LONG lRevenue, enScoreKind ScoreKind, LONG lPlayTimeCount)
{
	//Ч�����
	ASSERT(wChairID<m_wChairCount);
	ASSERT(m_pIUserItem[wChairID]!=NULL);
	if (wChairID>=m_wChairCount) return false;
	if (m_pIUserItem[wChairID]==NULL) return false;

	//���ñ���
	m_ScoreInfo[wChairID].lScore=lScore;
	m_ScoreInfo[wChairID].lRevenue=lRevenue;
	m_ScoreInfo[wChairID].ScoreKind=ScoreKind;

	//д�����
	WriteUserScore(m_pIUserItem[wChairID],lScore,lRevenue,ScoreKind, lPlayTimeCount);

	return true;
}

//д�����
bool __cdecl CTableFrame::WriteUserScore(IServerUserItem * pIServerUserItem, LONG lScore, LONG lRevenue, enScoreKind ScoreKind, LONG lPlayTimeCount)
{
	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	tagScoreInfo ScoreInfo;
	ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));

	//�����ж�
	if ( m_pGameServiceOption->wServerType != GAME_GENRE_GOLD && m_pGameServiceOption->wServerType != GAME_GENRE_MATCH )
	{
		tagServerUserData *pServerUserData = pIServerUserItem->GetUserData();
		CString strMessage;
		if ( 0 < lScore )
		{
			DWORD lPassedTime, lUseableTime;
			BYTE cbPropertyType = PROP_DOUBLE;
			lPassedTime = pServerUserData->PropertyInfo[PROP_DOUBLE].dwConsumedCount;
			lUseableTime = pServerUserData->PropertyInfo[PROP_DOUBLE].dwPropCount;
			if ( lUseableTime == 0 || lUseableTime <= lPassedTime )
			{
				lPassedTime = pServerUserData->PropertyInfo[PROP_FOURDOLD].dwConsumedCount;
				lUseableTime = pServerUserData->PropertyInfo[PROP_FOURDOLD].dwPropCount;
				cbPropertyType = PROP_FOURDOLD;
			}
			if ( 0 < lUseableTime && lPassedTime <= lUseableTime ) 
			{
				if ( cbPropertyType == PROP_DOUBLE )
				{
					lScore *= 2;
					strMessage.Format(TEXT("[ %s ] ʹ����[ ˫�����ֿ� ]���÷ַ�����)"), 
						pServerUserData->szAccounts);
				}
				else if ( cbPropertyType == PROP_FOURDOLD )
				{
					lScore *= 4;
					strMessage.Format(TEXT("[ %s ] ʹ����[ �ı����ֿ� ]���÷ַ��ı���)"), 
						pServerUserData->szAccounts);
				}
			}
		}
		else if ( lScore < 0 )
		{
			DWORD lPassedTime, lUseableTime;

			lPassedTime = pServerUserData->PropertyInfo[PROP_SHIELD].dwConsumedCount;
			lUseableTime = pServerUserData->PropertyInfo[PROP_SHIELD].dwPropCount;
			if ( 0 < lUseableTime && lPassedTime <= lUseableTime ) 
			{
				lScore = 0;

				strMessage.Format(TEXT("[ %s ] ʹ����[ ������� ]�����ֲ��䣡"), pServerUserData->szAccounts);
			}
		}

		if ( ! strMessage.IsEmpty() )
		{
			//��Ϸ���
			for (WORD i=0;i<m_wChairCount;i++)
			{
				if ((m_pIUserItem[i]!=NULL)&&(IsClientReady(m_pIUserItem[i])==true))
				{
					SendGameMessage(m_pIUserItem[i], strMessage, SMT_INFO);
				}
			}

			//�Թ����
			for (INT_PTR i=0;i<m_LookonUserItemPtr.GetCount();i++)
			{
				if (IsClientReady(m_LookonUserItemPtr[i])==true)
				{
					SendGameMessage(m_LookonUserItemPtr[i], strMessage, SMT_INFO);
				}
			}
		}
	}

	//���ñ���
	ScoreInfo.lScore=lScore;
	ScoreInfo.lRevenue=lRevenue;
	ScoreInfo.ScoreKind=ScoreKind;

	//��Ϸ˰��
	m_lGameTaxScore+=lRevenue;

	//�޸Ļ���
	if ( lPlayTimeCount == -1 )
	{
		DWORD dwPlayTimeCount=0L;
		if (m_dwTimeStart!=0L) dwPlayTimeCount=(DWORD)time(NULL)-m_dwTimeStart;
		pIServerUserItem->WriteScore(ScoreInfo,dwPlayTimeCount);
	}
	else
	{
		pIServerUserItem->WriteScore(ScoreInfo,DWORD(lPlayTimeCount));
	}

	//������Ϣ
	m_pIGameServiceFrame->SendUserScore(pIServerUserItem);

	//�洢����
	if ((m_pGameServiceOption->wServerType==GAME_GENRE_GOLD)&&(m_pGameServiceOption->lRestrictScore>0L))
	{
		//��ԭ����
		tagServerUserData * pServerUserData=pIServerUserItem->GetUserData();
		pServerUserData->UserScoreInfo.lScore+=pServerUserData->lStorageScore;
		pServerUserData->lStorageScore=0L;

		//�洢����
		if (pServerUserData->UserScoreInfo.lScore>m_pGameServiceOption->lRestrictScore)
		{
			LONG lUserScore=pServerUserData->UserScoreInfo.lScore;
			pServerUserData->UserScoreInfo.lScore=m_pGameServiceOption->lRestrictScore;
			pServerUserData->lStorageScore=lUserScore-m_pGameServiceOption->lRestrictScore;
		}
	}

	return true;
}

//���ͳ���
bool __cdecl CTableFrame::SendGameScene(IServerUserItem * pIServerUserItem, void * pData, WORD wDataSize)
{
	//ϵͳ��Ϣ
	TCHAR szMessage[256]=TEXT("");
	if (m_pGameServiceOption->wServerType==GAME_GENRE_MATCH)
	{
		_snprintf(szMessage,sizeof(szMessage),TEXT("�װ����û�����ӭ��������%s��������Ϸ���䣬ף����ѡ������ˮƽ��ȡ�úõĳɼ���"),m_pGameServiceAttrib->szKindName);
		SendGameMessage(pIServerUserItem,szMessage,SMT_INFO);
	}
	else
	{
		_snprintf(szMessage,sizeof(szMessage),TEXT("�װ����û�����ӭ��������%s������ӭ����ᱦ���飡"),m_pGameServiceAttrib->szKindName);
		SendGameMessage(pIServerUserItem,szMessage,SMT_INFO);
	}

	//��ȫ��ʾ
	if ((pIServerUserItem->GetUserStatus()!=US_LOOKON)&&(m_bAllowLookon[pIServerUserItem->GetChairID()]==true))
	{
		lstrcpyn(szMessage,TEXT("��ע�⣬������������������ҹۿ�����Ϸ��"),CountArray(szMessage));
		SendGameMessage(pIServerUserItem,szMessage,SMT_INFO);
	}

	//���ͳ���
	m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_SCENE,pData,wDataSize);

	return true;
}

//��ʼ��Ϸ
bool __cdecl CTableFrame::StartGame()
{
	//Ч��״̬
	ASSERT(m_bGameStarted==false);
	if (m_bGameStarted==true) return false;

	//���ñ���
	m_bGameStarted=true;
	m_dwTimeStart=(DWORD)time(NULL);
	ZeroMemory(m_wOffLineCount,sizeof(m_wOffLineCount));

	//��¼����
	m_lGameTaxScore=0L;
	ZeroMemory(m_ScoreInfo,sizeof(m_ScoreInfo));
	ZeroMemory(m_dwPlayerID,sizeof(m_dwPlayerID));

	//�������
	tagServerUserData * pUserData=NULL;
	for (WORD i=0;i<m_wChairCount;i++)
	{
		if (m_pIUserItem[i]!=NULL)
		{
			pUserData=m_pIUserItem[i]->GetUserData();
			pUserData->cbUserStatus=US_PLAY;
			m_dwPlayerID[i]=pUserData->dwUserID;
		}
	}

	//����״̬
	m_pIGameServiceFrame->SendTableStatus(m_wTableID);

	//�û�״̬
	for (WORD i=0;i<m_wChairCount;i++)
	{
		if (m_pIUserItem[i]!=NULL)
		{
			m_pIGameServiceFrame->SendUserStatus(m_pIUserItem[i]);
		}
	}

	//֪ͨ�¼�
	m_pITableFrameSink->OnEventGameStart();

	return true;
}

//�жϿ�ʼ
bool __cdecl CTableFrame::StartVerdict()
{
	//�����ж�
	if (m_bGameStarted==true) return false;

	//ʱ��ģʽ
	enStartMode StartMode=m_pITableFrameSink->GetGameStartMode();
	if (StartMode==enStartMode_TimeControl) return false;

	//׼������
	WORD wReadyUserCount=0;
	tagServerUserData * pUserData=NULL;
	for (WORD i=0;i<m_wChairCount;i++) 
	{
		if (m_pIUserItem[i]!=NULL)
		{
			wReadyUserCount++;
			pUserData=m_pIUserItem[i]->GetUserData();
			if (pUserData->cbUserStatus!=US_READY) return false;
		}
	}

	//�����ж�
	if (wReadyUserCount>1L)
	{
		if (StartMode==enStartMode_Symmetry)
		{
			if ((wReadyUserCount%2)!=0) return false;
			if (wReadyUserCount==m_wChairCount) return true;
			WORD wHalfCount=m_wChairCount/2;
			for (WORD i=0;i<wHalfCount;i++)
			{
				if ((m_pIUserItem[i]==NULL)&&(m_pIUserItem[i+wHalfCount]!=NULL)) return false;
				if ((m_pIUserItem[i]!=NULL)&&(m_pIUserItem[i+wHalfCount]==NULL)) return false;
			}
			return true;
		}
		else 
		{
			if (StartMode==enStartMode_FullReady) return (wReadyUserCount==m_wChairCount);
			if (StartMode==enStartMode_AllReady) return true;
		}
	}

	//�����ж�
	if ((wReadyUserCount==1)&&(m_pGameServiceAttrib->wChairCount==1)) return true;

	return false;
}

//����λ��
bool __cdecl CTableFrame::SwitchUserChair(WORD wSourceID[], WORD wTargetID[], WORD wSwitchCount)
{
	//Ч������
	ASSERT((wSwitchCount>0)&&(wSwitchCount<m_wChairCount));
	if ((wSwitchCount==0)||(wSwitchCount>=m_wChairCount)) return false;

	//�����ж�
	for (WORD i=0;i<wSwitchCount;i++)
	{
		//�����ж�
		WORD j=0;
		for (;j<wSwitchCount;j++)
		{
			if (wSourceID[i]==wTargetID[j]) break;
		}

		//�����ж�
		if (j==wSwitchCount)
		{
			ASSERT(FALSE);
			return false;
		}
	}

	//�ظ��ж�
	for (WORD i=0;i<wSwitchCount;i++)
	{
		//�����ж�
		ASSERT(wSourceID[i]<m_wChairCount);
		if (wSourceID[i]>=m_wChairCount) return false;

		//�����ж�
		ASSERT(m_pIUserItem[wSourceID[i]]!=NULL);
		if (m_pIUserItem[wSourceID[i]]==NULL) return false;

		//�ظ��ж�
		for (WORD j=0;j<wSwitchCount;j++)
		{
			if ((i!=j)&&(wSourceID[i]==wSourceID[j]))
			{
				ASSERT(FALSE);
				return false;
			}
		}
	}

	//��������
	BYTE cbUserStatus[MAX_CHAIR];
	IServerUserItem	* pIUserItem[MAX_CHAIR];

	//�û�����
	for (WORD i=0;i<wSwitchCount;i++)
	{
		//��������
		WORD wSourceChairID=wSourceID[i];

		//�����û�
		pIUserItem[wSourceChairID]=m_pIUserItem[wSourceChairID];
		cbUserStatus[wSourceChairID]=m_pIUserItem[wSourceChairID]->GetUserData()->cbUserStatus;

		//���ñ���
		m_pIUserItem[wSourceChairID]=NULL;
		pIUserItem[wSourceChairID]->SetUserStatus(US_FREE,INVALID_TABLE,INVALID_CHAIR);

		//����״̬
		m_pIGameServiceFrame->SendUserStatus(pIUserItem[wSourceChairID]);
	}

	//�û�����
	for (WORD i=0;i<wSwitchCount;i++)
	{
		//��������
		WORD wSourceChairID=wSourceID[i];
		WORD wTargetChairID=wTargetID[i];

		//���ñ���
		m_pIUserItem[wTargetChairID]=pIUserItem[wSourceChairID];
		pIUserItem[wSourceChairID]->SetUserStatus(cbUserStatus[wSourceChairID],m_wTableID,wTargetChairID);

		//����״̬
		m_pIGameServiceFrame->SendUserStatus(pIUserItem[wSourceChairID]);
	}

	return true;
}

//��ȡ��λ
WORD CTableFrame::GetNullChairID()
{
	//��������
	for (WORD i=0;i<m_wChairCount;i++)
	{
		if (m_pIUserItem[i]==NULL)
		{
			return i;
		}
	}

	return INVALID_CHAIR;
}

//�Ƿ����
bool CTableFrame::IsClientReady(IServerUserItem * pIServerUserItem)
{
	//�û��ж�
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//�û��ж�
	DWORD dwUserIDMap=0L;
	DWORD dwUserID=pIServerUserItem->GetUserID();
	if (m_ClientReadyUser.Lookup(dwUserID,dwUserIDMap)==FALSE) return false;

	return true;
}

//��������ʧ��
void CTableFrame::SendSitFailedPacket(IServerUserItem * pIServerUserItem, LPCTSTR pszFailedDescribe)
{
	//��������
	CMD_GR_SitFailed SitFailed;
	ZeroMemory(&SitFailed,sizeof(SitFailed));
	lstrcpyn(SitFailed.szFailedDescribe,pszFailedDescribe,CountArray(SitFailed.szFailedDescribe));

	//��������
	WORD wSendSize=sizeof(SitFailed)-sizeof(SitFailed.szFailedDescribe)+CountStringBuffer(SitFailed.szFailedDescribe);
	m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_SIT_FAILED,&SitFailed,wSendSize);

	return;
}

//////////////////////////////////////////////////////////////////////////
