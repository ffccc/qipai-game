//////////////////////////////////////////////////////////////////////
//�û������࣬����ֱ�ӻ�ȡ����������Ϣ
//////////////////////////////////////////////////////////////////////
#ifndef DX_CONTROL_HEAD_FILE
#define DX_CONTROL_HEAD_FILE
#pragma once
#define DIRECTINPUT_VERSION 0x0800


#include <dinput.h>
#include <dinputd.h>
#include <d3dx9math.h>

//////////////////////////////////////////////////////////////////////

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif

//////////////////////////////////////////////////////////////////////

//���������
class CDxControl
{
	//�豸����
private:
	LPDIRECTINPUT8			m_pDXInput;			//�����豸
	LPDIRECTINPUTDEVICE8	m_pDXInputKB;		//�����豸
	LPDIRECTINPUTDEVICE8	m_pDXInputMouse;	//����豸

	//״̬����
public:
	DIMOUSESTATE			m_MouseState;		//���״̬
	char					m_strKeyState[256];	//����״̬

public:
	CDxControl();
	~CDxControl();
	//���ܺ���
public:
	//��ʼ������
	LRESULT InitDXInput(HINSTANCE hInst,HWND hWnd);
	//����״̬
	LRESULT UpdateInput();
	//�ͷ���Դ
	void Release();

	//ȡ�غ���
public:
	LONG GetLX() {return m_MouseState.lX;};
	LONG GetLY() {return m_MouseState.lY;};
	LONG GetLZ() {return m_MouseState.lZ;};
};

#endif