#include "Stdafx.h"
#include "resource.h"
#include "SingleProItem.h"
#include ".\singleproitem.h"

//////////////////////////////////////////////////////////////////////

//��ʱ�� ID
#define IDT_STATUS_TEXT					1					//״̬����

//////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CBugleDlg, CSkinDialogEx)
	ON_BN_CLICKED(IDC_EXPRESSION, OnBnClickedExpression)
	ON_BN_CLICKED(IDC_COLOR_SET, OnBnClickedColorSet)
	ON_MESSAGE(WM_HIT_EXPMESSTION,OnHitExpression)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////

//���캯��
CBugleDlg::CBugleDlg(CWnd* pParent): CSkinDialogEx(IDD_BUGLE,pParent)
{
	//���ñ���
	m_pMeUserData=NULL;
	m_crChatTX = RGB(0,0,0);
	m_pIClientKernel=NULL;
	m_pClientSocket=NULL;
}

//��������
CBugleDlg::~CBugleDlg()
{
}

//�ؼ���
void CBugleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_EXPRESSION, m_btExpression);
	DDX_Control(pDX, IDC_COLOR_SET, m_btColorSet);
	DDX_Control(pDX, IDC_InputChat, m_InputChat);
	DDX_Text(pDX, IDC_InputChat, m_strInuptChat);
	DDV_MaxChars(pDX, m_strInuptChat, BUGLE_MAX_CHAR);
	DDX_Text(pDX, IDC_PROPERTY_INFO, m_strPropertyInfo);

	return;
}

//��ʼ���
BOOL CBugleDlg::OnInitDialog()
{
	CSkinDialogEx::OnInitDialog();

	//������Դ
	UpdateSkinResource();

	m_strPropertyInfo.Format(TEXT("С����%ld��Ϸ��һ����ÿ����Ϣ���ó���%d���ַ�����Ϣ���ڷ��估�����ڵ�������Ϸ����������Ŀ�ķ�ʽ��ʾ��"),
		CPropertyBar::m_PropertyInfo[PROP_BUGLE].lPrice1, BUGLE_MAX_CHAR);

	UpdateData(FALSE);

	CString strTitle ;
	strTitle.Format(TEXT("������ʣ���С������ĿΪ[ %d ]"), m_pMeUserData->dwPropResidualTime[PROP_BUGLE]);
	
	//���ñ���
	SetWindowText(strTitle);
	SetTimer(IDT_STATUS_TEXT,400,NULL);

	return TRUE;
}

//������Ϣ
LRESULT CBugleDlg::OnHitExpression(WPARAM wParam, LPARAM lParam)
{
	CExpressionItem * pExpressionItem=m_Expression.GetExpressItem((WORD)wParam);
	if (pExpressionItem!=NULL)
	{
		CString strChatMsg;
		m_InputChat.GetWindowText(strChatMsg);
		strChatMsg+=pExpressionItem->m_szTrancelate;
		m_InputChat.SetWindowText(strChatMsg);
		m_InputChat.SetFocus();
		m_InputChat.SetSel(strChatMsg.GetLength(),strChatMsg.GetLength());
	}

	return 0;
}

//��ɫ��ť
void CBugleDlg::OnBnClickedColorSet()
{
	//������ɫ
	CColorDialog ColorDialog(m_crChatTX,CC_FULLOPEN);
	
	if (ColorDialog.DoModal()==IDOK)
	{
		m_crChatTX=ColorDialog.GetColor();

		//���ý���
		m_InputChat.SetFocus();
	}

	return;
}

//���鰴ť
void CBugleDlg::OnBnClickedExpression()
{
	//�������鴰��
	if (m_Expression.GetSafeHwnd()==NULL) m_Expression.CreateExpression(this);

	//�ƶ�����
	CRect rcButton;
	CSize ExpressSize;
	m_Expression.GetFixSize(ExpressSize);
	m_btExpression.GetWindowRect(&rcButton);
	m_Expression.MoveWindow(rcButton.right-ExpressSize.cx,rcButton.top-ExpressSize.cy,ExpressSize.cx,ExpressSize.cy);
	m_Expression.ShowWindow(SW_SHOW);
	m_Expression.SetFocus();

	return;
}

//ȷ����Ϣ
void CBugleDlg::OnBnClickedOk()
{
	//��ȡ��Ϣ
	CString strMsg;
	m_InputChat.GetWindowText(strMsg);
	if ( strMsg.IsEmpty() )
	{
		ShowInformationEx(TEXT("�������ݲ���Ϊ�գ�"), 0,MB_ICONINFORMATION,TEXT("ϵͳ��ʾ")); 
		return ;
	}
	if ( BUGLE_MAX_CHAR <= strMsg.GetLength() )
	{ 
		CString strMsg;
		strMsg.Format(TEXT("�����������Ϊ%d���ַ���"), BUGLE_MAX_CHAR);
		ShowInformationEx(strMsg, 0,MB_ICONINFORMATION,TEXT("ϵͳ��ʾ")); 
		return ;
	}

	//��ȡ���
	tagUserData const *pMeUserData = m_pMeUserData;

	//��������
	CMD_GF_BugleProperty BugleProperty ;
	ZeroMemory(&BugleProperty, sizeof(BugleProperty));
	BugleProperty.cbSendLocation=(m_pIClientKernel!=NULL)?LOCATION_GAME_ROOM:LOCATION_PLAZA_ROOM;
	lstrcpyn(BugleProperty.szUserName, pMeUserData->szName, CountArray(BugleProperty.szUserName));
	BugleProperty.crText = m_crChatTX;
	lstrcpyn(BugleProperty.szContext, strMsg, CountArray(BugleProperty.szContext));

	//������Ϣ
	SendData(MDM_GF_PRESENT, SUB_GF_PROP_BUGLE, &BugleProperty,sizeof(BugleProperty));

	OnOK();
	return;
}

//�ؼ���ɫ
HBRUSH CBugleDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSkinDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( nCtlColor == CTLCOLOR_EDIT )
	{
		pDC->SetTextColor(m_crChatTX);
	}
	return hbr;
}

//ʱ����Ϣ
void CBugleDlg::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent)
	{
	case IDT_STATUS_TEXT:			//������Ŀ
		{
			//��ȡ���
			tagUserData const *pMeUserData = m_pMeUserData;

			//������Ϣ
			CString strTitle ;
			strTitle.Format(TEXT("������ʣ���С������ĿΪ[ %d ]"),(WORD)m_pMeUserData->dwPropResidualTime[PROP_BUGLE]);
			SetWindowText(strTitle);

			return;
		}
	}

	CSkinDialogEx::OnTimer(nIDEvent);
}

//�滭����
void CBugleDlg::OnPaint()
{
	CPaintDC dc(this); 

	//�滭����
	DrawCaption(&dc);

	CRect ClientRect;
	GetClientRect(&ClientRect);
	int nXExcursion=GetXExcursionPos();
	int nYExcursion=GetYExcursionPos();

	//�滭����
	COLORREF Color = RGB(161,212,226);
	dc.FillSolidRect(nXExcursion,nYExcursion,ClientRect.Width()-2*nXExcursion,4,Color);
	dc.FillSolidRect(nXExcursion,ClientRect.Height()-8,ClientRect.Width()-2*nXExcursion,7,Color);
	dc.FillSolidRect(nXExcursion,nYExcursion+4,12,ClientRect.Height()-nYExcursion-9,Color);
	dc.FillSolidRect(ClientRect.Width()-nXExcursion-12,nYExcursion+4,12,ClientRect.Height()-nYExcursion-9,Color);

	//�滭�߿�
	DrawBorder(&dc);

	return;
}

//������Դ
bool CBugleDlg::UpdateSkinResource()
{
	HINSTANCE hInstance= CPropertyBar::m_pPropertyBar->m_ReshInstance;
	m_btExpression.SetButtonImage(CPropertyBar::m_pPropertyBar->m_PropertyViewImage.uBtBugleExpression,hInstance,false);
	m_btColorSet.SetButtonImage(CPropertyBar::m_pPropertyBar->m_PropertyViewImage.uBtBugleColorSet,hInstance,false);

	return true;
}

//������Ϣ
void CBugleDlg::SetSendInfo(IClientKernel *pClientKernel,ITCPSocket *pClientSocket,tagUserData *pMeUserData)
{
	ASSERT((pClientKernel==NULL && pClientSocket!=NULL)||(pClientKernel!=NULL && pClientSocket==NULL));

	//������Ϣ
	m_pIClientKernel = pClientKernel;
	m_pClientSocket= pClientSocket;
	m_pMeUserData=pMeUserData;

	return;
}

//������Ϣ
void CBugleDlg::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	ASSERT((m_pIClientKernel==NULL && m_pClientSocket!=NULL)||(m_pIClientKernel!=NULL && m_pClientSocket==NULL));

	//������Ϣ
	if(m_pClientSocket!=NULL)m_pClientSocket->SendData(wMainCmdID, wSubCmdID, pBuffer, wDataSize);
	else m_pIClientKernel->SendSocketData(wMainCmdID, wSubCmdID, pBuffer, wDataSize);

	return ;
}

//////////////////////////////////////////////////////////////////////

//���˵���
extern "C" PROPERTY_MODULE_CLASS VOID __cdecl ShowBugleDlg(IClientKernel *pIClientKernel,ITCPSocket *pClientSocket,tagUserData *pMeUserData)
{
	//ģ̬�Ի���
	CBugleDlg BugleDlg;
	BugleDlg.SetSendInfo(pIClientKernel,pClientSocket,pMeUserData);
	BugleDlg.DoModal();

	return ;
}

//////////////////////////////////////////////////////////////////////

