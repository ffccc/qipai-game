#include "StdAfx.h"
#include "Resource.h"
#include "ServiceParameter.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CServiceParameter, CDialog)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CServiceParameter::CServiceParameter() : CDialog(IDD_PARAMETER)
{
	m_bLoaded=false;
	m_hDllInstance=NULL;
	m_pIGameServiceManager=NULL;
	memset(&m_OptionParameter,0,sizeof(m_OptionParameter));
	memset(&m_GameServiceAttrib,0,sizeof(m_GameServiceAttrib));
}

//��������
CServiceParameter::~CServiceParameter()
{
	FreeGameServiceModule();
}

//�ؼ����໯
void CServiceParameter::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
}

//��ʼ������
BOOL CServiceParameter::OnInitDialog()
{
	__super::OnInitDialog();

	//Ч�����
	ASSERT(m_hDllInstance!=NULL);
	ASSERT(m_pIGameServiceManager!=NULL);

	//���ÿؼ�
	((CEdit *)GetDlgItem(IDC_PORT))->LimitText(5);
	((CEdit *)GetDlgItem(IDC_SERVER_ID))->LimitText(5);
	((CEdit *)GetDlgItem(IDC_STATION_ID))->LimitText(5);
	((CEdit *)GetDlgItem(IDC_TABLE_COUNT))->LimitText(3);
	((CEdit *)GetDlgItem(IDC_MAX_CONNECT))->LimitText(3);
	((CEdit *)GetDlgItem(IDC_DATABASE_NAME))->LimitText(31);
	((CEdit *)GetDlgItem(IDC_SERVER_NAME))->LimitText(SERVER_LEN-1);

	//�����ַ
	TCHAR szHostName[64],szServerAddr[16];
	gethostname(szHostName,sizeof(szHostName));
	HOSTENT * pHostent=gethostbyname(szHostName);
    LPIN_ADDR * lpAddrList=(LPIN_ADDR *)pHostent->h_addr_list;
	CComboBox * pServerAddr=(CComboBox *)GetDlgItem(IDC_SERVER_ADDR);
    while (*lpAddrList!=NULL)
	{
		lstrcpyn(szServerAddr,inet_ntoa(**lpAddrList),CountArray(szServerAddr));
		int nIndex=pServerAddr->AddString(szServerAddr);
		pServerAddr->SetItemData(nIndex,(*lpAddrList)->S_un.S_addr);
		lpAddrList++;
	}

	//������Ϣ
	UpdateOptionParameter();

	return TRUE;
}

//ȷ������
void CServiceParameter::OnOK()
{
	//��������
	tagOptionParameter OptionParameter;
	memset(&OptionParameter,0,sizeof(OptionParameter));
	lstrcpyn(OptionParameter.szModuleName,m_GameServiceAttrib.szServerModuleName,CountArray(OptionParameter.szModuleName));

	//�����ʶ
	OptionParameter.GameServiceOption.wServerID=GetDlgItemInt(IDC_SERVER_ID,NULL,FALSE);
	if (OptionParameter.GameServiceOption.wServerID==0)
	{
		AfxMessageBox(TEXT("��������ȷ�ķ����ʶ����"),MB_ICONINFORMATION);
		GetDlgItem(IDC_SERVER_ID)->SetFocus();
		return;
	}

	//���ͱ�ʶ
	OptionParameter.GameServiceOption.wKindID=GetDlgItemInt(IDC_OPTION_KIND_ID,FALSE);
	if (OptionParameter.GameServiceOption.wKindID==0) OptionParameter.GameServiceOption.wKindID=m_GameServiceAttrib.wKindID;

	//�����ʶ
	OptionParameter.GameServiceOption.wSortID=GetDlgItemInt(IDC_OPTION_SORT_ID,FALSE);
	if (OptionParameter.GameServiceOption.wSortID==0) OptionParameter.GameServiceOption.wSortID=200;

	//վ���ʶ
	OptionParameter.GameServiceOption.wNodeID=GetDlgItemInt(IDC_STATION_ID,NULL,FALSE);

	//������Ŀ
	OptionParameter.GameServiceOption.wTableCount=GetDlgItemInt(IDC_TABLE_COUNT,NULL,FALSE);
	if (OptionParameter.GameServiceOption.wTableCount==0)
	{
		AfxMessageBox(TEXT("��������ȷ��������Ŀ"),MB_ICONINFORMATION);
		GetDlgItem(IDC_TABLE_COUNT)->SetFocus();
		return;
	}

	//��Ԫ����
	OptionParameter.GameServiceOption.lCellScore=GetDlgItemInt(IDC_CELL_SCORE,NULL,FALSE);

	//��ͻ���
	OptionParameter.GameServiceOption.lLessScore=GetDlgItemInt(IDC_LESS_SCORE,NULL,FALSE);

	//��߻���
	OptionParameter.GameServiceOption.lMaxScore=GetDlgItemInt(IDC_MAX_SCORE,NULL,FALSE);

	//���ƻ���
	OptionParameter.GameServiceOption.lRestrictScore=GetDlgItemInt(IDC_RESTRICT_SCORE,NULL,FALSE);

	//��Ϸ˰��
	OptionParameter.GameServiceOption.wRevenue=GetDlgItemInt(IDC_REVENUE,NULL,FALSE);
	OptionParameter.GameServiceOption.wRevenue=__min(OptionParameter.GameServiceOption.wRevenue,1000);

	//������Ϣ
	OptionParameter.GameServiceOption.cbHideUserInfo=(((CButton *)GetDlgItem(IDC_HIDE_USER_INFO))->GetCheck()==BST_CHECKED)?TRUE:FALSE;

	//�����Թ�
	OptionParameter.GameServiceOption.cbUnLookOnTag=(((CButton *)GetDlgItem(IDC_LIMIT_LOOK_ON))->GetCheck()==BST_CHECKED)?TRUE:FALSE;

	//����ͬIP
	OptionParameter.GameServiceOption.cbUnSameIPPlay=(((CButton *)GetDlgItem(IDC_LIMIT_SAME_IP))->GetCheck()==BST_CHECKED)?TRUE:FALSE;

	//���ƿ�ʼ
	OptionParameter.GameServiceOption.cbControlStart=(((CButton *)GetDlgItem(IDC_CONTROL_START))->GetCheck()==BST_CHECKED)?TRUE:FALSE;

	//��������
	OptionParameter.GameServiceOption.lMatchDraw=GetDlgItemInt(IDC_MATCH_DRAW,NULL,FALSE);;

	//�������
	OptionParameter.GameServiceOption.wMaxConnect=GetDlgItemInt(IDC_MAX_CONNECT,NULL,FALSE);
	WORD wMaxConnect=m_GameServiceAttrib.wChairCount*OptionParameter.GameServiceOption.wTableCount;
	OptionParameter.GameServiceOption.wMaxConnect=__max(wMaxConnect,OptionParameter.GameServiceOption.wMaxConnect);

	//��Ϸ����
	CComboBox * pGameGenre=(CComboBox *)GetDlgItem(IDC_GAME_GENRE);
	int nCurSel=pGameGenre->GetCurSel();
	if (nCurSel==CB_ERR)
	{
		AfxMessageBox(TEXT("��ѡ����ȷ����Ϸ����"),MB_ICONINFORMATION);
		GetDlgItem(IDC_GAME_GENRE)->SetFocus();
		return;
	}
	OptionParameter.GameServiceOption.wServerType=(WORD)pGameGenre->GetItemData(nCurSel);

	//��������
	GetDlgItemText(IDC_SERVER_NAME,OptionParameter.GameServiceOption.szGameRoomName,CountArray(OptionParameter.GameServiceOption.szGameRoomName));
	if (OptionParameter.GameServiceOption.szGameRoomName[0]==0)
	{
		AfxMessageBox(TEXT("����д��ȷ����Ϸ��������"),MB_ICONINFORMATION);
		GetDlgItem(IDC_SERVER_NAME)->SetFocus();
		return;
	}

	//�����˿�
	OptionParameter.GameServiceOption.wServerPort=GetDlgItemInt(IDC_PORT,NULL,FALSE);
	if (OptionParameter.GameServiceOption.wServerPort==0)
	{
		AfxMessageBox(TEXT("����д��ȷ�ķ�������˿�"),MB_ICONINFORMATION);
		GetDlgItem(IDC_PORT)->SetFocus();
		return;
	}

	//��Ƶ��ַ
	((CIPAddressCtrl *)GetDlgItem(IDC_VIDEO_ADDR))->GetAddress(OptionParameter.GameServiceOption.dwVideoServer);

	//�����ַ
	CComboBox * pServerAddr=(CComboBox *)GetDlgItem(IDC_SERVER_ADDR);
	nCurSel=pServerAddr->GetCurSel();
	if (nCurSel==CB_ERR)
	{
		TCHAR szServerAddr[256]=TEXT("");
		pServerAddr->GetWindowText(szServerAddr,CountArray(szServerAddr));
		if (szServerAddr[0]==0)
		{
			AfxMessageBox(TEXT("��ѡ����ȷ�ķ����ַ"),MB_ICONINFORMATION);
			GetDlgItem(IDC_SERVER_ADDR)->SetFocus();
			return;
		}
		OptionParameter.GameServiceOption.dwServerAddr=inet_addr(szServerAddr);
	}
	else OptionParameter.GameServiceOption.dwServerAddr=(DWORD)pServerAddr->GetItemData(nCurSel);

	//���ݿ�����
	GetDlgItemText(IDC_DATABASE_NAME,OptionParameter.szDataBaseName,CountArray(OptionParameter.szDataBaseName));
	if (OptionParameter.szDataBaseName[0]==0)
	{
		AfxMessageBox(TEXT("����д��ȷ����Ϸ���ݿ�����"),MB_ICONINFORMATION);
		GetDlgItem(IDC_DATABASE_NAME)->SetFocus();
		return;
	}

	//���ݿ��ַ
	DWORD dwDataBaseAddr=0L;
	((CIPAddressCtrl *)GetDlgItem(IDC_DATABASE_ADDR))->GetAddress(dwDataBaseAddr);
	if (dwDataBaseAddr==0L)
	{
		AfxMessageBox(TEXT("����д��ȷ����Ϸ���ݿ��ַ"),MB_ICONINFORMATION);
		GetDlgItem(IDC_DATABASE_ADDR)->SetFocus();
		return;
	}

	//�����ַ
	BYTE * pcbDataBaseAddr=(BYTE *)&dwDataBaseAddr;
	_snprintf(OptionParameter.szDataBaseAddr,sizeof(OptionParameter.szDataBaseAddr),TEXT("%d.%d.%d.%d"),
		pcbDataBaseAddr[3],pcbDataBaseAddr[2],pcbDataBaseAddr[1],pcbDataBaseAddr[0]);

	//�������
	m_bLoaded=true;
	m_OptionParameter=OptionParameter;

	__super::OnOK();
}

