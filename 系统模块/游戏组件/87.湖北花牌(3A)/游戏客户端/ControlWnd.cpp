#include "StdAfx.h"
#include "GameLogic.h"
#include "ControlWnd.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////

//按钮标识
#define IDC_CHIHU					100						//吃胡按钮
#define IDC_GIVEUP					101						//放弃按钮
#define ITEM_WIDTH					18						//子项宽度
#define ITEM_HEIGHT					58						//子项高度

#define CONTROL_WIDTH				173						//控制宽度
#define CONTROL_HEIGHT				41						//控制高度

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()

	ON_BN_CLICKED(IDC_CHIHU, OnChiHu)
	ON_BN_CLICKED(IDC_GIVEUP, OnGiveUp)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CControlWnd::CControlWnd()
{
	//配置变量
	m_cbActionMask=0;
	m_cbCenterCard=0;
	m_PointBenchmark.SetPoint(0,0);
	m_cbDuiHuaNm=0;
	m_cbSanZhaoNm=0;
	m_cbSiZhaoNm=0;
	ZeroMemory(m_cbsiTongHuaNm,sizeof(m_cbsiTongHuaNm));
	ZeroMemory(m_cbWuTongHuaNm,sizeof(m_cbWuTongHuaNm));

	//状态变量
	m_cbItemCount=0;
	m_cbCurrentItem=0xFF;

	//加载资源
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageControlTop.SetLoadInfo(IDB_CONTROL_TOP,hInstance);
	m_ImageControlMid.SetLoadInfo(IDB_CONTROL_MID,hInstance);
	m_ImageControlButtom.SetLoadInfo(IDB_CONTROL_BOTTOM,hInstance);
	m_ImageActionExplain.SetLoadInfo(IDB_ACTION_EXPLAIN,hInstance);

	return;
}

//析构函数
CControlWnd::~CControlWnd()
{
}

//基准位置
void CControlWnd::SetBenchmarkPos(int nXPos, int nYPos)
{
	//位置变量
	m_PointBenchmark.SetPoint(nXPos,nYPos);

	//调整控件
	RectifyControl();

	return;
}
BYTE CControlWnd::SwitchToCardIndex(BYTE cbCardData)
{
	BYTE cbValue=(cbCardData&MASK_VALUE);
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;

	//确保是合法的牌数据
	ASSERT(((cbColor == 0 || cbColor == 1) && (cbValue>0 && cbValue<=10) ) || ((cbColor == 2) && (cbValue>=1 && cbValue<=12)));
	
	//如果是数字，直接得到最后一位值
	if (cbColor == 0 || cbColor==1) return cbValue-1;
	else return  cbValue+9;
}

//设置对招操作的牌数据
void CControlWnd::SetDuiZhaoOperateCardData(BYTE cbCenterCard, BYTE cbActionMask,BYTE HuaCardIndex[])
{
	//判断用户的操作是否为规定操作,对，三招，四招共三种操作
	ASSERT((cbActionMask&WIK_DUI) != 0 || (cbActionMask&WIK_SAN_ZHAO) != 0 || (cbActionMask&WIK_SI_ZHAO) != 0);
	if ((cbActionMask&WIK_DUI) == 0 && (cbActionMask&WIK_SAN_ZHAO) == 0 && (cbActionMask&WIK_SI_ZHAO) == 0 ) return;

	//设置变量
	m_cbItemCount=0;
	m_cbCurrentItem=0xFF;
	m_cbActionMask=cbActionMask;
	m_cbCenterCard=cbCenterCard;

	//备份花牌个数的情况
	BYTE cbHuaCardIndex[5];
	CopyMemory(cbHuaCardIndex,HuaCardIndex,sizeof(cbHuaCardIndex));

	//如果传过来的引起操作的牌是花牌,将花牌保存到花牌索引数组中
	if ( (cbCenterCard != 0) && (cbCenterCard & MASK_COLOR) == 0x10)
	{
		BYTE cbCardindex = SwitchToCardIndex(cbCenterCard);
		cbHuaCardIndex[cbCardindex/2]++;
	}
	
	//分析之前，清空相关的变量值
	ZeroMemory(m_cbSiTongCard,sizeof(m_cbSiTongCard));
	ZeroMemory(m_cbsiTongHuaNm,sizeof(m_cbsiTongHuaNm));

	ZeroMemory(m_cbWuTongCard,sizeof(m_cbWuTongCard));
	ZeroMemory(m_cbWuTongHuaNm,sizeof(m_cbWuTongHuaNm));

	m_cbDuiHuaNm = 0;
	m_cbSanZhaoNm = 0;
	m_cbSiZhaoNm = 0;

	//1、进行对牌处理
	if ((m_cbActionMask&WIK_DUI)!=0)
	{
		//分析到的项数加1
		m_cbItemCount++;

		//如果是精牌的话，则将花牌都放到组合中
		BYTE cbCardindex = SwitchToCardIndex(cbCenterCard);
		if( cbCardindex == 0 || cbCardindex == 2 || cbCardindex == 4 || cbCardindex == 6 || cbCardindex == 8)
		{
			m_cbDuiHuaNm += cbHuaCardIndex[cbCardindex/2];
		}
	}
	//2、进行三招处理
	if ((m_cbActionMask&WIK_SAN_ZHAO)!=0)
	{
		//分析到的项数加1
		m_cbItemCount++;

		//如果是精牌的话，则将花牌都放到组合中
		BYTE cbCardindex = SwitchToCardIndex(cbCenterCard);
		if( cbCardindex == 0 || cbCardindex == 2 || cbCardindex == 4 || cbCardindex == 6 || cbCardindex == 8)
		{
			m_cbSanZhaoNm += cbHuaCardIndex[cbCardindex/2];
		}
	}
	//3、进行四招处理
	if ((m_cbActionMask&WIK_SI_ZHAO)!=0)
	{
		//分析到的项数加1
		m_cbItemCount++;

		//如果是精牌的话，则将花牌都放到组合中
		BYTE cbCardindex = SwitchToCardIndex(cbCenterCard);
		if( cbCardindex == 0 || cbCardindex == 2 || cbCardindex == 4 || cbCardindex == 6 || cbCardindex == 8)
		{
			m_cbSiZhaoNm += cbHuaCardIndex[cbCardindex/2];
		}
	}

	m_btChiHu.EnableWindow(false);

	//调整控件
	RectifyControl();
	//显示窗口
	ShowWindow(SW_SHOW);

	return;
}

//设置藏牌操作的牌数据
void CControlWnd::SetTongOperateCardData(BYTE cbCenterCard, BYTE cbActionMask, tagSiTongResult &SiTongCardResult,tagWuTongResult &WuTongCardResult,BYTE HuaCardIndex[])
{
	//判断用户的操作是否为规定操作,四藏，五藏共两种操作
	ASSERT((cbActionMask&WIK_SI_TONG) != 0 ||(cbActionMask&WIK_WU_TONG) != 0 );
	if ((cbActionMask&WIK_SI_TONG)== 0 &&(cbActionMask&WIK_WU_TONG) == 0 ) return;

	//设置变量
	m_cbItemCount=0;
	m_cbCurrentItem=0xFF;

	m_cbActionMask=cbActionMask;
	m_cbCenterCard=cbCenterCard;

	//备份花牌个数的情况
	BYTE cbHuaCardIndex[5];
	CopyMemory(cbHuaCardIndex,HuaCardIndex,sizeof(cbHuaCardIndex));

	//如果传过来的引起操作的牌是花牌,将花牌保存到花牌索引数组中
	if ( (cbCenterCard != 0) && (cbCenterCard & MASK_COLOR) == 0x10)
	{
		BYTE cbCardindex = SwitchToCardIndex(cbCenterCard);
		cbHuaCardIndex[cbCardindex/2]++;
	}

	//分析之前，清空相关的变量值
	ZeroMemory(m_cbSiTongCard,sizeof(m_cbSiTongCard));
	ZeroMemory(m_cbsiTongHuaNm,sizeof(m_cbsiTongHuaNm));

	ZeroMemory(m_cbWuTongCard,sizeof(m_cbWuTongCard));
	ZeroMemory(m_cbWuTongHuaNm,sizeof(m_cbWuTongHuaNm));

	m_cbDuiHuaNm = 0;
	m_cbSanZhaoNm = 0;
	m_cbSiZhaoNm = 0;

	//1、进行四藏处理
	if ((m_cbActionMask & WIK_SI_TONG)!=0)
	{
		//逐一处理四藏的组合
		for (BYTE i=0;i<SiTongCardResult.cbCardCount;i++) 
		{
			//分析到的项数加1
			m_cbItemCount++;

			//设置此四藏组合的中心牌
			m_cbSiTongCard[i]=SiTongCardResult.cbCardData[i];

			//如果是精牌的话，则将花牌都放到组合中
			BYTE cbCardindex = SwitchToCardIndex(m_cbSiTongCard[i]);
			if( cbCardindex == 0 || cbCardindex == 2 || cbCardindex == 4 || cbCardindex == 6 || cbCardindex == 8)
			{
				m_cbsiTongHuaNm[i]= cbHuaCardIndex[cbCardindex/2];
			}
			else
			{
				m_cbsiTongHuaNm[i]= 0;
			}
		}
	}
	//2、进行五藏处理
	if ((m_cbActionMask & WIK_WU_TONG)!=0)
	{
		for (BYTE i=0;i<WuTongCardResult.cbCardCount;i++) 
		{
			//分析到的项数加1
			m_cbItemCount++;

			//设置此四藏组合的中心牌
			m_cbWuTongCard[i]=WuTongCardResult.cbCardData[i];

			//如果是精牌的话，则将花牌都放到组合中
			BYTE cbCardindex = SwitchToCardIndex(m_cbWuTongCard[i]);
			if( cbCardindex == 0 || cbCardindex == 2 || cbCardindex == 4 || cbCardindex == 6 || cbCardindex == 8)
			{
				m_cbWuTongHuaNm[i]= 2;
			}
			else
			{
				m_cbWuTongHuaNm[i]= 0;
			}
		}
	}

	m_btChiHu.EnableWindow(false);

	//调整控件
	RectifyControl();
	//显示窗口
	ShowWindow(SW_SHOW);

	return;
}

//调整控件
void CControlWnd::RectifyControl()
{
	//设置位置
	CRect rcRect;
	rcRect.right=m_PointBenchmark.x;
	rcRect.bottom=m_PointBenchmark.y;
	rcRect.left=m_PointBenchmark.x-CONTROL_WIDTH;
	rcRect.top=m_PointBenchmark.y-ITEM_HEIGHT*m_cbItemCount-CONTROL_HEIGHT;
	if (m_cbItemCount>0) rcRect.top-=5;

	//移动窗口
	MoveWindow(&rcRect);

	//调整按钮
	CRect rcButton;
	m_btChiHu.GetWindowRect(&rcButton);
	int nYPos=rcRect.Height()-rcButton.Height()-9;
	m_btGiveUp.SetWindowPos(NULL,rcRect.Width()-rcButton.Width()-40,nYPos,0,0,SWP_NOZORDER|SWP_NOSIZE);
	m_btChiHu.SetWindowPos(NULL,rcRect.Width()-rcButton.Width()*2-60,nYPos,0,0,SWP_NOZORDER|SWP_NOSIZE);

	return;
}

//吃胡按钮
void CControlWnd::OnChiHu()
{
	AfxGetMainWnd()->PostMessage(IDM_CARD_OPERATE,WIK_CHI_HU,0);
	return;
}

//放弃按钮
void CControlWnd::OnGiveUp()
{
	AfxGetMainWnd()->PostMessage(IDM_CARD_OPERATE,WIK_NULL,0);
	return;
}

