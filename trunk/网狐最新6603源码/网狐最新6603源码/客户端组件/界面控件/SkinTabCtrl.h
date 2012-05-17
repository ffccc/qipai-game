#ifndef SKIN_TAB_CTRL_HEAD_FILE
#define SKIN_TAB_CTRL_HEAD_FILE

#pragma once

#include "SkinControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//选择控件
class SKIN_CONTROL_CLASS CSkinTabCtrl : public CTabCtrl
{
	//状态变量
protected:
	bool							m_bRenderImage;						//渲染背景

	//变量定义
protected:
	COLORREF						m_crBackGround;						//背景颜色
	COLORREF						m_crControlBorder;					//边框颜色

	//函数定义
public:
	//构造函数
	CSkinTabCtrl();
	//析构函数
	virtual ~CSkinTabCtrl();

	//功能函数
public:
	//设置渲染
	VOID SetRenderImage(bool bRenderImage);
	//设置颜色
	VOID SetTabCtrlColor(COLORREF crBackGround, COLORREF crControlBorder);

	//消息函数
protected:
	//重画函数	
	VOID OnPaint();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif