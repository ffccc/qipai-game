#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////

//�˿�����
#define CT_INVALID					0									//��������
#define CT_SINGLE					1									//��������
#define CT_DOUBLE					2									//��������
#define CT_THREE					3									//��������
//#define CT_ONE_LINE					4									//��������
//#define CT_DOUBLE_LINE				5									//��������
//#define CT_THREE_LINE				6									//��������
//#define CT_THREE_LINE_TAKE_ONE		7									//����һ��
//#define CT_THREE_LINE_TAKE_DOUBLE	8									//����һ��
//#define CT_FOUR_LINE_TAKE_ONE		9									//�Ĵ�����
//#define CT_FOUR_LINE_TAKE_DOUBLE	10									//�Ĵ�����
#define CT_LIANGZI					11									//��������
//#define CT_MISSILE_CARD				12									//�������
#define CT_DROGONANDPHONIX			13									//������
#define CT_EIGHTDOUBLE				14									//�˴��
#define CT_FISH						15									//��
#define CT_BAI						16									//��
#define CT_XIBA0LAING				17									//ϲ����
#define CT_XI						18
#define CT_OX						19

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
	BYTE							m_bSCardData[20];					//�����˿�
	BYTE							m_bDCardData[20];					//�����˿�
	BYTE							m_bTCardData[20];					//�����˿�
	BYTE							m_bFCardData[20];					//�����˿�
};
//�û�������Ϣ
struct tagUserOutCardInfo
{
	int								IsWiner;							//�Ƿ�Ӯ��
	BYTE							IsCiPai;								//����
	BYTE							bOxCard;							//ţ��
	BYTE							bXiCard;							//ϲ��
	BYTE							bXianPai;							//����
	BYTE							bBeiXian;							//����
	BYTE							bLiangPai;							//����

};

//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼���
class CGameLogic
{
	//��������
protected:
	static const BYTE				m_bCardListData[49];				//�˿�����

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//���ͺ���
public:
	//��ȡ����
	BYTE GetCardType(const BYTE bCardData[], BYTE bCardCount);
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE bCardData) { return bCardData&LOGIC_MASK_VALUE; }
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE bCardData) { return bCardData&LOGIC_MASK_COLOR; }

	//���ƺ���
public:
	//�����˿�
	void SortCardList(BYTE bCardData[], BYTE bCardCount);
	//�����˿�
	void RandCardList(BYTE bCardBuffer[], BYTE bBufferCount);
	//ɾ���˿�
	bool RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE bCardData[], BYTE bCardCount);

	//�߼�����
public:
	//��ڴ���
	bool IsRedAndBlackDouble(const BYTE bCardData[],BYTE bCardCount);
	bool IsRedAndBlackDouble(BYTE bFirstCardData,BYTE bNextCardData);
	//����
	bool IsQiangCi(const BYTE bCardData[],BYTE bCardCount);	
	//ţ��
	bool IsOx(const BYTE bCardData[],BYTE bCardCount);
	//ϲ��
	bool IsXi(const BYTE bCardData[],BYTE bCardCount);
	//�а�
	bool IshavingBai(const BYTE bCardData[],BYTE bCardCount);
	//����
	bool IsHavingFish(const BYTE bCardData[],BYTE bCardCount);
	//����
	bool IsBaiLiang(const BYTE bCardData[],BYTE bCardCount);
	//����
	bool IsFishLiang(const BYTE bCardData[],BYTE bCardCount);
	//ϲ����
	bool IsXiBaoLiang(const BYTE bCardData[],BYTE bCardCount);
	
	//�߼���ֵ
	BYTE GetCardLogicValue(BYTE bCardData);
	//�Ա��˿�
	bool CompareCard(const BYTE bFirstList[], const BYTE bNextList[], BYTE bFirstCount, BYTE bNextCount);

	//�ڲ�����
public:
	//�����˿�
	void AnalysebCardData(const BYTE bCardData[], BYTE bCardCount, tagAnalyseResult & AnalyseResult);
};

//////////////////////////////////////////////////////////////////////////

#endif