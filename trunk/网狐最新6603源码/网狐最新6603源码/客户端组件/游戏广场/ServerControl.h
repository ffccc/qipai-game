#ifndef SERVER_CONTROL_HEAD_FILE
#define SERVER_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////

//�˺ſؼ�
class CServerControl : public CSkinComboBox
{
	//��������
public:
	//���캯��
	CServerControl();
	//��������
	virtual ~CServerControl();

	//���غ���
protected:
	//�󶨺���
	virtual VOID PreSubclassWindow();
	//��������
	virtual VOID MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif