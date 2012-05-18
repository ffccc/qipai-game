#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//��������

#define MAX_WEAVE					4									//������
#define MAX_INDEX					42									//�������
#define MAX_COUNT					17									//�����Ŀ
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
#define WIK_GANG					0x20								//��������
#define WIK_CHI_HU					0x40								//�Ժ�����

//////////////////////////////////////////////////////////////////////////
//���ƶ���

//С������
#define CHK_NULL					0x0000								//�Ǻ�����
#define CHK_JI_HU					0x0002								//��������
#define CHK_PING_HU					0x0001								//ƽ������

//�������
#define CHK_PENG_PENG				0x0004								//��������
#define	CHR_GANG_HU					0x0008								//�ܺ��Ȩλ

//��������
#define CHK_SKY_SWIM				0x1000								//���κ���
#define CHK_SINGLE_SWIM				0x0100								//���κ���
#define CHK_DOUBLE_SWIM				0x0200								//˫�κ���
#define CHK_THREE_SWIM				0x0400								//���κ���

//����Ȩλ
#define CHR_SINGLE_SWIM				0x2000								//�ν�Ȩλ
#define CHR_DOUBLE_SWIM				0x4000								//˫��Ȩλ
#define CHR_THREE_SWIM				0x8000								//����Ȩλ

//////////////////////////////////////////////////////////////////////////

//��������
struct tagKindItem
{
	BYTE							cbWeaveKind;						//�������
	BYTE							cbCenterCard;						//�����齫
	BYTE							cbCardIndex[3];						//�齫����
};

//�������
struct tagWeaveItem
{
	BYTE							cbWeaveKind;						//�������
	BYTE							cbCenterCard;						//�����齫
	BYTE							cbPublicCard;						//������־
	WORD							wProvideUser;						//��Ӧ�û�
};

//���ƽ��
struct tagChiHuResult
{
	WORD							wChiHuKind;							//�Ժ�����
	WORD							wChiHuRight;						//����Ȩλ
};

//���ƽ��
struct tagGangCardResult
{
	BYTE							cbCardCount;						//�齫��Ŀ
	BYTE							cbCardData[4];						//�齫����
};

//��������
struct tagAnalyseItem
{
	BYTE							cbCardEye;							//�����齫
	BYTE							cbWeaveKind[4];						//�������
	BYTE							cbCenterCard[4];					//�����齫
};

//////////////////////////////////////////////////////////////////////////

//����˵��
typedef CArrayTemplate<tagAnalyseItem,tagAnalyseItem &> CAnalyseItemArray;

//��Ϸ�߼���
class CGameLogic
{
	//��������
protected:
	static const BYTE				m_cbCardDataArray[MAX_REPERTORY];	//�齫����

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//���ƺ���
public:
	//�����齫
	void RandCardData(BYTE cbCardData[], BYTE cbMaxCount);
	//ɾ���齫
	bool RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard);
	//ɾ���齫
	bool RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard[], BYTE cbRemoveCount);
	//ɾ���齫
	bool RemoveCard(BYTE cbCardData[], BYTE cbCardCount, BYTE cbRemoveCard[], BYTE cbRemoveCount);

	//��������
public:
	//��Ч�ж�
	bool IsValidCard(BYTE cbCardData);
	//�齫��Ŀ
	BYTE GetCardCount(BYTE cbCardIndex[MAX_INDEX]);
	//����齫
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
	BYTE AnalyseChiHuCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, WORD wChiHuRight, tagChiHuResult & ChiHuResult,BYTE cbGoldCard[],BYTE cbGoldCount,bool &bGoldCardEye);
	//�Ժ�����
	BYTE AnalyseChiHuLevity(BYTE cbCardIndex[MAX_INDEX],tagWeaveItem WeamItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, WORD wChiHuRight, tagChiHuResult &ChiHuResult,BYTE cbGoldCard[],BYTE cbGoldCount,BYTE cbReCardIndex[MAX_INDEX]);
	//�Ժ�����
	BYTE AnalyseChiHuCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, WORD wChiHuRight, tagChiHuResult & ChiHuResult);
	
public:	
	//������Ŀ
	BYTE GetGoldCardCount(BYTE cbCardIndex[MAX_INDEX],tagWeaveItem WeaveItem[],BYTE cbItemCount,BYTE bGoldCard[],BYTE bGoldCount);
	

	//ת������
public:
	//�齫ת��
	BYTE SwitchToCardData(BYTE cbCardIndex);
	//�齫ת��
	BYTE SwitchToCardIndex(BYTE cbCardData);
	//�齫ת��
	BYTE SwitchToCardData(BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT]);
	//�齫ת��
	BYTE SwitchToCardIndex(BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardIndex[MAX_INDEX]);

	//�ڲ�����
private:	
	//�����齫
	bool AnalyseCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount, CAnalyseItemArray & AnalyseItemArray);
	
public:
	

	//�ν�״̬
	//bool IsGoldSwimSingleStatus(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount,BYTE bGoldCard[],BYTE bGoldCount);
	// ���޻���
	bool IsHaveFlowerCard(BYTE cbCardIndex[MAX_INDEX],BYTE bFlowerCard[],BYTE bFlowerCount);
	//�Ƿ��ǻ���
	bool IsFlowerCard(BYTE cbCardData,BYTE bFlowerCard[],BYTE bFlowerCount);
	//��ȡ����
	BYTE GetFlowerCard(BYTE cbCardIndex[MAX_INDEX],BYTE bFlowerCard[],BYTE bFlowerCount);
	//�����ж�
	bool IsGoldCard(BYTE cbCardData,BYTE bGoldCard[],BYTE bGoldCount);
	//�����ϱ�
	bool IsExistAllSidesCard(BYTE cbCardIndex[MAX_INDEX]);
	//÷�����
	bool IsExistAllPlantCard(BYTE cbCardIndex[MAX_INDEX]);
	//�����ﶬ
	bool IsExistAllSeasonCard(BYTE cbCardIndex[MAX_INDEX]);
};

//////////////////////////////////////////////////////////////////////////

#endif