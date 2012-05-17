#include "StdAfx.h"
#include "Resource.h"
#include "Information.h"
#include "DlgInsureCounter.h"

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgInsureItem, CSkinDialog)
	ON_EN_CHANGE(IDC_SCORE, OnEnChangeScore)
	ON_BN_CLICKED(IDC_BT_QUERY_INFO, OnBnClickedQueryInfo)
	ON_BN_CLICKED(IDC_BT_SAVE_SCORE, OnBnClickedSaveScore)
	ON_BN_CLICKED(IDC_BT_TAKE_SCORE, OnBnClickedTakeScore)
	ON_STN_CLICKED(IDC_FORGET_INSURE, OnBnClickedForgetInsure)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgInsureItem::CDlgInsureItem() : CSkinDialog(IDD_INSURE_CONUTER)
{
	//�������
	m_bSaveMission=false;
	m_bTakeMission=false;
	m_bQueryMission=false;

	//���ñ���
	m_bInitInfo=false;
	ZeroMemory(&m_UserInsureInfo,sizeof(m_UserInsureInfo));

	return;
}

//��������
CDlgInsureItem::~CDlgInsureItem()
{
}

//�ؼ���
VOID CDlgInsureItem::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	//�ؼ�����
	DDX_Control(pDX, IDC_SCORE, m_edScore);
	DDX_Control(pDX, IDC_TAB_CTRL, m_TabControl);
	DDX_Control(pDX, IDC_PASSWORD, m_PasswordControl);
	DDX_Control(pDX, IDC_FORGET_INSURE, m_ForgetInsure);

	//��ť�ؼ�
	DDX_Control(pDX, IDC_BT_QUERY_INFO, m_btQueryInfo);
	DDX_Control(pDX, IDC_BT_TAKE_SCORE, m_btTakeScore);
	DDX_Control(pDX, IDC_BT_SAVE_SCORE, m_btSaveScore);
}

//��Ϣ����
BOOL CDlgInsureItem::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//��������
BOOL CDlgInsureItem::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("���չ�"));

	//������Դ
	AfxSetResourceHandle(GetModuleHandle(NULL));

	//���ÿؼ�
	m_edScore.LimitText(16L);
	m_TabControl.SetItemSize(CSize(100,25));

	//�����ǩ
	m_TabControl.InsertItem(0,TEXT(" �����ȡ�� "));
	//m_TabControl.InsertItem(1,TEXT(" ���������Ϸ�� "));

	//���ð�ť
	HINSTANCE hResInstance=GetModuleHandle(SHARE_CONTROL_DLL_NAME);
	m_btQueryInfo.SetButtonImage(IDB_BT_QUERY_INFO,hResInstance,false,false);
	m_btSaveScore.SetButtonImage(IDB_BT_INSURE_SCORE,TEXT("BT_SAVE_SCORE"),hResInstance,false,false);
	m_btTakeScore.SetButtonImage(IDB_BT_INSURE_SCORE,TEXT("BT_TAKE_SCORE"),hResInstance,false,false);

	//��ѯ��Ϣ
	OnBnClickedQueryInfo();

	return FALSE;
}

//ȷ����ť
VOID CDlgInsureItem::OnOK()
{
	//��ȡ��ť
	OnBnClickedTakeScore();

	return;
}

