#include "StdAfx.h"
#include "Resource.h"
#include "ApplyUserList.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CApplyUser, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CApplyUser::CApplyUser() : CDialog(IDD_DLG_GAME_RECORD)
{
}

//��������
CApplyUser::~CApplyUser()
{
}

//�ؼ���
void CApplyUser::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RECORD_LIST, m_AppyUserList);
}

//��ʼ������
BOOL CApplyUser::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	TCHAR szBuffer[_MAX_PATH];     
	::GetModuleFileName(AfxGetInstanceHandle(), szBuffer, _MAX_PATH);
	CString   sPath;   
	sPath.Format(_T("res://%s/#2/#193"),szBuffer);

	LVBKIMAGE   bki;   
	bki.ulFlags   =   LVBKIF_STYLE_TILE   |   LVBKIF_SOURCE_URL   ;   
	bki.pszImage   =   sPath.GetBuffer(sPath.GetLength());   
	bki.cchImageMax   =   sPath.GetLength();   
	BOOL bSuccess =m_AppyUserList.SetBkImage(&bki);  

	CWnd* pParent = GetParent();
	m_AppyUserList.Init(0,pParent);

	m_AppyUserList.SetTextBkColor(-1);
	m_AppyUserList.SetTextColor(RGB(232, 245, 3));


	//�����б�
	m_AppyUserList.InsertColumn(0,TEXT("�������"),LVCFMT_LEFT,145);
	m_AppyUserList.InsertColumn(1,TEXT("��Ϸ��"),LVCFMT_LEFT,100);

	//test
	//tagApplyUser ApplyUser;
	//ApplyUser.strUserName = "kdjfdksdsdsdk";
	//ApplyUser.lUserScore = 1000000;

	//InserUser(ApplyUser);
	//ApplyUser.strUserName = "kdjfd";
	//InserUser(ApplyUser);
	//ApplyUser.strUserName = "kdjfdsdsdk";
	//InserUser(ApplyUser);
	//ApplyUser.strUserName = "ksdsdsdk";
	//InserUser(ApplyUser);
	//ApplyUser.strUserName = "kksdsdsdk";
	//InserUser(ApplyUser);
	//ApplyUser.strUserName = "jfdksdsdsdk";
	//InserUser(ApplyUser);
	//ApplyUser.strUserName = "fdksdsdsdk";
	//InserUser(ApplyUser);
	//ApplyUser.strUserName = "jfdksdsds";
	//InserUser(ApplyUser);
	//ApplyUser.strUserName = "kk";
	//InserUser(ApplyUser);
	//ApplyUser.strUserName = "jfdksdsdk";
	//InserUser(ApplyUser);
	//ApplyUser.strUserName = "dsdk";
	//InserUser(ApplyUser);
	//ApplyUser.strUserName = "kdjf";
	//InserUser(ApplyUser);
	//ApplyUser.strUserName = "kdjfdksdsdk";
	//InserUser(ApplyUser);
	//ApplyUser.strUserName = "dsdsdk";
	//InserUser(ApplyUser);
	//ApplyUser.strUserName = "dk";
	//InserUser(ApplyUser);
	//ApplyUser.strUserName = "ksdsdsdk";
	//InserUser(ApplyUser);
	

	return FALSE;
}

//�����б�
void CApplyUser::InserUser(tagApplyUser & ApplyUser)
{
	//��������
	WORD wListIndex=0;
	WORD wColumnCount=1;
	TCHAR szBuffer[128]=TEXT("");

	//��Ϸ���
	INT nItemIndex=m_AppyUserList.InsertItem(m_AppyUserList.GetItemCount(),ApplyUser.strUserName);

	//��ҽ��
	_snprintf(szBuffer,CountArray(szBuffer),TEXT("%I64d"),ApplyUser.lUserScore);
	m_AppyUserList.SetItem(nItemIndex,wColumnCount++,LVIF_TEXT,szBuffer,0,0,0,0);

	return;
}

//λ�ñ仯
void CApplyUser::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	//�б�ؼ�
	if (m_AppyUserList.m_hWnd!=NULL)
	{
		m_AppyUserList.MoveWindow(0,0,cx,cy);
	}

	return;
}

//ɾ���б�
void CApplyUser::DeleteUser(tagApplyUser & ApplyUser)
{
	//�������
	LVFINDINFO lvFindInfo;
	ZeroMemory( &lvFindInfo, sizeof( lvFindInfo ) );
	lvFindInfo.flags = LVFI_STRING;
	lvFindInfo.psz = (LPCTSTR)ApplyUser.strUserName;

	//��������
	int nItem = m_AppyUserList.FindItem( &lvFindInfo );

	//ɾ������
	if ( nItem != -1 ) m_AppyUserList.DeleteItem( nItem );
}

//�����б�
void CApplyUser::UpdateUser( tagApplyUser & ApplyUser )
{
		//�������
	LVFINDINFO lvFindInfo;
	ZeroMemory( &lvFindInfo, sizeof( lvFindInfo ) );
	lvFindInfo.flags = LVFI_STRING;
	lvFindInfo.psz = (LPCTSTR)ApplyUser.strUserName;

	//��������
	int nItem = m_AppyUserList.FindItem( &lvFindInfo );

	//ɾ������
	if ( nItem != -1 ) 
	{
		TCHAR szBuffer[128]=TEXT("");

		//��ҽ��
		_snprintf(szBuffer,CountArray(szBuffer),TEXT("%I64d"),ApplyUser.lUserScore);
		m_AppyUserList.SetItem(nItem,1,LVIF_TEXT,szBuffer,0,0,0,0);
	}
}

//����б�
void CApplyUser::ClearAll()
{
	m_AppyUserList.DeleteAllItems();
}
//////////////////////////////////////////////////////////////////////////
