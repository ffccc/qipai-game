#pragma once

#include "Stdafx.h"
#include "GameRule.h"
#include "ChessBorad.h"
#include "ChessManual.h"
#include "GameScoreWnd.h"
#include "ChessCountInfo.h"
#include "DlgUserRequest.h"

//////////////////////////////////////////////////////////////////////////
//��Ϣ����

#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ
#define IDM_COUNT					(WM_USER+101)						//��Ŀ��Ϣ
#define IDM_PASS					(WM_USER+102)						//ͨ����Ϣ
#define IDM_REGRET					(WM_USER+103)						//������Ϣ
#define IDM_PEACE					(WM_USER+104)						//�����Ϣ
#define IDM_GIVEUP					(WM_USER+105)						//������Ϣ
#define IDM_PRESERVE				(WM_USER+106)						//������Ϣ
#define IDM_STUDY					(WM_USER+107)						//�о���Ϣ
#define IDM_REGRET_ANSWER			(WM_USER+108)						//����Ӧ��
#define IDM_PEACE_ANSWER			(WM_USER+109)						//���Ӧ��

//////////////////////////////////////////////////////////////////////////

//״̬��Ϣ
struct tagStatusInfo
{
	WORD							wStepCount;							//�岽��Ŀ
	WORD							wUseTimeCount;						//��ʱʱ��
	WORD							wLimitTimeCount;					//��ʱʱ��
	WORD							wTakeChessCount;					//������Ŀ
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//��Ϸ����
protected:
	WORD							m_wBlackUser;						//�����û�
	tagStatusInfo					m_StatusInfo[2];					//��Ϸ��Ϣ

	//λ����Ϣ
protected:
	CSize							m_UserInfoSize;						//��Ϣ��С

	//λͼ����
protected:
	CSkinImage						m_ImageBack;						//����ͼ��
	CSkinImage						m_ImageUserBlack;					//�û���Ϣ
	CSkinImage						m_ImageUserWhite;					//�û���Ϣ

	//�ؼ�����
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btCount;							//��Ŀ��ť
	CSkinButton						m_btPass;							//ͨ����ť
	CSkinButton						m_btRegret;							//���尴ť
	CSkinButton						m_btPeace;							//��Ͱ�ť
	CSkinButton						m_btGiveUp;							//���䰴ť
	CSkinButton						m_btPreserve;						//���水ť
	CSkinButton						m_btStudy;							//�о���ť
	CGameRule						m_GameRule;							//��Ϸ����
	CChessBorad						m_ChessBorad;						//��Ϸ����
	CChessManual					m_ChessManual;						//���״���
	CGameScoreWnd					m_GameScoreWnd;						//�ɼ�����
	CDlgUserRequest					m_PeaceRequest;						//��������
	CDlgUserRequest					m_RegretRequest;					//��������
	CChessCountInfo					m_ChessCountInfo;					//��Ŀ��Ϣ

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
	void ShowPeaceRequest();
	//��������
	void ShowRegretRequest();
	//���ú���
	void SetBlackUser(WORD wBlackUser);
	//������Ϣ
	void SetUserStatusInfo(WORD wViewChairID, tagStatusInfo & StatusInfo);

	//�ڲ����� 
private:
	//����״̬
	void UpdateUserInfoArea();
	//�û�ʱ��
	void DrawUserClock(CDC * pDC, int nXPos, int nYPos, WORD wClockCount);

	//�ؼ���Ϣ
protected:
	//�������
	LRESULT OnHitChessBorad(WPARAM wParam, LPARAM lParam);

	//��Ϣӳ��
protected:
	//��ʼ��ť
	afx_msg void OnStart();
	//��Ŀ��ť
	afx_msg void OnCount();
	//ͨ����ť
	afx_msg void OnPass();
	//���尴ť
	afx_msg void OnRegret();
	//��Ͱ�ť
	afx_msg void OnPeace();
	//���䰴ť
	afx_msg void OnGiveUp();
	//���水ť
	afx_msg void OnPreserve();
	//�о���ť
	afx_msg void OnStudy();
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()

};

//////////////////////////////////////////////////////////////////////////
