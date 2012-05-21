#ifndef CALL_SCORE_INFO_HEAD_FILE
#define CALL_SCORE_INFO_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"


//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////

//������ͼ��
class CCallScoreInfo : public CWnd
{
	//������Ϣ
protected:
	LONG							m_lScore[GAME_PLAYER][60];			//��Ϸ�з�
	LONG							m_lColor[GAME_PLAYER][60];			//��Ϸ��ɫ
	TCHAR							m_szUserName[GAME_PLAYER][NAME_LEN];//�û�����

	//��Դ����
protected:
	CSkinImage						m_ImageBack;						//����ͼ��
	CSkinImage						m_ImageColorFlag;					//��ɫ��־

	//��������
public:
	//���캯��
	CCallScoreInfo();
	//��������
	virtual ~CCallScoreInfo();
	
	//���ܺ���
public:
	
	//���û���
	void SetCallGameInfo(WORD wChairID, BYTE bColor,BYTE bScore);

	void SetCallUserName(WORD wChairID,LPCTSTR pszUserName);

	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif