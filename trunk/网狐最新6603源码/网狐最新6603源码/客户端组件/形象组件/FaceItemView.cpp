#include "StdAfx.h"
#include "FaceItemView.h"
#include "FaceItemControl.h"

//////////////////////////////////////////////////////////////////////////////////
//�궨��

#define COLOR_BACK_GROUND			RGB(229,233,250)					//������ɫ

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CFaceItemView, CStatic)
	ON_WM_PAINT()
	ON_WM_CREATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CFaceItemView::CFaceItemView()
{
	//��ɫ����
	m_rcBackGround=COLOR_BACK_GROUND;

	//���ñ���
	m_wFaceID=INVALID_WORD;
	m_dwCustonID=INVALID_DWORD;
	ZeroMemory(m_dwCustomFace,sizeof(m_dwCustomFace));

	return;
}

//��������
CFaceItemView::~CFaceItemView()
{
}

//������ɫ
VOID CFaceItemView::SetControlColor(COLORREF rcBackGround)
{
	if (m_rcBackGround!=rcBackGround)
	{
		//���ñ���
		m_rcBackGround=rcBackGround;

		//���½���
		if (m_hWnd!=NULL) Invalidate(FALSE);
	}

	return;
}

//����ͷ��
VOID CFaceItemView::SetSystemFace(WORD wFaceID)
{
	if (m_wFaceID!=wFaceID)
	{
		//���ñ���
		m_wFaceID=wFaceID;

		//��������
		m_dwCustonID=INVALID_DWORD;
		ZeroMemory(m_dwCustomFace,sizeof(m_dwCustomFace));

		//���½���
		if (m_hWnd!=NULL)
		{
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);
		}
	}

	return;
}

//����ͷ��
VOID CFaceItemView::SetCustomFace(DWORD dwCustonID, DWORD dwCustomFace[FACE_CX*FACE_CY])
{
	if (m_dwCustonID!=dwCustonID)
	{
		//����ͷ��
		m_wFaceID=INVALID_WORD;

		//���ñ���
		m_dwCustonID=dwCustonID;
		CopyMemory(m_dwCustomFace,dwCustomFace,sizeof(m_dwCustomFace));

		//���½���
		if (m_hWnd!=NULL)
		{
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);
		}
	}

	return;
}

//������Ϣ
INT CFaceItemView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	__super::OnCreate(lpCreateStruct);

	//���ô���
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	return 0;
}

//�ػ����� 
VOID CFaceItemView::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcRect;
	GetClientRect(&rcRect);
	
	//��ȡ����
	ASSERT(CFaceItemControl::GetInstance()!=NULL);
	IFaceItemControl * pIFaceItemControl=CFaceItemControl::GetInstance();

	//�滭����
	dc.FillSolidRect(&rcRect,m_rcBackGround);

	//�滭ͷ��
	if ((m_wFaceID!=INVALID_WORD)&&(m_dwCustonID==INVALID_DWORD))
	{
		pIFaceItemControl->DrawFaceNormal(&dc,(rcRect.Width()-FACE_CX)/2,(rcRect.Height()-FACE_CY)/2,m_wFaceID);
	}

	//�Զ�ͷ��
	if ((m_wFaceID==INVALID_WORD)&&(m_dwCustonID!=INVALID_DWORD))
	{
		pIFaceItemControl->DrawFaceNormal(&dc,(rcRect.Width()-FACE_CX)/2,(rcRect.Height()-FACE_CY)/2,m_dwCustomFace);
	}

	//�滭�߿�
	pIFaceItemControl->DrawFaceItemFrame(&dc,(rcRect.Width()-FACE_CX)/2,(rcRect.Height()-FACE_CY)/2,ITEM_FRAME_HOVER);

	return;
}

//////////////////////////////////////////////////////////////////////////////////
