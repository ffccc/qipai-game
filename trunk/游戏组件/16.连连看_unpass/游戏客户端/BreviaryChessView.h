#ifndef BRECHESSVIEW_HEAD_FILE
#define BRECHESSVIEW_HEAD_FILE

#pragma once


// BreviaryChessView

class BreviaryChessView
{
public:
	BreviaryChessView();
	virtual ~BreviaryChessView();


	DBall										m_Chess[82];						//��Ϸ���̣���0Ԫ��û��ʹ��
	int											m_score;								//����
	TCHAR										m_scorestring[32];							
	LONG										m_ChessFrameWidth;						//���̱߿���
	int											m_ChessFrame_W;
	int											m_ChessFrame_H;

	BYTE										m_GroupID;								//���ID

	TCHAR										m_class[32];
	TCHAR										m_per[32];

	bool										m_isLose;								//�Ƿ�ʧ��
protected:	

	CGameLogic      								m_Logic;								//��Ϸ�߼���
	
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	void InitChess(void);
};
#endif

