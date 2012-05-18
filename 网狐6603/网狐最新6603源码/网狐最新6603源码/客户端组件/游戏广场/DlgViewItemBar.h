#ifndef WND_VIEW_ITEM_CTRL_HEAD_FILE
#define WND_VIEW_ITEM_CTRL_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////////////

//子项信息
struct tagViewItemInfo
{
	CWnd *							pItemView;							//子项视图
	TCHAR							szItemTitle[32];					//标题字符
};

//控制事件
interface IViewItemEvent
{
	//激活事件
	virtual bool OnViewItemActive(WORD wItemIndex, CWnd * pItemView)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//子项控制
class CWndViewItemCtrl : public CWnd
{
	//点击信息
protected:
	bool							m_bHovering;						//盘旋标志
	WORD							m_wItemDown;						//按下子项
	WORD							m_wItemHover;						//盘旋子项
	WORD							m_wItemActive;						//激活子项

	//配置信息
protected:
	CSize							m_SizeButton;						//按钮大小
	CRect							m_RectViewItem;						//子项位置
	IViewItemEvent *				m_pIViewItemEvent;					//事件接口

	//子项信息
protected:
	CWHArray<tagViewItemInfo *>		m_ViewItemArray;					//控制数组

	//函数定义
public:
	//构造函数
	CWndViewItemCtrl();
	//析构函数
	virtual ~CWndViewItemCtrl();

	//功能函数
public:
	//获取大小
	VOID GetViewItemSize(CSize & ResultSize);
	//设置接口
	VOID SetViewItemEvent(IViewItemEvent * pIViewItemEvent);
	//设置位置
	VOID SetViewItemPostion(INT nXPos, INT nYPos, INT nWidth, INT nHeight);

	//控制函数
public:
	//激活子项
	VOID ActiveViewItem(CWnd * pItemView);
	//删除子项
	VOID RemoveViewItem(CWnd * pItemView);
	//插入子项
	VOID InsertViewItem(CWnd * pItemView, LPCTSTR pszTitleImage);

	//内部函数
private:
	//对象索引
	WORD GetViewItemIndex(CWnd * pItemView);
	//对象索引
	WORD GetViewItemIndex(CPoint MousePoint);

	//消息函数
protected:
	//重画消息
	VOID OnPaint();
	//创建消息
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//鼠标移动
	VOID OnMouseMove(UINT nFlags, CPoint Point);
	//鼠标消息
	VOID OnLButtonUp(UINT nFlags, CPoint Point);
	//鼠标消息
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//鼠标离开
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	//光标消息
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif