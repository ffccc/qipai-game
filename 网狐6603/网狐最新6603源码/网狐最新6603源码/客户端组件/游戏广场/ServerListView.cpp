#include "StdAfx.h"
#include "GlobalUnits.h"
#include "PlatformFrame.h"
#include "PlatformEvent.h"
#include "ServerListView.h"

//////////////////////////////////////////////////////////////////////////////////

//常量定义
#define ITEM_SIZE					18									//子项高度
#define ITEM_HEIGHT					26									//子项高度

//////////////////////////////////////////////////////////////////////////////////

//时间定义
#define IDI_UPDATE_ONLINE			100									//更新人数
#define TIME_UPDATE_ONLINE			600000								//更新人数

//////////////////////////////////////////////////////////////////////////////////

//图标索引
#define IND_ROOT					0									//列表顶项
#define IND_TYPE					1									//游戏类型
#define IND_NODE					2									//游戏节点
#define IND_KIND_NODOWN				3									//还没下载
#define IND_KIND_UNKNOW				4									//未知类型
#define IND_SERVER_UPDATE			5									//收藏房间
#define IND_SERVER_ATTACH			6									//收藏房间
#define IND_SERVER_NORMAL			7									//正常房间
#define IND_SERVER_ENTRANCE			8									//当前使用
#define IND_SERVER_EVERENTRANCE		9									//曾经房间
#define IND_BROWSE					10									//浏览子项
#define IND_FUNCTION				11									//功能子项

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CServerListView, CTreeCtrl)

	//系统消息
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()
	ON_NOTIFY_REFLECT(NM_CLICK, OnNMLClick)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnNMRClick)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnTvnSelchanged)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnTvnItemexpanding)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CServerListView::CServerListView()
{
	//设置变量
	m_nXScroll=0;
	m_nYScroll=0;

	//列表句柄
	m_TreeListRoot=NULL;
	m_TreeAssistant=NULL;

	//辅助变量
	m_hItemMouseHover=NULL;
	m_hTreeClickExpand=NULL;

	//内核变量
	m_GameImageMap.InitHashTable(PRIME_KIND);

	//加载资源
	m_ImageArrow.LoadImage(AfxGetInstanceHandle(),TEXT("SERVER_LIST_ARROW"));

	//创建字体
	m_FontBold.CreateFont(-12,0,0,0,600,0,0,0,134,3,2,ANTIALIASED_QUALITY,2,TEXT("宋体"));

	return;
}

//析构函数
CServerListView::~CServerListView()
{
}

//命令函数
BOOL CServerListView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	/*UINT nCommandID=LOWORD(wParam);

	//菜单命令
	switch (nCommandID)
	{
	case IDM_ENTER_SERVER:		//进入房间
		{
			//获取树项
			HTREEITEM hTreeItem=GetSelectedItem();

			//选择判断
			if (hTreeItem==NULL) return TRUE;

			//获取数据
			CGameListItem * pGameListItem=(CGameListItem *)GetItemData(hTreeItem);

			//命令处理
			if (pGameListItem!=NULL)
			{
				switch (pGameListItem->GetItemGenre())
				{
				case ItemGenre_Kind:	//游戏种类
					{
						//进入房间
						CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
						pPlatformFrame->EntranceServerItem((CGameKindItem *)pGameListItem);

						break;
					}
				case ItemGenre_Server:	//游戏房间
					{
						//进入房间
						CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
						pPlatformFrame->EntranceServerItem((CGameServerItem *)pGameListItem);

						break;
					}
				}
			}

			return TRUE;
		}
	case IDM_SHRINK_LIST:		//收缩列表
		{
			//收缩列表
			HTREEITEM hCurrentItem=GetSelectedItem();
			if (hCurrentItem!=NULL)	Expand(hCurrentItem,TVE_COLLAPSE);

			return TRUE;
		}
	case IDM_EXPAND_LIST:		//展开列表
		{
			//展开列表
			HTREEITEM hCurrentItem=GetSelectedItem();
			if (hCurrentItem!=NULL)	Expand(hCurrentItem,TVE_EXPAND);

			return TRUE;
		}
	}*/

	return __super::OnCommand(wParam,lParam);
}

//窗口函数
LRESULT CServerListView::DefWindowProc(UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	//双击消息
	switch (uMessage)
	{
	case WM_LBUTTONDOWN:		//鼠标消息
		{
			//鼠标坐标
			CPoint MousePoint;
			MousePoint.SetPoint(LOWORD(lParam),HIWORD(lParam));

			//设置变量
			m_hTreeClickExpand=NULL;

			//点击测试
			HTREEITEM hTreeItem=HitTest(MousePoint);

			//消息处理
			if ((hTreeItem!=NULL)&&(GetChildItem(hTreeItem)!=NULL))
			{
				//选择树项
				SetFocus();
				Select(hTreeItem,TVGN_CARET);

				//获取位置
				CRect rcTreeItem;
				GetItemRect(hTreeItem,&rcTreeItem,TRUE);

				//消息处理
				if (rcTreeItem.PtInRect(MousePoint)==TRUE)
				{
					//展开列表
					if (ExpandVerdict(hTreeItem)==false)
					{
						m_hTreeClickExpand=hTreeItem;
						Expand(m_hTreeClickExpand,TVE_EXPAND);
					}

					return 0;
				}
			}

			break;
		}
	case WM_LBUTTONDBLCLK:		//鼠标消息
		{
			//鼠标坐标
			CPoint MousePoint;
			MousePoint.SetPoint(LOWORD(lParam),HIWORD(lParam));

			//点击测试
			HTREEITEM hTreeItem=HitTest(MousePoint);

			//下载判断
			if (hTreeItem!=NULL)
			{
				//获取数据
				CGameListItem * pGameListItem=(CGameListItem *)GetItemData(hTreeItem);
				if ((pGameListItem!=NULL)&&(pGameListItem->GetItemGenre()==ItemGenre_Kind))
				{
					//变量定义
					CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;

					//下载判断
					if (pGameKindItem->m_dwProcessVersion==0L)
					{
						//获取版本
						tagGameKind * pGameKind=&pGameKindItem->m_GameKind;
						CWHService::GetModuleVersion(pGameKind->szProcessName,pGameKindItem->m_dwProcessVersion);

						//下载判断
						if (pGameKindItem->m_dwProcessVersion==0L)
						{
							CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
							pGlobalUnits->DownLoadClient(pGameKind->szKindName,pGameKind->wKindID,0);
						}
						else
						{
							OnGameItemUpdate(pGameKindItem);
						}
					}
				}
			}

			//消息处理
			if ((hTreeItem!=NULL)&&(GetChildItem(hTreeItem)!=NULL))
			{
				//选择树项
				SetFocus();
				Select(hTreeItem,TVGN_CARET);

				//位置判断
				CRect rcTreeItem;
				GetItemRect(hTreeItem,&rcTreeItem,TRUE);
				if (rcTreeItem.PtInRect(MousePoint)==FALSE) break;

				//展开判断
				if ((m_hTreeClickExpand!=hTreeItem)&&(ExpandVerdict(hTreeItem)==true))
				{
					//设置变量
					m_hTreeClickExpand=NULL;

					//展开列表
					Expand(hTreeItem,TVE_COLLAPSE);
				}

				return 0;
			}

			break;
		}
	}

	return __super::DefWindowProc(uMessage,wParam,lParam);
}

//快速通道
VOID CServerListView::InitAssistantItem()
{
	//工作目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//构造路径
	TCHAR szAssistantPath[MAX_PATH]=TEXT("");
	_sntprintf(szAssistantPath,CountArray(szAssistantPath),TEXT("%s\\Collocate\\Collocate.INI"),szDirectory);

	//读取名字
	TCHAR szAssistant[512]=TEXT("");
	GetPrivateProfileString(TEXT("Assistant"),TEXT("AssistantName"),TEXT(""),szAssistant,CountArray(szAssistant),szAssistantPath);

	//创建子项
	if (szAssistant[0]!=0)
	{
		//变量定义
		DWORD dwInsideID=1;

		//创建树项
		UINT nMask=TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_PARAM|TVIF_STATE;
		m_TreeAssistant=InsertItem(nMask,szAssistant,IND_FUNCTION,IND_FUNCTION,TVIS_BOLD,TVIS_BOLD,NULL,NULL,TVI_LAST);

		//创建子项
		do
		{
			//构造键名
			TCHAR szItemName[16]=TEXT("");
			_sntprintf(szItemName,CountArray(szItemName),TEXT("Assistant%ld"),dwInsideID);

			//读取连接
			TCHAR szAssistantName[128]=TEXT("");
			GetPrivateProfileString(szItemName,TEXT("AssistantName"),TEXT(""),szAssistantName,CountArray(szAssistantName),szAssistantPath);

			//完成判断
			if (szAssistantName[0]==0) break;

			//插入子项
			InsertInsideItem(szAssistantName,IND_BROWSE,dwInsideID++,m_TreeAssistant);

		} while (true);

		//展开列表
		ExpandListItem(m_TreeAssistant);
	}

	return;
}

//配置函数
VOID CServerListView::InitServerTreeView()
{
	//设置属性
	SetItemHeight(ITEM_HEIGHT);
	ModifyStyle(0,TVS_HASBUTTONS|TVS_HASLINES|TVS_SHOWSELALWAYS|TVS_TRACKSELECT|TVS_FULLROWSELECT|TVS_HASLINES);

	//加载标志
	CBitmap ServerImage;
	ServerImage.LoadBitmap(IDB_SERVER_LIST_IMAGE);
	m_ImageList.Create(ITEM_SIZE,ITEM_SIZE,ILC_COLOR16|ILC_MASK,0,0);

	//设置资源
	SetImageList(&m_ImageList,LVSIL_NORMAL);
	m_ImageList.Add(&ServerImage,RGB(255,0,255));

	//设置树项
	m_TreeListRoot=InsertInsideItem(szProduct,IND_ROOT,0,TVI_ROOT);

	//快速通道
	InitAssistantItem();

	//设置滚动
	m_SkinScrollBar.InitScroolBar(this);

	return;
}

//获取选择
HTREEITEM CServerListView::GetCurrentSelectItem(bool bOnlyText)
{
	//获取光标
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//点击判断
	HTREEITEM hTreeItem=HitTest(MousePoint);

	if (bOnlyText==true)
	{
		//获取位置
		CRect rcTreeItem;
		GetItemRect(hTreeItem,&rcTreeItem,TRUE);

		//选择判断
		if (rcTreeItem.PtInRect(MousePoint)==FALSE) return NULL;
	}

	return hTreeItem;
}

//展开状态
bool CServerListView::ExpandVerdict(HTREEITEM hTreeItem)
{
	if (hTreeItem!=NULL)
	{
		UINT uState=GetItemState(hTreeItem,TVIS_EXPANDED);
		return ((uState&TVIS_EXPANDED)!=0);
	}

	return false;
}

//展开列表
bool CServerListView::ExpandListItem(HTREEITEM hTreeItem)
{
	//效验参数
	ASSERT(hTreeItem!=NULL);
	if (hTreeItem==NULL) return false;

	//展开列表
	HTREEITEM hCurrentItem=hTreeItem;
	do
	{
		Expand(hCurrentItem,TVE_EXPAND);
		hCurrentItem=GetParentItem(hCurrentItem);
	} while (hCurrentItem!=NULL);

	return true;
}

//展开列表
bool CServerListView::ExpandListItem(CGameListItem * pGameListItem)
{
	//效验参数
	ASSERT(pGameListItem!=NULL);
	if (pGameListItem==NULL) return false;

	//展开树项
	if (pGameListItem->m_hTreeItemAttach!=NULL) ExpandListItem(pGameListItem->m_hTreeItemAttach);
	if (pGameListItem->m_hTreeItemNormal!=NULL) ExpandListItem(pGameListItem->m_hTreeItemNormal);
	
	return true;
}

//绘画子项
VOID CServerListView::DrawTreeItem(CDC * pDC, CRect & rcClient, CRect & rcClipBox)
{
	//首项判断
	HTREEITEM hItemCurrent=GetFirstVisibleItem();
	if (hItemCurrent==NULL) return;

	//获取属性
	UINT uTreeStyte=GetWindowLong(m_hWnd,GWL_STYLE);

	//获取对象
	ASSERT(CSkinRenderManager::GetInstance()!=NULL);
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();

	//绘画子项
	do
	{
		//变量定义
		CRect rcItem;
		CRect rcRect;

		//获取状态
		HTREEITEM hParent=GetParentItem(hItemCurrent);
		UINT uItemState=GetItemState(hItemCurrent,TVIF_STATE);

		//获取属性
		bool bDrawChildren=(ItemHasChildren(hItemCurrent)==TRUE);
		bool bDrawSelected=(uItemState&TVIS_SELECTED)&&((this==GetFocus())||(uTreeStyte&TVS_SHOWSELALWAYS));

		//获取区域
		if (GetItemRect(hItemCurrent,rcItem,TRUE))
		{
			//绘画过滤
			if (rcItem.top>=rcClient.bottom) break;
			if (rcItem.top>=rcClipBox.bottom) continue;

			//设置位置
			rcRect.left=0;
			rcRect.top=rcItem.top+1;
			rcRect.bottom=rcItem.bottom;
			rcRect.right=rcClient.Width();

			//绘画选择
			if (bDrawSelected==true)
			{
				pDC->FillSolidRect(&rcRect,RGB(253,231,161));
			}

			//绘画经过
			if ((bDrawSelected==false)&&(m_hItemMouseHover==hItemCurrent))
			{
				pDC->FillSolidRect(&rcRect,pSkinRenderManager->RenderColor(0.36,1.00));
			}

			//绘制箭头
			if (bDrawChildren==true)
			{
				//计算位置
				INT nXPos=rcItem.left-m_ImageArrow.GetWidth()/2-25;
				INT nYPos=rcItem.top+1+(rcItem.Height()-m_ImageArrow.GetHeight())/2;

				//绘画图标
				INT nIndex=(uItemState&TVIS_EXPANDED)?1L:0L;
				m_ImageArrow.DrawImage(pDC,nXPos,nYPos,m_ImageArrow.GetWidth()/2,m_ImageArrow.GetHeight(),nIndex*m_ImageArrow.GetWidth()/2,0);
			}

			//绘制列表
			DrawListImage(pDC,rcItem,hItemCurrent);	

			//绘制文本
			DrawItemString(pDC,rcItem,hItemCurrent,bDrawSelected);
		}
	} while ((hItemCurrent=GetNextVisibleItem(hItemCurrent))!= NULL);

	return;
}

//绘画背景
VOID CServerListView::DrawTreeBack(CDC * pDC, CRect & rcClient, CRect & rcClipBox)
{
	//绘画背景
	pDC->FillSolidRect(&rcClient,RGB(255,255,255));

	//绘制横线
	for (INT nYPos=m_nYScroll/ITEM_HEIGHT*ITEM_HEIGHT;nYPos<rcClient.Height();nYPos+=ITEM_HEIGHT)
	{
		pDC->FillSolidRect(0,nYPos,rcClient.Width(),1,RGB(223,223,223));
	}

	return;
}

//绘画图标
VOID CServerListView::DrawListImage(CDC * pDC, CRect rcRect, HTREEITEM hTreeItem)
{
	//获取属性
	INT nImage,nSelectedImage;
	GetItemImage(hTreeItem,nImage,nSelectedImage);

	//获取信息
	IMAGEINFO ImageInfo;
	m_ImageList.GetImageInfo(nImage,&ImageInfo);

	//绘画图标
	INT nImageWidth=ImageInfo.rcImage.right-ImageInfo.rcImage.left;
	INT nImageHeight=ImageInfo.rcImage.bottom-ImageInfo.rcImage.top;
	m_ImageList.Draw(pDC,nImage,CPoint(rcRect.left-nImageWidth-3,rcRect.top+(rcRect.Height()-nImageHeight)/2+1),ILD_TRANSPARENT);

	return;
}

//绘制文本
VOID CServerListView::DrawItemString(CDC * pDC, CRect rcRect, HTREEITEM hTreeItem, bool bSelected)
{
	//变量定义
	COLORREF crString=RGB(150,150,150);
	CGameListItem * pGameListItem=(CGameListItem *)GetItemData(hTreeItem);

	//颜色定义
	if (pGameListItem!=NULL)
	{
		//选择字体
		switch (pGameListItem->GetItemGenre())
		{
		case ItemGenre_Type:		//种类子项
			{
				//设置颜色
				crString=RGB(0,0,0);

				//设置字体
				pDC->SelectObject(m_FontBold);

				break;
			}
		case ItemGenre_Inside:		//内部子项
			{
				//设置颜色
				crString=RGB(0,0,0);

				//设置字体
				CGameInsideItem * pGameInsideItem=(CGameInsideItem *)pGameListItem;
				pDC->SelectObject((pGameInsideItem->m_dwInsideID==0)?m_FontBold:CSkinResourceManager::GetInstance()->GetDefaultFont());

				break;
			}
		default:					//其他子项
			{
				//设置颜色
				crString=RGB(0,0,0);

				//设置字体
				pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());
			}
		}
	}
	else
	{
		//设置颜色
		crString=RGB(0,0,0);

		//设置字体
		pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());
	}

	//设置环境
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(crString);

	//绘画字体
	CString strString=GetItemText(hTreeItem);
	pDC->DrawText(strString,rcRect,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	return;
}

//获取通知
VOID CServerListView::OnGameItemFinish()
{
	//变量定义
	POSITION Position=NULL;
	CServerListData * pServerListData=CServerListData::GetInstance();

	//展开列表
	for (DWORD i=0;i<pServerListData->GetGameTypeCount();i++)
	{
		CGameTypeItem * pGameTypeItem=pServerListData->EmunGameTypeItem(Position);
		if (pGameTypeItem!=NULL) ExpandListItem(pGameTypeItem);
	}

	//展开列表
	if (m_TreeListRoot!=NULL) ExpandListItem(m_TreeListRoot);

	//保证可视
	EnsureVisible(m_TreeListRoot);

	//更新人数
	SetTimer(IDI_UPDATE_ONLINE,TIME_UPDATE_ONLINE,NULL);

	return;
}

//获取通知
VOID CServerListView::OnGameKindFinish(WORD wKindID)
{
	//获取类型
	CServerListData * pServerListData=CServerListData::GetInstance();
	CGameKindItem * pGameKindItem=pServerListData->SearchGameKind(wKindID);

	//类型处理
	if (pGameKindItem!=NULL)
	{
		//变量定义
		LPCTSTR pszTitle=TEXT("没有可用游戏房间");
		HTREEITEM hItemAttachUpdate=pGameKindItem->m_hItemAttachUpdate;
		HTREEITEM hItemNormalUpdate=pGameKindItem->m_hItemNormalUpdate;

		//更新标题
		if (hItemAttachUpdate!=NULL) SetItemText(hItemAttachUpdate,pszTitle);
		if (hItemNormalUpdate!=NULL) SetItemText(hItemNormalUpdate,pszTitle);
	}

	return;
}

//插入通知
VOID CServerListView::OnGameItemInsert(CGameListItem * pGameListItem)
{
	//效验参数
	ASSERT(pGameListItem!=NULL);
	if (pGameListItem==NULL) return;

	//插入处理
	switch (pGameListItem->GetItemGenre())
	{
	case ItemGenre_Type:	//游戏种类
		{
			//变量定义
			CGameTypeItem * pGameTypeItem=(CGameTypeItem *)pGameListItem;
			CGameListItem * pParentListItem=pGameListItem->GetParentListItem();

			//属性定义
			LPCTSTR pszTypeName=pGameTypeItem->m_GameType.szTypeName;

			//插入新项
			if (pParentListItem!=NULL)
			{
				//常规插入
				HTREEITEM hTreeItemNormal=pParentListItem->m_hTreeItemNormal;
				if (hTreeItemNormal!=NULL) pGameTypeItem->m_hTreeItemNormal=InsertGameListItem(pszTypeName,IND_TYPE,pGameTypeItem,hTreeItemNormal);

				//喜爱插入
				HTREEITEM hTreeItemAttach=pParentListItem->m_hTreeItemAttach;
				if (hTreeItemAttach!=NULL) pGameTypeItem->m_hTreeItemAttach=InsertGameListItem(pszTypeName,IND_TYPE,pGameTypeItem,hTreeItemAttach);
			}
			else
			{
				//常规插入
				HTREEITEM hTreeItemNormal=m_TreeListRoot;
				pGameTypeItem->m_hTreeItemNormal=InsertGameListItem(pszTypeName,IND_TYPE,pGameTypeItem,hTreeItemNormal);
			}

			//设置状态
			if (pGameTypeItem->m_hTreeItemNormal!=NULL) SetItemState(pGameTypeItem->m_hTreeItemNormal,TVIS_BOLD,TVIS_BOLD);
			if (pGameTypeItem->m_hTreeItemAttach!=NULL) SetItemState(pGameTypeItem->m_hTreeItemAttach,TVIS_BOLD,TVIS_BOLD);

			break;
		}
	case ItemGenre_Kind:	//游戏类型
		{
			//变量定义
			CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;
			CGameListItem * pParentListItem=pGameListItem->GetParentListItem();

			//插入新项
			if (pParentListItem!=NULL)
			{
				//变量定义
				UINT nUpdateImage=IND_SERVER_UPDATE;
				UINT uNormalImage=GetGameImageIndex(pGameKindItem);

				//插入处理
				TCHAR szTitle[64]=TEXT("");
				LPCTSTR pszUpdateName=TEXT("正在下载房间列表...");
				GetGameItemTitle(pGameKindItem,szTitle,CountArray(szTitle));

				//常规插入
				if (pParentListItem->m_hTreeItemNormal!=NULL)
				{
					HTREEITEM hTreeItemNormal=pParentListItem->m_hTreeItemNormal;
					pGameKindItem->m_hTreeItemNormal=InsertGameListItem(szTitle,uNormalImage,pGameKindItem,hTreeItemNormal);
					pGameKindItem->m_hItemNormalUpdate=InsertGameListItem(pszUpdateName,nUpdateImage,NULL,pGameKindItem->m_hTreeItemNormal);
				}

				//喜爱插入
				if (pParentListItem->m_hTreeItemAttach!=NULL)
				{
					HTREEITEM hTreeItemAttach=pParentListItem->m_hTreeItemAttach;
					pGameKindItem->m_hTreeItemAttach=InsertGameListItem(szTitle,uNormalImage,pGameKindItem,hTreeItemAttach);
					pGameKindItem->m_hItemAttachUpdate=InsertGameListItem(pszUpdateName,nUpdateImage,NULL,pGameKindItem->m_hTreeItemAttach);
				}
			}

			break;
		}
	case ItemGenre_Node:	//游戏节点
		{
			//变量定义
			CGameNodeItem * pGameNodeItem=(CGameNodeItem *)pGameListItem;
			CGameListItem * pParentListItem=pGameListItem->GetParentListItem();

			//插入新项
			if (pParentListItem!=NULL)
			{
				//变量定义
				LPCTSTR pszNodeName=pGameNodeItem->m_GameNode.szNodeName;

				//删除更新
				DeleteUpdateItem(pGameNodeItem->GetParentListItem());

				//常规插入
				HTREEITEM hTreeItemNormal=pParentListItem->m_hTreeItemNormal;
				if (hTreeItemNormal!=NULL) pGameNodeItem->m_hTreeItemNormal=InsertGameListItem(pszNodeName,IND_NODE,pGameNodeItem,hTreeItemNormal);

				//喜爱插入
				HTREEITEM hTreeItemAttach=pParentListItem->m_hTreeItemAttach;
				if (hTreeItemAttach!=NULL) pGameNodeItem->m_hTreeItemAttach=InsertGameListItem(pszNodeName,IND_NODE,pGameNodeItem,hTreeItemAttach);
			}

			break;
		}
	case ItemGenre_Page:	//定制子项
		{
			//变量定义
			CGameListItem * pParentListItem=pGameListItem->GetParentListItem();
			LPCTSTR pszDisplayName=(((CGamePageItem *)pGameListItem)->m_GamePage.szDisplayName);

			//插入新项
			if (pParentListItem!=NULL)
			{
				//常规插入
				HTREEITEM hTreeItemNormal=pParentListItem->m_hTreeItemNormal;
				if (hTreeItemNormal!=NULL) pParentListItem->m_hTreeItemNormal=InsertGameListItem(pszDisplayName,IND_BROWSE,pParentListItem,hTreeItemNormal);

				//喜爱插入
				HTREEITEM hTreeItemAttach=pParentListItem->m_hTreeItemAttach;
				if (hTreeItemAttach!=NULL) pParentListItem->m_hTreeItemAttach=InsertGameListItem(pszDisplayName,IND_BROWSE,pParentListItem,hTreeItemAttach);
			}
			else
			{
				//常规插入
				HTREEITEM hTreeItemNormal=m_TreeListRoot;
				pGameListItem->m_hTreeItemNormal=InsertGameListItem(pszDisplayName,IND_BROWSE,pParentListItem,hTreeItemNormal);
			}

			break;
		}
	case ItemGenre_Server:	//游戏房间
		{
			//变量定义
			CGameListItem * pParentListItem=pGameListItem->GetParentListItem();
			CGameServerItem * pGameServerItem=(CGameServerItem *)pGameListItem;

			//插入新项
			if (pParentListItem!=NULL)
			{
				//插入处理
				TCHAR szTitle[64]=TEXT("");
				UINT uImageIndex=GetGameImageIndex(pGameServerItem);
				GetGameItemTitle(pGameServerItem,szTitle,CountArray(szTitle));

				//删除更新
				DeleteUpdateItem(pGameServerItem->GetParentListItem());

				//常规插入
				HTREEITEM hTreeItemNormal=pParentListItem->m_hTreeItemNormal;
				if (hTreeItemNormal!=NULL) pGameServerItem->m_hTreeItemNormal=InsertGameListItem(szTitle,uImageIndex,pGameServerItem,hTreeItemNormal);

				//喜爱插入
				HTREEITEM hTreeItemAttach=pParentListItem->m_hTreeItemAttach;
				if (hTreeItemAttach!=NULL) pGameServerItem->m_hTreeItemAttach=InsertGameListItem(szTitle,uImageIndex,pGameServerItem,hTreeItemAttach);
			}

			break;
		}
	}

	return;
}

//更新通知
VOID CServerListView::OnGameItemUpdate(CGameListItem * pGameListItem)
{
	//效验参数
	ASSERT(pGameListItem!=NULL);
	if (pGameListItem==NULL) return;

	//插入处理
	switch (pGameListItem->GetItemGenre())
	{
	case ItemGenre_Type:	//游戏种类
		{
			//变量定义
			CGameTypeItem * pGameTypeItem=(CGameTypeItem *)pGameListItem;

			//设置子项
			if (pGameTypeItem->m_hTreeItemAttach!=NULL)
			{
				LPCTSTR pszTypeName(pGameTypeItem->m_GameType.szTypeName);
				ModifyGameListItem(pGameTypeItem->m_hTreeItemAttach,pszTypeName,IND_TYPE);
			}

			//设置子项
			if (pGameTypeItem->m_hTreeItemNormal!=NULL)
			{
				LPCTSTR pszTypeName(pGameTypeItem->m_GameType.szTypeName);
				ModifyGameListItem(pGameTypeItem->m_hTreeItemNormal,pszTypeName,IND_TYPE);
			}

			break;
		}
	case ItemGenre_Kind:	//游戏类型
		{
			//变量定义
			CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;

			//构造数据
			TCHAR szTitle[64]=TEXT("");
			UINT uNormalImage=GetGameImageIndex(pGameKindItem);
			GetGameItemTitle(pGameKindItem,szTitle,CountArray(szTitle));

			//设置子项
			if (pGameKindItem->m_hTreeItemAttach!=NULL)
			{
				ModifyGameListItem(pGameKindItem->m_hTreeItemAttach,szTitle,uNormalImage);
			}

			//设置子项
			if (pGameKindItem->m_hTreeItemNormal!=NULL)
			{
				ModifyGameListItem(pGameKindItem->m_hTreeItemNormal,szTitle,uNormalImage);
			}

			break;
		}
	case ItemGenre_Node:	//游戏节点
		{
			//变量定义
			CGameNodeItem * pGameNodeItem=(CGameNodeItem *)pGameListItem;

			//设置子项
			if (pGameNodeItem->m_hTreeItemAttach!=NULL)
			{
				LPCTSTR pszNodeName(pGameNodeItem->m_GameNode.szNodeName);
				ModifyGameListItem(pGameNodeItem->m_hTreeItemAttach,pszNodeName,IND_NODE);
			}

			//设置子项
			if (pGameNodeItem->m_hTreeItemNormal!=NULL)
			{
				LPCTSTR pszNodeName(pGameNodeItem->m_GameNode.szNodeName);
				ModifyGameListItem(pGameNodeItem->m_hTreeItemNormal,pszNodeName,IND_NODE);
			}

			break;
		}
	case ItemGenre_Page:	//定制子项
		{
			//变量定义
			CGamePageItem * pGamePageItem=(CGamePageItem *)pGameListItem;

			//设置子项
			if (pGamePageItem->m_hTreeItemAttach!=NULL)
			{
				LPCTSTR pszDisplayName(pGamePageItem->m_GamePage.szDisplayName);
				ModifyGameListItem(pGamePageItem->m_hTreeItemAttach,pszDisplayName,IND_BROWSE);
			}

			//设置子项
			if (pGamePageItem->m_hTreeItemNormal!=NULL)
			{
				LPCTSTR pszDisplayName(pGamePageItem->m_GamePage.szDisplayName);
				ModifyGameListItem(pGamePageItem->m_hTreeItemNormal,pszDisplayName,IND_BROWSE);
			}

			break;
		}
	case ItemGenre_Server:	//游戏房间
		{
			//变量定义
			CGameServerItem * pGameServerItem=(CGameServerItem *)pGameListItem;

			//构造数据
			TCHAR szTitle[64]=TEXT("");
			UINT uImageIndex=GetGameImageIndex(pGameServerItem);
			GetGameItemTitle(pGameServerItem,szTitle,CountArray(szTitle));

			//设置子项
			if (pGameServerItem->m_hTreeItemAttach!=NULL)
			{
				ModifyGameListItem(pGameServerItem->m_hTreeItemAttach,szTitle,uImageIndex);
			}

			//设置子项
			if (pGameServerItem->m_hTreeItemNormal!=NULL)
			{
				ModifyGameListItem(pGameServerItem->m_hTreeItemNormal,szTitle,uImageIndex);
			}

			break;
		}
	}

	return;
}

//删除通知
VOID CServerListView::OnGameItemDelete(CGameListItem * pGameListItem)
{
	//效验参数
	ASSERT(pGameListItem!=NULL);
	if (pGameListItem==NULL) return;

	//删除树项
	if (pGameListItem->m_hTreeItemAttach!=NULL) DeleteItem(pGameListItem->m_hTreeItemAttach);
	if (pGameListItem->m_hTreeItemNormal!=NULL) DeleteItem(pGameListItem->m_hTreeItemNormal);

	return;
}

//获取图标
UINT CServerListView::GetGameImageIndex(CGameKindItem * pGameKindItem)
{
	//安装判断
	if (pGameKindItem->m_dwProcessVersion==0L) return IND_KIND_NODOWN;

	//寻找现存
	UINT uImageIndxe=0;
	tagGameKind * pGameKind=&pGameKindItem->m_GameKind;
	if (m_GameImageMap.Lookup(pGameKind->wKindID,uImageIndxe)==TRUE) return uImageIndxe;

	//加载图标
	if (pGameKindItem->m_dwProcessVersion!=0L)
	{
		//加载资源
		LPCTSTR strProcessName(pGameKind->szProcessName);
		HINSTANCE hInstance=AfxLoadLibrary(strProcessName);

		//加载图形
		CBitmap GameLogo;
		AfxSetResourceHandle(hInstance);
		if (GameLogo.LoadBitmap(TEXT("GAME_LOGO"))) uImageIndxe=m_ImageList.Add(&GameLogo,RGB(255,0,255));
		AfxSetResourceHandle(GetModuleHandle(NULL));

		//释放资源
		AfxFreeLibrary(hInstance);

		//保存信息
		if (uImageIndxe!=0L)
		{
			m_GameImageMap[pGameKind->wKindID]=uImageIndxe;
			return uImageIndxe;
		}
	}
	
	return IND_KIND_NODOWN;
}

//获取图标
UINT CServerListView::GetGameImageIndex(CGameServerItem * pGameServerItem)
{
	//获取图标
	if (pGameServerItem->m_ServerStatus==ServerStatus_Normal) return IND_SERVER_NORMAL;
	if (pGameServerItem->m_ServerStatus==ServerStatus_Entrance) return IND_SERVER_ENTRANCE;
	if (pGameServerItem->m_ServerStatus==ServerStatus_EverEntrance) return IND_SERVER_EVERENTRANCE;

	return IND_SERVER_NORMAL;
}

//获取标题
LPCTSTR CServerListView::GetGameItemTitle(CGameKindItem * pGameKindItem, LPTSTR pszTitle, UINT uMaxCount)
{
	//变量定义
	LPCTSTR pszKindName(pGameKindItem->m_GameKind.szKindName);
	DWORD dwOnLineCount=pGameKindItem->m_GameKind.dwOnLineCount;

	//构造标题
	if (pGameKindItem->m_dwProcessVersion!=0L)
	{
		_sntprintf(pszTitle,uMaxCount,TEXT("%s [ %ld ]"),(LPCTSTR)pszKindName,dwOnLineCount);
	}
	else
	{
		_sntprintf(pszTitle,uMaxCount,TEXT("%s [ %ld ] （双击下载）"),pszKindName,dwOnLineCount);
	}

	return pszTitle; 
}

//获取标题
LPCTSTR CServerListView::GetGameItemTitle(CGameServerItem * pGameServerItem, LPTSTR pszTitle, UINT uMaxCount)
{
	//变量定义
	LPCTSTR pszServerName(pGameServerItem->m_GameServer.szServerName);
	DWORD dwOnLineCount=pGameServerItem->m_GameServer.dwOnLineCount;

	//构造标题
	_sntprintf(pszTitle,uMaxCount,TEXT("%s [ %ld ]"),pszServerName,dwOnLineCount);

	return pszTitle; 
}

//删除更新
VOID CServerListView::DeleteUpdateItem(CGameListItem * pGameListItem)
{
	//效验参数
	ASSERT(pGameListItem!=NULL);
	if (pGameListItem==NULL) return;

	//删除更新
	while (pGameListItem!=NULL)
	{
		//类型判断
		if (pGameListItem->GetItemGenre()==ItemGenre_Kind)
		{
			//删除子项
			CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;
			if (pGameKindItem->m_hItemAttachUpdate!=NULL) DeleteItem(pGameKindItem->m_hItemAttachUpdate);
			if (pGameKindItem->m_hItemNormalUpdate!=NULL) DeleteItem(pGameKindItem->m_hItemNormalUpdate);

			//设置变量
			pGameKindItem->m_hItemAttachUpdate=NULL;
			pGameKindItem->m_hItemNormalUpdate=NULL;

			break;
		}

		//获取父项
		pGameListItem=pGameListItem->GetParentListItem();
	}

	return;
}

//修改子项
VOID CServerListView::ModifyGameListItem(HTREEITEM hTreeItem, LPCTSTR pszTitle, UINT uImage)
{
	//变量定义
	TVITEM TVItem;
	ZeroMemory(&TVItem,sizeof(TVItem));

	//子项属性
	TVItem.hItem=hTreeItem;
	TVItem.cchTextMax=64;
	TVItem.iImage=uImage;
	TVItem.iSelectedImage=uImage;
	TVItem.pszText=(LPTSTR)pszTitle;
	TVItem.mask=TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT;

	//修改子项
	SetItem(&TVItem);

	return;
}

//插入子项
HTREEITEM CServerListView::InsertInsideItem(LPCTSTR pszTitle, UINT uImage, DWORD dwInsideID, HTREEITEM hParentItem)
{
	//变量定义
	TV_INSERTSTRUCT InsertStruct;
	ZeroMemory(&InsertStruct,sizeof(InsertStruct));

	//创建变量
	CGameInsideItem * pGameInsideItem=new CGameInsideItem;
	if (pGameInsideItem!=NULL) pGameInsideItem->m_dwInsideID=dwInsideID;

	//设置变量
	InsertStruct.hParent=hParentItem;
	InsertStruct.hInsertAfter=TVI_LAST;

	//子项属性
	InsertStruct.item.cchTextMax=64;
	InsertStruct.item.iImage=uImage;
	InsertStruct.item.iSelectedImage=uImage;
	InsertStruct.item.pszText=(LPTSTR)pszTitle;
	InsertStruct.item.lParam=(LPARAM)pGameInsideItem;
	InsertStruct.item.mask=TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_PARAM;

	return InsertItem(&InsertStruct);
}

//插入子项
HTREEITEM CServerListView::InsertGameListItem(LPCTSTR pszTitle, UINT uImage, CGameListItem * pGameListItem, HTREEITEM hParentItem)
{
	//变量定义
	TV_INSERTSTRUCT InsertStruct;
	ZeroMemory(&InsertStruct,sizeof(InsertStruct));

	//设置变量
	InsertStruct.hParent=hParentItem;
	InsertStruct.hInsertAfter=TVI_FIRST;
	InsertStruct.item.cchTextMax=64;
	InsertStruct.item.iImage=uImage;
	InsertStruct.item.iSelectedImage=uImage;
	InsertStruct.item.pszText=(LPTSTR)pszTitle;
	InsertStruct.item.lParam=(LPARAM)pGameListItem;
	InsertStruct.item.mask=TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_PARAM;

	//寻找子项
	if ((hParentItem!=NULL)&&(pGameListItem!=NULL))
	{
		//变量定义
		WORD wSortID=pGameListItem->GetSortID();
		HTREEITEM hTreeItem=GetChildItem(hParentItem);

		//枚举子项
		while (hTreeItem!=NULL)
		{
			//获取数据
			CGameListItem * pGameListTemp=(CGameListItem *)GetItemData(hTreeItem);

			//数据对比
			if ((pGameListTemp!=NULL)&&(pGameListTemp->GetSortID()>wSortID)) break;

			//设置结果
			InsertStruct.hInsertAfter=hTreeItem;

			//切换树项
			hTreeItem=GetNextItem(hTreeItem,TVGN_NEXT);
		} 
	}

	return InsertItem(&InsertStruct);
}

//重画消息
VOID CServerListView::OnPaint()
{
	CPaintDC dc(this);

	//剪切位置
	CRect rcClip;
	dc.GetClipBox(&rcClip);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//创建缓冲
	CDC BufferDC;
	CBitmap BufferImage;
	BufferDC.CreateCompatibleDC(&dc);
	BufferImage.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());

	//设置 DC
	BufferDC.SelectObject(&BufferImage);

	//绘画控件
	DrawTreeBack(&BufferDC,rcClient,rcClip);
	DrawTreeItem(&BufferDC,rcClient,rcClip);

	//绘画背景
	dc.BitBlt(rcClip.left,rcClip.top,rcClip.Width(),rcClip.Height(),&BufferDC,rcClip.left,rcClip.top,SRCCOPY);

	//删除资源
	BufferDC.DeleteDC();
	BufferImage.DeleteObject();

	return;
}

//时间消息
VOID CServerListView::OnTimer(UINT nIDEvent)
{
	//更新人数
	if (nIDEvent==IDI_UPDATE_ONLINE)
	{
		ASSERT(CMissionList::GetInstance()!=NULL);
		CMissionList::GetInstance()->UpdateOnLineInfo();

		return;
	}

	__super::OnTimer(nIDEvent);
}

//绘画背景
BOOL CServerListView::OnEraseBkgnd(CDC * pDC)
{
	return TRUE;
}

//位置消息
VOID CServerListView::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//获取大小
	CRect rcClient;
	GetClientRect(&rcClient);

	//获取信息
	SCROLLINFO ScrollInfoH;
	SCROLLINFO ScrollInfoV;
	ZeroMemory(&ScrollInfoH,sizeof(ScrollInfoH));
	ZeroMemory(&ScrollInfoV,sizeof(ScrollInfoV));

	//获取信息
	GetScrollInfo(SB_HORZ,&ScrollInfoH,SIF_POS|SIF_RANGE);
	GetScrollInfo(SB_VERT,&ScrollInfoV,SIF_POS|SIF_RANGE);

	//设置变量
	m_nXScroll=-ScrollInfoH.nPos;
	m_nYScroll=-ScrollInfoV.nPos;

	return;
}

//光标消息
BOOL CServerListView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//获取光标
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//子项测试
	HTREEITEM hItemMouseHover=HitTest(MousePoint);

	//重画判断
	if ((hItemMouseHover!=NULL)&&(hItemMouseHover!=m_hItemMouseHover))
	{
		//设置变量
		m_hItemMouseHover=hItemMouseHover;

		//重画界面
		Invalidate(FALSE);
	}

	//设置光标
	if (hItemMouseHover!=NULL)
	{
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_HAND_CUR)));
		return true;
	}

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//右键列表
VOID CServerListView::OnNMRClick(NMHDR * pNMHDR, LRESULT * pResult)
{
	//获取选择
	HTREEITEM hTreeItem=GetCurrentSelectItem(false);

	//选择判断
	if (hTreeItem==NULL) return;

	//设置选择
	Select(hTreeItem,TVGN_CARET);

	//获取数据
/*	CGameListItem * pGameListItem=(CGameListItem *)GetItemData(hTreeItem);
	if (pGameListItem!=NULL)
	{
		switch (pGameListItem->GetItemGenre())
		{
		case ItemGenre_Kind:	//游戏种类
			{
				//变量定义
				TCHAR szBuffer[64]=TEXT("");
				CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;

				//构造菜单
				CSkinMenu Menu;
				Menu.CreateMenu();

				//自动进入
				Menu.AppendMenu(IDM_ENTER_SERVER,TEXT("自动进入"));
				Menu.AppendSeparator();

				//下载游戏
				CW2CT pszKindName(pGameKindItem->m_GameKind.szKindName);
				_sntprintf(szBuffer,CountArray(szBuffer),TEXT("下载“%s”"),(LPCTSTR)pszKindName);
				Menu.AppendMenu(IDM_DOWN_LOAD_CLIENT,szBuffer);

				//当前版本
				if (pGameKindItem->m_dwProcessVersion!=0)
				{
					DWORD dwVersion=pGameKindItem->m_dwProcessVersion;
					_sntprintf(szBuffer,CountArray(szBuffer),TEXT("安装版本 %d.%d.%d.%d"),GetProductVer(dwVersion),
						GetMainVer(dwVersion),GetSubVer(dwVersion),GetBuildVer(dwVersion));
					Menu.AppendMenu(IDM_NULL_COMMAND,szBuffer,MF_GRAYED);
				}
				else Menu.AppendMenu(IDM_DOWN_LOAD_CLIENT,TEXT("没有安装（点击下载）"));

				//控制菜单
				Menu.AppendSeparator();
				bool bExpand=ExpandVerdict(hTreeItem);
				Menu.AppendMenu(bExpand?IDM_SHRINK_LIST:IDM_EXPAND_LIST,bExpand?TEXT("收缩列表"):TEXT("展开列表"));

				//弹出菜单
				Menu.TrackPopupMenu(this);

				return;
			}
		case ItemGenre_Server:	//游戏房间
			{
				//变量定义
				TCHAR szBuffer[64]=TEXT("");
				CGameServerItem * pGameServerItem=(CGameServerItem *)pGameListItem;
				CGameKindItem * pGameKindItem=pGameServerItem->m_pGameKindItem;

				//构造菜单
				CSkinMenu Menu;
				Menu.CreateMenu();
				Menu.AppendMenu(IDM_ENTER_SERVER,TEXT("进入游戏房间"));
				Menu.AppendSeparator();
				Menu.AppendMenu(IDM_SET_COLLECTION,TEXT("设为常用服务器"));

				//下载游戏
				CW2CT pszKindName(pGameKindItem->m_GameKind.szKindName);
				_sntprintf(szBuffer,CountArray(szBuffer),TEXT("下载“%s”"),(LPCTSTR)pszKindName);
				Menu.AppendMenu(IDM_DOWN_LOAD_CLIENT,szBuffer);
				Menu.AppendSeparator();

				//当前版本
				if (pGameKindItem->m_dwProcessVersion!=0)
				{
					DWORD dwVersion=pGameKindItem->m_dwProcessVersion;
					_sntprintf(szBuffer,CountArray(szBuffer),TEXT("安装版本 %d.%d.%d.%d"),GetProductVer(dwVersion),
						GetMainVer(dwVersion),GetSubVer(dwVersion),GetBuildVer(dwVersion));
					Menu.AppendMenu(IDM_NULL_COMMAND,szBuffer,MF_GRAYED);
				}
				else Menu.AppendMenu(IDM_DOWN_LOAD_CLIENT,TEXT("没有安装（点击下载）"));

				//菜单设置
				bool Collection=false;//pGameServerItem->IsCollection();
				if (Collection==true) Menu.CheckMenuItem(IDM_SET_COLLECTION,MF_BYCOMMAND|MF_CHECKED);

				//弹出菜单
				Menu.TrackPopupMenu(this);

				return;
			}
		}
	}*/

	return;
}

//左击列表
VOID CServerListView::OnNMLClick(NMHDR * pNMHDR, LRESULT * pResult)
{
	//获取选择
	HTREEITEM hTreeItem=GetCurrentSelectItem(false);

	//选择判断
	if (hTreeItem==NULL) return;

	//设置选择
	Select(hTreeItem,TVGN_CARET);

	//获取数据
	CGameListItem * pGameListItem=(CGameListItem *)GetItemData(hTreeItem);

	//进入房间
	if ((pGameListItem!=NULL)&&(pGameListItem->GetItemGenre()==ItemGenre_Server))
	{
		//进入房间
		CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
		pPlatformFrame->EntranceServerItem((CGameServerItem *)pGameListItem);

		return;
	}

	//内部子项
	if ((pGameListItem!=NULL)&&(pGameListItem->GetItemGenre()==ItemGenre_Inside))
	{
		//变量定义
		CGameInsideItem * pGameInsideItem=(CGameInsideItem *)pGameListItem;

		//工作目录
		TCHAR szDirectory[MAX_PATH]=TEXT("");
		CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

		//构造路径
		TCHAR szAssistantPath[MAX_PATH]=TEXT("");
		_sntprintf(szAssistantPath,CountArray(szAssistantPath),TEXT("%s\\Collocate\\Collocate.INI"),szDirectory);

		//读取地址
		TCHAR szItemName[128]=TEXT(""),szAssistantLink[128]=TEXT("");
		_sntprintf(szItemName,CountArray(szItemName),TEXT("Assistant%ld"),pGameInsideItem->m_dwInsideID);
		GetPrivateProfileString(szItemName,TEXT("AssistantLink"),TEXT(""),szAssistantLink,CountArray(szAssistantLink),szAssistantPath);

		//连接地址
		if (szAssistantLink[0]!=0)
		{
			CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
			if (pPlatformFrame!=NULL) pPlatformFrame->WebBrowse(szAssistantLink,false);
		}

		return;
	}

	return;
}

//列表改变
VOID CServerListView::OnTvnSelchanged(NMHDR * pNMHDR, LRESULT * pResult)
{
	//获取选择
	HTREEITEM hTreeItem=GetSelectedItem();

	//选择判断
	if (hTreeItem==NULL) return;

	//获取数据
	CGameListItem * pGameListItem=(CGameListItem *)GetItemData(hTreeItem);

	//数据处理
	if (pGameListItem!=NULL)
	{
		switch (pGameListItem->GetItemGenre())
		{
		case ItemGenre_Kind:	//游戏种类
			{
				//变量定义
				WORD wGameID=((CGameKindItem *)pGameListItem)->m_GameKind.wGameID;

				//构造地址
				TCHAR szRuleLink[MAX_PATH]=TEXT("");
				_sntprintf(szRuleLink,CountArray(szRuleLink),TEXT("%s/GameRule.asp?GameID=%ld"),szPlatformLink,wGameID);

				//打开页面
				CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
				if (pPlatformFrame!=NULL) pPlatformFrame->WebBrowse(szRuleLink,false);

				return;
			}
		case ItemGenre_Page:	//定制类型
			{
				//变量定义
				WORD wPageID=((CGamePageItem *)pGameListItem)->m_GamePage.wPageID;

				//构造地址
				TCHAR szPageLink[MAX_PATH]=TEXT("");
				_sntprintf(szPageLink,CountArray(szPageLink),TEXT("%s/GamePage.asp?PageID=%ld"),szPlatformLink,wPageID);

				//打开页面
				CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
				if (pPlatformFrame!=NULL) pPlatformFrame->WebBrowse(szPageLink,false);

				return;
			}
		}
	}

	return;
}

//列表展开
VOID CServerListView::OnTvnItemexpanding(NMHDR * pNMHDR, LRESULT * pResult)
{
	//变量定义
	LPNMTREEVIEW pNMTreeView=reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	//动作处理
	if (pNMTreeView->action==TVE_EXPAND)
	{
		//获取子项
		CGameListItem * pGameListItem=(CGameListItem *)GetItemData(pNMTreeView->itemNew.hItem);

		//子项处理
		if ((pGameListItem!=NULL)&&(pGameListItem->GetItemGenre()==ItemGenre_Kind))
		{
			//变量定义
			DWORD dwNowTime=(DWORD)time(NULL);
			CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;
			bool bTimeOut=(dwNowTime>=(pGameKindItem->m_dwUpdateTime+30L));

			//更新判断
			if ((pGameKindItem->m_bUpdateItem==false)||(bTimeOut==true))
			{
				//设置列表
				pGameKindItem->m_bUpdateItem=true;
				pGameKindItem->m_dwUpdateTime=(DWORD)time(NULL);

				//设置任务
				CMissionList * pMissionList=CMissionList::GetInstance();
				if (pMissionList!=NULL) pMissionList->UpdateServerInfo(pGameKindItem->m_GameKind.wKindID);
			}

			return;
		}
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
