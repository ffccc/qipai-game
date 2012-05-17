#include "Stdafx.h"
#include "GamePlaza.h"
#include "DlgEnquire.h"
#include "DlgSearchUser.h"
#include "PlatformFrame.h"

//////////////////////////////////////////////////////////////////////////////////

//Բ�Ǵ�С
#define ROUND_CX					4									//Բ�ǿ��
#define ROUND_CY					4									//Բ�Ǹ߶�

//��Ļλ��
#define BORAD_SIZE					5									//�߿��С
#define CAPTION_SIZE				79									//�����С

//��Ļ����
#define LESS_LIST_CX				285									//�б���
#define SPLITTER_WIDTH				8									//��ֿ��

//��Ļ����
#define LESS_SCREEN_CY				740									//��С�߶�
#define LESS_SCREEN_CX				1024								//��С���

//////////////////////////////////////////////////////////////////////////////////

//���ư�ť
#define IDC_MIN						100									//��С��ť
#define IDC_MAX						101									//���ť
#define IDC_SKIN					102									//Ƥ����ť
#define IDC_CLOSE					103									//�رհ�ť

//���ư�ť
#define IDC_CONTROL					110									//���ư�ť
#define IDC_LOCK_LIST				111									//������ť

//�ؼ���ʶ
#define IDC_SERVER_LIST				300									//��Ϸ�б�
#define IDC_WEB_PUBLICIZE			301									//����ؼ�
#define IDC_SKIN_SPLITTER			302									//��ֿؼ�
#define IDC_SYSTEM_TRAY_ICON		303									//����ͼ��

//�ؼ���ʶ
#define IDC_VIEW_ITEM_CTRL			310									//������
#define IDC_USER_INFO_CTRL			311									//�û���Ϣ

//��Χ��ť
#define IDC_PLATFORM				400									//ƽ̨��ť
#define IDC_NAVIGATION				500									//������ť

//////////////////////////////////////////////////////////////////////////////////

//������Ŀ
#define MAX_SERVER_COUNT			5									//������Ŀ

//ʱ���ʶ
#define IDI_FLASH_TRAY_ICON			10									//����ͼ��

//////////////////////////////////////////////////////////////////////////////////
//��̬����

CPlatformFrame * CPlatformFrame::m_pPlatformFrame=NULL;					//���ָ��

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPlatformFrame, CFrameWnd)

	//ϵͳ��Ϣ
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_NCHITTEST()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_GETMINMAXINFO()
	ON_WM_SETTINGCHANGE()

	//��ť��Ϣ
	ON_BN_CLICKED((IDC_PLATFORM+0), OnBnClickedPlatform1)
	ON_BN_CLICKED((IDC_PLATFORM+1), OnBnClickedPlatform2)
	ON_BN_CLICKED((IDC_PLATFORM+2), OnBnClickedPlatform3)

	//�Զ���Ϣ
	ON_MESSAGE(WM_HOTKEY, OnMessageHotKey)
	ON_MESSAGE(WM_TRAY_ICON, OnMessageTrayIcon)
	ON_MESSAGE(WM_PLATFORM_EVENT, OnMessagePlatformEvent)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CListEncircle::CListEncircle()
{
}

//��������
CListEncircle::~CListEncircle()
{
}

//�滭����
bool CListEncircle::PreDrawEncircleImage(tagEncircleBMP & EncircleImage)
{
	//��������
	CDC * pDCL=CDC::FromHandle(EncircleImage.ImageTL.GetDC());
	CDC * pDCM=CDC::FromHandle(EncircleImage.ImageTM.GetDC());
	CDC * pDCR=CDC::FromHandle(EncircleImage.ImageTR.GetDC());

	//������Դ
	CPngImage ListTitleL;
	CPngImage ListTitleM;
	CPngImage ListTitleR;
	ListTitleL.LoadImage(AfxGetInstanceHandle(),TEXT("LIST_TITLE_L"));
	ListTitleM.LoadImage(AfxGetInstanceHandle(),TEXT("LIST_TITLE_M"));
	ListTitleR.LoadImage(AfxGetInstanceHandle(),TEXT("LIST_TITLE_R"));

	//������Դ
	ListTitleL.DrawImage(pDCL,0,0);
	ListTitleM.DrawImage(pDCM,0,0);
	ListTitleR.DrawImage(pDCR,0,0);

	//�ͷ���Դ
	EncircleImage.ImageTL.ReleaseDC();
	EncircleImage.ImageTM.ReleaseDC();
	EncircleImage.ImageTR.ReleaseDC();

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CFrameEncircle::CFrameEncircle()
{
}

//��������
CFrameEncircle::~CFrameEncircle()
{
}

//�滭����
bool CFrameEncircle::PreDrawEncircleImage(tagEncircleBMP & EncircleImage)
{
	//��������
	CDC * pDC=CDC::FromHandle(EncircleImage.ImageTL.GetDC());

	//������Դ
	CPngImage StationLogo;
	StationLogo.LoadImage(AfxGetInstanceHandle(),TEXT("PLATFORM_LOGO"));

	//�����־
	StationLogo.DrawImage(pDC,10,5);
	EncircleImage.ImageTL.ReleaseDC();

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CPlatformFrame::CPlatformFrame()
{
	//�������
	m_bMaxShow=false;
	m_bRectify=false;
	m_rcNormalSize.SetRect(0,0,0,0);

	//״̬����
	m_bWhisperIcon=false;
	m_bHideGameList=false;
	m_bLockGameList=false;
	m_bAutoMenuEnable=FALSE;

	//�������
	m_pServerViewItem=NULL;

	//�������
	m_MissionManager.InsertMissionItem(&m_MissionList);
	m_MissionManager.InsertMissionItem(&m_MissionLogon);

	//ƽ̨����
	ASSERT(m_pPlatformFrame==NULL);
	if (m_pPlatformFrame==NULL) m_pPlatformFrame=this;

	//���ýӿ�
	CCustomFaceManager * pCustomFaceManager=CCustomFaceManager::GetInstance();
	pCustomFaceManager->SetCustomFaceEvent(QUERY_OBJECT_PTR_INTERFACE(this,IUnknownEx));

	//������Դ
	tagEncircleResource	EncircleItem;
	EncircleItem.pszImageTL=MAKEINTRESOURCE(IDB_ITEM_FRAME_TL);
	EncircleItem.pszImageTM=MAKEINTRESOURCE(IDB_ITEM_FRAME_TM);
	EncircleItem.pszImageTR=MAKEINTRESOURCE(IDB_ITEM_FRAME_TR);
	EncircleItem.pszImageML=MAKEINTRESOURCE(IDB_ITEM_FRAME_ML);
	EncircleItem.pszImageMR=MAKEINTRESOURCE(IDB_ITEM_FRAME_MR);
	EncircleItem.pszImageBL=MAKEINTRESOURCE(IDB_ITEM_FRAME_BL);
	EncircleItem.pszImageBM=MAKEINTRESOURCE(IDB_ITEM_FRAME_BM);
	EncircleItem.pszImageBR=MAKEINTRESOURCE(IDB_ITEM_FRAME_BR);
	m_ItemEncircle.InitEncircleResource(EncircleItem,AfxGetInstanceHandle());

	//�б���Դ
	tagEncircleResource	EncircleList;
	EncircleList.pszImageTL=MAKEINTRESOURCE(IDB_SERVER_LIST_TL);
	EncircleList.pszImageTM=MAKEINTRESOURCE(IDB_SERVER_LIST_TM);
	EncircleList.pszImageTR=MAKEINTRESOURCE(IDB_SERVER_LIST_TR);
	EncircleList.pszImageML=MAKEINTRESOURCE(IDB_SERVER_LIST_ML);
	EncircleList.pszImageMR=MAKEINTRESOURCE(IDB_SERVER_LIST_MR);
	EncircleList.pszImageBL=MAKEINTRESOURCE(IDB_SERVER_LIST_BL);
	EncircleList.pszImageBM=MAKEINTRESOURCE(IDB_SERVER_LIST_BM);
	EncircleList.pszImageBR=MAKEINTRESOURCE(IDB_SERVER_LIST_BR);
	m_ListEncircle.InitEncircleResource(EncircleList,AfxGetInstanceHandle());

	//��ܻ���
	tagEncircleResource	EncircleFrame;
	EncircleFrame.pszImageTL=MAKEINTRESOURCE(IDB_FRAME_TL);
	EncircleFrame.pszImageTM=MAKEINTRESOURCE(IDB_FRAME_TM);
	EncircleFrame.pszImageTR=MAKEINTRESOURCE(IDB_FRAME_TR);
	EncircleFrame.pszImageML=MAKEINTRESOURCE(IDB_FRAME_ML);
	EncircleFrame.pszImageMR=MAKEINTRESOURCE(IDB_FRAME_MR);
	EncircleFrame.pszImageBL=MAKEINTRESOURCE(IDB_FRAME_BL);
	EncircleFrame.pszImageBM=MAKEINTRESOURCE(IDB_FRAME_BM);
	EncircleFrame.pszImageBR=MAKEINTRESOURCE(IDB_FRAME_BR);
	m_FrameEncircle.InitEncircleResource(EncircleFrame,AfxGetInstanceHandle());

	return;
}

//��������
CPlatformFrame::~CPlatformFrame()
{
	//ƽ̨����
	ASSERT(m_pPlatformFrame==this);
	if (m_pPlatformFrame==this) m_pPlatformFrame=NULL;

	return;
}

//�ӿڲ�ѯ
VOID * CPlatformFrame::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ICustomFaceEvent,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ICustomFaceEvent,Guid,dwQueryVer);
	return NULL;
}

//�����¼�
VOID CPlatformFrame::OnViewItemEvent(WORD wItemIndex, CWnd * pItemView)
{
	//�������
	bool bShowList=true;
	bool bPlazaView=(m_PlazaViewItem.m_hWnd==pItemView->GetSafeHwnd());

	//�����ж�
	if (bPlazaView==false) bShowList=m_bLockGameList;

	//�б����
	ControlServerList((bShowList==true)?ServerListControl_Show:ServerListControl_Hide,false);

	return;
}

//����¼�
VOID CPlatformFrame::OnSplitterEvent(UINT uControlID, INT nXPos, INT nYPos)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��ʾ�ؼ�
	if (m_bHideGameList==true)
	{
		//���ñ���
		m_bRectify=true;
		m_bHideGameList=false;
	
		//���ð�ť
		HINSTANCE hInstance=AfxGetInstanceHandle();
		m_btControl.SetButtonImage(IDB_BK_GAME_LIST,(m_bHideGameList==true)?TEXT("BT_GAME_LIST_SHOW"):TEXT("BT_GAME_LIST_HIDE"),hInstance,true,false);
	}

	//�����ؼ�
	RectifyControl(rcClient.Width(),rcClient.Height());

	return;
}

//����ʧ��
VOID CPlatformFrame::OnEventSystemFace(DWORD dwUserID, WORD wFaceID)
{
	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��������
	if (pGlobalUserData->dwUserID==dwUserID)
	{
		//���ñ���
		pGlobalUserData->wFaceID=wFaceID;

		//��������
		pGlobalUserData->dwCustomID=0L;
		ZeroMemory(&pGlobalUserData->CustomFaceInfo,sizeof(pGlobalUserData->CustomFaceInfo));

		//�����¼�
		CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
		if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
	}

	return;
}

//ͷ������
VOID CPlatformFrame::OnEventCustomFace(DWORD dwUserID, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo)
{
	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��������
	if (pGlobalUserData->dwUserID==dwUserID)
	{
		//���ñ���
		pGlobalUserData->dwCustomID=dwCustomID;
		CopyMemory(&pGlobalUserData->CustomFaceInfo,&CustomFaceInfo,sizeof(CustomFaceInfo));

		//�����¼�
		CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
		if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
	}

	//��ǰ����
	if (m_pServerViewItem!=NULL)
	{
		m_pServerViewItem->UpdateUserCustomFace(dwUserID,dwCustomID,CustomFaceInfo);
	}

	//���·���
	for (INT_PTR i=0;i<m_ServerViewItemArray.GetCount();i++)
	{
		m_ServerViewItemArray[i]->UpdateUserCustomFace(dwUserID,dwCustomID,CustomFaceInfo);
	}

	return;
}

//��Ϣ����
BOOL CPlatformFrame::PreTranslateMessage(MSG * pMsg)
{
	//��ʾ��Ϣ
	if (m_ToolTipCtrl.m_hWnd!=NULL)
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}

	return __super::PreTranslateMessage(pMsg);
}

//�����
BOOL CPlatformFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	UINT nCommandID=LOWORD(wParam);

	//���ܰ�ť
	switch (nCommandID)
	{
	case IDC_MIN:				//��С��ť
		{
			ShowWindow(SW_MINIMIZE);
			return TRUE;
		}
	case IDC_MAX:				//���ť
		{
			//���ڿ���
			if (m_bMaxShow==true)
			{
				RestoreWindow();
			}
			else
			{
				MaxSizeWindow();
			}

			//���½���
			RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW);

			return TRUE;
		}
	case IDC_CLOSE:				//�رհ�ť
		{
			PostMessage(WM_CLOSE,0,0);
			return TRUE;
		}
	case IDC_SKIN:				//�������
		{
			//��������
			if (m_DlgSkinControl.m_hWnd==NULL)
			{
				m_DlgSkinControl.Create(IDD_SKIN_CONTROL,this);
			}

			//��ȡλ��
			CRect rcButton;
			CRect rcControl;
			m_btSkin.GetWindowRect(&rcButton);
			m_DlgSkinControl.GetWindowRect(&rcControl);

			//��ʾ����
			UINT uFlags=SWP_NOZORDER|SWP_NOSIZE|SWP_SHOWWINDOW;
			m_DlgSkinControl.SetWindowPos(NULL,rcButton.right-rcControl.Width(),rcButton.bottom,0,0,uFlags);

			return TRUE;
		}
	case IDC_CONTROL:			//���ư�ť
		{
			//�����б�
			ControlServerList(ServerListControl_Turn,true);

			return TRUE;
		}
	case IDC_LOCK_LIST:			//�����б�
		{
			//���ñ���
			m_bLockGameList=!m_bLockGameList;

			//���ð�ť
			HINSTANCE hInstance=AfxGetInstanceHandle();
			m_btLockList.SetButtonImage(IDB_BK_LIST,(m_bLockGameList==true)?TEXT("BT_UNLOCK_LIST"):TEXT("BT_LOCK_LIST"),hInstance,true,false);

			return TRUE;
		}
	case IDC_NAVIGATION+0:
	case IDC_NAVIGATION+1:
	case IDC_NAVIGATION+2:
	case IDC_NAVIGATION+3:
	case IDC_NAVIGATION+4:		//������ť
		{
			//��������
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

			//�����ַ
			TCHAR szNavigation[256]=TEXT("");
			_sntprintf(szNavigation,CountArray(szNavigation),TEXT("%s/Navigation%ld.asp"),szPlatformLink,(nCommandID-IDC_NAVIGATION)+1);

			//���ӵ�ַ
			WebBrowse(szNavigation,true);

			return TRUE;
		}
	}

	//�˵�����
	switch (nCommandID)
	{
	case IDM_USER_LOGON:		//�û���¼
		{
			m_MissionLogon.ShowLogon();

			return TRUE;
		}
	case IDM_SWITCH_ACCOUNTS:	//�л��ʺ�
		{
			//�л�ѯ��
			if ((m_ServerViewItemArray.GetCount()>0L)||(m_pServerViewItem!=NULL))
			{
				CInformation Information(this);
				if (Information.ShowMessageBox(TEXT("���е���Ϸ���伴���رգ�ȷʵҪ���л��ʺš��� "),MB_YESNO)!=IDYES) return TRUE;
			}

			//ȫ������
			CGlobalUnits::GetInstance()->DeleteUserCookie();
			CGlobalUserInfo::GetInstance()->ResetUserInfoData();

			//�����¼�
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_LOGOUT,0L);

			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//�����ؼ�
VOID CPlatformFrame::RectifyControl(INT nWidth, INT nHeight)
{
	//״̬�ж�
	if ((nWidth==0)||(nHeight==0)) return;

	//���λ��
	CRect rcSplitter;
	m_SkinSplitter.GetWindowRect(&rcSplitter);

	//������Ϣ
	tagEncircleInfo EncircleInfoList;
	tagEncircleInfo EncircleInfoItem;
	tagEncircleInfo EncircleInfoFrame;
	m_ListEncircle.GetEncircleInfo(EncircleInfoList);
	m_ItemEncircle.GetEncircleInfo(EncircleInfoItem);
	m_FrameEncircle.GetEncircleInfo(EncircleInfoFrame);

	//��ȡλ��
	ScreenToClient(&rcSplitter);

	//��ֿؼ�
	if (m_bHideGameList==false)
	{
		//��ַ�Χ
		INT nLessPos=nWidth*28/100,nMaxPos=nWidth/2;
		if (nLessPos>=LESS_LIST_CX) nLessPos=LESS_LIST_CX;

		//�������
		if (m_bRectify==false)
		{
			m_bRectify=true;
			rcSplitter.left=nLessPos;
			rcSplitter.right=rcSplitter.left+SPLITTER_WIDTH;
		}

		//��ַ�Χ
		m_SkinSplitter.SetSplitterRange(nLessPos,nMaxPos);

		//����λ��
		if ((nWidth!=0)&&(nHeight!=0))
		{
			if (rcSplitter.left<nLessPos)
			{
				rcSplitter.left=nLessPos;
				rcSplitter.right=rcSplitter.left+SPLITTER_WIDTH;
			}
			if (rcSplitter.right>nMaxPos)
			{
				rcSplitter.right=nMaxPos;
				rcSplitter.left=rcSplitter.right-SPLITTER_WIDTH;
			}
		}
	}
	else 
	{
		//���λ��
		rcSplitter.left=EncircleInfoFrame.nLBorder;
		rcSplitter.right=rcSplitter.left+SPLITTER_WIDTH;
	}

	//λ�õ���
	rcSplitter.top=EncircleInfoFrame.nTBorder;
	rcSplitter.bottom=nHeight-EncircleInfoFrame.nBBorder;

	//�ؼ�λ��
	CRect rcUserInfoCtrl;
	m_WndUserInfoCtrl.GetWindowRect(&rcUserInfoCtrl);

	//�ƶ�׼��
	HDWP hDwp=BeginDeferWindowPos(64);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;

	//���ư�ť
	DeferWindowPos(hDwp,m_btMin,NULL,nWidth-94,0,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btMax,NULL,nWidth-66,0,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btSkin,NULL,nWidth-122,0,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btClose,NULL,nWidth-38,0,0,0,uFlags|SWP_NOSIZE);

	//������ť
	CRect rcNavigation;
	m_btNavigation[0].GetWindowRect(&rcNavigation);

	//������ť
	INT nNavigationArea=rcNavigation.Width()*CountArray(m_btNavigation);
	INT nSpace=__min(__max((nWidth-nNavigationArea-700)/(CountArray(m_btNavigation)+1),0),36);

	//������ť
	for (INT i=0;i<CountArray(m_btNavigation);i++)
	{
		INT nIndex=CountArray(m_btNavigation)-i;
		INT nXExcursion=nWidth-(rcNavigation.Width()+nSpace)*nIndex-nSpace-100;
		DeferWindowPos(hDwp,m_btNavigation[i],NULL,nXExcursion,8,0,0,uFlags|SWP_NOSIZE);
	}

	//ƽ̨��ť
	CRect rcPlatform;
	m_btPlatform[0].GetWindowRect(&rcPlatform);

	//ƽ̨��ť
	INT nYPlatform=EncircleInfoFrame.nTBorder+2L;
	DeferWindowPos(hDwp,m_btPlatform[0],NULL,nWidth-(rcPlatform.Width()+7)*3-EncircleInfoFrame.nRBorder-10,nYPlatform,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btPlatform[1],NULL,nWidth-(rcPlatform.Width()+7)*2-EncircleInfoFrame.nRBorder-10,nYPlatform,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btPlatform[2],NULL,nWidth-(rcPlatform.Width()+7)*1-EncircleInfoFrame.nRBorder-10,nYPlatform,0,0,uFlags|SWP_NOSIZE);

	//������ť
	CRect rcLockList;
	m_btLockList.GetWindowRect(&rcLockList);
	DeferWindowPos(hDwp,m_btLockList,NULL,rcSplitter.left-rcLockList.Width()-13,EncircleInfoFrame.nTBorder+rcUserInfoCtrl.Height()+4,
		rcLockList.Width(),rcLockList.Height(),uFlags);

	//ҳ��λ��
	m_WndViewItemCtrl.SetViewItemPostion(rcSplitter.right+EncircleInfoItem.nLBorder,EncircleInfoFrame.nTBorder+EncircleInfoItem.nTBorder,
		nWidth-EncircleInfoFrame.nRBorder-EncircleInfoItem.nRBorder-EncircleInfoItem.nLBorder-rcSplitter.right,
		nHeight-EncircleInfoFrame.nTBorder-EncircleInfoFrame.nBBorder-EncircleInfoItem.nTBorder-EncircleInfoItem.nBBorder);

	//������
	CSize SizeItemFrame;
	m_WndViewItemCtrl.GetViewItemSize(SizeItemFrame);
	DeferWindowPos(hDwp,m_WndViewItemCtrl,NULL,rcSplitter.right+10,EncircleInfoFrame.nTBorder+EncircleInfoItem.nTBorder-SizeItemFrame.cy,
		SizeItemFrame.cx,SizeItemFrame.cy,uFlags);

	//�����ؼ�
	DeferWindowPos(hDwp,m_PlatformPublicize,NULL,320,7,270,65,uFlags);
	DeferWindowPos(hDwp,m_SkinSplitter,NULL,rcSplitter.left,rcSplitter.top,rcSplitter.Width(),rcSplitter.Height(),uFlags);
	DeferWindowPos(hDwp,m_WndUserInfoCtrl,NULL,EncircleInfoFrame.nLBorder,EncircleInfoFrame.nTBorder,rcSplitter.left-EncircleInfoFrame.nLBorder,rcUserInfoCtrl.Height(),uFlags);
	DeferWindowPos(hDwp,m_ServerListView,NULL,EncircleInfoFrame.nLBorder+EncircleInfoList.nLBorder,EncircleInfoFrame.nTBorder+EncircleInfoList.nTBorder+rcUserInfoCtrl.Height(),
		rcSplitter.left-EncircleInfoList.nLBorder-EncircleInfoList.nRBorder-EncircleInfoFrame.nLBorder,nHeight-EncircleInfoList.nTBorder-EncircleInfoList.nBBorder
		-EncircleInfoFrame.nTBorder-rcUserInfoCtrl.Height()-EncircleInfoFrame.nBBorder,uFlags);

	//��������
	LockWindowUpdate();
	EndDeferWindowPos(hDwp);
	UnlockWindowUpdate();

	//���ư�ť
	CRect rcControl;
	m_btControl.GetClientRect(&rcControl);
	m_btControl.SetWindowPos(NULL,(rcSplitter.Width()-rcControl.Width())/2,(rcSplitter.Height()-rcControl.Height())/2,0,0,uFlags|SWP_NOSIZE);

	//���½���
	RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE);

	return;
}

//���ҳ��
bool CPlatformFrame::WebBrowse(LPCTSTR pszURL, bool bAutoFullView)
{
	//���ҳ��
	CPlazaViewItem::GetInstance()->WebBrowse(pszURL);

	//�������
	m_WndViewItemCtrl.ActiveViewItem(&m_PlazaViewItem);

	//�������
	if (bAutoFullView==true)
	{
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
		if (pParameterGlobal->m_bFullScreenBrowse==true) ControlServerList(ServerListControl_Hide,false);
	}

	return true;
}

//�б����
bool CPlatformFrame::ControlServerList(enServerListControl ServerListControl, bool bFocusControl)
{
	//λ�ÿ���
	bool bHideGameList=false;
	switch (ServerListControl)
	{
	case ServerListControl_Hide: { bHideGameList=true; break; }
	case ServerListControl_Show: { bHideGameList=false; break; }
	case ServerListControl_Turn: { bHideGameList=!m_bHideGameList; break; }
	}

	//���ƹ���
	if ((bHideGameList==true)&&(bFocusControl==false)&&(m_bLockGameList==true))
	{
		return true;
	}

	//�����ж�
	if (bHideGameList!=m_bHideGameList)
	{
		//���ñ���
		m_bRectify=false;
		m_bHideGameList=bHideGameList;

		//���ð�ť
		HINSTANCE hInstance=AfxGetInstanceHandle();
		m_btControl.SetButtonImage(IDB_BK_GAME_LIST,(m_bHideGameList==true)?TEXT("BT_GAME_LIST_SHOW"):TEXT("BT_GAME_LIST_HIDE"),hInstance,true,false);

		//��������
		CRect rcClient;
		GetClientRect(&rcClient);
		RectifyControl(rcClient.Width(),rcClient.Height());
	}

	return true;
}

//��ȡ˽��
bool CPlatformFrame::ShowWhisperItem()
{
	//��ȡ��Ϣ
	if (m_DlgWhisperItemArray.GetCount()>0L)
	{
		//��ȡ����
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[0];

		//��ʾ����
		pDlgWhisper->ShowWindow(SW_RESTORE);

		//�ö�����
		pDlgWhisper->SetActiveWindow();
		pDlgWhisper->BringWindowToTop();
		pDlgWhisper->SetForegroundWindow();

		//ɾ������
		m_DlgWhisperItemArray.RemoveAt(0L);

		//ɾ��ͼ��
		if (m_DlgWhisperItemArray.GetCount()==0L)
		{
			//���ñ���
			m_bWhisperIcon=false;

			//ɾ��ʱ��
			KillTimer(IDI_FLASH_TRAY_ICON);

			//����ͼ��
			m_SystemTrayIcon.ShowTrayIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)));
		}

		return true;
	}

	return false;
}

//����˽��
bool CPlatformFrame::InsertWhisperItem(CDlgWhisper * pDlgWhisper)
{
	//����Ч��
	ASSERT((pDlgWhisper!=NULL)&&(pDlgWhisper->m_hWnd!=NULL));
	if ((pDlgWhisper==NULL)||(pDlgWhisper->m_hWnd==NULL)) return false;

	//״̬�ж�
	if (pDlgWhisper->IsWindowVisible()) return false;

	//��������
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		if (m_DlgWhisperItemArray[i]==pDlgWhisper) return false;
	}

	//����˽��
	m_DlgWhisperItemArray.Add(pDlgWhisper);

	//����ͼ��
	if (m_DlgWhisperItemArray.GetCount()==1L)
	{
		//���ñ���
		m_bWhisperIcon=true;

		//����ʱ��
		SetTimer(IDI_FLASH_TRAY_ICON,500,NULL);

		//����ͼ��
		m_SystemTrayIcon.ShowTrayIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_WHISPER)));
	}

	return true;
}

//ɾ��˽��
bool CPlatformFrame::RemoveWhisperItem(CDlgWhisper * pDlgWhisper)
{
	//��������
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		if (m_DlgWhisperItemArray[i]==pDlgWhisper)
		{
			//ɾ������
			m_DlgWhisperItemArray.RemoveAt(i);

			//ɾ��ͼ��
			if (m_DlgWhisperItemArray.GetCount()==0L)
			{
				//���ñ���
				m_bWhisperIcon=false;

				//ɾ��ʱ��
				KillTimer(IDI_FLASH_TRAY_ICON);

				//����ͼ��
				m_SystemTrayIcon.ShowTrayIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)));
			}

			return true;
		}
	}

	return false;
}

//�����
bool CPlatformFrame::ActiveServerViewItem(CServerViewItem * pServerViewItem)
{
	//�����ж�
	if (m_pServerViewItem==pServerViewItem)
	{
		//���ñ���
		m_pServerViewItem=NULL;

		//���뷿��
		m_ServerViewItemArray.Add(pServerViewItem);

		//�����ǩ
		LPCTSTR pszKindName(pServerViewItem->GetKindName());
		m_WndViewItemCtrl.InsertViewItem(pServerViewItem,pszKindName);
	}

	//�����
	m_WndViewItemCtrl.ActiveViewItem(pServerViewItem);

	return true;
}

//ɾ������
bool CPlatformFrame::DeleteServerViewItem(CServerViewItem * pServerViewItem)
{
	//��ǰ����
	if ((m_pServerViewItem!=NULL)&&(m_pServerViewItem==pServerViewItem))
	{
		//���ñ���
		m_pServerViewItem=NULL;

		//���ٷ���
		pServerViewItem->DestroyWindow();

		return true;
	}

	//��������
	for (INT_PTR i=0;i<m_ServerViewItemArray.GetCount();i++)
	{
		if (m_ServerViewItemArray[i]==pServerViewItem)
		{
			//���ٷ���
			pServerViewItem->DestroyWindow();
			m_ServerViewItemArray.RemoveAt(i);

			//ɾ������
			m_WndViewItemCtrl.RemoveViewItem(pServerViewItem);
			m_WndViewItemCtrl.ActiveViewItem(&m_PlazaViewItem);

			//����֪ͨ
			CDlgSearchUser * pDlgSearchUser=CDlgSearchUser::GetInstance();
			if (pDlgSearchUser!=NULL) pDlgSearchUser->OnViewItemDelete(pServerViewItem);

			return true;
		}
	}

	return false;
}

//ö�ٷ���
CServerViewItem * CPlatformFrame::EmunServerViewItem(WORD wIndex)
{
	//�����ж�
	if (wIndex<m_ServerViewItemArray.GetCount())
	{
		return m_ServerViewItemArray[wIndex];
	}

	return NULL;
}

//���ҷ���
CServerViewItem * CPlatformFrame::SearchServerViewItem(WORD wServerID)
{
	//���ҷ���
	for (INT_PTR i=0;i<m_ServerViewItemArray.GetCount();i++)
	{
		CServerViewItem * pServerViewItem=m_ServerViewItemArray[i];
		if (pServerViewItem->GetServerID()==wServerID) return pServerViewItem;
	}

	return NULL;
}

//���뷿��
CServerViewItem * CPlatformFrame::EntranceServerItem(CGameKindItem * pGameKindItem)
{
	return NULL;
}

//���뷿��
CServerViewItem * CPlatformFrame::EntranceServerItem(CGameServerItem * pGameServerItem)
{
	//Ч�����
	ASSERT(pGameServerItem!=NULL);
	if (pGameServerItem==NULL) return NULL;

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��¼�ж�
	if (pGlobalUserData->dwUserID==0L) return NULL;

	//��������
	tagGameServer * pGameServer=&pGameServerItem->m_GameServer;
	tagGameKind * pGameKind=&pGameServerItem->m_pGameKindItem->m_GameKind;

	//���ҷ���
	WORD wServerID=pGameServer->wServerID;
	CServerViewItem * pServerViewItem=SearchServerViewItem(wServerID);

	//�����
	if (pServerViewItem!=NULL)
	{
		ActiveServerViewItem(pServerViewItem);
		return pServerViewItem;
	}

	//��ǰ�ж�
	if (m_pServerViewItem!=NULL)
	{
		//��ͬ�ж�
		if (wServerID==m_pServerViewItem->GetServerID()) return pServerViewItem;

		//��ʾ��Ϣ
		TCHAR szBuffer[256]=TEXT("");
		_sntprintf(szBuffer,CountArray(szBuffer),TEXT("���ڽ��� [ %s ] ��Ϸ�����У����Ժ�ȴ�Ƭ��..."),m_pServerViewItem->GetServerName());

		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(szBuffer,MB_OK|MB_ICONWARNING,30);

		return m_pServerViewItem;
	}

	//��Ա����
	if (m_ServerViewItemArray.GetCount()>=MAX_SERVER_COUNT)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("���������Ϸ����̫���ˣ���رղ��ֺ��ٴγ��ԣ�"),MB_ICONQUESTION,60);

		return NULL;
	}

	//��װ�ж�
	if (pGameServerItem->m_pGameKindItem->m_dwProcessVersion==0L)
	{
		//��ȡ�汾
		CGameKindItem * pGameKindItem=pGameServerItem->m_pGameKindItem;
		CWHService::GetModuleVersion(pGameKind->szProcessName,pGameKindItem->m_dwProcessVersion);

		//������Ϸ
		if (pGameServerItem->m_pGameKindItem->m_dwProcessVersion==0L)
		{
			CGlobalUnits * pCGlobalUnits=CGlobalUnits::GetInstance();
			pCGlobalUnits->DownLoadClient(pGameKind->szKindName,pGameKind->wKindID,wServerID);

			return NULL;
		}

		//�����б�
		m_ServerListView.OnGameItemUpdate(pGameKindItem);
	}

	//��������
	try
	{
		//��������
		m_pServerViewItem=new CServerViewItem;
		m_pServerViewItem->Create(IDD_GAME_SERVER,this);

		//���÷���
		m_pServerViewItem->InitServerViewItem(pGameServerItem);
	}
	catch (LPCTSTR pszString)
	{
		//���ٷ���
		if (m_pServerViewItem!=NULL)
		{
			if (m_pServerViewItem->m_hWnd!=NULL)
			{
				m_pServerViewItem->DestroyWindow();
			}
			else
			{
				SafeDelete(m_pServerViewItem);
			}
		}

		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(pGameServer->szServerName,pszString,MB_ICONERROR);

		return NULL;
	}

	return m_pServerViewItem;
}

//��ԭ����
bool CPlatformFrame::RestoreWindow()
{
	//״̬�ж�
	if (m_bMaxShow==true)
	{
		//���ñ���
		m_bMaxShow=false;
		m_bRectify=false;

		//���ð�ť
		m_btMax.SetButtonImage(IDB_BT_MAX,AfxGetInstanceHandle(),true,false);

		//�ƶ�����
		LockWindowUpdate();
		SetWindowPos(NULL,m_rcNormalSize.left,m_rcNormalSize.top,m_rcNormalSize.Width(),m_rcNormalSize.Height(),SWP_NOZORDER);
		UnlockWindowUpdate();
	}

	return true;
}

//��󴰿�
bool CPlatformFrame::MaxSizeWindow()
{
	//״̬�ж�
	if (m_bMaxShow==false)
	{
		//���ñ���
		m_bMaxShow=true;
		m_bRectify=false;

		//Ĭ��λ��
		GetWindowRect(&m_rcNormalSize);

		//���ð�ť
		m_btMax.SetButtonImage(IDB_BT_RESORE,AfxGetInstanceHandle(),true,false);

		//��ȡλ��
		CRect rcArce;
		SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

		//�ƶ�����
		LockWindowUpdate();
		SetWindowPos(NULL,rcArce.left-2,rcArce.top-2,rcArce.Width()+4,rcArce.Height()+4,SWP_NOZORDER);
		UnlockWindowUpdate();
	}

	return true;
}

//�ر���Ϣ
VOID CPlatformFrame::OnClose()
{
	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//�ر���ʾ
	if ((pGlobalUserData->dwUserID!=0L)&&(m_MissionLogon.GetActiveStatus()==false))
	{
		//��ʾ����
		CDlgEnquire DlgEnquire;
		INT_PTR nResult=DlgEnquire.DoModal();

		//�����
		switch (nResult)
		{
		case IDCANCEL:				//ȡ������
			{
				return;
			}
		case IDC_CLOSE_SERVER:		//�˳�����
			{
				//�رշ���
				ASSERT(m_WndViewItemCtrl.GetActiveItem()>0);
				WORD wActiveItem=m_WndViewItemCtrl.GetActiveItem();
				
				//�رշ���
				if (wActiveItem>=1)
				{
					ASSERT((wActiveItem-1)<m_ServerViewItemArray.GetCount());
					DeleteServerViewItem(m_ServerViewItemArray[wActiveItem-1]);
				}

				return;
			}
		case IDC_SWITCH_ACCOUNTS:	//�л��ʺ�
			{
				//Ͷ����Ϣ
				PostMessage(WM_COMMAND,IDM_SWITCH_ACCOUNTS,0);

				return;
			}
		}
	}

	//ע���ȼ�
	UnregisterHotKey(m_hWnd,IDI_HOT_KEY_BOSS);
	UnregisterHotKey(m_hWnd,IDI_HOT_KEY_WHISPER);

	__super::OnClose();
}

//�滭����
BOOL CPlatformFrame::OnEraseBkgnd(CDC * pDC)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//���λ��
	tagEncircleInfo ItemEncircleInfo;
	tagEncircleInfo FrameEncircleInfo;
	m_ItemEncircle.GetEncircleInfo(ItemEncircleInfo);
	m_FrameEncircle.GetEncircleInfo(FrameEncircleInfo);

	//���λ��
	CRect rcSplitter;
	m_SkinSplitter.GetWindowRect(&rcSplitter);

	//���λ��
	CRect rcWebPublicize;
	m_PlatformPublicize.GetWindowRect(&rcWebPublicize);

	//λ��ת��
	ScreenToClient(&rcSplitter);
	ScreenToClient(&rcWebPublicize);

	//�ؼ�λ��
	CRect rcUserInfoCtrl;
	m_WndUserInfoCtrl.GetWindowRect(&rcUserInfoCtrl);

	//��Ϸ�б�
	if (m_bHideGameList==false)
	{
		//�б�λ��
		CRect rcList;
		rcList.right=rcSplitter.left;
		rcList.left=FrameEncircleInfo.nLBorder;
		rcList.top=FrameEncircleInfo.nTBorder+rcUserInfoCtrl.Height();
		rcList.bottom=rcClient.Height()-FrameEncircleInfo.nBBorder;

		//��������
		CImage ImageBuffer;
		ImageBuffer.Create(rcList.Width(),rcList.Height(),32);

		//���� DC
		CDC BufferDC;
		BufferDC.CreateCompatibleDC(pDC);
		BufferDC.SelectObject(ImageBuffer);

		//���û���
		BufferDC.SetBkMode(TRANSPARENT);
		BufferDC.SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

		//�滭�б�
		CRect rcBuffer(0,0,rcList.Width(),rcList.Height());
		m_ListEncircle.DrawEncircleFrame(&BufferDC,rcBuffer);

		//�����ַ�
		TCHAR szOnLineInfo[32]=TEXT("");
		_sntprintf(szOnLineInfo,CountArray(szOnLineInfo),TEXT("ͬʱ����������%ld"),3000000);

		//�滭��Ϣ
		BufferDC.SetTextColor(RGB(0,0,0));
		//BufferDC.TextOut(10,7,szOnLineInfo,lstrlen(szOnLineInfo));

		//�滭����
		pDC->BitBlt(rcList.left,rcList.top,rcList.Width(),rcList.Height(),&BufferDC,0,0,SRCCOPY);

		//������Դ
		BufferDC.DeleteDC();
		ImageBuffer.Destroy();
	}

	//������
	CRect rcItem;
	rcItem.left=rcSplitter.right;
	rcItem.top=FrameEncircleInfo.nTBorder;
	rcItem.right=rcClient.Width()-FrameEncircleInfo.nRBorder;
	rcItem.bottom=rcClient.Height()-FrameEncircleInfo.nBBorder;

	//�滭���
	m_ItemEncircle.DrawEncircleFrame(pDC,rcItem);
	m_FrameEncircle.DrawEncircleFrame(pDC,rcClient);

	return TRUE;
}

//��������
UINT CPlatformFrame::OnNcHitTest(CPoint Point)
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
		if (ClientPoint.x<=BORAD_SIZE)
		{
			if (ClientPoint.y<=CAPTION_SIZE) return HTTOPLEFT;
			if (ClientPoint.y>=(rcClient.Height()-BORAD_SIZE)) return HTBOTTOMLEFT;
			return HTLEFT;
		}

		//����λ��
		if (ClientPoint.x>=(rcClient.Width()-BORAD_SIZE))
		{
			if (ClientPoint.y<=CAPTION_SIZE) return HTTOPRIGHT;
			if (ClientPoint.y>=(rcClient.Height()-BORAD_SIZE)) return HTBOTTOMRIGHT;
			return HTRIGHT;
		}

		//����λ��
		if (ClientPoint.y<=BORAD_SIZE) return HTTOP;
		if (ClientPoint.y>=(rcClient.Height()-BORAD_SIZE)) return HTBOTTOM;
	}

	return __super::OnNcHitTest(Point);
}

//ʱ����Ϣ
VOID CPlatformFrame::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case IDI_FLASH_TRAY_ICON:	//����ͼ��
		{
			//���ñ���
			m_bWhisperIcon=!m_bWhisperIcon;

			//����ͼ��
			UINT uIconID=(m_bWhisperIcon==true)?IDI_WHISPER:IDI_NULL;
			m_SystemTrayIcon.ShowTrayIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(uIconID)));

			return;
		}
	}

	__super::OnTimer(nIDEvent);
}

//λ����Ϣ
VOID CPlatformFrame::OnSize(UINT nType, INT cx, INT cy) 
{
	__super::OnSize(nType, cx, cy);

	//�����ؼ�
	RectifyControl(cx,cy);

	//��������
	CRgn WindowRgn;
	WindowRgn.CreateRoundRectRgn(0,0,cx+1,cy+1,ROUND_CX,ROUND_CY);

	//��������
	SetWindowRgn(WindowRgn,TRUE);

	return;
}

//λ����Ϣ
VOID CPlatformFrame::OnGetMinMaxInfo(MINMAXINFO * lpMMI)
{
	__super::OnGetMinMaxInfo(lpMMI);

	//����λ��
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

	//����λ��
	lpMMI->ptMinTrackSize.x=__min(LESS_SCREEN_CX,rcArce.Width());
	lpMMI->ptMinTrackSize.y=__min(LESS_SCREEN_CY,rcArce.Height());

	return;
}

//������Ϣ
INT CPlatformFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//���ô���
	ModifyStyle(WS_CAPTION,0,0);
	ModifyStyleEx(WS_BORDER|WS_EX_CLIENTEDGE|WS_EX_WINDOWEDGE,0,0);

	//�����㳡
	CRect rcCreate(0,0,0,0);
	m_PlazaViewItem.Create(IDD_GAME_PLAZA,this);

	//�����ؼ�
	m_ServerListView.Create(WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_SERVER_LIST);
	m_SkinSplitter.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_SKIN_SPLITTER);

	//���ڿؼ�
	m_WndViewItemCtrl.Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,IDC_VIEW_ITEM_CTRL);
	m_WndUserInfoCtrl.Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,IDC_USER_INFO_CTRL);
	m_PlatformPublicize.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcCreate,this,IDC_WEB_PUBLICIZE);

	//������ť
	m_btMin.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_MIN);
	m_btMax.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_MAX);
	m_btSkin.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_SKIN);
	m_btClose.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_CLOSE);
	m_btControl.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_CONTROL);
	m_btLockList.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_LOCK_LIST);

	//���ð�ť
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btMin.SetButtonImage(IDB_BT_MIN,hInstance,true,false);
	m_btMax.SetButtonImage(IDB_BT_MAX,hInstance,true,false);
	m_btSkin.SetButtonImage(IDB_BT_SKIN,hInstance,true,false);
	m_btClose.SetButtonImage(IDB_BT_CLOSE,TEXT("BT_CLOSE"),hInstance,true,false);
	m_btLockList.SetButtonImage(IDB_BK_LIST,TEXT("BT_LOCK_LIST"),hInstance,true,false);

	//���ư�ť
	m_btControl.SetParent(&m_SkinSplitter);
	m_btControl.SetButtonImage(IDB_BK_GAME_LIST,TEXT("BT_GAME_LIST_HIDE"),hInstance,true,false);

	//ƽ̨��ť
	for (INT i=0;i<CountArray(m_btPlatform);i++)
	{
		m_btPlatform[i].Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_PLATFORM+i);
	}

	//������ť
	for (INT i=0;i<CountArray(m_btNavigation);i++)
	{
		m_btNavigation[i].Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_NAVIGATION+i);
	}

	//ƽ̨��ť
	m_btPlatform[0].SetButtonImage(IDB_BK_PLATFORM_1,TEXT("BT_PLATFORM_1"),hInstance,true,false);
	m_btPlatform[1].SetButtonImage(IDB_BK_PLATFORM_2,TEXT("BT_PLATFORM_2"),hInstance,true,false);
	m_btPlatform[2].SetButtonImage(IDB_BK_PLATFORM_3,TEXT("BT_PLATFORM_3"),hInstance,true,false);

	//������ť
	m_btNavigation[0].SetButtonImage(IDB_BK_NAVIGATION,TEXT("BT_NAVIGATION_1"),hInstance,true,false);
	m_btNavigation[1].SetButtonImage(IDB_BK_NAVIGATION,TEXT("BT_NAVIGATION_2"),hInstance,true,false);
	m_btNavigation[2].SetButtonImage(IDB_BK_NAVIGATION,TEXT("BT_NAVIGATION_3"),hInstance,true,false);
	m_btNavigation[3].SetButtonImage(IDB_BK_NAVIGATION,TEXT("BT_NAVIGATION_4"),hInstance,true,false);
	m_btNavigation[4].SetButtonImage(IDB_BK_NAVIGATION,TEXT("BT_NAVIGATION_5"),hInstance,true,false);

	//�������
	m_SkinSplitter.SetSplitterEvent(this);
	m_SkinSplitter.SetSplitterColor(0.67,0.95);
	m_SkinSplitter.SetSplitterType(SplitterType_Vor);

	//��Ϸ�б�
	m_ServerListView.InitServerTreeView();
	CServerListData::GetInstance()->SetServerListDataSink(&m_ServerListView);

	//��ʾ�ؼ�
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(&m_btMin,TEXT("��С��"));
	m_ToolTipCtrl.AddTool(&m_btMax,TEXT("���"));
	m_ToolTipCtrl.AddTool(&m_btSkin,TEXT("���滻��"));
	m_ToolTipCtrl.AddTool(&m_btClose,TEXT("�ر���Ϸ"));
	m_ToolTipCtrl.AddTool(&m_btNavigation[0],TEXT("�ٷ���ҳ"));
	m_ToolTipCtrl.AddTool(&m_btNavigation[1],TEXT("�ϴ�ͷ��"));
	m_ToolTipCtrl.AddTool(&m_btNavigation[2],TEXT("�ʺų�ֵ"));
	m_ToolTipCtrl.AddTool(&m_btNavigation[3],TEXT("�����̳�"));
	m_ToolTipCtrl.AddTool(&m_btNavigation[4],TEXT("��Ա�û�"));
	m_ToolTipCtrl.AddTool(&m_btControl,TEXT("���أ���ʾ����Ϸ�б�"));

	//ϵͳ����
	m_WndViewItemCtrl.SetViewItemEvent(this);
	m_WndViewItemCtrl.InsertViewItem(&m_PlazaViewItem,TEXT("��Ϸ�㳡"));

	//ע���¼�
	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
	if (pPlatformEvent!=NULL) pPlatformEvent->RegisterEventWnd(m_hWnd);

	//����λ��
	CSize SizeRestrict;
	SizeRestrict.SetSize(LESS_SCREEN_CX,LESS_SCREEN_CY);

	//����λ��
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

	//��ȡλ��
	CWHRegKey InfoKeyItem;
	if (InfoKeyItem.OpenRegKey(REG_INTERFACE_INFO,false)==true)
	{
		SizeRestrict.cx=InfoKeyItem.GetValue(TEXT("ScreenWidth"),rcArce.Width()/2);
		SizeRestrict.cy=InfoKeyItem.GetValue(TEXT("ScreenHeight"),rcArce.Height()/2);
	}

	//λ�õ���
	SizeRestrict.cx=__max(LESS_SCREEN_CX,SizeRestrict.cx);
	SizeRestrict.cy=__max(LESS_SCREEN_CY,SizeRestrict.cy);
	SizeRestrict.cx=__min(rcArce.Width(),SizeRestrict.cx);
	SizeRestrict.cy=__min(rcArce.Height(),SizeRestrict.cy);

	//�ƶ�����
	m_rcNormalSize.top=(rcArce.Height()-SizeRestrict.cy)/2;
	m_rcNormalSize.left=(rcArce.Width()-SizeRestrict.cx)/2;
	m_rcNormalSize.right=m_rcNormalSize.left+SizeRestrict.cx;
	m_rcNormalSize.bottom=m_rcNormalSize.top+SizeRestrict.cy;
	SetWindowPos(NULL,m_rcNormalSize.left,m_rcNormalSize.top,m_rcNormalSize.Width(),m_rcNormalSize.Height(),SWP_NOZORDER);

	//�����ַ
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/Ads/Platform.asp"),szPlatformLink);

	//���ӹ��
	m_PlatformPublicize.Navigate(szBillUrl);
	m_PlatformPublicize.SetBoradColor(RGB(0,0,0));

	//��ʾ����
	MaxSizeWindow();

	//��¼ϵͳ
	PostMessage(WM_COMMAND,IDM_USER_LOGON,0);

	//ע���ȼ�
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
	CWHService::RegisterHotKey(m_hWnd,IDI_HOT_KEY_BOSS,pParameterGlobal->m_wBossHotKey);
	CWHService::RegisterHotKey(m_hWnd,IDI_HOT_KEY_WHISPER,pParameterGlobal->m_wWhisperHotKey);

	//��������
	m_SystemTrayIcon.InitTrayIcon(m_hWnd,IDC_SYSTEM_TRAY_ICON);
	m_SystemTrayIcon.ShowTrayIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)),szProduct);

	return 0;
}

//�����Ϣ
VOID CPlatformFrame::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//ģ�ⰴ����
	if ((IsZoomed()==FALSE)&&(Point.y<=CAPTION_SIZE))
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}

	return;
}

//�����Ϣ
VOID CPlatformFrame::OnLButtonDblClk(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDblClk(nFlags,Point);

	//״̬�ж�
	if (Point.y>CAPTION_SIZE) return;

	//���ڿ���
	if (m_bMaxShow==true)
	{
		RestoreWindow();
	}
	else
	{
		MaxSizeWindow();
	}
	
	return;
}

//���øı�
VOID CPlatformFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	__super::OnSettingChange(uFlags,lpszSection);

	//��ȡ��С
	CRect rcClient,rcWindow;
	GetClientRect(&rcClient);
	GetWindowRect(&rcWindow);
	ClientToScreen(&rcClient);

	//����λ��
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,0);

	//����λ��
	rcArce.top-=(rcClient.top-rcWindow.top);
	rcArce.left-=(rcClient.left-rcWindow.left);
	rcArce.right+=(rcWindow.right-rcClient.right);
	rcArce.bottom+=(rcWindow.bottom-rcClient.bottom);

	//�ƶ�����
	SetWindowPos(NULL,rcArce.left,rcArce.top,rcArce.Width(),rcArce.Height(),SWP_NOZORDER|SWP_NOCOPYBITS);

	return;
}

//ƽ̨��ť
VOID CPlatformFrame::OnBnClickedPlatform1()
{
	//�����û�
	CDlgSearchUser DlgSearchUser;
	DlgSearchUser.DoModal();

	return;
}

//ƽ̨��ť
VOID CPlatformFrame::OnBnClickedPlatform2()
{
	//��������
	bool bGoldServer=false;
	CServerViewItem * pServerViewGold=NULL;

	//���ҷ���
	for (INT_PTR i=0;i<m_ServerViewItemArray.GetCount();i++)
	{
		//��ȡ����
		ASSERT(m_ServerViewItemArray[i]!=NULL);
		CServerViewItem * pServerViewItem=m_ServerViewItemArray[i];

		//��������
		WORD wServerType=pServerViewItem->GetServerType();
		enServiceStatus ServiceStatus=pServerViewItem->GetServiceStatus();

		//�����ж�
		if (((wServerType&GAME_GENRE_GOLD)!=0)&&(ServiceStatus==ServiceStatus_ServiceIng))
		{
			//���ñ���
			bGoldServer=true;
			pServerViewGold=pServerViewItem;

			break;
		}
	}

	//������Դ
	AfxSetResourceHandle(GetModuleHandle(SHARE_CONTROL_DLL_NAME));

	//��������
	if (bGoldServer==false)
	{
		//�㳡����
		CDlgInsurePlaza DlgInsurePlaza;
		INT_PTR nResult=DlgInsurePlaza.DoModal();
	}
	else
	{
		//��������
		ASSERT(pServerViewGold!=NULL);
		pServerViewGold->ShowInsureView();
	}

	return;
}

//ƽ̨��ť
VOID CPlatformFrame::OnBnClickedPlatform3()
{
	//��������
	CParameterGame * pParameterGame=NULL;
	CParameterServer * pParameterServer=NULL;

	//�������
	WORD wActiveItem=m_WndViewItemCtrl.GetActiveItem();
	tagViewItemInfo * pViewItemInfo=m_WndViewItemCtrl.GetViewItemInfo(wActiveItem);

	//��������
	if ((wActiveItem>=1)&&(pViewItemInfo!=NULL))
	{
		//��ȡ����
		CWnd * pItemView=pViewItemInfo->pItemView;
		CServerViewItem * pServerViewItem=(CServerViewItem *)pItemView;

		//���ñ���
		if (pServerViewItem!=NULL)
		{
			pParameterGame=pServerViewItem->GetParameterGame();
			pParameterServer=pServerViewItem->GetParameterServer();
		}

	}

	//�������
	CDlgCollocate DlgCollocate;
	DlgCollocate.InitCollocate(pParameterGame,pParameterServer);

	//��ʾ����
	if (DlgCollocate.DoModal()==IDOK)
	{
		//���͹���
		for (INT_PTR i=0;i<m_ServerViewItemArray.GetCount();i++)
		{
			//��ȡ����
			CServerViewItem * pServerViewItem=m_ServerViewItemArray[i];
			if (pServerViewItem->GetServiceStatus()==ServiceStatus_ServiceIng) pServerViewItem->SendUserRulePacket();
		}
	}

	return;
}

//�ȼ���Ϣ
LRESULT CPlatformFrame::OnMessageHotKey(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case IDI_HOT_KEY_BOSS:		//�ϰ��ȼ�
		{
			//��������
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			//��¼�ж�
			if (pGlobalUserData->dwUserID!=0L)
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

					//��ʾͼ��
					m_SystemTrayIcon.ShowTrayIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)));
				}
				else
				{
					//���ش���
					ShowWindow(SW_MINIMIZE);
					ShowWindow(SW_HIDE);

					//����ͼ��
					m_SystemTrayIcon.HideTrayIcon();
				}

				//������Ϣ
				for (INT_PTR i=0;i<m_ServerViewItemArray.GetCount();i++)
				{
					ASSERT(m_ServerViewItemArray[i]!=NULL);
					m_ServerViewItemArray[i]->NotifyBossCome(bBossCome);
				}
			}

			return 0;
		}
	case IDI_HOT_KEY_WHISPER:	//˽���ȼ�
		{
			//��ȡ��Ϣ
			if (m_DlgWhisperItemArray.GetCount()>0L)
			{
				ShowWhisperItem();
				return 1;
			}

			return 0;
		}
	}

	return 0;
}

//ͼ����Ϣ
LRESULT CPlatformFrame::OnMessageTrayIcon(WPARAM wParam, LPARAM lParam)
{
	//�¼�����
	switch (lParam)
	{
	case WM_LBUTTONDOWN:		//��굥��
	case WM_LBUTTONDBLCLK:		//���˫��
		{
			//��ȡ��Ϣ
			if (m_DlgWhisperItemArray.GetCount()>0L)
			{
				ShowWhisperItem();
				return 1;
			}

			return 1;
		}
	/*case WM_RBUTTONDOWN:		//��굥��
	case WM_RBUTTONDBLCLK:		//���˫��
		{
			//�����˵�
			CSkinMenu Menu;
			Menu.CreateMenu();

			//����˵�
			Menu.AppendMenu(IDM_NULL_COMMAND,TEXT("��ʾ����"));
			Menu.AppendMenu(IDM_NULL_COMMAND,TEXT("�˳��㳡"));
			Menu.AppendSeparator();
			Menu.AppendMenu(IDM_NULL_COMMAND,TEXT("����..."));

			//��ʾ�˵�
			CPoint MousePoint;
			GetCursorPos(&MousePoint);
			Menu.TrackPopupMenu(MousePoint.x,MousePoint.y,this);

			return 1;
		}*/
	}

	return 0;
}

//�¼���Ϣ
LRESULT CPlatformFrame::OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam)
{
	//�¼�����
	switch (wParam)
	{
	case EVENT_USER_LOGON:			//��¼���
		{
			//��ʾ����
			ShowWindow(SW_SHOW);
			SetForegroundWindow();

			//��Ϸ�б�
			ControlServerList(ServerListControl_Show,false);

			//�û����
			ASSERT(CGlobalUnits::GetInstance()!=NULL);
			CGlobalUnits::GetInstance()->WriteUserCookie();

			return 0L;
		}
	case EVENT_USER_LOGOUT:			//ע���ɹ�
		{
			//����㳡
			m_WndViewItemCtrl.ActiveViewItem(&m_PlazaViewItem);

			//�رշ���
			if (m_pServerViewItem!=NULL)
			{
				DeleteServerViewItem(m_pServerViewItem);
			}

			//�رշ���
			while (m_ServerViewItemArray.GetCount()>0)
			{
				DeleteServerViewItem(m_ServerViewItemArray[0]);
			}

			//���ش���
			ShowWindow(SW_HIDE);

			//��ʾ��¼
			m_MissionLogon.ShowLogon();

			return 0L;
		}
	case EVENT_DOWN_LOAD_FINISH:	//�������
		{
			//��ȡ����
			WORD wKindID=LOWORD(lParam);
			WORD wServerID=HIWORD(lParam);

			//�����б�
			CServerListData * pServerListData=CServerListData::GetInstance();
			if (pServerListData!=NULL) pServerListData->OnEventDownLoadFinish(wKindID);

			//���뷿��
			if ((wKindID!=0)&&(wServerID!=0))
			{
				CGameServerItem * pGameServerItem=m_ServerListData.SearchGameServer(wServerID);
				if (pGameServerItem!=NULL) EntranceServerItem(pGameServerItem);
			}

			return 0L;
		}
	}

	return 0L;
}

//////////////////////////////////////////////////////////////////////////////////
