// GameClientDlg.h : ͷ�ļ�
//

#pragma once
#ifndef GAME_DLG_HEAD_FILE
#define GAME_DLG_HEAD_FILE

#endif
#include "StdAfx.h"
#include "GameClientView.h"
#include "Ball.h"

//��Ϣ����
#define IDM_BEGIN			WM_USER+120						//��ʼ��ť��Ϣ
#define IDM_HIT_BALL		WM_USER+121						//������Ϣ
#define IDM_PLACE_BALL		WM_USER+122						//������Ϣ
#define IDM_HITGIVEUP			WM_USER+123						//������Ϣ
#define IDM_HITREPLACE			WM_USER+124						//������Ϣ
#define IDM_HITCONCESSION		WM_USER+125						//������Ϣ



//��Ϸ��ʱ�� ID
#define BALL_TIMER						100					//̨��ʱ��
#define ID_BEGIN_TIME					200					//��ʼ��ʱ��
#define ID_HIT_BALL						201					//����ʱ��
#define PLACE_WHITE_BALL				202					//�Ű���ʱ��

//��ʱ��ʱ��	
#define BEGIN_COUNT						40					//��ʼʱ��

// CGameClientDlg �Ի���
class CGameClientDlg : public CGameFrameDlg
{
// ����
public:

	//��������
	BOOL						m_bCanHitBall;				//�Ƿ����
	HANDLE						m_hTimerThreadHandle;		//�߳̾��

	//�ؼ�
public:
	CAmericaDesk				m_Desk;
	CGameClientView				m_GameClientView;					//��Ϸ��ͼ
	small						m_NextHitBall;
	BOOL                        bAgreeConession;
	CGameClientDlg();	// ��׼���캯��
	virtual ~CGameClientDlg();
private:
	void	InitData();
	virtual bool InitGameFrame();
	virtual void ResetGameFrame();
	virtual void OnGameOptionSet();

	virtual bool OnTimerMessage(WORD wChairID, 
								UINT nElapse, 
								UINT nTimerID);
											//ʱ����Ϣ
	virtual void OnLookonChanged(bool bLookonUser, 
								const void * pBuffer, 
								WORD wDataSize);
											//�Թ�״̬
	virtual bool OnGameMessage(WORD wSubCmdID, 
								const void * pBuffer, 
								WORD wDataSize);
											//������Ϣ
	virtual bool OnGameSceneMessage(BYTE cbGameStatus, 
									bool bLookonOther, 
									const void * pBuffer, 
									WORD wDataSize);

	//��Ϸ����
protected:
	DECLARE_MESSAGE_MAP()

public:
	//��Ϸ�˳�
	void GameQuit();
	//��Ϸ��ʼ
	void GameStart();
	//��ʼ������Ϣ
	LRESULT	OnHitBall(WPARAM wparam, LPARAM lparam);
	//������Ϣ
	LRESULT	OnPlaceBall(WPARAM wparam, LPARAM lparam);
	//������Ϣ
	LRESULT	OnHitGiveUp(WPARAM wparam, LPARAM lparam);
	//��ʼ������Ϣ
	LRESULT	OnHitReplace(WPARAM wparam, LPARAM lparam);
	//������Ϣ
	LRESULT	OnHitConession(WPARAM wparam, LPARAM lparam);
	LRESULT OnAgreePeace(WPARAM wParam, LPARAM lParam);	
	//��ʼ��ť����
	LRESULT	OnHitBegin(WPARAM wparam, LPARAM lparam);
	afx_msg void OnTimer(UINT nIDEvent);

	//��Ϣ����
protected:
	//��ʱ���߳�
	static unsigned __stdcall TimerThread(LPVOID pThreadData);                                         //�����������
};
