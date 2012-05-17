#include "StdAfx.h"
#include "TableFrameSink.h"

int g_PrintLogFile=1;
void WriteLog(CString strFileName, CString strText)
{
//�ж��Ƿ��ӡ��־�ļ�
	if ( g_PrintLogFile != 1)
		return;

	try
	{
        CTime tm = CTime::GetCurrentTime();
        CString strTime = tm.Format(_T("%Y-%m-%d %H:%M:%S"));
		//BOOL bFull = FALSE;
		CStdioFile file;
		if( file.Open(strFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite) != 0)
		{
			file.SeekToEnd();
			file.WriteString(strTime);
			file.WriteString(strText);
			file.WriteString(_T("\n\n"));
			//if(file.GetLength() > 2000000)
			//	bFull = TRUE;
			file.Close();
		}
		/*
		if(!bFull) return;
		if( file.Open(strFileName, CFile::modeCreate|CFile::modeReadWrite) != 0)
		{
			file.SeekToEnd();
			file.WriteString(strTime);
			file.WriteString(strText);
			file.WriteString(_T("\n"));
			file.Close();
		}
		*/
	}
	catch(...)
	{
	}
}

//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_FullReady;	//��ʼģʽ

//��̬����

//�齫����
BYTE CTableFrameSink::m_byMjs[ORG_MJ_MAX] = 
{
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,				//��
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,				//��
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,				//��
	0x31, 0x32, 0x33, 0x34,												//��
	0x41, 0x42, 0x43,													//��
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,				//��
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,				//��
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,				//��
	0x31, 0x32, 0x33, 0x34,												//��
	0x41, 0x42, 0x43,													//��
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,				//��
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,				//��
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,				//��
	0x31, 0x32, 0x33, 0x34,												//��
	0x41, 0x42, 0x43,													//��
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,				//��
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,				//��
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,				//��
	0x31, 0x32, 0x33, 0x34,												//��
	0x41, 0x42, 0x43													//��
};

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ����
	m_wBankerUser = 0;
	m_wCurrentUser = INVALID_CHAIR;
	m_wLastUser    = INVALID_CHAIR;
	m_strFang= NULL;
	m_wLostUser= INVALID_CHAIR;
	m_wQFeng = 0;
	m_bZiM= false;
	//m_stEnd.clear();
	for( int i=0; i<m_wPlayerCount; i++ )
	{
		m_nFang[i]= 0;
		m_ActionRight[i] = 0;
		m_GameLogic.CleanHu( &m_hu[i] );
	}
	
	//������Ϣ
	m_wTurnWiner  = INVALID_CHAIR;
	m_byOutMj	  = 255;

	//�齫��Ϣ
	memset( m_byHandMjs, 0, sizeof( m_byHandMjs ) );

	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	//û������
	m_nTouchMjPos  = 0;
	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
	if( m_strFang)
	{
		delete []m_strFang;
	}

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
	//��Ϸ����
	//m_wBankerUser = 0;
	//m_wQFeng= 0;
	m_wCurrentUser = INVALID_CHAIR;
	m_wLastUser    = INVALID_CHAIR;

	m_wLostUser= INVALID_CHAIR;
	m_bZiM= false;
	m_strFang= NULL;
	//m_stEnd.clear();
	for( int i=0; i<m_wPlayerCount; i++ )
	{
 		m_nFang[i]= 0;
		m_ActionRight[i] = 0;
		m_GameLogic.CleanHu( &m_hu[i] );
	}

	//������Ϣ
	m_wTurnWiner  = INVALID_CHAIR;
	m_byOutMj = 255;

	//�齫��Ϣ
	memset( m_byHandMjs, 0, sizeof( m_byHandMjs ) );

	//û������
	m_nTouchMjPos  = 0;
	
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
	CMD_S_GameStart GameStart;
	//���������
	GameStart.nSartNumber1 = m_GameLogic.ActionDice();
	GameStart.nSartNumber2 = m_GameLogic.ActionDice();
	GameStart.wBankerUser= m_wBankerUser;
	GameStart.wQuanFeng= m_wQFeng;
	m_pITableFrame->SendTableData( INVALID_CHAIR, SUB_S_GAME_START, &GameStart, sizeof( GameStart));


	//Sleep( 3000 );
	//����״̬
	m_pITableFrame->SetGameStatus(GS_PLAYING);

	//��ʼ������
	m_byTouchMjCount = ORG_MJ_MAX;
	//�����齫
	m_GameLogic.RandMj( m_byMjs, ORG_MJ_MAX );
	//log
	for( int i=0; i<ORG_MJ_MAX; i++ )
	{
		REPORT_V( log, "i", i, 1 );
		REPORT_V( log, "���Һ�mj", m_byMjs[i], 1 );
	}
	log.DrawLine( 3, 1 );
	


	//test
	//BYTE mjs[14] = { 0x01,0x09,0x11,0x19,0x21,0x29,0x31,0x32,0x33,0x34,0x41,0x42,0x43,0x01 };
	//�ַ��齫
	CMD_S_SendMj	SendMj;
	SendMj.wBankerUser = m_wBankerUser;
	SendMj.byStartMjPos = m_nTouchMjPos;
	for( WORD i=0; i<m_wPlayerCount; i++ )
	{
		if( i== m_wBankerUser )
		{
			m_byHandMjCount[i] = HAND_MJ_MAX;
			//CopyMemory( &m_byHandMjs[i], &m_byMjs[0], sizeof(BYTE)*m_byHandMjCount[i] );	
			CopyMemory( &m_byHandMjs[i], &m_byMjs[i*(HAND_MJ_MAX-1)],  HAND_MJ_MAX-1 );	
			m_byHandMjs[i][HAND_MJ_MAX-1]=m_byMjs[52];
			//test
			//CopyMemory( &m_byHandMjs[i], &mjs, sizeof( BYTE)*m_byHandMjCount[i]);
			m_GameLogic.SortMj( m_byHandMjs[i], m_byHandMjCount[i], m_byHandMjCount[i] );
		}
		else
		{
			m_byHandMjCount[i] = HAND_MJ_MAX-1;
			CopyMemory( &m_byHandMjs[i], &m_byMjs[i*(HAND_MJ_MAX-1)],  HAND_MJ_MAX-1 );
			/*CopyMemory( &m_byHandMjs[i], &m_byMjs[i*m_byHandMjCount[i]+1], sizeof(BYTE)*m_byHandMjCount[i] );*/
			//test
		//	CopyMemory( &m_byHandMjs[i], &mjs, sizeof( BYTE)*m_byHandMjCount[i]);
			m_byHandMjs[i][HAND_MJ_MAX-1] = 0;
			m_GameLogic.SortMj( m_byHandMjs[i], m_byHandMjCount[i], m_byHandMjCount[i]  );
		//	m_GameLogic.LMove( m_byHandMjs[i], HAND_MJ_MAX-1 );	
		}	
	}	

	//���ñ���
	m_nTouchMjPos = 53;
	m_byTouchMjCount -= 53;

	//�����û�
	m_wCurrentUser = m_wBankerUser;
	//�����졢�ء��˺���ʼ��
	m_bTianHu=true;
	for(WORD i=0;i<m_wPlayerCount;i++)
	{
		if(i==m_wBankerUser)
		{
			m_bDiHu[i]=false;
			m_bRenHu[i]=false;
		}
		else
		{
			m_bDiHu[i]=true;
			m_bRenHu[i]=true;
		}
	}
	
	//�����齫��
	for( WORD i=0; i<m_wPlayerCount; i++ )
	{
		SendMj.byMjCount = m_byHandMjCount[i];
		CopyMemory( &SendMj.byMjs, &m_byHandMjs[i], sizeof(BYTE)*m_byHandMjCount[i] );
		if( i == m_wBankerUser )
		{
			SendMj.nHu = m_GameLogic.IsHu( m_byHandMjs[i], m_byHandMjCount[i]-1, m_byHandMjs[i][HAND_MJ_MAX-1], m_hu[i],&m_tempHu[i] );
			SendMj.nGang = m_GameLogic.IsAnGang( m_byHandMjs[i], m_byHandMjCount[i] ).GangStyle;
			if( SendMj.nGang)
				m_ActionRight[i] = 4;
			if( SendMj.nHu)
				m_ActionRight[i] = 6;
		}
		else
		{
			SendMj.nHu = false;
			SendMj.nGang = false;
		}

		m_pITableFrame->SendTableData( i, SUB_S_SEND_MJ, &SendMj, sizeof( SendMj ) );
		m_pITableFrame->SendLookonData( i, SUB_S_SEND_MJ, &SendMj, sizeof( SendMj ) );
	}
	return true;
}

//��Ϸ����
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sOnEventGameEnd.log",strTime);

	strTemp.Format("into OnEventGameEnd");
	WriteLog(strFile, strTemp);
	switch( cbReason )
	{
		//�������
	case GER_NORMAL:
		{
			//�������
			CMD_S_GameEnd GameEnd;
			memset( &GameEnd, 0, sizeof(CMD_S_GameEnd));

			//��������
			LONG lCeelScore=m_pGameServiceOption->lCellScore/512;
			bool bBankerWin=(m_wBankerUser==wChairID)?true:false;

			//ͳ�Ʒ���
			//strcpy( &GameEnd.strEnd, m_strFang );
			//int a = strlen(m_strFang);
			///int b = strlen(GameEnd.strEnd);
			//����ͳ��
			CopyMemory( &GameEnd.strEnd, m_strFang, strlen(m_strFang));
			//����
			for( int k=0; k<4; k++ )
			{
				CopyMemory( &GameEnd.byMjs[k], m_byHandMjs[k], sizeof(BYTE)*14);
			}
			for( WORD i=0; i<m_wPlayerCount;i++)
			{
				if( i==m_wTurnWiner )
				{
					//����
					if( m_bZiM)
					{
						//��������
						if( m_nFang==0)
						{
							GameEnd.lGameScore[i] = lCeelScore*3;
						}
						else
						{
							GameEnd.lGameScore[i] = lCeelScore*m_GameLogic.Pow( 2, m_nFang[i] )*3;
						}
						
					}
					else
					{
						//��������
						if( m_nFang==0)
						{
							GameEnd.lGameScore[i] = lCeelScore;
						}
						else
						{
							GameEnd.lGameScore[i] = m_GameLogic.Pow( 2, m_nFang[i] )*lCeelScore;
						}
						
					}
				}
				else 
				{
					//���������,������
					if( m_bZiM)
					{
						//��������
						if( m_nFang==0)
						{
							GameEnd.lGameScore[i] = (-1)*lCeelScore;
						}
						else
						{
							GameEnd.lGameScore[i] = (-1)*m_GameLogic.Pow( 2, m_nFang[m_wTurnWiner] )*lCeelScore;
						}
						
					}
					else
					{
						if( i==m_wLastUser)
						{
							//��������
							if( m_nFang==0)
							{
								GameEnd.lGameScore[i] = (-1)*lCeelScore;
							}
							else
							{
								GameEnd.lGameScore[i] = (-1)*m_GameLogic.Pow( 2, m_nFang[m_wTurnWiner] )*lCeelScore;
							}
							
						}
					}
				}
			}

			// ��̬˰��
			long myTax = m_pGameServiceOption->wRevenue;

			strTemp.Format(" ˰��=%d", myTax);
			WriteLog(strFile, strTemp);
			//����˰��
			if( m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)
			{
				for( WORD i=0; i<m_wPlayerCount; i++ )
				{
					//Doctor if (GameEnd.lGameScore[i]>=100L)
					//��˰�����յ� 10
					if (GameEnd.lGameScore[i]>= TAX_MIN_NUM)
					{/*
						GameEnd.lGameTax+=GameEnd.lGameScore[i]/50L;
						GameEnd.lGameScore[i]=GameEnd.lGameScore[i]*49L/50L;
						*/

						GameEnd.lGameTax+=GameEnd.lGameScore[i] * myTax / 100L;

						strTemp.Format("GameEnd.lGameScore[%d]=%d,GameEnd.lGameTax=%d", i, GameEnd.lGameScore[i], GameEnd.lGameTax);
						WriteLog(strFile, strTemp);

						GameEnd.lGameScore[i]=GameEnd.lGameScore[i]*(100-myTax)/100L;


						strTemp.Format("GameEnd.lGameScore[%d]=%d", i, GameEnd.lGameScore[i]);
						WriteLog(strFile, strTemp);
					}
				}
			}

			//������Ϣ
			m_pITableFrame->SendTableData( INVALID_CHAIR, SUB_S_GAME_END, &GameEnd, sizeof(CMD_S_GameEnd)+strlen(GameEnd.strEnd)+1);
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_GAME_END, &GameEnd, sizeof(CMD_S_GameEnd)+strlen(GameEnd.strEnd)+1);

			//ׯλת��
			if( m_wTurnWiner != m_wBankerUser )
			{	
				if( m_wBankerUser==3)
				{
					m_wQFeng= (m_wQFeng+1)%4;
				}
				m_wBankerUser= (m_wBankerUser+3)%4;

			}
			//�޸Ļ���
			tagScoreInfo ScoreInfo[m_wPlayerCount];
			memset(&ScoreInfo, 0, sizeof(ScoreInfo));
			for( WORD i=0;i<m_wPlayerCount;i++ )
			{
				ScoreInfo[i].lScore = GameEnd.lGameScore[i];
				ScoreInfo[i].ScoreKind=(GameEnd.lGameScore[i]>0L)?enScoreKind_Win:enScoreKind_Lost;
				m_pITableFrame->WriteUserScore(i,ScoreInfo[i].lScore,ScoreInfo[i].lRevenue,ScoreInfo[i].ScoreKind);
			}
			//m_pITableFrame->WriteTableScore(ScoreInfo, m_wPlayerCount, GameEnd.lGameTax);

			//������Ϸ
			m_pITableFrame->ConcludeGame();
			return true;
		}
	//	//�û��뿪
	//case GER_USER_LEFT:
	//	{
	//		//��������
	//		m_wBankerUser = 0;
	//		m_wQFeng= 0;
	//		return true;
	//	}
		//�û�ǿ��
	case GER_USER_LEFT:
		{
			//У�����
			ASSERT( pIServerUserItem != NULL );
			ASSERT( wChairID< m_wPlayerCount );

			m_wBankerUser = 0;
			m_wQFeng= 0;
			//��������
			CMD_S_GameEnd GameEnd;
			memset( &GameEnd, 0, sizeof( GameEnd ) );
			//��������
			LONG lCeelScore=m_pGameServiceOption->lCellScore/512;

			//����ͳ��
			//GameEnd.strEnd=NULL;
			//CopyMemory( &GameEnd.strEnd, m_strFang, strlen(m_strFang));
			//����
			for( int k=0; k<4; k++ )
			{
				CopyMemory( &GameEnd.byMjs[k], m_byHandMjs[k], sizeof(BYTE)*14);
			}
			for( WORD i=0; i<m_wPlayerCount;i++)
			{
				//ǿ��,��32���׷�
				if(i==wChairID)
				{
					GameEnd.lGameScore[i]=-32*lCeelScore;
				}
				else
				{
					GameEnd.lGameScore[i]=0;
				}
			}
			//������Ϣ
			m_pITableFrame->SendTableData( INVALID_CHAIR, SUB_S_GAME_END, &GameEnd, sizeof(CMD_S_GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_GAME_END, &GameEnd, sizeof(CMD_S_GameEnd)
				);
			//�޸Ļ���
			tagScoreInfo ScoreInfo;
			ScoreInfo.ScoreKind=enScoreKind_Draw;
			ScoreInfo.lScore=GameEnd.lGameScore[wChairID];
			m_pITableFrame->WriteUserScore(wChairID,ScoreInfo.lScore,ScoreInfo.lRevenue,ScoreInfo.ScoreKind);
			m_pITableFrame->ConcludeGame();
			return true;
		}
	}
	ASSERT( FALSE );
	return false;
}

