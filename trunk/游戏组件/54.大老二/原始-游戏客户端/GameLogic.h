#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////

//�˿�����
#define CT_ERROR					0									//��������
#define CT_SINGLE					1									//��������
#define CT_DOUBLE					2									//��������
#define CT_THREE_TIAO				3									//��������
#define	CT_SHUN_ZI					4									//˳������
#define CT_TONG_HUA					5									//ͬ������
#define CT_HU_LU					6									//��«����
#define CT_TIE_ZHI					7									//��֧����
#define CT_TONG_HUA_SHUN			8									//ͬ��˳��

//�궨��
#define MAX_COUNT					13									//�����Ŀ

//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����

//�����ṹ
struct tagAnalyseResult
{
	BYTE 							bFourCount;							//������Ŀ
	BYTE 							bThreeCount;						//������Ŀ
	BYTE 							bDoubleCount;						//������Ŀ
	BYTE							bSignedCount;						//������Ŀ
	BYTE 							bFourLogicVolue[5];					//�����б�
	BYTE 							bThreeLogicVolue[6];				//�����б�
	BYTE 							bDoubleLogicVolue[10];				//�����б�
	BYTE							m_bSCardData[13];					//�����˿�
	BYTE							m_bDCardData[13];					//�����˿�
	BYTE							m_bTCardData[13];					//�����˿�
	BYTE							m_bFCardData[13];					//�����˿�
};
//////////////////////////////////////////////////////////////////////////

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
	BYTE GetCardType(BYTE cbCardData[], BYTE cbCardCount);
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&LOGIC_MASK_COLOR; }

	//���ƺ���
public:
	//�����˿�
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount);
	//�����˿�
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//ɾ���˿�
	bool RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE cbCardData[], BYTE cbCardCount);

	//�߼�����
public:
	//��Ч�ж�
	bool IsValidCard(BYTE cbCardData);
	//�߼���ֵ
	BYTE GetCardLogicValue(BYTE cbCardData);

	//���ͺ���
public:
	//�Ƿ����
	bool IsDouble(BYTE cbCardData[], BYTE cbCardCount);
	//�Ƿ���֧
	bool IsTieZhi(BYTE cbCardData[], BYTE cbCardCount);
	//�Ƿ��«
	bool IsHuLu(BYTE cbCardData[], BYTE cbCardCount);
	//�Ƿ�ͬ��
	bool IsTongHua(BYTE cbCardData[], BYTE cbCardCount);
	//�Ƿ�˳��
	bool IsShunZhi(BYTE cbCardData[], BYTE cbCardCount);
	//�Ƿ�����
	bool IsSanTiao(BYTE cbCardData[], BYTE cbCardCount);
	//�Ƿ�ͬ��˳
	bool IsTongHuaShun(BYTE cbCardData[], BYTE cbCardCount);

	//���ܺ���
public:
	//�Ա��˿�
	bool CompareCard(BYTE cbFirstCardData, BYTE cbNextCardData);
	//�Ա��˿�
	bool CompareCard(BYTE bFirstList[], BYTE bNextList[], BYTE cbFirstCount, BYTE cbNextCount);
	//�����ж�
	bool SearchOutCard(BYTE cbCardData[], BYTE cbCardCount, BYTE bTurnCardData[], BYTE cbTurnCardCount, BYTE cbTurnOutType, BYTE cbOutCard[MAX_COUNT]);
	//�����˿�
	void AnalysebCardData(const BYTE bCardData[], BYTE bCardCount, tagAnalyseResult & AnalyseResult);
};

//////////////////////////////////////////////////////////////////////////

#endif