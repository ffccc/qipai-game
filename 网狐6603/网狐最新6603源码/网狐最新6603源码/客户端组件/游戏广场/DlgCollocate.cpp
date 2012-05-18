#include "StdAfx.h"
#include "GlobalUnits.h"
#include "DlgCollocate.h"
#include "PlatformFrame.h"

//////////////////////////////////////////////////////////////////////////////////

//按钮数目
#define BUTTON_COUNT_CX				3									//横向数目
#define BUTTON_COUNT_CY				5									//竖向数目

//尺寸定义
#define COLLOCATE_BAR_CX			150									//控制宽度
#define COLLOCATE_BAR_CY			330									//控制高度 

//尺寸定义
#define BAR_BT_SPACE				3									//按钮间距
#define BAR_BT_EXCURSION			5									//按钮偏移

//控件标识
#define IDC_COLLOCATE_BAR			100									//子项控制

//按钮位置
#define IMAGE_INDEX_NOTIFY			0									//消息设置
#define IMAGE_INDEX_CUSTOM			1									//控制设置
#define IMAGE_INDEX_SYSTEM			2									//系统设置
#define IMAGE_INDEX_PROXY			3									//代理设置
#define IMAGE_INDEX_SERVER			4									//房间设置

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CCollocateBar, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CCollocateItem, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CCollocateProxy, CCollocateItem)
	ON_BN_CLICKED(IDC_PROXY_TEST, OnBnClickedProxyTest)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CDlgCollocate, CSkinDialog)
	ON_BN_CLICKED(IDC_DEFAULT, OnBnClickedDefault)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CCollocateBar::CCollocateBar()
{
	//活动信息
	m_bHovering=false;
	m_wItemDown=INVALID_WORD;
	m_wItemHover=INVALID_WORD;
	m_wItemActive=INVALID_WORD;

	//子项信息
	m_SizeButton.SetSize(0,0);
	m_rcItemCreate.SetRect(0,0,0,0);

	//加载资源
	CPngImage ImageButton;
	ImageButton.LoadImage(AfxGetInstanceHandle(),TEXT("COLLOCATE_BAR_BT"));
	m_SizeButton.SetSize(ImageButton.GetWidth()/BUTTON_COUNT_CX,ImageButton.GetHeight()/BUTTON_COUNT_CY);

	return;
}

//析构函数
CCollocateBar::~CCollocateBar()
{
}

//消息解释
BOOL CCollocateBar::PreTranslateMessage(MSG * pMsg)
{
	//消息过虑
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
        GetParent()->PostMessage(WM_CLOSE,0,0);
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//保存参数
bool CCollocateBar::SaveParameter()
{
	//保存参数
	if (m_wItemActive!=INVALID_WORD)
	{
		CCollocateItem * pCollocateItem=m_CollocateItemArray[m_wItemActive];
		if (pCollocateItem->SaveParameter()==false) return false;
	}

	return true;
}

//默认参数
bool CCollocateBar::DefaultParameter()
{
	//设置子项
	for (INT_PTR i=0;i<m_CollocateItemArray.GetCount();i++)
	{
		//获取子项
		CCollocateItem * pCollocateItem=m_CollocateItemArray[i];

		//默认参数
		if (pCollocateItem->m_hWnd!=NULL) 
		{
			pCollocateItem->DefaultParameter();
			pCollocateItem->UpdateControlStatus();
		}
	}

	return true;
}

//设置区域
bool CCollocateBar::SetItemCreateRect(CRect rcItemCreate)
{
	//设置变量
	m_rcItemCreate=rcItemCreate;

	//调整控件
	for (INT_PTR i=0;i<m_CollocateItemArray.GetCount();i++)
	{
		CWnd * pWnd=m_CollocateItemArray[i];
		if (pWnd->m_hWnd!=NULL) pWnd->SetWindowPos(NULL,rcItemCreate.left,rcItemCreate.top,rcItemCreate.Width(),rcItemCreate.Height(),SWP_NOZORDER);
	}

	return true;
}

//插入子项
bool CCollocateBar::InsertCollocateItem(CCollocateItem * pCollocateItem)
{
	//设置变量
	m_CollocateItemArray.Add(pCollocateItem);

	//更新界面
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return true;
}

//激活设置
bool CCollocateBar::SetActiveCollocateItem(CCollocateItem * pCollocateItem)
{
	//效验参数
	ASSERT(pCollocateItem!=NULL);
	if (pCollocateItem==NULL) return false;

	//变量定义
	CCollocateItem * pCollocateActive=NULL;

	//切换判断
	if (m_wItemActive!=INVALID_WORD)
	{
		//获取子项
		ASSERT(m_wItemActive<m_CollocateItemArray.GetCount());
		pCollocateActive=m_CollocateItemArray[m_wItemActive];

		//切换判断
		if (pCollocateActive==pCollocateItem) return true;
		if (pCollocateActive->SaveParameter()==false) return false;
	}

	//激活子项
	for (INT_PTR i=0;i<m_CollocateItemArray.GetCount();i++)
	{
		if (m_CollocateItemArray[i]==pCollocateItem)
		{
			//创建子项
			if (pCollocateItem->m_hWnd==NULL)
			{
				pCollocateItem->CreateCollocateItem(GetParent(),m_rcItemCreate);
				pCollocateItem->UpdateControlStatus();
			}

			//设置变量
			m_wItemActive=(WORD)i;

			//显示窗口
			pCollocateItem->ShowWindow(SW_SHOW);
			pCollocateItem->SetFocus();

			//隐藏旧项
			if (pCollocateActive->GetSafeHwnd()!=NULL)
			{
				pCollocateActive->ShowWindow(SW_HIDE);
			}

			//更新界面
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);

			return true;
		}
	}

	return false;
}

//索引切换
WORD CCollocateBar::SwitchToButtonIndex(CPoint MousePoint)
{
	//窗口位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//边界判断
	if (MousePoint.x<((rcClient.Width()-m_SizeButton.cx)/2)) return INVALID_WORD;
	if (MousePoint.x>((rcClient.Width()+m_SizeButton.cx)/2)) return INVALID_WORD;

	//子项判断
	for (WORD i=0;i<m_CollocateItemArray.GetCount();i++)
	{
		//位置计算
		CRect rcButton;
		rcButton.left=(rcClient.Width()-m_SizeButton.cx)/2;
		rcButton.right=(rcClient.Width()+m_SizeButton.cx)/2;
		rcButton.top=(m_SizeButton.cy+BAR_BT_SPACE)*i+BAR_BT_EXCURSION;
		rcButton.bottom=(m_SizeButton.cy+BAR_BT_SPACE)*i+m_SizeButton.cy+BAR_BT_EXCURSION;

		//区域判断
		if (rcButton.PtInRect(MousePoint)) return i;
	}

	return INVALID_WORD;
}

//重画消息
VOID CCollocateBar::OnPaint()
{
	CPaintDC dc(this);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//创建缓冲
	CBitImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//创建设备
	CImageDC BufferDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	//设置设备
	pBufferDC->SetBkMode(TRANSPARENT);
	pBufferDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//加载资源
	CPngImage ImageButton;
	CPngImage ImageBarLogo;
	ImageButton.LoadImage(AfxGetInstanceHandle(),TEXT("COLLOCATE_BAR_BT"));
	ImageBarLogo.LoadImage(AfxGetInstanceHandle(),TEXT("COLLOCATE_BAR_LOGO"));

	//绘画背景
	pBufferDC->FillSolidRect(&rcClient,RGB(223,236,246));
	ImageBarLogo.DrawImage(pBufferDC,rcClient.Width()-ImageBarLogo.GetWidth(),rcClient.Height()-ImageBarLogo.GetHeight());

	//绘画按钮
	for (INT i=0;i<m_CollocateItemArray.GetCount();i++)
	{
		//获取对象
		ASSERT(m_CollocateItemArray[i]!=NULL);
		CCollocateItem * pCollocateItem=m_CollocateItemArray[i];

		//变量定义
		INT nXButtomPos=0;
		INT nYButtomPos=pCollocateItem->m_cbImageIndex*m_SizeButton.cy;

		//横行计算
		if ((m_wItemHover==i)&&(m_wItemDown!=i)) nXButtomPos=m_SizeButton.cx*2;
		if ((m_wItemActive==i)||(m_wItemDown==i)) nXButtomPos=m_SizeButton.cx;

		//绘画按钮
		INT nXDrawPos=(rcClient.Width()-m_SizeButton.cx)/2;
		INT nYDrawPos=(m_SizeButton.cy+BAR_BT_SPACE)*i+BAR_BT_EXCURSION;
		ImageButton.DrawImage(pBufferDC,nXDrawPos,nYDrawPos,m_SizeButton.cx,m_SizeButton.cy,nXButtomPos,nYButtomPos);
	}

	//绘画界面
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return;
}

//建立消息
INT CCollocateBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//设置变量
	m_bHovering=false;
	m_wItemDown=INVALID_WORD;
	m_wItemHover=INVALID_WORD;
	m_wItemActive=INVALID_WORD;

	//子项信息
	m_rcItemCreate.SetRect(0,0,0,0);
	m_CollocateItemArray.RemoveAll();

	return 0;
}

//鼠标消息
VOID CCollocateBar::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags, Point);

	//窗口位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//位置计算
	WORD wCollocateHover=SwitchToButtonIndex(Point);

	//盘旋变量
	if (wCollocateHover!=m_wItemHover)
	{
		//设置变量
		m_wItemHover=wCollocateHover;

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	//进入判断
	if (m_bHovering==false)
	{
		//设置变量
		m_bHovering=true;

		//变量定义
		TRACKMOUSEEVENT TrackMouseEvent;
		ZeroMemory(&TrackMouseEvent,sizeof(TrackMouseEvent));

		//注册消息
		TrackMouseEvent.hwndTrack=m_hWnd;
		TrackMouseEvent.dwFlags=TME_LEAVE;
		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);

		//注册事件
		_TrackMouseEvent(&TrackMouseEvent);
	}

	return;
}

//鼠标消息
VOID CCollocateBar::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags,Point);

	//激活子项
	if ((m_wItemHover!=INVALID_WORD)&&(m_wItemHover==m_wItemDown))
	{
		SetActiveCollocateItem(m_CollocateItemArray[m_wItemHover]);
	}

	//按起处理
	if (m_wItemDown!=INVALID_WORD)
	{
		//释放鼠标
		ReleaseCapture();

		//设置变量
		m_wItemDown=INVALID_WORD;

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//鼠标消息
VOID CCollocateBar::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//设置焦点
	SetFocus();

	//按下处理
	if ((m_wItemHover!=INVALID_WORD)&&(m_wItemHover!=m_wItemActive))
	{
		//鼠标扑获
		SetCapture();

		//设置变量
		m_wItemDown=m_wItemHover;

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//鼠标消息
LRESULT CCollocateBar::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	if (m_bHovering==true)
	{
		//设置变量
		m_bHovering=false;
		m_wItemHover=INVALID_WORD;

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return 0;
}

//光标消息
BOOL CCollocateBar::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//设置光标
	if (m_wItemHover!=INVALID_WORD)
	{
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_HAND_CUR)));
		return TRUE;
	}

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CCollocateItem::CCollocateItem(UINT uIDTemplate) : CDialog(uIDTemplate)
{
	//创建资源
	m_cbImageIndex=0;

	return;
}

//析构函数
CCollocateItem::~CCollocateItem()
{
}

//创建窗口
bool CCollocateItem::CreateCollocateItem(CWnd * pParentWnd, CRect rcCreate)
{
	//创建窗口
	Create(m_lpszTemplateName,pParentWnd);

	//移动窗口
	SetWindowPos(NULL,rcCreate.left,rcCreate.top,rcCreate.Width(),rcCreate.Height(),SWP_NOZORDER);

	return true;
}

