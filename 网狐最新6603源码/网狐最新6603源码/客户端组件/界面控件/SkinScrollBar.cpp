#include "StdAfx.h"
#include "Resource.h"
#include "SkinScrollBar.h"

//////////////////////////////////////////////////////////////////////////////////
//静态变量

CSkinScrollBarAttribute			CSkinScrollBar::m_SkinAttribute;		//资源变量

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CSkinScrollBarAttribute::CSkinScrollBarAttribute()
{
	//加载资源
	m_ImageScroll.LoadFromResource(GetModuleHandle(SKIN_CONTROL_DLL_NAME),IDB_SKIN_SCROLL);

	return;
}
	
//析构函数
CSkinScrollBarAttribute::~CSkinScrollBarAttribute()
{
	//销毁资源
	if (m_ImageScroll.IsNull()==false)
	{
		m_ImageScroll.Destroy();
	}

	return;
}

//配置资源
bool CSkinScrollBarAttribute::Initialization(CSkinRenderManager * pSkinRenderManager)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CSkinScrollBar::CSkinScrollBar()
{
}

//析构函数
CSkinScrollBar::~CSkinScrollBar()
{
}

//配置滚动
VOID CSkinScrollBar::InitScroolBar(CWnd * pWnd)
{
	//设置滚动
	SkinSB_Init(pWnd->GetSafeHwnd(),m_SkinAttribute.m_ImageScroll);

	return;
}

//////////////////////////////////////////////////////////////////////////////////