//���ͳ���
bool __cdecl CTableFrameSink::SendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret)
{
	switch( bGameStatus )
	{
	case GS_WK_FREE:
		{
			CMD_S_StatusFree StatusFree;
			memset( &StatusFree, 0, sizeof( StatusFree ) );

			//���ñ���
			StatusFree.lBaseGold = m_pGameServiceOption->lCellScore;
			return m_pITableFrame->SendGameScene( pIServerUserItem, &StatusFree,sizeof( StatusFree ) );
		}
	case GS_WK_PLAYING:
		{
			CMD_S_StatusPlay StatusPlay;
			memset( &StatusPlay, 0, sizeof( StatusPlay ) );

			//���ñ���
			StatusPlay.wLastOutUser = m_wTurnWiner;
			StatusPlay.lBaseScore = m_pGameServiceOption->lCellScore;
			StatusPlay.wCurrentUser = m_wCurrentUser;
			StatusPlay.byOutMj = m_byOutMj;
			CopyMemory( StatusPlay.byMjs, m_byHandMjs[wChairID], sizeof(BYTE)*m_byHandMjCount[wChairID] );

			//���ͳ���
			return m_pITableFrame->SendGameScene( pIServerUserItem, &StatusPlay, sizeof( StatusPlay ) );
		}
	}
	//Ч����
	ASSERT(FALSE);

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
	switch( wSubCmdID )
	{
	case SUB_C_TOUCH_MJ:

		break;
	case SUB_C_OUT_MJ:
		{
			//��������
			BYTE *pOutMj = ( BYTE* )pDataBuffer;
		
			//�û�Ч��
			tagServerUserData *pUserData = pIServerUserItem->GetUserData();
			if( pUserData->cbUserStatus != US_PLAY )
			{
				return true;
			}
			
			//��Ϣ����
			return OnUserOutMj( pUserData->wChairID, pOutMj );
		}
	case SUB_C_GANG_MJ:			//�ܲ���
		{
			//�û�Ч��
			tagServerUserData *pUserData= pIServerUserItem->GetUserData();
			if( pUserData->cbUserStatus!= US_PLAY)
			{
				return true;
			}
			//��Ϣ����
			return OnUserGang( pUserData->wChairID, pDataBuffer );
		}
	case SUB_C_SELECT_GANG:
		{
			//�û�Ч��
			tagServerUserData *pUserData= pIServerUserItem->GetUserData();
			if( pUserData->cbUserStatus!= US_PLAY )
			{
				return true;
			}
			//��Ϣ����
			return OnUserGangSelect( pUserData->wChairID, pDataBuffer);
		}
	case SUB_C_PENG_MJ:		//������
		{
			//�û�Ч��
			tagServerUserData *pUserData = pIServerUserItem->GetUserData();
			if( pUserData->cbUserStatus != US_PLAY )
			{
				MessageBox( 0, "UN_US_PLAY", 0, 0 );
				return true;
			}
			//��Ϣ����
			return OnUserPeng(pUserData->wChairID, pDataBuffer);
		}
	case SUB_C_CHI_MJ:		//�Բ���
		{
			//�û�Ч��
			tagServerUserData *pUserData = pIServerUserItem->GetUserData();
			if( pUserData->cbUserStatus!= US_PLAY )
			{
				MessageBox( 0, "UN_US_PLAY", 0, 0 );
				return true;
			}
			//��Ϣ����
			return OnUserChi(pUserData->wChairID, pDataBuffer);
		}
	case SUB_C_HU_MJ:
		{
			//�û�Ч��
			tagServerUserData *pUserData=pIServerUserItem->GetUserData();
			{
				if( pUserData->cbUserStatus != US_PLAY )
				{
					MessageBox( 0, "UN_US_PLAY", 0, 0 );
					return true;
				}
			}
			//��Ϣ����
			return OnUserHu(pUserData->wChairID, pDataBuffer);
		}
	case SUB_C_GIVEUP:			//����
		{
			//�û�Ч��
			tagServerUserData *pUserData = pIServerUserItem->GetUserData();
			if( pUserData->cbUserStatus != US_PLAY )
			{
				return true;
			}
			return OnUserGiveUp( pUserData->wChairID, pDataBuffer);
		}
	}
	return true;
}

