#ifndef SERVIVE_LOADER_DLG_HEAD_FILE
#define SERVIVE_LOADER_DLG_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "InitParamter.h"
#include "ServiceParameter.h"

//////////////////////////////////////////////////////////////////////////

//���ݿ���Ϣ
struct tagDBConnectInfo
{
	WORD							wDataBasePort;						//���ݿ�˿�
	DWORD							dwDataBaseAddr;						//���ݿ��ַ
	TCHAR							szDataBaseUser[32];					//���ݿ��û�
	TCHAR							szDataBasePass[32];					//���ݿ�����
};

//���鶨��
typedef CArrayTemplate<tagDBConnectInfo> CDBConnectInfoArray;			//��Ϣ����

//////////////////////////////////////////////////////////////////////////

//���öԻ���
class CSystemOptionDlg : public CDialog
{
	//���캯��
public:
	//�������
	CSystemOptionDlg();
	//��������
	virtual ~CSystemOptionDlg();

	//���غ���
protected:
	//��ʼ������
	virtual BOOL OnInitDialog();
	//�ؼ����໯
	virtual void DoDataExchange(CDataExchange * pDX);
	//ȷ������
	virtual void OnOK();

	//��Ϣӳ��
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

//����������
class CServiceLoaderDlg : public CDialog
{
	//�������
public:
	CGameServiceHelper				m_GameService;						//��Ϸ����

	//�ؼ�����
public:
	CRichEditTrace					m_RichEditCtrl;						//��Ϣ����
	CInitParamter					m_InitParamter;						//���ò���
	CServiceParameter				m_ServiceParameter;					//�������
	CDBConnectInfoArray				m_DBConnectInfoArray;				//��Ϣ����

	//��������
public:
	//���캯��
	CServiceLoaderDlg();
	//��������
	virtual ~CServiceLoaderDlg();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	virtual void OnOK();
	//ȡ������
	virtual void OnCancel();

	//�ڲ�����
protected:
	//��ȡ����
	const tagDBConnectInfo * GetDBConnectInfo(DWORD dwDataBaseAddr);
	//��ʾ����
	void ShowErrorMessasge(LPCTSTR pszString, enTraceLevel TraceLevel);

	//��Ϣӳ��
protected:
	//��������
	afx_msg void OnBnClickedStart();
	//ֹͣ����
	afx_msg void OnBnClickedStop();
	//��������
	afx_msg void OnBnClickedCreate();
	//�򿪷���
	afx_msg void OnBnClickedOpen();
	//���÷���
	afx_msg void OnBnClickedOption();
	//ϵͳ����
	afx_msg void OnBnClickedSystem();
	//��������
	afx_msg void OnBnClickedSaveOption();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnQueryEndSession();
};

//////////////////////////////////////////////////////////////////////////

#endif