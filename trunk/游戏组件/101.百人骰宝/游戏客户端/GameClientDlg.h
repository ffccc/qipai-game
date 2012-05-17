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
	__int64							m_lMeMaxScore;						// �����ע
	__int64							m_lAreaLimitScore;					// ��������	
	__int64							m_lApplyBankerCondition;			// ��������

	const tagServerAttribute		*pServerAttribute ;
	//������ע
protected:
	__int64							m_lMeScore[COUNT_AZIMUTH];          //ÿ����λ��ע
	__int64                         m_lAllScore[COUNT_AZIMUTH];			//ÿ����λ��ע
	//ׯ����Ϣ
protected:
	__int64							m_lBankerScore;						//ׯ�һ���
	WORD							m_wCurrentBanker;					//��ǰׯ��
	bool							m_bEnableSysBanker;					//ϵͳ��ׯ

	bool							m_bJoinInFree;

	//״̬����
protected:
	bool							m_bMeApplyBanker;					// �����ʶ
	bool                            m_bShowBtnSicbo;                     // ѡ��ҡ�����Ͱ�ť�Ƿ���ʾ

	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ

	//������Դ
protected:
	CDirectSound					m_DTSDBackground;					//��������
	CDirectSound					m_DTSDCheer[3];						//��������

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
	//�Լ����õ��Թ�״̬�����ı��ӽϳ�
	virtual void OnLookonViewChange(bool bLookon){};

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
	// ѡ��ҡ���ӷ�ʽ
	bool OnSubChangeSicboType(const void * pBuffer, WORD wDataSize);

	//��ֵ����
protected:
	//����ׯ��
	void SetBankerInfo(WORD wBanker,__int64 lScore);
	//������ע
	void SetMePlaceJetton(BYTE cbViewIndex, __int64 lJettonCount);

	//���ܺ���
protected:
	//���¿���
	void UpdateButtonContron(bool bEnablePlaceJetton);	

	//��Ϣӳ��
protected:
	//��ע��Ϣ
	LRESULT OnPlaceJetton(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnApplyBanker(WPARAM wParam, LPARAM lParam);	
	// ѡ��ҡ���ӷ�ʽ
	LRESULT OnChangeSicboType(WPARAM wParam, LPARAM lParam);
	// ���в���
	LRESULT OnOpenBank(WPARAM wParam, LPARAM lParam);
	// ʤ������
	LRESULT OnUseWinner(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	bool IsFreeze(void);
};

//////////////////////////////////////////////////////////////////////////
