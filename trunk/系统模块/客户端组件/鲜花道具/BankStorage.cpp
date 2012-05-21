#include "Stdafx.h"
#include "resource.h"
#include "BankStorage.h"
#include ".\bankstorage.h"

//////////////////////////////////////////////////////////////////////////

//��ʱ��I D
#define IDI_CHARMVALUE_UPDATE_VIEW		1								//���½���
#define TIME_CHARMVALUE_UPDATE_VIEW		200								//���½���

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CBankStorage, CDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()	
	ON_BN_CLICKED(IDOK,OnBnClickedOk)
	ON_EN_CHANGE(IDC_IN_COUNT, OnEnChangeInCount)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CBankDrawing, CDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()	
	ON_BN_CLICKED(IDOK,OnBnClickedOk)
	ON_EN_CHANGE(IDC_IN_COUNT, OnEnChangeInCount)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CBankStorage::CBankStorage(CWnd* pParent):CDialog(IDD_BANK_STORAGE, pParent)
{
	m_lInCount=0;
	m_lGameGold=0;
	m_lStorageGold=0;
	m_pMeUserData=NULL;
	m_pIClientKernel=NULL;
	m_pClientSocket=NULL;
}

//��������
CBankStorage::~CBankStorage()
{
}

//�ؼ���
void CBankStorage::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Text(pDX, IDC_GAME_GOLD, m_strGameGold);
	DDX_Text(pDX, IDC_STORAGE_GOLD, m_strStorageGold);
}

//��ȡ��ť
void CBankStorage::OnBnClickedOk()
{
	UpdateData(TRUE);

	//������֤
	if (IsButtonSelected(IDC_DRAWOUT) && (m_lInCount <= 0 || m_lInCount > m_lStorageGold ))
	{
		CString strMessage;
		if(m_lStorageGold>0)strMessage.Format(TEXT("���������Ϸ��ֵ������1��%ld֮��"), m_lStorageGold);
		else strMessage.Format(TEXT("��Ĵ洢��Ϸ����ĿΪ0,���ܽ�����ȡ����!"));
		ShowInformationEx(strMessage,0 ,MB_ICONINFORMATION,TEXT("ϵͳ��Ϣ")) ;

		GetDlgItem(IDC_IN_COUNT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(0,-1);
		return;
	}

	//������֤
	if (IsButtonSelected(IDC_STORAGE) && (m_lInCount <= 0 || m_lInCount > m_lGameGold ))
	{
		CString strMessage;
		if(m_lGameGold>0)strMessage.Format(TEXT("���������Ϸ��ֵ������1��%ld֮��"),m_lGameGold );
		else strMessage.Format(TEXT("��ĵ�ǰ��Ϸ����ĿΪ0,���ܽ��д洢����!"));
		ShowInformationEx(strMessage,0 ,MB_ICONINFORMATION,TEXT("ϵͳ��Ϣ")) ;

		GetDlgItem(IDC_IN_COUNT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(0,-1);
		return;
	}

	//������֤
	// modify by һ���⺮
	TCHAR szPassword[PASS_LEN]=TEXT("");
	//GetDlgItemText(IDC_USER_PASSWORD,szPassword,CountArray(szPassword));
	GetDlgItemText(IDC_USER_PASSWORD, szPassword, PASS_LEN);
	//strcpy(szPassword, "111111");
	if (szPassword[0]==0)
	{
		ShowInformationEx(TEXT("���벻��Ϊ�գ��������������룡"),0 ,MB_ICONINFORMATION,TEXT("ϵͳ��Ϣ")) ;
		GetDlgItem(IDC_USER_PASSWORD)->SetWindowText("");
		GetDlgItem(IDC_USER_PASSWORD)->SetFocus();
		return;
	}
	TCHAR szTempPassword[PASS_LEN]=TEXT("");
	CopyMemory(szTempPassword,szPassword,sizeof(szTempPassword));
	CMD5Encrypt::EncryptData(szTempPassword,szPassword);

	if (IsButtonSelected(IDC_DRAWOUT))
	{
		//������Ϣ
		CMD_GF_BankGet BankGetGold;
		ZeroMemory(&BankGetGold, sizeof(BankGetGold));
		BankGetGold.lGetValue = m_lInCount;
		CopyMemory(BankGetGold.szPassword,szPassword,sizeof(BankGetGold.szPassword));

		SendData(MDM_GF_BANK, SUB_GF_BANK_GET, &BankGetGold, sizeof(BankGetGold));
	}
	else
	{
		//������Ϣ
		CMD_GF_BankStorage BankStorage;
		ZeroMemory(&BankStorage, sizeof(BankStorage));
		BankStorage.lStorageValue = m_lInCount;
		CopyMemory(BankStorage.szPassword,szPassword,sizeof(BankStorage.szPassword));

		SendData(MDM_GF_BANK, SUB_GF_BANK_STORAGE, &BankStorage, sizeof(BankStorage));
	}

	SetDlgItemText(IDC_IN_COUNT,TEXT(""));

	return;
}

//��ʼ������
BOOL CBankStorage::OnInitDialog()
{
	__super::OnInitDialog();

	UpdateData(FALSE);

	SetWindowText(TEXT("����"));

	//������ֵ
	SetButtonSelected(IDC_STORAGE,true);

	//��ʱ����
	SetTimer(IDI_CHARMVALUE_UPDATE_VIEW,TIME_CHARMVALUE_UPDATE_VIEW,NULL);

	CSkinWndAttribute * pSkinAttribute=CSkinDialog::GetSkinAttribute();
	if (NULL==m_brBakcground.m_hObject) m_brBakcground.CreateSolidBrush(pSkinAttribute->m_crBackGround);

	return TRUE;
}

//���½���
void CBankStorage::UpdateView()
{
	CString strinCount;
	GetDlgItemText(IDC_IN_COUNT,strinCount);
	LONG OutNum = 0;
	MakeStringToNum(strinCount,OutNum);
	m_lInCount=OutNum;
	if(OutNum!=0)
	{
		MakeString(strinCount,OutNum);
		SetDlgItemText(IDC_IN_COUNT,strinCount);
		((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(strinCount.GetLength(),strinCount.GetLength(),TRUE); 
	}
	UpdateData(TRUE);

	if(m_pMeUserData==NULL)return;

	//������Ϣ
	m_lGameGold=((m_pMeUserData->lGameGold<0)?0:m_pMeUserData->lGameGold);
	m_lStorageGold=m_pMeUserData->lInsureScore;

	MakeString(m_strGameGold,m_lGameGold);
	MakeString(m_strStorageGold,m_lStorageGold);

	UpdateData(FALSE);
}

//�����Ϣ
void CBankStorage::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonDown(nFlags, point);
}

//�����Ϣ
void CBankStorage::OnMouseMove(UINT nFlags, CPoint point)
{
	__super::OnMouseMove(nFlags, point);
}

//������Ϣ
BOOL CBankStorage::PreTranslateMessage(MSG* pMsg)
{
	//������ĸ
	if(GetFocus()==GetDlgItem(IDC_IN_COUNT)&& pMsg->message==WM_CHAR)
	{
		BYTE bMesValue = (BYTE)(pMsg->wParam);
		BYTE bTemp = bMesValue-'0';
		if((bTemp<0 || bTemp>9) && bMesValue!=VK_BACK) return TRUE;
	}

	////���˻س�
	//if(pMsg->message==WM_KEYDOWN)
	//{
	//	BYTE bMesValue = (BYTE)(pMsg->wParam);
	//	if(bMesValue==VK_RETURN) return TRUE;
	//}

	return __super::PreTranslateMessage(pMsg);
}

//ʱ����Ϣ
void CBankStorage::OnTimer(UINT nIDEvent)
{
	//ʱ����Ϣ
	switch(nIDEvent)
	{
	case IDI_CHARMVALUE_UPDATE_VIEW:		//���½���
		{
			//���½���
			UpdateView();

			return;
		}
	}

	__super::OnTimer(nIDEvent);
}

//������Ϣ
void CBankStorage::OnEnChangeInCount()
{
	//CString strCount;
	//GetDlgItem(IDC_IN_COUNT)->GetWindowText(strCount);
	//m_lInCount=_tstoi64(strCount);

	return ;
}

//�Ƿ�ѡ��
bool CBankStorage::IsButtonSelected(UINT uButtonID)
{
	return ((CButton *)GetDlgItem(uButtonID))->GetCheck()==BST_CHECKED;
}

//ѡ�а�ť
void CBankStorage::SetButtonSelected(UINT uButtonID, bool bSelected)
{
	CButton * pButton=(CButton *)GetDlgItem(uButtonID);
	if (bSelected) pButton->SetCheck(BST_CHECKED);
	else pButton->SetCheck(BST_UNCHECKED);
	return;
}

//������Ϣ
void CBankStorage::SetSendInfo(IClientKernel *pClientKernel,ITCPSocket *pClientSocket,tagUserData *pMeUserData)
{
	ASSERT((pClientKernel==NULL && pClientSocket!=NULL)||(pClientKernel!=NULL && pClientSocket==NULL));

	//������Ϣ
	m_pIClientKernel = pClientKernel;
	m_pClientSocket= pClientSocket;
	m_pMeUserData= pMeUserData;

	return;
}

//������Ϣ
void CBankStorage::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	ASSERT((m_pIClientKernel==NULL && m_pClientSocket!=NULL)||(m_pIClientKernel!=NULL && m_pClientSocket==NULL));

	//������Ϣ
	if(m_pClientSocket!=NULL)m_pClientSocket->SendData(wMainCmdID, wSubCmdID, pBuffer, wDataSize);
	else m_pIClientKernel->SendSocketData(wMainCmdID, wSubCmdID, pBuffer, wDataSize);

	return ;
}
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////


void CBankStorage::OnPaint()
{
	CPaintDC dc(this); 

	//�滭����
	CRect ClipRect,ClientRect;
	dc.GetClipBox(&ClipRect);
	CSkinWndAttribute * pSkinAttribute=CSkinDialog::GetSkinAttribute();
	dc.FillSolidRect(&ClipRect,pSkinAttribute->m_crBackGround);
}

HBRUSH CBankStorage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (CTLCOLOR_STATIC==nCtlColor)
	{
		pDC->SetBkMode(TRANSPARENT);
		return m_brBakcground;
	}
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	return hbr;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//���캯��
CBankDrawing::CBankDrawing(CWnd* pParent):CDialog(IDD_BANK_DRAWING, pParent)
{
	m_lInCount=0;
	m_lGameGold=0;
	m_lStorageGold=0;
	m_pMeUserData=NULL;
	m_pIClientKernel=NULL;
	m_pClientSocket=NULL;
}

//��������
CBankDrawing::~CBankDrawing()
{
}

//�ؼ���
void CBankDrawing::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Text(pDX, IDC_GAME_GOLD, m_strGameGold);
	DDX_Text(pDX, IDC_STORAGE_GOLD, m_strStorageGold);
}

//��ȡ��ť
void CBankDrawing::OnBnClickedOk()
{
	UpdateData(TRUE);

	//������֤
	if (IsButtonSelected(IDC_DRAWOUT) && (m_lInCount <= 0 || m_lInCount > m_lStorageGold ))
	{
		CString strMessage;
		if(m_lStorageGold>0)strMessage.Format(TEXT("���������Ϸ��ֵ������1��%ld֮��"), m_lStorageGold);
		else strMessage.Format(TEXT("��Ĵ洢��Ϸ����ĿΪ0,���ܽ�����ȡ����!"));
		ShowInformationEx(strMessage,0 ,MB_ICONINFORMATION,TEXT("ϵͳ��Ϣ")) ;

		GetDlgItem(IDC_IN_COUNT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(0,-1);
		return;
	}

	//������֤
	if (IsButtonSelected(IDC_STORAGE) && (m_lInCount <= 0 || m_lInCount > m_lGameGold ))
	{
		CString strMessage;
		if(m_lGameGold>0)strMessage.Format(TEXT("���������Ϸ��ֵ������1��%ld֮��"),m_lGameGold );
		else strMessage.Format(TEXT("��ĵ�ǰ��Ϸ����ĿΪ0,���ܽ��д洢����!"));
		ShowInformationEx(strMessage,0 ,MB_ICONINFORMATION,TEXT("ϵͳ��Ϣ")) ;

		GetDlgItem(IDC_IN_COUNT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(0,-1);
		return;
	}

	//������֤
	TCHAR szPassword[PASS_LEN]=TEXT("");
	GetDlgItemText(IDC_USER_PASSWORD,szPassword,CountArray(szPassword));
	if (szPassword[0]==0)
	{
		ShowInformationEx(TEXT("���벻��Ϊ�գ��������������룡"),0 ,MB_ICONINFORMATION,TEXT("ϵͳ��Ϣ")) ;
		GetDlgItem(IDC_USER_PASSWORD)->SetWindowText("");
		GetDlgItem(IDC_USER_PASSWORD)->SetFocus();
		return;
	}
	TCHAR szTempPassword[PASS_LEN]=TEXT("");
	CopyMemory(szTempPassword,szPassword,sizeof(szTempPassword));
	CMD5Encrypt::EncryptData(szTempPassword,szPassword);

	if (IsButtonSelected(IDC_DRAWOUT))
	{
		//������Ϣ
		CMD_GF_BankGet BankGetGold;
		ZeroMemory(&BankGetGold, sizeof(BankGetGold));
		BankGetGold.lGetValue = m_lInCount;
		CopyMemory(BankGetGold.szPassword,szPassword,sizeof(BankGetGold.szPassword));

		SendData(MDM_GF_BANK, SUB_GF_BANK_GET, &BankGetGold, sizeof(BankGetGold));
	}
	else
	{
		//������Ϣ
		CMD_GF_BankStorage BankStorage;
		ZeroMemory(&BankStorage, sizeof(BankStorage));
		BankStorage.lStorageValue = m_lInCount;
//		CopyMemory(BankStorage.szPassword,szPassword,sizeof(BankStorage.szPassword));

		SendData(MDM_GF_BANK, SUB_GF_BANK_STORAGE, &BankStorage, sizeof(BankStorage));
	}

	SetDlgItemText(IDC_USER_PASSWORD,TEXT(""));
	SetDlgItemText(IDC_IN_COUNT,TEXT(""));

	return;
}

//��ʼ������
BOOL CBankDrawing::OnInitDialog()
{
	__super::OnInitDialog();

	UpdateData(FALSE);

	SetWindowText(TEXT("����"));

	//������ֵ
	SetButtonSelected(IDC_DRAWOUT,true);

	//��ʱ����
	SetTimer(IDI_CHARMVALUE_UPDATE_VIEW,TIME_CHARMVALUE_UPDATE_VIEW,NULL);

	CSkinWndAttribute * pSkinAttribute=CSkinDialog::GetSkinAttribute();
	if (NULL==m_brBakcground.m_hObject) m_brBakcground.CreateSolidBrush(pSkinAttribute->m_crBackGround);

	return TRUE;
}

//���½���
void CBankDrawing::UpdateView()
{
	CString strinCount;
	GetDlgItemText(IDC_IN_COUNT,strinCount);
	LONG OutNum = 0;
	MakeStringToNum(strinCount,OutNum);
	m_lInCount=OutNum;
	if(OutNum!=0)
	{
		MakeString(strinCount,OutNum);
		SetDlgItemText(IDC_IN_COUNT,strinCount);
		((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(strinCount.GetLength(),strinCount.GetLength(),TRUE); 
	}

	UpdateData(TRUE);

	if(m_pMeUserData==NULL)return;

	//������Ϣ
	m_lGameGold=((m_pMeUserData->lGameGold<0)?0:m_pMeUserData->lGameGold);
	m_lStorageGold=m_pMeUserData->lInsureScore;

	MakeString(m_strGameGold,m_lGameGold);
	MakeString(m_strStorageGold,m_lStorageGold);

	UpdateData(FALSE);
}

//�����Ϣ
void CBankDrawing::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonDown(nFlags, point);
}

//�����Ϣ
void CBankDrawing::OnMouseMove(UINT nFlags, CPoint point)
{
	__super::OnMouseMove(nFlags, point);
}

//������Ϣ
BOOL CBankDrawing::PreTranslateMessage(MSG* pMsg)
{
	//������ĸ
	if(GetFocus()==GetDlgItem(IDC_IN_COUNT)&& pMsg->message==WM_CHAR)
	{
		BYTE bMesValue = (BYTE)(pMsg->wParam);
		BYTE bTemp = bMesValue-'0';
		if((bTemp<0 || bTemp>9) && bMesValue!=VK_BACK) return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//ʱ����Ϣ
void CBankDrawing::OnTimer(UINT nIDEvent)
{
	//ʱ����Ϣ
	switch(nIDEvent)
	{
	case IDI_CHARMVALUE_UPDATE_VIEW:		//���½���
		{
			//���½���
			UpdateView();

			return;
		}
	}

	__super::OnTimer(nIDEvent);
}

//������Ϣ
void CBankDrawing::OnEnChangeInCount()
{
	//CString strCount;
	//GetDlgItem(IDC_IN_COUNT)->GetWindowText(strCount);
	//m_lInCount=_tstoi64(strCount);

	return ;
}

//�Ƿ�ѡ��
bool CBankDrawing::IsButtonSelected(UINT uButtonID)
{
	return ((CButton *)GetDlgItem(uButtonID))->GetCheck()==BST_CHECKED;
}

//ѡ�а�ť
void CBankDrawing::SetButtonSelected(UINT uButtonID, bool bSelected)
{
	CButton * pButton=(CButton *)GetDlgItem(uButtonID);
	if (bSelected) pButton->SetCheck(BST_CHECKED);
	else pButton->SetCheck(BST_UNCHECKED);
	return;
}

//������Ϣ
void CBankDrawing::SetSendInfo(IClientKernel *pClientKernel,ITCPSocket *pClientSocket,tagUserData *pMeUserData)
{
	ASSERT((pClientKernel==NULL && pClientSocket!=NULL)||(pClientKernel!=NULL && pClientSocket==NULL));

	//������Ϣ
	m_pIClientKernel = pClientKernel;
	m_pClientSocket= pClientSocket;
	m_pMeUserData= pMeUserData;

	return;
}

//������Ϣ
void CBankDrawing::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	ASSERT((m_pIClientKernel==NULL && m_pClientSocket!=NULL)||(m_pIClientKernel!=NULL && m_pClientSocket==NULL));

	//������Ϣ
	if(m_pClientSocket!=NULL)m_pClientSocket->SendData(wMainCmdID, wSubCmdID, pBuffer, wDataSize);
	else m_pIClientKernel->SendSocketData(wMainCmdID, wSubCmdID, pBuffer, wDataSize);

	return ;
}

void CBankDrawing::OnPaint()
{
	CPaintDC dc(this); 

	//�滭����
	CRect ClipRect,ClientRect;
	dc.GetClipBox(&ClipRect);
	CSkinWndAttribute * pSkinAttribute=CSkinDialog::GetSkinAttribute();
	dc.FillSolidRect(&ClipRect,pSkinAttribute->m_crBackGround);
}

HBRUSH CBankDrawing::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (CTLCOLOR_STATIC==nCtlColor)
	{
		pDC->SetBkMode(TRANSPARENT);
		return m_brBakcground;
	}
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	return hbr;
}

void CBankDrawing::Process()
{
	//CPageDlg *pSelPage=m_pPageDlg[m_nSelPage];
	//ASSERT(pSelPage!=NULL);

	//INT64 lGoldCount=0;

	//switch( m_nSelPage )
	//{
	//case 0: //������
	//	{
	//		lGoldCount=pSelPage->GetDlgItemInt64(IDC_IN_COUNT);

	//		if (lGoldCount <= 0 || lGoldCount > m_lGameGold)
	//		{
	//			CString strMessage;
	//			if(m_lGameGold>0)
	//				strMessage.Format(TEXT("������Ľ��ֵ������1��%I64d֮��"), m_lGameGold);
	//			else 
	//				strMessage.Format(TEXT("��ĵ�ǰ�����ĿΪ0,���ܽ��д洢����!"));

	//			ShowInformationEx(strMessage,0 ,MB_ICONINFORMATION,TEXT("ϵͳ��Ϣ"));
	//			//GetDlgItem������öԻ��������������ָ���ؼ����Ӵ��ڵ�ָ��
	//			//SetFocus����Ҫ��õ����뽹�㡣���뽹�㽫�������м������붨��������ڡ�
	//			//ԭ��ӵ�����뽹����κδ��ڶ���ʧȥ����SetFocus��Ա������ʧȥ���뽹��Ĵ��ڷ���һ��WM_KILLFOCUS��Ϣ��
	//			//����������뽹��Ĵ��ڷ���һ��WM_SETFOCUS��Ϣ����������ô��ڻ����ĸ����ڡ�
	//			//�����ǰ�����Ǽ���ģ����ǲ��������뽹�㣨����ζ�ţ�û�д��ھ������뽹�㣩�����κΰ��µļ���������WM_SYSCHAR��WM_SYSKEYDOWN��WM_SYSKEYUP��Ϣ��
	//			pSelPage->GetDlgItem(IDC_IN_COUNT)->SetFocus();
	//			((CEdit*)pSelPage->GetDlgItem(IDC_IN_COUNT))->SetSel(0,-1);
	//			return;
	//		}
	//		break;
	//	}
	//case 1: //ȡ�����
	//	{
	//		lGoldCount=pSelPage->GetDlgItemInt64(IDC_OUT_COUNT);
	//		if (lGoldCount <= 0 || lGoldCount > m_lStorageGold)
	//		{
	//			CString strMessage;
	//			if(m_lStorageGold>0)
	//				strMessage.Format(TEXT("������Ľ��ֵ������1��%I64d֮��"),m_lStorageGold);
	//			else 
	//				strMessage.Format(TEXT("��Ĵ洢�����ĿΪ0,���ܽ�����ȡ����!"));

	//			ShowInformationEx(strMessage,0 ,MB_ICONINFORMATION,TEXT("ϵͳ��Ϣ"));
	//			pSelPage->GetDlgItem(IDC_OUT_COUNT)->SetFocus();
	//			((CEdit*)pSelPage->GetDlgItem(IDC_OUT_COUNT))->SetSel(0,-1);
	//			return;
	//		}
	//		break;
	//	}
	//case 2: //ת�����
	//	{
	//		lGoldCount=pSelPage->GetDlgItemInt64(IDC_OUT_COUNT);
	//		DWORD dwTagGameID=pSelPage->GetDlgItemInt(IDC_USER_ID);

	//		if (dwTagGameID==0)
	//		{
	//			ShowInformationEx(TEXT("�����û�ID����ȷ"),0 ,MB_ICONINFORMATION,TEXT("ϵͳ��Ϣ"));
	//			pSelPage->GetDlgItem(IDC_USER_ID)->SetFocus();
	//			return;
	//		}

	//		if (dwTagGameID==m_pMeUserData->dwGameID)
	//		{
	//			ShowInformationEx(TEXT("�����û�ID���������ѣ�����������"),0 ,MB_ICONINFORMATION,TEXT("ϵͳ��Ϣ"));
	//			pSelPage->GetDlgItem(IDC_USER_ID)->SetFocus();
	//			return;
	//		}

	//		if (lGoldCount <= 0 || lGoldCount > m_lStorageGold)
	//		{
	//			CString strMessage;
	//			if (m_lStorageGold>0)
	//				strMessage.Format(TEXT("������Ľ��ֵ������1��%I64d֮��"),m_lStorageGold);
	//			else 
	//				strMessage.Format(TEXT("��Ĵ洢�����ĿΪ0,���ܽ���ת�ʲ���!"));
	//			ShowInformationEx(strMessage,0 ,MB_ICONINFORMATION,TEXT("ϵͳ��Ϣ"));
	//			pSelPage->GetDlgItem(IDC_OUT_COUNT)->SetFocus();
	//			((CEdit*)pSelPage->GetDlgItem(IDC_OUT_COUNT))->SetSel(0,-1);
	//			return;
	//		}

	//		if ((m_lStorageGold-lGoldCount) < 2000)
	//		{
	//			ShowInformationEx(TEXT("�������б������ٱ��� 2000 ��ң�����ȫ��ת����"),0,MB_ICONINFORMATION,TEXT("ϵͳ��Ϣ"));
	//			((CEdit*)pSelPage->GetDlgItem(IDC_OUT_COUNT))->SetSel(0,-1);
	//			return;
	//		}
	//		break;
	//	}
	//case 3: //��������
	//	{
	//		TCHAR szOldPassword[PASS_LEN]=TEXT("");
	//		TCHAR szPassword1[PASS_LEN]=TEXT("");
	//		TCHAR szPassword2[PASS_LEN]=TEXT("");

	//		pSelPage->GetDlgItemText(IDC_BANK_PASSWORD,szOldPassword,CountArray(szOldPassword));
	//		pSelPage->GetDlgItemText(IDC_NEW_PASSWORD,szPassword1,CountArray(szPassword1));
	//		pSelPage->GetDlgItemText(IDC_CONFIRM_PASSWORD,szPassword2,CountArray(szPassword2));

	//		if (szPassword1[0]==0)
	//		{
	//			ShowInformationEx(TEXT("�����벻��Ϊ�գ���������д"),0 ,MB_ICONINFORMATION,TEXT("ϵͳ��Ϣ"));
	//			pSelPage->GetDlgItem(IDC_NEW_PASSWORD)->SetFocus();
	//			return;
	//		}

	//		if (lstrlen(szPassword1) < 6)
	//		{
	//			ShowInformationEx(TEXT("Ϊ���������밲ȫ�����볤�ȱ�����5λ����"),0 ,MB_ICONINFORMATION,TEXT("ϵͳ��Ϣ"));
	//			pSelPage->GetDlgItem(IDC_NEW_PASSWORD)->SetFocus();
	//			return;
	//		}

	//		if (szPassword2[0]==0)
	//		{
	//			ShowInformationEx(TEXT("ȷ�����벻��Ϊ�գ���������д"),0 ,MB_ICONINFORMATION,TEXT("ϵͳ��Ϣ"));
	//			pSelPage->GetDlgItem(IDC_CONFIRM_PASSWORD)->SetFocus();
	//			return;
	//		}

	//		if (lstrlen(szPassword2) < 6)
	//		{
	//			ShowInformationEx(TEXT("Ϊ���������밲ȫ�����볤�ȱ�����5λ����"),0 ,MB_ICONINFORMATION,TEXT("ϵͳ��Ϣ"));
	//			pSelPage->GetDlgItem(IDC_CONFIRM_PASSWORD)->SetFocus();
	//			return;
	//		}

	//		if (strcmp(szPassword1,szPassword2)!=0)
	//		{
	//			ShowInformationEx(TEXT("��������������벻һ�£���������д"),0 ,MB_ICONINFORMATION,TEXT("ϵͳ��Ϣ"));
	//			pSelPage->GetDlgItem(IDC_CONFIRM_PASSWORD)->SetFocus();
	//			return;
	//		}

	//		if (strcmp(szOldPassword,szPassword1)==0)
	//		{
	//			ShowInformationEx(TEXT("�������������;�����ͬ����������д"),0 ,MB_ICONINFORMATION,TEXT("ϵͳ��Ϣ"));
	//			pSelPage->GetDlgItem(IDC_NEW_PASSWORD)->SetFocus();
	//			return;
	//		}
	//		break;
	//	}
	//}

	////����Ч��
	//TCHAR szPassword[PASS_LEN]=TEXT("");

	//if (m_nSelPage > 0)
	//{
	//	pSelPage->GetDlgItemText(IDC_BANK_PASSWORD, szPassword, CountArray(szPassword));

	//	if (szPassword[0]==0)
	//	{
	//		ShowInformationEx(TEXT("�������벻��Ϊ�գ��������������룡"),0,MB_ICONINFORMATION,TEXT("ϵͳ��Ϣ")) ;
	//		pSelPage->GetDlgItem(IDC_BANK_PASSWORD)->SetWindowText("");
	//		pSelPage->GetDlgItem(IDC_BANK_PASSWORD)->SetFocus();
	//		return;
	//	}

	//	TCHAR szTempPassword[PASS_LEN]=TEXT("");
	//	CopyMemory(szTempPassword,szPassword,sizeof(szTempPassword));
	//	CMD5Encrypt::EncryptData(szTempPassword,szPassword);
	//}


	//switch( m_nSelPage )
	//{
	//case 0: //������
	//	{
	//		//������Ϣ
	//		CMD_GF_BankStorage BankStorage;
	//		ZeroMemory(&BankStorage, sizeof(BankStorage));
	//		BankStorage.lStorageValue = lGoldCount;
	//		BankStorage.cbGameAction=m_bGameAction;
	//		CopyMemory(BankStorage.szPassword,szPassword,sizeof(BankStorage.szPassword));
	//		SendData(MDM_GF_BANK, SUB_GF_BANK_STORAGE, &BankStorage, sizeof(BankStorage));
	//		break;
	//	}
	//case 1: //ȡ�����
	//	{
	//		//������Ϣ
	//		CMD_GF_BankGet BankGetGold;
	//		ZeroMemory(&BankGetGold, sizeof(BankGetGold));
	//		BankGetGold.lGetValue = lGoldCount;
	//		BankGetGold.cbGameAction=m_bGameAction;
	//		CopyMemory(BankGetGold.szPassword,szPassword,sizeof(BankGetGold.szPassword));
	//		SendData(MDM_GF_BANK, SUB_GF_BANK_GET, &BankGetGold, sizeof(BankGetGold));
	//		break;
	//	}
	//case 2: //ת����
	//	{
	//		CString static strNumber=TEXT(""), strTmpNumber1,strTmpNumber2;
	//		strTmpNumber1.Empty();
	//		strTmpNumber2.Empty();
	//		strNumber.Empty();
	//		if(lGoldCount==0) strNumber=TEXT("0");
	//		int nNumberCount=0;
	//		int count=0;
	//		INT64 lTmpNumber=lGoldCount;
	//		while(lGoldCount>0)
	//		{
	//			strTmpNumber1.Format(TEXT("%I64d"),lGoldCount%10);
	//			nNumberCount++;
	//			strTmpNumber2 = strTmpNumber1+strTmpNumber2;

	//			if(nNumberCount==3)
	//			{
	//				count++;
	//				if(count==1)
	//				{
	//					strTmpNumber2 +=strNumber;
	//				}
	//				else
	//				{
	//					strTmpNumber2 += (TEXT(",") +strNumber);
	//				}
	//				strNumber=strTmpNumber2;
	//				nNumberCount=0;
	//				strTmpNumber2=TEXT("");
	//			}
	//			lGoldCount/=10;
	//		}

	//		if (strTmpNumber2.IsEmpty()==FALSE)
	//		{
	//			strTmpNumber2 += (TEXT(",") +strNumber);
	//			strNumber=strTmpNumber2;
	//		}

	//		DWORD dwTagGameID=pSelPage->GetDlgItemInt(IDC_USER_ID);

	//		INT64 lRevenue=lTmpNumber * 0.01;
	//		TCHAR szMessage[512]=TEXT("");
	//		sprintf(szMessage, TEXT("���Ƿ�ȷ��Ҫת %s ��ң���˰ %I64d ��ң����û�IDΪ��%d�ݵ��û���"),
	//			strNumber,lRevenue,dwTagGameID);

	//		if (ShowInformationEx(szMessage,0,MB_YESNO,TEXT("ת��ȷ��"))!=IDYES) return;

	//		CMD_GF_BankTransfer BankTransfer;
	//		ZeroMemory(&BankTransfer,sizeof(BankTransfer));
	//		BankTransfer.dwTagGameID=dwTagGameID;
	//		BankTransfer.lTransferValue=lTmpNumber;
	//		BankTransfer.cbGameAction=m_bGameAction;
	//		CopyMemory(BankTransfer.szPassword,szPassword,sizeof(BankTransfer.szPassword));
	//		SendData(MDM_GF_BANK, SUB_GF_BANK_TRANSFER, &BankTransfer, sizeof(BankTransfer));

	//		break;
	//	}
	//case 3: //�޸�����
	//	{
	//		TCHAR szNewPassword[PASS_LEN]=TEXT("");
	//		TCHAR szEncrypt[PASS_LEN]=TEXT("");
	//		pSelPage->GetDlgItemText(IDC_NEW_PASSWORD, szNewPassword, CountArray(szNewPassword));

	//		CMD_GF_BankSecurity BankSecurity;
	//		ZeroMemory(&BankSecurity,sizeof(BankSecurity));

	//		BankSecurity.cbGameAction=m_bGameAction;
	//		CMD5Encrypt::EncryptData(szNewPassword,szEncrypt);
	//		CopyMemory(BankSecurity.szPassword,szPassword,sizeof(BankSecurity.szPassword));
	//		CopyMemory(BankSecurity.szNewPassword,szEncrypt,sizeof(BankSecurity.szNewPassword));

	//		SendData(MDM_GF_BANK, SUB_GF_BANK_SECURITY, &BankSecurity, sizeof(BankSecurity));
	//		break;
	//	}
	//}

	//SetWaiting(TRUE);
}

void MakeString(CString &strNum,LONG lNumber)
{
	CString static strNumber=TEXT(""), strTmpNumber1,strTmpNumber2;
	strTmpNumber1.Empty();
	strTmpNumber2.Empty();
	strNumber.Empty();
	if (lNumber==0) strNumber=TEXT("0");
	int nNumberCount=0;
	LONG lTmpNumber=lNumber;
	if (lNumber<0) lNumber=-lNumber;
	bool blfirst = true;
	bool bLongNum = false;
	if(lNumber>=1000)
	{
		bLongNum = true;
	}
	while (lNumber>0)
	{
		
		strTmpNumber1.Format(TEXT("%ld"),lNumber%10);
		nNumberCount++;
		strTmpNumber2 = strTmpNumber1+strTmpNumber2;

		if (nNumberCount==3)
		{
			if(blfirst)
			{
				strTmpNumber2 += (TEXT("") +strNumber);
				blfirst = false;

			}
			else
			{
				strTmpNumber2 += (TEXT(",") +strNumber);

			}
			
			strNumber=strTmpNumber2;
			nNumberCount=0;
			strTmpNumber2=TEXT("");
		}
		lNumber/=10;
	}

	if (strTmpNumber2.IsEmpty()==FALSE)
	{
		if(bLongNum)
	    	strTmpNumber2 += (TEXT(",") +strNumber);
		else
			strTmpNumber2 += strNumber;

		strNumber=strTmpNumber2;
	}

	if (lTmpNumber<0) strNumber=TEXT("-")+strNumber;

	strNum = strNumber ;


}

void MakeStringToNum(CString str,LONG &Num)
{

	CString str1;
	int Pos = 0;
	int len = 0;
	int Start = 0;
	while (1)
	{
		len = str.GetLength();
		Pos = str.Find(",",Pos);
		if(Pos==-1)
		{
			str1+=str;
			break;
		}else
		{
			
			char Tmp[126];
			memset(Tmp,0,126);
			memcpy(Tmp,&str.GetBuffer()[Start],Pos);
			if(Pos+1<=str.GetLength())
			{
				str = &str.GetBuffer()[Pos+1];
				str1+=Tmp;
				Start =  0;
				Pos = 0;

			}else
			{
				break;
			}
			
			
		}
	}

	_snscanf(str1.GetBuffer(),str1.GetLength(),_TEXT("%ld"),&Num);
	

}