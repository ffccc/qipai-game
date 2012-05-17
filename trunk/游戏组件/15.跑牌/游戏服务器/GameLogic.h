#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////

//�˿�����
#define CT_ERROR					0									//��������
#define CT_SINGLE					1									//��������
#define CT_DOUBLE					2									//��������
#define CT_THREE_TIAO				3									//��������
#define CT_FOUR_TIAO				4									//��������	
#define CT_FOUR_LINE				5									//��������
#define CT_ALL_BIG					6									//ȫ��
#define CT_ALL_SMALL				7									//ȫС

//�궨��
#define MAX_COUNT					13									//�����Ŀ

//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����

//////////////////////////////////////////////////////////////////////////
//�����ṹ
struct tagAnalyseResult
{
	BYTE 							bFourCount;							//������Ŀ
	BYTE 							bThreeCount;						//������Ŀ
	BYTE 							bDoubleCount;						//������Ŀ
	BYTE							bSignedCount;						//������Ŀ
	BYTE 							bFourLogicVolue[3];					//�����б�
	BYTE 							bThreeLogicVolue[4];				//�����б�
	BYTE 							bDoubleLogicVolue[6];				//�����б�
	BYTE							m_bSCardData[13];					//�����˿�
	BYTE							m_bDCardData[13];					//�����˿�
	BYTE							m_bTCardData[13];					//�����˿�
	BYTE							m_bFCardData[13];					//�����˿�
};
////////////////////////////////////////////////////////////////////////
//��Ϸ�߼���
class CGameLogic
{
	//��������
protected:
	static const BYTE				m_bCardListData[52];				//�˿�����

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//���ͺ���
public:
	//��ȡ����
	BYTE GetCardType(BYTE bCardData[], BYTE bCardCount);
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE bCardData) { return bCardData&LOGIC_MASK_VALUE; }
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE bCardData) { return bCardData&LOGIC_MASK_COLOR; }

	//���ƺ���
public:
	//�����˿�
	void SortCardList(BYTE bCardData[], BYTE bCardCount);
	//�����˿�
	void RandCardList(BYTE bCardBuffer[], BYTE bBufferCount);
	//ɾ���˿�
	bool RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE bCardData[], BYTE bCardCount);

	//�߼�����
public:
	//��Ч�ж�
	bool IsValidCard(BYTE cbCardData);
	//�߼���ֵ
	BYTE GetCardLogicValue(BYTE cbCardData);

	bool MaxValue(BYTE FirstData[],BYTE NextData[],BYTE Count);

	//���ͺ���
public:
	//�Ƿ����
	bool IsDouble(BYTE bCardData[], BYTE bCardCount);
	//�Ƿ�����
	bool IsSanTiao(BYTE bCardData[], BYTE bCardCount);
	//�Ƿ�����
	bool IsFourTiao(BYTE bCardData[], BYTE bCardCount);
	//�Ƿ�����
	bool IsFourLine(BYTE bCardData[], BYTE bCardCount);

	//�Ƿ�4��2
	bool IsAllBig(BYTE bCardData[],BYTE bCardCount);
	//�Ƿ�ȫС
	bool IsAllSmall(BYTE bCardData[],BYTE bCardCount);

	//���ܺ���
public:
	//�Ա��˿�
	bool CompareCard(BYTE bFirstCardData, BYTE bNextCardData);
	//�Ա��˿�
	bool CompareCard(BYTE bFirstList[], BYTE bNextList[], BYTE bFirstCount, BYTE bNextCount);
	//�����˿�
	void AnalysebCardData(const BYTE bCardData[], BYTE bCardCount, tagAnalyseResult & AnalyseResult);
	//�����ж�
	bool SearchOutCard(BYTE bCardData[], BYTE bCardCount, BYTE bTurnCardData[], BYTE bTurnCardCount, BYTE bTurnOutType, BYTE cbOutCard[MAX_COUNT]);
};

//////////////////////////////////////////////////////////////////////////

#endif