#include "StdAfx.h"
#include "Resource.h"
#include "AccountsControl.h"

//////////////////////////////////////////////////////////////////////////////////

//��������
#define ITEM_CY						22									//�����С

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CAccountsCtrl, CSkinComboBox)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CAccountsCtrl::CAccountsCtrl()
{
}

//��������
CAccountsCtrl::~CAccountsCtrl()
{
}

//�󶨺���
VOID CAccountsCtrl::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	//���ÿؼ�
	m_SkinComboBoxEdit.LimitText(LEN_ACCOUNTS-1);

	//���ÿؼ�
	SetButtonImage(AfxGetInstanceHandle(),IDB_BT_COMBOBOX,CSize(22,18));

	return;
}

//��������
VOID CAccountsCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	//���ñ���
	lpMeasureItemStruct->itemWidth=0;
	lpMeasureItemStruct->itemHeight=ITEM_CY;

	return;
}

//////////////////////////////////////////////////////////////////////////////////
