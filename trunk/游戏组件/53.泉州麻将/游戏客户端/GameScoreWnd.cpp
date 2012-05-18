#include "StdAfx.h"
#include "CardControl.h"
#include "GameScoreWnd.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameScoreWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
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
	BufferDC.SelectObject(CSkinResourceManager::GetDefaultFont());

	//�滭�˿�
	g_CardResource.m_ImageUserBottom.DrawCardItem(&BufferDC,m_GameScoreInfo.cbChiHuCard,275,71);

	//�û��ɼ�
	CRect rcDraw;
	TCHAR szBuffer[128];
	BYTE bUserCount=0;
	BYTE bRowShow=0;

	//���������� 
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		bRowShow = 0;

		//������� 
		if (m_GameScoreInfo.wCardType[i]==0)
			BufferDC.SetTextColor(RGB(0,0,0));
		else
			BufferDC.SetTextColor(RGB(0,0,255));

		//�û�����
		rcDraw.left=90+i*120;
		rcDraw.right=90+i*120+120;
		rcDraw.top = 150+bRowShow*20;
		rcDraw.bottom = 150+bRowShow*20+20;
		BufferDC.DrawText(m_GameScoreInfo.szUserName[i],lstrlen(m_GameScoreInfo.szUserName[i]),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
		
		bRowShow++;

		//���ׯ�� 
		if(m_GameScoreInfo.wBankerUser==i)
		{			
			rcDraw.top = 150+bRowShow*20;
			rcDraw.bottom = 150+bRowShow*20+20;
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("ׯ��	X %d"),m_GameScoreInfo.bGoOnBankerTime);
			BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

			bRowShow++;

		}

		if(m_GameScoreInfo.wCardType[i]!=0)//�������
		{
			//������Ϣ
			rcDraw.top = 150+bRowShow*20;
			rcDraw.bottom = 150+bRowShow*20+20;
			BufferDC.DrawText(m_GameScoreInfo.m_cHu,lstrlen(m_GameScoreInfo.m_cHu),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

			bRowShow++;
		}
		else if(m_GameScoreInfo.wProvideUser==i)//�������
		{
			rcDraw.top = 150+bRowShow*20;
			rcDraw.bottom = 150+bRowShow*20+20;
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("����"));
			BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

			bRowShow++;

		}

		//������Ϣ
		if(m_GameScoreInfo.bGoldCount[i]>0)
		{
			rcDraw.top = 150+bRowShow*20;
			rcDraw.bottom = 150+bRowShow*20+20;
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("����	 %d ��"),m_GameScoreInfo.bGoldCount[i]);
			BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

			bRowShow++;

		}

		//������Ϣ
		if(m_GameScoreInfo.bGangCount[i]>0)
		{
			rcDraw.top = 150+bRowShow*20;
			rcDraw.bottom = 150+bRowShow*20+20;
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("����	 %d ��"),m_GameScoreInfo.bGangCount[i]);
			BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

			bRowShow++;

		}

		//������Ϣ
		if(m_GameScoreInfo.bFlowerCount[i]>0)
		{
			rcDraw.top = 150+bRowShow*20;
			rcDraw.bottom = 150+bRowShow*20+20;
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("����	 %d ��"),m_GameScoreInfo.bFlowerCount[i]);
			BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

			bRowShow++;

		}

		//�����ϱ�
		if(m_GameScoreInfo.bAllSidesCard[i]==true)
		{
			rcDraw.top = 150+bRowShow*20;
			rcDraw.bottom = 150+bRowShow*20+20;
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("�����ϱ�	 1 ��"));
			BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

			bRowShow++;

		}

		//÷�����
		if(m_GameScoreInfo.bAllPlantCard[i]==true)
		{
			rcDraw.top = 150+bRowShow*20;
			rcDraw.bottom = 150+bRowShow*20+20;
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("÷�����	 1 ��"));
			BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

			bRowShow++;

		}

		//�����ﶬ
		if(m_GameScoreInfo.bAllSeasonCard[i]==true)
		{
			rcDraw.top = 150+bRowShow*20;
			rcDraw.bottom = 150+bRowShow*20+20;
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("�����ﶬ	 1 ��"));
			BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

			bRowShow++;

		}


		//������
		BYTE bTotalPan = m_GameScoreInfo.bGoldCount[i]+m_GameScoreInfo.bFlowerCount[i]+m_GameScoreInfo.bGangCount[i]+
			(m_GameScoreInfo.bAllPlantCard[i]?1:0)+(m_GameScoreInfo.bAllSeasonCard[i]?1:0)+(m_GameScoreInfo.bAllSidesCard[i]?1:0);
	
		rcDraw.top = 340;
		rcDraw.bottom =360;

		//�ϼ�
		if(m_GameScoreInfo.wCardType[i]!=0)//�������
		{			

			LONG lScore = (bTotalPan+m_GameScoreInfo.lCellScore)*m_GameScoreInfo.bGoOnBankerTime;

			_snprintf(szBuffer,sizeof(szBuffer),TEXT("(%d��+%d��)X%d =%d"),bTotalPan,m_GameScoreInfo.lCellScore,m_GameScoreInfo.bGoOnBankerTime,lScore);

			BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
			
		}
		else//�Ǻ������
		{
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("%d��"),bTotalPan);

			BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		}

		//��ҷ���

		rcDraw.top = 360;
		rcDraw.bottom = 380;

		_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),m_GameScoreInfo.lScore[i]);

		BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);	



	}
	/*for (int i=0;i<GAME_PLAYER;i++)
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
		if (m_GameScoreInfo.wCardType[i]!=0) BufferDC.TextOut(180,125+i*25,TEXT("����"),4);
		else if (m_GameScoreInfo.wProvideUser==i) BufferDC.TextOut(180,125+i*25,TEXT("����"),4);

		//������Ϣ
		if (m_GameScoreInfo.wBankerUser==i) BufferDC.TextOut(228,125+i*25,TEXT("��"),2);

		if(m_GameScoreInfo.bFlowerCount[i]>0)
		{
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("%d"),m_GameScoreInfo.bFlowerCount[i]);
			BufferDC.TextOut(270,125+i*25,szBuffer,lstrlen(szBuffer));
		}

		if(m_GameScoreInfo.bGoldCount[i]>0)
		{
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("%d"),m_GameScoreInfo.bGoldCount[i]);
			BufferDC.TextOut(300,125+i*25,szBuffer,lstrlen(szBuffer));
		}
	}
	rcDraw.left=24;
	rcDraw.right=300;
	rcDraw.top=218;
	rcDraw.bottom=rcDraw.top+12;
	BufferDC.DrawText(m_GameScoreInfo.m_cHu,lstrlen(m_GameScoreInfo.m_cHu),&rcDraw,DT_VCENTER|DT_END_ELLIPSIS);*/

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

	//��ʾ����
	Invalidate(NULL);
	ShowWindow(SW_SHOW);

	return;
}

//////////////////////////////////////////////////////////////////////////
