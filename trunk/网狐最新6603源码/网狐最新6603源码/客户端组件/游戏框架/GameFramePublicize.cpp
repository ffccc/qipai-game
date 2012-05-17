#include "StdAfx.h"
#include "Resource.h"
#include "GameFramePublicize.h"

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameFramePublicize::CGameFramePublicize()
{
	//���ñ���
	m_crBoradColor=CLR_INVALID;

	return;
}

//��������
CGameFramePublicize::~CGameFramePublicize()
{
}

//λ�ú���
VOID CGameFramePublicize::GetWebBrowserRect(CRect & rcWebBrowser)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//����λ��
	if (m_crBoradColor!=CLR_INVALID)
	{
		rcWebBrowser.top=2;
		rcWebBrowser.left=2;
		rcWebBrowser.right=rcClient.Width()-2;
		rcWebBrowser.bottom=rcClient.Height()-2;
	}
	else
	{
		rcWebBrowser.top=0;
		rcWebBrowser.left=0;
		rcWebBrowser.right=rcClient.Width();
		rcWebBrowser.bottom=rcClient.Height();
	}

	return;
}

//�滭����
VOID CGameFramePublicize::DrawPublicizeView(CDC * pDC, INT nWidth, INT nHeight, BYTE cbStatus)
{
	//�滭����
	pDC->FillSolidRect(0,0,nWidth,nHeight,RGB(255,255,255));

	//������Դ
	CPngImage ImagePublicize;
	ImagePublicize.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("LOAD_STATUS"));

	//�滭����
	CSize SizePublicize;
	SizePublicize.SetSize(ImagePublicize.GetWidth(),ImagePublicize.GetHeight());
	ImagePublicize.DrawImage(pDC,(nWidth-SizePublicize.cx)/2,(nHeight-SizePublicize.cy)/2,SizePublicize.cx,SizePublicize.cy,0,0);

	//���߿�
	if (m_crBoradColor!=CLR_INVALID) pDC->Draw3dRect(0,0,nWidth,nHeight,m_crBoradColor,m_crBoradColor);

	return;
}

//���ñ߿�
VOID CGameFramePublicize::SetBoradColor(COLORREF crBoradColor)
{
	//���ñ���
	m_crBoradColor=crBoradColor;

	//���½���
	if (m_hWnd!=NULL) Invalidate(FALSE);

	return;
}

//////////////////////////////////////////////////////////////////////////////////
