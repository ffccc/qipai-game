#pragma once
#include "Resource.h"

#define FLAG_WIDTH					30									//��־���
#define FLAG_HEIGTH					30									//��־�߶�
// CShowFlagDlg dialog
class CChessBorad ;

class CShowFlagDlg : public CDialog
{
	DECLARE_DYNAMIC(CShowFlagDlg)

public:
	CShowFlagDlg(CWnd* pParent = NULL);   // standard constructor
	CShowFlagDlg(CChessBorad *Parent) ;
	virtual ~CShowFlagDlg();

// Dialog Data
	enum { IDD = IDD_FLAGDLG };
public:
	CSkinImage						m_ImageFlag;						//��־ͼ��

	//���ܺ���
public:
	//����ת��
	void SwitchMousePos(const CPoint &point , BYTE & bXPos, BYTE & bYPos) ;

private:
	CChessBorad						*m_pParent ;						//����ָ��

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
};
