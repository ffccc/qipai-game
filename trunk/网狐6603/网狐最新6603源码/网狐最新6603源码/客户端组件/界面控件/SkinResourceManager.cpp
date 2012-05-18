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
//��̬����

CSkinResourceManager *	CSkinResourceManager::m_pSkinResourceManager=NULL;

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CSkinResourceManager::CSkinResourceManager()
{
	//���ñ���
	ASSERT(m_pSkinResourceManager==NULL);
	if (m_pSkinResourceManager==NULL) m_pSkinResourceManager=this;

	return;
}

//��������
CSkinResourceManager::~CSkinResourceManager()
{
	//���ñ���
	ASSERT(m_pSkinResourceManager==this);
	if (m_pSkinResourceManager==this) m_pSkinResourceManager=NULL;

	return;
}

//������Դ
VOID CSkinResourceManager::SetSkinResource(const tagSkinRenderInfo & SkinRenderInfo)
{
	//������Ⱦ
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();
	if (pSkinRenderManager!=NULL) pSkinRenderManager->SetSkinRenderInfo(SkinRenderInfo);

	//��������
	if (m_DefaultFont.m_hObject==NULL)
	{
		m_DefaultFont.CreateFont(-12,0,0,0,0,0,0,0,0,3,2,ANTIALIASED_QUALITY,2,TEXT("����"));
	}

	//������Դ
	CSkinEdit::m_SkinAttribute.Initialization(pSkinRenderManager);
	CSkinDialog::m_SkinAttribute.Initialization(pSkinRenderManager);
	CSkinComboBox::m_SkinAttribute.Initialization(pSkinRenderManager);
	CSkinListCtrl::m_SkinAttribute.Initialization(pSkinRenderManager);
	CSkinHyperLink::m_SkinAttribute.Initialization(pSkinRenderManager);
	CSkinMenuKernel::m_SkinAttribute.Initialization(pSkinRenderManager);

	//���½���
	DWORD dwThreadID=GetCurrentThreadId();
	EnumThreadWindows(dwThreadID,EnumWindowsProc,(LPARAM)pSkinRenderManager);

	return;
}

//ö�ٺ���
BOOL CALLBACK CSkinResourceManager::EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	//ö�ٴ���
	EnumChildWindows(hWnd,EnumWindowsProc,lParam);

	//�ػ�����
	RedrawWindow(hWnd,NULL,NULL,RDW_FRAME|RDW_ERASE|RDW_ERASENOW|RDW_UPDATENOW|RDW_INVALIDATE|RDW_NOCHILDREN);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