//��������
bool CServiceParameter::LoadParameter(LPCTSTR pszFileName)
{
	//Ч�����
	ASSERT(pszFileName!=NULL);

	//���ļ�
	CFile ParameterFile;
	if (ParameterFile.Open(pszFileName,CFile::modeRead)==FALSE) return false;

	//��ȡ��С
	LONGLONG lFileLength=ParameterFile.GetLength();
	ASSERT(lFileLength==sizeof(tagRecordParameter));
	if (lFileLength!=sizeof(tagRecordParameter)) return false;

	//��ȡ��Ϣ
	tagRecordParameter RecordParameter;
	UINT uReadCount=ParameterFile.Read(&RecordParameter,sizeof(RecordParameter));
	ASSERT(uReadCount==sizeof(tagRecordParameter));
	if (uReadCount!=sizeof(tagRecordParameter)) return false;

	//Ч����Ϣ
	ASSERT(RecordParameter.wVersion==VER_PARAMETER);
	ASSERT(RecordParameter.wFileSize==sizeof(tagRecordParameter));
	if (RecordParameter.wVersion!=VER_PARAMETER) return false;
	if (RecordParameter.wFileSize!=sizeof(tagRecordParameter)) return false;

	//������Ϣ
	m_bLoaded=true;
	m_OptionParameter=RecordParameter.OptionParameter;

	//�ر��ļ�
	ParameterFile.Close();

	return true;
}

//��������
bool CServiceParameter::SaveParameter(LPCTSTR pszFileName)
{
	//Ч�����
	ASSERT(m_bLoaded==true);
	ASSERT(pszFileName!=NULL);

	//���ļ�
	CFile ParameterFile;
	if (ParameterFile.Open(pszFileName,CFile::modeWrite|CFile::modeCreate)==FALSE) return false;

	//������Ϣ
	tagRecordParameter RecordParameter;
	RecordParameter.wVersion=VER_PARAMETER;
	RecordParameter.wFileSize=sizeof(RecordParameter);
	RecordParameter.OptionParameter=m_OptionParameter;

	//д���ļ�
	ParameterFile.Write(&RecordParameter,sizeof(RecordParameter));
	ParameterFile.SetLength(RecordParameter.wFileSize);

	//�ر��ļ�
	ParameterFile.Close();

	return true;
}

