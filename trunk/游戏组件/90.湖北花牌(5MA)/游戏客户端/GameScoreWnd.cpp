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
	m_lTax=0;
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
	BufferDC.FillSolidRect(rcClient,RGB(24,60,104));
	//�滭����
	CImageHandle BackImageHandle(&m_ImageBack);
	//m_ImageBack.BitBlt(BufferDC,0,0);
	m_ImageBack.AlphaDrawImage(&BufferDC,0,0,RGB(255,0,255));
	//���� DC
	BufferDC.SetBkMode(TRANSPARENT);
	BufferDC.SetTextColor(RGB(255,255,255));
	BufferDC.SelectObject(CSkinResourceManager::GetDefaultFont());

	//�滭�˿�
	if ((m_GameScoreInfo.cbChiHuCard & 0xF0) != 0x10)
	{
		g_CardResource.m_ImageTableBottom.DrawCardItem(&BufferDC,m_GameScoreInfo.cbChiHuCard,80,280);
	}
	else
	{
		g_CardResource.m_ImageTableBottom1.DrawCardItem(&BufferDC,m_GameScoreInfo.cbChiHuCard,80,280);
	}
	//������
	g_CardResource.m_ImageTableBottom1.DrawCardItem(&BufferDC,m_GameScoreInfo.cbGoldCard,210,280);

	/////////////////////////////////
	//��ͳ�Ʒ���Ϊ0���û��ĸ���������������ֵ�����ֺ��Ƶ����
	BYTE cbCount=0;
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		if(m_GameScoreInfo.lScore[i]==0)
		{
			cbCount++;
		}
	}
	CFont newFont,*oldFont;
	newFont.CreatePointFont(125,"����_GB2312",&BufferDC);
	oldFont=BufferDC.SelectObject(&newFont);
	BufferDC.SetTextColor(RGB(253,228,139));
	BufferDC.SetBkMode(TRANSPARENT);

	CRect rcDraw;
	TCHAR strMessage[32];
	int xPoint[4]={15,115,190,250};
	//��ӡ��һ���е��û�����Ϣ
	rcDraw.left=xPoint[0];  rcDraw.right=xPoint[1];
	rcDraw.top=50; 	 rcDraw.bottom=80;
	//��Ϸ��������ʱ���һ���д�ӡ����������
	if(cbCount<2)
	{
		lstrcpyn(strMessage,m_GameScoreInfo.szUserName[m_GameScoreInfo.wWinner],CountArray(strMessage));
		BufferDC.DrawText(strMessage,lstrlen(strMessage),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	}
	//�����˳�ʱ��ӡ�˳�������
	else
	{
		BYTE OutID=255;//�˳��ߺ�
		for(BYTE i=0;i<GAME_PLAYER;i++)
		{
			if(m_GameScoreInfo.lScore[i]!=0)
			{
				lstrcpyn(strMessage,m_GameScoreInfo.szUserName[i],CountArray(strMessage));
				BufferDC.DrawText(strMessage,lstrlen(strMessage),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
			}
		}
	}
	//��ӡ��һ���еĽ�����Ϣ���˳����������ų�
	rcDraw.left=xPoint[1];  rcDraw.right=xPoint[2];
	switch(cbCount)
	{
	case 0:	
		_snprintf(strMessage,sizeof(strMessage),TEXT("����")); break;
	case 1:
		_snprintf(strMessage,sizeof(strMessage),TEXT("����")); break;
	case 2:
		_snprintf(strMessage,sizeof(strMessage),TEXT("�˳�")); break;
	}
	BufferDC.DrawText(strMessage,lstrlen(strMessage),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	
	//��ӡ��һ���еĵ�����Ϣ
	rcDraw.left=xPoint[2];  rcDraw.right=xPoint[3];
	sprintf(strMessage,"���� %d",m_GameScoreInfo.lHuPoint);
	BufferDC.DrawText(strMessage,lstrlen(strMessage),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//��ӡ�����û�����Ϣ����Ϣ
	int xPoint1[5]={15,115,190,250,365};
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		//��ӡ�û�����
		rcDraw.left=xPoint[0];
		rcDraw.right=xPoint[1];
		rcDraw.top=80+i*25;
		rcDraw.bottom=rcDraw.top+25;
		BufferDC.DrawText(m_GameScoreInfo.szUserName[i],lstrlen(m_GameScoreInfo.szUserName[i]),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//�û�����
		rcDraw.left=xPoint1[1];
		rcDraw.right=xPoint1[2];
		_snprintf(strMessage,sizeof(strMessage),TEXT("%ld"),m_GameScoreInfo.lScore[i]);
		BufferDC.DrawText(strMessage,lstrlen(strMessage),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

		//���Ƽ�
		if(m_GameScoreInfo.wWinner!=m_GameScoreInfo.wProvideUser && m_GameScoreInfo.wWinner==i)
		{
			rcDraw.left=xPoint1[2];
			rcDraw.right=xPoint1[3];
			_snprintf(strMessage,sizeof(strMessage),TEXT("����"));
			BufferDC.DrawText(strMessage,lstrlen(strMessage),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
		}
		//����
		if(m_GameScoreInfo.wWinner==m_GameScoreInfo.wProvideUser && m_GameScoreInfo.wWinner==i)
		{
			rcDraw.left=xPoint1[2];
			rcDraw.right=xPoint1[3];
			_snprintf(strMessage,sizeof(strMessage),TEXT("����"));
			BufferDC.DrawText(strMessage,lstrlen(strMessage),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
		}
		//����
		if(m_GameScoreInfo.wProvideUser==i && m_GameScoreInfo.wWinner!=i)
		{
			rcDraw.left=xPoint1[2];
			rcDraw.right=xPoint1[3];
			_snprintf(strMessage,sizeof(strMessage),TEXT("�ų�"));
			BufferDC.DrawText(strMessage,lstrlen(strMessage),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
		}
		//ׯ
		if(m_GameScoreInfo.wBankerUser==i)
		{
			rcDraw.left=xPoint1[3];
			rcDraw.right=xPoint1[4];
			_snprintf(strMessage,sizeof(strMessage),TEXT("ׯ"));
			BufferDC.DrawText(strMessage,lstrlen(strMessage),&rcDraw,DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
		}
	}

	if(cbCount<2)
	{
		//�������
		//���廭�����ϵ���ʼλ��
		WORD xPosition = 50;
		WORD yPosition = 160;

		//�滭�˸���ϵ��˿�
		for( BYTE i=0; i<8; i++)
		{
			//�����˿������ݣ��滭ÿ���˿�����˿���
			for (BYTE j=0; j<m_GameScoreInfo.HuCardWeave.cbCardCount[i]; j++)
			{
				//�滭�˿�,��ͨ��
				if ((m_GameScoreInfo.HuCardWeave.cbCardData[i][j] & 0xF0) != 0x10)
				{
					g_CardResource.m_ImageTableBottom.DrawCardItem(&BufferDC,m_GameScoreInfo.HuCardWeave.cbCardData[i][j],xPosition,yPosition+j*12);
				}
				//������
				else
				{
					g_CardResource.m_ImageTableBottom1.DrawCardItem(&BufferDC,m_GameScoreInfo.HuCardWeave.cbCardData[i][j],xPosition,yPosition+j*12);
				}
				//xPosition += 17;
			}
			xPosition += 25;

		}

		//��Ѿ��ֵ 
		for (BYTE i=0; i<2; i++)
		{
			//�滭�˿�,��ͨ��
			if ((m_GameScoreInfo.HuCardWeave.cbYaKou[i] & 0xF0) != 0x10)
			{
				g_CardResource.m_ImageTableBottom.DrawCardItem(&BufferDC,m_GameScoreInfo.HuCardWeave.cbYaKou[i],xPosition,yPosition+i*12);
			}
			//������
			else
			{
				g_CardResource.m_ImageTableBottom1.DrawCardItem(&BufferDC,m_GameScoreInfo.HuCardWeave.cbYaKou[i],xPosition,yPosition+i*12);
			}
		}
	}
	BufferDC.SelectObject(oldFont);
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
void CGameScoreWnd::SetScoreInfo(tagGameScoreInfo &GameScoreInfo)
{
	//���ñ���
	m_GameScoreInfo=GameScoreInfo;

	//��ʾ����
	Invalidate(NULL);
	ShowWindow(SW_SHOW);

	return;
}

//����˰��
void CGameScoreWnd::SetTax(LONG lTax)
{
	
	if (m_lTax!=lTax)
	{
		m_lTax=lTax;
		Invalidate(NULL);
	}
	return;
}

//////////////////////////////////////////////////////////////////////////
