#include "StdAfx.h"
#include "Resource.h"
#include "RecordGameList.h"
#include "Path.h"


//////////////////////////////////////////////////////////////////////////
#define IDC_PAGE_UP					203									//翻页按钮
#define IDC_PAGE_DOWN				204									//翻页按钮


//构造函数
CGameRecord::CGameRecord()
{
	m_wPageNO = 0;
	ZeroMemory(m_wHistoryWinCount, sizeof(m_wHistoryWinCount));

    m_Cardsot[0]=9;
    m_Cardsot[1]=10;
    m_Cardsot[2]=11;
	m_Cardsot[3]=6;
	m_Cardsot[4]=7;
	m_Cardsot[5]=8;
	m_Cardsot[6]=3;
	m_Cardsot[7]=4;
	m_Cardsot[8]=5;
	m_Cardsot[9]=0;
	m_Cardsot[10]=1;
	m_Cardsot[11]=2;
}

//析构函数
CGameRecord::~CGameRecord()
{
}
 
//历史开奖结果
void CGameRecord::SetHistoryResult(tagHistory Histories[], WORD wCount)
{
	m_HistoryArray.RemoveAll();
	ZeroMemory(m_wHistoryWinCount, sizeof(m_wHistoryWinCount));

	for (WORD i=0; i<wCount; i++)
	{
		AddHistoryResult(Histories[i].cbPlayerValue);
		//printf("Histories[i].cbPlayerValue   %d \n",Histories[i].cbPlayerValue);
	}


}

//历史开奖结果
void CGameRecord::AddHistoryResult(BYTE cbPlayerValue)
{
	if (m_HistoryArray.GetCount() >= HISTORY_COUNT)
	{
		tagHistory History = m_HistoryArray[0];

		m_HistoryArray.RemoveAt(0);

	}

	m_HistoryArray.Add(tagHistory(cbPlayerValue));
	
	m_wPageNO = (WORD)m_HistoryArray.GetCount(); //总是显示到最后一条记录
	//printf("m_wPageNO    %d\n",m_wPageNO);


}

void CGameRecord::OnDraw(SURFACE* psurfTarget,SURFACE* psurfSource,int nx, int ny)
{


	//有当前页要显示的数据才处理
	if (m_HistoryArray.GetCount() > 0)
	{	
		CSize SizeUnit(40, 38);
		CPoint PointStart(90, 2); 

		//如果最后一页不够显示则补足，保证总是满屏显示
		INT_PTR wEndPos = min(m_HistoryArray.GetCount(), m_wPageNO);

		for (int i=HISTORY_RECORD_SHOW-1; i>=0 && wEndPos > 0; i--)
		{
			tagHistory History = m_HistoryArray[--wEndPos];
			psurfSource->Blit(psurfTarget,0,m_Cardsot[History.cbPlayerValue]*psurfSource->GetHeight()/12,nx-490, ny-220+SizeUnit.cy* i,psurfSource->GetWidth(),psurfSource->GetHeight()/12-2);

		}

	}   

 	return;
}


//绘画数字
void CGameRecord::DrawNumberString(SURFACE* psurfTarget, SURFACE* psurfSource1, SURFACE* psurfSource2,LONG lNumber, INT nXPos, INT nYPos,bool isleft, BYTE cbNumCount)
{

	TCHAR szBuffer[256];
	_sntprintf(szBuffer, CountArray(szBuffer), TEXT("%ld"), lNumber);
	int cu = 0;
	string s(szBuffer);
	int weigth1=psurfSource1->GetWidth()/11;
	int weigth2=psurfSource2->GetWidth()/11;
	for (int i=strlen(szBuffer)-1;i>=1;i--)
	{
		cu++;
		if (cu%4 == 0)				
			s.insert(i,",");		
	}
	if (isleft)
	{
			nXPos=nXPos-(strlen(szBuffer)-1)*(weigth2-5)-weigth1-5-(strlen(szBuffer)-1)/4*(weigth2/2-3);
	}



	int n=0;

	CString sh =s.c_str();
	CString ch=sh.Left(1);
	int b=atoi(ch.GetBuffer(1));
	psurfSource1->Blit(psurfTarget,b*weigth1,0,nXPos+n,nYPos,weigth1,psurfSource1->GetHeight());
	sh=sh.Right(sh.GetLength()-1);
	n+=(weigth1-5);


	while(sh.GetLength())
	{


		ch=sh.Left(1);
		if (strcmp(ch.GetBuffer(1),",")==0)
		{
			psurfSource2->Blit(psurfTarget,10*weigth2,0,nXPos+n,nYPos,weigth2,psurfSource2->GetHeight());
			sh=sh.Right(sh.GetLength()-1);
			n+=weigth2/2-3;
		}else
		{
			int b=atoi(ch.GetBuffer(1));
			psurfSource2->Blit(psurfTarget,b*weigth2,0,nXPos+n,nYPos,weigth2,psurfSource2->GetHeight());
			sh=sh.Right(sh.GetLength()-1);
			n+=weigth2-5;
		}



	}

	return;
}
//绘画数字 (有正负)
void CGameRecord::DrawNumberString(SURFACE* psurfTarget, SURFACE* psurfSource, SURFACE* psurfSource2,LONG lNumber, INT nXPos, INT nYPos, BYTE cbNumCount)
{

	TCHAR szBuffer[256];
	_sntprintf(szBuffer, CountArray(szBuffer), TEXT("%ld"), abs(lNumber));
	int cu = 0;
	string s(szBuffer);
	for (int i=strlen(szBuffer)-1;i>=1;i--)
	{
		cu++;
		if (cu%4 == 0)				
			s.insert(i,",");		
	}
	int n=0;
	if (lNumber>0)
	{
         psurfSource2->Blit(psurfTarget,0,0,nXPos,nYPos,psurfSource2->GetWidth()/2,psurfSource2->GetHeight());
		 nXPos=nXPos+psurfSource2->GetWidth()/2;
	}
	if (lNumber<0)
	{
		psurfSource2->Blit(psurfTarget,psurfSource2->GetWidth()/2,0,nXPos,nYPos,psurfSource2->GetWidth()/2,psurfSource2->GetHeight());
		nXPos=nXPos+psurfSource2->GetWidth()/2;
	}

	CString sh =s.c_str();

	//if (lNumber<0)
	//{
	//	sh=sh.Right(sh.GetLength()-1);
	//}

	while(sh.GetLength())
	{

		int weigth=psurfSource->GetWidth()/11;

		CString ch=sh.Left(1);

		if (strcmp(ch.GetBuffer(1),",")==0)
		{
			psurfSource->Blit(psurfTarget,10*weigth+5,0,nXPos+n,nYPos,weigth,psurfSource->GetHeight());
			sh=sh.Right(sh.GetLength()-1);
			n+=weigth/2-2;
		}else
		{
			int b=atoi(ch.GetBuffer(1));
			psurfSource->Blit(psurfTarget,b*weigth+2,0,nXPos+n,nYPos,weigth,psurfSource->GetHeight());
			sh=sh.Right(sh.GetLength()-1);
			n+=weigth-4;
		}

	}

	return;
}



//绘画数字
void CGameRecord::DrawNumberString(SURFACE* psurfTarget, SURFACE* psurfSource, LONG lNumber, INT nXPos, INT nYPos)
{

	TCHAR szBuffer[256];
	_sntprintf(szBuffer, CountArray(szBuffer), TEXT("%ld"), lNumber);
	int cu = 0;
	string s(szBuffer);
	int n=0;
	CString sh =s.c_str();

	while(sh.GetLength())
	{
		int weigth=psurfSource->GetWidth()/11;

		CString ch=sh.Left(1);
		if (strcmp(ch.GetBuffer(1),"-")==0)
		{
			psurfSource->Blit(psurfTarget,10*weigth,0,nXPos+n,nYPos,weigth,psurfSource->GetHeight());
			sh=sh.Right(sh.GetLength()-1);
			n=n+weigth-5;
		}else
		{
			int b=atoi(ch.GetBuffer(1));
			psurfSource->Blit(psurfTarget,b*weigth,0,nXPos+n,nYPos,weigth,psurfSource->GetHeight());
			sh=sh.Right(sh.GetLength()-1);
			n+=weigth-5;
		}
	}

	return;
}