//�滭��Ϣ
VOID CDlgInsureItem::OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight)
{
	//��ȡλ��
	CRect rcScore;
	CRect rcTabControl;
	m_edScore.GetWindowRect(&rcScore);
	m_TabControl.GetWindowRect(&rcTabControl);

	//ת��λ��
	ScreenToClient(&rcScore);
	ScreenToClient(&rcTabControl);

	//�滭����
	INT nLBorder=m_SkinAttribute.m_EncircleInfoView.nLBorder;
	INT nRBorder=m_SkinAttribute.m_EncircleInfoView.nRBorder;
	pDC->FillSolidRect(nLBorder,rcTabControl.bottom-1L,nWidth-nLBorder-nRBorder,1,RGB(145,168,183));

	//�滭����
	COLORREF crScoreBorad=CSkinEdit::m_SkinAttribute.m_crEnableBorad;
	pDC->Draw3dRect(rcScore.left-6,rcScore.top-5,rcScore.Width()+12,24L,crScoreBorad,crScoreBorad);

	//������Դ
	CPngImage ImageInsureInfo;
	ImageInsureInfo.LoadImage(GetModuleHandle(SHARE_CONTROL_DLL_NAME),TEXT("INFO_FRAME"));

	//�滭��Ϣ
	INT nYPosInfo=75L;
	INT nXPosInfo=(nWidth-ImageInsureInfo.GetWidth())/2L;
	ImageInsureInfo.DrawImage(pDC,nXPosInfo,nYPosInfo,ImageInsureInfo.GetWidth(),ImageInsureInfo.GetHeight(),0,0);

	//������Ϣ
	if (m_bInitInfo==true)
	{
		//������Դ
		CPngImage ImageInsureIcon;
		ImageInsureIcon.LoadImage(GetModuleHandle(SHARE_CONTROL_DLL_NAME),TEXT("INSURE_ICON"));

		//�滭��־
		ImageInsureIcon.DrawImage(pDC,nXPosInfo+10,nYPosInfo+17);

		//�滭����
		DrawNumberString(pDC,m_UserInsureInfo.lUserScore,nXPosInfo+136,nYPosInfo+18L);
		DrawNumberString(pDC,m_UserInsureInfo.lUserInsure,nXPosInfo+136,nYPosInfo+53L);

		//�滭��ʾ
		LPCTSTR pszScoreString=TEXT("��Ϸ�ң�");
		LPCTSTR pszInsureString=TEXT("���չ�");
		pDC->TextOut(nXPosInfo+83,nYPosInfo+20L,pszScoreString,lstrlen(pszScoreString));
		pDC->TextOut(nXPosInfo+83,nYPosInfo+55L,pszInsureString,lstrlen(pszInsureString));
	}
	else
	{
	}

	//������ʾ
	TCHAR szString[128]=TEXT("");
	_sntprintf(szString,CountArray(szString),TEXT("��ܰ��ʾ��������Ϸ���������ѣ�ȡ����������Ϸ�ҽ��۳� %ld%% ��������"),100*INSURE_REVENUE_RATIO);

	//��ܰ��ʾ
	pDC->SetTextColor(RGB(50,50,50));
	pDC->TextOut(18,nHeight-28,szString,lstrlen(szString));

	return;
}

//��ʼ����
VOID CDlgInsureItem::OnMissionStart()
{
	return;
}

//��ֹ����
VOID CDlgInsureItem::OnMissionConclude()
{
	return;
}

//��ѯ����
VOID CDlgInsureItem::QueryUserInsureInfo()
{
	//���ñ���
	m_bSaveMission=false;
	m_bTakeMission=false;
	m_bQueryMission=true;

	//�¼�����
	OnMissionStart();

	//ִ�в�ѯ
	PerformQueryInfo();

	return;
}

//��ȡ���
SCORE CDlgInsureItem::GetUserControlScore()
{
	//��������
	WORD wIndex=0;
	SCORE lScore=0L;

	//��ȡ��ֵ
	TCHAR szBuffer[32]=TEXT("");
	GetDlgItemText(IDC_SCORE,szBuffer,CountArray(szBuffer));

	//������ֵ
	while (szBuffer[wIndex]!=0)
	{
		//��������
		WORD wCurrentIndex=wIndex++;

		//������ֵ
		if ((szBuffer[wCurrentIndex]>=TEXT('0'))&&(szBuffer[wCurrentIndex]<=TEXT('9')))
		{
			lScore=lScore*10L+(szBuffer[wCurrentIndex]-TEXT('0'));
		}
	}

	return lScore;
}

