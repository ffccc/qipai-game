#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//��Ϣ����

#define IDM_CHESS_COUNT_RESPONSION	(WM_USER+400)						//Ӧ����Ϣ

//////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CChessCountInfo : public CDialog
{
	//��������
protected:
	float							m_fBlackCount;						//������Ŀ
	float							m_fWhiteCount;						//������Ŀ
	float							m_fCommomCount;						//������Ŀ

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
	CChessCountInfo();
	//��������
	virtual ~CChessCountInfo();

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
	//������Ϣ
	void SetChessCountInfo(float fBlackCount, float fWhiteCount, float fCommomCount);

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

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
