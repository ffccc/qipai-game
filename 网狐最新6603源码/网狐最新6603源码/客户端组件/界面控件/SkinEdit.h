#ifndef SKIN_EDIT_HEAD_FILE
#define SKIN_EDIT_HEAD_FILE

#pragma once

#include "SkinControlHead.h"
#include "SkinRenderManager.h"

//////////////////////////////////////////////////////////////////////////////////

//控件属性
class SKIN_CONTROL_CLASS CSkinEditAttribute
{
	//画刷对象
public:
	CBrush							m_brEnable;							//画刷资源
	CBrush							m_brDisable;						//画刷资源

	//背景颜色
public:
	COLORREF						m_crEnableBK;						//背景颜色
	COLORREF						m_crDisableBK;						//背景颜色

	//字体颜色
public:
	COLORREF						m_crEnableText;						//字体颜色
	COLORREF						m_crDisableText;					//字体颜色

	//边框颜色
public:
	COLORREF						m_crEnableBorad;					//边框颜色
	COLORREF						m_crDisableBorad;					//边框颜色

	//函数定义
public:
	//构造函数
	CSkinEditAttribute();
	//析构函数
	virtual ~CSkinEditAttribute();

	//管理函数
public:
	//配置资源
	bool Initialization(CSkinRenderManager * pSkinRenderManager);
};

//////////////////////////////////////////////////////////////////////////////////

//编辑框
class SKIN_CONTROL_CLASS CSkinEdit : public CEdit
{
	//静态变量
public:
	static CSkinEditAttribute		m_SkinAttribute;					//界面属性

	//函数定义
public:
	//构造函数
	CSkinEdit();
	//析构函数
	virtual ~CSkinEdit();

	//内部函数
protected:
	//绘画控件
	VOID DrawControlView(CDC * pDC, COLORREF crColorBorad, COLORREF crColorBK);

	//消息函数 
protected:
	//重画消息
    VOID OnNcPaint();
	//绘画控件
	HBRUSH CtlColor(CDC * pDC, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//编辑框
class SKIN_CONTROL_CLASS CSkinEditEx : public CSkinEdit
{
	//画刷对象
public:
	CBrush							m_brEnable;							//画刷资源
	CBrush							m_brDisable;						//画刷资源

	//背景颜色
public:
	COLORREF						m_crEnableBK;						//背景颜色
	COLORREF						m_crDisableBK;						//背景颜色

	//字体颜色
public:
	COLORREF						m_crEnableText;						//字体颜色
	COLORREF						m_crDisableText;					//字体颜色

	//边框颜色
public:
	COLORREF						m_crEnableBorad;					//边框颜色
	COLORREF						m_crDisableBorad;					//边框颜色

	//函数定义
public:
	//构造函数
	CSkinEditEx();
	//析构函数
	virtual ~CSkinEditEx();

	//功能函数
public:
	//设置颜色
	VOID SetEnableColor(COLORREF crEnableText, COLORREF crEnableBK, COLORREF crEnableBorad);
	//设置颜色
	VOID SetDisableColor(COLORREF crDisableText, COLORREF crDisableBK, COLORREF crDisableBorad);

	//消息函数 
protected:
	//重画消息
    VOID OnNcPaint();
	//绘画控件
	HBRUSH CtlColor(CDC * pDC, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif