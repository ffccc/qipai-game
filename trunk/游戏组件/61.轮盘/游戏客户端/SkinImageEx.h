#ifndef SKIN_IMAGE_EX_HEAD_FILE
#define SKIN_IMAGE_EX_HEAD_FILE

#pragma once

#include <d3d9.h>
#include <d3dx9.h>

//dxͼƬ��ʾ
class CSkinImageEx
{
	//�ڲ�����
private:	
	int 				m_nImageWidth;			//ͼƬ��
	int 				m_nImageHeight;			//ͼƬ��
	int 				m_nScreenWidth;			//��Ļ��
	int 				m_nScreenHeight;		//��Ļ��
	DWORD				m_dwColour;				//͸����ɫ
	D3DXIMAGE_INFO		m_ImageInfo;//ͼƬ��Ϣ
	struct				PANEL_CUSTOMVERTEX		//FVF �����ʽ
	{
		FLOAT x, y, z;		//��ά����
		DWORD colour;		//��ɫ
		FLOAT u, v;			//��������
	};

	//dx�豸
private:	
	LPDIRECT3DTEXTURE9			m_pTexture;			//ͼƬ����
	LPDIRECT3DDEVICE9			m_pD3DDevice;		//�豸����
	LPDIRECT3DVERTEXBUFFER9		m_pVertexBuffer;	//���㻺��

	//�ڲ�����
private:
	//���¶���
	bool UpdateVertices();	
	//�������㻺��
	bool CreateVertexBuffer();	
	//����ƽ�������
	void SetupImageCamera();
	//ͼƬλ���ƶ�����
	void MoveTo(int x, int y);

	//��������
public:
	//���캯��
	CSkinImageEx(void);
	//��������
	virtual ~CSkinImageEx(void);

	//���ܺ���
public:
	//�Ƿ����ü�����Ϣ
	bool IsSetLoadInfo();
	//������ز���
	bool RemoveLoadInfo();
	//��ȡ���ز���
	bool GetLoadInfo();
	//���ü��ز���
	bool SetLoadInfo(LPDIRECT3DDEVICE9 pD3DDevice, LPCTSTR pszFileName, int ScreenWidth,int ScreenHeight);
	//���ü��ز���
	bool SetLoadInfo(LPDIRECT3DDEVICE9 pD3DDevice, UINT uResourceID, int ScreenWidth,int ScreenHeight,HINSTANCE hResourceDLL=NULL);
	//�ͷź���
	void Release();
	//��Ⱦ����
	void AlphaDrawImage(int x,int y);

public:
	//����ͼƬ���
	int GetWidth()	{return m_nImageWidth;}
	//����ͼƬ�߶�
	int GetHeight(){return m_nImageHeight;}
};

class CSkinSurface
{
	//�ڲ�����
private:
	LPDIRECT3DSURFACE9			m_Surface;			//�������
	LPDIRECT3DDEVICE9			m_pD3DDevice;		//�豸����
	D3DXIMAGE_INFO				m_ImageInfo;		//ͼ����Ϣ
	IDirect3DSurface9*			m_backbuffer;		//��̨����
	//��������
public:
	//���캯��
	CSkinSurface(void);
	//��������
	virtual ~CSkinSurface(void);

	//���ܺ���
public:
	//���ü��ز���
	bool SetLoadInfo(LPDIRECT3DDEVICE9 pD3DDevice, LPCTSTR pszFileName,D3DFORMAT Format=D3DFMT_X8R8G8B8);
	//���ü��ز���
	bool SetLoadInfo(LPDIRECT3DDEVICE9 pD3DDevice, UINT uResourceID,D3DFORMAT Format=D3DFMT_X8R8G8B8);
	//�ͷź���
	void Release();
	//�滭����
	void DrawSurface(int x=0,int y=0,bool drawAsBackGround=false);
	//ȡ��ͼƬ���
	int GetWidth(){return m_ImageInfo.Width;};
	//ȡ��ͼƬ�߶�
	int GetHeight(){return m_ImageInfo.Height;};
};
//////////////////////////////////////////////////////////////////////////

#endif