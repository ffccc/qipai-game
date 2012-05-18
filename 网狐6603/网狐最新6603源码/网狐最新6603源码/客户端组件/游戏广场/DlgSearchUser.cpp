#include "StdAfx.h"
#include "Resource.h"
#include "DlgSearchUser.h"
#include "ServerListData.h"
#include "WndViewItemCtrl.h"

//////////////////////////////////////////////////////////////////////////////////

//自定子项
#define UD_USER_STATION				(UD_CUSTOM+0)						//用户位置

//时间定义
#define IDI_SEARCH_TIME_OUT			100									//超时标识
#define TIME_SEARCH_TIME_OUT		15000L								//超时时间

//////////////////////////////////////////////////////////////////////////////////
//静态变量

CDlgSearchUser * CDlgSearchUser::m_pDlgSearchUser=NULL;					//对象指针

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgSearchUser, CSkinDialog)

	//系统消息
	ON_WM_TIMER()

	//按钮消息
	ON_BN_CLICKED(IDC_SEARCH_REMOTE, OnBnClickedSearchRemote)
	ON_BN_CLICKED(IDC_SEARCH_BY_GAME_ID, OnBnClickedSearchByGameID)

	//控件消息
	ON_NOTIFY(NM_RCLICK, IDC_USER_LIST_NATIVE, OnNMRclickNativeList)
	ON_NOTIFY(NM_DBLCLK, IDC_USER_LIST_NATIVE, OnNMDblclkNativeList)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CUserListNative::CUserListNative()
{
}

//析构函数
CUserListNative::~CUserListNative()
{
	//删除数据
	for (INT_PTR i=0;i<m_NativeListDataActive.GetCount();i++)
	{
		SafeDelete(m_NativeListDataActive[i]);
	}

	//删除数据
	for (INT_PTR i=0;i<m_NativeListDataBuffer.GetCount();i++)
	{
		SafeDelete(m_NativeListDataBuffer[i]);
	}

	//删除数组
	m_NativeListDataActive.RemoveAll();
	m_NativeListDataBuffer.RemoveAll();

	return;
}

//控件绑定
VOID CUserListNative::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	//用户昵称
	m_cbDataDescribe[m_wColumnCount]=UD_NICKNAME;
	InsertColumn(m_wColumnCount++,TEXT("用户昵称"),LVCFMT_CENTER,100,0);

	//游戏标识
	m_cbDataDescribe[m_wColumnCount]=UD_GAME_ID;
	InsertColumn(m_wColumnCount++,TEXT("ID"),LVCFMT_CENTER,60,0);

	//用户成绩
	m_cbDataDescribe[m_wColumnCount]=UD_SCORE;
	InsertColumn(m_wColumnCount++,TEXT("成绩"),LVCFMT_CENTER,100,0);

	//用户等级
	m_cbDataDescribe[m_wColumnCount]=UD_GAME_LEVEL;
	InsertColumn(m_wColumnCount++,TEXT("等级"),LVCFMT_CENTER,60,0);

	//用户桌号
	m_cbDataDescribe[m_wColumnCount]=UD_TABLE;
	InsertColumn(m_wColumnCount++,TEXT("桌号"),LVCFMT_CENTER,40,0);

	//用户经验
	m_cbDataDescribe[m_wColumnCount]=UD_USER_MEDAL;
	InsertColumn(m_wColumnCount++,TEXT("奖牌"),LVCFMT_CENTER,85,0);

	//用户经验
	m_cbDataDescribe[m_wColumnCount]=UD_EXPERIENCE;
	InsertColumn(m_wColumnCount++,TEXT("经验值"),LVCFMT_CENTER,85,0);

	//用户魅力
	m_cbDataDescribe[m_wColumnCount]=UD_LOVELINESS;
	InsertColumn(m_wColumnCount++,TEXT("魅力值"),LVCFMT_CENTER,85,0);

	//用户胜率
	m_cbDataDescribe[m_wColumnCount]=UD_WIN_RATE;
	InsertColumn(m_wColumnCount++,TEXT("胜率"),LVCFMT_CENTER,60,0);

	//用户逃率
	m_cbDataDescribe[m_wColumnCount]=UD_LOST_RATE;
	InsertColumn(m_wColumnCount++,TEXT("逃率"),LVCFMT_CENTER,60,0);

	//用户总局
	m_cbDataDescribe[m_wColumnCount]=UD_PLAY_COUNT;
	InsertColumn(m_wColumnCount++,TEXT("总局"),LVCFMT_CENTER,60,0);

	//用户赢局
	m_cbDataDescribe[m_wColumnCount]=UD_WIN_COUNT;
	InsertColumn(m_wColumnCount++,TEXT("赢局"),LVCFMT_CENTER,60,0);

	//用户输局
	m_cbDataDescribe[m_wColumnCount]=UD_LOST_COUNT;
	InsertColumn(m_wColumnCount++,TEXT("输局"),LVCFMT_CENTER,60,0);

	//用户和局
	m_cbDataDescribe[m_wColumnCount]=UD_DRAW_COUNT;
	InsertColumn(m_wColumnCount++,TEXT("和局"),LVCFMT_CENTER,60,0);

	//用户逃局
	m_cbDataDescribe[m_wColumnCount]=UD_FLEE_COUNT;
	InsertColumn(m_wColumnCount++,TEXT("逃局"),LVCFMT_CENTER,60,0);

	//用户社团
	m_cbDataDescribe[m_wColumnCount]=UD_GROUP_NAME;
	InsertColumn(m_wColumnCount++,TEXT("社团"),LVCFMT_CENTER,85,0);

	//用户签名
	m_cbDataDescribe[m_wColumnCount]=UD_UNDER_WRITE;
	InsertColumn(m_wColumnCount++,TEXT("签名"),LVCFMT_CENTER,150,0);

	//当前位置
	m_cbDataDescribe[m_wColumnCount]=UD_USER_STATION;
	InsertColumn(m_wColumnCount++,TEXT("当前位置"),LVCFMT_CENTER,300,0);

	return;
}

//用户解释
IClientUserItem * CUserListNative::GetClientUserItem(VOID * pItemData)
{
	//变量定义
	ASSERT(pItemData!=NULL);
	tagNativeListData * pNativeListData=(tagNativeListData *)pItemData;

	//获取用户
	ASSERT(pNativeListData->pIClientUserItem!=NULL);
	IClientUserItem * pIClientUserItem=pNativeListData->pIClientUserItem;

	return pIClientUserItem;
}

//描述字符
VOID CUserListNative::ConstructString(VOID * pItemData, WORD wColumnIndex, LPTSTR pszString, WORD wMaxCount)
{
	//效验参数
	ASSERT(wColumnIndex<m_wColumnCount);
	if (wColumnIndex>=m_wColumnCount) return;

	//变量定义
	ASSERT(pItemData!=NULL);
	tagNativeListData * pNativeListData=(tagNativeListData *)pItemData;

	//构造字符
	switch (m_cbDataDescribe[wColumnIndex])
	{
	case UD_GAME_LEVEL:		//游戏等级
		{
			//效验状态
			ASSERT(pNativeListData->pIClientUserItem!=NULL);
			ASSERT(pNativeListData->pIGameLevelParser!=NULL);

			//变量定义
			IClientUserItem * pIClientUserItem=pNativeListData->pIClientUserItem;
			IGameLevelParser * pIGameLevelParser=pNativeListData->pIGameLevelParser;

			//获取等级
			_sntprintf(pszString,wMaxCount,TEXT("%s"),pIGameLevelParser->GetLevelDescribe(pIClientUserItem));

			return;
		}
	case UD_USER_STATION:	//当前位置
		{
			//变量定义
			ASSERT(pNativeListData->pServerViewItem!=NULL);
			CServerViewItem * pServerViewItem=pNativeListData->pServerViewItem;

			//构造位置
			LPCTSTR pszKindName=pServerViewItem->GetKindName();
			LPCTSTR pszServerName=pServerViewItem->GetServerName();
			_sntprintf(pszString,wMaxCount,TEXT("正在“%s - %s”游戏房间中"),pszKindName,pszServerName);

			return;
		}
	}

	__super::ConstructString(pItemData,wColumnIndex,pszString,wMaxCount);
}

//插入数据
VOID CUserListNative::InsertUserItem(IClientUserItem * pIClientUserItem, CServerViewItem * pServerViewItem)
{
	//创建数据
	tagNativeListData * pNativeListData=CreateNativeListData();

	//设置接口
	pNativeListData->pServerViewItem=pServerViewItem;
	pNativeListData->pIClientUserItem=pIClientUserItem;
	pNativeListData->pIGameLevelParser=pServerViewItem->GetGameLevelParser();

	//插入列表
	InsertDataItem(pNativeListData);

	return;
}

//更新数据
VOID CUserListNative::UpdateUserItem(IClientUserItem * pIClientUserItem, CServerViewItem * pServerViewItem)
{
	//查找数据
	for (INT_PTR i=0;i<m_NativeListDataActive.GetCount();i++)
	{
		//获取数据
		tagNativeListData * pNativeListData=m_NativeListDataActive[i];

		//数据判断
		if (pNativeListData->pServerViewItem!=pServerViewItem) continue;
		if (pNativeListData->pIClientUserItem!=pIClientUserItem) continue;

		//更新数据
		UpdateDataItem(pNativeListData);

		break;
	}

	return;
}

//删除数据
VOID CUserListNative::DeleteUserItem(IClientUserItem * pIClientUserItem, CServerViewItem * pServerViewItem)
{
	//批量删除
	if (pIClientUserItem==NULL)
	{
		//全部判断
		if (pServerViewItem!=NULL)
		{
			//查找数据
			for (INT_PTR i=0;i<m_NativeListDataActive.GetCount();i++)
			{
				//获取数据
				tagNativeListData * pNativeListData=m_NativeListDataActive[i];

				//删除数据
				if (pNativeListData->pServerViewItem==pServerViewItem)
				{
					DeleteDataItem(pNativeListData);
					DeleteNativeListData(pNativeListData);
				}
			}
		}
		else
		{
			//删除列表
			DeleteAllItems();

			//设置数组
			m_NativeListDataBuffer.Append(m_NativeListDataActive);
			m_NativeListDataActive.RemoveAll();
		}

		return;
	}

	//查找数据
	for (INT_PTR i=0;i<m_NativeListDataActive.GetCount();i++)
	{
		//获取数据
		tagNativeListData * pNativeListData=m_NativeListDataActive[i];

		//数据判断
		if (pNativeListData->pServerViewItem!=pServerViewItem) continue;
		if (pNativeListData->pIClientUserItem!=pIClientUserItem) continue;

		//删除数据
		DeleteDataItem(pNativeListData);

		//删除缓冲
		DeleteNativeListData(pNativeListData);

		break;
	}

	return;
}

//获取数据
tagNativeListData * CUserListNative::CreateNativeListData()
{
	//现存查找
	if (m_NativeListDataBuffer.GetCount()>0L)
	{
		//变量定义
		INT_PTR nIndexBuffer=m_NativeListDataBuffer.GetCount()-1L;
		tagNativeListData * pNativeListData=m_NativeListDataBuffer[nIndexBuffer];

		//整理数据
		m_NativeListDataActive.Add(pNativeListData);
		m_NativeListDataBuffer.RemoveAt(nIndexBuffer);

		//设置数据
		ZeroMemory(pNativeListData,sizeof(tagNativeListData));

		return pNativeListData;
	}

	//创建对象
	try
	{
		//创建数据
		tagNativeListData * pNativeListData=new tagNativeListData;

		//错误判断
		if (pNativeListData==NULL)
		{
			ASSERT(FALSE);
			return NULL;
		}

		//设置数据
		m_NativeListDataActive.Add(pNativeListData);
		ZeroMemory(pNativeListData,sizeof(tagNativeListData));

		return pNativeListData;
	}
	catch (...)
	{
		ASSERT(FALSE);
	}

	return NULL;
}

//删除数据
VOID CUserListNative::DeleteNativeListData(tagNativeListData * pNativeListData)
{
	//激活查找
	for (INT_PTR i=0;i<m_NativeListDataActive.GetCount();i++)
	{
		if (m_NativeListDataActive[i]==pNativeListData)
		{
			m_NativeListDataActive.RemoveAt(i);
			m_NativeListDataBuffer.Add(pNativeListData);
			return;
		}
	}

	//错误断言
	ASSERT(FALSE);

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CUserListRemote::CUserListRemote()
{
	//设置控件
	m_SkinHeaderCtrl.SetLockCount(3L);
	m_SkinHeaderCtrl.SetItemHeight(130L);

	//列表信息
	m_wColumnCount=0;
	ZeroMemory(m_cbDataDescribe,sizeof(m_cbDataDescribe));

	return;
}

//析构函数
CUserListRemote::~CUserListRemote()
{
	//删除数据
	for (INT_PTR i=0;i<m_RemoteListDataActive.GetCount();i++)
	{
		SafeDelete(m_RemoteListDataActive[i]);
	}

	//删除数据
	for (INT_PTR i=0;i<m_RemoteListDataBuffer.GetCount();i++)
	{
		SafeDelete(m_RemoteListDataBuffer[i]);
	}

	//删除数组
	m_RemoteListDataActive.RemoveAll();
	m_RemoteListDataBuffer.RemoveAll();

	return;
}

//控件绑定
VOID CUserListRemote::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	//加载状态
	if (m_ImageUserStatus.m_hImageList==NULL)
	{
		//加载图片
		CBitmap Image;
		BITMAP ImageInfo;
		Image.LoadBitmap(IDB_REMOTE_USER_IMAGE);

		//创建图片
		Image.GetBitmap(&ImageInfo);
		m_ImageUserStatus.Create(20,ImageInfo.bmHeight,ILC_COLOR16|ILC_MASK,0,0);

		//插入图片
		m_ImageUserStatus.Add(&Image,RGB(255,0,255));
	}

	//设置控件
	SetImageList(&m_ImageUserStatus,LVSIL_SMALL);

	//占位标志
	m_cbDataDescribe[m_wColumnCount]=UD_NULL;
	InsertColumn(m_wColumnCount++,TEXT(""),LVCFMT_CENTER,0,0);

	//用户标志
	m_cbDataDescribe[m_wColumnCount]=UD_IMAGE_FLAG;
	InsertColumn(m_wColumnCount++,TEXT(""),LVCFMT_CENTER,20,0);

	//用户性别
	m_cbDataDescribe[m_wColumnCount]=UD_IMAGE_GENDER;
	InsertColumn(m_wColumnCount++,TEXT(""),LVCFMT_CENTER,20,0);

	//用户昵称
	m_cbDataDescribe[m_wColumnCount]=UD_NICKNAME;
	InsertColumn(m_wColumnCount++,TEXT("用户昵称"),LVCFMT_CENTER,100,0);

	//游戏标识
	m_cbDataDescribe[m_wColumnCount]=UD_GAME_ID;
	InsertColumn(m_wColumnCount++,TEXT("ID"),LVCFMT_CENTER,60,0);

	//当前位置
	m_cbDataDescribe[m_wColumnCount]=UD_USER_STATION;
	InsertColumn(m_wColumnCount++,TEXT("当前位置"),LVCFMT_CENTER,300,0);

	return;
}

//排列数据
INT CUserListRemote::SortItemData(LPARAM lParam1, LPARAM lParam2, WORD wColumnIndex, bool bAscendSort)
{
	//定义变量
	tagRemoteListData * pRemoteListData1=(tagRemoteListData *)lParam1;
	tagRemoteListData * pRemoteListData2=(tagRemoteListData *)lParam2;

	//对比数据
	switch (m_cbDataDescribe[wColumnIndex])
	{
	case UD_GAME_ID:		//游戏标识
		{
			//获取变量
			DWORD dwGameID1=pRemoteListData1->dwGameID;
			DWORD dwGameID2=pRemoteListData2->dwGameID;

			//对比数据
			INT nResult=dwGameID1>dwGameID2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	}

	return __super::SortItemData(lParam1,lParam2,wColumnIndex,bAscendSort);
}

//描述字符
VOID CUserListRemote::ConstructString(VOID * pItemData, WORD wColumnIndex, LPTSTR pszString, WORD wMaxCount)
{
	//效验参数
	ASSERT(wColumnIndex<m_wColumnCount);
	if (wColumnIndex>=m_wColumnCount) return;

	//变量定义
	ASSERT(pItemData!=NULL);
	tagRemoteListData * pRemoteListData=(tagRemoteListData *)pItemData;

	//构造字符
	switch (m_cbDataDescribe[wColumnIndex])
	{
	case UD_NICKNAME:		//游戏昵称
		{
			//获取昵称
			_sntprintf(pszString,wMaxCount,TEXT("%s"),pRemoteListData->szNickName);

			return;
		}
	case UD_GAME_ID:		//游戏标识
		{
			//构造标识
			_sntprintf(pszString,wMaxCount,TEXT("%ld"),pRemoteListData->dwGameID);

			return;
		}
	case UD_USER_STATION:	//当前位置
		{
			//构造位置
			LPCTSTR pszKindName=pRemoteListData->szGameKind;
			LPCTSTR pszServerName=pRemoteListData->szGameServer;
			_sntprintf(pszString,wMaxCount,TEXT("正在“%s - %s”游戏房间中"),pszKindName,pszServerName);

			return;
		}
	}

	__super::ConstructString(pItemData,wColumnIndex,pszString,wMaxCount);
}

//绘画数据
VOID CUserListRemote::DrawCustomItem(CDC * pDC, LPDRAWITEMSTRUCT lpDrawItemStruct, CRect & rcSubItem, INT nColumnIndex)
{
	//参数效验
	ASSERT(nColumnIndex<m_wColumnCount);
	if (nColumnIndex>=m_wColumnCount) return;

	//变量定义
	INT nItemID=lpDrawItemStruct->itemID;
	tagRemoteListData * pRemoteListData=(tagRemoteListData *)(lpDrawItemStruct->itemData);

	//绘画信息
	switch (m_cbDataDescribe[nColumnIndex])
	{
	case UD_IMAGE_FLAG:		//用户标志
		{
			//计算索引
			WORD wImageIndex=2;
			if (pRemoteListData->cbMemberOrder!=0) wImageIndex=3+__min(pRemoteListData->cbMemberOrder,4);
			if (pRemoteListData->cbMasterOrder!=0) wImageIndex=8+__min(pRemoteListData->cbMasterOrder,4);

			//绘画标志
			m_ImageUserStatus.Draw(pDC,wImageIndex,CPoint(rcSubItem.left,rcSubItem.top),ILD_TRANSPARENT);

			break;
		}
	case UD_IMAGE_GENDER:	//用户性别
		{
			//计算索引
			WORD wImageIndex=0;
			if (pRemoteListData->cbGender==GENDER_MANKIND) wImageIndex=1;

			//绘画标志
			m_ImageUserStatus.Draw(pDC,wImageIndex,CPoint(rcSubItem.left,rcSubItem.top),ILD_TRANSPARENT);
			
			break;
		}
	default:				//默认调用
		{
			__super::DrawCustomItem(pDC,lpDrawItemStruct,rcSubItem,nColumnIndex);
		}
	}

	return;
}

//插入数据
VOID CUserListRemote::InsertUserItem(tagUserRemoteInfo * pUserRemoteInfo, LPCTSTR pszKindName)
{
	//创建数据
	tagRemoteListData * pRemoteListData=CreateRemoteListData();

	//辅助信息
	pRemoteListData->cbGender=pUserRemoteInfo->cbGender;
	pRemoteListData->cbMemberOrder=pUserRemoteInfo->cbMemberOrder;
	pRemoteListData->cbMasterOrder=pUserRemoteInfo->cbMasterOrder;

	//用户信息
	pRemoteListData->dwUserID=pUserRemoteInfo->dwUserID;
	pRemoteListData->dwGameID=pUserRemoteInfo->dwGameID;
	lstrcpyn(pRemoteListData->szNickName,pUserRemoteInfo->szNickName,CountArray(pRemoteListData->szNickName));

	//位置信息
	pRemoteListData->wKindID=pUserRemoteInfo->wKindID;
	pRemoteListData->wServerID=pUserRemoteInfo->wServerID;
	lstrcpyn(pRemoteListData->szGameKind,pszKindName,CountArray(pRemoteListData->szGameKind));
	lstrcpyn(pRemoteListData->szGameServer,pUserRemoteInfo->szGameServer,CountArray(pRemoteListData->szGameServer));

	//插入列表
	InsertDataItem(pRemoteListData);

	return;
}

//获取数据
tagRemoteListData * CUserListRemote::CreateRemoteListData()
{
	//现存查找
	if (m_RemoteListDataBuffer.GetCount()>0L)
	{
		//变量定义
		INT_PTR nIndexBuffer=m_RemoteListDataBuffer.GetCount()-1L;
		tagRemoteListData * pRemoteListData=m_RemoteListDataBuffer[nIndexBuffer];

		//整理数据
		m_RemoteListDataActive.Add(pRemoteListData);
		m_RemoteListDataBuffer.RemoveAt(nIndexBuffer);

		//设置数据
		ZeroMemory(pRemoteListData,sizeof(tagRemoteListData));

		return pRemoteListData;
	}

	//创建对象
	try
	{
		//创建数据
		tagRemoteListData * pRemoteListData=new tagRemoteListData;

		//错误判断
		if (pRemoteListData==NULL)
		{
			ASSERT(FALSE);
			return NULL;
		}

		//设置数据
		m_RemoteListDataActive.Add(pRemoteListData);
		ZeroMemory(pRemoteListData,sizeof(tagRemoteListData));

		return pRemoteListData;
	}
	catch (...)
	{
		ASSERT(FALSE);
	}

	return NULL;
}

//删除数据
VOID CUserListRemote::DeleteRemoteListData(tagRemoteListData * pRemoteListData)
{
	//激活查找
	for (INT_PTR i=0;i<m_RemoteListDataActive.GetCount();i++)
	{
		if (m_RemoteListDataActive[i]==pRemoteListData)
		{
			m_RemoteListDataActive.RemoveAt(i);
			m_RemoteListDataBuffer.Add(pRemoteListData);
			return;
		}
	}

	//错误断言
	ASSERT(FALSE);

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgSearchUser::CDlgSearchUser() : CSkinDialog(IDD_DLG_SEARCH_USER)
{
	//设置任务
	m_MissionManager.InsertMissionItem(this);

	//设置变量
	m_dwGameID=0L;
	ZeroMemory(m_szNickName,sizeof(m_szNickName));

	//设置变量
	ASSERT(m_pDlgSearchUser==NULL);
	if (m_pDlgSearchUser==NULL) m_pDlgSearchUser=this;

	return;
}

//析构函数
CDlgSearchUser::~CDlgSearchUser()
{
	//设置变量
	ASSERT(m_pDlgSearchUser==this);
	if (m_pDlgSearchUser==this) m_pDlgSearchUser=NULL;

	return;
}

//控件绑定
VOID CDlgSearchUser::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	//按钮控件
	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Control(pDX, IDCANCEL, m_btCancel);

	//查找控件
	DDX_Control(pDX, IDC_GAME_ID, m_edGameID);
	DDX_Control(pDX, IDC_NICKNAME, m_edNickName);
	DDX_Control(pDX, IDC_SEARCH_AREA, m_ComSearchArea);

	//控件变量
	DDX_Control(pDX, IDC_SEARCH_REMOTE, m_SearchRemote);
	DDX_Control(pDX, IDC_SEARCH_BY_GAME_ID, m_SearchByGameID);

	//控件变量
	DDX_Control(pDX, IDC_STATIC_IMAGE, m_StaticImage);
	DDX_Control(pDX, IDC_USER_LIST_REMOTE, m_UserListRemote);
	DDX_Control(pDX, IDC_USER_LIST_NATIVE, m_UserListNative);
}

//消息过虑
BOOL CDlgSearchUser::PreTranslateMessage(MSG * pMsg)
{
	//发送聊天
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		//设置焦点
		if (m_edGameID.IsWindowVisible()==TRUE)
		{
			m_edGameID.SetFocus();
			m_edGameID.SetSel(0L,-1L);
		}

		//设置焦点
		if (m_edNickName.IsWindowVisible()==TRUE)
		{
			m_edNickName.SetFocus();
			m_edNickName.SetSel(0L,-1L);
		}

		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//创建函数
BOOL CDlgSearchUser::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("用户查找"));

	//设置控件
	m_edGameID.LimitText(10);
	m_edNickName.LimitText(LEN_ACCOUNTS-1);

	//房间范围
	ASSERT(CWndViewItemCtrl::GetInstance()!=NULL);
	CWndViewItemCtrl * pWndViewItemCtrl=CWndViewItemCtrl::GetInstance();

	//插入列表
	LPCTSTR pszString=TEXT("当前您所在的全部游戏房间");
	m_ComSearchArea.SetCurSel(m_ComSearchArea.InsertString(0,pszString));

	//房间列表
	for (WORD i=1;i<pWndViewItemCtrl->GetViewItemCount();i++)
	{
		//获取房间
		tagViewItemInfo * pViewItemInfo=pWndViewItemCtrl->GetViewItemInfo(i);

		//插入列表
		if (pViewItemInfo!=NULL)
		{
			//获取对象
			ASSERT(pViewItemInfo->pItemView!=NULL);
			CServerViewItem * pServerViewItem=(CServerViewItem *)pViewItemInfo->pItemView;

			//构造列表
			TCHAR szItemString[256]=TEXT("");
			_sntprintf(szItemString,CountArray(szItemString),TEXT("%s - %s"),pServerViewItem->GetKindName(),pServerViewItem->GetServerName());

			//插入列表
			INT nItem=m_ComSearchArea.InsertString(m_ComSearchArea.GetCount(),szItemString);
			if (nItem!=LB_ERR) m_ComSearchArea.SetItemData(nItem,pServerViewItem->GetServerID());
		}
	}

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//本地列表
	m_UserListNative.SetMySelfUserID(pGlobalUserData->dwUserID);

	//调整控件
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());

	return FALSE;
}

//确定消息
VOID CDlgSearchUser::OnOK()
{
	//模式变量
	bool bRemoteMode=(m_SearchRemote.GetCheck()==BST_CHECKED);
	bool bGameIDMode=(m_SearchByGameID.GetCheck()==BST_CHECKED);

	//获取输入
	CString strSearchKey;
	if (m_edGameID.IsWindowVisible()==TRUE) m_edGameID.GetWindowText(strSearchKey);
	if (m_edNickName.IsWindowVisible()==TRUE) m_edNickName.GetWindowText(strSearchKey);

	//输入处理
	strSearchKey.TrimLeft();
	strSearchKey.TrimRight();
	strSearchKey.MakeLower();

	//远程判断
	if (GetActiveStatus()==true)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("远程查找操作正在进行中，请稍后再进行新的查找提交！"),MB_ICONQUESTION,30L);

		return;
	}

	//效验输入
	if ((_tstol(strSearchKey)==0L)&&(bGameIDMode==true))
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("请输入您要查找的“游戏 I D”号码！"),MB_ICONSTOP);

		//设置焦点
		m_edGameID.SetFocus();

		return;
	}

	//效验输入
	if ((strSearchKey.IsEmpty()==TRUE)&&(bGameIDMode==false))
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("请输入完整的“用户昵称”或者其中的部分字符！"),MB_ICONSTOP);

		//设置焦点
		m_edNickName.SetFocus();

		return;
	}

	//列表清理
	m_UserListRemote.DeleteAllItems();
	m_UserListNative.DeleteUserItem(NULL,NULL);

	//列表控制
	if (bRemoteMode==true)
	{
		m_UserListRemote.ShowWindow(SW_SHOW);
		m_UserListNative.ShowWindow(SW_HIDE);
	}
	else
	{
		m_UserListNative.ShowWindow(SW_SHOW);
		m_UserListRemote.ShowWindow(SW_HIDE);
	}

	//查找处理
	if (bRemoteMode==false)
	{
		//变量定义
		ASSERT(CWndViewItemCtrl::GetInstance()!=NULL);
		CWndViewItemCtrl * pWndViewItemCtrl=CWndViewItemCtrl::GetInstance();

		//房间范围
		INT nItem=m_ComSearchArea.GetCurSel();
		WORD wServerID=(WORD)m_ComSearchArea.GetItemData(nItem);

		//设置列表
		for (WORD i=1;i<pWndViewItemCtrl->GetViewItemCount();i++)
		{
			//获取房间
			tagViewItemInfo * pViewItemInfo=pWndViewItemCtrl->GetViewItemInfo(i);

			//变量定义
			CServerViewItem * pServerViewItem=(CServerViewItem *)pViewItemInfo->pItemView;
			IPlazaUserManager * pIPlazaUserManager=pServerViewItem->GetPlazaUserManager();

			//房间过滤
			if ((wServerID!=0)&&(pServerViewItem->GetServerID()!=wServerID))
			{
				continue;
			}

			//搜索用户
			if (bGameIDMode==false)
			{
				//变量定义
				WORD wEmunIndex=0;
				CString strNickName;

				//搜索用户
				do
				{
					//枚举用户
					IClientUserItem * pIClientUserItem=pIPlazaUserManager->EnumUserItem(wEmunIndex++);

					//用户判断
					if (pIClientUserItem==NULL)
					{
						break;
					}

					//获取昵称
					strNickName=pIClientUserItem->GetNickName();
					strNickName.MakeLower();

					//用户判断
					if (strNickName.Find(strSearchKey)!=-1)
					{
						m_UserListNative.InsertUserItem(pIClientUserItem,pServerViewItem);
					}

				} while (true);
			}
			else
			{
				//转换 I D
				DWORD dwGameID=_tstol(strSearchKey);

				//搜索用户
				IClientUserItem * pIClientUserItem=pIPlazaUserManager->SearchUserByGameID(dwGameID);
				if (pIClientUserItem!=NULL) m_UserListNative.InsertUserItem(pIClientUserItem,pServerViewItem);
			}
		}

		//结果判断
		if (m_UserListNative.GetItemCount()==0L)
		{
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("您所查询的用户不存在或者没有在线，请更改查询条件再试！"),MB_ICONQUESTION,30L);
		}

		//设置焦点
		if (m_edGameID.IsWindowVisible()==TRUE)
		{
			m_edGameID.SetFocus();
			m_edGameID.SetSel(0L,-1L);
		}

		//设置焦点
		if (m_edNickName.IsWindowVisible()==TRUE)
		{
			m_edNickName.SetFocus();
			m_edNickName.SetSel(0L,-1L);
		}
	}
	else
	{
		//设置变量
		if (bGameIDMode==true)
		{
			m_dwGameID=_tstol(strSearchKey);
			ZeroMemory(m_szNickName,sizeof(m_szNickName));
		}
		else
		{
			m_dwGameID=0L;
			lstrcpyn(m_szNickName,strSearchKey,CountArray(m_szNickName));
		}

		//发起连接
		if (m_MissionManager.AvtiveMissionItem(this,false)==true) 
		{
			//设置时间
			SetTimer(IDI_SEARCH_TIME_OUT,TIME_SEARCH_TIME_OUT,NULL);
		}
	}

	return;
}

//房间关闭
VOID CDlgSearchUser::OnViewItemDelete(CServerViewItem * pServerViewItem)
{
	//删除数据
	if (m_UserListNative.GetItemCount()>0L)
	{
		m_UserListNative.DeleteUserItem(NULL,pServerViewItem);
	}

	return;
}

//用户删除
VOID CDlgSearchUser::OnUserItemDelete(IClientUserItem * pIClientUserItem, CServerViewItem * pServerViewItem)
{
	//删除数据
	if (m_UserListNative.GetItemCount()>0L)
	{
		m_UserListNative.DeleteUserItem(pIClientUserItem,pServerViewItem);
	}

	return;
}

//用户更新
VOID CDlgSearchUser::OnUserItemUpdate(IClientUserItem * pIClientUserItem, CServerViewItem * pServerViewItem)
{
	//更新数据
	if (m_UserListNative.GetItemCount()>0L)
	{
		m_UserListNative.UpdateUserItem(pIClientUserItem,pServerViewItem);
	}

	return;
}

//调整控件
VOID CDlgSearchUser::RectifyControl(INT nWidth, INT nHeight)
{
	//框架位置
	INT nTBorder=m_SkinAttribute.m_EncircleInfoFrame.nTBorder;
	INT nBBorder=m_SkinAttribute.m_EncircleInfoFrame.nBBorder;
	INT nLBorder=m_SkinAttribute.m_EncircleInfoFrame.nLBorder;
	INT nRBorder=m_SkinAttribute.m_EncircleInfoFrame.nRBorder;

	//列表位置
	CRect rcListControl;
	rcListControl.top=nTBorder+150L;
	rcListControl.left=nLBorder+155L;
	rcListControl.right=nWidth-nRBorder-10L;
	rcListControl.bottom=nHeight-nBBorder-5L;

	//移动控件
	m_StaticImage.SetWindowPos(NULL,nLBorder,nTBorder,0,0,SWP_NOZORDER|SWP_NOSIZE);
	m_UserListRemote.SetWindowPos(NULL,rcListControl.left,rcListControl.top,rcListControl.Width(),rcListControl.Height(),SWP_NOZORDER);
	m_UserListNative.SetWindowPos(NULL,rcListControl.left,rcListControl.top,rcListControl.Width(),rcListControl.Height(),SWP_NOZORDER);

	return;
}

//绘画消息
VOID CDlgSearchUser::OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight)
{
	//框架位置
	INT nTBorder=m_SkinAttribute.m_EncircleInfoFrame.nTBorder;
	INT nBBorder=m_SkinAttribute.m_EncircleInfoFrame.nBBorder;
	INT nLBorder=m_SkinAttribute.m_EncircleInfoFrame.nLBorder;
	INT nRBorder=m_SkinAttribute.m_EncircleInfoFrame.nRBorder;

	//列表位置
	CRect rcListControl;
	rcListControl.top=nTBorder+149L;
	rcListControl.left=nLBorder+154L;
	rcListControl.right=nWidth-nRBorder-9L;
	rcListControl.bottom=nHeight-nBBorder-4L;

	//绘画线条
	pDC->Draw3dRect(&rcListControl,RGB(84,165,213),RGB(84,165,213));

	//绘画图片
	CPngImage ImageSearchLogo;
	ImageSearchLogo.LoadImage(AfxGetInstanceHandle(),TEXT("SEARCH_LOGO"));
	ImageSearchLogo.DrawImage(pDC,rcListControl.left+6,rcListControl.top-ImageSearchLogo.GetHeight()-5);

	return;
}

//连接事件
bool CDlgSearchUser::OnEventMissionLink(INT nErrorCode)
{
	//连接结果
	if (nErrorCode!=0)
	{
		//重试任务
		if (m_MissionManager.AvtiveMissionItem(this,true)==true)
		{
			return true;
		}

		//删除时间
		KillTimer(IDI_SEARCH_TIME_OUT);

		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("连接服务器超时，远程查找操作失败！"),MB_OK|MB_ICONSTOP,30);

		return true;
	}

	//变量定义
	CMD_GP_C_SearchCorrespond SearchCorrespond;
	ZeroMemory(&SearchCorrespond,sizeof(SearchCorrespond));

	//构造数据
	SearchCorrespond.dwGameID=m_dwGameID;
	lstrcpyn(SearchCorrespond.szNickName,m_szNickName,CountArray(SearchCorrespond.szNickName));

	//发送数据
	m_MissionManager.SendData(MDM_GP_REMOTE_SERVICE,SUB_GP_C_SEARCH_CORRESPOND,&SearchCorrespond,sizeof(SearchCorrespond));

	return true;
}

//关闭事件
bool CDlgSearchUser::OnEventMissionShut(BYTE cbShutReason)
{
	//提示消息
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		//重试任务
		if (m_MissionManager.AvtiveMissionItem(this,true)==true)
		{
			return true;
		}

		//删除时间
		KillTimer(IDI_SEARCH_TIME_OUT);

		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("与服务器的连接异常断开了，远程查找操作失败！"),MB_ICONERROR,30L);
	}
	else
	{
		//删除时间
		KillTimer(IDI_SEARCH_TIME_OUT);
	}
	
	return true;
}

//读取事件
bool CDlgSearchUser::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//命令处理
	if (Command.wMainCmdID==MDM_GP_REMOTE_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_S_SEARCH_CORRESPOND:	//查找结果
			{
				//变量定义
				CMD_GP_S_SearchCorrespond * pSearchCorrespond=(CMD_GP_S_SearchCorrespond *)pData;

				//效验参数
				ASSERT(wDataSize<=sizeof(CMD_GP_S_SearchCorrespond));
				ASSERT(wDataSize>=(sizeof(CMD_GP_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)));
				ASSERT(wDataSize==(sizeof(CMD_GP_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)+pSearchCorrespond->wUserCount*sizeof(pSearchCorrespond->UserRemoteInfo[0])));

				//效验参数
				if (wDataSize>sizeof(CMD_GP_S_SearchCorrespond)) return false;
				if (wDataSize<(sizeof(CMD_GP_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo))) return false;
				if (wDataSize!=(sizeof(CMD_GP_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)+pSearchCorrespond->wUserCount*sizeof(pSearchCorrespond->UserRemoteInfo[0]))) return false;

				//失败处理
				KillTimer(IDI_SEARCH_TIME_OUT);

				//关闭连接
				m_MissionManager.ConcludeMissionItem(this,false);

				//插入列表
				for (WORD i=0;i<pSearchCorrespond->wUserCount;i++)
				{
					//查找类型
					CServerListData * pServerListData=CServerListData::GetInstance();
					CGameKindItem * pGameKindItem=pServerListData->SearchGameKind(pSearchCorrespond->UserRemoteInfo[i].wKindID);

					//插入列表
					if (pGameKindItem!=NULL)
					{
						LPCTSTR pszGameKind=pGameKindItem->m_GameKind.szKindName;
						m_UserListRemote.InsertUserItem(&pSearchCorrespond->UserRemoteInfo[i],pszGameKind);
					}
				}

				//结果判断
				if (m_UserListRemote.GetItemCount()==0L)
				{
					CInformation Information(this);
					Information.ShowMessageBox(TEXT("您所查询的用户不存在或者没有在线，请更改查询条件再试！"),MB_ICONQUESTION,30L);
				}

				return true;
			}
		}
	}

	return true;
}

//时间消息
VOID CDlgSearchUser::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case IDI_SEARCH_TIME_OUT:	//超时时间
		{
			//删除时间
			KillTimer(IDI_SEARCH_TIME_OUT);

			//关闭连接
			m_MissionManager.ConcludeMissionItem(this,false);

			//提示消息
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("服务器超时没有响应，远程查找操作失败，请稍后重试！"),MB_OK|MB_ICONSTOP,30L);

			return;
		}
	}

	__super::OnTimer(nIDEvent);
}

//远程查找
VOID CDlgSearchUser::OnBnClickedSearchRemote()
{
	//获取状态
	UINT nCurrentCheck=m_SearchRemote.GetCheck();

	//会员判断
	if (nCurrentCheck==BST_UNCHECKED)
	{
		//变量定义
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//会员判断
		if (pGlobalUserData->cbMemberOrder==0)
		{
			//提示消息
			CInformation Information(this);
			INT_PTR nResult=Information.ShowMessageBox(TEXT("您不是会员，无法使用远程查找功能，需要立即成功会员吗？"),MB_ICONQUESTION|MB_YESNO);

			//会员连接
			if (nResult==IDYES)
			{
				//更新窗口
				UpdateWindow();

				//构造地址
				TCHAR szLinkUrl[128]=TEXT("");
				_sntprintf(szLinkUrl,CountArray(szLinkUrl),TEXT("%s/PayIndex.asp"),szPlatformLink);

				//连接地址
				ShellExecute(NULL,TEXT("OPEN"),szLinkUrl,NULL,NULL,SW_NORMAL);
			}

			return;
		}
	}

	//设置焦点
	if (m_edGameID.IsWindowVisible()==TRUE) m_edGameID.SetFocus();
	if (m_edNickName.IsWindowVisible()==TRUE) m_edNickName.SetFocus();

	//设置控件
	m_ComSearchArea.EnableWindow((nCurrentCheck==BST_CHECKED)?TRUE:FALSE);
	m_SearchRemote.SetCheck((nCurrentCheck==BST_UNCHECKED)?BST_CHECKED:BST_UNCHECKED);

	return;
}

//标识模式
VOID CDlgSearchUser::OnBnClickedSearchByGameID()
{
	//模式判断
	if (m_SearchByGameID.GetCheck()==BST_CHECKED)
	{
		//设置控件
		m_edGameID.ShowWindow(SW_SHOW);
		m_edNickName.ShowWindow(SW_HIDE);

		//设置焦点
		m_edGameID.SetFocus();

		//设置标题
		SetDlgItemText(IDC_SEARCH_NAME,TEXT("请输入所要查找的用户 ID："));
	}
	else
	{
		//设置控件
		m_edGameID.ShowWindow(SW_HIDE);
		m_edNickName.ShowWindow(SW_SHOW);

		//设置焦点
		m_edNickName.SetFocus();

		//设置标题
		SetDlgItemText(IDC_SEARCH_NAME,TEXT("请输入所要查找的用户昵称："));
	}

	return;
}

//右键列表
VOID CDlgSearchUser::OnNMRclickNativeList(NMHDR * pNMHDR, LRESULT * pResult)
{
	//变量定义
	NMITEMACTIVATE * pListNotify=(NMITEMACTIVATE *)pNMHDR;
	
	//弹出菜单
	if (pListNotify->iItem!=-1)
	{
		//获取数据
		INT nItem=pListNotify->iItem;
		tagNativeListData * pNativeListData=(tagNativeListData *)m_UserListNative.GetItemData(nItem);

		//弹出菜单
		if (pNativeListData!=NULL)
		{
			ASSERT(pNativeListData->pServerViewItem!=NULL);
			ASSERT(pNativeListData->pIClientUserItem!=NULL);
			pNativeListData->pServerViewItem->TrackUserItemMenu(pNativeListData->pIClientUserItem);
		}
	}

	return;
}

//双击列表
VOID CDlgSearchUser::OnNMDblclkNativeList(NMHDR * pNMHDR, LRESULT * pResult)
{
	//变量定义
	NMITEMACTIVATE * pListNotify=(NMITEMACTIVATE *)pNMHDR;
	
	//消息处理
	if ((pListNotify->iItem!=-1)&&(pListNotify->lParam!=NULL))
	{
		//获取数据
		INT nItem=pListNotify->iItem;
		tagNativeListData * pNativeListData=(tagNativeListData *)m_UserListNative.GetItemData(nItem);

		//执行动作
		if ((pNativeListData!=NULL)&&(pNativeListData->pIClientUserItem!=NULL))
		{
			//变量定义
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

			//执行动作
			switch (pParameterGlobal->m_cbActionLeftDoubleList)
			{
			case ACTION_ORIENTATION:			//定位用户
				{
					//获取属性
					WORD wTableID=pNativeListData->pIClientUserItem->GetTableID();
					WORD wChairID=pNativeListData->pIClientUserItem->GetChairID();
					BYTE cbUserStatus=pNativeListData->pIClientUserItem->GetUserStatus();

					//定位位置
					if (wTableID!=INVALID_TABLE)
					{
						//变量定义
						ASSERT(pNativeListData->pServerViewItem!=NULL);
						CServerViewItem * pServerViewItem=pNativeListData->pServerViewItem;

						//变量定义
						ASSERT(CWndViewItemCtrl::GetInstance()!=NULL);
						CWndViewItemCtrl * pWndViewItemCtrl=CWndViewItemCtrl::GetInstance();

						//设置可视
						pWndViewItemCtrl->ActiveViewItem(pServerViewItem);
						pServerViewItem->m_TableViewFrame.VisibleTable(wTableID);

						//闪动椅子
						if (cbUserStatus==US_LOOKON)
						{
							pServerViewItem->m_TableViewFrame.FlashGameTable(wTableID);
						}
						else
						{
							pServerViewItem->m_TableViewFrame.FlashGameChair(wTableID,wChairID);
						}
					}

					break;
				}
			case ACTION_SEND_WHISPER:			//发送私聊
				{
					//效验参数
					ASSERT(pNativeListData->pServerViewItem!=NULL);
					ASSERT(pNativeListData->pIClientUserItem!=NULL);

					//变量定义
					CServerViewItem * pServerViewItem=pNativeListData->pServerViewItem;
					IClientUserItem * pIClientUserItem=pNativeListData->pIClientUserItem;

					//变量定义
					CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
					tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

					//状态判断
					if (pGlobalUserData->dwUserID==pIClientUserItem->GetUserID()) break;
					if (pServerViewItem->GetServiceStatus()!=ServiceStatus_ServiceIng) break;

					//创建私聊
					IClientUserItem * pIClientUserItemArray[]={pIClientUserItem};
					pServerViewItem->WhisperConversation(pIClientUserItemArray,CountArray(pIClientUserItemArray));

					break;
				}
			case ACTION_SHOW_USER_INFO:		//用户信息
				{
					break;
				}
			}
		}
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
