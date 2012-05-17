#pragma once

#include "Stdafx.h"
#include "GameClientView.h"
#include "DirectSound.h"
#include "DlgControl.h"
#include "GameBank.h"

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
	LONGLONG						m_lUserJettonScore[AREA_COUNT+1];	//������ע

	//ׯ����Ϣ
protected:
	LONGLONG						m_lBankerScore;						//ׯ�һ���
	LONGLONG						m_wCurrentBanker;					//��ǰׯ��
	BYTE							m_cbLeftCardCount;					//�˿���Ŀ
	bool							m_bEnableSysBanker;					//ϵͳ��ׯ

	//״̬����
protected:
	bool							m_bMeApplyBanker;					//�����ʶ

	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ
	CDlgControl						m_DlgControl;

	//������Դ
protected:
	CDirectSound					m_DTSDBackground;					//��������
	CDirectSound					m_DTSDCheer[3];						//��������


	struct SortTemp
	{
		LONG a;
		LONG b;
		LONG c;
		LONG d;
		SortTemp()
		{
			memset(this, 0, sizeof(*this));
		}
	};

	SortTemp m_TempData;
	CGameBank						m_GameBank;							//����

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

	virtual bool CGameClientDlg::OnFrameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize);

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
	//�˿���
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);

	//��ֵ����
protected:
	//����ׯ��
	void SetBankerInfo(WORD wBanker, LONGLONG lScore);
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
	//��������
	LRESULT OnContinueCard(WPARAM wParam, LPARAM lParam);

	//�ֹ�����
	LRESULT  OnOpenCard(WPARAM wParam, LPARAM lParam);
	//�Զ�����
	LRESULT  OnAutoOpenCard(WPARAM wParam, LPARAM lParam);

	LRESULT OnManageControl(WPARAM wParam, LPARAM lParam);

	LRESULT OnBank(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
