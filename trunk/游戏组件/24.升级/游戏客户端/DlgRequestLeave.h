#ifndef DLG_REQUEST_LEAVE_HEAD_FILE
#define DLG_REQUEST_LEAVE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//�����뿪
class CDlgRequestLeave : public CSkinDialogEx
{
	//��������
protected:
	CWnd 							*m_pGameFrameDlg;					//����ӿ�

	//�ؼ�����
public:
	CSkinButton						m_btOk;								//ȷ����ť
	CSkinButton						m_btCancel;							//ȡ����ť

	//��������
public:
	//���캯��
	CDlgRequestLeave();
	//��������
	virtual ~CDlgRequestLeave();

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();
	//ȡ����Ϣ
	virtual VOID OnCancel();

	//���ܺ���
public:
	//���ýӿ�
	void SetGameFrameEngine(CWnd * pIGameFrameDlg) { m_pGameFrameDlg=pIGameFrameDlg; }

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif