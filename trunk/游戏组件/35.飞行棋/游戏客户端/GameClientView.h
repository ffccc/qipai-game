#pragma once

#include "Stdafx.h"
#include "D3DWnd.h"
#include "GameScoreView.h"

//////////////////////////////////////////////////////////////////////////
//��Ϣ����

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	friend class CGameClientDlg;
	//�������
protected:
	CD3DWnd m_wndD3D;
	CGameScoreView m_GameScoreView;
	//�ؼ�����
public:

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();

	//�̳к���
private:
	//���ý���
	virtual void ResetGameView();
	//�����ؼ�
	virtual void RectifyGameView(int nWidth, int nHeight);
	//�滭����
	virtual void DrawGameView(CDC * pDC, int nWidth, int nHeight);

	//��Ϣӳ��
protected:
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()

};

//////////////////////////////////////////////////////////////////////////
