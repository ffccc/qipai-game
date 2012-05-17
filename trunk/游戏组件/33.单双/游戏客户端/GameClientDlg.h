#pragma once

#include "Stdafx.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameDlg
{
	//������Ϣ
protected:
	LONG							m_lMeMaxScore;						//�����ע
	LONG							m_lAreaLimitScore;					//��������

	//��ע��Ϣ
protected:
	LONG							m_lMeOddScore;						//��ƽ��ע
	LONG							m_lMeEvenScore;						//��ׯ��ע

	//״̬����
protected:
	bool							m_bPlaying;							//��Ϸ״̬
	int								m_nShowResultTime;					//��ʾʱ��

	//ɫ����Ϣ
protected:
	BYTE							m_cbDice[2];						//ɫ����Ϣ

	//��ʷ��Ϣ
protected:
	WORD							m_wDrawCount;						//��Ϸ����
	LONG							m_lMeResultCount;					//��Ϸ�ɼ�

	//ׯ����Ϣ
protected:
	LONG							m_lApplyBankerCondition;			//��������
	LONG							m_lBankerScore;						//ׯ�һ���
	WORD							m_wCurrentBanker;					//��ǰׯ��
	bool							m_bMeApplyBanker;					//�����ʶ

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

	//��Ϣ����
protected:
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�û���ע
	bool OnSubPlaceJetton(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//������ׯ
	bool OnUserApplyBanker(const void * pBuffer, WORD wDataSize);
	//�л�ׯ��
	bool OnChangeBanker(const void * pBuffer, WORD wDataSize);
	//���»���
	bool OnChangeUserScore(const void * pBuffer, WORD wDataSize);
	//��Ϸ��¼
	bool OnSubGameRecord(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameScore(const void * pBuffer, WORD wDataSize);
	//��ע����
	bool OnSubEndPlaceJetton(const void * pBuffer, WORD wDataSize);

	//���ܺ���
protected:
	//���¿���
	void UpdateButtonContron();
	//�ƶ�Ӯ��
	void DeduceWinner(BYTE &cbWinner);

	//��Ϣӳ��
protected:
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//��ע��Ϣ
	LRESULT OnPlaceJetton(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnApplyBanker(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnUpdateControl(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
