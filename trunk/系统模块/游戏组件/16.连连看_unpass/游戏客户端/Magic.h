#pragma once

#define					ID_MAGIC						3335

// CMagic �Ի���

class CMagic : public CDialog
{
	DECLARE_DYNAMIC(CMagic)

public:
	CMagic(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMagic();

// �Ի�������
	enum { IDD = IDD_MAGIC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	CSkinImage m_ImageBall;
	CSkinImage m_ImageBg;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
