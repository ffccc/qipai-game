#include "Stdafx.h"
#include <AfxDLLx.h>
#include "SkinControls.h"

static AFX_EXTENSION_MODULE SkinControlsDLL={NULL,NULL};

//////////////////////////////////////////////////////////////////////////

//DLL ������
extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);
	if (dwReason==DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(SkinControlsDLL, hInstance)) return 0;
		new CDynLinkLibrary(SkinControlsDLL);
	}
	else if (dwReason==DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(SkinControlsDLL);
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////

//͸���滭
extern "C" SKIN_CONTROL_CLASS void AlphaDrawImage(CDC * pDesDC, int nXDes, int nYDes, int nDesWidth, int nDesHeight, 
			   CDC * pScrDC, int nXScr, int nYScr, int nScrWidth, int nScrHeight, COLORREF crTransColor)
{
	//�������
	COLORREF crTextColor=pDesDC->GetTextColor();

	//��������
	CDC ScreenDC,ImageDC,MaskDC;
	CBitmap BmpScreen,BmpImage,BmpMaskBmp;
	BmpMaskBmp.CreateBitmap(nDesWidth,nDesHeight,1,1,NULL);
	BmpImage.CreateCompatibleBitmap(pDesDC,nDesWidth,nDesHeight);
	BmpScreen.CreateCompatibleBitmap(pDesDC,nDesWidth,nDesHeight);
	MaskDC.CreateCompatibleDC(pDesDC);
	ImageDC.CreateCompatibleDC(pDesDC);
	ScreenDC.CreateCompatibleDC(pDesDC);
	CBitmap * pOldBmpImage=ImageDC.SelectObject(&BmpImage);
	CBitmap * pOldMaskBmp=MaskDC.SelectObject(&BmpMaskBmp);
	CBitmap * pOldScreenBmp=ScreenDC.SelectObject(&BmpScreen);

	//�滭ͼ��
	if ((nDesWidth==nScrWidth)&&(nDesHeight==nScrHeight))
	{
		ScreenDC.BitBlt(0,0,nDesWidth,nDesHeight,pDesDC,nXDes,nYDes,SRCCOPY);
		ImageDC.BitBlt(0,0,nDesWidth,nDesHeight,pScrDC,nXScr,nYScr,SRCCOPY);
	}
	else 
	{
		ScreenDC.StretchBlt(0,0,nDesWidth,nDesHeight,pDesDC,nXDes,nYDes,nScrWidth,nScrHeight,SRCCOPY);
		ImageDC.StretchBlt(0,0,nDesWidth,nDesHeight,pScrDC,nXScr,nYScr,nScrWidth,nScrHeight,SRCCOPY);
	}
	ImageDC.SetBkColor(crTransColor);
	MaskDC.BitBlt(0,0,nDesWidth,nDesHeight,&ImageDC,0,0,SRCCOPY);

	//����͸������Ϊ��ɫ��λͼ
	ImageDC.SetBkColor(RGB(0,0,0));
	ImageDC.SetTextColor(RGB(255,255,255));
	ImageDC.BitBlt(0,0,nDesWidth,nDesHeight,&MaskDC,0,0,SRCAND);

	//�������ֱ�ɺ�ɫ
	ScreenDC.SetBkColor(RGB(255,255,255));
	ScreenDC.SetTextColor(RGB(0,0,0));
	ScreenDC.BitBlt(0,0,nDesWidth,nDesHeight,&MaskDC,0,0,SRCAND);
	ScreenDC.BitBlt(0,0,nDesWidth,nDesHeight,&ImageDC,0,0,SRCPAINT);

	//������Ļ
	pDesDC->BitBlt(nXDes,nYDes,nDesWidth,nDesHeight,&ScreenDC,0,0,SRCCOPY);

	//�������
	ImageDC.SelectObject(pOldBmpImage);
	MaskDC.SelectObject(pOldMaskBmp);
	ScreenDC.SelectObject(pOldScreenBmp);
	ImageDC.DeleteDC();
	MaskDC.DeleteDC();
	ScreenDC.DeleteDC();
	BmpImage.DeleteObject();
	BmpScreen.DeleteObject();
	BmpMaskBmp.DeleteObject();

	//�ָ�����
	pDesDC->SetTextColor(crTextColor);

	return;
}

//////////////////////////////////////////////////////////////////////////
