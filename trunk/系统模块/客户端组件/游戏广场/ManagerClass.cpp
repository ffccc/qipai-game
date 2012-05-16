#include "StdAfx.h"
#include "GlobalUnits.h"
#include "ManagerClass.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CManagerServer, CSkinDialogEx)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CManagerMessage, CSkinDialogEx)
	ON_BN_CLICKED(IDC_PRIVIEW, OnPreViewMessage)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CManagerUserRight, CSkinDialogEx)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CManagerSendWarning, CSkinDialogEx)
	ON_BN_CLICKED(IDC_PRIVIEW, OnPreViewMessage)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CManagerSendWarning::CManagerSendWarning() : CSkinDialogEx(IDD_SEND_WARNING)
{
	//���ñ���
	m_dwUserID=0L;
	m_pIUserItem=NULL;

	//���ýӿ�
	m_pIClientSocket=NULL;
	m_pIClientUserManager=NULL;

}

//��������
CManagerSendWarning::~CManagerSendWarning()
{
}

//�ؼ���
void CManagerSendWarning::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btSend);
	DDX_Control(pDX, IDCANCEL, m_btClose);
	DDX_Control(pDX, IDC_PRIVIEW, m_btPreView);
	DDX_Control(pDX, IDC_WARNING_MESSAGE, m_WarningMessage);
}

//��ʼ������
BOOL CManagerSendWarning::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("���;��棺"));

	return TRUE;
}

//ȷ������
void CManagerSendWarning::OnOK()
{
	//�����û�
	if ((m_pIUserItem==NULL)||(m_pIUserItem->GetUserID()!=m_dwUserID))
	{
		//Ѱ���û�
		ASSERT(m_pIClientUserManager!=NULL);
		m_pIUserItem=m_pIClientUserManager->SearchUserByUserID(m_dwUserID);

		//�û��ж�
		if (m_pIUserItem==NULL)
		{
			ShowInformation(TEXT("Ŀ���û��Ѿ��뿪����Ϸ���䣬�޷�����û����;���֪ͨ��"),0,MB_ICONQUESTION);
			return;
		}
	}

	//��������
	CMD_GR_SendWarning SendWarning;
	SendWarning.dwTargetUserID=m_dwUserID;
	m_WarningMessage.GetWindowText(SendWarning.szWarningMessage,CountArray(SendWarning.szWarningMessage));
	SendWarning.wChatLength=CountStringBuffer(SendWarning.szWarningMessage);

	//��Ϣ�ж�
	if (SendWarning.szWarningMessage[0]==0)
	{
		
		ShowInformation(TEXT("������Ϣ����Ϊ�գ���д�뾯����Ϣ���ݣ�"),0,MB_ICONQUESTION);
		m_WarningMessage.SetFocus();
		return;
	}

	//��ȫ����
	SendWarning.szWarningMessage[CountArray(SendWarning.szWarningMessage)-1]=0;

	//������Ϣ
	ASSERT(m_pIClientSocket!=NULL);
	WORD wSendSize=sizeof(SendWarning)-sizeof(SendWarning.szWarningMessage)+SendWarning.wChatLength;
	m_pIClientSocket->SendData(MDM_GR_MANAGER,SUB_GR_SEND_WARNING,&SendWarning,wSendSize);

	//��������
	g_GlobalUnits.CopyToClipboard(SendWarning.szWarningMessage);

	//���ý���
	m_WarningMessage.SetFocus();
	m_WarningMessage.SetWindowText(TEXT(""));

	return;
}

//ȡ����Ϣ
void CManagerSendWarning::OnCancel()
{
	//���ٴ���
	DestroyWindow();

	return;
}

//�����û�
bool CManagerSendWarning::SetUserItem(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pIUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IUserItem);
	if (m_pIUserItem==NULL) return false;

	//���ñ���
	m_dwUserID=m_pIUserItem->GetUserID();

	//���½���
	SetDlgItemInt(IDC_USER_ID,m_dwUserID,FALSE);
	SetDlgItemText(IDC_ACCOUNTS,m_pIUserItem->GetUserName());

	return true;
}

//���ýӿ�
bool CManagerSendWarning::SetClientSocket(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pIClientSocket=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITCPSocket);
	if (m_pIClientSocket==NULL) return false;

	return true;
}

//���ýӿ�
bool CManagerSendWarning::SetClientUserManager(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pIClientUserManager=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IClientUserManager);
	if (m_pIClientUserManager==NULL) return false;

	return true;
}

//Ԥ����Ϣ
void CManagerSendWarning::OnPreViewMessage()
{
	//��ȡ��Ϣ
	TCHAR szMessage[MAX_CHAT_LEN]=TEXT("");
	m_WarningMessage.GetWindowText(szMessage,CountArray(szMessage));

	//��Ϣ�ж�
	if (szMessage[0]==0)
	{
		
		ShowInformation(TEXT("������Ϣ����Ϊ�գ���д�뾯����Ϣ���ݣ�"),0,MB_ICONQUESTION);
		m_WarningMessage.SetFocus();
		return;
	}

	//Ԥ����Ϣ
	ShowInformationEx(szMessage,0,MB_ICONQUESTION,TEXT("����Ԥ����"));
	m_WarningMessage.SetFocus();

	return;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CManagerUserRight::CManagerUserRight() : CSkinDialogEx(IDD_MANAGER_RIGHT)
{
	//���ñ���
	m_dwUserID=0L;
	m_pIUserItem=NULL;

	//Ȩ�ޱ���
	m_dwUserRight=0L;
	//m_dwLastUserRight=0L;
	m_dwMasterRight=0L;

	//���ýӿ�
	m_pIClientSocket=NULL;
	m_pIClientUserManager=NULL;

}

//��������
CManagerUserRight::~CManagerUserRight()
{
}

//�ؼ���
void CManagerUserRight::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Control(pDX, IDCANCEL, m_btClose);
}

//��ʼ������
BOOL CManagerUserRight::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("Ȩ�޹���"));

	//���¿ؼ�
	UpdateRightData();
	UpdateMasterControl();

	return TRUE;
}

//ȷ������
void CManagerUserRight::OnOK()
{
	//�����û�
	if ((m_pIUserItem==NULL)||(m_pIUserItem->GetUserID()!=m_dwUserID))
	{
		//Ѱ���û�
		ASSERT(m_pIClientUserManager!=NULL);
		m_pIUserItem=m_pIClientUserManager->SearchUserByUserID(m_dwUserID);

		//�û��ж�
		if (m_pIUserItem==NULL)
		{
			
			ShowInformation(TEXT("Ŀ���û��Ѿ��뿪����Ϸ���䣬�û�Ȩ������ʧ�ܣ�"),0,MB_ICONQUESTION);
			return;
		}
	}

	//��������
	CMD_GR_SetUserRight SetUserRight;
	SetUserRight.dwTargetUserID=m_dwUserID;
	SetUserRight.cbGameRight=(((CButton *)GetDlgItem(IDC_BIND_GAME))->GetCheck()==BST_CHECKED)?TRUE:FALSE;
	SetUserRight.cbAccountsRight=(((CButton *)GetDlgItem(IDC_BIND_USER))->GetCheck()==BST_CHECKED)?TRUE:FALSE;
	SetUserRight.cbLimitRoomChat=(((CButton *)GetDlgItem(IDC_ROOM_CHAT))->GetCheck()==BST_CHECKED)?TRUE:FALSE;
	SetUserRight.cbLimitGameChat=(((CButton *)GetDlgItem(IDC_GAME_CHAT))->GetCheck()==BST_CHECKED)?TRUE:FALSE;
	SetUserRight.cbLimitPlayGame=(((CButton *)GetDlgItem(IDC_PLAY_GAME))->GetCheck()==BST_CHECKED)?TRUE:FALSE;
	SetUserRight.cbLimitSendWisper=(((CButton *)GetDlgItem(IDC_SEND_WISPER))->GetCheck()==BST_CHECKED)?TRUE:FALSE;
	SetUserRight.cbLimitLookonGame=(((CButton *)GetDlgItem(IDC_LOOKON_GAME))->GetCheck()==BST_CHECKED)?TRUE:FALSE;

	//������Ϣ
	ASSERT(m_pIClientSocket!=NULL);
	m_pIClientSocket->SendData(MDM_GR_MANAGER,SUB_GR_SET_USER_RIGHT,&SetUserRight,sizeof(SetUserRight));

	//�رմ���
	DestroyWindow();

	return;
}

//ȡ����Ϣ
void CManagerUserRight::OnCancel()
{
	//���ٴ���
	DestroyWindow();

	return;
}

//����Ȩ��
bool CManagerUserRight::SetMasterRight(DWORD dwMasterRight)
{
	//���ñ���
	m_dwMasterRight=dwMasterRight;

	//���¿ؼ�
	UpdateMasterControl();

	return true;
}

//�����û�
bool CManagerUserRight::SetUserItem(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pIUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IUserItem);
	if (m_pIUserItem==NULL) return false;

	//���ñ���
	m_dwUserID=m_pIUserItem->GetUserID();
	//m_dwLastUserRight=m_dwUserRight;
	m_dwUserRight=m_pIUserItem->GetUserData()->dwUserRight;

	//���½���
	SetDlgItemInt(IDC_USER_ID,m_dwUserID,FALSE);
	SetDlgItemText(IDC_ACCOUNTS,m_pIUserItem->GetUserName());

	//��������
	UpdateRightData();

	return true;
}

//���ýӿ�
bool CManagerUserRight::SetClientSocket(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pIClientSocket=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITCPSocket);
	if (m_pIClientSocket==NULL) return false;

	return true;
}

//���ýӿ�
bool CManagerUserRight::SetClientUserManager(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pIClientUserManager=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IClientUserManager);
	if (m_pIClientUserManager==NULL) return false;

	return true;
}

//��������
bool CManagerUserRight::UpdateRightData()
{
	//���ÿؼ�
	if (m_hWnd!=NULL)
	{
		((CButton *)GetDlgItem(IDC_PLAY_GAME))->SetCheck((CUserRight::CanPlay(m_dwUserRight)==true)?BST_UNCHECKED:BST_CHECKED);
		((CButton *)GetDlgItem(IDC_LOOKON_GAME))->SetCheck((CUserRight::CanLookon(m_dwUserRight)==true)?BST_UNCHECKED:BST_CHECKED);
		((CButton *)GetDlgItem(IDC_ROOM_CHAT))->SetCheck((CUserRight::CanRoomChat(m_dwUserRight)==true)?BST_UNCHECKED:BST_CHECKED);
		((CButton *)GetDlgItem(IDC_GAME_CHAT))->SetCheck((CUserRight::CanGameChat(m_dwUserRight)==true)?BST_UNCHECKED:BST_CHECKED);
		((CButton *)GetDlgItem(IDC_SEND_WISPER))->SetCheck((CUserRight::CanWisper(m_dwUserRight)==true)?BST_UNCHECKED:BST_CHECKED);
	}

	return true;
}

//���¿���
bool CManagerUserRight::UpdateMasterControl()
{
	//���ÿؼ�
	if (m_hWnd!=NULL)
	{
		//���ÿ���
		if (CUserRight::CanBindGame(m_dwMasterRight)==true) GetDlgItem(IDC_BIND_GAME)->EnableWindow(TRUE);
		if (CUserRight::CanLimitPlay(m_dwMasterRight)==true) GetDlgItem(IDC_PLAY_GAME)->EnableWindow(TRUE);
		if (CUserRight::CanBindGlobal(m_dwMasterRight)==true) GetDlgItem(IDC_BIND_USER)->EnableWindow(TRUE);
		if (CUserRight::CanLimitRoomChat(m_dwMasterRight)==true) GetDlgItem(IDC_ROOM_CHAT)->EnableWindow(TRUE);
		if (CUserRight::CanLimitGameChat(m_dwMasterRight)==true) GetDlgItem(IDC_GAME_CHAT)->EnableWindow(TRUE);
		if (CUserRight::CanLimitWisper(m_dwMasterRight)==true) GetDlgItem(IDC_SEND_WISPER)->EnableWindow(TRUE);
		if (CUserRight::CanLimitLookon(m_dwMasterRight)==true) GetDlgItem(IDC_LOOKON_GAME)->EnableWindow(TRUE);

		//���ÿؼ�
		if (m_dwMasterRight!=0) m_btOk.EnableWindow(TRUE);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CManagerMessage::CManagerMessage() : CSkinDialogEx(IDD_MANAGER_MESSAGE)
{
	//���ýӿ�
	m_pIClientSocket=NULL;

	return;
}

//��������
CManagerMessage::~CManagerMessage()
{
}

//�ؼ���
void CManagerMessage::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btSend);
	DDX_Control(pDX, IDCANCEL, m_btClose);
	DDX_Control(pDX, IDC_PRIVIEW, m_btPreView);
	DDX_Control(pDX, IDC_SYSTEM_MESSAGE, m_SystemMessage);
}

//��ʼ������
BOOL CManagerMessage::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("ϵͳ��Ϣ��"));

	return TRUE;
}

//ȷ������
void CManagerMessage::OnOK()
{
	//��������
	CMD_GR_SendMessage SendMessage;
	SendMessage.cbGame=(((CButton *)GetDlgItem(IDC_GAME_MESSAGE))->GetCheck()==BST_CHECKED)?TRUE:FALSE;
	SendMessage.cbRoom=(((CButton *)GetDlgItem(IDC_ROOM_MESSAGE))->GetCheck()==BST_CHECKED)?TRUE:FALSE;
	m_SystemMessage.GetWindowText(SendMessage.szSystemMessage,CountArray(SendMessage.szSystemMessage));
	SendMessage.wChatLength=CountStringBuffer(SendMessage.szSystemMessage);

	//��Ϣ�ж�
	if (SendMessage.szSystemMessage[0]==0)
	{
		
		ShowInformation(TEXT("ϵͳ��Ϣ����Ϊ�գ���д��ϵͳ��Ϣ���ݣ�"),0,MB_ICONQUESTION);
		m_SystemMessage.SetFocus();
		return;
	}

	//�����ж�
	if ((SendMessage.cbGame==FALSE)&&(SendMessage.cbRoom==FALSE))
	{
		
		ShowInformation(TEXT("����ʧ�ܣ���ѡ��ϵͳ��Ϣ�����ͣ�"),0,MB_ICONQUESTION);
		return;
	}

	//��ȫ����
	SendMessage.szSystemMessage[CountArray(SendMessage.szSystemMessage)-1]=0;

	//������Ϣ
	ASSERT(m_pIClientSocket!=NULL);
	WORD wSendSize=sizeof(SendMessage)-sizeof(SendMessage.szSystemMessage)+SendMessage.wChatLength;
	m_pIClientSocket->SendData(MDM_GR_MANAGER,SUB_GR_SEND_MESSAGE,&SendMessage,wSendSize);

	//��������
	g_GlobalUnits.CopyToClipboard(SendMessage.szSystemMessage);

	//���ý���
	m_SystemMessage.SetFocus();
	m_SystemMessage.SetWindowText(TEXT(""));

	return;
}

//ȡ����Ϣ
void CManagerMessage::OnCancel()
{
	//���ٴ���
	DestroyWindow();

	return;
}

//���ýӿ�
bool CManagerMessage::SetClientSocket(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pIClientSocket=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITCPSocket);
	if (m_pIClientSocket==NULL) return false;

	return true;
}

//Ԥ����Ϣ
void CManagerMessage::OnPreViewMessage()
{
	//��ȡ��Ϣ
	TCHAR szMessage[MAX_CHAT_LEN]=TEXT("");
	m_SystemMessage.GetWindowText(szMessage,CountArray(szMessage));

	//��Ϣ�ж�
	if (szMessage[0]==0)
	{
		
		ShowInformation(TEXT("ϵͳ��Ϣ����Ϊ�գ���д��ϵͳ��Ϣ���ݣ�"),0,MB_ICONQUESTION);
		m_SystemMessage.SetFocus();
		return;
	}

	//Ԥ����Ϣ
	ShowInformationEx(szMessage,0,MB_ICONQUESTION,TEXT("��ϢԤ����"));
	m_SystemMessage.SetFocus();

	return;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CManagerServer::CManagerServer() : CSkinDialogEx(IDD_MANAGER_SERVER)
{
	//���ýӿ�
	m_pIClientSocket=NULL;

	return;
}

//��������
CManagerServer::~CManagerServer()
{
}

//�ؼ���
void CManagerServer::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIMIT_GAME_CHAT, m_btLimitGameChat);
	DDX_Control(pDX, IDC_ALLOW_GAME_CHAT, m_btAllowGameChat);
	DDX_Control(pDX, IDC_LIMIT_ROOM_CHAT, m_btLimitRoomChat);
	DDX_Control(pDX, IDC_ALLOW_ROOM_CHAT, m_btAllowRoomChat);
	DDX_Control(pDX, IDC_LIMIT_ENTER_ROOM, m_btLimitEnterRoom);
	DDX_Control(pDX, IDC_ALLOW_ENTER_ROOM, m_btAllowEnterRoom);
	DDX_Control(pDX, IDC_LIMIT_ENTER_GAME, m_btLimitEnterGame);
	DDX_Control(pDX, IDC_ALLOW_ENTER_GAME, m_btAllowEnterGame);
	DDX_Control(pDX, IDC_SET_SHALL_CLOSE, m_btSetShallClose);
	DDX_Control(pDX, IDC_RESET_SHALL_CLOSE, m_btReSetShallClose);
	DDX_Control(pDX, IDC_LIMIT_WISPER, m_btLimitRoomWisper);
	DDX_Control(pDX, IDC_ALLOW_WISPER, m_btAllowRoomWisper);
}

//��ʼ������
BOOL CManagerServer::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("�������"));

	return TRUE;
}

//ȷ������
void CManagerServer::OnOK()
{
	return;
}

//ȡ����Ϣ
void CManagerServer::OnCancel()
{
	//���ٴ���
	DestroyWindow();

	return;
}

//�˵�����
BOOL CManagerServer::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	CMD_GR_OptionServer OptionServer;
	ZeroMemory(&OptionServer,sizeof(OptionServer));

	switch (LOWORD(wParam))
	{
	case IDC_SET_SHALL_CLOSE:		//�����ر�
		{
			//���ñ���
			OptionServer.cbOptionValue=TRUE;
			OptionServer.cbOptionFlags=OSF_SHALL_CLOSE;

			break;
		}
	case IDC_RESET_SHALL_CLOSE:		//�����ر�
		{
			//���ñ���
			OptionServer.cbOptionValue=FALSE;
			OptionServer.cbOptionFlags=OSF_SHALL_CLOSE;

			break;
		}
	case IDC_LIMIT_WISPER:			//����˽��
		{
			//���ñ���
			OptionServer.cbOptionValue=FALSE;
			OptionServer.cbOptionFlags=OSF_ROOM_WISPER;

			break;
		}
	case IDC_ALLOW_WISPER:			//����˽��
		{
			//���ñ���
			OptionServer.cbOptionValue=TRUE;
			OptionServer.cbOptionFlags=OSF_ROOM_WISPER;

			break;
		}
	case IDC_LIMIT_ROOM_CHAT:		//��������
		{
			//���ñ���
			OptionServer.cbOptionValue=FALSE;
			OptionServer.cbOptionFlags=OSF_ROOM_CHAT;

			break;
		}
	case IDC_ALLOW_ROOM_CHAT:		//��������
		{
			//���ñ���
			OptionServer.cbOptionValue=TRUE;
			OptionServer.cbOptionFlags=OSF_ROOM_CHAT;

			break;
		}
	case IDC_LIMIT_GAME_CHAT:		//��Ϸ����
		{
			//���ñ���
			OptionServer.cbOptionValue=FALSE;
			OptionServer.cbOptionFlags=OSF_GAME_CHAT;

			break;
		}
	case IDC_ALLOW_GAME_CHAT:		//��Ϸ����
		{
			//���ñ���
			OptionServer.cbOptionValue=TRUE;
			OptionServer.cbOptionFlags=OSF_GAME_CHAT;

			break;
		}
	case IDC_LIMIT_ENTER_ROOM:		//���뷿��
		{
			//���ñ���
			OptionServer.cbOptionValue=FALSE;
			OptionServer.cbOptionFlags=OSF_ENTER_ROOM;

			break;
		}
	case IDC_ALLOW_ENTER_ROOM:		//���뷿��
		{
			//���ñ���
			OptionServer.cbOptionValue=TRUE;
			OptionServer.cbOptionFlags=OSF_ENTER_ROOM;

			break;
		}
	case IDC_LIMIT_ENTER_GAME:		//������Ϸ
		{
			//���ñ���
			OptionServer.cbOptionValue=FALSE;
			OptionServer.cbOptionFlags=OSF_ENTER_GAME;

			break;
		}
	case IDC_ALLOW_ENTER_GAME:		//������Ϸ
		{
			//���ñ���
			OptionServer.cbOptionValue=TRUE;
			OptionServer.cbOptionFlags=OSF_ENTER_GAME;

			break;
		}
	}

	//������Ϣ
	if (OptionServer.cbOptionFlags!=0)
	{
		ASSERT(m_pIClientSocket!=NULL);
		m_pIClientSocket->SendData(MDM_GR_MANAGER,SUB_GR_OPTION_SERVER,&OptionServer,sizeof(OptionServer));
		return TRUE;
	}

	return __super::OnCommand(wParam,lParam);
}

//���ýӿ�
bool CManagerServer::SetClientSocket(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pIClientSocket=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITCPSocket);
	if (m_pIClientSocket==NULL) return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////
