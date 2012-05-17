#include "StdAfx.h"
#include "..\\��Ϸ������\\GameLogic.h"
#include "ControlWnd.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////

//��ť��ʶ
#define IDC_CHIHU					100									//�Ժ���ť
#define IDC_GIVEUP					101									//������ť

//λ�ñ�ʶ
#define ITEM_WIDTH					90									//������
#define ITEM_HEIGHT					46									//����߶�
#define CONTROL_WIDTH				173									//���ƿ��
#define CONTROL_HEIGHT				41									//���Ƹ߶�

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_CHIHU, OnChiHu)
	ON_BN_CLICKED(IDC_GIVEUP, OnGiveUp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CControlWnd::CControlWnd(IDataCtrl* pDataCtrl)
: m_pDataCtrl(pDataCtrl)
{
	//���ñ���
	m_cbActionMask=0;
	m_cbCenterCard=0;
	m_PointBenchmark.SetPoint(0,0);
	ZeroMemory(m_cbGangCard,sizeof(m_cbGangCard));

	//״̬����
	m_bItemCount=0;
	m_bCurrentItem=0xFF;

	//������Դ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageControlTop.SetLoadInfo(IDB_CONTROL_TOP,hInstance);
	m_ImageControlMid.SetLoadInfo(IDB_CONTROL_MID,hInstance);
	m_ImageControlButtom.SetLoadInfo(IDB_CONTROL_BOTTOM,hInstance);
	m_ImageActionExplain.SetLoadInfo(IDB_ACTION_EXPLAIN,hInstance);

	return;
}

//��������
CControlWnd::~CControlWnd()
{
}

//��׼λ��
void CControlWnd::SetBenchmarkPos(int nXPos, int nYPos)
{
	//λ�ñ���
	m_PointBenchmark.SetPoint(nXPos,nYPos);

	//�����ؼ�
	RectifyControl();

	return;
}

//�����˿�
void CControlWnd::SetControlInfo(BYTE cbCenterCard, BYTE cbActionMask, BYTE cbGangCard[4])
{
	//�й�
	if(m_pDataCtrl->IsConsign())
	{		
		OnGiveUp();
		ShowWindow(SW_HIDE);
		return;
	}

	//���ñ���
	m_bItemCount=0;
	m_bCurrentItem=0xFF;
	m_cbActionMask=cbActionMask;
	m_cbCenterCard=cbCenterCard;

	//������Ϣ
	ZeroMemory(m_cbGangCard,sizeof(m_cbGangCard));
	if (cbGangCard!=NULL) 
	{
		CopyMemory(m_cbGangCard,cbGangCard,sizeof(BYTE)*4);
	}

	//��������
	for (BYTE i=0;i<CountArray(m_cbGangCard);i++) 
	{
		if (m_cbGangCard[i]!=0)
		{
			m_bItemCount++;
		}
		else 
		{
			break;
		}
	}
	if (m_cbActionMask&ACK_GANG) 
	{
		m_bItemCount++;
		m_cbGangCard[i]=cbCenterCard;
	}

	//������Ŀ
	BYTE cbItemKind[5]={ACK_LEFT,ACK_CENTER,ACK_RIGHT,ACK_PENG};
	for (BYTE i=0;i<CountArray(cbItemKind);i++) 
	{
		if ((m_cbActionMask&cbItemKind[i])!=0) 
		{
			m_bItemCount++;
		}
	}

	//��ť����
	m_btChiHu.EnableWindow(cbActionMask&ACK_CHIHU);

	//�����ؼ�
	RectifyControl();

	//��ʾ����
	ShowWindow(SW_SHOW);

#ifdef _AUTOPALY
	if(m_btChiHu.IsWindowEnabled())
	{
		AfxGetMainWnd()->PostMessage(IDM_CARD_OPERATE,ACK_CHIHU,0);
	}
	else if (cbGangCard!=NULL && cbGangCard[0]!=0) 
	{
		AfxGetMainWnd()->PostMessage(IDM_CARD_OPERATE,ACK_GANG,cbGangCard[0]);
	}
	else if (m_cbActionMask&ACK_PENG) 
	{
		AfxGetMainWnd()->PostMessage(IDM_CARD_OPERATE,ACK_PENG,m_cbCenterCard);
	}
	else if (m_cbActionMask&ACK_LEFT) 
	{
		AfxGetMainWnd()->PostMessage(IDM_CARD_OPERATE,ACK_LEFT,m_cbCenterCard);
	}
	else if	(m_cbActionMask&ACK_CENTER)
	{
		AfxGetMainWnd()->PostMessage(IDM_CARD_OPERATE,ACK_CENTER,m_cbCenterCard);
	}
	else if (m_cbActionMask&ACK_RIGHT)
	{
		AfxGetMainWnd()->PostMessage(IDM_CARD_OPERATE,ACK_RIGHT,m_cbCenterCard);
	}

	m_cbActionMask = 0;
	ShowWindow(SW_HIDE);
#endif

	return;
}

//�����ؼ�
void CControlWnd::RectifyControl()
{
	//����λ��
	CRect rcRect;
	rcRect.right=m_PointBenchmark.x;
	rcRect.bottom=m_PointBenchmark.y;
	rcRect.left=m_PointBenchmark.x-CONTROL_WIDTH;
	rcRect.top=m_PointBenchmark.y-ITEM_HEIGHT*m_bItemCount-CONTROL_HEIGHT;
	if (m_bItemCount>0) rcRect.top-=5;

	//�ƶ�����
	MoveWindow(&rcRect);

	//������ť
	CRect rcButton;
	m_btChiHu.GetWindowRect(&rcButton);
	int nYPos=rcRect.Height()-rcButton.Height()-9;
	m_btGiveUp.SetWindowPos(NULL,rcRect.Width()-rcButton.Width()-25,nYPos,0,0,SWP_NOZORDER|SWP_NOSIZE);
	m_btChiHu.SetWindowPos(NULL,rcRect.Width()-rcButton.Width()*2-45,nYPos,0,0,SWP_NOZORDER|SWP_NOSIZE);

	return;
}

//�Ժ���ť
void CControlWnd::OnChiHu()
{
	AfxGetMainWnd()->PostMessage(IDM_CARD_OPERATE,ACK_CHIHU,0);
	return;
}

//������ť
void CControlWnd::OnGiveUp()
{
	AfxGetMainWnd()->PostMessage(IDM_CARD_OPERATE,ACK_NULL,0);
	return;
}

//�ػ�����
void CControlWnd::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CDC BufferDC;
	CBitmap BufferImage;
	BufferDC.CreateCompatibleDC(&dc);
	BufferImage.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());
	BufferDC.SelectObject(&BufferImage);

	//������Դ
	CImageHandle HandleControlTop(&m_ImageControlTop);
	CImageHandle HandleControlMid(&m_ImageControlMid);
	CImageHandle HandleControlButtom(&m_ImageControlButtom);
	CImageHandle HandleActionExplain(&m_ImageActionExplain);

	//�滭����
	m_ImageControlTop.BitBlt(BufferDC,0,0);
	for (int nImageYPos=m_ImageControlTop.GetHeight();nImageYPos<rcClient.Height();nImageYPos+=m_ImageControlMid.GetHeight())
	{
		m_ImageControlMid.BitBlt(BufferDC,0,nImageYPos);
	}
	m_ImageControlButtom.BitBlt(BufferDC,0,rcClient.Height()-m_ImageControlButtom.GetHeight());

	//��������
	int nYPos=5;
	BYTE cbCurrentItem=0;
	BYTE cbExcursion[3]={0,1,2};
	BYTE cbItemKind[5]={ACK_LEFT,ACK_CENTER,ACK_RIGHT,ACK_PENG};

	//�滭�˿�
	for (BYTE i=0;i<CountArray(cbItemKind);i++)
	{
		if ((m_cbActionMask&cbItemKind[i])>0)
		{
			//�滭�˿�
			for (BYTE j=0;j<((cbItemKind[i]==ACK_GANG)?4:3);j++)
			{
				BYTE cbCardData=m_cbCenterCard;
				if (i<CountArray(cbExcursion)) 
				{
					cbCardData=m_cbCenterCard+j-cbExcursion[i];
				}
				g_CardResource.m_ImageTableBottom.DrawCardItem(&BufferDC,cbCardData,j*26+12,nYPos+5);
			}

			//����λ��
			int nXImagePos=0;
			int nItemWidth=m_ImageActionExplain.GetWidth()/7;
			if ((m_cbActionMask&cbItemKind[i])&ACK_PENG) 
			{
				nXImagePos=nItemWidth;
			}
			else if ((m_cbActionMask&cbItemKind[i])&ACK_GANG) 
			{
				nXImagePos=nItemWidth*3;
			}
			else if ((m_cbActionMask&cbItemKind[i])&ACK_CHIHU) 
			{
				nXImagePos=nItemWidth*4;
			}

			//�滭��־
			int nItemHeight=m_ImageActionExplain.GetHeight();
			m_ImageActionExplain.BitBlt(BufferDC,126,nYPos+5,nItemWidth,nItemHeight,nXImagePos,0);

			//�滭�߿�
			if (cbCurrentItem==m_bCurrentItem)
			{
				int nWidth=((cbItemKind[i]==ACK_GANG)?(ITEM_WIDTH+26):ITEM_WIDTH);
				BufferDC.Draw3dRect(7,nYPos,rcClient.Width()-14,ITEM_HEIGHT,RGB(255,255,0),RGB(255,255,0));
			}

			//���ñ���
			++cbCurrentItem;
			nYPos+=ITEM_HEIGHT;
		}
	}

	//�����˿�
	for (BYTE i=0;i<CountArray(m_cbGangCard);i++)
	{
		if (m_cbGangCard[i]!=0)
		{
			//�滭�˿�
			for (BYTE j=0;j<4;j++)
			{
				g_CardResource.m_ImageTableBottom.DrawCardItem(&BufferDC,m_cbGangCard[i],j*26+12,nYPos+5);
			}

			//�滭�߿�
			if (cbCurrentItem==m_bCurrentItem)
			{
				BufferDC.Draw3dRect(7,nYPos,rcClient.Width()-14,ITEM_HEIGHT,RGB(255,255,0),RGB(255,255,0));
			}

			//�滭��־
			int nItemWidth=m_ImageActionExplain.GetWidth()/7;
			int nItemHeight=m_ImageActionExplain.GetHeight();
			m_ImageActionExplain.BitBlt(BufferDC,126,nYPos+5,nItemWidth,nItemHeight,nItemWidth*3,0);

			//���ñ���
			cbCurrentItem++;
			nYPos+=ITEM_HEIGHT;
		}
		else break;
	}

	//�滭����
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&BufferDC,0,0,SRCCOPY);

	//������Դ
	BufferDC.DeleteDC();
	BufferImage.DeleteObject();

	return;
}

