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
	m_wBankerUser=INVALID_CHAIR;

	//��λ������صı���
	RepositTableFrameSink();
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
bool __cdecl CTableFrameSink::InitTableFrameSink(IUnknownEx *pIUnknownEx)
{
	//��ѯ�ӿ�
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//��ʼ����ӡ��Ϣ����
	m_GameLogic.m_Debug.InitDebug(m_pITableFrame);

	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);
	return true;
}

//��λ����
void __cdecl CTableFrameSink::RepositTableFrameSink()
{

	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

	//������Ϣ
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));
	//������Ϣ
	m_bSendCount = 0;
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
	b_HuangZhuang=false;
	m_cbChiHuCard=0;

	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//����˿�
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	//�������
	//m_pITableFrame=NULL;
	//m_pGameServiceOption=NULL;

	ZeroMemory(m_cbSendCardData,sizeof(m_cbSendCardData));
	ZeroMemory(m_cbHuaCardIndex,sizeof(m_cbHuaCardIndex));
	ZeroMemory(m_cbHuaWeaveCardIndex,sizeof(m_cbHuaWeaveCardIndex));
	ZeroMemory(m_HuCardResult,sizeof(m_HuCardResult));

	ZeroMemory(m_SiTongCount,sizeof(m_SiTongCount));
	ZeroMemory(m_WuTongCount,sizeof(m_WuTongCount));

	for (BYTE j=0; j<3; j++)
	{
		for (BYTE i=0; i<8; i++)
		{
			m_WuTongIndex[j][i] = (BYTE)(-1);
			m_SiTongIndex[j][i] = (BYTE)(-1);
		}
	}
	
	ZeroMemory(m_cbSendCardData,sizeof(m_cbSendCardData));
	m_bGameStart = false;

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
//����Ϸ��ʼʱ�����ĵ������У�
//1 ϴ��
//2 ���Ʋ��Ұ���Ӧ����ת��Ϊ�߼�ֵ
//3 �ж��Ƿ��п����ڵ�һ�־����˺���
bool __cdecl CTableFrameSink::OnEventGameStart()
{
	BYTE	cbCardData[GAME_PLAYER][26]; //���淢�����û�������Ϣ
	ZeroMemory(cbCardData,sizeof(cbCardData));

	//����״̬
	m_pITableFrame->SetGameStatus(GS_MJ_PLAY);
	//�����˿�
	m_cbLeftCardCount=CountArray(m_cbRepertoryCard);
	m_GameLogic.RandCardData(m_cbRepertoryCard,CountArray(m_cbRepertoryCard));
	
	//2009.2.20
	//���ƶ�ׯ,����ǵ�һ��ׯ��Ϊ�գ�ÿ���������һ���ƣ��Ƚ��Ƶ��С���Ƶ��������ׯ
	if (m_wBankerUser == INVALID_CHAIR)
	{
		//�ҳ����������һ���Ƶ����
		BYTE cbTempCardData[m_wPlayerCount];
		do{
			//�õ�������
			BYTE cbTempCardRank[m_wPlayerCount];
			ZeroMemory(cbTempCardData,sizeof(cbTempCardData));
			ZeroMemory(cbTempCardRank,sizeof(cbTempCardRank));

			for (BYTE i=0; i<m_wPlayerCount; i++)
			{
				BYTE index = rand()%110;
				cbTempCardData[i] = m_cbRepertoryCard[index];
				cbTempCardRank[i]=m_GameLogic.GetCardRank(m_cbRepertoryCard[index]);
			}

			//�����Ƿ���Ψһ��һ��������
			BYTE index=0,maxCount=1;
			for (BYTE i=1; i<m_wPlayerCount; i++)
			{
				if(cbTempCardRank[i]>cbTempCardRank[index])
				{
					index=i;
					maxCount=1;
				}
				else if(cbTempCardRank[i]==cbTempCardRank[index])
				{
					maxCount++;
				}
			}

			if(maxCount==1)
			{
				m_wBankerUser=index;
				break;
			}
		}while(true);
		
		//����������͸����û�
		for (BYTE i=0; i<m_wPlayerCount; i++)
		{
			CMD_S_DecideBanker DecideBanker;
			DecideBanker.bCardData = cbTempCardData[i];
			DecideBanker.bUserID = i;

			//��������
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_DECIDE_BANKER,&DecideBanker,sizeof(DecideBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_DECIDE_BANKER,&DecideBanker,sizeof(DecideBanker));
		}
	}
	//	//////��������ҷ�25����
	
	//for(BYTE i=0;i<3;i++)
	//{
	//	for(BYTE j=0;j<25;j++)
	//	{
	//		cbCardData[i][j] = m_cbRepertoryCard[--m_cbLeftCardCount];
	//	}
	//}
	for (WORD i=0;i<25;i++)
	{
		cbCardData[0][i] = m_cbRepertoryCard[--m_cbLeftCardCount];
		cbCardData[1][i] = m_cbRepertoryCard[--m_cbLeftCardCount];
		cbCardData[2][i] = m_cbRepertoryCard[--m_cbLeftCardCount];
	}
	//BYTE cbCount=m_cbLeftCardCount;
	//m_cbRepertoryCard[--cbCount]=0x08;
	//m_cbRepertoryCard[--cbCount]=0x08;
	//m_cbRepertoryCard[--cbCount]=0x08;
	//m_cbRepertoryCard[--cbCount]=0x08;
	//m_cbRepertoryCard[--cbCount]=0x08;
	//m_cbRepertoryCard[--cbCount]=0x08;
	//m_cbRepertoryCard[--cbCount]=0x08;
	//m_cbRepertoryCard[--cbCount]=0x08;
	//m_cbRepertoryCard[--cbCount]=0x08;
	//m_cbRepertoryCard[--cbCount]=0x08;
	//m_cbRepertoryCard[--cbCount]=0x08;
	//m_cbRepertoryCard[--cbCount]=0x08;
	//m_cbRepertoryCard[--cbCount]=0x08;
	//m_cbRepertoryCard[--cbCount]=0x08;
	//m_cbRepertoryCard[--cbCount]=0x08;
	//m_cbRepertoryCard[--cbCount]=0x08;
	//m_cbRepertoryCard[--cbCount]=0x08;
	//m_cbRepertoryCard[--cbCount]=0x08;
	//m_cbRepertoryCard[--cbCount]=0x08;
	//m_cbRepertoryCard[--cbCount]=0x08;
	//m_cbRepertoryCard[--cbCount]=0x08;
	////////////Ӳ������ /////////////
		//m_wBankerUser = 2;
		//BYTE temp0[25] = {
		//					0x01,0x11,0x11,
		//					0x02,0x02,0x02,
		//					0x03,0x13,0x13, 
		//					0x04,0x04,0x04,
		//					0x05,0x15,0x15, 
		//					0x06,0x06,0x06,
		//					0x22,0x22,0x22,
		//					0x23,0x24,0x25, 
		//					0x07
		//				};

		//BYTE temp1[25] = {
		//					0x01,0x01,0x23,
		//					0x02,0x02,0x24,
		//					0x08,0x08,0x25, 
		//					0x04,0x04,0x04,
		//					0x29,0x29,0x29, 
		//					0x06,0x06,0x06,
		//					0x2A,0x2A,0x2A,
		//					0x2B,0x2B,0x2B, 
		//					0x02
		//				};

		//BYTE temp2[25] = {
		//					0x0A,0x0A,0x0A,
		//					0x02,0x02,0x02,
		//					0x03,0x13,0x13, 
		//					0x04,0x04,0x04,
		//					0x05,0x15,0x15, 
		//					0x06,0x06,0x06,
		//					0x22,0x22,0x22,
		//					0x26,0x26,0x26, 
		//					0x07
		//				};
	//BYTE temp2[25] = {
	//					0x09,0x09,0x09,0x19,0x19, 
	//					0x24,0x24,0x24,0x24,0x24,
	//					0x25,0x25,0x25,0x25,0x25,
	//					0x26,0x26,0x26,0x26,0x26, 
	//					0x27,0x27,0x27,0x27,0x07
	//                 };

	////��Ӳ������Ʒŵ������û���
	//CopyMemory(cbCardData[0],temp0,25);
	//CopyMemory(cbCardData[1],temp1,25);
	//CopyMemory(cbCardData[2],temp2,25);

		//��25���齫���浽��Ӧ��ҵ�������������
	//	CopyMemory(m_cbCardData[0],temp0,25);
	//	//дһ�������������������еõ�����
	//BYTE count = m_GameLogic.GetHuaCard(m_cbCardData[2],25,m_cbHuaCardIndex[2]);
	//m_GameLogic.SwitchToCardIndex(temp0,25,m_cbCardIndex[2]);

		//�ַ��˿�--��ÿһ���˽�������25���ƽ�������m_cbCardIndex[i]��
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//��25���齫���浽��Ӧ��ҵ�������������
		m_GameLogic.SwitchToCardIndex(cbCardData[i],25,m_cbCardIndex[i]);

		//�������������еõ����Ƹ���
		m_GameLogic.GetHuaCard(cbCardData[i],25,m_cbHuaCardIndex[i]);
	}

	//��ׯ�ҷ���һ����
	m_cbSendCardCount++;
	m_cbSendCardData[0]=m_cbRepertoryCard[--m_cbLeftCardCount];
		
	m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData[0])]++;
	//�����һ���Ʊ��浽ׯ��������������
	cbCardData[m_wBankerUser][25] = m_cbSendCardData[0];

	//���ׯ�����ǻ���,���浽ׯ�һ�������������
	if ((m_cbSendCardData[0] & MASK_COLOR) == 0x10)
	{
		BYTE index = m_GameLogic.SwitchToCardIndex(m_cbSendCardData[0]);
		m_cbHuaCardIndex[m_wBankerUser][index/2]++;
		ASSERT(m_cbHuaCardIndex[m_wBankerUser][index/2] <=2);		
	}

	
	////�������ҵĲ������
	for (WORD i=0; i<m_wPlayerCount; i++)
	{
		tagTongResult SiTongResult;
		tagTongResult WuTongResult;
		ZeroMemory(&SiTongResult,sizeof(SiTongResult));
		ZeroMemory(&WuTongResult,sizeof(WuTongResult));

		m_cbUserAction[i]|=m_GameLogic.AnalyseSiTong(m_cbCardIndex[i],SiTongResult);
		m_cbUserAction[i]|=m_GameLogic.AnalyseWuTong(m_cbCardIndex[i],NULL,0,WuTongResult);
	}

	//tagHuCardResult HuCardResult;
	ZeroMemory(m_HuCardResult,sizeof(m_HuCardResult));
	//����ׯ���û��Ƿ���Ժ���
//#ifdef _DEBUG
//	m_GameLogic.m_Debug.BeginPrint();
//#endif

	m_cbUserAction[m_wBankerUser]|=m_GameLogic.AnalyseHuCard(m_cbCardIndex[m_wBankerUser],NULL,0,0,m_cbHuaCardIndex[m_wBankerUser],m_cbHuaWeaveCardIndex[m_wBankerUser],m_HuCardResult[m_wBankerUser]);

//#ifdef _DEBUG
//		m_GameLogic.m_Debug.EndPrint(m_wBankerUser);
//#endif

	//��ʼ����һ�ֵĲ��ƿ��Ʊ���
	ZeroMemory(m_cbFirstUserAction,sizeof(m_cbFirstUserAction));
	ZeroMemory(m_cbFirstUser,sizeof(m_cbFirstUser));
	m_cbFirstCurrentIndex=3;
	m_cbFirstCurrentUser=m_wBankerUser;
	m_cbFirstCurrentAction=WIK_NULL;

	//�õ������û��¼��û������¼��û�
	WORD wBeforeOneUser = (m_wBankerUser+1+m_wPlayerCount)%m_wPlayerCount; //�ϼ�
	WORD wBeforeTwoUser = (m_wBankerUser+2+m_wPlayerCount)%m_wPlayerCount; //���ϼ�

	//��˳��������û���Ϣ
	m_cbFirstUser[0]=m_wBankerUser;
	m_cbFirstUser[1]=wBeforeOneUser;
	m_cbFirstUser[2]=wBeforeTwoUser;
	m_cbFirstUser[3]=m_wBankerUser;

	//��˳��������û��ܽ��еĲ�����Ϣ
	if((m_cbUserAction[m_wBankerUser]&WIK_CHI_HU)!=0)
	{
		m_cbFirstUserAction[0]=WIK_CHI_HU;
	}
	else
	{
		m_cbFirstUserAction[0]=WIK_NULL;
	}

	m_cbFirstUserAction[1]=m_cbUserAction[wBeforeOneUser];
	m_cbFirstUserAction[2]=m_cbUserAction[wBeforeTwoUser];

	if((m_cbUserAction[m_wBankerUser]&WIK_SI_TONG)!=0)	m_cbFirstUserAction[3]|=WIK_SI_TONG;
	if((m_cbUserAction[m_wBankerUser]&WIK_WU_TONG)!=0)	m_cbFirstUserAction[3]|=WIK_WU_TONG;

	//���õ�ǰ�û��������
	for(BYTE i=0;i<4;i++)
	{
		if(m_cbFirstUserAction[i]!=WIK_NULL)
		{
			m_cbFirstCurrentIndex=i;
			m_cbFirstCurrentUser=m_cbFirstUser[i];
			m_cbFirstCurrentAction=m_cbFirstUserAction[i];
			break;
		}
	}
	//�ж���Ϸ�Ƿ�������ʼ--��ǰ�����û�Ϊׯ�ң�����ׯ��û�в��ƵĻ���������Ϸ��ʽ��ʼ
	if(m_cbFirstCurrentIndex==3 && m_cbFirstCurrentAction==WIK_NULL)
	{   
		m_wProvideUser=m_wBankerUser;
		m_cbProvideCard=m_cbSendCardData[0];
		m_bGameStart=true;
		m_wCurrentUser=m_wBankerUser;
	}
	else
	{
		m_wProvideUser=INVALID_CHAIR;
		m_cbProvideCard=m_cbSendCardData[0];
		m_bGameStart = false;
		m_wCurrentUser = INVALID_CHAIR;
	}
////////////////////////////////////////
//CString str;
//IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_cbFirstCurrentUser);
//str.Format("��ǰ�����û���%s",pIServerUserItem->GetAccounts());
//if((m_cbFirstCurrentAction&WIK_SI_TONG)!=0)
//{
//	str.Format("%s  �������Ĳ�",str);
//}
//if((m_cbFirstCurrentAction&WIK_WU_TONG)!=0)
//{
//	str.Format("%s  ���������",str);
//}
//AfxMessageBox(str);
//m_pITableFrame->SendGameMessage(pIServerUserItem,str,SMT_INFO);
///////////////////////////////////////////

	//��������,���Ϳ�ʼ��Ϣ
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));
	GameStart.wBankerUser=m_wBankerUser;

	//�����ܽ��в����ĵ�ǰ�û��������
	GameStart.wCurrentUser=m_cbFirstCurrentUser;
	GameStart.cbCurrentAction=m_cbFirstCurrentAction;
	
	//��ÿ����ҷ�������
	for (BYTE i=0;i<m_wPlayerCount;i++)
	{
		//���ñ���
		CopyMemory(GameStart.cbCardData,cbCardData[i],sizeof(GameStart.cbCardData));

		//�����ׯ��,�����к��Ʋ�����������Ѻ������,������ϵĲ�����Ѿ��ֵ
		if (i == m_wBankerUser && (m_cbUserAction[i] & WIK_CHI_HU) != 0)
		{
			CopyMemory(GameStart.cbCenterCard,m_HuCardResult[m_wBankerUser].AnalyseItem.cbCenterCard,sizeof(GameStart.cbCenterCard));
			CopyMemory(GameStart.cbWeaveKind,m_HuCardResult[m_wBankerUser].AnalyseItem.cbWeaveKind,sizeof(GameStart.cbWeaveKind));
			CopyMemory(GameStart.cbYaKou,m_HuCardResult[m_wBankerUser].AnalyseItem.cbCardEye,sizeof(GameStart.cbYaKou));
			GameStart.cbHuScore = m_HuCardResult[m_wBankerUser].HuScore;
			GameStart.cbGoldCard = m_HuCardResult[m_wBankerUser].bRealGold;
		}

		//��������
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}
		
	return true;
}

//��Ϸ����,��Ϸ������ͳ�ƻ���
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	//wChairID  Ϊ�����û�
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
		{

			//�������
			CMD_S_GameEnd GameEnd;
			BYTE bChiHuUserCount = 0;   //babala 11-30
			CString strChiHu;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//GameEnd.wProvideUse �����û�
			GameEnd.wProvideUser=wChairID;				
			//11_19
			GameEnd.cbChiHuCard=m_cbChiHuCard;

			GameEnd.wChiHuUserCount=1;  //babala 11-30			
			WORD nWinner = INVALID_CHAIR;
		
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if (m_HuCardResult[i].IsHu == true)
				{
					nWinner = i;
					break;
				}
			}
			//�������
			GameEnd.wWinner = nWinner;

			
			//Ӯ�ҷ���
			if (m_wProvideUser != INVALID_CHAIR)
			{
				//����Ӯ�ҵĺ������
			
				CopyMemory(GameEnd.cbCenterCard,m_HuCardResult[nWinner].AnalyseItem.cbCenterCard,sizeof(GameEnd.cbCenterCard));
				CopyMemory(GameEnd.cbWeaveKind,m_HuCardResult[nWinner].AnalyseItem.cbWeaveKind,sizeof(GameEnd.cbWeaveKind));
				CopyMemory(GameEnd.cbYaKou,m_HuCardResult[nWinner].AnalyseItem.cbCardEye,sizeof(GameEnd.cbYaKou));
				
				CopyMemory(GameEnd.cbHuaWeaveCount,m_cbHuaWeaveCardIndex[nWinner],sizeof(GameEnd.cbHuaWeaveCount));
				CopyMemory(GameEnd.cbHuaHandCount,m_cbHuaCardIndex[nWinner],sizeof(GameEnd.cbHuaHandCount));

				bChiHuUserCount = 1; 
			
				if(m_wProvideUser==nWinner)//����
				{
					//AfxMessageBox("����");
					for(BYTE i=0; i<m_wPlayerCount;i++)
					{
						if(i==nWinner) continue;

						//������ʱ������һ���Ӯ����Ӯ�ķ���
						//GameEnd.lGameScore[i] -= m_HuCardResult[nWinner].HuScore;
						GameEnd.lGameScore[i] -= m_HuCardResult[nWinner].HuScore/2;
						GameEnd.lGameScore[nWinner] -= GameEnd.lGameScore[i];
					}
				}
				else//����
				{
					//AfxMessageBox("����");
					GameEnd.lGameScore[m_wProvideUser] -= m_HuCardResult[nWinner].HuScore;
					GameEnd.lGameScore[nWinner] += m_HuCardResult[nWinner].HuScore;
				}
                //�������
				GameEnd.lHuPoint = (LONG)m_HuCardResult[nWinner].bHuPoint;

				GameEnd.bGoldCard = m_GameLogic.SwitchToCardData(m_HuCardResult[nWinner].bRealGold);

			}
			//����
			else
			{
				//AfxMessageBox("����");
				GameEnd.cbChiHuCard=0;
				GameEnd.wChiHuUserCount=0;
				b_HuangZhuang=true;
			}			
			//��ׯ���мҺ��ƻ����֣�ׯ�ҵ���һ����ׯ
			if (m_wBankerUser != nWinner)
			{
				m_wBankerUser = (m_wBankerUser-1 + m_wPlayerCount)%m_wPlayerCount;
			}

			//AfxMessageBox("��������˿�");
			//�����ĸ���ҵ��˿�
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i],m_cbHuaCardIndex[i]);
			}
			//��������
			LONG lCellScore = m_pGameServiceOption->lCellScore;
			//У���ļҷ���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.lGameScore[i] = GameEnd.lGameScore[i]*lCellScore;
			}
			//˰�ռ���,����������½��м���,����е���,�ں�����е��� 
			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
			GameEnd.lTax=0;

			//����˰��
			//��ʼ�����ã���������֣�����Ϊ�;�״̬
			for (WORD i=0; i<GAME_PLAYER; i++)
			{
				ScoreInfo[i].ScoreKind = enScoreKind_Draw;
			}

			//�����������,�϶������䣬����Ӯ���۳���ҵ�˰ ��
			if(b_HuangZhuang == false)
			{
				//˰��ֻ������������Ҽ�Ӯ�ҷ���
				for (WORD i=0;i<GAME_PLAYER;i++)
				{
					//��¼���ҵķ���
					ScoreInfo[i].lScore=GameEnd.lGameScore[i];
					ScoreInfo[i].ScoreKind=(GameEnd.lGameScore[i]>0L)?enScoreKind_Win:enScoreKind_Lost;
					
				}
				//AfxMessageBox("���������");
				CString sz;
				sz.Format("�������Ϊ:%d,%d,%d,%d,%d,%d,%d,%d,",m_HuCardResult[nWinner].AnalyseItem.cbCenterCard[0],m_HuCardResult[nWinner].AnalyseItem.cbCenterCard[1],m_HuCardResult[nWinner].AnalyseItem.cbCenterCard[2],m_HuCardResult[nWinner].AnalyseItem.cbCenterCard[3],m_HuCardResult[nWinner].AnalyseItem.cbCenterCard[4],m_HuCardResult[nWinner].AnalyseItem.cbCenterCard[5],m_HuCardResult[nWinner].AnalyseItem.cbCenterCard[6],m_HuCardResult[nWinner].AnalyseItem.cbCenterCard[7]);
				//AfxMessageBox(sz);
				
			}

			//ͳ�ƻ���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//д�����
				m_pITableFrame->WriteUserScore(i,ScoreInfo[i].lScore,ScoreInfo[i].lRevenue,ScoreInfo[i].ScoreKind);
			}

			GameEnd.bIsQuit=false;
			GameEnd.wChiHuUserCount=bChiHuUserCount;  
		
			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));


			//������Ϸ
			m_pITableFrame->ConcludeGame();

			return true;
		}

	case GER_USER_LEFT:		//�û�ǿ��
		{
			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			//��������
			LONG lCellScore = m_pGameServiceOption->lCellScore;

			//���ñ���
			GameEnd.wProvideUser=INVALID_CHAIR;
			GameEnd.wWinner = INVALID_CHAIR;
			//GameEnd.lGameScore[wChairID]=-(20*lCellScore);
			GameEnd.lGameScore[wChairID]=-(4*lCellScore);
			GameEnd.bIsQuit=true;

			//֪ͨ��Ϣ
			TCHAR szMessage[128]=TEXT("");
			_snprintf(szMessage,CountArray(szMessage),TEXT("�� ( %s ) �뿪��Ϸ����Ϸ����"),pIServerUserItem->GetAccounts());
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
            m_pITableFrame->WriteUserScore(wChairID,ScoreInfo.lScore,0,ScoreInfo.ScoreKind);

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
	case GS_MJ_FREE://����״̬--����ׯ����Ϣ�뱶����Ϣ���͵��ͻ���
		{
			//��������
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));

			//��������
			StatusFree.wBankerUser=m_wBankerUser;
			StatusFree.lCellScore=m_pGameServiceOption->lCellScore;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_MJ_PLAY:	//��Ϸ״̬
	case GS_MJ_HAI_DI:	//����״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//��Ϸ����
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.lCellScore=m_pGameServiceOption->lCellScore;

			//״̬����
			StatusPlay.cbActionCard=m_cbProvideCard;
			StatusPlay.cbLeftCardCount=m_cbLeftCardCount;
			StatusPlay.cbActionMask=(m_bResponse[wChiarID]==false)?m_cbUserAction[wChiarID]:WIK_NULL;

			//�����������к��Ʋ����������������
			if (m_HuCardResult[wChiarID].IsHu == true)
			{
				CopyMemory(StatusPlay.cbCenterCard,m_HuCardResult[wChiarID].AnalyseItem.cbCenterCard,sizeof(m_HuCardResult[wChiarID].AnalyseItem.cbCenterCard));
				CopyMemory(StatusPlay.cbWeaveKind,m_HuCardResult[wChiarID].AnalyseItem.cbWeaveKind,sizeof(m_HuCardResult[wChiarID].AnalyseItem.cbWeaveKind));
				CopyMemory(StatusPlay.cbYaKou,m_HuCardResult[wChiarID].AnalyseItem.cbCardEye,sizeof(m_HuCardResult[wChiarID].AnalyseItem.cbCardEye));
				StatusPlay.cbHuScore = m_HuCardResult[wChiarID].HuScore;
				StatusPlay.cbGoldCard = m_HuCardResult[wChiarID].bRealGold;
			}

			//��ʷ��¼
			StatusPlay.wOutCardUser=m_wOutCardUser;
			StatusPlay.cbOutCardData=m_cbOutCardData;
			CopyMemory(StatusPlay.cbDiscardCard,m_cbDiscardCard,sizeof(StatusPlay.cbDiscardCard));
			CopyMemory(StatusPlay.cbDiscardCount,m_cbDiscardCount,sizeof(StatusPlay.cbDiscardCount));

			//����˿�
			CopyMemory(StatusPlay.WeaveItemArray,m_WeaveItemArray,sizeof(m_WeaveItemArray));
			CopyMemory(StatusPlay.cbWeaveCount,m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));
			CopyMemory(StatusPlay.cbHuaWeaveCardIndex,m_cbHuaWeaveCardIndex,sizeof(m_cbHuaWeaveCardIndex));

			//ת���˿�����
			StatusPlay.cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[wChiarID],StatusPlay.cbCardData,m_cbHuaCardIndex[wChiarID]);

			//�õ���������ҵ��˿˸���
			for (BYTE i=0; i<GAME_PLAYER; i++)
			{
				//ͳ���Ƶ�����
				for (BYTE j=0; j<MAX_INDEX; j++) StatusPlay.cbUserCardCount[i] += m_cbCardIndex[i][j];
			}

			//��Ϸ��ʼ��־
			StatusPlay.bGameStart = m_bGameStart;
			if(m_bGameStart==false)
			{
				StatusPlay.wFirstCurrentUser=m_cbFirstCurrentUser;
				StatusPlay.cbFirstCurrentAction=m_cbFirstCurrentAction;
			}

			//���ƴ���
			StatusPlay.bSiTongCount = m_SiTongCount[wChiarID];
			StatusPlay.bWuTongCount = m_WuTongCount[wChiarID];
			CopyMemory(StatusPlay.bSiTongIndex,m_SiTongIndex[wChiarID],sizeof(m_SiTongIndex[wChiarID]));
			CopyMemory(StatusPlay.bWuTongIndex,m_WuTongIndex[wChiarID],sizeof(m_WuTongIndex[wChiarID]));

			//��ʣ���Ƶ�����Ŀ
			StatusPlay.cbLeftCardCount=m_cbLeftCardCount;

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
	case SUB_S_ACTION:
		{
			return OnEventGameStart();			
		}
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
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

	//�������
	ASSERT(wChairID==m_wCurrentUser);
	ASSERT(m_GameLogic.IsValidCard(cbCardData)==true);

	//Ч�����
	if (wChairID!=m_wCurrentUser) return false;
	if (m_GameLogic.IsValidCard(cbCardData)==false) return false;

	//�õ���������
	BYTE index = m_GameLogic.SwitchToCardIndex(cbCardData);

	//����ǻ��ƣ��������������ж�Ӧλ�õ�������1
	if ( (cbCardData & MASK_COLOR) == 0x10 )
	{
		//���Ըû��Ƹ�����Ϊ0
		ASSERT(m_cbHuaCardIndex[wChairID][index/2] != 0);
		if (m_cbHuaCardIndex[wChairID][index/2] == 0) return false;
		
		//��������λ�ü�1
		m_cbHuaCardIndex[wChairID][index/2]--;
	}

	//�ӳ����������,ɾ���������˿�
	ASSERT(m_cbCardIndex[wChairID][index] >= 1);
	if (m_cbCardIndex[wChairID][index] <1) return false;
	m_cbCardIndex[wChairID][index]--;

#ifdef _DEBUG
	m_GameLogic.m_Debug.BeginPrint();
	m_GameLogic.m_Debug.PrintCardMessage(m_cbCardIndex[wChairID]);
	m_GameLogic.m_Debug.EndPrint(wChairID);
#endif
	
	//���ñ���
	m_bSendStatus=true;      //-------????????????
	m_cbUserAction[wChairID]=WIK_NULL;	 //�û�����
	m_cbPerformAction[wChairID]=WIK_NULL;//m_cbPerformAction ִ�ж���--������û�����ִ�еĲ���

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
	//�л����¼��û�
	m_wCurrentUser=(wChairID+m_wPlayerCount-1)%m_wPlayerCount;
	m_bSendCount = 1;

	//��Ӧ�жϣ�����û�������һ���ƣ��ж������û��Ƿ���Ҫ���ƣ�EstimatKind_OutCardֻ�����������ж�
	//�����ǰ�û��Լ� �����ƣ������Լ����Լ����з����ԣ�����
	bool bAroseAction=EstimateUserRespond(wChairID,cbCardData);
 
	//�ɷ��˿�
	if (bAroseAction==false) DispatchCardData(m_wCurrentUser);

	return true;
}

//�û�����
bool CTableFrameSink::OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard)
{
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

	//������֮ǰ��ӡ������Ϣ
//#ifdef _DEBUG
//	m_GameLogic.m_Debug.BeginPrint();
//	m_GameLogic.m_Debug.PrintCardMessage(m_cbCardIndex[wChairID]);
//	m_GameLogic.m_Debug.PrintWeaveItemsMessage(m_WeaveItemArray[wChairID],m_cbWeaveItemCount[wChairID]);
//	m_GameLogic.m_Debug.EndPrint(wChairID);
//#endif

    //���ж�����ͬʱ����������������ߵ��û�ִ���˲���������û�з��������û���
	//����ʱ�����û�Ҳ��������������ʱ��Ҫ������˵���
    // ���ǵ�һ�ֵĲ���                �����û������������                ���и߼�����û�ִ���˲���
	if((m_cbSendCardCount != 1) && (wChairID != m_wCurrentUser) && (m_wCurrentUser != INVALID_CHAIR))
	{   //  ���û���ǰû�в�����                   ����߼�����û��Ѳ�����
		if((m_bResponse[wChairID] == false) && (m_bResponse[m_wCurrentUser] == true))
		{
			return true;
		}
	}

	//Ч���û�,�ܽ���һ����Ҫô���û�����Ӧ����,Ҫô����������,����һ���ǵ�һ�ַ���ʱ������ͳ
	ASSERT((wChairID==m_wCurrentUser)||(m_wCurrentUser==INVALID_CHAIR) || (m_cbSendCardCount == 1));
	if ((wChairID!=m_wCurrentUser)&&(m_wCurrentUser!=INVALID_CHAIR)&&(m_cbSendCardCount != 1)) return false;

	//1������ǵ�һ�ֵĲ���:����ͳ����ͳ������ͳ�Ʋ���
	if (m_cbSendCardCount == 1 && m_bGameStart == false)
	{
		//����մ��û��Ĳ�����Ϣ
		m_cbFirstUserAction[m_cbFirstCurrentIndex]=WIK_NULL;
		//���ݲ������ദ��:
		switch (cbOperateCode)
		{
		case WIK_SI_TONG:        //��ͳ����
			{
				//��¼�����ͳ�Ĵ���,���ص���
				BYTE index = m_SiTongCount[wChairID]++;
				m_SiTongIndex[wChairID][index]=m_GameLogic.SwitchToCardIndex(cbOperateCard);
				
				//�Ĳز���һ����
				m_bSendCount = 1;
				
				break;
			}
		case WIK_WU_TONG:        //��ͳ����
			{
				BYTE index = m_WuTongCount[wChairID]++;
				m_WuTongIndex[wChairID][index]=m_GameLogic.SwitchToCardIndex(cbOperateCard);

				//��ز���������
				m_bSendCount = 2;

				//����������������,ǰ�������Ѿ��ع�,��ʱֻ�ܸ��û���һ����
				for (BYTE i=0; i<8; i++)
				{
					if ( m_GameLogic.SwitchToCardIndex(cbOperateCard) == m_SiTongIndex[wChairID][i] )
					{
						//�������Ĳ����ݸ��Ƶ�ǰ����
						for(BYTE j=i;j<m_SiTongCount[wChairID];j++)
						{
							m_SiTongIndex[wChairID][j]=m_SiTongIndex[wChairID][j+1];
						}

						//�޸Ĵ��û��ĲصĴ���
						m_SiTongCount[wChairID]--;
						m_SiTongIndex[wChairID][m_SiTongCount[wChairID]]=(BYTE)(-1);

						//��ʱӦ��ֻ�ܸ��û���һ����
						m_bSendCount = 1;
						break;
					}
				}
				break;
			}
		case WIK_TONG_GIVE_UP:   //���������
			{
				break;
			}
		case WIK_CHI_HU:         //��һ������º���,ֻ��ׯ���������ƵĿ�����
			{
				ASSERT(wChairID == m_wBankerUser);
				if (wChairID != m_wBankerUser) return false;

				//����պ��Ƽ�¼��Ϣ�������汾�κ����û��ĺ�����Ϣ
				ZeroMemory(m_HuCardResult,sizeof(m_HuCardResult));

				BYTE cbWeaveItemCount=m_cbWeaveItemCount[wChairID];
				tagWeaveItem * pWeaveItem=m_WeaveItemArray[wChairID];
//#ifdef _DEBUG
//					m_GameLogic.m_Debug.BeginPrint();
//#endif
				m_GameLogic.AnalyseHuCard(m_cbCardIndex[wChairID],pWeaveItem,cbWeaveItemCount,0,m_cbHuaCardIndex[wChairID],m_cbHuaWeaveCardIndex[wChairID],m_HuCardResult[wChairID]);
//#ifdef _DEBUG
//					m_GameLogic.m_Debug.EndPrint(wTargetUser);
//#endif
				m_wProvideUser=wChairID;	
				m_cbChiHuCard=m_cbProvideCard;
				ASSERT(m_HuCardResult[wChairID].IsHu == true);
				if (m_HuCardResult[wChairID].IsHu == false) return false;

				OnEventGameEnd(m_wProvideUser,NULL,GER_NORMAL);
				
				return true;
			}
		}

		//�����ͳ�Ʋ�����������û�����
		if ((cbOperateCode & WIK_SI_TONG)!=0 || (cbOperateCode & WIK_WU_TONG) != 0)
		{
			m_bSendStatus=true;
			DispatchCardData(wChairID);
		}

		//������һ�ֲ��Ʋ����Ŀ�����Ϣ
		for(BYTE i=m_cbFirstCurrentIndex;i<4;i++)
		{
			if(m_cbFirstUserAction[i]!=WIK_NULL)
			{
				m_cbFirstCurrentIndex=i;

				m_cbFirstCurrentUser=m_cbFirstUser[m_cbFirstCurrentIndex];
				m_cbFirstCurrentAction=m_cbFirstUserAction[m_cbFirstCurrentIndex];
				break;
			}
		}
			
		if(i==4)
		{
			m_cbFirstCurrentIndex=3;
			m_cbFirstCurrentUser=m_cbFirstUser[3];
			m_cbFirstCurrentAction=m_cbFirstUserAction[3];
		}

		//�ж���Ϸ�Ƿ�������ʼ
		if(m_cbFirstCurrentIndex==3 && m_cbFirstCurrentAction==WIK_NULL)
		{
			m_bGameStart = true;
			m_wCurrentUser = m_wBankerUser;
		}
		else
		{
			m_bGameStart = false;
			m_wCurrentUser = INVALID_CHAIR;
		}
////////////////////////////////////////
//CString str;
//IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_cbFirstCurrentUser);
//str.Format("��ǰ�����û���%s",pIServerUserItem->GetAccounts());
//if((m_cbFirstCurrentAction&WIK_SI_TONG)!=0)
//{
//	str.Format("%s  �������Ĳ�",str);
//}
//if((m_cbFirstCurrentAction&WIK_WU_TONG)!=0)
//{
//	str.Format("%s  ���������",str);
//}
//AfxMessageBox(str);
//m_pITableFrame->SendGameMessage(pIServerUserItem,str,SMT_INFO);
///////////////////////////////////////////
		//������
		CMD_S_OperateResult OperateResult;
		OperateResult.wOperateUser=wChairID;
		OperateResult.cbOperateCard=cbOperateCard;
		OperateResult.cbOperateCode=cbOperateCode;
		OperateResult.wProvideUser=wChairID;
		OperateResult.cbHuaCount = 0;

		//�����ܽ��в����ĵ�ǰ�û��������
		OperateResult.wCurrentUser=m_cbFirstCurrentUser;
		OperateResult.cbCurrentAction=m_cbFirstCurrentAction;

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

		return true;
	}

	//2�����˴�������Ĳ���������,����(���л�����)
	if (m_wCurrentUser==INVALID_CHAIR)
	{
		//Ч��״̬
		ASSERT(m_bResponse[wChairID]==false);
		ASSERT((cbOperateCode==WIK_NULL)||(cbOperateCode==WIK_TONG_GIVE_UP)||((m_cbUserAction[wChairID]&cbOperateCode)!=0));
		
		//Ч��״̬	//m_bResponse[]��Ӧ��־--������û��Ƿ���Ӧ���˲���
		if (m_bResponse[wChairID]==true) return false;
		if ((cbOperateCode!=WIK_NULL)&& (cbOperateCode!=WIK_TONG_GIVE_UP) &&((m_cbUserAction[wChairID]&cbOperateCode)==0)) return false;

		//��������
		//WORD wTargetUser=wChairID;
		WORD wTargetUser=wChairID; 
		BYTE cbTargetAction=cbOperateCode;

		//���´��û���������ر���
		m_bResponse[wChairID]=true; //��Ӧ��־--������û��Ƿ���Ӧ���˲���
		m_cbPerformAction[wChairID]=cbOperateCode; //ִ�ж���--������û�����ִ�еĲ���
		m_cbOperateCard[wChairID]=(cbOperateCard==0)?m_cbProvideCard:cbOperateCard;

		//2009.03.02�޸�
		//�Ǳ��˴���ƣ��������Ҳ��������

		//�õ������û��¼��û������¼��û�
		WORD wNextOneUser = (m_wProvideUser-1+m_wPlayerCount)%m_wPlayerCount; //�¼�
		WORD wNextTwoUser = (m_wProvideUser-2+m_wPlayerCount)%m_wPlayerCount; //���¼�

		//�������Ҷ��в���ʱ���¼Ҿ��в������ȼ���
		if((m_cbUserAction[wNextOneUser]!= WIK_NULL) && (m_cbUserAction[wNextTwoUser]!= WIK_NULL))
		{
			//�õ��¼��û������¼��û��Դ��ƵĲ���
			//m_bResponse[] ��Ӧ��־--������û��Ƿ���Ӧ���˲���    //m_cbUserAction[3];�û�����   //ִ�ж���--������û�����ִ�еĲ���
			BYTE cbNextOneUserAction=(m_bResponse[wNextOneUser]==false)?m_cbUserAction[wNextOneUser]:m_cbPerformAction[wNextOneUser];
			BYTE cbNextTwoUserAction=(m_bResponse[wNextTwoUser]==false)?m_cbUserAction[wNextTwoUser]:m_cbPerformAction[wNextTwoUser];
		
			//�õ��¼��û������¼��û��Դ��ƵĲ�������
			BYTE cbNextOneUserRank = m_GameLogic.GetUserActionRank(cbNextOneUserAction);
			BYTE cbNextTwoUserRank = m_GameLogic.GetUserActionRank(cbNextTwoUserAction);

			//�Ƚ����ߵĲ������������ͬ�Ĵ���
			//1���¼��û��Ĳ���������ڻ�������¼��û��Ĳ�������ʱ�����¼��û����Ȳ���
			if(cbNextOneUserRank >= cbNextTwoUserRank)
			{
				//AfxMessageBox("���¼ҵĲ�����������¼Ҵ�ʱ���Ƿ���������");
				//���¼��û�����Ӧ���ƵĻ�����ͳϵִ���¼��û��Ĳ���
				
				if(m_bResponse[wNextOneUser]==true)
				{
					wTargetUser = wNextOneUser;
					cbTargetAction = m_cbPerformAction[wNextOneUser];
				}	
				else
				{					
					return true;									
				}
			}
			//���������¼��û���������
			else
			{
				//�����¼��û�����Ӧ���ƵĻ�����ͳϵִ�����¼��û��Ĳ���
				if(m_bResponse[wNextTwoUser]==true)
				{
					wTargetUser = wNextTwoUser;
					cbTargetAction = m_cbPerformAction[wNextTwoUser];					
				}			
				else
				{
					return true;					
				}
			}		
		}

		BYTE cbTargetCard=(cbOperateCard==0)?m_cbProvideCard:cbOperateCard;
		m_bSendStatus=true;

		//�����û���������ʱ������ձ��ֲ�������صı��������¼ҷ�����
		if (cbTargetAction==WIK_NULL)
		{
			//�û�״̬
			ZeroMemory(m_bResponse,sizeof(m_bResponse));
			ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
			ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
			ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

			DispatchCardData(m_wResumeUser);

			return true;
		}

		//ִ�к��Ʋ���
		if (cbTargetAction==WIK_CHI_HU)
		{
				//������Ϣ
			m_cbChiHuCard=cbTargetCard;
			
			//��ͨ���ƣ����˵���--�˴�һ���ܳ���
			if (m_cbChiHuCard!=0)
			{
				//����պ��Ƽ�¼��Ϣ�������汾�κ����û��ĺ�����Ϣ
				ZeroMemory(m_HuCardResult,sizeof(m_HuCardResult));

				//�����ж�
				BYTE cbWeaveItemCount=m_cbWeaveItemCount[wTargetUser];
				tagWeaveItem * pWeaveItem=m_WeaveItemArray[wTargetUser];
			
				//�����û��Ƿ���Ժ���,�������,������Ϸ,����"ը��"�۲�����ҷ���,������Ϸ
//				//�������ƺ���û��....����.,,,,
//#ifdef _DEBUG
//		        m_GameLogic.m_Debug.BeginPrint();
//#endif
				//�õ�һ����ѵĺ��ƽ��,���浽��ҵĺ�����Ϣ��,�����Ƿ���Ժ���
				m_GameLogic.AnalyseHuCard(m_cbCardIndex[wTargetUser],pWeaveItem,cbWeaveItemCount,m_cbChiHuCard,m_cbHuaCardIndex[wTargetUser],m_cbHuaWeaveCardIndex[wTargetUser],m_HuCardResult[wTargetUser]);
//#ifdef _DEBUG
//		        m_GameLogic.m_Debug.EndPrint(wTargetUser);
//#endif
				ASSERT(m_HuCardResult[wTargetUser].IsHu == true);
				if (m_HuCardResult[wTargetUser].IsHu == false)  return false;

				//�����˿�
				if (m_HuCardResult[wTargetUser].IsHu == true) 
				{
					BYTE index = m_GameLogic.SwitchToCardIndex(m_cbChiHuCard);
					m_cbCardIndex[wTargetUser][index]++;

					//����ǻ���,�����Ʊ��浽��һ���������
					if((m_cbChiHuCard & 0xf0)==0x10)
					{
						m_cbHuaCardIndex[wTargetUser][index/2]++;
					}
				}
			}	
			OnEventGameEnd(m_wProvideUser,NULL,GER_NORMAL);
			return true;
		}
		//���������Ϣ
		m_cbOutCardData=0;
		m_wOutCardUser=INVALID_CHAIR;


		//�û�״̬
		ZeroMemory(m_bResponse,sizeof(m_bResponse));
		ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
		ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
		ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

		//����ִ�зǺ��Ƶ���������
		ASSERT(m_cbWeaveItemCount[wTargetUser]<9);
		WORD wIndex=m_cbWeaveItemCount[wTargetUser]++;
		m_WeaveItemArray[wTargetUser][wIndex].cbPublicCard=TRUE;
		m_WeaveItemArray[wTargetUser][wIndex].cbCenterCard=cbTargetCard;
		m_WeaveItemArray[wTargetUser][wIndex].cbWeaveKind=cbTargetAction;
		m_WeaveItemArray[wTargetUser][wIndex].wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;

		BYTE cbHuaCard = 0;

		//ɾ���˿�
		switch (cbTargetAction)
		{
		case WIK_DUI:		//���Ʋ���
			{
				BYTE index = m_GameLogic.SwitchToCardIndex(cbTargetCard);

				//���Զ���ʱ,�û���λ��������������
				ASSERT(m_cbCardIndex[wTargetUser][index] >= 2);
				if (m_cbCardIndex[wTargetUser][index] < 2) return false;

				//ɾ���˿�,�������������ת��������,ɾ�������������Ƶĸ���
				m_cbCardIndex[wTargetUser][index] -= 2;

				//����Ǿ���
				if (index == 0 || index == 2 || index == 4 || index == 6 || index == 8)
				{
					//�ж�������������Ƿ��ǻ���,���������������ϻ�������1
					if((cbTargetCard & 0xF0) == 0x10)
					{
						m_cbHuaWeaveCardIndex[wTargetUser][index/2]++;
						ASSERT(m_cbHuaWeaveCardIndex[wTargetUser][index/2]<=2);
						cbHuaCard++;
					}
					//�ж�������еĻ����Ƿ�Ϊ0�������Ϊ0��ȫ���û���ȥ�Ա��˵���
					if (m_cbHuaCardIndex[wTargetUser][index/2] > 0) 
					{
						cbHuaCard += m_cbHuaCardIndex[wTargetUser][index/2];
						m_cbHuaWeaveCardIndex[wTargetUser][index/2] += m_cbHuaCardIndex[wTargetUser][index/2];
						m_cbHuaCardIndex[wTargetUser][index/2] = 0;
					}
				}
				break;
			}
		case WIK_SAN_ZHAO:		//���Ʋ���
			{
				BYTE index = m_GameLogic.SwitchToCardIndex(cbTargetCard);

				//���Զ���ʱ,�û���λ��������������
				ASSERT(m_cbCardIndex[wTargetUser][index] >= 3);
				if (m_cbCardIndex[wTargetUser][index] < 3) return false;

				m_bSendCount = 1;
				//ɾ���˿�,�������������ת��������,ɾ�������������Ƶĸ���
				m_cbCardIndex[wTargetUser][index] -= 3;

				//����Ǿ���
				if (index == 0 || index == 2 || index == 4 || index == 6 || index == 8)
				{
					//�ж�������������Ƿ��ǻ���,���������������ϻ�������1
					if((cbTargetCard & 0xF0) == 0x10)
					{
						m_cbHuaWeaveCardIndex[wTargetUser][index/2]++;
						ASSERT(m_cbHuaWeaveCardIndex[wTargetUser][index/2]<=2);
						cbHuaCard++;
					}

					//�ж�������еĻ����Ƿ�Ϊ0�������Ϊ0��ȫ���û���ȥ�Ա��˵���
					if (m_cbHuaCardIndex[wTargetUser][index/2] > 0) 
					{
						cbHuaCard += m_cbHuaCardIndex[wTargetUser][index/2];
						m_cbHuaWeaveCardIndex[wTargetUser][index/2] += m_cbHuaCardIndex[wTargetUser][index/2];
						m_cbHuaCardIndex[wTargetUser][index/2] = 0;
					}
				}
				break;
			}
		case WIK_SI_ZHAO:		//���Ʋ���
			{
				BYTE index = m_GameLogic.SwitchToCardIndex(cbTargetCard);

				//��������ʱ,�û���λ��������������
				ASSERT(m_cbCardIndex[wTargetUser][index] >= 4);
				if (m_cbCardIndex[wTargetUser][index] < 4) return false;

				//ɾ���˿�,�������������ת��������,ɾ�������������Ƶĸ���
				m_cbCardIndex[wTargetUser][index] -= 4;
				m_bSendCount = 2;

				//������ý��Ĳ��Ʋ�ȥ�б��˵��ƣ���ǰ��Ҳ�������1��ԭ�����ص�����Ϊ0
				for (BYTE i=0; i<8;i++)
				{
					if (m_GameLogic.SwitchToCardIndex(cbTargetCard) == m_SiTongIndex[wTargetUser][i])
					{			
					    //2009.03.02�޸�
						//�������Ĳ����ݸ��Ƶ�ǰ����
						for(BYTE j=i;j<m_SiTongCount[wTargetUser];j++)
						{
							m_SiTongIndex[wTargetUser][j]=m_SiTongIndex[wTargetUser][j+1];
						}

						//�޸Ĵ��û��ĲصĴ���
						m_SiTongCount[wTargetUser]--;
						m_SiTongIndex[wTargetUser][m_SiTongCount[wTargetUser]]=(BYTE)(-1);

						//��ʱӦ��ֻ�ܸ��û���һ����
						m_bSendCount = 1;
						break;
					}
				}
				//����Ǿ���
				if (index == 0 || index == 2 || index == 4 || index == 6 || index == 8)
				{
					//�ж�������������Ƿ��ǻ���,���������������ϻ�������1
					if((cbTargetCard & 0xF0) == 0x10)
					{
						m_cbHuaWeaveCardIndex[wTargetUser][index/2]++;
						ASSERT(m_cbHuaWeaveCardIndex[wTargetUser][index/2]<=2);
						cbHuaCard++;
					}

					//�ж�������еĻ����Ƿ�Ϊ0�������Ϊ0��ȫ���û���ȥ�Ա��˵���
					if (m_cbHuaCardIndex[wTargetUser][index/2] > 0) 
					{
						cbHuaCard += m_cbHuaCardIndex[wTargetUser][index/2];
						m_cbHuaWeaveCardIndex[wTargetUser][index/2] += m_cbHuaCardIndex[wTargetUser][index/2];
						m_cbHuaCardIndex[wTargetUser][index/2] = 0;
					}
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
		OperateResult.cbHuaCount = cbHuaCard;

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

#ifdef _DEBUG
	m_GameLogic.m_Debug.BeginPrint();
	m_GameLogic.m_Debug.PrintCardMessage(m_cbCardIndex[wChairID]);
	m_GameLogic.m_Debug.PrintWeaveItemsMessage(m_WeaveItemArray[wChairID],m_cbWeaveItemCount[wChairID]);
	m_GameLogic.m_Debug.EndPrint(wChairID);
#endif

		//�����û�
		m_wCurrentUser=wTargetUser;

		//�������û������������ƣ�������û�����,������ɣ�����
		if ((cbTargetAction==WIK_SAN_ZHAO) || (cbTargetAction==WIK_SI_ZHAO)) 
		{
			DispatchCardData(wTargetUser);
			return true;
		}

		//����Ƕ��ƣ������ƣ��ͻ����������Ʋ����û��ĳ��ƶ�ʱ��
		return true;
	}

	//3���û�����ʱ�Ĳ������������ơ��Ĳأ������ţ�,��أ������ţ�
	if (m_wCurrentUser==wChairID)
	{
		//Ч�����--Ҫô������Ϊ�գ�Ҫô����������ʾ�Ĳ�������ͬ
		ASSERT((cbOperateCode==WIK_NULL) || ((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID] & cbOperateCode)!=0)));
		if ((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)==0)) return false;

		//�û�״̬
		ZeroMemory(m_bResponse,sizeof(m_bResponse));
		ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
		ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
		ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

		//���ñ���
		m_bSendStatus=true;
		m_cbUserAction[wChairID]=WIK_NULL;
		m_cbPerformAction[wChairID]=WIK_NULL;

		//ִ�ж���
		switch (cbOperateCode)
		{
		case WIK_SI_TONG:        //�Ĳز���
			{
				//��¼�����ͳ�Ĵ���,��ͳ����
				BYTE index = m_SiTongCount[wChairID]++;
				m_SiTongIndex[wChairID][index]=m_GameLogic.SwitchToCardIndex(cbOperateCard);

				m_bSendCount = 1;
				break;
			}
		case WIK_WU_TONG:        //��ز���
			{
				//��������
				BYTE cbWeaveIndex=0xFF;
				BYTE cbCardIndex=m_GameLogic.SwitchToCardIndex(cbOperateCard);
				m_bSendCount = 2;

				//��ϲ���--�����û����н�һ����ʱ��һ����ʹ��ԭ���е���Ͻ��в��ƣ���ʱԭ������ϱ�����ڵ��������
				if (m_cbCardIndex[wChairID][cbCardIndex]==1)
				{
					//Ѱ������������У��ж���������Ƿ�������һ����
					for (BYTE i=0;i<m_cbWeaveItemCount[wChairID];i++)
					{
						BYTE cbWeaveKind=m_WeaveItemArray[wChairID][i].cbWeaveKind;
						BYTE cbCenterCardIndex = m_GameLogic.SwitchToCardIndex(m_WeaveItemArray[wChairID][i].cbCenterCard);

						if ((cbCenterCardIndex == cbCardIndex)&&(cbWeaveKind==WIK_SAN_ZHAO))
						{
							cbWeaveIndex=i;
							break;
						}
					}
					//Ч�鶯��
					ASSERT(cbWeaveIndex!=0xFF);
					if (cbWeaveIndex==0xFF) return false;

					//������������Ҿ���Ϊ��ͳ��������۰�
					m_bSendCount = 1;

					//����״̬�½�������һ���Ʒŵ����µ���������ȥ,�޸������
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=TRUE;
					m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=WIK_SI_ZHAO;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard;

					//���ƴ�����������������������1��3��5��7��9�������еĻ��Ƹ�����Ϊ0
					if (cbCardIndex == 0 || cbCardIndex == 2 || cbCardIndex == 4 || cbCardIndex == 6 || cbCardIndex == 8)
					{
						//������л��Ƹ���Ϊ2
						m_cbHuaWeaveCardIndex[wChairID][cbCardIndex/2] = 2;

						//���л��Ƹ���Ϊ0
						m_cbHuaCardIndex[wChairID][cbCardIndex/2] = 0;
					}
					//���иþ��Ƹ���Ϊ0
					m_cbCardIndex[wChairID][cbCardIndex] = 0;						
				}
				//���������
				else
				{
					//�����������ǲ����Ѿ��Ĳ��ˣ���������������Ѿ��Ĳص��ƣ������ƶ�Ӧ���Ĳش�����1����ش�����1
					//���Ҵ��������ֻ�ܸ��û���һ�Ų���
					m_bSendCount = 2;

					//����������������,ǰ�������Ѿ��ع�,��ʱֻ�ܸ��û���һ����
					for (BYTE i=0; i<8; i++)
					{
						if ( m_GameLogic.SwitchToCardIndex(cbOperateCard) == m_SiTongIndex[wChairID][i] )
						{
							//2009.03.02�޸�
							//�������Ĳ����ݸ��Ƶ�ǰ����
							for(BYTE j=i;j<m_SiTongCount[wChairID];j++)
							{
								m_SiTongIndex[wChairID][j]=m_SiTongIndex[wChairID][j+1];
							}

							//�޸Ĵ��û��ĲصĴ���
							m_SiTongCount[wChairID]--;
							m_SiTongIndex[wChairID][m_SiTongCount[wChairID]]=(BYTE)(-1);

							//��ʱӦ��ֻ�ܸ��û���һ����
							m_bSendCount = 1;
							break;
						}
					}
					//��¼�����ͳ�Ĵ���
					BYTE index = m_WuTongCount[wChairID]++;
					m_WuTongIndex[wChairID][index]=m_GameLogic.SwitchToCardIndex(cbOperateCard);
				}
				break;
			}
		
		case WIK_CHI_HU:		 //�Ժ�����
			{
				//����պ��Ƽ�¼��Ϣ�������汾�κ����û��ĺ�����Ϣ
				ZeroMemory(m_HuCardResult,sizeof(m_HuCardResult));

				//��ͨ����
				BYTE cbWeaveItemCount=m_cbWeaveItemCount[wChairID];
				tagWeaveItem * pWeaveItem=m_WeaveItemArray[wChairID];
//
//#ifdef _DEBUG
//		        m_GameLogic.m_Debug.BeginPrint();
//#endif
				//�õ�һ����ѵĺ��ƽ��,���浽��ҵĺ�����Ϣ��,�����Ƿ���Ժ���
				m_GameLogic.AnalyseHuCard(m_cbCardIndex[wChairID],pWeaveItem,cbWeaveItemCount,0,m_cbHuaCardIndex[wChairID],m_cbHuaWeaveCardIndex[wChairID],m_HuCardResult[wChairID]);
//#ifdef _DEBUG
//		        m_GameLogic.m_Debug.EndPrint(wChairID);
//#endif
				ASSERT(m_HuCardResult[wChairID].IsHu == true);
				if (m_HuCardResult[wChairID].IsHu == false) return false;

				//����û����Ժ���,������Ϸ
				m_wProvideUser=wChairID;	
				//������Ϣ,���� ����ʱ,��������������Ϊ0
				m_cbChiHuCard=m_cbProvideCard;

				OnEventGameEnd(m_wProvideUser,NULL,GER_NORMAL);

				return true;
			}
		}

		//������
		CMD_S_OperateResult OperateResult;
		OperateResult.wOperateUser=wChairID;
		OperateResult.cbOperateCard=cbOperateCard;
		OperateResult.cbOperateCode=cbOperateCode;

		OperateResult.wProvideUser=m_wCurrentUser;
		OperateResult.cbHuaCount = 0;
		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

#ifdef _DEBUG
	m_GameLogic.m_Debug.BeginPrint();
	m_GameLogic.m_Debug.PrintCardMessage(m_cbCardIndex[wChairID]);
	m_GameLogic.m_Debug.PrintWeaveItemsMessage(m_WeaveItemArray[wChairID],m_cbWeaveItemCount[wChairID]);
	m_GameLogic.m_Debug.EndPrint(wChairID);
#endif
		//����ͳ��������ͳ����ҷ���
		if ((cbOperateCode & WIK_SI_TONG)!=0 || (cbOperateCode & WIK_WU_TONG) != 0)
		{
			m_bSendStatus = true;
			
			//��Ϸ�з�ͳ�ƣ���ͳ���û�����
			DispatchCardData(wChairID);
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
			ZeroMemory(&OperateNotify,sizeof(OperateNotify));

			OperateNotify.wResumeUser=m_wResumeUser;
			OperateNotify.cbActionCard=m_cbProvideCard;
			OperateNotify.cbActionMask=m_cbUserAction[i];

			//11-29
			//�����������
			if((m_cbUserAction[i] & WIK_CHI_HU) != 0)
			{
				CopyMemory(OperateNotify.cbCenterCard,m_HuCardResult[i].AnalyseItem.cbCenterCard,sizeof(m_HuCardResult[i].AnalyseItem.cbCenterCard));
				CopyMemory(OperateNotify.cbWeaveKind,m_HuCardResult[i].AnalyseItem.cbWeaveKind,sizeof(m_HuCardResult[i].AnalyseItem.cbWeaveKind));
				CopyMemory(OperateNotify.cbYaKou,m_HuCardResult[i].AnalyseItem.cbCardEye,sizeof(m_HuCardResult[i].AnalyseItem.cbCardEye));
				OperateNotify.cbHuScore = m_HuCardResult[i].HuScore;
				OperateNotify.cbGoldCard = m_HuCardResult[i].bRealGold;
			}

			//��������
			m_pITableFrame->SendTableData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
			m_pITableFrame->SendLookonData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
		}
	}
	return true;
}

//��������Ƿ���Բ��ƣ�������Է��ز�������
BYTE CTableFrameSink:: EstimateTongCard(WORD wChairID)
{
	BYTE bUserAction = WIK_NULL;
	
	//������ҵĲ������
	tagTongResult SiTongResult;
	tagTongResult WuTongResult;
	ZeroMemory(&SiTongResult,sizeof(SiTongResult));
	ZeroMemory(&WuTongResult,sizeof(WuTongResult));

	//����������������г�ȥ�ص��ƣ����ж϶��ƺ�����

	BYTE bTempIndex[MAX_INDEX];
	CopyMemory(bTempIndex,m_cbCardIndex[wChairID],sizeof(bTempIndex));

	//ɾ���Ѿ���ع��Ƶ�����
	for (BYTE j=0; j<8; j++)
	{
		BYTE index = m_WuTongIndex[wChairID][j];
		if (index != (BYTE)(-1))
		{
			bTempIndex[index] = 0;
		}
	}
	bUserAction |= m_GameLogic.AnalyseWuTong(bTempIndex,m_WeaveItemArray[wChairID],m_cbWeaveItemCount[wChairID],WuTongResult);

	//�����Ĳص����:ȥ����ǰ����Ѿ��ع��������ƣ�������
	//ɾ����ͳ��������m_SiTongIndex[GAME_PLAYER][8]
	for (BYTE j=0; j<8; j++)
	{
		BYTE index = m_SiTongIndex[wChairID][j];
		if (index != (BYTE)(-1))
		{
			bTempIndex[index] = 0;
		}
	}
	bUserAction |= m_GameLogic.AnalyseSiTong(bTempIndex,SiTongResult);
	
	return bUserAction;
}

