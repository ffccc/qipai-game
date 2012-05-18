#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
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
#define WIK_LISTEN					0x20								//��������
#define WIK_CHI_HU					0x40								//�Ժ�����

//////////////////////////////////////////////////////////////////////////
//���ƶ���

//��������
#define CHK_MASK_SMALL				0x0000FFFF							//С������
#define CHK_MASK_GREAT				0xFFFF0000							//�������

//С������
#define CHK_NULL					0x00000000							//�Ǻ�����
#define CHK_JI_HU					0x00000001							//��������
#define CHK_PING_HU					0x00000002							//ƽ������

//�������
#define CHK_PENG_PENG				0x00010000							//��������
#define CHK_QI_XIAO_DUI				0x00020000							//��С����
#define CHK_SHI_SAN_YAO				0x00040000							//ʮ������


//////////////////////////////////////////////////////////////////////////
//����Ȩλ

//��Ȩ����
#define CHR_MASK_SMALL				0x000000FF							//С������
#define CHR_MASK_GREAT				0xFFFFFF00							//�������

//���Ȩλ
#define CHR_DI						0x00000100							//�غ�Ȩλ
#define CHR_TIAN					0x00000200							//���Ȩλ
#define CHR_QING_YI_SE				0x00000400							//��һɫ��
#define CHR_QIANG_GANG				0x00000800							//����Ȩλ
#define CHK_QUAN_QIU_REN			0x00001000							//ȫ��Ȩλ
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
	DWORD							dwChiHuKind;						//�Ժ�����
	DWORD							dwChiHuRight;						//����Ȩλ
	DWORD							dwWinTimes;							//������Ŀ
};

//���ƽ��
struct tagGangCardResult
{
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbCardData[4];						//�˿�����
	BYTE                            cbGangType;							//��������
};

//��������
struct tagAnalyseItem
{
	BYTE							cbCardEye;							//�����˿�
	BYTE							cbWeaveKind[4];						//�������
	BYTE							cbCenterCard[4];					//�����˿�
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
	WORD GetChiHuActionRank(tagChiHuResult & ChiHuResult);

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
	BYTE AnalyseTingCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount, DWORD dwChiHuRight);
	//���Ʒ���
	BYTE AnalyseGangCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount, tagGangCardResult & GangCardResult);
	//�Ժ�����
	BYTE AnalyseChiHuCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount, BYTE cbCurrentCard, DWORD dwChiHuRight, tagChiHuResult & ChiHuResult);

	//�������
public:
	//��һɫ��
	bool IsQingYiSe(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount);
	//��С����
	bool IsQiXiaoDui(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount);
	//ʮ��ز��
	bool IsShiSanYao(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount);

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

	//�ڲ�����
private:
	//�����˿�
	bool AnalyseCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount, CAnalyseItemArray & AnalyseItemArray);
};

//////////////////////////////////////////////////////////////////////////

#endif