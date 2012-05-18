#ifndef CHESS_BORAD_HEAD_FILE
#define CHESS_BORAD_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��������
#define INVALID_POS					0xFF								//��Ч����

//��Ϣ����
#define WM_HIT_CHESS_BORAD			(WM_USER+500)						//�������

//////////////////////////////////////////////////////////////////////////

//���̽���
class CChessBorad
{
	//״̬����
protected:
	BYTE							m_cbUserColor;						//�û���ɫ
	bool							m_bManualView;						//��ʷ��ͼ
	bool							m_bPositively;						//��Ӧ��־
	bool							m_bWaitGameRule;					//�ȴ�����

	//��������
protected:
	BYTE							m_cbXPosStart;						//λ������
	BYTE							m_cbYPosStart;						//λ������
	BYTE							m_cbXPosSelect;						//λ������
	BYTE							m_cbYPosSelect;						//λ������
	BYTE							m_cbXPosTerminal;					//λ������
	BYTE							m_cbYPosTerminal;					//λ������

	//���ӱ���
protected:
	tagChessItem *					m_ChessItemPtr[COUNT_X][COUNT_Y];	//������Ϣ

	//λ�ñ���
protected:
	INT								m_XExcursion;						//ƫ�Ʊ���
	INT								m_YExcursion;						//ƫ�Ʊ���
	INT								m_XBoradSpace;						//�������
	INT								m_YBoradSpace;						//�������

	//λ�ñ���
protected:
	CSize							m_SizeChess;						//���Ӵ�С
	CSize							m_SizeBorad;						//���̴�С
	CPoint							m_BenchmarkPos;						//��׼λ��

	//��Դ����
protected:
	CPngImage						m_ImageChessItem;					//������Դ
	CSkinImage						m_ImageChessBorad;					//������Դ
	CSkinImage						m_ImageChessFrame;					//���ӿ��
	CSkinImage						m_ImageWaitGameRule;				//�ȴ���Դ

	//��������
public:
	//���캯��
	CChessBorad();
	//��������
	virtual ~CChessBorad();

	//���ܺ���
public:
	//�滭����
	VOID DrawChessBorad(CDC * pDC);
	//����ģʽ
	VOID SetChessBoradMode(bool bSmallMode);

	//���ӿ���
public:
	//�������
	VOID CleanChessItem();
	//��������
	VOID SetChessBorad(tagChessItem * ChessItemPtr[COUNT_X][COUNT_Y]);

	//ѡ������
public:
	//ѡ������
	VOID SelectChessItem(BYTE cbXPos, BYTE cbYPos);
	//����·��
	VOID SetMoveChessTrace(BYTE cbXPosStart, BYTE cbYPosStart, BYTE cbXPosTerminal, BYTE cbYPosTerminal);

	//״̬����
public:
	//������ɫ
	VOID SetChessColor(BYTE cbUserColor) { m_cbUserColor=cbUserColor; }
	//������Ӧ
	VOID SetPositively(bool bPositively) { m_bPositively=bPositively; }
	//������ͼ
	VOID SetManualView(bool bManualView) { m_bManualView=bManualView; }
	//�ȴ�����
	VOID SetWaitGameRule(bool bWaitGameRule) { m_bWaitGameRule=bWaitGameRule; }

	//λ�ú���
public:
	//��ȡ��С
	VOID GetControlSize(CSize & ControlSize) { ControlSize=m_SizeBorad; }
	//��ȡλ��
	VOID GetBenchmarkPos(CPoint & BenchmarkPos) { BenchmarkPos=m_BenchmarkPos; }
	//��׼λ��
	VOID SetBenchmarkPos(INT nXPos, INT nYPos) { m_BenchmarkPos.SetPoint(nXPos,nYPos); }

	//�¼�����
public:
	//�����
	VOID OnEventLMouseDown(CPoint Point);

	//��������
private:
	//��ȡ����
	INT GetChessXPos(BYTE cbXPos);
	//��ȡ����
	INT GetChessYPos(BYTE cbYPos);
	//ת������
	VOID SwitchMousePos(INT nXCursor, INT nYCursor, BYTE & cbXIndex, BYTE & cbYIndex);
};

//////////////////////////////////////////////////////////////////////////

#endif
