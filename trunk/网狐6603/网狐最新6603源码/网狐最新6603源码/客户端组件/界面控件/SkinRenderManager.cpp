#include "StdAfx.h"
#include "SkinRenderManager.h"

//////////////////////////////////////////////////////////////////////////////////
//静态变量

CSkinRenderManager * CSkinRenderManager::m_pSkinRenderManager=NULL;

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CSkinRenderManager::CSkinRenderManager()
{
	//设置变量
	m_SkinRenderInfo.wColorH=DEF_COLOR_H;
	m_SkinRenderInfo.dRectifyS=DEF_COLOR_S;
	m_SkinRenderInfo.dRectifyB=DEF_COLOR_B;

	//设置变量
	ASSERT(m_pSkinRenderManager==NULL);
	if (m_pSkinRenderManager==NULL) m_pSkinRenderManager=this;

	return;
}

//析构函数
CSkinRenderManager::~CSkinRenderManager()
{
	//设置变量
	ASSERT(m_pSkinRenderManager==this);
	if (m_pSkinRenderManager==this) m_pSkinRenderManager=NULL;

	return;
}

//渲染界面
VOID CSkinRenderManager::RenderImage(CDC * pDC)
{
	//资源效验
	ASSERT((pDC!=NULL)&&(pDC->m_hDC!=NULL));
	if ((pDC==NULL)||(pDC->m_hDC==NULL)) return;

	//渲染判断
	if ((m_SkinRenderInfo.wColorH==DEF_COLOR_H)&&(m_SkinRenderInfo.dRectifyS==DEF_COLOR_S)&&(m_SkinRenderInfo.dRectifyB==DEF_COLOR_B))
	{
		return;
	}

	//变量定义
	BITMAP Bitmap;
	ZeroMemory(&Bitmap,sizeof(Bitmap));

	//获取图像
	CBitmap * pBitmap=pDC->GetCurrentBitmap();
	if (pBitmap!=NULL) pBitmap->GetBitmap(&Bitmap);

	//获取大小
	CSize SizeImage;
	SizeImage.SetSize(Bitmap.bmWidth,Bitmap.bmHeight);

	//效验大小
	ASSERT((SizeImage.cx>0)&&(SizeImage.cy>0));
	if ((SizeImage.cx==0)||(SizeImage.cy==0)) return;

	return;
}

