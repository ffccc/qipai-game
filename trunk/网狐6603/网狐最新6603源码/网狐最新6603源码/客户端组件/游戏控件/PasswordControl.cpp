#include "StdAfx.h"
#include "Winable.h"
#include "Resource.h"
#include "PasswordControl.h"

//////////////////////////////////////////////////////////////////////////////////

//位置定义
#define ITEM_POS_S					3									//按钮间距
#define ITEM_POS_X					8									//按钮位置
#define ITEM_POS_Y					8									//按钮位置

//关闭按钮
#define POS_BUTTON_X				5									//按钮位置
#define POS_BUTTON_Y				8									//按钮位置

//常量定义
#define ROW_BACK					0									//退格按钮
#define ROW_SHIFT					1									//切换按钮
#define ROW_CAPITAL					2									//大写按钮
#define ROW_CLOSE_KEY				3									//关闭按钮
#define LINE_FUNCTION				4									//功能按钮

//控件标识
#define IDC_BT_KEYBOARD				100									//键盘按钮
#define IDC_ED_PASSWORD				200									//密码控件

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPasswordControl, CWnd)
	ON_WM_PAINT()
	ON_WM_NCPAINT()
	ON_WM_SETFOCUS()
	ON_WM_ERASEBKGND()
	ON_EN_CHANGE(IDC_ED_PASSWORD, OnEnChangePassword)
	ON_BN_CLICKED(IDC_BT_KEYBOARD, OnBnClickedKeyboard)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CPasswordKeyboard, CDialog)
	ON_WM_NCPAINT()
	ON_WM_KILLFOCUS()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CPasswordControl::CPasswordControl()
{
	//设置变量
	m_bModify=false;
	m_bFalsity=false;
	m_bDrawBorad=true;
	m_bRenderImage=true;
	ZeroMemory(m_szPassword,sizeof(m_szPassword));

	return;
}

//析构函数
CPasswordControl::~CPasswordControl()
{
}

//绑定函数
VOID CPasswordControl::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	//获取位置
	CRect rcClient;
	CRect rcWindow;
	GetClientRect(&rcClient);
	GetWindowRect(&rcWindow);

	//创建按钮
	CRect rcButton(0,0,0,0);
	m_btKeyboard.Create(NULL,WS_CHILD|WS_VISIBLE,rcButton,this,IDC_BT_KEYBOARD);
	m_btKeyboard.SetButtonImage(IDB_BT_KEYBOARD,GetModuleHandle(SHARE_CONTROL_DLL_NAME),false,false);

	//创建控件
	CRect rcEditCreate;
	rcEditCreate.top=4;
	rcEditCreate.left=4;
	rcEditCreate.bottom=16;
	rcEditCreate.right=rcClient.Width()-23;
	m_edPassword.Create(WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL|ES_PASSWORD,rcEditCreate,this,IDC_ED_PASSWORD);

	//设置控件
	m_edPassword.LimitText(LEN_PASSWORD-1);
	m_edPassword.SetEnableColor(RGB(0,0,0),RGB(255,255,255),RGB(92,100,105));
	m_edPassword.SetFont(&CSkinResourceManager::GetInstance()->GetDefaultFont());

	//移动窗口
	SetWindowPos(NULL,0,0,rcWindow.Width(),24,SWP_NOMOVE|SWP_NOZORDER|SWP_NOCOPYBITS);
	m_btKeyboard.SetWindowPos(NULL,rcClient.Width()-20,3,0,0,SWP_NOSIZE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOACTIVATE);

	return;
}

//边框设置
VOID CPasswordControl::SetDrawBorad(bool bDrawBorad)
{
	//设置变量
	m_bDrawBorad=bDrawBorad;

	//更新界面
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//设置密码
VOID CPasswordControl::SetUserPassword(LPCTSTR pszPassword)
{
	//设置控件
	INT nPasswordLen=lstrlen(pszPassword);
	m_edPassword.SetWindowText((nPasswordLen>0)?TEXT("********"):TEXT(""));

	//设置变量
	m_bModify=false;
	m_bFalsity=true;
	lstrcpyn(m_szPassword,pszPassword,CountArray(m_szPassword));

	return;
}

//获取密码
LPCTSTR CPasswordControl::GetUserPassword(TCHAR szPassword[LEN_PASSWORD])
{
	//控件密码
	if (m_bModify==true)
	{
		m_bModify=false;
		m_edPassword.GetWindowText(m_szPassword,CountArray(m_szPassword));
	}

	//拷贝密码
	lstrcpyn(szPassword,m_szPassword,LEN_PASSWORD);

	return szPassword;
}

//重画消息
VOID CPasswordControl::OnPaint()
{
	CPaintDC dc(this);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//绘画背景
	if (IsWindowEnabled()==TRUE)
	{
		dc.FillSolidRect(rcClient,CSkinEdit::m_SkinAttribute.m_crEnableBK);
	}
	else
	{
		dc.FillSolidRect(rcClient,CSkinDialog::m_SkinAttribute.m_crBackGround);
	}

	return;
}

//重画消息
VOID CPasswordControl::OnNcPaint()
{
	//获取位置
	CRect rcWindow;
	GetWindowRect(&rcWindow);
	ScreenToClient(&rcWindow);
	
	//颜色定义
	COLORREF crBackGround=CSkinEdit::m_SkinAttribute.m_crEnableBK;
	COLORREF crFrameBorad=CSkinEdit::m_SkinAttribute.m_crEnableBorad;

	//禁用状态
	if (IsWindowEnabled()==FALSE)
	{
		if (m_bRenderImage==false)
		{
			crBackGround=CSkinEdit::m_SkinAttribute.m_crDisableBK;
			crFrameBorad=CSkinEdit::m_SkinAttribute.m_crDisableBorad;
		}
		else
		{
			crBackGround=CSkinEdit::m_SkinAttribute.m_crDisableBK;
			crFrameBorad=CSkinEdit::m_SkinAttribute.m_crDisableBorad;
		}
	}
	else
	{
		crBackGround=CSkinEdit::m_SkinAttribute.m_crEnableBK;
		crFrameBorad=CSkinEdit::m_SkinAttribute.m_crEnableBorad;
	}

	//边框判断
	if (m_bDrawBorad==false)
	{
		crFrameBorad=crBackGround;
	}

	//绘画边框
	CClientDC ClientDC(this);
	ClientDC.Draw3dRect(rcWindow.left,rcWindow.top,rcWindow.Width(),rcWindow.Height(),crFrameBorad,crFrameBorad);
	ClientDC.Draw3dRect(rcWindow.left+1,rcWindow.top+1,rcWindow.Width()-2,rcWindow.Height()-2,crBackGround,crBackGround);

	return;

}

//绘画背景
BOOL CPasswordControl::OnEraseBkgnd(CDC * pDC)
{
	return TRUE;
}

//获取焦点
VOID CPasswordControl::OnSetFocus(CWnd * pOldWnd)
{
	__super::OnSetFocus(pOldWnd);

	//设置焦点
	m_edPassword.SetFocus();

	//设置选择
	m_edPassword.SetSel(0,-1,FALSE);

	return;
}

//密码改变
VOID CPasswordControl::OnEnChangePassword()
{
	//设置变量
	m_bModify=true;
	m_bFalsity=false;

	//发送消息
	CWnd * pParent=GetParent();
	if (pParent!=NULL) pParent->SendMessage(WM_COMMAND,MAKELONG(GetWindowLong(m_hWnd,GWL_ID),EN_CHANGE),(LPARAM)m_hWnd);

	return;
}

//键盘按钮
VOID CPasswordControl::OnBnClickedKeyboard()
{
	//获取位置
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//密码清理
	if (m_bFalsity==true)
	{
		m_bFalsity=false;
		m_edPassword.SetWindowText(TEXT(""));
	}

	//创建键盘
	if (m_PasswordKeyboard.m_hWnd==NULL)
	{
		m_PasswordKeyboard.Create(IDD_PASSWORD_KEYBOARD,this);
	}

	//显示窗口
	UINT uFlags=SWP_NOZORDER|SWP_NOSIZE|SWP_SHOWWINDOW;
	m_PasswordKeyboard.SetWindowPos(NULL,rcWindow.left,rcWindow.bottom,0,0,uFlags);

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CPasswordKeyboard::CPasswordKeyboard() : CDialog(IDD_PASSWORD_KEYBOARD)
{
	//设置变量
	m_nRandLine[0]=rand()%11;
	m_nRandLine[1]=rand()%10;
	m_nRandLine[2]=rand()%13;
	m_nRandLine[3]=rand()%13;

	//键盘状态
	m_bShiftStatus=false;
	m_bCapsLockStatus=false;

	//状态变量
	m_bMouseDown=false;
	m_wHoverRow=INVALID_WORD;
	m_wHoverLine=INVALID_WORD;

	//键盘字符
	m_nItemCount[0]=11;
	lstrcpyn(m_szKeyboradChar[0][0],TEXT("`1234567890"),CountArray(m_szKeyboradChar[0][0]));
	lstrcpyn(m_szKeyboradChar[0][1],TEXT("~!@#$%^&*()"),CountArray(m_szKeyboradChar[0][1]));

	//键盘字符
	m_nItemCount[1]=10;
	lstrcpyn(m_szKeyboradChar[1][0],TEXT("-=[]\\;',./"),CountArray(m_szKeyboradChar[1][0]));
	lstrcpyn(m_szKeyboradChar[1][1],TEXT("_+{}|:\"<>?"),CountArray(m_szKeyboradChar[1][1]));

	//键盘字符
	m_nItemCount[2]=13;
	lstrcpyn(m_szKeyboradChar[2][0],TEXT("abcdefghijklm"),CountArray(m_szKeyboradChar[2][0]));
	lstrcpyn(m_szKeyboradChar[2][1],TEXT("ABCDEFGHIJKLM"),CountArray(m_szKeyboradChar[2][1]));

	//键盘字符
	m_nItemCount[3]=13;
	lstrcpyn(m_szKeyboradChar[3][0],TEXT("nopqrstuvwxyz"),CountArray(m_szKeyboradChar[3][0]));
	lstrcpyn(m_szKeyboradChar[3][1],TEXT("NOPQRSTUVWXYZ"),CountArray(m_szKeyboradChar[3][1]));

	//变量定义
	HINSTANCE hResInstance=GetModuleHandle(SHARE_CONTROL_DLL_NAME);

	//加载资源
	CImage ImageItem1;
	CImage ImageItem2;
	CImage ImageItem3;
	ImageItem1.LoadFromResource(hResInstance,IDB_KEYBOARD_ITEM1);
	ImageItem2.LoadFromResource(hResInstance,IDB_KEYBOARD_ITEM2);
	ImageItem3.LoadFromResource(hResInstance,IDB_KEYBOARD_ITEM3);

	//设置大小
	m_SizeImageItem1.SetSize(ImageItem1.GetWidth()/3,ImageItem1.GetHeight());
	m_SizeImageItem2.SetSize(ImageItem2.GetWidth()/3,ImageItem2.GetHeight());
	m_SizeImageItem3.SetSize(ImageItem3.GetWidth()/3,ImageItem3.GetHeight());

	//关闭按钮
	CImage ImageButton;
	ImageButton.LoadFromResource(hResInstance,IDB_BT_KEYBOARD_CLOSE);
	m_SizeImageButton.SetSize(ImageButton.GetWidth()/3,ImageButton.GetHeight());

	//背景大小
	CImage ImageBackGround;
	ImageBackGround.LoadFromResource(hResInstance,IDB_PASSWORD_KEYBORAD_BK);
	m_SizeBackGround.SetSize(ImageBackGround.GetWidth(),ImageBackGround.GetHeight());

	return;
}

//析构函数
CPasswordKeyboard::~CPasswordKeyboard()
{
}

//消息解释
BOOL CPasswordKeyboard::PreTranslateMessage(MSG * pMsg)
{
	//大写锁键
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_CAPITAL))
	{
		//设置变量
		bool bCapsLockStatus=m_bCapsLockStatus;
		m_bCapsLockStatus=(GetKeyState(VK_CAPITAL)&0x0F)>0;

		//更新界面
		if (bCapsLockStatus!=m_bCapsLockStatus)
		{
			RedrawWindow(NULL,NULL,RDW_FRAME|RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASENOW);
		}

		return TRUE;
	}

	//切换按钮
	if ((pMsg->wParam==VK_SHIFT)&&(pMsg->message==WM_KEYUP)||(pMsg->message==WM_KEYDOWN))
	{
		//设置变量
		bool bShiftStatus=m_bShiftStatus;
		m_bShiftStatus=(GetKeyState(VK_SHIFT)&0xF0)>0;

		//更新界面
		if (bShiftStatus!=m_bShiftStatus)
		{
			RedrawWindow(NULL,NULL,RDW_FRAME|RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASENOW);
		}

		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//创建函数
BOOL CPasswordKeyboard::OnInitDialog()
{
	__super::OnInitDialog();

	//设置变量
	m_bMouseDown=false;
	m_wHoverRow=INVALID_WORD;
	m_wHoverLine=INVALID_WORD;

	//获取状态
	m_bShiftStatus=(GetKeyState(VK_SHIFT)&0xF0)>0;
	m_bCapsLockStatus=(GetKeyState(VK_CAPITAL)&0x0F)>0;

	//构造位置
	CRect rcWindow;
	rcWindow.SetRect(0,0,m_SizeBackGround.cx,m_SizeBackGround.cy);

	//移动窗口
	CalcWindowRect(&rcWindow,CWnd::adjustBorder);
	SetWindowPos(NULL,0,0,rcWindow.Width(),rcWindow.Height(),SWP_NOMOVE|SWP_NOZORDER);

	return FALSE;
}

//更新位置
VOID CPasswordKeyboard::SetCurrentStation(CPoint MousePoint)
{
	//变量定义
	WORD wHoverRow=INVALID_WORD;
	WORD wHoverLine=INVALID_WORD;

	//字符按钮
	if ((MousePoint.x>=ITEM_POS_X)&&(MousePoint.y>=ITEM_POS_Y))
	{
		//列数计算
		if (((MousePoint.x-ITEM_POS_X)%(m_SizeImageItem1.cx+ITEM_POS_S))<=m_SizeImageItem1.cx)
		{
			wHoverRow=(WORD)((MousePoint.x-ITEM_POS_X)/(m_SizeImageItem1.cx+ITEM_POS_S));
		}

		//行数计算
		if (((MousePoint.y-ITEM_POS_Y)%(m_SizeImageItem1.cy+ITEM_POS_S))<=m_SizeImageItem1.cy)
		{
			wHoverLine=(WORD)((MousePoint.y-ITEM_POS_Y)/(m_SizeImageItem1.cy+ITEM_POS_S));
		}

		//参数调整
		if (wHoverLine>=CountArray(m_nItemCount)) wHoverLine=INVALID_WORD;
		if ((wHoverLine==INVALID_WORD)||(wHoverRow>=m_nItemCount[wHoverLine])) wHoverRow=INVALID_WORD;
	}

	//功能按钮
	if ((wHoverLine<2)&&(wHoverRow==INVALID_WORD))
	{
		//变量定义
		INT nEndLine1=ITEM_POS_X+(m_SizeImageItem1.cx+ITEM_POS_S)*m_nItemCount[0];
		INT nEndLine2=ITEM_POS_X+(m_SizeImageItem1.cx+ITEM_POS_S)*m_nItemCount[1];

		//原点位置
		CPoint ButtonPoint[4];
		ButtonPoint[0].SetPoint(nEndLine1,ITEM_POS_Y);
		ButtonPoint[1].SetPoint(nEndLine2,ITEM_POS_Y+ITEM_POS_S+m_SizeImageItem3.cy);
		ButtonPoint[2].SetPoint(nEndLine2+ITEM_POS_S+m_SizeImageItem2.cx,ITEM_POS_Y+ITEM_POS_S+m_SizeImageItem3.cy);
		ButtonPoint[3].SetPoint(m_SizeBackGround.cx-m_SizeImageButton.cx-POS_BUTTON_X,POS_BUTTON_Y);

		//按钮区域
		CRect rcBack(ButtonPoint[0],m_SizeImageItem3);
		CRect rcShift(ButtonPoint[1],m_SizeImageItem2);
		CRect rcCapital(ButtonPoint[2],m_SizeImageItem2);
		CRect rcCloseKey(ButtonPoint[3],m_SizeImageButton);

		//退格按钮
		if (rcBack.PtInRect(MousePoint))
		{
			wHoverRow=ROW_BACK;
			wHoverLine=LINE_FUNCTION;
		}

		//切换按钮
		if (rcShift.PtInRect(MousePoint))
		{
			wHoverRow=ROW_SHIFT;
			wHoverLine=LINE_FUNCTION;
		}

		//大写按钮
		if (rcCapital.PtInRect(MousePoint))
		{
			wHoverRow=ROW_CAPITAL;
			wHoverLine=LINE_FUNCTION;
		}

		//关闭按钮
		if (rcCloseKey.PtInRect(MousePoint))
		{
			wHoverRow=ROW_CLOSE_KEY;
			wHoverLine=LINE_FUNCTION;
		}
	}

	//设置变量
	m_wHoverRow=wHoverRow;
	m_wHoverLine=wHoverLine;

	return;
}

//虚拟编码
WORD CPasswordKeyboard::GetVirualKeyCode(WORD wHoverLine, WORD wHoverRow)
{
	//功能建区
	if (wHoverLine==LINE_FUNCTION)
	{
		switch (wHoverRow)
		{
		case ROW_BACK: { return VK_BACK; }
		case ROW_SHIFT: { return VK_SHIFT; }
		case ROW_CAPITAL: { return VK_CAPITAL; }
		}
	}

	//字符建区
	if ((wHoverLine<CountArray(m_nItemCount))&&(wHoverRow<m_nItemCount[wHoverLine]))
	{
		//计算索引
		bool bLowerChar=true;
		if (m_bShiftStatus==true) bLowerChar=!bLowerChar;
		if ((wHoverLine>=2)&&(m_bCapsLockStatus==true)) bLowerChar=!bLowerChar;

		//获取字符
		INT nItemCount=m_nItemCount[wHoverLine];
		INT nCharRowIndex=(wHoverRow+m_nRandLine[wHoverLine])%nItemCount;
		TCHAR chChar=m_szKeyboradChar[wHoverLine][(bLowerChar==true)?0:1][nCharRowIndex];

		return chChar;
	}

	return 0;
}

//界面绘画
VOID CPasswordKeyboard::OnNcPaint()
{
	__super::OnNcPaint();

	//获取位置
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//绘画边框
	CWindowDC WindowDC(this);
	COLORREF crBoradFrame=CSkinEdit::m_SkinAttribute.m_crEnableBorad;
	WindowDC.Draw3dRect(0,0,rcWindow.Width(),rcWindow.Height(),crBoradFrame,crBoradFrame);

	return;
}

//绘画背景
BOOL CPasswordKeyboard::OnEraseBkgnd(CDC * pDC)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//创建缓冲
	CDC BufferDC;
	CBitmap ImageBuffer;
	BufferDC.CreateCompatibleDC(pDC);
	ImageBuffer.CreateCompatibleBitmap(pDC,rcClient.Width(),rcClient.Height());

	//设置环境
	BufferDC.SetBkMode(TRANSPARENT);
	BufferDC.SelectObject(&ImageBuffer);
	BufferDC.SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//变量定义
	HINSTANCE hResInstance=GetModuleHandle(SHARE_CONTROL_DLL_NAME);
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();

	//加载资源
	CImage ImageItem1;
	CImage ImageItem2;
	CImage ImageItem3;
	CImage ImageButton;
	CImage ImageBackGround;
	ImageItem1.LoadFromResource(hResInstance,IDB_KEYBOARD_ITEM1);
	ImageItem2.LoadFromResource(hResInstance,IDB_KEYBOARD_ITEM2);
	ImageItem3.LoadFromResource(hResInstance,IDB_KEYBOARD_ITEM3);
	ImageButton.LoadFromResource(hResInstance,IDB_BT_KEYBOARD_CLOSE);
	ImageBackGround.LoadFromResource(hResInstance,IDB_PASSWORD_KEYBORAD_BK);

	//渲染资源
	pSkinRenderManager->RenderImage(ImageItem1);
	pSkinRenderManager->RenderImage(ImageItem2);
	pSkinRenderManager->RenderImage(ImageItem3);
	pSkinRenderManager->RenderImage(ImageButton);
	pSkinRenderManager->RenderImage(ImageBackGround);

	//绘画背景
	ImageBackGround.BitBlt(BufferDC,0,0);

	//字符按钮
	for (INT nLine=0;nLine<CountArray(m_nItemCount);nLine++)
	{
		//绘画子项
		for (INT nRow=0;nRow<m_nItemCount[nLine];nRow++)
		{
			//计算位置
			INT nXImageIndex=0;
			INT nCharItemIndex=(nRow+m_nRandLine[nLine])%m_nItemCount[nLine];
			if ((m_wHoverLine==nLine)&&(m_wHoverRow==nRow)) nXImageIndex=(m_bMouseDown==false)?1:2;

			//绘画子项
			INT nXDrawPos=ITEM_POS_X+(m_SizeImageItem1.cx+ITEM_POS_S)*nRow;
			INT nYDrawPos=ITEM_POS_Y+(m_SizeImageItem1.cy+ITEM_POS_S)*nLine;
			ImageItem1.BitBlt(BufferDC,nXDrawPos,nYDrawPos,m_SizeImageItem1.cx,m_SizeImageItem1.cy,nXImageIndex*m_SizeImageItem1.cx,0);

			//构造颜色
			INT nColorIndex=0;
			COLORREF crTextColor[2]={RGB(0,0,0),RGB(125,125,125)};

			//颜色计算
			if (m_bShiftStatus==true) nColorIndex=(nColorIndex+1)%CountArray(crTextColor);
			if ((m_bCapsLockStatus==true)&&(nLine>=2)) nColorIndex=(nColorIndex+1)%CountArray(crTextColor);

			//绘画字符
			BufferDC.SetTextColor(crTextColor[nColorIndex]);
			BufferDC.TextOut(nXDrawPos+5,nYDrawPos+9,&m_szKeyboradChar[nLine][0][nCharItemIndex],1);

			//绘画字符
			BufferDC.SetTextColor(crTextColor[(nColorIndex+1)%CountArray(crTextColor)]);
			BufferDC.TextOut(nXDrawPos+15,nYDrawPos+3,&m_szKeyboradChar[nLine][1][nCharItemIndex],1);
		}
	}

	//退格按钮
	{
		//资源位置
		INT nXImageIndex=0;
		if ((m_wHoverLine==LINE_FUNCTION)&&(m_wHoverRow==ROW_BACK)) nXImageIndex=(m_bMouseDown==false)?1:2;

		//绘画背景
		INT nYDrawPos=ITEM_POS_Y;
		INT nXDrawPos=ITEM_POS_X+(m_SizeImageItem1.cx+ITEM_POS_S)*m_nItemCount[0];
		ImageItem3.BitBlt(BufferDC,nXDrawPos,nYDrawPos,m_SizeImageItem3.cx,m_SizeImageItem3.cy,nXImageIndex*m_SizeImageItem3.cx,0);

		//绘画字符
		BufferDC.SetTextColor(RGB(0,0,0));
		BufferDC.TextOut(nXDrawPos+20,ITEM_POS_Y+7,TEXT("←"),2);
	}

	//切换按钮
	{
		//资源位置
		INT nXImageIndex=(m_bShiftStatus==true)?1:0;
		if ((m_wHoverLine==LINE_FUNCTION)&&(m_wHoverRow==ROW_SHIFT)) nXImageIndex=(m_bMouseDown==false)?1:2;

		//绘画背景
		INT nYDrawPos=ITEM_POS_Y+ITEM_POS_S+m_SizeImageItem1.cx;
		INT nXDrawPos=ITEM_POS_X+(m_SizeImageItem1.cx+ITEM_POS_S)*m_nItemCount[1];
		ImageItem2.BitBlt(BufferDC,nXDrawPos,nYDrawPos,m_SizeImageItem2.cx,m_SizeImageItem2.cy,nXImageIndex*m_SizeImageItem2.cx,0);

		//切换按钮
		BufferDC.SetTextColor(RGB(0,0,0));
		BufferDC.TextOut(nXDrawPos+5,ITEM_POS_Y+ITEM_POS_S+m_SizeImageItem1.cx+6,TEXT("Shift"),5);
	}

	//大写按钮
	{
		//资源位置
		INT nXImageIndex=(m_bCapsLockStatus==true)?1:0;
		if ((m_wHoverLine==LINE_FUNCTION)&&(m_wHoverRow==ROW_CAPITAL)) nXImageIndex=(m_bMouseDown==false)?1:2;

		//绘画背景
		INT nYDrawPos=ITEM_POS_Y+ITEM_POS_S+m_SizeImageItem1.cx;
		INT nXDrawPos=ITEM_POS_X+(m_SizeImageItem1.cx+ITEM_POS_S)*m_nItemCount[1]+m_SizeImageItem2.cx+ITEM_POS_S;
		ImageItem2.BitBlt(BufferDC,nXDrawPos,nYDrawPos,m_SizeImageItem2.cx,m_SizeImageItem2.cy,nXImageIndex*m_SizeImageItem2.cx,0);

		//大写按钮
		BufferDC.SetTextColor(RGB(0,0,0));
		BufferDC.TextOut(nXDrawPos+8,ITEM_POS_Y+ITEM_POS_S+m_SizeImageItem1.cx+6,TEXT("Caps"),4);
	}

	//关闭按钮
	{
		//资源位置
		INT nXImageIndex=0;
		if ((m_wHoverLine==LINE_FUNCTION)&&(m_wHoverRow==ROW_CLOSE_KEY)) nXImageIndex=(m_bMouseDown==false)?1:2;

		//绘画背景
		INT nXDrawPos=rcClient.Width()-m_SizeImageButton.cx-POS_BUTTON_X;
		ImageButton.BitBlt(BufferDC,nXDrawPos,POS_BUTTON_Y,m_SizeImageButton.cx,m_SizeImageButton.cy,nXImageIndex*m_SizeImageButton.cx,0);
	}

	//绘画界面
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),&BufferDC,0,0,SRCCOPY);

	//清理资源
	BufferDC.DeleteDC();
	ImageBuffer.DeleteObject();

	return TRUE;
}

//失去焦点
VOID CPasswordKeyboard::OnKillFocus(CWnd * pNewWnd)
{
	__super::OnKillFocus(pNewWnd);

	//销毁窗口
	DestroyWindow();

	return;
}

//鼠标消息
VOID CPasswordKeyboard::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags,Point);

	//取消捕获
	if (m_bMouseDown==true)
	{
		//取消捕获
		ReleaseCapture();

		//设置变量
		m_bMouseDown=false;

		//获取光标
		CPoint MousePoint;
		GetCursorPos(&MousePoint);
		ScreenToClient(&MousePoint);

		//更新位置
		WORD wHoverRow=m_wHoverRow;
		WORD wHoverLine=m_wHoverLine;
		SetCurrentStation(MousePoint);

		//点击处理
		if ((m_wHoverRow==wHoverRow)&&(m_wHoverLine==wHoverLine))
		{
			//关闭按钮
			if ((m_wHoverLine==LINE_FUNCTION)&&(m_wHoverRow==ROW_CLOSE_KEY))
			{
				//设置焦点
				CONTAINING_RECORD(this,CPasswordControl,m_PasswordKeyboard)->m_edPassword.SetFocus();

				//销毁窗口
				DestroyWindow();

				return;
			}

			//虚拟编码
			WORD wViraulCode=GetVirualKeyCode(m_wHoverLine,m_wHoverRow);

			//按钮处理
			switch (wViraulCode)
			{
			case VK_SHIFT:		//切换按钮
				{
					//设置变量
					m_bShiftStatus=!m_bShiftStatus;

					break;
				}
			case VK_CAPITAL:	//大写按钮
				{
					//变量定义
					INPUT Input[2];
					ZeroMemory(Input,sizeof(Input));

					//设置变量
					Input[1].ki.dwFlags=KEYEVENTF_KEYUP;   
					Input[0].type=Input[1].type=INPUT_KEYBOARD;
					Input[0].ki.wVk=Input[1].ki.wVk=wViraulCode;

					//模拟输入
					SendInput(CountArray(Input),Input,sizeof(INPUT));

					break;
				}
			default:			//默认按钮
				{
					//设置变量
					m_bShiftStatus=(GetKeyState(VK_SHIFT)&0xF0)>0;

					//发送消息
					CPasswordControl * pPasswordControl=CONTAINING_RECORD(this,CPasswordControl,m_PasswordKeyboard);
					if (pPasswordControl!=NULL) pPasswordControl->m_edPassword.SendMessage(WM_CHAR,wViraulCode,0L);

					break;
				}
			}
		}

		//更新界面
		RedrawWindow(NULL,NULL,RDW_FRAME|RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//鼠标消息
VOID CPasswordKeyboard::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//点击按钮
	if ((m_wHoverLine!=INVALID_WORD)&&(m_wHoverRow!=INVALID_WORD))
	{
		//捕获鼠标
		SetCapture();

		//设置变量
		m_bMouseDown=true;

		//更新界面
		RedrawWindow(NULL,NULL,RDW_FRAME|RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//光标消息
BOOL CPasswordKeyboard::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//获取光标
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//更新位置
	WORD wHoverRow=m_wHoverRow;
	WORD wHoverLine=m_wHoverLine;
	SetCurrentStation(MousePoint);

	//更新界面
	if ((m_wHoverRow!=wHoverRow)||(m_wHoverLine!=wHoverLine))
	{
		RedrawWindow(NULL,NULL,RDW_FRAME|RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	//设置光标
	if ((m_wHoverRow!=INVALID_WORD)&&(m_wHoverLine!=INVALID_WORD))
	{
		SetCursor(LoadCursor(GetModuleHandle(SHARE_CONTROL_DLL_NAME),MAKEINTRESOURCE(IDC_HAND_CUR)));
		return true;
	}

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//////////////////////////////////////////////////////////////////////////////////
