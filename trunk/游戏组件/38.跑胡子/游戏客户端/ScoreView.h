#ifndef SCORE_VIEW_HEAD_FILE
#define SCORE_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

//������Ϣ
struct tagScoreViewInfo
{
	//ʤ����Ϣ
	BYTE							cbHuCard;							//�����˿�
	WORD							wWinUser;							//ʤ���û�
	WORD							wProvideUser;						//�����û�

	//�ɼ�����
	LONG							lGameTax;							//��Ϸ˰��
	LONG							lGameScore[GAME_PLAYER];			//��Ϸ����
	TCHAR							szUserName[GAME_PLAYER][NAME_LEN];	//�û�����

	//������Ϣ
	tagHuCardInfo					HuCardInfo;							//������Ϣ
};

//////////////////////////////////////////////////////////////////////////

//������ͼ��
class CScoreView : public CWnd
{
	//��������
protected:
	CSkinImage						m_ImageBack;						//����ͼ��
	tagScoreViewInfo				m_ScoreViewInfo;					//������Ϣ

	//��������
public:
	//���캯��
	CScoreView(void);
	//��������
	virtual ~CScoreView(void);
	
	//���ܺ���
public:
	//���û���
	void ResetScore();
	//������Ϣ
	void SetScoreViewInfo(tagScoreViewInfo & ScoreViewInfo, bool bShowWnd);

	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif