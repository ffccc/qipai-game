#ifndef TABLE_VIEW_FRAME_HEAD_FILE
#define TABLE_VIEW_FRAME_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "GamePlazaHead.h"

//////////////////////////////////////////////////////////////////////////////////

//进入索引
#define INDEX_ENTER_CHAIR			MAX_CHAIR							//进入区域

//桌子属性
struct tagTableAttribute
{
	//桌子标志
	bool							bLocker;							//密码标志
	bool							bPlaying;							//游戏标志
	bool							bFocusFrame;						//框架标志

	//桌子状态
	WORD							wWatchCount;						//旁观数目
	DWORD							dwTableOwnerID;						//桌主索引

	//属性变量
	WORD							wTableID;							//桌子号码
	WORD							wChairCount;						//椅子数目
	IClientUserItem *				pIClientUserItem[MAX_CHAIR];		//用户信息
};

//////////////////////////////////////////////////////////////////////////////////

//桌子资源
class CTableResource
{
	//桌子属性
public:
	CSize							m_SizeChair;						//椅子尺寸
	CSize							m_SizeTable;						//桌子尺寸
	CSize							m_SizeEnter;						//进入尺寸

	//属性变量
public:
	bool							m_bShowUser;						//显示用户
	bool							m_bShowEnter;						//显示进入
	bool							m_bShowChair;						//显示椅子
	bool							m_bShowTable;						//显示桌子
	bool							m_bShowFocus;						//显示焦点
	bool							m_bCustomUser;						//自定用户
	bool							m_bShowTableID;						//显示桌号
	bool							m_bRenderImage;						//渲染标志

	//数目变量
public:
	WORD							m_wTableItemCount;					//子项数目
	WORD							m_wChairItemCount;					//子项数目

	//类型数目
public:
	WORD							m_wUserGenreCount;					//类型数目
	WORD							m_wTableGenreCount;					//类型数目
	WORD							m_wChairGenreCount;					//类型数目

	//位置属性
public:
	CPoint							m_ptLock;							//桌锁位置
	CPoint							m_ptEnter;							//进入位置
	CPoint							m_ptTableID;						//桌号位置
	CPoint							m_ptReadyArray[MAX_CHAIR];			//准备位置

	//框架位置
public:
	CRect							m_rcNameArray[MAX_CHAIR];			//名字位置
	CRect							m_rcChairArray[MAX_CHAIR];			//椅子位置

	//颜色属性
public:
	COLORREF						m_crName;							//名字颜色
	COLORREF						m_crMember;							//会员颜色
	COLORREF						m_crMaster;							//管理颜色

	//辅助变量
public:
	INT								m_nDrawStyle[MAX_CHAIR];			//输出格式
	TCHAR							m_szDirectory[MAX_PATH];			//资源路径

	//外部资源
public:
	CPngImage						m_ImageEnter;						//进入位图
	CBitImage						m_ImageChair;						//椅子位图
	CBitImage						m_ImageTable;						//桌子位图
	CBitImage						m_ImageGround;						//背景位图
	CPngImage						m_ImageCustomUser;					//自定位图

	//内部资源
public:
	CPngImage						m_ImageReady;						//准备位图
	CPngImage						m_ImageLocker;						//桌锁位图
	CPngImage						m_ImageNumberNB;					//号码位图
	CPngImage						m_ImageNumberBK;					//号码背景

	//函数定义
public:
	//构造函数
	CTableResource();
	//析构函数
	virtual ~CTableResource();

	//功能函数
public:
	//释放资源
	VOID DestroyResource();
	//加载资源
	bool LoadResource(LPCTSTR pszResDirectory, WORD wChairCount);
	//更新资源
	bool UpdateResource(CSkinRenderManager * pSkinRenderManager);

	//辅助函数
protected:
	//读取数值
	LONG ReadStringValue(LPCTSTR & pszSring);
	//读取矩形
	bool ReadRect(RECT & ValueRect, LPCTSTR pszItem, LPCTSTR pszSubItem, LPCTSTR pszIniFile);
	//读取坐标
	bool ReadPoint(POINT & ValuePoint, LPCTSTR pszItem, LPCTSTR pszSubItem, LPCTSTR pszIniFile);
	//读取颜色
	bool ReadColor(COLORREF & ValueColor, LPCTSTR pszItem, LPCTSTR pszSubItem, LPCTSTR pszIniFile);
};

//////////////////////////////////////////////////////////////////////////////////

//桌子视图
class CTableView : public ITableView
{
	//状态变量
protected:
	bool							m_bMouseDown;						//是否按下
	WORD							m_wHoverChairID;					//盘旋椅子
	tagTableAttribute				m_TableAttribute;					//桌子属性

	//组件接口
protected:
	ITableViewFrame *				m_pITableViewFrame;					//桌子接口

	//函数定义
public:
	//构造函数
	CTableView();
	//析构函数
	virtual ~CTableView();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//功能接口
public:
	//空椅子数
	virtual WORD GetNullChairCount(WORD & wNullChairID);
	//配置函数
	virtual VOID InitTableView(WORD wTableID, WORD wChairCount, ITableViewFrame * pITableViewFrame);

	//用户接口
public:
	//获取用户
	virtual IClientUserItem * GetClientUserItem(WORD wChairID);
	//设置信息
	virtual bool SetClientUserItem(WORD wChairID, IClientUserItem * pIClientUserItem);

	//状态接口
public:
	//鼠标按下
	virtual VOID SetMouseDown(bool bMouseDown);
	//焦点框架
	virtual VOID SetFocusFrame(bool bFocusFrame);
	//盘旋位置
	virtual VOID SetHoverStatus(WORD wHoverChairID);
	//桌子状态 
	virtual VOID SetTableStatus(bool bPlaying, bool bLocker);

	//查询接口
public:
	//游戏标志
	virtual bool GetPlayFlag() { return m_TableAttribute.bPlaying; }
	//密码标志
	virtual bool GetLockerFlag() { return m_TableAttribute.bLocker; }

	//功能函数
public:
	//绘画函数
	VOID DrawTableView(CDC * pDC, DWORD dwServerRule, CTableResource * pTableResource);
	//绘画名字
	VOID DrawTableName(CDC * pDC, WORD wChairID, LPCTSTR pszNickName, CTableResource * pTableResource);
};

//////////////////////////////////////////////////////////////////////////////////

//数组定义
typedef CWHArray<CTableView *>		CTableViewArray;					//桌子数组

//桌子框架
class CTableViewFrame : public CWnd, public ITableViewFrame
{
	//友元定义
	friend class CTableView;

	//状态信息
protected:
	bool							m_bHovering;						//盘旋标志
	bool							m_bLMouseDown;						//右键标志
	bool							m_bRMouseDown;						//左键标志

	//索引变量
protected:
	WORD							m_wDownTableID;						//桌子号码
	WORD							m_wDownChairID;						//椅子号码

	//属性变量
protected:
	WORD							m_wTableCount;						//游戏桌数
	WORD							m_wChairCount;						//椅子数目
	DWORD							m_dwServerRule;						//房间规则

	//滚动信息
protected:
	INT								m_nXExcursion;						//横行偏移
	INT								m_nYExcursion;						//竖向偏移
	INT								m_nXTableCount;						//桌子列数
	INT								m_nYTableCount;						//桌子行数

	//滚动信息
protected:
	INT								m_nScrollPos;						//滚动位置
	INT								m_nScrollPosMax;					//最大位置
	INT								m_nVisibleLineCount;				//可见行数

	//控件变量
protected:
	CSkinScrollBar					m_SkinScrollBar;					//滚动条类
	CTableResource					m_TableResource;					//桌子资源
	CTableViewArray					m_TableViewArray;					//桌子数组

	//组件接口
protected:
	ITableViewFrameSink *			m_pITableViewFrameSink;				//通知接口

	//函数定义
public:
	//构造函数
	CTableViewFrame();
	//析构函数
	virtual ~CTableViewFrame();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { return; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//配置接口
public:
	//创建桌子
	virtual bool CreateTableFrame(CWnd * pParentWnd, UINT uWndID, IUnknownEx * pIUnknownEx);
	//配置桌子
	virtual bool ConfigTableFrame(WORD wTableCount, WORD wChairCount, DWORD dwServerRule, LPCTSTR pszResDirectory);

	//信息接口
public:
	//桌子数目
	virtual WORD GetTableCount() { return m_wTableCount; }
	//椅子数目
	virtual WORD GetChairCount() { return m_wChairCount; }
	//获取资源
	virtual CTableResource * GetTableResource() { return &m_TableResource; }

	//用户接口
public:
	//获取用户
	virtual IClientUserItem * GetClientUserItem(WORD wTableID, WORD wChairID);
	//设置信息
	virtual bool SetClientUserItem(WORD wTableID, WORD wChairID, IClientUserItem * pIClientUserItem);

	//状态接口
public:
	//游戏标志
	virtual bool GetPlayFlag(WORD wTableID);
	//密码标志
	virtual bool GetLockerFlag(WORD wTableID);
	//焦点框架
	virtual VOID SetFocusFrame(WORD wTableID, bool bFocusFrame);
	//桌子状态 
	virtual VOID SetTableStatus(WORD wTableID, bool bPlaying, bool bLocker);

	//视图控制
public:
	//桌子可视
	virtual bool VisibleTable(WORD wTableID);
	//闪动桌子
	virtual bool FlashGameTable(WORD wTableID);
	//闪动椅子
	virtual bool FlashGameChair(WORD wTableID, WORD wChairID);

	//功能接口
public:
	//更新桌子
	virtual bool UpdateTableView(WORD wTableID);
	//获取桌子
	virtual ITableView * GetTableViewItem(WORD wTableID);
	//空椅子数
	virtual WORD GetNullChairCount(WORD wTableID, WORD & wNullChairID);

	//重载函数
protected:
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//辅助函数
protected:
	//桌子测试
	WORD SwitchTableID(POINT MousePoint);
	//椅子测试
	WORD SwitchChairID(POINT MousePoint);
	//调整控件
	VOID RectifyControl(INT nWidth, INT nHeight);

	//消息函数
protected:
	//重画消息
	VOID OnPaint();
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);
	//鼠标消息
	VOID OnRButtonUp(UINT nFlags, CPoint MousePoint);
	//鼠标消息
	VOID OnLButtonUp(UINT nFlags, CPoint MousePoint);
	//鼠标离开
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	//鼠标消息
	VOID OnRButtonDown(UINT nFlags, CPoint MousePoint);
	//鼠标消息
	VOID OnLButtonDown(UINT nFlags, CPoint MousePoint);
	//鼠标消息
	VOID OnLButtonDblClk(UINT nFlags, CPoint MousePoint);
	//光标消息
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	//鼠标消息
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint MousePoint);
	//滚动消息
	VOID OnVScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar);

	//自定消息
protected:
	//事件消息
	LRESULT OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif