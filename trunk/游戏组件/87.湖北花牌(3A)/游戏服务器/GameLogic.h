#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"
//////////////////////////////////////////////////////////////////////////
//常量定义

#define MAX_WEAVE					4									//最大组合
#define MAX_INDEX					22									//最大索引
#define MAX_COUNT					42									//最大数目
#define MAX_REPERTORY				110									//最大库存

//////////////////////////////////////////////////////////////////////////
//逻辑掩码

#define	MASK_COLOR					0xF0								//花色掩码
#define	MASK_VALUE					0x0F								//数值掩码

//////////////////////////////////////////////////////////////////////////
//动作定义

////////////////////////////////////babala////////////////////////////
//动作标志
#define WIK_NULL					0x00								//没有类型
#define WIK_DUI 					0x01								//对牌类型
#define WIK_SAN_ZHAO				0x02								//三招一类型
#define WIK_SI_ZHAO			    	0x04								//四招一类型
#define WIK_TONG_GIVE_UP	    	0x08					        	//第一轮的情况下，点击弃按钮向服务器发送的操作码

#define WIK_SI_TONG     	    	0x10								//四统类型
#define WIK_WU_TONG					0x20								//五统类型
#define WIK_QI_TA			    	0x40					        	//三个连一起的类型
#define WIK_CHI_HU					0x80							   //胡牌类型

//////////////////////////////////////////////////////////////////////////

//动作标志
#define WIK_LEFT					0x21								//左吃类型
#define WIK_CENTER					0x22								//中吃类型
#define WIK_RIGHT					0x24								//右吃类型
#define WIK_PENG					0x28								//碰牌类型
#define WIK_FILL					0x23								//补牌类型
#define WIK_GANG					0x34								//杠牌类型
//#define WIK_CHI_HU					0x40								//吃胡类型
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//胡牌定义

//小胡牌型
#define CHK_NULL					0x00000							     //非胡类型
#define CHK_PING_HU		        	0x00001								//平胡类型

//半大胡
#define CHK_QUAN_QIU_REN			0x00010								//全求人胡牌，
#define CHK_MEN_QIAN_QING			0x00020								//门前清
#define CHK_JUE_ZHANG  				0x00040								//绝张

//不需将的胡牌类型
#define CHK_PENG_PENG				0x01000								//碰碰胡牌
#define CHK_QING_YI_SE				0x02000								//清色类型，包括了万一色，条一色，筒一色，
#define CHK_FENG_YI_SE				0x04000								//风一色，乱将
#define CHK_JIANG_JIANG				0x08000								//将将胡牌，即将一色
#define CHK_QI_DUI  				0x00100								//七对
#define CHK_HAO_HUA_QI_DUI  	    0x00200								//豪华七对

//需将的胡牌类型
#define CHK_QIANG_GANG				0x10000								//抢杠
#define CHK_HAI_DI					0x20000								//海底捞月
#define CHK_GANG_SHANG_KAI_HUA      0x40000                              //杠上开花

//乱将权位
#define CHR_QING_YI_SE				0x0100								//清色权位，包括了万一色，条一色，筒一色，将一色
#define CHR_QUAN_QIU_REN			0x0200								//全求权位
#define CHR_MEN_QIAN_QIN			0x0400			                	//门前清权位
#define CHR_JUE_ZHANG               0x0800				                //绝张权位

#define CHR_QIANG_GANG				0x1000								//抢杆权位
#define CHR_HAI_DI					0x2000								//海底权位
#define CHR_GANG_SHANG_KAI_HUA      0x4000                              //杠上开花权位

//类型子项
struct tagKindItem
{
	BYTE				cbWeaveKind;						//组合类型
	BYTE				cbCenterCard;						//中心扑克索引
	BYTE				cbCardIndex[5];						//扑克索引
	BYTE                cbCardCount;                        //扑克数量
};

//组合子项
struct tagWeaveItem
{
	BYTE				cbWeaveKind;						//组合类型
	BYTE				cbCenterCard;						//中心扑克
	BYTE				cbPublicCard;						//公开标志
	WORD				wProvideUser;						//供应用户
};

//胡牌结果
struct tagChiHuResult
{
	WORD				wChiHuKind;							//吃胡类型
	WORD				wChiHuRight;						//胡牌权位
	WORD				bChiHuUser;							//吃胡玩家
};

//藏牌的结果
struct tagTongResult
{
	BYTE				cbCardCount;						//藏牌的次数
	BYTE				cbCardData[6];						//每次所对应的中心牌的数据
};

struct tagChiChuItem
{
	BYTE                 cbCenterCard[8];               //8种组合的中心牌值
	BYTE                 cbWeaveKind[8];                //8种组合的操作类型
	BYTE                 cbYaKou[2];                    //一对丫口的值
	BYTE                 cbHuScore;                     //胡牌的分数
	BYTE                 cbGoldCard;                    //胡牌的主精牌
};

//分析子项
struct tagAnalyseItem
{
	BYTE				 cbCardEye[2];						//丫口索引
	BYTE				 cbWeaveKind[8];						//组合类型
	BYTE				 cbCenterCard[8];					//中心扑克
	WORD                 cbPoint;                            //组合牌的最佳点数
	BYTE                 cbGoldCard;                         //选定的主金

	//调试时用的信息
	BYTE				 cbGoldPoint[3];					    //保存五种主金计算出来的分数
	BYTE				 cbKindPoint[8];	     				//每一种胡牌组合中每一个组合项的点数
};

//胡牌结果
struct tagHuCardResult
{
	bool                            IsHu;                               //是否可以胡牌
	BYTE                            HuScore;                            //胡牌的分数
	WORD                            bHuPoint;                           //胡点
	BYTE                            bRealGold;                          //主金 
	tagAnalyseItem                  AnalyseItem;                        //胡牌时最佳组合
};

//////////////////////////////////////////////////////////////////////////
//数组说明
typedef CArrayTemplate<tagAnalyseItem,tagAnalyseItem &> CAnalyseItemArray;

//打印调试信息类的说明
//基本的设计思想是：一次将相关的信息打印到指定的客户端
//使用的过程：
//1、调用BeginPrint函数，清空字符串中的信息
//2、调用多个相关的打印函数，将相关的信息保存到字符串中
//3、调用EndPrint函数，将字符串中的信息发送到客户端
class CDebugMessage
{
private:
	//保存有花牌字面的相关信息
	static CString m_strCardsMessage[22];
	//游戏桌子框架对象，用来发送保存的消息到客户端
	ITableFrame		*m_pITableFrame;
	//保存本轮打印的信息
	CString m_strMessage;
	////在本轮打印过程中，用来进行锁定。只允许一个用户进行打印
	//CThreadLock m_Lock;
public:
	//构造函数与析构函数
	CDebugMessage();
	~CDebugMessage();

public:
	//初始化打印对象中的输出对象
	void InitDebug(ITableFrame	*pITableFrame);

	//开始打印调试信息
	void BeginPrint();
	//结束打印调度信息
	void EndPrint(WORD wChairID);

	//打印用户手中的牌信息
	void PrintCardMessage(BYTE cbCardIndex[MAX_INDEX]);

	//打印用户组合的牌信息
	void PrintWeaveItemsMessage(tagWeaveItem WeaveItem[], BYTE cbWeaveCount);

	//打印用户手中牌的所有组合信息
	void PrintKindItemsMessage(tagKindItem KindItem[],BYTE cbKindItemCount);

	//打印用户手中牌合法的组合信息
	void PrintValidKindItemsMessage(CAnalyseItemArray & AnalyseItemArray);

	//打印调整之前的组合项的点数、手中牌的总点数、组合项的总点数
	void PrintValidKindItemPointBeforeMessage(tagAnalyseItem *pAnalyseItem,BYTE cbWeavePoint,BYTE cbHandPoint);

	//打印调整之后的组合项的点数、手中牌的总点数、组合项的总点数
	void PrintValidKindItemPointAfterMessage(tagAnalyseItem *pAnalyseItem,BYTE cbWeavePoint,BYTE cbHandPoint);
	
	//打印最佳组合的信息
	void PrintHuCardMessage(tagAnalyseItem *pAnalyseItem);

private:
	BYTE SwitchToCardIndex(BYTE cbCardData);
};

////////////////////////////////////////////////////////////

//游戏逻辑类
class CGameLogic
{
	//变量定义
protected:
	static const BYTE				m_cbCardDataArray[MAX_REPERTORY];	//扑克数据

public:
	CDebugMessage m_Debug;

	//函数定义
public:
	//构造函数
	CGameLogic();
	//析构函数
	virtual ~CGameLogic();

	//控制函数
public:
	//混乱扑克
	void RandCardData(BYTE cbCardData[], BYTE cbMaxCount);

	//辅助函数
public:
	//有效判断
	bool IsValidCard(BYTE cbCardData);
	//组合扑克
	BYTE GetWeaveCard(BYTE cbWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[]);

	//等级函数
public:

	//////////////babala
	//得到扑克牌等级
	BYTE GetCardRank(BYTE cbCardData);

	////////////////babala 11_15///////////////

	//动作等级
	BYTE GetUserActionRank(BYTE cbUserAction);
	//将给定的牌数据数组中，是花牌的保存到花牌索引数组中
	void SaveHuaCard(BYTE cbCardData[],BYTE bCardCount,BYTE cbHuaCardIndex[]);
	////////////////babala 11_15
	//动作判断
public:
	////////////////babala///////////////
	BYTE EstimateDuiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbSiTong[],BYTE cbSiTongCount,BYTE cbCurrentCard);

	BYTE EstimateZhaoCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbSiTong[],BYTE cbSiTongCount, BYTE cbCurrentCard);

	//动作判断
public:
	//babala 11-31
	//求绝对值
	BYTE Abs(BYTE cbFirst,BYTE cbSecond);
	//bool IsYaKou(const BYTE cbCardIndex[],BYTE &FirstIndex,BYTE &SecondeIndex);
	bool IsYaKou(BYTE cbCardIndex[],BYTE &FirstIndex,BYTE &SecondeIndex);

	//根据扑克牌数据，得到花牌保存到索引数组中
	//                  扑克牌数据        牌数             花牌索引数组
	BYTE GetHuaCard(BYTE cbCardData[],BYTE bCardCount, BYTE cbHuaCardIndex[]);
	/////////////////////////////////////分析到这儿了/////////////////////////////////

	//分析四统             扑克牌数据               扑克个数             四统结果     已经存在的四统牌
	BYTE  AnalyseSiTong(BYTE cbCardIndex[MAX_INDEX],  tagTongResult &SiTongResult);

	//分析五统
	BYTE  AnalyseWuTong(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount,tagTongResult &WuTongResult);

	//分析胡牌
	BYTE AnalyseHuCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount,BYTE cbCurrentCard, BYTE HuaCardInex[], BYTE WeavHuaIndex[],tagHuCardResult &ChiHuResult);
	
	//分析组合牌中的藏牌
	BYTE AnalyseWeaveTong(BYTE cbCardIndex[MAX_INDEX],tagWeaveItem WeaveItem[],BYTE cbItemCount,tagTongResult &WeaveTongResult);
	////////////////////babala////////////////////////////////////

	//转换函数
public:
	//扑克转换
	BYTE SwitchToCardData(BYTE cbCardIndex);
	//扑克转换
	BYTE SwitchToCardIndex(BYTE cbCardData);
	//扑克转换
	BYTE SwitchToCardData(BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT], BYTE cbHuaCardIndex[]);
	//扑克转换
	BYTE SwitchToCardIndex(BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardIndex[MAX_INDEX]);

	//内部函数
private:
	//给定一个操作类型和操作中心牌，得到三张牌的索引
	BYTE GetWeaveIndex(BYTE cbWeaveKind,BYTE cbCenterCardIndex,BYTE cbCardIndex[]);
	//校正胡点,传进一个牌数据，和一个牌索引
	bool CheckHuPoint(BYTE cbCardData,BYTE cbCardIndex);
	//分析扑克
	bool AnalyseCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount, CAnalyseItemArray & AnalyseItemArray);
};

//////////////////////////////////////////////////////////////////////////

#endif