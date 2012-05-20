#include "StdAfx.h"
#include "CardControl.h"
#include "ControlWnd.h"
#include "hucarddlg.h"

//按钮标识
#define IDC_CHIHU					102				//吃胡按钮
#define IDC_GIVEUP					103				//放弃按钮

#define TABLE_CARD_WIDTH      		15				//胡牌框宽度
#define TABLE_CARD_HEIGHT		    54				//胡牌框高度

BEGIN_MESSAGE_MAP(CHuCardDlg, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_CHIHU, OnChiHu)
	ON_BN_CLICKED(IDC_GIVEUP, OnGiveUp)
END_MESSAGE_MAP()

CHuCardDlg::CHuCardDlg(void)
{
	ZeroMemory(&m_HuCardWeave,sizeof(m_HuCardWeave));
	m_bHuScore = 0;
	m_bGoldCard = 0;
	m_PointBenchmark.SetPoint(0,0);
	//加载资源
	m_ImageBack.SetLoadInfo(IDB_VIEW_HUCARD,AfxGetInstanceHandle());

	//加载资源
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageControlLeft.SetLoadInfo(IDB_HU_L,hInstance);
	m_ImageControlMid.SetLoadInfo(IDB_HU_M,hInstance);
	m_ImageControlRight.SetLoadInfo(IDB_HU_R,hInstance);
}

CHuCardDlg::~CHuCardDlg(void)
{
}

//建立消息
int CHuCardDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	////移动窗口
	//CImageHandle BackImageHandle(&m_ImageBack);
	//SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOZORDER|SWP_NOREPOSITION);

	//创建按钮
	CRect rcCreate(0,0,0,0);
	m_btGiveUp.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_GIVEUP);
	m_btChiHu.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_CHIHU);

	//设置位图
	m_btChiHu.SetButtonImage(IDB_BT_CHIHU,AfxGetInstanceHandle(),false);
	m_btGiveUp.SetButtonImage(IDB_BT_GIVEUP,AfxGetInstanceHandle(),false);

	return TRUE;
}

//画最佳组合扑克
void CHuCardDlg::OnPaint()
{
	CPaintDC dc(this);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);
	ScreenToClient(&rcClient);

	//创建缓冲
	CDC BufferDC;
	CBitmap ImageBuffer;
	BufferDC.CreateCompatibleDC(&dc);
	ImageBuffer.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());
	BufferDC.SelectObject(&ImageBuffer);

	//加载资源
	CImageHandle HandleControlLeft(&m_ImageControlLeft);
	CImageHandle HandleControlMid(&m_ImageControlMid);
	CImageHandle HandleControlRight(&m_ImageControlRight);

	//左边的背景
	m_ImageControlLeft.BitBlt(BufferDC,0,0);

	//画中间的图片
	for (int nImageXPos=m_ImageControlLeft.GetWidth();nImageXPos<rcClient.Width();nImageXPos+=m_ImageControlMid.GetWidth())
	{
		m_ImageControlMid.BitBlt(BufferDC,nImageXPos,0);
	}

	//画右边图片
	m_ImageControlRight.BitBlt(BufferDC,rcClient.Width()-m_ImageControlRight.GetWidth(),0);

	//设置 DC
	BufferDC.SetBkMode(TRANSPARENT);
	BufferDC.SetTextColor(RGB(255,255,255));
	BufferDC.SelectObject(CSkinResourceManager::GetDefaultFont());


	//定义画最佳组合的起始位置
	WORD xPosition = 10;
	WORD yPosition = 10;

	//绘画八个组合的扑克
	for( BYTE i=0; i<8; i++)
	{
		//根据扑克牌数据，绘画每组扑克里的扑克牌
		for (BYTE j=0; j<m_HuCardWeave.cbCardCount[i]; j++)
		{
			//绘画扑克,普通牌
			if ((m_HuCardWeave.cbCardData[i][j] & 0xF0) != 0x10)
			{
				g_CardResource.m_ImageTableBottom.DrawCardItem(&BufferDC,m_HuCardWeave.cbCardData[i][j],xPosition,yPosition);
			}
			//画花牌
			else
			{
				g_CardResource.m_ImageTableBottom1.DrawCardItem(&BufferDC,m_HuCardWeave.cbCardData[i][j],xPosition,yPosition);
			}
			xPosition += 17;
		}
		xPosition += 10;

	}

	//画丫口值 
	for (BYTE i=0; i<2; i++)
	{
		//绘画扑克,普通牌
		if ((m_HuCardWeave.cbYaKou[i] & 0xF0) != 0x10)
		{
			g_CardResource.m_ImageTableBottom.DrawCardItem(&BufferDC,m_HuCardWeave.cbYaKou[i],xPosition,yPosition);
		}
		//画花牌
		else
		{
			g_CardResource.m_ImageTableBottom1.DrawCardItem(&BufferDC,m_HuCardWeave.cbYaKou[i],xPosition,yPosition);
		}
		xPosition += 17;
	}

	//画“主金”起始坐标：（50，103）
	CString str;
	str.Format("主金:");
	BufferDC.TextOut(50,103,str);

	//画金牌,坐标位置（80,85）
	if ((m_bGoldCard & 0xF0) != 0x10)
	{
		g_CardResource.m_ImageTableBottom.DrawCardItem(&BufferDC,m_bGoldCard,80,85);
	}
	//画花牌
	else
	{
		g_CardResource.m_ImageTableBottom1.DrawCardItem(&BufferDC,m_bGoldCard,80,85);
	}

	//显示“分数”,坐标位置（105，103）
	str.Format("分数:%ld",m_bHuScore);
	BufferDC.TextOut(105,103,str);

	//绘画界面
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&BufferDC,0,0,SRCCOPY);

	//清理资源
	BufferDC.DeleteDC();
}

