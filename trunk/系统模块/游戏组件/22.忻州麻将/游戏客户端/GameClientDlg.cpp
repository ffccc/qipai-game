/**********
1.��ʱ�����⣻
2.�������ݣ�
3.
4.

***********/
#include "Stdafx.h"
#include "GameClient.h"
#include "GameOption.h"
#include "GameClientDlg.h"

#include "Struct.h"

int isStartInt = 0;
//////////////////////////////////////////////////////////////////////////

//��Ϸ��ʱ��
#define IDI_START_GAME				200									//��ʼ��ʱ��
#define IDI_OPERATE_CARD			202									//������ʱ��

//��Ϸ��ʱ��
#define TIME_START_GAME				40									//��ʼ��ʱ��
#define TIME_HEAR_STATUS			10 //3									//���ƶ�ʱ��  ����
#define TIME_OPERATE_CARD			30									//������ʱ��
#define TIME_TING_STATUS			2 //3									//���ƶ�ʱ��  ����
///////////////////////////��ʱ���߳�//////////////////////////////////////////////////////
extern int sz1,sz2;
int showNext = 0;

UINT MyThreadProc( LPVOID pParam )
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sMyThreadProc.log",strTime);

	strTemp.Format("MyThreadProc");
	theApp.WriteLog(strFile, strTemp);

	CGameClientDlg *GameView = (CGameClientDlg*)pParam;

	strTemp.Format("GameView->m_GameClientView.m_tDice=%d", GameView->m_GameClientView.m_tDice);
	theApp.WriteLog(strFile, strTemp);

	while( GameView->m_GameClientView.m_tDice > 0 )
	{
		Sleep( 50 );
		if( GameView->m_GameClientView.m_tDice<12)
		{
			GameView->m_GameClientView.m_tDice ++;
			GameView->m_GameClientView.UpdateGameView( NULL );

			strTemp.Format("GameView->m_GameClientView.m_tDice=%d", GameView->m_GameClientView.m_tDice);
			theApp.WriteLog(strFile, strTemp);

		}
		else
		{
			Sleep( 3000 );
			if (0 == showNext)
			showNext = 1;
		//	GameView->m_GameClientView.m_Timer.ShowWindow( SW_SHOW );
		//	GameView->m_GameClientView.m_tDice= 0;
	//		GameView->m_GameClientView.SetTimer(ID_OUTFLAG_TIME, 150,(TIMERPROC) NULL);
			GameView->ShowUserMJ();
			GameView->m_GameClientView.UpdateGameView( NULL );
			strTemp.Format("GameView->m_GameClientView.m_tDice=%d", GameView->m_GameClientView.m_tDice);
			theApp.WriteLog(strFile, strTemp);

			return 0;
		}
	}
	/*Del by doctor 20071214
	while( GameView->m_GameClientView.m_bActionOut== true )
	{
		Sleep(50);
		//int m_playTime = timeGetTime()*1000;
		//if( (m_playTime-GameView->m_GameClientView.m_tOutStartTime)%2==1)
		{
			GameView->m_GameClientView.m_tOut += 1;
			GameView->m_GameClientView.m_bPlay=false;
			if( GameView->m_GameClientView.m_tOut==10)
			{
				GameView->m_GameClientView.m_tOut = 0;
				GameView->m_GameClientView.m_bActionOut= false;
				GameView->m_GameClientView.m_iUpMj = -1;
			}
			GameView->m_GameClientView.UpdateGameView(NULL);
		}
	}*/
	strTemp.Format("end ");
	theApp.WriteLog(strFile, strTemp);

	return 0;
}

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

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_TING_CARD,OnTingCard)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_CARD_OPERATE,OnCardOperate)
	ON_MESSAGE(IDM_SHOW_SZ,OnShowSZ)
	ON_MESSAGE(IDM_RESET_UI,OnResetUI )							
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	uiShowInt = 0;

	//״̬����
	m_bHearStatus=false;

	//��������
	m_wHeapHand=0;
	m_wHeapTail=0;
	ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

	//������Ϣ
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//����齫
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�齫����
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

	return;
}

//��������
CGameClientDlg::~CGameClientDlg()
{
}

//��ʼ����
bool CGameClientDlg::InitGameFrame()
{
	//���ñ���
	SetWindowText(TEXT("�����齫"));

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//������Դ
	g_CardResource.LoadResource();

	return true;
}

//���ÿ��
void CGameClientDlg::ResetGameFrame()
{
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//״̬����
	m_bHearStatus=false;

	//��������
	m_wHeapHand=0;
	m_wHeapTail=0;
	ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

	//������Ϣ
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//����齫
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�齫����
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

	return;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{
	//��������
	CGameOption GameOption;
	GameOption.m_bEnableSound=IsEnableSound();

	//��������
	if (GameOption.DoModal()==IDOK)
	{
		EnableSound(GameOption.m_bEnableSound);
	}

	return;
}

//ʱ����Ϣ
bool CGameClientDlg::OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sOnTimerMessage.log",strTime);
	switch (nTimerID)
	{
	case IDI_START_GAME:		//��ʼ��Ϸ
		{
			if (nElapse==0)
			{
				OnStart(0,0);
				return false;
			}
			if (nElapse<=5) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_OPERATE_CARD:		//������ʱ��
		{
			//��ʱ�ж�
			if ((nElapse==0)&&(IsLookonMode()==false))
			{
				//��ȡλ��
				WORD wMeChairID=GetMeChairID();

				//��������
				if (wChairID==wMeChairID)
				{
					if (m_wCurrentUser==wMeChairID)
					{
						BYTE cbCardData=m_GameClientView.m_HandCardControl.GetCurrentCard();
						OnOutCard(cbCardData,cbCardData);
					}
					else OnCardOperate(WIK_NULL,0);
				}

				return false;
			}

			//��������
			if ((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	}

	return false;
}

//�Թ�״̬
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{
}

//������Ϣ
bool CGameClientDlg::OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sOnGameMessage.log",strTime);

			strTemp.Format("OnGameMessage wSubCmdID=%d", wSubCmdID);
			theApp.WriteLog(strFile, strTemp);
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
				for (int i=0; i < GAME_PLAYER; i++)
				{
					m_GameClientView.m_bTingFlag[i] = false;
					m_GameClientView.m_IntFirstTingFlag[i]=0;
				}
				strTemp.Format("��Ϸ��ʼ");
				theApp.WriteLog(strFile, strTemp);

				showNext = 0;
				isStartInt = 0;

				CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;
				cbSiceFirst=HIBYTE(pGameStart->wSiceCount) -1;
				cbSiceSecond=LOBYTE(pGameStart->wSiceCount) -1;


				cbSice2First=HIBYTE(pGameStart->wSiceCount2) -1;
				cbSice2Second=LOBYTE(pGameStart->wSiceCount2) -1;

				memcpy( (char *)&myGameStart , (char *) pBuffer, wDataSize);
				PostMessage(IDM_SHOW_SZ,0,0);

				return true;
			//return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_OUT_CARD:		//�û�����
		{
			strTemp.Format("�û�����");
			theApp.WriteLog(strFile, strTemp);
			return OnSubOutCard(pBuffer,wDataSize);
		}
	case SUB_S_SEND_CARD:		//������Ϣ
		{
			strTemp.Format("������Ϣ");
			theApp.WriteLog(strFile, strTemp);
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_OPERATE_NOTIFY:	//������ʾ
		{
			strTemp.Format("������ʾ");
			theApp.WriteLog(strFile, strTemp);
			return OnSubOperateNotify(pBuffer,wDataSize);
		}
	case SUB_S_OPERATE_RESULT:	//�������
		{
			strTemp.Format("�������");
			theApp.WriteLog(strFile, strTemp);
			return OnSubOperateResult(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			strTemp.Format("��Ϸ����");
			theApp.WriteLog(strFile, strTemp);
			return OnSubGameEnd(pBuffer,wDataSize);
		}
//����
	case SUB_S_TING_CARD:
		{
			strTemp.Format("����");
			theApp.WriteLog(strFile, strTemp);
			return OnSubTingCard(pBuffer,wDataSize);			
		};
	}

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sOnGameSceneMessage.log",strTime);

	strTemp.Format("OnGameSceneMessage  cbGameStation=%d", cbGameStation);
	theApp.WriteLog(strFile, strTemp);
	switch (cbGameStation)
	{
	case GS_MJ_FREE:	//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//��������
			m_wBankerUser=pStatusFree->wBankerUser;
			m_GameClientView.SetCellScore(pStatusFree->lCellScore);
			m_GameClientView.m_HandCardControl.SetDisplayItem(true);

			//���ý���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_cbHeapCardInfo[i][0]=0;
				m_cbHeapCardInfo[i][1]=HEAP_FULL_COUNT-(((i==m_wBankerUser)||((i+2)%4==m_wBankerUser))?34:34);
				m_GameClientView.m_HeapCard[SwitchViewChairID(i)].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1],HEAP_FULL_COUNT);
			}

			//���ÿؼ�
			if (IsLookonMode()==false)
			{
				m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
				m_GameClientView.m_btStart.SetFocus();
			}

			//���½���
			m_GameClientView.UpdateGameView(NULL);

			return true;
		}
	case GS_MJ_PLAY:	//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//���ñ���
			m_wBankerUser=pStatusPlay->wBankerUser;
			m_wCurrentUser=pStatusPlay->wCurrentUser;
			m_cbLeftCardCount=pStatusPlay->cbLeftCardCount;

			//��ʷ����
			m_wOutCardUser=pStatusPlay->wOutCardUser;
			m_cbOutCardData=pStatusPlay->cbOutCardData;
			CopyMemory(m_cbDiscardCard,pStatusPlay->cbDiscardCard,sizeof(m_cbDiscardCard));
			CopyMemory(m_cbDiscardCount,pStatusPlay->cbDiscardCount,sizeof(m_cbDiscardCount));

			//�齫����
			CopyMemory(m_cbWeaveCount,pStatusPlay->cbWeaveCount,sizeof(m_cbWeaveCount));
			CopyMemory(m_WeaveItemArray,pStatusPlay->WeaveItemArray,sizeof(m_WeaveItemArray));
			m_GameLogic.SwitchToCardIndex(pStatusPlay->cbCardData,pStatusPlay->cbCardCount,m_cbCardIndex);

			//��������
			WORD wViewChairID[GAME_PLAYER]={0,0,0,0};
			for (WORD i=0;i<GAME_PLAYER;i++) wViewChairID[i]=SwitchViewChairID(i);

			//��������
			m_GameClientView.SetCellScore(pStatusPlay->lCellScore);
			m_GameClientView.SetBankerUser(wViewChairID[m_wBankerUser]);

			//����齫
			BYTE cbWeaveCard[4]={0,0,0,0};
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				for (BYTE j=0;j<m_cbWeaveCount[i];j++)
				{
					BYTE cbWeaveKind=m_WeaveItemArray[i][j].cbWeaveKind;
					BYTE cbCenterCard=m_WeaveItemArray[i][j].cbCenterCard;
					BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(cbWeaveKind,cbCenterCard,cbWeaveCard);
					m_GameClientView.m_WeaveCard[wViewChairID[i]][j].SetCardData(cbWeaveCard,cbWeaveCardCount);
					if (cbWeaveKind&(WIK_GANG|WIK_FILL))
					{
						if (m_WeaveItemArray[i][j].wProvideUser==i) m_GameClientView.m_WeaveCard[wViewChairID[i]][j].SetDisplayItem(false);
					}
				}
			}

			//�û��齫
			if (m_wCurrentUser==GetMeChairID())
			{
				BYTE cbCardCount=pStatusPlay->cbCardCount;
				m_GameClientView.m_HandCardControl.SetCardData(pStatusPlay->cbCardData,cbCardCount-1,pStatusPlay->cbCardData[cbCardCount-cbCardCount]);
			}
			else m_GameClientView.m_HandCardControl.SetCardData(pStatusPlay->cbCardData,pStatusPlay->cbCardCount,0); 

			//�齫����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//�û��齫
				if (i!=GetMeChairID())
				{
					BYTE cbCardCount=13-m_cbWeaveCount[i]*3;
					WORD wUserCardIndex=(wViewChairID[i]<2)?wViewChairID[i]:2;
					m_GameClientView.m_UserCard[wUserCardIndex].SetCardData(cbCardCount,(m_wCurrentUser==i));
				}

				//�����齫
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.m_DiscardCard[wViewChairID].SetCardData(m_cbDiscardCard[i],m_cbDiscardCount[i]);
			}

			//��������
			if (IsLookonMode()==false) 
			{
				m_GameClientView.m_HandCardControl.SetPositively(true);
				m_GameClientView.m_HandCardControl.SetDisplayItem(true);
			}

			//�����齫
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_cbHeapCardInfo[i][0]=0;
				m_cbHeapCardInfo[i][1]=HEAP_FULL_COUNT-(((i==m_wBankerUser)||((i+2)%4==m_wBankerUser))?34:34);
			}

			//�ַ��齫
			BYTE cbTakeCount=MAX_REPERTORY-m_cbLeftCardCount;
			WORD wTakeChairID=(m_wBankerUser+7-cbSiceFirst)%GAME_PLAYER;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//������Ŀ
				BYTE cbValidCount=HEAP_FULL_COUNT-m_cbHeapCardInfo[wTakeChairID][1]-((i==0)?(cbSiceSecond-1)*2:0);
				BYTE cbRemoveCount=__min(cbValidCount,cbTakeCount);

				//��ȡ�齫
				cbTakeCount-=cbRemoveCount;
				m_cbHeapCardInfo[wTakeChairID][(i==0)?1:0]+=cbRemoveCount;

				//����ж�
				if (cbTakeCount==0)
				{
					m_wHeapHand=wTakeChairID;
					m_wHeapTail=(m_wBankerUser+7-cbSiceFirst)%GAME_PLAYER;
					break;
				}

				//�л�����
				wTakeChairID=(wTakeChairID+1)%GAME_PLAYER;
			}

			//��������
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.m_HeapCard[wViewChairID[i]].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1],HEAP_FULL_COUNT);
			}

			//��ʷ�齫
			if (m_wOutCardUser!=INVALID_CHAIR)
			{
				WORD wOutChairID=SwitchViewChairID(m_wOutCardUser);
				m_GameClientView.SetOutCardInfo(wOutChairID,m_cbOutCardData);
			}

			//��������
			if (cbGameStation==GS_MJ_PLAY)
			{
				//��������
				if ((IsLookonMode()==false)&&(pStatusPlay->cbActionMask!=WIK_NULL))
				{
					//��ȡ����
					BYTE cbActionMask=pStatusPlay->cbActionMask;
					BYTE cbActionCard=pStatusPlay->cbActionCard;

					//��������
					tagGangCardResult GangCardResult;
					ZeroMemory(&GangCardResult,sizeof(GangCardResult));

					//�����ж�
					if ((cbActionMask&(WIK_GANG|WIK_FILL))!=0)
					{
						//�������
						if ((m_wCurrentUser==INVALID_CHAIR)&&(cbActionCard!=0))
						{
							GangCardResult.cbCardCount=1;
							GangCardResult.cbCardData[0]=cbActionCard;
						}

						//�Լ�����
						if ((m_wCurrentUser==GetMeChairID())||(cbActionCard==0))
						{
							WORD wMeChairID=GetMeChairID();
							m_GameLogic.AnalyseGangCard(m_cbCardIndex,m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID],GangCardResult);
						}
					}

					//���ý���
					m_GameClientView.m_ControlWnd.SetControlInfo(cbActionCard,cbActionMask,GangCardResult, m_GameClientView.m_bTingFlag[SwitchViewChairID(GetMeChairID())]);
					if (m_wCurrentUser==INVALID_CHAIR) SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
				}

				//����ʱ��
				if (m_wCurrentUser!=INVALID_CHAIR)
				{
					//����ʱ��
					WORD wTimeCount=TIME_OPERATE_CARD;
					//������ʱ��
					if ((m_bHearStatus==true)&&(m_wCurrentUser==GetMeChairID()) ) 
					{
						wTimeCount=TIME_HEAR_STATUS;
					}

					//����ʱ��
					SetGameTimer(m_wCurrentUser,TIME_OPERATE_CARD,wTimeCount);
				}
			}

			//���½���
			m_GameClientView.UpdateGameView(NULL);

			return true;
		}
	}

	return false;
}

//��Ϸ��ʼ
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//��������
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//����״̬
//	SetGameStatus(GS_MJ_PLAY);

	//���ñ���
	m_bHearStatus=false;
	m_wBankerUser=pGameStart->wBankerUser;
	m_wCurrentUser=pGameStart->wCurrentUser;
	m_cbLeftCardCount=MAX_REPERTORY-GAME_PLAYER*(MAX_COUNT-1)-1;

	//������Ϣ
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//����齫
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�����齫
	BYTE cbCardCount=(GetMeChairID()==m_wBankerUser)?MAX_COUNT:(MAX_COUNT-1);
	m_GameLogic.SwitchToCardIndex(pGameStart->cbCardData,cbCardCount,m_cbCardIndex);

	//���ý���
	bool bPlayerMode=(IsLookonMode()==false);
	m_GameClientView.m_GameScoreWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_HandCardControl.SetPositively(bPlayerMode);
	m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

	//�Թ۽���
	if (bPlayerMode==false)
	{
		m_GameClientView.SetHuangZhuang(false);
		m_GameClientView.SetStatusFlag(false,false);
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
		m_GameClientView.m_GameScoreWnd.ShowWindow(SW_HIDE);
	}

	//�����齫
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_cbHeapCardInfo[i][0]=0;
		m_cbHeapCardInfo[i][1]=HEAP_FULL_COUNT-(((i==m_wBankerUser)||((i+2)%4==m_wBankerUser))?34:34);
	}

	//�ַ��齫
	BYTE cbTakeCount=(MAX_COUNT-1)*GAME_PLAYER+1;
	/*
	BYTE cbSiceFirst=HIBYTE(pGameStart->wSiceCount);
	BYTE cbSiceSecond=LOBYTE(pGameStart->wSiceCount);
	*/
	WORD wTakeChairID=(m_wBankerUser+ 13 -(cbSiceFirst+cbSiceSecond))%GAME_PLAYER;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//������Ŀ
		BYTE cbValidCount=HEAP_FULL_COUNT-m_cbHeapCardInfo[wTakeChairID][1]-((i==0)?(cbSice2First+cbSice2Second-1)*2:0);
		BYTE cbRemoveCount=__min(cbValidCount,cbTakeCount);

		//��ȡ�齫
		cbTakeCount-=cbRemoveCount;
		m_cbHeapCardInfo[wTakeChairID][(i==0)?1:0]+=cbRemoveCount;

		//����ж�
		if (cbTakeCount==0)
		{
			m_wHeapHand=wTakeChairID;
			m_wHeapTail=(m_wBankerUser+ 13 -(cbSiceFirst+cbSiceSecond) )%GAME_PLAYER;
			break;
		}

		//�л�����
		wTakeChairID=(wTakeChairID+1)%GAME_PLAYER;
	}

	//�齫����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wViewChairID=SwitchViewChairID(i);

		//��Ͻ���
		m_GameClientView.m_WeaveCard[i][0].SetDisplayItem(true);
		m_GameClientView.m_WeaveCard[i][1].SetDisplayItem(true);
		m_GameClientView.m_WeaveCard[i][2].SetDisplayItem(true);
		m_GameClientView.m_WeaveCard[i][3].SetDisplayItem(true);
		m_GameClientView.m_HeapCard[wViewChairID].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1],HEAP_FULL_COUNT);

		//�û��齫
		if (wViewChairID!=2)
		{
			WORD wIndex=(wViewChairID>=3)?2:wViewChairID;
			m_GameClientView.m_UserCard[wIndex].SetCardData(CountArray(pGameStart->cbCardData),(i==m_wBankerUser));
		}
		else
		{
			BYTE cbBankerCard=(i==m_wBankerUser)?pGameStart->cbCardData[MAX_COUNT-1]:0;
			m_GameClientView.m_HandCardControl.SetCardData(pGameStart->cbCardData,MAX_COUNT-1,cbBankerCard);
		}

		//�Թ۽���
		if (bPlayerMode==false)
		{
			m_GameClientView.m_TableCard[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_DiscardCard[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[wViewChairID][0].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[wViewChairID][1].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[wViewChairID][2].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[wViewChairID][3].SetCardData(NULL,0);
		}
	}

	//������ʾ
	if ((bPlayerMode==true)&&(m_wCurrentUser!=INVALID_CHAIR))
	{
		WORD wMeChairID=GetMeChairID();
		if (m_wCurrentUser==wMeChairID) m_GameClientView.SetStatusFlag(true,false);
	}

	//��������
	if ((bPlayerMode==true)&&(pGameStart->cbUserAction!=WIK_NULL))
	{
		ShowOperateControl(pGameStart->cbUserAction,0);
		SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
	}

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	//������
	if (bPlayerMode==true) ActiveGameFrame();

	//��������
//	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	//����ʱ��
	if (m_wCurrentUser!=INVALID_CHAIR) SetGameTimer(m_wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);

	return true;
}

//�û�����
bool CGameClientDlg::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sOnSubOutCard.log",strTime);

	strTemp.Format("OnSubOutCard ");
	theApp.WriteLog(strFile, strTemp);

	//Ч����Ϣ
	ASSERT(wDataSize==sizeof(CMD_S_OutCard));
	if (wDataSize!=sizeof(CMD_S_OutCard)) return false;

	//��Ϣ����
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;

	//��������
	WORD wMeChairID=GetMeChairID();
	WORD wOutViewChairID=SwitchViewChairID(pOutCard->wOutCardUser);

	//���ñ���
	m_wCurrentUser=INVALID_CHAIR;
	m_wOutCardUser=pOutCard->wOutCardUser;
	m_cbOutCardData=pOutCard->cbOutCardData;

	strTemp.Format("wMeChairID=%d", wMeChairID);
	theApp.WriteLog(strFile, strTemp);
	//�����û�
	if ((IsLookonMode()==true)||(pOutCard->wOutCardUser!=wMeChairID))
	{
		//��������
		KillGameTimer(IDI_OPERATE_CARD); 
		const tagUserData * pUserData=GetUserData(pOutCard->wOutCardUser);

	strTemp.Format("pOutCard->wOutCardUser=%d, wOutViewChairID=%d", pOutCard->wOutCardUser,wOutViewChairID);
	theApp.WriteLog(strFile, strTemp);

	for (int i=0; i < 4;i++)
	{
		if ( m_GameClientView.m_bTingFlag[i])
		{
		strTemp.Format("ture %d", m_GameClientView.m_IntFirstTingFlag[i]);
		}
		else
		{
		strTemp.Format("false %d", m_GameClientView.m_IntFirstTingFlag[i]);
		}
	theApp.WriteLog(strFile, strTemp);
	}
		//����Ѿ����ˣ����Ҵ��˵�һ�ų�ȥ
		if ( m_GameClientView.m_IntFirstTingFlag[wOutViewChairID]<=1 && m_GameClientView.m_bTingFlag[wOutViewChairID] )
		{}
		else
		{
			PlayCardSound(pOutCard->cbOutCardData, pUserData->cbGender);
		}

		//���ƽ���
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetOutCardInfo(wOutViewChairID,pOutCard->cbOutCardData);

		//�����齫
		if (wOutViewChairID==2)
		{
			//ɾ���齫
			BYTE cbCardData[MAX_COUNT];
			m_GameLogic.RemoveCard(m_cbCardIndex,pOutCard->cbOutCardData);

			//�����齫
			BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
			m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount,0);
		}
		else
		{
			WORD wUserIndex=(wOutViewChairID>2)?2:wOutViewChairID;
			m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(false);
		}
		//�Ѿ���
		if (m_GameClientView.m_bTingFlag[wOutViewChairID]  )
		{
				m_GameClientView.m_IntFirstTingFlag[wOutViewChairID]++;

	strTemp.Format("wOutViewChairID=%d, %d ", wOutViewChairID, m_GameClientView.m_IntFirstTingFlag[wOutViewChairID]);
	theApp.WriteLog(strFile, strTemp);

		}
	}

	return true;
}

//������Ϣ
bool CGameClientDlg::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//��������
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	//���ñ���
	WORD wMeChairID=GetMeChairID();
	m_wCurrentUser=pSendCard->wCurrentUser;

	//�����齫
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		//�����齫
		WORD wOutViewChairID=SwitchViewChairID(m_wOutCardUser);
		//����Ѿ����ˣ����Ҵ��˵�һ�ų�ȥ
		if ( m_GameClientView.m_IntFirstTingFlag[wOutViewChairID]==1 && m_GameClientView.m_bTingFlag[wOutViewChairID] )
			m_cbOutCardData=0x38;//0;
		m_GameClientView.m_DiscardCard[wOutViewChairID].AddCardItem(m_cbOutCardData);

		//���ñ���
		m_cbOutCardData=0;
		m_wOutCardUser=INVALID_CHAIR;
	}

	//���ƴ���
	if (pSendCard->cbCardData!=0)
	{
		//ȡ�ƽ���
		WORD wViewChairID=SwitchViewChairID(m_wCurrentUser);
		if (wViewChairID!=2)
		{
			WORD wUserIndex=(wViewChairID>2)?2:wViewChairID;
			m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(true);
		}
		else
		{
			m_cbCardIndex[m_GameLogic.SwitchToCardIndex(pSendCard->cbCardData)]++;
			m_GameClientView.m_HandCardControl.SetCurrentCard(pSendCard->cbCardData);
		}
		
		//�۳��齫
		DeductionTableCard(true);
	}

	//��ǰ�û�
	if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID))
	{
		//������
		ActiveGameFrame();

		//��������
		if (pSendCard->cbActionMask!=WIK_NULL)
		{
			//��ȡ����
			BYTE cbActionCard=pSendCard->cbCardData;
			BYTE cbActionMask=pSendCard->cbActionMask;

			//��������
			tagGangCardResult GangCardResult;
			ZeroMemory(&GangCardResult,sizeof(GangCardResult));

			//�����ж�
			if ((cbActionMask&(WIK_GANG|WIK_FILL))!=0)
			{
				WORD wMeChairID=GetMeChairID();
				m_GameLogic.AnalyseGangCard(m_cbCardIndex,m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID],GangCardResult);
			}

			//����
			bool isHaveB = false;
			for (int j=0; j < 4; j++)
			{
				if ( m_WeaveItemArray[m_wCurrentUser][j].cbCenterCard == cbActionCard)
				{
					GangCardResult.cbCardCount=0;
					GangCardResult.cbCardData[0]=0;
					GangCardResult.cbCardData[1]=0;
					GangCardResult.cbCardData[2]=0;
					GangCardResult.cbCardData[3]=0;
					isHaveB = true;
				}
			}
			//���ý���
			//����Ѿ�����
			if ( m_GameClientView.m_bTingFlag[SwitchViewChairID(GetMeChairID())])
			{
				if (cbActionCard&WIK_CHI_HU)
				{
					m_GameClientView.m_ControlWnd.SetControlInfo(cbActionCard,cbActionMask,GangCardResult, m_GameClientView.m_bTingFlag[SwitchViewChairID(GetMeChairID())]);
				}
			}
			else
			{
						m_GameClientView.m_ControlWnd.SetControlInfo(cbActionCard,cbActionMask,GangCardResult, m_GameClientView.m_bTingFlag[SwitchViewChairID(GetMeChairID())]);

			}
		}
	}

	//������ʾ
	m_GameClientView.SetStatusFlag((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID),false);

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	//����ʱ��
	WORD wTimeCount=TIME_OPERATE_CARD;
	//������ʱ��
	if ((m_bHearStatus==true)&&(m_wCurrentUser==wMeChairID) ) 
	{
		wTimeCount=TIME_HEAR_STATUS;
	}

	//����ʱ��
	SetGameTimer(m_wCurrentUser,IDI_OPERATE_CARD,wTimeCount);

	return true;
}

//������ʾ
bool CGameClientDlg::OnSubOperateNotify(const void * pBuffer, WORD wDataSize)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sOnSubOperateNotify.log",strTime);

	strTemp.Format("OnSubOperateNotify");
	theApp.WriteLog(strFile, strTemp);
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_OperateNotify));
	if (wDataSize!=sizeof(CMD_S_OperateNotify)) return false;

	//��������
	CMD_S_OperateNotify * pOperateNotify=(CMD_S_OperateNotify *)pBuffer;

	//�û�����
	if ((IsLookonMode()==false)&&(pOperateNotify->cbActionMask!=WIK_NULL))
	{
		//��ȡ����
		BYTE cbActionMask=pOperateNotify->cbActionMask;
		BYTE cbActionCard=pOperateNotify->cbActionCard;

		//��������
		tagGangCardResult GangCardResult;
		ZeroMemory(&GangCardResult,sizeof(GangCardResult));

		//�����ж�
		if ((cbActionMask&(WIK_GANG|WIK_FILL))!=0)
		{
			//�������
			if ((m_wCurrentUser==INVALID_CHAIR)&&(cbActionCard!=0))
			{
				GangCardResult.cbCardCount=1;
				GangCardResult.cbCardData[0]=cbActionCard;
			}

			//�Լ�����
			if ((m_wCurrentUser==GetMeChairID())||(cbActionCard==0))
			{
				WORD wMeChairID=GetMeChairID();
				m_GameLogic.AnalyseGangCard(m_cbCardIndex,m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID],GangCardResult);
			}
		}

		//���ý���
		ActiveGameFrame();
		//��һ������
		if ( m_GameClientView.m_IntFirstTingFlag[SwitchViewChairID(pOperateNotify->wOperateUser)]==1 && m_GameClientView.m_bTingFlag[SwitchViewChairID(pOperateNotify->wOperateUser)] )
		{
			
			SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_TING_STATUS);
			return true;
		}
		else
		{

			//����
			bool isHaveB = false;
			for (int j=0; j < 4; j++)
			{
				if ( m_WeaveItemArray[GetMeChairID()][j].cbCenterCard == cbActionCard)
				{
					GangCardResult.cbCardCount=0;
					GangCardResult.cbCardData[0]=0;
					GangCardResult.cbCardData[1]=0;
					GangCardResult.cbCardData[2]=0;
					GangCardResult.cbCardData[3]=0;
					isHaveB = true;
				}
			}


			//����Ѿ�����
			if ( m_GameClientView.m_bTingFlag[SwitchViewChairID(GetMeChairID())])
			{
				
				if (cbActionMask&WIK_CHI_HU)
				{
					SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
					m_GameClientView.m_ControlWnd.SetControlInfo(cbActionCard,cbActionMask,GangCardResult, m_GameClientView.m_bTingFlag[SwitchViewChairID(GetMeChairID())]);
				}
				else
				{
					SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD-20);
				}
			}
			else
			{
					SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
					m_GameClientView.m_ControlWnd.SetControlInfo(cbActionCard,cbActionMask,GangCardResult, m_GameClientView.m_bTingFlag[SwitchViewChairID(GetMeChairID())]);
			}

		}
	}

	return true;
}

//�������
bool CGameClientDlg::OnSubOperateResult(const void * pBuffer, WORD wDataSize)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sOnSubOperateResult.log",strTime);

			strTemp.Format("OnSubOperateResult");
			theApp.WriteLog(strFile, strTemp);
	//Ч����Ϣ
	ASSERT(wDataSize==sizeof(CMD_S_OperateResult));
	if (wDataSize!=sizeof(CMD_S_OperateResult)) return false;

	//��Ϣ����
	CMD_S_OperateResult * pOperateResult=(CMD_S_OperateResult *)pBuffer;

	//��������
	BYTE cbPublicCard=TRUE;
	WORD wOperateUser=pOperateResult->wOperateUser;
	BYTE cbOperateCard=pOperateResult->cbOperateCard;
	WORD wOperateViewID=SwitchViewChairID(wOperateUser);

	//���Ʊ���
	if (pOperateResult->cbOperateCode!=WIK_NULL)
	{
		m_cbOutCardData=0;
		m_wOutCardUser=INVALID_CHAIR;
	}

	//�������
	if ((pOperateResult->cbOperateCode&(WIK_GANG|WIK_FILL))!=0)
	{
		//���ñ���
		m_wCurrentUser=INVALID_CHAIR;

		//����齫
		BYTE cbWeaveIndex=0xFF;
		for (BYTE i=0;i<m_cbWeaveCount[wOperateUser];i++)
		{
			BYTE cbWeaveKind=m_WeaveItemArray[wOperateUser][i].cbWeaveKind;
			BYTE cbCenterCard=m_WeaveItemArray[wOperateUser][i].cbCenterCard;
			if ((cbCenterCard==cbOperateCard)&&(cbWeaveKind==WIK_PENG))
			{
				cbWeaveIndex=i;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=TRUE;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;
				break;
			}
		}

		//����齫
		if (cbWeaveIndex==0xFF)
		{
			//�����ж�
			cbPublicCard=(pOperateResult->wProvideUser==wOperateUser)?FALSE:TRUE;

			//�����齫
			cbWeaveIndex=m_cbWeaveCount[wOperateUser]++;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=cbPublicCard;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterCard=cbOperateCard;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;
		}

		//��Ͻ���
		BYTE cbWeaveCard[4]={0,0,0,0},cbWeaveKind=pOperateResult->cbOperateCode;
		BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(cbWeaveKind,cbOperateCard,cbWeaveCard);
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetCardData(cbWeaveCard,cbWeaveCardCount);
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetDisplayItem((cbPublicCard==TRUE)?true:false);

		//�齫����
		if (GetMeChairID()==wOperateUser)
		{
			m_cbCardIndex[m_GameLogic.SwitchToCardIndex(pOperateResult->cbOperateCard)]=0;
		}

		//�����齫
		if (GetMeChairID()==wOperateUser)
		{
			BYTE cbCardData[MAX_COUNT];
			BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
			m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount,0);
		}
		else
		{
			WORD wUserIndex=(wOperateViewID>=3)?2:wOperateViewID;
			BYTE cbCardCount=MAX_COUNT-m_cbWeaveCount[wOperateUser]*3;
			m_GameClientView.m_UserCard[wUserIndex].SetCardData(cbCardCount-1,false);
		}
	}
	else if (pOperateResult->cbOperateCode!=WIK_NULL)
	{
		//���ñ���
		m_wCurrentUser=pOperateResult->wOperateUser;

		//�������
		BYTE cbWeaveIndex=m_cbWeaveCount[wOperateUser]++;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=TRUE;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterCard=cbOperateCard;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;

		//��Ͻ���
		BYTE cbWeaveCard[4]={0,0,0,0},cbWeaveKind=pOperateResult->cbOperateCode;
		BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(cbWeaveKind,cbOperateCard,cbWeaveCard);
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetCardData(cbWeaveCard,cbWeaveCardCount);

		//ɾ���齫
		if (GetMeChairID()==wOperateUser)
		{
			m_GameLogic.RemoveCard(cbWeaveCard,cbWeaveCardCount,&cbOperateCard,1);
			m_GameLogic.RemoveCard(m_cbCardIndex,cbWeaveCard,cbWeaveCardCount-1);
		}

		//�����齫
		if (GetMeChairID()==wOperateUser)
		{
			BYTE cbCardData[MAX_COUNT];
			BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
			m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount-1,cbCardData[cbCardCount-1]);
		}
		else
		{
			WORD wUserIndex=(wOperateViewID>=3)?2:wOperateViewID;
			BYTE cbCardCount=MAX_COUNT-m_cbWeaveCount[wOperateUser]*3;
			m_GameClientView.m_UserCard[wUserIndex].SetCardData(cbCardCount-1,true);
		}
	}

	//���ý���
	m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.SetUserAction(wOperateViewID,pOperateResult->cbOperateCode);
	m_GameClientView.SetStatusFlag((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()),false);

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	const tagUserData * pUserData=GetUserData(wOperateUser);
	//��������
	PlayActionSound(pOperateResult->cbOperateCode, pUserData->cbGender);

	//����ʱ��
	if (m_wCurrentUser!=INVALID_CHAIR)
	{
		//����ʱ��
		WORD wTimeCount=TIME_OPERATE_CARD;
		//
		if ((m_bHearStatus==true)&&(m_wCurrentUser==GetMeChairID()) )
		{
			wTimeCount=TIME_HEAR_STATUS;
		}

		//����ʱ��
		SetGameTimer(m_wCurrentUser,IDI_OPERATE_CARD,wTimeCount);
	}

	return true;
}
	//����
bool CGameClientDlg::OnSubTingCard(const void * pBuffer, WORD wDataSize)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sOnSubTingCard.log",strTime);

	strTemp.Format("OnSubTingCard ");
	theApp.WriteLog(strFile, strTemp);

	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameTingCard));
	if (wDataSize!=sizeof(CMD_S_GameTingCard)) return false;

	//��Ϣ����
	CMD_S_GameTingCard * pGameEnd=(CMD_S_GameTingCard *)pBuffer;


	WORD wOutViewChairID=SwitchViewChairID(pGameEnd->wTingCardUser);

	m_GameClientView.m_bTingFlag[wOutViewChairID] = true;

//	m_GameClientView.m_bTingFlag[pGameEnd->wTingCardUser] = true;

	strTemp.Format("OnSubTingCard wOutViewChairID=%d", wOutViewChairID);
	theApp.WriteLog(strFile, strTemp);

