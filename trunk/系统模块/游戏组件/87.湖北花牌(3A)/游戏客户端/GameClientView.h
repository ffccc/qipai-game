#pragma once

#include "Stdafx.h"
#include "ControlWnd.h"
#include "CardControl.h"
#include "GameScoreWnd.h"
#include "HuCardDlg.h"
#include "dragdropmanager.h"

//////////////////////////////////////////////////////////////////////////
//��Ϣ����
#define IDM_TUOGUAN					(WM_USER+103)						//�й���Ϣ
#define IDM_DISTUOGUAN				(WM_USER+104)

#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ
#define IDM_OUT_CARD				(WM_USER+101)						//������Ϣ
#define IDM_HD						(WM_USER+102)						//������Ϣ

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
public:			
	bool					m_IsStartSendCard;

	//��־����
protected:
	CDragDropManager        m_ddManager;

	bool					m_bOutCard;							//���Ʊ�־
	bool					m_bWaitOther;						//�ȴ���־
	bool					m_bHuangZhuang;						//��ׯ��־

	//��Ϸ����
protected:
	LONG					m_lCellScore;						//��������
	WORD					m_wBankerUser;						//ׯ���û�
	//�û�״̬
protected:
	BYTE					m_cbCardData;						//�����˿�
	WORD					m_wOutCardUser;						//�����û�
	BYTE					m_cbUserAction[3];					//�û�����

	//λ�ñ���
protected:
	CPoint					m_UserFlagPos[3];					//��־λ��

	//λͼ����
protected:
	CSkinImage				m_ImageBack;						//����ͼ��
	CSkinImage				m_ImageWait;						//�ȴ���ʾ
	CSkinImage				m_ImageOutCard;						//������ʾ
	CSkinImage				m_ImageUserFlag;					//�û���־
	CSkinImage				m_ImageUserAction;					//�û�����
	CSkinImage				m_ImageActionBack;					//��������
	CSkinImage				m_ImageHuangZhuang;					//��ׯ��־
	CSkinImage				m_ImageHeadBg;						//ͷ�񱳾�
	
public:
	//������ʾ�Ƶĸ�������
	//������Ϣ
	CHeapCard				m_LHeapCard;                       //������ʾʱ����������Ŀ����ʾ�Ƶĸ���
	WORD					m_leaveHeapCardData;               //�û���������е�����Ŀ
	
	//ÿ���û��������Ϣ
	CWeaveCard				m_WeaveCard[3][8];					//����˿�
	
	//ÿ���û��ѳ��Ƶ���Ϣ
	CDiscardCard			m_DiscardCard[3];					//�����˿�
	
	//����0����2���û������Ƶ���Ϣ
	CUserCard				m_UserCard[3];						//�û��˿�--�������û������Ƶ���ʾ
	
	//����0����2���û�����֮�������Ƶ���Ϣ
	CTableCard				m_TableCard[3];						//�����˿�
	
	CHandCard               m_HandCard[9];                     //�����û���������9�������е����

	//�˿˿ؼ�
public:
	BYTE					m_cbCurrentCard;					//��ǰ��
	
	//�ؼ�����
public:

	
	CSkinButton						m_btDisTuoGuan;						//ȡ���йܰ�ť
	CSkinButton						m_btTuoGuan;						//�йܰ�ť


	CSkinButton				m_btStart;							//��ʼ��ť

	CControlWnd				m_ControlWnd;						//������ʾ����
	CGameScoreWnd			m_GameScoreWnd;						//����ʱ�Ƿִ���
	
	CHuCardDlg              m_HuCardDlg;                       //������ʾ����

	BYTE                    m_DecideBankerCard[3];             //Ϊ��ׯ����������
	CPoint                  m_UserDecideCardPos[3];            //�ŷ��Ķ�ׯ���Ƶ�λ��
	bool                    m_bDecideBanker;                   //�Ƿ���ʾ��ׯ����

	int						m_nAreaLeftBorder;					//���д��ڼ����ߴ�С
	int						m_nAreaYValue;						//���򴰿ڵ�y����λ��
	int						m_nAreaWidth;						//�ƴ���������
	CPoint					m_CurAreaPoiBench;					//��ǰ���ڵĻ���
	BYTE					m_cbTongCount[3];					//�������Ҳ��ƴ���
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

	//���ܺ���
public:
	//��������
	void SetCellScore(LONG lCellScore);
	//ׯ���û�
	void SetBankerUser(WORD wBankerUser);
	//��ׯ����
	void SetHuangZhuang(bool bHuangZhuang);
	//��������
	void SetHD(bool bHD);
	//״̬��־
	void SetStatusFlag(bool bOutCard, bool bWaitOther);
	//������Ϣ
	void SetOutCardInfo(WORD wViewChairID, BYTE cbCardData);
	//������Ϣ
	void SetUserAction(WORD wViewChairID, BYTE bUserAction);
	//��õ�ǰ�����������
	int GetCardAreaCount();
	//�����û�����
	void EnableOutCard();
	//��ֹ�û�����
	void DisableOutCard();

	//��������
protected:
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);

	//��Ϣӳ��
protected:
		//�йܰ�ť
	afx_msg void OnDisTuoGuan();
	afx_msg void OnTuoGuan();

	//��ʼ��ť
	afx_msg void OnStart();
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	afx_msg LRESULT OnSonMouseMove(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg LRESULT OnDragEnter(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnDragDrop(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnDragAbort(WPARAM wp, LPARAM lp);
};

//////////////////////////////////////////////////////////////////////////
