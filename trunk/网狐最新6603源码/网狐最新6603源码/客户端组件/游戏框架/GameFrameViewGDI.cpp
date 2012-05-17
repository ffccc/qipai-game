#include "StdAfx.h"
#include "GameFrameAvatar.h"
#include "GameFrameViewGDI.h"

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameFrameViewGDI, CGameFrameView)
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameFrameViewGDI::CGameFrameViewGDI()
{
	//�������
	m_SizeBackSurface.SetSize(0,0);
	m_RectDirtySurface.SetRect(0,0,0,0);

	return;
}

//��������
CGameFrameViewGDI::~CGameFrameViewGDI()
{
}

//���ý���
VOID CGameFrameViewGDI::ResetGameView()
{
	return;
}

//�����ؼ�
VOID CGameFrameViewGDI::RectifyControl(INT nWidth, INT nHeight)
{
	return;
}

//�������
VOID CGameFrameViewGDI::InvalidGameView(INT nXPos, INT nYPos, INT nWidth, INT nHeight)
{
	//����λ��
	CRect rcInvalid;
	rcInvalid.SetRect(nXPos,nYPos,nXPos+nWidth,nYPos+nHeight);

	//λ�õ���
	if (rcInvalid.IsRectNull()==TRUE) GetClientRect(&rcInvalid);

	//���þ���
	if (m_RectDirtySurface.IsRectEmpty()==FALSE)
	{
		//���ñ���
		m_RectDirtySurface.top=__min(m_RectDirtySurface.top,rcInvalid.top);
		m_RectDirtySurface.left=__min(m_RectDirtySurface.left,rcInvalid.left);
		m_RectDirtySurface.right=__max(m_RectDirtySurface.right,rcInvalid.right);
		m_RectDirtySurface.bottom=__max(m_RectDirtySurface.bottom,rcInvalid.bottom);
	}
	else
	{
		//���ñ���
		m_RectDirtySurface=rcInvalid;
	}

	//ˢ�´���
	InvalidateRect(&rcInvalid,FALSE);

	return;
}

//�滭׼��
VOID CGameFrameViewGDI::DrawUserReady(CDC * pDC, INT nXPos, INT nYPos)
{
	//������Դ
	CPngImage ImageUserReady;
	ImageUserReady.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("USER_READY"));

	//��ȡ��С
	CSize SizeUserReady;
	SizeUserReady.SetSize(ImageUserReady.GetWidth(),ImageUserReady.GetHeight());

	//�滭׼��
	ImageUserReady.DrawImage(pDC,nXPos-SizeUserReady.cx/2,nYPos-SizeUserReady.cy/2);

	return;
}

//�滭ʱ��
VOID CGameFrameViewGDI::DrawUserClock(CDC * pDC, INT nXPos, INT nYPos, WORD wUserClock)
{
	//�滭ʱ��
	if ((wUserClock>0)&&(wUserClock<100))
	{
		//������Դ
		CPngImage ImageClockItem;
		CPngImage ImageClockBack;
		ImageClockItem.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("USER_CLOCK_ITEM"));
		ImageClockBack.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("USER_CLOCK_BACK"));

		//��ȡ��С
		CSize SizeClockItem;
		CSize SizeClockBack;
		SizeClockBack.SetSize(ImageClockBack.GetWidth(),ImageClockBack.GetHeight());
		SizeClockItem.SetSize(ImageClockItem.GetWidth()/10,ImageClockItem.GetHeight());

		//�滭����
		INT nXDrawPos=nXPos-SizeClockBack.cx/2;
		INT nYDrawPos=nYPos-SizeClockBack.cy/2;
		ImageClockBack.DrawImage(pDC,nXDrawPos,nYDrawPos);

		//�滭ʱ��
		WORD nClockItem1=wUserClock/10;
		WORD nClockItem2=wUserClock%10;
		ImageClockItem.DrawImage(pDC,nXDrawPos+13,nYDrawPos+27,SizeClockItem.cx,SizeClockItem.cy,nClockItem1*SizeClockItem.cx,0);
		ImageClockItem.DrawImage(pDC,nXDrawPos+28,nYDrawPos+27,SizeClockItem.cx,SizeClockItem.cy,nClockItem2*SizeClockItem.cx,0);
	}

	return;
}

//�滭ͷ��
VOID CGameFrameViewGDI::DrawUserAvatar(CDC * pDC, INT nXPos, INT nYPos, IClientUserItem * pIClientUserItem)
{
	//����ͷ��
	CGameFrameAvatar * pGameFrameAvatar=CGameFrameAvatar::GetInstance();
	if (pGameFrameAvatar!=NULL) pGameFrameAvatar->DrawUserAvatar(pDC,nXPos,nYPos,pIClientUserItem);

	return;
}

//�滭����
VOID CGameFrameViewGDI::DrawViewImage(CDC * pDC, CBitImage & BitImage, BYTE cbDrawMode)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��Ч����
	CRect rcDirty;
	pDC->GetClipBox(&rcDirty);

	//�滭λͼ
	switch (cbDrawMode)
	{
	case DRAW_MODE_SPREAD:		//ƽ��ģʽ
		{
			//������Դ
			INT nImageWidth=BitImage.GetWidth();
			INT nImageHeight=BitImage.GetHeight();

			//�滭λͼ
			for (INT nXPos=0;nXPos<rcClient.right;nXPos+=nImageWidth)
			{
				for (INT nYPos=0;nYPos<rcClient.bottom;nYPos+=nImageHeight)
				{
					bool bIntersect=BitImage.IsRectIntersect(rcDirty,nXPos,nYPos,nImageWidth,nImageHeight);
					if (bIntersect==true) BitImage.BitBlt(pDC->m_hDC,nXPos,nYPos);
				}
			}

			return;
		}
	case DRAW_MODE_CENTENT:		//����ģʽ
		{
			//��������
			INT nImageWidth=BitImage.GetWidth();
			INT nImageHeight=BitImage.GetHeight();

			//λ�ü���
			INT nXPos=(rcClient.Width()-BitImage.GetWidth())/2;
			INT nYPos=(rcClient.Height()-BitImage.GetHeight())/2;

			//�滭λͼ
			bool bIntersect=BitImage.IsRectIntersect(rcDirty,nXPos,nYPos,nImageWidth,nImageHeight);
			if (bIntersect==true) BitImage.BitBlt(pDC->m_hDC,nXPos,nYPos);

			return;
		}
	case DRAW_MODE_ELONGGATE:	//����ģʽ
		{
			//��������
			INT nImageWidth=BitImage.GetWidth();
			INT nImageHeight=BitImage.GetHeight();

			//�滭λͼ
			bool bIntersect=BitImage.IsRectIntersect(rcDirty,0,0,rcClient.Width(),rcClient.Height());
			if (bIntersect==true) BitImage.StretchBlt(pDC->m_hDC,0,0,rcClient.Width(),rcClient.Height(),0,0,nImageWidth,nImageHeight);

			return;
		}
	}

	return;
}

//�滭�ַ�
VOID CGameFrameViewGDI::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, CRect rcRect)
{
	//��������
	INT nStringLength=lstrlen(pszString);
	INT nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	INT nYExcursion[8]={-1,0,1,1,1,0,-1,-1};
	INT nDrawFormat=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_SINGLELINE;

	//�����ʽ
	COLORREF rcTextColor=pDC->SetTextColor(crFrame);
	UINT nTextAlign=pDC->SetTextAlign(TA_TOP|TA_LEFT);

	//�滭�߿�
	for (INT i=0;i<CountArray(nXExcursion);i++)
	{
		//����λ��
		CRect rcFrame;
		rcFrame.top=rcRect.top+nYExcursion[i];
		rcFrame.left=rcRect.left+nXExcursion[i];
		rcFrame.right=rcRect.right+nXExcursion[i];
		rcFrame.bottom=rcRect.bottom+nYExcursion[i];

		//�滭�ַ�
		pDC->DrawText(pszString,nStringLength,&rcFrame,nDrawFormat);
	}

	//�滭�ַ�
	pDC->SetTextColor(crText);
	pDC->DrawText(pszString,nStringLength,&rcRect,nDrawFormat);

	//��ԭ����
	pDC->SetTextAlign(nTextAlign);
	pDC->SetTextColor(rcTextColor);

	return;
}

//��������
VOID CGameFrameViewGDI::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, INT nXPos, INT nYPos)
{
	//��������
	INT nStringLength=lstrlen(pszString);
	INT nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	INT nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//��������
	UINT nTextAlign=pDC->GetTextAlign();
	COLORREF rcTextColor=pDC->GetTextColor();

	//�滭�߿�
	for (INT i=0;i<CountArray(nXExcursion);i++)
	{
		pDC->SetTextColor(crFrame);
		pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
	}

	//�滭����
	pDC->SetTextColor(crText);
	pDC->TextOut(nXPos,nYPos,pszString,nStringLength);

	//��ԭ����
	pDC->SetTextAlign(nTextAlign);
	pDC->SetTextColor(rcTextColor);

	return;
}

//�滭����
VOID CGameFrameViewGDI::OnPaint()
{
	CPaintDC dc(this);

	//��������
	CRect rcDirty;
	dc.GetClipBox(&rcDirty);

	//���ڳߴ�
	CRect rcClient;
	GetClientRect(&rcClient);

	//�����ߴ�
	if ((rcClient.Width()>m_SizeBackSurface.cx)||(rcClient.Height()>m_SizeBackSurface.cy))
	{
		//���ô�С
		m_SizeBackSurface.cx=__max(m_SizeBackSurface.cx,rcClient.Width());
		m_SizeBackSurface.cy=__max(m_SizeBackSurface.cy,rcClient.Height());

		//��������
		m_RectDirtySurface.SetRect(0,0,rcClient.Width(),rcClient.Height());

		//���ٻ���
		if (m_ImageBackSurface.IsNull()==false) m_ImageBackSurface.Destroy();

		//��������
		m_ImageBackSurface.Create(m_SizeBackSurface.cx,m_SizeBackSurface.cy,32);
	}

	//�����ж�
	if (m_RectDirtySurface.IsRectEmpty()==FALSE)
	{
		//��ȡ DC
		HDC hSurfaceDC=m_ImageBackSurface.GetDC();
		CDC * pSurfaceDC=CDC::FromHandle(hSurfaceDC);

		//�������
		m_RectDirtySurface.top=__max(m_RectDirtySurface.top,rcClient.top);
		m_RectDirtySurface.left=__max(m_RectDirtySurface.left,rcClient.left);
		m_RectDirtySurface.right=__min(m_RectDirtySurface.right,rcClient.right);
		m_RectDirtySurface.bottom=__min(m_RectDirtySurface.bottom,rcClient.bottom);

		//��������
		CRgn RgnDirtySurface;
		RgnDirtySurface.CreateRectRgn(m_RectDirtySurface.left,m_RectDirtySurface.top,m_RectDirtySurface.right,m_RectDirtySurface.bottom);

		//���� DC
		pSurfaceDC->SetBkMode(TRANSPARENT);
		pSurfaceDC->SelectClipRgn(&RgnDirtySurface);
		pSurfaceDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

		//�滭����
		m_RectDirtySurface.SetRect(0,0,0,0);
		DrawGameView(pSurfaceDC,rcClient.Width(),rcClient.Height());

		//���½���
		dc.BitBlt(rcDirty.left,rcDirty.top,rcDirty.Width(),rcDirty.Height(),pSurfaceDC,rcDirty.left,rcDirty.top,SRCCOPY);

		//������Դ
		RgnDirtySurface.DeleteObject();
		m_ImageBackSurface.ReleaseDC();
	}
	else
	{
		//���½���
		m_ImageBackSurface.BitBlt(dc,rcDirty.left,rcDirty.top,rcDirty.Width(),rcDirty.Height(),rcDirty.left,rcDirty.top,SRCCOPY);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
