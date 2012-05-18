#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼���
class CGameLogic
{
	//��������
private:
	static const BYTE				m_bCardArray[52];					//�˿˶���

	//���ܺ���
public:	
	//�����˿�
	void RandCard(BYTE bCardBuffer[], BYTE bBufferCount);

	//��ȡ�˿���ֵ
	BYTE GetCardValue(BYTE bCard);
	//��ȡ�˿���ֵ
	BYTE GetCardSum(const BYTE bCardBuffer[], const BYTE bCardCount);	
	
	//�Աȵ�ֻ�˿�
	bool CompareOnlyOne(BYTE bFirstCard, BYTE bLastCard);	
	//�����˿�
	void SortCard(BYTE bCardList[], BYTE bCardCount);
	
	//�Ƿ���
	bool IsBurst(const BYTE bCardBuffer[], BYTE bCardCount);
	//�Ƿ�BlackJack
	bool IsBlackJack(const BYTE bCardBuffer[]);
	//�ܷ����
	bool CanCutCard(const BYTE bFirstCard, const BYTE bLastCard);
	
	//�ƶ�ʤ��
	BYTE DeduceWiner(const BYTE bBankerCard[],const BYTE bBankerCardCount,
	                 const BYTE bIdleCard[],const BYTE bIdleCardCount);

};

//////////////////////////////////////////////////////////////////////////

#endif