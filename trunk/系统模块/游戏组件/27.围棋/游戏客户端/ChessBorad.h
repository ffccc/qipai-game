#ifndef CHESS_BORAD_HEAD_FILE
#define CHESS_BORAD_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//λ�ö���
#define EXCURSION_X					18									//ƫ������
#define EXCURSION_Y					17									//ƫ������
#define SPACE_BORAD_X				25									//��϶��С
#define SPACE_BORAD_Y				25									//��϶��С

//ѡ��ģʽ
#define SM_NO_SELECT				0x00								//��ֹѡ��
#define SM_NULL_CHESS				0x01								//ѡ��հ�
#define SM_BLACK_CHESS				0x02								//ѡ�����
#define SM_WHITE_CHESS				0x04								//ѡ�����

//��Ϣ����
#define WM_HIT_CHESS_BORAD			(WM_USER+200)						//�������

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//���Ӷ���
struct tagChessInfo
{
	BYTE							cbFlag;								//���ӱ�־
	BYTE							cbColor;							//������ɫ
	WORD							wIdentifier;						//���ӱ�ʶ
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CChessBorad : public CWnd
{
	//���ñ���
protected:
	bool							m_bShowChessID;						//��ʾ����
	bool							m_bDoubleMouse;						//˫������
	BYTE							m_cbSelectMode;						//ѡ��ģʽ

	//��������
protected:
	BYTE							m_cbXCursor;						//���λ��
	BYTE							m_cbYCursor;						//���λ��

	//��Ŀ����
protected:
	bool							m_bCountInfo;						//��ʾ��Ŀ
	BYTE							m_cbCountInfo[19][19];				//��Ŀ���

	//״̬����
protected:
	BYTE							m_cbXPosFocus;						//��������
	BYTE							m_cbYPosFocus;						//��������
	tagChessInfo					m_ChessInfo[19][19];				//������Ϣ

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
	//�����Ŀ
	void CleanCountInfo();
	//��������
	void CleanChess(WORD wDeadChess[], WORD wDeadChessCount);
	//���õ�Ŀ
	void SetChessCountInfo(BYTE cbCountInfo[19][19]);
	//��������
	void SetChess(BYTE cbXPos, BYTE cbYPos, BYTE cbColor, WORD wIdentifier);
	//��������
	void SetChessBorad(const BYTE cbChessBorad[19][19], const WORD wIdentifier[19][19]);

	//״̬����
public:
	//��ʾ����
	void ShowChessID(bool bShowChessID);
	//˫������
	void SetDoubleMouse(bool bDoubleMouse);
	//ѡ��ģʽ
	void SetSelectMode(BYTE cbSelectMode);
	//���ý���
	void SetChessFocus(BYTE cbXPos, BYTE cbYPos);
	//��ʾʤ��
	void ShowWinChessLine(BYTE cbXFirstPos, BYTE cbYFirstPos, BYTE cbXEndPos, BYTE cbYEndPos);
	//��������
	void SetChessBorad(const WORD wBlackChess[], BYTE cbBlackCount, const WORD wWhiteChess[], BYTE cbWhiteCount);

	//״̬��ѯ
public:
	//�Ƿ��Ŀ
	bool IsChessCount() { return m_bCountInfo; }
	//��ʾ����
	bool IsShowChessID() { return m_bShowChessID; } 
	//˫������
	bool IsDoubleMouse() { return m_bDoubleMouse; }
	//ѡ��ģʽ
	BYTE GetSelectMode() { return m_cbSelectMode; }
	//��ȡ��С
	const CSize & GetChessBoradSize() { return m_BoradSize; }

	//��������
private:
	//��������
	void UpdateBoradPos(BYTE cbXPos, BYTE cbYPos);
	//ѡ���ж�
	bool EfficacySelect(BYTE cbXPos, BYTE cbYPos);
	//ת������
	void SwitchMousePos(const CPoint & MousePoint, BYTE & cbXPos, BYTE & cbYPos);
	//��ȡ����
	inline int GetChessXPos(BYTE cbXPos) { return EXCURSION_X+cbXPos*SPACE_BORAD_X; }
	//��ȡ����
	inline int GetChessYPos(BYTE cbYPos) { return EXCURSION_Y+cbYPos*SPACE_BORAD_Y; }

	//��Ϣӳ��
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	//���˫��
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif