#include "StdAfx.h"
#include "WndValidateCode.h"

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CWndValidateCode, CStatic)
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CWndValidateCode::CWndValidateCode()
{
	//设置变量
	m_bValidateCode=false;
	ZeroMemory(m_szValidateCode,sizeof(m_szValidateCode));

	return;
}

//析构函数
CWndValidateCode::~CWndValidateCode()
{
}

//创建验证
VOID CWndValidateCode::RandValidateCode()
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//创建验证
	for (INT i=0;i<CountArray(m_szValidateCode);i++)
	{
		m_szValidateCode[i]=TEXT('0')+rand()%10;
	}

	//创建图片
	CreateValidateCode(m_ImageValidateCode,rcClient.Width(),rcClient.Height());

	//更新界面
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

	return;
}

//效验验证
bool CWndValidateCode::CheckValidateCode(LPCTSTR pszValidateCode)
{
	//长度验证
	if (pszValidateCode==NULL) return false;
	if (lstrlen(pszValidateCode)!=VALIDATE_COUNT) return false;

	//字符对比
	for (BYTE i=0;i<VALIDATE_COUNT;i++)
	{
		if (pszValidateCode[i]!=m_szValidateCode[i])
		{
			return false;
		}
	}

	return true;
}

//构造图片
VOID CWndValidateCode::CreateValidateCode(CImage & ImageValidateCode, INT nWidth, INT nHeight)
{
	//销毁图片
	if (ImageValidateCode.IsNull()==false)
	{
		ImageValidateCode.Destroy();
	}

	//构造图片
	ASSERT((nWidth>0)&&((nHeight>0)));
	ImageValidateCode.Create(nWidth,nHeight,32);

	//创建 DC
	CImageDC BufferDC(ImageValidateCode);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	//填充背景
	pBufferDC->FillSolidRect(1,1,nWidth-2,nHeight-2,RGB(255,255,255));
	pBufferDC->Draw3dRect(0,0,nWidth,nHeight,RGB(255,255,255),RGB(125,125,125));

	//随机背景
	for (INT nXPos=1;nXPos<nWidth-1;nXPos+=2)
	{
		for (INT nYPos=1;nYPos<nHeight-1;nYPos+=2)
		{
			pBufferDC->SetPixel(nXPos,nYPos,RGB(rand()%255,rand()%255,rand()%255));
		}
	}

	//设置 DC
	pBufferDC->SetBkMode(TRANSPARENT);
	pBufferDC->SetTextColor(RGB(0,0,0));

	//变量定义
	INT nXSpace=nWidth/CountArray(m_szValidateCode);

	//显示内容
	for (BYTE i=0;i<CountArray(m_szValidateCode);i++)
	{
		//位置定义
		CRect rcDraw;
		rcDraw.SetRect(i*nXSpace,0,(i+1)*nXSpace,nHeight);

		//创建字体
		CFont DrawFont;
		DrawFont.CreateFont(-14,0,0,0,800,0,0,0,134,3,2,ANTIALIASED_QUALITY,2,TEXT("宋体"));

		//显示内容
		pBufferDC->SelectObject(DrawFont);
		pBufferDC->DrawText(&m_szValidateCode[i],1,&rcDraw,DT_SINGLELINE|DT_VCENTER|DT_CENTER);

		//清理资源
		pBufferDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());
	}

	return;
}

//重画消息
VOID CWndValidateCode::OnPaint()
{
	//变量定义
	CPaintDC dc(this);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//绘画界面
	if (m_ImageValidateCode.IsNull()==false)
	{
		//创建 DC
		CImageDC BufferDC(m_ImageValidateCode);
		CDC * pBufferDC=CDC::FromHandle(BufferDC);

		//绘画界面
		m_ImageValidateCode.BitBlt(dc,0,0,rcClient.Width(),rcClient.Height(),0,0,SRCCOPY);
	}
	else
	{
		//默认背景
		dc.FillSolidRect(&rcClient,CSkinDialog::m_SkinAttribute.m_crBackGround);
	}

	return;
}

//位置消息
VOID CWndValidateCode::OnSize(UINT nType, INT cx, INT cy) 
{
	__super::OnSize(nType, cx, cy);

	//调整控件
	if (m_bValidateCode==true)
	{
		//获取位置
		CRect rcClient;
		GetClientRect(&rcClient);

		//创建图片
		CreateValidateCode(m_ImageValidateCode,rcClient.Width(),rcClient.Height());

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
