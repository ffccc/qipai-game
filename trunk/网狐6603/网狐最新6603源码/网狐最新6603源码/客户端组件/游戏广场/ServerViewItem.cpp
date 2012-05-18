#include "StdAfx.h"
#include "GamePlaza.h"
#include "ServerViewItem.h"

//���ݿؼ�
#include "GlobalUnits.h"
#include "PlatformEvent.h"

//����ؼ�
#include "PlatformFrame.h"
#include "WndViewItemCtrl.h"

//�Ի�����
#include "DlgSearchUser.h"
#include "DlgServerManager.h"
#include "DlgTablePassword.h"

//////////////////////////////////////////////////////////////////////////////////

//��������
#define PROPERTY_CY					89									//��Ϸ����
#define PUBLICIZE_CY				80									//��Ϸ���
#define INCISE_SCALE				47/100								//����ָ�

//��������
#define SPLITTER_CY					3									//��ָ߶�
#define SPLITTER_CX					8									//��ֿ��
#define TABLE_BUTTON_SX				5									//��ť��϶
#define	TABLE_BUTTON_CX				330									//��ť����

//�ؼ���ʶ
#define IDC_CHAT_CONTROL			100									//�������
#define IDC_SERVER_TABLE			101									//���ӿؼ�
#define IDC_GAME_PROPERTY			102									//��Ϸ����
#define IDC_WEB_PUBLICIZE			103									//����ؼ�

//////////////////////////////////////////////////////////////////////////////////

//��ɫѡ��
#define MAX_CHAT_COLOR				16									//�����Ŀ
#define IDM_SELECT_CHAT_COLOR		(WM_USER+200)						//ѡ����ɫ

//��ݶ���
#define MAX_SHORT_COUNT				16									//�����Ŀ
#define IDM_SELECT_CHAT_SHORT		(WM_USER+300)						//ѡ�����

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CServerViewItem, CDialog)

	//ϵͳ��Ϣ
	ON_WM_SIZE()
	ON_WM_NCDESTROY()
	ON_WM_ERASEBKGND()

	//�ؼ���Ϣ
	ON_NOTIFY(NM_RCLICK, IDC_USER_LIST_CONTROL, OnNMRclickUserList)
	ON_NOTIFY(NM_DBLCLK, IDC_USER_LIST_CONTROL, OnNMDblclkUserList)

	//��������
	ON_COMMAND(IDM_MORE_COLOR, OnSelectMoreColor)
	ON_COMMAND_RANGE(IDM_SELECT_CHAT_COLOR, (IDM_SELECT_CHAT_COLOR+MAX_CHAT_COLOR), OnSelectChatColor)
	ON_COMMAND_RANGE(IDM_SELECT_CHAT_SHORT, (IDM_SELECT_CHAT_SHORT+MAX_SHORT_COUNT), OnSelectChatShort)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CServerViewItem::CServerViewItem()
{
	//�������
	m_bCreateFlag=false;
	m_bRectifyHor=false;
	m_bRectifyVor=false;
	m_bHideUserList=false;

	//��������
	m_dwChatTime=0L;
	m_wReqTableID=INVALID_TABLE;
	m_wReqChairID=INVALID_CHAIR;
	m_wFindTableID=INVALID_TABLE;

	//�û�����
	m_dwUserRight=0L;
	m_dwMasterRight=0L;
	m_pIMySelfUserItem=NULL;

	//��������
	m_wServerType=0L;
	m_dwServerRule=0L;
	m_ServiceStatus=ServiceStatus_Unknow;

	//���ò���
	m_pParameterGame=NULL;
	m_pParameterServer=NULL;

	//�ؼ�����
	m_pDlgSearchTable=NULL;
	m_pDlgInsureServer=NULL;
	m_pDlgServerManager=NULL;
	m_pExpressionControl=NULL;

	//�������
	m_DlgStatus.SetStatusViewSink(this);

	//������Ϣ
	m_cbColumnCount=0;
	m_cbPropertyCount=0;
	ZeroMemory(m_ColumnItem,sizeof(m_ColumnItem));
	ZeroMemory(m_PropertyInfo,sizeof(m_PropertyInfo));

	//��������
	ZeroMemory(&m_GameKind,sizeof(m_GameKind));
	ZeroMemory(&m_GameServer,sizeof(m_GameServer));

	//������Դ
	tagEncircleResource	EncircleChat;
	EncircleChat.pszImageTL=MAKEINTRESOURCE(IDB_CHAT_TL);
	EncircleChat.pszImageTM=MAKEINTRESOURCE(IDB_CHAT_TM);
	EncircleChat.pszImageTR=MAKEINTRESOURCE(IDB_CHAT_TR);
	EncircleChat.pszImageML=MAKEINTRESOURCE(IDB_CHAT_ML);
	EncircleChat.pszImageMR=MAKEINTRESOURCE(IDB_CHAT_MR);
	EncircleChat.pszImageBL=MAKEINTRESOURCE(IDB_CHAT_BL);
	EncircleChat.pszImageBM=MAKEINTRESOURCE(IDB_CHAT_BM);
	EncircleChat.pszImageBR=MAKEINTRESOURCE(IDB_CHAT_BR);
	m_ChatEncircle.InitEncircleResource(EncircleChat,AfxGetInstanceHandle());

	//������Դ
	tagEncircleResource	EncircleTable;
	EncircleTable.pszImageTL=MAKEINTRESOURCE(IDB_TABLE_TL);
	EncircleTable.pszImageTM=MAKEINTRESOURCE(IDB_TABLE_TM);
	EncircleTable.pszImageTR=MAKEINTRESOURCE(IDB_TABLE_TR);
	EncircleTable.pszImageML=MAKEINTRESOURCE(IDB_TABLE_ML);
	EncircleTable.pszImageMR=MAKEINTRESOURCE(IDB_TABLE_MR);
	EncircleTable.pszImageBL=MAKEINTRESOURCE(IDB_TABLE_BL);
	EncircleTable.pszImageBM=MAKEINTRESOURCE(IDB_TABLE_BM);
	EncircleTable.pszImageBR=MAKEINTRESOURCE(IDB_TABLE_BR);
	m_TableEncircle.InitEncircleResource(EncircleTable,AfxGetInstanceHandle());

	//�û���Դ
	tagEncircleResource	EncircleUserList;
	EncircleUserList.pszImageTL=MAKEINTRESOURCE(IDB_USER_LIST_TL);
	EncircleUserList.pszImageTM=MAKEINTRESOURCE(IDB_USER_LIST_TM);
	EncircleUserList.pszImageTR=MAKEINTRESOURCE(IDB_USER_LIST_TR);
	EncircleUserList.pszImageML=MAKEINTRESOURCE(IDB_USER_LIST_ML);
	EncircleUserList.pszImageMR=MAKEINTRESOURCE(IDB_USER_LIST_MR);
	EncircleUserList.pszImageBL=MAKEINTRESOURCE(IDB_USER_LIST_BL);
	EncircleUserList.pszImageBM=MAKEINTRESOURCE(IDB_USER_LIST_BM);
	EncircleUserList.pszImageBR=MAKEINTRESOURCE(IDB_USER_LIST_BR);
	m_UserListEncircle.InitEncircleResource(EncircleUserList,AfxGetInstanceHandle());

	return;
}

//��������
CServerViewItem::~CServerViewItem()
{
	//ɾ������
	SafeDelete(m_pDlgSearchTable);
	SafeDelete(m_pExpressionControl);

	//ɾ��˽��
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		//��ȡ����
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];

		//ɾ��˽��
		CPlatformFrame::GetInstance()->RemoveWhisperItem(pDlgWhisper);

		//�رմ���
		pDlgWhisper->DestroyWindow();

		//�����ڴ�
		SafeDelete(pDlgWhisper);
	}

	return;
}

//�ӿڲ�ѯ
VOID * CServerViewItem::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITCPSocketSink,Guid,dwQueryVer);
	QUERYINTERFACE(IServerViewItem,Guid,dwQueryVer);
	QUERYINTERFACE(IUserManagerSink,Guid,dwQueryVer);
	QUERYINTERFACE(IGamePropertySink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableViewFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(IProcessManagerSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IServerViewItem,Guid,dwQueryVer);
	return NULL;
}

//�ؼ���
VOID CServerViewItem::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	//����ؼ�
	DDX_Control(pDX, IDC_CHAT_EDIT, m_ChatEdit);
	DDX_Control(pDX, IDC_CHAT_MESSAGE, m_ChatMessage);

	//���찴ť
	DDX_Control(pDX, IDC_BT_SEND_CHAT, m_btSendChat);
	DDX_Control(pDX, IDC_BT_CHAT_SHORT, m_btChatShort);
	DDX_Control(pDX, IDC_BT_CHAT_BUTTON_1, m_btChatButton1);
	DDX_Control(pDX, IDC_BT_CHAT_BUTTON_2, m_btChatButton2);
	DDX_Control(pDX, IDC_BT_CHAT_BUTTON_3, m_btChatButton3);
	DDX_Control(pDX, IDC_BT_CHAT_BUTTON_4, m_btChatButton4);

	//���Ӱ�ť
	DDX_Control(pDX, IDC_BT_QUIT_SERVER, m_btQuitServer);
	DDX_Control(pDX, IDC_BT_TABLE_BUTTON_1, m_btTableButton1);
	DDX_Control(pDX, IDC_BT_TABLE_BUTTON_2, m_btTableButton2);
	DDX_Control(pDX, IDC_BT_TABLE_BUTTON_3, m_btTableButton3);
	DDX_Control(pDX, IDC_BT_TABLE_BUTTON_4, m_btTableButton4);
	DDX_Control(pDX, IDC_BT_TABLE_BUTTON_5, m_btTableButton5);

	//����ؼ�
	DDX_Control(pDX, IDC_VIEW_CONTROL, m_btControl);
	DDX_Control(pDX, IDC_VOR_SPLITER, m_VorSplitter);
	DDX_Control(pDX, IDC_HOR_SPLITER, m_HorSplitter);
	DDX_Control(pDX, IDC_USER_LIST_CONTROL, m_UserListControl);

	return;
}

//��������
BOOL CServerViewItem::OnInitDialog()
{
	__super::OnInitDialog();

	//��������
	CRect rcCreate(0,0,0,0);
	HINSTANCE hInstance=AfxGetInstanceHandle();

	//�����ؼ�
	m_TableViewFrame.CreateTableFrame(this,IDC_SERVER_TABLE,QUERY_ME_INTERFACE(IUnknownEx));
	m_GamePropertyCtrl.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcCreate,this,IDC_GAME_PROPERTY);
	m_PlatformPublicize.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcCreate,this,IDC_WEB_PUBLICIZE);

	//��ֿؼ�
	m_VorSplitter.SetSplitterEvent(this);
	m_HorSplitter.SetSplitterEvent(this);
	m_VorSplitter.SetSplitterColor(0.67,0.95);
	m_HorSplitter.SetSplitterColor(0.67,0.95);
	m_VorSplitter.SetSplitterType(SplitterType_Vor);
	m_HorSplitter.SetSplitterType(SplitterType_Hor);

	//�����б�
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	m_UserListControl.SetMySelfUserID(pGlobalUserInfo->GetGlobalUserData()->dwUserID);

	//����ؼ�
	m_ChatEdit.SetLimitText(LEN_USER_CHAT-1);
	m_ChatEdit.SetEnableColor(RGB(0,0,0),RGB(255,255,255),RGB(255,255,255));

	//����ؼ�
	m_ChatControl.SetChatControlSink(this);
	m_ChatMessage.SetExpressionManager(CExpressionManager::GetInstance(),RGB(255,255,255));

	//���Ӱ�ť
	m_btQuitServer.SetButtonImage(IDB_BT_QUIT_SERVER,TEXT("BT_QUIT_SERVER"),hInstance,true,false);
	m_btTableButton1.SetButtonImage(IDB_BT_TABLE_BUTTON_1,TEXT("BT_TABLE_BUTTON_1"),hInstance,true,false);
	m_btTableButton2.SetButtonImage(IDB_BT_TABLE_BUTTON_2,TEXT("BT_TABLE_BUTTON_2"),hInstance,true,false);
	m_btTableButton3.SetButtonImage(IDB_BT_TABLE_BUTTON_3,TEXT("BT_TABLE_BUTTON_3"),hInstance,true,false);
	m_btTableButton4.SetButtonImage(IDB_BT_TABLE_BUTTON_4,TEXT("BT_TABLE_BUTTON_4"),hInstance,true,false);
	m_btTableButton5.SetButtonImage(IDB_BT_TABLE_BUTTON_5,TEXT("BT_TABLE_BUTTON_5"),hInstance,true,false);

	//���찴ť
	m_btChatButton1.SetButtonImage(IDB_BT_CHAT_BUTTON_1,TEXT("BT_CHAT_BUTTON_1"),hInstance,true,false);
	m_btChatButton2.SetButtonImage(IDB_BT_CHAT_BUTTON_2,TEXT("BT_CHAT_BUTTON_2"),hInstance,true,false);
	m_btChatButton3.SetButtonImage(IDB_BT_CHAT_BUTTON_3,TEXT("BT_CHAT_BUTTON_3"),hInstance,true,false);
	m_btChatButton4.SetButtonImage(IDB_BT_CHAT_BUTTON_4,TEXT("BT_CHAT_BUTTON_4"),hInstance,true,false);

	//���찴ť
	m_btSendChat.SetButtonImage(IDB_BT_SEND_CHAT,hInstance,true,false);
	m_btChatShort.SetButtonImage(IDB_BT_CHAT_SHORT,hInstance,true,false);

	//���ư�ť
	m_btControl.SetParent(&m_VorSplitter);
	m_btControl.SetButtonImage(IDB_BK_USER_LIST,TEXT("BT_USER_LIST_HIDE"),hInstance,true,false);

	//��ʾ�ؼ�
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);

	//�������
	if (m_GameLevelParserModule.CreateInstance()==false) throw TEXT("�ȼ������������ʧ�ܣ�");
	if (m_PlazaUserManagerModule.CreateInstance()==false) throw TEXT("�û������������ʧ�ܣ�");

	//�������
	m_ProcessManager.SetProcessManagerSink(QUERY_ME_INTERFACE(IUnknownEx));
	m_UserListControl.SetGameLevelParser(m_GameLevelParserModule.GetInterface());

	//�������
	IUnknownEx * pIUnknownEx=QUERY_ME_INTERFACE(IUnknownEx);
	CUserInformation * pUserInformation=CUserInformation::GetInstance();
	if (m_PlazaUserManagerModule->SetUserManagerSink(pIUnknownEx)==false) throw TEXT("�û������������ʧ�ܣ�");
	if (m_PlazaUserManagerModule->SetUserInformation(pUserInformation)==false) throw TEXT("�û������������ʧ�ܣ�");

	//���ñ���
	m_bCreateFlag=true;

	//���ý���
	InitServerViewCtrl(CSkinRenderManager::GetInstance());

	//ע���¼�
	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
	if (pPlatformEvent!=NULL) pPlatformEvent->RegisterEventWnd(m_hWnd);

	return TRUE;
}

//��Ϣ����
BOOL CServerViewItem::PreTranslateMessage(MSG * pMsg)
{
	//��ʾ��Ϣ
	if (m_ToolTipCtrl.m_hWnd!=NULL)
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}

	//������Ϣ
	if (pMsg->message==WM_KEYDOWN)
	{
		//ȡ����ť
		if (pMsg->wParam==VK_ESCAPE)
		{
			return TRUE;
		}

		//�س���ť
		if (pMsg->wParam==VK_RETURN)
		{
			PostMessage(WM_COMMAND,IDC_BT_SEND_CHAT,0);
			return TRUE;
		}
	}

	return __super::PreTranslateMessage(pMsg);
}

//�����
BOOL CServerViewItem::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	UINT nCommandID=LOWORD(wParam);

	//��ť����
	switch (nCommandID)
	{
	case IDC_VIEW_CONTROL:			//���ư�ť
		{
			//���ñ���
			m_bHideUserList=!m_bHideUserList;

			//���ð�ť
			HINSTANCE hInstance=AfxGetInstanceHandle();
			m_btControl.SetButtonImage(IDB_BK_USER_LIST,(m_bHideUserList==true)?TEXT("BT_USER_LIST_SHOW"):TEXT("BT_USER_LIST_HIDE"),hInstance,true,false);

			//��������
			CRect rcClient;
			GetClientRect(&rcClient);
			RectifyControl(rcClient.Width(),rcClient.Height());

			return TRUE;
		}
	case IDC_BT_QUIT_SERVER:		//�˳�����
		{
			//��������
			PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0L);

			return TRUE;
		}
	case IDC_BT_SEND_CHAT:			//��������
		{
			//��������
			if (m_ServiceStatus==ServiceStatus_ServiceIng)
			{
				//��ȡ��Ϣ
				CString strMessage;
				m_ChatEdit.GetWindowText(strMessage);

				//��ʽ��Ϣ
				strMessage.TrimLeft();
				strMessage.TrimRight();

				//������Ϣ
				if ((strMessage.IsEmpty()==FALSE)&&(EfficacyUserChat(strMessage,INVALID_WORD)==true))
				{
					CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
					SendUserChatPacket(m_ChatControl.GetChatTargetUserID(),strMessage,pParameterGlobal->m_crChatTX);
				}

				//���ý���
				m_ChatEdit.SetFocus();
				m_ChatEdit.SetWindowText(TEXT(""));
			}

			return TRUE;
		}
	case IDC_BT_CHAT_SHORT:			//�������
		{
			//����˵�
			CSkinMenu ChatShortMenu;
			ChatShortMenu.CreateMenu();

			//����Ŀ¼
			TCHAR szDirectory[MAX_PATH]=TEXT("");
			CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

			//����·��
			TCHAR szFileName[MAX_PATH]=TEXT("");
			_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\PhraseInfo.ini"),szDirectory);

			//��������
			BYTE cbIndex=1;
			bool bSuccess=false;
			TCHAR szItemName[16]=TEXT(""),szShortcut[LEN_USER_CHAT]=TEXT("");

			//��ȡ��Ϣ
			while (true)
			{
				//��ȡ��Ϣ
				_sntprintf(szItemName,CountArray(szItemName),TEXT("Phrase%d"),cbIndex);
				GetPrivateProfileString(TEXT("ServerChatShort"),szItemName,TEXT(""),szShortcut,CountArray(szShortcut),szFileName);

				//����ж�
				if (szShortcut[0]!=0)
				{
					//���ñ���
					cbIndex++;
					bSuccess=true;

					//����˵�
					ChatShortMenu.AppendMenu(IDM_SELECT_CHAT_SHORT+cbIndex-1,szShortcut);
				}

				//�����ж�
				if ((szShortcut[0]==0)||(cbIndex>=MAX_SHORT_COUNT))
				{
					break;
				}
			}

			//�����˵�
			if (bSuccess==true)
			{
				CRect rcButton;
				m_btChatShort.GetWindowRect(&rcButton);
				ChatShortMenu.TrackPopupMenu(rcButton.right,rcButton.top,this);
			}

			return TRUE;
		}
	case IDC_BT_CHAT_BUTTON_1:		//���鰴ť
		{
			//��������
			CRect rcButton;
			m_btChatButton1.GetWindowRect(&rcButton);

			//��������
			if (m_pExpressionControl==NULL)
			{
				m_pExpressionControl=new CExpressionControl;
			}

			//��ʾ����
			m_pExpressionControl->ShowExpression(this,rcButton.left,rcButton.top,this);

			return TRUE;
		}
	case IDC_BT_CHAT_BUTTON_2:		//ѡ����ɫ
		{
			//��������
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

			//��������
			INT nWidth=72,nHeight=16;
			COLORREF crColor[]={RGB(255,0,0),RGB(255,255,0),RGB(128,255,0),RGB(0,255,255),
				RGB(0,0,128),RGB(0,128,0),RGB(128,0,128),RGB(128,0,0),RGB(0,0,0)};

			//������ɫ
			CImage ImageColor[CountArray(crColor)];
			for (INT i=0;i<CountArray(ImageColor);i++)
			{
				//����λͼ
				ImageColor[i].Create(nWidth,nHeight,16);

				//��ȡ�豸
				CImageDC BufferDC(ImageColor[i]);
				CDC * pDC=CDC::FromHandle(BufferDC);

				//�滭λͼ
				pDC->FillSolidRect(2,2,nWidth-4,nHeight-4,crColor[i]);
				pDC->Draw3dRect(0,0,nWidth,nHeight,RGB(0,0,0),RGB(0,0,0));
				pDC->Draw3dRect(1,1,nWidth-2,nHeight-2,RGB(255,255,255),RGB(255,255,255));
			}

			//�����˵�
			CSkinMenu MenuColor;
			MenuColor.CreateMenu();

			//����˵�
			MenuColor.AppendMenu(IDM_MORE_COLOR,TEXT("ѡ��������ɫ"));

			//��ɫѡ��
			MenuColor.AppendSeparator();
			for (INT i=0;i<CountArray(ImageColor);i++) 
			{
				MenuColor.AppendMenu(IDM_SELECT_CHAT_COLOR+i,ImageColor[i]);
				if (pParameterGlobal->m_crChatTX==crColor[i]) MenuColor.CheckMenuItem(IDM_SELECT_CHAT_COLOR+i,MF_BYCOMMAND|MF_CHECKED);
			}

			//�����˵�
			CRect rcButton;
			m_btChatButton2.GetWindowRect(&rcButton);
			MenuColor.TrackPopupMenu(rcButton.left,rcButton.top,this);

			return TRUE;
		}
	case IDC_BT_CHAT_BUTTON_3:		//���ΰ�ť
		{
			//�����˵�
			CSkinMenu MenuMessage;
			MenuMessage.CreateMenu();

			//����˵�
			MenuMessage.AppendMenu(IDM_MESSAGE_SHOW_ALL,TEXT("��ʾ������Ϣ"));
			MenuMessage.AppendMenu(IDM_MESSAGE_HIDE_DETEST,TEXT("���������Ϣ"));
			MenuMessage.AppendMenu(IDM_MESSAGE_ONLY_FRIEND,TEXT("ֻ��ʾ������Ϣ"));

			//��������
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

			//���ò˵�
			BYTE cbMessageMode=pParameterGlobal->m_cbMessageMode;
			if (cbMessageMode==MESSAGE_MODE_ALL) MenuMessage.CheckMenuItem(IDM_MESSAGE_SHOW_ALL,MF_BYCOMMAND|MF_CHECKED);
			if (cbMessageMode==MESSAGE_MODE_DETEST) MenuMessage.CheckMenuItem(IDM_MESSAGE_HIDE_DETEST,MF_BYCOMMAND|MF_CHECKED);
			if (cbMessageMode==MESSAGE_MODE_FRIEND) MenuMessage.CheckMenuItem(IDM_MESSAGE_ONLY_FRIEND,MF_BYCOMMAND|MF_CHECKED);

			//�����˵�
			CRect rcButton;
			m_btChatButton3.GetWindowRect(&rcButton);
			MenuMessage.TrackPopupMenu(rcButton.left,rcButton.top,this);

			return TRUE;
		}
	case IDC_BT_CHAT_BUTTON_4:		//������ť
		{
			//���ý���
			m_ChatEdit.SetFocus();

			//��������
			m_ChatMessage.CleanScreen();

			return TRUE;
		}
	case IDC_BT_TABLE_BUTTON_1:		//���밴ť
		{
			//��������
			if (m_ServiceStatus!=ServiceStatus_ServiceIng) return TRUE;
			if ((m_wReqTableID!=INVALID_TABLE)&&(m_wReqChairID!=INVALID_CHAIR)) return TRUE;

			//״̬�ж�
			if (m_pIMySelfUserItem->GetUserStatus()>=US_PLAYING)
			{
				CInformation Information(this);
				Information.ShowMessageBox(TEXT("��������Ϸ�У���ʱ�����뿪�����Ƚ�����ǰ��Ϸ��"),MB_ICONINFORMATION,30L);
				return TRUE;
			}

			//�����λ
			if (CServerRule::IsAvertCheatMode(m_dwServerRule)==true)
			{
				PerformSitDownAction(INVALID_TABLE,INVALID_CHAIR,false);
				return TRUE;
			}

			//��������
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

			//ִ�ж���
			switch (pParameterGlobal->m_cbActionHitAutoJoin)
			{
			case ACTION_SEARCH_TABLE:		//����λ��
				{
					//��������
					tagSearchTable SearchTable;
					ZeroMemory(&SearchTable,sizeof(SearchTable));

					//��������
					SearchTable.bNotFull=true;
					SearchTable.bOneNull=true;
					SearchTable.bAllNull=false;
					SearchTable.bFilterPass=true;
					SearchTable.bTwoNull=(m_TableViewFrame.GetChairCount()!=2);

					//�������
					SearchTable.wResultTableID=INVALID_TABLE;
					SearchTable.wResultChairID=INVALID_CHAIR;
					SearchTable.wStartTableID=m_wFindTableID+1;

					//��������
					SearchGameTable(SearchTable);
					m_wFindTableID=SearchTable.wResultTableID;

					//��������
					if (m_wFindTableID==INVALID_CHAIR)
					{
						//��������
						SearchTable.bAllNull=true;
						SearchTable.bNotFull=true;
						SearchTable.bOneNull=true;
						SearchTable.bFilterPass=true;
						SearchTable.bTwoNull=(m_TableViewFrame.GetChairCount()!=2);

						//�������
						SearchTable.wResultTableID=INVALID_TABLE;
						SearchTable.wResultChairID=INVALID_CHAIR;
						SearchTable.wStartTableID=m_wFindTableID+1;

						//��������
						SearchGameTable(SearchTable);
						m_wFindTableID=SearchTable.wResultTableID;
					}

					//����ж�
					if (m_wFindTableID!=INVALID_CHAIR)
					{
						//Ч������
						ASSERT(SearchTable.wResultTableID!=INVALID_TABLE);
						ASSERT(SearchTable.wResultChairID!=INVALID_CHAIR);

						//��������
						WORD wChairID=SearchTable.wResultChairID;
						m_TableViewFrame.VisibleTable(m_wFindTableID);
						m_TableViewFrame.FlashGameChair(m_wFindTableID,wChairID);

						//�Զ�����
						CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
						if (pParameterGlobal->m_bAutoSitDown==true) PerformSitDownAction(m_wFindTableID,wChairID,true);
					}
					else
					{
						//��ʾ��Ϣ
						CInformation Information(this);
						Information.ShowMessageBox(TEXT("��Ǹ��������ʱû�п��Լ������Ϸ�������Ժ��ٴγ��ԣ�"),MB_ICONINFORMATION,30L);
					}

					break;
				}
			case ACTION_SHOW_SEARCH_DLG:	//��������
				{
					//��������
					PostMessage(WM_COMMAND,IDC_BT_TABLE_BUTTON_2,0L);

					break;
				}
			}

			return TRUE;
		}
	case IDC_BT_TABLE_BUTTON_2:		//��������
		{
			//��������
			if (m_pDlgSearchTable==NULL)
			{
				//��������
				try
				{
					m_pDlgSearchTable=new CDlgSearchTable;
					if (m_pDlgSearchTable==NULL) return TRUE;
				}
				catch (...)
				{
					ASSERT(FALSE);
					return TRUE;
				}

				//���ö���
				m_pDlgSearchTable->SetServerViewItem(this);
				m_pDlgSearchTable->SetTableViewFrame(&m_TableViewFrame);
			}

			//��������
			if (m_pDlgSearchTable->m_hWnd==NULL)
			{
				m_pDlgSearchTable->Create(IDD_DLG_SEARCH_TABLE,this);
			}

			//��ʾ����
			m_pDlgSearchTable->ShowWindow(SW_SHOW);
			m_pDlgSearchTable->SetForegroundWindow();

			return TRUE;
		}
	case IDC_BT_TABLE_BUTTON_3:		//������ť
		{
			//��������
			CDlgTablePassword DlgTablePassword;
			DlgTablePassword.SetPromptString(TEXT("��������"));

			//��ʾ����
			if (DlgTablePassword.DoModal()==IDOK)
			{
				//��������
				m_pParameterServer->m_bTakePassword=true;
				lstrcpyn(m_pParameterServer->m_szPassword,DlgTablePassword.m_szPassword,CountArray(m_pParameterServer->m_szPassword));

				//���͹���
				SendUserRulePacket();
			}

			return TRUE;
		}
	case IDC_BT_TABLE_BUTTON_4:		//��Ϸ����
		{
			return TRUE;
		}
	case IDC_BT_TABLE_BUTTON_5:		//���ఴť
		{
			//����˵�
			CSkinMenu FunctionMenu;
			FunctionMenu.CreateMenu();

			//����˵�
			FunctionMenu.AppendMenu(IDM_GAME_RULE,TEXT("�������"));
			FunctionMenu.AppendMenu(IDM_GAME_CHART,TEXT("��Ϸ���а�"));

			//����˵�
			if (m_pIMySelfUserItem->GetMasterOrder()>0)
			{
				//����ָ�
				FunctionMenu.AppendSeparator();

				//����˵�
				FunctionMenu.AppendMenu(IDM_MANAGER_USER,TEXT("��Ϸ��ҹ���..."),MF_DISABLED);
				FunctionMenu.AppendMenu(IDM_MANAGER_SERVER,TEXT("��Ϸ�������..."),MF_DISABLED);
				FunctionMenu.AppendMenu(IDM_MANAGER_MESSAGE,TEXT("ϵͳ��Ϣ����..."),MF_DISABLED);

				//���ò˵�
				if (CMasterRight::CanManagerServer(m_dwMasterRight)==true)
				{
					FunctionMenu.EnableMenuItem(IDM_MANAGER_SERVER,MF_ENABLED);
				}

				//���ò˵�
				if (CMasterRight::CanIssueMessage(m_dwMasterRight)==true)
				{
					FunctionMenu.EnableMenuItem(IDM_MANAGER_MESSAGE,MF_ENABLED);
				}
			}

			//�����˵�
			CRect rcButton;
			m_btTableButton5.GetWindowRect(&rcButton);
			FunctionMenu.TrackPopupMenu(rcButton.left,rcButton.bottom,this);

			return TRUE;
		}
	}

	//�˵�����
	switch (nCommandID)
	{
	case IDM_GAME_RULE:				//��Ϸ����
		{
			//�����ַ
			TCHAR szBrowser[MAX_PATH]=TEXT("");
			_sntprintf(szBrowser,CountArray(szBrowser),TEXT("%s/GameRule.asp?GameID=%ld&KindID=%ld"),szPlatformLink,m_GameKind.wGameID,
				m_GameServer.wKindID);

			//��ҳ��
			ShellExecute(NULL,TEXT("OPEN"),szBrowser,NULL,NULL,SW_NORMAL);

			return TRUE;
		}
	case IDM_GAME_CHART:			//��Ϸ����
		{
			//��������
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			//�����ַ
			TCHAR szBrowser[256]=TEXT("");
			_sntprintf(szBrowser,CountArray(szBrowser),TEXT("%s/GameChart.asp?GameID=%ld&KindID=%ld&ServerID=%ld&UserID=%ld"),szPlatformLink,
				m_GameKind.wGameID,m_GameServer.wKindID,m_GameServer.wServerID,pGlobalUserData->dwUserID);

			//��ҳ��
			ShellExecute(NULL,TEXT("OPEN"),szBrowser,NULL,NULL,SW_NORMAL);

			return TRUE;
		}
	case IDM_DELETE_SERVER_ITEM:	//�رշ���
		{
			//�ж�����
			if (m_TCPSocketModule.GetInterface()!=NULL)
			{
				m_TCPSocketModule->CloseSocket();
				m_ServiceStatus=ServiceStatus_NetworkDown;
			}

			//�رշ���
			CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
			if (pPlatformFrame!=NULL) pPlatformFrame->DeleteServerViewItem(this);

			return TRUE;
		}
	case IDM_CREATE_ISER_WISPER:	//����˽��
		{
			//����˽��
			if (m_ServiceStatus==ServiceStatus_ServiceIng)
			{
				WORD wUserCount=(WORD)m_MenuUserItemArray.GetCount();
				WhisperConversation(m_MenuUserItemArray.GetData(),wUserCount);
			}

			return TRUE;
		}
	case IDM_COPY_USER_NICKNAME:	//�����ǳ�
		{
			//Ч��״̬
			ASSERT(m_MenuUserItemArray.GetCount()>0);
			if (m_MenuUserItemArray.GetCount()==0) return TRUE;

			//�����ַ�
			IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];
			CWHService::SetClipboardString(pIClientUserItem->GetNickName());

			//�����ַ�
			m_ChatEdit.Paste();
			m_ChatEdit.SetFocus();

			return TRUE;
		}
	case IDM_SEE_USER_LOCATION:		//�鿴λ��
		{
			//Ч��״̬
			ASSERT(m_MenuUserItemArray.GetCount()>0);
			if (m_MenuUserItemArray.GetCount()==0) return TRUE;

			//������Ϣ
			bool bHideUserInfo=CServerRule::IsAvertCheatMode(m_dwServerRule);
			if ((bHideUserInfo==true)&&(m_pIMySelfUserItem->GetMasterOrder()==0)) return FALSE;

			//��ȡ�û�
			IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];

			//��ȡ����
			WORD wTableID=pIClientUserItem->GetTableID();
			WORD wChairID=pIClientUserItem->GetChairID();
			BYTE cbUserStatus=pIClientUserItem->GetUserStatus();

			//��λλ��
			if (wTableID!=INVALID_TABLE)
			{
				//���ÿ���
				ASSERT(CWndViewItemCtrl::GetInstance()!=NULL);
				CWndViewItemCtrl::GetInstance()->ActiveViewItem(this);

				//���ÿ���
				m_TableViewFrame.VisibleTable(wTableID);

				//��������
				if (cbUserStatus==US_LOOKON)
				{
					m_TableViewFrame.FlashGameTable(wTableID);
				}
				else
				{
					m_TableViewFrame.FlashGameChair(wTableID,wChairID);
				}
			}

			//��������
			LVFINDINFO LVFindInfo;
			ZeroMemory(&LVFindInfo,sizeof(LVFindInfo));

			//���ñ���
			LVFindInfo.flags=LVFI_PARAM;
			LVFindInfo.lParam=(LPARAM)pIClientUserItem;

			//��λ�б�
			INT nItem=m_UserListControl.FindItem(&LVFindInfo);

			//�����б�
			if (nItem!=-1L)
			{
				m_UserListControl.EnsureVisible(nItem,FALSE);
				m_UserListControl.SetItemState(nItem,LVIS_FOCUSED|LVIS_SELECTED,LVIS_FOCUSED|LVIS_SELECTED);
			}

			return TRUE;
		}
	case IDM_SET_CHAT_USER:			//��̸����
		{
			//Ч��״̬
			ASSERT(m_MenuUserItemArray.GetCount()>0);
			if (m_MenuUserItemArray.GetCount()==0) return TRUE;

			//��������
			IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];
			if (pIClientUserItem!=NULL) m_ChatControl.SetChatTargetUser(pIClientUserItem);

			return TRUE;
		}
	case IDM_CANCEL_CHAT_USER:		//ȡ����̸
		{
			//��������
			m_ChatControl.SetChatTargetUser(NULL);

			return TRUE;
		}
	case IDM_SET_FRIEND_USER:		//��Ϊ����
		{
			//Ч��״̬
			ASSERT(m_MenuUserItemArray.GetCount()>0);
			if (m_MenuUserItemArray.GetCount()==0) return TRUE;

			//��������
			IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];
			BYTE cbSourCompanion=pIClientUserItem->GetUserCompanion();
			BYTE cbDestCompanion=(cbSourCompanion==CP_FRIEND)?CP_NORMAL:CP_FRIEND;

			//��������
			ASSERT(CUserInformation::GetInstance()!=NULL);
			CUserInformation * pUserInformation=CUserInformation::GetInstance();

			//���ù�ϵ
			pIClientUserItem->SetUserCompanion(cbDestCompanion);
			pUserInformation->InsertCompanionInfo(pIClientUserItem,cbDestCompanion);

			//�����б�
			m_UserListControl.UpdateDataItem(pIClientUserItem);

			return TRUE;
		}
	case IDM_SET_DETEST_USER:		//��Ϊ���
		{
			//Ч��״̬
			ASSERT(m_MenuUserItemArray.GetCount()>0);
			if (m_MenuUserItemArray.GetCount()==0) return TRUE;

			//��������
			IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];
			BYTE cbSourCompanion=pIClientUserItem->GetUserCompanion();
			BYTE cbDestCompanion=(cbSourCompanion==CP_DETEST)?CP_NORMAL:CP_DETEST;

			//��������
			ASSERT(CUserInformation::GetInstance()!=NULL);
			CUserInformation * pUserInformation=CUserInformation::GetInstance();

			//���ù�ϵ
			pIClientUserItem->SetUserCompanion(cbDestCompanion);
			pUserInformation->InsertCompanionInfo(pIClientUserItem,cbDestCompanion);

			//�����б�
			m_UserListControl.UpdateDataItem(pIClientUserItem);

			return TRUE;
		}
	case IDM_MESSAGE_SHOW_ALL:		//��ʾ����
		{
			//��������
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal::GetInstance()->m_cbMessageMode=MESSAGE_MODE_ALL;

			return TRUE;
		}
	case IDM_MESSAGE_HIDE_DETEST:	//�������
		{
			//��������
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal::GetInstance()->m_cbMessageMode=MESSAGE_MODE_DETEST;

			return TRUE;
		}
	case IDM_MESSAGE_ONLY_FRIEND:	//��ʾ����
		{
			//��������
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal::GetInstance()->m_cbMessageMode=MESSAGE_MODE_FRIEND;

			return TRUE;
		}
	case IDM_MANAGER_SERVER:		//�������
		{
			//��������
			CDlgServerManager DlgServerManager;
			DlgServerManager.Initialization(m_TCPSocketModule.GetInterface(),m_pIMySelfUserItem);

			//���ñ���
			m_pDlgServerManager=&DlgServerManager;

			//��ʾ����
			DlgServerManager.DoModal();

			//���ñ���
			m_pDlgServerManager=NULL;

			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//���˫��
VOID CServerViewItem::OnDButtonHitTable(WORD wTableID)
{
	return;
}

//�������
VOID CServerViewItem::OnLButtonHitTable(WORD wTableID, WORD wChairID)
{
	//��������
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return;
	if ((wTableID==INVALID_TABLE)||(wChairID==INVALID_CHAIR)) return;
	if ((wTableID==m_wReqTableID)&&(wChairID==m_wReqChairID)) return;

	//��ȡ�û�
	IClientUserItem * pTableUserItem=NULL;
	if (wChairID!=INDEX_ENTER_CHAIR) pTableUserItem=m_TableViewFrame.GetClientUserItem(wTableID,wChairID);

	//�����λ
	if (CServerRule::IsAvertCheatMode(m_dwServerRule)==true)
	{
		PerformSitDownAction(INVALID_TABLE,INVALID_CHAIR,false);
		return;
	}

	//��Ϣ����
	if (pTableUserItem!=NULL)
	{
		if (pTableUserItem==m_pIMySelfUserItem)
		{
			//״̬����
			if (m_pIMySelfUserItem->GetUserStatus()>=US_PLAYING) return;
			if ((m_wReqTableID!=INVALID_TABLE)||(m_wReqChairID!=INVALID_CHAIR)) return;

			//��������
			PerformStandUpAction(wTableID,wChairID);
		}
		else
		{
			//״̬����
			WORD wMeTableID=m_pIMySelfUserItem->GetTableID();
			WORD wMeChairID=m_pIMySelfUserItem->GetChairID();
			if ((wMeTableID==wTableID)&&(wMeChairID==wChairID)) return;
			if ((m_wReqTableID!=INVALID_TABLE)||(m_wReqChairID!=INVALID_CHAIR)) return;

			//�Թ۶���
			PerformLookonAction(wTableID,wChairID);
		}
	}
	else
	{
		//�������
		if (wChairID==INDEX_ENTER_CHAIR)
		{
			//��������
			WORD wUserTableID=m_pIMySelfUserItem->GetTableID();
			BYTE cbUserStatus=m_pIMySelfUserItem->GetUserStatus();

			//�������
			if ((wTableID==wUserTableID)&&(cbUserStatus!=US_LOOKON))
			{
				m_ProcessManager.SendProcessData(IPC_CMD_GF_CONTROL,IPC_SUB_GF_ACTIVE_PROCESS);
				return;
			}
		}

		//���¶���
		PerformSitDownAction(wTableID,wChairID,true);
	}

	return;
}

//�Ҽ�����
VOID CServerViewItem::OnRButtonHitTable(WORD wTableID, WORD wChairID)
{
	//���λ��
	if ((wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR)&&(wChairID!=INDEX_ENTER_CHAIR))
	{
		//��ȡ�û�
		IClientUserItem * pIClientUserItem=m_TableViewFrame.GetClientUserItem(wTableID,wChairID);

		//�û��˵�
		if (pIClientUserItem!=NULL)
		{
			TrackUserItemMenu(pIClientUserItem);
		}

		return;
	}

	return;
}

//�����¼�
bool CServerViewItem::OnEventTCPSocketLink(WORD wSocketID, INT nErrorCode)
{
	//������ʾ
	if (nErrorCode!=0L)
	{
		//�ر���ʾ
		m_DlgStatus.HideStatusWindow();

		//�رշ���
		PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("�ܱ�Ǹ����Ϸ��������ʧ�ܣ����Ժ����Ի�������վ���棡"),MB_ICONSTOP);

		return true;
	}

	//���͵�¼
	SendLogonPacket();

	//����״̬
	SetServiceStatus(ServiceStatus_Validate);

	return true;
}

//�ر��¼�
bool CServerViewItem::OnEventTCPSocketShut(WORD wSocketID, BYTE cbShutReason)
{
	//�ر�״̬
	m_DlgStatus.HideStatusWindow();

	//�ر���Ϸ
	m_ProcessManager.CloseGameProcess(false);

	//�رմ���
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		if (m_ServiceStatus==ServiceStatus_ServiceIng)
		{
			//��ʾ��Ϣ
			m_ChatMessage.InsertSystemString(TEXT("����Ϸ�������������ж���"));

			//�ر���ʾ
			CInformation Information(this);
			INT_PTR nResult=Information.ShowMessageBox(m_GameServer.szServerName,TEXT("����Ϸ�������������Ѿ��жϣ��Ƿ�رյ�ǰ��Ϸ���䣿"),
				MB_ICONERROR|MB_YESNO|MB_DEFBUTTON2,0L);

			//�رշ���
			if (nResult==IDYES)
			{
				PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM);
			}
		}
		else
		{
			//�ر���ʾ
			CInformation Information(this);
			Information.ShowMessageBox(m_GameServer.szServerName,TEXT("���������ԭ�򣬷�������ʧ�ܣ����Ժ����³��ԣ�"),MB_ICONERROR);

			//�رշ���
			PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);
		}
	}

	//����״̬
	m_ServiceStatus=ServiceStatus_NetworkDown;

	return true;
}

//��ȡ�¼�
bool CServerViewItem::OnEventTCPSocketRead(WORD wSocketID, TCP_Command Command, VOID * pData, WORD wDataSize)
{
	switch (Command.wMainCmdID)
	{
	case MDM_GR_LOGON:		//��¼��Ϣ
		{
			return OnSocketMainLogon(Command.wSubCmdID,pData,wDataSize);
		}
	case MDM_GR_CONFIG:		//������Ϣ
		{
			return OnSocketMainConfig(Command.wSubCmdID,pData,wDataSize);
		}
	case MDM_GR_USER:		//�û���Ϣ
		{
			return OnSocketMainUser(Command.wSubCmdID,pData,wDataSize);
		}
	case MDM_GR_STATUS:		//״̬��Ϣ
		{
			return OnSocketMainStatus(Command.wSubCmdID,pData,wDataSize);
		}
	case MDM_GR_INSURE:		//������Ϣ
		{
			return OnSocketMainInsure(Command.wSubCmdID,pData,wDataSize);
		}
	case MDM_GR_MANAGE:		//������Ϣ
		{
			return OnSocketMainManager(Command.wSubCmdID,pData,wDataSize);
		}
	case MDM_CM_SYSTEM:		//ϵͳ��Ϣ
		{
			return OnSocketMainSystem(Command.wSubCmdID,pData,wDataSize);
		}
	case MDM_GF_GAME:		//��Ϸ��Ϣ
	case MDM_GF_FRAME:		//�����Ϣ
		{
			return OnSocketMainGameFrame(Command.wMainCmdID,Command.wSubCmdID,pData,wDataSize);
		}
	}

	return true;
}

//�������
bool CServerViewItem::OnEventBuyProperty(LPCTSTR pszNickName, WORD wItemCount, WORD wPropertyIndex)
{
	//Ч�����
	ASSERT((pszNickName!=NULL)&&(wItemCount>0));
	if ((pszNickName==NULL)||(wItemCount==0)) return false;

	//��������
	ASSERT(CGamePropertyManager::GetInstance()!=NULL);
	CGamePropertyManager * pGamePropertyManager=CGamePropertyManager::GetInstance();

	//���ҵ���
	ASSERT(pGamePropertyManager->GetPropertyItem(wPropertyIndex)!=NULL);
	CGamePropertyItem * pGamePropertyItem=pGamePropertyManager->GetPropertyItem(wPropertyIndex);

	//�����û�
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByNickName(pszNickName);

	//�û��ж�
	if (pIClientUserItem==NULL)
	{
		return false;
	}

	//��������
	CMD_GR_C_BuyProperty BuyProperty;
	ZeroMemory(&BuyProperty,sizeof(BuyProperty));

	//���ñ���
	BuyProperty.wItemCount=wItemCount;
	BuyProperty.wPropertyIndex=wPropertyIndex;
	BuyProperty.dwTargetUserID=pIClientUserItem->GetUserID();

	//������Ϣ
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_BUY_PROPERTY,&BuyProperty,sizeof(BuyProperty));

	return true;
}

//���̴���
bool CServerViewItem::OnGameProcessError()
{
	return true;
}

//���̹ر�
bool CServerViewItem::OnGameProcessClose(DWORD dwExitCode)
{
	//Ч��״̬
	ASSERT(m_pIMySelfUserItem!=NULL);
	if (m_pIMySelfUserItem==NULL) return false;

	//��������
	WORD wTableID=m_pIMySelfUserItem->GetTableID();
	WORD wChairID=m_pIMySelfUserItem->GetChairID();
	BYTE cbUserStatus=m_pIMySelfUserItem->GetUserStatus();

	//�˳���Ϸ
	if ((wTableID!=INVALID_TABLE)&&(m_ServiceStatus==ServiceStatus_ServiceIng))
	{
		SendStandUpPacket(wTableID,wChairID,TRUE);
	}

	return true;
}

//��������
bool CServerViewItem::OnGameProcessCreate(bool bAlreadyExist)
{
	//��������
	WORD wTableID=m_pIMySelfUserItem->GetTableID();
	WORD wChairID=m_pIMySelfUserItem->GetChairID();

	//��������
	if (bAlreadyExist==false)
	{
		//��ȡ�ȼ�
		tagLevelItem LevelItem[64];
		BYTE cbLevelCount=(BYTE)m_GameLevelParserModule->GetGameLevelItem(LevelItem,CountArray(LevelItem));

		//��������
		m_ProcessManager.SendLevelInfo(LevelItem,cbLevelCount);
		m_ProcessManager.SendColumnInfo(m_ColumnItem,m_cbColumnCount);
		m_ProcessManager.SendPropertyInfo(m_PropertyInfo,m_cbPropertyCount);
	}

	//������Ϣ
	m_ProcessManager.SendServerInfo(m_pIMySelfUserItem,m_dwUserRight,m_dwMasterRight,m_dwServerRule,m_wServerType,m_GameServer);

	//�����û�
	if ((wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR))
	{
		//��������
		WORD wEnumIndex=0;
		IPlazaUserManager * pIPlazaUserManager=m_PlazaUserManagerModule.GetInterface();

		//�������
		while (true)
		{
			//��ȡ�û�
			IClientUserItem * pIClientUserItem=pIPlazaUserManager->EnumUserItem(wEnumIndex++);
			if (pIClientUserItem==NULL) break;

			//�����ж�
			if (pIClientUserItem->GetTableID()!=wTableID) continue;
			if (pIClientUserItem->GetUserStatus()==US_LOOKON) continue;

			//�����û�
			m_ProcessManager.SendUserItem(pIClientUserItem);
		};

		//�Թ��û�
		wEnumIndex=0;
		while (true)
		{
			//��ȡ�û�
			IClientUserItem * pIClientUserItem=pIPlazaUserManager->EnumUserItem(wEnumIndex++);
			if (pIClientUserItem==NULL) break;

			//�����ж�
			if (pIClientUserItem->GetTableID()!=wTableID) continue;
			if (pIClientUserItem->GetUserStatus()!=US_LOOKON) continue;

			//�����û�
			m_ProcessManager.SendUserItem(pIClientUserItem);
		};
	}

	//�������
	m_ProcessManager.SendProcessData(IPC_CMD_GF_CONFIG,IPC_SUB_GF_CONFIG_FINISH);

	return true;
}

//��������
bool CServerViewItem::OnProcessManagerData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//������Ϣ
	if ((wMainCmdID==IPC_CMD_GF_SOCKET)&&(wSubCmdID==IPC_SUB_GF_SOCKET_SEND))
	{
		//Ч������
		ASSERT(wDataSize>=sizeof(TCP_Command));
		if (wDataSize<sizeof(TCP_Command)) return false;

		//��������
		IPC_GF_SocketSend * pSocketPackage=(IPC_GF_SocketSend *)pData;
		WORD wPacketSize=wDataSize-(sizeof(IPC_GF_SocketSend)-sizeof(pSocketPackage->cbBuffer));

		//��������
		if (wPacketSize==0)
		{
			TCP_Command * pCommand=&pSocketPackage->CommandInfo;
			m_TCPSocketModule->SendData(pCommand->wMainCmdID,pCommand->wSubCmdID);
		}
		else 
		{
			TCP_Command * pCommand=&pSocketPackage->CommandInfo;
			m_TCPSocketModule->SendData(pCommand->wMainCmdID,pCommand->wSubCmdID,pSocketPackage->cbBuffer,wPacketSize);
		}

		return true;
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//�û�����
VOID CServerViewItem::OnUserItemAcitve(IClientUserItem * pIClientUserItem)
{
	//�ж��Լ�
	if (m_pIMySelfUserItem==NULL)
	{
		m_pIMySelfUserItem=pIClientUserItem;
	}

	//�����б�
	m_UserListControl.InsertDataItem(pIClientUserItem);

	//��������
	BYTE cbUserStatus=pIClientUserItem->GetUserStatus();
	if ((cbUserStatus>=US_SIT)&&(cbUserStatus!=US_LOOKON))
	{
		WORD wTableID=pIClientUserItem->GetTableID();
		WORD wChairID=pIClientUserItem->GetChairID();
		m_TableViewFrame.SetClientUserItem(wTableID,wChairID,pIClientUserItem);
	}

	//��ʾ��Ϣ
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
	if ((pParameterGlobal->m_bNotifyUserInOut==true)&&(m_ServiceStatus==ServiceStatus_ServiceIng))
	{
		m_ChatMessage.InsertUserEnter(pIClientUserItem->GetNickName());
	}

	return;
}

//�û�ɾ��
VOID CServerViewItem::OnUserItemDelete(IClientUserItem * pIClientUserItem)
{
	//��������
	WORD wLastTableID=pIClientUserItem->GetTableID();
	WORD wLastChairID=pIClientUserItem->GetChairID();
	DWORD dwLeaveUserID=pIClientUserItem->GetUserID();
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();

	//��������
	ASSERT(CParameterGlobal::GetInstance());
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//�뿪����
	if ((wLastTableID!=INVALID_TABLE)&&(wLastChairID!=INVALID_CHAIR))
	{
		//��ȡ�û�
		IClientUserItem * pITableUserItem=m_TableViewFrame.GetClientUserItem(wLastTableID,wLastChairID);
		if (pITableUserItem==pIClientUserItem) m_TableViewFrame.SetClientUserItem(wLastTableID,wLastChairID,NULL);

		//�뿪֪ͨ
		if ((pIClientUserItem==m_pIMySelfUserItem)||(wLastTableID==pMeUserInfo->wTableID))
		{
			tagUserStatus UserStatus;
			UserStatus.cbUserStatus=US_NULL;
			UserStatus.wTableID=INVALID_TABLE;
			UserStatus.wChairID=INVALID_CHAIR;
			m_ProcessManager.SendUserStatus(pIClientUserItem->GetUserID(),UserStatus);
		}
	}

	//�������
	m_ChatControl.DeleteUserItem(pIClientUserItem);

	//ɾ���б�
	m_UserListControl.DeleteDataItem(pIClientUserItem);

	//���Ҵ���
	CDlgSearchUser * pDlgSearchUser=CDlgSearchUser::GetInstance();
	if (pDlgSearchUser!=NULL) pDlgSearchUser->OnUserItemDelete(pIClientUserItem,this);

	//˽�Ĵ���
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];
		if (pDlgWhisper->m_hWnd!=NULL) pDlgWhisper->OnEventUserLeave(pIClientUserItem);
	}

	//�˵�����
	for (INT_PTR i=0;i<m_MenuUserItemArray.GetCount();i++)
	{
		//��ȡ�û�
		IClientUserItem * pIChatUserItem=m_MenuUserItemArray[i];
		if (pIChatUserItem->GetUserID()==dwLeaveUserID) m_MenuUserItemArray.RemoveAt(i);
	}

	//��ʾ��Ϣ
	if ((pParameterGlobal->m_bNotifyUserInOut==true)&&(m_ServiceStatus==ServiceStatus_ServiceIng))
	{
		m_ChatMessage.InsertUserLeave(pIClientUserItem->GetNickName());
	}

	return;
}

//�û�����
VOID CServerViewItem::OnUserFaceUpdate(IClientUserItem * pIClientUserItem)
{
	//��������
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

	//�б����
	m_UserListControl.UpdateDataItem(pIClientUserItem);

	//���Ҵ���
	CDlgSearchUser * pDlgSearchUser=CDlgSearchUser::GetInstance();
	if (pDlgSearchUser!=NULL) pDlgSearchUser->OnUserItemUpdate(pIClientUserItem,this);

	//˽�Ĵ���
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];
		if (pDlgWhisper->m_hWnd!=NULL) pDlgWhisper->OnEventUserStatus(pIClientUserItem);
	}

	//��������
	if ((pUserInfo->wTableID!=INVALID_TABLE)&&(pUserInfo->cbUserStatus!=US_LOOKON))
	{
		m_TableViewFrame.UpdateTableView(pUserInfo->wTableID);
	}

	//������Ϸ
	if ((pUserInfo->wTableID!=INVALID_TABLE)&&(m_pIMySelfUserItem->GetTableID()==pUserInfo->wTableID))
	{
		m_ProcessManager.SendCustomFace(pUserInfo->dwUserID,pUserInfo->dwCustomID,pCustomFaceInfo);
	}

	return;
}

//�û�����
VOID CServerViewItem::OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserScore & LastScore)
{
	//��������
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();

	//�б����
	m_UserListControl.UpdateDataItem(pIClientUserItem);

	//���Ҵ���
	CDlgSearchUser * pDlgSearchUser=CDlgSearchUser::GetInstance();
	if (pDlgSearchUser!=NULL) pDlgSearchUser->OnUserItemUpdate(pIClientUserItem,this);

	//˽�Ĵ���
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];
		if (pDlgWhisper->m_hWnd!=NULL) pDlgWhisper->OnEventUserStatus(pIClientUserItem);
	}

	//��Ϸ֪ͨ
	if ((pMeUserInfo->wTableID!=INVALID_TABLE)&&(pUserInfo->wTableID==pMeUserInfo->wTableID))
	{
		//��������
		tagUserScore UserScore;
		ZeroMemory(&UserScore,sizeof(UserScore));

		//���ñ���
		UserScore.lScore=pUserInfo->lScore;
		UserScore.lGrade=pUserInfo->lGrade;
		UserScore.lInsure=pUserInfo->lInsure;
		UserScore.dwWinCount=pUserInfo->dwWinCount;
		UserScore.dwLostCount=pUserInfo->dwLostCount;
		UserScore.dwDrawCount=pUserInfo->dwDrawCount;
		UserScore.dwFleeCount=pUserInfo->dwFleeCount;
		UserScore.dwUserMedal=pUserInfo->dwUserMedal;
		UserScore.dwExperience=pUserInfo->dwExperience;
		UserScore.dwLoveLiness=pUserInfo->dwLoveLiness;

		//��������
		m_ProcessManager.SendUserScore(pUserInfo->dwUserID,UserScore);
	}

	return;
}

//�û�����
VOID CServerViewItem::OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserStatus & LastStatus)
{
	//��������
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();
	WORD wNowTableID=pIClientUserItem->GetTableID(),wLastTableID=LastStatus.wTableID;
	WORD wNowChairID=pIClientUserItem->GetChairID(),wLastChairID=LastStatus.wChairID;
	BYTE cbNowStatus=pIClientUserItem->GetUserStatus(),cbLastStatus=LastStatus.cbUserStatus;

	//�б����
	m_UserListControl.UpdateDataItem(pIClientUserItem);

	//���Ҵ���
	CDlgSearchUser * pDlgSearchUser=CDlgSearchUser::GetInstance();
	if (pDlgSearchUser!=NULL) pDlgSearchUser->OnUserItemUpdate(pIClientUserItem,this);

	//˽�Ĵ���
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];
		if (pDlgWhisper->m_hWnd!=NULL) pDlgWhisper->OnEventUserStatus(pIClientUserItem);
	}

	//�����뿪
	if ((wLastTableID!=INVALID_TABLE)&&((wLastTableID!=wNowTableID)||(wLastChairID!=wNowChairID)))
	{
		IClientUserItem * pITableUserItem=m_TableViewFrame.GetClientUserItem(wLastTableID,wLastChairID);
		if (pITableUserItem==pIClientUserItem) m_TableViewFrame.SetClientUserItem(wLastTableID,wLastChairID,NULL);
	}

	//���Ӽ���
	if ((wNowTableID!=INVALID_TABLE)&&(cbNowStatus!=US_LOOKON)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		m_TableViewFrame.SetClientUserItem(wNowTableID,wNowChairID,pIClientUserItem);
	}

	//����״̬
	if ((wNowTableID!=INVALID_TABLE)&&(wNowTableID==wLastTableID)&&(wNowChairID==wLastChairID))
	{
		m_TableViewFrame.UpdateTableView(wNowTableID);
	}

	//�뿪֪ͨ
	if ((wLastTableID!=INVALID_TABLE)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		//��Ϸ֪ͨ
		if ((pIClientUserItem==m_pIMySelfUserItem)||(wLastTableID==pMeUserInfo->wTableID))
		{
			tagUserStatus UserStatus;
			UserStatus.wTableID=wNowTableID;
			UserStatus.wChairID=wNowChairID;
			UserStatus.cbUserStatus=cbNowStatus;
			m_ProcessManager.SendUserStatus(pUserInfo->dwUserID,UserStatus);
		}

		return;
	}

	//���봦��
	if ((wNowTableID!=INVALID_TABLE)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		//��Ϸ֪ͨ
		if ((m_pIMySelfUserItem!=pIClientUserItem)&&(pMeUserInfo->wTableID==wNowTableID))
		{
			ASSERT(wNowChairID!=INVALID_CHAIR);
			m_ProcessManager.SendUserItem(pIClientUserItem);
		}

		//�Լ��ж�
		if (m_pIMySelfUserItem==pIClientUserItem)
		{
			//���ñ���
			m_wReqTableID=INVALID_TABLE;
			m_wReqChairID=INVALID_CHAIR;

			//��������
			m_ProcessManager.CreateGameProcess(m_GameKind.szProcessName);
		}

		return;
	}

	//״̬�ı�
	if ((wNowTableID!=INVALID_TABLE)&&(wNowTableID==wLastTableID)&&(pMeUserInfo->wTableID==wNowTableID))
	{
		//��Ϸ֪ͨ
		tagUserStatus UserStatus;
		UserStatus.wTableID=wNowTableID;
		UserStatus.wChairID=wNowChairID;
		UserStatus.cbUserStatus=cbNowStatus;
		m_ProcessManager.SendUserStatus(pUserInfo->dwUserID,UserStatus);

		return;
	}

	return;
}

//ȡ������
VOID CServerViewItem::OnStatusCancel()
{
	//�رշ���
	PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return;
}

//����¼�
VOID CServerViewItem::OnSplitterEvent(UINT uControlID, INT nXPos, INT nYPos)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��ʾ�ؼ�
	if (m_bHideUserList==true)
	{
		//���ñ���
		m_bRectifyHor=true;
		m_bHideUserList=false;
	
		//���ð�ť
		HINSTANCE hInstance=AfxGetInstanceHandle();
		m_btControl.SetButtonImage(IDB_BK_USER_LIST,(m_bHideUserList==true)?TEXT("BT_USER_LIST_SHOW"):TEXT("BT_USER_LIST_HIDE"),hInstance,true,false);
	}

	//�����ؼ�
	RectifyControl(rcClient.Width(),rcClient.Height());

	return;
}

//�û�ѡ��
VOID CServerViewItem::OnChangeChatTarget(IClientUserItem * pIClientUserItem)
{
	//���ñ���
	if (pIClientUserItem!=NULL)
	{
		//���ý���
		if (m_ChatControl.m_hWnd==NULL)
		{
			//��������
			CRect rcCreate(0,0,0,0);
			m_ChatControl.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_CHAT_CONTROL);

			//��������
			CRect rcClient;
			GetClientRect(&rcClient);
			RectifyControl(rcClient.Width(),rcClient.Height());
		}
	}
	else
	{
		//���ý���
		if (m_ChatControl.m_hWnd!=NULL)
		{
			//���ٴ���
			m_ChatControl.DestroyWindow();

			//��������
			CRect rcClient;
			GetClientRect(&rcClient);
			RectifyControl(rcClient.Width(),rcClient.Height());
		}
	}

	//���ý���
	m_ChatEdit.SetFocus();

	return;
}

//�����¼�
VOID CServerViewItem::OnExpressionSelect(CExpression * pUserExpression, tagExpressionInfo * pExpressionInfo)
{
	//���ͱ���
	if (m_ServiceStatus==ServiceStatus_ServiceIng)
	{
		//���ý���
		m_ChatEdit.SetFocus();

		//���ͱ���
		if (EfficacyUserChat(NULL,pUserExpression->GetIndex())==true)
		{
			SendExpressionPacket(m_ChatControl.GetChatTargetUserID(),pUserExpression->GetIndex());
		}
	}

	return;
}

//��ʾ�˵�
VOID CServerViewItem::TrackUserItemMenu(IClientUserItem * pIClientUserItem)
{
	//Ч�����
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return;

	//�ж�״̬
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return;
	if (CServerRule::IsAvertCheatMode(m_dwServerRule)==true) return;

	//���ñ���
	m_MenuUserItemArray.RemoveAll();
	m_MenuUserItemArray.Add(pIClientUserItem);

	//����˵�
	CSkinMenu UserInfoMenu;
	UserInfoMenu.CreateMenu();

	//��������
	TCHAR szMenuString[256]=TEXT("");
	bool bMeUserItem=(pIClientUserItem==m_pIMySelfUserItem);

	//��������
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();
	IClientUserItem * pIChatTargetItem=m_ChatControl.GetChatTargetUserItem();

	//��������
	LPCTSTR pszUserNote=pIClientUserItem->GetUserNoteInfo();
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
	IGameLevelParser * pGameLevelParser=m_GameLevelParserModule.GetInterface();

	//�û���Ϣ
	WORD wTableID=pUserInfo->wTableID;
	WORD wChairID=pUserInfo->wChairID;
	BYTE cbUserStatus=pUserInfo->cbUserStatus;

	//�����Ϣ
	WORD wMeTableID=pMeUserInfo->wTableID;
	WORD wMeChiarID=pMeUserInfo->wChairID;
	BYTE cbMeUserStatus=pMeUserInfo->cbUserStatus;

	//������Ƭ
	CImage ImageUserCard;
	pUserItemElement->ConstructNameCard(pIClientUserItem,pGameLevelParser,ImageUserCard);
	if (ImageUserCard.IsNull()==false) UserInfoMenu.AppendMenu(IDM_NULL_COMMAND,ImageUserCard,MF_GRAYED);

	//����ָ�
	UserInfoMenu.AppendSeparator();

	//����˵�
	if (bMeUserItem==false)
	{
		UserInfoMenu.AppendMenu(IDM_CREATE_ISER_WISPER,TEXT("����˽����Ϣ ..."));
		UserInfoMenu.SetDefaultItem(IDM_CREATE_ISER_WISPER,MF_BYCOMMAND);
	}
	UserInfoMenu.AppendMenu(IDM_COPY_USER_NICKNAME,TEXT("�����ǳ�"));

	//���ý�̸
	if ((bMeUserItem==false)&&(pIChatTargetItem!=m_pIMySelfUserItem)&&(pIChatTargetItem!=pIClientUserItem))
	{
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("�� [ %s ] ��̸"),pIClientUserItem->GetNickName());
		UserInfoMenu.AppendMenu(IDM_SET_CHAT_USER,szMenuString);
	}

	//ȡ����̸
	if (pIChatTargetItem!=NULL)
	{
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("ȡ���� [ %s ] ��̸"),pIChatTargetItem->GetNickName());
		UserInfoMenu.AppendMenu(IDM_CANCEL_CHAT_USER,szMenuString);
	}

	//�����˵�
	if ((bMeUserItem==false)&&(cbMeUserStatus!=US_PLAYING))
	{
		//����ָ�
		UserInfoMenu.AppendSeparator();

		//�Թ���Ϸ
		bool bEnableMenu=((wTableID!=INVALID_TABLE)&&((cbUserStatus==US_PLAYING)||(cbUserStatus==US_OFFLINE)));
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("�Թ� [ %s ] ��Ϸ"),pUserInfo->szNickName);
		UserInfoMenu.AppendMenu(IDM_LOOKON_USER,szMenuString,(bEnableMenu==false)?MF_GRAYED:0);

		//һ����Ϸ
		bEnableMenu=((wTableID!=INVALID_TABLE)&&((cbUserStatus==US_SIT)||(cbUserStatus==US_READY)));
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("���� [ %s ] ����Ϸ��"),pUserInfo->szNickName);
		UserInfoMenu.AppendMenu(IDM_PLAY_GAME_TOGETHER,szMenuString,(bEnableMenu==false)?MF_GRAYED:0);

		//������Ϸ
		bEnableMenu=((wMeTableID!=INVALID_TABLE)&&(cbMeUserStatus!=US_LOOKON)&&(cbUserStatus!=US_PLAYING)&&(cbUserStatus!=US_OFFLINE));
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("���� [ %s ] һ������Ϸ"),pUserInfo->szNickName);
		UserInfoMenu.AppendMenu(IDM_INVITE_USER,szMenuString,(bEnableMenu==false)?MF_GRAYED:0);
	}

	//���ѹ���
	if (bMeUserItem==false)
	{
		//����ָ�
		UserInfoMenu.AppendSeparator();

		//����˵�
		BYTE cbCompanion=pIClientUserItem->GetUserCompanion();
		UserInfoMenu.AppendMenu(IDM_SET_FRIEND_USER,TEXT("����Ϊ����"),(cbCompanion==CP_FRIEND)?MF_CHECKED:0);
		UserInfoMenu.AppendMenu(IDM_SET_DETEST_USER,TEXT("����Ϊ���"),(cbCompanion==CP_DETEST)?MF_CHECKED:0);
	}

	//����˵�
	if ((bMeUserItem==false)&&(m_pIMySelfUserItem->GetMasterOrder()!=0))
	{
		//����ָ�
		/*UserInfoMenu.AppendSeparator();

		//����˵�
		UserInfoMenu.AppendMenu(IDM_KILL_USER,TEXT("���û�����..."),(CMasterRight::CanKillUser(m_dwMasterRight)==false)?MF_GRAYED:0);
		
		//��ҹ���
		UINT nMenuFlags=(CMasterRight::CanLimitUserChat(m_dwMasterRight)==false)?MF_GRAYED:0;
		UserInfoMenu.AppendSeparator();
		UserInfoMenu.AppendMenu(IDM_LIMIT_USER_ROOM_CHAT,TEXT("��ֹ���û���������"),nMenuFlags);
		UserInfoMenu.AppendMenu(IDM_ALLOW_USER_ROOM_CHAT,TEXT("������û���������"),nMenuFlags);
		UserInfoMenu.AppendMenu(IDM_LIMIT_USER_GAME_CHAT,TEXT("��ֹ���û���Ϸ����"),nMenuFlags);
		UserInfoMenu.AppendMenu(IDM_ALLOW_USER_GAME_CHAT,TEXT("������û���Ϸ����"),nMenuFlags);
		UserInfoMenu.AppendMenu(IDM_LIMIT_USER_WHISP_CHAT,TEXT("��ֹ���û�˽��"),nMenuFlags);
		UserInfoMenu.AppendMenu(IDM_ALLOW_USER_WHISP_CHAT,TEXT("������û�˽��"),nMenuFlags);*/
	}

	//����ָ�
	UserInfoMenu.AppendSeparator();

	//�鿴λ��
	UserInfoMenu.AppendMenu(IDM_SEE_USER_LOCATION,(bMeUserItem==true)?TEXT("�鿴�ҵ�λ��"):TEXT("�鿴�û�λ��"));
	if ((wTableID==INVALID_TABLE)||(wChairID==INVALID_CHAIR)) UserInfoMenu.EnableMenuItem(IDM_SEE_USER_LOCATION,MF_BYCOMMAND|MF_GRAYED);

	//���ܲ˵�
	UserInfoMenu.AppendMenu(IDM_SHOW_USER_INFO,TEXT("�û���ϸ���� ..."),MF_GRAYED);
	UserInfoMenu.AppendMenu(IDM_USER_INFO_MANAGER,TEXT("�û���Ϣ���� ..."),MF_GRAYED);
	UserInfoMenu.AppendMenu(IDM_SEARCH_ONLINE_USER,TEXT("���������û� ..."));

	//����ָ�
	UserInfoMenu.AppendSeparator();

	//���λ��
	if (wTableID!=INVALID_TABLE)
	{
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("���� [ %d ] ����Ϸ��"),wTableID+1);
		UserInfoMenu.AppendMenu(IDM_NULL_COMMAND,szMenuString);
	}

	//������Ϣ
	DWORD dwPlayCount=pIClientUserItem->GetUserPlayCount();
	FLOAT fUserFleeRate=pIClientUserItem->GetUserFleeRate();
	_sntprintf(szMenuString,CountArray(szMenuString),TEXT("�ɼ���") SCORE_STRING TEXT(" ����ֵ��%ld �ܾ�����%ld �����ʣ�%5.2f%%"),
		pUserInfo->lScore,pUserInfo->dwExperience,dwPlayCount,fUserFleeRate);
	UserInfoMenu.AppendMenu(IDM_NULL_COMMAND,szMenuString);

	//������Ϣ
	if (pUserInfo->szGroupName[0]!=0)
	{
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("��Ϸ���ţ�%s"),pUserInfo->szGroupName);
		UserInfoMenu.AppendMenu(IDM_NULL_COMMAND,szMenuString);
	}

	//��ע��Ϣ
	if ((pszUserNote!=NULL)&&(pszUserNote[0]!=0))
	{
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("��ע��Ϣ��%s"),pszUserNote);
		UserInfoMenu.AppendMenu(IDM_NULL_COMMAND,szMenuString);
	}

	//�����˵�
	UserInfoMenu.TrackPopupMenu(this);

	return;
}

//��ʾ�˵�
VOID CServerViewItem::TrackUserItemMenu(IClientUserItem * pIClientUserItem[], WORD wUserCount)
{
	//Ч�����
	ASSERT((pIClientUserItem!=NULL)&&(wUserCount>0));
	if ((pIClientUserItem==NULL)||(wUserCount==0)) return;

	//�ж�״̬
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return;
	if (CServerRule::IsAvertCheatMode(m_dwServerRule)==true) return;

	//���ñ���
	m_MenuUserItemArray.RemoveAll();
	m_MenuUserItemArray.SetSize(wUserCount);
	CopyMemory(m_MenuUserItemArray.GetData(),pIClientUserItem,wUserCount*sizeof(IClientUserItem *));

	//����˵�
	CSkinMenu UserInfoMenu;
	UserInfoMenu.CreateMenu();

	//��������
	TCHAR szMenuString[256]=TEXT("");
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();
	IClientUserItem * pIChatTargetItem=m_ChatControl.GetChatTargetUserItem();

	//�����Ϣ
	WORD wMeTableID=pMeUserInfo->wTableID;
	WORD wMeChiarID=pMeUserInfo->wChairID;
	BYTE cbMeUserStatus=pMeUserInfo->cbUserStatus;

	//����˽��
	UserInfoMenu.AppendMenu(IDM_CREATE_ISER_WISPER,TEXT("����Ⱥ����Ϣ ..."),MF_GRAYED);
	UserInfoMenu.SetDefaultItem(IDM_CREATE_ISER_WISPER,MF_BYCOMMAND);

	//�������
	if (pIChatTargetItem!=NULL)
	{
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("ȡ���� [ %s ] ��̸"),pIChatTargetItem->GetNickName());
		UserInfoMenu.AppendMenu(IDM_CANCEL_CHAT_USER,szMenuString);
	}

	//����ָ�
	UserInfoMenu.AppendSeparator();

	//����˵�
	UserInfoMenu.AppendMenu(IDM_SET_FRIEND_USER,TEXT("����Ϊ����"));
	UserInfoMenu.AppendMenu(IDM_SET_DETEST_USER,TEXT("����Ϊ���"));

	//����ָ�
	UserInfoMenu.AppendSeparator();

	//���ܲ˵�
	UserInfoMenu.AppendMenu(IDM_USER_INFO_MANAGER,TEXT("�û���Ϣ���� ..."));
	UserInfoMenu.AppendMenu(IDM_SEARCH_ONLINE_USER,TEXT("���������û� ..."));

	//�����˵�
	UserInfoMenu.TrackPopupMenu(this);

	return;
}

//��ʾ����
VOID CServerViewItem::ShowInsureView()
{
	//Ч�����
	ASSERT(m_pIMySelfUserItem!=NULL);
	ASSERT(m_TCPSocketModule.GetInterface()!=NULL);

	//��������
	CDlgInsureServer DlgInsureServer;
	DlgInsureServer.Initialization(m_TCPSocketModule.GetInterface(),m_pIMySelfUserItem);
	
	//���ñ���
	m_pDlgInsureServer=&DlgInsureServer;

	//��ʾ����
	DlgInsureServer.DoModal();

	//���ñ���
	m_pDlgInsureServer=NULL;

	return;
}

//�ϰ��¼�
VOID CServerViewItem::NotifyBossCome(bool bBossCome)
{
	return;
}

//��������
VOID CServerViewItem::SearchGameTable(tagSearchTable & SearchTable)
{
	//��������
	WORD wNullCount=0;
	WORD wChairUser=m_TableViewFrame.GetChairCount();
	WORD wMeTableID=m_pIMySelfUserItem->GetTableID();

	//��������
	for (WORD i=0;i<m_TableViewFrame.GetTableCount();i++)
	{
		//��������
		SearchTable.wResultTableID=(SearchTable.wStartTableID+i)%m_TableViewFrame.GetTableCount();

		//��������
		if (wMeTableID!=SearchTable.wResultTableID)
		{
			//��ȡ����
			ITableView * pITableView=m_TableViewFrame.GetTableViewItem(SearchTable.wResultTableID);

			//��������
			if (pITableView->GetPlayFlag()==true) continue;
			if ((SearchTable.bFilterPass==true)&&(pITableView->GetLockerFlag()==true)) continue;

			//Ѱ�ҿ�λ��
			wNullCount=pITableView->GetNullChairCount(SearchTable.wResultChairID);

			//�ж���Ŀ
			if (wNullCount>0)
			{
				//Ч�����
				CString strDescribe;
				if (EfficacyTableRule(SearchTable.wResultTableID,SearchTable.wResultChairID,false,strDescribe)==false)
				{
					continue;
				}
		
				//��Ŀ�ж�
				if ((SearchTable.bOneNull==true)&&(wNullCount==1)) return;
				if ((SearchTable.bTwoNull==true)&&(wNullCount==2)) return;
				if ((SearchTable.bNotFull==true)&&(wNullCount<wChairUser)) return;
				if ((SearchTable.bAllNull==true)&&(wNullCount==wChairUser)) return;
			}
		}
	}

	//��������
	SearchTable.wResultTableID=INVALID_TABLE;
	SearchTable.wResultChairID=INVALID_CHAIR;

	return;
}

//����ͷ��
VOID CServerViewItem::UpdateUserCustomFace(DWORD dwUserID, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo)
{
	//�����û�
	ASSERT(m_PlazaUserManagerModule.GetInterface()!=NULL);
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule.GetInterface()->SearchUserByUserID(dwUserID);

	//�����û�
	if (pIClientUserItem!=NULL)
	{
		ASSERT(m_PlazaUserManagerModule.GetInterface()!=NULL);
		m_PlazaUserManagerModule->UpdateUserCustomFace(pIClientUserItem,dwCustomID,CustomFaceInfo);
	}

	return;
}

//�û�����
bool CServerViewItem::OnSocketMainUser(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_USER_ENTER:			//�û�����
		{
			return OnSocketSubUserEnter(pData,wDataSize);
		}
	case SUB_GR_USER_SCORE:			//�û�����
		{
			return OnSocketSubUserScore(pData,wDataSize);
		}
	case SUB_GR_USER_STATUS:		//�û�״̬
		{
			return OnSocketSubUserStatus(pData,wDataSize);
		}
	case SUB_GR_REQUEST_FAILURE:	//����ʧ��
		{
			return OnSocketSubRequestFailure(pData,wDataSize);
		}
	case SUB_GR_USER_CHAT:			//�û�����
		{
			return OnSocketSubUserChat(pData,wDataSize);
		}
	case SUB_GR_USER_EXPRESSION:	//�û�����
		{
			return OnSocketSubExpression(pData,wDataSize);
		}
	case SUB_GR_WISPER_CHAT:		//�û�˽��
		{
			return OnSocketSubWisperUserChat(pData,wDataSize);
		}
	case SUB_GR_WISPER_EXPRESSION:	//˽�ı���
		{
			return OnSocketSubWisperExpression(pData,wDataSize);
		}
	}

	return true;
}

//��¼����
bool CServerViewItem::OnSocketMainLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_LOGON_SUCCESS:	//��¼�ɹ�
		{
			return OnSocketSubLogonSuccess(pData,wDataSize);
		}
	case SUB_GR_LOGON_FAILURE:	//��¼ʧ��
		{
			return OnSocketSubLogonFailure(pData,wDataSize);
		}
	case SUB_GR_LOGON_FINISH:	//��¼���
		{
			return OnSocketSubLogonFinish(pData,wDataSize);
		}
	case SUB_GR_UPDATE_NOTIFY:	//������ʾ
		{
			return OnSocketSubUpdateNotify(pData,wDataSize);
		}
	}

	return true;
}

//���ô���
bool CServerViewItem::OnSocketMainConfig(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_CONFIG_COLUMN:		//�б�����
		{
			//��������
			CMD_GR_ConfigColumn * pConfigColumn=(CMD_GR_ConfigColumn *)pData;
			WORD wHeadSize=sizeof(CMD_GR_ConfigColumn)-sizeof(pConfigColumn->ColumnItem);

			//Ч�����
			ASSERT((wHeadSize+pConfigColumn->cbColumnCount*sizeof(pConfigColumn->ColumnItem[0]))==wDataSize);
			if ((wHeadSize+pConfigColumn->cbColumnCount*sizeof(pConfigColumn->ColumnItem[0]))!=wDataSize) return false;

			//������Ϣ
			m_cbColumnCount=__min(pConfigColumn->cbColumnCount,CountArray(m_ColumnItem));
			CopyMemory(m_ColumnItem,pConfigColumn->ColumnItem,sizeof(tagColumnItem)*m_cbColumnCount);

			//�����б�
			m_UserListControl.SetColumnDescribe(pConfigColumn->ColumnItem,pConfigColumn->cbColumnCount);

			return true;
		}
	case SUB_GR_CONFIG_SERVER:		//��������
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_GR_ConfigServer));
			if (wDataSize<sizeof(CMD_GR_ConfigServer)) return false;

			//��Ϣ����
			CMD_GR_ConfigServer * pConfigServer=(CMD_GR_ConfigServer *)pData;

			//��������
			m_wServerType=pConfigServer->wServerType;
			m_dwServerRule=pConfigServer->dwServerRule;

			//�������
			WORD wTableCount=pConfigServer->wTableCount;
			WORD wChairCount=pConfigServer->wChairCount;
			IUnknownEx * pIUnknownEx=QUERY_ME_INTERFACE(IUnknownEx);

			//��ԴĿ¼
			TCHAR szResDirectory[LEN_KIND]=TEXT("");
			GetGameResDirectory(szResDirectory,CountArray(szResDirectory));

			//��������
			bool bSuccess=m_TableViewFrame.ConfigTableFrame(wTableCount,wChairCount,m_dwServerRule,szResDirectory);

			//������
			if (bSuccess==false)
			{
				//������ʾ
				m_DlgStatus.HideStatusWindow();

				//�ر�����
				m_TCPSocketModule->CloseSocket();

				//��ʾ��Ϣ
				CInformation Information(AfxGetMainWnd());
				INT nResult=Information.ShowMessageBox(TEXT("��Ϸ��Դ�ļ�����ʧ�ܻ��߸�ʽ�����Ƿ����ڽ����޸���"),MB_ICONERROR|MB_YESNO);

				//������Ϸ
				if (nResult==IDYES)
				{
					CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
					pGlobalUnits->DownLoadClient(m_GameKind.szKindName,m_GameKind.wGameID,m_GameServer.wServerID);
				}

				//�رշ���
				PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);
			}

			return true;
		}
	case SUB_GR_CONFIG_PROPERTY:	//��������
		{
			//��������
			CMD_GR_ConfigProperty * pConfigProperty=(CMD_GR_ConfigProperty *)pData;
			WORD wHeadSize=sizeof(CMD_GR_ConfigProperty)-sizeof(pConfigProperty->PropertyInfo);

			//Ч�����
			ASSERT((wHeadSize+pConfigProperty->cbPropertyCount*sizeof(pConfigProperty->PropertyInfo[0]))==wDataSize);
			if ((wHeadSize+pConfigProperty->cbPropertyCount*sizeof(pConfigProperty->PropertyInfo[0]))!=wDataSize) return false;

			//������Ϣ
			m_cbPropertyCount=__min(pConfigProperty->cbPropertyCount,CountArray(m_PropertyInfo));
			CopyMemory(m_PropertyInfo,pConfigProperty->PropertyInfo,sizeof(tagPropertyInfo)*m_cbPropertyCount);

			//���õ���
			m_GamePropertyCtrl.SetGamePropertyInfo(m_PropertyInfo,m_cbPropertyCount,PT_ISSUE_AREA_SERVER);

			return true;
		}
	case SUB_GR_CONFIG_FINISH:		//�������
		{
			//��ԴĿ¼
			TCHAR szResDirectory[LEN_KIND]=TEXT("");
			GetGameResDirectory(szResDirectory,CountArray(szResDirectory));

			//��Ϸ�ȼ�
			ASSERT(m_GameLevelParserModule.GetInterface()!=NULL);
			bool bSuccess=m_GameLevelParserModule->LoadGameLevelItem(m_GameKind.szKindName,szResDirectory,m_wServerType);

			//������
			if (bSuccess==false)
			{
				//�ر�����
				m_TCPSocketModule->CloseSocket();

				//��ʾ��Ϣ
				INT nResult=ShowInformation(TEXT("��Ϸ�ȼ����ö�ȡʧ�ܻ��߸�ʽ�����Ƿ����ڽ����޸���"),MB_ICONERROR|MB_YESNO);

				//������Ϸ
				if (nResult==IDYES)
				{
					CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
					pGlobalUnits->DownLoadClient(m_GameKind.szKindName,m_GameKind.wGameID,m_GameServer.wServerID);
				}

				//�رշ���
				PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);
			}

			return true;
		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//״̬����
bool CServerViewItem::OnSocketMainStatus(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_TABLE_INFO:		//������Ϣ
		{
			//��������
			CMD_GR_TableInfo * pTableInfo=(CMD_GR_TableInfo *)pData;
			WORD wHeadSize=sizeof(CMD_GR_TableInfo)-sizeof(pTableInfo->TableStatusArray);

			//Ч������
			ASSERT((wHeadSize+pTableInfo->wTableCount*sizeof(pTableInfo->TableStatusArray[0]))==wDataSize);
			if ((wHeadSize+pTableInfo->wTableCount*sizeof(pTableInfo->TableStatusArray[0]))!=wDataSize) return false;

			//��Ϣ����
			for (WORD i=0;i<pTableInfo->wTableCount;i++)
			{
				BYTE cbTableLock=pTableInfo->TableStatusArray[i].cbTableLock;
				BYTE cbPlayStatus=pTableInfo->TableStatusArray[i].cbPlayStatus;
				m_TableViewFrame.SetTableStatus(i,(cbPlayStatus==TRUE),(cbTableLock==TRUE));
			}

			return true;
		}
	case SUB_GR_TABLE_STATUS:	//����״̬
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GR_TableStatus));
			if (wDataSize!=sizeof(CMD_GR_TableStatus)) return false;

			//��������
			CMD_GR_TableStatus * pTableStatus=(CMD_GR_TableStatus *)pData;

			//��������
			BYTE cbTableLock=pTableStatus->TableStatus.cbTableLock;
			BYTE cbPlayStatus=pTableStatus->TableStatus.cbPlayStatus;
			m_TableViewFrame.SetTableStatus(pTableStatus->wTableID,(cbPlayStatus==TRUE),(cbTableLock==TRUE));

			return true;
		}
	}

	return true;
}

//���д���
bool CServerViewItem::OnSocketMainInsure(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//Ч��״̬
	ASSERT(m_pDlgInsureServer!=NULL);
	if (m_pDlgInsureServer==NULL) return true;

	//��Ϣ����
	if (m_pDlgInsureServer->OnServerInsureMessage(MDM_GR_INSURE,wSubCmdID,pData,wDataSize)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//������
bool CServerViewItem::OnSocketMainManager(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_OPTION_CURRENT:	//��ǰ����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GR_OptionCurrent));
			if (wDataSize!=sizeof(CMD_GR_OptionCurrent)) return false;

			//��Ϣ����
			if (m_pDlgServerManager!=NULL)
			{
				CMD_GR_OptionCurrent * pOptionCurrent=(CMD_GR_OptionCurrent *)pData;
				m_pDlgServerManager->SetServerOptionInfo(pOptionCurrent->ServerOptionInfo,pOptionCurrent->dwRuleMask);
			}

			return true;
		}
	}

	return true;
}

//ϵͳ����
bool CServerViewItem::OnSocketMainSystem(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CM_SYSTEM_MESSAGE:	//ϵͳ��Ϣ
		{
			return OnSocketSubSystemMessage(pData,wDataSize);
		}
	case SUB_CM_ACTION_MESSAGE:	//������Ϣ
		{
			return OnSocketSubActionMessage(pData,wDataSize);
		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//��Ϸ����
bool CServerViewItem::OnSocketMainGameFrame(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize<=SOCKET_TCP_PACKET);
	if (wDataSize>SOCKET_TCP_PACKET) return false;

	//��������
	IPC_GF_SocketRecv SocketRecv;
	SocketRecv.CommandInfo.wSubCmdID=wSubCmdID;
	SocketRecv.CommandInfo.wMainCmdID=wMainCmdID;

	//��������
	if (wDataSize>0)
	{
		ASSERT(pData!=NULL);
		CopyMemory(SocketRecv.cbBuffer,pData,wDataSize);
	}

	//��������
	WORD wSendSize=sizeof(SocketRecv)-sizeof(SocketRecv.cbBuffer)+wDataSize;
	m_ProcessManager.SendProcessData(IPC_CMD_GF_SOCKET,IPC_SUB_GF_SOCKET_RECV,&SocketRecv,wSendSize);

	return true;
}

//�û�����
bool CServerViewItem::OnSocketSubUserEnter(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(tagUserInfoHead));
	if (wDataSize<sizeof(tagUserInfoHead)) return false;

	//��������
	tagUserInfo UserInfo;
	tagCustomFaceInfo CustomFaceInfo;
	ZeroMemory(&UserInfo,sizeof(UserInfo));
	ZeroMemory(&CustomFaceInfo,sizeof(CustomFaceInfo));

	//��Ϣ����
	tagUserInfoHead * pUserInfoHead=(tagUserInfoHead *)pData;

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��������
	bool bHideUserInfo=CServerRule::IsAvertCheatMode(m_dwServerRule);
	bool bMySelfUserItem=pGlobalUserData->dwUserID==pUserInfoHead->dwUserID;
	bool bMasterUserOrder=((pUserInfoHead->cbMasterOrder>0)||((m_pIMySelfUserItem!=NULL)&&(m_pIMySelfUserItem->GetMasterOrder()>0)));

	//��ȡ��Ϣ
	if ((bHideUserInfo==false)||(bMySelfUserItem==true)||(bMasterUserOrder==true))
	{
		//�û�����
		UserInfo.wFaceID=pUserInfoHead->wFaceID;
		UserInfo.dwGameID=pUserInfoHead->dwGameID;
		UserInfo.dwUserID=pUserInfoHead->dwUserID;
		UserInfo.dwGroupID=pUserInfoHead->dwGroupID;
		UserInfo.dwCustomID=pUserInfoHead->dwCustomID;

		//�û�״̬
		UserInfo.wTableID=pUserInfoHead->wTableID;
		UserInfo.wChairID=pUserInfoHead->wChairID;
		UserInfo.cbUserStatus=pUserInfoHead->cbUserStatus;

		//�û�����
		UserInfo.cbGender=pUserInfoHead->cbGender;
		UserInfo.cbMemberOrder=pUserInfoHead->cbMemberOrder;
		UserInfo.cbMasterOrder=pUserInfoHead->cbMasterOrder;

		//�û�����
		UserInfo.lScore=pUserInfoHead->lScore;
		UserInfo.lGrade=pUserInfoHead->lGrade;
		UserInfo.lInsure=pUserInfoHead->lInsure;
		UserInfo.dwWinCount=pUserInfoHead->dwWinCount;
		UserInfo.dwLostCount=pUserInfoHead->dwLostCount;
		UserInfo.dwDrawCount=pUserInfoHead->dwDrawCount;
		UserInfo.dwFleeCount=pUserInfoHead->dwFleeCount;
		UserInfo.dwUserMedal=pUserInfoHead->dwUserMedal;
		UserInfo.dwExperience=pUserInfoHead->dwExperience;
		UserInfo.dwLoveLiness=pUserInfoHead->dwLoveLiness;

		//��������
		VOID * pDataBuffer=NULL;
		tagDataDescribe DataDescribe;
		CRecvPacketHelper RecvPacket(pUserInfoHead+1,wDataSize-sizeof(tagUserInfoHead));

		//��չ��Ϣ
		while (true)
		{
			pDataBuffer=RecvPacket.GetData(DataDescribe);
			if (DataDescribe.wDataDescribe==DTP_NULL) break;
			switch (DataDescribe.wDataDescribe)
			{
			case DTP_GR_NICK_NAME:		//�û��ǳ�
				{
					ASSERT(pDataBuffer!=NULL);
					ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szNickName));
					if (DataDescribe.wDataSize<=sizeof(UserInfo.szNickName))
					{
						CopyMemory(&UserInfo.szNickName,pDataBuffer,DataDescribe.wDataSize);
						UserInfo.szNickName[CountArray(UserInfo.szNickName)-1]=0;
					}
					break;
				}
			case DTP_GR_GROUP_NAME:		//�û�����
				{
					ASSERT(pDataBuffer!=NULL);
					ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szGroupName));
					if (DataDescribe.wDataSize<=sizeof(UserInfo.szGroupName))
					{
						CopyMemory(&UserInfo.szGroupName,pDataBuffer,DataDescribe.wDataSize);
						UserInfo.szGroupName[CountArray(UserInfo.szGroupName)-1]=0;
					}
					break;
				}
			case DTP_GR_UNDER_WRITE:	//����ǩ��
				{
					ASSERT(pDataBuffer!=NULL);
					ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szUnderWrite));
					if (DataDescribe.wDataSize<=sizeof(UserInfo.szUnderWrite))
					{
						CopyMemory(UserInfo.szUnderWrite,pDataBuffer,DataDescribe.wDataSize);
						UserInfo.szUnderWrite[CountArray(UserInfo.szUnderWrite)-1]=0;
					}
					break;
				}
			}
		}

		//�Զ�ͷ��
		if (pUserInfoHead->dwCustomID!=0L)
		{
			//����ͷ��
			CCustomFaceManager * pCustomFaceManager=CCustomFaceManager::GetInstance();
			bool bSuccess=pCustomFaceManager->LoadUserCustomFace(pUserInfoHead->dwUserID,pUserInfoHead->dwCustomID,CustomFaceInfo);

			//����ͷ��
			if (bSuccess==false)
			{
				pCustomFaceManager->LoadUserCustomFace(pUserInfoHead->dwUserID,pUserInfoHead->dwCustomID);
			}
		}
	}
	else
	{
		//�û���Ϣ
		UserInfo.dwUserID=pUserInfoHead->dwUserID;
		lstrcpyn(UserInfo.szNickName,TEXT("��Ϸ���"),CountArray(UserInfo.szNickName));

		//�û�״̬
		UserInfo.wTableID=pUserInfoHead->wTableID;
		UserInfo.wChairID=pUserInfoHead->wChairID;
		UserInfo.cbUserStatus=pUserInfoHead->cbUserStatus;

		//�û�����
		UserInfo.cbGender=pUserInfoHead->cbGender;
		UserInfo.cbMemberOrder=pUserInfoHead->cbMemberOrder;
		UserInfo.cbMasterOrder=pUserInfoHead->cbMasterOrder;
	}

	//�����û�
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(UserInfo.dwUserID);
	if (pIClientUserItem==NULL) pIClientUserItem=m_PlazaUserManagerModule->ActiveUserItem(UserInfo,CustomFaceInfo);

	return true;
}

//�û�����
bool CServerViewItem::OnSocketSubUserScore(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_UserScore));
	if (wDataSize<sizeof(CMD_GR_UserScore)) return false;

	//Ѱ���û�
	CMD_GR_UserScore * pUserScore=(CMD_GR_UserScore *)pData;
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserScore->dwUserID);

	//�û��ж�
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return true;

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��������
	bool bHideUserInfo=CServerRule::IsAvertCheatMode(m_dwServerRule);
	bool bMySelfUserItem=pGlobalUserData->dwUserID==pUserScore->dwUserID;
	bool bMasterUserOrder=((pIClientUserItem->GetMasterOrder()>0)||(m_pIMySelfUserItem->GetMasterOrder()>0));

	//ȫ����Ϣ
	if (bMySelfUserItem==true)
	{
		//���ý���
		DWORD dwLastMedal=pIClientUserItem->GetUserMedal();
		DWORD dwCurrentMedal=pUserScore->UserScore.dwUserMedal;

		//���ý���
		if (dwCurrentMedal>dwLastMedal)
		{
			//���ñ���
			pGlobalUserData->dwUserMedal+=(dwCurrentMedal-dwLastMedal);

			//�¼�֪ͨ
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
		}
	}

	//�����û�
	if ((bHideUserInfo==false)||(bMySelfUserItem==true)||(bMasterUserOrder==true))
	{
		m_PlazaUserManagerModule->UpdateUserItemScore(pIClientUserItem,&pUserScore->UserScore);
	}

	return true;
}

//�û�״̬
bool CServerViewItem::OnSocketSubUserStatus(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_UserStatus));
	if (wDataSize<sizeof(CMD_GR_UserStatus)) return false;

	//Ѱ���û�
	CMD_GR_UserStatus * pUserStatus=(CMD_GR_UserStatus *)pData;
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserStatus->dwUserID);

	//�û��ж�
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return true;

	//����״̬
	if (pUserStatus->UserStatus.cbUserStatus==US_NULL) 
	{
		//ɾ���û�
		m_PlazaUserManagerModule->DeleteUserItem(pIClientUserItem);
	}
	else
	{
		//�����û�
		m_PlazaUserManagerModule->UpdateUserItemStatus(pIClientUserItem,&pUserStatus->UserStatus);
	}

	return true;
}

//��¼���
bool CServerViewItem::OnSocketSubLogonFinish(VOID * pData, WORD wDataSize)
{
	//�ر���ʾ
	m_DlgStatus.HideStatusWindow();

	//����״̬
	SetServiceStatus(ServiceStatus_ServiceIng);

	//�����
	CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
	if (pPlatformFrame!=NULL) pPlatformFrame->ActiveServerViewItem(this);

	//���Ұ�ť
	if (CServerRule::IsAvertCheatMode(m_dwServerRule)==false)
	{
		m_btTableButton2.EnableWindow(TRUE);
	}

	//�����ж�
	if (CServerRule::IsForfendLockTable(m_dwServerRule)==false)
	{
		//���ð�ť
		m_btTableButton3.EnableWindow(TRUE);
	}

	//�����ж�
	if (CServerRule::IsForfendGameRule(m_dwServerRule)==false)
	{
		//���͹���
		SendUserRulePacket();

		//���ð�ť
		m_btTableButton4.EnableWindow(TRUE);
	}

	//�����ж�
	if ((m_pIMySelfUserItem!=NULL)&&(m_pIMySelfUserItem->GetTableID()!=INVALID_TABLE))
	{
		//��������	
		bool bSuccess=m_ProcessManager.CreateGameProcess(m_GameKind.szProcessName);

		//������
		if (bSuccess==false)
		{
			//�ж�����
			m_TCPSocketModule->CloseSocket();
			m_ServiceStatus=ServiceStatus_NetworkDown;
		}
	}

	return true;
}

//��¼�ɹ�
bool CServerViewItem::OnSocketSubLogonSuccess(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_LogonSuccess));
	if (wDataSize!=sizeof(CMD_GR_LogonSuccess)) return false;

	//��Ϣ����
	CMD_GR_LogonSuccess * pLogonSuccess=(CMD_GR_LogonSuccess *)pData;

	//�������
	m_dwUserRight=pLogonSuccess->dwUserRight;
	m_dwMasterRight=pLogonSuccess->dwMasterRight;

	//����״̬
	SetServiceStatus(ServiceStatus_RecvInfo);

	//�ر���ʾ
	m_DlgStatus.ShowStatusWindow(TEXT("���ڶ�ȡ������Ϣ..."));

	//�����ַ
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/Ads/GameRoom.asp?KindID=%ld&ServerID=%ld"),szPlatformLink,m_GameKind.wKindID,m_GameServer.wServerID);

	//���ӹ��
	m_PlatformPublicize.Navigate(szBillUrl);
	m_PlatformPublicize.SetBoradColor(RGB(0,0,0));

	return true;
}

//��¼ʧ��
bool CServerViewItem::OnSocketSubLogonFailure(VOID * pData, WORD wDataSize)
{
	//Ч�����
	CMD_GR_LogonFailure * pLogonFailure=(CMD_GR_LogonFailure *)pData;
	ASSERT(wDataSize>=(sizeof(CMD_GR_LogonFailure)-sizeof(pLogonFailure->szDescribeString)));
	if (wDataSize<(sizeof(CMD_GR_LogonFailure)-sizeof(pLogonFailure->szDescribeString))) return false;

	//�ر���ʾ
	m_DlgStatus.HideStatusWindow();

	//�رմ���
	m_TCPSocketModule->CloseSocket();

	//����״̬
	SetServiceStatus(ServiceStatus_NetworkDown);

	//��ʾ��Ϣ
	LPCTSTR pszDescribeString=pLogonFailure->szDescribeString;
	if (lstrlen(pszDescribeString)>0) ShowInformation(pszDescribeString,MB_ICONERROR,60);

	//�رշ���
	PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return true;
}

//������ʾ
bool CServerViewItem::OnSocketSubUpdateNotify(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_UpdateNotify));
	if (wDataSize!=sizeof(CMD_GR_UpdateNotify)) return false;

	//��������
	CMD_GR_UpdateNotify * pUpdateNotify=(CMD_GR_UpdateNotify *)pData;

	//��������
	if (pUpdateNotify->cbMustUpdatePlaza==TRUE)
	{
		//�رմ���
		m_DlgStatus.HideStatusWindow();
		m_TCPSocketModule->CloseSocket();

		//�رշ���
		PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

		//���ش���
		CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
		if (pGlobalUnits!=NULL) pGlobalUnits->DownLoadClient(TEXT("��Ϸ�㳡"),0,0);

		return true;
	}

	//��Ϸ����
	if ((pUpdateNotify->cbMustUpdateClient==TRUE)||(pUpdateNotify->cbAdviceUpdateClient==TRUE))
	{
		//�رմ���
		if (pUpdateNotify->cbMustUpdateClient==TRUE)
		{
			m_DlgStatus.HideStatusWindow();
			m_TCPSocketModule->CloseSocket();
		}

		//������ʾ
		if (pUpdateNotify->cbMustUpdateClient==FALSE)
		{
			//������ʾ
			TCHAR szDescribe[512]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("��%s���Ѿ������ˣ�����ǿ�ҽ��������и��£����ڽ��и�����"),m_GameKind.szKindName);

			//��ʾ��Ϣ
			CInformation Information(this);
			if (Information.ShowMessageBox(szDescribe,MB_ICONINFORMATION|MB_YESNO,0)!=IDYES) return true;
		}

		//�رշ���
		PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

		//������ʾ
		CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
		pGlobalUnits->DownLoadClient(m_GameKind.szKindName,m_GameKind.wKindID,m_GameServer.wServerID);
	}

	return true;
}

//����ʧ��
bool CServerViewItem::OnSocketSubRequestFailure(VOID * pData, WORD wDataSize)
{
	//��������
	CMD_GR_RequestFailure * pRequestFailure=(CMD_GR_RequestFailure *)pData;

	//Ч�����
	ASSERT(wDataSize>(sizeof(CMD_GR_RequestFailure)-sizeof(pRequestFailure->szDescribeString)));
	if (wDataSize<=(sizeof(CMD_GR_RequestFailure)-sizeof(pRequestFailure->szDescribeString))) return false;

	//��������
	IClientUserItem * pITableUserItem=NULL;
	WORD wMeTableID=m_pIMySelfUserItem->GetTableID();
	WORD wMeChairID=m_pIMySelfUserItem->GetChairID();

	//���滹ԭ
	if (m_wReqTableID!=INVALID_TABLE)
	{
		IClientUserItem * pITableUserItem=m_TableViewFrame.GetClientUserItem(m_wReqTableID,m_wReqChairID);
		if (pITableUserItem==m_pIMySelfUserItem) m_TableViewFrame.SetClientUserItem(m_wReqTableID,m_wReqChairID,NULL);
	}

	//���ñ���
	m_wReqTableID=INVALID_TABLE;
	m_wReqChairID=INVALID_CHAIR;

	//��ʾ��Ϣ
	if (pRequestFailure->szDescribeString[0]!=0)
	{
		CInformation Information;
		Information.ShowMessageBox(pRequestFailure->szDescribeString,MB_ICONINFORMATION,30L);
	}

	return true;
}

//�û�����
bool CServerViewItem::OnSocketSubUserChat(VOID * pData, WORD wDataSize)
{
	//��������
	CMD_GR_S_UserChat * pUserChat=(CMD_GR_S_UserChat *)pData;

	//Ч�����
	ASSERT(wDataSize>=(sizeof(CMD_GR_S_UserChat)-sizeof(pUserChat->szChatString)));
	ASSERT(wDataSize==(sizeof(CMD_GR_S_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0])));

	//Ч�����
	if (wDataSize<(sizeof(CMD_GR_S_UserChat)-sizeof(pUserChat->szChatString))) return false;
	if (wDataSize!=(sizeof(CMD_GR_S_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0]))) return false;

	//��ȡ�û�
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserChat->dwSendUserID);
	if (pISendUserItem==NULL) return true;

	//��Ϣ����
	if (pISendUserItem!=m_pIMySelfUserItem)
	{
		//���ű���
		DWORD dwUserGroupID=pISendUserItem->GetGroupID();
		DWORD dwMySelfGroupID=m_pIMySelfUserItem->GetGroupID();

		//�����ж�
		bool bSameGroup=false;
		if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

		//��������
		BYTE cbUserCompanion=pISendUserItem->GetUserCompanion();
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

		//����ģʽ
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//���ģʽ
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}
	}

	//��ʾ��Ϣ
	if (pUserChat->dwTargetUserID!=0L)
	{
		//��ȡ�û�
		IClientUserItem * pIRecvUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserChat->dwTargetUserID);
		if (pIRecvUserItem==NULL) return true;

		//��ʾ��Ϣ
		LPCTSTR pszSendUser=pISendUserItem->GetNickName();
		LPCTSTR pszRecvUser=pIRecvUserItem->GetNickName();
		m_ChatMessage.InsertUserChat(pszSendUser,pszRecvUser,pUserChat->szChatString,pUserChat->dwChatColor);
	}
	else 
	{
		//��ʾ��Ϣ
		LPCTSTR pszSendUser=pISendUserItem->GetNickName();
		m_ChatMessage.InsertUserChat(pszSendUser,pUserChat->szChatString,pUserChat->dwChatColor);
	}

	return true;
}

//�û�����
bool CServerViewItem::OnSocketSubExpression(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_S_UserExpression));
	if (wDataSize!=sizeof(CMD_GR_S_UserExpression)) return false;

	//��������
	CMD_GR_S_UserExpression * pUserExpression=(CMD_GR_S_UserExpression *)pData;

	//��ȡ�û�
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserExpression->dwSendUserID);
	if (pISendUserItem==NULL) return true;

	//��Ϣ����
	if (pISendUserItem!=m_pIMySelfUserItem)
	{
		//���ű���
		DWORD dwUserGroupID=pISendUserItem->GetGroupID();
		DWORD dwMySelfGroupID=m_pIMySelfUserItem->GetGroupID();

		//�����ж�
		bool bSameGroup=false;
		if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

		//��������
		BYTE cbUserCompanion=pISendUserItem->GetUserCompanion();
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

		//����ģʽ
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//���ģʽ
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}
	}

	//������Ϣ
	if (pUserExpression->dwTargetUserID==0L)
	{
		//��������
		CExpressionManager * pExpressionManager=CExpressionManager::GetInstance();
		CExpression * pExpressionItem=pExpressionManager->GetExpressionItem(pUserExpression->wItemIndex);

		//�������
		if (pExpressionItem!=NULL)
		{
			//��ȡ·��
			TCHAR szImagePath[MAX_PATH]=TEXT("");
			pExpressionItem->GetExpressionPath(szImagePath,CountArray(szImagePath));

			//������Ϣ
			LPCTSTR pszSendUser=pISendUserItem->GetNickName();
			m_ChatMessage.InsertExpression(pszSendUser,szImagePath);
		}
	}

	//˽����Ϣ
	if (pUserExpression->dwTargetUserID!=0L)
	{
		//��������
		CExpressionManager * pExpressionManager=CExpressionManager::GetInstance();
		CExpression * pExpressionItem=pExpressionManager->GetExpressionItem(pUserExpression->wItemIndex);

		//�������
		if (pExpressionItem!=NULL)
		{
			//��ȡ·��
			TCHAR szImagePath[MAX_PATH]=TEXT("");
			pExpressionItem->GetExpressionPath(szImagePath,CountArray(szImagePath));

			//��ȡ�û�
			IClientUserItem * pIRecvUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserExpression->dwTargetUserID);
			if (pIRecvUserItem==NULL) return true;

			//��ʾ��Ϣ
			LPCTSTR pszSendUser=pISendUserItem->GetNickName();
			LPCTSTR pszRecvUser=pIRecvUserItem->GetNickName();
			m_ChatMessage.InsertExpression(pszSendUser,pszRecvUser,szImagePath);
		}
	}

	return true;
}

//�û�˽��
bool CServerViewItem::OnSocketSubWisperUserChat(VOID * pData, WORD wDataSize)
{
	//��������
	CMD_GR_S_WisperChat * pWisperChat=(CMD_GR_S_WisperChat *)pData;

	//Ч�����
	ASSERT(wDataSize>=(sizeof(CMD_GR_S_WisperChat)-sizeof(pWisperChat->szChatString)));
	ASSERT(wDataSize==(sizeof(CMD_GR_S_WisperChat)-sizeof(pWisperChat->szChatString)+pWisperChat->wChatLength*sizeof(pWisperChat->szChatString[0])));

	//Ч�����
	if (wDataSize<(sizeof(CMD_GR_S_WisperChat)-sizeof(pWisperChat->szChatString))) return false;
	if (wDataSize!=(sizeof(CMD_GR_S_WisperChat)-sizeof(pWisperChat->szChatString)+pWisperChat->wChatLength*sizeof(pWisperChat->szChatString[0]))) return false;

	//��ȡ�û�
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pWisperChat->dwSendUserID);
	if (pISendUserItem==NULL) return true;

	//�����û�
	IClientUserItem * pIRecvUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pWisperChat->dwTargetUserID);
	if (pIRecvUserItem==NULL) return true;

	//��Ϣ����
	if (pISendUserItem!=m_pIMySelfUserItem)
	{
		//���ű���
		DWORD dwUserGroupID=pISendUserItem->GetGroupID();
		DWORD dwMySelfGroupID=m_pIMySelfUserItem->GetGroupID();

		//�����ж�
		bool bSameGroup=false;
		if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

		//��������
		BYTE cbUserCompanion=pISendUserItem->GetUserCompanion();
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

		//����ģʽ
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//���ģʽ
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}
	}

	//��������
	DWORD dwUserID[2]={0L,0L};
	dwUserID[0]=m_pIMySelfUserItem->GetUserID();
	dwUserID[1]=(pISendUserItem==m_pIMySelfUserItem)?pIRecvUserItem->GetUserID():pISendUserItem->GetUserID();
	CDlgWhisper * pDlgWhisper=CreateWhisperWnd(0L,dwUserID,CountArray(dwUserID),(pISendUserItem!=m_pIMySelfUserItem));

	//��ʾ����
	if (pDlgWhisper!=NULL)
	{
		pDlgWhisper->OnEventWisperChat(pISendUserItem,pWisperChat->szChatString,pWisperChat->dwChatColor);
	}

	return true;
}

//˽�ı���
bool CServerViewItem::OnSocketSubWisperExpression(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_S_WisperExpression));
	if (wDataSize!=sizeof(CMD_GR_S_WisperExpression)) return false;

	//��������
	CMD_GR_S_WisperExpression * pWisperExpression=(CMD_GR_S_WisperExpression *)pData;

	//��ȡ�û�
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pWisperExpression->dwSendUserID);
	if (pISendUserItem==NULL) return true;

	//�����û�
	IClientUserItem * pIRecvUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pWisperExpression->dwTargetUserID);
	if (pIRecvUserItem==NULL) return true;

	//��Ϣ����
	if (pISendUserItem!=m_pIMySelfUserItem)
	{
		//���ű���
		DWORD dwUserGroupID=pISendUserItem->GetGroupID();
		DWORD dwMySelfGroupID=m_pIMySelfUserItem->GetGroupID();

		//�����ж�
		bool bSameGroup=false;
		if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

		//��������
		BYTE cbUserCompanion=pISendUserItem->GetUserCompanion();
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

		//����ģʽ
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//���ģʽ
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}
	}

	//��������
	DWORD dwUserID[2]={0L,0L};
	dwUserID[0]=m_pIMySelfUserItem->GetUserID();
	dwUserID[1]=(pISendUserItem==m_pIMySelfUserItem)?pIRecvUserItem->GetUserID():pISendUserItem->GetUserID();
	CDlgWhisper * pDlgWhisper=CreateWhisperWnd(0L,dwUserID,CountArray(dwUserID),(pISendUserItem!=m_pIMySelfUserItem));

	//��ʾ����
	if (pDlgWhisper!=NULL)
	{
		pDlgWhisper->OnEventExpression(pISendUserItem,pWisperExpression->wItemIndex);
	}

	return true;
}

//ϵͳ��Ϣ
bool CServerViewItem::OnSocketSubSystemMessage(VOID * pData, WORD wDataSize)
{
	//��������
	CMD_CM_SystemMessage * pSystemMessage=(CMD_CM_SystemMessage *)pData;
	WORD wHeadSize=sizeof(CMD_CM_SystemMessage)-sizeof(pSystemMessage->szString);

	//Ч�����
	ASSERT((wDataSize>wHeadSize)&&(wDataSize==(wHeadSize+pSystemMessage->wLength*sizeof(TCHAR))));
	if ((wDataSize<=wHeadSize)||(wDataSize!=(wHeadSize+pSystemMessage->wLength*sizeof(TCHAR)))) return false;

	//�رմ���
	if ((pSystemMessage->wType&(SMT_CLOSE_ROOM|SMT_CLOSE_LINK))!=0)
	{
		//�ر���ʾ
		m_DlgStatus.HideStatusWindow();

		//�رմ���
		m_TCPSocketModule->CloseSocket();
		m_ProcessManager.CloseGameProcess(false);

		//״̬����
		m_ServiceStatus=ServiceStatus_NetworkDown;
	}

	//��ʾ��Ϣ
	if (pSystemMessage->wType&SMT_CHAT) 
	{
		m_ChatMessage.InsertSystemString(pSystemMessage->szString);
	}

	//������Ϣ
	if (pSystemMessage->wType&SMT_EJECT)
	{
		CInformation Information;
		Information.ShowMessageBox(pSystemMessage->szString,MB_ICONINFORMATION);
	}

	//�رշ���
	if (pSystemMessage->wType&SMT_CLOSE_ROOM) PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return true;
}

//������Ϣ
bool CServerViewItem::OnSocketSubActionMessage(VOID * pData, WORD wDataSize)
{
	//��������
	CMD_CM_ActionMessage * pActionMessage=(CMD_CM_ActionMessage *)pData;
	WORD wHeadSize=sizeof(CMD_CM_ActionMessage)-sizeof(pActionMessage->szString);

	//Ч�����
	ASSERT((wDataSize>wHeadSize)&&(wDataSize>=(wHeadSize+pActionMessage->wLength*sizeof(WCHAR))));
	if ((wDataSize<=wHeadSize)||(wDataSize<(wHeadSize+pActionMessage->wLength*sizeof(WCHAR)))) return false;

	//�رմ���
	if ((pActionMessage->wType&(SMT_CLOSE_ROOM|SMT_CLOSE_LINK))!=0)
	{
		//�ر���ʾ
		m_DlgStatus.HideStatusWindow();

		//�رմ���
		m_TCPSocketModule->CloseSocket();
		m_ProcessManager.CloseGameProcess(false);

		//״̬����
		m_ServiceStatus=ServiceStatus_NetworkDown;
	}

	//������Ϣ
	CInformation Information;
	INT nResultCode=Information.ShowMessageBox(pActionMessage->szString,MB_ICONINFORMATION);

	//��������
	WORD wExcursion=wHeadSize+pActionMessage->wLength*sizeof(TCHAR);

	//��ȡ����
	while (wExcursion<wDataSize)
	{
		//��������
		tagActionHead * pActionHead=(tagActionHead *)((BYTE *)pData+wExcursion);

		//Ч�����
		ASSERT((wExcursion+sizeof(tagActionHead))<=wDataSize);
		ASSERT((wExcursion+sizeof(tagActionHead)+pActionHead->wAppendSize)<=wDataSize);

		//Ч�����
		if ((wExcursion+sizeof(tagActionHead))>wDataSize) return false;
		if ((wExcursion+sizeof(tagActionHead)+pActionHead->wAppendSize)>wDataSize) return false;

		//��������
		if (nResultCode==pActionHead->uResponseID)
		{
			switch (pActionHead->cbActionType)
			{
			case ACT_BROWSE:	//�������
				{
					//��������
					WORD wDataPos=wExcursion+sizeof(tagActionHead);
					tagActionBrowse * pActionBrowse=(tagActionBrowse *)((BYTE *)pData+wDataPos);

					//I E ���
					if (pActionBrowse->cbBrowseType&BRT_IE)
					{
						ShellExecute(NULL,TEXT("OPEN"),pActionBrowse->szBrowseUrl,NULL,NULL,SW_NORMAL);
					}

					//�������
					if (pActionBrowse->cbBrowseType&BRT_PLAZA)
					{
						CPlatformFrame::GetInstance()->WebBrowse(pActionBrowse->szBrowseUrl,true);
					}

					break;
				}
			case ACT_DOWN_LOAD:	//���ض���
				{
					//��������
					WORD wDataPos=wExcursion+sizeof(tagActionHead);
					tagActionDownLoad * pActionDownLoad=(tagActionDownLoad *)((BYTE *)pData+wDataPos);

					//I E ����
					if (pActionDownLoad->cbDownLoadMode&DLT_IE)
					{
						ShellExecute(NULL,TEXT("OPEN"),pActionDownLoad->szDownLoadUrl,NULL,NULL,SW_NORMAL);
					}

					//����ģ��
					if (pActionDownLoad->cbDownLoadMode&DLT_MODULE)
					{
						ShellExecute(NULL,TEXT("OPEN"),pActionDownLoad->szDownLoadUrl,NULL,NULL,SW_NORMAL);
					}

					break;
				}
			}
		}

		//����ƫ��
		wExcursion+=(sizeof(tagActionHead)+pActionHead->wAppendSize);
	}

	//�رշ���
	if (pActionMessage->wType&SMT_CLOSE_ROOM) PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return true;
}

//�����ؼ�
VOID CServerViewItem::RectifyControl(INT nWidth, INT nHeight)
{
	//״̬�ж�
	if (m_bCreateFlag==false) return;
	if ((nWidth==0)||(nHeight==0)) return;

	//��Ļλ��
	CRect rcMainWnd;
	AfxGetMainWnd()->GetClientRect(&rcMainWnd);

	//��ȡλ��
	CRect rcVorSplitter;
	CRect rcHorSplitter;
	m_VorSplitter.GetWindowRect(&rcVorSplitter);
	m_HorSplitter.GetWindowRect(&rcHorSplitter);

	//λ��ת��
	ScreenToClient(&rcVorSplitter);
	ScreenToClient(&rcHorSplitter);

	//����Χ
	INT nVorMinPos=rcMainWnd.Width()/2;
	INT nVorMaxPos=nWidth-rcMainWnd.Width()*6/25;
	m_VorSplitter.SetSplitterRange(nVorMinPos,nVorMaxPos);

	//����Χ
	INT nHorMinPos=nHeight*3/10;
	INT nHorMaxPos=nHeight*6/10;
	m_HorSplitter.SetSplitterRange(nHorMinPos,nHorMaxPos);

	//������
	if (m_bHideUserList==false)
	{
		if (m_bRectifyVor==false)
		{
			m_bRectifyVor=true;
			rcVorSplitter.top=0;
			rcVorSplitter.bottom=nHeight;
			rcVorSplitter.left=nWidth-rcMainWnd.Width()*6/25;
			rcVorSplitter.right=rcVorSplitter.left+SPLITTER_CX;
		}
	}
	else
	{
		rcVorSplitter.top=0;
		rcVorSplitter.bottom=nHeight;
		rcVorSplitter.left=nWidth-SPLITTER_CX;
		rcVorSplitter.right=rcVorSplitter.left+SPLITTER_CX;
	}

	//������
	if (m_bRectifyHor==false)
	{
		m_bRectifyHor=true;
		rcHorSplitter.top=nHeight*INCISE_SCALE-PROPERTY_CY;
		rcHorSplitter.bottom=rcHorSplitter.top+SPLITTER_CY;
	}

	//����λ��
	if ((nWidth!=0)&&(nHeight!=0))
	{
		//������
		if (m_bHideUserList==false)
		{
			if (rcVorSplitter.left<nVorMinPos)
			{
				rcVorSplitter.left=nVorMinPos;
				rcVorSplitter.right=rcVorSplitter.left+SPLITTER_CX;
			}
			if (rcVorSplitter.right>nVorMaxPos)
			{
				rcVorSplitter.right=nVorMaxPos;
				rcVorSplitter.left=rcVorSplitter.right-SPLITTER_CX;
			}
		}

		//������
		if (rcHorSplitter.top<nHorMinPos)
		{
			rcHorSplitter.top=nHorMinPos;
			rcHorSplitter.bottom=rcHorSplitter.top+SPLITTER_CY;
		}
		if (rcHorSplitter.bottom>nHorMaxPos)
		{
			rcHorSplitter.bottom=nHorMaxPos;
			rcHorSplitter.top=rcHorSplitter.bottom-SPLITTER_CY;
		}
	}

	//��������
	CRect rcTableArea;
	rcTableArea.top=0;
	rcTableArea.left=0;
	rcTableArea.bottom=nHeight;
	rcTableArea.right=rcVorSplitter.left;

	//�б�����
	CRect rcListArea;
	rcListArea.right=nWidth;
	rcListArea.top=PUBLICIZE_CY;
	rcListArea.left=rcVorSplitter.right;
	rcListArea.bottom=rcHorSplitter.top;

	//��������
	CRect rcChatArea;
	rcChatArea.right=nWidth;
	rcChatArea.bottom=nHeight;
	rcChatArea.left=rcVorSplitter.right;
	rcChatArea.top=rcHorSplitter.bottom+PROPERTY_CY;

	//������Ϣ
	tagEncircleInfo EncircleInfoChat;
	m_ChatEncircle.GetEncircleInfo(EncircleInfoChat);

	//�ƶ�׼��
	HDWP hDwp=BeginDeferWindowPos(32);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;

	//�������
	DeferWindowPos(hDwp,m_VorSplitter,NULL,rcVorSplitter.left,rcVorSplitter.top,rcVorSplitter.Width(),rcVorSplitter.Height(),uFlags);
	DeferWindowPos(hDwp,m_HorSplitter,NULL,rcVorSplitter.right,rcHorSplitter.top,nWidth-rcVorSplitter.right,rcHorSplitter.Height(),uFlags);

	//���Ӱ�ť
	INT nXPosTable=rcTableArea.right-27;
	CSkinButton * TableButtonArray[]={&m_btTableButton5,&m_btTableButton4,&m_btTableButton3,&m_btTableButton2,&m_btTableButton1};

	//�رհ�ť
	CRect rcQuitServer;
	m_btQuitServer.GetWindowRect(&rcQuitServer);
	DeferWindowPos(hDwp,m_btQuitServer,NULL,rcTableArea.right-rcQuitServer.Width()-4,2,0,0,uFlags|SWP_NOSIZE);

	//���Ӱ�ť
	for (INT i=0;i<CountArray(TableButtonArray);i++)
	{
		//��ȡλ��
		CRect rcTableButton;
		TableButtonArray[i]->GetWindowRect(&rcTableButton);

		//�ƶ���ť
		nXPosTable-=(rcTableButton.Width()+TABLE_BUTTON_SX);
		DeferWindowPos(hDwp,TableButtonArray[i]->m_hWnd,NULL,nXPosTable,2,0,0,uFlags|SWP_NOSIZE);
	}

	//���찴ť
	INT nChatButtonSpace=3;
	INT nXPosChat=rcChatArea.left+3;
	INT nYPosChat=rcChatArea.bottom-50;
	CSkinButton * ChatButtonArray[]={&m_btChatButton1,&m_btChatButton2,&m_btChatButton3,&m_btChatButton4};

	//����ؼ�
	UINT uTargetFlag=(m_bHideUserList==false)?SWP_SHOWWINDOW:SWP_HIDEWINDOW;
	DeferWindowPos(hDwp,m_btSendChat,NULL,rcChatArea.right-35,rcChatArea.bottom-22,0,0,uTargetFlag|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btChatShort,NULL,rcChatArea.right-53,rcChatArea.bottom-22,0,0,uTargetFlag|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_ChatEdit,NULL,rcChatArea.left+5,rcChatArea.bottom-19,rcChatArea.Width()-59,12,uTargetFlag);

	//���찴ť
	for (INT i=0;i<CountArray(ChatButtonArray);i++)
	{
		//��ȡλ��
		CRect rcChatButton;
		ChatButtonArray[i]->GetWindowRect(&rcChatButton);

		//��ʾ�ж�
		bool bShowButton=(m_bHideUserList==false);
		if ((nXPosChat+rcChatButton.Width()+nChatButtonSpace)>(rcChatArea.right-7)) bShowButton=false;

		//�ƶ���ť
		UINT uTargetFlag=(bShowButton==true)?SWP_SHOWWINDOW:SWP_HIDEWINDOW;
		DeferWindowPos(hDwp,ChatButtonArray[i]->m_hWnd,NULL,nXPosChat,nYPosChat,0,0,uFlags|SWP_NOSIZE|uTargetFlag);

		//���ü��
		nXPosChat+=(rcChatButton.Width()+nChatButtonSpace);
	}

	//���ƿؼ�
	m_TableEncircle.DeferWindowPos(&m_TableViewFrame,hDwp,rcTableArea);
	m_UserListEncircle.DeferWindowPos(&m_UserListControl,hDwp,rcListArea);

	//�ƶ��ؼ�
	DeferWindowPos(hDwp,m_PlatformPublicize,NULL,rcListArea.left+1,0,rcListArea.Width()-6,PUBLICIZE_CY-3,uFlags);
	DeferWindowPos(hDwp,m_GamePropertyCtrl,NULL,rcListArea.left,rcHorSplitter.bottom,rcListArea.Width(),PROPERTY_CY,uFlags);

	//�������
	if ((m_ChatControl.m_hWnd!=NULL)&&(m_ChatControl.GetChatTargetUserItem()!=NULL))
	{
		//��ȡλ��
		CRect rcChatControl;
		m_ChatControl.GetWindowRect(&rcChatControl);

		//λ����Ϣ
		tagEncircleInfo EncircleInfo;
		m_ChatEncircle.GetEncircleInfo(EncircleInfo);

		//����ؼ�
		rcChatArea.bottom-=rcChatControl.Height();
		m_ChatEncircle.DeferWindowPos(&m_ChatMessage,hDwp,rcChatArea);

		//�������
		DeferWindowPos(hDwp,m_ChatControl,NULL,rcChatArea.left+EncircleInfo.nLBorder,rcChatArea.bottom-EncircleInfo.nBBorder,
			rcChatArea.Width()-EncircleInfo.nLBorder-EncircleInfo.nRBorder,rcChatControl.Height(),uFlags);
	}
	else
	{
		//����ؼ�
		m_ChatEncircle.DeferWindowPos(&m_ChatMessage,hDwp,rcChatArea);
	}

	//�ƶ�����
	EndDeferWindowPos(hDwp);

	//���ư�ť
	CRect rcControl;
	m_btControl.GetClientRect(&rcControl);
	m_btControl.SetWindowPos(NULL,(rcVorSplitter.Width()-rcControl.Width())/2,(rcVorSplitter.Height()-rcControl.Height())/2,0,0,uFlags|SWP_NOSIZE);

	//�ػ�����
	RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW);

	return;
}

//���÷���
VOID CServerViewItem::InitServerViewItem(CGameServerItem * pGameServerItem)
{
	//����״̬
	SetServiceStatus(ServiceStatus_Entering);

	//��������
	CGameKindItem * pGameKindItem=pGameServerItem->m_pGameKindItem;
	CopyMemory(&m_GameKind,&pGameKindItem->m_GameKind,sizeof(m_GameKind));
	CopyMemory(&m_GameServer,&pGameServerItem->m_GameServer,sizeof(m_GameServer));

	//�ر��ж�
	ASSERT(m_GameServer.wServerID!=0);
	if (m_GameServer.wServerID==0) throw TEXT("�ܱ�Ǹ������Ϸ�����Ѿ��ر��ˣ�������������룡");

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//�������
	if (m_TCPSocketModule.CreateInstance()==false) throw TEXT("�޷��������������������Ϸ�������ʧ�ܣ�");

	//��������
	m_pParameterGame=pParameterGlobal->GetParameterGame(&m_GameKind);
	m_pParameterServer=pParameterGlobal->GetParameterServer(&m_GameServer);

	//��������
	tagPropertyResource PropertyResource;
	ZeroMemory(&PropertyResource,sizeof(PropertyResource));

	//���õ���
	PropertyResource.uImageL=IDB_GAME_PROPERTY_L;
	PropertyResource.uImageM=IDB_GAME_PROPERTY_M;
	PropertyResource.uImageR=IDB_GAME_PROPERTY_R;
	PropertyResource.uImageItem=IDB_GAME_PROPERTY_ITEM;
	PropertyResource.hResInstance=AfxGetInstanceHandle();

	//����λ��
	PropertyResource.PointExcursion.x=4;
	PropertyResource.PointExcursion.y=10;
	PropertyResource.SizeExcursion.cx=38;
	PropertyResource.SizeExcursion.cy=39;

	//���õ���
	m_GamePropertyCtrl.SetPropertyResource(PropertyResource);
	m_GamePropertyCtrl.SetGamePropertySink(QUERY_ME_INTERFACE(IUnknownEx));

	//��������
	m_TCPSocketModule->SetTCPSocketSink(QUERY_ME_INTERFACE(IUnknownEx));
	m_TCPSocketModule->SetProxyServerInfo(pParameterGlobal->m_cbProxyType,pParameterGlobal->m_ProxyServer);

	//��������
	if (m_TCPSocketModule->Connect(m_GameServer.szServerAddr,m_GameServer.wServerPort)!=CONNECT_SUCCESS)
	{
		throw TEXT("��Ϸ��������ʧ�ܣ�����ϵͳ�������ò����Ƿ���ȷ��");
		return;
	}

	//������ʾ
	m_DlgStatus.ShowStatusWindow(TEXT("���ڽ�����Ϸ���䣬���Ժ�..."));

	//�������
	TCHAR szServerTitle[128]=TEXT("");
	_sntprintf(szServerTitle,CountArray(szServerTitle),TEXT("[ %s ] - %s"),m_GameKind.szKindName,m_GameServer.szServerName);

	//���ñ���
	SetWindowText(szServerTitle);

	return;
}

//���ý���
VOID CServerViewItem::InitServerViewCtrl(CSkinRenderManager * pSkinRenderManager)
{
	//�������
	tagColorUserList ColorUserList;
	ZeroMemory(&ColorUserList,sizeof(ColorUserList));

	//��ɫ����
	ColorUserList.crSelectTX=COLOR_SELECT_TX;
	ColorUserList.crSelectBK=COLOR_SELECT_BK;
	ColorUserList.crNormalTX=COLOR_NORMAL_TX;
	ColorUserList.crNormalBK=COLOR_NORMAL_BK;

	//��ɫ����
	ColorUserList.crMyselfTX=COLOR_MYSELF_TX;
	ColorUserList.crMyselfBK=COLOR_MYSELF_BK;
	ColorUserList.crMasterTX=COLOR_MASTER_TX;
	ColorUserList.crMasterBK=COLOR_MASTER_BK;
	ColorUserList.crMemberTX=COLOR_MEMBER_TX;
	ColorUserList.crMemberBK=COLOR_MEMBER_BK;

	//���ÿؼ�
	m_UserListControl.SetColorUserList(ColorUserList);

	return;
}

//˽�ĻỰ
VOID CServerViewItem::WhisperConversation(IClientUserItem * pIClientUserItem[], WORD wUserCount)
{
	//״̬����
	if (m_ServiceStatus!=ServiceStatus_ServiceIng)
	{
		return;
	}

	//�û�����
	WORD wTableID=m_pIMySelfUserItem->GetTableID();
	BYTE cbMasterOrder=m_pIMySelfUserItem->GetMasterOrder();

	//��������
	bool bSameTable=false;
	bool bMasterUser=(cbMasterOrder>0);

	//�����ж�
	if (bMasterUser==false)
	{
		for (WORD i=0;i<wUserCount;i++)
		{
			if (pIClientUserItem[i]->GetMasterOrder()>0)
			{
				bMasterUser=true;
				break;
			}
		}
	}

	//ͬ���ж�
	if (wTableID!=INVALID_TABLE)
	{
		for (WORD i=0;i<wUserCount;i++)
		{
			if (pIClientUserItem[i]->GetTableID()==wTableID)
			{
				bSameTable=true;
				break;
			}
		}
	}

	//Ȩ���ж�
	if (CUserRight::CanWisper(m_dwUserRight)==false)
	{
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("�ܱ�Ǹ����û�з���˽����Ϣ��Ȩ�ޣ�"),MB_ICONQUESTION);
		return;
	}

	//˽��Ȩ��
	if (CServerRule::IsForfendWisperChat(m_dwServerRule)&&(bMasterUser==false))
	{
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("�ܱ�Ǹ������Ϸ���䲻�������˽�ģ�"),MB_ICONQUESTION);
		return;
	}

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��������
	CWHArray<DWORD> CUserIDArray;
	CUserIDArray.Add(pGlobalUserData->dwUserID);

	//�����û�
	for (WORD i=0;i<wUserCount;i++)
	{
		//�û�����
		if (pIClientUserItem[i]->GetUserID()==0L) continue;
		if (pIClientUserItem[i]==m_pIMySelfUserItem) continue;

		//��������
		CUserIDArray.Add(pIClientUserItem[i]->GetUserID());
	}

	//����˽��
	WORD wUserIDArray=(WORD)CUserIDArray.GetCount();
	CDlgWhisper * pDlgWhisper=CreateWhisperWnd(0L,CUserIDArray.GetData(),wUserIDArray,true);

	//��ʾ����
	if (pDlgWhisper!=NULL)
	{
		//��ʾ����
		pDlgWhisper->ShowWindow(SW_RESTORE);

		//�ö�����
		pDlgWhisper->SetActiveWindow();
		pDlgWhisper->BringWindowToTop();

		//ɾ������
		CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
		if (pPlatformFrame!=NULL) pPlatformFrame->RemoveWhisperItem(pDlgWhisper);
	}

	return;
}

//����˽��
CDlgWhisper * CServerViewItem::CreateWhisperWnd(DWORD dwConversationID, DWORD dwUserID[], WORD wUserCount, bool bCreate)
{
	//Ѱ���ִ�
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		//��ȡ����
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];
		if ((pDlgWhisper->m_hWnd==NULL)||(pDlgWhisper->CompareUserItem(dwUserID,wUserCount)==false)) continue;

		return pDlgWhisper;
	}

	//�����ж�
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		//��ȡ����
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];
		if ((pDlgWhisper->m_hWnd==NULL)||(pDlgWhisper->DisuseEstimate()==false)) continue;

		//��������
		CClientUserItemArray ClientUserItemArray;
		ClientUserItemArray.Add(m_pIMySelfUserItem);

		//�������
		for (WORD i=0;i<wUserCount;i++)
		{
			IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(dwUserID[i]);
			if ((pIClientUserItem!=NULL)&&(pIClientUserItem!=m_pIMySelfUserItem)) ClientUserItemArray.Add(pIClientUserItem);
		}

		//��������
		ASSERT(ClientUserItemArray.GetCount()==wUserCount);
		WORD wUserItemCount=(WORD)ClientUserItemArray.GetCount();
		pDlgWhisper->CreateWhisperWnd(dwConversationID,ClientUserItemArray.GetData(),wUserItemCount,GetDesktopWindow());

		return pDlgWhisper;
	}

	//��������
	if (bCreate==true)
	{
		//��������
		CDlgWhisper * pDlgWhisper=NULL;
		CClientUserItemArray ClientUserItemArray;
		ClientUserItemArray.Add(m_pIMySelfUserItem);

		//Ѱ�ҹر�
		for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
		{
			//��ȡ����
			CDlgWhisper * pDlgWhisperTemp=m_DlgWhisperItemArray[i];
			if (pDlgWhisperTemp->GetSafeHwnd()==NULL) 
			{
				pDlgWhisper=pDlgWhisperTemp;
				break;
			}
		}

		//��������
		if (pDlgWhisper==NULL)
		{
			try
			{
				pDlgWhisper=new CDlgWhisper;
				if (pDlgWhisper==NULL) return NULL;
				m_DlgWhisperItemArray.Add(pDlgWhisper);
			}
			catch (...) { return NULL; }
		}

		//�������
		for (WORD i=0;i<wUserCount;i++)
		{
			IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(dwUserID[i]);
			if ((pIClientUserItem!=NULL)&&(pIClientUserItem!=m_pIMySelfUserItem)) ClientUserItemArray.Add(pIClientUserItem);
		}

		//�û���Ŀ
		ASSERT(ClientUserItemArray.GetCount()==wUserCount);
		WORD wUserItemCount=(WORD)ClientUserItemArray.GetCount();

		//���ô���
		pDlgWhisper->SetServerViewItem(this);
		pDlgWhisper->SetMySelfUserItem(m_pIMySelfUserItem);
		pDlgWhisper->SetTCPSocket(m_TCPSocketModule.GetInterface());
		pDlgWhisper->SetGameLevelParser(m_GameLevelParserModule.GetInterface());
		pDlgWhisper->SetUserOrderParser(theApp.m_UserOrderParserModule.GetInterface());

		//��������
		pDlgWhisper->CreateWhisperWnd(dwConversationID,ClientUserItemArray.GetData(),wUserItemCount,GetDesktopWindow());

		return pDlgWhisper;
	}

	return NULL;
}

//���͵�¼
bool CServerViewItem::SendLogonPacket()
{
	//��������
	CMD_GR_LogonUserID LogonUserID;
	ZeroMemory(&LogonUserID,sizeof(LogonUserID));

	//��������
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��Ϸ�汾
	LPCTSTR pszProcessName=m_GameKind.szProcessName;
	CWHService::GetModuleVersion(pszProcessName,LogonUserID.dwProcessVersion);

	//������Ϣ
	LogonUserID.dwPlazaVersion=pGlobalUnits->GetPlazaVersion();
	LogonUserID.dwFrameVersion=pGlobalUnits->GetFrameVersion();

	//��¼��Ϣ
	LogonUserID.dwUserID=pGlobalUserData->dwUserID;
	lstrcpyn(LogonUserID.szPassword,pGlobalUserData->szPassword,CountArray(LogonUserID.szPassword));

	//������ʶ
	CWHService::GetMachineID(LogonUserID.szMachineID);

	//��������
	m_TCPSocketModule->SendData(MDM_GR_LOGON,SUB_GR_LOGON_USERID,&LogonUserID,sizeof(LogonUserID));

	return true;
}

//��������
bool CServerViewItem::SendUserRulePacket()
{
	//��������
	BYTE cbBuffer[SOCKET_TCP_PACKET];
	CMD_GR_UserRule * pUserRule=(CMD_GR_UserRule *)cbBuffer;
	CSendPacketHelper SendPacket(cbBuffer+sizeof(CMD_GR_UserRule),sizeof(cbBuffer)-sizeof(CMD_GR_UserRule));

	//��������
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//��������
	pUserRule->cbRuleMask=0x00;

	//���Ƶ�ַ
	if (pParameterGlobal->m_bLimitSameIP==true)
	{
		pUserRule->cbRuleMask|=UR_LIMIT_SAME_IP;
	}

	//����ʤ��
	if (m_pParameterGame->m_bLimitWinRate==true)
	{
		pUserRule->cbRuleMask|=UR_LIMIT_WIN_RATE;
		pUserRule->wMinWinRate=m_pParameterGame->m_wMinWinRate;
	}

	//��������
	if (m_pParameterGame->m_bLimitFleeRate==true)
	{
		pUserRule->cbRuleMask|=UR_LIMIT_FLEE_RATE;
		pUserRule->wMaxFleeRate=m_pParameterGame->m_wMaxFleeRate;
	}

	//���ƻ���
	if (m_pParameterGame->m_bLimitGameScore==true)
	{
		pUserRule->cbRuleMask|=UR_LIMIT_GAME_SCORE;
		pUserRule->lMaxGameScore=m_pParameterGame->m_lMaxGameScore;
		pUserRule->lMinGameScore=m_pParameterGame->m_lMinGameScore;
	}

	//Я������
	if ((CServerRule::IsForfendLockTable(m_dwServerRule)==false)&&(m_pParameterServer->m_bTakePassword==true))
	{
		SendPacket.AddPacket(m_pParameterServer->m_szPassword,DTP_GR_TABLE_PASSWORD);
	}

	//��������
	WORD wSendSize=sizeof(CMD_GR_UserRule)+SendPacket.GetDataSize();
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_RULE,cbBuffer,wSendSize);

	return true;
}

//�����Թ�
bool CServerViewItem::SendLookonPacket(WORD wTableID, WORD wChairID)
{
	//��������
	CMD_GR_UserLookon UserLookon;
	ZeroMemory(&UserLookon,sizeof(UserLookon));

	//��������
	UserLookon.wTableID=wTableID;
	UserLookon.wChairID=wChairID;

	//��������
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_LOOKON,&UserLookon,sizeof(UserLookon));

	return true;
}

//��������
bool CServerViewItem::SendSitDownPacket(WORD wTableID, WORD wChairID)
{
	//��������
	CMD_GR_UserSitDown UserSitDown;
	ZeroMemory(&UserSitDown,sizeof(UserSitDown));

	//��������
	UserSitDown.wTableID=wTableID;
	UserSitDown.wChairID=wChairID;

	//��������
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_SITDOWN,&UserSitDown,sizeof(UserSitDown));

	return true;
}

//��������
bool CServerViewItem::SendStandUpPacket(WORD wTableID, WORD wChairID, BYTE cbForceLeave)
{
	//��������
	CMD_GR_UserStandUp UserStandUp;
	ZeroMemory(&UserStandUp,sizeof(UserStandUp));

	//��������
	UserStandUp.wTableID=wTableID;
	UserStandUp.wChairID=wChairID;
	UserStandUp.cbForceLeave=cbForceLeave;

	//��������
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_STANDUP,&UserStandUp,sizeof(UserStandUp));

	return true;
}

//��������
bool CServerViewItem::SendUserChatPacket(DWORD dwTargetUserID, LPCTSTR pszChatString, COLORREF crColor)
{
	//������Ϣ
	CMD_GR_C_UserChat UserChat;
	lstrcpyn(UserChat.szChatString,pszChatString,CountArray(UserChat.szChatString));

	//��������
	UserChat.dwChatColor=crColor;
	UserChat.dwTargetUserID=dwTargetUserID;
	UserChat.wChatLength=CountStringBuffer(UserChat.szChatString);

	//��������
	WORD wHeadSize=sizeof(UserChat)-sizeof(UserChat.szChatString);
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_CHAT,&UserChat,wHeadSize+UserChat.wChatLength*sizeof(UserChat.szChatString[0]));

	return true;
}

//���ͱ���
bool CServerViewItem::SendExpressionPacket(DWORD dwTargetUserID, WORD wItemIndex)
{
	//��������
	CMD_GR_C_UserExpression UserExpression;
	ZeroMemory(&UserExpression,sizeof(UserExpression));

	//������Ϣ
	UserExpression.wItemIndex=wItemIndex;
	UserExpression.dwTargetUserID=dwTargetUserID;

	//��������
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_EXPRESSION,&UserExpression,sizeof(UserExpression));

	return true;
}

//ִ���Թ�
bool CServerViewItem::PerformLookonAction(WORD wTableID, WORD wChairID)
{
	//Ч������
	ASSERT(wTableID!=INVALID_TABLE);
	ASSERT(wChairID!=INVALID_CHAIR);

	//״̬����
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return false;
	if ((m_wReqTableID==wTableID)&&(m_wReqChairID==wChairID)) return false;

	//�Լ�״̬
	if (m_pIMySelfUserItem->GetUserStatus()>=US_PLAYING)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("��������Ϸ�У���ʱ�����뿪�����Ƚ�����ǰ��Ϸ��"));

		return false;
	}

	//Ȩ���ж�
	if (CUserRight::CanLookon(m_dwUserRight)==false)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("��Ǹ������ʱû���Թ���Ϸ��Ȩ�ޣ�"));

		return false;
	}

	//�������
	if ((m_wReqTableID!=INVALID_TABLE)&&(m_wReqChairID!=INVALID_CHAIR))
	{
		IClientUserItem * pIClientUserItem=m_TableViewFrame.GetClientUserItem(m_wReqTableID,m_wReqChairID);
		if (pIClientUserItem==m_pIMySelfUserItem) m_TableViewFrame.SetClientUserItem(m_wReqTableID,m_wReqChairID,NULL);
	}

	//���ñ���
	m_wReqTableID=wTableID;
	m_wReqChairID=wChairID;
	m_wFindTableID=INVALID_TABLE;

	//���ý���
	m_TableViewFrame.VisibleTable(wTableID);

	//��������
	SendLookonPacket(wTableID,wChairID);

	return true;
}

//ִ������
bool CServerViewItem::PerformStandUpAction(WORD wTableID, WORD wChairID)
{
	//Ч������
	ASSERT(wTableID!=INVALID_TABLE);
	ASSERT(wChairID!=INVALID_CHAIR);

	//״̬����
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return false;
	if ((m_wReqTableID==wTableID)&&(m_wReqChairID==wChairID)) return false;

	//�Լ�״̬
	if (m_pIMySelfUserItem->GetUserStatus()>=US_PLAYING)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("��������Ϸ�У���ʱ�����뿪�����Ƚ�����ǰ��Ϸ��"));

		return false;
	}

	//���ñ���
	m_wReqTableID=INVALID_TABLE;
	m_wReqChairID=INVALID_CHAIR;
	m_wFindTableID=INVALID_TABLE;

	//���ý���
	m_TableViewFrame.VisibleTable(wTableID);

	//��������
	SendStandUpPacket(wTableID,wChairID,FALSE);

	return true;
}

//ִ������
bool CServerViewItem::PerformSitDownAction(WORD wTableID, WORD wChairID, bool bEfficacyPass)
{
	//״̬����
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return false;
	if ((m_wReqTableID!=INVALID_TABLE)&&(m_wReqTableID==wTableID)) return false;
	if ((m_wReqChairID!=INVALID_CHAIR)&&(m_wReqChairID==wChairID)) return false;

	//�Լ�״̬
	if (m_pIMySelfUserItem->GetUserStatus()>=US_PLAYING)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("��������Ϸ�У���ʱ�����뿪�����Ƚ�����ǰ��Ϸ��"));

		return false;
	}

	//Ȩ���ж�
	if (CUserRight::CanPlay(m_dwUserRight)==false)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("��Ǹ������ʱû�м�����Ϸ��Ȩ�ޣ�"));

		return false;
	}

	//����Ч��
	if ((wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR))
	{
		CString strDescribe;
		if ((wChairID!=INDEX_ENTER_CHAIR)&&(EfficacyTableRule(wTableID,wChairID,false,strDescribe)==false))
		{
			//��ʾ��Ϣ
			CInformation Information(this);
			Information.ShowMessageBox(m_GameServer.szServerName,strDescribe,MB_ICONINFORMATION,30L);

			return false;
		}
	}

	//�����ж�
	if ((bEfficacyPass==true)&&(wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR)&&(wChairID!=INDEX_ENTER_CHAIR))
	{
		//��������
		bool bLocker=m_TableViewFrame.GetLockerFlag(wTableID);
		bool bTakePassword=m_pParameterServer->m_bTakePassword;

		//�����ж�
		if ((bLocker==true)&&(bTakePassword==false))
		{
			//��������
			CDlgTablePassword DlgTablePassword;
			if (DlgTablePassword.DoModal()!=IDOK) return false;

			//��������
			m_pParameterServer->m_bTakePassword=true;
			lstrcpyn(m_pParameterServer->m_szPassword,DlgTablePassword.m_szPassword,CountArray(m_pParameterServer->m_szPassword));

			//���͹���
			SendUserRulePacket();
		}
	}

	//�������
	if ((m_wReqTableID!=INVALID_TABLE)&&(m_wReqChairID!=INVALID_CHAIR)&&(m_wReqChairID!=INDEX_ENTER_CHAIR))
	{
		IClientUserItem * pIClientUserItem=m_TableViewFrame.GetClientUserItem(m_wReqTableID,m_wReqChairID);
		if (pIClientUserItem==m_pIMySelfUserItem) m_TableViewFrame.SetClientUserItem(m_wReqTableID,m_wReqChairID,NULL);
	}

	//���ý���
	if ((wChairID!=INDEX_ENTER_CHAIR)&&(wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR))
	{
		m_TableViewFrame.VisibleTable(wTableID);
		m_TableViewFrame.SetClientUserItem(wTableID,wChairID,m_pIMySelfUserItem);
	}

	//���ñ���
	m_wReqTableID=wTableID;
	m_wReqChairID=wChairID;
	m_wFindTableID=INVALID_TABLE;

	//��������
	SendSitDownPacket(wTableID,wChairID);

	return true;
}

//����״̬
VOID CServerViewItem::SetServiceStatus(enServiceStatus ServiceStatus)
{
	//���ñ���
	m_ServiceStatus=ServiceStatus;

	return;
}

//��ԴĿ¼
VOID CServerViewItem::GetGameResDirectory(TCHAR szResDirectory[], WORD wBufferCount)
{
	//��������
	WORD wStringIndex=0;

	//����Ŀ¼
	ZeroMemory(szResDirectory,wBufferCount*sizeof(TCHAR));
	lstrcpyn(szResDirectory,m_GameKind.szProcessName,wBufferCount);
	while ((szResDirectory[wStringIndex]!=0)&&(szResDirectory[wStringIndex]!=TEXT('.'))) wStringIndex++;

	//�ַ���ֹ
	szResDirectory[wStringIndex]=0;

	return;
}

//����Ч��
bool CServerViewItem::EfficacyUserChat(LPCTSTR pszChatString, WORD wExpressionIndex)
{
	//��������
	ASSERT(m_pIMySelfUserItem!=NULL);
	BYTE cbMemberOrder=m_pIMySelfUserItem->GetMemberOrder();
	BYTE cbMasterOrder=m_pIMySelfUserItem->GetMasterOrder();

	//��������
	if (CServerRule::IsForfendRoomChat(m_dwServerRule)&&(cbMasterOrder==0))
	{
		//ԭʼ��Ϣ
		if (wExpressionIndex==INVALID_WORD)
		{
			CString strDescribe;
			strDescribe.Format(TEXT("\n��%s������ʧ��"),pszChatString);
			m_ChatMessage.InsertString(strDescribe,COLOR_EVENT);
		}

		//������Ϣ
		m_ChatMessage.InsertSystemString(TEXT("��Ǹ����ǰ����Ϸ�����ֹ�û��������죡"));

		return false;
	}

	//Ȩ���ж�
	if (CUserRight::CanRoomChat(m_dwUserRight)==false)
	{
		//ԭʼ��Ϣ
		if (wExpressionIndex==INVALID_WORD)
		{
			CString strDescribe;
			strDescribe.Format(TEXT("\n��%s������ʧ��"),pszChatString);
			m_ChatMessage.InsertString(strDescribe,COLOR_EVENT);
		}

		//������Ϣ
		m_ChatMessage.InsertSystemString(TEXT("��Ǹ����û�д������Ե�Ȩ�ޣ�����Ҫ����������ϵ��Ϸ�ͷ���ѯ��"));

		return false;
	}

	//�ٶ��ж�
	DWORD dwCurrentTime=(DWORD)time(NULL);
	if ((cbMasterOrder==0)&&((dwCurrentTime-m_dwChatTime)<=TIME_USER_CHAT))
	{
		//ԭʼ��Ϣ
		if (wExpressionIndex==INVALID_WORD)
		{
			CString strDescribe;
			strDescribe.Format(TEXT("\n��%s������ʧ��"),pszChatString);
			m_ChatMessage.InsertString(strDescribe,COLOR_EVENT);
		}

		//������Ϣ
		m_ChatMessage.InsertSystemString(TEXT("����˵���ٶ�̫���ˣ����������ȱ�����Ϣ�°ɣ�"));

		return false;
	}

	//���ñ���
	m_dwChatTime=dwCurrentTime;
	m_strChatString=pszChatString;

	return true;
}

//����Ч��
bool CServerViewItem::EfficacyTableRule(WORD wTableID, WORD wChairID, bool bReqLookon, CString & strDescribe)
{
	//Ч�����
	ASSERT(wTableID<m_TableViewFrame.GetTableCount());
	ASSERT(wChairID<m_TableViewFrame.GetChairCount());

	//״̬����
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return false;
	if (wTableID>=m_TableViewFrame.GetTableCount()) return false;
	if (wChairID>=m_TableViewFrame.GetChairCount()) return false;

	//��������
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//��������
	ITableView * pITableView=m_TableViewFrame.GetTableViewItem(wTableID);
	IClientUserItem * pITableUserItem=pITableView->GetClientUserItem(wChairID);

	//��������
	bool bGameStart=pITableView->GetPlayFlag();
	bool bDynamicJoin=CServerRule::IsDynamicJoin(m_dwServerRule);

	//��Ϸ״̬
	if ((bGameStart==true)&&(bDynamicJoin==false)&&(bReqLookon==false))
	{
		strDescribe=TEXT("����Ϸ���Ѿ���ʼ��Ϸ�ˣ���ʱ���ܼ��룡");
		return false;
	}

	//�����ж�
	if ((bReqLookon==false)&&(m_pIMySelfUserItem->GetMasterOrder()==0))
	{
		//�����ж�
		for (WORD i=0;i<m_TableViewFrame.GetChairCount();i++)
		{
			//��ȡ�û�
			IClientUserItem * pITableUserItem=pITableView->GetClientUserItem(i);
			if ((pITableUserItem==NULL)||(pITableUserItem==m_pIMySelfUserItem)) continue;

			//������
			if (pParameterGlobal->m_bLimitDetest==true)
			{
				if (pITableUserItem->GetUserCompanion()==CP_DETEST)
				{
					//������ʾ
					LPCTSTR pszNickName=pITableUserItem->GetNickName();
					strDescribe.Format(TEXT("�������˲��벻�ܻ�ӭ�������Ϸ��������������ӭ����� [ %s ] ��"),pszNickName);

					return false;
				}
			}

			//ʤ��Ч��
			if (m_pParameterGame->m_bLimitWinRate==true)
			{
				if (((WORD)(pITableUserItem->GetUserWinRate()*1000L))<m_pParameterGame->m_wMinWinRate)
				{
					strDescribe.Format(TEXT("[ %s ] ��ʤ��̫���ˣ����������ò�����"),pITableUserItem->GetNickName());
					return false;
				}
			}

			//����Ч��
			if (m_pParameterGame->m_bLimitFleeRate)
			{
				if (((WORD)(pITableUserItem->GetUserFleeRate()*1000L))<m_pParameterGame->m_wMaxFleeRate)
				{
					strDescribe.Format(TEXT("[ %s ] ������̫���ˣ����������ò�����"),pITableUserItem->GetNickName());
					return false;
				}
			}

			//����Ч��
			if (m_pParameterGame->m_bLimitGameScore)
			{
				//��߻���
				if (pITableUserItem->GetUserScore()>m_pParameterGame->m_lMaxGameScore)
				{
					strDescribe.Format(TEXT("[ %s ] �Ļ���̫���ˣ����������ò�����"),pITableUserItem->GetNickName());
					return false;
				}

				//��ͻ���
				if (pITableUserItem->GetUserScore()<m_pParameterGame->m_lMinGameScore)
				{
					strDescribe.Format(TEXT("[ %s ] �Ļ���̫���ˣ����������ò�����"),pITableUserItem->GetNickName());
					return false;
				}
			}
		}
	}

	return true;
}

//������ɫ
VOID CServerViewItem::OnSelectMoreColor()
{
	//��������
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//������ɫ
	CColorDialog ColorDialog(pParameterGlobal->m_crChatTX,CC_FULLOPEN,this);
	if (ColorDialog.DoModal()==IDOK) pParameterGlobal->m_crChatTX=ColorDialog.GetColor();

	//���ý���
	m_ChatEdit.SetFocus();

	return;
}

//ѡ����ɫ
VOID CServerViewItem::OnSelectChatColor(UINT nCommandID)
{
	//��������
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//��������
	UINT nIndex=nCommandID-IDM_SELECT_CHAT_COLOR;
	COLORREF crColor[]={RGB(255,0,0),RGB(255,255,0),RGB(128,255,0),RGB(0,255,255),
		RGB(0,0,128),RGB(0,128,0),RGB(128,0,128),RGB(128,0,0),RGB(0,0,0)};

	//ѡ����ɫ
	ASSERT(nIndex<CountArray(crColor));
	if (nIndex<CountArray(crColor)) pParameterGlobal->m_crChatTX=crColor[nIndex];

	//���ý���
	m_ChatEdit.SetFocus();

	return;
}

//ѡ�����
VOID CServerViewItem::OnSelectChatShort(UINT nCommandID)
{
	//��������
	UINT nIndex=nCommandID-IDM_SELECT_CHAT_SHORT;

	//����Ŀ¼
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//����·��
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\PhraseInfo.ini"),szDirectory);

	//��������
	TCHAR szItemName[16]=TEXT("");
	TCHAR szShortcut[LEN_USER_CHAT]=TEXT("");

	//��ȡ��Ϣ
	_sntprintf(szItemName,CountArray(szItemName),TEXT("Phrase%d"),nIndex);
	GetPrivateProfileString(TEXT("ServerChatShort"),szItemName,TEXT(""),szShortcut,CountArray(szShortcut),szFileName);

	//������Ϣ
	if (szShortcut[0]!=0)
	{
		//��ȡ��Ϣ
		CString strMessage=szShortcut;
		strMessage.TrimLeft();strMessage.TrimRight();

		//������Ϣ
		if ((strMessage.IsEmpty()==FALSE)&&(EfficacyUserChat(strMessage,INVALID_WORD)==true))
		{
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
			SendUserChatPacket(m_ChatControl.GetChatTargetUserID(),strMessage,pParameterGlobal->m_crChatTX);
		}
	}

	//���ý���
	m_ChatEdit.SetFocus();

	return;
}

//������Ϣ
VOID CServerViewItem::OnNcDestroy()
{
	__super::OnNcDestroy();

	//ɾ������
	try
	{
		delete this;
	}
	catch (...)
	{
		ASSERT(FALSE);
	}

	return;
}

//�滭����
BOOL CServerViewItem::OnEraseBkgnd(CDC * pDC)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//���λ��
	CRect rcVorSplitter;
	CRect rcHorSplitter;
	m_VorSplitter.GetWindowRect(&rcVorSplitter);
	m_HorSplitter.GetWindowRect(&rcHorSplitter);

	//��ȡλ��
	ScreenToClient(&rcVorSplitter);
	ScreenToClient(&rcHorSplitter);

	//�滭����
	CRect rcTable;
	rcTable.top=0;
	rcTable.left=0;
	rcTable.right=rcVorSplitter.left;
	rcTable.bottom=rcClient.Height();
	m_TableEncircle.DrawEncircleFrame(pDC,rcTable);

	//�滭����
	if (m_bHideUserList==false)
	{
		CRect rcChat;
		rcChat.right=rcClient.Width();
		rcChat.bottom=rcClient.Height();
		rcChat.left=rcVorSplitter.right;
		rcChat.top=rcHorSplitter.bottom+PROPERTY_CY;
		m_ChatEncircle.DrawEncircleFrame(pDC,rcChat);
	}

	//�û��б�
	if (m_bHideUserList==false)
	{
		CRect rcUserList;
		rcUserList.top=PUBLICIZE_CY;
		rcUserList.right=rcClient.Width();
		rcUserList.left=rcVorSplitter.right;
		rcUserList.bottom=rcHorSplitter.top;
		m_UserListEncircle.DrawEncircleFrame(pDC,rcUserList);
	}

	//�滭�ν�
	ASSERT(CWndViewItemCtrl::GetInstance()!=NULL);
	CWndViewItemCtrl::GetInstance()->DrawLinkBoradLine(this,pDC);

	//��ȡ����
	ASSERT(CSkinRenderManager::GetInstance()!=NULL);
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();

	//���� DC
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(13,45,75));
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//������Ϣ
	TCHAR szTitle[64]=TEXT("");
	GetWindowText(szTitle,CountArray(szTitle));

	//�滭����
	CRect rcTitle(15,8,rcVorSplitter.left-TABLE_BUTTON_CX-8,20);
	pDC->DrawText(szTitle,lstrlen(szTitle),&rcTitle,DT_END_ELLIPSIS|DT_VCENTER|DT_SINGLELINE);

	//�滭���
	COLORREF crBackGround=pSkinRenderManager->RenderColor(0.67,0.95);
	pDC->FillSolidRect(rcVorSplitter.right,0,rcClient.Width()-rcVorSplitter.right,PUBLICIZE_CY,crBackGround);

	return TRUE;
}

//λ����Ϣ
VOID CServerViewItem::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//���ñ���
	m_bRectifyVor=false;

	//�����ؼ�
	RectifyControl(cx,cy);

	return;
}

//�Ҽ��б�
VOID CServerViewItem::OnNMRclickUserList(NMHDR * pNMHDR, LRESULT * pResult)
{
	//��������
	NMITEMACTIVATE * pListNotify=(NMITEMACTIVATE *)pNMHDR;
	
	//�����˵�
	if (pListNotify->iItem!=-1)
	{
		//ѡ����Ŀ
		UINT nSelectCount=m_UserListControl.GetSelectedCount();

		//ѡ����
		if (nSelectCount>1L)
		{
			//�û�����
			IClientUserItem * pIClientUserItem[MAX_WHISPER_USER];
			ZeroMemory(pIClientUserItem,sizeof(pIClientUserItem));

			//��������
			WORD wUserCount=0;
			POSITION nSelectPos=m_UserListControl.GetFirstSelectedItemPosition();

			//ѡ���ռ�
			while ((nSelectPos!=NULL)&&(wUserCount<CountArray(pIClientUserItem)))
			{
				//��ȡѡ��
				INT nSelectItem=m_UserListControl.GetNextSelectedItem(nSelectPos);

				//�����û�
				DWORD_PTR lItemData=m_UserListControl.GetItemData(nSelectItem);
				if (lItemData!=NULL) pIClientUserItem[wUserCount++]=((IClientUserItem *)(lItemData));
			};

			//�����˵�
			if (wUserCount>0) TrackUserItemMenu(pIClientUserItem,wUserCount);
		}
		else
		{
			//�����˵�
			DWORD_PTR lItemData=m_UserListControl.GetItemData(pListNotify->iItem);
			if (lItemData!=NULL) TrackUserItemMenu((IClientUserItem *)(lItemData));
		}
	}

	return;
}

//˫���б�
VOID CServerViewItem::OnNMDblclkUserList(NMHDR * pNMHDR, LRESULT * pResult)
{
	//��������
	NMITEMACTIVATE * pListNotify=(NMITEMACTIVATE *)pNMHDR;
	
	//��Ϣ����
	if ((pListNotify->iItem!=-1)&&(pListNotify->lParam!=NULL))
	{
		//��ȡ����
		DWORD_PTR dwItemData=m_UserListControl.GetItemData(pListNotify->iItem);
		IClientUserItem * pIClientUserItem=(IClientUserItem *)dwItemData;

		//ִ�ж���
		if (pIClientUserItem!=NULL)
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
					WORD wTableID=pIClientUserItem->GetTableID();
					WORD wChairID=pIClientUserItem->GetChairID();
					BYTE cbUserStatus=pIClientUserItem->GetUserStatus();

					//��λλ��
					if (wTableID!=INVALID_TABLE)
					{
						//���ÿ���
						m_TableViewFrame.VisibleTable(wTableID);

						//��������
						if (cbUserStatus==US_LOOKON)
						{
							m_TableViewFrame.FlashGameTable(wTableID);
						}
						else
						{
							m_TableViewFrame.FlashGameChair(wTableID,wChairID);
						}
					}

					break;
				}
			case ACTION_SEND_WHISPER:			//����˽��
				{
					//����˽��
					if ((m_ServiceStatus==ServiceStatus_ServiceIng)&&(pIClientUserItem!=m_pIMySelfUserItem))
					{
						IClientUserItem * pIClientUserItemArray[]={pIClientUserItem};
						WhisperConversation(pIClientUserItemArray,CountArray(pIClientUserItemArray));
					}

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
