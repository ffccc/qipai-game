#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//��̬����

//�˿�����
const BYTE	CGameLogic::m_bCardListData[108]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	    //���� A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	    //÷�� A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	    //���� A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	    //���� A - K
	0x41,0x42,                                                              //С��������
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	    //���� A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	    //÷�� A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	    //���� A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	    //���� A - K
	0x41,0x42                                                               //С��������
};

//��ʼ����
BYTE CGameLogic::m_bSeries=0x22;


//////////////////////////////////////////////////////////////////////////

//���캯��
CGameLogic::CGameLogic()
{
}

//��������
CGameLogic::~CGameLogic()
{

}

//��ȡ����
short CGameLogic::GetCardType(BYTE cbCardData[], BYTE cbCardCount)
{
	//Ԥ���ж�
	if(cbCardCount>5)
		if(GetCardColor(cbCardData[0])==0x40)	  return	CT_ERROR;
	
	//�����ж�
	switch(cbCardCount)
	{ 
	case 1:
		 m_bValue=GetCardLogicValue(cbCardData[0]);
												  return	((short(m_bValue)<<8)|CT_SINGLE);
		 break;
	case 2:
	     if (IsDouble(cbCardData,cbCardCount))    return    ((short(m_bValue)<<8)|CT_DOUBLE);
		 break;
	case 3:
		if (IsSanTiao(cbCardData,cbCardCount))    return    ((short(m_bValue)<<8)|CT_THREE_TIAO);
	    break;
	case 4:
		if(IsFourKing(cbCardData,cbCardCount))    return    ((short(m_bValue)<<8)|CT_FOUR_KING);
		if(IsSiZhangBomb(cbCardData,cbCardCount)) return    ((short(m_bValue)<<8)|CT_SI_ZHANG_BOMB);
		break;
	case 5:
		if(IsWuZhangBomb(cbCardData,cbCardCount)) return    ((short(m_bValue)<<8)|CT_WU_ZHANG_BOMB);
		if(IsHuLu(cbCardData,cbCardCount))        return    ((short(m_bValue)<<8)|CT_HU_LU);    
		if (IsTongHuaShun(cbCardData,cbCardCount))return    ((short(m_bValue)<<8)|CT_TONG_HUA_SHUN);
	    if (IsShunZhi(cbCardData,cbCardCount))    return    ((short(m_bValue)<<8)|CT_SHUN_ZI);
		break;
	case 6:
		if(IsLiuZhangBomb(cbCardData,cbCardCount))return    ((short(m_bValue)<<8)|CT_LIU_ZHANG_BOMB);
		if(IsGangBan(cbCardData,cbCardCount))     return    ((short(m_bValue)<<8)|CT_GANG_BAN);
		if(IsLiangLianDui(cbCardData,cbCardCount))return    ((short(m_bValue)<<8)|CT_LIANG_LIAN_DUI);
		break;
	case 7:
		if(IsQiZhangBomb(cbCardData,cbCardCount)) return    ((short(m_bValue)<<8)|CT_QI_ZHANG_BOMB);
		break;
	case 8:
		if(IsBaZhangBomb(cbCardData,cbCardCount)) return    ((short(m_bValue)<<8)|CT_BA_ZHANG_BOMB);
		break;
	case 9:
		if(IsJiuZhangBomb(cbCardData,cbCardCount))return    ((short(m_bValue)<<8)|CT_JIU_ZHANG_BOMB);
		break;
	case 10:
		if(IsShiZhangBomb(cbCardData,cbCardCount))return    ((short(m_bValue)<<8)|CT_SHI_ZHANG_BOMB);
		break;
	}
	
	return CT_ERROR;
}
		
		

//�����˿�
void CGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount)
{
	//ת����ֵ
	BYTE cbLogicValue[MAX_COUNT];
	for (BYTE i=0;i<cbCardCount;i++) cbLogicValue[i]=GetCardLogicValue(cbCardData[i]);	

	//�������
	bool bSorted=true;
	BYTE cbTempData,bLast=cbCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<bLast;i++)
		{
			if ((cbLogicValue[i]<cbLogicValue[i+1])||
				(cbLogicValue[i]==cbLogicValue[i+1]&&cbCardData[i+1]==m_bSeries&&cbCardData[i]!=m_bSeries))
			{
				//����λ��
				cbTempData=cbCardData[i];
				cbCardData[i]=cbCardData[i+1];
				cbCardData[i+1]=cbTempData;
				cbTempData=cbLogicValue[i];
				cbLogicValue[i]=cbLogicValue[i+1];
				cbLogicValue[i+1]=cbTempData;
				bSorted=false;
			}	
		}
		bLast--;
	} while(bSorted==false);

	return;
}
//����ɫ��
void CGameLogic::SortByColor(BYTE  cbCardData[], BYTE cbCardCount)
{    
	//������
	SortCardList(cbCardData,cbCardCount);
	//ת����ֵ
	BYTE cbLogicValue[MAX_COUNT];
	for (BYTE i=0;i<cbCardCount;i++) cbLogicValue[i]=GetCardLogicValue(cbCardData[i]);	
	//�������
	bool bSorted=true;
	BYTE cbTempData,bLast=cbCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<bLast;i++)
		{
			if (((GetCardColor(cbCardData[i])<GetCardColor(cbCardData[i+1]))||
				(GetCardColor(cbCardData[i])==GetCardColor(cbCardData[i+1])&&cbLogicValue[i]<cbLogicValue[i+1]))
				&&cbCardData[i]!=m_bSeries)

			{
				//����λ��
				cbTempData=cbCardData[i];
				cbCardData[i]=cbCardData[i+1];
				cbCardData[i+1]=cbTempData;
				cbTempData=cbLogicValue[i];
				cbLogicValue[i]=cbLogicValue[i+1];
				cbLogicValue[i+1]=cbTempData;
				bSorted=false;
			}	
		}
		bLast--;
	} while(bSorted==false);
  return ;
}

//��ը����
void CGameLogic::SortByBomb	(BYTE  cbCardData[], BYTE cbCardCount)
{
  
  //˳������
  SortCardList(cbCardData,cbCardCount);
  if(cbCardCount<3) return ;
  //��ֵ��ͬ����
  BYTE   bTypeCount[15],bTypeStart[15];
  ZeroMemory(bTypeCount,sizeof(bTypeCount));

  BYTE bStart=0,bRound=0,bNext=0;
  
  while(bNext<cbCardCount)
  {
	  BYTE i=1;
	  bStart=bNext;
	  while(bNext<(cbCardCount-1)&&((
		  GetCardLogicValue(cbCardData[bNext])==GetCardLogicValue(cbCardData[bNext+1]))||
		  (GetCardColor(cbCardData[bNext])==0x40&&GetCardColor(cbCardData[bNext+1])==0x40)))
	  {
		  i++;
		  bNext++;
	  }
	  bTypeCount[bRound]=i;
	  bTypeStart[bRound]=bStart;
	  bRound++;
	  bNext++;
  }
  //��ʼ����
  bool bSorted;
  BYTE cbTempData,bLast=bRound-1;
  do{
      bSorted=false; 
	  for(BYTE i=0;i<bLast;i++)
	   {
		   if(bTypeCount[i]<bTypeCount[i+1])
		   {
			   cbTempData=bTypeCount[i];
			   bTypeCount[i]=bTypeCount[i+1];
			   bTypeCount[i+1]=cbTempData;

			   cbTempData=bTypeStart[i];
			   bTypeStart[i]=bTypeStart[i+1];
			   bTypeStart[i+1]=cbTempData;

			   bSorted=true;
		   }
	   }
	   bLast--;
  }while(bSorted);
  
  //��������
  BYTE  bTemp[27];
  CopyMemory(bTemp,cbCardData,cbCardCount);
  BYTE   bLocation=0;
  for(BYTE i=0;i<bRound;i++)
  {
	  for(BYTE j=0;j<bTypeCount[i];j++)
	  {
		  cbCardData[bLocation]=bTemp[bTypeStart[i]+j];
		  bLocation++;
	  }
  }

}

//�����˿�
void CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	//����׼��
	BYTE cbCardData[sizeof(m_bCardListData)];
	CopyMemory(cbCardData,m_bCardListData,sizeof(m_bCardListData));
	srand( (unsigned)time( NULL ));
	//�����˿�
	BYTE bRandCount=0,bPosition=0;
	do
	{
		bPosition=rand()%(cbBufferCount-bRandCount);
		cbCardBuffer[bRandCount++]=cbCardData[bPosition];
		cbCardData[bPosition]=cbCardData[cbBufferCount-bRandCount];
	} while (bRandCount<cbBufferCount);

	return;
}

//ɾ���˿�
bool CGameLogic::RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE cbCardData[], BYTE cbCardCount)
{
	//��������
	ASSERT(bRemoveCount<=cbCardCount);

	//�������
	BYTE bDeleteCount=0,bTempCardData[MAX_COUNT];
	if (cbCardCount>CountArray(bTempCardData)) return false;
	CopyMemory(bTempCardData,cbCardData,cbCardCount*sizeof(cbCardData[0]));
	//�����˿�
	for (BYTE i=0;i<bRemoveCount;i++)
	{
		for (BYTE j=0;j<cbCardCount;j++)
		{
			if (bRemoveCard[i]==bTempCardData[j])
			{
				bDeleteCount++;
				bTempCardData[j]=0;
				break;
			}
		}
	}
	if (bDeleteCount!=bRemoveCount) return false;

	//�����˿�
	BYTE bCardPos=0;
	for (BYTE i=0;i<cbCardCount;i++)
	{
		if (bTempCardData[i]!=0) cbCardData[bCardPos++]=bTempCardData[i];
	}

	return true;
}

//��Ч�ж�
bool CGameLogic::IsValidCard(BYTE cbCardData)
{
	//��������
	BYTE cbColor=GetCardColor(cbCardData);
	BYTE cbValue=GetCardValue(cbCardData);

	//��Ч�ж�
	if ((cbColor<=0x40)&&(cbValue>=0x01)&&(cbValue<=0x0D)) return true;

	return false;
}

//�߼���ֵ
BYTE CGameLogic::GetCardLogicValue(BYTE cbCardData)
{
	//�˿�����
	BYTE bCardColor=GetCardColor(cbCardData);
	BYTE bCardValue=GetCardValue(cbCardData);

	//ת����ֵ
    
	//����С��
    if(bCardColor==0x40)
		return (bCardValue+15);
	
	//�Ƿ�Ϊ��
	if(bCardValue==GetCardValue(m_bSeries))  return 15;
	
	//�Ƿ�A��
	if(bCardValue==1)          return 14;
	
	//������ֵ
	return bCardValue;
}

//�Ƿ����
bool CGameLogic::IsDouble(BYTE cbCardData[], BYTE cbCardCount)
{
	
	//��Ŀ�ж�
	m_bValue=GetCardLogicValue(cbCardData[1]);
	
	if(cbCardData[0]==m_bSeries)
		return true;
	
	//��������
	BYTE bNextValue=GetCardLogicValue(cbCardData[1]);
	BYTE bFirstValue=GetCardLogicValue(cbCardData[0]);
   

	return (bNextValue==bFirstValue);
}

//�Ƿ�����ը��
bool CGameLogic::IsSiZhangBomb(BYTE cbCardData[], BYTE cbCardCount)
{
	
	m_bValue=GetCardLogicValue(cbCardData[cbCardCount-1]);
	return (BombCount(cbCardData,cbCardCount)==4);
}