//	strTemp.Format("pGameEnd->wTingCardUser=%d", pGameEnd->wTingCardUser);
//	theApp.WriteLog(strFile, strTemp);


	return true;

}
//��Ϸ����
bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//����״̬
	SetGameStatus(GS_MJ_FREE);
	m_GameClientView.SetStatusFlag(false,false);

	//ɾ����ʱ��
	KillGameTimer(IDI_OPERATE_CARD);

	//���ÿؼ�
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_HandCardControl.SetPositively(false);

	//��������
	if (pGameEnd->cbChiHuCard!=0)
	{
		tagGameScoreInfo GameScoreInfo;
		GameScoreInfo.wBankerUser=m_wBankerUser;
		GameScoreInfo.cbChiHuCard=pGameEnd->cbChiHuCard;
		GameScoreInfo.wProvideUser=pGameEnd->wProvideUser;
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			const tagUserData * pUserData=GetUserData(i);
			GameScoreInfo.lScore[i]=pGameEnd->lGameScore[i];
			GameScoreInfo.wCardType[i]=pGameEnd->wChiHuKind[i];
			lstrcpyn(GameScoreInfo.szUserName[i],pUserData->szName,CountArray(GameScoreInfo.szUserName[i]));
		}
		m_GameClientView.m_GameScoreWnd.SetScoreInfo(GameScoreInfo);
	}
	else 
	{
		DeductionTableCard(true);
		m_GameClientView.SetHuangZhuang(true);
	}

	//�û��齫
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=SwitchViewChairID(i);
		if (pGameEnd->wChiHuKind[i]!=CHK_NULL) m_GameClientView.SetUserAction(wViewChairID,WIK_CHI_HU);
		m_GameClientView.m_TableCard[wViewChairID].SetCardData(pGameEnd->cbCardData[i],pGameEnd->cbCardCount[i]);
	}

	//�����齫
	m_GameClientView.m_UserCard[0].SetCardData(0,false);
	m_GameClientView.m_UserCard[1].SetCardData(0,false);
	m_GameClientView.m_UserCard[2].SetCardData(0,false);
	m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0);

	//��������
	LONG lScore=pGameEnd->lGameScore[GetMeChairID()];
	if (lScore>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
	else if (lScore<0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
	else PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_END"));

	//���ý���
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		SetGameTimer(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
	}

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	return true;
}

//��������
void CGameClientDlg::PlayCardSound(BYTE cbCardData, int sexInt)
{
	//��������
	TCHAR szSoundName[16]=TEXT("");
	TCHAR szKind[]={TEXT('W'),TEXT('S'),TEXT('T'),TEXT('Z')};

	//��������
	BYTE cbValue=(cbCardData&MASK_VALUE);
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;
	if (sexInt == 1)
	{
		_snprintf(szSoundName,CountArray(szSoundName),TEXT("%c_%d"),szKind[cbColor],cbValue);
	}
	else
	{
		_snprintf(szSoundName,CountArray(szSoundName),TEXT("W_%c_%d"),szKind[cbColor],cbValue);	
	}

	//��������
	PlayGameSound(AfxGetInstanceHandle(),szSoundName);

	return;
}

//��������
void CGameClientDlg::PlayActionSound(BYTE cbAction , int sexInt )
{
	switch (cbAction)
	{
	case WIK_NULL:		//ȡ��
		{
				//������
	if (sexInt == 1)
	{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	}
	else
	{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("W_OUT_CARD"));
	}
			break;
		}
	case WIK_PENG:		//����
		{
				//������
	if (sexInt == 1)
	{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("A_PENG"));
	}
	else
	{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("W_A_PENG"));
	}
			break;
		}
	case WIK_FILL:		//����
		{
				//������
	if (sexInt == 1)
	{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("A_FILL"));
	}
	else
	{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("W_A_FILL"));
	}
			break;
		}
	case WIK_GANG:		//����
		{
				//������
	if (sexInt == 1)
	{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("A_GANG"));
	}
	else
	{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("W_A_GANG"));
	}
			break;
		}
	case WIK_CHI_HU:	//�Ժ�
		{
				//������
	if (sexInt == 1)
	{
					PlayGameSound(AfxGetInstanceHandle(),TEXT("A_CHI_HU"));
	}
	else
	{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("W_A_CHI_HU"));
	}
			break;
		}
	}

	return;
}

//�۳��齫
void CGameClientDlg::DeductionTableCard(bool bHeadCard)
{
	if (bHeadCard==true)
	{
		//�л�����
		BYTE cbHeapCount=m_cbHeapCardInfo[m_wHeapHand][0]+m_cbHeapCardInfo[m_wHeapHand][1];
		if (cbHeapCount==HEAP_FULL_COUNT) m_wHeapHand=(m_wHeapHand+1)%CountArray(m_cbHeapCardInfo);

		//�����齫
		m_cbLeftCardCount--;
		m_cbHeapCardInfo[m_wHeapHand][0]++;

		//�����齫
		WORD wHeapViewID=SwitchViewChairID(m_wHeapHand);
		WORD wMinusHeadCount=m_cbHeapCardInfo[m_wHeapHand][0];
		WORD wMinusLastCount=m_cbHeapCardInfo[m_wHeapHand][1];
		m_GameClientView.m_HeapCard[wHeapViewID].SetCardData(wMinusHeadCount,wMinusLastCount,HEAP_FULL_COUNT);
	}
	else
	{
		//�л�����
		BYTE cbHeapCount=m_cbHeapCardInfo[m_wHeapTail][0]+m_cbHeapCardInfo[m_wHeapTail][1];
		if (cbHeapCount==HEAP_FULL_COUNT) m_wHeapTail=(m_wHeapTail+3)%CountArray(m_cbHeapCardInfo);

		//�����齫
		m_cbLeftCardCount--;
		m_cbHeapCardInfo[m_wHeapTail][1]++;

		//�����齫
		WORD wHeapViewID=SwitchViewChairID(m_wHeapTail);
		WORD wMinusHeadCount=m_cbHeapCardInfo[m_wHeapTail][0];
		WORD wMinusLastCount=m_cbHeapCardInfo[m_wHeapTail][1];
		m_GameClientView.m_HeapCard[wHeapViewID].SetCardData(wMinusHeadCount,wMinusLastCount,HEAP_FULL_COUNT);
	}

	return;
}

//��ʾ����
bool CGameClientDlg::ShowOperateControl(BYTE cbUserAction, BYTE cbActionCard)
{
	//��������
	tagGangCardResult GangCardResult;
	ZeroMemory(&GangCardResult,sizeof(GangCardResult));

	//�����ж�
	if ((cbUserAction&(WIK_GANG|WIK_FILL))!=0)
	{
		//�������
		if (cbActionCard!=0)
		{
			GangCardResult.cbCardCount=1;
			GangCardResult.cbCardData[0]=cbActionCard;
		}

		//�Լ�����
		if (cbActionCard==0)
		{
			WORD wMeChairID=GetMeChairID();
			m_GameLogic.AnalyseGangCard(m_cbCardIndex,m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID],GangCardResult);
		}
	}

	//��ʾ����
	//����Ѿ�����
	if ( m_GameClientView.m_bTingFlag[SwitchViewChairID(GetMeChairID())])
	{
		if (cbUserAction&WIK_CHI_HU)
		{
				m_GameClientView.m_ControlWnd.SetControlInfo(cbActionCard,cbUserAction,GangCardResult, m_GameClientView.m_bTingFlag[SwitchViewChairID(GetMeChairID())]);

		}
	}
	else
	{
		m_GameClientView.m_ControlWnd.SetControlInfo(cbActionCard,cbUserAction,GangCardResult, m_GameClientView.m_bTingFlag[SwitchViewChairID(GetMeChairID())]);

	}

	return true;
}
LRESULT CGameClientDlg::OnShowSZ(WPARAM wParam, LPARAM lParam)
{
	m_GameClientView.m_nDiceCount1 = cbSiceFirst;
	m_GameClientView.m_nDiceCount2 = cbSiceSecond;

	//strTemp.Format("m_nDiceCount1  =%d, m_GameClientView.m_nDiceCount2", m_GameClientView.m_nDiceCount1,m_GameClientView.m_nDiceCount2);
	//theApp.WriteLog(strFile, strTemp);
	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));
	m_GameClientView.m_tDice = 1;
	AfxBeginThread( MyThreadProc, this);

	return 0;
}
//��ʾ��Ϣ
int CGameClientDlg::ShowMessageBox(LPCTSTR pszMessage, UINT nType)
{
//	tagGameServer * pGameServer=m_pListServer->GetItemInfo();
	int nResult=MessageBox(pszMessage,"",nType);
	return nResult;
}

