#ifndef PNG_IMAGE_HEAD_FILE
#define PNG_IMAGE_HEAD_FILE

#pragma once

#include "WHImageHead.h"

//////////////////////////////////////////////////////////////////////////////////

//ͼƬ����
class WH_IMAGE_CLASS CPngImage
{
	//��������
protected:
	Image *							m_pImage;							//ͼƬ����

	//��������
public:
	//���캯��
	CPngImage();
	//��������
	virtual ~CPngImage();

	//״̬����
public:
	//�Ƿ����
	bool IsNull();
	//��ȡ���
	INT GetWidth();
	//��ȡ�߶�
	INT GetHeight();

	//������
public:
	//����ͼƬ
	bool DestroyImage();
	//����ͼƬ
	bool LoadImage(LPCTSTR pszFileName);
	//����ͼƬ
	bool LoadImage(HINSTANCE hInstance, LPCTSTR pszResourceName);

	//�滭����
public:
	//�滭ͼ��
	bool DrawImage(CDC * pDC, INT nXPos, INT nYPos);
	//�滭ͼ��
	bool DrawImage(CDC * pDC, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXScr, INT nYSrc);
	//�滭ͼ��
	bool DrawImage(CDC * pDC, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXScr, INT nYSrc, INT nSrcWidth, INT nSrcHeight);

	//͸���滭
public:
	//��ϻ滭
	bool AlphaDrawImage(CDC * pDestDC, INT xDest, INT yDest, BYTE cbAlphaDepth);
	//��ϻ滭
	bool AlphaDrawImage(CDC * pDestDC, INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, BYTE cbAlphaDepth);
	//��ϻ滭
	bool AlphaDrawImage(CDC * pDestDC, INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, INT cxSrc, INT cySrc, BYTE cbAlphaDepth);

	//��������
public:
	//��ȡ����
	Image * GetImage() { return m_pImage; }
	//ָ������
	Image * operator->() { return m_pImage; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif