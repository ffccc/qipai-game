#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

/*
���ţ�һ���������
���ӣ�����ͬ����С����
����������ͬ����С����
����������ͬ����С����

���ƣ���������˳���������ƣ�3��2��A���������ƣ�
�������ƣ���������˳�������Ķ��ӣ�3��2��A�������������ƣ�

6�����͵ıȽ�
�����Ĵ�С�� 3>2>A>K>Q>J>10>9>8>7>6>5>4
ֻ����ͬ�����Ͳſ��ԱȽϴ�С��
���š����ӡ��������������յ����Ƚϴ�С��
���ơ��������ƱȽ������Ƶĵ�����

*/
//////////////////////////////////////////////////////////////////////////

//�˿�����
#define CT_INVALID					0									//��������

#define CT_SINGLE					1									//�������� ok
#define CT_DOUBLE					2									//��������	ok
#define CT_THREE					3									//��������	ok

#define CT_ONE_LINE					4									//��������	OK abc
#define CT_DOUBLE_LINE				5									//��������	OK aabbcc    6
#define CT_THREE_LINE				6									//��������	OK aaabbbccc 9

//#define CT_THREE_LINE_TAKE_ONE		7									//����һ��
//#define CT_THREE_LINE_TAKE_DOUBLE	8									//����һ��
//#define CT_FOUR_LINE_TAKE_ONE		9									//�Ĵ�����
//#define CT_FOUR_LINE_TAKE_DOUBLE	10									//�Ĵ�����

#define CT_BOMB_CARD				11									//ը������ 4��һ���� ok
//#define CT_MISSILE_CARD				12									//������� ˫��



//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����

//�����ṹ
struct tagAnalyseResult
{
	BYTE 							bFourCount;							//������Ŀ
	BYTE 							bThreeCount;						//������Ŀ
	BYTE 							bDoubleCount;						//������Ŀ
	BYTE							bSignedCount;						//������Ŀ
	BYTE 							bFourLogicVolue[5];					//�����б�
	BYTE 							bThreeLogicVolue[6];				//�����б�
	BYTE 							bDoubleLogicVolue[10];				//�����б�
	BYTE							m_bSCardData[ZHUANG_CARD_NUM /*20*/];					//�����˿�
	BYTE							m_bDCardData[ZHUANG_CARD_NUM /*20*/];					//�����˿�
	BYTE							m_bTCardData[ZHUANG_CARD_NUM /*20*/];					//�����˿�
	BYTE							m_bFCardData[ZHUANG_CARD_NUM /*20*/];					//�����˿�
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼���
class CGameLogic
{
	//��������
protected:
	static const BYTE				m_bCardListData[ALL_CARD_NUM];				//�˿�����

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//���ͺ���
public:
	//�Ƿ��ж���
	//������>= 2
	bool IsHaveDuiCard(const BYTE bCardData[], BYTE bCardCount);

	//�Ƿ�Ϊ����
	//������>= 3
	bool IsDanLianCardType(const BYTE bCardData[], BYTE bCardCount);

	//�Ƿ�Ϊ˫��
	//������>= 6
	bool IsShuangLianCardType(const BYTE bCardData[], BYTE bCardCount);

	//�Ƿ�Ϊ����
	//������>= 9
	bool IsThreeLianCardType(const BYTE bCardData[], BYTE bCardCount);


	//��ȡ����
	BYTE GetCardType(const BYTE bCardData[], BYTE bCardCount);
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
	//�߼���ֵ
	BYTE GetCardLogicValue(BYTE bCardData);
	//�Ա��˿�
	bool CompareCard(const BYTE bFirstList[], const BYTE bNextList[], BYTE bFirstCount, BYTE bNextCount);

	//�ڲ�����
public:
	//�����˿�
	void AnalysebCardData(const BYTE bCardData[], BYTE bCardCount, tagAnalyseResult & AnalyseResult);
};

//////////////////////////////////////////////////////////////////////////

#endif