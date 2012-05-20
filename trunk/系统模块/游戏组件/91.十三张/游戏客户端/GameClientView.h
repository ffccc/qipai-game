#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "ScoreView.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
//��Ϣ����
#define IDM_START					(WM_APP+100)						//��ʼ��Ϣ
#define IDM_SETCHIP					(WM_APP+101)						//ѹע��Ϣ
#define IDM_SHOWCARD				(WM_APP+102)						//̯����Ϣ
#define IDM_FRONTCARD				(WM_APP+103)						//����ǰ��  
#define IDM_MIDCARD					(WM_APP+104)						//�����ж�
#define IDM_BACKCARD				(WM_APP+105)						//���ú��
#define IDM_DESCEND_SORT			(WM_APP+106)						//������Ϣ
#define IDM_ASCEND_SORT				(WM_APP+107)						//������Ϣ
#define IDM_COLOR_SORT				(WM_APP+108)						//��ɫ��Ϣ
#define IDM_AUTO_OUT_CARD           (WM_APP+109)                        //�Զ�����
#define IDM_BAO_PAI					(WM_APP+110)                        //������Ϣ
#define IDM_RESTORE_CARD			(WM_APP+111)                        //�ָ��˿�
#define IDM_CALL_BANKER				(WM_APP+112)                        //��ׯ��Ϣ
#define IDM_NO_CALL_BANKER			(WM_APP+113)                        //������Ϣ

////////////////////////////////////////////////////////////////////////////////////////////////

#define SET_CHIP_COUNT				4									//��ע����

//�˿�����
enum enCardType
{
	enHandCard,						//�����˿�
	enSegmentCard,					//�ֶ��˿�
	enAllSegmentCard,				//���зֶ�
	enCompareCard,					//�Ƚ��˿�
	enBankCard,						//ׯ���˿�
	enErrorType,					//��������
};

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//�������
protected:
	WORD							m_wBanker;							//ׯ�����	
	LONG							m_nChip[GAME_PLAYER];				//ѹע��С
	CString							m_strShowCompare;					//��ʾ�Ƚ�
	bool							m_bShowCompare;						//��ʾ�Ƚ�
	bool							m_bShowBankerName;					//��ʾׯ��
	enSegmentType					m_enSegmentType ;					//�Ƚ�����
	LONG							m_wMaxChip;							//�����ע
	bool							m_bNoCallBanker[GAME_PLAYER] ;		//���б�ʶ
	LONG							m_lChipsArray[SET_CHIP_COUNT];		//������ע
	

	//λ�ñ���
private:
	POINT							m_ptCallBankerInfo[GAME_PLAYER] ;	//��ׯ��Ϣ

	//�ؼ�����
	//��ť�ؼ�
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btShowCard;						//̯�ư�ť
	CSkinButton						m_btFrontCard;						//ǰ�հ�ť
	CSkinButton						m_btMidCard;						//�жհ�ť
	CSkinButton						m_btBackCard;						//��հ�ť
	CSkinButton						m_btSetChip[SET_CHIP_COUNT];		//ѹע��ť
	CSkinButton						m_btDescend;						//����ť
	CSkinButton						m_btAscend;							//����ť
	CSkinButton						m_btColor;							//��ɫ��ť
	CSkinButton						m_btAutoOutCard;                    //�Զ�����
	CSkinButton						m_btBaoPai;							//���ư�ť
	CSkinButton						m_btRestoreCard;					//�ָ��˿�
	CSkinButton						m_btCallBanker ;					//��ׯ��ť
	CSkinButton						m_btNoCallBanker ;					//���а�ť

	//�˿˿ؼ�
public:
	CCardControl					m_UserCardControl[GAME_PLAYER];		//�����˿�
	CCardControl					m_AllSegCardControl[GAME_PLAYER][3];//�����ֶ�
	CCardControl					m_SegCardControl[GAME_PLAYER];		//�ֶ��˿�
	CCardControl					m_HandSegCardControl[3];			//���Ϸֶ�
	CCardControl					m_CompareCarControl[GAME_PLAYER];	//�Ա��˿�
	CCardControl					m_BankerCardControl[GAME_PLAYER];	//ׯ���˿�

	//λͼ����
protected:
	CSkinImage						m_ImageBack;						//������Դ
	CSkinImage						m_ImageCenter;						//������Դ
	CSkinImage						m_ImageBanker;						//ׯ��ͼ��
	CSkinImage						m_ImageShowUserChip;				//��ʾѹע
	CSkinImage						m_ImageCallBankerInfo ;				//��ׯ��Ϣ
	CSkinImage						m_ImageChipBack ;					//��ע����

	//�ؼ����
public:
	CScoreView						m_ScoreView;						//������ͼ

#ifdef VIDEO_GAME
	//�ؼ�����
protected:
	CVideoServiceControl			m_DlgVideoService[GAME_PLAYER];		//��Ƶ����
#endif

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();

	//��������
protected:
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);

	//���ܺ���
public:
	//�����˿�
	void SetCardData(const WORD wChairID , const BYTE bCardData[] , const BYTE bCardCount , const enCardType bCardType , const BYTE bSeg=255) ;
	//����ׯ��
	void SetBanker(const WORD wBanker) ;
	//����ѹע
	void SetChip(const WORD wChiarID , const int bChip) ;
	//��ʾ�Ƚ�
	void SetShowCompare(bool bShow , const enSegmentType SegmentType=enErr) ;
	//��ʾׯ��
	void ShowBankerName(bool bShow) ;
	//�����ע
	void SetMaxChip(LONG wMaxChip) ;
	//��ׯ��Ϣ
	void SetCallBankerInfo( WORD wChairID, bool bNoCallBanker ) ;
	//������ע
	void SetChips( LONG lChipsArray[SET_CHIP_COUNT] ) ;

	//�̳к���
private:
	//���ý���
	virtual void ResetGameView();
	//�����ؼ�
	virtual void RectifyGameView(int nWidth, int nHeight);
	//�滭����
	virtual void DrawGameView(CDC * pDC, int nWidth, int nHeight);

	//��Ϣӳ��
protected:
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	//̯�ƺ���
	afx_msg void OnShowCard();
	//����ǰ��
	afx_msg void OnFrontCard();
	//�����ж�
	afx_msg void OnMidCard();
	//���ú��
	afx_msg void OnBackCard();
	//����ѹע
	afx_msg void OnSetChip0() ;
	//����ѹע
	afx_msg void OnSetChip1() ;
	//����ѹע
	afx_msg void OnSetChip2() ;
	//����ѹע
	afx_msg void OnSetChip3() ;
	//��ʼ��ť
	afx_msg void OnStart() ;
	//��������
	afx_msg void 	OnDescendSort() ;
	//��������
	afx_msg void 	OnAscendSort() ;
	//��ɫ����
	afx_msg void 	OnColorSort() ;
	//�Զ�����
	afx_msg void OnAutoOutCard() ;
	//�ָ��˿�
	afx_msg void OnRestoreCard() ;
	//��ׯ��Ϣ
	afx_msg void OnCallBanker() ;
	//������Ϣ
	afx_msg void OnNoCallBanker() ;

	DECLARE_MESSAGE_MAP()

};

//////////////////////////////////////////////////////////////////////////
