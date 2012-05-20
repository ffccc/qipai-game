#pragma once

#include "StdAfx.h"
#pragma comment(lib, "d3d9.lib") 
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxerr9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dsound.lib")
#include "DxGraphics.h"
#include "ChessBoard.h"
#include "DiceCastAnimation.h"



#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }


//#define GAME_STAGE_NOT_VALID		0
//#define GAME_STAGE_DICE				1
//#define GAME_STAGE_CHOOSE			2
/////////////////////////////////////////////////////////////////////


// CD3DWnd
class CD3DWnd : public CWnd
{
	friend class CGameClientDlg;
	DECLARE_DYNAMIC(CD3DWnd)

	//bool					g_bDeviceLost;
	HANDLE 					m_hRenderThread;	//线程句柄
public:


protected:

	
	cGraphics				m_Graphics;							// Graphics object
	LPDIRECT3DDEVICE9		m_pDevice;							//指向D3D驱动设备的接口指针
	DWORD					TFPS, FPS;	
	cFont					m_font2DFont;
	//cFont					m_nameFont;

	BYTE					m_GameStatus;						//游戏状态，0游戏未开始，1自己抛骰子，2自己选择飞机，3等待其他用户抛骰子和选择飞机，4游戏结束
	bool					m_AnimatedDiceStatus;
	bool					m_AnimatedChessStatus;
	//BYTE					m_GameStage;						//游戏阶段（由服务器决定，客户端被动接受的状态，服务器和客户端保持一致的状态）
	BYTE					m_DiceRandNum;

	DiceCastAnimation		m_DiceCastAnimate;					//投掷筛子容器
	long					m_lLimitTimeCount;					//游戏限时
	BYTE					m_aAutomatism[GAME_PLAYER];			//电脑自动托管
	WORD					m_wFaceID[GAME_PLAYER];				//头像索引
	TCHAR					m_szName[GAME_PLAYER][NAME_LEN];					//用户名字
	bool					m_bIsLookonMode;					//是否可以游戏

	cTexture				m_StartTexture;
	cTexture				m_ReadyTexture;
	cTexture				m_DiceButton;	
	cTexture				m_BaseBright;
	cTexture				m_AutomatismButton;
	cTexture				m_AutomatismFlag;
	cTexture				m_UserHead;


	//实际上只需要一些离屏表面就可以了
	//游戏中并不需要对模型进行渲染
	//背景表面
	CChessBoard				m_ChessBoard;				//游戏棋盘
	//HCURSOR					m_hHandCursor;


//public:
	//int						m_nTimeCounter;				//定时器时间

public:
	CD3DWnd();
	virtual ~CD3DWnd();	

	// 将棋盘表面拷贝到设备
	void blitToSurface(cSurface* srcSurface, const RECT *srcRect, const RECT *destRect) ;

	// 走动定时器
	void StepTimer(void);

protected:

	DECLARE_MESSAGE_MAP()
public:

	// 创建
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// 渲染场景
	void Render(void);
	afx_msg void OnPaint();
	// 释放资源
	void Cleanup(void);
	// 释放资源
	void Cleanup_(void);
	// 销毁
	afx_msg void OnDestroy();

	// 鼠标消息
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	//渲染
	LRESULT CD3DWnd::OnRender(WPARAM wParam, LPARAM lParam);
	//初始化D3D窗口
	void InitD3D(void);
	//显示FPS
	void ShowFPS();
	//用作棋子移动的动画显示，在动画结束的时候棋子的最终位置才计算出来。
	void MakeChessMove(DWORD ElapseTime);
	void MakeDiceAnimate(DWORD ElapseTime);
	void InitChessBoard();
	//清除设备
	void CleanUpD3D(void);

	//设置光标
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	void TestTileXYNum(BYTE bControlFlag);
	void TestChessNumber(BYTE bControlFlag);
	void TestChessMove();
	void TestActiveChess();
	void TestPlayStatus();
//void OnTimer(UINT nIDEvent);
};


