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

#define WIK_TING_CARD					0x30								//��������

#define WIK_CHI_HU					0x40								//�Ժ�����




//////////////////////////////////////////////////////////////////////////
//���ƶ���

//�Ǻ�����
#define CHK_NULL					0x0000								//�Ǻ�����

//С������
#define CHK_JI_HU					0x0001								//��������
#define CHK_PING_HU					0x0002								//ƽ������

//�������
#define CHR_DI						0x0100								//�غ�Ȩλ
#define CHR_TIAN					0x0200								//���Ȩλ
#define CHK_PENG_PENG				0x0300								//��������
#define CHK_QING_YI_SE				0x0800								//��ɫȨλ
#define CHR_QIANG_GANG				0x2000								//����Ȩλ
#define CHK_QUAN_QIU_REN			0x1000								//ȫ��Ȩλ

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
public:
	//����MJ�ķ���  ����  Ӯ��
	int GetWinMJNum(BYTE cbCardIndex[MAX_INDEX], BYTE cbItemCount);
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
//���룺BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount, BYTE cbCurrentCard, WORD wChiHuRight, tagChiHuResult & ChiHuResult
//���أ�
	BYTE AnalyseChiHuCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount, BYTE cbCurrentCard, WORD wChiHuRight, tagChiHuResult & ChiHuResult);

	//�������
public:
	//�Ƿ�Ϊ����
	//��  �ͣ����磺12��3��89��7��46��5��2��2������С�Բ��㣩
	bool IsKanMJ(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount);

	//��һɫ�� �Ƿ�Ϊ��һɫ
	bool IsQingYiSe(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount);

	//�Ƿ�Ϊһ����
	//�������������������һ�ŵ�123456789����
	bool IsOneLongMJ(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount);

	//��С����
	bool IsQiXiaoDui(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount);

	//ȱһɫ��
	//ȱһ�ţ�����ʱ����û���򣬱���������һ�ţ�ȱ��2�ŵĲ���ȴ��
	bool IsQueYiSe(BYTE cbCardIndex[MAX_INDEX]);

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
	//���룺BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount
	//���أ�CAnalyseItemArray & AnalyseItemArray
	bool AnalyseCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount, CAnalyseItemArray & AnalyseItemArray);
};

//////////////////////////////////////////////////////////////////////////

#endif