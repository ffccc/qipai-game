#include "StdAfx.h"
#include "GamePropertyManager.h"

//////////////////////////////////////////////////////////////////////////////////

//静态变量
CGamePropertyManager * CGamePropertyManager::m_pGamePropertyManager=NULL;//对象指针

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyManager::CGamePropertyManager()
{
	//设置变量
	ASSERT(m_pGamePropertyManager==NULL);
	if (m_pGamePropertyManager==NULL) m_pGamePropertyManager=this;

	return;
}

//析构函数
CGamePropertyManager::~CGamePropertyManager()
{
	//设置变量
	ASSERT(m_pGamePropertyManager==this);
	if (m_pGamePropertyManager==this) m_pGamePropertyManager=NULL;

	return;
}

//获取道具
CGamePropertyItem * CGamePropertyManager::GetPropertyItem(WORD wIndex)
{
	//查找道具
	CGamePropertyItem * pGamePropertyItem=NULL;
	m_MapPropertyItem.Lookup(wIndex,pGamePropertyItem);

	return pGamePropertyItem;
}

//创建道具
CGamePropertyItem * CGamePropertyManager::CreatePropertyItem(tagPropertyInfo & PropertyInfo)
{
	//查找现存
	CGamePropertyItem * pGamePropertyItem=NULL;
	m_MapPropertyItem.Lookup(PropertyInfo.wIndex,pGamePropertyItem);

	//现存判断
	if (pGamePropertyItem!=NULL)
	{
		return pGamePropertyItem;
	}

	//创建道具
	switch (PropertyInfo.wIndex)
	{
	case PROPERTY_ID_RED_FLOWER:	//红色鲜花
		{
			try
			{
				pGamePropertyItem=new CGamePropertyRedFlower;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	}

	//创建判断
	if (pGamePropertyItem!=NULL)
	{
		//设置道具
		pGamePropertyItem->m_PropertyInfo=PropertyInfo;
		m_MapPropertyItem[PropertyInfo.wIndex]=pGamePropertyItem;

		return pGamePropertyItem;
	}

	//错误断言
	ASSERT(FALSE);

	return NULL;
}

//////////////////////////////////////////////////////////////////////////////////