//�滭����
VOID CDlgInsureItem::DrawNumberString(CDC * pDC, SCORE lScore, INT nXPos, INT nYPos)
{
	//ת������
	TCHAR szControl[128]=TEXT("");
	SwitchScoreFormat(lScore,3L,szControl,CountArray(szControl));

	//��������
	INT nXDrawPos=nXPos;
	INT nScoreLength=lstrlen(szControl);

	//�滭�ж�
	if (nScoreLength>0L)
	{
		//������Դ
		CPngImage ImageNumber;
		ImageNumber.LoadImage(GetModuleHandle(SHARE_CONTROL_DLL_NAME),TEXT("SCORE_NUMBER"));

		//��ȡ��С
		CSize SizeNumber;
		SizeNumber.SetSize(ImageNumber.GetWidth()/12L,ImageNumber.GetHeight());

		//�滭����
		for (INT i=0;i<nScoreLength;i++)
		{
			//�滭����
			if (szControl[i]==TEXT(','))
			{
				ImageNumber.DrawImage(pDC,nXDrawPos,nYPos,SizeNumber.cx,SizeNumber.cy,SizeNumber.cx*10L,0L);
			}

			//�滭���
			if (szControl[i]==TEXT('.'))
			{
				ImageNumber.DrawImage(pDC,nXDrawPos,nYPos,SizeNumber.cx,SizeNumber.cy,SizeNumber.cx*11L,0L);
			}

			//�滭����
			if (szControl[i]>=TEXT('0')&&szControl[i]<=TEXT('9'))
			{
				ImageNumber.DrawImage(pDC,nXDrawPos,nYPos,SizeNumber.cx,SizeNumber.cy,SizeNumber.cx*(szControl[i]-TEXT('0')),0L);
			}

			//����λ��
			nXDrawPos+=SizeNumber.cx;
		}
	}

	return;
}

