#ifndef GAME_FRAME_VIEW_GDI_HEAD_FILE
#define GAME_FRAME_VIEW_GDI_HEAD_FILE

#pragma once

#include "GameFrameView.h"
#include "GameFrameHead.h"

//////////////////////////////////////////////////////////////////////////////////

//��ͼ���
class GAME_FRAME_CLASS CGameFrameViewGDI : public CGameFrameView
{
	//�������
private:
	CSize							m_SizeBackSurface;					//�����С
	CRect							m_RectDirtySurface;					//�ػ�����
	CImage							m_ImageBackSurface;					//�������

	//��������
public:
	//���캯��
	CGameFrameViewGDI();
	//��������
	virtual ~CGameFrameViewGDI();

	//���غ���
private:
	//�滭����
	virtual VOID DrawGameView(CDC * pDC, INT nWidth, INT nHeight)=NULL;

	//���غ���
public:
	//���ý���
	virtual VOID ResetGameView();
	//�����ؼ�
	virtual VOID RectifyControl(INT nWidth, INT nHeight);
	//�������
	virtual VOID InvalidGameView(INT nXPos, INT nYPos, INT nWidth, INT nHeight);

	//��ϷԪ��
public:
	//�滭׼��
	VOID DrawUserReady(CDC * pDC, INT nXPos, INT nYPos);
	//�滭ʱ��
	VOID DrawUserClock(CDC * pDC, INT nXPos, INT nYPos, WORD wUserClock);
	//�滭ͷ��
	VOID DrawUserAvatar(CDC * pDC, INT nXPos, INT nYPos, IClientUserItem * pIClientUserItem);

	//��ͼ����
public:
	//�滭����
	VOID DrawViewImage(CDC * pDC, CBitImage & BitImage, BYTE cbDrawMode);
	//�滭�ַ�
	VOID DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, CRect rcRect);
	//�滭�ַ�
	VOID DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, INT nXPos, INT nYPos);

	//��Ϣӳ��
protected:
	//�滭����
	VOID OnPaint();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif