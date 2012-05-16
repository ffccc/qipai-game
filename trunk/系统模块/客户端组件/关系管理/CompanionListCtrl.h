#ifndef COMPANION_LIST_CTRL_HEAD_FILE
#define COMPANION_LIST_CTRL_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Companion.h"

//////////////////////////////////////////////////////////////////////////

//��ϵ�б���ͼ
class CCompanionListCtrl : public CSkinListCtrl
{
	//�ؼ�����
protected:
	static CImageList					m_ImageList;					//ͼƬ��Դ

	//��������
public:
	//���캯��
	CCompanionListCtrl();
	//��������
	virtual ~CCompanionListCtrl();

	//���غ���
protected:
	//����Ϣ
	virtual void PreSubclassWindow();

	//���ܺ���
public:
	//�����ϵ
	bool InsertCompanionItem(const tagCompanionItem * pCompanionItem);
	//���¹�ϵ
	bool UpdateCompanionItem(const tagCompanionItem * pCompanionItem);
	//ɾ����ϵ
	bool DeleteCompanionItem(const tagCompanionItem * pCompanionItem);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
