#include "StdAfx.h"
#include "Resource.h"
#include "DlgBuyProperty.h"
#include "GamePropertyItem.h"

//////////////////////////////////////////////////////////////////////////////////

//��̬����
CDlgBuyProperty * CDlgBuyProperty::m_pDlgBuyProperty=NULL;				//����ָ��

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgBuyProperty, CSkinDialog)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgBuyProperty::CDlgBuyProperty() : CSkinDialog(IDD_DLG_PROPERTY)
{
	//���ñ���
	m_pGamePropertyItem=NULL;
	m_pIGamePropertySink=NULL;

	//���ö���
	ASSERT(m_pDlgBuyProperty==NULL);
	if (m_pDlgBuyProperty==NULL) m_pDlgBuyProperty=this;

	return;
}

//��������
CDlgBuyProperty::~CDlgBuyProperty()
{
	//���ö���
	ASSERT(m_pDlgBuyProperty==this);
	if (m_pDlgBuyProperty==this) m_pDlgBuyProperty=NULL;

	return;
}

//�ؼ���
VOID CDlgBuyProperty::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	//��ť�ؼ�
	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Control(pDX, IDCANCEL, m_btCancel);

	//�༭�ؼ�
	DDX_Control(pDX, IDC_NICK_NAME, m_edNickName);
	DDX_Control(pDX, IDC_PROPERTY_COUNT, m_edPropertyCount);
}

//��Ϣ����
BOOL CDlgBuyProperty::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//��������
BOOL CDlgBuyProperty::OnInitDialog()
{
	__super::OnInitDialog();

	//����״̬
	UpdateCtrlStatus();

	//��������
	m_edNickName.LimitText(LEN_NICKNAME-1);

	return FALSE;
}

//ȷ������
VOID CDlgBuyProperty::OnOK()
{
	//��ȡ��Ϣ
	CString strNickName;
	GetDlgItemText(IDC_NICK_NAME,strNickName);
	strNickName.TrimLeft();strNickName.TrimRight();

	//��ȡ��Ŀ
	WORD wItemCount=GetDlgItemInt(IDC_PROPERTY_COUNT);

	//�ǳ��ж�
	if (strNickName.IsEmpty()==true)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("�û��ǳƲ���ȷ������������"),MB_ICONERROR);

		//���ý���
		m_edNickName.SetFocus();

		return;
	}

	//��Ŀ�ж�
	if (wItemCount==0)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("������Ŀ����ȷ������������"),MB_ICONERROR);

		//���ý���
		m_edPropertyCount.SetFocus();

		return;
	}

	//��ȡ��Ϣ
	ASSERT(m_pGamePropertyItem->GetPropertyAttrib()!=NULL);
	tagPropertyAttrib * pPropertyAttrib=m_pGamePropertyItem->GetPropertyAttrib();

	//�¼�֪ͨ
	ASSERT(m_pIGamePropertySink!=NULL);
	if (m_pIGamePropertySink->OnEventBuyProperty(strNickName,wItemCount,pPropertyAttrib->wIndex)==true)
	{
		//�رմ���
		DestroyWindow();
	}

	return;
}

//ȡ����Ϣ
VOID CDlgBuyProperty::OnCancel()
{
	//�رմ���
	DestroyWindow();

	return;
}

//�滭��Ϣ
VOID CDlgBuyProperty::OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight)
{
	//���λ��
	INT nTBorder=m_SkinAttribute.m_EncircleInfoFrame.nTBorder;
	INT nBBorder=m_SkinAttribute.m_EncircleInfoFrame.nBBorder;
	INT nLBorder=m_SkinAttribute.m_EncircleInfoFrame.nLBorder;
	INT nRBorder=m_SkinAttribute.m_EncircleInfoFrame.nRBorder;

	//������Դ
	CPngImage ImagePropertyFrame;
	ImagePropertyFrame.LoadImage(GetModuleHandle(GAME_PROPERTY_DLL_NAME),TEXT("PROPERTY_FRAME"));

	//��ȡ��С
	CSize SizePropertyFrame;
	SizePropertyFrame.SetSize(ImagePropertyFrame.GetWidth(),ImagePropertyFrame.GetHeight());

	//�滭���
	INT nXItemPos=nLBorder+15;
	INT nYItemPos=nTBorder+15;
	ImagePropertyFrame.DrawImage(pDC,nXItemPos-(SizePropertyFrame.cx-PROPERTY_SYMBOL_CX)/2,nYItemPos-(SizePropertyFrame.cy-PROPERTY_SYMBOL_CY)/2);

	//�滭ͼ��
	if (m_pGamePropertyItem!=NULL)
	{
		//��ȡ��Դ
		tagPropertyImage PropertyImage;
		m_pGamePropertyItem->GetImageInfo(PropertyImage);

		//�滭ͼ��
		CPngImage ImageSymbol;
		ImageSymbol.LoadImage(PropertyImage.hResInstance,PropertyImage.pszSymbolID);
		ImageSymbol.DrawImage(pDC,nXItemPos,nYItemPos,PROPERTY_SYMBOL_CX,PROPERTY_SYMBOL_CY,0,0,ImageSymbol.GetWidth(),ImageSymbol.GetHeight());
	}

	return;
}

//����״̬
VOID CDlgBuyProperty::UpdateCtrlStatus()
{
	//Ч�����
	ASSERT(m_pGamePropertyItem!=NULL);
	ASSERT(m_pIGamePropertySink!=NULL);

	//��ȡ����
	tagPropertyInfo * pPropertyInfo=m_pGamePropertyItem->GetPropertyInfo();
	tagPropertyAttrib * pPropertyAttrib=m_pGamePropertyItem->GetPropertyAttrib();

	//���߼۸�
	TCHAR szPropertyGold[64]=TEXT("");
	_sntprintf(szPropertyGold,CountArray(szPropertyGold),TEXT("%I64d ��Ϸ��/�� ����Ա�ۿۣ�%ld%%��"),pPropertyInfo->lPropertyGold,pPropertyInfo->wDiscount);

	//���ñ���
	SetWindowText(pPropertyAttrib->szPropertyName);

	//���߼۸�
	SetDlgItemText(IDC_PROPERTY_GOLD,szPropertyGold);

	//��������
	SetDlgItemText(IDC_PROPERTY_NAME,pPropertyAttrib->szPropertyName);
	SetDlgItemText(IDC_PROPERTY_DESCRIBE,pPropertyAttrib->szRegulationsInfo);

	return;
}

//��ʾ����
VOID CDlgBuyProperty::ShowBuyPropertyWnd(CGamePropertyItem * pGamePropertyItem, IGamePropertySink * pIGamePropertySink)
{
	//Ч�����
	ASSERT(pGamePropertyItem!=NULL);
	ASSERT(pIGamePropertySink!=NULL);

	//��������
	CDlgBuyProperty * pDlgBuyProperty=m_pDlgBuyProperty;

	//��������
	if (pDlgBuyProperty==NULL)
	{
		try
		{
			pDlgBuyProperty=new CDlgBuyProperty;
		}
		catch (...)
		{
			ASSERT(FALSE);
		}
	}

	//���ñ���
	pDlgBuyProperty->m_pGamePropertyItem=pGamePropertyItem;
	pDlgBuyProperty->m_pIGamePropertySink=pIGamePropertySink;

	//��������
	if (pDlgBuyProperty->m_hWnd==NULL)
	{
		AfxSetResourceHandle(GetModuleHandle(GAME_PROPERTY_DLL_NAME));
		pDlgBuyProperty->Create(IDD_DLG_PROPERTY,AfxGetMainWnd());
		AfxSetResourceHandle(GetModuleHandle(NULL));
	}
	else
	{
		//����״̬
		pDlgBuyProperty->UpdateCtrlStatus();
	}

	//��ʾ����
	pDlgBuyProperty->ShowWindow(SW_SHOW);

	//�����
	pDlgBuyProperty->SetActiveWindow();
	pDlgBuyProperty->SetForegroundWindow();

	return;
}

//////////////////////////////////////////////////////////////////////////////////
