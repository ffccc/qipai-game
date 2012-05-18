#ifndef CMD_SPARROW_HEAD_FILE
#define CMD_SPARROW_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID						3001								//游戏 I D
#define GAME_PLAYER					3									//游戏人数
#define GAME_NAME					TEXT("湖北花牌五精动画版")					//游戏名字
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_MATCH)	//游戏类型

//游戏状态
#define GS_MJ_FREE					GS_FREE								//空闲状态
#define GS_MJ_PLAY					(GS_PLAYING+1)						//游戏状态
#define GS_MJ_HAI_DI				10

//////////////////////////////////////////////////////////////////////////

//组合子项
struct CMD_WeaveItem
{
	BYTE				cbWeaveKind;					//组合类型
	BYTE				cbCenterCard;					//中心扑克
	BYTE				cbPublicCard;					//公开标志
	WORD				wProvideUser;					//供应用户
};

//胡牌情况组合子项
struct CMD_HuCardItem
{
	BYTE                 cbCenterCard[8];               //8种组合的中心牌值
	BYTE                 cbWeaveKind[8];                //8种组合的操作类型
	BYTE                 cbYaKou[2];                    //一对丫口的值
	BYTE                 cbHuScore;                     //胡牌的分数
	BYTE                 cbGoldCard;                    //胡牌的主精牌
};

//藏牌情况组合子项
struct CMD_TongCardItem
{
	BYTE				cbSiTongCount;					//四藏的次数
	BYTE				cbSiTongCardData[6];			//四藏对应的中心牌

	BYTE				cbWuTongCount;					//五藏的次数
	BYTE				cbWuTongCardData[5];			//五藏对应的中心牌
};

//////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_GAME_START			100									//游戏开始
#define SUB_S_OUT_CARD				101									//出牌命令
#define SUB_S_SEND_CARD				102									//发送扑克
#define SUB_S_OPERATE_NOTIFY		103									//操作提示
#define SUB_S_OPERATE_RESULT		104									//操作命令
#define SUB_S_GAME_END				106									//游戏结束

#define SUB_S_HIDE  				107								    //藏牌命令
#define SUB_S_DECIDE_BANKER 		108									//定庄

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#define SUB_S_HD					110									//海底提示
#define SUB_S_ACTION                109                                 //转动骰子
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//定庄结构体
struct CMD_S_DecideBanker
{
	BYTE          bUserID;                  //接收牌的用户
	BYTE          bCardData;                //发送的定庄牌数据
};

//11-29
////////////////////babala
//游戏开始
struct CMD_S_GameStart
{
	WORD				 wCurrentUser;					//当前可操作的用户
	BYTE				 cbCurrentAction;				//当前用户可进行的操作

	WORD				 wBankerUser;					//庄家用户
	BYTE				 cbCardData[26];				//扑克数据

	BYTE                 cbCenterCard[8];               //8种组合的操作类型
	BYTE                 cbWeaveKind[8];                //8种组合的操作类型
	BYTE                 cbYaKou[2];                    //丫口的值
	BYTE                 cbHuScore;                     //丫口的值
	BYTE                 cbGoldCard;                    //丫口的值
};

/////////////////////babala
//藏牌结构体
struct CMD_S_HIDE
{
	WORD				  wTongUserCount;				//藏牌玩家的个数
	BYTE				  cbUserAction;				    //用户动作
};

//操作命令
struct CMD_S_OperateResult
{
	WORD					wOperateUser;				//操作用户
	WORD					wProvideUser;				//供应用户
	BYTE					cbOperateCode;				//操作码
	BYTE					cbOperateCard;				//操作扑克
	BYTE                    cbHuaCount;				    //花牌个数

	WORD				    wCurrentUser;					//当前可操作的用户
	BYTE				    cbCurrentAction;					//当前用户可进行的操作

};

//游戏状态
struct CMD_S_StatusFree
{
	LONG							lCellScore;							//基础金币
	WORD							wBankerUser;						//庄家用户
};

//游戏状态
struct CMD_S_StatusPlay
{
	//游戏变量
	LONG					lCellScore;							//单元积分
	WORD					wBankerUser;						//庄家用户
	WORD					wCurrentUser;						//当前用户

	//状态变量
	BYTE					cbActionCard;						//动作扑克
	BYTE					cbActionMask;						//动作掩码

	//出牌信息
	WORD					wOutCardUser;						//出牌用户
	BYTE					cbOutCardData;						//出牌扑克
	BYTE					cbDiscardCount[3];					//丢弃数目
	BYTE					cbDiscardCard[3][34];				//丢弃记录

	//扑克数据
	BYTE					cbCardCount;						//扑克数目
	BYTE					cbCardData[36];						//扑克列表
	BYTE                    cbUserCardCount[3];                 //用户玩家手中牌张数
	//组合扑克
	BYTE					cbWeaveCount[3];					//组合数目
	CMD_WeaveItem			WeaveItemArray[3][8];				//组合扑克
	BYTE                    cbHuaWeaveCardIndex[3][5];          //三家组合的花牌的个数

	BYTE                    bSiTongCount;                       //四统的次数
	BYTE                    bSiTongIndex[8];                    //四统的索引

	BYTE                    bWuTongCount;                       //五统的次数
	BYTE                    bWuTongIndex[8];                    //五统的索引

	BYTE					cbLeftCardCount;					//剩余数目

	bool                    bGameStart;                         //标记游戏是否开始
	WORD				    wFirstCurrentUser;					//当前可操作的用户
	BYTE				    cbFirstCurrentAction;				//当前用户可进行的操作
	
	//胡牌组合,因为不知道消息里是否能传结构体，因此就把胡牌组合拆开放进来
	BYTE                    cbCenterCard[8];                     //8种组合的操作类型
	BYTE                    cbWeaveKind[8];                      //8种组合的操作类型
	BYTE                    cbYaKou[2];                          //丫口的值
	BYTE                    cbHuScore;                           //丫口的值
	BYTE                    cbGoldCard;                          //丫口的值

};

//出牌命令
struct CMD_S_OutCard
{
	WORD							wOutCardUser;						//出牌用户
	BYTE							cbOutCardData;						//出牌扑克
};

//发送扑克
struct CMD_S_SendCard
{
	BYTE                            cbSendCount;                        //每次发送的牌张数
	BYTE							cbCardData[2];						//扑克数据
	BYTE							cbActionMask;						//动作掩码
	WORD							wCurrentUser;						//当前用户
	bool							bIsHD;								//是否海底

	//下面是能胡牌的信息
	BYTE                            cbCenterCard[8];                     //8种组合的操作类型
	BYTE                            cbWeaveKind[8];                      //8种组合的操作类型
	BYTE                            cbYaKou[2];                          //一对丫口的值
	BYTE                            cbHuScore;                           //胡牌的分值
	BYTE                            cbGoldCard;                          //胡牌的主精牌
};

//11-29
//操作提示
struct CMD_S_OperateNotify
{
	WORD							wResumeUser;						//还原用户
	BYTE							cbActionMask;						//动作掩码
	BYTE							cbActionCard;						//动作扑克

	//下面是能胡牌的信息
	BYTE                            cbCenterCard[8];                     //8种组合的操作类型
	BYTE                            cbWeaveKind[8];                     //8种组合的操作类型
	BYTE                            cbYaKou[2];                         //丫口的值
	BYTE                            cbHuScore;                         //丫口的值
	BYTE                            cbGoldCard;                         //丫口的值
};


//游戏结束
struct CMD_S_GameEnd
{
	BYTE							cbChiHuCard;						//吃胡扑克
	WORD							wProvideUser;						//点炮用户
	LONG							lGameScore[3];						//游戏积分

	BYTE							cbCardCount[3];						//扑克数目
	BYTE							cbCardData[3][36];					//扑克数据
	BYTE							bGoldCard;							//鸟牌
	LONG							lTax;								//游戏税收
	WORD							wChiHuUserCount;					//胡牌总数
	bool							bIsQuit;							//是否强退
	WORD                            wWinner;                            //胡牌玩家
//lHuPoint
	LONG							lHuPoint; 						//基础分
	//以下是有关胡牌组合的信息
	BYTE							cbCenterCard[8];					//8种组合的中心牌
	BYTE							cbWeaveKind[8];						//8种组合类型
	BYTE							cbYaKou[2];							//丫口值
	BYTE							cbHuaWeaveCount[5];					//组合牌中花牌的个数
	BYTE							cbHuaHandCount[5];					//手中牌中花牌的个数

};


//海底提示
struct CMD_S_HD
{
	WORD							wTargetUser;						//下一个赌海底用户
	bool							bIsCanHD;							//能否赌海底	
	//BYTE							bHDCard;							//海底牌
};

//////////////////////////////////////////////////////////////////////////
//客户端命令结构

#define SUB_C_OUT_CARD				1									//出牌命令
#define SUB_C_OPERATE_CARD			2									//操作扑克

#define SUB_C_SORT_READY		    3							       //组牌完成



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#define SUB_C_HD			        4							//海底操作
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


//排序完成结构体
struct CMD_C_SortReady
{
	WORD                            cbSortUserID;                                //排序好的玩家号
	BYTE							cbCardData[9][5];							//排好的扑克数组
};


//操作命令
struct CMD_C_OperateCard
{
	BYTE							cbOperateCode;						//操作代码
	BYTE							cbOperateCard;						//操作扑克
};

//出牌命令
struct CMD_C_OutCardHua
{

	BYTE							cbCardData;							//打出的扑克数据
	BYTE                            cbUserHandData[9][5];                   //用户手中排好序的牌数组
};


//出牌命令
struct CMD_C_OutCard
{
	BYTE							  cbCardData;  						//扑克数据
};


//海底操作
struct CMD_C_HD
{
	bool							cbHDAction;							//是否要海底
	WORD							wTargetUser;						//操作用户	
};

//////////////////////////////////////////////////////////////////////////

#endif