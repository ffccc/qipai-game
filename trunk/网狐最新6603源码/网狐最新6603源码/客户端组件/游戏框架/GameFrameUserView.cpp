#include "StdAfx.h"
#include "Resource.h"
#include "GlobalUnits.h"
#include "GameFrameUserView.h"

//////////////////////////////////////////////////////////////////////////////////

//广告偏移
#define	BILL_T						5									//广告上高
#define	BILL_L						7									//广告左宽
#define	BILL_R						7									//广告右宽
#define	BILL_B						143									//广告下高

//控件标示
#define IDC_WEB_PUBLICIZE			100									//浏览控件
#define IDC_BT_USER_MEDAL			101									//兑换奖牌
#define IDC_BT_LOVE_LINESS			102									//兑换魅力

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameFrameUserView, CWnd)

	//系统消息
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()

	//按钮消息
	ON_BN_CLICKED(IDC_BT_USER_MEDAL, OnBnClickedUserMedal)
	ON_BN_CLICKED(IDC_BT_LOVE_LINESS, OnBnClickedLoveLiness)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameFrameUserView::CGameFrameUserView()
{
	//设置变量
	m_pIClientKernel=NULL;
	m_pICurrentUserItem=NULL;

	return;
}

//析构函数
CGameFrameUserView::~CGameFrameUserView()
{
}

//设置广告
VOID CGameFrameUserView::SetGameBillInfo(WORD wKindID, WORD wServerID)
{
	//构造地址
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/Ads/GameFrame.asp?KindID=%ld&ServerID=%ld"),szPlatformLink,wKindID,wServerID);

	//打开地址
	m_GameFramePublicize.Navigate(szBillUrl);

	return;
}

//设置用户
VOID CGameFrameUserView::SetUserViewInfo(IClientUserItem * pIClientUserItem)
{
	//设置用户
	if (m_pICurrentUserItem!=pIClientUserItem)
	{
		//设置变量
		m_pICurrentUserItem=pIClientUserItem;

		//更新界面
		if (m_hWnd!=NULL)
		{
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);
		}
	}

	return;
}

//调整控件
VOID CGameFrameUserView::RectifyControl(INT nWidth, INT nHeight)
{
	//获取大小
	CRect rcButton;
	m_btUserMedal.GetWindowRect(&rcButton);

	//计算位置
	INT nXBorad=12L;
	INT nXButtonPos=(nWidth-rcButton.Width()*2L-nXBorad*2L)/3L;

	//调整按钮
	m_btUserMedal.SetWindowPos(NULL,nXBorad+nXButtonPos,nHeight-rcButton.Height()-6,0,0,SWP_NOZORDER|SWP_NOSIZE);
	m_btLoveLiness.SetWindowPos(NULL,nXBorad+nXButtonPos*2+rcButton.Width(),nHeight-rcButton.Height()-6,0,0,SWP_NOZORDER|SWP_NOSIZE);

	//调整广告
	if (m_GameFramePublicize.m_hWnd!=NULL)
	{
		m_GameFramePublicize.SetWindowPos(NULL,BILL_L,BILL_T,nWidth-BILL_L-BILL_R,nHeight-BILL_T-BILL_B,SWP_NOZORDER);
	}

	return;
}

//奖牌兑换
VOID CGameFrameUserView::OnBnClickedUserMedal()
{
	//构造地址
	TCHAR szLogonLink[256]=TEXT("");
	_sntprintf(szLogonLink,CountArray(szLogonLink),TEXT("%s/UserMedal.asp"),szPlatformLink);

	//打开页面
	ShellExecute(NULL,TEXT("OPEN"),szLogonLink,NULL,NULL,SW_NORMAL);

	return;
}

//魅力兑换
VOID CGameFrameUserView::OnBnClickedLoveLiness()
{
	//构造地址
	TCHAR szLogonLink[256]=TEXT("");
	_sntprintf(szLogonLink,CountArray(szLogonLink),TEXT("%s/LoveLiness.asp"),szPlatformLink);

	//打开页面
	ShellExecute(NULL,TEXT("OPEN"),szLogonLink,NULL,NULL,SW_NORMAL);

	return;
}

//绘画背景
BOOL CGameFrameUserView::OnEraseBkgnd(CDC * pDC)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//建立缓冲
	CImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//创建 DC
	CImageDC BufferDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	//设置缓冲
	pBufferDC->SetBkMode(TRANSPARENT);
	pBufferDC->SetTextAlign(TA_LEFT|TA_TOP);
	pBufferDC->SetTextColor(RGB(250,250,250));
	pBufferDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//加载资源
	CImage ImageUserInfoL;
	CImage ImageUserInfoM;
	CImage ImageUserInfoR;
	ImageUserInfoL.LoadFromResource(GetModuleHandle(GAME_FRAME_DLL_NAME),IDB_USER_INFO_L);
	ImageUserInfoM.LoadFromResource(GetModuleHandle(GAME_FRAME_DLL_NAME),IDB_USER_INFO_M);
	ImageUserInfoR.LoadFromResource(GetModuleHandle(GAME_FRAME_DLL_NAME),IDB_USER_INFO_R);

	//变量定义
	INT nXStartPos=ImageUserInfoL.GetWidth();
	INT nXConcludePos=rcClient.Width()-ImageUserInfoR.GetWidth();

	//绘画框架
	ImageUserInfoL.BitBlt(BufferDC,0,0);
	ImageUserInfoR.BitBlt(BufferDC,rcClient.Width()-ImageUserInfoR.GetWidth(),0);

	//绘画中间
	for (INT nXPos=nXStartPos;nXPos<nXConcludePos;nXPos+=ImageUserInfoM.GetWidth())
	{
		INT nCXDraw=__min(ImageUserInfoM.GetWidth(),nXConcludePos-nXPos);
		ImageUserInfoM.BitBlt(BufferDC,nXPos,0,nCXDraw,ImageUserInfoM.GetHeight(),0,0);
	}

	//绘画用户
	if (m_pICurrentUserItem!=NULL)
	{
		//变量定义
		INT nYExcursion=80L;
		TCHAR szString[128]=TEXT("");

		//变量定义
		tagUserInfo * pUserInfo=m_pICurrentUserItem->GetUserInfo();
		tagGameAttribute * pGameAttribute=m_pIClientKernel->GetGameAttribute();
		tagCustomFaceInfo * pCustomFaceInfo=m_pICurrentUserItem->GetCustomFaceInfo();

		//变量定义
		CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
		CFaceItemControl * pFaceItemControl=CFaceItemControl::GetInstance();
		CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();

		//接口变量
		IUserOrderParser * pIUserOrderParser=pGlobalUnits->m_UserOrderParserModule.GetInterface();
		IGameLevelParser * pIGameLevelParser=pGlobalUnits->m_GameLevelParserModule.GetInterface();

		//绘画头像
		if ((pCustomFaceInfo->dwDataSize!=0L)&&(m_pICurrentUserItem->GetCustomID()!=0L))
		{
			pFaceItemControl->DrawFaceNormal(pBufferDC,16,nYExcursion+14,pCustomFaceInfo->dwCustomFace);
		}
		else
		{
			pFaceItemControl->DrawFaceNormal(pBufferDC,16,nYExcursion+14,m_pICurrentUserItem->GetFaceID());
		}

		//位置定义
		CRect rcNickName(72,nYExcursion+14,rcClient.Width()-12,nYExcursion+26);
		CRect rcExperience(72,nYExcursion+33,rcClient.Width()-12,nYExcursion+45);
		CRect rcUserGameID(72,nYExcursion+52,rcClient.Width()-12,nYExcursion+64);

		//位置定义
		CRect rcGameLevel(15,nYExcursion+72,rcClient.Width()-12,nYExcursion+84);
		CRect rcGameDrawInfo(15,nYExcursion+91,rcClient.Width()-12,nYExcursion+103);

		//经验信息
		DWORD dwExperience=m_pICurrentUserItem->GetUserExperience();
		WORD wExperienceLevel=pUserItemElement->GetExperienceLevel(dwExperience);

		//用户信息
		LPCTSTR pszMember=pIUserOrderParser->GetMemberDescribe(m_pICurrentUserItem->GetMemberOrder());
		LPCTSTR pszMaster=pIUserOrderParser->GetMasterDescribe(m_pICurrentUserItem->GetMasterOrder());

		//用户信息
		LPCTSTR pszUserOrder=(pszMaster[0]!=0)?pszMaster:((pszMember[0]!=0)?pszMember:TEXT("普通玩家"));
		_sntprintf(szString,CountArray(szString),TEXT("昵称：%s [ %s ]"),m_pICurrentUserItem->GetNickName(),pszUserOrder);
		pBufferDC->DrawText(szString,lstrlen(szString),&rcNickName,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//用户经验
		_sntprintf(szString,CountArray(szString),TEXT("经验：%d 级"),wExperienceLevel);
		pBufferDC->DrawText(szString,lstrlen(szString),&rcExperience,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//经验标志
		CSize SizeExperience=pDC->GetTextExtent(szString,lstrlen(szString));
		pUserItemElement->DrawExperience(pBufferDC,rcExperience.left+SizeExperience.cx+6,rcExperience.top-4L,dwExperience);

		//游戏标识
		_sntprintf(szString,CountArray(szString),TEXT("号码：%ld"),pUserInfo->dwGameID);
		pBufferDC->DrawText(szString,lstrlen(szString),&rcUserGameID,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//积分等级
		LPCTSTR pszGameLevel=pIGameLevelParser->GetLevelDescribe(m_pICurrentUserItem);
		_sntprintf(szString,CountArray(szString),TEXT("成绩：") SCORE_STRING TEXT(" 等级：%s [ %s ]"),pUserInfo->lScore,pszGameLevel,pGameAttribute->szGameName);
		pBufferDC->DrawText(szString,lstrlen(szString),&rcGameLevel,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//游戏信息
		LONG lPlayCount=m_pICurrentUserItem->GetUserPlayCount();
		float fUserWinRate=m_pICurrentUserItem->GetUserWinRate();
		float fUserFleeRate=m_pICurrentUserItem->GetUserFleeRate();
		_sntprintf(szString,CountArray(szString),TEXT("局数：%ld 胜率：%.2f%% 逃率：%.2f%%"),lPlayCount,fUserWinRate,fUserFleeRate);
		pBufferDC->DrawText(szString,lstrlen(szString),&rcGameDrawInfo,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	}

	//绘画界面
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return TRUE;
}

//位置消息
VOID CGameFrameUserView::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//调整控件
	RectifyControl(cx,cy);

	return;
}

//创建消息
INT CGameFrameUserView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//查询接口
	ASSERT(CGlobalUnits::GetInstance()->m_ClientKernelModule.GetInterface()!=NULL);
	m_pIClientKernel=CGlobalUnits::GetInstance()->m_ClientKernelModule.GetInterface();

	//创建控件
	CRect rcCreate(0,0,0,0);
	m_btUserMedal.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BT_USER_MEDAL);
	m_btLoveLiness.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BT_LOVE_LINESS);

	//设置按钮
	HINSTANCE hInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);
	m_btUserMedal.SetButtonImage(IDB_BT_USER_INFO,TEXT("BT_USER_DEMAL"),hInstance,false,false);
	m_btLoveLiness.SetButtonImage(IDB_BT_USER_INFO,TEXT("BT_LOVE_LINESS"),hInstance,false,false);

	//创建广告
	m_GameFramePublicize.SetBoradColor(RGB(37,127,184));
	m_GameFramePublicize.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcCreate,this,IDC_WEB_PUBLICIZE);

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
