#include "StdAfx.h"
#include "GlobalUnits.h"
#include "MissionLogon.h"
#include "PlatformEvent.h"
#include "PlatformFrame.h"

//////////////////////////////////////////////////////////////////////////////////

//��̬����
CMissionLogon * CMissionLogon::m_pMissionLogon=NULL;					//����ָ��

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CMissionLogon, CDlgStatus)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CMissionLogon::CMissionLogon()
{
	//���ñ���
	m_bRegister=false;
	m_bRemPassword=false;

	//�ؼ�ָ��
	m_pDlgLogon=NULL;
	m_pDlgRegister=NULL;

	//�������
	SetStatusViewSink(this);

	//���ö���
	ASSERT(m_pMissionLogon==NULL);
	if (m_pMissionLogon==NULL) m_pMissionLogon=this;

	return;
}

//��������
CMissionLogon::~CMissionLogon()
{
	//ɾ������
	SafeDelete(m_pDlgLogon);
	SafeDelete(m_pDlgRegister);

	//�ͷŶ���
	ASSERT(m_pMissionLogon==this);
	if (m_pMissionLogon==this) m_pMissionLogon=NULL;

	return;
}

//ȡ������
VOID CMissionLogon::OnStatusCancel()
{
	//���ؽ���
	HideStatusWindow();

	//��ֹ����
	CMissionManager * pMissionManager=GetMissionManager();
	if (pMissionManager!=NULL) pMissionManager->ConcludeMissionItem(this,true);

	//���µ�¼
	if (m_bRegister==false)
	{
		ShowLogon();
	}
	else
	{
		ShowRegister();
	}

	return;
}

//�����¼�
bool CMissionLogon::OnEventMissionLink(INT nErrorCode)
{
	//������
	if (nErrorCode!=0L)
	{
		//�ر���ʾ
		HideStatusWindow();

		//��ʾ��ʾ
		CInformation Information(CPlatformFrame::GetInstance());
		Information.ShowMessageBox(TEXT("���������еķ��������޷��ɹ����ӷ���������������վά�����棡"),MB_ICONERROR);

		//���µ�¼
		if (m_bRegister==false)
		{
			ShowLogon();
		}
		else
		{
			ShowRegister();
		}
	}
	else
	{
		//��������
		WORD wPacketSize,wSubCmdID;
		BYTE cbBuffer[SOCKET_TCP_PACKET];

		//��������
		if (m_bRegister==false)
		{
			wPacketSize=m_pDlgLogon->ConstructLogonPacket(cbBuffer,sizeof(cbBuffer));
			wSubCmdID=(m_pDlgLogon->m_cbLogonMode==LOGON_BY_GAME_ID)?SUB_GP_LOGON_GAMEID:SUB_GP_LOGON_ACCOUNTS;
		}
		else
		{
			wSubCmdID=SUB_GP_REGISTER_ACCOUNTS;
			wPacketSize=m_pDlgRegister->ConstructRegisterPacket(cbBuffer,sizeof(cbBuffer));
		}

		//��ʾ��Ϣ
		ShowStatusWindow(TEXT("������֤�û���¼��Ϣ..."));

		//��������
		ASSERT(GetMissionManager()!=NULL);
		GetMissionManager()->SendData(MDM_GP_LOGON,wSubCmdID,cbBuffer,wPacketSize);
	}

	return true;
}

//�ر��¼�
bool CMissionLogon::OnEventMissionShut(BYTE cbShutReason)
{
	//�رմ���
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		//��������
		CMissionManager * pMissionManager=GetMissionManager();
		if (pMissionManager->AvtiveMissionItem(this,true)==true) return true;

		//�ر���ʾ
		HideStatusWindow();

		//��ʾ��ʾ
		CInformation Information(CPlatformFrame::GetInstance());
		Information.ShowMessageBox(TEXT("���������еķ��������޷��ɹ����ӷ���������������վά��������Ժ������ԣ�"),MB_ICONERROR);

		//���µ�¼
		if (m_bRegister==false)
		{
			ShowLogon();
		}
		else
		{
			ShowRegister();
		}
	}

	return true;
}

//��ȡ�¼�
bool CMissionLogon::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	if (Command.wMainCmdID==MDM_GP_LOGON)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_LOGON_SUCCESS:	//��¼�ɹ�
			{
				return OnSocketSubLogonSuccess(pData,wDataSize);
			}
		case SUB_GP_LOGON_FAILURE:	//��¼ʧ��
			{
				return OnSocketSubLogonFailure(pData,wDataSize);
			}
		case SUB_GP_LOGON_FINISH:	//��¼���
			{
				return OnSocketSubLogonFinish(pData,wDataSize);
			}
		case SUB_GP_UPDATE_NOTIFY:	//������ʾ
			{
				return OnSocketSubUpdateNotify(pData,wDataSize);
			}
		}
	}

	return false;
}

//��ʾ��¼
VOID CMissionLogon::ShowLogon()
{
	//Ч��״̬
	ASSERT(GetActiveStatus()==false);
	if (GetActiveStatus()==true) return;

	//���ñ���
	m_bRegister=false;

	//��������
	if (m_pDlgLogon==NULL)
	{
		m_pDlgLogon=new CDlgLogon;
	}

	//��������
	if (m_pDlgLogon->m_hWnd==NULL)
	{
		m_pDlgLogon->Create(IDD_DLG_LOGON,GetDesktopWindow());
	}

	//��ʾ����
	if (GetActiveStatus()==false)
	{
		m_pDlgLogon->ShowWindow(SW_SHOW);
		m_pDlgLogon->SetForegroundWindow();
	}

	//���ش���
	if ((m_pDlgRegister!=NULL)&&(m_pDlgRegister->m_hWnd!=NULL))
	{
		m_pDlgRegister->ShowWindow(SW_HIDE);
	}

	return;
}

//��ʾע��
VOID CMissionLogon::ShowRegister()
{
	//Ч��״̬
	ASSERT(GetActiveStatus()==false);
	if (GetActiveStatus()==true) return;

	//���ñ���
	m_bRegister=true;

	//��������
	if (m_pDlgRegister==NULL)
	{
		m_pDlgRegister=new CDlgRegister;
	}
	
	//��������
	if (m_pDlgRegister->m_hWnd==NULL)
	{
		m_pDlgRegister->Create(IDD_DLG_REGISTER,GetDesktopWindow());
	}

	//��ʾ����
	m_pDlgRegister->ShowWindow(SW_SHOW);
	m_pDlgRegister->SetForegroundWindow();

	//���ش���
	if ((m_pDlgLogon!=NULL)&&(m_pDlgLogon->m_hWnd!=NULL))
	{
		m_pDlgLogon->ShowWindow(SW_HIDE);
	}

	return;
}

//��¼�û�
VOID CMissionLogon::UpdateUserInfo()
{
	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//���ñ�־
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
	if (pParameterGlobal!=NULL) pParameterGlobal->m_bRemberPassword=m_bRemPassword;

	//������Ϣ
	TCHAR szUserID[16];
	_sntprintf(szUserID,CountArray(szUserID),TEXT("%ld"),pGlobalUserData->dwUserID);

	//�ϴ��û�
	CWHRegKey RegUserInfo;
	RegUserInfo.OpenRegKey(REG_USER_INFO,true);
	RegUserInfo.WriteValue(TEXT("LastUserID"),pGlobalUserData->dwUserID);

	//��������
	TCHAR szGameID[16]=TEXT("");
	TCHAR szPassBuffer[MAX_ENCRYPT_LEN]=TEXT("");

	//�û�����
	CWHRegKey RegUserItem;
	RegUserItem.Attach(RegUserInfo.CreateItemKey(szUserID));

	//��Ϸ��ʶ
	if (pGlobalUserData->dwGameID!=0L)
	{
		_sntprintf(szGameID,CountArray(szGameID),TEXT("%ld"),pGlobalUserData->dwGameID);
	}

	//��������
	if (m_bRemPassword==true)
	{
		if (m_bRegister==false)
		{
			LPCTSTR pszPassword=m_pDlgLogon->m_szPassword;
			CWHEncrypt::XorEncrypt(pszPassword,szPassBuffer,CountArray(szPassBuffer));
		}
		else
		{
			LPCTSTR pszPassword=m_pDlgRegister->m_szLogonPass;
			CWHEncrypt::XorEncrypt(pszPassword,szPassBuffer,CountArray(szPassBuffer));
		}
	}

	//������Ϣ
	RegUserItem.WriteValue(TEXT("FaceID"),pGlobalUserData->wFaceID);
	RegUserItem.WriteValue(TEXT("AcitveTimeToken"),(DWORD)time(NULL));

	//�ʺ���Ϣ
	RegUserItem.WriteString(TEXT("GameID"),szGameID);
	RegUserItem.WriteString(TEXT("UserPassword"),szPassBuffer);
	RegUserItem.WriteString(TEXT("UserAccount"),pGlobalUserData->szAccounts);
	RegUserItem.WriteString(TEXT("UserPassToken"),pGlobalUserData->szPassword);

	return;
}

//ִ�е�¼
bool CMissionLogon::PerformLogonMission(bool bRemPassword)
{
	//Ч��״̬
	ASSERT(GetActiveStatus()==false);
	if (GetActiveStatus()==true) return false;

	//��ȡ��ַ
	TCHAR szCustomServer[LEN_SERVER]=TEXT("");
	if (m_pDlgLogon!=NULL) lstrcpyn(szCustomServer,m_pDlgLogon->m_szLogonServer,CountArray(szCustomServer));

	//���õ�ַ
	CMissionManager * pMissionManager=GetMissionManager();
	if (pMissionManager!=NULL) pMissionManager->SetCustomServer(szCustomServer);

	//��������
	if (pMissionManager->AvtiveMissionItem(this,false)==false)
	{
		//������ʾ
		CInformation Information(CPlatformFrame::GetInstance());
		Information.ShowMessageBox(TEXT("����������ʧ�ܣ��������������Լ���������״����"),MB_ICONERROR);

		//���µ�¼
		if (m_bRegister==false)
		{
			ShowLogon();
		}
		else
		{
			ShowRegister();
		}

		return false;
	}

	//���ñ���
	m_bRemPassword=bRemPassword;

	//д���ַ
	CGlobalServer GlobalServer;
	GlobalServer.SetLastServerName(szCustomServer);

	//��ʾ����
	ShowStatusWindow(TEXT("�������ӷ��������������Ժ�Ƭ��..."));

	return true;
}

//��¼�ɹ�
bool CMissionLogon::OnSocketSubLogonSuccess(VOID * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize>=sizeof(CMD_GP_LogonSuccess));
	if (wDataSize<sizeof(CMD_GP_LogonSuccess)) return false;

	//��Ϣ����
	CMD_GP_LogonSuccess * pLogonSuccess=(CMD_GP_LogonSuccess *)pData;

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//������Ϣ
	pGlobalUserData->wFaceID=pLogonSuccess->wFaceID;
	pGlobalUserData->cbGender=pLogonSuccess->cbGender;
	pGlobalUserData->dwGameID=pLogonSuccess->dwGameID;
	pGlobalUserData->dwUserID=pLogonSuccess->dwUserID;
	pGlobalUserData->dwCustomID=pLogonSuccess->dwCustomID;
	pGlobalUserData->dwUserMedal=pLogonSuccess->dwUserMedal;
	pGlobalUserData->dwExperience=pLogonSuccess->dwExperience;
	pGlobalUserData->dwLoveLiness=pLogonSuccess->dwLoveLiness;
	pGlobalUserData->cbMoorMachine=pLogonSuccess->cbMoorMachine;

	//�ʺ���Ϣ
	lstrcpyn(pGlobalUserData->szAccounts,pLogonSuccess->szAccounts,CountArray(pGlobalUserData->szAccounts));
	lstrcpyn(pGlobalUserData->szNickName,pLogonSuccess->szNickName,CountArray(pGlobalUserData->szNickName));

	//��չ��Ϣ
	VOID * pDataBuffer=NULL;
	tagDataDescribe DataDescribe;
	CRecvPacketHelper RecvPacket(pLogonSuccess+1,wDataSize-sizeof(CMD_GP_LogonSuccess));
	while (true)
	{
		//��ȡ����
		pDataBuffer=RecvPacket.GetData(DataDescribe);
		if (DataDescribe.wDataDescribe==DTP_NULL) break;

		//��ȡ����
		switch (DataDescribe.wDataDescribe)
		{
		case DTP_GP_MEMBER_INFO:	//��Ա��Ϣ
			{
				ASSERT(DataDescribe.wDataSize==sizeof(DTP_GP_MemberInfo));
				if (DataDescribe.wDataSize==sizeof(DTP_GP_MemberInfo))
				{
					DTP_GP_MemberInfo * pMemberInfo=(DTP_GP_MemberInfo *)pDataBuffer;
					pGlobalUserData->cbMemberOrder=pMemberInfo->cbMemberOrder;
					pGlobalUserData->MemberOverDate=pMemberInfo->MemberOverDate;
				}

				break;
			}
		case DTP_GP_UNDER_WRITE:	//����ǩ��
			{
				ASSERT(DataDescribe.wDataSize<=sizeof(pGlobalUserData->szUnderWrite));
				if (DataDescribe.wDataSize<=sizeof(pGlobalUserData->szUnderWrite))
				{
					pGlobalUserData->szUnderWrite[DataDescribe.wDataSize/sizeof(TCHAR)-1L]=0;
					CopyMemory(pGlobalUserData->szUnderWrite,pDataBuffer,DataDescribe.wDataSize);
				}

				break;
			}
		}
	}

	//���ؼ���
	if (pLogonSuccess->dwCustomID!=0)
	{
		CCustomFaceManager * pCustomFaceManager=CCustomFaceManager::GetInstance();
		pCustomFaceManager->LoadUserCustomFace(pLogonSuccess->dwUserID,pLogonSuccess->dwCustomID,pGlobalUserData->CustomFaceInfo);
	}

	//������ʾ
	ShowStatusWindow(TEXT("���ڻ�ȡ��Ϸ�б�..."));

	return true;
}

