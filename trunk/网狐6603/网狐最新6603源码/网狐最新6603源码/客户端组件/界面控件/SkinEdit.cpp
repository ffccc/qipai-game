#include "Stdafx.h"
#include "SkinEdit.h"
#include "SkinDialog.h"

//////////////////////////////////////////////////////////////////////////////////

//��̬����
CSkinEditAttribute					CSkinEdit::m_SkinAttribute;				//��������

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSkinEdit, CEdit)
    ON_WM_NCPAINT()
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSkinEditEx, CSkinEdit)
    ON_WM_NCPAINT()
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CSkinEditAttribute::CSkinEditAttribute()
{
	//������ɫ
	m_crEnableBK=CLR_INVALID;
	m_crDisableBK=CLR_INVALID;

	//������ɫ
	m_crEnableText=CLR_INVALID;
	m_crDisableText=CLR_INVALID;

	//�߿���ɫ
	m_crEnableBorad=CLR_INVALID;
	m_crDisableBorad=CLR_INVALID;

	return;
}

//��������
CSkinEditAttribute::~CSkinEditAttribute() 
{
	//������Դ
	if (m_brEnable.GetSafeHandle()!=NULL) m_brEnable.DeleteObject();
	if (m_brDisable.GetSafeHandle()!=NULL) m_brDisable.DeleteObject();

	return;
}

