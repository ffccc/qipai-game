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
	BufferDC.SelectObject(CSkinAttribute::m_DefaultFont);

	//�滭�齫
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
		if (m_GameScoreInfo.wCardType[i]!=0) BufferDC.TextOut(180,125+i*25,TEXT("����"),4);
		else if (m_GameScoreInfo.wProvideUser==i) BufferDC.TextOut(180,125+i*25,TEXT("����"),4);

		//������Ϣ
		if (m_GameScoreInfo.wBankerUser==i) BufferDC.TextOut(228,125+i*25,TEXT("��"),2);
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
