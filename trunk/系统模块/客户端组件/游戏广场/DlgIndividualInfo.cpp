#include "Stdafx.h"
#include "Resource.h"
#include "GlobalUnits.h"
#include "DlgIndividualInfo.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgIndividualInfo, CSkinDialogEx)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//���캯��
CDlgIndividualInfo::CDlgIndividualInfo() : CSkinDialogEx(IDD_MODIFY_INDIVIDUAL)
{

	return;
}

//��������
CDlgIndividualInfo::~CDlgIndividualInfo()
{
}

//�ؼ���
void CDlgIndividualInfo::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
}

//��ʼ������
BOOL CDlgIndividualInfo::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("�û�������Ϣ"));

	//���ؼ�
	m_BrowerAD.Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this,100,NULL);
	m_BrowerAD.Navigate(TEXT("http://221.234.25.72/user/"),NULL,NULL,NULL,NULL);

	//���λ��
	int nXExcursion=GetXExcursionPos();
	int nYExcursion=GetYExcursionPos();
	m_BrowerAD.MoveWindow(nXExcursion+4,nYExcursion+5,327,248);

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////

