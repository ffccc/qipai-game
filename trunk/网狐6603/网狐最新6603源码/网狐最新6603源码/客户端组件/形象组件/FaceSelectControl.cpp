#include "StdAfx.h"
#include "Resource.h"
#include "FaceSelectControl.h"

//////////////////////////////////////////////////////////////////////////////////
//�궨��

//λ�ö���
#define TOP_PELS					10									//�ϸ�����
#define LINE_PELS					10									//��������
#define FRAME_WIDTH					12									//��ܿ��
#define FRAME_HEIGHT				12									//��ܸ߶�

//��������
#define WHELL_PELS					5									//��������

//��ɫ����
#define CR_BACK_FRAME				RGB(250,250,250)					//������ɫ

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CFaceItemSelectWnd, CWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_VSCROLL()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CFaceSelectControl, CSkinDialog)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CFaceItemSelectWnd::CFaceItemSelectWnd()
{
	//������Ϣ
	m_nMaxPels=0;
	m_nPagePels=0;
	m_nLineCount=0;
	m_nWidthCount=0;
	m_nCurrentPos=0;
	m_nXExcursion=0;

	//ͷ�����
	m_wFaceCount=0;
	m_wSelectFace=INVALID_WORD;

	//���ñ���
	m_pIFaceItemSelectEvent=NULL;

	return;
}

//��������
CFaceItemSelectWnd::~CFaceItemSelectWnd()
{
}

//ѡ����Ŀ
VOID CFaceItemSelectWnd::SetAllowItemFull()
{
	//��ȡ����
	ASSERT(CFaceItemControl::GetInstance()!=NULL);
	IFaceItemControl * pIFaceItemControl=CFaceItemControl::GetInstance();

	//���ñ���
	m_wFaceCount=pIFaceItemControl->GetFaceCount();
	if (m_wSelectFace!=INVALID_WORD) m_wSelectFace=__min(m_wSelectFace,m_wFaceCount);

	//�����ؼ�
	if (m_hWnd!=NULL)
	{
		CRect rcRect;
		GetClientRect(&rcRect);
		RectifyControl(rcRect.Width(),rcRect.Height());
	}

	return;
}

//ѡ����Ŀ
VOID CFaceItemSelectWnd::SetAllowItemCount(WORD wFaceCount)
{
	//��ȡ����
	ASSERT(CFaceItemControl::GetInstance()!=NULL);
	IFaceItemControl * pIFaceItemControl=CFaceItemControl::GetInstance();

	//���ñ���
	m_wFaceCount=wFaceCount;
	m_wFaceCount=__min(m_wFaceCount,pIFaceItemControl->GetFaceCount());

	//��������
	if (m_wSelectFace!=INVALID_WORD)
	{
		m_wSelectFace=__min(m_wSelectFace,m_wFaceCount);
	}

	//�����ؼ�
	if (m_hWnd!=NULL)
	{
		CRect rcRect;
		GetClientRect(&rcRect);
		RectifyControl(rcRect.Width(),rcRect.Height());
	}

	return;
}

//�滭��Ϣ 
VOID CFaceItemSelectWnd::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//�滭����
	CRect rcClip;
	dc.GetClipBox(&rcClip);
	dc.FillSolidRect(&rcClip,CR_BACK_FRAME);

	//��������
	INT nXBeginIndex=(rcClip.left-m_nXExcursion)/(FRAME_WIDTH+FACE_CX);
	INT nYBeginIndex=(rcClip.top+m_nCurrentPos-TOP_PELS)/(FRAME_HEIGHT+FACE_CY);

	//��������
	nXBeginIndex=__max(0,nXBeginIndex);
	nYBeginIndex=__max(0,nYBeginIndex);

	//�滭ͷ��
	for (INT x=nXBeginIndex;x<m_nWidthCount;x++)
	{
		//�����ж�
		INT nXDrawPos=m_nXExcursion+x*(FRAME_WIDTH+FACE_CX);
		if (nXDrawPos>rcClip.right) break;

		//����滭
		for (INT y=nYBeginIndex;y<m_nLineCount;y++)
		{
			//�����ж�
			WORD wIndex=(m_nWidthCount*y+x);
			if (wIndex>=m_wFaceCount) break;

			//�����ж�
			INT nYDrawPos=TOP_PELS+(FRAME_HEIGHT+FACE_CY)*y-m_nCurrentPos;
			if (nYDrawPos>rcClip.bottom) break;

			//�滭ͷ��
			DrawFaceItem(&dc,wIndex,nXDrawPos,nYDrawPos,m_wSelectFace==wIndex);
		}
	}

	return;
}

//λ����Ϣ
VOID CFaceItemSelectWnd::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType,cx,cy);

	//�����ؼ�
	RectifyControl(cx,cy);

	return;
}

//������Ϣ
INT CFaceItemSelectWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//���ù���
	m_SkinScrollBar.InitScroolBar(this);

	return 0;
}

//�����Ϣ
VOID CFaceItemSelectWnd::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//��Ч�ж�
	if (m_wSelectFace==INVALID_WORD) return;

	//�¼�֪ͨ
	ASSERT(m_pIFaceItemSelectEvent!=NULL);
	if (m_pIFaceItemSelectEvent!=NULL) m_pIFaceItemSelectEvent->OnEventFaceSelect(m_wSelectFace);

	return;
}

//������Ϣ
VOID CFaceItemSelectWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	__super::OnKeyDown(nChar,nRepCnt,nFlags);

	switch (nChar)
	{
	case VK_UP:
		{
			SendMessage(WM_VSCROLL,MAKELONG(SB_LINEUP,m_nCurrentPos),NULL);
			return;
		}
	case VK_DOWN:
		{
			SendMessage(WM_VSCROLL,MAKELONG(SB_LINEDOWN,m_nCurrentPos),NULL);
			return;
		}
	case VK_PRIOR:
		{
			SendMessage(WM_VSCROLL,MAKELONG(SB_PAGEUP,m_nCurrentPos),NULL);
			return;
		}
	case VK_NEXT:
	case VK_SPACE:
		{
			SendMessage(WM_VSCROLL,MAKELONG(SB_PAGEDOWN,m_nCurrentPos),NULL);
			return;
		}
	case VK_HOME:
		{
			SendMessage(WM_VSCROLL,MAKELONG(SB_TOP,m_nCurrentPos),NULL);
			return;
		}
	case VK_END:
		{
			SendMessage(WM_VSCROLL,MAKELONG(SB_BOTTOM,m_nCurrentPos),NULL);
			return;
		}
	}

	return;
}

//�����Ϣ
BOOL CFaceItemSelectWnd::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//����ж�
	WORD wLastFaceID=m_wSelectFace;
	WORD wHoverFaceID=MouseHitTest(MousePoint);

	//��ͬ�ж�
	if (wHoverFaceID!=m_wSelectFace)
	{
		//��������
		CClientDC ClientDC(this);

		//���ñ���
		m_wSelectFace=wHoverFaceID;

		//����ѡ��
		if (wLastFaceID!=INVALID_WORD)
		{
			//����λ��
			INT nXPos=m_nXExcursion+wLastFaceID%m_nWidthCount*(FRAME_WIDTH+FACE_CX);
			INT nYPos=TOP_PELS+wLastFaceID/m_nWidthCount*(FRAME_HEIGHT+FACE_CY)-m_nCurrentPos;

			//�滭ͷ��
			DrawFaceItem(&ClientDC,wLastFaceID,nXPos,nYPos,false);
		}

		//�滭ѡ��
		if (m_wSelectFace!=INVALID_WORD)
		{
			//����λ��
			INT nXPos=m_nXExcursion+m_wSelectFace%m_nWidthCount*(FRAME_WIDTH+FACE_CX);
			INT nYPos=TOP_PELS+m_wSelectFace/m_nWidthCount*(FRAME_HEIGHT+FACE_CY)-m_nCurrentPos;

			//�滭ͷ��
			DrawFaceItem(&ClientDC,m_wSelectFace,nXPos,nYPos,true);
		}
	}

	//���ù��
	if (m_wSelectFace!=INVALID_WORD)
	{
		SetCursor(LoadCursor(GetModuleHandle(AVATAR_CONTROL_DLL_NAME),MAKEINTRESOURCE(IDC_HAND_CUR)));
		return TRUE;
	}

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//������Ϣ
BOOL CFaceItemSelectWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint Point)
{
	__super::OnMouseWheel(nFlags,zDelta,Point);

	//���ñ���
	INT nLastPos=m_nCurrentPos;
	m_nCurrentPos=__max(0,__min(m_nCurrentPos-zDelta/WHELL_PELS,m_nMaxPels-m_nPagePels));

	//���ù���
	SetScrollPos(SB_VERT,m_nCurrentPos);
	ScrollWindow(0,nLastPos-m_nCurrentPos,NULL,NULL);

	return TRUE;
}

//������Ϣ
VOID CFaceItemSelectWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar)
{
	__super::OnVScroll(nSBCode,nPos,pScrollBar);

	INT nLastPos=m_nCurrentPos;
	switch (nSBCode)
	{
	case SB_TOP:
		{
			m_nCurrentPos=0;
			break;
		}
	case SB_BOTTOM:
		{
			m_nCurrentPos=m_nMaxPels;
			break;
		}
	case SB_LINEUP:
		{
			m_nCurrentPos-=LINE_PELS;
			break;
		}
	case SB_PAGEUP:
		{
			m_nCurrentPos-=m_nPagePels;
			break;
		}
	case SB_LINEDOWN:
		{
			m_nCurrentPos+=LINE_PELS;
			break;
		}
	case SB_PAGEDOWN:
		{
			m_nCurrentPos+=m_nPagePels;
			break;
		}
	case SB_THUMBTRACK:
		{
			m_nCurrentPos=nPos;
			break;
		}
	}
	
	//���ý���
	SetFocus();

	//����λ��
	m_nCurrentPos=__max(0,__min(m_nCurrentPos,m_nMaxPels-m_nPagePels));

	//���ù���
	if (nLastPos!=m_nCurrentPos)
	{
		SetScrollPos(SB_VERT,m_nCurrentPos);
		ScrollWindow(0,nLastPos-m_nCurrentPos,NULL,NULL);
	}
	
	return;
}

//��������
WORD CFaceItemSelectWnd::MouseHitTest(CPoint MousePoint)
{
	//���ñ���
	MousePoint.y+=m_nCurrentPos;

	//��Χ�ж�
	if ((MousePoint.y<TOP_PELS)||(MousePoint.x<m_nXExcursion))
	{
		return INVALID_WORD;
	}

	//��������
	INT nXIndex=(MousePoint.x-m_nXExcursion)/(FRAME_WIDTH+FACE_CX);
	if (nXIndex>=m_nWidthCount) return INVALID_WORD;

	//��������
	INT nYIndex=(MousePoint.y-TOP_PELS)/(FRAME_HEIGHT+FACE_CY);
	if (nYIndex>=m_nLineCount) return INVALID_WORD;

	//��������
	WORD wIndexHit=nYIndex*m_nWidthCount+nXIndex;
	if (wIndexHit>=m_wFaceCount) return INVALID_WORD;

	return wIndexHit;
}

//�����ؼ�
VOID CFaceItemSelectWnd::RectifyControl(INT nWidth, INT nHeight)
{
	//״̬�ж�
	if ((nWidth==0)||(nHeight==0)) return;

	//��������
	m_nWidthCount=nWidth/(FRAME_WIDTH+FACE_CX);
	m_nLineCount=(m_wFaceCount+m_nWidthCount-1)/m_nWidthCount;

	//ҳ�����
	m_nPagePels=nHeight;
	m_nMaxPels=TOP_PELS*2+m_nLineCount*(FRAME_HEIGHT+FACE_CY);
	m_nXExcursion=(nWidth-m_nWidthCount*(FRAME_WIDTH+FACE_CX))/2;
	m_nCurrentPos=__max(0,__min(m_nCurrentPos,m_nMaxPels-m_nPagePels));

	//���ù���
	SCROLLINFO ScrollInfo;
	ScrollInfo.cbSize=sizeof(SCROLLINFO);
	ScrollInfo.fMask=SIF_RANGE|SIF_PAGE|SIF_POS;
	ScrollInfo.nMin=0;
	ScrollInfo.nMax=m_nMaxPels;
	ScrollInfo.nPage=m_nPagePels;
	ScrollInfo.nPos=m_nCurrentPos;
	SetScrollInfo(SB_VERT,&ScrollInfo,TRUE);

	return;
}

//�滭ͷ��
VOID CFaceItemSelectWnd::DrawFaceItem(CDC * pDC, WORD wIndex, INT nXPos, INT nYPos, bool bHover)
{
	//��ȡ����
	ASSERT(CFaceItemControl::GetInstance()!=NULL);
	IFaceItemControl * pIFaceItemControl=CFaceItemControl::GetInstance();

	//��������
	CImage ImageBuffer;
	ImageBuffer.Create(FRAME_WIDTH+FACE_CX,FRAME_HEIGHT+FACE_CY,32);

	//�滭����
	CDC * pBufferDC=CDC::FromHandle(ImageBuffer.GetDC());
	pBufferDC->FillSolidRect(0,0,FRAME_WIDTH+FACE_CX,FRAME_HEIGHT+FACE_CY,CR_BACK_FRAME);

	//�滭���
	BYTE cbFrameKind=(bHover==true)?ITEM_FRAME_HOVER:ITEM_FRAME_NORMAL;
	pIFaceItemControl->DrawFaceItemFrame(pBufferDC,FRAME_WIDTH/2,FRAME_HEIGHT/2,cbFrameKind);

	//�滭ͷ��
	pIFaceItemControl->DrawFaceNormal(pBufferDC,FRAME_WIDTH/2,FRAME_HEIGHT/2,wIndex);

	//�滭����
	pDC->BitBlt(nXPos,nYPos,FRAME_WIDTH+FACE_CX,FRAME_HEIGHT+FACE_CY,pBufferDC,0,0,SRCCOPY);

	//������Դ
	ImageBuffer.ReleaseDC();

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CFaceSelectControl::CFaceSelectControl() : CSkinDialog(IDD_DLG_FACE_SELECT)
{
	//���ýӿ�
	m_FaceItemSelectWnd.SetFaceItemSelectEvent(this);

	return;
}

//��������
CFaceSelectControl::~CFaceSelectControl()
{
}

//�ӿڲ�ѯ
VOID * CFaceSelectControl::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IFaceSelectControl,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IFaceSelectControl,Guid,dwQueryVer);
	return NULL;
}

//ѡ��ͷ��
bool CFaceSelectControl::GetSelectFaceID(WORD & wFaceID)
{
	//������Դ
	AfxSetResourceHandle(GetModuleHandle(AVATAR_CONTROL_DLL_NAME));

	//��ʾ����
	if (DoModal()!=IDOK) return false;

	//���ñ���
	wFaceID=m_FaceItemSelectWnd.GetSelectFace();

	return true;
}

//ͼ���¼�
VOID CFaceSelectControl::OnEventFaceSelect(WORD wFaceID)
{
	//�رմ���
	EndDialog(IDOK);

	return;
}

//���ú���
BOOL CFaceSelectControl::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("ͷ��ѡ��"));

	//������Դ
	AfxSetResourceHandle(GetModuleHandle(NULL));

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//�����ؼ�
	CRect rcSelectWnd;
	rcSelectWnd.top=m_SkinAttribute.m_EncircleInfoFrame.nTBorder;
	rcSelectWnd.left=m_SkinAttribute.m_EncircleInfoFrame.nLBorder;
	rcSelectWnd.right=rcClient.Width()-m_SkinAttribute.m_EncircleInfoFrame.nRBorder;
	rcSelectWnd.bottom=rcClient.Height()-m_SkinAttribute.m_EncircleInfoFrame.nBBorder;
	m_FaceItemSelectWnd.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_VSCROLL,rcSelectWnd,this,100);

	//���ý���
	m_FaceItemSelectWnd.SetFocus();
	m_FaceItemSelectWnd.SetAllowItemFull();

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////

//�����������
DECLARE_CREATE_MODULE(FaceSelectControl);

//////////////////////////////////////////////////////////////////////////////////
