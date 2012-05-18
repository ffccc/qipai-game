#include "Stdafx.h"
#include "GamePlaza.h"
#include "DlgService.h"
#include "DlgCustomFace.h"
#include "PlatformFrame.h"
#include "DlgLockMachine.h"
#include "WndUserInfoCtrl.h"

//////////////////////////////////////////////////////////////////////////////////

//���ư�ť
#define IDC_USER_INFO_1				100									//�û���ť
#define IDC_USER_INFO_2				101									//�û���ť
#define IDC_USER_INFO_3				102									//�û���ť
#define IDC_USER_INFO_SET			103									//�û���ť
#define IDC_USER_INFO_EDIT			104									//�༭�ؼ�

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CWndUserInfoCtrl, CWnd)

	//ϵͳ��Ϣ
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()

	//�Զ���Ϣ
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_PLATFORM_EVENT, OnMessagePlatformEvent)

END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CEditUnderWrite, CSkinEditEx)
	ON_WM_DESTROY()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CEditUnderWrite::CEditUnderWrite()
{
	//���ñ���
	m_bNeedSend=false;
	m_pParentWnd=NULL;
	ZeroMemory(m_szUnderWrite,sizeof(m_szUnderWrite));

	//���ÿؼ�
	m_MissionManager.InsertMissionItem(this);

	return;
}

//��������
CEditUnderWrite::~CEditUnderWrite()
{
}

//��Ϣ����
BOOL CEditUnderWrite::PreTranslateMessage(MSG * pMsg)
{
	//ȡ����Ϣ
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		//�رտؼ�
		CloseUnderWrite();

		return TRUE;
	}

	//ȷ����Ϣ
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_RETURN))
	{
		//���ý���
		ASSERT(m_pParentWnd!=NULL);
		if (m_pParentWnd!=NULL) m_pParentWnd->SetFocus();

		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//�����¼�
bool CEditUnderWrite::OnEventMissionLink(INT nErrorCode)
{
	//�����ж�
	if (nErrorCode!=0L)
	{
		//������ʾ
		CInformation Information(m_pParentWnd);
		Information.ShowMessageBox(TEXT("����������ʧ�ܣ�����ǩ������ʧ�ܣ�"),MB_ICONERROR,30L);

		return true;
	}

	//���ñ���
	m_bNeedSend=false;

	//��������
	ASSERT(GetMissionManager()!=NULL);
	CMissionManager * pMissionManager=GetMissionManager();

	//�û���Ϣ
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��������
	CMD_GP_ModifyUnderWrite ModifyUnderWrite;
	ZeroMemory(&ModifyUnderWrite,sizeof(ModifyUnderWrite));

	//��������
	ModifyUnderWrite.dwUserID=pGlobalUserData->dwUserID;
	lstrcpyn(ModifyUnderWrite.szUnderWrite,m_szUnderWrite,CountArray(ModifyUnderWrite.szUnderWrite));
	lstrcpyn(ModifyUnderWrite.szPassword,pGlobalUserData->szPassword,CountArray(ModifyUnderWrite.szPassword));

	//��������
	WORD wHeadSize=sizeof(ModifyUnderWrite)-sizeof(ModifyUnderWrite.szUnderWrite);
	pMissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_MODIFY_UNDER_WRITE,&ModifyUnderWrite,wHeadSize+CountStringBuffer(ModifyUnderWrite.szUnderWrite));

	return true;
}

//�ر��¼�
bool CEditUnderWrite::OnEventMissionShut(BYTE cbShutReason)
{
	//�رմ���
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		//��������
		CMissionManager * pMissionManager=GetMissionManager();
		if (pMissionManager->AvtiveMissionItem(this,true)==true) return true;

		//���ÿؼ�
		CloseUnderWrite();

		//��ʾ��ʾ
		CInformation Information(m_pParentWnd);
		Information.ShowMessageBox(TEXT("���ڵ�ǰ����������æ������ǩ���޸�ʧ�ܣ����Ժ������ԣ�"),MB_ICONERROR);
	}

	return true;
}

