#pragma once

#include "Stdafx.h"
#include "GameClientView.h"
#include "..\��Ϣ����\GameLogic.h"
//#include "DxSound.h"
//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameDlg, public CGameApp
{
	//��ע��Ϣ
public:
	INT64							m_lMyMaxStake;						//�����ע

	//�˿���Ϣ
protected:
	BYTE							m_cbCardCount;					//�˿���Ŀ
    //BYTE							m_cbTableCardArray[2][3];			//�����˿�

	//��ʷ��Ϣ
protected:
	WORD							m_wMyWinCount;						//Ӯ�ľ���
	WORD							m_wMyLossCount;						//��ľ���
	INT64							m_lMyWinAmountSum;					//��Ϸ�ɼ�

	//��ׯ���
public:
	LONG							m_lApplyBankerCondition;			//��������
	WORD							m_wCurrentBanker;					//��ǰׯ��
	WORD							m_wApplyCountMax;					//������ׯ���������
	BYTE							m_cbRakeOffPlayer;					//ׯ��˰�հٷֱ�
	BYTE							m_cbRakeOffBanker;					//�м�˰�հٷֱ�


	//��Դ���
protected:
	CGameSound						m_GameSound;						//������Ϸ����
	string							m_strSoundPath;						//�����ļ�·��
	//�ؼ�����
public:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ

	BOOL                            m_sound;                            //��������

	//��������
public:
	//���캯��
	CGameClientDlg();
	//��������
	virtual ~CGameClientDlg();

	//����̳�
private:
	//��ʼ����
	virtual  bool OnInit();

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
	//ʱ��״̬
	bool OnSubTimerType(const void * pBuffer, WORD wDataSize);
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�û���ע
	bool OnSubPlaceJetton(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//������ׯ
	bool OnSubApplyBanker(const void * pBuffer, WORD wDataSize);
	//�л�ׯ��
	bool OnSubChangeBanker(const void * pBuffer, WORD wDataSize);
	//��������ׯ���б����
	bool OnSubChangeApplyScore(const void * pBuffer, WORD wDataSize);
    bool OnSupperuser( const void * pBuffer, WORD wDataSize );

	bool OnGAME( const void * pBuffer, WORD wDataSize );
    bool OnPLU( const void * pBuffer, WORD wDataSize );
	//���ܺ���
protected:
	//���³��밴ť
	void UpdateJettonButtons();
	//������������ׯ��ť
	void UpdateBankerButtons();
	//�ɷ��˿�
	bool DispatchUserCard(BYTE cbPlayerCount);
	//void PlayGameSound(string strSoundName);
	//void StopGameSound(string strSoundName);
	void ResetGameData();

	//��Ϣӳ��
protected:
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//��ע��Ϣ
	LRESULT OnPlaceJetton(WPARAM wParam, LPARAM lParam);

	//������ׯ
	LRESULT OnApplyBanker(WPARAM wParam, LPARAM lParam);

	//������ׯ
	LRESULT OnFang(WPARAM wParam, LPARAM lParam);

	LRESULT OnInsertSystemString(WPARAM wParam, LPARAM lParam);

	LRESULT OnDealAnimationFinish(WPARAM wParam, LPARAM lParam);

	LRESULT OnPlaySound(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
