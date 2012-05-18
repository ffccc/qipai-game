#include "Stdafx.h"
#include "ImageEditorControl.h"

//////////////////////////////////////////////////////////////////////////////////

//������
#define HT_NONE						0x00								//��Ч����
#define HT_IMAGE					0x10								//ͼƬ����
#define HT_FRAME_L					0x01								//����߿�
#define HT_FRAME_R					0x02								//����߿�
#define HT_FRAME_T					0x04								//����߿�
#define HT_FRAME_B					0x08								//����߿�

//������ʽ
#define LOCK_NULL					0x00								//��Ч����
#define LOCK_TYPE_L					0x01								//����߿�
#define LOCK_TYPE_R					0x02								//����߿�
#define LOCK_TYPE_T					0x04								//����߿�
#define LOCK_TYPE_B					0x08								//����߿�

//��������
#define OPERATE_NULL				0x00								//��Ч����
#define OPERATE_DRAG				0x01								//�����ק
#define OPERATE_SIZE				0x02								//�ı�ߴ�

//��������
#define COLOR_CONTROL_FRMAE			RGB(10,124,202)						//������ɫ

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CImageEditorControl, CWnd)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CImageEditorControl::CImageEditorControl()
{
	//״̬����
	m_bVShowLine=true;
	m_cbConsultType=LOCK_NULL;
	m_cbOperateType=OPERATE_NULL;
	m_rcImageSelect.SetRectEmpty();

	//��������
	m_cbHitTest=HT_NONE;
	m_ptLockPoint.SetPoint(0,0);
	m_ptDragOrigin.SetPoint(0,0);

	//״̬����
	m_dZoomPercent=1.0;
	m_SizeSource.SetSize(0,0);
	m_SizeTarget.SetSize(0,0);

	//�ӿڱ���
	m_pIImageEditorEvent=NULL;

	return;
}

//��������
CImageEditorControl::~CImageEditorControl()
{
	//������Դ
	m_ImageSource.Destroy();

	return;
}

//����ͼƬ
bool CImageEditorControl::DestoryImage()
{
	//������Դ
	m_ImageSource.Destroy();

	//״̬����
	m_cbConsultType=LOCK_NULL;
	m_cbOperateType=OPERATE_NULL;
	m_rcImageSelect.SetRectEmpty();

	//��������
	m_cbHitTest=HT_NONE;
	m_ptLockPoint.SetPoint(0,0);
	m_ptDragOrigin.SetPoint(0,0);

	//״̬����
	m_dZoomPercent=1.0;
	m_SizeSource.SetSize(0,0);
	m_SizeTarget.SetSize(0,0);

	//���½���
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

	return true;
}


//����ͼƬ
bool CImageEditorControl::LoadEditImage(CImage & Image)
{
	//״̬Ч��
	ASSERT(Image.IsNull()==false);
	if (Image.IsNull()==true) return false;

	//������Դ
	m_ImageSource.Destroy();

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//����ͼƬ
	m_ImageSource.Attach(Image.Detach());

	//���ű���
	if (((DOUBLE)rcClient.Width()/(DOUBLE)rcClient.Height())<((DOUBLE)m_ImageSource.GetWidth()/(DOUBLE)m_ImageSource.GetHeight()))
	{
		m_dZoomPercent=(DOUBLE)rcClient.Width()/(DOUBLE)m_ImageSource.GetWidth();
	}
	else
	{
		m_dZoomPercent=(DOUBLE)rcClient.Height()/(DOUBLE)m_ImageSource.GetHeight();
	}

	//���ô�С
	m_SizeSource.SetSize(m_ImageSource.GetWidth(),m_ImageSource.GetHeight());
	m_SizeTarget.SetSize((INT)(m_SizeSource.cx*m_dZoomPercent),(INT)(m_SizeSource.cy*m_dZoomPercent));

	//ѡ������
	INT nSelectArea=__min(m_SizeTarget.cx,m_SizeTarget.cy)*4/5L;
	m_rcImageSelect.SetRect((rcClient.Width()-nSelectArea)/2,(rcClient.Height()-nSelectArea)/2,(rcClient.Width()+nSelectArea)/2,(rcClient.Height()+nSelectArea)/2);

	//���½���
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

	//�¼�֪ͨ
	ASSERT(m_pIImageEditorEvent!=NULL);
	if (m_pIImageEditorEvent!=NULL) m_pIImageEditorEvent->OnEventImageSelect(true,false);

	return true;
}

//����ͼƬ
bool CImageEditorControl::LoadEditImage(LPCTSTR pszImage)
{
	//����ͼƬ
	CImage ImageSource;
	if (ImageSource.Load(pszImage)!=S_OK)
	{
		//�������
		ASSERT(FALSE);

		//�¼�֪ͨ
		ASSERT(m_pIImageEditorEvent!=NULL);
		if (m_pIImageEditorEvent!=NULL) m_pIImageEditorEvent->OnEventImageSelect(false,false);

		return false;
	}

	//����ͼƬ
	if (LoadEditImage(ImageSource)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//��תͼƬ
VOID CImageEditorControl::SetRoundImage(BYTE cbRoundType)
{
	//״̬Ч��
	ASSERT(m_ImageSource.IsNull()==false);
	if (m_ImageSource.IsNull()==true) return;

	//����λ��
	CSize SizeSource(m_ImageSource.GetWidth(),m_ImageSource.GetHeight());
	CSize SizeResult(m_ImageSource.GetHeight(),m_ImageSource.GetWidth());

	//����λͼ
	CImage ImageSource;
	CImage ImageResult;
	ImageSource.Create(m_ImageSource.GetWidth(),m_ImageSource.GetHeight(),32);
	ImageResult.Create(m_ImageSource.GetHeight(),m_ImageSource.GetWidth(),32);

	//�滭λͼ
	CDC * pDCResult=CDC::FromHandle(ImageResult.GetDC());
	CDC * pDCSource=CDC::FromHandle(ImageSource.GetDC());
	m_ImageSource.BitBlt(pDCSource->m_hDC,0,0,m_ImageSource.GetWidth(),m_ImageSource.GetHeight(),0,0);

	//��ȡ����
	INT nPitchResult=ImageResult.GetPitch();
	INT nPitchSource=ImageSource.GetPitch();

	//��ȡ����
	LPBYTE cbBitResult=(LPBYTE)ImageResult.GetBits();
	LPBYTE cbBitSource=(LPBYTE)ImageSource.GetBits();

	//��������
	for (INT nYSourcePos=0;nYSourcePos<SizeSource.cy;nYSourcePos++)
	{
		for (INT nXSourcePos=0;nXSourcePos<SizeSource.cx;nXSourcePos++)
		{
			//Ŀ��λ��
			INT nXResultPos=(cbRoundType==ROUND_TYPE_LEFT)?nYSourcePos:(SizeSource.cy-nYSourcePos-1);
			INT nYResultPos=(cbRoundType==ROUND_TYPE_LEFT)?(SizeSource.cx-nXSourcePos-1):nXSourcePos;

			//������ɫ
			DWORD dwSource=nYSourcePos*nPitchSource+nXSourcePos*4L;
			DWORD dwResult=nYResultPos*nPitchResult+nXResultPos*4L;
			*(COLORREF *)(cbBitResult+dwResult)=*(COLORREF *)(cbBitSource+dwSource);
		}
	}

	//�ͷŶ���
	ImageSource.ReleaseDC();
	ImageResult.ReleaseDC();

	//����ͼƬ
	if (LoadEditImage(ImageResult)==false)
	{
		ASSERT(FALSE);
		return;
	}

	return;
}

//�����ж�
bool CImageEditorControl::IsNull()
{
	return m_ImageSource.IsNull();
}

//�滭ͼƬ
VOID CImageEditorControl::DrawEditImage(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight)
{
	//״̬Ч��
	ASSERT(m_ImageSource.IsNull()==false);
	if (m_ImageSource.IsNull()==true) return;

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//���û���
	pDC->SetStretchBltMode(HALFTONE);

	//�滭ͼƬ
	m_ImageSource.StretchBlt(pDC->m_hDC,nXPos,nYPos,nWidth,nHeight,
		m_SizeSource.cx*(m_rcImageSelect.left-(rcClient.Width()-m_SizeTarget.cx)/2)/m_SizeTarget.cx,
		m_SizeSource.cy*(m_rcImageSelect.top-(rcClient.Height()-m_SizeTarget.cy)/2)/m_SizeTarget.cy,
		m_SizeSource.cx*m_rcImageSelect.Width()/m_SizeTarget.cx,m_SizeSource.cy*m_rcImageSelect.Height()/m_SizeTarget.cy);

	return;
}

//��������
VOID CImageEditorControl::SetVLineStatus(bool bShowVLine)
{
	if (bShowVLine!=m_bVShowLine)
	{
		//���ñ���
		m_bVShowLine=bShowVLine;

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);
	}

	return;
}

//�滭����
VOID CImageEditorControl::DrawLineFrame(CDC * pDC)
{
	//��������
	INT nXSpace=m_rcImageSelect.Width()/3;
	INT nYSpace=m_rcImageSelect.Height()/3;

	//�滭�߿�
	pDC->Draw3dRect(&m_rcImageSelect,COLOR_CONTROL_FRMAE,COLOR_CONTROL_FRMAE);

	//�滭����
	pDC->FillSolidRect(m_rcImageSelect.left-2,m_rcImageSelect.top-2,4,4,COLOR_CONTROL_FRMAE);
	pDC->FillSolidRect(m_rcImageSelect.right-1,m_rcImageSelect.top-2,4,4,COLOR_CONTROL_FRMAE);
	pDC->FillSolidRect(m_rcImageSelect.left+m_rcImageSelect.Width()/2-1,m_rcImageSelect.top-2,4,4,COLOR_CONTROL_FRMAE);

	//�滭����
	pDC->FillSolidRect(m_rcImageSelect.left-2,m_rcImageSelect.bottom-2,4,4,COLOR_CONTROL_FRMAE);
	pDC->FillSolidRect(m_rcImageSelect.right-1,m_rcImageSelect.bottom-2,4,4,COLOR_CONTROL_FRMAE);
	pDC->FillSolidRect(m_rcImageSelect.left+m_rcImageSelect.Width()/2-1,m_rcImageSelect.bottom-2,4,4,COLOR_CONTROL_FRMAE);

	//�滭����
	pDC->FillSolidRect(m_rcImageSelect.left-2,m_rcImageSelect.top+m_rcImageSelect.Height()/2,4,4,COLOR_CONTROL_FRMAE);
	pDC->FillSolidRect(m_rcImageSelect.right-2,m_rcImageSelect.top+m_rcImageSelect.Height()/2,4,4,COLOR_CONTROL_FRMAE);

	//�滭����
	if (m_bVShowLine==true)
	{
		//��������
		CPen PenDot(PS_DOT,1,RGB(117,130,161));
		CPen * pOldPen=pDC->SelectObject(&PenDot);

		//���� DC
		pDC->SetBkMode(TRANSPARENT);

		//�滭����
		pDC->MoveTo(m_rcImageSelect.left+nXSpace,m_rcImageSelect.top);
		pDC->LineTo(m_rcImageSelect.left+nXSpace,m_rcImageSelect.bottom);

		//�滭����
		pDC->MoveTo(m_rcImageSelect.left+nXSpace*2,m_rcImageSelect.top);
		pDC->LineTo(m_rcImageSelect.left+nXSpace*2,m_rcImageSelect.bottom);

		//�滭����
		pDC->MoveTo(m_rcImageSelect.left,m_rcImageSelect.top+nYSpace);
		pDC->LineTo(m_rcImageSelect.right,m_rcImageSelect.top+nYSpace);

		//�滭����
		pDC->MoveTo(m_rcImageSelect.left,m_rcImageSelect.top+nYSpace*2);
		pDC->LineTo(m_rcImageSelect.right,m_rcImageSelect.top+nYSpace*2);

		//���� DC
		pDC->SelectObject(pOldPen);
	}

	return;
}

//�������
BYTE CImageEditorControl::OnEventHitTest(CPoint & MousePoint)
{
	//��������
	BYTE cbHitTest=HT_NONE;

	//������
	if (m_rcImageSelect.PtInRect(MousePoint)==TRUE)
	{
		//�������
		if (MousePoint.y<=m_rcImageSelect.top+3)
		{
			cbHitTest|=HT_FRAME_T;
		}

		//�������
		if (MousePoint.y>=m_rcImageSelect.bottom-3)
		{
			cbHitTest|=HT_FRAME_B;
		}

		//�������
		if(MousePoint.x<=m_rcImageSelect.left+3)
		{
			cbHitTest|=HT_FRAME_L;
		}

		//�������
		if (MousePoint.x>=m_rcImageSelect.right-3)
		{
			cbHitTest|=HT_FRAME_R;
		}

		//ͼƬ����
		if (cbHitTest==0) cbHitTest=HT_IMAGE;
	}

	return cbHitTest;
}

//���Ʊ���
BOOL CImageEditorControl::OnEraseBkgnd(CDC * pDC)
{
	//��ȡ��С
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//��������
	HDC hBufferDC=ImageBuffer.GetDC();
	CDC * pBufferDC=CDC::FromHandle(hBufferDC);

	//�滭����
	pBufferDC->FillSolidRect(&rcClient,RGB(255,255,255));

	//�滭ͼ��
	if (m_ImageSource.IsNull()==false)
	{
		//����ͼƬ
		CPngImage ImageEditGray;
		ImageEditGray.LoadImage(GetModuleHandle(AVATAR_CONTROL_DLL_NAME),TEXT("FACE_EDIT_GRAY"));

		//���û���
		pBufferDC->SetStretchBltMode(HALFTONE);

		//�滭ͼƬ
		m_ImageSource.StretchBlt(hBufferDC,(rcClient.Width()-m_SizeTarget.cx)/2,(rcClient.Height()-m_SizeTarget.cy)/2,
			m_SizeTarget.cx,m_SizeTarget.cy,0,0,m_SizeSource.cx,m_SizeSource.cy);

		//�滭��ͼ
		ImageEditGray.DrawImage(pBufferDC,(rcClient.Width()-m_SizeTarget.cx)/2,(rcClient.Height()-m_SizeTarget.cy)/2,
			m_SizeTarget.cx,m_SizeTarget.cy,0,0,ImageEditGray.GetWidth(),ImageEditGray.GetHeight());

		//�滭ͼƬ
		m_ImageSource.StretchBlt(hBufferDC,m_rcImageSelect.left,m_rcImageSelect.top,m_rcImageSelect.Width(),m_rcImageSelect.Height(),
			m_SizeSource.cx*(m_rcImageSelect.left-(rcClient.Width()-m_SizeTarget.cx)/2)/m_SizeTarget.cx,
			m_SizeSource.cy*(m_rcImageSelect.top-(rcClient.Height()-m_SizeTarget.cy)/2)/m_SizeTarget.cy,
			m_SizeSource.cx*m_rcImageSelect.Width()/m_SizeTarget.cx,m_SizeSource.cy*m_rcImageSelect.Height()/m_SizeTarget.cy);

		//�滭���
		DrawLineFrame(pBufferDC);
	}

	//�滭����
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	//������Դ
	ImageBuffer.ReleaseDC();

	return TRUE;
}

//����ƶ�
VOID CImageEditorControl::OnMouseMove(UINT nFlags, CPoint MousePoint)
{
	//�ı��С
	if (m_cbOperateType==OPERATE_SIZE)
	{
		//��ȡλ��
		CRect rcClient;
		GetClientRect(&rcClient);

		//��������
		CRect rcImageSelect=m_rcImageSelect;

		//ͼƬ����
		CRect rcImageArea;
		rcImageArea.top=(rcClient.Height()-m_SizeTarget.cy)/2L;
		rcImageArea.left=(rcClient.Width()-m_SizeTarget.cx)/2L;
		rcImageArea.right=(rcClient.Width()+m_SizeTarget.cx)/2L;
		rcImageArea.bottom=(rcClient.Height()+m_SizeTarget.cy)/2L;

		//�¼�����
		switch (m_cbConsultType)
		{
		case LOCK_TYPE_L:					//�����߿�
		case LOCK_TYPE_R:					//�����߿�
			{
				//Ŀ����
				INT nSelectDrag=MousePoint.x-m_ptLockPoint.x;

				//����ж�
				if ((m_ptLockPoint.x+nSelectDrag)<rcImageArea.left) nSelectDrag=rcImageArea.left-m_ptLockPoint.x;
				if ((m_ptLockPoint.x+nSelectDrag)>rcImageArea.right) nSelectDrag=rcImageArea.right-m_ptLockPoint.x;

				//�߶��ж�
				INT nYQuotiety=(nSelectDrag>0)?1L:-1L;
				if ((m_ptLockPoint.y-nSelectDrag*nYQuotiety/2L)<rcImageArea.top) nSelectDrag=(m_ptLockPoint.y-rcImageArea.top)*nYQuotiety*2L;
				if ((m_ptLockPoint.y+nSelectDrag*nYQuotiety/2L)>rcImageArea.bottom) nSelectDrag=(rcImageArea.bottom-m_ptLockPoint.y)*nYQuotiety*2L;

				//λ������
				m_rcImageSelect.SetRect(m_ptLockPoint.x,m_ptLockPoint.y-nSelectDrag/2L,m_ptLockPoint.x+nSelectDrag,m_ptLockPoint.y+nSelectDrag/2L);

				break;
			}
		case LOCK_TYPE_T:					//�����߿�
		case LOCK_TYPE_B:					//�����߿�
			{
				//Ŀ��߶�
				INT nSelectDrag=MousePoint.y-m_ptLockPoint.y;

				//�߶��ж�
				if ((m_ptLockPoint.y+nSelectDrag)<rcImageArea.top) nSelectDrag=rcImageArea.top-m_ptLockPoint.y;
				if ((m_ptLockPoint.y+nSelectDrag)>rcImageArea.bottom) nSelectDrag=rcImageArea.bottom-m_ptLockPoint.y;

				//����ж�
				INT nXQuotiety=(nSelectDrag>0)?1L:-1L;
				if ((m_ptLockPoint.x-nSelectDrag*nXQuotiety/2L)<rcImageArea.left) nSelectDrag=(m_ptLockPoint.x-rcImageArea.left)*nXQuotiety*2L;
				if ((m_ptLockPoint.x+nSelectDrag*nXQuotiety/2L)>rcImageArea.right) nSelectDrag=(rcImageArea.right-m_ptLockPoint.x)*nXQuotiety*2L;

				//λ������
				m_rcImageSelect.SetRect(m_ptLockPoint.x-nSelectDrag/2L,m_ptLockPoint.y,m_ptLockPoint.x+nSelectDrag/2L,m_ptLockPoint.y+nSelectDrag);

				break;
			}
		case LOCK_TYPE_L|LOCK_TYPE_T:		//����ת��
		case LOCK_TYPE_R|LOCK_TYPE_T:		//����ת��
		case LOCK_TYPE_L|LOCK_TYPE_B:		//����ת��
		case LOCK_TYPE_R|LOCK_TYPE_B:		//����ת��
			{
				//Ŀ����
				INT nSelectDrag=MousePoint.x-m_ptLockPoint.x;

				//Ŀ��ϵ��
				INT nYQuotiety=1L;
				if (m_cbConsultType==(LOCK_TYPE_R|LOCK_TYPE_T)) { nYQuotiety=-1L; }
				if (m_cbConsultType==(LOCK_TYPE_L|LOCK_TYPE_B)) { nYQuotiety=-1L; }

				//����ж�
				if ((m_ptLockPoint.x+nSelectDrag)<rcImageArea.left) nSelectDrag=rcImageArea.left-m_ptLockPoint.x;
				if ((m_ptLockPoint.x+nSelectDrag)>rcImageArea.right) nSelectDrag=rcImageArea.right-m_ptLockPoint.x;

				//�߶��ж�
				if ((m_ptLockPoint.y+nSelectDrag*nYQuotiety)<rcImageArea.top) nSelectDrag=(rcImageArea.top-m_ptLockPoint.y)*nYQuotiety;
				if ((m_ptLockPoint.y+nSelectDrag*nYQuotiety)>rcImageArea.bottom) nSelectDrag=(rcImageArea.bottom-m_ptLockPoint.y)*nYQuotiety;

				//λ������
				m_rcImageSelect.SetRect(m_ptLockPoint.x,m_ptLockPoint.y,m_ptLockPoint.x+nSelectDrag,m_ptLockPoint.y+nSelectDrag*nYQuotiety);

				break;
			}
		}

		//�淶λ��
		m_rcImageSelect.NormalizeRect();

		//���½���
		if (rcImageSelect!=m_rcImageSelect)
		{
			//���½���
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

			//�¼�֪ͨ
			ASSERT(m_pIImageEditorEvent!=NULL);
			if (m_pIImageEditorEvent!=NULL) m_pIImageEditorEvent->OnEventImageSelect(true,true);
		}
	}

	//�ı�λ��
	if (m_cbOperateType==OPERATE_DRAG)
	{
		//��ȡλ��
		CRect rcClient;
		GetClientRect(&rcClient);

		//ͼƬ����
		CRect rcImageArea;
		rcImageArea.top=(rcClient.Height()-m_SizeTarget.cy)/2L;
		rcImageArea.left=(rcClient.Width()-m_SizeTarget.cx)/2L;
		rcImageArea.right=(rcClient.Width()+m_SizeTarget.cx)/2L;
		rcImageArea.bottom=(rcClient.Height()+m_SizeTarget.cy)/2L;

		//Ŀ��λ��
		INT nSelectDragCX=MousePoint.x-m_ptDragOrigin.x;
		INT nSelectDragCY=MousePoint.y-m_ptDragOrigin.y;

		//����ж�
		if ((m_rcImageSelect.left+nSelectDragCX)<rcImageArea.left) nSelectDragCX=rcImageArea.left-m_rcImageSelect.left;
		if ((m_rcImageSelect.right+nSelectDragCX)>rcImageArea.right) nSelectDragCX=rcImageArea.right-m_rcImageSelect.right;

		//�߶��ж�
		if ((m_rcImageSelect.top+nSelectDragCY)<rcImageArea.top) nSelectDragCY=rcImageArea.top-m_rcImageSelect.top;
		if ((m_rcImageSelect.bottom+nSelectDragCY)>rcImageArea.bottom) nSelectDragCY=rcImageArea.bottom-m_rcImageSelect.bottom;

		//����λ��
		if ((nSelectDragCX!=0)||(nSelectDragCY!=0))
		{
			//λ������
			m_ptDragOrigin.Offset(nSelectDragCX,nSelectDragCY);
			m_rcImageSelect.OffsetRect(nSelectDragCX,nSelectDragCY);

			//���½���
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

			//�¼�֪ͨ
			ASSERT(m_pIImageEditorEvent!=NULL);
			if (m_pIImageEditorEvent!=NULL) m_pIImageEditorEvent->OnEventImageSelect(true,true);
		}
	}

	__super::OnMouseMove(nFlags, MousePoint);
}

//�����
VOID CImageEditorControl::OnLButtonDown(UINT nFlags, CPoint MousePoint)
{
	//��겶��
	if (m_cbHitTest!=HT_NONE)
	{
		SetCapture();
	}

	//��������
	switch (m_cbHitTest)
	{
	case HT_IMAGE:					//ͼƬ����
		{
			m_cbConsultType=LOCK_NULL;
			m_cbOperateType=OPERATE_DRAG;
			m_ptDragOrigin.SetPoint(MousePoint.x,MousePoint.y);

			break;
		}
	case HT_FRAME_L:				//�߿�����
		{
			m_cbConsultType=LOCK_TYPE_R;
			m_cbOperateType=OPERATE_SIZE;
			m_ptLockPoint.SetPoint(m_rcImageSelect.right,m_rcImageSelect.top+m_rcImageSelect.Height()/2L);

			break;
		}
	case HT_FRAME_R:				//�߿�����
		{	
			m_cbConsultType=LOCK_TYPE_L;
			m_cbOperateType=OPERATE_SIZE;
			m_ptLockPoint.SetPoint(m_rcImageSelect.left,m_rcImageSelect.top+m_rcImageSelect.Height()/2L);

			break;
		}
	case HT_FRAME_T:				//�߿�����
		{
			m_cbConsultType=LOCK_TYPE_B;
			m_cbOperateType=OPERATE_SIZE;
			m_ptLockPoint.SetPoint(m_rcImageSelect.left+m_rcImageSelect.Width()/2L,m_rcImageSelect.bottom);

			break;
		}
	case HT_FRAME_B:				//�߿�����
		{
			m_cbConsultType=LOCK_TYPE_T;
			m_cbOperateType=OPERATE_SIZE;
			m_ptLockPoint.SetPoint(m_rcImageSelect.left+m_rcImageSelect.Width()/2L,m_rcImageSelect.top);

			break;
		}
	case HT_FRAME_R|HT_FRAME_T:		//ת������
		{
			m_cbOperateType=OPERATE_SIZE;
			m_cbConsultType=LOCK_TYPE_L|LOCK_TYPE_B;
			m_ptLockPoint.SetPoint(m_rcImageSelect.left,m_rcImageSelect.bottom);

			break;
		}
	case HT_FRAME_R|HT_FRAME_B:		//ת������
		{
			m_cbOperateType=OPERATE_SIZE;
			m_cbConsultType=LOCK_TYPE_L|HT_FRAME_T;
			m_ptLockPoint.SetPoint(m_rcImageSelect.left,m_rcImageSelect.top);

			break;
		}
	case HT_FRAME_L|HT_FRAME_T:		//ת������
		{
			m_cbOperateType=OPERATE_SIZE;
			m_cbConsultType=LOCK_TYPE_R|HT_FRAME_B;
			m_ptLockPoint.SetPoint(m_rcImageSelect.right,m_rcImageSelect.bottom);

			break;
		}
	case HT_FRAME_L|HT_FRAME_B:		//ת������
		{
			m_cbOperateType=OPERATE_SIZE;
			m_cbConsultType=LOCK_TYPE_R|HT_FRAME_T;
			m_ptLockPoint.SetPoint(m_rcImageSelect.right,m_rcImageSelect.top);

			break;
		}
	}

	__super::OnLButtonDown(nFlags, MousePoint);
}

//��굯��
VOID CImageEditorControl::OnLButtonUp(UINT nFlags, CPoint MousePoint)
{
	//�¼�����
	if (m_cbOperateType!=OPERATE_NULL)
	{
		//�ͷŲ���
		ReleaseCapture();

		//���ñ���
		m_cbConsultType=LOCK_NULL;
		m_cbOperateType=OPERATE_NULL;

		//�¼�֪ͨ
		ASSERT(m_pIImageEditorEvent!=NULL);
		if (m_pIImageEditorEvent!=NULL) m_pIImageEditorEvent->OnEventImageSelect(true,false);
	}

	__super::OnLButtonUp(nFlags, MousePoint);
}

//���ù��
BOOL CImageEditorControl::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT nMessage)
{
	//����״̬
	if (m_cbOperateType==OPERATE_NULL)
	{
		//��ȡ���
		CPoint MousePoint;
		GetCursorPos(&MousePoint);
		ScreenToClient(&MousePoint);

		//��������
		m_cbHitTest=OnEventHitTest(MousePoint);

		//���ù��
		switch (m_cbHitTest)
		{
		case HT_IMAGE:					//ͼƬ����
			{
				SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZEALL)));
				return TRUE;
			}
		case HT_FRAME_L:				//�߿�����
		case HT_FRAME_R:				//�߿�����
			{
				SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZEWE)));
				return TRUE;
			}
		case HT_FRAME_T:				//�߿�����
		case HT_FRAME_B:				//�߿�����
			{
				SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZENS)));
				return TRUE;
			}
		case HT_FRAME_R|HT_FRAME_T:		//ת������
		case HT_FRAME_L|HT_FRAME_B:		//ת������
			{
				SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZENESW)));
				return TRUE;
			}
		case HT_FRAME_L|HT_FRAME_T:		//ת������
		case HT_FRAME_R|HT_FRAME_B:		//ת������
			{
				SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZENWSE)));
				return TRUE;
			}
		}
	}

	return __super::OnSetCursor(pWnd,nHitTest,nMessage);
}

//////////////////////////////////////////////////////////////////////////////////
