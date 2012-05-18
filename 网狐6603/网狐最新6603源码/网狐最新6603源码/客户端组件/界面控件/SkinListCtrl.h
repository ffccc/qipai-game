#ifndef SKIN_LISTCTRL_HEAD_FILE
#define SKIN_LISTCTRL_HEAD_FILE

#pragma once

#include "SkinScrollBar.h"
#include "SkinControlHead.h"
#include "SkinRenderManager.h"

//////////////////////////////////////////////////////////////////////////////////

//排序位置
#define SORT_AFTER					1									//后面位置
#define SORT_FRONT					-1									//前面位置

//////////////////////////////////////////////////////////////////////////////////
//枚举定义

//排序类型
enum enSortType
{
	SortType_NoSort,				//没有排序
	SortType_Ascend,				//升序排序
	SortType_Descale,				//降序排序
};

//////////////////////////////////////////////////////////////////////////////////

//类说明
class CSkinListData;
class CSkinListCtrl;
class CSkinHeaderCtrl;

//////////////////////////////////////////////////////////////////////////////////

//列表属性
class SKIN_CONTROL_CLASS CSkinListCtrlAttribute
{
	//友元定义
	friend class CSkinListCtrl;
	friend class CSkinHeaderCtrl;

	//颜色属性
public:
	COLORREF						m_crTitle;							//字体颜色
	COLORREF						m_crListTX;							//列表文字
	COLORREF						m_crListBK;							//列表底色

	//资源变量
public:
	CBitImage						m_SkinImageL;						//标题位图
	CBitImage						m_SkinImageM;						//标题位图
	CBitImage						m_SkinImageR;						//标题位图

	//函数定义
protected:
	//构造函数
	CSkinListCtrlAttribute();
	//析构函数
	virtual ~CSkinListCtrlAttribute();

	//功能函数
public:
	//配置资源
	bool Initialization(CSkinRenderManager * pSkinRenderManager);
};

//////////////////////////////////////////////////////////////////////////////////

//列头控件
class SKIN_CONTROL_CLASS CSkinHeaderCtrl : public CHeaderCtrl
{
	//锁定列表
protected:
	UINT							m_uLockCount;						//锁定数目
	UINT							m_uItemHeight;						//子项高度

	//函数定义
public:
	//构造函数
	CSkinHeaderCtrl();
	//析构函数
	virtual ~CSkinHeaderCtrl();

	//重载函数
public:
	//控件绑定
	virtual VOID PreSubclassWindow();
	//控件消息
	virtual BOOL OnChildNotify(UINT uMessage, WPARAM wParam, LPARAM lParam, LRESULT * pLResult);

	//功能函数
public:
	//设置锁定
	VOID SetLockCount(UINT uLockCount);
	//设置列高
	VOID SetItemHeight(UINT uItemHeight);

	//消息函数
protected:
	//重画消息
	VOID OnPaint();
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//列表控件
class SKIN_CONTROL_CLASS CSkinListCtrl : public CListCtrl
{
	//数据变量
protected:
	bool							m_bAscendSort;						//升序标志

	//变量定义
public:
	CSkinScrollBar					m_SkinScrollBar;					//滚动条类
	CSkinHeaderCtrl					m_SkinHeaderCtrl;					//列头控件

	//属性变量
public:
	static CSkinListCtrlAttribute	m_SkinAttribute;					//列表属性

	//函数定义
public:
	//构造函数
	CSkinListCtrl();
	//析构函数
	virtual ~CSkinListCtrl();

	//重载函数
public:
	//控件绑定
	virtual VOID PreSubclassWindow();
	//绘画函数
	virtual VOID DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	//插入函数
public:
	//插入数据
	virtual VOID InsertDataItem(VOID * pItemData);
	//更新数据
	virtual VOID UpdateDataItem(VOID * pItemData);
	//删除数据
	virtual VOID DeleteDataItem(VOID * pItemData);

	//数据控制
protected:
	//获取位置
	virtual INT GetInsertIndex(VOID * pItemData);
	//排列数据
	virtual INT SortItemData(LPARAM lParam1, LPARAM lParam2, WORD wColumnIndex, bool bAscendSort);
	//描述字符
	virtual VOID ConstructString(VOID * pItemData, WORD wColumnIndex, LPTSTR pszString, WORD wMaxCount);

	//绘画控制
protected:
	//获取颜色
	virtual VOID GetItemColor(LPDRAWITEMSTRUCT lpDrawItemStruct, COLORREF & crColorText, COLORREF & crColorBack);
	//绘画数据
	virtual VOID DrawCustomItem(CDC * pDC, LPDRAWITEMSTRUCT lpDrawItemStruct, CRect & rcSubItem, INT nColumnIndex);

	//消息函数
protected:
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//建立消息
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);

	//静态函数
protected:
	//排列函数
	static INT CALLBACK SortFunction(LPARAM lParam1, LPARAM lParam2, LPARAM lParamList);

	//消息映射
protected:
	//点击列表
	VOID OnColumnclick(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif