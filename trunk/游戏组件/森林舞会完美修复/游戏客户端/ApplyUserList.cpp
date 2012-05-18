#include "StdAfx.h"
#include "Resource.h"
#include "ApplyUserList.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CApplyUser, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CApplyUser::CApplyUser() : CDialog(IDD_DLG_APPLY_BANKER)
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
   // m_AppyUserList.SetWindowPos(HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	//������ɫ
	m_AppyUserList.SetTextColor(RGB(255,255,0));
	m_AppyUserList.SetBkColor(RGB(9,45,109));
	m_AppyUserList.SetTextBkColor(RGB(9,45,109));
	//m_AppyUserList.SetWindowPos(&wndTopMost,0,0,150,150,SWP_NOZORDER);

	//m_AppyUserList.SetLayeredWindowAttributes();
	//�����б�
	m_AppyUserList.InsertColumn(0,TEXT("�������"),LVCFMT_LEFT,85);
	m_AppyUserList.InsertColumn(1,TEXT("��Ϸ��"),LVCFMT_LEFT,140);

	return FALSE;
}

//λ�ñ仯
void CApplyUser::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	//�б�ؼ�
	if (m_AppyUserList.m_hWnd != NULL)
	{
		m_AppyUserList.MoveWindow(0,0,cx,cy);
		m_AppyUserList.SetColumnWidth(1,cx-m_AppyUserList.GetColumnWidth(0));
	}

	return;
}

//ɾ���б�
void CApplyUser::DeleteUser(tagApplyUser & ApplyUser)
{
	//��������
	int nItem = FindUser(ApplyUser.dwUserID);

	//ɾ������
	if ( nItem != -1 )
	{
		m_AppyUserList.DeleteItem( nItem );
	}
}

//�����б�
void CApplyUser::InserUser(tagApplyUser & ApplyUser)
{
	//��������
	int nItem = FindUser(ApplyUser.dwUserID);

	//ɾ������
	if ( nItem != -1 ) 
	{
		TCHAR szBuffer[128]=TEXT("");

		//���ϲ��
		_snprintf(szBuffer,CountArray(szBuffer),TEXT("%I64d"),ApplyUser.lUserScore);
		m_AppyUserList.SetItem(nItem,1,LVIF_TEXT,szBuffer,0,0,0,0);
	}
	else
	{
		//��������
		WORD wListIndex=0;
		WORD wColumnCount=1;
		TCHAR szBuffer[128]=TEXT("");

		//��Ϸ���
		INT nItemIndex=m_AppyUserList.InsertItem(m_AppyUserList.GetItemCount(), (LPCTSTR)ApplyUser.stAname);
		//��ҽ��

		m_AppyUserList.SetItem(nItemIndex,wColumnCount++,LVIF_TEXT,DrawNUM(ApplyUser.lUserScore),0,0,0,0);
		m_AppyUserList.SetItemData(nItemIndex, ApplyUser.dwUserID);
	}


	return;
}
char* CApplyUser::DrawNUM(INT64 num)
{
	TCHAR szBuffer[256];
	_sntprintf(szBuffer, CountArray(szBuffer), TEXT("%I64d"),num);
	int vf=1;
	if (num<0)
	{
		vf=2;
	}
	int cu = 0;
	s1="";
	s1=szBuffer;  
	for (int i=strlen(szBuffer)-1;i>=vf;i--)
	{
		cu++;
		//if (cu%4 == 0)				
		//	s1.insert(i,",");		
	}
	return (char*)s1.c_str();

}

//�����б�
void CApplyUser::UpdateUser( tagApplyUser & ApplyUser )
{

	//��������
	int nItem = FindUser(ApplyUser.dwUserID);

	//ɾ������
	if ( nItem != -1 ) 
	{
		//��ҽ��
		m_AppyUserList.SetItem(nItem,1,LVIF_TEXT,DrawNUM(ApplyUser.lUserScore),0,0,0,0);
	}


}

//����б�
void CApplyUser::ClearAll()
{
	m_AppyUserList.DeleteAllItems();
}

//ɾ���б�
int CApplyUser::FindUser(DWORD dwUserID)
{
	TCHAR szFindTxt[100];
	sprintf(szFindTxt, "%d", dwUserID);

	//�������
	LVFINDINFO lvFindInfo;
	ZeroMemory( &lvFindInfo, sizeof( lvFindInfo ) );
	lvFindInfo.flags = LVFI_PARAM;
	lvFindInfo.lParam = dwUserID;

	//��������
	return m_AppyUserList.FindItem( &lvFindInfo );
}

//////////////////////////////////////////////////////////////////////////
