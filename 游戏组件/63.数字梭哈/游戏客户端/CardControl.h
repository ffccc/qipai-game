#ifndef CARD_CONTROL_HEAD_FILE
#define CARD_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//宏定义

//属性定义
#define MAX_CARD_COUNT				5									//扑克数目
#define DEF_X_DISTANCE				17									//默认间距

//数值掩码
#define	CARD_MASK_COLOR				0xF0								//花色掩码
#define	CARD_MASK_VALUE				0x0F								//数值掩码

#define MAX_MOVE_Y					40
#define MOVE_STEP_X					5
#define MOVE_STEP_Y					3

//////////////////////////////////////////////////////////////////////////
//枚举定义

//X 排列方式
enum enXCollocateMode 
{ 
	enXLeft,						//左对齐
	enXCenter,						//中对齐
	enXRight,						//右对齐
};

//Y 排列方式
enum enYCollocateMode 
{ 
	enYTop,							//上对齐
	enYCenter,						//中对齐
	enYBottom,						//下对齐
};

//
struct tagCardItem
{
	BYTE cbCardData;
	bool bDisplay;
};

//////////////////////////////////////////////////////////////////////////

//扑克控件
class CCardControl
{
	//状态变量
protected:
	bool							m_bPositively;						//响应标志
	bool							m_bDisplayHead;						//显示标志

	//动画变量
protected:
	bool							m_bMoving;							//
	INT								m_nYStep;							//
	INT								m_nYMove;							//
	BYTE							m_cbShowIndex;						//

	//扑克数据
protected:
	WORD							m_wCardCount;						//扑克数目
	tagCardItem						m_CardItem[MAX_CARD_COUNT];			//扑克数据

	//间隔变量
protected:
	INT								m_nXDistance;						//横向间隔
	INT								m_nXOrgDistance;					//

	//位置变量
protected:
	CPoint							m_BenchmarkPos;						//基准位置
	enXCollocateMode				m_XCollocateMode;					//显示模式
	enYCollocateMode				m_YCollocateMode;					//显示模式

	//资源变量
protected:
	CSize							m_CardSize;							//扑克大小
	CSkinImage						m_ImageCard;						//图片资源

	//函数定义
public:
	//构造函数
	CCardControl();
	//析构函数
	virtual ~CCardControl();

	//扑克控制
public:
	//扑克数目
	WORD GetCardCount() { return m_wCardCount; }
	//获取扑克
	WORD GetCardData(BYTE cbCardData[], WORD wBufferCount);
	//设置扑克
	bool SetCardData(const BYTE cbCardData[], WORD wCardCount);

	//状态查询
public:
	//获取大小
	CSize GetCardSize() { return m_CardSize; }
	//查询响应
	bool GetPositively() { return m_bPositively; }
	//查询显示
	bool GetDisplayHead() { return m_bDisplayHead; }

	//状态控制
public:
	//设置距离
	VOID SetXDistance(UINT nXDistance);
	//设置响应
	VOID AllowPositively(bool bPositively);
	//设置显示
	VOID ShowFirstCard(bool bDisplayHead);
	//获取牌尾位置
	CPoint GetTailPos();
	//
	bool BeginMoveCard();
	//
	bool PlayMoveCard();
	//
	void SetDispalyCard( WORD wCardIndex,bool bDisplay );
	
	//控件控制
public:
	//基准位置
	VOID SetBasicStation(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);

	//事件控制
public:
	//绘画扑克
	VOID DrawCardControl(CDC * pDC);
	//光标消息
	bool OnEventSetCursor(CPoint Point);
	//鼠标消息
	bool OnEventLeftMouseDown(CPoint Point);
	//
	bool OnEventLeftMouseUp(CPoint Point);

	//内部函数
private:
	//获取大小
	VOID GetControlSize(CSize & ControlSize);
	//索引切换
	WORD SwitchCardPoint(CPoint & MousePoint);
	//获取原点
	VOID GetOriginPoint(CPoint & OriginPoint);
};

//////////////////////////////////////////////////////////////////////////

#endif
