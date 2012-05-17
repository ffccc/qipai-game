/**************************************************
WinMain.cpp
GameCore Component

Programming Role-Playing Games with DirectX
by Jim Adams (01 Jan 2002)
**************************************************/
#include "StdAfx.h"
#include "DxGraphics.h"

#include "rmxftmpl.h"
#include "rmxfguid.h"

cGraphics::cGraphics()
{
  m_hWnd = NULL;
  
  m_pD3D = NULL;
  m_pD3DDevice = NULL;
  m_pSprite = NULL;
  m_Surface = NULL;

}

cGraphics::~cGraphics()
{
  Shutdown();
}

BOOL cGraphics::Init(HWND hWnd)
{
	Shutdown();

	if((m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
		return FALSE;



	HRESULT rslt;
	D3DPRESENT_PARAMETERS	d3dpp;	

	rslt = m_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &m_d3ddm );
	if(FAILED(rslt)) {MessageBox(NULL,TEXT("获得显示适配器模式失败!"),TEXT(""),MB_OK);return FALSE;}

	ZeroMemory( &d3dpp, sizeof(d3dpp) );

	d3dpp.Windowed         = TRUE;
	d3dpp.SwapEffect       = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = m_d3ddm.Format;

	d3dpp.EnableAutoDepthStencil = FALSE;
	//d3dpp.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
	//d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE; // Do NOT sync to vertical retrace
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_DEFAULT; // Sync to vertical retrace
	d3dpp.Flags                  = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	//创建DirectX9驱动
	rslt = m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice );
	if(FAILED(rslt))
	{
		rslt = m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice );

		if(FAILED(rslt))
		{
			if(MessageBox( NULL,TEXT("亲爱的用户：\r\n运行连连看的时候，创建DirectX9驱动失败!\r\n您的电脑可能没有安装DirectX9.0c,必须安装之后才能运行游戏！\r\n是否立即安装？"),TEXT(""),MB_OKCANCEL) == IDOK)
			{
				ShellExecute(NULL,TEXT("open"),TEXT("http://www.microsoft.com/downloads/info.aspx?na=90&p=&SrcDisplayLang=zh-cn&SrcCategoryId=&SrcFamilyId=402111C4-6478-46C1-BBD9-1BF9FCCE52F4&u=http%3a%2f%2fdownload.microsoft.com%2fdownload%2fa%2f6%2f1%2fa6153887-2fe7-4dab-88c4-4da642909b9e%2fdirectx_9c_Apr05sdk_redist.exe"),NULL,NULL,SW_SHOWDEFAULT);
			
			}
			return FALSE;
		}
	}

	if(FAILED(D3DXCreateSprite(m_pD3DDevice, &m_pSprite)))
		return FALSE;
	return TRUE;
}



BOOL cGraphics::Shutdown()
{
  ReleaseCOM(m_pSprite);
  ReleaseCOM(m_Surface);
  ReleaseCOM(m_pD3DDevice);
  ReleaseCOM(m_pD3D);
  
  return TRUE;
}

inline IDirect3D9 *cGraphics::GetDirect3DCOM()
{
  return m_pD3D;
}

D3DFORMAT cGraphics::GetBackBufferFormat()
{
	return m_d3ddm.Format;
}

inline IDirect3DDevice9 *cGraphics::GetDeviceCOM()
{
  return m_pD3DDevice;
}

inline ID3DXSprite *cGraphics::GetSpriteCOM()
{
  return m_pSprite;
}

long cGraphics::GetNumDisplayModes(D3DFORMAT Format)
{
  if(m_pD3D == NULL)
    return 0;

  return (long)m_pD3D->GetAdapterModeCount(D3DADAPTER_DEFAULT, Format);
}

BOOL cGraphics::GetDisplayModeInfo(D3DFORMAT Format,long Num, D3DDISPLAYMODE *Mode)
{
  long Max;

  if(m_pD3D == NULL)
    return FALSE;

  Max = GetNumDisplayModes(Format);
  if(Num >= Max)
    return FALSE;

  if(FAILED(m_pD3D->EnumAdapterModes(D3DADAPTER_DEFAULT,Format,
            Num, Mode)))
    return FALSE;

  return TRUE;
}


BOOL cGraphics::BeginScene()
{
  if(m_pD3DDevice == NULL)
    return FALSE;

  if(FAILED(m_pD3DDevice->BeginScene()))
    return FALSE;

  return TRUE;
}

BOOL cGraphics::EndScene()
{
  short i;

  // Error checking
  if(m_pD3DDevice == NULL)
    return FALSE;

  // Release all textures
  for(i=0;i<8;i++)
    m_pD3DDevice->SetTexture(i, NULL);

  // End the scene
  if(FAILED(m_pD3DDevice->EndScene()))
    return FALSE;

  return TRUE;
}

BOOL cGraphics::BeginSprite(DWORD Flags)
{
  if(m_pSprite == NULL)
    return FALSE;

  if(FAILED(m_pSprite->Begin(Flags)))
    return FALSE;

  return TRUE;
}

BOOL cGraphics::EndSprite()
{
  if(m_pSprite == NULL)
    return FALSE;

  if(FAILED(m_pSprite->End()))
    return FALSE;

  return TRUE;
}


BOOL cGraphics::Display()
{
  if(m_pD3DDevice == NULL)
    return FALSE;

  if(FAILED(m_pD3DDevice->Present(NULL, NULL, NULL, NULL)))
    return FALSE;

  return TRUE;
}

BOOL cGraphics::Clear(long Color, float ZBuffer)
{
  if(m_pD3DDevice == NULL)
    return FALSE;

   ClearDisplay(Color);

  return TRUE; 
}

BOOL cGraphics::ClearDisplay(long Color)
{
  if(m_pD3DDevice == NULL)
    return FALSE;

  if(FAILED(m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, Color, 1.0f, 0)))
    return FALSE;

  return TRUE; 
}  




BOOL cGraphics::SetCamera(cCamera *Camera)
{
  if(m_pD3DDevice == NULL || Camera == NULL)
    return FALSE;

  if(FAILED(m_pD3DDevice->SetTransform(D3DTS_VIEW, Camera->GetMatrix())))
    return FALSE;

  return TRUE;
}

BOOL cGraphics::SetWorldPosition(cWorldPosition *WorldPos)
{
  if(WorldPos == NULL || m_pD3DDevice == NULL)
    return FALSE;

  if(FAILED(m_pD3DDevice->SetTransform(D3DTS_WORLD, WorldPos->GetMatrix(this))))
    return FALSE;

  return TRUE;
}

BOOL cGraphics::SetLight(long Num, cLight *Light)
{
  if(Light == NULL)
    return FALSE;
  if(m_pD3DDevice == NULL)
    return FALSE;

  if(FAILED(m_pD3DDevice->SetLight(Num, Light->GetLight())))
    return FALSE;
  return TRUE;
}


BOOL cGraphics::SetMaterial(cMaterial *Material)
{
  if(m_pD3DDevice == NULL)
    return FALSE;

  if(Material != NULL) {
    if(FAILED(m_pD3DDevice->SetMaterial(Material->GetMaterial())))
      return FALSE;
  }

  return TRUE;
}

BOOL cGraphics::SetTexture(short Num, cTexture *Texture)
{
  // Error checking
  if(m_pD3DDevice == NULL || Num < 0 || Num > 7)
    return FALSE;

  if(Texture == NULL) {
    // Clear the texture
    if(FAILED(m_pD3DDevice->SetTexture(Num, NULL)))
      return NULL;
  } else {
    // Set the texture
    if(FAILED(m_pD3DDevice->SetTexture(Num, Texture->GetTextureCOM())))
      return FALSE;
  }

  return TRUE;
}

BOOL cGraphics::SetPerspective(float FOV, float Aspect, float Near, float Far)
{
  D3DXMATRIX matProjection;

  if(m_pD3DDevice == NULL)
    return FALSE;

  D3DXMatrixPerspectiveFovLH(&matProjection, FOV, Aspect, Near, Far);
  if(FAILED(m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProjection)))
    return FALSE;
  return TRUE;
}

BOOL cGraphics::EnableLight(long Num, BOOL Enable)
{
  if(m_pD3DDevice == NULL)
    return FALSE;

  if(FAILED(m_pD3DDevice->LightEnable(Num, Enable)))
    return FALSE;

  return TRUE;
}

BOOL cGraphics::EnableLighting(BOOL Enable)
{
  if(m_pD3DDevice == NULL)
    return FALSE;

  if(FAILED(m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, Enable)))
    return FALSE;

  return TRUE;
}


BOOL cGraphics::EnableAlphaBlending(BOOL Enable, DWORD Src, DWORD Dest)
{
  if(m_pD3DDevice == NULL)
    return FALSE;

  // Enable or disable
  if(FAILED(m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, Enable)))
    return FALSE;

  // Set blend type
  if(Enable == TRUE) {
    m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,  Src);
    m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, Dest);
  }

  return TRUE;
}

BOOL cGraphics::EnableAlphaTesting(BOOL Enable)
{
  if(m_pD3DDevice == NULL)
    return FALSE;

  if(FAILED(m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, Enable)))
    return FALSE;

  // Set test type
  if(Enable == TRUE) {
    m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0x08);
    m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
  }

  return TRUE;
}


IDirect3DSurface9* cGraphics::getBackBuffer() 
{ 
	
	if(m_pD3DDevice == NULL)
		return NULL;

	ReleaseCOM(m_Surface);
	HRESULT hResult = m_pD3DDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO, &m_Surface); 


	if (FAILED(hResult)) 
	{
		//AfxMessageBox("获取背景缓存区指针失败");
		return NULL; 
	}
	else 
		return m_Surface; 
} 




cMaterial::cMaterial()
{
  // Set a default material (white)
  ZeroMemory(&m_Material, sizeof(D3DMATERIAL9));
  SetDiffuseColor((char)255,(char)255,(char)255);
  SetAmbientColor((char)255,(char)255,(char)255);
  SetSpecularColor((char)255,(char)0xFF,(char)255);
  SetEmissiveColor(0,0,0);
  SetPower(1.0f);
}

BOOL cMaterial::SetDiffuseColor(char Red, char Green, char Blue)
{
  m_Material.Diffuse.r = 1.0f / 255.0f * (float)Red;
  m_Material.Diffuse.g = 1.0f / 255.0f * (float)Green;
  m_Material.Diffuse.b = 1.0f / 255.0f * (float)Blue;

  return TRUE;
}

BOOL cMaterial::GetDiffuseColor(char *Red, char *Green, char *Blue)
{
  if(Red != NULL)
    *Red = (char)(255.0f * m_Material.Diffuse.r);

  if(Green != NULL)
    *Green = (char)(255.0f * m_Material.Diffuse.g);

  if(Blue != NULL)
    *Blue = (char)(255.0f * m_Material.Diffuse.b);

  return TRUE;
}

BOOL cMaterial::SetAmbientColor(char Red, char Green, char Blue)
{
  m_Material.Ambient.r = 1.0f / 255.0f * (float)Red;
  m_Material.Ambient.g = 1.0f / 255.0f * (float)Green;
  m_Material.Ambient.b = 1.0f / 255.0f * (float)Blue;

  return TRUE;
}

BOOL cMaterial::GetAmbientColor(char *Red, char *Green, char *Blue)
{
  if(Red != NULL)
    *Red = (char)(255.0f * m_Material.Ambient.r);

  if(Green != NULL)
    *Green = (char)(255.0f * m_Material.Ambient.g);

  if(Blue != NULL)
    *Blue = (char)(255.0f * m_Material.Ambient.b);

  return TRUE;
}

BOOL cMaterial::SetSpecularColor(char Red, char Green, char Blue)
{
  m_Material.Specular.r = 1.0f / 255.0f * (float)Red;
  m_Material.Specular.g = 1.0f / 255.0f * (float)Green;
  m_Material.Specular.b = 1.0f / 255.0f * (float)Blue;

  return TRUE;
}

BOOL cMaterial::GetSpecularColor(char *Red, char *Green, char *Blue)
{
  if(Red != NULL)
    *Red = (char)(255.0f * m_Material.Specular.r);

  if(Green != NULL)
    *Green = (char)(255.0f * m_Material.Specular.g);

  if(Blue != NULL)
    *Blue = (char)(255.0f * m_Material.Specular.b);

  return TRUE;
}

BOOL cMaterial::SetEmissiveColor(char Red, char Green, char Blue)
{
  m_Material.Emissive.r = 1.0f / 255.0f * (float)Red;
  m_Material.Emissive.g = 1.0f / 255.0f * (float)Green;
  m_Material.Emissive.b = 1.0f / 255.0f * (float)Blue;

  return TRUE;
}

BOOL cMaterial::GetEmissiveColor(char *Red, char *Green, char *Blue)
{
  if(Red != NULL)
    *Red = (char)(255.0f * m_Material.Emissive.r);

  if(Green != NULL)
    *Green = (char)(255.0f * m_Material.Emissive.g);

  if(Blue != NULL)
    *Blue = (char)(255.0f * m_Material.Emissive.b);

  return TRUE;
}

BOOL cMaterial::SetPower(float Power)
{
  m_Material.Power = Power;
  return TRUE;
}

float cMaterial::GetPower(float Power)
{
  return m_Material.Power;
}

D3DMATERIAL9 *cMaterial::GetMaterial()
{
  return &m_Material;
}



cWorldPosition::cWorldPosition()
{
  m_Billboard = FALSE;
  m_matCombine1 = m_matCombine2 = NULL;

  Move(0.0f,0.0f,0.0f);
  Rotate(0.0f,0.0f,0.0f);
  Scale(1.0f, 1.0f, 1.0f);

  Update();
}

BOOL cWorldPosition::Copy(cWorldPosition *DestPos)
{
  DestPos->Move(m_XPos, m_YPos, m_ZPos);
  DestPos->Rotate(m_XRotation, m_YRotation, m_ZRotation);
  DestPos->Scale(m_XScale, m_YScale, m_ZScale);
  DestPos->EnableBillboard(m_Billboard);
 
  return TRUE;
}

BOOL cWorldPosition::Move(float XPos, float YPos, float ZPos)
{
  m_XPos = XPos;
  m_YPos = YPos;
  m_ZPos = ZPos;

  D3DXMatrixTranslation(&m_matTranslation, m_XPos, m_YPos, m_ZPos);

  return TRUE;
}

BOOL cWorldPosition::MoveRel(float XAdd, float YAdd, float ZAdd)
{
  return Move(m_XPos + XAdd, m_YPos + YAdd, m_ZPos + ZAdd);
}

BOOL cWorldPosition::Rotate(float XRot, float YRot, float ZRot)
{
  m_XRotation = XRot;
  m_YRotation = YRot;
  m_ZRotation = ZRot;

  D3DXMatrixRotationYawPitchRoll(&m_matRotation, m_YRotation, m_XRotation, m_ZRotation);

  return TRUE;
}

BOOL cWorldPosition::RotateRel(float XAdd, float YAdd, float ZAdd)
{
  return Rotate(m_XRotation + XAdd, m_YRotation + YAdd, m_ZRotation + ZAdd);
}

BOOL cWorldPosition::Scale(float XScale, float YScale, float ZScale)
{
  m_XScale = XScale;
  m_YScale = YScale;
  m_ZScale = ZScale;

  D3DXMatrixScaling(&m_matScale, XScale, YScale, ZScale);

  return TRUE;
}

BOOL cWorldPosition::ScaleRel(float XAdd, float YAdd, float ZAdd)
{
  return Scale(m_XScale + XAdd, m_YScale + YAdd, m_ZScale + ZAdd);
}

BOOL cWorldPosition::Update(cGraphics *Graphics)
{
  D3DXMATRIX matView, matTransposed;

  // Setup billboarding matrix
  if(m_Billboard == TRUE) {
    if(Graphics != NULL && Graphics->GetDeviceCOM() != NULL) {
      Graphics->GetDeviceCOM()->GetTransform(D3DTS_VIEW, &matView);
      D3DXMatrixTranspose(&matTransposed, &matView);
      matTransposed._41 = matTransposed._42 = matTransposed._43 = matTransposed._14 = matTransposed._24 = matTransposed._34 = 0.0f;
    } else {
      D3DXMatrixIdentity(&matTransposed);
    }
  }

  // Combine scaling and rotation matrices first
  D3DXMatrixMultiply(&m_matWorld, &m_matScale, &m_matRotation);

  // Apply billboard matrix
  if(m_Billboard == TRUE)
    D3DXMatrixMultiply(&m_matWorld, &m_matWorld, &matTransposed);  

  // Combine with translation matrix
  D3DXMatrixMultiply(&m_matWorld, &m_matWorld, &m_matTranslation);

  // Combine with combined matrices (if any)
  if(m_matCombine1 != NULL) 
    D3DXMatrixMultiply(&m_matWorld, &m_matWorld, m_matCombine1);
  if(m_matCombine2 != NULL) 
    D3DXMatrixMultiply(&m_matWorld, &m_matWorld, m_matCombine2);

  return TRUE;
}

BOOL cWorldPosition::EnableBillboard(BOOL Enable)
{
  m_Billboard = Enable;
  return TRUE;
}

D3DXMATRIX *cWorldPosition::GetMatrix(cGraphics *Graphics)
{
  Update(Graphics);
  return &m_matWorld;
}

BOOL cWorldPosition::SetCombineMatrix1(D3DXMATRIX *Matrix)
{
  m_matCombine1 = Matrix;
  return TRUE;
}

BOOL cWorldPosition::SetCombineMatrix2(D3DXMATRIX *Matrix)
{
  m_matCombine2 = Matrix;
  return TRUE;
}

float cWorldPosition::GetXPos()
{
  return m_XPos;
}

float cWorldPosition::GetYPos()
{
  return m_YPos;
}

float cWorldPosition::GetZPos()
{
  return m_ZPos;
}

float cWorldPosition::GetXRotation()
{
  return m_XRotation;
}

float cWorldPosition::GetYRotation()
{
  return m_YRotation;
}

float cWorldPosition::GetZRotation()
{
  return m_ZRotation;
}

float cWorldPosition::GetXScale()
{
  return m_XScale;
}

float cWorldPosition::GetYScale()
{
  return m_YScale;
}

float cWorldPosition::GetZScale()
{
  return m_ZScale;
}

cCamera::cCamera()
{
  Move(0.0f,0.0f,0.0f);
  Rotate(0.0f,0.0f,0.0f);
  Update();
}

BOOL cCamera::Move(float XPos, float YPos, float ZPos)
{
  m_XPos = XPos;
  m_YPos = YPos;
  m_ZPos = ZPos;

  D3DXMatrixTranslation(&m_matTranslation, -m_XPos, -m_YPos, -m_ZPos);

  return TRUE;
}

BOOL cCamera::MoveRel(float XAdd, float YAdd, float ZAdd)
{
  return Move(m_XPos + XAdd, m_YPos + YAdd, m_ZPos + ZAdd);
}

BOOL cCamera::Rotate(float XRot, float YRot, float ZRot)
{
  D3DXMATRIX matXRotation, matYRotation, matZRotation;

  m_XRot = XRot;
  m_YRot = YRot;
  m_ZRot = ZRot;

  D3DXMatrixRotationX(&matXRotation, -m_XRot);
  D3DXMatrixRotationY(&matYRotation, -m_YRot);
  D3DXMatrixRotationZ(&matZRotation, -m_ZRot);

  m_matRotation = matZRotation;
  D3DXMatrixMultiply(&m_matRotation, &m_matRotation, &matYRotation);
  D3DXMatrixMultiply(&m_matRotation, &m_matRotation, &matXRotation);

  return TRUE;
}

BOOL cCamera::RotateRel(float XAdd, float YAdd, float ZAdd)
{
  return Rotate(m_XRot + XAdd, m_YRot + YAdd, m_ZRot + ZAdd);
}

BOOL cCamera::Point(float XEye, float YEye, float ZEye, float XAt, float YAt, float ZAt)
{
  float XRot, YRot, XDiff, YDiff, ZDiff;

  // Calculate angles between points
  XDiff = XAt - XEye;
  YDiff = YAt - YEye;
  ZDiff = ZAt - ZEye;
  XRot = (float)atan2(-YDiff, sqrt(XDiff*XDiff+ZDiff*ZDiff));
  YRot = (float)atan2(XDiff, ZDiff);

  Move(XEye, YEye, ZEye);
  Rotate(XRot, YRot, 0.0f);

  return TRUE;
}

BOOL cCamera::SetStartTrack()
{
  m_StartXPos = m_XPos;
  m_StartYPos = m_YPos;
  m_StartZPos = m_ZPos;
  m_StartXRot = m_XRot;
  m_StartYRot = m_YRot;
  m_StartZRot = m_ZRot;
  return TRUE;
}

BOOL cCamera::SetEndTrack()
{
  m_EndXPos = m_XPos;
  m_EndYPos = m_YPos;
  m_EndZPos = m_ZPos;
  m_EndXRot = m_XRot;
  m_EndYRot = m_YRot;
  m_EndZRot = m_ZRot;
  return TRUE;
}

BOOL cCamera::Track(float Time, float Length)
{
  float x, y, z;
  float TimeOffset;

  TimeOffset = Length * Time;

  x = (m_EndXPos - m_StartXPos) / Length * TimeOffset;
  y = (m_EndYPos - m_StartYPos) / Length * TimeOffset;
  z = (m_EndZPos - m_StartZPos) / Length * TimeOffset;
  Move(m_StartXPos + x, m_StartYPos + y, m_StartZPos + z);

  x = (m_EndXRot - m_StartXRot) / Length * TimeOffset;
  y = (m_EndYRot - m_StartYRot) / Length * TimeOffset;
  z = (m_EndZRot - m_StartZRot) / Length * TimeOffset;
  Rotate(m_StartXRot + x, m_StartYRot + y, m_StartZRot + z);

  return TRUE;
}

BOOL cCamera::Update()
{
  D3DXMatrixMultiply(&m_matWorld, &m_matTranslation, &m_matRotation);
  return TRUE;
}

D3DXMATRIX *cCamera::GetMatrix()
{
  Update();
  return &m_matWorld;
}

float cCamera::GetXPos()
{
  return m_XPos;
}

float cCamera::GetYPos()
{
  return m_YPos;
}

float cCamera::GetZPos()
{
  return m_ZPos;
}

float cCamera::GetXRotation()
{
  return m_XRot;
}

float cCamera::GetYRotation()
{
  return m_YRot;
}

float cCamera::GetZRotation()
{
  return m_ZRot;
}





cTexture::cTexture()
{
  m_Graphics = NULL;
  m_Texture = NULL;
}

