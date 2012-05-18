#ifndef GAME_PROPERTY_CTRL_HEAD_FILE
#define GAME_PROPERTY_CTRL_HEAD_FILE

#pragma once

#include "GamePropertyHead.h"
#include "GamePropertyItem.h"

//////////////////////////////////////////////////////////////////////////////////

//游戏道具
class GAME_PROPERTY_CLASS CGamePropertyCtrl : public CWnd
{
	//状态变量
protected:
	bool							m_bHovering;						//盘旋标志
	WORD							m_wDownItem;						//点击子项
	WORD							m_wHoverItem;						//经过子项

	//配置变量
protected:
	CSize							m_SizeImageItem;					//子项图片
	tagPropertyResource				m_PropertyResource;					//资源变量

	//变量定义
protected:
	BYTE							m_cbPropertyCount;					//道具数目
	CGamePropertyItem *				m_pGamePropertyItem[MAX_PROPERTY];	//道具数组

	//接口变量
protected:
	IGamePropertySink *				m_pIGamePropertySink;				//回调接口

	//函数定义
public:
	//构造函数
	CGamePropertyCtrl();
	//析构函数
	virtual ~CGamePropertyCtrl();

	//界面函数
private:
	//调整控件
	VOID RectifyControl(INT nWidth, INT nHeight);

	//配置函数
public:
	//设置接口
	bool SetGamePropertySink(IUnknownEx * pIUnknownEx);
	//设置资源
	bool SetPropertyResource(tagPropertyResource & PropertyResource);
	//设置道具
	bool SetGamePropertyInfo(tagPropertyInfo PropertyInfo[], BYTE cbPropertyCount, WORD wIssueArea);

	//内部函数
protected:
	//点击判断
	WORD SwitchItemFromPoint(CPoint MousePoint);

	//消息映射
protected:
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);
	//鼠标移动
	VOID OnMouseMove(UINT nFlags, CPoint MousePoint);
	//鼠标消息
	VOID OnLButtonUp(UINT nFlags, CPoint MousePoint);
	//鼠标消息
	VOID OnLButtonDown(UINT nFlags, CPoint MousePoint);
	//鼠标离开
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	//光标消息
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif