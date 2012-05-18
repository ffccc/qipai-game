#ifndef DLG_SEARCH_TABLE_HEAD_FILE
#define DLG_SEARCH_TABLE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "TableViewFrame.h"

//////////////////////////////////////////////////////////////////////////

//类说明
class CTableViewFrame;
class CServerViewItem;

//////////////////////////////////////////////////////////////////////////

//搜索窗口
class CDlgSearchTable : public CSkinDialog
{
	//变量定义
protected:
	WORD							m_wResultTableID;					//结果桌子

	//控件变量
public:
	CSkinButton						m_btSearch;							//查询按钮
	CSkinButton						m_btCancel;							//关闭按钮
	CSkinButton						m_btAutoJoin;						//加入按钮

	//接口变量
protected:
	CTableViewFrame *				m_pTableViewFrame;					//桌子接口
	CServerViewItem *				m_pServerViewItem;					//游戏房间

	//函数定义
public:
	//构造函数
	CDlgSearchTable();
	//析构函数
	virtual ~CDlgSearchTable();

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//配置函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();
	//取消函数
	virtual VOID OnCancel();

	//功能函数
public:
	//设置桌子
	VOID SetTableViewFrame(CTableViewFrame * pTableViewFrame);
	//设置房间
	VOID SetServerViewItem(CServerViewItem * pServerViewItem);

	//按钮消息
protected:
	//快速加入
	VOID OnBnClickedAutoJoin();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif