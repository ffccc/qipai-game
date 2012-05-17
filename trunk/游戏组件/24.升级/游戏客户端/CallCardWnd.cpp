#include "StdAfx.h"
#include "..\��Ϸ������\GameLogic.h"
#include "CallCardWnd.h"

//////////////////////////////////////////////////////////////////////////

//��ť��ʶ
#define IDC_CALL_NT						100								//���ư�ť
#define IDC_CALL_HEI_TAO				101								//���ư�ť
#define IDC_CALL_HONG_TAO				102								//���ư�ť
#define IDC_CALL_MEI_HUA				103								//���ư�ť
#define IDC_CALL_FANG_KUAI				104								//���ư�ť

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CCallCardWnd, CWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CCallCardWnd::CCallCardWnd()
{
}

//��������
CCallCardWnd::~CCallCardWnd()
{
}

//�����
BOOL CCallCardWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_CALL_NT:			//����
		{
			//������Ϣ
			AfxGetMainWnd()->PostMessage(IDM_CALL_CARD,COLOR_NT,COLOR_NT);

			return TRUE;
		}
	case IDC_CALL_HEI_TAO:		//����
		{
			//������Ϣ
			AfxGetMainWnd()->PostMessage(IDM_CALL_CARD,COLOR_HEI_TAO,COLOR_HEI_TAO);

			return TRUE;
		}
	case IDC_CALL_HONG_TAO:		//����
		{
			//������Ϣ
			AfxGetMainWnd()->PostMessage(IDM_CALL_CARD,COLOR_HONG_TAO,COLOR_HONG_TAO);

			return TRUE;
		}
	case IDC_CALL_MEI_HUA:		//÷��
		{
			//������Ϣ
			AfxGetMainWnd()->PostMessage(IDM_CALL_CARD,COLOR_MEI_HUA,COLOR_MEI_HUA);

			return TRUE;
		}
	case IDC_CALL_FANG_KUAI:	//����
		{
			//������Ϣ
			AfxGetMainWnd()->PostMessage(IDM_CALL_CARD,COLOR_FANG_KUAI,COLOR_FANG_KUAI);

			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//���¿���
void CCallCardWnd::UpdateCallControl(BYTE cbCallColor)
{
	//���¿ؼ�
	m_btCallNT.EnableWindow((cbCallColor&CALL_NT)?TRUE:FALSE);
	m_btCallHeiTao.EnableWindow((cbCallColor&CALL_HEI_TAO)?TRUE:FALSE);
	m_btCallHongTao.EnableWindow((cbCallColor&CALL_HONG_TAO)?TRUE:FALSE);
	m_btCallMeiHua.EnableWindow((cbCallColor&CALL_MEI_HUA)?TRUE:FALSE);
	m_btCallFangKuai.EnableWindow((cbCallColor&CALL_FANG_KUAI)?TRUE:FALSE);

	return;
}

//���¿���
void CCallCardWnd::UpdateCallControl(bool bCallNT, bool bCallHeiTao, bool bCallHongTao, bool CallMeiHua, bool bCallFangKuai)
{
	//���¿ؼ�
	m_btCallNT.EnableWindow((bCallNT==true)?TRUE:FALSE);
	m_btCallHeiTao.EnableWindow((bCallHeiTao==true)?TRUE:FALSE);
	m_btCallHongTao.EnableWindow((bCallHongTao==true)?TRUE:FALSE);
	m_btCallMeiHua.EnableWindow((CallMeiHua==true)?TRUE:FALSE);
	m_btCallFangKuai.EnableWindow((bCallFangKuai==true)?TRUE:FALSE);

	return;
}

//������Ϣ
int CCallCardWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//������ť
	CRect rcCreate(0,0,0,0);
	m_btCallNT.Create(TEXT(""),WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_CALL_NT);
	m_btCallHeiTao.Create(TEXT(""),WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_CALL_HEI_TAO);
	m_btCallHongTao.Create(TEXT(""),WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_CALL_HONG_TAO);
	m_btCallMeiHua.Create(TEXT(""),WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_CALL_MEI_HUA);
	m_btCallFangKuai.Create(TEXT(""),WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_CALL_FANG_KUAI);

	//���ð�ť
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btCallNT.SetButtonImage(IDB_CALL_NT,hInstance,false);
	m_btCallHeiTao.SetButtonImage(IDB_CALL_HEI_TAO,hInstance,false);
	m_btCallHongTao.SetButtonImage(IDB_CALL_HONG_TAO,hInstance,false);
	m_btCallMeiHua.SetButtonImage(IDB_CALL_MEI_HUA,hInstance,false);
	m_btCallFangKuai.SetButtonImage(IDB_CALL_FANG_KUAI,hInstance,false);

	//��ȡλ��
	CRect rcButton;
	m_btCallNT.GetClientRect(&rcButton);

	//�ƶ���ť
	m_btCallNT.SetWindowPos(NULL,0,0,0,0,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE);
	m_btCallHeiTao.SetWindowPos(NULL,rcButton.Width(),0,0,0,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE);
	m_btCallHongTao.SetWindowPos(NULL,rcButton.Width()*2,0,0,0,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE);
	m_btCallMeiHua.SetWindowPos(NULL,rcButton.Width()*3,0,0,0,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE);
	m_btCallFangKuai.SetWindowPos(NULL,rcButton.Width()*4,0,0,0,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE);

	//�ƶ�����
	SetWindowPos(NULL,0,0,rcButton.Width()*5,rcButton.Height(),SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