//重画函数
void CControlWnd::OnPaint()
{
	CPaintDC dc(this);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//创建缓冲
	CDC BufferDC;
	CBitmap BufferImage;
	BufferDC.CreateCompatibleDC(&dc);
	BufferImage.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());
	BufferDC.SelectObject(&BufferImage);

	//加载资源
	CImageHandle HandleControlTop(&m_ImageControlTop);
	CImageHandle HandleControlMid(&m_ImageControlMid);
	CImageHandle HandleControlButtom(&m_ImageControlButtom);
	CImageHandle HandleActionExplain(&m_ImageActionExplain);
	
	//绘画背景
	m_ImageControlTop.BitBlt(BufferDC,0,0);
	
	for (int nImageYPos=m_ImageControlTop.GetHeight();nImageYPos<rcClient.Height();nImageYPos+=m_ImageControlMid.GetHeight())
	{
		m_ImageControlMid.BitBlt(BufferDC,0,nImageYPos);
	}
	m_ImageControlButtom.BitBlt(BufferDC,0,rcClient.Height()-m_ImageControlButtom.GetHeight());
	
	//变量定义
	int nYPos=5;
	BYTE cbCurrentItem=0;
	BYTE cbExcursion[3]={0,1,2};
	BYTE cbItemKind[3]={WIK_DUI,WIK_SAN_ZHAO,WIK_SI_ZHAO};
	
	//绘画扑克
	for (BYTE i=0;i<CountArray(cbItemKind);i++)
	{
		if ((m_cbActionMask&cbItemKind[i])!=0)
		{
			//1、绘画对牌扑克
  			if((m_cbActionMask & cbItemKind[i])==WIK_DUI)
			{
				if(m_cbDuiHuaNm != 0)
				{
					for(BYTE k=0;k<m_cbDuiHuaNm;k++)
					{
						BYTE cbCardData=m_cbCenterCard;
						g_CardResource.m_ImageTableBottom1.DrawCardItem(&BufferDC,cbCardData,k*26+12,nYPos+5);
					}
				}
				for (BYTE j=m_cbDuiHuaNm;j<3;j++)
				{
					BYTE cbCardData=m_cbCenterCard;
					g_CardResource.m_ImageTableBottom.DrawCardItem(&BufferDC,cbCardData,j*26+12,nYPos+5);
				}
			}
			//2、绘画三招
			else if((m_cbActionMask & cbItemKind[i])==WIK_SAN_ZHAO)
			{
				if(m_cbSanZhaoNm!=0)
				{
					for(BYTE k=0;k<m_cbSanZhaoNm;k++)
					{
						BYTE cbCardData=m_cbCenterCard;
						g_CardResource.m_ImageTableBottom1.DrawCardItem(&BufferDC,cbCardData,k*26+12,nYPos+5);
					}
				}
				for (BYTE j=m_cbSanZhaoNm;j<4;j++)
				{
					BYTE cbCardData=m_cbCenterCard;
					g_CardResource.m_ImageTableBottom.DrawCardItem(&BufferDC,cbCardData,j*26+12,nYPos+5);
				}
			}
			//3、绘画四招
			else if((m_cbActionMask&cbItemKind[i])==WIK_SI_ZHAO)
			{
				if(m_cbSiZhaoNm!=0)
				{
					for(BYTE k=0;k<m_cbSiZhaoNm;k++)
					{
						BYTE cbCardData=m_cbCenterCard;
						g_CardResource.m_ImageTableBottom1.DrawCardItem(&BufferDC,cbCardData,k*26+8,nYPos+5);
					}
				}
				for (BYTE j=m_cbSiZhaoNm;j<5;j++)
				{
					BYTE cbCardData=m_cbCenterCard;
					g_CardResource.m_ImageTableBottom.DrawCardItem(&BufferDC,cbCardData,j*26+8,nYPos+5);
				}
			}

			//计算位置
			int nXImagePos=0;
			int nItemWidth=m_ImageActionExplain.GetWidth()/7;
			if ((m_cbActionMask&cbItemKind[i])==WIK_SAN_ZHAO||
				(m_cbActionMask&cbItemKind[i])==WIK_SI_ZHAO) nXImagePos=nItemWidth;
			
			//绘画标志
			int nItemHeight=m_ImageActionExplain.GetHeight();
			m_ImageActionExplain.BitBlt(BufferDC,126,nYPos+5,nItemWidth,nItemHeight,nXImagePos,0);

			//绘画边框
			if (cbCurrentItem==m_cbCurrentItem)
			{
				BufferDC.Draw3dRect(7,nYPos,rcClient.Width()-14,ITEM_HEIGHT,RGB(255,255,0),RGB(255,255,0));
			}

			//设置变量
			++cbCurrentItem;
			nYPos+=ITEM_HEIGHT;		
		}
	}

	//画四藏的牌--在同一次操作提示中，可能显示个藏(即以前能藏而没有藏的，现在都显示出来)
	for (BYTE i=0;i<CountArray(m_cbSiTongCard);i++)
	{
		//显示藏牌的情况
		if (m_cbSiTongCard[i]!=0)
		{
			if(m_cbsiTongHuaNm[i]!=0)
			{
				for(BYTE k=0;k<m_cbsiTongHuaNm[i];k++)
				{
					g_CardResource.m_ImageTableBottom1.DrawCardItem(&BufferDC,m_cbSiTongCard[i],k*26+12,nYPos+5);
				}
			}
			for (BYTE j=m_cbsiTongHuaNm[i];j<4;j++)
			{
				g_CardResource.m_ImageTableBottom.DrawCardItem(&BufferDC,m_cbSiTongCard[i],j*26+12,nYPos+5);
			}

			//绘画边框
			if (cbCurrentItem==m_cbCurrentItem)
			{
				BufferDC.Draw3dRect(7,nYPos,rcClient.Width()-14,ITEM_HEIGHT,RGB(255,255,0),RGB(255,255,0));
			}

			//绘画标志
			int nItemWidth=m_ImageActionExplain.GetWidth()/7;
			int nItemHeight=m_ImageActionExplain.GetHeight();
			m_ImageActionExplain.BitBlt(BufferDC,126,nYPos+5,nItemWidth,nItemHeight,nItemWidth*3,0);

			//设置变量
			cbCurrentItem++;
			nYPos+=ITEM_HEIGHT;
		}
	}

	//画五统的牌
	for (BYTE i=0;i<CountArray(m_cbWuTongCard);i++)
	{
		//普通杠牌
		if (m_cbWuTongCard[i]!=0)
		{

			if(m_cbWuTongHuaNm[i]!=0)
			{
				for(BYTE k=0;k<m_cbWuTongHuaNm[i];k++)
				{
					g_CardResource.m_ImageTableBottom1.DrawCardItem(&BufferDC,m_cbWuTongCard[i],k*26+8,nYPos+5);
				}
			}
		
			for (BYTE j=m_cbWuTongHuaNm[i];j<5;j++)
			{
				g_CardResource.m_ImageTableBottom.DrawCardItem(&BufferDC,m_cbWuTongCard[i],j*26+8,nYPos+5);
			}

			//绘画边框
			if (cbCurrentItem==m_cbCurrentItem)
			{
				BufferDC.Draw3dRect(7,nYPos,rcClient.Width()-14,ITEM_HEIGHT,RGB(255,255,0),RGB(255,255,0));
			}

			//绘画标志
			int nItemWidth=m_ImageActionExplain.GetWidth()/7;
			int nItemHeight=m_ImageActionExplain.GetHeight();
			m_ImageActionExplain.BitBlt(BufferDC,126,nYPos+5,nItemWidth,nItemHeight,nItemWidth*3,0);

			//设置变量
			cbCurrentItem++;
			nYPos+=ITEM_HEIGHT;
		}
	}

	//绘画界面--最后一次显示出来
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&BufferDC,0,0,SRCCOPY);

	//清理资源
	BufferDC.DeleteDC();
	BufferImage.DeleteObject();

	return;
}

//建立消息
int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//创建按钮
	CRect rcCreate(0,0,0,0);
	m_btGiveUp.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_GIVEUP);
	m_btChiHu.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_CHIHU);

	//设置位图
	m_btChiHu.SetButtonImage(IDB_BT_CHIHU,AfxGetInstanceHandle(),false);
	m_btGiveUp.SetButtonImage(IDB_BT_GIVEUP,AfxGetInstanceHandle(),false);

	return 0;
}

//鼠标消息
void CControlWnd::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);

	//索引判断
	if (m_cbCurrentItem!=0xFF)
	{
		//变量定义
		BYTE cbIndex=0;
		BYTE cbItemKind[3]={WIK_DUI,WIK_SAN_ZHAO,WIK_SI_ZHAO};

		//类型子项
		for (BYTE i=0;i<CountArray(cbItemKind);i++)
		{
			if (((m_cbActionMask&cbItemKind[i])!=0)&&(m_cbCurrentItem==cbIndex++))
			{
				AfxGetMainWnd()->PostMessage(IDM_CARD_OPERATE,cbItemKind[i],m_cbCenterCard);
				return;
			}
		}

		//四统牌子项
		for (BYTE i=0;i<CountArray(m_cbSiTongCard);i++)
		{
			if ((m_cbSiTongCard[i]!=0)&&(m_cbCurrentItem==cbIndex++))
			{
				AfxGetMainWnd()->PostMessage(IDM_CARD_OPERATE,WIK_SI_TONG,m_cbSiTongCard[i]);
				return;
			}
		}

		//五统牌子项
		for (BYTE i=0;i<CountArray(m_cbWuTongCard);i++)
		{
			if ((m_cbWuTongCard[i]!=0)&&(m_cbCurrentItem==cbIndex++))
			{
				AfxGetMainWnd()->PostMessage(IDM_CARD_OPERATE,WIK_WU_TONG,m_cbWuTongCard[i]);
				return;
			}
		}

		//错误断言
		ASSERT(FALSE);
	}

	return;
}

//光标消息
BOOL CControlWnd::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//位置测试
	if (m_cbItemCount!=0)
	{
		//获取位置
		CRect rcClient;
		CPoint MousePoint;
		GetClientRect(&rcClient);
		GetCursorPos(&MousePoint);
		ScreenToClient(&MousePoint);
		
		//计算索引
		BYTE bCurrentItem=0xFF;
		CRect rcItem(5,5,rcClient.Width()-5,ITEM_HEIGHT*m_cbItemCount+5);
		if (rcItem.PtInRect(MousePoint)) bCurrentItem=(BYTE)((MousePoint.y-7)/ITEM_HEIGHT);

		//设置索引
		if (m_cbCurrentItem!=bCurrentItem)
		{
			Invalidate(FALSE);
			m_cbCurrentItem=bCurrentItem;
		}
		
		//设置光标
		if (m_cbCurrentItem!=0xFF)
		{
			SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
			return TRUE;
		}
	}

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//////////////////////////////////////////////////////////////////////////