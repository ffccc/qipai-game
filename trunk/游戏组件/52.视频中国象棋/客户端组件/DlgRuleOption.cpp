#include "Stdafx.h"
#include "Resource.h"
#include "DlgRuleOption.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//ʱ���ʶ
#define IDI_GAME_RULE_TIME			10									//ʱ���ʶ

//��ɫ����
#define COLOR_RULE_BACK				RGB(221,197,164)					//������ɫ

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgRuleOption, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_FAST_MODE, OnBnClickedFastMode)
	ON_BN_CLICKED(IDC_SLOW_MODE, OnBnClickedSlowMode)
	ON_BN_CLICKED(IDC_CUSTOM_MODE, OnBnClickedCustomMode)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CDlgRuleOption::CDlgRuleOption() : CDialog(IDD_RULE_OPTION)
{
	//״̬����
	m_nResidualTime=0L;

	//���ñ���
	m_bControl=false;
	m_bSelectMode=false;
	ZeroMemory(&m_GameRuleInfo,sizeof(m_GameRuleInfo));

	//������Դ
	m_ImageViewBack.SetLoadInfo(IDB_RULE_OPTION_VIEW,AfxGetInstanceHandle());

	return;
}

//��������
CDlgRuleOption::~CDlgRuleOption()
{
}

//�ؼ���
VOID CDlgRuleOption::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
}

//��ʼ������
BOOL CDlgRuleOption::OnInitDialog()
{
	__super::OnInitDialog();

	//��������
	CRgn RgnImage;
	CImageHandle BackImageHandle(&m_ImageViewBack);
	m_ImageViewBack.CreateImageRegion(RgnImage,RGB(255,0,255));

	//���ô���
	SetWindowRgn(RgnImage,TRUE);
	SetWindowPos(NULL,0,0,m_ImageViewBack.GetWidth(),m_ImageViewBack.GetHeight(),SWP_NOZORDER|SWP_NOMOVE);

	//���ô���
	SetWindowText(TEXT("�������ã�"));
	m_Brush.CreateSolidBrush(COLOR_RULE_BACK);

	//���ÿؼ�
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_btOk.SetButtonImage(IDB_OPTION_BT_OK,hResInstance,false);
	m_btCancel.SetButtonImage(IDB_OPTION_BT_CANCEL,hResInstance,false);

	//������ʾ
	TCHAR szFastDescribe[128]=TEXT(""),szSlowDescribe[128]=TEXT("");
	_sntprintf(szFastDescribe,CountChar(szFastDescribe),TEXT("��ʱ %d ����  ��ʱ %d ����  ���� %d ��"),
		MODE_FAST_DRAW_TIME/60L,MODE_FAST_STEP_TIME/60L,MODE_FAST_SECOND_TIME);
	_sntprintf(szSlowDescribe,CountChar(szSlowDescribe),TEXT("��ʱ %d ����  ��ʱ %d ����  �޶���"),
		MODE_SLOW_DRAW_TIME/60L,MODE_SLOW_STEP_TIME/60L);

	//������ʾ
	SetDlgItemText(IDC_FAST_DESCRIBE,szFastDescribe);
	SetDlgItemText(IDC_SLOW_DESCRIBE,szSlowDescribe);

	//��������
	TCHAR szBuffer[32]=TEXT("");
	LONG lDrawTime[]={600L,1800L,3600L},lStepTime[]={60L,180L,300L};
	LONG lRuleSecondTime[]={30L,60L,180L,300L},lRuleSecondCount[]={1L,3L,5L};

	//��Ϸ��ʱ
	CComboBox * pComboBox=(CComboBox *)GetDlgItem(IDC_DRAW_TIME);
	for (INT i=0;i<CountArray(lDrawTime);i++)
	{
		_sntprintf(szBuffer,CountChar(szBuffer),TEXT("%ld"),lDrawTime[i]/60L);
		pComboBox->SetItemData(pComboBox->InsertString(i,szBuffer),lDrawTime[i]/60L);
	}

	//��Ϸ��ʱ
	pComboBox=(CComboBox *)GetDlgItem(IDC_STEP_TIME);
	for (INT i=0;i<CountArray(lStepTime);i++)
	{
		_sntprintf(szBuffer,CountChar(szBuffer),TEXT("%ld"),lStepTime[i]/60L);
		pComboBox->SetItemData(pComboBox->InsertString(i,szBuffer),lStepTime[i]/60L);
	}

	//��Ϸ����
	pComboBox=(CComboBox *)GetDlgItem(IDC_SECOND_TIME);
	for (INT i=0;i<CountArray(lRuleSecondTime);i++)
	{
		_sntprintf(szBuffer,CountChar(szBuffer),TEXT("%ld"),lRuleSecondTime[i]);
		pComboBox->SetItemData(pComboBox->InsertString(i,szBuffer),lRuleSecondTime[i]);
	}

	//���¿ؼ�
	UpdateControlStatus();

	return TRUE;
}

//ȷ����Ϣ
VOID CDlgRuleOption::OnOK()
{
	//��ȡ����
	if (m_bControl==true)
	{
		//��ʱ����
		WORD wRuleDrawTime=0;
		WORD wRuleStepTime=0;
		WORD wRuleSecondTime=0;
		GetComboBoxValue(IDC_DRAW_TIME,wRuleDrawTime);
		GetComboBoxValue(IDC_STEP_TIME,wRuleStepTime);
		GetComboBoxValue(IDC_SECOND_TIME,wRuleSecondTime);

		//���ñ���
		m_GameRuleInfo.wRuleDrawTime=wRuleDrawTime*60L;
		m_GameRuleInfo.wRuleStepTime=wRuleStepTime*60L;
		m_GameRuleInfo.wRuleSecondTime=wRuleSecondTime;

		//��ʱģʽ
		if (IsDlgButtonChecked(IDC_FAST_MODE)==BST_CHECKED) m_GameRuleInfo.cbCurrentMode=MODE_FAST;
		if (IsDlgButtonChecked(IDC_SLOW_MODE)==BST_CHECKED) m_GameRuleInfo.cbCurrentMode=MODE_SLOW;
		if (IsDlgButtonChecked(IDC_CUSTOM_MODE)==BST_CHECKED) m_GameRuleInfo.cbCurrentMode=MODE_CUSTOM;

		//��Ϸģʽ
		m_GameRuleInfo.cbDirections=(IsDlgButtonChecked(IDC_DIRECTIONS)==BST_CHECKED)?TRUE:FALSE;
		m_GameRuleInfo.cbRegretFlag=(IsDlgButtonChecked(IDC_REGRET_FLAG)==BST_CHECKED)?TRUE:FALSE;
	}

	//��ѯ�ӿ�
	//CGlobalUnits * pGlobalUnits=CGlobalUnits::GetGlobalUnits();
	//IGameFrameEngine * pIGameFrameEngine=(IGameFrameEngine *)pGlobalUnits->QueryGlobalModule(MODULE_GAME_FRAME_ENGINE,IID_IGameFrameEngine,VER_IGameFrameEngine);

	//������Ϣ
	//ASSERT(pIGameFrameEngine!=NULL);
	/*if (pIGameFrameEngine!=NULL)*/ AfxGetMainWnd()->SendMessage(IDM_GAME_RULE_AGREE,0,0);

	//�رմ���
	DestroyWindow();

	return;
}

//ȡ����Ϣ
VOID CDlgRuleOption::OnCancel()
{
	//��ѯ�ӿ�
	//CGlobalUnits * pGlobalUnits=CGlobalUnits::GetGlobalUnits();
	//IGameFrameEngine * pIGameFrameEngine=(IGameFrameEngine *)pGlobalUnits->QueryGlobalModule(MODULE_GAME_FRAME_ENGINE,IID_IGameFrameEngine,VER_IGameFrameEngine);

	//������Ϣ
	//ASSERT(pIGameFrameEngine!=NULL);
	/*if (pIGameFrameEngine!=NULL)*/ AfxGetMainWnd()->SendMessage(IDM_GAME_RULE_REJECT,0,0);

	//�رմ���
	DestroyWindow();

	return;
}

//���ÿ���
VOID CDlgRuleOption::EnableControl(bool bControl)
{
	//���ñ���
	m_bControl=bControl;

	//���¿���
	if (m_hWnd!=NULL) UpdateControlStatus();

	return;
}

//��ȡ����
VOID CDlgRuleOption::GetGameRuleInfo(tagGameRuleInfo & GameRuleInfo)
{
	//���ñ���
	GameRuleInfo=m_GameRuleInfo;

	return;
}

//��������
VOID CDlgRuleOption::SetGameRuleInfo(const tagGameRuleInfo & GameRuleInfo, bool bControl, bool bSelectMode)
{
	//���ñ���
	m_bControl=bControl;
	m_bSelectMode=bSelectMode;
	m_GameRuleInfo=GameRuleInfo;

	//���¿���
	if (m_hWnd!=NULL) UpdateControlStatus();

	return;
}

//����״̬
VOID CDlgRuleOption::UpdateControlStatus()
{
	//��������
	INT nShowCmd=(m_bControl==true)?SW_SHOW:SW_HIDE;

	//��ʱ����
	GetDlgItem(IDC_DRAW_TIME)->ShowWindow(nShowCmd);
	GetDlgItem(IDC_STEP_TIME)->ShowWindow(nShowCmd);
	GetDlgItem(IDC_SECOND_TIME)->ShowWindow(nShowCmd);

	//���ÿ���
	GetDlgItem(IDC_FAST_MODE)->ShowWindow(nShowCmd);
	GetDlgItem(IDC_SLOW_MODE)->ShowWindow(nShowCmd);
	GetDlgItem(IDC_CUSTOM_MODE)->ShowWindow(nShowCmd);

	//��ʾ�ַ�
	GetDlgItem(IDC_FAST_DESCRIBE)->ShowWindow(nShowCmd);
	GetDlgItem(IDC_SLOW_DESCRIBE)->ShowWindow(nShowCmd);
	GetDlgItem(IDC_CUSTOM_STRING_1)->ShowWindow(nShowCmd);
	GetDlgItem(IDC_CUSTOM_STRING_2)->ShowWindow(nShowCmd);
	GetDlgItem(IDC_CUSTOM_STRING_3)->ShowWindow(nShowCmd);
	GetDlgItem(IDC_CUSTOM_STRING_4)->ShowWindow(nShowCmd);
	GetDlgItem(IDC_CUSTOM_STRING_5)->ShowWindow(nShowCmd);
	GetDlgItem(IDC_CUSTOM_STRING_6)->ShowWindow(nShowCmd);

	//��ʱ����
	SetComboBoxValue(IDC_DRAW_TIME,m_GameRuleInfo.wRuleDrawTime/60L);
	SetComboBoxValue(IDC_STEP_TIME,m_GameRuleInfo.wRuleStepTime/60L);
	SetComboBoxValue(IDC_SECOND_TIME,m_GameRuleInfo.wRuleSecondTime);

	//���ÿ���
	GetDlgItem(IDC_FAST_MODE)->EnableWindow(((m_bControl==true)&&(m_bSelectMode==true))?TRUE:FALSE);
	GetDlgItem(IDC_SLOW_MODE)->EnableWindow(((m_bControl==true)&&(m_bSelectMode==true))?TRUE:FALSE);
	GetDlgItem(IDC_CUSTOM_MODE)->EnableWindow(((m_bControl==true)&&(m_bSelectMode==true))?TRUE:FALSE);

	//ģʽ����
	CheckDlgButton(IDC_FAST_MODE,(m_GameRuleInfo.cbCurrentMode==MODE_FAST)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_SLOW_MODE,(m_GameRuleInfo.cbCurrentMode==MODE_SLOW)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_CUSTOM_MODE,(m_GameRuleInfo.cbCurrentMode==MODE_CUSTOM)?BST_CHECKED:BST_UNCHECKED);

	//�Զ��ؼ�
	if ((m_bControl==true)&&(m_bSelectMode==true)&&(m_GameRuleInfo.cbCurrentMode==MODE_CUSTOM))
	{
		GetDlgItem(IDC_DRAW_TIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_STEP_TIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_SECOND_TIME)->EnableWindow(TRUE);
	}

	//��Ϸ����
	GetDlgItem(IDC_DIRECTIONS)->EnableWindow((m_bControl==true)?TRUE:FALSE);
	GetDlgItem(IDC_REGRET_FLAG)->EnableWindow((m_bControl==true)?TRUE:FALSE);
	CheckDlgButton(IDC_DIRECTIONS,(m_GameRuleInfo.cbDirections==TRUE)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_REGRET_FLAG,(m_GameRuleInfo.cbRegretFlag==TRUE)?BST_CHECKED:BST_UNCHECKED);

	//����ʱ��
	m_nResidualTime=30L;
	SetTimer(IDI_GAME_RULE_TIME,1000,NULL);

	//���½���
	Invalidate(FALSE);

	return;
}

//����ѡ��
bool CDlgRuleOption::SetComboBoxValue(UINT uComboBoxID, WORD wValue)
{
	//��ȡ�ؼ�
	CComboBox * pComboBox=(CComboBox *)GetDlgItem(uComboBoxID);

	//Ѱ��ѡ��
	INT nItemCount=pComboBox->GetCount();
	for (INT i=0;i<nItemCount;i++)
	{
		if (wValue==(WORD)pComboBox->GetItemData(i))
		{
			pComboBox->SetCurSel(i);
			return true;
		}
	}

	//�����ַ�
	TCHAR szBuffer[16];
	_sntprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),wValue);

	//���ÿؼ�
	pComboBox->SetWindowText(szBuffer);

	return true;
}

//��ȡ��ֵ
bool CDlgRuleOption::GetComboBoxValue(UINT uComboBoxID, WORD & wValue)
{
	//��ȡ�ؼ�
	CComboBox * pComboBox=(CComboBox *)GetDlgItem(uComboBoxID);

	//��ȡѡ��
	INT nItem=pComboBox->GetCurSel();
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
	INT nIndex=0;
	do
	{
		if ((szBuffer[nIndex]<TEXT('0'))||(szBuffer[nIndex]>TEXT('9'))) return false;
	} while (szBuffer[++nIndex]!=0);

	//��ֵת��
	wValue=_ttoi(szBuffer);

	return true;
}

//����ģʽ
VOID CDlgRuleOption::OnBnClickedFastMode()
{
	//������ֵ
	SetComboBoxValue(IDC_DRAW_TIME,MODE_FAST_DRAW_TIME/60L);
	SetComboBoxValue(IDC_STEP_TIME,MODE_FAST_STEP_TIME/60L);
	SetComboBoxValue(IDC_SECOND_TIME,MODE_FAST_SECOND_TIME);

	//���ÿؼ�
	GetDlgItem(IDC_DRAW_TIME)->EnableWindow(FALSE);
	GetDlgItem(IDC_STEP_TIME)->EnableWindow(FALSE);
	GetDlgItem(IDC_SECOND_TIME)->EnableWindow(FALSE);

	return;
}

//����ģʽ
VOID CDlgRuleOption::OnBnClickedSlowMode()
{
	//������ֵ
	SetComboBoxValue(IDC_DRAW_TIME,MODE_SLOW_DRAW_TIME/60L);
	SetComboBoxValue(IDC_STEP_TIME,MODE_SLOW_STEP_TIME/60L);
	SetComboBoxValue(IDC_SECOND_TIME,MODE_SLOW_SECOND_TIME);

	//���ÿؼ�
	GetDlgItem(IDC_DRAW_TIME)->EnableWindow(FALSE);
	GetDlgItem(IDC_STEP_TIME)->EnableWindow(FALSE);
	GetDlgItem(IDC_SECOND_TIME)->EnableWindow(FALSE);

	return;
}

//�Զ�ģʽ
VOID CDlgRuleOption::OnBnClickedCustomMode()
{
	//���ÿؼ�
	GetDlgItem(IDC_DRAW_TIME)->EnableWindow(TRUE);
	GetDlgItem(IDC_STEP_TIME)->EnableWindow(TRUE);
	GetDlgItem(IDC_SECOND_TIME)->EnableWindow(TRUE);

	return;
}

//�ػ�����
VOID CDlgRuleOption::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CDC DCBuffer;
	CBitmap ImageBuffer;
	DCBuffer.CreateCompatibleDC(&dc);
	DCBuffer.SetTextAlign(TA_LEFT|TA_TOP);
	ImageBuffer.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());

	//���� DC
	DCBuffer.SetBkMode(TRANSPARENT);
	DCBuffer.SelectObject(&ImageBuffer);
	DCBuffer.SelectObject(CSkinResourceManager::GetDefaultFont());

	//�滭����
	CImageHandle HandleViewBack(&m_ImageViewBack);
	m_ImageViewBack.BitBlt(DCBuffer,0,0);

	//�滭��Ϣ
	if (m_bControl==false)
	{
		//���� DC
		DCBuffer.SetTextColor(RGB(64,10,10));

		//��Ϸģʽ
		TCHAR szModeString[64]=TEXT("");
		LPCTSTR pszGameMode[]={TEXT("����ģʽ"),TEXT("����ģʽ"),TEXT("�Զ���ģʽ")};
		_sntprintf(szModeString,CountChar(szModeString),TEXT("��Ϸģʽ��%s"),pszGameMode[m_GameRuleInfo.cbCurrentMode-1]);

		//��Ϸ��ʱ
		TCHAR szDrawString[64]=TEXT("");
		_sntprintf(szDrawString,CountChar(szDrawString),TEXT("��Ϸ��ʱ��%d ����"),m_GameRuleInfo.wRuleDrawTime/60);

		//��Ϸ��ʱ
		TCHAR szStepString[64]=TEXT("");
		_sntprintf(szStepString,CountChar(szStepString),TEXT("��Ϸ��ʱ��%d ����"),m_GameRuleInfo.wRuleStepTime/60);

		//��Ϸ����
		TCHAR szSecondString[64]=TEXT("");
		_sntprintf(szSecondString,CountChar(szSecondString),TEXT("����ʱ�䣺%d ��"),m_GameRuleInfo.wRuleSecondTime);

		//�����Ϣ
		const INT nXPos=50,nYPos=87,nStringSpace=25;
		DCBuffer.TextOut(nXPos,nYPos,szModeString,lstrlen(szModeString));
		DCBuffer.TextOut(nXPos,nYPos+nStringSpace,szDrawString,lstrlen(szDrawString));
		DCBuffer.TextOut(nXPos,nYPos+nStringSpace*2,szStepString,lstrlen(szStepString));
		DCBuffer.TextOut(nXPos,nYPos+nStringSpace*3,szSecondString,lstrlen(szSecondString));
	}

	//���� DC
	DCBuffer.SetTextColor(RGB(125,125,125));
	DCBuffer.SetTextAlign(TA_CENTER|TA_TOP);

	//ʣ��ʱ��
	TCHAR szTimeLeave[32]=TEXT("");
	_sntprintf(szTimeLeave,CountChar(szTimeLeave),TEXT("�˶Ի����� %d ����Զ��ر�"),m_nResidualTime);
	DCBuffer.TextOut(rcClient.Width()/2,rcClient.bottom-80,szTimeLeave,lstrlen(szTimeLeave));

	//�滭����
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&DCBuffer,0,0,SRCCOPY);

	//������Դ
	DCBuffer.DeleteDC();
	ImageBuffer.DeleteObject();

	return;
}

//�滭����
BOOL CDlgRuleOption::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();

	return TRUE;
}

//������Ϣ
VOID CDlgRuleOption::OnDestroy()
{
	//������Դ
	m_Brush.DeleteObject();

	__super::OnDestroy();
}

//ʱ����Ϣ
VOID CDlgRuleOption::OnTimer(UINT nIDEvent)
{
	if (nIDEvent==IDI_GAME_RULE_TIME)
	{
		//�ر��ж�
		if (m_nResidualTime>0)
		{
			//���ñ���
			m_nResidualTime--;

			//���½���
			Invalidate(FALSE);
		}
		else PostMessage(WM_CLOSE,0,0);

		return;
	}

	__super::OnTimer(nIDEvent);
}

//�����Ϣ
VOID CDlgRuleOption::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//��Ϣģ��
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));

	return;
}

//�ؼ���ɫ
HBRUSH CDlgRuleOption::OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
	case CTLCOLOR_STATIC:
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(64,10,10));
			pDC->SetBkColor(COLOR_RULE_BACK);
			return m_Brush;
		}
	}

	return __super::OnCtlColor(pDC,pWnd,nCtlColor);
}

//////////////////////////////////////////////////////////////////////////
