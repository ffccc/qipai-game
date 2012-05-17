#include "StdAfx.h"
#include "Winable.h"
#include "Resource.h"
#include "PasswordControl.h"

//////////////////////////////////////////////////////////////////////////////////

//λ�ö���
#define ITEM_POS_S					3									//��ť���
#define ITEM_POS_X					8									//��ťλ��
#define ITEM_POS_Y					8									//��ťλ��

//�رհ�ť
#define POS_BUTTON_X				5									//��ťλ��
#define POS_BUTTON_Y				8									//��ťλ��

//��������
#define ROW_BACK					0									//�˸�ť
#define ROW_SHIFT					1									//�л���ť
#define ROW_CAPITAL					2									//��д��ť
#define ROW_CLOSE_KEY				3									//�رհ�ť
#define LINE_FUNCTION				4									//���ܰ�ť

//�ؼ���ʶ
#define IDC_BT_KEYBOARD				100									//���̰�ť
#define IDC_ED_PASSWORD				200									//����ؼ�

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPasswordControl, CWnd)
	ON_WM_PAINT()
	ON_WM_NCPAINT()
	ON_WM_SETFOCUS()
	ON_WM_ERASEBKGND()
	ON_EN_CHANGE(IDC_ED_PASSWORD, OnEnChangePassword)
	ON_BN_CLICKED(IDC_BT_KEYBOARD, OnBnClickedKeyboard)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CPasswordKeyboard, CDialog)
	ON_WM_NCPAINT()
	ON_WM_KILLFOCUS()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CPasswordControl::CPasswordControl()
{
	//���ñ���
	m_bModify=false;
	m_bFalsity=false;
	m_bDrawBorad=true;
	m_bRenderImage=true;
	ZeroMemory(m_szPassword,sizeof(m_szPassword));

	return;
}

//��������
CPasswordControl::~CPasswordControl()
{
}

//�󶨺���
VOID CPasswordControl::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	//��ȡλ��
	CRect rcClient;
	CRect rcWindow;
	GetClientRect(&rcClient);
	GetWindowRect(&rcWindow);

	//������ť
	CRect rcButton(0,0,0,0);
	m_btKeyboard.Create(NULL,WS_CHILD|WS_VISIBLE,rcButton,this,IDC_BT_KEYBOARD);
	m_btKeyboard.SetButtonImage(IDB_BT_KEYBOARD,GetModuleHandle(SHARE_CONTROL_DLL_NAME),false,false);

	//�����ؼ�
	CRect rcEditCreate;
	rcEditCreate.top=4;
	rcEditCreate.left=4;
	rcEditCreate.bottom=16;
	rcEditCreate.right=rcClient.Width()-23;
	m_edPassword.Create(WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL|ES_PASSWORD,rcEditCreate,this,IDC_ED_PASSWORD);

	//���ÿؼ�
	m_edPassword.LimitText(LEN_PASSWORD-1);
	m_edPassword.SetEnableColor(RGB(0,0,0),RGB(255,255,255),RGB(92,100,105));
	m_edPassword.SetFont(&CSkinResourceManager::GetInstance()->GetDefaultFont());

	//�ƶ�����
	SetWindowPos(NULL,0,0,rcWindow.Width(),24,SWP_NOMOVE|SWP_NOZORDER|SWP_NOCOPYBITS);
	m_btKeyboard.SetWindowPos(NULL,rcClient.Width()-20,3,0,0,SWP_NOSIZE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOACTIVATE);

	return;
}

//�߿�����
VOID CPasswordControl::SetDrawBorad(bool bDrawBorad)
{
	//���ñ���
	m_bDrawBorad=bDrawBorad;

	//���½���
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//��������
VOID CPasswordControl::SetUserPassword(LPCTSTR pszPassword)
{
	//���ÿؼ�
	INT nPasswordLen=lstrlen(pszPassword);
	m_edPassword.SetWindowText((nPasswordLen>0)?TEXT("********"):TEXT(""));

	//���ñ���
	m_bModify=false;
	m_bFalsity=true;
	lstrcpyn(m_szPassword,pszPassword,CountArray(m_szPassword));

	return;
}

//��ȡ����
LPCTSTR CPasswordControl::GetUserPassword(TCHAR szPassword[LEN_PASSWORD])
{
	//�ؼ�����
	if (m_bModify==true)
	{
		m_bModify=false;
		m_edPassword.GetWindowText(m_szPassword,CountArray(m_szPassword));
	}

	//��������
	lstrcpyn(szPassword,m_szPassword,LEN_PASSWORD);

	return szPassword;
}

//�ػ���Ϣ
VOID CPasswordControl::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//�滭����
	if (IsWindowEnabled()==TRUE)
	{
		dc.FillSolidRect(rcClient,CSkinEdit::m_SkinAttribute.m_crEnableBK);
	}
	else
	{
		dc.FillSolidRect(rcClient,CSkinDialog::m_SkinAttribute.m_crBackGround);
	}

	return;
}

//�ػ���Ϣ
VOID CPasswordControl::OnNcPaint()
{
	//��ȡλ��
	CRect rcWindow;
	GetWindowRect(&rcWindow);
	ScreenToClient(&rcWindow);
	
	//��ɫ����
	COLORREF crBackGround=CSkinEdit::m_SkinAttribute.m_crEnableBK;
	COLORREF crFrameBorad=CSkinEdit::m_SkinAttribute.m_crEnableBorad;

	//����״̬
	if (IsWindowEnabled()==FALSE)
	{
		if (m_bRenderImage==false)
		{
			crBackGround=CSkinEdit::m_SkinAttribute.m_crDisableBK;
			crFrameBorad=CSkinEdit::m_SkinAttribute.m_crDisableBorad;
		}
		else
		{
			crBackGround=CSkinEdit::m_SkinAttribute.m_crDisableBK;
			crFrameBorad=CSkinEdit::m_SkinAttribute.m_crDisableBorad;
		}
	}
	else
	{
		crBackGround=CSkinEdit::m_SkinAttribute.m_crEnableBK;
		crFrameBorad=CSkinEdit::m_SkinAttribute.m_crEnableBorad;
	}

	//�߿��ж�
	if (m_bDrawBorad==false)
	{
		crFrameBorad=crBackGround;
	}

	//�滭�߿�
	CClientDC ClientDC(this);
	ClientDC.Draw3dRect(rcWindow.left,rcWindow.top,rcWindow.Width(),rcWindow.Height(),crFrameBorad,crFrameBorad);
	ClientDC.Draw3dRect(rcWindow.left+1,rcWindow.top+1,rcWindow.Width()-2,rcWindow.Height()-2,crBackGround,crBackGround);

	return;

}

//�滭����
BOOL CPasswordControl::OnEraseBkgnd(CDC * pDC)
{
	return TRUE;
}

//��ȡ����
VOID CPasswordControl::OnSetFocus(CWnd * pOldWnd)
{
	__super::OnSetFocus(pOldWnd);

	//���ý���
	m_edPassword.SetFocus();

	//����ѡ��
	m_edPassword.SetSel(0,-1,FALSE);

	return;
}

//����ı�
VOID CPasswordControl::OnEnChangePassword()
{
	//���ñ���
	m_bModify=true;
	m_bFalsity=false;

	//������Ϣ
	CWnd * pParent=GetParent();
	if (pParent!=NULL) pParent->SendMessage(WM_COMMAND,MAKELONG(GetWindowLong(m_hWnd,GWL_ID),EN_CHANGE),(LPARAM)m_hWnd);

	return;
}

//���̰�ť
VOID CPasswordControl::OnBnClickedKeyboard()
{
	//��ȡλ��
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//��������
	if (m_bFalsity==true)
	{
		m_bFalsity=false;
		m_edPassword.SetWindowText(TEXT(""));
	}

	//��������
	if (m_PasswordKeyboard.m_hWnd==NULL)
	{
		m_PasswordKeyboard.Create(IDD_PASSWORD_KEYBOARD,this);
	}

	//��ʾ����
	UINT uFlags=SWP_NOZORDER|SWP_NOSIZE|SWP_SHOWWINDOW;
	m_PasswordKeyboard.SetWindowPos(NULL,rcWindow.left,rcWindow.bottom,0,0,uFlags);

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CPasswordKeyboard::CPasswordKeyboard() : CDialog(IDD_PASSWORD_KEYBOARD)
{
	//���ñ���
	m_nRandLine[0]=rand()%11;
	m_nRandLine[1]=rand()%10;
	m_nRandLine[2]=rand()%13;
	m_nRandLine[3]=rand()%13;

	//����״̬
	m_bShiftStatus=false;
	m_bCapsLockStatus=false;

	//״̬����
	m_bMouseDown=false;
	m_wHoverRow=INVALID_WORD;
	m_wHoverLine=INVALID_WORD;

	//�����ַ�
	m_nItemCount[0]=11;
	lstrcpyn(m_szKeyboradChar[0][0],TEXT("`1234567890"),CountArray(m_szKeyboradChar[0][0]));
	lstrcpyn(m_szKeyboradChar[0][1],TEXT("~!@#$%^&*()"),CountArray(m_szKeyboradChar[0][1]));

	//�����ַ�
	m_nItemCount[1]=10;
	lstrcpyn(m_szKeyboradChar[1][0],TEXT("-=[]\\;',./"),CountArray(m_szKeyboradChar[1][0]));
	lstrcpyn(m_szKeyboradChar[1][1],TEXT("_+{}|:\"<>?"),CountArray(m_szKeyboradChar[1][1]));

	//�����ַ�
	m_nItemCount[2]=13;
	lstrcpyn(m_szKeyboradChar[2][0],TEXT("abcdefghijklm"),CountArray(m_szKeyboradChar[2][0]));
	lstrcpyn(m_szKeyboradChar[2][1],TEXT("ABCDEFGHIJKLM"),CountArray(m_szKeyboradChar[2][1]));

	//�����ַ�
	m_nItemCount[3]=13;
	lstrcpyn(m_szKeyboradChar[3][0],TEXT("nopqrstuvwxyz"),CountArray(m_szKeyboradChar[3][0]));
	lstrcpyn(m_szKeyboradChar[3][1],TEXT("NOPQRSTUVWXYZ"),CountArray(m_szKeyboradChar[3][1]));

	//��������
	HINSTANCE hResInstance=GetModuleHandle(SHARE_CONTROL_DLL_NAME);

	//������Դ
	CImage ImageItem1;
	CImage ImageItem2;
	CImage ImageItem3;
	ImageItem1.LoadFromResource(hResInstance,IDB_KEYBOARD_ITEM1);
	ImageItem2.LoadFromResource(hResInstance,IDB_KEYBOARD_ITEM2);
	ImageItem3.LoadFromResource(hResInstance,IDB_KEYBOARD_ITEM3);

	//���ô�С
	m_SizeImageItem1.SetSize(ImageItem1.GetWidth()/3,ImageItem1.GetHeight());
	m_SizeImageItem2.SetSize(ImageItem2.GetWidth()/3,ImageItem2.GetHeight());
	m_SizeImageItem3.SetSize(ImageItem3.GetWidth()/3,ImageItem3.GetHeight());

	//�رհ�ť
	CImage ImageButton;
	ImageButton.LoadFromResource(hResInstance,IDB_BT_KEYBOARD_CLOSE);
	m_SizeImageButton.SetSize(ImageButton.GetWidth()/3,ImageButton.GetHeight());

	//������С
	CImage ImageBackGround;
	ImageBackGround.LoadFromResource(hResInstance,IDB_PASSWORD_KEYBORAD_BK);
	m_SizeBackGround.SetSize(ImageBackGround.GetWidth(),ImageBackGround.GetHeight());

	return;
}

//��������
CPasswordKeyboard::~CPasswordKeyboard()
{
}

//��Ϣ����
BOOL CPasswordKeyboard::PreTranslateMessage(MSG * pMsg)
{
	//��д����
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_CAPITAL))
	{
		//���ñ���
		bool bCapsLockStatus=m_bCapsLockStatus;
		m_bCapsLockStatus=(GetKeyState(VK_CAPITAL)&0x0F)>0;

		//���½���
		if (bCapsLockStatus!=m_bCapsLockStatus)
		{
			RedrawWindow(NULL,NULL,RDW_FRAME|RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASENOW);
		}

		return TRUE;
	}

	//�л���ť
	if ((pMsg->wParam==VK_SHIFT)&&(pMsg->message==WM_KEYUP)||(pMsg->message==WM_KEYDOWN))
	{
		//���ñ���
		bool bShiftStatus=m_bShiftStatus;
		m_bShiftStatus=(GetKeyState(VK_SHIFT)&0xF0)>0;

		//���½���
		if (bShiftStatus!=m_bShiftStatus)
		{
			RedrawWindow(NULL,NULL,RDW_FRAME|RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASENOW);
		}

		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//��������
BOOL CPasswordKeyboard::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	m_bMouseDown=false;
	m_wHoverRow=INVALID_WORD;
	m_wHoverLine=INVALID_WORD;

	//��ȡ״̬
	m_bShiftStatus=(GetKeyState(VK_SHIFT)&0xF0)>0;
	m_bCapsLockStatus=(GetKeyState(VK_CAPITAL)&0x0F)>0;

	//����λ��
	CRect rcWindow;
	rcWindow.SetRect(0,0,m_SizeBackGround.cx,m_SizeBackGround.cy);

	//�ƶ�����
	CalcWindowRect(&rcWindow,CWnd::adjustBorder);
	SetWindowPos(NULL,0,0,rcWindow.Width(),rcWindow.Height(),SWP_NOMOVE|SWP_NOZORDER);

	return FALSE;
}

//����λ��
VOID CPasswordKeyboard::SetCurrentStation(CPoint MousePoint)
{
	//��������
	WORD wHoverRow=INVALID_WORD;
	WORD wHoverLine=INVALID_WORD;

	//�ַ���ť
	if ((MousePoint.x>=ITEM_POS_X)&&(MousePoint.y>=ITEM_POS_Y))
	{
		//��������
		if (((MousePoint.x-ITEM_POS_X)%(m_SizeImageItem1.cx+ITEM_POS_S))<=m_SizeImageItem1.cx)
		{
			wHoverRow=(WORD)((MousePoint.x-ITEM_POS_X)/(m_SizeImageItem1.cx+ITEM_POS_S));
		}

		//��������
		if (((MousePoint.y-ITEM_POS_Y)%(m_SizeImageItem1.cy+ITEM_POS_S))<=m_SizeImageItem1.cy)
		{
			wHoverLine=(WORD)((MousePoint.y-ITEM_POS_Y)/(m_SizeImageItem1.cy+ITEM_POS_S));
		}

		//��������
		if (wHoverLine>=CountArray(m_nItemCount)) wHoverLine=INVALID_WORD;
		if ((wHoverLine==INVALID_WORD)||(wHoverRow>=m_nItemCount[wHoverLine])) wHoverRow=INVALID_WORD;
	}

	//���ܰ�ť
	if ((wHoverLine<2)&&(wHoverRow==INVALID_WORD))
	{
		//��������
		INT nEndLine1=ITEM_POS_X+(m_SizeImageItem1.cx+ITEM_POS_S)*m_nItemCount[0];
		INT nEndLine2=ITEM_POS_X+(m_SizeImageItem1.cx+ITEM_POS_S)*m_nItemCount[1];

		//ԭ��λ��
		CPoint ButtonPoint[4];
		ButtonPoint[0].SetPoint(nEndLine1,ITEM_POS_Y);
		ButtonPoint[1].SetPoint(nEndLine2,ITEM_POS_Y+ITEM_POS_S+m_SizeImageItem3.cy);
		ButtonPoint[2].SetPoint(nEndLine2+ITEM_POS_S+m_SizeImageItem2.cx,ITEM_POS_Y+ITEM_POS_S+m_SizeImageItem3.cy);
		ButtonPoint[3].SetPoint(m_SizeBackGround.cx-m_SizeImageButton.cx-POS_BUTTON_X,POS_BUTTON_Y);

		//��ť����
		CRect rcBack(ButtonPoint[0],m_SizeImageItem3);
		CRect rcShift(ButtonPoint[1],m_SizeImageItem2);
		CRect rcCapital(ButtonPoint[2],m_SizeImageItem2);
		CRect rcCloseKey(ButtonPoint[3],m_SizeImageButton);

		//�˸�ť
		if (rcBack.PtInRect(MousePoint))
		{
			wHoverRow=ROW_BACK;
			wHoverLine=LINE_FUNCTION;
		}

		//�л���ť
		if (rcShift.PtInRect(MousePoint))
		{
			wHoverRow=ROW_SHIFT;
			wHoverLine=LINE_FUNCTION;
		}

		//��д��ť
		if (rcCapital.PtInRect(MousePoint))
		{
			wHoverRow=ROW_CAPITAL;
			wHoverLine=LINE_FUNCTION;
		}

		//�رհ�ť
		if (rcCloseKey.PtInRect(MousePoint))
		{
			wHoverRow=ROW_CLOSE_KEY;
			wHoverLine=LINE_FUNCTION;
		}
	}

	//���ñ���
	m_wHoverRow=wHoverRow;
	m_wHoverLine=wHoverLine;

	return;
}

//�������
WORD CPasswordKeyboard::GetVirualKeyCode(WORD wHoverLine, WORD wHoverRow)
{
	//���ܽ���
	if (wHoverLine==LINE_FUNCTION)
	{
		switch (wHoverRow)
		{
		case ROW_BACK: { return VK_BACK; }
		case ROW_SHIFT: { return VK_SHIFT; }
		case ROW_CAPITAL: { return VK_CAPITAL; }
		}
	}

	//�ַ�����
	if ((wHoverLine<CountArray(m_nItemCount))&&(wHoverRow<m_nItemCount[wHoverLine]))
	{
		//��������
		bool bLowerChar=true;
		if (m_bShiftStatus==true) bLowerChar=!bLowerChar;
		if ((wHoverLine>=2)&&(m_bCapsLockStatus==true)) bLowerChar=!bLowerChar;

		//��ȡ�ַ�
		INT nItemCount=m_nItemCount[wHoverLine];
		INT nCharRowIndex=(wHoverRow+m_nRandLine[wHoverLine])%nItemCount;
		TCHAR chChar=m_szKeyboradChar[wHoverLine][(bLowerChar==true)?0:1][nCharRowIndex];

		return chChar;
	}

	return 0;
}

//����滭
VOID CPasswordKeyboard::OnNcPaint()
{
	__super::OnNcPaint();

	//��ȡλ��
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//�滭�߿�
	CWindowDC WindowDC(this);
	COLORREF crBoradFrame=CSkinEdit::m_SkinAttribute.m_crEnableBorad;
	WindowDC.Draw3dRect(0,0,rcWindow.Width(),rcWindow.Height(),crBoradFrame,crBoradFrame);

	return;
}

//�滭����
BOOL CPasswordKeyboard::OnEraseBkgnd(CDC * pDC)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CDC BufferDC;
	CBitmap ImageBuffer;
	BufferDC.CreateCompatibleDC(pDC);
	ImageBuffer.CreateCompatibleBitmap(pDC,rcClient.Width(),rcClient.Height());

	//���û���
	BufferDC.SetBkMode(TRANSPARENT);
	BufferDC.SelectObject(&ImageBuffer);
	BufferDC.SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//��������
	HINSTANCE hResInstance=GetModuleHandle(SHARE_CONTROL_DLL_NAME);
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();

	//������Դ
	CImage ImageItem1;
	CImage ImageItem2;
	CImage ImageItem3;
	CImage ImageButton;
	CImage ImageBackGround;
	ImageItem1.LoadFromResource(hResInstance,IDB_KEYBOARD_ITEM1);
	ImageItem2.LoadFromResource(hResInstance,IDB_KEYBOARD_ITEM2);
	ImageItem3.LoadFromResource(hResInstance,IDB_KEYBOARD_ITEM3);
	ImageButton.LoadFromResource(hResInstance,IDB_BT_KEYBOARD_CLOSE);
	ImageBackGround.LoadFromResource(hResInstance,IDB_PASSWORD_KEYBORAD_BK);

	//��Ⱦ��Դ
	pSkinRenderManager->RenderImage(ImageItem1);
	pSkinRenderManager->RenderImage(ImageItem2);
	pSkinRenderManager->RenderImage(ImageItem3);
	pSkinRenderManager->RenderImage(ImageButton);
	pSkinRenderManager->RenderImage(ImageBackGround);

	//�滭����
	ImageBackGround.BitBlt(BufferDC,0,0);

	//�ַ���ť
	for (INT nLine=0;nLine<CountArray(m_nItemCount);nLine++)
	{
		//�滭����
		for (INT nRow=0;nRow<m_nItemCount[nLine];nRow++)
		{
			//����λ��
			INT nXImageIndex=0;
			INT nCharItemIndex=(nRow+m_nRandLine[nLine])%m_nItemCount[nLine];
			if ((m_wHoverLine==nLine)&&(m_wHoverRow==nRow)) nXImageIndex=(m_bMouseDown==false)?1:2;

			//�滭����
			INT nXDrawPos=ITEM_POS_X+(m_SizeImageItem1.cx+ITEM_POS_S)*nRow;
			INT nYDrawPos=ITEM_POS_Y+(m_SizeImageItem1.cy+ITEM_POS_S)*nLine;
			ImageItem1.BitBlt(BufferDC,nXDrawPos,nYDrawPos,m_SizeImageItem1.cx,m_SizeImageItem1.cy,nXImageIndex*m_SizeImageItem1.cx,0);

			//������ɫ
			INT nColorIndex=0;
			COLORREF crTextColor[2]={RGB(0,0,0),RGB(125,125,125)};

			//��ɫ����
			if (m_bShiftStatus==true) nColorIndex=(nColorIndex+1)%CountArray(crTextColor);
			if ((m_bCapsLockStatus==true)&&(nLine>=2)) nColorIndex=(nColorIndex+1)%CountArray(crTextColor);

			//�滭�ַ�
			BufferDC.SetTextColor(crTextColor[nColorIndex]);
			BufferDC.TextOut(nXDrawPos+5,nYDrawPos+9,&m_szKeyboradChar[nLine][0][nCharItemIndex],1);

			//�滭�ַ�
			BufferDC.SetTextColor(crTextColor[(nColorIndex+1)%CountArray(crTextColor)]);
			BufferDC.TextOut(nXDrawPos+15,nYDrawPos+3,&m_szKeyboradChar[nLine][1][nCharItemIndex],1);
		}
	}

	//�˸�ť
	{
		//��Դλ��
		INT nXImageIndex=0;
		if ((m_wHoverLine==LINE_FUNCTION)&&(m_wHoverRow==ROW_BACK)) nXImageIndex=(m_bMouseDown==false)?1:2;

		//�滭����
		INT nYDrawPos=ITEM_POS_Y;
		INT nXDrawPos=ITEM_POS_X+(m_SizeImageItem1.cx+ITEM_POS_S)*m_nItemCount[0];
		ImageItem3.BitBlt(BufferDC,nXDrawPos,nYDrawPos,m_SizeImageItem3.cx,m_SizeImageItem3.cy,nXImageIndex*m_SizeImageItem3.cx,0);

		//�滭�ַ�
		BufferDC.SetTextColor(RGB(0,0,0));
		BufferDC.TextOut(nXDrawPos+20,ITEM_POS_Y+7,TEXT("��"),2);
	}

	//�л���ť
	{
		//��Դλ��
		INT nXImageIndex=(m_bShiftStatus==true)?1:0;
		if ((m_wHoverLine==LINE_FUNCTION)&&(m_wHoverRow==ROW_SHIFT)) nXImageIndex=(m_bMouseDown==false)?1:2;

		//�滭����
		INT nYDrawPos=ITEM_POS_Y+ITEM_POS_S+m_SizeImageItem1.cx;
		INT nXDrawPos=ITEM_POS_X+(m_SizeImageItem1.cx+ITEM_POS_S)*m_nItemCount[1];
		ImageItem2.BitBlt(BufferDC,nXDrawPos,nYDrawPos,m_SizeImageItem2.cx,m_SizeImageItem2.cy,nXImageIndex*m_SizeImageItem2.cx,0);

		//�л���ť
		BufferDC.SetTextColor(RGB(0,0,0));
		BufferDC.TextOut(nXDrawPos+5,ITEM_POS_Y+ITEM_POS_S+m_SizeImageItem1.cx+6,TEXT("Shift"),5);
	}

	//��д��ť
	{
		//��Դλ��
		INT nXImageIndex=(m_bCapsLockStatus==true)?1:0;
		if ((m_wHoverLine==LINE_FUNCTION)&&(m_wHoverRow==ROW_CAPITAL)) nXImageIndex=(m_bMouseDown==false)?1:2;

		//�滭����
		INT nYDrawPos=ITEM_POS_Y+ITEM_POS_S+m_SizeImageItem1.cx;
		INT nXDrawPos=ITEM_POS_X+(m_SizeImageItem1.cx+ITEM_POS_S)*m_nItemCount[1]+m_SizeImageItem2.cx+ITEM_POS_S;
		ImageItem2.BitBlt(BufferDC,nXDrawPos,nYDrawPos,m_SizeImageItem2.cx,m_SizeImageItem2.cy,nXImageIndex*m_SizeImageItem2.cx,0);

		//��д��ť
		BufferDC.SetTextColor(RGB(0,0,0));
		BufferDC.TextOut(nXDrawPos+8,ITEM_POS_Y+ITEM_POS_S+m_SizeImageItem1.cx+6,TEXT("Caps"),4);
	}

	//�رհ�ť
	{
		//��Դλ��
		INT nXImageIndex=0;
		if ((m_wHoverLine==LINE_FUNCTION)&&(m_wHoverRow==ROW_CLOSE_KEY)) nXImageIndex=(m_bMouseDown==false)?1:2;

		//�滭����
		INT nXDrawPos=rcClient.Width()-m_SizeImageButton.cx-POS_BUTTON_X;
		ImageButton.BitBlt(BufferDC,nXDrawPos,POS_BUTTON_Y,m_SizeImageButton.cx,m_SizeImageButton.cy,nXImageIndex*m_SizeImageButton.cx,0);
	}

	//�滭����
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),&BufferDC,0,0,SRCCOPY);

	//������Դ
	BufferDC.DeleteDC();
	ImageBuffer.DeleteObject();

	return TRUE;
}

//ʧȥ����
VOID CPasswordKeyboard::OnKillFocus(CWnd * pNewWnd)
{
	__super::OnKillFocus(pNewWnd);

	//���ٴ���
	DestroyWindow();

	return;
}

//�����Ϣ
VOID CPasswordKeyboard::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags,Point);

	//ȡ������
	if (m_bMouseDown==true)
	{
		//ȡ������
		ReleaseCapture();

		//���ñ���
		m_bMouseDown=false;

		//��ȡ���
		CPoint MousePoint;
		GetCursorPos(&MousePoint);
		ScreenToClient(&MousePoint);

		//����λ��
		WORD wHoverRow=m_wHoverRow;
		WORD wHoverLine=m_wHoverLine;
		SetCurrentStation(MousePoint);

		//�������
		if ((m_wHoverRow==wHoverRow)&&(m_wHoverLine==wHoverLine))
		{
			//�رհ�ť
			if ((m_wHoverLine==LINE_FUNCTION)&&(m_wHoverRow==ROW_CLOSE_KEY))
			{
				//���ý���
				CONTAINING_RECORD(this,CPasswordControl,m_PasswordKeyboard)->m_edPassword.SetFocus();

				//���ٴ���
				DestroyWindow();

				return;
			}

			//�������
			WORD wViraulCode=GetVirualKeyCode(m_wHoverLine,m_wHoverRow);

			//��ť����
			switch (wViraulCode)
			{
			case VK_SHIFT:		//�л���ť
				{
					//���ñ���
					m_bShiftStatus=!m_bShiftStatus;

					break;
				}
			case VK_CAPITAL:	//��д��ť
				{
					//��������
					INPUT Input[2];
					ZeroMemory(Input,sizeof(Input));

					//���ñ���
					Input[1].ki.dwFlags=KEYEVENTF_KEYUP;   
					Input[0].type=Input[1].type=INPUT_KEYBOARD;
					Input[0].ki.wVk=Input[1].ki.wVk=wViraulCode;

					//ģ������
					SendInput(CountArray(Input),Input,sizeof(INPUT));

					break;
				}
			default:			//Ĭ�ϰ�ť
				{
					//���ñ���
					m_bShiftStatus=(GetKeyState(VK_SHIFT)&0xF0)>0;

					//������Ϣ
					CPasswordControl * pPasswordControl=CONTAINING_RECORD(this,CPasswordControl,m_PasswordKeyboard);
					if (pPasswordControl!=NULL) pPasswordControl->m_edPassword.SendMessage(WM_CHAR,wViraulCode,0L);

					break;
				}
			}
		}

		//���½���
		RedrawWindow(NULL,NULL,RDW_FRAME|RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//�����Ϣ
VOID CPasswordKeyboard::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//�����ť
	if ((m_wHoverLine!=INVALID_WORD)&&(m_wHoverRow!=INVALID_WORD))
	{
		//�������
		SetCapture();

		//���ñ���
		m_bMouseDown=true;

		//���½���
		RedrawWindow(NULL,NULL,RDW_FRAME|RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//�����Ϣ
BOOL CPasswordKeyboard::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//����λ��
	WORD wHoverRow=m_wHoverRow;
	WORD wHoverLine=m_wHoverLine;
	SetCurrentStation(MousePoint);

	//���½���
	if ((m_wHoverRow!=wHoverRow)||(m_wHoverLine!=wHoverLine))
	{
		RedrawWindow(NULL,NULL,RDW_FRAME|RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	//���ù��
	if ((m_wHoverRow!=INVALID_WORD)&&(m_wHoverLine!=INVALID_WORD))
	{
		SetCursor(LoadCursor(GetModuleHandle(SHARE_CONTROL_DLL_NAME),MAKEINTRESOURCE(IDC_HAND_CUR)));
		return true;
	}

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//////////////////////////////////////////////////////////////////////////////////