//ת���ַ�
VOID CDlgInsureItem::SwitchScoreString(SCORE lScore, LPTSTR pszBuffer, WORD wBufferSize)
{
	//��������
	LPCTSTR pszNumber[]={TEXT("��"),TEXT("Ҽ"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("½"),TEXT("��"),TEXT("��"),TEXT("��")};
	LPCTSTR pszWeiName[]={TEXT("ʰ"),TEXT("��"),TEXT("Ǫ"),TEXT("��"),TEXT("ʰ"),TEXT("��"),TEXT("Ǫ"),TEXT("��"),TEXT("ʰ"),TEXT("��"),TEXT("Ǫ"),TEXT("��")};

	//ת����ֵ
	TCHAR szSwitchScore[16]=TEXT("");
	_sntprintf(szSwitchScore,CountArray(szSwitchScore),TEXT("%I64d"),lScore);

	//��������
	bool bNeedFill=false;
	bool bNeedZero=false;
	UINT uTargetIndex=0,uSourceIndex=0;

	//�ַ�����
	UINT uItemLength=lstrlen(pszNumber[0]);
	UINT uSwitchLength=lstrlen(szSwitchScore);

	//ת������
	for (UINT i=0;i<uSwitchLength;i++)
	{
		//��������
		WORD wNumberIndex=szSwitchScore[i]-TEXT('0');

		//�������
		if ((bNeedZero==true)&&(wNumberIndex!=0L))
		{
			bNeedZero=false;
			_tcsncat(pszBuffer,pszNumber[0],wBufferSize-lstrlen(pszBuffer)-1);
		}

		//��������
		if (wNumberIndex!=0L)
		{
			_tcsncat(pszBuffer,pszNumber[wNumberIndex],wBufferSize-lstrlen(pszBuffer)-1);
		}

		//����λ��
		if ((wNumberIndex!=0L)&&((uSwitchLength-i)>=2))
		{
			bNeedZero=false;
			_tcsncat(pszBuffer,pszWeiName[uSwitchLength-i-2],wBufferSize-lstrlen(pszBuffer)-1);
		}

		//�����ж�
		if ((bNeedZero==false)&&(wNumberIndex==0))
		{
			bNeedZero=true;
		}

		//��λ�ж�
		if ((bNeedFill==false)&&(wNumberIndex!=0))
		{
			bNeedFill=true;
		}

		//�λ��
		if (((uSwitchLength-i)==5)||((uSwitchLength-i)==9))
		{
			//����λ��
			if ((bNeedFill==true)&&(wNumberIndex==0L))
			{
				_tcsncat(pszBuffer,pszWeiName[uSwitchLength-i-2],wBufferSize-lstrlen(pszBuffer)-1);
			}

			//���ñ���
			bNeedZero=false;
			bNeedFill=false;
		}
	}

	return;
}

//ת���ַ�
VOID CDlgInsureItem::SwitchScoreFormat(SCORE lScore, UINT uSpace, LPTSTR pszBuffer, WORD wBufferSize)
{
	//ת����ֵ
	TCHAR szSwitchScore[16]=TEXT("");
	_sntprintf(szSwitchScore,CountArray(szSwitchScore),TEXT("%I64d"),lScore);

	//��������
	WORD wTargetIndex=0;
	WORD wSourceIndex=0;
	UINT uSwitchLength=lstrlen(szSwitchScore);

	//ת���ַ�
	while (szSwitchScore[wSourceIndex]!=0)
	{
		//�����ַ�
		pszBuffer[wTargetIndex++]=szSwitchScore[wSourceIndex++];

		//���붺��
		if ((uSwitchLength!=wSourceIndex)&&(((uSwitchLength-wSourceIndex)%uSpace)==0L))
		{
			pszBuffer[wTargetIndex++]=TEXT(',');
		}
	}

	//�����ַ�
	pszBuffer[wTargetIndex++]=0;

	return;
}

//������Ϣ
VOID CDlgInsureItem::OnEnChangeScore()
{
	//��������
	SCORE lScore=GetUserControlScore();

	//���ý���
	if (lScore>0L)
	{
		//ת����ֵ
		TCHAR szString[128]=TEXT("");
		SwitchScoreString(lScore,szString,CountArray(szString));

		//ת������
		TCHAR szControl[128]=TEXT("");
		SwitchScoreFormat(lScore,3L,szControl,CountArray(szControl));

		//���ÿؼ�
		SetDlgItemText(IDC_SCORE,szControl);
		SetDlgItemText(IDC_SCORE_STRING,szString);

		//���ù��
		INT nScoreLength=m_edScore.GetWindowTextLength();
		m_edScore.SetSel(nScoreLength,nScoreLength,FALSE);
	}
	else
	{
		//���ÿؼ�
		SetDlgItemText(IDC_SCORE,TEXT(""));
		SetDlgItemText(IDC_SCORE_STRING,TEXT(""));
	}

	return;
}

//��ѯ��ť
VOID CDlgInsureItem::OnBnClickedQueryInfo()
{
	//���ý���
	m_edScore.SetFocus();

	//��ѯ��Ϣ
	QueryUserInsureInfo();

	return;
}

//��ť
VOID CDlgInsureItem::OnBnClickedSaveScore()
{
	//��ȡ����
	SCORE lSaveScore=GetUserControlScore();

	//�����ж�
	if (lSaveScore<=0L)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("�������Ϸ����������Ϊ�գ�������������Ϸ��������"),MB_ICONERROR,30);

		//���ý���
		m_edScore.SetFocus();

		return;
	}

	//��Ŀ�ж�
	if (lSaveScore>m_UserInsureInfo.lUserScore)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("����Я����Ϸ�ҵ���Ŀ���㣬������������Ϸ��������"),MB_ICONERROR,30);

		//���ý���
		m_edScore.SetFocus();

		return;
	}

	//�¼�����
	OnMissionStart();

	//���ñ���
	m_bSaveMission=true;
	m_bTakeMission=false;
	m_bQueryMission=false;

	//ִ�д��
	PerformSaveScore(lSaveScore);

	return;
}

//ȡ�ť
VOID CDlgInsureItem::OnBnClickedTakeScore()
{
	//��ȡ����
	SCORE lTakeScore=GetUserControlScore();

	//��ȡ����
	TCHAR szPassword[LEN_PASSWORD];
	m_PasswordControl.GetUserPassword(szPassword);

	//��������
	TCHAR szInsurePass[LEN_MD5];
	CWHEncrypt::MD5Encrypt(szPassword,szInsurePass);

	//�����ж�
	if (lTakeScore<=0L)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("ȡ������Ϸ����������Ϊ�գ�������������Ϸ��������"),MB_ICONERROR,30);

		//���ý���
		m_edScore.SetFocus();

		return;
	}

	//��Ŀ�ж�
	if (lTakeScore>m_UserInsureInfo.lUserInsure)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("�����չ���Ϸ�ҵ���Ŀ���㣬������������Ϸ��������"),MB_ICONERROR,30);

		//���ý���
		m_edScore.SetFocus();

		return;
	}

	//�����ж�
	if (szPassword[0]==0)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("���չ����벻��Ϊ�գ����������뱣�չ����룡"),MB_ICONERROR,30);

		//���ý���
		m_PasswordControl.SetFocus();

		return;
	}

	//�¼�����
	OnMissionStart();

	//���ñ���
	m_bTakeMission=true;
	m_bSaveMission=false;
	m_bQueryMission=false;

	//�¼�֪ͨ
	PerformTakeScore(lTakeScore,szInsurePass);

	return;
}

//��������
VOID CDlgInsureItem::OnBnClickedForgetInsure()
{
	//�����ַ
	TCHAR szLogonLink[256]=TEXT("");
	_sntprintf(szLogonLink,CountArray(szLogonLink),TEXT("%s/LogonLink4.asp"),szPlatformLink);

	//��ҳ��
	ShellExecute(NULL,TEXT("OPEN"),szLogonLink,NULL,NULL,SW_NORMAL);

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgInsurePlaza::CDlgInsurePlaza()
{
	//��������
	m_MissionManager.InsertMissionItem(this);

	//���ñ���
	m_lScore=0L;
	ZeroMemory(m_szInsurePass,sizeof(m_szInsurePass));

	return;
}

//��������
CDlgInsurePlaza::~CDlgInsurePlaza()
{
}

//��ѯ�¼�
VOID CDlgInsurePlaza::PerformQueryInfo()
{
	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		OnMissionConclude();
		return;
	}

	return;
}

//�����¼�
VOID CDlgInsurePlaza::PerformSaveScore(SCORE lSaveScore)
{
	//���ñ���
	m_lScore=lSaveScore;

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		OnMissionConclude();
		return;
	}

	return;
}

//ȡ���¼�
VOID CDlgInsurePlaza::PerformTakeScore(SCORE lTakeScore, LPCTSTR pszInsurePass)
{
	//���ñ���
	m_lScore=lTakeScore;
	lstrcpyn(m_szInsurePass,pszInsurePass,CountArray(m_szInsurePass));

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		OnMissionConclude();
		return;
	}

	return;
}

//�����¼�
bool CDlgInsurePlaza::OnEventMissionLink(INT nErrorCode)
{
	//���ӽ��
	if (nErrorCode!=0)
	{
		//��������
		if (m_MissionManager.AvtiveMissionItem(this,true)==true)
		{
			return true;
		}

		//�¼�����
		OnMissionConclude();

		//������ʾ
		if (m_bSaveMission==true)
		{
			//��ʾ��Ϣ
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("���ӷ�������ʱ����Ϸ�Ҵ���ʧ�ܣ����Ժ��ٴ����ԣ�"),MB_OK|MB_ICONSTOP,30);
		}

		//������ʾ
		if (m_bTakeMission==true)
		{
			//��ʾ��Ϣ
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("���ӷ�������ʱ����Ϸ��ȡ��ʧ�ܣ����Ժ��ٴ����ԣ�"),MB_OK|MB_ICONSTOP,30);
		}

		//������ʾ
		if (m_bQueryMission==true)
		{
			//��ʾ��Ϣ
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("���ӷ�������ʱ�����չ����ϻ�ȡʧ�ܣ�"),MB_OK|MB_ICONSTOP,30);
		}

		return true;
	}

	//��������
	if (m_bSaveMission==true)
	{
		//��������
		CMD_GP_UserSaveScore UserSaveScore;
		ZeroMemory(&UserSaveScore,sizeof(UserSaveScore));

		//��������
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//���ñ���
		UserSaveScore.lSaveScore=m_lScore;
		UserSaveScore.dwUserID=pGlobalUserData->dwUserID;
		CWHService::GetMachineID(UserSaveScore.szMachineID);

		//��������
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_USER_SAVE_SCORE,&UserSaveScore,sizeof(UserSaveScore));

		return true;
	}

	//ȡ������
	if (m_bTakeMission==true)
	{
		//��������
		CMD_GP_UserTakeScore UserTakeScore;
		ZeroMemory(&UserTakeScore,sizeof(UserTakeScore));

		//��������
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//���ñ���
		UserTakeScore.lTakeScore=m_lScore;
		UserTakeScore.dwUserID=pGlobalUserData->dwUserID;
		CWHService::GetMachineID(UserTakeScore.szMachineID);
		lstrcpyn(UserTakeScore.szPassword,m_szInsurePass,CountArray(UserTakeScore.szPassword));

		//��������
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_USER_TAKE_SCORE,&UserTakeScore,sizeof(UserTakeScore));

		return true;
	}

	//��ѯ����
	if (m_bQueryMission==true)
	{
		//��������
		CMD_GP_QueryInsureInfo QueryInsureInfo;
		ZeroMemory(&QueryInsureInfo,sizeof(QueryInsureInfo));

		//���ñ���
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		QueryInsureInfo.dwUserID=pGlobalUserInfo->GetGlobalUserData()->dwUserID;

		//��������
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_QUERY_INSURE_INFO,&QueryInsureInfo,sizeof(QueryInsureInfo));
	}

	return true;
}

//�ر��¼�
bool CDlgInsurePlaza::OnEventMissionShut(BYTE cbShutReason)
{
	//��ʾ��Ϣ
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		//��������
		if (m_MissionManager.AvtiveMissionItem(this,true)==true)
		{
			return true;
		}

		//��������
		if (m_bSaveMission==true)
		{
			//��ʾ��Ϣ
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("��������������쳣�Ͽ��ˣ���Ϸ�Ҵ���ʧ�ܣ�"),MB_ICONERROR,30);
		}

		//ȡ������
		if (m_bTakeMission==true)
		{
			//��ʾ��Ϣ
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("��������������쳣�Ͽ��ˣ���Ϸ��ȡ��ʧ�ܣ�"),MB_ICONERROR,30);
		}

		//��ѯ����
		if (m_bQueryMission==true)
		{
			//��ʾ��Ϣ
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("��������������쳣�Ͽ��ˣ����չ����ϻ�ȡʧ�ܣ�"),MB_ICONERROR,30);
		}
	}

	//�¼�����
	OnMissionConclude();

	return true;
}

