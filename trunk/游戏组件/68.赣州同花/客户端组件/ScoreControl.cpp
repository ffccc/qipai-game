#include "StdAfx.h"
#include "GameClient.h"
#include "ScoreControl.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CScoreControl::CScoreControl()
{
	//���ñ���
	m_bValid=false;
	ZeroMemory(&m_ScoreInfo,sizeof(m_ScoreInfo));

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
	m_bValid=false;
	ZeroMemory(&m_ScoreInfo,sizeof(m_ScoreInfo));

	return;
}

//���û���
VOID CScoreControl::SetScoreInfo(const tagScoreInfoC & ScoreInfo)
{
	//���ñ���
	m_bValid=true;
	m_ScoreInfo=ScoreInfo;

	return;
}

//�滭����
VOID CScoreControl::DrawScoreView(CDC * pDC, INT nXPos, INT nYPos)
{
	//��Ч�ж�
	if (m_bValid==false) return;

	//������Դ
	CImageHandle HandleWinLose(&m_ImageWinLose);
	CImageHandle HandleGameScore(&m_ImageGameScore);

	//��������
	INT nXBenchmark=nXPos-m_ImageGameScore.GetWidth()/2;
	INT nYBenchmark=nYPos-m_ImageGameScore.GetHeight()/2;
	UINT nFormat=DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER;

	//�滭����
	m_ImageGameScore.BlendDrawImage(pDC,nXBenchmark,nYBenchmark,RGB(255,0,255),250);

	//�滭����
	pDC->SetTextAlign(TA_LEFT|TA_TOP);
	pDC->SetTextColor(RGB(255,215,0));

	//�滭��Ϣ
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		TCHAR szUserScore[16]=TEXT("");
		_sntprintf(szUserScore,CountArray(szUserScore),TEXT("%I64d"),m_ScoreInfo.lGameScore[i]);

		//λ�ü���
		CRect rcName(nXBenchmark+30,nYBenchmark+70+i*30,nXBenchmark+128,nYBenchmark+124+i*24);
		CRect rcScore(nXBenchmark+128,nYBenchmark+70+i*30,nXBenchmark+203,nYBenchmark+124+i*24);

		//�滭��Ϣ
		pDC->DrawText(szUserScore,lstrlen(szUserScore),&rcScore,nFormat|DT_CENTER);
		pDC->DrawText(m_ScoreInfo.szUserName[i],lstrlen(m_ScoreInfo.szUserName[i]),&rcName,nFormat|DT_CENTER);

		//��Ӯ��־
		INT nImageWidht=m_ImageWinLose.GetWidth()/3;
		INT nImageHeight=m_ImageWinLose.GetHeight();

		//��Ӯ��־
		INT nImageExcursion=nImageWidht;
		if (m_ScoreInfo.lGameScore[i]>0L) nImageExcursion=0;
		if (m_ScoreInfo.lGameScore[i]==0L) nImageExcursion=2*nImageWidht;
		if (m_ScoreInfo.lGameScore[i]<0L) nImageExcursion=nImageWidht;
		m_ImageWinLose.AlphaDrawImage(pDC,nXBenchmark+225,nYBenchmark+80+i*30,nImageWidht,nImageHeight,nImageExcursion,0,RGB(255,0,255));
	}

	return;
}

//////////////////////////////////////////////////////////////////////////
