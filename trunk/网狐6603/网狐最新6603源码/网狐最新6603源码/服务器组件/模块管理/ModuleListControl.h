#ifndef MODULE_LIST_CONTROL_HEAD_FILE
#define MODULE_LIST_CONTROL_HEAD_FILE

#pragma once

#include "ListControl.h"
#include "ModuleManagerHead.h"

//////////////////////////////////////////////////////////////////////////////////

//模块列表
class MODULE_MANAGER_CLASS CModuleListControl : public CListControl
{
	//函数定义
public:
	//构造函数
	CModuleListControl();
	//析构函数
	virtual ~CModuleListControl();

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
	bool InsertModuleInfo(tagGameModuleInfo * pGameModuleInfo);

	//辅助函数
private:
	//插入索引
	WORD GetInsertIndex(tagGameModuleInfo * pGameModuleInfo);
	//描述字符
	LPCTSTR GetDescribeString(tagGameModuleInfo * pGameModuleInfo, WORD wColumnIndex);
};

//////////////////////////////////////////////////////////////////////////////////

#endif