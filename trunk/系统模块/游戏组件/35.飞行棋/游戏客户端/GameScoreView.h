#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CGameScoreView : public CSkinDialogEx
{


	//�ؼ�����
public:
	CSkinButton						m_btOK;								//ȷ����ť
	CSkinButton						m_btCancel;							//ȡ����ť

	//��������
public:
	//���캯��
	CGameScoreView();
	//��������
	virtual ~CGameScoreView();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
public:
	void SetScoreText(BYTE bTextIndex, const TCHAR *strName, long lScore);
	void SetWinnerText(const TCHAR *strName);
	afx_msg void OnPaint();
};

//////////////////////////////////////////////////////////////////////////
