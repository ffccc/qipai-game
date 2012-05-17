#include "StdAfx.h"
#include "DlgWhisper.h"
#include "PlatformFrame.h"
#include "ServerViewItem.h"

//////////////////////////////////////////////////////////////////////////////////

//�û���Ϣ
#define USER_INFO_CX				188									//��Ϣ���
#define USER_INFO_CY				180									//��Ϣ�߶�
#define CHAT_INPUT_CY				100									//����߶�

//��ɫѡ��
#define MAX_CHAT_COLOR				16									//�����Ŀ
#define IDM_SELECT_CHAT_COLOR		(WM_USER+200)						//ѡ����ɫ

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgWhisper, CSkinDialog)

	//ϵͳ����
	ON_WM_SIZE()
	ON_WM_NCDESTROY()

	//��ť����
	ON_BN_CLICKED(IDC_BT_USER_MENU, OnBnClickedUserMenu)
	ON_BN_CLICKED(IDC_BT_SEND_CHAT, OnBnClickedSendChat)
	ON_BN_CLICKED(IDC_BT_SEND_CONTROL, OnBnClickedSendControl)
	ON_BN_CLICKED(IDC_BT_CHAT_BUTTON_1, OnBnClickedChatButton1)
	ON_BN_CLICKED(IDC_BT_CHAT_BUTTON_2, OnBnClickedChatButton2)
	ON_BN_CLICKED(IDC_BT_CHAT_BUTTON_3, OnBnClickedChatButton3)

	//�˵�����
	ON_COMMAND(IDM_MORE_COLOR, OnSelectMoreColor)
	ON_COMMAND_RANGE(IDM_SELECT_CHAT_COLOR, (IDM_SELECT_CHAT_COLOR+MAX_CHAT_COLOR), OnSelectChatColor)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CDlgWhisper::CDlgWhisper() : CSkinDialog(IDD_DLG_WHISPER)
{
	//״̬����
	m_bCreateFlag=false;
	m_dwConversationID=0L;

	//�ӿڱ���
	m_pITCPSocket=NULL;
	m_pIMySelfUserItem=NULL;
	m_pIUserOrderParser=NULL;
	m_pIGameLevelParser=NULL;

	//�������
	m_pServerViewItem=NULL;
	m_pExpressionControl=NULL;

    //������Դ
	tagEncircleResource	EncircleChatInput;
	EncircleChatInput.pszImageTL=MAKEINTRESOURCE(IDB_INPUT_WHISPER_TL);
	EncircleChatInput.pszImageTM=MAKEINTRESOURCE(IDB_INPUT_WHISPER_TM);
	EncircleChatInput.pszImageTR=MAKEINTRESOURCE(IDB_INPUT_WHISPER_TR);
	EncircleChatInput.pszImageML=MAKEINTRESOURCE(IDB_INPUT_WHISPER_ML);
	EncircleChatInput.pszImageMR=MAKEINTRESOURCE(IDB_INPUT_WHISPER_MR);
	EncircleChatInput.pszImageBL=MAKEINTRESOURCE(IDB_INPUT_WHISPER_BL);
	EncircleChatInput.pszImageBM=MAKEINTRESOURCE(IDB_INPUT_WHISPER_BM);
	EncircleChatInput.pszImageBR=MAKEINTRESOURCE(IDB_INPUT_WHISPER_BR);
	m_EncircleChatInput.InitEncircleResource(EncircleChatInput,AfxGetInstanceHandle());

	//��Ϣ��Դ
	tagEncircleResource	EncircleChatMessage;
	EncircleChatMessage.pszImageTL=MAKEINTRESOURCE(IDB_CHAT_WHISPER_TL);
	EncircleChatMessage.pszImageTM=MAKEINTRESOURCE(IDB_CHAT_WHISPER_TM);
	EncircleChatMessage.pszImageTR=MAKEINTRESOURCE(IDB_CHAT_WHISPER_TR);
	EncircleChatMessage.pszImageML=MAKEINTRESOURCE(IDB_CHAT_WHISPER_ML);
	EncircleChatMessage.pszImageMR=MAKEINTRESOURCE(IDB_CHAT_WHISPER_MR);
	EncircleChatMessage.pszImageBL=MAKEINTRESOURCE(IDB_CHAT_WHISPER_BL);
	EncircleChatMessage.pszImageBM=MAKEINTRESOURCE(IDB_CHAT_WHISPER_BM);
	EncircleChatMessage.pszImageBR=MAKEINTRESOURCE(IDB_CHAT_WHISPER_BR);
	m_EncircleChatMessage.InitEncircleResource(EncircleChatMessage,AfxGetInstanceHandle());

	return;
}

//��������
CDlgWhisper::~CDlgWhisper()
{
	//ɾ������
	SafeDelete(m_pExpressionControl);

	return;
}

//�ؼ���
VOID CDlgWhisper::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	//���찴ť
	DDX_Control(pDX, IDC_BT_CHAT_HISTORY, m_btChatHistory);
	DDX_Control(pDX, IDC_BT_CHAT_BUTTON_1, m_btChatButton1);
	DDX_Control(pDX, IDC_BT_CHAT_BUTTON_2, m_btChatButton2);
	DDX_Control(pDX, IDC_BT_CHAT_BUTTON_3, m_btChatButton3);

	//���ư�ť
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_BT_USER_MENU, m_btUserMenu);
	DDX_Control(pDX, IDC_BT_SEND_CHAT, m_btSendChat);
	DDX_Control(pDX, IDC_BT_CHAT_HISTORY, m_btChatHistory);
	DDX_Control(pDX, IDC_BT_SEND_CONTROL, m_btSendControl);

	//����ؼ�
	DDX_Control(pDX, IDC_CHAT_INPUT, m_ChatInput);
	DDX_Control(pDX, IDC_CHAT_MESSAGE, m_ChatMessage);
}

//��Ϣ����
BOOL CDlgWhisper::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}

	//���Ϳ���
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_RETURN))
	{
		//��������
		bool bLControlDown=((GetAsyncKeyState(VK_LCONTROL)&0xFFF0)!=0);
		bool bRControlDown=((GetAsyncKeyState(VK_RCONTROL)&0xFFF0)!=0);

		//��������
		ASSERT(CParameterGlobal::GetInstance()!=NULL);
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

		//���Ϳ���
		if ((pParameterGlobal->m_bSendWhisperByEnter==true)&&(bLControlDown==false)&&(bRControlDown==false))
		{
			OnBnClickedSendChat();
			return TRUE;
		}

		//���Ϳ���
		if ((pParameterGlobal->m_bSendWhisperByEnter==false)&&((bLControlDown==true)||(bRControlDown==true)))
		{
			OnBnClickedSendChat();
			return TRUE;
		}
	}

	return __super::PreTranslateMessage(pMsg);
}

//���ú���
BOOL CDlgWhisper::OnInitDialog()
{
	__super::OnInitDialog();

	//���ô���
	SetWindowText(TEXT("˽��"));
	SetIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_WHISPER)),TRUE);

	//��������
	CRect rcCreate(0,0,0,0);
	HINSTANCE hInstance=AfxGetInstanceHandle();

	//����ؼ�
	m_ChatInput.SetLimitText(LEN_USER_CHAT-1);
	m_ChatMessage.SetExpressionManager(CExpressionManager::GetInstance(),RGB(255,255,255));

	//���찴ť
	m_btUserMenu.SetButtonImage(IDB_BT_WHISPER_USER_MENU,hInstance,false,false);
	m_btSendControl.SetButtonImage(IDB_BT_WHISPER_SEND_CONTROL,hInstance,false,false);
	m_btChatHistory.SetButtonImage(IDB_BT_WHISPER_HISTORY,TEXT("BT_WHISPER_HISTORY"),hInstance,false,false);
	m_btChatButton1.SetButtonImage(IDB_BT_WHISPER_BUTTON_1,TEXT("BT_WHISPER_BUTTON_1"),hInstance,false,false);
	m_btChatButton2.SetButtonImage(IDB_BT_WHISPER_BUTTON_2,TEXT("BT_WHISPER_BUTTON_2"),hInstance,false,false);
	m_btChatButton3.SetButtonImage(IDB_BT_WHISPER_BUTTON_3,TEXT("BT_WHISPER_BUTTON_3"),hInstance,false,false);

	//���ñ���
	m_bCreateFlag=true;

	//�����ؼ�
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());

	//��ʾ��Ϣ
	m_ChatMessage.InsertPromptString(TEXT("������˽���������κ���ʽ���н���Ϣ���������κ���͸¶�������룡\r\n"));

	return TRUE;
}

//ȷ����Ϣ
VOID CDlgWhisper::OnOK()
{
	return;
}

//ȡ����Ϣ
VOID CDlgWhisper::OnCancel()
{
	//�رմ���
	DestroyWindow();

	return;
}

//�����
BOOL CDlgWhisper::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//�����
	switch (LOWORD(wParam))
	{
	case IDM_SEND_BY_ENTER:
		{
			//���ñ���
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
			if (pParameterGlobal!=NULL) pParameterGlobal->m_bSendWhisperByEnter=true;

			//���ý���
			m_ChatInput.SetFocus();

			return TRUE;
		}
	case IDM_SEND_BY_CTRL_ENTER:
		{
			//���ñ���
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
			if (pParameterGlobal!=NULL) pParameterGlobal->m_bSendWhisperByEnter=false;

			//���ý���
			m_ChatInput.SetFocus();

			return TRUE;
		}
	}

	//��ɫѡ��

	return __super::OnCommand(wParam,lParam);
}

//�����¼�
VOID CDlgWhisper::OnExpressionSelect(CExpression * pExpression, tagExpressionInfo * pExpressionInfo)
{
	//���ý���
	m_ChatInput.SetFocus();

	//���ͱ���
	if (EfficacyUserChat(NULL,pExpression->GetIndex())==true)
	{
		if (m_ClientUserItemArray.GetCount()==2L)
		{
			//˽����Ϣ
			ASSERT(m_ClientUserItemArray[1]!=NULL);
			SendExpressionPacket(m_ClientUserItemArray[1],pExpression->GetIndex());
		}
		else if (m_ClientUserItemArray.GetCount()>=3L)
		{
			//Ⱥ����Ϣ
			IClientUserItem * * pIClientUserItem=m_ClientUserItemArray.GetData();
		//	SendExpressionPacket(pIClientUserItem,(WORD)m_ClientUserItemArray.GetCount(),strMessage);
		}
		else
		{
			//������ʾ
			TCHAR szMessage[LEN_USER_CHAT+128]=TEXT("");
			_sntprintf(szMessage,CountArray(szMessage),TEXT("���λỰ�������û��Ѿ��뿪����Ϣ�޷�����"));

			//������Ϣ
			m_ChatMessage.InsertString(TEXT("\r\n"),0);
			m_ChatMessage.InsertSystemString(szMessage);
		}
	}

	return;
}

//�滭��Ϣ
VOID CDlgWhisper::OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight)
{
	//���λ��
	INT nTBorder=m_SkinAttribute.m_EncircleInfoFrame.nTBorder;
	INT nBBorder=m_SkinAttribute.m_EncircleInfoFrame.nBBorder;
	INT nLBorder=m_SkinAttribute.m_EncircleInfoFrame.nLBorder;
	INT nRBorder=m_SkinAttribute.m_EncircleInfoFrame.nRBorder;

	//����λ��
	CRect rcChatInput;
	rcChatInput.left=nLBorder;
	rcChatInput.bottom=nHeight-nBBorder;
	rcChatInput.right=nWidth-USER_INFO_CX-nRBorder;
	rcChatInput.top=nHeight-CHAT_INPUT_CY-nBBorder;

	//��Ϣλ��
	CRect rcChatMessage;
	rcChatMessage.top=nTBorder;
	rcChatMessage.left=nLBorder;
	rcChatMessage.right=nWidth-USER_INFO_CX-nRBorder;
	rcChatMessage.bottom=nHeight-CHAT_INPUT_CY-nBBorder;

	//��������
	CRect rcChatControl;
	rcChatControl.right=nWidth-5;
	rcChatControl.bottom=nHeight-9;
	rcChatControl.top=nTBorder+USER_INFO_CY+5;
	rcChatControl.left=nWidth-USER_INFO_CX-nRBorder;

	//�滭����
	COLORREF crBackGround=RGB(215,223,228);
	pDC->FillSolidRect(nLBorder,nTBorder,nWidth-nLBorder-nRBorder,nHeight-nTBorder-nBBorder,crBackGround);

	//�滭����
	m_EncircleChatInput.DrawEncircleFrame(pDC,rcChatInput);
	m_EncircleChatMessage.DrawEncircleFrame(pDC,rcChatMessage);

	//������Ϣ
	DrawUserInfoArea(pDC,nWidth-USER_INFO_CX-nRBorder,nTBorder,USER_INFO_CX,USER_INFO_CY);
	DrawFunctionArea(pDC,rcChatControl.left,rcChatControl.top,rcChatControl.Width(),rcChatControl.Height());

	return;
}

//�����ж�
bool CDlgWhisper::DisuseEstimate()
{
	//�����ж�
	if (m_hWnd==NULL) return true;
	if (m_ClientUserItemArray.GetCount()<=1L) return true;

	return false;
}

//�û��Ƚ�
bool CDlgWhisper::CompareUserItem(DWORD dwUserID[], WORD wUserCount)
{
	//��Ŀ�Ա�
	if (m_ClientUserItemArray.GetCount()!=wUserCount)
	{
		return false;
	}

	//�û��Ա�
	for (INT_PTR i=0;i<m_ClientUserItemArray.GetCount();i++)
	{
		//�û���ʶ
		DWORD dwUserItemID=m_ClientUserItemArray[i]->GetUserID();

		//�û��Ա�
		for (WORD j=0;j<wUserCount;j++) 
		{
			if (dwUserID[j]==dwUserItemID) break;
		}

		//����ж�
		if (j==wUserCount) return false;
	}

	return true;
}

//����Ч��
bool CDlgWhisper::EfficacyUserChat(LPCTSTR pszChatString, WORD wExpressionIndex)
{
	return true;
}

//��������
bool CDlgWhisper::CreateWhisperWnd(DWORD dwConversationID, IClientUserItem * pIClientUserItem[], WORD wUserCount, CWnd * pParentWnd)
{
	//���ñ���
	m_ClientUserItemArray.RemoveAll();
	m_dwConversationID=dwConversationID;

	//�û��ռ�
	for (WORD i=0;i<wUserCount;i++)
	{
		//��ֹ�ж�
		if (wUserCount>=MAX_WHISPER_USER) 
		{
			ASSERT(FALSE);
			break;
		}

		//�����û�
		ASSERT(pIClientUserItem[i]!=NULL);
		m_ClientUserItemArray.Add(pIClientUserItem[i]);
	}

	//��������
	if (m_hWnd!=NULL)
	{
		//��������
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//���´���
		RectifyTitle();
	}
	else 
	{
		//��������
		Create(IDD_DLG_WHISPER,pParentWnd);
	}

	//�ö�����
	SetActiveWindow();
	BringWindowToTop();
	SetForegroundWindow();

	return true;
}

//�û�����
VOID CDlgWhisper::OnEventUserEnter(IClientUserItem * pIClientUserItem)
{
	//�û�����
	for (INT_PTR i=0;i<m_ClientUserItemArray.GetCount();i++)
	{
		if (pIClientUserItem==m_ClientUserItemArray[i])
		{
			ASSERT(FALSE);
			return;
		}
	}

	//��������
	m_ClientUserItemArray.Add(pIClientUserItem);

	//�¼�֪ͨ
	m_ChatMessage.InsertUserAccounts(pIClientUserItem->GetNickName());
	m_ChatMessage.InsertString(TEXT("�����˱��λỰ\r\n"),COLOR_EVENT);

	//���±���
	RectifyTitle();

	//���½���
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE);

	return;
}

//�û��뿪
VOID CDlgWhisper::OnEventUserLeave(IClientUserItem * pIClientUserItem)
{
	//�û�����
	for (INT_PTR i=0;i<m_ClientUserItemArray.GetCount();i++)
	{
		if (pIClientUserItem==m_ClientUserItemArray[i])
		{
			//��������
			m_ClientUserItemArray.RemoveAt(i);

			//�Ự����
			if (m_ClientUserItemArray.GetCount()<=2)
			{
				m_dwConversationID=0L;
			}

			//���뻻��
			LONG lTextLength=m_ChatMessage.GetWindowTextLength();
			if (lTextLength!=0L) m_ChatMessage.InsertString(TEXT("\r\n"),COLOR_EVENT);

			//�¼�֪ͨ
			m_ChatMessage.InsertUserAccounts(pIClientUserItem->GetNickName());
			m_ChatMessage.InsertString(TEXT("�뿪�˱��λỰ\r\n"),COLOR_EVENT);

			//���±���
			RectifyTitle();

			//���½���
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE);

			return;
		}
	}

	return;
}

//�û�״̬
VOID CDlgWhisper::OnEventUserStatus(IClientUserItem * pIClientUserItem)
{
	//״̬����
	for (INT_PTR i=0;i<m_ClientUserItemArray.GetCount();i++)
	{
		if (m_ClientUserItemArray[i]==pIClientUserItem)
		{
			//���½���
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE);

			break;
		}
	}

	return;
}

//�û�����
VOID CDlgWhisper::OnEventExpression(IClientUserItem * pIClientUserItem, WORD wItemIndex)
{
	//Ч�����
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return;

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��������
	CExpressionManager * pExpressionManager=CExpressionManager::GetInstance();
	CExpression * pExpressionItem=pExpressionManager->GetExpressionItem(wItemIndex);

	//������Ϣ
	if (pExpressionItem!=NULL)
	{
		//״̬����
		DWORD dwSendUserID=pIClientUserItem->GetUserID();
		bool bMySelfString=(pGlobalUserData->dwUserID==dwSendUserID);

		//��ȡ·��
		TCHAR szImagePath[MAX_PATH]=TEXT("");
		pExpressionItem->GetExpressionPath(szImagePath,CountArray(szImagePath));

		//��ʾ��Ϣ
		LPCTSTR pszNiceName=pIClientUserItem->GetNickName();
		m_ChatMessage.InsertExpression(pszNiceName,szImagePath,bMySelfString);
	}

	//��ʾ����
	if (IsWindowVisible()==FALSE)
	{
		if (pParameterGlobal->m_bAutoShowWhisper==true)
		{
			//��ʾ����
			ShowWindow(SW_RESTORE);

			//�ö�����
			SetActiveWindow();
			BringWindowToTop();
			SetForegroundWindow();
		}
		else
		{
			//�ռ�����
			CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
			if (pPlatformFrame!=NULL) pPlatformFrame->InsertWhisperItem(this);
		}
	}
	else
	{
		//�����
		if (IsChild(GetFocus())==FALSE)
		{
			FlashWindow(TRUE);
		}
	}

	return;
}

//�û�˽��
VOID CDlgWhisper::OnEventWisperChat(IClientUserItem * pIClientUserItem, LPCTSTR pszString, COLORREF rcColor)
{
	//Ч�����
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return;

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//״̬����
	DWORD dwSendUserID=pIClientUserItem->GetUserID();
	bool bMySelfString=(pGlobalUserData->dwUserID==dwSendUserID);

	//��ʾ��Ϣ
	LPCTSTR pszNiceName=pIClientUserItem->GetNickName();
	m_ChatMessage.InsertWhisperChat(pszNiceName,pszString,rcColor,bMySelfString);

	//��ʾ����
	if (IsWindowVisible()==FALSE)
	{
		if (pParameterGlobal->m_bAutoShowWhisper==true)
		{
			//��ʾ����
			ShowWindow(SW_RESTORE);

			//�ö�����
			SetActiveWindow();
			BringWindowToTop();
			SetForegroundWindow();
		}
		else
		{
			//�ռ�����
			CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
			if (pPlatformFrame!=NULL) pPlatformFrame->InsertWhisperItem(this);
		}
	}
	else
	{
		//�����
		if (IsChild(GetFocus())==FALSE)
		{
			FlashWindow(TRUE);
		}
	}

	return;
}

//���±���
VOID CDlgWhisper::RectifyTitle()
{
	//��������
	TCHAR szTitle[MAX_PATH]=TEXT("");
	LPCTSTR pszServerName=m_pServerViewItem->GetServerName();

	//�������
	if (m_ClientUserItemArray.GetCount()<=2)
	{
		_sntprintf(szTitle,CountArray(szTitle),TEXT("[ %s ] �� ˽��"),pszServerName);
	}
	else
	{
		_sntprintf(szTitle,CountArray(szTitle),TEXT("[ %s ] �� ��������"),pszServerName);
	}

	//���ñ���
	SetWindowText(szTitle);

	return;
}

//�����ؼ�
VOID CDlgWhisper::RectifyControl(INT nWidth, INT nHeight)
{
	//״̬�ж�
	if (m_bCreateFlag==false) return;
	if ((nWidth==0)||(nHeight==0)) return;

	//���λ��
	INT nTBorder=m_SkinAttribute.m_EncircleInfoFrame.nTBorder;
	INT nBBorder=m_SkinAttribute.m_EncircleInfoFrame.nBBorder;
	INT nLBorder=m_SkinAttribute.m_EncircleInfoFrame.nLBorder;
	INT nRBorder=m_SkinAttribute.m_EncircleInfoFrame.nRBorder;

	//����λ��
	CRect rcChatInput;
	rcChatInput.left=nLBorder;
	rcChatInput.bottom=nHeight-nBBorder;
	rcChatInput.right=nWidth-USER_INFO_CX-nRBorder;
	rcChatInput.top=nHeight-CHAT_INPUT_CY-nBBorder;

	//��Ϣλ��
	CRect rcChatMessage;
	rcChatMessage.top=nTBorder;
	rcChatMessage.left=nLBorder;
	rcChatMessage.right=nWidth-USER_INFO_CX-nRBorder;
	rcChatMessage.bottom=nHeight-CHAT_INPUT_CY-nBBorder;

	//�ƶ��ؼ�
	HDWP hDwp=BeginDeferWindowPos(32);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;

	//������Դ
	m_EncircleChatInput.DeferWindowPos(&m_ChatInput,hDwp,rcChatInput);
	m_EncircleChatMessage.DeferWindowPos(&m_ChatMessage,hDwp,rcChatMessage);

	//�û��˵�
	DeferWindowPos(hDwp,m_btUserMenu,NULL,nWidth-123-nRBorder,nTBorder+13,0,0,uFlags|SWP_NOSIZE);

	//���찴ť
	CRect rcChatButton;
	m_btChatButton1.GetWindowRect(&rcChatButton);
	DeferWindowPos(hDwp,m_btChatButton1,NULL,rcChatInput.left+10,rcChatInput.top+3,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btChatButton2,NULL,rcChatInput.left+rcChatButton.Width()+10,rcChatInput.top+3,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btChatButton3,NULL,rcChatInput.left+rcChatButton.Width()*2+10,rcChatInput.top+3,0,0,uFlags|SWP_NOSIZE);

	//�����¼
	CRect rcChatHistory;
	m_btChatHistory.GetWindowRect(&rcChatHistory);
	DeferWindowPos(hDwp,m_btChatHistory,NULL,rcChatInput.right-rcChatHistory.Width()-6,rcChatInput.top+3,0,0,uFlags|SWP_NOSIZE);

	//���ư�ť
	CRect rcControl;
	CRect rcSendControl;
	m_btCancel.GetWindowRect(&rcControl);
	m_btSendControl.GetWindowRect(&rcSendControl);
	DeferWindowPos(hDwp,m_btSendControl,NULL,rcChatInput.right-rcSendControl.Width()-3,nHeight-rcControl.Height()-12,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btCancel,NULL,rcChatInput.right-rcSendControl.Width()-rcControl.Width()*2-13,nHeight-rcControl.Height()-12,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btSendChat,NULL,rcChatInput.right-rcSendControl.Width()-rcControl.Width()*1-8,nHeight-rcControl.Height()-12,0,0,uFlags|SWP_NOSIZE);

	//�ƶ�����
	EndDeferWindowPos(hDwp);

	//���½���
	RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE);

	return;
}

//������
VOID CDlgWhisper::SortUserIDData(DWORD dwUserID[], WORD wItemCount)
{
	//��������
	bool bSorted=true;
	DWORD dwTempUserID=0L;
	WORD wLastIndex=wItemCount-1;

	//�������
	do
	{
		//���ñ���
		bSorted=true;

		//�������
		for (WORD i=0;i<wLastIndex;i++)
		{
			//����λ��
			if (dwUserID[i]<dwUserID[i+1])
			{
				bSorted=false;
				dwTempUserID=dwUserID[i];
				dwUserID[i]=dwUserID[i+1];
				dwUserID[i+1]=dwTempUserID;
			}	
		}

		//��������
		wLastIndex--;

	} while (bSorted==false);

	return;
}

//�滭����
VOID CDlgWhisper::DrawFunctionArea(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight)
{
	//��������
	ASSERT(CSkinRenderManager::GetInstance()!=NULL);
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();

	//������Դ
	CBitImage ImageControlT;
	CBitImage ImageControlM;
	CBitImage ImageControlB;
	ImageControlT.LoadFromResource(AfxGetInstanceHandle(),IDB_WHISPER_CONTRUL_T);
	ImageControlM.LoadFromResource(AfxGetInstanceHandle(),IDB_WHISPER_CONTRUL_M);
	ImageControlB.LoadFromResource(AfxGetInstanceHandle(),IDB_WHISPER_CONTRUL_B);

	//��Ⱦ��Դ
	pSkinRenderManager->RenderImage(ImageControlT);
	pSkinRenderManager->RenderImage(ImageControlM);
	pSkinRenderManager->RenderImage(ImageControlB);

	//�û���Ϣ
	ImageControlT.BitBlt(pDC->m_hDC,nXPos,nYPos);
	ImageControlB.BitBlt(pDC->m_hDC,nXPos,nYPos+nHeight-ImageControlB.GetHeight());

	//�滭�м�
	INT nYStart=nYPos+ImageControlT.GetHeight();
	INT nYTerminate=nYPos+nHeight-ImageControlB.GetHeight();
	for (INT nYDrawPos=nYStart;nYDrawPos<nYTerminate;nYDrawPos+=ImageControlM.GetHeight())
	{
		INT nWidth=ImageControlM.GetWidth();
		ImageControlM.BitBlt(pDC->m_hDC,nXPos,nYDrawPos,nWidth,__min(nYTerminate-nYDrawPos,nWidth),0,0);
	}

	return;
}

//�滭�û�
VOID CDlgWhisper::DrawUserInfoArea(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight)
{
	//������Դ
	CBitImage ImageUserInfoT;
	CBitImage ImageUserInfoM;
	CBitImage ImageUserInfoB;
	ImageUserInfoT.LoadFromResource(AfxGetInstanceHandle(),IDB_WHISPER_USER_INFO_T);
	ImageUserInfoM.LoadFromResource(AfxGetInstanceHandle(),IDB_WHISPER_USER_INFO_M);
	ImageUserInfoB.LoadFromResource(AfxGetInstanceHandle(),IDB_WHISPER_USER_INFO_B);

	//�û���Ϣ
	ImageUserInfoT.BitBlt(pDC->m_hDC,nXPos,nYPos);
	ImageUserInfoB.BitBlt(pDC->m_hDC,nXPos,nYPos+nHeight-ImageUserInfoB.GetHeight());

	//�滭�м�
	INT nYStart=nYPos+ImageUserInfoT.GetHeight();
	INT nYTerminate=nYPos+nHeight-ImageUserInfoB.GetHeight();
	for (INT nYDrawPos=nYStart;nYDrawPos<nYTerminate;nYDrawPos+=ImageUserInfoM.GetHeight())
	{
		INT nWidth=ImageUserInfoM.GetWidth();
		ImageUserInfoM.BitBlt(pDC->m_hDC,nXPos,nYDrawPos,nWidth,__min(nYTerminate-nYDrawPos,nWidth),0,0);
	}

	//�û���Ϣ
	if (m_ClientUserItemArray.GetCount()>=2L)
	{
		//��������
		ASSERT(CFaceItemControl::GetInstance()!=NULL);
		IFaceItemControl * pIFaceItemControl=CFaceItemControl::GetInstance();

		//��������
		ASSERT(CUserItemElement::GetInstance()!=NULL);
		CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();

		//��ȡ�û�
		IClientUserItem * pIClientUserItem=m_ClientUserItemArray[1];
		tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

		//��Ϸ״̬
		BYTE cbUserStatus=pIClientUserItem->GetUserStatus();
		LPCTSTR pszUserStatus[]={TEXT("����"),TEXT("����"),TEXT("׼��"),TEXT("�Թ�"),TEXT("��Ϸ"),TEXT("����")};

		//�滭ͷ��
		if ((pCustomFaceInfo->dwDataSize!=0L)&&(pIClientUserItem->GetCustomID()!=0L))
		{
			pIFaceItemControl->DrawFaceNormal(pDC,nXPos+13,nYPos+14,pCustomFaceInfo->dwCustomFace);
		}
		else
		{
			pIFaceItemControl->DrawFaceNormal(pDC,nXPos+13,nYPos+14,pIClientUserItem->GetFaceID());
		}

		//���� DC
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,0));

		//������Ϣ
		DWORD dwExperience=pIClientUserItem->GetUserExperience();
		WORD wExperienceLevel=pUserItemElement->GetExperienceLevel(dwExperience);

		//�ȼ���Ϣ
		LPCTSTR pszKindName=m_pIGameLevelParser->GetKindName();
		LPCTSTR pszGameLevel=m_pIGameLevelParser->GetLevelDescribe(pIClientUserItem);

		//�û���Ϣ
		TCHAR szString[128]=TEXT("");
		LPCTSTR pszMember=m_pIUserOrderParser->GetMemberDescribe(pIClientUserItem->GetMemberOrder());
		LPCTSTR pszMaster=m_pIUserOrderParser->GetMasterDescribe(pIClientUserItem->GetMasterOrder());

		//��Ϣλ��
		CRect rcNickName(nXPos+80,nYPos+18,nXPos+nWidth-5,nYPos+18+12);
		CRect rcUserGameID(nXPos+80,nYPos+40,nXPos+nWidth-5,nYPos+40+12);

		//��Ϣλ��
		CRect rcGameLevel(nXPos+11,nYPos+74,nXPos+nWidth-5,nYPos+74+12);
		CRect rcExperience(nXPos+11,nYPos+95,nXPos+nWidth-5,nYPos+95+12);
		CRect rcGameScore(nXPos+11,nYPos+116,nXPos+nWidth-5,nYPos+116+12);
		CRect rcUserStatus(nXPos+11,nYPos+137,nXPos+nWidth-5,nYPos+137+12);
		CRect rcUnderWrite(nXPos+11,nYPos+158,nXPos+nWidth-5,nYPos+158+12);

		//�û���Ϣ
		_sntprintf(szString,CountArray(szString),TEXT("%s"),pIClientUserItem->GetNickName());
		pDC->DrawText(szString,lstrlen(szString),&rcNickName,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//�û���ʶ
		_sntprintf(szString,CountArray(szString),TEXT("%ld"),pIClientUserItem->GetGameID());
		pDC->DrawText(szString,lstrlen(szString),&rcUserGameID,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//��Ϸ�ȼ�
		_sntprintf(szString,CountArray(szString),TEXT("����%s [ %s ]"),pszGameLevel,pszKindName);
		pDC->DrawText(szString,lstrlen(szString),&rcGameLevel,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//�û�����
		_sntprintf(szString,CountArray(szString),TEXT("���飺%d ��"),wExperienceLevel);
		pDC->DrawText(szString,lstrlen(szString),&rcExperience,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//�����־
		CSize SizeExperience=pDC->GetTextExtent(szString,lstrlen(szString));
		pUserItemElement->DrawExperience(pDC,rcExperience.left+SizeExperience.cx+6,rcExperience.top-4L,dwExperience);

		//�û��ɼ�
		_sntprintf(szString,CountArray(szString),TEXT("�ɼ���%ld"),pIClientUserItem->GetUserScore());
		pDC->DrawText(szString,lstrlen(szString),&rcGameScore,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//��Ϸ״̬
		_sntprintf(szString,CountArray(szString),TEXT("״̬��%s"),pszUserStatus[cbUserStatus-1]);
		pDC->DrawText(szString,lstrlen(szString),&rcUserStatus,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//�û�ǩ��
		_sntprintf(szString,CountArray(szString),TEXT("ǩ����%s"),pIClientUserItem->GetUnderWrite());
		pDC->DrawText(szString,lstrlen(szString),&rcUnderWrite,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	}

	return;
}

//���ͱ���
bool CDlgWhisper::SendExpressionPacket(IClientUserItem * pIClientUserItem, WORD wItemIndex)
{
	//��������
	CMD_GR_C_WisperExpression WisperExpression;
	ZeroMemory(&WisperExpression,sizeof(WisperExpression));

	//������Ϣ
	WisperExpression.wItemIndex=wItemIndex;
	WisperExpression.dwTargetUserID=pIClientUserItem->GetUserID();

	//��������
	m_pITCPSocket->SendData(MDM_GR_USER,SUB_GR_WISPER_EXPRESSION,&WisperExpression,sizeof(WisperExpression));

	return true;
}

//��������
bool CDlgWhisper::SendWhisperChatPacket(IClientUserItem * pIClientUserItem, LPCTSTR pszChatString)
{
	//��������
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//������Ϣ
	CMD_GR_C_WisperChat WisperChat;
	lstrcpyn(WisperChat.szChatString,pszChatString,CountArray(WisperChat.szChatString));

	//��������
	WisperChat.dwChatColor=pParameterGlobal->m_crWhisperTX;
	WisperChat.dwTargetUserID=pIClientUserItem->GetUserID();
	WisperChat.wChatLength=CountStringBuffer(WisperChat.szChatString);

	//��������
	WORD wHeadSize=sizeof(WisperChat)-sizeof(WisperChat.szChatString);
	m_pITCPSocket->SendData(MDM_GR_USER,SUB_GR_WISPER_CHAT,&WisperChat,wHeadSize+WisperChat.wChatLength*sizeof(WisperChat.szChatString[0]));

	return true;
}

//���ͻỰ
bool CDlgWhisper::SendColloquyChatPacket(IClientUserItem * pIClientUserItem[], WORD wUserCount, LPCTSTR pszChatString)
{
	//��������
	CMD_GR_ColloquyChat ColloquyChat;
	ZeroMemory(&ColloquyChat,sizeof(ColloquyChat));

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//�����û�
	for (WORD i=0;i<wUserCount;i++)
	{
		//Ч������
		ASSERT(i<CountArray(ColloquyChat.dwTargetUserID));
		if (i>=CountArray(ColloquyChat.dwTargetUserID)) break;

		//��������
		ColloquyChat.dwTargetUserID[i]=pIClientUserItem[i]->GetUserID();
	}

	//��������
	SortUserIDData(ColloquyChat.dwTargetUserID,wUserCount);

	//�Ự��Ϣ
	lstrcpyn(ColloquyChat.szChatString,pszChatString,CountArray(ColloquyChat.szChatString));

	//�Ự����
	ColloquyChat.dwConversationID=m_dwConversationID;
	ColloquyChat.dwSendUserID=pGlobalUserData->dwUserID;
	ColloquyChat.dwChatColor=pParameterGlobal->m_crWhisperTX;
	ColloquyChat.wChatLength=CountStringBuffer(ColloquyChat.szChatString);

	//��������
	WORD wHeadSize=sizeof(ColloquyChat)-sizeof(ColloquyChat.szChatString);
	m_pITCPSocket->SendData(MDM_GR_USER,SUB_GR_COLLOQUY_CHAT,&ColloquyChat,wHeadSize+ColloquyChat.wChatLength*sizeof(ColloquyChat.szChatString[0]));

	return true;
}

//�û��˵�
VOID CDlgWhisper::OnBnClickedUserMenu()
{
	//�����˵�
	if (m_ClientUserItemArray.GetCount()>=2L)
	{
		ASSERT(m_ClientUserItemArray[1]!=NULL);
		m_pServerViewItem->TrackUserItemMenu(m_ClientUserItemArray[1]);
	}

	return;
}

//������Ϣ
VOID CDlgWhisper::OnBnClickedSendChat()
{
	//��ȡ��Ϣ
	CString strMessage;
	m_ChatInput.GetWindowText(strMessage);

	//������Ϣ
	if (strMessage.IsEmpty()==FALSE)
	{
		if (m_ClientUserItemArray.GetCount()==2L)
		{
			//˽����Ϣ
			ASSERT(m_ClientUserItemArray[1]!=NULL);
			SendWhisperChatPacket(m_ClientUserItemArray[1],strMessage);
		}
		else if (m_ClientUserItemArray.GetCount()>=3L)
		{
			//Ⱥ����Ϣ
			IClientUserItem * * pIClientUserItem=m_ClientUserItemArray.GetData();
			SendColloquyChatPacket(pIClientUserItem,(WORD)m_ClientUserItemArray.GetCount(),strMessage);
		}
		else
		{
			//������ʾ
			TCHAR szMessage[LEN_USER_CHAT+128]=TEXT("");
			_sntprintf(szMessage,CountArray(szMessage),TEXT("���λỰ�������û��Ѿ��뿪����%s����Ϣ�޷�����"),strMessage);

			//������Ϣ
			m_ChatMessage.InsertString(TEXT("\r\n"),0);
			m_ChatMessage.InsertSystemString(szMessage);
		}
	}
	else
	{
		//������Ϣ
		m_ChatMessage.InsertCustomString(TEXT("�������ݲ���Ϊ�գ�����������"),RGB(125,125,125));
	}

	//���ý���
	m_ChatInput.SetFocus();
	m_ChatInput.SetWindowText(TEXT(""));

	return;
}

//���Ϳ���
VOID CDlgWhisper::OnBnClickedSendControl()
{
	//����˵�
	CSkinMenu Menu;
	Menu.CreateMenu();

	//����˵�
	Menu.AppendMenu(IDM_SEND_BY_ENTER,TEXT("�� Enter ��������Ϣ"));
	Menu.AppendMenu(IDM_SEND_BY_CTRL_ENTER,TEXT("�� Ctrl+Enter ��������Ϣ"));

	//���ò˵�
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
	if (pParameterGlobal->m_bSendWhisperByEnter==true) Menu.CheckMenuItem(IDM_SEND_BY_ENTER,MF_BYCOMMAND|MF_CHECKED);
	if (pParameterGlobal->m_bSendWhisperByEnter==false) Menu.CheckMenuItem(IDM_SEND_BY_CTRL_ENTER,MF_BYCOMMAND|MF_CHECKED);

	//��ȡλ��
	CRect rcButton;
	m_btSendControl.GetWindowRect(&rcButton);

	//�����˵�
	Menu.TrackPopupMenu(rcButton.left,rcButton.bottom,this);

	return;
}

//���찴ť
VOID CDlgWhisper::OnBnClickedChatButton1()
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

	return;
}

//���찴ť
VOID CDlgWhisper::OnBnClickedChatButton2()
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
	CSkinMenu ColorMenu;
	ColorMenu.CreateMenu();

	//����˵�
	ColorMenu.AppendMenu(IDM_MORE_COLOR,TEXT("ѡ��������ɫ"));

	//��ɫѡ��
	ColorMenu.AppendSeparator();
	for (INT i=0;i<CountArray(ImageColor);i++) 
	{
		ColorMenu.AppendMenu(IDM_SELECT_CHAT_COLOR+i,ImageColor[i]);
		if (pParameterGlobal->m_crWhisperTX==crColor[i]) ColorMenu.CheckMenuItem(IDM_SELECT_CHAT_COLOR+i,MF_BYCOMMAND|MF_CHECKED);
	}

	//�����˵�
	CRect rcButton;
	m_btChatButton2.GetWindowRect(&rcButton);
	ColorMenu.TrackPopupMenu(rcButton.right,rcButton.top,this);

	return;
}

//���찴ť
VOID CDlgWhisper::OnBnClickedChatButton3()
{
	return;
}

//������ɫ
VOID CDlgWhisper::OnSelectMoreColor()
{
	//��������
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//������ɫ
	CColorDialog ColorDialog(pParameterGlobal->m_crWhisperTX,CC_FULLOPEN,this);
	if (ColorDialog.DoModal()==IDOK) pParameterGlobal->m_crWhisperTX=ColorDialog.GetColor();

	//���ý���
	m_ChatInput.SetFocus();

	return;
}

//ѡ����ɫ
VOID CDlgWhisper::OnSelectChatColor(UINT nCommandID)
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
	if (nIndex<CountArray(crColor)) pParameterGlobal->m_crWhisperTX=crColor[nIndex];

	//���ý���
	m_ChatInput.SetFocus();

	return;
}

//������Ϣ
VOID CDlgWhisper::OnNcDestroy()
{
	//״̬����
	m_bCreateFlag=false;

	//���ñ���
	m_dwConversationID=0L;
	m_ClientUserItemArray.RemoveAll();

	//ɾ������
	SafeDelete(m_pExpressionControl);

	__super::OnNcDestroy();
}

//λ����Ϣ
VOID CDlgWhisper::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//�����ؼ�
	RectifyControl(cx,cy);

	return;
}

//////////////////////////////////////////////////////////////////////////
