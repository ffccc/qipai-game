#ifndef PLATFORM_PUBLICIZE_HEAD_FILE
#define PLATFORM_PUBLICIZE_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////

//��ܹ��
class CPlatformPublicize : public CWebPublicize
{
	//��������
protected:
	COLORREF						m_crBoradColor;						//�߿���ɫ

	//��������
public:
	//���캯��
	CPlatformPublicize();
	//��������
	virtual ~CPlatformPublicize();

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