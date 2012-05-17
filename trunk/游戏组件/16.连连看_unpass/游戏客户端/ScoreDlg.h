#pragma once
#include"Resource.h"

// CScoreDlg �Ի���

class CScoreDlg : public CDialog
{
	DECLARE_DYNAMIC(CScoreDlg)

public:
	CScoreDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CScoreDlg();
	virtual BOOL OnInitDialog();

    // �Ի�������
	enum { IDD = IDD_SCORE };

	//��������
protected:
	LONG							m_lTax;								//��Ϸ˰��
	LONG							m_lScore[MAX_CHAIR];				//��Ϸ����
	float							m_lPer[MAX_CHAIR];			//��Ϸ�÷�
	TCHAR							m_szUserName[MAX_CHAIR][NAME_LEN];	//�û�����

	bool							m_isExer;							//�Ƿ�ʱ��ϰģʽ

	//��Դ����
protected:
	CSkinImage						m_ImageBack;						//����ͼ��
	CSkinImage						m_ImageBackExer;					//����ͼ��

	CSkinImage						m_ImageRate_bg;						//��������
	CSkinImage						m_ImageRate;				


	//���ܺ���
public:
	//���û���
	void ResetScore();
	//����˰��
	void SetTax(LONG lTax);
	//���û���
	void SetGameScore(WORD wChairID, LPCTSTR pszUserName, float per, int lScore);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	void SetCurrMode(bool Exer);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
