#include "StdAfx.h"
#include "Resource.h"
#include "GamePlaza.h"
#include "ServerControl.h"

//////////////////////////////////////////////////////////////////////////////////

//��������
#define ITEM_CY						22									//�����С

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CServerControl, CSkinComboBox)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CServerControl::CServerControl()
{
}

//��������
CServerControl::~CServerControl()
{
}

//�󶨺���
VOID CServerControl::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	//�༭�ؼ�
	m_SkinComboBoxEdit.LimitText(LEN_SERVER-1);

	//���ÿؼ�
	SetButtonImage(AfxGetInstanceHandle(),IDB_BT_COMBOBOX,CSize(22,18));

	return;
}

//��������
VOID CServerControl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	//����λ��
	if (lpMeasureItemStruct->itemID<(UINT)GetCount())
	{
		lpMeasureItemStruct->itemWidth=0;
		lpMeasureItemStruct->itemHeight=ITEM_CY;
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
