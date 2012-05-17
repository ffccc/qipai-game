#include "StdAfx.h"
#include "CardControl.h"
#include "GameScoreWnd.h"
#include "GameLogic.h"
#include ".\gamescorewnd.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameScoreWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOVE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameScoreWnd::CGameScoreWnd()
{
	//���ñ���
	ZeroMemory(&m_GameScoreInfo,sizeof(m_GameScoreInfo));
	m_ImageBack.SetLoadInfo(IDB_GAME_SCORE,AfxGetInstanceHandle());

	return;
}

//��������
CGameScoreWnd::~CGameScoreWnd()
{
}

//������Ϣ
int CGameScoreWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//�ƶ�����
	CImageHandle BackImageHandle(&m_ImageBack);
	SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOZORDER|SWP_NOREPOSITION);

	return TRUE;
}

//�ػ�����
void CGameScoreWnd::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CDC BufferDC;
	CBitmap ImageBuffer;
	BufferDC.CreateCompatibleDC(&dc);
	ImageBuffer.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());
	BufferDC.SelectObject(&ImageBuffer);

	//�滭����
	CImageHandle BackImageHandle(&m_ImageBack);
	m_ImageBack.BitBlt(BufferDC,0,0);

	//���� DC
	BufferDC.SetBkMode(TRANSPARENT);
	BufferDC.SetTextColor(RGB(230,230,230));
	BufferDC.SelectObject(CSkinAttribute::m_DefaultFont);

	//�滭�˿�
	g_CardResource.m_ImageUserBottom.DrawCardItem(&BufferDC,m_GameScoreInfo.cbChiHuCard,85,23);

	//�û��ɼ�
	CRect rcDraw;
	TCHAR szBuffer[64];
	for (int i=0;i<GAME_PLAYER;i++)
	{
		//�û�����
		rcDraw.left=25;
		rcDraw.right=125;
		rcDraw.top=125+i*25;
		rcDraw.bottom=rcDraw.top+12;
		BufferDC.DrawText(m_GameScoreInfo.szUserName[i],lstrlen(m_GameScoreInfo.szUserName[i]),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//�û�����
		rcDraw.left=130;
		rcDraw.right=170;
		rcDraw.top=125+i*25;
		rcDraw.bottom=rcDraw.top+12;
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),m_GameScoreInfo.lScore[i]);
		BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

		//�û�״̬
		if (m_GameScoreInfo.wCardType[i]!=0)
			BufferDC.TextOut(180,125+i*25,TEXT("����"),4);
		else if (m_GameScoreInfo.wProvideUser==i)
			BufferDC.TextOut(180,125+i*25,TEXT("����"),4);

		//������Ϣ
		if (m_GameScoreInfo.wBankerUser==i) 
			BufferDC.TextOut(228,125+i*25,TEXT("��"),2);
	}
	
	//��Ϸ˰��		
	if(m_GameScoreInfo.lGameTax!=0L)
	{

		rcDraw.left=25;
		rcDraw.right=125;
		rcDraw.top=125+4*25;
		rcDraw.bottom=rcDraw.top+12;
		LPCTSTR pszTax=TEXT("����˰");
		BufferDC.DrawText(pszTax,lstrlen(pszTax),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);

		rcDraw.left=130;
		rcDraw.right=170;
		rcDraw.top=125+4*25;
		rcDraw.bottom=rcDraw.top+12;
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),m_GameScoreInfo.lGameTax);
		BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}

	//���ƽ��
	int row=0,colum=0;
	WORD  KindIndex[]={CHK_JI_HU,		
		CHK_PING_HU,	
		CHK_PENG_PENG,	
		CHK_QI_XIAO_DUI,	
		CHR_DI,			
		CHR_TIAN,
		CHR_ZI_YI_SE,	
		CHR_HUN_YI_SE,	
		CHK_QING_YI_SE,	
		CHR_QIANG_GANG,
		CHR_YI_TIAO_LONG,
		CHK_QUAN_QIU_REN,
		CHK_DA_SAN_YUAN};

	TCHAR *KindBuffer[]={"��  ��","ƽ  ��","������","��С��","��  ��"," ��  ��",
		"��һɫ","��һɫ","��һɫ","��  ��","һ����","ȫ����","����Ԫ"};

	if(m_GameScoreInfo.wwChiHuKind!=0&&(m_GameScoreInfo.wwChiHuKind&0xff00)==0)
	{	
		if(m_GameScoreInfo.wwChiHuKind!=0)
		{
			BufferDC.TextOut(28+colum*84,250+row*15,KindBuffer[1]);
			if(colum==2)
			{ 
				row++;
				colum=0;
			}
			else
				colum++;
		}
	}
	else
	{
		m_GameScoreInfo.wwChiHuKind&=0xff00;
		for(BYTE i=0;i<CountArray(KindIndex);i++)
		{
			if(i<4&&m_GameScoreInfo.wwChiHuKind&KindIndex[i])
			{
				BufferDC.TextOut(28+colum*84,250+row*15,KindBuffer[i]);
				if(colum==2)
				{ 
					row++;
					colum=0;
				}
				else
					colum++;
			}
			if(i>=4&&m_GameScoreInfo.wChiHuRight&KindIndex[i])
			{
				BufferDC.TextOut(28+colum*84,250+row*15,KindBuffer[i]);
				if(colum==2)
				{ 
					row++;
					colum=0;
				}
				else
					colum++;
			}
		}
	}

/*
		if(m_GameScoreInfo.wwChiHuKind != 0)
		{
	
			m_GameScoreInfo.wwChiHuKind&=0xffff;
			for(BYTE i=0;i<CountArray(KindIndex);i++)
			{
				if(i<4&&m_GameScoreInfo.wwChiHuKind&KindIndex[i])
				{
					BufferDC.TextOut(28+colum*84,250+row*15,KindBuffer[i]);
					if(colum==2)
					{ 
						row++;
						colum=0;
					}
					else
						colum++;
				}
				if(i>=4&&m_GameScoreInfo.wChiHuRight&KindIndex[i])
				{
					BufferDC.TextOut(28+colum*84,250+row*15,KindBuffer[i]);
					if(colum==2)
					{ 
						row++;
						colum=0;
					}
					else
						colum++;
				}
			}
		}*/
	
	//�滭����
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&BufferDC,0,0,SRCCOPY);

	//������Դ
	BufferDC.DeleteDC();
	ImageBuffer.DeleteObject();

	return;
}

//�滭����
BOOL CGameScoreWnd::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

//�����Ϣ
void CGameScoreWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonDown(nFlags,point);

	//��Ϣģ��
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));

	return;
}

//���û���
void CGameScoreWnd::SetScoreInfo(tagGameScoreInfo & GameScoreInfo)
{
	//���ñ���
	m_GameScoreInfo=GameScoreInfo;
/*
	if(m_GameScoreInfo.wwChiHuKind !=0)
		ASSERT(FALSE);
*/

	//��ʾ����
	Invalidate(NULL);
	ShowWindow(SW_SHOW);

	return;
}

//////////////////////////////////////////////////////////////////////////

void CGameScoreWnd::OnMove(int x, int y)
{
	__super::OnMove(x, y);

	//ˢ�½���
	Invalidate(FALSE);
	UpdateWindow();

	return;
}