//�ɷ��˿�
bool CTableFrameSink::DispatchCardData(WORD wCurrentUser)
{
	//״̬Ч��
	ASSERT(wCurrentUser!=INVALID_CHAIR);
	if (wCurrentUser==INVALID_CHAIR) return false;

	//�������û������Ʒŵ�������������
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		m_cbDiscardCount[m_wOutCardUser]++;
		//ֱ�ӱ��涪�����˿�����
		m_cbDiscardCard[m_wOutCardUser][m_cbDiscardCount[m_wOutCardUser]-1]=m_cbOutCardData;
	}

	//���ñ���
	ZeroMemory(m_cbSendCardData,sizeof(m_cbSendCardData));

	//�Ƿ񺣵�,�����˺���ʱ����û��ʱ,��ʱ���ǽ������ƺ���,�������������ʣ���Ƶĸ���Ϊ����ô�Ͳ������������Ĳ���,ֱ�ӵ�����Ϸ��������
	//��ʣ����Ϊ3��ʱ�����ֺ���,��ÿ���˷�һ���ƣ��ܺ��ͺ������ܺ��ͽ���
	if (m_bSendCount!=2 && m_cbLeftCardCount <= 3)
	{
		ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
		bool bAroseAction=false;
		WORD HDUser = INVALID_CHAIR;
		m_wCurrentUser = INVALID_CHAIR;
		//��ׯ�������ù����û�Ϊ��
		m_cbChiHuCard=0;
		m_wProvideUser=INVALID_CHAIR;

		//��������
		CMD_S_SendCard SendCard;
		ZeroMemory(&SendCard,sizeof(SendCard));

		SendCard.cbActionMask=WIK_NULL;
		SendCard.bIsHD=true;
		//�ӵ�ǰ�û���ʼ,�����ҷ����ƺ���������
		for(BYTE i=0; i<GAME_PLAYER;i++)
		{

////��ӡû��������֮ǰ�û������Ƶ���Ϣ
//#ifdef _DEBUG
//			m_GameLogic.m_Debug.BeginPrint();
//			m_GameLogic.m_Debug.PrintCardMessage(m_cbCardIndex[i]);
//			m_GameLogic.m_Debug.EndPrint(i);
//#endif

			BYTE wUser = (wCurrentUser-i+GAME_PLAYER)%GAME_PLAYER;
            
			//��û������ʱ���򲻷�������
			if(m_cbLeftCardCount==0) break;

			SendCard.wCurrentUser=wUser;
			SendCard.cbCardData[0] = m_cbRepertoryCard[--m_cbLeftCardCount];
			SendCard.cbSendCount = 1;
			m_cbProvideCard =  SendCard.cbCardData[0];

			//������ҷ���
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

			BYTE index = m_GameLogic.SwitchToCardIndex(SendCard.cbCardData[0]);
			//���溣����
			m_cbCardIndex[wUser][index]++;

////��ӡ���꺣����֮���û������Ƶ���Ϣ
//#ifdef _DEBUG
//			m_GameLogic.m_Debug.BeginPrint();
//			m_GameLogic.m_Debug.PrintCardMessage(m_cbCardIndex[wUser]);
//			m_GameLogic.m_Debug.EndPrint(wUser);
//#endif

			//����ǻ��ƣ������Ʊ��浽��Ӧ��ҵĻ�������������
			if (index==0 || index==2 || index==4 || index==6 || index==8) 
			{
				m_cbHuaCardIndex[wUser][index/2]++;
				ASSERT(m_cbHuaCardIndex[wUser][index/2] <=2);
			}
			//�����Ƿ���Ժ���
			BYTE cbWeaveItemCount=m_cbWeaveItemCount[wUser];
			tagWeaveItem * pWeaveItem=m_WeaveItemArray[wUser];

//#ifdef _DEBUG
//		    m_GameLogic.m_Debug.BeginPrint();
//#endif
			m_cbUserAction[wUser]|=m_GameLogic.AnalyseHuCard(m_cbCardIndex[wUser],pWeaveItem,cbWeaveItemCount,0,m_cbHuaCardIndex[wUser],m_cbHuaWeaveCardIndex[wUser],m_HuCardResult[wUser]);
//#ifdef _DEBUG
//		    m_GameLogic.m_Debug.EndPrint(wUser);
//#endif
			//�ӷ��Ƶĵ�ǰ�û���ʼ,��¼�ҵ��ĵ�һ�����Ժ��Ƶ���ң����Ժ��Ƽ�¼����ҵı��
			if (m_HuCardResult[wUser].IsHu == true && bAroseAction == false) 
			{ 
	   			m_wProvideUser=wUser;
				m_cbChiHuCard = SendCard.cbCardData[0] ;
				HDUser = wUser;
				bAroseAction=true;
			}
			else
			{
				//��ֻ֤�е�һ���û��ܺ���
				m_HuCardResult[wUser].IsHu = false; 
			}
		}

		OnEventGameEnd(HDUser,NULL,GER_NORMAL);

		return true;
	}
	//11-25
	//���ݷ��Ƹ������û�����
	for (BYTE i=0; i<m_bSendCount; i++)
	{
		m_cbSendCardData[i]=m_cbRepertoryCard[--m_cbLeftCardCount];

		//����ת����������������
		m_cbCardIndex[wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData[i])]++;				
	}
	//����ǻ��ƣ��ٽ����Ʊ��浽��ǰ��һ�������������
	m_GameLogic.SaveHuaCard(m_cbSendCardData,m_bSendCount,m_cbHuaCardIndex[wCurrentUser]);

//#ifdef _DEBUG
//	m_GameLogic.m_Debug.BeginPrint();
//	m_GameLogic.m_Debug.PrintCardMessage(m_cbCardIndex[wCurrentUser]);
//	m_GameLogic.m_Debug.EndPrint(wCurrentUser);
//#endif

	//��һ�ִ��ƵĹ����У������Ʒ��͸��û��������κε��жϴ���
	if (m_cbSendCardCount == 1 && m_bGameStart == false)
	{
		//���ж��û��õ�������֮�󣬻��Ƿ��ܲ���
		BYTE bUserAction = WIK_NULL;
		//�õ����û����ܵĲ��Ƶ��ܴ���
		tagTongResult SiTongResult;
		tagTongResult WuTongResult;
		ZeroMemory(&SiTongResult,sizeof(SiTongResult));
		ZeroMemory(&WuTongResult,sizeof(WuTongResult));


		BYTE bTempIndex[MAX_INDEX];
		CopyMemory(bTempIndex,m_cbCardIndex[wCurrentUser],sizeof(bTempIndex));

		//ɾ���Ѿ���ع��Ƶ�����
		for (BYTE j=0; j<8; j++)
		{
			BYTE index = m_WuTongIndex[wCurrentUser][j];
			if (index != (BYTE)(-1))
			{
				bTempIndex[index] = 0;
			}
		}	
		bUserAction|=m_GameLogic.AnalyseWuTong(bTempIndex,NULL,0,WuTongResult);

		//ɾ����ͳ��������m_SiTongIndex[GAME_PLAYER][8]
		for (BYTE j=0; j<8; j++)
		{
			BYTE index = m_SiTongIndex[wCurrentUser][j];
			if (index != (BYTE)(-1))
			{
				bTempIndex[index] = 0;
			}
		}			
		bUserAction|=m_GameLogic.AnalyseSiTong(bTempIndex,SiTongResult);

		//ׯ���ܷ���к��Ƶķ���
		tagHuCardResult HuCardResult;
		ZeroMemory(&HuCardResult,sizeof(HuCardResult));
		if(m_wBankerUser==wCurrentUser)
		{
			bUserAction|=m_GameLogic.AnalyseHuCard(m_cbCardIndex[m_wBankerUser],NULL,0,0,m_cbHuaCardIndex[m_wBankerUser],m_cbHuaWeaveCardIndex[m_wBankerUser],HuCardResult);
		}

		//������һ�ֲ��Ʋ����Ŀ�����Ϣ
		//����ׯ���û�ʱ�����к��ƻ����֮��
		if(m_wBankerUser==wCurrentUser)
		{
			//��ׯ���ܺ���ʱ����ص��˵�����ʼʱ��״̬
			if((bUserAction & WIK_CHI_HU) != 0)
			{
				m_cbFirstUserAction[0]=WIK_CHI_HU;

				m_cbFirstCurrentIndex=0;
				m_cbFirstCurrentUser=m_cbFirstUser[0];
				m_cbFirstCurrentAction=m_cbFirstUserAction[0];

				//�������������ʱ,Ҫ�������ҵĲ������������Ϊ��
				m_cbFirstUserAction[1]=WIK_NULL;
				m_cbFirstUserAction[2]=WIK_NULL;
			}
			else if((bUserAction&WIK_SI_TONG)!=0 || (bUserAction&WIK_WU_TONG)!=0)	
			{
				m_cbFirstUserAction[3]=bUserAction;

				m_cbFirstCurrentIndex=3;
				m_cbFirstCurrentUser=m_cbFirstUser[3];
				m_cbFirstCurrentAction=m_cbFirstUserAction[3];
			}
			else
			{
				m_cbFirstUserAction[3]=WIK_NULL;

				m_cbFirstCurrentIndex=3;
				m_cbFirstCurrentUser=m_cbFirstUser[3];
				m_cbFirstCurrentAction=m_cbFirstUserAction[3];
			}
		}
		//���������û�ʱ��ֻ�����в���֮��
		else
		{
			if((bUserAction&WIK_SI_TONG)!=0 || (bUserAction&WIK_WU_TONG)!=0)
			{
				m_cbFirstUserAction[m_cbFirstCurrentIndex]=bUserAction;
			}
			else
			{
				m_cbFirstUserAction[m_cbFirstCurrentIndex]=WIK_NULL;
			}
			m_cbFirstCurrentUser=m_cbFirstUser[m_cbFirstCurrentIndex];
			m_cbFirstCurrentAction=m_cbFirstUserAction[m_cbFirstCurrentIndex];
		}

		//��������
		CMD_S_SendCard SendCard;
		SendCard.wCurrentUser=wCurrentUser;
		SendCard.cbActionMask=bUserAction; 
		SendCard.cbSendCount = m_bSendCount;
		CopyMemory(SendCard.cbCardData,m_cbSendCardData,sizeof(m_cbSendCardData));
		SendCard.bIsHD=false;

		//����ׯ���ܺ���ʱ��Ҫ������Ӧ�ĺ�������
		if((m_wBankerUser==wCurrentUser) && (bUserAction& WIK_CHI_HU) != 0)
		{
			CopyMemory(SendCard.cbCenterCard,HuCardResult.AnalyseItem.cbCenterCard,sizeof(HuCardResult.AnalyseItem.cbCenterCard));
			CopyMemory(SendCard.cbWeaveKind,HuCardResult.AnalyseItem.cbWeaveKind,sizeof(HuCardResult.AnalyseItem.cbWeaveKind));
			CopyMemory(SendCard.cbYaKou,HuCardResult.AnalyseItem.cbCardEye,sizeof(HuCardResult.AnalyseItem.cbCardEye));
			SendCard.cbHuScore = HuCardResult.HuScore;
			SendCard.cbGoldCard = HuCardResult.bRealGold;
		}

		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

		return true;
	}

	//���洦���������������
	m_wProvideUser=wCurrentUser;
	m_cbProvideCard=m_cbSendCardData[0];
	ZeroMemory(&m_cbUserAction[wCurrentUser],sizeof(m_cbUserAction[wCurrentUser]));
	m_cbSendCardCount++;

	//�����Ƿ����Ĳأ����
	m_cbUserAction[wCurrentUser]|= EstimateTongCard(wCurrentUser);

	//�����Ƿ���Ժ���
	tagHuCardResult HuCardResult;
	BYTE cbWeaveItemCount=m_cbWeaveItemCount[wCurrentUser];
	tagWeaveItem * pWeaveItem=m_WeaveItemArray[wCurrentUser];

