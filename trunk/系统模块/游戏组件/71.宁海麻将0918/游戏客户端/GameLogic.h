#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//��������

#define MAX_WEAVE					4									//������
#define MAX_INDEX					42									//�������
#define MAX_COUNT					14									//�����Ŀ
#define MAX_REPERTORY				144									//�����

//////////////////////////////////////////////////////////////////////////
//�߼�����

#define	MASK_COLOR					0xF0								//��ɫ����
#define	MASK_VALUE					0x0F								//��ֵ����

//////////////////////////////////////////////////////////////////////////
//��������

//������־
#define WIK_NULL					0x00								//û������
#define WIK_LEFT					0x01								//�������
#define WIK_CENTER					0x02								//�г�����
#define WIK_RIGHT					0x04								//�ҳ�����
#define WIK_PENG					0x08								//��������
#define WIK_GANG					0x10								//��������
#define WIK_CHI_HU					0x20								//�Ժ�����


//////////////////////////////////////////////////////////////////////////
//���ƶ���

//�Ǻ�����
#define CHK_NULL					0x0000								//�Ǻ�����

//��������
#define CHK_JI_HU					0x0001								//��������

//����Ȩλ
#define CHR_NULL					0x0000								//��Ȩλ
#define CHR_HUN_YI_SE				0x0001								//��һɫ
#define CHR_ZI_YI_SE				0x0002								//��һɫ	
#define CHR_PENG_PENG				0x0004								//������	
#define CHR_ZI_MO					0x0008								//����	
#define CHR_QING_YI_SE				0x0010								//��ɫȨλ

//��������  ��̨
const BYTE KIND_JI_HU				= 0;								//����

//����Ȩλ ��̨
const BYTE RIGHT_HUN_YI_SE			= 2;								//��һɫ
const BYTE RIGHT_ZI_YI_SE			= 3;								//��һɫ
const BYTE RIGHT_PENG_PENG			= 2;								//������
const BYTE RIGHT_QING_YI_SE			= 4;								//��һɫ
//////////////////////////////////////////////////////////////////////////

//��������
struct tagKindItem
{
	BYTE							cbWeaveKind;						//�������
	BYTE							cbCenterCard;						//�����˿�
	BYTE							cbCardIndex[3];						//�˿�����
};

//�������
struct tagWeaveItem
{
	BYTE							cbWeaveKind;						//�������
	BYTE							cbCenterCard;						//�����˿�
	BYTE							cbPublicCard;						//������־
	WORD							wProvideUser;						//��Ӧ�û�
};

//���ƽ��
struct tagChiHuResult
{
	WORD							wChiHuKind;							//�Ժ�����
	WORD							wChiHuRight;						//����Ȩλ
	BYTE							cbHuCount;							//���ƺ���	
	BYTE							cbHuTimes;							//���Ʒ���
};

//���ƽ��
struct tagGangCardResult
{
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbCardData[4];						//�˿�����
};

//��������
struct tagAnalyseItem
{
	BYTE							cbCardEye;							//�����˿�
	BYTE							cbWeaveKind[4];						//�������
	BYTE							cbCenterCard[4];					//�����˿�
	BYTE							cbCardData[4][4];					//�˿�����
};


//////////////////////////////////////////////////////////////////////////
//���������

//���ƽ��
struct tagOutCardResult
{
	BYTE							cbOperateCode;						//���ƴ���
	BYTE							cbOperateCard;						//����ֵ
};

//���Ʒ������
struct tagTingCardResult
{
	BYTE bAbandonCount;													//�ɶ�������
	BYTE bAbandonCard[MAX_COUNT];										//�ɶ�����
	BYTE bTingCardCount[MAX_COUNT];										//������Ŀ
	BYTE bTingCard[MAX_COUNT][MAX_COUNT-1];								//����
	BYTE bRemainCount[MAX_COUNT];										//�����Ƶ�ʣ����
};

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

//����˵��
typedef CArrayTemplate<tagAnalyseItem,tagAnalyseItem &> CAnalyseItemArray;

//��Ϸ�߼���
class CGameLogic
{
	//��������
protected:
	static const BYTE				m_cbCardDataArray[MAX_REPERTORY];	//�˿�����

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//���ƺ���
public:
	//�����˿�
	void RandCardData(BYTE cbCardData[], BYTE cbMaxCount);
	//ɾ���˿�
	bool RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard);
	//ɾ���˿�
	bool RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard[], BYTE cbRemoveCount);
	//ɾ���˿�
	bool RemoveCard(BYTE cbCardData[], BYTE cbCardCount, BYTE cbRemoveCard[], BYTE cbRemoveCount);
	//����,������ֵ����
	bool SortCardList( BYTE cbCardData[MAX_COUNT], BYTE cbCardCount );
	//��������
public:
	//��Ч�ж�
	bool IsValidCard(BYTE cbCardData);
	//�˿���Ŀ
	BYTE GetCardCount(BYTE cbCardIndex[MAX_INDEX]);
	//����˿�
	BYTE GetWeaveCard(BYTE cbWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[4]);

	//�ȼ�����
public:
	//�����ȼ�
	BYTE GetUserActionRank(BYTE cbUserAction);
	//���Ƶȼ�
	BYTE GetChiHuActionRank(tagChiHuResult & ChiHuResult);

	//�����ж�
public:
	//�����ж�
	BYTE EstimateEatCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);
	//�����ж�
	BYTE EstimatePengCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);
	//�����ж�
	BYTE EstimateGangCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);

	//�����ж�
public:
	//���Ʒ���
	BYTE AnalyseGangCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount, tagGangCardResult & GangCardResult);
	//�Ժ�����
	BYTE AnalyseChiHuCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount, BYTE cbCurrentCard, WORD wChiHuRight, tagChiHuResult & ChiHuResult,WORD wBankerUser,bool bJugeHuOnly,bool bZimo = false);

	//�������
public:
	//��һɫ��
	bool IsQingYiSe(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount);
	//��һɫ��
	bool IsHunYiSe(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount);
	//��һɫ��
	bool IsZiYiSe(tagAnalyseItem & AnalyseItem);

	//ת������
public:
	//�˿�ת��
	BYTE SwitchToCardData(BYTE cbCardIndex);
	//�˿�ת��
	BYTE SwitchToCardIndex(BYTE cbCardData);
	//�˿�ת��
	BYTE SwitchToCardData(BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT]);
	//�˿�ת��
	BYTE SwitchToCardIndex(BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardIndex[MAX_INDEX]);
	//�˿�ת��
	BYTE SwitchToCardData(BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT],BYTE bMaxCount);
	
	//��̨����
public:
	//�������
	LONG  CalScore(tagChiHuResult & ChiHuResult,LONG &lTimes);


	//�ڲ�����
private:
	//�����˿�
	bool AnalyseCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount, CAnalyseItemArray & AnalyseItemArray);

	//���������
protected:
	//��������
	BYTE AnalyseSingleCount( BYTE cbCardIndex[MAX_INDEX] );
	//����������
	bool AnalyseTrushCard( BYTE cbCardIndex[MAX_INDEX],BYTE bTrushCardIndex[MAX_INDEX] );
public:
	//���Ʒ���
	bool AnalyseTingCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount, WORD wChiHuRight);
	//���ط�������
	bool SearchTingCard(WORD wMeChairId,BYTE (&cbCardIndex)[GAME_PLAYER][MAX_INDEX],tagWeaveItem (&cWeaveItem)[GAME_PLAYER][4],BYTE cbItemCount[4],
		BYTE (&cbDiscardCard)[GAME_PLAYER][55],BYTE cbDiscardCount[GAME_PLAYER],tagTingCardResult &TingCardResult);
	//��������
	bool SearchOutCard( WORD wMeChairId,WORD wCurrentUser,BYTE (&cbCardIndex)[GAME_PLAYER][MAX_INDEX],tagWeaveItem (&cWeaveItem)[GAME_PLAYER][4],
		BYTE cbItemCount[4],BYTE (&cbDiscardCard)[GAME_PLAYER][55],BYTE cbDiscardCount[GAME_PLAYER],BYTE cbActionMask,BYTE cbActionCard,const BYTE cbEnjoinCard[MAX_COUNT],BYTE cbEnjoinCount,tagOutCardResult &OutCardResult );

};

//////////////////////////////////////////////////////////////////////////

#endif