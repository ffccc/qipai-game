#include "StdAfx.h"
#include "Resource.h"
#include "GlobalOption.h"
#include "GameFrameView.h"
#include ".\gameframeview.h"

//////////////////////////////////////////////////////////////////////////

//��̬����
const int			CGameFrameView::m_nXFace=32;						//ͷ��߶�
const int			CGameFrameView::m_nYFace=32;						//ͷ����
const int			CGameFrameView::m_nXTimer=48;						//��ʱ����
const int			CGameFrameView::m_nYTimer=48;						//��ʱ����
const int			CGameFrameView::m_nXBorder=0;						//��ʱ����
const int			CGameFrameView::m_nYBorder=0;						//��ʱ����

//ʱ�䶨��
#define IDI_FLOWER_START			3000								//�ʻ���ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameFrameView, CWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameFrameView::CGameFrameView(bool bDoubleBuf ,UINT uColorCount, CThreadDraw * pThreadDraw) 
	: m_bDoubleBuf(bDoubleBuf), m_uColorCount(uColorCount), m_pThreadDraw(pThreadDraw)
{
	//���ñ���
	m_bReDraw=true;
	m_pReserve=NULL;
	m_nAnimeStep=30;
	m_pIUserFaceRes=NULL;
	memset(&m_wTimer,0,sizeof(m_wTimer));
	memset(&m_ptName,0,sizeof(m_ptName));
	memset(&m_ptFace,0,sizeof(m_ptFace));
	memset(&m_ptTimer,0,sizeof(m_ptTimer));
	memset(&m_ptReady,0,sizeof(m_ptReady));
	memset(&m_pUserItem,0,sizeof(m_pUserItem));
	ZeroMemory(m_FlowerInfo, sizeof(m_FlowerInfo));
	m_nFlowerCount=0;

	return;
}

//��������
CGameFrameView::~CGameFrameView()
{
	//�ͷſؼ�
	for(WORD i=0;i<CountArray(m_FlowerEffectControl);i++)
		if ( m_FlowerEffectControl[i].m_hWnd != NULL )
			m_FlowerEffectControl[i].DestroyWindow();
}

//�ӿڲ�ѯ
void * __cdecl CGameFrameView::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameFrameView,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameFrameView,Guid,dwQueryVer);
	return NULL;
}

//���½���
void CGameFrameView::UpdateGameView(const CRect * pRect)
{
	if (m_bDoubleBuf==true) m_bReDraw=true;
	if (pRect!=NULL) InvalidateRect(pRect,FALSE);
	else Invalidate(FALSE);
	return;
}

//��ȡ����
bool CGameFrameView::GetGameViewImage(CImage & ImageBuffer, int nXPos, int nYPos, int nWidth, int nHeight)
{
	ASSERT(ImageBuffer.IsNull()==false);
	ASSERT(m_ImageBuffer.IsNull()==false);
	if (m_ImageBuffer.IsNull()==false)
	{
		m_ImageBuffer.BitBlt(ImageBuffer.GetDC(),nXPos,nYPos,nWidth,nHeight,nXPos,nYPos);
		ImageBuffer.ReleaseDC();
		return true;
	}
	return false;
}

//����λ��
void CGameFrameView::SetFlowerControlInfo(WORD wChairID,int nXPos, int nYPos, int nWidth, int nHeight, UINT nAnimeStep)
{
	//�Ϸ��ж�
	ASSERT(wChairID<CountArray(m_FlowerEffectControl));
	if (wChairID>=CountArray(m_FlowerEffectControl)) return;

	//���ñ���
	m_nAnimeStep = nAnimeStep;

	//�ƶ�����
	if ( m_FlowerEffectControl[wChairID].m_hWnd != NULL )
	{
		//����ת��
		CRect rcScreen(nXPos, nYPos, nXPos+nWidth, nYPos+nHeight);
		ClientToScreen(&rcScreen);

		//�ƶ�����
		m_FlowerEffectControl[wChairID].MoveWindow(rcScreen);
	}

	return ;
}

//�滭�ʻ�
void CGameFrameView::ShowFlower(WORD wSourceChairID, WORD wTargetChairID, UINT uFlowerID, UINT uFlowerEffectID, bool bBombEffect)
{
	//�Ϸ��ж�
	ASSERT(wTargetChairID<CountArray(m_FlowerEffectControl) && wSourceChairID<CountArray(m_FlowerEffectControl));
	if (wTargetChairID>=CountArray(m_FlowerEffectControl) || wSourceChairID>=CountArray(m_FlowerEffectControl)) return;
	if(m_FlowerEffectControl[wTargetChairID].m_hWnd == NULL || m_FlowerEffectControl[wSourceChairID].m_hWnd == NULL) return ;
	if (m_nFlowerCount>0) return;

	for ( int n = 0; n < MAX_FLOWER_COUNT; ++n )
	{
		tagFlowerEffectInfo & FlowerInfo = m_FlowerInfo[n];

		if ( FlowerInfo.wMoveCount == 0 ) 
		{
			//��ȡλ��
			CRect rcVideo,rcTargetVideo;
			m_FlowerEffectControl[wSourceChairID].GetClientRect(&rcVideo);
			m_FlowerEffectControl[wTargetChairID].GetClientRect(&rcTargetVideo);
			m_FlowerEffectControl[wSourceChairID].MapWindowPoints(this, &rcVideo);
			m_FlowerEffectControl[wTargetChairID].MapWindowPoints(this, &rcTargetVideo);

			//λ����Ϣ
			FlowerInfo.ptFrom.x = rcVideo.left + rcVideo.Width()/2;
			FlowerInfo.ptFrom.y = rcVideo.top  + rcVideo.Height()/2;
			FlowerInfo.ptDest.x = rcTargetVideo.left + rcTargetVideo.Width()/2;
			FlowerInfo.ptDest.y = rcTargetVideo.top  + rcTargetVideo.Height()/2;

			//�ж�λ��
			if(FlowerInfo.ptFrom.x==FlowerInfo.ptDest.x && FlowerInfo.ptFrom.y==FlowerInfo.ptDest.y)
			{
				//��ʾ����
				m_FlowerEffectControl[wTargetChairID].StartFlowerEffect(uFlowerEffectID);
				m_FlowerEffectControl[wTargetChairID].BringWindowToTop();

				//�������
				ZeroMemory(&FlowerInfo, sizeof(FlowerInfo));
			}
			else
			{
				//��ʼλ��
				FlowerInfo.ptCurPos = FlowerInfo.ptFrom ;

				//��������
				INT nXCount=abs(FlowerInfo.ptDest.x-FlowerInfo.ptFrom.x)/m_nAnimeStep;
				INT nYCount=abs(FlowerInfo.ptDest.y-FlowerInfo.ptFrom.y)/m_nAnimeStep;
				FlowerInfo.wMoveCount=__max(1,__max(nXCount,nYCount));
				FlowerInfo.wMoveIndex = 0;

				//�û���Ϣ
				FlowerInfo.wSourceChairID = wSourceChairID;
				FlowerInfo.wTargetChairID = wTargetChairID;
				FlowerInfo.uFlowerID = uFlowerID;
				FlowerInfo.uFlowerEffectID = uFlowerEffectID;
				FlowerInfo.bBombEffect = bBombEffect;

				//��������
				m_nFlowerCount++;

				//�״��ж�
				if ( m_nFlowerCount == 1 ) SetTimer(IDI_FLOWER_START, 100, NULL);
			}
			break;
		}
	}

	return ;
}

//��ȡʱ��
WORD CGameFrameView::GetUserTimer(WORD wChairID)
{
	ASSERT(wChairID<MAX_CHAIR);
	if (wChairID>=MAX_CHAIR) return 0;
	return m_wTimer[wChairID];
}

//��ȡ���
const tagUserData * CGameFrameView::GetUserInfo(WORD wChairID)
{
	ASSERT(wChairID<MAX_CHAIR);
	if (wChairID>=MAX_CHAIR) return NULL;
	return m_pUserItem[wChairID];
}

//�滭����
void CGameFrameView::DrawViewImage(CDC * pDC, CSkinImage & SkinImage, enImageMode ImageMode)
{
	//��ȡλ��
	RECT rcClient;
	GetClientRect(&rcClient);

	//�滭λͼ
	switch (ImageMode)
	{
	case enMode_ElongGate:	//����ģʽ
		{
			CImageHandle ImageHandle(&SkinImage);
			int nImageWidth=ImageHandle->GetWidth();
			int nImageHeight=ImageHandle->GetHeight();
			ImageHandle->StretchBlt(pDC->m_hDC,0,0,rcClient.right,rcClient.bottom,0,0,nImageWidth,nImageHeight);
			return;
		}
	case enMode_Centent:	//����ģʽ
		{
			CImageHandle ImageHandle(&SkinImage);
			int nXPos=(rcClient.right-ImageHandle->GetWidth())/2;
			int nYPos=(rcClient.bottom-ImageHandle->GetHeight())/2;
			ImageHandle->BitBlt(pDC->m_hDC,nXPos,nYPos);
			return;
		}
	case enMode_Spread:		//ƽ��ģʽ
		{
			CImageHandle ImageHandle(&SkinImage);
			int nImageWidth=ImageHandle->GetWidth();
			int nImageHeight=ImageHandle->GetHeight();
			for (int nXPos=0;nXPos<rcClient.right;nXPos+=nImageWidth)
			{
				for (int nYPos=0;nYPos<rcClient.bottom;nYPos+=nImageHeight)
				{
					ImageHandle->BitBlt(pDC->m_hDC,nXPos,nYPos);
				}
			}
			return;
		}
	}
	return;
}

//�滭ͷ��
void CGameFrameView::DrawUserFace(CDC * pDC,const tagUserData * pUserData, int nXPos, int nYPos, bool bOffLine)
{
	ASSERT(m_pIUserFaceRes!=NULL && pUserData!=NULL);
	m_pIUserFaceRes->DrawNormalFace(pDC,nXPos,nYPos,pUserData->wFaceID,pUserData->dwUserID,pUserData->dwCustomFaceVer);
	return;
}

//�滭ͷ��
void CGameFrameView::DrawCustomFace(CDC * pDC,const tagUserData * pUserData, int nXPos, int nYPos)
{
	ASSERT(m_pIUserFaceRes!=NULL && pUserData!=NULL);
	m_pIUserFaceRes->DrawCustomBigFace(pDC,nXPos,nYPos,pUserData->dwUserID,pUserData->dwCustomFaceVer);

	return;
}

//�滭׼��
void CGameFrameView::DrawUserReady(CDC * pDC, int nXPos, int nYPos)
{
	//������Դ
	CPngImage ImageUserReady;
	ImageUserReady.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("USER_READY"));

	//�滭׼��
	CSize SizeImage(ImageUserReady.GetWidth(),ImageUserReady.GetHeight());
	ImageUserReady.DrawImage(pDC,nXPos-SizeImage.cx/2,nYPos-SizeImage.cy/2);

	return;
}

//�滭ʱ��
void CGameFrameView::DrawUserTimer(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea)
{
	//������Դ
	CPngImage ImageTimeBack;
	CPngImage ImageTimeNumber;
	ImageTimeBack.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("TIME_BACK"));
	ImageTimeNumber.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("TIME_NUMBER"));

	//��ȡ����
	const INT nNumberHeight=ImageTimeNumber.GetHeight();
	const INT nNumberWidth=ImageTimeNumber.GetWidth()/10;

	//������Ŀ
	LONG lNumberCount=0;
	WORD wNumberTemp=wTime;
	do
	{
		lNumberCount++;
		wNumberTemp/=10;
	} while (wNumberTemp>0L);

	//λ�ö���
	INT nYDrawPos=nYPos-nNumberHeight/2+1;
	INT nXDrawPos=nXPos+(lNumberCount*nNumberWidth)/2-nNumberWidth;

	//�滭����
	CSize SizeTimeBack(ImageTimeBack.GetWidth(),ImageTimeBack.GetHeight());
	ImageTimeBack.DrawImage(pDC,nXPos-SizeTimeBack.cx/2,nYPos-SizeTimeBack.cy/2);

	//�滭����
	for (LONG i=0;i<lNumberCount;i++)
	{
		//�滭����
		WORD wCellNumber=wTime%10;
		ImageTimeNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,wCellNumber*nNumberWidth,0);

		//���ñ���
		wTime/=10;
		nXDrawPos-=nNumberWidth;
	};

	return;
}

//͸���滭
void CGameFrameView::AlphaDrawImage(CDC * pDesDC, int nXDes, int nYDes, int nDesWidth, int nDesHeight, CDC * pScrDC, int nXScr, int nYScr, COLORREF crTransColor)
{
	::AlphaDrawImage(pDesDC,nXDes,nYDes,nDesWidth,nDesHeight,pScrDC,nXScr,nYScr,nDesWidth,nDesHeight,crTransColor);
	return;
}

//�滭͸��ͼ
void CGameFrameView::AlphaDrawImage(CDC * pDesDC, int nXDes, int nYDes, int nDesWidth, int nDesHeight, HBITMAP hBitBmp, int nXScr, int nYScr, COLORREF crTransColor)
{
	//���� DC
	CDC DCBuffer;
	DCBuffer.CreateCompatibleDC(NULL);
	HGDIOBJ hOldGdiObj=DCBuffer.SelectObject(hBitBmp);

	//�滭λͼ
	::AlphaDrawImage(pDesDC,nXDes,nYDes,nDesWidth,nDesHeight,&DCBuffer,nXScr,nYScr,nDesWidth,nDesHeight,crTransColor);

	//������Դ
	DCBuffer.SelectObject(hOldGdiObj);
	DCBuffer.DeleteDC();

	return;
}

//���ý���
void CGameFrameView::ResetData()
{
	//���ñ���
	memset(m_wTimer,0,sizeof(m_wTimer));
	memset(m_pUserItem,0,sizeof(m_pUserItem));

	//���ý���
	ResetGameView();
	UpdateGameView(NULL);

	return;
}

//���ýӿ�
bool CGameFrameView::SetUserFaceRes(IUnknownEx * pIUnknownEx)
{
	ASSERT(pIUnknownEx!=NULL);
	m_pIUserFaceRes=(IUserFaceRes *)pIUnknownEx->QueryInterface(IID_IUserFaceRes,VER_IUserFaceRes);
	return (m_pIUserFaceRes!=NULL);
}

//����ʱ��
bool CGameFrameView::SetUserTimer(WORD wChairID, WORD wTimer)
{
	ASSERT(wChairID<MAX_CHAIR);
	if (wChairID>=MAX_CHAIR) return false;
	m_wTimer[wChairID]=wTimer;
	UpdateGameView(NULL);
	return true;
}

//�����û�
bool CGameFrameView::SetUserInfo(WORD wChairID, tagUserData * pUserItem)
{
	ASSERT(wChairID<MAX_CHAIR);
	if (wChairID>=MAX_CHAIR) return false;
	m_pUserItem[wChairID]=pUserItem;
	UpdateGameView(NULL);
	return true;
}

//������Ϣ
int CGameFrameView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//��ʼ��
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);
	if (m_pThreadDraw!=NULL) m_pThreadDraw->BeginThread();
	
	//�����ؼ�
	for(WORD i=0;i<CountArray(m_FlowerEffectControl);i++)
	{
		SetFlowerControlInfo(i,0,0);
		m_FlowerEffectControl[i].CreateControl(this);
	}

	return 0;
}

//�滭����
void CGameFrameView::OnPaint()
{
	CPaintDC dc(this);

	//�ж�ģʽ
	if (m_pThreadDraw!=NULL) return;

	//��ȡλ��
	CRect ClientRect;
	GetClientRect(&ClientRect);

	//�ػ�������
	if ((m_bDoubleBuf==true)&&(m_ImageBuffer.IsNull()==false))
	{
		if (m_bReDraw==true)
		{
			//��������
			CFont DrawFont;
			DrawFont.CreateFont(-12,0,0,0,400,0,0,0,134,3,2,1,2,TEXT("����"));
			CDC * pDC=CDC::FromHandle(m_ImageBuffer.GetDC());
			CFont * pOldFont=pDC->SelectObject(&DrawFont);

			//���»���
			m_bReDraw=false;
			pDC->SetBkMode(TRANSPARENT);
			pDC->FillSolidRect(0,0,ClientRect.Width(),ClientRect.Height(),RGB(0,0,0));

			//�滭����
			DrawGameView(pDC,ClientRect.Width(),ClientRect.Height());

			//�滭�ʻ�
			for ( int n = 0; n < MAX_FLOWER_COUNT; ++n )
			{
				tagFlowerEffectInfo & FlowerInfo = m_FlowerInfo[n];

				if ( FlowerInfo.ptCurPos.x == 0 && FlowerInfo.ptCurPos.y == 0 ) continue;
				//m_FlowerResHelper->DrawGift(pDC, FlowerInfo.ptCurPos.x, FlowerInfo.ptCurPos.y, FlowerInfo.uFlowerID);

				//����ͼƬ
				CPngImage PngGift;
				PngGift.LoadImage(g_GlobalOption.m_PlatformResourceModule->GetResInstance(),g_GlobalOption.m_GiftFlashImage.pszGiftMove[FlowerInfo.uFlowerID]);

				//�滭ͼƬ
				PngGift.DrawImage(pDC, FlowerInfo.ptCurPos.x, FlowerInfo.ptCurPos.y);
			}

			//������Դ
			pDC->SelectObject(pOldFont);
			m_ImageBuffer.ReleaseDC();
			DrawFont.DeleteObject();
		}

		//�滭����
		CRect rcClip;
		dc.GetClipBox(&rcClip);
		m_ImageBuffer.BitBlt(dc,rcClip.left,rcClip.top,rcClip.Width(),rcClip.Height(),rcClip.left,rcClip.top);
	}
	else 
	{
		//��������
		CFont DrawFont;
		DrawFont.CreateFont(-12,0,0,0,400,0,0,0,134,3,2,1,2,TEXT("����"));
		CFont * pOldFont=dc.SelectObject(&DrawFont);

		//�滭����
		dc.SetBkMode(TRANSPARENT);
		DrawGameView(&dc,ClientRect.Width(),ClientRect.Height());

		//������Դ
		dc.SelectObject(pOldFont);
		DrawFont.DeleteObject();
	}

	return;
}

