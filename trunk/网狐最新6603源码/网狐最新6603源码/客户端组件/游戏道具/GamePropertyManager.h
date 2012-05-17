#ifndef GAME_PROPERTY_MANAGER_HEAD_FILE
#define GAME_PROPERTY_MANAGER_HEAD_FILE

#pragma once

#include "GamePropertyHead.h"
#include "GamePropertyItem.h"

//////////////////////////////////////////////////////////////////////////////////

//道具索引
typedef CMap<WORD,WORD,CGamePropertyItem *,CGamePropertyItem *> CMapPropertyItem;

//////////////////////////////////////////////////////////////////////////////////

//道具管理
class GAME_PROPERTY_CLASS CGamePropertyManager
{
	//变量定义
protected:
	CMapPropertyItem				m_MapPropertyItem;					//道具索引

	//静态变量
protected:
	static CGamePropertyManager *	m_pGamePropertyManager;				//对象指针

	//函数定义
public:
	//构造函数
	CGamePropertyManager();
	//析构函数
	virtual ~CGamePropertyManager();

	//配置函数
public:
	//获取道具
	CGamePropertyItem * GetPropertyItem(WORD wIndex);
	//创建道具
	CGamePropertyItem * CreatePropertyItem(tagPropertyInfo & PropertyInfo);

	//静态函数
public:
	//获取对象
	static CGamePropertyManager * GetInstance() { return m_pGamePropertyManager; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif