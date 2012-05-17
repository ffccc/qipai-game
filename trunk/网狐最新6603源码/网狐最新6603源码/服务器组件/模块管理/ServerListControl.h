#ifndef SERVER_LIST_CONTROL_HEAD_FILE
#define SERVER_LIST_CONTROL_HEAD_FILE

#pragma once

#include "ListControl.h"
#include "ModuleManagerHead.h"

//////////////////////////////////////////////////////////////////////////////////

//房间列表
class MODULE_MANAGER_CLASS CServerListControl : public CListControl
{
	//函数定义
public:
	//构造函数
	CServerListControl();
	//析构函数
	virtual ~CServerListControl();

	//重载函数
public:
	//配置列表
	virtual VOID InitListControl();
	//子项排序
	virtual INT SortListItemData(LPARAM lParam1, LPARAM lParam2, WORD wColumnIndex);
	//获取颜色
	virtual VOID GetListItemColor(LPARAM lItemParam, UINT uItemStatus, tagListItemColor & ListItemColor);

	//功能函数
public:
	//插入列表
	bool InsertServerInfo(tagGameServerInfo * pGameServerInfo);

	//辅助函数
private:
	//插入索引
	WORD GetInsertIndex(tagGameServerInfo * pGameServerInfo);
	//描述字符
	LPCTSTR GetDescribeString(tagGameServerInfo * pGameServerInfo, WORD wColumnIndex);
};

//////////////////////////////////////////////////////////////////////////////////

#endif