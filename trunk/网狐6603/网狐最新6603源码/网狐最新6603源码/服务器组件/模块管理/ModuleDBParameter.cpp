#include "StdAfx.h"
#include "ModuleDBParameter.h"

//////////////////////////////////////////////////////////////////////////////////

//静态变量
CModuleDBParameter * CModuleDBParameter::m_pModuleDBParameter=NULL;		//对象指针

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CModuleDBParameter::CModuleDBParameter()
{
	//设置对象
	ASSERT(m_pModuleDBParameter==NULL);
	if (m_pModuleDBParameter==NULL) m_pModuleDBParameter=this;

	//设置变量
	ZeroMemory(&m_PlatformDBParameter,sizeof(m_PlatformDBParameter));

	return;
}

//析构函数
CModuleDBParameter::~CModuleDBParameter()
{
	//设置对象
	ASSERT(m_pModuleDBParameter==this);
	if (m_pModuleDBParameter==this) m_pModuleDBParameter=NULL;

	return;
}

//////////////////////////////////////////////////////////////////////////////////