//�����Ϣ����
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�û�����
bool CTableFrameSink::OnUserOutMj( WORD wChairID, BYTE *byOutMj )
{
	//Ч��״̬
	if( m_pITableFrame->GetGameStatus() != GS_WK_PLAYING )	return false;
	if( wChairID != m_wCurrentUser )
	{
		return false;
	}

	//���
	if(m_bTianHu)
	{
		m_bTianHu=false;
	}
	if(m_bRenHu[wChairID])
	{
		//ׯ��������ƣ��˺ͷ�
		for(WORD i=0; i<m_wPlayerCount;i++)
		{
			m_bRenHu[i]=false;
		}
	}
	if(m_bDiHu[wChairID])
	{
		m_bDiHu[wChairID]=false;
	}
	//����
	m_byOutMj = *byOutMj;
	//ȡ������Ȩ
	if( m_ActionRight[wChairID]>0)
	{
		m_ActionRight[wChairID] = 0;
	}

	//��ֵУ��
	for( BYTE i=0; i<HAND_MJ_MAX; i++ )
	{
		if( m_byHandMjs[wChairID][i] == m_byOutMj )
		{
			//��Чɾ���齫
			m_byHandMjs[wChairID][i] = 0;
			
			//����,ʹ������ΪO
			if( m_byHandMjs[wChairID][HAND_MJ_MAX-1] ==0 )
			{
				m_byHandMjCount[wChairID] -= 1;
			}
			else
			{
				m_GameLogic.LMove( m_byHandMjs[wChairID], m_byHandMjCount[wChairID] );
				m_byHandMjCount[wChairID] -= 1;
				m_GameLogic.InsertMjList( m_byHandMjs[wChairID], m_byHandMjCount[wChairID] );	
			}

			//m_GameLogic.InsertMjList( m_byHandMjs[wChairID], m_byHandMjCount[wChairID] );
			break;
		}
	}

	//�������ݽṹ
	m_wCurrentUser = wChairID;
	CMD_S_OutMj OutMj;
	OutMj.wOutMjUser = wChairID;
	OutMj.byOutMj = m_byOutMj;

	//��������
	WORD wSendSize = sizeof( CMD_S_OutMj );
	for( WORD i=0; i<m_wPlayerCount; i++ )
	{	
		//�������Լ�����Ϣ
		if( i != wChairID )
		{
			//�¼�,���Գ���
			if( i == (wChairID+3)%m_wPlayerCount )
			{
				OutMj.nHu	= m_GameLogic.IsHu( m_byHandMjs[i], m_byHandMjCount[i], m_byOutMj, m_hu[i], &m_tempHu[i] );
				OutMj.nGang = m_GameLogic.IsGang( m_byHandMjs[i], m_byHandMjCount[i], m_byOutMj ).GangStyle;
				OutMj.bPeng = m_GameLogic.IsPeng( m_byHandMjs[i], m_byHandMjCount[i], m_byOutMj );
				OutMj.nChi  = m_GameLogic.IsChi( m_byHandMjs[i], m_byHandMjCount[i], m_byOutMj );

				//����Ȩ��
				if( OutMj.nHu )
					m_ActionRight[i] = 6;
				else if( OutMj.nGang )
					m_ActionRight[i] = 4;
				else if( OutMj.bPeng )
					m_ActionRight[i] = 3;
				else if( OutMj.nChi )
					m_ActionRight[i] = 2;
			}
			else
			{
				OutMj.nHu	= m_GameLogic.IsHu( m_byHandMjs[i], m_byHandMjCount[i], m_byOutMj, m_hu[i], &m_tempHu[i] );
				OutMj.nGang = m_GameLogic.IsGang( m_byHandMjs[i], m_byHandMjCount[i], m_byOutMj ).GangStyle;
				OutMj.bPeng = m_GameLogic.IsPeng( m_byHandMjs[i], m_byHandMjCount[i], m_byOutMj );
				OutMj.nChi  = false;

				//����Ȩ��
				if( OutMj.nHu )
					m_ActionRight[i] = 6;
				else if( OutMj.nGang )
					m_ActionRight[i] = 4;
				else if( OutMj.bPeng )
					m_ActionRight[i] = 3;
				else if( OutMj.nChi )
					m_ActionRight[i] = 2;
			}
			m_pITableFrame->SendTableData( i, SUB_S_OUT_MJ, &OutMj, wSendSize );
			m_pITableFrame->SendLookonData( i, SUB_S_OUT_MJ, &OutMj, wSendSize );
		}
	}

	//�����в���,�¼�����
	for( int j=0; j<m_wPlayerCount; j++ )
	{
		if( m_ActionRight[j] != 0 )
			break;
		else
		{
			//�����г����ܺ�
			if( j == m_wPlayerCount-1 )
			{	
				//�л��û�
				if( m_byHandMjCount[wChairID] != 0 )
				{
					m_wCurrentUser = ( wChairID+3 )%m_wPlayerCount;
				}
				else
				{
					m_wCurrentUser = INVALID_CHAIR;
				}
				OnUserTouchMj( m_wCurrentUser );
			}
		}
	}

	//�Ƿ������Ϸ
	//if( m_wCurrentUser == INVALID_CHAIR )
	//	OnEventGameEnd( wChairID, NULL, GER_NORMAL );

	return true;
}

//�û�����
bool CTableFrameSink::OnUserTouchMj( WORD wChairID )
{
	CMD_S_TouchMj TouchMj;
	TouchMj.wCurrentUser = wChairID;
	
	//���óɵ�ǰ���
	m_wCurrentUser = wChairID;

	for( int i=0; i<m_wPlayerCount; i++ )
	{
		if( i == wChairID )
		{
			if( m_nTouchMjPos == ORG_MJ_MAX )
			{
				return OnGameEndEmpty();
			}
			TouchMj.byTouchMj = m_byMjs[m_nTouchMjPos];
			//��������
			m_byHandMjs[i][HAND_MJ_MAX-1] = m_byMjs[m_nTouchMjPos];
			m_byHandMjCount[wChairID] += 1;
		
			//���������ж�
			//����
			TouchMj.nHu = m_GameLogic.IsHu(m_byHandMjs[i], m_byHandMjCount[i]-1, m_byHandMjs[i][HAND_MJ_MAX-1], m_hu[i], &m_tempHu[i] );
			TouchMj.nGang = m_GameLogic.IsAnGang(m_byHandMjs[i], m_byHandMjCount[i]).GangStyle+m_GameLogic.IsZMGang(m_byHandMjs[i], m_byHandMjCount[i], m_hu[i]).GangStyle;
			if( TouchMj.nGang>0)
				m_ActionRight[i] = 4;
			if( TouchMj.nHu)
				m_ActionRight[i] = 6;
		}
		else
		{
			TouchMj.nGang = false;
			TouchMj.nHu   = false;
			TouchMj.byTouchMj = 0xff;		//��Ч��
		}
		m_pITableFrame->SendTableData( i, SUB_S_TOUCH_MJ, &TouchMj, sizeof( CMD_S_TouchMj ) );
		m_pITableFrame->SendLookonData( i, SUB_S_TOUCH_MJ, &TouchMj, sizeof( CMD_S_TouchMj ) );
	}
	//������-1;
	m_nTouchMjPos += 1;
	m_byTouchMjCount -= 1;
	
	return true;
}

bool CTableFrameSink::OnUserGiveUp( WORD wChairID, const void *pDataBuffer)
{
	//Ч��״̬
	if( m_pITableFrame->GetGameStatus() != GS_WK_PLAYING )	
		return false;
	//��������
	CMD_C_Operation *Operation = ( CMD_C_Operation* )pDataBuffer;
	m_ActionRight[wChairID] = Operation->nRight;
	//�ǵ�ǰ���
	if( m_byHandMjs[wChairID][HAND_MJ_MAX-1] == 0)
	{	
		int tempRight=0;
		WORD UserID=-1;
		for( int i=0; i<m_wPlayerCount; i++ )
		{
			if( m_ActionRight[i]>tempRight )
			{
				tempRight=m_ActionRight[i];
				UserID=i;
			}
		}
		if(tempRight>0)
		{
			//���ͷ�����Ϣ
			m_pITableFrame->SendTableData(UserID,SUB_S_GIVEUP);
		}
		else
		{
			//ȫ���˶���������,�¼�����
			WORD UserID = (m_wCurrentUser+3)%m_wPlayerCount;
			OnUserTouchMj( UserID );
		}
	}

	return true;
}

//�û�����
bool CTableFrameSink::OnUserPeng( WORD wChairID, const void *pDataBuffer )
{
	//Ч��״̬
	if( m_pITableFrame->GetGameStatus() != GS_WK_PLAYING )	
	{
		return false;
	}


	CMD_C_Operation *Peng = ( CMD_C_Operation * )pDataBuffer;
	//Ȩ��Ч��
	if( m_ActionRight[wChairID] < Peng->nRight )
	{
		return false;
	}

	//��������Ȩ�����ϵĲ���
	int userCount = 0;		//��������
	for( int i=0; i<m_wPlayerCount; i++ )
	{
		//��������,�ͳ�����
		if( i==wChairID || i==m_wCurrentUser )
			continue;

		if( m_ActionRight[i]>3 )
		{
			break;
		}

		//������
		else 
		{
			userCount ++;
			m_ActionRight[i] = 0;
			if( userCount == 2 )
			{
				m_ActionRight[wChairID] = 0;
				//������
				MJ_BLOCK block;
				block = m_GameLogic.Peng( m_byHandMjs[wChairID], m_byHandMjCount[wChairID], m_byOutMj );
				m_GameLogic.AddBlock( &m_hu[wChairID], &block );
				m_GameLogic.RMove( m_byHandMjs[wChairID], m_byHandMjCount[wChairID], 3 );
				m_byHandMjCount[wChairID] -= 2;				//�����2

				//������Ϣ
				CMD_S_Operation  Peng;
				Peng.mjFirst = block.first;
				Peng.blockStyle = block.style;
				Peng.wActionUser = wChairID;
				Peng.wOutMjUser= m_wCurrentUser;
				
				//��ǰ�û��任
				m_wCurrentUser = wChairID;

				m_pITableFrame->SendTableData( INVALID_CHAIR, SUB_S_PENG_MJ, &Peng, sizeof(CMD_S_Operation) );
				m_pITableFrame->SendLookonData( INVALID_CHAIR, SUB_S_PENG_MJ, &Peng, sizeof(CMD_S_Operation) );
				break;
			}
		}
	}
	return true;
}

//�û��Բ���
bool CTableFrameSink::OnUserChi(WORD wChairID, const void *pDataBuffer)
{
	//Ч��״̬
	if( m_pITableFrame->GetGameStatus() != GS_WK_PLAYING )	
	{
		MessageBox( NULL, "GS_WK_PLAYING", 0, 0 );
		return false;
	}

	//���ñ���
 	CMD_C_Chi *Chi = ( CMD_C_Chi * )pDataBuffer;
	if( m_ActionRight[wChairID] < Chi->nRight )
	{
		return false;
	}

	//��������Ȩ�����ϵĲ���
	int userCount = 0;		//��������
	for( int i=0; i<m_wPlayerCount; i++ )
	{
		//��������,�ͳ�����
		if( i==wChairID || i==m_wCurrentUser )
			continue;

		if( m_ActionRight[i]>2 )
		{
			break;
		}
		//�Դ���
		else 
		{
			userCount ++;
			m_ActionRight[i] = 0;
			if( userCount == 2 )
			{
				//�Ƿ��г�ѡ��
				if( Chi->nChiType>2 && Chi->nChiType!= 4)
				{
					OnUserChiSelect( wChairID, Chi->nChiType);
				}
				else
				{
					m_ActionRight[wChairID] = 0;
					//�����

					MJ_BLOCK block;
					block = m_GameLogic.Chi( m_byHandMjs[wChairID], m_byHandMjCount[wChairID], m_byOutMj,Chi->nChiType );
					m_GameLogic.AddBlock( &m_hu[wChairID], &block );
					m_GameLogic.RMove( m_byHandMjs[wChairID], m_byHandMjCount[wChairID], 3 );
					m_byHandMjCount[wChairID] -= 2;				//�����2
					//������Ϣ
					CMD_S_Chi  ChiMj;
					ChiMj.mjFirst = block.first;
					ChiMj.blockStyle = block.style;
					ChiMj.wActionUser = wChairID;
					ChiMj.nChiType= Chi->nChiType;
					//�ϴβ������-������
					ChiMj.wOutMjUser= m_wCurrentUser;

					//��ǰ�û��任
					m_wCurrentUser = wChairID;

					m_pITableFrame->SendTableData( INVALID_CHAIR, SUB_S_CHI_MJ, &ChiMj, sizeof(CMD_S_Chi) );
					m_pITableFrame->SendLookonData( INVALID_CHAIR, SUB_S_CHI_MJ, &ChiMj, sizeof(CMD_S_Chi) );
				}

			}
		}
	}
	return true;
}

//�û�����ѡ��
bool CTableFrameSink::OnUserChiSelect( WORD wChairID, int nChiType)
{
	CMD_S_ChiMode ChiMode;
	ChiMode.nChiType= nChiType;

	m_pITableFrame->SendTableData( wChairID, SUB_S_SELECT_CHI, &ChiMode, sizeof(CMD_S_ChiMode));
	return true;
}
//�û�����
bool CTableFrameSink::OnUserHu( WORD wChairID, const void *pDataBuffer)
{
	//Ч��״̬
	if( m_pITableFrame->GetGameStatus() != GS_WK_PLAYING )	
	{
		MessageBox( NULL, "GS_WK_PLAYING", 0, 0 );
		return false;
	}

	CMD_C_Hu *hu = ( CMD_C_Hu * )pDataBuffer;
	//Ȩ��Ч��
	if( m_ActionRight[wChairID] < hu->nRight )
	{
		return false;
	}
	m_strFang= new char[300];
	int userCount = 0;		//��������
	for( int i=0; i<m_wPlayerCount; i++ )
	{
		//��������,�ͳ�����
		if( i==wChairID || i==m_wCurrentUser )
			continue;

		//if( m_ActionRight[i]>6 )
		//{
		//	MessageBox( 0,"��һ�ڶ���", 0, 0 );
		//	//break;
		//	//�ȴ�,�����ж��˺�
		//}
		//������
		else 
		{
			userCount ++;
			m_ActionRight[i] = 0;
			if( userCount == 2 )
			{
				//��ҽ���
				m_wTurnWiner= wChairID;				//Ӯ��
				//����
				if( m_byHandMjs[wChairID][HAND_MJ_MAX-1] != 0 )
				{
					//���
					if(m_bTianHu)
					{
						strcpy(m_strFang, "��� +9��");
						m_nFang[wChairID] = 9;	
						return  OnEventGameEnd( wChairID, NULL, GER_NORMAL );
					}
					//�غ�
					if(m_bDiHu[wChairID])
					{
						strcpy(m_strFang, "�غ� +9��");
						m_nFang[wChairID] = 9;
						return  OnEventGameEnd( wChairID, NULL, GER_NORMAL );
					}
					m_bZiM= true;
					int huType = m_GameLogic.IsHu( m_byHandMjs[wChairID], m_byHandMjCount[wChairID]-1, m_byHandMjs[wChairID][HAND_MJ_MAX-1], m_hu[wChairID], &m_tempHu[wChairID]);
					if(  huType== 1)
					{
						m_nFang[wChairID] = m_GameLogic.CheckGDHu( m_tempHu[wChairID], m_GameLogic.TransIDToFeng( wChairID, m_wBankerUser), m_wQFeng, m_strFang );
						strcat( m_strFang, "���� +1��" );
						//���� +1��
						if( m_nFang[wChairID]<5)
						{
							m_nFang[wChairID]+= 1;
						}
					}
					else
					{
						//������
						if( huType-20 == 9 )
						{
							strcpy(m_strFang, "�������� +9��,");
							m_nFang[wChairID] = 9;							
						}
						//ʮ����
						else if( huType-20== 13)
						{
							strcpy( m_strFang, "ʮ���� +9��,");
							m_nFang[wChairID] = 9;
						}
					}
				}
				//���˷���
				else
				{
					if(m_bRenHu[wChairID])
					{
						strcpy(m_strFang, "�˺� +9��");
						m_nFang[wChairID] = 9;	
						return  OnEventGameEnd( wChairID, NULL, GER_NORMAL );
					}
					m_wLastUser= m_wCurrentUser;		//���
					int huType = m_GameLogic.IsHu( m_byHandMjs[wChairID], m_byHandMjCount[wChairID], m_byOutMj, m_hu[wChairID], &m_tempHu[wChairID]);
					if(  huType== 1)
					{
						m_nFang[wChairID] = m_GameLogic.CheckGDHu( m_tempHu[wChairID], m_GameLogic.TransIDToFeng( wChairID, m_wBankerUser), m_wQFeng, m_strFang );
					}
					else
					{
						//������
						if( huType-20 == 9 )
						{
							strcpy(m_strFang, "�������� +9��,");
							m_nFang[wChairID] = 9;							
						}
						//ʮ����
						else if( huType-20== 13)
						{
							strcpy( m_strFang, "ʮ���� +9��,");
							m_nFang[wChairID] = 9;
						}
					}
				}
				////����
				//if( m_byHandMjs[wChairID][HAND_MJ_MAX-1] != 0 )
				//{
				//	
				//	//for( int i=0; i<m_wPlayerCount; i++)
				//	//{
				//	//	if( i==wChairID)
				//	//		continue;
				//	//	else
				//	//		m_nFang[i] -= m_nFang[wChairID];
				//	//}
				//}
				////���˷���
				//else
				//{
				//
				//	//m_nFang[m_wCurrentUser] -= m_nFang[wChairID];
				//}
				//���ƴ���
				 return  OnEventGameEnd( wChairID, NULL, GER_NORMAL );
			}
		}
	}
	return true;
}

