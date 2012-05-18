#include "StdAfx.h"
#include "Resource.h"
#include "RecordLZ.h"
#include "Path.h"
#include "..\..\..\QPLib\include\EngineInterface.h"

//////////////////////////////////////////////////////////////////////////
#define IDC_LZ_CLOSE					203									//关闭


//构造函数
CGameLZ::CGameLZ()
{
	m_wPageNO = 0;
	ZeroMemory(m_wHistoryWinCount, sizeof(m_wHistoryWinCount));

}

//析构函数
CGameLZ::~CGameLZ()
{
}
 
bool CGameLZ::OnCreate()
{
	//CVIEWMANAGE* pViewManager = AfxGetViewManager();


	CString m_strResPath = g_pPath->StripFileExtension(g_pPath->GetPathExecutable()) + "\\LZ\\";

	CString strFile = m_strResPath + "背景.psd";
	m_pImageBG = AfxGetViewManager()->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);

	strFile = m_strResPath + "红圈.psd";
	m_pImageRed = AfxGetViewManager()->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);

	strFile = m_strResPath + "蓝圈.psd";
	m_pImageBlue = AfxGetViewManager()->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);


	strFile = m_strResPath + "绿圈.psd";
	m_pImageGreen = AfxGetViewManager()->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);

	strFile = m_strResPath + "平.psd";
	m_pImageP = AfxGetViewManager()->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);

	strFile = m_strResPath + "闲.psd";
	m_pImageX = AfxGetViewManager()->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);

	strFile = m_strResPath + "庄.psd";
	m_pImageZ = AfxGetViewManager()->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);

	strFile = m_strResPath + "红蓝点.psd";
	m_pImageD = AfxGetViewManager()->CreateSurface(strFile.GetBuffer(), PSD_SURFACE);

	CRect rcCreate(0,0,0,0);
	strFile = m_strResPath + "关闭按钮.psd";
	m_pImageClose.Create(IDC_LZ_CLOSE, strFile.GetBuffer(), rcCreate, this);
	m_pImageClose.Show(true);
	m_bTopOfSibling=true;
	return true;
}


//历史开奖结果
void CGameLZ::SetHistoryResult(tagHistory Histories[], WORD wCount)
{
	m_HistoryArray.RemoveAll();
	ZeroMemory(m_wHistoryWinCount, sizeof(m_wHistoryWinCount));

	for (WORD i=0; i<wCount; i++)
	{
		AddHistoryResult(Histories[i].cbPlayerValue, Histories[i].cbBankerValue, Histories[i].chyazhu,Histories[i].ISBankerINT64,Histories[i].ISPlayerINT64,Histories[i].ISbig);
	}

	Update();
}

//历史开奖结果
void CGameLZ::AddHistoryResult(BYTE cbPlayerValue, BYTE cbBankerValue, int bRefresh,bool Banker,bool Player, bool big)
{

	//	printf("Banker%d   Player%d  \n",Banker,Player);
	m_HistoryArray.Add(tagHistory(cbPlayerValue, cbBankerValue ,bRefresh, Banker, Player, big));

 	Update();

}

//重启开奖结果
void CGameLZ::ReHistoryResult()
{

	//历史开奖结果
	tagHistory Histories[HISTORY_COUNT];
	ZeroMemory(&Histories, sizeof(Histories));
    int wEndPos = m_HistoryArray.GetCount();
	for (int i=HISTORY_COUNT-1; i>=0 && wEndPos > 0; i--)
	{
		Histories[i] = m_HistoryArray[--wEndPos];
	}
	m_HistoryArray.RemoveAll();


	for (WORD i=0; i<HISTORY_COUNT; i++)
	{
		AddHistoryResult(Histories[i].cbPlayerValue, Histories[i].cbBankerValue, Histories[i].chyazhu,Histories[i].ISBankerINT64,Histories[i].ISPlayerINT64,Histories[i].ISbig);
	}
	
	Update();

}

void CGameLZ::OnDraw(SURFACE* psurfTarget)
{

	//绘制背景
	if (m_pImageBG)
	{
		m_pImageBG->Blit(psurfTarget, 0, 0, 0, 0, m_pImageBG->GetWidth(), m_pImageBG->GetHeight());
	}
    CRect rcClient = GetWndRect();
	CPoint zxp;
	zxp.y= rcClient.Height()/2-21;
	zxp.x= rcClient.Width()/2-233;

	CPoint qq;
	qq.y= rcClient.Height()/2-124;
	qq.x= rcClient.Width()/2-232;




	//有当前页要显示的数据才处理
	if (m_HistoryArray.GetCount() > 0)
	{	
		BYTE kz=0;  //换行数字
		BYTE hhf=0;  //换行数字
		BYTE vkz=0;  //换行数字

		BYTE zd=0;  //庄对子
		BYTE xd=0;  //闲对子
		BYTE zt=0;  //庄天王
		BYTE xt=0;  //闲对子
		float zhuang=0.0;
		float xian=0.0;
		float ping=0.0;
		int m_count=m_HistoryArray.GetCount();

		if (m_HistoryArray[0].cbPlayerValue > m_HistoryArray[0].cbBankerValue)
		{
     		kz=1;
		}
		else if (m_HistoryArray[0].cbPlayerValue < m_HistoryArray[0].cbBankerValue)
		{
			kz=2;
		}
		for (int i=0; i<m_HistoryArray.GetCount(); i++)
		{

			if (m_HistoryArray[i].cbPlayerValue > m_HistoryArray[i].cbBankerValue)
			{
				m_pImageX->Blit(psurfTarget,zxp.x,zxp.y);
				if (kz!=1||hhf>5)
				{
                    qq.x=qq.x+16;
				    qq.y= rcClient.Height()/2-124;
					hhf=0;
					vkz++;
				}
				kz=1;
				m_pImageBlue->Blit(psurfTarget,qq.x,qq.y);
                xian++;

			}
			else if (m_HistoryArray[i].cbPlayerValue < m_HistoryArray[i].cbBankerValue)
			{
				if (kz!=2||hhf>5)
				{
					qq.x=qq.x+16;
					qq.y= rcClient.Height()/2-124;
					hhf=0;
					vkz++;
				}
				kz=2;
				m_pImageZ->Blit(psurfTarget,zxp.x,zxp.y);
				m_pImageRed->Blit(psurfTarget,qq.x,qq.y);
				zhuang++;
				
			}
			else
			{
				if (hhf>5)
				{
					qq.x=qq.x+16;
					qq.y= rcClient.Height()/2-124;
					hhf=0;
					vkz++;
				}
				m_pImageP->Blit(psurfTarget,zxp.x,zxp.y);
				m_pImageGreen->Blit(psurfTarget,qq.x,qq.y);
                ping++;
			}

			if (m_HistoryArray[i].ISBankerINT64)
			{
               m_pImageD->Blit(psurfTarget,m_pImageD->GetWidth()/2,0,zxp.x+25,zxp.y+25,m_pImageD->GetWidth()/2,m_pImageD->GetHeight());
			   zd++;
			}
			if (m_HistoryArray[i].ISPlayerINT64)
			{
			   m_pImageD->Blit(psurfTarget,0,0,zxp.x+25,zxp.y+25,m_pImageD->GetWidth()/2,m_pImageD->GetHeight());
			   xd++;
			}
			if (m_HistoryArray[i].cbBankerValue>7)
			{
				zt++;
			}
			if (m_HistoryArray[i].cbPlayerValue>7)
			{
				xt++;
			}

           if (vkz>29||m_HistoryArray.GetCount()>90)
           {
			   ReHistoryResult();
           }
            hhf++;
			qq.y=qq.y+16;
			if ((i+1)%6==0)
			{
                zxp.x=zxp.x+31;
				zxp.y= rcClient.Height()/2-21;
			}else
			{
                zxp.y=zxp.y+31;
			}

		}


		TCHAR szBuffer[256];

		_sntprintf(szBuffer, CountArray(szBuffer), TEXT("%0.f   %0.2f%s"),zhuang,zhuang/m_count*100,"%");

		psurfTarget->DrawText( 390 , 22 , PIXEL(255,0,0), szBuffer);

		_sntprintf(szBuffer, CountArray(szBuffer), TEXT("%0.f   %0.2f%s"),xian,xian/m_count*100,"%");

		psurfTarget->DrawText( 80 , 22 , PIXEL(13,23,160), szBuffer);

		_sntprintf(szBuffer, CountArray(szBuffer), TEXT("%0.f   %0.2f%s"),ping,ping/m_count*100,"%");

		psurfTarget->DrawText( 240 , 22 , PIXEL(5,150,35), szBuffer);

		_sntprintf(szBuffer, CountArray(szBuffer), TEXT("%d"),xd);

		psurfTarget->DrawText( 90 , 48 , PIXEL(13,23,160), szBuffer);

		_sntprintf(szBuffer, CountArray(szBuffer), TEXT("%d"),zd);

		psurfTarget->DrawText( 205 , 48 , PIXEL(255,0,0), szBuffer);

		_sntprintf(szBuffer, CountArray(szBuffer), TEXT("%d"),xt);

		psurfTarget->DrawText( 320 , 48 , PIXEL(13,23,160), szBuffer);

		_sntprintf(szBuffer, CountArray(szBuffer), TEXT("%d"),zt);

		psurfTarget->DrawText( 435 , 48 , PIXEL(255,0,0), szBuffer);

	}   

 	return;
}


//
LRESULT CGameLZ::OnWndMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (CGameWnd::OnWndMsg(hWnd, msg, wParam, lParam) == 1)
	{
		return 1;
	}

	switch(msg)
	{
	case WM_GAME_BASE_CTRL_BUTTON_CLICK_UP:
		{
			CGameWnd* pGameWnd = (CGameWnd*)wParam;
			return OnButtonClick(pGameWnd, lParam, lParam);
		}
		break;
	}

	return 0;
}
//
LRESULT CGameLZ::OnButtonClick(CGameWnd* pGameWnd, WPARAM wParam, LPARAM lParam)
{
	if (pGameWnd == &m_pImageClose)
	{
        Close();
		return true;
	}


	return false;
}

void CGameLZ::OnSize()
 {
	 CRect rcClient = GetWndRect();
	 m_pImageClose.MoveTo(CPoint(rcClient.right - 30, rcClient.top + 4));
 };