//��ȡ�¼�
bool CEditUnderWrite::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//�����
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_OPERATE_FAILURE:	//����ʧ��
			{
				//Ч�����
				CMD_GP_OperateFailure * pOperateFailure=(CMD_GP_OperateFailure *)pData;
				ASSERT(wDataSize>=(sizeof(CMD_GP_OperateFailure)-sizeof(pOperateFailure->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GP_OperateFailure)-sizeof(pOperateFailure->szDescribeString))) return false;

				//�ر�����
				CMissionManager * pMissionManager=GetMissionManager();
				if (pMissionManager!=NULL) pMissionManager->ConcludeMissionItem(this,false);

				//���ÿؼ�
				CloseUnderWrite();

				//��ʾ��Ϣ
				if (pOperateFailure->szDescribeString[0]!=0)
				{
					CInformation Information(m_pParentWnd);
					Information.ShowMessageBox(pOperateFailure->szDescribeString,MB_ICONERROR,60);
				}

				return true;
			}
		case SUB_GP_OPERATE_SUCCESS:	//�����ɹ�
			{
				//��������
				CMD_GP_OperateSuccess * pOperateSuccess=(CMD_GP_OperateSuccess *)pData;

				//Ч������
				ASSERT(wDataSize>=(sizeof(CMD_GP_OperateSuccess)-sizeof(pOperateSuccess->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GP_OperateSuccess)-sizeof(pOperateSuccess->szDescribeString))) return false;

				//�ر�����
				CMissionManager * pMissionManager=GetMissionManager();
				if (pMissionManager!=NULL) pMissionManager->ConcludeMissionItem(this,false);

				//������Ϣ
				CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
				lstrcpyn(pGlobalUserData->szUnderWrite,m_szUnderWrite,CountArray(pGlobalUserData->szUnderWrite));

				//���ÿؼ�
				CloseUnderWrite();

				//�����¼�
				CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
				if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);

				//��ʾ��Ϣ
				if (pOperateSuccess->szDescribeString[0]!=0)
				{
					CInformation Information(m_pParentWnd);
					Information.ShowMessageBox(pOperateSuccess->szDescribeString,MB_ICONINFORMATION,60);
				}

				return true;
			}
		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//�޸��ж�
bool CEditUnderWrite::IsModifyStatus()
{
	//״̬�ж�
	if (m_bNeedSend==true) return true;
	if (GetActiveStatus()==true) return true;

	return false;
}

//���ô���
VOID CEditUnderWrite::SetParentWindow(CWnd * pParentWnd)
{
	//���ñ���
	m_pParentWnd=pParentWnd;

	return;
}

//�رտؼ�
VOID CEditUnderWrite::CloseUnderWrite()
{
	//�ر��ж�
	if ((m_bNeedSend==false)&&(GetActiveStatus()==false))
	{
		//���ٴ���
		DestroyWindow();
	}
	else
	{
		//���ش���
		ShowWindow(SW_HIDE);
	}

	//���½���
	ASSERT(m_pParentWnd!=NULL);
	m_pParentWnd->RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);

	return;
}

//����ǩ��
VOID CEditUnderWrite::UpdateUnderWrite()
{
	if (IsWindowVisible()==TRUE)
	{
		//��ȡ��Ϣ
		TCHAR szUnderWrite[LEN_UNDER_WRITE]=TEXT("");
		GetWindowText(szUnderWrite,CountArray(szUnderWrite));

		//�û���Ϣ
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//�����ж�
		bool bActiveMission=false;
		if ((IsModifyStatus()==true)&&(lstrcmp(szUnderWrite,m_szUnderWrite)!=0L)) bActiveMission=true;
		if ((IsModifyStatus()==false)&&(lstrcmp(szUnderWrite,pGlobalUserData->szUnderWrite)!=0L)) bActiveMission=true;

		//�����ж�
		if (bActiveMission==true)
		{
			//������Ϣ
			m_bNeedSend=true;
			lstrcpyn(m_szUnderWrite,szUnderWrite,CountArray(m_szUnderWrite));

			//��ֹ����
			CMissionManager * pMissionManager=GetMissionManager();
			if (GetActiveStatus()==true) pMissionManager->ConcludeMissionItem(this,false);

			//���õ�ַ
			LPCTSTR pszCurrentServer=pMissionManager->GetCurrentServer();
			if ((pszCurrentServer!=NULL)&&(pszCurrentServer[0]!=0)) pMissionManager->SetCustomServer(pszCurrentServer);

			//��������
			pMissionManager->AvtiveMissionItem(this,false);

			//���ؿؼ�
			ShowWindow(SW_HIDE);

			//���½���
			ASSERT(m_pParentWnd!=NULL);
			m_pParentWnd->RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
		}
		else
		{
			//�رտؼ�
			CloseUnderWrite();
		}
	}

	return;
}

//������Ϣ
VOID CEditUnderWrite::OnDestroy()
{
	__super::OnDestroy();

	//��ֹ����
	if (GetActiveStatus()==true)
	{
		//��������
		CMissionManager * pMissionManager=GetMissionManager();
		if (pMissionManager!=NULL) pMissionManager->ConcludeMissionItem(this,false);

		//���½���
		ASSERT(m_pParentWnd!=NULL);
		m_pParentWnd->RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//ʧȥ����
VOID CEditUnderWrite::OnKillFocus(CWnd * pNewWnd)
{
	__super::OnKillFocus(pNewWnd);

	//�رտؼ�
	UpdateUnderWrite();

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CWndUserInfoCtrl::CWndUserInfoCtrl()
{
	//״̬��־
	m_bClickFace=false;
	m_bMouseEvent=false;

	//������־
	m_bHoverFace=false;
	m_bHoverUnderWrite=false;

	//��������
	m_rcFaceArea.SetRect(13,14,69,70);

	//���ÿؼ�
	m_EditUnderWrite.SetParentWindow(this);

	return;
}

//��������
CWndUserInfoCtrl::~CWndUserInfoCtrl()
{
}

//��Ϣ����
BOOL CWndUserInfoCtrl::PreTranslateMessage(MSG * pMsg)
{
	//��ʾ��Ϣ
	if (m_ToolTipCtrl.m_hWnd!=NULL)
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}

	return __super::PreTranslateMessage(pMsg);
}

//�����
BOOL CWndUserInfoCtrl::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	UINT nCommandID=LOWORD(wParam);

	//���ܰ�ť
	switch (nCommandID)
	{
	case IDC_USER_INFO_1:		//�Զ�ͷ��
		{
			//��ʾ����
			CDlgCustomFace DlgCustomFace;
			DlgCustomFace.SetCustomFaceEvent(CPlatformFrame::GetInstance());

			//��ʾ����
			DlgCustomFace.DoModal();
						
			return TRUE;
		}
	case IDC_USER_INFO_2:		//��������
		{
			//��ʾ����
			CDlgLockMachine DlgLockMachine;
			DlgLockMachine.DoModal();

			return TRUE;
		}
	case IDC_USER_INFO_3:		//�����һ�
		{
			return TRUE;
		}
	case IDC_USER_INFO_SET:		//��������
		{
			//��������
			CDlgService DlgService;
			DlgService.DoModal();

			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//λ����Ϣ
VOID CWndUserInfoCtrl::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//�����ؼ�
	RectifyControl(cx,cy);

	return;
}

//������Ϣ
INT CWndUserInfoCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//ע���¼�
	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
	if (pPlatformEvent!=NULL) pPlatformEvent->RegisterEventWnd(m_hWnd);

	//������Դ
	CBitImage ImageTitleM;
	ImageTitleM.LoadFromResource(AfxGetInstanceHandle(),IDB_USER_INFO_M);

	//������ť
	CRect rcCreate(0,0,0,0);
	m_btUserInfo1.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_USER_INFO_1);
	m_btUserInfo2.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_USER_INFO_2);
	m_btUserInfo3.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_USER_INFO_3);
	m_btUserInfoSet.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_USER_INFO_SET);

	//���ð�ť
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btUserInfo1.SetButtonImage(IDB_BK_USER_INFO,TEXT("BT_USER_INFO_1"),hInstance,true,false);
	m_btUserInfo2.SetButtonImage(IDB_BK_USER_INFO,TEXT("BT_USER_INFO_2"),hInstance,true,false);
	m_btUserInfo3.SetButtonImage(IDB_BK_USER_INFO,TEXT("BT_USER_INFO_3"),hInstance,true,false);
	m_btUserInfoSet.SetButtonImage(IDB_BK_USER_INFO_SET,TEXT("BT_USER_INFO_SET"),hInstance,true,false);

	//�ƶ�����
	CRect rcUserInfo;
	GetWindowRect(&rcUserInfo);
	SetWindowPos(NULL,0,0,rcUserInfo.Width(),ImageTitleM.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	//��ʾ�ؼ�
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(&m_btUserInfo1,TEXT("�ϴ�ͷ��"));
	m_ToolTipCtrl.AddTool(&m_btUserInfo2,TEXT("��������"));
	m_ToolTipCtrl.AddTool(&m_btUserInfo3,TEXT("�����һ�"));
	m_ToolTipCtrl.AddTool(&m_btUserInfoSet,TEXT("������Ϣ����"));

	return 0;
}

//�����ؼ�
VOID CWndUserInfoCtrl::RectifyControl(INT nWidth, INT nHeight)
{
	//��������
	m_rcUnderWrite.SetRect(127,52,nWidth-13,72);

	//��������
	HDWP hDwp=BeginDeferWindowPos(64);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER|SWP_NOSIZE;

	//��ť��С
	CRect rcButton;
	m_btUserInfo1.GetWindowRect(&rcButton);

	//�������
	UINT nSpace=14;
	UINT nStartPos=18;

	//������ť
	DeferWindowPos(hDwp,m_btUserInfoSet,NULL,69,15,0,0,uFlags);
	DeferWindowPos(hDwp,m_btUserInfo1,NULL,nStartPos,nHeight-rcButton.Height()-9,0,0,uFlags);
	DeferWindowPos(hDwp,m_btUserInfo2,NULL,nStartPos+rcButton.Width()+nSpace,nHeight-rcButton.Height()-9,0,0,uFlags);
	DeferWindowPos(hDwp,m_btUserInfo3,NULL,nStartPos+(rcButton.Width()+nSpace)*2,nHeight-rcButton.Height()-9,0,0,uFlags);

	//��������
	EndDeferWindowPos(hDwp);

	return;
}

//�滭����
BOOL CWndUserInfoCtrl::OnEraseBkgnd(CDC * pDC)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//���� DC
	CImageDC BufferDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	//��������
	HINSTANCE hInstance=AfxGetInstanceHandle();
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();

	//������Դ
	CBitImage ImageTitleL;
	CBitImage ImageTitleM;
	CBitImage ImageTitleR;
	ImageTitleL.LoadFromResource(hInstance,IDB_USER_INFO_L);
	ImageTitleM.LoadFromResource(hInstance,IDB_USER_INFO_M);
	ImageTitleR.LoadFromResource(hInstance,IDB_USER_INFO_R);

	//��Ⱦ��Դ
	pSkinRenderManager->RenderImage(ImageTitleL);
	pSkinRenderManager->RenderImage(ImageTitleM);
	pSkinRenderManager->RenderImage(ImageTitleR);

	//�滭����
	ImageTitleL.BitBlt(BufferDC,0,0);

	//����м�
	for (INT nXPos=ImageTitleL.GetWidth();nXPos<rcClient.Width();nXPos+=ImageTitleM.GetWidth())
	{
		ImageTitleM.BitBlt(BufferDC,nXPos,0);
	}

	//�滭����
	ImageTitleR.BitBlt(BufferDC,rcClient.Width()-ImageTitleR.GetWidth(),0);

	//���û���
	pBufferDC->SetBkMode(TRANSPARENT);
	pBufferDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//�û���Ϣ
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//�滭�û�
	if (pGlobalUserData->dwUserID!=0L)
	{
		//�滭�׿�
		if ((m_bHoverUnderWrite==true)||((m_EditUnderWrite.m_hWnd!=NULL)&&(m_EditUnderWrite.IsWindowVisible()==TRUE)))
		{
			//������Դ
			CPngImage ImageUnderWriteL;
			CPngImage ImageUnderWriteM;
			CPngImage ImageUnderWriteR;
			ImageUnderWriteL.LoadImage(AfxGetInstanceHandle(),TEXT("UNDER_WRITE_L"));
			ImageUnderWriteM.LoadImage(AfxGetInstanceHandle(),TEXT("UNDER_WRITE_M"));
			ImageUnderWriteR.LoadImage(AfxGetInstanceHandle(),TEXT("UNDER_WRITE_R"));

			//�滭����
			ImageUnderWriteL.DrawImage(pBufferDC,m_rcUnderWrite.left,m_rcUnderWrite.top);
			ImageUnderWriteR.DrawImage(pBufferDC,m_rcUnderWrite.right-ImageUnderWriteR.GetWidth(),m_rcUnderWrite.top);

			//�滭�м�
			INT nXStart=m_rcUnderWrite.left+ImageUnderWriteL.GetWidth();
			INT nXTerminate=m_rcUnderWrite.right-ImageUnderWriteR.GetWidth();
			for (INT nXPos=nXStart;nXPos<nXTerminate;nXPos+=ImageUnderWriteM.GetWidth())
			{
				ImageUnderWriteM.DrawImage(pBufferDC,nXPos,m_rcUnderWrite.top,__min(nXTerminate-nXPos,ImageUnderWriteM.GetWidth()),ImageUnderWriteM.GetHeight(),0,0);
			}
		}

		//�滭�߿�
		if (m_bHoverFace==true)
		{
			theApp.m_FaceItemControlModule->DrawFaceItemFrame(pBufferDC,17,18,ITEM_FRAME_HOVER);
		}

		//�滭ͷ��
		if (pGlobalUserData->CustomFaceInfo.dwDataSize!=0)
		{
			DWORD * pdwCustomFace=pGlobalUserData->CustomFaceInfo.dwCustomFace;
			theApp.m_FaceItemControlModule->DrawFaceNormal(pBufferDC,16,17,pdwCustomFace);
		}
		else
		{
			theApp.m_FaceItemControlModule->DrawFaceNormal(pBufferDC,16,17,pGlobalUserData->wFaceID);
		}

		//�滭����
		pBufferDC->SetTextColor(RGB(0,0,0));
		pBufferDC->TextOut(88,16,TEXT("�� �ƣ�"));
		pBufferDC->TextOut(88,36,TEXT("�� �ƣ�"));
		pBufferDC->TextOut(88,56,TEXT("ǩ ����"));

		//���콱��
		TCHAR szUserMedal[64]=TEXT("");
		_sntprintf(szUserMedal,CountArray(szUserMedal),TEXT("%ld"),pGlobalUserData->dwUserMedal);

		//�������
		CRect rcUserMedal;
		rcUserMedal.SetRect(130,36,rcClient.Width()-15,48);
		pBufferDC->DrawText(szUserMedal,lstrlen(szUserMedal),&rcUserMedal,DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

		//����ʺ�
		CRect rcNickName;
		rcNickName.SetRect(130,16,rcClient.Width()-15,28);
		pBufferDC->DrawText(pGlobalUserData->szNickName,lstrlen(pGlobalUserData->szNickName),&rcNickName,DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

		//��������
		LPCTSTR pszUnderWrite=NULL;
		COLORREF crUnderWrite=RGB(0,0,0);

		//ǩ��λ��
		CRect rcUnderWrite;
		rcUnderWrite.SetRect(130,56,rcClient.Width()-15,68);

		//����״̬
		if ((m_EditUnderWrite.IsModifyStatus()==false)&&(pGlobalUserData->szUnderWrite[0]!=0))
		{
			crUnderWrite=RGB(0,0,0);
			pszUnderWrite=pGlobalUserData->szUnderWrite;
		}

		//�޸�״̬
		if ((m_EditUnderWrite.IsModifyStatus()==true)&&(m_EditUnderWrite.m_szUnderWrite[0]!=0))
		{
			crUnderWrite=RGB(100,100,100);
			pszUnderWrite=m_EditUnderWrite.m_szUnderWrite;
		}

		//��ʾ״̬
		if (pszUnderWrite==NULL)
		{
			crUnderWrite=RGB(150,150,150);
			pszUnderWrite=TEXT("�༭����ǩ��");
		}

		//���ǩ��
		pBufferDC->SetTextColor(crUnderWrite);
		pBufferDC->DrawText(pszUnderWrite,lstrlen(pszUnderWrite),&rcUnderWrite,DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
	}

	//�滭����
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return TRUE;
}

//�����Ϣ
VOID CWndUserInfoCtrl::OnLButtonUp(UINT nFlags, CPoint MousePoint)
{
	__super::OnLButtonUp(nFlags,MousePoint);

	if ((m_bHoverFace==true)&&(m_bClickFace==true))
	{
		//�ͷŲ���
		ReleaseCapture();

		//���ñ���
		m_bClickFace=false;

		//������Ϣ
		if (m_rcFaceArea.PtInRect(MousePoint)==TRUE)
		{
			CDlgService DlgService;
			DlgService.DoModal();
		}
	}

	return;
}

//�����Ϣ
VOID CWndUserInfoCtrl::OnLButtonDown(UINT nFlags, CPoint MousePoint)
{
	__super::OnLButtonDown(nFlags,MousePoint);

	//���ý���
	SetFocus();

	//��������
	if ((m_bHoverFace==true)&&(m_bClickFace==false))
	{
		//����˻�
		SetCapture();

		//���ñ���
		m_bClickFace=true;
	}

	//�༭ǩ��
	if (m_bHoverUnderWrite==true)
	{
		//�û���Ϣ
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//�����ؼ�
		if (m_EditUnderWrite.m_hWnd==NULL)
		{
			//�����ؼ�
			m_EditUnderWrite.Create(WS_CHILD|ES_AUTOHSCROLL,CRect(0,0,0,0),this,IDC_USER_INFO_EDIT);

			//������Ϣ
			m_EditUnderWrite.LimitText(LEN_UNDER_WRITE-1L);
			m_EditUnderWrite.SetWindowText(pGlobalUserData->szUnderWrite);

			//���ÿؼ�
			m_EditUnderWrite.SetFont(&CSkinResourceManager::GetInstance()->GetDefaultFont());
		}

		//����λ��
		CRect rcClient;
		GetClientRect(&rcClient);
		m_EditUnderWrite.SetWindowPos(NULL,130,56,rcClient.Width()-145,14,SWP_SHOWWINDOW|SWP_NOZORDER);

		//���ÿؼ�
		m_EditUnderWrite.SetFocus();
		m_EditUnderWrite.SetSel(0L,-1L);

		//���´���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//�����Ϣ
BOOL CWndUserInfoCtrl::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//��������
	bool bMouseEvent=false;
	bool bRedrawWindow=false;

	//�����ж�
	if ((m_bHoverFace==false)&&(m_rcFaceArea.PtInRect(MousePoint)==TRUE))
	{
		//���ñ���
		bMouseEvent=true;
		bRedrawWindow=true;

		//���ñ���
		m_bHoverFace=true;
	}

	//�뿪�ж�
	if ((m_bHoverFace==true)&&(m_rcFaceArea.PtInRect(MousePoint)==FALSE))
	{
		//���ñ���
		bRedrawWindow=true;

		//���ñ���
		m_bHoverFace=false;
	}

	//�����ж�
	if ((m_bHoverUnderWrite==false)&&(m_rcUnderWrite.PtInRect(MousePoint)==TRUE))
	{
		//���ñ���
		bRedrawWindow=true;

		//���ñ���
		m_bHoverUnderWrite=true;
	}

	//�뿪�ж�
	if ((m_bHoverUnderWrite==true)&&(m_rcUnderWrite.PtInRect(MousePoint)==FALSE))
	{
		//���ñ���
		bRedrawWindow=true;

		//���ñ���
		m_bHoverUnderWrite=false;
	}

	//ע���¼�
	if ((m_bMouseEvent==false)&&(bMouseEvent==true))
	{
		//���ñ���
		m_bMouseEvent=true;

		//��������
		TRACKMOUSEEVENT TrackMouseEvent;
		ZeroMemory(&TrackMouseEvent,sizeof(TrackMouseEvent));

		//ע����Ϣ
		TrackMouseEvent.hwndTrack=m_hWnd;
		TrackMouseEvent.dwFlags=TME_LEAVE;
		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);

		//ע���¼�
		_TrackMouseEvent(&TrackMouseEvent);
	}

	//���½���
	if (bRedrawWindow==true)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	//���ù��
	if (m_bHoverFace==true)
	{
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_HAND_CUR)));
		return true;
	}

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//�����Ϣ
LRESULT CWndUserInfoCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	//���ñ���
	m_bMouseEvent=false;

	//�뿪����
	if (m_bHoverFace==true)
	{
		//���ñ���
		m_bMouseEvent=false;

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	//�뿪����
	if (m_bHoverUnderWrite==true)
	{
		//���ñ���
		m_bHoverUnderWrite=false;

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return 0;
}

//�¼���Ϣ
LRESULT CWndUserInfoCtrl::OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case EVENT_USER_LOGON:			//��¼�ɹ�
		{
			//���´���
			Invalidate();

			//���ð�ť
			m_btUserInfo1.EnableWindow(TRUE);
			m_btUserInfo2.EnableWindow(TRUE);
			m_btUserInfo3.EnableWindow(TRUE);
			m_btUserInfoSet.EnableWindow(TRUE);

			//�û���Ϣ
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			//���ð�ť
			if (pGlobalUserData->cbMoorMachine==TRUE)
			{
				HINSTANCE hInstance=AfxGetInstanceHandle();
				m_btUserInfo2.SetButtonImage(IDB_BK_USER_INFO,TEXT("BT_USER_INFO_4"),hInstance,true,false);
			}

			return 0L;
		}
	case EVENT_USER_LOGOUT:			//ע���ɹ�
		{
			//���´���
			Invalidate();

			//���ٿؼ�
			m_EditUnderWrite.DestroyWindow();

			//���ð�ť
			m_btUserInfo1.EnableWindow(FALSE);
			m_btUserInfo2.EnableWindow(FALSE);
			m_btUserInfo3.EnableWindow(FALSE);
			m_btUserInfoSet.EnableWindow(FALSE);

			//���ð�ť
			HINSTANCE hInstance=AfxGetInstanceHandle();
			m_btUserInfo2.SetButtonImage(IDB_BK_USER_INFO,TEXT("BT_USER_INFO_2"),hInstance,true,false);

			return 0L;
		}
	case EVENT_USER_INFO_UPDATE:	//���ϸ���
		{
			//���´���
			Invalidate();

			return 0L;
		}
	case EVENT_USER_MOOR_MACHINE:	//�󶨻���
		{
			//�û���Ϣ
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			//���ð�ť
			if (pGlobalUserData->cbMoorMachine==FALSE)
			{
				HINSTANCE hInstance=AfxGetInstanceHandle();
				m_btUserInfo2.SetButtonImage(IDB_BK_USER_INFO,TEXT("BT_USER_INFO_2"),hInstance,true,false);
			}
			else
			{
				HINSTANCE hInstance=AfxGetInstanceHandle();
				m_btUserInfo2.SetButtonImage(IDB_BK_USER_INFO,TEXT("BT_USER_INFO_4"),hInstance,true,false);
			}

			return 0L;
		}
	}

	return 0L;
}

//////////////////////////////////////////////////////////////////////////////////
