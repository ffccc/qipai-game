#include "StdAfx.h"
#include "Resource.h"
#include "GoldView.h"

//////////////////////////////////////////////////////////////////////////

//�궨��
#define GOLD_IMAGE_WIDTH		55										//�����
#define GOLD_IMAGE_HEIGHT		56										//�����

//��̬����
bool							CGoldView::m_bInit=false;				//��ʼ��־
CSkinImage						CGoldView::m_ImageGold;					//��������

//////////////////////////////////////////////////////////////////////////

//���캯��
CGoldView::CGoldView()
{
	m_lGold=0L;
	m_lMaxLayer=6;
	memset(m_lGoldCount,0,sizeof(m_lGoldCount));
	if (m_bInit==false)
	{
		m_bInit=true;
		m_ImageGold.SetLoadInfo(IDB_SCORE,GetModuleHandle(NULL));
	}

	return;
}

//��������
CGoldView::~CGoldView()
{
}

//���ó���
void CGoldView::SetGold(LONG lGold)
{
	if (m_lGold!=lGold)
	{
		m_lGold=lGold;
		RectifyGoldLayer();
	}
	return;
}

//���ò���
void CGoldView::SetMaxGoldLayer(LONG lMaxLayer)
{
	if (m_lMaxLayer!=lMaxLayer)
	{
		m_lMaxLayer=lMaxLayer; 
		RectifyGoldLayer();
	}
	return;
}

//�滭����
void CGoldView::DrawGoldView(CDC * pDC, int nXPos, int nYPos, bool bCount)
{
	//����λͼ
	CImageHandle ImageHandle(&m_ImageGold);

	//�滭����
	int nYPosDraw=nYPos-GOLD_IMAGE_HEIGHT/2;
	for (LONG i=0;i<CountArray(m_lGoldCount);i++)
	{
		for (LONG j=0;j<m_lGoldCount[i];j++)
		{
			m_ImageGold.AlphaDrawImage(pDC,nXPos-GOLD_IMAGE_WIDTH/2,nYPosDraw,GOLD_IMAGE_WIDTH,GOLD_IMAGE_HEIGHT,
				i*GOLD_IMAGE_WIDTH,0,RGB(255,0,255));
			nYPosDraw-=4;
		}
	}

	//�滭����
	if ((bCount==true)&&(m_lGold!=0L))
	{
		TCHAR szBuffer[64]=TEXT("��");
		GetGlodString(m_lGold,szBuffer+2);
		CRect DrawRect(nXPos-50,nYPos+GOLD_IMAGE_HEIGHT/2+5,nXPos+46,nYPos+GOLD_IMAGE_HEIGHT/2+17);
		pDC->SetTextColor(RGB(250,250,250));
		DrawTextString(pDC,szBuffer,RGB(255,255,255),RGB(0,0,0),&DrawRect);
		//pDC->DrawText(szBuffer,lstrlen(szBuffer),&DrawRect,DT_CENTER|DT_VCENTER|DT_NOCLIP|DT_SINGLELINE);
	}

	return;
}

//���������
void CGoldView::RectifyGoldLayer()
{
	//��������
	LONG lBasicGold[]={500L,100L,50L,10L,5L,1L},lGold=m_lGold,lGoldLayer=0L;
	memset(m_lGoldCount,0,sizeof(m_lGoldCount));

	//���������
	for (LONG i=0;i<CountArray(lBasicGold);i++)
	{
		if (lGold>=lBasicGold[i])
		{
			m_lGoldCount[i]=lGold/lBasicGold[i];
			lGold-=m_lGoldCount[i]*lBasicGold[i];
			lGoldLayer+=m_lGoldCount[i];
			if (lGoldLayer>=m_lMaxLayer)
			{
				m_lGoldCount[i]-=(lGoldLayer-m_lMaxLayer);
				return;
			}
			if (lGold==0L) break;
		}
	}

	return;
}

//���Ա��ַ�
LPCTSTR CGoldView::GetGlodString(LONG lGold, TCHAR szString[])
{
	//ת���ַ�
	DWORD dwNumBit=0L,dwCharBit=0L;
	do
	{
		dwNumBit++;
		szString[dwCharBit++]=(TCHAR)(lGold%10+TEXT('0'));
		if (dwNumBit%3==0) szString[dwCharBit++]=TEXT(',');
		lGold/=10;
	} while (lGold!=0L);

	//�����ַ�
	if (szString[dwCharBit-1]==TEXT(',')) szString[dwCharBit-1]=0;
	szString[dwCharBit]=0;
	_tcsrev(szString);

	return szString;
}

//��������
void CGoldView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect)
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	pDC->SetTextColor(crFrame);
	CRect rcDraw;
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		rcDraw.CopyRect(lpRect);
		rcDraw.OffsetRect(nXExcursion[i],nYExcursion[i]);
		pDC->DrawText(pszString,nStringLength,&rcDraw,DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	}

	//�滭����
	rcDraw.CopyRect(lpRect);
	pDC->SetTextColor(crText);
	pDC->DrawText(pszString,nStringLength,&rcDraw,DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	return;
}
//////////////////////////////////////////////////////////////////////////
