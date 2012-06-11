#include "Stdafx.h"
#include "GameFrame.h"
#include "DlgOption.h"
#include "GlobalUnits.h"
#include "DlgIndividualInfo.h"

//////////////////////////////////////////////////////////////////////////

//���ƫ��
#define FRAME_EXALTATION			24									//�����߶�

//��Ļλ��
#define LESS_SCREEN_X				1024								//��С���
#define LESS_SCREEN_Y				720									//��С�߶�

//��ť��ʾ
#define IDC_BT_MIN					1000								//��С��ť
#define IDC_BT_MAX					1001								//��С��ť
#define IDC_BT_CLOSE				1002								//�رհ�ť
#define IDC_BT_EXCHANGE_SINK		1003								//������ť
#define IDC_BT_BUTTON_1				1004								//���ܰ�ť
#define IDC_BT_BUTTON_2				1005								//���ܰ�ť
#define IDC_BT_BUTTON_3				1006								//���ܰ�ť
#define IDC_BT_BUTTON_4				1007								//���ܰ�ť
#define IDC_BT_BUTTON_5				1008								//���ܰ�ť

//�ؼ���ʶ
#define IDC_BT_PLAZA				1038								//��Ϸ�㳡
#define IDC_BT_SWITCH_ACCOUNTS		1009								//�л��ʺ�
#define IDC_BT_SYSTEM_OPTION		1010								//ϵͳ����
#define IDC_BT_SELF_OPTION			1011								//��������
#define IDC_BT_QUIT_GAME			1012								//�˳���Ϸ
#define IDC_BT_VIEW_ITEM			1013								//����ؼ�
#define IDC_BT_LIST_CONTROL_1		21015								//���ư�ť
#define IDC_BT_LIST_CONTROL_2		21016								//���ư�ť
#define IDC_BROWER_AD				1017								//�������
#define IDC_SPLITTER				1018								//��������
#define IDC_SERVER_ITEM_VIEW		1019								//�б����
#define IDC_COMPANION_LIST			1020								//�����б�
	
//��Ϣ����
#define WM_CLOSE_ROOM_VIEW_ITEM		WM_USER+10							//�ر���Ϣ
#define WM_SETUP_FINISH				WM_USER+100							//��װ���

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameFrame, CFrameWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_WM_NCHITTEST()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETTINGCHANGE()
	ON_WM_GETMINMAXINFO()
	ON_MESSAGE(WM_HOTKEY,OnHotKeyMessage)
	ON_MESSAGE(WM_SETUP_FINISH,OnMessageSetupFinish)
	ON_MESSAGE(WM_CLOSE_ROOM_VIEW_ITEM,OnCloseRoomViewItem)
	ON_WM_NCMOUSEMOVE()
//	ON_WM_MOUSEMOVE()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()



//////////////////////////////////////////////////////////////////////////
//���캯��
CGameFrame::CGameFrame():m_Splitter(VorSpliter)
{
	//���ñ���
	m_bRectify=false;
	m_bHideGameList=false;
	m_wShowListType=IDC_BT_LIST_CONTROL_1;
	m_pActiveViewItem=NULL;
	m_pRoomViewItemCreate=NULL;
	memset(m_pRoomViewItem,0,sizeof(m_pRoomViewItem));

	//����״̬
	m_bMaxShow=false;
	m_rcNormalSize.SetRect(0,0,0,0);

	return;
}

//��������
CGameFrame::~CGameFrame()
{
}

//��ť��Ϣ
bool CGameFrame::OnSplitterButton(CSkinSplitter * pSkinSplitter, CSkinButton * pSplitterButton)
{
	//�б���Ϣ
	ControlServerList(ServerListControl_Turn);

	return true;
}

//�������Ϣ
bool CGameFrame::OnSplitterEvent(CSkinSplitter * pSkinSplitter, UINT uSplitID, int nXPos, int nYPos)
{
	//��ʾ�ؼ�
	if (m_bHideGameList==true)
	{
		//���ñ���
		m_bRectify=true;
		m_bHideGameList=false;

		//�������
		m_UserInfoView.ShowWindow(SW_SHOW);
		OnCommand(m_wShowListType,0);

		//���ð�ť
		m_Splitter.ShowSplitterButton(GetPlatformRes().uBtHideGameList,GetResInstanceHandle());
	}

	//�����ؼ�
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());

	return true;
}

//�������
void __cdecl CGameFrame::OnTreeLeftClick(CListItem *pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl)
{
	//Ч�����
	if (pListItem==NULL) 
	{
		return;
	}

	//��Ϣ����
	switch (pListItem->GetItemGenre())
	{
	case ItemGenre_Inside:
		{
			//��������
			CListInside * pListInside=(CListInside *)pListItem;
			tagGameInside * pGameInside=pListInside->GetItemInfo();

			WebBrowse(TEXT("http://127.0.0.1:8086"),false);

			return;
		}
	case ItemGenre_Kind:		//��Ϸ����
		{
			//��������
			CListKind * pListKind=(CListKind *)pListItem;
			tagGameKind * pGameKind=pListKind->GetItemInfo();

			//���ӹ���
			TCHAR szRuleUrl[256]=TEXT("");
			_snprintf(szRuleUrl,sizeof(szRuleUrl),TEXT("http://127.0.0.1:8086/GameRule.asp?KindID=%ld"),pGameKind->wKindID);
			WebBrowse(szRuleUrl,true);

			return;
		}
	}

	return;
}

//�Ҽ�����
void __cdecl CGameFrame::OnTreeRightClick(CListItem *pListItem, HTREEITEM hTreeItem, CTreeCtrl *pTreeCtrl)
{
	return;
}

//���˫��
void __cdecl CGameFrame::OnTreeLeftDBClick(CListItem *pListItem, HTREEITEM hTreeItem, CTreeCtrl *pTreeCtrl)
{
	//Ч�����
	if(pListItem==NULL) return;

	//��Ϣ����
	switch (pListItem->GetItemGenre())
	{
	case ItemGenre_Kind:	//��Ϸ����
		{
			//��������
			CListKind * pListKind=(CListKind *)pListItem;
			tagGameKind * pGameKind=pListKind->GetItemInfo();

			//��װ�ж�
			if ((pListKind->m_bInstall==false)&&(pListKind->m_GameKind.dwMaxVersion!=0L))
			{
				//������ʾ
				TCHAR szBuffer[512]=TEXT("");
				_snprintf(szBuffer,sizeof(szBuffer),TEXT("��%s����û�а�װ�������Ƿ����أ�"),pGameKind->szKindName);

				//��ʾ��Ϣ
				if (ShowMessageBox(szBuffer,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1)==IDYES)
				{
					g_GlobalAttemper.DownLoadClient(pGameKind->szKindName,pGameKind->wKindID,true);
				}
			}

			return;
		}
	case ItemGenre_Server:	//��Ϸ����
		{
			CListServer * pListServer=(CListServer *)pListItem;
			CreateRoomViewItem(pListServer);
			return;
		}
	}

	return;
}

//�Ҽ�˫��
void __cdecl CGameFrame::OnTreeRightDBClick(CListItem *pListItem, HTREEITEM hTreeItem, CTreeCtrl *pTreeCtrl)
{
	return;
}

//ѡ��ı�
void __cdecl CGameFrame::OnTreeSelectChanged(CListItem *pListItem, HTREEITEM hTreeItem, CTreeCtrl *pTreeCtrl)
{
	return;
}

//����չ��
void __cdecl CGameFrame::OnTreeItemexpanded(CListItem *pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl)
{
	return;
}

//��Ϣ����
BOOL CGameFrame::PreTranslateMessage(MSG * pMsg)
{
	m_ToolTipCtrl.RelayEvent(pMsg);
	return __super::PreTranslateMessage(pMsg);
}

//�����
BOOL CGameFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	UINT nCommandID=LOWORD(wParam);

	//�л���ť
	if ((nCommandID>=IDC_BT_VIEW_ITEM)&&(nCommandID<(IDC_BT_VIEW_ITEM+CountArray(m_btViewItem))))
	{
		ActiveRoomViewItem(m_pRoomViewItem[nCommandID-IDC_BT_VIEW_ITEM]);
		return TRUE;
	}

	//��Ϣ����
	switch(nCommandID)
	{
	case IDC_BT_MIN	:					//��С����
		{
			ShowWindow(SW_MINIMIZE);

			return TRUE;
		}
	case IDC_BT_MAX		:				//��󻯴�
		{
			if (m_bMaxShow==true) RestoreWindow();
			else MaxSizeWindow();

			return TRUE;
		}
	case IDC_BT_BUTTON_1:				//���ܰ�ť
	case IDC_BT_BUTTON_3:				//���ܰ�ť
	case IDC_BT_BUTTON_4:				//���ܰ�ť
	case IDC_BT_BUTTON_5:				//���ܰ�ť
		{
			if(IDC_BT_BUTTON_1==nCommandID)WebBrowse(TEXT("http://127.0.0.1:8086/"),true);
			if(IDC_BT_BUTTON_3==nCommandID)WebBrowse(TEXT("http://127.0.0.1:8086/PayBuy.asp"),true);
			if(IDC_BT_BUTTON_4==nCommandID)WebBrowse(TEXT("http://127.0.0.1:8086/user/ExChange.asp"),true);
			if(IDC_BT_BUTTON_5==nCommandID)WebBrowse(TEXT("http://127.0.0.1:8086/"),true);

			return TRUE;
		}
	case IDC_BT_BUTTON_2:				//�ϴ�ͷ��
		{
			//��������
			if ( m_DlgCustomFace.m_hWnd == NULL )
			{
				m_DlgCustomFace.Create(IDD_CUSTOM_FACE, this);
			}

			//��ʾ����
			m_DlgCustomFace.CenterWindow();
			m_DlgCustomFace.ShowWindow(SW_SHOW);
			m_DlgCustomFace.SetActiveWindow();
			m_DlgCustomFace.SetForegroundWindow();

			return TRUE;
		}
	case IDC_BT_PLAZA	:				//������ť
		{
			ActivePlazaViewItem();

			return TRUE;
		}
	case IDC_BT_EXCHANGE_SINK:			//������ť
		{
			g_GlobalUnits.m_PlatformResourceModule->SetResourceType(RESOURCE_RED);
			UpdateSkinResource();
			m_DlgGamePlaza.UpdateSkinResource();
			m_UserInfoView.UpdateSkinResource();
			for (int i=0;i<CountArray(m_pRoomViewItem);i++)
			{
				if (m_pRoomViewItem[i]==NULL) break;
				m_pRoomViewItem[i]->UpdateSkinResource();
			}

			return TRUE;
		}
	case IDC_BT_CLOSE	:				//�رշ���
	case IDC_BT_QUIT_GAME	:			//�˳���Ϸ
		{
			CloseCurrentViewItem();

			return TRUE;
		}
	case IDC_BT_SYSTEM_OPTION	:		//ϵͳ����
		{
			//ϵͳ����
			ShowSystemOption();

			return TRUE;
		}
	case IDC_BT_SELF_OPTION:			//��������
		{
			CDlgIndividualInfo DlgIndividualInfo;
			DlgIndividualInfo.DoModal();

			return TRUE;
		}
	case IDC_BT_SWITCH_ACCOUNTS :		//�л���ť
		{
			//״̬�ж�
			tagGlobalUserData & GlobalUserData=g_GlobalUnits.GetGolbalUserData();
			if (GlobalUserData.dwUserID==0L) return TRUE;

			//�л�ѯ��
			if (m_pRoomViewItem[0]!=NULL)
			{
				const TCHAR szMessage[]=TEXT("�л��û��ʺţ�����ر�������Ϸ���䣬ȷʵҪ�л��û��ʺ��� ");
				int iResult=ShowMessageBox(szMessage,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2);
				if (iResult!=IDYES) return TRUE;
			}
			else
			{
				const TCHAR szMessage[]=TEXT("ȷʵҪע����ǰ�û����л��û��ʺ��� ");
				int iResult=ShowMessageBox(szMessage,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2);
				if (iResult!=IDYES) return TRUE;
			}

			//�رշ���
			ActivePlazaViewItem();
			CloseAllRoomViewItem();

			//ɾ����¼
			g_GlobalUnits.DeleteUserCookie();

			//���ñ���
			memset(&GlobalUserData,0,sizeof(GlobalUserData));

			//Ͷ����Ϣ
			m_DlgGamePlaza.SendLogonMessage();

			return TRUE;
		}
	case IDC_BT_LIST_CONTROL_1:			//�б�ť
	case IDC_BT_LIST_CONTROL_2:			//�б�ť
		{
			//�����б�
			if(m_bHideGameList)
			{
				//���ذ�ť
				m_btListControl1.ShowWindow(SW_HIDE);
				m_btListControl2.ShowWindow(SW_HIDE);
				m_ServerItemView.ShowWindow(SW_HIDE);
				m_UserCompanionList.ShowWindow(SW_HIDE);
			}
			//��ʾ�б�
			else
			{
				//���ñ���
				m_wShowListType = nCommandID;

				//���ð�ť
				m_btListControl1.ShowWindow(SW_SHOW);
				m_btListControl2.ShowWindow(SW_SHOW);
				UINT uControl1=((IDC_BT_LIST_CONTROL_1==m_wShowListType)?GetPlatformRes().uFrameServiceBtShow1:GetPlatformRes().uFrameServiceBtHide1);
				UINT uControl2=((IDC_BT_LIST_CONTROL_2==m_wShowListType)?GetPlatformRes().uFrameServiceBtShow2:GetPlatformRes().uFrameServiceBtHide2);
				m_btListControl1.SetButtonImage(uControl1,GetResInstanceHandle(),false);
				m_btListControl2.SetButtonImage(uControl2,GetResInstanceHandle(),false);

				//�ж���ʾ
				m_ServerItemView.ShowWindow((m_wShowListType==IDC_BT_LIST_CONTROL_1)?SW_SHOW:SW_HIDE);
				m_UserCompanionList.ShowWindow((m_wShowListType==IDC_BT_LIST_CONTROL_2)?SW_SHOW:SW_HIDE);
			}

			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//��ʾ��ҳ
void CGameFrame::WebBrowse(LPCTSTR lpszUrl, bool bForceBrowse)
{
	//����㳡
	ActivePlazaViewItem();

	//����ҳ
	if (&m_DlgGamePlaza!=NULL) 
	{
		m_DlgGamePlaza.WebBrowse(lpszUrl,bForceBrowse);
	}

	return;
}

//ϵͳ����
void CGameFrame::ShowSystemOption()
{
	//��ȡ����
	CGameOption * pGameOption=NULL;
	CServerOption * pServerOption=NULL;
	if (m_pActiveViewItem!=&m_DlgGamePlaza)
	{
		for (int i=0;i<CountArray(m_pRoomViewItem);i++)
		{
			if (m_pRoomViewItem[i]==NULL) break;
			if (m_pActiveViewItem==m_pRoomViewItem[i])
			{
				pGameOption=m_pRoomViewItem[i]->GetGameOption();
				pServerOption=m_pRoomViewItem[i]->GetServerOption();
				break;
			}
		}
	}

	//��ʾ����
	CDlgOption DlgOption(pGameOption,pServerOption);
	INT_PTR iResult=DlgOption.DoModal();
	if (iResult==IDOK)
	{
		for (int i=0;i<CountArray(m_pRoomViewItem);i++)
		{
			if (m_pRoomViewItem[i]==NULL) break;
			m_pRoomViewItem[i]->SendUserRule();
		}
	}

	return;
}

//��ʾ��Ϣ
int CGameFrame::ShowMessageBox(LPCTSTR pszMessage, UINT nType)
{
	INT nResult=ShowInformationEx(pszMessage,0,nType,TEXT("��Ϸ����"));
	return nResult;
}

//�б����
bool CGameFrame::ControlServerList(enServerListControl ServerListControl)
{
	//λ�ÿ���
	bool bHideGameList=false;
	switch (ServerListControl)
	{
	case ServerListControl_Hide: { bHideGameList=true; break; }
	case ServerListControl_Show: { bHideGameList=false; break; }
	case ServerListControl_Turn: { bHideGameList=!m_bHideGameList; break; }
	}

	//�����ж�
	if (bHideGameList==m_bHideGameList) return false;

	//���ñ���
	m_bRectify=false;
	m_bHideGameList=bHideGameList;

	//�������
	m_UserInfoView.ShowWindow((bHideGameList==false)?SW_SHOW:SW_HIDE);
	OnCommand(m_wShowListType,0);

	//���ð�ť
	UINT uControlId = ((m_bHideGameList)?GetPlatformRes().uBtShowGameList:GetPlatformRes().uBtHideGameList);
	m_Splitter.ShowSplitterButton(uControlId,GetResInstanceHandle());

	//��������
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());

	return true;
}

//��Դ���
HINSTANCE CGameFrame::GetResInstanceHandle()
{
	return g_GlobalUnits.m_PlatformResourceModule->GetResInstance();
}

//��ȡ��Դ
tagPlatformFrameImage CGameFrame::GetPlatformRes()
{
	return g_GlobalUnits.m_PlatformFrameImage;
}

//������Դ
bool CGameFrame::UpdateSkinResource()
{
	//��ȡ��Դ
	tagPlatformFrameImage & PlatformFrameImage=GetPlatformRes();
	HINSTANCE hResInstance=GetResInstanceHandle();

	//������Դ
	m_EncircleFrame.ImageML.SetLoadInfo(PlatformFrameImage.uFrameML,hResInstance);
	m_EncircleFrame.ImageMR.SetLoadInfo(PlatformFrameImage.uFrameMR,hResInstance);
	m_EncircleFrame.ImageTL.SetLoadInfo(PlatformFrameImage.uFrameTL,hResInstance);
	m_EncircleFrame.ImageTM.SetLoadInfo(PlatformFrameImage.uFrameTM,hResInstance);
	m_EncircleFrame.ImageTR.SetLoadInfo(PlatformFrameImage.uFrameTR,hResInstance);
	m_EncircleFrame.ImageBL.SetLoadInfo(PlatformFrameImage.uFrameBL,hResInstance);
	m_EncircleFrame.ImageBM.SetLoadInfo(PlatformFrameImage.uFrameBM,hResInstance);
	m_EncircleFrame.ImageBR.SetLoadInfo(PlatformFrameImage.uFrameBR,hResInstance);

	//�������б�
	m_EncircleServer.ImageML.SetLoadInfo(PlatformFrameImage.uServerML,hResInstance);
	m_EncircleServer.ImageMR.SetLoadInfo(PlatformFrameImage.uServerMR,hResInstance);
	m_EncircleServer.ImageTL.SetLoadInfo(PlatformFrameImage.uServerTL,hResInstance);
	m_EncircleServer.ImageTM.SetLoadInfo(PlatformFrameImage.uServerTM,hResInstance);
	m_EncircleServer.ImageTR.SetLoadInfo(PlatformFrameImage.uServerTR,hResInstance);
	m_EncircleServer.ImageBL.SetLoadInfo(PlatformFrameImage.uServerBL,hResInstance);
	m_EncircleServer.ImageBM.SetLoadInfo(PlatformFrameImage.uServerBM,hResInstance);
	m_EncircleServer.ImageBR.SetLoadInfo(PlatformFrameImage.uServerBR,hResInstance);

	//��ȡ��Ϣ
	CSkinAide::GetEncircleInfo(m_ImageInfoFrame,m_EncircleFrame);
	CSkinAide::GetEncircleInfo(m_ImageInfoServer,m_EncircleServer);

	//������ؼ�
	UINT uControlId = ((m_bHideGameList)?PlatformFrameImage.uBtShowGameList:PlatformFrameImage.uBtHideGameList);
	m_Splitter.ShowSplitterButton(uControlId,hResInstance);

	//����ͼƬ
	m_btMin.SetButtonImage(PlatformFrameImage.uBtFrameMin,hResInstance,false);
	m_btMax.SetButtonImage((m_bMaxShow)?PlatformFrameImage.uBtFrameResore:PlatformFrameImage.uBtFrameMax,hResInstance,false);
	m_btClose.SetButtonImage(PlatformFrameImage.uBtFrameClose,hResInstance,false);
	m_btExChangeSkin.SetButtonImage(PlatformFrameImage.uBtFrameClose,hResInstance,false);
	m_btButton1.SetButtonImage(PlatformFrameImage.uFrameNavigation[0],hResInstance,false);
	m_btButton2.SetButtonImage(PlatformFrameImage.uFrameNavigation[1],hResInstance,false);
	m_btButton3.SetButtonImage(PlatformFrameImage.uFrameNavigation[2],hResInstance,false);
	m_btButton4.SetButtonImage(PlatformFrameImage.uFrameNavigation[3],hResInstance,false);
	m_btButton5.SetButtonImage(PlatformFrameImage.uFrameNavigation[4],hResInstance,false);
	m_btSwitchUser.SetButtonImage(PlatformFrameImage.uBtFrameSwitch,hResInstance,false);
	m_btGlobalOption.SetButtonImage(PlatformFrameImage.uBtFrameOption,hResInstance,false);
	m_btSelfOption.SetButtonImage(PlatformFrameImage.uBtFrameSelfSet,hResInstance,false);

	//�б�ť
	UINT uControl1=((IDC_BT_LIST_CONTROL_1==m_wShowListType)?PlatformFrameImage.uFrameServiceBtShow1:PlatformFrameImage.uFrameServiceBtHide1);
	UINT uControl2=((IDC_BT_LIST_CONTROL_2==m_wShowListType)?PlatformFrameImage.uFrameServiceBtShow2:PlatformFrameImage.uFrameServiceBtHide2);
	m_btListControl1.SetButtonImage(uControl1,hResInstance,false);
	m_btListControl2.SetButtonImage(uControl2,hResInstance,false);

	//����ͼƬ
	int i=0;
	for (;i<CountArray(m_pRoomViewItem);i++)
	{
		if(m_pActiveViewItem==m_pRoomViewItem[i])break;
	}
	CSkinButton * pTempButton[]={&m_btViewItem[0],&m_btViewItem[1],&m_btViewItem[2],&m_btGamePlaza};
	ASSERT((CountArray(m_btViewItem)+1)==CountArray(pTempButton));
	for (WORD j=0;j<CountArray(pTempButton);j++)
	{
		if(i==j)
		{
			pTempButton[j]->SetTextColor(RGB(0,0,0),RGB(133,215,255),true);
			pTempButton[j]->SetButtonImage(PlatformFrameImage.uBtFrameItemActive,hResInstance,false);
		}
		else
		{
			pTempButton[j]->SetTextColor(RGB(245,245,245),RGB(20,20,20),true);
			pTempButton[j]->SetButtonImage(PlatformFrameImage.uBtFrameItemNormal,hResInstance,false);
		}
	}
	return true;
}

//����㳡
void CGameFrame::ActivePlazaViewItem()
{
	//��ȡ����
	HWND hWndActive=NULL;
	if (m_pActiveViewItem!=NULL) hWndActive=m_pActiveViewItem->GetWindowHandle();
	if (m_DlgGamePlaza.GetSafeHwnd()!=hWndActive)
	{
		m_pActiveViewItem=&m_DlgGamePlaza;
		m_DlgGamePlaza.ShowWindow(SW_SHOW);
		if (hWndActive!=NULL) ::ShowWindow(hWndActive,SW_HIDE);

		//��ʾ�б�
		if(!ControlServerList(ServerListControl_Show))
		{
			//��������
			CRect rcClient;
			GetClientRect(&rcClient);
			RectifyControl(rcClient.Width(),rcClient.Height());
		}
	}

	return;
}

//�����
void CGameFrame::ActiveRoomViewItem(CRoomViewItem * pRoomViewItem)
{
	//Ч�����
	ASSERT(pRoomViewItem!=NULL);
	if (pRoomViewItem==NULL) return;

	//�жϴ���
	if (m_pRoomViewItemCreate==pRoomViewItem)
	{
		m_pRoomViewItemCreate=NULL;
		MoveMemory(m_pRoomViewItem+1,m_pRoomViewItem,sizeof(m_pRoomViewItem[0])*(CountArray(m_pRoomViewItem)-1));
		m_pRoomViewItem[0]=pRoomViewItem;
	}

	//�����
	HWND hWndActive=NULL;
	if (m_pActiveViewItem!=NULL) hWndActive=m_pActiveViewItem->GetWindowHandle();
	if (pRoomViewItem->GetSafeHwnd()!=hWndActive)
	{
		m_pActiveViewItem=pRoomViewItem;
		pRoomViewItem->ShowWindow(SW_SHOW);
		if (hWndActive!=NULL) ::ShowWindow(hWndActive,SW_HIDE);

		//�����б�
		if(!ControlServerList(ServerListControl_Hide))
		{
			//��������
			CRect rcClient;
			GetClientRect(&rcClient);
			RectifyControl(rcClient.Width(),rcClient.Height());
		}
	}

	//���¿���
	UpdateControlButton();

	return;
}

//���ҷ���
CRoomViewItem * CGameFrame::SearchRoomViewItem(WORD wKindID, WORD wServerID)
{
	tagGameServer * pGameServer=NULL;
	for (int i=0;i<CountArray(m_pRoomViewItem);i++)
	{
		if (m_pRoomViewItem[i]==NULL) return NULL;
		pGameServer=m_pRoomViewItem[i]->GetServerInfo();
		if ((pGameServer->wKindID==wKindID)&&(pGameServer->wServerID==wServerID)) return m_pRoomViewItem[i];
	}

	return NULL;
}

//���뷿��
CRoomViewItem * CGameFrame::CreateRoomViewItem(CListServer * pListServer)
{
	//�ж�״̬
	tagGlobalUserData & GlobalUserData=g_GlobalUnits.GetGolbalUserData();
	if (GlobalUserData.dwUserID==0L)
	{
		ShowMessageBox(TEXT("�㻹û�е�¼�����ȵ�¼��Ϸ�㳡��"),MB_ICONQUESTION);
		return NULL;
	}

	//Ч�����
	ASSERT(pListServer!=NULL);
	CListKind * pListKind=pListServer->GetListKind();
	tagGameKind * pGameKind=pListKind->GetItemInfo();
	tagGameServer * pGameServer=pListServer->GetItemInfo();

	//�ж�����
	if (m_pRoomViewItemCreate!=NULL)
	{
		tagGameServer * pGameServerCreate=m_pRoomViewItemCreate->GetServerInfo();
		if ((pGameServer->wKindID==pGameServerCreate->wKindID)&&
			(pGameServer->wServerID==pGameServerCreate->wServerID))
			return NULL;
	}

	//Ѱ�ҷ���
	CRoomViewItem * pRoomViewItem=SearchRoomViewItem(pGameServer->wKindID,pGameServer->wServerID);
	if (pRoomViewItem!=NULL) 
	{
		ActiveRoomViewItem(pRoomViewItem);
		return pRoomViewItem;
	}

	//���뷿����Ŀ
	if (m_pRoomViewItem[CountArray(m_pRoomViewItem)-1]!=NULL)
	{
		ShowMessageBox(TEXT("�������Ϸ����̫���ˣ���رղ�����Ϸ�������ԣ�"),MB_ICONINFORMATION);
		return NULL;
	}

	//��ȡ�汾
	CWinFileInfo WinFileInfo;
	if (WinFileInfo.OpenWinFile(pGameKind->szProcessName)==false)
	{
		TCHAR szBuffer[512]=TEXT("");
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("��%s����û�а�װ�������Ƿ����أ�"),pGameKind->szKindName);
		int nResult=ShowMessageBox(szBuffer,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1);
		if (nResult==IDYES) g_GlobalAttemper.DownLoadClient(pGameKind->szKindName,pGameKind->wKindID,true);
		return NULL;
	}

	//��ȡ�汾
	DWORD dwFileVerMS=0L,dwFileVerLS=0L;
	WinFileInfo.GetFileVersion(dwFileVerMS,dwFileVerLS);

	//�汾����
	BYTE cbFileVer1=(BYTE)(HIWORD(dwFileVerMS));
	BYTE cbFileVer2=(BYTE)(LOWORD(dwFileVerMS));
	BYTE cbFileVer3=(BYTE)(HIWORD(dwFileVerLS));
	BYTE cbFileVer4=(BYTE)(LOWORD(dwFileVerLS));
	BYTE cbListVer1=(BYTE)(LOWORD(LOWORD(pGameKind->dwMaxVersion)));
	BYTE cbListVer2=(BYTE)(HIBYTE(LOWORD(pGameKind->dwMaxVersion)));
	BYTE cbListVer3=(BYTE)(LOBYTE(HIWORD(pGameKind->dwMaxVersion)));
	BYTE cbListVer4=(BYTE)(HIBYTE(HIWORD(pGameKind->dwMaxVersion)));

	//�жϰ汾
	if ((cbFileVer1!=cbListVer1)||(cbFileVer2!=cbListVer2)||(cbFileVer3!=cbListVer3))
	{
		TCHAR szBuffer[512]=TEXT("");
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("��%s���Ѿ�����Ϊ %ld.%ld.%ld.%ld �汾����İ汾���ܼ���ʹ�ã������Ƿ����أ�"),pGameKind->szKindName,
			cbListVer1,cbListVer2,cbListVer3,cbListVer4);
		int nResult=ShowMessageBox(szBuffer,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1);
		if (nResult==IDYES) g_GlobalAttemper.DownLoadClient(pGameKind->szKindName,pGameKind->wKindID,true);
		return NULL;
	}

	//���ݰ汾
	if (cbFileVer4!=cbListVer4)
	{
		TCHAR szBuffer[512]=TEXT("");
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("��%s���Ѿ�����Ϊ %ld.%ld.%ld.%ld �汾�������Ƿ�����������"),pGameKind->szKindName,
			cbListVer1,cbListVer2,cbListVer3,cbListVer4);
		int nResult=ShowMessageBox(szBuffer,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1);
		if (nResult==IDYES) 
		{
			g_GlobalAttemper.DownLoadClient(pGameKind->szKindName,pGameKind->wKindID,true);
			return NULL;
		}
	}

	//��������
	try
	{
		pRoomViewItem=new CRoomViewItem;
		if (pRoomViewItem==NULL) return NULL;
		pRoomViewItem->Create(IDD_GAME_ROOM,AfxGetMainWnd());
		if (pRoomViewItem->InitRoomViewItem(pListServer)==false) throw TEXT("��Ϸ�����ʼ��ʧ�ܣ�");
	}
	catch (...) { SafeDelete(pRoomViewItem); }
	if (pRoomViewItem==NULL)
	{
		ShowMessageBox(TEXT("��Ϸ���䴴��ʧ��"),MB_ICONQUESTION);
		return NULL;
	}

	//������Ϸ
	bool bSuccess=pRoomViewItem->ConnectGameServer();
	if (bSuccess==false)
	{
		pRoomViewItem->DestroyWindow();
		SafeDelete(pRoomViewItem);
		return NULL;
	}

	//���ñ���
	m_pRoomViewItemCreate=pRoomViewItem;

	//������ʾ
	CString strBuffer;
	strBuffer.Format(TEXT("�������ӷ��䣬���Ժ�..."));
	g_GlobalAttemper.ShowStatusMessage(strBuffer,pRoomViewItem);
	
	return m_pRoomViewItemCreate;
}

//�رշ���
void CGameFrame::CloseRoomViewItem(IRoomViewItem * pIRoomViewItem)
{
	//Ч�����
	ASSERT(pIRoomViewItem!=NULL);
	if (pIRoomViewItem==NULL) return;

	//���ñ���
	if (m_pRoomViewItemCreate!=pIRoomViewItem)
	{
		//�ر���ʾ
		if (pIRoomViewItem->QueryCloseRoom()==false) return;

		//Ѱ�ҷ���
		for (int i=0;i<CountArray(m_pRoomViewItem);i++)
		{
			if (m_pRoomViewItem[i]==NULL) break;
			if (m_pRoomViewItem[i]==pIRoomViewItem) 
			{
				//�ƶ�����
				if ((i+1)!=CountArray(m_pRoomViewItem))
				{
					int nMoveSize=(CountArray(m_pRoomViewItem)-i-1)*sizeof(m_pRoomViewItem[0]);
					MoveMemory(m_pRoomViewItem+i,m_pRoomViewItem+i+1,nMoveSize);
				}
				m_pRoomViewItem[CountArray(m_pRoomViewItem)-1]=NULL;

				//������ͼ
				if (m_pActiveViewItem==pIRoomViewItem)
				{
					int nActivePos=CountArray(m_pRoomViewItem);
					for (int i=CountArray(m_pRoomViewItem)-1;i>=0;i--)
					{
						if (m_pRoomViewItem[i]!=NULL)
						{
							nActivePos=i;
							break;
						}
					}
					if (nActivePos==CountArray(m_pRoomViewItem)) ActivePlazaViewItem();
					else ActiveRoomViewItem(m_pRoomViewItem[nActivePos]);
				}

				//���¿���
				UpdateControlButton();

				break;
			}
		}
	}
	else m_pRoomViewItemCreate=NULL;

	//�رշ���
	pIRoomViewItem->CloseRoomViewItem();
	PostMessage(WM_CLOSE_ROOM_VIEW_ITEM,(WPARAM)pIRoomViewItem,(LPARAM)pIRoomViewItem);

	return;
}

//�رշ���
void CGameFrame::CloseAllRoomViewItem()
{
	//�رշ���
	IRoomViewItem * pIRoomViewItem=NULL;
	for (int i=0;i<CountArray(m_pRoomViewItem);i++)
	{
		if (m_pRoomViewItem[i]==NULL) break;

		//�رշ���
		pIRoomViewItem=QUERY_OBJECT_PTR_INTERFACE(m_pRoomViewItem[i],IRoomViewItem);
		pIRoomViewItem->CloseRoomViewItem();
		PostMessage(WM_CLOSE_ROOM_VIEW_ITEM,(WPARAM)pIRoomViewItem,(LPARAM)pIRoomViewItem);

		//���ñ���
		m_pRoomViewItem[i]=NULL;
	}

	//�رմ�������
	if (m_pRoomViewItemCreate!=NULL) 
	{
		//�رշ���
		g_GlobalAttemper.DestroyStatusWnd(NULL);
		m_pRoomViewItemCreate->CloseRoomViewItem();
		pIRoomViewItem=QUERY_OBJECT_PTR_INTERFACE(m_pRoomViewItemCreate,IRoomViewItem);
		PostMessage(WM_CLOSE_ROOM_VIEW_ITEM,(WPARAM)pIRoomViewItem,(LPARAM)pIRoomViewItem);

		//���ñ���
		m_pRoomViewItemCreate=NULL;
	}

	//���¿���
	UpdateControlButton();

	return;
}

//�رյ�ǰ
void CGameFrame::CloseCurrentViewItem()
{
	//�����ж�
	ASSERT(m_pActiveViewItem!=NULL);
	if (m_pActiveViewItem!=&m_DlgGamePlaza)
	{
		IRoomViewItem * pIRoomViewItem=QUERY_OBJECT_PTR_INTERFACE(m_pActiveViewItem,IRoomViewItem);
		ASSERT(pIRoomViewItem!=NULL);
		CloseRoomViewItem(pIRoomViewItem);
	}
	else AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0); 

	return;
}

//���¿���
void CGameFrame::UpdateControlButton()
{
	//���°�ť
	for (int i=0;i<CountArray(m_btViewItem);i++)
	{
		if (m_pRoomViewItem[i]!=NULL)
		{
			tagGameKind * pGameKind=m_pRoomViewItem[i]->GetKindInfo();
			m_btViewItem[i].ShowWindow(SW_SHOW);
			m_btViewItem[i].SetWindowText(pGameKind->szKindName);
		}
		else m_btViewItem[i].ShowWindow(SW_HIDE);
	}

	return;
}

//�����ؼ�
void CGameFrame::RectifyControl(int nWidth, int nHeight)
{
	//״̬�ж�
	if (m_DlgGamePlaza.m_bInitDialog==false) return;
	if ((nWidth==0)||(nHeight==0)) return;

	//��������
	const int nSplitterWidth=__max(6,m_Splitter.GetButtonSize().cx);

	//��ȡλ��
	CRect rcSplitter;
	m_Splitter.GetWindowRect(&rcSplitter);
	ScreenToClient(&rcSplitter);

	//���������
	if(!m_bHideGameList)
	{
		int nLessPos=nWidth/4,nMaxPos=nWidth/2;
		if (m_bRectify==false)
		{
			m_bRectify=true;
			rcSplitter.left=nLessPos;
			rcSplitter.right=rcSplitter.left+nSplitterWidth;
		}

		//���ò�ַ�Χ
		m_Splitter.SetSplitterRange(nLessPos,nMaxPos);
		if ((nWidth!=0)&&(nHeight!=0))
		{
			if (rcSplitter.left<nLessPos) 
			{
				rcSplitter.left=nLessPos;
				rcSplitter.right=rcSplitter.left+nSplitterWidth;
			}
			if (rcSplitter.right>nMaxPos)
			{
				rcSplitter.right=nMaxPos;
				rcSplitter.left=rcSplitter.right-nSplitterWidth;
			}
		}
	}
	else
	{
		//���λ��
		rcSplitter.left=0;
		rcSplitter.right=rcSplitter.left+nSplitterWidth;
	}

	//λ�õ���
	rcSplitter.top=m_ImageInfoFrame.nTBorder-FRAME_EXALTATION;
	rcSplitter.bottom=nHeight-m_ImageInfoFrame.nBBorder;

	//�����ؼ�
	HDWP hDwp=BeginDeferWindowPos(32);
	UINT uUserTitleHeight=m_UserInfoView.GetTitleHeight();
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;
	DeferWindowPos(hDwp,m_Splitter,NULL,rcSplitter.left,rcSplitter.top,rcSplitter.Width(),rcSplitter.Height(),uFlags);

	//�б�ؼ�
	if(!m_bHideGameList)
	{
		DeferWindowPos(hDwp,m_ServerItemView,NULL,m_ImageInfoFrame.nLBorder+m_ImageInfoServer.nLBorder,m_ImageInfoFrame.nTBorder+m_ImageInfoServer.nTBorder+uUserTitleHeight-FRAME_EXALTATION,
			rcSplitter.right-m_ImageInfoServer.nLBorder-m_ImageInfoServer.nRBorder-m_ImageInfoFrame.nLBorder,nHeight-m_ImageInfoServer.nTBorder-m_ImageInfoServer.nBBorder-m_ImageInfoFrame.nTBorder-uUserTitleHeight-m_ImageInfoFrame.nBBorder+FRAME_EXALTATION,uFlags);
		DeferWindowPos(hDwp,m_UserCompanionList,NULL,m_ImageInfoFrame.nLBorder+m_ImageInfoServer.nLBorder,m_ImageInfoFrame.nTBorder+m_ImageInfoServer.nTBorder+uUserTitleHeight-FRAME_EXALTATION,
			rcSplitter.right-m_ImageInfoServer.nLBorder-m_ImageInfoServer.nRBorder-m_ImageInfoFrame.nLBorder,nHeight-m_ImageInfoServer.nTBorder-m_ImageInfoServer.nBBorder-m_ImageInfoFrame.nTBorder-uUserTitleHeight-m_ImageInfoFrame.nBBorder+FRAME_EXALTATION,uFlags);
	}

	//��Ϣ�ؼ�
	DeferWindowPos(hDwp,m_UserInfoView,NULL,m_ImageInfoFrame.nLBorder,m_ImageInfoFrame.nTBorder-FRAME_EXALTATION,rcSplitter.right-m_ImageInfoFrame.nLBorder,uUserTitleHeight,uFlags);

	//��ȡλ��
	CRect rcItemRect;
	m_btGamePlaza.GetWindowRect(&rcItemRect);

	//�ƶ��ؼ�
	INT nYPos = m_ImageInfoFrame.nTBorder-FRAME_EXALTATION;
	DeferWindowPos(hDwp,m_btGamePlaza,NULL,rcSplitter.right+5,nYPos,0,0,uFlags|SWP_NOSIZE);
	for (int i=0;i<CountArray(m_btViewItem);i++) DeferWindowPos(hDwp,m_btViewItem[i],NULL,rcSplitter.right+rcItemRect.Width()+rcItemRect.Width()*i+5,nYPos,0,0,uFlags|SWP_NOSIZE);

	//ҳ��ؼ�
	HWND hWndActive=m_pActiveViewItem->GetWindowHandle();
	DeferWindowPos(hDwp,hWndActive,NULL,rcSplitter.right,nYPos+rcItemRect.Height(),nWidth-m_ImageInfoFrame.nRBorder-rcSplitter.right,
		nHeight-(nYPos+rcItemRect.Height())-m_ImageInfoFrame.nBBorder,uFlags);

	//�����ؼ�
	DeferWindowPos(hDwp,m_btMin,NULL,nWidth-102,3,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btMax,NULL,nWidth-72,3,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btClose,NULL,nWidth-42,3,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btExChangeSkin,NULL,nWidth-132,3,0,0,uFlags|SWP_NOSIZE);

	//�б�ť
	m_btListControl1.GetWindowRect(&rcItemRect);
	INT nY = m_ImageInfoFrame.nTBorder-FRAME_EXALTATION+uUserTitleHeight+5;
	DeferWindowPos(hDwp,m_btListControl1,NULL,m_ImageInfoFrame.nLBorder+5,nY,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btListControl2,NULL,m_ImageInfoFrame.nLBorder+5,nY+rcItemRect.Height()+1,0,0,uFlags|SWP_NOSIZE);

	//��ťλ��
	CRect rcButton;
	m_btButton1.GetWindowRect(&rcButton);

	//�������
	int nEndPos=105;
	int nBeginPos=560;
	int nWindth=nWidth-nBeginPos-rcButton.Width()*5-nEndPos;
	int nButtonSpace=__min((nWidth-nBeginPos-rcButton.Width()*5-nEndPos)/4,30);

	//���ؼ�
	// modify by һ��
	//DeferWindowPos(hDwp,m_BrowerAD,NULL,300,8,250,52,uFlags);

	//������ť
	DeferWindowPos(hDwp,m_btButton1,NULL,nWidth-rcButton.Width()*5-nButtonSpace*4-nEndPos,6,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btButton2,NULL,nWidth-rcButton.Width()*4-nButtonSpace*3-nEndPos,6,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btButton3,NULL,nWidth-rcButton.Width()*3-nButtonSpace*2-nEndPos,6,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btButton4,NULL,nWidth-rcButton.Width()*2-nButtonSpace*1-nEndPos,6,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btButton5,NULL,nWidth-rcButton.Width()*1-nButtonSpace*0-nEndPos,6,0,0,uFlags|SWP_NOSIZE);

	//��ȡλ��
	CRect rcButtonRect;
	m_btSelfOption.GetWindowRect(&rcButtonRect);

	//�ƶ��ؼ�
	nYPos = m_ImageInfoFrame.nTBorder-FRAME_EXALTATION-4;
	CButton * pButton[]={/*&m_btQuitGame,*/&m_btSelfOption,&m_btGlobalOption,&m_btSwitchUser};
	for (int i=0;i<CountArray(pButton);i++) DeferWindowPos(hDwp,pButton[i]->m_hWnd,NULL,nWidth-(rcButtonRect.Width()+6)*(i+1)-8,nYPos,0,0,uFlags|SWP_NOSIZE);

	//��������
	LockWindowUpdate();
	EndDeferWindowPos(hDwp);
	UnlockWindowUpdate();

	//����Ƥ��
	int i=0;
	for (;i<CountArray(m_btViewItem);i++)
	{
		if(m_pActiveViewItem==m_pRoomViewItem[i])break;
	}
	CSkinButton * pTempButton[]={&m_btViewItem[0],&m_btViewItem[1],&m_btViewItem[2],&m_btGamePlaza};
	ASSERT((CountArray(m_btViewItem)+1)==CountArray(pTempButton));
	for (WORD j=0;j<CountArray(pTempButton);j++)
	{
		if(i==j)
		{
			pTempButton[j]->SetTextColor(RGB(0,0,0),RGB(133,215,255),true);
			pTempButton[j]->SetButtonImage(GetPlatformRes().uBtFrameItemActive,GetResInstanceHandle(),false);
		}
		else
		{
			pTempButton[j]->SetTextColor(RGB(245,245,245),RGB(20,20,20),true);
			pTempButton[j]->SetButtonImage(GetPlatformRes().uBtFrameItemNormal,GetResInstanceHandle(),false);
		}
	}

	return;
}

//��󴰿�
bool CGameFrame::MaxSizeWindow()
{
	//״̬�ж�
	if (m_bMaxShow==false)
	{
		//Ĭ��λ��
		GetWindowRect(&m_rcNormalSize);

		//���ð�ť
		m_btMax.SetButtonImage(GetPlatformRes().uBtFrameResore,GetResInstanceHandle(),false);

		//��ȡλ��
		CRect rcArce;
		SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

		//�ƶ�����
		m_bMaxShow=true;
		LockWindowUpdate();
		SetWindowPos(NULL,rcArce.left-2,rcArce.top-2,rcArce.Width()+4,rcArce.Height()+4,SWP_NOZORDER);
		UnlockWindowUpdate();
	}

	return true;
}

//��ԭ����
bool CGameFrame::RestoreWindow()
{
	//״̬�ж�
	if (m_bMaxShow==true)
	{
		//���ð�ť
		m_btMax.SetButtonImage(GetPlatformRes().uBtFrameMax,GetResInstanceHandle(),false);

		//�ƶ�����
		m_bMaxShow=false;
		LockWindowUpdate();
		SetWindowPos(NULL,m_rcNormalSize.left,m_rcNormalSize.top,m_rcNormalSize.Width(),m_rcNormalSize.Height(),SWP_NOZORDER);
		UnlockWindowUpdate();
	}

	return true;
}

//������Ϣ
int CGameFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//��������
	ModifyStyle(WS_CAPTION|WS_BORDER,0,0);

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//���ñ���
	CString strTitle;
	strTitle.LoadString(AfxGetInstanceHandle(),IDS_MAIN_TITLE);
	SetWindowText(strTitle);

	//���ؼ�
	m_BrowerAD.Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this,IDC_BROWER_AD,NULL);
	m_BrowerAD.Navigate(TEXT("http://127.0.0.1:8086/AD/GamePlazaAD.asp"),NULL,NULL,NULL,NULL);

	//������ؼ�
	m_Splitter.Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this,IDC_SPLITTER,NULL);
	m_Splitter.SetSplitterSink(this);

	//�û���ͼ
	m_UserInfoView.Create(IDD_USER_INFO,this);
	m_UserInfoView.ShowWindow(SW_SHOW);

	//��������ͼ
	m_ServerItemView.Create(WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this,IDC_SERVER_ITEM_VIEW);
	m_ServerItemView.InitServerItemView(this);
	g_GlobalUnits.m_ServerListManager.InitServerListManager(&m_ServerItemView);

	//�����б�
	m_wShowListType = IDC_BT_LIST_CONTROL_1;
	m_UserCompanionList.Create(WS_CHILD,CRect(0,0,0,0),this,IDC_COMPANION_LIST);

	//������ť
	m_btMin.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_MIN);
	m_btMax.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_MAX);
	m_btClose.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_CLOSE);
	m_btExChangeSkin.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_EXCHANGE_SINK);
	m_btExChangeSkin.ShowWindow(SW_HIDE);
	m_btButton1.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_BUTTON_1);
	m_btButton2.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_BUTTON_2);
	m_btButton3.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_BUTTON_3);
	m_btButton4.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_BUTTON_4);
	m_btButton5.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_BUTTON_5);
	m_btGamePlaza.Create(TEXT("��Ϸ����"),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_PLAZA);
	m_btSelfOption.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_SELF_OPTION);
	m_btSwitchUser.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_SWITCH_ACCOUNTS);
	m_btGlobalOption.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_SYSTEM_OPTION);
	m_btListControl1.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_LIST_CONTROL_1);
	m_btListControl2.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_LIST_CONTROL_2);
	for (WORD i=0;i<CountArray(m_btViewItem);i++)
	{
		CRect rcCreate(0,0,0,0);
		m_btViewItem[i].Create(NULL,WS_CHILD,rcCreate,this,IDC_BT_VIEW_ITEM+i);
	}

	//Ƥ����Դ
	UpdateSkinResource();

	//������ʾ�ؼ�
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(GetDlgItem(IDC_BT_MIN),TEXT("��С����Ϸ�㳡"));
	m_ToolTipCtrl.AddTool(GetDlgItem(IDC_BT_CLOSE),TEXT("�˳���Ϸ�㳡"));

	//�����ؼ�
	m_DlgGamePlaza.Create(IDD_GAME_PLAZA,this);
	ActivePlazaViewItem();

	//ע���ȼ�
	g_GlobalUnits.RegisterHotKey(m_hWnd,IDI_HOT_KEY_BOSS,g_GlobalOption.m_wBossHotKey);

	//����λ��
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

	//��ʾ����
	MaxSizeWindow();
	ShowWindow(SW_SHOW);

	//Ĭ��λ��
	m_rcNormalSize.left=(rcArce.Width()-LESS_SCREEN_X)/2;
	m_rcNormalSize.top=(rcArce.Height()-LESS_SCREEN_Y)/2;
	m_rcNormalSize.right=(rcArce.Width()+LESS_SCREEN_X)/2;
	m_rcNormalSize.bottom=(rcArce.Height()+LESS_SCREEN_Y)/2;

	return 0;
}

//�滭��Ϣ
void CGameFrame::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��Ϸ�б�
	if (!m_bHideGameList)
	{
		//��ȡλ��
		CRect rcSplitter;
		m_Splitter.GetWindowRect(&rcSplitter);
		ScreenToClient(&rcSplitter);

		//�滭�б�
		CRect rcServer;
		rcServer.right=rcSplitter.right;
		rcServer.left=m_ImageInfoFrame.nLBorder;
		rcServer.bottom=rcClient.Height()-m_ImageInfoFrame.nBBorder;
		rcServer.top=m_ImageInfoFrame.nTBorder+m_UserInfoView.GetTitleHeight()-FRAME_EXALTATION;
		CSkinAide::DrawEncircleFrame(&dc,rcServer,m_EncircleServer);
	}

	//�滭���
	CSkinAide::DrawEncircleFrame(&dc,rcClient,m_EncircleFrame);

	return;
}

//�滭����
BOOL CGameFrame::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

//���øı�
void CGameFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	__super::OnSettingChange(uFlags, lpszSection);

	//������ܴ�С
	CRect rcClient;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcClient,SPIF_SENDCHANGE);
	MoveWindow(&rcClient,TRUE);

	return;
}

//�����Ϣ
void CGameFrame::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	//ȥ���˵�
	LONG lStyle = GetWindowLong(m_hWnd,GWL_STYLE);
	if((lStyle&WS_SYSMENU)!=0)ModifyStyle(WS_SYSMENU,0,0);

	__super::OnNcMouseMove(nHitTest, point);
}

//�����¼�
void CGameFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	//���Ӳ˵�
	LONG lStyle = GetWindowLong(m_hWnd,GWL_STYLE);
	if((lStyle&WS_SYSMENU)==0)ModifyStyle(0,WS_SYSMENU,0);

	__super::OnActivate(nState, pWndOther, bMinimized);
}

//�ر���Ϣ
void CGameFrame::OnClose()
{
	//�ر���ʾ
	if (g_GlobalUnits.GetGolbalUserData().dwUserID!=0L)
	{
		int nCode=ShowInformation(TEXT("��ȷʵҪ�ر���Ϸ�㳡��"),0,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION);
		if (nCode!=IDYES) return;
	}

	//���ؽ���
	ShowWindow(SW_HIDE);

	//�رշ���
	CloseAllRoomViewItem();

	//��������
	g_GlobalOption.SaveOptionParameter();
	g_GlobalUnits.m_CompanionManager->SaveCompanion();

	__super::OnClose();
}

//��ȡ���λ��
void CGameFrame::OnGetMinMaxInfo(MINMAXINFO FAR * lpMMI) 
{
	//���ñ���
	lpMMI->ptMinTrackSize.x=LESS_SCREEN_X;
	lpMMI->ptMinTrackSize.y=LESS_SCREEN_Y;

	__super::OnGetMinMaxInfo(lpMMI);
}

//λ����Ϣ
void CGameFrame::OnSize(UINT nType, int cx, int cy) 
{
	__super::OnSize(nType, cx, cy);

	//���������
	RectifyControl(cx,cy);

	return;
}

//��������
LRESULT CGameFrame::OnNcHitTest(CPoint Point)
{
	//��ť����
	if (m_bMaxShow==false)
	{
		//��ȡλ��
		CRect rcClient;
		GetClientRect(&rcClient);

		//ת��λ��
		CPoint ClientPoint=Point;
		ScreenToClient(&ClientPoint);

		//����λ��
		if (ClientPoint.x<=m_ImageInfoFrame.nRBorder)
		{
			if (ClientPoint.y<=m_ImageInfoFrame.nTBorder-FRAME_EXALTATION) return HTTOPLEFT;
			if (ClientPoint.y>=(rcClient.Height()-m_ImageInfoFrame.nRBorder)) return HTBOTTOMLEFT;
			return HTLEFT;
		}

		//����λ��
		if (ClientPoint.x>=(rcClient.Width()-m_ImageInfoFrame.nRBorder))
		{
			if (ClientPoint.y<=m_ImageInfoFrame.nTBorder-FRAME_EXALTATION) return HTTOPRIGHT;
			if (ClientPoint.y>=(rcClient.Height()-m_ImageInfoFrame.nRBorder)) return HTBOTTOMRIGHT;
			return HTRIGHT;
		}

		//����λ��
		if (ClientPoint.y<=m_ImageInfoFrame.nRBorder) return HTTOP;
		if (ClientPoint.y>=(rcClient.Height()-m_ImageInfoFrame.nRBorder)) return HTBOTTOM;
	}

	return __super::OnNcHitTest(Point);
}

//�����Ϣ
void CGameFrame::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//ģ�ⰴ����
	if ((m_bMaxShow==false)&&(Point.y<=m_ImageInfoFrame.nTBorder-FRAME_EXALTATION))
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}

	return;
}

//�����Ϣ
void CGameFrame::OnLButtonDblClk(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDblClk(nFlags,Point);

	//״̬�ж�
	if (Point.y>m_ImageInfoFrame.nTBorder-FRAME_EXALTATION) return;

	//���ƴ���
	if (m_bMaxShow==true) RestoreWindow();
	else MaxSizeWindow();

	return;
}

//��װ���
LRESULT CGameFrame::OnMessageSetupFinish(WPARAM wParam, LPARAM lParam)
{
	g_GlobalUnits.m_ServerListManager.UpdateGameKind((WORD)wParam);
	return 0;
}

//�ȼ���Ϣ
LRESULT CGameFrame::OnHotKeyMessage(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case IDI_HOT_KEY_BOSS:		//�ϰ��ȼ�
		{
			//��������
			bool bBossCome=(IsWindowVisible()==FALSE)?false:true;

			//���ش���
			if (bBossCome==false)
			{
				//��ԭ����
				ShowWindow(SW_RESTORE);
				ShowWindow(SW_SHOW);

				//�ö�����
				SetActiveWindow();
				BringWindowToTop();
				SetForegroundWindow();
			}
			else
			{
				//���ش���
				ShowWindow(SW_MINIMIZE);
				ShowWindow(SW_HIDE);
			}

			//������Ϣ
			for (INT_PTR i=0;i<MAX_SERVER;i++)
			{
				if (m_pRoomViewItem[i]!=NULL)
				{
					m_pRoomViewItem[i]->NotifyBossCome(bBossCome);
				}
			}

			return 0;
		}
	}

	return 0;
}

//�رշ���
LRESULT CGameFrame::OnCloseRoomViewItem(WPARAM wParam, LPARAM lParam)
{
	//�ͷŷ���
	IRoomViewItem * pIRoomViewItem=(IRoomViewItem *)wParam;
	if (pIRoomViewItem!=NULL) pIRoomViewItem->Release();

	return 0;
}

//////////////////////////////////////////////////////////////////////////
