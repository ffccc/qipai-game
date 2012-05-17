#include "Stdafx.h"
#include "Resource.h"
#include "GameRule.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameRule, CDialog)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_ENABLE_STEP_TIME, OnBnClickedEnableStep)
	ON_BN_CLICKED(IDC_ENABLE_LIMIT_TIME, OnBnClickedEnableLimit)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameRule::CGameRule() : CDialog(IDD_GAME_RULE)
{
	//�������
	memset(&m_GameRuleInfo,0,sizeof(m_GameRuleInfo));

	//������Դ
	m_ImageBack.SetLoadInfo(IDB_GAME_RULE,AfxGetInstanceHandle());

	return;
}

//��������
CGameRule::~CGameRule()
{
}

//�ؼ���
void CGameRule::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
}

//��ʼ������
BOOL CGameRule::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("�������ã�"));

	//������ˢ
	m_Brush.CreateSolidBrush(RGB(245,238,221));

	//�ƶ�����
	CImageHandle BackImageHandle(&m_ImageBack);
	SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOZORDER|SWP_NOMOVE);

	//���ð�ť
	m_btOk.SetButtonImage(IDB_RULE_AGREE,AfxGetInstanceHandle(),false);
	m_btCancel.SetButtonImage(IDB_RULE_QUIT,AfxGetInstanceHandle(),false);

	//��������
	int nItem=0;
	TCHAR szBuffer[64];
	WORD wStepTime[]={10,30,60};
	BYTE wTimeCountCount[]={1,3,6};
	WORD wLimitTime[]={300,600,1200,1800,3600};

	//��ʱ��ʱ
	CComboBox * pComboBox=(CComboBox *)GetDlgItem(IDC_STEP_TIME);
	for (int i=0;i<CountArray(wStepTime);i++)
	{
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("%d"),wStepTime[i]);
		nItem=pComboBox->InsertString(i,szBuffer);
		pComboBox->SetItemData(nItem,wStepTime[i]);
	}

	//��ʱ����
	pComboBox=(CComboBox *)GetDlgItem(IDC_TIME_OUT_COUNT);
	for (int i=0;i<CountArray(wTimeCountCount);i++)
	{
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("%d"),wTimeCountCount[i]);
		nItem=pComboBox->InsertString(i,szBuffer);
		pComboBox->SetItemData(nItem,wTimeCountCount[i]);
	}

	//�޶�ʱ��
	pComboBox=(CComboBox *)GetDlgItem(IDC_LIMIT_TIME);
	for (int i=0;i<CountArray(wLimitTime);i++)
	{
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("%d"),wLimitTime[i]/60);
		nItem=pComboBox->InsertString(i,szBuffer);
		pComboBox->SetItemData(nItem,wLimitTime[i]/60);
	}

	//����״̬
	UpdateControlStatus();

	return TRUE;
}

//ȷ����Ϣ
void CGameRule::OnOK()
{
	//�޶���ʱ
	CButton * pButton=(CButton *)(GetDlgItem(IDC_ENABLE_STEP_TIME));
	if (pButton->GetCheck()==BST_CHECKED)
	{
		//��ʱ��ʱ
		if (GetComboBoxValue(IDC_STEP_TIME,m_GameRuleInfo.wRuleStepTime)==false)
		{
			AfxMessageBox(TEXT("����ʱ�����������������ȷ�Ķ���ʱ�䣡"),MB_ICONQUESTION);
			GetDlgItem(IDC_STEP_TIME)->SetFocus();
			return;
		}

		//��ʱ����
		if (GetComboBoxValue(IDC_TIME_OUT_COUNT,m_GameRuleInfo.wRuleTimeOutCount)==false)
		{
			AfxMessageBox(TEXT("�����������������������ȷ�Ķ��������"),MB_ICONQUESTION);
			GetDlgItem(IDC_TIME_OUT_COUNT)->SetFocus();
			return;
		}
	}
	else 
	{
		m_GameRuleInfo.wRuleStepTime=0;
		m_GameRuleInfo.wRuleTimeOutCount=0;
	}

	//�޶�ʱ��
	pButton=(CButton *)(GetDlgItem(IDC_ENABLE_LIMIT_TIME));
	if (pButton->GetCheck()==BST_CHECKED)
	{
		//�޶�ʱ��
		if (GetComboBoxValue(IDC_LIMIT_TIME,m_GameRuleInfo.wRuleLimitTime)==false)
		{
			AfxMessageBox(TEXT("�޶�ʱ�����������������ȷ���޶�ʱ�䣡"),MB_ICONQUESTION);
			GetDlgItem(IDC_LIMIT_TIME)->SetFocus();
			return;
		}

		//��������
		m_GameRuleInfo.wRuleLimitTime*=60;
	}
	else m_GameRuleInfo.wRuleLimitTime=0;

	//����ģʽ
	pButton=(CButton *)GetDlgItem(IDC_MATCH_MODE);
	m_GameRuleInfo.cbGameMode=(pButton->GetCheck()==BST_CHECKED)?GM_MATCH:GM_FRIEND;

	//ָ���־
	m_GameRuleInfo.cbDirections=(((CButton *)GetDlgItem(IDC_DIRECTIONS))->GetCheck()==BST_CHECKED)?TRUE:FALSE;

	//������Ϣ
	AfxGetMainWnd()->PostMessage(IDM_AGREE_GAME_RULE,0,0);

	//�رմ���
	DestroyWindow();

	return;
}

//ȡ����Ϣ
void CGameRule::OnCancel()
{
	//������Ϣ
	AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);

	//�رմ���
	DestroyWindow();

	return;
}

//����״̬
void CGameRule::UpdateControlStatus()
{
	//����Ч��
	if (GetSafeHwnd()==NULL) return;

	//ʱ�����
	SetComboBoxValue(IDC_STEP_TIME,m_GameRuleInfo.wRuleStepTime);
	SetComboBoxValue(IDC_LIMIT_TIME,m_GameRuleInfo.wRuleLimitTime/60);
	SetComboBoxValue(IDC_TIME_OUT_COUNT,m_GameRuleInfo.wRuleTimeOutCount);

	//�ȶ���ʱ
	if (m_GameRuleInfo.wRuleStepTime!=0) ((CButton *)GetDlgItem(IDC_ENABLE_STEP_TIME))->SetCheck(BST_CHECKED);
	else ((CButton *)GetDlgItem(IDC_ENABLE_STEP_TIME))->SetCheck(BST_UNCHECKED);

	//��ʱ�ж�
	if (m_GameRuleInfo.wRuleLimitTime!=0) ((CButton *)GetDlgItem(IDC_ENABLE_LIMIT_TIME))->SetCheck(BST_CHECKED);
	else ((CButton *)GetDlgItem(IDC_ENABLE_LIMIT_TIME))->SetCheck(BST_UNCHECKED);

	//����ģʽ
	CButton * pButton=(CButton *)GetDlgItem(IDC_MATCH_MODE);
	pButton->SetCheck((m_GameRuleInfo.cbGameMode==GM_MATCH)?BST_CHECKED:BST_UNCHECKED);

	//����ģʽ
	pButton=(CButton *)GetDlgItem(IDC_FRIEND_MODE);
	pButton->SetCheck((m_GameRuleInfo.cbGameMode==GM_FRIEND)?BST_CHECKED:BST_UNCHECKED);

	//ָ������
	pButton=(CButton *)GetDlgItem(IDC_DIRECTIONS);
	pButton->SetCheck(m_GameRuleInfo.cbDirections?BST_CHECKED:BST_UNCHECKED);

	return;
}

//�������
void CGameRule::ControlEnable(bool bEnable)
{
	//��������
	BOOL bEnableFlag=bEnable?TRUE:FALSE;

	//���ÿؼ�
	GetDlgItem(IDC_MATCH_MODE)->EnableWindow(bEnableFlag);
	GetDlgItem(IDC_FRIEND_MODE)->EnableWindow(bEnableFlag);
	GetDlgItem(IDC_DIRECTIONS)->EnableWindow(bEnableFlag);
	GetDlgItem(IDC_ENABLE_STEP_TIME)->EnableWindow(bEnableFlag);
	GetDlgItem(IDC_ENABLE_LIMIT_TIME)->EnableWindow(bEnableFlag);

	//�������
	if (bEnable==true)
	{
		//�޶���ʱ
		if (((CButton *)GetDlgItem(IDC_ENABLE_STEP_TIME))->GetCheck()==BST_CHECKED)
		{
			GetDlgItem(IDC_STEP_TIME)->EnableWindow(true);
			GetDlgItem(IDC_TIME_OUT_COUNT)->EnableWindow(true);
		}
		else
		{
			GetDlgItem(IDC_STEP_TIME)->EnableWindow(false);
			GetDlgItem(IDC_TIME_OUT_COUNT)->EnableWindow(false);
		}

		//�޶�ʱ��
		if (((CButton *)GetDlgItem(IDC_ENABLE_LIMIT_TIME))->GetCheck()==BST_CHECKED)
		{
			GetDlgItem(IDC_LIMIT_TIME)->EnableWindow(true);
		}
		else GetDlgItem(IDC_LIMIT_TIME)->EnableWindow(false);
	}
	else
	{
		GetDlgItem(IDC_STEP_TIME)->EnableWindow(false);
		GetDlgItem(IDC_LIMIT_TIME)->EnableWindow(false);
		GetDlgItem(IDC_TIME_OUT_COUNT)->EnableWindow(false);
	}

	return;
}

//����ѡ��
bool CGameRule::SetComboBoxValue(UINT uComboBoxID, WORD wValue)
{
	//��ȡ�ؼ�
	CComboBox * pComboBox=(CComboBox *)GetDlgItem(uComboBoxID);

	//Ѱ��ѡ��
	int nItemCount=pComboBox->GetCount();
	for (int i=0;i<nItemCount;i++)
	{
		if (wValue==(WORD)pComboBox->GetItemData(i))
		{
			pComboBox->SetCurSel(i);
			return true;
		}
	}

	//������д
	TCHAR szBuffer[16];
	_snprintf(szBuffer,sizeof(szBuffer),TEXT("%d"),wValue);
	pComboBox->SetWindowText(szBuffer);

	return true;
}

//��ȡ��ֵ
bool CGameRule::GetComboBoxValue(UINT uComboBoxID, WORD & wValue)
{
	//��ȡ�ؼ�
	CComboBox * pComboBox=(CComboBox *)GetDlgItem(uComboBoxID);

	//��ȡѡ��
	int nItem=pComboBox->GetCurSel();
	if (nItem!=LB_ERR)
	{
		wValue=(WORD)pComboBox->GetItemData(nItem);
		return true;
	}

	//��ȡ�ַ�
	TCHAR szBuffer[32];
	pComboBox->GetWindowText(szBuffer,CountArray(szBuffer));
	if (szBuffer[0]==0) return false;

	//����Ч��
	int nIndex=0;
	do
	{
		if ((szBuffer[nIndex]<TEXT('0'))||(szBuffer[nIndex]>TEXT('9'))) return false;
	} while (szBuffer[++nIndex]!=0);

	//��ֵת��
	wValue=atoi(szBuffer);

	return true;
}

//�ػ�����
void CGameRule::OnPaint()
{
	CPaintDC dc(this);

	//�滭����
	CImageHandle BackImageHandle(&m_ImageBack);
	m_ImageBack.BitBlt(dc,0,0);

	return;
}

//�滭����
BOOL CGameRule::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();

	return TRUE;
}

//������Ϣ
void CGameRule::OnDestroy()
{
	//������Դ
	m_Brush.DeleteObject();

	__super::OnDestroy();
}

//�����Ϣ
void CGameRule::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonDown(nFlags,point);

	//��Ϣģ��
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));

	return;
}

//�ؼ���ɫ
HBRUSH CGameRule::OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
	case CTLCOLOR_STATIC:
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(10,10,10));
			pDC->SetBkColor(RGB(245,238,221));
			return m_Brush;
		}
	}
	return __super::OnCtlColor(pDC,pWnd,nCtlColor);
}

//�޶���ʱ
void CGameRule::OnBnClickedEnableStep()
{
	//���ÿؼ�
	if (((CButton *)GetDlgItem(IDC_ENABLE_STEP_TIME))->GetCheck()==BST_CHECKED)
	{
		GetDlgItem(IDC_STEP_TIME)->EnableWindow(true);
		GetDlgItem(IDC_TIME_OUT_COUNT)->EnableWindow(true);
	}
	else
	{
		GetDlgItem(IDC_STEP_TIME)->EnableWindow(false);
		GetDlgItem(IDC_TIME_OUT_COUNT)->EnableWindow(false);
	}

	return;
}

//�޶���ʱ
void CGameRule::OnBnClickedEnableLimit()
{
	//���ÿؼ�
	if (((CButton *)GetDlgItem(IDC_ENABLE_LIMIT_TIME))->GetCheck()==BST_CHECKED)
	{
		GetDlgItem(IDC_LIMIT_TIME)->EnableWindow(true);
	}
	else GetDlgItem(IDC_LIMIT_TIME)->EnableWindow(false);

	return;
}

//////////////////////////////////////////////////////////////////////////
