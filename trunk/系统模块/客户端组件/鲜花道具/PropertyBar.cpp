#include "stdafx.h"
#include "resource.h"
#include "PropertyBar.h"
#include "PropertyItem.h"

///////////////////////////////////////////////////////////////////////////////////
//�궨��
#define CR_BACK_GROUND					RGB(57,162,214)					//������ɫ

#define BTM_BUTTON_Y					41									//��ťλ��
#define TOP_BUTTON_Y					6									//��ťλ��
#define BUTTON_EXCUSION_X				8									//ƫ��λ��
#define BUTTON_DISTANCE_X				5									//��ť���

//��������
tagPropertyInfo	CPropertyBar::m_PropertyInfo[PROPERTY_COUNT];				//������Ϣ
tagFlowerInfo	CPropertyBar::m_FlowerInfo[FLOWER_COUNT];					//�ʻ���Ϣ

//////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPropertyBar, CDialog)

	//������Ϣ
	ON_BN_CLICKED(IDC_BOTTOM_MOVE_TO_L, OnBnClickedBtmMoveToL)
	ON_BN_CLICKED(IDC_BOTTOM_MOVE_TO_R, OnBnClickedBtmMoveToR)
	ON_BN_CLICKED(IDC_TOP_MOVE_TO_L, OnBnClickedTopMoveToL)
	ON_BN_CLICKED(IDC_TOP_MOVE_TO_R, OnBnClickedTopMoveToR)

	//��ͨ��Ϣ
	ON_WM_PAINT()
	ON_WM_SIZE()

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////

//��̬����
CPropertyBar *CPropertyBar::m_pPropertyBar = NULL;					//���ƶ���


//////////////////////////////////////////////////////////////////////
//���캯��
CPropertyBar::CPropertyBar(CWnd* pParent):CDialog(IDD_PROPERTY_BAR, pParent)
{
	//��Դ����
	m_ReshInstance = NULL;
	ZeroMemory(&m_PropertyViewImage, sizeof(m_PropertyViewImage));	

	//�������
	m_pIPurchaseInfo = NULL;

	//����λ��
	m_nXoriginal=0;
	m_nButtonCount=0;
	m_nTopButtonHead=0;	
	m_nBtmButtonHead=0;	
	//��ʼ����
	m_bInitDialog = false;
	m_pPropertyItem= new CPropertyItem;
	m_pFlowerSetting = new CFlowerSetting;

	//��̬����
	//ASSERT(m_pPropertyBar==NULL);
	m_pPropertyBar = this;

	//���߱���
	m_nPropertyCount=0;	
	m_nFlowerCount=0;
	ZeroMemory(m_PropertyInfo, CountArray(m_PropertyInfo));	
	ZeroMemory(m_FlowerInfo, CountArray(m_FlowerInfo));	
	for ( int nIndex = 0; nIndex < CountArray(m_PropertyInfo); ++nIndex ) m_PropertyInfo[nIndex].bNullity = true;
	for ( int nIndex = 0; nIndex < CountArray(m_FlowerInfo); ++nIndex ) m_FlowerInfo[nIndex].bNullity = true;
}

//��������
CPropertyBar::~CPropertyBar()
{
	delete m_pPropertyItem;
	delete m_pFlowerSetting;
	m_pPropertyItem=NULL;
	m_pFlowerSetting=NULL;
}

//�ؼ���
void CPropertyBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//����ӳ��
	UINT uProperyID[] = {IDC_BT_PROPERTY1,IDC_BT_PROPERTY2,IDC_BT_PROPERTY3,IDC_BT_PROPERTY4,IDC_BT_PROPERTY5,IDC_BT_PROPERTY6,
		IDC_BT_PROPERTY7,IDC_BT_PROPERTY8,IDC_BT_PROPERTY9,IDC_BT_PROPERTY10,IDC_BT_PROPERTY11,IDC_BT_PROPERTY12,IDC_BT_PROPERTY13,
		IDC_BT_PROPERTY14,IDC_BT_PROPERTY15};
	for ( int nIndex = 0; nIndex < CountArray(m_BtProperty); ++nIndex )
		DDX_Control(pDX, uProperyID[nIndex], m_BtProperty[nIndex]);

	//�ʻ�ӳ��
	UINT uFlowerID[] = {IDC_BT_FLOWER1,IDC_BT_FLOWER2,IDC_BT_FLOWER3,IDC_BT_FLOWER4,IDC_BT_FLOWER5,IDC_BT_FLOWER6,IDC_BT_FLOWER7,
	IDC_BT_FLOWER8,IDC_BT_FLOWER9,IDC_BT_FLOWER10,IDC_BT_FLOWER11};
	for ( int nIndex = 0; nIndex < CountArray(m_BtFlower); ++nIndex ) 
		DDX_Control(pDX, uFlowerID[nIndex], m_BtFlower[nIndex]);

	//����ӳ��
	DDX_Control(pDX, IDC_TOP_MOVE_TO_L, m_btTopMoveToL);
	DDX_Control(pDX, IDC_TOP_MOVE_TO_R, m_btTopMoveToR);
	DDX_Control(pDX, IDC_BOTTOM_MOVE_TO_L, m_btBtmMoveToL);
	DDX_Control(pDX, IDC_BOTTOM_MOVE_TO_R, m_btBtmMoveToR);

	//��ʼ����
	m_bInitDialog = true;

	return;
}

//�滭����
void CPropertyBar::OnPaint()
{
	CPaintDC dc(this); 

	//��ȡ��С
	CRect rcClient;
	GetWindowRect(rcClient);

	//��䱳��
	dc.FillSolidRect(0, 0, rcClient.Width(), rcClient.Height(), CR_BACK_GROUND);

	//ͼƬ���
	CImageHandle ImagePropertyLHandle(&m_ImageL);
	CImageHandle ImagePropertyMHandle(&m_ImageM);
	CImageHandle ImagePropertyRHandle(&m_ImageR);

	//��ʼ�滭
	m_ImageL.BitBlt(dc.GetSafeHdc(), 0, 0);
	int nXDest = rcClient.Width() - m_ImageR.GetWidth();
	for ( int nXPos = m_ImageL.GetWidth(); nXPos <= nXDest; nXPos += m_ImageM.GetWidth() )
	{
		int nWidth = __min(nXDest-nXPos,m_ImageM.GetWidth());
		m_ImageM.BitBlt(dc.GetSafeHdc(), 0+nXPos, 0, nWidth, m_ImageM.GetHeight(),0,0);
	}
	m_ImageR.BitBlt(dc.GetSafeHdc(), nXDest , 0);

	return;
}

//��ʼ����
BOOL CPropertyBar::OnInitDialog()
{
	CDialog::OnInitDialog();

	//��ԴЧ��
	ASSERT(m_ReshInstance!=NULL);
	if(m_ReshInstance==NULL)return FALSE;

	//������Դ
	UpdateSkinResource();

	//���ܰ�ť
	m_btTopMoveToL.EnableWindow(FALSE);
	m_btTopMoveToR.EnableWindow(FALSE);
	m_btBtmMoveToL.EnableWindow(FALSE);
	m_btBtmMoveToR.EnableWindow(FALSE);

	//������ʾ
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	for ( int nIndex = 0; nIndex < CountArray(m_BtProperty); ++nIndex )
		m_ToolTipCtrl.AddTool(&m_BtProperty[nIndex],g_PropertyDescribe[nIndex].szName);

	//�ʻ���ʾ
	for ( int nIndex = 0; nIndex < CountArray(m_BtFlower); ++nIndex )
		m_ToolTipCtrl.AddTool(&m_BtFlower[nIndex],g_FlowerDescribe[nIndex].szName);

	return TRUE; 
}

//�����ؼ�
void CPropertyBar::RectifyControl(int nWidth, int nHeight)
{
	//��ʼ�ж�
	if ( m_bInitDialog == false ) return ;

	//��ȡ��С
	CRect rcClient;
	GetWindowRect(rcClient);

	//����ͼƬ
	CImageHandle ImageHandleAreaL(&m_ImageL);
	CImageHandle ImageHandleAreaLM(&m_ImageM);
	CImageHandle ImageHandleAreaLR(&m_ImageR);

	//��ȡ��С
	const int nAreaLWidth = m_ImageL.GetWidth();
	const int nAreaMWidth = m_ImageM.GetWidth();
	const int nAreaRWidth = m_ImageR.GetWidth();
	const int nAreaMGift = m_ImageM.GetWidth();

	//��ť��С
	CRect rcButtonClient,rcMoveButClient;
	m_BtProperty[0].GetWindowRect(rcButtonClient);
	m_btTopMoveToL.GetWindowRect(rcMoveButClient);

	//��ť����
	INT nAllLong = rcClient.Width() - (nAreaLWidth-5) - (14+rcMoveButClient.Width()*2+8);
	m_nButtonCount = nAllLong / (rcButtonClient.Width()+BUTTON_DISTANCE_X);
	const int nButtonCount=m_nButtonCount;

	//��ʼλ��
	m_nXoriginal = nAreaLWidth-5+(nAllLong-m_nButtonCount*(rcButtonClient.Width()+BUTTON_DISTANCE_X))/2;

	//���ñ���
	m_nBtmButtonHead = 0;
	m_nTopButtonHead = 0;
	while (m_PropertyInfo[m_nTopButtonHead].bNullity==true) m_nTopButtonHead++;
	while (m_FlowerInfo[m_nBtmButtonHead].bNullity==true) m_nBtmButtonHead++;

	//ʹ���ж�
	m_btBtmMoveToR.EnableWindow(FALSE);
	m_btBtmMoveToL.EnableWindow((m_nFlowerCount <= nButtonCount)?FALSE:TRUE);
	m_btTopMoveToR.EnableWindow(FALSE);
	m_btTopMoveToL.EnableWindow((m_nPropertyCount <= nButtonCount)?FALSE:TRUE);

	//�ƶ���ť
	HDWP hDwp = BeginDeferWindowPos(20);
	const UINT uFlags = SWP_NOSIZE | SWP_NOCOPYBITS | SWP_NOACTIVATE | SWP_NOZORDER;

	//���Ұ�ť
	DeferWindowPos(hDwp, m_btTopMoveToR, NULL, rcClient.Width() - 18-rcMoveButClient.Width()*2, 10, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btTopMoveToL, NULL, rcClient.Width() - rcMoveButClient.Width()-10, 10, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btBtmMoveToL, NULL, rcClient.Width() - rcMoveButClient.Width()-10, 45, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btBtmMoveToR, NULL, rcClient.Width() - 18-rcMoveButClient.Width()*2, 45, 0, 0, uFlags);

	EndDeferWindowPos(hDwp);

	//���¿ؼ�
	UpdateTopButton(m_nTopButtonHead);
	UpdateBtmButton(m_nBtmButtonHead);

	return;
}

//���ڸ���
void CPropertyBar::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	//�����ؼ�
	RectifyControl(cx, cy);
	InvalidateRect(NULL);

	return;
}

//���¿ؼ�
void CPropertyBar::UpdateTopButton(INT nTopButtonHead)
{
	//��ȡ��С
	CRect rcClient;
	GetWindowRect(rcClient);

	//����ͼƬ
	CImageHandle ImageHandleAreaL(&m_ImageL);
	CImageHandle ImageHandleAreaLM(&m_ImageM);
	CImageHandle ImageHandleAreaLR(&m_ImageR);

	//��ȡ��С
	const int nAreaLWidth = m_ImageL.GetWidth();
	const int nAreaMWidth = m_ImageM.GetWidth();
	const int nAreaRWidth = m_ImageR.GetWidth();

	//��ť����
	const int nButtonCount = m_nButtonCount;

	//�ƶ���ť
	HDWP hDwp = BeginDeferWindowPos(20);
	const UINT uFlags = SWP_NOSIZE | SWP_NOCOPYBITS | SWP_NOACTIVATE | SWP_NOZORDER;

	//��ť��С
	CRect rcButtonClient;
	m_BtProperty[0].GetWindowRect(rcButtonClient);

	//�ϱ߰�ť
	for ( int nIndex =0, nButtonIndex = nTopButtonHead; nIndex < nButtonCount && nIndex < m_nPropertyCount && 
		nButtonIndex < CountArray(m_BtProperty) ; nButtonIndex++ )
	{
		if ( m_PropertyInfo[nButtonIndex].bNullity == true ) continue;

		DeferWindowPos(hDwp, m_BtProperty[nButtonIndex].GetSafeHwnd(), NULL,m_nXoriginal+ nIndex * (rcButtonClient.Width()+BUTTON_DISTANCE_X) + BUTTON_DISTANCE_X,
			TOP_BUTTON_Y, 0, 0, uFlags);
		++nIndex;
	}

	EndDeferWindowPos(hDwp);

	//���ذ�ť
	for ( INT nIndex = 0; nIndex < CountArray(m_BtProperty); ++nIndex ) 
		m_BtProperty[nIndex].ShowWindow(SW_HIDE);

	//�ɼ�����
	for ( INT nIndex = nTopButtonHead, nShowButtonCount=0; nShowButtonCount < nButtonCount && nShowButtonCount < m_nPropertyCount && 
		nIndex < CountArray(m_BtProperty) ; ++nIndex )
	{
		if ( m_PropertyInfo[nIndex].bNullity == false )
		{
			m_BtProperty[nIndex].ShowWindow(SW_SHOW);
			nShowButtonCount++;
		}
	}

	return;
}

//���¿ؼ�
void CPropertyBar::UpdateBtmButton(INT nBtmButtonHead)
{
	//��ȡ��С
	CRect rcClient;
	GetWindowRect(rcClient);

	//����ͼƬ
	CImageHandle ImageHandleAreaL(&m_ImageL);
	CImageHandle ImageHandleAreaLM(&m_ImageM);
	CImageHandle ImageHandleAreaLR(&m_ImageR);

	//��ȡ��С
	const int nAreaLWidth = m_ImageL.GetWidth();
	const int nAreaMWidth = m_ImageM.GetWidth();
	const int nAreaRWidth = m_ImageR.GetWidth();

	//��ť����
	const int nButtonCount = m_nButtonCount;

	//�ƶ���ť
	HDWP hDwp = BeginDeferWindowPos(20);
	const UINT uFlags = SWP_NOSIZE | SWP_NOCOPYBITS | SWP_NOACTIVATE | SWP_NOZORDER;

	//��ť��С
	CRect rcButtonClient;
	m_BtProperty[0].GetWindowRect(rcButtonClient);

	//�±߰�ť
	for ( int nIndex =0, nButtonIndex = nBtmButtonHead; nIndex < nButtonCount && nIndex < m_nFlowerCount && 
		nButtonIndex < CountArray(m_BtFlower) ; nButtonIndex++ )
	{		
		if ( m_FlowerInfo[nButtonIndex].bNullity == true ) continue;

		DeferWindowPos(hDwp, m_BtFlower[nButtonIndex].GetSafeHwnd(), NULL, m_nXoriginal+ nIndex * (rcButtonClient.Width()+BUTTON_DISTANCE_X) + BUTTON_DISTANCE_X,
			BTM_BUTTON_Y, 0, 0, uFlags);
		++nIndex;
	}

	EndDeferWindowPos(hDwp);

	//���ذ�ť
	for ( INT nIndex = 0; nIndex < CountArray(m_BtFlower); ++nIndex ) 
		m_BtFlower[nIndex].ShowWindow(SW_HIDE);

	//��ʾ��ť
	for ( INT nIndex = nBtmButtonHead, nShowButtonCount=0; nShowButtonCount < nButtonCount && nShowButtonCount < m_nFlowerCount && 
		nIndex < CountArray(m_BtFlower) ; ++nIndex )
	{
		if ( m_FlowerInfo[nIndex].bNullity == false )
		{
			m_BtFlower[nIndex].ShowWindow(SW_SHOW);
			nShowButtonCount++;
		}
	}

	return;
}

//��ȡ�߶�
int CPropertyBar::GetHeight()
{
	CImageHandle ImagePropertyLHandle(&m_ImageL);

	return m_ImageL.GetHeight();
}

//������Դ
void CPropertyBar::SetSkinResource(HINSTANCE ReshInstance,tagPropertyViewImage *pPropertyViewImage)
{
	//������Դ
	m_ReshInstance=ReshInstance;
	CopyMemory(&m_PropertyViewImage,pPropertyViewImage,sizeof(tagPropertyViewImage));

	return ;
}

