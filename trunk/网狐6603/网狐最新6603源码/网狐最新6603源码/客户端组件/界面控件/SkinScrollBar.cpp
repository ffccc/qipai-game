#include "StdAfx.h"
#include "Resource.h"
#include "SkinScrollBar.h"

//////////////////////////////////////////////////////////////////////////////////
//��̬����

CSkinScrollBarAttribute			CSkinScrollBar::m_SkinAttribute;		//��Դ����

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CSkinScrollBarAttribute::CSkinScrollBarAttribute()
{
	//������Դ
	m_ImageScroll.LoadFromResource(GetModuleHandle(SKIN_CONTROL_DLL_NAME),IDB_SKIN_SCROLL);

	return;
}
	
//��������
CSkinScrollBarAttribute::~CSkinScrollBarAttribute()
{
	//������Դ
	if (m_ImageScroll.IsNull()==false)
	{
		m_ImageScroll.Destroy();
	}

	return;
}

//������Դ
bool CSkinScrollBarAttribute::Initialization(CSkinRenderManager * pSkinRenderManager)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CSkinScrollBar::CSkinScrollBar()
{
}

//��������
CSkinScrollBar::~CSkinScrollBar()
{
}

//���ù���
VOID CSkinScrollBar::InitScroolBar(CWnd * pWnd)
{
	//���ù���
	SkinSB_Init(pWnd->GetSafeHwnd(),m_SkinAttribute.m_ImageScroll);

	return;
}

//////////////////////////////////////////////////////////////////////////////////
