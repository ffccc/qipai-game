#ifndef GAME_PROPERTY_ITEM_HEAD_FILE
#define GAME_PROPERTY_ITEM_HEAD_FILE

#pragma once

#include "GamePropertyHead.h"

//////////////////////////////////////////////////////////////////////////////////

//道具子项
class GAME_PROPERTY_CLASS CGamePropertyItem
{
	//友元定义
	friend class CGamePropertyManager;

	//变量定义
protected:
	tagPropertyInfo					m_PropertyInfo;						//道具信息
	tagPropertyAttrib				m_PropertyAttrib;					//道具属性

	//函数定义
public:
	//构造函数
	CGamePropertyItem();
	//析构函数
	virtual ~CGamePropertyItem();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage)=NULL;

	//功能函数
public:
	//获取信息
	tagPropertyInfo * GetPropertyInfo() { return &m_PropertyInfo; }
	//获取属性
	tagPropertyAttrib * GetPropertyAttrib() { return & m_PropertyAttrib; }

	//功能函数
public:
	//使用道具
	VOID ShowBuyPropertyWnd(IGamePropertySink * pIGamePropertySink);
};

//////////////////////////////////////////////////////////////////////////////////

//红色鲜花
class GAME_PROPERTY_CLASS CGamePropertyRedFlower : public CGamePropertyItem
{
	//函数定义
public:
	//构造函数
	CGamePropertyRedFlower();
	//析构函数
	virtual ~CGamePropertyRedFlower();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
};

//////////////////////////////////////////////////////////////////////////////////

#endif