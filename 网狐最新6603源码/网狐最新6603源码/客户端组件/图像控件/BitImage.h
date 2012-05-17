#ifndef BIT_IMAGE_HEAD_FILE
#define BIT_IMAGE_HEAD_FILE

#pragma once

#include "WHImageHead.h"

//////////////////////////////////////////////////////////////////////////////////
//结构定义

//渲染信息
struct tagImageRender
{
	INT								nXImage;							//图片位置
	INT								nYImage;							//图片位置
	INT								nXScreen;							//渲染位置
	INT								nYScreen;							//渲染位置
	INT								cxRender;							//渲染尺寸
	INT								cyRender;							//渲染尺寸
};

//////////////////////////////////////////////////////////////////////////////////

//位图对象
class WH_IMAGE_CLASS CBitImage : public CImage
{
	//函数定义
public:
	//构造函数
	CBitImage();
	//析构函数
	virtual ~CBitImage();

	//重载函数
public:
	//位图指针
	operator CBitmap * () { return CBitmap::FromHandle((HBITMAP)(*this)); }

	//区域函数
public:
	//创建区域
	bool CreateImageRegion(CRgn & ImageRgn, COLORREF crTransColor);
	//创建区域
	bool CreateImageRegion(CRgn & ImageRgn, INT nXSrcPos, INT nYSrcPos, INT nSrcWidth, INT nSrcHeight, COLORREF crTransColor);

	//绘画图片
public:
	//平铺绘画
	bool DrawImageTile(CDC * pDC, CRect & rcDestRect);
	//平铺绘画
	bool DrawImageTile(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight);

	//透明绘画
public:
	//透明绘画
	bool TransDrawImage(CDC * pDestDC, INT xDest, INT yDest, COLORREF crTransColor);
	//透明绘画
	bool TransDrawImage(CDC * pDestDC, INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, COLORREF crTransColor);
	//透明绘画
	bool TransDrawImage(CDC * pDestDC, INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, INT cxSrc, INT cySrc, COLORREF crTransColor);

	//透明绘画
public:
	//混合绘画
	bool AlphaDrawImage(CDC * pDestDC, INT xDest, INT yDest, BYTE cbAlphaDepth);
	//混合绘画
	bool AlphaDrawImage(CDC * pDestDC, INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, BYTE cbAlphaDepth);
	//混合绘画
	bool AlphaDrawImage(CDC * pDestDC, INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, INT cxSrc, INT cySrc, BYTE cbAlphaDepth);

	//混合绘画
public:
	//混合绘画
	bool BlendDrawImage(CDC * pDestDC, INT xDest, INT yDest, COLORREF crTransColor, BYTE cbAlphaDepth);
	//混合绘画
	bool BlendDrawImage(CDC * pDestDC, INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, COLORREF crTransColor, BYTE cbAlphaDepth);
	//混合绘画
	bool BlendDrawImage(CDC * pDestDC, INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, INT cxSrc, INT cySrc, COLORREF crTransColor, BYTE cbAlphaDepth);

	//辅助函数
public:
	//矩形检查
	bool IsRectIntersect(const CRect & rcRect, INT nXPos, INT nYPos, INT nWidth, INT nHeight);
	//区域计算
	bool GetDrawImageArea(INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, CRect & rcDirty, tagImageRender & ImageRender);
};

//////////////////////////////////////////////////////////////////////////////////

#endif