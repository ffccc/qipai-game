#ifndef SCORE_VIEW_HEAD_FILE
#define SCORE_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

#define  LINE_COUNT 130

#define	IDC_BT_OK						455				//��ʼ��Ť

//������ͼ��
class CScoreView : public CDialog
{
	//��������
protected:
	LONG							m_lGameTax;							//��Ϸ˰��
	LONG							m_lGameScore[GAME_PLAYER];			//��Ϸ�÷�
	TCHAR							m_szUserName[GAME_PLAYER][NAME_LEN];//�û�����
    char							*m_szFang;	
	//��Դ����
protected:
	CSkinImage						m_ImageBack;						//����ͼ��
    CSkinImage                      m_ImageHead;
	CSkinImage                      m_ImageBody;                      
	CSkinImage                      m_ImageLine;
	CSkinButton                     m_ButtonOK;
	//��������
public:
	//���캯��
	CScoreView(void);
	//��������
	virtual ~CScoreView(void);
	
	//���غ���
public:
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
	//����˰��
	void SetGameTax(LONG lGameTax);
	//���û���
	void SetGameScore(WORD wChairID, LPCTSTR pszUserName, LONG lScore);
	//���÷���
    void SetFangText( char *pszFang );
protected:
	void SetupRgn(CDC *pDC,CSkinImage bImage,COLORREF TransColor);	//͸��ɫ
	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//�滭����
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    
	afx_msg void OnBtOK();
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif