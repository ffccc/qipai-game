#include "StdAfx.h"
#include "GlobalUnits.h"
#include "DlgCollocate.h"
#include "PlatformFrame.h"

//////////////////////////////////////////////////////////////////////////////////

//��ť��Ŀ
#define BUTTON_COUNT_CX				3									//������Ŀ
#define BUTTON_COUNT_CY				5									//������Ŀ

//�ߴ綨��
#define COLLOCATE_BAR_CX			150									//���ƿ��
#define COLLOCATE_BAR_CY			330									//���Ƹ߶� 

//�ߴ綨��
#define BAR_BT_SPACE				3									//��ť���
#define BAR_BT_EXCURSION			5									//��ťƫ��

//�ؼ���ʶ
#define IDC_COLLOCATE_BAR			100									//�������

//��ťλ��
#define IMAGE_INDEX_NOTIFY			0									//��Ϣ����
#define IMAGE_INDEX_CUSTOM			1									//��������
#define IMAGE_INDEX_SYSTEM			2									//ϵͳ����
#define IMAGE_INDEX_PROXY			3									//��������
#define IMAGE_INDEX_SERVER			4									//��������

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CCollocateBar, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CCollocateItem, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CCollocateProxy, CCollocateItem)
	ON_BN_CLICKED(IDC_PROXY_TEST, OnBnClickedProxyTest)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CDlgCollocate, CSkinDialog)
	ON_BN_CLICKED(IDC_DEFAULT, OnBnClickedDefault)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CCollocateBar::CCollocateBar()
{
	//���Ϣ
	m_bHovering=false;
	m_wItemDown=INVALID_WORD;
	m_wItemHover=INVALID_WORD;
	m_wItemActive=INVALID_WORD;

	//������Ϣ
	m_SizeButton.SetSize(0,0);
	m_rcItemCreate.SetRect(0,0,0,0);

	//������Դ
	CPngImage ImageButton;
	ImageButton.LoadImage(AfxGetInstanceHandle(),TEXT("COLLOCATE_BAR_BT"));
	m_SizeButton.SetSize(ImageButton.GetWidth()/BUTTON_COUNT_CX,ImageButton.GetHeight()/BUTTON_COUNT_CY);

	return;
}

//��������
CCollocateBar::~CCollocateBar()
{
}

//��Ϣ����
BOOL CCollocateBar::PreTranslateMessage(MSG * pMsg)
{
	//��Ϣ����
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
        GetParent()->PostMessage(WM_CLOSE,0,0);
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//�������
bool CCollocateBar::SaveParameter()
{
	//�������
	if (m_wItemActive!=INVALID_WORD)
	{
		CCollocateItem * pCollocateItem=m_CollocateItemArray[m_wItemActive];
		if (pCollocateItem->SaveParameter()==false) return false;
	}

	return true;
}

//Ĭ�ϲ���
bool CCollocateBar::DefaultParameter()
{
	//��������
	for (INT_PTR i=0;i<m_CollocateItemArray.GetCount();i++)
	{
		//��ȡ����
		CCollocateItem * pCollocateItem=m_CollocateItemArray[i];

		//Ĭ�ϲ���
		if (pCollocateItem->m_hWnd!=NULL) 
		{
			pCollocateItem->DefaultParameter();
			pCollocateItem->UpdateControlStatus();
		}
	}

	return true;
}

//��������
bool CCollocateBar::SetItemCreateRect(CRect rcItemCreate)
{
	//���ñ���
	m_rcItemCreate=rcItemCreate;

	//�����ؼ�
	for (INT_PTR i=0;i<m_CollocateItemArray.GetCount();i++)
	{
		CWnd * pWnd=m_CollocateItemArray[i];
		if (pWnd->m_hWnd!=NULL) pWnd->SetWindowPos(NULL,rcItemCreate.left,rcItemCreate.top,rcItemCreate.Width(),rcItemCreate.Height(),SWP_NOZORDER);
	}

	return true;
}

//��������
bool CCollocateBar::InsertCollocateItem(CCollocateItem * pCollocateItem)
{
	//���ñ���
	m_CollocateItemArray.Add(pCollocateItem);

	//���½���
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return true;
}

//��������
bool CCollocateBar::SetActiveCollocateItem(CCollocateItem * pCollocateItem)
{
	//Ч�����
	ASSERT(pCollocateItem!=NULL);
	if (pCollocateItem==NULL) return false;

	//��������
	CCollocateItem * pCollocateActive=NULL;

	//�л��ж�
	if (m_wItemActive!=INVALID_WORD)
	{
		//��ȡ����
		ASSERT(m_wItemActive<m_CollocateItemArray.GetCount());
		pCollocateActive=m_CollocateItemArray[m_wItemActive];

		//�л��ж�
		if (pCollocateActive==pCollocateItem) return true;
		if (pCollocateActive->SaveParameter()==false) return false;
	}

	//��������
	for (INT_PTR i=0;i<m_CollocateItemArray.GetCount();i++)
	{
		if (m_CollocateItemArray[i]==pCollocateItem)
		{
			//��������
			if (pCollocateItem->m_hWnd==NULL)
			{
				pCollocateItem->CreateCollocateItem(GetParent(),m_rcItemCreate);
				pCollocateItem->UpdateControlStatus();
			}

			//���ñ���
			m_wItemActive=(WORD)i;

			//��ʾ����
			pCollocateItem->ShowWindow(SW_SHOW);
			pCollocateItem->SetFocus();

			//���ؾ���
			if (pCollocateActive->GetSafeHwnd()!=NULL)
			{
				pCollocateActive->ShowWindow(SW_HIDE);
			}

			//���½���
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);

			return true;
		}
	}

	return false;
}

