#include "StdAfx.h"
#include "GamePropertyManager.h"

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGamePropertyManager::CGamePropertyManager()
{
}

//��������
CGamePropertyManager::~CGamePropertyManager()
{
}

//���õ���
bool CGamePropertyManager::SetGamePropertyInfo(tagPropertyInfo PropertyInfo[], WORD wPropertyCount)
{
	//���ñ���
	m_PropertyInfoArray.SetSize(wPropertyCount);

	//��������
	CopyMemory(m_PropertyInfoArray.GetData(),PropertyInfo,sizeof(tagPropertyInfo)*wPropertyCount);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
