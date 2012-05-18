#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//���̶���
#define COUNT_X						9									//�����Ŀ
#define COUNT_Y						10									//�����Ŀ
#define COUNT_CHESS					16									//������Ŀ				

//������ɫ
#define CHESS_NULL					0									//û������
#define CHESS_BLACK					1									//��ɫ����
#define CHESS_WHITE					2									//��ɫ����

//���Ӷ���
#define CHESS_GENERAL				1									//��
#define CHESS_KAVASS				2									//ʿ
#define CHESS_ELEPHANT				3									//��
#define CHESS_HORSE					4									//��
#define CHESS_ROOK					5									//��
#define CHESS_CANNON				6									//��
#define CHESS_SOLDIER				7									//��

//////////////////////////////////////////////////////////////////////////

//���Ӷ���
struct tagChessItem
{
	BYTE							cbXPos;								//��������
	BYTE							cbYPos;								//��������
	BYTE							cbColor;							//������ɫ
	BYTE							cbChess;							//����Ȩλ
	BYTE							cbChessID;							//���ӱ�ʶ
};

//���׽ṹ
struct tagChessManual
{
	BYTE							cbXSourceChessPos;					//����λ��
	BYTE							cbYSourceChessPos;					//����λ��
	BYTE							cbXTargetChessPos;					//����λ��
	BYTE							cbYTargetChessPos;					//����λ��
};

//׽����
struct tagCatchChessInfo
{
	BYTE							cbSourceCount;						//׽����Ŀ
	tagChessItem *					pTargetChessItem;					//׽��ָ��
	tagChessItem *					pSourceChessItem[18];				//׽��ָ��
};

//�������
struct tagGeneralChessInfo
{
	BYTE							cbSourceCount;						//������Ŀ
	tagChessItem *					pSourceChessItem[18];				//����ָ��
};

//������
struct tagMoveChessResult
{
	bool							bCatchChess;						//׽�ӱ�־
	bool							bMinatoryKing;						//������־
};

//������Ϣ
struct tagConcludeInfo
{
	bool							bGameConclude;						//������־
	BYTE							cbWinnerColor;						//ʤ�����
	BYTE							cbReason;							//��������
};

//////////////////////////////////////////////////////////////////////////

//���׶���
typedef	CArrayTemplate<tagChessManual,tagChessManual &>	CChessManualArray;

//��Ϸ�߼���
class CGameLogic
{
	//���̱���
public:
	tagChessItem					m_ChessItem[2][COUNT_CHESS];		//������Ϣ
	tagChessItem *					m_ChessBorad[COUNT_X][COUNT_Y];		//������Ϣ
	//tagChessItem *					m_KeppChessBorad[COUNT_X][COUNT_Y];	//������Ϣ

	//״̬����
public:
	WORD							m_wNullStep;						//���߲���
	tagCatchChessInfo				m_CatchChessInfo[2];				//׽����Ϣ
	tagGeneralChessInfo				m_GeneralChessInfo[2];				//������Ϣ

	//���ױ���
public:
	CChessManualArray				m_ChessManualArray;					//������Ϣ

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
	tagChessManual * GetLastChessManual();
	//��ȡ����
	tagChessItem * GetChessItem(BYTE cbXPos, BYTE cbYPos);

	//������
public:
	//��������
	VOID ResetChessBorad();
	//��ʷ״̬
	VOID GetHistoryStatus(tagChessItem ChessItem[2][COUNT_CHESS], tagChessItem * ChessBorad[COUNT_X][COUNT_Y], WORD wStepCount);

	//���ƺ���
public:
	//�������
	bool RegretChess(WORD wTargetStep);
	//�ƶ�����
	bool MoveChess(BYTE cbXSourcePos, BYTE cbYSourcePos, BYTE cbXTargetPos, BYTE cbYTargetPos, tagMoveChessResult & MoveChessResult, LPTSTR pszManualName);

	//�߼�����
public:
	//�����ж�
	bool IsGameFinish(BYTE cbMoveColor, tagConcludeInfo & ConcludeInfo);
	//Ч�鲽��
	bool IsWalkLegality(const tagChessItem * pSourceChessItem, BYTE cbXTargetPos, BYTE cbYTargetPos);
	//�жϽ���
	bool IsKillGeneral(BYTE cbXSourcePos, BYTE cbYSourcePos, BYTE cbXTargetPos, BYTE cbYTargetPos);
	//�ж�����
	bool IsLoseChess(BYTE cbColor);

	//�ڲ�����
private:
	//��ȡ����
	inline BYTE GetColorIndex(BYTE cbChessColor);
	//��ȡƫ��
	inline BYTE GetExcursion(BYTE cbSourcePos, BYTE cbTargetPos);

	//�ڲ�����
private:
	//��λ����
	bool InitChessBorad(tagChessItem ChessItem[2][COUNT_CHESS], tagChessItem * ChessBorad[COUNT_X][COUNT_Y]);
	//��������
	bool GetManualItemName(BYTE cbXSourcePos, BYTE cbYSourcePos, BYTE cbXTargetPos, BYTE cbYTargetPos, LPTSTR pszManualName);
	//ִ������
	bool PerformMoveChess(BYTE cbXSourcePos, BYTE cbYSourcePos, BYTE cbXTargetPos, BYTE cbYTargetPos, tagMoveChessResult & MoveChessResult);

	//��������
private:
	//��в����
	tagChessItem * GetMinatoryChess(BYTE cbXTargetPos, BYTE cbYTargetPos);
	//�и��ж�
	tagChessItem * GetProtectedChess(BYTE cbXTargetPos, BYTE cbYTargetPos);
	//��������
	bool ObstructAnalyse(BYTE cbXSourcePos, BYTE cbYSourcePos, BYTE cbXTargetPos, BYTE cbYTargetPos);
};

//////////////////////////////////////////////////////////////////////////

#endif