//�����л�
WORD CCollocateBar::SwitchToButtonIndex(CPoint MousePoint)
{
	//����λ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//�߽��ж�
	if (MousePoint.x<((rcClient.Width()-m_SizeButton.cx)/2)) return INVALID_WORD;
	if (MousePoint.x>((rcClient.Width()+m_SizeButton.cx)/2)) return INVALID_WORD;

	//�����ж�
	for (WORD i=0;i<m_CollocateItemArray.GetCount();i++)
	{
		//λ�ü���
		CRect rcButton;
		rcButton.left=(rcClient.Width()-m_SizeButton.cx)/2;
		rcButton.right=(rcClient.Width()+m_SizeButton.cx)/2;
		rcButton.top=(m_SizeButton.cy+BAR_BT_SPACE)*i+BAR_BT_EXCURSION;
		rcButton.bottom=(m_SizeButton.cy+BAR_BT_SPACE)*i+m_SizeButton.cy+BAR_BT_EXCURSION;

		//�����ж�
		if (rcButton.PtInRect(MousePoint)) return i;
	}

	return INVALID_WORD;
}

//�ػ���Ϣ
VOID CCollocateBar::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CBitImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//�����豸
	CImageDC BufferDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	//�����豸
	pBufferDC->SetBkMode(TRANSPARENT);
	pBufferDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//������Դ
	CPngImage ImageButton;
	CPngImage ImageBarLogo;
	ImageButton.LoadImage(AfxGetInstanceHandle(),TEXT("COLLOCATE_BAR_BT"));
	ImageBarLogo.LoadImage(AfxGetInstanceHandle(),TEXT("COLLOCATE_BAR_LOGO"));

	//�滭����
	pBufferDC->FillSolidRect(&rcClient,RGB(223,236,246));
	ImageBarLogo.DrawImage(pBufferDC,rcClient.Width()-ImageBarLogo.GetWidth(),rcClient.Height()-ImageBarLogo.GetHeight());

	//�滭��ť
	for (INT i=0;i<m_CollocateItemArray.GetCount();i++)
	{
		//��ȡ����
		ASSERT(m_CollocateItemArray[i]!=NULL);
		CCollocateItem * pCollocateItem=m_CollocateItemArray[i];

		//��������
		INT nXButtomPos=0;
		INT nYButtomPos=pCollocateItem->m_cbImageIndex*m_SizeButton.cy;

		//���м���
		if ((m_wItemHover==i)&&(m_wItemDown!=i)) nXButtomPos=m_SizeButton.cx*2;
		if ((m_wItemActive==i)||(m_wItemDown==i)) nXButtomPos=m_SizeButton.cx;

		//�滭��ť
		INT nXDrawPos=(rcClient.Width()-m_SizeButton.cx)/2;
		INT nYDrawPos=(m_SizeButton.cy+BAR_BT_SPACE)*i+BAR_BT_EXCURSION;
		ImageButton.DrawImage(pBufferDC,nXDrawPos,nYDrawPos,m_SizeButton.cx,m_SizeButton.cy,nXButtomPos,nYButtomPos);
	}

	//�滭����
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return;
}

//������Ϣ
INT CCollocateBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//���ñ���
	m_bHovering=false;
	m_wItemDown=INVALID_WORD;
	m_wItemHover=INVALID_WORD;
	m_wItemActive=INVALID_WORD;

	//������Ϣ
	m_rcItemCreate.SetRect(0,0,0,0);
	m_CollocateItemArray.RemoveAll();

	return 0;
}

//�����Ϣ
VOID CCollocateBar::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags, Point);

	//����λ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//λ�ü���
	WORD wCollocateHover=SwitchToButtonIndex(Point);

	//��������
	if (wCollocateHover!=m_wItemHover)
	{
		//���ñ���
		m_wItemHover=wCollocateHover;

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	//�����ж�
	if (m_bHovering==false)
	{
		//���ñ���
		m_bHovering=true;

		//��������
		TRACKMOUSEEVENT TrackMouseEvent;
		ZeroMemory(&TrackMouseEvent,sizeof(TrackMouseEvent));

		//ע����Ϣ
		TrackMouseEvent.hwndTrack=m_hWnd;
		TrackMouseEvent.dwFlags=TME_LEAVE;
		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);

		//ע���¼�
		_TrackMouseEvent(&TrackMouseEvent);
	}

	return;
}

//�����Ϣ
VOID CCollocateBar::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags,Point);

	//��������
	if ((m_wItemHover!=INVALID_WORD)&&(m_wItemHover==m_wItemDown))
	{
		SetActiveCollocateItem(m_CollocateItemArray[m_wItemHover]);
	}

	//������
	if (m_wItemDown!=INVALID_WORD)
	{
		//�ͷ����
		ReleaseCapture();

		//���ñ���
		m_wItemDown=INVALID_WORD;

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//�����Ϣ
VOID CCollocateBar::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//���ý���
	SetFocus();

	//���´���
	if ((m_wItemHover!=INVALID_WORD)&&(m_wItemHover!=m_wItemActive))
	{
		//����˻�
		SetCapture();

		//���ñ���
		m_wItemDown=m_wItemHover;

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//�����Ϣ
LRESULT CCollocateBar::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	if (m_bHovering==true)
	{
		//���ñ���
		m_bHovering=false;
		m_wItemHover=INVALID_WORD;

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return 0;
}

//�����Ϣ
BOOL CCollocateBar::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//���ù��
	if (m_wItemHover!=INVALID_WORD)
	{
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_HAND_CUR)));
		return TRUE;
	}

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CCollocateItem::CCollocateItem(UINT uIDTemplate) : CDialog(uIDTemplate)
{
	//������Դ
	m_cbImageIndex=0;

	return;
}

//��������
CCollocateItem::~CCollocateItem()
{
}

//��������
bool CCollocateItem::CreateCollocateItem(CWnd * pParentWnd, CRect rcCreate)
{
	//��������
	Create(m_lpszTemplateName,pParentWnd);

	//�ƶ�����
	SetWindowPos(NULL,rcCreate.left,rcCreate.top,rcCreate.Width(),rcCreate.Height(),SWP_NOZORDER);

	return true;
}

//������Ϣ
BOOL CCollocateItem::OnEraseBkgnd(CDC * pDC)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��䱳��
	pDC->FillSolidRect(&rcClient,CSkinDialog::m_SkinAttribute.m_crBackGround);

	return TRUE;
}

//��ɫ��Ϣ
HBRUSH CCollocateItem::OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
	case CTLCOLOR_STATIC:
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetBkColor(RGB(255,255,255));
			pDC->SetTextColor(RGB(0,0,0));
			return CSkinDialog::m_SkinAttribute.m_brBackGround;
		}
	}

	return __super::OnCtlColor(pDC,pWnd,nCtlColor);
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CCollocateServer::CCollocateServer() : CCollocateItem(IDD_COLLOCATE_SERVER)
{
	//Ĭ�ϲ���
	DefaultParameter();

	//���ñ���
	m_cbImageIndex=IMAGE_INDEX_SERVER;

	return;
}

//��������
CCollocateServer::~CCollocateServer()
{
}

//�ؼ���
VOID CCollocateServer::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WIN_RATE, m_edWinRate);
	DDX_Control(pDX, IDC_FLEE_RATE, m_edFleeRate);
	DDX_Control(pDX, IDC_GAME_SCORE_MIN, m_edScoreMin);
	DDX_Control(pDX, IDC_GAME_SCORE_MAX, m_edScoreMax);
	DDX_Control(pDX, IDC_TABLE_PASSWORD, m_edPassword);
}

//���ú���
BOOL CCollocateServer::OnInitDialog()
{
	__super::OnInitDialog();

	//��������
	((CEdit *)GetDlgItem(IDC_WIN_RATE))->LimitText(5);
	((CEdit *)GetDlgItem(IDC_FLEE_RATE))->LimitText(5);
	((CEdit *)GetDlgItem(IDC_GAME_SCORE_MAX))->LimitText(10);
	((CEdit *)GetDlgItem(IDC_GAME_SCORE_MIN))->LimitText(11);
	((CEdit *)GetDlgItem(IDC_TABLE_PASSWORD))->LimitText(LEN_PASSWORD-1);

	return TRUE;
}

//�������
bool CCollocateServer::SaveParameter()
{
	//��ȡ����
	m_bLimitDetest=(IsDlgButtonChecked(IDC_LIMIT_DETEST)==BST_CHECKED)?true:false;
	m_bLimitSameIP=(IsDlgButtonChecked(IDC_LIMIT_SAME_IP)==BST_CHECKED)?true:false;
	m_bTakePassword=(IsDlgButtonChecked(IDC_TAKE_PASSWORD)==BST_CHECKED)?true:false;
	m_bLimitWinRate=(IsDlgButtonChecked(IDC_LIMIT_WIN_RATE)==BST_CHECKED)?true:false;
	m_bLimitFleeRate=(IsDlgButtonChecked(IDC_LIMIT_FLEE_RATE)==BST_CHECKED)?true:false;
	m_bLimitGameScore=(IsDlgButtonChecked(IDC_LIMIT_GAME_SCORE)==BST_CHECKED)?true:false;

	//���ʤ��
	TCHAR szBuffer[128]=TEXT("");
	GetDlgItemText(IDC_WIN_RATE,szBuffer,CountArray(szBuffer));
	m_wMinWinRate=(WORD)(_tstof(szBuffer)*100.0)%10000;

	//�������
	GetDlgItemText(IDC_FLEE_RATE,szBuffer,CountArray(szBuffer));
	m_wMaxFleeRate=(WORD)(_tstof(szBuffer)*100.0)%10000;

	//��������
	m_lMaxGameScore=GetDlgItemInt(IDC_GAME_SCORE_MAX,NULL,TRUE);
	m_lMinGameScore=GetDlgItemInt(IDC_GAME_SCORE_MIN,NULL,TRUE);
	if ((m_bLimitGameScore)&&(m_lMinGameScore>=m_lMaxGameScore))
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("�������Ʒ�Χ��������ȷ������ȷ���û������Ʒ�Χ��"),MB_ICONINFORMATION);

		//���ý���
		GetDlgItem(IDC_GAME_SCORE_MAX)->SetFocus();

		return false;
	}

	//Я������
	GetDlgItemText(IDC_TABLE_PASSWORD,m_szPassword,CountArray(m_szPassword));
	if ((m_bTakePassword==true)&&(m_szPassword[0]==0))
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("����Я������û�����ã���������������Я�����룡"),MB_ICONINFORMATION);

		//���ý���
		GetDlgItem(IDC_TABLE_PASSWORD)->SetFocus();

		return false;
	}

	return true;
}

//Ĭ�ϲ���
bool CCollocateServer::DefaultParameter()
{
	//ʤ������
	m_wMinWinRate=0;
	m_bLimitWinRate=false;

	//��������
	m_wMaxFleeRate=5000;
	m_bLimitFleeRate=false;

	//��������
	m_bLimitGameScore=false;
	m_lMaxGameScore=2147483647L;
	m_lMinGameScore=-2147483646L;

	//��������
	m_bLimitDetest=true;
	m_bLimitSameIP=true;

	//Я������
	m_bTakePassword=false;
	ZeroMemory(m_szPassword,sizeof(m_szPassword));

	return true;
}

//���¿���
bool CCollocateServer::UpdateControlStatus()
{
	//���ÿؼ�
	CheckDlgButton(IDC_LIMIT_DETEST,(m_bLimitDetest==true)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_LIMIT_SAME_IP,(m_bLimitSameIP==true)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_TAKE_PASSWORD,(m_bTakePassword==true)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_LIMIT_WIN_RATE,(m_bLimitWinRate==true)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_LIMIT_FLEE_RATE,(m_bLimitFleeRate==true)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_LIMIT_GAME_SCORE,(m_bLimitGameScore==true)?BST_CHECKED:BST_UNCHECKED);

	//���ʤ��
	TCHAR szBuffer[128]=TEXT("");
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("%.2f"),((DOUBLE)(m_wMinWinRate))/100.0);
	SetDlgItemText(IDC_WIN_RATE,szBuffer);

	//�������
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("%.2f"),((DOUBLE)(m_wMaxFleeRate))/100.0);
	SetDlgItemText(IDC_FLEE_RATE,szBuffer);

	//��������
	SetDlgItemInt(IDC_GAME_SCORE_MAX,m_lMaxGameScore,TRUE);
	SetDlgItemInt(IDC_GAME_SCORE_MIN,m_lMinGameScore,TRUE);

	//Я������
	SetDlgItemText(IDC_TABLE_PASSWORD,m_szPassword);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CCollocateProxy::CCollocateProxy() : CCollocateItem(IDD_COLLOCATE_PROXY)
{
	//Ĭ�ϲ���
	DefaultParameter();

	//���ñ���
	m_cbImageIndex=IMAGE_INDEX_PROXY;

	return;
}

//��������
CCollocateProxy::~CCollocateProxy()
{
}

//�ؼ���
VOID CCollocateProxy::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROXY_TYPE, m_cmProxyType);
	DDX_Control(pDX, IDC_PROXY_TEST, m_btProxyTest);
	DDX_Control(pDX, IDC_PROXY_PORT, m_edProxyPort);
	DDX_Control(pDX, IDC_PROXY_USER, m_edProxyUserName);
	DDX_Control(pDX, IDC_PROXY_PASS, m_edProxyPassword);
	DDX_Control(pDX, IDC_PROXY_SERVER, m_edProxyServer);
}

//��������
BOOL CCollocateProxy::OnInitDialog()
{
	__super::OnInitDialog();

	//��������
	((CEdit *)(GetDlgItem(IDC_PROXY_PORT)))->LimitText(5);
	((CEdit *)(GetDlgItem(IDC_PROXY_USER)))->LimitText(31);
	((CEdit *)(GetDlgItem(IDC_PROXY_PASS)))->LimitText(31);
	((CEdit *)(GetDlgItem(IDC_PROXY_SERVER)))->LimitText(31);

	//��������
	BYTE cbProxyType[]={PROXY_NONE,PROXY_HTTP,PROXY_SOCKS4,PROXY_SOCKS5};
	LPCTSTR szProxyType[]={TEXT("��ʹ�ô���"),TEXT("HTTP ����"),TEXT("SOCKS 4"),TEXT("SOCKS 5")};

	//��������
	for (INT i=0;i<CountArray(szProxyType);i++)
	{
		INT nItem=m_cmProxyType.AddString(szProxyType[i]);
		m_cmProxyType.SetItemData(nItem,cbProxyType[i]);
		if (m_cbProxyType==cbProxyType[i]) m_cmProxyType.SetCurSel(nItem);
	}

	//������Ϣ
	SetDlgItemText(IDC_PROXY_USER,m_ProxyServer.szUserName);
	SetDlgItemText(IDC_PROXY_PASS,m_ProxyServer.szPassword);
	SetDlgItemText(IDC_PROXY_SERVER,m_ProxyServer.szProxyServer);
	if (m_ProxyServer.wProxyPort!=0) SetDlgItemInt(IDC_PROXY_PORT,m_ProxyServer.wProxyPort);

	//����ѡ��
	if (m_cmProxyType.GetCurSel()==CB_ERR) m_cmProxyType.SetCurSel(0);

	return FALSE;
}

//�������
bool CCollocateProxy::SaveParameter()
{
	//��������
	m_cbProxyType=(BYTE)m_cmProxyType.GetItemData(m_cmProxyType.GetCurSel());

	//������Ϣ
	m_ProxyServer.wProxyPort=GetDlgItemInt(IDC_PROXY_PORT);
	GetDlgItemText(IDC_PROXY_USER,m_ProxyServer.szUserName,CountArray(m_ProxyServer.szUserName));
	GetDlgItemText(IDC_PROXY_PASS,m_ProxyServer.szPassword,CountArray(m_ProxyServer.szPassword));
	GetDlgItemText(IDC_PROXY_SERVER,m_ProxyServer.szProxyServer,CountArray(m_ProxyServer.szProxyServer));

	//Ч�����
	if (m_cbProxyType!=PROXY_NONE)
	{
		//�����ַ
		if (m_ProxyServer.szProxyServer[0]==0)
		{
			//������ʾ
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("�����������ַ����Ϊ�գ����������룡"),MB_ICONERROR,30);

			//���ý���
			m_edProxyServer.SetFocus();

			return false;
		}

		//����˿�
		if (m_ProxyServer.wProxyPort==0)
		{
			//������ʾ
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("���������������˿ںţ����磺1080��"),MB_ICONERROR,30);

			//���ý���
			m_edProxyPort.SetFocus();

			return false;
		}
	}

	return true;
}

//Ĭ�ϲ���
bool CCollocateProxy::DefaultParameter()
{
	//���ñ���
	m_cbProxyType=PROXY_NONE;
	ZeroMemory(&m_ProxyServer,sizeof(m_ProxyServer));

	return true;
}

//���¿���
bool CCollocateProxy::UpdateControlStatus()
{
	//������Ϣ
	SetDlgItemText(IDC_PROXY_USER,m_ProxyServer.szUserName);
	SetDlgItemText(IDC_PROXY_PASS,m_ProxyServer.szPassword);
	SetDlgItemText(IDC_PROXY_SERVER,m_ProxyServer.szProxyServer);

	//����˿�
	if (m_ProxyServer.wProxyPort==0)
	{
		SetDlgItemText(IDC_PROXY_PORT,TEXT(""));
	}
	else
	{
		SetDlgItemInt(IDC_PROXY_PORT,m_ProxyServer.wProxyPort);
	}

	//����ѡ��
	for (INT i=0;i<m_cmProxyType.GetCount();i++)
	{
		if (m_cmProxyType.GetItemData(i)==m_cbProxyType)
		{
			m_cmProxyType.SetCurSel(i);
		}
	}

	//Ĭ��ѡ��
	if (i==m_cmProxyType.GetCount())
	{
		m_cmProxyType.SetCurSel(0);
	}

	return true;
}

//�������
VOID CCollocateProxy::OnBnClickedProxyTest()
{
	//��������
	BYTE cbProxyType=(BYTE)m_cmProxyType.GetItemData(m_cmProxyType.GetCurSel());

	//������Ϣ
	tagProxyServer ProxyServer;
	ZeroMemory(&ProxyServer,sizeof(ProxyServer));
	ProxyServer.wProxyPort=GetDlgItemInt(IDC_PROXY_PORT);
	GetDlgItemText(IDC_PROXY_USER,ProxyServer.szUserName,CountArray(ProxyServer.szUserName));
	GetDlgItemText(IDC_PROXY_PASS,ProxyServer.szPassword,CountArray(ProxyServer.szPassword));
	GetDlgItemText(IDC_PROXY_SERVER,ProxyServer.szProxyServer,CountArray(ProxyServer.szProxyServer));

	//Ч�����
	if (cbProxyType!=PROXY_NONE)
	{
		//�����ַ
		if (ProxyServer.szProxyServer[0]==0)
		{
			//������ʾ
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("�����������ַ����Ϊ�գ����������룡"),MB_ICONERROR,30);

			//���ý���
			m_edProxyServer.SetFocus();

			return;
		}

		//����˿�
		if (ProxyServer.wProxyPort==0)
		{
			//������ʾ
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("���������������˿ںţ����磺1080��"),MB_ICONERROR,30);

			//���ý���
			m_edProxyPort.SetFocus();

			return;
		}
	}
	else 
	{
		//������ʾ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("����ѡ������������������������������Ϣ��"),MB_ICONERROR,30);

		//���ý���
		m_cmProxyType.SetFocus();

		return;
	}

	//�������
	CWHNetworkHelper WHNetworkModule;
	if (WHNetworkModule.CreateInstance()==false)
	{
		ShowInformation(TEXT("�����������������ʧ�ܣ�����ʧ�ܣ�"),MB_ICONERROR,30);
		return;
	}

	//�������
	switch (WHNetworkModule->ProxyServerTesting(cbProxyType,ProxyServer))
	{
	case CONNECT_SUCCESS:				//���ӳɹ�
		{
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("�������������������"),MB_ICONINFORMATION);
			return;
		}
	case CONNECT_PROXY_USER_INVALID:	//�û�����
		{
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("����������û��������������"),MB_ICONERROR);
			return;
		}
	default:							//Ĭ�ϴ���
		{
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("��������������ڻ�������ʧ�ܣ�"),MB_ICONERROR);
			return;
		}
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CCollocateNotify::CCollocateNotify() : CCollocateItem(IDD_COLLOCATE_NOTIFY)
{
	//Ĭ�ϲ���
	DefaultParameter();

	//���ñ���
	m_cbImageIndex=IMAGE_INDEX_NOTIFY;

	return;
}

//��������
CCollocateNotify::~CCollocateNotify()
{
}

//�������
bool CCollocateNotify::SaveParameter()
{
	//��������
	if (IsDlgButtonChecked(IDC_MESSAGE_SHOW_ALL)==BST_CHECKED) m_cbMessageMode=INVITE_MODE_ALL;
	if (IsDlgButtonChecked(IDC_MESSAGE_HIDE_DETEST)==BST_CHECKED) m_cbMessageMode=MESSAGE_MODE_DETEST;
	if (IsDlgButtonChecked(IDC_MESSAGE_ONLY_FRIEND)==BST_CHECKED) m_cbMessageMode=MESSAGE_MODE_FRIEND;

	//��������
	if (IsDlgButtonChecked(IDC_INVITE_ALL)==BST_CHECKED) m_cbInviteMode=INVITE_MODE_ALL;
	if (IsDlgButtonChecked(IDC_INVITE_NONE)==BST_CHECKED) m_cbInviteMode=INVITE_MODE_NONE;
	if (IsDlgButtonChecked(IDC_INVITE_ONLY_FRIEND)==BST_CHECKED) m_cbInviteMode=INVITE_MODE_FRIEND;

	//˽������
	m_bAutoShowWhisper=(IsDlgButtonChecked(IDC_AUTO_SHOW_WHISPER)==BST_CHECKED);
	m_wWhisperHotKey=(WORD)((CHotKeyCtrl *)GetDlgItem(IDC_WHISPER_HOTKEY))->GetHotKey();

	return true;
}

//Ĭ�ϲ���
bool CCollocateNotify::DefaultParameter()
{
	//���ñ���
	m_bAutoShowWhisper=false;
	m_wWhisperHotKey=MAKEWORD(VK_F11,0);

	//ö�ٱ���
	m_cbInviteMode=INVITE_MODE_ALL;
	m_cbMessageMode=MESSAGE_MODE_DETEST;

	return true;
}

//���¿���
bool CCollocateNotify::UpdateControlStatus()
{
	//��������
	CheckDlgButton(IDC_MESSAGE_SHOW_ALL,(m_cbMessageMode==INVITE_MODE_ALL)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_MESSAGE_HIDE_DETEST,(m_cbMessageMode==MESSAGE_MODE_DETEST)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_MESSAGE_ONLY_FRIEND,(m_cbMessageMode==MESSAGE_MODE_FRIEND)?BST_CHECKED:BST_UNCHECKED);

	//��������
	CheckDlgButton(IDC_INVITE_ALL,(m_cbInviteMode==INVITE_MODE_ALL)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_INVITE_NONE,(m_cbInviteMode==INVITE_MODE_NONE)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_INVITE_ONLY_FRIEND,(m_cbInviteMode==INVITE_MODE_FRIEND)?BST_CHECKED:BST_UNCHECKED);

	//˽������
	CheckDlgButton(IDC_AUTO_SHOW_WHISPER,(m_bAutoShowWhisper==true)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_SHOW_WHISPER_BY_HOTKEY,(m_bAutoShowWhisper==false)?BST_CHECKED:BST_UNCHECKED);
	((CHotKeyCtrl *)GetDlgItem(IDC_WHISPER_HOTKEY))->SetHotKey(LOBYTE(m_wWhisperHotKey),HIBYTE(m_wWhisperHotKey));

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CCollocateCustom::CCollocateCustom() : CCollocateItem(IDD_COLLOCATE_CUSTOM)
{
	//Ĭ�ϲ���
	DefaultParameter();

	//���ñ���
	m_cbImageIndex=IMAGE_INDEX_CUSTOM;

	return;
}

//��������
CCollocateCustom::~CCollocateCustom()
{
}

//�������
bool CCollocateCustom::SaveParameter()
{
	//˫���б�
	if (IsDlgButtonChecked(IDC_ACTION_ORIENTATION)==BST_CHECKED) m_cbActionLeftDoubleList=ACTION_ORIENTATION;
	if (IsDlgButtonChecked(IDC_ACTION_SEND_WHISPER)==BST_CHECKED) m_cbActionLeftDoubleList=ACTION_SEND_WHISPER;
	if (IsDlgButtonChecked(IDC_ACTION_SHOW_USER_INFO)==BST_CHECKED) m_cbActionLeftDoubleList=ACTION_SHOW_USER_INFO;

	//�Զ�����
	if (IsDlgButtonChecked(IDC_ACTION_SEARCH_TABLE)==BST_CHECKED) m_cbActionHitAutoJoin=ACTION_SEARCH_TABLE;
	if (IsDlgButtonChecked(IDC_SHOW_SEARCH_TABLE_DLG)==BST_CHECKED) m_cbActionHitAutoJoin=ACTION_SHOW_SEARCH_DLG;

	//����˽��
	if (IsDlgButtonChecked(IDC_SNED_WHISPER_BY_ENTER)==BST_CHECKED) m_bSendWhisperByEnter=true;
	if (IsDlgButtonChecked(IDC_SNED_WHISPER_BY_CTRL_ENTER)==BST_CHECKED) m_bSendWhisperByEnter=false;

	return true;
}

//Ĭ�ϲ���
bool CCollocateCustom::DefaultParameter()
{
	//���ñ���
	m_bSendWhisperByEnter=true;

	//��������
	m_cbActionHitAutoJoin=ACTION_SEARCH_TABLE;
	m_cbActionLeftDoubleList=ACTION_SEND_WHISPER;

	return true;
}

//���¿���
bool CCollocateCustom::UpdateControlStatus()
{
	//˫���б�
	CheckDlgButton(IDC_ACTION_ORIENTATION,(m_cbActionLeftDoubleList==ACTION_ORIENTATION)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_ACTION_SEND_WHISPER,(m_cbActionLeftDoubleList==ACTION_SEND_WHISPER)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_ACTION_SHOW_USER_INFO,(m_cbActionLeftDoubleList==ACTION_SHOW_USER_INFO)?BST_CHECKED:BST_UNCHECKED);

	//�Զ�����
	CheckDlgButton(IDC_ACTION_SEARCH_TABLE,(m_cbActionHitAutoJoin==ACTION_SEARCH_TABLE)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_SHOW_SEARCH_TABLE_DLG,(m_cbActionHitAutoJoin==ACTION_SHOW_SEARCH_DLG)?BST_CHECKED:BST_UNCHECKED);

	//����˽��
	CheckDlgButton(IDC_SNED_WHISPER_BY_ENTER,(m_bSendWhisperByEnter==true)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_SNED_WHISPER_BY_CTRL_ENTER,(m_bSendWhisperByEnter==false)?BST_CHECKED:BST_UNCHECKED);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CCollocateSystem::CCollocateSystem() : CCollocateItem(IDD_COLLOCATE_SYSTEM)
{
	//Ĭ�ϲ���
	DefaultParameter();

	//���ñ���
	m_cbImageIndex=IMAGE_INDEX_SYSTEM;

	return;
}

//��������
CCollocateSystem::~CCollocateSystem()
{
}

//�������
bool CCollocateSystem::SaveParameter()
{
	//��������
	m_bAllowSound=(IsDlgButtonChecked(IDC_ALLOW_SOUND)==BST_CHECKED);
	m_bAutoSitDown=(IsDlgButtonChecked(IDC_AUTO_SIT_DOWN)==BST_CHECKED);
	m_bSalienceTable=(IsDlgButtonChecked(IDC_SALIENCE_TABLE)==BST_CHECKED);
	m_bSaveWhisperChat=(IsDlgButtonChecked(IDC_AUTO_SAVE_WHISPER)==BST_CHECKED);
	m_bNotifyUserInOut=(IsDlgButtonChecked(IDC_NOTIFY_USER_INOUT)==BST_CHECKED);
	m_bNotifyFriendCome=(IsDlgButtonChecked(IDC_NOTIFY_FRIEND_COME)==BST_CHECKED);
	m_bFullScreenBrowse=(IsDlgButtonChecked(IDC_FULL_SCREEN_BROWSE)==BST_CHECKED);

	//�ϰ��
	m_wBossHotKey=(WORD)((CHotKeyCtrl *)GetDlgItem(IDC_BOSS_HOTKEY))->GetHotKey();

	return true;
}

//Ĭ�ϲ���
bool CCollocateSystem::DefaultParameter()
{
	//���ñ���
	m_wBossHotKey=MAKEWORD(VK_F12,HOTKEYF_CONTROL);

	//���ñ���
	m_bAllowSound=true;
	m_bAutoSitDown=true;
	m_bSalienceTable=true;
	m_bSaveWhisperChat=true;
	m_bNotifyUserInOut=false;
	m_bNotifyFriendCome=true;
	m_bFullScreenBrowse=false;

	return true;
}

//���¿���
bool CCollocateSystem::UpdateControlStatus()
{
	//���ÿؼ�
	CheckDlgButton(IDC_ALLOW_SOUND,(m_bAllowSound==true)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_AUTO_SIT_DOWN,(m_bAutoSitDown==true)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_SALIENCE_TABLE,(m_bSalienceTable==true)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_AUTO_SAVE_WHISPER,(m_bSaveWhisperChat==true)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_NOTIFY_USER_INOUT,(m_bNotifyUserInOut==true)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_NOTIFY_FRIEND_COME,(m_bNotifyFriendCome==true)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_FULL_SCREEN_BROWSE,(m_bFullScreenBrowse==true)?BST_CHECKED:BST_UNCHECKED);
	
	//�ϰ��
	((CHotKeyCtrl *)GetDlgItem(IDC_BOSS_HOTKEY))->SetHotKey(LOBYTE(m_wBossHotKey),HIBYTE(m_wBossHotKey));

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgCollocate::CDlgCollocate() : CSkinDialog(IDD_COLLOCATE)
{
	//���ñ���
	m_pParameterGame=NULL;
	m_pParameterServer=NULL;

	return;
}

//��������
CDlgCollocate::~CDlgCollocate()
{
}

//���ò���
bool CDlgCollocate::InitCollocate(CParameterGame * pParameterGame, CParameterServer * pParameterServer)
{
	//���ñ���
	m_pParameterGame=pParameterGame;
	m_pParameterServer=pParameterServer;

	return true;
}

//�ؼ���
VOID CDlgCollocate::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_DEFAULT, m_btDefault);
}

//��������
BOOL CDlgCollocate::OnInitDialog()
{
	__super::OnInitDialog();

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//���ñ���
	SetWindowText(TEXT("ϵͳ����"));

	//��������
	INT nTViewBorder=m_SkinAttribute.m_EncircleInfoView.nTBorder;
	INT nBViewBorder=m_SkinAttribute.m_EncircleInfoView.nBBorder;
	INT nLViewBorder=m_SkinAttribute.m_EncircleInfoView.nLBorder;
	INT nRViewBorder=m_SkinAttribute.m_EncircleInfoView.nRBorder;
	INT nBFrameBorder=m_SkinAttribute.m_EncircleInfoFrame.nBBorder;
	
	//��������
	m_CollocateBar.Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this,IDC_COLLOCATE_BAR);
	m_CollocateBar.SetWindowPos(NULL,nLViewBorder,nTViewBorder,COLLOCATE_BAR_CX,COLLOCATE_BAR_CY,SWP_NOZORDER);

	//����λ��
	CRect rcItemCreate;
	rcItemCreate.top=nTViewBorder;
	rcItemCreate.left=nLViewBorder+COLLOCATE_BAR_CX;
	rcItemCreate.right=rcClient.Width()-nRViewBorder;
	rcItemCreate.bottom=nTViewBorder+COLLOCATE_BAR_CY;
	m_CollocateBar.SetItemCreateRect(rcItemCreate);

	//�ƶ���ť
	CRect rcButton;
	m_btOk.GetWindowRect(&rcButton);

	//�ƶ�����
	rcClient.bottom=nTViewBorder+nBFrameBorder+COLLOCATE_BAR_CY;
	SetWindowPos(NULL,0,0,rcClient.Width(),rcClient.Height(),SWP_NOZORDER|SWP_NOMOVE);

	//�ƶ���ť
	INT nYButtonPos=rcClient.Height()-rcButton.Height()-nBViewBorder-4;
	m_btOk.SetWindowPos(NULL,rcClient.Width()-rcButton.Width()*3-nRViewBorder-18,nYButtonPos,0,0,SWP_NOZORDER|SWP_NOSIZE);
	m_btCancel.SetWindowPos(NULL,rcClient.Width()-rcButton.Width()-nRViewBorder-8,nYButtonPos,0,0,SWP_NOZORDER|SWP_NOSIZE);
	m_btDefault.SetWindowPos(NULL,rcClient.Width()-rcButton.Width()*2-nRViewBorder-13,nYButtonPos,0,0,SWP_NOZORDER|SWP_NOSIZE);

	//��������
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//����ѡ��
	m_CollocateProxy.m_cbProxyType=pParameterGlobal->m_cbProxyType;
	m_CollocateProxy.m_ProxyServer=pParameterGlobal->m_ProxyServer;

	//��Ϣ����
	m_CollocateNotify.m_cbInviteMode=pParameterGlobal->m_cbInviteMode;
	m_CollocateNotify.m_cbMessageMode=pParameterGlobal->m_cbMessageMode;
	m_CollocateNotify.m_wWhisperHotKey=pParameterGlobal->m_wWhisperHotKey;
	m_CollocateNotify.m_bAutoShowWhisper=pParameterGlobal->m_bAutoShowWhisper;

	//ϵͳ����
	m_CollocateSystem.m_wBossHotKey=pParameterGlobal->m_wBossHotKey;
	m_CollocateSystem.m_bAllowSound=pParameterGlobal->m_bAllowSound;
	m_CollocateSystem.m_bAutoSitDown=pParameterGlobal->m_bAutoSitDown;
	m_CollocateSystem.m_bSalienceTable=pParameterGlobal->m_bSalienceTable;
	m_CollocateSystem.m_bSaveWhisperChat=pParameterGlobal->m_bSaveWhisperChat;
	m_CollocateSystem.m_bNotifyUserInOut=pParameterGlobal->m_bNotifyUserInOut;
	m_CollocateSystem.m_bNotifyFriendCome=pParameterGlobal->m_bNotifyFriendCome;
	m_CollocateSystem.m_bFullScreenBrowse=pParameterGlobal->m_bFullScreenBrowse;

	//��������
	m_CollocateCustom.m_cbActionHitAutoJoin=pParameterGlobal->m_cbActionHitAutoJoin;
	m_CollocateCustom.m_bSendWhisperByEnter=pParameterGlobal->m_bSendWhisperByEnter;
	m_CollocateCustom.m_cbActionLeftDoubleList=pParameterGlobal->m_cbActionLeftDoubleList;

	//ȫ������
	m_CollocateBar.InsertCollocateItem(&m_CollocateNotify);
	m_CollocateBar.InsertCollocateItem(&m_CollocateSystem);
	m_CollocateBar.InsertCollocateItem(&m_CollocateCustom);
	m_CollocateBar.InsertCollocateItem(&m_CollocateProxy);

	//��������
	if ((m_pParameterServer!=NULL)&&(m_pParameterGame!=NULL))
	{
		//��Ϸ����
		m_CollocateServer.m_wMinWinRate=m_pParameterGame->m_wMinWinRate;
		m_CollocateServer.m_wMaxFleeRate=m_pParameterGame->m_wMaxFleeRate;
		m_CollocateServer.m_bLimitWinRate=m_pParameterGame->m_bLimitWinRate;
		m_CollocateServer.m_lMaxGameScore=m_pParameterGame->m_lMaxGameScore;
		m_CollocateServer.m_lMinGameScore=m_pParameterGame->m_lMinGameScore;
		m_CollocateServer.m_bLimitFleeRate=m_pParameterGame->m_bLimitFleeRate;
		m_CollocateServer.m_bLimitGameScore=m_pParameterGame->m_bLimitGameScore;

		//��������
		m_CollocateServer.m_bLimitDetest=pParameterGlobal->m_bLimitDetest;
		m_CollocateServer.m_bLimitSameIP=pParameterGlobal->m_bLimitSameIP;

		//��������
		m_CollocateServer.m_bTakePassword=m_pParameterServer->m_bTakePassword;
		lstrcpyn(m_CollocateServer.m_szPassword,m_pParameterServer->m_szPassword,CountArray(m_CollocateServer.m_szPassword));

		//��������
		m_CollocateBar.InsertCollocateItem(&m_CollocateServer);
	}

	//����ѡ��
	if ((m_pParameterServer!=NULL)&&(m_pParameterGame!=NULL))
	{
		m_CollocateBar.SetActiveCollocateItem(&m_CollocateServer);
	}
	else
	{
		m_CollocateBar.SetActiveCollocateItem(&m_CollocateNotify);
	}

	return TRUE;
}

//ȷ������
VOID CDlgCollocate::OnOK()
{
	//�������
	if (m_CollocateBar.SaveParameter()==false) return;

	//��������
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//�ϰ��ȼ�
	UnregisterHotKey(AfxGetMainWnd()->GetSafeHwnd(),IDI_HOT_KEY_BOSS);
	if (CWHService::RegisterHotKey(AfxGetMainWnd()->GetSafeHwnd(),IDI_HOT_KEY_BOSS,m_CollocateSystem.m_wBossHotKey)==false)
	{
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("ϵͳ�ȼ���ͻ���ϰ��ȼ�ע��ʧ�ܣ�"));
	}

	//˽���ȼ�
	UnregisterHotKey(AfxGetMainWnd()->GetSafeHwnd(),IDI_HOT_KEY_WHISPER);
	if (CWHService::RegisterHotKey(AfxGetMainWnd()->GetSafeHwnd(),IDI_HOT_KEY_WHISPER,m_CollocateNotify.m_wWhisperHotKey)==false)
	{
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("ϵͳ�ȼ���ͻ����ȡ��Ϣ�ȼ�ע��ʧ�ܣ�"));
	}

	//ͬ������
	if ((m_pParameterServer!=NULL)&&(m_pParameterGame!=NULL))
	{
		//��������
		pParameterGlobal->m_bLimitDetest=m_CollocateServer.m_bLimitDetest;
		pParameterGlobal->m_bLimitSameIP=m_CollocateServer.m_bLimitSameIP;

		//��Ϸ����
		m_pParameterGame->m_wMinWinRate=m_CollocateServer.m_wMinWinRate;
		m_pParameterGame->m_wMaxFleeRate=m_CollocateServer.m_wMaxFleeRate;
		m_pParameterGame->m_lMaxGameScore=m_CollocateServer.m_lMaxGameScore;
		m_pParameterGame->m_lMinGameScore=m_CollocateServer.m_lMinGameScore;
		m_pParameterGame->m_bLimitWinRate=m_CollocateServer.m_bLimitWinRate;
		m_pParameterGame->m_bLimitFleeRate=m_CollocateServer.m_bLimitFleeRate;
		m_pParameterGame->m_bLimitGameScore=m_CollocateServer.m_bLimitGameScore;

		//��������
		m_pParameterServer->m_bTakePassword=m_CollocateServer.m_bTakePassword;
		lstrcpyn(m_pParameterServer->m_szPassword,m_CollocateServer.m_szPassword,CountArray(m_pParameterServer->m_szPassword));
	}

	//����ѡ��
	pParameterGlobal->m_cbProxyType=m_CollocateProxy.m_cbProxyType;
	pParameterGlobal->m_ProxyServer=m_CollocateProxy.m_ProxyServer;

	//��Ϣ����
	pParameterGlobal->m_cbInviteMode=m_CollocateNotify.m_cbInviteMode;
	pParameterGlobal->m_cbMessageMode=m_CollocateNotify.m_cbMessageMode;
	pParameterGlobal->m_wWhisperHotKey=m_CollocateNotify.m_wWhisperHotKey;
	pParameterGlobal->m_bAutoShowWhisper=m_CollocateNotify.m_bAutoShowWhisper;

	//ϵͳ����
	pParameterGlobal->m_wBossHotKey=m_CollocateSystem.m_wBossHotKey;
	pParameterGlobal->m_bAllowSound=m_CollocateSystem.m_bAllowSound;
	pParameterGlobal->m_bAutoSitDown=m_CollocateSystem.m_bAutoSitDown;
	pParameterGlobal->m_bSalienceTable=m_CollocateSystem.m_bSalienceTable;
	pParameterGlobal->m_bSaveWhisperChat=m_CollocateSystem.m_bSaveWhisperChat;
	pParameterGlobal->m_bNotifyUserInOut=m_CollocateSystem.m_bNotifyUserInOut;
	pParameterGlobal->m_bNotifyFriendCome=m_CollocateSystem.m_bNotifyFriendCome;
	pParameterGlobal->m_bFullScreenBrowse=m_CollocateSystem.m_bFullScreenBrowse;

	//��������
	pParameterGlobal->m_cbActionHitAutoJoin=m_CollocateCustom.m_cbActionHitAutoJoin;
	pParameterGlobal->m_bSendWhisperByEnter=m_CollocateCustom.m_bSendWhisperByEnter;
	pParameterGlobal->m_cbActionLeftDoubleList=m_CollocateCustom.m_cbActionLeftDoubleList;

	__super::OnOK();
}

//Ĭ�ϰ�ť
VOID CDlgCollocate::OnBnClickedDefault()
{
	//Ĭ�ϲ���
	m_CollocateBar.DefaultParameter();

	return;
}

//////////////////////////////////////////////////////////////////////////////////
