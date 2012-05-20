#include "StdAfx.h"
#include "Resource.h"
#include "JetonControl.h"

//////////////////////////////////////////////////////////////////////////

//�궨��
#define IMAGE_WIDTH					40									//�����
#define IMAGE_HEIGHT				40									//�����

//////////////////////////////////////////////////////////////////////////

//��̬����
bool								CJetonControl::m_bLoad=false;		//��ʼ��־
CSkinImage							CJetonControl::m_ImageJeton;		//������Դ

//////////////////////////////////////////////////////////////////////////

//���캯��
CJetonControl::CJetonControl()
{
	//���ñ���
	m_lJeton=0L;
	m_nImagePos=0;

	//������Դ
	if (m_bLoad==false)
	{
		m_bLoad=true;
		m_ImageJeton.SetLoadInfo(IDB_MONEY,AfxGetInstanceHandle());
	}

	return;
}

//��������
CJetonControl::~CJetonControl()
{
}

//��ȡ����
LONG CJetonControl::GetJeton()
{
	return m_lJeton;
}

//���ó���
void CJetonControl::SetJeton(LONG lJeton)
{
	//���ñ���
	m_lJeton=lJeton;

	return;
}

//��������
void CJetonControl::SetImagePos(int nImagePos)
{
	//���ñ���
	m_nImagePos=nImagePos;

	return;
}

//�滭����
void CJetonControl::DrawJetonControl(CDC * pDC)
{
	//�滭�ж�
	if (m_lJeton==0L) return;

	//�滭����
	CImageHandle HandleJeton(&m_ImageJeton);
	int nXDrawPos=m_BenchmarkPos.x-IMAGE_WIDTH/2;
	int nYDrawPos=m_BenchmarkPos.y-IMAGE_HEIGHT/2;

	LONG lTemp=m_lJeton;
	LONG lTimes[6]={500,100,50,10,5,1};
	bool bl=false;
	BYTE bX=0;
	for (WORD i=0;i<6;i++)
	{
		if(lTemp>=lTimes[i])
		{
			bX++;
			BYTE bTemp;
			if(lTimes[i]==500)bTemp=0;			//$500
			else if(lTimes[i]==100)bTemp=1;		//$100
			else if(lTimes[i]==50)bTemp=2;		//$50
			else if(lTimes[i]==10)bTemp=3;		//$10
			else if(lTimes[i]==5)bTemp=4;		//$5
			else bTemp=5;						//$1
			BYTE bwhile=lTemp/lTimes[i];
			lTemp=lTemp%lTimes[i];
			for(int j=0;j<bwhile;j++)
			{
				if(j%7==0 && j!=0)bX++;
				m_ImageJeton.AlphaDrawImage(pDC,nXDrawPos+bX*45,nYDrawPos-j*2,m_ImageJeton.GetWidth()/6,m_ImageJeton.GetHeight(),m_ImageJeton.GetWidth()/6*bTemp,0,RGB(255,0,255));
			}	
		}
	}

	//�滭��Ŀ
	TCHAR szBuffer[64];
	_snprintf(szBuffer,CountArray(szBuffer),TEXT("%ld"),m_lJeton);

	//����λ��
	CRect rcDrawRect;
	rcDrawRect.left=nXDrawPos+180;
	rcDrawRect.top=nYDrawPos+65;
	rcDrawRect.right=m_BenchmarkPos.x+50;
	rcDrawRect.bottom=m_BenchmarkPos.y+IMAGE_HEIGHT/2+12;

	//��������
	CFont ViewFont;
	ViewFont.CreateFont(-14,0,0,0,700,0,0,0,134,3,2,1,2,TEXT("����"));

	//��������
	CFont * pOldFont=pDC->SelectObject(&ViewFont);

	//�滭��Ŀ
	pDC->SetTextAlign(TA_LEFT);
	pDC->SetTextColor(RGB(255,250,40));
	pDC->DrawText(szBuffer,lstrlen(szBuffer),&rcDrawRect,DT_CENTER|DT_VCENTER|DT_NOCLIP|DT_SINGLELINE);

	//��������
	pDC->SelectObject(pOldFont);

	return;
}

//�滭λ��
bool CJetonControl::SetBenchmarkPos(int nXPos, int nYPos)
{
	//���ñ���
	m_BenchmarkPos.SetPoint(nXPos,nYPos);

	return true;
}

//////////////////////////////////////////////////////////////////////////
