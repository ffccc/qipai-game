#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//�˿�����
BYTE const CGameLogic::m_cbCardDataArray[MAX_REPERTORY]=
{
	0x01,0x01,0x01,0x11,0x11,               //һ--0
	0x02,0x02,0x02,0x02,0x02,               //��--1
	0x03,0x03,0x03,0x13,0x13,               //��--2
	0x04,0x04,0x04,0x04,0x04,               //��--3
	0x05,0x05,0x05,0x15,0x15,               //��--4
	0x06,0x06,0x06,0x06,0x06,               //��--5
	0x07,0x07,0x07,0x17,0x17,               //��--6
	0x08,0x08,0x08,0x08,0x08,               //��--7
	0x09,0x09,0x09,0x19,0x19,				//��--8
	0x0A,0x0A,0x0A,0x0A,0x0A,               //ʮ--9
	
	0x21,0x21,0x21,0x21,0x21,               //��--10
	0x22,0x22,0x22,0x22,0x22,               //��--11

	0x23,0x23,0x23,0x23,0x23,               //��--12
	0x24,0x24,0x24,0x24,0x24,               //��--13
	0x25,0x25,0x25,0x25,0x25,               //��--14

	0x26,0x26,0x26,0x26,0x26,               //��--15
	0x27,0x27,0x27,0x27,0x27,               //��--16
	0x28,0x28,0x28,0x28,0x28,               //ǧ--17

	0x29,0x29,0x29,0x29,0x29,               //��--18
	0x2A,0x2A,0x2A,0x2A,0x2A,               //֪--19
	0x2B,0x2B,0x2B,0x2B,0x2B,               //��--20

	0x2C,0x2C,0x2C,0x2C,0x2C,               //��--21
};

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameLogic::CGameLogic()
{
}

//��������
CGameLogic::~CGameLogic()
{
}

//ɾ���˿�
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard)
{
	//Ч���˿�
	ASSERT(IsValidCard(cbRemoveCard));

	BYTE cbRemoveIndex=SwitchToCardIndex(cbRemoveCard);
	ASSERT(cbCardIndex[cbRemoveIndex]>0);

	//ɾ���˿�
	if (cbCardIndex[cbRemoveIndex]>0)
	{
		cbCardIndex[cbRemoveIndex]--;
		return true;
	}

	//ʧ��Ч��
	ASSERT(FALSE);

	return false;
}

//ɾ���˿�
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard[], BYTE cbRemoveCount)
{
	//ɾ���˿�
	for (BYTE i=0;i<cbRemoveCount;i++)
	{
		//Ч���˿�
		ASSERT(IsValidCard(cbRemoveCard[i]));
		ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard[i])]>0);

		BYTE cbRemoveIndex=SwitchToCardIndex(cbRemoveCard[i]);
		//ɾ���˿�
		if (cbCardIndex[cbRemoveIndex]==0)
		{
			//�������
			ASSERT(FALSE);

			//��ԭɾ��
			for (BYTE j=0;j<i;j++) 
			{
				ASSERT(IsValidCard(cbRemoveCard[j]));
				cbCardIndex[SwitchToCardIndex(cbRemoveCard[j])]++;
			}

			return false;
		}
		else 
		{
			//ɾ���˿�
			--cbCardIndex[cbRemoveIndex];
		}
	}

	return true;
}


//��Ч�ж�
bool CGameLogic::IsValidCard(BYTE cbCardData)
{

	BYTE cbValue=(cbCardData&MASK_VALUE);
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;

	//�����ɫ��0||1����ֵ������0--A(10)
	//�����ɫ��2����ֵ������0--C(12)
	return ( ((cbColor == 0 || cbColor == 1) && (cbValue>0 && cbValue<=10) ) || ( (cbColor == 2) && (cbValue>=1 && cbValue<=12)));
}