//���Ʋ���
bool CTableFrameSink::OnUserGang( WORD wChairID, const void *pDataBuffer )
{
	//Ч��״̬
	if( m_pITableFrame->GetGameStatus() != GS_WK_PLAYING )	
	{
		MessageBox( NULL, "GS_WK_PLAYING", 0, 0 );
		return false;
	}

	CMD_C_Gang *Gang = ( CMD_C_Gang * )pDataBuffer;
	//Ȩ��Ч��
	if( m_ActionRight[wChairID] < Gang->nRight )
	{
		return false;
	}

	//�޸ܲ���Ȩ�����ϵĲ���
	int userCount = 0;		//��������
	for( int i=0; i<m_wPlayerCount; i++ )
	{
		//��������,�ͳ�����
		if( i==wChairID || i==m_wCurrentUser )
			continue;

		if( m_ActionRight[i]>Gang->nRight )
		{
			break;
		}

		//�ܴ���
		else 
		{
			userCount ++;
			m_ActionRight[i] = 0;
			if( userCount == 2 )
			{
				switch( Gang->nGangType)
				{
				case 1:					//��ͨ��
					{
						m_ActionRight[wChairID] = 0;
						MJ_BLOCK block;
						block = m_GameLogic.MGang( m_byHandMjs[wChairID], m_byHandMjCount[wChairID], m_byOutMj );
						m_GameLogic.AddBlock( &m_hu[wChairID], &block);

						//����
						m_GameLogic.RMove( m_byHandMjs[wChairID], m_byHandMjCount[wChairID], 4 );
						m_byHandMjCount[wChairID] -= 2;				//�����2
						m_GameLogic.LMove( m_byHandMjs[wChairID], m_byHandMjCount[wChairID] );
						m_byHandMjCount[wChairID] -= 1;


						//���͸�����Ϣ
						CMD_S_Gang GangS;
						GangS.blockStyle= block.style;
						GangS.mjFirst= block.first;
						GangS.wActionUser= wChairID;
						GangS.gangType= 1;
						GangS.wOutMjUser= m_wCurrentUser;

						//��ǰ�û��任
						m_wCurrentUser = wChairID;

						m_pITableFrame->SendTableData( INVALID_CHAIR, SUB_S_GANG_MJ, &GangS, sizeof(CMD_S_Gang) );
						m_pITableFrame->SendLookonData( INVALID_CHAIR, SUB_S_GANG_MJ, &GangS, sizeof(CMD_S_Gang) );
						//����������
						OnUserTouchMj(wChairID);
					}
					break;
				case 2:					//����
					{
						//����ж������,�������ѡ��ģʽ
						BYTE GangMj[3] = { 0xff, 0xff, 0xff };
						MJ_GANGINFO gangInfo;
						gangInfo = m_GameLogic.IsZMGang( m_byHandMjs[wChairID], m_byHandMjCount[wChairID], m_hu[wChairID] );
						GangMj[0] = gangInfo.GangMj;

						gangInfo =m_GameLogic.IsZMGang( m_byHandMjs[wChairID], m_byHandMjCount[wChairID], m_hu[wChairID], GangMj[0]);
						GangMj[1] = gangInfo.GangMj;

						//�ж������,�������ѡ��ģʽ
						if( GangMj[1] != 0xff)
						{
							gangInfo = m_GameLogic.IsZMGang( m_byHandMjs[wChairID], m_byHandMjCount[wChairID], m_hu[wChairID],GangMj[0], GangMj[1] );
							GangMj[2] = gangInfo.GangMj;
							
							//�����ṹ
							CMD_S_GangMode GangMode;
							CopyMemory( &GangMode.byMjGang, &GangMj, 3*sizeof(BYTE));
							GangMode.nGangType= 2;

							m_pITableFrame->SendTableData( wChairID, SUB_S_SELECT_GANG, &GangMode, sizeof( CMD_S_GangMode ));
						}
						//û��ѡ��,�ܴ���
						else
						{
							m_ActionRight[wChairID] = 0;
							//MJ_BLOCK block;	
							//m_GameLogic.AddBlock( &m_hu[wChairID], &block);
						
							//block= m_GameLogic.ZMGang( m_byHandMjs[wChairID], m_byHandMjCount[wChairID],&m_hu[wChairID],  m_byOutMj );
							BYTE GangMj;
							GangMj = m_GameLogic.IsZMGang( m_byHandMjs[wChairID], m_byHandMjCount[wChairID], m_hu[wChairID]).GangMj;
							m_GameLogic.ZMGang( m_byHandMjs[wChairID], m_byHandMjCount[wChairID],&m_hu[wChairID], GangMj );
							//����
							m_GameLogic.LMove( m_byHandMjs[wChairID], m_byHandMjCount[wChairID] );
							//m_GameLogic.RMove( m_byHandMjs[wChairID], m_byHandMjCount[wChairID], 1 );
							m_byHandMjCount[wChairID] -= 1;				//�����1

							//���͸�����Ϣ
							CMD_S_Gang GangS;
							GangS.blockStyle= BLOCK_GANG;
							GangS.mjFirst= GangMj;
							GangS.wActionUser= wChairID;
							GangS.gangType= 2;
							GangS.wOutMjUser= m_wCurrentUser;

							//��ǰ�û��任
							m_wCurrentUser = wChairID;

							m_pITableFrame->SendTableData( INVALID_CHAIR, SUB_S_GANG_MJ, &GangS, sizeof(CMD_S_Gang) );
							m_pITableFrame->SendLookonData( INVALID_CHAIR, SUB_S_GANG_MJ, &GangS, sizeof(CMD_S_Gang) );
							//����������
							OnUserTouchMj(wChairID);
						}
					}
					break;
				case 4:					//����
					{
						//����ж������,���ȸ���ѡ��ģʽ
						BYTE GangMj[3] = { 0xff, 0xff, 0xff };
						MJ_GANGINFO gangInfo;
						gangInfo = m_GameLogic.IsAnGang( m_byHandMjs[wChairID], m_byHandMjCount[wChairID] );
						GangMj[0] = gangInfo.GangMj;

						gangInfo =m_GameLogic.IsAnGang( m_byHandMjs[wChairID], m_byHandMjCount[wChairID], GangMj[0]);
						GangMj[1] = gangInfo.GangMj;

						//�ж������,���ȸ���ѡ��ģʽ
						if( GangMj[1] != 0xff)
						{
							gangInfo = m_GameLogic.IsAnGang( m_byHandMjs[wChairID], m_byHandMjCount[wChairID], GangMj[0], GangMj[1] );
							GangMj[2] = gangInfo.GangMj;

							//�����ṹ
							CMD_S_GangMode GangMode;
							CopyMemory( &GangMode.byMjGang, &GangMj, 3*sizeof(BYTE));
							GangMode.nGangType= 4;

							m_pITableFrame->SendTableData( wChairID, SUB_S_SELECT_GANG, &GangMode, sizeof( CMD_S_GangMode ));
						}
						else
						{
							m_ActionRight[wChairID] = 0;
							BYTE GangMj = m_GameLogic.IsAnGang( m_byHandMjs[wChairID], m_byHandMjCount[wChairID] ).GangMj;
							MJ_BLOCK block;

							block  = m_GameLogic.AnGang( m_byHandMjs[wChairID], m_byHandMjCount[wChairID], GangMj );
							m_GameLogic.AddBlock( &m_hu[wChairID], &block);

							//���ƴ���
							m_GameLogic.SortMj(m_byHandMjs[wChairID],14,14);
							m_byHandMjCount[wChairID]-=3;
							m_GameLogic.LMove(m_byHandMjs[wChairID],m_byHandMjCount[wChairID]);
							m_byHandMjCount[wChairID] -= 1;
							//m_GameLogic.RMove( m_byHandMjs[wChairID], m_byHandMjCount[wChairID], 4 );
							//m_byHandMjCount[wChairID] -= 3;				//�����1
							//m_GameLogic.LMove( m_byHandMjs[wChairID], m_byHandMjCount[wChairID] );
							//m_byHandMjCount[wChairID] -= 1;
							//��Ϣ����
							CMD_S_Gang GangS;
							GangS.blockStyle= block.style;
							GangS.mjFirst= block.first;
							GangS.wActionUser= wChairID;
							GangS.gangType= 4;

							//������Ϣ
							m_pITableFrame->SendTableData( INVALID_CHAIR, SUB_S_GANG_MJ, &GangS, sizeof( CMD_S_Gang));
							m_pITableFrame->SendLookonData( INVALID_CHAIR, SUB_S_GANG_MJ, &GangS, sizeof( CMD_S_Gang));

							//����������
							OnUserTouchMj(wChairID);
						}
					}
					break;
				case 6:					//�в���Ҳ�а���,,,���и���ѡ��
					{
						BYTE GangMj[3] = { 0xff, 0xff, 0xff };
						MJ_GANGINFO gangInfo;
						gangInfo = m_GameLogic.IsAnGang( m_byHandMjs[wChairID], m_byHandMjCount[wChairID] );
						GangMj[0] = gangInfo.GangMj;

						gangInfo =m_GameLogic.IsZMGang( m_byHandMjs[wChairID], m_byHandMjCount[wChairID], m_hu[wChairID], GangMj[0]);
						GangMj[1] = gangInfo.GangMj;

						//�ж������,���ȸ���ѡ��ģʽ
						if( GangMj[1] != 0xff)
						{
							gangInfo = m_GameLogic.IsAnGang( m_byHandMjs[wChairID], m_byHandMjCount[wChairID] );
							if( gangInfo.GangMj== 0xff)
							{
								gangInfo = m_GameLogic.IsZMGang( m_byHandMjs[wChairID], m_byHandMjCount[wChairID], m_hu[wChairID],GangMj[0], GangMj[1] );
								GangMj[2] = gangInfo.GangMj;
							}
							else
							{
								GangMj[2] = gangInfo.GangMj;
							}
							//�����ṹ
							CMD_S_GangMode GangMode;
							CopyMemory( &GangMode.byMjGang, &GangMj, 3*sizeof(BYTE));
							GangMode.nGangType= 2;

							m_pITableFrame->SendTableData( wChairID, SUB_S_SELECT_GANG, &GangMode, sizeof( CMD_S_GangMode ));
					}
					break;
					}
				}
			}
		}
	}
	return true;
}