//�Ƿ��«
bool CGameLogic::IsHuLu(BYTE cbCardData[], BYTE cbCardCount)
{
	//��������
	BYTE bSeriesCount=0,j=0;
	//�������
	while(cbCardData[j]==m_bSeries)
	{
		bSeriesCount++;
		j++;
	} 
	
	//��������
	BYTE bFirstCount=1,bNextCount=1;
	
	//��������
	if(GetCardColor(cbCardData[0])==0x40)
	{
		if(cbCardData[1]!=cbCardData[0])
			return false;
		j++;
		bFirstCount++;
	}else{
		while(j<(cbCardCount-1)&&(GetCardValue(cbCardData[j])==GetCardValue(cbCardData[j+1])))
		{
			bFirstCount++;
			j++;
		}
	}
	if(bFirstCount>3) return false;
    if(j==4) return false;
	j++;
	while(j<(cbCardCount-1)&&(GetCardValue(cbCardData[j])==GetCardValue(cbCardData[j+1])))
	{
		bNextCount++;
		j++;
	}
	if(bNextCount>3) return false;
	if((bSeriesCount+bFirstCount+bNextCount)!=5) return false;
	if(bNextCount<3&&GetCardColor(cbCardData[0])!=0x40)
		m_bValue=GetCardLogicValue(cbCardData[2]);
	else
		m_bValue=GetCardLogicValue(cbCardData[4]);
          

	return true;
}

//�Ƿ�����ը��
bool CGameLogic::IsWuZhangBomb(BYTE cbCardData[], BYTE cbCardCount)
{
	if(GetCardColor(cbCardData[0])==0x40)	  return	false;
    m_bValue=GetCardLogicValue(cbCardData[cbCardCount-1]);
	return (BombCount(cbCardData,cbCardCount)==5);
}

	
//�Ƿ�����ը��
bool CGameLogic::IsLiuZhangBomb(BYTE cbCardData[], BYTE cbCardCount)
{
    m_bValue=GetCardLogicValue(cbCardData[cbCardCount-1]);
	return (BombCount(cbCardData,cbCardCount)==6);
} 
	
//�Ƿ�����ը��
bool CGameLogic::IsQiZhangBomb(BYTE cbCardData[], BYTE cbCardCount)
{
    m_bValue=GetCardLogicValue(cbCardData[cbCardCount-1]);
	return (BombCount(cbCardData,cbCardCount)==7);
} 
	
//�Ƿ����ը��
bool CGameLogic::IsBaZhangBomb(BYTE cbCardData[], BYTE cbCardCount)
{
    m_bValue=GetCardLogicValue(cbCardData[cbCardCount-1]);
	return (BombCount(cbCardData,cbCardCount)==8);
} 

//�Ƿ����ը��
bool CGameLogic::IsJiuZhangBomb(BYTE cbCardData[], BYTE cbCardCount)
{
    m_bValue=GetCardLogicValue(cbCardData[cbCardCount-1]);
	return (BombCount(cbCardData,cbCardCount)==9);
}

//�Ƿ�ʮ��ը��
bool CGameLogic::IsShiZhangBomb(BYTE cbCardData[], BYTE cbCardCount)
{
    m_bValue=GetCardLogicValue(cbCardData[cbCardCount-1]);
	return (BombCount(cbCardData,cbCardCount)==10);
} 

//�Ƿ�����
bool CGameLogic::IsFourKing(BYTE cbCardData[], BYTE cbCardCount)
{
	m_bValue=GetCardLogicValue(cbCardData[0]);
	//��������
	for(BYTE i=0;i<cbCardCount;i++)
		if(GetCardColor(cbCardData[i])!=0x40)
		   return false;
	
	return true;
}

//�Ƿ�˳��
bool CGameLogic::IsShunZhi(BYTE cbCardData[], BYTE cbCardCount)
{
	
	if(GetCardColor(cbCardData[0])==0x40)	  return	false;
	//��������
	BYTE bSeriesCount=0,j=0;
	BYTE bCardTemp[5];
	memcpy(bCardTemp,cbCardData,cbCardCount);

	//�������
	while(cbCardData[j]==m_bSeries&&j<cbCardCount)
	{
		bSeriesCount++;
		j++;
	} 

    BYTE bFirstValue=GetCardValue(cbCardData[j]);
	BYTE bLastValue;

	//˳�����
	if((bFirstValue==GetCardValue(m_bSeries))&&(bFirstValue!=0x01)){
		for( BYTE i=j+1;i<cbCardCount;i++)
		{
			BYTE bNextValue=GetCardValue(cbCardData[i]);
			if(bFirstValue<bNextValue||bNextValue==0x01){
				bCardTemp[i-1]=bCardTemp[i];
		        bCardTemp[i]=bFirstValue;
			}
		}
	}
    
	//��ȡ��ֵ
    bFirstValue=GetCardValue(bCardTemp[j]);
    bLastValue=GetCardValue(bCardTemp[4]);
    
	//��������
	BYTE cbValue;
	BYTE bTemp=0;
	if (bFirstValue==1)
	{   
	   if(bLastValue>0 && bLastValue<5)
	   {
		   m_bValue=5;
		   bTemp=5;
	   }
	   else 
	   {   
		   m_bValue=14;
		   bTemp=13;
	   }
    }
		
    //��ʼ�ж�
	for(BYTE i=j+1,m=1;i<cbCardCount;i++,m++)
	{
		cbValue=GetCardValue(bCardTemp[i]);
		while(cbValue!=(bFirstValue+bTemp-m))
		{
		    if(bSeriesCount<=0){
			   return false;
			 }
			 else{
					m=m+1;
					bSeriesCount--;
				 }
		}
     }

     //����ֵ����
	 if(bFirstValue!=1)
	    m_bValue=((bFirstValue+bSeriesCount)>14)?  14:(bFirstValue+bSeriesCount);

	return true;
}

//�Ƿ�����
bool CGameLogic::IsSanTiao(BYTE cbCardData[], BYTE cbCardCount)
{
	//��������
	
	m_bValue=GetCardLogicValue(cbCardData[cbCardCount-1]);
	return (BombCount(cbCardData,cbCardCount)==3);
}

//�Ƿ�ͬ��˳
bool CGameLogic::IsTongHuaShun(BYTE cbCardData[], BYTE cbCardCount)
{
	if(GetCardColor(cbCardData[0])==0x40)	  return	false;
	//��������
	BYTE bSeriesCount=0,j=0;
	BYTE bCardTemp[5];
	memcpy(bCardTemp,cbCardData,cbCardCount);
	
	//�������
	while(cbCardData[j]==m_bSeries&&j<cbCardCount)
	{
		bSeriesCount++;
		j++;
	} 
	
	//��������
	BYTE bFirstValue=GetCardValue(cbCardData[j]);
    BYTE bLastValue;
	BYTE bTempCard=cbCardData[j];

	//˳�����
	if((bFirstValue==GetCardValue(m_bSeries))&&(bFirstValue!=0x01)){
		for( BYTE i=j+1;i<cbCardCount;i++)
		{
			BYTE bNextValue=GetCardValue(cbCardData[i]);
			if(bFirstValue<bNextValue||bNextValue==0x01){
				bCardTemp[i-1]=bCardTemp[i];
		        bCardTemp[i]=bTempCard;
			}
		}
	}
	
	//��ȡ��ֵ
	BYTE bFirstColor=GetCardColor(bCardTemp[j]);
	bFirstValue=GetCardValue(bCardTemp[j]);
    bLastValue=GetCardValue(bCardTemp[4]);
    
	//��������
	BYTE cbValue;
	BYTE bTemp=0;
	if (bFirstValue==1)
	{   
	   if(bLastValue>0 && bLastValue<5)
	   {
		   m_bValue=5;
		   bTemp=5;
	   }
	   else 
	   {   
		   m_bValue=14;
		   bTemp=13;
	   }
    }
	
	 //��ʼ�ж�
	for(BYTE i=j+1,m=1;i<cbCardCount;i++,m++)
	{
		cbValue=GetCardValue(bCardTemp[i]);
		if (GetCardColor(bCardTemp[i])!=bFirstColor)
		{
				return false;
		}
		while (cbValue!=(bFirstValue+bTemp-m))
		{
		    if(bSeriesCount<=0){
			   return false;
			 }
			 else{
					m=m+1;
					bSeriesCount--;
				 }
		}
     }
	
	//����ֵ����
    if(bFirstValue!=1)    
      m_bValue=((bFirstValue+bSeriesCount)>14)? 14:(bFirstValue+bSeriesCount);

	return true;
}
//�Ƿ�ְ�
bool CGameLogic::IsGangBan(BYTE cbCardData[], BYTE cbCardCount)
{
    //��������
	BYTE bSeriesCount=0,j=0;
	BYTE bFirstCount=1,bNextCount=1;
	
	//�������
	while(j<cbCardCount&&cbCardData[j]==m_bSeries)
	{
		bSeriesCount++;
		j++;
	} 
    
	
	//��������
	while(j<(cbCardCount-1)&&(GetCardValue(cbCardData[j])==GetCardValue(cbCardData[j+1])))
	{
		bFirstCount++;
		j++;
	}
	//�����ж�
	if(bFirstCount>3) return false;
	if(j==cbCardCount) return false;

	//ȡֵ
    BYTE bFirstValue = GetCardValue(cbCardData[j]);
	BYTE bNextValue  = GetCardValue(cbCardData[j+1]);
    
	//�����ж�
    if((bFirstValue!=(bNextValue+1))&&(bFirstValue!=(bNextValue-1))
		&&(bFirstValue!=(bNextValue-12))&&(bFirstValue!=(bNextValue+12)))
	   return false;
	if(bFirstValue==(bNextValue+1))
		m_bValue=bFirstValue;
	if(bFirstValue==(bNextValue-1))
		m_bValue=bNextValue;
	if((bFirstValue==(bNextValue-12))||(bFirstValue==(bNextValue+12)))
		m_bValue=14;

	
	j++;
	while(j<(cbCardCount-1)&&(GetCardValue(cbCardData[j])==GetCardValue(cbCardData[j+1])))
	{
		bNextCount++;
		j++;
	}
	if(bNextCount>3) return false;
	
	if((bSeriesCount+bFirstCount+bNextCount)!=6) return false;
	
	return true;
	
}
//�Ƿ�������
bool CGameLogic::IsLiangLianDui(BYTE cbCardData[], BYTE cbCardCount)
{ 
	//��������
	BYTE bSeriesCount=0,j=0;
	BYTE bCardTemp[6];
	memcpy(bCardTemp,cbCardData,cbCardCount);

	//�������
	while(cbCardData[j]==m_bSeries&&j<cbCardCount)
	{
		bSeriesCount++;
		j++;
	} 

    BYTE bFirstValue=GetCardValue(cbCardData[j]);
	BYTE bMidValue,bLastValue;
    BYTE bFirstCount=1,bMidCount=0,bLastCount=0;
    
	//˳�����
	if((bFirstValue==GetCardValue(m_bSeries))&&(bFirstValue!=0x01)){
		for( BYTE i=j+1;i<cbCardCount;i++)
			if(bFirstValue<=GetCardValue(cbCardData[i])){
				bCardTemp[i-1]=bCardTemp[i];
		        bCardTemp[i]=bFirstValue;
			}
	   for( BYTE i=j+1;i<cbCardCount;i++)
			if(bFirstValue<=GetCardValue(cbCardData[i])){
				bCardTemp[i-1]=bCardTemp[i];
		        bCardTemp[i]=bFirstValue;
			}
	}

	//��ʼ����
	bFirstValue=GetCardValue(bCardTemp[j]);
    while(j<(cbCardCount-1)&&GetCardValue(bCardTemp[j])==GetCardValue(bCardTemp[j+1]))
	{
		bFirstCount++;
		j++;
	}
	if(bFirstCount>2) return false;
	j++;
	bMidValue=GetCardValue(bCardTemp[j]);
	bMidCount++;
	while(j<(cbCardCount-1)&&GetCardValue(bCardTemp[j])==GetCardValue(bCardTemp[j+1]))
	{
		bMidCount++;
		j++;
	}
	if(bMidCount>2) return false;
	j++;
	if(j<cbCardCount)
	{
		bLastValue=GetCardValue(bCardTemp[j]);
		bLastCount++;
		while(j<(cbCardCount-1)&&(GetCardValue(bCardTemp[j])==GetCardValue(bCardTemp[j+1])))
		{
			bLastCount++;
			j++;
		}
		j++;
	}
    if(bLastCount>2) return false;
	if((bSeriesCount+bFirstCount+bMidCount+bLastCount)!=6) return false;
 
	if(bLastCount==0){
		if(bSeriesCount!=2)
			   return false;
        if(bFirstValue==1)
		{ 
				m_bValue=14;
				return   (bMidValue==13||bMidValue==12||bMidValue==3||bMidValue==2);
		} 
		else
		{   
		
			if((bFirstValue-bMidValue)==1)
			{
				m_bValue=bFirstValue+1;
				return true;
			}
		   else if((bFirstValue-bMidValue)==2)
		   { 
			   m_bValue=bFirstValue;
			   return true;
		   }
		   return false;
		}
	}
	m_bValue=bFirstValue;
    if(bFirstValue==1)
	{
		m_bValue=14;
		return  (bMidValue==13&&bLastValue==12)||(bMidValue==3&&bLastValue==2);
	}
	
	return (bFirstValue==(bMidValue+1))&&(bMidValue==(bLastValue+1));

}


//�����ж�
bool CGameLogic::CompareCard(BYTE bFirstList[], BYTE bNextList[], BYTE cbFirstCount, BYTE cbNextCount)
{
	//��ȡ����
	short bFirstType	=	GetCardType(bFirstList,cbFirstCount);
	short bNextType		=	GetCardType(bNextList,  cbNextCount);

	if((bFirstType&0x00ff)==(bNextType&0x00ff)){
		return  bFirstType>bNextType;
	}
	if((bFirstType&0x00ff)>(bNextType&0x00ff)){
		if((bFirstType&0x00ff)>=CT_SI_ZHANG_BOMB)
			return true;
	}
	return false;
}
//ը������
int CGameLogic::BombCount(BYTE cbCardData[], BYTE cbCardCount)
{
	//��������
	BYTE bFirstValue,bNextValue;

	//���⴦��
	bFirstValue=GetCardColor(cbCardData[0]);
    if(bFirstValue==0x40) return 0;
	
	
	//�������
	BYTE bBombCount=0;
	BYTE i=0;
    while(cbCardData[i]==m_bSeries)
	{
		i++;
		bBombCount++;
	}
	
	//��������
	bFirstValue=GetCardValue(cbCardData[i]);
	bBombCount++;
	i++;
	for(;i<cbCardCount;i++)
	{
		bNextValue=GetCardValue(cbCardData[i]);
		if(bFirstValue==bNextValue)
			bBombCount++;
		else
			return 0;
	}
	return  bBombCount;
}

//����˿�
bool CGameLogic::Add(BYTE bAddCard, BYTE  cbCardData[],BYTE cbCardCount=27)
{
	cbCardData[cbCardCount]=bAddCard;
	SortCardList(cbCardData,cbCardCount+1);
	return true;
}



//
//////////////////////////////////////////////////////////////////////////
