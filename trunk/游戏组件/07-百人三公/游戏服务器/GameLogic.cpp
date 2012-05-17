#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//扑克数据
const BYTE CGameLogic::m_cbCardListData[54]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//黑桃 A - K
	0x4E,0x4F															//小王、大王
};

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameLogic::CGameLogic()
{
}

//析构函数
CGameLogic::~CGameLogic()
{
}

//混乱扑克
void CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	//混乱准备
	BYTE cbCardData[CountArray(m_cbCardListData)];
	CopyMemory(cbCardData,m_cbCardListData,sizeof(m_cbCardListData));

	//混乱扑克
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(CountArray(cbCardData)-cbRandCount);
		cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
		cbCardData[cbPosition]=cbCardData[CountArray(cbCardData)-cbRandCount];
	} while (cbRandCount<cbBufferCount);

	return;
}

//获取牌点
BYTE CGameLogic::GetCardPip(BYTE cbCardData)
{
	//计算牌点
	BYTE cbCardValue=GetCardValue(cbCardData);
	BYTE cbPipCount=(cbCardValue>=10)?0:cbCardValue;

	return cbPipCount;
}

//获取牌点
BYTE CGameLogic::GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount)
{
	//变量定义
	BYTE cbPipCount=0;

	//获取牌点
	for (BYTE i=0;i<cbCardCount;i++)
	{
		cbPipCount=(GetCardPip(cbCardData[i])+cbPipCount)%10;
	}

	return cbPipCount;
}

//有效判断
bool CGameLogic::IsValidCard(BYTE cbCardData)
{
	//获取属性
	BYTE cbCardColor=GetCardColor(cbCardData);
	BYTE cbCardValue=GetCardValue(cbCardData);

	//有效判断
	if ((cbCardData==0x4E)||(cbCardData==0x4F)) return true;
	if ((cbCardColor<=0x30)&&(cbCardValue>=0x01)&&(cbCardValue<=0x0D)) return true;

	return false;
}

//排序
void CGameLogic::SortCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	if(cbBufferCount == 0)
		return;

	for(BYTE i=0; i<cbBufferCount-1; i++)
	{
		for(BYTE j=i+1; j<cbBufferCount; j++)
		{
			if(GetCardValue(cbCardBuffer[i]) < GetCardValue(cbCardBuffer[j]))
			{
				//交换位置
				BYTE cbTemp = cbCardBuffer[i];
				cbCardBuffer[i] = cbCardBuffer[j];
				cbCardBuffer[j] = cbTemp;
			}
		}
	}
}

//获取单张牌的点数
BYTE CGameLogic::GetCardPoint(BYTE cbCardData)
{
	BYTE cbCardPoint = GetCardValue(cbCardData);

	if(cbCardPoint > 10 )
		return 0;
	else
		return cbCardPoint;
}

//获取牌的类型
BYTE CGameLogic::GetCardType(const BYTE cbCardBuffer[], const BYTE cbBufferCount)
{
	if(cbBufferCount != 3)
		return CARDTYPE_ERROR;

	if( IsSanZhang(cbCardBuffer, cbBufferCount) )
		return CARDTYPE_SAN_ZHANG;
	else if( IsSanGong(cbCardBuffer, cbBufferCount) )
		return CARDTYPE_SAN_GONG;
	else if( IsSanPai(cbCardBuffer, cbBufferCount) )
		return CARDTYPE_SAN_PAI;
	else if( IsBiShi(cbCardBuffer, cbBufferCount) )
		return CARDTYPE_BI_SHI;
	else
		return CARDTYPE_ERROR;	
}

//是否是三公
bool CGameLogic::IsSanGong(const BYTE cbCardBuffer[], const BYTE cbBufferCount)
{
	if(cbBufferCount != 3)
		return false;
	
	for(BYTE i=0; i<cbBufferCount; i++)
	{
		if(GetCardValue(cbCardBuffer[i]) < 0x0B)
			return false;
	}

	return true;
}

//是否三张
bool CGameLogic::IsSanZhang(const BYTE cbCardBuffer[], const BYTE cbBufferCount)
{
	if(cbBufferCount != 3)
		return false;

	BYTE cbValue1 = GetCardValue(cbCardBuffer[0]);
	BYTE cbValue2 = GetCardValue(cbCardBuffer[1]);
	BYTE cbValue3 = GetCardValue(cbCardBuffer[2]);

	/*::OutputDebugString("IsSanZhang");
	CString str;
	for(BYTE i=0; i<3; i++)
	{
		BYTE cbValue = GetCardValue(cbCardBuffer[i]);
		str.Format("%x  Value：%d", cbCardBuffer[i], cbValue);
		::OutputDebugString(str);
	}*/

	if( (cbValue1==cbValue2) && (cbValue1==cbValue3) )
		return true;
	else
		return false;
}

//是否是毙十
bool CGameLogic::IsBiShi(const BYTE cbCardBuffer[], const BYTE cbBufferCount)
{
	BYTE cbPoint1 = GetCardPip(cbCardBuffer[0]);
	BYTE cbPoint2 = GetCardPip(cbCardBuffer[1]);
	BYTE cbPoint3 = GetCardPip(cbCardBuffer[2]);

	if( (cbPoint1 + cbPoint2 + cbPoint3)%10 == 0)
	{
		if( IsSanGong(cbCardBuffer, cbBufferCount) || IsSanZhang(cbCardBuffer, cbBufferCount) )
			return false;
		else
			return true;
	}
	else
		return false;
}

//是否散牌
bool CGameLogic::IsSanPai(const BYTE cbCardBuffer[], const BYTE cbBufferCount)
{
	if(cbBufferCount != 3)
		return false;

	if( IsSanGong(cbCardBuffer, cbBufferCount) || IsSanZhang(cbCardBuffer, cbBufferCount) )
		return false;
	else
	{
		BYTE cbPoint1 = GetCardPip(cbCardBuffer[0]);
		BYTE cbPoint2 = GetCardPip(cbCardBuffer[1]);
		BYTE cbPoint3 = GetCardPip(cbCardBuffer[2]);

		if( (cbPoint1 + cbPoint2 + cbPoint3)%10 == 0)
			return false;
		else
			return true;
	}
}

//获取点数
BYTE CGameLogic::GetPoint(const BYTE cbCardBuffer[], const BYTE cbBufferCount)
{
	if(cbBufferCount != 3)
		return 0xFF;

	BYTE cbTotalPoint = 0;
	for( BYTE i=0; i<cbBufferCount; i++ )
	{
		cbTotalPoint += GetCardPoint(cbCardBuffer[i]);
	}

	return cbTotalPoint%10;
}

//比较牌大小
bool CGameLogic::IsBigger(const BYTE _cbFirstCard[], const BYTE _cbNextCard[], const BYTE cbFirstCount, const BYTE cbNextCount)
{
	/*::OutputDebugString("比较牌大小");
	CString str;
	str.Format("First: %x %x %x", _cbFirstCard[0], _cbFirstCard[1], _cbFirstCard[2] );
	::OutputDebugString(str);
	str.Format("Next: %x %x %x", _cbNextCard[0], _cbNextCard[1], _cbNextCard[2] );
	::OutputDebugString(str);*/

	BYTE cbFirstCard[3];
	BYTE cbNextCard[3];
	CopyMemory(cbFirstCard, _cbFirstCard, sizeof(cbFirstCard));
	CopyMemory(cbNextCard, _cbNextCard, sizeof(_cbNextCard));

	SortCardList(cbFirstCard, cbFirstCount);
	SortCardList(cbNextCard, cbNextCount);

	BYTE cbFirstType = GetCardType(cbFirstCard, cbFirstCount);
	BYTE cbNextType = GetCardType(cbNextCard, cbNextCount);

	/*str.Format("FirstType:%d", cbFirstType);
	::OutputDebugString(str);
	str.Format("NextType:%d", cbNextType);
	::OutputDebugString(str);*/

	//类型相同
	if(cbFirstType == cbNextType)
	{
		if(cbFirstType == CARDTYPE_SAN_ZHANG)	//三张
		{
			BYTE cbFirstValue = GetCardValue(cbFirstCard[0]);
			BYTE cbNextValue = GetCardValue(cbNextCard[0]);
			if(cbFirstValue == 3)
				return true;
			else if(cbNextValue == 3)
				return false;
			else
				return cbFirstValue > cbNextValue;
		}
		else if(cbFirstType == CARDTYPE_SAN_GONG)	//三公
		{
			BYTE cbFirstValue[3] = {GetCardValue(cbFirstCard[0]), GetCardValue(cbFirstCard[1]), GetCardValue(cbFirstCard[2])};
			BYTE cbNextValue[3] = {GetCardValue(cbNextCard[0]), GetCardValue(cbNextCard[1]), GetCardValue(cbNextCard[2])};

			for(BYTE i=0; i<3; i++)
			{
				if(cbFirstValue[i] != cbNextValue[i])
					return cbFirstValue[i] < cbNextValue[i];
				else
					continue;
			}

			return false;
		}
		else if(cbFirstType == CARDTYPE_SAN_PAI)	//散牌
		{
			return GetPoint(cbFirstCard, cbFirstCount) > GetPoint(cbNextCard, cbNextCount);
		}
		else if(cbFirstType == CARDTYPE_BI_SHI)		//毙十
		{
			return false;
		}
	}
	else
		return (cbFirstType<cbNextType);

	return true;
}

//获取番的倍数
BYTE CGameLogic::GetMultiple(const BYTE cbCardBuffer[], const BYTE cbBufferCount)
{
	BYTE cbMultiple = 1;

	if( IsSanZhang(cbCardBuffer, cbBufferCount) )
	{
		if(cbCardBuffer[0] == 3)
			cbMultiple = 5;
		else
			cbMultiple = 4;
	}
	else if(IsSanGong(cbCardBuffer, cbBufferCount))
		cbMultiple = 3;
	else
	{
		BYTE cbPoint = GetPoint(cbCardBuffer, cbBufferCount);
		if( cbPoint==8 || cbPoint==9 )
			cbMultiple = 2;
		else 
			cbMultiple = 1;
	}

	return cbMultiple;
}

//////////////////////////////////////////////////////////////////////////
