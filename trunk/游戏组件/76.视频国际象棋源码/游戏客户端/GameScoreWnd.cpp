#include "StdAfx.h"
#include "GameScoreWnd.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameScoreWnd, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameScoreWnd::CGameScoreWnd()
{
	//���ñ���
	memset(&m_GameScoreInfo,0,sizeof(m_GameScoreInfo));

	//������Դ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageBack.SetLoadInfo(IDB_GAME_SCORE,hInstance);

	//��ȡ��С
	CImageHandle BackImageHandle(&m_ImageBack);
	m_ScoreSize.cx=m_ImageBack.GetWidth();
	m_ScoreSize.cy=m_ImageBack.GetHeight();

	return;
}

//��������
CGameScoreWnd::~CGameScoreWnd()
{
}

//�ػ�����
void CGameScoreWnd::OnPaint()
{
	CPaintDC dc(this);

	//������Դ
	CImageHandle BackHandle(&m_ImageBack);

	//�滭����
	m_ImageBack.BitBlt(dc,0,0);

	//���� DC
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(255,255,255));
	dc.SelectObject(CSkinResourceManager::GetDefaultFont());

	//�û��ɼ�
	TCHAR szBuffer[64];
	for (int i=0;i<GAME_PLAYER;i++)
	{
		dc.SetTextAlign((i==0)?TA_LEFT:TA_RIGHT);
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),m_GameScoreInfo.lScore[i]);
		dc.TextOut(20+230*i,66,m_GameScoreInfo.szUserName[i],lstrlen(m_GameScoreInfo.szUserName[i]));
		dc.SetTextAlign(TA_CENTER);
		dc.TextOut(100+75*i,45,szBuffer,lstrlen(szBuffer));
	}

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
