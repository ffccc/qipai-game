#ifndef GAME_PROPERTY_MANAGER_HEAD_FILE
#define GAME_PROPERTY_MANAGER_HEAD_FILE

#pragma once

#include "GamePropertyHead.h"
#include "GamePropertyItem.h"

//////////////////////////////////////////////////////////////////////////////////

//��������
typedef CMap<WORD,WORD,CGamePropertyItem *,CGamePropertyItem *> CMapPropertyItem;

//////////////////////////////////////////////////////////////////////////////////

//���߹���
class GAME_PROPERTY_CLASS CGamePropertyManager
{
	//��������
protected:
	CMapPropertyItem				m_MapPropertyItem;					//��������

	//��̬����
protected:
	static CGamePropertyManager *	m_pGamePropertyManager;				//����ָ��

	//��������
public:
	//���캯��
	CGamePropertyManager();
	//��������
	virtual ~CGamePropertyManager();

	//���ú���
public:
	//��ȡ����
	CGamePropertyItem * GetPropertyItem(WORD wIndex);
	//��������
	CGamePropertyItem * CreatePropertyItem(tagPropertyInfo & PropertyInfo);

	//��̬����
public:
	//��ȡ����
	static CGamePropertyManager * GetInstance() { return m_pGamePropertyManager; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif