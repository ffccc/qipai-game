#include "StdAfx.h"
#include "GameLogic.h"
#include "ScoreView.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CScoreView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CScoreView::CScoreView(void)
{
	//���ñ���
	ZeroMemory(&m_ScoreViewInfo,sizeof(m_ScoreViewInfo));

	//������Դ
	m_ImageBack.SetLoadInfo(IDB_GAME_END,AfxGetInstanceHandle());

	return;
}

//��������
CScoreView::~CScoreView(void)
{
}

//������Ϣ
int CScoreView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//��������
	ZeroMemory(&m_ScoreViewInfo,sizeof(m_ScoreViewInfo));

	//���ý���
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	//�ƶ�����
	CImageHandle ImageHandle(&m_ImageBack);
	SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	return 0;
}

//�ػ�����
void CScoreView::OnPaint() 
{
	CPaintDC dc(this); 

	//���� DC
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(0,0,0));
	dc.SelectObject(CSkinResourceManager::GetDefaultFont());

	//�滭����
	CImageHandle ImageHandle(&m_ImageBack);
	m_ImageBack.BitBlt(dc,0,0);

	//��ʾ����
	TCHAR szBuffer[64]=TEXT("");
	for (WORD i=0;i<CountArray(m_ScoreViewInfo.szUserName);i++)
	{
		if (m_ScoreViewInfo.szUserName[i][0]!=0)
		{
			//�������
			CRect rcName(29,i*22+250,136,i*22+272);
			dc.DrawText(m_ScoreViewInfo.szUserName[i],lstrlen(m_ScoreViewInfo.szUserName[i]),&rcName,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

			//����ɼ�
			CRect rcScore(136,i*22+250,249,i*22+272);
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),m_ScoreViewInfo.lGameScore[i]);
			dc.DrawText(szBuffer,lstrlen(szBuffer),&rcScore,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

			//�û�״̬
			if (m_ScoreViewInfo.wWinUser==i)
			{
				CRect rcStatus(249,i*22+250,315,i*22+272);
				dc.DrawText(TEXT("����"),lstrlen(TEXT("����")),&rcStatus,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
			}
			else if (m_ScoreViewInfo.wProvideUser==i)
			{
				CRect rcStatus(249,i*22+250,315,i*22+272);
				dc.DrawText(TEXT("����"),lstrlen(TEXT("����")),&rcStatus,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
			}
		}
	}

	//��ʾ˰��
	if (m_ScoreViewInfo.lGameTax!=0L)
	{
		//�������
		CRect rcName(29,i*22+250,136,i*22+272);
		dc.DrawText(TEXT("˰��"),lstrlen(TEXT("˰��")),&rcName,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//����ɼ�
		CRect rcScore(136,i*22+250,249,i*22+272);
		_snprintf(szBuffer,sizeof(szBuffer),"%ld",m_ScoreViewInfo.lGameTax);
		dc.DrawText(szBuffer,lstrlen(szBuffer),&rcScore,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	}

	//�������
	CWeaveCard WeaveCard;
	CGameLogic GameLogic;
	WeaveCard.SetDirection(Direction_South);
	for (BYTE i=0;i<m_ScoreViewInfo.HuCardInfo.cbWeaveCount;i++)
	{
		//��Ϣ�ռ�
		dc.SetTextColor(RGB(10,10,10));
		CRect rcHuXi(33+i*42,195,59+i*42,211);
		CRect rcWeaveKind(33+i*42,61,59+i*42,79);

		//�滭����
		LPCTSTR pszWeaveKind=NULL;
		switch (m_ScoreViewInfo.HuCardInfo.WeaveItemArray[i].cbWeaveKind)
		{
		case ACK_TI:	{ pszWeaveKind=TEXT("��"); break; }
		case ACK_PAO:	{ pszWeaveKind=TEXT("��"); break; }
		case ACK_WEI:	{ pszWeaveKind=TEXT("��"); break; }
		case ACK_CHI:	{ pszWeaveKind=TEXT("��"); break; }
		case ACK_PENG:	{ pszWeaveKind=TEXT("��"); break; }
		}
		dc.DrawText(pszWeaveKind,lstrlen(pszWeaveKind),&rcWeaveKind,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//�滭��Ϣ
		_itot(GameLogic.GetWeaveHuXi(m_ScoreViewInfo.HuCardInfo.WeaveItemArray[i]),szBuffer,10);
		dc.DrawText(szBuffer,lstrlen(szBuffer),&rcHuXi,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//�������
		WeaveCard.SetControlPoint(36+i*42,82);
		WeaveCard.SetCardData(m_ScoreViewInfo.HuCardInfo.WeaveItemArray[i]);

		//�滭���
		WeaveCard.DrawCardControl(&dc);
	}

	//�滭����
	if (m_ScoreViewInfo.HuCardInfo.cbCardEye!=0)
	{
		//��Ϣ�ռ�
		dc.SetTextColor(RGB(10,10,10));
		CRect rcHuXi(33+i*42,195,59+i*42,211);
		CRect rcWeaveKind(33+i*42,61,59+i*42,79);

		//�滭����
		dc.DrawText(TEXT("��"),lstrlen(TEXT("��")),&rcWeaveKind,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//�滭���
		g_CardResource.m_ImageTableBottom.DrawCardItem(&dc,m_ScoreViewInfo.HuCardInfo.cbCardEye,36+i*42,82+CARD_SPACE);
		g_CardResource.m_ImageTableBottom.DrawCardItem(&dc,m_ScoreViewInfo.HuCardInfo.cbCardEye,36+i*42,82+CARD_SPACE*2);
	}

	//�ܺ�Ϣ
	CRect rcHuXi(192,30,220,44);
	dc.SetTextColor(RGB(250,250,250));
	_snprintf(szBuffer,sizeof(szBuffer),"%ld",m_ScoreViewInfo.HuCardInfo.cbHuXiCount);
	dc.DrawText(szBuffer,lstrlen(szBuffer),&rcHuXi,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	return;
}

//���û���
void CScoreView::ResetScore()
{
	//���ñ���
	ZeroMemory(&m_ScoreViewInfo,sizeof(m_ScoreViewInfo));

	//�滭��ͼ
	if (m_hWnd!=NULL) Invalidate(NULL);
	
	return;
}

//������Ϣ
void CScoreView::SetScoreViewInfo(tagScoreViewInfo & ScoreViewInfo, bool bShowWnd)
{
	//���ñ���
	m_ScoreViewInfo=ScoreViewInfo;

	//��������
	if (m_hWnd!=NULL)
	{
		Invalidate(NULL);
		ShowWindow((bShowWnd==true)?SW_SHOW:SW_HIDE);
	}
	
	return;
}

//////////////////////////////////////////////////////////////////////////