//������Դ
bool CPropertyBar::UpdateSkinResource()
{
	//������Դ
	HINSTANCE hInstance = m_ReshInstance;
	m_ImageL.SetLoadInfo(m_PropertyViewImage.uPropertyFrameL, hInstance, false);
	m_ImageM.SetLoadInfo(m_PropertyViewImage.uPropertyFrameM, hInstance, false);
	m_ImageR.SetLoadInfo(m_PropertyViewImage.uPropertyFrameR, hInstance, false);

	//���߰�ť
	for ( int nIndex = 0; nIndex < CountArray(m_BtProperty); ++nIndex ) 
		m_BtProperty[nIndex].SetButtonImage(m_PropertyViewImage.uBtProperty[nIndex], hInstance, false);

	//�ʻ���ť
	for ( int nIndex = 0; nIndex < CountArray(m_BtFlower); ++nIndex )
		m_BtFlower[nIndex].SetButtonImage(m_PropertyViewImage.uBtGift[nIndex], hInstance, false);

	//���ư�ť
	m_btTopMoveToL.SetButtonImage(m_PropertyViewImage.uBtPropertyToLeft, hInstance, false);
	m_btTopMoveToR.SetButtonImage(m_PropertyViewImage.uBtPropertyToRight, hInstance, false);
	m_btBtmMoveToL.SetButtonImage(m_PropertyViewImage.uBtPropertyToLeft, hInstance, false);
	m_btBtmMoveToR.SetButtonImage(m_PropertyViewImage.uBtPropertyToRight, hInstance, false);

	return true;
}

//�������
void CPropertyBar::SetPurchaseInfoSink(IPurchaseInfo *pIPurchaseInfo)
{
	m_pIPurchaseInfo = pIPurchaseInfo;

	return;
}

//��Ϣ����
BOOL CPropertyBar::PreTranslateMessage(MSG * pMsg)
{
	m_ToolTipCtrl.RelayEvent(pMsg);
	return __super::PreTranslateMessage(pMsg);
}

//�ƶ���Ϣ
void CPropertyBar::OnBnClickedTopMoveToR()
{
	if ( m_bInitDialog == false ) return ;

	//���ñ���
	m_nTopButtonHead--;
	while (m_PropertyInfo[m_nTopButtonHead].bNullity==true) m_nTopButtonHead--;
	int nButtonHead=m_nTopButtonHead-1;
	bool bLastBt=false;
	if (m_nTopButtonHead==0) bLastBt=true;
	if (nButtonHead>0)
	{
		while (m_PropertyInfo[nButtonHead].bNullity==true)
		{
			nButtonHead--;
			if (nButtonHead<0)
			{
				bLastBt=true;
				break;
			}
		}
	}

	//���ð�ť
	if (bLastBt) m_btTopMoveToR.EnableWindow(FALSE);
	m_btTopMoveToL.EnableWindow(TRUE);

	//���¿ؼ�
	UpdateTopButton(m_nTopButtonHead);

	return;
}

//�ƶ���Ϣ
void CPropertyBar::OnBnClickedTopMoveToL()
{
	if ( m_bInitDialog == false ) return ;

	//���ñ���
	m_nTopButtonHead++;
	while (m_PropertyInfo[m_nTopButtonHead].bNullity==true) m_nTopButtonHead++;

	//���ð�ť
	m_btTopMoveToR.EnableWindow(TRUE);
	int nLeftButtonCount = 0;
	for ( int nIndex = 0; nIndex < m_nTopButtonHead; ++ nIndex ) 
		if ( m_PropertyInfo[nIndex].bNullity == false ) 
			nLeftButtonCount++;
	if ( m_nPropertyCount - nLeftButtonCount <= m_nButtonCount ) m_btTopMoveToL.EnableWindow(FALSE);
	else m_btTopMoveToL.EnableWindow(TRUE);

	//���¿ؼ�
	UpdateTopButton(m_nTopButtonHead);

	return;
}


//�ƶ���Ϣ
void CPropertyBar::OnBnClickedBtmMoveToL()
{
	if ( m_bInitDialog == false ) return ;

	//���ñ���
	m_nBtmButtonHead++;
	while (m_FlowerInfo[m_nBtmButtonHead].bNullity==true) m_nBtmButtonHead++;

	
	//���ð�ť
	m_btBtmMoveToR.EnableWindow(TRUE);
	int nLeftButtonCount = 0;
	for ( int nIndex = 0; nIndex < m_nBtmButtonHead; ++ nIndex ) if ( m_FlowerInfo[nIndex].bNullity == false ) nLeftButtonCount++;
	if ( m_nFlowerCount - nLeftButtonCount <= m_nButtonCount ) m_btBtmMoveToL.EnableWindow(FALSE);
	else m_btBtmMoveToL.EnableWindow(TRUE);

	//���¿ؼ�
	UpdateBtmButton(m_nBtmButtonHead);

	return;
}

//�ƶ���Ϣ
void CPropertyBar::OnBnClickedBtmMoveToR()
{
	if ( m_bInitDialog == false ) return ;

	//���ñ���
	m_nBtmButtonHead--;
	while (m_FlowerInfo[m_nBtmButtonHead].bNullity==true) m_nBtmButtonHead--;
	int nButtonHead=m_nBtmButtonHead-1;
	bool bLastBt=false;
	if (m_nBtmButtonHead==0) bLastBt=true;
	if (nButtonHead>0)
	{
		while (m_FlowerInfo[nButtonHead].bNullity==true)
		{
			nButtonHead--;
			if (nButtonHead<0)
			{
				bLastBt=true;
				break;
			}
		}
	}

	//���ð�ť
	if ( m_nBtmButtonHead == 0 ) m_btBtmMoveToR.EnableWindow(FALSE);
	m_btBtmMoveToL.EnableWindow(TRUE);

	//���¿ؼ�
	UpdateBtmButton(m_nBtmButtonHead);

	return;
}

//�����¼�
void CPropertyBar::PropertyEvent(int nPropertyID)
{
	//�����ж�
	if ( m_pPropertyItem->m_hWnd == NULL )
	{
		ASSERT(m_pIPurchaseInfo != NULL);
		m_pPropertyItem->SetPurchaseInfoSink(m_pIPurchaseInfo);
		m_pPropertyItem->Create(IDD_PROPERTY_ITEM, this);
	}

	//��������
	m_pPropertyItem->SetPropertyID(nPropertyID);
	m_pPropertyItem->CenterWindow();
	m_pPropertyItem->ShowWindow(SW_SHOW);
	m_pPropertyItem->BringWindowToTop();

	return;
}

//�ʻ�����
void CPropertyBar::FlowerEvent(UINT nFlowerID)
{
	//�����ж�
	if ( m_pFlowerSetting->m_hWnd == NULL )
	{
		ASSERT(m_pIPurchaseInfo != NULL);
		m_pFlowerSetting->SetPurchaseInfoSink(m_pIPurchaseInfo);
		m_pFlowerSetting->Create(IDD_FLOWER_SETTING, this);
		
	}

	//��������
	m_pFlowerSetting->SetFlowerID(nFlowerID);
	m_pFlowerSetting->CenterWindow();
	m_pFlowerSetting->ShowWindow(SW_SHOW);
	m_pFlowerSetting->BringWindowToTop();

	return;
}

//��������
void CPropertyBar::CreateBar(CWnd *pParent)
{
	if ( m_hWnd == NULL )
	{
		Create(IDD_PROPERTY_BAR, pParent);
	}
	return;
}

//���ٺ���
void CPropertyBar::DestroyBar()
{
	if ( m_hWnd != NULL ) DestroyWindow();
	return;
}

//��ʼ����
void CPropertyBar::SetPropertyInfo(int nPropID, tagPropertyInfo const & PropertyInfo)
{
	//������֤
	ASSERT(nPropID < PROPERTY_COUNT);
	if ( nPropID >= PROPERTY_COUNT ) return;

    //����Ԫ��
	m_PropertyInfo[nPropID] = PropertyInfo;
	m_PropertyInfo[nPropID].bNullity = false;
	m_nPropertyCount++;

	if (m_PropertyInfo[m_nTopButtonHead].bNullity==true || m_nTopButtonHead>nPropID) m_nTopButtonHead=nPropID;

	//����λ��
	CRect rcClient;
	GetClientRect(rcClient);
	RectifyControl(rcClient.Width(), rcClient.Height());

	return;
}

//��ʼ����
void CPropertyBar::SetFlowerInfo (int nFlowerID, tagFlowerInfo const & FlowerInfo)
{
	//������֤
	ASSERT(nFlowerID < FLOWER_COUNT);
	if ( nFlowerID >= FLOWER_COUNT ) return;

	//����Ԫ��
	m_FlowerInfo[nFlowerID] = FlowerInfo;
	m_FlowerInfo[nFlowerID].bNullity = false;
	m_nFlowerCount++;

	if (m_PropertyInfo[m_nBtmButtonHead].bNullity==true || m_nBtmButtonHead>nFlowerID) m_nBtmButtonHead=nFlowerID;

	//����λ��
	CRect rcClient;
	GetClientRect(rcClient);
	RectifyControl(rcClient.Width(), rcClient.Height());

	return;
}

//ѡ���û�
void CPropertyBar::SetUserDataInfo(tagUserData *pUserData)
{
	if(m_pFlowerSetting->m_hWnd != NULL && m_pFlowerSetting->IsWindowVisible()==TRUE)
	{
		m_pFlowerSetting->SetUserDataInfo(pUserData);
	}

	if(m_pPropertyItem->m_hWnd != NULL && m_pPropertyItem->IsWindowVisible()==TRUE)
	{
		m_pPropertyItem->SetUserDataInfo(pUserData);
	}

	return;
}

//������Ϣ
BOOL CPropertyBar::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	struct DealWithList
	{
		WORD wCommandID;
		UINT uFunctionPar;
	}CommandList[]=
	{
		IDC_BT_FLOWER1,		FLOWER_1,
		IDC_BT_FLOWER2,		FLOWER_2,
		IDC_BT_FLOWER3,		FLOWER_3,
		IDC_BT_FLOWER4,		FLOWER_4,
		IDC_BT_FLOWER5,		FLOWER_5,
		IDC_BT_FLOWER6,		FLOWER_6,
		IDC_BT_FLOWER7,		FLOWER_7,
		IDC_BT_FLOWER8,		FLOWER_8,
		IDC_BT_FLOWER9,		FLOWER_9,
		IDC_BT_FLOWER10,	FLOWER_10,
		IDC_BT_FLOWER11,	FLOWER_11,

		IDC_BT_PROPERTY1,	PROP_DOUBLE,
		IDC_BT_PROPERTY2,	PROP_FOURDOLD,
		IDC_BT_PROPERTY3,	PROP_NEGAGIVE,
		IDC_BT_PROPERTY4,	PROP_FLEE,
		IDC_BT_PROPERTY5,	PROP_BUGLE,
		IDC_BT_PROPERTY6,	PROP_KICK,
		IDC_BT_PROPERTY7,	PROP_SHIELD,
		IDC_BT_PROPERTY8,	PROP_MEMBER_1,
		IDC_BT_PROPERTY9,	PROP_MEMBER_2,
		IDC_BT_PROPERTY10,	PROP_MEMBER_3,
		IDC_BT_PROPERTY11,	PROP_MEMBER_4,
		IDC_BT_PROPERTY12,	PROP_MEMBER_5,
		IDC_BT_PROPERTY13,	PROP_MEMBER_6,
		IDC_BT_PROPERTY14,	PROP_MEMBER_7,
		IDC_BT_PROPERTY15,	PROP_MEMBER_7,
	};

	//������Ϣ
	WORD wContorlID = LOWORD(wParam);
	for(WORD i=0;i<CountArray(CommandList);i++)
	{
		if(wContorlID!=CommandList[i].wCommandID)continue;

		//������Ϣ
		if(i<FLOWER_COUNT)FlowerEvent(CommandList[i].uFunctionPar);
		else PropertyEvent(CommandList[i].uFunctionPar);

		return TRUE;
	}

	return CDialog::OnCommand(wParam, lParam);
}

//////////////////////////////////////////////////////////////////////