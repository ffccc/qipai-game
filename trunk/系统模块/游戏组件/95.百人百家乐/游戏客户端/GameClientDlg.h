#pragma once

#include "Stdafx.h"
#include "GameClientView.h"
#include "DirectSound.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameDlg
{
	//��Ԫ����
	friend class CGameClientView;

	//������Ϣ
protected:
	LONGLONG						m_lMeMaxScore;						//�����ע
	LONGLONG						m_lAreaLimitScore;					//��������
	LONGLONG						m_lApplyBankerCondition;			//��������

	//������ע
protected:
	LONGLONG						m_lMeTieScore;						//��ƽ��ע
	LONGLONG						m_lMeBankerScore;					//��ׯ��ע
	LONGLONG						m_lMePlayerScore;					//������ע
	LONGLONG						m_lMeTieSamePointScore;				//ͬ��ƽע
	LONGLONG						m_lMePlayerKingScore;				//������ע
	LONGLONG						m_lMeBankerKingScore;				//ׯ����ע
	LONGLONG						m_lMePlayerTwoPair;					//������ע
	LONGLONG						m_lMeBankerTwoPair;					//������ע

	//ׯ����Ϣ
protected:
	LONGLONG						m_lBankerScore;						//ׯ�һ���
	WORD							m_wCurrentBanker;					//��ǰׯ��
	bool							m_bEnableSysBanker;					//ϵͳ��ׯ

	//״̬����
protected:
	bool							m_bMeApplyBanker;					//�����ʶ

	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ

	//������Դ
protected:
	CDirectSound					m_DTSDBackground;					//��������
	CDirectSound					m_DTSDCheer[3];						//��������

	//��������
	CArrayTemplate<CMD_S_PlaceJetton,CMD_S_PlaceJetton> m_PlaceJettonArray;

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
	//��Ϸ����
	bool OnSubGameFree(const void * pBuffer, WORD wDataSize);
	//�û���ע
	bool OnSubPlaceJetton(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//������ׯ
	bool OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize);
	//ȡ����ׯ
	bool OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize);
	//�л�ׯ��
	bool OnSubChangeBanker(const void * pBuffer, WORD wDataSize);
	//��Ϸ��¼
	bool OnSubGameRecord(const void * pBuffer, WORD wDataSize);
	//��עʧ��
	bool OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize);
	//������
	bool OnSubReqResult(const void * pBuffer, WORD wDataSize);
	//��ֵ����
protected:
	//����ׯ��
	void SetBankerInfo(WORD wBanker,LONGLONG lScore);
	//������ע
	void SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount);

	//���ܺ���
protected:
	//���¿���
	void UpdateButtonContron();	

	//��Ϣӳ��
protected:
	//��ע��Ϣ
	LRESULT OnPlaceJetton(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnApplyBanker(WPARAM wParam, LPARAM lParam);
	//����Ա����
	LRESULT OnAdminCommand(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT nIDEvent);
};

//////////////////////////////////////////////////////////////////////////
