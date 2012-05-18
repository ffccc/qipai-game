#ifndef VIRTUAL_WINDOW_HEAD_FILE
#define VIRTUAL_WINDOW_HEAD_FILE

#pragma once

#include "GameEngineHead.h"
#include "VirtualEngine.h"

//////////////////////////////////////////////////////////////////////////////////

//���ⴰ��
class GAME_ENGINE_CLASS CVirtualWindow
{
	//��Ԫ����
	friend class CVirtualEngine;
	friend class CVirtualButton;

	//��������
protected:
	bool							m_bActive;							//�����־
	bool							m_bEnable;							//���ñ�־
	bool							m_bVisible;							//��ʾ��־

	//���Ա���
protected:
	UINT							m_uWindowID;						//���ڱ�ʶ
	UINT							m_uLayerIndex;						//���ڵȼ�

	//λ�ñ���
protected:
	CSize							m_WindowSize;						//���ڴ�С
	CPoint							m_BenchmarkPos;						//��׼λ��

	//�ں˱���
protected:
	CVirtualWindow *				m_pParentWindow;					//�ϲ㴰��
	CVirtualEngine *				m_pVirtualEngine;					//������
	CVirtualWindowPtrArray			m_VirtualWindowPtrArray;			//��������

	//��������
public:
	//���캯��
	CVirtualWindow();
	//��������
	virtual ~CVirtualWindow();

	//���ڱ�ʶ
public:
	//��ȡ��ʶ
	UINT GetWindowID() { return m_uWindowID; }
	//���ñ�ʶ
	VOID SetWindowID(UINT uWindowID) { m_uWindowID=uWindowID; }

	//���ڵȼ�
public:
	//���õȼ�
	VOID SetLayerIndex(UINT uLayerIndex);
	//��ȡ�ȼ�
	UINT GetLayerIndex() { return m_uLayerIndex; }

	//���Զ���
public:
	//�ϲ㴰��
	CVirtualWindow * GetParentWindow() { return m_pParentWindow; }
	//������
	CVirtualEngine * GetVirtualEngine() { return m_pVirtualEngine; }

	//������
public:
	//ɾ������
	VOID DeleteWindow();
	//�����
	bool ActiveWindow(CRect & rcWindow, DWORD dwStyle, UINT uWindowID, CVirtualEngine * pVirtualEngine, CVirtualWindow * pParentWindow);

	//��������
public:
	//��������
	bool IsWindowActive() { return m_bActive; }
	//��������
	bool IsWindowEnable() { return m_bEnable; }
	//��ʾ����
	bool IsWindowVisible() { return m_bVisible; }

	//���ڿ���
public:
	//��ʾ����
	VOID ShowWindow(bool bVisible);
	//��ֹ����
	VOID EnableWindow(bool bEnable);
	//���´���
	VOID InvalidWindow(bool bCoerce);

	//����λ��
public:
	//���ڴ�С
	VOID GetClientRect(CRect & rcClient);
	//���ڴ�С
	VOID GetWindowRect(CRect & rcWindow);
	//����λ��
	VOID SetWindowPos(INT nXPos, INT nYPos, INT nWidth, INT nHeight, UINT uFlags);

	//���ܺ���
public:
	//��������
	bool IsChildWindow(CVirtualWindow * pVirtualWindow);

	//ϵͳ�¼�
protected:
	//������Ϣ
	virtual VOID OnWindowMovie() { return; }
	//������Ϣ
	virtual VOID OnWindowCreate(CD3DDevice * pD3DDevice) { return; }
	//������Ϣ
	virtual VOID OnWindowDestory(CD3DDevice * pD3DDevice) { return; }
	//λ����Ϣ
	virtual VOID OnWindowPosition(CD3DDevice * pD3DDevice) { return; }

	//���غ���
protected:
	//����¼�
	virtual VOID OnEventMouse(UINT uMessage, UINT nFlags, INT nXMousePos, INT nYMousePos)=NULL;
	//��ť�¼�
	virtual VOID OnEventButton(UINT uButtonID, UINT uMessage, INT nXMousePos, INT nYMousePos)=NULL;
	//�滭����
	virtual VOID OnEventDrawWindow(CD3DDevice * pD3DDevice, INT nXOriginPos, INT nYOriginPos)=NULL;

	//�ڲ�����
private:
	//���ô���
	VOID ResetWindow();
	//�滭����
	VOID OnEventDrawChildWindow(CD3DDevice * pD3DDevice, INT nXOriginPos, INT nYOriginPos);
};

//////////////////////////////////////////////////////////////////////////////////

#endif