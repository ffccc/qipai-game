#include "Stdafx.h"
#include "ManualList.h"

//////////////////////////////////////////////////////////////////////////

#define COLOR_LIST_BACK				RGB(210,193,158)					//������ɫ

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CManualList, CListBox)
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CManualList::CManualList()
{
	//������ˢ
	m_BackBrush.CreateSolidBrush(COLOR_LIST_BACK);

	return;
}

//��������
CManualList::~CManualList()
{
}

//�ؼ���ɫ
HBRUSH CManualList::CtlColor(CDC * pDC, UINT nCtlColor)
{
	//������ɫ
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetBkColor(COLOR_LIST_BACK);
	//pDC->SetTextColor(CSkinDialog::m_SkinAttribute.m_crControlText);
	
	return m_BackBrush;
}

//////////////////////////////////////////////////////////////////////////
