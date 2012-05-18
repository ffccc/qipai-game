#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////
//静态变量
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//游戏人数
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_FullReady;	//开始模式

//////////////////////////////////////////////////////////////////////////

//构造函数
CTableFrameSink::CTableFrameSink()
{
	//游戏变量
	m_wBankerUser=INVALID_CHAIR;

	//复位其它相关的变量
	RepositTableFrameSink();
}

//析构函数
CTableFrameSink::~CTableFrameSink(void)
{

}

//接口查询
void * __cdecl CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//初始化
bool __cdecl CTableFrameSink::InitTableFrameSink(IUnknownEx *pIUnknownEx)
{
	//查询接口
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//初始化打印信息对象
	m_GameLogic.m_Debug.InitDebug(m_pITableFrame);

	//获取参数
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);
	return true;
}

//复位桌子
void __cdecl CTableFrameSink::RepositTableFrameSink()
{

	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

	//出牌信息
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));
	//发牌信息
	m_bSendCount = 0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));

	//运行变量
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//状态变量
	m_bSendStatus=false;
	b_HuangZhuang=false;
	m_cbChiHuCard=0;

	//用户状态
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//组合扑克
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	//组件变量
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

//开始模式
enStartMode __cdecl CTableFrameSink::GetGameStartMode()
{
	return m_GameStartMode;
}

//游戏状态
bool __cdecl CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	return true;
}

//游戏开始
//在游戏开始时所做的的事情有：
//1 洗牌
//2 发牌并且把相应的牌转换为逻辑值
//3 判断是否有可能在第一轮就有人胡牌
bool __cdecl CTableFrameSink::OnEventGameStart()
{
	BYTE	cbCardData[GAME_PLAYER][26]; //保存发给各用户的牌信息
	ZeroMemory(cbCardData,sizeof(cbCardData));

	//设置状态
	m_pITableFrame->SetGameStatus(GS_MJ_PLAY);
	//混乱扑克
	m_cbLeftCardCount=CountArray(m_cbRepertoryCard);
	m_GameLogic.RandCardData(m_cbRepertoryCard,CountArray(m_cbRepertoryCard));
	
	//2009.2.20
	//发牌定庄,如果是第一轮庄家为空，每家随机机发一张牌，比较牌点大小，牌点大的玩家坐庄
	if (m_wBankerUser == INVALID_CHAIR)
	{
		//找出能区分最大一张牌的情况
		BYTE cbTempCardData[m_wPlayerCount];
		do{
			//得到三张牌
			BYTE cbTempCardRank[m_wPlayerCount];
			ZeroMemory(cbTempCardData,sizeof(cbTempCardData));
			ZeroMemory(cbTempCardRank,sizeof(cbTempCardRank));

			for (BYTE i=0; i<m_wPlayerCount; i++)
			{
				BYTE index = rand()%110;
				cbTempCardData[i] = m_cbRepertoryCard[index];
				cbTempCardRank[i]=m_GameLogic.GetCardRank(m_cbRepertoryCard[index]);
			}

			//分析是否有唯一的一张最大的牌
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
		
		//将最后结果发送给各用户
		for (BYTE i=0; i<m_wPlayerCount; i++)
		{
			CMD_S_DecideBanker DecideBanker;
			DecideBanker.bCardData = cbTempCardData[i];
			DecideBanker.bUserID = i;

			//发送数据
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_DECIDE_BANKER,&DecideBanker,sizeof(DecideBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_DECIDE_BANKER,&DecideBanker,sizeof(DecideBanker));
		}
	}
	//	//////给三个玩家发25张牌
	
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
	////////////硬编码牌 /////////////
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

	////将硬编码的牌放到三个用户中
	//CopyMemory(cbCardData[0],temp0,25);
	//CopyMemory(cbCardData[1],temp1,25);
	//CopyMemory(cbCardData[2],temp2,25);

		//将25张麻将保存到对应玩家的牌数据数组中
	//	CopyMemory(m_cbCardData[0],temp0,25);
	//	//写一函数，从牌数据数组中得到花牌
	//BYTE count = m_GameLogic.GetHuaCard(m_cbCardData[2],25,m_cbHuaCardIndex[2]);
	//m_GameLogic.SwitchToCardIndex(temp0,25,m_cbCardIndex[2]);

		//分发扑克--即每一个人解析他的25张牌结果存放在m_cbCardIndex[i]中
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//将25张麻将保存到对应玩家的牌数据数组中
		m_GameLogic.SwitchToCardIndex(cbCardData[i],25,m_cbCardIndex[i]);

		//从牌数据数组中得到花牌个数
		m_GameLogic.GetHuaCard(cbCardData[i],25,m_cbHuaCardIndex[i]);
	}

	//给庄家发送一个牌
	m_cbSendCardCount++;
	m_cbSendCardData[0]=m_cbRepertoryCard[--m_cbLeftCardCount];
		
	m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData[0])]++;
	//将最后一张牌保存到庄家牌数据数组中
	cbCardData[m_wBankerUser][25] = m_cbSendCardData[0];

	//如果庄家牌是花牌,保存到庄家花牌索引数组中
	if ((m_cbSendCardData[0] & MASK_COLOR) == 0x10)
	{
		BYTE index = m_GameLogic.SwitchToCardIndex(m_cbSendCardData[0]);
		m_cbHuaCardIndex[m_wBankerUser][index/2]++;
		ASSERT(m_cbHuaCardIndex[m_wBankerUser][index/2] <=2);		
	}

	
	////分析三家的藏牌情况
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
	//分析庄家用户是否可以胡牌
//#ifdef _DEBUG
//	m_GameLogic.m_Debug.BeginPrint();
//#endif

	m_cbUserAction[m_wBankerUser]|=m_GameLogic.AnalyseHuCard(m_cbCardIndex[m_wBankerUser],NULL,0,0,m_cbHuaCardIndex[m_wBankerUser],m_cbHuaWeaveCardIndex[m_wBankerUser],m_HuCardResult[m_wBankerUser]);

//#ifdef _DEBUG
//		m_GameLogic.m_Debug.EndPrint(m_wBankerUser);
//#endif

	//初始化第一轮的藏牌控制变量
	ZeroMemory(m_cbFirstUserAction,sizeof(m_cbFirstUserAction));
	ZeroMemory(m_cbFirstUser,sizeof(m_cbFirstUser));
	m_cbFirstCurrentIndex=3;
	m_cbFirstCurrentUser=m_wBankerUser;
	m_cbFirstCurrentAction=WIK_NULL;

	//得到出牌用户下家用户和下下家用户
	WORD wBeforeOneUser = (m_wBankerUser+1+m_wPlayerCount)%m_wPlayerCount; //上家
	WORD wBeforeTwoUser = (m_wBankerUser+2+m_wPlayerCount)%m_wPlayerCount; //上上家

	//按顺序操作的用户信息
	m_cbFirstUser[0]=m_wBankerUser;
	m_cbFirstUser[1]=wBeforeOneUser;
	m_cbFirstUser[2]=wBeforeTwoUser;
	m_cbFirstUser[3]=m_wBankerUser;

	//按顺序操作的用户能进行的操作信息
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

	//设置当前用户及其操作
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
	//判断游戏是否正常开始--当前操作用户为庄家，并且庄家没有藏牌的话，表明游戏正式开始
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
//str.Format("当前操作用户是%s",pIServerUserItem->GetAccounts());
//if((m_cbFirstCurrentAction&WIK_SI_TONG)!=0)
//{
//	str.Format("%s  操作有四藏",str);
//}
//if((m_cbFirstCurrentAction&WIK_WU_TONG)!=0)
//{
//	str.Format("%s  操作有五藏",str);
//}
//AfxMessageBox(str);
//m_pITableFrame->SendGameMessage(pIServerUserItem,str,SMT_INFO);
///////////////////////////////////////////

	//构造数据,发送开始信息
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));
	GameStart.wBankerUser=m_wBankerUser;

	//设置能进行操作的当前用户及其操作
	GameStart.wCurrentUser=m_cbFirstCurrentUser;
	GameStart.cbCurrentAction=m_cbFirstCurrentAction;
	
	//向每个玩家发送数据
	for (BYTE i=0;i<m_wPlayerCount;i++)
	{
		//设置变量
		CopyMemory(GameStart.cbCardData,cbCardData[i],sizeof(GameStart.cbCardData));

		//如果是庄家,并且有胡牌操作，拷贝最佳胡牌组合,胡牌组合的操作，丫口值
		if (i == m_wBankerUser && (m_cbUserAction[i] & WIK_CHI_HU) != 0)
		{
			CopyMemory(GameStart.cbCenterCard,m_HuCardResult[m_wBankerUser].AnalyseItem.cbCenterCard,sizeof(GameStart.cbCenterCard));
			CopyMemory(GameStart.cbWeaveKind,m_HuCardResult[m_wBankerUser].AnalyseItem.cbWeaveKind,sizeof(GameStart.cbWeaveKind));
			CopyMemory(GameStart.cbYaKou,m_HuCardResult[m_wBankerUser].AnalyseItem.cbCardEye,sizeof(GameStart.cbYaKou));
			GameStart.cbHuScore = m_HuCardResult[m_wBankerUser].HuScore;
			GameStart.cbGoldCard = m_HuCardResult[m_wBankerUser].bRealGold;
		}

		//发送数据
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}
		
	return true;
}

//游戏结束,流戏结束，统计积分
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	//wChairID  为胡牌用户
	switch (cbReason)
	{
	case GER_NORMAL:		//常规结束
		{

			//定义变量
			CMD_S_GameEnd GameEnd;
			BYTE bChiHuUserCount = 0;   //babala 11-30
			CString strChiHu;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//GameEnd.wProvideUse 点炮用户
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
			//胡牌玩家
			GameEnd.wWinner = nWinner;

			
			//赢家分数
			if (m_wProvideUser != INVALID_CHAIR)
			{
				//拷贝赢家的胡牌组合
			
				CopyMemory(GameEnd.cbCenterCard,m_HuCardResult[nWinner].AnalyseItem.cbCenterCard,sizeof(GameEnd.cbCenterCard));
				CopyMemory(GameEnd.cbWeaveKind,m_HuCardResult[nWinner].AnalyseItem.cbWeaveKind,sizeof(GameEnd.cbWeaveKind));
				CopyMemory(GameEnd.cbYaKou,m_HuCardResult[nWinner].AnalyseItem.cbCardEye,sizeof(GameEnd.cbYaKou));
				
				CopyMemory(GameEnd.cbHuaWeaveCount,m_cbHuaWeaveCardIndex[nWinner],sizeof(GameEnd.cbHuaWeaveCount));
				CopyMemory(GameEnd.cbHuaHandCount,m_cbHuaCardIndex[nWinner],sizeof(GameEnd.cbHuaHandCount));

				bChiHuUserCount = 1; 
			
				if(m_wProvideUser==nWinner)//自摸
				{
					//AfxMessageBox("自摸");
					for(BYTE i=0; i<m_wPlayerCount;i++)
					{
						if(i==nWinner) continue;

						//自摸的时候，两家一起给赢家所赢的分数
						//GameEnd.lGameScore[i] -= m_HuCardResult[nWinner].HuScore;
						GameEnd.lGameScore[i] -= m_HuCardResult[nWinner].HuScore/2;
						GameEnd.lGameScore[nWinner] -= GameEnd.lGameScore[i];
					}
				}
				else//点炮
				{
					//AfxMessageBox("点炮");
					GameEnd.lGameScore[m_wProvideUser] -= m_HuCardResult[nWinner].HuScore;
					GameEnd.lGameScore[nWinner] += m_HuCardResult[nWinner].HuScore;
				}
                //保存胡点
				GameEnd.lHuPoint = (LONG)m_HuCardResult[nWinner].bHuPoint;

				GameEnd.bGoldCard = m_GameLogic.SwitchToCardData(m_HuCardResult[nWinner].bRealGold);

			}
			//流局
			else
			{
				//AfxMessageBox("流局");
				GameEnd.cbChiHuCard=0;
				GameEnd.wChiHuUserCount=0;
				b_HuangZhuang=true;
			}			
			//轮庄，闲家胡牌或流局，庄家的下一家坐庄
			if (m_wBankerUser != nWinner)
			{
				m_wBankerUser = (m_wBankerUser-1 + m_wPlayerCount)%m_wPlayerCount;
			}

			//AfxMessageBox("拷贝玩家扑克");
			//拷贝四个玩家的扑克
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i],m_cbHuaCardIndex[i]);
			}
			//变量定义
			LONG lCellScore = m_pGameServiceOption->lCellScore;
			//校正四家分数
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.lGameScore[i] = GameEnd.lGameScore[i]*lCellScore;
			}
			//税收计算,按正常情况下进行计算,如果有调整,在后面进行调整 
			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
			GameEnd.lTax=0;

			//不求税收
			//初始化设置，如果是流局，设置为和局状态
			for (WORD i=0; i<GAME_PLAYER; i++)
			{
				ScoreInfo[i].ScoreKind = enScoreKind_Draw;
			}

			//如果不是流局,肯定有人输，有人赢，扣除玩家的税 收
			if(b_HuangZhuang == false)
			{
				//税收只扣正分数的玩家即赢家分数
				for (WORD i=0;i<GAME_PLAYER;i++)
				{
					//记录各家的分数
					ScoreInfo[i].lScore=GameEnd.lGameScore[i];
					ScoreInfo[i].ScoreKind=(GameEnd.lGameScore[i]>0L)?enScoreKind_Win:enScoreKind_Lost;
					
				}
				//AfxMessageBox("输出最佳组合");
				CString sz;
				sz.Format("组合牌数为:%d,%d,%d,%d,%d,%d,%d,%d,",m_HuCardResult[nWinner].AnalyseItem.cbCenterCard[0],m_HuCardResult[nWinner].AnalyseItem.cbCenterCard[1],m_HuCardResult[nWinner].AnalyseItem.cbCenterCard[2],m_HuCardResult[nWinner].AnalyseItem.cbCenterCard[3],m_HuCardResult[nWinner].AnalyseItem.cbCenterCard[4],m_HuCardResult[nWinner].AnalyseItem.cbCenterCard[5],m_HuCardResult[nWinner].AnalyseItem.cbCenterCard[6],m_HuCardResult[nWinner].AnalyseItem.cbCenterCard[7]);
				//AfxMessageBox(sz);
				
			}

			//统计积分
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//写入积分
				m_pITableFrame->WriteUserScore(i,ScoreInfo[i].lScore,ScoreInfo[i].lRevenue,ScoreInfo[i].ScoreKind);
			}

			GameEnd.bIsQuit=false;
			GameEnd.wChiHuUserCount=bChiHuUserCount;  
		
			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));


			//结束游戏
			m_pITableFrame->ConcludeGame();

			return true;
		}

	case GER_USER_LEFT:		//用户强退
		{
			//变量定义
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			//变量定义
			LONG lCellScore = m_pGameServiceOption->lCellScore;

			//设置变量
			GameEnd.wProvideUser=INVALID_CHAIR;
			GameEnd.wWinner = INVALID_CHAIR;
			//GameEnd.lGameScore[wChairID]=-(20*lCellScore);
			GameEnd.lGameScore[wChairID]=-(4*lCellScore);
			GameEnd.bIsQuit=true;

			//通知消息
			TCHAR szMessage[128]=TEXT("");
			_snprintf(szMessage,CountArray(szMessage),TEXT("因 ( %s ) 离开游戏，游戏结束"),pIServerUserItem->GetAccounts());
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

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//修改积分
			tagScoreInfo ScoreInfo;
			ScoreInfo.ScoreKind=enScoreKind_Flee;
			ScoreInfo.lScore=GameEnd.lGameScore[wChairID];
            m_pITableFrame->WriteUserScore(wChairID,ScoreInfo.lScore,0,ScoreInfo.ScoreKind);

			//结束游戏
			m_pITableFrame->ConcludeGame();
			return true;
		}

	
	}
	return false;
}

//发送场景
bool __cdecl CTableFrameSink::SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_MJ_FREE://空闲状态--仅将庄家信息与倍数信息发送到客户端
		{
			//变量定义
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));

			//构造数据
			StatusFree.wBankerUser=m_wBankerUser;
			StatusFree.lCellScore=m_pGameServiceOption->lCellScore;

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_MJ_PLAY:	//游戏状态
	case GS_MJ_HAI_DI:	//海底状态
		{
			//变量定义
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//游戏变量
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.lCellScore=m_pGameServiceOption->lCellScore;

			//状态变量
			StatusPlay.cbActionCard=m_cbProvideCard;
			StatusPlay.cbLeftCardCount=m_cbLeftCardCount;
			StatusPlay.cbActionMask=(m_bResponse[wChiarID]==false)?m_cbUserAction[wChiarID]:WIK_NULL;

			//如果断线玩家有胡牌操作，拷贝胡牌组合
			if (m_HuCardResult[wChiarID].IsHu == true)
			{
				CopyMemory(StatusPlay.cbCenterCard,m_HuCardResult[wChiarID].AnalyseItem.cbCenterCard,sizeof(m_HuCardResult[wChiarID].AnalyseItem.cbCenterCard));
				CopyMemory(StatusPlay.cbWeaveKind,m_HuCardResult[wChiarID].AnalyseItem.cbWeaveKind,sizeof(m_HuCardResult[wChiarID].AnalyseItem.cbWeaveKind));
				CopyMemory(StatusPlay.cbYaKou,m_HuCardResult[wChiarID].AnalyseItem.cbCardEye,sizeof(m_HuCardResult[wChiarID].AnalyseItem.cbCardEye));
				StatusPlay.cbHuScore = m_HuCardResult[wChiarID].HuScore;
				StatusPlay.cbGoldCard = m_HuCardResult[wChiarID].bRealGold;
			}

			//历史记录
			StatusPlay.wOutCardUser=m_wOutCardUser;
			StatusPlay.cbOutCardData=m_cbOutCardData;
			CopyMemory(StatusPlay.cbDiscardCard,m_cbDiscardCard,sizeof(StatusPlay.cbDiscardCard));
			CopyMemory(StatusPlay.cbDiscardCount,m_cbDiscardCount,sizeof(StatusPlay.cbDiscardCount));

			//组合扑克
			CopyMemory(StatusPlay.WeaveItemArray,m_WeaveItemArray,sizeof(m_WeaveItemArray));
			CopyMemory(StatusPlay.cbWeaveCount,m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));
			CopyMemory(StatusPlay.cbHuaWeaveCardIndex,m_cbHuaWeaveCardIndex,sizeof(m_cbHuaWeaveCardIndex));

			//转换扑克数据
			StatusPlay.cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[wChiarID],StatusPlay.cbCardData,m_cbHuaCardIndex[wChiarID]);

			//得到另两个玩家的扑克个数
			for (BYTE i=0; i<GAME_PLAYER; i++)
			{
				//统计牌的张数
				for (BYTE j=0; j<MAX_INDEX; j++) StatusPlay.cbUserCardCount[i] += m_cbCardIndex[i][j];
			}

			//游戏开始标志
			StatusPlay.bGameStart = m_bGameStart;
			if(m_bGameStart==false)
			{
				StatusPlay.wFirstCurrentUser=m_cbFirstCurrentUser;
				StatusPlay.cbFirstCurrentAction=m_cbFirstCurrentAction;
			}

			//藏牌次数
			StatusPlay.bSiTongCount = m_SiTongCount[wChiarID];
			StatusPlay.bWuTongCount = m_WuTongCount[wChiarID];
			CopyMemory(StatusPlay.bSiTongIndex,m_SiTongIndex[wChiarID],sizeof(m_SiTongIndex[wChiarID]));
			CopyMemory(StatusPlay.bWuTongIndex,m_WuTongIndex[wChiarID],sizeof(m_WuTongIndex[wChiarID]));

			//还剩下牌的总数目
			StatusPlay.cbLeftCardCount=m_cbLeftCardCount;

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	return false;
}

//定时器事件
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
	return false;
}

//游戏消息处理
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_S_ACTION:
		{
			return OnEventGameStart();			
		}
	case SUB_C_OUT_CARD:		//出牌消息
		{
			//效验消息
			ASSERT(wDataSize==sizeof(CMD_C_OutCard));
			if (wDataSize!=sizeof(CMD_C_OutCard)) return false;

			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//消息处理
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pDataBuffer;

			return OnUserOutCard(pUserData->wChairID,pOutCard->cbCardData);
		}
	case SUB_C_OPERATE_CARD:	//操作消息
		{
			//效验消息
			ASSERT(wDataSize==sizeof(CMD_C_OperateCard));
			if (wDataSize!=sizeof(CMD_C_OperateCard)) return false;

			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//消息处理
			CMD_C_OperateCard * pOperateCard=(CMD_C_OperateCard *)pDataBuffer;
			return OnUserOperateCard(pUserData->wChairID,pOperateCard->cbOperateCode,pOperateCard->cbOperateCard);
		}
	}

	return false;
}

//框架消息处理
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}


//用户坐下
bool __cdecl CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//庄家设置
	if ((bLookonUser==false)&&(m_wBankerUser==INVALID_CHAIR))
	{
		m_wBankerUser=pIServerUserItem->GetChairID();
	}

	return true;
}

//用户起来
bool __cdecl CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//庄家设置
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

//用户出牌
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData)
{
	//效验状态
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

	//错误断言
	ASSERT(wChairID==m_wCurrentUser);
	ASSERT(m_GameLogic.IsValidCard(cbCardData)==true);

	//效验参数
	if (wChairID!=m_wCurrentUser) return false;
	if (m_GameLogic.IsValidCard(cbCardData)==false) return false;

	//得到出牌索引
	BYTE index = m_GameLogic.SwitchToCardIndex(cbCardData);

	//如果是花牌，花牌索引数组中对应位置的牌数减1
	if ( (cbCardData & MASK_COLOR) == 0x10 )
	{
		//断言该花牌个数不为0
		ASSERT(m_cbHuaCardIndex[wChairID][index/2] != 0);
		if (m_cbHuaCardIndex[wChairID][index/2] == 0) return false;
		
		//花牌索引位置减1
		m_cbHuaCardIndex[wChairID][index/2]--;
	}

	//从出牌玩家手中,删除所出的扑克
	ASSERT(m_cbCardIndex[wChairID][index] >= 1);
	if (m_cbCardIndex[wChairID][index] <1) return false;
	m_cbCardIndex[wChairID][index]--;

#ifdef _DEBUG
	m_GameLogic.m_Debug.BeginPrint();
	m_GameLogic.m_Debug.PrintCardMessage(m_cbCardIndex[wChairID]);
	m_GameLogic.m_Debug.EndPrint(wChairID);
#endif
	
	//设置变量
	m_bSendStatus=true;      //-------????????????
	m_cbUserAction[wChairID]=WIK_NULL;	 //用户动作
	m_cbPerformAction[wChairID]=WIK_NULL;//m_cbPerformAction 执行动作--保存各用户真正执行的操作

	m_wOutCardUser=wChairID;
	m_cbOutCardData=cbCardData;

	//构造数据
	CMD_S_OutCard OutCard;
	OutCard.wOutCardUser=wChairID;
	OutCard.cbOutCardData=cbCardData;
	
	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));


	//用户切换
	m_wProvideUser=wChairID;
	m_cbProvideCard=cbCardData;
	//切换到下家用户
	m_wCurrentUser=(wChairID+m_wPlayerCount-1)%m_wPlayerCount;
	m_bSendCount = 1;

	//响应判断，如果用户出的是一般牌，判断其他用户是否需要该牌，EstimatKind_OutCard只是正常出牌判断
	//如果当前用户自己 出了牌，不能自己对自己进行分析吃，碰杠
	bool bAroseAction=EstimateUserRespond(wChairID,cbCardData);
 
	//派发扑克
	if (bAroseAction==false) DispatchCardData(m_wCurrentUser);

	return true;
}

