#include "Stdafx.h"
#include "Resource.h"
#include "ServerItemView.h"

//�궨��
#define WN_SET_LINE_COLOR				(TV_FIRST+40)		//������ɫ��Ϣ

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CServerItemView, CTreeCtrl)
	ON_NOTIFY_REFLECT(NM_CLICK, OnNMClick)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnNMRclick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnNMDblclk)
	ON_NOTIFY_REFLECT(NM_RDBLCLK, OnNMRdblclk)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnTvnSelchanged)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnTvnItemexpanded)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//��̬����
IServerListSink		* CListItem::m_pIServerListSink=NULL;		//�ص��ӿ�	

//��������
CListItem::~CListItem()
{
	m_dwData=0;
	m_pParentItem=NULL;
}

//ö������
CListItem * CListItem::EnumChildItem(INT_PTR nIndex)
{
	if (nIndex>=m_ListItemArray.GetCount()) return NULL;
	return m_ListItemArray[nIndex];
}

//��������
CListType * CListItem::SearchTypeItem(WORD wTypeID)
{
	for (INT_PTR i=0;i<m_ListItemArray.GetCount();i++)
	{
		CListItem * pListItem=m_ListItemArray[i];
		if (pListItem->GetItemGenre()==ItemGenre_Type)
		{
			CListType * pListType=(CListType *)pListItem;
			tagGameType * pGameType=pListType->GetItemInfo();
			if (pGameType->wTypeID==wTypeID) return pListType;
		}
	}

	return NULL;
}

//��������
CListKind * CListItem::SearchKindItem(WORD wKindID)
{
	for (INT_PTR i=0;i<m_ListItemArray.GetCount();i++)
	{
		CListItem * pListItem=m_ListItemArray[i];
		if (pListItem->GetItemGenre()==ItemGenre_Kind)
		{
			CListKind * pListKind=(CListKind *)pListItem;
			const tagGameKind * pGameKind=pListKind->GetItemInfo();
			if (pGameKind->wKindID==wKindID) return pListKind;
		}
	}

	return NULL;
}

//��������
CListStation * CListItem::SearchStationItem(WORD wKindID, WORD wStationID, bool bDeepSearch)
{
	//��������
	for (INT_PTR i=0;i<m_ListItemArray.GetCount();i++)
	{
		CListItem * pListItem=m_ListItemArray[i];
		if (pListItem->GetItemGenre()==ItemGenre_Station)
		{
			CListStation * pListStation=(CListStation *)pListItem;
			const tagGameStation * pGameStation=pListStation->GetItemInfo();
			if ((pGameStation->wStationID==wStationID)&&(pGameStation->wKindID==wKindID)) return pListStation;
		}
	}

	//�������
	if (bDeepSearch)
	{
		for (INT_PTR i=0;i<m_ListItemArray.GetCount();i++)
		{
			CListItem * pItemBase=m_ListItemArray[i];
			CListStation * pListStation=pItemBase->SearchStationItem(wKindID,wStationID,bDeepSearch);
			if (pListStation!=NULL) return pListStation;
		}
	}

	return NULL;
}

//��������
CListServer * CListItem::SearchServerItem(WORD wKindID, WORD wServerID, bool bDeepSearch)
{
	//��������
	for (INT_PTR i=0;i<m_ListItemArray.GetCount();i++)
	{
		CListItem * pListItem=m_ListItemArray[i];
		if (pListItem->GetItemGenre()==ItemGenre_Server)
		{
			CListServer * pListServer=(CListServer *)pListItem;
			const tagGameServer * pGameServer=pListServer->GetItemInfo();
			if ((pGameServer->wServerID==wServerID)&&(pGameServer->wKindID==wKindID)) return pListServer;
		}
	}

	//�������
	if (bDeepSearch)
	{
		for (INT_PTR i=0;i<m_ListItemArray.GetCount();i++)
		{
			CListItem * pItemBase=m_ListItemArray[i];
			CListServer * pListServer=pItemBase->SearchServerItem(wKindID,wServerID,bDeepSearch);
			if (pListServer!=NULL) return pListServer;
		}
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CServerListManager::CServerListManager()
{
	m_pIServerListSink=NULL;
}

//��������
CServerListManager::~CServerListManager()
{
	INT_PTR nCount=m_PtrArrayType.GetCount();
	int i=0;
	for (;i<nCount;i++) delete m_PtrArrayType[i];
	m_PtrArrayType.RemoveAll();

	nCount=m_PtrArrayKind.GetCount();
	i=0;
	for (;i<nCount;i++) delete m_PtrArrayKind[i];
	m_PtrArrayKind.RemoveAll();

	nCount=m_PtrArrayStation.GetCount();
	for (i=0;i<nCount;i++) delete m_PtrArrayStation[i];
	m_PtrArrayStation.RemoveAll();

	nCount=m_PtrArrayServer.GetCount();
	for (i=0;i<nCount;i++) delete m_PtrArrayServer[i];
	m_PtrArrayServer.RemoveAll();

	return;
}

//��ʼ������
bool CServerListManager::InitServerListManager(IServerListSink * pIServerListSink)
{
	//���ñ���
	m_pIServerListSink=pIServerListSink;
	CListItem::m_pIServerListSink=pIServerListSink;

	//��������
	tagGameInside GameInside;
	memset(&GameInside,0,sizeof(GameInside));
	lstrcpyn(GameInside.szDisplayName,szProductName,sizeof(GameInside.szDisplayName));
	InsertInsideItem(&GameInside,1);

	return true;
}

//չ���б�
bool CServerListManager::ExpandListItem(CListItem * pListItem)
{
	ASSERT(m_pIServerListSink!=NULL);
	return m_pIServerListSink->ExpandListItem(pListItem);
}

//ö������
CListType * CServerListManager::EnumTypeItem(INT_PTR nIndex)
{
	if (nIndex>=m_PtrArrayType.GetCount()) return NULL;
	return m_PtrArrayType[nIndex];
}

//ö����Ϸ
CListKind * CServerListManager::EnumKindItem(INT_PTR nIndex)
{
	if (nIndex>=m_PtrArrayKind.GetCount()) return NULL;
	return m_PtrArrayKind[nIndex];
}

//ö��վ��
CListStation * CServerListManager::EnumStationItem(INT_PTR nIndex)
{
	if (nIndex>=m_PtrArrayStation.GetCount()) return NULL;
	return m_PtrArrayStation[nIndex];
}

//ö�ٷ���
CListServer * CServerListManager::EnumServerItem(INT_PTR nIndex)
{
	if (nIndex>=m_PtrArrayServer.GetCount()) return NULL;
	return m_PtrArrayServer[nIndex];
}

//ö���ڲ���
CListInside * CServerListManager::EnumInsideItem(INT_PTR nIndex)
{
	if (nIndex>=m_PtrArrayInside.GetCount()) return NULL;
	return m_PtrArrayInside[nIndex];
}

//��������
bool CServerListManager::InsertTypeItem(tagGameType GameType[], WORD wCount)
{
	//Ч�����
	ASSERT(m_PtrArrayInside.GetCount()>0);
	if (wCount==0) return false;

	//��������
	CListType * pListType=NULL;
	CListInside * pRootListInside=m_PtrArrayInside[0];

	try
	{
		//��������
		CListType * pListTypeTemp=NULL;

		for (WORD i=0;i<wCount;i++)
		{
			//���Ҵ���
			pListTypeTemp=pRootListInside->SearchTypeItem(GameType[i].wTypeID);
			if (pListTypeTemp!=NULL) continue;

			//�������
			pListType=new CListType(pRootListInside);
			CopyMemory(pListType->GetItemInfo(),&GameType[i],sizeof(tagGameType));
			m_PtrArrayType.Add(pListType);
			m_pIServerListSink->OnListItemInserted(pListType);
		}
		return true;
	}
	catch (...) { SafeDelete(pListType); }

	return false;
}

//��������
bool CServerListManager::InsertKindItem(tagGameKind GameKind[], WORD wCount)
{
	CListKind * pListKind=NULL;
	try
	{
		//��������
		WORD wTypeID=0;
		CListType * pListType=NULL;
		CListKind * pListKindTemp=NULL;

		for (WORD i=0;i<wCount;i++)
		{
			//Ѱ�Ҹ���
			if ((pListType==NULL)||(GameKind[i].wTypeID!=wTypeID))
			{
				wTypeID=GameKind[i].wTypeID;
				pListType=SearchTypeItem(wTypeID);
				ASSERT(pListType!=NULL);
				if (pListType==NULL) continue;
			}

			//���Ҵ���
			pListKindTemp=pListType->SearchKindItem(GameKind[i].wKindID);
			if (pListKindTemp!=NULL) continue;

			//�������
			CWinFileInfo WinFileInfo;
			pListKind=new CListKind(pListType);
			CopyMemory(pListKind->GetItemInfo(),&GameKind[i],sizeof(tagGameKind));
			pListKind->m_bInstall=WinFileInfo.OpenWinFile(GameKind[i].szProcessName);
			m_PtrArrayKind.Add(pListKind);
			m_pIServerListSink->OnListItemInserted(pListKind);
		}
		return true;
	}
	catch (...) { SafeDelete(pListKind); }
	
	return false;
}

//��������
bool CServerListManager::InsertStationItem(tagGameStation GameStation[], WORD wCount)
{
	CListStation * pListStation=NULL;
	try
	{
		//��������
		WORD wKindID=0,wStationID=0;
		CListKind * pListKind=NULL;
		CListItem * pListParent=NULL;
		CListStation * pListStationJoin=NULL;
		CListStation * pListStationTemp=NULL;

		for (WORD i=0;i<wCount;i++)
		{
			//Ѱ������
			if ((pListKind==NULL)||(GameStation[i].wKindID!=wKindID))
			{
				pListKind=SearchKindItem(GameStation[i].wKindID);
				ASSERT(pListKind!=NULL);
				if (pListKind==NULL) continue;
				wKindID=GameStation[i].wKindID;
				pListParent=pListKind;
			}

			//���Ҵ���
			pListStationTemp=pListKind->SearchStationItem(GameStation[i].wKindID,GameStation[i].wStationID,true);
			if (pListStationTemp!=NULL) continue;

			//Ѱ��վ��
			if (GameStation[i].wJoinID!=0)
			{
				if ((pListStationJoin==NULL)||(GameStation[i].wJoinID!=wStationID))
				{
					pListStationJoin=pListKind->SearchStationItem(GameStation[i].wKindID,GameStation[i].wJoinID,true);
					ASSERT(pListStationJoin!=NULL);
					if (pListStationJoin==NULL) continue;
					wStationID=GameStation[i].wJoinID;
				}
				pListParent=pListStationJoin;
			}

			//�������
			pListStation=new CListStation(pListParent,pListKind);
			CopyMemory(pListStation->GetItemInfo(),&GameStation[i],sizeof(tagGameStation));
			m_PtrArrayStation.Add(pListStation);
			m_pIServerListSink->OnListItemInserted(pListStation);
		}
		return true;
	}
	catch (...) { SafeDelete(pListStation); }

	return false;
}

//��������
bool CServerListManager::InsertServerItem(tagGameServer GameServer[], WORD wCount)
{
	CListServer * pListServer=NULL;
	try
	{
		//��������
		WORD wKindID=0,wStationID=0;
		CListKind * pListKind=NULL;
		CListItem * pListParent=NULL;
		CListStation * pListStation=NULL;
		CListServer * pListServerTemp=NULL;

		for (WORD i=0;i<wCount;i++)
		{
			//Ѱ������
			if ((pListKind==NULL)||(GameServer[i].wKindID!=wKindID))
			{
				pListKind=SearchKindItem(GameServer[i].wKindID);
				ASSERT(pListKind!=NULL);
				if (pListKind==NULL) continue;
				wKindID=GameServer[i].wKindID;
				pListParent=pListKind;
			}

			//���Ҵ���
			pListServerTemp=pListKind->SearchServerItem(GameServer[i].wKindID,GameServer[i].wServerID,true);
			if (pListServerTemp!=NULL) continue;

			//Ѱ��վ��
			if (GameServer[i].wStationID!=0)
			{
				if ((pListStation==NULL)||(GameServer[i].wStationID!=wStationID))
				{
					pListStation=pListKind->SearchStationItem(GameServer[i].wKindID,GameServer[i].wStationID,true);
					if (pListStation==NULL) continue;
					wStationID=GameServer[i].wStationID;
				}
				pListParent=pListStation;
			}

			//�������
			pListServer=new CListServer(pListParent,pListKind);
			CopyMemory(pListServer->GetItemInfo(),&GameServer[i],sizeof(tagGameServer));
			m_PtrArrayServer.Add(pListServer);
			m_pIServerListSink->OnListItemInserted(pListServer);
		}
		return true;
	}
	catch (...) { SafeDelete(pListServer); }

	return false;
}

//��������
bool CServerListManager::InsertInsideItem(tagGameInside GameInside[], WORD wCount)
{
	CListInside * pListInside=NULL;
	try
	{
		for (WORD i=0;i<wCount;i++)
		{
			pListInside=new CListInside(NULL);
			CopyMemory(pListInside->GetItemInfo(),&GameInside[i],sizeof(tagGameInside));
			m_PtrArrayInside.Add(pListInside);
			m_pIServerListSink->OnListItemInserted(pListInside);
		}
		return true;
	}
	catch (...) { SafeDelete(pListInside); }

	return false;
}

//��������
CListType * CServerListManager::SearchTypeItem(WORD wTypeID)
{
	for (INT_PTR i=0;i<m_PtrArrayType.GetCount();i++)
	{
		CListType * pListType=m_PtrArrayType[i];
		tagGameType * pGameType=pListType->GetItemInfo();
		if (pGameType->wTypeID==wTypeID) return pListType;
	}
	return NULL;
}

//��������
CListKind * CServerListManager::SearchKindItem(WORD wKindID)
{
	for (INT_PTR i=0;i<m_PtrArrayKind.GetCount();i++)
	{
		CListKind * pListKind=m_PtrArrayKind[i];
		tagGameKind * pGameKind=pListKind->GetItemInfo();
		if (pGameKind->wKindID==wKindID) return pListKind;
	}
	return NULL;
}

//��������
CListStation * CServerListManager::SearchStationItem(WORD wKindID, WORD wStationID)
{
	for (INT_PTR i=0;i<m_PtrArrayStation.GetCount();i++)
	{
		CListStation * pListStation=m_PtrArrayStation[i];
		tagGameStation * pGameStation=pListStation->GetItemInfo();
		if ((pGameStation->wStationID==wStationID)&&(pGameStation->wKindID==wKindID)) return pListStation;
	}
	return NULL;
}

//��������
CListServer * CServerListManager::SearchServerItem(WORD wKindID, WORD wServerID)
{
	for (INT_PTR i=0;i<m_PtrArrayServer.GetCount();i++)
	{
		CListServer * pListServer=m_PtrArrayServer[i];
		tagGameServer * pGameServer=pListServer->GetItemInfo();
		if ((pGameServer->wServerID==wServerID)&&(pGameServer->wKindID==wKindID)) return pListServer;
	}
	return NULL;
}

//��������
bool CServerListManager::UpdateGameKind(WORD wKindID)
{
	CListKind * pListKind=SearchKindItem(wKindID);
	if (pListKind!=NULL)
	{
		CWinFileInfo WinFileInfo;
		tagGameKind * pGameKind=pListKind->GetItemInfo();
		pListKind->m_bInstall=WinFileInfo.OpenWinFile(pGameKind->szProcessName);
		m_pIServerListSink->OnListItemUpdate(pListKind);
		return true;
	}

	return false;
}

//��������
bool CServerListManager::UpdateGameOnLineCount(DWORD dwOnLineCount)
{
	if (m_PtrArrayInside.GetCount()>0)
	{
		CListInside * pListInside=m_PtrArrayInside[0];
		tagGameInside * pGameInside=pListInside->GetItemInfo();
		_snprintf(pGameInside->szDisplayName,sizeof(pGameInside->szDisplayName),TEXT("%s [ %ld ��]"),szProductName,dwOnLineCount);
		m_pIServerListSink->OnListItemUpdate(pListInside);
		return true;
	}

	return false;
}

//��������
bool CServerListManager::UpdateGameKindOnLine(WORD wKindID, DWORD dwOnLineCount)
{
	//Ѱ������
	CListKind * pListKind=SearchKindItem(wKindID);
	if (pListKind!=NULL)
	{
		tagGameKind * pGameKind=pListKind->GetItemInfo();
		pGameKind->dwOnLineCount=dwOnLineCount;
		m_pIServerListSink->OnListItemUpdate(pListKind);
		return true;
	}

	return false;
}

//��������
bool CServerListManager::UpdateGameServerOnLine(CListServer * pListServer, DWORD dwOnLineCount)
{
	//Ч�����
	ASSERT(pListServer!=NULL);
	if (pListServer==NULL) return false;

	//������Ϣ
	tagGameServer * pGameServer=pListServer->GetItemInfo();
	pGameServer->dwOnLineCount=dwOnLineCount;
	m_pIServerListSink->OnListItemUpdate(pListServer);

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CServerItemView::CServerItemView()
{
	m_bShowOnLineCount=false;
}

//��������
CServerItemView::~CServerItemView()
{
}

//��ʾ����
void CServerItemView::ShowOnLineCount(bool bShowOnLineCount)
{
	bShowOnLineCount=true;
	if (m_bShowOnLineCount!=bShowOnLineCount)
	{
		m_bShowOnLineCount=bShowOnLineCount;
	}

	return;
}

//���ú���
bool CServerItemView::InitServerItemView(ITreeCtrlSink * pITreeCtrlSink)
{	
	//���ÿؼ�
	SetItemHeight(20);
	SetTextColor(RGB(0,0,0));
	SetBkColor(RGB(230,249,255));
	// modify by yijian
	ModifyStyle(0,TVS_HASBUTTONS|TVS_HASLINES|TVS_SHOWSELALWAYS|TVS_TRACKSELECT);
	SendMessage(WN_SET_LINE_COLOR,0,(LPARAM)RGB(72,79,63));

	//���ñ���
	ASSERT(pITreeCtrlSink!=NULL);
	m_pITreeCtrlSink=pITreeCtrlSink;

	//����ͼƬ
	if (m_ImageList.GetSafeHandle()==NULL)
	{
		CBitmap ServerImage;
		ServerImage.LoadBitmap(IDB_SERVER_LIST_IMAGE);
		m_ImageList.Create(18,18,ILC_COLOR16|ILC_MASK,0,0);
		m_ImageList.Add(&ServerImage,RGB(255,0,255));
		SetImageList(&m_ImageList,LVSIL_NORMAL);
	}

	return true;
}

//չ���б�
bool __cdecl CServerItemView::ExpandListItem(CListItem * pListItem)
{
	//Ч�����
	ASSERT(pListItem!=NULL);

	//չ���б�
	HTREEITEM hTreeItem=(HTREEITEM)pListItem->GetItemData();
	if (hTreeItem!=NULL) 
	{
		Expand(hTreeItem,TVE_EXPAND);
		return true;
	}

	return false;
}

//����֪ͨ
void __cdecl CServerItemView::OnListItemUpdate(CListItem * pListItem)
{
	//Ч�����
	ASSERT(pListItem!=NULL);

	//������ʾ
	switch (pListItem->GetItemGenre())
	{
	case ItemGenre_Kind:		//��Ϸ����
		{
			//��������
			CListKind * pListKind=(CListKind *)pListItem;

			//��ȡ��Ϣ
			TCHAR szItemTitle[128]=TEXT("");
			DWORD dwImageIndex=GetGameImageIndex(pListKind);
			GetGameItemTitle(pListKind,szItemTitle,sizeof(szItemTitle));

			//��������
			HTREEITEM hTreeItem=(HTREEITEM)pListKind->GetItemData();
			SetItem(hTreeItem,TVIF_IMAGE|TVIF_TEXT|TVIF_SELECTEDIMAGE,szItemTitle,dwImageIndex,dwImageIndex,0,0,0);

			return;
		}
	case ItemGenre_Server:		//��Ϸ����
		{
			//��������
			CListServer * pListServer=(CListServer *)pListItem;
			tagGameServer * pGameServer=pListServer->GetItemInfo();

			//��ȡ��Ϣ
			TCHAR szItemTitle[128]=TEXT("");
			GetGameItemTitle(pListServer,szItemTitle,sizeof(szItemTitle));

			//������ʾ
			HTREEITEM hTreeItem=(HTREEITEM)pListServer->GetItemData();
			SetItemText(hTreeItem,szItemTitle);

			//��������
			INT_PTR nIndex=0;
			CListItem * pListItemTemp=NULL;
			CListKind * pListKind=pListServer->GetListKind();
			tagGameKind * pGameKind=pListKind->GetItemInfo();
			pGameKind->dwOnLineCount=0L;
			do
			{
				pListItemTemp=pListKind->EnumChildItem(nIndex++);
				if (pListItemTemp==NULL) break;
				if (pListItemTemp->GetItemGenre()==ItemGenre_Server)
				{
					pListServer=(CListServer *)pListItemTemp;
					pGameKind->dwOnLineCount+=pListServer->GetItemInfo()->dwOnLineCount;
				}
			} while (true);

			//������ʾ
			GetGameItemTitle(pListKind,szItemTitle,sizeof(szItemTitle));
			hTreeItem=(HTREEITEM)pListKind->GetItemData();
			SetItemText(hTreeItem,szItemTitle);

			return;
		}
	case ItemGenre_Inside:	//�ڲ�����
		{
			//��������
			CListInside * pListInside=(CListInside *)pListItem;
			tagGameInside * pGameInside=pListInside->GetItemInfo();

			//��Ϣ����
			HTREEITEM hTreeItem=(HTREEITEM)pListInside->GetItemData();
			SetItemText(hTreeItem,pGameInside->szDisplayName);

			return;
		}
	}

	return;
}

//����֪ͨ
void __cdecl CServerItemView::OnListItemInserted(CListItem * pListItem)
{
	//Ч�����
	ASSERT(pListItem!=NULL);

	//��������
	TV_INSERTSTRUCT InsertInf;
	memset(&InsertInf,0,sizeof(InsertInf));

	//���ñ���
	InsertInf.item.cchTextMax=128;
	InsertInf.hInsertAfter=TVI_LAST;
	InsertInf.item.lParam=(LPARAM)pListItem;
	InsertInf.item.mask=TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_PARAM;

	//Ѱ�Ҹ���
	CListItem * pListParent=pListItem->GetParentItem();
	if (pListParent!=NULL) InsertInf.hParent=(HTREEITEM)pListParent->GetItemData();

	//��������
	switch (pListItem->GetItemGenre())
	{
	case ItemGenre_Type:		//��Ϸ����
		{
			//��������
			CListType * pListType=(CListType *)pListItem;
			tagGameType * pGameType=pListType->GetItemInfo();

			//��������
			InsertInf.item.iImage=IND_TYPE;
			InsertInf.item.iSelectedImage=IND_TYPE;
			InsertInf.item.pszText=pGameType->szTypeName;
			pListType->SetItemData((DWORD_PTR)InsertItem(&InsertInf));
			SetItemState((HTREEITEM)pListType->GetItemData(),TVIS_BOLD,TVIS_BOLD);

			break;
		}
	case ItemGenre_Kind:		//��Ϸ����
		{
			//��������
			CListKind * pListKind=(CListKind *)pListItem;
			tagGameKind * pGameKind=pListKind->GetItemInfo();

			//��ȡ��Ϣ
			TCHAR szItemTitle[128]=TEXT("");
			DWORD dwImageIndex=GetGameImageIndex(pListKind);
			GetGameItemTitle(pListKind,szItemTitle,sizeof(szItemTitle));

			//��������
			InsertInf.item.pszText=szItemTitle;
			InsertInf.item.iImage=dwImageIndex;
			InsertInf.item.iSelectedImage=dwImageIndex;
			pListKind->SetItemData((DWORD_PTR)InsertItem(&InsertInf));

			break;
		}
	case ItemGenre_Station:		//��Ϸվ��
		{
			//��������
			CListStation * pListStation=(CListStation *)pListItem;
			tagGameStation * pGameStation=pListStation->GetItemInfo();

			//��������
			InsertInf.item.iImage=IND_STATION;
			InsertInf.item.iSelectedImage=IND_STATION;
			InsertInf.item.pszText=pGameStation->szStationName;
			pListStation->SetItemData((DWORD_PTR)InsertItem(&InsertInf));

			break;
		}
	case ItemGenre_Server:		//��Ϸ����
		{
			//��������
			CListServer * pListServer=(CListServer *)pListItem;
			tagGameServer * pGameServer=pListServer->GetItemInfo();

			//����λ��
			InsertInf.hInsertAfter=TVI_FIRST;

			//Ѱ������
			if (InsertInf.hParent!=NULL)
			{
				//��������
				WORD wSortID=pGameServer->wSortID;
				HTREEITEM hTreeItem=GetChildItem(InsertInf.hParent);

				//����λ��
				if (hTreeItem!=NULL)
				{
					do
					{
						//��ȡ����
						CListServer * pItemTemp=(CListServer *)GetItemData(hTreeItem);
						if (pItemTemp!=NULL)
						{
							//��ȡλ��
							WORD wItemSortID=200;
							if (pItemTemp->GetItemGenre()==ItemGenre_Server)
							{
								wItemSortID=((CListServer *)pItemTemp)->GetItemInfo()->wSortID;
							}

							//λ���ж�
							if (wSortID>wItemSortID) InsertInf.hInsertAfter=hTreeItem;
							else break;
						}
						else InsertInf.hInsertAfter=hTreeItem;

						//�л�����
						hTreeItem=GetNextItem(hTreeItem,TVGN_NEXT);

					} while (hTreeItem!=NULL);
				}
			}

			//�������
			TCHAR szItemTitle[128]=TEXT("");
			DWORD dwImageIndex=GetGameImageIndex(pListServer);
			GetGameItemTitle(pListServer,szItemTitle,sizeof(szItemTitle));

			//��������
			InsertInf.item.pszText=szItemTitle;
			InsertInf.item.iImage=dwImageIndex;
			InsertInf.item.iSelectedImage=dwImageIndex;
			pListServer->SetItemData((DWORD_PTR)InsertItem(&InsertInf));

			break;
		}
	case ItemGenre_Inside:		//�ڲ�����
		{
			//�������
			CListInside * pListInside=(CListInside *)pListItem;
			tagGameInside * pGameInside=pListInside->GetItemInfo();

			//��������
			InsertInf.item.iImage=pGameInside->iImageIndex;
			InsertInf.item.iSelectedImage=pGameInside->iImageIndex;
			InsertInf.item.pszText=pGameInside->szDisplayName;
			pListInside->SetItemData((DWORD_PTR)InsertItem(&InsertInf));
			SetItemState((HTREEITEM)pListInside->GetItemData(),TVIS_BOLD,TVIS_BOLD);

			break;
		}
	}
	
	return;
}

//����б�
void CServerItemView::OnNMClick(NMHDR * pNMHDR, LRESULT * pResult)
{
	//��������
	CListItem * pListItem=NULL;
	HTREEITEM hTreeItem=GetCurrentTreeItem();

	//չ���б�
	if (hTreeItem!=NULL)
	{
		Select(hTreeItem,TVGN_CARET);
		pListItem=(CListItem *)GetItemData(hTreeItem);
	}

	//������Ϣ
	ASSERT(m_pITreeCtrlSink!=NULL);
	m_pITreeCtrlSink->OnTreeLeftClick(pListItem,hTreeItem,this);

	return;
}

//�һ��б�
void CServerItemView::OnNMRclick(NMHDR * pNMHDR, LRESULT * pResult)
{
	//��������
	CListItem * pListItem=NULL;
	HTREEITEM hTreeItem=GetCurrentTreeItem();

	//ѡ���б�
	if (hTreeItem!=NULL)
	{
		Select(hTreeItem,TVGN_CARET);
		pListItem=(CListItem *)GetItemData(hTreeItem);
	}

	//������Ϣ
	ASSERT(m_pITreeCtrlSink!=NULL);
	m_pITreeCtrlSink->OnTreeRightClick(pListItem,hTreeItem,this);

	return;
}

//���˫��
void CServerItemView::OnNMDblclk(NMHDR * pNMHDR, LRESULT * pResult)
{
	//��������
	CListItem * pListItem=NULL;
	HTREEITEM hTreeItem=GetCurrentTreeItem();

	//չ���б�
	if (hTreeItem!=NULL)
	{
		Select(hTreeItem,TVGN_CARET);
		pListItem=(CListItem *)GetItemData(hTreeItem);
	}

	//������Ϣ
	ASSERT(m_pITreeCtrlSink!=NULL);
	m_pITreeCtrlSink->OnTreeLeftDBClick(pListItem,hTreeItem,this);

	return;
}

//�Ҽ�˫��
void CServerItemView::OnNMRdblclk(NMHDR * pNMHDR, LRESULT * pResult)
{
	//��������
	CListItem * pListItem=NULL;
	HTREEITEM hTreeItem=GetCurrentTreeItem();

	//չ���б�
	if (hTreeItem!=NULL)
	{
		Select(hTreeItem,TVGN_CARET);
		pListItem=(CListItem *)GetItemData(hTreeItem);
	}

	//������Ϣ
	ASSERT(m_pITreeCtrlSink!=NULL);
	m_pITreeCtrlSink->OnTreeRightDBClick(pListItem,hTreeItem,this);

	return;
}

//ѡ��ı�
void CServerItemView::OnTvnSelchanged(NMHDR * pNMHDR, LRESULT * pResult)
{
	LPNMTREEVIEW pNMTreeView=reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	//��������
	CListItem * pListItem=NULL;
	HTREEITEM hTreeItem=pNMTreeView->itemNew.hItem;
	if (hTreeItem!=NULL) pListItem=(CListItem *)pNMTreeView->itemNew.lParam;

	//������Ϣ
	ASSERT(m_pITreeCtrlSink!=NULL);
	m_pITreeCtrlSink->OnTreeSelectChanged(pListItem,hTreeItem,this);

	return;
}

//�б�չ��
void CServerItemView::OnTvnItemexpanded(NMHDR * pNMHDR, LRESULT * pResult)
{
	LPNMTREEVIEW pNMTreeView=reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	//��������
	CListItem * pListItem=NULL;
	HTREEITEM hTreeItem=pNMTreeView->itemNew.hItem;
	if (hTreeItem!=NULL) pListItem=(CListItem *)pNMTreeView->itemNew.lParam;

	//������Ϣ
	ASSERT(m_pITreeCtrlSink!=NULL);
	m_pITreeCtrlSink->OnTreeItemexpanded(pListItem,hTreeItem,this);

	return;
}

//��ť����
HTREEITEM CServerItemView::GetCurrentTreeItem()
{
	TVHITTESTINFO HitTestInfo;
	memset(&HitTestInfo,0,sizeof(HitTestInfo));
	HitTestInfo.flags=TVHT_ONITEM;
	GetCursorPos(&HitTestInfo.pt);
	ScreenToClient(&HitTestInfo.pt);
	return TreeView_HitTest(m_hWnd,&HitTestInfo);
}

//��ȡͼ��
DWORD CServerItemView::GetGameImageIndex(CListKind * pListKind)
{
	//Ч�����
	ASSERT(pListKind!=NULL);
	
	//��װ�ж�
	if (pListKind->m_GameKind.dwMaxVersion==0L) return IND_KIND_UNKNOW;
	if ((pListKind->m_GameKind.dwMaxVersion!=0L)&&(pListKind->m_bInstall==false)) return IND_KIND_NODOWN;

	//��ȡͼ��
	tagGameKind * pGameKind=pListKind->GetItemInfo();
	DWORD dwImageIndex=GetGameImageIndex(pGameKind->szProcessName,pGameKind->wKindID);

	return dwImageIndex;
}

//��ȡͼ��
DWORD CServerItemView::GetGameImageIndex(CListServer * pListServer)
{
	return IND_SERVER_NORNAL;
}

//��ȡͼ��
DWORD CServerItemView::GetGameImageIndex(LPCTSTR pszProcess, WORD wKindID)
{
	//Ѱ���ִ�
	tagGameResourceInfo * pGameResourceInfo=NULL;
	for (INT_PTR i=0;i<m_GameResourceArray.GetCount();i++)
	{
		pGameResourceInfo=&m_GameResourceArray[i];
		if (pGameResourceInfo->wKindID==wKindID) return pGameResourceInfo->dwImageIndex;
	}

	//������Դ
	HINSTANCE hInstance=AfxLoadLibrary(pszProcess);
	if (hInstance==NULL) return IND_KIND_UNKNOW;
	
	//���ر�־
	CBitmap GameLogo;
	DWORD dwImagePos=0L;
	AfxSetResourceHandle(hInstance);
	if (GameLogo.LoadBitmap(TEXT("GAME_LOGO"))) dwImagePos=m_ImageList.Add(&GameLogo,RGB(255,0,255));
	AfxSetResourceHandle(GetModuleHandle(NULL));
	AfxFreeLibrary(hInstance);

	//������Ϣ
	if (dwImagePos!=0L)
	{
		tagGameResourceInfo GameResourceInfo;
		memset(&GameResourceInfo,0,sizeof(GameResourceInfo));
		GameResourceInfo.wKindID=wKindID;
		GameResourceInfo.dwImageIndex=dwImagePos;
		m_GameResourceArray.Add(GameResourceInfo);
		return GameResourceInfo.dwImageIndex;
	}
	
	return IND_KIND_UNKNOW;
}

//��ȡ����
LPCTSTR CServerItemView::GetGameItemTitle(CListKind * pListKind, LPTSTR pszTitle, WORD wBufferSize)
{
	//Ч�����
	ASSERT(pszTitle!=NULL);
	ASSERT(pListKind!=NULL);

	//���ɱ���
	tagGameKind * pGameKind=pListKind->GetItemInfo();

	//�������
	if (pGameKind->dwMaxVersion==0L)
	{
		lstrcpyn(pszTitle,pGameKind->szKindName,wBufferSize);
		return pszTitle;
	}

	//�������
	if (m_bShowOnLineCount==true)
	{
		if (pListKind->m_bInstall==true)
		{
			_snprintf(pszTitle,wBufferSize,TEXT("%s [ %ld ]"),pGameKind->szKindName,pGameKind->dwOnLineCount);
		}
		else
		{
			_snprintf(pszTitle,wBufferSize,TEXT("%s [ %ld ] ��˫�����أ�"),pGameKind->szKindName,pGameKind->dwOnLineCount);
		}
	}
	else
	{
		if (pListKind->m_bInstall==true)
		{
			_snprintf(pszTitle,wBufferSize,TEXT("%s"),pGameKind->szKindName);
		}
		else
		{
			_snprintf(pszTitle,wBufferSize,TEXT("%s ��˫�����أ�"),pGameKind->szKindName);
		}
	}

	return pszTitle;
}

//��ȡ����
LPCTSTR CServerItemView::GetGameItemTitle(CListServer * pListServer, LPTSTR pszTitle, WORD wBufferSize)
{
	//Ч�����
	ASSERT(pszTitle!=NULL);
	ASSERT(pListServer!=NULL);

	//���ɱ���
	tagGameServer * pGameServer=pListServer->GetItemInfo();
	if (m_bShowOnLineCount==true)
	{
		_snprintf(pszTitle,wBufferSize,TEXT("%s [ %ld ]"),pGameServer->szServerName,pGameServer->dwOnLineCount);
	}
	else 
	{
		_snprintf(pszTitle,wBufferSize,TEXT("%s"),pGameServer->szServerName);
	}

	return pszTitle;
}

//////////////////////////////////////////////////////////////////////////
