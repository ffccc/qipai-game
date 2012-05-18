#include "StdAfx.h"
#include "DxControl.h"
#pragma comment (lib,"dinput8.lib")
#pragma comment (lib,"dxguid.lib")

//////////////////////////////////////////////////////////////////

CDxControl::CDxControl()
{
	m_pDXInputKB=NULL;
	m_pDXInputMouse=NULL;
	m_pDXInput=NULL;
}

CDxControl::~CDxControl()
{

}

//��ʼ������
LRESULT CDxControl::InitDXInput(HINSTANCE hInst,HWND hWnd)
{
	//����DXInput����
	if(DI_OK!=DirectInput8Create(hInst,DIRECTINPUT_VERSION,IID_IDirectInput8,(LPVOID*)&m_pDXInput,NULL))
	{		MessageBox(hWnd,"����DXInput����ʧ��","ERROR",MB_OK);			}

	//���������豸
	if(DI_OK!=m_pDXInput->CreateDevice(GUID_SysKeyboard,&m_pDXInputKB,NULL))
	{		MessageBox(hWnd,"���������豸ʧ��","ERROR",MB_OK);				}
	//���ü������ݸ�ʽ
	if(DI_OK!=m_pDXInputKB->SetDataFormat(&c_dfDIKeyboard))
	{		MessageBox(hWnd,"MISS","���ü������ݸ�ʽʧ��",MB_OK);			}
	m_pDXInputKB->SetCooperativeLevel(hWnd,DISCL_EXCLUSIVE|DISCL_NOWINKEY);
	m_pDXInputKB->Acquire();

	//��������豸
	if(DI_OK!=m_pDXInput->CreateDevice(GUID_SysMouse,&m_pDXInputMouse,NULL))
	{		MessageBox(hWnd,"��������豸ʧ��","ERROR",MB_OK);				}
	//����������ݸ�ʽ
	if(DI_OK!=m_pDXInputMouse->SetDataFormat(&c_dfDIMouse))
	{		MessageBox(hWnd,"����������ݸ�ʽʧ��","ERROR",MB_OK);			}
	m_pDXInputMouse->SetCooperativeLevel(hWnd,DISCL_EXCLUSIVE|DISCL_FOREGROUND);
	m_pDXInputMouse->Acquire();

	return TRUE;
}

//��������
LRESULT CDxControl::UpdateInput()
{
	if(m_pDXInputMouse==NULL||m_pDXInputKB==NULL) return DI_NOTATTACHED;

	//��ȡ���
	if(DI_OK!=m_pDXInputMouse->GetDeviceState(sizeof(m_MouseState),&m_MouseState))
	{	
		memset(&m_MouseState,0,sizeof(m_MouseState));	
		m_pDXInputMouse->Acquire();
	}
	//��ȡ����
	if(DI_OK!=m_pDXInputKB->GetDeviceState(sizeof(m_strKeyState),m_strKeyState))
	{	
		memset(&m_strKeyState,0,sizeof(m_strKeyState));	
		m_pDXInputKB->Acquire();
	}

	return DI_OK;
}

//�ͷ��豸
void CDxControl::Release()
{
	SAFE_RELEASE(m_pDXInputKB);
	SAFE_RELEASE(m_pDXInputMouse);
	SAFE_RELEASE(m_pDXInput);
}