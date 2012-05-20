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

//�齫���ͽṹ
static enum BLOCKSTYLE
{	
	BLOCK_NULL	= 0,				//��
	BLOCK_SUN	= 1,				//˳��ṹ
	BLOCK_PENG	= 2,				//���ӽṹ,3����ͬ	
	BLOCK_KE	= 3,				//���ӽṹ,3����ͬ,����
	BLOCK_GANG	= 4,				//���ӽṹ,4����ͬ

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
	MJ_BLOCK block[4];				//�齫��
}MJ_HU, *PTR_MJ_HU;

//����
typedef struct GANG
{
	BYTE GangPos;				//����λ��0~16
	BYTE GangStyle;				//��������,3-����,2-��������,1-��ͨ��,0-�޸�
}MJ_GANGINFO, *PTR_MJ_GANGINFO;

class CGameLogic
{
	//��������
protected:
	static const BYTE m_byMjs[ORG_MJ_MAX];						//�齫������
	static const BYTE mjThirteen[13];									//ʮ������

	//����,����
public:
	CGameLogic();
	~CGameLogic();

	//���ͺ���
public:
	//��ȡ�齫����
	BYTE GetMjType( BYTE bMjData[] );
	//��ȡ�齫��ֵ
	BYTE GetMjValue( BYTE bMjData ) { return bMjData&LOGIC_MASK_VALUE; }

	//���ƺ���
public:
	//�����齫
	void SortMj( BYTE byMjs[], BYTE byMjCount );
	//�����齫
	void RandMj( BYTE byMjs[], BYTE byMjCount );
	//ɾ���齫

	//�߼�����
public:
	//�Ƿ�Ϊ����
	bool IsJiang( BYTE byMj1, BYTE byMj2 );
	//�Ƿ���Գ���
	bool IsChi();
	//�Ƿ��������
	bool IsPeng();
	//�Ƿ���Ժ���
	bool IsHu( BYTE byHandMj[], BYTE byHandMjCount, BYTE byMj, HU hu, HU tempHu );
	//�Ƿ���Ը���
	MJ_GANGINFO CheckGang();

	//��������
public:
	//�����齫
	bool LMove( BYTE byMjs[], BYTE byMaxCount, BYTE byRestCount );
	bool RMove( BYTE byMjs[] );
	//�����齫
	bool RMove( BYTE byMjs[], BYTE byMaxCount, BYTE byRestCount, BYTE MoveCount=1 );
	//�����齫
	void InsertMjList( BYTE byMjList[], BYTE byMaxCount, BYTE byRestCount );
	
	//�齫ֵת��
	//�齫ֵ--ͼ��У��
	bool TransMjDToI( BYTE srcMjData, BYTE *desMjData );
	bool TransMjDToI( BYTE srcMjData[], BYTE srcCount, BYTE *desMjData[], BYTE desCount );

	//ͼ����ֵ--�齫ֵ
	bool TransMjIToD( BYTE srcMjData, BYTE *desMjData );

	//����齫������ƽṹ
	void AddBlock( HU *hu, PTR_MJ_BLOCK block );
	//������ƽṹ����
	void CleanHu( HU *hu );
	//�������ƽṹ
	void CopyHu( HU *destHu, HU *srcHu );

	//�ڲ�����
private:
	//���ͷ����ж�
	MJ_BLOCK CheckBlock( BYTE byMj1, BYTE byMj2, BYTE byMj3 );
};

#endif