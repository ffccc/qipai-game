#include "StdAfx.h"
#include "DlgBuyProperty.h"
#include "GamePropertyItem.h"
#include "GamePropertyManager.h"

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGamePropertyItem::CGamePropertyItem()
{
	//���ñ���
	ZeroMemory(&m_PropertyInfo,sizeof(m_PropertyInfo));
	ZeroMemory(&m_PropertyAttrib,sizeof(m_PropertyAttrib));

	return;
}

//��������
CGamePropertyItem::~CGamePropertyItem()
{
}

//ʹ�õ���
VOID CGamePropertyItem::ShowBuyPropertyWnd(IGamePropertySink * pIGamePropertySink)
{
	//���򴰿�
	CDlgBuyProperty::ShowBuyPropertyWnd(this,pIGamePropertySink);

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGamePropertyRedFlower::CGamePropertyRedFlower()
{
	//���ñ���
	m_PropertyAttrib.wIndex=PROPERTY_ID_RED_FLOWER;
	m_PropertyAttrib.wServiceArea=PT_SERVICE_AREA_PLAYER|PT_SERVICE_AREA_LOOKON;
	lstrcpyn(m_PropertyAttrib.szPropertyName,TEXT("�ʻ�����"),CountArray(m_PropertyAttrib.szPropertyName));
	lstrcpyn(m_PropertyAttrib.szRegulationsInfo,TEXT("�����߽������ֳ��ʻ��Ķ�������Ч"),CountArray(m_PropertyAttrib.szRegulationsInfo));
	
	return;
}

//��������
CGamePropertyRedFlower::~CGamePropertyRedFlower()
{
}

//ͼƬ��Ϣ
VOID CGamePropertyRedFlower::GetImageInfo(tagPropertyImage & PropertyImage)
{
	//���ñ���
	PropertyImage.pszImageID=TEXT("IMAGE_RED_FLOWER");
	PropertyImage.pszSymbolID=TEXT("SYMBOL_RED_FLOWER");
	PropertyImage.hResInstance=GetModuleHandle(GAME_PROPERTY_DLL_NAME);

	return;
}

//////////////////////////////////////////////////////////////////////////////////
