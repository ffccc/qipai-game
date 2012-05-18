#include "StdAfx.h"
#include ".\gameprompt.h"



//���캯��
CGamePrompt::CGamePrompt(void)
{
    //��������
	m_bTurnCardType=CT_ERROR;
	m_bHongTaoCount=0;
	m_bCardCount=0;
	ZeroMemory(m_bHongTaoLocation,sizeof(m_bHongTaoLocation));
	ZeroMemory(m_bCardData,sizeof(m_bCardData));
	ZeroMemory(m_bCardPosition,sizeof(m_bCardPosition));
	ZeroMemory(desiCardData,sizeof(desiCardData));
	ZeroMemory(CardData,sizeof(CardData));
}


//��������
CGamePrompt::~CGamePrompt(void)
{
}

//������ʾ
void   CGamePrompt::Prompt()
{
	m_bHongTaoCount=0;
	
	//�������������
    for(BYTE i=0,j=0;i<m_bCardCount;i++)
	{ 
		
		if(m_bCardData[i]==m_bSeries)
		{
            m_bHongTaoLocation[m_bHongTaoCount]=i;
			m_bHongTaoCount++;
		}else
		{
			m_bCardPosition[j]=i;
			j++;
		}
	}
	
	//ɾ������
	BYTE   card2[]={m_bSeries,m_bSeries};
	RemoveCard(card2,m_bHongTaoCount,m_bCardData,m_bCardCount);
	m_bCardCount=m_bCardCount-m_bHongTaoCount;

	BYTE cbLogicValue[MAX_COUNT];
	for (BYTE i=0;i<m_bCardCount;i++)
	{
		cbLogicValue[i]		=	GetCardLogicValue(m_bCardData[i]);
	}

	//�������
	bool bSorted;
	BYTE cbTempData,bLast=m_bCardCount-1;
	do
	{
		bSorted=false;
		for (BYTE i=0;i<bLast;i++)
		{
			if (cbLogicValue[i]<cbLogicValue[i+1])
			{
				//����λ��
				cbTempData=m_bCardData[i];
				m_bCardData[i]=m_bCardData[i+1];
				m_bCardData[i+1]=cbTempData;
				
				cbTempData=cbLogicValue[i];
				cbLogicValue[i]=cbLogicValue[i+1];
				cbLogicValue[i+1]=cbTempData;
                
				cbTempData			= m_bCardPosition[i];
				m_bCardPosition[i]	= m_bCardPosition[i+1];
				m_bCardPosition[i+1]= cbTempData;

				bSorted=true;
			}	
		}
		bLast--;
	} while(bSorted);
  
  //��ֵ��ͬ����
  BYTE bStart=0,bRound=0,bNext=0;
  while(bNext<m_bCardCount)
  {
	  BYTE i=1;
	  bStart=bNext;
	  while(bNext<(m_bCardCount-1)&&(cbLogicValue[bNext]==cbLogicValue[bNext+1])) 
	  {
		  i++;
		  bNext++;
	  }

	  CardData[bRound].bCardCount		=i;
	  CardData[bRound].bCardLocation	=bStart;
	  CardData[bRound].bCardLogicValue	=GetCardLogicValue(m_bCardData[bStart]);
	  CardData[bRound].bCardValue		=m_bCardData[bStart];
	  bRound++;
	  bNext++;
  }
  m_bRound=bRound;
  CopyMemory(desiCardData,CardData,sizeof(CardData));
  
  //����
  bSorted=true;
  bLast=m_bRound-1;
  CardNumber bCardTemp;
  do
  {
	  bSorted=false;
	  for(BYTE i=0;i<bLast;i++)
	  {
		  if(CardData[i].bCardCount<CardData[i+1].bCardCount)
		  {
			  bCardTemp		=	CardData[i];
			  CardData[i]	=	CardData[i+1];
			  CardData[i+1]	=	bCardTemp;

			  bSorted=true;
		  }
	  }
	  bLast--;
  }while(bSorted);
  //��ʼ����
	//��������
	GetSingle();
	//��������
	GetDouble();
	//����santiao
	GetSanTiao();
	//������«
	GetHulu();
	//����������
	GetLiangLianDui();
	//����˳��
	GetShunZhi();
	//�����ְ�
	GetGangBan();
	//������ը��
	GetSiZhangBomb();
	//������ը��
	GetWuZhangBomb();
	//����ͬ��˳
	GetTongHuaShun();
	//����ͬ��˳
	GetTongHuaShun(13);
	//������ը��
	GetLiuZhangBomb();
	//������ը��
	GetQiZhangBomb();
	//������ը��
	 GetBaZhangBomb();
	//������ը��
	 GetJiuZhangBomb();
	//����ʮը��
	GetShiZhangBomb();
	//��������
	GetSiWang();

}
    
//��������
void  CGamePrompt::GetSingle()
{
	//�����ж�
	if(((BYTE)m_bTurnCardType)!=CT_SINGLE)
		return;
	
	//��������
	BYTE  bFirstValue=(m_bTurnCardType>>8);
    for(int  i=m_bRound-1; i>=0; i--)
	{
		if(CardData[i].bCardLogicValue>bFirstValue)
		{
            
			CardType   card	;
			card.bCardCount			=1;
			card.bCardType			=CT_SINGLE;
			card.bCardLocation[0]	=m_bCardPosition[CardData[i].bCardLocation];
            
			OutCardList.Add(card);
		}
	}
}


//��������
void  CGamePrompt::GetDouble()
{
	//�����ж�
	if(((BYTE)m_bTurnCardType)!=CT_DOUBLE)
		return;
	
	//��������
	BYTE bFirstValue=(m_bTurnCardType>>8);
    for(int  i=m_bRound-1;i>=0 ; i--)
	{
		if(CardData[i].bCardCount>3) break;
		if(CardData[i].bCardCount>=2&&CardData[i].bCardCount<=3&&CardData[i].bCardLogicValue>bFirstValue)
		{
			  
			  CardType  card;		
			  card.bCardCount			=	2;
			  card.bCardType			=	CT_DOUBLE;
			  card.bCardLocation[0]		=	m_bCardPosition[CardData[i].bCardLocation];
			  card.bCardLocation[1]		=	m_bCardPosition[CardData[i].bCardLocation+1];

			  OutCardList.Add(card);
		}

	}
}
	

//����santiao
void   CGamePrompt::GetSanTiao()
{
    //�����ж�
	if(((BYTE)m_bTurnCardType)!=CT_THREE_TIAO)
		return;
	
	//��������
	BYTE bFirstValue=(m_bTurnCardType>>8);
    for(int  i=m_bRound-1;i>=0 ; i--)
	{
		if(CardData[i].bCardCount>3) break;
		if(CardData[i].bCardCount==3&&CardData[i].bCardLogicValue>bFirstValue)
		{
			           
				CardType  card			;
				card.bCardCount			=	3;
				card.bCardType			=	CT_THREE_TIAO;
				card.bCardLocation[0]		=	m_bCardPosition[CardData[i].bCardLocation];
				card.bCardLocation[1]		=	m_bCardPosition[CardData[i].bCardLocation+1];
				card.bCardLocation[2]		=   m_bCardPosition[CardData[i].bCardLocation+2];

				 OutCardList.Add(card);
		}

	}

}
//������«
void   CGamePrompt::GetHulu()
{
	//�����ж�
	if(((BYTE)m_bTurnCardType)!=CT_HU_LU)
		return ;
	
	//��������
	BYTE bFirstValue=(m_bTurnCardType>>8);
	for(int  i=m_bRound-1;i>=0;i--)
	{
		if(CardData[i].bCardCount>3) break;
		if(CardData[i].bCardCount==3&&CardData[i].bCardLogicValue>bFirstValue)
		{
			for(int j=m_bRound-1;j>=0;j--)
			{
				if(CardData[j].bCardCount>3) break;
				if(j!=i&&(CardData[j].bCardCount==2||CardData[j].bCardCount==3))
				{
					CardType  card;
					card.bCardCount			=	5;
					card.bCardType			=	CT_HU_LU;
					card.bCardLocation[0]	=	m_bCardPosition[CardData[i].bCardLocation];
					card.bCardLocation[1]	=	m_bCardPosition[CardData[i].bCardLocation+1];
					card.bCardLocation[2]	=   m_bCardPosition[CardData[i].bCardLocation+2];
					card.bCardLocation[3]	=   m_bCardPosition[CardData[j].bCardLocation];
					card.bCardLocation[4]	=	m_bCardPosition[CardData[j].bCardLocation+1];

					OutCardList.Add(card);
                    
				}

			}
	   }
	}
}

//����������
void  CGamePrompt::GetLiangLianDui()
{
    //�����ж�
	if(((BYTE)m_bTurnCardType)!=CT_LIANG_LIAN_DUI)
		return ;
	if(m_bRound<3) return ;
	
	//��������
	BYTE bFirstValue=(m_bTurnCardType>>8);
	for(int i=m_bRound-3;i>=0;i--)
    {
		if(CardData[i].bCardLogicValue<=bFirstValue)
			continue;
		if(CardData[i].bCardLogicValue>=15) continue;  
		if(CardData[i].bCardCount>=3) break;
		if(CardData[i].bCardCount==2&&CardData[i+1].bCardCount==2&&CardData[i+2].bCardCount==2)
			if(CardData[i].bCardLogicValue==(CardData[i+1].bCardLogicValue+1))
				if(CardData[i+1].bCardLogicValue==(CardData[i+2].bCardLogicValue+1))
				{
				         CardType card;
						 card.bCardCount		=	6;
						 card.bCardType			=	CT_LIANG_LIAN_DUI;
						 card.bCardLocation[0]	=	m_bCardPosition[CardData[i].bCardLocation];
						 card.bCardLocation[1]	=	m_bCardPosition[CardData[i].bCardLocation+1];
						 card.bCardLocation[2]	=	m_bCardPosition[CardData[i+1].bCardLocation];
						 card.bCardLocation[3]	=	m_bCardPosition[CardData[i+1].bCardLocation+1];
						 card.bCardLocation[4]	=	m_bCardPosition[CardData[i+2].bCardLocation];
						 card.bCardLocation[5]	=	m_bCardPosition[CardData[i+2].bCardLocation+1];

						 OutCardList.Add(card);

				} 

	} 
}

//����˳��
void  CGamePrompt::GetShunZhi()
{
    //�����ж�
	if((BYTE(m_bTurnCardType))!=CT_SHUN_ZI)	return;
	if(m_bRound<5)							return;

	//��ʼ����
	BYTE		bFirstValue=(m_bTurnCardType>>8);
	for(int i=m_bRound-1;i>=0;i--)
	{
		if(desiCardData[i].bCardLogicValue>=16) break;
		if(desiCardData[i].bCardLogicValue<=bFirstValue)
		   continue;
		if(	desiCardData[i].bCardCount>2)
		{ 
			i=i-4;
			continue;
		}
		
		if((i+4)<m_bRound)
		{   
			BYTE l=1;
            while(desiCardData[i].bCardLogicValue==(desiCardData[i+l].bCardLogicValue+l))
			{
				l++;
				if(l==5) break;
			}
			if(l==5)
			{
                 
					ASSERT(l==5);	
					CardType  card;
					card.bCardCount			=	5;
					card.bCardType			=	CT_SHUN_ZI;
					card.bCardLocation[0]	=	m_bCardPosition[desiCardData[i].bCardLocation];
					card.bCardLocation[1]	=	m_bCardPosition[desiCardData[i+1].bCardLocation];
					card.bCardLocation[2]	=   m_bCardPosition[desiCardData[i+2].bCardLocation];
					card.bCardLocation[3]	=   m_bCardPosition[desiCardData[i+3].bCardLocation];
					card.bCardLocation[4]	=	m_bCardPosition[desiCardData[i+4].bCardLocation];

					OutCardList.Add(card);
			}
		}
	}
}
//�����ְ�
void  CGamePrompt::GetGangBan()
{
       //�����ж�
	if(((BYTE)m_bTurnCardType)!=CT_GANG_BAN)
		return ;
	
	//��������
	BYTE bFirstValue=(m_bTurnCardType>>8);
	for(int  i=m_bRound-1;i>=0;i--)
	{
		if(CardData[i].bCardLogicValue>=16) continue;    
		if(CardData[i].bCardCount==3&&CardData[i].bCardLogicValue>bFirstValue)
		{
			for(int j=m_bRound-1;j>=0;j--)
			{
				if(j!=i&&CardData[j].bCardCount==3&&(CardData[i].bCardLogicValue==(CardData[j].bCardLogicValue+1)))
				{
					CardType  card;
					card.bCardCount			=	6;
					card.bCardType			=	CT_GANG_BAN;
					card.bCardLocation[0]	=	m_bCardPosition[CardData[i].bCardLocation];
					card.bCardLocation[1]	=	m_bCardPosition[CardData[i].bCardLocation+1];
					card.bCardLocation[2]	=   m_bCardPosition[CardData[i].bCardLocation+2];
					card.bCardLocation[3]	=   m_bCardPosition[CardData[j].bCardLocation];
					card.bCardLocation[4]	=	m_bCardPosition[CardData[j].bCardLocation+1];
					card.bCardLocation[5]	=	m_bCardPosition[CardData[j].bCardLocation+2];

					OutCardList.Add(card);
                    
				}
           }
	   }
	}
}
//����ͬ��˳
void  CGamePrompt::GetTongHuaShun(BYTE mark)
{
  if((BYTE(m_bTurnCardType))>CT_TONG_HUA_SHUN)
	  return ;
  if(m_bRound<5) return ;

  //��ʼ����
  BYTE  bFirstValue;
  if(CT_TONG_HUA_SHUN>(BYTE(m_bTurnCardType)))
	  bFirstValue=0;
  else 
	  bFirstValue=(m_bTurnCardType>>8);
  
//ת����ֵ
	BYTE cbCardPosition[28],cbCardData[28];
	CopyMemory(cbCardPosition,	m_bCardPosition,sizeof(BYTE)*27);
	CopyMemory(cbCardData,		m_bCardData,	sizeof(BYTE)*27);	
    BYTE     bHongTaoCount=m_bHongTaoCount;
	
	//����ɫ�������
	bool bSorted;
	BYTE cbTempData,bLast=m_bCardCount-1;
	do
	{
		bSorted=false;
		for (BYTE i=0;i<bLast;i++)
		{
			if ((((cbCardData[i]&0x0f)!=1)&&cbCardData[i]<cbCardData[i+1])||(
				((cbCardData[i]&0x0f)==1)&&(cbCardData[i]+mark)<cbCardData[i+1]))
				
			{
				//����λ��
				cbTempData=cbCardData[i];
				cbCardData[i]=cbCardData[i+1];
				cbCardData[i+1]=cbTempData;
				
				cbTempData=cbCardPosition[i];
				cbCardPosition[i]=cbCardPosition[i+1];
				cbCardPosition[i+1]=cbTempData;
				bSorted=true;
			}	
		}
		bLast--;
	}while(bSorted);
    
	//��ʱλ��
	BYTE    CardPosition[10];
	for(BYTE i=0;i<m_bCardCount-4+m_bHongTaoCount;i++)
	{
		if(GetCardLogicValue(cbCardData[i])>=16)
			continue;
		bHongTaoCount=m_bHongTaoCount;
		CardPosition[0]=cbCardPosition[i];
		BYTE j=1,k=1;
		if(cbCardData[i+1]==cbCardData[i])
			k++;
		bool bCanGo=true;
		while(bCanGo)
		{
			while(((i+k)<m_bCardCount)&&cbCardData[i]!=(cbCardData[i+k]+j))
			{
				if(bHongTaoCount>0)
				{	
					CardPosition[j]=m_bHongTaoLocation[m_bHongTaoCount-bHongTaoCount];
					bHongTaoCount--;
					j++;
				}else
				{
					bCanGo=false;
					break;
				}
				if(j==5)
				{
					bCanGo=false;
					break;
				}
			}
			if(j==5) break;
			while(((i+k)<m_bCardCount)&&cbCardData[i]==(cbCardData[i+k]+j))
			{
				CardPosition[j]=cbCardPosition[i+k];
				j++;
				if((i+k+1)<m_bCardCount&&cbCardData[i+k+1]==cbCardData[i+k])
					k++;
				k++;
				if(j>=5||(i+k)>=m_bCardCount)
				{
					bCanGo=false;
					break;
				}
			}
			if((i+k)>=m_bCardCount)
				break;
		}
	
		if(j==5)
		{
			CardType card;
			card.bCardCount		=	5;
			card.bCardType		=	CT_TONG_HUA_SHUN;
			CopyMemory(card.bCardLocation,CardPosition,sizeof(BYTE)* 5);

			OutCardList.Add(card);
		}
		if(j==3&&bHongTaoCount==2)
		{
			CardPosition[3]=m_bHongTaoLocation[0];
			CardPosition[4]=m_bHongTaoLocation[1];
			CardType card;
			card.bCardCount		=	5;
			card.bCardType		=	CT_TONG_HUA_SHUN;
			CopyMemory(card.bCardLocation,CardPosition,sizeof(BYTE)* 5);

			OutCardList.Add(card);
		}
		if(j==4&&bHongTaoCount==1)
		{
			
			CardPosition[4]=m_bHongTaoLocation[m_bHongTaoCount-bHongTaoCount];
			CardType card;
			card.bCardCount		=	5;
			card.bCardType		=	CT_TONG_HUA_SHUN;
			CopyMemory(card.bCardLocation,CardPosition,sizeof(BYTE)* 5);

			OutCardList.Add(card);
		}

	}

}

//������ը��
void  CGamePrompt::GetSiZhangBomb()
{
	 //�����ж�
	if(((BYTE)m_bTurnCardType)>CT_SI_ZHANG_BOMB)
		return ;
	
	//��������
	BYTE  bFirstValue;
	if(CT_SI_ZHANG_BOMB>(BYTE(m_bTurnCardType)))
		bFirstValue=0;
	else 
		bFirstValue=(m_bTurnCardType>>8);
	for(int  i=m_bRound-1;i>=0;i--)
	{
		if(CardData[i].bCardLogicValue<=bFirstValue||CardData[i].bCardLogicValue>=16)
			continue;
		
		if(CardData[i].bCardCount==2&&m_bHongTaoCount==2)
		{
			CardType  card;
			card.bCardCount			=	4;
			card.bCardType			=	CT_SI_ZHANG_BOMB;
			card.bCardLocation[0]	=	m_bCardPosition[CardData[i].bCardLocation];
			card.bCardLocation[1]	=	m_bCardPosition[CardData[i].bCardLocation+1];
			card.bCardLocation[2]	=   m_bHongTaoLocation[0];
			card.bCardLocation[3]	=   m_bHongTaoLocation[1];

			OutCardList.Add(card);
		}
		else if(CardData[i].bCardCount==3&&m_bHongTaoCount==1)
		{
			CardType  card	;
			card.bCardCount			=	4;
			card.bCardType			=	CT_SI_ZHANG_BOMB;
			card.bCardLocation[0]	=	m_bCardPosition[CardData[i].bCardLocation];
			card.bCardLocation[1]	=	m_bCardPosition[CardData[i].bCardLocation+1];
			card.bCardLocation[2]	=   m_bCardPosition[CardData[i].bCardLocation+2];
			card.bCardLocation[3]	=   m_bHongTaoLocation[0];

			OutCardList.Add(card);
		}
		else if(CardData[i].bCardCount==4)
		{
	        CardType  card;
			card.bCardCount			=	4;
			card.bCardType			=	CT_SI_ZHANG_BOMB;
			card.bCardLocation[0]	=	m_bCardPosition[CardData[i].bCardLocation];
			card.bCardLocation[1]	=	m_bCardPosition[CardData[i].bCardLocation+1];
			card.bCardLocation[2]	=   m_bCardPosition[CardData[i].bCardLocation+2];
			card.bCardLocation[3]	=   m_bCardPosition[CardData[i].bCardLocation+3];

			OutCardList.Add(card);
		}
	}

}

//������ը��
void  CGamePrompt::GetWuZhangBomb()
{
	//�����ж�
	if(((BYTE)m_bTurnCardType)>CT_WU_ZHANG_BOMB)
		return ;
	
	//��������
	BYTE  bFirstValue;
	if(CT_WU_ZHANG_BOMB>(BYTE(m_bTurnCardType)))
		bFirstValue=0;
	else 
		bFirstValue=(m_bTurnCardType>>8);
    for(int  i=m_bRound-1;i>=0;i--)
	{
		if(CardData[i].bCardLogicValue<=bFirstValue||CardData[i].bCardLogicValue>=16)
			continue;
		
		if(CardData[i].bCardCount==3&&m_bHongTaoCount==2)
		{
			CardType  card;
			card.bCardCount				=	5;
			card.bCardType				=	CT_WU_ZHANG_BOMB;
			CopyMemory(card.bCardLocation,&m_bCardPosition[CardData[i].bCardLocation],sizeof(BYTE)*3);
			card.bCardLocation[3]		=   m_bHongTaoLocation[0];
			card.bCardLocation[4]		=   m_bHongTaoLocation[1];

			OutCardList.Add(card);
		}
		else if(CardData[i].bCardCount==4&&m_bHongTaoCount==1)
		{
			CardType  card;
			card.bCardCount				=	5;
			card.bCardType				=	CT_WU_ZHANG_BOMB;
			CopyMemory(card.bCardLocation,&m_bCardPosition[CardData[i].bCardLocation],sizeof(BYTE)*4);
			card.bCardLocation[4]		=   m_bHongTaoLocation[0];

			OutCardList.Add(card);
		}
		else if(CardData[i].bCardCount==5)
		{
	        CardType  card	;
			card.bCardCount			=	5;
			card.bCardType			=	CT_WU_ZHANG_BOMB;
			CopyMemory(card.bCardLocation,&m_bCardPosition[CardData[i].bCardLocation],sizeof(BYTE)*5);

			OutCardList.Add(card);
		}
	}

}

//������ը��
void  CGamePrompt::GetLiuZhangBomb()
{
    //�����ж�
	if(((BYTE)m_bTurnCardType)>CT_LIU_ZHANG_BOMB)
		return ;
	
	//��������
	BYTE  bFirstValue;
	if(CT_LIU_ZHANG_BOMB>(BYTE(m_bTurnCardType)))
		bFirstValue=0;
	else 
		bFirstValue=(m_bTurnCardType>>8);
    for(int  i=m_bRound-1;i>=0;i--)
	{
		if(CardData[i].bCardLogicValue<=bFirstValue||CardData[i].bCardLogicValue>=16)
			continue;
		
		if(CardData[i].bCardCount==4&&m_bHongTaoCount==2)
		{
			CardType  card;
			card.bCardCount			=	6;
			card.bCardType			=	CT_LIU_ZHANG_BOMB;
			CopyMemory(card.bCardLocation,&m_bCardPosition[CardData[i].bCardLocation],sizeof(BYTE)*4);
			card.bCardLocation[4]	=   m_bHongTaoLocation[0];
			card.bCardLocation[5]	=   m_bHongTaoLocation[1];
			
			OutCardList.Add(card);
		}
		else if(CardData[i].bCardCount==5&&m_bHongTaoCount==1)
		{
			CardType  card;
			card.bCardCount			=	6;
			card.bCardType			=	CT_LIU_ZHANG_BOMB;
			CopyMemory(card.bCardLocation,&m_bCardPosition[CardData[i].bCardLocation],sizeof(BYTE)*5);
			card.bCardLocation[5]	=   m_bHongTaoLocation[0];

			OutCardList.Add(card);
		}
		else if(CardData[i].bCardCount==6)
		{
	        CardType  card;
			card.bCardCount			=	6;
			card.bCardType			=	CT_LIU_ZHANG_BOMB;
			CopyMemory(card.bCardLocation,&m_bCardPosition[CardData[i].bCardLocation],sizeof(BYTE)*6);

			OutCardList.Add(card);
		}
	}
}

//������ը��
void  CGamePrompt::GetQiZhangBomb()
{
	//�����ж�
	if(((BYTE)m_bTurnCardType)>CT_QI_ZHANG_BOMB)
		return ;
	
	//��������
	BYTE  bFirstValue;
	if(CT_QI_ZHANG_BOMB>(BYTE(m_bTurnCardType)))
		bFirstValue=0;
	else 
		bFirstValue=(m_bTurnCardType>>8);
    for(int  i=m_bRound-1;i>=0;i--)
	{
		if(CardData[i].bCardLogicValue<=bFirstValue||CardData[i].bCardLogicValue>=16)
			continue;
		
		if(CardData[i].bCardCount==5&&m_bHongTaoCount==2)
		{
			CardType  card	;
			card.bCardCount			=	7;
			card.bCardType			=	CT_QI_ZHANG_BOMB;
			CopyMemory(card.bCardLocation,&m_bCardPosition[CardData[i].bCardLocation],sizeof(BYTE)*5);
			card.bCardLocation[5]	=   m_bHongTaoLocation[0];
			card.bCardLocation[6]	=   m_bHongTaoLocation[1];
			
			OutCardList.Add(card);
		}
		else if(CardData[i].bCardCount==6&&m_bHongTaoCount==1)
		{
			CardType  card;
			card.bCardCount			=	7;
			card.bCardType			=	CT_QI_ZHANG_BOMB;
			CopyMemory(card.bCardLocation,&m_bCardPosition[CardData[i].bCardLocation],sizeof(BYTE)*6);
			card.bCardLocation[6]	=   m_bHongTaoLocation[0];

			OutCardList.Add(card);
		}
		else if(CardData[i].bCardCount==7)
		{
	        CardType  card;
			card.bCardCount			=	7;
			card.bCardType			=	CT_QI_ZHANG_BOMB;
			CopyMemory(card.bCardLocation,&m_bCardPosition[CardData[i].bCardLocation],sizeof(BYTE)*7);

			OutCardList.Add(card);
		}
	}
}

//������ը��
void  CGamePrompt::GetBaZhangBomb()
{
     
	//�����ж�
	if(((BYTE)m_bTurnCardType)>CT_BA_ZHANG_BOMB)
		return ;
	
	//��������
	BYTE  bFirstValue;
	if(CT_BA_ZHANG_BOMB>(BYTE(m_bTurnCardType)))
		bFirstValue=0;
	else 
		bFirstValue=(m_bTurnCardType>>8);
    for(int  i=m_bRound-1;i>=0;i--)
	{
		if(CardData[i].bCardLogicValue<=bFirstValue||CardData[i].bCardLogicValue>=16)
			continue;
		
		if(CardData[i].bCardCount==6&&m_bHongTaoCount==2)
		{
			CardType  card	;
			card.bCardCount			=	8;
			card.bCardType			=	CT_BA_ZHANG_BOMB;
			CopyMemory(card.bCardLocation,&m_bCardPosition[CardData[i].bCardLocation],sizeof(BYTE)*6);
			card.bCardLocation[6]	=   m_bHongTaoLocation[0];
			card.bCardLocation[7]	=   m_bHongTaoLocation[1];
			
			OutCardList.Add(card);
		}
		else if(CardData[i].bCardCount==7&&m_bHongTaoCount==1)
		{
			CardType  card	;
			card.bCardCount			=	8;
			card.bCardType			=	CT_QI_ZHANG_BOMB;
			CopyMemory(card.bCardLocation,&m_bCardPosition[CardData[i].bCardLocation],sizeof(BYTE)*7);
			card.bCardLocation[7]	=   m_bHongTaoLocation[0];

			OutCardList.Add(card);
		}
		else if(CardData[i].bCardCount==8)
		{
	        CardType  card	;
			card.bCardCount			=	8;
			card.bCardType			=	CT_BA_ZHANG_BOMB;
			CopyMemory(card.bCardLocation,&m_bCardPosition[CardData[i].bCardLocation],sizeof(BYTE)*8);

			OutCardList.Add(card);
		}
	}
}

//������ը��
void  CGamePrompt::GetJiuZhangBomb()
{
     //�����ж�
	if(((BYTE)m_bTurnCardType)>CT_JIU_ZHANG_BOMB)
		return ;
	
	//��������
	BYTE  bFirstValue;
	if(CT_JIU_ZHANG_BOMB>(BYTE(m_bTurnCardType)))
		bFirstValue=0;
	else 
		bFirstValue=(m_bTurnCardType>>8);
    for(int  i=m_bRound-1;i>=0;i--)
	{
		if(CardData[i].bCardLogicValue<=bFirstValue||CardData[i].bCardLogicValue>=16)
			continue;
		
		if(CardData[i].bCardCount==7&&m_bHongTaoCount==2)
		{
			CardType  card	;
			card.bCardCount			=	9;
			card.bCardType			=	CT_JIU_ZHANG_BOMB;
			CopyMemory(card.bCardLocation,&m_bCardPosition[CardData[i].bCardLocation],sizeof(BYTE)*7);
			card.bCardLocation[7]	=   m_bHongTaoLocation[0];
			card.bCardLocation[8]	=   m_bHongTaoLocation[1];
			
			OutCardList.Add(card);
		}
		else if(CardData[i].bCardCount==8&&m_bHongTaoCount==1)
		{
			CardType  card;
			card.bCardCount			=	9;
			card.bCardType			=	CT_JIU_ZHANG_BOMB;
			CopyMemory(card.bCardLocation,&m_bCardPosition[CardData[i].bCardLocation],sizeof(BYTE)*8);
			card.bCardLocation[8]	=   m_bHongTaoLocation[0];

			OutCardList.Add(card);
		}
	}

}
	
//����ʮը��
void  CGamePrompt::GetShiZhangBomb()
{  
	
	//�����ж�
	if(((BYTE)m_bTurnCardType)>CT_SHI_ZHANG_BOMB)
		return ;
	if(m_bHongTaoCount!=2) return;
	
	//��������
	BYTE  bFirstValue;
	if(CT_SHI_ZHANG_BOMB>(BYTE(m_bTurnCardType)))
		bFirstValue=0;
	else 
		bFirstValue=(m_bTurnCardType>>8);
    for(BYTE i=0;i<m_bRound;i++)
	{
		if(CardData[i].bCardLogicValue<=bFirstValue||CardData[i].bCardLogicValue>=16)
			continue;
		if(CardData[i].bCardCount==8)
		{
			CardType  card;
			card.bCardCount			=	10;
			card.bCardType			=	CT_SHI_ZHANG_BOMB;
			CopyMemory(card.bCardLocation,&m_bCardPosition[CardData[i].bCardLocation],sizeof(BYTE)*8);
			card.bCardLocation[8]	=   m_bHongTaoLocation[0];
			card.bCardLocation[9]	=   m_bHongTaoLocation[1];
			
			OutCardList.Add(card);
			
			break;
		}
	}
}

//��������
void  CGamePrompt::GetSiWang()
{
	//��������
	for(BYTE i=0;i<m_bRound-1;i++)
	{
		if(i<(m_bRound-1)&&CardData[i].bCardLogicValue==GetCardLogicValue(0x42)&&CardData[i].bCardCount==2
			&&CardData[i+1].bCardLogicValue==GetCardLogicValue(0x41)&&CardData[i+1].bCardCount==2)
		{
			CardType card;
			card.bCardCount		=	4;
			card.bCardType			=	CT_FOUR_KING;
			CopyMemory(card.bCardLocation,&m_bCardPosition[CardData[i].bCardLocation],sizeof(BYTE)*2);
			CopyMemory(&card.bCardLocation[2],&m_bCardPosition[CardData[i+1].bCardLocation],sizeof(BYTE)*2);
           
			OutCardList.Add(card);
			
			break;
		}
	}
}
