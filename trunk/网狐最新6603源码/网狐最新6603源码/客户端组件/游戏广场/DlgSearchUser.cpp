#include "StdAfx.h"
#include "Resource.h"
#include "DlgSearchUser.h"
#include "ServerListData.h"
#include "WndViewItemCtrl.h"

//////////////////////////////////////////////////////////////////////////////////

//�Զ�����
#define UD_USER_STATION				(UD_CUSTOM+0)						//�û�λ��

//ʱ�䶨��
#define IDI_SEARCH_TIME_OUT			100									//��ʱ��ʶ
#define TIME_SEARCH_TIME_OUT		15000L								//��ʱʱ��

//////////////////////////////////////////////////////////////////////////////////
//��̬����

CDlgSearchUser * CDlgSearchUser::m_pDlgSearchUser=NULL;					//����ָ��

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgSearchUser, CSkinDialog)

	//ϵͳ��Ϣ
	ON_WM_TIMER()

	//��ť��Ϣ
	ON_BN_CLICKED(IDC_SEARCH_REMOTE, OnBnClickedSearchRemote)
	ON_BN_CLICKED(IDC_SEARCH_BY_GAME_ID, OnBnClickedSearchByGameID)

	//�ؼ���Ϣ
	ON_NOTIFY(NM_RCLICK, IDC_USER_LIST_NATIVE, OnNMRclickNativeList)
	ON_NOTIFY(NM_DBLCLK, IDC_USER_LIST_NATIVE, OnNMDblclkNativeList)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CUserListNative::CUserListNative()
{
}

//��������
CUserListNative::~CUserListNative()
{
	//ɾ������
	for (INT_PTR i=0;i<m_NativeListDataActive.GetCount();i++)
	{
		SafeDelete(m_NativeListDataActive[i]);
	}

	//ɾ������
	for (INT_PTR i=0;i<m_NativeListDataBuffer.GetCount();i++)
	{
		SafeDelete(m_NativeListDataBuffer[i]);
	}

	//ɾ������
	m_NativeListDataActive.RemoveAll();
	m_NativeListDataBuffer.RemoveAll();

	return;
}

//�ؼ���
VOID CUserListNative::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	//�û��ǳ�
	m_cbDataDescribe[m_wColumnCount]=UD_NICKNAME;
	InsertColumn(m_wColumnCount++,TEXT("�û��ǳ�"),LVCFMT_CENTER,100,0);

	//��Ϸ��ʶ
	m_cbDataDescribe[m_wColumnCount]=UD_GAME_ID;
	InsertColumn(m_wColumnCount++,TEXT("ID"),LVCFMT_CENTER,60,0);

	//�û��ɼ�
	m_cbDataDescribe[m_wColumnCount]=UD_SCORE;
	InsertColumn(m_wColumnCount++,TEXT("�ɼ�"),LVCFMT_CENTER,100,0);

	//�û��ȼ�
	m_cbDataDescribe[m_wColumnCount]=UD_GAME_LEVEL;
	InsertColumn(m_wColumnCount++,TEXT("�ȼ�"),LVCFMT_CENTER,60,0);

	//�û�����
	m_cbDataDescribe[m_wColumnCount]=UD_TABLE;
	InsertColumn(m_wColumnCount++,TEXT("����"),LVCFMT_CENTER,40,0);

	//�û�����
	m_cbDataDescribe[m_wColumnCount]=UD_USER_MEDAL;
	InsertColumn(m_wColumnCount++,TEXT("����"),LVCFMT_CENTER,85,0);

	//�û�����
	m_cbDataDescribe[m_wColumnCount]=UD_EXPERIENCE;
	InsertColumn(m_wColumnCount++,TEXT("����ֵ"),LVCFMT_CENTER,85,0);

	//�û�����
	m_cbDataDescribe[m_wColumnCount]=UD_LOVELINESS;
	InsertColumn(m_wColumnCount++,TEXT("����ֵ"),LVCFMT_CENTER,85,0);

	//�û�ʤ��
	m_cbDataDescribe[m_wColumnCount]=UD_WIN_RATE;
	InsertColumn(m_wColumnCount++,TEXT("ʤ��"),LVCFMT_CENTER,60,0);

	//�û�����
	m_cbDataDescribe[m_wColumnCount]=UD_LOST_RATE;
	InsertColumn(m_wColumnCount++,TEXT("����"),LVCFMT_CENTER,60,0);

	//�û��ܾ�
	m_cbDataDescribe[m_wColumnCount]=UD_PLAY_COUNT;
	InsertColumn(m_wColumnCount++,TEXT("�ܾ�"),LVCFMT_CENTER,60,0);

	//�û�Ӯ��
	m_cbDataDescribe[m_wColumnCount]=UD_WIN_COUNT;
	InsertColumn(m_wColumnCount++,TEXT("Ӯ��"),LVCFMT_CENTER,60,0);

	//�û����
	m_cbDataDescribe[m_wColumnCount]=UD_LOST_COUNT;
	InsertColumn(m_wColumnCount++,TEXT("���"),LVCFMT_CENTER,60,0);

	//�û��;�
	m_cbDataDescribe[m_wColumnCount]=UD_DRAW_COUNT;
	InsertColumn(m_wColumnCount++,TEXT("�;�"),LVCFMT_CENTER,60,0);

	//�û��Ӿ�
	m_cbDataDescribe[m_wColumnCount]=UD_FLEE_COUNT;
	InsertColumn(m_wColumnCount++,TEXT("�Ӿ�"),LVCFMT_CENTER,60,0);

	//�û�����
	m_cbDataDescribe[m_wColumnCount]=UD_GROUP_NAME;
	InsertColumn(m_wColumnCount++,TEXT("����"),LVCFMT_CENTER,85,0);

	//�û�ǩ��
	m_cbDataDescribe[m_wColumnCount]=UD_UNDER_WRITE;
	InsertColumn(m_wColumnCount++,TEXT("ǩ��"),LVCFMT_CENTER,150,0);

	//��ǰλ��
	m_cbDataDescribe[m_wColumnCount]=UD_USER_STATION;
	InsertColumn(m_wColumnCount++,TEXT("��ǰλ��"),LVCFMT_CENTER,300,0);

	return;
}

