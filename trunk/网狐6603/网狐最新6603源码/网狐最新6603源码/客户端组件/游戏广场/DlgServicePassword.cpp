#include "StdAfx.h"
#include "GlobalUnits.h"
#include "DlgServicePassword.h"

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgServicePassword, CDlgServiceItem)
	ON_WM_ERASEBKGND()
	ON_EN_CHANGE(IDC_LOGON_PASSWORD2, OnEnChangeLogonPassword)
	ON_EN_CHANGE(IDC_INSURE_PASSWORD2, OnEnChangeInserePassword)
	ON_BN_CLICKED(IDC_MODIFY_LOGON_PASSWORD, OnBnClickedLogonPassword)
	ON_BN_CLICKED(IDC_MODIFY_INSURE_PASSWORD, OnBnClickedInsurePassword)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgServicePassword::CDlgServicePassword() : CDlgServiceItem(IDD_DLG_SERVICE_PASSWORD)
{
	//����ͼ��
	m_cbImageIndex=1;

	//��������
	m_MissionManager.InsertMissionItem(this);

	//���ñ���
	m_cbLogonPasswordLevel=PASSWORD_LEVEL_0;
	m_cbInsurePasswordLevel=PASSWORD_LEVEL_0;

	//���ñ���
	m_bLogonMission=false;
	ZeroMemory(m_szSrcPassword,sizeof(m_szSrcPassword));
	ZeroMemory(m_szDesPassword,sizeof(m_szDesPassword));

	return;
}

//��������
CDlgServicePassword::~CDlgServicePassword()
{
}

//�ؼ���
VOID CDlgServicePassword::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	//�ʺ�����
	DDX_Control(pDX, IDC_LOGON_PASSWORD1, m_edLogonPassword1);
	DDX_Control(pDX, IDC_LOGON_PASSWORD2, m_edLogonPassword2);
	DDX_Control(pDX, IDC_LOGON_PASSWORD3, m_edLogonPassword3);

	//��������
	DDX_Control(pDX, IDC_INSURE_PASSWORD1, m_edInsurePassword1);
	DDX_Control(pDX, IDC_INSURE_PASSWORD2, m_edInsurePassword2);
	DDX_Control(pDX, IDC_INSURE_PASSWORD3, m_edInsurePassword3);

	//��ť����
	DDX_Control(pDX, IDC_MODIFY_LOGON_PASSWORD, m_btLogonPassword);
	DDX_Control(pDX, IDC_MODIFY_INSURE_PASSWORD, m_btInsurePassword);
}

//��Ϣ����
BOOL CDlgServicePassword::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_RETURN))
	{
		//��������
		CWnd * pWndFouns=GetFocus();
		TCHAR szPassword[LEN_PASSWORD]=TEXT("");

		//�����ж�
		if (m_edLogonPassword1.IsChild(pWndFouns))
		{
			m_edLogonPassword1.GetUserPassword(szPassword);
			if (szPassword[0]!=0) m_edLogonPassword2.SetFocus();

			return TRUE;
		}

		//�����ж�
		if (m_edLogonPassword2.IsChild(pWndFouns))
		{
			m_edLogonPassword2.GetUserPassword(szPassword);
			if (szPassword[0]!=0) m_edLogonPassword3.SetFocus();

			return TRUE;
		}

		//�����ж�
		if (m_edLogonPassword3.IsChild(pWndFouns))
		{
			m_edLogonPassword3.GetUserPassword(szPassword);
			if (szPassword[0]!=0) OnBnClickedLogonPassword();

			return TRUE;
		}

		//�����ж�
		if (m_edInsurePassword1.IsChild(pWndFouns))
		{
			m_edInsurePassword1.GetUserPassword(szPassword);
			if (szPassword[0]!=0) m_edInsurePassword2.SetFocus();

			return TRUE;
		}

		//�����ж�
		if (m_edInsurePassword2.IsChild(pWndFouns))
		{
			m_edInsurePassword2.GetUserPassword(szPassword);
			if (szPassword[0]!=0) m_edInsurePassword3.SetFocus();

			return TRUE;
		}

		//�����ж�
		if (m_edInsurePassword3.IsChild(pWndFouns))
		{
			m_edInsurePassword3.GetUserPassword(szPassword);
			if (szPassword[0]!=0) OnBnClickedInsurePassword();

			return TRUE;
		}
	}

	return __super::PreTranslateMessage(pMsg);
}

//�����¼�
bool CDlgServicePassword::OnEventMissionLink(INT nErrorCode)
{
	//���ӽ��
	if (nErrorCode!=0)
	{
		//��������
		if (m_MissionManager.AvtiveMissionItem(this,true)==true)
		{
			return true;
		}

		//�¼�����
		OnMissionConclude();

		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("���ӷ�������ʱ�������޸�ʧ�ܣ�"),MB_OK|MB_ICONSTOP,30);

		return true;
	}

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��¼����
	if (m_bLogonMission==true)
	{
		//��������
		CMD_GP_ModifyLogonPass ModifyLogonPass;
		ZeroMemory(&ModifyLogonPass,sizeof(ModifyLogonPass));

		//��������
		TCHAR szSrcPassword[LEN_PASSWORD]=TEXT("");
		TCHAR szDesPassword[LEN_PASSWORD]=TEXT("");
		CWHEncrypt::MD5Encrypt(m_szSrcPassword,szSrcPassword);
		CWHEncrypt::MD5Encrypt(m_szDesPassword,szDesPassword);

		//��������
		ModifyLogonPass.dwUserID=pGlobalUserData->dwUserID;
		lstrcpyn(ModifyLogonPass.szScrPassword,szSrcPassword,CountArray(ModifyLogonPass.szScrPassword));
		lstrcpyn(ModifyLogonPass.szDesPassword,szDesPassword,CountArray(ModifyLogonPass.szDesPassword));

		//��������
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_MODIFY_LOGON_PASS,&ModifyLogonPass,sizeof(ModifyLogonPass));
	}
	else
	{
		//��������
		CMD_GP_ModifyInsurePass ModifyInsurePass;
		ZeroMemory(&ModifyInsurePass,sizeof(ModifyInsurePass));

		//��������
		TCHAR szSrcPassword[LEN_PASSWORD]=TEXT("");
		TCHAR szDesPassword[LEN_PASSWORD]=TEXT("");
		CWHEncrypt::MD5Encrypt(m_szSrcPassword,szSrcPassword);
		CWHEncrypt::MD5Encrypt(m_szDesPassword,szDesPassword);

		//��������
		ModifyInsurePass.dwUserID=pGlobalUserData->dwUserID;
		lstrcpyn(ModifyInsurePass.szScrPassword,szSrcPassword,CountArray(ModifyInsurePass.szScrPassword));
		lstrcpyn(ModifyInsurePass.szDesPassword,szDesPassword,CountArray(ModifyInsurePass.szDesPassword));

		//��������
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_MODIFY_INSURE_PASS,&ModifyInsurePass,sizeof(ModifyInsurePass));
	}

	return true;
}

//�ر��¼�
bool CDlgServicePassword::OnEventMissionShut(BYTE cbShutReason)
{
	//��ʾ��Ϣ
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		//��������
		if (m_MissionManager.AvtiveMissionItem(this,true)==true)
		{
			return true;
		}

		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("��������������쳣�Ͽ��ˣ��û������޸�ʧ�ܣ�"),MB_ICONERROR,30);
	}
	
	//�¼�����
	OnMissionConclude();

	return true;
}

//��ȡ�¼�
bool CDlgServicePassword::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
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

				//ʧ�ܴ���
				OnMissionConclude();

				//�ر�����
				m_MissionManager.ConcludeMissionItem(this,false);

				//��ʾ��Ϣ
				if (pOperateFailure->szDescribeString[0]!=0)
				{
					CInformation Information(this);
					Information.ShowMessageBox(pOperateFailure->szDescribeString,MB_ICONERROR,60);
				}

				//���ý���
				if (m_bLogonMission==true)
				{
					m_edLogonPassword1.SetFocus();
				}
				else
				{
					m_edInsurePassword1.SetFocus();
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
				m_MissionManager.ConcludeMissionItem(this,false);

				//�ʺ�����
				if (m_bLogonMission==true)
				{
					//��������
					CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
					CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
					tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

					//��������
					TCHAR szDesPassword[LEN_PASSWORD]=TEXT("");
					CWHEncrypt::MD5Encrypt(m_szDesPassword,szDesPassword);
					lstrcpyn(pGlobalUserData->szPassword,szDesPassword,CountArray(pGlobalUserData->szPassword));

					//��¼����
					if (pParameterGlobal->m_bRemberPassword==true)
					{
						//������Ϣ
						TCHAR szKeyUserID[16];
						_sntprintf(szKeyUserID,CountArray(szKeyUserID),TEXT("%s\\%ld"),REG_USER_INFO,pGlobalUserData->dwUserID);

						//�ϴ��û�
						CWHRegKey RegUserInfo;
						RegUserInfo.OpenRegKey(REG_USER_INFO,true);

						//�û�����
						CWHRegKey RegUserItem;
						RegUserItem.OpenRegKey(szKeyUserID,true);

						//��������
						TCHAR szPassBuffer[MAX_ENCRYPT_LEN]=TEXT("");
						CWHEncrypt::XorEncrypt(m_szDesPassword,szPassBuffer,CountArray(szPassBuffer));

						//�ʺ���Ϣ
						RegUserItem.WriteString(TEXT("UserPassword"),szPassBuffer);
						RegUserItem.WriteString(TEXT("UserPassToken"),pGlobalUserData->szPassword);
					}
				}

				//��ʾ��Ϣ
				if (pOperateSuccess->szDescribeString[0]!=0)
				{
					CInformation Information(this);
					Information.ShowMessageBox(pOperateSuccess->szDescribeString,MB_ICONINFORMATION,60);
				}

				//�ɹ�����
				OnMissionConclude();
                
				//���ý���
				if (m_bLogonMission==true)
				{
					m_edLogonPassword1.SetFocus();
				}
				else
				{
					m_edInsurePassword1.SetFocus();
				}

				return true;
			}
		}
	}

	return true;
}

