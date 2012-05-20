#include "StdAfx.h"
#include "ConcealCardView.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CConcealCardView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CConcealCardView::CConcealCardView()
{
	//��������
	m_cbConcealCount=0;
	ZeroMemory(m_cbConcealCard,sizeof(m_cbConcealCard));

	//������Դ
	m_ImageCard.SetLoadInfo(IDB_CARD,AfxGetInstanceHandle());
	m_ImageBack.SetLoadInfo(IDB_CONCEAL_CARD_VIEW,AfxGetInstanceHandle());

	//��ȡ��С
	CImageHandle CardImageHandle(&m_ImageCard);
	m_CardSize.cx=m_ImageCard.GetWidth()/13;
	m_CardSize.cy=m_ImageCard.GetHeight()/5;

	return;
}

//��������
CConcealCardView::~CConcealCardView()
{
}

//������Ϣ
int CConcealCardView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//���ñ���
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	//�ƶ�����
	CImageHandle ImageHandle(&m_ImageBack);
	SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	return 0;
}

//�ػ�����
void CConcealCardView::OnPaint() 
{
	CPaintDC dc(this); 

	//������Դ
	CImageHandle HandleBack(&m_ImageBack);
	CImageHandle HandleCard(&m_ImageCard);

	//�滭����
	m_ImageBack.BitBlt(dc,0,0);

	//�滭�˿�
	if (m_cbConcealCount>0)
	{
		//��ȡλ��
		CRect rcClient;
		GetClientRect(&rcClient);

		//�����С
		CSize ControlSize;
		ControlSize.cy=m_CardSize.cy;
		ControlSize.cx=m_CardSize.cx+(m_cbConcealCount-1)*18;

		//����λ��
		int nXPos=(rcClient.Width()-ControlSize.cx)/2+17;
		int nYPos=(rcClient.Height()-ControlSize.cy)/2;

		//�滭�˿�
		for (BYTE i=0;i<m_cbConcealCount;i++)
		{
			//��������
			int nXImagePos=0,nYImagePos=0;

			//ͼƬλ��
			if (m_cbConcealCard[i]!=0)
			{
				if ((m_cbConcealCard[i]==0x4E)||(m_cbConcealCard[i]==0x4F))
				{
					nYImagePos=((m_cbConcealCard[i]&CARD_MASK_COLOR)>>4)*m_CardSize.cy;
					nXImagePos=((m_cbConcealCard[i]&CARD_MASK_VALUE)%14)*m_CardSize.cx;
				}
				else
				{
					nXImagePos=((m_cbConcealCard[i]&CARD_MASK_VALUE)-1)*m_CardSize.cx;
					nYImagePos=((m_cbConcealCard[i]&CARD_MASK_COLOR)>>4)*m_CardSize.cy;
				}
			}
			else
			{
				nXImagePos=m_CardSize.cx*2;
				nYImagePos=m_CardSize.cy*4;
			}

			//�滭�˿�
			m_ImageCard.AlphaDrawImage(&dc,nXPos+i*18,nYPos,m_CardSize.cx,m_CardSize.cy,nXImagePos,nYImagePos,RGB(255,0,255));
		}
	}

	return;
}

//������Ϣ
void CConcealCardView::SetConcealCard(BYTE cbConcealCard[], BYTE cbConcealCount)
{
	//���ñ���
	m_cbConcealCount=cbConcealCount;
	CopyMemory(m_cbConcealCard,cbConcealCard,cbConcealCount*sizeof(BYTE));

	//ˢ�½���
	if (m_hWnd!=NULL) Invalidate(FALSE);

	return;
}

//////////////////////////////////////////////////////////////////////////
