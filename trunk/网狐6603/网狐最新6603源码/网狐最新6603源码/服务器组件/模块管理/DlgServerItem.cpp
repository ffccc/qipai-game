#include "Stdafx.h"
#include "Resource.h"
#include "DlgServerItem.h"

//////////////////////////////////////////////////////////////////////////////////

//�б�����
#define LIST_STYTE LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_FLATSB

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgServerItem, CDialog)

	//��ť��Ϣ
	ON_BN_CLICKED(IDC_LOAD_SERVER, OnBnClickedLoadServer)
	ON_BN_CLICKED(IDC_RELOAD_LIST, OnBnClickedReLoadList)
	ON_BN_CLICKED(IDC_DELETE_SERVER, OnBnClickedDeleteServer)
	ON_BN_CLICKED(IDC_SERVICE_ATTRIB, OnBnClickedServiceAttrib)

	//�ؼ���Ϣ
	ON_NOTIFY(NM_DBLCLK, IDC_SERVER_LIST, OnNMDblclkServerList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SERVER_LIST, OnLvnItemChangedServerList)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgServerItem::CDlgServerItem() : CDialog(IDD_SERVER_ITEM)
{
	//ģ�����
	ZeroMemory(&m_ModuleInitParameter,sizeof(m_ModuleInitParameter));

	return;
}

//��������
CDlgServerItem::~CDlgServerItem()
{
}

//�ؼ���
VOID CDlgServerItem::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERVER_LIST, m_ServerListControl);
}

//��Ϣ����
BOOL CDlgServerItem::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//��ʼ������
BOOL CDlgServerItem::OnInitDialog()
{
	__super::OnInitDialog();

	//������Դ
	AfxSetResourceHandle(GetModuleHandle(NULL));

	//���ط���
	LoadDBServerItem();

	return TRUE;
}

//ȷ����Ϣ
VOID CDlgServerItem::OnOK()
{
	return;
}

//�򿪷���
bool CDlgServerItem::OpenGameServer()
{
	//������Դ
	AfxSetResourceHandle(GetModuleHandle(MODULE_MANAGER_DLL_NAME));

	//���÷���
	if (DoModal()==IDC_LOAD_SERVER)
	{
		return true;
	}

	return false;
}

//�򿪷���
bool CDlgServerItem::OpenGameServer(WORD wServerID)
{
	//��������
	tagGameServerInfo GameServerResult;
	ZeroMemory(&GameServerResult,sizeof(GameServerResult));

	//������ʶ
	TCHAR szMachineID[LEN_MACHINE_ID];
	CWHService::GetMachineID(szMachineID);

	//���ط���
	CServerInfoManager ServerInfoManager;
	if (ServerInfoManager.LoadGameServerInfo(szMachineID,wServerID,GameServerResult)==false)
	{
		return false;
	}

	//��ȡ����
	if (GetModuleInitParameter(&GameServerResult,true)==false)
	{
		return false;
	}

	return true;
}

//�����б�
bool CDlgServerItem::LoadDBServerItem()
{
	//�����б�
	m_ServerListControl.DeleteAllItems();

	//���ð�ť
	GetDlgItem(IDC_LOAD_SERVER)->EnableWindow(FALSE);
	GetDlgItem(IDC_DELETE_SERVER)->EnableWindow(FALSE);
	GetDlgItem(IDC_SERVICE_ATTRIB)->EnableWindow(FALSE);

	//������ʶ
	TCHAR szMachineID[LEN_MACHINE_ID];
	CWHService::GetMachineID(szMachineID);

	//������Ϣ
	CServerInfoManager ServerInfoManager;
	if (ServerInfoManager.LoadGameServerInfo(szMachineID,m_ServerInfoBuffer)==true)
	{
		//��������
		POSITION Position=m_ServerInfoBuffer.m_GameServerInfoMap.GetStartPosition();

		//ö��ģ��
		while (Position!=NULL)
		{
			//��ȡ����
			WORD wServerID=0L;
			tagGameServerInfo * pGameServerInfo=NULL;
			m_ServerInfoBuffer.m_GameServerInfoMap.GetNextAssoc(Position,wServerID,pGameServerInfo);

			//�����б�
			ASSERT(pGameServerInfo!=NULL);
			if (pGameServerInfo!=NULL) m_ServerListControl.InsertServerInfo(pGameServerInfo);
		}

		return true;
	}

	return false;
}

//���°�ť
bool CDlgServerItem::UpdateControlStatus()
{
	//��ȡѡ��
	POSITION Position=m_ServerListControl.GetFirstSelectedItemPosition();

	//�����б�
	if (Position!=NULL)
	{
		//��ȡ����
		INT nListItem=m_ServerListControl.GetNextSelectedItem(Position);
		tagGameServerInfo * pGameServerInfo=(tagGameServerInfo *)m_ServerListControl.GetItemData(nListItem);

		//���ð�ť
		if (pGameServerInfo->dwNativeVersion!=0L)
		{
			GetDlgItem(IDC_LOAD_SERVER)->EnableWindow(TRUE);
			GetDlgItem(IDC_DELETE_SERVER)->EnableWindow(TRUE);
			GetDlgItem(IDC_SERVICE_ATTRIB)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_LOAD_SERVER)->EnableWindow(FALSE);
			GetDlgItem(IDC_DELETE_SERVER)->EnableWindow(TRUE);
			GetDlgItem(IDC_SERVICE_ATTRIB)->EnableWindow(FALSE);
		}
	}
	else
	{
		//���ð�ť
		GetDlgItem(IDC_LOAD_SERVER)->EnableWindow(FALSE);
		GetDlgItem(IDC_DELETE_SERVER)->EnableWindow(FALSE);
		GetDlgItem(IDC_SERVICE_ATTRIB)->EnableWindow(FALSE);
	}

	return true;
}

//��ȡ����
bool CDlgServerItem::GetModuleInitParameter(tagGameServerInfo * pGameServerInfo, bool bAutoMode)
{
	//Ч�����
	ASSERT(pGameServerInfo!=NULL);
	if (pGameServerInfo==NULL) return false;

	//��Ϸģ��
	CGameServiceManagerHelper GameServiceManager;
	GameServiceManager.SetModuleCreateInfo(pGameServerInfo->szServerDLLName,GAME_SERVICE_CREATE_NAME);

	//�����ж�
	if (pGameServerInfo->dwNativeVersion==0)
	{
		//������ʾ
		TCHAR szString[128]=TEXT("");
		_sntprintf(szString,CountArray(szString),TEXT("[ %s ] ��Ϸ�����������û�а�װ�����Ȱ�װ��Ӧ����Ϸ������"),pGameServerInfo->szGameName);

		//��ʾ��Ϣ
		if (bAutoMode==true) CTraceService::TraceString(szString,TraceLevel_Exception);
		else AfxMessageBox(szString,MB_ICONERROR);

		return false;
	}

	//�����ж�
	if (pGameServerInfo->dwNativeVersion!=pGameServerInfo->dwServerVersion)
	{
		//������ʾ
		TCHAR szString[128]=TEXT("");
		_sntprintf(szString,CountArray(szString),TEXT("[ %s ] ��Ϸ����������Ѿ������ˣ����ܼ���������������"),pGameServerInfo->szGameName);

		//��ʾ��Ϣ
		if (bAutoMode==true) CTraceService::TraceString(szString,TraceLevel_Exception);
		else AfxMessageBox(szString,MB_ICONERROR);

		return false;
	}

	//����ģ��
	if (GameServiceManager.CreateInstance()==false)
	{
		//������ʾ
		TCHAR szString[128]=TEXT("");
		_sntprintf(szString,CountArray(szString),TEXT("[ %s ] ������������ڻ��߼���ʧ�ܣ������°�װ�������"),pGameServerInfo->szGameName);

		//��ʾ��Ϣ
		if (bAutoMode==true) CTraceService::TraceString(szString,TraceLevel_Exception);
		else AfxMessageBox(szString,MB_ICONERROR);

		return false;
	}

	//ģ������
	GameServiceManager->GetServiceAttrib(m_ModuleInitParameter.GameServiceAttrib);

	//�ҽ�����
	m_ModuleInitParameter.GameServiceOption.wKindID=pGameServerInfo->wKindID;
	m_ModuleInitParameter.GameServiceOption.wNodeID=pGameServerInfo->wNodeID;
	m_ModuleInitParameter.GameServiceOption.wSortID=pGameServerInfo->wSortID;
	m_ModuleInitParameter.GameServiceOption.wServerID=pGameServerInfo->wServerID;

	//˰������
	m_ModuleInitParameter.GameServiceOption.lCellScore=pGameServerInfo->lCellScore;
	m_ModuleInitParameter.GameServiceOption.cbRevenueRatio=pGameServerInfo->cbRevenueRatio;

	//��������
	m_ModuleInitParameter.GameServiceOption.lRestrictScore=pGameServerInfo->lRestrictScore;
	m_ModuleInitParameter.GameServiceOption.lMinTableScore=pGameServerInfo->lMinTableScore;
	m_ModuleInitParameter.GameServiceOption.lMinEnterScore=pGameServerInfo->lMinEnterScore;
	m_ModuleInitParameter.GameServiceOption.lMaxEnterScore=pGameServerInfo->lMaxEnterScore;

	//��Ա����
	m_ModuleInitParameter.GameServiceOption.cbMinEnterMember=pGameServerInfo->cbMinEnterMember;
	m_ModuleInitParameter.GameServiceOption.cbMaxEnterMember=pGameServerInfo->cbMaxEnterMember;

	//��������
	m_ModuleInitParameter.GameServiceOption.dwServerRule=pGameServerInfo->dwServerRule;
	m_ModuleInitParameter.GameServiceOption.dwAttachUserRight=pGameServerInfo->dwAttachUserRight;

	//��������
	m_ModuleInitParameter.GameServiceOption.wMaxPlayer=pGameServerInfo->wMaxPlayer;
	m_ModuleInitParameter.GameServiceOption.wTableCount=pGameServerInfo->wTableCount;
	m_ModuleInitParameter.GameServiceOption.wServerPort=pGameServerInfo->wServerPort;
	m_ModuleInitParameter.GameServiceOption.wServerType=pGameServerInfo->wServerType;
	lstrcpyn(m_ModuleInitParameter.GameServiceOption.szServerName,pGameServerInfo->szServerName,LEN_SERVER);

	//��������
	m_ModuleInitParameter.GameServiceOption.cbDistributeRule=pGameServerInfo->cbDistributeRule;
	m_ModuleInitParameter.GameServiceOption.wMinDistributeUser=pGameServerInfo->wMinDistributeUser;
	m_ModuleInitParameter.GameServiceOption.wMaxDistributeUser=pGameServerInfo->wMaxDistributeUser;
	m_ModuleInitParameter.GameServiceOption.wDistributeTimeSpace=pGameServerInfo->wDistributeTimeSpace;
	m_ModuleInitParameter.GameServiceOption.wDistributeDrawCount=pGameServerInfo->wDistributeDrawCount;
	m_ModuleInitParameter.GameServiceOption.wDistributeStartDelay=pGameServerInfo->wDistributeStartDelay;

	//������Ϣ
	lstrcpyn(m_ModuleInitParameter.GameServiceOption.szDataBaseName,pGameServerInfo->szDataBaseName,CountArray(m_ModuleInitParameter.GameServiceOption.szDataBaseName));
	lstrcpyn(m_ModuleInitParameter.GameServiceOption.szDataBaseAddr,pGameServerInfo->szDataBaseAddr,CountArray(m_ModuleInitParameter.GameServiceOption.szDataBaseAddr));

	//��������
	UINT uCustomRuleSize=sizeof(m_ModuleInitParameter.GameServiceOption.cbCustomRule);
	CopyMemory(m_ModuleInitParameter.GameServiceOption.cbCustomRule,pGameServerInfo->cbCustomRule,uCustomRuleSize);

	return true;
}