//��ȡ�¼�
bool CDlgInsurePlaza::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//�����
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_USER_INSURE_INFO:		//��������
			{
				//Ч�����
				ASSERT(wDataSize==sizeof(CMD_GP_UserInsureInfo));
				if (wDataSize<sizeof(CMD_GP_UserInsureInfo)) return false;

				//��������
				CMD_GP_UserInsureInfo * pUserInsureInfo=(CMD_GP_UserInsureInfo *)pData;

				//���ñ���
				m_bInitInfo=true;
				m_UserInsureInfo.wServerID=pUserInsureInfo->wServerID;
				m_UserInsureInfo.lUserScore=pUserInsureInfo->lUserScore;
				m_UserInsureInfo.lUserInsure=pUserInsureInfo->lUserInsure;

				//�ر�����
				m_MissionManager.ConcludeMissionItem(this,false);

				//���½���
				RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

				return true;
			}
		case SUB_GP_USER_INSURE_SUCCESS:	//���гɹ�
			{
				//Ч�����
				CMD_GP_UserInsureSuccess * pUserInsureSuccess=(CMD_GP_UserInsureSuccess *)pData;
				ASSERT(wDataSize>=(sizeof(CMD_GP_UserInsureSuccess)-sizeof(pUserInsureSuccess->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GP_UserInsureSuccess)-sizeof(pUserInsureSuccess->szDescribeString))) return false;

				//���ñ���
				m_UserInsureInfo.lUserScore=pUserInsureSuccess->lUserScore;
				m_UserInsureInfo.lUserInsure=pUserInsureSuccess->lUserInsure;

				//����ؼ�
				m_PasswordControl.SetFocus();
				m_PasswordControl.SetUserPassword(TEXT(""));

				//�ر�����
				m_MissionManager.ConcludeMissionItem(this,false);

				//���½���
				RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

				//��ʾ��Ϣ
				if (pUserInsureSuccess->szDescribeString[0]!=0)
				{
					CInformation Information(this);
					Information.ShowMessageBox(pUserInsureSuccess->szDescribeString,MB_ICONINFORMATION,60);
				}

				return true;
			}
		case SUB_GP_USER_INSURE_FAILURE:	//����ʧ��
			{
				//Ч�����
				CMD_GP_UserInsureFailure * pUserInsureFailure=(CMD_GP_UserInsureFailure *)pData;
				ASSERT(wDataSize>=(sizeof(CMD_GP_UserInsureFailure)-sizeof(pUserInsureFailure->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GP_UserInsureFailure)-sizeof(pUserInsureFailure->szDescribeString))) return false;

				//ʧ�ܴ���
				OnMissionConclude();

				//�ر�����
				m_MissionManager.ConcludeMissionItem(this,false);

				//����ؼ�
				m_PasswordControl.SetFocus();
				m_PasswordControl.SetUserPassword(TEXT(""));

				//��ʾ��Ϣ
				if (pUserInsureFailure->szDescribeString[0]!=0)
				{
					CInformation Information(this);
					Information.ShowMessageBox(pUserInsureFailure->szDescribeString,MB_ICONERROR,60);
				}

				return true;
			}
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgInsureServer::CDlgInsureServer()
{
	//���ñ���
	m_pITCPSocket=NULL;
	m_pIMySelfUserItem=NULL;

	return;
}

//��������
CDlgInsureServer::~CDlgInsureServer()
{
}

//��ѯ�¼�
VOID CDlgInsureServer::PerformQueryInfo()
{
	//Ч�����
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//��������
	m_pITCPSocket->SendData(MDM_GR_INSURE,SUB_GR_QUERY_INSURE_INFO);

	return;
}

//�����¼�
VOID CDlgInsureServer::PerformSaveScore(SCORE lSaveScore)
{
	//Ч�����
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//��������
	CMD_GR_C_SaveScoreRequest SaveScoreRequest;
	ZeroMemory(&SaveScoreRequest,sizeof(SaveScoreRequest));

	//���ñ���
	SaveScoreRequest.lSaveScore=lSaveScore;

	//��������
	m_pITCPSocket->SendData(MDM_GR_INSURE,SUB_GR_SAVE_SCORE_REQUEST,&SaveScoreRequest,sizeof(SaveScoreRequest));

	return;
}

//ȡ���¼�
VOID CDlgInsureServer::PerformTakeScore(SCORE lTakeScore, LPCTSTR pszInsurePass)
{
	//Ч�����
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//��������
	CMD_GR_C_TakeScoreRequest TakeScoreRequest;
	ZeroMemory(&TakeScoreRequest,sizeof(TakeScoreRequest));

	//���ñ���
	TakeScoreRequest.lTakeScore=lTakeScore;
	lstrcpyn(TakeScoreRequest.szInsurePass,pszInsurePass,CountArray(TakeScoreRequest.szInsurePass));

	//��������
	m_pITCPSocket->SendData(MDM_GR_INSURE,SUB_GR_TAKE_SCORE_REQUEST,&TakeScoreRequest,sizeof(TakeScoreRequest));

	return;
}

//��������
VOID CDlgInsureServer::Initialization(ITCPSocket * pITCPSocket, IClientUserItem * pIMySelfUserItem)
{
	//���ñ���
	m_pITCPSocket=pITCPSocket;
	m_pIMySelfUserItem=pIMySelfUserItem;

	return;
}

//������Ϣ
bool CDlgInsureServer::OnServerInsureMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//��Ϣ�ж�
	if (wMainCmdID==MDM_GR_INSURE)
	{
		switch (wSubCmdID)
		{
		case SUB_GR_USER_INSURE_INFO:		//��������
			{
				//Ч�����
				ASSERT(wDataSize==sizeof(CMD_GR_S_UserInsureInfo));
				if (wDataSize<sizeof(CMD_GR_S_UserInsureInfo)) return false;

				//��������
				CMD_GR_S_UserInsureInfo * pUserInsureInfo=(CMD_GR_S_UserInsureInfo *)pData;

				//���ñ���
				m_bInitInfo=true;
				m_UserInsureInfo.wServerID=pUserInsureInfo->wServerID;
				m_UserInsureInfo.lUserScore=pUserInsureInfo->lUserScore;
				m_UserInsureInfo.lUserInsure=pUserInsureInfo->lUserInsure;

				//���½���
				RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

				return true;
			}
		case SUB_GR_USER_INSURE_SUCCESS:	//���гɹ�
			{
				//Ч�����
				CMD_GR_S_UserInsureSuccess * pUserInsureSuccess=(CMD_GR_S_UserInsureSuccess *)pData;
				ASSERT(wDataSize>=(sizeof(CMD_GR_S_UserInsureSuccess)-sizeof(pUserInsureSuccess->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GR_S_UserInsureSuccess)-sizeof(pUserInsureSuccess->szDescribeString))) return false;

				//���ñ���
				m_UserInsureInfo.lUserScore=pUserInsureSuccess->lUserScore;
				m_UserInsureInfo.lUserInsure=pUserInsureSuccess->lUserInsure;

				//����ؼ�
				m_PasswordControl.SetFocus();
				m_PasswordControl.SetUserPassword(TEXT(""));

				//���½���
				RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

				//��ʾ��Ϣ
				if (pUserInsureSuccess->szDescribeString[0]!=0)
				{
					CInformation Information(this);
					Information.ShowMessageBox(pUserInsureSuccess->szDescribeString,MB_ICONINFORMATION,60);
				}

				return true;
			}
		case SUB_GR_USER_INSURE_FAILURE:	//����ʧ��
			{
				//Ч�����
				CMD_GR_S_UserInsureFailure * pUserInsureFailure=(CMD_GR_S_UserInsureFailure *)pData;
				ASSERT(wDataSize>=(sizeof(CMD_GP_UserInsureFailure)-sizeof(pUserInsureFailure->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GP_UserInsureFailure)-sizeof(pUserInsureFailure->szDescribeString))) return false;

				//ʧ�ܴ���
				OnMissionConclude();

				//����ؼ�
				if (m_bInitInfo==true)
				{
					m_PasswordControl.SetFocus();
					m_PasswordControl.SetUserPassword(TEXT(""));
				}

				//��ʾ��Ϣ
				if (pUserInsureFailure->szDescribeString[0]!=0)
				{
					CInformation Information(this);
					Information.ShowMessageBox(pUserInsureFailure->szDescribeString,MB_ICONERROR,60);
				}

				return true;
			}
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
