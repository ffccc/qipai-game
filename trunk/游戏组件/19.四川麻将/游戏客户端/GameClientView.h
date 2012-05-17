#pragma once

#include "Stdafx.h"
#include "ControlWnd.h"
#include "CardControl.h"
#include "ScoreControl.h"

//////////////////////////////////////////////////////////////////////////
//��Ϣ����

#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ
#define IDM_OUT_CARD				(WM_USER+101)						//������Ϣ
#define IDM_TRUSTEE_CONTROL			(WM_USER+102)						//�йܿ���
#define IDI_DISC_EFFECT					102								//����Ч��

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//��־����
protected:
	bool							m_bOutCard;							//���Ʊ�־
	bool							m_bWaitOther;						//�ȴ���־
	bool							m_bHuangZhuang;						//��ׯ��־
	bool							m_bListenStatus[4];					//���Ʊ�־
	bool							m_bTrustee[GAME_PLAYER];			//�Ƿ��й�


	//��Ϸ����
protected:
	LONG							m_lCellScore;						//��������
	WORD							m_wBankerUser;						//ׯ���û�
	WORD							m_wCurrentUser;						//��ǰ�û�

	//��������
protected:
	bool							m_bBombEffect;						//����Ч��
	BYTE							m_cbBombFrameIndex;					//֡������

	//����Ч��
	WORD							m_wDiscUser;						//�����û�
	BYTE							m_cbDiscFrameIndex;					//֡������	


	//�û�״̬
protected:
	BYTE							m_cbCardData;						//�����˿�
	WORD							m_wOutCardUser;						//�����û�
	BYTE							m_cbUserAction[4];					//�û�����

	//λ�ñ���
protected:
	CPoint							m_UserFlagPos[4];					//��־λ��
	CPoint							m_UserListenPos[4];					//��־λ��
	CPoint							m_PointTrustee[GAME_PLAYER];		//�й�λ��

	//λͼ����
protected:
	CSkinImage						m_ImageBack;						//����ͼ��
	CSkinImage						m_ImageCenter;						//LOGOͼ
	CSkinImage						m_ImageWait;						//�ȴ���ʾ
	CSkinImage						m_ImageOutCard;						//������ʾ
	CSkinImage						m_ImageUserFlag;					//�û���־
	CSkinImage						m_ImageUserAction;					//�û�����
	CSkinImage						m_ImageActionBack;					//��������
	CSkinImage						m_ImageHuangZhuang;					//��ׯ��־
	CSkinImage						m_ImageListenStatusH;				//���Ʊ�־
	CSkinImage						m_ImageListenStatusV;				//���Ʊ�־
	CPngImage						m_ImageTrustee;						//�йܱ�־	

	CPngImage						m_ImageActionAni;					//���ƶ�����Դ
	CPngImage						m_ImageDisc;						//����Ч��
	CPngImage						m_ImageArrow;						//��ʱ����ͷ	

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
	CSkinButton						m_btStusteeControl;					//�Ϲܿ���
	CControlWnd						m_ControlWnd;						//���ƴ���
	CScoreControl					m_ScoreControl;						//���ֿؼ�
	
	//��Ƶ���
private:
	CVideoServiceControl 			m_DlgVedioService[4];				//��Ƶ����

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
	//״̬��־
	void SetStatusFlag(bool bOutCard, bool bWaitOther);
	//������Ϣ
	void SetOutCardInfo(WORD wViewChairID, BYTE cbCardData);
	//������Ϣ
	void SetUserAction(WORD wViewChairID, BYTE bUserAction);
	//���Ʊ�־
	void SetUserListenStatus(WORD wViewChairID, bool bListenStatus);
	//���ö���
	bool SetBombEffect(bool bBombEffect);
	//�����û�
	void SetDiscUser(WORD wDiscUser);
	//��ʱ���
	void SetCurrentUser(WORD wCurrentUser);
	//�����й�
	void SetTrustee(WORD wTrusteeUser,bool bTrustee);


	//��������
protected:
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);

	//��Ϣӳ��
protected:
	//��ʼ��ť
	afx_msg void OnStart();
	//�Ϲܿ���
	afx_msg void OnStusteeControl();
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTimer(UINT nIDEvent);
};

//////////////////////////////////////////////////////////////////////////
