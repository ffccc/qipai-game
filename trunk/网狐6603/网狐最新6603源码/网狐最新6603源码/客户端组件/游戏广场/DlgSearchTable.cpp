#include "StdAfx.h"
#include "Resource.h"
#include "ServerViewItem.h"
#include "DlgSearchTable.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgSearchTable, CSkinDialog)
	ON_BN_CLICKED(IDC_AUTO_JOIN, OnBnClickedAutoJoin)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CDlgSearchTable::CDlgSearchTable() : CSkinDialog(IDD_DLG_SEARCH_TABLE)
{
	//接口变量
	m_pTableViewFrame=NULL;
	m_pServerViewItem=NULL;

	//设置变量
	m_wResultTableID=INVALID_TABLE;

	return;
}

//析构函数
CDlgSearchTable::~CDlgSearchTable()
{
}

//控件绑定
VOID CDlgSearchTable::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btSearch);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_AUTO_JOIN, m_btAutoJoin);
}

//配置函数
BOOL CDlgSearchTable::OnInitDialog()
{
	__super::OnInitDialog();

	//设置变量
	m_wResultTableID=INVALID_TABLE;

	//设置标题
	SetWindowText(TEXT("查找游戏桌"));

	//设置条件
	CheckDlgButton(IDC_FILTRATE_PASS,BST_CHECKED);
	CheckDlgButton(IDC_SEARCH_ONE_NULL,BST_CHECKED);
	CheckDlgButton(IDC_SEARCH_TWO_NULL,BST_CHECKED);
	CheckDlgButton(IDC_SEARCH_NOT_FULL,BST_CHECKED);

	return FALSE;
}

//消息解释
BOOL CDlgSearchTable::PreTranslateMessage(MSG * pMsg)
{
	//按键过滤
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//确定函数
VOID CDlgSearchTable::OnOK()
{
	//变量定义
	tagSearchTable SearchTable;
	ZeroMemory(&SearchTable,sizeof(SearchTable));

	//设置变量
	SearchTable.bOneNull=(IsDlgButtonChecked(IDC_SEARCH_ONE_NULL)==BST_CHECKED);
	SearchTable.bTwoNull=(IsDlgButtonChecked(IDC_SEARCH_TWO_NULL)==BST_CHECKED);
	SearchTable.bNotFull=(IsDlgButtonChecked(IDC_SEARCH_NOT_FULL)==BST_CHECKED);
	SearchTable.bAllNull=(IsDlgButtonChecked(IDC_SEARCH_ALL_NULL)==BST_CHECKED);
	SearchTable.bFilterPass=(IsDlgButtonChecked(IDC_FILTRATE_PASS)==BST_CHECKED);

	//搜索变量
	SearchTable.wResultTableID=INVALID_TABLE;
	SearchTable.wResultChairID=INVALID_CHAIR;
	SearchTable.wStartTableID=m_wResultTableID+1;

	//查找桌子
	ASSERT(m_pServerViewItem!=NULL);
	m_pServerViewItem->SearchGameTable(SearchTable);

	//设置结果
	m_wResultTableID=SearchTable.wResultTableID;

	//结果判断
	if (SearchTable.wResultTableID==INVALID_TABLE)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("抱歉，暂时找不到符合您要求的游戏桌，请调整查找条件或稍后再试！"),MB_ICONINFORMATION,30);

		return;
	}

	//设置界面
	m_btAutoJoin.EnableWindow(m_wResultTableID!=INVALID_TABLE);
	m_pTableViewFrame->VisibleTable(SearchTable.wResultTableID);

	return; 
}

//取消函数
VOID CDlgSearchTable::OnCancel() 
{
	//关闭窗口
	DestroyWindow();

	return; 
}

//设置房间
VOID CDlgSearchTable::SetServerViewItem(CServerViewItem * pServerViewItem)
{
	//设置变量
	m_pServerViewItem=pServerViewItem;

	return;
}

//设置桌子
VOID CDlgSearchTable::SetTableViewFrame(CTableViewFrame * pTableViewFrame)
{
	//设置镜像
	ASSERT(pTableViewFrame!=NULL);
	m_pTableViewFrame=pTableViewFrame;

	return;
}

//快速加入
VOID CDlgSearchTable::OnBnClickedAutoJoin()
{
	//效验状态
	ASSERT(m_wResultTableID!=INVALID_TABLE);
	if (m_wResultTableID==INVALID_TABLE) return;

	//加入桌子
	WORD wChairID=INVALID_CHAIR;
	m_pTableViewFrame->GetNullChairCount(m_wResultTableID,wChairID);

	//加入判断
	if (wChairID==INVALID_CHAIR)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("此游戏桌已经满人了，无法加入此游戏桌，请重新搜索后再次尝试！"),MB_ICONINFORMATION,30);

		return;
	}

	//加入游戏
	ASSERT(m_pServerViewItem!=NULL);
	m_pServerViewItem->OnLButtonHitTable(m_wResultTableID,wChairID);

	return;
}

//////////////////////////////////////////////////////////////////////////
