#ifndef MANUAL_LIST_HEAD_FILE
#define MANUAL_LIST_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//�����б�
class CManualList : public CListBox
{
	//��������
protected:
	CBrush							m_BackBrush;						//������ˢ

	//��������
public:
	//���캯��
	CManualList();
	//��������
	virtual ~CManualList();

	//��Ϣӳ��
protected:
	//�ؼ���ɫ
	HBRUSH CtlColor(CDC * pDC, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
