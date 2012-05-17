#include "StdAfx.h"
#include "GlobalUserInfo.h"

//////////////////////////////////////////////////////////////////////////////////

//静态变量
CGlobalUserInfo * CGlobalUserInfo::m_pGlobalUserInfo=NULL;				//用户信息

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGlobalUserInfo::CGlobalUserInfo()
{
	//设置对象
	ASSERT(m_pGlobalUserInfo==NULL);
	if (m_pGlobalUserInfo==NULL) m_pGlobalUserInfo=this;

	//设置变量
	ZeroMemory(&m_GlobalUserData,sizeof(m_GlobalUserData));
	ZeroMemory(&m_IndividualUserData,sizeof(m_IndividualUserData));

	return;
}

//析构函数
CGlobalUserInfo::~CGlobalUserInfo()
{
	//释放对象
	ASSERT(m_pGlobalUserInfo==this);
	if (m_pGlobalUserInfo==this) m_pGlobalUserInfo=NULL;

	return;
}

//重置资料
VOID CGlobalUserInfo::ResetUserInfoData()
{
	//设置变量
	ZeroMemory(&m_GlobalUserData,sizeof(m_GlobalUserData));
	ZeroMemory(&m_IndividualUserData,sizeof(m_IndividualUserData));

	return;
}

//////////////////////////////////////////////////////////////////////////////////
