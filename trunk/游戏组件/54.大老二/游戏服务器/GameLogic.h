#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////

//�˿�����
#define CT_ERROR					0									//��������
#define CT_SINGLE					1									//��������
#define CT_DOUBLE					2									//��������
#define CT_THREE_TIAO				3									//��������
#define	CT_SHUN_ZI					4									//˳������
#define CT_TONG_HUA					5									//ͬ������
#define CT_HU_LU					6									//��«����
#define CT_TIE_ZHI					7									//��֧����
#define CT_TONG_HUA_SHUN			8									//ͬ��˳��

//�궨��
#define MAX_COUNT					13									//�����Ŀ

//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����

//////////////////////////////////////////////////////////////////////////

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

	//���ͺ���
public:
	//�Ƿ����
	bool IsDouble(BYTE bCardData[], BYTE bCardCount);
	//�Ƿ���֧
	bool IsTieZhi(BYTE bCardData[], BYTE bCardCount);
	//�Ƿ��«
	bool IsHuLu(BYTE bCardData[], BYTE bCardCount);
	//�Ƿ�ͬ��
	bool IsTongHua(BYTE bCardData[], BYTE bCardCount);
	//�Ƿ�˳��
	bool IsShunZhi(BYTE bCardData[], BYTE bCardCount);
	//�Ƿ�����
	bool IsSanTiao(BYTE bCardData[], BYTE bCardCount);
	//�Ƿ�ͬ��˳
	bool IsTongHuaShun(BYTE bCardData[], BYTE bCardCount);

	//���ܺ���
public:
	//�Ա��˿�
	bool CompareCard(BYTE bFirstCardData, BYTE bNextCardData);
	//�Ա��˿�
	bool CompareCard(BYTE bFirstList[], BYTE bNextList[], BYTE bFirstCount, BYTE bNextCount);
	//�����ж�
	bool SearchOutCard(BYTE bCardData[], BYTE bCardCount, BYTE bTurnCardData[], BYTE bTurnCardCount, BYTE bTurnOutType, BYTE cbOutCard[MAX_COUNT]);
};

//////////////////////////////////////////////////////////////////////////

#endif