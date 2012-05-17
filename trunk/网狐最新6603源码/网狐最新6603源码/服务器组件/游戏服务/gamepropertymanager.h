#ifndef GAME_PROPERTY_MANAGER_HEAD_FILE
#define GAME_PROPERTY_MANAGER_HEAD_FILE

#pragma once

#include "GameServiceHead.h"

//////////////////////////////////////////////////////////////////////////////////

//���Ͷ���
typedef CWHArray<tagPropertyInfo>	CPropertyInfoArray;					//��������

//////////////////////////////////////////////////////////////////////////////////

//���߹���
class GAME_SERVICE_CLASS CGamePropertyManager
{
	//��������
protected:
	CPropertyInfoArray				m_PropertyInfoArray;				//��������

	//��������
public:
	//���캯��
	CGamePropertyManager();
	//��������
	virtual ~CGamePropertyManager();

	//���ú���
public:
	//���õ���
	bool SetGamePropertyInfo(tagPropertyInfo PropertyInfo[], WORD wPropertyCount);
};

//////////////////////////////////////////////////////////////////////////////////

#endif