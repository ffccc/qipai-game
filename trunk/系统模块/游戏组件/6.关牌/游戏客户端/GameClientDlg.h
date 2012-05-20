#pragma once

#include "Stdafx.h"
#include "..\��Ϸ������\GameLogic.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameDlg
{
	//��Ϸ����
protected:
	BYTE							m_cbBombCount;						//ը����Ŀ
	WORD							m_wBankerUser;						//ׯ���û�
	BYTE							m_bCardCount[2];					//�˿���Ŀ
	BYTE							m_bHandCardCount;					//�˿���Ŀ
	BYTE							m_bHandCardData[MAX_COUNT];			//�����˿�
	bool							m_bAutoStatus[2];					//�й�״̬
	BYTE							m_bOverTimes;						//��ʱ����

	//���Ʊ���
protected:
	BYTE							m_bTurnOutType;						//��������
	BYTE							m_bTurnCardCount;					//������Ŀ
	BYTE							m_bTurnCardData[MAX_COUNT];			//�����б�

	//���ñ���
protected:
	DWORD							m_dwCardHSpace;						//�˿�����
	bool							m_bStustee;							//�йܱ�־

	//��������
protected:
	WORD							m_wMostUser;						//������
	WORD							m_wTimeOutCount;					//��ʱ����

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

	//��Ϣ����
protected:
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubOutCard(const void * pBuffer, WORD wDataSize);
	//��������
	bool OnSubPassCard(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);

	bool OnSubUserAutomatism(const void * pBuffer, WORD wDataSize);

	//��������
protected:
	//�����ж�
	bool VerdictOutCard();
	//�����ж�
	bool VerdictPassCard();
	//�Զ�����
	bool AutomatismOutCard();

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
	//������ʾ
	LRESULT OnOutPrompt(WPARAM wParam, LPARAM lParam);
	//�Ҽ��˿�
	LRESULT OnLeftHitCard(WPARAM wParam, LPARAM lParam);
	//����˿�
	LRESULT OnRightHitCard(WPARAM wParam, LPARAM lParam);
	//�Ϲܿ���
	LRESULT OnStusteeControl(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
