#ifndef SKIN_MENU_HEAD_FILE
#define SKIN_MENU_HEAD_FILE

#pragma once

#include "SkinControlHead.h"
#include "SkinRenderManager.h"
#include "SkinResourceManager.h"

//////////////////////////////////////////////////////////////////////////////////

//菜单属性
class SKIN_CONTROL_CLASS CSkinMenuAttribute
{
	//字体颜色
public:
	COLORREF						m_crNormalText;						//菜单颜色
	COLORREF						m_crSelectText;						//菜单颜色

	//边框颜色
public:
	COLORREF						m_crMenuBar;						//菜单颜色
	COLORREF						m_crSeparator;						//菜单颜色
	COLORREF						m_crNormalBack;						//菜单颜色
	COLORREF						m_crSelectBack;						//菜单颜色
	COLORREF						m_crSelectBorder;					//菜单颜色

	//函数定义
public:
	//构造函数
	CSkinMenuAttribute();
	//析构函数
	virtual ~CSkinMenuAttribute();

	//管理函数
public:
	//配置资源
	bool Initialization(CSkinRenderManager * pSkinRenderManager);
};

//////////////////////////////////////////////////////////////////////////////////

//子项类型
enum enMenuItemType
{
	MenuItemType_Image,				//图形类型
	MenuItemType_String,			//字符类型
	MenuItemType_Separator,			//拆分类型
};

//////////////////////////////////////////////////////////////////////////////////

//菜单子项
class CSkinMenuItem
{
	//变量定义
public:
	const enMenuItemType			m_MenuItemType;						//子项类型

	//函数定义
public:
	//构造函数
	CSkinMenuItem(enMenuItemType MenuItemType) : m_MenuItemType(MenuItemType) {}
	//析构函数
	virtual ~CSkinMenuItem() {}
};

//图形菜单
class CSkinMenuImage : public CSkinMenuItem
{
	//变量定义
public:
	HBITMAP							m_hBitmap;							//图像句柄

	//函数定义
public:
	//构造函数
	CSkinMenuImage() : CSkinMenuItem(MenuItemType_Image) { m_hBitmap=NULL; }
	//析构函数
	virtual ~CSkinMenuImage() {}
};

//字符菜单
class CSkinMenuString : public CSkinMenuItem
{
	//变量定义
public:
	CString							m_strString;						//菜单字符

	//函数定义
public:
	//构造函数
	CSkinMenuString() : CSkinMenuItem(MenuItemType_String) {}
	//析构函数
	virtual ~CSkinMenuString() {}
};

//拆分菜单
class CSkinMenuSeparator : public CSkinMenuItem
{
	//函数定义
public:
	//构造函数
	CSkinMenuSeparator() : CSkinMenuItem(MenuItemType_Separator) {}
	//析构函数
	virtual ~CSkinMenuSeparator() {}
};

//////////////////////////////////////////////////////////////////////////////////

//菜单内核
class SKIN_CONTROL_CLASS CSkinMenuKernel : public CMenu
{
	//菜单属性
public:
	static CSkinMenuAttribute	m_SkinAttribute;						//菜单属性

	//函数定义
public:
	//构造函数
	CSkinMenuKernel();
	//析构函数
	virtual ~CSkinMenuKernel();

	//重载函数
public:
	//绘画函数
	virtual VOID DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//测量位置
	virtual VOID MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
};

//////////////////////////////////////////////////////////////////////////////////

//数组说明
typedef CWHArray<CSkinMenuItem *>		CMenuItemArray;					//子项数组
typedef CWHArray<CSkinMenuImage *>		CMenuImageArray;				//子项数组
typedef CWHArray<CSkinMenuString *>		CMenuStringArray;				//子项数组
typedef CWHArray<CSkinMenuSeparator *>	CMenuSeparatorArray;			//子项数组

//界面菜单
class SKIN_CONTROL_CLASS CSkinMenu
{
	//内核变量
protected:
	CMenuItemArray					m_MenuItemActive;					//子项数组
	CSkinMenuKernel					m_SkinMenuKernel;					//菜单内核

	//静态变量
protected:
	static CMenuImageArray			m_MenuItemImage;					//图形子项
	static CMenuStringArray			m_MenuItemString;					//字符子项
	static CMenuSeparatorArray		m_MenuItemSeparator;				//拆分子项

	//函数定义
public:
	//构造函数
	CSkinMenu();
	//析构函数
	virtual ~CSkinMenu();

	//管理函数
public:
	//创建菜单
	bool CreateMenu();
	//销毁菜单
	bool DestroyMenu();
	//弹出菜单
	bool TrackPopupMenu(CWnd * pWnd);
	//弹出菜单
	bool TrackPopupMenu(INT nXPos, INT nYPos, CWnd * pWnd);

	//增加函数
public:
	//插入拆分
	bool AppendSeparator();
	//插入位图
	bool AppendMenu(UINT nMenuID, HBITMAP hBitmap, UINT nFlags=0);
	//插入字符
	bool AppendMenu(UINT nMenuID, LPCTSTR pszString, UINT nFlags=0);

	//插入函数
public:
	//插入拆分
	bool InsertSeparator(UINT nPosition);
	//插入位图
	bool InsertMenu(UINT nMenuID, HBITMAP hBitmap, UINT nPosition, UINT nFlags=0);
	//插入字符
	bool InsertMenu(UINT nMenuID, LPCTSTR pszString, UINT nPosition, UINT nFlags=0);

	//修改函数
public:
	//删除菜单
	bool RemoveMenu(UINT nPosition, UINT nFlags);
	//修改菜单
	bool ModifyMenu(UINT nMenuID, HBITMAP hBitmap, UINT nPosition, UINT nFlags=0);
	//修改菜单
	bool ModifyMenu(UINT nMenuID, LPCTSTR pszString, UINT nPosition, UINT nFlags=0);

	//状况控制
public:
	//设置默认
	bool SetDefaultItem(UINT uItem, BOOL fByPos);
	//选择控制
	UINT CheckMenuItem(UINT nMenuID, UINT nCheck);
	//启用控制
	UINT EnableMenuItem(UINT nMenuID, UINT nEnable);

	//操作重栽
public:
	//菜单句柄
	HMENU GetSafeHMenu() { return m_SkinMenuKernel; }
	//菜单句柄
	operator HMENU () { return (HMENU)m_SkinMenuKernel; }

	//内部函数
private:
	//释放子项
	VOID FreeMenuItem(CSkinMenuItem * pSkinMenuItem);
	//获取子项
	CSkinMenuItem * AcitveMenuItem(enMenuItemType MenuItemType);
};

//////////////////////////////////////////////////////////////////////////////////

#endif