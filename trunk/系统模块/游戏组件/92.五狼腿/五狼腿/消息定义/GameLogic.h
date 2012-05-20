#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////

//��������
#define ST_ORDER					0
#define ST_VIEW						1
#define ST_BOMB						2

//�˿�����
#define CT_ERROR					0									//��������
#define CT_SINGLE					1									//��������
#define CT_DOUBLE					2									//��������
#define CT_SINGLE_LINE				3									//��������
#define CT_DOUBLE_LINE				4									//��������
#define CT_BOMB						5									//ը������
#define CT_DOUBLE_BOMB				6									//˫ը����
#define CT_CRURA_FAKE				7									//��������
#define CT_KING_LITTET				8									//С������
#define CT_KING_BIG					9									//��������
#define CT_CRURA_PURE				10									//��������

//�궨��
#define MAX_COUNT					27									//�����Ŀ

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
	BYTE 							bFourLogicVolue[6];					//�����б�
	BYTE 							bThreeLogicVolue[9];				//�����б�
	BYTE 							bDoubleLogicVolue[13];				//�����б�
};

//���ƽ��
struct tagOutCardResult
{
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbResultCard[MAX_COUNT];			//����˿�
};

//�ֲ���Ϣ
struct tagDistributing
{
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbDistributing[15][6];				//�ֲ���Ϣ
};


//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼���
class CGameLogic
{
	//��������
protected:
	static const BYTE				m_bCardListData[108];				//�˿�����


	//���ͺ���
public:
	//��ȡ����
	static BYTE GetCardType(BYTE bCardData[], BYTE bCardCount, bool bDoubleBomo);
	//��ȡ��ֵ
	static BYTE GetCardValue(BYTE bCardData) { return bCardData&LOGIC_MASK_VALUE; }
	//��ȡ��ɫ
	static BYTE GetCardColor(BYTE bCardData) { return bCardData&LOGIC_MASK_COLOR; }

	//���ƺ���
public:
	//�����˿�
	static void SortCardList(BYTE bCardData[], BYTE bCardCount, BYTE cbSortType);
	//�����˿�
	static void RandCardList(BYTE bCardBuffer[], BYTE bBufferCount);
	//ɾ���˿�
	static bool RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE bCardData[], BYTE bCardCount);

	//�߼�����
public:
	//��Ч�ж�
	static bool IsValidCard(BYTE cbCardData);
	//�߼���ֵ
	static BYTE GetCardLogicValue(BYTE cbCardData);
	//�����˿�
	static BYTE MakeCardData(BYTE cbValueIndex, BYTE cbColorIndex);
	//��ȡ�ֲ�
	static void GetDistributingInfo(BYTE cbCardData[], BYTE cbCardCount, tagDistributing & Distributing);

	//���ܺ���
public:
	//�Ա��˿�
	static bool CompareCard(BYTE bFirstList[], BYTE bNextList[], BYTE bFirstCount, BYTE bNextCount);
	//�����˿�
	static void AnalysebCardData(const BYTE bCardData[], BYTE bCardCount, tagAnalyseResult & AnalyseResult);
	//�����ж�
	static bool SearchOutCard(BYTE bCardData[], BYTE bCardCount, BYTE bTurnCardData[], BYTE bTurnCardCount, BYTE bTurnOutType, tagOutCardResult & OutCardResult);
	//�õ�ը������
private:
	static void _GetPrivateCard(BYTE bCards[], BYTE& bCardCount, BYTE bPrivateCards[], BYTE& bPrivateCount);
	static void	_DeleteCards(BYTE bCards[], BYTE& bCardCount, BYTE bDelete[], BYTE bDeleteCount);
	static void	_AddCards(BYTE bCards[], BYTE& bCardCount, BYTE bAdd[], BYTE bAddCount);
};

//////////////////////////////////////////////////////////////////////////

#endif