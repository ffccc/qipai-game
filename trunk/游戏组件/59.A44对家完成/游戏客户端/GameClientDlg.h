#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameDlg
{
	//��Ϸ����
protected:
	WORD							m_wLandUser;						//�ڿ��û�
	WORD							m_wBombTime;						//ը������
	BYTE							m_bCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							m_bHandCardCount;					//�˿���Ŀ
	BYTE							m_bHandCardData[MAX_COUNT];			//�����˿�
	bool							m_bAutoPlay ;						//�йܱ���

	//���Ʊ���
protected:
	BYTE							m_bTurnOutType;						//��������
	BYTE							m_bTurnCardCount;					//������Ŀ
	BYTE							m_bTurnCardData[MAX_COUNT];			//�����б�

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
	/////////////////////////////////////////////��ӵı���///////////////////////////////////////
	BYTE                            m_cbLiOrJue;                        //��������״̬
	BYTE                            m_cbLiOrJueCardData[MAX_COUNT];     //�������������е���

	//��������
protected:
	BYTE							m_cbRemnantCardCount;				//ʣ����Ŀ
	BYTE							m_cbDispatchCardCount;				//�ɷ���Ŀ
	BYTE							m_cbDispatchCardData[NORMAL_COUNT];	//�ɷ��˿�

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
	//�����˿�
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//�û��з�
	//bool OnSubLandScore(const void * pBuffer, WORD wDataSize);
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubOutCard(const void * pBuffer, WORD wDataSize);
	//��������
	bool OnSubPassCard(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubChaCard(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubDianCard(const void * pBuffer, WORD wDataSize);

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
	////////////////////////////////////////////////////////////
	//�����ж�
	bool VerdictChaCard();
	//�����ж�
	bool VerdictDianCard();
	////////////////////////////////////////////////////////////

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
	LRESULT OnAutoOutCard(WPARAM wParam, LPARAM lParam);
	//�Ҽ��˿�
	LRESULT OnLeftHitCard(WPARAM wParam, LPARAM lParam);
	//����˿�
	LRESULT OnRightHitCard(WPARAM wParam, LPARAM lParam);
	//�й���Ϣ
	LRESULT OnAutoPlay(WPARAM wParam, LPARAM lParam);
	//�����˿�
	LRESULT OnMessageSortCard(WPARAM wParam, LPARAM lParam);
	////////////////////////////////////////////////////////
	//��
	LRESULT OnCha(WPARAM wParam, LPARAM lParam);
	//����
	LRESULT OnNoCha(WPARAM wParam, LPARAM lParam);
	//��
	LRESULT OnDian(WPARAM wParam, LPARAM lParam);
	//����
	LRESULT OnNoDian(WPARAM wParam, LPARAM lParam);
    //����
	LRESULT OnHitLiGun(WPARAM wParam, LPARAM lParam);
    //���
	LRESULT OnHitJueGun(WPARAM wParam, LPARAM lParam);
    //ͬ�⡢��ͬ��
	LRESULT OnHitAgree(WPARAM wParam, LPARAM lParam);
	//������
	LRESULT OnHitNotLiGun(WPARAM wParam, LPARAM lParam);
	//�����
	LRESULT OnHitNotJueGun(WPARAM wParam, LPARAM lParam);

	////////////////////////////////////////////////////////
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
