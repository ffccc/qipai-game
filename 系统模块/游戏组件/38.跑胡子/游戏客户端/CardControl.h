#ifndef CARD_CONTROL_HEAD_FILE
#define CARD_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//宏定义

//公共定义
#define POS_SHOOT					10									//弹起象素
#define ITEM_COUNT					21									//子项数目
#define CARD_SPACE					18									//扑克间距
#define INVALID_ITEM				0xFFFF								//无效索引

//扑克大小
#define CARD_WIDTH					28									//扑克宽度
#define CARD_HEIGHT					80									//扑克高度
#define CARD_TABLE_WIDTH			20									//扑克宽度
#define CARD_TABLE_HEIGHT			55									//扑克高度

//////////////////////////////////////////////////////////////////////////

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

//方向枚举
enum enDirection
{
	Direction_East,					//东向
	Direction_South,				//南向
	Direction_West,					//西向
	Direction_North,				//北向
};

//////////////////////////////////////////////////////////////////////////

//扑克图片
class CCardListImage
{
	//位置变量
protected:
	int								m_nItemWidth;						//子项高度
	int								m_nItemHeight;						//子项宽度

	//资源变量
protected:
	CSkinImage						m_CardListImage;					//图片资源

	//函数定义
public:
	//构造函数
	CCardListImage();
	//析构函数
	virtual ~CCardListImage();

	//信息函数
public:
	//获取宽度
	int GetItemWidth() { return m_nItemWidth; }
	//获取高度
	int GetItemHeight() { return m_nItemHeight; }

	//资源管理
public:
	//加载资源
	bool LoadResource(UINT uResourceID, int nItemWidth, int nItemHeight);
	//释放资源
	bool DestroyResource();

	//功能函数
public:
	//获取位置
	inline int GetImageIndex(BYTE cbCardData);
	//绘画扑克
	inline bool DrawCardItem(CDC * pDestDC, BYTE cbCardData, int xDest, int yDest);
};

//////////////////////////////////////////////////////////////////////////

//扑克资源
class CCardResource
{
	//用户扑克
public:
	CCardListImage					m_ImageUserTop;						//图片资源
	CCardListImage					m_ImageUserLeft;					//图片资源
	CCardListImage					m_ImageUserRight;					//图片资源
	CCardListImage					m_ImageUserBottom;					//图片资源

	//桌面扑克
public:
	CCardListImage					m_ImageTableTop;					//图片资源
	CCardListImage					m_ImageTableLeft;					//图片资源
	CCardListImage					m_ImageTableRight;					//图片资源
	CCardListImage					m_ImageTableBottom;					//图片资源

	//函数定义
public:
	//构造函数
	CCardResource();
	//析构函数
	virtual ~CCardResource();

	//功能函数
public:
	//加载资源
	bool LoadResource();
	//消耗资源
	bool DestroyResource();
};

//////////////////////////////////////////////////////////////////////////

//丢弃扑克
class CDiscardCard
{
	//扑克数据
protected:
	WORD							m_wCardCount;						//扑克数目
	BYTE							m_cbCardData[16];					//扑克数据

	//控制变量
protected:
	CPoint							m_ControlPoint;						//基准位置
	enDirection						m_CardDirection;					//扑克方向

	//函数定义
public:
	//构造函数
	CDiscardCard();
	//析构函数
	virtual ~CDiscardCard();

	//功能函数
public:
	//绘画扑克
	void DrawCardControl(CDC * pDC);
	//增加扑克
	bool AddCardItem(BYTE cbCardData);
	//设置扑克
	bool SetCardData(const BYTE cbCardData[], WORD wCardCount);

	//控件控制
public:
	//设置方向
	void SetDirection(enDirection Direction) { m_CardDirection=Direction; }
	//基准位置
	void SetControlPoint(int nXPos, int nYPos) { m_ControlPoint.SetPoint(nXPos,nYPos); }
};

//////////////////////////////////////////////////////////////////////////

//桌面扑克
class CUserCard
{
	//扑克数据
protected:
	WORD							m_wCardCount;						//扑克数目
	BYTE							m_cbCardData[21];					//扑克数据

	//控制变量
protected:
	CPoint							m_ControlPoint;						//基准位置
	enDirection						m_CardDirection;					//扑克方向

