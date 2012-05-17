#ifndef DLG_SEARCH_TABLE_HEAD_FILE
#define DLG_SEARCH_TABLE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "TableViewFrame.h"

//////////////////////////////////////////////////////////////////////////

//��˵��
class CTableViewFrame;
class CServerViewItem;

//////////////////////////////////////////////////////////////////////////

//��������
class CDlgSearchTable : public CSkinDialog
{
	//��������
protected:
	WORD							m_wResultTableID;					//�������

	//�ؼ�����
public:
	CSkinButton						m_btSearch;							//��ѯ��ť
	CSkinButton						m_btCancel;							//�رհ�ť
	CSkinButton						m_btAutoJoin;						//���밴ť

	//�ӿڱ���
protected:
	CTableViewFrame *				m_pTableViewFrame;					//���ӽӿ�
	CServerViewItem *				m_pServerViewItem;					//��Ϸ����

	//��������
public:
	//���캯��
	CDlgSearchTable();
	//��������
	virtual ~CDlgSearchTable();

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//���ú���
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();
	//ȡ������
	virtual VOID OnCancel();

	//���ܺ���
public:
	//��������
	VOID SetTableViewFrame(CTableViewFrame * pTableViewFrame);
	//���÷���
	VOID SetServerViewItem(CServerViewItem * pServerViewItem);

	//��ť��Ϣ
protected:
	//���ټ���
	VOID OnBnClickedAutoJoin();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif