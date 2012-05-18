#ifndef SKIN_ENCIRCLE_HEAD_FILE
#define SKIN_ENCIRCLE_HEAD_FILE

#pragma once

#include "SkinControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//������Ϣ
struct tagEncircleInfo
{
	INT								nTBorder;							//�ϱ߿��
	INT								nBBorder;							//�±߿��
	INT								nLBorder;							//��߿��
	INT								nRBorder;							//�ұ߿��
};

//����λͼ
struct tagEncirclePNG
{
	CPngImage						ImageTL;							//λͼ����
	CPngImage						ImageTM;							//λͼ����
	CPngImage						ImageTR;							//λͼ����
	CPngImage						ImageML;							//λͼ����
	CPngImage						ImageMR;							//λͼ����
	CPngImage						ImageBL;							//λͼ����
	CPngImage						ImageBM;							//λͼ����
	CPngImage						ImageBR;							//λͼ����
};

//����λͼ
struct tagEncircleBMP
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

//////////////////////////////////////////////////////////////////////////////////

//���ƽ���
class SKIN_CONTROL_CLASS CEncircleBMP
{
	//��Դ��Ϣ
protected:
	HINSTANCE						m_hResInstance;						//��Դ���
	tagEncircleInfo					m_EncircleInfo;						//������Ϣ
	tagEncircleResource				m_EncircleResource;					//������Դ

	//��������
public:
	//���캯��
	CEncircleBMP();
	//��������
	virtual ~CEncircleBMP();

	//���غ���
protected:
	//������Դ
	virtual bool LoadEncircleImage(tagEncircleBMP & EncircleImage);
	//�滭����
	virtual bool PreDrawEncircleImage(tagEncircleBMP & EncircleImage) { return true; }

	//��Դ����
public:
	//Ĭ����Դ
	bool InitDefaultResource();
	//������Դ
	bool InitEncircleResource(tagEncircleResource & EncircleResource, HINSTANCE hResInstance);

	//���ƻ滭
public:
	//�滭����
	bool DrawEncircleFrame(CDC * pDC, CRect & rcEncircle);
	//�滭����
	bool DrawEncircleFrame(CDC * pDC, CRect & rcEncircle, COLORREF crTrans);
	//�滭����
	bool DrawEncircleFrame(CDC * pDC, CRect & rcEncircle, COLORREF crTrans, BYTE cbAlphaDepth);

	//λ�ú���
public:
	//��ȡλ��
	bool GetEncircleInfo(tagEncircleInfo & EncircleInfo);
	//��ȡλ��
	bool GetEncircleRect(CRect & rcFrameBorad, CRect & rcControl);

	//���Ƶ���
public:
	//���Ƶ���
	bool SetWindowPos(CWnd * pWnd, CRect & rcEncircleBorad);
	//���Ƶ���
	bool DeferWindowPos(CWnd * pWnd, HDWP hDwp, CRect & rcEncircleBorad);
};

//////////////////////////////////////////////////////////////////////////////////

//���ƽ���
class SKIN_CONTROL_CLASS CEncirclePNG
{
	//��Դ��Ϣ
protected:
	HINSTANCE						m_hResInstance;						//��Դ���
	tagEncircleInfo					m_EncircleInfo;						//������Ϣ
	tagEncircleResource				m_EncircleResource;					//������Դ

	//��������
public:
	//���캯��
	CEncirclePNG();
	//��������
	virtual ~CEncirclePNG();

	//���غ���
protected:
	//������Դ
	virtual bool LoadEncircleImage(tagEncirclePNG & EncircleImage);
	//�滭����
	virtual bool PreDrawEncircleImage(tagEncirclePNG & EncircleImage) { return true; }

	//��Դ����
public:
	//Ĭ����Դ
	bool InitDefaultResource();
	//������Դ
	bool InitEncircleResource(tagEncircleResource & EncircleResource, HINSTANCE hResInstance);

	//���ƻ滭
public:
	//�滭����
	bool DrawEncircleFrame(CDC * pDC, CRect & rcEncircle);

	//λ�ú���
public:
	//��ȡλ��
	bool GetEncircleInfo(tagEncircleInfo & EncircleInfo);
	//��ȡλ��
	bool GetEncircleRect(CRect & rcFrameBorad, CRect & rcControl);

	//���Ƶ���
public:
	//���Ƶ���
	bool SetWindowPos(CWnd * pWnd, CRect & rcEncircleBorad);
	//���Ƶ���
	bool DeferWindowPos(CWnd * pWnd, HDWP hDwp, CRect & rcEncircleBorad);
};

//////////////////////////////////////////////////////////////////////////////////

#endif