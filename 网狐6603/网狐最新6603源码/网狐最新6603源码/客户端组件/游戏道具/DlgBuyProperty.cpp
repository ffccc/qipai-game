#include "StdAfx.h"
#include "Resource.h"
#include "DlgBuyProperty.h"
#include "GamePropertyItem.h"

//////////////////////////////////////////////////////////////////////////////////

//静态变量
CDlgBuyProperty * CDlgBuyProperty::m_pDlgBuyProperty=NULL;				//对象指针

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgBuyProperty, CSkinDialog)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgBuyProperty::CDlgBuyProperty() : CSkinDialog(IDD_DLG_PROPERTY)
{
	//设置变量
	m_pGamePropertyItem=NULL;
	m_pIGamePropertySink=NULL;

	//设置对象
	ASSERT(m_pDlgBuyProperty==NULL);
	if (m_pDlgBuyProperty==NULL) m_pDlgBuyProperty=this;

	return;
}

//析构函数
CDlgBuyProperty::~CDlgBuyProperty()
{
	//设置对象
	ASSERT(m_pDlgBuyProperty==this);
	if (m_pDlgBuyProperty==this) m_pDlgBuyProperty=NULL;

	return;
}

//控件绑定
VOID CDlgBuyProperty::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	//按钮控件
	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Control(pDX, IDCANCEL, m_btCancel);

	//编辑控件
	DDX_Control(pDX, IDC_NICK_NAME, m_edNickName);
	DDX_Control(pDX, IDC_PROPERTY_COUNT, m_edPropertyCount);
}

//消息解释
BOOL CDlgBuyProperty::PreTranslateMessage(MSG * pMsg)
{
	//按键过虑
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//创建函数
BOOL CDlgBuyProperty::OnInitDialog()
{
	__super::OnInitDialog();

	//更新状态
	UpdateCtrlStatus();

	//输入限制
	m_edNickName.LimitText(LEN_NICKNAME-1);

	return FALSE;
}

//确定函数
VOID CDlgBuyProperty::OnOK()
{
	//获取信息
	CString strNickName;
	GetDlgItemText(IDC_NICK_NAME,strNickName);
	strNickName.TrimLeft();strNickName.TrimRight();

	//获取数目
	WORD wItemCount=GetDlgItemInt(IDC_PROPERTY_COUNT);

	//昵称判断
	if (strNickName.IsEmpty()==true)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("用户昵称不正确，请重新输入"),MB_ICONERROR);

		//设置焦点
		m_edNickName.SetFocus();

		return;
	}

	//数目判断
	if (wItemCount==0)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("购买数目不正确，请重新输入"),MB_ICONERROR);

		//设置焦点
		m_edPropertyCount.SetFocus();

		return;
	}

	//获取信息
	ASSERT(m_pGamePropertyItem->GetPropertyAttrib()!=NULL);
	tagPropertyAttrib * pPropertyAttrib=m_pGamePropertyItem->GetPropertyAttrib();

	//事件通知
	ASSERT(m_pIGamePropertySink!=NULL);
	if (m_pIGamePropertySink->OnEventBuyProperty(strNickName,wItemCount,pPropertyAttrib->wIndex)==true)
	{
		//关闭窗口
		DestroyWindow();
	}

	return;
}

//取消消息
VOID CDlgBuyProperty::OnCancel()
{
	//关闭窗口
	DestroyWindow();

	return;
}

//绘画消息
VOID CDlgBuyProperty::OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight)
{
	//框架位置
	INT nTBorder=m_SkinAttribute.m_EncircleInfoFrame.nTBorder;
	INT nBBorder=m_SkinAttribute.m_EncircleInfoFrame.nBBorder;
	INT nLBorder=m_SkinAttribute.m_EncircleInfoFrame.nLBorder;
	INT nRBorder=m_SkinAttribute.m_EncircleInfoFrame.nRBorder;

	//加载资源
	CPngImage ImagePropertyFrame;
	ImagePropertyFrame.LoadImage(GetModuleHandle(GAME_PROPERTY_DLL_NAME),TEXT("PROPERTY_FRAME"));

	//获取大小
	CSize SizePropertyFrame;
	SizePropertyFrame.SetSize(ImagePropertyFrame.GetWidth(),ImagePropertyFrame.GetHeight());

	//绘画框架
	INT nXItemPos=nLBorder+15;
	INT nYItemPos=nTBorder+15;
	ImagePropertyFrame.DrawImage(pDC,nXItemPos-(SizePropertyFrame.cx-PROPERTY_SYMBOL_CX)/2,nYItemPos-(SizePropertyFrame.cy-PROPERTY_SYMBOL_CY)/2);

	//绘画图标
	if (m_pGamePropertyItem!=NULL)
	{
		//获取资源
		tagPropertyImage PropertyImage;
		m_pGamePropertyItem->GetImageInfo(PropertyImage);

		//绘画图标
		CPngImage ImageSymbol;
		ImageSymbol.LoadImage(PropertyImage.hResInstance,PropertyImage.pszSymbolID);
		ImageSymbol.DrawImage(pDC,nXItemPos,nYItemPos,PROPERTY_SYMBOL_CX,PROPERTY_SYMBOL_CY,0,0,ImageSymbol.GetWidth(),ImageSymbol.GetHeight());
	}

	return;
}

//更新状态
VOID CDlgBuyProperty::UpdateCtrlStatus()
{
	//效验参数
	ASSERT(m_pGamePropertyItem!=NULL);
	ASSERT(m_pIGamePropertySink!=NULL);

	//获取属性
	tagPropertyInfo * pPropertyInfo=m_pGamePropertyItem->GetPropertyInfo();
	tagPropertyAttrib * pPropertyAttrib=m_pGamePropertyItem->GetPropertyAttrib();

	//道具价格
	TCHAR szPropertyGold[64]=TEXT("");
	_sntprintf(szPropertyGold,CountArray(szPropertyGold),TEXT("%I64d 游戏币/个 （会员折扣：%ld%%）"),pPropertyInfo->lPropertyGold,pPropertyInfo->wDiscount);

	//设置标题
	SetWindowText(pPropertyAttrib->szPropertyName);

	//道具价格
	SetDlgItemText(IDC_PROPERTY_GOLD,szPropertyGold);

	//道具属性
	SetDlgItemText(IDC_PROPERTY_NAME,pPropertyAttrib->szPropertyName);
	SetDlgItemText(IDC_PROPERTY_DESCRIBE,pPropertyAttrib->szRegulationsInfo);

	return;
}

//显示窗口
VOID CDlgBuyProperty::ShowBuyPropertyWnd(CGamePropertyItem * pGamePropertyItem, IGamePropertySink * pIGamePropertySink)
{
	//效验参数
	ASSERT(pGamePropertyItem!=NULL);
	ASSERT(pIGamePropertySink!=NULL);

	//变量定义
	CDlgBuyProperty * pDlgBuyProperty=m_pDlgBuyProperty;

	//创建对象
	if (pDlgBuyProperty==NULL)
	{
		try
		{
			pDlgBuyProperty=new CDlgBuyProperty;
		}
		catch (...)
		{
			ASSERT(FALSE);
		}
	}

	//设置变量
	pDlgBuyProperty->m_pGamePropertyItem=pGamePropertyItem;
	pDlgBuyProperty->m_pIGamePropertySink=pIGamePropertySink;

	//创建窗口
	if (pDlgBuyProperty->m_hWnd==NULL)
	{
		AfxSetResourceHandle(GetModuleHandle(GAME_PROPERTY_DLL_NAME));
		pDlgBuyProperty->Create(IDD_DLG_PROPERTY,AfxGetMainWnd());
		AfxSetResourceHandle(GetModuleHandle(NULL));
	}
	else
	{
		//更新状态
		pDlgBuyProperty->UpdateCtrlStatus();
	}

	//显示窗口
	pDlgBuyProperty->ShowWindow(SW_SHOW);

	//激活窗口
	pDlgBuyProperty->SetActiveWindow();
	pDlgBuyProperty->SetForegroundWindow();

	return;
}

//////////////////////////////////////////////////////////////////////////////////
