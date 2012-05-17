#include "StdAfx.h"
#include "Resource.h"
#include "WndChatControl.h"

//////////////////////////////////////////////////////////////////////////

//��ť��ʶ
#define IDC_CHAT_CLOSE				100									//�رհ�ť
#define IDC_CHAT_CONTROL			101									//���ư�ť

//�û�ѡ��
#define MAX_HISTOY_USER				8									//�����Ŀ
#define IDM_CANCEL_CHAT_USER		(WM_USER+100)						//��ʷ�û�
#define IDM_HISTORY_CHAT_USER		(WM_USER+101)						//��ʷ�û�

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CWndChatControl, CWnd)

	//ϵͳ��Ϣ
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()

	//��ť��Ϣ
	ON_BN_CLICKED(IDC_CHAT_CLOSE, OnBnClickedClose)
	ON_BN_CLICKED(IDC_CHAT_CONTROL, OnBnClickedControl)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CWndChatControl::CWndChatControl()
{
	//���ñ���
	m_pIClientUserItem=NULL;
	m_pIChatControlSink=NULL;

	return;
}

//��������
CWndChatControl::~CWndChatControl()
{
}

//�����
BOOL CWndChatControl::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//ȡ������
	if (LOWORD(wParam)==IDM_CANCEL_CHAT_USER)
	{
		SetChatTargetUser(NULL);
		return TRUE;
	}

	//��ʷ�û�
	if ((LOWORD(wParam)>=IDM_HISTORY_CHAT_USER)&&(LOWORD(wParam)<(IDM_HISTORY_CHAT_USER+MAX_HISTOY_USER)))
	{
		//ѡ���û�
		INT_PTR nIndex=LOWORD(wParam)-(IDM_HISTORY_CHAT_USER+MAX_HISTOY_USER);

		//ѡ���û�
		ASSERT(nIndex<m_ChatUserItemArray.GetCount());
		if (nIndex<m_ChatUserItemArray.GetCount()) SetChatTargetUser(m_ChatUserItemArray[nIndex]);

		return TRUE;
	}

	return __super::OnCommand(wParam,lParam);
}

//��ȡ�û�
DWORD CWndChatControl::GetChatTargetUserID()
{
	if (m_pIClientUserItem!=NULL)
	{
		return m_pIClientUserItem->GetUserID();
	}

	return 0L;
}

//��ȡ�û�
IClientUserItem * CWndChatControl::GetChatTargetUserItem()
{
	return m_pIClientUserItem;
}

//ɾ���û�
VOID CWndChatControl::DeleteUserItem(IClientUserItem * pIClientUserItem)
{
	//ɾ���û�
	for (INT_PTR i=0;i<m_ChatUserItemArray.GetCount();i++)
	{
		//��ȡ�û�
		IClientUserItem * pIChatUserItem=m_ChatUserItemArray[i];
		if (pIChatUserItem->GetUserID()==pIClientUserItem->GetUserID())
		{
			m_ChatUserItemArray.RemoveAt(i);
			break;
		}
	}

	//��ǰ�û�
	if (m_pIClientUserItem==pIClientUserItem)
	{
		//���ñ���
		m_pIClientUserItem=NULL;

		//�¼�֪ͨ
		ASSERT(m_pIChatControlSink!=NULL);
		m_pIChatControlSink->OnChangeChatTarget(m_pIClientUserItem);

		//���½���
		if (m_hWnd!=NULL)
		{
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
		}
	}

	return;
}

//�����û�
VOID CWndChatControl::SetChatTargetUser(IClientUserItem * pIClientUserItem)
{
	//���ñ���
	m_pIClientUserItem=pIClientUserItem;

	//��ʷ�û�
	if (pIClientUserItem!=NULL)
	{
		//ɾ���û�
		for (INT_PTR i=0;i<m_ChatUserItemArray.GetCount();i++)
		{
			//��ȡ�û�
			IClientUserItem * pIChatUserItem=m_ChatUserItemArray[i];
			if (pIChatUserItem->GetUserID()==pIClientUserItem->GetUserID())
			{
				m_ChatUserItemArray.RemoveAt(i);
			}
		}

		//�����û�
		m_ChatUserItemArray.Add(pIClientUserItem);

		//�������
		if (m_ChatUserItemArray.GetCount()>MAX_HISTOY_USER)
		{
			m_ChatUserItemArray.RemoveAt(0);
		}
	}

	//�¼�֪ͨ
	ASSERT(m_pIChatControlSink!=NULL);
	m_pIChatControlSink->OnChangeChatTarget(m_pIClientUserItem);

	//���½���
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//���ýӿ�
VOID CWndChatControl::SetChatControlSink(IChatControlSink * pIChatControlSink)
{
	//���ñ���
	m_pIChatControlSink=pIChatControlSink;

	return;
}

//������Ϣ
INT CWndChatControl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//��ȡλ��
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//������ť
	CRect rcButton(0,0,0,0);
	m_btClose.Create(NULL,WS_CHILD|WS_VISIBLE,rcButton,this,IDC_CHAT_CLOSE);
	m_btControl.Create(NULL,WS_CHILD|WS_VISIBLE,rcButton,this,IDC_CHAT_CONTROL);

	//���ð�ť
	HINSTANCE hInstance=GetModuleHandle(SHARE_CONTROL_DLL_NAME);
	m_btClose.SetButtonImage(IDB_BT_CHAT_CLOSE,hInstance,true,false);
	m_btControl.SetButtonImage(IDB_BT_CHAT_HISTORY,hInstance,true,false);

	//����λ��
	CBitImage ImageBackGroud;
	ImageBackGroud.LoadFromResource(hInstance,IDB_CHAT_CONTROL_BACK);
	SetWindowPos(NULL,0,0,rcWindow.Width(),ImageBackGroud.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	return 0;
}

//�滭����
VOID CWndChatControl::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//������Դ
	CBitImage ImageBackGroud;
	ImageBackGroud.LoadFromResource(GetModuleHandle(SHARE_CONTROL_DLL_NAME),IDB_CHAT_CONTROL_BACK);

	//��Ⱦ����
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();
	if (pSkinRenderManager!=NULL) pSkinRenderManager->RenderImage(ImageBackGroud);

	//�滭����
	for (INT nXPos=0;nXPos<rcClient.Width();nXPos+=ImageBackGroud.GetWidth())
	{
		ImageBackGroud.BitBlt(dc,nXPos,0);
	}

	//�滭��Ϣ
	if (m_pIClientUserItem!=NULL)
	{
		//����λ��
		CRect rcTargetUser;
		rcTargetUser.top=rcClient.top+6;
		rcTargetUser.left=rcClient.left+22;
		rcTargetUser.bottom=rcClient.bottom;
		rcTargetUser.right=rcClient.right-40;

		//���� DC
		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(RGB(10,10,10));
		dc.SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

		//�滭��Ϣ
		TCHAR szString[64]=TEXT("");
		_sntprintf(szString,CountArray(szString),TEXT("Ŀ���û���%s��"),m_pIClientUserItem->GetNickName());
		dc.DrawText(szString,lstrlen(szString),&rcTargetUser,DT_LEFT|DT_VCENTER|DT_END_ELLIPSIS|DT_SINGLELINE);
	}

	return;
}

//������Ϣ
VOID CWndChatControl::OnDestroy()
{
	__super::OnDestroy();

	//���ñ���
	m_pIClientUserItem=NULL;

	return;
}

//λ�ñ仯
VOID CWndChatControl::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//���ư�ť
	if (m_btControl.m_hWnd!=NULL)
	{
		CRect rcButton;
		m_btControl.GetWindowRect(&rcButton);
		m_btControl.SetWindowPos(NULL,3,cy-rcButton.Height(),0,0,SWP_NOSIZE|SWP_NOZORDER);
	}

	//�رհ�ť
	if (m_btClose.m_hWnd!=NULL)
	{
		CRect rcButton;
		m_btClose.GetWindowRect(&rcButton);
		m_btClose.SetWindowPos(NULL,cx-rcButton.Width()-3,cy-rcButton.Height(),0,0,SWP_NOSIZE|SWP_NOZORDER);
	}

	return;
}

//�رհ�ť
VOID CWndChatControl::OnBnClickedClose()
{
	//ȡ������
	ASSERT(m_pIChatControlSink!=NULL);
	if (m_pIChatControlSink!=NULL) m_pIChatControlSink->OnChangeChatTarget(NULL);

	return;
}

//���ư�ť
VOID CWndChatControl::OnBnClickedControl()
{
	//Ч��״̬
	ASSERT((m_pIClientUserItem!=NULL)&&(m_ChatUserItemArray.GetCount()>0));
	if ((m_pIClientUserItem==NULL)||(m_ChatUserItemArray.GetCount()==0)) return;

	//����˵�
	CSkinMenu ChatUserMenu;
	ChatUserMenu.CreateMenu();

	//����˵�
	if (m_ChatUserItemArray.GetCount()>1)
	{
		for (INT_PTR i=0;i<m_ChatUserItemArray.GetCount();i++)
		{
			//��ȡ�û�
			IClientUserItem * pIClientUserItem=m_ChatUserItemArray[i];
			if (pIClientUserItem->GetUserID()==m_pIClientUserItem->GetUserID()) continue;

			//����˵�
			TCHAR szMenuString[64]=TEXT("");
			_sntprintf(szMenuString,CountArray(szMenuString),TEXT("�� [ %s ] ��̸"),pIClientUserItem->GetNickName());

			//����˵�
			ChatUserMenu.AppendMenu(IDM_HISTORY_CHAT_USER+(UINT)i,szMenuString);
		}
	}
	else ChatUserMenu.AppendMenu(0,TEXT("û����ʷ�������"),MF_GRAYED);

	//ȡ������
	if (m_pIClientUserItem!=NULL)
	{
		//����˵�
		TCHAR szMenuString[64]=TEXT("");
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("ȡ���� [ %s ] ��̸"),m_pIClientUserItem->GetNickName());

		//����˵�
		ChatUserMenu.AppendSeparator();
		ChatUserMenu.AppendMenu(IDM_CANCEL_CHAT_USER,szMenuString);
	}
	
	//�����˵�
	ChatUserMenu.TrackPopupMenu(this);
	
	return;
}

//////////////////////////////////////////////////////////////////////////
