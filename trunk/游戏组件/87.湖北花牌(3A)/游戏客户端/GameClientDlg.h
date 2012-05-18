#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "HuCardDlg.h"
#include "GameClientView.h"
#include "mmsystem.h"
//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameDlg
{
public:
	bool				m_IsTuoGuan;						//����ǰ״̬�Ƿ����й�״̬
	int                m_wPlayNativeSound;                 //���ű��ػ�

	//�������������еı���
protected:

	WORD			    m_wBankerUser;						    //���ֵ�ׯ���û�

	BYTE                m_bCardCount[GAME_PLAYER];               //��¼���ҵ��˿�����
	BYTE                m_SiTongCount;                           //��ͳ�Ĵ���
	BYTE                m_SiTongIndex[8];                        //��ͳ������

	BYTE                m_WuTongCount;                           //��ͳ�Ĵ���
	BYTE                m_WuTongIndex[8];                        //��ͳ������

	BYTE                m_cbHuaWeaveCardIndex[5];                //5�Ż�������
	BYTE                m_cbHuaCardIndex[5];                     //5�Ż�������

	//����˿�
	BYTE				m_cbWeaveCount[GAME_PLAYER];		     //�����Ŀ
	tagWeaveItem		m_WeaveItemArray[GAME_PLAYER][8];	     //����˿�

	bool                m_bGameStart;                            //�����Ϸ�Ƿ�ʼ

	//�˿˱���
	BYTE				m_cbLeftCardCount;					     //ʣ����Ŀ
	BYTE				m_cbCardIndex[MAX_INDEX];			     //�����˿�

	BYTE				m_cbDiscardCount[GAME_PLAYER];		     //������Ŀ
	BYTE				m_cbDiscardCard[GAME_PLAYER][34];	     //������¼

	//�����������ǣ������ʱ�����ܺ��ƾ�ֱ�ӷ���
	bool				b_IsHD;								     //�Ƿ񺣵�

	//������ÿ�ֲ����еı���
protected:
	BYTE                m_cbSex;                                 //�����û����Ա�
	WORD				m_wCurrentUser;						     //��ǰ���ڲ������û�

	//���汾�ֹ����У��û��ܲ��������
	BYTE                m_cbActionMask;                            //������
	BYTE                m_cbActionCard;                            //������

	//������Ϣ
	WORD				m_wOutCardUser;						     //�����û�
	BYTE				m_cbOutCardData;					     //�����˿�

	//�ؼ�����
protected:
	CGameLogic			m_GameLogic;						    //��Ϸ�߼�
	CGameClientView		m_GameClientView;					    //��Ϸ��ͼ

	
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
	//��ׯ
	bool OnSubGameDecideBanker(const void * pBuffer, WORD wDataSize);
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubOutCard(const void * pBuffer, WORD wDataSize);
	//������Ϣ
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//������ʾ
	bool OnSubOperateNotify(const void * pBuffer, WORD wDataSize);
	//�������
	bool OnSubOperateResult(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);

	//�����û����е���
	bool SetWoindowData(BYTE cbCardData[] ,BYTE cbDataCount);
	//��������
protected:

	//�����������������������������ͣ��ó������������ݣ����浽
	bool GetHuCardWeave(BYTE cbCardIndex[],BYTE cbWeaveKind[],BYTE cbYaKou[],tagHuCardWeave &HuCardWeave);
	//�����������������������������ͣ��ó������������ݣ����浽
	bool GetHuCardWeave(BYTE cbCardIndex[],BYTE cbWeaveKind[],BYTE cbYaKou[],BYTE cbHuaWeaveCount[],BYTE cbHuaHandCount[],BYTE cbWeaveCount,tagHuCardWeave &HuCardWeave);

	//��������
	void PlayCardSound(BYTE cbCardData);
	//��������
	void PlayActionSound(BYTE cbAction);
	//��ʾ����
	void ShowTongOperateControl(BYTE cbUserAction, BYTE cbActionCard);
	//�����û�����
	void EnableOutCard();
	//��ֹ�û�����
	void DisableOutCard();
	//�������û����е��ư�һ���Ĺ���ֵ���ͬ������
	void StartSendAfterFace();

	//��Ϣӳ��
protected:
	//ȡ���й���Ϣ
	LRESULT OnDisTuoGuan(WPARAM wParam, LPARAM lParam);
	//�й���Ϣ
	LRESULT OnTuoGuan(WPARAM wParam, LPARAM lParam);

	//��ʼ��Ϣ
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//���Ʋ���
	LRESULT OnOutCard(WPARAM wParam, LPARAM lParam);
	//�˿˲���
	LRESULT OnCardOperate(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void GameStartedFace(void);
};

//////////////////////////////////////////////////////////////////////////
