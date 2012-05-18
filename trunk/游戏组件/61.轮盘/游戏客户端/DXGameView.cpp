#include "StdAfx.h"
#include "dxgameview.h"
#include "Resource.h"
#include <mmsystem.h>

//////////////////////////////////////////////////////////////////////////

#define IDC_CHIP_IN				100	//��ע��ť
#define IDC_CLEAR_CHIP			101	//�����ע

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDXGameView, CDxDialog)
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_CHIP_IN,OnBnClickedChipIn)
	ON_BN_CLICKED(IDC_CLEAR_CHIP,OnBnClickedClearChip)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯�� 
CDXGameView::CDXGameView(void)
{
	//���б���
	m_bShowEnd=0;
	m_wAllMoney=0;
	m_lMinBetScore=0;
	m_lMaxBetWin=0;
	m_wCurrentChip=50;
	m_bAllowBet=false;
	m_bZeroLight=false;
	m_bStatus=STATUS_FREE;
	ZeroMemory(m_wChipInfo,sizeof(m_wChipInfo));
	ZeroMemory(m_TipsMessage,sizeof(m_TipsMessage));
	ZeroMemory(m_bNumberLight,sizeof(m_bNumberLight));
	memset(m_bHistoryInfo,0xff,sizeof(m_bHistoryInfo));
	ZeroMemory(m_wHistoryChipInfo,sizeof(m_wHistoryChipInfo));
	ZeroMemory(m_wOtherUsersChips,sizeof(m_wOtherUsersChips));

	LONG lScoreIndextemp[JETTON_COUNT]={1L,5L,10L,50L,100L,500L};
	CopyMemory(lScoreIndex,lScoreIndextemp,sizeof(LONG)*6);
	//lScoreIndex[JETTON_COUNT]={1L,5L,10L,50L,100L,500L};

	m_bDrawMoveChips=false;
	m_iMoveChipsCount=0;
	m_bTimeLeave=0;

	//�����
	m_vBallMovePath.x=0;
	m_vBallMovePath.y=0;
	m_vBallMovePath.z=0;
	m_fBallMoveAngel=0;
	m_bWinNumber=255;
	m_bTargetNumber=0;
	m_Ball_StartHeight=-310;
	m_Ball_MoveRadius=WIN_RADIUS;

	//��ͼ����
	m_pReflect=NULL;
	m_pBump=NULL;
}

//��������
CDXGameView::~CDXGameView(void)
{

}

//���ÿ�ѡ����
void CDXGameView::OnSetCellScore(long lCellScore)
{
	if(lCellScore<5)
	{
		LONG lScoreIndextemp[JETTON_COUNT]={1L,5L,10L,50L,100L,500L};
		CopyMemory(lScoreIndex,lScoreIndextemp,sizeof(LONG)*6);
	}
	if(lCellScore>=5&&lCellScore<10)
	{
		LONG lScoreIndextemp[JETTON_COUNT]={5L,10L,50L,100L,500L,1000L};
		CopyMemory(lScoreIndex,lScoreIndextemp,sizeof(LONG)*6);
	}
	if(lCellScore>10)
	{
		LONG lScoreIndextemp[JETTON_COUNT]={10L,50L,100L,500L,1000L,5000L};
		CopyMemory(lScoreIndex,lScoreIndextemp,sizeof(LONG)*6);
	}
	return;
}

//������Դ
void CDXGameView::InitScene(int nWidth,int nHeight)
{
	//������ť
	CRect CreateRect(0,0,0,0);
	if(!m_btChipIn.GetSafeHwnd()) m_btChipIn.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_CHIP_IN);
	if(!m_btClearChip.GetSafeHwnd()) m_btClearChip.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_CLEAR_CHIP);

	//���ð�ť
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btChipIn.SetButtonImage(IDB_CHIP_IN,hInstance,false);
	m_btClearChip.SetButtonImage(IDB_CLEAR_CHIP,hInstance,false);
	m_btChipIn.ShowWindow(SW_HIDE);
	m_btClearChip.ShowWindow(SW_HIDE);

	//�����
	CRect rcCreate(0,0,0,0);
	if(!m_ScoreView.GetSafeHwnd()) m_ScoreView.Create(IDD_GAME_SCORE,this);

	//����λ��
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	m_btChipIn.GetWindowRect(&rcButton);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	DeferWindowPos(hDwp,m_btChipIn,NULL,nWidth/2-275-rcButton.Width()/2,nHeight/2+95-rcButton.Height()/2,0,0,uFlags);
	DeferWindowPos(hDwp,m_btClearChip,NULL,nWidth/2-196-rcButton.Width()/2,nHeight/2+157-rcButton.Height()/2,0,0,uFlags);
	EndDeferWindowPos(hDwp);

	//������Ļ���
	m_nScreenWidth=nWidth;
	m_nScreenHeight=nHeight;
	m_GameLogic.SetDxWindowSize(m_nScreenWidth,m_nScreenHeight);

	try{
		//����ƽ��ͼƬ
		m_SurfaceTable.SetLoadInfo(g_pD3DDevice,"Round\\background.bmp");
		m_SurfaceBack.SetLoadInfo(g_pD3DDevice,"Round\\VIEW_BACK.bmp");
		m_NumberMask.SetLoadInfo(g_pD3DDevice,"Round\\light.png");

		m_Chip1.SetLoadInfo(g_pD3DDevice,"Round\\1.png");
		m_Chip5.SetLoadInfo(g_pD3DDevice,"Round\\5.png");
		m_Chip10.SetLoadInfo(g_pD3DDevice,"Round\\10.png");
		m_Chip50.SetLoadInfo(g_pD3DDevice,"Round\\50.png");
		m_Chip100.SetLoadInfo(g_pD3DDevice,"Round\\100.png");
		m_Chip500.SetLoadInfo(g_pD3DDevice,"Round\\500.png");
		m_Chip1000.SetLoadInfo(g_pD3DDevice,"Round\\1k.png");
		m_Chip5000.SetLoadInfo(g_pD3DDevice,"Round\\5k.png");
		m_Chip10000.SetLoadInfo(g_pD3DDevice,"Round\\10k.png");
		m_SprCurrentChip.SetLoadInfo(g_pD3DDevice,"Round\\hongquan.png");

		m_SprTimer[0].SetLoadInfo(g_pD3DDevice,"Round\\n0.png");
		m_SprTimer[1].SetLoadInfo(g_pD3DDevice,"Round\\n1.png");
		m_SprTimer[2].SetLoadInfo(g_pD3DDevice,"Round\\n2.png");
		m_SprTimer[3].SetLoadInfo(g_pD3DDevice,"Round\\n3.png");
		m_SprTimer[4].SetLoadInfo(g_pD3DDevice,"Round\\n4.png");
		m_SprTimer[5].SetLoadInfo(g_pD3DDevice,"Round\\n5.png");
		m_SprTimer[6].SetLoadInfo(g_pD3DDevice,"Round\\n6.png");
		m_SprTimer[7].SetLoadInfo(g_pD3DDevice,"Round\\n7.png");
		m_SprTimer[8].SetLoadInfo(g_pD3DDevice,"Round\\n8.png");
		m_SprTimer[9].SetLoadInfo(g_pD3DDevice,"Round\\n9.png");

		m_PlaneBall.SetLoadInfo(g_pD3DDevice,"Round\\xiaoqiu.png");
		m_PlatePlane.SetLoadInfo(g_pD3DDevice,"Round\\shuzi.png");
		m_PlatePlaneMask.SetLoadInfo(g_pD3DDevice,"Round\\PlaneMask.png");

		//����ģ��
		m_vCenterPos.x=455;
		m_vCenterPos.y=-330;
		m_vCenterPos.z=456;
		m_MeshPlate.SetLoadInfo(g_pD3DDevice,"Round\\pan.x",true);
		m_MeshPlate.SetPosition(m_vCenterPos);
		m_MeshStock.SetLoadInfo(g_pD3DDevice,"Round\\waipan.x",true);
		m_MeshStock.SetPosition(m_vCenterPos);
		m_MeshShelfHigh.SetLoadInfo(g_pD3DDevice,"Round\\jiazi.x",true);
		m_MeshShelfHigh.SetPosition(m_vCenterPos);
		m_MeshShelfLow.SetLoadInfo(g_pD3DDevice,"Round\\jiazidi.x",true);
		m_MeshShelfLow.SetPosition(m_vCenterPos);
		m_MeshRing.SetLoadInfo(g_pD3DDevice,"Round\\huan.x",true);
		m_MeshRing.SetPosition(m_vCenterPos);
		m_MeshBall.SetLoadInfo(g_pD3DDevice,"Round\\ball.x",true);
		m_MeshBall.SetPosition(360,-310,515);

		//Ч������
		//D3DXCreateTextureFromFile(g_pD3DDevice,TEXT("Round\\pan.bmp"),&m_pReflect);
		//D3DXCreateTextureFromFile(g_pD3DDevice,TEXT("Round\\jiazi.bmp"),&m_pBump);
		//m_MeshShelfHigh.SetBumpTexture(m_pBump);
		//m_MeshShelfHigh.SetReflectTexture(m_pReflect);
		//m_MeshShelfLow.SetReflectTexture(m_pReflect);
		
		//ID3DXEffect*      s_pEffect = NULL;   
		//D3DXCreateEffectFromFile( g_pD3DDevice, "Round\\test.fx", NULL, NULL, D3DXFX_NOT_CLONEABLE, NULL, &s_pEffect, NULL );
		//s_pEffect->SetTexture(s_pEffect->GetParameterByName(NULL,"g_txCubeMap2"),m_pReflect);
	}catch(...)
	{
		StopRender();
		AfxMessageBox("��Դ����ʧ�ܣ��볢�����°�װ��");	
		AfxGetMainWnd()->SendMessage(WM_CLOSE);

		return;
	}

	//����� 
	g_pD3DDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS,TRUE);

	//�����ƹ�
	SetLight();	
}

//�������
void CDXGameView::UpDateControl()
{
	//���ý���λ��
	g_Camera.SetFocus(&D3DXVECTOR3(0,0,0));
	return;

	g_UserControl.UpdateInput();
	
	float mouseX,mouseY,mouseZ;	//���״̬
	float dSide=0,dForward=0;	//ƫ�Ʊ���

	//��ס����Ҽ���ת��ͷ
	if(g_UserControl.m_MouseState.rgbButtons[1]&0x80)
	{
		mouseX=g_UserControl.GetLX()*D3DX_PI/720.0f;	//ÿ�ƶ�һ���㣬����ı�1/4��
		mouseY=g_UserControl.GetLY()*D3DX_PI/720.0f;
		g_Camera.YawPitchAt(-mouseX,mouseY);
		dSide+=mouseX;
	}

	//�����ֿ������������
	mouseZ=g_UserControl.GetLZ()*D3DX_PI/10;
	g_Camera.Move(0,mouseZ);

	return;
}

//��Ⱦ����
void CDXGameView::DrawGameView2D()
{
	//���������뱳��
	m_SurfaceBack.DrawSurface(0,0,1);
	m_SurfaceTable.DrawSurface(m_nScreenWidth/2-m_SurfaceTable.GetWidth()/2,m_nScreenHeight/2-m_SurfaceTable.GetHeight()/2); 

	//ƽ������
	D3DXMATRIX matRot;
	D3DXMatrixIdentity(&matRot);
	float   iAngle=0;
	if((int)m_Ball_MoveRadius!=WIN_RADIUS&&m_bWinNumber==255) //��ת����
	{
		iAngle=1.5f*timeGetTime()/500.0f;   
	}
	else if(m_bWinNumber!=255)	//ת��ʤ������
	{
		for(int i=0;i<37;i++) if(m_bWinNumber==numberArrayIndex[i]) iAngle=i*(2*D3DX_PI/37);;
	}
	D3DXMatrixTransformation2D(	&matRot,0,0,0,&D3DXVECTOR2((float)m_PlatePlane.GetWidth()/2,(float)m_PlatePlane.GetHeight()/2),iAngle,&D3DXVECTOR2((float)m_nScreenWidth/2+309-m_PlatePlane.GetWidth()/2,(float)m_nScreenHeight/2-68-m_PlatePlane.GetHeight()/2));
	m_PlatePlane.DrawSprite(0,0,D3DXVECTOR3(0,0,0), CRect(0,0,584,584),&matRot);
	m_PlatePlaneMask.DrawSprite(m_nScreenWidth/2,m_nScreenHeight/2-m_PlatePlaneMask.GetHeight()/2,0xffffffff);
	if(m_bWinNumber!=255) m_PlaneBall.DrawSprite(m_nScreenWidth/2+206,m_nScreenHeight/2-226,0xffffffff);

	//�Ƿ���ʾ�����ע��Ŧ
	if(GetAllMoney()!=0&&m_bStatus==STATUS_FREE) m_btClearChip.ShowWindow(SW_SHOW);
	else	m_btClearChip.ShowWindow(SW_HIDE);
	if(m_bStatus==STATUS_FREE) m_btChipIn.ShowWindow(SW_SHOW);
	else	m_btChipIn.ShowWindow(SW_HIDE);

	//�����ʾ��Ϣ
	DxDrawText(m_TipsMessage,m_nScreenWidth/2+170,m_nScreenHeight/2-330,RGB(100,255,100));
	if(m_bStatus==STATUS_READY) DxDrawText(TEXT("�ȴ���ʼ..."),m_nScreenWidth/2+170,m_nScreenHeight/2-330,RGB(100,255,100));
	if(m_bStatus==STATUS_PLAY) DxDrawText(TEXT("������ת..."),m_nScreenWidth/2+170,m_nScreenHeight/2-330,RGB(100,255,100));

	//��ע��Ϣ
	TCHAR szBuffer[256];
	ZeroMemory(szBuffer,sizeof(szBuffer));
	_snprintf(szBuffer,sizeof(szBuffer),TEXT("���עӮ��\n %d"),m_lMaxBetWin);
	DxDrawText(szBuffer,m_nScreenWidth/2+26,m_nScreenHeight/2-282,RGB(0,0,0));
	_snprintf(szBuffer,sizeof(szBuffer),TEXT("����ע�� %d"),GetAllMoney());
	DxDrawText(szBuffer,m_nScreenWidth/2+26,m_nScreenHeight/2-249,RGB(0,0,0));

	//ʣ��ʱ����Ϣ
	if(m_bStatus!=STATUS_PLAY)
	{
		m_SprTimer[m_bTimeLeave/10].DrawSprite(m_nScreenWidth/2+45,m_nScreenHeight/2-200);
		m_SprTimer[m_bTimeLeave%10].DrawSprite(m_nScreenWidth/2+65,m_nScreenHeight/2-200);
	}

	//��ʾ��ǰ��ѡ��ĳ���
	for(BYTE i=0;i<CountArray(lScoreIndex);i++)
	{
		if(m_wCurrentChip==lScoreIndex[i])
		{
			int xPos=m_nScreenWidth/2 + (ChooseChipArea[i].BasePoint.x+ChooseChipArea[i].VertexButtom.x/2) -m_SprCurrentChip.GetWidth()/2;
			int yPos=m_nScreenHeight/2+ (ChooseChipArea[i].BasePoint.y+(ChooseChipArea[i].VertexButtom.y)/2) -m_SprCurrentChip.GetHeight()/2;
			m_SprCurrentChip.DrawSprite(xPos,yPos,0xffffffff);	
		}
	}
	//�滭ѡ�����
	if(lScoreIndex[0]==1)
	{
		int xPos=m_nScreenWidth/2 + (ChooseChipArea[0].BasePoint.x+ChooseChipArea[0].VertexButtom.x/2) -m_Chip1.GetWidth()/2;
		int yPos=m_nScreenHeight/2+ (ChooseChipArea[0].BasePoint.y+(ChooseChipArea[0].VertexButtom.y)/2) -m_Chip1.GetHeight()/2;
		m_Chip1.DrawSprite(xPos,yPos,0xffffffff);

		xPos=m_nScreenWidth/2 + (ChooseChipArea[1].BasePoint.x+ChooseChipArea[1].VertexButtom.x/2) -m_Chip1.GetWidth()/2;
		yPos=m_nScreenHeight/2+ (ChooseChipArea[1].BasePoint.y+(ChooseChipArea[1].VertexButtom.y)/2) -m_Chip1.GetHeight()/2;
		m_Chip5.DrawSprite(xPos,yPos,0xffffffff);

		xPos=m_nScreenWidth/2 + (ChooseChipArea[2].BasePoint.x+ChooseChipArea[2].VertexButtom.x/2) -m_Chip1.GetWidth()/2;
		yPos=m_nScreenHeight/2+ (ChooseChipArea[2].BasePoint.y+(ChooseChipArea[2].VertexButtom.y)/2) -m_Chip1.GetHeight()/2;
		m_Chip10.DrawSprite(xPos,yPos,0xffffffff);

		xPos=m_nScreenWidth/2 + (ChooseChipArea[3].BasePoint.x+ChooseChipArea[3].VertexButtom.x/2) -m_Chip1.GetWidth()/2;
		yPos=m_nScreenHeight/2+ (ChooseChipArea[3].BasePoint.y+(ChooseChipArea[3].VertexButtom.y)/2) -m_Chip1.GetHeight()/2;
		m_Chip50.DrawSprite(xPos,yPos,0xffffffff);

		xPos=m_nScreenWidth/2 + (ChooseChipArea[4].BasePoint.x+ChooseChipArea[4].VertexButtom.x/2) -m_Chip1.GetWidth()/2;
		yPos=m_nScreenHeight/2+ (ChooseChipArea[4].BasePoint.y+(ChooseChipArea[4].VertexButtom.y)/2) -m_Chip1.GetHeight()/2;
		m_Chip100.DrawSprite(xPos,yPos,0xffffffff);

		xPos=m_nScreenWidth/2 + (ChooseChipArea[5].BasePoint.x+ChooseChipArea[5].VertexButtom.x/2) -m_Chip1.GetWidth()/2;
		yPos=m_nScreenHeight/2+ (ChooseChipArea[5].BasePoint.y+(ChooseChipArea[5].VertexButtom.y)/2) -m_Chip1.GetHeight()/2;
		m_Chip500.DrawSprite(xPos,yPos,0xffffffff);
	}
	else if(lScoreIndex[0]==5)
	{
		int xPos=m_nScreenWidth/2 + (ChooseChipArea[0].BasePoint.x+ChooseChipArea[0].VertexButtom.x/2) -m_Chip1.GetWidth()/2;
		int yPos=m_nScreenHeight/2+ (ChooseChipArea[0].BasePoint.y+(ChooseChipArea[0].VertexButtom.y)/2) -m_Chip1.GetHeight()/2;
		m_Chip5.DrawSprite(xPos,yPos,0xffffffff);

		xPos=m_nScreenWidth/2 + (ChooseChipArea[1].BasePoint.x+ChooseChipArea[1].VertexButtom.x/2) -m_Chip1.GetWidth()/2;
		yPos=m_nScreenHeight/2+ (ChooseChipArea[1].BasePoint.y+(ChooseChipArea[1].VertexButtom.y)/2) -m_Chip1.GetHeight()/2;
		m_Chip10.DrawSprite(xPos,yPos,0xffffffff);

		xPos=m_nScreenWidth/2 + (ChooseChipArea[2].BasePoint.x+ChooseChipArea[2].VertexButtom.x/2) -m_Chip1.GetWidth()/2;
		yPos=m_nScreenHeight/2+ (ChooseChipArea[2].BasePoint.y+(ChooseChipArea[2].VertexButtom.y)/2) -m_Chip1.GetHeight()/2;
		m_Chip50.DrawSprite(xPos,yPos,0xffffffff);

		xPos=m_nScreenWidth/2 + (ChooseChipArea[3].BasePoint.x+ChooseChipArea[3].VertexButtom.x/2) -m_Chip1.GetWidth()/2;
		yPos=m_nScreenHeight/2+ (ChooseChipArea[3].BasePoint.y+(ChooseChipArea[3].VertexButtom.y)/2) -m_Chip1.GetHeight()/2;
		m_Chip100.DrawSprite(xPos,yPos,0xffffffff);

		xPos=m_nScreenWidth/2 + (ChooseChipArea[4].BasePoint.x+ChooseChipArea[4].VertexButtom.x/2) -m_Chip1.GetWidth()/2;
		yPos=m_nScreenHeight/2+ (ChooseChipArea[4].BasePoint.y+(ChooseChipArea[4].VertexButtom.y)/2) -m_Chip1.GetHeight()/2;
		m_Chip500.DrawSprite(xPos,yPos,0xffffffff);

		xPos=m_nScreenWidth/2 + (ChooseChipArea[5].BasePoint.x+ChooseChipArea[5].VertexButtom.x/2) -m_Chip1.GetWidth()/2;
		yPos=m_nScreenHeight/2+ (ChooseChipArea[5].BasePoint.y+(ChooseChipArea[5].VertexButtom.y)/2) -m_Chip1.GetHeight()/2;
		m_Chip1000.DrawSprite(xPos,yPos,0xffffffff);
	}
	else if(lScoreIndex[0]==10)
	{
		int xPos=m_nScreenWidth/2 + (ChooseChipArea[0].BasePoint.x+ChooseChipArea[0].VertexButtom.x/2) -m_Chip1.GetWidth()/2;
		int yPos=m_nScreenHeight/2+ (ChooseChipArea[0].BasePoint.y+(ChooseChipArea[0].VertexButtom.y)/2) -m_Chip1.GetHeight()/2;
		m_Chip10.DrawSprite(xPos,yPos,0xffffffff);

		xPos=m_nScreenWidth/2 + (ChooseChipArea[1].BasePoint.x+ChooseChipArea[1].VertexButtom.x/2) -m_Chip1.GetWidth()/2;
		yPos=m_nScreenHeight/2+ (ChooseChipArea[1].BasePoint.y+(ChooseChipArea[1].VertexButtom.y)/2) -m_Chip1.GetHeight()/2;
		m_Chip50.DrawSprite(xPos,yPos,0xffffffff);

		xPos=m_nScreenWidth/2 + (ChooseChipArea[2].BasePoint.x+ChooseChipArea[2].VertexButtom.x/2) -m_Chip1.GetWidth()/2;
		yPos=m_nScreenHeight/2+ (ChooseChipArea[2].BasePoint.y+(ChooseChipArea[2].VertexButtom.y)/2) -m_Chip1.GetHeight()/2;
		m_Chip100.DrawSprite(xPos,yPos,0xffffffff);

		xPos=m_nScreenWidth/2 + (ChooseChipArea[3].BasePoint.x+ChooseChipArea[3].VertexButtom.x/2) -m_Chip1.GetWidth()/2;
		yPos=m_nScreenHeight/2+ (ChooseChipArea[3].BasePoint.y+(ChooseChipArea[3].VertexButtom.y)/2) -m_Chip1.GetHeight()/2;
		m_Chip500.DrawSprite(xPos,yPos,0xffffffff);

		xPos=m_nScreenWidth/2 + (ChooseChipArea[4].BasePoint.x+ChooseChipArea[4].VertexButtom.x/2) -m_Chip1.GetWidth()/2;
		yPos=m_nScreenHeight/2+ (ChooseChipArea[4].BasePoint.y+(ChooseChipArea[4].VertexButtom.y)/2) -m_Chip1.GetHeight()/2;
		m_Chip1000.DrawSprite(xPos,yPos,0xffffffff);

		xPos=m_nScreenWidth/2 + (ChooseChipArea[5].BasePoint.x+ChooseChipArea[5].VertexButtom.x/2) -m_Chip1.GetWidth()/2;
		yPos=m_nScreenHeight/2+ (ChooseChipArea[5].BasePoint.y+(ChooseChipArea[5].VertexButtom.y)/2) -m_Chip1.GetHeight()/2;
		m_Chip5000.DrawSprite(xPos,yPos,0xffffffff);	
	}

	//�滭�����������Ӧ
	if(m_bZeroLight)
	{
		int xPos=m_nScreenWidth/2 + (Bet0Area.BasePoint.x+Bet0Area.VertexButtom.x/2) -m_NumberMask.GetWidth()/2-12;
		int yPos=m_nScreenHeight/2+ (Bet0Area.BasePoint.y+(Bet0Area.VertexButtom.y)/2) -m_NumberMask.GetHeight()/2-8;
		m_NumberMask.DrawSprite(xPos,yPos,0xffffffff);	
	}
	for(BYTE i=0;i<3;i++)for(BYTE j=0;j<12;j++)
	{
		//д��ô�ֻ࣬��Ϊ����ͼ�ξ���
		if(m_bNumberLight[i][j]) 
		{
			int xPos=m_nScreenWidth/2 + (BetNumberArea[i][j].BasePoint.x+BetNumberArea[i][j].VertexRight.x/2) -m_NumberMask.GetWidth()/2;
			int yPos=m_nScreenHeight/2+ (BetNumberArea[i][j].BasePoint.y+BetNumberArea[i][j].VertexTop.y+(BetNumberArea[i][j].VertexButtom.y-BetNumberArea[i][j].VertexTop.y)/2 ) -m_NumberMask.GetHeight()/2;
			m_NumberMask.DrawSprite(xPos,yPos,0xffffffff);	
		}
	}

	//�滭������Ϣ
	GetChipsInfo();

	//�滭��ʷ��¼
	DrawHistory();

	//�滭������Ϣ
	if(m_bShowEnd==0)
	{
		if(m_ScoreView.GetSafeHwnd()) 
		{
			m_ScoreView.SetGameScore(0);
			m_ScoreView.ShowWindow(SW_HIDE);
		}
	}
	if(m_bShowEnd==1)
	{
		if(m_ScoreView.GetSafeHwnd()&&m_ScoreView.IsWindowVisible()==false) 
		{
			m_ScoreView.SetGameScore(m_lGameEndScore);
			m_ScoreView.ShowWindow(SW_SHOW);
		}
	}
	if(m_bShowEnd==2)
	{
		if(m_ScoreView.GetSafeHwnd()&&m_ScoreView.IsWindowVisible()==false)
		{
			m_ScoreView.SetGameScore(m_lGameEndScore);
			m_ScoreView.ShowWindow(SW_SHOW);
		}
	}

	if(m_bDrawMoveChips)
	{
		for(int i=0;i<MAX_CHIPS;i++)
		{
			if(m_SprMoveChips[i].IsLoaded())
				m_SprMoveChips[i].DrawMoveSprite();
		}
	}

	return;
}

//��Ⱦ����
void CDXGameView::DrawGameView3D()
{	
	//��Ⱦģ��
	float scale=0.65f;
	float rotate=0.5f;

	//�������
	m_MeshStock.SetRenderState((float)scale);
	m_MeshStock.Render();
	//������
	m_MeshPlate.SetRenderState((float)scale,rotate);
	m_MeshPlate.Render();
	//�м�ļ��ӵ���
	m_MeshShelfLow.SetRenderState((float)scale,rotate);
	m_MeshShelfLow.Render();
	//�������ֵĻ�
	m_MeshRing.SetRenderState((float)scale,rotate);
	m_MeshRing.Render();
	//�м�ļ����ϲ�
	m_MeshShelfHigh.SetRenderState((float)scale,rotate);
	m_MeshShelfHigh.Render();	
	//��
	m_MeshBall.SetRenderState((float)scale);
	calcPath(&m_vBallMovePath);
	m_MeshBall.SetPosition(m_vBallMovePath);
	m_MeshBall.Render();

	return;
}

//������趨���·��
void CDXGameView::calcPath(D3DXVECTOR3 *movePath)
{
	//��ʤ������ ����Ϊ��ֹ�豸��ʧ����λ�ô��ң�
	if(m_bWinNumber!=255)
	{
		m_Ball_MoveRadius=WIN_RADIUS;
		//ȡ�õ�ǰ���̽Ƕ�
		D3DXMATRIX plateRot= m_MeshPlate.GetRotate();
		float x=plateRot._11;
		float z=plateRot._13;
		//��ǰ���������ĵ�н�(�Ѿ���ת�Ķ���)
		float currentPlatAngel=0;
		float PlatAngel=asin(z)/D3DX_PI*180;
		if(x>0&&z>0) currentPlatAngel=PlatAngel;	//��һ����
		if(x<0&&z>0) currentPlatAngel=180-PlatAngel;//�ڶ�����
		if(x<0&&z<0) currentPlatAngel=180-PlatAngel;//��������
		if(x>0&&z<0) currentPlatAngel=360+PlatAngel;//��������

		//ʤ�����ֻ���
		float iAngle=0;
		for(int i=0;i<37;i++) if(m_bWinNumber==numberArrayIndex[i]) iAngle=i*(2*D3DX_PI/37);
		//ת��Ϊ�Ƕ�
		iAngle=iAngle/D3DX_PI*180;

		float BallAngel=(float)(((int)(iAngle+currentPlatAngel+5))%360);
		BallAngel=BallAngel/180*D3DX_PI;

		x=m_Ball_MoveRadius*cosf(BallAngel)+455;
		z=m_Ball_MoveRadius*sinf(BallAngel)+456;

		movePath->x=x;
		movePath->y=-325;
		movePath->z=z;
		return;
	}

	//�Ѿ�ת������С�뾶λ��
	if((int)m_Ball_MoveRadius==WIN_RADIUS) 
	{
		//��ǰ��������������ĵ�����
		float	x=movePath->x-m_vCenterPos.x;
		float	z=movePath->z-m_vCenterPos.z;

		//��ǰ�����������ĵ�н�(δ��תʱ����0λ����0��)
		float currentAngel=0;
		float Angel=asin(z/m_Ball_MoveRadius)/D3DX_PI*180;
		if(x>0&&z>0) currentAngel=Angel;	//��һ����
		if(x<0&&z>0) currentAngel=180-Angel;//�ڶ�����
		if(x<0&&z<0) currentAngel=180-Angel;//��������
		if(x>0&&z<0) currentAngel=360+Angel;//��������	

		//ȡ�õ�ǰ���̽Ƕ�
		D3DXMATRIX plateRot= m_MeshPlate.GetRotate();
		x=plateRot._11;
		z=plateRot._13;
		//��ǰ���������ĵ�н�(�Ѿ���ת�Ķ���)
		float currentPlatAngel=0;
		float PlatAngel=asin(z)/D3DX_PI*180;
		if(x>0&&z>0) currentPlatAngel=PlatAngel;	//��һ����
		if(x<0&&z>0) currentPlatAngel=180-PlatAngel;//�ڶ�����
		if(x<0&&z<0) currentPlatAngel=180-PlatAngel;//��������
		if(x>0&&z<0) currentPlatAngel=360+PlatAngel;//��������

		//�õ��������̼нǡ������ֵ����֪����ͣ�����̵��ĸ�������
		float thisAngel=currentAngel-currentPlatAngel;
		if(thisAngel<0) thisAngel=thisAngel+360;

		//ʤ������
		if(m_bWinNumber==255)
		{	
			m_bWinNumber=GetBallStopNumber(thisAngel);

			//������Ϣ
			if(m_bStatus==STATUS_PLAY)
			{	
				AfxGetMainWnd()->SendMessage(IDM_WIN_NUMBER,(WPARAM)m_bWinNumber);
				m_bStatus=STATUS_FREE;
			}
		}
		return;
	}

	//�ֶ�·��
	//תȦ
	if((int)m_Ball_MoveRadius>=FALL_RADIUS)
	{
		//ת���뾶
		m_Ball_MoveRadius-=0.01f;

		//ת���Ƕ���߶�
		m_fBallMoveAngel+=0.05f;
		//if(m_fBallMoveAngel>2*D3DX_PI) m_fBallMoveAngel=0;
		//m_Ball_StartHeight-=0.02f;

		//��������
		float x=m_Ball_MoveRadius*cosf(m_fBallMoveAngel)+455;
		float z=m_Ball_MoveRadius*sinf(m_fBallMoveAngel)+456;

		//�趨����
		movePath->x=x;
		movePath->y=m_Ball_StartHeight;
		movePath->z=z;
		return;
	}
	//����
	else if((int)m_Ball_MoveRadius<FALL_RADIUS&&(int)m_Ball_MoveRadius>=JUMP_RADIUS)
	{
		//ת���뾶
		m_Ball_MoveRadius-=0.5f;

		//ת���Ƕ���߶�
		m_fBallMoveAngel+=0.03f;
		//if(m_fBallMoveAngel>2*D3DX_PI) m_fBallMoveAngel=0;
		m_Ball_StartHeight-=0.26f;

		//��������
		float x=m_Ball_MoveRadius*cosf(m_fBallMoveAngel)+455;
		float z=m_Ball_MoveRadius*sinf(m_fBallMoveAngel)+456;

		//�趨����
		movePath->x=x;
		movePath->y=m_Ball_StartHeight;
		movePath->z=z;
		return;
	}
	//��Ծ
	else if((int)m_Ball_MoveRadius<JUMP_RADIUS&&(int)m_Ball_MoveRadius>=WIN_RADIUS)
	{
		//ת���뾶
		m_Ball_MoveRadius-=0.20f;

		//ת���Ƕ���߶�
		m_fBallMoveAngel+=0.02f;
		//if(m_fBallMoveAngel>2*D3DX_PI) m_fBallMoveAngel=0;

		int iAngle=((int)(m_fBallMoveAngel/D3DX_PI*180))%10;
		if(iAngle>=0&&iAngle<=5) m_Ball_StartHeight-=3.6f+rand()%2;
		else m_Ball_StartHeight+=3.6f+rand()%2;

		//��������
		float x=m_Ball_MoveRadius*cosf(m_fBallMoveAngel)+455;
		float z=m_Ball_MoveRadius*sinf(m_fBallMoveAngel)+456;

		//��������߶�
		if(m_Ball_StartHeight<-320) m_Ball_StartHeight=-320;
		if(m_Ball_StartHeight>-310) m_Ball_StartHeight=-310;

		//�趨����
		movePath->x=x;
		movePath->y=m_Ball_StartHeight;
		movePath->z=z;
		return;
	}

	return;
}

//�õ�����ͣ����
BYTE CDXGameView::GetBallStopNumber(float angel)
{
	float nn=(float)360/37;
	int xx=(int)(angel/nn);
	return numberArrayIndex[xx];
}

//���õƹ�
void CDXGameView::SetLight()
{	
	//���õƹ�
	D3DLIGHT9 d3dLight;
	D3DXVECTOR3 vecDir;
	ZeroMemory(&d3dLight, sizeof(d3dLight));

	//1�ŷ����
	d3dLight.Type = D3DLIGHT_DIRECTIONAL;
	d3dLight.Diffuse.r  = 0.2f;
	d3dLight.Diffuse.g  = 0.2f;
	d3dLight.Diffuse.b  = 0.2f;

	d3dLight.Specular.r =1.0f;
	d3dLight.Specular.g =1.0f;
	d3dLight.Specular.b =1.0f;

	vecDir = D3DXVECTOR3(9, -13, 11);
	D3DXVec3Normalize((D3DXVECTOR3*)&d3dLight.Direction,&vecDir);
	d3dLight.Range        = 5000.0f;
	g_pD3DDevice->SetLight(1, &d3dLight);
	g_pD3DDevice->LightEnable( 1, TRUE );
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING,TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_AMBIENT,0x00a0a0a0);

	////2�ž۹��
	//d3dLight.Type=D3DLIGHT_SPOT;
	//d3dLight.Diffuse.r  = 1.0f;
	//d3dLight.Diffuse.g  = 1.0f;
	//d3dLight.Diffuse.b  = 1.0f;

	//d3dLight.Specular.r =1.0f;
	//d3dLight.Specular.g =1.0f;
	//d3dLight.Specular.b =1.0f;
	////d3dLight.Attenuation0=0.5f;
	//d3dLight.Position=D3DXVECTOR3(455,30,456);
	//d3dLight.Direction=D3DXVECTOR3(0.0f,-1.0f,0.0f);
	//d3dLight.Range        = 1000.0f;
	//d3dLight.Theta=D3DXToRadian(10.0f);
	//d3dLight.Phi=D3DXToRadian(60.0f);
	//d3dLight.Falloff=5.0f;
	//g_pD3DDevice->SetLight(2, &d3dLight);
	//g_pD3DDevice->LightEnable( 2, TRUE );
	//g_pD3DDevice->SetRenderState(D3DRS_LIGHTING,TRUE);

	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
}

//�ͷ���Դ
void CDXGameView::ReleaseScene()
{
	m_Chip1.Release();
	m_Chip5.Release();
	m_Chip10.Release();
	m_Chip50.Release();
	m_Chip100.Release();
	m_Chip500.Release();
	m_Chip1000.Release();
	m_Chip5000.Release();
	m_Chip10000.Release();
	m_SprCurrentChip.Release();

	for(int i=0;i<MAX_CHIPS;i++)
	{
		m_SprMoveChips[i].Release();
	}
	for(int i=0;i<10;i++)
	{
		m_SprTimer[i].Release();
	}
	m_PlatePlane.Release();
	m_PlatePlaneMask.Release();
	m_PlaneBall.Release();

	m_NumberMask.Release();
	m_SurfaceTable.Release();
	m_SurfaceBack.Release();

	m_MeshPlate.Release();
	m_MeshStock.Release();
	m_MeshShelfHigh.Release();
	m_MeshShelfLow.Release();
	m_MeshRing.Release();
	m_MeshBall.Release();

	SAFE_RELEASE(m_pReflect);
	SAFE_RELEASE(m_pBump);
}

//ȡ��ѹע��Ϣ���滭����
void CDXGameView::GetChipsInfo()
{
	//�滭����
	int x=0,y=0;		//��ע����
	int xPos=0,yPos=0;	//����λ��
	
	long lAllChips[256];
	ZeroMemory(lAllChips,sizeof(lAllChips));
	if(m_bStatus==STATUS_PLAY||m_bStatus==STATUS_READY)
	{
		CopyMemory(lAllChips,m_wOtherUsersChips,sizeof(lAllChips));
	}
	else
	{	for(int i=0;i<256;i++)
		{

			lAllChips[i]=m_wChipInfo[i]+m_wOtherUsersChips[i];
		}
	}

	//͸�ӹ�ϵ����ע����������˳��
	int temp[256]=
	{
		//������0��
		200,
		//������һ��
		206,3,132,6,135,9,138,12,141,15,144,18,147,21,150,24,153,27,156,30,159,33,162,36,
		205,92,55,95,58,98,61,101,64,104,67,107,70,110,73,113,76,116,79,119,82,122,85,125,
		204,2,131,5,134,8,137,11,140,14,143,17,146,20,149,23,152,26,155,29,158,32,161,35,
		203,91,54,94,57,97,60,100,63,103,66,106,69,109,72,112,75,115,78,118,81,121,84,124,
		202,1,130,4,133,7,136,10,139,13,142,16,145,19,148,22,151,25,154,28,157,31,160,34,
		201,90,53,93,56,96,59,99,62,102,65,105,68,108,71,111,74,114,77,117,80,120,83,123,
	};
	for(int i=0;i<256;i++)
	{
		int index=temp[i];
		if(lAllChips[index]!=0)
		{		
			if(index>0&&index<37)//��������
			{
				x=(index-1)%3,y=(index-1)/3;
				xPos=m_nScreenWidth/2 + (BetNumberArea[x][y].BasePoint.x+BetNumberArea[x][y].VertexRight.x/2) -m_Chip10.GetWidth()/2;
				yPos=m_nScreenHeight/2+ (BetNumberArea[x][y].BasePoint.y+BetNumberArea[x][y].VertexTop.y+(BetNumberArea[x][y].VertexButtom.y-BetNumberArea[x][y].VertexTop.y)/2 ) -m_Chip10.GetHeight()/2;
				DrawChips(xPos,yPos,lAllChips[index]);
			}
			if(index>=50&&index<86)//ʮ���ཻ����
			{
				x=(index-50)%3,y=(index-50)/3;
				xPos=m_nScreenWidth/2 + (CrossBetArea[x][y].BasePoint.x+CrossBetArea[x][y].VertexRight.x/2) -m_Chip10.GetWidth()/2;
				yPos=m_nScreenHeight/2+ (CrossBetArea[x][y].BasePoint.y+CrossBetArea[x][y].VertexTop.y+(CrossBetArea[x][y].VertexButtom.y-CrossBetArea[x][y].VertexTop.y)/2 ) -m_Chip10.GetHeight()/2;
				DrawChips(xPos,yPos,lAllChips[index]);
			}
			if(index>=90&&index<126)//�����ཻ����
			{
				x=(index-90)%3,y=(index-90)/3;
				xPos=m_nScreenWidth/2 + (WideSideCrossArea[x][y].BasePoint.x+WideSideCrossArea[x][y].VertexRight.x/2) -m_Chip10.GetWidth()/2;
				yPos=m_nScreenHeight/2+ (WideSideCrossArea[x][y].BasePoint.y+WideSideCrossArea[x][y].VertexTop.y+(WideSideCrossArea[x][y].VertexButtom.y-WideSideCrossArea[x][y].VertexTop.y)/2 ) -m_Chip10.GetHeight()/2;
				DrawChips(xPos,yPos,lAllChips[index]);
			}
			if(index>=130&&index<166)//�����ཻ����
			{
				x=(index-130)%3,y=(index-130)/3;
				xPos=m_nScreenWidth/2 + (HeightSideCrossArea[x][y].BasePoint.x+HeightSideCrossArea[x][y].VertexRight.x/2) -m_Chip10.GetWidth()/2;
				yPos=m_nScreenHeight/2+ (HeightSideCrossArea[x][y].BasePoint.y+HeightSideCrossArea[x][y].VertexTop.y+(HeightSideCrossArea[x][y].VertexButtom.y-HeightSideCrossArea[x][y].VertexTop.y)/2 ) -m_Chip10.GetHeight()/2;
				DrawChips(xPos,yPos,lAllChips[index]);
			}
			if(index==200)//0����
			{
				xPos=m_nScreenWidth/2 + (Bet0Area.BasePoint.x+Bet0Area.VertexButtom.x/2) -m_NumberMask.GetWidth()/2-8;
				yPos=m_nScreenHeight/2+ (Bet0Area.BasePoint.y+(Bet0Area.VertexButtom.y)/2) -m_NumberMask.GetHeight()/2+8;
				DrawChips(xPos,yPos,lAllChips[index]);
			}
			if(index>200&&index<207)//0�ཻ����
			{
				int x=index-201;
				xPos=m_nScreenWidth/2 + (Cross0Area[x].BasePoint.x+Cross0Area[x].VertexRight.x/2) -m_Chip10.GetWidth()/2;
				yPos=m_nScreenHeight/2+ (Cross0Area[x].BasePoint.y+Cross0Area[x].VertexTop.y+(Cross0Area[x].VertexButtom.y-Cross0Area[x].VertexTop.y)/2 ) -m_Chip10.GetHeight()/2;
				DrawChips(xPos,yPos,lAllChips[index]);
			}

		}
	}
	//�ܱ�����
	for(int k=37;k<49;k++)
	{
		if(lAllChips[k]>0)
		{
			switch(k)
			{
			case BET_IN_ERROR:
				break;
			case BET_IN_1TO12://��һ��
				{
					xPos=m_nScreenWidth/2 + (BetDozenArea[0].BasePoint.x+BetDozenArea[0].VertexRight.x/2) -m_Chip10.GetWidth()/2;
					yPos=m_nScreenHeight/2+ (BetDozenArea[0].BasePoint.y+BetDozenArea[0].VertexTop.y+(BetDozenArea[0].VertexButtom.y-BetDozenArea[0].VertexTop.y)/2 ) -m_Chip10.GetHeight()/2;
					DrawChips(xPos,yPos,lAllChips[BET_IN_1TO12]);
					break;
				}
			case BET_IN_13TO24://�ڶ���
				{
					xPos=m_nScreenWidth/2 + (BetDozenArea[1].BasePoint.x+BetDozenArea[1].VertexRight.x/2) -m_Chip10.GetWidth()/2;
					yPos=m_nScreenHeight/2+ (BetDozenArea[1].BasePoint.y+BetDozenArea[1].VertexTop.y+(BetDozenArea[1].VertexButtom.y-BetDozenArea[1].VertexTop.y)/2 ) -m_Chip10.GetHeight()/2;
					DrawChips(xPos,yPos,lAllChips[BET_IN_13TO24]);
					break;
				}
			case BET_IN_25TO36://������
				{
					xPos=m_nScreenWidth/2 + (BetDozenArea[2].BasePoint.x+BetDozenArea[2].VertexRight.x/2) -m_Chip10.GetWidth()/2;
					yPos=m_nScreenHeight/2+ (BetDozenArea[2].BasePoint.y+BetDozenArea[2].VertexTop.y+(BetDozenArea[2].VertexButtom.y-BetDozenArea[2].VertexTop.y)/2 ) -m_Chip10.GetHeight()/2;
					DrawChips(xPos,yPos,lAllChips[BET_IN_25TO36]);
					break;
				}
			case BET_IN_1TO18://1~18��
				{
					xPos=m_nScreenWidth/2 + (Bet1to18Area.BasePoint.x+Bet1to18Area.VertexRight.x/2) -m_Chip10.GetWidth()/2;
					yPos=m_nScreenHeight/2+ (Bet1to18Area.BasePoint.y+Bet1to18Area.VertexTop.y+(Bet1to18Area.VertexButtom.y-Bet1to18Area.VertexTop.y)/2 ) -m_Chip10.GetHeight()/2;
					DrawChips(xPos,yPos,lAllChips[BET_IN_1TO18]);
					break;
				}
			case BET_IN_19TO36://19~36��
				{
					xPos=m_nScreenWidth/2 + (Bet19to36Area.BasePoint.x+Bet19to36Area.VertexRight.x/2) -m_Chip10.GetWidth()/2;
					yPos=m_nScreenHeight/2+ (Bet19to36Area.BasePoint.y+Bet19to36Area.VertexTop.y+(Bet19to36Area.VertexButtom.y-Bet19to36Area.VertexTop.y)/2 ) -m_Chip10.GetHeight()/2;
					DrawChips(xPos,yPos,lAllChips[BET_IN_19TO36]);
					break;
				}
			case BET_IN_RED://��ɫ��
				{
					xPos=m_nScreenWidth/2 + (BetRedArea.BasePoint.x+BetRedArea.VertexRight.x/2) -m_Chip10.GetWidth()/2;
					yPos=m_nScreenHeight/2+ (BetRedArea.BasePoint.y+BetRedArea.VertexTop.y+(BetRedArea.VertexButtom.y-BetRedArea.VertexTop.y)/2 ) -m_Chip10.GetHeight()/2;
					DrawChips(xPos,yPos,lAllChips[BET_IN_RED]);
					break;
				}
			case BET_IN_BLACK://��ɫ��
				{
					xPos=m_nScreenWidth/2 + (BetBlackArea.BasePoint.x+BetBlackArea.VertexRight.x/2) -m_Chip10.GetWidth()/2;
					yPos=m_nScreenHeight/2+ (BetBlackArea.BasePoint.y+BetBlackArea.VertexTop.y+(BetBlackArea.VertexButtom.y-BetBlackArea.VertexTop.y)/2 ) -m_Chip10.GetHeight()/2;
					DrawChips(xPos,yPos,lAllChips[BET_IN_BLACK]);
					break;
				}
			case BET_IN_SINGLE://������
				{
					xPos=m_nScreenWidth/2 + (BetSingleArea.BasePoint.x+BetSingleArea.VertexRight.x/2) -m_Chip10.GetWidth()/2;
					yPos=m_nScreenHeight/2+ (BetSingleArea.BasePoint.y+BetSingleArea.VertexTop.y+(BetSingleArea.VertexButtom.y-BetSingleArea.VertexTop.y)/2 ) -m_Chip10.GetHeight()/2;
					DrawChips(xPos,yPos,lAllChips[BET_IN_SINGLE]);
					break;
				}
			case BET_IN_DOUBLE://˫����
				{
					xPos=m_nScreenWidth/2 + (BetDoubleArea.BasePoint.x+BetDoubleArea.VertexRight.x/2) -m_Chip10.GetWidth()/2;
					yPos=m_nScreenHeight/2+ (BetDoubleArea.BasePoint.y+BetDoubleArea.VertexTop.y+(BetDoubleArea.VertexButtom.y-BetDoubleArea.VertexTop.y)/2 ) -m_Chip10.GetHeight()/2;
					DrawChips(xPos,yPos,lAllChips[BET_IN_DOUBLE]);
					break;
				}
			case BET_IN_LINE1://��һ��
				{
					xPos=m_nScreenWidth/2 + (BetLineArea[0].BasePoint.x+BetLineArea[0].VertexRight.x/2) -m_Chip10.GetWidth()/2;
					yPos=m_nScreenHeight/2+ (BetLineArea[0].BasePoint.y+BetLineArea[0].VertexTop.y+(BetLineArea[0].VertexButtom.y-BetLineArea[0].VertexTop.y)/2 ) -m_Chip10.GetHeight()/2;
					DrawChips(xPos,yPos,lAllChips[BET_IN_LINE1]);
					break;
				}
			case BET_IN_LINE2://�ڶ���
				{
					xPos=m_nScreenWidth/2 + (BetLineArea[1].BasePoint.x+BetLineArea[1].VertexRight.x/2) -m_Chip10.GetWidth()/2;
					yPos=m_nScreenHeight/2+ (BetLineArea[1].BasePoint.y+BetLineArea[1].VertexTop.y+(BetLineArea[1].VertexButtom.y-BetLineArea[1].VertexTop.y)/2 ) -m_Chip10.GetHeight()/2;
					DrawChips(xPos,yPos,lAllChips[BET_IN_LINE2]);
					break;
				}
			case BET_IN_LINE3://������
				{
					xPos=m_nScreenWidth/2 + (BetLineArea[2].BasePoint.x+BetLineArea[2].VertexRight.x/2) -m_Chip10.GetWidth()/2;
					yPos=m_nScreenHeight/2+ (BetLineArea[2].BasePoint.y+BetLineArea[2].VertexTop.y+(BetLineArea[2].VertexButtom.y-BetLineArea[2].VertexTop.y)/2 ) -m_Chip10.GetHeight()/2;
					DrawChips(xPos,yPos,lAllChips[BET_IN_LINE3]);
					break;
				}
			default:
				{
					break;
				}
			}
		}
	}

	return;
}

//ȡ��ѹע���
long CDXGameView::GetAllMoney()
{
	long allmoney=0;
	for(int i=0;i<MAX_BET_POS;i++)
	{
		if(m_wChipInfo[i]!=0&&i!=BET_IN_ERROR)
			allmoney+=m_wChipInfo[i];
	}
	return allmoney;
}
//ȡ����ʷ��ע
long CDXGameView::GetHistoryMoney()
{
	long allmoney=0;
	for(int i=0;i<MAX_BET_POS;i++)
	{
		if(m_wHistoryChipInfo[i]!=0&&i!=BET_IN_ERROR)
			allmoney+=m_wHistoryChipInfo[i];
	}
	return allmoney;
}

//����ƶ���Ϣ
void CDXGameView::OnMouseMove(UINT nFlags,CPoint point)
{
	SetCursor(LoadCursor(NULL,IDC_ARROW));

	//����Ӧ
	if(!m_bAllowBet) return;

	//�����־
	m_bZeroLight=false;
	ZeroMemory(m_TipsMessage,sizeof(m_TipsMessage));
	ZeroMemory(m_bNumberLight,sizeof(m_bNumberLight));
	
	//�����Ӧ
	if(m_GameLogic.RespondMouseInRegionX(point,m_bNumberLight,m_bZeroLight,m_TipsMessage))
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
	return __super::OnMouseMove(nFlags,point);
}

//���������Ϣ
void CDXGameView::OnLButtonDown(UINT nFlags,CPoint point)
{
	if(!m_bAllowBet) return __super::OnLButtonDown(nFlags,point);;

	//�������
	WORD index=m_GameLogic.GetMouseClickRegion(point);
	if(index==BET_IN_ERROR) return __super::OnLButtonDown(nFlags,point);

	//ѡ���������
	if(index>=900)
	{
		m_wCurrentChip=lScoreIndex[index-900];
		return __super::OnLButtonDown(nFlags,point);
	}

	//�����û�������
	if((GetAllMoney()+m_wCurrentChip)>m_lMaxUserScore)
	{
		AfxMessageBox("���ķ��������Թ���˳��롣");
		return __super::OnLButtonDown(nFlags,point);
	}

	//������ע
	//��һ������
	if(index>=1&&index<=36)
	{
		if((m_wChipInfo[index]+m_wCurrentChip)>(m_lMaxBetWin/36))
		{
			m_wChipInfo[index]=m_lMaxBetWin/36;
			_snprintf(m_TipsMessage,sizeof(TCHAR)*64,TEXT("�ﵽ�����ע %d"),m_lMaxBetWin/36);
			return __super::OnLButtonDown(nFlags,point);
			//lstrcpyn(m_TipsMessage,TEXT("�ﵽ�����ע500"),sizeof(TCHAR)*64);
		}
	}
	//�д�����
	if(index>=37&&index<=42) 
	{
		if((m_wChipInfo[index]+m_wCurrentChip)>(m_lMaxBetWin/3))
		{
			m_wChipInfo[index]=m_lMaxBetWin/3;
			_snprintf(m_TipsMessage,sizeof(TCHAR)*64,TEXT("�ﵽ�����ע %d"),m_lMaxBetWin/3);
			return __super::OnLButtonDown(nFlags,point);
			//lstrcpyn(m_TipsMessage,TEXT("�ﵽ�����ע8750"),sizeof(TCHAR)*64);
		}
	}
	//1~18��19~36��˫�����
	if(index>=43&&index<=48)
	{
		if((m_wChipInfo[index]+m_wCurrentChip)>m_lMaxBetWin/2)
		{
			m_wChipInfo[index]=m_lMaxBetWin/2;
			_snprintf(m_TipsMessage,sizeof(TCHAR)*64,TEXT("�ﵽ�����ע %d"),m_lMaxBetWin/2);
			return __super::OnLButtonDown(nFlags,point);
			//lstrcpyn(m_TipsMessage,TEXT("�ﵽ�����ע17500"),sizeof(TCHAR)*64);
		}
	}
	//ʮ�ֽ��������
	if(index>=53&&index<=83&&(index-53)%3==0)
	{
		if((m_wChipInfo[index]+m_wCurrentChip)>(m_lMaxBetWin/6))
		{
			m_wChipInfo[index]=m_lMaxBetWin/6;
			_snprintf(m_TipsMessage,sizeof(TCHAR)*64,TEXT("�ﵽ�����ע %d"),m_lMaxBetWin/6);
			return __super::OnLButtonDown(nFlags,point);
			//lstrcpyn(m_TipsMessage,TEXT("�ﵽ�����ע3500"),sizeof(TCHAR)*64);
		}
	}
	//ʮ�ֽ���
	if(index>=53&&index<=85&&(index-53)%3!=0)
	{
		if((m_wChipInfo[index]+m_wCurrentChip)>(m_lMaxBetWin/9))
		{
			m_wChipInfo[index]=(m_lMaxBetWin/9);
			_snprintf(m_TipsMessage,sizeof(TCHAR)*64,TEXT("�ﵽ�����ע %d"),m_lMaxBetWin/9);
			return __super::OnLButtonDown(nFlags,point);
			//lstrcpyn(m_TipsMessage,TEXT("�ﵽ�����ע2187"),sizeof(TCHAR)*64);
		}
	}
	//���ཻ�������
	if(index>=90&&index<=123&&index%3==0)
	{
		if((m_wChipInfo[index]+m_wCurrentChip)>(m_lMaxBetWin/12))
		{
			m_wChipInfo[index]=(m_lMaxBetWin/12);
			_snprintf(m_TipsMessage,sizeof(TCHAR)*64,TEXT("�ﵽ�����ע %d"),m_lMaxBetWin/12);
			return __super::OnLButtonDown(nFlags,point);
			//lstrcpyn(m_TipsMessage,TEXT("�ﵽ�����ע1590"),sizeof(TCHAR)*64);
		}
	}
	//���ཻ��
	if(index>=90&&index<=125&&index%3!=0)
	{
		if((m_wChipInfo[index]+m_wCurrentChip)>(m_lMaxBetWin/18))
		{
			m_wChipInfo[index]=(m_lMaxBetWin/18);
			_snprintf(m_TipsMessage,sizeof(TCHAR)*64,TEXT("�ﵽ�����ע %d"),m_lMaxBetWin/18);
			return __super::OnLButtonDown(nFlags,point);
			//lstrcpyn(m_TipsMessage,TEXT("�ﵽ�����ע1030"),sizeof(TCHAR)*64);
		}
	}
	//���ཻ��
	if(index>=130&&index<=162)	
	{
		if((m_wChipInfo[index]+m_wCurrentChip)>(m_lMaxBetWin/18))
		{
			m_wChipInfo[index]=(m_lMaxBetWin/18);
			_snprintf(m_TipsMessage,sizeof(TCHAR)*64,TEXT("�ﵽ�����ע %d"),m_lMaxBetWin/18);
			return __super::OnLButtonDown(nFlags,point);
			//lstrcpyn(m_TipsMessage,TEXT("�ﵽ�����ע1030"),sizeof(TCHAR)*64);
		}
	}
	//������
	if(index==200)	
	{
		if((m_wChipInfo[index]+m_wCurrentChip)>(m_lMaxBetWin/36))
		{
			m_wChipInfo[index]=(m_lMaxBetWin/36);
			_snprintf(m_TipsMessage,sizeof(TCHAR)*64,TEXT("�ﵽ�����ע %d"),m_lMaxBetWin/36);
			return __super::OnLButtonDown(nFlags,point);
			//lstrcpyn(m_TipsMessage,TEXT("�ﵽ�����ע500"),sizeof(TCHAR)*64);
		}
	}
	if(index==201)		
	{
		if((m_wChipInfo[index]+m_wCurrentChip)>(m_lMaxBetWin/9))
		{
			m_wChipInfo[index]=(m_lMaxBetWin/9);
			_snprintf(m_TipsMessage,sizeof(TCHAR)*64,TEXT("�ﵽ�����ע %d"),m_lMaxBetWin/9);
			return __super::OnLButtonDown(nFlags,point);
			//lstrcpyn(m_TipsMessage,TEXT("�ﵽ�����ע2187"),sizeof(TCHAR)*64);
		}
	}
	if(index==203||index==205)		
	{
		if((m_wChipInfo[index]+m_wCurrentChip)>(m_lMaxBetWin/12))
		{
			m_wChipInfo[index]=(m_lMaxBetWin/12);
			_snprintf(m_TipsMessage,sizeof(TCHAR)*64,TEXT("�ﵽ�����ע %d"),m_lMaxBetWin/12);
			return __super::OnLButtonDown(nFlags,point);
			//lstrcpyn(m_TipsMessage,TEXT("�ﵽ�����ע1590"),sizeof(TCHAR)*64);
		}
	}
	if(index==202||index==204||index==206)	
	{
		if((m_wChipInfo[index]+m_wCurrentChip)>(m_lMaxBetWin/18))
		{
			m_wChipInfo[index]=(m_lMaxBetWin/18);
			_snprintf(m_TipsMessage,sizeof(TCHAR)*64,TEXT("�ﵽ�����ע %d"),m_lMaxBetWin/18);
			return __super::OnLButtonDown(nFlags,point);
			//lstrcpyn(m_TipsMessage,TEXT("�ﵽ�����ע1030"),sizeof(TCHAR)*64);
		}
	}

	//��ע����
	m_wChipInfo[index]+=m_wCurrentChip;

	return __super::OnLButtonDown(nFlags,point);
}

//�Ҽ�������Ϣ
void CDXGameView::OnRButtonDown(UINT nFlags,CPoint point)
{
	if(!m_bAllowBet) return;

	//������ע
	WORD index=m_GameLogic.GetMouseClickRegion(point);

	//ѡ���������
	if(index>=900) return __super::OnRButtonDown(nFlags,point);

	//��ע����
	m_wChipInfo[index]-=m_wCurrentChip;
	if(m_wChipInfo[index]<0) m_wChipInfo[index]=0;

	return __super::OnRButtonDown(nFlags,point);
}

//ȷ����ע
void CDXGameView::OnBnClickedChipIn()
{
	//��ȡ��ʷ��Ϣ
	if(GetHistoryMoney()>0&&GetAllMoney()==0)
	{
		//�ж��Ƿ񹻷����ظ���ע
		if(GetHistoryMoney()>m_lMaxUserScore)
		{
			AfxMessageBox("���ĳ��벻�����ظ����ֵ�ע���ѡ����ע��");
			return;
		}
		CopyMemory(m_wChipInfo,m_wHistoryChipInfo,sizeof(m_wChipInfo));
	}

	//δ��ע����
	if(GetAllMoney()==0)
	{
		AfxMessageBox("������ע��");
		return;
	}

	//�ж�����λ����עС�������ע
	for(int i=0;i<MAX_BET_POS;i++)
	{
		//����ע
		if(m_wChipInfo[i]>0)
		{
			//С�������ע
			if(m_wChipInfo[i]<m_lMinBetScore)
			{
				TCHAR szBuffer[64];ZeroMemory(szBuffer,sizeof(szBuffer));
				_snprintf(szBuffer,sizeof(TCHAR)*64,TEXT("ĳ����עλ�ó���С�������ע %ld �������ӳ��롣"),m_lMinBetScore);
				AfxMessageBox(szBuffer);
				return;
			}
		}
	}

	//������ע��Ϣ
	AfxGetMainWnd()->SendMessage(IDM_CHIP_IN,(WPARAM)m_wChipInfo);

	//���ñ���
	m_bShowEnd=0;
	m_bAllowBet=false;
	m_bStatus=STATUS_READY;
		
	m_bZeroLight=false;
	ZeroMemory(m_TipsMessage,sizeof(m_TipsMessage));
	ZeroMemory(m_bNumberLight,sizeof(m_bNumberLight));

	//���浽��ʷ��Ϣ
	CopyMemory(m_wHistoryChipInfo,m_wChipInfo,sizeof(m_wChipInfo));

	m_btChipIn.ShowWindow(SW_HIDE);
	m_btClearChip.ShowWindow(SW_HIDE);
	m_ScoreView.ShowWindow(SW_HIDE);
	//m_Ball_MoveRadius=MAX_MOVE_RADIUS;

	return;
}

//�����ע
void CDXGameView::OnBnClickedClearChip()
{
	ClearAllMoney();
	return ;
}

//�����ע
void CDXGameView::ClearAllMoney()
{
	for(int i=0;i<MAX_BET_POS;i++)
	{
		m_wChipInfo[i]=0;
	}
	return;
}

//��ʼ��ת
void CDXGameView::OnGameStart(BYTE bTargetNumber)
{
	//���ñ���
	m_bWinNumber=255;
	m_bStatus=STATUS_PLAY;
	m_Ball_MoveRadius=MAX_MOVE_RADIUS;
	m_Ball_StartHeight=-310;
	//m_fBallMoveAngel=0.;
	m_bAllowBet=false;
	m_bTargetNumber=bTargetNumber;

	//�����־
	m_bZeroLight=false;
	ZeroMemory(m_TipsMessage,sizeof(m_TipsMessage));
	ZeroMemory(m_bNumberLight,sizeof(m_bNumberLight));

	//ȡ�õ�ǰ���̽Ƕ�
	D3DXMATRIX plateRot= m_MeshPlate.GetRotate();
	float x=plateRot._11;
	float z=plateRot._13;
	//��ǰ���������ĵ�н�(�Ѿ���ת�Ķ���)
	float currentPlatAngel=0;
	float PlatAngel=asin(z)/D3DX_PI*180;
	if(x>0&&z>0) currentPlatAngel=PlatAngel;	//��һ����
	if(x<0&&z>0) currentPlatAngel=180-PlatAngel;//�ڶ�����
	if(x<0&&z<0) currentPlatAngel=180-PlatAngel;//��������
	if(x>0&&z<0) currentPlatAngel=360+PlatAngel;//��������

	//ת��ʤ������
	float iAngle=0;
	for(int i=0;i<37;i++)
	{
		if(m_bTargetNumber==numberArrayIndex[i]) iAngle=i*(2*D3DX_PI/37);;
	}
	m_fBallMoveAngel=iAngle+(2*D3DX_PI/37)*4+((currentPlatAngel/180)*D3DX_PI);
	//if(m_fBallMoveAngel>2*D3DX_PI) m_fBallMoveAngel=m_fBallMoveAngel/(2*D3DX_PI);


	return;
}
//������ע
void CDXGameView::OnUserChip(long lAllChip[256])
{
	CopyMemory(m_wOtherUsersChips,lAllChip,sizeof(m_wOtherUsersChips));
	return;
}
//������ת
void CDXGameView::OnRollStop(BYTE bWinNumber)
{
	m_bWinNumber=bWinNumber;
	m_bStatus=STATUS_FREE;
	AfxGetMainWnd()->SendMessage(IDM_GET_STOP);
}
//��Ϸ����
void CDXGameView::OnGameEnd()
{
	//�����ƶ�����
	for(int i=0;i<MAX_BET_POS;i++)
	{
		if(m_wChipInfo[i]>0)
		{
			long temp[256];ZeroMemory(temp,sizeof(temp));
			temp[i]=m_wChipInfo[i];
			long lScore=m_GameLogic.ComputeScore(temp,m_bWinNumber);
			//�˴��÷�
			if(lScore>0)
			{
				//AfxMessageBox("sdfd");
				CPoint point(0,0);
				GetChipPosition(i,&point);
				m_bDrawMoveChips=true;
				SetMoveChips(point.x,point.y,m_wChipInfo[i]);
			}
		}
	}

	m_bStatus=STATUS_FREE;
	ZeroMemory(m_wChipInfo,sizeof(m_wChipInfo));
	ZeroMemory(m_wOtherUsersChips,sizeof(m_wOtherUsersChips));
}

//ȡ�ó���λ��
void CDXGameView::GetChipPosition(int index,CPoint *pPosition)
{
	int x=0,y=0;
	if(index>0&&index<37)//������������
	{
		x=(index-1)%3,y=(index-1)/3;
		pPosition->x=m_nScreenWidth/2 + (BetNumberArea[x][y].BasePoint.x+BetNumberArea[x][y].VertexRight.x/2) -m_Chip10.GetWidth()/2;
		pPosition->y=m_nScreenHeight/2+ (BetNumberArea[x][y].BasePoint.y+BetNumberArea[x][y].VertexTop.y+(BetNumberArea[x][y].VertexButtom.y-BetNumberArea[x][y].VertexTop.y)/2 ) -m_Chip10.GetHeight()/2;
		return;
	}
	if(index>=50&&index<86)//ʮ���ཻ����
	{
		x=(index-50)%3,y=(index-50)/3;
		pPosition->x=m_nScreenWidth/2 + (CrossBetArea[x][y].BasePoint.x+CrossBetArea[x][y].VertexRight.x/2) -m_Chip10.GetWidth()/2;
		pPosition->y=m_nScreenHeight/2+ (CrossBetArea[x][y].BasePoint.y+CrossBetArea[x][y].VertexTop.y+(CrossBetArea[x][y].VertexButtom.y-CrossBetArea[x][y].VertexTop.y)/2 ) -m_Chip10.GetHeight()/2;
		return;
	}
	if(index>=90&&index<126)//�����ཻ����
	{
		x=(index-90)%3,y=(index-90)/3;
		pPosition->x=m_nScreenWidth/2 + (WideSideCrossArea[x][y].BasePoint.x+WideSideCrossArea[x][y].VertexRight.x/2) -m_Chip10.GetWidth()/2;
		pPosition->y=m_nScreenHeight/2+ (WideSideCrossArea[x][y].BasePoint.y+WideSideCrossArea[x][y].VertexTop.y+(WideSideCrossArea[x][y].VertexButtom.y-WideSideCrossArea[x][y].VertexTop.y)/2 ) -m_Chip10.GetHeight()/2;
		return;
	}
	if(index>=130&&index<166)//�����ཻ����
	{
		x=(index-130)%3,y=(index-130)/3;
		pPosition->x=m_nScreenWidth/2 + (HeightSideCrossArea[x][y].BasePoint.x+HeightSideCrossArea[x][y].VertexRight.x/2) -m_Chip10.GetWidth()/2;
		pPosition->y=m_nScreenHeight/2+ (HeightSideCrossArea[x][y].BasePoint.y+HeightSideCrossArea[x][y].VertexTop.y+(HeightSideCrossArea[x][y].VertexButtom.y-HeightSideCrossArea[x][y].VertexTop.y)/2 ) -m_Chip10.GetHeight()/2;
		return;
	}
	if(index==200)//0����
	{
		pPosition->x=m_nScreenWidth/2 + (Bet0Area.BasePoint.x+Bet0Area.VertexButtom.x/2) -m_NumberMask.GetWidth()/2-8;
		pPosition->y=m_nScreenHeight/2+ (Bet0Area.BasePoint.y+(Bet0Area.VertexButtom.y)/2) -m_NumberMask.GetHeight()/2+8;
		return;
	}
	if(index>200&&index<207)//0�ཻ����
	{
		int x=index-201;
		pPosition->x=m_nScreenWidth/2 + (Cross0Area[x].BasePoint.x+Cross0Area[x].VertexRight.x/2) -m_Chip10.GetWidth()/2;
		pPosition->y=m_nScreenHeight/2+ (Cross0Area[x].BasePoint.y+Cross0Area[x].VertexTop.y+(Cross0Area[x].VertexButtom.y-Cross0Area[x].VertexTop.y)/2 ) -m_Chip10.GetHeight()/2;
		return;
	}

	////�ܱ�����
	//for(int k=37;k<49;k++)
	//{
	//	if(m_wChipInfo[k]>0)
	//	{
	//		switch(k)
	//		{
	//		case BET_IN_ERROR:
	//			break;
			if(index== BET_IN_1TO12)//��һ��
				{
					pPosition->x=m_nScreenWidth/2 + (BetDozenArea[0].BasePoint.x+BetDozenArea[0].VertexRight.x/2) -m_Chip10.GetWidth()/2;
					pPosition->y=m_nScreenHeight/2+ (BetDozenArea[0].BasePoint.y+BetDozenArea[0].VertexTop.y+(BetDozenArea[0].VertexButtom.y-BetDozenArea[0].VertexTop.y)/2 ) -m_Chip10.GetHeight()/2;
					return;
				}
			if(index==  BET_IN_13TO24)//�ڶ���
				{
					pPosition->x=m_nScreenWidth/2 + (BetDozenArea[1].BasePoint.x+BetDozenArea[1].VertexRight.x/2) -m_Chip10.GetWidth()/2;
					pPosition->y=m_nScreenHeight/2+ (BetDozenArea[1].BasePoint.y+BetDozenArea[1].VertexTop.y+(BetDozenArea[1].VertexButtom.y-BetDozenArea[1].VertexTop.y)/2 ) -m_Chip10.GetHeight()/2;
					return;
				}
			if(index==  BET_IN_25TO36)//������
				{
					pPosition->x=m_nScreenWidth/2 + (BetDozenArea[2].BasePoint.x+BetDozenArea[2].VertexRight.x/2) -m_Chip10.GetWidth()/2;
					pPosition->y=m_nScreenHeight/2+ (BetDozenArea[2].BasePoint.y+BetDozenArea[2].VertexTop.y+(BetDozenArea[2].VertexButtom.y-BetDozenArea[2].VertexTop.y)/2 ) -m_Chip10.GetHeight()/2;
					return;
				}
			if(index==  BET_IN_1TO18)//1~18��
				{
					pPosition->x=m_nScreenWidth/2 + (Bet1to18Area.BasePoint.x+Bet1to18Area.VertexRight.x/2) -m_Chip10.GetWidth()/2;
					pPosition->y=m_nScreenHeight/2+ (Bet1to18Area.BasePoint.y+Bet1to18Area.VertexTop.y+(Bet1to18Area.VertexButtom.y-Bet1to18Area.VertexTop.y)/2 ) -m_Chip10.GetHeight()/2;
					return;
				}
			if(index==  BET_IN_19TO36)//19~36��
				{
					pPosition->x=m_nScreenWidth/2 + (Bet19to36Area.BasePoint.x+Bet19to36Area.VertexRight.x/2) -m_Chip10.GetWidth()/2;
					pPosition->y=m_nScreenHeight/2+ (Bet19to36Area.BasePoint.y+Bet19to36Area.VertexTop.y+(Bet19to36Area.VertexButtom.y-Bet19to36Area.VertexTop.y)/2 ) -m_Chip10.GetHeight()/2;
					return;
				}
			if(index==  BET_IN_RED)//��ɫ��
				{
					pPosition->x=m_nScreenWidth/2 + (BetRedArea.BasePoint.x+BetRedArea.VertexRight.x/2) -m_Chip10.GetWidth()/2;
					pPosition->y=m_nScreenHeight/2+ (BetRedArea.BasePoint.y+BetRedArea.VertexTop.y+(BetRedArea.VertexButtom.y-BetRedArea.VertexTop.y)/2 ) -m_Chip10.GetHeight()/2;
					return;
				}
			if(index==  BET_IN_BLACK)//��ɫ��
				{
					pPosition->x=m_nScreenWidth/2 + (BetBlackArea.BasePoint.x+BetBlackArea.VertexRight.x/2) -m_Chip10.GetWidth()/2;
					pPosition->y=m_nScreenHeight/2+ (BetBlackArea.BasePoint.y+BetBlackArea.VertexTop.y+(BetBlackArea.VertexButtom.y-BetBlackArea.VertexTop.y)/2 ) -m_Chip10.GetHeight()/2;
					return;
				}
			if(index==  BET_IN_SINGLE)//������
				{
					pPosition->x=m_nScreenWidth/2 + (BetSingleArea.BasePoint.x+BetSingleArea.VertexRight.x/2) -m_Chip10.GetWidth()/2;
					pPosition->y=m_nScreenHeight/2+ (BetSingleArea.BasePoint.y+BetSingleArea.VertexTop.y+(BetSingleArea.VertexButtom.y-BetSingleArea.VertexTop.y)/2 ) -m_Chip10.GetHeight()/2;
					return;
				}
			if(index==  BET_IN_DOUBLE)//˫����
				{
					pPosition->x=m_nScreenWidth/2 + (BetDoubleArea.BasePoint.x+BetDoubleArea.VertexRight.x/2) -m_Chip10.GetWidth()/2;
					pPosition->y=m_nScreenHeight/2+ (BetDoubleArea.BasePoint.y+BetDoubleArea.VertexTop.y+(BetDoubleArea.VertexButtom.y-BetDoubleArea.VertexTop.y)/2 ) -m_Chip10.GetHeight()/2;
					return;
				}
			if(index==  BET_IN_LINE1)//��һ��
				{
					pPosition->x=m_nScreenWidth/2 + (BetLineArea[0].BasePoint.x+BetLineArea[0].VertexRight.x/2) -m_Chip10.GetWidth()/2;
					pPosition->y=m_nScreenHeight/2+ (BetLineArea[0].BasePoint.y+BetLineArea[0].VertexTop.y+(BetLineArea[0].VertexButtom.y-BetLineArea[0].VertexTop.y)/2 ) -m_Chip10.GetHeight()/2;
					return;
				}
			if(index==  BET_IN_LINE2)//�ڶ���
				{
					pPosition->x=m_nScreenWidth/2 + (BetLineArea[1].BasePoint.x+BetLineArea[1].VertexRight.x/2) -m_Chip10.GetWidth()/2;
					pPosition->y=m_nScreenHeight/2+ (BetLineArea[1].BasePoint.y+BetLineArea[1].VertexTop.y+(BetLineArea[1].VertexButtom.y-BetLineArea[1].VertexTop.y)/2 ) -m_Chip10.GetHeight()/2;
					return;
				}
			if(index==  BET_IN_LINE3)//������
				{
					pPosition->x=m_nScreenWidth/2 + (BetLineArea[2].BasePoint.x+BetLineArea[2].VertexRight.x/2) -m_Chip10.GetWidth()/2;
					pPosition->y=m_nScreenHeight/2+ (BetLineArea[2].BasePoint.y+BetLineArea[2].VertexTop.y+(BetLineArea[2].VertexButtom.y-BetLineArea[2].VertexTop.y)/2 ) -m_Chip10.GetHeight()/2;
					return;
				}

	return;
}

//��ʷ����
void CDXGameView::SetHistoryNumber(BYTE bHistoryInfo[])
{
	CopyMemory(m_bHistoryInfo,bHistoryInfo,sizeof(m_bHistoryInfo));
	return;
}

//�滭��ʷ��¼
void CDXGameView::DrawHistory()
{
	//��ʼλ��
	int StartX,StartY,x,y;
	x=StartX=m_nScreenWidth/2+325;
	y=StartY=m_nScreenHeight/2-275;

	//������ɫ
	COLORREF cTextColor=RGB(100,255,100);
	//�滭��ʷ����
	for(BYTE i=0;i<HISTORY_NUMBER;i++)
	{
		//�滭����
		if(m_bHistoryInfo[i]==0xff) break;

		//�ж�λ��
		BYTE color=m_GameLogic.GetNumberColor(m_bHistoryInfo[i]);
		//��ɫ����
		if(color==COLOR_RED) 
		{
			x=StartX;
			cTextColor=RGB(100,255,100);
		}
		//����0
		if(color==COLOR_0)
		{
			x=StartX+18;
		}
		//��ɫ����
		if(color==COLOR_BLACK)
		{
			x=StartX+36;
			cTextColor=RGB(255,0,100);
		}

		//ת������
		CString nText;
		nText.Format("%d",m_bHistoryInfo[i]);
		DxDrawText(nText,x,y,cTextColor);

		//���ƻ滭
		y+=15;
	}
}

//�滭����
void CDXGameView::DrawChips(int x,int y,long money)
{
	long lScoreCount=money;
	//��������
	LONG lScoreIndex[JETTON_COUNT]={1L,5L,10L,50L,100L,500L,1000L,5000L,10000L};

	//���ӳ���
	for (BYTE i=0;i<CountArray(lScoreIndex);i++)
	{
		//������Ŀ
		BYTE cbScoreIndex=JETTON_COUNT-i-1;
		LONG lCellCount=lScoreCount/lScoreIndex[cbScoreIndex];

		//�������
		if (lCellCount==0L) continue;

		//�������
		for (LONG j=0;j<lCellCount;j++)
		{
			switch(cbScoreIndex)
			{
			case 0:{	m_Chip1.DrawSprite(x,y-=4,0xffffffff);	break;	}
			case 1:{	m_Chip5.DrawSprite(x,y-=4,0xffffffff);	break;	}
			case 2:{	m_Chip10.DrawSprite(x,y-=4,0xffffffff);	break;	}
			case 3:{	m_Chip50.DrawSprite(x,y-=4,0xffffffff);	break;	}
			case 4:{	m_Chip100.DrawSprite(x,y-=4,0xffffffff);break;	}
			case 5:{	m_Chip500.DrawSprite(x,y-=4,0xffffffff);break;	}
			case 6:{	m_Chip1000.DrawSprite(x,y-=4,0xffffffff);break;	}
			case 7:{	m_Chip5000.DrawSprite(x,y-=4,0xffffffff);break;	}
			case 8:{	m_Chip10000.DrawSprite(x,y-=4,0xffffffff);break;}
			default:break;
			}
		}

		//������Ŀ
		lScoreCount-=lCellCount*lScoreIndex[cbScoreIndex];
	}

	return;
}

//�滭�����ƶ�
void CDXGameView::SetMoveChips(int x,int y,long money)
{
	long lScoreCount=money;
	//��������
	LONG lScoreIndex[JETTON_COUNT]={1L,5L,10L,50L,100L,500L,1000L,5000L,10000L};
	
	//����·��
	int startX=0,startY=0,stopX=0,stopY=0;
	startX=x;
	startY=y;
	stopX=WIN_CHIP_POS_X;
	stopY=WIN_CHIP_POS_Y;

	//���ӳ���
	for (BYTE i=0;i<CountArray(lScoreIndex);i++)
	{
		//������Ŀ
		BYTE cbScoreIndex=JETTON_COUNT-i-1;
		LONG lCellCount=lScoreCount/lScoreIndex[cbScoreIndex];

		//�������
		if (lCellCount==0L) continue;

		//�������
		for (LONG j=0;j<lCellCount;j++)
		{
			switch(cbScoreIndex)
			{
			case 0:
			{
				m_SprMoveChips[m_iMoveChipsCount].SetLoadInfo(g_pD3DDevice,TEXT("Round\\1.png")); 
				m_SprMoveChips[m_iMoveChipsCount].SetMovePath(startX,startY-=4,stopX,stopY-=4);	
				m_iMoveChipsCount++;
				break;	
			}
			case 1:
			{
				m_SprMoveChips[m_iMoveChipsCount].SetLoadInfo(g_pD3DDevice,TEXT("Round\\5.png")); 
				m_SprMoveChips[m_iMoveChipsCount].SetMovePath(startX,startY-=4,stopX,stopY-=4);	
				m_iMoveChipsCount++;
				break;	
			}
			case 2:
			{
				m_SprMoveChips[m_iMoveChipsCount].SetLoadInfo(g_pD3DDevice,TEXT("Round\\10.png")); 
				m_SprMoveChips[m_iMoveChipsCount].SetMovePath(startX,startY-=4,stopX,stopY-=4);	
				m_iMoveChipsCount++;
				break;	
			}
			case 3:
			{	
				m_SprMoveChips[m_iMoveChipsCount].SetLoadInfo(g_pD3DDevice,TEXT("Round\\50.png")); 
				m_SprMoveChips[m_iMoveChipsCount].SetMovePath(startX,startY-=4,stopX,stopY-=4);	
				m_iMoveChipsCount++;
				break;
			}
			case 4:
			{
				m_SprMoveChips[m_iMoveChipsCount].SetLoadInfo(g_pD3DDevice,TEXT("Round\\100.png")); 
				m_SprMoveChips[m_iMoveChipsCount].SetMovePath(startX,startY-=4,stopX,stopY-=4);	
				m_iMoveChipsCount++;
				break;	
			}
			case 5:
			{
				m_SprMoveChips[m_iMoveChipsCount].SetLoadInfo(g_pD3DDevice,TEXT("Round\\500.png")); 
				m_SprMoveChips[m_iMoveChipsCount].SetMovePath(startX,startY-=4,stopX,stopY-=4);	
				m_iMoveChipsCount++;
				break;	
			}
			case 6:
			{	
				m_SprMoveChips[m_iMoveChipsCount].SetLoadInfo(g_pD3DDevice,TEXT("Round\\1k.png")); 
				m_SprMoveChips[m_iMoveChipsCount].SetMovePath(startX,startY-=4,stopX,stopY-=4);	
				m_iMoveChipsCount++;
				break;	
			}
			case 7:
			{	
				m_SprMoveChips[m_iMoveChipsCount].SetLoadInfo(g_pD3DDevice,TEXT("Round\\5k.png")); 
				m_SprMoveChips[m_iMoveChipsCount].SetMovePath(startX,startY-=4,stopX,stopY-=4);	
				m_iMoveChipsCount++;
				break;
			}
			case 8:
			{
				m_SprMoveChips[m_iMoveChipsCount].SetLoadInfo(g_pD3DDevice,TEXT("Round\\10k.png")); 
				m_SprMoveChips[m_iMoveChipsCount].SetMovePath(startX,startY-=4,stopX,stopY-=4);	
				m_iMoveChipsCount++;
				break;
			}
			default:
				break;
			}
		}

		//������Ŀ
		lScoreCount-=lCellCount*lScoreIndex[cbScoreIndex];
	}

	return;
}