cTexture::~cTexture()
{
  Free();
}

BOOL cTexture::LoadTextureFromFile(cGraphics *Graphics,TCHAR *Filename, UINT Width, UINT Height,   D3DCOLOR ColorKey)
{

  if((m_Graphics = Graphics) == NULL)
    return FALSE;
  if(Graphics->GetDeviceCOM() == NULL)
    return FALSE;
  if(Filename == NULL)
    return FALSE;

  Free();


  if(FAILED(D3DXCreateTextureFromFileEx(Graphics->GetDeviceCOM(),
    Filename, Width, Height, 1,
    0, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, 
    D3DX_DEFAULT, D3DX_DEFAULT,
	ColorKey, NULL, NULL, &m_Texture))){
  if(FAILED(D3DXCreateTextureFromFileEx(Graphics->GetDeviceCOM(),
    Filename, Width, Height, 1,
    0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, 
    D3DX_DEFAULT, D3DX_DEFAULT,
	ColorKey, NULL, NULL, &m_Texture))){
	  MessageBox(NULL, TEXT("从文件中加载图片失败!"), TEXT(""), MB_OK);
      return FALSE;
	}
	}

  m_Graphics =Graphics;

  return TRUE;
}



BOOL cTexture::LoadTextureFromResource(cGraphics *Graphics, 
										  LONG RCID, UINT Width,  UINT Height, D3DCOLOR ColorKey, TCHAR* strRes )
{
	HRESULT hr;
	HMODULE hModule = NULL;
	HRSRC rsrc;
	HGLOBAL hgData;
	LPVOID pvData;
	DWORD cbData;


  if((m_Graphics = Graphics) == NULL)
    return FALSE;
  if(Graphics->GetDeviceCOM() == NULL)
    return FALSE;


  Free();

	if( FAILED(D3DXCreateTextureFromResourceEx(Graphics->GetDeviceCOM(), GetModuleHandle( NULL ), MAKEINTRESOURCE(RCID),
		Width, 
		Height, 
		1,
		0,
		D3DFMT_A8R8G8B8, 
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT, 
		D3DX_DEFAULT,
		ColorKey,
		NULL,
		NULL, 
		&m_Texture))){

	if( FAILED(D3DXCreateTextureFromResourceEx(Graphics->GetDeviceCOM(), GetModuleHandle( NULL ), MAKEINTRESOURCE(RCID),
		Width, 
		Height, 
		1,
		0,
		D3DFMT_A8R8G8B8, 
		D3DPOOL_MANAGED,
		D3DX_DEFAULT, 
		D3DX_DEFAULT,
		ColorKey,
		NULL,
		NULL, 
		&m_Texture))){

			rsrc = FindResource( hModule, MAKEINTRESOURCE(RCID), strRes );
			if( rsrc != NULL )
			{
				cbData = SizeofResource( hModule, rsrc );
				if( cbData > 0 )
				{
					hgData = LoadResource( hModule, rsrc );
					if( hgData != NULL )
					{
						pvData = LockResource( hgData );
						if( pvData != NULL )
						{
							if( FAILED( hr = D3DXCreateTextureFromFileInMemoryEx( 
								Graphics->GetDeviceCOM(), pvData, cbData, Width, Height, 
								1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, 
								D3DX_FILTER_NONE, ColorKey, NULL, NULL, &m_Texture ) ) )
							{
								MessageBox(NULL, TEXT("不能从内存中加载此资源！"), TEXT(""), MB_OK);
								return FALSE;
							}
						} else {
							MessageBox(NULL, TEXT("不能锁住资源！"), TEXT(""), MB_OK);
							return FALSE;
						}
					} else {
						MessageBox(NULL, TEXT("不能加载资源！"), TEXT(""), MB_OK);
						return FALSE;
					}
				} else {
					MessageBox(NULL, TEXT("资源大小为0！"), TEXT(""), MB_OK);
					return FALSE;
				}
			} else {
				MessageBox(NULL, TEXT("不能找到资源！"), TEXT(""), MB_OK);
				return FALSE;
			}
		} 
		}
		m_Graphics =Graphics;
		return TRUE;


}


BOOL cTexture::CreateRenderTexture(cGraphics *Graphics, UINT Width, UINT Height){
  if((m_Graphics = Graphics) == NULL)
    return FALSE;
  if(Graphics->GetDeviceCOM() == NULL)
    return FALSE;
	if(FAILED(Graphics->GetDeviceCOM()->CreateTexture(Width,Height,1,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&m_Texture,NULL))){
		return FALSE;
	}
	else {
		return TRUE;
	}

}
BOOL cTexture::LoadRenderTextureFromResource(cGraphics *Graphics, 
										  LONG RCID, UINT Width,  UINT Height, D3DCOLOR ColorKey )
{

  if((m_Graphics = Graphics) == NULL)
    return FALSE;
  if(Graphics->GetDeviceCOM() == NULL)
    return FALSE;


  Free();

	if( FAILED(D3DXCreateTextureFromResourceEx(Graphics->GetDeviceCOM(), GetModuleHandle( NULL ), MAKEINTRESOURCE(RCID),
		Width, 
		Height, 
		1,
		D3DUSAGE_RENDERTARGET,//0
		D3DFMT_A8R8G8B8, 
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT, 
		D3DX_DEFAULT,
		ColorKey,
		NULL,
		NULL, 
		&m_Texture))){

			MessageBox(NULL, TEXT("Wrong Load Texture with D3DFMT_A8R8G8B8"), TEXT(""), MB_OK);
			return FALSE;
		}
		m_Graphics =Graphics;
		return TRUE;
}


BOOL cTexture::LoadDynamicTextureFromResource(cGraphics *Graphics, 
										  LONG RCID, UINT Width,  UINT Height, D3DCOLOR ColorKey )
{

  if((m_Graphics = Graphics) == NULL)
    return FALSE;
  if(Graphics->GetDeviceCOM() == NULL)
    return FALSE;


  Free();

	if( FAILED(D3DXCreateTextureFromResourceEx(Graphics->GetDeviceCOM(), GetModuleHandle( NULL ), MAKEINTRESOURCE(RCID),
		Width, 
		Height, 
		1,
		D3DUSAGE_DYNAMIC,//0
		D3DFMT_A8R8G8B8, 
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT, 
		D3DX_DEFAULT,
		ColorKey,
		NULL,
		NULL, 
		&m_Texture))){

			//MessageBox(NULL, TEXT("Wrong Load Texture with use Dynamic"), TEXT(""), MB_OK);
			return FALSE;
		}
		m_Graphics =Graphics;
		return TRUE;


}



BOOL cTexture::Create(cGraphics *Graphics, IDirect3DTexture9 *Texture)
{
  D3DLOCKED_RECT SrcRect, DestRect;
  D3DSURFACE_DESC Desc;

  if((m_Graphics = Graphics) == NULL)
    return FALSE;

  if(Texture == NULL)
    return TRUE;
  
  Free();
    
  // Copy texture over
  Texture->GetLevelDesc(0, &Desc);

  Graphics->GetDeviceCOM()->CreateTexture(Desc.Width, Desc.Height, 0, 0, Desc.Format, D3DPOOL_MANAGED, &m_Texture, NULL);

  Texture->LockRect(0, &SrcRect, NULL, D3DLOCK_READONLY);
  m_Texture->LockRect(0, &DestRect, NULL, 0);

  memcpy(DestRect.pBits, SrcRect.pBits, SrcRect.Pitch * Desc.Height);

  m_Texture->UnlockRect(0);
  Texture->UnlockRect(0);

  return TRUE;
}



BOOL cTexture::GetTextureFromTexture(const RECT* desRect, cTexture* srcTexture, const RECT* srcRect,COLORREF colorkey)
{
	D3DSURFACE_DESC ddsd;
	D3DLOCKED_RECT lockedRectBackBufferSrc;
	D3DLOCKED_RECT lockedRectBackBufferDes;
	IDirect3DSurface9* targetsuffer = NULL; 
	int nColorDepth ;
	int linearPitchSrc =0;
	int linearPitchDes =0;
	//IDirect3DDevice9 *pDevice;
	IDirect3DTexture9* src;
	src = srcTexture->GetTextureCOM();

	if(!m_Graphics)
		return FALSE;
	if(!m_Texture)
		return FALSE;
	if(!src)
		return FALSE;



	src->LockRect(0, &lockedRectBackBufferSrc, srcRect, D3DLOCK_READONLY) ;
	m_Texture->LockRect(0, &lockedRectBackBufferDes, desRect, 0 ) ;

	m_Texture->GetLevelDesc( 0, &ddsd );

	switch( ddsd.Format)
	{
		case D3DFMT_R8G8B8 :

			nColorDepth = 24;
			break;

		case D3DFMT_A2R10G10B10:
			// 32-bit pixel format using 10 bits each for red, green, and blue, 
			// and 2 bits for alpha. Used only inn full-screen mode.
			nColorDepth = 32;
			break;

		case D3DFMT_A8R8G8B8:
			// 32-bit ARGB pixel format with alpha, using 8 bits per channel.
			nColorDepth = 32;
			break;

		case D3DFMT_X8R8G8B8:
			// 32-bit RGB pixel format, where 8 bits are reserved for each color.
			nColorDepth = 32;
			break;

		case D3DFMT_A4R4G4B4 :
			nColorDepth = 16;
			break;

		case D3DFMT_A1R5G5B5:
			// 16-bit pixel format where 5 bits are reserved for each color 
			// and 1 bit is reserved for alpha.
			nColorDepth = 16;
			break;

		case D3DFMT_X1R5G5B5:
			// 16-bit pixel format where 5 bits are reserved for each color.
			nColorDepth = 16;
			break;

		case D3DFMT_R5G6B5:
			// 16-bit RGB pixel format with 5 bits for red, 6 bits for green, 
			// and 5 bits for blue.
			nColorDepth = 16;
			break;
		case D3DFMT_A8R3G3B2 :
			nColorDepth = 16;
			break;
		case D3DFMT_X4R4G4B4 :
			nColorDepth = 16;
			break;

		default:
			CString msg;
			msg.Format("%d",ddsd.Format );
			AfxMessageBox(msg);
			break;
	}

	linearPitchSrc = lockedRectBackBufferSrc.Pitch;
	linearPitchDes= lockedRectBackBufferDes.Pitch;

	if( nColorDepth == 16 )
	{
		colorkey = 0x0000;
		USHORT *surfaceBufferSrc = static_cast<USHORT*>(lockedRectBackBufferSrc.pBits);
		USHORT *surfaceBufferDes = static_cast<USHORT*>(lockedRectBackBufferDes.pBits);
		linearPitchSrc = (linearPitchSrc>>1);
		linearPitchDes=(linearPitchDes>>1);

		for( int j = srcRect->top,m = 0; m < desRect->bottom-desRect->top; j++ ,m++)
			for( int i = srcRect->left,n=0; n < desRect->right-desRect->left; i++ ,n++)
			{
				if(surfaceBufferSrc[i+j*linearPitchSrc] != (USHORT)colorkey)
					surfaceBufferDes[n+m*linearPitchDes] = (USHORT)surfaceBufferSrc[i+j*linearPitchSrc];
			}
	}	
	else
	if( nColorDepth == 24)
	{
		colorkey = 0x000000;
		UINT *surfaceBufferSrc = static_cast<UINT*>(lockedRectBackBufferSrc.pBits);
		UINT *surfaceBufferDes = static_cast<UINT*>(lockedRectBackBufferDes.pBits);

		for( int j = srcRect->top,m = 0; m < desRect->bottom-desRect->top; j++ ,m++)
			for( int i = srcRect->left,n=0; n < desRect->right-desRect->left; i++ ,n++)
			{
				if(surfaceBufferSrc[i*3+j*lockedRectBackBufferSrc.Pitch] != (USHORT)colorkey)
					surfaceBufferDes[n*3+m*lockedRectBackBufferDes.Pitch] = (USHORT)surfaceBufferSrc[i*3+j*lockedRectBackBufferSrc.Pitch];
			}
	}
	else
	if( nColorDepth == 32 )
	{
		colorkey = 0x00000000;
		UINT *surfaceBufferSrc = static_cast<UINT*>(lockedRectBackBufferSrc.pBits);
		UINT *surfaceBufferDes = static_cast<UINT*>(lockedRectBackBufferDes.pBits);
		linearPitchSrc = (linearPitchSrc>>2);
		linearPitchDes=(linearPitchDes>>2);
		
		for( int j = srcRect->top,m = 0; m < desRect->bottom-desRect->top; j++ ,m++)
			for( int i = srcRect->left,n=0; n < desRect->right-desRect->left; i++ ,n++)
			{
				if(surfaceBufferSrc[i+j*linearPitchSrc] != colorkey)
					surfaceBufferDes[n+m*linearPitchDes] = surfaceBufferSrc[i+j*linearPitchSrc];
			}
	}

	m_Texture->UnlockRect(0);
	src->UnlockRect(0);


	return TRUE;
}



BOOL cTexture::BlackTexture(const RECT* srcRect)
{
	D3DSURFACE_DESC ddsd;
	D3DLOCKED_RECT lockedRectBackBufferSrc;
	//双缓冲画图
	IDirect3DTexture9* targetsuffer = NULL; 
	int nColorDepth ;
	int linearPitchSrc =0;
	//=======位转换============
	int cMask; //取24位
	int aMask; //alphi掩码
	int rMask; //红色掩码
	int gMask; //绿色掩码
	int bMask; //兰色掩码

	int rMove;
	int gMove;
	//IDirect3DDevice9 *pDevice;

	if(!m_Graphics)
		return FALSE;
	if(!m_Texture)
		return FALSE;


	int sour,r,g,b,t; //临时变量

	m_Texture->LockRect(0, &lockedRectBackBufferSrc, srcRect, 0 ) ;

	m_Texture->GetLevelDesc( 0, &ddsd );

	switch( ddsd.Format )
	{
		case D3DFMT_A2R10G10B10:
			// 32-bit pixel format using 10 bits each for red, green, and blue, 
			// and 2 bits for alpha. Used only inn full-screen mode.
			nColorDepth = 32;

			//=======32位转换============
			cMask=0x03FFFFFF; //取30位
			aMask=0xC0000000; //alphi掩码
			rMask=0x3ff00000; //红色掩码
			gMask=0x000FFC00; //绿色掩码
			bMask=0x000003FF; //兰色掩码

			rMove = 20;
			gMove = 10;
			break;

		case D3DFMT_A8R8G8B8:
			// 32-bit ARGB pixel format with alpha, using 8 bits per channel.
			nColorDepth = 32;
			//=======32位转换============
			cMask=0x00FFFFFF; //取24位
			aMask=0xff000000; //alphi掩码
			rMask=0x00ff0000; //红色掩码
			gMask=0x0000ff00; //绿色掩码
			bMask=0x000000ff; //兰色掩码

			rMove = 16;
			gMove = 8;
			break;

		case D3DFMT_X8R8G8B8:
			// 32-bit RGB pixel format, where 8 bits are reserved for each color.
			nColorDepth = 32;

			//=======32位转换============
			cMask=0x00FFFFFF; //取24位
			aMask=0xff000000; //alphi掩码
			rMask=0x00ff0000; //红色掩码
			gMask=0x0000ff00; //绿色掩码
			bMask=0x000000ff; //兰色掩码

			rMove = 16;
			gMove = 8;
			break;

		case D3DFMT_A1R5G5B5:
			// 16-bit pixel format where 5 bits are reserved for each color 
			// and 1 bit is reserved for alpha.
			nColorDepth = 16;

			//=======16位转换============
			rMask=0x03ff; //红色掩码
			gMask=0x03E0; //绿色掩码
			bMask=0x001F; //兰色掩码

			rMove = 10;
			gMove = 5;
			break;

		case D3DFMT_X1R5G5B5:
			// 16-bit pixel format where 5 bits are reserved for each color.
			nColorDepth = 16;

			//=======16位转换============
			rMask=0x03ff; //红色掩码
			gMask=0x03E0; //绿色掩码
			bMask=0x001F; //兰色掩码

			rMove = 10;
			gMove = 5;
			break;

		case D3DFMT_R5G6B5:
			// 16-bit RGB pixel format with 5 bits for red, 6 bits for green, 
			// and 5 bits for blue.
			nColorDepth = 16;

			//=======16位转换============
			rMask=0x07ff; //红色掩码
			gMask=0x07E0; //绿色掩码
			bMask=0x001F; //兰色掩码

			rMove = 11;
			gMove = 6;
			break;

		default:
			m_Texture->UnlockRect(0);
			//targetsuffer->Release();
			return TRUE;
	}

	linearPitchSrc = lockedRectBackBufferSrc.Pitch;

	

	if( nColorDepth == 16 )
	{
		USHORT *surfaceBufferSrc = static_cast<USHORT*>(lockedRectBackBufferSrc.pBits);
		linearPitchSrc = (linearPitchSrc>>1);

		for( int j = 0; j < srcRect->bottom-srcRect->top; j++)
			for( int i = 0; i < srcRect->right-srcRect->left; i++ )
			{
				sour =  (USHORT)surfaceBufferSrc[i+j*linearPitchSrc];
				r=(sour>>rMove);
				g=(sour & gMask) >>gMove;
				b=sour & bMask;
				t=(r*3+g*6+b)/10;
				surfaceBufferSrc[i+j*linearPitchSrc] = (USHORT)((t<<rMove)|(t<<gMove)|t);
			}
	}
	else
		if( nColorDepth == 32 )
		{
			UINT *surfaceBufferSrc = static_cast<UINT*>(lockedRectBackBufferSrc.pBits);
			linearPitchSrc = (linearPitchSrc>>2);

			for( int j = 0; j < srcRect->bottom-srcRect->top; j++)
			for( int i = 0; i < srcRect->right-srcRect->left; i++ )
				{
				/*	sour = surfaceBufferSrc[i+j*linearPitchSrc] & cMask;

					r=(sour>>rMove);
					g=(sour & gMask) >>gMove;
					b=sour & bMask;
					t=(r*3+g*6+b)/10;*/
					if(0x00000000 != surfaceBufferSrc[i+j*linearPitchSrc])
					surfaceBufferSrc[i+j*linearPitchSrc] = 0x99010101;
				}
		}
	
	m_Texture->UnlockRect(0);
	


	return TRUE;
}


BOOL cTexture::Free()
{
  ReleaseCOM(m_Texture);
  m_Graphics = NULL;

  return TRUE;
}

BOOL cTexture::IsLoaded()
{
  if(m_Texture == NULL)
    return FALSE;
  return TRUE;
}

inline IDirect3DTexture9 *cTexture::GetTextureCOM()
{
  return m_Texture;
}

long cTexture::GetWidth()
{
  D3DSURFACE_DESC d3dsd;

  if(m_Texture == NULL)
    return 0;

  if(FAILED(m_Texture->GetLevelDesc(0, &d3dsd)))
    return 0;

  return d3dsd.Width;
}

long cTexture::GetHeight()
{
  D3DSURFACE_DESC d3dsd;

  if(m_Texture == NULL)
    return 0;

  if(FAILED(m_Texture->GetLevelDesc(0, &d3dsd)))
    return 0;

  return d3dsd.Height;
}

D3DFORMAT cTexture::GetFormat()
{
  D3DSURFACE_DESC d3dsd;

  if(m_Texture == NULL)
    return D3DFMT_UNKNOWN;

  if(FAILED(m_Texture->GetLevelDesc(0, &d3dsd)))
    return D3DFMT_UNKNOWN;

  return d3dsd.Format;
}




BOOL cTexture::Blit(long DestX, long DestY,                   \
                    long SrcX, long SrcY,                     \
                    long Width, long Height,                  \
                    D3DCOLOR Color)
{
  RECT Rect;

  if(m_Texture == NULL)
    return FALSE;
  if(m_Graphics == NULL)
    return FALSE;
  if((m_Graphics->GetSpriteCOM()) == NULL)
    return FALSE;



  Rect.left = SrcX;
  Rect.top  = SrcY;
  Rect.right = Rect.left + Width;
  Rect.bottom = Rect.top + Height;

  m_Graphics->BeginSprite(D3DXSPRITE_ALPHABLEND);
  if(FAILED(m_Graphics->GetSpriteCOM()->Draw(m_Texture,                          \
	  &Rect, &D3DXVECTOR3(0.0f, 0.0f, 0.0f),                 \
	  &D3DXVECTOR3((float)DestX, (float)DestY, 0.0f),            \
	  Color))){
		  MessageBox(NULL, TEXT("Sprite DRaw False!!!"), TEXT(""), MB_OK);
		  m_Graphics->EndSprite();
		  return FALSE;
	  }
  m_Graphics->EndSprite();
  return TRUE;
}


BOOL cTexture::Blit(CONST RECT * pSrcRect,long DestX, long DestY,  D3DCOLOR Color)
{



  if(m_Texture == NULL)
    return FALSE;
  if(m_Graphics == NULL)
    return FALSE;
  if((m_Graphics->GetSpriteCOM()) == NULL)
    return FALSE;



  m_Graphics->BeginSprite(D3DXSPRITE_ALPHABLEND);
  if(FAILED(m_Graphics->GetSpriteCOM()->Draw(m_Texture,                          \
	  pSrcRect, &D3DXVECTOR3(0.0f, 0.0f, 0.0f),                 \
	  &D3DXVECTOR3((float)DestX, (float)DestY, 0.0f),            \
	  Color))){
		  MessageBox(NULL, TEXT("Sprite DRaw False!!!"), TEXT(""), MB_OK);
		  m_Graphics->EndSprite();
		  return FALSE;
	  }
  m_Graphics->EndSprite();
  return TRUE;
}



BOOL cTexture::Blit(CONST RECT * pSrcRect,CONST D3DXVECTOR3 * pCenter, long DestX, long DestY,  D3DCOLOR Color)
{



  if(m_Texture == NULL)
    return FALSE;
  if(m_Graphics == NULL)
    return FALSE;
  if((m_Graphics->GetSpriteCOM()) == NULL)
    return FALSE;



  m_Graphics->BeginSprite(D3DXSPRITE_ALPHABLEND);
  if(FAILED(m_Graphics->GetSpriteCOM()->Draw(m_Texture,                          \
	  pSrcRect, pCenter,                 \
	  &D3DXVECTOR3((float)DestX, (float)DestY, 0.0f),            \
	  Color))){
		  MessageBox(NULL, TEXT("Sprite DRaw False!!!"), TEXT(""), MB_OK);
		  m_Graphics->EndSprite();
		  return FALSE;
	  }
  m_Graphics->EndSprite();
  return TRUE;
}




cSprite::cSprite(void)
{
}

cSprite::~cSprite(void)
{

}

cSprite & cSprite::operator = (const cSprite & s)
{
	if(this == &s)return *this;
	
	m_bActive		= s.m_bActive;
	m_nFrameWidth	= s.m_nFrameWidth;
	m_nFrameHeight	= s.m_nFrameHeight;
	m_nFrameBegin	= s.m_nFrameBegin;
	m_nFrameEnd		= s.m_nFrameEnd	;	
	m_nCurrentFrame	= s.m_nCurrentFrame;	
	
	m_nPosX			= s.m_nPosX	;		
	m_nPosY			= s.m_nPosY	;

	m_pTexture = s.m_pTexture;

	return *this;

}

// 重置所有变量值
void cSprite::zeroSpriteValues(void)
{
	m_bActive		= false;
	m_nFrameWidth	= 0;
	m_nFrameHeight	= 0;
	m_nFrameBegin	= 0;
	m_nFrameEnd		= 0;
	m_nCurrentFrame	= 0;

	m_nPosX			= 0;
	m_nPosY			= 0;

	m_pTexture = NULL;
}

// 绑定纹理
bool cSprite::SetTexture(cTexture *pTexture)
{
	m_pTexture = pTexture;
	return false;
}

// 绘制精灵
void cSprite::DrawSprite(cGraphics *Graphics, float a)
{
	if(m_bActive)
	{
		ID3DXSprite *pSprite;
		if(!Graphics) 
			return;
		if((pSprite = Graphics->GetSpriteCOM()) == NULL)
			return;
		if(m_pTexture == NULL )
			return;


		pSprite->Begin( D3DXSPRITE_ALPHABLEND );

		pSprite->Draw( m_pTexture->GetTextureCOM(),
			&CRect(0, 0, m_nFrameWidth, m_nFrameHeight),
			NULL,
			&D3DXVECTOR3(m_nPosX, m_nPosY, 0.0f ),
			D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,a) );
		pSprite->End();
	}

}


// 循环一次
void cSprite::LoopIt(void)
{
	if(m_bActive)
	{
		m_nCurrentFrame ++;
		if(m_nCurrentFrame >= m_nFrameEnd)
		{
			m_nCurrentFrame = m_nFrameBegin;
			m_bActive = false;
		}
	}
}


cLight::cLight()
{
  // Set a default light to point
  ZeroMemory(&m_Light, sizeof(D3DLIGHT9));
  SetType(D3DLIGHT_POINT);
  Move(0.0f, 0.0f, 0.0f);
  SetDiffuseColor((char)255,(char)255,(char)255);
  SetAmbientColor((char)255,(char)255,(char)255);
  SetRange(1000.0f);
}

BOOL cLight::SetType(D3DLIGHTTYPE Type)
{
  m_Light.Type = Type;
  return TRUE;
}

BOOL cLight::Move(float XPos, float YPos, float ZPos)
{
  m_Light.Position.x = XPos;
  m_Light.Position.y = YPos;
  m_Light.Position.z = ZPos;
  return TRUE;
}

BOOL cLight::MoveRel(float XPos, float YPos, float ZPos)
{
  m_Light.Position.x += XPos;
  m_Light.Position.y += YPos;
  m_Light.Position.z += ZPos;
  return TRUE;
}

BOOL cLight::GetPos(float *XPos, float *YPos, float *ZPos)
{
  if(XPos != NULL)
    *XPos = m_Light.Position.x;
  if(YPos != NULL)
    *YPos = m_Light.Position.y;
  if(ZPos != NULL)
    *ZPos = m_Light.Position.z;
  return TRUE;
}

BOOL cLight::Point(float XFrom, float YFrom, float ZFrom,
                   float XAt,   float YAt,   float ZAt)
{
  D3DXVECTOR3 vecSrc;
  D3DXVECTOR3 vecDest;

  // Move the light
  Move(XFrom, YFrom, ZFrom);

  // Calculate vector between angles
  m_Light.Direction.x = XAt - XFrom;
  m_Light.Direction.y = YAt - YFrom;
  m_Light.Direction.z = ZAt - ZFrom;

  return TRUE;
}

BOOL cLight::GetDirection(float *XDir, float *YDir, float *ZDir)
{
  if(XDir != NULL)
    *XDir = m_Light.Direction.x;
  if(YDir != NULL)
    *YDir = m_Light.Direction.y;
  if(ZDir != NULL)
    *ZDir = m_Light.Direction.z;
  return TRUE;
}

BOOL cLight::SetDiffuseColor(char Red, char Green, char Blue)
{
  m_Light.Diffuse.r = 1.0f / 255.0f * (float)Red;
  m_Light.Diffuse.g = 1.0f / 255.0f * (float)Green;
  m_Light.Diffuse.b = 1.0f / 255.0f * (float)Blue;

  return TRUE;
}

BOOL cLight::GetDiffuseColor(char *Red, char *Green, char *Blue)
{
  if(Red != NULL)
    *Red = (char)(255.0f * m_Light.Diffuse.r);

  if(Green != NULL)
    *Green = (char)(255.0f * m_Light.Diffuse.g);

  if(Blue != NULL)
    *Blue = (char)(255.0f * m_Light.Diffuse.b);

  return TRUE;
}

BOOL cLight::SetSpecularColor(char Red, char Green, char Blue)
{
  m_Light.Specular.r = 1.0f / 255.0f * (float)Red;
  m_Light.Specular.g = 1.0f / 255.0f * (float)Green;
  m_Light.Specular.b = 1.0f / 255.0f * (float)Blue;

  return TRUE;
}

BOOL cLight::GetSpecularColor(char *Red, char *Green, char *Blue)
{
  if(Red != NULL)
    *Red = (char)(255.0f * m_Light.Specular.r);

  if(Green != NULL)
    *Green = (char)(255.0f * m_Light.Specular.g);

  if(Blue != NULL)
    *Blue = (char)(255.0f * m_Light.Specular.b);

  return TRUE;
}

BOOL cLight::SetAmbientColor(char Red, char Green, char Blue)
{
  m_Light.Ambient.r = 1.0f / 255.0f * (float)Red;
  m_Light.Ambient.g = 1.0f / 255.0f * (float)Green;
  m_Light.Ambient.b = 1.0f / 255.0f * (float)Blue;

  return TRUE;
}

BOOL cLight::GetAmbientColor(char *Red, char *Green, char *Blue)
{
  if(Red != NULL)
    *Red = (char)(255.0f * m_Light.Ambient.r);

  if(Green != NULL)
    *Green = (char)(255.0f * m_Light.Ambient.g);

  if(Blue != NULL)
    *Blue = (char)(255.0f * m_Light.Ambient.b);

  return TRUE;
}

BOOL cLight::SetRange(float Range)
{
  m_Light.Range = Range;
  return TRUE;
}

float cLight::GetRange()
{
  return m_Light.Range;
}

BOOL cLight::SetFalloff(float Falloff)
{
  m_Light.Falloff = Falloff;
  return TRUE;
}

float cLight::GetFalloff()
{
  return m_Light.Falloff;
}

BOOL cLight::SetAttenuation0(float Attenuation)
{
  m_Light.Attenuation0 = Attenuation;
  return TRUE;
}

float cLight::GetAttenuation0()
{
  return m_Light.Attenuation0;
}

BOOL cLight::SetAttenuation1(float Attenuation)
{
  m_Light.Attenuation1 = Attenuation;
  return TRUE;
}

float cLight::GetAttenuation1()
{
  return m_Light.Attenuation1;
}

BOOL cLight::SetAttenuation2(float Attenuation)
{
  m_Light.Attenuation2 = Attenuation;
  return TRUE;
}

float cLight::GetAttenuation2()
{
  return m_Light.Attenuation2;
}

BOOL cLight::SetTheta(float Theta)
{
  m_Light.Theta = Theta;
  return TRUE;
}

float cLight::GetTheta()
{
  return m_Light.Theta;
}

BOOL cLight::SetPhi(float Phi)
{
  m_Light.Phi = Phi;
  return TRUE;
}

float cLight::GetPhi()
{
  return m_Light.Phi;
}

D3DLIGHT9 *cLight::GetLight()
{
  return &m_Light;
}


cFont::cFont()
{
  m_Font = NULL;
}

cFont::~cFont()
{
  Free();
}

ID3DXFont *cFont::GetFontCOM()
{
  return m_Font;
}

BOOL cFont::Create(cGraphics *Graphics, LPCTSTR pFacename, INT Height, UINT Width, BOOL Bold)
{

  if(Graphics == NULL )
    return FALSE;
  if(Graphics->GetDeviceCOM() == NULL)
    return FALSE;


  if(FAILED(D3DXCreateFont( Graphics->GetDeviceCOM(), Height, Width, Bold, 0, FALSE, 
         DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
         DEFAULT_PITCH , pFacename,
		 &m_Font ))){
			  return FALSE;
		 }
  return TRUE;
}

BOOL cFont::Free()
{
  ReleaseCOM(m_Font);
  return TRUE;
}




BOOL cFont::Print(LPCTSTR Text, long XPos, long YPos, long Width, long Height, D3DCOLOR Color, DWORD Format)
{
  RECT Rect;

  if(m_Font == NULL)
    return FALSE;

  if(!Width)
    Width = 65535;
  if(!Height)
    Height = 65536;

  Rect.left   = XPos;
  Rect.top    = YPos;
  Rect.right  = Rect.left + Width;
  Rect.bottom = Rect.top + Height;
  if(FAILED(m_Font->DrawText(NULL, Text, -1, &Rect, Format, Color)))
    return FALSE;
  return TRUE;
}

BOOL cFont::PrintA(LPCSTR Text, long XPos, long YPos, long Width, long Height, D3DCOLOR Color, DWORD Format)
{
  RECT Rect;

  if(m_Font == NULL)
    return FALSE;

  if(!Width)
    Width = 65535;
  if(!Height)
    Height = 65536;

  Rect.left   = XPos;
  Rect.top    = YPos;
  Rect.right  = Rect.left + Width;
  Rect.bottom = Rect.top + Height;
  if(FAILED(m_Font->DrawTextA(NULL, Text, -1, &Rect, Format, Color)))
    return FALSE;
  return TRUE;
}


BOOL cFont::Print(LPCTSTR Text, CRect *Rect, D3DCOLOR Color, DWORD Format)
{


  if(m_Font == NULL)
    return FALSE;




  if(FAILED(m_Font->DrawText(NULL, Text, -1, Rect, Format, Color)))
    return FALSE;
  return TRUE;
}

BOOL cFont::PrintA(LPCSTR Text, CRect *Rect, D3DCOLOR Color, DWORD Format)
{

  if(m_Font == NULL)
    return FALSE;




  if(FAILED(m_Font->DrawTextA(NULL, Text, -1, Rect, Format, Color)))
    return FALSE;
  return TRUE;
}


cSurface::cSurface()
{
	m_Graphics =NULL;
	m_Surface = NULL;

}


cSurface::~cSurface()
{
  Free();
}


BOOL cSurface::CreateSurface(cGraphics *Graphics, UINT Width,UINT Height)
{
	if(!Graphics) 
		return FALSE;
	if(!Graphics->GetDeviceCOM())
		return FALSE;

	m_Graphics = Graphics;
	if ( FAILED(Graphics->GetDeviceCOM()->CreateOffscreenPlainSurface(Width, Height, Graphics->GetBackBufferFormat(), D3DPOOL_DEFAULT, &m_Surface, NULL)))
		return FALSE;


	return TRUE;
}


BOOL cSurface::Free()
{
  ReleaseCOM(m_Surface);
  m_Graphics = NULL;


  return TRUE;
}

BOOL cSurface::GetDesc(D3DSURFACE_DESC * pDesc)
{
  if(!m_Surface) return FALSE;
  m_Surface->GetDesc( pDesc );
  return TRUE;

}

BOOL cSurface::GetSurfaceFromSurface(const RECT* desRect, cSurface* srcSurface, const RECT* srcRect,COLORREF colorkey)
{
	D3DSURFACE_DESC ddsd;
	D3DLOCKED_RECT lockedRectBackBufferSrc;
	D3DLOCKED_RECT lockedRectBackBufferDes;
	IDirect3DSurface9* targetsuffer = NULL; 
	int nColorDepth ;
	int linearPitchSrc =0;
	int linearPitchDes =0;
	IDirect3DDevice9 *pDevice;
	IDirect3DSurface9* src;
	src = srcSurface->GetSurfaceCOM();

	if(!m_Graphics)
		return FALSE;
	if(( pDevice = m_Graphics->GetDeviceCOM()) == NULL)
		return FALSE;
	if(!m_Surface)
		return FALSE;
	if(!src)
		return FALSE;

	pDevice->CreateOffscreenPlainSurface(desRect->right - desRect->left, desRect->bottom - desRect->top, m_Graphics->GetBackBufferFormat(), D3DPOOL_DEFAULT, &targetsuffer, NULL); 

	pDevice->StretchRect(m_Surface, desRect, targetsuffer, CRect(0,0,desRect->right - desRect->left, desRect->bottom - desRect->top),D3DTEXF_NONE);

	src->LockRect( &lockedRectBackBufferSrc, NULL, D3DLOCK_READONLY) ;
	targetsuffer->LockRect( &lockedRectBackBufferDes, NULL, 0 ) ;

	m_Surface->GetDesc( &ddsd );

	switch( ddsd.Format)
	{
		case D3DFMT_R8G8B8 :

			nColorDepth = 24;
			break;

		case D3DFMT_A2R10G10B10:
			// 32-bit pixel format using 10 bits each for red, green, and blue, 
			// and 2 bits for alpha. Used only inn full-screen mode.
			nColorDepth = 32;
			break;

		case D3DFMT_A8R8G8B8:
			// 32-bit ARGB pixel format with alpha, using 8 bits per channel.
			nColorDepth = 32;
			break;

		case D3DFMT_X8R8G8B8:
			// 32-bit RGB pixel format, where 8 bits are reserved for each color.
			nColorDepth = 32;
			break;

		case D3DFMT_A4R4G4B4 :
			nColorDepth = 16;
			break;

		case D3DFMT_A1R5G5B5:
			// 16-bit pixel format where 5 bits are reserved for each color 
			// and 1 bit is reserved for alpha.
			nColorDepth = 16;
			break;

		case D3DFMT_X1R5G5B5:
			// 16-bit pixel format where 5 bits are reserved for each color.
			nColorDepth = 16;
			break;

		case D3DFMT_R5G6B5:
			// 16-bit RGB pixel format with 5 bits for red, 6 bits for green, 
			// and 5 bits for blue.
			nColorDepth = 16;
			break;
		case D3DFMT_A8R3G3B2 :
			nColorDepth = 16;
			break;
		case D3DFMT_X4R4G4B4 :
			nColorDepth = 16;
			break;

		default:
			CString msg;
			msg.Format("%d",ddsd.Format );
			AfxMessageBox(msg);
			break;
	}

	linearPitchSrc = lockedRectBackBufferSrc.Pitch;
	linearPitchDes= lockedRectBackBufferDes.Pitch;

	if( nColorDepth == 16 )
	{
		//colorkey = 0x0000;
		USHORT *surfaceBufferSrc = static_cast<USHORT*>(lockedRectBackBufferSrc.pBits);
		USHORT *surfaceBufferDes = static_cast<USHORT*>(lockedRectBackBufferDes.pBits);
		linearPitchSrc = (linearPitchSrc>>1);
		linearPitchDes=(linearPitchDes>>1);

		for( int j = srcRect->top,m = 0; m < desRect->bottom-desRect->top; j++ ,m++)
			for( int i = srcRect->left,n=0; n < desRect->right-desRect->left; i++ ,n++)
			{
				if(surfaceBufferSrc[i+j*linearPitchSrc] != (USHORT)colorkey)
					surfaceBufferDes[n+m*linearPitchDes] = (USHORT)surfaceBufferSrc[i+j*linearPitchSrc];
			}
	}	
	else
	if( nColorDepth == 24)
	{
		//colorkey = 0x000000;
		UINT *surfaceBufferSrc = static_cast<UINT*>(lockedRectBackBufferSrc.pBits);
		UINT *surfaceBufferDes = static_cast<UINT*>(lockedRectBackBufferDes.pBits);

		for( int j = srcRect->top,m = 0; m < desRect->bottom-desRect->top; j++ ,m++)
			for( int i = srcRect->left,n=0; n < desRect->right-desRect->left; i++ ,n++)
			{
				if(surfaceBufferSrc[i*3+j*lockedRectBackBufferSrc.Pitch] != (USHORT)colorkey)
					surfaceBufferDes[n*3+m*lockedRectBackBufferDes.Pitch] = (USHORT)surfaceBufferSrc[i*3+j*lockedRectBackBufferSrc.Pitch];
			}
	}
	else
	if( nColorDepth == 32 )
	{
		//colorkey = 0x00000000;
		UINT *surfaceBufferSrc = static_cast<UINT*>(lockedRectBackBufferSrc.pBits);
		UINT *surfaceBufferDes = static_cast<UINT*>(lockedRectBackBufferDes.pBits);
		linearPitchSrc = (linearPitchSrc>>2);
		linearPitchDes=(linearPitchDes>>2);
		
		for( int j = srcRect->top,m = 0; m < desRect->bottom-desRect->top; j++ ,m++)
			for( int i = srcRect->left,n=0; n < desRect->right-desRect->left; i++ ,n++)
			{
				if(surfaceBufferSrc[i+j*linearPitchSrc] != colorkey)
					surfaceBufferDes[n+m*linearPitchDes] = surfaceBufferSrc[i+j*linearPitchSrc];
			}
	}

	targetsuffer->UnlockRect();
	src->UnlockRect();

	pDevice->StretchRect(targetsuffer, CRect(0,0,desRect->right - desRect->left, desRect->bottom - desRect->top),m_Surface,  desRect, D3DTEXF_NONE);

	targetsuffer->Release();
	return TRUE;
}


BOOL cSurface::LoadSurfaceFromFile(cGraphics *Graphics, TCHAR *Filename, UINT Width, UINT Height, D3DCOLOR ColorKey)
{
	HRESULT hResult; 
	if(!Graphics)
		return FALSE;
	if(Graphics->GetDeviceCOM() == NULL)
		return FALSE;
	if(Filename == NULL)
		return FALSE;

	Free();

	hResult = Graphics->GetDeviceCOM()->CreateOffscreenPlainSurface(Width, Height, Graphics->GetBackBufferFormat(), D3DPOOL_DEFAULT, &m_Surface, NULL); 
	if (FAILED(hResult)) 
		return NULL;


	if(FAILED(D3DXLoadSurfaceFromFile(m_Surface, 
		NULL, 
		NULL,Filename,
		NULL, D3DX_FILTER_NONE,
		D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,0.0f),
		NULL))){
			MessageBox(NULL, TEXT("从文件中加载图片失败!"), TEXT(""), MB_OK);
			return FALSE;
		}

	m_Graphics = Graphics;
	return TRUE;
}



BOOL cSurface::LoadSurfaceFromResource(cGraphics *Graphics, LONG RCID, UINT Width, UINT Height, D3DCOLOR ColorKey, TCHAR* strRes)
{
	HRESULT hr;
	HMODULE hModule = NULL;
	HRSRC rsrc;
	HGLOBAL hgData;
	LPVOID pvData;
	DWORD cbData;
	IDirect3DSurface9* surface = NULL; 

	ASSERT(Graphics!=NULL);
	ASSERT(Graphics->GetDeviceCOM() != NULL);
	if(!Graphics)
		return FALSE;
	if(Graphics->GetDeviceCOM() == NULL)
		return FALSE;


	Free();
	//Graphics->GetBackBufferFormat()

	hr = Graphics->GetDeviceCOM()->CreateOffscreenPlainSurface(Width, Height, Graphics->GetBackBufferFormat(), D3DPOOL_DEFAULT, &m_Surface, NULL); 
	if (FAILED(hr)) 
		return FALSE;


	if( FAILED(D3DXLoadSurfaceFromResource(m_Surface, 
		NULL, 
		NULL,GetModuleHandle( NULL ),
		MAKEINTRESOURCE(RCID),
		NULL, D3DX_FILTER_NONE,
		ColorKey,
		NULL))){



			rsrc = FindResource( hModule, MAKEINTRESOURCE(RCID), strRes );
			if( rsrc != NULL )
			{
				cbData = SizeofResource( hModule, rsrc );
				if( cbData > 0 )
				{
					hgData = LoadResource( hModule, rsrc );
					if( hgData != NULL )
					{
						pvData = LockResource( hgData );
						if( pvData != NULL )
						{
							if( FAILED( hr = D3DXLoadSurfaceFromFileInMemory( 
								m_Surface, NULL, NULL, pvData, cbData,
								NULL, D3DX_FILTER_NONE,
								ColorKey,
								NULL) ) )
							{
								MessageBox(NULL, TEXT("不能从内存中加载此资源！"), TEXT(""), MB_OK);
								return E_FAIL;
							}
						} else {
							MessageBox(NULL, TEXT("不能锁住资源！"), TEXT(""), MB_OK);
							return E_FAIL;
						}
					} else {
						MessageBox(NULL, TEXT("不能加载资源！"), TEXT(""), MB_OK);
						return E_FAIL;
					}
				} else {
					MessageBox(NULL, TEXT("资源大小为0！"), TEXT(""), MB_OK);
					return E_FAIL;
				}
			} else {
				MessageBox(NULL, TEXT("不能找到资源！"), TEXT(""), MB_OK);
				return E_FAIL;
			}
		} 

		m_Graphics = Graphics;
		return S_OK;
}




