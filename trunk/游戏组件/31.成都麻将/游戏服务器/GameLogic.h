#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//逻辑掩码

#define	MASK_COLOR					0xF0								//花色掩码
#define	MASK_VALUE					0x0F								//数值掩码

//////////////////////////////////////////////////////////////////////////
//动作定义

//动作标志
#define WIK_NULL					0x00								//没有类型
#define WIK_LEFT					0x01								//左吃类型
#define WIK_CENTER					0x02								//中吃类型
#define WIK_RIGHT					0x04								//右吃类型
#define WIK_PENG					0x08								//碰牌类型
#define WIK_GANG					0x10								//杠牌类型
#define WIK_LISTEN					0x20								//吃牌类型
#define WIK_CHI_HU					0x40								//吃胡类型
#define WIK_ZI_MO					0x80								//自摸

//////////////////////////////////////////////////////////////////////////
//胡牌定义

//胡牌
#define CHK_NULL					0x00										//非胡类型
#define CHK_CHI_HU					0x01										//胡类型

#define CHR_QIANG_GANG				0x00000001									//抢杠
#define CHR_GANG_SHANG_PAO			0x00000002									//杠上炮
#define CHR_GANG_KAI				0x00000004									//杠上花
#define CHR_TIAN_HU					0x00000008									//天胡
#define CHR_DI_HU					0x00000010									//地胡
#define CHR_DA_DUI_ZI				0x00000020									//大对子
#define CHR_QING_YI_SE				0x00000040									//清一色
#define CHR_QI_XIAO_DUI				0x00000080									//暗七对
#define CHR_DAI_YAO					0x00000100									//带幺
#define CHR_JIANG_DUI				0x00000200									//将对
#define CHR_SHU_FAN					0x00000400									//素番
#define CHR_QING_DUI				0x00000800									//清对
#define CHR_LONG_QI_DUI				0x00001000									//龙七对
#define CHR_QING_QI_DUI				0x00002000									//清七对
#define CHR_QING_YAO_JIU			0x00004000									//清幺九
#define CHR_QING_LONG_QI_DUI		0x00008000									//清龙七对

//////////////////////////////////////////////////////////////////////////

//类型子项
struct tagKindItem
{
	BYTE							cbWeaveKind;						//组合类型
	BYTE							cbCenterCard;						//中心扑克
	BYTE							cbCardIndex[3];						//扑克索引
	BYTE							cbValidIndex[3];					//实际扑克索引
};

//组合子项
struct tagWeaveItem
{
	BYTE							cbWeaveKind;						//组合类型
	BYTE							cbCenterCard;						//中心扑克
	BYTE							cbPublicCard;						//公开标志
	WORD							wProvideUser;						//供应用户
};

//杠牌结果
struct tagGangCardResult
{
	BYTE							cbCardCount;						//扑克数目
	BYTE							cbCardData[4];						//扑克数据
};

//分析子项
struct tagAnalyseItem
{
	BYTE							cbCardEye;							//牌眼扑克
	bool                            bMagicEye;                          //牌眼是否是王霸
	BYTE							cbWeaveKind[4];						//组合类型
	BYTE							cbCenterCard[4];					//中心扑克
	BYTE                            cbCardData[4][4];                   //实际扑克
};

//////////////////////////////////////////////////////////////////////////


#define MASK_CHI_HU_RIGHT			0x0fffffff

/*
//	权位类。
//  注意，在操作仅位时最好只操作单个权位.例如
//  CChiHuRight chr;
//  chr |= (chr_zi_mo|chr_peng_peng)，这样结果是无定义的。
//  只能单个操作:
//  chr |= chr_zi_mo;
//  chr |= chr_peng_peng;
*/
class CChiHuRight
{	
	//静态变量
private:
	static bool						m_bInit;
	static DWORD					m_dwRightMask[MAX_RIGHT_COUNT];

	//权位变量
private:
	DWORD							m_dwRight[MAX_RIGHT_COUNT];

public:
	//构造函数
	CChiHuRight();

	//运算符重载
public:
	//赋值符
	CChiHuRight & operator = ( DWORD dwRight );

	//与等于
	CChiHuRight & operator &= ( DWORD dwRight );
	//或等于
	CChiHuRight & operator |= ( DWORD dwRight );

	//与
	CChiHuRight operator & ( DWORD dwRight );
	CChiHuRight operator & ( DWORD dwRight ) const;

	//或
	CChiHuRight operator | ( DWORD dwRight );
	CChiHuRight operator | ( DWORD dwRight ) const;

	//功能函数
public:
	//是否权位为空
	bool IsEmpty();

	//设置权位为空
	void SetEmpty();

	//获取权位数值
	BYTE GetRightData( DWORD dwRight[], BYTE cbMaxCount );

	//设置权位数值
	bool SetRightData( const DWORD dwRight[], BYTE cbRightCount );

private:
	//检查权位是否正确
	bool IsValidRight( DWORD dwRight );
};


//////////////////////////////////////////////////////////////////////////

//数组说明
typedef CArrayTemplate<tagAnalyseItem,tagAnalyseItem &> CAnalyseItemArray;

//游戏逻辑类
class CGameLogic
{
	//变量定义
protected:
	static const BYTE				m_cbCardDataArray[MAX_REPERTORY];	//扑克数据
	BYTE							m_cbMagicIndex;						//钻牌索引

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
	//删除扑克
	bool RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard);
	//删除扑克
	bool RemoveCard(BYTE cbCardIndex[MAX_INDEX], const BYTE cbRemoveCard[], BYTE cbRemoveCount);
	//删除扑克
	bool RemoveCard(BYTE cbCardData[], BYTE cbCardCount, const BYTE cbRemoveCard[], BYTE cbRemoveCount);
	//设置钻牌
	void SetMagicIndex( BYTE cbMagicIndex ) { m_cbMagicIndex = cbMagicIndex; }
	//钻牌
	bool IsMagicCard( BYTE cbCardData );

	//辅助函数
public:
	//有效判断
	bool IsValidCard(BYTE cbCardData);
	//扑克数目
	BYTE GetCardCount(const BYTE cbCardIndex[MAX_INDEX]);
	//组合扑克
	BYTE GetWeaveCard(BYTE cbWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[4]);

	//等级函数
public:
	//动作等级
	BYTE GetUserActionRank(BYTE cbUserAction);
	//胡牌等级
	WORD GetChiHuActionRank(const CChiHuRight & ChiHuRight);

	//动作判断
public:
	//吃牌判断
	BYTE EstimateEatCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);
	//碰牌判断
	BYTE EstimatePengCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);
	//杠牌判断
	BYTE EstimateGangCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);

	//动作判断
public:
	//杠牌分析
	BYTE AnalyseGangCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, tagGangCardResult & GangCardResult);
	//吃胡分析
	BYTE AnalyseChiHuCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight);
	//是否听牌
	bool IsTingCard( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount );
	//是否花猪
	bool IsHuaZhu( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount );

	//转换函数
public:
	//扑克转换
	BYTE SwitchToCardData(BYTE cbCardIndex);
	//扑克转换
	BYTE SwitchToCardIndex(BYTE cbCardData);
	//扑克转换
	BYTE SwitchToCardData(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT]);
	//扑克转换
	BYTE SwitchToCardIndex(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardIndex[MAX_INDEX]);

	//胡法分析
protected:
	//大对子
	bool IsPengPeng( const tagAnalyseItem *pAnalyseItem );
	//清一色牌
	bool IsQingYiSe(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount,const BYTE cbCurrentCard);
	//七小对牌
	bool IsQiXiaoDui(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[],const  BYTE cbWeaveCount,const BYTE cbCurrentCard);
	//带幺
	bool IsDaiYao( const tagAnalyseItem *pAnalyseItem );
	//将对
	bool IsJiangDui( const tagAnalyseItem *pAnalyseItem );

	//内部函数
private:
	//分析扑克
	bool AnalyseCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbItemCount, CAnalyseItemArray & AnalyseItemArray);
	//排序,根据牌值排序
	bool SortCardList( BYTE cbCardData[MAX_COUNT], BYTE cbCardCount );
};

//////////////////////////////////////////////////////////////////////////

#endif
