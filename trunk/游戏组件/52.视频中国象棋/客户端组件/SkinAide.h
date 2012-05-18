#ifndef SKIN_AIDE_HEAD_FILE
#define SKIN_AIDE_HEAD_FILE

#pragma once

#include "SkinControlsModule.h"

//////////////////////////////////////////////////////////////////////////

//������Ϣ
struct tagEncircleInfo
{
	INT								nTBorder;							//�ϱ߿��
	INT								nBBorder;							//�±߿��
	INT								nLBorder;							//��߿��
	INT								nRBorder;							//�ұ߿��
};

//������Դ
struct tagEncircleResource
{
	LPCTSTR							pszImageTL;							//��Դ��ʶ
	LPCTSTR							pszImageTM;							//��Դ��ʶ
	LPCTSTR							pszImageTR;							//��Դ��ʶ
	LPCTSTR							pszImageML;							//��Դ��ʶ
	LPCTSTR							pszImageMR;							//��Դ��ʶ
	LPCTSTR							pszImageBL;							//��Դ��ʶ
	LPCTSTR							pszImageBM;							//��Դ��ʶ
	LPCTSTR							pszImageBR;							//��Դ��ʶ
};

//����λͼ
struct tagEncircleImage
{
	CBitImage						ImageTL;							//λͼ����
	CBitImage						ImageTM;							//λͼ����
	CBitImage						ImageTR;							//λͼ����
	CBitImage						ImageML;							//λͼ����
	CBitImage						ImageMR;							//λͼ����
	CBitImage						ImageBL;							//λͼ����
	CBitImage						ImageBM;							//λͼ����
	CBitImage						ImageBR;							//λͼ����
};

//���ƾ��
struct tagEncircleHandle
{
	CImageHandle					ImageHandleTL;						//λͼ���
	CImageHandle					ImageHandleTM;						//λͼ���
	CImageHandle					ImageHandleTR;						//λͼ���
	CImageHandle					ImageHandleML;						//λͼ���
	CImageHandle					ImageHandleMR;						//λͼ���
	CImageHandle					ImageHandleBL;						//λͼ���
	CImageHandle					ImageHandleBM;						//λͼ���
	CImageHandle					ImageHandleBR;						//λͼ���
};

//////////////////////////////////////////////////////////////////////////

//��������
class SKIN_CONTROL_CLASS CSkinAide
{
	//��������
protected:
	//���캯��
	CSkinAide();
	//��������
	virtual ~CSkinAide();

	//���Ƹ���
public:
	//������Ϣ
	static bool GetEncircleInfo(tagEncircleInfo & EncircleInfo, tagEncircleImage & EncircleImage);
	//��Դ��ʶ
	static bool SetEncircleResource(tagEncircleImage & EncircleImage, tagEncircleResource & EncircleResource, HINSTANCE hResInstance);

	//���ƻ滭
public:
	//�滭����
	static bool DrawEncircleFrame(CDC * pDC, CRect & rcEncircle, tagEncircleImage & EncircleImage);
	//�滭����
	static bool DrawEncircleFrame(CDC * pDC, CRect & rcEncircle, tagEncircleHandle & EncircleHandle);
	//�滭����
	static bool DrawEncircleFrame(CDC * pDC, CRect & rcEncircle, tagEncircleImage & EncircleImage, COLORREF crTrans);
	//�滭����
	static bool DrawEncircleFrame(CDC * pDC, CRect & rcEncircle, tagEncircleHandle & EncircleHandle, COLORREF crTrans);

	//���Ƽ���
public:
	//����λ��
	static bool CaleWindowRect(CRect & rcEncircle, tagEncircleInfo & EncircleInfo, CRect & rcControl);
	//����λ��
	static bool CaleWindowRect(CRect & rcEncircle, tagEncircleImage & EncircleImage, CRect & rcControl);
	//����λ��
	static bool CaleWindowRect(INT nXPos, INT nYPos, INT nWidth, INT nHeight, tagEncircleInfo & EncircleInfo, CRect & rcControl);
	//����λ��
	static bool CaleWindowRect(INT nXPos, INT nYPos, INT nWidth, INT nHeight, tagEncircleImage & EncircleImage, CRect & rcControl);

	//���Ƶ���
public:
	//���Ƶ���
	static bool SetWindowPos(CWnd * pWnd, CRect & rcEncircle, tagEncircleInfo & EncircleInfo);
	//���Ƶ���
	static bool SetWindowPos(CWnd * pWnd, CRect & rcEncircle, tagEncircleImage & EncircleImage);
	//���Ƶ���
	static bool SetWindowPos(CWnd * pWnd, INT nXPos, INT nYPos, INT nWidth, INT nHeight, tagEncircleInfo & EncircleInfo);
	//���Ƶ���
	static bool SetWindowPos(CWnd * pWnd, INT nXPos, INT nYPos, INT nWidth, INT nHeight, tagEncircleImage & EncircleImage);

	//���Ƶ���
public:
	//���Ƶ���
	static bool DeferWindowPos(CWnd * pWnd, HDWP hDwp, CRect & rcEncircle, tagEncircleInfo & EncircleInfo);
	//���Ƶ���
	static bool DeferWindowPos(CWnd * pWnd, HDWP hDwp, CRect & rcEncircle, tagEncircleImage & EncircleImage);
	//���Ƶ���
	static bool DeferWindowPos(CWnd * pWnd, HDWP hDwp, INT nXPos, INT nYPos, INT nWidth, INT nHeight, tagEncircleInfo & EncircleInfo);
	//���Ƶ���
	static bool DeferWindowPos(CWnd * pWnd, HDWP hDwp, INT nXPos, INT nYPos, INT nWidth, INT nHeight, tagEncircleImage & EncircleImage);
};

//////////////////////////////////////////////////////////////////////////

#endif
