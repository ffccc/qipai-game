/*-------------------------------------------------------------------------------------------
|
|								Mj		CGameLogic.cpp
|	
|------------------------------------------------------------------------------------------*/


#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once 
#include "..\��Ϣ����\CMD_Mj.h"

//��ֵ����
#define LOGIC_MASK_VALUE		0x0F							//�齫��ֵ����
#define LOGIC_MASK_TYPE			0xF0							//�齫��������

//Ȧ��
static enum FENG
{
	FENG_EAST_EAST = 0,
	FENG_EAST_SOUTH,
	FENG_EAST_WEST,
	FENG_EAST_NORTH,
	FENG_SOUTH_EAST,
	FENG_SOUTH_SOUTH,
	FENG_SOUTH_WEST,
	FENG_SOUTH_NORTH,
	FENG_WEST_EAST,
	FENG_WEST_SOUTH,
	FENG_WEST_WEST,
	FENG_WEST_NORTH,
	FENG_NORTH_EAST,
	FENG_NORTH_SOUTH,
	FENG_NORTH_WEST,
	FENG_NORTH_NORTH,
};

//�齫���ͽṹ
static enum BLOCKSTYLE
{	
	BLOCK_NULL	 = 0,				//��
	BLOCK_CHI,						//���ƽṹ
	BLOCK_SUN,	 					//˳��ṹ
	BLOCK_PENG,	 					//���ӽṹ,3����ͬ	
	BLOCK_KE,						//���ӽṹ,3����ͬ,����
	BLOCK_GANG,						//���ӽṹ,4����ͬ
	BLOCK_AN_GANG ,					//����
};

//�齫��ṹ
typedef struct BLOCK
{
	BYTE first;						//�齫���һ����ֵ
	BLOCKSTYLE style;				//�齫������
}MJ_BLOCK,*PTR_MJ_BLOCK;

//���ƽṹ
typedef struct HU
{
	BYTE jiangMj;					//����
	BLOCK block[(HAND_MJ_MAX-2)/3];				//�齫��
}MJ_HU, *PTR_MJ_HU;

//����
typedef struct GANG
{
	BYTE GangMj;				//���ܵ���
	BYTE GangStyle;				//��������,4-����,2-��������,1-��ͨ��,0-�޸�
}MJ_GANGINFO, *PTR_MJ_GANGINFO;

class CGameLogic
{
	//��������
protected:
	//static const BYTE m_byMjs[ORG_MJ_MAX];								//�齫������
	static const BYTE mjThirteen[13];									//ʮ������
	static const BYTE mjNineLight[13];									//������
	
	//����,����
public:
	CGameLogic();
	~CGameLogic();

	//���ͺ���
public:
	//��ȡ�齫����
	BYTE GetMjType( BYTE byMjData )	{ return byMjData&LOGIC_MASK_TYPE; }
	//��ȡ�齫��ֵ
	BYTE GetMjValue( BYTE bMjData ) { return bMjData&LOGIC_MASK_VALUE; }

	//���ƺ���
public:
	//�����齫
	void SortMj( BYTE byMjs[], BYTE byMjCount, BYTE byMjMax );
	//�����齫
	void RandMj( BYTE byMjs[], BYTE byMjCount );
	//ɾ���齫

	//�߼�����
public:
	//�Ƿ�Ϊ����
	bool IsJiang( BYTE byMj1, BYTE byMj2 );
	//�Ƿ���Գ���, ���س�����Ϣ��0-���ɹ�,1-**@, 2-*@*, 3-@@*;
	int IsChi( BYTE byHandMj[], BYTE byHandMjCount, BYTE byMj );
	//�Ƿ��������	
	bool IsPeng( BYTE byHandMj[], BYTE byHandMjCount, BYTE byMj );
	//�Ƿ��������
	MJ_GANGINFO IsGang( BYTE byHandMj[], BYTE byHandMjCount, BYTE byMj );
	//�Ƿ���������
	MJ_GANGINFO IsZMGang( BYTE byHandMj[], BYTE byHandMjCount, HU hu, BYTE jPos1=-1, BYTE jPos2=-1 );
	//�Ƿ񰵸�
	MJ_GANGINFO IsAnGang( BYTE byHandMj[], BYTE byHandMjCount,BYTE jPos1=-1, BYTE jPos2=-1 );	
	
	//�Ƿ���Ժ���
	int IsHu( BYTE byHandMj[], BYTE byHandMjCount, BYTE byMj, HU hu, HU *tempHu );
	//�������
	bool IsSpecialHu( BYTE tempMj[] );
	bool IsThirteenOne( BYTE tempMj[] );
	bool IsSevenPairs( BYTE tempMj[] );
	bool IsNineLight( BYTE tempMj[], BYTE byMj );

	//��������
public:
	//������
	long Pow( long x, long y);
	//Ͷ����
	int ActionDice();
	//�����齫
	bool LMove( BYTE byMjs[], BYTE byRestCount );
	bool RMove( BYTE byMjs[] );
	//�����齫
	bool RMove( BYTE byMjs[], BYTE byRestCount, BYTE MoveCount=1 );
	//�����齫
	void InsertMjList( BYTE byMjList[], BYTE byRestCount );
	
	//�齫ֵת��
	//�齫ֵ--ͼ��У��
	bool TransMjDToI( BYTE srcMjData, BYTE *desMjData );
	bool TransMjDToI( BYTE srcMjData[], BYTE *desMjData[], BYTE desCount );

	//ͼ����ֵ--�齫ֵ
	bool TransMjIToD( BYTE srcMjData, BYTE *desMjData );

	//����齫������ƽṹ
	void AddBlock( HU *hu, PTR_MJ_BLOCK block );
	//������ƽṹ����
	void CleanHu( HU *hu );
	//�������ƽṹ
	void CopyHu( HU *destHu, HU *srcHu );


	//�ŷ����
	BYTE TransIDToFeng(  WORD wChairID, WORD wBanker );

	//����
public:
	//��
	BLOCK Peng( BYTE byMjs[], BYTE byRestCount, BYTE byMj );
	//��
	BLOCK Chi( BYTE byMjs[], BYTE byRestCount, BYTE byMj, int ChiMode );
	//��
	BLOCK MGang( BYTE byMjs[], BYTE byRestCount, BYTE byMj );
	//������
	void ZMGang( BYTE byMjs[], BYTE byRestCount, HU *hu, BYTE GangMj );
	//����
	BLOCK AnGang( BYTE byMjs[], BYTE byRestCount, BYTE GangMj );
	//�������ƣ����ط�ֵ
	long CheckGDHu( HU hu, BYTE byFengWei, BYTE byQuanWei, char *strFang );

	//�ڲ�����
private:
	//���ͷ����ж�
	MJ_BLOCK CheckBlock( BYTE byMj1, BYTE byMj2, BYTE byMj3 );
};

#endif