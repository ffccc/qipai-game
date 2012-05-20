#include "StdAfx.h"
#include "GameLogic.h"
#include "ControlWnd.h"

//////////////////////////////////////////////////////////////////////////

//��ť��ʶ
#define IDC_CHI						100									//���ư�ť
#define IDC_PENG					101									//���ư�ť
#define IDC_CHIHU					102									//�Ժ���ť
#define IDC_GIVEUP					103									//������ť

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_CHI, OnChi)
	ON_BN_CLICKED(IDC_PENG, OnPeng)
	ON_BN_CLICKED(IDC_CHIHU, OnChiHu)
	ON_BN_CLICKED(IDC_GIVEUP, OnGiveUp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CControlWnd::CControlWnd()
{
	//���ñ���
	m_cbCurrentCard=0;

	//������Դ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageButtonBack.SetLoadInfo(IDB_BUTTON_BACK,hInstance);

	return;
}

//��������
CControlWnd::~CControlWnd()
{
}

//�����˿�
void CControlWnd::SetControlInfo(BYTE cbCurrentCard, BYTE cbOperateCode)
{
	//���ñ���
	m_cbCurrentCard=cbCurrentCard;

	//���ð�ť
	m_btChi.EnableWindow(((cbOperateCode&ACK_CHI)!=0)?TRUE:FALSE);
	m_btPeng.EnableWindow(((cbOperateCode&ACK_PENG)!=0)?TRUE:FALSE);	
	m_btChiHu.EnableWindow(((cbOperateCode&ACK_CHIHU)!=0)?TRUE:FALSE);
		
	//��ʾ����
	ShowWindow(SW_SHOW);

	return;
}

//���ư�ť
void CControlWnd::OnChi()
{
	AfxGetMainWnd()->PostMessage(IDM_CARD_OPERATE,ACK_CHI,m_cbCurrentCard);
	return;
}

//���ư�ť
void CControlWnd::OnPeng()
{
	AfxGetMainWnd()->PostMessage(IDM_CARD_OPERATE,ACK_PENG,m_cbCurrentCard);
	return;
}

//�Ժ���ť
void CControlWnd::OnChiHu()
{
	AfxGetMainWnd()->PostMessage(IDM_CARD_OPERATE,ACK_CHIHU,m_cbCurrentCard);
	return;
}

//������ť
void CControlWnd::OnGiveUp()
{
	AfxGetMainWnd()->PostMessage(IDM_CARD_OPERATE,ACK_NULL,m_cbCurrentCard);
	return;
}

//�ػ�����
void CControlWnd::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//�滭����
	CImageHandle HandleButtonBack(&m_ImageButtonBack);
	m_ImageButtonBack.BitBlt(dc,0,0);

	return;
}

//������Ϣ
int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//������ť
	CRect rcCreate(0,0,0,0);
	m_btGiveUp.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_GIVEUP);
	m_btChi.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_CHI);
	m_btPeng.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_PENG);
	m_btChiHu.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_CHIHU);

	//����λͼ
	m_btChi.SetButtonImage(IDB_BT_CHI,AfxGetInstanceHandle(),false);
	m_btPeng.SetButtonImage(IDB_BT_PENG,AfxGetInstanceHandle(),false);
	m_btChiHu.SetButtonImage(IDB_BT_CHIHU,AfxGetInstanceHandle(),false);
	m_btGiveUp.SetButtonImage(IDB_BT_GIVEUP,AfxGetInstanceHandle(),false);

	//�ƶ�����
	CImageHandle HandleButtonBack(&m_ImageButtonBack);
	SetWindowPos(NULL,0,0,m_ImageButtonBack.GetWidth(),m_ImageButtonBack.GetHeight(),SWP_NOZORDER|SWP_NOREPOSITION);

	//��ȡλ��
	CRect rcButton,rcClient;
	GetClientRect(&rcClient);
	m_btGiveUp.GetWindowRect(&rcButton);
	int nYPos=(rcClient.Height()-rcButton.Height())/2;
	int nSpace=(rcClient.Width()-rcButton.Width()*4)/5;

	//�ƶ���ť
	m_btChi.SetWindowPos(NULL,nSpace,nYPos,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_btPeng.SetWindowPos(NULL,nSpace*2+rcButton.Width(),nYPos,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_btChiHu.SetWindowPos(NULL,nSpace*3+rcButton.Width()*2,nYPos,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_btGiveUp.SetWindowPos(NULL,nSpace*4+rcButton.Width()*3,nYPos,0,0,SWP_NOSIZE|SWP_NOZORDER);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
