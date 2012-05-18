#include "StdAfx.h"
#include "DlgWhisper.h"
#include "PlatformFrame.h"
#include "ServerViewItem.h"

//////////////////////////////////////////////////////////////////////////////////

//用户信息
#define USER_INFO_CX				188									//信息宽度
#define USER_INFO_CY				180									//信息高度
#define CHAT_INPUT_CY				100									//输入高度

//颜色选择
#define MAX_CHAT_COLOR				16									//最大数目
#define IDM_SELECT_CHAT_COLOR		(WM_USER+200)						//选择颜色

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgWhisper, CSkinDialog)

	//系统命令
	ON_WM_SIZE()
	ON_WM_NCDESTROY()

	//按钮命令
	ON_BN_CLICKED(IDC_BT_USER_MENU, OnBnClickedUserMenu)
	ON_BN_CLICKED(IDC_BT_SEND_CHAT, OnBnClickedSendChat)
	ON_BN_CLICKED(IDC_BT_SEND_CONTROL, OnBnClickedSendControl)
	ON_BN_CLICKED(IDC_BT_CHAT_BUTTON_1, OnBnClickedChatButton1)
	ON_BN_CLICKED(IDC_BT_CHAT_BUTTON_2, OnBnClickedChatButton2)
	ON_BN_CLICKED(IDC_BT_CHAT_BUTTON_3, OnBnClickedChatButton3)

	//菜单命令
	ON_COMMAND(IDM_MORE_COLOR, OnSelectMoreColor)
	ON_COMMAND_RANGE(IDM_SELECT_CHAT_COLOR, (IDM_SELECT_CHAT_COLOR+MAX_CHAT_COLOR), OnSelectChatColor)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CDlgWhisper::CDlgWhisper() : CSkinDialog(IDD_DLG_WHISPER)
{
	//状态变量
	m_bCreateFlag=false;
	m_dwConversationID=0L;

	//接口变量
	m_pITCPSocket=NULL;
	m_pIMySelfUserItem=NULL;
	m_pIUserOrderParser=NULL;
	m_pIGameLevelParser=NULL;

	//组件变量
	m_pServerViewItem=NULL;
	m_pExpressionControl=NULL;

    //输入资源
	tagEncircleResource	EncircleChatInput;
	EncircleChatInput.pszImageTL=MAKEINTRESOURCE(IDB_INPUT_WHISPER_TL);
	EncircleChatInput.pszImageTM=MAKEINTRESOURCE(IDB_INPUT_WHISPER_TM);
	EncircleChatInput.pszImageTR=MAKEINTRESOURCE(IDB_INPUT_WHISPER_TR);
	EncircleChatInput.pszImageML=MAKEINTRESOURCE(IDB_INPUT_WHISPER_ML);
	EncircleChatInput.pszImageMR=MAKEINTRESOURCE(IDB_INPUT_WHISPER_MR);
	EncircleChatInput.pszImageBL=MAKEINTRESOURCE(IDB_INPUT_WHISPER_BL);
	EncircleChatInput.pszImageBM=MAKEINTRESOURCE(IDB_INPUT_WHISPER_BM);
	EncircleChatInput.pszImageBR=MAKEINTRESOURCE(IDB_INPUT_WHISPER_BR);
	m_EncircleChatInput.InitEncircleResource(EncircleChatInput,AfxGetInstanceHandle());

	//信息资源
	tagEncircleResource	EncircleChatMessage;
	EncircleChatMessage.pszImageTL=MAKEINTRESOURCE(IDB_CHAT_WHISPER_TL);
	EncircleChatMessage.pszImageTM=MAKEINTRESOURCE(IDB_CHAT_WHISPER_TM);
	EncircleChatMessage.pszImageTR=MAKEINTRESOURCE(IDB_CHAT_WHISPER_TR);
	EncircleChatMessage.pszImageML=MAKEINTRESOURCE(IDB_CHAT_WHISPER_ML);
	EncircleChatMessage.pszImageMR=MAKEINTRESOURCE(IDB_CHAT_WHISPER_MR);
	EncircleChatMessage.pszImageBL=MAKEINTRESOURCE(IDB_CHAT_WHISPER_BL);
	EncircleChatMessage.pszImageBM=MAKEINTRESOURCE(IDB_CHAT_WHISPER_BM);
	EncircleChatMessage.pszImageBR=MAKEINTRESOURCE(IDB_CHAT_WHISPER_BR);
	m_EncircleChatMessage.InitEncircleResource(EncircleChatMessage,AfxGetInstanceHandle());

	return;
}

//析构函数
CDlgWhisper::~CDlgWhisper()
{
	//删除对象
	SafeDelete(m_pExpressionControl);

	return;
}

//控件绑定
VOID CDlgWhisper::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	//聊天按钮
	DDX_Control(pDX, IDC_BT_CHAT_HISTORY, m_btChatHistory);
	DDX_Control(pDX, IDC_BT_CHAT_BUTTON_1, m_btChatButton1);
	DDX_Control(pDX, IDC_BT_CHAT_BUTTON_2, m_btChatButton2);
	DDX_Control(pDX, IDC_BT_CHAT_BUTTON_3, m_btChatButton3);

	//控制按钮
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_BT_USER_MENU, m_btUserMenu);
	DDX_Control(pDX, IDC_BT_SEND_CHAT, m_btSendChat);
	DDX_Control(pDX, IDC_BT_CHAT_HISTORY, m_btChatHistory);
	DDX_Control(pDX, IDC_BT_SEND_CONTROL, m_btSendControl);

	//聊天控件
	DDX_Control(pDX, IDC_CHAT_INPUT, m_ChatInput);
	DDX_Control(pDX, IDC_CHAT_MESSAGE, m_ChatMessage);
}

//消息解释
BOOL CDlgWhisper::PreTranslateMessage(MSG * pMsg)
{
	//按键过虑
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}

	//发送控制
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_RETURN))
	{
		//变量定义
		bool bLControlDown=((GetAsyncKeyState(VK_LCONTROL)&0xFFF0)!=0);
		bool bRControlDown=((GetAsyncKeyState(VK_RCONTROL)&0xFFF0)!=0);

		//变量定义
		ASSERT(CParameterGlobal::GetInstance()!=NULL);
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

		//发送控制
		if ((pParameterGlobal->m_bSendWhisperByEnter==true)&&(bLControlDown==false)&&(bRControlDown==false))
		{
			OnBnClickedSendChat();
			return TRUE;
		}

		//发送控制
		if ((pParameterGlobal->m_bSendWhisperByEnter==false)&&((bLControlDown==true)||(bRControlDown==true)))
		{
			OnBnClickedSendChat();
			return TRUE;
		}
	}

	return __super::PreTranslateMessage(pMsg);
}

//配置函数
BOOL CDlgWhisper::OnInitDialog()
{
	__super::OnInitDialog();

	//设置窗口
	SetWindowText(TEXT("私聊"));
	SetIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_WHISPER)),TRUE);

	//变量定义
	CRect rcCreate(0,0,0,0);
	HINSTANCE hInstance=AfxGetInstanceHandle();

	//聊天控件
	m_ChatInput.SetLimitText(LEN_USER_CHAT-1);
	m_ChatMessage.SetExpressionManager(CExpressionManager::GetInstance(),RGB(255,255,255));

	//聊天按钮
	m_btUserMenu.SetButtonImage(IDB_BT_WHISPER_USER_MENU,hInstance,false,false);
	m_btSendControl.SetButtonImage(IDB_BT_WHISPER_SEND_CONTROL,hInstance,false,false);
	m_btChatHistory.SetButtonImage(IDB_BT_WHISPER_HISTORY,TEXT("BT_WHISPER_HISTORY"),hInstance,false,false);
	m_btChatButton1.SetButtonImage(IDB_BT_WHISPER_BUTTON_1,TEXT("BT_WHISPER_BUTTON_1"),hInstance,false,false);
	m_btChatButton2.SetButtonImage(IDB_BT_WHISPER_BUTTON_2,TEXT("BT_WHISPER_BUTTON_2"),hInstance,false,false);
	m_btChatButton3.SetButtonImage(IDB_BT_WHISPER_BUTTON_3,TEXT("BT_WHISPER_BUTTON_3"),hInstance,false,false);

	//设置变量
	m_bCreateFlag=true;

	//调整控件
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());

	//提示信息
	m_ChatMessage.InsertPromptString(TEXT("请勿在私聊中相信任何形式的中奖信息，请勿向任何人透露您的密码！\r\n"));

	return TRUE;
}

//确定消息
VOID CDlgWhisper::OnOK()
{
	return;
}

//取消消息
VOID CDlgWhisper::OnCancel()
{
	//关闭窗口
	DestroyWindow();

	return;
}

//命令函数
BOOL CDlgWhisper::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//命令处理
	switch (LOWORD(wParam))
	{
	case IDM_SEND_BY_ENTER:
		{
			//设置变量
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
			if (pParameterGlobal!=NULL) pParameterGlobal->m_bSendWhisperByEnter=true;

			//设置焦点
			m_ChatInput.SetFocus();

			return TRUE;
		}
	case IDM_SEND_BY_CTRL_ENTER:
		{
			//设置变量
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
			if (pParameterGlobal!=NULL) pParameterGlobal->m_bSendWhisperByEnter=false;

			//设置焦点
			m_ChatInput.SetFocus();

			return TRUE;
		}
	}

	//颜色选择

	return __super::OnCommand(wParam,lParam);
}

//表情事件
VOID CDlgWhisper::OnExpressionSelect(CExpression * pExpression, tagExpressionInfo * pExpressionInfo)
{
	//设置焦点
	m_ChatInput.SetFocus();

	//发送表情
	if (EfficacyUserChat(NULL,pExpression->GetIndex())==true)
	{
		if (m_ClientUserItemArray.GetCount()==2L)
		{
			//私聊信息
			ASSERT(m_ClientUserItemArray[1]!=NULL);
			SendExpressionPacket(m_ClientUserItemArray[1],pExpression->GetIndex());
		}
		else if (m_ClientUserItemArray.GetCount()>=3L)
		{
			//群聊信息
			IClientUserItem * * pIClientUserItem=m_ClientUserItemArray.GetData();
		//	SendExpressionPacket(pIClientUserItem,(WORD)m_ClientUserItemArray.GetCount(),strMessage);
		}
		else
		{
			//构造提示
			TCHAR szMessage[LEN_USER_CHAT+128]=TEXT("");
			_sntprintf(szMessage,CountArray(szMessage),TEXT("本次会话的所有用户已经离开，消息无法发送"));

			//插入信息
			m_ChatMessage.InsertString(TEXT("\r\n"),0);
			m_ChatMessage.InsertSystemString(szMessage);
		}
	}

	return;
}

//绘画消息
VOID CDlgWhisper::OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight)
{
	//框架位置
	INT nTBorder=m_SkinAttribute.m_EncircleInfoFrame.nTBorder;
	INT nBBorder=m_SkinAttribute.m_EncircleInfoFrame.nBBorder;
	INT nLBorder=m_SkinAttribute.m_EncircleInfoFrame.nLBorder;
	INT nRBorder=m_SkinAttribute.m_EncircleInfoFrame.nRBorder;

	//输入位置
	CRect rcChatInput;
	rcChatInput.left=nLBorder;
	rcChatInput.bottom=nHeight-nBBorder;
	rcChatInput.right=nWidth-USER_INFO_CX-nRBorder;
	rcChatInput.top=nHeight-CHAT_INPUT_CY-nBBorder;

	//信息位置
	CRect rcChatMessage;
	rcChatMessage.top=nTBorder;
	rcChatMessage.left=nLBorder;
	rcChatMessage.right=nWidth-USER_INFO_CX-nRBorder;
	rcChatMessage.bottom=nHeight-CHAT_INPUT_CY-nBBorder;

	//控制区域
	CRect rcChatControl;
	rcChatControl.right=nWidth-5;
	rcChatControl.bottom=nHeight-9;
	rcChatControl.top=nTBorder+USER_INFO_CY+5;
	rcChatControl.left=nWidth-USER_INFO_CX-nRBorder;

	//绘画背景
	COLORREF crBackGround=RGB(215,223,228);
	pDC->FillSolidRect(nLBorder,nTBorder,nWidth-nLBorder-nRBorder,nHeight-nTBorder-nBBorder,crBackGround);

	//绘画环绕
	m_EncircleChatInput.DrawEncircleFrame(pDC,rcChatInput);
	m_EncircleChatMessage.DrawEncircleFrame(pDC,rcChatMessage);

	//控制信息
	DrawUserInfoArea(pDC,nWidth-USER_INFO_CX-nRBorder,nTBorder,USER_INFO_CX,USER_INFO_CY);
	DrawFunctionArea(pDC,rcChatControl.left,rcChatControl.top,rcChatControl.Width(),rcChatControl.Height());

	return;
}

//废弃判断
bool CDlgWhisper::DisuseEstimate()
{
	//废弃判断
	if (m_hWnd==NULL) return true;
	if (m_ClientUserItemArray.GetCount()<=1L) return true;

	return false;
}

//用户比较
bool CDlgWhisper::CompareUserItem(DWORD dwUserID[], WORD wUserCount)
{
	//数目对比
	if (m_ClientUserItemArray.GetCount()!=wUserCount)
	{
		return false;
	}

	//用户对比
	for (INT_PTR i=0;i<m_ClientUserItemArray.GetCount();i++)
	{
		//用户标识
		DWORD dwUserItemID=m_ClientUserItemArray[i]->GetUserID();

		//用户对比
		for (WORD j=0;j<wUserCount;j++) 
		{
			if (dwUserID[j]==dwUserItemID) break;
		}

		//结果判断
		if (j==wUserCount) return false;
	}

	return true;
}

//聊天效验
bool CDlgWhisper::EfficacyUserChat(LPCTSTR pszChatString, WORD wExpressionIndex)
{
	return true;
}

//创建窗口
bool CDlgWhisper::CreateWhisperWnd(DWORD dwConversationID, IClientUserItem * pIClientUserItem[], WORD wUserCount, CWnd * pParentWnd)
{
	//设置变量
	m_ClientUserItemArray.RemoveAll();
	m_dwConversationID=dwConversationID;

	//用户收集
	for (WORD i=0;i<wUserCount;i++)
	{
		//终止判断
		if (wUserCount>=MAX_WHISPER_USER) 
		{
			ASSERT(FALSE);
			break;
		}

		//插入用户
		ASSERT(pIClientUserItem[i]!=NULL);
		m_ClientUserItemArray.Add(pIClientUserItem[i]);
	}

	//创建窗口
	if (m_hWnd!=NULL)
	{
		//变量定义
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//更新窗口
		RectifyTitle();
	}
	else 
	{
		//创建窗口
		Create(IDD_DLG_WHISPER,pParentWnd);
	}

	//置顶窗口
	SetActiveWindow();
	BringWindowToTop();
	SetForegroundWindow();

	return true;
}

//用户进入
VOID CDlgWhisper::OnEventUserEnter(IClientUserItem * pIClientUserItem)
{
	//用户搜索
	for (INT_PTR i=0;i<m_ClientUserItemArray.GetCount();i++)
	{
		if (pIClientUserItem==m_ClientUserItemArray[i])
		{
			ASSERT(FALSE);
			return;
		}
	}

	//设置数据
	m_ClientUserItemArray.Add(pIClientUserItem);

	//事件通知
	m_ChatMessage.InsertUserAccounts(pIClientUserItem->GetNickName());
	m_ChatMessage.InsertString(TEXT("加入了本次会话\r\n"),COLOR_EVENT);

	//更新标题
	RectifyTitle();

	//更新界面
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE);

	return;
}

//用户离开
VOID CDlgWhisper::OnEventUserLeave(IClientUserItem * pIClientUserItem)
{
	//用户搜索
	for (INT_PTR i=0;i<m_ClientUserItemArray.GetCount();i++)
	{
		if (pIClientUserItem==m_ClientUserItemArray[i])
		{
			//设置数据
			m_ClientUserItemArray.RemoveAt(i);

			//会话设置
			if (m_ClientUserItemArray.GetCount()<=2)
			{
				m_dwConversationID=0L;
			}

			//插入换行
			LONG lTextLength=m_ChatMessage.GetWindowTextLength();
			if (lTextLength!=0L) m_ChatMessage.InsertString(TEXT("\r\n"),COLOR_EVENT);

			//事件通知
			m_ChatMessage.InsertUserAccounts(pIClientUserItem->GetNickName());
			m_ChatMessage.InsertString(TEXT("离开了本次会话\r\n"),COLOR_EVENT);

			//更新标题
			RectifyTitle();

			//更新界面
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE);

			return;
		}
	}

	return;
}

//用户状态
VOID CDlgWhisper::OnEventUserStatus(IClientUserItem * pIClientUserItem)
{
	//状态更新
	for (INT_PTR i=0;i<m_ClientUserItemArray.GetCount();i++)
	{
		if (m_ClientUserItemArray[i]==pIClientUserItem)
		{
			//更新界面
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE);

			break;
		}
	}

	return;
}

//用户表情
VOID CDlgWhisper::OnEventExpression(IClientUserItem * pIClientUserItem, WORD wItemIndex)
{
	//效验参数
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return;

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//变量定义
	CExpressionManager * pExpressionManager=CExpressionManager::GetInstance();
	CExpression * pExpressionItem=pExpressionManager->GetExpressionItem(wItemIndex);

	//插入消息
	if (pExpressionItem!=NULL)
	{
		//状态变量
		DWORD dwSendUserID=pIClientUserItem->GetUserID();
		bool bMySelfString=(pGlobalUserData->dwUserID==dwSendUserID);

		//获取路径
		TCHAR szImagePath[MAX_PATH]=TEXT("");
		pExpressionItem->GetExpressionPath(szImagePath,CountArray(szImagePath));

		//显示消息
		LPCTSTR pszNiceName=pIClientUserItem->GetNickName();
		m_ChatMessage.InsertExpression(pszNiceName,szImagePath,bMySelfString);
	}

	//显示控制
	if (IsWindowVisible()==FALSE)
	{
		if (pParameterGlobal->m_bAutoShowWhisper==true)
		{
			//显示窗口
			ShowWindow(SW_RESTORE);

			//置顶窗口
			SetActiveWindow();
			BringWindowToTop();
			SetForegroundWindow();
		}
		else
		{
			//收集窗口
			CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
			if (pPlatformFrame!=NULL) pPlatformFrame->InsertWhisperItem(this);
		}
	}
	else
	{
		//激活窗口
		if (IsChild(GetFocus())==FALSE)
		{
			FlashWindow(TRUE);
		}
	}

	return;
}

//用户私聊
VOID CDlgWhisper::OnEventWisperChat(IClientUserItem * pIClientUserItem, LPCTSTR pszString, COLORREF rcColor)
{
	//效验参数
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return;

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//状态变量
	DWORD dwSendUserID=pIClientUserItem->GetUserID();
	bool bMySelfString=(pGlobalUserData->dwUserID==dwSendUserID);

	//显示消息
	LPCTSTR pszNiceName=pIClientUserItem->GetNickName();
	m_ChatMessage.InsertWhisperChat(pszNiceName,pszString,rcColor,bMySelfString);

	//显示控制
	if (IsWindowVisible()==FALSE)
	{
		if (pParameterGlobal->m_bAutoShowWhisper==true)
		{
			//显示窗口
			ShowWindow(SW_RESTORE);

			//置顶窗口
			SetActiveWindow();
			BringWindowToTop();
			SetForegroundWindow();
		}
		else
		{
			//收集窗口
			CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
			if (pPlatformFrame!=NULL) pPlatformFrame->InsertWhisperItem(this);
		}
	}
	else
	{
		//激活窗口
		if (IsChild(GetFocus())==FALSE)
		{
			FlashWindow(TRUE);
		}
	}

	return;
}

//更新标题
VOID CDlgWhisper::RectifyTitle()
{
	//变量定义
	TCHAR szTitle[MAX_PATH]=TEXT("");
	LPCTSTR pszServerName=m_pServerViewItem->GetServerName();

	//构造标题
	if (m_ClientUserItemArray.GetCount()<=2)
	{
		_sntprintf(szTitle,CountArray(szTitle),TEXT("[ %s ] － 私聊"),pszServerName);
	}
	else
	{
		_sntprintf(szTitle,CountArray(szTitle),TEXT("[ %s ] － 多人聊天"),pszServerName);
	}

	//设置标题
	SetWindowText(szTitle);

	return;
}

//调整控件
VOID CDlgWhisper::RectifyControl(INT nWidth, INT nHeight)
{
	//状态判断
	if (m_bCreateFlag==false) return;
	if ((nWidth==0)||(nHeight==0)) return;

	//框架位置
	INT nTBorder=m_SkinAttribute.m_EncircleInfoFrame.nTBorder;
	INT nBBorder=m_SkinAttribute.m_EncircleInfoFrame.nBBorder;
	INT nLBorder=m_SkinAttribute.m_EncircleInfoFrame.nLBorder;
	INT nRBorder=m_SkinAttribute.m_EncircleInfoFrame.nRBorder;

	//输入位置
	CRect rcChatInput;
	rcChatInput.left=nLBorder;
	rcChatInput.bottom=nHeight-nBBorder;
	rcChatInput.right=nWidth-USER_INFO_CX-nRBorder;
	rcChatInput.top=nHeight-CHAT_INPUT_CY-nBBorder;

	//信息位置
	CRect rcChatMessage;
	rcChatMessage.top=nTBorder;
	rcChatMessage.left=nLBorder;
	rcChatMessage.right=nWidth-USER_INFO_CX-nRBorder;
	rcChatMessage.bottom=nHeight-CHAT_INPUT_CY-nBBorder;

	//移动控件
	HDWP hDwp=BeginDeferWindowPos(32);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;

	//环绕资源
	m_EncircleChatInput.DeferWindowPos(&m_ChatInput,hDwp,rcChatInput);
	m_EncircleChatMessage.DeferWindowPos(&m_ChatMessage,hDwp,rcChatMessage);

	//用户菜单
	DeferWindowPos(hDwp,m_btUserMenu,NULL,nWidth-123-nRBorder,nTBorder+13,0,0,uFlags|SWP_NOSIZE);

	//聊天按钮
	CRect rcChatButton;
	m_btChatButton1.GetWindowRect(&rcChatButton);
	DeferWindowPos(hDwp,m_btChatButton1,NULL,rcChatInput.left+10,rcChatInput.top+3,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btChatButton2,NULL,rcChatInput.left+rcChatButton.Width()+10,rcChatInput.top+3,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btChatButton3,NULL,rcChatInput.left+rcChatButton.Width()*2+10,rcChatInput.top+3,0,0,uFlags|SWP_NOSIZE);

	//聊天记录
	CRect rcChatHistory;
	m_btChatHistory.GetWindowRect(&rcChatHistory);
	DeferWindowPos(hDwp,m_btChatHistory,NULL,rcChatInput.right-rcChatHistory.Width()-6,rcChatInput.top+3,0,0,uFlags|SWP_NOSIZE);

	//控制按钮
	CRect rcControl;
	CRect rcSendControl;
	m_btCancel.GetWindowRect(&rcControl);
	m_btSendControl.GetWindowRect(&rcSendControl);
	DeferWindowPos(hDwp,m_btSendControl,NULL,rcChatInput.right-rcSendControl.Width()-3,nHeight-rcControl.Height()-12,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btCancel,NULL,rcChatInput.right-rcSendControl.Width()-rcControl.Width()*2-13,nHeight-rcControl.Height()-12,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btSendChat,NULL,rcChatInput.right-rcSendControl.Width()-rcControl.Width()*1-8,nHeight-rcControl.Height()-12,0,0,uFlags|SWP_NOSIZE);

	//移动结束
	EndDeferWindowPos(hDwp);

	//更新界面
	RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE);

	return;
}

//排序函数
VOID CDlgWhisper::SortUserIDData(DWORD dwUserID[], WORD wItemCount)
{
	//变量定义
	bool bSorted=true;
	DWORD dwTempUserID=0L;
	WORD wLastIndex=wItemCount-1;

	//排序操作
	do
	{
		//设置变量
		bSorted=true;

		//排序操作
		for (WORD i=0;i<wLastIndex;i++)
		{
			//交换位置
			if (dwUserID[i]<dwUserID[i+1])
			{
				bSorted=false;
				dwTempUserID=dwUserID[i];
				dwUserID[i]=dwUserID[i+1];
				dwUserID[i+1]=dwTempUserID;
			}	
		}

		//设置索引
		wLastIndex--;

	} while (bSorted==false);

	return;
}

//绘画控制
VOID CDlgWhisper::DrawFunctionArea(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight)
{
	//变量定义
	ASSERT(CSkinRenderManager::GetInstance()!=NULL);
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();

	//加载资源
	CBitImage ImageControlT;
	CBitImage ImageControlM;
	CBitImage ImageControlB;
	ImageControlT.LoadFromResource(AfxGetInstanceHandle(),IDB_WHISPER_CONTRUL_T);
	ImageControlM.LoadFromResource(AfxGetInstanceHandle(),IDB_WHISPER_CONTRUL_M);
	ImageControlB.LoadFromResource(AfxGetInstanceHandle(),IDB_WHISPER_CONTRUL_B);

	//渲染资源
	pSkinRenderManager->RenderImage(ImageControlT);
	pSkinRenderManager->RenderImage(ImageControlM);
	pSkinRenderManager->RenderImage(ImageControlB);

	//用户信息
	ImageControlT.BitBlt(pDC->m_hDC,nXPos,nYPos);
	ImageControlB.BitBlt(pDC->m_hDC,nXPos,nYPos+nHeight-ImageControlB.GetHeight());

	//绘画中间
	INT nYStart=nYPos+ImageControlT.GetHeight();
	INT nYTerminate=nYPos+nHeight-ImageControlB.GetHeight();
	for (INT nYDrawPos=nYStart;nYDrawPos<nYTerminate;nYDrawPos+=ImageControlM.GetHeight())
	{
		INT nWidth=ImageControlM.GetWidth();
		ImageControlM.BitBlt(pDC->m_hDC,nXPos,nYDrawPos,nWidth,__min(nYTerminate-nYDrawPos,nWidth),0,0);
	}

	return;
}

//绘画用户
VOID CDlgWhisper::DrawUserInfoArea(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight)
{
	//加载资源
	CBitImage ImageUserInfoT;
	CBitImage ImageUserInfoM;
	CBitImage ImageUserInfoB;
	ImageUserInfoT.LoadFromResource(AfxGetInstanceHandle(),IDB_WHISPER_USER_INFO_T);
	ImageUserInfoM.LoadFromResource(AfxGetInstanceHandle(),IDB_WHISPER_USER_INFO_M);
	ImageUserInfoB.LoadFromResource(AfxGetInstanceHandle(),IDB_WHISPER_USER_INFO_B);

	//用户信息
	ImageUserInfoT.BitBlt(pDC->m_hDC,nXPos,nYPos);
	ImageUserInfoB.BitBlt(pDC->m_hDC,nXPos,nYPos+nHeight-ImageUserInfoB.GetHeight());

	//绘画中间
	INT nYStart=nYPos+ImageUserInfoT.GetHeight();
	INT nYTerminate=nYPos+nHeight-ImageUserInfoB.GetHeight();
	for (INT nYDrawPos=nYStart;nYDrawPos<nYTerminate;nYDrawPos+=ImageUserInfoM.GetHeight())
	{
		INT nWidth=ImageUserInfoM.GetWidth();
		ImageUserInfoM.BitBlt(pDC->m_hDC,nXPos,nYDrawPos,nWidth,__min(nYTerminate-nYDrawPos,nWidth),0,0);
	}

	//用户信息
	if (m_ClientUserItemArray.GetCount()>=2L)
	{
		//变量定义
		ASSERT(CFaceItemControl::GetInstance()!=NULL);
		IFaceItemControl * pIFaceItemControl=CFaceItemControl::GetInstance();

		//变量定义
		ASSERT(CUserItemElement::GetInstance()!=NULL);
		CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();

		//获取用户
		IClientUserItem * pIClientUserItem=m_ClientUserItemArray[1];
		tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

		//游戏状态
		BYTE cbUserStatus=pIClientUserItem->GetUserStatus();
		LPCTSTR pszUserStatus[]={TEXT("空闲"),TEXT("入座"),TEXT("准备"),TEXT("旁观"),TEXT("游戏"),TEXT("断线")};

		//绘画头像
		if ((pCustomFaceInfo->dwDataSize!=0L)&&(pIClientUserItem->GetCustomID()!=0L))
		{
			pIFaceItemControl->DrawFaceNormal(pDC,nXPos+13,nYPos+14,pCustomFaceInfo->dwCustomFace);
		}
		else
		{
			pIFaceItemControl->DrawFaceNormal(pDC,nXPos+13,nYPos+14,pIClientUserItem->GetFaceID());
		}

		//设置 DC
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,0));

		//经验信息
		DWORD dwExperience=pIClientUserItem->GetUserExperience();
		WORD wExperienceLevel=pUserItemElement->GetExperienceLevel(dwExperience);

		//等级信息
		LPCTSTR pszKindName=m_pIGameLevelParser->GetKindName();
		LPCTSTR pszGameLevel=m_pIGameLevelParser->GetLevelDescribe(pIClientUserItem);

		//用户信息
		TCHAR szString[128]=TEXT("");
		LPCTSTR pszMember=m_pIUserOrderParser->GetMemberDescribe(pIClientUserItem->GetMemberOrder());
		LPCTSTR pszMaster=m_pIUserOrderParser->GetMasterDescribe(pIClientUserItem->GetMasterOrder());

		//信息位置
		CRect rcNickName(nXPos+80,nYPos+18,nXPos+nWidth-5,nYPos+18+12);
		CRect rcUserGameID(nXPos+80,nYPos+40,nXPos+nWidth-5,nYPos+40+12);

		//信息位置
		CRect rcGameLevel(nXPos+11,nYPos+74,nXPos+nWidth-5,nYPos+74+12);
		CRect rcExperience(nXPos+11,nYPos+95,nXPos+nWidth-5,nYPos+95+12);
		CRect rcGameScore(nXPos+11,nYPos+116,nXPos+nWidth-5,nYPos+116+12);
		CRect rcUserStatus(nXPos+11,nYPos+137,nXPos+nWidth-5,nYPos+137+12);
		CRect rcUnderWrite(nXPos+11,nYPos+158,nXPos+nWidth-5,nYPos+158+12);

		//用户信息
		_sntprintf(szString,CountArray(szString),TEXT("%s"),pIClientUserItem->GetNickName());
		pDC->DrawText(szString,lstrlen(szString),&rcNickName,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//用户标识
		_sntprintf(szString,CountArray(szString),TEXT("%ld"),pIClientUserItem->GetGameID());
		pDC->DrawText(szString,lstrlen(szString),&rcUserGameID,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//游戏等级
		_sntprintf(szString,CountArray(szString),TEXT("级别：%s [ %s ]"),pszGameLevel,pszKindName);
		pDC->DrawText(szString,lstrlen(szString),&rcGameLevel,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//用户经验
		_sntprintf(szString,CountArray(szString),TEXT("经验：%d 级"),wExperienceLevel);
		pDC->DrawText(szString,lstrlen(szString),&rcExperience,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//经验标志
		CSize SizeExperience=pDC->GetTextExtent(szString,lstrlen(szString));
		pUserItemElement->DrawExperience(pDC,rcExperience.left+SizeExperience.cx+6,rcExperience.top-4L,dwExperience);

		//用户成绩
		_sntprintf(szString,CountArray(szString),TEXT("成绩：%ld"),pIClientUserItem->GetUserScore());
		pDC->DrawText(szString,lstrlen(szString),&rcGameScore,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//游戏状态
		_sntprintf(szString,CountArray(szString),TEXT("状态：%s"),pszUserStatus[cbUserStatus-1]);
		pDC->DrawText(szString,lstrlen(szString),&rcUserStatus,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//用户签名
		_sntprintf(szString,CountArray(szString),TEXT("签名：%s"),pIClientUserItem->GetUnderWrite());
		pDC->DrawText(szString,lstrlen(szString),&rcUnderWrite,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	}

	return;
}

//发送表情
bool CDlgWhisper::SendExpressionPacket(IClientUserItem * pIClientUserItem, WORD wItemIndex)
{
	//变量定义
	CMD_GR_C_WisperExpression WisperExpression;
	ZeroMemory(&WisperExpression,sizeof(WisperExpression));

	//构造信息
	WisperExpression.wItemIndex=wItemIndex;
	WisperExpression.dwTargetUserID=pIClientUserItem->GetUserID();

	//发送命令
	m_pITCPSocket->SendData(MDM_GR_USER,SUB_GR_WISPER_EXPRESSION,&WisperExpression,sizeof(WisperExpression));

	return true;
}

//发送聊天
bool CDlgWhisper::SendWhisperChatPacket(IClientUserItem * pIClientUserItem, LPCTSTR pszChatString)
{
	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//构造信息
	CMD_GR_C_WisperChat WisperChat;
	lstrcpyn(WisperChat.szChatString,pszChatString,CountArray(WisperChat.szChatString));

	//构造数据
	WisperChat.dwChatColor=pParameterGlobal->m_crWhisperTX;
	WisperChat.dwTargetUserID=pIClientUserItem->GetUserID();
	WisperChat.wChatLength=CountStringBuffer(WisperChat.szChatString);

	//发送命令
	WORD wHeadSize=sizeof(WisperChat)-sizeof(WisperChat.szChatString);
	m_pITCPSocket->SendData(MDM_GR_USER,SUB_GR_WISPER_CHAT,&WisperChat,wHeadSize+WisperChat.wChatLength*sizeof(WisperChat.szChatString[0]));

	return true;
}

//发送会话
bool CDlgWhisper::SendColloquyChatPacket(IClientUserItem * pIClientUserItem[], WORD wUserCount, LPCTSTR pszChatString)
{
	//变量定义
	CMD_GR_ColloquyChat ColloquyChat;
	ZeroMemory(&ColloquyChat,sizeof(ColloquyChat));

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//构造用户
	for (WORD i=0;i<wUserCount;i++)
	{
		//效验数据
		ASSERT(i<CountArray(ColloquyChat.dwTargetUserID));
		if (i>=CountArray(ColloquyChat.dwTargetUserID)) break;

		//设置数据
		ColloquyChat.dwTargetUserID[i]=pIClientUserItem[i]->GetUserID();
	}

	//数据排序
	SortUserIDData(ColloquyChat.dwTargetUserID,wUserCount);

	//会话信息
	lstrcpyn(ColloquyChat.szChatString,pszChatString,CountArray(ColloquyChat.szChatString));

	//会话属性
	ColloquyChat.dwConversationID=m_dwConversationID;
	ColloquyChat.dwSendUserID=pGlobalUserData->dwUserID;
	ColloquyChat.dwChatColor=pParameterGlobal->m_crWhisperTX;
	ColloquyChat.wChatLength=CountStringBuffer(ColloquyChat.szChatString);

	//发送命令
	WORD wHeadSize=sizeof(ColloquyChat)-sizeof(ColloquyChat.szChatString);
	m_pITCPSocket->SendData(MDM_GR_USER,SUB_GR_COLLOQUY_CHAT,&ColloquyChat,wHeadSize+ColloquyChat.wChatLength*sizeof(ColloquyChat.szChatString[0]));

	return true;
}

//用户菜单
VOID CDlgWhisper::OnBnClickedUserMenu()
{
	//弹出菜单
	if (m_ClientUserItemArray.GetCount()>=2L)
	{
		ASSERT(m_ClientUserItemArray[1]!=NULL);
		m_pServerViewItem->TrackUserItemMenu(m_ClientUserItemArray[1]);
	}

	return;
}

//发送消息
VOID CDlgWhisper::OnBnClickedSendChat()
{
	//获取信息
	CString strMessage;
	m_ChatInput.GetWindowText(strMessage);

	//发送信息
	if (strMessage.IsEmpty()==FALSE)
	{
		if (m_ClientUserItemArray.GetCount()==2L)
		{
			//私聊信息
			ASSERT(m_ClientUserItemArray[1]!=NULL);
			SendWhisperChatPacket(m_ClientUserItemArray[1],strMessage);
		}
		else if (m_ClientUserItemArray.GetCount()>=3L)
		{
			//群聊信息
			IClientUserItem * * pIClientUserItem=m_ClientUserItemArray.GetData();
			SendColloquyChatPacket(pIClientUserItem,(WORD)m_ClientUserItemArray.GetCount(),strMessage);
		}
		else
		{
			//构造提示
			TCHAR szMessage[LEN_USER_CHAT+128]=TEXT("");
			_sntprintf(szMessage,CountArray(szMessage),TEXT("本次会话的所有用户已经离开，“%s”消息无法发送"),strMessage);

			//插入信息
			m_ChatMessage.InsertString(TEXT("\r\n"),0);
			m_ChatMessage.InsertSystemString(szMessage);
		}
	}
	else
	{
		//插入信息
		m_ChatMessage.InsertCustomString(TEXT("发送内容不能为空，请输入内容"),RGB(125,125,125));
	}

	//设置焦点
	m_ChatInput.SetFocus();
	m_ChatInput.SetWindowText(TEXT(""));

	return;
}

//发送控制
VOID CDlgWhisper::OnBnClickedSendControl()
{
	//构造菜单
	CSkinMenu Menu;
	Menu.CreateMenu();

	//构造菜单
	Menu.AppendMenu(IDM_SEND_BY_ENTER,TEXT("按 Enter 键发送消息"));
	Menu.AppendMenu(IDM_SEND_BY_CTRL_ENTER,TEXT("按 Ctrl+Enter 键发送消息"));

	//设置菜单
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
	if (pParameterGlobal->m_bSendWhisperByEnter==true) Menu.CheckMenuItem(IDM_SEND_BY_ENTER,MF_BYCOMMAND|MF_CHECKED);
	if (pParameterGlobal->m_bSendWhisperByEnter==false) Menu.CheckMenuItem(IDM_SEND_BY_CTRL_ENTER,MF_BYCOMMAND|MF_CHECKED);

	//获取位置
	CRect rcButton;
	m_btSendControl.GetWindowRect(&rcButton);

	//弹出菜单
	Menu.TrackPopupMenu(rcButton.left,rcButton.bottom,this);

	return;
}

//聊天按钮
VOID CDlgWhisper::OnBnClickedChatButton1()
{
	//创建表情
	CRect rcButton;
	m_btChatButton1.GetWindowRect(&rcButton);

	//创建窗口
	if (m_pExpressionControl==NULL)
	{
		m_pExpressionControl=new CExpressionControl;
	}

	//显示窗口
	m_pExpressionControl->ShowExpression(this,rcButton.left,rcButton.top,this);

	return;
}

//聊天按钮
VOID CDlgWhisper::OnBnClickedChatButton2()
{
	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//变量定义
	INT nWidth=72,nHeight=16;
	COLORREF crColor[]={RGB(255,0,0),RGB(255,255,0),RGB(128,255,0),RGB(0,255,255),
		RGB(0,0,128),RGB(0,128,0),RGB(128,0,128),RGB(128,0,0),RGB(0,0,0)};

	//构造颜色
	CImage ImageColor[CountArray(crColor)];
	for (INT i=0;i<CountArray(ImageColor);i++)
	{
		//创建位图
		ImageColor[i].Create(nWidth,nHeight,16);

		//获取设备
		CImageDC BufferDC(ImageColor[i]);
		CDC * pDC=CDC::FromHandle(BufferDC);

		//绘画位图
		pDC->FillSolidRect(2,2,nWidth-4,nHeight-4,crColor[i]);
		pDC->Draw3dRect(0,0,nWidth,nHeight,RGB(0,0,0),RGB(0,0,0));
		pDC->Draw3dRect(1,1,nWidth-2,nHeight-2,RGB(255,255,255),RGB(255,255,255));
	}

	//创建菜单
	CSkinMenu ColorMenu;
	ColorMenu.CreateMenu();

	//构造菜单
	ColorMenu.AppendMenu(IDM_MORE_COLOR,TEXT("选择其他颜色"));

	//颜色选项
	ColorMenu.AppendSeparator();
	for (INT i=0;i<CountArray(ImageColor);i++) 
	{
		ColorMenu.AppendMenu(IDM_SELECT_CHAT_COLOR+i,ImageColor[i]);
		if (pParameterGlobal->m_crWhisperTX==crColor[i]) ColorMenu.CheckMenuItem(IDM_SELECT_CHAT_COLOR+i,MF_BYCOMMAND|MF_CHECKED);
	}

	//弹出菜单
	CRect rcButton;
	m_btChatButton2.GetWindowRect(&rcButton);
	ColorMenu.TrackPopupMenu(rcButton.right,rcButton.top,this);

	return;
}

//聊天按钮
VOID CDlgWhisper::OnBnClickedChatButton3()
{
	return;
}

//更多颜色
VOID CDlgWhisper::OnSelectMoreColor()
{
	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//设置颜色
	CColorDialog ColorDialog(pParameterGlobal->m_crWhisperTX,CC_FULLOPEN,this);
	if (ColorDialog.DoModal()==IDOK) pParameterGlobal->m_crWhisperTX=ColorDialog.GetColor();

	//设置界面
	m_ChatInput.SetFocus();

	return;
}

//选择颜色
VOID CDlgWhisper::OnSelectChatColor(UINT nCommandID)
{
	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//变量定义
	UINT nIndex=nCommandID-IDM_SELECT_CHAT_COLOR;
	COLORREF crColor[]={RGB(255,0,0),RGB(255,255,0),RGB(128,255,0),RGB(0,255,255),
		RGB(0,0,128),RGB(0,128,0),RGB(128,0,128),RGB(128,0,0),RGB(0,0,0)};

	//选择颜色
	ASSERT(nIndex<CountArray(crColor));
	if (nIndex<CountArray(crColor)) pParameterGlobal->m_crWhisperTX=crColor[nIndex];

	//设置焦点
	m_ChatInput.SetFocus();

	return;
}

//销毁消息
VOID CDlgWhisper::OnNcDestroy()
{
	//状态变量
	m_bCreateFlag=false;

	//设置变量
	m_dwConversationID=0L;
	m_ClientUserItemArray.RemoveAll();

	//删除对象
	SafeDelete(m_pExpressionControl);

	__super::OnNcDestroy();
}

//位置消息
VOID CDlgWhisper::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//调整控件
	RectifyControl(cx,cy);

	return;
}

//////////////////////////////////////////////////////////////////////////
