#include "StdAfx.h"
#include "Resource.h"
#include "DlgBrowser.h"

//////////////////////////////////////////////////////////////////////////////////

//�ؼ���ʶ
#define IDC_BROWSER					100									//����ؼ�

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgBrowser, CSkinDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgBrowser::CDlgBrowser() : CSkinDialog(IDD_BROWSER)
{
	//���ñ���
	m_SizeWindow.SetSize(0,0);
	
	return;
}

//��������
CDlgBrowser::~CDlgBrowser()
{
}

//��Ϣ����
BOOL CDlgBrowser::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//��������
BOOL CDlgBrowser::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(m_strWindowTitle);

	//������Դ
	AfxSetResourceHandle(GetModuleHandle(NULL));

	//�����ؼ�
	CRect rcCreate(0,0,0,0);
	m_WebPublicize.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcCreate,this,IDC_BROWSER);

	//������С
	if ((m_SizeWindow.cx>0)&&(m_SizeWindow.cy>0))
	{
		SetWindowPos(NULL,0,0,m_SizeWindow.cx,m_SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE);
	}

	//���д���
	CenterWindow();

	//�����ؼ�
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());

	//�����ַ
	TCHAR szBrowser[256]=TEXT("");
	_sntprintf(szBrowser,CountArray(szBrowser),TEXT("%s/%s"),szPlatformLink,m_strBrowserURL);

	//����ҳ��
	m_WebPublicize.Navigate(szBrowser,NULL,NULL,NULL,NULL);

	return FALSE;
}

//ȷ������
VOID CDlgBrowser::OnOK()
{
	return;
}

//�����ؼ�
VOID CDlgBrowser::RectifyControl(INT nWidth, INT nHeight)
{
	//����λ��
	if (m_WebPublicize.m_hWnd!=NULL)
	{
		//���λ��
		INT nTBorder=m_SkinAttribute.m_EncircleInfoFrame.nTBorder;
		INT nBBorder=m_SkinAttribute.m_EncircleInfoFrame.nBBorder;
		INT nLBorder=m_SkinAttribute.m_EncircleInfoFrame.nLBorder;
		INT nRBorder=m_SkinAttribute.m_EncircleInfoFrame.nRBorder;

		//�ƶ�λ��
		m_WebPublicize.SetWindowPos(NULL,nLBorder,nTBorder,nWidth-nRBorder-nLBorder,nHeight-nBBorder-nTBorder,SWP_NOZORDER);
	}

	return;
}

//��������
VOID CDlgBrowser::CreateBrowserControl(LPCTSTR pszTitle, LPCTSTR pszBrowserURL, CSize SizeWindow)
{
	//���ñ���
	m_SizeWindow=SizeWindow;
	m_strWindowTitle=pszTitle;
	m_strBrowserURL=pszBrowserURL;

	//������Դ
	AfxSetResourceHandle(GetModuleHandle(SHARE_CONTROL_DLL_NAME));

	//��ʾ����
	DoModal();

	return;
}

//λ����Ϣ
VOID CDlgBrowser::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//�����ؼ�
	RectifyControl(cx,cy);

	return;
}

//////////////////////////////////////////////////////////////////////////////////
