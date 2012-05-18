#include "StdAfx.h"
#include "Resource.h"
#include "GamePlaza.h"
#include "ServerControl.h"

//////////////////////////////////////////////////////////////////////////////////

//常量定义
#define ITEM_CY						22									//子项大小

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CServerControl, CSkinComboBox)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CServerControl::CServerControl()
{
}

//析构函数
CServerControl::~CServerControl()
{
}

//绑定函数
VOID CServerControl::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	//编辑控件
	m_SkinComboBoxEdit.LimitText(LEN_SERVER-1);

	//设置控件
	SetButtonImage(AfxGetInstanceHandle(),IDB_BT_COMBOBOX,CSize(22,18));

	return;
}

//测量子项
VOID CServerControl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	//计算位置
	if (lpMeasureItemStruct->itemID<(UINT)GetCount())
	{
		lpMeasureItemStruct->itemWidth=0;
		lpMeasureItemStruct->itemHeight=ITEM_CY;
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
