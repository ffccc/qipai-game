#include "Stdafx.h"
#include "Resource.h"
#include "DlgServerItem.h"

//////////////////////////////////////////////////////////////////////////////////

//列表属性
#define LIST_STYTE LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_FLATSB

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgServerItem, CDialog)

	//按钮消息
	ON_BN_CLICKED(IDC_LOAD_SERVER, OnBnClickedLoadServer)
	ON_BN_CLICKED(IDC_RELOAD_LIST, OnBnClickedReLoadList)
	ON_BN_CLICKED(IDC_DELETE_SERVER, OnBnClickedDeleteServer)
	ON_BN_CLICKED(IDC_SERVICE_ATTRIB, OnBnClickedServiceAttrib)

	//控件消息
	ON_NOTIFY(NM_DBLCLK, IDC_SERVER_LIST, OnNMDblclkServerList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SERVER_LIST, OnLvnItemChangedServerList)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgServerItem::CDlgServerItem() : CDialog(IDD_SERVER_ITEM)
{
	//模块参数
	ZeroMemory(&m_ModuleInitParameter,sizeof(m_ModuleInitParameter));

	return;
}

//析构函数
CDlgServerItem::~CDlgServerItem()
{
}

//控件绑定
VOID CDlgServerItem::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERVER_LIST, m_ServerListControl);
}

//消息解释
BOOL CDlgServerItem::PreTranslateMessage(MSG * pMsg)
{
	//按键过滤
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//初始化函数
BOOL CDlgServerItem::OnInitDialog()
{
	__super::OnInitDialog();

	//设置资源
	AfxSetResourceHandle(GetModuleHandle(NULL));

	//加载房间
	LoadDBServerItem();

	return TRUE;
}

//确定消息
VOID CDlgServerItem::OnOK()
{
	return;
}

//打开房间
bool CDlgServerItem::OpenGameServer()
{
	//设置资源
	AfxSetResourceHandle(GetModuleHandle(MODULE_MANAGER_DLL_NAME));

	//配置房间
	if (DoModal()==IDC_LOAD_SERVER)
	{
		return true;
	}

	return false;
}

//打开房间
bool CDlgServerItem::OpenGameServer(WORD wServerID)
{
	//变量定义
	tagGameServerInfo GameServerResult;
	ZeroMemory(&GameServerResult,sizeof(GameServerResult));

	//机器标识
	TCHAR szMachineID[LEN_MACHINE_ID];
	CWHService::GetMachineID(szMachineID);

	//加载房间
	CServerInfoManager ServerInfoManager;
	if (ServerInfoManager.LoadGameServerInfo(szMachineID,wServerID,GameServerResult)==false)
	{
		return false;
	}

	//获取参数
	if (GetModuleInitParameter(&GameServerResult,true)==false)
	{
		return false;
	}

	return true;
}

//加载列表
bool CDlgServerItem::LoadDBServerItem()
{
	//重置列表
	m_ServerListControl.DeleteAllItems();

	//设置按钮
	GetDlgItem(IDC_LOAD_SERVER)->EnableWindow(FALSE);
	GetDlgItem(IDC_DELETE_SERVER)->EnableWindow(FALSE);
	GetDlgItem(IDC_SERVICE_ATTRIB)->EnableWindow(FALSE);

	//机器标识
	TCHAR szMachineID[LEN_MACHINE_ID];
	CWHService::GetMachineID(szMachineID);

	//加载信息
	CServerInfoManager ServerInfoManager;
	if (ServerInfoManager.LoadGameServerInfo(szMachineID,m_ServerInfoBuffer)==true)
	{
		//变量定义
		POSITION Position=m_ServerInfoBuffer.m_GameServerInfoMap.GetStartPosition();

		//枚举模块
		while (Position!=NULL)
		{
			//获取对象
			WORD wServerID=0L;
			tagGameServerInfo * pGameServerInfo=NULL;
			m_ServerInfoBuffer.m_GameServerInfoMap.GetNextAssoc(Position,wServerID,pGameServerInfo);

			//插入列表
			ASSERT(pGameServerInfo!=NULL);
			if (pGameServerInfo!=NULL) m_ServerListControl.InsertServerInfo(pGameServerInfo);
		}

		return true;
	}

	return false;
}

//更新按钮
bool CDlgServerItem::UpdateControlStatus()
{
	//获取选择
	POSITION Position=m_ServerListControl.GetFirstSelectedItemPosition();

	//设置列表
	if (Position!=NULL)
	{
		//获取房间
		INT nListItem=m_ServerListControl.GetNextSelectedItem(Position);
		tagGameServerInfo * pGameServerInfo=(tagGameServerInfo *)m_ServerListControl.GetItemData(nListItem);

		//设置按钮
		if (pGameServerInfo->dwNativeVersion!=0L)
		{
			GetDlgItem(IDC_LOAD_SERVER)->EnableWindow(TRUE);
			GetDlgItem(IDC_DELETE_SERVER)->EnableWindow(TRUE);
			GetDlgItem(IDC_SERVICE_ATTRIB)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_LOAD_SERVER)->EnableWindow(FALSE);
			GetDlgItem(IDC_DELETE_SERVER)->EnableWindow(TRUE);
			GetDlgItem(IDC_SERVICE_ATTRIB)->EnableWindow(FALSE);
		}
	}
	else
	{
		//设置按钮
		GetDlgItem(IDC_LOAD_SERVER)->EnableWindow(FALSE);
		GetDlgItem(IDC_DELETE_SERVER)->EnableWindow(FALSE);
		GetDlgItem(IDC_SERVICE_ATTRIB)->EnableWindow(FALSE);
	}

	return true;
}

//获取参数
bool CDlgServerItem::GetModuleInitParameter(tagGameServerInfo * pGameServerInfo, bool bAutoMode)
{
	//效验参数
	ASSERT(pGameServerInfo!=NULL);
	if (pGameServerInfo==NULL) return false;

	//游戏模块
	CGameServiceManagerHelper GameServiceManager;
	GameServiceManager.SetModuleCreateInfo(pGameServerInfo->szServerDLLName,GAME_SERVICE_CREATE_NAME);

	//创建判断
	if (pGameServerInfo->dwNativeVersion==0)
	{
		//构造提示
		TCHAR szString[128]=TEXT("");
		_sntprintf(szString,CountArray(szString),TEXT("[ %s ] 游戏服务器组件还没有安装，请先安装对应的游戏服务器"),pGameServerInfo->szGameName);

		//提示消息
		if (bAutoMode==true) CTraceService::TraceString(szString,TraceLevel_Exception);
		else AfxMessageBox(szString,MB_ICONERROR);

		return false;
	}

	//更新判断
	if (pGameServerInfo->dwNativeVersion!=pGameServerInfo->dwServerVersion)
	{
		//构造提示
		TCHAR szString[128]=TEXT("");
		_sntprintf(szString,CountArray(szString),TEXT("[ %s ] 游戏服务器组件已经更新了，不能继续用于启动房间"),pGameServerInfo->szGameName);

		//提示消息
		if (bAutoMode==true) CTraceService::TraceString(szString,TraceLevel_Exception);
		else AfxMessageBox(szString,MB_ICONERROR);

		return false;
	}

	//加载模块
	if (GameServiceManager.CreateInstance()==false)
	{
		//构造提示
		TCHAR szString[128]=TEXT("");
		_sntprintf(szString,CountArray(szString),TEXT("[ %s ] 服务组件不存在或者加载失败，请重新安装服务组件"),pGameServerInfo->szGameName);

		//提示消息
		if (bAutoMode==true) CTraceService::TraceString(szString,TraceLevel_Exception);
		else AfxMessageBox(szString,MB_ICONERROR);

		return false;
	}

	//模块属性
	GameServiceManager->GetServiceAttrib(m_ModuleInitParameter.GameServiceAttrib);

	//挂接属性
	m_ModuleInitParameter.GameServiceOption.wKindID=pGameServerInfo->wKindID;
	m_ModuleInitParameter.GameServiceOption.wNodeID=pGameServerInfo->wNodeID;
	m_ModuleInitParameter.GameServiceOption.wSortID=pGameServerInfo->wSortID;
	m_ModuleInitParameter.GameServiceOption.wServerID=pGameServerInfo->wServerID;

	//税收配置
	m_ModuleInitParameter.GameServiceOption.lCellScore=pGameServerInfo->lCellScore;
	m_ModuleInitParameter.GameServiceOption.cbRevenueRatio=pGameServerInfo->cbRevenueRatio;

	//房间配置
	m_ModuleInitParameter.GameServiceOption.lRestrictScore=pGameServerInfo->lRestrictScore;
	m_ModuleInitParameter.GameServiceOption.lMinTableScore=pGameServerInfo->lMinTableScore;
	m_ModuleInitParameter.GameServiceOption.lMinEnterScore=pGameServerInfo->lMinEnterScore;
	m_ModuleInitParameter.GameServiceOption.lMaxEnterScore=pGameServerInfo->lMaxEnterScore;

	//会员限制
	m_ModuleInitParameter.GameServiceOption.cbMinEnterMember=pGameServerInfo->cbMinEnterMember;
	m_ModuleInitParameter.GameServiceOption.cbMaxEnterMember=pGameServerInfo->cbMaxEnterMember;

	//房间配置
	m_ModuleInitParameter.GameServiceOption.dwServerRule=pGameServerInfo->dwServerRule;
	m_ModuleInitParameter.GameServiceOption.dwAttachUserRight=pGameServerInfo->dwAttachUserRight;

	//房间属性
	m_ModuleInitParameter.GameServiceOption.wMaxPlayer=pGameServerInfo->wMaxPlayer;
	m_ModuleInitParameter.GameServiceOption.wTableCount=pGameServerInfo->wTableCount;
	m_ModuleInitParameter.GameServiceOption.wServerPort=pGameServerInfo->wServerPort;
	m_ModuleInitParameter.GameServiceOption.wServerType=pGameServerInfo->wServerType;
	lstrcpyn(m_ModuleInitParameter.GameServiceOption.szServerName,pGameServerInfo->szServerName,LEN_SERVER);

	//分组属性
	m_ModuleInitParameter.GameServiceOption.cbDistributeRule=pGameServerInfo->cbDistributeRule;
	m_ModuleInitParameter.GameServiceOption.wMinDistributeUser=pGameServerInfo->wMinDistributeUser;
	m_ModuleInitParameter.GameServiceOption.wMaxDistributeUser=pGameServerInfo->wMaxDistributeUser;
	m_ModuleInitParameter.GameServiceOption.wDistributeTimeSpace=pGameServerInfo->wDistributeTimeSpace;
	m_ModuleInitParameter.GameServiceOption.wDistributeDrawCount=pGameServerInfo->wDistributeDrawCount;
	m_ModuleInitParameter.GameServiceOption.wDistributeStartDelay=pGameServerInfo->wDistributeStartDelay;

	//连接信息
	lstrcpyn(m_ModuleInitParameter.GameServiceOption.szDataBaseName,pGameServerInfo->szDataBaseName,CountArray(m_ModuleInitParameter.GameServiceOption.szDataBaseName));
	lstrcpyn(m_ModuleInitParameter.GameServiceOption.szDataBaseAddr,pGameServerInfo->szDataBaseAddr,CountArray(m_ModuleInitParameter.GameServiceOption.szDataBaseAddr));

	//数据设置
	UINT uCustomRuleSize=sizeof(m_ModuleInitParameter.GameServiceOption.cbCustomRule);
	CopyMemory(m_ModuleInitParameter.GameServiceOption.cbCustomRule,pGameServerInfo->cbCustomRule,uCustomRuleSize);

	return true;
}

//加载房间
VOID CDlgServerItem::OnBnClickedLoadServer()
{
	//获取选择
	ASSERT(m_ServerListControl.GetFirstSelectedItemPosition()!=NULL);
	POSITION Position=m_ServerListControl.GetFirstSelectedItemPosition();
	
	//获取房间
	INT nListItem=m_ServerListControl.GetNextSelectedItem(Position);
	tagGameServerInfo * pGameServerInfo=(tagGameServerInfo *)m_ServerListControl.GetItemData(nListItem);

	//设置变量
	if ((pGameServerInfo!=NULL)&&(GetModuleInitParameter(pGameServerInfo,false)==true))
	{
		EndDialog(IDC_LOAD_SERVER);
	}

	return;
}

//刷新列表
VOID CDlgServerItem::OnBnClickedReLoadList()
{
	//加载列表
	LoadDBServerItem();

	return;
}

//删除房间
VOID CDlgServerItem::OnBnClickedDeleteServer()
{
	//获取选择
	ASSERT(m_ServerListControl.GetFirstSelectedItemPosition()!=NULL);
	POSITION Position=m_ServerListControl.GetFirstSelectedItemPosition();
	
	//获取房间
	INT nListItem=m_ServerListControl.GetNextSelectedItem(Position);
	tagGameServerInfo * pGameServerInfo=(tagGameServerInfo *)m_ServerListControl.GetItemData(nListItem);

	//设置变量
	if (pGameServerInfo!=NULL)
	{
		//构造提示
		TCHAR szString[128]=TEXT("");
		_sntprintf(szString,CountArray(szString),TEXT("确实要删除 [ %s ] 游戏房间吗？"),pGameServerInfo->szServerName);

		//提示消息
		if (AfxMessageBox(szString,MB_ICONQUESTION|MB_YESNO)!=IDYES) return;

		//删除房间
		CServerInfoManager ServerInfoManager;
		if (ServerInfoManager.DeleteGameServer(pGameServerInfo->wServerID)==false) return;

		//删除列表
		m_ServerListControl.DeleteItem(nListItem);
		m_ServerInfoBuffer.DeleteServerInfo(pGameServerInfo->wServerID);

		//设置列表
		if (m_ServerListControl.GetItemCount()>0)
		{
			//设置变量
			INT nItemCount=m_ServerListControl.GetItemCount();
			INT nNextItem=(nListItem>=nItemCount)?(nItemCount-1):nListItem;

			//设置选择
			m_ServerListControl.SetItemState(nNextItem,LVIS_SELECTED,LVIS_SELECTED);
		}

		//更新控制
		UpdateControlStatus();
	}

	return;
}

//组件属性
VOID CDlgServerItem::OnBnClickedServiceAttrib()
{
	return;
}

//双击列表
VOID CDlgServerItem::OnNMDblclkServerList(NMHDR * pNMHDR, LRESULT * pResult)
{
	//加载配置
	if (((NMITEMACTIVATE *)pNMHDR)->iItem!=LB_ERR)
	{
		OnBnClickedLoadServer();
	}

	return;
}

//选择改变
VOID CDlgServerItem::OnLvnItemChangedServerList(NMHDR * pNMHDR, LRESULT * pResult)
{
	//更新控制
	UpdateControlStatus();

	return;
}

//////////////////////////////////////////////////////////////////////////////////
