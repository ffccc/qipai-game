#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

typedef CArrayTemplate<tagApplyUser,tagApplyUser&> CTagApplyUser;

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameDlg
{
	//��Ԫ����
	friend class CGameClientView;
	//������Ϣ
protected:
	LONG							m_lMeMaxScore;						//�����ע
	LONG							m_lAreaLimitScore;					//��������
	LONG							m_lApplyBankerCondition;			//��������
	BYTE							m_cbMoveIndex;						//�ƶ�����
	BYTE							m_cbMoveEndIndex;					//�ƶ���������
	BYTE							m_cbResultIndex;					//�������
	BYTE							m_cbEndIndex;						//ֹͣ����
	LONG							m_lMeCanUseScore;					//���ý��
	bool							m_bFlashFlags;						//��������
	//ׯ����Ϣ
protected:
	LONG							m_lBankerScore;						//ׯ�һ���
	WORD							m_wCurrentBanker;					//��ǰׯ��
	bool							m_bEnableSysBanker;					//ϵͳ��ׯ
	WORD							m_wBankerTime;						//��ׯ����
	LONG							m_lTmpBankerWinScore;				//ׯ�ҳɼ�	
	//������Ϣ
protected:
	LONG							m_lMeScore;							//���˻���
	//������ע��Ϣ
protected:
	LONG							m_lMeBigSharkScore;					//�����
	LONG							m_lMeFeiQinScore;					//����
	LONG							m_lMeZouShouScore;					//����
	LONG							m_lMeYanZiScore;					//�Լ�����������
	LONG							m_lMeTuZiScore;						//����
	LONG							m_lMeGeZiScore;						//����
	LONG							m_lMeXiongMaoScore;					//��è
	LONG							m_lMeKongQueScore;					//��ȸ
	LONG							m_lMeHouZiScore;					//����
	LONG							m_lMeLaoYingScore;					//��ӥ
	LONG							m_lMeShiZiScore;					//ʨ��

	//ȫ����ע
protected:
	LONG							m_lALLBigSharkScore;				//�Լ�����������
	LONG							m_lALLFeiQinScore;					//�Լ�����������
	LONG							m_lALLZouShouScore;					//�Լ�����������
	LONG							m_lALLYanZiScore;					//�Լ�����������
	LONG							m_lALLTuZiScore;					//����
	LONG							m_lALLGeZiScore;					//����
	LONG							m_lALLXiongMaoScore;				//��è
	LONG							m_lALLKongQueScore;					//��ȸ
	LONG							m_lALLHouZiScore;					//����
	LONG							m_lALLLaoYingScore;					//��ӥ
	LONG							m_lALLShiZiScore;					//ʨ��
	//��ʷ��Ϣ
protected:
	LONG							m_lMeStatisticScore;				//��Ϸ�ɼ�
	BYTE							m_cbScoreHistroy[MAX_SCORE_HISTORY];//��ʷ�ɼ�
	LONG							m_lBankerEndScore;					//ׯ�ҳɼ�
	LONG							m_lMeEndScore;						//�����ɼ�
	LONG							m_lRevenue;							//˰��

	//״̬����
protected:
	bool							m_bMeApplyBanker;					//�����ʶ
	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ
	//������ׯ��Ϣ
	CTagApplyUser					m_ApplyUser;						//������ׯ��Ϣ
	BYTE							m_cbApplyCount;						//��������
	BYTE							m_cbApplyIndex;						//�鿴����
	CDirectSound					m_DirectSound[8];					//��Ϸ����
	//��������
public:
	//���캯��
	CGameClientDlg();
	//��������
	virtual ~CGameClientDlg();

	//����̳�
private:
	virtual bool SetDlgTitle(){return true;}
	//��ʼ����
	virtual bool InitGameFrame();
	//��ʼ����
	virtual BOOL OnInitDialog();
	//���ÿ��
	virtual void ResetGameFrame();
	//��Ϸ����
	virtual void OnGameOptionSet();
	//ʱ����Ϣ
	virtual bool OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID);
	//�Թ�״̬
	virtual void OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize){return;}
	//������Ϣ
	virtual bool OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	virtual bool OnGameSceneMessage(BYTE cbGameStatus, bool bLookonOther, const void * pBuffer, WORD wDataSize);
	//���ܺ���
public:
	//����ׯ��
	void SetBankerInfo(WORD wBanker,LONG lScore);
	void UpdateButtonContron();	
	//��ʼ��ת
	void StartGameMove();
	//��Ϣ����
protected:
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�û���ע
	bool OnSubPlaceJetton(const void * pBuffer, WORD wDataSize);
	//��עʧ��
	bool OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameFree(const void * pBuffer, WORD wDataSize);
	//������ע
	void SetMePlaceJetton(BYTE cbViewIndex, LONG lJettonCount);
	//������ׯ
	bool OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize);
	//ȡ����ׯ
	bool OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize);
	//�л�ׯ��
	bool OnSubChangeBanker(const void * pBuffer, WORD wDataSize);
	//�л�ׯ��
	bool OnSubChangeSysBanker(const void * pBuffer, WORD wDataSize);
	//��ʷ��¼
	bool OnSubSocreHistory(const void * pBuffer, WORD wDataSize);
	//����ɼ�
	CString DrawScoreString(LONG lNumber);

	//��Ϣӳ��
protected:
	//������Ϣ
	LRESULT OnScoreLeft(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnScoreRight(WPARAM wParam, LPARAM lParam);
	////������Ϣ
	//LRESULT OnBankerTop(WPARAM wParam, LPARAM lParam);
	////������Ϣ
	//LRESULT OnBankerBottom(WPARAM wParam, LPARAM lParam);
protected:
	//��ע��Ϣ
	LRESULT OnPlaceJetton(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnApplyBanker(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnBank(WPARAM wParam, LPARAM lParam);

	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
public:
};

//////////////////////////////////////////////////////////////////////////
