#include "StdAfx.h"
#include "DlgBuyProperty.h"
#include "GamePropertyItem.h"
#include "GamePropertyManager.h"

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyItem::CGamePropertyItem()
{
	//设置变量
	ZeroMemory(&m_PropertyInfo,sizeof(m_PropertyInfo));
	ZeroMemory(&m_PropertyAttrib,sizeof(m_PropertyAttrib));

	return;
}

//析构函数
CGamePropertyItem::~CGamePropertyItem()
{
}

//使用道具
VOID CGamePropertyItem::ShowBuyPropertyWnd(IGamePropertySink * pIGamePropertySink)
{
	//购买窗口
	CDlgBuyProperty::ShowBuyPropertyWnd(this,pIGamePropertySink);

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyRedFlower::CGamePropertyRedFlower()
{
	//设置变量
	m_PropertyAttrib.wIndex=PROPERTY_ID_RED_FLOWER;
	m_PropertyAttrib.wServiceArea=PT_SERVICE_AREA_PLAYER|PT_SERVICE_AREA_LOOKON;
	lstrcpyn(m_PropertyAttrib.szPropertyName,TEXT("鲜花道具"),CountArray(m_PropertyAttrib.szPropertyName));
	lstrcpyn(m_PropertyAttrib.szRegulationsInfo,TEXT("获赠者将出现手持鲜花的动画与声效"),CountArray(m_PropertyAttrib.szRegulationsInfo));
	
	return;
}

//析构函数
CGamePropertyRedFlower::~CGamePropertyRedFlower()
{
}

//图片信息
VOID CGamePropertyRedFlower::GetImageInfo(tagPropertyImage & PropertyImage)
{
	//设置变量
	PropertyImage.pszImageID=TEXT("IMAGE_RED_FLOWER");
	PropertyImage.pszSymbolID=TEXT("SYMBOL_RED_FLOWER");
	PropertyImage.hResInstance=GetModuleHandle(GAME_PROPERTY_DLL_NAME);

	return;
}

//////////////////////////////////////////////////////////////////////////////////
