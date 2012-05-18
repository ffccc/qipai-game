#pragma once

#include "..\��Ϣ����\GameLogic.h"

///////////////////////////////////////////////////
//�ṹ����
struct  CardNumber
{
	BYTE           bCardLogicValue;										//�˿˴�С
	BYTE           bCardValue;											//�˿�ֵ
	BYTE           bCardCount;											//�˿���Ŀ
	BYTE           bCardLocation;										//�˿�λ��
};

struct CardType
{
	short           bCardType;											//�˿�����
	BYTE            bCardCount;											//�˿���Ŀ
	BYTE            bCardLocation[15];									//�˿�λ��
	CardType(){ZeroMemory(bCardLocation,sizeof(bCardLocation));}
};
//////////////////////////////////////////////////
//��ʾ��
class CGamePrompt:public CGameLogic
{
public:
	CGamePrompt(void);
	~CGamePrompt(void);

	//���Ʊ���
public:
    short							m_bTurnCardType;				    //��������
    BYTE                            m_bCardData[28];					//�˿�����
	BYTE                            m_bCardCount;						//�˿���Ŀ
	//��������
protected:
    BYTE                            m_bRound;							//��ͬ�����˿���
	BYTE                            m_bCardPosition[28];				//�˿�λ��
	BYTE							m_bHongTaoCount;                    //������Ŀ
	BYTE                            m_bHongTaoLocation[2];				//����λ��

	//��������
public:
	CArrayTemplate<CardType>		OutCardList;						//�����б�
	CardNumber					    CardData[20];						//ͬ����Ŀ
	CardNumber						desiCardData[20];					//ͬ����Ŀ
	
	//��������
public:
    //������ʾ
	void   Prompt();

	//��������
protected:
	//��������
	void  GetSingle();
	//��������
	void  GetDouble();
	//����santiao
	void  GetSanTiao();
	//������«
	void  GetHulu();
	//����������
	void  GetLiangLianDui();
	//����˳��
	void GetShunZhi();
	//�����ְ�
	void GetGangBan();
	//����ͬ��˳
	void GetTongHuaShun(BYTE mark=0);
	//������ը��
	void GetSiZhangBomb();
	//������ը��
	void GetWuZhangBomb();
	//������ը��
	void GetLiuZhangBomb();
	//������ը��
	void GetQiZhangBomb();
	//������ը��
	void GetBaZhangBomb();
	//������ը��
	void GetJiuZhangBomb();
	//����ʮը��
	void GetShiZhangBomb();
	//��������
	void GetSiWang();


};
