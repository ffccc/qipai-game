#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//��������

#define MAX_WEAVE					4									//������
#define MAX_INDEX					34									//�������			
#define MAX_REPERTORY				136									//�����

//////////////////////////////////////////////////////////////////////////
//�߼�����

#define	MASK_COLOR					0xF0								//��ɫ����
#define	MASK_VALUE					0x0F								//��ֵ����

//////////////////////////////////////////////////////////////////////////
//��������

//������־
#define WIK_NULL					0x0000								//û������
#define WIK_LEFT					0x0001								//�������
#define WIK_CENTER					0x0002								//�г�����
#define WIK_RIGHT					0x0004								//�ҳ�����
#define WIK_PENG					0x0008								//��������
#define WIK_GANG					0x0010								//��������
#define WIK_CHI_HU					0x0020								//�Ժ�����
#define WIK_DNBL					0x0040								//���ϱ���
#define WIK_DNBC					0x0080								//���ϱ���
#define WIK_DNBR					0x0100								//���ϱ���
#define WIK_DXBL					0x0200								//��������	
#define WIK_DXBC					0x0400								//��������	
#define WIK_DXBR					0x0800								//��������	
//////////////////////////////////////////////////////////////////////////
//���ƶ���

//�Ǻ�����
#define CHK_NULL					0x0000								//�Ǻ�����

//��������
#define CHK_JI_HU					0x0001								//������
#define CHK_QI_DUI					0x0002								//�߶�
#define CHK_THIRTEEN				0x0004								//13��
#define CHK_SERVEN					0x0008								//����
#define CHK_GERMAN_SERVEN			0x0010								//�¹�����
#define CHK_PENG_PENG				0x0020								//���߶ԣ���������
//����Ȩλ
#define CHR_ZI_MO					0x0001								//����Ȩλ
#define CHR_QIANG_GANG				0x0002								//����Ȩλ
#define CHR_GANG_FLOWER				0x0004								//���Ͽ���
#define CHR_KING_WAIT				0x0008								//����Ȩλ
#define CHR_GERMAN					0x0010								//�¹�Ȩλ
#define CHR_GERMAN_GERMAN			0x0020								//���е�	
#define CHR_TIAN					0x0040								//���Ȩλ	
#define CHR_DI						0x0080								//�غ�Ȩλ
//////////////////////////////////////////////////////////////////////////

//��������
struct tagKindItem
{
	WORD							wWeaveKind;							//�������
	BYTE							cbCenterCard;						//�����˿�
	BYTE							cbCardIndex[3];						//�˿�����
};

//�������
struct tagWeaveItem
{
	WORD							wWeaveKind;						//�������
	BYTE							cbCenterCard;						//�����˿�
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
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbCardData[4];						//�˿�����
};

//��������
struct tagAnalyseItem
{
	BYTE							cbEyeKingReplace;					//�����滻
	BYTE							cbWeaveKingReplace;					//�����滻	
	BYTE							cbCardEye;							//�����˿�
	WORD							wWeaveKind[MAX_WEAVE];				//�������
	BYTE							cbCenterCard[MAX_WEAVE];			//�����˿�
};

//////////////////////////////////////////////////////////////////////////

//����˵��
typedef CArrayTemplate<tagAnalyseItem,tagAnalyseItem &> CAnalyseItemArray;


//��Ϸ�߼���
class CGameLogic
{
	//��������
protected:
	static const BYTE				m_cbCardDataArray[MAX_REPERTORY];	//�˿�����
	BYTE							m_cbKingCardIndex[2];				//����

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
	//���þ���
	void SetKingCardIndex(BYTE cbKingCardIndex[2]);
	//��ȡ����
	BYTE *GetKingCardIndex(BYTE cbKingCardIndex[2]);

	//��������
public:
	//�ж�����
	bool IsKingCardData(BYTE cbCardData);
	//�ж�����
	bool IsKingCardIndex(BYTE cbCardIndex);
	//�жϸ���
	bool IsViceKingCardData(BYTE cbCardData);
	//�жϸ���
	bool IsViceKingCardIndex(BYTE cbCardIndex);
	//��Ч�ж�
	bool IsValidCard(BYTE cbCardData);
	//�˿���Ŀ
	BYTE GetCardCount(BYTE cbCardIndex[MAX_INDEX]);
	//����˿�
	BYTE GetWeaveCard(WORD wWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[4]);
	//�Ƿ񾫵�
	bool IsKingWait(BYTE cbCardIndex[MAX_INDEX],tagWeaveItem WeaveItem[], BYTE cbWeaveCount,BYTE cbCurrentCard,bool bZimo);

	//�ȼ�����
public:
	//�����ȼ�
	BYTE GetUserActionRank(WORD wUserAction);
	//���Ƶȼ�
	BYTE GetChiHuActionRank(tagChiHuResult & ChiHuResult);

	//�����ж�
public:
	//�����ж�
	WORD EstimateEatCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);
	//�����ж�
	WORD EstimatePengCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);
	//�����ж�
	WORD EstimateGangCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);

	//�����ж�
public:
	//���Ʒ���
	WORD AnalyseGangCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount, tagGangCardResult & GangCardResult);
	//�Ժ�����
	WORD AnalyseChiHuCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, WORD wChiHuRight, tagChiHuResult & ChiHuResult,bool bZimo=false);

	//ת������
public:
	//�˿�ת��
	BYTE SwitchToCardData(BYTE cbCardIndex);
	//�˿�ת��
	BYTE SwitchToCardIndex(BYTE cbCardData);
	//�˿�ת��
	BYTE SwitchToCardData(BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT],BYTE bMaxCount);
	//�˿�ת��
	BYTE SwitchToCardIndex(BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardIndex[MAX_INDEX]);
	//�������
	LONG CalScore(tagChiHuResult & ChihuResult,LONG &lScore,LONG &lTimes);
	//���㸱��
	BYTE GetKingFromBrother(BYTE cbBrotherData);

	//�ڲ�����
private:
	//�����˿�
	bool AnalyseCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount, CAnalyseItemArray & AnalyseItemArray,BYTE cbCurrentCard,bool bZimo);
	//7��
	bool ServenStar(BYTE cbCardIndex[MAX_INDEX],BYTE &cbGerman,BYTE cbCurrentCard,bool bZimo);
	//��С����
	bool IsQiXiaoDui(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount,const BYTE cbCurrentCard,BYTE &cbGerman,bool bZimo);
	//3˳ 3��
	bool IsNeat3(const BYTE cbCardIndex[MAX_INDEX],BYTE &cbGerman);
	// ���� ��������
	bool IsNeat2(BYTE cbCardIndex[MAX_INDEX],tagWeaveItem WeaveItem[], BYTE cbWeaveCount);
	//ȫ����
	bool IsNeatAlone(BYTE cbCardIndex[MAX_INDEX],BYTE &cbGerman,BYTE cbCurrentCard,bool bZimo);

};

//////////////////////////////////////////////////////////////////////////

#endif