//λ�ñ仯
void CGameFrameView::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
	
	//Ч��״̬
	if ((cx==0)||(cy==0)) return;
	if (GetSystemMetrics(SM_CXSCREEN)<cx) return;

	//���Ļ���ͼ
	if ((m_bDoubleBuf==true)&&(nType!=SIZE_MINIMIZED))
	{
		if ((cx>m_ImageSize.cx)||(cy>m_ImageSize.cy))
		{
			m_bReDraw=true;
			m_ImageBuffer.Destroy();
			m_ImageSize.cx=__max(cx,m_ImageSize.cx);
			m_ImageSize.cy=__max(cy,m_ImageSize.cy);
			m_ImageBuffer.Create(m_ImageSize.cx,m_ImageSize.cy,m_uColorCount);
		}
	}

	//������ͼ
	RectifyGameView(cx,cy);

	//���û滭����
	if (m_pThreadDraw!=NULL)
	{
		m_pThreadDraw->SetClientRange(cx,cy);
		if (nType==SIZE_MINIMIZED) m_pThreadDraw->SuspendDrawThread();
		else m_pThreadDraw->ResumeDrawThread();
	}
	else UpdateGameView(NULL);

	return;
}

//�����Ϣ
void CGameFrameView::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	__super::OnLButtonDown(nFlags, point);
}

//������Ϣ
void CGameFrameView::OnDestroy()
{
	__super::OnDestroy();

	//�����߳�
	if (m_pThreadDraw!=NULL) m_pThreadDraw->EndThread();

	return;
}

//ʱ����Ϣ
void CGameFrameView::OnTimer(UINT nIDEvent)
{
	if ( nIDEvent == IDI_FLOWER_START )
	{
		//�ƶ��ж�
		for ( int nIndex = 0; nIndex < MAX_FLOWER_COUNT; ++nIndex )
		{
			//�Ϸ��ж�
			if(m_FlowerInfo[nIndex].wMoveCount==0)continue;

			//�ƶ�����
			if(m_FlowerInfo[nIndex].wMoveIndex<m_FlowerInfo[nIndex].wMoveCount)
			{
				m_FlowerInfo[nIndex].wMoveIndex++;
				WORD wMoveIndex = m_FlowerInfo[nIndex].wMoveIndex;
				WORD wMoveCount = m_FlowerInfo[nIndex].wMoveCount;
				m_FlowerInfo[nIndex].ptCurPos.x =m_FlowerInfo[nIndex].ptFrom.x + (m_FlowerInfo[nIndex].ptDest.x-m_FlowerInfo[nIndex].ptFrom.x)*wMoveIndex/wMoveCount;
				m_FlowerInfo[nIndex].ptCurPos.y =m_FlowerInfo[nIndex].ptFrom.y + (m_FlowerInfo[nIndex].ptDest.y-m_FlowerInfo[nIndex].ptFrom.y)*wMoveIndex/wMoveCount;
			}
			//�����ж�
			else
			{
				//��ʾ����
				m_FlowerEffectControl[m_FlowerInfo[nIndex].wTargetChairID].StartFlowerEffect(m_FlowerInfo[nIndex].uFlowerEffectID);
				m_FlowerEffectControl[m_FlowerInfo[nIndex].wTargetChairID].BringWindowToTop();

				//���ٴ���
				m_nFlowerCount--;

				//�������
				ZeroMemory(&m_FlowerInfo[nIndex], sizeof(m_FlowerInfo[nIndex]));
			}
		}

		//�����ж�
		if ( m_nFlowerCount == 0 ) KillTimer(nIDEvent);

		//���½���
		UpdateGameView(NULL);

		return;
	}
	__super::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////