//������Դ
bool CSkinEditAttribute::Initialization(CSkinRenderManager * pSkinRenderManager)
{
	//������ɫ
	m_crEnableBK=RGB(255,255,255);
	m_crDisableBK=RGB(125,125,125);

	//������ɫ
	m_crEnableText=RGB(10,10,10);
	m_crDisableText=RGB(50,50,50);

	//�߿���ɫ
	m_crEnableBorad=RGB(92,100,105);
	m_crDisableBorad=GetSysColor(COLOR_WINDOW);

	//������ˢ
	m_brEnable.DeleteObject();
	m_brDisable.DeleteObject();
	m_brEnable.CreateSolidBrush(m_crEnableBK);
	m_brDisable.CreateSolidBrush(m_crDisableBK);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CSkinEdit::CSkinEdit()
{
}

//��������
CSkinEdit::~CSkinEdit()
{
}

//�滭�ؼ�
HBRUSH CSkinEdit::CtlColor(CDC * pDC, UINT nCtlColor)
{
	//���û���
	if (IsWindowEnabled()==FALSE)
	{
		//����״̬
		pDC->SetBkColor(m_SkinAttribute.m_crDisableBK);
		pDC->SetTextColor(m_SkinAttribute.m_crDisableText);
		return m_SkinAttribute.m_brDisable;
	}
	else
	{
		//��ͨ״̬
		pDC->SetBkColor(m_SkinAttribute.m_crEnableBK);
		pDC->SetTextColor(m_SkinAttribute.m_crEnableText);
		return m_SkinAttribute.m_brEnable;
	}

	return NULL;
}

//�滭�ؼ�
VOID CSkinEdit::DrawControlView(CDC * pDC, COLORREF crColorBorad,  COLORREF crColorBK)
{
	//��ȡλ��
	CRect rcWindow;
	GetWindowRect(&rcWindow);
	ScreenToClient(&rcWindow);

	//�滭�߿�
	if (GetExStyle()&WS_EX_CLIENTEDGE)
	{
		pDC->Draw3dRect(&rcWindow,crColorBorad,crColorBorad);
		pDC->Draw3dRect(rcWindow.left+1,rcWindow.top+1,rcWindow.Width()-2,rcWindow.Height()-2,crColorBK,crColorBK);
	}
	else
	{
		pDC->Draw3dRect(&rcWindow,crColorBK,crColorBK);
		pDC->Draw3dRect(rcWindow.left+1,rcWindow.top+1,rcWindow.Width()-2,rcWindow.Height()-2,crColorBK,crColorBK);
	}

	return;
}

//�ػ���Ϣ
VOID CSkinEdit::OnNcPaint()
{
	//״̬����
	bool bDisable=(IsWindowEnabled()==FALSE);
	COLORREF crColorBK=(bDisable==false)?m_SkinAttribute.m_crEnableBK:m_SkinAttribute.m_crDisableBK;
	COLORREF crColorBorad=(bDisable==false)?m_SkinAttribute.m_crEnableBorad:m_SkinAttribute.m_crDisableBorad;

	//�滭�߿�
	CClientDC ClientDC(this);
	DrawControlView(&ClientDC,crColorBorad,crColorBK);

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CSkinEditEx::CSkinEditEx()
{
	//������ɫ
	m_crEnableBK=m_SkinAttribute.m_crEnableBK;
	m_crDisableBK=m_SkinAttribute.m_crDisableBK;

	//������ɫ
	m_crEnableText=m_SkinAttribute.m_crEnableText;
	m_crDisableText=m_SkinAttribute.m_crDisableText;

	//�߿���ɫ
	m_crEnableBorad=m_SkinAttribute.m_crEnableBorad;
	m_crDisableBorad=m_SkinAttribute.m_crDisableBorad;

	return;
}

//��������
CSkinEditEx::~CSkinEditEx()
{
}

//������ɫ
VOID CSkinEditEx::SetEnableColor(COLORREF crEnableText, COLORREF crEnableBK, COLORREF crEnableBorad)
{
	//������ɫ
	m_crEnableBK=crEnableBK;
	m_crEnableText=crEnableText;
	m_crEnableBorad=crEnableBorad;

	//������ˢ
	m_brEnable.DeleteObject();
	m_brEnable.CreateSolidBrush(m_crEnableBK);

	//���½���
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//������ɫ
VOID CSkinEditEx::SetDisableColor(COLORREF crDisableText, COLORREF crDisableBK, COLORREF crDisableBorad)
{
	//������ɫ
	m_crDisableBK=crDisableBK;
	m_crDisableText=crDisableText;
	m_crDisableBorad=crDisableBorad;

	//������ˢ
	m_brDisable.DeleteObject();
	m_brDisable.CreateSolidBrush(m_crDisableBK);

	//���½���
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//�ػ���Ϣ
VOID CSkinEditEx::OnNcPaint()
{
	//״̬����
	bool bDisable=(IsWindowEnabled()==FALSE);
	COLORREF crColorBK=(bDisable==false)?m_crEnableBK:m_crDisableBK;
	COLORREF crColorBorad=(bDisable==false)?m_crEnableBorad:m_crDisableBorad;

	//�滭�߿�
	CClientDC ClientDC(this);
	DrawControlView(&ClientDC,crColorBorad,crColorBK);

	return;
}

//�滭�ؼ�
HBRUSH CSkinEditEx::CtlColor(CDC * pDC, UINT nCtlColor)
{
	//���û���
	if (IsWindowEnabled()==FALSE)
	{
		//����״̬
		pDC->SetBkColor((m_crDisableBK==CLR_INVALID)?m_SkinAttribute.m_crDisableBK:m_crDisableBK);
		pDC->SetTextColor((m_crDisableText==CLR_INVALID)?m_SkinAttribute.m_crDisableText:m_crDisableText);
		return (m_brDisable.GetSafeHandle()==NULL)?m_SkinAttribute.m_brDisable:m_brDisable;
	}
	else
	{
		//����״̬
		pDC->SetBkColor((m_crEnableBK==CLR_INVALID)?m_SkinAttribute.m_crEnableBK:m_crEnableBK);
		pDC->SetTextColor((m_crEnableText==CLR_INVALID)?m_SkinAttribute.m_crEnableText:m_crEnableText);
		return (m_brEnable.GetSafeHandle()==NULL)?m_SkinAttribute.m_brEnable:m_brEnable;
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////////////
