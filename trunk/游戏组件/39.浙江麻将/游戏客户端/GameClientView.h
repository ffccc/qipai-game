#pragma once

#include "Stdafx.h"
#include "ControlWnd.h"
#include "CardControl.h"
#include "GameScoreWnd.h"
#include "IDataCtrl.h"

//////////////////////////////////////////////////////////////////////////
//��Ϣ����

#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ
#define IDM_OUT_CARD				(WM_USER+101)						//������Ϣ
#define IDM_CONSIGN_CHANGED			(WM_USER+102)						//�й���Ϣ

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//��Ԫ����
	friend class CGameClientDlg;

	//��Ϸ����
protected:
	IDataCtrl*						m_pDataCtrl;						//���ݿ���
	bool							m_bOutCard;							//���Ʊ�־
	bool							m_bWaitOther;						//�ȴ���־
	WORD							m_wBankerUser;						//ׯ���û�

	//�û�״̬
protected:
	BYTE							m_cbCardData;						//�����˿�
	WORD							m_wOutCardUser;						//�����û�
	BYTE							m_bUserAction[4];					//�û�����

	//λ�ñ���
protected:
	CPoint							m_UserFlagPos[4];					//��־λ��

	//λͼ����
protected:
	CSkinImage						m_ImageBack;						//����ͼ��
	CSkinImage						m_ImageWait;						//�ȴ���ʾ
	CSkinImage						m_ImageOutCard;						//������ʾ
	CSkinImage						m_ImageUserFlag;					//�û���־
	CSkinImage						m_ImageUserAction;					//�û�����
	CSkinImage						m_ImageActionBack;					//��������
	CSkinImage						m_ImageMoneyLeft;					//ʣ��Ƹ�
	CSkinImage						m_ImageTipInfo;						//��ʾ��Ϣ

	//�˿˿ؼ�
public:
	CHeapCard						m_HeapCard[4];						//�����˿�
	CUserCard						m_UserCard[3];						//�û��˿�
	CTableCard						m_TableCard[4];						//�����˿�
	CWeaveCard						m_WeaveCard[4][4];					//����˿�
	CDiscardCard					m_DiscardCard[4];					//�����˿�
	CCardControl					m_HandCardControl;					//�����˿�

	//�ؼ�����
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btConsignEnter;					//�йܰ�ť
	CSkinButton						m_btConsignCancel;					//ȡ���й�
	CControlWnd						m_ControlWnd;						//���ƴ���
	CGameScoreWnd					m_GameScoreWnd;						//��������

	//��������
public:
	//���캯��
	CGameClientView(IDataCtrl* pDataCtrl);
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
	//ׯ���û�
	void SetBankerUser(WORD wBankerUser);
	//״̬��־
	void SetStatusFlag(bool bOutCard, bool bWaitOther);
	//������Ϣ
	void SetOutCardInfo(WORD wViewChairID, BYTE cbCardData);
	//������Ϣ
	void SetUserAction(WORD wViewChairID, BYTE bUserAction);
	//�й���Ϣ
	void SetConsign(bool bEnter);

	//��Ϣӳ��
protected:
	//��ʼ��ť
	afx_msg void OnStart();
	//�йܰ�ť
	afx_msg void OnConsignEnter();
	//ȡ���й�
	afx_msg void OnConsignCancel();
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()

private:
	//��ʣ��Ƹ�
	void	_DrawScoreLeft(CDC * pDC, int nWidth, int nHeight);
	//����ʾ��Ϣ
	void    _DrawTipInfo(CDC * pDC, int nWidth, int nHeight);
	//��ѡװ����
	CRect   _DrawTitleInRect(CDC *pDC, CString szString, LPRECT lpRect, long lMode = 1, long lHori = 1, long lVert = 1);
};

//////////////////////////////////////////////////////////////////////////