//��ʼ����
VOID CDlgServicePassword::OnMissionStart()
{
	//���ܰ�ť
	m_btLogonPassword.EnableWindow(FALSE);
	m_btInsurePassword.EnableWindow(FALSE);

	return;
}

//��ֹ����
VOID CDlgServicePassword::OnMissionConclude()
{
	//���ܰ�ť
	m_btLogonPassword.EnableWindow(TRUE);
	m_btInsurePassword.EnableWindow(TRUE);

	//���ÿؼ�
	m_edLogonPassword1.SetUserPassword(TEXT(""));
	m_edLogonPassword2.SetUserPassword(TEXT(""));
	m_edLogonPassword3.SetUserPassword(TEXT(""));

	//���ÿؼ�
	m_edInsurePassword1.SetUserPassword(TEXT(""));
	m_edInsurePassword2.SetUserPassword(TEXT(""));
	m_edInsurePassword3.SetUserPassword(TEXT(""));

	return;
}

//�滭����
BOOL CDlgServicePassword::OnEraseBkgnd(CDC * pDC)
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

	//������Դ
	CPngImage ImagePassword;
	ImagePassword.LoadImage(AfxGetInstanceHandle(),TEXT("PASSWORD_LEVEL"));

	//��ȡ��С
	CSize SizePassword;
	SizePassword.SetSize(ImagePassword.GetWidth()/3,ImagePassword.GetHeight()/2);

	//��䱳��
	pBufferDC->FillSolidRect(&rcClient,CSkinDialog::m_SkinAttribute.m_crBackGround);

	//�滭�ȼ�
	ImagePassword.DrawImage(pBufferDC,230,76,SizePassword.cx*3,SizePassword.cy,0,0);
	ImagePassword.DrawImage(pBufferDC,230,256,SizePassword.cx*3,SizePassword.cy,0,0);

	//�滭����
	if (m_cbLogonPasswordLevel>=PASSWORD_LEVEL_1)
	{
		INT nImagePos=(m_cbLogonPasswordLevel-PASSWORD_LEVEL_1)*SizePassword.cx;
		ImagePassword.DrawImage(pBufferDC,230+nImagePos,76,SizePassword.cx,SizePassword.cy,nImagePos,SizePassword.cy);
	}

	//�滭����
	if (m_cbInsurePasswordLevel>=PASSWORD_LEVEL_1)
	{
		INT nImagePos=(m_cbInsurePasswordLevel-PASSWORD_LEVEL_1)*SizePassword.cx;
		ImagePassword.DrawImage(pBufferDC,230+nImagePos,256,SizePassword.cx,SizePassword.cy,nImagePos,SizePassword.cy);
	}

	//�滭����
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return TRUE;
}

//��������
VOID CDlgServicePassword::OnEnChangeLogonPassword()
{
	//��ȡ����
	TCHAR szPassword[LEN_PASSWORD]=TEXT("");
	m_edLogonPassword2.GetUserPassword(szPassword);

	//�ȼ��ж�
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
	BYTE cbPasswordLevel=pUserItemElement->GetPasswordLevel(szPassword);

	//���´���
	if (m_cbLogonPasswordLevel!=cbPasswordLevel)
	{
		//���ñ���
		m_cbLogonPasswordLevel=cbPasswordLevel;

		//���´���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);
	}

	return;
}

//��������
VOID CDlgServicePassword::OnEnChangeInserePassword()
{
	//��ȡ����
	TCHAR szPassword[LEN_PASSWORD]=TEXT("");
	m_edInsurePassword2.GetUserPassword(szPassword);

	//�ȼ��ж�
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
	BYTE cbPasswordLevel=pUserItemElement->GetPasswordLevel(szPassword);

	//���´���
	if (m_cbInsurePasswordLevel!=cbPasswordLevel)
	{
		//���ñ���
		m_cbInsurePasswordLevel=cbPasswordLevel;

		//���´���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);
	}

	return;
}

//�����޸�
VOID CDlgServicePassword::OnBnClickedLogonPassword()
{
	//��������
	TCHAR szSrcPassword[LEN_PASSWORD];
	TCHAR szDesPassword[2][LEN_PASSWORD];

	//��������
	TCHAR szDescribe[128]=TEXT("");
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��ȡ��Ϣ
	m_edLogonPassword1.GetUserPassword(szSrcPassword);
	m_edLogonPassword2.GetUserPassword(szDesPassword[0]);
	m_edLogonPassword3.GetUserPassword(szDesPassword[1]);

	//��������
	TCHAR szEncryptPassword[LEN_PASSWORD]=TEXT("");
	CWHEncrypt::MD5Encrypt(szSrcPassword,szEncryptPassword);

	//�����ж�
	if (szSrcPassword[0]==0)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("ԭ�ʺ�����������������룡"),MB_ICONERROR,0);

		//���ý���
		m_edLogonPassword1.SetFocus();

		return;
	}

	//�����ж�
	if (lstrcmp(szEncryptPassword,pGlobalUserData->szPassword)!=0)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("ԭ�ʺ�����������������룡"),MB_ICONERROR,0);

		//���ý���
		m_edLogonPassword1.SetFocus();

		return;
	}

	//�����ж�
	if (lstrcmp(szDesPassword[0],szDesPassword[1])!=0)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("������������ʺ����벻һ�£����������룡"),MB_ICONERROR,0);

		//���ý���
		m_edLogonPassword2.SetFocus();

		return;
	}

	//�����ж�
	if (pUserItemElement->EfficacyPassword(szDesPassword[0],szDescribe,CountArray(szDescribe))==false)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//���ý���
		m_edLogonPassword2.SetFocus();

		return;
	}

	//һ���ж�
	if (lstrcmp(szSrcPassword,szDesPassword[0])==0)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("�����������������ԭ����һ�£����������������룡"),MB_ICONERROR,0);

		//���ý���
		m_edLogonPassword2.SetFocus();

		//��������
		m_edLogonPassword2.SetUserPassword(TEXT(""));
		m_edLogonPassword3.SetUserPassword(TEXT(""));

		return;
	}

	//���ñ���
	m_bLogonMission=true;
	lstrcpyn(m_szSrcPassword,szSrcPassword,CountArray(m_szSrcPassword));
	lstrcpyn(m_szDesPassword,szDesPassword[0],CountArray(m_szDesPassword));

	//�¼�����
	OnMissionStart();

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false) 
	{
		OnMissionConclude();
		return;
	}

	return;
}

//�����޸�
VOID CDlgServicePassword::OnBnClickedInsurePassword()
{
	//��������
	TCHAR szSrcPassword[LEN_PASSWORD];
	TCHAR szDesPassword[2][LEN_PASSWORD];

	//��ȡ��Ϣ
	m_edInsurePassword1.GetUserPassword(szSrcPassword);
	m_edInsurePassword2.GetUserPassword(szDesPassword[0]);
	m_edInsurePassword3.GetUserPassword(szDesPassword[1]);

	//�����ж�
	if (szSrcPassword[0]==0)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("ԭ���չ�����������������룡"),MB_ICONERROR,0);

		m_edLogonPassword1.SetFocus();

		return;
	}

	//�����ж�
	if (lstrcmp(szDesPassword[0],szDesPassword[1])!=0)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("����������±��չ�һ�£����������룡"),MB_ICONERROR,0);

		//���ý���
		m_edInsurePassword2.SetFocus();

		return;
	}

	//��������
	TCHAR szDescribe[128]=TEXT("");
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();

	//�����ж�
	if (pUserItemElement->EfficacyPassword(szDesPassword[0],szDescribe,CountArray(szDescribe))==false)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("�±��չ����벻��Ϊ�գ����������룡"),MB_ICONERROR,0);

		//���ý���
		m_edInsurePassword2.SetFocus();

		return;
	}

	//һ���ж�
	if (lstrcmp(szSrcPassword,szDesPassword[0])==0)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("�����������������ԭ����һ�£����������������룡"),MB_ICONERROR,0);

		//���ý���
		m_edInsurePassword2.SetFocus();

		//��������
		m_edInsurePassword2.SetUserPassword(TEXT(""));
		m_edInsurePassword3.SetUserPassword(TEXT(""));

		return;
	}

	//���ñ���
	m_bLogonMission=false;
	lstrcpyn(m_szSrcPassword,szSrcPassword,CountArray(m_szSrcPassword));
	lstrcpyn(m_szDesPassword,szDesPassword[0],CountArray(m_szDesPassword));

	//�¼�����
	OnMissionStart();

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false) 
	{
		OnMissionConclude();
		return;
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
