#include "StdAfx.h"
#include "Resource.h"
#include "SkinDialog.h"
#include "SkinResourceManager.h"

//////////////////////////////////////////////////////////////////////////////////
//��������

//Բ�Ǵ�С
#define ROUND_CX					5									//Բ�ǿ��
#define ROUND_CY					5									//Բ�Ǹ߶�

//��Ӱ����
#define SHADOW_CX					5									//��Ӱ���
#define SHADOW_CY					5									//��Ӱ�߶�

//��ť��ʶ
#define BST_CLOSE					0									//�رհ�ť
#define BST_MAX_SIZE				1									//��ԭ��ť
#define BST_MIN_SIZE				2									//��С��ť

//��ť״̬
#define BUT_EXIST					0x01								//��ť����
#define BUT_NORMAL					0x02								//��ť����
#define BUT_DISABLE					0x04								//��ť��ֹ

//////////////////////////////////////////////////////////////////////////////////
//���ɱ���

//ʱ���ʶ
#define IDI_TRANSITION				10									//���ɱ�ʶ
#define TIME_TRANSITION				30									//����ʱ��

//��������
#define MIN_TRANSITION_INDEX		0									//��������
#define MAX_TRANSITION_INDEX		8									//��������

//////////////////////////////////////////////////////////////////////////////////

//ö�ٽṹ
struct tagEnumChildInfo
{
	CWnd *							pWndControl;						//���ƴ���
	CWnd *							pWndLayered;						//�ֲ㴰��
	CRgn *							pRegionResult;						//�������
};

//////////////////////////////////////////////////////////////////////////////////
//��̬����

CSkinDialogAttribute				CSkinDialog::m_SkinAttribute;		//��Դ����

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSkinLayered, CWnd)
	ON_WM_CLOSE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CSkinDialog, CDialog)
	
	//ϵͳ��Ϣ
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_WINDOWPOSCHANGED()

	//�����Ϣ
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()

	//�Զ���Ϣ
	ON_MESSAGE(WM_SETTEXT,OnSetTextMesage)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CSkinDialogAttribute::CSkinDialogAttribute()
{
	//������ɫ
	m_crTitleText=RGB(0,0,0);
	m_crTitleFrame=RGB(0,0,0);
	m_crBackGround=RGB(0,0,0);
	m_crControlText=RGB(0,0,0);

	//��������
	tagEncircleResource EncircleResource;
	HINSTANCE hResInstance=GetModuleHandle(SKIN_CONTROL_DLL_NAME);

	//������Դ
	EncircleResource.pszImageTL=TEXT("SKIN_WND_TL");
	EncircleResource.pszImageTM=TEXT("SKIN_WND_TM");
	EncircleResource.pszImageTR=TEXT("SKIN_WND_TR");
	EncircleResource.pszImageML=TEXT("SKIN_WND_ML");
	EncircleResource.pszImageMR=TEXT("SKIN_WND_MR");
	EncircleResource.pszImageBL=TEXT("SKIN_WND_BL");
	EncircleResource.pszImageBM=TEXT("SKIN_WND_BM");
	EncircleResource.pszImageBR=TEXT("SKIN_WND_BR");
	m_PNGEncircleFrame.InitEncircleResource(EncircleResource,hResInstance);

	//��ȡ��С
	CBitImage ImageButtonClose;
	ImageButtonClose.LoadFromResource(hResInstance,IDB_SKIN_WND_CLOSE);
	m_SizeButton.SetSize(ImageButtonClose.GetWidth()/5,ImageButtonClose.GetHeight());

	//������ͼ
	m_PNGEncircleFrame.GetEncircleInfo(m_EncircleInfoFrame);

	//������ͼ
	m_EncircleInfoView.nBBorder=8;
	m_EncircleInfoView.nTBorder=m_EncircleInfoFrame.nTBorder;
	m_EncircleInfoView.nLBorder=m_EncircleInfoFrame.nLBorder;
	m_EncircleInfoView.nRBorder=m_EncircleInfoFrame.nRBorder;

	return;
}
	
