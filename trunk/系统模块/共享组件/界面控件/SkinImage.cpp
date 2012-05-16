#include "StdAfx.h"
#include "SkinImage.h"

/////////////////////////////////////////////////////////////////////////////////////////

//���캯��
CSkinImage::CSkinImage(void)
{
	m_nRefCount=0;
	m_bLoadAlways=false;
}

//��������
CSkinImage::~CSkinImage(void)
{
	Destroy();
}

//��������
int CSkinImage::LockResource()
{
	if (m_nRefCount==0)
	{
		if (m_LoadInfo.hResourceDLL==NULL) Load(m_LoadInfo.strFileName);
		else LoadFromResource(m_LoadInfo.hResourceDLL,m_LoadInfo.uResourceID);
		if (IsNull()) return 0;
	}
	return ++m_nRefCount;
}

//��������
int CSkinImage::UnLockResource()
{
	try
	{
		if (m_nRefCount>0) m_nRefCount--;
		if (m_nRefCount==0) Destroy();
	}
	catch (...) {}
	return m_nRefCount;
}

//�Ƿ����ü�����Ϣ
bool CSkinImage::IsSetLoadInfo()
{
	return ((m_LoadInfo.hResourceDLL)||(m_LoadInfo.strFileName.IsEmpty()==false));
}

//������ز���
bool CSkinImage::RemoveLoadInfo()
{
	m_LoadInfo.uResourceID=0;
	m_LoadInfo.hResourceDLL=NULL;
	m_LoadInfo.strFileName.Empty();
	return true;
}

//��ȡ���ز���
bool CSkinImage::GetLoadInfo(tagImageLoadInfo & LoadInfo)
{
	LoadInfo=m_LoadInfo;
	return IsSetLoadInfo();
}

//���ò���
bool CSkinImage::SetLoadInfo(const tagImageLoadInfo & LoadInfo)
{
	//�������
	int nOldRefCount=m_nRefCount;
	bool bOldLoadAlways=m_bLoadAlways;

	//������Դ
	Destroy();
	m_nRefCount=0;
	m_bLoadAlways=(LoadInfo.hResourceDLL==NULL);

	//���ñ���
	m_LoadInfo.uResourceID=LoadInfo.uResourceID;
	m_LoadInfo.hResourceDLL=LoadInfo.hResourceDLL;
	m_LoadInfo.strFileName=LoadInfo.strFileName;

	//�ָ�����
	if (bOldLoadAlways) nOldRefCount--;
	if (m_bLoadAlways) nOldRefCount++;
	while (nOldRefCount>0)
	{
		nOldRefCount--;
		LockResource();
	};

	return true;
}

//���ü��ز���
bool CSkinImage::SetLoadInfo(LPCTSTR pszFileName, bool bLoadAlways)
{
	//�������
	int nOldRefCount=m_nRefCount;
	bool bOldLoadAlways=m_bLoadAlways;

	//������Դ
	Destroy();
	m_nRefCount=0;
	m_bLoadAlways=bLoadAlways;

	//���ñ���
	m_LoadInfo.uResourceID=0;
	m_LoadInfo.hResourceDLL=NULL;
	m_LoadInfo.strFileName=pszFileName;

	//�ָ�����
	if (bOldLoadAlways) nOldRefCount--;
	if (m_bLoadAlways) nOldRefCount++;
	while (nOldRefCount>0)
	{
		nOldRefCount--;
		LockResource();
	};

	return true;
}

//���ü��ز���
bool CSkinImage::SetLoadInfo(UINT uResourceID, HINSTANCE hResourceDLL, bool bLoadAlways)
{
	//�������
	int nOldRefCount=m_nRefCount;
	bool bOldLoadAlways=m_bLoadAlways;

	//������Դ
	Destroy();
	m_nRefCount=0;
	m_bLoadAlways=bLoadAlways;
	m_LoadInfo.strFileName.Empty();

	//���ñ���
	m_LoadInfo.uResourceID=uResourceID;
	m_LoadInfo.hResourceDLL=hResourceDLL;

	//�ָ�����
	if (bOldLoadAlways) nOldRefCount--;
	if (m_bLoadAlways) nOldRefCount++;
	while (nOldRefCount>0)
	{
		nOldRefCount--;
		LockResource();
	};

	return true;
}

//��������
bool CSkinImage::CreateImageRegion(CRgn & ImageRgn, COLORREF crTransColor)
{
	//��������
	CreateImageRegion(ImageRgn,0,0,GetWidth(),GetHeight(),crTransColor);

	return true;
}

//��������
bool CSkinImage::CreateImageRegion(CRgn & ImageRgn, INT nXSrcPos, INT nYSrcPos, INT nSrcWidth, INT nSrcHeight, COLORREF crTransColor)
{
	//�ж�״̬
	ASSERT(IsNull()==false);
	if (IsNull()==true) return false;

	//��������
	INT nImageWidht=GetWidth();
	INT nImageHeight=GetHeight();
	if ((nXSrcPos+nSrcWidth)>nImageWidht) nSrcWidth=nImageWidht-nXSrcPos;
	if ((nYSrcPos+nSrcHeight)>nImageHeight) nSrcHeight=nImageHeight-nYSrcPos;

	//��������
	CDC MemDC;
	MemDC.CreateCompatibleDC(NULL);
	ImageRgn.CreateRectRgn(0,0,0,0);

	//����λͼ
	CImage ImageConsult;
	ImageConsult.Create(nSrcWidth,nSrcHeight,32);

	//����λͼ
	BITMAP ImageInfo;
	GetObject(ImageConsult,sizeof(ImageInfo),&ImageInfo);
	while ((ImageInfo.bmWidthBytes%4)!=0) ImageInfo.bmWidthBytes++;

	//�滭λͼ
	BitBlt(ImageConsult.GetDC(),0,0,nSrcWidth,nSrcHeight,nXSrcPos,nYSrcPos);
	ImageConsult.ReleaseDC();

	//��ȡ����
	BYTE cbRTranColor=GetRValue(crTransColor);
	BYTE cbGTranColor=GetGValue(crTransColor);
	BYTE cbBTranColor=GetBValue(crTransColor);
	BYTE * pcbImageBitIndex=(BYTE *)ImageInfo.bmBits+(ImageInfo.bmHeight-1)*ImageInfo.bmWidthBytes;

	//��������
	for (INT nYPos=0;nYPos<ImageInfo.bmHeight;nYPos++)
	{
		for (INT nXPos=0;nXPos<ImageInfo.bmWidth;nXPos++)
		{
			//��������
			INT nXStartPos=nXPos;

			//͸���ж�
			do
			{
				COLORREF crImageColor=*(((LONG *)pcbImageBitIndex)+nXPos);
				if ((cbRTranColor==GetRValue(crImageColor))&&(cbGTranColor==GetGValue(crImageColor))&&(cbBTranColor==GetBValue(crImageColor))) break;
			} while ((++nXPos)<ImageInfo.bmWidth);

			//��������
			if ((nXPos-nXStartPos)>0)
			{
				//��������
				CRgn RgnUnite;
				RgnUnite.CreateRectRgn(nXStartPos,nYPos,nXPos,nYPos+1);

				//�ϲ�����
				CombineRgn(ImageRgn,RgnUnite,ImageRgn,RGN_OR);
			}
		}

		//����λ��
		pcbImageBitIndex-=ImageInfo.bmWidthBytes;
	}

	return true;
}

//͸���滭
bool CSkinImage::AlphaDrawImage(CDC * pDestDC, INT xDest, INT yDest, COLORREF crTransColor)
{
	//�滭ͼƬ
	AlphaDrawImage(pDestDC,xDest,yDest,GetWidth(),GetHeight(),0,0,crTransColor);

	return true;
}

//͸���滭
bool CSkinImage::AlphaDrawImage(CDC * pDestDC, INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, COLORREF crTransColor)
{
	//��Ч����
	CRect rcDirty;
	pDestDC->GetClipBox(&rcDirty);

	//�滭�ж�
	if (IsNull()==true) return false;

	//λ�õ���
	tagImageRender ImageRender;
	GetDrawImageArea(xDest,yDest,cxDest,cyDest,xSrc,ySrc,rcDirty,ImageRender);

	//��ɫ����
	COLORREF crTextColor=pDestDC->SetTextColor(RGB(0,0,0));
	COLORREF crBackColor=pDestDC->SetBkColor(RGB(255,255,255));

	//�������
	CDC MaskDC;
	CBitmap MaskImage;
	MaskDC.CreateCompatibleDC(pDestDC);
	MaskImage.CreateBitmap(ImageRender.cxRender,ImageRender.cyRender,1,1,NULL);

	//λͼ DC
	CDC * pDCImage=CDC::FromHandle(GetDC());
	COLORREF crImageBkColor=pDCImage->SetBkColor(crTransColor);

	//��������
	MaskDC.SelectObject(&MaskImage);
	MaskDC.BitBlt(0,0,ImageRender.cxRender,ImageRender.cyRender,pDCImage,ImageRender.nXImage,ImageRender.nYImage,SRCINVERT);

	//�滭����
	pDestDC->BitBlt(ImageRender.nXScreen,ImageRender.nYScreen,ImageRender.cxRender,ImageRender.cyRender,
		pDCImage,ImageRender.nXImage,ImageRender.nYImage,SRCINVERT);
	pDestDC->BitBlt(ImageRender.nXScreen,ImageRender.nYScreen,ImageRender.cxRender,ImageRender.cyRender,
		&MaskDC,0,0,SRCAND);
	pDestDC->BitBlt(ImageRender.nXScreen,ImageRender.nYScreen,ImageRender.cxRender,ImageRender.cyRender,
		pDCImage,ImageRender.nXImage,ImageRender.nYImage,SRCINVERT);

	//��ɫ��ԭ
	pDestDC->SetBkColor(crBackColor);
	pDestDC->SetTextColor(crTextColor);

	//�ͷ���Դ
	ReleaseDC();
	MaskDC.DeleteDC();
	MaskImage.DeleteObject();

	return true;
}

//��ϻ滭
bool CSkinImage::BlendDrawImage(CDC * pDestDC, INT xDest, INT yDest, COLORREF crTransColor, BYTE cbAlphaDepth)
{
	//͸���滭
	BlendDrawImage(pDestDC,xDest,yDest,GetWidth(),GetHeight(),0,0,crTransColor,cbAlphaDepth);

	return true;
}

//��ϻ滭
bool CSkinImage::BlendDrawImage(CDC * pDestDC, INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, COLORREF crTransColor, BYTE cbAlphaDepth)
{
	//��Ч����
	CRect rcDirty;
	pDestDC->GetClipBox(&rcDirty);

	//�滭�ж�
	if (IsNull()==true) return false;

	//λ�õ���
	tagImageRender ImageRender;
	GetDrawImageArea(xDest,yDest,cxDest,cyDest,xSrc,ySrc,rcDirty,ImageRender);

	//����λͼ
	CImage ImageResult;
	CImage ImageSource;
	ImageResult.Create(ImageRender.cxRender,ImageRender.cyRender,32);
	ImageSource.Create(ImageRender.cxRender,ImageRender.cyRender,32);

	//�滭λͼ
	CDC * pDCImage=CDC::FromHandle(GetDC());
	CDC * pDCResult=CDC::FromHandle(ImageResult.GetDC());
	CDC * pDCSource=CDC::FromHandle(ImageSource.GetDC());
	pDCSource->BitBlt(0,0,ImageRender.cxRender,ImageRender.cyRender,pDCImage,ImageRender.nXImage,ImageRender.nYImage,SRCCOPY);
	pDCResult->BitBlt(0,0,ImageRender.cxRender,ImageRender.cyRender,pDestDC,ImageRender.nXScreen,ImageRender.nYScreen,SRCCOPY);

	//��ȡ����
	float fAlpha=(float)(cbAlphaDepth/255.0);
	INT nPitchResult=ImageResult.GetPitch();
	INT nPitchSource=ImageSource.GetPitch();

	//��ȡ����
	LPBYTE cbBitResult=(LPBYTE)ImageResult.GetBits();
	LPBYTE cbBitSource=(LPBYTE)ImageSource.GetBits();

	//��������
	for (INT nYPos=0;nYPos<ImageRender.cyRender;nYPos++)
	{
		for (INT nXPos=0;nXPos<ImageRender.cxRender;nXPos++)
		{
			//��ȡ��ɫ
			COLORREF * pcrResult=(COLORREF *)(cbBitResult+nYPos*nPitchResult+nXPos*4);
			COLORREF * pcrSource=(COLORREF *)(cbBitSource+nYPos*nPitchSource+nXPos*4);

			//��ϴ���
			if (*pcrSource!=crTransColor)
			{
				//�����ɫ
				BYTE cbResultR=GetRValue(*pcrResult);
				BYTE cbResultG=GetGValue(*pcrResult);
				BYTE cbResultB=GetBValue(*pcrResult);

				//ԭͼ��ɫ
				BYTE cbSourceR=GetRValue(*pcrSource);
				BYTE cbSourceG=GetGValue(*pcrSource);
				BYTE cbSourceB=GetBValue(*pcrSource);

				//��ɫ���
				cbResultR=(BYTE)(cbSourceR*fAlpha+cbResultR*(1.0-fAlpha));
				cbResultG=(BYTE)(cbSourceG*fAlpha+cbResultG*(1.0-fAlpha));
				cbResultB=(BYTE)(cbSourceB*fAlpha+cbResultB*(1.0-fAlpha));

				//��ɫ���
				*pcrResult=RGB(cbResultR,cbResultG,cbResultB);
			}
		}
	}

	//�滭����
	ImageResult.BitBlt(pDestDC->m_hDC,ImageRender.nXScreen,ImageRender.nYScreen);

	//�ͷŶ���
	ReleaseDC();
	ImageSource.ReleaseDC();
	ImageResult.ReleaseDC();

	return true;
}

//�������
bool CSkinImage::GetDrawImageArea(INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, CRect & rcDirty, tagImageRender & ImageRender)
{
	//�ཻ����
	CRect rcIntersect;
	rcIntersect.top=__max(yDest,rcDirty.top);
	rcIntersect.left=__max(xDest,rcDirty.left);
	rcIntersect.right=__min((xDest+cxDest),rcDirty.right);
	rcIntersect.bottom=__min((yDest+cyDest),rcDirty.bottom);

	//��Ⱦλ��
	ImageRender.nYScreen=rcIntersect.top;
	ImageRender.nXScreen=rcIntersect.left;

	//��Ⱦ��С
	ImageRender.cxRender=rcIntersect.Width();
	ImageRender.cyRender=rcIntersect.Height();

	//ͼ������
	ImageRender.nXImage=xSrc+ImageRender.nXScreen-xDest;
	ImageRender.nYImage=ySrc+ImageRender.nYScreen-yDest;

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////

//���캯��
CImageHandle::CImageHandle(CSkinImage * pImageObject)
{
	m_pImage=NULL;
	AttachResource(pImageObject);
	return;
}

//��������
CImageHandle::~CImageHandle()
{
	DetchResource();
	return;
}

//����Դ
bool CImageHandle::AttachResource(IGDIResource * pImageObject)
{
	DetchResource();
	if (pImageObject)
	{
		CSkinImage * pSkinImage=(CSkinImage *)pImageObject;
		if (pImageObject->LockResource()>0)
		{
			m_pImage=pSkinImage;
			return true;
		}
	}
	return false;
}

//�����
bool CImageHandle::DetchResource()
{
	if (m_pImage) 
	{
		m_pImage->UnLockResource();
		m_pImage=NULL;
		return true;
	} 
	return false;
}

//�Ƿ��
bool CImageHandle::IsAttached()
{
	return (m_pImage!=NULL);
}

//�Ƿ���Ч
bool CImageHandle::IsResourceValid()
{
	return ((m_pImage)&&(m_pImage->IsNull()==false));
}

//��ȡ���
HBITMAP CImageHandle::GetBitmapHandle()
{
	if (IsResourceValid()==false) return NULL;
	return (HBITMAP)(*m_pImage);
}

/////////////////////////////////////////////////////////////////////////////////////////
