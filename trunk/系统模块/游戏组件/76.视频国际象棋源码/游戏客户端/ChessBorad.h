#ifndef CHESS_BORAD_HEAD_FILE
#define CHESS_BORAD_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��Ϣ����
#define WM_HIT_CHESS_BORAD			(WM_USER+200)						//�������

//////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CChessBorad : public CWnd
{
	//���ñ���
protected:
	BYTE							m_cbUserColor;						//�û���ɫ
	bool							m_bPositively;						//��Ӧ��־
	bool							m_bShowChessName;					//��ʾ����

	//��������
protected:
	BYTE							m_cbXPosStart;						//λ������
	BYTE							m_cbYPosStart;						//λ������
	BYTE							m_cbXPosSelect;						//λ������
	BYTE							m_cbYPosSelect;						//λ������
	BYTE							m_cbXPosTerminal;					//λ������
	BYTE							m_cbYPosTerminal;					//λ������

	//״̬����
protected:
	tagChessItem					* m_ChessBorad[8][8];				//������Ϣ

	//��Դ����
protected:
	CSize							m_BoradSize;						//���̴�С
	CSkinImage						m_ImageChess;						//������Դ
	CSkinImage						m_ImageBorad;						//������Դ
	CSkinImage						m_ImageFrame;						//���ӿ��

	//��������
public:
	//���캯��
	CChessBorad();
	//��������
	virtual ~CChessBorad();

	//���ӿ���
public:
	//�������
	void CleanChess();
	//ѡ������
	void SetSelectChess(BYTE cbXPos, BYTE cbYPos);
	//��������
	void SetChessBorad(tagChessItem * ChessBorad[8][8]);

	//״̬����
public:
	//������ɫ
	void SetChessColor(BYTE cbUserColor);
	//������Ӧ
	void SetPositively(bool bPositively);
	//��ʾ����
	void ShowChessName(bool bShowChessName);
	//�������
	void SetChessFrame(BYTE cbXPosStart, BYTE cbYPosStart, BYTE cbXPosTerminal, BYTE cbYPosTerminal);

	//״̬��ѯ
public:
	//��ȡ��С
	const CSize & GetChessBoradSize() { return m_BoradSize; }

	//��������
private:
	//��ȡ����
	int GetChessXPos(BYTE cbXPos);
	//��ȡ����
	int GetChessYPos(BYTE cbYPos);
	//��������
	void UpdateBoradPos(BYTE cbXPos, BYTE cbYPos);
	//ת������
	void SwitchMousePos(const CPoint & MousePoint, BYTE & cbXPos, BYTE & cbYPos);

	//��Ϣӳ��
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