//资源转换
VOID CSkinRenderManager::RenderImage(CImage & ImageSource)
{
	//资源效验
	ASSERT(ImageSource.IsNull()==false);
	if (ImageSource.IsNull()==true) return;

	//渲染判断
	if ((m_SkinRenderInfo.wColorH==DEF_COLOR_H)&&(m_SkinRenderInfo.dRectifyS==DEF_COLOR_S)&&(m_SkinRenderInfo.dRectifyB==DEF_COLOR_B))
	{
		return;
	}

	//索引资源
	if (ImageSource.IsIndexed()==true)
	{
		//获取索引
		RGBQUAD ImageColorTable[256];
		INT nIndexCount=ImageSource.GetMaxColorTableEntries();
		ImageSource.GetColorTable(0,nIndexCount,ImageColorTable);

		//转换索引
		for (INT nIndex=0;nIndex<nIndexCount;nIndex++)
		{
			//获取颜色
			BYTE cbColorR=ImageColorTable[nIndex].rgbRed;
			BYTE cbColorB=ImageColorTable[nIndex].rgbBlue;
			BYTE cbColorG=ImageColorTable[nIndex].rgbGreen;

			//获取参数
			COLORHSB SourceHSB=RGBToHSB(RGB(cbColorR,cbColorG,cbColorB));
			COLORRGB ResultRGB=CompoundColor(m_SkinRenderInfo.wColorH,SourceHSB.S,SourceHSB.B);

			//设置结果
			ImageColorTable[nIndex].rgbRed=ResultRGB.R;
			ImageColorTable[nIndex].rgbBlue=ResultRGB.B;
			ImageColorTable[nIndex].rgbGreen=ResultRGB.G;
		}

		//设置索引
		ImageSource.SetColorTable(0,nIndexCount,ImageColorTable);
	}
	else
	{
		//获取参数
		INT nBPPSource=ImageSource.GetBPP()/8L;
		INT nPitchSource=ImageSource.GetPitch();
		LPBYTE cbBitSource=(LPBYTE)ImageSource.GetBits();

		//资源大小
		CSize SizeImage;
		SizeImage.SetSize(ImageSource.GetWidth(),ImageSource.GetHeight());

		//渲染资源
		for (INT nXPos=0;nXPos<SizeImage.cx;nXPos++)
		{
			for (INT nYPos=0;nYPos<SizeImage.cy;nYPos++)
			{
				//位图处理
				switch (nBPPSource)
				{
				case 2:		//两位模式
					{
						//获取颜色
						WORD * pcrSource=(WORD *)(cbBitSource+nYPos*nPitchSource+nXPos*nBPPSource);

						//分析颜色
						BYTE cbColorB=((*pcrSource)&0x001F)*256/31;
						BYTE cbColorG=(((*pcrSource)&0x03E0)>>5)*256/31;
						BYTE cbColorR=(((*pcrSource)&0x7C00)>>10)*256/31;

						//获取参数
						COLORHSB SourceHSB=RGBToHSB(RGB(cbColorR,cbColorG,cbColorB));
						COLORRGB ResultRGB=CompoundColor(m_SkinRenderInfo.wColorH,SourceHSB.S,SourceHSB.B);

						//设置结果
						*pcrSource=(ResultRGB.R<<10)|(ResultRGB.G<<5)|ResultRGB.B;

						break;
					}
				case 3:		//三位模式
				case 4:		//四位模式
					{
						//获取颜色
						COLORREF * pcrSource=(COLORREF *)(cbBitSource+nYPos*nPitchSource+nXPos*nBPPSource);

						//获取参数
						COLORHSB SourceHSB=RGBToHSB(*pcrSource);
						COLORRGB ResultRGB=CompoundColor(m_SkinRenderInfo.wColorH,SourceHSB.S,SourceHSB.B);

						//设置结果
						((BYTE *)pcrSource)[0]=ResultRGB.B;
						((BYTE *)pcrSource)[1]=ResultRGB.G;
						((BYTE *)pcrSource)[2]=ResultRGB.R;

						break;
					}
				default:	//无效格式
					{
						ASSERT(FALSE);
						return;
					}
				}
			}
		}
	}

	return;
}

//渲染颜色
COLORREF CSkinRenderManager::RenderColor(DOUBLE S, DOUBLE B)
{
	//构造颜色
	COLORRGB ResultRGB=CompoundColor(m_SkinRenderInfo.wColorH,S,B);

	return RGB(ResultRGB.R,ResultRGB.G,ResultRGB.B);
}

//颜色转换
COLORHSB CSkinRenderManager::RGBToHSB(COLORREF crColor)
{
	//变量定义
	COLORHSB ColorHSB;
	ZeroMemory(&ColorHSB,sizeof(ColorHSB));

	//拆分颜色
	DOUBLE ColorR=((DOUBLE)GetRValue(crColor)/255.0);
	DOUBLE ColorG=((DOUBLE)GetGValue(crColor)/255.0);
	DOUBLE ColorB=((DOUBLE)GetBValue(crColor)/255.0);

	//获取参数
	DOUBLE MaxColor=__max(ColorR,__max(ColorG,ColorB));
	DOUBLE MinColor=__min(ColorR,__min(ColorG,ColorB));

	//设置变量
	ColorHSB.B=MaxColor;
    ColorHSB.S=(MaxColor==0)?0.0:(1.0-(MinColor/MaxColor));

	//颜色色相
	if ((MaxColor==ColorR)&&(ColorG>=ColorB))
	{
		ColorHSB.H=(WORD)(60*(ColorG-ColorB)/(MaxColor-MinColor));
	}
	else if ((MaxColor==ColorR)&&(ColorG<ColorB))
	{
		ColorHSB.H=(WORD)(60*(ColorG-ColorB)/(MaxColor-MinColor))+360;
	}
	else if (MaxColor==ColorG)
	{
		ColorHSB.H=(WORD)(60*(ColorB-ColorR)/(MaxColor-MinColor))+120;
	}
	else if (MaxColor==ColorB)
	{
		ColorHSB.H=(WORD)(60*(ColorR-ColorG)/(MaxColor-MinColor))+240;
	}

    return ColorHSB;
}

//颜色转换
COLORRGB CSkinRenderManager::HSBToRGB(WORD H, DOUBLE S, DOUBLE B)
{
	//变量定义
    DOUBLE ColorR=0;
    DOUBLE ColorG=0;
    DOUBLE ColorB=0;

	//参数调整
	if (S>1.0) S=1.0;
	if (S<0.0) S=0.0;
	if (B>1.0) B=1.0;
	if (B<0.0) B=0.0;
	if (H>360) H=360;

	//变量定义
	COLORRGB ColorRGB;
	ZeroMemory(&ColorRGB,sizeof(ColorRGB));

	//饱和转换
    if (S!=0.0)
    {
		//变量定义
        DOUBLE SectorPos=(DOUBLE)(H)/60.0;
        INT SectorNumber=(INT)((SectorPos));
        DOUBLE FractionalSector=SectorPos-SectorNumber;

        //转换变量
        DOUBLE P=B*(1.0-S);
        DOUBLE Q=B*(1.0-(S*FractionalSector));
        DOUBLE T=B*(1.0-(S*(1-FractionalSector)));

		//颜色转换
		switch (SectorNumber)
		{
		case 0:
			{
				ColorR=B;
				ColorG=T;
				ColorB=P;
				break;
			}
		case 1:
			{
				ColorR=Q;
				ColorG=B;
				ColorB=P;
				break;
			}
		case 2:
			{
				ColorR=P;
				ColorG=B;
				ColorB=T;
				break;
			}
		case 3:
			{
				ColorR=P;
				ColorG=Q;
				ColorB=B;
				break;
			}
		case 4:
			{
				ColorR=T;
				ColorG=P;
				ColorB=B;
				break;
			}
		case 5:
			{
				ColorR=B;
				ColorG=P;
				ColorB=Q;
				break;
			}
		case 6:
			{
				ColorR=B;
				ColorG=T;
				ColorB=P;
				break;
			}
		}
	}
	else
	{
        ColorR=B;
		ColorG=B;
		ColorB=B;
    }

	//设置结果
	ColorRGB.R=(BYTE)(ColorR*255);
	ColorRGB.G=(BYTE)(ColorG*255);
	ColorRGB.B=(BYTE)(ColorB*255);

	return ColorRGB;
}

//合成颜色
COLORRGB CSkinRenderManager::CompoundColor(WORD H, DOUBLE S, DOUBLE B)
{
	//变量定义
	WORD ResultH=H;
	DOUBLE ResultS=S;
	DOUBLE ResultB=B;

	//参数调整
	if (m_SkinRenderInfo.dRectifyS<DEF_COLOR_S)
	{
		ResultS=S+S*m_SkinRenderInfo.dRectifyS;
	}

	//参数调整
	if (m_SkinRenderInfo.dRectifyS>DEF_COLOR_S)
	{
		ResultS=S+(1.0-S)*m_SkinRenderInfo.dRectifyS;
	}

	//参数调整
	if (m_SkinRenderInfo.dRectifyB<DEF_COLOR_B)
	{
		ResultB=B+B*m_SkinRenderInfo.dRectifyB;
	}

	//参数调整
	if (m_SkinRenderInfo.dRectifyB>DEF_COLOR_B)
	{
		ResultB=B+(1.0-B)*m_SkinRenderInfo.dRectifyB;
	}

	return HSBToRGB(ResultH,ResultS,ResultB);
}

//////////////////////////////////////////////////////////////////////////////////
