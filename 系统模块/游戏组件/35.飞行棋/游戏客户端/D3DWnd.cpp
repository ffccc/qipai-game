// D3DWnd.cpp : 实现文件

#include "stdafx.h"
#include "D3DWnd.h"
#include "GameClientDlg.h"

#define			RENDER_THREAD			101			
///////////////////////////////////////////////

static bool keyi;

static DWORD dwCurTime;
static DWORD dwLastRenderTime = GetTickCount();


// CD3DWnd


IMPLEMENT_DYNAMIC(CD3DWnd, CWnd)
CD3DWnd::CD3DWnd()
{
	TFPS=0;
	FPS=0;
	//m_hButtonCursor = AfxGetApp()->LoadCursor(IDC_MY_HAND);
	//::SetCursor(m_hButtonCursor);
	m_GameStatus = GAME_NOT_ACTIVE;
	memset(m_aAutomatism, NOT_AUTOMATISM, sizeof(m_aAutomatism));
	m_AnimatedDiceStatus = false;
	m_AnimatedChessStatus = false;
	m_bIsLookonMode = false;
	m_lLimitTimeCount = LIMIT_TIME;
	
	
}

CD3DWnd::~CD3DWnd()
{


}


BEGIN_MESSAGE_MAP(CD3DWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//ON_WM_TIMER()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(RENDER_THREAD, OnRender)

END_MESSAGE_MAP()

void ThreadRender(LPTHREAD_START_ROUTINE pThreadData)
{
	//效验参数
	ASSERT(pThreadData!=NULL);
	CD3DWnd * pDirect3D=(CD3DWnd *)pThreadData;
	while(keyi)
	{
		pDirect3D->SendMessage(RENDER_THREAD,0,0);
		Sleep(1);
	}
}


// 初始化D3D
void CD3DWnd::InitD3D(void)
{

	m_Graphics.Init(m_hWnd);

	// Set camera and construct frustum
	cCamera Camera;
	Camera.Point(0.0f, 0.0f,-25.0f, 0.0f, 0.0f, 0.0f);
	m_Graphics.SetCamera(&Camera);
	m_Graphics.EnableLighting(FALSE);
	m_pDevice = m_Graphics.GetDeviceCOM();
	m_Graphics.SetPerspective(D3DX_PI/4, 1.0f, 1.0f, 100.0f);
	m_ChessBoard.InitBackGround(&m_Graphics);
	//创建2维文字字体
	m_font2DFont.Create(&m_Graphics, TEXT("宋体"), 14, 0, FALSE);
	//m_nameFont.Create(&m_Graphics, TEXT("宋体"), 28, 0, TRUE);
	m_DiceCastAnimate.InitDiceCast(&m_Graphics);
	m_StartTexture.LoadTextureFromResource(&m_Graphics, IDB_START, 58, 31, 0xFF0000FF);
	m_ReadyTexture.LoadTextureFromResource(&m_Graphics, IDB_READY, 90, 34, 0XFFFF00FF);
	m_DiceButton.LoadTextureFromResource(&m_Graphics, IDB_DICEBUTTON, 47, 33, 0xFF0000FF);
	m_BaseBright.LoadTextureFromResource(&m_Graphics, IDB_HOME_BRIGHT, 564, 564, 0xFFFF00FF);
	m_AutomatismButton.LoadTextureFromResource(&m_Graphics, IDB_AUTOMATISM_BUTTON, 47, 33, 0xFF0000FF);
	m_AutomatismFlag.LoadTextureFromResource(&m_Graphics, IDB_COMPUTER, 66, 56, 0xFFFF00FF);
	m_UserHead.LoadTextureFromResource(&m_Graphics, IDB_HEADER, 1920, 32, 0xFFFF00FF);
}

void CD3DWnd::CleanUpD3D(void){
	m_ChessBoard.CleanUpTexture();
	m_font2DFont.Free();
	m_DiceCastAnimate.CleanUp();
	m_StartTexture.Free();
	m_ReadyTexture.Free();
	m_DiceButton.Free();
	m_BaseBright.Free();
	m_AutomatismButton.Free();
	m_Graphics.Shutdown();

}

int CD3DWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	InitD3D();
	//m_ChessBoard.SetCurMeChairID(0);
	//memset(m_ChessBoard.m_bPlayStatus, US_PLAY, sizeof(m_ChessBoard.m_bPlayStatus));
	//InitChessBoard();


	//开启渲染线程
	UINT uThreadID=0;
	keyi = TRUE;
	m_hRenderThread=(HANDLE)::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadRender,this,0,0);
	//SetTimer(0, 30,NULL);
	return 0;
}


/*void CD3DWnd::OnTimer(UINT nIDEvent)
{
	OnRender(0,0);
	CWnd::OnTimer(nIDEvent);
}*/


LRESULT CD3DWnd::OnRender(WPARAM wParam, LPARAM lParam)
{


	static DWORD tempCurTime;
	tempCurTime = GetTickCount();
	if(tempCurTime - dwCurTime> 30){
	//	RetailOutput("\ntempCurTime = %d, dwLastRenderTime = %d, tempCurTime - dwLastRenderTime = %d", tempCurTime, dwLastRenderTime,tempCurTime - dwLastRenderTime );
		dwLastRenderTime = dwCurTime;
		dwCurTime = tempCurTime; 
		Render();
	}
	return true;
}



void CD3DWnd::Render(void)
{ 
	static DWORD dwLastFPSTime = GetTickCount(), dwElapseTime;
	static HRESULT hr;
	static const tagUserData * pUserData;
	static TCHAR aTimeBuff[16];

	if( FAILED( hr = m_pDevice->TestCooperativeLevel()))
	{
		//Sleep( 100 ); 
		if( hr == D3DERR_DEVICELOST )
			return;
		if( hr == D3DERR_DEVICENOTRESET )
		{
			m_ChessBoard.CleanUpTexture();
			CleanUpD3D();
			InitD3D();
			m_ChessBoard.InitChessBoard(&m_Graphics);
		}
		
	}

	TFPS++;
	if ( dwCurTime - dwLastFPSTime>=1000)
	{
		//RetailOutput("\ndwCurTime  = %d, dwLastFPSTime = %d, dwCurTime- dwLastFPSTime = %d", dwCurTime, dwLastFPSTime,dwCurTime- dwLastFPSTime);
		dwLastFPSTime = dwCurTime;
		FPS=TFPS;
		TFPS=0;
	}

	
	//先清除后备缓冲区
	//m_pDevice->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0f,0);

	m_pDevice->BeginScene();


	//m_BackBuffer.SetSurface(&m_Graphics, m_Graphics.getBackBuffer());
//	if(!m_BackBuffer.CopySurface(&m_ChessBoard.m_ChessBoardSurface, CRect(0,0,728, 699),CRect(0,0,728, 699)))
//	{
		//MessageBox(TEXT("离屏表面拷贝失败！"),"",MB_OK);return;
//	}

	m_Graphics.BeginSprite(D3DXSPRITE_ALPHABLEND);
	m_ChessBoard.m_ChessBoardTexture.Blit(&CRect(0, 0, 750, 720), 0, 0);

	dwElapseTime = dwCurTime - dwLastRenderTime;
	MakeChessMove(dwElapseTime);
	//Debuglog.LogOutput("CurcbTileIndex=%d", m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurPlayerID][m_ChessBoard.m_CurChessID].cbTileIndex);
	//Debuglog.write("sdfsd");

	switch(m_GameStatus)
	{
	case GAME_WAIT_START:
		{
		m_StartTexture.Blit(&CRect(0, 0, 58, 31), 620, 620);
		}
	case GAME_READY_TO_START:
		{
		for(int i =0; i < GAME_PLAYER; i++){
			if(m_ChessBoard.m_bPlayStatus[i] == US_READY){
				switch(m_ChessBoard.m_bViewSeatID[i]){
					case 0:
						m_ReadyTexture.Blit(&CRect(0, 0, 90, 34), 600, 600);
						break;
					case 1:
						m_ReadyTexture.Blit(&CRect(0, 0, 90, 34), 100, 600);
						break;
					case 2:
						m_ReadyTexture.Blit(&CRect(0, 0, 90, 34), 100, 20);
						break;
					case 3:
						m_ReadyTexture.Blit(&CRect(0, 0, 90, 34), 600, 20);
						break;
				}
				
			}
		}
		break;
		}
	case GAME_WAIT_DICE:
		{
			if(!m_AnimatedChessStatus && !m_AnimatedDiceStatus) m_DiceButton.Blit(&CRect(0,0,47,33), 600, 600);
		}
	case GAME_WAIT_CHOOSE:{}
	case GAME_WAIT_OTHER:
		{
			if(!m_AnimatedDiceStatus && !m_AnimatedChessStatus) {
				m_lLimitTimeCount -= dwElapseTime;
				BYTE cbWidthIndex =(m_ChessBoard.m_bViewSeatID[m_ChessBoard.m_CurPlayerID] +1) %GAME_PLAYER;
				static SHORT shDownCount = 33;
				if(shDownCount >= 0){
					m_BaseBright.Blit(&CRect(cbWidthIndex * 141,m_ChessBoard.m_CurPlayerID * 141, cbWidthIndex * 141 +141, \
						m_ChessBoard.m_CurPlayerID * 141 +141 ), m_ChessBoard.m_BasePosition[m_ChessBoard.m_CurPlayerID].x, m_ChessBoard.m_BasePosition[m_ChessBoard.m_CurPlayerID].y);
					shDownCount --;
				} else {
					shDownCount --;
					if(shDownCount < -33)shDownCount =33;
				}
			}
		}
	case GAME_DICE_ANIMATED:{}
	case GAME_CHESS_ANIMATED:{
		if(!(m_GameStatus == GAME_WAIT_DICE && (m_AnimatedChessStatus || m_AnimatedDiceStatus))){
			wsprintf(aTimeBuff, "%ld", m_lLimitTimeCount >0?m_lLimitTimeCount /1000:-1);
			if(m_lLimitTimeCount <=0 && m_ChessBoard.m_CurPlayerID == m_ChessBoard.m_CurMeChairID) AfxGetMainWnd()->SendMessage(IDM_TIMEOUT,0,0);

			switch(m_ChessBoard.m_bViewSeatID[m_ChessBoard.m_CurPlayerID]){
			case 0:
				m_font2DFont.Print(aTimeBuff, &CRect(620, 520 +32 +14, 620 +80, 520 +32 +14 +14), aSeatColor[m_ChessBoard.m_CurPlayerID], DT_CENTER);
			break;
			case 1:
				m_font2DFont.Print(aTimeBuff, &CRect(15, 520 +32 +14, 15 +80, 520 +32 +14 +14), aSeatColor[m_ChessBoard.m_CurPlayerID], DT_CENTER);
			break;
			case 2:
				m_font2DFont.Print(aTimeBuff, &CRect(15, 110 +32 +14, 15 +80, 110 +32 +14 +14), aSeatColor[m_ChessBoard.m_CurPlayerID], DT_CENTER);
			break;
			case 3:
				m_font2DFont.Print(aTimeBuff, &CRect(620, 110 +32 +14, 620 +80, 110 +32 +14 +14), aSeatColor[m_ChessBoard.m_CurPlayerID], DT_CENTER);
			break;
			}
		}
		m_AutomatismButton.Blit(&CRect(0, 0, 47, 33), 650, 600);
		for(int i =0; i < GAME_PLAYER; i++){
			if(m_aAutomatism[i] == ALL_AI_AUTOMATISM){
				switch(m_ChessBoard.m_bViewSeatID[i]){
				case 0:
					m_AutomatismFlag.Blit(&CRect(0, 0, 66, 56), 540, 540);
				break;
				case 1:
					m_AutomatismFlag.Blit(&CRect(0, 0, 66, 56), 120, 540);
				break;
				case 2:
					m_AutomatismFlag.Blit(&CRect(0, 0, 66, 56), 120, 60);
				break;
				case 3:
					m_AutomatismFlag.Blit(&CRect(0, 0, 66, 56), 540, 60);
				break;
					}
			}
		}
		MakeDiceAnimate(dwElapseTime);
		break;
	    }
	}

	m_ChessBoard.BlitChess();
	for(int i= 0; i< GAME_PLAYER; i++ ){
		if(m_ChessBoard.m_bPlayStatus[i] == US_FREE) continue;
		switch(m_ChessBoard.m_bViewSeatID[i]){
			case 0:
				m_UserHead.Blit(&CRect(m_wFaceID[i], 0, m_wFaceID[i] + 32, 32), 660 -16, 520);
				m_font2DFont.Print(m_szName[i], &CRect(620, 520 +32, 620 +80, 520 +32 +14), aSeatColor[i], DT_CENTER);
				break;
			case 1:
				m_UserHead.Blit(&CRect(m_wFaceID[i], 0, m_wFaceID[i] + 32, 32), 55 -16, 520);
				m_font2DFont.Print(m_szName[i], &CRect(15, 520 +32, 15+80, 520 +32 +14), aSeatColor[i], DT_CENTER);
				break;
			case 2:
				m_UserHead.Blit(&CRect(m_wFaceID[i], 0, m_wFaceID[i] + 32, 32), 55 -16, 110);
				m_font2DFont.Print(m_szName[i], &CRect(15, 110 +32, 15+80, 110 +32 +14), aSeatColor[i], DT_CENTER);
				break;
			case 3:
				m_UserHead.Blit(&CRect(m_wFaceID[i], 0, m_wFaceID[i] + 32, 32), 660 -16, 110);
				m_font2DFont.Print(m_szName[i], &CRect(620, 110 +32, 620 +80, 110 +32 +14), aSeatColor[i], DT_CENTER);
				break;
		}
	}
	//TestTileXYNum(5);
	//TestChessNumber(1);
	//刷新频率
	//ShowFPS();
	//TestChessMove();
	//TestActiveChess();
	//TestPlayStatus();
	m_Graphics.EndSprite();

	m_pDevice->EndScene();

	//--------------------------------------
	m_pDevice->Present(NULL,NULL,NULL,NULL);

//Debuglog.last();
}

