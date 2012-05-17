#pragma once
#include "resource.h"
#include "stdafx.h"

//�������
enum enOperateResult
{
	enOperateResult_NULL,
	enOperateResult_Win,
	enOperateResult_Lost
};

//��¼��Ϣ
struct tagClientGameRecord
{
	enOperateResult					enOperateFlags;						//������ʶ
	BYTE							cbPlayerCount;						//�мҵ���
	BYTE							cbBankerCount;						//ׯ�ҵ���
	BYTE							cbKingWinner;						//����Ӯ��
	bool							bPlayerTwoPair;						//���ӱ�ʶ
	bool							bBankerTwoPair;						//���ӱ�ʶ
};

//��ʷ��¼
#define MAX_SCORE_HISTORY			65									//��ʷ����

//////////////////////////////////////////////////////////////////////////////////////
class CDlgViewChart : public CDialog
{
//	//��ʷ��Ϣ
//public:
//	LONG							m_lMeStatisticScore;				//��Ϸ�ɼ�
//	tagClientGameRecord				m_TraceGameRecordArray[100];		//·����¼
//	tagClientGameRecord				m_GameRecordArray[100];				//�����¼					
//	int								m_TraceGameRecordCount;				//·����Ŀ
//	int								m_GameRecordCount;					//��¼��Ŀ
//	bool							m_bFillTrace[6][29];				//����ʶ
//
//	//��Դ����
//protected:
//	CSkinImage						m_ImageBack;						//����ͼƬ
//	CPngImage						m_PngPlayerFlag;					//�м�ͼƬ
//	CPngImage						m_PngPlayerEXFlag;					//�м�ͼƬ
//	CPngImage						m_PngBankerFlag;					//ׯ��ͼƬ
//	CPngImage						m_PngBankerEXFlag;					//ׯ��ͼƬ
//	CPngImage						m_PngTieFlag;						//ƽ��ͼƬ
//	CPngImage						m_PngTieEXFlag;						//ƽ��ͼƬ
//	CPngImage						m_PngTwopairFlag;					//ƽ��ͼƬ
//
//	//�ؼ�����
//protected:
//	CSkinButton							m_btClose;							//�رհ�ť
//
//	//���溯��
//public:
//	//���½���
//	void UpdateChart(){InvalidateRect(NULL);}
//	//�滭�ٷֱ�
//	void DrawPrecent(CDC *pDC);
//	//�滭���
//	void DrawChart(CDC *pDC);
//	//��������
//	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
//
//	//��Ϣ�ӿ�
//public:
//	//���ý��
//	void SetGameRecord(const tagClientGameRecord &ClientGameRecord);
//	
public:
	CDlgViewChart(); 
	virtual ~CDlgViewChart();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_DYNAMIC(CDlgViewChart)
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