//���ط���
VOID CDlgServerItem::OnBnClickedLoadServer()
{
	//��ȡѡ��
	ASSERT(m_ServerListControl.GetFirstSelectedItemPosition()!=NULL);
	POSITION Position=m_ServerListControl.GetFirstSelectedItemPosition();
	
	//��ȡ����
	INT nListItem=m_ServerListControl.GetNextSelectedItem(Position);
	tagGameServerInfo * pGameServerInfo=(tagGameServerInfo *)m_ServerListControl.GetItemData(nListItem);

	//���ñ���
	if ((pGameServerInfo!=NULL)&&(GetModuleInitParameter(pGameServerInfo,false)==true))
	{
		EndDialog(IDC_LOAD_SERVER);
	}

	return;
}

//ˢ���б�
VOID CDlgServerItem::OnBnClickedReLoadList()
{
	//�����б�
	LoadDBServerItem();

	return;
}

//ɾ������
VOID CDlgServerItem::OnBnClickedDeleteServer()
{
	//��ȡѡ��
	ASSERT(m_ServerListControl.GetFirstSelectedItemPosition()!=NULL);
	POSITION Position=m_ServerListControl.GetFirstSelectedItemPosition();
	
	//��ȡ����
	INT nListItem=m_ServerListControl.GetNextSelectedItem(Position);
	tagGameServerInfo * pGameServerInfo=(tagGameServerInfo *)m_ServerListControl.GetItemData(nListItem);

	//���ñ���
	if (pGameServerInfo!=NULL)
	{
		//������ʾ
		TCHAR szString[128]=TEXT("");
		_sntprintf(szString,CountArray(szString),TEXT("ȷʵҪɾ�� [ %s ] ��Ϸ������"),pGameServerInfo->szServerName);

		//��ʾ��Ϣ
		if (AfxMessageBox(szString,MB_ICONQUESTION|MB_YESNO)!=IDYES) return;

		//ɾ������
		CServerInfoManager ServerInfoManager;
		if (ServerInfoManager.DeleteGameServer(pGameServerInfo->wServerID)==false) return;

		//ɾ���б�
		m_ServerListControl.DeleteItem(nListItem);
		m_ServerInfoBuffer.DeleteServerInfo(pGameServerInfo->wServerID);

		//�����б�
		if (m_ServerListControl.GetItemCount()>0)
		{
			//���ñ���
			INT nItemCount=m_ServerListControl.GetItemCount();
			INT nNextItem=(nListItem>=nItemCount)?(nItemCount-1):nListItem;

			//����ѡ��
			m_ServerListControl.SetItemState(nNextItem,LVIS_SELECTED,LVIS_SELECTED);
		}

		//���¿���
		UpdateControlStatus();
	}

	return;
}

//�������
VOID CDlgServerItem::OnBnClickedServiceAttrib()
{
	return;
}

//˫���б�
VOID CDlgServerItem::OnNMDblclkServerList(NMHDR * pNMHDR, LRESULT * pResult)
{
	//��������
	if (((NMITEMACTIVATE *)pNMHDR)->iItem!=LB_ERR)
	{
		OnBnClickedLoadServer();
	}

	return;
}

//ѡ��ı�
VOID CDlgServerItem::OnLvnItemChangedServerList(NMHDR * pNMHDR, LRESULT * pResult)
{
	//���¿���
	UpdateControlStatus();

	return;
}

//////////////////////////////////////////////////////////////////////////////////
