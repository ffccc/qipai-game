#pragma once

#include "Stdafx.h"
#include "..\\��Ϸ������\\GameLogic.h"
#include "GameClientView.h"
#include "IDataCtrl.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameDlg, public IDataCtrl
{
	//��Ϸ����
protected:
	bool							m_bChiHu;							//�Ժ���־
	WORD							m_wHeapHand;						//����ͷ��
	WORD							m_wHeapTail;						//����β��
	WORD							m_wBankerUser;						//ׯ���û�
	WORD							m_wCurrentUser;						//��ǰ�û�
	BYTE							m_bLeftCardCount;					//ʣ����Ŀ
	LONG							m_lScoreLeft[4];					//ʣ��Ƹ�
	LONG							m_lCellScore;						//��λע��
	WORD							m_wSiceCount;						//������Ŀ

	//������Ϣ
protected:
	BYTE							m_bOutCardCount;					//������Ŀ
	WORD							m_wOutCardUser[108];				//�����û�
	BYTE							m_cbOutCardData[108];				//�����˿�

	//����˿�
protected:
	BYTE							m_bWeaveCount[4];					//�����Ŀ
	tagWeaveItem					m_WeaveItemArray[4][4];				//����˿�

	//�˿˱���
protected:
	BYTE							m_bHeapCardInfo[4][2];				//������Ϣ
	BYTE							m_cbCardIndex[MAX_INDEX];			//�����˿�

	//����ܵ���ر���
	WORD							m_wKingBrotherHeapPos;
	WORD							m_wKingBrotherCount;

	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ

	//���Ա���
protected:
	bool							m_bSelfReady;						//�Ƿ��Զ���ready
	bool							m_bReady[4];						//��ready
	BYTE							m_cbOverTimeCount;					//��ʱ����
	bool							m_bConsign;							//�Ƿ��й�
	
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

	//IDataCtrl
public:
	//��ȡ��Ϸ״̬
	virtual BYTE	GetGameStatus(){ return GetGameStatus(); }	
	//��ȡׯ��
	virtual WORD	GetBankerUser(){ return m_wBankerUser; }
	//��ȡ����
	virtual BYTE	GetKingCardData(){ return m_GameLogic.GetKingCardData(); };	
	//��ȡ����ĵܵ�
	virtual BYTE	GetKingBrotherData(){ return m_GameLogic.GetBrotherFromKing(m_GameLogic.GetKingCardData());	}
	//�Ƿ�׼��
	virtual bool	IsUserReady(WORD wChairID);		
	//�Ƿ��й�
	virtual bool	IsConsign(){ return m_bConsign;	}
	//��ȡʣ��Ƹ�
	virtual LONG    GetScoreLeft(WORD wChairID){ return m_lScoreLeft[wChairID]; }
	//��ȡ��ע
	virtual LONG	GetCellScore(){ return m_lCellScore; }
	//�Ƿ��Թ���
	virtual bool	IsLookOn(){ return IsLookonMode();	}
	//�Ƿ������Թ�
	virtual bool	IsAllowLookOn(){ return CGameFrameDlg::IsAllowUserLookon(); }
	//�Ƿ�����Թ�
	virtual bool	IsEnableLookOn(){ return CGameFrameDlg::IsAllowLookon(); }
	//�Ƿ�������Ч
	virtual bool	IsEnableSound(){ return CGameFrameDlg::IsEnableSound(); }

	//��Ϣ����
protected:
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubOutCard(const void * pBuffer, WORD wDataSize);
	//�����˿�
	bool OnSubOperateCard(const void * pBuffer, WORD wDataSize);
	//�����˿�
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//ϵͳ��Ϣ
	bool OnSubSysMsg(const void * pBuffer, WORD wDataSize);
	//���׼��
	bool OnSubUserReady(const void * pBuffer, WORD wDataSize);
	//ע��ı�
	bool OnSubStake(const void * pBuffer, WORD wDataSize);

	//��������
protected:
	//�۳��˿�
	void DeductionTableCard(bool bHeadCard);
	//������Ϣ
	void SetHandCardData(BYTE cbCardData[], WORD wCardCount, BYTE cbCurrentCard);
	//����֮��ǰ��
	void SetHandCurrentCard(BYTE cbCurrentCard);
	//���ܴ���
	BYTE GetIsolatedCard();
	//�����й�
	void SetConsign(bool bEnter);
	//���ö�ʱ��
	bool SetGameTimer(WORD wChairID, UINT nTimerID, UINT nElapse);
	//���ų�������
	void PlayOutCardSound(WORD wChairID, BYTE cbCardData);
	//������������
	void PlayChairSound(WORD wChairID, LPCSTR lpstrName);
	//DEBUG
	void _DebugAssert();

	//��Ϣӳ��
protected:
	//��ʼ��Ϣ
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//���Ʋ���
	LRESULT OnOutCard(WPARAM wParam, LPARAM lParam);
	//�˿˲���
	LRESULT OnCardOperate(WPARAM wParam, LPARAM lParam);
	//�йܲ���
	LRESULT OnConsignChanged(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
