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
	LONGLONG							m_lMeMaxScore;						//�����ע
	LONGLONG							m_lAreaLimitScore;					//��������
	LONGLONG							m_lApplyBankerCondition;			//��������

	//������ע
protected:
	LONGLONG							m_lMeScore1;						//��ע
	LONGLONG							m_lMeScore2;						//��ע
	LONGLONG							m_lMeScore3;						//��ע
	LONGLONG							m_lMeScore4;						//��ע
	LONGLONG							m_lMeScore5;						//��ע	
	LONGLONG							m_lMeScore6;						//ƽע
	LONGLONG							m_lMeScore7;						//��ע
	LONGLONG							m_lMeScore8;						//��ע

	//ׯ����Ϣ
protected:
	LONGLONG							m_lBankerScore;						//ׯ�һ���
	WORD							m_wCurrentBanker;					//��ǰׯ��
	bool							m_bEnableSysBanker;					//ϵͳ��ׯ

	//״̬����
protected:
	bool							m_bMeApplyBanker;					//�����ʶ	

	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ
	//CGameBank						m_GameBank;							//����
	//������Դ
protected:
	CDirectSound					m_DTSDBackground;					//��ע����
	CDirectSound					m_DTSDCheer[3];						//��������

protected:
	CRect							rtMax;								//��󻯰�ť����
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
	//��ʼ����
	virtual BOOL OnInitDialog();

	//�����Ϣ
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point){;}

	afx_msg void OnGetMinMaxInfo(MINMAXINFO * lpMMI);

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);


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
	LRESULT OnBank(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