//设置胡牌信息
void CHuCardDlg::SetHuCardWeave(tagHuCardWeave HuCardWeave,BYTE HuScore,BYTE GoldCard)
{

	m_HuCardWeave = HuCardWeave;
	m_bHuScore = HuScore;
	m_bGoldCard = GoldCard;

	//采用硬编码测试时使用的数据
	//BYTE cbTemp0[] = {0x01,0x01,0x01,0x11,0x11};
	//CopyMemory(m_HuCardWeave.cbCardData[0],cbTemp0,sizeof(cbTemp0));
	//m_HuCardWeave.cbCardCount[0] = 5;

	//BYTE cbTemp1[] = {0x02,0x03,0x04};
	//CopyMemory(m_HuCardWeave.cbCardData[1],cbTemp1,sizeof(cbTemp1));
	//m_HuCardWeave.cbCardCount[1] = 3;

	//BYTE cbTemp2[] = {0x03,0x04,0x05};
	//CopyMemory(m_HuCardWeave.cbCardData[2],cbTemp2,sizeof(cbTemp2));
	//m_HuCardWeave.cbCardCount[2] = 3;

	//BYTE cbTemp3[] = {0x02,0x02,0x02,0x11,0x11};
	//CopyMemory(m_HuCardWeave.cbCardData[3],cbTemp3,sizeof(cbTemp3));
	//m_HuCardWeave.cbCardCount[3] = 5;


	//BYTE cbTemp4[] = {0x01,0x01,0x01,0x11,0x11};
	//CopyMemory(m_HuCardWeave.cbCardData[4],cbTemp4,sizeof(cbTemp4));
	//m_HuCardWeave.cbCardCount[4] = 5;

	//BYTE cbTemp5[] = {0x01,0x01,0x01,0x11,0x11};
	//CopyMemory(m_HuCardWeave.cbCardData[5],cbTemp5,sizeof(cbTemp5));
	//m_HuCardWeave.cbCardCount[5] = 5;

	//BYTE cbTemp6[] = {0x01,0x01,0x01,0x11,0x11};
	//CopyMemory(m_HuCardWeave.cbCardData[6],cbTemp6,sizeof(cbTemp6));
	//m_HuCardWeave.cbCardCount[6] = 5;

	//BYTE cbTemp7[] = {0x23,0x23,0x23,0x23,0x23};
	//CopyMemory(m_HuCardWeave.cbCardData[7],cbTemp7,sizeof(cbTemp7));
	//m_HuCardWeave.cbCardCount[7] = 5;

	//m_bHuScore = 40;
	//m_bGoldCard = 0x01;

	//按钮控制
	m_btChiHu.EnableWindow(true);
	m_btGiveUp.EnableWindow(true);

	//调整控件
	RectifyControl();

	//显示窗口
	ShowWindow(SW_SHOW);

	return;
}

//基准位置
void CHuCardDlg::SetBenchmarkPos(int nXPos, int nYPos)
{
	//位置变量
	m_PointBenchmark.SetPoint(nXPos,nYPos);

	//调整控件
	RectifyControl();

	return;
}
//调整控件
void CHuCardDlg::RectifyControl()
{
	BYTE sum = 0;
	for (BYTE i=0; i<8; i++)  sum += m_HuCardWeave.cbCardCount[i];

	CRect rcRect;
	rcRect.left=m_PointBenchmark.x;
	rcRect.top=m_PointBenchmark.y; 
	rcRect.right=rcRect.left + TABLE_CARD_WIDTH*sum + 40 + 70 +120; //x轴+扑克数据宽度+左右边距 + 扑克组间距+扑克间距
	rcRect.bottom=rcRect.top + TABLE_CARD_HEIGHT*2 + 40 ;           //y轴+扑克高度+上下间距+中间距离

	//移动窗口
	MoveWindow(&rcRect);
	CenterWindow();

	//调整按钮
	CRect rcButton;
	m_btChiHu.GetWindowRect(&rcButton);
	int nYPos=rcRect.Height()-rcButton.Height()-20;
	m_btGiveUp.SetWindowPos(NULL,rcRect.Width()-rcButton.Width()*2-150,nYPos,0,0,SWP_NOZORDER|SWP_NOSIZE);
	m_btChiHu.SetWindowPos(NULL,rcRect.Width()-rcButton.Width()-100,nYPos,0,0,SWP_NOZORDER|SWP_NOSIZE);

	Invalidate(true);

	return;
}
//吃胡按钮
void CHuCardDlg::OnChiHu()
{
	AfxGetMainWnd()->PostMessage(IDM_CARD_OPERATE,WIK_CHI_HU,0);
	return;
}
//放弃按钮
void CHuCardDlg::OnGiveUp()
{
	AfxGetMainWnd()->PostMessage(IDM_CARD_OPERATE,WIK_NULL,0);
	return;
}
BOOL CHuCardDlg::OnEraseBkgnd(CDC* pDC)
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

void CHuCardDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonDown(nFlags,point);

	//消息模拟
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));

	return;
}
