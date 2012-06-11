#include "Stdafx.h"
#include "Resource.h"
#include "DlgLogon.h"
#include "DlgOption.h"
#include "Mmsystem.h"
#include "GlobalUnits.h"
#include "RoomViewItem.h"

//////////////////////////////////////////////////////////////////////////

//��ť����
#define	BUTTON_AREA_WIDTH			200									//��ť����

//////////////////////////////////////////////////////////////////////////

//�������
#define	SR_CREATE_ERROR				0									//��������
#define	SR_CREATE_SUCCESS			1									//�����ɹ�
#define	SR_ALREADY_EXIST			2									//�Ѿ�����

//////////////////////////////////////////////////////////////////////////
//�˵�����

//����˵�
#define	IDM_UM_WISPER				WM_USER+100							//˽�Ĳ˵�
#define	IDM_UM_COPY_NAME			WM_USER+101							//��������
#define	IDM_UM_SET_CHAT				WM_USER+102							//��������
#define	IDM_UM_CANECL_CHAT			WM_USER+103							//ȡ������
#define	IDM_UM_LOOK_LOCATION		WM_USER+104							//�鿴λ��
#define	IDM_UM_INVITE_PLAY			WM_USER+105							//������Ϸ
#define	IDM_UM_SEARCH_USER			WM_USER+106							//Ѱ���û�
#define	IDM_UM_SET_FRIEND			WM_USER+107							//��Ϊ����
#define	IDM_UM_SET_DETEST			WM_USER+108							//��Ϊ���

//��������
#define IDM_UM_MANAGER_USER			WM_USER+150							//Ȩ�޹���
#define IDM_UM_ISSUE_MESSAGE		WM_USER+151							//������Ϣ
#define IDM_IM_SYSTEM_OPTION		WM_USER+152							//ϵͳ����
#define IDM_UM_SEND_WARNNING		WM_USER+153							//���;���
#define IDM_UM_LOOK_USER_IP			WM_USER+154							//�鿴��ַ
#define IDM_UM_CUT_USER				WM_USER+155							//�û�����
#define IDM_UM_LIMIT_ACCOUN			WM_USER+156							//��ֹ�ʻ�

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CRoomViewItem, CDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_COPYDATA()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_MESSAGE(WM_HIT_EXPMESSTION,	OnHitExpression)
	ON_BN_CLICKED(IDC_BANK,	OnBnClickedBank)
	ON_BN_CLICKED(IDC_AUTO_SIT,	OnBnClickedAutoSit)
	ON_BN_CLICKED(IDC_FIND_USER, OnBnClickedFindUser)
	ON_BN_CLICKED(IDC_QUIT_ROOM,	OnBnClickedQuitRoom)
	ON_BN_CLICKED(IDC_SEND_CHAT, OnBnClickedSendChat)
	ON_BN_CLICKED(IDC_COLOR_SET, OnBnClickedColorSet)
	ON_BN_CLICKED(IDC_EXPRESSION, OnBnClickedExpression)
	ON_BN_CLICKED(IDC_CLEAN_SCREEN,	OnBnClickedCleanScreen)
	ON_BN_CLICKED(IDC_BUGLE,	OnBnClickedBugle)
	ON_NOTIFY(NM_DBLCLK, IDC_USER_LIST, OnNMDblclkUserList)
	ON_NOTIFY(NM_RCLICK, IDC_USER_LIST, OnNMRclickUserList)
	ON_NOTIFY(NM_CLICK, IDC_USER_LIST, OnNMclickUserList)
	ON_MESSAGE(WM_UPDATE_FACE, OnUpdateFace)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

CRoomViewItem::CRoomViewItem() : CDialog(IDD_GAME_ROOM), m_VorSplitter(VorSpliter), m_HorSplitter(HorSpliter)
{
	//��������
	m_wGameGenre=0;
	m_dwVideoAddr=0L;
	m_bRectifyHor=false;
	m_bRectifyVor=false;
	m_bInitDialog=false;
	m_bHideUserList=false;
	m_dwLimitChatTimes=0;
	m_dwMenuUserID=0L;
	m_wReqTableID=INVALID_TABLE;
	m_wReqChairID=INVALID_CHAIR;
	m_wFindTableID=INVALID_TABLE;
	memset(&m_ListColumnInfo,0,sizeof(m_ListColumnInfo));

	//��������
	m_cbHideUserInfo=FALSE;

	//���ñ���
	m_pGameOption=NULL;
	m_pServerOption=NULL;

	//�����û�
	m_dwChatUserID=0L;
	m_szChatName[0]=0;

	//������Ϣ
	m_szShareName[0]=0;
	m_hWndChannel=NULL;
	m_hShareMemory=NULL;
	m_pShareMemory=NULL;
	memset(&m_GameProcessInfo,0,sizeof(m_GameProcessInfo));

	//���ݻ���
	m_wPacketTableID=INVALID_TABLE;
	m_wPacketChairID=INVALID_CHAIR;

	//���ñ���
	m_pMeUserItem=NULL;
	m_pListServer=NULL;
	m_ServiceStatus=ServiceStatus_Null;
	memset(&m_OptionBuffer,0,sizeof(m_OptionBuffer));
	m_pFindUserDlg=NULL;

	return;
}

CRoomViewItem::~CRoomViewItem()
{
	//�����ڴ�
	if (m_pShareMemory!=NULL)
	{
		UnmapViewOfFile(m_pShareMemory);
		m_pShareMemory=NULL;
	}
	if (m_hShareMemory!=NULL)
	{
		CloseHandle(m_hShareMemory);
		m_hShareMemory=NULL;
	}

	for (INT_PTR i=0;i<m_ShortMessage.GetCount();i++) 
	{
		delete ((CShortMessage *)m_ShortMessage.GetAt(i));
		m_ShortMessage.SetAt(i,NULL);
	}
	SafeDelete(m_pFindUserDlg);
	m_PropertyBar.DestroyBar();

	return;
}

//�ӿڲ�ѯ
void * __cdecl CRoomViewItem::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IViewItem,Guid,dwQueryVer);
	QUERYINTERFACE(IRoomViewItem,Guid,dwQueryVer);
	QUERYINTERFACE(IPurchaseInfo,Guid,dwQueryVer);
	QUERYINTERFACE(ITCPSocketSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(IUserManagerSink,Guid,dwQueryVer);
	QUERYINTERFACE(IChannelMessageSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IViewItem,Guid,dwQueryVer);
	return NULL;
}

//��������
WORD CRoomViewItem::GetGameGenre()
{
	return m_wGameGenre;
}

//ö�����
tagUserData * CRoomViewItem::EnumLookonUser(WORD wIndex)
{
	return NULL;
}

//�����û�
tagUserData * CRoomViewItem::SearchUserItem(DWORD dwUserID)
{
	IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(dwUserID);

	return (pIUserItem!=NULL)?(pIUserItem->GetUserData()):NULL;
}

//�����û�
tagUserData * CRoomViewItem::SearchUserItem(LPCTSTR pszUserName)
{
	WORD wEnumIndex=0;
	while (true)
	{
		//��ȡ�û�
		IUserItem * pIUserItem=m_ClientUserManager.EnumUserItem(wEnumIndex++);
		if (pIUserItem==NULL) break;
		tagUserData * pUserData=pIUserItem->GetUserData();
		if(pUserData==NULL)continue;

		//�Ƚ�����
		CString strName,strFineUser;
		strName.Format(TEXT("%s"),pUserData->szName);
		strFineUser.Format(TEXT("%s"),pszUserName);
		if (strName==strFineUser) return pUserData;
	};
	return NULL;
}

//��ȡ���
const tagUserData * CRoomViewItem::GetUserInfo(WORD wChairID)
{
	return NULL;
}

//��ȡ�Լ�
const tagUserData * CRoomViewItem::GetMeUserInfo()
{
	return (m_pMeUserItem!=NULL)?(m_pMeUserItem->GetUserData()):NULL;
}

//������Ϣ
void CRoomViewItem::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	m_ClientSocket->SendData(wMainCmdID, wSubCmdID, pData, wDataSize);

	return;
}

//�ؼ���
void CRoomViewItem::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUGLE, m_btBugle);
	//DDX_Control(pDX, IDC_PHRASE, m_btPhrase);
	DDX_Control(pDX, IDC_COLOR_SET, m_btColorSet);
	DDX_Control(pDX, IDC_SEND_CHAT, m_btSendChat);
	DDX_Control(pDX, IDC_EXPRESSION, m_btExpression);
	DDX_Control(pDX, IDC_CLEAN_SCREEN, m_btCleanScreen);
	DDX_Control(pDX, IDC_CHAT_INPUT, m_ChatInput);
	DDX_Control(pDX, IDC_USER_LIST, m_UserListView);
	DDX_Control(pDX, IDC_VOR_SPLITER, m_VorSplitter);
	DDX_Control(pDX, IDC_CHAT_MESSAGE, m_ChatMessage);
	DDX_Control(pDX, IDC_AUTO_SIT, m_btAutoSit);
	DDX_Control(pDX, IDC_BANK,m_btBank);
	DDX_Control(pDX, IDC_FIND_USER, m_btFindUser);
	DDX_Control(pDX, IDC_CHAT_OBJECT, m_ChatObject);
	DDX_Control(pDX, IDC_QUIT_ROOM, m_btQuitRoom);
	DDX_Control(pDX, IDC_HOR_SPLITER, m_HorSplitter);
}

//��ʼ������
BOOL CRoomViewItem::OnInitDialog()
{
	__super::OnInitDialog();

	//���ÿؼ�
	m_UserListView.InitUserListView();
	m_VorSplitter.SetSplitterSink(this);
	m_HorSplitter.SetSplitterSink(this);

	m_ChatInput.LimitText(MAX_CHAT_LEN-1);
	m_ChatMessage.SetBackgroundColor(FALSE,RGB(230,249,255));

	//�����ؼ�
	m_BrowerAD.Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this,100,NULL);
	m_BrowerAD.Navigate(TEXT("http://127.0.0.1:8086/AD/GameRoomAD.asp"),NULL,NULL,NULL,NULL);

	//���߿ؼ�
	m_PropertyBar.SetSkinResource(GetResInstanceHandle(),&g_GlobalUnits.m_PropertyViewImage);
	m_PropertyBar.CreateBar(this);
	m_PropertyBar.SetPurchaseInfoSink(this);

	//������ʾ�ؼ�
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(GetDlgItem(IDC_AUTO_SIT),TEXT("�Զ�Ѱ����Ϸλ��"));
	m_ToolTipCtrl.AddTool(GetDlgItem(IDC_FIND_USER),TEXT("�����û�"));
	m_ToolTipCtrl.AddTool(GetDlgItem(IDC_QUIT_ROOM),TEXT("�˳�����"));
	m_ToolTipCtrl.AddTool(GetDlgItem(IDC_EXPRESSION),TEXT("����"));
	m_ToolTipCtrl.AddTool(GetDlgItem(IDC_COLOR_SET),TEXT("��ɫ"));
	m_ToolTipCtrl.AddTool(GetDlgItem(IDC_CLEAN_SCREEN),TEXT("����"));
	m_ToolTipCtrl.AddTool(GetDlgItem(IDC_SEND_CHAT),TEXT("����"));
	m_ToolTipCtrl.AddTool(GetDlgItem(IDC_BUGLE),TEXT("С����"));

	//�������
	m_ChatInput.AddString(TEXT("�����ҵȵĻ���Ҳл��"));
	m_ChatInput.AddString(TEXT("��Һã��ܸ��˼�����λ��"));
	m_ChatInput.AddString(TEXT("��ô�ֶ����ˣ�����̫����..."));
	m_ChatInput.AddString(TEXT("��Ҫ�ߣ���ս��������"));
	m_ChatInput.AddString(TEXT("�����������̫�����..."));
	m_ChatInput.AddString(TEXT("���ǽ������ѣ��ܸ����������ϵ��ʽ��"));
	m_ChatInput.AddString(TEXT("����MM,����GG?"));
	m_ChatInput.AddString(TEXT("��㣬��������л�ˣ�"));
	m_ChatInput.AddString(TEXT("ʹ�죬ʹ�죬��ʮ��ţ�⣬����þƣ�"));
	m_ChatInput.AddString(TEXT("��Ҫ���ˣ���ʲô�ó���..."));
	m_ChatInput.AddString(TEXT("�ټ��ˣ��һ������ҵģ�BYE"));

	//�������
	m_ChatObject.AddString(TEXT("������"));
	m_ChatObject.SetCurSel(0);

	//���ñ���
	m_bInitDialog=true;

	//������Դ
	UpdateSkinResource();

	return TRUE;
}

//��Ϣ����
BOOL CRoomViewItem::PreTranslateMessage(MSG * pMsg)
{
	if((pMsg->message==WM_KEYDOWN)
		&& (pMsg->wParam==VK_RETURN)
		&& (m_ServiceStatus==ServiceStatus_Serviceing))
	{
		OnBnClickedSendChat();
		return TRUE;
	}
	m_ToolTipCtrl.RelayEvent(pMsg);
	
	return __super::PreTranslateMessage(pMsg);
}

//�˵�����
BOOL CRoomViewItem::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDM_UM_WISPER:			//˽����Ϣ
		{
			//Ѱ�����
			if (m_dwMenuUserID==0L) return TRUE;
			IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(m_dwMenuUserID);
			if (pIUserItem==NULL) return TRUE;

			//����˽��
			if (m_dwMenuUserID!=m_pMeUserItem->GetUserID()) SendUserWhisper(pIUserItem);
			
			return TRUE;
		}
	case IDM_UM_COPY_NAME:		//��������
		{
			//Ѱ�����
			if (m_dwMenuUserID==0L) return TRUE;
			IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(m_dwMenuUserID);
			if (pIUserItem==NULL) return TRUE;

			//�򿪼��а�
			if (OpenClipboard()==FALSE) return TRUE;
			if (EmptyClipboard()==FALSE) { CloseClipboard(); return TRUE; }

			//��������
			HANDLE hData=GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT,NAME_LEN);
			if (hData==NULL) 
			{
				CloseClipboard();
				return TRUE;
			}
			LPTSTR szMemName=(LPTSTR)GlobalLock(hData);
			lstrcpy(szMemName,pIUserItem->GetUserName());
			SetClipboardData(CF_TEXT,hData);
			GlobalUnlock(hData);
			CloseClipboard();

			//��ʾ����
			m_ChatInput.Paste();
			m_ChatInput.SetFocus();

			return TRUE;
		}
	case IDM_UM_SET_CHAT:		//��������
		{
			//Ѱ�����
			if (m_dwMenuUserID==0L) return TRUE;
			IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(m_dwMenuUserID);
			if (pIUserItem==NULL) return TRUE;

			//���ö���
			if (m_dwMenuUserID!=m_pMeUserItem->GetUserID())
			{
				SetChatObject(pIUserItem);
				m_ChatInput.SetFocus();
			}

			return TRUE;
		}
	case IDM_UM_CANECL_CHAT:	//ȡ������
		{
			//���ö���
			SetChatObject(NULL);
			m_ChatInput.SetFocus();

			return TRUE;
		}
	case IDM_UM_LOOK_LOCATION:	//�鿴λ��
		{
			//Ѱ�����
			if (m_dwMenuUserID==0L) return TRUE;
			IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(m_dwMenuUserID);
			if (pIUserItem==NULL) return TRUE;

			//��λλ��
			tagUserData * pUserData=pIUserItem->GetUserData();
			if ((pUserData->wTableID!=INVALID_TABLE)&&(pUserData->wChairID!=INVALID_CHAIR))
			{
				m_TableFrame.EnsureVisibleTable(pUserData->wTableID);
				if (pUserData->cbUserStatus==US_LOOKON) m_TableFrame.FlashTable(pUserData->wTableID);
				else m_TableFrame.FlashChair(pUserData->wTableID,pUserData->wChairID);
			}

			return TRUE;
		}
	case IDM_UM_SEARCH_USER:	//Ѱ�����
		{
			//�����ť
			OnBnClickedFindUser();

			return TRUE;
		}
	case IDM_UM_INVITE_PLAY:	//�������
		{
			//״̬�ж�
			if (m_ServiceStatus!=ServiceStatus_Serviceing) return TRUE;

			//������Ϣ
			tagUserData * pUserData=m_pMeUserItem->GetUserData();
			if (pUserData->cbUserStatus==US_PLAY) return TRUE;
			if (pUserData->wTableID==INVALID_TABLE) return TRUE;

			//Ѱ�����
			if (m_dwMenuUserID==0L) return TRUE;
			IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(m_dwMenuUserID);
			if (pIUserItem==NULL) return TRUE;

			//��������
			CMD_GR_UserInviteReq UserInviteReq;
			UserInviteReq.wTableID=pUserData->wTableID;
			UserInviteReq.dwUserID=pIUserItem->GetUserID();
			m_ClientSocket->SendData(MDM_GR_USER,SUB_GR_USER_INVITE_REQ,&UserInviteReq,sizeof(UserInviteReq));

			//��ʾ��Ϣ
			m_MessageProxyHelper->InsertSystemString(TEXT("�ɹ������û���������"),0);

			return TRUE;
		}
	case IDM_UM_SET_FRIEND:		//��Ϊ����
		{
			//Ѱ�����
			if (m_dwMenuUserID==0L) return TRUE;
			IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(m_dwMenuUserID);
			if (pIUserItem==NULL) return TRUE;

			//��������
			tagCompanionItem CompanionItem;
			memset(&CompanionItem,0,sizeof(CompanionItem));
			tagUserData * pUserData=pIUserItem->GetUserData();

			//���ù�ϵ
			CompanionItem.wFaceID=pUserData->wFaceID;
			CompanionItem.dwUserID=pUserData->dwUserID;
			CompanionItem.dwGameID=pUserData->dwGameID;
			if (pUserData->cbCompanion==enCompanion_Friend)
			{
				pUserData->cbCompanion=enCompanion_UnKnow;
				CompanionItem.Companion=enCompanion_UnKnow;
			}
			else
			{
				pUserData->cbCompanion=enCompanion_Friend;
				CompanionItem.Companion=enCompanion_Friend;
			}
			lstrcpyn(CompanionItem.szAccounts,pUserData->szName,CountArray(CompanionItem.szAccounts));
			g_GlobalUnits.m_CompanionManager->InsertCompanionItem(CompanionItem);
			((CGameFrame*)AfxGetMainWnd())->m_UserCompanionList.OnCompanionUpdate(&CompanionItem);

			//�ı�֪ͨ
			OnUserItemUpdate(pIUserItem);

			return TRUE;
		}
	case IDM_UM_SET_DETEST:		//��Ϊ���
		{
			//Ѱ�����
			if (m_dwMenuUserID==0L) return TRUE;
			IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(m_dwMenuUserID);
			if (pIUserItem==NULL) return TRUE;

			//��������
			tagCompanionItem CompanionItem;
			memset(&CompanionItem,0,sizeof(CompanionItem));
			tagUserData * pUserData=pIUserItem->GetUserData();

			//���ù�ϵ
			CompanionItem.wFaceID=pUserData->wFaceID;
			CompanionItem.dwUserID=pUserData->dwUserID;
			CompanionItem.dwGameID=pUserData->dwGameID;
			if (pUserData->cbCompanion==enCompanion_Detest)
			{
				pUserData->cbCompanion=enCompanion_UnKnow;
				CompanionItem.Companion=enCompanion_UnKnow;
			}
			else
			{
				pUserData->cbCompanion=enCompanion_Detest;
				CompanionItem.Companion=enCompanion_Detest;
			}
			lstrcpyn(CompanionItem.szAccounts,pUserData->szName,CountArray(CompanionItem.szAccounts));
			g_GlobalUnits.m_CompanionManager->InsertCompanionItem(CompanionItem);
			((CGameFrame*)AfxGetMainWnd())->m_UserCompanionList.OnCompanionUpdate(&CompanionItem);

			//�ı�֪ͨ
			OnUserItemUpdate(pIUserItem);

			//��ʾ��Ϣ
			if (pUserData->cbCompanion==enCompanion_Detest)
			{
				TCHAR szMessage[256]=TEXT("");
				_snprintf(szMessage,CountArray(szMessage),TEXT("[ %s ] ����Ϊ�����ң�ϵͳ����������һ��������Ϣ"),pUserData->szName);
				m_MessageProxyHelper->InsertSystemString(szMessage,0);
			}

			return TRUE;
		}
	case IDM_UM_SEND_WARNNING:	//���;���
		{
			//Ѱ�����
			if (m_dwMenuUserID==0L) return TRUE;
			IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(m_dwMenuUserID);
			if (pIUserItem==NULL) return TRUE;

			//Ȩ���ж�
			tagUserData * pMeUserData=m_pMeUserItem->GetUserData();
			if (CUserRight::CanSendWarning(pMeUserData->dwMasterRight)==false) return TRUE;

			//��������
			if (m_ManagerSendWarning.m_hWnd==NULL) 
			{
				m_ManagerSendWarning.Create(IDD_SEND_WARNING,this);
				m_ManagerSendWarning.SetClientUserManager(QUERY_OBJECT_INTERFACE(m_ClientUserManager,IUnknownEx));
				m_ManagerSendWarning.SetClientSocket(QUERY_OBJECT_PTR_INTERFACE(m_ClientSocket.GetInterface(),IUnknownEx));
			}

			//���ñ���
			m_ManagerSendWarning.SetUserItem(pIUserItem);

			//��ʾ����
			m_ManagerSendWarning.ShowWindow(SW_SHOW);
			m_ManagerSendWarning.BringWindowToTop();

			return TRUE;
		}
	case IDM_UM_LOOK_USER_IP:	//�鿴��ַ
		{
			//Ѱ�����
			if (m_dwMenuUserID==0L) return TRUE;
			IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(m_dwMenuUserID);
			if (pIUserItem==NULL) return TRUE;

			//Ȩ���ж�
			tagUserData * pMeUserData=m_pMeUserItem->GetUserData();
			if (CUserRight::CanSeeUserIP(pMeUserData->dwMasterRight)==false) return TRUE;

			//��������
			CMD_GR_LookUserIP LookUserIP;
			LookUserIP.dwTargetUserID=pIUserItem->GetUserID();
			m_ClientSocket->SendData(MDM_GR_MANAGER,SUB_GR_LOOK_USER_IP,&LookUserIP,sizeof(LookUserIP));

			return TRUE;
		}
	case IDM_UM_CUT_USER:		//�߳��û�
		{
			//Ѱ�����
			if (m_dwMenuUserID==0L) return TRUE;
			IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(m_dwMenuUserID);
			if (pIUserItem==NULL) return TRUE;

			//Ȩ���ж�
			tagUserData * pMeUserData=m_pMeUserItem->GetUserData();
			if (CUserRight::CanCutUser(pMeUserData->dwMasterRight)==false) return TRUE;

			//��������
			CMD_GR_KillUser KillUser;
			KillUser.dwTargetUserID=pIUserItem->GetUserID();
			m_ClientSocket->SendData(MDM_GR_MANAGER,SUB_GR_KILL_USER,&KillUser,sizeof(KillUser));

			return TRUE;
		}
	case IDM_UM_LIMIT_ACCOUN:	//�����ʻ�
		{
			//Ѱ�����
			if (m_dwMenuUserID==0L) return TRUE;
			IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(m_dwMenuUserID);
			if (pIUserItem==NULL) return TRUE;

			//Ȩ���ж�
			tagUserData * pMeUserData=m_pMeUserItem->GetUserData();
			if (CUserRight::CanForbidAccounts(pMeUserData->dwMasterRight)==false) return TRUE;

			//ȷ����ʾ
			TCHAR szMessage[128]=TEXT("");
			_snprintf(szMessage,CountArray(szMessage),TEXT("ȷʵҪ���� [ %s ] ���ʺ���"),pIUserItem->GetUserName());
			if (ShowMessageBox(szMessage,MB_ICONQUESTION|MB_YESNO)!=IDYES) return TRUE;

			//�û��ж�
			if (pIUserItem->GetUserID()!=m_dwMenuUserID) 
			{
				_snprintf(szMessage,CountArray(szMessage),TEXT("�û� [ %s ] �Ѿ��뿪����Ϸ���䣬�ʺŶ���ʧ��"),pIUserItem->GetUserName());
				m_MessageProxyHelper->InsertSystemString(szMessage,0);

				return TRUE;
			}

			//��������
			CMD_GR_LimitAccounts LimitAccounts;
			LimitAccounts.dwTargetUserID=pIUserItem->GetUserID();
			m_ClientSocket->SendData(MDM_GR_MANAGER,SUB_GR_LIMIT_ACCOUNS,&LimitAccounts,sizeof(LimitAccounts));

			return TRUE;
		}
	case IDM_UM_MANAGER_USER:	//Ȩ�޹���
		{
			//Ѱ�����
			if (m_dwMenuUserID==0L) return TRUE;
			IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(m_dwMenuUserID);
			if (pIUserItem==NULL) return TRUE;

			//��������
			if (m_ManagerUserRight.m_hWnd==NULL) 
			{
				m_ManagerUserRight.Create(IDD_MANAGER_RIGHT,this);
				m_ManagerUserRight.SetMasterRight(m_pMeUserItem->GetUserData()->dwMasterRight);
				m_ManagerUserRight.SetClientUserManager(QUERY_OBJECT_INTERFACE(m_ClientUserManager,IUnknownEx));
				m_ManagerUserRight.SetClientSocket(QUERY_OBJECT_PTR_INTERFACE(m_ClientSocket.GetInterface(),IUnknownEx));
			}

			//���ñ���
			m_ManagerUserRight.SetUserItem(pIUserItem);

			//��ʾ����
			m_ManagerUserRight.ShowWindow(SW_SHOW);
			m_ManagerUserRight.BringWindowToTop();

			return TRUE;
		}
	case IDM_UM_ISSUE_MESSAGE:	//������Ϣ
		{
			//Ȩ���ж�
			tagUserData * pMeUserData=m_pMeUserItem->GetUserData();
			if (CUserRight::CanIssueMessage(pMeUserData->dwMasterRight)==false) return TRUE;

			//��������
			if (m_ManagerMessage.m_hWnd==NULL) 
			{
				m_ManagerMessage.Create(IDD_MANAGER_MESSAGE,this);
				m_ManagerMessage.SetClientSocket(QUERY_OBJECT_PTR_INTERFACE(m_ClientSocket.GetInterface(),IUnknownEx));
			}

			//��ʾ����
			m_ManagerMessage.ShowWindow(SW_SHOW);
			m_ManagerMessage.BringWindowToTop();

			return TRUE;
		}
	case IDM_IM_SYSTEM_OPTION:	//�������
		{
			//Ȩ���ж�
			tagUserData * pMeUserData=m_pMeUserItem->GetUserData();
			if (CUserRight::CanServerOption(pMeUserData->dwMasterRight)==false) return TRUE;

			//��������
			if (m_ManagerServer.m_hWnd==NULL) 
			{
				m_ManagerServer.Create(IDD_MANAGER_SERVER,this);
				m_ManagerServer.SetClientSocket(QUERY_OBJECT_PTR_INTERFACE(m_ClientSocket.GetInterface(),IUnknownEx));
			}

			//��ʾ����
			m_ManagerServer.ShowWindow(SW_SHOW);
			m_ManagerServer.BringWindowToTop();

			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//�滭��Ϣ
void CRoomViewItem::OnPaint()
{
	CPaintDC dc(this);

	//�滭����
	DrawTableFrame(&dc);

	//�����ͼ
	if(!m_bHideUserList)DrawLeftViewFrame(&dc);

	return;
}

//�滭����
BOOL CRoomViewItem::OnEraseBkgnd(CDC * pDC)
{
	return TRUE;
}

//λ����Ϣ
void CRoomViewItem::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	//���ñ���
	m_bRectifyVor=false;

	//�����ؼ�
	RectifyControl(cx,cy);

	return;
}

//��ʾ��Ϣ
void CRoomViewItem::OnShowWindow(BOOL bShow, UINT nStatus)
{
	__super::OnShowWindow(bShow, nStatus);

	//���ñ���
	if (bShow) UpdateTitleText();

	return;
}

//˫����������
void CRoomViewItem::OnNMDblclkUserList(NMHDR * pNMHDR, LRESULT * pResult)
{
	NMITEMACTIVATE * pListNotify=(NMITEMACTIVATE *)pNMHDR;
	if ((pListNotify->iItem!=-1)&&(m_cbHideUserInfo==FALSE))
	{
		IUserItem * pIUserItem=(IUserItem *)m_UserListView.GetItemData(pListNotify->iItem);
		if ((pIUserItem!=NULL)&&(pIUserItem!=m_pMeUserItem)) SendUserWhisper(pIUserItem);
	}

	*pResult=0;
}

//�Ҽ�����û��б�
void CRoomViewItem::OnNMRclickUserList(NMHDR * pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pListNotify = (NMITEMACTIVATE *)pNMHDR;

	if (pListNotify->iItem != -1)
	{
		CPoint MousePoint;
		GetCursorPos(&MousePoint);

		IUserItem * pIUserItem=(IUserItem *)m_UserListView.GetItemData(pListNotify->iItem);
		if (pIUserItem!=NULL) ShowUserInfoMenu(pIUserItem,MousePoint);
	}

	*pResult=0;

	return ;
}

//����û��б�
void CRoomViewItem::OnNMclickUserList(NMHDR * pNMHDR, LRESULT * pResult)
{
	NMITEMACTIVATE * pListNotify = (NMITEMACTIVATE *)pNMHDR;

	if (pListNotify->iItem != -1)
	{
		CPoint MousePoint;
		GetCursorPos(&MousePoint);

		//ѡ���û�
		IUserItem * pIUserItem=(IUserItem *)m_UserListView.GetItemData(pListNotify->iItem);
		if (pIUserItem!=NULL) m_PropertyBar.SetUserDataInfo(pIUserItem->GetUserData());
	}

	*pResult=0;
	return ;
}

//����ͷ��
LRESULT CRoomViewItem::OnUpdateFace(WPARAM wParam, LPARAM lParam)
{
	DWORD dwUserID = DWORD(lParam);
	DWORD cbFaceVer = DWORD(wParam);

	//������Ϣ
	tagUserData *pUserData = m_ClientUserManager.SearchUserByUserID(dwUserID)->GetUserData();
	if(pUserData!=NULL)pUserData->dwCustomFaceVer = cbFaceVer;

	//����ͷ��
	if (pUserData->wTableID!=INVALID_TABLE)
	{
		IPC_UpdateFace UpdateFace;
		memset(&UpdateFace,0,sizeof(UpdateFace));
		UpdateFace.dwCustomFace = cbFaceVer;
		SendProcessData(IPC_MAIN_USER,IPC_SUB_UPDATE_FACE,&UpdateFace,sizeof(UpdateFace));
	}

	return 0;
}

//IPC ��Ϣ
BOOL CRoomViewItem::OnCopyData(CWnd *pWnd, COPYDATASTRUCT *pCopyDataStruct)
{
	if (m_IPCRecvCopyData.OnCopyData(pWnd->GetSafeHwnd(), pCopyDataStruct)==true) return TRUE;
	return __super::OnCopyData(pWnd, pCopyDataStruct);
}

//������Ϣ
LRESULT CRoomViewItem::OnHitExpression(WPARAM wParam, LPARAM lParam)
{
	CExpressionItem * pExpressionItem=m_Expression.GetExpressItem((WORD)wParam);
	if (pExpressionItem!=NULL)
	{
		CString strBuffer;
		m_ChatInput.GetWindowText(strBuffer);
		strBuffer+=pExpressionItem->m_szTrancelate;
		m_ChatInput.SetWindowText(strBuffer);
		m_ChatInput.SetFocus();
		m_ChatInput.SetEditSel(strBuffer.GetLength(),strBuffer.GetLength());
	}

	return 0;
}

//���Ͱ�ť
void CRoomViewItem::OnBnClickedSendChat()
{
	//������Ϣ
	TCHAR szChatMessage[MAX_CHAT_LEN]=TEXT("");
	m_ChatInput.GetWindowText(szChatMessage,CountArray(szChatMessage));
	if (szChatMessage[0]!=0)
	{
		//��ʱ����
		DWORD dwCurTimes = GetTickCount();
		if(m_dwLimitChatTimes==0 || dwCurTimes-m_dwLimitChatTimes>=LIMIT_CHAT_TIMES)
		{
			m_dwLimitChatTimes = dwCurTimes;
			m_ChatInput.SetWindowText(TEXT(""));
			SendChatPacket(m_dwChatUserID,szChatMessage,g_GlobalOption.m_crChatTX);
		}
	}

	//���ý���
	m_ChatInput.SetFocus();

	return;
}

//���鰴ť
void CRoomViewItem::OnBnClickedExpression()
{
	//�������鴰��
	if (m_Expression.GetSafeHwnd()==NULL) m_Expression.CreateExpression(this);

	//�ƶ�����
	CRect rcButton;
	CSize ExpressSize;
	m_Expression.GetFixSize(ExpressSize);
	m_btExpression.GetWindowRect(&rcButton);
	m_Expression.MoveWindow(rcButton.right-ExpressSize.cx,rcButton.top-ExpressSize.cy,ExpressSize.cx,ExpressSize.cy);
	m_Expression.ShowWindow(SW_SHOW);
	m_Expression.SetFocus();

	return;
}

//��ɫ��ť
void CRoomViewItem::OnBnClickedColorSet()
{
	//������ɫ
	CColorDialog ColorDialog(g_GlobalOption.m_crChatTX,CC_FULLOPEN);
	if (ColorDialog.DoModal()==IDOK) g_GlobalOption.m_crChatTX = ColorDialog.GetColor();

	//���ý���
	m_ChatInput.SetFocus();

	return;
}

//������ť
void CRoomViewItem::OnBnClickedCleanScreen()
{
	m_MessageProxyHelper->CleanScreen();
	return;
}

//���Ȱ�ť
void CRoomViewItem::OnBnClickedBugle()
{
	if(m_ClientSocket.GetInterface()==NULL)return;

	//��ȡ�û�
	tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
	IUserItem *pMeUserItem = m_ClientUserManager.SearchUserByUserID(GlobalUserData.dwUserID);
	tagUserData *pMeUserData = pMeUserItem->GetUserData();

	//���öԻ���
	ShowBugleDlg(NULL,m_ClientSocket.GetInterface(),pMeUserData);

	return ;
}

//������Ϣ
tagGameKind * __cdecl CRoomViewItem::GetKindInfo()
{
	if (m_pListServer!=NULL)
	{
		CListKind * pListKind=m_pListServer->GetListKind();
		ASSERT(pListKind!=NULL);
		if (pListKind!=NULL)
		{
			return pListKind->GetItemInfo();
		}
	}
	return NULL;
}

//���а�ť
void CRoomViewItem::OnBnClickedBank()
{
	//��ȡ�û�
	tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
	IUserItem *pMeUserItem = m_ClientUserManager.SearchUserByUserID(GlobalUserData.dwUserID);
	tagUserData *pMeUserData = pMeUserItem->GetUserData();

	//��ʾ����
	ShowBankStorageDlg(NULL,m_ClientSocket.GetInterface(),pMeUserData);

	return;
}

//���밴ť
void CRoomViewItem::OnBnClickedAutoSit()
{
	//�ж�״̬
	if (m_ServiceStatus!=ServiceStatus_Serviceing) return;

	if ((m_wReqTableID!=INVALID_TABLE)||(m_wReqChairID!=INVALID_CHAIR))
	{
		TCHAR szMessage[]=TEXT("�ϴ�����û���յ���������Ӧ,���Ժ�");
		ShowMessageBox(szMessage,MB_ICONINFORMATION);
		return;
	}

	//�ж�״̬
	tagUserData * pUserData=m_pMeUserItem->GetUserData();
	if (pUserData->cbUserStatus==US_PLAY)
	{
		TCHAR szMessage[]=TEXT("��������Ϸ�У����ܻ�λ�ã����Ƚ�����Ϸ��");
		ShowMessageBox(szMessage,MB_ICONINFORMATION);
		return;
	}

	//�����׷�
	if(m_cbHideUserInfo==TRUE)
	{
		//ģ�����
		m_TableFrame.EnsureVisibleTable(0);
		m_TableFrame.FlashChair(0,rand()%m_TableFrame.GetChairCount());

		//�Զ�����
		SendSitDownPacket(INVALID_TABLE,INVALID_CHAIR,NULL);
		return;
	}

	//���������Ӳ�ȫ�յ�
	tagFindTable FindInfo;
	FindInfo.bAllNull=false;
	FindInfo.bFilterPass=true;
	FindInfo.bNotFull=true;
	FindInfo.bOneNull=true;
	FindInfo.bTwoNull=(m_TableFrame.GetChairCount()!=2);
	FindInfo.wBeginTableID=m_wFindTableID+1;
	FindInfo.wResultTableID=INVALID_TABLE;
	FindInfo.wResultChairID=INVALID_CHAIR;
	bool bSuccess=FindGameTable(FindInfo);
	m_wFindTableID=FindInfo.wResultTableID;

	//����ȫ����Ϸ��
	if (bSuccess==false)
	{
		FindInfo.bAllNull=true;
		FindInfo.bFilterPass=true;
		FindInfo.bNotFull=true;
		FindInfo.bOneNull=true;
		FindInfo.bTwoNull=true;
		FindInfo.wBeginTableID=m_wFindTableID+1;
		FindInfo.wResultTableID=INVALID_TABLE;
		FindInfo.wResultChairID=INVALID_CHAIR;
		bSuccess=FindGameTable(FindInfo);
		m_wFindTableID=FindInfo.wResultTableID;
	}

	//����ж�
	if (bSuccess==true)
	{
		//Ч������
		ASSERT(FindInfo.wResultTableID!=INVALID_TABLE);
		ASSERT(FindInfo.wResultChairID!=INVALID_CHAIR);

		//��������
		WORD wChairID=FindInfo.wResultChairID;
		m_TableFrame.EnsureVisibleTable(m_wFindTableID);
		m_TableFrame.FlashChair(m_wFindTableID,wChairID);

		//�Զ�����
		PerformSitDownAction(m_wFindTableID,wChairID);
	}
	else
	{
		TCHAR szMessage[]=TEXT("�Ҳ���������������Ϸ����");
		ShowMessageBox(szMessage,MB_ICONINFORMATION);
	}

	return;
}

//���Ұ�ť
void CRoomViewItem::OnBnClickedFindUser()
{
	if (m_pFindUserDlg==NULL) 
	{
		m_pFindUserDlg=new CFindUserDlg(this,0);
		m_pFindUserDlg->m_UserList.m_wGameGenre=m_wGameGenre;
		m_pFindUserDlg->m_UserList.m_wKindID=m_pListServer->GetItemInfo()->wKindID;
	}
	if (m_pFindUserDlg->GetSafeHwnd()==NULL)
	{
		m_pFindUserDlg->Create(IDD_FIND_USER,this);
		m_pFindUserDlg->m_UserList.SetColumnDescribe(m_ListColumnInfo.ColumnItem,m_ListColumnInfo.wColumnCount);
	}
	m_pFindUserDlg->ShowWindow(SW_SHOW);

	return;
}

//�˳���ť
void CRoomViewItem::OnBnClickedQuitRoom()
{
	((CGameFrame*)AfxGetMainWnd())->CloseRoomViewItem(this);
	return;
}

//������Ϣ
tagGameServer * __cdecl CRoomViewItem::GetServerInfo()
{
	if (m_pListServer==NULL) return NULL;
	return m_pListServer->GetItemInfo();
}

//�ϰ��¼�
void CRoomViewItem::NotifyBossCome(bool bBossCome)
{
	//������Ϣ
	if (m_pShareMemory==NULL) return;
	if (bBossCome==true) ::ShowWindow(m_pShareMemory->hWndGameFrame,SW_HIDE);
	else 
	{
		::ShowWindow(m_pShareMemory->hWndGameFrame,SW_SHOW);
	}

	return;
}

//�ر�ѯ��
bool __cdecl CRoomViewItem::QueryCloseRoom()
{
	//״̬�ж�
	if (m_pMeUserItem==NULL) return true;
	if (m_ServiceStatus!=ServiceStatus_Serviceing) return true;
	if (m_pMeUserItem->GetUserData()->cbUserStatus!=US_PLAY) return true;

#ifndef _DEBUG

	//��ʾ��Ϣ
	int iRet=ShowInformation(TEXT("��������Ϸ�У�ǿ���˳������۷֣�ȷʵҪǿ����"),0,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2);

	//�����ж�
	if (m_pMeUserItem==NULL) return true;
	if (m_ServiceStatus!=ServiceStatus_Serviceing) return true;
	if (m_pMeUserItem->GetUserData()->cbUserStatus!=US_PLAY) return true;

	//����ж�
	if (iRet==IDYES)
	{
		SendLeftGamePacket();
		return true;
	}

	return false;

#else

	return true;

#endif

}

//�ر�֪ͨ
void __cdecl CRoomViewItem::CloseRoomViewItem()
{
	//�ر�����
	if (m_ClientSocket.GetInterface()!=NULL) m_ClientSocket->CloseSocket();

	//�ر���Ϸ
	CloseGameClient();

	//�����ڴ�
	if (m_pShareMemory!=NULL)
	{
		UnmapViewOfFile(m_pShareMemory);
		m_pShareMemory=NULL;
	}
	if (m_hShareMemory!=NULL)
	{
		CloseHandle(m_hShareMemory);
		m_hShareMemory=NULL;
	}

	//�رմ���
	DestroyWindow();

	return;
}

//��ť��Ϣ
bool CRoomViewItem::OnSplitterButton(CSkinSplitter * pSkinSplitter, CSkinButton * pSplitterButton)
{
	//���ñ���
	m_bHideUserList=!m_bHideUserList;

	//���ư�ť
	UINT uControlId = ((!m_bHideUserList)?GetServerViewRes().uBtHideUserList:GetServerViewRes().uBtShowUserList);
	m_VorSplitter.ShowSplitterButton(uControlId,GetResInstanceHandle());

	//��������
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());

	return true;
}

//�������Ϣ
bool CRoomViewItem::OnSplitterEvent(CSkinSplitter * pSkinSplitter, UINT uSplitID, int nXPos, int nYPos)
{
	//��ʾ�ؼ�
	if (m_bHideUserList==true)
	{
		//���ñ���
		m_bRectifyHor=true;
		m_bHideUserList=false;
	
		//���ð�ť
		UINT uControlId = ((!m_bHideUserList)?GetServerViewRes().uBtHideUserList:GetServerViewRes().uBtShowUserList);
		m_VorSplitter.ShowSplitterButton(uControlId,GetResInstanceHandle());
	}

	//��������
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());

	return true;
}

//��ʼ��
bool CRoomViewItem::InitRoomViewItem(CListServer * pListServer)
{
	//���ñ���
	ASSERT(pListServer!=NULL);
	m_pListServer=pListServer;

	//��Ϣ����
	if (m_MessageProxyHelper.CreateInstance()==false) return false;
	if (m_MessageProxyHelper->SetRichEditHwnd(&m_ChatMessage)==false) return false;

	//�������
	IUnknownEx * pIUnknownEx=QUERY_ME_INTERFACE(IUnknownEx);
	if (m_UserListView.m_GameRankManager.CreateInstance()==false) return false;
	if (m_ClientSocket.CreateInstance()==false) return false;
	if (m_ClientSocket->SetTCPSocketSink(pIUnknownEx)==false) return false;
	if (m_ClientUserManager.SetUserManagerSink(pIUnknownEx)==false) return false;
	if (m_IPCRecvCopyData.SetChannelMessageSink(pIUnknownEx)==false) return false;

	//������ɫ
	tagMessageOption MessageOption;
	memset(&MessageOption,0,sizeof(MessageOption));
	MessageOption.crName=g_GlobalOption.m_crMsgName;
	MessageOption.crSystemHead=g_GlobalOption.m_crMsgSysHead;
	MessageOption.crSystemString=g_GlobalOption.m_crMsgSysString;
	m_MessageProxyHelper->SetMessageOption(MessageOption);

	//���������ڴ�
	tagGameServer * pGameServer=m_pListServer->GetItemInfo();
	_snprintf(m_szShareName,sizeof(m_szShareName),TEXT("0x8%ld%ld%ld%ld"),pGameServer->dwServerAddr,pGameServer->wServerPort,time(NULL),rand()%100);
	m_hShareMemory=(HANDLE)CreateFileMapping((HANDLE)0xFFFFFFFFFFFF,NULL,PAGE_READWRITE,0,sizeof(tagShareMemory),m_szShareName);
	if ((m_hShareMemory==NULL)||(GetLastError()==ERROR_ALREADY_EXISTS)) return false;
	m_pShareMemory=(tagShareMemory *)MapViewOfFile(m_hShareMemory,FILE_MAP_ALL_ACCESS,0,0,0);

	//���ù����ڴ�
	memset(m_pShareMemory,0,sizeof(tagShareMemory));
	m_pShareMemory->wDataSize=sizeof(tagShareMemory);
	m_pShareMemory->hWndGameServer=m_hWnd;
	m_pShareMemory->hWndGamePlaza=AfxGetMainWnd()->m_hWnd;

	//������Ϣ
	m_pServerOption=g_GlobalOption.GetServerOption(pListServer);
	m_pGameOption=g_GlobalOption.GetGameOption(pListServer->GetListKind());
	ASSERT(m_pGameOption!=NULL);
	ASSERT(m_pServerOption!=NULL);

	//���Ա���
	m_PropertyAttributeArrary.RemoveAll();
	m_FlowerAttributeArrary.RemoveAll();

	return true;
}

