#include "StdAfx.h"
#include "Resource.h"
#include "ApplyUserList.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CApplyUser, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CApplyUser::CApplyUser() : CDialog(IDD_DLG_APPLY_BANKER)
{
}

//析构函数
CApplyUser::~CApplyUser()
{
}

//控件绑定
void CApplyUser::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RECORD_LIST, m_AppyUserList);
}

//初始化函数
BOOL CApplyUser::OnInitDialog()
{
	__super::OnInitDialog();
   // m_AppyUserList.SetWindowPos(HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	//设置颜色
	m_AppyUserList.SetTextColor(RGB(255,255,0));
	m_AppyUserList.SetBkColor(RGB(9,45,109));
	m_AppyUserList.SetTextBkColor(RGB(9,45,109));
	//m_AppyUserList.SetWindowPos(&wndTopMost,0,0,150,150,SWP_NOZORDER);

	//m_AppyUserList.SetLayeredWindowAttributes();
	//插入列表
	m_AppyUserList.InsertColumn(0,TEXT("申请玩家"),LVCFMT_LEFT,85);
	m_AppyUserList.InsertColumn(1,TEXT("游戏币"),LVCFMT_LEFT,140);

	return FALSE;
}

//位置变化
void CApplyUser::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	//列表控件
	if (m_AppyUserList.m_hWnd != NULL)
	{
		m_AppyUserList.MoveWindow(0,0,cx,cy);
		m_AppyUserList.SetColumnWidth(1,cx-m_AppyUserList.GetColumnWidth(0));
	}

	return;
}

//删除列表
void CApplyUser::DeleteUser(tagApplyUser & ApplyUser)
{
	//查找子项
	int nItem = FindUser(ApplyUser.dwUserID);

	//删除子项
	if ( nItem != -1 )
	{
		m_AppyUserList.DeleteItem( nItem );
	}
}

//插入列表
void CApplyUser::InserUser(tagApplyUser & ApplyUser)
{
	//查找子项
	int nItem = FindUser(ApplyUser.dwUserID);

	//删除子项
	if ( nItem != -1 ) 
	{
		TCHAR szBuffer[128]=TEXT("");

		//玩家喜糖
		_snprintf(szBuffer,CountArray(szBuffer),TEXT("%I64d"),ApplyUser.lUserScore);
		m_AppyUserList.SetItem(nItem,1,LVIF_TEXT,szBuffer,0,0,0,0);
	}
	else
	{
		//变量定义
		WORD wListIndex=0;
		WORD wColumnCount=1;
		TCHAR szBuffer[128]=TEXT("");

		//游戏玩家
		INT nItemIndex=m_AppyUserList.InsertItem(m_AppyUserList.GetItemCount(), (LPCTSTR)ApplyUser.stAname);
		//玩家金币

		m_AppyUserList.SetItem(nItemIndex,wColumnCount++,LVIF_TEXT,DrawNUM(ApplyUser.lUserScore),0,0,0,0);
		m_AppyUserList.SetItemData(nItemIndex, ApplyUser.dwUserID);
	}


	return;
}
char* CApplyUser::DrawNUM(INT64 num)
{
	TCHAR szBuffer[256];
	_sntprintf(szBuffer, CountArray(szBuffer), TEXT("%I64d"),num);
	int vf=1;
	if (num<0)
	{
		vf=2;
	}
	int cu = 0;
	s1="";
	s1=szBuffer;  
	for (int i=strlen(szBuffer)-1;i>=vf;i--)
	{
		cu++;
		//if (cu%4 == 0)				
		//	s1.insert(i,",");		
	}
	return (char*)s1.c_str();

}

//更新列表
void CApplyUser::UpdateUser( tagApplyUser & ApplyUser )
{

	//查找子项
	int nItem = FindUser(ApplyUser.dwUserID);

	//删除子项
	if ( nItem != -1 ) 
	{
		//玩家金币
		m_AppyUserList.SetItem(nItem,1,LVIF_TEXT,DrawNUM(ApplyUser.lUserScore),0,0,0,0);
	}


}

//清空列表
void CApplyUser::ClearAll()
{
	m_AppyUserList.DeleteAllItems();
}

//删除列表
int CApplyUser::FindUser(DWORD dwUserID)
{
	TCHAR szFindTxt[100];
	sprintf(szFindTxt, "%d", dwUserID);

	//构造变量
	LVFINDINFO lvFindInfo;
	ZeroMemory( &lvFindInfo, sizeof( lvFindInfo ) );
	lvFindInfo.flags = LVFI_PARAM;
	lvFindInfo.lParam = dwUserID;

	//查找子项
	return m_AppyUserList.FindItem( &lvFindInfo );
}

//////////////////////////////////////////////////////////////////////////
