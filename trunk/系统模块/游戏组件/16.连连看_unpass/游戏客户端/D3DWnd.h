#pragma once

#pragma comment(lib, "d3d9.lib") 
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxerr9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dsound.lib")

#include "d3d9.h"
#include <d3dx9.h>
#include "d3dx9math.h"
#include "sprite.h"
#include "D3DImagePro.h"
#include "pointsprite.h"
#include "rollfont.h"

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

/////////////////////////////////////////////////////////////////////
#define			IDM_D3D_LINK				WM_USER+150			//������Ϣ
#define			IDM_D3D_GUIDE				WM_USER+151			//������Ϣ
#define			IDM_D3D_BOMB				WM_USER+152			//������Ϣ
#define			IDM_D3D_REALIGN				WM_USER+153			//������Ϣ
#define			IDM_D3D_STOPIT				WM_USER+154			//������Ϣ
#define			IDM_D3D_CLOG				WM_USER+155			//�ϰ���Ϣ

#define			IDM_D3D_TIMER				WM_USER+160			//��ʱ��
#define			IDM_D3D_ALLMUSIC			WM_USER+161			//������Ϣ

//����
#define			IDM_MUSIC_START					WM_USER+171			//��ʼ����	
#define			IDM_MUSIC_LINK					WM_USER+172			//��ʼ����	
#define			IDM_MUSIC_BOMB					WM_USER+173			//��ʼ����	
#define			IDM_MUSIC_FLYSTAR					WM_USER+174		//��ʼ����	
#define			IDM_MUSIC_END						WM_USER+175		//��ʼ����	
#define			IDM_MUSIC_PROP					WM_USER+176			//��ʼ����
#define			IDM_MUSIC_SEL						WM_USER+177		//��ʼ����
#define			IDM_MUSIC_BG					WM_USER+178			//��������
#define			IDM_MUSIC_STP					WM_USER+179

/////////////////////////////////////////////////////////////////////
// ���ӱ�ըЧ��
class Blast{
public:
	CArrayTemplate<CPoint>	result;				//�����߽��
	CArrayTemplate<int>		m_LinePath;			//����������
	CSprite					m_A_Link;			//��ը����
	CSprite					m_A_Line;

	Blast & operator = (Blast & b)
	{	
		if(this == &b) return *this;
		for(int i=0; i<b.result.GetCount();++i)
			result.InsertAt(result.GetCount(),b.result.ElementAt(i));
		for(int i=0; i<b.m_LinePath.GetCount(); ++i)
			m_LinePath.InsertAt(m_LinePath.GetCount(), b.m_LinePath.ElementAt(i));
		m_A_Link = b.m_A_Link;
		m_A_Line = b.m_A_Line;
		return *this;
	}
	
};

// CD3DWnd

class CD3DWnd : public CWnd
{
	DECLARE_DYNAMIC(CD3DWnd)

	bool					g_bDeviceLost;
	HANDLE 					m_hRenderThread;	//�߳̾��
public:
	CPoint					m_nSelectFirst,m_ptLinkFirst;
	CPoint					m_nSelectSecond,m_ptLinkSecond;
	BYTE					m_Chess[GAME_PLAYER][CHESSCOUNT_H][CHESSCOUNT_W]; //��������
	BYTE					m_bPlayState[GAME_PLAYER];			//�û�״̬
	const tagUserData *		m_UserData[GAME_PLAYER];			//�û����ݽӿ�
	bool					m_bCanGame;							//�Ƿ������Ϸ
	bool					m_bGameOver;						//��Ϸ�Ƿ����
	bool					m_bGameLose[GAME_PLAYER];			//��Ϸ�Ƿ�ʧ��
	bool					m_ExerciseMode;						//�Ƿ���ϰģʽ
	BYTE					m_UserTaxis[GAME_PLAYER];			//����
	int						m_UserGrade[GAME_PLAYER];			//�û�����	

protected:
	CLogic					m_Logic;							//��Ϸ�߼���
	D3DPRESENT_PARAMETERS	d3dpp;								//��������
	LPDIRECT3D9				m_pD3D;								//ָ��D3D����ӿ�ָ��
	LPDIRECT3DDEVICE9		m_pDevice;							//ָ��D3D�����豸�Ľӿ�ָ��
	D3DDISPLAYMODE			D3DDisplayMode;						//��ʾģʽ
	
	CArrayTemplate<Blast,Blast &>	m_Blast;					//�洢����Ч������

	CArrayTemplate<CRollFont,CRollFont &>   m_RollText;					//������������

	//�б�Ҫ�����ɾ������ֲ�������ͼƬ

	CSprite					m_Sprite_ready;
	CSprite					m_Sprite_StopIt;
	CSprite					m_Sprite_Cry;
	CSprite					m_Sprite_SmallCry;
	CSprite					m_Sprite_Select;
	CSprite					m_Sprite_Number;
	CSprite					m_Sprite_ScoreFrame;
	CSprite					m_Sprite_Win;
	CSprite					m_Sprite_Lose;

	LPD3DXFONT				m_p2DFont;
	LPD3DXFONT				m_p2DFontBig;

	//ʵ����ֻ��ҪһЩ��������Ϳ�����
	//��Ϸ�в�����Ҫ��ģ�ͽ�����Ⱦ
	//��������
	IDirect3DSurface9*		m_pSurface;
	//���汳��
	IDirect3DSurface9*		m_pBkSurface;
	IDirect3DSurface9*		m_pLoseSurface;
	IDirect3DSurface9*		m_pChess_Surface;
	IDirect3DSurface9*		m_pChessFace_Surface;
	IDirect3DSurface9*		m_pChessFaceSmall_Surface;
	IDirect3DSurface9*		m_pTimerBK_Surface;
	IDirect3DSurface9*		m_pTimer_Surface;
	IDirect3DSurface9*		m_pChessFaceProp_Surface;

	//����
	LPDIRECT3DTEXTURE9		m_link_Texture[46];
	LPDIRECT3DTEXTURE9		m_line_Texture[10];
	LPDIRECT3DTEXTURE9		m_ready_Texture;
	LPDIRECT3DTEXTURE9		m_StopIt_Texture;
	LPDIRECT3DTEXTURE9		m_Cry_Texture;
	LPDIRECT3DTEXTURE9		m_SmallCry_Texture;
	LPDIRECT3DTEXTURE9		m_Select_Texture;
	LPDIRECT3DTEXTURE9		m_Number_Texture;
	LPDIRECT3DTEXTURE9		m_Lose_Texture;
	LPDIRECT3DTEXTURE9		m_Win_Texture;
	LPDIRECT3DTEXTURE9		m_ScoreFrame_Texture;

	CPoint					m_KeepFirst,	m_KeepEnd, m_KeepIPoint1, m_KeepIPoint2;
	DWORD					TFPS, FPS, fTimerFPS;	

	

	TCHAR					m_ChessCountString[64];					//��������
	bool					m_bHitState;							//����״̬
	int						m_nHitCount;							//���Ӽ���
	//���ֵ���
	bool					m_bStopIt;
public:
	int						m_nTimeCounter;							//��ʱ��ʱ��
	BYTE					m_nMouseState;							//���״̬

	CPointSprite			m_PointSprite_Star;						//��������
public:
	CD3DWnd();
	virtual ~CD3DWnd();	
	// ����Դ��������
	IDirect3DSurface9* getSurfaceFromResource(LONG RCID, int width, int height) ;
	// ͸������
	HRESULT LoadAlphaIntoSurface(LONG RCID,LPDIRECT3DSURFACE9 psurf);
	// ��ȡ���豸��ָ��
	IDirect3DSurface9* getBackBuffer(void) ;
	// ���������濽�����豸
	void blitToSurface(IDirect3DSurface9* srcSurface, const RECT *srcRect, const RECT *destRect) ;
	// ��һ�������ȥ����
	void getSurfaceFromSurface(IDirect3DSurface9* des,const RECT* srcRect, IDirect3DSurface9* src, const RECT* desRect,COLORREF colorkey);
	// ��ȥ����
	void ExpunctionChess(int UserID, CPoint first);
	// ����������ȥ����
	void CD3DWnd::sExpunctionChess(int UserID, CPoint first);
	// ����
	void UpdateBackSurface(void);
	// ������������
	void AccountChessCount(void);
	// �߶���ʱ��
	void StepItTimer(void);
	// ��ըЧ��
	void MakeBlast(void);
	// ����С����
	void UpdateChessSmall(int UserID);
	// ��Ϸʧ�ܱ��洦��
	void GameLoseSurface(IDirect3DSurface9* srcSurface, const RECT* srcRect);
	// ��ϰģʽ
	void Execise(void);

protected:
	DECLARE_MESSAGE_MAP()
public:
	// ��ʼ��D3D
	void InitD3D(void);
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
	// ��ģ
	void Geometry(void);
	// ��ʱ��
	afx_msg void OnTimer(UINT nIDEvent);
	// �����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	// ��ʾˢ��Ƶ��
	void ShowFPS(char *buffer);
	// ���þ���
	void SetM(void);
	// �������
	int RandNumber(int min, int max);
	// ��������
	bool SeystrokeProcess(int w, int h);
	// ��ʼ����
	void InitChess(void);
	// ���Ӵ���
	bool LinkProcess(CPoint first, CPoint second,WORD userID);
	// �����ߴ���
	void LineProcess(CArrayTemplate<CPoint>	*result, CArrayTemplate<int>		*m_LinePath);
	// ������Ϸ��Ϣ
	void DrawGameInfo(void);
	// ��ʾ����״̬
	void AddHitState(int nHitCount);

	////////////////////////////////////////////////////////////
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT OnRender(WPARAM wParam, LPARAM lParam);
	// ����
	// ������ʾ
	void AutoLink();
	// ����
	void Realign(void);
	// ����
	void StopIt(void);
	// �ϰ�
	void Clog(BYTE posX[12], BYTE posY[12], BYTE color[12], int UserID);
	// ָ����
	void Guide(void);
	// ����
	void UserFlee(int userid);
};


