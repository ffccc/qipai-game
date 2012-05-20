#include "StdAfx.h"
#include "GameClient.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////

//静态变量
CPngImage					CCardControl::m_ImageCard[9];						//图片资源

//构造函数
CCardControl::CCardControl()
{
	//状态变量
	m_bSmallMode = false;
	m_bHorizontal = true;
	m_bPositively = false;
	m_bDisplayItem = false;
	m_bShowCardControl = true;
	m_blGameEnd = false;
	m_blhideOneCard = false;

	//选择变量
	m_wEndIndex = INVALID_ITEM;
	m_wStartIndex = INVALID_ITEM;
	m_dwCurrentIndex = INVALID_ITEM;

	//扑克数据
	m_wCardCount = 0;

	//位置变量
	m_YCollocateMode = enYTop;
	m_XCollocateMode = enXLeft;
	m_BenchmarkPos.SetPoint(0, 0);
	m_MovePoint.SetPoint(0, 0);

	//间隔变量
	m_nXDistance = (m_bSmallMode == false) ? DEF_X_DISTANCE : DEF_X_DISTANCE_SMALL;
	m_nYDistance = (m_bSmallMode == false) ? DEF_Y_DISTANCE : DEF_Y_DISTANCE_SMALL;
	m_nShootDistance = (m_bSmallMode == false) ? DEF_SHOOT_DISTANCE : DEF_SHOOT_DISTANCE_SMALL;

	//加载资源
	HINSTANCE hResInstance = AfxGetInstanceHandle();

	if (m_ImageCard[0].IsNull()) m_ImageCard[0].LoadImage(hResInstance, "UP_1_2");
	if (m_ImageCard[1].IsNull()) m_ImageCard[1].LoadImage(hResInstance, "UP_2_2");
	if (m_ImageCard[2].IsNull()) m_ImageCard[2].LoadImage(hResInstance, "UP_3_2");
	if (m_ImageCard[3].IsNull()) m_ImageCard[3].LoadImage(hResInstance, "UP_4_2");
	if (m_ImageCard[4].IsNull()) m_ImageCard[4].LoadImage(hResInstance, "UP_5_2");
	if (m_ImageCard[5].IsNull()) m_ImageCard[5].LoadImage(hResInstance, "UP_6_2");
	if (m_ImageCard[6].IsNull()) m_ImageCard[6].LoadImage(hResInstance, "UP_7_2");
	if (m_ImageCard[7].IsNull()) m_ImageCard[7].LoadImage(hResInstance, "UP_8_2");
	if (m_ImageCard[8].IsNull()) m_ImageCard[8].LoadImage(hResInstance, "UP_9_2");


	m_ImageOpenCardHandle.LoadImage(hResInstance, "IDB_HAND");

	//获取大小
	//CImageHandle HandleCard(&m_ImageCard);
	m_CardSize.SetSize(m_ImageCard[0].GetWidth() / 8, m_ImageCard[0].GetHeight());

	memset(m_nOpenIndex, 0, sizeof(m_nOpenIndex));

	return;
}

//析构函数
CCardControl::~CCardControl()
{

}

//设置扑克
bool CCardControl::SetCardData(WORD wCardCount)
{
	//效验参数
	ASSERT(wCardCount <= m_CardItemArray.GetCount());
	if (wCardCount > m_CardItemArray.GetCount()) return false;

	m_CardItemArray.SetSize(wCardCount);
	//设置变量
	m_wCardCount = (WORD)m_CardItemArray.GetCount();
	memset(m_nOpenIndex, 0, sizeof(m_nOpenIndex));

	return true;
}

//设置模式
bool CCardControl::SetCardMode(bool bSmallMode)
{
	//设置模式
	if (m_bSmallMode != bSmallMode)
	{
		//设置变量
		m_bSmallMode = bSmallMode;

		//设置间距
		m_nXDistance = (m_bSmallMode == false) ? DEF_X_DISTANCE : DEF_X_DISTANCE_SMALL;
		m_nYDistance = (m_bSmallMode == false) ? DEF_Y_DISTANCE : DEF_Y_DISTANCE_SMALL;
		m_nShootDistance = (m_bSmallMode == false) ? DEF_SHOOT_DISTANCE : DEF_SHOOT_DISTANCE_SMALL;

		//加载资源
		HINSTANCE hResInstance = AfxGetInstanceHandle();

		if (m_ImageCard[0].IsNull()) m_ImageCard[0].LoadImage(hResInstance, "UP_1_2");
		if (m_ImageCard[1].IsNull()) m_ImageCard[1].LoadImage(hResInstance, "UP_2_2");
		if (m_ImageCard[2].IsNull()) m_ImageCard[2].LoadImage(hResInstance, "UP_3_2");
		if (m_ImageCard[3].IsNull()) m_ImageCard[3].LoadImage(hResInstance, "UP_4_2");
		if (m_ImageCard[4].IsNull()) m_ImageCard[4].LoadImage(hResInstance, "UP_5_2");
		if (m_ImageCard[5].IsNull()) m_ImageCard[5].LoadImage(hResInstance, "UP_6_2");
		if (m_ImageCard[6].IsNull()) m_ImageCard[6].LoadImage(hResInstance, "UP_7_2");
		if (m_ImageCard[7].IsNull()) m_ImageCard[7].LoadImage(hResInstance, "UP_8_2");
		if (m_ImageCard[8].IsNull()) m_ImageCard[8].LoadImage(hResInstance, "UP_9_2");

		//获取大小
		//CImageHandle HandleCard(&m_ImageCard);
		m_CardSize.SetSize(m_ImageCard[0].GetWidth() / 8, m_ImageCard[0].GetHeight());
	}

	return true;
}

//设置扑克
bool CCardControl::SetCardData(const BYTE cbCardData[], WORD wCardCount, bool blTmp)
{
	if (blTmp == false)
	{
		ASSERT(wCardCount <= CountArray(m_CardItemArray));
		if (wCardCount > CountArray(m_CardItemArray)) return false;
		m_CardItemArray.SetSize(wCardCount);
		//设置扑克
		for (WORD i = 0; i < wCardCount; i++)
		{
			m_CardItemArray[i].bShoot = false;
			m_CardItemArray[i].cbCardData = cbCardData[i];
		}
		return true;


	}
	if (wCardCount == 0)
	{
		m_CardItemArray.SetSize(0);
	}
	m_CardTempItemArray.SetSize(wCardCount);
	//效验参数
	ASSERT(wCardCount <= CountArray(m_CardTempItemArray));
	if (wCardCount > CountArray(m_CardTempItemArray)) return false;

	//设置变量
	m_wEndIndex = INVALID_ITEM;
	m_wStartIndex = INVALID_ITEM;

	//扑克数目
	m_wCardCount = 0;

	//设置扑克
	for (WORD i = 0; i < wCardCount; i++)
	{
		m_CardTempItemArray[i].bShoot = false;
		m_CardTempItemArray[i].cbCardData = cbCardData[i];
	}

	return true;
}

//设置扑克
bool CCardControl::SetCardItem(const tagCardItem CardItemArray[], WORD wCardCount)
{
	//效验参数
	ASSERT(wCardCount <= CountArray(m_CardItemArray));
	if (wCardCount > CountArray(m_CardItemArray)) return false;

	//设置变量
	m_wEndIndex = INVALID_ITEM;
	m_wStartIndex = INVALID_ITEM;

	//设置扑克
	m_wCardCount = wCardCount;
	m_CardItemArray.SetSize(wCardCount);
	for (int i = 0; i < wCardCount; i++)
	{
		m_CardItemArray[i] = CardItemArray[i];
	}
	return true;
}

//设置扑克
bool CCardControl::SetShootIndex(WORD wStartIndex, WORD wEndIndex)
{
	//设置变量
	m_wEndIndex = INVALID_ITEM;
	m_wStartIndex = INVALID_ITEM;

	//设置扑克
	for (WORD i = 0; i < m_wCardCount; i++)
	{
		if ((i >= wStartIndex) && (i <= wEndIndex))
		{
			m_CardItemArray[i].bShoot = true;
		}
	}

	return true;
}

//设置扑克
bool CCardControl::SetSelectIndex(WORD wStartIndex, WORD wEndIndex)
{
	//设置变量
	m_wEndIndex = wEndIndex;
	m_wStartIndex = wStartIndex;

	return true;
}

//设置扑克
bool CCardControl::SetShootCard(const BYTE cbCardData[], WORD wCardCount)
{
	//变量定义
	bool bChangeStatus = false;

	//收起扑克
	for (WORD i = 0; i < m_wCardCount; i++)
	{
		if (m_CardItemArray[i].bShoot == true)
		{
			bChangeStatus = true;
			m_CardItemArray[i].bShoot = false;
		}
	}

	//弹起扑克
	for (WORD i = 0; i < wCardCount; i++)
	{
		for (WORD j = 0; j < m_wCardCount; j++)
		{
			if ((m_CardItemArray[j].bShoot == false) && (m_CardItemArray[j].cbCardData == cbCardData[i]))
			{
				bChangeStatus = true;
				m_CardItemArray[j].bShoot = true;
				break;
			}
		}
	}

	return bChangeStatus;
}

//删除扑克
bool CCardControl::RemoveShootItem()
{
	//设置变量
	m_wEndIndex = INVALID_ITEM;
	m_wStartIndex = INVALID_ITEM;

	//保存扑克
	tagCardItem	CardItemArray[MAX_CARD_COUNT];

	WORD wCardCount = (WORD)m_CardItemArray.GetCount();

	m_wCardCount = wCardCount;

	for (int i = 0; i < wCardCount; i++)
	{
		CardItemArray[i] = m_CardItemArray[i];
	}
	//删除扑克
	WORD wRemoveCount = 0;
	for (WORD i = 0; i < m_wCardCount; i++)
	{
		if (CardItemArray[i].bShoot == true)
		{
			wRemoveCount++;
			CardItemArray[i].cbCardData = 0x00;
		}
	}

	//设置扑克
	if (wRemoveCount > 0)
	{
		//设置扑克
		WORD wInsertCount = 0;
		for (WORD i = 0; i < m_wCardCount; i++)
		{
			if (CardItemArray[i].cbCardData != 0x00)
			{
				m_CardItemArray[wInsertCount++] = CardItemArray[i];
			}
		}

		//设置变量
		m_wCardCount = m_wCardCount - wRemoveCount;
	}

	return true;
}

//获取扑克
tagCardItem * CCardControl::GetCardFromIndex(WORD wIndex)
{
	return (wIndex < m_wCardCount) ? &m_CardItemArray[wIndex] : NULL;
}

//获取扑克
tagCardItem * CCardControl::GetCardFromPoint(CPoint & MousePoint)
{
	WORD wIndex = SwitchCardPoint(MousePoint);
	return (wIndex != INVALID_ITEM) ? &m_CardItemArray[wIndex] : NULL;
}

//获取扑克
WORD CCardControl::GetCardData(BYTE cbCardData[], WORD wBufferCount)
{
	//效验参数
	ASSERT(wBufferCount >= m_wCardCount);
	if (wBufferCount < m_wCardCount) return 0;

	//拷贝扑克
	for (WORD i = 0; i < m_wCardCount; i++) cbCardData[i] = m_CardItemArray[i].cbCardData;

	return m_wCardCount;
}

//获取扑克
WORD CCardControl::GetShootCard(BYTE cbCardData[], WORD wBufferCount)
{
	//变量定义
	WORD wShootCount = 0;

	//拷贝扑克
	for (WORD i = 0; i < m_wCardCount; i++)
	{
		//效验参数
		ASSERT(wBufferCount > wShootCount);
		if (wBufferCount <= wShootCount) break;

		//拷贝扑克
		if (m_CardItemArray[i].bShoot == true) cbCardData[wShootCount++] = m_CardItemArray[i].cbCardData;
	}

	return wShootCount;
}

//获取扑克
WORD CCardControl::GetCardData(tagCardItem CardItemArray[], WORD wBufferCount)
{
	//效验参数
	ASSERT(wBufferCount >= m_wCardCount);
	if (wBufferCount < m_wCardCount) return 0;

	m_wCardCount = (WORD)m_CardItemArray.GetCount();
	for (int i = 0; i < m_wCardCount; i++)
	{
		CardItemArray[i] = m_CardItemArray[i];

	}
	return m_wCardCount;
}

//设置距离
VOID CCardControl::SetCardSpace(UINT nXDistance, UINT nYDistance, UINT nShootDistance)
{
	//设置变量
	m_nXDistance = nXDistance;
	m_nYDistance = nYDistance;
	m_nShootDistance = nShootDistance;

	return;
}

//获取中心
VOID CCardControl::GetCenterPoint(CPoint & CenterPoint)
{
	//获取原点
	CPoint OriginPoint;
	GetOriginPoint(OriginPoint);

	//获取位置
	CSize ControlSize;
	GetControlSize(ControlSize);

	//设置中心
	CenterPoint.x = OriginPoint.x + ControlSize.cx / 2;
	CenterPoint.y = OriginPoint.y + ControlSize.cy / 2;

	return;
}

//基准位置
VOID CCardControl::SetBenchmarkPos(INT nXPos, INT nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//设置变量
	m_BenchmarkPos.x = nXPos;
	m_BenchmarkPos.y = nYPos;
	m_XCollocateMode = XCollocateMode;
	m_YCollocateMode = YCollocateMode;

	return;
}

//基准位置
VOID CCardControl::SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//设置变量
	m_BenchmarkPos = BenchmarkPos;
	m_XCollocateMode = XCollocateMode;
	m_YCollocateMode = YCollocateMode;
	return;
}

//绘画扑克
VOID CCardControl::DrawCardControl(CDC * pDC)
{
	//显示判断
	if (m_bShowCardControl == false) return;

	//获取位置
	CPoint OriginPoint;
	GetOriginPoint(OriginPoint);

	//变量定义
	INT nXDrawPos = 0, nYDrawPos = 0;
	INT nXImagePos = 0, nYImagePos = 0;

	m_wCardCount = (WORD)m_CardItemArray.GetCount();

	//绘画扑克
	for (WORD i = 0; i < m_wCardCount; i++)
	{
		//获取扑克
		bool bShoot = m_CardItemArray[i].bShoot;
		BYTE cbCardData = m_CardItemArray[i].cbCardData;

		//间隙过滤
		if (cbCardData == SPACE_CARD_DATA || cbCardData <= 0 || cbCardData > 9) continue;

		//图片位置
		if ((m_bDisplayItem == true) && (cbCardData >= 1 && cbCardData <= 9))
		{
			nXImagePos = (cbCardData & CARD_MASK_VALUE) * m_CardSize.cx;
			nYImagePos = 0;

			nXDrawPos += i * m_CardSize.cx;
		}
		else
		{
			nXImagePos = 0;
			nYImagePos = 0;

			nXDrawPos += i * m_CardSize.cx;
		}

		//绘画扑克
		if (m_nOpenIndex[i] >= OPEN_CARD_MAX_INDEX)
			m_ImageCard[cbCardData - 1].DrawImage(pDC, OriginPoint.x + nXDrawPos + 25, OriginPoint.y + nYDrawPos, m_CardSize.cx, m_CardSize.cy, 440 / 8 * 7, 0, 55, 151);
		else
			m_ImageCard[cbCardData - 1].DrawImage(pDC, OriginPoint.x + nXDrawPos + 25, OriginPoint.y + nYDrawPos, m_CardSize.cx, m_CardSize.cy, 440 / 8 * m_nOpenIndex[i], 0, 55, 151);
	}

	return;
}

//光标消息
bool CCardControl::OnEventSetCursor(CPoint Point)
{
	return false;
}

//获取大小
VOID CCardControl::GetControlSize(CSize & ControlSize, WORD dwCardCount)
{
	if (dwCardCount != 0)
	{
		//获取大小
		if (m_bHorizontal == true)
		{
			ControlSize.cy = 2 * m_CardSize.cy + m_nShootDistance;
			ControlSize.cx = (dwCardCount > 0) ? (m_CardSize.cx + (dwCardCount / 2 + 1 - 1) * m_nXDistance * 2) : 0;
		}
		else
		{
			ControlSize.cx = m_CardSize.cx;
			ControlSize.cy = (dwCardCount > 0) ? (m_CardSize.cy + (dwCardCount - 1) * m_nYDistance) : 0;
		}
		return ;

	}
	m_wCardCount = (WORD)m_CardItemArray.GetCount();
	//获取大小
	if (m_bHorizontal == true)
	{
		ControlSize.cy = 2 * m_CardSize.cy + m_nShootDistance;
		ControlSize.cx = (m_wCardCount > 0) ? (m_CardSize.cx + (m_wCardCount / 2 + 1 - 1) * m_nXDistance * 2) : 0;
	}
	else
	{
		ControlSize.cx = m_CardSize.cx;
		ControlSize.cy = (m_wCardCount > 0) ? (m_CardSize.cy + (m_wCardCount - 1) * m_nYDistance) : 0;
	}

	return;
}

//获取原点
VOID CCardControl::GetOriginPoint(CPoint & OriginPoint)
{
	//获取位置
	CSize ControlSize;
	GetControlSize(ControlSize);

	//横向位置
	switch (m_XCollocateMode)
	{
		case enXLeft:
		{
			OriginPoint.x = m_BenchmarkPos.x;
			break;
		}
		case enXCenter:
		{
			OriginPoint.x = m_BenchmarkPos.x - ControlSize.cx / 2;
			break;
		}
		case enXRight:
		{
			OriginPoint.x = m_BenchmarkPos.x - ControlSize.cx;
			break;
		}
	}

	//竖向位置
	switch (m_YCollocateMode)
	{
		case enYTop:
		{
			OriginPoint.y = m_BenchmarkPos.y;
			break;
		}
		case enYCenter:
		{
			OriginPoint.y = m_BenchmarkPos.y - ControlSize.cy / 2;
			break;
		}
		case enYBottom:
		{
			OriginPoint.y = m_BenchmarkPos.y - ControlSize.cy;
			break;
		}
	}

	return;
}

//鼠标消息
void CCardControl::OnMouseMove(UINT nFlags, CPoint wndPoint)
{

	if (m_blAuto)
		return;
	if (m_dwCurrentIndex == INVALID_ITEM)
	{
		m_MovePoint.SetPoint(0, 0);
		m_DownPoint.SetPoint(0, 0);
		m_dwCurrentIndex = INVALID_ITEM;
		return ;

	}
	if (m_dwCurrentIndex >= 1 && m_dwCurrentIndex != INVALID_ITEM)
	{
		CSize ControlSize;
		GetControlSize(ControlSize);
		CPoint OriginPoint;
		GetOriginPoint(OriginPoint);

		//基准位置
		bool bldecY = false;
		INT nXPos = wndPoint.x - m_DownPoint.x;
		INT nYPos = wndPoint.y - m_DownPoint.y;
		if (nYPos < 0)
		{
			nYPos = 0;

		}
		if (nXPos < 0)
		{
			nXPos = 0;
		}

		if (nXPos > 40)
		{
			nXPos = 40;

		}
		if (nYPos > m_CardSize.cy / 2)
		{
			nYPos = m_CardSize.cy / 2;
		}
		m_MovePoint.y =  nYPos;
		m_MovePoint.x = nXPos;
	}
	return;
}
afx_msg void CCardControl::OnLButtonUp(UINT nFlags, CPoint wndPoint)
{

	CPoint OriPoint;
	GetOriginPoint(OriPoint);
	int x = wndPoint.x ;
	int y = wndPoint.y;
	CPoint Point(x, y);
	m_MovePoint.SetPoint(0, 0);
	m_DownPoint.SetPoint(0, 0);
	m_dwCurrentIndex = INVALID_ITEM;
	return ;

}
void CCardControl::OnLButtonDown(UINT nFlags, CPoint wndPoint)
{


	CPoint OriPoint;
	GetOriginPoint(OriPoint);
	int x = wndPoint.x ;
	int y = wndPoint.y;
	CPoint Point(x, y);
	m_dwDragMax = DWORD(-1);
	//获取索引
	m_dwCurrentIndex = SwitchCardPoint(Point);
	if (m_dwCurrentIndex == INVALID_ITEM && m_dwCurrentIndex > 2)
	{
		printf("");
	}
	else
	{
		m_DownPoint.SetPoint(wndPoint.x, wndPoint.y);
	}

	return;
}
//索引切换
WORD CCardControl::SwitchCardPoint(CPoint & MousePoint)
{
	//横向模式
	if ((m_bHorizontal == true) && (m_wCardCount > 0))
	{
		//获取位置
		CSize ControlSize;
		CPoint OriginPoint;
		GetControlSize(ControlSize);
		GetOriginPoint(OriginPoint);

		//基准位置
		INT nXPos = MousePoint.x - OriginPoint.x;
		INT nYPos = MousePoint.y - OriginPoint.y;

		//越界判断
		if ((nXPos < 0) || (nXPos > ControlSize.cx)) return INVALID_ITEM;
		if ((nYPos < 0) || (nYPos > ControlSize.cy)) return INVALID_ITEM;

		//计算索引
		WORD wCardIndex = nXPos / m_nXDistance;
		if (wCardIndex >= m_wCardCount) wCardIndex = (m_wCardCount - 1);

		//扑克搜索
		for (WORD i = 0; i <= wCardIndex; i++)
		{
			//变量定义
			WORD wCurrentIndex = wCardIndex - i;

			//横向测试
			if (nXPos > (INT)(wCurrentIndex*m_nXDistance + m_CardSize.cx)) break;

			//竖向测试
			bool bShoot = m_CardItemArray[wCurrentIndex].bShoot;
			if ((bShoot == true) && (nYPos <= m_CardSize.cy)) return wCurrentIndex;
			if ((bShoot == false) && (nYPos >= (INT)m_nShootDistance)) return wCurrentIndex;
		}
	}

	return INVALID_ITEM;
}

//移动扑克
bool CCardControl::MoveCardItem(WORD wTargerItem)
{
	//效验参数
	ASSERT(wTargerItem < m_wCardCount);
	if (wTargerItem >= m_wCardCount) return false;

	//统计扑克
	WORD wShootCount = 0;
	for (WORD i = 0; i < m_wCardCount; i++)
	{
		if (m_CardItemArray[i].bShoot == true)
		{
			wShootCount++;
		}
	}

	//移动扑克
	if ((wShootCount > 0) && (wShootCount < m_wCardCount))
	{
		//调整索引
		wTargerItem = __min(m_wCardCount - wShootCount, wTargerItem);

		//变量定义
		WORD wShootIndex = wTargerItem;
		WORD wNormalIndex = (wTargerItem == 0) ? (wTargerItem + wShootCount) : 0;

		//提取扑克
		tagCardItem CardItemTemp[MAX_CARD_COUNT];
		m_wCardCount = (WORD)m_CardItemArray.GetCount();
		for (int i = 0; i < m_wCardCount; i++)
		{
			CardItemTemp[i] = m_CardItemArray[i];

		}
		//调整扑克
		for (WORD i = 0; i < m_wCardCount; i++)
		{
			if (CardItemTemp[i].bShoot == false)
			{
				//设置扑克
				m_CardItemArray[wNormalIndex++] = CardItemTemp[i];

				//调整索引
				if (wNormalIndex == wTargerItem) wNormalIndex = wTargerItem + wShootCount;
			}
			else
			{
				//设置扑克
				m_CardItemArray[wShootIndex++] = CardItemTemp[i];
			}
		}

		return true;
	}

	return false;
}
//交换扑克
bool CCardControl::SwitchCardItem(WORD wSourceItem, WORD wTargerItem)
{
	//效验参数
	if (wSourceItem == wTargerItem) return false;
	if ((wSourceItem >= m_wCardCount) || (wTargerItem >= m_wCardCount)) return false;

	//保存扑克
	tagCardItem CardItem = m_CardItemArray[wSourceItem];

	//移动扑克
	if (wSourceItem > wTargerItem)
	{
		WORD wMoveCount = wSourceItem - wTargerItem;
		MoveMemory(&m_CardItemArray[wTargerItem+1], &m_CardItemArray[wTargerItem], sizeof(tagCardItem)*wMoveCount);
	}
	else
	{
		WORD wMoveCount = wTargerItem - wSourceItem;
		MoveMemory(&m_CardItemArray[wSourceItem], &m_CardItemArray[wSourceItem+1], sizeof(tagCardItem)*wMoveCount);
	}

	//插入目标
	m_CardItemArray[wTargerItem] = CardItem;

	return true;
}
//拷贝缓存中的牌实现动画
void CCardControl::OnCopyCard()
{
	WORD wCount = (WORD) m_CardItemArray.GetCount();
	m_blhideOneCard = false;
	if (m_CardTempItemArray.GetCount() > wCount)
	{
		m_blGameEnd = true;
		m_CardItemArray.Add(m_CardTempItemArray[wCount]);
	}
	//zhangchen
	memset(m_nOpenIndex, 0, sizeof(m_nOpenIndex));
}
//弹起扑克
void CCardControl::ShootAllCard(bool bShoot)
{
	if (bShoot == true)
	{
		SetShootIndex(0, m_wCardCount - 1);
	}
	else
	{
		for (WORD i = 0; i < m_wCardCount; i++)
		{
			m_CardItemArray[i].bShoot = false;
		}
	}
}
//////////////////////////////////////////////////////////////////////////

