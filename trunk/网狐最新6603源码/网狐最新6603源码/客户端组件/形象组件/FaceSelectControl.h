#ifndef FACE_SELECT_CONTRIL_HEAD_FILE
#define FACE_SELECT_CONTRIL_HEAD_FILE

#pragma once

#include "FaceItemControl.h"
#include "AvatarControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//接口定义
interface IFaceItemSelectEvent
{
	//图像事件
	virtual VOID OnEventFaceSelect(WORD wFaceID)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//头像选择
class AVATAR_CONTROL_CLASS CFaceItemSelectWnd : public CWnd
{
	//滚动信息
protected:
	INT								m_nMaxPels;							//最大象素
	INT								m_nPagePels;						//页面象素
	INT								m_nLineCount;						//总行数目
	INT								m_nWidthCount;						//总列数目
	INT								m_nCurrentPos;						//当前位置
	INT								m_nXExcursion;						//横行偏移

	//头像变量
protected:
	WORD							m_wFaceCount;						//头像数目
	WORD							m_wSelectFace;						//选择索引

	//组件变量
protected:
	CSkinScrollBar					m_SkinScrollBar;					//滚动控件
	IFaceItemSelectEvent *			m_pIFaceItemSelectEvent;			//回调接口

	//函数定义
public:
	//构造函数
	CFaceItemSelectWnd();
	//析构函数
	virtual ~CFaceItemSelectWnd();

	//选择函数
public:
	//选择数目
	VOID SetAllowItemFull();
	//选择数目
	VOID SetAllowItemCount(WORD wFaceCount);

	//功能函数
public:
	//获取选择
	WORD GetSelectFace() { return m_wSelectFace; }
	//设置接口
	VOID SetFaceItemSelectEvent(IFaceItemSelectEvent * pIFaceItemSelectEvent) { m_pIFaceItemSelectEvent=pIFaceItemSelectEvent; }

	//辅助函数
protected:
	//按键测试
	WORD MouseHitTest(CPoint MousePoint);
	//调整控件
	VOID RectifyControl(INT nWidth, INT nHeight);
	//绘画头像
	VOID DrawFaceItem(CDC * pDC, WORD wIndex, INT nXPos, INT nYPos, bool bHover);

	//消息映射
protected:
	//绘画消息
	VOID OnPaint();
	//位置信息
	VOID OnSize(UINT nType, INT cx, INT cy);
	//创建消息
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//鼠标消息
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//按键消息
	VOID OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//光标消息
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	//滚动消息
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint Point);
	//滚动消息
	VOID OnVScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//头像选择
class CFaceSelectControl : public CSkinDialog, public IFaceSelectControl, public IFaceItemSelectEvent
{
	//组件变量
protected:
	CFaceItemSelectWnd				m_FaceItemSelectWnd;				//选择窗口

	//函数定义
public:
	//构造函数
	CFaceSelectControl();
	//析构函数
	virtual ~CFaceSelectControl();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//设置选择
public:
	//选择数目
	virtual VOID SetAllowItemFull() { m_FaceItemSelectWnd.SetAllowItemFull(); }
	//选择数目
	virtual VOID SetAllowItemCount(WORD wFaceCount) { m_FaceItemSelectWnd.SetAllowItemCount(wFaceCount); }

	//获取选择
public:
	//获取选择
	virtual bool GetSelectFaceID(WORD & wFaceID);

	//编辑接口
public:
	//图像事件
	virtual VOID OnEventFaceSelect(WORD wFaceID);

	//重载函数
public:
	//配置函数
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif