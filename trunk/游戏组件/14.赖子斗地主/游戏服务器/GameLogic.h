#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////

//�˿�����
#define CT_INVALID					0									//��������
#define CT_SINGLE					1									//��������
#define CT_DOUBLE					2									//��������
#define CT_THREE					3									//��������
#define CT_ONE_LINE					4									//��������
#define CT_DOUBLE_LINE				5									//��������
#define CT_THREE_LINE				6									//��������
#define CT_THREE_LINE_TAKE_ONE		7									//����һ��
#define CT_THREE_LINE_TAKE_DOUBLE	8									//����һ��
#define CT_FOUR_LINE_TAKE_ONE		9									//�Ĵ�����
#define CT_FOUR_LINE_TAKE_DOUBLE	10									//�Ĵ�����
#define CT_BOMB_SOFT				11									//��ը
#define CT_BOMB_CARD				12									//ը������
#define CT_MISSILE_CARD				13									//�������

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
	BYTE 							bThreeLogicVolue[7];				//�����б�
	BYTE 							bDoubleLogicVolue[10];				//�����б�
	BYTE							m_bSCardData[21];					//�����˿�
	BYTE							m_bDCardData[21];					//�����˿�
	BYTE							m_bTCardData[21];					//�����˿�
	BYTE							m_bFCardData[21];					//�����˿�
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼���
class CGameLogic
{
	//��������
protected:
	static const BYTE				m_bCardListData[55];				//�˿�����

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//���ͺ���
public:
	//��ȡ����
	BYTE GetCardType(const BYTE bCardData[], BYTE bCardCount);
	//��ȡ����(������)
	BYTE GetMagicCardType(const BYTE bCardData[], BYTE bCardCount);
	//��ȡ����(������)
	BYTE GetMagicCardType(const BYTE bCardData[], BYTE bCardCount,BYTE *bMagicCard);
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
	//�Ƿ�������
	bool IsHadRoguishCard(const BYTE bCardData[],BYTE bCardCount);
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