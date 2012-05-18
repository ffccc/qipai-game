#ifndef VIRTUAL_BUTTON_HEAD_FILE
#define VIRTUAL_BUTTON_HEAD_FILE

#pragma once

#include "D3DTexture.h"
#include "LapseCount.h"
#include "VirtualWindow.h"
#include "GameEngineHead.h"

//////////////////////////////////////////////////////////////////////////////////

//���ⰴť
class GAME_ENGINE_CLASS CVirtualButton : public CVirtualWindow
{
	//״̬����
protected:
	bool							m_bMouseDown;						//���±�־
	bool							m_bMouseMove;						//������־
	CRect							m_rcButtonRect;						//��ť����

	//��Դ����
protected:
	LPCTSTR							m_pszTypeName;						//��Դ����
	LPCTSTR							m_pszResource;						//��Դ��Ϣ
	HINSTANCE						m_hResInstance;						//��Դ���

	//��������
protected:
	WORD							m_wImageIndex;						//��������
	CLapseCount						m_ImageLapseCount;					//���ż���

	//��Դ����
protected:
	CD3DTexture						m_D3DTextureButton;					//��ť����

	//��������
public:
	//���캯��
	CVirtualButton();
	//��������
	virtual ~CVirtualButton();

	//ϵͳ�¼�
protected:
	//������Ϣ
	virtual VOID OnWindowMovie();
	//������Ϣ
	virtual VOID OnWindowCreate(CD3DDevice * pD3DDevice);
	//������Ϣ
	virtual VOID OnWindowDestory(CD3DDevice * pD3DDevice);

	//���غ���
protected:
	//����¼�
	virtual VOID OnEventMouse(UINT uMessage, UINT nFlags, INT nXMousePos, INT nYMousePos);
	//��ť�¼�
	virtual VOID OnEventButton(UINT uButtonID, UINT uMessage, INT nXMousePos, INT nYMousePos);
	//�滭����
	virtual VOID OnEventDrawWindow(CD3DDevice * pD3DDevice, INT nXOriginPos, INT nYOriginPos);

	//���ܺ���
public:
	//��������
	VOID SetButtonRect(CRect rcButtonRect);
	//����λͼ
	VOID SetButtonImage(CD3DDevice * pD3DDevice, LPCTSTR pszResource, LPCTSTR pszTypeName, HINSTANCE hResInstance);

	//�ڲ�����
protected:
	//�����ؼ�
	VOID RectifyControl(CD3DDevice * pD3DDevice);
};

//////////////////////////////////////////////////////////////////////////////////

#endif