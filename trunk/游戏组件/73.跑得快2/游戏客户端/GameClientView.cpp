#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"
#include ".\gameclientview.h"

#define	BASEOFFESTX	30	//����x���Ե�ļ��
#define BASEOFFESTY	30	//����y���Ե�ļ��
#define OUTOFFEST	20	//�������뱾����Ƶļ��
#define BTNOFFEST	30	//���ư�ť�����ĸ߶�
#define OUTPISA		20	//׼�������Ƶĸ߶�
#define BANKSIZE	20	//��ǰ������ͼ���С
#ifdef _TWICE
	#define PISAOFFEST	15	//ÿֻ��֮��ļ��
#else
	#define PISAOFFEST	20	//ÿֻ��֮��ļ��
#endif

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_OUT_CARD, OnOutCard)
	ON_BN_CLICKED(IDC_PASS_CARD, OnPassCard)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView(void) : CGameFrameView(true,24)
{
	ResetGameView();
	//������Դ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageDisk.SetLoadInfo(IDB_VIEW_BACK,AfxGetInstanceHandle());
	m_ImageCenter.SetLoadInfo(IDB_VIEW_CENTER,AfxGetInstanceHandle());
	m_ImageFocus.SetLoadInfo(IDB_BANKER,hInstance);
	m_ImagePass.SetLoadInfo(IDB_BOMB,hInstance);
	if (GetSystemMetrics(SM_CXSCREEN)>=1024)
		m_ImagePisa.SetLoadInfo(IDB_CARD,AfxGetInstanceHandle());
	else
		m_ImagePisa.SetLoadInfo(IDB_CARD_800,AfxGetInstanceHandle());
	//��ȡ��С
	CImageHandle CardImageHandle(&m_ImagePisa);
	m_szPisa.cx=m_ImagePisa.GetWidth()/13;
	m_szPisa.cy=m_ImagePisa.GetHeight()/5;
	m_bStart=FALSE;
	return;
}

//��������
CGameClientView::~CGameClientView(void)
{
}

//������Ϣ
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	m_ScoreView.Create(IDD_GAME_SCORE,this);

	CRect CreateRect(0,0,0,0);
	m_btStart.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_START);
	m_btOutCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_OUT_CARD);
	m_btPassCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_PASS_CARD);

	//���ð�ť
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btStart.SetButtonImage(IDB_START,hInstance,false);
	m_btOutCard.SetButtonImage(IDB_OUT_CARD,hInstance,false);
	m_btPassCard.SetButtonImage(IDB_PASS,hInstance,false);

	//��ȡ����
	m_bDeasilOrder=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),TRUE)?true:false;

	m_Timer.SetBitmap(IDB_TIMERBACK,IDB_TIMERARROW);
	m_Timer.SetSinkWindow(AfxGetMainWnd());
	m_Timer.Create(NULL,NULL,WS_CHILD,CreateRect,this,21);
	m_Timer.ShowWindow(SW_SHOW);

	return 0;
}

//�û�˳��
void CGameClientView::SetUserOrder(bool bDeasilOrder)
{
	//״̬�ж�
	if (m_bDeasilOrder==bDeasilOrder) return;

	//���ñ���
	m_bDeasilOrder=bDeasilOrder;
	AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),m_bDeasilOrder?TRUE:FALSE);

	//���ý���
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyGameView(rcClient.Width(),rcClient.Height());

	//ˢ�½���
	UpdateGameView(NULL);

	return;
}

//���ý���
void CGameClientView::ResetGameView()
{
	for(int i=0;i<GAME_PLAYER;i++)
	{
		m_nPlayerLeftCount[i]=0;
		m_OutPisaArray[i].RemoveAll();
		m_bOutPass[i]=FALSE;
	}
	for(int i=0;i<PISACOUNT;i++)
		m_bOutSelect[i]=FALSE;
	m_nOwnerIndex=-1;
	m_nFocusIndex=-1;
	m_bReady=FALSE;
	m_PisaArray.RemoveAll();
	m_rtPisa.SetRectEmpty();
	//���ؿؼ�
	if (IsWindow(m_ScoreView.m_hWnd)) m_ScoreView.ShowWindow(SW_HIDE);
	return;
}

//�����ؼ�
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	//�ƶ���ť
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(3);
	m_btStart.GetWindowRect(&rcButton);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,(nHeight-rcButton.Height())/2+120,0,0,uFlags);
	int nbtHeight=nHeight-50-m_szPisa.cy-20-rcButton.Height()-20;
	DeferWindowPos(hDwp,m_btOutCard,NULL,(nWidth-rcButton.Width())/2-60,nbtHeight,0,0,uFlags);
	DeferWindowPos(hDwp,m_btPassCard,NULL,(nWidth-rcButton.Width())/2+60,nbtHeight,0,0,uFlags);
	EndDeferWindowPos(hDwp);
	//������ͼ
	CRect rcControl;
	m_ScoreView.GetWindowRect(&rcControl);
	m_ScoreView.SetWindowPos(NULL,(nWidth-rcControl.Width())/2,(nHeight-rcControl.Height()-100)/2,0,0,SWP_NOZORDER|SWP_NOSIZE);
	//m_Timer.SetBenchmarkPos(nWidth/2,nHeight/2);
	return;
}

void CGameClientView::SetTimerUp(BOOL bUp)
{
	if (bUp)
		m_Timer.SetTimerUp(m_nFocusIndex,15,m_nFocusIndex==2);
	else
		m_Timer.ShowTimer(FALSE);
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	CDC Mem;
    Mem.CreateCompatibleDC(pDC);
	CBitmap Bmp;
	Bmp.CreateCompatibleBitmap(pDC,nWidth,nHeight);
    CBitmap* pOldBmp=Mem.SelectObject(&Bmp);

	//�滭����
	DrawViewImage(&Mem,m_ImageDisk,enMode_Spread);
	DrawViewImage(&Mem,m_ImageCenter,enMode_Centent);

	//����λͼ
	CImageHandle HandleCard(&m_ImagePisa);

/*2��left:
	x=BASEOFFESTX;
	y=(nHeight-m_szPisa.cy)/2;//-getcount/2
	step+=y,20
0��right:
	x=nWidth-m_szPisa.cx-BASEOFFESTX;
	y=(nHeight-m_szPisa.cy)/2;//-getcount/2
	step+=y,20
3��top:
	x=(nWidth-m_szPisa.cx)/2;//-getcount/2
	y=BASEOFFESTY;
	step+=x,20
1��bottom:
	x=(nWidth-m_szPisa.cx)/2;//-getcount/2
	y=nWidth-m_szPisa.cy-BASEOFFESTY;
	step+=x,20

2��outleft:
	x=BASEOFFESTX+m_szPisa.cx+OUTOFFEST;
	y=(nHeight-m_szPisa.cy)/2;
	step+=x,20
0��outright:
	x=nWidth-m_szPisa.cx-BASEOFFESTX-OUTOFFEST;//-getcount
	y=(nHeight-m_szPisa.cy)/2;
	step+=x,20
3��outtop:
	x=(nWidth-m_szPisa.cx)/2;//-getcount/2
	y=BASEOFFESTY+m_szPisa.cy+OUTOFFEST;
	step+=x,20
1��outbottom:
	x=(nWidth-m_szPisa.cx)/2;//-getcount/2
	y=nWidth-m_szPisa.cy*2-BASEOFFESTY-20-OUTOFFEST-BTNOFFEST;
	step+=x,20*/
	//��ʾ����λ�õ���
	//��������ͨ�ƵĻ�׼x,��׼y,��������x����y,��׼ƫ�Ƶ���������
	int BaseStartPoint[][4]={
		{(nWidth-m_szPisa.cx)/2,BASEOFFESTY,0,2},//top
		{BASEOFFESTX+10,(nHeight-m_szPisa.cy)/2,1,2},//left
		{(nWidth-m_szPisa.cx)/2,nHeight-m_szPisa.cy-BASEOFFESTY-20,0,2},//bottom
		{nWidth-m_szPisa.cx-BASEOFFESTX+10,(nHeight-m_szPisa.cy)/2,1,2}//right
	};

	//��ʾ����λ�ó�����
	//��������ͨ�ƵĻ�׼x,��׼y,��������x����y,��׼ƫ�Ƶ���������
	int OutStartPoint[][4]={
		{(nWidth-m_szPisa.cx)/2,BASEOFFESTY+m_szPisa.cy+OUTOFFEST,0,2},//top
		{BASEOFFESTX+m_szPisa.cx+OUTOFFEST+10,(nHeight-m_szPisa.cy)/2-BANKSIZE,0,0},//left
		{(nWidth-m_szPisa.cx)/2,nWidth-m_szPisa.cy*2-BASEOFFESTY-20-OUTOFFEST-BTNOFFEST,0,2},//bottom
		{nWidth-m_szPisa.cx*2-BASEOFFESTX-OUTOFFEST+10,(nHeight-m_szPisa.cy)/2-BANKSIZE,0,1}//right
	};

	int TmpPoint[4];
	if (!m_bDeasilOrder)
	{
		//�������ʱ�뷽�����,�������е�left��right�������ת
		memcpy(&TmpPoint,&BaseStartPoint[1],sizeof(TmpPoint));
		memcpy(&BaseStartPoint[1],&BaseStartPoint[3],sizeof(TmpPoint));
		memcpy(&BaseStartPoint[3],&TmpPoint,sizeof(TmpPoint));
		memcpy(&TmpPoint,&OutStartPoint[1],sizeof(TmpPoint));
		memcpy(&OutStartPoint[1],&OutStartPoint[3],sizeof(TmpPoint));
		memcpy(&OutStartPoint[3],&TmpPoint,sizeof(TmpPoint));
	}

	CRect rtPisa;
	rtPisa.SetRectEmpty();
	int iStartPointX,iStartPointY;
	//�����Լ�������
	m_nPlayerLeftCount[2]=(int)m_PisaArray.GetCount();
	for(int i=0;i<GAME_PLAYER;i++)
	{
		//ȷ���Ƶ���ʼ����
		if (BaseStartPoint[i][2]==0)
		{
			if (BaseStartPoint[i][3]!=0)
				iStartPointX=BaseStartPoint[i][0]-PISAOFFEST*(m_nPlayerLeftCount[i]-1)/BaseStartPoint[i][3];
			else
				iStartPointX=BaseStartPoint[i][0];
			iStartPointY=BaseStartPoint[i][1];
		}
		else
		{
			iStartPointX=BaseStartPoint[i][0];
			if (BaseStartPoint[i][3]!=0)
				iStartPointY=BaseStartPoint[i][1]-PISAOFFEST*(m_nPlayerLeftCount[i]-1)/BaseStartPoint[i][3];
			else
				iStartPointY=BaseStartPoint[i][1];
		}
		//�滭��
		if (i==2)
		{
			//�Լ�����
			//��ʾ�Լ�����
			for(int j=0;j<m_PisaArray.GetCount();j++)
			{
				struct tagPisa pisa=m_PisaArray.GetAt(j);
				//������С����ʾ
				if (pisa.row==14 && pisa.line==1)
				{
					pisa.row=1;
					pisa.line=5;
				}
				if (pisa.row==14 && pisa.line==2)
				{
					pisa.row=2;
					pisa.line=5;
				}
				//�滭�˿�
				m_ImagePisa.BitBlt(Mem,iStartPointX+j*PISAOFFEST,iStartPointY-m_bOutSelect[j]*OUTPISA,m_szPisa.cx,m_szPisa.cy,(pisa.row-1)*m_szPisa.cx,(pisa.line-1)*m_szPisa.cy,SRCCOPY);
			}
			//if (m_nFocusIndex==2)
			//�����Ϊ�κ�ʱ�򶼿���������
			rtPisa.SetRect(iStartPointX,iStartPointY-OUTPISA,iStartPointX+(m_nPlayerLeftCount[i]-1)*PISAOFFEST+m_szPisa.cx,iStartPointY+m_szPisa.cy);
		}
		else
		{
			if (BaseStartPoint[i][2]==0)
			{
				for(int j=0;j<m_nPlayerLeftCount[i];j++)
					m_ImagePisa.BitBlt(Mem,iStartPointX+j*PISAOFFEST,iStartPointY,m_szPisa.cx,m_szPisa.cy,2*m_szPisa.cx,4*m_szPisa.cy,SRCCOPY);
			}
			else
			{
				for(int j=0;j<m_nPlayerLeftCount[i];j++)
					m_ImagePisa.BitBlt(Mem,iStartPointX,iStartPointY+j*PISAOFFEST,m_szPisa.cx,m_szPisa.cy,2*m_szPisa.cx,4*m_szPisa.cy,SRCCOPY);
			}
		}
		//ȷ���ѳ��Ƶ���ʼ����
		if (OutStartPoint[i][2]==0)
		{
			if (OutStartPoint[i][3]!=0)
				iStartPointX=OutStartPoint[i][0]-PISAOFFEST*((int)m_OutPisaArray[i].GetCount()-1)/OutStartPoint[i][3];
			else
				iStartPointX=OutStartPoint[i][0];
			iStartPointY=OutStartPoint[i][1];
		}
		else
		{
			iStartPointX=OutStartPoint[i][0];
			if (OutStartPoint[i][3]!=0)
				iStartPointY=OutStartPoint[i][1]-PISAOFFEST*((int)m_OutPisaArray[i].GetCount()-1)/OutStartPoint[i][3];
			else
				iStartPointY=OutStartPoint[i][1];
		}
		if (m_bOutPass[i])
		{
			//Pass
			CImageHandle ImageHandle(&m_ImagePass);
			iStartPointX-=m_ImagePass.GetWidth()/2;
			iStartPointY-=m_ImagePass.GetHeight()/2;
			m_ImagePass.AlphaDrawImage(&Mem,iStartPointX,iStartPointY,RGB(255,0,255));
		}
		else
		{
			//�滭�ѳ�����
			for(int j=0;j<m_OutPisaArray[i].GetCount();j++)
			{
				struct tagPisa pisa=m_OutPisaArray[i].GetAt(j);
				//������С����ʾ
				if (pisa.row==14 && pisa.line==1)
				{
					pisa.row=1;
					pisa.line=5;
				}
				if (pisa.row==14 && pisa.line==2)
				{
					pisa.row=2;
					pisa.line=5;
				}
				m_ImagePisa.BitBlt(Mem,iStartPointX+j*PISAOFFEST,iStartPointY,m_szPisa.cx,m_szPisa.cy,(pisa.row-1)*m_szPisa.cx,(pisa.line-1)*m_szPisa.cy,SRCCOPY);
			}
		}
	}
	m_rtPisa=rtPisa;

	//�滭���㼰�жϰ�ť
	if (m_bReady)
	{
		switch(m_nFocusIndex)
		{
			case 2://�Լ�
				if (!m_btOutCard.IsWindowVisible()) m_btOutCard.ShowWindow(SW_SHOW);
				if (!m_btPassCard.IsWindowVisible()) m_btPassCard.ShowWindow(SW_SHOW);
				break;
			case 0://�Ϸ�
			case 1://��
			case 3://�ҷ�
			{
				if (m_btOutCard.IsWindowVisible()) m_btOutCard.ShowWindow(SW_HIDE);
				if (m_btPassCard.IsWindowVisible()) m_btPassCard.ShowWindow(SW_HIDE);
				CImageHandle ImageHandle(&m_ImageFocus);
								//	top		left				bottom	right
				int nOffWidth[]=	{0,		-150,				0,		150};
				int nOffHeight[]=	{-80,	m_szPisa.cy/2+10,	0,		m_szPisa.cy/2+10};
								//	top		right				bottom	left
				int nOffWidth1[]=	{0,		150,				0,		-150};
				int nOffHeight1[]=	{-80,	m_szPisa.cy/2+10,	0,		m_szPisa.cy/2+10};
				if (m_bDeasilOrder)
					m_ImageFocus.AlphaDrawImage(&Mem,(nWidth-m_ImageFocus.GetWidth())/2+nOffWidth[m_nFocusIndex]+50,(nHeight-m_ImageFocus.GetHeight())/2+nOffHeight[m_nFocusIndex],RGB(255,0,255));
				else
					m_ImageFocus.AlphaDrawImage(&Mem,(nWidth-m_ImageFocus.GetWidth())/2+nOffWidth1[m_nFocusIndex]+50,(nHeight-m_ImageFocus.GetHeight())/2+nOffHeight1[m_nFocusIndex],RGB(255,0,255));
			}
				break;
		}
		if (m_btStart.IsWindowVisible()) m_btStart.ShowWindow(SW_HIDE);
		if (m_ScoreView.IsWindowVisible()) m_ScoreView.ShowWindow(SW_HIDE);
	}
	else
	{
		if (!m_btStart.IsWindowVisible()) m_btStart.ShowWindow(SW_SHOW);
		if (m_btOutCard.IsWindowVisible()) m_btOutCard.ShowWindow(SW_HIDE);
		if (m_btPassCard.IsWindowVisible()) m_btPassCard.ShowWindow(SW_HIDE);
	}

	//�滭�û�
	TCHAR szBuffer[32];
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		int index;
		if (m_nOwnerIndex==-1)
			//��û�п�ʼ��ʱ��ֱ�Ӱ�ԭʼ����
			index=i;
		else
			//��ʼ֮��ȡ���ط�ʽ����������
			index=(i+GAME_PLAYER-m_nOwnerIndex)%GAME_PLAYER;
		const tagUserData *pUserData=GetUserInfo(index);

						//	top					left				bottom			right
		int nOffWidth[]=	{nWidth/2-PISAOFFEST*7-PISAOFFEST/2-80,70,nWidth/2+PISAOFFEST*6+PISAOFFEST/2+m_szPisa.cx+20,nWidth-70};
		int nOffHeight[]=	{50,nHeight/2+PISAOFFEST*6+PISAOFFEST/2+m_szPisa.cy/2+10,nHeight-100,nHeight/2-PISAOFFEST*6-PISAOFFEST/2-m_szPisa.cy/2-40};
						//	top					right				bottom			left
		int nOffWidth1[]=	{nWidth/2-PISAOFFEST*7-PISAOFFEST/2-100,
												nWidth-150,			nWidth/2+PISAOFFEST*6+PISAOFFEST/2+m_szPisa.cx+20,
																					150};
		int nOffHeight1[]=	{150,				nHeight/2-PISAOFFEST*6-PISAOFFEST/2-m_szPisa.cy/2-40,
																	nHeight-160,	nHeight/2+PISAOFFEST*6+PISAOFFEST/2+m_szPisa.cy/2+20};
		//�滭�û�
		if (pUserData!=NULL)
		{
			//�û�����
			Mem.SetTextColor(RGB(220,220,220));
			Mem.SetBkMode(TRANSPARENT);
			Mem.SetTextAlign(TA_LEFT);
			if (m_bDeasilOrder)
				Mem.TextOut(nOffWidth[index],nOffHeight[index],pUserData->szName,lstrlen(pUserData->szName));
			else
				Mem.TextOut(nOffWidth1[index],nOffHeight1[index],pUserData->szName,lstrlen(pUserData->szName));

			//ʣ���˿�
			//if (pUserData->cbUserStatus>=GS_WK_PLAYING)
			if (m_nPlayerLeftCount[index]!=0)
			{
				_snprintf(szBuffer,sizeof(szBuffer),TEXT("��%ld �ţ�"),m_nPlayerLeftCount[index]);
				if (m_bDeasilOrder)
					Mem.TextOut(nOffWidth[index]-5,nOffHeight[index]+18,szBuffer,lstrlen(szBuffer));
				else
					Mem.TextOut(nOffWidth1[index]-5,nOffHeight1[index]+18,szBuffer,lstrlen(szBuffer));
			}

			//������Ϣ
			if (pUserData->cbUserStatus==US_READY)
			{
				if (m_bDeasilOrder)
					DrawUserReady(&Mem,nOffWidth[index]-50,nOffHeight[index]+5);
				else
					DrawUserReady(&Mem,nOffWidth1[index]-50,nOffHeight1[index]+5);
			}
		/*	if (m_bDeasilOrder)
				DrawUserFace(&Mem,pUserData->wFaceID,nOffWidth[index]-36,nOffHeight[index],pUserData->cbUserStatus==US_OFFLINE);
			else
				DrawUserFace(&Mem,pUserData->wFaceID,nOffWidth1[index]-36,nOffHeight1[index],pUserData->cbUserStatus==US_OFFLINE);*/

						if (pUserData->cbUserStatus==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			DrawUserFace(pDC,pUserData,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);
		}
	}
	pDC->BitBlt(0,0,nWidth,nHeight,&Mem,0,0,SRCCOPY);

	Mem.SelectObject(pOldBmp);
	Bmp.DeleteObject();
	Mem.DeleteDC();
	return;
}

void CGameClientView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_bReady)
	{
		if (!m_rtPisa.IsRectEmpty() && m_rtPisa.PtInRect(point))
		{
			//���¶�Ӧ����
			int count=(point.x-m_rtPisa.left)/PISAOFFEST;
			if (count>=m_PisaArray.GetCount()) count=(int)m_PisaArray.GetCount()-1;
			m_bOutSelect[count]=!m_bOutSelect[count];
			UpdateGameView(&m_rtPisa);
		}
/*		//FIXME
		else
		{
			m_nFocusIndex=(m_nFocusIndex+1)%GAME_PLAYER;
			UpdateGameView(NULL);
		}*/
	}

	CGameFrameView::OnLButtonDown(nFlags, point);
}

//��ʼ��ť
void CGameClientView::OnStart()
{
	m_bReady=TRUE;
	AfxGetMainWnd()->SendMessage(WM_USERREADY);
	UpdateGameView(NULL);
//FIXME,temp for use
/*	struct tagPisa Pisa;
	for(int i=0;i<PISACOUNT;i++)
	{
		Pisa.line=rand()%4+1;
		Pisa.row=rand()%PISACOUNT+1;
		m_PisaArray.Add(Pisa);
		m_OutPisaArray[0].Add(Pisa);
		m_OutPisaArray[1].Add(Pisa);
		m_OutPisaArray[2].Add(Pisa);
		m_OutPisaArray[3].Add(Pisa);
	}
	m_nPlayerLeftCount[0]=PISACOUNT;
	m_nPlayerLeftCount[1]=PISACOUNT;
	m_nPlayerLeftCount[2]=PISACOUNT;
	m_nPlayerLeftCount[3]=PISACOUNT;
	m_nFocusIndex=0;
	UpdateGameView(NULL);*/
}

//���ư�ť
void CGameClientView::OnOutCard()
{
	//���¡����ơ���ť
	struct tagSetOutPisa out;
	memset(&out,0,sizeof(tagSetOutPisa));
	//out.nChairId=(m_nOwnerIndex+GAME_PLAYER-2)%GAME_PLAYER;//�ڷ������ϵ����ƫ��
	int count=0;
	for(int i=0;i<PISACOUNT;i++)
	{
		if (m_bOutSelect[i])
			out.tOutPisa[count++]=m_PisaArray.GetAt(i);
	}
	if (count>0)
	{
		out.nCount=count;
		AfxGetMainWnd()->SendMessage(WM_OUTPISA,(WPARAM)&out);
	}
}

//������ť
void CGameClientView::OnPassCard()
{
	struct tagSetOutPisa out;
	memset(&out,0,sizeof(tagSetOutPisa));
	//out.nChairId=(m_nOwnerIndex+GAME_PLAYER-2)%GAME_PLAYER;//�ڷ������ϵ����ƫ��
	AfxGetMainWnd()->SendMessage(WM_OUTPISA,(WPARAM)&out);
}

void CGameClientView::SetTimerPause(BOOL bPause)
{
	m_Timer.SetPause(bPause);
}

int CGameClientView::SetHeiTao3Out()
{
	//�ָ������Ƶ���ʾ
	for(int i=0;i<PISACOUNT;i++)
		m_bOutSelect[i]=FALSE;
	for(int i=0;i<m_PisaArray.GetCount();i++)
	{
		const tagPisa pisa=m_PisaArray.GetAt(i);
		//����Ǻ���3
		if (pisa.line==4 && pisa.row==3)
		{
			m_bOutSelect[i]=TRUE;
			UpdateGameView(NULL);
			return i;
		}
	}
	UpdateGameView(NULL);
	return -1;
}