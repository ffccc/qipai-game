#include "StdAfx.h"
#include "ServerListControl.h"

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CServerListControl::CServerListControl()
{
}

//析构函数
CServerListControl::~CServerListControl()
{
}

//配置列表
VOID CServerListControl::InitListControl()
{
	//变量定义
	INT nColIndex=0;

	//配置列表
	InsertColumn(nColIndex++,TEXT("房间名字"),LVCFMT_LEFT,90);
	InsertColumn(nColIndex++,TEXT("模块名字"),LVCFMT_CENTER,80);
	InsertColumn(nColIndex++,TEXT("房间类型"),LVCFMT_CENTER,60);
	InsertColumn(nColIndex++,TEXT("桌子数目"),LVCFMT_CENTER,60);
	InsertColumn(nColIndex++,TEXT("监听端口"),LVCFMT_CENTER,60);
	InsertColumn(nColIndex++,TEXT("房间标识"),LVCFMT_CENTER,60);
	InsertColumn(nColIndex++,TEXT("类型标识"),LVCFMT_CENTER,60);
	InsertColumn(nColIndex++,TEXT("节点标识"),LVCFMT_CENTER,60);
	InsertColumn(nColIndex++,TEXT("排序标识"),LVCFMT_CENTER,60);
	InsertColumn(nColIndex++,TEXT("数据库名"),LVCFMT_LEFT,100);
	InsertColumn(nColIndex++,TEXT("数据库地址"),LVCFMT_LEFT,100);
	InsertColumn(nColIndex++,TEXT("服务器名"),LVCFMT_LEFT,140);
	InsertColumn(nColIndex++,TEXT("客户端名"),LVCFMT_LEFT,120);

	return;
}

//子项排序
INT CServerListControl::SortListItemData(LPARAM lParam1, LPARAM lParam2, WORD wColumnIndex)
{
	//变量定义
	tagGameServerInfo * pGameServerInfo1=(tagGameServerInfo *)lParam1;
	tagGameServerInfo * pGameServerInfo2=(tagGameServerInfo *)lParam2;

	//安装排序
	bool bInstall1=(pGameServerInfo1->dwNativeVersion!=0L);
	bool bInstall2=(pGameServerInfo2->dwNativeVersion!=0L);
	if (bInstall1!=bInstall2) return (bInstall1==true)?1:-1;

	//子项排序
	switch (wColumnIndex)
	{
	case 0:		//房间名字
		{
			return lstrcmp(pGameServerInfo1->szServerName,pGameServerInfo2->szServerName);
		}
	case 1:		//游戏名字
		{
			return lstrcmp(pGameServerInfo1->szGameName,pGameServerInfo2->szGameName);
		}
	case 2:		//房间类型
		{
			return (pGameServerInfo1->wServerType>pGameServerInfo2->wServerType)?SORT_POSITION_AFTER:SORT_POSITION_FRONT;
		}
	case 3:		//桌子数目
		{
			return (pGameServerInfo1->wTableCount>pGameServerInfo2->wTableCount)?SORT_POSITION_AFTER:SORT_POSITION_FRONT;
		}
	case 4:		//监听端口
		{
			return (pGameServerInfo1->wServerPort>pGameServerInfo2->wServerPort)?SORT_POSITION_AFTER:SORT_POSITION_FRONT;
		}
	case 5:		//房间标识
		{
			return (pGameServerInfo1->wServerID>pGameServerInfo2->wServerID)?SORT_POSITION_AFTER:SORT_POSITION_FRONT;
		}
	case 6:		//类型标识
		{
			//房间标识
			WORD wKindID1=pGameServerInfo1->wKindID;
			if (wKindID1==0) wKindID1=pGameServerInfo1->wGameID;

			//房间标识
			WORD wKindID2=pGameServerInfo2->wKindID;
			if (wKindID2==0) wKindID2=pGameServerInfo2->wGameID;

			return (wKindID1>wKindID2)?SORT_POSITION_AFTER:SORT_POSITION_FRONT;
		}
	case 7:		//节点标识
		{
			return (pGameServerInfo1->wNodeID>pGameServerInfo2->wNodeID)?SORT_POSITION_AFTER:SORT_POSITION_FRONT;
		}
	case 8:		//排序标识
		{
			return (pGameServerInfo1->wSortID>pGameServerInfo2->wSortID)?SORT_POSITION_AFTER:SORT_POSITION_FRONT;
		}
	case 9:		//数据库名
		{
			return lstrcmp(pGameServerInfo1->szDataBaseName,pGameServerInfo2->szDataBaseName);
		}
	case 10:	//数据库地址
		{
			return lstrcmp(pGameServerInfo1->szDataBaseAddr,pGameServerInfo2->szDataBaseAddr);
		}
	case 11:	//服务器名
		{
			return lstrcmp(pGameServerInfo1->szServerDLLName,pGameServerInfo2->szServerDLLName);
		}
	case 12:	//客户端名
		{
			return lstrcmp(pGameServerInfo1->szClientEXEName,pGameServerInfo2->szClientEXEName);
		}
	}

	return 0;
}

//获取颜色
VOID CServerListControl::GetListItemColor(LPARAM lItemParam, UINT uItemStatus, tagListItemColor & ListItemColor)
{
	//变量定义
	ASSERT(lItemParam!=NULL);
	tagGameServerInfo * pGameServerInfo=(tagGameServerInfo *)lItemParam;

	//没有安装
	if (pGameServerInfo->dwNativeVersion==0L)
	{
		//设置颜色
		ListItemColor.rcTextColor=RGB(125,125,125);
		ListItemColor.rcBackColor=(uItemStatus&ODS_SELECTED)?RGB(50,50,50):CR_NORMAL_BK;

		return;
	}

	//存在更新
	if (pGameServerInfo->dwNativeVersion!=pGameServerInfo->dwServerVersion)
	{
		//设置颜色
		ListItemColor.rcTextColor=RGB(128,0,0);
		ListItemColor.rcBackColor=(uItemStatus&ODS_SELECTED)?RGB(230,230,0):CR_NORMAL_BK;

		return;
	}

	//版本一致
	if (pGameServerInfo->dwNativeVersion==pGameServerInfo->dwServerVersion)
	{
		//设置颜色
		ListItemColor.rcBackColor=(uItemStatus&ODS_SELECTED)?RGB(0,0,128):CR_NORMAL_BK;
		ListItemColor.rcTextColor=(uItemStatus&ODS_SELECTED)?RGB(255,255,255):RGB(0,0,0);

		return;
	}

	return;
}

//插入列表
bool CServerListControl::InsertServerInfo(tagGameServerInfo * pGameServerInfo)
{
	//变量定义
	LVFINDINFO FindInfo;
	ZeroMemory(&FindInfo,sizeof(FindInfo));

	//设置变量
	FindInfo.flags=LVFI_PARAM;
	FindInfo.lParam=(LPARAM)pGameServerInfo;

	//存在判断
	INT nInsertItem=FindItem(&FindInfo);
	if (nInsertItem!=LB_ERR) return true;

	//插入列表
	for (WORD i=0;i<m_ListHeaderCtrl.GetItemCount();i++)
	{
		if (i==0)
		{
			//插入首项
			INT nIndex=GetInsertIndex(pGameServerInfo);
			LPCTSTR pszDescribe=GetDescribeString(pGameServerInfo,i);
			nInsertItem=InsertItem(LVIF_TEXT|LVIF_PARAM,nIndex,pszDescribe,0,0,0,(LPARAM)pGameServerInfo);
		}
		else
		{
			//字符子项
			SetItem(nInsertItem,i,LVIF_TEXT,GetDescribeString(pGameServerInfo,i),0,0,0,0);
		}
	}

	return true;
}

//插入索引
WORD CServerListControl::GetInsertIndex(tagGameServerInfo * pGameServerInfo)
{
	//变量定义
	INT nItemCount=GetItemCount();
	tagGameServerInfo * pGameServerTemp=NULL;

	//获取位置
	for (INT i=0;i<nItemCount;i++)
	{
		//获取数据
		pGameServerTemp=(tagGameServerInfo *)GetItemData(i);

		//安装判断
		if ((pGameServerInfo->dwNativeVersion==0)&&(pGameServerTemp->dwNativeVersion!=0))
		{
			continue;
		}
		
		//游戏名字
		if (lstrcmp(pGameServerInfo->szGameName,pGameServerTemp->szGameName)<0)
		{
			return i;
		}

		//房间名字
		if (lstrcmp(pGameServerInfo->szServerName,pGameServerTemp->szServerName)<0)
		{
			return i;
		}
	}

	return nItemCount;
}

//描述字符
LPCTSTR CServerListControl::GetDescribeString(tagGameServerInfo * pGameServerInfo, WORD wColumnIndex)
{
	//变量定义
	static TCHAR szDescribe[128]=TEXT("");

	//构造字符
	switch (wColumnIndex)
	{
	case 0:		//房间名字
		{
			return pGameServerInfo->szServerName;
		}
	case 1:		//游戏名字
		{
			return pGameServerInfo->szGameName;
		}
	case 2:		//房间类型
		{
			//财富类型
			if (pGameServerInfo->wServerType&GAME_GENRE_GOLD)
			{
				lstrcpyn(szDescribe,TEXT("财富类型"),CountArray(szDescribe));
				return szDescribe;
			}

			//积分类型
			if (pGameServerInfo->wServerType&GAME_GENRE_SCORE)
			{
				lstrcpyn(szDescribe,TEXT("积分类型"),CountArray(szDescribe));
				return szDescribe;
			}

			//比赛类型
			if (pGameServerInfo->wServerType&GAME_GENRE_MATCH)
			{
				lstrcpyn(szDescribe,TEXT("比赛类型"),CountArray(szDescribe));
				return szDescribe;
			}

			//训练类型
			if (pGameServerInfo->wServerType&GAME_GENRE_EDUCATE)
			{
				lstrcpyn(szDescribe,TEXT("训练类型"),CountArray(szDescribe));
				return szDescribe;
			}

			return TEXT("未知类型");
		}
	case 3:		//桌子数目
		{
			return _itot(pGameServerInfo->wTableCount,szDescribe,10);
		}
	case 4:		//监听端口
		{
			if (pGameServerInfo->wServerPort==0) return TEXT("-");
			return _itot(pGameServerInfo->wServerPort,szDescribe,10);
		}
	case 5:		//房间标识
		{
			return _itot(pGameServerInfo->wServerID,szDescribe,10);
		}
	case 6:		//类型标识
		{
			WORD wKindID=pGameServerInfo->wKindID;
			WORD wGameID=pGameServerInfo->wGameID;
			return _itot((wKindID==0)?wGameID:wKindID,szDescribe,10);
		}
	case 7:		//节点标识
		{
			return _itot(pGameServerInfo->wNodeID,szDescribe,10);
		}
	case 8:		//排序标识
		{
			return _itot(pGameServerInfo->wSortID,szDescribe,10);
		}
	case 9:		//数据库名
		{
			return pGameServerInfo->szDataBaseName;
		}
	case 10:	//数据库地址
		{
			return pGameServerInfo->szDataBaseAddr;
		}
	case 11:	//服务器名
		{
			return pGameServerInfo->szServerDLLName;
		}
	case 12:	//客户端名
		{
			return pGameServerInfo->szClientEXEName;
		}
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////////////
