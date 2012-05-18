#include "StdAfx.h"
#include "Resource.h"
#include "SkinDialog.h"
#include "SkinComboBox.h"
#include "SkinRenderManager.h"

//////////////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_CHECK_HOVER				10									//���ɱ�ʶ
#define TIME_CHECK_HOVER			10									//����ʱ��

//////////////////////////////////////////////////////////////////////////////////

//��̬����
CSkinComboBoxAttribute			CSkinComboBox::m_SkinAttribute;			//��ť����

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSkinComboBoxList, CListBox)
	ON_WM_NCPAINT()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CSkinComboBox, CComboBox)
	ON_WM_SIZE()
    ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCbnCloseup)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CSkinComboBoxAttribute::CSkinComboBoxAttribute()
{
	//���ñ���
	m_uButtonWidth=0;
	m_crEnableBack=RGB(0,0,0);
	m_crDisableBack=RGB(0,0,0);
	m_crEnableBorad=RGB(0,0,0);
	m_crDisableBorad=RGB(0,0,0);

	return;
}

//��������
CSkinComboBoxAttribute::~CSkinComboBoxAttribute() 
{
}

//������Դ
bool CSkinComboBoxAttribute::Initialization(CSkinRenderManager * pSkinRenderManager)
{
	//���ñ���
	m_uButtonWidth=16;
	m_crEnableBack=RGB(255,255,255);
	m_crEnableBorad=RGB(92,100,105);
	m_crDisableBack=RGB(125,125,125);
	m_crDisableBorad=RGB(92,100,105);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CSkinComboBoxList::CSkinComboBoxList()
{
}

//��������
CSkinComboBoxList::~CSkinComboBoxList()
{
}

//�滭����
VOID CSkinComboBoxList::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//��������
	CRect rcItem=lpDrawItemStruct->rcItem;
	CDC * pDCControl=CDC::FromHandle(lpDrawItemStruct->hDC);

	//��������
	CDC BufferDC;
	CBitmap ImageBuffer;
	BufferDC.CreateCompatibleDC(pDCControl);
	ImageBuffer.CreateCompatibleBitmap(pDCControl,rcItem.Width(),rcItem.Height());

	//���û���
	BufferDC.SelectObject(&ImageBuffer);
	BufferDC.SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//��ȡ�ַ�
	CString strString;
	GetText(lpDrawItemStruct->itemID,strString);

	//����λ��
	CRect rcString;
	rcString.SetRect(4,0,rcItem.Width()-8,rcItem.Height());

	//��ɫ����
	COLORREF crTextColor=((lpDrawItemStruct->itemState&ODS_SELECTED)!=0)?RGB(255,255,255):RGB(0,0,0);
	COLORREF crBackColor=((lpDrawItemStruct->itemState&ODS_SELECTED)!=0)?RGB(10,36,106):RGB(255,255,255);

	//�滭����
	BufferDC.FillSolidRect(0,0,rcItem.Width(),rcItem.Height(),crBackColor);

	//�滭�ַ�
	BufferDC.SetBkMode(TRANSPARENT);
	BufferDC.SetTextColor(crTextColor);
	BufferDC.DrawText(strString,&rcString,DT_VCENTER|DT_SINGLELINE);

	//�滭����
	pDCControl->BitBlt(rcItem.left,rcItem.top,rcItem.Width(),rcItem.Height(),&BufferDC,0,0,SRCCOPY);

	//������Դ
	BufferDC.DeleteDC();
	ImageBuffer.DeleteObject();

	return;
}

//����滭
VOID CSkinComboBoxList::OnNcPaint()
{
	__super::OnNcPaint();

	//��ȡλ��
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//�滭�߿�
	CWindowDC WindowDC(this);
	COLORREF crBoradFrame=CSkinComboBox::m_SkinAttribute.m_crEnableBorad;
	WindowDC.Draw3dRect(0,0,rcWindow.Width(),rcWindow.Height(),crBoradFrame,crBoradFrame);

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CSkinComboBox::CSkinComboBox()
{
	//���Ʊ���
	m_bHovering=false;
	m_bDrawBorad=true;
	m_bRenderImage=false;
	m_uButtonWidth=m_SkinAttribute.m_uButtonWidth;

	//��Դ����
	m_uResourceID=0;
	m_hResInstance=NULL;

	return;
}

//��������
CSkinComboBox::~CSkinComboBox()
{
}

//�ؼ���
VOID CSkinComboBox::PreSubclassWindow()
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//���ÿؼ�
	Initialization();
	RectifyControl(rcClient.Width(),rcClient.Height());

	return;
}

//��������
VOID CSkinComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	//���ñ���
	lpMeasureItemStruct->itemWidth=0;
	lpMeasureItemStruct->itemHeight=GetItemHeight(LB_ERR);

	return;
}

//�滭�ؼ�
VOID CSkinComboBox::DrawControlView(CDC * pDC)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//״̬����
	DWORD dwStyle=GetStyle();
	bool bDisable=(IsWindowEnabled()==FALSE);
	bool bDropped=(GetDroppedState()!=FALSE);

	//�滭����
	if (((GetStyle()&0x000F)==CBS_DROPDOWN)||(GetCurSel()==LB_ERR))
	{
		if (bDisable==false)
		{
			COLORREF crColor=m_SkinAttribute.m_crEnableBack;
			pDC->FillSolidRect(1,1,rcClient.Width()-m_uButtonWidth-2,rcClient.Height()-2,crColor);
		}
		else
		{
			COLORREF crColor=CSkinDialog::m_SkinAttribute.m_crBackGround;
			pDC->FillSolidRect(1,1,rcClient.Width()-m_uButtonWidth-2,rcClient.Height()-2,crColor);
		}
	}

	//�滭����
	if ((GetStyle()&0x000F)==CBS_DROPDOWNLIST)
	{
		//������ɫ
		if (GetFocus()->GetSafeHwnd()==m_hWnd)
		{
			COLORREF crColor=m_SkinAttribute.m_crEnableBack;
			pDC->FillSolidRect(0,0,rcClient.Width()-m_uButtonWidth,rcClient.Height()-2,crColor);
		}
		else
		{
			COLORREF crColor=m_SkinAttribute.m_crEnableBack;
			pDC->FillSolidRect(0,0,rcClient.Width(),rcClient.Height(),crColor);
		}
	}

	//�滭�ַ�
	if ((GetStyle()&0x000F)==CBS_DROPDOWNLIST)
	{
		//��ȡ�ַ�
		CString strString;
		GetWindowText(strString);

		//��ȡ�ַ�
		CRect rcText;
		GetClientRect(&rcText);

		//���û���
		pDC->SetBkMode(TRANSPARENT);
		pDC->SelectObject(GetFont());

		//�滭�ַ�
		rcText.left+=4;
		pDC->DrawText(strString,&rcText,DT_VCENTER|DT_SINGLELINE);
	}

	//�滭�߿�
	if (m_bDrawBorad==true)
	{
		if (bDisable==false)
		{
			if (m_bRenderImage==true)
			{
				//��������
				ASSERT(CSkinRenderManager::GetInstance()!=NULL);
				CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();

				//��Ⱦ��ɫ
				COLORREF crEnableBorad=pSkinRenderManager->RenderColor(0.77,0.65);
				pDC->Draw3dRect(0,0,rcClient.Width(),rcClient.Height(),crEnableBorad,crEnableBorad);
			}
			else
			{
				COLORREF crEnableBorad=m_SkinAttribute.m_crEnableBorad;
				pDC->Draw3dRect(0,0,rcClient.Width(),rcClient.Height(),crEnableBorad,crEnableBorad);
			}
		}
		else
		{
			COLORREF crDisableBorad=m_SkinAttribute.m_crDisableBorad;
			pDC->Draw3dRect(0,0,rcClient.Width(),rcClient.Height(),crDisableBorad,crDisableBorad);
		}
	}
	else
	{
		COLORREF crEnableBorad=m_SkinAttribute.m_crEnableBack;
		pDC->Draw3dRect(0,0,rcClient.Width(),rcClient.Height(),crEnableBorad,crEnableBorad);
	}

	//����λ��
	INT nImageIndex=0;
	if (bDisable==true) nImageIndex=4;
	else if (bDropped==true) nImageIndex=1;
	else if (m_bHovering==true) nImageIndex=3;

	//���ذ�ť
	CBitImage ImageButton;
	if ((m_hResInstance==NULL)||(m_uResourceID==0))
	{
		ImageButton.LoadFromResource(GetModuleHandle(SKIN_CONTROL_DLL_NAME),IDB_SKIN_COMBOBOX_BUTTON);
	}
	else
	{
		ImageButton.LoadFromResource(m_hResInstance,m_uResourceID);
	}

	//��Ⱦ��Դ
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();
	if ((pSkinRenderManager!=NULL)&&(m_bRenderImage==true)) pSkinRenderManager->RenderImage(ImageButton);

	//�滭��ť
	CSize SizeButton;
	SizeButton.SetSize(ImageButton.GetWidth()/5L,ImageButton.GetHeight());
	ImageButton.StretchBlt(pDC->m_hDC,rcClient.Width()-m_uButtonWidth-1,1,m_uButtonWidth,rcClient.Height()-2,
		nImageIndex*SizeButton.cx,0,SizeButton.cx,SizeButton.cy);

	return;
}

//��ť���
bool CSkinComboBox::VerdictOverButton(CPoint MousePoint)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//�����б�
	if ((rcClient.PtInRect(MousePoint)==TRUE)&&((GetStyle()&CBS_SIMPLE)!=0)) return true;

	//�������
	if ((MousePoint.y>(rcClient.Height()-1))||(MousePoint.y<1)) return false;
	if ((MousePoint.x<(rcClient.Width()-(INT)m_uButtonWidth-1))||(MousePoint.x>(rcClient.Width()-1))) return false;

	return true;
}

//�߿�����
VOID CSkinComboBox::SetDrawBorad(bool bDrawBorad)
{
	//���ñ���
	m_bDrawBorad=bDrawBorad;

	//���½���
	if (m_hWnd!=NULL) Invalidate(FALSE);

	return;
}

//������Ⱦ
VOID CSkinComboBox::SetRenderImage(bool bRenderImage)
{
	//���ñ���
	m_bRenderImage=bRenderImage;

	//���½���
	if (m_hWnd!=NULL) Invalidate(FALSE);

	return;
}

//������Դ
VOID CSkinComboBox::SetButtonImage(HINSTANCE hResInstance, UINT uResourceID, CSize & SizeButton)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//���ñ���
	m_uResourceID=uResourceID;
	m_hResInstance=hResInstance;

	//���ÿؼ�
	m_uButtonWidth=SizeButton.cx;
	SetItemHeight(LB_ERR,SizeButton.cy);

	//���ÿؼ�
	RectifyControl(rcClient.Width(),rcClient.Height());

	//���½���
	if (m_hWnd!=NULL) RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW);

	return;
}

//���ÿؼ�
VOID CSkinComboBox::Initialization()
{
	//����״̬
	m_bHovering=false;
	m_uButtonWidth=m_SkinAttribute.m_uButtonWidth;

	//��������
	COMBOBOXINFO ComboBoxInfo;
	ComboBoxInfo.cbSize=sizeof(ComboBoxInfo);

	//�󶨿ؼ�
	if (GetComboBoxInfo(&ComboBoxInfo)==TRUE)
	{
		if (ComboBoxInfo.hwndItem!=NULL) m_SkinComboBoxEdit.SubclassWindow(ComboBoxInfo.hwndItem);
		if (ComboBoxInfo.hwndList!=NULL) m_SkinComboBoxList.SubclassWindow(ComboBoxInfo.hwndList);
	}

	return;
}

//���°�ť
VOID CSkinComboBox::RedrawDropButton()
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��ťλ��
	CRect rcDropButton;
	rcDropButton.SetRect(rcClient.Width()-m_uButtonWidth-1,1,rcClient.Width()-1,rcClient.Height()-1);

	//���½���
	RedrawWindow(&rcDropButton,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW);

	return;
}

//�����ؼ�
VOID CSkinComboBox::RectifyControl(INT nWidth, INT nHeight)
{
	//�ƶ��ؼ�
	if (m_SkinComboBoxEdit.m_hWnd!=NULL)
	{
		//��ȡλ��
		CRect rcClient;
		GetClientRect(&rcClient);

		//�ƶ��ؼ�
		CSize SizeEdit;
		SizeEdit.SetSize(rcClient.Width()-m_uButtonWidth-10,12);
		m_SkinComboBoxEdit.SetWindowPos(NULL,6,(rcClient.Height()-12)/2,SizeEdit.cx,SizeEdit.cy,SWP_NOZORDER);
	}

	return;
}

//�ػ���Ϣ
VOID CSkinComboBox::OnPaint()
{
	CPaintDC dc(this);

	//�滭�ؼ�
	DrawControlView(&dc);

	return;
}

//�б�ر�
VOID CSkinComboBox::OnCbnCloseup()
{
	//���°�ť
	RedrawDropButton();

	return;
}

//�滭����
BOOL CSkinComboBox::OnEraseBkgnd(CDC * pDC)
{
	return TRUE;
}

//ʱ����Ϣ
VOID CSkinComboBox::OnTimer(UINT_PTR nIDEvent)
{
	//�����ж�
	if (nIDEvent==IDI_CHECK_HOVER)
	{
		//�����ж�
		if (m_bHovering==true)
		{
			//��ȡ���
			CPoint MousePoint;
			GetCursorPos(&MousePoint);
			ScreenToClient(&MousePoint);

			//�����ж�
			m_bHovering=VerdictOverButton(MousePoint);

			//���½���
			if (m_bHovering==false) RedrawDropButton();
		}

		//ɾ��ʱ��
		if (m_bHovering==false) KillTimer(IDI_CHECK_HOVER);

		return;
	}

	__super::OnTimer(nIDEvent);
}

//λ����Ϣ
VOID CSkinComboBox::OnSize(UINT nType, INT cx, INT cy) 
{
	__super::OnSize(nType, cx, cy);

	//�����ؼ�
	RectifyControl(cx,cy);

	return;
}

//������Ϣ
INT CSkinComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//���ÿؼ�
	Initialization();
	RectifyControl(rcClient.Width(),rcClient.Height());

	return 0;
}

//�����Ϣ
VOID CSkinComboBox::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags, Point);

	if (m_bHovering==false)
	{
		//�����ж�
		m_bHovering=VerdictOverButton(Point);

		//״̬�ж�
		if (m_bHovering==true)
		{
			//���½���
			RedrawDropButton();

			//����ʱ��
			SetTimer(IDI_CHECK_HOVER,TIME_CHECK_HOVER,NULL);
		}
	}

	return;
}

//�����Ϣ
VOID CSkinComboBox::OnLButtonDown(UINT nFlags, CPoint Point)
{
	//���ý���
	SetFocus();

	//��ʾ�б�
	if (VerdictOverButton(Point)==true)
	{
		//��ʾ�б�
		ShowDropDown(GetDroppedState()==FALSE);

		//���°�ť
		RedrawDropButton();
	}

	return;
}

//�����Ϣ
VOID CSkinComboBox::OnLButtonDblClk(UINT nFlags, CPoint Point)
{
	//���ý���
	SetFocus();

	//��ʾ�б�
	if (VerdictOverButton(Point)==true)
	{
		//��ʾ�б�
		ShowDropDown(GetDroppedState()==FALSE);

		//���°�ť
		RedrawDropButton();
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////