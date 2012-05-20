#include "StdAfx.h"
#include "GameRuleSet.h"
#include "GameClientDlg.h"
#include ".\gameruleset.h"

IMPLEMENT_DYNAMIC(CGameRuleSet, CSkinDialogEx)

BEGIN_MESSAGE_MAP(CGameRuleSet, CSkinDialogEx)
	ON_BN_CLICKED(IDC_STRAT_PLAY, OnBnClickedStratPlay)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

//���캯��
CGameRuleSet::CGameRuleSet(CWnd* pParent /*=NULL*/)
	: CSkinDialogEx(CGameRuleSet::IDD, pParent)
{
}

//��������
CGameRuleSet::~CGameRuleSet()
{
}

//DDX/DDV ֧��
void CGameRuleSet::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STRAT_PLAY, m_Play);
}

//��ʼ������
BOOL CGameRuleSet::OnInitDialog()
{
	CSkinDialogEx::OnInitDialog();
	return TRUE;
}


void CGameRuleSet::OnBnClickedStratPlay()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	((CGameClientDlg*)AfxGetMainWnd())->GameStart();
	this->OnOK();
}
void CGameRuleSet::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	((CGameClientDlg*)AfxGetMainWnd())->GameQuit();
	CSkinDialogEx::OnClose();
}
