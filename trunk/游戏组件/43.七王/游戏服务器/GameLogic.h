#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////

//�˿�����
#define CT_ERROR					0									//��������
#define CT_SINGLE					1									//��������
#define CT_DOUBLE					2									//��������
#define	CT_DRAGON					3									//˳������
#define CT_THREE_TIAO				4									//��������
#define CT_DOUBLE_DRAGON            5									//˫��
#define CT_FOUR                     6									//��������
//#define CT_TONG_HUA					5									//ͬ������
//#define CT_HU_LU					6									//��«����
//#define CT_TIE_ZHI					7									//��֧����
//#define CT_TONG_HUA_SHUN			8									//ͬ��˳��

#define CT_DOUBLE_REDTEN            7									//˫��ʮ

//�궨��
#define MAX_COUNT					13									//�����Ŀ

//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����
//������
#define FangTen                     1                                    //��ʮ
#define TaoTen                      2                                    //��ʮ

//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼���
class CGameLogic
{
	friend class CTableFrameSink;
	//��������
protected:
	static const BYTE				m_bCardListData[54];				//�˿�����
	static const BYTE				m_bBankListData[12];				//ѡׯ����

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
	//�����˿�
	void RandBankCardList(BYTE bCardBuffer[], BYTE bBufferCount);
	//�����˿�
	void RandCardList(BYTE bDCardData[],BYTE &bDCardCount,BYTE bSCardData[],BYTE &bSCardCount);
	//ɾ���˿�
	bool RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE bCardData[], BYTE bCardCount);

	//�߼�����
public:

	//��Ч�ж�
	bool IsValidCard(BYTE cbCardData);
	//�߼���ֵ
	BYTE GetCardLogicValue(BYTE cbCardData);

	//���ͺ���
public:
	//�Ƿ����
	bool IsDouble(BYTE bCardData[], BYTE bCardCount);
	////�Ƿ���֧
	//bool IsTieZhi(BYTE bCardData[], BYTE bCardCount);
	////�Ƿ��«
	//bool IsHuLu(BYTE bCardData[], BYTE bCardCount);
	////�Ƿ�ͬ��
	//bool IsTongHua(BYTE bCardData[], BYTE bCardCount);
	//�Ƿ���
	bool IsShunZhi(BYTE bCardData[], BYTE bCardCount);
	//�Ƿ�����
	bool IsSanTiao(BYTE bCardData[], BYTE bCardCount);
	//�Ƿ�˫��
	bool IsDoubleDragon(BYTE bCardData[],BYTE bCardCount);
	//�ĸ�
	bool IsFour(BYTE bCardData[],BYTE bCardCount);
	//�ϲ�����
	void CombinationArray(BYTE bFirstCardData[],BYTE bFirstCardCount,BYTE bNextCardData[],BYTE bNextCardCount);


	//���ܺ���
public:
	//�Ա��˿�
	bool CompareCard(BYTE bFirstCardData, BYTE bNextCardData);
	//��
	//�Ա��˿�
	bool CompareCard(BYTE bFirstList[], BYTE bNextList[], BYTE bFirstCount, BYTE bNextCount);
	//�����ж�
	bool SearchOutCard(BYTE bCardData[], BYTE bCardCount, BYTE bTurnCardData[], BYTE bTurnCardCount, BYTE bTurnOutType, BYTE cbOutCard[MAX_COUNT]);
};

//////////////////////////////////////////////////////////////////////////

#endif