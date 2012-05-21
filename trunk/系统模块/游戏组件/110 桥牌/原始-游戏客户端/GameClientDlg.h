#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"
#include "CallScore.h"


//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameDlg
{
	//�߼�����
protected:
	BYTE							m_cbPackCount;						//�Ƹ���Ŀ
	BYTE							m_cbMainColor;						//���ƻ�ɫ
	BYTE							m_cbMultiples;						//��Ϸ����
	BYTE							m_cbJuKuang;						//�ֿ�
	BYTE							m_cbMainValue;                      //������ֵ

protected:
	BYTE							m_cbValueOrder[2];					//��ֵ�ȼ�

	//������Ϣ
protected:
	BYTE							m_cbCallCard;						//�����˿�
	BYTE							m_cbCallCount;						//������Ŀ
	WORD							m_wCallCardUser;					//�����û�

	//״̬����
protected:
	WORD							m_wBankerUser;						//ׯ���û�
	WORD							m_wCurrentUser;						//��ǰ�û�

	//�÷���Ϣ
protected:
	BYTE							m_cbScoreCardCount;					//������Ŀ
	BYTE							m_cbScoreCardData[12*2];		    //�÷��˿�

	//˦�Ʊ���
protected:
	bool							m_bThrowCard;						//˦�Ʊ�־
	WORD							m_wThrowCardUser;					//˦���û�
	BYTE							m_cbResultCardCount;				//��ԭ�˿�
	BYTE							m_cbResultCardData[MAX_COUNT];		//��ԭ�˿�

	//���Ʊ���
protected:
	bool							m_bLastTurn;						//���ֱ�־
	WORD							m_wFirstOutUser;					//�����û�
	BYTE							m_cbOutCardCount[4][2];				//������Ŀ
	BYTE							m_cbOutCardData[4][2][MAX_COUNT];	//�����б�

	//�����˿�
protected:
	BYTE							m_cbConcealCount;					//������Ŀ
	BYTE							m_cbConcealCard[8];					//�����˿�

	//�û��˿�
protected:
	BYTE							m_cbHandCardCount;					//�˿���Ŀ
	BYTE							m_cbHandCardData[MAX_COUNT];		//�����˿�

	BYTE							m_HandOutCardCount[4];				//�˿�����
	BYTE							m_HandOutCardData[4][MAX_COUNT];	//�����˿�

	//���ñ���
protected:
	bool							m_bStustee;							//�йܱ�־
	bool							m_bDeasilOrder;						//����˳��
	DWORD							m_dwCardHSpace;						//�˿�����

	//��������
protected:
	BYTE							m_cbRemnantCardCount;				//ʣ����Ŀ
	BYTE							m_cbDispatchCardCount;				//�ɷ���Ŀ
	BYTE							m_cbDispatchCardData[MAX_COUNT];	//�ɷ��˿�
	WORD							m_cbFocusUser;						//add
	bool							m_bHandOutControl;					//���ֿ���


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
	//�����˿�
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//��Ϸ��ʼ
	bool OnSubGamePlay(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubOutCard(const void * pBuffer, WORD wDataSize);
	//˦�ƽ��
	bool OnSubThrowResult(const void * pBuffer, WORD wDataSize);
	//һ�ֽ���
	bool OnSubTurnBalance(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//�û��з�
	bool OnSubCallScore(const void * pBuffer, WORD wDataSize);

	//���ܺ���
protected:
	//�����ж�
	bool VerdictOutCard();
	//���ֳ����ж�
	bool VerdictHandOutCard();
	//�Զ�����
	bool AutomatismOutCard();
	//��ǰ�˿�
	bool SwitchToCurrentCard();
	//˦�ƽ��
	bool SwitchToThrowResult();
	//�ɷ��˿�
	bool DispatchUserCard(BYTE cbCardData[], BYTE cbCardCount);
	//�����˿�
	bool SetHandCardControl(BYTE cbCardData[], BYTE cbCardCount);
	//���������˿�
	bool SetHandOutCardControl(BYTE cbCardData[], BYTE cbCardCount, WORD wMeChairID, WORD wViewChairID);

	//���溯��
protected:

	//��Ϣӳ��
protected:
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//��ʼ��Ϣ
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnOutCard(WPARAM wParam, LPARAM lParam);
	//������ʾ
	LRESULT OnOutPrompt(WPARAM wParam, LPARAM lParam);
	//�����뿪
	LRESULT OnRuquestLeave(WPARAM wParam, LPARAM lParam);
	//�����˿�
	LRESULT OnLastTurnCard(WPARAM wParam, LPARAM lParam);
	//�Ϲܿ���
	LRESULT OnStusteeControl(WPARAM wParam, LPARAM lParam);
	//����˿�
	LRESULT OnLeftHitCard(WPARAM wParam, LPARAM lParam);
	//�Ҽ��˿�
	LRESULT OnRightHitCard(WPARAM wParam, LPARAM lParam);
	//�з�
	LRESULT OnLandScore(WPARAM wParam, LPARAM lParam);
	//�ӱ�
	LRESULT OnMultiples(WPARAM wParam, LPARAM lParam);
	//����
	LRESULT OnGiveUpScore(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
