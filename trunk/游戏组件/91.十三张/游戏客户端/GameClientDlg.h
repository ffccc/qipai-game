#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameDlg
{
	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ

	//���ñ���
protected:
	DWORD							m_dwCardHSpace;						//�˿�����
	static int						m_bCompareTime;						//�Ƚ�ʱ��
	BYTE							m_bShowCardTime;					//̯��ʱ��

	//��Ϸ����
protected:
	BYTE							m_bHandCardData[HAND_CARD_COUNT];	//�����˿�
	BYTE							m_bTmpHandCardData[HAND_CARD_COUNT];//�����˿�
	BYTE							m_bHandCardCount ;					//�˿���Ŀ
	WORD							m_wBanker;							//ׯ�����
	LONG							m_nChip[GAME_PLAYER];				//ѹע��С
	BYTE							m_bSegmentCard[GAME_PLAYER][3][5];	//�ֶ��˿�
	UINT_PTR						m_nTimer ;							//��ʱ��ID
	LONG							m_wMaxChip;							//�����ע
	bool							m_bFinishSegment;					//��ɷֶ�
	bool							m_bReady;							//�Ƿ�׼��
	bool							m_bCanSetChip;						//�����ע
	LONG							m_lGameScore[GAME_PLAYER];			//��һ���
	
	//��������
public:
	//���캯��
	CGameClientDlg();
	//��������
	virtual ~CGameClientDlg();

	//��Ϣ����
protected:
	//�����˿�
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize) ;
	//������ע
	bool OnSubSetChip(const void * pBuffer , WORD wDataSize) ;
	//���÷ֶ�
    bool OnSubSetSegment(const void * pBuffer , WORD wDataSize) ;
    //���̯��
	bool OnSubShowCard(const void * pBuffer , WORD wDataSize) ;
	//�Ƚ��˿�
	bool OnSubCompareCard(const void * pBuffer , WORD wDataSize) ;
	//̯��ʱ��
	bool OnSubShowCardTime(const void * pBuffer , WORD wDataSize) ;



    //��������
protected:
	//�Զ�����
	void AutomaticOutCard() ;
	//��ȡ��ע
	void GetChips( LONG lChipArray[SET_CHIP_COUNT] ) ;

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
	//��ʼ��Ϣ
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//ѹע��Ϣ
	LRESULT OnSetChip(WPARAM wParam, LPARAM lParam);
	//����˿�
	LRESULT OnLeftHitCard(WPARAM wParam, LPARAM lParam) ;
	//����ǰ��
	LRESULT OnFrontCard(WPARAM wParam, LPARAM lParam) ;
	//�����ж�
	LRESULT OnMidCard(WPARAM wParam, LPARAM lParam) ;
	//���ú��
	LRESULT OnBackCard(WPARAM wParam, LPARAM lParam) ;
	//���̯��
	LRESULT OnShowCard(WPARAM wParam, LPARAM lParam) ;	
	//��������
	LRESULT	OnDescendSort(WPARAM wParam, LPARAM lParam) ;	
	//��������
	LRESULT	OnAscendSort(WPARAM wParam, LPARAM lParam) ;	
	//��ɫ����
	LRESULT	OnColorSort(WPARAM wParam, LPARAM lParam) ;	
	//�Զ�����
	LRESULT OnAutoOutCard(WPARAM wParam, LPARAM lParam);
	//�ָ��˿�
	LRESULT OnRestoreCard(WPARAM wParam, LPARAM lParam) ;
	//��ׯ��Ϣ
	LRESULT OnCallBanker(WPARAM wParam, LPARAM lParam) ;
	//������Ϣ
	LRESULT OnNoCallBanker(WPARAM wParam, LPARAM lParam) ;

	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
