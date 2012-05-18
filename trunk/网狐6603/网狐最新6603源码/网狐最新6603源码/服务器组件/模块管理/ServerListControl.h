#ifndef SERVER_LIST_CONTROL_HEAD_FILE
#define SERVER_LIST_CONTROL_HEAD_FILE

#pragma once

#include "ListControl.h"
#include "ModuleManagerHead.h"

//////////////////////////////////////////////////////////////////////////////////

//�����б�
class MODULE_MANAGER_CLASS CServerListControl : public CListControl
{
	//��������
public:
	//���캯��
	CServerListControl();
	//��������
	virtual ~CServerListControl();

	//���غ���
public:
	//�����б�
	virtual VOID InitListControl();
	//��������
	virtual INT SortListItemData(LPARAM lParam1, LPARAM lParam2, WORD wColumnIndex);
	//��ȡ��ɫ
	virtual VOID GetListItemColor(LPARAM lItemParam, UINT uItemStatus, tagListItemColor & ListItemColor);

	//���ܺ���
public:
	//�����б�
	bool InsertServerInfo(tagGameServerInfo * pGameServerInfo);

	//��������
private:
	//��������
	WORD GetInsertIndex(tagGameServerInfo * pGameServerInfo);
	//�����ַ�
	LPCTSTR GetDescribeString(tagGameServerInfo * pGameServerInfo, WORD wColumnIndex);
};

//////////////////////////////////////////////////////////////////////////////////

#endif