//��¼ʧ��
bool CMissionLogon::OnSocketSubLogonFailure(VOID * pData, WORD wDataSize)
{
	//��������
	CMD_GP_LogonFailure * pLogonFailure=(CMD_GP_LogonFailure *)pData;

	//Ч������
	ASSERT(wDataSize>=(sizeof(CMD_GP_LogonFailure)-sizeof(pLogonFailure->szDescribeString)));
	if (wDataSize<(sizeof(CMD_GP_LogonFailure)-sizeof(pLogonFailure->szDescribeString))) return false;

	//�رմ���
	CMissionManager * pMissionManager=GetMissionManager();
	if (pMissionManager!=NULL) pMissionManager->ConcludeMissionItem(this,false);

	//���Ƚ���
	HideStatusWindow();

	//��ʾ��Ϣ
	LPCTSTR pszDescribeString(pLogonFailure->szDescribeString);
	if (lstrlen(pszDescribeString)>0) ShowInformation(pszDescribeString,MB_ICONERROR,60);

	//���µ�¼
	if (m_bRegister==false)
	{
		ShowLogon();
	}
	else
	{
		ShowRegister();
	}

	return true;
}

//��¼���
bool CMissionLogon::OnSocketSubLogonFinish(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GP_LogonFinish));
	if (wDataSize!=sizeof(CMD_GP_LogonFinish)) return false;

	//��������
	CMD_GP_LogonFinish * pLogonFinish=(CMD_GP_LogonFinish *)pData;

	//��������
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//ʱ������
	pParameterGlobal->m_wIntermitTime=pLogonFinish->wIntermitTime;
	pParameterGlobal->m_wOnLineCountTime=pLogonFinish->wOnLineCountTime;

	//������Ϣ
	UpdateUserInfo();

	//�رմ���
	HideStatusWindow();

	//ɾ���ؼ�
	SafeDelete(m_pDlgLogon);
	SafeDelete(m_pDlgRegister);

	//����ҳ
	ASSERT(CPlatformFrame::GetInstance()!=NULL);
	CPlatformFrame::GetInstance()->WebBrowse(szPlatformLink,false);

	//��ֹ����
	CMissionManager * pMissionManager=GetMissionManager();
	if (pMissionManager!=NULL) pMissionManager->ConcludeMissionItem(this,false);

	//�����¼�
	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
	if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_LOGON,0L);

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//�������
	if ((pGlobalUserData->dwCustomID!=0)&&(pGlobalUserData->CustomFaceInfo.dwDataSize==0L))
	{
		CCustomFaceManager * pCustomFaceManager=CCustomFaceManager::GetInstance();
		pCustomFaceManager->LoadUserCustomFace(pGlobalUserData->dwUserID,pGlobalUserData->dwCustomID);
	}

	return true;
}

//������ʾ
bool CMissionLogon::OnSocketSubUpdateNotify(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GP_UpdateNotify));
	if (wDataSize!=sizeof(CMD_GP_UpdateNotify)) return false;

	//���ñ���
	CMD_GP_UpdateNotify * pUpdateNotify=(CMD_GP_UpdateNotify *)pData;

	//��ֹ����
	CMissionManager * pMissionManager=GetMissionManager();
	if (pMissionManager!=NULL) pMissionManager->ConcludeMissionItem(this,false);

	//���ش���
	HideStatusWindow();

	//��ʾ��Ϣ
	if ((pUpdateNotify->cbMustUpdate==TRUE)||(pUpdateNotify->cbAdviceUpdate==TRUE))
	{
		//������ʾ
		if (pUpdateNotify->cbMustUpdate==FALSE)
		{
			//������ʾ
			LPCTSTR pszString=TEXT("��Ϸ�����������������ڵİ汾���ܼ���ʹ�ã��������������ظ��£�");

			//�˳��ж�
			CInformation Information(CPlatformFrame::GetInstance());
			if (Information.ShowMessageBox(pszString,MB_ICONINFORMATION|MB_YESNO,0)!=IDYES) return true;
		}
	}

	//���ش���
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	if (pGlobalUnits!=NULL) pGlobalUnits->DownLoadClient(TEXT("��Ϸ����"),0,0);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
