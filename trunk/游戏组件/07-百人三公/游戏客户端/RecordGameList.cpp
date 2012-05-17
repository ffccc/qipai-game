#include "StdAfx.h"
#include "Resource.h"
#include "RecordGameList.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameRecord, CSkinDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameRecord::CGameRecord() : CSkinDialogEx(IDD_DLG_GAME_RECORD)
{
}

//��������
CGameRecord::~CGameRecord()
{
}

//�ؼ���
void CGameRecord::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RECORD_LIST, m_RecordGameList);
}

//��ʼ������
BOOL CGameRecord::OnInitDialog()
{
	__super::OnInitDialog();

	//������ɫ
	m_RecordGameList.SetTextColor(RGB(52,87,130));
	m_RecordGameList.SetBkColor(RGB(255,255,255));
	m_RecordGameList.SetTextBkColor(RGB(255,255,255));

	//�����б�
	m_RecordGameList.InsertColumn(0,TEXT("����"),LVCFMT_LEFT,50);
	m_RecordGameList.InsertColumn(1,TEXT("Ӯ��"),LVCFMT_LEFT,50);
	m_RecordGameList.InsertColumn(2,TEXT("���ֳɼ�"),LVCFMT_LEFT,70);
	m_RecordGameList.InsertColumn(3,TEXT("������ע"),LVCFMT_LEFT,70);
	m_RecordGameList.InsertColumn(4,TEXT("������ע"),LVCFMT_LEFT,80);
	m_RecordGameList.InsertColumn(5,TEXT("��ƽ��ע"),LVCFMT_LEFT,80);
	m_RecordGameList.InsertColumn(6,TEXT("��ׯ��ע"),LVCFMT_LEFT,80);
	m_RecordGameList.InsertColumn(7,TEXT("�м��Ƶ�"),LVCFMT_LEFT,70);
	m_RecordGameList.InsertColumn(8,TEXT("ׯ���Ƶ�"),LVCFMT_LEFT,70);

	return FALSE;
}

//�����б�
void CGameRecord::FillGameRecourd(tagGameRecord & GameRecord)
{
	//��������
	WORD wListIndex=0;
	WORD wColumnCount=1;
	TCHAR szBuffer[128]=TEXT("");

	//��Ϸ����
	_snprintf(szBuffer,CountArray(szBuffer),TEXT("%d"),GameRecord.wDrawCount);
	INT nItemIndex=m_RecordGameList.InsertItem(m_RecordGameList.GetItemCount(),szBuffer);

	//��ϷӮ��
	if (GameRecord.cbWinerSide==0) m_RecordGameList.SetItem(nItemIndex,wColumnCount++,LVIF_TEXT,TEXT("�м�"),0,0,0,0);
	else if (GameRecord.cbWinerSide==1) m_RecordGameList.SetItem(nItemIndex,wColumnCount++,LVIF_TEXT,TEXT("ƽ��"),0,0,0,0);
	else m_RecordGameList.SetItem(nItemIndex,wColumnCount++,LVIF_TEXT,TEXT("ׯ��"),0,0,0,0);

	//���ֳɼ�
	_snprintf(szBuffer,CountArray(szBuffer),TEXT("%ld"),GameRecord.lGameScore);
	m_RecordGameList.SetItem(nItemIndex,wColumnCount++,LVIF_TEXT,szBuffer,0,0,0,0);

	//������ע
	_snprintf(szBuffer,CountArray(szBuffer),TEXT("%ld"),GameRecord.lMyAddGold);
	m_RecordGameList.SetItem(nItemIndex,wColumnCount++,LVIF_TEXT,szBuffer,0,0,0,0);

	//�м���ע
	_snprintf(szBuffer,CountArray(szBuffer),TEXT("%ld"),GameRecord.lDrawPlayerScore);
	m_RecordGameList.SetItem(nItemIndex,wColumnCount++,LVIF_TEXT,szBuffer,0,0,0,0);

	//ƽ����ע
	_snprintf(szBuffer,CountArray(szBuffer),TEXT("%ld"),GameRecord.lDrawTieScore);
	m_RecordGameList.SetItem(nItemIndex,wColumnCount++,LVIF_TEXT,szBuffer,0,0,0,0);

	//ׯ����ע
	_snprintf(szBuffer,CountArray(szBuffer),TEXT("%ld"),GameRecord.lDrawBankerScore);
	m_RecordGameList.SetItem(nItemIndex,wColumnCount++,LVIF_TEXT,szBuffer,0,0,0,0);

	//�м��Ƶ�
	_snprintf(szBuffer,CountArray(szBuffer),TEXT("%ld"),GameRecord.cbPlayerPoint);
	m_RecordGameList.SetItem(nItemIndex,wColumnCount++,LVIF_TEXT,szBuffer,0,0,0,0);

	//ׯ���Ƶ�
	_snprintf(szBuffer,CountArray(szBuffer),TEXT("%ld"),GameRecord.cbBankerPoint);
	m_RecordGameList.SetItem(nItemIndex,wColumnCount++,LVIF_TEXT,szBuffer,0,0,0,0);

	//�ƶ��б�
	int nCount = m_RecordGameList.GetItemCount();
	if (nCount > 0)
		m_RecordGameList.EnsureVisible(nCount-1, FALSE);

	return;
}

//λ�ñ仯
void CGameRecord::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	//�б�ؼ�
	if (m_RecordGameList.m_hWnd!=NULL)
	{
		m_RecordGameList.MoveWindow(0,0,cx,cy);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////
