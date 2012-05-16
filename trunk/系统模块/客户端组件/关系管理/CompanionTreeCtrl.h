#ifndef COMPANION_TREE_CTRL_HEAD_FILE
#define COMPANION_TREE_CTRL_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Companion.h"

//////////////////////////////////////////////////////////////////////////

//��ϵ�������ؼ�
class CCompanionTreeCtrl : public CTreeCtrl
{
	//��������
protected:
	HTREEITEM							m_hItemDetest;					//�������
	HTREEITEM							m_hItemFriend;					//��������

	//�ؼ�����
protected:
	CImageList							m_ImageList;					//ͼƬ��Դ

	//��������
public:
	//���캯��
	CCompanionTreeCtrl();
	//��������
	virtual ~CCompanionTreeCtrl();

	//���غ���
protected:
	//����Ϣ
	virtual void PreSubclassWindow();

	//���ܺ���
public:
	//չ���б�
	bool ExpandListItem();
	//�����ϵ
	bool InsertCompanionItem(const tagCompanionItem * pCompanionItem);
	//ɾ����ϵ
	bool DeleteCompanionItem(const tagCompanionItem * pCompanionItem);

	//�ڲ�����
private:
	//Ѱ����
	HTREEITEM SearchCompanionItem(HTREEITEM hRootTreeItem, DWORD_PTR dwParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif