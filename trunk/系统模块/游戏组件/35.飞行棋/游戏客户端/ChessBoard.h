#ifndef CHESS_BOARD_HEAD_FILE
#define CHESS_BOARD_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "DxGraphics.h"

//////////////////////////////////////////////////////////////////////////
//�궨��
#define		CHESSMOVE_NOT_ACTIVE		0
#define		CHESSMOVE_NOT_FINISH		1
#define		CHESSMOVE_HAS_FINISHED		2


struct ChessMoveData{
	SHORT shXDirection;
	SHORT shYDirection;
};
// �����ƶ�Ч��
class ChessMove{
public:

	WORD							m_wPlayerID;
	BYTE							m_cbChessID;
	BYTE							m_MoveStatus;				//·������״̬��0δ���1���δ��ɣ�2����ɡ�
	BYTE							m_CurFrame;					//��ǰ֡��
	BYTE							m_cbRefChessMove;			//�����ƶ��Ķ��岻��0xFFʱ���ǹ�����ChessMove����������±꣬��ͬһ�������ж��·����ʱ��ʹ��
	BYTE							m_cbBeRefedCount;			//0��ʾû�й�����1��ʾ��һ��������2��ʾ�������������������ơ�
	CArrayTemplate<ChessMoveRoute>	m_ChessMoveRoute;			//Mapת��


	ChessMove()
	{	
		m_CurFrame = 0;
	}
	~ChessMove()
	{
		m_ChessMoveRoute.RemoveAll();
	}
	
};
//���Ӷ�λ����
struct tagChessID
{
	BYTE							cbColor;							//������ɫ
	BYTE							cbChessID;							//���ӱ�ʶ���ɻ�0��1��2��3��
};

struct BeEatedTile
{
	BYTE							cbChessCount;
	BYTE							cbTileIndex;
	BYTE							cbRefChessMove;						//��Ӧ�Ŀ�ʼ��ChessMove����
	BeEatedTile(BYTE lcbChessCount, BYTE lcbTileIndex, BYTE lcbRefChessMove){
		cbChessCount = lcbChessCount;
		cbTileIndex = lcbTileIndex;
		cbRefChessMove = lcbRefChessMove;
	}
	BeEatedTile(){}
};
//////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CChessBoard 
{
	friend class CD3DWnd;
	friend class CGameClientDlg;
	//���ñ���


	//״̬����
public:

	cTexture		m_ChessBoardTexture;								//����Texture
	cTexture		m_PlaneTexture;										//���ӱ���
	cTexture		m_FinishPlaneTexture;								//�ɹ��غ��ɻ�����
protected:
	CArrayTemplate<tagClientTileItem>		m_ChessBoard;										//������������Ϣ
	tagClientChessItem				m_ClientChessItem[GAME_PLAYER][PLAYER_CHESS_COUNT];	//�ͻ�������
	WORD							m_CurPlayerID;										//��ǰ���
	WORD							m_CurMeChairID;										//��������Ӧ��ҵ���λ��
	BYTE							m_CurChessID;										//��ǰ���ѡ�������
	BYTE							m_bPlayStatus[GAME_PLAYER];				//���״̬
	BYTE							m_bViewSeatID[GAME_PLAYER];				//�߼����ݵ���ͼ���ݵ�ת����
    struct ChessMoveData m_ChessMoveMap[8];
    USHORT			m_usSpeed;			//�����ٶ�
	CPoint			m_BasePosition[GAME_PLAYER];

	CArrayTemplate<ChessMove,ChessMove &>	m_ChessMove;				//�����߶�����
	//CGameLogic								m_GameLogic;						//��Ϸ�߼�
	CArrayTemplate<BYTE>					m_CurActiveChess;					//��ǰ��ҵĻ����
	//��������
public:
	//���캯��
	CChessBoard();
	//��ʼ������
	void InitChessBoard(cGraphics *pGraphics);
	//��ʼ������
	void InitChess();
	//��������
	virtual ~CChessBoard();
	//ת�������Ӻ�׼���ƶ����ӣ���������·���ͳ����жϣ��������Ӻ��·�����㡣
	//void PrepareMoveChess(BYTE randNum);
	//���������ƶ��Ķ�����ʾ���ڶ���������ʱ�����ӵ�����λ�òż��������
	void MakeChessMove(DWORD ElapseTime);
	//�л���ǰ�û�
	void SwitchCurrentUser(WORD cbPlayerID);
	//�л���ǰ����
	void SwitchCurrentChess(BYTE cbChessID);
	//���ý�������Ӧ��ҵ���λ��
	void SetCurMeChairID(BYTE cbChairID);
	//�����ӻ�����Ļ
	void BlitChess();
	//�������ӵ�·��
	BYTE BeKilledRouteGen(BYTE cbTileID, BYTE &cbChessCount, BYTE cbColor);
	//����·��
	BOOL ChessRouteGen( BYTE randNum );
	//����x��y�����������
	void IncrementGen(const tagClientTileItem & origin, const tagClientTileItem & target, float &fXIncrement, float &fYIncrement);
	//�����������
	BYTE DirectionMapNum(const tagClientTileItem & origin, const tagClientTileItem & target);
	//�ͷ�Texture;
	void CleanUpTexture();
	void TestChess(BYTE cbTestFlag);
	BOOL InitBackGround(cGraphics *pGraphics);
	//void TestActiveChess();
	//void TestChessMove();
};

//////////////////////////////////////////////////////////////////////////

#endif