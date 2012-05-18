#ifndef SKIN_SPLITTER_HEAD_FILE
#define SKIN_SPLITTER_HEAD_FILE

#pragma once

#include "SkinControlHead.h"
#include "SkinRenderManager.h"

//////////////////////////////////////////////////////////////////////////////////

//拆分类型
enum enSplitterType
{
	SplitterType_Hor,				//横向拆分
	SplitterType_Vor,				//竖向拆分
};

//拆分事件
interface ISplitterEvent
{
	//拆分事件
	virtual VOID OnSplitterEvent(UINT uControlID, INT nXPos, INT nYPos)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//拆分条类
class SKIN_CONTROL_CLASS CSkinSplitter : public CWnd
{
	//状态变量
protected:
	bool							m_bMouseDown;						//是否按下
	CRect							m_rcDrawLine;						//线条位置

	//属性变量
protected:
	INT								m_nMaxDragPos;						//最大位置
	INT								m_nMinDragPos;						//最小位置
	enSplitterType					m_SplitterType;						//拆分类型

	//颜色变量
protected:
	DOUBLE							m_ColorS;							//背景颜色
	DOUBLE							m_ColorB;							//背景颜色
	COLORREF						m_crBackColor;						//背景颜色

	//组件变量
protected:
	ISplitterEvent *				m_pISplitterEvent;					//拆分事件

	//函数定义
public:
	//构造函数
	CSkinSplitter();
	//析构函数
	virtual ~CSkinSplitter();

	//重载函数
protected:
	//绑定函数
	virtual VOID PreSubclassWindow();
	//命令消息
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//配置函数
public:
	//设置类型
	VOID SetSplitterType(enSplitterType SplitterType);
	//设置范围
	VOID SetSplitterRange(INT nMinDragPos, INT nMaxDragPos);
	//设置接口
	VOID SetSplitterEvent(ISplitterEvent * pISplitterEvent);

	//设置颜色
public:
	//背景颜色
	VOID SetSplitterColor(DOUBLE S, DOUBLE B);
	//背景颜色
	VOID SetSplitterColor(COLORREF crBackColor);

	//查询函数
public:
	//获取类型
	enSplitterType GetSplitterType() { return m_SplitterType; }
	//获取接口
	ISplitterEvent * GetSplitterEvent() { return m_pISplitterEvent; };
	//设置范围
	VOID GetSplitterRange(INT & nMinDragPos, INT & nMaxDragPos) { nMinDragPos=m_nMinDragPos; nMaxDragPos=m_nMaxDragPos; }

	//消息函数
protected:
	//重画消息
	VOID OnPaint();
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//失去焦点
	VOID OnKillFocus(CWnd * pNewWnd);
	//鼠标消息
	VOID OnMouseMove(UINT nFlags, CPoint point);
	//鼠标消息
	VOID OnLButtonUp(UINT nFlags, CPoint point);
	//鼠标消息
	VOID OnLButtonDown(UINT nFlags, CPoint point);
	//光标消息
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMmessage);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif