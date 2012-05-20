#ifndef CMD_PAOHUZI_HEAD_FILE
#define CMD_PAOHUZI_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID						22									//游戏 I D
#define GAME_PLAYER					3									//游戏人数
#define GAME_NAME					TEXT("跑胡子游戏")					//游戏名字
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_GOLD)	//游戏类型

//////////////////////////////////////////////////////////////////////////

//数值定义
#define MAX_WEAVE					7									//最大组合
#define MAX_INDEX					20									//最大索引
#define MAX_COUNT					21									//最大数目
#define	MASK_COLOR					0xF0								//花色掩码
#define	MASK_VALUE					0x0F								//数值掩码

//动作定义
#define ACK_NULL					0x00								//空
#define ACK_TI						0x01								//提
#define ACK_PAO						0x02								//跑
#define ACK_WEI						0x04								//偎
#define ACK_CHI						0x08								//吃
#define ACK_CHI_EX					0x10								//吃
#define ACK_PENG					0x20								//碰
#define ACK_CHIHU					0x40								//胡

//吃牌类型
#define CK_NULL						0x00								//无效类型
#define CK_XXD						0x01								//小小大搭
#define CK_XDD						0x02								//小大大搭
#define CK_EQS						0x04								//二七十吃
#define CK_LEFT						0x10								//靠左对齐
#define CK_CENTER					0x20								//居中对齐
#define CK_RIGHT					0x40								//靠右对齐

//////////////////////////////////////////////////////////////////////////

//组合子项
struct tagWeaveItem
{
	BYTE							cbWeaveKind;						//组合类型
	BYTE							cbCardCount;						//扑克数目
	BYTE							cbCenterCard;						//中心扑克
	BYTE							cbCardList[4];						//扑克列表
};

//吃牌信息
struct tagChiCardInfo
{
	BYTE							cbChiKind;							//吃牌类型
	BYTE							cbCenterCard;						//中心扑克
	BYTE							cbResultCount;						//结果数目
	BYTE							cbCardData[3][3];					//吃牌组合
};

//分析子项
struct tagAnalyseItem
{
	BYTE							cbCardEye;							//牌眼扑克
	BYTE							cbHuXiCount;						//胡息数目
	BYTE							cbWeaveCount;						//组合数目
	tagWeaveItem					WeaveItemArray[MAX_WEAVE];			//组合扑克
};

//胡牌信息
struct tagHuCardInfo
{
	BYTE							cbCardEye;							//牌眼扑克
	BYTE							cbHuXiCount;						//胡息数目
	BYTE							cbWeaveCount;						//组合数目
	tagWeaveItem					WeaveItemArray[MAX_WEAVE];			//组合扑克
};

//////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_GAME_START			100									//游戏开始
#define SUB_S_USER_TI_CARD			101									//用户提牌
#define SUB_S_USER_PAO_CARD			102									//用户跑牌
#define SUB_S_USER_WEI_CARD			103									//用户偎牌
#define SUB_S_USER_PENG_CARD		104									//用户碰牌
#define SUB_S_USER_CHI_CARD			105									//用户吃牌
#define SUB_S_OPERATE_NOTIFY		106									//操作提示
#define SUB_S_OUT_CARD_NOTIFY		107									//出牌提示
#define SUB_S_OUT_CARD				108									//用户出牌
#define SUB_S_SEND_CARD				109									//发牌命令
#define SUB_S_GAME_END				110									//游戏结束

//游戏状态
struct CMD_S_StatusFree
{
	LONG							lCellScore;							//基础积分
	WORD							wBankerUser;						//庄家用户
};

//游戏状态
struct CMD_S_StatusPlay
{
	//游戏信息
	LONG							lCellScore;							//基础积分
	WORD							wBankerUser;						//庄家用户
	WORD							wCurrentUser;						//当前用户

	//出牌信息
	BYTE							bOutCard;							//出牌标志
	WORD							wOutCardUser;						//出牌用户
	BYTE							cbOutCardData;						//出牌扑克

	//扑克信息
	BYTE							bLeftCardCount;						//剩余数目
	BYTE							cbCardIndex[MAX_INDEX];				//用户扑克
	BYTE							bUserCardCount[GAME_PLAYER];		//扑克数目

	//组合信息
	BYTE							bWeaveItemCount[GAME_PLAYER];		//组合数目
	tagWeaveItem					WeaveItemArray[GAME_PLAYER][7];		//组合扑克

	//动作信息
	BYTE							bResponse;							//响应标志
	BYTE							bUserAction;						//用户动作
};

//游戏开始
struct CMD_S_GameStart
{
	WORD							wBankerUser;						//庄家用户
	WORD							wCurrentUser;						//当前用户
	BYTE							cbCardData[21];						//扑克列表
};

//提牌命令
struct CMD_S_UserTiCard
{
	WORD							wActionUser;						//动作用户
	BYTE							cbActionCard;						//操作扑克
	BYTE							cbRemoveCount;						//删除数目
};

//跑牌命令
struct CMD_S_UserPaoCard
{
	WORD							wActionUser;						//动作用户
	BYTE							cbActionCard;						//操作扑克
	BYTE							cbRemoveCount;						//删除数目
};

//偎牌命令
struct CMD_S_UserWeiCard
{
	WORD							wActionUser;						//动作用户
	BYTE							cbActionCard;						//操作扑克
};

//碰牌命令
struct CMD_S_UserPengCard
{
	WORD							wActionUser;						//动作用户
	BYTE							cbActionCard;						//操作扑克
};

//吃牌命令
struct CMD_S_UserChiCard
{
	WORD							wActionUser;						//动作用户
	BYTE							cbActionCard;						//操作扑克
	BYTE							cbResultCount;						//结果数目
	BYTE							cbCardData[3][3];					//吃牌组合
};

//操作提示
struct CMD_S_OperateNotify
{
	WORD							wResumeUser;						//还原用户
	BYTE							cbActionCard;						//操作扑克
	BYTE							cbOperateCode;						//操作代码
};

//出牌提示
struct CMD_S_OutCardNotify
{
	BYTE							bOutCard;							//出牌标志
	WORD							wCurrentUser;						//当前用户
};

//用户出牌
struct CMD_S_OutCard
{
	WORD							wOutCardUser;						//出牌用户
	BYTE							cbOutCardData;						//出牌扑克
};

//发牌命令
struct CMD_S_SendCard
{
	BYTE							cbCardData;							//发牌扑克
	WORD							wAttachUser;						//绑定用户
};

//游戏结束
struct CMD_S_GameEnd
{
	//结束信息
	BYTE							cbReason;							//结束原因
	BYTE							cbHuCard;							//胡牌扑克
	WORD							wWinUser;							//胜利用户
	WORD							wProvideUser;						//放跑用户

	//成绩变量
	LONG							lGameTax;							//游戏税收
	LONG							lGameScore[3];						//游戏积分

	//扑克变量
	BYTE							bCardCount[3];						//扑克数目
	BYTE							bCardData[63];						//扑克列表 
};

//////////////////////////////////////////////////////////////////////////
//客户端命令结构

#define SUB_C_OUT_CARD				1									//出牌命令
#define SUB_C_OPERATE_CARD			2									//操作扑克
#define SUB_C_CONTINUE_CARD			3									//继续命令

//出牌命令
struct CMD_C_OutCard
{
	BYTE							cbCardData;							//扑克数据
};

//操作命令
struct CMD_C_OperateCard
{
	BYTE							cbChiKind;							//吃牌类型
	BYTE							cbOperateCode;						//操作代码
};

//////////////////////////////////////////////////////////////////////////

#endif