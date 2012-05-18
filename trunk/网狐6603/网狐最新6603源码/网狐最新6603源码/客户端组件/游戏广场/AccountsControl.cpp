#include "StdAfx.h"
#include "Resource.h"
#include "AccountsControl.h"

//////////////////////////////////////////////////////////////////////////////////

//常量定义
#define ITEM_CY						22									//子项大小

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CAccountsCtrl, CSkinComboBox)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CAccountsCtrl::CAccountsCtrl()
{
}

//析构函数
CAccountsCtrl::~CAccountsCtrl()
{
}

//绑定函数
VOID CAccountsCtrl::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	//设置控件
	m_SkinComboBoxEdit.LimitText(LEN_ACCOUNTS-1);

	//设置控件
	SetButtonImage(AfxGetInstanceHandle(),IDB_BT_COMBOBOX,CSize(22,18));

	return;
}

//测量子项
VOID CAccountsCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	//设置变量
	lpMeasureItemStruct->itemWidth=0;
	lpMeasureItemStruct->itemHeight=ITEM_CY;

	return;
}

//////////////////////////////////////////////////////////////////////////////////