//��������
CSkinDialogAttribute::~CSkinDialogAttribute()
{
	//������Դ
	if (m_brBackGround.GetSafeHandle()!=NULL)
	{
		m_brBackGround.DeleteObject();
	}

	return;
}

//������Դ
bool CSkinDialogAttribute::Initialization(CSkinRenderManager * pSkinRenderManager)
{
	//������ɫ
	m_crTitleText=RGB(0,0,0);
	m_crTitleFrame=RGB(200,200,200);

	//�ؼ���ɫ
	m_crControlText=RGB(10,10,10);
	m_crBackGround=RGB(250,250,250);

	//������Դ
	m_brBackGround.DeleteObject();
	m_brBackGround.CreateSolidBrush(m_crBackGround);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CSkinLayered::CSkinLayered()
{
	//���ñ���
	m_pWndControl=NULL;

	return;
}

//��������
CSkinLayered::~CSkinLayered()
{
}

//��������
VOID CSkinLayered::CreateLayered(CWnd * pWndControl, CRect & rcWindow)
{
	//Ч�����
	ASSERT((pWndControl!=NULL)&&(pWndControl->m_hWnd!=NULL));
	if ((pWndControl==NULL)||(pWndControl->m_hWnd==NULL)) return;

	//���ñ���
	m_pWndControl=pWndControl;

	//��������
	CreateEx(WS_EX_LAYERED,TEXT("STATIC"),TEXT(""),0,rcWindow,pWndControl,0L);

	return;
}

//��������
VOID CSkinLayered::InitLayeredArea(CDC * pDCImage, BYTE cbAlpha, CRect & rcUnLayered, CPoint & PointRound, bool bUnLayeredChild)
{
	//Ч�����
	ASSERT((pDCImage!=NULL)&&(pDCImage->m_hDC!=NULL));
	if ((pDCImage==NULL)||(pDCImage->m_hDC==NULL)) return;

	//��������
	BITMAP Bitmap;
	ZeroMemory(&Bitmap,sizeof(Bitmap));

	//��ȡͼ��
	CBitmap * pBitmap=pDCImage->GetCurrentBitmap();
	if (pBitmap!=NULL) pBitmap->GetBitmap(&Bitmap);

	//��ȡ��С
	CSize SizeImage;
	SizeImage.SetSize(Bitmap.bmWidth,Bitmap.bmHeight);

	//Ч���С
	ASSERT((SizeImage.cx>0)&&(SizeImage.cy>0));
	if ((SizeImage.cx==0)||(SizeImage.cy==0)) return;

	//��������
	BLENDFUNCTION BlendFunction;
	ZeroMemory(&BlendFunction,sizeof(BlendFunction));

	//���ò���
	BlendFunction.BlendOp=0;
	BlendFunction.BlendFlags=0;
	BlendFunction.AlphaFormat=AC_SRC_ALPHA;
	BlendFunction.SourceConstantAlpha=cbAlpha;

	//���÷ֲ�
	CPoint ImagePoint(0,0);
	CClientDC ClientDC(this);
	UpdateLayeredWindow(&ClientDC,NULL,&SizeImage,pDCImage,&ImagePoint,0L,&BlendFunction,ULW_ALPHA);

	//��������
	CRgn RegionResult;
	RegionResult.CreateRectRgn(0,0,SizeImage.cx,SizeImage.cy);

	//�����ų�
	if (bUnLayeredChild==true)
	{
		//��������
		tagEnumChildInfo EnumChildInfo;
		ZeroMemory(&EnumChildInfo,sizeof(EnumChildInfo));

		//���ñ���
		EnumChildInfo.pWndLayered=this;
		EnumChildInfo.pWndControl=m_pWndControl;
		EnumChildInfo.pRegionResult=&RegionResult;

		//ö�ٴ���
		ASSERT(m_pWndControl->GetSafeHwnd()!=NULL);
		EnumChildWindows(m_pWndControl->m_hWnd,EnumChildProc,(LPARAM)&EnumChildInfo);
	}

	//�����ų�
	if (rcUnLayered.IsRectEmpty()==FALSE)
	{
		//��������
		CRgn RegionUnLayered;
		RegionUnLayered.CreateRoundRectRgn(rcUnLayered.left,rcUnLayered.top,rcUnLayered.right+1,rcUnLayered.bottom+1,PointRound.x,PointRound.y);

		//�ϲ�����
		RegionResult.CombineRgn(&RegionResult,&RegionUnLayered,RGN_DIFF);
	}

	//��������
	SetWindowRgn(RegionResult,TRUE);

	return;
}

//��������
VOID CSkinLayered::InitLayeredArea(CPngImage & Image, BYTE cbAlpha, CRect & rcUnLayered, CPoint & PointRound, bool bUnLayeredChild)
{
	//��������
	CImage ImageBuffer;
	ImageBuffer.Create(Image.GetWidth(),Image.GetHeight(),32);

	//�滭����
	CImageDC ImageDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(ImageDC);

	//�滭����
	ASSERT(pBufferDC!=NULL);
	if (pBufferDC!=NULL) Image.DrawImage(pBufferDC,0,0);

	//�����ֲ�
	InitLayeredArea(pBufferDC,cbAlpha,rcUnLayered,PointRound,bUnLayeredChild);

	return;
}

//�ر���Ϣ
VOID CSkinLayered::OnClose()
{
	//Ͷ����Ϣ
	if (m_pWndControl!=NULL)
	{
		m_pWndControl->PostMessage(WM_CLOSE);
	}

	return;
}

//������Ϣ
VOID CSkinLayered::OnSetFocus(CWnd * pOldWnd)
{
	//���ý���
	if (m_pWndControl!=NULL)
	{
		m_pWndControl->SetFocus();
	}
}

//ö�ٺ���
BOOL CALLBACK CSkinLayered::EnumChildProc(HWND hWndChild, LPARAM lParam)
{
	//��ȡλ��
	CRect rcWindow;
	::GetWindowRect(hWndChild,&rcWindow);

	//��������
	if ((rcWindow.Width()>0)&&(rcWindow.Height()>0))
	{
		//��������
		ASSERT(lParam!=0L);
		tagEnumChildInfo * pEnumChildInfo=(tagEnumChildInfo *)lParam;

		//�����ж�
		HWND hWndParent=::GetParent(hWndChild);
		if (hWndParent!=pEnumChildInfo->pWndControl->GetSafeHwnd())
		{
			return TRUE;
		}

		//ת��λ��
		ASSERT(pEnumChildInfo->pWndControl!=NULL);
		pEnumChildInfo->pWndControl->ScreenToClient(&rcWindow);

		//��������
		CRgn RgnChild;
		RgnChild.CreateRectRgn(rcWindow.left,rcWindow.top,rcWindow.right,rcWindow.bottom);

		//�ϲ�����
		ASSERT(pEnumChildInfo->pRegionResult!=NULL);
		pEnumChildInfo->pRegionResult->CombineRgn(pEnumChildInfo->pRegionResult,&RgnChild,RGN_DIFF);
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CSkinDialog::CSkinDialog(UINT nIDTemplate, CWnd * pParentWnd) : CDialog(nIDTemplate,pParentWnd)
{
	//����״̬
	m_bTransition=false;
	ZeroMemory(m_wImageIndex,sizeof(m_wImageIndex));

	//��ť״̬
	m_cbNowHotIndex=INVALID_BYTE;
	m_cbNowHitIndex=INVALID_BYTE;
	ZeroMemory(m_cbButtonState,sizeof(m_cbButtonState));

	return;
}

//��������
CSkinDialog::~CSkinDialog()
{
}

//���ú���
BOOL CSkinDialog::OnInitDialog()
{
	__super::OnInitDialog();

	//��������
	ModifyStyle(WS_CAPTION,0,SWP_DRAWFRAME);

	//��ť״̬
	m_cbNowHotIndex=INVALID_BYTE;
	m_cbNowHitIndex=INVALID_BYTE;
	ZeroMemory(m_cbButtonState,sizeof(m_cbButtonState));

	//�����ж�
	LONG lWindowStyte=GetWindowLongPtr(m_hWnd,GWL_STYLE);
	if (lWindowStyte&WS_SYSMENU)
	{
		//���ð�ť
		m_cbButtonState[BST_CLOSE]=BUT_EXIST|BUT_NORMAL;
		m_cbButtonState[BST_MAX_SIZE]=BUT_EXIST|((lWindowStyte&WS_MAXIMIZEBOX)?BUT_NORMAL:BUT_DISABLE);
		m_cbButtonState[BST_MIN_SIZE]=BUT_EXIST|(((lWindowStyte&WS_MINIMIZEBOX)||(GetParent()==NULL))?BUT_NORMAL:BUT_DISABLE);

		//��ť����
		if (((m_cbButtonState[BST_MAX_SIZE]&BUT_DISABLE)!=0)&&((m_cbButtonState[BST_MIN_SIZE]&BUT_DISABLE)!=0))
		{
			m_cbButtonState[BST_MAX_SIZE]=0;
			m_cbButtonState[BST_MIN_SIZE]=0;
		}
	}

	//�����ֲ�
	CRect rcWindow;
	GetWindowRect(&rcWindow);
	m_SkinLayered.CreateLayered(this,rcWindow);

	//������Դ
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyResource(rcClient.Width(),rcClient.Height());

	return FALSE;
}

//�滭��Ϣ
VOID CSkinDialog::OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight)
{
	return;
}

//������Դ
VOID CSkinDialog::RectifyResource(INT nWidth, INT nHeight)
{
	//���ñ���
	m_cbNowHotIndex=INVALID_BYTE;
	m_cbNowHitIndex=INVALID_BYTE;

	//����״̬
	m_bTransition=false;
	ZeroMemory(m_wImageIndex,sizeof(m_wImageIndex));

	//�����ж�
	if ((nWidth!=0)&&(nHeight!=0))
	{
		//������ť
		CaleTitleButton(nWidth,nHeight);

		//���λ��
		CRect rcFrame;
		rcFrame.SetRect(SHADOW_CX,SHADOW_CY,nWidth-SHADOW_CX,nHeight-SHADOW_CY);

		//��������
		CRgn RegionWindow;
		RegionWindow.CreateRoundRectRgn(rcFrame.left,rcFrame.top,rcFrame.right+1,rcFrame.bottom+1,ROUND_CX,ROUND_CY);

		//��������
		SetWindowRgn(RegionWindow,TRUE);

		//�ֲ㴰��
		if (m_SkinLayered.m_hWnd!=NULL)
		{
			//��������
			CImage ImageBuffer;
			ImageBuffer.Create(nWidth,nHeight,32);

			//���� DC
			CImageDC ImageDC(ImageBuffer);
			CDC * pBufferDC=CDC::FromHandle(ImageDC);

			//�滭����
			DrawControlView(pBufferDC,nWidth,nHeight);

			//���·ֲ�
			m_SkinLayered.InitLayeredArea(pBufferDC,255,rcFrame,CPoint(ROUND_CX,ROUND_CX),false);
		}
	}

	return;
}

//������ť
VOID CSkinDialog::CaleTitleButton(INT nWidth, INT nHeight)
{
	//������ť
	for (INT i=0;i<CountArray(m_rcButton);i++)
	{
		//����ƫ��
		INT nYPos=10;
		INT nXPos=nWidth-(m_SkinAttribute.m_SizeButton.cx+2)*(i+1)-12;

		//����λ��
		m_rcButton[i].top=nYPos;
		m_rcButton[i].left=nXPos;
		m_rcButton[i].right=nXPos+m_SkinAttribute.m_SizeButton.cx;
		m_rcButton[i].bottom=nYPos+m_SkinAttribute.m_SizeButton.cy;
	}

	return;
}

//�滭��Ӱ
VOID CSkinDialog::DrawControlView(CDC * pDC, INT nWdith, INT nHeight)
{
	//���û���
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//���λ��
	CRect rcFrame;
	rcFrame.SetRect(0,0,nWdith,nHeight);

	//��ͼλ��
	CRect rcClientView;
	rcClientView.top=m_SkinAttribute.m_EncircleInfoFrame.nTBorder;
	rcClientView.left=m_SkinAttribute.m_EncircleInfoFrame.nLBorder;
	rcClientView.right=nWdith-m_SkinAttribute.m_EncircleInfoFrame.nRBorder;
	rcClientView.bottom=nHeight-m_SkinAttribute.m_EncircleInfoFrame.nBBorder;

	//�滭��ͼ
	pDC->FillSolidRect(&rcClientView,m_SkinAttribute.m_crBackGround);

	//�滭���
	m_SkinAttribute.m_PNGEncircleFrame.DrawEncircleFrame(pDC,rcFrame);

	//�滭����
	DrawSystemTitle(pDC,nWdith,nHeight);
	DrawSystemButton(pDC,nWdith,nHeight);

	//�滭����
	OnDrawClientArea(pDC,nWdith,nHeight);

	return;
}

//�滭����
VOID CSkinDialog::DrawSystemTitle(CDC * pDC, INT nWdith, INT nHeight)
{
	//��ȡ����
	CString strTitle;
	GetWindowText(strTitle);

	//����ͼ��
	CPngImage ImageLogo;
	ImageLogo.LoadImage(GetModuleHandle(SKIN_CONTROL_DLL_NAME),TEXT("SKIN_WND_LOGO"));

	//�滭ͼ��
	if (ImageLogo.IsNull()==false)
	{
		ImageLogo.DrawImage(pDC,11,6);
	}

	//�滭����
	if (strTitle.IsEmpty()==false)
	{
		//��������
		CRect rcTitle;
		rcTitle.SetRect(48,16,nWdith-95,28);

		//��������
		INT nXMove[8]={1,1,1,0,-1,-1,-1,0};
		INT nYMove[8]={-1,0,1,1,1,0,-1,-1};

		//���û���
		pDC->SetTextColor(m_SkinAttribute.m_crTitleFrame);

		//�滭�߿�
		for (INT i=0;i<CountArray(nXMove);i++)
		{
			//����λ��
			CRect rcTitleFrame;
			rcTitleFrame.top=rcTitle.top+nYMove[i];
			rcTitleFrame.left=rcTitle.left+nXMove[i];
			rcTitleFrame.right=rcTitle.right+nXMove[i];
			rcTitleFrame.bottom=rcTitle.bottom+nYMove[i];

			//�滭�ַ�
			pDC->DrawText(strTitle,&rcTitleFrame,DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER);
		}

		//�滭����
		pDC->SetTextColor(m_SkinAttribute.m_crTitleText);
		pDC->DrawText(strTitle,&rcTitle,DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER);
	}

	return;
}

//�滭��ť
VOID CSkinDialog::DrawSystemButton(CDC * pDC, INT nWdith, INT nHeight)
{
	//��������
	HINSTANCE hResInstance=GetModuleHandle(SKIN_CONTROL_DLL_NAME);

	//�滭��ť
	for (BYTE nIndex=0;nIndex<3;nIndex++)
	{
		if ((m_cbButtonState[nIndex]&BUT_EXIST)!=0)
		{
			//��������
			CBitImage ImageButton;

			//���ذ�ť
			switch (nIndex)
			{
			case BST_CLOSE:		//�رհ�ť
				{
					ImageButton.LoadFromResource(hResInstance,IDB_SKIN_WND_CLOSE);
					break;
				}
			case BST_MIN_SIZE:	//��С��ť
				{
					ImageButton.LoadFromResource(hResInstance,IDB_SKIN_WND_MIN);
					break;
				}
			case BST_MAX_SIZE:	//��ԭ��ť
				{
					ImageButton.LoadFromResource(hResInstance,(IsZoomed()==FALSE)?IDB_SKIN_WND_MAX:IDB_SKIN_WND_RESORE);
					break;
				}
			}

			//��������
			INT nDrawPos=0;
			BYTE cbButtonState=m_cbButtonState[nIndex];
			CSize SizeImageButton=m_SkinAttribute.m_SizeButton;

			//�滭λ��
			if (cbButtonState&BUT_DISABLE) nDrawPos=m_SkinAttribute.m_SizeButton.cx*4;
			else if (m_cbNowHitIndex==nIndex) nDrawPos=m_SkinAttribute.m_SizeButton.cx;
			else if (m_cbNowHotIndex==nIndex) nDrawPos=m_SkinAttribute.m_SizeButton.cx*2;

			//״̬�ж�
			bool bTransitionDraw=false;
			if ((m_cbNowHotIndex==nIndex)&&(m_wImageIndex[nIndex]<MAX_TRANSITION_INDEX)) bTransitionDraw=true;
			if ((m_cbNowHotIndex!=nIndex)&&(m_wImageIndex[nIndex]>MIN_TRANSITION_INDEX)) bTransitionDraw=true;

			//�滭����
			if ((bTransitionDraw==true)&&((cbButtonState&BUT_DISABLE)==0)&&(m_cbNowHitIndex!=nIndex))
			{
				//��������
				CBitImage BufferImage;
				BufferImage.Create(SizeImageButton.cx,SizeImageButton.cy,16);

				//��ȡ�豸
				HDC hBufferDC=BufferImage.GetDC();
				CDC * pBufferDC=CDC::FromHandle(hBufferDC);

				//�滭��ͼ
				if (m_wImageIndex[nIndex]<MAX_TRANSITION_INDEX)
				{
					ImageButton.BitBlt(hBufferDC,0,0,SizeImageButton.cx,SizeImageButton.cy,0,0,SRCCOPY);
				}

				//�滭����
				if (m_wImageIndex[nIndex]>MIN_TRANSITION_INDEX)
				{
					BYTE cbAlphaDepth=m_wImageIndex[nIndex]*255/MAX_TRANSITION_INDEX;
					ImageButton.AlphaDrawImage(pBufferDC,0,0,SizeImageButton.cx,SizeImageButton.cy,
						SizeImageButton.cx*2,0,SizeImageButton.cx,SizeImageButton.cy,cbAlphaDepth);
				}

				//�滭����
				BufferImage.BitBlt(pDC->m_hDC,m_rcButton[nIndex].left,m_rcButton[nIndex].top,SizeImageButton.cx,SizeImageButton.cy,0,0);

				//�ͷ���Դ
				BufferImage.ReleaseDC();
			}
			else
			{
				//�������
				ImageButton.BitBlt(pDC->m_hDC,m_rcButton[nIndex].left,m_rcButton[nIndex].top,SizeImageButton.cx,SizeImageButton.cy,nDrawPos,0);
			}
		}
	}

	return;
}

//��������
UINT CSkinDialog::ControlHitTest(CPoint Point)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��ȡ��Ϣ
	tagEncircleInfo EncircleInfo;
	m_SkinAttribute.m_PNGEncircleFrame.GetEncircleInfo(EncircleInfo);

	//�رհ�ť
	if ((m_cbButtonState[BST_CLOSE]&BUT_EXIST)&&(m_rcButton[BST_CLOSE].PtInRect(Point)))
	{
		return HTCLOSE;
	}

	//���ť
	if ((m_cbButtonState[BST_MAX_SIZE]&BUT_EXIST)&&(m_rcButton[BST_MAX_SIZE].PtInRect(Point)))
	{
		return HTMAXBUTTON;
	}

	//��С��ť
	if ((m_cbButtonState[BST_MIN_SIZE]&BUT_EXIST)&&(m_rcButton[BST_MIN_SIZE].PtInRect(Point)))
	{
		return HTMINBUTTON;
	}

	//�������
	if (Point.y<=m_SkinAttribute.m_EncircleInfoView.nTBorder) return HTCAPTION;

	return HTCLIENT;
}

//�����Ϣ
VOID CSkinDialog::OnMouseMove(UINT nFlags, CPoint Point)
{
	//�жϰ���
	if (m_cbNowHitIndex!=INVALID_BYTE) return;
	
	//��ť����
	for (INT i=0;i<CountArray(m_cbButtonState);i++)
	{
		if ((m_cbButtonState[i]&BUT_EXIST)&&(m_rcButton[i].PtInRect(Point)))
		{
			//��ǰ�ж�
			if (m_cbNowHotIndex==i) return;

			//����״̬
			if (m_cbNowHotIndex!=INVALID_BYTE)
			{
				m_cbButtonState[m_cbNowHotIndex]|=BUT_NORMAL;
				m_cbNowHotIndex=INVALID_BYTE;
			}

			//���״̬
			if ((m_cbButtonState[i]&BUT_EXIST)&&(!(m_cbButtonState[i]&BUT_DISABLE)))
			{
				m_cbNowHotIndex=i;
				m_cbButtonState[m_cbNowHotIndex]&=~BUT_NORMAL;
			}

			//��������
			if (m_bTransition==false)
			{
				//���ñ���
				m_bTransition=true;

				//����ʱ��
				SetTimer(IDI_TRANSITION,TIME_TRANSITION,NULL);
			}

			return;
		}
	}

	//������
	if (m_cbNowHotIndex!=INVALID_BYTE)
	{
		//���ñ���
		m_cbButtonState[m_cbNowHotIndex]|=BUT_NORMAL;
		m_cbNowHotIndex=INVALID_BYTE;

		//��������
		if (m_bTransition==false)
		{
			//���ñ���
			m_bTransition=true;

			//����ʱ��
			SetTimer(IDI_TRANSITION,TIME_TRANSITION,NULL);
		}
	}

	return;
}

//�����Ϣ
VOID CSkinDialog::OnLButtonDown(UINT nFlags, CPoint Point)
{
	//���ý���
	SetFocus();

	//��ť����
	UINT nHitCode=ControlHitTest(Point);

	//��ť����
	if (m_cbNowHitIndex==INVALID_BYTE)
	{
		//�رհ�ť
		if ((nHitCode==HTCLOSE)&&(m_cbButtonState[BST_CLOSE]&BUT_EXIST)&&((m_cbButtonState[BST_CLOSE]&BUT_DISABLE)==0))
		{
			m_cbNowHitIndex=BST_CLOSE;
		}

		//���ť
		if ((nHitCode==HTMAXBUTTON)&&(m_cbButtonState[BST_MAX_SIZE]&BUT_EXIST)&&((m_cbButtonState[BST_MAX_SIZE]&BUT_DISABLE)==0))
		{
			m_cbNowHitIndex=BST_MAX_SIZE;
		}

		//��С��ť
		if ((nHitCode==HTMINBUTTON)&&(m_cbButtonState[BST_MIN_SIZE]&BUT_EXIST)&&((m_cbButtonState[BST_MIN_SIZE]&BUT_DISABLE)==0))
		{
			m_cbNowHitIndex=BST_MIN_SIZE;
		}

		//����ж�
		if (m_cbNowHitIndex!=INVALID_BYTE)
		{
			//����״̬
			SetCapture();
			RedrawWindow(NULL,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

			return;
		}
	}

	//ģ�����
	if ((IsZoomed()==FALSE)&&(nHitCode==HTCAPTION))
	{
		//ģ�����
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));

		return;
	}

	__super::OnNcLButtonDown(nFlags,Point);
}

//�滭����
BOOL CSkinDialog::OnEraseBkgnd(CDC * pDC)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//���� DC
	CImageDC ImageDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(ImageDC);

	//�滭����
	DrawControlView(pBufferDC,rcClient.Width(),rcClient.Height());

	//�滭����
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return TRUE;
}

//ʱ����Ϣ
VOID CSkinDialog::OnTimer(UINT_PTR nIDEvent)
{
	//���ɱ���
	if (nIDEvent==IDI_TRANSITION)
	{
		//��������
		bool bWantContinue=false;

		//��ť����
		for (BYTE i=0;i<3;i++)
		{
			//��������
			if ((m_cbNowHotIndex==i)&&(m_wImageIndex[i]<MAX_TRANSITION_INDEX))
			{
				m_wImageIndex[i]++;
			}

			//��̬����
			if ((m_cbNowHotIndex!=i)&&(m_wImageIndex[i]>MIN_TRANSITION_INDEX))
			{
				m_wImageIndex[i]--;
			}

			//״̬�ж�
			if ((m_wImageIndex[i]>MIN_TRANSITION_INDEX)&&(m_wImageIndex[i]<MAX_TRANSITION_INDEX))
			{
				bWantContinue=true;
			}
		}

		//ɾ��ʱ��
		if (bWantContinue==false)
		{
			//���ñ���
			m_bTransition=false;

			//ɾ��ʱ��
			KillTimer(IDI_TRANSITION);
		}

		//��ȡλ��
		CRect rcClient;
		GetClientRect(&rcClient);

		//�滭��ť
		CClientDC ClientDC(this);
		DrawSystemButton(&ClientDC,rcClient.Width(),rcClient.Height());

		return;
	}

	__super::OnTimer(nIDEvent);
}

//λ����Ϣ
VOID CSkinDialog::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType,cx,cy);

	//������Դ
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyResource(rcClient.Width(),rcClient.Height());

	return;
}

//��ʾ��Ϣ
VOID CSkinDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	__super::OnShowWindow(bShow, nStatus);

	//��ʾ�ֲ�
	if (m_SkinLayered.m_hWnd!=NULL)
	{
		m_SkinLayered.ShowWindow((bShow==FALSE)?SW_HIDE:SW_SHOW);
	}

	return;
}

//λ�øı�
VOID CSkinDialog::OnWindowPosChanged(WINDOWPOS * lpWndPos)
{
	__super::OnWindowPosChanged(lpWndPos);

	//�ƶ��ֲ�
	if ((m_SkinLayered.m_hWnd!=NULL)&&(lpWndPos->cx>=0)&&(lpWndPos->cy>0))
	{
		m_SkinLayered.SetWindowPos(NULL,lpWndPos->x,lpWndPos->y,lpWndPos->cx,lpWndPos->cy,SWP_NOZORDER);
	}

	return;
}

//�����Ϣ
VOID CSkinDialog::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags,Point);

	//��ť����
	if (m_cbNowHitIndex!=INVALID_BYTE)
	{
		//�����ͷ�
		ReleaseCapture();

		//��ť����
		UINT nHitCode=ControlHitTest(Point);

		//�رհ�ť
		if ((nHitCode==HTCLOSE)&&(m_cbNowHitIndex==BST_CLOSE))
		{
			PostMessage(WM_CLOSE,0,0);
		}

		//���ť
		if ((nHitCode==HTMAXBUTTON)&&(m_cbNowHitIndex==BST_MAX_SIZE))
		{
			ShowWindow((IsZoomed()==FALSE)?SW_MAXIMIZE:SW_RESTORE);
		}

		//��С��ť
		if ((nHitCode==HTMINBUTTON)&&(m_cbNowHitIndex==BST_MIN_SIZE))
		{
			ShowWindow(SW_MINIMIZE);
		}

		//��������
		m_cbNowHitIndex=INVALID_BYTE;

		//���½���
		RedrawWindow(NULL,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);
	}

	return;
}

//�ؼ���ɫ
HBRUSH CSkinDialog::OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
	case CTLCOLOR_BTN:
	case CTLCOLOR_STATIC:
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(m_SkinAttribute.m_crControlText);
			return m_SkinAttribute.m_brBackGround;
		}
	}

	return __super::OnCtlColor(pDC,pWnd,nCtlColor);
}

//������Ϣ
LRESULT	CSkinDialog::OnSetTextMesage(WPARAM wParam, LPARAM lParam)
{
	//Ĭ�ϵ���
	LRESULT lResult=DefWindowProc(WM_SETTEXT,wParam,lParam);

	//���½���
	RedrawWindow(NULL,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

	return lResult;
}

//////////////////////////////////////////////////////////////////////////////////
