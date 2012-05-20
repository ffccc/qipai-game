#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//���׽ṹ
struct tagChessManual
{
	BYTE							cbXPos;								//����λ��
	BYTE							cbYPos;								//����λ��
	BYTE							cbColor;							//������ɫ
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼���
class CGameLogic
{
	//���ױ���
public:
	BYTE							m_cbStepCount[2];					//������Ŀ
	BYTE							m_cbIdentifier[15][15];				//���ӱ�ʶ
	BYTE							m_cbChessBorad[15][15];				//������Ϣ

	//���ױ���
public:
	CArrayTemplate<tagChessManual>	m_ChessManualInfo;					//������Ϣ

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//��Ϣ����
public:
	//��ȡ����
	BYTE GetStepCount(BYTE cbColor);
	//��ȡ����
	const tagChessManual * GetLastChessManual();

	//���ƺ���
public:
	//��ʼ�߼�
	void InitGameLogic();
	//�������
	bool RegretChess(WORD wStepCount);
	//��������
	BYTE PlaceChess(BYTE cbXPos, BYTE cbYPos, BYTE cbColor);

	//�߼�����
public:
	//ʤ���ж�
	bool WinEstimate(BYTE cbXPos, BYTE cbYPos);
};

//////////////////////////////////////////////////////////////////////////

#endif