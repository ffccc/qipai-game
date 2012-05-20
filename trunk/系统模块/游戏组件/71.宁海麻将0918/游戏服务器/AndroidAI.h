#ifndef _ANDROIDAI_H_
#define _ANDROIDAI_H_

#pragma once

#include "GameLogic.h"

class CAndroidAIBase
{
public:
	CAndroidAIBase();
	~CAndroidAIBase();
	//������
	void SetCardData( const BYTE cbCardData[],BYTE byCardCount );	
	//ģ�����
	void SetAction( BYTE byActionMask,BYTE byActionCard );
	//��ȡ�����
	BYTE GetBadlyCard();													
	//��ȡ����
	int GetMaxScore();	
	//˼��
	virtual void Think()=NULL;

protected:
	//����Ƿ��Ǳ�
	bool IsEdge( BYTE byCard1,BYTE byCard2 );
	
	//������ͬ��
	bool SearchSameCard( BYTE byCardData,BYTE &byIndex1,BYTE &byIndex2 );
	//��������
	bool SearchLinkCard( BYTE byCardData,BYTE &byIndex1,BYTE &byIndex2 );

	//������ֻͬ��
	bool SearchSameCardRemain( BYTE byCardData,BYTE &byIndex,BYTE byStart );
	//�����п�����
	bool SearchLinkCardRemain( BYTE byCardData,BYTE byLinkType,BYTE &byIndex,BYTE byStart );

	//������
	BYTE FindIndex( BYTE byCardData,BYTE byStart = 0 );
	//���Ƴ������ֻ��������
	BYTE FindIndexRemain( BYTE byCardData,BYTE byStart = 0 );
	//�Ƴ���
	bool RemoveCard( BYTE byCardIndex );

protected:
	BYTE		m_byCardData[MAX_COUNT];				//ԭʼ��
	BYTE		m_byCardCount;							//ԭʼ����

	bool		m_bSelect[MAX_COUNT];					//���Ƿ�ѡ��

	BYTE		m_byThreeCard[MAX_COUNT];				//���������
	BYTE		m_byThreeCount;							//�����������
	BYTE		m_byGoodThreeCard[MAX_COUNT];			//������������
	BYTE		m_byGoodThreeCount;						//��������������

	BYTE		m_byTwoCard[MAX_COUNT];					//��ֻ�����
	BYTE		m_byTwoCount;							//��ֻ�������
	BYTE		m_byGoodTwoCard[MAX_COUNT];				//�����ֻ�����
	BYTE		m_byGoodTwoCount;						//�����ֻ�������

	BYTE		m_byRemainThree[MAX_COUNT];				//��ȥ�����ֻ��Ϻ����
	BYTE		m_byRemainThreeCount;					//��ȥ�����ֻ��Ϻ������

	BYTE		m_byRemainTwo[MAX_COUNT];				//��ȥ�����ֻ��ϼ���ֻ��Ϻ����
	BYTE		m_byRemainTwoCount;						//��ȥ�����ֻ��ϼ���ֻ��Ϻ������

	int			m_nMaxScoreThree;						//�����ֻ����Ϸ���
	int			m_nMaxScoreTwo;							//�����ֻ����Ϸ���
	int			m_nActionScore;							//ģ������÷�

	int			m_nScoreThree;							//��ʱ��ֻ����Ϸ���
	int			m_nScoreTwo;							//��ʱ��ֻ����Ϸ���

	bool		m_bHaveJiang;							//�Ƿ��н�

	BYTE		m_byBadlyCard;							//�����
};

class CAndroidAI:public CAndroidAIBase
{
public:
	CAndroidAI(void);
	~CAndroidAI(void);
	//˼��
	virtual void Think();
	//�������ֻ����������������
	BYTE GetBadlyIn3Card();
	//�������ֻ����������������
	BYTE GetBadlyIn2Card();
	//���ý�ֹ������
	void SetEnjoinOutCard( const BYTE cbEnjoinOutCard[],BYTE cbEnjoinOutCount );

protected:
	//��Ȩ
	int AddScore( BYTE byCardData );
	//������ֻ
	void AnalyseThree();
	//������ֻ
	void AnalyseTwo();
	//������ֻ
	void AnalyseOne();
	//����ֻ����з���
	void SearchTwo();
	//�ж����Ƿ��ֹ��
	bool IsEnjoinOutCard( BYTE byCard );

protected:
	BYTE m_byEnjoinOutCard[MAX_COUNT];				//��ֹ������
	BYTE m_byEnjoinOutCount;						//��ֹ��������
};

#endif