#ifndef USER_LIST_CONTROL_HEAD_FILE
#define USER_LIST_CONTROL_HEAD_FILE

#pragma once

#include "ShareControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//常量定义
#define	MAX_COLUMN					32									//最大列数

//宽度定义
#define USER_STATUS_CX				20									//列表宽度

//索引属性
#define INDEX_STATUS				0									//状态索引
#define COUNT_STATUS				6									//状态数目
#define INDEX_NORMAL				12									//常规索引
#define INDEX_FRIEND				13									//好友索引
#define INDEX_MEMBER				15									//管理索引
#define INDEX_MASTER				20									//会员索引
#define INDEX_INVALID				0xFFFF								//无效索引

//////////////////////////////////////////////////////////////////////////////////

//默认颜色
#define COLOR_SELECT_TX				RGB(0,0,0)							//字体颜色
#define COLOR_SELECT_BK				RGB(212,208,200)					//背景颜色
#define COLOR_NORMAL_TX				RGB(0,0,0)							//字体颜色
#define COLOR_NORMAL_BK				RGB(229,249,255)					//背景颜色

//默认颜色
#define COLOR_MYSELF_TX				RGB(29,56,117)						//字体颜色
#define COLOR_MYSELF_BK				RGB(199,229,245)					//背景颜色
#define COLOR_MASTER_TX				RGB(95,106,17)						//字体颜色
#define COLOR_MASTER_BK				RGB(225,237,173)					//背景颜色
#define COLOR_MEMBER_TX				RGB(114,45,3)						//字体颜色
#define COLOR_MEMBER_BK				RGB(251,236,166)					//背景颜色

//////////////////////////////////////////////////////////////////////////////////

//颜色定义
struct tagColorUserList
{
	//常规颜色
	COLORREF						crSelectTX;							//字体颜色
	COLORREF						crSelectBK;							//背景颜色
	COLORREF						crNormalTX;							//字体颜色
	COLORREF						crNormalBK;							//背景颜色

	//特殊颜色
	COLORREF						crMyselfTX;							//字体颜色
	COLORREF						crMyselfBK;							//背景颜色
	COLORREF						crMasterTX;							//字体颜色
	COLORREF						crMasterBK;							//背景颜色
	COLORREF						crMemberTX;							//字体颜色
	COLORREF						crMemberBK;							//背景颜色
};

//////////////////////////////////////////////////////////////////////////////////

//用户列表
class SHARE_CONTROL_CLASS CUserListControl : public CSkinListCtrl
{
	//状态变量
protected:
	bool							m_bCreateColumn;					//创建列项
	DWORD							m_dwMySelfUserID;					//自己信息

	//列表信息
protected:
	WORD							m_wColumnCount;						//列表数目
	BYTE							m_cbDataDescribe[MAX_COLUMN];		//数据描述

	//接口变量
protected:
	IGameLevelParser *				m_pIGameLevelParser;				//等级接口
	IGameUserManager *				m_pIGameUserManager;				//用户接口

	//资源变量
protected:
	tagColorUserList				m_ColorUserList;					//颜色定义
	static CImageList				m_ImageUserStatus;					//状态位图

	//函数定义
public:
	//构造函数
	CUserListControl();
	//析构函数
	virtual ~CUserListControl();

	//重载函数
public:
	//控件绑定
	virtual VOID PreSubclassWindow();

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
	virtual VOID GetItemColor(LPDRAWITEMSTRUCT lpDrawItemStruct, COLORREF&crColorText, COLORREF&crColorBack);
	//绘画数据
	virtual VOID DrawCustomItem(CDC * pDC, LPDRAWITEMSTRUCT lpDrawItemStruct, CRect&rcSubItem, INT nColumnIndex);

	//扩展控制
protected:
	//用户解释
	virtual IClientUserItem * GetClientUserItem(VOID * pItemData);

	//配置函数
public:
	//设置接口
	bool SetMySelfUserID(DWORD dwMySelfUserID);
	//设置接口
	bool SetGameLevelParser(IUnknownEx * pIUnknownEx);
	//设置接口
	bool SetGameUserManager(IUnknownEx * pIUnknownEx);

	//列表配置
public:
	//设置颜色
	bool SetColorUserList(tagColorUserList&ColorUserList);
	//设置列表
	bool SetColumnDescribe(tagColumnItem ColumnItem[], WORD wColumnCount);

	//消息映射
protected:
	//销毁消息
	VOID OnNcDestroy();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif