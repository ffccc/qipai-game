#ifndef SCORE_CONTROL_HEAD_FILE
#define SCORE_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//������Ϣ
struct tagScoreInfo
{
	TCHAR							szUserName[GAME_PLAYER][NAME_LEN];		//�û�����

	//�û���Ϣ
	WORD							wMeChairId;								//
	BYTE							cbWinOrder;								//��������

	//������Ϣ
	WORD							wProvideUser;							//��Ӧ�û�
	LONG							lGameScore;								//��Ϸ����
	LONG							lGameTax;								//

	DWORD							dwChiHuRight[MAX_RIGHT_COUNT];			//

	LONG							lGangScore;								//��ϸ�÷�
	BYTE							cbGenCount;								//
	WORD							wLostFanShu[GAME_PLAYER];				//
};

//////////////////////////////////////////////////////////////////////////

//���ֿؼ�
class CScoreControl
{
	//��������
protected:
	tagScoreInfo					m_ScoreInfo;							//������Ϣ
	CChiHuRight						m_ChiHuRight;							//
	bool							m_bShow;								//

	//λ�ñ���
protected:
	CPoint							m_ptBenchmark;							//

	//��Դ����
protected:
	CPngImage						m_ImageGameWin;							//������ͼ
	CPngImage						m_ImageGameLost;						//
	CPngImage						m_PngNum;								//

	//��������
public:
	//���캯��
	CScoreControl();
	//��������
	virtual ~CScoreControl();

	//���ܺ���
public:
	//��λ����
	void RestorationData();
	//���û���
	void SetScoreInfo(const tagScoreInfo & ScoreInfo);
	//
	void DrawScoreControl( CDC *pDC );
	//
	void SetBenchmarkPos( int nXPos, int nYPos );

	//��������
protected:
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos, bool bDrawCenter);
	//�滭����
	void DrawNumberString( CDC *pDC, CPngImage &PngNumber, int nXPos, int nYPos, LONG lNumber, bool bDrawCenter);
};

//////////////////////////////////////////////////////////////////////////

#endif