void CD3DWnd::OnPaint()
{
	CPaintDC dc(this);
	if(keyi)Render();

}

// 释放资源
void CD3DWnd::Cleanup(void)
{
	keyi = FALSE;
	//ASSERT(m_hRenderThread!=NULL);
	if (m_hRenderThread!=NULL)
	{
		CloseHandle(m_hRenderThread);
	}


	Cleanup_();
	
	m_Graphics.Shutdown();
}

// 释放资源
void CD3DWnd::Cleanup_(void)
{
	m_font2DFont.Free();
	m_UserHead.Free();
//	Debuglog.last();
	
}

void CD3DWnd::OnDestroy()
{
	//::SendMessage(AfxGetMainWnd()->m_hWnd,IDM_D3D_LEAVE,0,0);

	CWnd::OnDestroy();

	Cleanup();

}



void CD3DWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_bIsLookonMode)return;
	//srand(GetTickCount());
	//RetailOutput("\nOnLButtonDown");
	if( m_GameStatus != GAME_NOT_ACTIVE && m_GameStatus != GAME_WAIT_START && m_GameStatus!=GAME_READY_TO_START && point.x >= 650 && point.x <= 650+47 && point.y >= 600 && point.y <= 600+33){
		if(m_aAutomatism[m_ChessBoard.m_CurMeChairID] ==HALF_AI_AUTOMATISM) {
			m_aAutomatism[m_ChessBoard.m_CurMeChairID] = ALL_AI_AUTOMATISM;
			AfxGetMainWnd()->SendMessage(IDM_AUTOMATISM, 0, m_aAutomatism[m_ChessBoard.m_CurMeChairID]);
			return;
		} else m_aAutomatism[m_ChessBoard.m_CurMeChairID] = m_aAutomatism[m_ChessBoard.m_CurMeChairID] ==ALL_AI_AUTOMATISM?NOT_AUTOMATISM:ALL_AI_AUTOMATISM;
		AfxGetMainWnd()->SendMessage(IDM_AUTOMATISM, 0, m_aAutomatism[m_ChessBoard.m_CurMeChairID]);
		if(m_aAutomatism[m_ChessBoard.m_CurMeChairID] == NOT_AUTOMATISM)return;
		switch(m_GameStatus){
			case GAME_WAIT_DICE:{
				if(!m_AnimatedDiceStatus) OnLButtonDown(0, CPoint(610, 610));
				break;
			}
			case GAME_WAIT_CHOOSE:{
				if(!m_AnimatedChessStatus) {
					if(m_DiceRandNum == MAX_RANDNUM){
						BYTE i;
						for(/*BYTE*/i = 0; i< GAME_PLAYER; i++){
							if(m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][i].cbTileIndex > MAX_ROADWAY && m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][i].cbTileIndex<= MAX_ROADWAY_BASE && m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][i].cbChessStatus ==CHESS_NOT_FINISHED){
								OnLButtonDown(0, CPoint((int)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][i].fXPos+PLANE_W/2, (int)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][i].fYPos+PLANE_H/2));
								break;
							}
						}
						if(i == GAME_PLAYER) {
							if(m_ChessBoard.m_CurActiveChess.GetCount()){
								OnLButtonDown(0, CPoint((int)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][m_ChessBoard.m_CurActiveChess.GetAt(0)].fXPos+PLANE_W/2, (int)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][m_ChessBoard.m_CurActiveChess.GetAt(0)].fYPos+PLANE_H/2));
							} else {
								for(BYTE i = 0; i< GAME_PLAYER; i++){
									if(m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][i].cbChessStatus ==CHESS_NOT_FINISHED){
										OnLButtonDown(0, CPoint((int)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][i].fXPos+PLANE_W/2, (int)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][i].fYPos+PLANE_H/2));
										//Debuglog.LogOutput("\nCpoint.x = %d, Cpoint.y = %d", (int)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][i].fXPos+PLANE_W/2, (int)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][i].fYPos+PLANE_H/2);
										break;
									}
								}
							}
						} 
					} else {
						if(m_ChessBoard.m_CurActiveChess.GetCount()){
							OnLButtonDown(0, CPoint((int)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][m_ChessBoard.m_CurActiveChess.GetAt(0)].fXPos+PLANE_W/2, (int)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][m_ChessBoard.m_CurActiveChess.GetAt(0)].fYPos+PLANE_H/2));
						} else {
							OnLButtonDown(0, CPoint((int)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][0].fXPos+PLANE_W/2, (int)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][0].fYPos+PLANE_H/2));
						}
					}
				}
				break;
			}
		}
		return;
	}

	switch(m_GameStatus){
		case GAME_WAIT_START:
			{
				if( point.x >= 620 && point.x <= 620+58 && point.y >= 620 && point.y <= 620+31){
					AfxGetMainWnd()->SendMessage(IDM_START,0,0);
					m_GameStatus = GAME_READY_TO_START;
					m_ChessBoard.m_bPlayStatus[m_ChessBoard.m_CurMeChairID] = US_READY;
				}
				break;
			}
		case GAME_WAIT_DICE:
			{
				if(m_AnimatedChessStatus || m_AnimatedDiceStatus) break;
				//Debuglog.LogOutput("\nOnLButtonDown GAME_WAIT_DICE");
				if(point.x >= 600 && point.x <= 600+58 && point.y >= 600 && point.y <= 600+31){
					AfxGetMainWnd()->SendMessage(IDM_DICE_CAST,0,0);
					m_GameStatus = GAME_DICE_ANIMATED;
					m_DiceCastAnimate.Reset(m_ChessBoard.m_bViewSeatID[m_ChessBoard.m_CurPlayerID] == 1||m_ChessBoard.m_bViewSeatID[m_ChessBoard.m_CurPlayerID] ==2?DICE_LEFT_MOVE:DICE_RIGHT_MOVE);
					//if(m_aAutomatism[m_ChessBoard.m_CurMeChairID] == ALL_AI_AUTOMATISM){
						m_DiceCastAnimate.m_cbStatus = NO_ANIMATION_WAIT;
				//	} else {
						//m_DiceCastAnimate.m_cbStatus = FIRST_STAGE;
				//	}
					m_AnimatedDiceStatus = true;
				}
				break;
			} 
		case  GAME_WAIT_CHOOSE:
			{
				//Debuglog.LogOutput("\nOnLButtonDown GAME_WAIT_CHOOSE, cpoint.x = %d, cpoint.y = %d", point.x, point.y);
				tagClientChessItem *pClientChessItem;
				if(m_DiceRandNum == MAX_RANDNUM){

					if(m_ChessBoard.m_CurActiveChess.GetCount()){
						for(BYTE i=0; i<m_ChessBoard.m_CurActiveChess.GetCount(); i++){
							pClientChessItem = &m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][m_ChessBoard.m_CurActiveChess.GetAt(i)];
							if((USHORT)point.x >= pClientChessItem->fXPos && (USHORT)point.y >= pClientChessItem->fYPos &&
								(USHORT)point.x <= pClientChessItem->fXPos + PLANE_W && (USHORT)point.y <= pClientChessItem->fYPos + PLANE_H){
									AfxGetMainWnd()->SendMessage(IDM_CHESS_CHOOSE, 0, pClientChessItem->cbChessID);
									m_ChessBoard.SwitchCurrentChess(pClientChessItem->cbChessID);
									m_ChessBoard.ChessRouteGen(m_DiceRandNum );
									m_AnimatedChessStatus = true;
									return;
								}
						}
					} 
					for(BYTE i=0; i<PLAYER_CHESS_COUNT; i++){
						pClientChessItem = &m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][i];
						if((USHORT)point.x >= pClientChessItem->fXPos && (USHORT)point.y >= pClientChessItem->fYPos &&
							(USHORT)point.x <= pClientChessItem->fXPos + PLANE_W && (USHORT)point.y <= pClientChessItem->fYPos + PLANE_H && pClientChessItem->cbChessStatus ==CHESS_NOT_FINISHED){
								AfxGetMainWnd()->SendMessage(IDM_CHESS_CHOOSE, 0, pClientChessItem->cbChessID);
								m_ChessBoard.SwitchCurrentChess(pClientChessItem->cbChessID);
								m_ChessBoard.ChessRouteGen(m_DiceRandNum);
								m_AnimatedChessStatus = true;
								return;
							}
					}
					

				} else {
					if(m_ChessBoard.m_CurActiveChess.GetCount()){
						for(BYTE i=0; i<m_ChessBoard.m_CurActiveChess.GetCount(); i++){
							pClientChessItem = &m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][m_ChessBoard.m_CurActiveChess.GetAt(i)];
							if((USHORT)point.x >= pClientChessItem->fXPos && (USHORT)point.y >= pClientChessItem->fYPos &&
								(USHORT)point.x <= pClientChessItem->fXPos + PLANE_W && (USHORT)point.y <= pClientChessItem->fYPos + PLANE_H){
									AfxGetMainWnd()->SendMessage(IDM_CHESS_CHOOSE, 0, pClientChessItem->cbChessID);
									m_ChessBoard.SwitchCurrentChess(pClientChessItem->cbChessID);
									m_ChessBoard.ChessRouteGen(m_DiceRandNum );
									break;
								}
						}
					} else {
						AfxGetMainWnd()->SendMessage(IDM_CHESS_CHOOSE, 0, 0);
					}
					m_AnimatedChessStatus = true;
				}
				break;
			}
	}

}


// 走动定时器
void CD3DWnd::StepTimer(void)
{	



}

void CD3DWnd::ShowFPS()
{
	//CRect rect(10,0,699,60);
	static char buffer[512];
	//m_font2DFont.PrintA(buffer, &rect, 0, DT_CALCRECT);
	sprintf(buffer,"FPS: %ld, GameStatus=%d, CurPlayerID=%d, CurChessID=%d, cbAutomatism=%d, AnimatedDice=%d, AnimatedChess=%d",FPS, m_GameStatus, m_ChessBoard.m_CurPlayerID, m_ChessBoard.m_CurChessID, m_aAutomatism[m_ChessBoard.m_CurMeChairID], m_AnimatedDiceStatus, m_AnimatedChessStatus);
	m_font2DFont.Print(buffer, &CRect(0,0,728,20), D3DCOLOR_XRGB(255,0,0), DT_LEFT);

}

void CD3DWnd::MakeChessMove(DWORD ElapseTime){
	if(m_AnimatedChessStatus){
		m_ChessBoard.MakeChessMove(ElapseTime);
		//主动
		if(	m_ChessBoard.m_CurMeChairID == m_ChessBoard.m_CurPlayerID){

			if(m_ChessBoard.m_ChessMove.GetCount() == 0) {
				//m_GameStatus =GAME_WAIT_DICE;
				m_AnimatedChessStatus = false;
				if(m_AnimatedDiceStatus) return;
				if(m_aAutomatism[m_ChessBoard.m_CurMeChairID] == ALL_AI_AUTOMATISM) OnLButtonDown(0, CPoint(610, 610));

			}

		} else {//被动
			if(m_ChessBoard.m_ChessMove.GetCount() == 0) {
				m_AnimatedChessStatus = false;
			}

		}
	}
}


void CD3DWnd::MakeDiceAnimate(DWORD ElapseTime){
	m_DiceCastAnimate.Draw(&m_Graphics, ElapseTime, m_ChessBoard.m_bViewSeatID[m_ChessBoard.m_CurPlayerID] == 1||m_ChessBoard.m_bViewSeatID[m_ChessBoard.m_CurPlayerID] ==2?DICE_LEFT_MOVE:DICE_RIGHT_MOVE);
	if(m_AnimatedDiceStatus){
		//主动
		if(	m_ChessBoard.m_CurMeChairID == m_ChessBoard.m_CurPlayerID){
			if(m_DiceCastAnimate.m_cbStatus == NO_ANIMATION) {
				if(m_GameStatus == GAME_DICE_ANIMATED )m_GameStatus = GAME_WAIT_CHOOSE;
				m_AnimatedDiceStatus = false;
				if(m_AnimatedChessStatus) return;
				if(m_aAutomatism[m_ChessBoard.m_CurMeChairID] != NOT_AUTOMATISM){
					if(m_GameStatus == GAME_WAIT_DICE){
						ASSERT(m_aAutomatism[m_ChessBoard.m_CurMeChairID] != HALF_AI_AUTOMATISM);
						OnLButtonDown(0, CPoint(610, 610));
						
						return;
					}
					if(m_DiceRandNum == MAX_RANDNUM){
						BYTE i;
						for(/*BYTE*/ i = 0; i< GAME_PLAYER; i++){
							if(m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][i].cbTileIndex > MAX_ROADWAY && m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][i].cbTileIndex<= MAX_ROADWAY_BASE && m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][i].cbChessStatus ==CHESS_NOT_FINISHED){
								OnLButtonDown(0, CPoint((int)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][i].fXPos+PLANE_W/2, (int)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][i].fYPos+PLANE_H/2));
								//Debuglog.LogOutput("\nCpoint.x = %d, Cpoint.y = %d", (int)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][i].fXPos+PLANE_W/2, (int)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][i].fYPos+PLANE_H/2);
								break;
							}
						}
						if(i == GAME_PLAYER) {
							if(m_ChessBoard.m_CurActiveChess.GetCount()){
								OnLButtonDown(0, CPoint((int)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][m_ChessBoard.m_CurActiveChess.GetAt(0)].fXPos+PLANE_W/2, (int)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][m_ChessBoard.m_CurActiveChess.GetAt(0)].fYPos+PLANE_H/2));
							} else {
								//Debuglog.LogOutput("\nCpoint.x = %d, Cpoint.y = %d", (int)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][0].fXPos+PLANE_W/2, (int)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][0].fYPos+PLANE_H/2);
								for(BYTE i = 0; i< GAME_PLAYER; i++){
									if(m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][i].cbChessStatus ==CHESS_NOT_FINISHED){
										OnLButtonDown(0, CPoint((int)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][i].fXPos+PLANE_W/2, (int)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][i].fYPos+PLANE_H/2));
										//Debuglog.LogOutput("\nCpoint.x = %d, Cpoint.y = %d", (int)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][i].fXPos+PLANE_W/2, (int)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][i].fYPos+PLANE_H/2);
										break;
									}
								}
							}
						}
					} else {
						if(m_ChessBoard.m_CurActiveChess.GetCount()){
							OnLButtonDown(0, CPoint((int)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][m_ChessBoard.m_CurActiveChess.GetAt(0)].fXPos+PLANE_W/2, (int)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][m_ChessBoard.m_CurActiveChess.GetAt(0)].fYPos+PLANE_H/2));
							//Debuglog.LogOutput("\nCpoint.x = %d, Cpoint.y = %d", (int)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][m_ChessBoard.m_CurActiveChess.GetAt(m_ChessBoard.m_CurActiveChess.GetUpperBound())].fXPos+PLANE_W/2, (int)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][m_ChessBoard.m_CurActiveChess.GetAt(m_ChessBoard.m_CurActiveChess.GetUpperBound())].fYPos+PLANE_H/2);
						} else {
							OnLButtonDown(0, CPoint((int)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][0].fXPos+PLANE_W/2, (int)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][0].fYPos+PLANE_H/2));
							//Debuglog.LogOutput("\nCpoint.x = %d, Cpoint.y = %d", (int)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][0].fXPos+PLANE_W/2, (int)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][0].fYPos+PLANE_H/2);
						}
						if(m_aAutomatism[m_ChessBoard.m_CurMeChairID] == HALF_AI_AUTOMATISM) m_aAutomatism[m_ChessBoard.m_CurMeChairID] = NOT_AUTOMATISM;
					}
				}
			}
			
		} else {//被动
			if(m_DiceCastAnimate.m_cbStatus == NO_ANIMATION) m_AnimatedDiceStatus = false;
		
		}

	}
}


void CD3DWnd::InitChessBoard(){
	//m_ChessBoard.InitChessBoard(&m_Graphics);
	m_ChessBoard.InitChessBoard(&m_Graphics);
	//m_ChessBoard.TestChess(3);
	m_ChessBoard.InitChess();
}



//设置光标
BOOL CD3DWnd::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message)
{
	//获取光标
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);


	if(MousePoint.x >= 650 && MousePoint.x <= 697 && MousePoint.y >= 600 && MousePoint.y <= 633 && m_GameStatus != GAME_NOT_ACTIVE && m_GameStatus != GAME_WAIT_START && m_GameStatus!=GAME_READY_TO_START){
		::SetCursor(AfxGetApp()->LoadCursor(IDC_HAND_CURSOR));
		return TRUE;
	} 
	switch(m_GameStatus)
	{
	case GAME_WAIT_START:
		if(MousePoint.x >= 620 && MousePoint.x <= 620+58 && MousePoint.y >= 620 && MousePoint.y <= 620+31){
			::SetCursor(AfxGetApp()->LoadCursor(IDC_HAND_CURSOR));
			return TRUE;
		}
		break;
	case GAME_WAIT_DICE:
		if(m_AnimatedChessStatus || m_AnimatedDiceStatus) break;
		if(MousePoint.x >= 600 && MousePoint.x <= 647 && MousePoint.y >= 600 && MousePoint.y <= 633){
			::SetCursor(AfxGetApp()->LoadCursor(IDC_HAND_CURSOR));
			return TRUE;
		} 
		break;
	case GAME_WAIT_CHOOSE:
		if(m_ChessBoard.m_CurMeChairID >= PLAYER_CHESS_COUNT) return TRUE;
		if(m_DiceRandNum == MAX_RANDNUM){
			USHORT usX, usY;
			for(BYTE i = 0; i< PLAYER_CHESS_COUNT; i++){
				usX = (USHORT)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][i].fXPos;
				usY = (USHORT)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][i].fYPos;

				if(MousePoint.x >= usX && MousePoint.x <= usX + PLANE_W && MousePoint.y >= usY && MousePoint.y <= usY +PLANE_H && m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][i].cbChessStatus == CHESS_NOT_FINISHED){
					::SetCursor(AfxGetApp()->LoadCursor(IDC_HAND_CURSOR));
					return TRUE;
				}
			}
		} else {
			USHORT  usX, usY;
			for(BYTE i = 0; i< m_ChessBoard.m_CurActiveChess.GetCount(); i++){
				usX = (USHORT)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][m_ChessBoard.m_CurActiveChess.GetAt(i)].fXPos;
				usY = (USHORT)m_ChessBoard.m_ClientChessItem[m_ChessBoard.m_CurMeChairID][m_ChessBoard.m_CurActiveChess.GetAt(i)].fYPos;

				if(MousePoint.x >= usX && MousePoint.x <= usX + PLANE_W && MousePoint.y >= usY && MousePoint.y <= usY +PLANE_H){
					::SetCursor(AfxGetApp()->LoadCursor(IDC_HAND_CURSOR));
					return TRUE;
				}
			}
		}

		break;
	/*case GAME_WAIT_OTHER:{}
	case GAME_DICE_ANIMATED:{}
	case GAME_CHESS_ANIMATED:
		{
			if(MousePoint.x >= 650 && MousePoint.x <= 697 && MousePoint.y >= 600 && MousePoint.y <= 633){
				::SetCursor(AfxGetApp()->LoadCursor(IDC_HAND_CURSOR));
				return TRUE;
			} 
			break;
		}*/
	}

	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	return TRUE;

}


