#ifndef DLG_STATUS_HEAD_FILE
#define DLG_STATUS_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////////////

//״̬�ӿ�
interface IStatusViewSink
{
	//ȡ������
	virtual VOID OnStatusCancel()=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//״̬����
class CDlgStatus : public CDialog
{
	//״̬����
protected:
	UINT							m_uImageIndex;						//λͼλ��
	CString							m_strDescribe;						//������Ϣ

	//�ӿڱ���
protected:
	IStatusViewSink *				m_pIStatusViewSink;					//�ص��ӿ�

	//�ؼ�����
protected:
	CSkinButton						m_btGiveUp;							//ȡ����ť
	CSkinLayered					m_SkinLayered;						//�ֲ㴰��

	//��������
public:
	//���캯��
	CDlgStatus();
	//��������
	virtual ~CDlgStatus();

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��������
	virtual BOOL OnInitDialog();

	//���ܺ���
public:
	//�رմ���
	VOID HideStatusWindow();
	//������Ϣ
	VOID ShowStatusWindow(LPCTSTR pszDescribe);
	//���ýӿ�
	VOID SetStatusViewSink(IStatusViewSink * pIStatusViewSink);

	//��Ϣӳ��
protected:
	//ȡ����ť
	VOID OnBnClickedGiveUp();
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//��ʾ��Ϣ
	VOID OnShowWindow(BOOL bShow, UINT nStatus);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//λ�øı�
	VOID OnWindowPosChanged(WINDOWPOS * lpWndPos);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif