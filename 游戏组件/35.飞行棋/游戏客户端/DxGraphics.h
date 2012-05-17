/**************************************************
WinMain.cpp
GameCore Component

Programming Role-Playing Games with DirectX
by Jim Adams (01 Jan 2002)
**************************************************/

#ifndef _CORE_GRAPHICS_H_
#define _CORE_GRAPHICS_H_
#include "d3dx9.h"

#define ReleaseCOM(x) if(x) { x->Release(); x = NULL; }

class cGraphics;

class cTexture;
class cMaterial;
class cLight;
class cFont;

class cWorldPosition;
class cCamera;




class cGraphics
{
  protected:
    HWND              m_hWnd;
    IDirect3D9       *m_pD3D;
    IDirect3DDevice9 *m_pD3DDevice;
    ID3DXSprite      *m_pSprite;
	IDirect3DSurface9* m_Surface;

    D3DDISPLAYMODE    m_d3ddm;


  public:
    cGraphics();
    ~cGraphics();

    IDirect3D9       *GetDirect3DCOM();
    IDirect3DDevice9 *GetDeviceCOM();
    ID3DXSprite      *GetSpriteCOM();

    BOOL Init(HWND hWnd);
    BOOL Shutdown();

    long GetNumDisplayModes(D3DFORMAT Forma);
    BOOL GetDisplayModeInfo(D3DFORMAT Format, long Num, D3DDISPLAYMODE *Mode);
	D3DFORMAT GetBackBufferFormat();

    BOOL Display();

    BOOL BeginScene();
    BOOL EndScene();

    BOOL BeginSprite(DWORD Flags);
    BOOL EndSprite();

    BOOL Clear(long Color = 0, float ZBuffer = 1.0f);
    BOOL ClearDisplay(long Color = 0);


    BOOL SetPerspective(float FOV=D3DX_PI / 4.0f, float Aspect=1.3333f, float Near=1.0f, float Far=10000.0f);
    
    BOOL SetWorldPosition(cWorldPosition *WorldPos);
    BOOL SetCamera(cCamera *Camera);
    BOOL SetLight(long Num, cLight *Light);
    BOOL SetMaterial(cMaterial *Material);
    BOOL SetTexture(short Num, cTexture *Texture);
    

    BOOL EnableLight(long Num, BOOL Enable = TRUE);
    BOOL EnableLighting(BOOL Enable = TRUE);
    BOOL EnableAlphaBlending(BOOL Enable = TRUE, DWORD Src = D3DBLEND_SRCALPHA, DWORD Dest = D3DBLEND_INVSRCALPHA);
    BOOL EnableAlphaTesting(BOOL Enable = TRUE);
	IDirect3DSurface9* getBackBuffer();
};


class cMaterial
{
  protected:
    D3DMATERIAL9  m_Material;

  public:
    cMaterial();

    D3DMATERIAL9 *GetMaterial();

    BOOL SetDiffuseColor(char Red, char Green, char Blue);
    BOOL GetDiffuseColor(char *Red, char *Green, char *Blue);

    BOOL SetAmbientColor(char Red, char Green, char Blue);
    BOOL GetAmbientColor(char *Red, char *Green, char *Blue);
    
    BOOL SetSpecularColor(char Red, char Green, char Blue);
    BOOL GetSpecularColor(char *Red, char *Green, char *Blue);
    
    BOOL SetEmissiveColor(char Red, char Green, char Blue);
    BOOL GetEmissiveColor(char *Red, char *Green, char *Blue);

    BOOL  SetPower(float Power);
    float GetPower(float Power);
};

class cTexture
{
  protected:
    cGraphics         *m_Graphics;
    IDirect3DTexture9 *m_Texture;

  public:
    cTexture();
    ~cTexture();

    IDirect3DTexture9 *GetTextureCOM();

	BOOL LoadTextureFromFile(cGraphics *Graphics,TCHAR *Filename, UINT Width, UINT Height,   D3DCOLOR ColorKey = D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,1.0f));
	BOOL LoadTextureFromResource(cGraphics *Graphics,  LONG RCID, UINT Width,  UINT Height, D3DCOLOR ColorKey = D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,1.0f), TCHAR* strRes = NULL );
	BOOL LoadRenderTextureFromResource(cGraphics *Graphics, LONG RCID, UINT Width,  UINT Height, D3DCOLOR ColorKey = D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,1.0f) );
	BOOL LoadDynamicTextureFromResource(cGraphics *Graphics, LONG RCID, UINT Width,  UINT Height, D3DCOLOR ColorKey = D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,1.0f) );
    BOOL Create(cGraphics *Graphics, IDirect3DTexture9 *Texture);
	BOOL BlackTexture(const RECT* srcRect);
    BOOL GetTextureFromTexture(const RECT* desRect, cTexture* srcTexture, const RECT* srcRect,COLORREF colorkey = 0x00);
	BOOL CreateRenderTexture(cGraphics *Graphics, UINT Width, UINT Height);
    BOOL Free();

    BOOL      IsLoaded();

    long      GetWidth();
    long      GetHeight();
    D3DFORMAT GetFormat();

    BOOL Blit(long DestX, long DestY,                         \
              long SrcX = 0, long SrcY = 0,                   \
              long Width = 0, long Height = 0,                \
              D3DCOLOR Color = 0xFFFFFFFF);
	BOOL Blit(CONST RECT * pSrcRect,long DestX, long DestY, D3DCOLOR Color = 0xFFFFFFFF);
	BOOL Blit(CONST RECT * pSrcRect,CONST D3DXVECTOR3 * pCenter,long DestX, long DestY,  D3DCOLOR Color = 0xFFFFFFFF);

};

//精灵类
class cSprite
{
	//接口
public:
	cTexture		*m_pTexture;								//位图矩形的纹理对象接口指针

public:
	bool					m_bActive;								//是否可用
	int						m_nFrameWidth;							//帧的宽度
	int						m_nFrameHeight;							//帧的高度
	int						m_nFrameBegin;							//起始帧
	int						m_nFrameEnd;							//结束帧

	int						m_nCurrentFrame;						//当前帧
	float					m_nPosX;								//屏幕x坐标
	float					m_nPosY;								//屏幕y坐标

	//方法
public:
	cSprite(void);
	~cSprite(void);

	cSprite & operator = (const cSprite & s);
	
	void zeroSpriteValues(void);									// 重置所有变量值
	bool SetTexture(cTexture *pTexture);		// 绑定纹理
	
	void DrawSprite(cGraphics *Graphics, float a = 1.0f);		// 绘制精灵, a为绘制的半透明度

	// 循环一次
	void LoopIt(void);

};

class cLight
{
  protected:
    D3DLIGHT9 m_Light;

  public:
    cLight();

    D3DLIGHT9 *GetLight();

    BOOL SetType(D3DLIGHTTYPE Type);

    BOOL Move(float XPos, float YPos, float ZPos);
    BOOL MoveRel(float XPos, float YPos, float ZPos);
    BOOL GetPos(float *XPos, float *YPos, float *ZPos);

    BOOL Point(float XFrom, float YFrom, float ZFrom,
               float XAt,   float YAt,   float ZAt);
    BOOL GetDirection(float *XDir, float *YDir, float *ZDir);

    BOOL SetDiffuseColor(char Red, char Green, char Blue);
    BOOL GetDiffuseColor(char *Red, char *Green, char *Blue);

    BOOL SetSpecularColor(char Red, char Green, char Blue);
    BOOL GetSpecularColor(char *Red, char *Green, char *Blue);
    
    BOOL SetAmbientColor(char Red, char Green, char Blue);
    BOOL GetAmbientColor(char *Red, char *Green, char *Blue);

    BOOL SetRange(float Range);
    float GetRange();
    
    BOOL SetFalloff(float Falloff);
    float GetFalloff();
    
    BOOL SetAttenuation0(float Attenuation);
    float GetAttenuation0();

    BOOL SetAttenuation1(float Attenuation);
    float GetAttenuation1();

    BOOL SetAttenuation2(float Attenuation);
    float GetAttenuation2();
    
    BOOL SetTheta(float Theta);
    float GetTheta();
    
    BOOL SetPhi(float Phi);
    float GetPhi();
};

class cWorldPosition
{
  protected:
    BOOL  m_Billboard;

    float m_XPos,      m_YPos,      m_ZPos;
    float m_XRotation, m_YRotation, m_ZRotation;
    float m_XScale,    m_YScale,    m_ZScale;
    
    D3DXMATRIX m_matWorld;
    D3DXMATRIX m_matScale;
    D3DXMATRIX m_matRotation;
    D3DXMATRIX m_matTranslation;
    D3DXMATRIX *m_matCombine1;
    D3DXMATRIX *m_matCombine2;

  public:
    cWorldPosition();

    D3DXMATRIX *GetMatrix(cGraphics *Graphics = NULL);
    BOOL SetCombineMatrix1(D3DXMATRIX *Matrix = NULL);
    BOOL SetCombineMatrix2(D3DXMATRIX *Matrix = NULL);

    BOOL Copy(cWorldPosition *DestPos);

    BOOL Move(float XPos, float YPos, float ZPos);
    BOOL MoveRel(float XAdd, float YAdd, float ZAdd);

    BOOL Rotate(float XRot, float YRot, float ZRot);
    BOOL RotateRel(float XAdd, float YAdd, float ZAdd);

    BOOL Scale(float XScale, float YScale, float ZScale);
    BOOL ScaleRel(float XAdd, float YAdd, float ZAdd);

    BOOL Update(cGraphics *Graphics = NULL);
    BOOL EnableBillboard(BOOL Enable = TRUE);

    float GetXPos();
    float GetYPos();
    float GetZPos();
    float GetXRotation();
    float GetYRotation();
    float GetZRotation();
    float GetXScale();
    float GetYScale();
    float GetZScale();
};

class cCamera
{
  protected:
    float m_XPos, m_YPos, m_ZPos;
    float m_XRot, m_YRot, m_ZRot;

    float m_StartXPos, m_StartYPos, m_StartZPos;
    float m_StartXRot, m_StartYRot, m_StartZRot;

    float m_EndXPos, m_EndYPos, m_EndZPos;
    float m_EndXRot, m_EndYRot, m_EndZRot;
    
    D3DXMATRIX m_matWorld;
    D3DXMATRIX m_matTranslation;
    D3DXMATRIX m_matRotation;

  public:
    cCamera();

    D3DXMATRIX *GetMatrix(); // Get view transformation matrix
    BOOL Update();           // Update transformation matrix

    BOOL Move(float XPos, float YPos, float ZPos);
    BOOL MoveRel(float XAdd, float YAdd, float ZAdd);

    BOOL Rotate(float XRot, float YRot, float ZRot);
    BOOL RotateRel(float XAdd, float YAdd, float ZAdd);

    BOOL Point(float XEye, float YEye, float ZEye, float XAt, float YAt, float ZAt);

    BOOL SetStartTrack();
    BOOL SetEndTrack();
    BOOL Track(float Time, float Length);

    float GetXPos();
    float GetYPos();
    float GetZPos();
    float GetXRotation();
    float GetYRotation();
    float GetZRotation();
};


class cFont
{
  private:
    ID3DXFont *m_Font;

  public:
    cFont();
    ~cFont();

    ID3DXFont *GetFontCOM();

	BOOL cFont::Create(cGraphics *Graphics, LPCTSTR pFacename, INT Height, UINT Width, BOOL Bold);
    BOOL Free();


    BOOL PrintA(LPCSTR Text, long XPos, long YPos, long Width = 0, long Height = 0, D3DCOLOR Color = 0xFFFFFFFF, DWORD Format = 0);
	BOOL Print(LPCTSTR Text, long XPos, long YPos, long Width = 0, long Height = 0, D3DCOLOR Color = 0xFFFFFFFF, DWORD Format = 0);
	BOOL cFont::Print(LPCTSTR Text, CRect *Rect, D3DCOLOR Color = 0xFFFFFFFF, DWORD Format = 0);
	BOOL cFont::PrintA(LPCSTR Text, CRect *Rect, D3DCOLOR Colo = 0xFFFFFFFF, DWORD Format = 0);
};



class cSurface
{
  private:
	//cGraphics         *m_Graphics;
 

  public:
    cSurface();
    ~cSurface();
   IDirect3DSurface9* m_Surface;
   cGraphics         *m_Graphics;
    IDirect3DSurface9 *GetSurfaceCOM();

	BOOL CreateSurface(cGraphics *Graphics, UINT Width,UINT Height);
    BOOL Free();

	//使表面变灰
	BOOL GreySurface(const RECT* srcRect);
	//从表面处取得表面
	BOOL GetSurfaceFromSurface(const RECT* desRect, cSurface* srcSurface, const RECT* srcRect,COLORREF colorkey=D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,1.0f));
	BOOL LoadSurfaceFromFile(cGraphics *Graphics, TCHAR *Filename, UINT Width, UINT Height, D3DCOLOR ColorKey=D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,1.0f));
	BOOL LoadSurfaceFromResource(cGraphics *Graphics, LONG RCID, UINT Width, UINT Height, D3DCOLOR ColorKey =D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,1.0f), TCHAR* strRes =NULL );
	BOOL CopySurface(cSurface* srcSurface,CONST RECT* pSourceRect,CONST RECT* pDestRect);
	void SetSurface(cGraphics *Graphics, IDirect3DSurface9* m_Surface);
	BOOL LoadSurfaceFromResource(const RECT *pDestRect, LONG RCID, const RECT *pSrcRect, D3DCOLOR ColorKey =D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,1.0f), TCHAR* strRes =NULL );
	BOOL GetDesc(D3DSURFACE_DESC * pDesc);
	BOOL BlackSurface(const RECT* srcRect);
};



struct Vertex
{
    D3DXVECTOR3 posit;
    D3DCOLOR    color;

	enum FVF
	{
		FVF_Flags = D3DFVF_XYZ|D3DFVF_DIFFUSE
	};
};

struct Particle
{
    D3DXVECTOR3 m_vCurPos;
    D3DXVECTOR3 m_vCurVel;
	D3DCOLOR    m_vColor;
};

const int MAX_PARTICLES = 100;

inline DWORD FtoDW( FLOAT f ) { return *((DWORD*)&f); }
class cPointSprite
{
public:
	bool	m_bActive;
	bool	m_bFirstFrame;
	Particle g_particles[MAX_PARTICLES];	 //顶点精灵容器

	//--------------------
	LPDIRECT3DDEVICE9		m_pDevice;		 //设备接口指针
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer ;//顶点缓存区
	cTexture      m_Texture;//纹理接口

public:
	cPointSprite(void);
	~cPointSprite(void);

	//----------------
	// 随机函数
	float getRandomMinMax( float fMin, float fMax );
	// 随机函数
	D3DXVECTOR3 getRandomVector( void );
	// 初始化顶点精灵
	void initPointSprites();
	// 更新顶点位置
	bool updatePointSprites( void );
	// 绘制
	void renderPointSprites( void );
	// 绘制接口
	void DrawPointSprite(void);
	// 初始
	void zeroSpriteValues(void);
	// 设置顶点精灵参数
	void SetPointSprite(cGraphics *Graphics,  LONG RCID, TCHAR* strRes, UINT Width,  UINT Height, D3DCOLOR ColorKey = D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,1.0f));
	// 开按钮
	void On(void);
	// 关按钮
	void Off(void);
};

#endif
