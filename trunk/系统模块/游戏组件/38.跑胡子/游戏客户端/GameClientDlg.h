#pragma once

#include "Stdafx.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//��������
#define ACTION_TIP					1									//��ʾ����
#define ACTION_TI_CARD				2									//���ƶ���
#define ACTION_PAO_CARD				3									//���ƶ���
#define ACTION_WEI_CARD				4									//���ƶ���
#define ACTION_PENG_CARD			5									//���ƶ���
#define ACTION_CHI_CARD				6									//���ƶ���
#define ACTION_OUT_CARD				7									//���ƶ���
#define ACTION_SEND_CARD			8									//���ƶ���
#define ACTION_OPERATE_NOTIFY		9									//���ƶ���
#define ACTION_OUT_CARD_NOTIFY		10									//���ƶ���

//��������
struct tagUserAction
{
	BYTE							cbActionKind;						//��������
	BYTE							cbActionBuffer[64];					//��������
};

//��ʾ����
struct tagActionTip
{
	WORD							wChairID;							//�û�λ��
	BYTE							cbActionFlags;						//������ʶ
};

//���ƶ���
struct tagActionTiCard
{
	BYTE							cbIndex;							//��������
	WORD							wChairID;							//�û�λ��
	BYTE							bHuXiCount;							//��Ϣ��Ŀ
	BYTE							cbRemoveCount;						//ɾ����Ŀ
};

//���ƶ���
struct tagActionPaoCard
{
	BYTE							cbIndex;							//��������
	WORD							wChairID;							//�û�λ��
	BYTE							bHuXiCount;							//��Ϣ��Ŀ
	BYTE							cbRemoveCount;						//ɾ����Ŀ
};

//���ƶ���
struct tagActionWeiCard
{
	BYTE							cbIndex;							//��������
	WORD							wChairID;							//�û�λ��
	BYTE							bHuXiCount;							//��Ϣ��Ŀ
};

//���ƶ���
struct tagActionPengCard
{
	BYTE							cbIndex;							//��������
	WORD							wChairID;							//�û�λ��
	BYTE							bHuXiCount;							//��Ϣ��Ŀ
};

//���ƶ���
struct tagActionChiCard
{
	BYTE							cbIndex;							//��������
	WORD							wChairID;							//�û�λ��
	BYTE							bHuXiCount;							//��Ϣ��Ŀ
	BYTE							cbActionCard;						//�����˿�
	BYTE							cbResultCount;						//�����Ŀ
	BYTE							cbCardData[3][3];					//�������
};

//���ƶ���
struct tagActionOutCard
{
	WORD							wOutCardUser;						//�����û�
	BYTE							cbOutCardData;						//�����˿�
};

//���ƶ���
struct tagActionSendCard
{
	WORD							wAttachUser;						//���û�
	BYTE							cbSendCardData;						//�����˿�
};

//������ʾ
struct tagActionOperateNotify
{
	WORD							wResumeUser;						//��ԭ�û�
	BYTE							cbOperateCode;						//��������
	BYTE							cbCurrentCard;						//��ǰ�˿�
};

//������ʾ
struct tagActionOutCardNotify
{
	BYTE							bOutCard;							//���Ʊ�־
	WORD							wCurrentUser;						//��ǰ�û�
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameDlg
{
	//��Ϸ����
protected:
	WORD							m_wBankerUser;						//ׯ���û�
	BYTE							m_bUserCardCount[3];				//�˿���Ŀ
	BYTE							m_bUserHuXiCount[3];				//��Ϣ��Ŀ

	//��������
protected:
	bool							m_bOutCard;							//���Ʊ�־
	WORD							m_wResumeUser;						//��ԭ�û�
	WORD							m_wCurrentUser;						//��ǰ�û�

	//����˿�
protected:
	BYTE							m_bWeaveItemCount[3];				//�����Ŀ
	tagWeaveItem					m_WeaveItemArray[3][7];				//����˿�

	//�˿˱���
protected:
	WORD							m_wOutCardUser;						//�����û�
	BYTE							m_cbOutCardData;					//�����˿�
	BYTE							m_cbCardIndex[MAX_INDEX];			//�����˿�

	//��������
protected:
	WORD							m_wActionCount;						//������Ŀ
	tagUserAction					m_UserActionArray[32];				//�û�����

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

	//��������
protected:
	//��ֹ����
	bool HangUserAction();
	//ִ�ж���
	bool ExecuteActionSendCard(BYTE cbCardData, WORD wAttachUser);
	//ִ�ж���
	bool ExecuteActionOutCard(WORD wOutCardUser, BYTE cbOutCardData);
	//ִ�ж���
	bool ExecuteActionOutCardNotify(WORD wCurrentUser, BYTE bOutCard);
	//ִ�ж���
	bool ExecuteAction(BYTE cbActionKind, void * pActionInfo, WORD wDataSize);
	//ִ�ж���
	bool ExecuteActionOperateNotify(BYTE cbOperateCode, BYTE cbOperateCard, WORD wResumeUser);

	//��������
protected:
	//ִ�ж���
	bool ExecuteActionWeiCard(WORD wChairID, BYTE cbCardData);
	//ִ�ж���
	bool ExecuteActionPengCard(WORD wChairID, BYTE cbCardData);
	//ִ�ж���
	bool ExecuteActionTiCard(WORD wChairID, BYTE cbCardData, BYTE cbRemoveCount);
	//ִ�ж���
	bool ExecuteActionPaoCard(WORD wChairID, BYTE cbCardData, BYTE cbRemoveCount);
	//ִ�ж���
	bool ExecuteActionChiCard(WORD wChairID, BYTE cbCardData, BYTE cbResultCount, BYTE cbResultData[][3]);

	//��������
protected:
	//���º�Ϣ
	bool UpdateUserHuXiCount(WORD wChairID);
	//ɾ���˿�
	bool RemoveControlCard(WORD wChairID, BYTE cbRemoveCard[], WORD wCardCount);

	//��Ϣ����
protected:
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubUserTiCard(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubUserPaoCard(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubUserWeiCard(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubUserPengCard(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubUserChiCard(const void * pBuffer, WORD wDataSize);
	//������ʾ
	bool OnSubOperateNotify(const void * pBuffer, WORD wDataSize);
	//������ʾ
	bool OnSubOutCardNotify(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubOutCard(const void * pBuffer, WORD wDataSize);
	//��������
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);

	//��Ϣӳ��
protected:
	//ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//��ʼ��Ϣ
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//���Ʋ���
	LRESULT OnOutCard(WPARAM wParam, LPARAM lParam);
	//ѡ�Ʋ���
	LRESULT OnChooseCard(WPARAM wParam, LPARAM lParam);
	//�˿˲���
	LRESULT OnCardOperate(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