BYTE CGameLogic::GetHuCardWeave(BYTE cbWeaveKind, BYTE cbCenterCardIndex, BYTE cbCardBuffer[],BYTE cbHuaCount)
{
	//����˿�
	switch (cbWeaveKind)
	{
	case WIK_DUI:		//���Ʋ���
		{
			//���ݻ��Ƶĸ��������ñ���
			switch(cbHuaCount)
			{
			case 0:        //0�Ż���,ȫ�����Ƥ��
				{
					cbCardBuffer[0]=SwitchToCardData(cbCenterCardIndex);
					cbCardBuffer[1]=SwitchToCardData(cbCenterCardIndex);
					cbCardBuffer[2]=SwitchToCardData(cbCenterCardIndex);

					break;
				}
			case 1:  //1�Ż���
				{
					cbCardBuffer[0]=0x10|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);
					cbCardBuffer[1]=0x00|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);
					cbCardBuffer[2]=0x00|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);

					break;
				}
			case 2:    //2�Ż���
				{
					cbCardBuffer[0]=0x10|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);
					cbCardBuffer[1]=0x10|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);
					cbCardBuffer[2]=0x00|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);

					break;
				}
			}
			return 3;
		}
	case WIK_WU_TONG:        //���
	case WIK_SI_ZHAO:		//���в���
		{
			switch(cbHuaCount)
			{
			case 0:  //0�Ż���///////////////////2.26/////////////////
				{
					cbCardBuffer[0]=SwitchToCardData(cbCenterCardIndex);
					cbCardBuffer[1]=SwitchToCardData(cbCenterCardIndex);
					cbCardBuffer[2]=SwitchToCardData(cbCenterCardIndex);
					cbCardBuffer[3]=SwitchToCardData(cbCenterCardIndex);
					cbCardBuffer[4]=SwitchToCardData(cbCenterCardIndex);

					break;
				}
			case 1:
			case 2:
				{

					cbCardBuffer[0]=0x10|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);
					cbCardBuffer[1]=0x10|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);
					cbCardBuffer[2]=0x00|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);
					cbCardBuffer[3]=0x00|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);
					cbCardBuffer[4]=0x00|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);
					break;
				}				
			}
			return 5;
		}
	case WIK_SI_TONG:       //�Ĳز���
	case WIK_SAN_ZHAO:		//���в���
		{

			switch(cbHuaCount)
			{
			case 0:       //0�Ż���	
				{
					cbCardBuffer[0]=SwitchToCardData(cbCenterCardIndex);
					cbCardBuffer[1]=SwitchToCardData(cbCenterCardIndex);
					cbCardBuffer[2]=SwitchToCardData(cbCenterCardIndex);
					cbCardBuffer[3]=SwitchToCardData(cbCenterCardIndex);

					break;
				}
			case 1:       //1�Ż���
				{
					cbCardBuffer[0]=0x10|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);
					cbCardBuffer[1]=0x00|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);
					cbCardBuffer[2]=0x00|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);
					cbCardBuffer[3]=0x00|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);
					
					break;
				}
			case 2:    //2�Ż���
				{
					cbCardBuffer[0]=0x10|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);
					cbCardBuffer[1]=0x10|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);
					cbCardBuffer[2]=0x00|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);
					cbCardBuffer[3]=0x00|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);

					break;
				}
			}
			return 4;
		}
	case WIK_QI_TA:             //���������,��ʱ�ͽ����е��ƶ���Ϊ��Ƥ�������
		{
			//0--9֮������������
			if (cbCenterCardIndex<(10-2))
			{
				cbCardBuffer[0]=SwitchToCardData(cbCenterCardIndex);
				cbCardBuffer[1]=SwitchToCardData(cbCenterCardIndex+1);
				cbCardBuffer[2]=SwitchToCardData(cbCenterCardIndex+2);
			}
			//������
			if (cbCenterCardIndex==10)
			{
				cbCardBuffer[0]=SwitchToCardData(cbCenterCardIndex);
				cbCardBuffer[1]=SwitchToCardData(0);
				cbCardBuffer[2]=SwitchToCardData(cbCenterCardIndex+1);
			}
			//�ϴ���
			if( cbCenterCardIndex ==12 )
			{
				cbCardBuffer[0]=SwitchToCardData(cbCenterCardIndex);
				cbCardBuffer[1]=SwitchToCardData(cbCenterCardIndex+1);
				cbCardBuffer[2]=SwitchToCardData(cbCenterCardIndex+2);
			}
			//��ʮ��
			if (cbCenterCardIndex==15)
			{
				cbCardBuffer[0]=SwitchToCardData(6);
				cbCardBuffer[1]=SwitchToCardData(9);
				cbCardBuffer[2]=SwitchToCardData(cbCenterCardIndex);
			}
			//����ǧ
			if (cbCenterCardIndex==16)
			{
				cbCardBuffer[0]=SwitchToCardData(cbCenterCardIndex);
				cbCardBuffer[1]=SwitchToCardData(2);
				cbCardBuffer[2]=SwitchToCardData(cbCenterCardIndex+1);
			}
			//��֪��
			if (cbCenterCardIndex==18)
			{
				cbCardBuffer[0]=SwitchToCardData(cbCenterCardIndex);
				cbCardBuffer[1]=SwitchToCardData(cbCenterCardIndex+1);
				cbCardBuffer[2]=SwitchToCardData(cbCenterCardIndex+2);
			}
			//�˾���
			if (cbCenterCardIndex==21)
			{
				cbCardBuffer[0]=SwitchToCardData(7);
				cbCardBuffer[1]=SwitchToCardData(8);
				cbCardBuffer[2]=SwitchToCardData(cbCenterCardIndex);
			}
			return 3;
		}
	default:
		{
			ASSERT(FALSE);
		}
	}

	return 0;
}

//��ȡ���
BYTE CGameLogic::GetWeaveCard(BYTE cbWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[],BYTE cbHuaCount)
{
	//����˿�
	switch (cbWeaveKind)
	{
	case WIK_DUI:		//���Ʋ���
		{
			//���ݻ��Ƶĸ��������ñ���
			switch(cbHuaCount)
			{
			case 0:        //0�Ż���,ȫ�����Ƥ��
				{
					cbCardBuffer[0]=cbCenterCard;
					cbCardBuffer[1]=cbCenterCard;
					cbCardBuffer[2]=cbCenterCard;

					break;
				}
			case 1:  //1�Ż���
				{
					cbCardBuffer[0]=0x10|(cbCenterCard&MASK_VALUE);
					cbCardBuffer[1]=0x00|(cbCenterCard&MASK_VALUE);
					cbCardBuffer[2]=0x00|(cbCenterCard&MASK_VALUE);

					break;
				}
			case 2:    //2�Ż���
				{
					cbCardBuffer[0]=0x10|(cbCenterCard&MASK_VALUE);
					cbCardBuffer[1]=0x10|(cbCenterCard&MASK_VALUE);
					cbCardBuffer[2]=0x00|(cbCenterCard&MASK_VALUE);

					break;
				}
			}
			return 3;
		}
	case WIK_SI_ZHAO:		//���в���
		{
			//���ñ���
			switch(cbHuaCount)
			{
			case 0:        //0�Ż���
				{
					cbCardBuffer[0]=cbCenterCard;
					cbCardBuffer[1]=cbCenterCard;
					cbCardBuffer[2]=cbCenterCard;
					cbCardBuffer[3]=cbCenterCard;
					cbCardBuffer[4]=cbCenterCard;
				
					break;
				}	
			case 2:    //2�Ż���
				{
					cbCardBuffer[0]=0x10|(cbCenterCard&MASK_VALUE);
					cbCardBuffer[1]=0x10|(cbCenterCard&MASK_VALUE);
					cbCardBuffer[2]=0x00|(cbCenterCard&MASK_VALUE);
					cbCardBuffer[3]=0x00|(cbCenterCard&MASK_VALUE);
					cbCardBuffer[4]=0x00|(cbCenterCard&MASK_VALUE);

					break;
				}
			}
			return 5;
		}
	case WIK_SAN_ZHAO:		//���в���
		{
			switch(cbHuaCount)
			{
			case 0:        //0�Ż���
				{
					//���ñ���
					cbCardBuffer[0]=cbCenterCard;
					cbCardBuffer[1]=cbCenterCard;
					cbCardBuffer[2]=cbCenterCard;
					cbCardBuffer[3]=cbCenterCard;
					
					break;
				}
			case 1:       //1�Ż���
				{
					cbCardBuffer[0]=0x10|(cbCenterCard&MASK_VALUE);
					cbCardBuffer[1]=0x00|(cbCenterCard&MASK_VALUE);
					cbCardBuffer[2]=0x00|(cbCenterCard&MASK_VALUE);
					cbCardBuffer[3]=0x00|(cbCenterCard&MASK_VALUE);
					
					break;
				}
			case 2:    //2�Ż���
				{
					cbCardBuffer[0]=0x10|(cbCenterCard&MASK_VALUE);
					cbCardBuffer[1]=0x10|(cbCenterCard&MASK_VALUE);
					cbCardBuffer[2]=0x00|(cbCenterCard&MASK_VALUE);
					cbCardBuffer[3]=0x00|(cbCenterCard&MASK_VALUE);

					break;
				}
			}
			return 4;
		}
	default:
		{
			ASSERT(FALSE);
		}
	}

	return 0;
}

//�����˿������ݵõ����Ʊ��浽������,���ػ��Ƶĸ���
BYTE CGameLogic:: GetHuaCard(BYTE cbCardData[],BYTE bCardCount, BYTE cbHuaCardIndex[])
{
	BYTE bHuaCardCount =0;
	//ѭ���������������еõ�����
	for (BYTE i=0; i<bCardCount; i++)
	{
		//����Ч��
		ASSERT(IsValidCard(cbCardData[i]));

		//����ǻ��ƣ�������ת����������ŵ���������������
		if((cbCardData[i]&MASK_COLOR) == 0x10)
		{
			cbHuaCardIndex[SwitchToCardIndex(cbCardData[i])/2]++;

			ASSERT(cbHuaCardIndex[SwitchToCardIndex(cbCardData[i])/2] <= 2);
			bHuaCardCount++;
		}
	}
	return bHuaCardCount;

}

////////////////////babala//////////
//��������������������Ƿ���Բ���
BYTE CGameLogic::AnalyseSiTong(BYTE cbCardIndex[MAX_INDEX],  tagSiTongResult & SiTongResult)
{
	BYTE cbActionMask=WIK_NULL;
	ZeroMemory(&SiTongResult,sizeof(SiTongResult));
	//bool bSiTong = false;

	//���������������Ƿ���������ͬ��,����еõ���ͳ�Ĳ���
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if (cbCardIndex[i] == 4 || cbCardIndex[i] == 5) 
		{
			SiTongResult.cbCardData[SiTongResult.cbCardCount++] = SwitchToCardData(i);
			cbActionMask |=WIK_SI_TONG;
			//bSiTong = true;
		}
	}
	return cbActionMask;
}

///////////////babala  11_15////////////////////////////////////
void CGameLogic::SaveHuaCard(BYTE cbCardData[],BYTE bCardCount,BYTE cbHuaCardIndex[])
{
	//�һ���
	for (BYTE i=0; i<bCardCount; i++)
	{
		//����ǻ��ƣ����浽��������������
		if ((cbCardData[i]&MASK_COLOR)==0x10)
		{
			BYTE index = SwitchToCardIndex(cbCardData[i]);
			cbHuaCardIndex[index/2]++;
		}
	}
}

//��������������������Ƿ���Բ��ƣ���ͳ
BYTE CGameLogic::AnalyseWuTong(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount,tagWuTongResult & WuTongResult)
{
	BYTE cbActionMask=WIK_NULL;
	ZeroMemory(&WuTongResult,sizeof(WuTongResult));

	//���������������Ƿ���������ͬ��,����еõ���ͳ�Ĳ���
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if (cbCardIndex[i] == 5) 
		{
			WuTongResult.cbCardData[WuTongResult.cbCardCount++] = SwitchToCardData(i);
			cbActionMask |=WIK_WU_TONG;
		}
	}

	//������в��Ʒ���
	for (BYTE j=0; j<cbItemCount; j++)
	{
		BYTE cbWeaveKind = WeaveItem[j].cbWeaveKind;
		BYTE cbCenterCardIndex = SwitchToCardIndex( WeaveItem[j].cbCenterCard );
		//Ҫ��������ж�Ӧ���Ʋ���Ϊ���У����������ƶ�Ӧ����λ�õ�����Ϊ1
		if ((cbCardIndex[cbCenterCardIndex] == 1) && (cbWeaveKind == WIK_SAN_ZHAO))
		{
			WuTongResult.cbCardData[WuTongResult.cbCardCount++] = SwitchToCardData(cbCenterCardIndex);
			cbActionMask |= WIK_WU_TONG;
		}
	}
	return cbActionMask;
}

//�˿�ת����������
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex)
{
	ASSERT((cbCardIndex >=0) && (cbCardIndex<22));

	if((cbCardIndex >= 0)&&(cbCardIndex<10))
	{
		return 0x00|(cbCardIndex+1);
	}
	else
	{
		return 0x20|(cbCardIndex-9);
	}
}

//�˿�ת�����Ƶ���
BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData)
{
	ASSERT(IsValidCard(cbCardData));

	BYTE cbValue=(cbCardData&MASK_VALUE);
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;

	//��������֣�ֱ�ӵõ����һλֵ
	if (cbColor == 0 || cbColor==1) return cbValue-1;
	else return  cbValue+9;
}

//�˿�ת��,
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT], BYTE cbHuaCardIndex[])
{
	//BYTE cbTempIndex[5];
	///CopyMemory(cbTempIndex,cbHuaCardIndex,sizeof(cbHuaCardIndex));
	//ת���˿�
	BYTE cbPosition=0;
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if (cbCardIndex[i]!=0)
		{
			ASSERT(cbPosition<MAX_COUNT);
			//����Ǿ��ƣ����⴦��
			if (i==0 || i==2 || i==4 || i==6 || i==8 )
			{
				//�����������л��Ƹ������浽��������
				for (BYTE k=0; k<cbHuaCardIndex[i/2]; k++)
				{
					cbCardData[cbPosition++]=0x10|(i+1);
				}
				//��ʣ�µĻ���Ƥ��
				for (k=0; k<(cbCardIndex[i]-cbHuaCardIndex[i/2]); k++)
				{
					cbCardData[cbPosition++]=0x00|(i+1);
				}
			}
			//��ͨ�ƴ���
			else
			{
				for (BYTE j=0;j<cbCardIndex[i];j++)
				{
					cbCardData[cbPosition++]=SwitchToCardData(i);
				}
			}

		}
	}

	return cbPosition;
}
//�˿�ת��
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex[], BYTE cbCardCount, BYTE cbCardData[], BYTE cbHuaCardIndex[])
{
	for(BYTE cbCount=0;cbCount<cbCardCount;cbCount++)
	{
		BYTE cbIndex=cbCardIndex[cbCount];
		//����Ǿ��ƣ����⴦��
		if (cbIndex==0 || cbIndex==2 || cbIndex==4 || cbIndex==6 || cbIndex==8 )
		{
			//����˾��ƵĻ��Ƹ�����Ϊ0������ת���ɻ���
			if(cbHuaCardIndex[cbIndex/2]>0)
			{
				cbCardData[cbCount]=0x10|(cbIndex+1);
				cbHuaCardIndex[cbIndex/2]--;
			}
			else
			{
				cbCardData[cbCount]=0x00|(cbIndex+1);
			}
		}
		else
		{
			cbCardData[cbCount]=SwitchToCardData(cbIndex);
		}
	}
	return cbCardCount;
}
//�˿�ת��
BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardIndex[MAX_INDEX])
{
	//���ñ���
	ZeroMemory(cbCardIndex,sizeof(BYTE)*MAX_INDEX);

	//ת���˿�
	for (BYTE i=0;i<cbCardCount;i++)
	{
		ASSERT(IsValidCard(cbCardData[i]));
		cbCardIndex[SwitchToCardIndex(cbCardData[i])]++;
	}

	return cbCardCount;
}
///////babala 11_16wna
//�жϼ����˴��������ƺ��Ƿ񹹳�Ѿ��
bool CGameLogic:: IsYaKou(BYTE cbCardIndex[MAX_INDEX],BYTE &FirstIndex,BYTE &SecondeIndex)
{
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//ͳ�������еĸ���ҪΪ2
	BYTE cbCardCount=0;
	for (BYTE i=0;i<MAX_INDEX;i++) cbCardCount+=cbCardIndexTemp[i];

	//Ѿ��Ϊ��仰
	for (BYTE i=0;i<MAX_INDEX;i++)
	{			
		if (cbCardIndexTemp[i] != 0)
		{
			FirstIndex = i;
			cbCardIndexTemp[i]--;
			break;
		}
	}
	//����һ����
	for (BYTE i=0;i<MAX_INDEX;i++)
	{			
		if (cbCardIndexTemp[i] != 0)
		{
			SecondeIndex = i;
			cbCardIndexTemp[i]--;
			break;
		}
	}
	//�ж��ǲ���Ѿ�ڣ�
	//1.��������ƶ������ֵ�����£��������ֵ����<=2  ���������ߣ�ʮ��
	if (FirstIndex < 10 && SecondeIndex <10)
	{
		//�������ߣ�ʮ
		if ((FirstIndex == 6 && SecondeIndex == 9) || (FirstIndex == 9 && SecondeIndex == 6 ) ) return true;

		//������������������,�������ֵ����<=2 
		if( abs(FirstIndex-SecondeIndex) <= 2 ) return true;
	}
	//2.�����ֺ��ֵ������
	//��,�ң���
	if ((FirstIndex == 10) && (SecondeIndex == 10 || SecondeIndex == 0 || SecondeIndex == 11 )) return true;
	if ((FirstIndex == 0) && (SecondeIndex == 10 || SecondeIndex == 0 || SecondeIndex == 11 )) return true;
	if ((FirstIndex == 11) && (SecondeIndex == 10 || SecondeIndex == 0 || SecondeIndex == 11 )) return true;

	//��,��,ǧ
	if ((FirstIndex == 16) && (SecondeIndex == 16 || SecondeIndex == 2 || SecondeIndex == 17 )) return true;
	if ((FirstIndex == 2) && (SecondeIndex == 16 || SecondeIndex == 2 || SecondeIndex == 17 )) return true;
	if ((FirstIndex == 17) && (SecondeIndex == 16 || SecondeIndex == 2 || SecondeIndex == 17 )) return true;

	//��ʮ��
	if ((FirstIndex == 6) && (SecondeIndex == 6 || SecondeIndex == 9 || SecondeIndex == 15 )) return true;
	if ((FirstIndex == 9) && (SecondeIndex == 6 || SecondeIndex == 9 || SecondeIndex == 15 )) return true;
	if ((FirstIndex == 15) && (SecondeIndex == 6 || SecondeIndex == 9 || SecondeIndex == 15 )) return true;

	//�˾���
	if ((FirstIndex == 7) && (SecondeIndex == 7 || SecondeIndex == 8 || SecondeIndex == 21 )) return true;
	if ((FirstIndex == 8) && (SecondeIndex == 7 || SecondeIndex == 8 || SecondeIndex == 21 )) return true;
	if ((FirstIndex == 21) && (SecondeIndex == 7 || SecondeIndex == 8 || SecondeIndex == 21 )) return true;

	return false;


}
