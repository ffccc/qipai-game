#ifndef CMD_HOE_HEAD_FILE
#define CMD_HOE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//公共宏定义

#ifdef	VIDEO_GAME
#define KIND_ID						513									//视频游戏 I D
#else
#define KIND_ID						105									//非视频游戏 I D
#endif

#define GAME_PLAYER					4									//游戏人数
#define GAME_NAME					TEXT("锄大地")						//游戏名字

//游戏状态
#define GS_RF_FREE					GS_FREE								//等待开始
#define GS_RF_PLAYING				GS_PLAYING							//叫分状态

//////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_GAME_START			101									//游戏开始
#define SUB_S_OUT_CARD				102									//用户出牌
#define SUB_S_PASS_CARD				103									//放弃出牌
#define SUB_S_GAME_END				104									//游戏结束
#define SUB_S_STRUSTEE				105									//用户托管

//游戏状态
struct CMD_S_StatusFree
{
	LONG							lBaseScore;							//基础积分
	//托管变量
	bool							bStrustee[GAME_PLAYER];				//托管标志
};

//游戏状态
struct CMD_S_StatusPlay
{
	LONG							lBaseScore;							//基础积分
	WORD							wBankerUser;						//庄家用户
	WORD							wLastOutUser;						//出牌的人
	WORD				 			wCurrentUser;						//当前玩家
	BYTE							bCardData[13*GAME_PLAYER];			//手上扑克
	BYTE							bCardCount[4];						//扑克数目
	BYTE							bTurnCardCount;						//基础出牌
	BYTE							bTurnCardData[13];					//出牌列表
	LONG			                lAllTurnScore[4];					//总局得分
	LONG	                        lLastTurnScore[4];					//上局得分
	//托管变量
	bool							bStrustee[GAME_PLAYER];				//托管标志
};

//发送扑克
struct CMD_S_GameStart
{
	BYTE							cbOutCard3;							//出三标志
	WORD							wBankerUser;						//庄家用户
	WORD				 			wCurrentUser;						//当前玩家
	BYTE							cbCardData[13*GAME_PLAYER];			//扑克列表
};

//用户出牌
struct CMD_S_OutCard
{
	BYTE							bCardCount;							//扑克数目
	WORD				 			wCurrentUser;						//当前玩家
	WORD							wOutCardUser;						//出牌玩家
	BYTE							bCardData[13];						//扑克列表
};

//放弃出牌
struct CMD_S_PassCard
{
	BYTE							bNewTurn;							//一轮开始
	WORD				 			wPassUser;							//放弃玩家
	WORD				 			wCurrentUser;						//当前玩家
};

//游戏结束
struct CMD_S_GameEnd
{
	LONG							lGameScore[4];						//游戏积分
	BYTE							bCardCount[4];						//扑克数目
	BYTE							bCardData[52];						//扑克列表 
};

//用户托管
struct CMD_S_Strustee
{
	WORD							wChairId;							//托管用户
	bool							bStrustee;							//是否托管
};

//////////////////////////////////////////////////////////////////////////
//客户端命令结构

#define SUB_C_OUT_CART				2									//用户出牌
#define SUB_C_PASS_CARD				3									//放弃出牌
#define SUB_C_STRUSTEE				4									//用户托管

//出牌数据包
struct CMD_C_OutCard
{
	BYTE							bCardCount;							//出牌数目
	BYTE							bCardData[13];						//扑克列表
};

//用户托管
struct CMD_C_Strustee
{
	bool bStrustee;														//是否托管
};

//////////////////////////////////////////////////////////////////////////

#endif
