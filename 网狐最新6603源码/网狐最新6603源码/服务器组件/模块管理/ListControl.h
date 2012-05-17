#ifndef LIST_CONTROL_HEAD_FILE
#define LIST_CONTROL_HEAD_FILE

#pragma once

#include "ModuleManagerHead.h"

//////////////////////////////////////////////////////////////////////////////////

//颜色定义
#define CR_NORMAL_TK				RGB(255,255,255)					//列表背景
#define CR_NORMAL_BK				RGB(255,255,255)					//列表背景

//排序位置
#define SORT_POSITION_AFTER			1									//后面位置
#define SORT_POSITION_FRONT			-1									//前面位置

//////////////////////////////////////////////////////////////////////////////////

//颜色定义
struct tagListItemColor
{
	COLORREF						rcTextColor;						//字体颜色
	COLORREF						rcBackColor;						//背景颜色
};

//////////////////////////////////////////////////////////////////////////////////

//列头控件
class MODULE_MANAGER_CLASS CListHeaderCtrl : public CHeaderCtrl
{
	//函数定义
public:
	//构造函数
	CListHeaderCtrl();
	//析构函数
	virtual ~CListHeaderCtrl();

	//消息函数
protected:
	//重画消息
	VOID OnPaint();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//列表控件
class MODULE_MANAGER_CLASS CListControl : public CListCtrl
{
	//状态变量
protected:
	bool							m_bAscendSort;						//升序标志

	//组件变量
protected:
	CListHeaderCtrl					m_ListHeaderCtrl;					//列头控件

	//函数定义
public:
	//构造函数
	CListControl();
	//析构函数
	virtual ~CListControl();

	//重载函数
public:
	//控件绑定
	virtual VOID PreSubclassWindow();
	//绘画函数
	virtual VOID DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	//重置函数
private:
	//配置列表
	virtual VOID InitListControl()=NULL;
	//子项排序
	virtual INT SortListItemData(LPARAM lParam1, LPARAM lParam2, WORD wColumnIndex)=NULL;
	//获取颜色
	virtual VOID GetListItemColor(LPARAM lParam, UINT uItemStatus, tagListItemColor & ListItemColor)=NULL;

	//静态函数
protected:
	//排列函数
	static INT CALLBACK SortFunction(LPARAM lParam1, LPARAM lParam2, LPARAM lParamList);

	//消息映射
protected:
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//点击列表
	VOID OnColumnclick(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////

#endif
