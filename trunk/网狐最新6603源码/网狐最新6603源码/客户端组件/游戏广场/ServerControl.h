#ifndef SERVER_CONTROL_HEAD_FILE
#define SERVER_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////

//账号控件
class CServerControl : public CSkinComboBox
{
	//函数定义
public:
	//构造函数
	CServerControl();
	//析构函数
	virtual ~CServerControl();

	//重载函数
protected:
	//绑定函数
	virtual VOID PreSubclassWindow();
	//测量子项
	virtual VOID MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif