#include "StdAfx.h"
#include "SkinEdit.h"
#include "SkinMenu.h"
#include "SkinButton.h"
#include "SkinDialog.h"
#include "SkinSplitter.h"
#include "SkinListCtrl.h"
#include "SkinComboBox.h"
#include "SkinHyperLink.h"
#include "SkinResourceManager.h"

//////////////////////////////////////////////////////////////////////////////////
//静态变量

CSkinResourceManager *	CSkinResourceManager::m_pSkinResourceManager=NULL;

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CSkinResourceManager::CSkinResourceManager()
{
	//设置变量
	ASSERT(m_pSkinResourceManager==NULL);
	if (m_pSkinResourceManager==NULL) m_pSkinResourceManager=this;

	return;
}

//析构函数
CSkinResourceManager::~CSkinResourceManager()
{
	//设置变量
	ASSERT(m_pSkinResourceManager==this);
	if (m_pSkinResourceManager==this) m_pSkinResourceManager=NULL;

	return;
}

//设置资源
VOID CSkinResourceManager::SetSkinResource(const tagSkinRenderInfo & SkinRenderInfo)
{
	//设置渲染
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();
	if (pSkinRenderManager!=NULL) pSkinRenderManager->SetSkinRenderInfo(SkinRenderInfo);

	//创建字体
	if (m_DefaultFont.m_hObject==NULL)
	{
		m_DefaultFont.CreateFont(-12,0,0,0,0,0,0,0,0,3,2,ANTIALIASED_QUALITY,2,TEXT("宋体"));
	}

	//更新资源
	CSkinEdit::m_SkinAttribute.Initialization(pSkinRenderManager);
	CSkinDialog::m_SkinAttribute.Initialization(pSkinRenderManager);
	CSkinComboBox::m_SkinAttribute.Initialization(pSkinRenderManager);
	CSkinListCtrl::m_SkinAttribute.Initialization(pSkinRenderManager);
	CSkinHyperLink::m_SkinAttribute.Initialization(pSkinRenderManager);
	CSkinMenuKernel::m_SkinAttribute.Initialization(pSkinRenderManager);

	//更新界面
	DWORD dwThreadID=GetCurrentThreadId();
	EnumThreadWindows(dwThreadID,EnumWindowsProc,(LPARAM)pSkinRenderManager);

	return;
}

//枚举函数
BOOL CALLBACK CSkinResourceManager::EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	//枚举窗口
	EnumChildWindows(hWnd,EnumWindowsProc,lParam);

	//重画窗口
	RedrawWindow(hWnd,NULL,NULL,RDW_FRAME|RDW_ERASE|RDW_ERASENOW|RDW_UPDATENOW|RDW_INVALIDATE|RDW_NOCHILDREN);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