//�û�����
IClientUserItem * CUserListNative::GetClientUserItem(VOID * pItemData)
{
	//��������
	ASSERT(pItemData!=NULL);
	tagNativeListData * pNativeListData=(tagNativeListData *)pItemData;

	//��ȡ�û�
	ASSERT(pNativeListData->pIClientUserItem!=NULL);
	IClientUserItem * pIClientUserItem=pNativeListData->pIClientUserItem;

	return pIClientUserItem;
}

//�����ַ�
VOID CUserListNative::ConstructString(VOID * pItemData, WORD wColumnIndex, LPTSTR pszString, WORD wMaxCount)
{
	//Ч�����
	ASSERT(wColumnIndex<m_wColumnCount);
	if (wColumnIndex>=m_wColumnCount) return;

	//��������
	ASSERT(pItemData!=NULL);
	tagNativeListData * pNativeListData=(tagNativeListData *)pItemData;

	//�����ַ�
	switch (m_cbDataDescribe[wColumnIndex])
	{
	case UD_GAME_LEVEL:		//��Ϸ�ȼ�
		{
			//Ч��״̬
			ASSERT(pNativeListData->pIClientUserItem!=NULL);
			ASSERT(pNativeListData->pIGameLevelParser!=NULL);

			//��������
			IClientUserItem * pIClientUserItem=pNativeListData->pIClientUserItem;
			IGameLevelParser * pIGameLevelParser=pNativeListData->pIGameLevelParser;

			//��ȡ�ȼ�
			_sntprintf(pszString,wMaxCount,TEXT("%s"),pIGameLevelParser->GetLevelDescribe(pIClientUserItem));

			return;
		}
	case UD_USER_STATION:	//��ǰλ��
		{
			//��������
			ASSERT(pNativeListData->pServerViewItem!=NULL);
			CServerViewItem * pServerViewItem=pNativeListData->pServerViewItem;

			//����λ��
			LPCTSTR pszKindName=pServerViewItem->GetKindName();
			LPCTSTR pszServerName=pServerViewItem->GetServerName();
			_sntprintf(pszString,wMaxCount,TEXT("���ڡ�%s - %s����Ϸ������"),pszKindName,pszServerName);

			return;
		}
	}

	__super::ConstructString(pItemData,wColumnIndex,pszString,wMaxCount);
}

//��������
VOID CUserListNative::InsertUserItem(IClientUserItem * pIClientUserItem, CServerViewItem * pServerViewItem)
{
	//��������
	tagNativeListData * pNativeListData=CreateNativeListData();

	//���ýӿ�
	pNativeListData->pServerViewItem=pServerViewItem;
	pNativeListData->pIClientUserItem=pIClientUserItem;
	pNativeListData->pIGameLevelParser=pServerViewItem->GetGameLevelParser();

	//�����б�
	InsertDataItem(pNativeListData);

	return;
}

//��������
VOID CUserListNative::UpdateUserItem(IClientUserItem * pIClientUserItem, CServerViewItem * pServerViewItem)
{
	//��������
	for (INT_PTR i=0;i<m_NativeListDataActive.GetCount();i++)
	{
		//��ȡ����
		tagNativeListData * pNativeListData=m_NativeListDataActive[i];

		//�����ж�
		if (pNativeListData->pServerViewItem!=pServerViewItem) continue;
		if (pNativeListData->pIClientUserItem!=pIClientUserItem) continue;

		//��������
		UpdateDataItem(pNativeListData);

		break;
	}

	return;
}

//ɾ������
VOID CUserListNative::DeleteUserItem(IClientUserItem * pIClientUserItem, CServerViewItem * pServerViewItem)
{
	//����ɾ��
	if (pIClientUserItem==NULL)
	{
		//ȫ���ж�
		if (pServerViewItem!=NULL)
		{
			//��������
			for (INT_PTR i=0;i<m_NativeListDataActive.GetCount();i++)
			{
				//��ȡ����
				tagNativeListData * pNativeListData=m_NativeListDataActive[i];

				//ɾ������
				if (pNativeListData->pServerViewItem==pServerViewItem)
				{
					DeleteDataItem(pNativeListData);
					DeleteNativeListData(pNativeListData);
				}
			}
		}
		else
		{
			//ɾ���б�
			DeleteAllItems();

			//��������
			m_NativeListDataBuffer.Append(m_NativeListDataActive);
			m_NativeListDataActive.RemoveAll();
		}

		return;
	}

	//��������
	for (INT_PTR i=0;i<m_NativeListDataActive.GetCount();i++)
	{
		//��ȡ����
		tagNativeListData * pNativeListData=m_NativeListDataActive[i];

		//�����ж�
		if (pNativeListData->pServerViewItem!=pServerViewItem) continue;
		if (pNativeListData->pIClientUserItem!=pIClientUserItem) continue;

		//ɾ������
		DeleteDataItem(pNativeListData);

		//ɾ������
		DeleteNativeListData(pNativeListData);

		break;
	}

	return;
}

//��ȡ����
tagNativeListData * CUserListNative::CreateNativeListData()
{
	//�ִ����
	if (m_NativeListDataBuffer.GetCount()>0L)
	{
		//��������
		INT_PTR nIndexBuffer=m_NativeListDataBuffer.GetCount()-1L;
		tagNativeListData * pNativeListData=m_NativeListDataBuffer[nIndexBuffer];

		//��������
		m_NativeListDataActive.Add(pNativeListData);
		m_NativeListDataBuffer.RemoveAt(nIndexBuffer);

		//��������
		ZeroMemory(pNativeListData,sizeof(tagNativeListData));

		return pNativeListData;
	}

	//��������
	try
	{
		//��������
		tagNativeListData * pNativeListData=new tagNativeListData;

		//�����ж�
		if (pNativeListData==NULL)
		{
			ASSERT(FALSE);
			return NULL;
		}

		//��������
		m_NativeListDataActive.Add(pNativeListData);
		ZeroMemory(pNativeListData,sizeof(tagNativeListData));

		return pNativeListData;
	}
	catch (...)
	{
		ASSERT(FALSE);
	}

	return NULL;
}

//ɾ������
VOID CUserListNative::DeleteNativeListData(tagNativeListData * pNativeListData)
{
	//�������
	for (INT_PTR i=0;i<m_NativeListDataActive.GetCount();i++)
	{
		if (m_NativeListDataActive[i]==pNativeListData)
		{
			m_NativeListDataActive.RemoveAt(i);
			m_NativeListDataBuffer.Add(pNativeListData);
			return;
		}
	}

	//�������
	ASSERT(FALSE);

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CUserListRemote::CUserListRemote()
{
	//���ÿؼ�
	m_SkinHeaderCtrl.SetLockCount(3L);
	m_SkinHeaderCtrl.SetItemHeight(130L);

	//�б���Ϣ
	m_wColumnCount=0;
	ZeroMemory(m_cbDataDescribe,sizeof(m_cbDataDescribe));

	return;
}

//��������
CUserListRemote::~CUserListRemote()
{
	//ɾ������
	for (INT_PTR i=0;i<m_RemoteListDataActive.GetCount();i++)
	{
		SafeDelete(m_RemoteListDataActive[i]);
	}

	//ɾ������
	for (INT_PTR i=0;i<m_RemoteListDataBuffer.GetCount();i++)
	{
		SafeDelete(m_RemoteListDataBuffer[i]);
	}

	//ɾ������
	m_RemoteListDataActive.RemoveAll();
	m_RemoteListDataBuffer.RemoveAll();

	return;
}

//�ؼ���
VOID CUserListRemote::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	//����״̬
	if (m_ImageUserStatus.m_hImageList==NULL)
	{
		//����ͼƬ
		CBitmap Image;
		BITMAP ImageInfo;
		Image.LoadBitmap(IDB_REMOTE_USER_IMAGE);

		//����ͼƬ
		Image.GetBitmap(&ImageInfo);
		m_ImageUserStatus.Create(20,ImageInfo.bmHeight,ILC_COLOR16|ILC_MASK,0,0);

		//����ͼƬ
		m_ImageUserStatus.Add(&Image,RGB(255,0,255));
	}

	//���ÿؼ�
	SetImageList(&m_ImageUserStatus,LVSIL_SMALL);

	//ռλ��־
	m_cbDataDescribe[m_wColumnCount]=UD_NULL;
	InsertColumn(m_wColumnCount++,TEXT(""),LVCFMT_CENTER,0,0);

	//�û���־
	m_cbDataDescribe[m_wColumnCount]=UD_IMAGE_FLAG;
	InsertColumn(m_wColumnCount++,TEXT(""),LVCFMT_CENTER,20,0);

	//�û��Ա�
	m_cbDataDescribe[m_wColumnCount]=UD_IMAGE_GENDER;
	InsertColumn(m_wColumnCount++,TEXT(""),LVCFMT_CENTER,20,0);

	//�û��ǳ�
	m_cbDataDescribe[m_wColumnCount]=UD_NICKNAME;
	InsertColumn(m_wColumnCount++,TEXT("�û��ǳ�"),LVCFMT_CENTER,100,0);

	//��Ϸ��ʶ
	m_cbDataDescribe[m_wColumnCount]=UD_GAME_ID;
	InsertColumn(m_wColumnCount++,TEXT("ID"),LVCFMT_CENTER,60,0);

	//��ǰλ��
	m_cbDataDescribe[m_wColumnCount]=UD_USER_STATION;
	InsertColumn(m_wColumnCount++,TEXT("��ǰλ��"),LVCFMT_CENTER,300,0);

	return;
}

//��������
INT CUserListRemote::SortItemData(LPARAM lParam1, LPARAM lParam2, WORD wColumnIndex, bool bAscendSort)
{
	//�������
	tagRemoteListData * pRemoteListData1=(tagRemoteListData *)lParam1;
	tagRemoteListData * pRemoteListData2=(tagRemoteListData *)lParam2;

	//�Ա�����
	switch (m_cbDataDescribe[wColumnIndex])
	{
	case UD_GAME_ID:		//��Ϸ��ʶ
		{
			//��ȡ����
			DWORD dwGameID1=pRemoteListData1->dwGameID;
			DWORD dwGameID2=pRemoteListData2->dwGameID;

			//�Ա�����
			INT nResult=dwGameID1>dwGameID2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	}

	return __super::SortItemData(lParam1,lParam2,wColumnIndex,bAscendSort);
}

//�����ַ�
VOID CUserListRemote::ConstructString(VOID * pItemData, WORD wColumnIndex, LPTSTR pszString, WORD wMaxCount)
{
	//Ч�����
	ASSERT(wColumnIndex<m_wColumnCount);
	if (wColumnIndex>=m_wColumnCount) return;

	//��������
	ASSERT(pItemData!=NULL);
	tagRemoteListData * pRemoteListData=(tagRemoteListData *)pItemData;

	//�����ַ�
	switch (m_cbDataDescribe[wColumnIndex])
	{
	case UD_NICKNAME:		//��Ϸ�ǳ�
		{
			//��ȡ�ǳ�
			_sntprintf(pszString,wMaxCount,TEXT("%s"),pRemoteListData->szNickName);

			return;
		}
	case UD_GAME_ID:		//��Ϸ��ʶ
		{
			//�����ʶ
			_sntprintf(pszString,wMaxCount,TEXT("%ld"),pRemoteListData->dwGameID);

			return;
		}
	case UD_USER_STATION:	//��ǰλ��
		{
			//����λ��
			LPCTSTR pszKindName=pRemoteListData->szGameKind;
			LPCTSTR pszServerName=pRemoteListData->szGameServer;
			_sntprintf(pszString,wMaxCount,TEXT("���ڡ�%s - %s����Ϸ������"),pszKindName,pszServerName);

			return;
		}
	}

	__super::ConstructString(pItemData,wColumnIndex,pszString,wMaxCount);
}

//�滭����
VOID CUserListRemote::DrawCustomItem(CDC * pDC, LPDRAWITEMSTRUCT lpDrawItemStruct, CRect & rcSubItem, INT nColumnIndex)
{
	//����Ч��
	ASSERT(nColumnIndex<m_wColumnCount);
	if (nColumnIndex>=m_wColumnCount) return;

	//��������
	INT nItemID=lpDrawItemStruct->itemID;
	tagRemoteListData * pRemoteListData=(tagRemoteListData *)(lpDrawItemStruct->itemData);

	//�滭��Ϣ
	switch (m_cbDataDescribe[nColumnIndex])
	{
	case UD_IMAGE_FLAG:		//�û���־
		{
			//��������
			WORD wImageIndex=2;
			if (pRemoteListData->cbMemberOrder!=0) wImageIndex=3+__min(pRemoteListData->cbMemberOrder,4);
			if (pRemoteListData->cbMasterOrder!=0) wImageIndex=8+__min(pRemoteListData->cbMasterOrder,4);

			//�滭��־
			m_ImageUserStatus.Draw(pDC,wImageIndex,CPoint(rcSubItem.left,rcSubItem.top),ILD_TRANSPARENT);

			break;
		}
	case UD_IMAGE_GENDER:	//�û��Ա�
		{
			//��������
			WORD wImageIndex=0;
			if (pRemoteListData->cbGender==GENDER_MANKIND) wImageIndex=1;

			//�滭��־
			m_ImageUserStatus.Draw(pDC,wImageIndex,CPoint(rcSubItem.left,rcSubItem.top),ILD_TRANSPARENT);
			
			break;
		}
	default:				//Ĭ�ϵ���
		{
			__super::DrawCustomItem(pDC,lpDrawItemStruct,rcSubItem,nColumnIndex);
		}
	}

	return;
}

//��������
VOID CUserListRemote::InsertUserItem(tagUserRemoteInfo * pUserRemoteInfo, LPCTSTR pszKindName)
{
	//��������
	tagRemoteListData * pRemoteListData=CreateRemoteListData();

	//������Ϣ
	pRemoteListData->cbGender=pUserRemoteInfo->cbGender;
	pRemoteListData->cbMemberOrder=pUserRemoteInfo->cbMemberOrder;
	pRemoteListData->cbMasterOrder=pUserRemoteInfo->cbMasterOrder;

	//�û���Ϣ
	pRemoteListData->dwUserID=pUserRemoteInfo->dwUserID;
	pRemoteListData->dwGameID=pUserRemoteInfo->dwGameID;
	lstrcpyn(pRemoteListData->szNickName,pUserRemoteInfo->szNickName,CountArray(pRemoteListData->szNickName));

	//λ����Ϣ
	pRemoteListData->wKindID=pUserRemoteInfo->wKindID;
	pRemoteListData->wServerID=pUserRemoteInfo->wServerID;
	lstrcpyn(pRemoteListData->szGameKind,pszKindName,CountArray(pRemoteListData->szGameKind));
	lstrcpyn(pRemoteListData->szGameServer,pUserRemoteInfo->szGameServer,CountArray(pRemoteListData->szGameServer));

	//�����б�
	InsertDataItem(pRemoteListData);

	return;
}

//��ȡ����
tagRemoteListData * CUserListRemote::CreateRemoteListData()
{
	//�ִ����
	if (m_RemoteListDataBuffer.GetCount()>0L)
	{
		//��������
		INT_PTR nIndexBuffer=m_RemoteListDataBuffer.GetCount()-1L;
		tagRemoteListData * pRemoteListData=m_RemoteListDataBuffer[nIndexBuffer];

		//��������
		m_RemoteListDataActive.Add(pRemoteListData);
		m_RemoteListDataBuffer.RemoveAt(nIndexBuffer);

		//��������
		ZeroMemory(pRemoteListData,sizeof(tagRemoteListData));

		return pRemoteListData;
	}

	//��������
	try
	{
		//��������
		tagRemoteListData * pRemoteListData=new tagRemoteListData;

		//�����ж�
		if (pRemoteListData==NULL)
		{
			ASSERT(FALSE);
			return NULL;
		}

		//��������
		m_RemoteListDataActive.Add(pRemoteListData);
		ZeroMemory(pRemoteListData,sizeof(tagRemoteListData));

		return pRemoteListData;
	}
	catch (...)
	{
		ASSERT(FALSE);
	}

	return NULL;
}

//ɾ������
VOID CUserListRemote::DeleteRemoteListData(tagRemoteListData * pRemoteListData)
{
	//�������
	for (INT_PTR i=0;i<m_RemoteListDataActive.GetCount();i++)
	{
		if (m_RemoteListDataActive[i]==pRemoteListData)
		{
			m_RemoteListDataActive.RemoveAt(i);
			m_RemoteListDataBuffer.Add(pRemoteListData);
			return;
		}
	}

	//�������
	ASSERT(FALSE);

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgSearchUser::CDlgSearchUser() : CSkinDialog(IDD_DLG_SEARCH_USER)
{
	//��������
	m_MissionManager.InsertMissionItem(this);

	//���ñ���
	m_dwGameID=0L;
	ZeroMemory(m_szNickName,sizeof(m_szNickName));

	//���ñ���
	ASSERT(m_pDlgSearchUser==NULL);
	if (m_pDlgSearchUser==NULL) m_pDlgSearchUser=this;

	return;
}

//��������
CDlgSearchUser::~CDlgSearchUser()
{
	//���ñ���
	ASSERT(m_pDlgSearchUser==this);
	if (m_pDlgSearchUser==this) m_pDlgSearchUser=NULL;

	return;
}

//�ؼ���
VOID CDlgSearchUser::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	//��ť�ؼ�
	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Control(pDX, IDCANCEL, m_btCancel);

	//���ҿؼ�
	DDX_Control(pDX, IDC_GAME_ID, m_edGameID);
	DDX_Control(pDX, IDC_NICKNAME, m_edNickName);
	DDX_Control(pDX, IDC_SEARCH_AREA, m_ComSearchArea);

	//�ؼ�����
	DDX_Control(pDX, IDC_SEARCH_REMOTE, m_SearchRemote);
	DDX_Control(pDX, IDC_SEARCH_BY_GAME_ID, m_SearchByGameID);

	//�ؼ�����
	DDX_Control(pDX, IDC_STATIC_IMAGE, m_StaticImage);
	DDX_Control(pDX, IDC_USER_LIST_REMOTE, m_UserListRemote);
	DDX_Control(pDX, IDC_USER_LIST_NATIVE, m_UserListNative);
}

//��Ϣ����
BOOL CDlgSearchUser::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		//���ý���
		if (m_edGameID.IsWindowVisible()==TRUE)
		{
			m_edGameID.SetFocus();
			m_edGameID.SetSel(0L,-1L);
		}

		//���ý���
		if (m_edNickName.IsWindowVisible()==TRUE)
		{
			m_edNickName.SetFocus();
			m_edNickName.SetSel(0L,-1L);
		}

		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//��������
BOOL CDlgSearchUser::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("�û�����"));

	//���ÿؼ�
	m_edGameID.LimitText(10);
	m_edNickName.LimitText(LEN_ACCOUNTS-1);

	//���䷶Χ
	ASSERT(CWndViewItemCtrl::GetInstance()!=NULL);
	CWndViewItemCtrl * pWndViewItemCtrl=CWndViewItemCtrl::GetInstance();

	//�����б�
	LPCTSTR pszString=TEXT("��ǰ�����ڵ�ȫ����Ϸ����");
	m_ComSearchArea.SetCurSel(m_ComSearchArea.InsertString(0,pszString));

	//�����б�
	for (WORD i=1;i<pWndViewItemCtrl->GetViewItemCount();i++)
	{
		//��ȡ����
		tagViewItemInfo * pViewItemInfo=pWndViewItemCtrl->GetViewItemInfo(i);

		//�����б�
		if (pViewItemInfo!=NULL)
		{
			//��ȡ����
			ASSERT(pViewItemInfo->pItemView!=NULL);
			CServerViewItem * pServerViewItem=(CServerViewItem *)pViewItemInfo->pItemView;

			//�����б�
			TCHAR szItemString[256]=TEXT("");
			_sntprintf(szItemString,CountArray(szItemString),TEXT("%s - %s"),pServerViewItem->GetKindName(),pServerViewItem->GetServerName());

			//�����б�
			INT nItem=m_ComSearchArea.InsertString(m_ComSearchArea.GetCount(),szItemString);
			if (nItem!=LB_ERR) m_ComSearchArea.SetItemData(nItem,pServerViewItem->GetServerID());
		}
	}

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//�����б�
	m_UserListNative.SetMySelfUserID(pGlobalUserData->dwUserID);

	//�����ؼ�
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());

	return FALSE;
}

//ȷ����Ϣ
VOID CDlgSearchUser::OnOK()
{
	//ģʽ����
	bool bRemoteMode=(m_SearchRemote.GetCheck()==BST_CHECKED);
	bool bGameIDMode=(m_SearchByGameID.GetCheck()==BST_CHECKED);

	//��ȡ����
	CString strSearchKey;
	if (m_edGameID.IsWindowVisible()==TRUE) m_edGameID.GetWindowText(strSearchKey);
	if (m_edNickName.IsWindowVisible()==TRUE) m_edNickName.GetWindowText(strSearchKey);

	//���봦��
	strSearchKey.TrimLeft();
	strSearchKey.TrimRight();
	strSearchKey.MakeLower();

	//Զ���ж�
	if (GetActiveStatus()==true)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("Զ�̲��Ҳ������ڽ����У����Ժ��ٽ����µĲ����ύ��"),MB_ICONQUESTION,30L);

		return;
	}

	//Ч������
	if ((_tstol(strSearchKey)==0L)&&(bGameIDMode==true))
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("��������Ҫ���ҵġ���Ϸ I D�����룡"),MB_ICONSTOP);

		//���ý���
		m_edGameID.SetFocus();

		return;
	}

	//Ч������
	if ((strSearchKey.IsEmpty()==TRUE)&&(bGameIDMode==false))
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("�����������ġ��û��ǳơ��������еĲ����ַ���"),MB_ICONSTOP);

		//���ý���
		m_edNickName.SetFocus();

		return;
	}

	//�б�����
	m_UserListRemote.DeleteAllItems();
	m_UserListNative.DeleteUserItem(NULL,NULL);

	//�б����
	if (bRemoteMode==true)
	{
		m_UserListRemote.ShowWindow(SW_SHOW);
		m_UserListNative.ShowWindow(SW_HIDE);
	}
	else
	{
		m_UserListNative.ShowWindow(SW_SHOW);
		m_UserListRemote.ShowWindow(SW_HIDE);
	}

	//���Ҵ���
	if (bRemoteMode==false)
	{
		//��������
		ASSERT(CWndViewItemCtrl::GetInstance()!=NULL);
		CWndViewItemCtrl * pWndViewItemCtrl=CWndViewItemCtrl::GetInstance();

		//���䷶Χ
		INT nItem=m_ComSearchArea.GetCurSel();
		WORD wServerID=(WORD)m_ComSearchArea.GetItemData(nItem);

		//�����б�
		for (WORD i=1;i<pWndViewItemCtrl->GetViewItemCount();i++)
		{
			//��ȡ����
			tagViewItemInfo * pViewItemInfo=pWndViewItemCtrl->GetViewItemInfo(i);

			//��������
			CServerViewItem * pServerViewItem=(CServerViewItem *)pViewItemInfo->pItemView;
			IPlazaUserManager * pIPlazaUserManager=pServerViewItem->GetPlazaUserManager();

			//�������
			if ((wServerID!=0)&&(pServerViewItem->GetServerID()!=wServerID))
			{
				continue;
			}

			//�����û�
			if (bGameIDMode==false)
			{
				//��������
				WORD wEmunIndex=0;
				CString strNickName;

				//�����û�
				do
				{
					//ö���û�
					IClientUserItem * pIClientUserItem=pIPlazaUserManager->EnumUserItem(wEmunIndex++);

					//�û��ж�
					if (pIClientUserItem==NULL)
					{
						break;
					}

					//��ȡ�ǳ�
					strNickName=pIClientUserItem->GetNickName();
					strNickName.MakeLower();

					//�û��ж�
					if (strNickName.Find(strSearchKey)!=-1)
					{
						m_UserListNative.InsertUserItem(pIClientUserItem,pServerViewItem);
					}

				} while (true);
			}
			else
			{
				//ת�� I D
				DWORD dwGameID=_tstol(strSearchKey);

				//�����û�
				IClientUserItem * pIClientUserItem=pIPlazaUserManager->SearchUserByGameID(dwGameID);
				if (pIClientUserItem!=NULL) m_UserListNative.InsertUserItem(pIClientUserItem,pServerViewItem);
			}
		}

		//����ж�
		if (m_UserListNative.GetItemCount()==0L)
		{
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("������ѯ���û������ڻ���û�����ߣ�����Ĳ�ѯ�������ԣ�"),MB_ICONQUESTION,30L);
		}

		//���ý���
		if (m_edGameID.IsWindowVisible()==TRUE)
		{
			m_edGameID.SetFocus();
			m_edGameID.SetSel(0L,-1L);
		}

		//���ý���
		if (m_edNickName.IsWindowVisible()==TRUE)
		{
			m_edNickName.SetFocus();
			m_edNickName.SetSel(0L,-1L);
		}
	}
	else
	{
		//���ñ���
		if (bGameIDMode==true)
		{
			m_dwGameID=_tstol(strSearchKey);
			ZeroMemory(m_szNickName,sizeof(m_szNickName));
		}
		else
		{
			m_dwGameID=0L;
			lstrcpyn(m_szNickName,strSearchKey,CountArray(m_szNickName));
		}

		//��������
		if (m_MissionManager.AvtiveMissionItem(this,false)==true) 
		{
			//����ʱ��
			SetTimer(IDI_SEARCH_TIME_OUT,TIME_SEARCH_TIME_OUT,NULL);
		}
	}

	return;
}

//����ر�
VOID CDlgSearchUser::OnViewItemDelete(CServerViewItem * pServerViewItem)
{
	//ɾ������
	if (m_UserListNative.GetItemCount()>0L)
	{
		m_UserListNative.DeleteUserItem(NULL,pServerViewItem);
	}

	return;
}

//�û�ɾ��
VOID CDlgSearchUser::OnUserItemDelete(IClientUserItem * pIClientUserItem, CServerViewItem * pServerViewItem)
{
	//ɾ������
	if (m_UserListNative.GetItemCount()>0L)
	{
		m_UserListNative.DeleteUserItem(pIClientUserItem,pServerViewItem);
	}

	return;
}

//�û�����
VOID CDlgSearchUser::OnUserItemUpdate(IClientUserItem * pIClientUserItem, CServerViewItem * pServerViewItem)
{
	//��������
	if (m_UserListNative.GetItemCount()>0L)
	{
		m_UserListNative.UpdateUserItem(pIClientUserItem,pServerViewItem);
	}

	return;
}

//�����ؼ�
VOID CDlgSearchUser::RectifyControl(INT nWidth, INT nHeight)
{
	//���λ��
	INT nTBorder=m_SkinAttribute.m_EncircleInfoFrame.nTBorder;
	INT nBBorder=m_SkinAttribute.m_EncircleInfoFrame.nBBorder;
	INT nLBorder=m_SkinAttribute.m_EncircleInfoFrame.nLBorder;
	INT nRBorder=m_SkinAttribute.m_EncircleInfoFrame.nRBorder;

	//�б�λ��
	CRect rcListControl;
	rcListControl.top=nTBorder+150L;
	rcListControl.left=nLBorder+155L;
	rcListControl.right=nWidth-nRBorder-10L;
	rcListControl.bottom=nHeight-nBBorder-5L;

	//�ƶ��ؼ�
	m_StaticImage.SetWindowPos(NULL,nLBorder,nTBorder,0,0,SWP_NOZORDER|SWP_NOSIZE);
	m_UserListRemote.SetWindowPos(NULL,rcListControl.left,rcListControl.top,rcListControl.Width(),rcListControl.Height(),SWP_NOZORDER);
	m_UserListNative.SetWindowPos(NULL,rcListControl.left,rcListControl.top,rcListControl.Width(),rcListControl.Height(),SWP_NOZORDER);

	return;
}

//�滭��Ϣ
VOID CDlgSearchUser::OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight)
{
	//���λ��
	INT nTBorder=m_SkinAttribute.m_EncircleInfoFrame.nTBorder;
	INT nBBorder=m_SkinAttribute.m_EncircleInfoFrame.nBBorder;
	INT nLBorder=m_SkinAttribute.m_EncircleInfoFrame.nLBorder;
	INT nRBorder=m_SkinAttribute.m_EncircleInfoFrame.nRBorder;

	//�б�λ��
	CRect rcListControl;
	rcListControl.top=nTBorder+149L;
	rcListControl.left=nLBorder+154L;
	rcListControl.right=nWidth-nRBorder-9L;
	rcListControl.bottom=nHeight-nBBorder-4L;

	//�滭����
	pDC->Draw3dRect(&rcListControl,RGB(84,165,213),RGB(84,165,213));

	//�滭ͼƬ
	CPngImage ImageSearchLogo;
	ImageSearchLogo.LoadImage(AfxGetInstanceHandle(),TEXT("SEARCH_LOGO"));
	ImageSearchLogo.DrawImage(pDC,rcListControl.left+6,rcListControl.top-ImageSearchLogo.GetHeight()-5);

	return;
}

//�����¼�
bool CDlgSearchUser::OnEventMissionLink(INT nErrorCode)
{
	//���ӽ��
	if (nErrorCode!=0)
	{
		//��������
		if (m_MissionManager.AvtiveMissionItem(this,true)==true)
		{
			return true;
		}

		//ɾ��ʱ��
		KillTimer(IDI_SEARCH_TIME_OUT);

		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("���ӷ�������ʱ��Զ�̲��Ҳ���ʧ�ܣ�"),MB_OK|MB_ICONSTOP,30);

		return true;
	}

	//��������
	CMD_GP_C_SearchCorrespond SearchCorrespond;
	ZeroMemory(&SearchCorrespond,sizeof(SearchCorrespond));

	//��������
	SearchCorrespond.dwGameID=m_dwGameID;
	lstrcpyn(SearchCorrespond.szNickName,m_szNickName,CountArray(SearchCorrespond.szNickName));

	//��������
	m_MissionManager.SendData(MDM_GP_REMOTE_SERVICE,SUB_GP_C_SEARCH_CORRESPOND,&SearchCorrespond,sizeof(SearchCorrespond));

	return true;
}

//�ر��¼�
bool CDlgSearchUser::OnEventMissionShut(BYTE cbShutReason)
{
	//��ʾ��Ϣ
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		//��������
		if (m_MissionManager.AvtiveMissionItem(this,true)==true)
		{
			return true;
		}

		//ɾ��ʱ��
		KillTimer(IDI_SEARCH_TIME_OUT);

		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("��������������쳣�Ͽ��ˣ�Զ�̲��Ҳ���ʧ�ܣ�"),MB_ICONERROR,30L);
	}
	else
	{
		//ɾ��ʱ��
		KillTimer(IDI_SEARCH_TIME_OUT);
	}
	
	return true;
}

//��ȡ�¼�
bool CDlgSearchUser::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//�����
	if (Command.wMainCmdID==MDM_GP_REMOTE_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_S_SEARCH_CORRESPOND:	//���ҽ��
			{
				//��������
				CMD_GP_S_SearchCorrespond * pSearchCorrespond=(CMD_GP_S_SearchCorrespond *)pData;

				//Ч�����
				ASSERT(wDataSize<=sizeof(CMD_GP_S_SearchCorrespond));
				ASSERT(wDataSize>=(sizeof(CMD_GP_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)));
				ASSERT(wDataSize==(sizeof(CMD_GP_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)+pSearchCorrespond->wUserCount*sizeof(pSearchCorrespond->UserRemoteInfo[0])));

				//Ч�����
				if (wDataSize>sizeof(CMD_GP_S_SearchCorrespond)) return false;
				if (wDataSize<(sizeof(CMD_GP_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo))) return false;
				if (wDataSize!=(sizeof(CMD_GP_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)+pSearchCorrespond->wUserCount*sizeof(pSearchCorrespond->UserRemoteInfo[0]))) return false;

				//ʧ�ܴ���
				KillTimer(IDI_SEARCH_TIME_OUT);

				//�ر�����
				m_MissionManager.ConcludeMissionItem(this,false);

				//�����б�
				for (WORD i=0;i<pSearchCorrespond->wUserCount;i++)
				{
					//��������
					CServerListData * pServerListData=CServerListData::GetInstance();
					CGameKindItem * pGameKindItem=pServerListData->SearchGameKind(pSearchCorrespond->UserRemoteInfo[i].wKindID);

					//�����б�
					if (pGameKindItem!=NULL)
					{
						LPCTSTR pszGameKind=pGameKindItem->m_GameKind.szKindName;
						m_UserListRemote.InsertUserItem(&pSearchCorrespond->UserRemoteInfo[i],pszGameKind);
					}
				}

				//����ж�
				if (m_UserListRemote.GetItemCount()==0L)
				{
					CInformation Information(this);
					Information.ShowMessageBox(TEXT("������ѯ���û������ڻ���û�����ߣ�����Ĳ�ѯ�������ԣ�"),MB_ICONQUESTION,30L);
				}

				return true;
			}
		}
	}

	return true;
}

//ʱ����Ϣ
VOID CDlgSearchUser::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case IDI_SEARCH_TIME_OUT:	//��ʱʱ��
		{
			//ɾ��ʱ��
			KillTimer(IDI_SEARCH_TIME_OUT);

			//�ر�����
			m_MissionManager.ConcludeMissionItem(this,false);

			//��ʾ��Ϣ
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("��������ʱû����Ӧ��Զ�̲��Ҳ���ʧ�ܣ����Ժ����ԣ�"),MB_OK|MB_ICONSTOP,30L);

			return;
		}
	}

	__super::OnTimer(nIDEvent);
}

//Զ�̲���
VOID CDlgSearchUser::OnBnClickedSearchRemote()
{
	//��ȡ״̬
	UINT nCurrentCheck=m_SearchRemote.GetCheck();

	//��Ա�ж�
	if (nCurrentCheck==BST_UNCHECKED)
	{
		//��������
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//��Ա�ж�
		if (pGlobalUserData->cbMemberOrder==0)
		{
			//��ʾ��Ϣ
			CInformation Information(this);
			INT_PTR nResult=Information.ShowMessageBox(TEXT("�����ǻ�Ա���޷�ʹ��Զ�̲��ҹ��ܣ���Ҫ�����ɹ���Ա��"),MB_ICONQUESTION|MB_YESNO);

			//��Ա����
			if (nResult==IDYES)
			{
				//���´���
				UpdateWindow();

				//�����ַ
				TCHAR szLinkUrl[128]=TEXT("");
				_sntprintf(szLinkUrl,CountArray(szLinkUrl),TEXT("%s/PayIndex.asp"),szPlatformLink);

				//���ӵ�ַ
				ShellExecute(NULL,TEXT("OPEN"),szLinkUrl,NULL,NULL,SW_NORMAL);
			}

			return;
		}
	}

	//���ý���
	if (m_edGameID.IsWindowVisible()==TRUE) m_edGameID.SetFocus();
	if (m_edNickName.IsWindowVisible()==TRUE) m_edNickName.SetFocus();

	//���ÿؼ�
	m_ComSearchArea.EnableWindow((nCurrentCheck==BST_CHECKED)?TRUE:FALSE);
	m_SearchRemote.SetCheck((nCurrentCheck==BST_UNCHECKED)?BST_CHECKED:BST_UNCHECKED);

	return;
}

//��ʶģʽ
VOID CDlgSearchUser::OnBnClickedSearchByGameID()
{
	//ģʽ�ж�
	if (m_SearchByGameID.GetCheck()==BST_CHECKED)
	{
		//���ÿؼ�
		m_edGameID.ShowWindow(SW_SHOW);
		m_edNickName.ShowWindow(SW_HIDE);

		//���ý���
		m_edGameID.SetFocus();

		//���ñ���
		SetDlgItemText(IDC_SEARCH_NAME,TEXT("��������Ҫ���ҵ��û� ID��"));
	}
	else
	{
		//���ÿؼ�
		m_edGameID.ShowWindow(SW_HIDE);
		m_edNickName.ShowWindow(SW_SHOW);

		//���ý���
		m_edNickName.SetFocus();

		//���ñ���
		SetDlgItemText(IDC_SEARCH_NAME,TEXT("��������Ҫ���ҵ��û��ǳƣ�"));
	}

	return;
}

//�Ҽ��б�
VOID CDlgSearchUser::OnNMRclickNativeList(NMHDR * pNMHDR, LRESULT * pResult)
{
	//��������
	NMITEMACTIVATE * pListNotify=(NMITEMACTIVATE *)pNMHDR;
	
	//�����˵�
	if (pListNotify->iItem!=-1)
	{
		//��ȡ����
		INT nItem=pListNotify->iItem;
		tagNativeListData * pNativeListData=(tagNativeListData *)m_UserListNative.GetItemData(nItem);

		//�����˵�
		if (pNativeListData!=NULL)
		{
			ASSERT(pNativeListData->pServerViewItem!=NULL);
			ASSERT(pNativeListData->pIClientUserItem!=NULL);
			pNativeListData->pServerViewItem->TrackUserItemMenu(pNativeListData->pIClientUserItem);
		}
	}

	return;
}

//˫���б�
VOID CDlgSearchUser::OnNMDblclkNativeList(NMHDR * pNMHDR, LRESULT * pResult)
{
	//��������
	NMITEMACTIVATE * pListNotify=(NMITEMACTIVATE *)pNMHDR;
	
	//��Ϣ����
	if ((pListNotify->iItem!=-1)&&(pListNotify->lParam!=NULL))
	{
		//��ȡ����
		INT nItem=pListNotify->iItem;
		tagNativeListData * pNativeListData=(tagNativeListData *)m_UserListNative.GetItemData(nItem);

		//ִ�ж���
		if ((pNativeListData!=NULL)&&(pNativeListData->pIClientUserItem!=NULL))
		{
			//��������
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

			//ִ�ж���
			switch (pParameterGlobal->m_cbActionLeftDoubleList)
			{
			case ACTION_ORIENTATION:			//��λ�û�
				{
					//��ȡ����
					WORD wTableID=pNativeListData->pIClientUserItem->GetTableID();
					WORD wChairID=pNativeListData->pIClientUserItem->GetChairID();
					BYTE cbUserStatus=pNativeListData->pIClientUserItem->GetUserStatus();

					//��λλ��
					if (wTableID!=INVALID_TABLE)
					{
						//��������
						ASSERT(pNativeListData->pServerViewItem!=NULL);
						CServerViewItem * pServerViewItem=pNativeListData->pServerViewItem;

						//��������
						ASSERT(CWndViewItemCtrl::GetInstance()!=NULL);
						CWndViewItemCtrl * pWndViewItemCtrl=CWndViewItemCtrl::GetInstance();

						//���ÿ���
						pWndViewItemCtrl->ActiveViewItem(pServerViewItem);
						pServerViewItem->m_TableViewFrame.VisibleTable(wTableID);

						//��������
						if (cbUserStatus==US_LOOKON)
						{
							pServerViewItem->m_TableViewFrame.FlashGameTable(wTableID);
						}
						else
						{
							pServerViewItem->m_TableViewFrame.FlashGameChair(wTableID,wChairID);
						}
					}

					break;
				}
			case ACTION_SEND_WHISPER:			//����˽��
				{
					//Ч�����
					ASSERT(pNativeListData->pServerViewItem!=NULL);
					ASSERT(pNativeListData->pIClientUserItem!=NULL);

					//��������
					CServerViewItem * pServerViewItem=pNativeListData->pServerViewItem;
					IClientUserItem * pIClientUserItem=pNativeListData->pIClientUserItem;

					//��������
					CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
					tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

					//״̬�ж�
					if (pGlobalUserData->dwUserID==pIClientUserItem->GetUserID()) break;
					if (pServerViewItem->GetServiceStatus()!=ServiceStatus_ServiceIng) break;

					//����˽��
					IClientUserItem * pIClientUserItemArray[]={pIClientUserItem};
					pServerViewItem->WhisperConversation(pIClientUserItemArray,CountArray(pIClientUserItemArray));

					break;
				}
			case ACTION_SHOW_USER_INFO:		//�û���Ϣ
				{
					break;
				}
			}
		}
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
