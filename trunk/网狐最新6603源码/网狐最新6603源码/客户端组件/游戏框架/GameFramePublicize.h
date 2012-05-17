#ifndef GAME_FRAME_PUBLICIZE_HEAD_FILE
#define GAME_FRAME_PUBLICIZE_HEAD_FILE

#pragma once

#include "GameFrameHead.h"

//////////////////////////////////////////////////////////////////////////////////

//��ܹ��
class GAME_FRAME_CLASS CGameFramePublicize : public CWebPublicize
{
	//��������
protected:
	COLORREF						m_crBoradColor;						//�߿���ɫ

	//��������
public:
	//���캯��
	CGameFramePublicize();
	//��������
	virtual ~CGameFramePublicize();

	//���ú���
protected:
	//λ�ú���
	virtual VOID GetWebBrowserRect(CRect & rcWebBrowser);
	//�滭����
	virtual VOID DrawPublicizeView(CDC * pDC, INT nWidth, INT nHeight, BYTE cbStatus);

	//���ܺ���
public:
	//���ñ߿�
	VOID SetBoradColor(COLORREF crBoradColor);
};

//////////////////////////////////////////////////////////////////////////////////

#endif