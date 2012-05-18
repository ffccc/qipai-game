#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once
//////////////////////////////////////////////////////////////////////////

//�˿�����

#define CT_INVALID						0								//��������
#define CT_SINGLE						1								//��������
#define CT_ONE_DOUBLE					2								//ֻ��һ��
#define CT_FIVE_TWO_DOUBLE				3								//��������
#define CT_THREE						4								//��������
#define CT_FIVE_MIXED_FLUSH_FIRST_A		5								//A��ǰ˳��
#define CT_FIVE_MIXED_FLUSH_NO_A		6								//ûA��˳
#define CT_FIVE_MIXED_FLUSH_BACK_A		7								//A�ں�˳��
#define CT_FIVE_FLUSH					8								//ͬ������
#define CT_FIVE_THREE_DEOUBLE			9								//����һ��
#define CT_FIVE_FOUR_ONE				10								//�Ĵ�һ��
#define CT_FIVE_STRAIGHT_FLUSH_FIRST_A	11								//A��ǰͬ��˳
#define CT_FIVE_STRAIGHT_FLUSH			12								//ͬ��˳��

//��ֵ����
#define	LOGIC_MASK_COLOR				0xF0							//��ɫ����
#define	LOGIC_MASK_VALUE				0x0F							//��ֵ����

//�����ṹ
struct tagAnalyseData
{
	BYTE							bOneCount ;								//������Ŀ
	BYTE							bTwoCount ;								//������Ŀ 
	BYTE							bThreeCount ;							//������Ŀ
	BYTE							bFourCount ;							//������Ŀ
	BYTE							bFiveCount ;							//������Ŀ
	BYTE							bOneFirst[13];							//����λ��
	BYTE							bTwoFirst[13];							//����λ��
	BYTE							bThreeFirst[13];						//����λ��
	BYTE							bFourFirst[13];							//����λ��
	bool							bStraight;								//�Ƿ�˳��
};

//�����ṹ
struct tagAnalyseResult
{
	BYTE 							cbFourCount;						//������Ŀ
	BYTE 							cbThreeCount;						//������Ŀ
	BYTE 							cbDoubleCount;						//������Ŀ
	BYTE							cbSignedCount;						//������Ŀ
	BYTE							cbFourCardData[13];					//�����˿�
	BYTE							cbThreeCardData[13];				//�����˿�
	BYTE							cbDoubleCardData[13];				//�����˿�
	BYTE							cbSignedCardData[13];				//�����˿�
};
//////////////////////////////////////////////////////////////////////////
//��������
enum enSortCardType
{
	enDescend ,																//�������� 
	enAscend ,																//��������
	enColor																	//��ɫ����
};


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
	BYTE GetCardValue(BYTE bCardData) { return bCardData&LOGIC_MASK_VALUE; } //ʮ������ǰ����λ��ʾ�Ƶ���ֵ
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE bCardData) { return (bCardData&LOGIC_MASK_COLOR)>>4; } //ʮ�����ƺ�����λ��ʾ�ƵĻ�ɫ 

	//���ƺ���
public:
	//�����˿�
	void SortCardList(BYTE bCardData[], BYTE bCardCount,enSortCardType SortCardType=enDescend);
	//�����˿�
	void RandCardList(BYTE bCardBuffer[], BYTE bBufferCount);
	//ɾ���˿�
	bool RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE bCardData[], BYTE bCardCount);

	//�߼�����
public:
	//�߼���ֵ
	BYTE GetCardLogicValue(BYTE bCardData);
	//�Ա��˿�
	bool CompareCard(BYTE bInFirstList[], BYTE bInNextList[], BYTE bFirstCount, BYTE bNextCount , bool bComperWithOther);

	//�ڲ�����

	//�����˿�
	void AnalyseCard(const BYTE bCardDataList[] , const BYTE bCardCount , tagAnalyseData& AnalyseData) ;
	//�����˿�
	void AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult) ;
};

//////////////////////////////////////////////////////////////////////////

#endif
