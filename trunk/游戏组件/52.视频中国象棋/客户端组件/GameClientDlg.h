#ifndef GAME_CLIENT_ENGINE_HEAD_FILE
#define GAME_CLIENT_ENGINE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CGameClientDlg : public CGameFrameDlg
{
	//�������
protected:
	WORD							m_wRuleRoundID;						//�����ʶ
	tagGameRuleInfo					m_GameRuleInfo;						//��Ϸ����

	//��Ϸ����
protected:
	WORD							m_wBankerUser;						//ׯ�����
	WORD							m_wCurrentUser;						//��ǰ�û�
	BYTE							m_cbChessColor;						//������ɫ

	//ѡ����Ϣ
protected:
	BYTE							m_cbXSourcePos;						//ѡ����Ϣ
	BYTE							m_cbYSourcePos;						//ѡ����Ϣ

	//��������
protected:
	bool							m_bDrawTimeMode;					//��ʱģʽ
	bool							m_bRequestAgree;					//����ͬ��
	bool							m_bRequestCustomize;				//������
	TCHAR							m_szUserAccounts[2][LEN_ACCOUNTS];	//����ʺ�

	//״̬����
protected:
	WORD							m_wViewStepCount;					//���ײ���
	tagStatusInfo					m_UserStatusInfo[2];				//�û�״̬

	//��������
protected:
	tagChessItem					m_ChessItemView[2][COUNT_CHESS];	//������Ϣ
	tagChessItem *					m_ChessBoradView[COUNT_X][COUNT_Y];	//������Ϣ

	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ

	//��������
public:
	//���캯��
	CGameClientDlg();
	//��������
	virtual ~CGameClientDlg();

	//���ƽӿ�
public:
	//��ʼ����
	virtual bool InitGameFrame();
	//��������
	virtual void ResetGameFrame();
	//��Ϸ����
	virtual void OnGameOptionSet();
	//�Թ�״̬
	virtual void OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize);
	////���ò���
	//virtual bool __cdecl OnGameOptionSet(IUnknownEx * pIUnknownEx[], WORD wItemCount);

	//�¼��ӿ�
public:
	//ʱ����Ϣ
	virtual bool OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID);
	//������Ϣ
	virtual bool  OnGameMessage(WORD wSubCmdID,const VOID * pData, WORD wDataSize);
	//��Ϸ����
	virtual bool  OnGameSceneMessage(BYTE cbGameStatus, bool bLookonOther,const VOID * pData, WORD wDataSize);

//	//�û��¼�
//public:
//	//�û�����
//	virtual VOID __cdecl OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser);
//	//�û��뿪
//	virtual VOID __cdecl OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser);

	//�ڲ�����
protected:
	//���°�ť
	bool UpdateManualControl(WORD wViewStepCount);
	//ִ������
	bool PerformMoveChess(BYTE cbXSourcePos, BYTE cbYSourcePos, BYTE cbXTargetPos, BYTE cbYTargetPos);

	//��Ϣ����
protected:
	//��Ϸ��ʼ
	bool OnSubGameStart(const VOID * pData, WORD wDataSize);
	//�ƶ�����
	bool OnSubMoveChess(const VOID * pData, WORD wDataSize);
	//����ʧ��
	bool OnSubRegretFaile(const VOID * pData, WORD wDataSize);
	//������
	bool OnSubRegretResult(const VOID * pData, WORD wDataSize);
	//��������
	bool OnSubRegretRequest(const VOID * pData, WORD wDataSize);
	//������Ӧ
	bool OnSubPeaceRespond(const VOID * pData, WORD wDataSize);
	//��������
	bool OnSubPeaceRequest(const VOID * pData, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const VOID * pData, WORD wDataSize);
	//��������
	bool OnSubRequestRule(const VOID * pData, WORD wDataSize);
	//����ͬ��
	bool OnSubRequestAgree(const VOID * pData, WORD wDataSize);
	//��Ϸ����
	bool OnSubChessManual(const VOID * pData, WORD wDataSize);

	//��Ϣ����
protected:
	//��ʼ��Ϣ
	LRESULT OnMessageStart(WPARAM wParam, LPARAM lParam);
	//�����Ϣ
	LRESULT OnMessagePeace(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnMessageRegret(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnMessageGiveUp(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnMessageManual(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnMessagePreserve(WPARAM wParam, LPARAM lParam);
	//�������
	LRESULT OnHitChessBorad(WPARAM wParam, LPARAM lParam);
	//ͬ�����
	LRESULT OnGameRuleAgree(WPARAM wParam, LPARAM lParam);
	//�ܾ�����
	LRESULT OnGameRuleReject(WPARAM wParam, LPARAM lParam);
	//���׶�λ
	LRESULT OnOrientationManual(WPARAM wParam, LPARAM lParam);
	//�����Ӧ
	LRESULT OnUserRequestRespond(WPARAM wParam, LPARAM lParam);

	//ϵͳ��Ϣ
protected:
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
