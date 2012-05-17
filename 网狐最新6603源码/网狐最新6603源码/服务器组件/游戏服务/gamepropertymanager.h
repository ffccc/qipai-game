#ifndef GAME_PROPERTY_MANAGER_HEAD_FILE
#define GAME_PROPERTY_MANAGER_HEAD_FILE

#pragma once

#include "GameServiceHead.h"

//////////////////////////////////////////////////////////////////////////////////

//类型定义
typedef CWHArray<tagPropertyInfo>	CPropertyInfoArray;					//道具数组

//////////////////////////////////////////////////////////////////////////////////

//道具管理
class GAME_SERVICE_CLASS CGamePropertyManager
{
	//变量定义
protected:
	CPropertyInfoArray				m_PropertyInfoArray;				//道具数组

	//函数定义
public:
	//构造函数
	CGamePropertyManager();
	//析构函数
	virtual ~CGamePropertyManager();

	//配置函数
public:
	//设置道具
	bool SetGamePropertyInfo(tagPropertyInfo PropertyInfo[], WORD wPropertyCount);
};

//////////////////////////////////////////////////////////////////////////////////

#endif