#include "Stdafx.h"
#include "SkinEdit.h"
#include "SkinDialog.h"

//////////////////////////////////////////////////////////////////////////////////

//静态变量
CSkinEditAttribute					CSkinEdit::m_SkinAttribute;				//界面属性

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSkinEdit, CEdit)
    ON_WM_NCPAINT()
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSkinEditEx, CSkinEdit)
    ON_WM_NCPAINT()
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CSkinEditAttribute::CSkinEditAttribute()
{
	//背景颜色
	m_crEnableBK=CLR_INVALID;
	m_crDisableBK=CLR_INVALID;

	//字体颜色
	m_crEnableText=CLR_INVALID;
	m_crDisableText=CLR_INVALID;

	//边框颜色
	m_crEnableBorad=CLR_INVALID;
	m_crDisableBorad=CLR_INVALID;

	return;
}

//析构函数
CSkinEditAttribute::~CSkinEditAttribute() 
{
	//销毁资源
	if (m_brEnable.GetSafeHandle()!=NULL) m_brEnable.DeleteObject();
	if (m_brDisable.GetSafeHandle()!=NULL) m_brDisable.DeleteObject();

	return;
}

//配置资源
bool CSkinEditAttribute::Initialization(CSkinRenderManager * pSkinRenderManager)
{
	//背景颜色
	m_crEnableBK=RGB(255,255,255);
	m_crDisableBK=RGB(125,125,125);

	//字体颜色
	m_crEnableText=RGB(10,10,10);
	m_crDisableText=RGB(50,50,50);

	//边框颜色
	m_crEnableBorad=RGB(92,100,105);
	m_crDisableBorad=GetSysColor(COLOR_WINDOW);

	//创建画刷
	m_brEnable.DeleteObject();
	m_brDisable.DeleteObject();
	m_brEnable.CreateSolidBrush(m_crEnableBK);
	m_brDisable.CreateSolidBrush(m_crDisableBK);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CSkinEdit::CSkinEdit()
{
}

//析构函数
CSkinEdit::~CSkinEdit()
{
}

//绘画控件
HBRUSH CSkinEdit::CtlColor(CDC * pDC, UINT nCtlColor)
{
	//设置环境
	if (IsWindowEnabled()==FALSE)
	{
		//禁用状态
		pDC->SetBkColor(m_SkinAttribute.m_crDisableBK);
		pDC->SetTextColor(m_SkinAttribute.m_crDisableText);
		return m_SkinAttribute.m_brDisable;
	}
	else
	{
		//普通状态
		pDC->SetBkColor(m_SkinAttribute.m_crEnableBK);
		pDC->SetTextColor(m_SkinAttribute.m_crEnableText);
		return m_SkinAttribute.m_brEnable;
	}

	return NULL;
}

//绘画控件
VOID CSkinEdit::DrawControlView(CDC * pDC, COLORREF crColorBorad,  COLORREF crColorBK)
{
	//获取位置
	CRect rcWindow;
	GetWindowRect(&rcWindow);
	ScreenToClient(&rcWindow);

	//绘画边框
	if (GetExStyle()&WS_EX_CLIENTEDGE)
	{
		pDC->Draw3dRect(&rcWindow,crColorBorad,crColorBorad);
		pDC->Draw3dRect(rcWindow.left+1,rcWindow.top+1,rcWindow.Width()-2,rcWindow.Height()-2,crColorBK,crColorBK);
	}
	else
	{
		pDC->Draw3dRect(&rcWindow,crColorBK,crColorBK);
		pDC->Draw3dRect(rcWindow.left+1,rcWindow.top+1,rcWindow.Width()-2,rcWindow.Height()-2,crColorBK,crColorBK);
	}

	return;
}

//重画消息
VOID CSkinEdit::OnNcPaint()
{
	//状态变量
	bool bDisable=(IsWindowEnabled()==FALSE);
	COLORREF crColorBK=(bDisable==false)?m_SkinAttribute.m_crEnableBK:m_SkinAttribute.m_crDisableBK;
	COLORREF crColorBorad=(bDisable==false)?m_SkinAttribute.m_crEnableBorad:m_SkinAttribute.m_crDisableBorad;

	//绘画边框
	CClientDC ClientDC(this);
	DrawControlView(&ClientDC,crColorBorad,crColorBK);

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CSkinEditEx::CSkinEditEx()
{
	//背景颜色
	m_crEnableBK=m_SkinAttribute.m_crEnableBK;
	m_crDisableBK=m_SkinAttribute.m_crDisableBK;

	//字体颜色
	m_crEnableText=m_SkinAttribute.m_crEnableText;
	m_crDisableText=m_SkinAttribute.m_crDisableText;

	//边框颜色
	m_crEnableBorad=m_SkinAttribute.m_crEnableBorad;
	m_crDisableBorad=m_SkinAttribute.m_crDisableBorad;

	return;
}

//析构函数
CSkinEditEx::~CSkinEditEx()
{
}

//设置颜色
VOID CSkinEditEx::SetEnableColor(COLORREF crEnableText, COLORREF crEnableBK, COLORREF crEnableBorad)
{
	//设置颜色
	m_crEnableBK=crEnableBK;
	m_crEnableText=crEnableText;
	m_crEnableBorad=crEnableBorad;

	//创建画刷
	m_brEnable.DeleteObject();
	m_brEnable.CreateSolidBrush(m_crEnableBK);

	//更新界面
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//设置颜色
VOID CSkinEditEx::SetDisableColor(COLORREF crDisableText, COLORREF crDisableBK, COLORREF crDisableBorad)
{
	//设置颜色
	m_crDisableBK=crDisableBK;
	m_crDisableText=crDisableText;
	m_crDisableBorad=crDisableBorad;

	//创建画刷
	m_brDisable.DeleteObject();
	m_brDisable.CreateSolidBrush(m_crDisableBK);

	//更新界面
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//重画消息
VOID CSkinEditEx::OnNcPaint()
{
	//状态变量
	bool bDisable=(IsWindowEnabled()==FALSE);
	COLORREF crColorBK=(bDisable==false)?m_crEnableBK:m_crDisableBK;
	COLORREF crColorBorad=(bDisable==false)?m_crEnableBorad:m_crDisableBorad;

	//绘画边框
	CClientDC ClientDC(this);
	DrawControlView(&ClientDC,crColorBorad,crColorBK);

	return;
}

//绘画控件
HBRUSH CSkinEditEx::CtlColor(CDC * pDC, UINT nCtlColor)
{
	//设置环境
	if (IsWindowEnabled()==FALSE)
	{
		//禁用状态
		pDC->SetBkColor((m_crDisableBK==CLR_INVALID)?m_SkinAttribute.m_crDisableBK:m_crDisableBK);
		pDC->SetTextColor((m_crDisableText==CLR_INVALID)?m_SkinAttribute.m_crDisableText:m_crDisableText);
		return (m_brDisable.GetSafeHandle()==NULL)?m_SkinAttribute.m_brDisable:m_brDisable;
	}
	else
	{
		//启用状态
		pDC->SetBkColor((m_crEnableBK==CLR_INVALID)?m_SkinAttribute.m_crEnableBK:m_crEnableBK);
		pDC->SetTextColor((m_crEnableText==CLR_INVALID)?m_SkinAttribute.m_crEnableText:m_crEnableText);
		return (m_brEnable.GetSafeHandle()==NULL)?m_SkinAttribute.m_brEnable:m_brEnable;
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////////////
