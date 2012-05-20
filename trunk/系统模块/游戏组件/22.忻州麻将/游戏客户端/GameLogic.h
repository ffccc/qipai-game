#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//��������

#define MAX_WEAVE					4									//������
#define MAX_INDEX					34									//�������
#define MAX_COUNT					14									//�����Ŀ
#define MAX_REPERTORY				136  //108									//�����

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
#define WIK_FILL					0x10								//��������
#define WIK_GANG					0x20								//��������
#define WIK_CHI_HU					0x40								//�Ժ�����

//////////////////////////////////////////////////////////////////////////
//���ƶ���

//С������
#define CHK_NULL					0x0000								//�Ǻ�����
#define CHK_JI_HU					0x0001								//��������
#define CHK_PING_HU					0x0002								//ƽ������

//�������
/*
#define CHK_SIXI_HU					0x0010								//��ϲ����
#define CHK_BANBAN_HU				0x0020								//������
#define CHK_LIULIU_HU				0x0040								//����˳��
#define CHK_QUEYISE_HU				0x0080								//ȱһɫ��
*/

//�������
#define CHK_PENG_PENG				0x0100								//��������
//#define CHK_JIANG_JIANG				0x0200								//��������
#define CHK_QI_XIAO_DUI				0x0400								//��С����
#define CHK_HAO_HUA_DUI				0x0400								//��������

//�轫Ȩλ
#define CHR_DI						0x0001								//�غ�Ȩλ
#define CHR_TIAN					0x0002								//���Ȩλ
#define CHR_HAI_DI					0x0004								//����Ȩλ
#define CHR_QIANG_GANG				0x0008								//����Ȩλ

//�ҽ�Ȩλ
#define CHR_QING_YI_SE				0x0100								//��ɫȨλ
#define CHR_QUAN_QIU_REN			0x0200								//ȫ��Ȩλ

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

#define ONE_TYPE_NUM	9
#define MJ_KIND_MAX	4 //TONG,TIAO,WAN,ZI
#define MJ_NUM_MAX 4 //1,2,3,4
//�����б�
//˫���б�
//�����б�
//4���б�
//1----->9  0=��
typedef struct _MJ_LIST_ITEM
{
	BYTE useMjInt;//
	BYTE	mjList[ONE_TYPE_NUM];//
}
MJ_LIST_ITEM;

typedef struct _CARD_LIST
{
	int cardType;//1,2,3,4
	MJ_LIST_ITEM    mjCardListAll;//���е�����
	MJ_LIST_ITEM	mjCardList1;//
	MJ_LIST_ITEM	mjCardList2;//
	MJ_LIST_ITEM	mjCardList3;//
	MJ_LIST_ITEM	mjCardList4;//
}
CARD_LIST;
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
	//�Ժ��ж�
//	BYTE EstimateChiHu(BYTE cbCardIndex[MAX_INDEX]);
	//�����ж�
//	BYTE EstimateEatCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);
	//�����ж�
	BYTE EstimatePengCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);
	//�����ж�
	BYTE EstimateGangCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);

	//�����ж�
public:
	//���Ʒ���
	BYTE AnalyseGangCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount, tagGangCardResult & GangCardResult);
	//�Ժ�����
//	BYTE AnalyseChiHuCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount, BYTE cbCurrentCard, WORD wChiHuRight, tagChiHuResult & ChiHuResult);

	//�������
public:
	//��ϲ����
//	bool IsSiXi(BYTE cbCardIndex[MAX_INDEX]);
	//ȱһɫ��
	bool IsQueYiSe(BYTE cbCardIndex[MAX_INDEX]);
	//������
//	bool IsBanBanHu(BYTE cbCardIndex[MAX_INDEX]);
	//����˳��
//	bool IsLiuLiuShun(BYTE cbCardIndex[MAX_INDEX]);

	//�������
public:
	//��һɫ��
	bool IsQingYiSe(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount);
	//��С����
	bool IsQiXiaoDui(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount);
	//��������
	bool IsHaoHuaDui(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount);
	//��������
	bool IsJiangJiangHu(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount);

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
};

//////////////////////////////////////////////////////////////////////////

#endif