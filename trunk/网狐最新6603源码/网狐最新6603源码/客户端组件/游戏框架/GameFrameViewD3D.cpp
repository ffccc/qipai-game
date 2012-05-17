#include "StdAfx.h"
#include "GameFrameAvatar.h"
#include "GameFrameViewD3D.h"
#include ".\gameframeviewd3d.h"

//////////////////////////////////////////////////////////////////////////////////

//��Ⱦ��Ϣ
#define WM_D3D_RENDER				WM_USER+300							//��Ⱦ��Ϣ

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameFrameViewD3D, CGameFrameView)

	//ϵͳ��Ϣ
	ON_WM_SIZE()
	ON_WM_PAINT()

	//�Զ���Ϣ
	ON_MESSAGE(WM_D3D_RENDER, OnMessageD3DRender)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameFrameViewD3D::CGameFrameViewD3D()
{
	//״̬����
	m_bInitD3D=false;
	m_hRenderThread=NULL;

	//֡��ͳ��
	m_dwDrawBenchmark=0;
	m_dwDrawLastCount=0;
	m_dwDrawCurrentCount=0L;

	//��������
	m_RectDirtySurface.SetRect(0,0,0,0);

	return;
}

//��������
CGameFrameViewD3D::~CGameFrameViewD3D()
{
}

//���ý���
VOID CGameFrameViewD3D::ResetGameView()
{
	return;
}

//�����ؼ�
VOID CGameFrameViewD3D::RectifyControl(INT nWidth, INT nHeight)
{
	return;
}

//�������
VOID CGameFrameViewD3D::InvalidGameView(INT nXPos, INT nYPos, INT nWidth, INT nHeight)
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

//�����豸
VOID CGameFrameViewD3D::OnInitDevice(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight)
{
	//���ñ���
	m_bInitD3D=true;

	//��ȡ����
	ASSERT(CSkinResourceManager::GetInstance()!=NULL);
	CSkinResourceManager * pSkinResourceManager=CSkinResourceManager::GetInstance();

	//��ȡ����
	LOGFONT LogFont;
	pSkinResourceManager->GetDefaultFont().GetLogFont(&LogFont);

	//��������
	m_D3DFont.CreateFont(LogFont,0L);

	//�����豸
	m_VirtualEngine.SetD3DDevice(&m_D3DDevice);

	//����ͷ��
	CGameFrameAvatar * pGameFrameAvatar=CGameFrameAvatar::GetInstance();
	if (pGameFrameAvatar!=NULL) pGameFrameAvatar->Initialization(pD3DDevice);

	//������Դ
	HINSTANCE hInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);
	m_D3DTextureReady.LoadImage(pD3DDevice,hInstance,TEXT("USER_READY"),TEXT("PNG"));
	m_D3DTextureMember.LoadImage(pD3DDevice,hInstance,TEXT("MEMBER_FLAG"),TEXT("PNG"));
	m_D3DTextureClockItem.LoadImage(pD3DDevice,hInstance,TEXT("USER_CLOCK_ITEM"),TEXT("PNG"));
	m_D3DTextureClockBack.LoadImage(pD3DDevice,hInstance,TEXT("USER_CLOCK_BACK"),TEXT("PNG"));

	//���ô���
	InitGameView(pD3DDevice,nWidth,nHeight);

	return;
}

//��ʧ�豸
VOID CGameFrameViewD3D::OnLostDevice(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight)
{
	return;
}

//�����豸
VOID CGameFrameViewD3D::OnResetDevice(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight)
{
	return;
}

//��Ⱦ����
VOID CGameFrameViewD3D::OnRenderWindow(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight)
{
	//��������
	CartoonMovie();

	//�滭����
	DrawGameView(pD3DDevice,nWidth,nHeight);

	//������
	m_VirtualEngine.DrawWindows(pD3DDevice);

	//���ñ���
	m_dwDrawCurrentCount++;

	//�ۼ��ж�
	if ((GetTickCount()-m_dwDrawBenchmark)>=1000L)
	{
		//���ñ���
		m_dwDrawLastCount=m_dwDrawCurrentCount;

		//ͳ�ƻ�ԭ
		m_dwDrawCurrentCount=0L;
		m_dwDrawBenchmark=GetTickCount();
	}

	return;
}

//��Ϣ����
BOOL CGameFrameViewD3D::PreTranslateMessage(MSG * pMsg)
{
	//������
	if (m_VirtualEngine.PreTranslateMessage(pMsg->message,pMsg->wParam,pMsg->lParam)==true)
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//��Ϣ����
LRESULT CGameFrameViewD3D::DefWindowProc(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	//������
	if (m_VirtualEngine.DefWindowProc(nMessage,wParam,lParam)==true)
	{
		return 0L;
	}

	return __super::DefWindowProc(nMessage,wParam,lParam);
}

//��Ⱦ�߳�
VOID CGameFrameViewD3D::StartRenderThread()
{
	//Ч��״̬
	ASSERT(m_hRenderThread==NULL);
	if (m_hRenderThread!=NULL) return;

	//�����߳�
	m_hRenderThread=(HANDLE)::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)D3DRenderThread,(LPVOID)(m_hWnd),0L,0L);

	return;
}

//�滭׼��
VOID CGameFrameViewD3D::DrawUserReady(CD3DDevice * pD3DDevice, INT nXPos, INT nYPos)
{
	//��ȡ��С
	CSize SizeUserReady;
	SizeUserReady.SetSize(m_D3DTextureReady.GetWidth(),m_D3DTextureReady.GetHeight());

	//�滭׼��
	m_D3DTextureReady.DrawImage(pD3DDevice,nXPos-SizeUserReady.cx/2,nYPos-SizeUserReady.cy/2);

	return;
}

//�滭��־
VOID CGameFrameViewD3D::DrawOrderFlag(CD3DDevice * pD3DDevice, INT nXPos, INT nYPos, BYTE cbImageIndex)
{
	//��ȡ��С
	CSize SizeMember;
	SizeMember.SetSize(m_D3DTextureMember.GetWidth()/3,m_D3DTextureMember.GetHeight());

	//�滭��־
	m_D3DTextureMember.DrawImage(pD3DDevice,nXPos,nYPos,SizeMember.cx,SizeMember.cy,SizeMember.cx*cbImageIndex,0);

	return;
}

//�滭ʱ��
VOID CGameFrameViewD3D::DrawUserClock(CD3DDevice * pD3DDevice, INT nXPos, INT nYPos, WORD wUserClock)
{
	//�滭ʱ��
	if ((wUserClock>0)&&(wUserClock<100))
	{
		//��ȡ��С
		CSize SizeClockItem;
		CSize SizeClockBack;
		SizeClockBack.SetSize(m_D3DTextureClockBack.GetWidth(),m_D3DTextureClockBack.GetHeight());
		SizeClockItem.SetSize(m_D3DTextureClockItem.GetWidth()/10,m_D3DTextureClockItem.GetHeight());

		//�滭����
		INT nXDrawPos=nXPos-SizeClockBack.cx/2;
		INT nYDrawPos=nYPos-SizeClockBack.cy/2;
		m_D3DTextureClockBack.DrawImage(pD3DDevice,nXDrawPos,nYDrawPos);

		//�滭ʱ��
		WORD nClockItem1=wUserClock/10;
		WORD nClockItem2=wUserClock%10;
		m_D3DTextureClockItem.DrawImage(pD3DDevice,nXDrawPos+13,nYDrawPos+27,SizeClockItem.cx,SizeClockItem.cy,nClockItem1*SizeClockItem.cx,0);
		m_D3DTextureClockItem.DrawImage(pD3DDevice,nXDrawPos+28,nYDrawPos+27,SizeClockItem.cx,SizeClockItem.cy,nClockItem2*SizeClockItem.cx,0);
	}

	return;
}

//�滭ͷ��
VOID CGameFrameViewD3D::DrawUserAvatar(CD3DDevice * pD3DDevice, INT nXPos, INT nYPos, IClientUserItem * pIClientUserItem)
{
	//����ͷ��
	CGameFrameAvatar * pGameFrameAvatar=CGameFrameAvatar::GetInstance();
	if (pGameFrameAvatar!=NULL) pGameFrameAvatar->DrawUserAvatar(pD3DDevice,nXPos,nYPos,pIClientUserItem);

	return;
}

//�滭����
VOID CGameFrameViewD3D::DrawViewImage(CD3DDevice * pD3DDevice, CD3DTexture & D3DTexture, BYTE cbDrawMode)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//�滭λͼ
	switch (cbDrawMode)
	{
	case DRAW_MODE_SPREAD:		//ƽ��ģʽ
		{
			//��ȡ��С
			CSize SizeTexture;
			SizeTexture.SetSize(D3DTexture.GetWidth(),D3DTexture.GetHeight());

			//�滭λͼ
			for (INT nXPos=0;nXPos<rcClient.right;nXPos+=SizeTexture.cx)
			{
				for (INT nYPos=0;nYPos<rcClient.bottom;nYPos+=SizeTexture.cy)
				{
					D3DTexture.DrawImage(pD3DDevice,nXPos,nYPos);
				}
			}

			return;
		}
	case DRAW_MODE_CENTENT:		//����ģʽ
		{
			//��ȡ��С
			CSize SizeTexture;
			SizeTexture.SetSize(D3DTexture.GetWidth(),D3DTexture.GetHeight());

			//λ�ü���
			INT nXPos=(rcClient.Width()-SizeTexture.cx)/2;
			INT nYPos=(rcClient.Height()-SizeTexture.cy)/2;

			//�滭λͼ
			D3DTexture.DrawImage(pD3DDevice,nXPos,nYPos);

			return;
		}
	case DRAW_MODE_ELONGGATE:	//����ģʽ
		{
			//��ȡ��С
			CSize SizeTexture;
			SizeTexture.SetSize(D3DTexture.GetWidth(),D3DTexture.GetHeight());

			//�滭λͼ
			D3DTexture.DrawImage(pD3DDevice,0,0,rcClient.Width(),rcClient.Height(),0,0,SizeTexture.cx,SizeTexture.cy);

			return;
		}
	}

	return;
}

//�滭����
VOID CGameFrameViewD3D::DrawNumberString(CD3DDevice * pD3DDevice, CD3DTexture & D3DTexture, LONG lNumber, INT nXPos, INT nYPos)
{
	//Ч��״̬
	ASSERT(D3DTexture.IsNull()==false);
	if (D3DTexture.IsNull()==true) return;

	//��ȡ��С
	CSize SizeNumber;
	SizeNumber.SetSize(D3DTexture.GetWidth()/10,D3DTexture.GetHeight());

	//��������
	LONG lNumberCount=0;
	LONG lNumberTemp=lNumber;

	//������Ŀ
	do
	{
		lNumberCount++;
		lNumberTemp/=10L;
	} while (lNumberTemp>0);

	//λ�ö���
	INT nYDrawPos=nYPos-SizeNumber.cy/2;
	INT nXDrawPos=nXPos+lNumberCount*SizeNumber.cx/2-SizeNumber.cx;

	//�滭����
	for (LONG i=0;i<lNumberCount;i++)
	{
		//�滭����
		LONG lCellNumber=lNumber%10L;
		D3DTexture.DrawImage(pD3DDevice,nXDrawPos,nYDrawPos,SizeNumber.cx,SizeNumber.cy,lCellNumber*SizeNumber.cx,0);

		//���ñ���
		lNumber/=10;
		nXDrawPos-=SizeNumber.cx;
	};

	return;
}

//�滭����
VOID CGameFrameViewD3D::DrawViewImage(CD3DDevice * pD3DDevice, CD3DSprite & D3DSprite, BYTE cbDrawMode)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//�滭λͼ
	switch (cbDrawMode)
	{
	case DRAW_MODE_SPREAD:		//ƽ��ģʽ
		{
			//��ȡ��С
			CSize SizeTexture;
			SizeTexture.SetSize(D3DSprite.GetWidth(),D3DSprite.GetHeight());

			//�滭λͼ
			for (INT nXPos=0;nXPos<rcClient.right;nXPos+=SizeTexture.cx)
			{
				for (INT nYPos=0;nYPos<rcClient.bottom;nYPos+=SizeTexture.cy)
				{
					D3DSprite.DrawImage(pD3DDevice,nXPos,nYPos);
				}
			}

			return;
		}
	case DRAW_MODE_CENTENT:		//����ģʽ
		{
			//��ȡ��С
			CSize SizeTexture;
			SizeTexture.SetSize(D3DSprite.GetWidth(),D3DSprite.GetHeight());

			//λ�ü���
			INT nXPos=(rcClient.Width()-SizeTexture.cx)/2;
			INT nYPos=(rcClient.Height()-SizeTexture.cy)/2;

			//�滭λͼ
			D3DSprite.DrawImage(pD3DDevice,nXPos,nYPos);

			return;
		}
	case DRAW_MODE_ELONGGATE:	//����ģʽ
		{
			//��ȡ��С
			CSize SizeTexture;
			SizeTexture.SetSize(D3DSprite.GetWidth(),D3DSprite.GetHeight());

			//�滭λͼ
			D3DSprite.DrawImage(pD3DDevice,0,0,rcClient.Width(),rcClient.Height(),0,0,SizeTexture.cx,SizeTexture.cy);

			return;
		}
	}

	return;
}

//�滭����
VOID CGameFrameViewD3D::DrawNumberString(CD3DDevice * pD3DDevice, CD3DSprite & D3DSprite, LONG lNumber, INT nXPos, INT nYPos)
{
	//Ч��״̬
	ASSERT(D3DSprite.IsNull()==false);
	if (D3DSprite.IsNull()==true) return;

	//��ȡ��С
	CSize SizeNumber;
	SizeNumber.SetSize(D3DSprite.GetWidth()/10,D3DSprite.GetHeight());

	//��������
	LONG lNumberCount=0;
	LONG lNumberTemp=lNumber;

	//������Ŀ
	do
	{
		lNumberCount++;
		lNumberTemp/=10L;
	} while (lNumberTemp>0);

	//λ�ö���
	INT nYDrawPos=nYPos-SizeNumber.cy/2;
	INT nXDrawPos=nXPos+lNumberCount*SizeNumber.cx/2-SizeNumber.cx;

	//�滭����
	for (LONG i=0;i<lNumberCount;i++)
	{
		//�滭����
		LONG lCellNumber=lNumber%10L;
		D3DSprite.DrawImage(pD3DDevice,nXDrawPos,nYDrawPos,SizeNumber.cx,SizeNumber.cy,lCellNumber*SizeNumber.cx,0);

		//���ñ���
		lNumber/=10;
		nXDrawPos-=SizeNumber.cx;
	};

	return;
}

//�滭�ַ�
VOID CGameFrameViewD3D::DrawTextString(CD3DDevice * pD3DDevice, LPCTSTR pszString, CRect rcDraw, UINT nFormat, D3DCOLOR crText, D3DCOLOR crFrame)
{
	//��������
	INT nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	INT nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	for (INT i=0;i<CountArray(nXExcursion);i++)
	{
		//����λ��
		CRect rcFrame;
		rcFrame.top=rcDraw.top+nYExcursion[i];
		rcFrame.left=rcDraw.left+nXExcursion[i];
		rcFrame.right=rcDraw.right+nXExcursion[i];
		rcFrame.bottom=rcDraw.bottom+nYExcursion[i];

		//�滭�ַ�
		m_D3DFont.DrawText(pD3DDevice,pszString,&rcFrame,nFormat,crFrame);
	}

	//�滭�ַ�
	m_D3DFont.DrawText(pD3DDevice,pszString,&rcDraw,nFormat,crText);

	return;
}

//�滭�ַ�
VOID CGameFrameViewD3D::DrawTextString(CD3DDevice * pD3DDevice, LPCTSTR pszString, INT nXPos, INT nYPos, UINT nFormat, D3DCOLOR crText, D3DCOLOR crFrame)
{
	//��������
	INT nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	INT nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	for (INT i=0;i<CountArray(nXExcursion);i++)
	{
		m_D3DFont.DrawText(pD3DDevice,pszString,nXPos+nXExcursion[i],nYPos+nYExcursion[i],nFormat,crFrame);
	}

	//�滭�ַ�
	m_D3DFont.DrawText(pD3DDevice,pszString,nXPos,nYPos,nFormat,crText);

	return;
}

//�������
VOID CGameFrameViewD3D::DrawTextString(CD3DDevice * pD3DDevice, LPCTSTR pszString, CRect rcDraw,UINT uFormat, D3DCOLOR D3DColor)
{
	//�������
	m_D3DFont.DrawText(pD3DDevice,pszString,rcDraw,uFormat,D3DColor);

	return;
}

//�������
VOID CGameFrameViewD3D::DrawTextString(CD3DDevice * pD3DDevice, LPCTSTR pszString, INT nXPos, INT nYPos, UINT uFormat, D3DCOLOR D3DColor)
{
	//�������
	m_D3DFont.DrawText(pD3DDevice,pszString,nXPos,nYPos,uFormat,D3DColor);

	return;
}

//�滭����
VOID CGameFrameViewD3D::OnPaint()
{
	CPaintDC dc(this);

	//��Ⱦ�豸
	if (m_bInitD3D==true) m_D3DDevice.RenderD3DDevice();

	return;
}

//λ�ñ仯
VOID CGameFrameViewD3D::OnSize(UINT nType, INT cx, INT cy)
{
	//�����豸
	if (m_bInitD3D==false)
	{
		if ((cx>=0L)&&(cy>0L))
		{
			//���û���
			if (m_D3DDirect.CreateD3DDirect()==false)
			{
				ASSERT(FALSE);
				return;
			}

			//�����豸
			if (m_D3DDevice.CreateD3DDevice(m_hWnd,this)==false)
			{
				ASSERT(FALSE);
				return;
			}
		}
	}
	else
	{
		//�����豸
		m_D3DDevice.ResetD3DDevice();
	}

	__super::OnSize(nType, cx, cy);
}

//��Ⱦ��Ϣ
LRESULT CGameFrameViewD3D::OnMessageD3DRender(WPARAM wParam, LPARAM lParam)
{
	//��Ⱦ�豸
	if (m_bInitD3D==true)
	{
		m_D3DDevice.RenderD3DDevice();
	}

	return 0L;
}

//��Ⱦ�߳�
VOID CGameFrameViewD3D::D3DRenderThread(LPVOID pThreadData)
{
	//Ч�����
	ASSERT(pThreadData!=NULL);
	if (pThreadData==NULL) return;

	//��Ⱦѭ��
	while (TRUE)
	{
		//��Ⱦ�ȴ�
		Sleep(10);

		//������Ϣ
		::SendMessage((HWND)pThreadData,WM_D3D_RENDER,0L,0L);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