void CD3DWnd::TestTileXYNum(BYTE bControlFlag){
	TCHAR temp[20];

	for(BYTE i=0; i< m_ChessBoard.m_ChessBoard.GetCount(); i++){
		switch(bControlFlag){
		case 1:
			wsprintf(temp, "%d\n%d", m_ChessBoard.m_ChessBoard.GetAt(i).usXPos + PLANE_TILE_XOFFSET, m_ChessBoard.m_ChessBoard.GetAt(i).usYPos + PLANE_TILE_YOFFSET);
			break;
		case 2:
			wsprintf(temp, "%d", i);
			break;
		case 3:
			wsprintf(temp, "%d,%d\n%d", m_ChessBoard.m_ChessBoard.GetAt(i).usXPos + PLANE_TILE_XOFFSET, m_ChessBoard.m_ChessBoard.GetAt(i).cbChessCount, m_ChessBoard.m_ChessBoard.GetAt(i).usYPos + PLANE_TILE_YOFFSET);
			break;
		case 4:
			wsprintf(temp, "%d\n%d", i, m_ChessBoard.m_ChessBoard.GetAt(i).cbChessCount);
			break;
		case 5:
			wsprintf(temp, "%d", m_ChessBoard.m_ChessBoard.GetAt(i).cbChessCount);
			break;
		}
		m_font2DFont.Print(temp,&CRect(m_ChessBoard.m_ChessBoard.GetAt(i).usXPos, m_ChessBoard.m_ChessBoard.GetAt(i).usYPos, m_ChessBoard.m_ChessBoard.GetAt(i).usXPos+TILE_W, m_ChessBoard.m_ChessBoard.GetAt(i).usYPos+TILE_H), 0xFFFFFFFF);
	}
	
	//m_ChessBoard.InitChessBoard(&m_Graphics);
}


void CD3DWnd::TestChessNumber(BYTE bControlFlag)
{
	TCHAR temp[20];
	for(int i= 0; i< GAME_PLAYER; i++) {
		if( m_ChessBoard.m_bPlayStatus[i] == US_FREE) continue;
		for(int j= 0; j < GAME_PLAYER; j++){
			switch(bControlFlag)
			{
			case 0:
				wsprintf(temp, "%d\n%d", m_ChessBoard.m_ClientChessItem[i][j].cbChessID, m_ChessBoard.m_ClientChessItem[i][j].cbChessStatus);
				break;
			case 1:
				wsprintf(temp, "%d", m_ChessBoard.m_ChessBoard.GetAt(m_ChessBoard.m_ClientChessItem[i][j].cbTileIndex).cbChessCount);
				break;
			}
			m_font2DFont.Print(temp,&CRect((int)(m_ChessBoard.m_ClientChessItem[i][j].fXPos +0.5), (int)(m_ChessBoard.m_ClientChessItem[i][j].fYPos +0.5), (int)(m_ChessBoard.m_ClientChessItem[i][j].fXPos +PLANE_W +0.5), (int)(m_ChessBoard.m_ClientChessItem[i][j].fYPos +PLANE_H +0.5)), 0xFFFFFF00);
		}
	}
}

void CD3DWnd::TestChessMove()
{
	TCHAR temp[40],tempAll[2048];
	ChessMove *pChessMove;
	for(BYTE i =0; i< m_ChessBoard.m_ChessMove.GetCount(); i++){
		pChessMove = &m_ChessBoard.m_ChessMove.GetAt(i);
		wsprintf(tempAll, "ChessMove: cbPlayerID=%d, cbChessID=%d, MoveStatus=%d, cbRefChessMove=%d, m_cbBeRefedCount=%d, CurFrame=%d\n", pChessMove->m_wPlayerID, pChessMove->m_cbChessID, pChessMove->m_MoveStatus, pChessMove->m_cbRefChessMove, pChessMove->m_cbBeRefedCount, pChessMove->m_CurFrame);
		for(BYTE j =0; j < pChessMove->m_ChessMoveRoute.GetCount(); j++){
			wsprintf(temp, "(%d,%d, %d, %d)", pChessMove->m_ChessMoveRoute.GetAt(j).usXPos, pChessMove->m_ChessMoveRoute.GetAt(j).usYPos,  pChessMove->m_ChessMoveRoute.GetAt(j).cbChessMoveFlag,  pChessMove->m_ChessMoveRoute.GetAt(j).cbRefChessMove);
			wsprintf(tempAll, "%s%s", tempAll, temp);
		}
		m_font2DFont.Print(tempAll, &CRect(10, 20 + i*30, 728, 50 + i*30), 0xFFFFFFFF);

	}
}

void CD3DWnd::TestActiveChess()
{
	TCHAR temp[4],tempAll[32];
	tempAll[0] = '\0';
	for(BYTE i =0; i< m_ChessBoard.m_CurActiveChess.GetCount(); i++){
		wsprintf(temp, "%d", m_ChessBoard.m_CurActiveChess.GetAt(i));
		if(tempAll[0])wsprintf(tempAll, "%s, %s", tempAll, temp);
		else wsprintf(tempAll, "CurActiveChess: %s", temp);
	}
	m_font2DFont.Print(tempAll, &CRect(10, 600, 728, 620), 0xFFFFFFFF);
}



void CD3DWnd::TestPlayStatus()
{
	TCHAR temp[4],tempAll[32];
	tempAll[0] = '\0';
	for(BYTE i =0; i< GAME_PLAYER; i++){
		wsprintf(temp, "%d", m_ChessBoard.m_bPlayStatus[i]);
		if(tempAll[0])wsprintf(tempAll, "%s, %s", tempAll, temp);
		else wsprintf(tempAll, "m_bPlayStatus: %s", temp);
	}
	m_font2DFont.Print(tempAll, &CRect(10, 600, 728, 620), 0xFFFFFFFF);
}