	//函数定义
public:
	//构造函数
	CUserCard();
	//析构函数
	virtual ~CUserCard();

	//功能函数
public:
	//绘画扑克
	void DrawCardControl(CDC * pDC);

	//功能函数
public:
	//设置扑克
	bool SetCardData(WORD wCardCount);
	//扑克数目
	WORD GetCardCount() { return m_wCardCount; }
	//设置扑克
	bool SetCardData(const BYTE cbCardData[], WORD wCardCount);

	//控件控制
public:
	//设置方向
	void SetDirection(enDirection Direction) { m_CardDirection=Direction; }
	//基准位置
	void SetControlPoint(int nXPos, int nYPos) { m_ControlPoint.SetPoint(nXPos,nYPos); }
};

//////////////////////////////////////////////////////////////////////////

//组合扑克
class CWeaveCard
{
	//控制变量
protected:
	CPoint							m_ControlPoint;						//基准位置
	enDirection						m_CardDirection;					//扑克方向

	//扑克数据
protected:
	WORD							m_wCardCount;						//扑克数目
	BYTE							m_cbCardData[4];					//扑克数据
	bool                            m_bDisplayStyle;					//显示方式

	//函数定义
public:
	//构造函数
	CWeaveCard();
	//析构函数
	virtual ~CWeaveCard();

	//功能函数
public:
	//绘画扑克
	void DrawCardControl(CDC * pDC);
	//设置扑克
	bool SetCardData(const tagWeaveItem & WeaveItem);

	//控件控制
public:
	//设置方向
	void SetDirection(enDirection Direction) { m_CardDirection=Direction; }
	//基准位置
	void SetControlPoint(int nXPos, int nYPos) { m_ControlPoint.SetPoint(nXPos,nYPos); }
	// 显示方式
	void SetDisplayStyle(bool bDisplayStyle);
};

//////////////////////////////////////////////////////////////////////////

//扑克结构
struct tagCardItem
{
	bool							bShoot;								//弹起标志
	bool							bEnable;							//禁止标志
	BYTE							cbCardData;							//扑克数据
};

//扑克控件
class CCardControl
{
	//状态变量
protected:
	bool							m_bPositively;						//响应标志
	bool							m_bDisplayItem;						//显示标志

	//位置变量
protected:
	CPoint							m_BenchmarkPos;						//基准位置
	enXCollocateMode				m_XCollocateMode;					//显示模式
	enYCollocateMode				m_YCollocateMode;					//显示模式

	//扑克数据
protected:
	WORD							m_wHoverItem;						//盘旋子项
	WORD							m_wCardCount;						//扑克数目
	tagCardItem						m_CardItemArray[MAX_COUNT];			//扑克数据

	//资源变量
protected:
	CSize							m_ControlSize;						//控件大小
	CPoint							m_ControlPoint;						//控件位置

	//函数定义
public:
	//构造函数
	CCardControl();
	//析构函数
	virtual ~CCardControl();

	//查询函数
public:
	//获取扑克
	BYTE GetHoverCard();
	//索引切换
	WORD SwitchCardPoint(CPoint & MousePoint);
	//响应查询
	bool IsPositively() { return m_bPositively; }

	//扑克控制
public:
	//获取扑克
	WORD GetCardData(BYTE cbCardData[], WORD wMaxCount);
	//交换扑克
	bool SwitchCardItem(WORD wSourceItem, WORD wTargerItem);
	//设置扑克
	bool SetCardData(const BYTE cbCardData[], WORD wCardCount);
	//设置扑克
	bool SetCardItem(const tagCardItem CardItemArray[], WORD wCardCount);

	//控件控制
public:
	//设置响应
	void SetPositively(bool bPositively) { m_bPositively=bPositively; }
	//设置显示
	void SetDisplayItem(bool bDisplayItem) { m_bDisplayItem=bDisplayItem; }
	//基准位置
	void SetBenchmarkPos(int nXPos, int nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);
	//基准位置
	void SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);

	//事件控制
public:
	//绘画扑克
	void DrawCardControl(CDC * pDC);
	//光标消息
	bool OnEventSetCursor(CPoint Point, bool & bRePaint);
};

//////////////////////////////////////////////////////////////////////////

//变量声明
extern CCardResource				g_CardResource;						//扑克资源

//////////////////////////////////////////////////////////////////////////

#endif