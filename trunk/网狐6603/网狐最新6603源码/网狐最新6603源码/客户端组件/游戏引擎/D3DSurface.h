#ifndef D3D_SURFACE_HEAD_FILE
#define D3D_SURFACE_HEAD_FILE

#pragma once

#include "D3DDevice.h"
#include "GameEngineHead.h"

//////////////////////////////////////////////////////////////////////////////////

//�������
class GAME_ENGINE_CLASS CD3DSurface
{
	//ͼƬ��Ϣ
public:
	CSize							m_ImageSize;						//ͼƬ��С

	//�ӿڱ���
public:
	LPDIRECT3DSURFACE9				m_pISurface;						//����ӿ�

	//��������
public:
	//���캯��
	CD3DSurface();
	//��������
	virtual ~CD3DSurface();

	//ͼƬ��Ϣ
public:
	//�����ж�
	bool IsNull();
	//ͼƬ��С
	bool GetSurfaceSize(CSize & ImageSize);

	//���ܺ���
public:
	//��������
	bool Destory();
	//���ر���
	bool LoadImage(CD3DDevice * pD3DDevice, HINSTANCE hInstance, LPCTSTR pszResource);

	//�滭����
public:
	//�滭ͼƬ
	bool DrawImage(CD3DDevice * pD3DDevice, INT nXDest, INT nYDest);
	//�滭ͼƬ
	bool DrawImage(CD3DDevice * pD3DDevice, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXSource, INT nYSource);
	//�滭ͼƬ
	bool DrawImage(CD3DDevice * pD3DDevice, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXSource, INT nYSource, INT nSourceWidth, INT nSourceHeight);
};

//////////////////////////////////////////////////////////////////////////////////

#endif