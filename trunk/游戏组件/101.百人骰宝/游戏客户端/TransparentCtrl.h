#pragma once


// CTransparentCtrl

class CTransparentCtrl : public CStatic
{
	DECLARE_DYNAMIC(CTransparentCtrl)

public:
	CTransparentCtrl();
	virtual ~CTransparentCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();

	CPngImage m_PngBack;
	INT       m_iXPos;

public:
	virtual void SetWindowText(LPCTSTR lpszString);
	virtual int GetWindowText(LPTSTR lpszStringBuf, int nMaxCount);
	virtual void GetWindowText(CString& rString);
	virtual int GetWindowTextLength();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);	

protected:
	CString  m_csText;
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


