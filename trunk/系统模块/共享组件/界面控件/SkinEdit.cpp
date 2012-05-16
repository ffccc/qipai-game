#include "Stdafx.h"
#include "SkinEdit.h"

//////////////////////////////////////////////////////////////////////////

//��̬����
CSkinEditAttribute					CSkinEdit::m_SkinAttribute;				//��������

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CSkinEdit, CEdit)

BEGIN_MESSAGE_MAP(CSkinEdit, CEdit)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CSkinEditAttribute::CSkinEditAttribute() 
{
	//��ʼ������
	m_crFocusTX=RGB(0,0,0);
	m_crFocusBK=RGB(0,0,0);
	m_crNoFocusTX=RGB(0,0,0);
	m_crNoFocusBK=RGB(0,0,0);
	m_crDisFocusTX=RGB(0,0,0);
	m_crDisFocusBK=RGB(0,0,0);

	return;
}

//��������
CSkinEditAttribute::~CSkinEditAttribute() 
{
}

//��������
bool CSkinEditAttribute::UpdateSkinResource(ISkinResource * pISkinResource)
{
	//��ȡ��Ϣ
	tagSkinEditResource SkinEditResource;
	if (pISkinResource!=NULL) pISkinResource->GetResourceInfo(SkinEditResource);

	//���ñ���
	m_crFocusTX=SkinEditResource.crFocusTX;
	m_crFocusBK=SkinEditResource.crFocusBK;
	m_crNoFocusTX=SkinEditResource.crNoFocusTX;
	m_crNoFocusBK=SkinEditResource.crNoFocusBK;
	m_crDisFocusTX=SkinEditResource.crDisFocusTX;
	m_crDisFocusBK=SkinEditResource.crDisFocusBK;

	//������ˢ
	m_brFocus.DeleteObject();
	m_brNoFocus.DeleteObject();
	m_brDisbale.DeleteObject();
	m_brFocus.CreateSolidBrush(m_crFocusBK);
	m_brNoFocus.CreateSolidBrush(m_crNoFocusBK);

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CSkinEdit::CSkinEdit()
{
	m_bFocus=false;
}

//��������
CSkinEdit::~CSkinEdit()
{
}

//�滭�ؼ�
HBRUSH CSkinEdit::CtlColor(CDC * pDC, UINT nCtlColor)
{
	pDC->SetBkMode(OPAQUE);
	if (IsWindowEnabled())
	{
		if (m_bFocus) 
		{
			pDC->SetTextColor(m_SkinAttribute.m_crFocusTX);
			pDC->SetBkColor(m_SkinAttribute.m_crFocusBK);
			return m_SkinAttribute.m_brFocus;
		}
		pDC->SetTextColor(m_SkinAttribute.m_crNoFocusTX);
		pDC->SetBkColor(m_SkinAttribute.m_crNoFocusBK);
		return m_SkinAttribute.m_brNoFocus;
	}
	pDC->SetTextColor(m_SkinAttribute.m_crDisFocusBK);
	pDC->SetBkColor(m_SkinAttribute.m_crDisFocusBK);
	return m_SkinAttribute.m_brDisbale;
}

//�õ�����
void CSkinEdit::OnSetFocus(CWnd * pOldWnd)
{
	m_bFocus=true;
	Invalidate(TRUE);
	__super::OnSetFocus(pOldWnd);
}

//ʧȥ����
void CSkinEdit::OnKillFocus(CWnd * pNewWnd)
{
	m_bFocus=false;
	Invalidate(TRUE);
	__super::OnKillFocus(pNewWnd);
}

//////////////////////////////////////////////////////////////////////////
