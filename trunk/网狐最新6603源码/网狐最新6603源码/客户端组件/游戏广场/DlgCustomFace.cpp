#include "Stdafx.h"
#include "Resource.h"
#include "DlgCustomFace.h"

//////////////////////////////////////////////////////////////////////////////////

//�༭ģʽ
#define MM_SOURCE					0									//ԭ��ģʽ
#define MM_CUSTOM					1									//�Զ�ģʽ
#define MM_SYSTEM					2									//ϵͳģʽ

//���Ͷ���
#define FACE_CUSTOM_EDIT			0									//�Զ�ͷ��
#define FACE_SELECT_ITEM			1									//ѡ��ͷ��

//�ؼ���ʶ
#define IDC_FACE_CUSTOM_WND			100									//�Զ�ͷ��
#define IDC_FACE_SELECT_WND			101									//ѡ��ͷ��

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgCustomFace, CSkinDialog)
	ON_NOTIFY(TCN_SELCHANGE,IDC_TAB_FACE,OnTcnSelAlterFaceMode)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgCustomFace::CDlgCustomFace() : CSkinDialog(IDD_DLG_COSTOM_FACE)
{
	//���ñ���
	m_pICustomFaceEvent=NULL;

	//���ñ���
	m_cbMode=MM_SOURCE;
	m_wFaceID=INVALID_WORD;
	ZeroMemory(&m_CustomFaceInfo,sizeof(m_CustomFaceInfo));

	//�������
	m_MissionManager.InsertMissionItem(this);

	//���ýӿ�
	m_FaceItemCustomWnd.SetImageEditorEvent(this);
	m_FaceItemSelectWnd.SetFaceItemSelectEvent(this);

	return;
}

//��������
CDlgCustomFace::~CDlgCustomFace()
{
}

//ͼ���¼�
VOID CDlgCustomFace::OnEventFaceSelect(WORD wFaceID)
{
	//���ñ���
	m_wFaceID=wFaceID;

	//����ģʽ
	m_cbMode=MM_SYSTEM;

	//���ÿؼ�
	m_FaceItemCustomWnd.DestoryImage();

	//���½���
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

	return;
}

//ͼ���¼�
VOID CDlgCustomFace::OnEventImageSelect(bool bLoadImage, bool bOperateing)
{
	//���ñ���
	m_cbMode=MM_CUSTOM;

	//���½���
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

	return;
}

//�ؼ���
VOID CDlgCustomFace::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	//�󶨿ؼ�
	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_TAB_FACE, m_TabControl);
}

//��������
BOOL CDlgCustomFace::OnInitDialog()
{
	__super::OnInitDialog();

	//��ȡ��С
	CRect rcClient;
	GetClientRect(&rcClient);

	//���ñ���
	SetWindowText(TEXT("����ͷ��"));

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//���ñ���
	m_wFaceID=pGlobalUserData->wFaceID;
	CopyMemory(&m_CustomFaceInfo,&pGlobalUserData->CustomFaceInfo,sizeof(m_CustomFaceInfo));

	//���ÿؼ�
	m_FaceItemSelectWnd.SetAllowItemFull();

	//���ÿؼ�
	m_TabControl.SetItemSize(CSize(150,25));
	m_TabControl.InsertItem(FACE_CUSTOM_EDIT,TEXT(" �Զ�ͷ�� "));
	m_TabControl.InsertItem(FACE_SELECT_ITEM,TEXT(" ϵͳͷ�� "));

	//��������
	CRect rcCreate(0,0,0,0);
	m_FaceItemSelectWnd.Create(NULL,NULL,WS_CHILD,rcCreate,&m_TabControl,IDC_FACE_SELECT_WND);
	m_FaceItemCustomWnd.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rcCreate,&m_TabControl,IDC_FACE_CUSTOM_WND);

	//����λ��
	CRect rcTabCtrl(10,40,rcClient.Width()-110,rcClient.Height()-45);
	m_FaceItemCustomWnd.SetWindowPos(NULL,1,27,rcTabCtrl.Width()-2,rcTabCtrl.Height()-28,SWP_NOZORDER);
	m_FaceItemSelectWnd.SetWindowPos(NULL,1,27,rcTabCtrl.Width()-2,rcTabCtrl.Height()-28,SWP_NOZORDER);
	m_TabControl.SetWindowPos(NULL,rcTabCtrl.left,rcTabCtrl.top,rcTabCtrl.Width(),rcTabCtrl.Height(),SWP_NOZORDER);

	return TRUE;
}

//�滭����
VOID CDlgCustomFace::OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight)
{
	__super::OnDrawClientArea(pDC,nWidth,nHeight);

	//��ȡ����
	ASSERT(CSkinResourceManager::GetInstance()!=NULL);
	CSkinResourceManager * pSkinResourceManager=CSkinResourceManager::GetInstance();

	//���û���
	pDC->SetTextColor(RGB(0,0,0));
	pDC->SelectObject(pSkinResourceManager->GetDefaultFont());

	//�������
	LPCTSTR pszTitle=TEXT("Ԥ����");
	pDC->TextOut(nWidth-FACE_CX-47,88,pszTitle,lstrlen(pszTitle));

	//�滭�߿�
	CFaceItemControl * pIFaceItemControl=CFaceItemControl::GetInstance();
	pIFaceItemControl->DrawFaceItemFrame(pDC,nWidth-FACE_CX-32,112,ITEM_FRAME_HOVER);

	//�滭ͷ��
	switch (m_cbMode)
	{
	case MM_SOURCE:		//ԭ��ģʽ
		{
			if (m_CustomFaceInfo.dwDataSize!=0L)
			{
				IFaceItemControl * pIFaceItemControl=CFaceItemControl::GetInstance();
				pIFaceItemControl->DrawFaceNormal(pDC,nWidth-FACE_CX-32,112,m_CustomFaceInfo.dwCustomFace);
			}
			else
			{
				IFaceItemControl * pIFaceItemControl=CFaceItemControl::GetInstance();
				pIFaceItemControl->DrawFaceNormal(pDC,nWidth-FACE_CX-32,112,m_wFaceID);
			}

			break;
		}
	case MM_SYSTEM:		//ϵͳģʽ
		{
			IFaceItemControl * pIFaceItemControl=CFaceItemControl::GetInstance();
			pIFaceItemControl->DrawFaceNormal(pDC,nWidth-FACE_CX-32,112,m_wFaceID);

			break;
		}
	case MM_CUSTOM:		//�Զ�ģʽ
		{
			//�Զ�ͷ��
			ASSERT(m_FaceItemCustomWnd.IsNull()==false);
			m_FaceItemCustomWnd.DrawEditImage(pDC,nWidth-FACE_CX-32,112,FACE_CX,FACE_CY);

			break;
		}
	}

	return;
}

//�����¼�
bool CDlgCustomFace::OnEventMissionLink(INT nErrorCode)
{
	//�����ж�
	if (nErrorCode==0L)
	{
		//��������
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//������Ϣ 
		switch (m_cbMode)
		{
		case MM_SYSTEM:		//ϵͳģʽ
			{
				//��������
				CMD_GP_SystemFaceInfo SystemFaceInfo;
				ZeroMemory(&SystemFaceInfo,sizeof(SystemFaceInfo));

				//���ñ���
				SystemFaceInfo.wFaceID=m_wFaceID;
				SystemFaceInfo.dwUserID=pGlobalUserData->dwUserID;
				CWHService::GetMachineID(SystemFaceInfo.szMachineID);
				CopyMemory(SystemFaceInfo.szPassword,pGlobalUserData->szPassword,sizeof(SystemFaceInfo.szPassword));

				//��������
				CMissionManager * pMissionManager=GetMissionManager();
				pMissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_SYSTEM_FACE_INFO,&SystemFaceInfo,sizeof(SystemFaceInfo));

				break;
			}
		case MM_CUSTOM:		//�Զ�ģʽ
			{
				//��������
				CMD_GP_CustomFaceInfo CustomFaceInfo;
				ZeroMemory(&CustomFaceInfo,sizeof(CustomFaceInfo));

				//���ñ���
				CustomFaceInfo.dwUserID=pGlobalUserData->dwUserID;
				CWHService::GetMachineID(CustomFaceInfo.szMachineID);
				CopyMemory(CustomFaceInfo.szPassword,pGlobalUserData->szPassword,sizeof(CustomFaceInfo.szPassword));
				CopyMemory(CustomFaceInfo.dwCustomFace,m_CustomFaceInfo.dwCustomFace,sizeof(CustomFaceInfo.dwCustomFace));

				//��������
				CMissionManager * pMissionManager=GetMissionManager();
				pMissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_CUSTOM_FACE_INFO,&CustomFaceInfo,sizeof(CustomFaceInfo));

				break;
			}
		}

		return true;
	}
	else
	{
		//��ʾ�ؼ�
		EnableControl(true);

		//��ʾ��ʾ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("�޷����ӷ�������ͷ���޸�ʧ�ܣ����������Ƿ�ͨ��"),MB_ICONERROR);
	}

	return true;
}

//�ر��¼�
bool CDlgCustomFace::OnEventMissionShut(BYTE cbShutReason)
{
	//�رմ���
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		//��������
		CMissionManager * pMissionManager=GetMissionManager();
		if (pMissionManager->AvtiveMissionItem(this,true)==true) return true;

		//��ʾ�ؼ�
		EnableControl(true);

		//��ʾ��ʾ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("���ڵ�ǰ����������æ���ϴ��Զ���ͷ��ʧ�ܣ����Ժ������ԣ�"),MB_ICONERROR);
	}

	return true;
}