//���ư�ť
LRESULT CGameClientDlg::OnTingCard(WPARAM wParam, LPARAM lParam)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sOnTingCard.log",strTime);

	strTemp.Format("into OnTingCard");
	theApp.WriteLog(strFile, strTemp);

	WORD wMeChairID=GetMeChairID();

	strTemp.Format("wMeChairID=%d", wMeChairID);
	theApp.WriteLog(strFile, strTemp);
		TCHAR szBuffer[512]=TEXT("���ƺ��ܸ�����������(Ҳ����������),ֱ������Ϊֹ!");

		int nResult=ShowMessageBox(szBuffer,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1);
		if (nResult==IDYES) 
		{
			m_GameClientView.m_btTingCard.ShowWindow(SW_HIDE);
			//��������������Ϣ
			//��������
			CMD_C_TingCard  OutCard;
			OutCard.cbCardData=0;
			SendData(SUB_C_TING_CARD,&OutCard,sizeof(OutCard));

			m_bHearStatus = true;//��������״̬
		}
	return 0;
}
//��ʼ��ť
LRESULT CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	//��������
	KillGameTimer(IDI_START_GAME);
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_GameScoreWnd.ShowWindow(SW_HIDE);

	//���ý���
	m_GameClientView.SetHuangZhuang(false);
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.SetBankerUser(INVALID_CHAIR);
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);

	//�齫����
	m_GameClientView.m_UserCard[0].SetCardData(0,false);
	m_GameClientView.m_UserCard[1].SetCardData(0,false);
	m_GameClientView.m_UserCard[2].SetCardData(0,false);
	m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0);

	//�齫����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_TableCard[i].SetCardData(NULL,0);
		m_GameClientView.m_DiscardCard[i].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[i][0].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[i][1].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[i][2].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[i][3].SetCardData(NULL,0);
	}

	//�����齫
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_cbHeapCardInfo[i][0]=0;
		m_cbHeapCardInfo[i][1]=HEAP_FULL_COUNT-(((i==m_wBankerUser)||((i+2)%4==m_wBankerUser))?34:34);
		m_GameClientView.m_HeapCard[SwitchViewChairID(i)].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1],HEAP_FULL_COUNT);
	}

	//״̬����
	m_bHearStatus=false;

	//��Ϸ����
	m_wCurrentUser=INVALID_CHAIR;

	//������Ϣ
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//����齫
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�����齫
	m_wHeapHand=0;
	m_wHeapTail=0;
	ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

	//�齫����
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

	//������Ϣ
	SendUserReady(NULL,0);

	return 0;
}

//���Ʋ���
LRESULT CGameClientDlg::OnOutCard(WPARAM wParam, LPARAM lParam)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sOnOutCard.log",strTime);

	strTemp.Format("OnOutCard ");
	//theApp.WriteLog(strFile, strTemp);
	//�����ж�
	if ((IsLookonMode()==true)||(m_wCurrentUser!=GetMeChairID())) return 0;

	const tagUserData * pUserData=GetUserData(m_wCurrentUser);
	//�����ж�
	//����Ѿ�������,���ܻ���
	//&&   m_GameClientView.m_IntFirstTingFlag[2] > 1
//	const tagUserData * mypUserData=GetUserData(GetMeChairID());

	if ((m_bHearStatus==true && m_GameClientView.m_IntFirstTingFlag[2] > 0 )&&(m_GameClientView.m_HandCardControl.GetCurrentCard()!=(BYTE)wParam)) 
	{

		strTemp.Format("����Ѿ�������,���ܻ��� %d, %d", pUserData->dwUserID, m_GameClientView.m_IntFirstTingFlag[2]);
		theApp.WriteLog(strFile, strTemp);

		return 0;
	}
	else
	{
		if ( pUserData->dwUserID == 4)
		{
			strTemp.Format("m_GameClientView.m_IntFirstTingFlag[2]=%d ", m_GameClientView.m_IntFirstTingFlag[2]);
			theApp.WriteLog(strFile, strTemp);
		}

	}

	//�Լ� �Ѿ��� ��
	if (m_GameClientView.m_bTingFlag[2]  )
	{
			m_GameClientView.m_IntFirstTingFlag[2]++;

//strTemp.Format("wOutViewChairID=%d, %d ", wOutViewChairID, m_GameClientView.m_IntFirstTingFlag[wOutViewChairID]);
//theApp.WriteLog(strFile, strTemp);

	}

	//���ñ���
	m_wCurrentUser=INVALID_CHAIR;
	BYTE cbOutCardData=(BYTE)wParam;
	m_GameLogic.RemoveCard(m_cbCardIndex,cbOutCardData);

	//�����齫
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
	m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount,0);

	//���ý���
	KillGameTimer(IDI_OPERATE_CARD);
	m_GameClientView.UpdateGameView(NULL);
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetOutCardInfo(2,cbOutCardData);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);

if (m_GameClientView.m_bTingFlag[2])
{
	strTemp.Format("m_GameClientView.m_bTingFlag[2]=true,m_GameClientView.m_IntFirstTingFlag[2]=%d ", m_GameClientView.m_IntFirstTingFlag[2]);
}
else
{
	strTemp.Format("m_GameClientView.m_bTingFlag[2]=false,m_GameClientView.m_IntFirstTingFlag[2]=%d ", m_GameClientView.m_IntFirstTingFlag[2]);

}
	theApp.WriteLog(strFile, strTemp);

	//��������
if (m_GameClientView.m_bTingFlag[2] && m_GameClientView.m_IntFirstTingFlag[2] <= 1 ) 
{
	strTemp.Format("�������� ");
	theApp.WriteLog(strFile, strTemp);
}
else
{
	PlayCardSound(cbOutCardData, pUserData->cbGender);
}

	//��������
	CMD_C_OutCard OutCard;
	OutCard.cbCardData=cbOutCardData;
	SendData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard));

	return 0;
}

//�齫����
LRESULT CGameClientDlg::OnCardOperate(WPARAM wParam, LPARAM lParam)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sOnCardOperate.log",strTime);

	strTemp.Format("into OnCardOperate");
	WriteLog(strFile, strTemp);
	//��������
	BYTE cbOperateCode=(BYTE)(wParam);
	BYTE cbOperateCard=(BYTE)(LOWORD(lParam));
	//����Ѿ����ˣ���ֻ�ܲ���ȡ��
	if (m_GameClientView.m_bTingFlag[SwitchViewChairID(GetMeChairID())] && cbOperateCode != WIK_CHI_HU)
	{
		cbOperateCode = WIK_NULL;
		cbOperateCard = 0;

		strTemp.Format("����Ѿ��� WIK_NULL 0");
		WriteLog(strFile, strTemp);

	}

	//״̬�ж�
	if ((m_wCurrentUser==GetMeChairID())&&(cbOperateCode==WIK_NULL))
	{
		m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
		return 0;
	};

	//��������
	KillGameTimer(IDI_OPERATE_CARD);
	m_GameClientView.SetStatusFlag(false,true);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);

	//��������
	CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=cbOperateCode;
	OperateCard.cbOperateCard=cbOperateCard;
	SendData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	return 0;
}

