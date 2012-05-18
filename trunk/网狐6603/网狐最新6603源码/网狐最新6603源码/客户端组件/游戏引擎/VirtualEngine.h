#ifndef VIRTUAL_ENGINE_HEAD_FILE
#define VIRTUAL_ENGINE_HEAD_FILE

#pragma once

#include "D3DDevice.h"
#include "GameEngineHead.h"

//////////////////////////////////////////////////////////////////////////////////

//��˵��
class CVirtualEngine;
class CVirtualWindow;

//��������
typedef CWHArray<CVirtualWindow *> CVirtualWindowPtrArray;				//��������

//////////////////////////////////////////////////////////////////////////////////

//��������
class GAME_ENGINE_CLASS CVirtualEngine
{
	//��Ԫ����
	friend class CVirtualWindow;

	//���ñ���
protected:
	CD3DDevice *					m_pD3DDevice;						//�豸����

	//״̬����
protected:
	CVirtualWindow *				m_pWindowLeave;						//�뿪����
	CVirtualWindow *				m_pWindowCapture;					//���񴰿�

	//�ں˱���
protected:
	CVirtualWindowPtrArray			m_VirtualWindowPtrArray;			//��������

	//��������
public:
	//���캯��
	CVirtualEngine();
	//��������
	virtual ~CVirtualEngine();

	//���ú���
public:
	//��ȡ�豸
	CD3DDevice * GetD3DDevice() { return m_pD3DDevice; }
	//�����豸
	VOID SetD3DDevice(CD3DDevice * pD3DDevice) { m_pD3DDevice=pD3DDevice; }

	//��������
public:
	//�滭����
	bool DrawWindows(CD3DDevice * pD3DDevice);
	//��Ϣ����
	bool DefWindowProc(UINT uMessage, WPARAM wParam, LPARAM lParam);
	//��Ϣ����
	bool PreTranslateMessage(UINT uMessage, WPARAM wParam, LPARAM lParam);

	//������
public:
	//�뿪ע��
	bool RegisterLeave(CVirtualWindow * pVirtualWindow, bool bRegister);
	//�˻�ע��
	bool RegisterCapture(CVirtualWindow * pVirtualWindow, bool bRegister);

	//���º���
private:
	//���´���
	VOID InvalidWindow();
	//���´���
	VOID InvalidWindow(INT nXPos, INT nYPos, INT nWidth, INT nHeight);

	//��������
private:
	//��ȡ����
	CVirtualWindow * SwitchToWindow(INT nXMousePos, INT nYMousePos);
	//��ȡ����
	CVirtualWindow * SwitchToWindow(CVirtualWindow * pVirtualWindow, INT nXMousePos, INT nYMousePos);
};

//////////////////////////////////////////////////////////////////////////////////

#endif