//��ȡ�¼�
bool CDlgCustomFace::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//�����ɹ�
	if ((Command.wMainCmdID==MDM_GP_USER_SERVICE)&&(Command.wSubCmdID==SUB_GP_USER_FACE_INFO))
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(CMD_GP_UserFaceInfo));
		if (wDataSize!=sizeof(CMD_GP_UserFaceInfo)) return false;

		//��Ϣ����
		CMD_GP_UserFaceInfo * pUserFaceInfo=(CMD_GP_UserFaceInfo *)pData;

		//��������
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//��������
		if (pUserFaceInfo->dwCustomID==0)
		{
			//�¼�֪ͨ
			ASSERT(m_pICustomFaceEvent!=NULL);
			if (m_pICustomFaceEvent!=NULL) m_pICustomFaceEvent->OnEventSystemFace(pGlobalUserData->dwUserID,pUserFaceInfo->wFaceID);
		}
		else
		{
			//��������
			CCustomFaceManager * pCustomFaceManager=CCustomFaceManager::GetInstance();
			pCustomFaceManager->SaveUserCustomFace(pGlobalUserData->dwUserID,pUserFaceInfo->dwCustomID,m_CustomFaceInfo.dwCustomFace);

			//�¼�֪ͨ
			ASSERT(m_pICustomFaceEvent!=NULL);
			if (m_pICustomFaceEvent!=NULL) m_pICustomFaceEvent->OnEventCustomFace(pGlobalUserData->dwUserID,pUserFaceInfo->dwCustomID,m_CustomFaceInfo);
		}

		//�رմ���
		EndDialog(IDOK);

		return true;
	}

	return true;
}

//ȷ������
VOID CDlgCustomFace::OnOK()
{
	//�����ж�
	bool bConnect=false;

	//ϵͳģʽ
	if (m_cbMode==MM_SYSTEM)
	{
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		bConnect=(pGlobalUserInfo->GetGlobalUserData()->wFaceID!=m_wFaceID);
	}

	//�Զ�ģʽ
	if (m_cbMode==MM_CUSTOM)
	{
		//��������
		CImage ImageCustomFace;
		ImageCustomFace.Create(FACE_CX,FACE_CY,32);

		//���� DC
		CImageDC BufferDC(ImageCustomFace);
		m_FaceItemCustomWnd.DrawEditImage(CDC::FromHandle(BufferDC),0,0,FACE_CX,FACE_CY);

		//��ȡ����
		INT nImagePitch=ImageCustomFace.GetPitch();
		LPBYTE cbBitCustomFace=(LPBYTE)ImageCustomFace.GetBits();

		//��������
		for (INT nYImagePos=0;nYImagePos<FACE_CY;nYImagePos++)
		{
			for (INT nXImagePos=0;nXImagePos<FACE_CX;nXImagePos++)
			{
				//������ɫ
				DWORD dwImageTarget=nYImagePos*nImagePitch+nXImagePos*4L;
				m_CustomFaceInfo.dwCustomFace[nYImagePos*FACE_CX+nXImagePos]=*(COLORREF *)(cbBitCustomFace+dwImageTarget);
			}
		}

		//���ñ���
		bConnect=true;
		m_CustomFaceInfo.dwDataSize=sizeof(m_CustomFaceInfo);
	}

	//��������
	if (bConnect==true)
	{
		//�ؼ�����
		EnableControl(false);

		//��������
		m_MissionManager.AvtiveMissionItem(this,false);

		return;
	}

	__super::OnOK();
}

//���ýӿ�
bool CDlgCustomFace::SetCustomFaceEvent(IUnknownEx * pIUnknownEx)
{
	//���ýӿ�
	if (pIUnknownEx!=NULL)
	{
		//��ѯ�ӿ�
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ICustomFaceEvent)!=NULL);
		m_pICustomFaceEvent=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ICustomFaceEvent);

		//�ɹ��ж�
		if (m_pICustomFaceEvent==NULL) return false;
	}
	else m_pICustomFaceEvent=NULL;

	return true;
}

//�ؼ�����
VOID CDlgCustomFace::EnableControl(bool bEnableControl)
{
	//���ÿؼ�
	m_btOk.EnableWindow((bEnableControl==true)?TRUE:FALSE);
	m_TabControl.EnableWindow((bEnableControl==true)?TRUE:FALSE);
	m_FaceItemCustomWnd.EnableWindow((bEnableControl==true)?TRUE:FALSE);
	m_FaceItemSelectWnd.EnableWindow((bEnableControl==true)?TRUE:FALSE);

	return;
}

//���͸ı�
VOID CDlgCustomFace::OnTcnSelAlterFaceMode(NMHDR * pNMHDR, LRESULT * pResult)
{
	switch (m_TabControl.GetCurSel())
	{
	case FACE_CUSTOM_EDIT:		//�Զ�ͷ��
		{
			//��ʾ�ؼ�
			m_FaceItemCustomWnd.ShowWindow(SW_SHOW);
			m_FaceItemSelectWnd.ShowWindow(SW_HIDE);

			//���ý���
			m_FaceItemCustomWnd.SetFocus();
			m_FaceItemCustomWnd.SetActiveWindow();

			break;
		}
	case FACE_SELECT_ITEM:		//ϵͳͷ��
		{
			//��ʾ�ؼ�
			m_FaceItemCustomWnd.ShowWindow(SW_HIDE);
			m_FaceItemSelectWnd.ShowWindow(SW_SHOW);

			//���ý���
			m_FaceItemSelectWnd.SetFocus();
			m_FaceItemSelectWnd.SetActiveWindow();

			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////
