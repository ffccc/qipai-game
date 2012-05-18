#ifndef D3D_DEVICE_HEAD_FILE
#define D3D_DEVICE_HEAD_FILE

#include "GameEngineHead.h"

//////////////////////////////////////////////////////////////////////////////////

//��˵��
class CD3DDirect;
class CD3DDevice;
class CD3DTexture;

//////////////////////////////////////////////////////////////////////////////////

//�豸�ӿ�
interface ID3DDeviceSink
{
	//�豸�¼�
public:
	//�����豸
	virtual VOID OnInitDevice(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight)=NULL;
	//��ʧ�豸
	virtual VOID OnLostDevice(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight)=NULL;
	//�����豸
	virtual VOID OnResetDevice(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight)=NULL;
	//��Ⱦ����
	virtual VOID OnRenderWindow(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//�豸����
class GAME_ENGINE_CLASS CD3DDevice
{
	//��������
public:
	HWND 							m_hWndDevice;						//�豸����
	CSize							m_SizeWindow;						//���ڴ�С

	//���ñ���
protected:
	bool							m_bMultiThreaded;					//���߳�ֵ

	//�ӿڱ���
protected:
	ID3DDeviceSink *				m_pID3DDeviceSink;					//�ص��ӿ�
	IDirect3DDevice9 *				m_pIDirect3DDevice;					//�豸����

	//��������
public:
	//���캯��
	CD3DDevice();
	//��������
	virtual ~CD3DDevice();

	//��Ϣ����
public:
	//��ȡ����
	HWND GetWndDevice() { return m_hWndDevice; }
	//��ȡ��С
	CSize GetSizeWindow() { return m_SizeWindow; }

	//���ýӿ�
public:
	//����߳�
	VOID SetMultiThreaded(bool bMultiThreaded);

	//��ȡ�ӿ�
public:
	//�ص��ӿ�
	ID3DDeviceSink * GetD3DDeviceSink() { return m_pID3DDeviceSink; }
	//�豸�ӿ�
	IDirect3DDevice9 * GetDirect3DDevice() { return m_pIDirect3DDevice; }

	//���ƺ���
public:
	//�����豸
	bool ResetD3DDevice();
	//��Ⱦ�豸
	bool RenderD3DDevice();
	//�����豸
	bool CreateD3DDevice(HWND hWndDevice, ID3DDeviceSink * pID3DDeviceSink);

	//��Ⱦ����
public:
	//��Ⱦ����
	HRESULT SetRenderState(D3DRENDERSTATETYPE State, DWORD Value);
	//��Ⱦ����
	HRESULT GetRenderState(D3DRENDERSTATETYPE State, DWORD & Value);

	//��亯��
public:
	//�����ɫ
	VOID FillRect(CRect rcRect, D3DCOLOR D3DColor);
	//�����ɫ
	VOID FillRect(INT nXPos, INT nYPos, INT nWidth, INT nHeigth, D3DCOLOR D3DColor);

	//�滭����
public:
	//�滭����
	VOID DrawRect(CRect rcRect, UINT uLineWidth, D3DCOLOR D3DColor);
	//�滭����
	VOID DrawLine(INT nXPos1, INT nYPos1, INT nXPos2, INT nYPos2, UINT uLineWidth, D3DCOLOR D3DColor);

	//�ڲ�����
private:
	//���û���
	VOID InitD3DDevice();
	//��������
	VOID InitPresentParameters(D3DPRESENT_PARAMETERS & PresentParameters, HWND hWnd, INT nWidth, INT nHeight, D3DFORMAT BufferFormat);
};

//////////////////////////////////////////////////////////////////////////////////

#endif