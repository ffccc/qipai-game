#ifndef DLG_RULE_OPTION_HEAD_FILE
#define DLG_RULE_OPTION_HEAD_FILE

#pragma once

#include "Stdafx.h"
//#include "GameClient.h"

//////////////////////////////////////////////////////////////////////////
//��Ϣ����

#define IDM_GAME_RULE_AGREE			(WM_USER+300)						//ͬ����Ϣ
#define IDM_GAME_RULE_REJECT		(WM_USER+301)						//�ܾ���Ϣ

//////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CDlgRuleOption : public CDialog
{
	//״̬����
protected:
	LONG							m_nResidualTime;					//ʣ��ʱ��

	//�������
protected:
	bool							m_bControl;							//���Ʊ�־
	bool							m_bSelectMode;						//ģʽ��־
	tagGameRuleInfo					m_GameRuleInfo;						//��Ϸ����

	//��Դ����
protected:
	CBrush							m_Brush;							//������ˢ
	CSkinImage						m_ImageViewBack;					//������Դ

	//�ؼ�����
protected:
	CSkinButton						m_btOk;								//��ť�ؼ�
	CSkinButton						m_btCancel;							//��ť�ؼ�

	//��������
public:
	//���캯��
	CDlgRuleOption();
	//��������
	virtual ~CDlgRuleOption();

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	virtual VOID OnOK();
	//ȡ����Ϣ
	virtual VOID OnCancel();

	//���ܺ���
public:
	//���ÿ���
	VOID EnableControl(bool bControl);
	//��ȡ����
	VOID GetGameRuleInfo(tagGameRuleInfo & GameRuleInfo);
	//��������
	VOID SetGameRuleInfo(const tagGameRuleInfo & GameRuleInfo, bool bControl, bool bSelectMode);

	//�ڲ�����
private:
	//����״̬
	VOID UpdateControlStatus();
	//������ֵ
	bool SetComboBoxValue(UINT uComboBoxID, WORD wValue);
	//��ȡ��ֵ
	bool GetComboBoxValue(UINT uComboBoxID, WORD & wValue);

	//��ť��Ϣ
protected:
	//����ģʽ
	VOID OnBnClickedFastMode();
	//����ģʽ
	VOID OnBnClickedSlowMode();
	//�Զ�ģʽ
	VOID OnBnClickedCustomMode();

	//��Ϣӳ��
protected:
	//�ػ�����
	VOID OnPaint();
	//������Ϣ
	VOID OnDestroy();
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//�ؼ���ɫ
	HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
