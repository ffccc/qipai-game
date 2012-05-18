#ifndef DLG_SERVER_ITEM_HEAD_FILE
#define DLG_SERVER_ITEM_HEAD_FILE

#pragma once

#include "ModuleManagerHead.h"
#include "ServerInfoManager.h"
#include "ServerListControl.h"

//////////////////////////////////////////////////////////////////////////////////

//���Ի���
class MODULE_MANAGER_CLASS CDlgServerItem : public CDialog
{
	//�б����
protected:
	CServerInfoBuffer				m_ServerInfoBuffer;					//ģ����Ϣ
	CServerListControl				m_ServerListControl;				//�����б�

	//���ñ���
public:
	tagModuleInitParameter			m_ModuleInitParameter;				//���ò���

	//��������
public:
	//���캯��
	CDlgServerItem();
	//��������
	virtual ~CDlgServerItem();

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	virtual VOID OnOK();

	//���ú���
public:
	//�򿪷���
	bool OpenGameServer();
	//�򿪷���
	bool OpenGameServer(WORD wServerID);

	//���ܺ���
protected:
	//���ط���
	bool LoadDBServerItem();
	//���°�ť
	bool UpdateControlStatus();
	//��ȡ����
	bool GetModuleInitParameter(tagGameServerInfo * pGameServerInfo, bool bAutoMode);

	//��ť��Ϣ
protected:
	//���ط���
	VOID OnBnClickedLoadServer();
	//ˢ���б�
	VOID OnBnClickedReLoadList();
	//ɾ������
	VOID OnBnClickedDeleteServer();
	//�������
	VOID OnBnClickedServiceAttrib();

	//�ؼ���Ϣ
protected:
	//˫���б�
	VOID OnNMDblclkServerList(NMHDR * pNMHDR, LRESULT * pResult);
	//ѡ��ı�
	VOID OnLvnItemChangedServerList(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif