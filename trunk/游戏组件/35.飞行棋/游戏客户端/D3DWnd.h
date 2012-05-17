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
	HANDLE 					m_hRenderThread;	//�߳̾��
public:


protected:

	
	cGraphics				m_Graphics;							// Graphics object
	LPDIRECT3DDEVICE9		m_pDevice;							//ָ��D3D�����豸�Ľӿ�ָ��
	DWORD					TFPS, FPS;	
	cFont					m_font2DFont;
	//cFont					m_nameFont;

	BYTE					m_GameStatus;						//��Ϸ״̬��0��Ϸδ��ʼ��1�Լ������ӣ�2�Լ�ѡ��ɻ���3�ȴ������û������Ӻ�ѡ��ɻ���4��Ϸ����
	bool					m_AnimatedDiceStatus;
	bool					m_AnimatedChessStatus;
	//BYTE					m_GameStage;						//��Ϸ�׶Σ��ɷ������������ͻ��˱������ܵ�״̬���������Ϳͻ��˱���һ�µ�״̬��
	BYTE					m_DiceRandNum;

	DiceCastAnimation		m_DiceCastAnimate;					//Ͷ��ɸ������
	long					m_lLimitTimeCount;					//��Ϸ��ʱ
	BYTE					m_aAutomatism[GAME_PLAYER];			//�����Զ��й�
	WORD					m_wFaceID[GAME_PLAYER];				//ͷ������
	TCHAR					m_szName[GAME_PLAYER][NAME_LEN];					//�û�����
	bool					m_bIsLookonMode;					//�Ƿ������Ϸ

	cTexture				m_StartTexture;
	cTexture				m_ReadyTexture;
	cTexture				m_DiceButton;	
	cTexture				m_BaseBright;
	cTexture				m_AutomatismButton;
	cTexture				m_AutomatismFlag;
	cTexture				m_UserHead;


	//ʵ����ֻ��ҪһЩ��������Ϳ�����
	//��Ϸ�в�����Ҫ��ģ�ͽ�����Ⱦ
	//��������
	CChessBoard				m_ChessBoard;				//��Ϸ����
	//HCURSOR					m_hHandCursor;


//public:
	//int						m_nTimeCounter;				//��ʱ��ʱ��

public:
	CD3DWnd();
	virtual ~CD3DWnd();	

	// �����̱��濽�����豸
	void blitToSurface(cSurface* srcSurface, const RECT *srcRect, const RECT *destRect) ;

	// �߶���ʱ��
	void StepTimer(void);

protected:

	DECLARE_MESSAGE_MAP()
public:

	// ����
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// ��Ⱦ����
	void Render(void);
	afx_msg void OnPaint();
	// �ͷ���Դ
	void Cleanup(void);
	// �ͷ���Դ
	void Cleanup_(void);
	// ����
	afx_msg void OnDestroy();

	// �����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	//��Ⱦ
	LRESULT CD3DWnd::OnRender(WPARAM wParam, LPARAM lParam);
	//��ʼ��D3D����
	void InitD3D(void);
	//��ʾFPS
	void ShowFPS();
	//���������ƶ��Ķ�����ʾ���ڶ���������ʱ�����ӵ�����λ�òż��������
	void MakeChessMove(DWORD ElapseTime);
	void MakeDiceAnimate(DWORD ElapseTime);
	void InitChessBoard();
	//����豸
	void CleanUpD3D(void);

	//���ù��
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	void TestTileXYNum(BYTE bControlFlag);
	void TestChessNumber(BYTE bControlFlag);
	void TestChessMove();
	void TestActiveChess();
	void TestPlayStatus();
//void OnTimer(UINT nIDEvent);
};


