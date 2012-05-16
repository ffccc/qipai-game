#include "Stdafx.h"
#include "Resource.h"
#include "DlgCompanionManager.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgCompanionManager, CSkinDialogEx)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CDlgCompanionManager::CDlgCompanionManager() : CSkinDialogEx(IDD_COMPANION_MANAGER)
{
	m_pICompanionManager=NULL;
}

//��������
CDlgCompanionManager::~CDlgCompanionManager()
{
}

//�ؼ���
void CDlgCompanionManager::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX,IDCANCEL,m_btClose);
	DDX_Control(pDX,IDC_FRIEND_LIST,m_FriendListCtrl);
	DDX_Control(pDX,IDC_DETEST_LIST,m_DetestListCtrl);
	DDX_Control(pDX,IDC_COMPANION_TREE,m_CompanionTreeCtrl);
}

//��ʼ������
BOOL CDlgCompanionManager::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("�û����Ϲ���"));

	//������Ϣ
	WORD wIndex=0;
	const tagCompanionItem * pCompanionItem=NULL;
	do
	{
		//ö�ٹ�ϵ
		pCompanionItem=m_pICompanionManager->EmunCompanionItem(wIndex++);
		if (pCompanionItem==NULL) break;

		//���ƽ���
		m_CompanionTreeCtrl.InsertCompanionItem(pCompanionItem);
		if (pCompanionItem->Companion==enCompanion_Friend) m_FriendListCtrl.InsertCompanionItem(pCompanionItem);
		if (pCompanionItem->Companion==enCompanion_Detest) m_DetestListCtrl.InsertCompanionItem(pCompanionItem);
	} while (true);

	//չ���б�
	m_CompanionTreeCtrl.ExpandListItem();

	return TRUE;
}

//ȡ����Ϣ
void CDlgCompanionManager::OnCancel()
{
	DestroyWindow();
	return;
}

//���ýӿ�
bool CDlgCompanionManager::SetCompanionManager(IUnknownEx * pIUnknownEx)
{
	ASSERT(pIUnknownEx!=NULL);
	m_pICompanionManager=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ICompanionManager);
	ASSERT(m_pICompanionManager!=NULL);
	return (m_pICompanionManager!=NULL);
}

//�����ϵ
bool CDlgCompanionManager::InsertCompanionItem(const tagCompanionItem * pCompanionItem)
{
	if (GetSafeHwnd()!=NULL)
	{
		m_CompanionTreeCtrl.InsertCompanionItem(pCompanionItem);
		if (pCompanionItem->Companion==enCompanion_Friend)
		{
			m_FriendListCtrl.InsertCompanionItem(pCompanionItem);
		}
		else if (pCompanionItem->Companion==enCompanion_Detest) 
		{
			m_DetestListCtrl.InsertCompanionItem(pCompanionItem);
		}
	}

	return true;
}

//���¹�ϵ
bool CDlgCompanionManager::UpdateCompanionItem(const tagCompanionItem * pCompanionItem, bool bChangeCompanion)
{
	if (GetSafeHwnd()!=NULL)
	{
		if (bChangeCompanion==true)
		{
			//ɾ��������
			m_FriendListCtrl.DeleteCompanionItem(pCompanionItem);
			m_DetestListCtrl.DeleteCompanionItem(pCompanionItem);
			m_CompanionTreeCtrl.DeleteCompanionItem(pCompanionItem);

			//����������
			m_CompanionTreeCtrl.InsertCompanionItem(pCompanionItem);
			if (pCompanionItem->Companion==enCompanion_Friend) m_FriendListCtrl.InsertCompanionItem(pCompanionItem);
			if (pCompanionItem->Companion==enCompanion_Detest) m_DetestListCtrl.InsertCompanionItem(pCompanionItem);
		}
		else
		{
			if (pCompanionItem->Companion==enCompanion_Friend) m_FriendListCtrl.UpdateCompanionItem(pCompanionItem);
			if (pCompanionItem->Companion==enCompanion_Detest) m_DetestListCtrl.UpdateCompanionItem(pCompanionItem);
		}
	}

	return true;
}

//ɾ����ϵ
bool CDlgCompanionManager::DeleteCompanionItem(const tagCompanionItem * pCompanionItem)
{
	if (GetSafeHwnd()!=NULL)
	{
		m_CompanionTreeCtrl.DeleteCompanionItem(pCompanionItem);
		if (pCompanionItem->Companion==enCompanion_Friend)
		{
			m_FriendListCtrl.DeleteCompanionItem(pCompanionItem);
		}
		else if (pCompanionItem->Companion==enCompanion_Detest) 
		{
			m_DetestListCtrl.DeleteCompanionItem(pCompanionItem);
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
