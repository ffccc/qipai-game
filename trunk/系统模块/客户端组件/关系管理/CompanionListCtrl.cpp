#include "Stdafx.h"
#include "Resource.h"
#include "CompanionListCtrl.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

#define IMAGE_FRIEND_ITEM				6					//����ͼ��
#define IMAGE_DETEST_ITEM				12					//���ͼ��

//////////////////////////////////////////////////////////////////////////
//��̬����

CImageList		CCompanionListCtrl::m_ImageList;			//ͼƬ��Դ

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CCompanionListCtrl, CSkinListCtrl)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CCompanionListCtrl::CCompanionListCtrl()
{
}

//��������
CCompanionListCtrl::~CCompanionListCtrl()
{
}

//����Ϣ
void CCompanionListCtrl::PreSubclassWindow()
{
	//���ر���ͼ
	if (m_ImageList.GetSafeHandle()==NULL)
	{
		CBitmap Image;
		Image.LoadBitmap(IDB_LIST_IMAGE);
		m_ImageList.Create(24,16,ILC_COLOR16|ILC_MASK,0,0);
		m_ImageList.Add(&Image,RGB(255,0,255));
	}
	SetImageList(&m_ImageList,LVSIL_SMALL);

	//������ɫ
	SetTextColor(RGB(10,10,10));
	SetBkColor(RGB(245,255,205));
	SetTextBkColor(RGB(245,255,205));

	//�����б�
	InsertColumn(0,TEXT("��ϵ��"),LVCFMT_LEFT,100);
	InsertColumn(1,TEXT("��Ϸ ID"),LVCFMT_LEFT,80);
	InsertColumn(2,TEXT("��ע��Ϣ"),LVCFMT_LEFT,200);

	__super::PreSubclassWindow();
}

//�����ϵ
bool CCompanionListCtrl::InsertCompanionItem(const tagCompanionItem * pCompanionItem)
{
	//Ч�����
	ASSERT(pCompanionItem!=NULL);
	if (pCompanionItem==NULL) return false;

	//����λͼ
	int nImageStation=0;
	if (pCompanionItem->Companion==enCompanion_Friend) nImageStation=IMAGE_FRIEND_ITEM;
	else if (pCompanionItem->Companion==enCompanion_Detest) nImageStation=IMAGE_DETEST_ITEM;
	else return false;

	//��������
	TCHAR szBuffer[32]=TEXT("");
	int iItem=InsertItem(LVIF_IMAGE|LVIF_TEXT|LVIF_PARAM,0,pCompanionItem->szAccounts,0,0,nImageStation,(LPARAM)pCompanionItem);
	SetItem(iItem,1,LVIF_TEXT,ltoa(pCompanionItem->dwGameID,szBuffer,10),0,0,0,0);
	SetItem(iItem,2,LVIF_TEXT,pCompanionItem->szNote,0,0,0,0);

	return true;
}

//���¹�ϵ
bool CCompanionListCtrl::UpdateCompanionItem(const tagCompanionItem * pCompanionItem)
{
	//Ч�����
	ASSERT(pCompanionItem!=NULL);
	if (pCompanionItem==NULL) return false;
	
	//���ҹ�ϵ
	LVFINDINFO FindInfo;
	memset(&FindInfo,0,sizeof(FindInfo));
	FindInfo.flags=LVFI_PARAM;
	FindInfo.lParam=(LPARAM)pCompanionItem;
	int iItem=FindItem(&FindInfo);
	if (iItem==-1) return false;

	//����λͼ
	int nImageStation=0;
	if (pCompanionItem->Companion==enCompanion_Friend) nImageStation=IMAGE_FRIEND_ITEM;
	else if (pCompanionItem->Companion==enCompanion_Detest) nImageStation=IMAGE_DETEST_ITEM;
	else return false;

	//���¹�ϵ
	TCHAR szBuffer[32]=TEXT("");
	SetItem(iItem,0,LVIF_IMAGE|LVIF_TEXT,pCompanionItem->szAccounts,nImageStation,0,0,0);
	SetItemText(iItem,1,ltoa(pCompanionItem->dwGameID,szBuffer,10));
	SetItemText(iItem,2,pCompanionItem->szNote);

	return true;
}

//ɾ����ϵ
bool CCompanionListCtrl::DeleteCompanionItem(const tagCompanionItem * pCompanionItem)
{
	//Ч�����
	ASSERT(pCompanionItem!=NULL);
	if (pCompanionItem==NULL) return false;
	
	//���ҹ�ϵ
	LVFINDINFO FindInfo;
	memset(&FindInfo,0,sizeof(FindInfo));
	FindInfo.flags=LVFI_PARAM;
	FindInfo.lParam=(LPARAM)pCompanionItem;
	int iItem=FindItem(&FindInfo);
	if (iItem!=-1)
	{
		DeleteItem(iItem);
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
