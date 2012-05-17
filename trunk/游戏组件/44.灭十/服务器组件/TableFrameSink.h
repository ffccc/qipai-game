#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//�˿�����
#define CT_SINGLE								0						//����
#define CT_COLOR								1						//����
#define CT_DOUBLE								2						//����

//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����

//����ԭ��
#define GER_NO_PLAYER						0x10						//û�����

//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼���
class CGameLogic
{
	//��������
private:
	static const BYTE				m_bCardArray[52];					//�˿˶���

	//��������
private:
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE bCardData) 
	{ 
		if ((bCardData&LOGIC_MASK_VALUE)==1) 
		{
			return 21;
		}
		return bCardData&LOGIC_MASK_VALUE; 
	}
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE bCardData) { return bCardData&LOGIC_MASK_COLOR; }
	//�߼���ֵ
	BYTE GetCardLogicValue(BYTE bCardData)
	{
		//�˿�����
		BYTE bCardColor=GetCardColor(bCardData);
		BYTE bCardValue=GetCardValue(bCardData);

		//ת����ֵ
		if (bCardValue==21) return 1;
		if (bCardValue>10) return 0;

		return bCardValue;
	}
	BYTE GetMaxColor(BYTE bCardList[]);
	//���ͺ���
public:
	//�Ƿ����
	BYTE GetCardType(BYTE bCardList[], BYTE bCardCount);

	

	//���ܺ���
public:
	//�����˿�
	void RandCard(BYTE bCardBuffer[], BYTE bBufferCount);
	//�����˿�
	void SortCard(BYTE bCardList[], BYTE bCardCount);
	//�Ա��˿�
	bool CompareCard(BYTE bFirstList[], BYTE bNextList[], BYTE bCardCount);

	BYTE GetMaxValue(BYTE bCardList[]);
	BYTE GetMinValue(BYTE bCardList[]);
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink
{
	//��Ϸ����
protected:
	WORD							m_wOperaCount;							//��������
	WORD							m_wCurrentUser;							//��ǰ�û�
	BYTE							m_bSendCardCount;						//������Ŀ
	BYTE							m_bTableCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							m_bTableCardArray[GAME_PLAYER][4];		//�����˿�

	//�����Ϣ
protected:
	LONG							m_lMaxGold;								//�����ע
	LONG							m_lTurnBasicGold;						//������ע
	LONG							m_lTableGold[2*GAME_PLAYER];			//��ע���

	//��������
protected:
	BYTE							m_bPlayStatus[GAME_PLAYER];				//��Ϸ״̬

	//�������
protected:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;						//��ܽӿ�
	const tagGameServiceOption		* m_pGameServiceOption;					//���ò���

	//���Ա���
protected:
	static const WORD				m_wPlayerCount;							//��Ϸ����
	static const enStartMode		m_GameStartMode;						//��ʼģʽ

	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() { if (IsValid()) delete this; return ; }
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CTableFrameSink))?true:false; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��ʼ��
	virtual bool __cdecl InitTableFrameSink(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual void __cdecl RepositTableFrameSink();

	//��Ϣ�ӿ�
public:
	//��ʼģʽ
	virtual enStartMode __cdecl GetGameStartMode();
	//��Ϸ״̬
	virtual bool __cdecl IsUserPlaying(WORD wChairID);

	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool __cdecl OnEventGameStart();
	//��Ϸ����
	virtual bool __cdecl OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool __cdecl SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret);

	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool __cdecl OnTimerMessage(WORD wTimerID, WPARAM wBindParam);
	//��Ϸ��Ϣ����
	virtual bool __cdecl OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ����
	virtual bool __cdecl OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//��Ϸ�¼�
protected:
	//�����¼�
	bool OnUserGiveUp(WORD wChairID);
	//��ע�¼�
	bool OnUserAddGold(WORD wChairID, LONG lGold, bool bCancel);

	//��Ϸ����
protected:
	//�ƶ�ʤ��
	WORD DeduceWiner(BYTE bBeginPos, BYTE bEndPos);
};

//////////////////////////////////////////////////////////////////////////

#endif