//#ifdef _DEBUG
//	m_GameLogic.m_Debug.BeginPrint();
//#endif
	m_cbUserAction[wCurrentUser]|=m_GameLogic.AnalyseHuCard(m_cbCardIndex[wCurrentUser],pWeaveItem,cbWeaveItemCount,0,m_cbHuaCardIndex[wCurrentUser],m_cbHuaWeaveCardIndex[wCurrentUser],HuCardResult);
//#ifdef _DEBUG
//	m_GameLogic.m_Debug.EndPrint(wCurrentUser);
//#endif
	
	//���ͳ���û��ĸ���Ϊ0������һ�ֽ���,���Ƶ�ʱ�����õ�ǰ���
	m_wCurrentUser= wCurrentUser;

	//���ñ���
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;

	//��������
	CMD_S_SendCard SendCard;
	ZeroMemory(&SendCard,sizeof(SendCard));
	SendCard.wCurrentUser=wCurrentUser;
	SendCard.cbActionMask=m_cbUserAction[wCurrentUser];

	//11-29
	//�����������
	if(m_cbUserAction[wCurrentUser] & WIK_CHI_HU)
	{
		CopyMemory(SendCard.cbCenterCard,HuCardResult.AnalyseItem.cbCenterCard,sizeof(HuCardResult.AnalyseItem.cbCenterCard));
		CopyMemory(SendCard.cbWeaveKind,HuCardResult.AnalyseItem.cbWeaveKind,sizeof(HuCardResult.AnalyseItem.cbWeaveKind));
		CopyMemory(SendCard.cbYaKou,HuCardResult.AnalyseItem.cbCardEye,sizeof(HuCardResult.AnalyseItem.cbCardEye));
		SendCard.cbHuScore = HuCardResult.HuScore;
		SendCard.cbGoldCard = HuCardResult.bRealGold;
	}

	SendCard.cbSendCount = m_bSendCount;
	CopyMemory(SendCard.cbCardData,m_cbSendCardData,sizeof(m_cbSendCardData));
	SendCard.bIsHD=(m_cbLeftCardCount == 0)?true:false;

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

	return true;
}


////////////////////////babala////////////////////
//��Ӧ�ж�
bool CTableFrameSink::EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard)
{
	//��������
	bool bAroseAction=false;

	//����û�������ص�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_HuCardResult,sizeof(m_HuCardResult));

	//�����ж�
	for (WORD i=0;i<m_wPlayerCount;i++)//m_wPlayerCount ��Ϸ����
	{
		//�����Լ����Ƶ��û�
		if (wCenterUser==i) continue;

		//��Ӧ�ж�
		if (m_cbLeftCardCount >= 3)
		{
			//�����ж�
			m_cbUserAction[i]|=m_GameLogic.EstimateDuiCard(m_cbCardIndex[i],m_SiTongIndex[i],m_SiTongCount[i],cbCenterCard);

			//�����ж�--���л�����
			m_cbUserAction[i]|=m_GameLogic.EstimateZhaoCard(m_cbCardIndex[i],m_SiTongIndex[i],m_SiTongCount[i],cbCenterCard);
		}
		//�Ժ��ж�
		//tagHuCardResult HuResult;
		BYTE cbWeaveItemCount=m_cbWeaveItemCount[i];
		tagWeaveItem * pWeaveItem=m_WeaveItemArray[i];

//#ifdef _DEBUG
//		m_GameLogic.m_Debug.BeginPrint();
//#endif
		m_cbUserAction[i]|=m_GameLogic.AnalyseHuCard(m_cbCardIndex[i],pWeaveItem,cbWeaveItemCount,cbCenterCard,m_cbHuaCardIndex[i],m_cbHuaWeaveCardIndex[i],m_HuCardResult[i]);
//#ifdef _DEBUG
//		m_GameLogic.m_Debug.EndPrint(i);
//#endif
		if (m_cbUserAction[i]!=WIK_NULL) bAroseAction=true;
	}

	//���������־Ϊ��˵����һ���û������������˿���Ҫ������Ҫ����һ����ʾ��Ϣ�Ŀ�
	if (bAroseAction==true) //
	{
		//���ñ���
		m_wProvideUser=wCenterUser;
		m_cbProvideCard=cbCenterCard;
		m_wResumeUser=m_wCurrentUser;
		m_wCurrentUser=INVALID_CHAIR;

		//���в��������,������ʾ
		SendOperateNotify();

		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