//��ʾ�û�MJ
void CGameClientDlg::ShowUserMJ()
{
	m_GameClientView.m_tDice ++;
	m_GameClientView.UpdateGameView(NULL);
	if (1 == showNext)
	{
		showNext++;

		PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

		isStartInt = 1;
		m_GameClientView.m_tDice = 1;
		m_GameClientView.m_nDiceCount1 = cbSice2First;
		m_GameClientView.m_nDiceCount2 = cbSice2Second;
		AfxBeginThread( MyThreadProc, this);

		return ;
	}
	if( isStartInt == 1)
	{
		OnSubGameStart(&myGameStart ,sizeof(myGameStart));
		isStartInt = 0;
	}

	
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//������Ƶ����
void CGameClientDlg::ResetVideoWindowPostion(int wInt, int hInt)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sResetVideoWindowPostion.log",strTime);

	strTemp.Format("%s", "ResetVideoWindowPostion()");
	WriteLog(strFile, strTemp);

	uiShowInt++;
//�����Լ�����Ƶ��ʾ����
	int myX,myY,myW,myH;
	myW= MYSLEF_VIDEO_W;
	myH= MYSLEF_VIDEO_H;
	myX= 10;//
	myY= hInt-myH-10; 
	m_showSelfVideo.MoveWindow( myX, myY, myW, myH);
	if ( 1== uiShowInt)
	m_showSelfVideo.ShowWindow(true);

	//�Լ�������
	if ( 1== uiShowInt)
	m_MyselfOpKuang.ShowWindow(true);
	m_MyselfOpKuang.MoveWindow(myX, myY+myH+15, 80, 20);
	//��̬����X,Y
	int x,y,w,h;
	int xx,yy,ww,hh;

	int xOff = wInt;
	int yOff = 25;//m_GameClientView.yOffInt;


	strTemp.Format("xOff = %d", xOff);
	WriteLog(strFile, strTemp);

	CString showMsg;
	showMsg.Format("xOff=%d,yOff=%d", xOff, yOff);

	int screenInt = GetSystemMetrics(SM_CXSCREEN);
	ww= AV_TOOLS_ALL_BUTTON_W  ;
	hh= AV_TOOLS_BUTTON_H  ;

	int opW,opH;
	opW = AV_TOOLS_W  ;
	opH = AV_TOOLS_H + 8 ;


	w= VIDEO_W;
	h= VIDEO_H;

	int topX = 8;
	int topY = 34+ yOff;

	x = topX ;//+ xOff;
	y = topY+VIDEO_PIC_H/3;

	int rlTopY = hInt/2;

	int topLeftY = topY+VIDEO_PIC_H/3;

	int rightX= xOff - (VIDEO_PIC_W +OP_BACK_PIC_W ) ;//577+xOff;
	int leftX = 8;

	if ( 800 == screenInt)
	{
		y = 70;
	}
	else if (1152 == screenInt)
	{
		y = 210;
	}


	int myDeskInt = GetMeChairID();

	if ( myDeskInt > GAME_PLAYER)
	{
		strTemp.Format("myDeskInt=%d",myDeskInt);
		WriteLog(strFile, strTemp);
		m_GameClientView.xOffInt = -1;
		m_GameClientView.yOffInt = -1;
		uiShowInt=0;
		return ;
	}
	else
	{
		strTemp.Format("myDeskInt=%d",myDeskInt);
		WriteLog(strFile, strTemp);
	}

	CStatic * myVideoList[GAME_PLAYER] = {&m_showRemoteVideo, &m_showRemoteVideo1, &m_showRemoteVideo2, &m_showRemoteVideo3};
	CLabelEx * myToolsList[GAME_PLAYER]={&m_OpKuang, &m_OpKuang1, &m_OpKuang2, &m_OpKuang3};
	for (int i=0; i < GAME_PLAYER; i++)
	{
		if ( i == myDeskInt)
		{
			m_GameClientView.m_uVideoInt[i] = 0;
			myVideoList[i]->ShowWindow(false);
			myToolsList[i]->ShowWindow(false);
		}
		else
		{
			m_GameClientView.m_uVideoInt[i] = 1;
			if ( 1== uiShowInt)
			{
			myVideoList[i]->ShowWindow(true);
			myToolsList[i]->ShowWindow(true);
			}

		}//End if

	}	//End for

//top right left
CStatic * myVideo[GAME_PLAYER-1];
CLabelEx * myTools[GAME_PLAYER-1];
CPoint * myPt[GAME_PLAYER-1];
int listInt [3]={1, 2, 3};

if ( myDeskInt > (GAME_PLAYER-1) || myDeskInt < 0)
return ;
	//˳ʱ��
//0,1,2,3,0,1,2,3
bool m_bDeasilOrder = true;
	if (m_bDeasilOrder==true)
	{
	switch(myDeskInt)
	{
	case 0:
		{
			listInt[0]=1;
			listInt[1]=2;
			listInt[2]=3;

		}
		break;
	case 1:
		{
			//0,2,3


			listInt[0]=2;
			listInt[1]=3;
			listInt[2]=0;

		}
		break;
	case 2:
		{
			//0,1,3

			listInt[0]=3;
			listInt[1]=0;
			listInt[2]=1;

		}
		break;
	case 3:
		{
			//0,1,2
			listInt[0]=0;
			listInt[1]=1;
			listInt[2]=2;

		}
		break;
	}
	}
//NI
//3,2,1,0,3,2,1,0
	else
	{
	
	switch(myDeskInt)
	{
	case 0:
		{

			listInt[0]=3;
			listInt[1]=2;
			listInt[2]=1;

		}
		break;
	case 1:
		{
			//0,2,3


			listInt[0]=0;
			listInt[1]=3;
			listInt[2]=2;

		}
		break;
	case 2:
		{
			//0,1,3

			listInt[0]=1;
			listInt[1]=0;
			listInt[2]=3;

		}
		break;
	case 3:
		{
			//0,1,2
			listInt[0]=2;
			listInt[1]=1;
			listInt[2]=0;

		}
		break;
	}
	}

//top right left
for (int i=0; i < GAME_PLAYER-1; i++)
{
myVideo[i]=myVideoList[listInt[i]];
myTools[i]=myToolsList[listInt[i]];
myPt[i]=&m_GameClientView.m_PtVideo[listInt[i]];
//
		/*
		if ( i==1 )
		{	
			x = rightX;
		}
//
		else if(i == 2)
		{
			y = topY;
			x = (xOff-VIDEO_PIC_W)/2 +30;
		}
*/
		switch(i)
		{

//ok
		case 0:
			y = rlTopY;
			x = leftX;			
			break;
//ok
		case 1:
			y = topLeftY;
			x = leftX;
			break;
//ok
		case 2:
			y = topY;
			x = rightX;
			break;

		}

	myVideo[i]->MoveWindow( x, y, w, h);
	myPt[i]->x =x-VIDEO_X_OFF;
	myPt[i]->y =y-VIDEO_Y_OFF;

	xx = x+opW;
	yy = y+opH;
	myTools[i]->MoveWindow(xx, yy, ww, hh);
}//End for


m_GameClientView.UpdateGameView(NULL);

	strTemp.Format("UpdateGameView");
	WriteLog(strFile, strTemp);
}


	//RESET UI
LRESULT CGameClientDlg::OnResetUI(WPARAM wParam, LPARAM lParam)
{
	int wInt=(int)wParam;
	int hInt=(int)lParam;

	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sOnResetUI.log",strTime);

	strTemp.Format("into OnResetUI(%d, %d", wInt, hInt);
	WriteLog(strFile, strTemp);

	if ( wInt >0 && wInt < 1500)
	{
		ResetVideoWindowPostion( wInt,  hInt);
	}
	return 0;
}
