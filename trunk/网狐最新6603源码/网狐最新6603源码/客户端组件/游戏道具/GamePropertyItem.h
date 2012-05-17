#ifndef GAME_PROPERTY_ITEM_HEAD_FILE
#define GAME_PROPERTY_ITEM_HEAD_FILE

#pragma once

#include "GamePropertyHead.h"

//////////////////////////////////////////////////////////////////////////////////

//��������
class GAME_PROPERTY_CLASS CGamePropertyItem
{
	//��Ԫ����
	friend class CGamePropertyManager;

	//��������
protected:
	tagPropertyInfo					m_PropertyInfo;						//������Ϣ
	tagPropertyAttrib				m_PropertyAttrib;					//��������

	//��������
public:
	//���캯��
	CGamePropertyItem();
	//��������
	virtual ~CGamePropertyItem();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage)=NULL;

	//���ܺ���
public:
	//��ȡ��Ϣ
	tagPropertyInfo * GetPropertyInfo() { return &m_PropertyInfo; }
	//��ȡ����
	tagPropertyAttrib * GetPropertyAttrib() { return & m_PropertyAttrib; }

	//���ܺ���
public:
	//ʹ�õ���
	VOID ShowBuyPropertyWnd(IGamePropertySink * pIGamePropertySink);
};

//////////////////////////////////////////////////////////////////////////////////

//��ɫ�ʻ�
class GAME_PROPERTY_CLASS CGamePropertyRedFlower : public CGamePropertyItem
{
	//��������
public:
	//���캯��
	CGamePropertyRedFlower();
	//��������
	virtual ~CGamePropertyRedFlower();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
};

//////////////////////////////////////////////////////////////////////////////////

#endif