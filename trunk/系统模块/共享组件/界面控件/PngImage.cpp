#include "StdAfx.h"
#include "PngImage.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CPngImage::CPngImage()
{
	//���ñ���
	m_pImage=NULL;

	return;
}

//��������
CPngImage::~CPngImage()
{
	//����ͼƬ
	DestroyImage(); 

	return;
}

//�Ƿ����
bool CPngImage::IsNull()
{
	//�����ж�
	if (m_pImage==NULL) return true;
	if (m_pImage->GetLastStatus()!=Ok) return true;

	return false;
}

//��ȡ���
INT CPngImage::GetWidth()
{
	//�����ж�
	ASSERT(IsNull()==false);
	if (IsNull()==true) return 0;

	//��ȡ���
	return m_pImage->GetWidth();
}

//��ȡ�߶�
INT CPngImage::GetHeight()
{
	//�����ж�
	ASSERT(IsNull()==false);
	if (IsNull()==true) return 0;

	//��ȡ�߶�
	return m_pImage->GetHeight();
}

//����ͼƬ
bool CPngImage::DestroyImage()
{
	//ɾ������
	if (m_pImage!=NULL) SafeDelete(m_pImage);

	return true;
}

//����ͼƬ
bool CPngImage::LoadImage(LPCTSTR pszFileName)
{
	//�����ж�
	ASSERT(m_pImage==NULL);
	if (m_pImage!=NULL) return false;

	//�����ļ�
	CT2CW strFileName(pszFileName);
	m_pImage=Image::FromFile((LPCWSTR)strFileName);

	//�����ж�
	if ((m_pImage==NULL)||(m_pImage->GetLastStatus()!=Ok)) 
	{
		DestroyImage();
		return false;
	}

	return true;
}

//����ͼƬ
bool CPngImage::LoadImage(HINSTANCE hInstance, LPCTSTR pszResourceName)
{
	//�����ж�
	ASSERT(m_pImage==NULL);
	if (m_pImage!=NULL) return false;

	//��������
	HRSRC hResource=NULL;
	HGLOBAL hGlobal=NULL;
	IStream * pIStream=NULL;

	//����λͼ
	try
	{
		//������Դ
		hResource=FindResource(hInstance,pszResourceName,TEXT("PNG"));
		if (hResource==NULL) throw 0;

		//������Դ
		hGlobal=LoadResource(hInstance,hResource);
		if (hGlobal==NULL) throw 0;

		//��ȡ��Դ
		LPVOID pImageBuffer=LockResource(hGlobal);
		DWORD dwImageSize=SizeofResource(hInstance,hResource);

		//��������
		CreateStreamOnHGlobal(NULL,TRUE,&pIStream);
		if (pIStream==NULL) throw 0;

		//д������
		pIStream->Write(pImageBuffer,dwImageSize,NULL);

		//����λͼ
		m_pImage=Image::FromStream(pIStream);
		if ((m_pImage==NULL)||(m_pImage->GetLastStatus()!=Ok)) throw 0;

		//�ͷ���Դ
		if (pIStream!=NULL) pIStream->Release();

		//�Ƿ���Դ
		if (hGlobal!=NULL)
		{
			UnlockResource(hGlobal);
			FreeResource(hGlobal);
		}

		return true;
	}
	catch (...)
	{
		//�ͷ���Դ
		if (pIStream!=NULL) pIStream->Release();

		//�Ƿ���Դ
		if (hGlobal!=NULL)
		{
			UnlockResource(hGlobal);
			FreeResource(hGlobal);
		}

		return false;
	}

	return true;
}

//�滭ͼ��
bool CPngImage::DrawImage(CDC * pDC, INT nXPos, INT nYPos)
{
	//�����ж�
	ASSERT(m_pImage!=NULL);
	if (m_pImage==NULL) return false;

	//������Ļ
	ASSERT(pDC!=NULL);
	Graphics graphics(pDC->GetSafeHdc());

	//��ȡ����
	INT nImageWidth=m_pImage->GetWidth();
	INT nImageHeight=m_pImage->GetHeight();

	//����λ��
	RectF rcDrawRect;
	rcDrawRect.X=(REAL)nXPos;
	rcDrawRect.Y=(REAL)nYPos;
	rcDrawRect.Width=(REAL)nImageWidth;
	rcDrawRect.Height=(REAL)nImageHeight;

	//�滭ͼ��
	graphics.DrawImage(m_pImage,rcDrawRect,0,0,(REAL)nImageWidth,(REAL)nImageHeight,UnitPixel);

	return true;
}

//�滭ͼ��
bool CPngImage::DrawImage(CDC * pDC, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXScr, INT YSrc)
{
	//�����ж�
	ASSERT(m_pImage!=NULL);
	if (m_pImage==NULL) return false;

	//������Ļ
	ASSERT(pDC!=NULL);
	Graphics graphics(pDC->GetSafeHdc());

	//����λ��
	RectF rcDrawRect;
	rcDrawRect.X=(REAL)nXDest;
	rcDrawRect.Y=(REAL)nYDest;
	rcDrawRect.Width=(REAL)nDestWidth;
	rcDrawRect.Height=(REAL)nDestHeight;

	//�滭ͼ��
	graphics.DrawImage(m_pImage,rcDrawRect,(REAL)nXScr,(REAL)YSrc,(REAL)nDestWidth,(REAL)nDestHeight,UnitPixel);

	return true;
}

//�滭ͼ��
bool CPngImage::DrawImage(CDC * pDC, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXScr, INT YSrc, INT nSrcWidth, INT nSrcHeight)
{
	//�����ж�
	ASSERT(m_pImage!=NULL);
	if (m_pImage==NULL) return false;

	//������Ļ
	ASSERT(pDC!=NULL);
	Graphics graphics(pDC->GetSafeHdc());

	//����λ��
	RectF rcDrawRect;
	rcDrawRect.X=(REAL)nXDest;
	rcDrawRect.Y=(REAL)nYDest;
	rcDrawRect.Width=(REAL)nDestWidth;
	rcDrawRect.Height=(REAL)nDestHeight;

	//�滭ͼ��
	graphics.DrawImage(m_pImage,rcDrawRect,(REAL)nXScr,(REAL)YSrc,(REAL)nSrcWidth,(REAL)nSrcHeight,UnitPixel);

	return true;
}

//////////////////////////////////////////////////////////////////////////
