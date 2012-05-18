#include "stdafx.h"
#include "DxDialog.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDxDialog, CWnd)
	//ON_WM_PAINT()
	ON_MESSAGE(WM_DX_RENDER, OnFrameRender)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

static bool		bD3DSucess;						//DX��ʼ���ɹ���־

//////////////////////////////////////////////////////////////////////////

//���캯��
CDxDialog::CDxDialog()
{
}
//��������
CDxDialog::~CDxDialog()
{
}

void CDxDialog::OnPaint()
{
	CPaintDC dc(this);
	if(g_pD3DDevice==NULL) return ;
	HRESULT hResult=g_pD3DDevice->TestCooperativeLevel();
	if (hResult==D3DERR_DEVICELOST) return ;
	if (hResult==D3DERR_DEVICENOTRESET)
	{
		UpdateDxWindow(d3dpp.BackBufferWidth,d3dpp.BackBufferHeight);
	}
	OnRender() ;
	Invalidate(true);
}

//��Ⱦ�߳�
void DxThreadRender(LPVOID pThreadData)
{
	//Ч�����
	ASSERT(pThreadData!=NULL);
	if(pThreadData==NULL) return;
	
	while(bD3DSucess)
	{	
		Sleep(5);	
		SendMessage((HWND)pThreadData, WM_DX_RENDER, 0, 0) ;
	}
}

//��Ⱦ��Ϣ
LRESULT CDxDialog::OnFrameRender(WPARAM wParam, LPARAM lParam)
{
	if(g_pD3DDevice==NULL) return 0;
	HRESULT hResult=g_pD3DDevice->TestCooperativeLevel();
	if (hResult==D3DERR_DEVICELOST) return 0;
	if (hResult==D3DERR_DEVICENOTRESET)
	{
		UpdateDxWindow(d3dpp.BackBufferWidth,d3dpp.BackBufferHeight);
	}
	OnRender() ;
	return 0 ;
}

//dx���ڳ�ʼ��
bool CDxDialog::InitDxWindow(int nWidth,int nHeight)
{
	//�Ѿ��������Ļ�����
	if(bD3DSucess==true) return true;

	//���ʵ��
	g_hWnd=this->GetSafeHwnd();
	if(g_hWnd==NULL) return false;

	//��ʼ��d3d
	try{
		//ȡ��DX�汾,��ʾģʽ
		g_pD3D=Direct3DCreate9(D3D_SDK_VERSION);
		g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);


		//������ʾ����
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.Windowed=true;										//�Ƿ񴰿�ģʽ��ʾ
		d3dpp.BackBufferFormat=d3ddm.Format;						//�󻺳���ģʽ
		d3dpp.SwapEffect=D3DSWAPEFFECT_DISCARD;						//������������(ֱ�ӷ���)
		d3dpp.BackBufferWidth=nWidth;								//�󻺳��
		d3dpp.BackBufferHeight=nHeight;								//�󻺳��
		d3dpp.hDeviceWindow=g_hWnd;									//���ھ��
		d3dpp.FullScreen_RefreshRateInHz=D3DPRESENT_RATE_DEFAULT;	//��ʾ��ˢ����
		d3dpp.PresentationInterval=D3DPRESENT_INTERVAL_IMMEDIATE;	//ͼ�����ˢ���ٶ�
		d3dpp.AutoDepthStencilFormat=D3DFMT_D16;					//��Ȼ����ʽ
		d3dpp.EnableAutoDepthStencil=TRUE;							//������Ȼ���

		CString xx;xx.Format("%d",d3dpp.BackBufferHeight);
		//AfxMessageBox(xx);
		//�����豸
		g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice);
		if(g_pD3DDevice==NULL) return false;

		//������Ⱦ�߳�
		UINT uThreadID=0;
		bD3DSucess = TRUE;
		m_hDxThread=(HANDLE)::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)DxThreadRender,LPVOID(this->m_hWnd),0,0);
	}catch(...){}

	//////////////////////////////////////////////////////////////////////////////////////
	//�����豸
	//�򿪻�����
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING,TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_AMBIENT,0xffffffff);

	//����Ȼ���
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESSEQUAL);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);

	//ʹ��Alpha������
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);		
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA );

	//�رյƹ�
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING,FALSE);
	//////////////////////////////////////////////////////////////////////////////////////

	//��������
	D3DXCreateFont(g_pD3DDevice,12,0,0,1,0,GB2312_CHARSET,0,0,0,TEXT("����"),&g_pFont);	g_pFont->AddRef();
 	//��ʼ������
	g_UserControl.InitDXInput(AfxGetInstanceHandle(),g_hWnd);
	//�������
	g_Camera.CreateCamera(g_pD3DDevice);
	//��ʼ������
	InitScene(nWidth,nHeight);
	return (g_hWnd!=NULL);
}
//���´���
bool CDxDialog::UpdateDxWindow(int nWidth,int nHeight)
{
	//���ڴ�С�ı���Ҫ��������d3d�豸�����ʼ������
	//�ͷ��豸
	ReleaseScene();
 	g_Camera.Release();
	g_UserControl.Release();
	SAFE_RELEASE(g_pFont);
	SAFE_RELEASE(g_pD3DDevice);
	if(g_pD3D) { 
		 int xx=(g_pD3D)->Release(); 
		 (g_pD3D)=NULL; }

	try{
		g_pD3D=Direct3DCreate9(D3D_SDK_VERSION);
		g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

		//������ʾ����
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.Windowed=true;										//�Ƿ񴰿�ģʽ��ʾ
		d3dpp.BackBufferFormat=d3ddm.Format;						//�󻺳���ģʽ
		d3dpp.SwapEffect=D3DSWAPEFFECT_DISCARD;						//������������(ֱ�ӷ���)
		d3dpp.BackBufferWidth=nWidth;								//�󻺳��
		d3dpp.BackBufferHeight=nHeight;								//�󻺳��
		d3dpp.hDeviceWindow=g_hWnd;									//���ھ��
		d3dpp.FullScreen_RefreshRateInHz=D3DPRESENT_RATE_DEFAULT;	//��ʾ��ˢ����
		d3dpp.PresentationInterval=D3DPRESENT_INTERVAL_IMMEDIATE;	//ͼ�����ˢ���ٶ�
		d3dpp.AutoDepthStencilFormat=D3DFMT_D16;					//��Ȼ����ʽ
		d3dpp.EnableAutoDepthStencil=TRUE;							//������Ȼ���

		//�����豸
		g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice);
		if(g_pD3DDevice==NULL) return false;
	}catch(...){}


	//////////////////////////////////////////////////////////////////////////////////////
	//�����ڴ������豸����vertexbuffer��ʽ�滭��͸��Ч��
	//�ƹ�
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING,TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_AMBIENT,0xffffffff);

	//����Ȼ���
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESSEQUAL);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);

	//ʹ��Alpha������
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);		
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA );

	//�رյƹ�
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING,FALSE);
	//////////////////////////////////////////////////////////////////////////////////////

	//��������
	D3DXCreateFont(g_pD3DDevice,12,0,0,1,0,GB2312_CHARSET,0,0,0,TEXT("����"),&g_pFont);g_pFont->AddRef();
	//��ʼ������
	g_UserControl.InitDXInput(AfxGetInstanceHandle(),g_hWnd);
	//�������
	g_Camera.CreateCamera(g_pD3DDevice);
	//��ʼ������
	InitScene(nWidth,nHeight);
	return true;
}

//�������
void CDxDialog::DxDrawText(LPCTSTR pText,int x,int y,D3DCOLOR rgbFontColor)
{
	if(g_pFont==NULL) return;

	RECT Rect;
	Rect.left = x;	Rect.top = y;
	Rect.right = 0;	Rect.bottom = 0;
	g_pFont->DrawText(NULL,pText, -1, &Rect, DT_CALCRECT, 0);					//ͨ��������������ռ�õľ��δ�С
	g_pFont->DrawText(NULL,pText, -1, &Rect, DT_LEFT, rgbFontColor|0xFF000000);	//Ȼ������ı�
}

//�û�����
void CDxDialog::UpDateControl()
{
	if(&g_UserControl==NULL) return;

	//��������
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

	//���ý���λ��
	g_Camera.SetFocus(&D3DXVECTOR3(0,0,0));
}

//��Ⱦ���� �ͻ��˿�������onrender();������дDrawGameViewEx()
void CDxDialog::OnRender()
{	
	if(bD3DSucess==false) return;
	//��������
	if((GetTickCount()-g_Time)<10) return;
	g_Time=GetTickCount();

#ifdef _DEBUG
	//��ʼʱ��
	starttime=GetTickCount();
	float fps=1000/(float)(starttime-endtime);
#endif

	//���¿���
	UpDateControl();

	if(g_pD3DDevice==NULL) return;

	//����󱸻�����
	g_pD3DDevice->Clear(0,NULL,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(0,0,0),1.0f,0);
	//��ʼ���Ƴ���
	g_pD3DDevice->BeginScene();

	g_Camera.Update2DMatrix();
	DrawGameView2D();
	g_Camera.Update3DMatrix();
	DrawGameView3D();

#ifdef _DEBUG
	TCHAR szBuffer[256];
	_snprintf(szBuffer,sizeof(szBuffer),TEXT("fps�� %3.1f"),fps);
	DxDrawText(szBuffer,d3dpp.BackBufferWidth/2+35,d3dpp.BackBufferHeight/2-329,RGB(0,0,0));
#endif

	//��������
	g_pD3DDevice->EndScene();
	//��ҳ
	g_pD3DDevice->Present(NULL,NULL,NULL,NULL);

#ifdef _DEBUG
	//����ʱ��
	endtime=GetTickCount();
#endif
}


void CDxDialog::StopRender()
{
	bD3DSucess=false;
}
