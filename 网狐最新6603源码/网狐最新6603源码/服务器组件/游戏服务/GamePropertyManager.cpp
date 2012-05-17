#include "StdAfx.h"
#include "GamePropertyManager.h"

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyManager::CGamePropertyManager()
{
}

//析构函数
CGamePropertyManager::~CGamePropertyManager()
{
}

//设置道具
bool CGamePropertyManager::SetGamePropertyInfo(tagPropertyInfo PropertyInfo[], WORD wPropertyCount)
{
	//设置变量
	m_PropertyInfoArray.SetSize(wPropertyCount);

	//拷贝数组
	CopyMemory(m_PropertyInfoArray.GetData(),PropertyInfo,sizeof(tagPropertyInfo)*wPropertyCount);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
