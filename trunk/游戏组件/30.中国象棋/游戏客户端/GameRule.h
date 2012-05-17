#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//��Ϣ����

#define IDM_AGREE_GAME_RULE			(WM_USER+300)						//ͬ����Ϣ

//////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CGameRule : public CDialog
{
	//��Ϸ����
public:
	tagGameRuleInfo					m_GameRuleInfo;						//��Ϸ����

	//��Դ����
public:
	CBrush							m_Brush;							//������ˢ
	CSkinImage						m_ImageBack;						//������Դ

	//�ؼ�����
public:
	CSkinButton						m_btOk;								//��ť�ؼ�
	CSkinButton						m_btCancel;							//��ť�ؼ�

	//��������
public:
	//���캯��
	CGameRule();
	//��������
	virtual ~CGameRule();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	virtual void OnOK();
	//ȡ����Ϣ
	virtual void OnCancel();

	//���ܺ���
public:
	//����״̬
	void UpdateControlStatus();
	//�������
	void ControlEnable(bool bEnable);
	
	//�ڲ�����
private:
	//������ֵ
	bool SetComboBoxValue(UINT uComboBoxID, WORD wValue);
	//��ȡ��ֵ
	bool GetComboBoxValue(UINT uComboBoxID, WORD & wValue);

	//��Ϣӳ��
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//������Ϣ
	afx_msg void OnDestroy();
	//�滭����
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//�ؼ���ɫ
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);

	//�ؼ���Ϣ
protected:
	//�޶���ʱ
	afx_msg void OnBnClickedEnableStep();
	//�޶���ʱ
	afx_msg void OnBnClickedEnableLimit();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
