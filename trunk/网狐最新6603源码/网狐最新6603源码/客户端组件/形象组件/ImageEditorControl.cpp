#include "Stdafx.h"
#include "ImageEditorControl.h"

//////////////////////////////////////////////////////////////////////////////////

//鼠标测试
#define HT_NONE						0x00								//无效区域
#define HT_IMAGE					0x10								//图片区域
#define HT_FRAME_L					0x01								//左面边框
#define HT_FRAME_R					0x02								//右面边框
#define HT_FRAME_T					0x04								//上面边框
#define HT_FRAME_B					0x08								//下面边框

//锁定方式
#define LOCK_NULL					0x00								//无效类型
#define LOCK_TYPE_L					0x01								//左面边框
#define LOCK_TYPE_R					0x02								//右面边框
#define LOCK_TYPE_T					0x04								//上面边框
#define LOCK_TYPE_B					0x08								//下面边框

//操作类型
#define OPERATE_NULL				0x00								//无效类型
#define OPERATE_DRAG				0x01								//鼠标拖拽
#define OPERATE_SIZE				0x02								//改变尺寸

//常量定义
#define COLOR_CONTROL_FRMAE			RGB(10,124,202)						//控制颜色

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CImageEditorControl, CWnd)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CImageEditorControl::CImageEditorControl()
{
	//状态变量
	m_bVShowLine=true;
	m_cbConsultType=LOCK_NULL;
	m_cbOperateType=OPERATE_NULL;
	m_rcImageSelect.SetRectEmpty();

	//操作变量
	m_cbHitTest=HT_NONE;
	m_ptLockPoint.SetPoint(0,0);
	m_ptDragOrigin.SetPoint(0,0);

	//状态变量
	m_dZoomPercent=1.0;
	m_SizeSource.SetSize(0,0);
	m_SizeTarget.SetSize(0,0);

	//接口变量
	m_pIImageEditorEvent=NULL;

	return;
}

//析构函数
CImageEditorControl::~CImageEditorControl()
{
	//清理资源
	m_ImageSource.Destroy();

	return;
}

//重置图片
bool CImageEditorControl::DestoryImage()
{
	//清理资源
	m_ImageSource.Destroy();

	//状态变量
	m_cbConsultType=LOCK_NULL;
	m_cbOperateType=OPERATE_NULL;
	m_rcImageSelect.SetRectEmpty();

	//操作变量
	m_cbHitTest=HT_NONE;
	m_ptLockPoint.SetPoint(0,0);
	m_ptDragOrigin.SetPoint(0,0);

	//状态变量
	m_dZoomPercent=1.0;
	m_SizeSource.SetSize(0,0);
	m_SizeTarget.SetSize(0,0);

	//更新界面
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

	return true;
}


//加载图片
bool CImageEditorControl::LoadEditImage(CImage & Image)
{
	//状态效验
	ASSERT(Image.IsNull()==false);
	if (Image.IsNull()==true) return false;

	//清理资源
	m_ImageSource.Destroy();

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//加载图片
	m_ImageSource.Attach(Image.Detach());

	//缩放比例
	if (((DOUBLE)rcClient.Width()/(DOUBLE)rcClient.Height())<((DOUBLE)m_ImageSource.GetWidth()/(DOUBLE)m_ImageSource.GetHeight()))
	{
		m_dZoomPercent=(DOUBLE)rcClient.Width()/(DOUBLE)m_ImageSource.GetWidth();
	}
	else
	{
		m_dZoomPercent=(DOUBLE)rcClient.Height()/(DOUBLE)m_ImageSource.GetHeight();
	}

	//设置大小
	m_SizeSource.SetSize(m_ImageSource.GetWidth(),m_ImageSource.GetHeight());
	m_SizeTarget.SetSize((INT)(m_SizeSource.cx*m_dZoomPercent),(INT)(m_SizeSource.cy*m_dZoomPercent));

	//选择区域
	INT nSelectArea=__min(m_SizeTarget.cx,m_SizeTarget.cy)*4/5L;
	m_rcImageSelect.SetRect((rcClient.Width()-nSelectArea)/2,(rcClient.Height()-nSelectArea)/2,(rcClient.Width()+nSelectArea)/2,(rcClient.Height()+nSelectArea)/2);

	//更新界面
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

	//事件通知
	ASSERT(m_pIImageEditorEvent!=NULL);
	if (m_pIImageEditorEvent!=NULL) m_pIImageEditorEvent->OnEventImageSelect(true,false);

	return true;
}

//加载图片
bool CImageEditorControl::LoadEditImage(LPCTSTR pszImage)
{
	//加载图片
	CImage ImageSource;
	if (ImageSource.Load(pszImage)!=S_OK)
	{
		//错误断言
		ASSERT(FALSE);

		//事件通知
		ASSERT(m_pIImageEditorEvent!=NULL);
		if (m_pIImageEditorEvent!=NULL) m_pIImageEditorEvent->OnEventImageSelect(false,false);

		return false;
	}

	//更新图片
	if (LoadEditImage(ImageSource)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//旋转图片
VOID CImageEditorControl::SetRoundImage(BYTE cbRoundType)
{
	//状态效验
	ASSERT(m_ImageSource.IsNull()==false);
	if (m_ImageSource.IsNull()==true) return;

	//构造位置
	CSize SizeSource(m_ImageSource.GetWidth(),m_ImageSource.GetHeight());
	CSize SizeResult(m_ImageSource.GetHeight(),m_ImageSource.GetWidth());

	//创建位图
	CImage ImageSource;
	CImage ImageResult;
	ImageSource.Create(m_ImageSource.GetWidth(),m_ImageSource.GetHeight(),32);
	ImageResult.Create(m_ImageSource.GetHeight(),m_ImageSource.GetWidth(),32);

	//绘画位图
	CDC * pDCResult=CDC::FromHandle(ImageResult.GetDC());
	CDC * pDCSource=CDC::FromHandle(ImageSource.GetDC());
	m_ImageSource.BitBlt(pDCSource->m_hDC,0,0,m_ImageSource.GetWidth(),m_ImageSource.GetHeight(),0,0);

	//获取属性
	INT nPitchResult=ImageResult.GetPitch();
	INT nPitchSource=ImageSource.GetPitch();

	//获取数据
	LPBYTE cbBitResult=(LPBYTE)ImageResult.GetBits();
	LPBYTE cbBitSource=(LPBYTE)ImageSource.GetBits();

	//创建区域
	for (INT nYSourcePos=0;nYSourcePos<SizeSource.cy;nYSourcePos++)
	{
		for (INT nXSourcePos=0;nXSourcePos<SizeSource.cx;nXSourcePos++)
		{
			//目标位置
			INT nXResultPos=(cbRoundType==ROUND_TYPE_LEFT)?nYSourcePos:(SizeSource.cy-nYSourcePos-1);
			INT nYResultPos=(cbRoundType==ROUND_TYPE_LEFT)?(SizeSource.cx-nXSourcePos-1):nXSourcePos;

			//设置颜色
			DWORD dwSource=nYSourcePos*nPitchSource+nXSourcePos*4L;
			DWORD dwResult=nYResultPos*nPitchResult+nXResultPos*4L;
			*(COLORREF *)(cbBitResult+dwResult)=*(COLORREF *)(cbBitSource+dwSource);
		}
	}

	//释放对象
	ImageSource.ReleaseDC();
	ImageResult.ReleaseDC();

	//更新图片
	if (LoadEditImage(ImageResult)==false)
	{
		ASSERT(FALSE);
		return;
	}

	return;
}

//加载判断
bool CImageEditorControl::IsNull()
{
	return m_ImageSource.IsNull();
}

//绘画图片
VOID CImageEditorControl::DrawEditImage(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight)
{
	//状态效验
	ASSERT(m_ImageSource.IsNull()==false);
	if (m_ImageSource.IsNull()==true) return;

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//设置环境
	pDC->SetStretchBltMode(HALFTONE);

	//绘画图片
	m_ImageSource.StretchBlt(pDC->m_hDC,nXPos,nYPos,nWidth,nHeight,
		m_SizeSource.cx*(m_rcImageSelect.left-(rcClient.Width()-m_SizeTarget.cx)/2)/m_SizeTarget.cx,
		m_SizeSource.cy*(m_rcImageSelect.top-(rcClient.Height()-m_SizeTarget.cy)/2)/m_SizeTarget.cy,
		m_SizeSource.cx*m_rcImageSelect.Width()/m_SizeTarget.cx,m_SizeSource.cy*m_rcImageSelect.Height()/m_SizeTarget.cy);

	return;
}

//设置虚线
VOID CImageEditorControl::SetVLineStatus(bool bShowVLine)
{
	if (bShowVLine!=m_bVShowLine)
	{
		//设置变量
		m_bVShowLine=bShowVLine;

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);
	}

	return;
}

//绘画函数
VOID CImageEditorControl::DrawLineFrame(CDC * pDC)
{
	//变量定义
	INT nXSpace=m_rcImageSelect.Width()/3;
	INT nYSpace=m_rcImageSelect.Height()/3;

	//绘画边框
	pDC->Draw3dRect(&m_rcImageSelect,COLOR_CONTROL_FRMAE,COLOR_CONTROL_FRMAE);

	//绘画控制
	pDC->FillSolidRect(m_rcImageSelect.left-2,m_rcImageSelect.top-2,4,4,COLOR_CONTROL_FRMAE);
	pDC->FillSolidRect(m_rcImageSelect.right-1,m_rcImageSelect.top-2,4,4,COLOR_CONTROL_FRMAE);
	pDC->FillSolidRect(m_rcImageSelect.left+m_rcImageSelect.Width()/2-1,m_rcImageSelect.top-2,4,4,COLOR_CONTROL_FRMAE);

	//绘画控制
	pDC->FillSolidRect(m_rcImageSelect.left-2,m_rcImageSelect.bottom-2,4,4,COLOR_CONTROL_FRMAE);
	pDC->FillSolidRect(m_rcImageSelect.right-1,m_rcImageSelect.bottom-2,4,4,COLOR_CONTROL_FRMAE);
	pDC->FillSolidRect(m_rcImageSelect.left+m_rcImageSelect.Width()/2-1,m_rcImageSelect.bottom-2,4,4,COLOR_CONTROL_FRMAE);

	//绘画控制
	pDC->FillSolidRect(m_rcImageSelect.left-2,m_rcImageSelect.top+m_rcImageSelect.Height()/2,4,4,COLOR_CONTROL_FRMAE);
	pDC->FillSolidRect(m_rcImageSelect.right-2,m_rcImageSelect.top+m_rcImageSelect.Height()/2,4,4,COLOR_CONTROL_FRMAE);

	//绘画虚线
	if (m_bVShowLine==true)
	{
		//创建画笔
		CPen PenDot(PS_DOT,1,RGB(117,130,161));
		CPen * pOldPen=pDC->SelectObject(&PenDot);

		//设置 DC
		pDC->SetBkMode(TRANSPARENT);

		//绘画虚线
		pDC->MoveTo(m_rcImageSelect.left+nXSpace,m_rcImageSelect.top);
		pDC->LineTo(m_rcImageSelect.left+nXSpace,m_rcImageSelect.bottom);

		//绘画虚线
		pDC->MoveTo(m_rcImageSelect.left+nXSpace*2,m_rcImageSelect.top);
		pDC->LineTo(m_rcImageSelect.left+nXSpace*2,m_rcImageSelect.bottom);

		//绘画虚线
		pDC->MoveTo(m_rcImageSelect.left,m_rcImageSelect.top+nYSpace);
		pDC->LineTo(m_rcImageSelect.right,m_rcImageSelect.top+nYSpace);

		//绘画虚线
		pDC->MoveTo(m_rcImageSelect.left,m_rcImageSelect.top+nYSpace*2);
		pDC->LineTo(m_rcImageSelect.right,m_rcImageSelect.top+nYSpace*2);

		//设置 DC
		pDC->SelectObject(pOldPen);
	}

	return;
}

//点击测试
BYTE CImageEditorControl::OnEventHitTest(CPoint & MousePoint)
{
	//变量定义
	BYTE cbHitTest=HT_NONE;

	//区域检测
	if (m_rcImageSelect.PtInRect(MousePoint)==TRUE)
	{
		//竖轴测试
		if (MousePoint.y<=m_rcImageSelect.top+3)
		{
			cbHitTest|=HT_FRAME_T;
		}

		//竖轴测试
		if (MousePoint.y>=m_rcImageSelect.bottom-3)
		{
			cbHitTest|=HT_FRAME_B;
		}

		//横轴测试
		if(MousePoint.x<=m_rcImageSelect.left+3)
		{
			cbHitTest|=HT_FRAME_L;
		}

		//横轴测试
		if (MousePoint.x>=m_rcImageSelect.right-3)
		{
			cbHitTest|=HT_FRAME_R;
		}

		//图片区域
		if (cbHitTest==0) cbHitTest=HT_IMAGE;
	}

	return cbHitTest;
}

//绘制背景
BOOL CImageEditorControl::OnEraseBkgnd(CDC * pDC)
{
	//获取大小
	CRect rcClient;
	GetClientRect(&rcClient);

	//创建缓冲
	CImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//变量定义
	HDC hBufferDC=ImageBuffer.GetDC();
	CDC * pBufferDC=CDC::FromHandle(hBufferDC);

	//绘画背景
	pBufferDC->FillSolidRect(&rcClient,RGB(255,255,255));

	//绘画图像
	if (m_ImageSource.IsNull()==false)
	{
		//加载图片
		CPngImage ImageEditGray;
		ImageEditGray.LoadImage(GetModuleHandle(AVATAR_CONTROL_DLL_NAME),TEXT("FACE_EDIT_GRAY"));

		//设置环境
		pBufferDC->SetStretchBltMode(HALFTONE);

		//绘画图片
		m_ImageSource.StretchBlt(hBufferDC,(rcClient.Width()-m_SizeTarget.cx)/2,(rcClient.Height()-m_SizeTarget.cy)/2,
			m_SizeTarget.cx,m_SizeTarget.cy,0,0,m_SizeSource.cx,m_SizeSource.cy);

		//绘画蒙图
		ImageEditGray.DrawImage(pBufferDC,(rcClient.Width()-m_SizeTarget.cx)/2,(rcClient.Height()-m_SizeTarget.cy)/2,
			m_SizeTarget.cx,m_SizeTarget.cy,0,0,ImageEditGray.GetWidth(),ImageEditGray.GetHeight());

		//绘画图片
		m_ImageSource.StretchBlt(hBufferDC,m_rcImageSelect.left,m_rcImageSelect.top,m_rcImageSelect.Width(),m_rcImageSelect.Height(),
			m_SizeSource.cx*(m_rcImageSelect.left-(rcClient.Width()-m_SizeTarget.cx)/2)/m_SizeTarget.cx,
			m_SizeSource.cy*(m_rcImageSelect.top-(rcClient.Height()-m_SizeTarget.cy)/2)/m_SizeTarget.cy,
			m_SizeSource.cx*m_rcImageSelect.Width()/m_SizeTarget.cx,m_SizeSource.cy*m_rcImageSelect.Height()/m_SizeTarget.cy);

		//绘画框架
		DrawLineFrame(pBufferDC);
	}

	//绘画界面
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	//清理资源
	ImageBuffer.ReleaseDC();

	return TRUE;
}

//鼠标移动
VOID CImageEditorControl::OnMouseMove(UINT nFlags, CPoint MousePoint)
{
	//改变大小
	if (m_cbOperateType==OPERATE_SIZE)
	{
		//获取位置
		CRect rcClient;
		GetClientRect(&rcClient);

		//变量定义
		CRect rcImageSelect=m_rcImageSelect;

		//图片区域
		CRect rcImageArea;
		rcImageArea.top=(rcClient.Height()-m_SizeTarget.cy)/2L;
		rcImageArea.left=(rcClient.Width()-m_SizeTarget.cx)/2L;
		rcImageArea.right=(rcClient.Width()+m_SizeTarget.cx)/2L;
		rcImageArea.bottom=(rcClient.Height()+m_SizeTarget.cy)/2L;

		//事件处理
		switch (m_cbConsultType)
		{
		case LOCK_TYPE_L:					//锁定边框
		case LOCK_TYPE_R:					//锁定边框
			{
				//目标宽度
				INT nSelectDrag=MousePoint.x-m_ptLockPoint.x;

				//宽度判断
				if ((m_ptLockPoint.x+nSelectDrag)<rcImageArea.left) nSelectDrag=rcImageArea.left-m_ptLockPoint.x;
				if ((m_ptLockPoint.x+nSelectDrag)>rcImageArea.right) nSelectDrag=rcImageArea.right-m_ptLockPoint.x;

				//高度判断
				INT nYQuotiety=(nSelectDrag>0)?1L:-1L;
				if ((m_ptLockPoint.y-nSelectDrag*nYQuotiety/2L)<rcImageArea.top) nSelectDrag=(m_ptLockPoint.y-rcImageArea.top)*nYQuotiety*2L;
				if ((m_ptLockPoint.y+nSelectDrag*nYQuotiety/2L)>rcImageArea.bottom) nSelectDrag=(rcImageArea.bottom-m_ptLockPoint.y)*nYQuotiety*2L;

				//位置设置
				m_rcImageSelect.SetRect(m_ptLockPoint.x,m_ptLockPoint.y-nSelectDrag/2L,m_ptLockPoint.x+nSelectDrag,m_ptLockPoint.y+nSelectDrag/2L);

				break;
			}
		case LOCK_TYPE_T:					//锁定边框
		case LOCK_TYPE_B:					//锁定边框
			{
				//目标高度
				INT nSelectDrag=MousePoint.y-m_ptLockPoint.y;

				//高度判断
				if ((m_ptLockPoint.y+nSelectDrag)<rcImageArea.top) nSelectDrag=rcImageArea.top-m_ptLockPoint.y;
				if ((m_ptLockPoint.y+nSelectDrag)>rcImageArea.bottom) nSelectDrag=rcImageArea.bottom-m_ptLockPoint.y;

				//宽度判断
				INT nXQuotiety=(nSelectDrag>0)?1L:-1L;
				if ((m_ptLockPoint.x-nSelectDrag*nXQuotiety/2L)<rcImageArea.left) nSelectDrag=(m_ptLockPoint.x-rcImageArea.left)*nXQuotiety*2L;
				if ((m_ptLockPoint.x+nSelectDrag*nXQuotiety/2L)>rcImageArea.right) nSelectDrag=(rcImageArea.right-m_ptLockPoint.x)*nXQuotiety*2L;

				//位置设置
				m_rcImageSelect.SetRect(m_ptLockPoint.x-nSelectDrag/2L,m_ptLockPoint.y,m_ptLockPoint.x+nSelectDrag/2L,m_ptLockPoint.y+nSelectDrag);

				break;
			}
		case LOCK_TYPE_L|LOCK_TYPE_T:		//锁定转角
		case LOCK_TYPE_R|LOCK_TYPE_T:		//锁定转角
		case LOCK_TYPE_L|LOCK_TYPE_B:		//锁定转角
		case LOCK_TYPE_R|LOCK_TYPE_B:		//锁定转角
			{
				//目标宽度
				INT nSelectDrag=MousePoint.x-m_ptLockPoint.x;

				//目标系数
				INT nYQuotiety=1L;
				if (m_cbConsultType==(LOCK_TYPE_R|LOCK_TYPE_T)) { nYQuotiety=-1L; }
				if (m_cbConsultType==(LOCK_TYPE_L|LOCK_TYPE_B)) { nYQuotiety=-1L; }

				//宽度判断
				if ((m_ptLockPoint.x+nSelectDrag)<rcImageArea.left) nSelectDrag=rcImageArea.left-m_ptLockPoint.x;
				if ((m_ptLockPoint.x+nSelectDrag)>rcImageArea.right) nSelectDrag=rcImageArea.right-m_ptLockPoint.x;

				//高度判断
				if ((m_ptLockPoint.y+nSelectDrag*nYQuotiety)<rcImageArea.top) nSelectDrag=(rcImageArea.top-m_ptLockPoint.y)*nYQuotiety;
				if ((m_ptLockPoint.y+nSelectDrag*nYQuotiety)>rcImageArea.bottom) nSelectDrag=(rcImageArea.bottom-m_ptLockPoint.y)*nYQuotiety;

				//位置设置
				m_rcImageSelect.SetRect(m_ptLockPoint.x,m_ptLockPoint.y,m_ptLockPoint.x+nSelectDrag,m_ptLockPoint.y+nSelectDrag*nYQuotiety);

				break;
			}
		}

		//规范位置
		m_rcImageSelect.NormalizeRect();

		//更新界面
		if (rcImageSelect!=m_rcImageSelect)
		{
			//更新界面
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

			//事件通知
			ASSERT(m_pIImageEditorEvent!=NULL);
			if (m_pIImageEditorEvent!=NULL) m_pIImageEditorEvent->OnEventImageSelect(true,true);
		}
	}

	//改变位置
	if (m_cbOperateType==OPERATE_DRAG)
	{
		//获取位置
		CRect rcClient;
		GetClientRect(&rcClient);

		//图片区域
		CRect rcImageArea;
		rcImageArea.top=(rcClient.Height()-m_SizeTarget.cy)/2L;
		rcImageArea.left=(rcClient.Width()-m_SizeTarget.cx)/2L;
		rcImageArea.right=(rcClient.Width()+m_SizeTarget.cx)/2L;
		rcImageArea.bottom=(rcClient.Height()+m_SizeTarget.cy)/2L;

		//目标位移
		INT nSelectDragCX=MousePoint.x-m_ptDragOrigin.x;
		INT nSelectDragCY=MousePoint.y-m_ptDragOrigin.y;

		//宽度判断
		if ((m_rcImageSelect.left+nSelectDragCX)<rcImageArea.left) nSelectDragCX=rcImageArea.left-m_rcImageSelect.left;
		if ((m_rcImageSelect.right+nSelectDragCX)>rcImageArea.right) nSelectDragCX=rcImageArea.right-m_rcImageSelect.right;

		//高度判断
		if ((m_rcImageSelect.top+nSelectDragCY)<rcImageArea.top) nSelectDragCY=rcImageArea.top-m_rcImageSelect.top;
		if ((m_rcImageSelect.bottom+nSelectDragCY)>rcImageArea.bottom) nSelectDragCY=rcImageArea.bottom-m_rcImageSelect.bottom;

		//设置位置
		if ((nSelectDragCX!=0)||(nSelectDragCY!=0))
		{
			//位置设置
			m_ptDragOrigin.Offset(nSelectDragCX,nSelectDragCY);
			m_rcImageSelect.OffsetRect(nSelectDragCX,nSelectDragCY);

			//更新界面
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

			//事件通知
			ASSERT(m_pIImageEditorEvent!=NULL);
			if (m_pIImageEditorEvent!=NULL) m_pIImageEditorEvent->OnEventImageSelect(true,true);
		}
	}

	__super::OnMouseMove(nFlags, MousePoint);
}

//鼠标点击
VOID CImageEditorControl::OnLButtonDown(UINT nFlags, CPoint MousePoint)
{
	//鼠标捕获
	if (m_cbHitTest!=HT_NONE)
	{
		SetCapture();
	}

	//操作设置
	switch (m_cbHitTest)
	{
	case HT_IMAGE:					//图片区域
		{
			m_cbConsultType=LOCK_NULL;
			m_cbOperateType=OPERATE_DRAG;
			m_ptDragOrigin.SetPoint(MousePoint.x,MousePoint.y);

			break;
		}
	case HT_FRAME_L:				//边框区域
		{
			m_cbConsultType=LOCK_TYPE_R;
			m_cbOperateType=OPERATE_SIZE;
			m_ptLockPoint.SetPoint(m_rcImageSelect.right,m_rcImageSelect.top+m_rcImageSelect.Height()/2L);

			break;
		}
	case HT_FRAME_R:				//边框区域
		{	
			m_cbConsultType=LOCK_TYPE_L;
			m_cbOperateType=OPERATE_SIZE;
			m_ptLockPoint.SetPoint(m_rcImageSelect.left,m_rcImageSelect.top+m_rcImageSelect.Height()/2L);

			break;
		}
	case HT_FRAME_T:				//边框区域
		{
			m_cbConsultType=LOCK_TYPE_B;
			m_cbOperateType=OPERATE_SIZE;
			m_ptLockPoint.SetPoint(m_rcImageSelect.left+m_rcImageSelect.Width()/2L,m_rcImageSelect.bottom);

			break;
		}
	case HT_FRAME_B:				//边框区域
		{
			m_cbConsultType=LOCK_TYPE_T;
			m_cbOperateType=OPERATE_SIZE;
			m_ptLockPoint.SetPoint(m_rcImageSelect.left+m_rcImageSelect.Width()/2L,m_rcImageSelect.top);

			break;
		}
	case HT_FRAME_R|HT_FRAME_T:		//转角区域
		{
			m_cbOperateType=OPERATE_SIZE;
			m_cbConsultType=LOCK_TYPE_L|LOCK_TYPE_B;
			m_ptLockPoint.SetPoint(m_rcImageSelect.left,m_rcImageSelect.bottom);

			break;
		}
	case HT_FRAME_R|HT_FRAME_B:		//转角区域
		{
			m_cbOperateType=OPERATE_SIZE;
			m_cbConsultType=LOCK_TYPE_L|HT_FRAME_T;
			m_ptLockPoint.SetPoint(m_rcImageSelect.left,m_rcImageSelect.top);

			break;
		}
	case HT_FRAME_L|HT_FRAME_T:		//转角区域
		{
			m_cbOperateType=OPERATE_SIZE;
			m_cbConsultType=LOCK_TYPE_R|HT_FRAME_B;
			m_ptLockPoint.SetPoint(m_rcImageSelect.right,m_rcImageSelect.bottom);

			break;
		}
	case HT_FRAME_L|HT_FRAME_B:		//转角区域
		{
			m_cbOperateType=OPERATE_SIZE;
			m_cbConsultType=LOCK_TYPE_R|HT_FRAME_T;
			m_ptLockPoint.SetPoint(m_rcImageSelect.right,m_rcImageSelect.top);

			break;
		}
	}

	__super::OnLButtonDown(nFlags, MousePoint);
}

//鼠标弹起
VOID CImageEditorControl::OnLButtonUp(UINT nFlags, CPoint MousePoint)
{
	//事件处理
	if (m_cbOperateType!=OPERATE_NULL)
	{
		//释放捕获
		ReleaseCapture();

		//设置变量
		m_cbConsultType=LOCK_NULL;
		m_cbOperateType=OPERATE_NULL;

		//事件通知
		ASSERT(m_pIImageEditorEvent!=NULL);
		if (m_pIImageEditorEvent!=NULL) m_pIImageEditorEvent->OnEventImageSelect(true,false);
	}

	__super::OnLButtonUp(nFlags, MousePoint);
}

//设置光标
BOOL CImageEditorControl::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT nMessage)
{
	//空闲状态
	if (m_cbOperateType==OPERATE_NULL)
	{
		//获取光标
		CPoint MousePoint;
		GetCursorPos(&MousePoint);
		ScreenToClient(&MousePoint);

		//按键测试
		m_cbHitTest=OnEventHitTest(MousePoint);

		//设置光标
		switch (m_cbHitTest)
		{
		case HT_IMAGE:					//图片区域
			{
				SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZEALL)));
				return TRUE;
			}
		case HT_FRAME_L:				//边框区域
		case HT_FRAME_R:				//边框区域
			{
				SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZEWE)));
				return TRUE;
			}
		case HT_FRAME_T:				//边框区域
		case HT_FRAME_B:				//边框区域
			{
				SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZENS)));
				return TRUE;
			}
		case HT_FRAME_R|HT_FRAME_T:		//转角区域
		case HT_FRAME_L|HT_FRAME_B:		//转角区域
			{
				SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZENESW)));
				return TRUE;
			}
		case HT_FRAME_L|HT_FRAME_T:		//转角区域
		case HT_FRAME_R|HT_FRAME_B:		//转角区域
			{
				SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZENWSE)));
				return TRUE;
			}
		}
	}

	return __super::OnSetCursor(pWnd,nHitTest,nMessage);
}

//////////////////////////////////////////////////////////////////////////////////