//用户操作
bool CTableFrameSink::OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard)
{
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

	//操作牌之前打印调试信息
//#ifdef _DEBUG
//	m_GameLogic.m_Debug.BeginPrint();
//	m_GameLogic.m_Debug.PrintCardMessage(m_cbCardIndex[wChairID]);
//	m_GameLogic.m_Debug.PrintWeaveItemsMessage(m_WeaveItemArray[wChairID],m_cbWeaveItemCount[wChairID]);
//	m_GameLogic.m_Debug.EndPrint(wChairID);
//#endif

    //当有多人能同时操作，但当级别最高的用户执行了操作但其结果没有返回其它用户，
	//而此时其它用户也发送来操作请求时，要将其过滤掉。
    // 不是第一轮的藏牌                不是用户自摸操作情况                已有高级别的用户执行了操作
	if((m_cbSendCardCount != 1) && (wChairID != m_wCurrentUser) && (m_wCurrentUser != INVALID_CHAIR))
	{   //  此用户以前没有操作过                   但最高级别的用户已操作过
		if((m_bResponse[wChairID] == false) && (m_bResponse[m_wCurrentUser] == true))
		{
			return true;
		}
	}

	//效验用户,能进这一函数要么是用户有响应操作,要么是自摸操作,还有一种是第一轮发牌时三家请统
	ASSERT((wChairID==m_wCurrentUser)||(m_wCurrentUser==INVALID_CHAIR) || (m_cbSendCardCount == 1));
	if ((wChairID!=m_wCurrentUser)&&(m_wCurrentUser!=INVALID_CHAIR)&&(m_cbSendCardCount != 1)) return false;

	//1、如果是第一轮的藏牌:有四统，五统，放弃统牌操作
	if (m_cbSendCardCount == 1 && m_bGameStart == false)
	{
		//先清空此用户的操作信息
		m_cbFirstUserAction[m_cbFirstCurrentIndex]=WIK_NULL;
		//根据操作分类处理:
		switch (cbOperateCode)
		{
		case WIK_SI_TONG:        //四统操作
			{
				//记录玩家四统的次数,所藏的牌
				BYTE index = m_SiTongCount[wChairID]++;
				m_SiTongIndex[wChairID][index]=m_GameLogic.SwitchToCardIndex(cbOperateCard);
				
				//四藏补发一张牌
				m_bSendCount = 1;
				
				break;
			}
		case WIK_WU_TONG:        //五统操作
			{
				BYTE index = m_WuTongCount[wChairID]++;
				m_WuTongIndex[wChairID][index]=m_GameLogic.SwitchToCardIndex(cbOperateCard);

				//五藏补发两张牌
				m_bSendCount = 2;

				//如果所操作的五藏中,前四张牌已经藏过,此时只能给用户发一张牌
				for (BYTE i=0; i<8; i++)
				{
					if ( m_GameLogic.SwitchToCardIndex(cbOperateCard) == m_SiTongIndex[wChairID][i] )
					{
						//将其后的四藏数据复制到前面来
						for(BYTE j=i;j<m_SiTongCount[wChairID];j++)
						{
							m_SiTongIndex[wChairID][j]=m_SiTongIndex[wChairID][j+1];
						}

						//修改此用户四藏的次数
						m_SiTongCount[wChairID]--;
						m_SiTongIndex[wChairID][m_SiTongCount[wChairID]]=(BYTE)(-1);

						//此时应该只能给用户发一张牌
						m_bSendCount = 1;
						break;
					}
				}
				break;
			}
		case WIK_TONG_GIVE_UP:   //点放弃操作
			{
				break;
			}
		case WIK_CHI_HU:         //第一轮情况下胡牌,只有庄家自摸胡牌的可能性
			{
				ASSERT(wChairID == m_wBankerUser);
				if (wChairID != m_wBankerUser) return false;

				//先清空胡牌记录信息，仅保存本次胡牌用户的胡牌信息
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

		//如果是统牌操作，向操作用户发牌
		if ((cbOperateCode & WIK_SI_TONG)!=0 || (cbOperateCode & WIK_WU_TONG) != 0)
		{
			m_bSendStatus=true;
			DispatchCardData(wChairID);
		}

		//修正第一轮藏牌操作的控制信息
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

		//判断游戏是否正常开始
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
//str.Format("当前操作用户是%s",pIServerUserItem->GetAccounts());
//if((m_cbFirstCurrentAction&WIK_SI_TONG)!=0)
//{
//	str.Format("%s  操作有四藏",str);
//}
//if((m_cbFirstCurrentAction&WIK_WU_TONG)!=0)
//{
//	str.Format("%s  操作有五藏",str);
//}
//AfxMessageBox(str);
//m_pITableFrame->SendGameMessage(pIServerUserItem,str,SMT_INFO);
///////////////////////////////////////////
		//构造结果
		CMD_S_OperateResult OperateResult;
		OperateResult.wOperateUser=wChairID;
		OperateResult.cbOperateCard=cbOperateCard;
		OperateResult.cbOperateCode=cbOperateCode;
		OperateResult.wProvideUser=wChairID;
		OperateResult.cbHuaCount = 0;

		//设置能进行操作的当前用户及其操作
		OperateResult.wCurrentUser=m_cbFirstCurrentUser;
		OperateResult.cbCurrentAction=m_cbFirstCurrentAction;

		//发送消息
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

		return true;
	}

	//2、别人打牌引起的操作：对牌,招牌(三招或四招)
	if (m_wCurrentUser==INVALID_CHAIR)
	{
		//效验状态
		ASSERT(m_bResponse[wChairID]==false);
		ASSERT((cbOperateCode==WIK_NULL)||(cbOperateCode==WIK_TONG_GIVE_UP)||((m_cbUserAction[wChairID]&cbOperateCode)!=0));
		
		//效验状态	//m_bResponse[]响应标志--保存各用户是否响应过此操作
		if (m_bResponse[wChairID]==true) return false;
		if ((cbOperateCode!=WIK_NULL)&& (cbOperateCode!=WIK_TONG_GIVE_UP) &&((m_cbUserAction[wChairID]&cbOperateCode)==0)) return false;

		//变量定义
		//WORD wTargetUser=wChairID;
		WORD wTargetUser=wChairID; 
		BYTE cbTargetAction=cbOperateCode;

		//更新此用户操作的相关变量
		m_bResponse[wChairID]=true; //响应标志--保存各用户是否响应过此操作
		m_cbPerformAction[wChairID]=cbOperateCode; //执行动作--保存各用户真正执行的操作
		m_cbOperateCard[wChairID]=(cbOperateCard==0)?m_cbProvideCard:cbOperateCard;

		//2009.03.02修改
		//是别人打出牌，而另两家操作的情况

		//得到出牌用户下家用户和下下家用户
		WORD wNextOneUser = (m_wProvideUser-1+m_wPlayerCount)%m_wPlayerCount; //下家
		WORD wNextTwoUser = (m_wProvideUser-2+m_wPlayerCount)%m_wPlayerCount; //下下家

		//当另两家都有操作时，下家具有操作优先级别
		if((m_cbUserAction[wNextOneUser]!= WIK_NULL) && (m_cbUserAction[wNextTwoUser]!= WIK_NULL))
		{
			//得到下家用户与下下家用户对此牌的操作
			//m_bResponse[] 响应标志--保存各用户是否响应过此操作    //m_cbUserAction[3];用户动作   //执行动作--保存各用户真正执行的操作
			BYTE cbNextOneUserAction=(m_bResponse[wNextOneUser]==false)?m_cbUserAction[wNextOneUser]:m_cbPerformAction[wNextOneUser];
			BYTE cbNextTwoUserAction=(m_bResponse[wNextTwoUser]==false)?m_cbUserAction[wNextTwoUser]:m_cbPerformAction[wNextTwoUser];
		
			//得到下家用户与下下家用户对此牌的操作级别
			BYTE cbNextOneUserRank = m_GameLogic.GetUserActionRank(cbNextOneUserAction);
			BYTE cbNextTwoUserRank = m_GameLogic.GetUserActionRank(cbNextTwoUserAction);

			//比较两者的操作级别而作不同的处理
			//1当下家用户的操作级别大于或等于下下家用户的操作级别时，由下家用户优先操作
			if(cbNextOneUserRank >= cbNextTwoUserRank)
			{
				//AfxMessageBox("当下家的操作级别比下下家大时，是否进到了这里？");
				//当下家用户已响应此牌的话，则统系执行下家用户的操作
				
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
			//否则由下下家用户操作此牌
			else
			{
				//当下下家用户已响应此牌的话，则统系执行下下家用户的操作
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

		//当是用户放弃操作时，则清空本轮操作牌相关的变量，向下家发送牌
		if (cbTargetAction==WIK_NULL)
		{
			//用户状态
			ZeroMemory(m_bResponse,sizeof(m_bResponse));
			ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
			ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
			ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

			DispatchCardData(m_wResumeUser);

			return true;
		}

		//执行胡牌操作
		if (cbTargetAction==WIK_CHI_HU)
		{
				//结束信息
			m_cbChiHuCard=cbTargetCard;
			
			//普通胡牌，有人点炮--此处一定能成立
			if (m_cbChiHuCard!=0)
			{
				//先清空胡牌记录信息，仅保存本次胡牌用户的胡牌信息
				ZeroMemory(m_HuCardResult,sizeof(m_HuCardResult));

				//胡牌判断
				BYTE cbWeaveItemCount=m_cbWeaveItemCount[wTargetUser];
				tagWeaveItem * pWeaveItem=m_WeaveItemArray[wTargetUser];
			
				//分析用户是否可以胡牌,如果可以,结束游戏,否则报"炸胡"扣操作玩家分数,继续游戏
//				//分析胡牌函数没有....待续.,,,,
//#ifdef _DEBUG
//		        m_GameLogic.m_Debug.BeginPrint();
//#endif
				//得到一个最佳的胡牌结果,保存到玩家的胡牌信息中,返回是否可以胡牌
				m_GameLogic.AnalyseHuCard(m_cbCardIndex[wTargetUser],pWeaveItem,cbWeaveItemCount,m_cbChiHuCard,m_cbHuaCardIndex[wTargetUser],m_cbHuaWeaveCardIndex[wTargetUser],m_HuCardResult[wTargetUser]);
//#ifdef _DEBUG
//		        m_GameLogic.m_Debug.EndPrint(wTargetUser);
//#endif
				ASSERT(m_HuCardResult[wTargetUser].IsHu == true);
				if (m_HuCardResult[wTargetUser].IsHu == false)  return false;

				//插入扑克
				if (m_HuCardResult[wTargetUser].IsHu == true) 
				{
					BYTE index = m_GameLogic.SwitchToCardIndex(m_cbChiHuCard);
					m_cbCardIndex[wTargetUser][index]++;

					//如果是花牌,将花牌保存到玩家花牌索引中
					if((m_cbChiHuCard & 0xf0)==0x10)
					{
						m_cbHuaCardIndex[wTargetUser][index/2]++;
					}
				}
			}	
			OnEventGameEnd(m_wProvideUser,NULL,GER_NORMAL);
			return true;
		}
		//清除出牌信息
		m_cbOutCardData=0;
		m_wOutCardUser=INVALID_CHAIR;


		//用户状态
		ZeroMemory(m_bResponse,sizeof(m_bResponse));
		ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
		ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
		ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

		//下面执行非胡牌的其它操作
		ASSERT(m_cbWeaveItemCount[wTargetUser]<9);
		WORD wIndex=m_cbWeaveItemCount[wTargetUser]++;
		m_WeaveItemArray[wTargetUser][wIndex].cbPublicCard=TRUE;
		m_WeaveItemArray[wTargetUser][wIndex].cbCenterCard=cbTargetCard;
		m_WeaveItemArray[wTargetUser][wIndex].cbWeaveKind=cbTargetAction;
		m_WeaveItemArray[wTargetUser][wIndex].wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;

		BYTE cbHuaCard = 0;

		//删除扑克
		switch (cbTargetAction)
		{
		case WIK_DUI:		//对牌操作
			{
				BYTE index = m_GameLogic.SwitchToCardIndex(cbTargetCard);

				//断言对牌时,用户该位置至少有两张牌
				ASSERT(m_cbCardIndex[wTargetUser][index] >= 2);
				if (m_cbCardIndex[wTargetUser][index] < 2) return false;

				//删除扑克,将引起操作的牌转换成索引,删除索引数组中牌的个数
				m_cbCardIndex[wTargetUser][index] -= 2;

				//如果是经牌
				if (index == 0 || index == 2 || index == 4 || index == 6 || index == 8)
				{
					//判断引起操作的牌是否是花牌,如果是最大操作的组合花牌数加1
					if((cbTargetCard & 0xF0) == 0x10)
					{
						m_cbHuaWeaveCardIndex[wTargetUser][index/2]++;
						ASSERT(m_cbHuaWeaveCardIndex[wTargetUser][index/2]<=2);
						cbHuaCard++;
					}
					//判断玩家手中的花牌是否为0，如果不为0，全部用花牌去对别人的牌
					if (m_cbHuaCardIndex[wTargetUser][index/2] > 0) 
					{
						cbHuaCard += m_cbHuaCardIndex[wTargetUser][index/2];
						m_cbHuaWeaveCardIndex[wTargetUser][index/2] += m_cbHuaCardIndex[wTargetUser][index/2];
						m_cbHuaCardIndex[wTargetUser][index/2] = 0;
					}
				}
				break;
			}
		case WIK_SAN_ZHAO:		//招牌操作
			{
				BYTE index = m_GameLogic.SwitchToCardIndex(cbTargetCard);

				//断言对牌时,用户该位置至少有三张牌
				ASSERT(m_cbCardIndex[wTargetUser][index] >= 3);
				if (m_cbCardIndex[wTargetUser][index] < 3) return false;

				m_bSendCount = 1;
				//删除扑克,将引起操作的牌转换成索引,删除索引数组中牌的个数
				m_cbCardIndex[wTargetUser][index] -= 3;

				//如果是经牌
				if (index == 0 || index == 2 || index == 4 || index == 6 || index == 8)
				{
					//判断引起操作的牌是否是花牌,如果是最大操作的组合花牌数加1
					if((cbTargetCard & 0xF0) == 0x10)
					{
						m_cbHuaWeaveCardIndex[wTargetUser][index/2]++;
						ASSERT(m_cbHuaWeaveCardIndex[wTargetUser][index/2]<=2);
						cbHuaCard++;
					}

					//判断玩家手中的花牌是否为0，如果不为0，全部用花牌去对别人的牌
					if (m_cbHuaCardIndex[wTargetUser][index/2] > 0) 
					{
						cbHuaCard += m_cbHuaCardIndex[wTargetUser][index/2];
						m_cbHuaWeaveCardIndex[wTargetUser][index/2] += m_cbHuaCardIndex[wTargetUser][index/2];
						m_cbHuaCardIndex[wTargetUser][index/2] = 0;
					}
				}
				break;
			}
		case WIK_SI_ZHAO:		//招牌操作
			{
				BYTE index = m_GameLogic.SwitchToCardIndex(cbTargetCard);

				//断言四招时,用户该位置至少有四张牌
				ASSERT(m_cbCardIndex[wTargetUser][index] >= 4);
				if (m_cbCardIndex[wTargetUser][index] < 4) return false;

				//删除扑克,将引起操作的牌转换成索引,删除索引数组中牌的个数
				m_cbCardIndex[wTargetUser][index] -= 4;
				m_bSendCount = 2;

				//如果是用将四藏牌拆开去招别人的牌，当前玩家藏牌数减1，原来所藏的牌置为0
				for (BYTE i=0; i<8;i++)
				{
					if (m_GameLogic.SwitchToCardIndex(cbTargetCard) == m_SiTongIndex[wTargetUser][i])
					{			
					    //2009.03.02修改
						//将其后的四藏数据复制到前面来
						for(BYTE j=i;j<m_SiTongCount[wTargetUser];j++)
						{
							m_SiTongIndex[wTargetUser][j]=m_SiTongIndex[wTargetUser][j+1];
						}

						//修改此用户四藏的次数
						m_SiTongCount[wTargetUser]--;
						m_SiTongIndex[wTargetUser][m_SiTongCount[wTargetUser]]=(BYTE)(-1);

						//此时应该只能给用户发一张牌
						m_bSendCount = 1;
						break;
					}
				}
				//如果是经牌
				if (index == 0 || index == 2 || index == 4 || index == 6 || index == 8)
				{
					//判断引起操作的牌是否是花牌,如果是最大操作的组合花牌数加1
					if((cbTargetCard & 0xF0) == 0x10)
					{
						m_cbHuaWeaveCardIndex[wTargetUser][index/2]++;
						ASSERT(m_cbHuaWeaveCardIndex[wTargetUser][index/2]<=2);
						cbHuaCard++;
					}

					//判断玩家手中的花牌是否为0，如果不为0，全部用花牌去对别人的牌
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

		//构造结果
		CMD_S_OperateResult OperateResult;
		OperateResult.wOperateUser=wTargetUser;
		OperateResult.cbOperateCard=cbTargetCard;
		OperateResult.cbOperateCode=cbTargetAction;
		OperateResult.wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;
		OperateResult.cbHuaCount = cbHuaCard;

		//发送消息
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

#ifdef _DEBUG
	m_GameLogic.m_Debug.BeginPrint();
	m_GameLogic.m_Debug.PrintCardMessage(m_cbCardIndex[wChairID]);
	m_GameLogic.m_Debug.PrintWeaveItemsMessage(m_WeaveItemArray[wChairID],m_cbWeaveItemCount[wChairID]);
	m_GameLogic.m_Debug.EndPrint(wChairID);
#endif

		//设置用户
		m_wCurrentUser=wTargetUser;

		//最大操作用户操作的是招牌，向操作用户发牌,处理完成，返回
		if ((cbTargetAction==WIK_SAN_ZHAO) || (cbTargetAction==WIK_SI_ZHAO)) 
		{
			DispatchCardData(wTargetUser);
			return true;
		}

		//如果是对牌，不发牌，客户端启动对牌操作用户的出牌定时器
		return true;
	}

	//3、用户自摸时的操作：自摸胡牌、四藏（藏四张）,五藏（藏五张）
	if (m_wCurrentUser==wChairID)
	{
		//效验操作--要么操作码为空，要么操作码与提示的操作码相同
		ASSERT((cbOperateCode==WIK_NULL) || ((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID] & cbOperateCode)!=0)));
		if ((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)==0)) return false;

		//用户状态
		ZeroMemory(m_bResponse,sizeof(m_bResponse));
		ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
		ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
		ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

		//设置变量
		m_bSendStatus=true;
		m_cbUserAction[wChairID]=WIK_NULL;
		m_cbPerformAction[wChairID]=WIK_NULL;

		//执行动作
		switch (cbOperateCode)
		{
		case WIK_SI_TONG:        //四藏操作
			{
				//记录玩家四统的次数,所统的牌
				BYTE index = m_SiTongCount[wChairID]++;
				m_SiTongIndex[wChairID][index]=m_GameLogic.SwitchToCardIndex(cbOperateCard);

				m_bSendCount = 1;
				break;
			}
		case WIK_WU_TONG:        //五藏操作
			{
				//变量定义
				BYTE cbWeaveIndex=0xFF;
				BYTE cbCardIndex=m_GameLogic.SwitchToCardIndex(cbOperateCard);
				m_bSendCount = 2;

				//组合藏牌--当此用户手中仅一张牌时，一定是使用原三招的组合进行藏牌，此时原三招组合变成现在的四招组合
				if (m_cbCardIndex[wChairID][cbCardIndex]==1)
				{
					//寻找组合牌数组中，判断组合牌中是否有三招一的牌
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
					//效验动作
					ASSERT(cbWeaveIndex!=0xFF);
					if (cbWeaveIndex==0xFF) return false;

					//将此种情况暂且就作为四统的情况讨论吧
					m_bSendCount = 1;

					//主动状态下将摸到的一张牌放到倒下的四张牌中去,修改组合牌
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=TRUE;
					m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=WIK_SI_ZHAO;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard;

					//经牌处理，如果引起操作的那张牌是1，3，5，7，9，将手中的花牌个数记为0
					if (cbCardIndex == 0 || cbCardIndex == 2 || cbCardIndex == 4 || cbCardIndex == 6 || cbCardIndex == 8)
					{
						//组合牌中花牌个数为2
						m_cbHuaWeaveCardIndex[wChairID][cbCardIndex/2] = 2;

						//手中花牌个数为0
						m_cbHuaCardIndex[wChairID][cbCardIndex/2] = 0;
					}
					//手中该经牌个数为0
					m_cbCardIndex[wChairID][cbCardIndex] = 0;						
				}
				//手中牌五藏
				else
				{
					//检查操作的牌是不是已经四藏了，如果操作的牌是已经四藏的牌，将该牌对应的四藏次数减1，五藏次数加1
					//并且此种情况下只能给用户发一张藏牌
					m_bSendCount = 2;

					//如果所操作的五藏中,前四张牌已经藏过,此时只能给用户发一张牌
					for (BYTE i=0; i<8; i++)
					{
						if ( m_GameLogic.SwitchToCardIndex(cbOperateCard) == m_SiTongIndex[wChairID][i] )
						{
							//2009.03.02修改
							//将其后的四藏数据复制到前面来
							for(BYTE j=i;j<m_SiTongCount[wChairID];j++)
							{
								m_SiTongIndex[wChairID][j]=m_SiTongIndex[wChairID][j+1];
							}

							//修改此用户四藏的次数
							m_SiTongCount[wChairID]--;
							m_SiTongIndex[wChairID][m_SiTongCount[wChairID]]=(BYTE)(-1);

							//此时应该只能给用户发一张牌
							m_bSendCount = 1;
							break;
						}
					}
					//记录玩家五统的次数
					BYTE index = m_WuTongCount[wChairID]++;
					m_WuTongIndex[wChairID][index]=m_GameLogic.SwitchToCardIndex(cbOperateCard);
				}
				break;
			}
		
		case WIK_CHI_HU:		 //自胡操作
			{
				//先清空胡牌记录信息，仅保存本次胡牌用户的胡牌信息
				ZeroMemory(m_HuCardResult,sizeof(m_HuCardResult));

				//普通胡牌
				BYTE cbWeaveItemCount=m_cbWeaveItemCount[wChairID];
				tagWeaveItem * pWeaveItem=m_WeaveItemArray[wChairID];
//
//#ifdef _DEBUG
//		        m_GameLogic.m_Debug.BeginPrint();
//#endif
				//得到一个最佳的胡牌结果,保存到玩家的胡牌信息中,返回是否可以胡牌
				m_GameLogic.AnalyseHuCard(m_cbCardIndex[wChairID],pWeaveItem,cbWeaveItemCount,0,m_cbHuaCardIndex[wChairID],m_cbHuaWeaveCardIndex[wChairID],m_HuCardResult[wChairID]);
//#ifdef _DEBUG
//		        m_GameLogic.m_Debug.EndPrint(wChairID);
//#endif
				ASSERT(m_HuCardResult[wChairID].IsHu == true);
				if (m_HuCardResult[wChairID].IsHu == false) return false;

				//如果用户可以胡牌,结束游戏
				m_wProvideUser=wChairID;	
				//结束信息,自摸 胡牌时,传过来的中心牌为0
				m_cbChiHuCard=m_cbProvideCard;

				OnEventGameEnd(m_wProvideUser,NULL,GER_NORMAL);

				return true;
			}
		}

		//构造结果
		CMD_S_OperateResult OperateResult;
		OperateResult.wOperateUser=wChairID;
		OperateResult.cbOperateCard=cbOperateCard;
		OperateResult.cbOperateCode=cbOperateCode;

		OperateResult.wProvideUser=m_wCurrentUser;
		OperateResult.cbHuaCount = 0;
		//发送消息
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

#ifdef _DEBUG
	m_GameLogic.m_Debug.BeginPrint();
	m_GameLogic.m_Debug.PrintCardMessage(m_cbCardIndex[wChairID]);
	m_GameLogic.m_Debug.PrintWeaveItemsMessage(m_WeaveItemArray[wChairID],m_cbWeaveItemCount[wChairID]);
	m_GameLogic.m_Debug.EndPrint(wChairID);
#endif
		//根据统的张数向统牌玩家发牌
		if ((cbOperateCode & WIK_SI_TONG)!=0 || (cbOperateCode & WIK_WU_TONG) != 0)
		{
			m_bSendStatus = true;
			
			//游戏中发统牌，向统牌用户发牌
			DispatchCardData(wChairID);
		}

		return true;
	}

	return false;
}


//发送操作
bool CTableFrameSink::SendOperateNotify()
{
	//发送提示
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbUserAction[i]!=WIK_NULL)
		{
			//构造数据
			CMD_S_OperateNotify OperateNotify;
			ZeroMemory(&OperateNotify,sizeof(OperateNotify));

			OperateNotify.wResumeUser=m_wResumeUser;
			OperateNotify.cbActionCard=m_cbProvideCard;
			OperateNotify.cbActionMask=m_cbUserAction[i];

			//11-29
			//拷贝胡牌组合
			if((m_cbUserAction[i] & WIK_CHI_HU) != 0)
			{
				CopyMemory(OperateNotify.cbCenterCard,m_HuCardResult[i].AnalyseItem.cbCenterCard,sizeof(m_HuCardResult[i].AnalyseItem.cbCenterCard));
				CopyMemory(OperateNotify.cbWeaveKind,m_HuCardResult[i].AnalyseItem.cbWeaveKind,sizeof(m_HuCardResult[i].AnalyseItem.cbWeaveKind));
				CopyMemory(OperateNotify.cbYaKou,m_HuCardResult[i].AnalyseItem.cbCardEye,sizeof(m_HuCardResult[i].AnalyseItem.cbCardEye));
				OperateNotify.cbHuScore = m_HuCardResult[i].HuScore;
				OperateNotify.cbGoldCard = m_HuCardResult[i].bRealGold;
			}

			//发送数据
			m_pITableFrame->SendTableData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
			m_pITableFrame->SendLookonData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
		}
	}
	return true;
}

//分析玩家是否可以藏牌，如果可以返回藏牌类型
BYTE CTableFrameSink:: EstimateTongCard(WORD wChairID)
{
	BYTE bUserAction = WIK_NULL;
	
	//分析玩家的藏牌情况
	tagTongResult SiTongResult;
	tagTongResult WuTongResult;
	ZeroMemory(&SiTongResult,sizeof(SiTongResult));
	ZeroMemory(&WuTongResult,sizeof(WuTongResult));

	//从玩家牌索引数组中除去藏的牌，后判断对牌和招牌

	BYTE bTempIndex[MAX_INDEX];
	CopyMemory(bTempIndex,m_cbCardIndex[wChairID],sizeof(bTempIndex));

	//删除已经五藏过牌的索引
	for (BYTE j=0; j<8; j++)
	{
		BYTE index = m_WuTongIndex[wChairID][j];
		if (index != (BYTE)(-1))
		{
			bTempIndex[index] = 0;
		}
	}
	bUserAction |= m_GameLogic.AnalyseWuTong(bTempIndex,m_WeaveItemArray[wChairID],m_cbWeaveItemCount[wChairID],WuTongResult);

	//分析四藏的情况:去掉当前玩家已经藏过的四张牌，五张牌
	//删除四统的牌索引m_SiTongIndex[GAME_PLAYER][8]
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

//派发扑克
bool CTableFrameSink::DispatchCardData(WORD wCurrentUser)
{
	//状态效验
	ASSERT(wCurrentUser!=INVALID_CHAIR);
	if (wCurrentUser==INVALID_CHAIR) return false;

	//将出牌用户出的牌放到丢弃牌数组中
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		m_cbDiscardCount[m_wOutCardUser]++;
		//直接保存丢弃的扑克数据
		m_cbDiscardCard[m_wOutCardUser][m_cbDiscardCount[m_wOutCardUser]-1]=m_cbOutCardData;
	}

	//设置变量
	ZeroMemory(m_cbSendCardData,sizeof(m_cbSendCardData));

	//是否海底,当到了海底时即牌没有时,此时就是进到发牌函数,当这个函数发现剩余牌的个数为零那么就不用再做其他的操作,直接调用游戏结束函数
	//当剩余牌为3张时，出现海底,给每个人发一张牌，能胡就胡，不能胡就结束
	if (m_bSendCount!=2 && m_cbLeftCardCount <= 3)
	{
		ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
		bool bAroseAction=false;
		WORD HDUser = INVALID_CHAIR;
		m_wCurrentUser = INVALID_CHAIR;
		//荒庄结赖设置供牌用户为空
		m_cbChiHuCard=0;
		m_wProvideUser=INVALID_CHAIR;

		//构造数据
		CMD_S_SendCard SendCard;
		ZeroMemory(&SendCard,sizeof(SendCard));

		SendCard.cbActionMask=WIK_NULL;
		SendCard.bIsHD=true;
		//从当前用户开始,向三家发送牌海底牌数据
		for(BYTE i=0; i<GAME_PLAYER;i++)
		{

////打印没发海底牌之前用户手中牌的信息
//#ifdef _DEBUG
//			m_GameLogic.m_Debug.BeginPrint();
//			m_GameLogic.m_Debug.PrintCardMessage(m_cbCardIndex[i]);
//			m_GameLogic.m_Debug.EndPrint(i);
//#endif

			BYTE wUser = (wCurrentUser-i+GAME_PLAYER)%GAME_PLAYER;
            
			//当没有了牌时，则不发送牌了
			if(m_cbLeftCardCount==0) break;

			SendCard.wCurrentUser=wUser;
			SendCard.cbCardData[0] = m_cbRepertoryCard[--m_cbLeftCardCount];
			SendCard.cbSendCount = 1;
			m_cbProvideCard =  SendCard.cbCardData[0];

			//向发牌玩家发牌
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

			BYTE index = m_GameLogic.SwitchToCardIndex(SendCard.cbCardData[0]);
			//保存海底牌
			m_cbCardIndex[wUser][index]++;

////打印发完海底牌之后用户手中牌的信息
//#ifdef _DEBUG
//			m_GameLogic.m_Debug.BeginPrint();
//			m_GameLogic.m_Debug.PrintCardMessage(m_cbCardIndex[wUser]);
//			m_GameLogic.m_Debug.EndPrint(wUser);
//#endif

			//如果是花牌，将花牌保存到对应玩家的花牌索引数组中
			if (index==0 || index==2 || index==4 || index==6 || index==8) 
			{
				m_cbHuaCardIndex[wUser][index/2]++;
				ASSERT(m_cbHuaCardIndex[wUser][index/2] <=2);
			}
			//分析是否可以胡牌
			BYTE cbWeaveItemCount=m_cbWeaveItemCount[wUser];
			tagWeaveItem * pWeaveItem=m_WeaveItemArray[wUser];

//#ifdef _DEBUG
//		    m_GameLogic.m_Debug.BeginPrint();
//#endif
			m_cbUserAction[wUser]|=m_GameLogic.AnalyseHuCard(m_cbCardIndex[wUser],pWeaveItem,cbWeaveItemCount,0,m_cbHuaCardIndex[wUser],m_cbHuaWeaveCardIndex[wUser],m_HuCardResult[wUser]);
//#ifdef _DEBUG
//		    m_GameLogic.m_Debug.EndPrint(wUser);
//#endif
			//从发牌的当前用户开始,记录找到的第一个可以胡牌的玩家，可以胡牌记录该玩家的标号
			if (m_HuCardResult[wUser].IsHu == true && bAroseAction == false) 
			{ 
	   			m_wProvideUser=wUser;
				m_cbChiHuCard = SendCard.cbCardData[0] ;
				HDUser = wUser;
				bAroseAction=true;
			}
			else
			{
				//保证只有第一个用户能胡牌
				m_HuCardResult[wUser].IsHu = false; 
			}
		}

		OnEventGameEnd(HDUser,NULL,GER_NORMAL);

		return true;
	}
	//11-25
	//根据发牌个数给用户发牌
	for (BYTE i=0; i<m_bSendCount; i++)
	{
		m_cbSendCardData[i]=m_cbRepertoryCard[--m_cbLeftCardCount];

		//将牌转换成索引保存起来
		m_cbCardIndex[wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData[i])]++;				
	}
	//如果是花牌，再将花牌保存到当前玩家花牌索引数组中
	m_GameLogic.SaveHuaCard(m_cbSendCardData,m_bSendCount,m_cbHuaCardIndex[wCurrentUser]);

//#ifdef _DEBUG
//	m_GameLogic.m_Debug.BeginPrint();
//	m_GameLogic.m_Debug.PrintCardMessage(m_cbCardIndex[wCurrentUser]);
//	m_GameLogic.m_Debug.EndPrint(wCurrentUser);
//#endif

	//第一轮打牌的过程中，仅将牌发送给用户，不作任何的判断处理
	if (m_cbSendCardCount == 1 && m_bGameStart == false)
	{
		//先判断用户得到本张牌之后，还是否能藏牌
		BYTE bUserAction = WIK_NULL;
		//得到此用户可能的藏牌的总次数
		tagTongResult SiTongResult;
		tagTongResult WuTongResult;
		ZeroMemory(&SiTongResult,sizeof(SiTongResult));
		ZeroMemory(&WuTongResult,sizeof(WuTongResult));


		BYTE bTempIndex[MAX_INDEX];
		CopyMemory(bTempIndex,m_cbCardIndex[wCurrentUser],sizeof(bTempIndex));

		//删除已经五藏过牌的索引
		for (BYTE j=0; j<8; j++)
		{
			BYTE index = m_WuTongIndex[wCurrentUser][j];
			if (index != (BYTE)(-1))
			{
				bTempIndex[index] = 0;
			}
		}	
		bUserAction|=m_GameLogic.AnalyseWuTong(bTempIndex,NULL,0,WuTongResult);

		//删除四统的牌索引m_SiTongIndex[GAME_PLAYER][8]
		for (BYTE j=0; j<8; j++)
		{
			BYTE index = m_SiTongIndex[wCurrentUser][j];
			if (index != (BYTE)(-1))
			{
				bTempIndex[index] = 0;
			}
		}			
		bUserAction|=m_GameLogic.AnalyseSiTong(bTempIndex,SiTongResult);

		//庄家能否进行胡牌的分析
		tagHuCardResult HuCardResult;
		ZeroMemory(&HuCardResult,sizeof(HuCardResult));
		if(m_wBankerUser==wCurrentUser)
		{
			bUserAction|=m_GameLogic.AnalyseHuCard(m_cbCardIndex[m_wBankerUser],NULL,0,0,m_cbHuaCardIndex[m_wBankerUser],m_cbHuaWeaveCardIndex[m_wBankerUser],HuCardResult);
		}

		//修正第一轮藏牌操作的控制信息
		//当是庄家用户时，其有胡牌或藏牌之分
		if(m_wBankerUser==wCurrentUser)
		{
			//当庄家能胡牌时，则回到了当初开始时的状态
			if((bUserAction & WIK_CHI_HU) != 0)
			{
				m_cbFirstUserAction[0]=WIK_CHI_HU;

				m_cbFirstCurrentIndex=0;
				m_cbFirstCurrentUser=m_cbFirstUser[0];
				m_cbFirstCurrentAction=m_cbFirstUserAction[0];

				//当这种情况发生时,要将另两家的操作情况都设置为空
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
		//当是其它用户时，只可能有藏牌之分
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

		//构造数据
		CMD_S_SendCard SendCard;
		SendCard.wCurrentUser=wCurrentUser;
		SendCard.cbActionMask=bUserAction; 
		SendCard.cbSendCount = m_bSendCount;
		CopyMemory(SendCard.cbCardData,m_cbSendCardData,sizeof(m_cbSendCardData));
		SendCard.bIsHD=false;

		//当是庄家能胡牌时，要复制相应的胡牌数据
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

	//下面处理正常发牌情况：
	m_wProvideUser=wCurrentUser;
	m_cbProvideCard=m_cbSendCardData[0];
	ZeroMemory(&m_cbUserAction[wCurrentUser],sizeof(m_cbUserAction[wCurrentUser]));
	m_cbSendCardCount++;

	//分析是否有四藏，五藏
	m_cbUserAction[wCurrentUser]|= EstimateTongCard(wCurrentUser);

	//分析是否可以胡牌
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
	
	//如果统牌用户的个数为0，即第一轮结束,发牌的时候设置当前玩家
	m_wCurrentUser= wCurrentUser;

	//设置变量
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;

	//构造数据
	CMD_S_SendCard SendCard;
	ZeroMemory(&SendCard,sizeof(SendCard));
	SendCard.wCurrentUser=wCurrentUser;
	SendCard.cbActionMask=m_cbUserAction[wCurrentUser];

	//11-29
	//拷贝胡牌组合
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

	//发送数据
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

	return true;
}


////////////////////////babala////////////////////
//响应判断
bool CTableFrameSink::EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard)
{
	//变量定义
	bool bAroseAction=false;

	//清空用户操作相关的状态
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_HuCardResult,sizeof(m_HuCardResult));

	//动作判断
	for (WORD i=0;i<m_wPlayerCount;i++)//m_wPlayerCount 游戏人数
	{
		//过滤自己出牌的用户
		if (wCenterUser==i) continue;

		//响应判断
		if (m_cbLeftCardCount >= 3)
		{
			//对牌判断
			m_cbUserAction[i]|=m_GameLogic.EstimateDuiCard(m_cbCardIndex[i],m_SiTongIndex[i],m_SiTongCount[i],cbCenterCard);

			//招牌判断--三招或四招
			m_cbUserAction[i]|=m_GameLogic.EstimateZhaoCard(m_cbCardIndex[i],m_SiTongIndex[i],m_SiTongCount[i],cbCenterCard);
		}
		//吃胡判断
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

	//结果处理，标志为真说明上一个用户所出的牌有人可以要，所以要发送一个提示信息的框
	if (bAroseAction==true) //
	{
		//设置变量
		m_wProvideUser=wCenterUser;
		m_cbProvideCard=cbCenterCard;
		m_wResumeUser=m_wCurrentUser;
		m_wCurrentUser=INVALID_CHAIR;

		//向有操作的玩家,发送提示
		SendOperateNotify();

		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
