#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"
#include "HistoryRecord.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameDlg
{
	//��Ϸ����
protected:
	WORD							m_wLandUser;						//�ڿ��û�
	WORD							m_wBombTime;						//ը������
	BYTE							m_bCardCount[3];					//�˿���Ŀ
	BYTE							m_bHandCardCount;					//�˿���Ŀ
	BYTE							m_bHandCardData[20];				//�����˿�
	bool							m_bAutoPlay ;						//�йܱ���

	//���Ʊ���
protected:
	BYTE							m_bTurnOutType;						//��������
	BYTE							m_bTurnCardCount;					//������Ŀ
	BYTE							m_bTurnCardData[20];				//�����б�

	//���ñ���
protected:
	bool							m_bDeasilOrder;						//����˳��
	DWORD							m_dwCardHSpace;						//�˿�����
	bool							m_bAllowLookon;						//�����Թ�

	//��������
protected:
	WORD							m_wMostUser;						//������
	WORD							m_wCurrentUser;						//��ǰ�û�
	WORD							m_wTimeOutCount;					//��ʱ����
	BYTE							m_cbSortType;						//��������

	//��������
protected:
	BYTE							m_cbRemnantCardCount;				//ʣ����Ŀ
	BYTE							m_cbDispatchCardCount;				//�ɷ���Ŀ
	BYTE							m_cbDispatchCardData[20];			//�ɷ��˿�

	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ
	CHistoryScore					m_HistoryScore;						//��ʷ����

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

	//�û��¼�
public:
	//�û�����
	virtual void __cdecl OnEventUserEnter(tagUserData * pUserData, WORD wChairID, bool bLookonUser);
	//�û��뿪
	virtual void __cdecl OnEventUserLeave(tagUserData * pUserData, WORD wChairID, bool bLookonUser);
	//�û�����
	virtual void __cdecl OnEventUserScore(tagUserData * pUserData, WORD wChairID, bool bLookonUser);
	//�û�״̬
	virtual void __cdecl OnEventUserStatus(tagUserData * pUserData, WORD wChairID, bool bLookonUser);

	//��Ϣ����
protected:
	//�����˿�
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//�û��з�
	bool OnSubLandScore(const void * pBuffer, WORD wDataSize);
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubOutCard(const void * pBuffer, WORD wDataSize);
	//��������
	bool OnSubPassCard(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);

	//��������
protected:
	//�����ж�
	bool VerdictOutCard();
	//�Զ�����
	bool AutomatismOutCard();
	//ֹͣ����
	bool DoncludeDispatchCard();
	//�ɷ��˿�
	bool DispatchUserCard(BYTE cbCardData[], BYTE cbCardCount);

	//��Ϣӳ��
protected:
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//��ʼ��Ϣ
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnOutCard(WPARAM wParam, LPARAM lParam);
	//��������
	LRESULT OnPassCard(WPARAM wParam, LPARAM lParam);
	//�з���Ϣ
	LRESULT OnLandScore(WPARAM wParam, LPARAM lParam);
	//������ʾ
	LRESULT OnAutoOutCard(WPARAM wParam, LPARAM lParam);
	//�Ҽ��˿�
	LRESULT OnLeftHitCard(WPARAM wParam, LPARAM lParam);
	//����˿�
	LRESULT OnRightHitCard(WPARAM wParam, LPARAM lParam);
	//�й���Ϣ
	LRESULT OnAutoPlay(WPARAM wParam, LPARAM lParam);
	//�����˿�
	LRESULT OnMessageSortCard(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
