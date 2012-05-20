#include "StdAfx.h"
#include "GameLogic.h"
#include "ControlWnd.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////

//��ť��ʶ
#define IDC_CHIHU					100						//�Ժ���ť
#define IDC_GIVEUP					101						//������ť
#define ITEM_WIDTH					18						//������
#define ITEM_HEIGHT					58						//����߶�

#define CONTROL_WIDTH				173						//���ƿ��
#define CONTROL_HEIGHT				41						//���Ƹ߶�

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
CControlWnd::CControlWnd()
{
	//���ñ���
	m_cbActionMask=0;
	m_cbCenterCard=0;
	m_PointBenchmark.SetPoint(0,0);
	m_cbDuiHuaNm=0;
	m_cbSanZhaoNm=0;
	m_cbSiZhaoNm=0;
	ZeroMemory(m_cbsiTongHuaNm,sizeof(m_cbsiTongHuaNm));
	ZeroMemory(m_cbWuTongHuaNm,sizeof(m_cbWuTongHuaNm));

	//״̬����
	m_cbItemCount=0;
	m_cbCurrentItem=0xFF;

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
BYTE CControlWnd::SwitchToCardIndex(BYTE cbCardData)
{
	BYTE cbValue=(cbCardData&MASK_VALUE);
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;

	//ȷ���ǺϷ���������
	ASSERT(((cbColor == 0 || cbColor == 1) && (cbValue>0 && cbValue<=10) ) || ((cbColor == 2) && (cbValue>=1 && cbValue<=12)));
	
	//��������֣�ֱ�ӵõ����һλֵ
	if (cbColor == 0 || cbColor==1) return cbValue-1;
	else return  cbValue+9;
}

//���ö��в�����������
void CControlWnd::SetDuiZhaoOperateCardData(BYTE cbCenterCard, BYTE cbActionMask,BYTE HuaCardIndex[])
{
	//�ж��û��Ĳ����Ƿ�Ϊ�涨����,�ԣ����У����й����ֲ���
	ASSERT((cbActionMask&WIK_DUI) != 0 || (cbActionMask&WIK_SAN_ZHAO) != 0 || (cbActionMask&WIK_SI_ZHAO) != 0);
	if ((cbActionMask&WIK_DUI) == 0 && (cbActionMask&WIK_SAN_ZHAO) == 0 && (cbActionMask&WIK_SI_ZHAO) == 0 ) return;

	//���ñ���
	m_cbItemCount=0;
	m_cbCurrentItem=0xFF;
	m_cbActionMask=cbActionMask;
	m_cbCenterCard=cbCenterCard;

	//���ݻ��Ƹ��������
	BYTE cbHuaCardIndex[5];
	CopyMemory(cbHuaCardIndex,HuaCardIndex,sizeof(cbHuaCardIndex));

	//�����������������������ǻ���,�����Ʊ��浽��������������
	if ( (cbCenterCard != 0) && (cbCenterCard & MASK_COLOR) == 0x10)
	{
		BYTE cbCardindex = SwitchToCardIndex(cbCenterCard);
		cbHuaCardIndex[cbCardindex/2]++;
	}
	
	//����֮ǰ�������صı���ֵ
	ZeroMemory(m_cbSiTongCard,sizeof(m_cbSiTongCard));
	ZeroMemory(m_cbsiTongHuaNm,sizeof(m_cbsiTongHuaNm));

	ZeroMemory(m_cbWuTongCard,sizeof(m_cbWuTongCard));
	ZeroMemory(m_cbWuTongHuaNm,sizeof(m_cbWuTongHuaNm));

	m_cbDuiHuaNm = 0;
	m_cbSanZhaoNm = 0;
	m_cbSiZhaoNm = 0;

	//1�����ж��ƴ���
	if ((m_cbActionMask&WIK_DUI)!=0)
	{
		//��������������1
		m_cbItemCount++;

		//����Ǿ��ƵĻ����򽫻��ƶ��ŵ������
		BYTE cbCardindex = SwitchToCardIndex(cbCenterCard);
		if( cbCardindex == 0 || cbCardindex == 2 || cbCardindex == 4 || cbCardindex == 6 || cbCardindex == 8)
		{
			m_cbDuiHuaNm += cbHuaCardIndex[cbCardindex/2];
		}
	}
	//2���������д���
	if ((m_cbActionMask&WIK_SAN_ZHAO)!=0)
	{
		//��������������1
		m_cbItemCount++;

		//����Ǿ��ƵĻ����򽫻��ƶ��ŵ������
		BYTE cbCardindex = SwitchToCardIndex(cbCenterCard);
		if( cbCardindex == 0 || cbCardindex == 2 || cbCardindex == 4 || cbCardindex == 6 || cbCardindex == 8)
		{
			m_cbSanZhaoNm += cbHuaCardIndex[cbCardindex/2];
		}
	}
	//3���������д���
	if ((m_cbActionMask&WIK_SI_ZHAO)!=0)
	{
		//��������������1
		m_cbItemCount++;

		//����Ǿ��ƵĻ����򽫻��ƶ��ŵ������
		BYTE cbCardindex = SwitchToCardIndex(cbCenterCard);
		if( cbCardindex == 0 || cbCardindex == 2 || cbCardindex == 4 || cbCardindex == 6 || cbCardindex == 8)
		{
			m_cbSiZhaoNm += cbHuaCardIndex[cbCardindex/2];
		}
	}

	m_btChiHu.EnableWindow(false);

	//�����ؼ�
	RectifyControl();
	//��ʾ����
	ShowWindow(SW_SHOW);

	return;
}

//���ò��Ʋ�����������
void CControlWnd::SetTongOperateCardData(BYTE cbCenterCard, BYTE cbActionMask, tagSiTongResult &SiTongCardResult,tagWuTongResult &WuTongCardResult,BYTE HuaCardIndex[])
{
	//�ж��û��Ĳ����Ƿ�Ϊ�涨����,�Ĳأ���ع����ֲ���
	ASSERT((cbActionMask&WIK_SI_TONG) != 0 ||(cbActionMask&WIK_WU_TONG) != 0 );
	if ((cbActionMask&WIK_SI_TONG)== 0 &&(cbActionMask&WIK_WU_TONG) == 0 ) return;

	//���ñ���
	m_cbItemCount=0;
	m_cbCurrentItem=0xFF;

	m_cbActionMask=cbActionMask;
	m_cbCenterCard=cbCenterCard;

	//���ݻ��Ƹ��������
	BYTE cbHuaCardIndex[5];
	CopyMemory(cbHuaCardIndex,HuaCardIndex,sizeof(cbHuaCardIndex));

	//�����������������������ǻ���,�����Ʊ��浽��������������
	if ( (cbCenterCard != 0) && (cbCenterCard & MASK_COLOR) == 0x10)
	{
		BYTE cbCardindex = SwitchToCardIndex(cbCenterCard);
		cbHuaCardIndex[cbCardindex/2]++;
	}

	//����֮ǰ�������صı���ֵ
	ZeroMemory(m_cbSiTongCard,sizeof(m_cbSiTongCard));
	ZeroMemory(m_cbsiTongHuaNm,sizeof(m_cbsiTongHuaNm));

	ZeroMemory(m_cbWuTongCard,sizeof(m_cbWuTongCard));
	ZeroMemory(m_cbWuTongHuaNm,sizeof(m_cbWuTongHuaNm));

	m_cbDuiHuaNm = 0;
	m_cbSanZhaoNm = 0;
	m_cbSiZhaoNm = 0;

	//1�������Ĳش���
	if ((m_cbActionMask & WIK_SI_TONG)!=0)
	{
		//��һ�����Ĳص����
		for (BYTE i=0;i<SiTongCardResult.cbCardCount;i++) 
		{
			//��������������1
			m_cbItemCount++;

			//���ô��Ĳ���ϵ�������
			m_cbSiTongCard[i]=SiTongCardResult.cbCardData[i];

			//����Ǿ��ƵĻ����򽫻��ƶ��ŵ������
			BYTE cbCardindex = SwitchToCardIndex(m_cbSiTongCard[i]);
			if( cbCardindex == 0 || cbCardindex == 2 || cbCardindex == 4 || cbCardindex == 6 || cbCardindex == 8)
			{
				m_cbsiTongHuaNm[i]= cbHuaCardIndex[cbCardindex/2];
			}
			else
			{
				m_cbsiTongHuaNm[i]= 0;
			}
		}
	}
	//2��������ش���
	if ((m_cbActionMask & WIK_WU_TONG)!=0)
	{
		for (BYTE i=0;i<WuTongCardResult.cbCardCount;i++) 
		{
			//��������������1
			m_cbItemCount++;

			//���ô��Ĳ���ϵ�������
			m_cbWuTongCard[i]=WuTongCardResult.cbCardData[i];

			//����Ǿ��ƵĻ����򽫻��ƶ��ŵ������
			BYTE cbCardindex = SwitchToCardIndex(m_cbWuTongCard[i]);
			if( cbCardindex == 0 || cbCardindex == 2 || cbCardindex == 4 || cbCardindex == 6 || cbCardindex == 8)
			{
				m_cbWuTongHuaNm[i]= 2;
			}
			else
			{
				m_cbWuTongHuaNm[i]= 0;
			}
		}
	}

	m_btChiHu.EnableWindow(false);

	//�����ؼ�
	RectifyControl();
	//��ʾ����
	ShowWindow(SW_SHOW);

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
	rcRect.top=m_PointBenchmark.y-ITEM_HEIGHT*m_cbItemCount-CONTROL_HEIGHT;
	if (m_cbItemCount>0) rcRect.top-=5;

	//�ƶ�����
	MoveWindow(&rcRect);

	//������ť
	CRect rcButton;
	m_btChiHu.GetWindowRect(&rcButton);
	int nYPos=rcRect.Height()-rcButton.Height()-9;
	m_btGiveUp.SetWindowPos(NULL,rcRect.Width()-rcButton.Width()-40,nYPos,0,0,SWP_NOZORDER|SWP_NOSIZE);
	m_btChiHu.SetWindowPos(NULL,rcRect.Width()-rcButton.Width()*2-60,nYPos,0,0,SWP_NOZORDER|SWP_NOSIZE);

	return;
}

//�Ժ���ť
void CControlWnd::OnChiHu()
{
	AfxGetMainWnd()->PostMessage(IDM_CARD_OPERATE,WIK_CHI_HU,0);
	return;
}

//������ť
void CControlWnd::OnGiveUp()
{
	AfxGetMainWnd()->PostMessage(IDM_CARD_OPERATE,WIK_NULL,0);
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
	BYTE cbItemKind[3]={WIK_DUI,WIK_SAN_ZHAO,WIK_SI_ZHAO};
	
	//�滭�˿�
	for (BYTE i=0;i<CountArray(cbItemKind);i++)
	{
		if ((m_cbActionMask&cbItemKind[i])!=0)
		{
			//1���滭�����˿�
  			if((m_cbActionMask & cbItemKind[i])==WIK_DUI)
			{
				if(m_cbDuiHuaNm != 0)
				{
					for(BYTE k=0;k<m_cbDuiHuaNm;k++)
					{
						BYTE cbCardData=m_cbCenterCard;
						g_CardResource.m_ImageTableBottom1.DrawCardItem(&BufferDC,cbCardData,k*26+12,nYPos+5);
					}
				}
				for (BYTE j=m_cbDuiHuaNm;j<3;j++)
				{
					BYTE cbCardData=m_cbCenterCard;
					g_CardResource.m_ImageTableBottom.DrawCardItem(&BufferDC,cbCardData,j*26+12,nYPos+5);
				}
			}
			//2���滭����
			else if((m_cbActionMask & cbItemKind[i])==WIK_SAN_ZHAO)
			{
				if(m_cbSanZhaoNm!=0)
				{
					for(BYTE k=0;k<m_cbSanZhaoNm;k++)
					{
						BYTE cbCardData=m_cbCenterCard;
						g_CardResource.m_ImageTableBottom1.DrawCardItem(&BufferDC,cbCardData,k*26+12,nYPos+5);
					}
				}
				for (BYTE j=m_cbSanZhaoNm;j<4;j++)
				{
					BYTE cbCardData=m_cbCenterCard;
					g_CardResource.m_ImageTableBottom.DrawCardItem(&BufferDC,cbCardData,j*26+12,nYPos+5);
				}
			}
			//3���滭����
			else if((m_cbActionMask&cbItemKind[i])==WIK_SI_ZHAO)
			{
				if(m_cbSiZhaoNm!=0)
				{
					for(BYTE k=0;k<m_cbSiZhaoNm;k++)
					{
						BYTE cbCardData=m_cbCenterCard;
						g_CardResource.m_ImageTableBottom1.DrawCardItem(&BufferDC,cbCardData,k*26+8,nYPos+5);
					}
				}
				for (BYTE j=m_cbSiZhaoNm;j<5;j++)
				{
					BYTE cbCardData=m_cbCenterCard;
					g_CardResource.m_ImageTableBottom.DrawCardItem(&BufferDC,cbCardData,j*26+8,nYPos+5);
				}
			}

			//����λ��
			int nXImagePos=0;
			int nItemWidth=m_ImageActionExplain.GetWidth()/7;
			if ((m_cbActionMask&cbItemKind[i])==WIK_SAN_ZHAO||
				(m_cbActionMask&cbItemKind[i])==WIK_SI_ZHAO) nXImagePos=nItemWidth;
			
			//�滭��־
			int nItemHeight=m_ImageActionExplain.GetHeight();
			m_ImageActionExplain.BitBlt(BufferDC,126,nYPos+5,nItemWidth,nItemHeight,nXImagePos,0);

			//�滭�߿�
			if (cbCurrentItem==m_cbCurrentItem)
			{
				BufferDC.Draw3dRect(7,nYPos,rcClient.Width()-14,ITEM_HEIGHT,RGB(255,255,0),RGB(255,255,0));
			}

			//���ñ���
			++cbCurrentItem;
			nYPos+=ITEM_HEIGHT;		
		}
	}

	//���Ĳص���--��ͬһ�β�����ʾ�У�������ʾ����(����ǰ�ܲض�û�вصģ����ڶ���ʾ����)
	for (BYTE i=0;i<CountArray(m_cbSiTongCard);i++)
	{
		//��ʾ���Ƶ����
		if (m_cbSiTongCard[i]!=0)
		{
			if(m_cbsiTongHuaNm[i]!=0)
			{
				for(BYTE k=0;k<m_cbsiTongHuaNm[i];k++)
				{
					g_CardResource.m_ImageTableBottom1.DrawCardItem(&BufferDC,m_cbSiTongCard[i],k*26+12,nYPos+5);
				}
			}
			for (BYTE j=m_cbsiTongHuaNm[i];j<4;j++)
			{
				g_CardResource.m_ImageTableBottom.DrawCardItem(&BufferDC,m_cbSiTongCard[i],j*26+12,nYPos+5);
			}

			//�滭�߿�
			if (cbCurrentItem==m_cbCurrentItem)
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
	}

	//����ͳ����
	for (BYTE i=0;i<CountArray(m_cbWuTongCard);i++)
	{
		//��ͨ����
		if (m_cbWuTongCard[i]!=0)
		{

			if(m_cbWuTongHuaNm[i]!=0)
			{
				for(BYTE k=0;k<m_cbWuTongHuaNm[i];k++)
				{
					g_CardResource.m_ImageTableBottom1.DrawCardItem(&BufferDC,m_cbWuTongCard[i],k*26+8,nYPos+5);
				}
			}
		
			for (BYTE j=m_cbWuTongHuaNm[i];j<5;j++)
			{
				g_CardResource.m_ImageTableBottom.DrawCardItem(&BufferDC,m_cbWuTongCard[i],j*26+8,nYPos+5);
			}

			//�滭�߿�
			if (cbCurrentItem==m_cbCurrentItem)
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
	}

	//�滭����--���һ����ʾ����
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
	if (m_cbCurrentItem!=0xFF)
	{
		//��������
		BYTE cbIndex=0;
		BYTE cbItemKind[3]={WIK_DUI,WIK_SAN_ZHAO,WIK_SI_ZHAO};

		//��������
		for (BYTE i=0;i<CountArray(cbItemKind);i++)
		{
			if (((m_cbActionMask&cbItemKind[i])!=0)&&(m_cbCurrentItem==cbIndex++))
			{
				AfxGetMainWnd()->PostMessage(IDM_CARD_OPERATE,cbItemKind[i],m_cbCenterCard);
				return;
			}
		}

		//��ͳ������
		for (BYTE i=0;i<CountArray(m_cbSiTongCard);i++)
		{
			if ((m_cbSiTongCard[i]!=0)&&(m_cbCurrentItem==cbIndex++))
			{
				AfxGetMainWnd()->PostMessage(IDM_CARD_OPERATE,WIK_SI_TONG,m_cbSiTongCard[i]);
				return;
			}
		}

		//��ͳ������
		for (BYTE i=0;i<CountArray(m_cbWuTongCard);i++)
		{
			if ((m_cbWuTongCard[i]!=0)&&(m_cbCurrentItem==cbIndex++))
			{
				AfxGetMainWnd()->PostMessage(IDM_CARD_OPERATE,WIK_WU_TONG,m_cbWuTongCard[i]);
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
	if (m_cbItemCount!=0)
	{
		//��ȡλ��
		CRect rcClient;
		CPoint MousePoint;
		GetClientRect(&rcClient);
		GetCursorPos(&MousePoint);
		ScreenToClient(&MousePoint);
		
		//��������
		BYTE bCurrentItem=0xFF;
		CRect rcItem(5,5,rcClient.Width()-5,ITEM_HEIGHT*m_cbItemCount+5);
		if (rcItem.PtInRect(MousePoint)) bCurrentItem=(BYTE)((MousePoint.y-7)/ITEM_HEIGHT);

		//��������
		if (m_cbCurrentItem!=bCurrentItem)
		{
			Invalidate(FALSE);
			m_cbCurrentItem=bCurrentItem;
		}
		
		//���ù��
		if (m_cbCurrentItem!=0xFF)
		{
			SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
			return TRUE;
		}
	}

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//////////////////////////////////////////////////////////////////////////