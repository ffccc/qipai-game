#pragma once

#include "Stdafx.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ
#define IDM_TIMEOUT					(WM_USER+101)						//��ʱ��Ϣ
#define IDM_DICE_CAST				(WM_USER+102)						//Ͷ��������Ϣ
#define IDM_CHESS_CHOOSE			(WM_USER+103)						//ѡ��������Ϣ
#define IDM_AUTOMATISM				(WM_USER+104)						//ѡ��������Ϣ

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameDlg
{
	//�ؼ�����
protected:
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ
	
	//��������
public:
	//���캯��
	CGameClientDlg();
	//��������
	virtual ~CGameClientDlg();

	//����̳�
private:
	//��ʼ����
	virtual bool InitGameFrame();
	//���ÿ��
	virtual void ResetGameFrame();
	//��Ϸ����
	virtual void OnGameOptionSet();
	//ʱ����Ϣ
	virtual bool OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID);
	//�Թ�״̬
	virtual void OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize);
	//������Ϣ
	virtual bool OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	virtual bool OnGameSceneMessage(BYTE cbGameStatus, bool bLookonOther, const void * pBuffer, WORD wDataSize);

	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	LRESULT OnDiceCast(WPARAM wParam, LPARAM lParam);
	LRESULT OnChessChoose(WPARAM wParam, LPARAM lParam);
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	bool OnSubDiceCast(const void * pBuffer, WORD wDataSize);
	bool OnSubChessChoose(const void * pBuffer, WORD wDataSize);
	bool OnSubUserLeft(const void * pBuffer, WORD wDataSize);
	bool OnSubUserGiveUp(const void * pBuffer, WORD wDataSize);
	bool OnSubUserEnter(const void * pBuffer, WORD wDataSize);
	bool OnSubUserReady(const void * pBuffer, WORD wDataSize);
	bool OnSubUserAutomatism(const void * pBuffer, WORD wDataSize);
	bool OnSubUserTimeOut(const void * pBuffer, WORD wDataSize);
	LRESULT OnUserTimeOut(WPARAM wParam, LPARAM lParam);
	LRESULT OnUserStart(WPARAM wParam, LPARAM lParam);
	LRESULT OnUserAutomatism(WPARAM wParam, LPARAM lParam);
	//����ѡ��ť����
	LRESULT OnPlayMusic(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
