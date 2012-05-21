#ifndef SCORE_VIEW_HEAD_FILE
#define SCORE_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//�����
#define IDM_CONTINUE			(WM_USER+200)							//������Ϣ

//////////////////////////////////////////////////////////////////////////

//������ͼ��
class CScoreView : public CDialog
{
	//״̬����
protected:
	UINT							m_uLeaveTime;						//ʣ��ʱ��

	//��������
protected:
	LONG							m_lRevenue[GAME_PLAYER];			//��Ϸ˰��
	LONG							m_lGameScore[GAME_PLAYER];			//��Ϸ�÷�
	TCHAR							m_szUserName[GAME_PLAYER][NAME_LEN];//�û�����

	//��Դ����
protected:
	CSkinImage						m_ImageBack;						//����ͼ��

	//�ؼ�����
public:
	CSkinButton						m_btContinue;						//������ť
	CSkinButton						m_btQuitGame;						//�˳���ť

	//��������
public:
	//���캯��
	CScoreView(void);
	//��������
	virtual ~CScoreView(void);
	
	//���غ���
public:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	virtual void OnOK() { return; }
	//ȡ����Ϣ
	virtual void OnCancel() { return; }

	//���ܺ���
public:
	//���û���
	void ResetScore();
	//��ʾ�ɼ�
	void ShowScore();
	//���û���
	void SetGameScore(WORD wChairID, LPCTSTR pszUserName, LONG lGameScore, LONG lRevenue);

	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//������ť
	afx_msg void OnContinue();
	//�뿪��ť
	afx_msg void OnQuitGame();
	//ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//�滭����
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
