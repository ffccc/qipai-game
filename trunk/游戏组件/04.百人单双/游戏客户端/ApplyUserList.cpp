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

	//������ɫ
	m_AppyUserList.SetTextColor(RGB(52,87,130));
	m_AppyUserList.SetBkColor(RGB(255,255,255));
	m_AppyUserList.SetTextBkColor(RGB(255,255,255));

	//�����б�
	m_AppyUserList.InsertColumn(0,TEXT("�������"),LVCFMT_LEFT,101);
	m_AppyUserList.InsertColumn(1,TEXT("��Ϸ��"),LVCFMT_LEFT,100);

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
//�������
bool CApplyUser::FindUser(LPCTSTR lpszUserName)
{
	//�������
	LVFINDINFO lvFindInfo;
	ZeroMemory( &lvFindInfo, sizeof( lvFindInfo ) );
	lvFindInfo.flags=LVFI_STRING;
	lvFindInfo.psz=lpszUserName;

	//��������
	int nItem=m_AppyUserList.FindItem(&lvFindInfo);

	//���ؽ��
	return -1==nItem ? false : true;
}


//����б�
void CApplyUser::ClearAll()
{
	m_AppyUserList.DeleteAllItems();
}
//////////////////////////////////////////////////////////////////////////
