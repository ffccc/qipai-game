#include "StdAfx.h"
#include "Resource.h"
#include "SkinDialog.h"
#include "SkinComboBox.h"
#include "SkinRenderManager.h"

//////////////////////////////////////////////////////////////////////////////////

//时间标识
#define IDI_CHECK_HOVER				10									//过渡标识
#define TIME_CHECK_HOVER			10									//过渡时间

//////////////////////////////////////////////////////////////////////////////////

//静态变量
CSkinComboBoxAttribute			CSkinComboBox::m_SkinAttribute;			//按钮属性

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSkinComboBoxList, CListBox)
	ON_WM_NCPAINT()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CSkinComboBox, CComboBox)
	ON_WM_SIZE()
    ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCbnCloseup)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CSkinComboBoxAttribute::CSkinComboBoxAttribute()
{
	//设置变量
	m_uButtonWidth=0;
	m_crEnableBack=RGB(0,0,0);
	m_crDisableBack=RGB(0,0,0);
	m_crEnableBorad=RGB(0,0,0);
	m_crDisableBorad=RGB(0,0,0);

	return;
}

//析构函数
CSkinComboBoxAttribute::~CSkinComboBoxAttribute() 
{
}

//配置资源
bool CSkinComboBoxAttribute::Initialization(CSkinRenderManager * pSkinRenderManager)
{
	//设置变量
	m_uButtonWidth=16;
	m_crEnableBack=RGB(255,255,255);
	m_crEnableBorad=RGB(92,100,105);
	m_crDisableBack=RGB(125,125,125);
	m_crDisableBorad=RGB(92,100,105);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CSkinComboBoxList::CSkinComboBoxList()
{
}

//析构函数
CSkinComboBoxList::~CSkinComboBoxList()
{
}

//绘画子项
VOID CSkinComboBoxList::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//变量定义
	CRect rcItem=lpDrawItemStruct->rcItem;
	CDC * pDCControl=CDC::FromHandle(lpDrawItemStruct->hDC);

	//创建缓冲
	CDC BufferDC;
	CBitmap ImageBuffer;
	BufferDC.CreateCompatibleDC(pDCControl);
	ImageBuffer.CreateCompatibleBitmap(pDCControl,rcItem.Width(),rcItem.Height());

	//设置环境
	BufferDC.SelectObject(&ImageBuffer);
	BufferDC.SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//获取字符
	CString strString;
	GetText(lpDrawItemStruct->itemID,strString);

	//计算位置
	CRect rcString;
	rcString.SetRect(4,0,rcItem.Width()-8,rcItem.Height());

	//颜色定义
	COLORREF crTextColor=((lpDrawItemStruct->itemState&ODS_SELECTED)!=0)?RGB(255,255,255):RGB(0,0,0);
	COLORREF crBackColor=((lpDrawItemStruct->itemState&ODS_SELECTED)!=0)?RGB(10,36,106):RGB(255,255,255);

	//绘画背景
	BufferDC.FillSolidRect(0,0,rcItem.Width(),rcItem.Height(),crBackColor);

	//绘画字符
	BufferDC.SetBkMode(TRANSPARENT);
	BufferDC.SetTextColor(crTextColor);
	BufferDC.DrawText(strString,&rcString,DT_VCENTER|DT_SINGLELINE);

	//绘画界面
	pDCControl->BitBlt(rcItem.left,rcItem.top,rcItem.Width(),rcItem.Height(),&BufferDC,0,0,SRCCOPY);

	//清理资源
	BufferDC.DeleteDC();
	ImageBuffer.DeleteObject();

	return;
}

//界面绘画
VOID CSkinComboBoxList::OnNcPaint()
{
	__super::OnNcPaint();

	//获取位置
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//绘画边框
	CWindowDC WindowDC(this);
	COLORREF crBoradFrame=CSkinComboBox::m_SkinAttribute.m_crEnableBorad;
	WindowDC.Draw3dRect(0,0,rcWindow.Width(),rcWindow.Height(),crBoradFrame,crBoradFrame);

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CSkinComboBox::CSkinComboBox()
{
	//控制变量
	m_bHovering=false;
	m_bDrawBorad=true;
	m_bRenderImage=false;
	m_uButtonWidth=m_SkinAttribute.m_uButtonWidth;

	//资源变量
	m_uResourceID=0;
	m_hResInstance=NULL;

	return;
}

//析构函数
CSkinComboBox::~CSkinComboBox()
{
}

//控件绑定
VOID CSkinComboBox::PreSubclassWindow()
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//配置控件
	Initialization();
	RectifyControl(rcClient.Width(),rcClient.Height());

	return;
}

//测量子项
VOID CSkinComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	//设置变量
	lpMeasureItemStruct->itemWidth=0;
	lpMeasureItemStruct->itemHeight=GetItemHeight(LB_ERR);

	return;
}

//绘画控件
VOID CSkinComboBox::DrawControlView(CDC * pDC)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//状态变量
	DWORD dwStyle=GetStyle();
	bool bDisable=(IsWindowEnabled()==FALSE);
	bool bDropped=(GetDroppedState()!=FALSE);

	//绘画背景
	if (((GetStyle()&0x000F)==CBS_DROPDOWN)||(GetCurSel()==LB_ERR))
	{
		if (bDisable==false)
		{
			COLORREF crColor=m_SkinAttribute.m_crEnableBack;
			pDC->FillSolidRect(1,1,rcClient.Width()-m_uButtonWidth-2,rcClient.Height()-2,crColor);
		}
		else
		{
			COLORREF crColor=CSkinDialog::m_SkinAttribute.m_crBackGround;
			pDC->FillSolidRect(1,1,rcClient.Width()-m_uButtonWidth-2,rcClient.Height()-2,crColor);
		}
	}

	//绘画背景
	if ((GetStyle()&0x000F)==CBS_DROPDOWNLIST)
	{
		//设置颜色
		if (GetFocus()->GetSafeHwnd()==m_hWnd)
		{
			COLORREF crColor=m_SkinAttribute.m_crEnableBack;
			pDC->FillSolidRect(0,0,rcClient.Width()-m_uButtonWidth,rcClient.Height()-2,crColor);
		}
		else
		{
			COLORREF crColor=m_SkinAttribute.m_crEnableBack;
			pDC->FillSolidRect(0,0,rcClient.Width(),rcClient.Height(),crColor);
		}
	}

	//绘画字符
	if ((GetStyle()&0x000F)==CBS_DROPDOWNLIST)
	{
		//获取字符
		CString strString;
		GetWindowText(strString);

		//获取字符
		CRect rcText;
		GetClientRect(&rcText);

		//设置环境
		pDC->SetBkMode(TRANSPARENT);
		pDC->SelectObject(GetFont());

		//绘画字符
		rcText.left+=4;
		pDC->DrawText(strString,&rcText,DT_VCENTER|DT_SINGLELINE);
	}

	//绘画边框
	if (m_bDrawBorad==true)
	{
		if (bDisable==false)
		{
			if (m_bRenderImage==true)
			{
				//变量定义
				ASSERT(CSkinRenderManager::GetInstance()!=NULL);
				CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();

				//渲染颜色
				COLORREF crEnableBorad=pSkinRenderManager->RenderColor(0.77,0.65);
				pDC->Draw3dRect(0,0,rcClient.Width(),rcClient.Height(),crEnableBorad,crEnableBorad);
			}
			else
			{
				COLORREF crEnableBorad=m_SkinAttribute.m_crEnableBorad;
				pDC->Draw3dRect(0,0,rcClient.Width(),rcClient.Height(),crEnableBorad,crEnableBorad);
			}
		}
		else
		{
			COLORREF crDisableBorad=m_SkinAttribute.m_crDisableBorad;
			pDC->Draw3dRect(0,0,rcClient.Width(),rcClient.Height(),crDisableBorad,crDisableBorad);
		}
	}
	else
	{
		COLORREF crEnableBorad=m_SkinAttribute.m_crEnableBack;
		pDC->Draw3dRect(0,0,rcClient.Width(),rcClient.Height(),crEnableBorad,crEnableBorad);
	}

	//计算位置
	INT nImageIndex=0;
	if (bDisable==true) nImageIndex=4;
	else if (bDropped==true) nImageIndex=1;
	else if (m_bHovering==true) nImageIndex=3;

	//加载按钮
	CBitImage ImageButton;
	if ((m_hResInstance==NULL)||(m_uResourceID==0))
	{
		ImageButton.LoadFromResource(GetModuleHandle(SKIN_CONTROL_DLL_NAME),IDB_SKIN_COMBOBOX_BUTTON);
	}
	else
	{
		ImageButton.LoadFromResource(m_hResInstance,m_uResourceID);
	}

	//渲染资源
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();
	if ((pSkinRenderManager!=NULL)&&(m_bRenderImage==true)) pSkinRenderManager->RenderImage(ImageButton);

	//绘画按钮
	CSize SizeButton;
	SizeButton.SetSize(ImageButton.GetWidth()/5L,ImageButton.GetHeight());
	ImageButton.StretchBlt(pDC->m_hDC,rcClient.Width()-m_uButtonWidth-1,1,m_uButtonWidth,rcClient.Height()-2,
		nImageIndex*SizeButton.cx,0,SizeButton.cx,SizeButton.cy);

	return;
}

//按钮检测
bool CSkinComboBox::VerdictOverButton(CPoint MousePoint)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//下拉列表
	if ((rcClient.PtInRect(MousePoint)==TRUE)&&((GetStyle()&CBS_SIMPLE)!=0)) return true;

	//坐标计算
	if ((MousePoint.y>(rcClient.Height()-1))||(MousePoint.y<1)) return false;
	if ((MousePoint.x<(rcClient.Width()-(INT)m_uButtonWidth-1))||(MousePoint.x>(rcClient.Width()-1))) return false;

	return true;
}

//边框设置
VOID CSkinComboBox::SetDrawBorad(bool bDrawBorad)
{
	//设置变量
	m_bDrawBorad=bDrawBorad;

	//更新界面
	if (m_hWnd!=NULL) Invalidate(FALSE);

	return;
}

//设置渲染
VOID CSkinComboBox::SetRenderImage(bool bRenderImage)
{
	//设置变量
	m_bRenderImage=bRenderImage;

	//更新界面
	if (m_hWnd!=NULL) Invalidate(FALSE);

	return;
}

//设置资源
VOID CSkinComboBox::SetButtonImage(HINSTANCE hResInstance, UINT uResourceID, CSize & SizeButton)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//设置变量
	m_uResourceID=uResourceID;
	m_hResInstance=hResInstance;

	//设置控件
	m_uButtonWidth=SizeButton.cx;
	SetItemHeight(LB_ERR,SizeButton.cy);

	//配置控件
	RectifyControl(rcClient.Width(),rcClient.Height());

	//更新界面
	if (m_hWnd!=NULL) RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW);

	return;
}

//配置控件
VOID CSkinComboBox::Initialization()
{
	//设置状态
	m_bHovering=false;
	m_uButtonWidth=m_SkinAttribute.m_uButtonWidth;

	//变量定义
	COMBOBOXINFO ComboBoxInfo;
	ComboBoxInfo.cbSize=sizeof(ComboBoxInfo);

	//绑定控件
	if (GetComboBoxInfo(&ComboBoxInfo)==TRUE)
	{
		if (ComboBoxInfo.hwndItem!=NULL) m_SkinComboBoxEdit.SubclassWindow(ComboBoxInfo.hwndItem);
		if (ComboBoxInfo.hwndList!=NULL) m_SkinComboBoxList.SubclassWindow(ComboBoxInfo.hwndList);
	}

	return;
}

//更新按钮
VOID CSkinComboBox::RedrawDropButton()
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//按钮位置
	CRect rcDropButton;
	rcDropButton.SetRect(rcClient.Width()-m_uButtonWidth-1,1,rcClient.Width()-1,rcClient.Height()-1);

	//更新界面
	RedrawWindow(&rcDropButton,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW);

	return;
}

//调整控件
VOID CSkinComboBox::RectifyControl(INT nWidth, INT nHeight)
{
	//移动控件
	if (m_SkinComboBoxEdit.m_hWnd!=NULL)
	{
		//获取位置
		CRect rcClient;
		GetClientRect(&rcClient);

		//移动控件
		CSize SizeEdit;
		SizeEdit.SetSize(rcClient.Width()-m_uButtonWidth-10,12);
		m_SkinComboBoxEdit.SetWindowPos(NULL,6,(rcClient.Height()-12)/2,SizeEdit.cx,SizeEdit.cy,SWP_NOZORDER);
	}

	return;
}

//重画消息
VOID CSkinComboBox::OnPaint()
{
	CPaintDC dc(this);

	//绘画控件
	DrawControlView(&dc);

	return;
}

//列表关闭
VOID CSkinComboBox::OnCbnCloseup()
{
	//更新按钮
	RedrawDropButton();

	return;
}

//绘画背景
BOOL CSkinComboBox::OnEraseBkgnd(CDC * pDC)
{
	return TRUE;
}

//时间消息
VOID CSkinComboBox::OnTimer(UINT_PTR nIDEvent)
{
	//盘旋判断
	if (nIDEvent==IDI_CHECK_HOVER)
	{
		//盘旋判断
		if (m_bHovering==true)
		{
			//获取光标
			CPoint MousePoint;
			GetCursorPos(&MousePoint);
			ScreenToClient(&MousePoint);

			//盘旋判断
			m_bHovering=VerdictOverButton(MousePoint);

			//更新界面
			if (m_bHovering==false) RedrawDropButton();
		}

		//删除时间
		if (m_bHovering==false) KillTimer(IDI_CHECK_HOVER);

		return;
	}

	__super::OnTimer(nIDEvent);
}

//位置消息
VOID CSkinComboBox::OnSize(UINT nType, INT cx, INT cy) 
{
	__super::OnSize(nType, cx, cy);

	//调整控件
	RectifyControl(cx,cy);

	return;
}

//创建消息
INT CSkinComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//配置控件
	Initialization();
	RectifyControl(rcClient.Width(),rcClient.Height());

	return 0;
}

//鼠标消息
VOID CSkinComboBox::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags, Point);

	if (m_bHovering==false)
	{
		//盘旋判断
		m_bHovering=VerdictOverButton(Point);

		//状态判断
		if (m_bHovering==true)
		{
			//更新界面
			RedrawDropButton();

			//设置时间
			SetTimer(IDI_CHECK_HOVER,TIME_CHECK_HOVER,NULL);
		}
	}

	return;
}

//鼠标消息
VOID CSkinComboBox::OnLButtonDown(UINT nFlags, CPoint Point)
{
	//设置焦点
	SetFocus();

	//显示列表
	if (VerdictOverButton(Point)==true)
	{
		//显示列表
		ShowDropDown(GetDroppedState()==FALSE);

		//更新按钮
		RedrawDropButton();
	}

	return;
}

//鼠标消息
VOID CSkinComboBox::OnLButtonDblClk(UINT nFlags, CPoint Point)
{
	//设置焦点
	SetFocus();

	//显示列表
	if (VerdictOverButton(Point)==true)
	{
		//显示列表
		ShowDropDown(GetDroppedState()==FALSE);

		//更新按钮
		RedrawDropButton();
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////