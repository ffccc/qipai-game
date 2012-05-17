#include "StdAfx.h"
#include "Resource.h"
#include "DlgServiceItem.h"

//////////////////////////////////////////////////////////////////////////////////

//�ؼ���ʶ
#define IDC_WEB_PUBLICIZE			100									//����ؼ�

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgServiceItem, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CDlgServiceBrowser, CDlgServiceItem)
	ON_WM_SIZE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgServiceItem::CDlgServiceItem(UINT uIDTemplate) : CDialog(uIDTemplate)
{
	//������Դ
	m_cbImageIndex=0;

	return;
}

//��������
CDlgServiceItem::~CDlgServiceItem()
{
}

//�滭����
VOID CDlgServiceItem::DrawItemView(CDC * pDC, INT nWidth, INT nHeight)
{
	return;
}

//��������
VOID CDlgServiceItem::CreateServiceItem(CWnd * pParentWnd, CRect rcCreate)
{
	//��������
	Create(m_lpszTemplateName,pParentWnd);

	//�ƶ�����
	SetWindowPos(NULL,rcCreate.left,rcCreate.top,rcCreate.Width(),rcCreate.Height(),SWP_NOZORDER|SWP_SHOWWINDOW);

	return;
}

//��ȡ�ַ�
VOID CDlgServiceItem::GetControlItemText(UINT uID, TCHAR szString[], WORD wMaxCount)
{
	//��ȡ�ַ�
	CString strString;
	GetDlgItemText(uID,strString);

	//�����ַ�
	strString.TrimLeft();
	strString.TrimRight();

	//���ý��
	lstrcpyn(szString,strString,wMaxCount);

	return;
}

//�滭����
BOOL CDlgServiceItem::OnEraseBkgnd(CDC * pDC)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��䱳��
	pDC->FillSolidRect(&rcClient,CSkinDialog::m_SkinAttribute.m_crBackGround);

	return TRUE;
}

//��ɫ��Ϣ
HBRUSH CDlgServiceItem::OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
	case CTLCOLOR_STATIC:
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetBkColor(CSkinDialog::m_SkinAttribute.m_crBackGround);
			pDC->SetTextColor(CSkinDialog::m_SkinAttribute.m_crControlText);
			return CSkinDialog::m_SkinAttribute.m_brBackGround;
		}
	}

	return __super::OnCtlColor(pDC,pWnd,nCtlColor);
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgServiceBrowser::CDlgServiceBrowser() : CDlgServiceItem(IDD_DLG_SERVICE_BROWSER)
{
	//���ñ���
	ZeroMemory(m_szItemName,sizeof(m_szItemName));
	ZeroMemory(m_szServiceUrl,sizeof(m_szServiceUrl));

	return;
}

//��������
CDlgServiceBrowser::~CDlgServiceBrowser()
{
}

//��������
BOOL CDlgServiceBrowser::OnInitDialog()
{
	__super::OnInitDialog();

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//�����ؼ�
	m_ServicePublicize.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rcClient,this,IDC_WEB_PUBLICIZE);

	//����ҳ��
	if (m_szServiceUrl[0]!=0)
	{
		//�����ַ
		TCHAR szBrowseUrl[256]=TEXT("");
		_sntprintf(szBrowseUrl,CountArray(szBrowseUrl),TEXT("%s/%s"),szPlatformLink,m_szServiceUrl);

		//���ӵ�ַ
		m_ServicePublicize.Navigate(szBrowseUrl,NULL,NULL,NULL,NULL);
	}

	return TRUE;
}

//���÷���
VOID CDlgServiceBrowser::InitService(BYTE cbImageIndex, LPCTSTR pszItemName, LPCTSTR pszServiceUrl)
{
	//���ñ���
	m_cbImageIndex=cbImageIndex;
	lstrcpyn(m_szItemName,pszItemName,CountArray(m_szItemName));
	lstrcpyn(m_szServiceUrl,pszServiceUrl,CountArray(m_szServiceUrl));

	return;
}

//λ����Ϣ
VOID CDlgServiceBrowser::OnSize(UINT nType, INT cx, INT cy) 
{
	__super::OnSize(nType, cx, cy);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//�ƶ��ؼ�
	if (m_ServicePublicize.m_hWnd!=NULL)
	{
		m_ServicePublicize.SetWindowPos(NULL,0,0,rcClient.Width(),rcClient.Height(),SWP_NOZORDER);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
