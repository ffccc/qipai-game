#include "StdAfx.h"
#include "GlobalUnits.h"
#include "PlatformFrame.h"
#include "PlatformEvent.h"
#include "ServerListView.h"

//////////////////////////////////////////////////////////////////////////////////

//��������
#define ITEM_SIZE					18									//����߶�
#define ITEM_HEIGHT					26									//����߶�

//////////////////////////////////////////////////////////////////////////////////

//ʱ�䶨��
#define IDI_UPDATE_ONLINE			100									//��������
#define TIME_UPDATE_ONLINE			600000								//��������

//////////////////////////////////////////////////////////////////////////////////

//ͼ������
#define IND_ROOT					0									//�б���
#define IND_TYPE					1									//��Ϸ����
#define IND_NODE					2									//��Ϸ�ڵ�
#define IND_KIND_NODOWN				3									//��û����
#define IND_KIND_UNKNOW				4									//δ֪����
#define IND_SERVER_UPDATE			5									//�ղط���
#define IND_SERVER_ATTACH			6									//�ղط���
#define IND_SERVER_NORMAL			7									//��������
#define IND_SERVER_ENTRANCE			8									//��ǰʹ��
#define IND_SERVER_EVERENTRANCE		9									//��������
#define IND_BROWSE					10									//�������
#define IND_FUNCTION				11									//��������

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CServerListView, CTreeCtrl)

	//ϵͳ��Ϣ
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

//���캯��
CServerListView::CServerListView()
{
	//���ñ���
	m_nXScroll=0;
	m_nYScroll=0;

	//�б���
	m_TreeListRoot=NULL;
	m_TreeAssistant=NULL;

	//��������
	m_hItemMouseHover=NULL;
	m_hTreeClickExpand=NULL;

	//�ں˱���
	m_GameImageMap.InitHashTable(PRIME_KIND);

	//������Դ
	m_ImageArrow.LoadImage(AfxGetInstanceHandle(),TEXT("SERVER_LIST_ARROW"));

	//��������
	m_FontBold.CreateFont(-12,0,0,0,600,0,0,0,134,3,2,ANTIALIASED_QUALITY,2,TEXT("����"));

	return;
}

//��������
CServerListView::~CServerListView()
{
}

//�����
BOOL CServerListView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	/*UINT nCommandID=LOWORD(wParam);

	//�˵�����
	switch (nCommandID)
	{
	case IDM_ENTER_SERVER:		//���뷿��
		{
			//��ȡ����
			HTREEITEM hTreeItem=GetSelectedItem();

			//ѡ���ж�
			if (hTreeItem==NULL) return TRUE;

			//��ȡ����
			CGameListItem * pGameListItem=(CGameListItem *)GetItemData(hTreeItem);

			//�����
			if (pGameListItem!=NULL)
			{
				switch (pGameListItem->GetItemGenre())
				{
				case ItemGenre_Kind:	//��Ϸ����
					{
						//���뷿��
						CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
						pPlatformFrame->EntranceServerItem((CGameKindItem *)pGameListItem);

						break;
					}
				case ItemGenre_Server:	//��Ϸ����
					{
						//���뷿��
						CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
						pPlatformFrame->EntranceServerItem((CGameServerItem *)pGameListItem);

						break;
					}
				}
			}

			return TRUE;
		}
	case IDM_SHRINK_LIST:		//�����б�
		{
			//�����б�
			HTREEITEM hCurrentItem=GetSelectedItem();
			if (hCurrentItem!=NULL)	Expand(hCurrentItem,TVE_COLLAPSE);

			return TRUE;
		}
	case IDM_EXPAND_LIST:		//չ���б�
		{
			//չ���б�
			HTREEITEM hCurrentItem=GetSelectedItem();
			if (hCurrentItem!=NULL)	Expand(hCurrentItem,TVE_EXPAND);

			return TRUE;
		}
	}*/

	return __super::OnCommand(wParam,lParam);
}

//���ں���
LRESULT CServerListView::DefWindowProc(UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	//˫����Ϣ
	switch (uMessage)
	{
	case WM_LBUTTONDOWN:		//�����Ϣ
		{
			//�������
			CPoint MousePoint;
			MousePoint.SetPoint(LOWORD(lParam),HIWORD(lParam));

			//���ñ���
			m_hTreeClickExpand=NULL;

			//�������
			HTREEITEM hTreeItem=HitTest(MousePoint);

			//��Ϣ����
			if ((hTreeItem!=NULL)&&(GetChildItem(hTreeItem)!=NULL))
			{
				//ѡ������
				SetFocus();
				Select(hTreeItem,TVGN_CARET);

				//��ȡλ��
				CRect rcTreeItem;
				GetItemRect(hTreeItem,&rcTreeItem,TRUE);

				//��Ϣ����
				if (rcTreeItem.PtInRect(MousePoint)==TRUE)
				{
					//չ���б�
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
	case WM_LBUTTONDBLCLK:		//�����Ϣ
		{
			//�������
			CPoint MousePoint;
			MousePoint.SetPoint(LOWORD(lParam),HIWORD(lParam));

			//�������
			HTREEITEM hTreeItem=HitTest(MousePoint);

			//�����ж�
			if (hTreeItem!=NULL)
			{
				//��ȡ����
				CGameListItem * pGameListItem=(CGameListItem *)GetItemData(hTreeItem);
				if ((pGameListItem!=NULL)&&(pGameListItem->GetItemGenre()==ItemGenre_Kind))
				{
					//��������
					CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;

					//�����ж�
					if (pGameKindItem->m_dwProcessVersion==0L)
					{
						//��ȡ�汾
						tagGameKind * pGameKind=&pGameKindItem->m_GameKind;
						CWHService::GetModuleVersion(pGameKind->szProcessName,pGameKindItem->m_dwProcessVersion);

						//�����ж�
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

			//��Ϣ����
			if ((hTreeItem!=NULL)&&(GetChildItem(hTreeItem)!=NULL))
			{
				//ѡ������
				SetFocus();
				Select(hTreeItem,TVGN_CARET);

				//λ���ж�
				CRect rcTreeItem;
				GetItemRect(hTreeItem,&rcTreeItem,TRUE);
				if (rcTreeItem.PtInRect(MousePoint)==FALSE) break;

				//չ���ж�
				if ((m_hTreeClickExpand!=hTreeItem)&&(ExpandVerdict(hTreeItem)==true))
				{
					//���ñ���
					m_hTreeClickExpand=NULL;

					//չ���б�
					Expand(hTreeItem,TVE_COLLAPSE);
				}

				return 0;
			}

			break;
		}
	}

	return __super::DefWindowProc(uMessage,wParam,lParam);
}

//����ͨ��
VOID CServerListView::InitAssistantItem()
{
	//����Ŀ¼
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//����·��
	TCHAR szAssistantPath[MAX_PATH]=TEXT("");
	_sntprintf(szAssistantPath,CountArray(szAssistantPath),TEXT("%s\\Collocate\\Collocate.INI"),szDirectory);

	//��ȡ����
	TCHAR szAssistant[512]=TEXT("");
	GetPrivateProfileString(TEXT("Assistant"),TEXT("AssistantName"),TEXT(""),szAssistant,CountArray(szAssistant),szAssistantPath);

	//��������
	if (szAssistant[0]!=0)
	{
		//��������
		DWORD dwInsideID=1;

		//��������
		UINT nMask=TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_PARAM|TVIF_STATE;
		m_TreeAssistant=InsertItem(nMask,szAssistant,IND_FUNCTION,IND_FUNCTION,TVIS_BOLD,TVIS_BOLD,NULL,NULL,TVI_LAST);

		//��������
		do
		{
			//�������
			TCHAR szItemName[16]=TEXT("");
			_sntprintf(szItemName,CountArray(szItemName),TEXT("Assistant%ld"),dwInsideID);

			//��ȡ����
			TCHAR szAssistantName[128]=TEXT("");
			GetPrivateProfileString(szItemName,TEXT("AssistantName"),TEXT(""),szAssistantName,CountArray(szAssistantName),szAssistantPath);

			//����ж�
			if (szAssistantName[0]==0) break;

			//��������
			InsertInsideItem(szAssistantName,IND_BROWSE,dwInsideID++,m_TreeAssistant);

		} while (true);

		//չ���б�
		ExpandListItem(m_TreeAssistant);
	}

	return;
}

//���ú���
VOID CServerListView::InitServerTreeView()
{
	//��������
	SetItemHeight(ITEM_HEIGHT);
	ModifyStyle(0,TVS_HASBUTTONS|TVS_HASLINES|TVS_SHOWSELALWAYS|TVS_TRACKSELECT|TVS_FULLROWSELECT|TVS_HASLINES);

	//���ر�־
	CBitmap ServerImage;
	ServerImage.LoadBitmap(IDB_SERVER_LIST_IMAGE);
	m_ImageList.Create(ITEM_SIZE,ITEM_SIZE,ILC_COLOR16|ILC_MASK,0,0);

	//������Դ
	SetImageList(&m_ImageList,LVSIL_NORMAL);
	m_ImageList.Add(&ServerImage,RGB(255,0,255));

	//��������
	m_TreeListRoot=InsertInsideItem(szProduct,IND_ROOT,0,TVI_ROOT);

	//����ͨ��
	InitAssistantItem();

	//���ù���
	m_SkinScrollBar.InitScroolBar(this);

	return;
}

//��ȡѡ��
HTREEITEM CServerListView::GetCurrentSelectItem(bool bOnlyText)
{
	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//����ж�
	HTREEITEM hTreeItem=HitTest(MousePoint);

	if (bOnlyText==true)
	{
		//��ȡλ��
		CRect rcTreeItem;
		GetItemRect(hTreeItem,&rcTreeItem,TRUE);

		//ѡ���ж�
		if (rcTreeItem.PtInRect(MousePoint)==FALSE) return NULL;
	}

	return hTreeItem;
}

//չ��״̬
bool CServerListView::ExpandVerdict(HTREEITEM hTreeItem)
{
	if (hTreeItem!=NULL)
	{
		UINT uState=GetItemState(hTreeItem,TVIS_EXPANDED);
		return ((uState&TVIS_EXPANDED)!=0);
	}

	return false;
}

//չ���б�
bool CServerListView::ExpandListItem(HTREEITEM hTreeItem)
{
	//Ч�����
	ASSERT(hTreeItem!=NULL);
	if (hTreeItem==NULL) return false;

	//չ���б�
	HTREEITEM hCurrentItem=hTreeItem;
	do
	{
		Expand(hCurrentItem,TVE_EXPAND);
		hCurrentItem=GetParentItem(hCurrentItem);
	} while (hCurrentItem!=NULL);

	return true;
}

//չ���б�
bool CServerListView::ExpandListItem(CGameListItem * pGameListItem)
{
	//Ч�����
	ASSERT(pGameListItem!=NULL);
	if (pGameListItem==NULL) return false;

	//չ������
	if (pGameListItem->m_hTreeItemAttach!=NULL) ExpandListItem(pGameListItem->m_hTreeItemAttach);
	if (pGameListItem->m_hTreeItemNormal!=NULL) ExpandListItem(pGameListItem->m_hTreeItemNormal);
	
	return true;
}

//�滭����
VOID CServerListView::DrawTreeItem(CDC * pDC, CRect & rcClient, CRect & rcClipBox)
{
	//�����ж�
	HTREEITEM hItemCurrent=GetFirstVisibleItem();
	if (hItemCurrent==NULL) return;

	//��ȡ����
	UINT uTreeStyte=GetWindowLong(m_hWnd,GWL_STYLE);

	//��ȡ����
	ASSERT(CSkinRenderManager::GetInstance()!=NULL);
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();

	//�滭����
	do
	{
		//��������
		CRect rcItem;
		CRect rcRect;

		//��ȡ״̬
		HTREEITEM hParent=GetParentItem(hItemCurrent);
		UINT uItemState=GetItemState(hItemCurrent,TVIF_STATE);

		//��ȡ����
		bool bDrawChildren=(ItemHasChildren(hItemCurrent)==TRUE);
		bool bDrawSelected=(uItemState&TVIS_SELECTED)&&((this==GetFocus())||(uTreeStyte&TVS_SHOWSELALWAYS));

		//��ȡ����
		if (GetItemRect(hItemCurrent,rcItem,TRUE))
		{
			//�滭����
			if (rcItem.top>=rcClient.bottom) break;
			if (rcItem.top>=rcClipBox.bottom) continue;

			//����λ��
			rcRect.left=0;
			rcRect.top=rcItem.top+1;
			rcRect.bottom=rcItem.bottom;
			rcRect.right=rcClient.Width();

			//�滭ѡ��
			if (bDrawSelected==true)
			{
				pDC->FillSolidRect(&rcRect,RGB(253,231,161));
			}

			//�滭����
			if ((bDrawSelected==false)&&(m_hItemMouseHover==hItemCurrent))
			{
				pDC->FillSolidRect(&rcRect,pSkinRenderManager->RenderColor(0.36,1.00));
			}

			//���Ƽ�ͷ
			if (bDrawChildren==true)
			{
				//����λ��
				INT nXPos=rcItem.left-m_ImageArrow.GetWidth()/2-25;
				INT nYPos=rcItem.top+1+(rcItem.Height()-m_ImageArrow.GetHeight())/2;

				//�滭ͼ��
				INT nIndex=(uItemState&TVIS_EXPANDED)?1L:0L;
				m_ImageArrow.DrawImage(pDC,nXPos,nYPos,m_ImageArrow.GetWidth()/2,m_ImageArrow.GetHeight(),nIndex*m_ImageArrow.GetWidth()/2,0);
			}

			//�����б�
			DrawListImage(pDC,rcItem,hItemCurrent);	

			//�����ı�
			DrawItemString(pDC,rcItem,hItemCurrent,bDrawSelected);
		}
	} while ((hItemCurrent=GetNextVisibleItem(hItemCurrent))!= NULL);

	return;
}

//�滭����
VOID CServerListView::DrawTreeBack(CDC * pDC, CRect & rcClient, CRect & rcClipBox)
{
	//�滭����
	pDC->FillSolidRect(&rcClient,RGB(255,255,255));

	//���ƺ���
	for (INT nYPos=m_nYScroll/ITEM_HEIGHT*ITEM_HEIGHT;nYPos<rcClient.Height();nYPos+=ITEM_HEIGHT)
	{
		pDC->FillSolidRect(0,nYPos,rcClient.Width(),1,RGB(223,223,223));
	}

	return;
}

//�滭ͼ��
VOID CServerListView::DrawListImage(CDC * pDC, CRect rcRect, HTREEITEM hTreeItem)
{
	//��ȡ����
	INT nImage,nSelectedImage;
	GetItemImage(hTreeItem,nImage,nSelectedImage);

	//��ȡ��Ϣ
	IMAGEINFO ImageInfo;
	m_ImageList.GetImageInfo(nImage,&ImageInfo);

	//�滭ͼ��
	INT nImageWidth=ImageInfo.rcImage.right-ImageInfo.rcImage.left;
	INT nImageHeight=ImageInfo.rcImage.bottom-ImageInfo.rcImage.top;
	m_ImageList.Draw(pDC,nImage,CPoint(rcRect.left-nImageWidth-3,rcRect.top+(rcRect.Height()-nImageHeight)/2+1),ILD_TRANSPARENT);

	return;
}

//�����ı�
VOID CServerListView::DrawItemString(CDC * pDC, CRect rcRect, HTREEITEM hTreeItem, bool bSelected)
{
	//��������
	COLORREF crString=RGB(150,150,150);
	CGameListItem * pGameListItem=(CGameListItem *)GetItemData(hTreeItem);

	//��ɫ����
	if (pGameListItem!=NULL)
	{
		//ѡ������
		switch (pGameListItem->GetItemGenre())
		{
		case ItemGenre_Type:		//��������
			{
				//������ɫ
				crString=RGB(0,0,0);

				//��������
				pDC->SelectObject(m_FontBold);

				break;
			}
		case ItemGenre_Inside:		//�ڲ�����
			{
				//������ɫ
				crString=RGB(0,0,0);

				//��������
				CGameInsideItem * pGameInsideItem=(CGameInsideItem *)pGameListItem;
				pDC->SelectObject((pGameInsideItem->m_dwInsideID==0)?m_FontBold:CSkinResourceManager::GetInstance()->GetDefaultFont());

				break;
			}
		default:					//��������
			{
				//������ɫ
				crString=RGB(0,0,0);

				//��������
				pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());
			}
		}
	}
	else
	{
		//������ɫ
		crString=RGB(0,0,0);

		//��������
		pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());
	}

	//���û���
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(crString);

	//�滭����
	CString strString=GetItemText(hTreeItem);
	pDC->DrawText(strString,rcRect,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	return;
}

//��ȡ֪ͨ
VOID CServerListView::OnGameItemFinish()
{
	//��������
	POSITION Position=NULL;
	CServerListData * pServerListData=CServerListData::GetInstance();

	//չ���б�
	for (DWORD i=0;i<pServerListData->GetGameTypeCount();i++)
	{
		CGameTypeItem * pGameTypeItem=pServerListData->EmunGameTypeItem(Position);
		if (pGameTypeItem!=NULL) ExpandListItem(pGameTypeItem);
	}

	//չ���б�
	if (m_TreeListRoot!=NULL) ExpandListItem(m_TreeListRoot);

	//��֤����
	EnsureVisible(m_TreeListRoot);

	//��������
	SetTimer(IDI_UPDATE_ONLINE,TIME_UPDATE_ONLINE,NULL);

	return;
}

//��ȡ֪ͨ
VOID CServerListView::OnGameKindFinish(WORD wKindID)
{
	//��ȡ����
	CServerListData * pServerListData=CServerListData::GetInstance();
	CGameKindItem * pGameKindItem=pServerListData->SearchGameKind(wKindID);

	//���ʹ���
	if (pGameKindItem!=NULL)
	{
		//��������
		LPCTSTR pszTitle=TEXT("û�п�����Ϸ����");
		HTREEITEM hItemAttachUpdate=pGameKindItem->m_hItemAttachUpdate;
		HTREEITEM hItemNormalUpdate=pGameKindItem->m_hItemNormalUpdate;

		//���±���
		if (hItemAttachUpdate!=NULL) SetItemText(hItemAttachUpdate,pszTitle);
		if (hItemNormalUpdate!=NULL) SetItemText(hItemNormalUpdate,pszTitle);
	}

	return;
}

//����֪ͨ
VOID CServerListView::OnGameItemInsert(CGameListItem * pGameListItem)
{
	//Ч�����
	ASSERT(pGameListItem!=NULL);
	if (pGameListItem==NULL) return;

	//���봦��
	switch (pGameListItem->GetItemGenre())
	{
	case ItemGenre_Type:	//��Ϸ����
		{
			//��������
			CGameTypeItem * pGameTypeItem=(CGameTypeItem *)pGameListItem;
			CGameListItem * pParentListItem=pGameListItem->GetParentListItem();

			//���Զ���
			LPCTSTR pszTypeName=pGameTypeItem->m_GameType.szTypeName;

			//��������
			if (pParentListItem!=NULL)
			{
				//�������
				HTREEITEM hTreeItemNormal=pParentListItem->m_hTreeItemNormal;
				if (hTreeItemNormal!=NULL) pGameTypeItem->m_hTreeItemNormal=InsertGameListItem(pszTypeName,IND_TYPE,pGameTypeItem,hTreeItemNormal);

				//ϲ������
				HTREEITEM hTreeItemAttach=pParentListItem->m_hTreeItemAttach;
				if (hTreeItemAttach!=NULL) pGameTypeItem->m_hTreeItemAttach=InsertGameListItem(pszTypeName,IND_TYPE,pGameTypeItem,hTreeItemAttach);
			}
			else
			{
				//�������
				HTREEITEM hTreeItemNormal=m_TreeListRoot;
				pGameTypeItem->m_hTreeItemNormal=InsertGameListItem(pszTypeName,IND_TYPE,pGameTypeItem,hTreeItemNormal);
			}

			//����״̬
			if (pGameTypeItem->m_hTreeItemNormal!=NULL) SetItemState(pGameTypeItem->m_hTreeItemNormal,TVIS_BOLD,TVIS_BOLD);
			if (pGameTypeItem->m_hTreeItemAttach!=NULL) SetItemState(pGameTypeItem->m_hTreeItemAttach,TVIS_BOLD,TVIS_BOLD);

			break;
		}
	case ItemGenre_Kind:	//��Ϸ����
		{
			//��������
			CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;
			CGameListItem * pParentListItem=pGameListItem->GetParentListItem();

			//��������
			if (pParentListItem!=NULL)
			{
				//��������
				UINT nUpdateImage=IND_SERVER_UPDATE;
				UINT uNormalImage=GetGameImageIndex(pGameKindItem);

				//���봦��
				TCHAR szTitle[64]=TEXT("");
				LPCTSTR pszUpdateName=TEXT("�������ط����б�...");
				GetGameItemTitle(pGameKindItem,szTitle,CountArray(szTitle));

				//�������
				if (pParentListItem->m_hTreeItemNormal!=NULL)
				{
					HTREEITEM hTreeItemNormal=pParentListItem->m_hTreeItemNormal;
					pGameKindItem->m_hTreeItemNormal=InsertGameListItem(szTitle,uNormalImage,pGameKindItem,hTreeItemNormal);
					pGameKindItem->m_hItemNormalUpdate=InsertGameListItem(pszUpdateName,nUpdateImage,NULL,pGameKindItem->m_hTreeItemNormal);
				}

				//ϲ������
				if (pParentListItem->m_hTreeItemAttach!=NULL)
				{
					HTREEITEM hTreeItemAttach=pParentListItem->m_hTreeItemAttach;
					pGameKindItem->m_hTreeItemAttach=InsertGameListItem(szTitle,uNormalImage,pGameKindItem,hTreeItemAttach);
					pGameKindItem->m_hItemAttachUpdate=InsertGameListItem(pszUpdateName,nUpdateImage,NULL,pGameKindItem->m_hTreeItemAttach);
				}
			}

			break;
		}
	case ItemGenre_Node:	//��Ϸ�ڵ�
		{
			//��������
			CGameNodeItem * pGameNodeItem=(CGameNodeItem *)pGameListItem;
			CGameListItem * pParentListItem=pGameListItem->GetParentListItem();

			//��������
			if (pParentListItem!=NULL)
			{
				//��������
				LPCTSTR pszNodeName=pGameNodeItem->m_GameNode.szNodeName;

				//ɾ������
				DeleteUpdateItem(pGameNodeItem->GetParentListItem());

				//�������
				HTREEITEM hTreeItemNormal=pParentListItem->m_hTreeItemNormal;
				if (hTreeItemNormal!=NULL) pGameNodeItem->m_hTreeItemNormal=InsertGameListItem(pszNodeName,IND_NODE,pGameNodeItem,hTreeItemNormal);

				//ϲ������
				HTREEITEM hTreeItemAttach=pParentListItem->m_hTreeItemAttach;
				if (hTreeItemAttach!=NULL) pGameNodeItem->m_hTreeItemAttach=InsertGameListItem(pszNodeName,IND_NODE,pGameNodeItem,hTreeItemAttach);
			}

			break;
		}
	case ItemGenre_Page:	//��������
		{
			//��������
			CGameListItem * pParentListItem=pGameListItem->GetParentListItem();
			LPCTSTR pszDisplayName=(((CGamePageItem *)pGameListItem)->m_GamePage.szDisplayName);

			//��������
			if (pParentListItem!=NULL)
			{
				//�������
				HTREEITEM hTreeItemNormal=pParentListItem->m_hTreeItemNormal;
				if (hTreeItemNormal!=NULL) pParentListItem->m_hTreeItemNormal=InsertGameListItem(pszDisplayName,IND_BROWSE,pParentListItem,hTreeItemNormal);

				//ϲ������
				HTREEITEM hTreeItemAttach=pParentListItem->m_hTreeItemAttach;
				if (hTreeItemAttach!=NULL) pParentListItem->m_hTreeItemAttach=InsertGameListItem(pszDisplayName,IND_BROWSE,pParentListItem,hTreeItemAttach);
			}
			else
			{
				//�������
				HTREEITEM hTreeItemNormal=m_TreeListRoot;
				pGameListItem->m_hTreeItemNormal=InsertGameListItem(pszDisplayName,IND_BROWSE,pParentListItem,hTreeItemNormal);
			}

			break;
		}
	case ItemGenre_Server:	//��Ϸ����
		{
			//��������
			CGameListItem * pParentListItem=pGameListItem->GetParentListItem();
			CGameServerItem * pGameServerItem=(CGameServerItem *)pGameListItem;

			//��������
			if (pParentListItem!=NULL)
			{
				//���봦��
				TCHAR szTitle[64]=TEXT("");
				UINT uImageIndex=GetGameImageIndex(pGameServerItem);
				GetGameItemTitle(pGameServerItem,szTitle,CountArray(szTitle));

				//ɾ������
				DeleteUpdateItem(pGameServerItem->GetParentListItem());

				//�������
				HTREEITEM hTreeItemNormal=pParentListItem->m_hTreeItemNormal;
				if (hTreeItemNormal!=NULL) pGameServerItem->m_hTreeItemNormal=InsertGameListItem(szTitle,uImageIndex,pGameServerItem,hTreeItemNormal);

				//ϲ������
				HTREEITEM hTreeItemAttach=pParentListItem->m_hTreeItemAttach;
				if (hTreeItemAttach!=NULL) pGameServerItem->m_hTreeItemAttach=InsertGameListItem(szTitle,uImageIndex,pGameServerItem,hTreeItemAttach);
			}

			break;
		}
	}

	return;
}

//����֪ͨ
VOID CServerListView::OnGameItemUpdate(CGameListItem * pGameListItem)
{
	//Ч�����
	ASSERT(pGameListItem!=NULL);
	if (pGameListItem==NULL) return;

	//���봦��
	switch (pGameListItem->GetItemGenre())
	{
	case ItemGenre_Type:	//��Ϸ����
		{
			//��������
			CGameTypeItem * pGameTypeItem=(CGameTypeItem *)pGameListItem;

			//��������
			if (pGameTypeItem->m_hTreeItemAttach!=NULL)
			{
				LPCTSTR pszTypeName(pGameTypeItem->m_GameType.szTypeName);
				ModifyGameListItem(pGameTypeItem->m_hTreeItemAttach,pszTypeName,IND_TYPE);
			}

			//��������
			if (pGameTypeItem->m_hTreeItemNormal!=NULL)
			{
				LPCTSTR pszTypeName(pGameTypeItem->m_GameType.szTypeName);
				ModifyGameListItem(pGameTypeItem->m_hTreeItemNormal,pszTypeName,IND_TYPE);
			}

			break;
		}
	case ItemGenre_Kind:	//��Ϸ����
		{
			//��������
			CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;

			//��������
			TCHAR szTitle[64]=TEXT("");
			UINT uNormalImage=GetGameImageIndex(pGameKindItem);
			GetGameItemTitle(pGameKindItem,szTitle,CountArray(szTitle));

			//��������
			if (pGameKindItem->m_hTreeItemAttach!=NULL)
			{
				ModifyGameListItem(pGameKindItem->m_hTreeItemAttach,szTitle,uNormalImage);
			}

			//��������
			if (pGameKindItem->m_hTreeItemNormal!=NULL)
			{
				ModifyGameListItem(pGameKindItem->m_hTreeItemNormal,szTitle,uNormalImage);
			}

			break;
		}
	case ItemGenre_Node:	//��Ϸ�ڵ�
		{
			//��������
			CGameNodeItem * pGameNodeItem=(CGameNodeItem *)pGameListItem;

			//��������
			if (pGameNodeItem->m_hTreeItemAttach!=NULL)
			{
				LPCTSTR pszNodeName(pGameNodeItem->m_GameNode.szNodeName);
				ModifyGameListItem(pGameNodeItem->m_hTreeItemAttach,pszNodeName,IND_NODE);
			}

			//��������
			if (pGameNodeItem->m_hTreeItemNormal!=NULL)
			{
				LPCTSTR pszNodeName(pGameNodeItem->m_GameNode.szNodeName);
				ModifyGameListItem(pGameNodeItem->m_hTreeItemNormal,pszNodeName,IND_NODE);
			}

			break;
		}
	case ItemGenre_Page:	//��������
		{
			//��������
			CGamePageItem * pGamePageItem=(CGamePageItem *)pGameListItem;

			//��������
			if (pGamePageItem->m_hTreeItemAttach!=NULL)
			{
				LPCTSTR pszDisplayName(pGamePageItem->m_GamePage.szDisplayName);
				ModifyGameListItem(pGamePageItem->m_hTreeItemAttach,pszDisplayName,IND_BROWSE);
			}

			//��������
			if (pGamePageItem->m_hTreeItemNormal!=NULL)
			{
				LPCTSTR pszDisplayName(pGamePageItem->m_GamePage.szDisplayName);
				ModifyGameListItem(pGamePageItem->m_hTreeItemNormal,pszDisplayName,IND_BROWSE);
			}

			break;
		}
	case ItemGenre_Server:	//��Ϸ����
		{
			//��������
			CGameServerItem * pGameServerItem=(CGameServerItem *)pGameListItem;

			//��������
			TCHAR szTitle[64]=TEXT("");
			UINT uImageIndex=GetGameImageIndex(pGameServerItem);
			GetGameItemTitle(pGameServerItem,szTitle,CountArray(szTitle));

			//��������
			if (pGameServerItem->m_hTreeItemAttach!=NULL)
			{
				ModifyGameListItem(pGameServerItem->m_hTreeItemAttach,szTitle,uImageIndex);
			}

			//��������
			if (pGameServerItem->m_hTreeItemNormal!=NULL)
			{
				ModifyGameListItem(pGameServerItem->m_hTreeItemNormal,szTitle,uImageIndex);
			}

			break;
		}
	}

	return;
}

//ɾ��֪ͨ
VOID CServerListView::OnGameItemDelete(CGameListItem * pGameListItem)
{
	//Ч�����
	ASSERT(pGameListItem!=NULL);
	if (pGameListItem==NULL) return;

	//ɾ������
	if (pGameListItem->m_hTreeItemAttach!=NULL) DeleteItem(pGameListItem->m_hTreeItemAttach);
	if (pGameListItem->m_hTreeItemNormal!=NULL) DeleteItem(pGameListItem->m_hTreeItemNormal);

	return;
}

//��ȡͼ��
UINT CServerListView::GetGameImageIndex(CGameKindItem * pGameKindItem)
{
	//��װ�ж�
	if (pGameKindItem->m_dwProcessVersion==0L) return IND_KIND_NODOWN;

	//Ѱ���ִ�
	UINT uImageIndxe=0;
	tagGameKind * pGameKind=&pGameKindItem->m_GameKind;
	if (m_GameImageMap.Lookup(pGameKind->wKindID,uImageIndxe)==TRUE) return uImageIndxe;

	//����ͼ��
	if (pGameKindItem->m_dwProcessVersion!=0L)
	{
		//������Դ
		LPCTSTR strProcessName(pGameKind->szProcessName);
		HINSTANCE hInstance=AfxLoadLibrary(strProcessName);

		//����ͼ��
		CBitmap GameLogo;
		AfxSetResourceHandle(hInstance);
		if (GameLogo.LoadBitmap(TEXT("GAME_LOGO"))) uImageIndxe=m_ImageList.Add(&GameLogo,RGB(255,0,255));
		AfxSetResourceHandle(GetModuleHandle(NULL));

		//�ͷ���Դ
		AfxFreeLibrary(hInstance);

		//������Ϣ
		if (uImageIndxe!=0L)
		{
			m_GameImageMap[pGameKind->wKindID]=uImageIndxe;
			return uImageIndxe;
		}
	}
	
	return IND_KIND_NODOWN;
}

//��ȡͼ��
UINT CServerListView::GetGameImageIndex(CGameServerItem * pGameServerItem)
{
	//��ȡͼ��
	if (pGameServerItem->m_ServerStatus==ServerStatus_Normal) return IND_SERVER_NORMAL;
	if (pGameServerItem->m_ServerStatus==ServerStatus_Entrance) return IND_SERVER_ENTRANCE;
	if (pGameServerItem->m_ServerStatus==ServerStatus_EverEntrance) return IND_SERVER_EVERENTRANCE;

	return IND_SERVER_NORMAL;
}

//��ȡ����
LPCTSTR CServerListView::GetGameItemTitle(CGameKindItem * pGameKindItem, LPTSTR pszTitle, UINT uMaxCount)
{
	//��������
	LPCTSTR pszKindName(pGameKindItem->m_GameKind.szKindName);
	DWORD dwOnLineCount=pGameKindItem->m_GameKind.dwOnLineCount;

	//�������
	if (pGameKindItem->m_dwProcessVersion!=0L)
	{
		_sntprintf(pszTitle,uMaxCount,TEXT("%s [ %ld ]"),(LPCTSTR)pszKindName,dwOnLineCount);
	}
	else
	{
		_sntprintf(pszTitle,uMaxCount,TEXT("%s [ %ld ] ��˫�����أ�"),pszKindName,dwOnLineCount);
	}

	return pszTitle; 
}

//��ȡ����
LPCTSTR CServerListView::GetGameItemTitle(CGameServerItem * pGameServerItem, LPTSTR pszTitle, UINT uMaxCount)
{
	//��������
	LPCTSTR pszServerName(pGameServerItem->m_GameServer.szServerName);
	DWORD dwOnLineCount=pGameServerItem->m_GameServer.dwOnLineCount;

	//�������
	_sntprintf(pszTitle,uMaxCount,TEXT("%s [ %ld ]"),pszServerName,dwOnLineCount);

	return pszTitle; 
}

//ɾ������
VOID CServerListView::DeleteUpdateItem(CGameListItem * pGameListItem)
{
	//Ч�����
	ASSERT(pGameListItem!=NULL);
	if (pGameListItem==NULL) return;

	//ɾ������
	while (pGameListItem!=NULL)
	{
		//�����ж�
		if (pGameListItem->GetItemGenre()==ItemGenre_Kind)
		{
			//ɾ������
			CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;
			if (pGameKindItem->m_hItemAttachUpdate!=NULL) DeleteItem(pGameKindItem->m_hItemAttachUpdate);
			if (pGameKindItem->m_hItemNormalUpdate!=NULL) DeleteItem(pGameKindItem->m_hItemNormalUpdate);

			//���ñ���
			pGameKindItem->m_hItemAttachUpdate=NULL;
			pGameKindItem->m_hItemNormalUpdate=NULL;

			break;
		}

		//��ȡ����
		pGameListItem=pGameListItem->GetParentListItem();
	}

	return;
}

//�޸�����
VOID CServerListView::ModifyGameListItem(HTREEITEM hTreeItem, LPCTSTR pszTitle, UINT uImage)
{
	//��������
	TVITEM TVItem;
	ZeroMemory(&TVItem,sizeof(TVItem));

	//��������
	TVItem.hItem=hTreeItem;
	TVItem.cchTextMax=64;
	TVItem.iImage=uImage;
	TVItem.iSelectedImage=uImage;
	TVItem.pszText=(LPTSTR)pszTitle;
	TVItem.mask=TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT;

	//�޸�����
	SetItem(&TVItem);

	return;
}

//��������
HTREEITEM CServerListView::InsertInsideItem(LPCTSTR pszTitle, UINT uImage, DWORD dwInsideID, HTREEITEM hParentItem)
{
	//��������
	TV_INSERTSTRUCT InsertStruct;
	ZeroMemory(&InsertStruct,sizeof(InsertStruct));

	//��������
	CGameInsideItem * pGameInsideItem=new CGameInsideItem;
	if (pGameInsideItem!=NULL) pGameInsideItem->m_dwInsideID=dwInsideID;

	//���ñ���
	InsertStruct.hParent=hParentItem;
	InsertStruct.hInsertAfter=TVI_LAST;

	//��������
	InsertStruct.item.cchTextMax=64;
	InsertStruct.item.iImage=uImage;
	InsertStruct.item.iSelectedImage=uImage;
	InsertStruct.item.pszText=(LPTSTR)pszTitle;
	InsertStruct.item.lParam=(LPARAM)pGameInsideItem;
	InsertStruct.item.mask=TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_PARAM;

	return InsertItem(&InsertStruct);
}

//��������
HTREEITEM CServerListView::InsertGameListItem(LPCTSTR pszTitle, UINT uImage, CGameListItem * pGameListItem, HTREEITEM hParentItem)
{
	//��������
	TV_INSERTSTRUCT InsertStruct;
	ZeroMemory(&InsertStruct,sizeof(InsertStruct));

	//���ñ���
	InsertStruct.hParent=hParentItem;
	InsertStruct.hInsertAfter=TVI_FIRST;
	InsertStruct.item.cchTextMax=64;
	InsertStruct.item.iImage=uImage;
	InsertStruct.item.iSelectedImage=uImage;
	InsertStruct.item.pszText=(LPTSTR)pszTitle;
	InsertStruct.item.lParam=(LPARAM)pGameListItem;
	InsertStruct.item.mask=TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_PARAM;

	//Ѱ������
	if ((hParentItem!=NULL)&&(pGameListItem!=NULL))
	{
		//��������
		WORD wSortID=pGameListItem->GetSortID();
		HTREEITEM hTreeItem=GetChildItem(hParentItem);

		//ö������
		while (hTreeItem!=NULL)
		{
			//��ȡ����
			CGameListItem * pGameListTemp=(CGameListItem *)GetItemData(hTreeItem);

			//���ݶԱ�
			if ((pGameListTemp!=NULL)&&(pGameListTemp->GetSortID()>wSortID)) break;

			//���ý��
			InsertStruct.hInsertAfter=hTreeItem;

			//�л�����
			hTreeItem=GetNextItem(hTreeItem,TVGN_NEXT);
		} 
	}

	return InsertItem(&InsertStruct);
}

//�ػ���Ϣ
VOID CServerListView::OnPaint()
{
	CPaintDC dc(this);

	//����λ��
	CRect rcClip;
	dc.GetClipBox(&rcClip);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CDC BufferDC;
	CBitmap BufferImage;
	BufferDC.CreateCompatibleDC(&dc);
	BufferImage.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());

	//���� DC
	BufferDC.SelectObject(&BufferImage);

	//�滭�ؼ�
	DrawTreeBack(&BufferDC,rcClient,rcClip);
	DrawTreeItem(&BufferDC,rcClient,rcClip);

	//�滭����
	dc.BitBlt(rcClip.left,rcClip.top,rcClip.Width(),rcClip.Height(),&BufferDC,rcClip.left,rcClip.top,SRCCOPY);

	//ɾ����Դ
	BufferDC.DeleteDC();
	BufferImage.DeleteObject();

	return;
}

//ʱ����Ϣ
VOID CServerListView::OnTimer(UINT nIDEvent)
{
	//��������
	if (nIDEvent==IDI_UPDATE_ONLINE)
	{
		ASSERT(CMissionList::GetInstance()!=NULL);
		CMissionList::GetInstance()->UpdateOnLineInfo();

		return;
	}

	__super::OnTimer(nIDEvent);
}

//�滭����
BOOL CServerListView::OnEraseBkgnd(CDC * pDC)
{
	return TRUE;
}

//λ����Ϣ
VOID CServerListView::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//��ȡ��С
	CRect rcClient;
	GetClientRect(&rcClient);

	//��ȡ��Ϣ
	SCROLLINFO ScrollInfoH;
	SCROLLINFO ScrollInfoV;
	ZeroMemory(&ScrollInfoH,sizeof(ScrollInfoH));
	ZeroMemory(&ScrollInfoV,sizeof(ScrollInfoV));

	//��ȡ��Ϣ
	GetScrollInfo(SB_HORZ,&ScrollInfoH,SIF_POS|SIF_RANGE);
	GetScrollInfo(SB_VERT,&ScrollInfoV,SIF_POS|SIF_RANGE);

	//���ñ���
	m_nXScroll=-ScrollInfoH.nPos;
	m_nYScroll=-ScrollInfoV.nPos;

	return;
}

//�����Ϣ
BOOL CServerListView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//�������
	HTREEITEM hItemMouseHover=HitTest(MousePoint);

	//�ػ��ж�
	if ((hItemMouseHover!=NULL)&&(hItemMouseHover!=m_hItemMouseHover))
	{
		//���ñ���
		m_hItemMouseHover=hItemMouseHover;

		//�ػ�����
		Invalidate(FALSE);
	}

	//���ù��
	if (hItemMouseHover!=NULL)
	{
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_HAND_CUR)));
		return true;
	}

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//�Ҽ��б�
VOID CServerListView::OnNMRClick(NMHDR * pNMHDR, LRESULT * pResult)
{
	//��ȡѡ��
	HTREEITEM hTreeItem=GetCurrentSelectItem(false);

	//ѡ���ж�
	if (hTreeItem==NULL) return;

	//����ѡ��
	Select(hTreeItem,TVGN_CARET);

	//��ȡ����
/*	CGameListItem * pGameListItem=(CGameListItem *)GetItemData(hTreeItem);
	if (pGameListItem!=NULL)
	{
		switch (pGameListItem->GetItemGenre())
		{
		case ItemGenre_Kind:	//��Ϸ����
			{
				//��������
				TCHAR szBuffer[64]=TEXT("");
				CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;

				//����˵�
				CSkinMenu Menu;
				Menu.CreateMenu();

				//�Զ�����
				Menu.AppendMenu(IDM_ENTER_SERVER,TEXT("�Զ�����"));
				Menu.AppendSeparator();

				//������Ϸ
				CW2CT pszKindName(pGameKindItem->m_GameKind.szKindName);
				_sntprintf(szBuffer,CountArray(szBuffer),TEXT("���ء�%s��"),(LPCTSTR)pszKindName);
				Menu.AppendMenu(IDM_DOWN_LOAD_CLIENT,szBuffer);

				//��ǰ�汾
				if (pGameKindItem->m_dwProcessVersion!=0)
				{
					DWORD dwVersion=pGameKindItem->m_dwProcessVersion;
					_sntprintf(szBuffer,CountArray(szBuffer),TEXT("��װ�汾 %d.%d.%d.%d"),GetProductVer(dwVersion),
						GetMainVer(dwVersion),GetSubVer(dwVersion),GetBuildVer(dwVersion));
					Menu.AppendMenu(IDM_NULL_COMMAND,szBuffer,MF_GRAYED);
				}
				else Menu.AppendMenu(IDM_DOWN_LOAD_CLIENT,TEXT("û�а�װ��������أ�"));

				//���Ʋ˵�
				Menu.AppendSeparator();
				bool bExpand=ExpandVerdict(hTreeItem);
				Menu.AppendMenu(bExpand?IDM_SHRINK_LIST:IDM_EXPAND_LIST,bExpand?TEXT("�����б�"):TEXT("չ���б�"));

				//�����˵�
				Menu.TrackPopupMenu(this);

				return;
			}
		case ItemGenre_Server:	//��Ϸ����
			{
				//��������
				TCHAR szBuffer[64]=TEXT("");
				CGameServerItem * pGameServerItem=(CGameServerItem *)pGameListItem;
				CGameKindItem * pGameKindItem=pGameServerItem->m_pGameKindItem;

				//����˵�
				CSkinMenu Menu;
				Menu.CreateMenu();
				Menu.AppendMenu(IDM_ENTER_SERVER,TEXT("������Ϸ����"));
				Menu.AppendSeparator();
				Menu.AppendMenu(IDM_SET_COLLECTION,TEXT("��Ϊ���÷�����"));

				//������Ϸ
				CW2CT pszKindName(pGameKindItem->m_GameKind.szKindName);
				_sntprintf(szBuffer,CountArray(szBuffer),TEXT("���ء�%s��"),(LPCTSTR)pszKindName);
				Menu.AppendMenu(IDM_DOWN_LOAD_CLIENT,szBuffer);
				Menu.AppendSeparator();

				//��ǰ�汾
				if (pGameKindItem->m_dwProcessVersion!=0)
				{
					DWORD dwVersion=pGameKindItem->m_dwProcessVersion;
					_sntprintf(szBuffer,CountArray(szBuffer),TEXT("��װ�汾 %d.%d.%d.%d"),GetProductVer(dwVersion),
						GetMainVer(dwVersion),GetSubVer(dwVersion),GetBuildVer(dwVersion));
					Menu.AppendMenu(IDM_NULL_COMMAND,szBuffer,MF_GRAYED);
				}
				else Menu.AppendMenu(IDM_DOWN_LOAD_CLIENT,TEXT("û�а�װ��������أ�"));

				//�˵�����
				bool Collection=false;//pGameServerItem->IsCollection();
				if (Collection==true) Menu.CheckMenuItem(IDM_SET_COLLECTION,MF_BYCOMMAND|MF_CHECKED);

				//�����˵�
				Menu.TrackPopupMenu(this);

				return;
			}
		}
	}*/

	return;
}

//����б�
VOID CServerListView::OnNMLClick(NMHDR * pNMHDR, LRESULT * pResult)
{
	//��ȡѡ��
	HTREEITEM hTreeItem=GetCurrentSelectItem(false);

	//ѡ���ж�
	if (hTreeItem==NULL) return;

	//����ѡ��
	Select(hTreeItem,TVGN_CARET);

	//��ȡ����
	CGameListItem * pGameListItem=(CGameListItem *)GetItemData(hTreeItem);

	//���뷿��
	if ((pGameListItem!=NULL)&&(pGameListItem->GetItemGenre()==ItemGenre_Server))
	{
		//���뷿��
		CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
		pPlatformFrame->EntranceServerItem((CGameServerItem *)pGameListItem);

		return;
	}

	//�ڲ�����
	if ((pGameListItem!=NULL)&&(pGameListItem->GetItemGenre()==ItemGenre_Inside))
	{
		//��������
		CGameInsideItem * pGameInsideItem=(CGameInsideItem *)pGameListItem;

		//����Ŀ¼
		TCHAR szDirectory[MAX_PATH]=TEXT("");
		CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

		//����·��
		TCHAR szAssistantPath[MAX_PATH]=TEXT("");
		_sntprintf(szAssistantPath,CountArray(szAssistantPath),TEXT("%s\\Collocate\\Collocate.INI"),szDirectory);

		//��ȡ��ַ
		TCHAR szItemName[128]=TEXT(""),szAssistantLink[128]=TEXT("");
		_sntprintf(szItemName,CountArray(szItemName),TEXT("Assistant%ld"),pGameInsideItem->m_dwInsideID);
		GetPrivateProfileString(szItemName,TEXT("AssistantLink"),TEXT(""),szAssistantLink,CountArray(szAssistantLink),szAssistantPath);

		//���ӵ�ַ
		if (szAssistantLink[0]!=0)
		{
			CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
			if (pPlatformFrame!=NULL) pPlatformFrame->WebBrowse(szAssistantLink,false);
		}

		return;
	}

	return;
}

//�б�ı�
VOID CServerListView::OnTvnSelchanged(NMHDR * pNMHDR, LRESULT * pResult)
{
	//��ȡѡ��
	HTREEITEM hTreeItem=GetSelectedItem();

	//ѡ���ж�
	if (hTreeItem==NULL) return;

	//��ȡ����
	CGameListItem * pGameListItem=(CGameListItem *)GetItemData(hTreeItem);

	//���ݴ���
	if (pGameListItem!=NULL)
	{
		switch (pGameListItem->GetItemGenre())
		{
		case ItemGenre_Kind:	//��Ϸ����
			{
				//��������
				WORD wGameID=((CGameKindItem *)pGameListItem)->m_GameKind.wGameID;

				//�����ַ
				TCHAR szRuleLink[MAX_PATH]=TEXT("");
				_sntprintf(szRuleLink,CountArray(szRuleLink),TEXT("%s/GameRule.asp?GameID=%ld"),szPlatformLink,wGameID);

				//��ҳ��
				CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
				if (pPlatformFrame!=NULL) pPlatformFrame->WebBrowse(szRuleLink,false);

				return;
			}
		case ItemGenre_Page:	//��������
			{
				//��������
				WORD wPageID=((CGamePageItem *)pGameListItem)->m_GamePage.wPageID;

				//�����ַ
				TCHAR szPageLink[MAX_PATH]=TEXT("");
				_sntprintf(szPageLink,CountArray(szPageLink),TEXT("%s/GamePage.asp?PageID=%ld"),szPlatformLink,wPageID);

				//��ҳ��
				CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
				if (pPlatformFrame!=NULL) pPlatformFrame->WebBrowse(szPageLink,false);

				return;
			}
		}
	}

	return;
}

//�б�չ��
VOID CServerListView::OnTvnItemexpanding(NMHDR * pNMHDR, LRESULT * pResult)
{
	//��������
	LPNMTREEVIEW pNMTreeView=reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	//��������
	if (pNMTreeView->action==TVE_EXPAND)
	{
		//��ȡ����
		CGameListItem * pGameListItem=(CGameListItem *)GetItemData(pNMTreeView->itemNew.hItem);

		//�����
		if ((pGameListItem!=NULL)&&(pGameListItem->GetItemGenre()==ItemGenre_Kind))
		{
			//��������
			DWORD dwNowTime=(DWORD)time(NULL);
			CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;
			bool bTimeOut=(dwNowTime>=(pGameKindItem->m_dwUpdateTime+30L));

			//�����ж�
			if ((pGameKindItem->m_bUpdateItem==false)||(bTimeOut==true))
			{
				//�����б�
				pGameKindItem->m_bUpdateItem=true;
				pGameKindItem->m_dwUpdateTime=(DWORD)time(NULL);

				//��������
				CMissionList * pMissionList=CMissionList::GetInstance();
				if (pMissionList!=NULL) pMissionList->UpdateServerInfo(pGameKindItem->m_GameKind.wKindID);
			}

			return;
		}
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
