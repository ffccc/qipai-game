#pragma once

#include "Stdafx.h"
#include "GameClientView.h"


//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameDlg
{
	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ
	
	//��Ϸ����
protected:
	WORD							m_wBankerUser;						//ׯ���û�
	BYTE							m_bCardCount[4];					//�˿���Ŀ
	BYTE                            m_bHandCardCount;                   //�˿���Ŀ
	BYTE                            m_bHandCardData[28];                //�����˿�
	BYTE                            m_bPayTribute;						//�Ƿ����
	bool                            m_bGameEnd;						    //��Ϸ�Ƿ����
    WORD							m_wToUser;							//������
	bool							m_bAI;								//�Ƿ��й�
	WORD                            m_wTime;							//ʱ��
	bool                            m_bLookon;							//�Ƿ�������

	//���Ʊ���
protected:
	short							m_bTurnCardType;				    //��������
	BYTE                            m_bTurnCardCount;					//������Ŀ
	BYTE                            m_bTurnCardData[27];                //��������

	//���ñ���
protected:
	bool							m_bDeasilOrder;						//����˳��
	DWORD							m_dwCardHSpace;						//�˿�����
	enRoomType                      m_RoomType;							//��������
	WORD							m_wServerType;						//����������
	
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
	//�û�����
	bool OnSubOutCard(const void * pBuffer, WORD wDataSize);
	//��������
	bool OnSubPassCard(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//�����˿�
	bool OnSubPayTribute(const void * pBuffer, WORD wDataSize);

    
	//��������
protected:
	//�����ж�
	bool VerdictOutCard(void);
    //�����ж�
    bool VerdictPassCard();
	// �Ƿ�ɽ���
	bool VerdictChangeCard(void);
	//�Զ�����
	void AutoOutCard(void);

	//��Ϣӳ��
protected:
	//�˳�
	afx_msg void OnClose();
	//��ʼ��Ϣ
	LRESULT OnStart			(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnOutCard		(WPARAM wParam, LPARAM lParam);
	//��������
	LRESULT OnPassCard		(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnPayTribute	(WPARAM wParam, LPARAM lParam);
	//�Ҽ��˿�
	LRESULT OnLeftHitCard	(WPARAM wParam, LPARAM lParam);
	//����˿�
	LRESULT OnRightHitCard	(WPARAM wParam, LPARAM lParam);
	//������Ϣ
    LRESULT OnSortCard		(WPARAM wParam,	LPARAM lParam);
	//�ı�λ��
	LRESULT OnChangePosition(WPARAM wParam,	LPARAM lParam);
	//�йܰ�ť
	LRESULT OnAI			(WPARAM wParam,	LPARAM lParam);
	
	DECLARE_MESSAGE_MAP()
	
	//�����˳�����
protected:
	//�����˳�
	bool OnRequest(const void * pBuffer);
	//�ָ��˳�
	bool OnAnswer(const void * pBuffer);
	
	//��ʾ�÷�
protected:
	void ShowScore(void);

};

//////////////////////////////////////////////////////////////////////////