//���ӷ�����
bool CRoomViewItem::ConnectGameServer()
{
	m_ServiceStatus=ServiceStatus_Connecting;
	tagGameServer * pGameServer=m_pListServer->GetItemInfo();
	return (m_ClientSocket->Connect(pGameServer->dwServerAddr,pGameServer->wServerPort)==CONNECT_SUCCESS);
}

//���͹���
bool CRoomViewItem::SendUserRule()
{
	if (m_ServiceStatus!=ServiceStatus_Serviceing) return false;
	return SendUserRulePacket();
}

//˽����Ϣ
bool CRoomViewItem::SendUserWhisper(IUserItem * pIUserItem)
{
	//��ȡ�û�
	ASSERT(pIUserItem!=NULL);
	if (pIUserItem==NULL) return false;

	//�����
	CShortMessage *pShortMessageWnd = ActiveShortWnd(pIUserItem->GetUserData()->dwUserID,pIUserItem,true);
	if (pShortMessageWnd==NULL)	ShowMessageBox(TEXT("����Ϣ����̫���ˣ���رղ��ֶ���Ϣ�������ԣ�"),MB_ICONINFORMATION);

	return true;
}

//�û��˵�
bool CRoomViewItem::ShowUserInfoMenu(IUserItem * pIUserItem, CPoint Point)
{
	//�ж�״̬
	if (m_ServiceStatus!=ServiceStatus_Serviceing) return false;

	//�û��ж�
	if (pIUserItem==NULL) 
	{
		m_MessageProxyHelper->InsertGeneralString(TEXT("���û��Ѿ��뿪������ "),RGB(0,0,0),MS_NORMAL,true);
		return false;
	}

	//����˵�
	CMenu UserInfoMenu;
	if (UserInfoMenu.CreatePopupMenu()==FALSE) return false;

	//��ȡ���
	tagUserData * pUserData=pIUserItem->GetUserData();
	tagUserData * pMeUserData=m_pMeUserItem->GetUserData();

	//�û���Ϣ
	WORD wTableID=pUserData->wTableID;
	WORD wChiarID=pUserData->wChairID;
	BYTE cbUserStatus=pUserData->cbUserStatus;

	//�����Ϣ
	WORD wMeTableID=pMeUserData->wTableID;
	WORD wMeChiarID=pMeUserData->wChairID;
	BYTE cbMeUserStatus=pMeUserData->cbUserStatus;

	//���ñ���
	TCHAR szBuffer[256]=TEXT("");
	m_dwMenuUserID=pUserData->dwUserID;
	bool bMySelf=(pIUserItem==m_pMeUserItem);

	//���ò˵�
	if ((bMySelf==false)&&(m_cbHideUserInfo==FALSE)) 
	{
		UserInfoMenu.AppendMenu(0,IDM_UM_WISPER,TEXT("����˽����Ϣ..."));
		UserInfoMenu.SetDefaultItem(IDM_UM_WISPER,MF_BYCOMMAND);
	}
	UserInfoMenu.AppendMenu(0,IDM_UM_COPY_NAME,TEXT("�����û���"));

	//�������
	if ((bMySelf==false)&&(m_cbHideUserInfo==FALSE)&&(m_dwChatUserID!=pUserData->dwUserID))
	{
		_snprintf(szBuffer,CountArray(szBuffer),TEXT("�� [ %s ] ��̸"),pUserData->szName);
		UserInfoMenu.AppendMenu(0,IDM_UM_SET_CHAT,szBuffer);
	}
	if (m_szChatName[0]!=0)
	{
		_snprintf(szBuffer,CountArray(szBuffer),TEXT("ȡ���� [ %s ] ��̸"),m_szChatName);
		UserInfoMenu.AppendMenu(0,IDM_UM_CANECL_CHAT,szBuffer);
	}

	//�鿴λ��
	UserInfoMenu.AppendMenu(MF_SEPARATOR);
	if (bMySelf==false) UserInfoMenu.AppendMenu(0,IDM_UM_LOOK_LOCATION,TEXT("�鿴��ҵ�λ��"));
	else UserInfoMenu.AppendMenu(0,IDM_UM_LOOK_LOCATION,TEXT("�鿴�ҵ�λ��"));
	if (pUserData->wTableID==INVALID_TABLE) UserInfoMenu.EnableMenuItem(IDM_UM_LOOK_LOCATION,MF_BYCOMMAND|MF_GRAYED);

	//������Ϸ
	if ((bMySelf==false)&&(m_cbHideUserInfo==FALSE)&&(cbMeUserStatus!=US_PLAY)&&(wMeTableID!=INVALID_TABLE)&&(cbUserStatus<US_PLAY))
	{
		_snprintf(szBuffer,CountArray(szBuffer),TEXT("���� [ %s ] һ����Ϸ"),pUserData->szName);
		UserInfoMenu.AppendMenu(0,IDM_UM_INVITE_PLAY,szBuffer);
	}

	//���ܲ˵�
	UserInfoMenu.AppendMenu(MF_SEPARATOR);
	UserInfoMenu.AppendMenu(0,IDM_UM_SEARCH_USER,TEXT("Ѱ�����..."));
	if ((bMySelf==false)&&(m_cbHideUserInfo==FALSE))
	{
		UserInfoMenu.AppendMenu(0,IDM_UM_SET_FRIEND,TEXT("��Ϊ����"));
		UserInfoMenu.AppendMenu(0,IDM_UM_SET_DETEST,TEXT("��Ϊ���"));
		if (pUserData->cbCompanion==enCompanion_Friend) 
		{
			UserInfoMenu.CheckMenuItem(IDM_UM_SET_FRIEND,MF_BYCOMMAND|MF_CHECKED);
		}
		if (pUserData->cbCompanion==enCompanion_Detest) 
		{
			UserInfoMenu.CheckMenuItem(IDM_UM_SET_DETEST,MF_BYCOMMAND|MF_CHECKED);
		}
	}

	//�����Ϣ
	double dFleeRate=0.0;
	LONG lPlayCount=pIUserItem->GetUserPlayCount();
	if (pUserData->lFleeCount>0)
	{
		dFleeRate=(double)(pUserData->lFleeCount*100)/(double)lPlayCount;
		if (dFleeRate<0.01) dFleeRate=0.0;
	}

	//�˵��ָ�
	UserInfoMenu.AppendMenu(0,MF_SEPARATOR);

	//�������
	LPCTSTR pszMember[]={TEXT("��ͨ��Ա"),TEXT("�м���Ա"),TEXT("�߼���Ա")};
	if ((m_cbHideUserInfo==FALSE)&&(pUserData->cbMemberOrder>0)&&(pUserData->cbMemberOrder<CountArray(pszMember)))
	{
		BYTE cbMemberOrder=pUserData->cbMemberOrder;
		_snprintf(szBuffer,CountArray(szBuffer),TEXT("�û�����%s [ %s ]"),pUserData->szName,pszMember[cbMemberOrder]);
	}
	else _snprintf(szBuffer,CountArray(szBuffer),TEXT("�û�����%s"),pUserData->szName);
	UserInfoMenu.AppendMenu(0,0,szBuffer);

	//��Ϸ ID
	_snprintf(szBuffer,CountArray(szBuffer),TEXT("��Ϸ ID��%ld"),pUserData->dwGameID);
	UserInfoMenu.AppendMenu(0,0,szBuffer);
	
	//����ǩ��
	if (pUserData->szUnderWrite[0]!=0)
	{
		_snprintf(szBuffer,CountArray(szBuffer),TEXT("����ǩ����%s"),pUserData->szUnderWrite);
		UserInfoMenu.AppendMenu(0,0,szBuffer);
	}

	//�˵��ָ�
	UserInfoMenu.AppendMenu(0,MF_SEPARATOR);

	//������ֵ
	_snprintf(szBuffer,CountArray(szBuffer),TEXT("����ֵ��%ld"),pUserData->lExperience);
	UserInfoMenu.AppendMenu(0,0,szBuffer);
	
	//���λ��
	if (pUserData->wTableID!=INVALID_TABLE)
	{
		_snprintf(szBuffer,CountArray(szBuffer),TEXT("���ţ�%d"),pUserData->wTableID+1);
		UserInfoMenu.AppendMenu(0,0,szBuffer);
	}

	//��Ӯ��Ϣ
	_snprintf(szBuffer,CountArray(szBuffer),TEXT("�ܾ�����%ld  Ӯ�֣�%ld  ��֣�%ld  �;֣�%ld"),lPlayCount,pUserData->lWinCount,pUserData->lLostCount,pUserData->lDrawCount);
	UserInfoMenu.AppendMenu(0,0,szBuffer);

	//������Ϣ
	if (m_wGameGenre==GAME_GENRE_GOLD)
	{
		_snprintf(szBuffer,CountArray(szBuffer),TEXT("��Ϸ�ң�%ld  �����ʣ�%5.2f%%"),pUserData->lScore,dFleeRate);
			UserInfoMenu.AppendMenu(0,0,szBuffer);
	}
	else
	{
		_snprintf(szBuffer,CountArray(szBuffer),TEXT("���֣�%ld  �ܾ�����%ld  �����ʣ�%5.2f%%"),pUserData->lScore,lPlayCount,dFleeRate);
			UserInfoMenu.AppendMenu(0,0,szBuffer);
	}

	//������Ϣ
	if (pUserData->szGroupName[0]!=0)
	{
		_snprintf(szBuffer,CountArray(szBuffer),TEXT("���ţ�%s"),pUserData->szGroupName);
		UserInfoMenu.AppendMenu(0,0,szBuffer);
	}

	//�û�����
	if ((pMeUserData->dwUserID!=m_dwMenuUserID)&&(pMeUserData->dwMasterRight!=0L))
	{
		//����˵�
		UserInfoMenu.AppendMenu(0,MF_SEPARATOR);
		UserInfoMenu.AppendMenu(0,IDM_UM_SEND_WARNNING,TEXT("���;�����Ϣ..."));
		UserInfoMenu.AppendMenu(0,IDM_UM_LOOK_USER_IP,TEXT("�鿴�û� IP..."));
		UserInfoMenu.AppendMenu(0,IDM_UM_CUT_USER,TEXT("���û�����..."));
		UserInfoMenu.AppendMenu(0,IDM_UM_LIMIT_ACCOUN,TEXT("�����û��ʺ�..."));
		UserInfoMenu.AppendMenu(0,IDM_UM_MANAGER_USER,TEXT("�û�Ȩ������..."));

		//��ֹ�˵�
		DWORD dwMasterRight=pMeUserData->dwMasterRight;
		if (CUserRight::CanSendWarning(dwMasterRight)==false) UserInfoMenu.EnableMenuItem(IDM_UM_SEND_WARNNING,MF_BYCOMMAND|MF_GRAYED);
		if (CUserRight::CanSeeUserIP(dwMasterRight)==false) UserInfoMenu.EnableMenuItem(IDM_UM_LOOK_USER_IP,MF_BYCOMMAND|MF_GRAYED);
		if (CUserRight::CanCutUser(dwMasterRight)==false) UserInfoMenu.EnableMenuItem(IDM_UM_CUT_USER,MF_BYCOMMAND|MF_GRAYED);
		if (CUserRight::CanForbidAccounts(dwMasterRight)==false) UserInfoMenu.EnableMenuItem(IDM_UM_LIMIT_ACCOUN,MF_BYCOMMAND|MF_GRAYED);
	}

	//�������
	if (pMeUserData->dwMasterRight!=0L)
	{
		//����˵�
		UserInfoMenu.AppendMenu(0,MF_SEPARATOR);
		UserInfoMenu.AppendMenu(0,IDM_UM_ISSUE_MESSAGE,TEXT("����ϵͳ��Ϣ..."));
		UserInfoMenu.AppendMenu(0,IDM_IM_SYSTEM_OPTION,TEXT("��Ϸ��������..."));

		//��ֹ�˵�
		DWORD dwMasterRight=pMeUserData->dwMasterRight;
		if (CUserRight::CanIssueMessage(dwMasterRight)==false) UserInfoMenu.EnableMenuItem(IDM_UM_ISSUE_MESSAGE,MF_BYCOMMAND|MF_GRAYED);
		if (CUserRight::CanServerOption(dwMasterRight)==false) UserInfoMenu.EnableMenuItem(IDM_IM_SYSTEM_OPTION,MF_BYCOMMAND|MF_GRAYED);
	}

	//��ʾ�˵�
	UserInfoMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,Point.x,Point.y,this);

	return true;
}

//��Դ���
HINSTANCE CRoomViewItem::GetResInstanceHandle()
{
	return g_GlobalUnits.m_PlatformResourceModule->GetResInstance();
}

//��ȡ��Դ
tagServerViewImage CRoomViewItem::GetServerViewRes()
{
	return g_GlobalUnits.m_ServerViewImage;
}

//������Դ
bool CRoomViewItem::UpdateSkinResource()
{
	//��ȡ��Դ
	HINSTANCE hInstance=GetResInstanceHandle();

	//�б�����
	m_EncircleList.ImageML.SetLoadInfo(GetServerViewRes().uUserListML,hInstance);
	m_EncircleList.ImageMR.SetLoadInfo(GetServerViewRes().uUserListMR,hInstance);
	m_EncircleList.ImageTL.SetLoadInfo(GetServerViewRes().uUserListTL,hInstance);
	m_EncircleList.ImageTM.SetLoadInfo(GetServerViewRes().uUserListTM,hInstance);
	m_EncircleList.ImageTR.SetLoadInfo(GetServerViewRes().uUserListTR,hInstance);
	m_EncircleList.ImageBL.SetLoadInfo(GetServerViewRes().uUserListBL,hInstance);
	m_EncircleList.ImageBM.SetLoadInfo(GetServerViewRes().uUserListBM,hInstance);
	m_EncircleList.ImageBR.SetLoadInfo(GetServerViewRes().uUserListBR,hInstance);

	//��������
	m_EncircleChat.ImageML.SetLoadInfo(GetServerViewRes().uChatML,hInstance);
	m_EncircleChat.ImageMR.SetLoadInfo(GetServerViewRes().uChatMR,hInstance);
	m_EncircleChat.ImageTL.SetLoadInfo(GetServerViewRes().uChatTL,hInstance);
	m_EncircleChat.ImageTM.SetLoadInfo(GetServerViewRes().uChatTM,hInstance);
	m_EncircleChat.ImageTR.SetLoadInfo(GetServerViewRes().uChatTR,hInstance);
	m_EncircleChat.ImageBL.SetLoadInfo(GetServerViewRes().uChatBL,hInstance);
	m_EncircleChat.ImageBM.SetLoadInfo(GetServerViewRes().uChatBM,hInstance);
	m_EncircleChat.ImageBR.SetLoadInfo(GetServerViewRes().uChatBR,hInstance);

	//��������
	m_EncircleTable.ImageML.SetLoadInfo(GetServerViewRes().uTableML,hInstance);
	m_EncircleTable.ImageMR.SetLoadInfo(GetServerViewRes().uTableMR,hInstance);
	m_EncircleTable.ImageTL.SetLoadInfo(GetServerViewRes().uTableTL,hInstance);
	m_EncircleTable.ImageTM.SetLoadInfo(GetServerViewRes().uTableTM,hInstance);
	m_EncircleTable.ImageTR.SetLoadInfo(GetServerViewRes().uTableTR,hInstance);
	m_EncircleTable.ImageBL.SetLoadInfo(GetServerViewRes().uTableBL,hInstance);
	m_EncircleTable.ImageBM.SetLoadInfo(GetServerViewRes().uTableBM,hInstance);
	m_EncircleTable.ImageBR.SetLoadInfo(GetServerViewRes().uTableBR,hInstance);
	
	//��ȡ��Ϣ
	CSkinAide::GetEncircleInfo(m_ImageInfoChat,m_EncircleChat);
	CSkinAide::GetEncircleInfo(m_ImageInfoList,m_EncircleList);
	CSkinAide::GetEncircleInfo(m_ImageInfoTable,m_EncircleTable);

	//��ֿؼ�
	UINT uControlId = ((!m_bHideUserList)?GetServerViewRes().uBtHideUserList:GetServerViewRes().uBtShowUserList);
	m_VorSplitter.ShowSplitterButton(uControlId,GetResInstanceHandle());

	//���ð�ť
	m_btAutoSit.SetButtonImage(GetServerViewRes().uBtAutoJoin,hInstance,false);
	m_btBank.SetButtonImage(GetServerViewRes().uBtBank,hInstance,false);
	m_btFindUser.SetButtonImage(GetServerViewRes().uBtSearchUser,hInstance,false);
	m_btQuitRoom.SetButtonImage(GetServerViewRes().uBtQuitRoom,hInstance,false);
	//m_btPhrase.SetButtonImage(GetServerViewRes().uBtCleanScreen,hInstance,false);
	m_btExpression.SetButtonImage(GetServerViewRes().uBtExpression,hInstance,false);
	m_btColorSet.SetButtonImage(GetServerViewRes().uBtColorSet,hInstance,false);
	m_btCleanScreen.SetButtonImage(GetServerViewRes().uBtCleanScreen,hInstance,false);
	m_btSendChat.SetButtonImage(GetServerViewRes().uBtSendChat,hInstance,false);
	m_btBugle.SetButtonImage(GetServerViewRes().uBtBugle,hInstance,false);

	//���µ���
	m_PropertyBar.UpdateSkinResource();

	return true;
}

//���͵�¼��
bool CRoomViewItem::SendLogonPacket()
{
	//��ȡ��Ϣ
	BYTE cbBuffer[SOCKET_PACKET];
	tagGlobalUserData & GlobalUserData=g_GlobalUnits.GetGolbalUserData();

	//��¼���ݰ�
	CMD_GR_LogonByUserID * pLogonByUserID=(CMD_GR_LogonByUserID *)cbBuffer;
	pLogonByUserID->dwUserID=GlobalUserData.dwUserID;
	pLogonByUserID->dwPlazaVersion=g_GlobalUnits.GetPlazaVersion();
	lstrcpyn(pLogonByUserID->szPassWord,GlobalUserData.szPassWord,sizeof(pLogonByUserID->szPassWord));

	//�������к�
	tagClientSerial ClientSerial;
	g_GlobalUnits.GetClientSerial(ClientSerial);

	//�������ݰ�
	CSendPacketHelper Packet(cbBuffer+sizeof(CMD_GR_LogonByUserID),sizeof(cbBuffer)-sizeof(CMD_GR_LogonByUserID));
	Packet.AddPacket(&ClientSerial,sizeof(ClientSerial),DTP_COMPUTER_ID);
	m_ClientSocket->SendData(MDM_GR_LOGON,SUB_GR_LOGON_USERID,cbBuffer,sizeof(CMD_GR_LogonByUserID)+Packet.GetDataSize());

	return true;
}

//������������
bool CRoomViewItem::SendOptionsPacket()
{
	return true;
}

//������������
bool CRoomViewItem::SendStandUpPacket()
{
	m_ClientSocket->SendData(MDM_GR_USER,SUB_GR_USER_STANDUP_REQ);

	return true;
}

//����ǿ������
bool CRoomViewItem::SendLeftGamePacket()
{
	m_ClientSocket->SendData(MDM_GR_USER,SUB_GR_USER_LEFT_GAME_REQ);

	return true;
}

//���ͷ������
bool CRoomViewItem::SendUserRulePacket()
{
	//�������ݰ�
	CMD_GR_UserRule UserRule;
	UserRule.bLimitWin=m_pGameOption->m_bLimitWin;
	UserRule.bLimitFlee=m_pGameOption->m_bLimitFlee;
	UserRule.wWinRate=m_pGameOption->m_wWinRate;
	UserRule.wFleeRate=m_pGameOption->m_wFleeRate;
	UserRule.lMaxScore=m_pGameOption->m_lMaxScore;
	UserRule.lLessScore	=m_pGameOption->m_lLessScore;
	UserRule.bLimitScore=m_pGameOption->m_bLimitScore;
	UserRule.bPassword=m_pServerOption->m_bPassword;
	UserRule.bCheckSameIP=g_GlobalOption.m_bCheckSameIP;
	lstrcpyn(UserRule.szPassword,m_pServerOption->m_szPassword,CountArray(UserRule.szPassword));

	//�������ݰ�
	m_ClientSocket->SendData(MDM_GR_USER,SUB_GR_USER_RULE,&UserRule,sizeof(UserRule));

	return true;
}

//�����Թ�����
bool CRoomViewItem::SendLookonPacket(WORD wTableID, WORD wChairID, LPCTSTR pszTablePass)
{
	//�������ݰ�
	CMD_GR_UserSitReq UserUserSitReq;
	memset(&UserUserSitReq,0,sizeof(UserUserSitReq));
	UserUserSitReq.wTableID=wTableID;
	UserUserSitReq.wChairID=wChairID;
	lstrcpyn(UserUserSitReq.szTablePass,pszTablePass,sizeof(UserUserSitReq.szTablePass));
	UserUserSitReq.cbPassLen=CountStringBuffer(UserUserSitReq.szTablePass);

	//�������ݰ�
	WORD wSendSize=sizeof(UserUserSitReq)-sizeof(UserUserSitReq.szTablePass)+UserUserSitReq.cbPassLen;
	m_ClientSocket->SendData(MDM_GR_USER,SUB_GR_USER_LOOKON_REQ,&UserUserSitReq,wSendSize);

	return true;
}

//������������
bool CRoomViewItem::SendSitDownPacket(WORD wTableID, WORD wChairID, LPCTSTR pszTablePass)
{
	//�������ݰ�
	CMD_GR_UserSitReq UserSitReq;
	memset(&UserSitReq,0,sizeof(UserSitReq));
	UserSitReq.wTableID=wTableID;
	UserSitReq.wChairID=wChairID;
	lstrcpyn(UserSitReq.szTablePass,pszTablePass,sizeof(UserSitReq.szTablePass));
	UserSitReq.cbPassLen=CountStringBuffer(UserSitReq.szTablePass);

	//�������ݰ�
	WORD wSendSize=sizeof(UserSitReq)-sizeof(UserSitReq.szTablePass)+UserSitReq.cbPassLen;
	m_ClientSocket->SendData(MDM_GR_USER,SUB_GR_USER_SIT_REQ,&UserSitReq,wSendSize);

	return true;
}

//�Թ۶���
bool CRoomViewItem::PerformLookonAction(WORD wTableID, WORD wChairID)
{
	//Ч������
	ASSERT(wTableID!=INVALID_TABLE);
	ASSERT(wChairID!=INVALID_CHAIR);

	//����Ч��
	if (CanSitDownTable(wTableID,wChairID,true,true)==false) return false;

	//�������
	if ((m_wReqTableID!=INVALID_TABLE)&&(m_wReqChairID!=INVALID_CHAIR))
	{
		IUserItem * pIUserItem=m_TableFrame.GetUserInfo(m_wReqTableID,m_wReqChairID);
		if (pIUserItem==m_pMeUserItem) m_TableFrame.SetUserInfo(m_wReqTableID,m_wReqChairID,NULL);
	}

	//��������
	m_wReqTableID=wTableID;
	m_wReqChairID=wChairID;
	m_wFindTableID=INVALID_TABLE;
	m_TableFrame.EnsureVisibleTable(wTableID);
	SendLookonPacket(wTableID,wChairID,m_pServerOption->m_szEnterPassword);

	return true;
}

//���¶���
bool CRoomViewItem::PerformSitDownAction(WORD wTableID, WORD wChairID)
{
	//Ч������
	ASSERT(wTableID!=INVALID_TABLE);
	ASSERT(wChairID!=INVALID_CHAIR);

	//����Ч��
	if (CanSitDownTable(wTableID,wChairID,false,true)==false) return false;

	//�������
	if ((m_wReqTableID!=INVALID_TABLE)&&(m_wReqChairID!=INVALID_CHAIR))
	{
		IUserItem * pIUserItem=m_TableFrame.GetUserInfo(m_wReqTableID,m_wReqChairID);
		if (pIUserItem==m_pMeUserItem) m_TableFrame.SetUserInfo(m_wReqTableID,m_wReqChairID,NULL);
	}

	//��������
	m_wReqTableID=wTableID;
	m_wReqChairID=wChairID;
	m_wFindTableID=INVALID_TABLE;
	m_TableFrame.EnsureVisibleTable(wTableID);
	SendSitDownPacket(wTableID,wChairID,m_pServerOption->m_szEnterPassword);

	//Ԥ����ʾ
	if (m_TableFrame.GetChairCount()<=MAX_CHAIR_NORMAL)
	{
		IUserItem * pIUserItem=m_TableFrame.GetUserInfo(wTableID,wChairID);
		if(pIUserItem==NULL)m_TableFrame.SetUserInfo(wTableID,wChairID,m_pMeUserItem);
	}

	return true;
}

//������������
bool CRoomViewItem::SendChatPacket(DWORD dwTargetUserID, LPCTSTR pszChatMessage, COLORREF crFontColor)
{
	//��ȡ�û�
	ASSERT(m_pMeUserItem!=NULL);
	tagUserData * pUserData=m_pMeUserItem->GetUserData();

	//��������
	CMD_GR_UserChat UserChat;
	UserChat.crFontColor=crFontColor;
	UserChat.dwTargetUserID=dwTargetUserID;
	UserChat.dwSendUserID=pUserData->dwUserID;
	lstrcpyn(UserChat.szChatMessage,pszChatMessage,CountArray(UserChat.szChatMessage));
	UserChat.wChatLength=CountStringBuffer(UserChat.szChatMessage);

	//��������
	WORD wSendSize=sizeof(UserChat)-sizeof(UserChat.szChatMessage)+UserChat.wChatLength;
	m_ClientSocket->SendData(MDM_GR_USER,SUB_GR_USER_CHAT,&UserChat,wSendSize);

	return true;
}

//������Ϸ��Ϣ
bool CRoomViewItem::IPCSendGameInfo(CIPCSendCopyData * pSendCopyData)
{
	//Ч�����
	ASSERT(pSendCopyData!=NULL);
	ASSERT(m_pMeUserItem!=NULL);

	//�������
	tagUserData * pUserData=m_pMeUserItem->GetUserData();
	CListKind * pListKind=m_pListServer->GetListKind();
	tagGameKind * pGameKind=pListKind->GetItemInfo();
	tagGameServer * pGameServer=m_pListServer->GetItemInfo();

	//��������
	IPC_GF_ServerInfo ServerInfo;
	memset(&ServerInfo,0,sizeof(ServerInfo));
	ServerInfo.dwUserID=pUserData->dwUserID;
	ServerInfo.wTableID=pUserData->wTableID;
	ServerInfo.wChairID=pUserData->wChairID;
	ServerInfo.wKindID=pGameKind->wKindID;
	ServerInfo.wServerID=pGameServer->wServerID;
	ServerInfo.wGameGenre=m_wGameGenre;
	ServerInfo.dwVideoAddr=m_dwVideoAddr;
	ServerInfo.cbHideUserInfo=m_cbHideUserInfo;
	ServerInfo.wChairCount=m_TableFrame.GetChairCount();
	lstrcpyn(ServerInfo.szKindName,pGameKind->szKindName,CountArray(ServerInfo.szKindName));
	lstrcpyn(ServerInfo.szServerName,pGameServer->szServerName,CountArray(ServerInfo.szServerName));

	//��������
	pSendCopyData->SendData(IPC_MAIN_CONFIG,IPC_SUB_SERVER_INFO,&ServerInfo,sizeof(ServerInfo));

	return true;
}

//�����û�
bool CRoomViewItem::IPCSendTableUsers(CIPCSendCopyData * pSendCopyData)
{
	//Ч�����
	ASSERT(pSendCopyData!=NULL);

	//�����Լ���Ϣ
	ASSERT(m_pMeUserItem!=NULL);
	tagUserData * pMeUserData=m_pMeUserItem->GetUserData();
	ASSERT(pMeUserData->wTableID!=(WORD)INVALID_TABLE);
	if (pMeUserData->wTableID==(WORD)INVALID_TABLE) return false;
	SendTableUser(m_pMeUserItem,pSendCopyData);

	//���������û�
	IUserItem * pIUserItem=NULL;
	tagUserData * pUserData=NULL;

	//��Ϸ�û�
	WORD wEnumIndex=0;
	while (true)
	{
		pIUserItem=m_ClientUserManager.EnumUserItem(wEnumIndex++);
		if (pIUserItem==NULL) break;
		if (pIUserItem==m_pMeUserItem) continue;
		pUserData=pIUserItem->GetUserData();
		if ((pUserData->wTableID==pMeUserData->wTableID)&&(pUserData->cbUserStatus!=US_LOOKON))
			SendTableUser(pIUserItem,pSendCopyData);
	};

	//�Թ��û�
	wEnumIndex=0;
	while (true)
	{
		pIUserItem=m_ClientUserManager.EnumUserItem(wEnumIndex++);
		if (pIUserItem==NULL) break;
		if (pIUserItem==m_pMeUserItem) continue;
		pUserData=pIUserItem->GetUserData();
		if ((pUserData->wTableID==pMeUserData->wTableID)&&(pUserData->cbUserStatus==US_LOOKON))
			SendTableUser(pIUserItem,pSendCopyData);
	};

	return true;
}

//������Ϸ
int CRoomViewItem::StartGameClient()
{
	//�ж��Ƿ��Ѿ�����
	ASSERT(m_pShareMemory!=NULL);
	if ((m_GameProcessInfo.hProcess!=NULL)&&(WaitForSingleObject(m_GameProcessInfo.hProcess,0)==WAIT_TIMEOUT))
	{
		if (m_pShareMemory->hWndGameFrame!=NULL)
		{
			//��ʾ����
			::ShowWindow(m_pShareMemory->hWndGameFrame,SW_SHOW);
			::SetForegroundWindow(m_pShareMemory->hWndGameFrame);

			//������Ϣ
			CIPCSendCopyData SendCopyData(m_hWndChannel,m_hWnd);
			IPCSendGameInfo(&SendCopyData);
			IPCSendTableUsers(&SendCopyData);
			SendCopyData.SendData(IPC_MAIN_CONCTROL,IPC_SUB_START_FINISH);

			return SR_ALREADY_EXIST;
		}
		else ::TerminateProcess(m_GameProcessInfo.hProcess,0);
	}

	//��������
	m_hWndChannel=NULL;
	m_pShareMemory->hWndGameFrame=NULL;
	CloseHandle(m_GameProcessInfo.hThread);
	CloseHandle(m_GameProcessInfo.hProcess);
	memset(&m_GameProcessInfo,0,sizeof(m_GameProcessInfo));

	//����������
	CString strCommonLine;
	CListKind * pListKind=m_pListServer->GetListKind();
	tagGameKind * pGameKind=pListKind->GetItemInfo();
	strCommonLine.Format(TEXT("%s /RoomToken:%s /ServerType:%ld /HideUserInfo:%d"),pGameKind->szProcessName,m_szShareName,m_wGameGenre,m_cbHideUserInfo);

	//������Ϸ�ͻ���
	STARTUPINFO StartInfo;
	memset(&StartInfo,0,sizeof(StartInfo));
	StartInfo.cb=sizeof(StartInfo);
	StartInfo.wShowWindow=SW_SHOWMAXIMIZED;
	BOOL bSuccess=CreateProcess(NULL,strCommonLine.GetBuffer(),NULL,NULL,FALSE,CREATE_DEFAULT_ERROR_MODE,NULL,NULL,&StartInfo,&m_GameProcessInfo);
	strCommonLine.ReleaseBuffer();
	if (bSuccess==FALSE)
	{
		memset(&m_GameProcessInfo,0,sizeof(m_GameProcessInfo));
		return SR_CREATE_ERROR;
	}

	return SR_CREATE_SUCCESS;
}

//�ر���Ϸ
void CRoomViewItem::CloseGameClient()
{
	//������Ϣ
	if (m_GameProcessInfo.hProcess!=NULL)
	{
		SendProcessData(IPC_MAIN_CONCTROL,IPC_SUB_CLOSE_FRAME,NULL,0);
		DWORD dwResult=::WaitForSingleObject(m_GameProcessInfo.hProcess,1000);
		if (dwResult==WAIT_TIMEOUT) ::TerminateProcess(m_GameProcessInfo.hProcess,0);
	}

	//��������
	m_hWndChannel=NULL;
	m_pShareMemory->hWndGameFrame=NULL;
	CloseHandle(m_GameProcessInfo.hThread);
	CloseHandle(m_GameProcessInfo.hProcess);
	memset(&m_GameProcessInfo,0,sizeof(m_GameProcessInfo));

	return;
}

//���±���
void CRoomViewItem::UpdateTitleText()
{
	//��ȡλ��
	CRect rcVorSplitter;
	m_VorSplitter.GetWindowRect(&rcVorSplitter);
	ScreenToClient(&rcVorSplitter);

	//ˢ�½���
	CRect rcInvalidate;
	rcInvalidate.SetRect(0,0,rcVorSplitter.left-BUTTON_AREA_WIDTH,m_ImageInfoTable.nTBorder);
	InvalidateRect(&rcInvalidate);

	return;
}

//�滭����
void CRoomViewItem::DrawTableFrame(CDC * pDC)
{
	//��ȡλ��
	CRect rcClient,rcVorSplitter;
	GetClientRect(&rcClient);
	m_VorSplitter.GetWindowRect(&rcVorSplitter);
	ScreenToClient(&rcVorSplitter);

	//�滭���
	CRect rcTable;
	rcTable.right=rcVorSplitter.left;
	rcTable.left=0;
	rcTable.bottom=rcClient.Height();
	rcTable.top=0;
	CSkinAide::DrawEncircleFrame(pDC,rcTable,m_EncircleTable);

	//�������
	TCHAR szRoomTitle[128]=TEXT("");
	CListKind * pListKind=m_pListServer->GetListKind();
	_snprintf(szRoomTitle,sizeof(szRoomTitle),TEXT("[ %s ] - [ %s ] ��%ld �ˣ�"),pListKind->GetItemInfo()->szKindName,
		m_pListServer->GetItemInfo()->szServerName,m_ClientUserManager.GetOnLineCount());

	//�������
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(64,64,64));
	pDC->SelectObject(CSkinResourceManager::GetDefaultFont());
	CRect rcTitle(10,5,rcVorSplitter.left-BUTTON_AREA_WIDTH-8,m_ImageInfoTable.nTBorder);
	pDC->DrawText(szRoomTitle,lstrlen(szRoomTitle),&rcTitle,DT_END_ELLIPSIS|DT_VCENTER|DT_SINGLELINE);

	return;
}

//����ͼ��
void CRoomViewItem::DrawLeftViewFrame(CDC * pDC)
{
	//��ȡλ��
	CRect rcClient,rcVorSplitter,rcHorSplitter;
	GetClientRect(&rcClient);
	m_VorSplitter.GetWindowRect(&rcVorSplitter);
	m_HorSplitter.GetWindowRect(&rcHorSplitter);
	ScreenToClient(&rcVorSplitter);
	ScreenToClient(&rcHorSplitter);

	//�滭�Ŀ�
	CRect rcChat;
	rcChat.left=rcVorSplitter.right;
	rcChat.right=rcClient.Width();
	rcChat.top=rcHorSplitter.bottom-rcHorSplitter.Height()/2;
	rcChat.bottom=rcClient.Height();
	CSkinAide::DrawEncircleFrame(pDC,rcChat,m_EncircleChat);

	//�滭�б�
	CRect rcList;
	rcList.left=rcVorSplitter.right;
	rcList.right=rcClient.Width();
	rcList.top=0;
	rcList.bottom=rcHorSplitter.top+rcHorSplitter.Height()/2;
	CSkinAide::DrawEncircleFrame(pDC,rcList,m_EncircleList);

	return;
}

//��������
bool CRoomViewItem::FindGameTable(tagFindTable & FindInfo)
{
	//��������
	WORD wNullCount=0;
	ITableView * pTableView=NULL;
	WORD wChairUser=m_TableFrame.GetChairCount();
	WORD wMeTableID=m_pMeUserItem->GetUserData()->wTableID;

	//��������
	for (WORD i=0;i<m_TableFrame.GetTableCount();i++)
	{
		FindInfo.wResultTableID=(FindInfo.wBeginTableID+i)%m_TableFrame.GetTableCount();
		if (wMeTableID!=FindInfo.wResultTableID)
		{
			//��ȡ����
			pTableView=m_TableFrame.GetTableArrayPtr(FindInfo.wResultTableID);
			ASSERT(pTableView!=NULL);

			//�ж��Ƿ�ʼ��Ϸ
			if (pTableView->QueryPlayFlag()==true) continue;

			//��������
			bool bTablePass=pTableView->QueryPassFlag();
			if ((FindInfo.bFilterPass==true)&&(bTablePass==true)) continue;

			//Ѱ�ҿ�λ��
			wNullCount=pTableView->GetNullChairCount(FindInfo.wResultChairID);

			//Ч�����
			if (CanSitDownTable(FindInfo.wResultTableID,FindInfo.wResultChairID,false,false)==false) continue;

			//�ж���Ŀ
			if (wNullCount>0)
			{
				if ((FindInfo.bNotFull==true)&&(wNullCount<wChairUser)) return true;
				if ((FindInfo.bOneNull==true)&&(wNullCount==1)) return true;
				if ((FindInfo.bTwoNull==true)&&(wNullCount==2)) return true;
				if ((FindInfo.bAllNull==true)&&(wNullCount==wChairUser)) return true;
			}
		}
	}

	//��������
	FindInfo.wResultTableID=INVALID_TABLE;
	FindInfo.wResultChairID=INVALID_CHAIR;

	return false;
}

//�����ؼ�
void CRoomViewItem::RectifyControl(int nWidth, int nHeight)
{
	//״̬�ж�
	if (m_bInitDialog==false) return;
	if ((nWidth==0)||(nHeight==0)) return;

	//��������
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;
	const int nSplitterWidth=__max(6,m_VorSplitter.GetButtonSize().cx);
	const int nSplitterHeight=__max(6,m_HorSplitter.GetButtonSize().cy);
	const INT nXScreen=GetSystemMetrics(SM_CXSCREEN);

	//��ȡλ��
	CRect rcVorSplitter;
	CRect rcHorSplitter;
	m_VorSplitter.GetWindowRect(&rcVorSplitter);
	m_HorSplitter.GetWindowRect(&rcHorSplitter);

	//λ��ת��
	ScreenToClient(&rcVorSplitter);
	ScreenToClient(&rcHorSplitter);

	//����Χ
	INT nVorMaxPos=nWidth-nXScreen/4;
	INT nVorLessPos=nXScreen*2/5;
	m_VorSplitter.SetSplitterRange(nVorLessPos,nVorMaxPos);
	UINT uControlId = ((!m_bHideUserList)?GetServerViewRes().uBtHideUserList:GetServerViewRes().uBtShowUserList);
	m_VorSplitter.ShowSplitterButton(uControlId,GetResInstanceHandle());

	//����Χ
	INT nHorMaxPos=0;
	INT nHorLessPos=0;
	nHorLessPos=nHeight/4;
	nHorMaxPos=nHeight*3/4;
	m_HorSplitter.SetSplitterRange(nHorLessPos,nHorMaxPos);

	//������
	if (m_bHideUserList==false)
	{
		if (m_bRectifyVor==false)
		{
			m_bRectifyVor=true;
			rcVorSplitter.top=0;
			rcVorSplitter.bottom=nHeight;
			rcVorSplitter.left=nWidth-nXScreen/4;
			rcVorSplitter.right=rcVorSplitter.left+nSplitterWidth;
		}
	}
	else
	{
		rcVorSplitter.top=0;
		rcVorSplitter.bottom=nHeight;
		rcVorSplitter.left=nWidth-nSplitterWidth;
		rcVorSplitter.right=rcVorSplitter.left+nSplitterWidth;
	}

	//������
	if (m_bRectifyHor==false)
	{
		m_bRectifyHor=true;
		rcHorSplitter.top=nHeight*2/5;
		rcHorSplitter.bottom=rcHorSplitter.top+nSplitterHeight;
	}

	//����λ��
	if ((nWidth!=0)&&(nHeight!=0))
	{
		//������
		if (!m_bHideUserList)
		{
			if (rcVorSplitter.left<nVorLessPos)
			{
				rcVorSplitter.left=nVorLessPos;
				rcVorSplitter.right=rcVorSplitter.left+nSplitterWidth;
			}
			if (rcVorSplitter.right>nVorMaxPos)
			{
				rcVorSplitter.right=nVorMaxPos;
				rcVorSplitter.left=rcVorSplitter.right-nSplitterWidth;
			}
		}

		//������
		if (rcHorSplitter.top<nHorLessPos)
		{
			rcHorSplitter.top=nHorLessPos;
			rcHorSplitter.bottom=rcHorSplitter.top+nSplitterHeight;
		}
		if (rcHorSplitter.bottom>nHorMaxPos)
		{
			rcHorSplitter.bottom=nHorMaxPos;
			rcHorSplitter.top=rcHorSplitter.bottom-nSplitterHeight;
		}
	}
 
	//�ƶ��ؼ�
	HDWP hDwp=BeginDeferWindowPos(20);

	//��������
	DeferWindowPos(hDwp,m_VorSplitter,NULL,rcVorSplitter.left,0,rcVorSplitter.Width(),nHeight,uFlags);

	//�������
	DeferWindowPos(hDwp,m_BrowerAD,NULL,rcVorSplitter.right+m_ImageInfoList.nLBorder,8,nWidth-rcVorSplitter.right-m_ImageInfoList.nLBorder-m_ImageInfoList.nRBorder,75,uFlags);

	//��������
	DeferWindowPos(hDwp,m_TableFrame,NULL,m_ImageInfoTable.nLBorder,m_ImageInfoTable.nTBorder,rcVorSplitter.left-m_ImageInfoTable.nRBorder-m_ImageInfoTable.nLBorder,
		nHeight-m_ImageInfoTable.nBBorder-m_ImageInfoTable.nTBorder,uFlags);

	//��ʾ����
	if(!m_bHideUserList)
	{
		//�б�����
		DeferWindowPos(hDwp,m_UserListView,NULL,rcVorSplitter.right+m_ImageInfoList.nLBorder,m_ImageInfoList.nTBorder,
			nWidth-rcVorSplitter.right-m_ImageInfoList.nLBorder-m_ImageInfoList.nRBorder,
			rcHorSplitter.top-m_ImageInfoList.nTBorder-m_ImageInfoList.nBBorder+rcHorSplitter.Height()/2,uFlags);

		//��������
		DeferWindowPos(hDwp,m_HorSplitter,NULL,rcVorSplitter.right+m_ImageInfoList.nLBorder,rcHorSplitter.top,
			nWidth-rcVorSplitter.right-m_ImageInfoList.nLBorder-m_ImageInfoList.nRBorder,rcHorSplitter.Height(),uFlags);

		//��������
		m_PropertyBar.ShowWindow(SW_SHOW);
		DeferWindowPos(hDwp, m_PropertyBar, NULL, rcVorSplitter.right+m_ImageInfoList.nLBorder, rcHorSplitter.top+rcHorSplitter.Height()/2-35, 
			nWidth-rcVorSplitter.right-m_ImageInfoList.nLBorder-m_ImageInfoList.nRBorder, 
			71, uFlags); 

		//��������
		DeferWindowPos(hDwp,m_ChatMessage,NULL,rcVorSplitter.right+m_ImageInfoChat.nLBorder,
			rcHorSplitter.bottom+m_ImageInfoChat.nTBorder-rcHorSplitter.Height()/2,
			nWidth-rcVorSplitter.right-m_ImageInfoChat.nRBorder-m_ImageInfoChat.nLBorder,
			nHeight-rcHorSplitter.bottom-m_ImageInfoChat.nTBorder-m_ImageInfoChat.nBBorder+rcHorSplitter.Height()/2,uFlags);

		//���찴ť
		CRect rcButton;
		m_btColorSet.GetWindowRect(&rcButton);
		CButton * pButtonArray[]={&m_btExpression,&m_btColorSet,&m_btCleanScreen,&m_btBugle};
		for (int i=0;i<CountArray(pButtonArray);i++)
		{
			DeferWindowPos(hDwp,pButtonArray[i]->m_hWnd,NULL,rcVorSplitter.right+m_ImageInfoChat.nLBorder+(rcButton.Width()+5)*i,nHeight-54,rcButton.Width(),rcButton.Height(),uFlags);
		}
		m_btSendChat.GetWindowRect(&rcButton);
		DeferWindowPos(hDwp,m_btSendChat,NULL,nWidth-rcButton.Width()-8,nHeight-33,rcButton.Width(),rcButton.Height(),uFlags);
		DeferWindowPos(hDwp,m_ChatObject,NULL,rcVorSplitter.right+8,nHeight-33,70,200,uFlags);
		DeferWindowPos(hDwp,m_ChatInput,NULL,rcVorSplitter.right+81,nHeight-33,nWidth-rcVorSplitter.right-rcButton.Width()-93,19,uFlags);
	}

	//��������
	CRect rcButton;
	m_btAutoSit.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btQuitRoom,NULL,rcVorSplitter.left-rcButton.Width()-m_ImageInfoTable.nRBorder-12,5,rcButton.Width(),rcButton.Height(),uFlags);
	DeferWindowPos(hDwp,m_btAutoSit,NULL,rcVorSplitter.left-rcButton.Width()*2-m_ImageInfoTable.nRBorder-20,5,rcButton.Width(),rcButton.Height(),uFlags);
	DeferWindowPos(hDwp,m_btFindUser,NULL,rcVorSplitter.left-rcButton.Width()*3-m_ImageInfoTable.nRBorder-28,5,rcButton.Width(),rcButton.Height(),uFlags);
	DeferWindowPos(hDwp,m_btBank,NULL,rcVorSplitter.left-rcButton.Width()*4-m_ImageInfoTable.nRBorder-36,5,rcButton.Width(),rcButton.Height(),uFlags);

	EndDeferWindowPos(hDwp);

	//�ж���ʾ
	//m_HorSplitter.ShowWindow((!m_bHideUserList)?SW_SHOW:SW_HIDE);
	m_HorSplitter.ShowWindow(SW_HIDE);
	m_UserListView.ShowWindow((!m_bHideUserList)?SW_SHOW:SW_HIDE);
	m_PropertyBar.ShowWindow((!m_bHideUserList)?SW_SHOW:SW_HIDE);
	m_ChatMessage.ShowWindow((!m_bHideUserList)?SW_SHOW:SW_HIDE);
	m_btExpression.ShowWindow((!m_bHideUserList)?SW_SHOW:SW_HIDE);
	m_btBugle.ShowWindow((!m_bHideUserList)?SW_SHOW:SW_HIDE);
	m_btColorSet.ShowWindow((!m_bHideUserList)?SW_SHOW:SW_HIDE);
	m_btCleanScreen.ShowWindow((!m_bHideUserList)?SW_SHOW:SW_HIDE);
	m_btSendChat.ShowWindow((!m_bHideUserList)?SW_SHOW:SW_HIDE);
	m_ChatObject.ShowWindow((!m_bHideUserList)?SW_SHOW:SW_HIDE);
	m_ChatInput.ShowWindow((!m_bHideUserList)?SW_SHOW:SW_HIDE);

	//�ػ�����
	Invalidate(FALSE);
	UpdateWindow();

	return;
}

//��ʾ��Ϣ
int CRoomViewItem::ShowMessageBox(LPCTSTR pszMessage, UINT nType)
{
	tagGameServer * pGameServer=m_pListServer->GetItemInfo();
	
	int nResult=ShowInformationEx(pszMessage,0,nType,pGameServer->szServerName);
	return nResult;
}

//�����û�
bool CRoomViewItem::SendTableUser(IUserItem * pIUserItem, CIPCSendCopyData * pSendCopyData)
{
	//Ч�����
	ASSERT(pIUserItem!=NULL);
	ASSERT(pSendCopyData!=NULL);

	//�������
	BYTE cbBuffer[IPC_PACKAGE];
	tagUserData * pUserData=pIUserItem->GetUserData();
	tagUserInfoHead * pUserInfoHead=(tagUserInfoHead *)cbBuffer;

	//��������
	memset(cbBuffer,0,sizeof(cbBuffer));
	pUserInfoHead->dwUserID=pUserData->dwUserID;
	pUserInfoHead->dwGameID=pUserData->dwGameID;
	pUserInfoHead->dwGroupID=pUserData->dwGroupID;
	pUserInfoHead->dwUserRight=pUserData->dwUserRight;
	pUserInfoHead->lLoveliness=pUserData->lLoveliness;
	pUserInfoHead->dwMasterRight=pUserData->dwMasterRight;
	pUserInfoHead->wFaceID=pUserData->wFaceID;
	pUserInfoHead->dwCustomFaceVer=pUserData->dwCustomFaceVer;
	pUserInfoHead->wTableID=pUserData->wTableID;
	pUserInfoHead->wChairID=pUserData->wChairID;
	pUserInfoHead->cbGender=pUserData->cbGender;
	pUserInfoHead->cbUserStatus=pUserData->cbUserStatus;
	pUserInfoHead->cbMemberOrder=pUserData->cbMemberOrder;
	pUserInfoHead->cbMasterOrder=pUserData->cbMasterOrder;
	pUserInfoHead->UserScoreInfo.lScore=pUserData->lScore;
	pUserInfoHead->UserScoreInfo.lGameGold=pUserData->lGameGold;
	pUserInfoHead->UserScoreInfo.lInsureScore=pUserData->lInsureScore;
	pUserInfoHead->UserScoreInfo.lWinCount=pUserData->lWinCount;
	pUserInfoHead->UserScoreInfo.lLostCount=pUserData->lLostCount;
	pUserInfoHead->UserScoreInfo.lDrawCount=pUserData->lDrawCount;
	pUserInfoHead->UserScoreInfo.lFleeCount=pUserData->lFleeCount;
	pUserInfoHead->UserScoreInfo.lExperience=pUserData->lExperience;
	//pUserInfoHead->lInsureScore = pUserData->lInsureScore;
	for ( WORD wPropID = 0; wPropID < PROPERTY_COUNT; ++wPropID )
	{
		pUserInfoHead->dwPropResidualTime[wPropID] = pUserData->dwPropResidualTime[wPropID];
	}

	//��������
	WORD wHeadSize=sizeof(tagUserInfoHead);
	CSendPacketHelper SendPacketHelper(cbBuffer+wHeadSize,sizeof(cbBuffer)-wHeadSize);
	SendPacketHelper.AddPacket(pUserData->szName,CountStringBuffer(pUserData->szName),DTP_USER_ACCOUNTS);
	SendPacketHelper.AddPacket(&pUserData->cbCompanion,sizeof(pUserData->cbCompanion),DTP_USER_COMPANION);
	SendPacketHelper.AddPacket(pUserData->szUnderWrite,CountStringBuffer(pUserData->szUnderWrite),DTP_UNDER_WRITE);
	SendPacketHelper.AddPacket(pUserData->szGroupName,CountStringBuffer(pUserData->szGroupName),DTP_USER_GROUP_NAME);
	
	//��������
	WORD wSendSize=wHeadSize+SendPacketHelper.GetDataSize();
	pSendCopyData->SendData(IPC_MAIN_USER,IPC_SUB_USER_COME,cbBuffer,wSendSize);

	return true;
}

//������Ϣ
bool CRoomViewItem::SendProcessData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	//������Ϣ
	CIPCSendCopyData IPCSendCopyData(m_hWndChannel,m_hWnd);
	return IPCSendCopyData.SendData(wMainCmdID,wSubCmdID,pData,wDataSize);
}

//�ܷ�����
bool CRoomViewItem::CanSitDownTable(WORD wTableID, WORD wChairID, bool bLookon, bool bShowError)
{
	//Ч�����
	ASSERT(wTableID!=INVALID_TABLE);
	ASSERT(wChairID!=INVALID_CHAIR);
	if (wTableID>=m_TableFrame.GetTableCount()) return false;
	if (wChairID>=m_TableFrame.GetChairCount()) return false;
	if (m_ServiceStatus!=ServiceStatus_Serviceing) return false;

	//�ж���Ӧ
	if ((m_wReqTableID!=INVALID_TABLE)||(m_wReqChairID!=INVALID_CHAIR))
	{
		if (bShowError==true)
		{
			LPCTSTR pszDescribe=TEXT("�ϴ�����û���յ���������Ӧ,���Ժ�");
			ShowMessageBox(pszDescribe,MB_ICONINFORMATION);
			return false;
		}
	}

	//�ж����
	ITableView * pITableView=m_TableFrame.GetTableArrayPtr(wTableID);
	IUserItem * pITableUserItem=pITableView->GetUserInfo(wChairID);
	if ((bLookon==true)&&(pITableUserItem==NULL)) return false;
	if ((bLookon==false)&&(pITableUserItem!=NULL)) return false;

	//�ж�״̬
	BYTE cbUserStatus=m_pMeUserItem->GetUserData()->cbUserStatus;
	if (cbUserStatus==US_PLAY)
	{
		if (bShowError==true)
		{
			LPCTSTR pszDescribe=TEXT("��������Ϸ�У���ʱ���ܻ�λ�ã����Ƚ�����Ϸ��");
			ShowMessageBox(pszDescribe,MB_ICONINFORMATION);
		}
		return false;
	}

	//�ж���Ϸ״̬
	bool bGameStart=pITableView->QueryPlayFlag();
	if ((bGameStart==false)&&(bLookon==true)&&(m_pMeUserItem->GetUserData()->dwMasterRight==0)) return false;

	//�ж�����
	if ((pITableView->QueryPassFlag()==true) && 0==m_pMeUserItem->GetUserData()->dwMasterRight)
	{
		if (bShowError==true)
		{
			//��������
			CDlgEnterTablePass DlgEnterTablePass;
			ZeroMemory(m_pServerOption->m_szEnterPassword,sizeof(m_pServerOption->m_szEnterPassword));
			DlgEnterTablePass.SetPassword(m_pServerOption->m_szEnterPassword,CountArray(m_pServerOption->m_szEnterPassword));

			//��ʾ����
			DlgEnterTablePass.DoModal();
			if(m_pServerOption->m_szEnterPassword[0]==0)return false;
		}
		else return false;
	}

	//�Թ��ж�
	if (bLookon==true) return true;

	//�����ж�
	tagUserData * pTableUserData=NULL;
	for (WORD i=0;i<m_TableFrame.GetTableCount();i++)
	{
		//��ȡ�û�
		pITableUserItem=pITableView->GetUserInfo(i);
		if ((pITableUserItem==NULL)||(pITableUserItem==m_pMeUserItem)) continue;
		pTableUserData=pITableUserItem->GetUserData();

		//������
		if (g_GlobalOption.m_bLimitDetest==true)
		{
			if (pTableUserData->cbCompanion==enCompanion_Detest)
			{
				if (bShowError==true)
				{
					TCHAR szDescribe[128]=TEXT("");
					_snprintf(szDescribe,sizeof(szDescribe),TEXT("�������˲��벻�ܻ�ӭ�������Ϸ���������㲻��ӭ����� [ %s ] ��"),pTableUserData->szName);
					ShowMessageBox(szDescribe,MB_ICONINFORMATION);
				}
				return false;
			}
		}

		//ʤ��Ч��
		if (m_pGameOption->m_bLimitWin)
		{
			WORD wWinRate=0;
			LONG lAllCount=pITableUserItem->GetUserPlayCount();
			if (lAllCount>0) wWinRate=(WORD)(pTableUserData->lWinCount*10000/lAllCount);
			if (wWinRate<m_pGameOption->m_wWinRate)
			{
				if (bShowError==true)
				{
					TCHAR szDescribe[128]=TEXT("");
					_snprintf(szDescribe,sizeof(szDescribe),TEXT("[ %s ] ��ʤ��̫���ˣ���������ò�����"),pTableUserData->szName);
					ShowMessageBox(szDescribe,MB_ICONINFORMATION);
				}
				return false;
			}
		}

		//����Ч��
		if (m_pGameOption->m_bLimitFlee)
		{
			WORD wFillRate=0;
			LONG lAllCount=pITableUserItem->GetUserPlayCount();
			if (lAllCount>0) wFillRate=(WORD)(pTableUserData->lFleeCount*10000/lAllCount);
			if ((wFillRate>0)&&(wFillRate>m_pGameOption->m_wFleeRate))
			{
				if (bShowError==true)
				{
					TCHAR szDescribe[128]=TEXT("");
					_snprintf(szDescribe,sizeof(szDescribe),TEXT("[ %s ] ��������̫���ˣ���������ò�����"),pTableUserData->szName);
					ShowMessageBox(szDescribe,MB_ICONINFORMATION);
				}
				return false;
			}
		}

		//����Ч��
		if (m_pGameOption->m_bLimitScore)
		{
			LONG lTableUserScore=pTableUserData->lScore;
			if (lTableUserScore>m_pGameOption->m_lMaxScore)
			{
				if (bShowError==true)
				{
					TCHAR szDescribe[128]=TEXT("");
					_snprintf(szDescribe,sizeof(szDescribe),TEXT("[ %s ] �Ļ���̫���ˣ���������ò�����"),pTableUserData->szName);
					ShowMessageBox(szDescribe,MB_ICONINFORMATION);
				}
				return false;
			}
			if (lTableUserScore<m_pGameOption->m_lLessScore)
			{
				if (bShowError==true)
				{
					TCHAR szDescribe[128]=TEXT("");
					_snprintf(szDescribe,sizeof(szDescribe),TEXT("[ %s ] �Ļ���̫���ˣ���������ò�����"),pTableUserData->szName);
					ShowMessageBox(szDescribe,MB_ICONINFORMATION);
				}
				return false;
			}
		}
	}

	return true;
}

//����������Ϣ
bool __cdecl CRoomViewItem::OnEventTCPSocketLink(WORD wSocketID, INT nErrorCode)
{
	//������
	if (nErrorCode!=0)
	{
		g_GlobalAttemper.DestroyStatusWnd(this);
		ShowMessageBox(TEXT("��Ϸ��������ʧ�ܣ�����ʱ���ܽ������Ϸ���䣡"),MB_ICONINFORMATION);
		((CGameFrame*)AfxGetMainWnd())->CloseRoomViewItem(this);
		return true;
	}

	//���͵�¼��Ϣ
	SendLogonPacket();
	m_ServiceStatus=ServiceStatus_EfficacyUser;

	return true;
}

//�����ȡ��Ϣ
bool __cdecl CRoomViewItem::OnEventTCPSocketRead(WORD wSocketID, CMD_Command Command, VOID * pData, WORD wDataSize)
{
	switch (Command.wMainCmdID)
	{
	case MDM_GR_LOGON:			//��¼��Ϣ
		{
			return OnSocketMainLogon(Command,pData,wDataSize);
		}
	case MDM_GR_USER:			//�û���Ϣ
		{
			return OnSocketMainUser(Command,pData,wDataSize);
		}
	case MDM_GR_INFO:			//������Ϣ
		{
			return OnSocketMainInfo(Command,pData,wDataSize);
		}
	case MDM_GR_STATUS:			//״̬��Ϣ
		{
			return OnSocketMainStatus(Command,pData,wDataSize);
		}
	case MDM_GR_SYSTEM:			//ϵͳ��Ϣ
		{
			return OnSocketMainSystem(Command,pData,wDataSize);
		}
	case MDM_GR_SERVER_INFO:	//������Ϣ
		{
			return OnSocketMainServerInfo(Command,pData,wDataSize);
		}
	case MDM_GF_GAME:			//��Ϸ��Ϣ
	case MDM_GF_FRAME:			//�����Ϣ
	case MDM_GF_VIDEO:			//��Ƶ��Ϣ
	case MDM_GF_PRESENT:		//������Ϣ
		{
			return OnSocketMainGameFrame(Command,pData,wDataSize);
		}
	}

	return true;
}

//����ر���Ϣ
bool __cdecl CRoomViewItem::OnEventTCPSocketShut(WORD wSocketID, BYTE cbShutReason)
{
	//�жϹر�
	bool bCloseRoomView=(m_ServiceStatus!=ServiceStatus_Serviceing && cbShutReason != SHUT_REASON_NORMAL);

	//�رմ���
	CloseGameClient();
	g_GlobalAttemper.DestroyStatusWnd(this);
	m_ServiceStatus=ServiceStatus_NetShutDown;
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		if ((cbShutReason==SHUT_REASON_REMOTE)||(cbShutReason==SHUT_REASON_TIME_OUT))
		{
			m_MessageProxyHelper->InsertSystemString("�����������⣬���Ѿ���������Ͽ����ӣ�����������",0);
			ShowMessageBox("�����������⣬���Ѿ���������Ͽ����ӣ�����������",MB_ICONINFORMATION);
		}
		else
		{
			ShowMessageBox("�����������ݰ�����ʧ�ܣ������ж���",MB_ICONINFORMATION);
		}
	}

	//�رշ���
	if (bCloseRoomView==true) ((CGameFrame*)AfxGetMainWnd())->CloseRoomViewItem(this);

	return true;
}

//��¼��Ϣ
bool CRoomViewItem::OnSocketMainLogon(CMD_Command Command, void * pData, WORD wDataSize)
{
	ASSERT(Command.wMainCmdID==MDM_GR_LOGON);
	switch (Command.wSubCmdID)
	{
	case SUB_GR_LOGON_SUCCESS:		//��¼�ɹ�
		{
			//���ñ���
			m_ServiceStatus=ServiceStatus_RecvConfigInfo;

			return true;
		}
	case SUB_GR_LOGON_ERROR:		//��¼ʧ��
		{
			//Ч�����
			CMD_GR_LogonError * pLogonError=(CMD_GR_LogonError *)pData;
			ASSERT(wDataSize>=(sizeof(CMD_GR_LogonError)-sizeof(pLogonError->szErrorDescribe)));
			if (wDataSize<(sizeof(CMD_GR_LogonError)-sizeof(pLogonError->szErrorDescribe))) return false;

			//�ر�����
			g_GlobalAttemper.DestroyStatusWnd(this);
			m_ClientSocket->CloseSocket();
			m_ServiceStatus=ServiceStatus_NetShutDown;

			//��ʾ��Ϣ
			WORD wDescribeSize=wDataSize-(sizeof(CMD_GR_LogonError)-sizeof(pLogonError->szErrorDescribe));
			if (wDescribeSize>0)
			{
				pLogonError->szErrorDescribe[wDescribeSize-1]=0;
				ShowMessageBox(pLogonError->szErrorDescribe,MB_ICONINFORMATION);
			}

			//�رշ���
			((CGameFrame*)AfxGetMainWnd())->CloseRoomViewItem(this);

			return true;
		}
	case SUB_GR_LOGON_FINISH:		//��¼���
		{
			//���ý���
			if (m_TableFrame.GetChairCount()<=MAX_CHAIR_NORMAL) m_TableFrame.ShowUserInfo(true);
			g_GlobalAttemper.DestroyStatusWnd(this);
			((CGameFrame*)AfxGetMainWnd())->ActiveRoomViewItem(this);

			//���ñ���
			m_ServiceStatus=ServiceStatus_Serviceing;

			//���͹���
			SendUserRulePacket();

			//�����ж�
			ASSERT(m_pMeUserItem!=NULL);
			tagUserData * pUserData=m_pMeUserItem->GetUserData();
			if (pUserData->wTableID!=INVALID_TABLE)
			{
				int iResult=StartGameClient();
			}

			return true;
		}
	}

	return true;
}

//�û���Ϣ
bool CRoomViewItem::OnSocketMainUser(CMD_Command Command, void * pData, WORD wDataSize)
{
	ASSERT(Command.wMainCmdID==MDM_GR_USER);
	switch (Command.wSubCmdID)
	{
	case SUB_GR_USER_COME:			//�û�����
		{
			return OnSocketSubUserCome(Command,pData,wDataSize);
		}
	case SUB_GR_USER_STATUS:		//�û�״̬
		{
			return OnSocketSubStatus(Command,pData,wDataSize);
		}
	case SUB_GR_USER_SCORE:			//�û�����
		{
			return OnSocketSubScore(Command,pData,wDataSize);
		}
	case SUB_GR_USER_RIGHT:			//�û�Ȩ��
		{
			return OnSocketSubRight(Command,pData,wDataSize);
		}
	case SUB_GR_MEMBER_ORDER:		//��Ա�ȼ�
		{
			return OnSocketSubMemberOrder(Command,pData,wDataSize);
		}
	case SUB_GR_SIT_FAILED:			//����ʧ��
		{
			return OnSocketSubSitFailed(Command,pData,wDataSize);
		}
	case SUB_GR_USER_CHAT:			//�û�����
		{
			return OnSocketSubChat(Command,pData,wDataSize);
		}
	case SUB_GR_USER_WISPER:		//�û�˽��
		{
			return OnSocketSubWisper(Command,pData,wDataSize);
		}
	case SUB_GR_USER_INVITE:		//�������
		{
			return OnSocketSubUserInvite(Command,pData,wDataSize);
		}
	}

	return true;
}

//������Ϣ
bool CRoomViewItem::OnSocketMainInfo(CMD_Command Command, void * pData, WORD wDataSize)
{
	ASSERT(Command.wMainCmdID==MDM_GR_INFO);
	switch (Command.wSubCmdID)
	{
	case SUB_GR_SERVER_INFO:	//������Ϣ
		{
			//Ч������
			ASSERT(wDataSize>=sizeof(CMD_GR_ServerInfo));
			if (wDataSize<sizeof(CMD_GR_ServerInfo)) return false;

			//��Ϣ����
			CMD_GR_ServerInfo * pServerInfo=(CMD_GR_ServerInfo *)pData;

			//���ñ���
			m_wGameGenre=pServerInfo->wGameGenre;
			m_dwVideoAddr=pServerInfo->dwVideoAddr;
			m_cbHideUserInfo=pServerInfo->cbHideUserInfo;

			//��ȡ��Ϣ
			CListKind * pListKind=m_pListServer->GetListKind();
			tagGameKind * pGameKind=pListKind->GetItemInfo();
			IUnknownEx * pIUnknownEx=QUERY_ME_INTERFACE(IUnknownEx);
			m_UserListView.m_wKindID=pServerInfo->wKindID;
			m_UserListView.m_wGameGenre=pServerInfo->wGameGenre;

			//��������
			try
			{
				m_TableFrame.CreateTableFrame(this,100);
				m_TableFrame.InitTableFrame(pServerInfo->wTableCount,pServerInfo->wChairCount,(pServerInfo->cbHideUserInfo==TRUE),pGameKind,pIUnknownEx);
			}
			catch (...)
			{
				//�ر�����
				m_ClientSocket->CloseSocket();
				g_GlobalAttemper.DestroyStatusWnd(this);

				//��ʾ��Ϣ
				int nResult=ShowMessageBox(TEXT("��Ϸ������Դ����ʧ�ܣ��������ذ�װ���ܻ������⣬�Ƿ��������أ�"),MB_ICONQUESTION|MB_YESNO);
				if (nResult==IDYES) 
				{
					CListKind * pListKind=m_pListServer->GetListKind();
					tagGameKind * pGameKind=pListKind->GetItemInfo();
					g_GlobalAttemper.DownLoadClient(pGameKind->szKindName,pGameKind->wKindID,true);
				}

				//�رշ���
				IRoomViewItem * pIRoomViewItem=QUERY_ME_INTERFACE(IRoomViewItem);
				((CGameFrame*)AfxGetMainWnd())->CloseRoomViewItem(pIRoomViewItem);

				return false;
			}

			return true;
		}
	case SUB_GR_COLUMN_INFO:	//�б����
		{
			//��������
			CMD_GR_ColumnInfo * pColumnInfo=(CMD_GR_ColumnInfo *)pData;
			WORD wHeadSize=sizeof(CMD_GR_ColumnInfo)-sizeof(pColumnInfo->ColumnItem);

			//Ч�����
			ASSERT(wDataSize>=wHeadSize);
			ASSERT((wHeadSize+pColumnInfo->wColumnCount*sizeof(pColumnInfo->ColumnItem[0]))==wDataSize);
			if (wDataSize<wHeadSize) return false;
			if ((wHeadSize+pColumnInfo->wColumnCount*sizeof(pColumnInfo->ColumnItem[0]))!=wDataSize) return false;

			//�����б�
			CopyMemory(&m_ListColumnInfo,pColumnInfo,__min(wDataSize,sizeof(m_ListColumnInfo)));
			m_UserListView.SetColumnDescribe(pColumnInfo->ColumnItem,pColumnInfo->wColumnCount);

			return true;
		}
	case SUB_GR_CONFIG_FINISH:	//�������
		{
			//��ԴĿ¼
			TCHAR szResDirectory[128]=TEXT("");
			WORD wStringIndex=0,wBufferCount=CountArray(szResDirectory);

			//����Ŀ¼
			const tagGameKind * pGameKind = GetKindInfo();
			ZeroMemory(szResDirectory,wBufferCount*sizeof(TCHAR));
			lstrcpyn(szResDirectory,(LPCTSTR)pGameKind->szProcessName,wBufferCount);
			while ((szResDirectory[wStringIndex]!=0)&&(szResDirectory[wStringIndex]!=TEXT('.'))) wStringIndex++;

			//�ַ���ֹ
			szResDirectory[wStringIndex]=0;

			//��Ϸ�ȼ�
			ASSERT(m_UserListView.m_GameRankManager.GetInterface()!=NULL);
			/*bool bSuccess=*/m_UserListView.m_GameRankManager->LoadGameLevel(szResDirectory,m_wGameGenre);

			//��ʾ����
			ShowWindow(SW_SHOW);
			((CGameFrame*)AfxGetMainWnd())->ActiveRoomViewItem(this);

			//���ñ���
			m_ServiceStatus=ServiceStatus_RecvRoomInfo;

			return true;
		}
	}

	return true;
}

//״̬��Ϣ
bool CRoomViewItem::OnSocketMainStatus(CMD_Command Command, void * pData, WORD wDataSize)
{
	ASSERT(Command.wMainCmdID==MDM_GR_STATUS);
	switch (Command.wSubCmdID)
	{
	case SUB_GR_TABLE_INFO:		//������Ϣ
		{
			//��������
			CMD_GR_TableInfo * pTableInfo=(CMD_GR_TableInfo *)pData;
			const WORD wHeadSize=sizeof(CMD_GR_TableInfo)-sizeof(pTableInfo->TableStatus);

			//Ч������
			ASSERT(wDataSize>=wHeadSize);
			ASSERT((wHeadSize+pTableInfo->wTableCount*sizeof(pTableInfo->TableStatus[0]))==wDataSize);
			if (wDataSize<wHeadSize) return false;
			if ((wHeadSize+pTableInfo->wTableCount*sizeof(pTableInfo->TableStatus[0]))!=wDataSize) return false;

			//��Ϣ����
			for (WORD i=0;i<pTableInfo->wTableCount;i++)
			{
				m_TableFrame.SetPassFlag(i,pTableInfo->TableStatus[i].bTableLock?true:false);
				m_TableFrame.SetPlayFlag(i,pTableInfo->TableStatus[i].bPlayStatus?true:false);
			}

			return true;
		}
	case SUB_GR_TABLE_STATUS:	//����״̬
		{
			//Ч������
			ASSERT(wDataSize>=sizeof(CMD_GR_TableStatus));
			if (wDataSize<sizeof(CMD_GR_TableStatus)) return false;

			//��Ϣ����
			CMD_GR_TableStatus * pTableStatus=(CMD_GR_TableStatus *)pData;
			ASSERT(pTableStatus->wTableID<m_TableFrame.GetTableCount());
			if (pTableStatus->wTableID<m_TableFrame.GetTableCount())
			{
				//��������
				IUserItem * pIUserItem=NULL;
				tagUserData * pUserData=NULL;
				BYTE bPlayStatus=m_TableFrame.QueryPlayFlag(pTableStatus->wTableID);

				//�����û�
				if (bPlayStatus!=pTableStatus->bPlayStatus)
				{
					BYTE cbUserStatus=pTableStatus->bPlayStatus?US_PLAY:US_SIT;
					for (WORD i=0;i<m_TableFrame.GetChairCount();i++)
					{
						pIUserItem=m_TableFrame.GetUserInfo(pTableStatus->wTableID,i);
						if (pIUserItem!=NULL)
						{
							pUserData=pIUserItem->GetUserData();
							if (pUserData->cbUserStatus!=US_OFFLINE) 
							{
								pUserData->cbUserStatus=cbUserStatus;
								OnUserItemUpdate(pIUserItem);
							}
						}
					}
				}

				//��������
				m_TableFrame.SetPassFlag(pTableStatus->wTableID,pTableStatus->bTableLock?true:false);
				m_TableFrame.SetPlayFlag(pTableStatus->wTableID,pTableStatus->bPlayStatus?true:false);

				//֪ͨ��Ϸ
				if (bPlayStatus!=pTableStatus->bPlayStatus)
				{
					tagUserData * pMeUserData=m_pMeUserItem->GetUserData();
					if (pMeUserData->wTableID==pTableStatus->wTableID)
					{
						WORD wSubCmdID=pTableStatus->bPlayStatus?IPC_SUB_GAME_START:IPC_SUB_GAME_FINISH;
						SendProcessData(IPC_MAIN_USER,wSubCmdID,NULL,0);
					}
				}
			}

			return true;
		}
	}

	return true;
}

//ϵͳ��Ϣ
bool CRoomViewItem::OnSocketMainSystem(CMD_Command Command, void * pData, WORD wDataSize)
{
	ASSERT(Command.wMainCmdID==MDM_GR_SYSTEM);
	switch (Command.wSubCmdID)
	{
	case SUB_GR_MESSAGE:		//ϵͳ��Ϣ
		{
			//Ч�����
			CMD_GR_Message * pMessage=(CMD_GR_Message *)pData;
			ASSERT(wDataSize>(sizeof(CMD_GR_Message)-sizeof(pMessage->szContent)));
			if (wDataSize<=(sizeof(CMD_GR_Message)-sizeof(pMessage->szContent))) return false;

			//��Ϣ����
			WORD wHeadSize=sizeof(CMD_GR_Message)-sizeof(pMessage->szContent);
			ASSERT(wDataSize==(wHeadSize+pMessage->wMessageLength*sizeof(TCHAR)));
			if (wDataSize!=(wHeadSize+pMessage->wMessageLength*sizeof(TCHAR))) return false;
			pMessage->szContent[pMessage->wMessageLength-1]=0;

			//�ر�����
			bool bIntermet=false;
			if (pMessage->wMessageType&SMT_INTERMIT_LINE) bIntermet=true;
			else if (pMessage->wMessageType&SMT_CLOSE_ROOM) bIntermet=true;
			if (bIntermet==true) 
			{
				g_GlobalAttemper.DestroyStatusWnd(this);
				m_ClientSocket->CloseSocket();
				CloseGameClient();
			}

			//��ʾ��Ϣ
			if (pMessage->wMessageType&SMT_INFO) m_MessageProxyHelper->InsertSystemString(pMessage->szContent,MS_NORMAL);
			if (pMessage->wMessageType&SMT_EJECT) ShowMessageBox(pMessage->szContent,MB_ICONINFORMATION);

			//�رշ���
			if (pMessage->wMessageType&SMT_CLOSE_ROOM) 
			{
				IRoomViewItem * pIRoomViewItem=QUERY_ME_INTERFACE(IRoomViewItem);
				((CGameFrame*)AfxGetMainWnd())->CloseRoomViewItem(pIRoomViewItem);
			}

			return true;
		}
	}

	return true;
}

//������Ϣ
bool CRoomViewItem::OnSocketMainServerInfo(CMD_Command Command, void * pData, WORD wDataSize)
{
	ASSERT(Command.wMainCmdID==MDM_GR_SERVER_INFO);
	switch (Command.wSubCmdID)
	{
	case SUB_GR_ONLINE_COUNT_INFO:		//������Ϣ
		{
			//Ч������
			ASSERT(wDataSize%sizeof(tagOnLineCountInfo)==0);
			if (wDataSize%sizeof(tagOnLineCountInfo)!=0) return false;

			//��Ϣ����
			WORD wKindID=0;
			DWORD dwKindOnLineCount=0,dwAllOnLineCount=0L;
			WORD wInfoCount=wDataSize/sizeof(tagOnLineCountInfo);
			tagOnLineCountInfo * pOnLineCountInfo=(tagOnLineCountInfo *)pData;
			for (int i=0;i<wInfoCount;i++)
			{
				wKindID=(pOnLineCountInfo+i)->wKindID;
				dwKindOnLineCount=(pOnLineCountInfo+i)->dwOnLineCount;
				dwAllOnLineCount+=dwKindOnLineCount;
				g_GlobalUnits.m_ServerListManager.UpdateGameKindOnLine(wKindID,dwKindOnLineCount);
			}

			//��������
			g_GlobalUnits.m_ServerListManager.UpdateGameOnLineCount(dwAllOnLineCount);

			return true;
		}
	}

	return true;
}

//��Ϸ��Ϣ
bool CRoomViewItem::OnSocketMainGameFrame(CMD_Command Command, void * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize<=SOCKET_PACKET);
	if (wDataSize>SOCKET_PACKET) return false;

	//��������
	IPC_SocketPackage SocketPackage;
	memset(&SocketPackage,0,sizeof(SocketPackage));
	SocketPackage.Command=Command;
	if (wDataSize>0)
	{
		ASSERT(pData!=NULL);
		CopyMemory(SocketPackage.cbBuffer,pData,wDataSize);
	}

	//���ݻ���
	if (m_hWndChannel==NULL)
	{
		//״̬�ж�
		ASSERT(m_pMeUserItem!=NULL);
		if (m_pMeUserItem==NULL) return false;

		//��ȡ�û�
		tagUserData * pUserData=m_pMeUserItem->GetUserData();

		//���ݻ���
		if ((pUserData->wTableID!=INVALID_TABLE)&&(pUserData->wChairID!=INVALID_CHAIR))
		{
			//���ñ���
			m_wPacketTableID=pUserData->wTableID;
			m_wPacketChairID=pUserData->wChairID;

			//��������
			BYTE cbBuffer[SOCKET_BUFFER];
			CopyMemory(cbBuffer,&Command,sizeof(Command));
			CopyMemory(&cbBuffer[sizeof(Command)],pData,wDataSize);

			//��������
			m_PacketDataStorage.InsertData(0,cbBuffer,sizeof(Command)+wDataSize);
		}
	}
	else
	{
		//��������
		WORD wSendSize=sizeof(SocketPackage.Command)+wDataSize;
		SendProcessData(IPC_MAIN_SOCKET,IPC_SUB_SOCKET_RECV,&SocketPackage,wSendSize);
	}

	//������Ŀ
	if ( Command.wSubCmdID == SUB_GF_RESIDUAL_PROPERTY )
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(CMD_GF_ResidualProperty));
		if (wDataSize!=sizeof(CMD_GF_ResidualProperty)) return false;

		//��������
		CMD_GF_ResidualProperty * ResidualProperty=(CMD_GF_ResidualProperty *)pData;

		//��ȡ���
		tagUserData *pMeUserData = m_pMeUserItem->GetUserData();

		//���õ���
		for ( WORD wPropID = 0; wPropID < PROPERTY_COUNT; ++wPropID )
		{
			pMeUserData->dwPropResidualTime[wPropID] = ResidualProperty->dwResidualTime[wPropID];
		}
	}
	//������Ϣ
	else if ( Command.wSubCmdID == SUB_GF_PROP_ATTRIBUTE )
	{
		//У�����
		ASSERT( wDataSize % sizeof(tagPropertyInfo) == 0 );
		if ( wDataSize % sizeof(tagPropertyInfo) != 0 ) return true;

		//����ת��
		int nInfoCount = wDataSize / sizeof(tagPropertyInfo);
		tagPropertyInfo *pPropertyInfo = (tagPropertyInfo*)pData;

		//��������
		for ( INT_PTR nIndex = 0; nIndex < nInfoCount; ++nIndex )
			m_PropertyAttributeArrary.Add(pPropertyInfo[nIndex]);
		
		//��ȡ����
		CPropertyBar *pPropertyBar = CPropertyBar::GetInstance();
		ASSERT( pPropertyBar != NULL );
		if ( pPropertyBar == NULL ) return false;

		//��ʼ�ṹ
		for ( int nIndex = 0; nIndex < nInfoCount; ++nIndex )
		{
			pPropertyBar->SetPropertyInfo((pPropertyInfo+nIndex)->nPropertyID, *(pPropertyInfo+nIndex));
		}
	}
	//�ʻ�����
	else if ( Command.wSubCmdID == SUB_GF_FLOWER_ATTRIBUTE )
	{
		//У�����
		ASSERT( wDataSize % sizeof(tagFlowerInfo) == 0 );
		if ( wDataSize % sizeof(tagFlowerInfo) != 0 ) return true;

		//����ת��
		int nInfoCount = wDataSize / sizeof(tagFlowerInfo);
		tagFlowerInfo *pFlowerInfo= (tagFlowerInfo*)pData;

		//��������
		for ( INT_PTR nIndex = 0; nIndex < nInfoCount; ++nIndex )
			m_FlowerAttributeArrary.Add(pFlowerInfo[nIndex]);

		//��ȡ����
		CPropertyBar *pPropertyBar = CPropertyBar::GetInstance();
		ASSERT( pPropertyBar != NULL );
		if ( pPropertyBar == NULL ) return false;

		//��ʼ�ṹ
		for ( int nIndex = 0; nIndex < nInfoCount; ++nIndex )
		{
			pPropertyBar->SetFlowerInfo((pFlowerInfo+nIndex)->nFlowerID, *(pFlowerInfo+nIndex));
		}

	}
	//������Ϣ
	else if ( Command.wSubCmdID == SUB_GF_PROP_BUGLE )
	{
		ASSERT( sizeof(CMD_GF_BugleProperty) == wDataSize );
		if ( sizeof(CMD_GF_BugleProperty) != wDataSize ) return false;

		//����ת��
		CMD_GF_BugleProperty *pBugleProperty = (CMD_GF_BugleProperty*)pData;

		//����ͼƬ
		CBitmap Bitmap;
		AfxSetResourceHandle(GetResInstanceHandle());
		if ( Bitmap.LoadBitmap(g_GlobalUnits.m_ChatExpViewImage.uChatBugle) )
		{
			m_MessageProxyHelper->InsertImage(&Bitmap);
			Bitmap.DeleteObject();
		}
		AfxSetResourceHandle(GetModuleHandle(NULL));

		//��������
		m_MessageProxyHelper->InsertUserChat(pBugleProperty->szUserName,pBugleProperty->szContext,pBugleProperty->crText,MS_NORMAL);

		//��������
		HINSTANCE hInstance = AfxGetInstanceHandle();
		HRSRC hResour=FindResource(hInstance,TEXT("BUGLE"),TEXT("WAVE"));
		if (hResour!=NULL)
		{
			HGLOBAL hGlobal=LoadResource(hInstance,hResour);
			if (hGlobal!=NULL) 
			{
				//��������
				LPCTSTR pszMemory=(LPCTSTR)LockResource(hGlobal);
				BOOL bResult = ::PlaySound(pszMemory,hInstance,SND_ASYNC|SND_MEMORY|SND_NODEFAULT);

				//������Դ
				UnlockResource(hGlobal);
			}
			FreeResource(hGlobal);
		}
	}
	//��������
	else if( Command.wSubCmdID == SUB_GF_FLOWER )
	{
		//Ч�����
		ASSERT( sizeof(CMD_GF_GiftNotify) == wDataSize );
		if ( sizeof(CMD_GF_GiftNotify) != wDataSize ) return false;

		//��������
		CMD_GF_GiftNotify * pGiftNotify=(CMD_GF_GiftNotify *)pData;

		//��������
		if(pGiftNotify->cbSendLocation==LOCATION_PLAZA_ROOM)
		{
			//��ȡ���
			IUserItem * pISendUserItem=m_ClientUserManager.SearchUserByUserID(pGiftNotify->dwSendUserID);
			IUserItem * pIRcvUserItem=m_ClientUserManager.SearchUserByUserID(pGiftNotify->dwRcvUserID);
			
			ASSERT( pISendUserItem != NULL && pIRcvUserItem != NULL );
			if ( pISendUserItem == NULL || pIRcvUserItem == NULL ) return true;

			tagUserData const *pSendUserData = pISendUserItem->GetUserData();
			tagUserData const *pRcvUserData = pIRcvUserItem->GetUserData();

			//����ж�
			ASSERT( pSendUserData != NULL && pRcvUserData != NULL );
			if ( pSendUserData == NULL || pRcvUserData == NULL ) return true;
			ASSERT( pGiftNotify->wGiftID < FLOWER_COUNT ) ;
			if ( pGiftNotify->wGiftID >= FLOWER_COUNT ) return true;

			//������Ϣ
			CString strGiftMsg;
			int nFlowerID = pGiftNotify->wGiftID;

			strGiftMsg.Format(TEXT("[%s]"),pSendUserData->szName);
			m_MessageProxyHelper->InsertShtickString(strGiftMsg,RGB(0,0,255), strGiftMsg.GetLength(), false);

			strGiftMsg = g_FlowerDescribe[nFlowerID].szAction;
			m_MessageProxyHelper->InsertShtickString(strGiftMsg,RGB(255,0,255), strGiftMsg.GetLength(), false);

			strGiftMsg.Format(TEXT("[%s]"),pRcvUserData->szName);
			m_MessageProxyHelper->InsertShtickString(strGiftMsg,RGB(0,0,255),strGiftMsg.GetLength(),false);

			strGiftMsg = g_FlowerDescribe[nFlowerID].szQuantifier1;
			m_MessageProxyHelper->InsertShtickString(strGiftMsg,RGB(255,0,255), strGiftMsg.GetLength(), false);

			//����ͼƬ
			CBitmap Bitmap;
			AfxSetResourceHandle(GetResInstanceHandle());
			if ( Bitmap.LoadBitmap(g_GlobalUnits.m_ChatExpViewImage.uChatGift[nFlowerID]) )
			{
				m_MessageProxyHelper->InsertImage(&Bitmap);
				Bitmap.DeleteObject();
			}
			AfxSetResourceHandle(GetModuleHandle(NULL));

			WORD wFlowerCount = pGiftNotify->wFlowerCount;
			strGiftMsg.Format(TEXT("����%d%s%s[ %s ]"), pGiftNotify->wFlowerCount, g_FlowerDescribe[nFlowerID].szQuantifier2,
				g_FlowerDescribe[nFlowerID].szResult, pRcvUserData->szName);
			m_MessageProxyHelper->InsertShtickString(strGiftMsg,RGB(255,0,255), strGiftMsg.GetLength(), false);	

			strGiftMsg.Format(g_FlowerDescribe[nFlowerID].szCharmResult, wFlowerCount * abs(CPropertyBar::m_FlowerInfo[nFlowerID].lRcvUserCharm));
			m_MessageProxyHelper->InsertShtickString(strGiftMsg,RGB(255,0,255), strGiftMsg.GetLength(), true);	
		}

	}

	return true;
}

//�û�����
bool CRoomViewItem::OnSocketSubUserCome(CMD_Command Command, void * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(Command.wMainCmdID==MDM_GR_USER);
	ASSERT(Command.wSubCmdID==SUB_GR_USER_COME);
	ASSERT(wDataSize>=sizeof(tagUserInfoHead));
	if (wDataSize<sizeof(tagUserInfoHead)) return false;

	//��ȡ������Ϣ
	tagUserData UserData;
	memset(&UserData,0,sizeof(UserData));
	tagUserInfoHead * pUserInfoHead=(tagUserInfoHead *)pData;

	//��ȡ��Ϣ
	UserData.dwUserID=pUserInfoHead->dwUserID;
	UserData.wTableID=pUserInfoHead->wTableID;
	UserData.wChairID=pUserInfoHead->wChairID;
	UserData.cbUserStatus=pUserInfoHead->cbUserStatus;
	UserData.dwUserRight=pUserInfoHead->dwUserRight;
	UserData.dwMasterRight=pUserInfoHead->dwMasterRight;
	//UserData.lInsureScore = pUserInfoHead->lInsureScore;

	//�����ж�
	if ((pUserInfoHead->dwUserID==g_GlobalUnits.GetGolbalUserData().dwUserID)&&(pUserInfoHead->cbMasterOrder>=2))
	{
		m_cbHideUserInfo=FALSE;
	}

	//��ȡ��Ϣ
	if ((m_cbHideUserInfo==FALSE)||(pUserInfoHead->dwUserID==g_GlobalUnits.GetGolbalUserData().dwUserID))
	{
		UserData.wFaceID=pUserInfoHead->wFaceID;
		UserData.dwCustomFaceVer=pUserInfoHead->dwCustomFaceVer;
		UserData.cbGender=pUserInfoHead->cbGender;
		UserData.cbMemberOrder=pUserInfoHead->cbMemberOrder;
		UserData.cbMasterOrder=pUserInfoHead->cbMasterOrder;
		UserData.dwGameID=pUserInfoHead->dwGameID;
		UserData.dwGroupID=pUserInfoHead->dwGroupID;
		UserData.lLoveliness=pUserInfoHead->lLoveliness;
		UserData.lScore=pUserInfoHead->UserScoreInfo.lScore;
		UserData.lGameGold=pUserInfoHead->UserScoreInfo.lGameGold;
		UserData.lInsureScore=pUserInfoHead->UserScoreInfo.lInsureScore;
		UserData.lWinCount=pUserInfoHead->UserScoreInfo.lWinCount;
		UserData.lLostCount=pUserInfoHead->UserScoreInfo.lLostCount;
		UserData.lDrawCount=pUserInfoHead->UserScoreInfo.lDrawCount;
		UserData.lFleeCount=pUserInfoHead->UserScoreInfo.lFleeCount;
		UserData.lExperience=pUserInfoHead->UserScoreInfo.lExperience;
	}

	//��ȡ��չ��Ϣ
	void * pDataBuffer=NULL;
	tagDataDescribe DataDescribe;
	CRecvPacketHelper RecvPacket(pUserInfoHead+1,wDataSize-sizeof(tagUserInfoHead));
	while (true)
	{
		pDataBuffer=RecvPacket.GetData(DataDescribe);
		if (DataDescribe.wDataDescribe==DTP_NULL) break;
		switch (DataDescribe.wDataDescribe)
		{
		case DTP_USER_ACCOUNTS:		//�û��ʻ�
			{
				if ((m_cbHideUserInfo==FALSE)||(pUserInfoHead->dwUserID==g_GlobalUnits.GetGolbalUserData().dwUserID))
				{
					ASSERT(pDataBuffer!=NULL);
					ASSERT(DataDescribe.wDataSize<=sizeof(UserData.szName));
					if (DataDescribe.wDataSize<=sizeof(UserData.szName))
					{
						CopyMemory(&UserData.szName,pDataBuffer,DataDescribe.wDataSize);
						UserData.szName[CountArray(UserData.szName)-1]=0;
					}
				}
				else
				{
					lstrcpyn(UserData.szName,TEXT("��Ϸ�û�"),CountArray(UserData.szName));
				}
				break;
			}
		case DTP_UNDER_WRITE:		//����ǩ��
			{
				if ((m_cbHideUserInfo==FALSE)||(pUserInfoHead->dwUserID==g_GlobalUnits.GetGolbalUserData().dwUserID))
				{
					ASSERT(pDataBuffer!=NULL);
					ASSERT(DataDescribe.wDataSize<=sizeof(UserData.szUnderWrite));
					if (DataDescribe.wDataSize<=sizeof(UserData.szUnderWrite))
					{
						CopyMemory(&UserData.szUnderWrite,pDataBuffer,DataDescribe.wDataSize);
						UserData.szUnderWrite[CountArray(UserData.szUnderWrite)-1]=0;
					}
				}
				break;
			}
		case DTP_USER_GROUP_NAME:	//�û�����
			{
				if ((m_cbHideUserInfo==FALSE)||(pUserInfoHead->dwUserID==g_GlobalUnits.GetGolbalUserData().dwUserID))
				{
					ASSERT(pDataBuffer!=NULL);
					ASSERT(DataDescribe.wDataSize<=sizeof(UserData.szGroupName));
					if (DataDescribe.wDataSize<=sizeof(UserData.szGroupName))
					{
						CopyMemory(&UserData.szGroupName,pDataBuffer,DataDescribe.wDataSize);
						UserData.szGroupName[CountArray(UserData.szGroupName)-1]=0;
					}
				}
				break;
			}
		}
	}

	//�����û�
	IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(UserData.dwUserID);
	if (pIUserItem==NULL) 
	{
		const tagCompanionItem * pCompanionItem=NULL;
		pCompanionItem=g_GlobalUnits.m_CompanionManager->SearchCompanionItem(UserData.dwUserID);
		if (pCompanionItem!=NULL) UserData.cbCompanion=pCompanionItem->Companion;
		pIUserItem=m_ClientUserManager.ActiveUserItem(UserData);
	}
	else OnUserItemUpdate(pIUserItem);

	//�����û���Ϣ
	ASSERT(pIUserItem!=NULL);
	if (pIUserItem!=NULL)
	{
		//�ж��Լ�
		if (m_pMeUserItem==NULL) m_pMeUserItem=pIUserItem;

		//���ý���
		BYTE cbUserStatus=UserData.cbUserStatus;
		if ((cbUserStatus>=US_SIT)&&(cbUserStatus!=US_LOOKON))
			m_TableFrame.SetUserInfo(UserData.wTableID,UserData.wChairID,pIUserItem);

		//��ʾ��Ϣ
		if ((m_cbHideUserInfo==FALSE)&&(m_ServiceStatus==ServiceStatus_Serviceing))
		{
			if ((UserData.cbCompanion==enCompanion_Friend)||(UserData.dwMasterRight!=0L))
			{
				TCHAR szMessage[256]=TEXT("");
				_snprintf(szMessage,sizeof(szMessage),TEXT("[ %s ] ������"),UserData.szName);
				m_MessageProxyHelper->InsertSystemString(szMessage,0);
			}
			else if (g_GlobalOption.m_bShowInOutMessage==true)
			{
				TCHAR szMessage[256]=TEXT("");
				_snprintf(szMessage,sizeof(szMessage),TEXT("[ %s ] ������"),UserData.szName);
				m_MessageProxyHelper->InsertGeneralString(szMessage,0,0,true);
			}
		}
	}

	//�Զ�ͷ��
	if(UserData.dwCustomFaceVer!=0)
	{
		//ͷ������
		CString strDirName = CString(g_GlobalUnits.GetWorkDirectory()) + TEXT("\\CustomFace");
		CString strFileName;
		strFileName.Format(TEXT("\\%ld_%d.bmp"), pUserInfoHead->dwUserID, pUserInfoHead->dwCustomFaceVer);

		//��ȡ�ļ�
		CImage FaceImage;
		HRESULT hResult = FaceImage.Load(strDirName + strFileName);
		if (SUCCEEDED(hResult))
		{
			FaceImage.Destroy();
		}
		//����ͷ��
		else
		{
			//Ͷ����Ϣ
			CPlazaViewItem *pPlazaViewItem = ((CGameFrame *)AfxGetMainWnd())->GetPlazaViewItem();
			pPlazaViewItem->PostMessage(WM_DOWN_LOAD_FACE, LPARAM(pUserInfoHead->dwCustomFaceVer), WPARAM(pUserInfoHead->dwUserID));
		}
	}

	//���±���
	UpdateTitleText();

	//��������
	DWORD dwOnLineCount=m_ClientUserManager.GetOnLineCount();
	g_GlobalUnits.m_ServerListManager.UpdateGameServerOnLine(m_pListServer,dwOnLineCount);

	return true;
}

//�û�״̬
bool CRoomViewItem::OnSocketSubStatus(CMD_Command Command, void * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(Command.wMainCmdID==MDM_GR_USER);
	ASSERT(Command.wSubCmdID==SUB_GR_USER_STATUS);
	ASSERT(wDataSize>=sizeof(CMD_GR_UserStatus));
	if (wDataSize<sizeof(CMD_GR_UserStatus)) return false;

	//��������
	CMD_GR_UserStatus * pUserStatus=(CMD_GR_UserStatus *)pData;
	IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(pUserStatus->dwUserID);
	ASSERT(pIUserItem!=NULL);
	if (pIUserItem==NULL) return true;

	//�������
	tagUserData * pUserData=pIUserItem->GetUserData();
	tagUserData * pMeUserData=m_pMeUserItem->GetUserData();
	WORD wNowTableID=pUserStatus->wTableID,wLastTableID=pUserData->wTableID;
	WORD wNowChairID=pUserStatus->wChairID,wLastChairID=pUserData->wChairID;
	BYTE cbNowStatus=pUserStatus->cbUserStatus,cbLastStatus=pUserData->cbUserStatus;

	//�����״̬
	if ((wLastTableID!=INVALID_TABLE)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		ASSERT(wLastChairID!=INVALID_CHAIR);
		IUserItem * pITableUserItem=m_TableFrame.GetUserInfo(wLastTableID,wLastChairID);
		if (pITableUserItem==pIUserItem) m_TableFrame.SetUserInfo(wLastTableID,wLastChairID,NULL);
	}

	//�û��뿪
	if (cbNowStatus==US_NULL)
	{
		//֪ͨ��Ϸ
		if ((pMeUserData->wTableID!=INVALID_TABLE)&&(pMeUserData->wTableID==wLastTableID))
		{
			IPC_UserStatus UserStatus;
			memset(&UserStatus,0,sizeof(UserStatus));
			UserStatus.dwUserID=pUserData->dwUserID;
			UserStatus.cbUserStatus=pUserData->cbUserStatus;
			SendProcessData(IPC_MAIN_USER,IPC_SUB_USER_STATUS,&UserStatus,sizeof(UserStatus));
		}

		//ɾ���û�
		m_ClientUserManager.DeleteUserItem(pIUserItem);

		//���±���
		UpdateTitleText();

		//��������
		DWORD dwOnLineCount=m_ClientUserManager.GetOnLineCount();
		g_GlobalUnits.m_ServerListManager.UpdateGameServerOnLine(m_pListServer,dwOnLineCount);

		return true;
	}

	//����״̬
	tagUserStatus UserStatus;
	UserStatus.wTableID=wNowTableID;
	UserStatus.wChairID=wNowChairID;
	UserStatus.cbUserStatus=cbNowStatus;
	m_ClientUserManager.UpdateUserItemStatus(pIUserItem,&UserStatus);

	//������״̬
	if ((wNowTableID!=INVALID_TABLE)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		//��������
		if (cbNowStatus!=US_LOOKON)
		{
			ASSERT(wNowChairID!=INVALID_CHAIR);
			m_TableFrame.SetUserInfo(wNowTableID,wNowChairID,pIUserItem);
		}

		//�����û�
		if ((m_pMeUserItem!=pIUserItem)&&(pMeUserData->wTableID==wNowTableID))
		{
			CIPCSendCopyData SendCopyData(m_hWndChannel,m_hWnd);
			SendTableUser(pIUserItem,&SendCopyData);
		}
	}

	//���½���
	if ((wNowTableID!=INVALID_TABLE)&&(cbNowStatus!=US_LOOKON)
		&&(wNowTableID==wLastTableID)&&(wNowChairID==wLastChairID))
	{
		ASSERT(wNowChairID!=INVALID_CHAIR);
		m_TableFrame.UpdateTableView(wNowTableID,false);
	}

	//�жϷ���
	bool bNotifyGame=false;
	if (pIUserItem==m_pMeUserItem) bNotifyGame=true;
	else if ((pMeUserData->wTableID!=INVALID_TABLE)&&(pMeUserData->wTableID==wNowTableID)) bNotifyGame=true;
	else if ((pMeUserData->wTableID!=INVALID_TABLE)&&(pMeUserData->wTableID==wLastTableID)) bNotifyGame=true;

	//����״̬
	if (bNotifyGame==true)
	{
		IPC_UserStatus UserStatus;
		memset(&UserStatus,0,sizeof(UserStatus));
		UserStatus.dwUserID=pUserData->dwUserID;
		UserStatus.cbUserStatus=pUserData->cbUserStatus;
		SendProcessData(IPC_MAIN_USER,IPC_SUB_USER_STATUS,&UserStatus,sizeof(UserStatus));
	}

	//�л�λ���ж�
	if (cbLastStatus==US_FREE && cbNowStatus==US_PLAY && pMeUserData->cbUserStatus==US_LOOKON && 
		wNowTableID==pMeUserData->wTableID && wNowChairID==pMeUserData->wChairID) 
	{
		IPC_UserStatus UserStatus;
		memset(&UserStatus,0,sizeof(UserStatus));
		UserStatus.dwUserID=pMeUserData->dwUserID;
		UserStatus.cbUserStatus=US_FREE;
		SendProcessData(IPC_MAIN_USER,IPC_SUB_USER_STATUS,&UserStatus,sizeof(UserStatus));
		StartGameClient();
	}

	//�ж��Լ�
	if (pIUserItem==m_pMeUserItem)
	{
		//���ñ���
		if ((wNowTableID==m_wReqTableID)&&(wNowChairID==m_wReqChairID))
		{
			m_wReqTableID=INVALID_TABLE;
			m_wReqChairID=INVALID_CHAIR;
		}

		//�ټ����ж�
		if (m_TableFrame.GetChairCount()>MAX_CHAIR_NORMAL && US_SIT==cbNowStatus)
		{
			m_wReqTableID=INVALID_TABLE;
			m_wReqChairID=INVALID_CHAIR;
		}

		//��������
		if ((m_wPacketTableID!=INVALID_TABLE)&&((m_wPacketTableID!=wNowTableID)||(m_wPacketChairID!=wNowChairID)))
		{
			m_wPacketTableID=INVALID_TABLE;
			m_wPacketChairID=INVALID_CHAIR;
			m_PacketDataStorage.RemoveData(false);
		}

		//������Ϸ
		if ((wNowTableID!=INVALID_TABLE)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
		{
			int iResult=StartGameClient();
		}
	}

	return true;
}

//�û�����
bool CRoomViewItem::OnSocketSubScore(CMD_Command Command, void * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(Command.wMainCmdID==MDM_GR_USER);
	ASSERT(Command.wSubCmdID==SUB_GR_USER_SCORE);
	ASSERT(wDataSize>=sizeof(CMD_GR_UserScore));
	if (wDataSize<sizeof(CMD_GR_UserScore)) return false;

	//��������
	CMD_GR_UserScore * pUserScore=(CMD_GR_UserScore *)pData;
	IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(pUserScore->dwUserID);
	ASSERT(pIUserItem!=NULL);
	if (pIUserItem==NULL) return true;

	//�����ж�
	if ((m_cbHideUserInfo==FALSE)||(pIUserItem==m_pMeUserItem))
	{
		//���·���
		tagUserData * pUserData=pIUserItem->GetUserData();
		pUserData->lLoveliness = pUserScore->lLoveliness;
		pUserData->lGameGold = pUserScore->UserScore.lGameGold;
		pUserData->lInsureScore = pUserScore->UserScore.lInsureScore;
		m_ClientUserManager.UpdateUserItemScore(pIUserItem,&pUserScore->UserScore);

		//������Ϸ
		tagUserData * pMeUserData=m_pMeUserItem->GetUserData();
		if ((pMeUserData->wTableID!=INVALID_TABLE)&&(pMeUserData->wTableID==pUserData->wTableID))
		{
			IPC_UserScore UserScore;
			memset(&UserScore,0,sizeof(UserScore));
			UserScore.dwUserID=pUserScore->dwUserID;
			UserScore.UserScore=pUserScore->UserScore;
			UserScore.lLoveliness = pUserScore->lLoveliness;
			SendProcessData(IPC_MAIN_USER,IPC_SUB_USER_SCORE,&UserScore,sizeof(UserScore));
		}
	}

	return true;
}

//�û�Ȩ��
bool CRoomViewItem::OnSocketSubRight(CMD_Command Command, void * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(Command.wMainCmdID==MDM_GR_USER);
	ASSERT(Command.wSubCmdID==SUB_GR_USER_RIGHT);
	ASSERT(wDataSize==sizeof(CMD_GR_UserRight));
	if (wDataSize!=sizeof(CMD_GR_UserRight)) return false;

	//��Ϣ����
	CMD_GR_UserRight * pUserRight=(CMD_GR_UserRight *)pData;

	//�û�Ȩ��
	IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(pUserRight->dwUserID);
	pIUserItem->GetUserData()->dwUserRight = pUserRight->dwUserRight;

	return true;
}

//��Ա�ȼ�
bool CRoomViewItem::OnSocketSubMemberOrder(CMD_Command Command, void * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(Command.wMainCmdID==MDM_GR_USER);
	ASSERT(Command.wSubCmdID==SUB_GR_MEMBER_ORDER);
	ASSERT(wDataSize==sizeof(CMD_GR_MemberOrder));
	if (wDataSize!=sizeof(CMD_GR_MemberOrder)) return false;

	//��Ϣ����
	CMD_GR_MemberOrder * pMemberOrder=(CMD_GR_MemberOrder *)pData;

	//��Ա�ȼ�
	IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(pMemberOrder->dwUserID);
	tagUserData * pUserData=pIUserItem->GetUserData();
	pUserData->cbMemberOrder = pMemberOrder->cbMemberOrder;
	tagGlobalUserData & GlobalUserInfo=g_GlobalUnits.GetGolbalUserData();
	GlobalUserInfo.cbMember = pMemberOrder->cbMemberOrder;

	//������Ϸ
	tagUserData * pMeUserData=m_pMeUserItem->GetUserData();
	if ((pMeUserData->wTableID!=INVALID_TABLE)&&(pMeUserData->wTableID==pUserData->wTableID))
	{
		IPC_MemberOrder MemberOrder;
		memset(&MemberOrder,0,sizeof(MemberOrder));
		MemberOrder.dwUserID=pUserData->dwUserID;
		MemberOrder.cbMember=pUserData->cbMemberOrder;
		SendProcessData(IPC_MAIN_USER,IPC_SUB_MEMBERORDER,&MemberOrder,sizeof(MemberOrder));
	}

	return true;
}

//����ʧ��
bool CRoomViewItem::OnSocketSubSitFailed(CMD_Command Command, void * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(Command.wMainCmdID==MDM_GR_USER);
	ASSERT(Command.wSubCmdID==SUB_GR_SIT_FAILED);

	//��Ϣ����
	CMD_GR_SitFailed * pSitFailed=(CMD_GR_SitFailed *)pData;
	IUserItem * pISendUserItem=m_TableFrame.GetUserInfo(m_wReqTableID,m_wReqChairID);
	if (pISendUserItem==m_pMeUserItem) m_TableFrame.SetUserInfo(m_wReqTableID,m_wReqChairID,NULL);

	//���ñ���
	m_wReqTableID=INVALID_TABLE;
	m_wReqChairID=INVALID_CHAIR;

	//��ʾ��Ϣ
	ShowMessageBox(pSitFailed->szFailedDescribe,MB_ICONINFORMATION);

	return true;
}

//�û�����
bool CRoomViewItem::OnSocketSubChat(CMD_Command Command, void * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(Command.wMainCmdID==MDM_GR_USER);
	ASSERT(Command.wSubCmdID==SUB_GR_USER_CHAT);

	//Ч�����
	CMD_GR_UserChat * pUserChat=(CMD_GR_UserChat *)pData;
	ASSERT(wDataSize>=(sizeof(CMD_GR_UserChat)-sizeof(pUserChat->szChatMessage)));
	ASSERT(wDataSize==(sizeof(CMD_GR_UserChat)-sizeof(pUserChat->szChatMessage)+pUserChat->wChatLength));
	if (wDataSize<(sizeof(CMD_GR_UserChat)-sizeof(pUserChat->szChatMessage))) return false;
	if (wDataSize!=(sizeof(CMD_GR_UserChat)-sizeof(pUserChat->szChatMessage)+pUserChat->wChatLength)) return false;

	//Ѱ���û�
	IUserItem * pISendUserItem=m_ClientUserManager.SearchUserByUserID(pUserChat->dwSendUserID);
	if (pISendUserItem==NULL) return true;
	tagUserData * pSendUserData=pISendUserItem->GetUserData();

	//��Ϣ����
	if ((pISendUserItem!=m_pMeUserItem)&&(pSendUserData->cbCompanion==enCompanion_Detest)) return true;
	
	//��ʾ��Ϣ
	if (pUserChat->dwTargetUserID!=0L)
	{
		IUserItem * pIRecvUserItem=m_ClientUserManager.SearchUserByUserID(pUserChat->dwTargetUserID);
		if (pIRecvUserItem==NULL) return true;
		tagUserData * pRecvUserData=pIRecvUserItem->GetUserData();
		m_MessageProxyHelper->InsertUserChat(pSendUserData->szName,pRecvUserData->szName,pUserChat->szChatMessage,pUserChat->crFontColor,MS_NORMAL);
	}
	else m_MessageProxyHelper->InsertUserChat(pSendUserData->szName,pUserChat->szChatMessage,pUserChat->crFontColor,MS_NORMAL);

	return true;
}

//�û�˽��
bool CRoomViewItem::OnSocketSubWisper(CMD_Command Command, void * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(Command.wMainCmdID==MDM_GR_USER);
	ASSERT(Command.wSubCmdID==SUB_GR_USER_WISPER);

	//Ч�����
	CMD_GR_Wisper * pWisper=(CMD_GR_Wisper *)pData;
	ASSERT(wDataSize>=(sizeof(CMD_GR_Wisper)-sizeof(pWisper->szChatMessage)));
	ASSERT(wDataSize==(sizeof(CMD_GR_Wisper)-sizeof(pWisper->szChatMessage)+pWisper->wChatLength));
	if (wDataSize<(sizeof(CMD_GR_Wisper)-sizeof(pWisper->szChatMessage))) return false;
	if (wDataSize!=(sizeof(CMD_GR_Wisper)-sizeof(pWisper->szChatMessage)+pWisper->wChatLength)) return false;

	//Ѱ���û�
	IUserItem * pISendUserItem=m_ClientUserManager.SearchUserByUserID(pWisper->dwSendUserID);
	IUserItem * pIRecvUserItem=m_ClientUserManager.SearchUserByUserID(pWisper->dwTargetUserID);
	if ((pISendUserItem==NULL)||(pIRecvUserItem==NULL)) return true;
	tagUserData * pUserDataSend=pISendUserItem->GetUserData();
	tagUserData * pUserDataRecv=pIRecvUserItem->GetUserData();

	//��Ϣ����
	if ((pUserDataSend->cbMasterOrder==0) && (pISendUserItem!=m_pMeUserItem)&&(pUserDataSend->cbCompanion==enCompanion_Detest)) return true;
	
	//��ʾ��Ϣ
	CShortMessage * pShortMessageWnd=NULL;
	if (pWisper->dwSendUserID==m_pMeUserItem->GetUserID())
	{
		//�Լ����͵���Ϣ
		pShortMessageWnd=ActiveShortWnd(pWisper->dwTargetUserID,pIRecvUserItem,true);
		if (pShortMessageWnd!=NULL) pShortMessageWnd->OnRecvMessage(pUserDataSend->szName,pWisper->szChatMessage,pWisper->crFontColor,true);
	}
	else	//������Ϸ�߷�����Ϣ
	{
		pShortMessageWnd=ActiveShortWnd(pWisper->dwSendUserID,pISendUserItem,true);
		if (pShortMessageWnd!=NULL)	pShortMessageWnd->OnRecvMessage(pUserDataSend->szName,pWisper->szChatMessage,pWisper->crFontColor,false);
	}

	return true;
}

//�û�����
bool CRoomViewItem::OnSocketSubUserInvite(CMD_Command Command, void * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(Command.wMainCmdID==MDM_GR_USER);
	ASSERT(Command.wSubCmdID==SUB_GR_USER_INVITE);

	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_UserInvite));
	if (wDataSize!=sizeof(CMD_GR_UserInvite)) return false;

	//��Ϣ����
	CMD_GR_UserInvite * pUserInvite=(CMD_GR_UserInvite *)pData;
	ASSERT(pUserInvite->wTableID<m_TableFrame.GetTableCount());
	if (m_TableFrame.QueryPlayFlag(pUserInvite->wTableID)==true) return true;

	//Ѱ���û�
	IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(pUserInvite->dwUserID);
	if (pIUserItem==NULL) return true;
	tagUserData * pUserData=pIUserItem->GetUserData();
	if (pUserData->wTableID==INVALID_TABLE) return true;

	//�û�����
	if (pUserData->cbCompanion==enCompanion_Detest) return true;
	if (g_GlobalOption.m_InviteMode==enInviteMode_None) return true;
	if ((g_GlobalOption.m_InviteMode==enInviteMode_Friend)&&(pUserData->cbCompanion!=enCompanion_Friend)) return true;

	//��ʾ��Ϣ
	TCHAR szMessage[256]=TEXT("");
	_snprintf(szMessage,sizeof(szMessage),TEXT("[ %s ] ���������� [ %ld ] ��Ϸ��������Ϸ���Ƿ�ͬ�⣿"),pUserData->szName,pUserInvite->wTableID+1);
	if (ShowMessageBox(szMessage,MB_ICONINFORMATION|MB_YESNO)==IDYES)
	{
		WORD wChairID=INVALID_CHAIR;
		WORD wNullCount=m_TableFrame.GetNullChairCount(pUserInvite->wTableID,wChairID);
		if (wNullCount==0) 
		{
			ShowMessageBox(TEXT("����Ϸ���Ѿ�û�п�λ���ˣ�"),MB_ICONINFORMATION);
			return true;
		}
		PerformSitDownAction(pUserInvite->wTableID,wChairID);
	}

	return true;
}

//IPC �ں�����
bool CRoomViewItem::OnIPCKernel(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{
	ASSERT(pHead->wMainCmdID==IPC_MIAN_IPC_KERNEL);
	switch (pHead->wSubCmdID)
	{
	case IPC_SUB_IPC_CLIENT_CONNECT:	//������Ϣ
		{
			//�ж�����
			if (::IsChild(m_pShareMemory->hWndGameFrame,hWndSend)) return false;

			//���ñ���
			ASSERT(m_hWndChannel==NULL);
			m_hWndChannel=hWndSend;

			//������Ϣ
			CIPCSendCopyData SendCopyData(m_hWndChannel,m_hWnd);
			IPCSendGameInfo(&SendCopyData);
			IPCSendTableUsers(&SendCopyData);
			SendCopyData.SendData(IPC_MAIN_CONCTROL,IPC_SUB_START_FINISH);

			//��������
			if ((m_wPacketTableID!=INVALID_TABLE)&&(m_wPacketChairID!=INVALID_CHAIR))
			{
				//��������
				tagDataHead DataHead;
				ZeroMemory(&DataHead,sizeof(DataHead));

				//��������
				BYTE cbBuffer[SOCKET_BUFFER];
				IPC_SocketPackage SocketPackage;
				ZeroMemory(cbBuffer,sizeof(cbBuffer));
				ZeroMemory(&SocketPackage,sizeof(SocketPackage));

				//��ȡ����
				do
				{
					//��ȡ����
					tagBurthenInfo BurthenInfo;
					m_PacketDataStorage.GetBurthenInfo(BurthenInfo);

					//��ȡ����
					if (BurthenInfo.dwDataPacketCount==0L) break;
					if (m_PacketDataStorage.DistillData(DataHead,cbBuffer,sizeof(cbBuffer))==false) break;

					//��������
					CopyMemory(&SocketPackage.Command,cbBuffer,sizeof(CMD_Command));
					CopyMemory(SocketPackage.cbBuffer,&cbBuffer[sizeof(CMD_Command)],DataHead.wDataSize-sizeof(CMD_Command));

					//���ݴ���
					SendProcessData(IPC_MAIN_SOCKET,IPC_SUB_SOCKET_RECV,&SocketPackage,DataHead.wDataSize);

				} while (true);

				//���ñ���
				m_wPacketTableID=INVALID_TABLE;
				m_wPacketChairID=INVALID_CHAIR;
			}

			//��������
			IPC_SocketPackage SocketPackage;
			ZeroMemory(&SocketPackage,sizeof(SocketPackage));
			WORD wSendSize=0;

			//������Ϣ
			SocketPackage.Command.wMainCmdID = MDM_GF_PRESENT;
			SocketPackage.Command.wSubCmdID = SUB_GF_PROP_ATTRIBUTE;

			//��������
			for (INT_PTR nIndex =0; nIndex < m_PropertyAttributeArrary.GetCount(); ++nIndex)
			{
				if ((wSendSize+sizeof(tagPropertyInfo))>SOCKET_PACKET)
				{
					SendProcessData(IPC_MAIN_SOCKET,IPC_SUB_SOCKET_RECV,&SocketPackage,wSendSize+sizeof(CMD_Command));
					wSendSize=0;
				}
				tagPropertyInfo &PropertyInfo = m_PropertyAttributeArrary[nIndex];
				CopyMemory(SocketPackage.cbBuffer+wSendSize,&PropertyInfo,sizeof(PropertyInfo));
				wSendSize+=sizeof(PropertyInfo);
			}
			if (wSendSize>0) SendProcessData(IPC_MAIN_SOCKET,IPC_SUB_SOCKET_RECV,&SocketPackage,wSendSize+sizeof(CMD_Command));

			//������Ϣ
			SocketPackage.Command.wSubCmdID = SUB_GF_FLOWER_ATTRIBUTE;

			//�ʻ�����
			wSendSize=0L;
			for (INT_PTR nIndex =0; nIndex < m_FlowerAttributeArrary.GetCount(); ++nIndex)
			{
				if ((wSendSize+sizeof(tagFlowerInfo))>SOCKET_PACKET)
				{
					SendProcessData(IPC_MAIN_SOCKET,IPC_SUB_SOCKET_RECV,&SocketPackage,wSendSize+sizeof(CMD_Command));
					wSendSize=0;
				}
				tagFlowerInfo &FlowerInfo = m_FlowerAttributeArrary[nIndex];
				CopyMemory(SocketPackage.cbBuffer+wSendSize,&FlowerInfo,sizeof(FlowerInfo));
				wSendSize+=sizeof(FlowerInfo);
			}
			if (wSendSize>0) SendProcessData(IPC_MAIN_SOCKET,IPC_SUB_SOCKET_RECV,&SocketPackage,wSendSize+sizeof(CMD_Command));

			return true;
		}
	case IPC_SUB_IPC_CLIENT_CLOSE:		//�ر���Ϣ
		{
			//�ж�����
			if (hWndSend!=m_hWndChannel) return false;

			//������Ϣ
			tagUserData * pUserData=m_pMeUserItem->GetUserData();
			if (pUserData->wTableID!=WORD(INVALID_TABLE))
			{
				if (pUserData->cbUserStatus==US_PLAY) SendLeftGamePacket();
				SendStandUpPacket();
			}

			//���ñ���
			m_hWndChannel=NULL;
			m_pShareMemory->hWndGameFrame=NULL;
			CloseHandle(m_GameProcessInfo.hThread);
			CloseHandle(m_GameProcessInfo.hProcess);
			memset(&m_GameProcessInfo,0,sizeof(m_GameProcessInfo));

			return true;
		}
	}

	return false;
}

//IPC ��������
bool CRoomViewItem::OnIPCSocket(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{
	ASSERT(pHead->wMainCmdID==IPC_MAIN_SOCKET);
	switch (pHead->wSubCmdID)
	{
	case IPC_SUB_SOCKET_SEND:	//������Ϣ
		{
			//Ч������
			ASSERT(wDataSize>=sizeof(CMD_Command));
			if (wDataSize<sizeof(CMD_Command)) 
			{
				return false;
			}

			//��������
			IPC_SocketPackage * pSocketPackage=(IPC_SocketPackage *)pIPCBuffer;
			CMD_Command *pCommand= &pSocketPackage->Command;
			WORD wSendSize=wDataSize-sizeof(CMD_Command);
			if (wSendSize==0) 
			{
				m_ClientSocket->SendData(pCommand->wMainCmdID,
									pCommand->wSubCmdID);
			}
			else 
			{
				m_ClientSocket->SendData(pCommand->wMainCmdID,pCommand->wSubCmdID,pSocketPackage->cbBuffer,wSendSize);
			}

			return true;
		}
	}

	return false;
}

//�ŵ����ݴ���
bool __cdecl CRoomViewItem::OnChannelMessage(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{
	switch (pHead->wMainCmdID)
	{
	case IPC_MIAN_IPC_KERNEL:		//�ں���Ϣ
		{
			return OnIPCKernel(pHead,pIPCBuffer,wDataSize,hWndSend);
		}
	case IPC_MAIN_SOCKET:			//�����¼�
		{
			return OnIPCSocket(pHead,pIPCBuffer,wDataSize,hWndSend);
		}
	case IPC_MAIN_CONCTROL:			//������Ϣ
		{
			//������Ϸ
			if (pHead->wSubCmdID==IPC_SUB_JOIN_IN_GAME)
			{
				IPC_JoinInGame * pJoinInGame=(IPC_JoinInGame *)pIPCBuffer;
				if ((pJoinInGame->wTableID!=INVALID_TABLE)&&(pJoinInGame->wChairID!=INVALID_CHAIR))
				{
					PerformSitDownAction(pJoinInGame->wTableID,pJoinInGame->wChairID);
				}
			}

			return true;
		}
	}
	return false;
}

//����������
void __cdecl CRoomViewItem::OnLButtonHitTable(WORD wTableID, WORD wChairID, bool bMirror)
{
	//�ж�״̬
	if (m_ServiceStatus==ServiceStatus_NetShutDown)
	{
		if ((wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR))
		{
			LPCTSTR szMessage=TEXT("����Ϸ�������������Ѿ��жϣ��Ƿ��˳���ǰ��Ϸ���䣿");
			int iCode=ShowMessageBox(szMessage,MB_ICONINFORMATION|MB_YESNO);
			if (iCode==IDYES) ((CGameFrame*)AfxGetMainWnd())->CloseRoomViewItem(this);
		}
		return;
	}

	//��������
	if (m_ServiceStatus!=ServiceStatus_Serviceing) return;
	if ((wTableID==m_wReqTableID)&&(wChairID==m_wReqChairID)) return;
	if ((wTableID==INVALID_TABLE)||(wChairID==INVALID_CHAIR)) return;

	//��Ϣ����
	IUserItem * pTableUserItem=m_TableFrame.GetUserInfo(wTableID,wChairID);
	if (pTableUserItem!=NULL)
	{
		//�Թۻ�������
		if (pTableUserItem==m_pMeUserItem)
		{
			if (m_pMeUserItem->GetUserData()->cbUserStatus>=US_PLAY) 
			{
				return;
			}
			if ((m_wReqTableID!=INVALID_TABLE)||(m_wReqChairID!=INVALID_CHAIR)) 
			{
				return;
			}
			m_wFindTableID=INVALID_TABLE;
			SendStandUpPacket();
		}
		else 
		{
			tagUserData * pUserData=m_pMeUserItem->GetUserData();
			if((pUserData->wTableID==wTableID)
				&& (pUserData->wChairID==wChairID)) 
			{
				return;
			}
			PerformLookonAction(wTableID,wChairID);
		}
	}
	else 
	{
		//���´���
		if (m_cbHideUserInfo==TRUE)
		{
			SendSitDownPacket(INVALID_TABLE,INVALID_CHAIR,NULL);
		}
		else
		{
			PerformSitDownAction(wTableID, wChairID);
		}
	}

	return;
}

//����Ҽ�����
void __cdecl CRoomViewItem::OnRButtonHitTable(WORD wTableID, 
											  WORD wChairID, 
											  bool bMirror)
{
	//��ȡ����
	ITableView * pITableView=m_TableFrame.GetTableArrayPtr(wTableID);
	if (pITableView==NULL) 
	{
		return;
	}

	//��ȡ�û�
	IUserItem * pIUserItem=pITableView->GetUserInfo(wChairID);
	if (pIUserItem==NULL) 
	{
		return;
	}

	//��ʾ�˵�
	CPoint Point;
	GetCursorPos(&Point);
	ShowUserInfoMenu(pIUserItem,Point);

	return;
}

//���˫��
void __cdecl CRoomViewItem::OnDoubleHitTable(WORD wTableID, bool bMirror)
{
}

//�û�����
void __cdecl CRoomViewItem::OnUserItemAcitve(IUserItem * pIUserItem)
{
	m_UserListView.InsertUserItem(pIUserItem);
	return ;
}

//�û�����
void __cdecl CRoomViewItem::OnUserItemUpdate(IUserItem * pIUserItem)
{
	m_UserListView.UpdateUserItem(pIUserItem);

	if (m_pFindUserDlg!=NULL) 
	{
		m_pFindUserDlg->UpdateUserItem(pIUserItem);
	}
	return ;
}

//�û�ɾ��
void __cdecl CRoomViewItem::OnUserItemDelete(IUserItem * pIUserItem)
{
	//��ȡ�û�
	ASSERT(pIUserItem!=NULL);
	DWORD dwUserID=pIUserItem->GetUserID();

	//�������
	if (m_dwChatUserID==dwUserID)
	{
		SetChatObject(NULL);
		TCHAR szMessage[256]=TEXT("");
		_snprintf(szMessage,sizeof(szMessage),TEXT("[ %s ] �뿪�ˣ������������Ϊ������"),pIUserItem->GetUserName());
		m_MessageProxyHelper->InsertSystemString(szMessage,0);
	}

	//�������
	int nChatObjectCount=m_ChatObject.GetCount();
	for (int i=1;i<nChatObjectCount;i++)
	{
		if (m_ChatObject.GetItemData(i)==dwUserID)
		{
			if (m_ChatObject.GetCurSel()==i) 
			{
				m_dwChatUserID=0;
				m_szChatName[0]=0;
				m_ChatObject.SetCurSel(0);
			}
			m_ChatObject.DeleteString(i);
			break;
		}
	}

	//������ͼ
	m_UserListView.DeleteUserItem(pIUserItem);
	if (m_pFindUserDlg!=NULL) m_pFindUserDlg->DeleteUserItem(pIUserItem);

	return;
}

//������Ϣ����
CShortMessage * CRoomViewItem::ActiveShortWnd(long int dwUserID, IUserItem * pUserItem, bool bCreate)
{
	//��������
	INT_PTR iCloseIndex=-1,iHideIndex=-1;
	
	//Ѱ�����ϴ���
	for (INT_PTR i=0;i<m_ShortMessage.GetCount();i++)
	{
		CShortMessage * pShortMessageWnd=(CShortMessage *)m_ShortMessage.GetAt(i);
		ASSERT(pShortMessageWnd!=NULL);
		if (dwUserID==pShortMessageWnd->m_dwTargetUserID)
		{
			if (pShortMessageWnd->GetSafeHwnd()==NULL)
			{
				AfxSetResourceHandle(GetModuleHandle(NULL));
				pShortMessageWnd->Create(IDD_SHORT_MESSAGE,GetDesktopWindow());
			}
			pShortMessageWnd->ShowWindow(SW_SHOW);
			return pShortMessageWnd;
		}
		if (bCreate==true)
		{
			if ((iCloseIndex==-1)
				&&  (pShortMessageWnd->GetSafeHwnd()==NULL)) 
			{
				iCloseIndex=i;
			}
			if ((iHideIndex==-1)
				&& (pShortMessageWnd->GetSafeHwnd()!=NULL)
				&& (pShortMessageWnd->IsWindowVisible()==FALSE)) 
			{
				iHideIndex=i;
			}
		}
	}

	//ʹ�ùرյĴ���
	if ((bCreate==true)&&(iCloseIndex>=0))
	{
		CShortMessage *pShortMessageWnd = (CShortMessage *)m_ShortMessage.GetAt(iCloseIndex);
		ASSERT(pShortMessageWnd!=NULL);
		if (pShortMessageWnd->GetSafeHwnd()==NULL)
		{
			AfxSetResourceHandle(GetModuleHandle(NULL));
			pShortMessageWnd->Create(IDD_SHORT_MESSAGE,GetDesktopWindow());
		}
		pShortMessageWnd->SetTalkInfo(pUserItem,m_pListServer->GetItemInfo()->szServerName);
		pShortMessageWnd->ShowWindow(SW_SHOW);
		//�ö�����
		pShortMessageWnd->SetActiveWindow();
		pShortMessageWnd->BringWindowToTop();
		return pShortMessageWnd;
	}

	//ʹ�����ش���
	if ((bCreate==true)&&(iHideIndex>=0))
	{
		CShortMessage * pShortMessageWnd=(CShortMessage *)m_ShortMessage.GetAt(iHideIndex);
		ASSERT(pShortMessageWnd!=NULL);
		pShortMessageWnd->SetTalkInfo(pUserItem,m_pListServer->GetItemInfo()->szServerName);
		pShortMessageWnd->ShowWindow(SW_SHOW);
		//�ö�����
		pShortMessageWnd->SetActiveWindow();
		pShortMessageWnd->BringWindowToTop();
		return pShortMessageWnd;
	}

	//�����´���
	if ((bCreate==true)&&(m_ShortMessage.GetCount()<10))
	{
		try
		{
			CShortMessage * pShortMessageWnd=new CShortMessage(this);
			AfxSetResourceHandle(GetModuleHandle(NULL));
			pShortMessageWnd->SetTalkInfo(pUserItem,m_pListServer->GetItemInfo()->szServerName);
			pShortMessageWnd->Create(IDD_SHORT_MESSAGE,GetDesktopWindow());
			pShortMessageWnd->ShowWindow(SW_SHOW);
			//�ö�����
			pShortMessageWnd->SetActiveWindow();
			pShortMessageWnd->BringWindowToTop();

			m_ShortMessage.Add(pShortMessageWnd);
			
			return pShortMessageWnd;
		}
		catch (...) 
		{ 
			return NULL; 
		}
	}

	return NULL;
}

//�������
bool CRoomViewItem::SetChatObject(IUserItem * pIUserItem)
{
	if (pIUserItem!=NULL)
	{
		//���ñ���
		tagUserData * pUserData=pIUserItem->GetUserData();
		m_dwChatUserID=pUserData->dwUserID;
		lstrcpyn(m_szChatName,pUserData->szName,CountArray(m_szChatName));

		//Ѱ�����
		int nItemCount=m_ChatObject.GetCount();
		for (int i=0;i<nItemCount;i++)
		{
			if (m_ChatObject.GetItemData(i)==m_dwChatUserID)
			{
				m_ChatObject.SetCurSel(i);
				return true;
			}
		}
		
		//�������
		int iItem=m_ChatObject.AddString(m_szChatName);
		m_ChatObject.SetItemData(iItem,m_dwChatUserID);
		m_ChatObject.SetCurSel(iItem);
	}
	else
	{
		//���ñ���
		m_dwChatUserID=0L;
		m_szChatName[0]=0;

		//���ý���
		m_ChatObject.SetCurSel(0);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
