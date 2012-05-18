#include "StdAfx.h"
#include "Resource.h"
#include "ServerViewItem.h"
#include "DlgSearchTable.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgSearchTable, CSkinDialog)
	ON_BN_CLICKED(IDC_AUTO_JOIN, OnBnClickedAutoJoin)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CDlgSearchTable::CDlgSearchTable() : CSkinDialog(IDD_DLG_SEARCH_TABLE)
{
	//�ӿڱ���
	m_pTableViewFrame=NULL;
	m_pServerViewItem=NULL;

	//���ñ���
	m_wResultTableID=INVALID_TABLE;

	return;
}

//��������
CDlgSearchTable::~CDlgSearchTable()
{
}

//�ؼ���
VOID CDlgSearchTable::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btSearch);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_AUTO_JOIN, m_btAutoJoin);
}

//���ú���
BOOL CDlgSearchTable::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	m_wResultTableID=INVALID_TABLE;

	//���ñ���
	SetWindowText(TEXT("������Ϸ��"));

	//��������
	CheckDlgButton(IDC_FILTRATE_PASS,BST_CHECKED);
	CheckDlgButton(IDC_SEARCH_ONE_NULL,BST_CHECKED);
	CheckDlgButton(IDC_SEARCH_TWO_NULL,BST_CHECKED);
	CheckDlgButton(IDC_SEARCH_NOT_FULL,BST_CHECKED);

	return FALSE;
}

//��Ϣ����
BOOL CDlgSearchTable::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//ȷ������
VOID CDlgSearchTable::OnOK()
{
	//��������
	tagSearchTable SearchTable;
	ZeroMemory(&SearchTable,sizeof(SearchTable));

	//���ñ���
	SearchTable.bOneNull=(IsDlgButtonChecked(IDC_SEARCH_ONE_NULL)==BST_CHECKED);
	SearchTable.bTwoNull=(IsDlgButtonChecked(IDC_SEARCH_TWO_NULL)==BST_CHECKED);
	SearchTable.bNotFull=(IsDlgButtonChecked(IDC_SEARCH_NOT_FULL)==BST_CHECKED);
	SearchTable.bAllNull=(IsDlgButtonChecked(IDC_SEARCH_ALL_NULL)==BST_CHECKED);
	SearchTable.bFilterPass=(IsDlgButtonChecked(IDC_FILTRATE_PASS)==BST_CHECKED);

	//��������
	SearchTable.wResultTableID=INVALID_TABLE;
	SearchTable.wResultChairID=INVALID_CHAIR;
	SearchTable.wStartTableID=m_wResultTableID+1;

	//��������
	ASSERT(m_pServerViewItem!=NULL);
	m_pServerViewItem->SearchGameTable(SearchTable);

	//���ý��
	m_wResultTableID=SearchTable.wResultTableID;

	//����ж�
	if (SearchTable.wResultTableID==INVALID_TABLE)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("��Ǹ����ʱ�Ҳ���������Ҫ�����Ϸ��������������������Ժ����ԣ�"),MB_ICONINFORMATION,30);

		return;
	}

	//���ý���
	m_btAutoJoin.EnableWindow(m_wResultTableID!=INVALID_TABLE);
	m_pTableViewFrame->VisibleTable(SearchTable.wResultTableID);

	return; 
}

//ȡ������
VOID CDlgSearchTable::OnCancel() 
{
	//�رմ���
	DestroyWindow();

	return; 
}

//���÷���
VOID CDlgSearchTable::SetServerViewItem(CServerViewItem * pServerViewItem)
{
	//���ñ���
	m_pServerViewItem=pServerViewItem;

	return;
}

//��������
VOID CDlgSearchTable::SetTableViewFrame(CTableViewFrame * pTableViewFrame)
{
	//���þ���
	ASSERT(pTableViewFrame!=NULL);
	m_pTableViewFrame=pTableViewFrame;

	return;
}

//���ټ���
VOID CDlgSearchTable::OnBnClickedAutoJoin()
{
	//Ч��״̬
	ASSERT(m_wResultTableID!=INVALID_TABLE);
	if (m_wResultTableID==INVALID_TABLE) return;

	//��������
	WORD wChairID=INVALID_CHAIR;
	m_pTableViewFrame->GetNullChairCount(m_wResultTableID,wChairID);

	//�����ж�
	if (wChairID==INVALID_CHAIR)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("����Ϸ���Ѿ������ˣ��޷��������Ϸ�����������������ٴγ��ԣ�"),MB_ICONINFORMATION,30);

		return;
	}

	//������Ϸ
	ASSERT(m_pServerViewItem!=NULL);
	m_pServerViewItem->OnLButtonHitTable(m_wResultTableID,wChairID);

	return;
}

//////////////////////////////////////////////////////////////////////////
