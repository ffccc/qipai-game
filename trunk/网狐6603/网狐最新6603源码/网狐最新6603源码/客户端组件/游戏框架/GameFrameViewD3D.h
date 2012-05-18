#ifndef GAME_FRAME_VIEW_D3D_HEAD_FILE
#define GAME_FRAME_VIEW_D3D_HEAD_FILE

#pragma once

#include "GameFrameHead.h"
#include "GameFrameView.h"

//////////////////////////////////////////////////////////////////////////////////

//��ͼ���
class GAME_FRAME_CLASS CGameFrameViewD3D : public CGameFrameView, public ID3DDeviceSink
{
	//״̬����
protected:
	bool							m_bInitD3D;							//������־
	HANDLE 							m_hRenderThread;					//��Ⱦ�߳�

	//��������
protected:
	CRect							m_RectDirtySurface;					//�ػ�����

	//֡��ͳ��
protected:
	DWORD							m_dwDrawBenchmark;					//��׼���
	DWORD							m_dwDrawLastCount;					//�滭����
	DWORD							m_dwDrawCurrentCount;				//�滭����

	//��Դ����
protected:
	CD3DTexture						m_D3DTextureReady;					//׼������
	CD3DTexture						m_D3DTextureMember;					//��Ա����
	CD3DTexture						m_D3DTextureClockItem;				//ʱ������
	CD3DTexture						m_D3DTextureClockBack;				//ʱ������

	//�������
public:
	CD3DFont						m_D3DFont;							//�������
	CD3DDirect						m_D3DDirect;						//��������
	CD3DDevice						m_D3DDevice;						//�豸����
	CVirtualEngine					m_VirtualEngine;					//��������

	//��������
public:
	//���캯��
	CGameFrameViewD3D();
	//��������
	virtual ~CGameFrameViewD3D();

	//���غ���
public:
	//���ý���
	virtual VOID ResetGameView();
	//�����ؼ�
	virtual VOID RectifyControl(INT nWidth, INT nHeight);
	//�������
	virtual VOID InvalidGameView(INT nXPos, INT nYPos, INT nWidth, INT nHeight);

	//���溯��
private:
	//��������
	virtual VOID CartoonMovie()=NULL;
	//���ý���
	virtual VOID InitGameView(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight)=NULL;
	//�滭����
	virtual VOID DrawGameView(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight)=NULL;

	//�豸�ӿ�
protected:
	//�����豸
	virtual VOID OnInitDevice(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight);
	//��ʧ�豸
	virtual VOID OnLostDevice(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight);
	//�����豸
	virtual VOID OnResetDevice(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight);
	//��Ⱦ����
	virtual VOID OnRenderWindow(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight);

	//���غ���
protected:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//Ĭ�ϻص�
	virtual LRESULT DefWindowProc(UINT nMessage, WPARAM wParam, LPARAM lParam);

	//���ܺ���
public:
	//��Ⱦ�߳�
	VOID StartRenderThread();

	//��ϷԪ��
public:
	//�滭׼��
	VOID DrawUserReady(CD3DDevice * pD3DDevice, INT nXPos, INT nYPos);
	//�滭ʱ��
	VOID DrawUserClock(CD3DDevice * pD3DDevice, INT nXPos, INT nYPos, WORD wUserClock);
	//�滭��־
	VOID DrawOrderFlag(CD3DDevice * pD3DDevice, INT nXPos, INT nYPos, BYTE cbImageIndex);
	//�滭ͷ��
	VOID DrawUserAvatar(CD3DDevice * pD3DDevice, INT nXPos, INT nYPos, IClientUserItem * pIClientUserItem);

	//��ͼ����
public:
	//�滭����
	VOID DrawViewImage(CD3DDevice * pD3DDevice, CD3DTexture & D3DTexture, BYTE cbDrawMode);
	//�滭����
	VOID DrawNumberString(CD3DDevice * pD3DDevice, CD3DTexture & D3DTexture, LONG lNumber, INT nXPos, INT nYPos);

	//��ͼ����
public:
	//�滭����
	VOID DrawViewImage(CD3DDevice * pD3DDevice, CD3DSprite & D3DSprite, BYTE cbDrawMode);
	//�滭����
	VOID DrawNumberString(CD3DDevice * pD3DDevice, CD3DSprite & D3DSprite, LONG lNumber, INT nXPos, INT nYPos);

	//�������
public:
	//�������
	VOID DrawTextString(CD3DDevice * pD3DDevice, LPCTSTR pszString, CRect rcDraw,UINT nFormat, D3DCOLOR D3DColor);
	//�������
	VOID DrawTextString(CD3DDevice * pD3DDevice, LPCTSTR pszString, INT nXPos, INT nYPos, UINT nFormat, D3DCOLOR D3DColor);
	//�滭�ַ�
	VOID DrawTextString(CD3DDevice * pD3DDevice, LPCTSTR pszString, CRect rcDraw, UINT nFormat, D3DCOLOR crText, D3DCOLOR crFrame);
	//�滭�ַ�
	VOID DrawTextString(CD3DDevice * pD3DDevice, LPCTSTR pszString, INT nXPos, INT nYPos, UINT nFormat, D3DCOLOR crText, D3DCOLOR crFrame);

	//��Ϣӳ��
protected:
	//�滭����
	VOID OnPaint();
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);

	//�Զ���Ϣ
protected:
	//������Ϣ
	LRESULT OnMessageD3DStatus(WPARAM wParam, LPARAM lParam);
	//��Ⱦ��Ϣ
	LRESULT OnMessageD3DRender(WPARAM wParam, LPARAM lParam);

	//��̬����
private:
	//��Ⱦ�߳�
	static VOID D3DRenderThread(LPVOID pThreadData);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif