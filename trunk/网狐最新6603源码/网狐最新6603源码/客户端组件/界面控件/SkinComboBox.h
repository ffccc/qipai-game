#ifndef SKIN_COMBOBOX_HEAD_FILE
#define SKIN_COMBOBOX_HEAD_FILE

#pragma once

#include "SkinEdit.h"
#include "SkinControlHead.h"
#include "SkinRenderManager.h"
#include "SkinResourceManager.h"

//////////////////////////////////////////////////////////////////////////////////

//按钮属性
class SKIN_CONTROL_CLASS CSkinComboBoxAttribute
{
	//设置部分
public:
	UINT							m_uButtonWidth;						//按钮宽度
	COLORREF						m_crEnableBack;						//背景颜色
	COLORREF						m_crDisableBack;					//背景颜色
	COLORREF						m_crEnableBorad;					//边框颜色
	COLORREF						m_crDisableBorad;					//边框颜色

	//函数定义
public:
	//构造函数
	CSkinComboBoxAttribute();
	//析构函数
	virtual ~CSkinComboBoxAttribute();

	//管理函数
public:
	//配置资源
	bool Initialization(CSkinRenderManager * pSkinRenderManager);
};

//////////////////////////////////////////////////////////////////////////////////

//列表控件
class SKIN_CONTROL_CLASS CSkinComboBoxList : public CListBox
{
	//函数定义
public:
	//构造函数
	CSkinComboBoxList();
	//析构函数
	virtual ~CSkinComboBoxList();

	//重载函数
protected:
	//绘画子项
	virtual VOID DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct); 

	//消息映射
protected:
	//界面绘画
	VOID OnNcPaint();

    DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//选择控件
class SKIN_CONTROL_CLASS CSkinComboBox : public CComboBox
{
	//状态变量
protected:
	bool							m_bHovering;						//盘旋标志
	bool							m_bDrawBorad;						//绘画边框
	bool							m_bRenderImage;						//渲染标志
	UINT							m_uButtonWidth;						//按钮宽度

	//资源变量
protected:
	UINT							m_uResourceID;						//资源标识
	HINSTANCE						m_hResInstance;						//资源句柄

	//控件变量
protected:
	CSkinEdit						m_SkinComboBoxEdit;					//编辑控件
	CSkinComboBoxList				m_SkinComboBoxList;					//列表控件

	//按钮属性
public:
	static CSkinComboBoxAttribute	m_SkinAttribute;					//按钮属性

	//函数定义
public:
	//构造函数
	CSkinComboBox();
	//析构函数
	virtual ~CSkinComboBox();

	//重载函数
protected:
	//控件绑定
    virtual VOID PreSubclassWindow();
	//测量子项
	virtual VOID MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	//功能函数
public:
	//边框设置
	VOID SetDrawBorad(bool bDrawBorad);
	//设置渲染
	VOID SetRenderImage(bool bRenderImage);
	//设置资源
	VOID SetButtonImage(HINSTANCE hResInstance, UINT uResourceID, CSize & SizeButton);

	//辅助函数
protected:
	//绘画控件
	VOID DrawControlView(CDC * pDC);
	//按钮检测
	bool VerdictOverButton(CPoint MousePoint);

	//内部函数
private:
	//配置控件
	VOID Initialization();
	//更新按钮
	VOID RedrawDropButton();
	//调整控件
	VOID RectifyControl(INT nWidth, INT nHeight);

	//消息映射
protected:
	//重画消息
    VOID OnPaint();
	//列表关闭
	VOID OnCbnCloseup();
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//时间消息
	VOID OnTimer(UINT_PTR nIDEvent);
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);
	//创建消息
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//鼠标移动
	VOID OnMouseMove(UINT nFlags, CPoint Point);
	//鼠标消息
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//鼠标消息
	VOID OnLButtonDblClk(UINT nFlags, CPoint Point);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif