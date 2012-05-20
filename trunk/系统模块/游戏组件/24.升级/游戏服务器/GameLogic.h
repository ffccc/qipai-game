#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////
//�궨��

//���Զ���
#define MAX_TRACKOR					18									//�������
#define	COLOR_RIGHT					40									//��ɫȨλ

//��Ч����
#define CT_ERROR					0									//��������
#define VALUE_ERROR					0x00								//��ֵ����
#define COLOR_ERROR					0xFF								//��ɫ����

//��ɫ����
#define COLOR_NT					0x40								//��ɫ����
#define COLOR_HEI_TAO				0x30								//��ɫ����
#define COLOR_HONG_TAO				0x20								//��ɫ����
#define COLOR_MEI_HUA				0x10								//��ɫ����
#define COLOR_FANG_KUAI				0x00								//��ɫ����

//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����

//�˿�����
#define CT_ERROR					0									//��������
#define CT_SINGLE					1									//��������
#define CT_SAME_2					2									//��������
#define CT_SAME_3					3									//��������
#define CT_SAME_4					4									//��������
#define CT_TRACKOR_2				5									//��������
#define CT_TRACKOR_3				6									//��������
#define CT_TRACKOR_4				7									//��������
#define CT_THROW_CARD				8									//˦������

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//������Ϣ
struct tagSameDataInfo
{
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbBlockCount;						//�ƶ���Ŀ
	BYTE							cbSameData[MAX_COUNT];				//��������
};

//��������Ϣ
struct tagTractorDataInfo
{
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbTractorCount;						//���Ƹ���
	BYTE							cbTractorMaxIndex;					//����λ��
	BYTE							cbTractorMaxLength;					//��󳤶�
	BYTE							cbTractorData[MAX_COUNT*3/2];		//��������
};

//���ͽṹ
struct tagAnalyseResult
{
	BYTE							cbCardColor;						//�˿˻�ɫ
	tagSameDataInfo					SameDataInfo[PACK_COUNT];			//ͬ������
	tagTractorDataInfo				TractorDataInfo[PACK_COUNT-1];		//��������
};

//����ṹ
struct tagDemandInfo
{
	BYTE 							cbMaxTractor[PACK_COUNT-1];			//��������
	BYTE							cbSameCardCount[PACK_COUNT];			//ͬ������
	BYTE 							cbTractorCardCount[PACK_COUNT-1];		//��������
};

//���ƽ��
struct tagOutCardResult
{
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbResultCard[MAX_COUNT];			//����˿�
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼���
class CGameLogic
{
	//���Ա���
protected:
	BYTE							m_cbPackCount;						//�Ƹ���Ŀ
	BYTE							m_cbDispatchCount;					//�ɷ���Ŀ

	//�߼�����
protected:
	BYTE							m_cbMainColor;						//���ƻ�ɫ
	BYTE							m_cbMainValue;						//������ֵ

	//��������
protected:
	BYTE							m_cbSortRight[5];					//����Ȩλ

	//��������
public:
	static const BYTE				m_cbNTValue;						//������ֵ
	static const BYTE				m_cbCardData[CELL_PACK];			//�˿�����

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//״̬����
public:
	//���ø���
	bool SetPackCount(BYTE cbPackCount);
	//��ȡ����
	BYTE GetPackCount() { return m_cbPackCount; }
	//�ɷ���Ŀ
	BYTE GetDispatchCount() { return m_cbDispatchCount; }

	//��Ϣ����
public:
	//���ƻ�ɫ
	bool SetMainColor(BYTE cbMainColor);
	//������ֵ
	bool SetMainValue(BYTE cbMainValue);
	//���ƻ�ɫ
	BYTE GetMainColor() { return m_cbMainColor; }
	//������ֵ
	BYTE GetMainValue() { return m_cbMainValue; }

	//���ƺ���
public:
	//�����˿�
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount);
	//�����˿�
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//ɾ���˿�
	bool RemoveCard(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount);

	//���ֺ���
public:
	//��ȡ����
	WORD GetCardScore(const BYTE cbCardData[], BYTE cbCardCount);
	//�����˿�
	BYTE GetScoreCard(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbScoreCard[], BYTE cbMaxCount);

	//���ܺ���
public:
	//�����˿�
	bool AnalyseCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult);
	//�����ж�
	bool SearchOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount,const  BYTE cbTurnCardData[], BYTE cbTurnCardCount, tagOutCardResult & OutCardResult);
	//Ч�����
	bool EfficacyOutCard(const BYTE cbOutCardData[], BYTE cbOutCardCount, const BYTE cbFirstCardData[], BYTE cbFirstCardCount, const BYTE cbHandCardData[], BYTE cbHandCardCount);
	//Ч��˦��
	bool EfficacyThrowCard(const BYTE cbOutCardData[], BYTE cbOutCardCount, WORD wOutCardIndex, const BYTE cbHandCardData[GAME_PLAYER][MAX_COUNT], BYTE cbHandCardCount, tagOutCardResult & OutCardResult);

	//�ԱȺ���
public:
	//�Ա��˿�
	bool CompareCardData(BYTE cbFirstCardData, BYTE cbNextCardData);
	//�Ա��˿�
	bool CompareCardResult(const tagAnalyseResult WinnerResult, const tagAnalyseResult UserResult);
	//�Ա��˿�
	WORD CompareCardArray(const BYTE cbOutCardData[GAME_PLAYER][MAX_COUNT], BYTE cbCardCount, WORD wFirstIndex);

	//���ͺ���
public:
	//��ȡ����
	BYTE GetCardType(const BYTE cbCardData[], BYTE cbCardCount);
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&LOGIC_MASK_COLOR; }

	//�߼�����
public:
	//��Ч�ж�
	bool IsValidCard(BYTE cbCardData);
	//����ȼ�
	BYTE GetCardSortOrder(BYTE cbCardData);
	//�߼���ֵ
	BYTE GetCardLogicValue(BYTE cbCardData);
	//�߼���ɫ
	BYTE GetCardLogicColor(BYTE cbCardData);
	//���л�ɫ
	BYTE GetCardLogicColor(const BYTE cbCardData[], BYTE cbCardCount);

	//�߼�����
protected:
	//�Ƿ�����
	bool IsLineValue(BYTE cbFirstCard, BYTE cbSecondCard);
	//�Ƿ�ͬ��
	bool IsSameColor(BYTE cbFirstCard, BYTE cbSecondCard);
	//������Ŀ
	BYTE GetIntersectionCount(const BYTE cbCardData1[], BYTE cbCardCount1, const BYTE cbCardData2[], BYTE cbCardCount2);

	//��ȡ����
protected:
	//��ȡ�˿�
	BYTE DistillCardByColor(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardColor, BYTE cbResultCard[]);
	//��ȡ�˿�
	BYTE DistillCardByCount(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbSameCount, tagSameDataInfo & SameDataInfo);
	//��ȡ�˿�
	BYTE DistillTrackorByCount(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbSameCount, tagTractorDataInfo & TractorDataInfo);

	//�ڲ�����
protected:
	//����Ȩλ
	void UpdateSortRight();
	//�����˿�
	bool RectifyCardWeave(const BYTE cbCardData[], BYTE cbCardCount, const tagAnalyseResult & TargetResult, tagAnalyseResult & RectifyResult);
};

//////////////////////////////////////////////////////////////////////////

#endif