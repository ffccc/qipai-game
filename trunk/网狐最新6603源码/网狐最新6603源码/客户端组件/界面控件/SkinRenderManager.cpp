#include "StdAfx.h"
#include "SkinRenderManager.h"

//////////////////////////////////////////////////////////////////////////////////
//��̬����

CSkinRenderManager * CSkinRenderManager::m_pSkinRenderManager=NULL;

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CSkinRenderManager::CSkinRenderManager()
{
	//���ñ���
	m_SkinRenderInfo.wColorH=DEF_COLOR_H;
	m_SkinRenderInfo.dRectifyS=DEF_COLOR_S;
	m_SkinRenderInfo.dRectifyB=DEF_COLOR_B;

	//���ñ���
	ASSERT(m_pSkinRenderManager==NULL);
	if (m_pSkinRenderManager==NULL) m_pSkinRenderManager=this;

	return;
}

//��������
CSkinRenderManager::~CSkinRenderManager()
{
	//���ñ���
	ASSERT(m_pSkinRenderManager==this);
	if (m_pSkinRenderManager==this) m_pSkinRenderManager=NULL;

	return;
}

//��Ⱦ����
VOID CSkinRenderManager::RenderImage(CDC * pDC)
{
	//��ԴЧ��
	ASSERT((pDC!=NULL)&&(pDC->m_hDC!=NULL));
	if ((pDC==NULL)||(pDC->m_hDC==NULL)) return;

	//��Ⱦ�ж�
	if ((m_SkinRenderInfo.wColorH==DEF_COLOR_H)&&(m_SkinRenderInfo.dRectifyS==DEF_COLOR_S)&&(m_SkinRenderInfo.dRectifyB==DEF_COLOR_B))
	{
		return;
	}

	//��������
	BITMAP Bitmap;
	ZeroMemory(&Bitmap,sizeof(Bitmap));

	//��ȡͼ��
	CBitmap * pBitmap=pDC->GetCurrentBitmap();
	if (pBitmap!=NULL) pBitmap->GetBitmap(&Bitmap);

	//��ȡ��С
	CSize SizeImage;
	SizeImage.SetSize(Bitmap.bmWidth,Bitmap.bmHeight);

	//Ч���С
	ASSERT((SizeImage.cx>0)&&(SizeImage.cy>0));
	if ((SizeImage.cx==0)||(SizeImage.cy==0)) return;

	return;
}

//��Դת��
VOID CSkinRenderManager::RenderImage(CImage & ImageSource)
{
	//��ԴЧ��
	ASSERT(ImageSource.IsNull()==false);
	if (ImageSource.IsNull()==true) return;

	//��Ⱦ�ж�
	if ((m_SkinRenderInfo.wColorH==DEF_COLOR_H)&&(m_SkinRenderInfo.dRectifyS==DEF_COLOR_S)&&(m_SkinRenderInfo.dRectifyB==DEF_COLOR_B))
	{
		return;
	}

	//������Դ
	if (ImageSource.IsIndexed()==true)
	{
		//��ȡ����
		RGBQUAD ImageColorTable[256];
		INT nIndexCount=ImageSource.GetMaxColorTableEntries();
		ImageSource.GetColorTable(0,nIndexCount,ImageColorTable);

		//ת������
		for (INT nIndex=0;nIndex<nIndexCount;nIndex++)
		{
			//��ȡ��ɫ
			BYTE cbColorR=ImageColorTable[nIndex].rgbRed;
			BYTE cbColorB=ImageColorTable[nIndex].rgbBlue;
			BYTE cbColorG=ImageColorTable[nIndex].rgbGreen;

			//��ȡ����
			COLORHSB SourceHSB=RGBToHSB(RGB(cbColorR,cbColorG,cbColorB));
			COLORRGB ResultRGB=CompoundColor(m_SkinRenderInfo.wColorH,SourceHSB.S,SourceHSB.B);

			//���ý��
			ImageColorTable[nIndex].rgbRed=ResultRGB.R;
			ImageColorTable[nIndex].rgbBlue=ResultRGB.B;
			ImageColorTable[nIndex].rgbGreen=ResultRGB.G;
		}

		//��������
		ImageSource.SetColorTable(0,nIndexCount,ImageColorTable);
	}
	else
	{
		//��ȡ����
		INT nBPPSource=ImageSource.GetBPP()/8L;
		INT nPitchSource=ImageSource.GetPitch();
		LPBYTE cbBitSource=(LPBYTE)ImageSource.GetBits();

		//��Դ��С
		CSize SizeImage;
		SizeImage.SetSize(ImageSource.GetWidth(),ImageSource.GetHeight());

		//��Ⱦ��Դ
		for (INT nXPos=0;nXPos<SizeImage.cx;nXPos++)
		{
			for (INT nYPos=0;nYPos<SizeImage.cy;nYPos++)
			{
				//λͼ����
				switch (nBPPSource)
				{
				case 2:		//��λģʽ
					{
						//��ȡ��ɫ
						WORD * pcrSource=(WORD *)(cbBitSource+nYPos*nPitchSource+nXPos*nBPPSource);

						//������ɫ
						BYTE cbColorB=((*pcrSource)&0x001F)*256/31;
						BYTE cbColorG=(((*pcrSource)&0x03E0)>>5)*256/31;
						BYTE cbColorR=(((*pcrSource)&0x7C00)>>10)*256/31;

						//��ȡ����
						COLORHSB SourceHSB=RGBToHSB(RGB(cbColorR,cbColorG,cbColorB));
						COLORRGB ResultRGB=CompoundColor(m_SkinRenderInfo.wColorH,SourceHSB.S,SourceHSB.B);

						//���ý��
						*pcrSource=(ResultRGB.R<<10)|(ResultRGB.G<<5)|ResultRGB.B;

						break;
					}
				case 3:		//��λģʽ
				case 4:		//��λģʽ
					{
						//��ȡ��ɫ
						COLORREF * pcrSource=(COLORREF *)(cbBitSource+nYPos*nPitchSource+nXPos*nBPPSource);

						//��ȡ����
						COLORHSB SourceHSB=RGBToHSB(*pcrSource);
						COLORRGB ResultRGB=CompoundColor(m_SkinRenderInfo.wColorH,SourceHSB.S,SourceHSB.B);

						//���ý��
						((BYTE *)pcrSource)[0]=ResultRGB.B;
						((BYTE *)pcrSource)[1]=ResultRGB.G;
						((BYTE *)pcrSource)[2]=ResultRGB.R;

						break;
					}
				default:	//��Ч��ʽ
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

//��Ⱦ��ɫ
COLORREF CSkinRenderManager::RenderColor(DOUBLE S, DOUBLE B)
{
	//������ɫ
	COLORRGB ResultRGB=CompoundColor(m_SkinRenderInfo.wColorH,S,B);

	return RGB(ResultRGB.R,ResultRGB.G,ResultRGB.B);
}

//��ɫת��
COLORHSB CSkinRenderManager::RGBToHSB(COLORREF crColor)
{
	//��������
	COLORHSB ColorHSB;
	ZeroMemory(&ColorHSB,sizeof(ColorHSB));

	//�����ɫ
	DOUBLE ColorR=((DOUBLE)GetRValue(crColor)/255.0);
	DOUBLE ColorG=((DOUBLE)GetGValue(crColor)/255.0);
	DOUBLE ColorB=((DOUBLE)GetBValue(crColor)/255.0);

	//��ȡ����
	DOUBLE MaxColor=__max(ColorR,__max(ColorG,ColorB));
	DOUBLE MinColor=__min(ColorR,__min(ColorG,ColorB));

	//���ñ���
	ColorHSB.B=MaxColor;
    ColorHSB.S=(MaxColor==0)?0.0:(1.0-(MinColor/MaxColor));

	//��ɫɫ��
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

//��ɫת��
COLORRGB CSkinRenderManager::HSBToRGB(WORD H, DOUBLE S, DOUBLE B)
{
	//��������
    DOUBLE ColorR=0;
    DOUBLE ColorG=0;
    DOUBLE ColorB=0;

	//��������
	if (S>1.0) S=1.0;
	if (S<0.0) S=0.0;
	if (B>1.0) B=1.0;
	if (B<0.0) B=0.0;
	if (H>360) H=360;

	//��������
	COLORRGB ColorRGB;
	ZeroMemory(&ColorRGB,sizeof(ColorRGB));

	//����ת��
    if (S!=0.0)
    {
		//��������
        DOUBLE SectorPos=(DOUBLE)(H)/60.0;
        INT SectorNumber=(INT)((SectorPos));
        DOUBLE FractionalSector=SectorPos-SectorNumber;

        //ת������
        DOUBLE P=B*(1.0-S);
        DOUBLE Q=B*(1.0-(S*FractionalSector));
        DOUBLE T=B*(1.0-(S*(1-FractionalSector)));

		//��ɫת��
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

	//���ý��
	ColorRGB.R=(BYTE)(ColorR*255);
	ColorRGB.G=(BYTE)(ColorG*255);
	ColorRGB.B=(BYTE)(ColorB*255);

	return ColorRGB;
}

//�ϳ���ɫ
COLORRGB CSkinRenderManager::CompoundColor(WORD H, DOUBLE S, DOUBLE B)
{
	//��������
	WORD ResultH=H;
	DOUBLE ResultS=S;
	DOUBLE ResultB=B;

	//��������
	if (m_SkinRenderInfo.dRectifyS<DEF_COLOR_S)
	{
		ResultS=S+S*m_SkinRenderInfo.dRectifyS;
	}

	//��������
	if (m_SkinRenderInfo.dRectifyS>DEF_COLOR_S)
	{
		ResultS=S+(1.0-S)*m_SkinRenderInfo.dRectifyS;
	}

	//��������
	if (m_SkinRenderInfo.dRectifyB<DEF_COLOR_B)
	{
		ResultB=B+B*m_SkinRenderInfo.dRectifyB;
	}

	//��������
	if (m_SkinRenderInfo.dRectifyB>DEF_COLOR_B)
	{
		ResultB=B+(1.0-B)*m_SkinRenderInfo.dRectifyB;
	}

	return HSBToRGB(ResultH,ResultS,ResultB);
}

//////////////////////////////////////////////////////////////////////////////////
