#include "StdAfx.h"
#include "ModuleListControl.h"

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CModuleListControl::CModuleListControl()
{
}

//析构函数
CModuleListControl::~CModuleListControl()
{
}

//配置列表
VOID CModuleListControl::InitListControl()
{
	//变量定义
	INT nColIndex=0;

	//配置列表
	InsertColumn(nColIndex++,TEXT("游戏名字"),LVCFMT_CENTER,80);
	InsertColumn(nColIndex++,TEXT("组件状态"),LVCFMT_CENTER,80);
	InsertColumn(nColIndex++,TEXT("数据库名"),LVCFMT_LEFT,100);
	InsertColumn(nColIndex++,TEXT("连接地址"),LVCFMT_LEFT,100);
	InsertColumn(nColIndex++,TEXT("服务器名"),LVCFMT_LEFT,140);
	InsertColumn(nColIndex++,TEXT("客户端名"),LVCFMT_LEFT,120);

	return;
}

//子项排序
INT CModuleListControl::SortListItemData(LPARAM lParam1, LPARAM lParam2, WORD wColumnIndex)
{
	//变量定义
	tagGameModuleInfo * pGameModuleInfo1=(tagGameModuleInfo *)lParam1;
	tagGameModuleInfo * pGameModuleInfo2=(tagGameModuleInfo *)lParam2;

	//安装排序
	bool bInstall1=(pGameModuleInfo1->dwNativeVersion!=0L);
	bool bInstall2=(pGameModuleInfo2->dwNativeVersion!=0L);
	if (bInstall1!=bInstall2) return (bInstall1==true)?1:-1;

	//子项排序
	switch (wColumnIndex)
	{
	case 0:		//游戏名字
		{
			return lstrcmp(pGameModuleInfo1->szGameName,pGameModuleInfo2->szGameName);
		}
	case 1:		//组件状态
		{
			//组件状态
			BYTE cbStatus1=0;
			if (pGameModuleInfo1->dwNativeVersion!=0)
			{
				cbStatus1=(pGameModuleInfo1->dwNativeVersion==pGameModuleInfo1->dwServerVersion)?2:1;
			}

			//组件状态
			BYTE cbStatus2=0;
			if (pGameModuleInfo2->dwNativeVersion!=0)
			{
				cbStatus2=(pGameModuleInfo2->dwNativeVersion==pGameModuleInfo2->dwServerVersion)?2:1;
			}

			return (cbStatus1>cbStatus2)?SORT_POSITION_AFTER:SORT_POSITION_FRONT;
		}
	case 2:		//数据库名
		{
			return lstrcmp(pGameModuleInfo1->szDataBaseName,pGameModuleInfo2->szDataBaseName);
		}
	case 3:		//连接地址
		{
			return lstrcmp(pGameModuleInfo1->szDataBaseAddr,pGameModuleInfo2->szDataBaseAddr);
		}
	case 4:		//服务器名
		{
			return lstrcmp(pGameModuleInfo1->szServerDLLName,pGameModuleInfo2->szServerDLLName);
		}
	case 5:		//客户端名
		{
			return lstrcmp(pGameModuleInfo1->szClientEXEName,pGameModuleInfo2->szClientEXEName);
		}
	}

	return 0;
}

//获取颜色
VOID CModuleListControl::GetListItemColor(LPARAM lItemParam, UINT uItemStatus, tagListItemColor & ListItemColor)
{
	//变量定义
	ASSERT(lItemParam!=NULL);
	tagGameModuleInfo * pGameModuleInfo=(tagGameModuleInfo *)lItemParam;

	//没有安装
	if (pGameModuleInfo->dwNativeVersion==0L)
	{
		//设置颜色
		ListItemColor.rcTextColor=RGB(125,125,125);
		ListItemColor.rcBackColor=(uItemStatus&ODS_SELECTED)?RGB(50,50,50):CR_NORMAL_BK;

		return;
	}

	//存在更新
	if (pGameModuleInfo->dwNativeVersion!=pGameModuleInfo->dwServerVersion)
	{
		//设置颜色
		ListItemColor.rcTextColor=RGB(128,0,0);
		ListItemColor.rcBackColor=(uItemStatus&ODS_SELECTED)?RGB(230,230,0):CR_NORMAL_BK;

		return;
	}

	//版本一致
	if (pGameModuleInfo->dwNativeVersion==pGameModuleInfo->dwServerVersion)
	{
		//设置颜色
		ListItemColor.rcBackColor=(uItemStatus&ODS_SELECTED)?RGB(0,0,128):CR_NORMAL_BK;
		ListItemColor.rcTextColor=(uItemStatus&ODS_SELECTED)?RGB(255,255,255):RGB(0,0,0);

		return;
	}

	return;
}

//插入列表
bool CModuleListControl::InsertModuleInfo(tagGameModuleInfo * pGameModuleInfo)
{
	//变量定义
	LVFINDINFO FindInfo;
	ZeroMemory(&FindInfo,sizeof(FindInfo));

	//设置变量
	FindInfo.flags=LVFI_PARAM;
	FindInfo.lParam=(LPARAM)pGameModuleInfo;

	//存在判断
	INT nInsertItem=FindItem(&FindInfo);
	if (nInsertItem!=LB_ERR) return true;

	//插入列表
	for (WORD i=0;i<m_ListHeaderCtrl.GetItemCount();i++)
	{
		if (i==0)
		{
			//插入首项
			INT nIndex=GetInsertIndex(pGameModuleInfo);
			LPCTSTR pszDescribe=GetDescribeString(pGameModuleInfo,i);
			nInsertItem=InsertItem(LVIF_TEXT|LVIF_PARAM,nIndex,pszDescribe,0,0,0,(LPARAM)pGameModuleInfo);
		}
		else
		{
			//字符子项
			SetItem(nInsertItem,i,LVIF_TEXT,GetDescribeString(pGameModuleInfo,i),0,0,0,0);
		}
	}

	return true;
}

//插入索引
WORD CModuleListControl::GetInsertIndex(tagGameModuleInfo * pGameModuleInfo)
{
	//变量定义
	INT nItemCount=GetItemCount();
	tagGameModuleInfo * pGameModuleTemp=NULL;

	//获取位置
	for (INT i=0;i<nItemCount;i++)
	{
		//获取数据
		pGameModuleTemp=(tagGameModuleInfo *)GetItemData(i);

		//安装判断
		if ((pGameModuleInfo->dwNativeVersion==0)&&(pGameModuleTemp->dwNativeVersion!=0))
		{
			continue;
		}
		
		//名字判断
		if (lstrcmp(pGameModuleInfo->szGameName,pGameModuleTemp->szGameName)<0)
		{
			return i;
		}
	}

	return nItemCount;
}

//描述字符
LPCTSTR CModuleListControl::GetDescribeString(tagGameModuleInfo * pGameModuleInfo, WORD wColumnIndex)
{
	//构造字符
	switch (wColumnIndex)
	{
	case 0:		//游戏名字
		{
			return pGameModuleInfo->szGameName;
		}
	case 1:		//组件状态
		{
			if (pGameModuleInfo->dwNativeVersion==0L) return TEXT("没有安装");
			if (pGameModuleInfo->dwServerVersion!=pGameModuleInfo->dwNativeVersion) return TEXT("存在更新");
			return TEXT("已经安装");
		}
	case 2:		//数据库名
		{
			return pGameModuleInfo->szDataBaseName;
		}
	case 3:		//连接地址
		{
			return pGameModuleInfo->szDataBaseAddr;
		}
	case 4:		//服务器名
		{
			return pGameModuleInfo->szServerDLLName;
		}
	case 5:		//客户端名
		{
			return pGameModuleInfo->szClientEXEName;
		}
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////////////
