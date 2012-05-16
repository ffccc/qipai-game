#include "StdAfx.h"
#include "Resource.h"
#include "GlobalUnits.h"
#include "UserCompanionList.h"

//////////////////////////////////////////////////////////////////////////

//��������
#define IMAGE_FRIEND				1									//����ͼ��
#define IMAGE_DETEST				1									//���ͼ��
#define IMAGE_NORMAL				1									//İ��ͼ��
#define IMAGE_FRIEND_ITEM			5									//����ͼ��
#define IMAGE_DETEST_ITEM			5									//���ͼ��
#define IMAGE_NORMAL_ITEM			5									//İ��ͼ��

//��������
#define ITEM_SIZE					18									//����߶�
#define WN_SET_LINE_COLOR			(TV_FIRST+40)						//������ɫ

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CUserCompanionList, CTreeCtrl)
	ON_WM_CREATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CUserCompanionList::CUserCompanionList()
{
	//���ñ���
	m_hItemRoot=NULL;
	m_hItemDetest=NULL;
	m_hItemFriend=NULL;
	m_hItemNormal=NULL;

	return;
}

//��������
CUserCompanionList::~CUserCompanionList()
{
	//ɾ����ϵ
	for (INT_PTR i=0;i<m_CompanionInfoActive.GetCount();i++)
	{
		tagCompanionItem * pCompanionInfo=m_CompanionInfoActive[i];
		SafeDelete(pCompanionInfo);
	}
	for (INT_PTR i=0;i<m_CompanionInfoBuffer.GetCount();i++)
	{
		tagCompanionItem * pCompanionInfo=m_CompanionInfoBuffer[i];
		SafeDelete(pCompanionInfo);
	}
	m_CompanionInfoActive.RemoveAll();
	m_CompanionInfoBuffer.RemoveAll();
}

//����֪ͨ
bool __cdecl CUserCompanionList::OnCompanionInsert(tagCompanionItem * pCompanionInfo)
{
	//��ȡ����
	ASSERT(pCompanionInfo!=NULL);
	HTREEITEM hTreeItem=SearchCompanionInfo(NULL,pCompanionInfo);

	//�¼�����
	if (hTreeItem==NULL) InsertCompanionInfo(pCompanionInfo);
	else UpdateCompanionInfo(hTreeItem,pCompanionInfo);

	return true;
}

//����֪ͨ
bool __cdecl CUserCompanionList::OnCompanionUpdate(tagCompanionItem * pCompanionInfo)
{
	//��ȡ����
	ASSERT(pCompanionInfo!=NULL);
	HTREEITEM hTreeItem=SearchCompanionInfo(NULL,pCompanionInfo);

	//�¼�����
	if (hTreeItem==NULL) InsertCompanionInfo(pCompanionInfo);
	else
	{
		tagCompanionItem * pCurrCompanion=(tagCompanionItem *)GetItemData(hTreeItem);
		UpdateCompanionInfo(hTreeItem,pCompanionInfo);
	}

	//���½���
	Invalidate(FALSE);
	UpdateWindow();

	return true;
}

//ɾ��֪ͨ
bool __cdecl CUserCompanionList::OnCompanionDelete(tagCompanionItem * pCompanionInfo)
{
	//ɾ������
	ASSERT(pCompanionInfo!=NULL);
	DeleteCompanionInfo(pCompanionInfo);

	return true;
}

//�����ϵ
bool CUserCompanionList::InsertCompanionInfo(const tagCompanionItem * pCompanionInfo)
{
	//Ч�����
	ASSERT(pCompanionInfo!=NULL);
	if (pCompanionInfo==NULL) return false;

	//��������
	INT nImageIndex=0;
	HTREEITEM hParentTreeItem=NULL;

	//����׼��
	switch (pCompanionInfo->Companion)
	{
	case enCompanion_Friend:		//��Ϸ����
		{
			hParentTreeItem=m_hItemFriend;
			nImageIndex=IMAGE_FRIEND_ITEM;
			break;
		}
	case enCompanion_Detest:		//������
		{
			hParentTreeItem=m_hItemDetest;
			nImageIndex=IMAGE_DETEST_ITEM;
			break;
		}
	default:						//Ĭ�����
		{
			hParentTreeItem=m_hItemNormal;
			nImageIndex=IMAGE_NORMAL_ITEM;
			break;
		}
	}

	//���ɱ���
	TCHAR szTreeTitle[128]=TEXT("");
	_sntprintf(szTreeTitle,CountArray(szTreeTitle),TEXT("%s [ %ld ]"),pCompanionInfo->szAccounts,pCompanionInfo->dwGameID);

	//��������
	TV_INSERTSTRUCT InsertStruct;
	ZeroMemory(&InsertStruct,sizeof(InsertStruct));

	//������ϵ
	tagCompanionItem *pNewCompanion=CreateCompanionItem();
	if (pNewCompanion==NULL) return false;

	//���ñ���
	CopyMemory(pNewCompanion,pCompanionInfo,sizeof(tagCompanionItem));

	//�������
	InsertStruct.hInsertAfter=TVI_LAST;
	InsertStruct.hParent=hParentTreeItem;
	InsertStruct.item.iImage=nImageIndex;
	InsertStruct.item.iSelectedImage=nImageIndex;
	InsertStruct.item.pszText=szTreeTitle;
	InsertStruct.item.lParam=(LPARAM)pNewCompanion;
	InsertStruct.item.cchTextMax=CountArray(szTreeTitle);
	InsertStruct.item.mask=TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_PARAM;

	//��������
	InsertItem(&InsertStruct);

	return true;
}

//ɾ����ϵ
bool CUserCompanionList::DeleteCompanionInfo(tagCompanionItem * pCompanionInfo)
{
	//��������
	HTREEITEM hTreeItem=SearchCompanionInfo(NULL,pCompanionInfo);

	//ɾ������
	if (hTreeItem!=NULL)
	{
		//��ȡ����
		tagCompanionItem * pCompanionItemTreeData=(tagCompanionItem *)GetItemData(hTreeItem);

		//ɾ����ϵ
		if (pCompanionItemTreeData!=NULL) DeleteCompanionItem(pCompanionItemTreeData);
		
		//ɾ������
		DeleteItem(hTreeItem);
		return true;
	}

	return false;
}

//������Ϣ
bool CUserCompanionList::UpdateCompanionInfo(HTREEITEM hTreeItem, tagCompanionItem * pCompanionInfo)
{
	//Ч�����
	ASSERT(hTreeItem!=NULL);
	if (hTreeItem==NULL) return false;

	//���´���
	if (GetTreeItemCompanion(hTreeItem)==pCompanionInfo->Companion)
	{
		//���ɱ���
		TCHAR szTreeTitle[128]=TEXT("");
		_sntprintf(szTreeTitle,CountArray(szTreeTitle),TEXT("%s [ %ld ]"),pCompanionInfo->szAccounts,pCompanionInfo->dwGameID);

		//��������
		SetItemText(hTreeItem,szTreeTitle);
	}
	else
	{
		//ɾ������
		DeleteItem(hTreeItem);
		InsertCompanionInfo(pCompanionInfo);
	}
	
	return true;
}

//��ȡ��ϵ
BYTE CUserCompanionList::GetTreeItemCompanion(HTREEITEM hTreeItem)
{
	//Ч�����
	ASSERT(hTreeItem!=NULL);
	if (hTreeItem==NULL) return enCompanion_UnKnow;

	//��������
	do
	{
		//Ŀ���ж�
		if (hTreeItem==m_hItemDetest) return enCompanion_Detest;
		if (hTreeItem==m_hItemFriend) return enCompanion_Friend;
		if (hTreeItem==m_hItemNormal) return enCompanion_UnKnow;

		//��ȡ����
		hTreeItem=GetParentItem(hTreeItem);

	} while (hTreeItem!=NULL);

	//�������
	ASSERT(FALSE);

	return enCompanion_UnKnow;
}

//Ѱ������
HTREEITEM CUserCompanionList::SearchCompanionInfo(HTREEITEM hRootTreeItem, tagCompanionItem * pCompanionInfo)
{
	//��ȡ����
	if (hRootTreeItem==NULL) hRootTreeItem=m_hItemRoot;
	if (hRootTreeItem==NULL) return NULL;

	//��������
	HTREEITEM hTreeItemTemp=NULL;
	DWORD_PTR dwBindParam=(DWORD_PTR)pCompanionInfo;

	//ѭ������
	do
	{
		//���ж�
		tagCompanionItem * pRootCompanionItem=(tagCompanionItem * )GetItemData(hRootTreeItem);
		if (pRootCompanionItem!=NULL&&pRootCompanionItem->dwUserID==pCompanionInfo->dwUserID) return hRootTreeItem;

		//��������
		hTreeItemTemp=GetChildItem(hRootTreeItem);
		if (hTreeItemTemp!=NULL)
		{
			hTreeItemTemp=SearchCompanionInfo(hTreeItemTemp,pCompanionInfo);
			if (hTreeItemTemp!=NULL) return hTreeItemTemp;
		}

		//��һ����
		hRootTreeItem=GetNextItem(hRootTreeItem,TVGN_NEXT);

	} while (hRootTreeItem!=NULL);

	return NULL;
}

//������Ϣ
INT CUserCompanionList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//���ÿؼ�
	SetItemHeight(20);
	SetTextColor(RGB(0,0,0));
	SetBkColor(RGB(230,249,255));
	ModifyStyle(0,TVS_HASBUTTONS|TVS_HASLINES|TVS_SHOWSELALWAYS|TVS_TRACKSELECT);
	SendMessage(WN_SET_LINE_COLOR,0,(LPARAM)RGB(72,79,63));

	//����ͼƬ
	if (m_ImageCompanionList.GetSafeHandle()==NULL)
	{
		CBitmap ServerImage;
		ServerImage.LoadBitmap(IDB_SERVER_LIST_IMAGE);
		m_ImageCompanionList.Create(18,18,ILC_COLOR16|ILC_MASK,0,0);
		m_ImageCompanionList.Add(&ServerImage,RGB(255,0,255));
		SetImageList(&m_ImageCompanionList,LVSIL_NORMAL);
	}

	//�����б�
	m_hItemRoot=InsertItem(TEXT("���ѹ���"),0,0);
	m_hItemFriend=InsertItem(TEXT("��Ϸ����"),IMAGE_FRIEND,IMAGE_FRIEND,m_hItemRoot);
	m_hItemDetest=InsertItem(TEXT("������"),IMAGE_DETEST,IMAGE_DETEST,m_hItemRoot);
	m_hItemNormal=InsertItem(TEXT("İ����"),IMAGE_NORMAL,IMAGE_NORMAL,m_hItemRoot);

	//�����б�
	WORD wIndex=0;
	do
	{
		//��ȡ��ϵ
		const tagCompanionItem * pCompanionInfo=g_GlobalUnits.m_CompanionManager->EmunCompanionItem(wIndex++);
 
		//��ֹ�ж�
		if (pCompanionInfo==NULL) break;

		//���Ǳ���
		if(g_GlobalUnits.GetGolbalUserData().dwUserID==pCompanionInfo->dwUserID)continue;

		//��������
		InsertCompanionInfo(pCompanionInfo);

	} while (true);

	//չ���б�
	Expand(m_hItemRoot,TVE_EXPAND);
	Expand(m_hItemFriend,TVE_EXPAND);

	return 0;
}

//������ϵ
tagCompanionItem * CUserCompanionList::CreateCompanionItem()
{
	//��������
	tagCompanionItem * pCompanionInfo=NULL;

	//��ȡ����
	if (m_CompanionInfoBuffer.GetCount()>0)
	{
		INT_PTR nItemCount=m_CompanionInfoBuffer.GetCount();
		pCompanionInfo=m_CompanionInfoBuffer[nItemCount-1];
		m_CompanionInfoBuffer.RemoveAt(nItemCount-1);
	}

	//��������
	if (pCompanionInfo==NULL)
	{
		pCompanionInfo=new tagCompanionItem;
		if (pCompanionInfo==NULL) return NULL;
	}

	//��ԭ����
	ZeroMemory(pCompanionInfo,sizeof(tagCompanionItem));

	//��������
	m_CompanionInfoActive.Add(pCompanionInfo);

	return pCompanionInfo;
}

//�ͷŹ�ϵ
bool CUserCompanionList::DeleteCompanionItem(tagCompanionItem * pCompanionInfo)
{
	//��������
	for (INT_PTR i=0;i<m_CompanionInfoActive.GetCount();i++)
	{
		if (m_CompanionInfoActive[i]==pCompanionInfo)
		{
			//ɾ������
			m_CompanionInfoActive.RemoveAt(i);
			m_CompanionInfoBuffer.Add(pCompanionInfo);

			return true;
		}
	}

	//�������
	ASSERT(FALSE);

	return false;
}
//////////////////////////////////////////////////////////////////////////