//�û�ѡ���
bool CTableFrameSink::OnUserGangSelect( WORD wChairID, const void *pDataBuffer )
{
	CMD_C_SelectGang *SelectGang = (CMD_C_SelectGang* )pDataBuffer;
	
	switch( SelectGang->nGangType)
	{
	case 2:		//����
		{
			m_ActionRight[wChairID] = 0;

			m_GameLogic.ZMGang( m_byHandMjs[wChairID], m_byHandMjCount[wChairID],&m_hu[wChairID], SelectGang->byGangMj );

			m_GameLogic.LMove( m_byHandMjs[wChairID], m_byHandMjCount[wChairID] );
			m_byHandMjCount[wChairID] -= 1;				//�����1

			//���͸�����Ϣ
			CMD_S_Gang GangS;
			GangS.blockStyle= BLOCK_GANG;
			GangS.mjFirst= SelectGang->byGangMj;
			GangS.wActionUser= wChairID;
			GangS.gangType= 2;

			GangS.wOutMjUser= m_wCurrentUser;
			//��ǰ�û��任
			m_wCurrentUser = wChairID;

			m_pITableFrame->SendTableData( INVALID_CHAIR, SUB_S_GANG_MJ, &GangS, sizeof(CMD_S_Gang) );
			m_pITableFrame->SendLookonData( INVALID_CHAIR, SUB_S_GANG_MJ, &GangS, sizeof(CMD_S_Gang) );
			//����������
			OnUserTouchMj(wChairID);
		}
		break;
	case 4:		//����
		{
			m_ActionRight[wChairID] = 0;
			MJ_BLOCK block;

			block  = m_GameLogic.AnGang( m_byHandMjs[wChairID], m_byHandMjCount[wChairID], SelectGang->byGangMj );
			m_GameLogic.AddBlock( &m_hu[wChairID], &block);

			//���ƴ���
			m_GameLogic.SortMj(m_byHandMjs[wChairID],14,14);
			m_byHandMjCount[wChairID]-=3;
			m_GameLogic.LMove(m_byHandMjs[wChairID],m_byHandMjCount[wChairID]);
			m_byHandMjCount[wChairID] -= 1;
			//m_GameLogic.RMove( m_byHandMjs[wChairID], m_byHandMjCount[wChairID], 4 );
			//m_byHandMjCount[wChairID] -= 3;				//�����1
			//m_GameLogic.LMove( m_byHandMjs[wChairID], m_byHandMjCount[wChairID] );
			//m_byHandMjCount[wChairID] -= 1;
			
			//��Ϣ����
			CMD_S_Gang GangS;
			GangS.blockStyle= block.style;
			GangS.mjFirst= block.first;
			GangS.wActionUser= wChairID;
			GangS.gangType= 4;
			//������Ϣ
			m_pITableFrame->SendTableData( INVALID_CHAIR, SUB_S_GANG_MJ, &GangS, sizeof( CMD_S_Gang));
			m_pITableFrame->SendLookonData( INVALID_CHAIR, SUB_S_GANG_MJ, &GangS, sizeof( CMD_S_Gang));

			//����������
			OnUserTouchMj(wChairID);
		}
		break;		
	}
	return true;
}
//����
bool CTableFrameSink::OnGameEndEmpty()
{
	CMD_S_GameEndEmpty EndEmpty;
	memset( &EndEmpty, 0, sizeof(CMD_S_GameEndEmpty));
	//����
	for( int k=0; k<4; k++ )
	{
		CopyMemory( &EndEmpty.byMjs[k], m_byHandMjs[k], sizeof(BYTE)*14);
	}
	m_pITableFrame->SendTableData( INVALID_CHAIR, SUB_S_END_EMPTY, &EndEmpty, sizeof( CMD_S_GameEndEmpty));
	m_pITableFrame->SendLookonData( INVALID_CHAIR, SUB_S_END_EMPTY, &EndEmpty, sizeof( CMD_S_GameEndEmpty));
	m_pITableFrame->ConcludeGame();
	return true;
}
//////////////////////////////////////////////////////////////////////////
