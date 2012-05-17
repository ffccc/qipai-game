#include "StdAfx.h"
#include "CardControl.h"
#include "GameScoreWnd.h"
#include "DrawFunc.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameScoreWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameScoreWnd::CGameScoreWnd(IDataCtrl* pDataCtrl)
:	m_pDataCtrl(pDataCtrl)
{
	//���ñ���
	ZeroMemory(&m_GameScoreInfo,sizeof(m_GameScoreInfo));
	m_ImageBack.SetLoadInfo(IDB_GAME_SCORE,AfxGetInstanceHandle());
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

	//m_GameScoreInfo.wBankerUser = 1;
	//for(int i=0; i<4; i++)
	//{
	//	m_GameScoreInfo.lScore[i] = 80;
	//	strcpy(m_GameScoreInfo.szUserName[i], "��Ϫ���");
	//}
	//m_GameScoreInfo.stHuCourt.cbHuCardData = 1;
	//m_GameScoreInfo.stHuCourt.cbHuCardQueueType = EN_HU_CARD_QUEUE_13A;
	//m_GameScoreInfo.stHuCourt.wHuAttachedType = EN_HU_ATTACHED_ZIMO|EN_HU_ATTACHED_TIANDI|EN_HU_ATTACHED_ALL_ASK;
	//m_GameScoreInfo.stHuCourt.wFuseChair = 3;
	//m_GameScoreInfo.stHuCourt.wWinChair = 2;
	//m_GameScoreInfo.stHuCourt.AffordItem[0].wChair = 4;

	
	//�滭�˿�
	BYTE cbHuCardData = m_GameScoreInfo.stHuCourt.cbHuCardData;
	int iX = 57, iY = 23;
	if(cbHuCardData == CARD_KING_DATA_MASK)
	{
		cbHuCardData = m_pDataCtrl->GetKingCardData();
		g_CardResource.m_ImageUserBottom.DrawCardItem(&BufferDC,cbHuCardData,iX,iY);
		RECT rcKing;
		rcKing.left		= iX+2;
		rcKing.top		= iY+5;
		rcKing.right	= iX+g_CardResource.m_ImageUserBottom.GetViewWidth();
		rcKing.bottom	= iY+g_CardResource.m_ImageUserBottom.GetViewHeight()-3;				
		CDrawFunc::DrawAlphaRect(&BufferDC, &rcKing, RGB(255,255,0), 0.6f);
	}
	else
	{
		g_CardResource.m_ImageUserBottom.DrawCardItem(&BufferDC,cbHuCardData,iX,iY);
	}
	
	//��������
	CRect rcDraw;
	TCHAR szBuffer[64];
	szBuffer[0] = 0;

	//��������
	iX = 119; 
	iY = 18;
	rcDraw.left = iX;
	rcDraw.right = rcDraw.left+170;
	rcDraw.top = iY;
	rcDraw.bottom = rcDraw.top+16;
	_snprintf(szBuffer,sizeof(szBuffer),TEXT("�� ע�� %ld"),m_pDataCtrl->GetCellScore());
	BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	if(m_GameScoreInfo.stHuCourt.IsHu())
	{
		//������
		int iPosCal = 0;
		CRect rcOrg = rcDraw + CPoint(0, 16);
		rcOrg.right = rcOrg.left + 85;

		if(m_GameScoreInfo.stHuCourt.wHuCardQueueType == EN_HU_CARD_QUEUE_WW)
		{
			rcDraw = rcOrg + CPoint((iPosCal%2)*85,(iPosCal/2)*16);
			iPosCal ++;
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("ȫ����  Ӯ��"));
			BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		}
		else
		{
			rcDraw = rcOrg + CPoint((iPosCal%2)*85,(iPosCal/2)*16);
			iPosCal ++;
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("ƽ  �� %d��"),SCORE_ADD_COMMON);
			BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

			if(m_GameScoreInfo.stHuCourt.wHuCardQueueType != EN_HU_CARD_QUEUE_3A2)
			{
				rcDraw = rcOrg + CPoint((iPosCal%2)*85,(iPosCal/2)*16);
				iPosCal ++;
				switch(m_GameScoreInfo.stHuCourt.wHuCardQueueType)
				{
				case EN_HU_CARD_QUEUE_3A2:
					break;
				case EN_HU_CARD_QUEUE_7TS:
					_snprintf(szBuffer,sizeof(szBuffer),TEXT("�߶Ժ� %d��"),SCORE_ADD_7TS);
					break;
				case EN_HU_CARD_QUEUE_13A:
					_snprintf(szBuffer,sizeof(szBuffer),TEXT("13���� %d��"),SCORE_ADD_13A);
					break;
				case EN_HU_CARD_QUEUE_W7T:
					_snprintf(szBuffer,sizeof(szBuffer),TEXT("���߶� %d��"),SCORE_ADD_W7T);
					break;
				}
				BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
			}

			if(m_GameScoreInfo.stHuCourt.wHuAttachedType & EN_HU_ATTACHED_ZIMO)
			{
				_snprintf(szBuffer,sizeof(szBuffer),TEXT("��  �� %d��"),SCORE_TIME_ZIMO);
				rcDraw = rcOrg + CPoint((iPosCal%2)*85,(iPosCal/2)*16);
				iPosCal ++;
				BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
			}
			if(m_GameScoreInfo.stHuCourt.wHuAttachedType & EN_HU_ATTACHED_TIANDI)
			{
				_snprintf(szBuffer,sizeof(szBuffer),TEXT("��غ� %d��"),SCORE_ADD_TIANDI);
				rcDraw = rcOrg + CPoint((iPosCal%2)*85,(iPosCal/2)*16);
				iPosCal ++;
				BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
			}
			if(m_GameScoreInfo.stHuCourt.wHuAttachedType & EN_HU_ATTACHED_KING_WAIT)
			{
				_snprintf(szBuffer,sizeof(szBuffer),TEXT("�׻��� %d��"),SCORE_ADD_KING_WAIT);
				rcDraw = rcOrg + CPoint((iPosCal%2)*85,(iPosCal/2)*16);
				iPosCal ++;
				BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
			}
			if(m_GameScoreInfo.stHuCourt.wHuAttachedType & EN_HU_ATTACHED_WHOLE_COLOR)
			{
				_snprintf(szBuffer,sizeof(szBuffer),TEXT("��һɫ %d��"),SCORE_ADD_WHOLE_COLOR);
				rcDraw = rcOrg + CPoint((iPosCal%2)*85,(iPosCal/2)*16);
				iPosCal ++;
				BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
			}
			if(m_GameScoreInfo.stHuCourt.wHuAttachedType & EN_HU_ATTACHED_ALL_ASK)
			{
				_snprintf(szBuffer,sizeof(szBuffer),TEXT("ץ��ȸ %d��"),SCORE_ADD_ALL_ASK);
				rcDraw = rcOrg + CPoint((iPosCal%2)*85,(iPosCal/2)*16);
				iPosCal ++;
				BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
			}
			if(m_GameScoreInfo.stHuCourt.wHuAttachedType & EN_HU_ATTACHED_GANG_FLOWER)
			{
				_snprintf(szBuffer,sizeof(szBuffer),TEXT("���ϻ� %d��"),SCORE_ADD_GANG_FLOWER);
				rcDraw = rcOrg + CPoint((iPosCal%2)*85,(iPosCal/2)*16);
				iPosCal ++;
				BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
			}
			if(m_GameScoreInfo.stHuCourt.wHuAttachedType & EN_HU_ATTACHED_7WINDS)
			{
				_snprintf(szBuffer,sizeof(szBuffer),TEXT("�߷��� %d��"),SCORE_ADD_7WINDS);
				rcDraw = rcOrg + CPoint((iPosCal%2)*85,(iPosCal/2)*16);
				iPosCal ++;
				BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
			}
			
			switch(m_GameScoreInfo.stHuCourt.cbKingCount)
			{
			case 0:
				_snprintf(szBuffer,sizeof(szBuffer),TEXT("�޲��� 2��"));
				break;
			case 1:
				_snprintf(szBuffer,sizeof(szBuffer),TEXT("һ���� 1��"));
				break;
			case 2:;
				_snprintf(szBuffer,sizeof(szBuffer),TEXT("������ 2��"));
				break;
			case 3:
				_snprintf(szBuffer,sizeof(szBuffer),TEXT("������ 3��"));
				break;
			case 4:
				_snprintf(szBuffer,sizeof(szBuffer),TEXT("�Ĳ��� 4��"));
				break;
			}
			rcDraw = rcOrg + CPoint((iPosCal%2)*85,(iPosCal/2)*16);
			iPosCal ++;
			BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

			BYTE cbCount = m_GameScoreInfo.stHuCourt.cbBankerCount;
			if(cbCount > 1)
			{
				_snprintf(szBuffer,sizeof(szBuffer),TEXT("%d��ׯ %d��"), cbCount-1, (cbCount-1)*2);
				rcDraw = rcOrg + CPoint((iPosCal%2)*85,(iPosCal/2)*16);
				iPosCal ++;
				BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
			}
		}		
	}
	

	//�û��ɼ�
	iX = 20;
	iY = 138;
	for (int i=0;i<GAME_PLAYER;i++)
	{
		//�û�����
		rcDraw.left=iX;
		rcDraw.right=rcDraw.left+97;
		rcDraw.top=iY+i*25;
		rcDraw.bottom=rcDraw.top+12;
		BufferDC.DrawText(m_GameScoreInfo.szUserName[i],lstrlen(m_GameScoreInfo.szUserName[i]),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//�û�����
		rcDraw.left=rcDraw.right;
		rcDraw.right=rcDraw.left+53;
		rcDraw.top=iY+i*25;
		rcDraw.bottom=rcDraw.top+12;
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),m_GameScoreInfo.lScore[i]);
		BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

		//�û�״̬
		szBuffer[0] = 0;
		rcDraw.left = rcDraw.right;
		rcDraw.right=rcDraw.left+44;
		rcDraw.top=iY+i*25;
		rcDraw.bottom=rcDraw.top+12;
		if (m_GameScoreInfo.stHuCourt.wWinChair == i) 
		{		
			if(m_GameScoreInfo.stHuCourt.wHuAttachedType & EN_HU_ATTACHED_ZIMO)
			{
				strcpy(szBuffer, _T("����"));
			}
			else
			{
				strcpy(szBuffer, _T("����"));
			}
		}
		else if (m_GameScoreInfo.stHuCourt.AffordItem[0].wChair==i) 
		{
			strcpy(szBuffer, _T("����"));
		}
		else if(m_GameScoreInfo.stHuCourt.wFuseChair==i) 
		{
			strcpy(szBuffer, _T("����"));
		}
		BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

		//ׯ��
		szBuffer[0] = 0;
		rcDraw.left = rcDraw.right;
		rcDraw.right=rcDraw.left+75;
		rcDraw.top=iY+i*25;
		rcDraw.bottom=rcDraw.top+12;
		if (m_GameScoreInfo.wBankerUser==i) 
		{
			strcpy(szBuffer, _T("��"));
			BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		}
		
	}

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
