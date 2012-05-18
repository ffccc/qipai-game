#ifndef GAME_CLIENT_HEAD_FILE
#define GAME_CLIENT_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "ChessBorad.h"
#include "ManualList.h"
#include "ScoreControl.h"
#include "DlgRuleOption.h"
#include "DlgUserRequest.h"
#include "DlgChessManual.h"
#include "DlgHint.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

////����λͼ
//struct tagEncircleImage
//{
//	CSkinImage						ImageTL;							//λͼ����
//	CSkinImage						ImageTM;							//λͼ����
//	CSkinImage						ImageTR;							//λͼ����
//	CSkinImage						ImageML;							//λͼ����
//	CSkinImage						ImageMR;							//λͼ����
//	CSkinImage						ImageBL;							//λͼ����
//	CSkinImage						ImageBM;							//λͼ����
//	CSkinImage						ImageBR;							//λͼ����
//};
//
////������Ϣ
//struct tagEncircleInfo
//{
//	INT								nTBorder;							//�ϱ߿��
//	INT								nBBorder;							//�±߿��
//	INT								nLBorder;							//��߿��
//	INT								nRBorder;							//�ұ߿��
//};

//��ͼλ��
#define MYSELF_VIEW_ID				1									//��ͼλ��

//������Ϣ
#define IDM_START					(WM_USER+200)						//��ʼ��Ϣ
#define IDM_PEACE					(WM_USER+201)						//�����Ϣ
#define IDM_REGRET					(WM_USER+202)						//������Ϣ
#define IDM_GIVEUP					(WM_USER+203)						//������Ϣ
#define IDM_MANUAL					(WM_USER+204)						//������Ϣ
#define IDM_PRESERVE				(WM_USER+205)						//������Ϣ

//������Ϣ
#define IDM_ORIENTATION_MANUAL		(WM_USER+210)						//��λ����

//////////////////////////////////////////////////////////////////////////

//״̬��Ϣ
struct tagStatusInfo
{
	WORD							wDrawTimeCount;						//��ʱʱ��
	WORD							wStepTimeCount;						//��ʱʱ��
	WORD							wSecondTimeCount;					//����ʱ��
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//��Ϸ����
protected:
	INT								m_nFrameWidth;						//�����С
	INT								m_nFrameHeight;						//�����С
	WORD							m_wBankerUser;						//ׯ���û�
	WORD							m_wCurrentUser;						//��ǰ�û�
	tagStatusInfo					m_RuleStatusInfo;					//����״̬
	tagStatusInfo					m_UserStatusInfo[2];				//�û�״̬

	//���ư�ť
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btPeace;							//��Ͱ�ť
	CSkinButton						m_btManual;							//���װ�ť
	CSkinButton						m_btRegret;							//���尴ť
	CSkinButton						m_btGiveUp;							//���䰴ť
	CSkinButton						m_btPreserve;						//���水ť

	//���װ�ť
public:
	CSkinButton						m_btManualHead;						//���װ�ť
	CSkinButton						m_btManualLast;						//���װ�ť
	CSkinButton						m_btManualOpen;						//���װ�ť
	CSkinButton						m_btManualNext;						//���װ�ť
	CSkinButton						m_btManualTail;						//���װ�ť

	//��Ϸ�ؼ�
public:
	CManualList						m_ManualList;						//�����б�
	CChessBorad						m_ChessBorad;						//���̿ؼ�
	CToolTipCtrl					m_ToolTipCtrl;						//��ʾ�ؼ�
	CScoreControl					m_ScoreControl;						//��Ϸ�ɼ�
	CDlgRuleOption					m_DlgRuleOption;					//��������
	CDlgChessManual					m_DlgChessManual;					//������ͼ
	CDlgUserRequest					m_UserRequestPeace;					//��������
	CDlgUserRequest					m_UserRequestRegret;				//��������
	CDlgHint						m_UserHint;							//��ʾ����

	//λ�ñ���
protected:
	CPoint							m_ptBanker[GAME_PLAYER];			//ׯ��λ��

	//�ؼ�λ��
protected:
	CRect							m_rcChessManual;					//����λ��
	CPoint							m_ptControlView;					//������ͼ
	CPoint							m_ptUserInfoView;					//��Ϣ��ͼ

	//��Դ��С
protected:
	INT								m_nManualWidth;						//���׿��
	CSize							m_SizeChessBorad;					//���̴�С
	CSize							m_SizeControlView;					//���ƴ�С
	CSize							m_SizeUserInfoView;					//��Ϣ��С
	tagEncircleInfo					m_EncircleInfoViewBack;				//�����Ϣ

	//��Դ����
protected:
	CSkinImage						m_ImageViewFill;					//������Դ
	CSkinImage						m_ImageClockNumber;					//������Դ
	CSkinImage						m_ImageControlBack;					//���Ʊ���
	CSkinImage						m_ImageUserInfoFlag;				//�û���־
	CSkinImage						m_ImageUserInfoView;				//�û���Ϣ
	CSkinImage						m_ImageChessManualT;				//���ױ���
	CSkinImage						m_ImageChessManualM;				//���ױ���
	CSkinImage						m_ImageChessManualB;				//���ױ���
	tagEncircleImage				m_EncircleImageViewBack;			//�����Ϣ

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

	//���غ���
public:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//���ƽӿ�
private:
	//���ý���
	virtual void ResetGameView();

	//�̳к���
protected:
	//�����ؼ�
	virtual VOID RectifyGameView(INT nWidth, INT nHeight);
	//�滭����
	virtual VOID DrawGameView(CDC * pDC, INT nWidth, INT nHeight);

	//���ܺ���
public:
	//ׯ���û�
	bool SetBankerUser(WORD wBankerUser);
	//��ǰ�û�
	bool SetCurrentUser(WORD wCurrentUser);
	//���ù���
	bool SetGameRuleInfo(const tagGameRuleInfo & GameRuleInfo);
	//�û�״̬
	bool SetUserStatusInfo(WORD wChairID, const tagStatusInfo & UserStatusInfo);

	//���̺���
public:
	//ѡ������
	bool SelectChessItem(BYTE cbXPos, BYTE cbYPos);
	//����·��
	bool SetMoveChessTrace(BYTE cbXPosStart, BYTE cbYPosStart, BYTE cbXPosTerminal, BYTE cbYPosTerminal);

	//��������
protected:
	//�û�ʱ��
	VOID DrawUserClock(CDC * pDC, INT nXPos, INT nYPos, WORD wClockCount);
	//�û��ȼ�
	TCHAR *GetLevelDescribe(LONG lUserScore,TCHAR tcUserInfo[32]);
	
	//��ť��Ϣ
protected:
	//��ʼ��ť
	VOID OnBnClickedStart();
	//��Ͱ�ť
	VOID OnBnClickedPeace();
	//���尴ť
	VOID OnBnClickedRegret();
	//���䰴ť
	VOID OnBnClickedGiveUp();
	//���װ�ť
	VOID OnBnClickedManual();
	//���水ť
	VOID OnBnClickedPreserve();

	//���װ�ť
protected:
	//���װ�ť
	VOID OnBnClickedHead();
	//���װ�ť
	VOID OnBnClickedLast();
	//���װ�ť
	VOID OnBnClickedOpen();
	//���װ�ť
	VOID OnBnClickedNext();
	//���װ�ť
	VOID OnBnClickedTail();

	//��Ϣ����
protected:
	//ѡ��ı�
	VOID OnLbnSelchangeManualList();
	//��������
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonDblClk(UINT nFlags, CPoint Point);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
