#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "ScoreView.h"
//////////////////////////////////////////////////////////////////////////

//��Ϣ����
#define IDM_START                         (WM_USER+100)                 //��ʼ��Ϣ
#define IDM_OUT_CARD                      (WM_USER+101)                 //������Ϣ
#define IDM_PASS_CARD                     (WM_USER+102)                 //������Ϣ
#define IDM_PAY_TRIBUTE                   (WM_USER+103)                 //������Ϣ
#define IDM_SORT_CARD					  (WM_USER+104)					//������Ϣ
#define IDM_AI							  (WM_USER+105)					//�й���Ϣ

//////////////////////////////////////////////////////////////////////////


//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//��Ԫ����
	friend class CGameClientDlg;
	//���ñ���
protected:
    bool                            m_bDeasilOrder;                      //����˳��
	//�������
protected:
    
    //״̬����
protected:
	CPoint							m_PointUserPass[GAME_PLAYER];		//����λ��
	CPoint							m_PointStick[GAME_PLAYER];			//����λ��
	CPoint                          m_PointGameResult[GAME_PLAYER];     //�ɼ�λ��
	bool                            m_bPass[GAME_PLAYER];               //��������
	BYTE                            m_bCardCount[GAME_PLAYER];          //�˿���Ŀ
	int                             m_bProgress;                        //����λ��
	int                             m_bGameResult[GAME_PLAYER];         //��Ϸ�ɼ�
	Series                          m_Series;                           //��Ϸ����
	BYTE							m_HongTao;							//��������
	BOOL							m_bGamePlaying;						//��Ϸ״̬
	WORD							m_wFirstID;							//��ʼ�û�
	BOOL							m_bFirstShow;						//�Ƿ���ʾ
	BYTE                            m_bLiangCard;						//��ʾ����
	bool							m_bJinGong[GAME_PLAYER];			//��ʾ�ع�
	BOOL							m_bAI;								//�Ƿ��й�
	bool							m_bKanGong[GAME_PLAYER];			//����λ��

    //��ʷ����
public:
	LONG                            m_lAllTurnScore[GAME_PLAYER];		//�ֵܾ÷�
	LONG                            m_lLastTurnScore[GAME_PLAYER];		//�Ͼֵ÷�
	
	//�ؼ�����
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btOutCard;						//���ư�ť
	CSkinButton						m_btPassCard;						//������ť
    CSkinButton						m_btPayTribute;						//������ť
	CSkinButton                     m_btChangeCard;						//������ť
	CSkinButton                     m_btResetCard;						//����ť
	CSkinButton						m_btColorSort;						//��ɫ��ť
	CSkinButton						m_btBombSort;						//ը����ť
	CSkinButton						m_btAI;								//�йܰ�ť
    
	//�˿˿ؼ�
public:
	CCardControl                    m_HandCardControl;                  //�����˿�
	CCardControl                    m_UserCardControl[4];               //�˿���ͼ
	CCardControl                    m_LeaveCardControl[3];              //ʣ���˿�

	//λͼ����
protected:
	CSkinImage						m_ImageBack;						//������Դ
	CSkinImage						m_ImageUserPass;					//������־
	CSkinImage						m_ImageCard;						//ͼƬ��Դ
	CSkinImage						m_ImageStick;						//����ͼƬ
	CSkinImage						m_ImageViewFill;

protected:
	CScoreView						m_ScoreView;						//������ͼ
protected:
	CToolTipCtrl					m_tip[7];							//��ť��ʾ
#ifdef VIDEO_GAME
	CVideoServiceControl			m_DlgVideoService[4];				//��Ƶ����
#endif
	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();

	//�̳к���
private:
	//���ý���
	virtual void ResetGameView();
	//�����ؼ�
	virtual void RectifyGameView(int nWidth, int nHeight);
	//�滭����
	virtual void DrawGameView(CDC * pDC, int nWidth, int nHeight);
#ifdef VIDEO_GAME
	void RectifyVideoView(int nWidth, int nHeight);
#endif
	//�û�˳��
public:
	//����˳��
	void SetUserOrder(bool bDeasilOrder);
	//��ȡ˳��
	bool IsDeasilOrder() { return m_bDeasilOrder; }

	//���ܺ���
public:
	//������Ŀ
	void SetCardCount(WORD wChairID ,BYTE bCardCount);
	//���÷���
	void SetPassFlag(WORD wChairID, bool bPass);
	//������Ϸ״̬
	void SetGamePlay(bool bPlaying);

	//��Ϣӳ��
protected:
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    //��ʼ��ť
	afx_msg void OnStart();
	//���ư�ť
	afx_msg void OnOutCard();
	//������ť
	afx_msg void OnPassCard();
	//������ť
	afx_msg void OnPayTribute();
	//������ť
	afx_msg void OnChangeCard();
	//��������
	afx_msg void OnReset();
	//����ɫ��
	afx_msg void OnColorSort();
	//��ը����
	afx_msg	void OnBombSort();
	//����һ�
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//����й�
	afx_msg void OnAI();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//����ƶ�
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	
	DECLARE_MESSAGE_MAP()
	
	//���ܺ���	
public:
	//����ʼ�û�
	void SetFirstUser(WORD wChairID,bool bshow,BYTE bCard=0);
	//��ʾ������
	void ShowJinGong(WORD wChairID,bool bShow);
	//������ͼ
	void DrawStick(CDC * pDC, POINT x);

};

//////////////////////////////////////////////////////////////////////////