//������Ϣ
int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//������ť
	CRect rcCreate(0,0,0,0);
	m_btGiveUp.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_GIVEUP);
	m_btChiHu.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_CHIHU);

	//����λͼ
	m_btChiHu.SetButtonImage(IDB_BT_CHIHU,AfxGetInstanceHandle(),false);
	m_btGiveUp.SetButtonImage(IDB_BT_GIVEUP,AfxGetInstanceHandle(),false);

	return 0;
}

//�����Ϣ
void CControlWnd::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);

	//�����ж�
	if (m_bCurrentItem!=0xFF)
	{
		//��������
		BYTE cbIndex=0;
		BYTE cbItemKind[5]={ACK_LEFT,ACK_CENTER,ACK_RIGHT,ACK_PENG};

		//��������
		for (BYTE i=0;i<CountArray(cbItemKind);i++)
		{
			if (((m_cbActionMask&cbItemKind[i])!=0)&&(m_bCurrentItem==cbIndex++))
			{
				AfxGetMainWnd()->PostMessage(IDM_CARD_OPERATE,cbItemKind[i],m_cbCenterCard);
				return;
			}
		}

		//��������
		for (BYTE i=0;i<CountArray(m_cbGangCard);i++)
		{
			if ((m_cbGangCard[i]!=0)&&(m_bCurrentItem==cbIndex++))
			{
				AfxGetMainWnd()->PostMessage(IDM_CARD_OPERATE,ACK_GANG,m_cbGangCard[i]);
				return;
			}
		}

		//�������
		ASSERT(FALSE);
	}

	return;
}

//�����Ϣ
BOOL CControlWnd::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//λ�ò���
	if (m_bItemCount!=0)
	{
		//��ȡλ��
		CRect rcClient;
		CPoint MousePoint;
		GetClientRect(&rcClient);
		GetCursorPos(&MousePoint);
		ScreenToClient(&MousePoint);
		
		//��������
		BYTE bCurrentItem=0xFF;
		CRect rcItem(5,5,rcClient.Width()-5,ITEM_HEIGHT*m_bItemCount+5);
		if (rcItem.PtInRect(MousePoint)) bCurrentItem=(BYTE)((MousePoint.y-7)/ITEM_HEIGHT);

		//��������
		if (m_bCurrentItem!=bCurrentItem)
		{
			Invalidate(FALSE);
			m_bCurrentItem=bCurrentItem;
		}

		//���ù��
		if (m_bCurrentItem!=0xFF)
		{
			SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
			return TRUE;
		}
	}

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//////////////////////////////////////////////////////////////////////////
