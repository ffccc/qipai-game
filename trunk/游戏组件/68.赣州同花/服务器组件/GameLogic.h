#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//��������
#define ST_ORDER					0									//��С����
#define ST_COUNT					1									//��Ŀ����
#define ST_VALUE					2									//��ֵ����
#define ST_COLOR					3									//��ɫ����
#define ST_CUSTOM					4									//�Զ�����

//////////////////////////////////////////////////////////////////////////

//�����ṹ
struct tagAnalyseResult
{
	BYTE 							cbBlockCount[10];					//�˿���Ŀ
	BYTE							cbCardData[10][MAX_COUNT];			//�˿�����
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

struct tagMagicResult
{
	BYTE							cbMagicCardCount;					//ħ������Ŀ
	BYTE							cbNormalCardCount;					//��������Ŀ
	BYTE							cbMagicCard[2];						//ħ��������
	BYTE							cbNormalCard[MAX_COUNT];			//����������
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼���
class CGameLogic
{
	//�߼�����
protected:
	BYTE							m_cbMainValue;						//������ֵ

	//��������
protected:
	static const BYTE				m_cbCardData[FULL_COUNT];			//�˿�����

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//��Ϣ����
public:
	//������ֵ
	BYTE GetMainValue() { return m_cbMainValue; }
	//������ֵ
	VOID SetMainValue(BYTE cbMainValue) { m_cbMainValue=cbMainValue; }

	//���Ժ���
public:
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&MASK_VALUE; }
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&MASK_COLOR; }

	//���ͺ���
public:
	//��ȡ����
	BYTE GetCardType(const BYTE cbCardData[], BYTE cbCardCount);
	//��ȡ��ɫ
	BYTE GetCardColor(const BYTE cbCardData[], BYTE cbCardCount);
	//��ȡ����
	WORD GetCardScore(const BYTE cbCardData[], BYTE cbCardCount);

	//���ƺ���
public:
	//�����˿�
	VOID RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//�����˿�
	VOID SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType);
	//ɾ���˿�
	bool RemoveCard(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount);

	//�߼�����
public:
	//�߼���ֵ
	BYTE GetCardLogicValue(BYTE cbCardData);
	//�Ա��˿�
	bool CompareCard(const BYTE cbFirstCard[], const BYTE cbNextCard[], BYTE cbFirstCount, BYTE cbNextCount);
	//�Ա��˿�
	bool CompareCard( const BYTE cbFirstCard,const BYTE cbNextCard );

	//���ܺ���
public:
	//�����˿�
	VOID AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult);
	//�����ֲ�
	VOID AnalysebDistributing(const BYTE cbCardData[], BYTE cbCardCount, tagDistributing & Distributing);
	//����˿�
	bool MagicCardData(BYTE cbCardData[], BYTE cbCardCount, BYTE cbResultCard[MAX_COUNT], BYTE cbCardType);
	//��������
	bool SearchOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount, tagOutCardResult & OutCardResult);

	//�ڲ�����
private:
	//�����˿�
	BYTE MakeCardData(BYTE cbValueIndex, BYTE cbColorIndex);
	//�Ƿ�����
	bool IsStructureLink(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbCellCount);
	//�Ա��˿�
	bool CompareCardByValue(const BYTE cbFirstCard[], const BYTE cbNextCard[], BYTE cbFirstCount, BYTE cbNextCount);

	//��������
protected:
	//��������
	bool SearchDong( const BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE cbReferCard,tagOutCardResult &OutCardResult );
	//����ͬ��˳
	bool SearchTongHuaShun( const BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE cbReferCard,tagOutCardResult &OutCardResult );
	//��������
	bool SearchLinkCard( const BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE cbReferCard,BYTE cbCardType,tagOutCardResult &OutCardResult ,BYTE TurnCardCount);
	//������«
	bool SearchHuLu( const BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE cbReferCard,tagOutCardResult &OutCardResult ,BYTE TurnCardCount );
	//��������
	bool SearchHuDie( const BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE cbReferCard,tagOutCardResult &OutCardResult ,BYTE TurnCardCount );
	//��������
	bool SearchTianWang( const BYTE cbHandCardData[],BYTE cbHandCardCount,tagOutCardResult &OutCardResult );
	//����ɫ�����˿�
	//VOID AnalyseColorData( const BYTE cbCardData[],BYTE cbCardCount,tagColorResult &ColorResult );
	//������Ʒ����˿�
	VOID AnalyseMagicData( const BYTE cbHandCardData[],BYTE cbHandCardCount,tagMagicResult &MagicResult );
	//�����������
	bool SearchMagicOutCard( const BYTE cbHandCardData[],BYTE cbHandCardCount,const BYTE cbTurnCardData[],BYTE cbTurnCardCount,tagOutCardResult &OutCardResult );
	//���������ͬ��
	bool SearchMagicSameCard( const BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE cbReferCard,BYTE cbLessSameCount,tagOutCardResult &OutCardResult );
	//�����������
	bool SearchMagicLink( const BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE cbReferCard,BYTE cbCardType,tagOutCardResult &OutCardResult );
	//�������ͬ��˳
	bool SearchMagicTongHuaShun( const BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE cbReferCard,tagOutCardResult &OutCardResult );
	//���������«
	bool SearchMagicHuLu( const BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE cbReferCard,tagOutCardResult &OutCardResult );
	
};

//////////////////////////////////////////////////////////////////////////

#endif