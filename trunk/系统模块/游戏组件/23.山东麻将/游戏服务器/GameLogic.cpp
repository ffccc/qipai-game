/*-------------------------------------------------------------------------------------------
|
|								Mj		CGameLogic.cpp
|	
|------------------------------------------------------------------------------------------*/

#include "stdafx.h"
#include "GameLogic.h"
#include <vector>
//#define NOVE_FLOWER			//�л�
///////////////////////////////////////////////////////////////////////////////////////////////
//��̬����

//�齫����
//#ifdef HAVE_FLOWER
//const BYTE CGameLogic::m_byMjs[ORG_MJ_MAX] = 
//{
//	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,				//��
//	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,				//��
//	0X21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,				//��
//	0x31, 0x32, 0x33, 0x34,												//��
//	0x41, 0x42, 0x43,													//��
//	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,				//��
//	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,				//��
//	0X21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,				//��
//	0x31, 0x32, 0x33, 0x34,												//��
//	0x41, 0x42, 0x43,													//��
//	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,				//��
//	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,				//��
//	0X21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,				//��
//	0x31, 0x32, 0x33, 0x34,												//��
//	0x41, 0x42, 0x43,													//��
//	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,				//��
//	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,				//��
//	0X21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,				//��
//	0x31, 0x32, 0x33, 0x34,												//��
//	0x41, 0x42, 0x43,													//��
//	0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,						//��
//};
//#endif
////�޻�
//#ifdef NO_FLOWER
//const BYTE CGameLogic::m_byMjs[ORG_MJ_MAX] = 
//{
//	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,				//��
//	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,				//��
//	0X21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,				//��
//	0x31, 0x32, 0x33, 0x34,												//��
//	0x41, 0x42, 0x43,													//��
//	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,				//��
//	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,				//��
//	0X21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,				//��
//	0x31, 0x32, 0x33, 0x34,												//��
//	0x41, 0x42, 0x43,													//��
//	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,				//��
//	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,				//��
//	0X21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,				//��
//	0x31, 0x32, 0x33, 0x34,												//��
//	0x41, 0x42, 0x43,													//��
//	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,				//��
//	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,				//��
//	0X21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,				//��
//	0x31, 0x32, 0x33, 0x34,												//��
//	0x41, 0x42, 0x43,													//��
//};
//#endif
//ʮ����
const BYTE CGameLogic::mjThirteen[13] =
{
	0x01, 0x09, 0x11, 0x19, 0x21, 0x29, 0x31, 0x32, 0x33, 0x34, 0x41, 0x42, 0x43
};
const BYTE CGameLogic::mjNineLight[13] =
{
	1, 1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 9 
};
/////////////////////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameLogic::CGameLogic()
{
}

//��������
CGameLogic::~CGameLogic()
{
}

//�����齫
void CGameLogic::RandMj( BYTE byMjs[], BYTE byMjCount )
{
	srand( (unsigned)time( NULL ) );
	BYTE index,mjTemp;

	//���Ҵ���
	//for( int i=0; i<2; i++ )
	{
		for( int j=0; j<byMjCount; j++ )
		{
			index = rand()%byMjCount;
			if( j!=index )
			{
				mjTemp = byMjs[j];
				byMjs[j] = byMjs[index];
				byMjs[index] = mjTemp;
			}
		}
	}
}

//�����齫
void CGameLogic::SortMj( BYTE byMjs[], BYTE byMjCount, BYTE byMjMax )
{
	BYTE i, j, tag, mjTemp;
	for( i=0,tag=1; tag==1&&i<byMjCount; i++ )
	{
		tag = 0;
		for( j=0; j<byMjCount; j++ )
		{
			if( j+1<byMjMax && byMjs[j]>byMjs[j+1]  )
			{
				mjTemp = byMjs[j];
				byMjs[j] = byMjs[j+1];
				byMjs[j+1] = mjTemp;
				tag = 1;
			}
		}
	}
}

//�����ж�
MJ_BLOCK CGameLogic::CheckBlock( BYTE byMj1, BYTE byMj2, BYTE byMj3 )
{
	MJ_BLOCK block;
	block.first = 0xff;
	block.style = BLOCK_NULL;
	
	//���byMj1, byMj2, byMj3����˳��ṹ,����
	int i, tag, temp;
	for( i=0,tag=1; tag==1&&i<3; i++ )
	{
		tag = 0;
		if( byMj1 > byMj2 )
		{
			temp = byMj1;
			byMj1 = byMj2;
			byMj2 = temp;
			tag = 1;
		}
		if( byMj2 > byMj3 )
		{
			temp = byMj2;
			byMj2 = byMj3;
			byMj3 = temp;
			tag = 1;
		}
	}
	
	//���νṹ

	if( byMj1==byMj2 && byMj2==byMj3 )
	{
		block.first = byMj1;
		block.style = BLOCK_KE;
		return block;
	}

	//˳�νṹ	
	if( byMj3<0x31 )			//��Ϊ��, ��
	{
		if( byMj3==byMj2+1 && byMj2==byMj1+1 )
		{
			block.first = byMj1;
			block.style = BLOCK_SUN;
			return block;
		}
	}
	return block;
}

//�Ƿ����������
bool CGameLogic::IsJiang( BYTE byMj1, BYTE byMj2 )
{
	if( byMj1 == byMj2 )
	{
		if (GetMjValue(byMj1)==2 || GetMjValue(byMj1)==5 || GetMjValue(byMj1)==8)//ɽ���齫2��5��8������
			if (GetMjType(byMj1)<0x30)
				return true;
	}
	return false;
}

//�Ƿ���Գ���
int CGameLogic::IsChi( BYTE byHandMj[], BYTE byHandMjCount, BYTE byMj )
{
	//��,�ֲ�����
	if( byMj > 0x30 )
	{
		return false;
	}
	int chiInfo = 0;			//������Ϣ, 0-�޳���,1-**@��, 2-*@*, 4-@**��
	bool pos2,pos1,pos;
	pos2 = pos1 = pos = true;
	for( int i=HAND_MJ_MAX-byHandMjCount-1; i<HAND_MJ_MAX-1; i++ )
	{
		//**@
		if( byHandMj[i] == byMj-2 && pos2 )
		{
			for( int j=i; j<HAND_MJ_MAX-1; j++ )
			{
				if( byHandMj[j] == byMj-1 )
				{
					chiInfo += 1;			// ��������1
					break;
				}
			}
			pos2 = false;
		}
		//*@*
		else if( byHandMj[i] == byMj-1 && pos1 )
		{
			for( int j=i; j<HAND_MJ_MAX-1; j++ )
			{
				if( byHandMj[j] == byMj+1 )
				{
					chiInfo += 2;			//��������2
					break;
				}
			}
			pos1 = false;
		}
		//@**
		else if( byHandMj[i] == byMj+1 && pos )
		{
			for( int j=i; j<HAND_MJ_MAX-1; j++ )
			{
				if( byHandMj[j] == byMj+2 )
				{
					chiInfo += 4;			//��������2
					break;
				}
			}
			pos = false;
		}
	}
	return chiInfo;
}

//�Ƿ��������
bool CGameLogic::IsPeng( BYTE byHandMj[], BYTE byHandMjCount, BYTE byMj )
{
	for( int i=HAND_MJ_MAX-byHandMjCount-1; i<HAND_MJ_MAX-1-1; i++ )
	{
		if( byHandMj[i]==byMj && byHandMj[i+1]== byMj )
		{
			return true;
		}
	}
	return false;
}

//�Ƿ�����
MJ_GANGINFO CGameLogic::IsGang( BYTE byHandMj[], BYTE byHandMjCount, BYTE byMj )
{
	MJ_GANGINFO gangInfo;
	gangInfo.GangStyle= 0;				
	gangInfo.GangMj= 0;
	//�������ϲ����е�ʮ������
	for( int i=HAND_MJ_MAX-byHandMjCount-1; i<HAND_MJ_MAX-3-1; i++ )
	{
		if( byHandMj[i]==byMj && byHandMj[i+1]==byMj && byHandMj[i+2]==byMj )
		{
			gangInfo.GangMj= byHandMj[i];
			gangInfo.GangStyle= 1;
		}
	}
	return gangInfo;
}

//�������� ����,
MJ_GANGINFO CGameLogic::IsZMGang( BYTE byHandMj[], BYTE byHandMjCount, HU hu,BYTE jPos1, BYTE jPos2 )
{
	MJ_GANGINFO gangInfo;
	gangInfo.GangStyle= 0;				
	gangInfo.GangMj= 0xff;
	for( int i=0; i<(HAND_MJ_MAX-2)/3; i++ )
	{
		if( hu.block[i].style == BLOCK_NULL )
			break;
		else if( hu.block[i].style != BLOCK_PENG )
			continue;
		else
		{
			//�����������
			if( hu.block[i].first==jPos1 || hu.block[i].first==jPos2 )
				continue;
			for( int j=HAND_MJ_MAX-byHandMjCount; j<HAND_MJ_MAX; j++ )
			{
				//if( byHandMj[j]==jPos1 || byHandMj[j]==jPos2 )
				//	continue;
				if(  byHandMj[j]==hu.block[i].first )
				{
					gangInfo.GangStyle= 2;					//��������
					gangInfo.GangMj= byHandMj[j];			//���ܺ��ƿ�λ
					return gangInfo;
				}
			}
		}
	}
	return gangInfo;
}
//�Ƿ񰵸�
MJ_GANGINFO CGameLogic::IsAnGang( BYTE byHandMj[], BYTE byHandMjCount,BYTE jPos1, BYTE jPos2 )
{
	MJ_GANGINFO gangInfo;
	gangInfo.GangStyle= 0;				
	gangInfo.GangMj= 0xff;
	for( int i=HAND_MJ_MAX-byHandMjCount; i<HAND_MJ_MAX-3; i++ )
	{
		//����
		if( byHandMj[i]== jPos1 || byHandMj[i]==jPos2 )
			continue;
		if( byHandMj[i]==byHandMj[i+1] && byHandMj[i]==byHandMj[i+2] )
		{
			//�����и���
			if( byHandMj[i]==byHandMj[i+3] )
			{
				gangInfo.GangStyle= 4;
				gangInfo.GangMj= byHandMj[i];
			}
			//����������
			else if( byHandMj[i] == byHandMj[HAND_MJ_MAX-1])
			{
				gangInfo.GangStyle= 4;
				gangInfo.GangMj= byHandMj[i];
			}
		}
	}
	return gangInfo;
}

