#ifndef SERVER_LIST_VIEW_HEAD_FILE
#define SERVER_LIST_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "ServerListData.h"

//////////////////////////////////////////////////////////////////////////////////

//数组说明
typedef CMap<WORD,WORD,UINT,UINT>	CGameImageMap;						//游戏图标

//////////////////////////////////////////////////////////////////////////////////

//房间列表
class CServerListView : public CTreeCtrl, public IServerListDataSink
{
	//位置变量
protected:
	INT								m_nXScroll;							//滚动偏移
	INT								m_nYScroll;							//滚动偏移

	//列表句柄
protected:
	HTREEITEM						m_TreeListRoot;						//顶项句柄
	HTREEITEM						m_TreeAssistant;					//辅助句柄

	//资源变量
protected:
	CFont							m_FontBold;							//粗体字体
	CPngImage						m_ImageArrow;						//箭头图标
	CSkinScrollBar					m_SkinScrollBar;					//滚动条类

	//资源变量
protected:
	CImageList						m_ImageList;						//图片资源
	CGameImageMap					m_GameImageMap;						//图形索引

	//辅助变量
protected:
	HTREEITEM						m_hItemMouseHover;					//盘旋子项
	HTREEITEM						m_hTreeClickExpand;					//单击树项

	//字体变量
protected:

	//函数定义
public:
	//构造函数
	CServerListView();
	//析构函数
	virtual ~CServerListView();

	//状态通知
public:
	//获取通知
	virtual VOID OnGameItemFinish();
	//获取通知
	virtual VOID OnGameKindFinish(WORD wKindID);

	//更新通知
public:
	//插入通知
	virtual VOID OnGameItemInsert(CGameListItem * pGameListItem);
	//更新通知
	virtual VOID OnGameItemUpdate(CGameListItem * pGameListItem);
	//删除通知
	virtual VOID OnGameItemDelete(CGameListItem * pGameListItem);

	//重载函数
protected:
	//命令函数
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//窗口函数
	virtual LRESULT DefWindowProc(UINT uMessage, WPARAM wParam, LPARAM lParam);

	//功能函数
public:
	//快速通道
	VOID InitAssistantItem();
	//配置函数
	VOID InitServerTreeView();

	//功能函数
public:
	//获取选择
	HTREEITEM GetCurrentSelectItem(bool bOnlyText);

	//展开函数
public:
	//展开判断
	bool ExpandVerdict(HTREEITEM hTreeItem);
	//展开列表
	bool ExpandListItem(HTREEITEM hTreeItem);
	//展开列表
	bool ExpandListItem(CGameListItem * pGameListItem);

	//绘画函数
private:
	//绘画子项
	VOID DrawTreeItem(CDC * pDC, CRect & rcClient, CRect & rcClipBox);
	//绘画背景
	VOID DrawTreeBack(CDC * pDC, CRect & rcClient, CRect & rcClipBox);

	//绘画辅助
private:
	//绘制图标
	VOID DrawListImage(CDC * pDC, CRect rcRect, HTREEITEM hTreeItem);
	//绘制文本
	VOID DrawItemString(CDC * pDC, CRect rcRect, HTREEITEM hTreeItem, bool bSelected);

	//图标函数
private:
	//获取图标
	UINT GetGameImageIndex(CGameKindItem * pGameKindItem);
	//获取图标
	UINT GetGameImageIndex(CGameServerItem * pGameServerItem);

	//标题函数
private:
	//获取标题
	LPCTSTR GetGameItemTitle(CGameKindItem * pGameKindItem, LPTSTR pszTitle, UINT uMaxCount);
	//获取标题
	LPCTSTR GetGameItemTitle(CGameServerItem * pGameServerItem, LPTSTR pszTitle, UINT uMaxCount);

	//辅助函数
private:
	//删除更新
	VOID DeleteUpdateItem(CGameListItem * pGameListItem);
	//修改子项
	VOID ModifyGameListItem(HTREEITEM hTreeItem, LPCTSTR pszTitle, UINT uImage);
	//插入子项
	HTREEITEM InsertInsideItem(LPCTSTR pszTitle, UINT uImage, DWORD dwInsideID, HTREEITEM hParentItem);
	//插入子项
	HTREEITEM InsertGameListItem(LPCTSTR pszTitle, UINT uImage, CGameListItem * pGameListItem, HTREEITEM hParentItem);

	//系统消息
protected:
	//重画消息
	VOID OnPaint();
	//时间消息
	VOID OnTimer(UINT nIDEvent);
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);
	//光标消息
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	//列表消息
protected:
	//右键列表
	VOID OnNMRClick(NMHDR * pNMHDR, LRESULT * pResult);
	//左击列表
	VOID OnNMLClick(NMHDR * pNMHDR, LRESULT * pResult);
	//列表改变
	VOID OnTvnSelchanged(NMHDR * pNMHDR, LRESULT * pResult);
	//列表展开
	VOID OnTvnItemexpanding(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif