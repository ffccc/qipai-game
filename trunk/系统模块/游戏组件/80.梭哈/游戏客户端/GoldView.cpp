#include "StdAfx.h"
#include "Resource.h"
#include "GoldView.h"

//////////////////////////////////////////////////////////////////////////

//�궨��
#define GOLD_IMAGE_WIDTH		48										//�����
#define GOLD_IMAGE_HEIGHT		41										//�����

//��̬����
bool							CGoldView::m_bInit=false;				//��ʼ��־
CSkinImage						CGoldView::m_ImageGold;					//��������

//////////////////////////////////////////////////////////////////////////

//���캯��
CGoldView::CGoldView()
{
	m_lGold=0L;
	m_nGoldIndex = 1;
	if (m_bInit==false)
	{
		m_bInit=true;
		m_ImageGold.SetLoadInfo(IDB_GOLD,GetModuleHandle(NULL));
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
	}
	return;
}

//�滭����
void CGoldView::DrawGoldView(CDC * pDC, int nXPos, int nYPos)
{
	//����λͼ
	CImageHandle ImageHandle(&m_ImageGold);

	//�滭�ж� 
	if (m_lGold<=0L) return;

	//��ȡ����
	INT nJettonHeight=m_ImageGold.GetHeight();
	INT nJettonWidth=m_ImageGold.GetWidth()/3;

	//�滭����
	INT nXPos1=nXPos-nJettonWidth/2;
	INT nYPos1=nYPos-nJettonHeight/2;
	m_ImageGold.AlphaDrawImage(pDC,nXPos1,nYPos1,nJettonWidth,nJettonHeight,nJettonWidth*m_nGoldIndex,0,RGB(255,0,255));

	//������
	TCHAR szNum[64];
	//_snprintf( szNum,CountArray(szNum),TEXT("%ld"),m_lScore );
	GetGlodString( m_lGold,szNum );
	CRect rcNum( nXPos1,nYPos1,nXPos1+nJettonWidth,nYPos1+nJettonHeight );

	//��������
	CFont ViewFont;
	ViewFont.CreateFont(-12,0,0,0,700,0,0,0,134,3,2,1,1,TEXT("����"));
	COLORREF OldClr = pDC->SetTextColor(RGB(0x0, 0x10, 0x70));
	CFont * pOldFont=pDC->SelectObject(&ViewFont);
	pDC->DrawText( szNum,lstrlen(szNum),&rcNum,DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS );
	pDC->SetTextColor(OldClr);
	//������Դ
	pDC->SelectObject(pOldFont);

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
		//if (dwNumBit%3==0) szString[dwCharBit++]=TEXT(',');
		lGold/=10;
	} while (lGold!=0L);

	//�����ַ�
	if (szString[dwCharBit-1]==TEXT(',')) szString[dwCharBit-1]=0;
	szString[dwCharBit]=0;
	_tcsrev(szString);

	return szString;
}

//
VOID CGoldView::SetGoldIndex( INT nIndex )
{
	m_nGoldIndex = nIndex;
}

//////////////////////////////////////////////////////////////////////////
