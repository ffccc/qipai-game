#ifndef DLG_PROXY_CONFIG_HEAD_FILE
#define DLG_PROXY_CONFIG_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////

//��������
class CDlgProxyConfig : public CSkinDialog
{
	//�ؼ�����
protected:
	CSkinButton						m_btOk;								//ȷ����ť
	CSkinButton						m_btCancel;							//ȡ����ť
	CSkinButton						m_btProxyTest;						//���԰�ť
	CSkinComboBox					m_cmProxyType;						//��������

	//�༭�ؼ�
protected:
	CSkinEdit						m_edPassword;						//�û�����
	CSkinEdit						m_edProxyPort;						//����˿�
	CSkinEdit						m_edProxyServer;					//�����ַ
	CSkinEdit						m_edProxyUserName;					//�����û�
	CSkinEdit						m_edProxyPassword;					//��������

	//��������
public:
	//���캯��
	CDlgProxyConfig();
	//��������
	virtual ~CDlgProxyConfig();

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();

	//��������
private:
	//��ȡ��Ϣ
	bool GetInformation();

	//��ť��Ϣ
protected:
	//�������
	VOID OnBnClickedProxyTest();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif