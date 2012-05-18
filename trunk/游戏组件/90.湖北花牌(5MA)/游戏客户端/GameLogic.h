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
	BYTE							cbWeaveKind;						//组合类型
	BYTE							cbCenterCard;						//中心扑克索引
	BYTE							cbCardIndex[5];						//扑克索引
	BYTE                            cbCardCount;                        //扑克数量
};


//组合子项
struct tagWeaveItem
{
	BYTE							cbWeaveKind;						//组合类型
	BYTE							cbCenterCard;						//中心扑克
	BYTE							cbPublicCard;						//公开标志
	WORD							wProvideUser;						//供应用户
};

//胡牌结果
struct tagChiHuResult
{
	WORD							wChiHuKind;							//吃胡类型
	WORD							wChiHuRight;						//胡牌权位
	WORD							bChiHuUser;							//吃胡玩家
};

//四统的结果
struct tagSiTongResult
{
	BYTE							cbCardCount;						//扑克数目
	BYTE							cbCardData[6];						//扑克数据
};

//组合牌中的藏牌
struct tagWeaveTongResult
{
	BYTE							cbCardCount;						//扑克数目
	BYTE							cbCardData[8];						//扑克数据
};
//五统的结果
struct tagWuTongResult
{
	BYTE							cbCardCount;						//扑克数目
	BYTE							cbCardData[5];						//扑克数据
};

//杠牌结果
struct tagGangCardResult
{
	BYTE                            cbMagicCard;                       //赖子牌
	BYTE							cbCardCount;						//扑克数目
	BYTE							cbCardData[6];						//扑克数据
};

//补牌结果
struct tagBuResult
{
	BYTE							cbCardCount;						//扑克数目
	BYTE							cbCardData[6];						//扑克数据
};

//分析子项
struct tagAnalyseItem
{
	BYTE							cbCardEye[2];						//丫口索引
	BYTE							cbWeaveKind[8];						//组合类型
	BYTE							cbCenterCard[8];					//中心扑克
	BYTE							cbGoldPoint[5];					    //保存五种主金计算出来的分数

	BYTE							cbKindPoint[8];	     				//每一种胡牌组合中每一个组合项的点数
	WORD                            cbPoint;                            //组合牌的最佳点数
	BYTE                            cbGoldCard;                         //选定的主金
};

//胡牌结果
struct tagHuCardResult
{
	bool                            IsHu;                               //是否可以胡牌
	BYTE                            HuScore;                            //胡牌的分数
	WORD                            bHuPoint;                          //胡点
	BYTE                            bRealGold;                        //主金 
	tagAnalyseItem                  AnalyseItem;                       //胡牌时最佳组合
};
//最佳胡牌组合
struct tagHuCardWeave
{
	BYTE               cbCardData[8][5];            //八组最佳组合的牌数据
	BYTE               cbCardCount[8];              //八组牌，每组的牌个数
	BYTE               cbYaKou[2];                  //两张丫口的牌数据
};
//数组说明
typedef CArrayTemplate<tagAnalyseItem,tagAnalyseItem &> CAnalyseItemArray;

//游戏逻辑类
class CGameLogic
{
	//变量定义
protected:
	static const BYTE				m_cbCardDataArray[MAX_REPERTORY];	//扑克数据

	//函数定义
public:
	//构造函数
	CGameLogic();
	//析构函数
	virtual ~CGameLogic();

	//控制函数
public:
	//删除扑克
	bool RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard);
	//删除扑克
	bool RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard[], BYTE cbRemoveCount);

	//辅助函数
public:
	//有效判断
	bool IsValidCard(BYTE cbCardData);

	//组合扑克
	BYTE GetWeaveCard(BYTE cbWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[],BYTE cbHuaCount);

	//动作判断
public:
	//得到胡牌组合数据
	BYTE GetHuCardWeave(BYTE cbWeaveKind, BYTE cbCenterCardIndex, BYTE cbCardBuffer[],BYTE cbHuaCount);

	bool IsYaKou(BYTE cbCardIndex[MAX_INDEX],BYTE &FirstIndex,BYTE &SecondeIndex);

	//根据扑克牌数据，得到花牌保存到索引数组中
	//                  扑克牌数据        牌数             花牌索引数组
	BYTE GetHuaCard(BYTE cbCardData[],BYTE bCardCount, BYTE cbHuaCardIndex[]);
	
	//将给定的牌数据数组中，是花牌的保存到花牌索引数组中
	void SaveHuaCard(BYTE cbCardData[],BYTE bCardCount,BYTE cbHuaCardIndex[]);

	BYTE  AnalyseSiTong(BYTE cbCardIndex[MAX_INDEX],  tagSiTongResult & SiTongResult);

	//分析五统
	BYTE  AnalyseWuTong(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount,tagWuTongResult & WuTongResult);
	
	//转换函数
public:
	//扑克转换
	BYTE SwitchToCardData(BYTE cbCardIndex);
	//扑克转换
	BYTE SwitchToCardIndex(BYTE cbCardData);
	//扑克转换
	BYTE SwitchToCardData(BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT], BYTE cbHuaCardIndex[]);
	//扑克转换
	BYTE SwitchToCardData(BYTE cbCardIndex[], BYTE cbCardCount, BYTE cbCardData[], BYTE cbHuaCardIndex[]);
	//扑克转换
	BYTE SwitchToCardIndex(BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardIndex[MAX_INDEX]);
};

//////////////////////////////////////////////////////////////////////////

#endif