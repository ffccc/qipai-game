#include "StdAfx.h"
#include "GamePropertyManager.h"

//////////////////////////////////////////////////////////////////////////////////

//��̬����
CGamePropertyManager * CGamePropertyManager::m_pGamePropertyManager=NULL;//����ָ��

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGamePropertyManager::CGamePropertyManager()
{
	//���ñ���
	ASSERT(m_pGamePropertyManager==NULL);
	if (m_pGamePropertyManager==NULL) m_pGamePropertyManager=this;

	return;
}

//��������
CGamePropertyManager::~CGamePropertyManager()
{
	//���ñ���
	ASSERT(m_pGamePropertyManager==this);
	if (m_pGamePropertyManager==this) m_pGamePropertyManager=NULL;

	return;
}

//��ȡ����
CGamePropertyItem * CGamePropertyManager::GetPropertyItem(WORD wIndex)
{
	//���ҵ���
	CGamePropertyItem * pGamePropertyItem=NULL;
	m_MapPropertyItem.Lookup(wIndex,pGamePropertyItem);

	return pGamePropertyItem;
}

//��������
CGamePropertyItem * CGamePropertyManager::CreatePropertyItem(tagPropertyInfo & PropertyInfo)
{
	//�����ִ�
	CGamePropertyItem * pGamePropertyItem=NULL;
	m_MapPropertyItem.Lookup(PropertyInfo.wIndex,pGamePropertyItem);

	//�ִ��ж�
	if (pGamePropertyItem!=NULL)
	{
		return pGamePropertyItem;
	}

	//��������
	switch (PropertyInfo.wIndex)
	{
	case PROPERTY_ID_RED_FLOWER:	//��ɫ�ʻ�
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

	//�����ж�
	if (pGamePropertyItem!=NULL)
	{
		//���õ���
		pGamePropertyItem->m_PropertyInfo=PropertyInfo;
		m_MapPropertyItem[PropertyInfo.wIndex]=pGamePropertyItem;

		return pGamePropertyItem;
	}

	//�������
	ASSERT(FALSE);

	return NULL;
}

//////////////////////////////////////////////////////////////////////////////////
