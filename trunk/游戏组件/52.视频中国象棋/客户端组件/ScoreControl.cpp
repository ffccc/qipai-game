#include "StdAfx.h"
#include "GameClient.h"
#include "ScoreControl.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CScoreControl, CWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CScoreControl::CScoreControl()
{
	//״̬����
	ZeroMemory(&m_GameScoreInfo,sizeof(m_GameScoreInfo));

	//������Դ
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_ImageWinLose.SetLoadInfo(IDB_WIN_LOSE,hResInstance);
	m_ImageGameScore.SetLoadInfo(IDB_GAME_SCORE,hResInstance);

	return;
}

//��������
CScoreControl::~CScoreControl()
{
}

//��λ����
VOID CScoreControl::RestorationData()
{
	//���ñ���
	ZeroMemory(&m_GameScoreInfo,sizeof(m_GameScoreInfo));

	//���ش���
	if (m_hWnd!=NULL) ShowWindow(SW_HIDE);

	return;
}

//���û���
VOID CScoreControl::SetScoreInfo(const tagScoreInfo & ScoreInfo)
{
	//���ñ���
	m_GameScoreInfo=ScoreInfo;

	//���½���
	if (m_hWnd!=NULL) Invalidate(FALSE);

	//��ʾ����
	if (m_hWnd!=NULL) ShowWindow(SW_SHOW);

	return;
}

//�ػ�����
VOID CScoreControl::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CDC DCBuffer;
	CBitmap ImageBuffer;
	DCBuffer.CreateCompatibleDC(&dc);
	ImageBuffer.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());

	//���� DC
	DCBuffer.SetBkMode(TRANSPARENT);
	DCBuffer.SelectObject(&ImageBuffer);
	DCBuffer.SetTextColor(RGB(125,125,125));
	DCBuffer.SelectObject(CSkinResourceManager::GetDefaultFont());

	//������Դ
	CImageHandle HandleWinLose(&m_ImageWinLose);
	CImageHandle HandleGameScore(&m_ImageGameScore);

	//��������
	TCHAR szBuffer[32]=TEXT("");
	UINT nFormat=DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER;

	//�滭����
	m_ImageGameScore.AlphaDrawImage(&DCBuffer,0,0,RGB(255,0,255));

	//�滭����
	DCBuffer.SetTextColor(RGB(10,10,10));
	DCBuffer.SetTextAlign(TA_LEFT|TA_TOP);

	//�ж�ʮ��
	//if(m_GameScoreInfo.cbReason!=REASON_TEN_STEP)
	{
		//�滭��Ϣ
		for (BYTE i=0;i<GAME_PLAYER;i++)
		{
			//��������
			TCHAR szWinRate[16]=TEXT("");
			TCHAR szUserScore[16]=TEXT("");
			_sntprintf(szWinRate,CountChar(szWinRate),TEXT("%5.2f%%"),m_GameScoreInfo.fWinRate[i]);
			_sntprintf(szUserScore,CountChar(szUserScore),TEXT("%I64d"),m_GameScoreInfo.lGameScore[i]);

			//λ�ü���
			CRect rcName(76,133+i*52,169,184+i*52);
			CRect rcScore(170,133+i*52,240,184+i*52);
			CRect rcWinRate(241,133+i*52,306,184+i*52);
			CRect rcLevelName(307,133+i*52,382,184+i*52);

			//�滭��Ϣ
			DCBuffer.DrawText(szWinRate,lstrlen(szWinRate),&rcWinRate,nFormat);
			DCBuffer.DrawText(szUserScore,lstrlen(szUserScore),&rcScore,nFormat);
			DCBuffer.DrawText(m_GameScoreInfo.szUserName[i],lstrlen(m_GameScoreInfo.szUserName[i]),&rcName,nFormat);
			DCBuffer.DrawText(m_GameScoreInfo.szLevelName[i],lstrlen(m_GameScoreInfo.szLevelName[i]),&rcLevelName,nFormat);

			//��Ӯ��־
			INT nImageWidht=m_ImageWinLose.GetWidth()/3;
			INT nImageHeight=m_ImageWinLose.GetHeight();

			//�滭��־
			INT nImageExcursion=0L;
			if (m_GameScoreInfo.cbReason<REASON_AGREE_DRAW)
			{
				if (m_GameScoreInfo.lGameScore[i]<0L) nImageExcursion=nImageWidht;
			}
			else nImageExcursion=nImageWidht*2;
			m_ImageWinLose.AlphaDrawImage(&DCBuffer,22,138+i*52,nImageWidht,nImageHeight,nImageExcursion,0,RGB(255,0,255));
		}
	}

	//��������
	CFont InfoFont;
	InfoFont.CreateFont(-14,0,0,0,700,0,0,0,134,3,2,1,2,TEXT("����"));

	//������Դ
	CFont * pOldFont = DCBuffer.SelectObject(&InfoFont);
	DCBuffer.SetTextColor(RGB(60,60,60));
	DCBuffer.SetTextAlign(TA_CENTER|TA_TOP);

	//��Ϣ����
	TCHAR tCh[256] = TEXT("");

	//����ԭ��
	WORD wMeChiarID = m_GameScoreInfo.wMeChiarID;
	//m_GameScoreInfo.cbReason = REASON_CHESS_FEAZE;
	//m_GameScoreInfo.lGameScore[wMeChiarID]=-10;
	switch(m_GameScoreInfo.cbReason)
	{
	case REASON_CATCH_CHESS:		//��׽����
		{
			if (m_GameScoreInfo.lGameScore[wMeChiarID]<0L)
				_sntprintf(tCh,sizeof(tCh),TEXT("��Ϸ����:��׽���� T_T"));
			else _sntprintf(tCh,sizeof(tCh),TEXT("��Ϸ����:�Է���׽����"));
			break;
		}
	case REASON_CATCH_GENERA:		//��������
		{
			if (m_GameScoreInfo.lGameScore[wMeChiarID]<0L)
				_sntprintf(tCh,sizeof(tCh),TEXT("��Ϸ����:������������ T_T"));
			else _sntprintf(tCh,sizeof(tCh),TEXT("��Ϸ����:�Է���������"));
			break;
		}
	case REASON_CHESS_FEAZE:		//��������
		{
			if (m_GameScoreInfo.lGameScore[wMeChiarID]<0L)
				_sntprintf(tCh,sizeof(tCh),TEXT("������������ T_T"));
			else _sntprintf(tCh,sizeof(tCh),TEXT("���жԷ�,��ʤ! ��.��"));
			break;
		}
	case REASON_KILL_GENERA:		//��������
		{
			if (m_GameScoreInfo.lGameScore[wMeChiarID]<0L)
				_sntprintf(tCh,sizeof(tCh),TEXT("������������ T_T"));
			else _sntprintf(tCh,sizeof(tCh),TEXT("�����Է�,��ʤ! ��.��"));
			break;
		}
	//case REASON_GIVE_UP:			//�������
	//	{
	//		if (m_GameScoreInfo.lGameScore[wMeChiarID]>=0L)
	//			_sntprintf(tCh,sizeof(tCh),TEXT("�Է�����"));
	//		break;
	//	}
	case REASON_OVER_TIME:			//��ʱ����
		{
			if (m_GameScoreInfo.lGameScore[wMeChiarID]<0L)
				_sntprintf(tCh,sizeof(tCh),TEXT("��ʱ���� T_T"));
			//else _sntprintf(tCh,sizeof(tCh),TEXT("��ʤ:�Է���ʱ ��.��"));
			break;
		}
	case REASON_USER_EXIT:			//ǿ������
		{
			_sntprintf(tCh,sizeof(tCh),TEXT("�Է�ǿ��"));
			break;
		}
	//case REASON_AGREE_DRAW:			//ͬ��;�
	//	{
	//		_sntprintf(tCh,sizeof(tCh),TEXT("ͬ�����,˫���к�"));
	//		break;
	//	}
	case REASON_OVER_STEP:			//�����;�
		{
			_sntprintf(tCh,sizeof(tCh),TEXT("��Ϸ����:�ܲ����ﵽ400��,˫���к�"));
			break;
		}
	case REASON_LEISURE_STEP:		//�ղ��;�
		{
			_sntprintf(tCh,sizeof(tCh),TEXT("��Ϸ����:�޳��Ӳ����ﵽ120��,˫���к�."));
			break;
		}
	case REASON_TEN_STEP:			//ʮ������
		{
			_sntprintf(tCh,sizeof(tCh),TEXT("�ܲ�����10������,��Ӯ���Ʒ�."));
			break;
		}
	case REASON_TWENTY_STEP:		//��ʮ����
		{
			_sntprintf(tCh,sizeof(tCh),TEXT("�ܲ�����10��-20��֮��,Ӯ�Ҳ��÷�."));
			break;
		}
	}

	//�滭��Ϣ
	//DCBuffer.TextOut(200,65,tCh);

	//������Դ
	DCBuffer.SelectObject(pOldFont);
	InfoFont.DeleteObject();

	//�滭����
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&DCBuffer,0,0,SRCCOPY);

	//������Դ
	DCBuffer.DeleteDC();
	ImageBuffer.DeleteObject();

	return;
}

//�滭����
BOOL CScoreControl::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();

	return TRUE;
}

//��������
INT CScoreControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//���ñ���
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	//�ƶ�����
	CImageHandle ImageHandle(&m_ImageGameScore);
	SetWindowPos(NULL,0,0,m_ImageGameScore.GetWidth(),m_ImageGameScore.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	////��������
	//CRgn RgnImage;
	//CImageHandle HandleGameScore(&m_ImageGameScore);
	//m_ImageGameScore.CreateImageRegion(RgnImage,RGB(255,0,255));

	////���ô���
	//SetWindowRgn(RgnImage,TRUE);
	//SetWindowPos(NULL,0,0,m_ImageGameScore.GetWidth(),m_ImageGameScore.GetHeight(),SWP_NOZORDER|SWP_NOMOVE);

	return 0;
}

//�����Ϣ
VOID CScoreControl::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//��Ϣģ��
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));

	return;
}

//////////////////////////////////////////////////////////////////////////
