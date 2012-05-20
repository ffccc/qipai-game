#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//��������
enum enMoveType
{
	enMoveType_Error,			//�������
	enMoveType_Normal,			//��ͨ����
	enMoveType_SoldierUp,		//��������
	enMoveType_KingRook,		//������λ
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼���
class CGameLogic
{
	//���ױ���
public:
	tagChessItem *					m_pItemLately[2];					//�������
	CArrayTemplate<tagChessManual>	m_ChessManualInfo;					//������Ϣ

	//���̱���
public:
	WORD							m_wStepCount[2];					//���Ӳ���
	tagChessItem					m_ChessItem[2][16];					//������Ϣ
	tagChessItem *					m_ChessBorad[8][8];					//������Ϣ

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//��Ϣ����
public:
	//��ȡ����
	WORD GetStepCount(BYTE cbColor);
	//��ȡ����
	const tagChessManual * GetLastChessManual();
	//��ȡ����
	const tagChessItem * GetChessItem(BYTE cbXPos, BYTE cbYPos);

	//������
public:
	//��������
	void ResetChessBorad();
	//��������
	void UpdateChessBorad();
	//��������
	void InsertChessManual(tagChessManual ChessManual[], WORD wManualCount);

	//���ƺ���
public:
	//�������
	bool RegretChess(WORD wStepCount);
	//�ƶ�����
	enMoveType MoveChess(BYTE cbXSourcePos, BYTE cbYSourcePos, BYTE cbXTargetPos, BYTE cbYTargetPos, BYTE cbSwitchChess);

	//�߼�����
public:
	//�����ж�
	bool IsGameFinish(BYTE cbColor);
	//ɱ���ж�
	bool IsKingDanger(BYTE cbColor);
	//Σ���ж�
	bool IsChessDanger(BYTE cbXTargetPos, BYTE cbYTargetPos);
	//Ч�鲽��
	bool IsWalkLegality(BYTE cbXSourcePos, BYTE cbYSourcePos, BYTE cbXTargetPos, BYTE cbYTargetPos);

	//�ڲ�����
private:
	//��ȡ����
	inline BYTE GetColorIndex(BYTE cbChessColor);
	//��ȡƫ��
	inline BYTE GetExcursion(BYTE cbSourcePos, BYTE cbTargetPos);
	//��������
	inline bool ObstructAnalyse(BYTE cbXSourcePos, BYTE cbYSourcePos, BYTE cbXTargetPos, BYTE cbYTargetPos);
};

//////////////////////////////////////////////////////////////////////////
