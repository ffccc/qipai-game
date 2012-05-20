//***********************************************
//File		: ScoreWnd.h 
//Time		: 2005-02-25
//Author	: Luwenguang
//Comment	: ��ʾ�÷������
//***********************************************


#pragma once



class CScoreWnd : public CWnd
{
	DECLARE_DYNAMIC(CScoreWnd)

//===============================================
//	��Ա��������
//===============================================
public:
	CScoreWnd();
	virtual ~CScoreWnd();
	void	Init(void);
	void	GetSize(CSize & size);

protected:
	DECLARE_MESSAGE_MAP()


	//===========================================
	//	�����麯������Ϣ��Ӧ����
	//===========================================
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	

	//===========================================
	//	������Ա����
	//===========================================
protected:
	bool	LoadBitmap_(void);
	void	DrawScore(CDC *pDC, CRect &rect);

public:
	void	SetName(int desk, TCHAR *pName);
	void	SetFactScore(int desk, LONG score);


//===============================================
//	��Ա��������
//===============================================
protected:
	TCHAR		m_name[GAME_PLAYER][32];	//�û�����
	LONG		m_factscore[GAME_PLAYER];	//ʵ�ʵ÷�
		
	CSkinImage	m_bkg;

	CRect		m_Rct_Play;
};


//===============================================
//					the end
//===============================================

