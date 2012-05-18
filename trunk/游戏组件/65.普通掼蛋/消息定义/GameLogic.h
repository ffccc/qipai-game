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
#define CT_HU_LU					5									//��«����
#define CT_GANG_BAN                 6                                   //�ְ�����
#define CT_LIANG_LIAN_DUI           7                                   //����������
#define CT_SI_ZHANG_BOMB            8                                   //����ը��
#define CT_WU_ZHANG_BOMB            9                                   //����ը��
#define CT_TONG_HUA_SHUN			10									//ͬ��˳��
#define CT_LIU_ZHANG_BOMB           11                                  //����ը��
#define CT_QI_ZHANG_BOMB            12                                  //����ը��
#define CT_BA_ZHANG_BOMB            13                                  //����ը��
#define CT_JIU_ZHANG_BOMB           14                                  //����ը��
#define CT_SHI_ZHANG_BOMB           15                                  //ʮ��ը��
#define CT_FOUR_KING                16                                  //��������

//�궨��
#define MAX_COUNT					54									//�����Ŀ
#define MAX_ROUND                   50                                  //�������

//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����

//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼���
class CGameLogic
{
	//��Ԫ����
	friend class CGamePrompt;
	
	//��������
protected:
	static const BYTE				m_bCardListData[108];				//�˿�����
	BYTE                            m_bValue;                           //������ֵ
public:
	static BYTE                     m_bSeries;                          //��Ϸ����
	
	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//���ͺ���
public:
	//��ȡ����
	short GetCardType(BYTE cbCardData[], BYTE cbCardCount);
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&LOGIC_MASK_COLOR; }

	//���ƺ���
public:
	//�����˿�
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount);
	//����ɫ��
	void SortByColor(BYTE  cbCardData[], BYTE cbCardCount);
	//��ը����
	void SortByBomb	(BYTE  cbCardData[], BYTE cbCardCount);
	//�����˿�
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//ɾ���˿�
	bool RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE cbCardData[], BYTE cbCardCount);
    //����˿�
	bool Add(BYTE bAddCard, BYTE  cbCardData[],BYTE cbCardCount);
	//�߼�����
public:
	//��Ч�ж�
	bool IsValidCard(BYTE cbCardData);
	//�߼���ֵ
	BYTE GetCardLogicValue(BYTE cbCardData);

	//���ͺ���
public:
	//ը��ֵ����
	int  BombCount(BYTE cbCardData[],BYTE cbCardCount);
	//�Ƿ����
	bool IsDouble(BYTE cbCardData[], BYTE cbCardCount);
	//�Ƿ��«
	bool IsHuLu(BYTE cbCardData[], BYTE cbCardCount);
	//�Ƿ�˳��
	bool IsShunZhi(BYTE cbCardData[], BYTE cbCardCount);
	//�Ƿ�����
	bool IsSanTiao(BYTE cbCardData[], BYTE cbCardCount);
	//�Ƿ�����ը��
	bool IsSiZhangBomb(BYTE cbCardData[], BYTE cbCardCount);
	//�Ƿ�����ը��
	bool IsWuZhangBomb(BYTE cbCardData[], BYTE cbCardCount);
	//�Ƿ�ͬ��˳
	bool IsTongHuaShun(BYTE cbCardData[], BYTE cbCardCount);
	//�Ƿ�����ը��
	bool IsLiuZhangBomb(BYTE cbCardData[], BYTE cbCardCount);
	//�Ƿ�����ը��
	bool IsQiZhangBomb(BYTE cbCardData[], BYTE cbCardCount);
	//�Ƿ����ը��
	bool IsBaZhangBomb(BYTE cbCardData[], BYTE cbCardCount);
	//�Ƿ����ը��
	bool IsJiuZhangBomb(BYTE cbCardData[], BYTE cbCardCount);
	//�Ƿ�ʮ��ը��
	bool IsShiZhangBomb(BYTE cbCardData[], BYTE cbCardCount);
	//�Ƿ�����
	bool IsFourKing(BYTE cbCardData[], BYTE cbCardCount);
	//�Ƿ�ְ�
	bool IsGangBan(BYTE cbCardData[], BYTE cbCardCount);
	//�Ƿ�������
	bool IsLiangLianDui(BYTE cbCardData[], BYTE cbCardCount);
    
	//���ܺ���
public:
	//�Ա��˿�
	bool CompareCard(BYTE bFirstList[], BYTE bNextList[], BYTE cbFirstCount, BYTE cbNextCount);
	//�����ж�
	bool SearchOutCard(BYTE cbCardData[], BYTE cbCardCount, BYTE bTurnCardData[], BYTE cbTurnCardCount, BYTE cbTurnOutType, BYTE cbOutCard[MAX_COUNT]);
	
};

//////////////////////////////////////////////////////////////////////////

#endif