BOOL cSurface::LoadSurfaceFromResource(const RECT *pDestRect, LONG RCID, const RECT *pSrcRect, D3DCOLOR ColorKey, TCHAR* strRes)
{
	HRESULT hr;
	HMODULE hModule = NULL;
	HRSRC rsrc;
	HGLOBAL hgData;
	LPVOID pvData;
	DWORD cbData;


	if(!m_Surface) return FALSE;
	if(!m_Graphics) return FALSE;


	if( FAILED(D3DXLoadSurfaceFromResource(m_Surface, 
		NULL, 
		pDestRect,GetModuleHandle( NULL ),
		MAKEINTRESOURCE(RCID),
		pSrcRect, D3DX_FILTER_NONE,
		ColorKey,
		NULL))){



			rsrc = FindResource( hModule, MAKEINTRESOURCE(RCID), strRes );
			if( rsrc != NULL )
			{
				cbData = SizeofResource( hModule, rsrc );
				if( cbData > 0 )
				{
					hgData = LoadResource( hModule, rsrc );
					if( hgData != NULL )
					{
						pvData = LockResource( hgData );
						if( pvData != NULL )
						{
							if( FAILED( hr = D3DXLoadSurfaceFromFileInMemory( 
								m_Surface, NULL, pDestRect, pvData, cbData,
								pSrcRect, D3DX_FILTER_NONE,
								ColorKey,
								NULL) ) )
							{
								MessageBox(NULL, TEXT("不能从内存中加载此资源！"), TEXT(""), MB_OK);
								return E_FAIL;
							}
						} else {
							MessageBox(NULL, TEXT("不能锁住资源！"), TEXT(""), MB_OK);
							return E_FAIL;
						}
					} else {
						MessageBox(NULL, TEXT("不能加载资源！"), TEXT(""), MB_OK);
						return E_FAIL;
					}
				} else {
					MessageBox(NULL, TEXT("资源大小为0！"), TEXT(""), MB_OK);
					return E_FAIL;
				}
			} else {
				MessageBox(NULL, TEXT("不能找到资源！"), TEXT(""), MB_OK);
				return E_FAIL;
			}
		} 


		return S_OK;
}


BOOL cSurface::CopySurface(cSurface* srcSurface,CONST RECT* pSourceRect,CONST RECT* pDestRect)
{
	ASSERT(m_Graphics!=NULL);
	ASSERT(m_Graphics->GetDeviceCOM() != NULL);
	if(!m_Graphics)
		return FALSE;
	if(m_Graphics->GetDeviceCOM() == NULL)
		return FALSE;

	
	if( FAILED(m_Graphics->GetDeviceCOM()->StretchRect(srcSurface->GetSurfaceCOM(), pSourceRect, m_Surface, pDestRect,D3DTEXF_NONE)))
		return FALSE;
	return TRUE;

}


BOOL cSurface::GreySurface(const RECT* srcRect)
{
	D3DSURFACE_DESC ddsd;
	D3DLOCKED_RECT lockedRectBackBufferSrc;
	//双缓冲画图
	IDirect3DSurface9* targetsuffer = NULL; 
	int nColorDepth ;
	int linearPitchSrc =0;
	//=======位转换============
	int cMask; //取24位
	int aMask; //alphi掩码
	int rMask; //红色掩码
	int gMask; //绿色掩码
	int bMask; //兰色掩码

	int rMove;
	int gMove;
	IDirect3DDevice9 *pDevice;

	if(!m_Graphics)
		return FALSE;
	if((pDevice = m_Graphics->GetDeviceCOM()) == NULL)
		return FALSE;
	if(!m_Surface)
		return FALSE;

	pDevice->CreateOffscreenPlainSurface(srcRect->right - srcRect->left, srcRect->bottom - srcRect->top, m_Graphics->GetBackBufferFormat(), D3DPOOL_DEFAULT, &targetsuffer, NULL); 
	pDevice->StretchRect(m_Surface, srcRect, targetsuffer, CRect(0,0,srcRect->right - srcRect->left, srcRect->bottom - srcRect->top),D3DTEXF_NONE);
	
	int sour,r,g,b,t; //临时变量

	targetsuffer->LockRect( &lockedRectBackBufferSrc, NULL, 0 ) ;

	targetsuffer->GetDesc( &ddsd );

	switch( ddsd.Format )
	{
		case D3DFMT_A2R10G10B10:
			// 32-bit pixel format using 10 bits each for red, green, and blue, 
			// and 2 bits for alpha. Used only inn full-screen mode.
			nColorDepth = 32;

			//=======32位转换============
			cMask=0x03FFFFFF; //取30位
			aMask=0xC0000000; //alphi掩码
			rMask=0x3ff00000; //红色掩码
			gMask=0x000FFC00; //绿色掩码
			bMask=0x000003FF; //兰色掩码

			rMove = 20;
			gMove = 10;
			break;

		case D3DFMT_A8R8G8B8:
			// 32-bit ARGB pixel format with alpha, using 8 bits per channel.
			nColorDepth = 32;
			//=======32位转换============
			cMask=0x00FFFFFF; //取24位
			aMask=0xff000000; //alphi掩码
			rMask=0x00ff0000; //红色掩码
			gMask=0x0000ff00; //绿色掩码
			bMask=0x000000ff; //兰色掩码

			rMove = 16;
			gMove = 8;
			break;

		case D3DFMT_X8R8G8B8:
			// 32-bit RGB pixel format, where 8 bits are reserved for each color.
			nColorDepth = 32;

			//=======32位转换============
			cMask=0x00FFFFFF; //取24位
			aMask=0xff000000; //alphi掩码
			rMask=0x00ff0000; //红色掩码
			gMask=0x0000ff00; //绿色掩码
			bMask=0x000000ff; //兰色掩码

			rMove = 16;
			gMove = 8;
			break;

		case D3DFMT_A1R5G5B5:
			// 16-bit pixel format where 5 bits are reserved for each color 
			// and 1 bit is reserved for alpha.
			nColorDepth = 16;

			//=======16位转换============
			rMask=0x03ff; //红色掩码
			gMask=0x03E0; //绿色掩码
			bMask=0x001F; //兰色掩码

			rMove = 10;
			gMove = 5;
			break;

		case D3DFMT_X1R5G5B5:
			// 16-bit pixel format where 5 bits are reserved for each color.
			nColorDepth = 16;

			//=======16位转换============
			rMask=0x03ff; //红色掩码
			gMask=0x03E0; //绿色掩码
			bMask=0x001F; //兰色掩码

			rMove = 10;
			gMove = 5;
			break;

		case D3DFMT_R5G6B5:
			// 16-bit RGB pixel format with 5 bits for red, 6 bits for green, 
			// and 5 bits for blue.
			nColorDepth = 16;

			//=======16位转换============
			rMask=0x07ff; //红色掩码
			gMask=0x07E0; //绿色掩码
			bMask=0x001F; //兰色掩码

			rMove = 11;
			gMove = 6;
			break;

		default:
			targetsuffer->UnlockRect();
			targetsuffer->Release();
			return TRUE;
	}

	linearPitchSrc = lockedRectBackBufferSrc.Pitch;

	

	if( nColorDepth == 16 )
	{
		USHORT *surfaceBufferSrc = static_cast<USHORT*>(lockedRectBackBufferSrc.pBits);
		linearPitchSrc = (linearPitchSrc>>1);

		for( int j = 0; j < srcRect->bottom-srcRect->top; j++)
			for( int i = 0; i < srcRect->right-srcRect->left; i++ )
			{
				sour =  (USHORT)surfaceBufferSrc[i+j*linearPitchSrc];
				r=(sour>>rMove);
				g=(sour & gMask) >>gMove;
				b=sour & bMask;
				t=(r*3+g*6+b)/10;
				surfaceBufferSrc[i+j*linearPitchSrc] = (USHORT)((t<<rMove)|(t<<gMove)|t);
			}
	}
	else
		if( nColorDepth == 32 )
		{
			UINT *surfaceBufferSrc = static_cast<UINT*>(lockedRectBackBufferSrc.pBits);
			linearPitchSrc = (linearPitchSrc>>2);

			for( int j = 0; j < srcRect->bottom-srcRect->top; j++)
			for( int i = 0; i < srcRect->right-srcRect->left; i++ )
				{
					sour = surfaceBufferSrc[i+j*linearPitchSrc] & cMask;

					r=(sour>>rMove);
					g=(sour & gMask) >>gMove;
					b=sour & bMask;
					t=(r*3+g*6+b)/10;
					surfaceBufferSrc[i+j*linearPitchSrc] = surfaceBufferSrc[i+j*linearPitchSrc]&aMask | (t<<rMove)|(t<<gMove)|t;
				}
		}
	
	targetsuffer->UnlockRect();
	

	pDevice->StretchRect(targetsuffer, CRect(0,0,srcRect->right - srcRect->left, srcRect->bottom - srcRect->top),m_Surface, srcRect, D3DTEXF_NONE);
	targetsuffer->Release();
	return TRUE;
}



BOOL cSurface::BlackSurface(const RECT* srcRect)
{
	D3DSURFACE_DESC ddsd;
	D3DLOCKED_RECT lockedRectBackBufferSrc;
	//双缓冲画图
	IDirect3DSurface9* targetsuffer = NULL; 
	int nColorDepth ;
	int linearPitchSrc =0;
	//=======位转换============
	int cMask; //取24位
	int aMask; //alphi掩码
	int rMask; //红色掩码
	int gMask; //绿色掩码
	int bMask; //兰色掩码

	int rMove;
	int gMove;
	IDirect3DDevice9 *pDevice;

	if(!m_Graphics)
		return FALSE;
	if((pDevice = m_Graphics->GetDeviceCOM()) == NULL)
		return FALSE;
	if(!m_Surface)
		return FALSE;

	pDevice->CreateOffscreenPlainSurface(srcRect->right - srcRect->left, srcRect->bottom - srcRect->top, m_Graphics->GetBackBufferFormat(), D3DPOOL_DEFAULT, &targetsuffer, NULL); 
	pDevice->StretchRect(m_Surface, srcRect, targetsuffer, CRect(0,0,srcRect->right - srcRect->left, srcRect->bottom - srcRect->top),D3DTEXF_NONE);
	
	int sour,r,g,b,t; //临时变量

	targetsuffer->LockRect( &lockedRectBackBufferSrc, NULL, 0 ) ;

	targetsuffer->GetDesc( &ddsd );

	switch( ddsd.Format )
	{
		case D3DFMT_A2R10G10B10:
			// 32-bit pixel format using 10 bits each for red, green, and blue, 
			// and 2 bits for alpha. Used only inn full-screen mode.
			nColorDepth = 32;

			//=======32位转换============
			cMask=0x03FFFFFF; //取30位
			aMask=0xC0000000; //alphi掩码
			rMask=0x3ff00000; //红色掩码
			gMask=0x000FFC00; //绿色掩码
			bMask=0x000003FF; //兰色掩码

			rMove = 20;
			gMove = 10;
			break;

		case D3DFMT_A8R8G8B8:
			// 32-bit ARGB pixel format with alpha, using 8 bits per channel.
			nColorDepth = 32;
			//=======32位转换============
			cMask=0x00FFFFFF; //取24位
			aMask=0xff000000; //alphi掩码
			rMask=0x00ff0000; //红色掩码
			gMask=0x0000ff00; //绿色掩码
			bMask=0x000000ff; //兰色掩码

			rMove = 16;
			gMove = 8;
			break;

		case D3DFMT_X8R8G8B8:
			// 32-bit RGB pixel format, where 8 bits are reserved for each color.
			nColorDepth = 32;

			//=======32位转换============
			cMask=0x00FFFFFF; //取24位
			aMask=0xff000000; //alphi掩码
			rMask=0x00ff0000; //红色掩码
			gMask=0x0000ff00; //绿色掩码
			bMask=0x000000ff; //兰色掩码

			rMove = 16;
			gMove = 8;
			break;

		case D3DFMT_A1R5G5B5:
			// 16-bit pixel format where 5 bits are reserved for each color 
			// and 1 bit is reserved for alpha.
			nColorDepth = 16;

			//=======16位转换============
			rMask=0x03ff; //红色掩码
			gMask=0x03E0; //绿色掩码
			bMask=0x001F; //兰色掩码

			rMove = 10;
			gMove = 5;
			break;

		case D3DFMT_X1R5G5B5:
			// 16-bit pixel format where 5 bits are reserved for each color.
			nColorDepth = 16;

			//=======16位转换============
			rMask=0x03ff; //红色掩码
			gMask=0x03E0; //绿色掩码
			bMask=0x001F; //兰色掩码

			rMove = 10;
			gMove = 5;
			break;

		case D3DFMT_R5G6B5:
			// 16-bit RGB pixel format with 5 bits for red, 6 bits for green, 
			// and 5 bits for blue.
			nColorDepth = 16;

			//=======16位转换============
			rMask=0x07ff; //红色掩码
			gMask=0x07E0; //绿色掩码
			bMask=0x001F; //兰色掩码

			rMove = 11;
			gMove = 6;
			break;

		default:
			targetsuffer->UnlockRect();
			targetsuffer->Release();
			return TRUE;
	}

	linearPitchSrc = lockedRectBackBufferSrc.Pitch;

	

	if( nColorDepth == 16 )
	{
		USHORT *surfaceBufferSrc = static_cast<USHORT*>(lockedRectBackBufferSrc.pBits);
		linearPitchSrc = (linearPitchSrc>>1);

		for( int j = 0; j < srcRect->bottom-srcRect->top; j++)
			for( int i = 0; i < srcRect->right-srcRect->left; i++ )
			{
				sour =  (USHORT)surfaceBufferSrc[i+j*linearPitchSrc];
				r=(sour>>rMove);
				g=(sour & gMask) >>gMove;
				b=sour & bMask;
				t=(r*3+g*6+b)/10;
				surfaceBufferSrc[i+j*linearPitchSrc] = (USHORT)((t<<rMove)|(t<<gMove)|t);
			}
	}
	else
		if( nColorDepth == 32 )
		{
			UINT *surfaceBufferSrc = static_cast<UINT*>(lockedRectBackBufferSrc.pBits);
			linearPitchSrc = (linearPitchSrc>>2);

			for( int j = 0; j < srcRect->bottom-srcRect->top; j++)
			for( int i = 0; i < srcRect->right-srcRect->left; i++ )
				{
				/*	sour = surfaceBufferSrc[i+j*linearPitchSrc] & cMask;

					r=(sour>>rMove);
					g=(sour & gMask) >>gMove;
					b=sour & bMask;
					t=(r*3+g*6+b)/10;*/
					if(0x000000 != surfaceBufferSrc[i+j*linearPitchSrc])
					surfaceBufferSrc[i+j*linearPitchSrc] = 0x00010101;
				}
		}
	
	targetsuffer->UnlockRect();
	

	pDevice->StretchRect(targetsuffer, CRect(0,0,srcRect->right - srcRect->left, srcRect->bottom - srcRect->top),m_Surface, srcRect, D3DTEXF_NONE);
	targetsuffer->Release();
	return TRUE;
}



IDirect3DSurface9 *cSurface::GetSurfaceCOM()
{
	return m_Surface;
}


void cSurface::SetSurface(cGraphics *Graphics, IDirect3DSurface9* Surface)
{
	m_Graphics = Graphics;
	m_Surface = Surface;
}




cPointSprite::cPointSprite(void)
{
	zeroSpriteValues();
}

cPointSprite::~cPointSprite(void)
{
	m_pVertexBuffer->Release();
	m_Texture.Free();

}

// 初始
void cPointSprite::zeroSpriteValues(void)
{
	m_bActive = FALSE;
	m_bFirstFrame = FALSE;
}

float cPointSprite::getRandomMinMax( float fMin, float fMax )
{
    float fRandNum = (float)rand () / RAND_MAX;
    return fMin + (fMax - fMin) * fRandNum;
}
D3DXVECTOR3 cPointSprite::getRandomVector( void )
{
	D3DXVECTOR3 vVector;

    // Pick a random Z between -1.0f and 1.0f.
    vVector.z = getRandomMinMax( -1.0f, 1.0f );
    
    // Get radius of this circle
    float radius = (float)sqrt(1 - vVector.z * vVector.z);
    
    // Pick a random point on a circle.
    float t = getRandomMinMax( -D3DX_PI, D3DX_PI );

    // Compute matching X and Y for our Z.
    vVector.x = (float)cosf(t) * radius;
    vVector.y = (float)sinf(t) * radius;

	return vVector;
}

void cPointSprite::initPointSprites()
{

	m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	//
	// Create a vertex bufer which can be used with point sprites...
	//

     m_pDevice->CreateVertexBuffer( 2048 * sizeof(Vertex), 
                                      D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS, 
									  Vertex::FVF_Flags, D3DPOOL_DEFAULT, 
									  &m_pVertexBuffer, NULL );



	//
	// Initialize our particles so they'll start at the origin with some 
	// random direction and color.
	//

	for( int i = 0; i < MAX_PARTICLES; ++i )
    {
		g_particles[i].m_vCurPos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		g_particles[i].m_vCurVel = getRandomVector() * getRandomMinMax( 0.3f, 7.0f );

		g_particles[i].m_vColor = D3DCOLOR_COLORVALUE( getRandomMinMax( 0.0f, 1.0f ), 
                                                       getRandomMinMax( 0.0f, 1.0f ), 
                                                       getRandomMinMax( 0.0f, 1.0f ), 
                                                       1.0f );
	}
}
bool cPointSprite::updatePointSprites( void )
{
	//
	// To repeat the sample automatically, keep track of the overall app time.
	//

	static DWORD dStartAppTime;
	static DWORD dLastFrameTime;

	if(m_bFirstFrame)
	{
		dStartAppTime = GetTickCount();
		dLastFrameTime = GetTickCount();
		m_bFirstFrame = false;
	}

	DWORD dCurrenFrameTime = GetTickCount();
	DWORD fElpasedAppTime = dCurrenFrameTime - dStartAppTime;

	//
	// To move the particles via their velocity, keep track of how much time
	// has elapsed since last frame update...
	//

	float dElpasedFrameTime = (float)((dCurrenFrameTime - dLastFrameTime) * 0.001);
	dLastFrameTime = dCurrenFrameTime;

	//
	// After 5 seconds, repeat the sample by returning all the particles 
	// back to the origin.
	//

    if( fElpasedAppTime >= 4000 )
	{
		srand((UINT)time(0));
		Off();		
		return false;
	}
	
	//
	// Move each particle via its velocity and elapsed frame time.
	//
	
	for( int i = 0; i < MAX_PARTICLES; ++i )
		g_particles[i].m_vCurPos += g_particles[i].m_vCurVel * dElpasedFrameTime;
	return true;

}

void cPointSprite::renderPointSprites( void )
{
	//
	// Setting D3DRS_ZWRITEENABLE to FALSE makes the Z-Buffer read-only, which 
	// helps remove graphical artifacts generated from  rendering a list of 
	// particles that haven't been sorted by distance to the eye.
	//
    // Setting D3DRS_ALPHABLENDENABLE to TRUE allows particles, which overlap, 
	// to alpha blend with each other correctly.
	//

    m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

    m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

	//
	// Set up the render states for using point sprites...
	//

    m_pDevice->SetRenderState( D3DRS_POINTSPRITEENABLE, TRUE );    // Turn on point sprites
    m_pDevice->SetRenderState( D3DRS_POINTSCALEENABLE,  TRUE );    // Allow sprites to be scaled with distance
    m_pDevice->SetRenderState( D3DRS_POINTSIZE,     FtoDW(1.0) );  // Float value that specifies the size to use for point size computation in cases where point size is not specified for each vertex.
    m_pDevice->SetRenderState( D3DRS_POINTSIZE_MIN, FtoDW(1.0f) ); // Float value that specifies the minimum size of point primitives. Point primitives are clamped to this size during rendering. 
    m_pDevice->SetRenderState( D3DRS_POINTSCALE_A,  FtoDW(0.0f) ); // Default 1.0
    m_pDevice->SetRenderState( D3DRS_POINTSCALE_B,  FtoDW(0.0f) ); // Default 0.0
    m_pDevice->SetRenderState( D3DRS_POINTSCALE_C,  FtoDW(1.0f) ); // Default 0.0

	//
	// Lock the vertex buffer, and set up our point sprites in accordance with 
	// our particles that we're keeping track of in our application.
	//

	Vertex *pPointVertices;

	m_pVertexBuffer->Lock( 0, MAX_PARTICLES * sizeof(Vertex),
		                   (void**)&pPointVertices, D3DLOCK_DISCARD );

	for( int i = 0; i < MAX_PARTICLES; ++i )
    {
        pPointVertices->posit = g_particles[i].m_vCurPos;
        pPointVertices->color = g_particles[i].m_vColor;
        pPointVertices++;
    }

    m_pVertexBuffer->Unlock();
	
	//
	// Render point sprites...
	//

    m_pDevice->SetStreamSource( 0, m_pVertexBuffer, 0, sizeof(Vertex) );
    m_pDevice->SetFVF( Vertex::FVF_Flags );
	m_pDevice->DrawPrimitive( D3DPT_POINTLIST, 0, MAX_PARTICLES );

	//
    // Reset render states...
	//
	
    m_pDevice->SetRenderState( D3DRS_POINTSPRITEENABLE, FALSE );
    m_pDevice->SetRenderState( D3DRS_POINTSCALEENABLE,  FALSE );

    m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
    m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
}
void cPointSprite::DrawPointSprite(void)
{
	if(!m_bActive)
		return;
	D3DXMATRIX matTrans;
	D3DXMATRIX matRot;
	D3DXMATRIX matWorld;

    D3DXMatrixTranslation( &matTrans, 0.0f, 0.0f, 10.0f );

	/*
	//旋转
	D3DXMatrixRotationYawPitchRoll( &matRot, 
		                            D3DXToRadian(0.0f), 
		                            D3DXToRadian(0.0f), 
		                            0.0f );
									*/
    //matWorld = matRot * matTrans;

	matWorld = matTrans;
    m_pDevice->SetTransform( D3DTS_WORLD, &matWorld );

	if(!updatePointSprites())
		return;

	m_pDevice->SetTexture( 0, m_Texture.GetTextureCOM() );

	renderPointSprites();
}


// 设置顶点精灵参数
void cPointSprite::SetPointSprite(cGraphics *Graphics,  LONG RCID, TCHAR* strRes, UINT Width,  UINT Height, D3DCOLOR ColorKey)
{
	//
	// Load up the point sprite's texture...
	//
	if(!Graphics) return;

	m_Texture.LoadTextureFromResource(Graphics, RCID, NULL, Width, Height);
	m_pDevice = Graphics->GetDeviceCOM();

	initPointSprites();
}

// 开按钮
void cPointSprite::On(void)
{
	m_bActive = TRUE;
	m_bFirstFrame = TRUE;

}

// 关按钮
void cPointSprite::Off(void)
{
	m_bActive = FALSE;
	m_bFirstFrame = FALSE;

	for( int i = 0; i < MAX_PARTICLES; ++i )
	{
		g_particles[i].m_vCurPos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		g_particles[i].m_vCurVel = getRandomVector() * getRandomMinMax( 0.3f, 7.0f );

		g_particles[i].m_vColor = D3DCOLOR_COLORVALUE( getRandomMinMax( 0.0f, 1.0f ), 
													getRandomMinMax( 0.0f, 1.0f ), 
													getRandomMinMax( 0.0f, 1.0f ), 
													1.0f );
	}
}