//背景消息
BOOL CCollocateItem::OnEraseBkgnd(CDC * pDC)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//填充背景
	pDC->FillSolidRect(&rcClient,CSkinDialog::m_SkinAttribute.m_crBackGround);

	return TRUE;
}

//颜色消息
HBRUSH CCollocateItem::OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
	case CTLCOLOR_STATIC:
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetBkColor(RGB(255,255,255));
			pDC->SetTextColor(RGB(0,0,0));
			return CSkinDialog::m_SkinAttribute.m_brBackGround;
		}
	}

	return __super::OnCtlColor(pDC,pWnd,nCtlColor);
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CCollocateServer::CCollocateServer() : CCollocateItem(IDD_COLLOCATE_SERVER)
{
	//默认参数
	DefaultParameter();

	//设置变量
	m_cbImageIndex=IMAGE_INDEX_SERVER;

	return;
}

//析构函数
CCollocateServer::~CCollocateServer()
{
}

//控件绑定
VOID CCollocateServer::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WIN_RATE, m_edWinRate);
	DDX_Control(pDX, IDC_FLEE_RATE, m_edFleeRate);
	DDX_Control(pDX, IDC_GAME_SCORE_MIN, m_edScoreMin);
	DDX_Control(pDX, IDC_GAME_SCORE_MAX, m_edScoreMax);
	DDX_Control(pDX, IDC_TABLE_PASSWORD, m_edPassword);
}

//配置函数
BOOL CCollocateServer::OnInitDialog()
{
	__super::OnInitDialog();

	//限制输入
	((CEdit *)GetDlgItem(IDC_WIN_RATE))->LimitText(5);
	((CEdit *)GetDlgItem(IDC_FLEE_RATE))->LimitText(5);
	((CEdit *)GetDlgItem(IDC_GAME_SCORE_MAX))->LimitText(10);
	((CEdit *)GetDlgItem(IDC_GAME_SCORE_MIN))->LimitText(11);
	((CEdit *)GetDlgItem(IDC_TABLE_PASSWORD))->LimitText(LEN_PASSWORD-1);

	return TRUE;
}

//保存参数
bool CCollocateServer::SaveParameter()
{
	//获取参数
	m_bLimitDetest=(IsDlgButtonChecked(IDC_LIMIT_DETEST)==BST_CHECKED)?true:false;
	m_bLimitSameIP=(IsDlgButtonChecked(IDC_LIMIT_SAME_IP)==BST_CHECKED)?true:false;
	m_bTakePassword=(IsDlgButtonChecked(IDC_TAKE_PASSWORD)==BST_CHECKED)?true:false;
	m_bLimitWinRate=(IsDlgButtonChecked(IDC_LIMIT_WIN_RATE)==BST_CHECKED)?true:false;
	m_bLimitFleeRate=(IsDlgButtonChecked(IDC_LIMIT_FLEE_RATE)==BST_CHECKED)?true:false;
	m_bLimitGameScore=(IsDlgButtonChecked(IDC_LIMIT_GAME_SCORE)==BST_CHECKED)?true:false;

	//最低胜率
	TCHAR szBuffer[128]=TEXT("");
	GetDlgItemText(IDC_WIN_RATE,szBuffer,CountArray(szBuffer));
	m_wMinWinRate=(WORD)(_tstof(szBuffer)*100.0)%10000;

	//最高逃率
	GetDlgItemText(IDC_FLEE_RATE,szBuffer,CountArray(szBuffer));
	m_wMaxFleeRate=(WORD)(_tstof(szBuffer)*100.0)%10000;

	//积分限制
	m_lMaxGameScore=GetDlgItemInt(IDC_GAME_SCORE_MAX,NULL,TRUE);
	m_lMinGameScore=GetDlgItemInt(IDC_GAME_SCORE_MIN,NULL,TRUE);
	if ((m_bLimitGameScore)&&(m_lMinGameScore>=m_lMaxGameScore))
	{
		//显示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("积分限制范围参数不正确，请正确设置积分限制范围！"),MB_ICONINFORMATION);

		//设置焦点
		GetDlgItem(IDC_GAME_SCORE_MAX)->SetFocus();

		return false;
	}

	//携带密码
	GetDlgItemText(IDC_TABLE_PASSWORD,m_szPassword,CountArray(m_szPassword));
	if ((m_bTakePassword==true)&&(m_szPassword[0]==0))
	{
		//显示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("桌子携带密码没有设置，请输入设置桌子携带密码！"),MB_ICONINFORMATION);

		//设置焦点
		GetDlgItem(IDC_TABLE_PASSWORD)->SetFocus();

		return false;
	}

	return true;
}

//默认参数
bool CCollocateServer::DefaultParameter()
{
	//胜率限制
	m_wMinWinRate=0;
	m_bLimitWinRate=false;

	//逃率限制
	m_wMaxFleeRate=5000;
	m_bLimitFleeRate=false;

	//积分限制
	m_bLimitGameScore=false;
	m_lMaxGameScore=2147483647L;
	m_lMinGameScore=-2147483646L;

	//其他配置
	m_bLimitDetest=true;
	m_bLimitSameIP=true;

	//携带密码
	m_bTakePassword=false;
	ZeroMemory(m_szPassword,sizeof(m_szPassword));

	return true;
}

//更新控制
bool CCollocateServer::UpdateControlStatus()
{
	//设置控件
	CheckDlgButton(IDC_LIMIT_DETEST,(m_bLimitDetest==true)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_LIMIT_SAME_IP,(m_bLimitSameIP==true)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_TAKE_PASSWORD,(m_bTakePassword==true)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_LIMIT_WIN_RATE,(m_bLimitWinRate==true)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_LIMIT_FLEE_RATE,(m_bLimitFleeRate==true)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_LIMIT_GAME_SCORE,(m_bLimitGameScore==true)?BST_CHECKED:BST_UNCHECKED);

	//最低胜率
	TCHAR szBuffer[128]=TEXT("");
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("%.2f"),((DOUBLE)(m_wMinWinRate))/100.0);
	SetDlgItemText(IDC_WIN_RATE,szBuffer);

	//最高逃率
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("%.2f"),((DOUBLE)(m_wMaxFleeRate))/100.0);
	SetDlgItemText(IDC_FLEE_RATE,szBuffer);

	//积分限制
	SetDlgItemInt(IDC_GAME_SCORE_MAX,m_lMaxGameScore,TRUE);
	SetDlgItemInt(IDC_GAME_SCORE_MIN,m_lMinGameScore,TRUE);

	//携带密码
	SetDlgItemText(IDC_TABLE_PASSWORD,m_szPassword);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CCollocateProxy::CCollocateProxy() : CCollocateItem(IDD_COLLOCATE_PROXY)
{
	//默认参数
	DefaultParameter();

	//设置变量
	m_cbImageIndex=IMAGE_INDEX_PROXY;

	return;
}

