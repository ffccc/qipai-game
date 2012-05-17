#ifndef SERVICE_PASSWORD_HEAD_FILE
#define SERVICE_PASSWORD_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "DlgServiceItem.h"

//////////////////////////////////////////////////////////////////////////////////

//�޸�����
class CDlgServicePassword : public CDlgServiceItem, public CMissionItem
{
	//��������
protected:
	bool							m_bLogonMission;					//�޸�����
	CMissionManager					m_MissionManager;					//�������

	//����ǿ��
protected:
	BYTE							m_cbLogonPasswordLevel;				//����ǿ��
	BYTE							m_cbInsurePasswordLevel;			//����ǿ��

	//��������
protected:
	TCHAR							m_szSrcPassword[LEN_PASSWORD];		//�û�����
	TCHAR							m_szDesPassword[LEN_PASSWORD];		//�û�����

	//�ؼ�����
protected:
	CPasswordControl				m_edLogonPassword1;					//�ʺ�����
	CPasswordControl				m_edLogonPassword2;					//�ʺ�����
	CPasswordControl				m_edLogonPassword3;					//�ʺ�����
	CPasswordControl				m_edInsurePassword1;				//��������
	CPasswordControl				m_edInsurePassword2;				//��������
	CPasswordControl				m_edInsurePassword3;				//��������

	//�ؼ�����
protected:
	CSkinButton						m_btLogonPassword;					//�޸İ�ť
	CSkinButton						m_btInsurePassword;					//�޸İ�ť

	//��������
public:
	//���캯��
	CDlgServicePassword();
	//��������
	virtual ~CDlgServicePassword();

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//���غ���
public:
	//��ȡ����
	virtual LPCTSTR GetServiceItemName() { return TEXT("�޸�����"); }

	//�����¼�
private:
	//�����¼�
	virtual bool OnEventMissionLink(INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//�¼�����
protected:
	//��ʼ����
	VOID OnMissionStart();
	//��ֹ����
	VOID OnMissionConclude();

	//��Ϣӳ��
protected:
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//��������
	VOID OnEnChangeLogonPassword();
	//��������
	VOID OnEnChangeInserePassword();
	//�����޸�
	VOID OnBnClickedLogonPassword();
	//�����޸�
	VOID OnBnClickedInsurePassword();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif