#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//�����־
#define AREA_UNKNOW					0									//δ֪����
#define AREA_CHESS					1									//��������
#define AREA_BURROW					2									//��Ѩ����

//��Ŀ��־
#define CT_UNKNOW					0x00								//δ��ͳ��
#define CT_BLACE_CHESS				0x01								//��������
#define CT_WHITE_CHESS				0x02								//��������

//////////////////////////////////////////////////////////////////////////

//������
struct tagAreaResult
{
	WORD							wChessCount;						//������Ŀ
	WORD							wBurrowCount;						//��Ѩ��Ŀ
	WORD							wChessList[361];					//��������
	BYTE							cbAreaResult[19][19];				//������Ϣ
};

//��Ŀ���
struct tagCountResult
{
	WORD							wCommomCount;						//������Ŀ
	WORD							wChessCount[2];						//������Ŀ
	BYTE							cbCountInfo[19][19];				//��Ŀ���
};

//��Ŀ����
struct tagCountAssistant
{
	BYTE							cbCurIndex;							//��ǰ����
	BYTE							cbChessMask;						//��������
	WORD							wChessCount[2];						//������Ŀ
	WORD							wChessList[2][361];					//��������
};

//���ӽṹ
struct tagChessItem
{
	BYTE							cbXPos;								//����λ��
	BYTE							cbYPos;								//����λ��
	WORD							wIdentifier;						//���ӱ�ʶ
};

//���׽ṹ
struct tagChessManual
{
	BYTE							cbXPos;								//����λ��
	BYTE							cbYPos;								//����λ��
	BYTE							cbColor;							//������ɫ
	WORD							wDeadChessCount;					//Χ����Ŀ
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼���
class CGameLogic
{
	//״̬����
protected:
	WORD							m_wStepCount[2];					//�岽��Ŀ
	WORD							m_wTakeChessCount[2];				//������Ŀ
	BYTE							m_cbLastChessBorad[2][19][19];		//��ʷ��Ϣ

	//���̱���
public:
	WORD							m_wIdentifier[19][19];				//���ӱ�ʶ
	BYTE							m_cbChessBorad[19][19];				//������Ϣ

	//���ױ���
public:
	CArrayTemplate<tagChessItem>	m_DeadChessInfo;					//������Ϣ
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
	WORD GetStepCount(BYTE cbColor);
	//������Ŀ
	WORD GetTakeChessCount(BYTE cbColor);
	//��ȡ����
	const tagChessManual * GetLastChessManual();

	//���ƺ���
public:
	//��ʼ�߼�
	void InitGameLogic();
	//�������
	bool RegretChess(WORD wStepCount);
	//��Ŀͳ��
    bool GetChessCountInfo(tagCountResult & CountResult);
	//��������
	WORD PlaceChess(BYTE cbXPos, BYTE cbYPos, BYTE cbColor, WORD wDeadChess[361], WORD & wDeadChessCount);

	//�߼�����
protected:
	//���Ӵ���
	WORD TakeOutChess(BYTE cbXPos, BYTE cbYPos, WORD wDeadChess[361]);
	//��ȡ����
	void GetChessArea(BYTE cbXPos, BYTE cbYPos, tagAreaResult & AreaResult);
	//ʮ������
	void GetCrossArea(BYTE cbXPos, BYTE cbYPos, tagAreaResult & AreaResult);
	//ʮ������
	void GetCrossExpand(BYTE cbXPos, BYTE cbYPos, tagCountResult & CountResult, tagCountAssistant & CountAssistant);
};

//////////////////////////////////////////////////////////////////////////

#endif