//���ò���
void CServiceParameter::ResetParameter()
{
	//�ͷ����
	FreeGameServiceModule();

	//���ñ���
	m_bLoaded=false;
	memset(&m_OptionParameter,0,sizeof(m_OptionParameter));

	//��Ϸ˰��
	m_OptionParameter.GameServiceOption.wRevenue=5;

	return;
}

//�޸Ĳ���
bool CServiceParameter::ConfigParameter()
{
	//Ч��״̬
	ASSERT(m_bLoaded==true);
	ASSERT(m_hDllInstance==NULL);

	//������Ϣ
	if (LoadGameServiceModule(m_OptionParameter.szModuleName)==false)
	{
		AfxMessageBox(TEXT("��������������ڡ����޷�ʶ�𡱣��޷�������Ϸ���䣡"),MB_ICONINFORMATION);
		return false;
	}

	//���ò���
	INT_PTR nResult=DoModal();

	//�ͷ�ģ��
	FreeGameServiceModule();

	return (nResult==IDOK);
}

//��������
bool CServiceParameter::CreateParameter(LPCTSTR pszModuleName)
{
	//���ò���
	ResetParameter();

	//����ģ��
	if (LoadGameServiceModule(pszModuleName)==false)
	{
		AfxMessageBox(TEXT("��������������ڡ����޷�ʶ�𡱣��޷�������Ϸ���䣡"),MB_ICONINFORMATION);
		return false;
	}

	//���ò���
	INT_PTR nResult=DoModal();

	//�ͷ�ģ��
	FreeGameServiceModule();

	return (nResult==IDOK);
}

//����ģ��
bool CServiceParameter::LoadGameServiceModule(LPCTSTR pszModuleName)
{
	//Ч��״̬
	ASSERT(m_hDllInstance==NULL);
	ASSERT(m_pIGameServiceManager==NULL);

	//����ģ��
	m_hDllInstance=AfxLoadLibrary(pszModuleName);
	if (m_hDllInstance==NULL)
	{
		FreeGameServiceModule();
		return false;
	}

	//���Һ���
	ModuleCreateProc * CreateFunc=(ModuleCreateProc *)GetProcAddress(m_hDllInstance,TEXT("CreateGameServiceManager"));
	if (CreateFunc==NULL)
	{
		FreeGameServiceModule();
		return false;
	}

	//��ȡ�ӿ�
	m_pIGameServiceManager=(IGameServiceManager *)CreateFunc(IID_IGameServiceManager,VER_IGameServiceManager);
	if (m_pIGameServiceManager==NULL)
	{
		FreeGameServiceModule();
		return false;
	}

	//��ȡ����
	m_pIGameServiceManager->GetGameServiceAttrib(m_GameServiceAttrib);
	lstrcpyn(m_OptionParameter.szModuleName,m_GameServiceAttrib.szServerModuleName,CountArray(m_OptionParameter.szModuleName));

	return true;
}

//�ͷ�ģ��
void CServiceParameter::FreeGameServiceModule()
{
	//�ͷ�ģ��
	if (m_pIGameServiceManager!=NULL)
	{
		m_pIGameServiceManager->Release();
		m_pIGameServiceManager=NULL;
	}
	if (m_hDllInstance!=NULL)
	{
		AfxFreeLibrary(m_hDllInstance);
		m_hDllInstance=NULL;
	}

	//���ñ���
	memset(&m_GameServiceAttrib,0,sizeof(m_GameServiceAttrib));

	return;
}

//��������
void CServiceParameter::UpdateOptionParameter()
{
	//��������
	CComboBox * pComboBox=(CComboBox *)GetDlgItem(IDC_GAME_GENRE);
	WORD wGameGenre[4]={GAME_GENRE_EDUCATE,GAME_GENRE_MATCH,GAME_GENRE_GOLD,GAME_GENRE_SCORE};
	LPCTSTR pszGameGenre[4]={TEXT("��ϰ����"),TEXT("��������"),TEXT("�������"),TEXT("��ֵ����")};

	//��Ϸ����
	pComboBox->Clear();
	for (int i=0;i<CountArray(wGameGenre);i++)
	{
		int nIndex=pComboBox->AddString(pszGameGenre[i]);
		pComboBox->SetItemData(nIndex,wGameGenre[i]);
		if (m_OptionParameter.GameServiceOption.wServerType==wGameGenre[i]) pComboBox->SetCurSel(nIndex);
	}

	//���ͱ�ʶ
	if (m_OptionParameter.GameServiceOption.wKindID==0) SetDlgItemText(IDC_OPTION_KIND_ID,TEXT(""));
	else SetDlgItemInt(IDC_OPTION_KIND_ID,m_OptionParameter.GameServiceOption.wKindID,FALSE);

	//�����ʶ
	if (m_OptionParameter.GameServiceOption.wSortID==0) SetDlgItemText(IDC_OPTION_SORT_ID,TEXT(""));
	else SetDlgItemInt(IDC_OPTION_SORT_ID,m_OptionParameter.GameServiceOption.wSortID,FALSE);

	//�����ʶ
	if (m_OptionParameter.GameServiceOption.wServerID==0) SetDlgItemText(IDC_SERVER_ID,TEXT(""));
	else SetDlgItemInt(IDC_SERVER_ID,m_OptionParameter.GameServiceOption.wServerID,FALSE);

	//վ���ʶ
	if (m_OptionParameter.GameServiceOption.wNodeID==0) SetDlgItemText(IDC_STATION_ID,TEXT(""));
	else SetDlgItemInt(IDC_STATION_ID,m_OptionParameter.GameServiceOption.wNodeID,FALSE);

	//������Ŀ
	if (m_OptionParameter.GameServiceOption.wTableCount==0) SetDlgItemText(IDC_TABLE_COUNT,TEXT(""));
	else SetDlgItemInt(IDC_TABLE_COUNT,m_OptionParameter.GameServiceOption.wTableCount,FALSE);

	//��Ԫ����
	if (m_OptionParameter.GameServiceOption.lCellScore==0) SetDlgItemText(IDC_CELL_SCORE,TEXT(""));
	else SetDlgItemInt(IDC_CELL_SCORE,m_OptionParameter.GameServiceOption.lCellScore,FALSE);

	//���ٻ���
	if (m_OptionParameter.GameServiceOption.lLessScore==0) SetDlgItemText(IDC_LESS_SCORE,TEXT(""));
	else SetDlgItemInt(IDC_LESS_SCORE,m_OptionParameter.GameServiceOption.lLessScore,FALSE);

	//������
	if (m_OptionParameter.GameServiceOption.lMaxScore==0) SetDlgItemText(IDC_MAX_SCORE,TEXT(""));
	else SetDlgItemInt(IDC_MAX_SCORE,m_OptionParameter.GameServiceOption.lMaxScore,FALSE);

	//��Ӯ����
	if (m_OptionParameter.GameServiceOption.lRestrictScore==0) SetDlgItemText(IDC_RESTRICT_SCORE,TEXT(""));
	else SetDlgItemInt(IDC_RESTRICT_SCORE,m_OptionParameter.GameServiceOption.lRestrictScore,FALSE);

	//��Ϸ˰��
	SetDlgItemInt(IDC_REVENUE,m_OptionParameter.GameServiceOption.wRevenue);

	//������Ϣ
	BYTE cbHideUserInfo=m_OptionParameter.GameServiceOption.cbHideUserInfo;
	((CButton *)GetDlgItem(IDC_HIDE_USER_INFO))->SetCheck((cbHideUserInfo==TRUE)?BST_CHECKED:BST_UNCHECKED);

	//�����Թ�
	BYTE cbUnLookOnTag=m_OptionParameter.GameServiceOption.cbUnLookOnTag;
	((CButton *)GetDlgItem(IDC_LIMIT_LOOK_ON))->SetCheck((cbUnLookOnTag==TRUE)?BST_CHECKED:BST_UNCHECKED);

	//����ͬIP
	BYTE cbUnSameIPPlay=m_OptionParameter.GameServiceOption.cbUnSameIPPlay;
	((CButton *)GetDlgItem(IDC_LIMIT_SAME_IP))->SetCheck((cbUnSameIPPlay==TRUE)?BST_CHECKED:BST_UNCHECKED);

	//���ƿ�ʼ
	BYTE cbControlStart=m_OptionParameter.GameServiceOption.cbControlStart;
	((CButton *)GetDlgItem(IDC_CONTROL_START))->SetCheck((cbControlStart==TRUE)?BST_CHECKED:BST_UNCHECKED);

	//��������
	LONG lMatchDraw=m_OptionParameter.GameServiceOption.lMatchDraw;
	if (lMatchDraw>0) SetDlgItemInt(IDC_MATCH_DRAW,lMatchDraw,FALSE);

	//�������
	if (m_OptionParameter.GameServiceOption.wMaxConnect==0) SetDlgItemText(IDC_MAX_CONNECT,TEXT(""));
	else SetDlgItemInt(IDC_MAX_CONNECT,m_OptionParameter.GameServiceOption.wMaxConnect,FALSE);

	//��������
	if (m_OptionParameter.GameServiceOption.szGameRoomName[0]==0) 
	{
		TCHAR szServerName[SERVER_LEN]=TEXT("");
		_snprintf(szServerName,sizeof(szServerName),TEXT("%s����"),m_GameServiceAttrib.szKindName);
		SetDlgItemText(IDC_SERVER_NAME,szServerName);
	}
	else SetDlgItemText(IDC_SERVER_NAME,m_OptionParameter.GameServiceOption.szGameRoomName);

	//��Ƶ��ַ
	((CIPAddressCtrl *)GetDlgItem(IDC_VIDEO_ADDR))->SetAddress(m_OptionParameter.GameServiceOption.dwVideoServer);

	//�����˿�
	if (m_OptionParameter.GameServiceOption.wServerPort==0) SetDlgItemText(IDC_PORT,TEXT(""));
	else SetDlgItemInt(IDC_PORT,m_OptionParameter.GameServiceOption.wServerPort,FALSE);

	//�����ַ
	CComboBox * pServerAddr=(CComboBox *)GetDlgItem(IDC_SERVER_ADDR);
	int nItemCount=pServerAddr->GetCount();
	int i=0;
	for (;i<nItemCount;i++)
	{
		DWORD_PTR dwItemAddr=pServerAddr->GetItemData(i);
		if (m_OptionParameter.GameServiceOption.dwServerAddr==dwItemAddr) 
		{
			pServerAddr->SetCurSel(i);
			break;
		}
	}
	if (nItemCount==i)
	{
		TCHAR szServerAddr[256]=TEXT("");
		BYTE * cbServerAddr=(BYTE *)&m_OptionParameter.GameServiceOption.dwServerAddr;
		_snprintf(szServerAddr,CountArray(szServerAddr),TEXT("%ld.%ld.%ld.%ld"),cbServerAddr[0],cbServerAddr[1],cbServerAddr[2],cbServerAddr[3]);
		pServerAddr->SetWindowText(szServerAddr);
	}

	//���ݿ�����
	if (m_OptionParameter.szDataBaseName[0]!=0) SetDlgItemText(IDC_DATABASE_NAME,m_OptionParameter.szDataBaseName);
	else SetDlgItemText(IDC_DATABASE_NAME,m_GameServiceAttrib.szDataBaseName);

	//���ݿ��ַ
	DWORD dwDataBaseAddr=ntohl(inet_addr(m_OptionParameter.szDataBaseAddr));
	((CIPAddressCtrl *)GetDlgItem(IDC_DATABASE_ADDR))->SetAddress(dwDataBaseAddr);

	//�������
	SetDlgItemInt(IDC_KIND_ID,m_GameServiceAttrib.wKindID,FALSE);
	SetDlgItemText(IDC_KIND_NAME,m_GameServiceAttrib.szKindName);
	SetDlgItemInt(IDC_CHAIR_COUNT,m_GameServiceAttrib.wChairCount,FALSE);
	SetDlgItemText(IDC_DEF_DATABASE_NAME,m_GameServiceAttrib.szDataBaseName);
	SetDlgItemText(IDC_CLIENT_MODULE,m_GameServiceAttrib.szClientModuleName);
	SetDlgItemText(IDC_SERVICE_MODULE,m_GameServiceAttrib.szServerModuleName);
	SetDlgItemText(IDC_MODULE_DESCRIBE,m_GameServiceAttrib.szDescription);

	return;
}

//////////////////////////////////////////////////////////////////////////
