/////////////////////////////////////////
//�����dx����
////////////////////////////////////////
#ifndef DX_DIALOG_HEAD_FILE
#define DX_DIALOG_HEAD_FILE
#pragma once
////////////////////////////////////////////////////////////////////////////////

#include "Stdafx.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "Camera.h"
#include "DxControl.h"

#define WM_DX_RENDER					WM_APP+100

////////////////////////////////////////////////////////////////////////////////

//dx���ڹ�����
class CDxDialog : public CWnd
{
	//��������
private:
	HANDLE 					m_hDxThread;	//�߳̾��
	//Ӧ�ñ���
private:
	D3DDISPLAYMODE			d3ddm;			//��ʾģʽ
	D3DPRESENT_PARAMETERS	d3dpp;			//��ʾ����
	//�豸����
public:
	HWND					g_hWnd;			//d3d���ھ��
	LPD3DXFONT				g_pFont;		//dx����
	LPDIRECT3D9             g_pD3D;			//d3d����
	LPDIRECT3DDEVICE9       g_pD3DDevice;	//d3d�豸����
	//�ؼ�����
public:
	CCamera					g_Camera;		//���
	CDxControl				g_UserControl;	//�û�����
	DWORD					g_Time;
	DWORD					starttime,endtime;
	//��������
public:
	CDxDialog();
	~CDxDialog();

public:
	//��������
	bool InitDxWindow(int nWidth,int nHeight);
	//���´���
	bool UpdateDxWindow(int nWidth,int nHeight);
	//�������
	void DxDrawText(LPCTSTR pText,int x,int y,D3DCOLOR rgbFontColor=D3DCOLOR_XRGB(255,255,255));

	//�̳к���
protected:
	//��Ⱦ����
	virtual void OnRender();
	//�滭2d
	virtual void DrawGameView2D()=NULL;
	//�滭3d
	virtual void DrawGameView3D()=NULL;
	//��ʼ������
	virtual void InitScene(int nWidth,int nHeight)=NULL;
	//�豸����
    virtual void ReleaseScene()=NULL;
	//�������
	virtual void UpDateControl();

	virtual void StopRender();

	//�ڲ�����
private:
	//��Ⱦ��Ϣ
	LRESULT OnFrameRender(WPARAM wParam, LPARAM lParam) ;

	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

#endif
