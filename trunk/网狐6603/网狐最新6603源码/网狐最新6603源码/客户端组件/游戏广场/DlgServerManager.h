#ifndef DLG_SERVER_MANAGER_HEAD_FILE
#define DLG_SERVER_MANAGER_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////

//�������
class CDlgServerManager : public CSkinDialog
{
	//��Դ����
protected:
	CPen							m_BoradPen;							//��Դ����
	CBrush							m_FrameBrush;						//��Դ����

	//��������
protected:
	ITCPSocket *					m_pITCPSocket;						//����ӿ�
	IClientUserItem *				m_pIMySelfUserItem;					//�Լ�ָ��

	//���ܰ�ť
protected:
	CSkinButton			 			m_btOk;								//�洢�˳�
	CSkinButton			 			m_btCancel;							//������ť

	//�б�����
protected:
	CSkinEdit						m_edKindID;							//�༭�ؼ�
	CSkinEdit						m_edNodeID;							//�༭�ؼ�
	CSkinEdit						m_edSortID;							//�༭�ؼ�

	//��������
protected:
	CSkinEdit						m_edServerName;						//�༭�ؼ�
	CSkinEdit						m_edRevenueRatio;					//�༭�ؼ�
	CSkinEdit						m_edRestrictScore;					//�༭�ؼ�

	//��������
protected:
	CSkinEdit						m_edMinEnterScore;					//�༭�ؼ�
	CSkinEdit						m_edMaxEnterScore;					//�༭�ؼ�
	CSkinEdit						m_edMinTableScore;					//�༭�ؼ�
	CSkinComboBox					m_comMinEnterMember;				//�༭�ؼ�
	CSkinComboBox					m_comMaxEnterMember;				//�༭�ؼ�

	//��������
public:
	//���캯��
	CDlgServerManager();
	//��������
	virtual ~CDlgServerManager();

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//���ú���
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();

	//���غ���
protected:
	//�滭��Ϣ
	virtual VOID OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight);

	//���ܺ���
public:
	//���ù���
	VOID Initialization(ITCPSocket * pITCPSocket, IClientUserItem * pIMySelfUserItem);
	//��������
	VOID SetServerOptionInfo(tagServerOptionInfo & ServerOptionInfo, DWORD dwRuleMask);

	//��Ϣӳ��
protected:
	//�ؼ���ɫ
	HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif