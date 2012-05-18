#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

using namespace std;

#pragma once

//#include "..\..\..\���������\��Ϸ����\GameServiceExport.h"

//////////////////////////////////////////////////////////////////////////

//��עλ�ö��壻�ء��졢�͡���1->��9����1->��9����21����עλ��
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������
#define INDEX_DRAW					2									//������
#define INDEX_PLAYER_MIN		    3									//����С����
#define INDEX_PLAYER_MAX		   10									//���������
#define INDEX_BANKER_MIN		   10									//ׯ��С����
#define INDEX_BANKER_MAX		   10									//ׯ�������
#define INDEX_MAX				   15									//�������


#define HISTORY_COUNT				30									//��ʷ��¼����

//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����

//#define MAX_SCORE					100000000							//�����
#define MAX_SCORE					((INT64)pow(2.0,31.0)-1)					//�����

#define NO_WINNER					0									//û��Ӯ��ʱ��ȡֵ

const int LOSS_RATES[] ={19,19,19,19,4,4,4,4};


//��Ϸ�߼�
class CGameLogic
{
	//��������
private:
	static const BYTE				m_cbCardListData[52*8];				//�˿˶���

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//���ͺ���
public:
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&LOGIC_MASK_COLOR; }

	//���ƺ���
public:
	//�����˿�
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);

	//�߼�����
public:
	//��ȡ�Ƶ�
	BYTE GetCardPip(BYTE cbCardData);
	//��ȡ�Ƶ�
	BYTE GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount);
	//�����ƾ�ȡÿ����ע���������
	void GetOdds(BYTE cbPlayerPip, BYTE cbBankerPip, INT nOdds[],BYTE playCardArray[] ,BYTE bankCardArray[],int playcount ,int bankcount);

	//�������ʣ���ע�������Ӯ���
	void GetWinAcount(INT nOdds[], INT64 pUserStakes[], LONG lRakeRate, long& lWinAmount, long& lRakeOff);

	//�����������ע�����ׯ����Ӯ���
	LONG GetWinAcountBanker(BYTE cbPlayerValue, BYTE cbBankerValue, LONG pUserStakes[], LONG lRakeRate);

	//�������ׯ������⸶
	LONG GetBankerMaxLoss(BYTE cbBetArea, LONG lBetSum[], LONG lRakeRate);

	//����ÿ������������ע��
	void GetMaxStakes(LONG lMaxStakes[], LONG lBetSum[], LONG lBankerMaxLoss, LONG lRakeRate);

	//������㣬�ж��Ƿ񳬹�ׯ������⸶
	bool BankerIsOver(BYTE cbBetArea, LONG lBetAmount, LONG lBetSumOld[], LONG lBankerMaxLoss, LONG lRakeRate);
    bool BankerIsEnd(BYTE cbBetArea, LONG lBetAmount, INT64 lBetSumOld[], INT64 lBankerMaxLoss, INT64 lBankersore);
	//ȡ������е���Сֵ
	INT64 Min(INT64 iFirst, ...);

};

//////////////////////////////////////////////////////////////////////////

#endif