//�Ƿ���Ժ���
int CGameLogic::IsHu( BYTE byHandMj[], BYTE byHandMjCount, BYTE byMj, HU hu, HU *tempHu )
{
	//�����
	if( byHandMjCount == HAND_MJ_MAX-1 )
	{
		if( IsNineLight( byHandMj, byMj ) ) return 20+9;		//������,�ŷ�
		//else return false;
	}
	
	//��������
	BYTE tempMj[HAND_MJ_MAX];
	for( int tag=1, i= HAND_MJ_MAX-1-byHandMjCount; i< HAND_MJ_MAX-1; i++ )
	{
		if( tag )
		{
			tempMj[i] = byHandMj[i];
			if( tag==1 && tempMj[i]>byMj )
			{
				tempMj[i+1] = tempMj[i];
				tempMj[i] = byMj;
				tag = 0;
			}
			else
			{
				if( i == HAND_MJ_MAX-1-1 )
				{
					tempMj[HAND_MJ_MAX-1] = byMj;
				}
			}
		}
		else
		{
			tempMj[i+1] = byHandMj[i];
		}
	}
	//�����
	if( byHandMjCount == HAND_MJ_MAX-1 )
	{
		if( IsSpecialHu( tempMj ) ) return 20+13;				//ʮ����,�ŷ�
		//else return false;
	}
	//����λ��������( 0, 3, 6, 9, 12 )��
	//���ñ���
	MJ_BLOCK block1;
	for( int i=HAND_MJ_MAX-1-byHandMjCount; i<HAND_MJ_MAX-1; i+=3 )
	{
		if( IsJiang( tempMj[i], tempMj[i+1] ) )
		{
			//���ƽṹ���� 
			CleanHu( tempHu );
			CopyHu( tempHu, &hu );
			tempHu->jiangMj = tempMj[i];
			//����
			if( tempHu->block[(HAND_MJ_MAX-2)/3-1].style != BLOCK_NULL )
			{
				return 1;
			}
			for( int j=HAND_MJ_MAX-1-byHandMjCount; j<HAND_MJ_MAX-1-1; j+=3 )
			{
				if(  j==i )
				{
					if( i== HAND_MJ_MAX-1-1 )
					{
						return 1;
					}
					else
					{
						j += 2;
					}
				}

				block1 = CheckBlock( tempMj[j], tempMj[j+1], tempMj[j+2] );
				if( block1.style != BLOCK_NULL )
				{
					AddBlock( tempHu, &block1 );
				}
				else			
				{
					//����Ƕ������BLOCK�ṹ,������BLOCK�ṹ
					if( j>i&&j+5<=HAND_MJ_MAX-1 || j<i-5 )
					{
						MJ_BLOCK block2;
						//�������  223344 �� 233334
						//���1   223344 
						block1 = CheckBlock( tempMj[j], tempMj[j+2], tempMj[j+4] );
						block2 = CheckBlock( tempMj[j+1], tempMj[j+3], tempMj[j+5] );
						if( block1.style != BLOCK_NULL  && block2.style != BLOCK_NULL )
						{
							AddBlock( tempHu, &block1 );
							AddBlock( tempHu, &block2 );
							j += 3;
						}
						//���2 233334
						else
						{
							block1 = CheckBlock( tempMj[j], tempMj[j+1], tempMj[j+5] );
							block2 = CheckBlock( tempMj[j+2], tempMj[j+3], tempMj[j+4] );
							if( block1.style != BLOCK_NULL && block2.style != BLOCK_NULL )
							{
								AddBlock( tempHu, &block1 );
								AddBlock( tempHu, &block2 );
								j += 3;
							}
						}
						
					}	
					else
					{
						break;
					}
				}
				if( tempHu->block[(HAND_MJ_MAX-2)/3-1].style != BLOCK_NULL )
				{
					return 1;
				}
			}
		}
	}

	//����������λ��( 1,4, 7, 10 ),( 2, 5, 8, 11 ), �����㷨��ͬ
	//ֻ��һ��, �磺12223 ����λ��Ϊ1, 2
	//				12333
	for( int i=HAND_MJ_MAX-1-byHandMjCount+2; i< HAND_MJ_MAX-1; i+=3 )
	{
		if( IsJiang( tempMj[i], tempMj[i+1] ) )
		{
			CleanHu( tempHu );
			CopyHu( tempHu, &hu );
			block1 = CheckBlock( tempMj[i-2], tempMj[i-1], tempMj[i+2] );
			if( block1.style != BLOCK_NULL )
			{
				AddBlock( tempHu, &block1 );
				for( int j=HAND_MJ_MAX-1-byHandMjCount; j<HAND_MJ_MAX-1-1; j+=3 )
				{
					if( j == i-2 )
					{
						j+=5;				//������������5����
					}
					block1 = CheckBlock( tempMj[j], tempMj[j+1], tempMj[j+2] );
					if( block1.style != BLOCK_NULL )
					{
						AddBlock( tempHu, &block1 );
					}
					else
					{
						if( j>i&&j+5<=HAND_MJ_MAX-1 || j+5<i )
						{
							MJ_BLOCK block2;
							//�������
							block1 = CheckBlock( tempMj[j], tempMj[j+2], tempMj[j+4] );
							if( block1.style != BLOCK_NULL )
							{
								block2 = CheckBlock( tempMj[j+1], tempMj[j+3],tempMj[j+5] );
								if( block2.style != BLOCK_NULL )
								{
									AddBlock( tempHu, &block1 );
									AddBlock( tempHu, &block2 );
								}
							}

							else
							{
								block1 = CheckBlock( tempMj[j], tempMj[j+1], tempMj[j+5] );
								if( block1.style != BLOCK_NULL )
								{
									block2 = CheckBlock( tempMj[j+2], tempMj[j+3], tempMj[j+4] );
									if( block2.style != BLOCK_NULL )
									{
										AddBlock( tempHu, &block1 );
										AddBlock( tempHu, &block2 );
									}
								}
							}
						}
						else
						{
							break;
						}
					}
					if( tempHu->block[(HAND_MJ_MAX-2)/3-1].style != BLOCK_NULL )
					{
						return 1;
					}
				}
			}
		}
	}

	return 0;
}

//�������
bool CGameLogic::IsSpecialHu( BYTE tempMj[] )
{
	if( IsThirteenOne( tempMj ) ) return true;
	//if( IsSevenPairs( tempMj ) )  return true;
	return false;
}

//ʮ����
bool CGameLogic::IsThirteenOne( BYTE tempMj[] )
{
	//ʮ����
	int thirteenJpos = 0;
	for( int i=0; i<13; i++ )
	{
		if( IsJiang( tempMj[i], tempMj[i+1] ) )
		{
			thirteenJpos = i;
			for( int j=0; j<14; j++ )
			{
				if( j <= thirteenJpos )
				{
					if( tempMj[j] != mjThirteen[j] )
					{
						break;
					}
				}
				else if( j>thirteenJpos+1)
				{
					if( tempMj[j] != mjThirteen[j-1] )
					{
						break;
					}
				}
				if( j == 12 )
				{
					//MessageBox( NULL, "ʮ����", 0, 0 );
					return true;
				}
			}
			break;
		}
	}
	return false;
}

//�߶���
bool CGameLogic::IsSevenPairs( BYTE tempMj[] )
{
	//�߶�
	int pair=0;
	for( int i=0; i<HAND_MJ_MAX-1; i+=2 )
	{
		if( IsJiang( tempMj[i], tempMj[i+1] ) )
		{
			pair ++;
		}
		else
		{
			break;
		}
		if( pair == 7 )
		{
			MessageBox( NULL, "�߶���", "kk", MB_OK );
			return true;
		}
	}
	return false;
}

//������ +9��
bool CGameLogic::IsNineLight( BYTE tempMj[], BYTE byMj )
{
	BYTE mjType = GetMjType( tempMj[0]);
	for( int i=0; i<HAND_MJ_MAX-1; i++ )
	{
		if( GetMjType(tempMj[i]) != mjType)
			return false;
		if( GetMjValue(tempMj[i]) != mjNineLight[i] )
			return false;
	}
	return true;
}

/////////////////��������/////////////////////////////////////////////////////////////////

//Ͷ����,�������
int CGameLogic::ActionDice()
{
	srand( GetTickCount() );
	int pos=rand()%6;
	return pos;
}
//�����齫,0ֵ����,��������Ϊ0
bool CGameLogic::LMove( BYTE byMjs[], BYTE byRestCount )
{
	int start;
	if( byMjs[HAND_MJ_MAX-1] == 0 )
	{
		start = HAND_MJ_MAX-byRestCount-1;
	}
	else
	{
		start = HAND_MJ_MAX-byRestCount;
	}
	for( int i=start; i<HAND_MJ_MAX; i++ )
	{
		if( byMjs[i] == 0 )
		{
			//����Ч��,���һ��,������
			if( i == HAND_MJ_MAX-1 )
			{
				return true;
			}
			else
			{
				for( int j=i; j<HAND_MJ_MAX; j++ )
				{
					byMjs[j] = byMjs[j+1];
				}
				//ĩֵ��Ϊ0
				byMjs[HAND_MJ_MAX-1] = 0;
				return true;
			}
		}
	}
	return false;
}

//�����齫, 0ֵ��
bool CGameLogic::RMove( BYTE byMjs[], BYTE byRestCount, BYTE byMoveCount )
{
	std::vector< BYTE > temp;
	int pos = 0;
	int start;
	if( byMjs[HAND_MJ_MAX-1] == 0 )
	{
		start = HAND_MJ_MAX-byRestCount-1;
	}
	else
	{
		start = HAND_MJ_MAX-byRestCount;
	}
	for( int i=start; i<HAND_MJ_MAX; i++ )
	{
		if( byMjs[i] != 0 )
		{
			temp.push_back( byMjs[i] );
		}
	}
	//��0
	for( int j=start; j<start+byMoveCount; j++ )
	{
		byMjs[j] = 0;
	}
	for( int k=start+byMoveCount; k<HAND_MJ_MAX; k++ )
	{
		if((pos+1) >(int)temp.size())
			return true;
		byMjs[k] = temp[pos++];
		if( k == HAND_MJ_MAX-1 )
			return true;
	}
	return false;
	//for( int i=HAND_MJ_MAX; i>byRestCount; i-- )
	//{
	//	if( byMjs[i] == 0 )
	//	{
	//		for( int j=i; j>pos; j-- )
	//		{
	//			byMjs[j] = byMjs[j-byMoveCount];
	//			if( byMjs[j] == 0 )
	//				break;
	//			if( j ==
	//			//
	//		}
	//	}
	//}
}

//�����齫
void CGameLogic::InsertMjList( BYTE byMjList[], BYTE byRestCount  )
{
	int start;
	if( byRestCount == HAND_MJ_MAX )
	{
		start = HAND_MJ_MAX-byRestCount;
	}
	else
	{
		start = HAND_MJ_MAX-byRestCount-1;
	}
	//��⵽�����ڶ�����,���ƺ���в������
	for( int i=start; i<HAND_MJ_MAX-2; i++ )
	{
		//�����ڶ����Ʋ�Ϊ���
		if( byMjList[i] > byMjList[HAND_MJ_MAX-2] )
		{
			BYTE temp = byMjList[HAND_MJ_MAX-2];
			for( int j=HAND_MJ_MAX-2; j>i; j-- )
			{
				byMjList[j] = byMjList[j-1];
			}
			byMjList[i] = temp;
			break;
		}
	}
	//LMove( byMjList, HAND_MJ_MAX, byRestCount );
}

//�齫ֵ��ͼλת��
bool CGameLogic::TransMjDToI( BYTE srcMjData, BYTE *desMjData )
{
	BYTE type,buffer;
	type = srcMjData&LOGIC_MASK_TYPE;
	switch( type )
	{
	case 0:				//��
		{
			buffer = srcMjData;
			CopyMemory( desMjData, &buffer, sizeof( buffer ) );

			return true;
		}		
	case 16:			//��
		{
			buffer = 9+( srcMjData&LOGIC_MASK_VALUE );
			CopyMemory( desMjData, &buffer, sizeof( buffer ) );
			return true;
		}
	case 32:			//��
		{
			buffer = 18+( srcMjData&LOGIC_MASK_VALUE );
			CopyMemory( desMjData, &buffer, sizeof( buffer ) );
			return true;
		}		
	case 48:			//��
		{
			buffer = 27+( srcMjData&LOGIC_MASK_VALUE );
			CopyMemory( desMjData, &buffer, sizeof( buffer ) );
			return true;
		}
	case 64:			//��
		{
			buffer = 31+( srcMjData&LOGIC_MASK_VALUE );
			CopyMemory( desMjData, &buffer, sizeof( buffer ) );
			return true;
		}
	case 80:			//��
		{
			buffer = 34+( srcMjData&LOGIC_MASK_VALUE );
			CopyMemory( desMjData, &buffer, sizeof( buffer ) );
			//MessageBox( "����" );
			return true;
		}
	}
	return false;
}

bool CGameLogic::TransMjDToI( BYTE srcMjData[], BYTE *desMjData[], BYTE desCount )
{
	for( int i=0; i<desCount; i++)
	{
		if( !TransMjDToI( srcMjData[i], desMjData[i] ) )
			return false;
	}
	return true;
}
//�齫ֵ��ͼλת��
bool CGameLogic::TransMjIToD( BYTE srcMjData, BYTE *desMjData )
{
	BYTE buffer;
	if( srcMjData <= 9 )
	{
		buffer = srcMjData;
		CopyMemory( desMjData, &buffer, sizeof( buffer ) );
		return true;
	}
	else if( srcMjData <= 18 )
	{
		buffer = srcMjData-9+16;
		CopyMemory( desMjData, &buffer, sizeof( buffer ) );
		return true;
	}
	else if( srcMjData <= 27 )
	{
		buffer = srcMjData-18+32;
		CopyMemory( desMjData, &buffer, sizeof( buffer ) );
		return true;
	}
	else if( srcMjData <= 31 )
	{
		buffer = srcMjData-27+48;
		CopyMemory( desMjData, &buffer, sizeof( buffer ) );
		return true;
	}
	else if( srcMjData <= 34 )
	{
		buffer = srcMjData-31+64;
		CopyMemory( desMjData, &buffer, sizeof( buffer ) );
		return true;
	}
	else
	{
		buffer = srcMjData-34+80;
		CopyMemory( desMjData, &buffer, sizeof( buffer ) );
		return true;
	}
	return false;
}

//������
long CGameLogic::Pow( long x, long y )
{
	long r = 1;
	for( int i=0; i<y; i++ )
	{ 
		r *= x;
	}
	return r;
}
//�ŷ����
BYTE CGameLogic::TransIDToFeng( WORD wChairID, WORD wBanker )
{
	BYTE feng= (BYTE)(wChairID+wBanker);
	if( feng>GAME_PLAYER-1)
	{
		feng -= 4; 
	}
	return feng;
}
//����鵽���ƽṹ
void CGameLogic::AddBlock( HU *hu, PTR_MJ_BLOCK block )
{
	for( int i=0; i<(HAND_MJ_MAX-2)/3; i++ )
	{
		if( hu->block[i].style == BLOCK_NULL )
		{
			hu->block[i].first = block->first;
			hu->block[i].style = block->style;
			break;
		}
	}
}

//�����������
void CGameLogic::CleanHu( HU *hu )
{
	hu->jiangMj = 0;
	for( int i=0; i<(HAND_MJ_MAX-2)/3; i++ )
	{
		hu->block[i].first = 0;
		hu->block[i].style = BLOCK_NULL;
	}
}

//������������
void CGameLogic::CopyHu( HU *destHu, HU *srcHu )
{
	destHu->jiangMj = srcHu->jiangMj;
	for( int i=0; i<(HAND_MJ_MAX-2)/3; i++ )
	{
		destHu->block[i].first = srcHu->block[i].first;
		destHu->block[i].style = srcHu->block[i].style;
	}
}

//������������,���ط���
long CGameLogic::CheckGDHu( HU hu, BYTE byFengWei, BYTE byQuanWei, char *strFang)
{
	memset( strFang, 0, strlen(strFang));
	long nFangCount = 0;
	int nSanYuanCount ;			//��Ԫ����
	int nFengPaiCount ;			//������
	int nKePaiCount;			//�������� ֵΪ�ģ�Ϊ�Զ���
	bool bSameMjType = true;	//��һɫ����
	BYTE byMjType = 255;			//ͬɫ�齫����

	//for( int i=0; i<(HAND_MJ_MAX-2)/3; i++ )
	{
		//����Ϊ����
		if( GetMjType( hu.jiangMj )== 3 )
		{
			nFengPaiCount = 0;		//������
			nSanYuanCount = 0;
			nKePaiCount = 0;
			//��һɫ,С��ϲ������Ԫ����һɫ,������ƽ�͡��Զ���,��������
			for( int j=0; j<(HAND_MJ_MAX-2)/3; j++ )
			{	
				//����
				if( hu.block[j].first>0x30 )
				{
					nKePaiCount += 1;
					//��Ԫ��+1��
					if( hu.block[j].first > 0x34 )
					{
						nSanYuanCount += 1;
						nFangCount += 1;
						strcat( strFang, "��Ԫ��  +1��     ");
					}
					//��λ��Ȧλ��+1��
					else if( GetMjValue(hu.block[j].first)==byFengWei+1)
					{
						nFengPaiCount += 1;
						nFangCount += 1;
						strcat( strFang, "�ŷ��� +1��     ");
						if( GetMjValue(hu.block[j].first)== byQuanWei+1)
						{
							nFangCount += 1;
							strcat( strFang, "Ȧ���� +1��     ");
						}
					}
					else if( GetMjValue(hu.block[j].first)==byQuanWei+1)
					{
						nFengPaiCount += 1;
						nFangCount += 1;
						strcat( strFang, "Ȧ���� +1��     ");
						if( hu.block[j].first==byFengWei+1)
						{
							nFangCount += 1;
							strcat( strFang, "�ŷ��� +1��     ");
						}
					}
					else
					{
						nFengPaiCount += 1;
					}
				}
				//��һɫ,��������,ƽ�Ϳ���
				else if( hu.block[j].style<BLOCK_PENG )
				{
					if( byMjType == 255)
					{
						byMjType = GetMjType(hu.block[j].first);
					}
					else
					{
						if( GetMjType(hu.block[j].first)!=byMjType)
						{
							//��һɫ/��һɫ��
							bSameMjType = false;
						}
					}
				}
				//�Զ���,��������
				else
				{
					nKePaiCount += 1;
					if( byMjType == 255)
					{
						byMjType = GetMjType(hu.block[j].first);
					}
					else
					{
						if( GetMjType(hu.block[j].first)!=byMjType)
						{
							//��һɫ/��һɫ��
							bSameMjType = false;
						}
					}
				}
			}	
			//����Ԫ, 9��
			if( nSanYuanCount == 3)
			{
				nFangCount = 9;
				strcpy( strFang, "����Ԫ  +9��     ");
				return nFangCount;
			}
			//��һɫ,	8��
			if( nSanYuanCount+nFengPaiCount == 4 )
			{
				nFangCount = 8;
				strcpy( strFang, "��һɫ +8��      ");
				return nFangCount;
			}
			//С��ϲ, 7��
			if( nFengPaiCount== 3 )
			{
				//С��ϲ��, 7��
				nFangCount = 7;
				strcpy( strFang, "С��ϲ +7��     ");
				return nFangCount;
			}
			//����С�����ͣ���������������һɫ��ƽ�͡�����
			//������
			if( nKePaiCount==4 )
			{
				//����, +5��
				if( bSameMjType==true )
				{
					nFangCount += 5;
					strcat( strFang, "��һɫ�Զ��� +5��     ");
				}
				//�Զ���
				else
				{
					nFangCount += 3;
					strcat( strFang, "�Զ��� +3��     ");
				}
			}
			else
			{	
				//��һɫ,+3��
				if( bSameMjType)
				{
					nFangCount += 3;
					strcat( strFang,"��һɫ +3��     ");
				}
				else
				{
					//ƽ�ͣ�+1��
					if( nKePaiCount==0 )
					{
						nFangCount += 1;
						strcat( strFang, "ƽ�� +1��     " );
					}
					//������0
					else
					{
						if( nFangCount== 0 )
							strcpy( strFang, "���� +0��     " );
					}
				}
			}
		}
		//����Ϊ��Ԫ��
		if( GetMjType( hu.jiangMj ) == 4 )
		{
			nSanYuanCount = 0;	//��Ԫ����
			nFengPaiCount = 0;	//����
			nKePaiCount   = 0;
			bSameMjType   = true;
			byMjType	  = 255;
			//��һɫ, С��Ԫ����
			 for( int j=0; j<(HAND_MJ_MAX-2)/3; j++)
			 {
				 //����
				 if( hu.block[j].first>0x30 )
				 {
					 nKePaiCount += 1;
					 //��Ԫ��+1��
					 if( hu.block[j].first > 0x34 )
					 {
						 nSanYuanCount += 1;
						 nFangCount += 1;
						 strcat( strFang, "��Ԫ�� +1��     " );
					 }
					 //��λ��Ȧλ��+1��
					 else if( GetMjValue(hu.block[j].first)==byFengWei+1)
					 {
						 nFengPaiCount += 1;
						 nFangCount += 1;
						 strcat( strFang, "�ŷ��� +1��     ");
						 if( GetMjValue(hu.block[j].first)== byQuanWei+1)
						 {
							 nFangCount += 1;
							 strcat( strFang, "Ȧ���� +1��     ");
						 }
					 }
					 else if( GetMjValue(hu.block[j].first)==byQuanWei+1)
					 {
						 nFengPaiCount += 1;
						 nFangCount += 1;
						 strcat( strFang, "Ȧ���� +1��     ");
						 if( GetMjValue(hu.block[j].first)==byFengWei+1)
						 {
							 nFangCount += 1;
							 strcat( strFang, "�ŷ��� +1��     ");
						 }
					 }
					 else
					 {
						 nFengPaiCount += 1;
					 }
					 ////��λ��Ȧλ��+1��
					 //if( hu.block[j].first==byFengWei || hu.block[j].first==byQuanWei )
					 //{
						// nFangCount += 1;
						// //����Ȧλ�����Ƿ�λ���ټ�һ��
						// if( hu.block[j].first==byFengWei&&hu.block[j].first==byQuanWei)
						// {
						//	 nFangCount += 1;
						// }
					 //}
				 }
				 //��һɫ,��������,ƽ�Ϳ���
				 else if( hu.block[j].style<BLOCK_PENG )
				 {
					 if( byMjType == 255)
					 {
						 byMjType = GetMjType(hu.block[j].first);
					 }
					 else
					 {
						 if( GetMjType(hu.block[j].first)!=byMjType)
						 {
							 //��һɫ/��һɫ��
							 bSameMjType = false;
						 }
					 }
				 }
				 //�Զ���,��������
				 else
				 {
					 nKePaiCount += 1;
					 if( byMjType == 255)
					 {
						 byMjType = GetMjType(hu.block[j].first);
					 }
					 else
					 {
						 if( GetMjType(hu.block[j].first)!=byMjType)
						 {
							 //��һɫ/��һɫ��
							 bSameMjType = false;
						 }
					 }
				 }
			 }
			 //����ϲ, 9��
			 if( nFengPaiCount==4 )
			 {
				 nFangCount = 9;
				 strcpy( strFang, "����ϲ +9��     " );
				 return nFangCount;
			 }
			 //��һɫ, 8��
			 if( nFengPaiCount+nSanYuanCount==4)
			 {
				 nFangCount = 8;
				 strcpy( strFang, "��һɫ +8��     " );
				 return nFangCount;
			 }
			 //С��Ԫ, 7��
			 if( nSanYuanCount==2)
			 {
				 nFangCount = 7;
				 strcpy( strFang, "С��Ԫ +7��     " );
				 return nFangCount;
			 }
			 //����С�����ͣ���������������һɫ��ƽ�͡�����
			 //������
			 if( nKePaiCount==4 )
			 {
				 //����, +5��
				 if( bSameMjType==true )
				 {
					 nFangCount += 5;
					 strcat( strFang, "��һɫ�Զ��� +5��     " );
				 }
				 //�Զ���, +3��
				 else
				 {
					 nFangCount += 3;
					 strcat( strFang, "�Զ��� +3��     " );
				 }
			 }
			 else
			 {
				 //��һɫ,+3��
				 if( bSameMjType)
					{
						nFangCount += 3;
						strcat( strFang,"��һɫ +3��     ");
					}
				 else
					{
						//ƽ�ͣ�+1��
						if( nKePaiCount==0 )
						{
							nFangCount += 1;
							strcat( strFang, "ƽ�� +1��     " );
						}
						//������0
						else
						{
							if( nFangCount== 0 )
								strcpy( strFang, "���� +0��     " );
						}
					}	strcpy( strFang, "���� +0��     " );
				}
			 }
			 //ƽ��, +1��

			 ////��һɫ
			 //for( int k=0; k<(HAND_MJ_MAX-2)/3-1; k++ )
			 //{
				// if( GetMjType(hu.block[k].first) != GetMjType(hu.block[k].first) )
				//	 break;
				// else
				// {
				//	 //��һɫ
				//	 if( k == (HAND_MJ_MAX-2)/3-2)
				//	 {
				//		 //�Ƿ����
				//		 for( int l=0; l<(HAND_MJ_MAX-2)/3; l++)
				//		 {
				//			 if( hu.block[l].style<BLOCK_PENG ) 
				//				 break;
				//			 else
				//			 {
				//				 //��һɫ�Զ���
				//				 if( l == (HAND_MJ_MAX-2)/3-2)
				//				 {
				//					 nFangCount = 5;
				//					 return nFangCount;
				//				 }
				//			 }
				//		 }
				//		 //��һɫ��  3��
				//		 nFangCount += 3;
				//	 }
				// }
			 //}
			 ////�Զ���
			 //for( int j=0; j<(HAND_MJ_MAX-2)/3; j++ )
			 //{
				// if( hu.block[j].style<BLOCK_PENG )
				// {
				//	 break;
				// }
				// else
				// {
				//	 //�Զ���
				//	 if( j==(HAND_MJ_MAX-2)/3-1 )
				//	 {
				//		 nFangCount += 3;
				//	 }
				// }
			 //}
		//}
		////�۾���
		//if( GetMjValue(hu.jiangMj)==1 || GetMjValue(hu.jiangMj)==9 )
		//{
		//	//
		//	for( int i=0; i<(HAND_MJ_MAX-2)/3; i++ )
		//	{
		//		//��Ϊ��,��,����
		//		if(hu.block[i].style<BLOCK_PENG)
		//		{
		//			break;
		//		}
		//		//��Ϊ�۾�,����
		//		else if( GetMjValue(hu.block[i].first)!=1 && hu.block[i].first!=9 )
		//		{
		//			break;
		//		}
		//		else
		//		{
		//			//�۾���
		//			if( i == (HAND_MJ_MAX-2)/3-1)
		//			{
		//				for( int j=0; j<(HAND_MJ_MAX-2)/3-1; j++ )
		//				{
		//					//ͬһ��ɫ
		//					if( GetMjType(hu.block[j].first)!=GetMjType(hu.block[j].first) )
		//					{
		//						break;
		//					}
		//					//���۾���,8��
		//					if( j==(HAND_MJ_MAX-2)/3-2)
		//					{
		//						nFangCount = 8;
		//						return nFangCount;
		//					}
		//				}
		//				//���۾���, 6��
		//				nFangCount = 6;
		//				return nFangCount;
		//			}
		//		}
		//	}
		//}
		//��������	
		else
		{	
			//����ʼ������
			nSanYuanCount = 0;	//��Ԫ����
			nFengPaiCount = 0;	//����
			nKePaiCount   = 0;	//����
			byMjType = GetMjType( hu.jiangMj );
			bSameMjType = true;
			//�۾���
			if( GetMjValue(hu.jiangMj)==1 || GetMjValue(hu.jiangMj)==9 )
			{
				//
				for( int i=0; i<(HAND_MJ_MAX-2)/3; i++ )
				{
					//��Ϊ��,��,����
					if(hu.block[i].style<BLOCK_PENG)
					{
						break;
					}
					//��Ϊ�۾�,����
					else if( GetMjValue(hu.block[i].first)!=1 && hu.block[i].first!=9 )
					{
						break;
					}
					else
					{
						//�۾���
						if( i == (HAND_MJ_MAX-2)/3-1)
						{
							for( int j=0; j<(HAND_MJ_MAX-2)/3; j++ )
							{
								//ͬһ��ɫ
								if( GetMjType(hu.block[j].first)!=byMjType )
								{
									break;
								}
								//���۾���,8��
								if( j==(HAND_MJ_MAX-2)/3-1)
								{
									nFangCount = 8;
									strcpy( strFang, "���۾� +8��     " );
									return nFangCount;
								}
							}
							//���۾���, 6��
							nFangCount = 6;
							strcpy( strFang, "���۾� +6��     " );
							return nFangCount;
						}
					}
				}
			}

			//����ϲ������Ԫ����һɫ����������һɫ��������������ƽ�͡��Զ���,��������
			for( int j=0; j<(HAND_MJ_MAX-2)/3; j++ )
			{	
				//����
				if( hu.block[j].first>0x30 )
				{
					nKePaiCount += 1;
					//��Ԫ��+1��
					if( hu.block[j].first > 0x34 )
					{
						nSanYuanCount += 1;
						nFangCount += 1;
						strcat( strFang, "��Ԫ�� +1��     " );
					}
					//��λ��Ȧλ��+1��
					else if( GetMjValue(hu.block[j].first)==byFengWei+1)
					{
						nFengPaiCount += 1;
						nFangCount += 1;
						strcat( strFang, "�ŷ��� +1��     ");
						if( GetMjValue(hu.block[j].first)== byQuanWei+1)
						{
							nFangCount += 1;
							strcat( strFang, "Ȧ���� +1��     ");
						}
					}
					else if( GetMjValue(hu.block[j].first)==byQuanWei+1)
					{
						nFengPaiCount += 1;
						nFangCount += 1;
						strcat( strFang, "Ȧ���� +1��     ");
						if( GetMjValue(hu.block[j].first)==byFengWei+1)
						{
							nFangCount += 1;
							strcat( strFang, "�ŷ��� +1��     ");
						}
					}
					else
					{
						nFengPaiCount += 1;
					}
					////��λ��Ȧλ��+1��
					//if( hu.block[j].first==byFengWei || hu.block[j].first==byQuanWei )
					//{
					//	nFangCount += 1;
					//	//����Ȧλ�����Ƿ�λ���ټ�һ��
					//	if( hu.block[j].first==byFengWei&&hu.block[j].first==byQuanWei)
					//	{
					//		nFangCount += 1;
					//	}
					//}
				}
				//��һɫ,��������,ƽ�Ϳ���
				else if( hu.block[j].style<BLOCK_PENG )
				{
					if( GetMjType(hu.block[j].first)!=byMjType)
					{
						//��һɫ/��һɫ��
						bSameMjType = false;
					}
				}
				//�Զ���,��������
				else
				{
					nKePaiCount += 1;
					if( GetMjType(hu.block[j].first)!=byMjType)
					{
						//��һɫ/��һɫ��
						bSameMjType = false;
					}
				}
			}	
			//����Ԫ, 9��
			if( nSanYuanCount == 3)
			{
				nFangCount = 9;
				strcpy( strFang, "����Ԫ +9��     " );
				return nFangCount;
			}
			//����ϲ, 9��
			if( nFengPaiCount==4)
			{
				nFangCount = 9;
				strcpy( strFang, "����ϲ +9��     " );
				return nFangCount;
			}
			//��һɫ����������һɫ������
			if( bSameMjType== true)
			{
				//��һ
				if( nFengPaiCount==0 && nSanYuanCount==0)
				{
					//������ 6��
					if( nKePaiCount==4)
					{
						nFangCount = 6;
						strcpy( strFang, "��һɫ�Զ��� +6��     " );
						return nFangCount;
					}
					//��һɫ��+5��
					else
					{
						nFangCount += 5;
						strcat( strFang, "��һɫ +5��     " );
					}
				}
				//��һ
				else
				{
					//����,+5��
					if( nKePaiCount== 4 )
					{
						nFangCount += 5;
						strcat( strFang, "��һɫ�Զ��� +5��     " );
					}
					//��һɫ, +3��
					else
					{
						nFangCount += 3;
						strcat( strFang, "��һɫ +3��     " );
					}
				}
			}
			else 
			{
				//���ͣ� +3��
				if( nKePaiCount==4 )
				{
					nFangCount += 3;
					strcat( strFang, "�Զ��� +3��     " );
				}
				else
				{
					//��һɫ,+3��
					if( bSameMjType)
					{
						nFangCount += 3;
						strcat( strFang,"��һɫ +3��     ");
					}
					else
					{
						//ƽ�ͣ�+1��
						if( nKePaiCount==0 )
						{
							nFangCount += 1;
							strcat( strFang, "ƽ�� +1��     " );
						}
						//������0
						else
						{
							if( nFangCount== 0 )
								strcpy( strFang, "���� +0��     " );
						}
					}

				}
			}

		//	//ȫ��������ϲ������Ԫ������������������	
		//	for( int i=0; i<(HAND_MJ_MAX-2)/3; i++ )
		//	{
		//		if( hu.block[i].style < BLOCK_PENG )
		//		{
		//			break;
		//		}
		//		else
		//		{
		//			//����
		//			if( GetMjValue( hu.block[i].first ) > 0x30 )
		//			{
		//				if( GetMjValue(hu.block[i].first)>0x40 )
		//				{
		//					nSanYuanCount += 1;
		//				}
		//				else
		//				{
		//					nFengPaiCount += 1;
		//				}
		//			}
		//			else
		//			{
		//				//��ͨ�Զ���
		//				if( bSameMjType && GetMjType( hu.block[i].first) != byMjType)
		//				{
		//					bSameMjType = false;
		//				}
		//			}
		//		}
		//		if( i == (HAND_MJ_MAX-2)/3 )
		//		{
		//			//����ϲ, �ŷ�
		//			if( nFengPaiCount == 4)
		//			{
		//				nFangCount = 9;
		//				return nFangCount;
		//			}
		//			//����Ԫ�� 9��
		//			if( nSanYuanCount == 3 )
		//			{
		//				nFangCount = 9; 
		//				return nFangCount;
		//			}
		//			//����, ����
		//			if( bSameMjType )
		//			{
		//				//��һɫ�Զ���, 6��
		//				if( nFengPaiCount== 0 && nSanYuanCount==0 )
		//				{
		//					nFangCount = 6;
		//					return nFangCount;
		//				}
		//				//��һɫ�Զ����� 5��
		//				else
		//				{
		//					nFangCount +=5;
		//				}
		//			}
		//			//�Զ����� 3��
		//			else
		//			{
		//				nFangCount += 3;
		//			}
		//		}
		//	}
		//}
		}
	}
	
	if( nFangCount >5 )
	{
		strcat( strFang, "     ����   ");
		nFangCount = 5;
	}
	return nFangCount;
}
////////////////////////////////����
BLOCK CGameLogic::Peng( BYTE byMjs[], BYTE byRestCount, BYTE byMj )
{
	MJ_BLOCK block;
	for( int i=HAND_MJ_MAX-1-byRestCount; i<HAND_MJ_MAX-1; i++ )
	{
		if( byMjs[i] == byMj )
		{
			byMjs[i] = 0;
			byMjs[i+1] = 0;
			//����3��
			break;
		}
	}
	block.first = byMj;
	block.style = BLOCK_PENG;
	return block;
}

//�ܲ���
BLOCK CGameLogic::MGang( BYTE byMjs[], BYTE byRestCount, BYTE byMj )
{
	BLOCK block;
	block.style= BLOCK_NULL;
	block.first= -1;
	for( int i=HAND_MJ_MAX-1-byRestCount; i<HAND_MJ_MAX-3; i++ )
	{
		if( byMjs[i] == byMj )
		{
			byMjs[i] = 0;
			byMjs[i+1] = 0;
			byMjs[i+2] = 0;
			block.style= BLOCK_GANG;
			block.first= byMj;
			//����4��
			return block;
		}
	}
	return block;
}

//��������-����
void CGameLogic::ZMGang( BYTE byMjs[], BYTE byRestCount, HU *hu, BYTE GangMj )
{
	for( int i=0; i<(HAND_MJ_MAX-2)/3; i++)
	{
		if( hu->block[i].first== GangMj)
		{
			hu->block[i].style= BLOCK_GANG;
			break;
			//����һ��
		}
	}
	for( int j=0; j<HAND_MJ_MAX-1; j++ )
	{
		if( byMjs[j] == GangMj)
		{
			byMjs[j] = 0;
			break;
		}
	}
}

//����
BLOCK CGameLogic::AnGang( BYTE byMjs[], BYTE byRestCount, BYTE GangMj )
{
	BLOCK block;
	for( int i=HAND_MJ_MAX-byRestCount; i<HAND_MJ_MAX-1; i++ )
	{
		if( byMjs[i] == GangMj)
		{
			byMjs[i] = 0;
		}
	}
	block.style= BLOCK_AN_GANG;
	block.first= GangMj;
	return block;
}
//���Ʋ���
BLOCK CGameLogic::Chi( BYTE byMjs[], BYTE byRestCount, BYTE byMj, int ChiMode )
{
	MJ_BLOCK block;
	block.first = 0;
	block.style = BLOCK_NULL;
	switch( ChiMode )
	{
	case 1:				//@@*
		{
			block.first = byMj-2;
			block.style = BLOCK_CHI;
			//�����齫
			for( int i=HAND_MJ_MAX-1-byRestCount; i<HAND_MJ_MAX-1; i++ )
			{
				if( byMjs[i] == byMj-2 )
				{
					byMjs[i] = 0;
					for( int j=i; j<HAND_MJ_MAX-1; j++)
					{
						if( byMjs[j]==byMj-1)
						{
							byMjs[j] = 0;
							return block;
						}
					}
					break;
				}
			}
		}
		break;
	case 2:					//@*@
		{
			block.first = byMj-1;
			block.style = BLOCK_CHI;
			//�����齫
			for( int i=HAND_MJ_MAX-1-byRestCount; i<HAND_MJ_MAX-1; i++ )
			{
				if( byMjs[i] == byMj-1 )
				{
					byMjs[i] = 0;
					for( int j=i; j<HAND_MJ_MAX-1; j++)
					{
						if( byMjs[j]==byMj+1)
						{
							byMjs[j] = 0;
							return block;
						}
					}
					break;
				}
			}
		}
		break;
	case 4:				//*@@
		{
			block.first = byMj;
			block.style = BLOCK_CHI;
			//�����齫
			for( int i=HAND_MJ_MAX-1-byRestCount; i<HAND_MJ_MAX-1; i++ )
			{
				if( byMjs[i] == byMj+1 )
				{
					byMjs[i] = 0;
					for( int j=i; j<HAND_MJ_MAX-1; j++)
					{
						if( byMjs[j]==byMj+2 )
						{
							byMjs[j] = 0;
							return block;
						}
					}
					break;
				}
			}
		}
		break;
	}
	return block;
}