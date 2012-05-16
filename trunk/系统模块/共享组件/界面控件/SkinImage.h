#ifndef SKIN_IMAGE_HEAD_FILE
#define SKIN_IMAGE_HEAD_FILE

#pragma once

#include "SkinControls.h"

//////////////////////////////////////////////////////////////////////////

//GDI ��Դ�ӿ�
interface IGDIResource
{
	//��������
	virtual int LockResource()=NULL;
	//��������
	virtual int UnLockResource()=NULL;
	//��ȡ���ü���
	virtual int GetReferenceCount()=NULL;
};

//��Դ����ӿ�
interface IGDIResourceHandle
{
	//�Ƿ��
	virtual bool IsAttached()=NULL;
	//�Ƿ���Ч
	virtual bool IsResourceValid()=NULL;
	//�����
	virtual bool DetchResource()=NULL;
	//����Դ
	virtual bool AttachResource(IGDIResource * pGDIObject)=NULL;
};

//////////////////////////////////////////////////////////////////////////

//λͼ������
class SKIN_CONTROL_CLASS CSkinImage : public CImage, public IGDIResource
{
	friend class CImageHandle;

	//�ڲ�����
private:
	int									m_nRefCount;						//���ü���
	bool								m_bLoadAlways;						//�Ƿ���
	tagImageLoadInfo					m_LoadInfo;							//���ز���

	//��������
public:
	//���캯��
	CSkinImage(void);
	//��������
	virtual ~CSkinImage(void);

	//�ӿں���
protected:
	//��������
	virtual int LockResource();
	//��������
	virtual int UnLockResource();
	//��ȡ���ü���
	virtual int GetReferenceCount() { return m_nRefCount; }

	//���ܺ���
public:
	//�Ƿ����ü�����Ϣ
	bool IsSetLoadInfo();
	//������ز���
	bool RemoveLoadInfo();
	//��ȡ���ز���
	bool GetLoadInfo(tagImageLoadInfo & LoadInfo);
	//���ò���
	bool SetLoadInfo(const tagImageLoadInfo & LoadInfo);
	//���ü��ز���
	bool SetLoadInfo(LPCTSTR pszFileName, bool bLoadAlways=false);
	//���ü��ز���
	bool SetLoadInfo(UINT uResourceID, HINSTANCE hResourceDLL=NULL, bool bLoadAlways=false);

	//���غ���
public:
	//λͼָ��
	operator CBitmap * () { return CBitmap::FromHandle((HBITMAP)(*this)); }

	//������
public:
	//��������
	bool CreateImageRegion(CRgn & ImageRgn, COLORREF crTransColor);
	//��������
	bool CreateImageRegion(CRgn & ImageRgn, INT nXSrcPos, INT nYSrcPos, INT nSrcWidth, INT nSrcHeight, COLORREF crTransColor);

	//��ͼ����
public:
	//͸���滭
	bool AlphaDrawImage(CDC * pDestDC, INT xDest, INT yDest, COLORREF crTransColor);
	//͸���滭
	bool AlphaDrawImage(CDC * pDestDC, INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, COLORREF crTransColor);

	//��ϻ滭
public:
	//��ϻ滭
	bool BlendDrawImage(CDC * pDestDC, INT xDest, INT yDest, COLORREF crTransColor, BYTE cbAlphaDepth);
	//��ϻ滭
	bool BlendDrawImage(CDC * pDestDC, INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, COLORREF crTransColor, BYTE cbAlphaDepth);

	//��������
private:
	//�������
	bool GetDrawImageArea(INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, CRect & rcDirty, tagImageRender & ImageRender);
};

//////////////////////////////////////////////////////////////////////////

//λͼ��Դ���
class SKIN_CONTROL_CLASS CImageHandle : public IGDIResourceHandle
{
	//��������
protected:
	CSkinImage							* m_pImage;							//λͼָ��
	
	//��������
public:
	//���캯��
	CImageHandle(CSkinImage * pImageObject=NULL);
	//��������
	virtual ~CImageHandle();
	
	//�ӿں���
public:
	//�Ƿ��
	virtual bool IsAttached();
	//�Ƿ���Ч
	virtual bool IsResourceValid();
	//�����
	virtual bool DetchResource();
	//����Դ
	virtual bool AttachResource(IGDIResource * pImageObject);

	//��������
public:
	//��ȡ���
	HBITMAP GetBitmapHandle();
	//��ȡ����
	CSkinImage * GetImageObject() { return m_pImage; };
	//ָ������
	CSkinImage * operator->() { return GetImageObject(); }
};

//////////////////////////////////////////////////////////////////////////

#endif