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
	BYTE							m_bCardCount[GAME_PLAYER];	    	//�˿���Ŀ
	BYTE							m_bHandCardCount;					//��ǰ��Ŀ
	BYTE							m_bHandCardData[HAND_CARD_COUNT];	//�����˿�
	bool							m_bFirstGame ;                      //�׾���Ϸ
	bool							m_bAutoOutCard ;                    //�Զ�����

	//���Ʊ���
protected:
	BYTE							m_bTurnCardData[GAME_PLAYER];	    //�������
	WORD							m_wLastOutUser;                     //������
	WORD							m_wFirstOutUser ;                   //���ȳ���
	BYTE							m_bTurnCardCount;                   //�������  
	bool							m_bFirstOutSpade;                   //��һ����
	bool							m_bFirstHearts;                     //��һ����
	bool							m_bFirstClubs;                      //��һ����
	bool							m_bFirstDiamonds;                   //��һ����
	bool							m_bFirstOutCard;                    //��һ����

	//���ñ���
protected:
	bool							m_bDeasilOrder;						//����˳��
	DWORD							m_dwCardHSpace;						//�˿�����
	BYTE							m_bTimerElapse ;                    //��ʱ����


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
	//��������
protected:
	//�����ж�
	bool VerdictOutCard(bool bAutoOutCard=false);
	//�����ж�
	bool VerdictShowCard() ;
	//��һ�ֻ�ɫ
	bool VerdictFirstGameColor(BYTE bFirstOutColor , BYTE bCardDatabool , bool bAutoOutCard=false) ;
	//�Զ�����
	bool AutomatismOutCard() ;

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
	//��Ϣӳ��
	
	//��Ϣ����
protected:
	//�����˿�
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubOutCard(const void * pBuffer, WORD wDataSize) ;
	//�û�����
	bool OnSubShowCard(const void * pBuffer, WORD wDataSize) ;
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize) ;


protected:
	//��ʼ��Ϣ
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//�Զ�����
	LRESULT OnAutoOutCard(WPARAM wParam, LPARAM lParam);
	//ȡ������
	LRESULT OnCancelAutoOutCard(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnOutCard(WPARAM wParam, LPARAM lParam);
	//�������
	LRESULT OnShowCard(WPARAM wParam, LPARAM lParam);
	//��Ҳ���
	LRESULT OnNotShowCard(WPARAM wParam, LPARAM lParam);
	//�Ҽ��˿�
	LRESULT OnLeftHitCard(WPARAM wParam, LPARAM lParam);
	//����˿�
	LRESULT OnRightHitCard(WPARAM wParam, LPARAM lParam);
	//�ȴ���ʼ��ʱ
	LRESULT OnWaitStart(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT nIDEvent);
};

//////////////////////////////////////////////////////////////////////////
