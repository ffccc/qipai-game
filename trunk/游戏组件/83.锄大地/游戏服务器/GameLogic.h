#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//�˿�����
#define CT_ERROR					0									//��������
#define CT_SINGLE					1									//��������
#define CT_DOUBLE					2									//��������
#define CT_THREE_TIAO				3									//��������
#define	CT_FOUR_TIAO				4									//��������
#define	CT_SHUN_ZI					5									//˳������
#define CT_TONG_HUA					6									//ͬ������
#define CT_HU_LU					7									//��«����
#define CT_TIE_ZHI					8									//��֧����
#define CT_TONG_HUA_SHUN			9									//ͬ��˳��

//�궨��
#define MAX_COUNT					13									//�����Ŀ
#define FULL_COUNT					52									//������
#define BIG_COUNT					24									//

//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����


//��������
#define ST_ORDER					0									//��С����
#define ST_COLOR					1									//��ɫ����

//////////////////////////////////////////////////////////////////////////

//�����ṹ
struct tagAnalyseResult
{
	BYTE 							cbCardCount[4];						//�˿���Ŀ
	BYTE							cbCardData[4][MAX_COUNT];			//�˿�����
};

//���ƽ��
struct tagOutCardResult
{
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbResultCard[5];					//����˿�
};

//�ֲ���Ϣ
struct tagDistributing
{
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbDistributing[15][6];				//�ֲ���Ϣ
};

//������
struct tagAnalysedCard
{
	BYTE							bCardCount[9];						//������������Ŀ
	BYTE							bCardData[9][MAX_COUNT];			//��������������
};

struct tagOutCardType
{
	bool							bPower;								//�Ƿ�ɳ�������Ͳ��ջس���Ȩ
	bool							bOutAll;							//�Ƿ������
	BYTE							bRemainCount;						//ʣ����Ŀ
	BYTE							bCardCount;							//
	BYTE							bCardData[5];						//
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼���
class CGameLogic
{
	//��������
public:
	static const BYTE				m_cbCardData[FULL_COUNT];			//�˿�����
	static const BYTE				m_cbBigCardData[BIG_COUNT];			//

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();


	//���Ժ���
public:
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&LOGIC_MASK_COLOR; }

	//���ͺ���
public:
	//��ȡ����
	BYTE GetCardType(const BYTE cbCardData[], BYTE cbCardCount);
	//��ȡ��ɫ
	BYTE GetCardColor(const BYTE cbCardData[], BYTE cbCardCount);

	//���ƺ���
public:
	//�����˿�
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//�����˿�
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType=ST_ORDER);
	//ɾ���˿�
	bool RemoveCard(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount);
	//�����˿�
	void RandCard(const BYTE bCardData[],BYTE bCardCount,BYTE bCardBuffer[],BYTE bBufferCount);

	//�߼�����
public:
	//��Ч�ж�
	bool IsValidCard(BYTE cbCardData);
	//�߼���ֵ
	BYTE GetCardLogicValue(BYTE cbCardData);
	//�Ա��˿�
	bool CompareCard(BYTE cbFirstCard, BYTE cbNextCard);
	//�Ա��˿�
	bool CompareCard(const BYTE cbFirstCard[], const BYTE cbNextCard[], BYTE cbFirstCount, BYTE cbNextCount);
	//��������
	bool SearchOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount, tagOutCardResult & OutCardResult);

	//��������
protected:
	//������«
	BYTE SearchHuLu(const tagDistributing & Distributing, BYTE cbReferCard, BYTE cbResultCard[MAX_COUNT]);
	//������֧
	BYTE SearchTieZhi(const tagDistributing & Distributing, BYTE cbReferCard, BYTE cbResultCard[MAX_COUNT]);
	//����˳��
	BYTE SearchShunZi(const tagDistributing & Distributing, BYTE cbReferCard, BYTE cbResultCard[MAX_COUNT]);
	//����ͬ��
	BYTE SearchTongHua(const tagDistributing & Distributing, BYTE cbReferCard, BYTE cbResultCard[MAX_COUNT]);
	//����ͬ��˳
	BYTE SearchTongHuaShun(const tagDistributing & Distributing, BYTE cbReferCard, BYTE cbResultCard[MAX_COUNT]);

	//�ڲ�����
private:
	//�����˿�
	BYTE MakeCardData(BYTE cbValueIndex, BYTE cbColorIndex);
	//�Ƿ�����
	bool IsStructureLink(const BYTE cbCardData[], BYTE cbCardCount);
	//�����˿�
	void AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult);
	//�����ֲ�
	void AnalysebDistributing(const BYTE cbCardData[], BYTE cbCardCount, tagDistributing & Distributing);

public:
	//��������
	bool StrusteeOutCard( const BYTE cbHandCardData[],BYTE cbHandCardCount,const BYTE cbTurnCardData[],BYTE cbTurnCardCount,
		bool bOutCard3,bool bOutMost,tagOutCardResult &OutCardResult );

public:
	//�����˿�
	void AnalyseCardData( const BYTE cbCardData[],BYTE cbCardCount,tagAnalysedCard & AnalysedCard );
	//��������
	bool SearchFromAnalysedCard( const tagAnalysedCard& AnalysedCard,WORD wChairId,const BYTE bTurnCardData[], BYTE bTurnCardCount,const BYTE bOutCardRecord[],BYTE bOutCardCount,
		const BYTE (&cbCardData)[GAME_PLAYER][13],const BYTE cbCardCount[],tagOutCardResult & OutCardResult,bool &bSplit );
	//���ȳ�������
	bool SearchFirstOutCard( const tagAnalysedCard& AnalysedCard,tagOutCardResult &OutCardResult,bool &bSplit );
	//ǿ�Ƴ����������
	bool SearchForcedOutCard( const tagAnalysedCard& AnalysedCard,WORD wChairId,const BYTE bTurnCardData[],BYTE bTurnCardCount,const BYTE bOutCardRecord[],BYTE bOutCardCount,
		const BYTE (&cbCardData)[GAME_PLAYER][13],const BYTE cbCardCount[],tagOutCardResult& OutCardResult,bool &bReAnalyse );
	//ץ��
	bool HuntUser( const tagAnalysedCard &AnalysedCard,BYTE bHandCardData[],BYTE bHandCardCount,BYTE bNextUserCardCount,tagOutCardResult &OutCardResult,bool &bSplit );
	//�Զ�����
	bool AutomatismPlay( tagAnalysedCard &AnalysedCard,WORD wChairId,WORD wOutCardUser,const BYTE bTurnCardData[],BYTE bTurnCardCount,const BYTE (&bCardData)[GAME_PLAYER][13],const BYTE bCardCount[],const BYTE bOutCardRecord[],BYTE bOutCardCount,tagOutCardResult &OutCardResult,bool bOutCard3=false );
	//��ȡ�����
	BYTE GetMostCardValue( const tagAnalysedCard &AnalysedCard,const BYTE bOutCardRecord[],BYTE bOutCardCount );
	//�ж��Ƿ��ܳ���
	bool IsOutCardFinish( WORD wChairId,const tagAnalysedCard &AnalysedCard,const BYTE bOutCardRecord[],BYTE bOutCardCount,bool bTurn,const BYTE (&cbCardData)[GAME_PLAYER][13],const BYTE cbCardCount[],BYTE &bMostCount,BYTE &bLessCount );
	//��������
	bool AnalyseOutCardType( const tagAnalysedCard &cAnalysedCard,WORD wMeChairId,BYTE bCardType,const BYTE cbTurnCardData[],BYTE cbTurnCardCount,
		const BYTE (&cbCardData)[GAME_PLAYER][13],const BYTE cbCardCount[],tagOutCardType &OutCardType );
	//��������
	bool SearchTurnCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount,tagOutCardResult &OutCardResult);
	//�ж��Ƿ����
	bool IsTurnCardMost(WORD wChairId,const BYTE cbTurnCardData[],BYTE cbTurnCardCount,const BYTE (&cbCardData)[GAME_PLAYER][13],const BYTE cbCardCount[]);
};

//////////////////////////////////////////////////////////////////////////

#endif
