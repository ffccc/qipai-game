#include "StdAfx.h"
#include "SkinEdit.h"
#include "SkinDialog.h"
#include "SkinButton.h"
#include "SkinListCtrl.h"
#include "SkinSplitter.h"
#include "SkinHyperLink.h"
#include "SkinResourceManager.h"

//////////////////////////////////////////////////////////////////////////
//��̬����

CFont				CSkinResourceManager::m_DefaultFont;				//Ĭ������
ISkinResource *		CSkinResourceManager::m_pISkinResource=NULL;		//��Դ�ӿ�
//////////////////////////////////////////////////////////////////////////

//���캯��
CSkinResourceManager::CSkinResourceManager()
{
	return;
}

//��������
CSkinResourceManager::~CSkinResourceManager()
{
	return;
}

//������Դ
bool CSkinResourceManager::LoadSkinResource(IUnknownEx * pIUnknownEx)
{
	//Ч�����
	ASSERT(pIUnknownEx!=NULL);
	if (pIUnknownEx==NULL) return false;

	//��ѯ�ӿ�
	ISkinResource * pISkinResource=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ISkinResource);
	ASSERT(pISkinResource!=NULL);
	if (pISkinResource==NULL) return false;

	//������Դ
	if (m_pISkinResource!=pISkinResource)
	{
		//�ͷ���Դ
		m_DefaultFont.DeleteObject();
		if (m_pISkinResource!=NULL) m_pISkinResource->Release();

		//������Դ
		m_pISkinResource=pISkinResource;
		if (m_pISkinResource->CreateDefaultFont(m_DefaultFont)==false) return false;

		//����֪ͨ
		if (CSkinEdit::m_SkinAttribute.UpdateSkinResource(pISkinResource)==false) return false;
		if (CSkinButton::m_SkinAttribute.UpdateSkinResource(pISkinResource)==false) return false;
		if (CSkinDialog::m_SkinAttribute.UpdateSkinResource(pISkinResource)==false) return false;
		if (CSkinSplitter::m_SkinAttribute.UpdateSkinResource(pISkinResource)==false) return false;
		if (CSkinHeaderCtrl::m_SkinAttribute.UpdateSkinResource(pISkinResource)==false) return false;
		if (CSkinHyperLink::m_SkinAttribute.UpdateSkinResource(pISkinResource)==false) return false;
	}

	return true;
}

//��ȡ��Դ
VOID * CSkinResourceManager::GetSkinResource(REFGUID Guid, DWORD dwQueryVer)
{
	if (m_pISkinResource==NULL) return NULL;
	return m_pISkinResource->QueryInterface(Guid,dwQueryVer);
}

//////////////////////////////////////////////////////////////////////////
