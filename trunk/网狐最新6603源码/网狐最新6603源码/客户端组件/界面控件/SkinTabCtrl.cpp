#include "StdAfx.h"
#include "SkinTabCtrl.h"
#include "SkinRenderManager.h"
#include "SkinResourceManager.h"

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSkinTabCtrl, CTabCtrl)
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CSkinTabCtrl::CSkinTabCtrl()
{
	//״̬����
	m_bRenderImage=false;

	//���ñ���
	m_crBackGround=RGB(250,250,250);
	m_crControlBorder=RGB(145,168,183);

	return;
}

//��������
CSkinTabCtrl::~CSkinTabCtrl()
{
}

//������Ⱦ
VOID CSkinTabCtrl::SetRenderImage(bool bRenderImage)
{
	//���ñ���
	m_bRenderImage=bRenderImage;

	//���´���
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//������ɫ
VOID CSkinTabCtrl::SetTabCtrlColor(COLORREF crBackGround, COLORREF crControlBorder)
{
	//���ñ���
	m_crBackGround=crBackGround;
	m_crControlBorder=crControlBorder;

	//���½���
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//�ػ���Ϣ
VOID CSkinTabCtrl::OnPaint()
{
	//��������
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//�滭����
	CRect rcClip;
	dc.GetClipBox(&rcClip);
	dc.FillSolidRect(&rcClip,m_crBackGround);

	//���� DC
	dc.SetBkMode(TRANSPARENT);
	dc.SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());
	
	//�滭�߿�
	rcClient.top+=26;
	dc.Draw3dRect(&rcClient,m_crControlBorder,m_crControlBorder);

	//�滭��ť
	CPen LinePen(PS_SOLID,1,m_crControlBorder);
	CPen * pOldPen=dc.SelectObject(&LinePen);

	for (INT i=0;i<GetItemCount();i++)
	{
		//��������
		TCITEM ItemInfo;
		ZeroMemory(&ItemInfo,sizeof(ItemInfo));

		//��������
		TCHAR szBuffer[100];
		ItemInfo.mask=TCIF_TEXT|TCIF_IMAGE;
		ItemInfo.pszText=szBuffer;
		ItemInfo.cchTextMax=sizeof(szBuffer);

		//��ȡ��Ϣ
		CRect rcItem;
		GetItem(i,&ItemInfo);
		GetItemRect(i,&rcItem);

		//��䱳��
		dc.FillSolidRect(&rcItem,m_crBackGround);

		//�滭�߿�
		dc.MoveTo(rcItem.left,rcItem.bottom-1);
		dc.LineTo(rcItem.left,rcItem.top+2);
		dc.LineTo(rcItem.left+2,rcItem.top);
		dc.LineTo(rcItem.right-2,rcItem.top);
		dc.LineTo(rcItem.right,rcItem.top+2);
		dc.LineTo(rcItem.right,rcItem.bottom-1);
		
		//�滭�߿�
		if (i!=GetCurSel())
		{
			rcItem.top+=2;
			dc.SetTextColor(RGB(0,0,0));
			dc.MoveTo(rcItem.left,rcItem.bottom-1);
			dc.LineTo(rcItem.right,rcItem.bottom-1);
			DrawText(dc,szBuffer,lstrlen(szBuffer),&rcItem,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
		}
		else 
		{
			rcItem.top+=3;
			dc.SetTextColor(RGB(0,0,0));
			DrawText(dc,szBuffer,lstrlen(szBuffer),&rcItem,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
		}
	}

	//������Դ
	dc.SelectObject(pOldPen);
	LinePen.DeleteObject();

	return;
}

//////////////////////////////////////////////////////////////////////////////////