//析构函数
CCollocateProxy::~CCollocateProxy()
{
}

//控件绑定
VOID CCollocateProxy::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROXY_TYPE, m_cmProxyType);
	DDX_Control(pDX, IDC_PROXY_TEST, m_btProxyTest);
	DDX_Control(pDX, IDC_PROXY_PORT, m_edProxyPort);
	DDX_Control(pDX, IDC_PROXY_USER, m_edProxyUserName);
	DDX_Control(pDX, IDC_PROXY_PASS, m_edProxyPassword);
	DDX_Control(pDX, IDC_PROXY_SERVER, m_edProxyServer);
}

//创建函数
BOOL CCollocateProxy::OnInitDialog()
{
	__super::OnInitDialog();

	//限制输入
	((CEdit *)(GetDlgItem(IDC_PROXY_PORT)))->LimitText(5);
	((CEdit *)(GetDlgItem(IDC_PROXY_USER)))->LimitText(31);
	((CEdit *)(GetDlgItem(IDC_PROXY_PASS)))->LimitText(31);
	((CEdit *)(GetDlgItem(IDC_PROXY_SERVER)))->LimitText(31);

	//变量定义
	BYTE cbProxyType[]={PROXY_NONE,PROXY_HTTP,PROXY_SOCKS4,PROXY_SOCKS5};
	LPCTSTR szProxyType[]={TEXT("不使用代理"),TEXT("HTTP 代理"),TEXT("SOCKS 4"),TEXT("SOCKS 5")};

	//加载类型
	for (INT i=0;i<CountArray(szProxyType);i++)
	{
		INT nItem=m_cmProxyType.AddString(szProxyType[i]);
		m_cmProxyType.SetItemData(nItem,cbProxyType[i]);
		if (m_cbProxyType==cbProxyType[i]) m_cmProxyType.SetCurSel(nItem);
	}

	//设置信息
	SetDlgItemText(IDC_PROXY_USER,m_ProxyServer.szUserName);
	SetDlgItemText(IDC_PROXY_PASS,m_ProxyServer.szPassword);
	SetDlgItemText(IDC_PROXY_SERVER,m_ProxyServer.szProxyServer);
	if (m_ProxyServer.wProxyPort!=0) SetDlgItemInt(IDC_PROXY_PORT,m_ProxyServer.wProxyPort);

	//设置选择
	if (m_cmProxyType.GetCurSel()==CB_ERR) m_cmProxyType.SetCurSel(0);

	return FALSE;
}

//保存参数
bool CCollocateProxy::SaveParameter()
{
	//代理类型
	m_cbProxyType=(BYTE)m_cmProxyType.GetItemData(m_cmProxyType.GetCurSel());

	//代理信息
	m_ProxyServer.wProxyPort=GetDlgItemInt(IDC_PROXY_PORT);
	GetDlgItemText(IDC_PROXY_USER,m_ProxyServer.szUserName,CountArray(m_ProxyServer.szUserName));
	GetDlgItemText(IDC_PROXY_PASS,m_ProxyServer.szPassword,CountArray(m_ProxyServer.szPassword));
	GetDlgItemText(IDC_PROXY_SERVER,m_ProxyServer.szProxyServer,CountArray(m_ProxyServer.szProxyServer));

	//效验代理
	if (m_cbProxyType!=PROXY_NONE)
	{
		//代理地址
		if (m_ProxyServer.szProxyServer[0]==0)
		{
			//错误提示
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("代理服务器地址不能为空，请重新输入！"),MB_ICONERROR,30);

			//设置焦点
			m_edProxyServer.SetFocus();

			return false;
		}

		//代理端口
		if (m_ProxyServer.wProxyPort==0)
		{
			//错误提示
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("请输入代理服务器端口号，例如：1080！"),MB_ICONERROR,30);

			//设置焦点
			m_edProxyPort.SetFocus();

			return false;
		}
	}

	return true;
}

//默认参数
bool CCollocateProxy::DefaultParameter()
{
	//配置变量
	m_cbProxyType=PROXY_NONE;
	ZeroMemory(&m_ProxyServer,sizeof(m_ProxyServer));

	return true;
}

//更新控制
bool CCollocateProxy::UpdateControlStatus()
{
	//代理信息
	SetDlgItemText(IDC_PROXY_USER,m_ProxyServer.szUserName);
	SetDlgItemText(IDC_PROXY_PASS,m_ProxyServer.szPassword);
	SetDlgItemText(IDC_PROXY_SERVER,m_ProxyServer.szProxyServer);

	//代理端口
	if (m_ProxyServer.wProxyPort==0)
	{
		SetDlgItemText(IDC_PROXY_PORT,TEXT(""));
	}
	else
	{
		SetDlgItemInt(IDC_PROXY_PORT,m_ProxyServer.wProxyPort);
	}

	//设置选择
	for (INT i=0;i<m_cmProxyType.GetCount();i++)
	{
		if (m_cmProxyType.GetItemData(i)==m_cbProxyType)
		{
			m_cmProxyType.SetCurSel(i);
		}
	}

	//默认选择
	if (i==m_cmProxyType.GetCount())
	{
		m_cmProxyType.SetCurSel(0);
	}

	return true;
}

//代理测试
VOID CCollocateProxy::OnBnClickedProxyTest()
{
	//代理类型
	BYTE cbProxyType=(BYTE)m_cmProxyType.GetItemData(m_cmProxyType.GetCurSel());

	//代理信息
	tagProxyServer ProxyServer;
	ZeroMemory(&ProxyServer,sizeof(ProxyServer));
	ProxyServer.wProxyPort=GetDlgItemInt(IDC_PROXY_PORT);
	GetDlgItemText(IDC_PROXY_USER,ProxyServer.szUserName,CountArray(ProxyServer.szUserName));
	GetDlgItemText(IDC_PROXY_PASS,ProxyServer.szPassword,CountArray(ProxyServer.szPassword));
	GetDlgItemText(IDC_PROXY_SERVER,ProxyServer.szProxyServer,CountArray(ProxyServer.szProxyServer));

	//效验代理
	if (cbProxyType!=PROXY_NONE)
	{
		//代理地址
		if (ProxyServer.szProxyServer[0]==0)
		{
			//错误提示
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("代理服务器地址不能为空，请重新输入！"),MB_ICONERROR,30);

			//设置焦点
			m_edProxyServer.SetFocus();

			return;
		}

		//代理端口
		if (ProxyServer.wProxyPort==0)
		{
			//错误提示
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("请输入代理服务器端口号，例如：1080！"),MB_ICONERROR,30);

			//设置焦点
			m_edProxyPort.SetFocus();

			return;
		}
	}
	else 
	{
		//错误提示
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("请先选择代理服务器类型与代理服务器连接信息！"),MB_ICONERROR,30);

		//设置焦点
		m_cmProxyType.SetFocus();

		return;
	}

	//创建组件
	CWHNetworkHelper WHNetworkModule;
	if (WHNetworkModule.CreateInstance()==false)
	{
		ShowInformation(TEXT("网络服务管理组件创建失败，测试失败！"),MB_ICONERROR,30);
		return;
	}

	//代理测试
	switch (WHNetworkModule->ProxyServerTesting(cbProxyType,ProxyServer))
	{
	case CONNECT_SUCCESS:				//连接成功
		{
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("代理服务器工作正常！"),MB_ICONINFORMATION);
			return;
		}
	case CONNECT_PROXY_USER_INVALID:	//用户错误
		{
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("代理服务器用户名或者密码错误！"),MB_ICONERROR);
			return;
		}
	default:							//默认处理
		{
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("代理服务器不存在或者连接失败！"),MB_ICONERROR);
			return;
		}
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CCollocateNotify::CCollocateNotify() : CCollocateItem(IDD_COLLOCATE_NOTIFY)
{
	//默认参数
	DefaultParameter();

	//设置变量
	m_cbImageIndex=IMAGE_INDEX_NOTIFY;

	return;
}

//析构函数
CCollocateNotify::~CCollocateNotify()
{
}

//保存参数
bool CCollocateNotify::SaveParameter()
{
	//聊天设置
	if (IsDlgButtonChecked(IDC_MESSAGE_SHOW_ALL)==BST_CHECKED) m_cbMessageMode=INVITE_MODE_ALL;
	if (IsDlgButtonChecked(IDC_MESSAGE_HIDE_DETEST)==BST_CHECKED) m_cbMessageMode=MESSAGE_MODE_DETEST;
	if (IsDlgButtonChecked(IDC_MESSAGE_ONLY_FRIEND)==BST_CHECKED) m_cbMessageMode=MESSAGE_MODE_FRIEND;

	//邀请设置
	if (IsDlgButtonChecked(IDC_INVITE_ALL)==BST_CHECKED) m_cbInviteMode=INVITE_MODE_ALL;
	if (IsDlgButtonChecked(IDC_INVITE_NONE)==BST_CHECKED) m_cbInviteMode=INVITE_MODE_NONE;
	if (IsDlgButtonChecked(IDC_INVITE_ONLY_FRIEND)==BST_CHECKED) m_cbInviteMode=INVITE_MODE_FRIEND;

	//私聊设置
	m_bAutoShowWhisper=(IsDlgButtonChecked(IDC_AUTO_SHOW_WHISPER)==BST_CHECKED);
	m_wWhisperHotKey=(WORD)((CHotKeyCtrl *)GetDlgItem(IDC_WHISPER_HOTKEY))->GetHotKey();

	return true;
}

//默认参数
bool CCollocateNotify::DefaultParameter()
{
	//配置变量
	m_bAutoShowWhisper=false;
	m_wWhisperHotKey=MAKEWORD(VK_F11,0);

	//枚举变量
	m_cbInviteMode=INVITE_MODE_ALL;
	m_cbMessageMode=MESSAGE_MODE_DETEST;

	return true;
}

//更新控制
bool CCollocateNotify::UpdateControlStatus()
{
	//聊天设置
	CheckDlgButton(IDC_MESSAGE_SHOW_ALL,(m_cbMessageMode==INVITE_MODE_ALL)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_MESSAGE_HIDE_DETEST,(m_cbMessageMode==MESSAGE_MODE_DETEST)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_MESSAGE_ONLY_FRIEND,(m_cbMessageMode==MESSAGE_MODE_FRIEND)?BST_CHECKED:BST_UNCHECKED);

	//邀请设置
	CheckDlgButton(IDC_INVITE_ALL,(m_cbInviteMode==INVITE_MODE_ALL)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_INVITE_NONE,(m_cbInviteMode==INVITE_MODE_NONE)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_INVITE_ONLY_FRIEND,(m_cbInviteMode==INVITE_MODE_FRIEND)?BST_CHECKED:BST_UNCHECKED);

	//私聊设置
	CheckDlgButton(IDC_AUTO_SHOW_WHISPER,(m_bAutoShowWhisper==true)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_SHOW_WHISPER_BY_HOTKEY,(m_bAutoShowWhisper==false)?BST_CHECKED:BST_UNCHECKED);
	((CHotKeyCtrl *)GetDlgItem(IDC_WHISPER_HOTKEY))->SetHotKey(LOBYTE(m_wWhisperHotKey),HIBYTE(m_wWhisperHotKey));

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CCollocateCustom::CCollocateCustom() : CCollocateItem(IDD_COLLOCATE_CUSTOM)
{
	//默认参数
	DefaultParameter();

	//设置变量
	m_cbImageIndex=IMAGE_INDEX_CUSTOM;

	return;
}

//析构函数
CCollocateCustom::~CCollocateCustom()
{
}

//保存参数
bool CCollocateCustom::SaveParameter()
{
	//双击列表
	if (IsDlgButtonChecked(IDC_ACTION_ORIENTATION)==BST_CHECKED) m_cbActionLeftDoubleList=ACTION_ORIENTATION;
	if (IsDlgButtonChecked(IDC_ACTION_SEND_WHISPER)==BST_CHECKED) m_cbActionLeftDoubleList=ACTION_SEND_WHISPER;
	if (IsDlgButtonChecked(IDC_ACTION_SHOW_USER_INFO)==BST_CHECKED) m_cbActionLeftDoubleList=ACTION_SHOW_USER_INFO;

	//自动加入
	if (IsDlgButtonChecked(IDC_ACTION_SEARCH_TABLE)==BST_CHECKED) m_cbActionHitAutoJoin=ACTION_SEARCH_TABLE;
	if (IsDlgButtonChecked(IDC_SHOW_SEARCH_TABLE_DLG)==BST_CHECKED) m_cbActionHitAutoJoin=ACTION_SHOW_SEARCH_DLG;

	//发送私聊
	if (IsDlgButtonChecked(IDC_SNED_WHISPER_BY_ENTER)==BST_CHECKED) m_bSendWhisperByEnter=true;
	if (IsDlgButtonChecked(IDC_SNED_WHISPER_BY_CTRL_ENTER)==BST_CHECKED) m_bSendWhisperByEnter=false;

	return true;
}

//默认参数
bool CCollocateCustom::DefaultParameter()
{
	//配置变量
	m_bSendWhisperByEnter=true;

	//动作定义
	m_cbActionHitAutoJoin=ACTION_SEARCH_TABLE;
	m_cbActionLeftDoubleList=ACTION_SEND_WHISPER;

	return true;
}

//更新控制
bool CCollocateCustom::UpdateControlStatus()
{
	//双击列表
	CheckDlgButton(IDC_ACTION_ORIENTATION,(m_cbActionLeftDoubleList==ACTION_ORIENTATION)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_ACTION_SEND_WHISPER,(m_cbActionLeftDoubleList==ACTION_SEND_WHISPER)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_ACTION_SHOW_USER_INFO,(m_cbActionLeftDoubleList==ACTION_SHOW_USER_INFO)?BST_CHECKED:BST_UNCHECKED);

	//自动加入
	CheckDlgButton(IDC_ACTION_SEARCH_TABLE,(m_cbActionHitAutoJoin==ACTION_SEARCH_TABLE)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_SHOW_SEARCH_TABLE_DLG,(m_cbActionHitAutoJoin==ACTION_SHOW_SEARCH_DLG)?BST_CHECKED:BST_UNCHECKED);

	//发送私聊
	CheckDlgButton(IDC_SNED_WHISPER_BY_ENTER,(m_bSendWhisperByEnter==true)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_SNED_WHISPER_BY_CTRL_ENTER,(m_bSendWhisperByEnter==false)?BST_CHECKED:BST_UNCHECKED);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CCollocateSystem::CCollocateSystem() : CCollocateItem(IDD_COLLOCATE_SYSTEM)
{
	//默认参数
	DefaultParameter();

	//设置变量
	m_cbImageIndex=IMAGE_INDEX_SYSTEM;

	return;
}

//析构函数
CCollocateSystem::~CCollocateSystem()
{
}

//保存参数
bool CCollocateSystem::SaveParameter()
{
	//保存设置
	m_bAllowSound=(IsDlgButtonChecked(IDC_ALLOW_SOUND)==BST_CHECKED);
	m_bAutoSitDown=(IsDlgButtonChecked(IDC_AUTO_SIT_DOWN)==BST_CHECKED);
	m_bSalienceTable=(IsDlgButtonChecked(IDC_SALIENCE_TABLE)==BST_CHECKED);
	m_bSaveWhisperChat=(IsDlgButtonChecked(IDC_AUTO_SAVE_WHISPER)==BST_CHECKED);
	m_bNotifyUserInOut=(IsDlgButtonChecked(IDC_NOTIFY_USER_INOUT)==BST_CHECKED);
	m_bNotifyFriendCome=(IsDlgButtonChecked(IDC_NOTIFY_FRIEND_COME)==BST_CHECKED);
	m_bFullScreenBrowse=(IsDlgButtonChecked(IDC_FULL_SCREEN_BROWSE)==BST_CHECKED);

	//老板键
	m_wBossHotKey=(WORD)((CHotKeyCtrl *)GetDlgItem(IDC_BOSS_HOTKEY))->GetHotKey();

	return true;
}

//默认参数
bool CCollocateSystem::DefaultParameter()
{
	//配置变量
	m_wBossHotKey=MAKEWORD(VK_F12,HOTKEYF_CONTROL);

	//配置变量
	m_bAllowSound=true;
	m_bAutoSitDown=true;
	m_bSalienceTable=true;
	m_bSaveWhisperChat=true;
	m_bNotifyUserInOut=false;
	m_bNotifyFriendCome=true;
	m_bFullScreenBrowse=false;

	return true;
}

//更新控制
bool CCollocateSystem::UpdateControlStatus()
{
	//设置控件
	CheckDlgButton(IDC_ALLOW_SOUND,(m_bAllowSound==true)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_AUTO_SIT_DOWN,(m_bAutoSitDown==true)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_SALIENCE_TABLE,(m_bSalienceTable==true)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_AUTO_SAVE_WHISPER,(m_bSaveWhisperChat==true)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_NOTIFY_USER_INOUT,(m_bNotifyUserInOut==true)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_NOTIFY_FRIEND_COME,(m_bNotifyFriendCome==true)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_FULL_SCREEN_BROWSE,(m_bFullScreenBrowse==true)?BST_CHECKED:BST_UNCHECKED);
	
	//老板键
	((CHotKeyCtrl *)GetDlgItem(IDC_BOSS_HOTKEY))->SetHotKey(LOBYTE(m_wBossHotKey),HIBYTE(m_wBossHotKey));

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgCollocate::CDlgCollocate() : CSkinDialog(IDD_COLLOCATE)
{
	//配置变量
	m_pParameterGame=NULL;
	m_pParameterServer=NULL;

	return;
}

//析构函数
CDlgCollocate::~CDlgCollocate()
{
}

//配置参数
bool CDlgCollocate::InitCollocate(CParameterGame * pParameterGame, CParameterServer * pParameterServer)
{
	//设置变量
	m_pParameterGame=pParameterGame;
	m_pParameterServer=pParameterServer;

	return true;
}

//控件绑定
VOID CDlgCollocate::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_DEFAULT, m_btDefault);
}

//创建函数
BOOL CDlgCollocate::OnInitDialog()
{
	__super::OnInitDialog();

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//设置标题
	SetWindowText(TEXT("系统设置"));

	//变量定义
	INT nTViewBorder=m_SkinAttribute.m_EncircleInfoView.nTBorder;
	INT nBViewBorder=m_SkinAttribute.m_EncircleInfoView.nBBorder;
	INT nLViewBorder=m_SkinAttribute.m_EncircleInfoView.nLBorder;
	INT nRViewBorder=m_SkinAttribute.m_EncircleInfoView.nRBorder;
	INT nBFrameBorder=m_SkinAttribute.m_EncircleInfoFrame.nBBorder;
	
	//创建控制
	m_CollocateBar.Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this,IDC_COLLOCATE_BAR);
	m_CollocateBar.SetWindowPos(NULL,nLViewBorder,nTViewBorder,COLLOCATE_BAR_CX,COLLOCATE_BAR_CY,SWP_NOZORDER);

	//子项位置
	CRect rcItemCreate;
	rcItemCreate.top=nTViewBorder;
	rcItemCreate.left=nLViewBorder+COLLOCATE_BAR_CX;
	rcItemCreate.right=rcClient.Width()-nRViewBorder;
	rcItemCreate.bottom=nTViewBorder+COLLOCATE_BAR_CY;
	m_CollocateBar.SetItemCreateRect(rcItemCreate);

	//移动按钮
	CRect rcButton;
	m_btOk.GetWindowRect(&rcButton);

	//移动窗口
	rcClient.bottom=nTViewBorder+nBFrameBorder+COLLOCATE_BAR_CY;
	SetWindowPos(NULL,0,0,rcClient.Width(),rcClient.Height(),SWP_NOZORDER|SWP_NOMOVE);

	//移动按钮
	INT nYButtonPos=rcClient.Height()-rcButton.Height()-nBViewBorder-4;
	m_btOk.SetWindowPos(NULL,rcClient.Width()-rcButton.Width()*3-nRViewBorder-18,nYButtonPos,0,0,SWP_NOZORDER|SWP_NOSIZE);
	m_btCancel.SetWindowPos(NULL,rcClient.Width()-rcButton.Width()-nRViewBorder-8,nYButtonPos,0,0,SWP_NOZORDER|SWP_NOSIZE);
	m_btDefault.SetWindowPos(NULL,rcClient.Width()-rcButton.Width()*2-nRViewBorder-13,nYButtonPos,0,0,SWP_NOZORDER|SWP_NOSIZE);

	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//代理选项
	m_CollocateProxy.m_cbProxyType=pParameterGlobal->m_cbProxyType;
	m_CollocateProxy.m_ProxyServer=pParameterGlobal->m_ProxyServer;

	//消息设置
	m_CollocateNotify.m_cbInviteMode=pParameterGlobal->m_cbInviteMode;
	m_CollocateNotify.m_cbMessageMode=pParameterGlobal->m_cbMessageMode;
	m_CollocateNotify.m_wWhisperHotKey=pParameterGlobal->m_wWhisperHotKey;
	m_CollocateNotify.m_bAutoShowWhisper=pParameterGlobal->m_bAutoShowWhisper;

	//系统配置
	m_CollocateSystem.m_wBossHotKey=pParameterGlobal->m_wBossHotKey;
	m_CollocateSystem.m_bAllowSound=pParameterGlobal->m_bAllowSound;
	m_CollocateSystem.m_bAutoSitDown=pParameterGlobal->m_bAutoSitDown;
	m_CollocateSystem.m_bSalienceTable=pParameterGlobal->m_bSalienceTable;
	m_CollocateSystem.m_bSaveWhisperChat=pParameterGlobal->m_bSaveWhisperChat;
	m_CollocateSystem.m_bNotifyUserInOut=pParameterGlobal->m_bNotifyUserInOut;
	m_CollocateSystem.m_bNotifyFriendCome=pParameterGlobal->m_bNotifyFriendCome;
	m_CollocateSystem.m_bFullScreenBrowse=pParameterGlobal->m_bFullScreenBrowse;

	//操作定制
	m_CollocateCustom.m_cbActionHitAutoJoin=pParameterGlobal->m_cbActionHitAutoJoin;
	m_CollocateCustom.m_bSendWhisperByEnter=pParameterGlobal->m_bSendWhisperByEnter;
	m_CollocateCustom.m_cbActionLeftDoubleList=pParameterGlobal->m_cbActionLeftDoubleList;

	//全局设置
	m_CollocateBar.InsertCollocateItem(&m_CollocateNotify);
	m_CollocateBar.InsertCollocateItem(&m_CollocateSystem);
	m_CollocateBar.InsertCollocateItem(&m_CollocateCustom);
	m_CollocateBar.InsertCollocateItem(&m_CollocateProxy);

	//房间设置
	if ((m_pParameterServer!=NULL)&&(m_pParameterGame!=NULL))
	{
		//游戏配置
		m_CollocateServer.m_wMinWinRate=m_pParameterGame->m_wMinWinRate;
		m_CollocateServer.m_wMaxFleeRate=m_pParameterGame->m_wMaxFleeRate;
		m_CollocateServer.m_bLimitWinRate=m_pParameterGame->m_bLimitWinRate;
		m_CollocateServer.m_lMaxGameScore=m_pParameterGame->m_lMaxGameScore;
		m_CollocateServer.m_lMinGameScore=m_pParameterGame->m_lMinGameScore;
		m_CollocateServer.m_bLimitFleeRate=m_pParameterGame->m_bLimitFleeRate;
		m_CollocateServer.m_bLimitGameScore=m_pParameterGame->m_bLimitGameScore;

		//其他配置
		m_CollocateServer.m_bLimitDetest=pParameterGlobal->m_bLimitDetest;
		m_CollocateServer.m_bLimitSameIP=pParameterGlobal->m_bLimitSameIP;

		//房间配置
		m_CollocateServer.m_bTakePassword=m_pParameterServer->m_bTakePassword;
		lstrcpyn(m_CollocateServer.m_szPassword,m_pParameterServer->m_szPassword,CountArray(m_CollocateServer.m_szPassword));

		//插入子项
		m_CollocateBar.InsertCollocateItem(&m_CollocateServer);
	}

	//激活选项
	if ((m_pParameterServer!=NULL)&&(m_pParameterGame!=NULL))
	{
		m_CollocateBar.SetActiveCollocateItem(&m_CollocateServer);
	}
	else
	{
		m_CollocateBar.SetActiveCollocateItem(&m_CollocateNotify);
	}

	return TRUE;
}

//确定函数
VOID CDlgCollocate::OnOK()
{
	//保存参数
	if (m_CollocateBar.SaveParameter()==false) return;

	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//老板热键
	UnregisterHotKey(AfxGetMainWnd()->GetSafeHwnd(),IDI_HOT_KEY_BOSS);
	if (CWHService::RegisterHotKey(AfxGetMainWnd()->GetSafeHwnd(),IDI_HOT_KEY_BOSS,m_CollocateSystem.m_wBossHotKey)==false)
	{
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("系统热键冲突，老板热键注册失败！"));
	}

	//私聊热键
	UnregisterHotKey(AfxGetMainWnd()->GetSafeHwnd(),IDI_HOT_KEY_WHISPER);
	if (CWHService::RegisterHotKey(AfxGetMainWnd()->GetSafeHwnd(),IDI_HOT_KEY_WHISPER,m_CollocateNotify.m_wWhisperHotKey)==false)
	{
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("系统热键冲突，提取消息热键注册失败！"));
	}

	//同桌规则
	if ((m_pParameterServer!=NULL)&&(m_pParameterGame!=NULL))
	{
		//其他配置
		pParameterGlobal->m_bLimitDetest=m_CollocateServer.m_bLimitDetest;
		pParameterGlobal->m_bLimitSameIP=m_CollocateServer.m_bLimitSameIP;

		//游戏配置
		m_pParameterGame->m_wMinWinRate=m_CollocateServer.m_wMinWinRate;
		m_pParameterGame->m_wMaxFleeRate=m_CollocateServer.m_wMaxFleeRate;
		m_pParameterGame->m_lMaxGameScore=m_CollocateServer.m_lMaxGameScore;
		m_pParameterGame->m_lMinGameScore=m_CollocateServer.m_lMinGameScore;
		m_pParameterGame->m_bLimitWinRate=m_CollocateServer.m_bLimitWinRate;
		m_pParameterGame->m_bLimitFleeRate=m_CollocateServer.m_bLimitFleeRate;
		m_pParameterGame->m_bLimitGameScore=m_CollocateServer.m_bLimitGameScore;

		//房间配置
		m_pParameterServer->m_bTakePassword=m_CollocateServer.m_bTakePassword;
		lstrcpyn(m_pParameterServer->m_szPassword,m_CollocateServer.m_szPassword,CountArray(m_pParameterServer->m_szPassword));
	}

	//代理选项
	pParameterGlobal->m_cbProxyType=m_CollocateProxy.m_cbProxyType;
	pParameterGlobal->m_ProxyServer=m_CollocateProxy.m_ProxyServer;

	//消息设置
	pParameterGlobal->m_cbInviteMode=m_CollocateNotify.m_cbInviteMode;
	pParameterGlobal->m_cbMessageMode=m_CollocateNotify.m_cbMessageMode;
	pParameterGlobal->m_wWhisperHotKey=m_CollocateNotify.m_wWhisperHotKey;
	pParameterGlobal->m_bAutoShowWhisper=m_CollocateNotify.m_bAutoShowWhisper;

	//系统配置
	pParameterGlobal->m_wBossHotKey=m_CollocateSystem.m_wBossHotKey;
	pParameterGlobal->m_bAllowSound=m_CollocateSystem.m_bAllowSound;
	pParameterGlobal->m_bAutoSitDown=m_CollocateSystem.m_bAutoSitDown;
	pParameterGlobal->m_bSalienceTable=m_CollocateSystem.m_bSalienceTable;
	pParameterGlobal->m_bSaveWhisperChat=m_CollocateSystem.m_bSaveWhisperChat;
	pParameterGlobal->m_bNotifyUserInOut=m_CollocateSystem.m_bNotifyUserInOut;
	pParameterGlobal->m_bNotifyFriendCome=m_CollocateSystem.m_bNotifyFriendCome;
	pParameterGlobal->m_bFullScreenBrowse=m_CollocateSystem.m_bFullScreenBrowse;

	//操作定制
	pParameterGlobal->m_cbActionHitAutoJoin=m_CollocateCustom.m_cbActionHitAutoJoin;
	pParameterGlobal->m_bSendWhisperByEnter=m_CollocateCustom.m_bSendWhisperByEnter;
	pParameterGlobal->m_cbActionLeftDoubleList=m_CollocateCustom.m_cbActionLeftDoubleList;

	__super::OnOK();
}

//默认按钮
VOID CDlgCollocate::OnBnClickedDefault()
{
	//默认参数
	m_CollocateBar.DefaultParameter();

	return;
}

//////////////////////////////////////////////////////////////////////////////////
