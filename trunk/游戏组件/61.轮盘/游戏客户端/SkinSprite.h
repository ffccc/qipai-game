#ifndef SKIN_SPRITE_HEAD_FILE
#define SKIN_SPRITE_HEAD_FILE

#pragma once

#include <d3d9.h>
#include <d3dx9.h>

////////////////////////////////////////////////////////////////////////////////

//dxͼƬ��ʾ
class CSkinSprite
{
	//���б���
private:
	ID3DXSprite*		m_Sprite;	//����
	IDirect3DTexture9*	m_pTexture;	//����
	D3DXIMAGE_INFO		m_ImageInfo;//ͼƬ��Ϣ

	//�ƶ�����
protected:
	int m_StartX,m_StartY,m_StopX,m_StopY;	//ͼƬ�ƶ�·��
	int offsetX,offsetY;					//ͼƬ�ƶ�����

	//��������
public:
	CSkinSprite();
	~CSkinSprite();

	//���ܺ���
public:
	//���غ���
	void SetLoadInfo(LPDIRECT3DDEVICE9	pD3DDevice,LPCTSTR szFileName,D3DCOLOR dwKeyColor=D3DCOLOR_XRGB(255, 0, 255));
	void SetLoadInfo(LPDIRECT3DDEVICE9	pD3DDevice,UINT uResourceID,D3DCOLOR dwKeyColor=D3DCOLOR_XRGB(255, 0, 255));
	//�ͷź���
	void Release();
	//�滭����
	void DrawSprite(int x=0,int y=0,D3DCOLOR channelColor=0xffffffff);
	void DrawSprite(int x,int y,D3DXVECTOR3 vCenterPoint, CRect drawRect,D3DXMATRIX *matRot=NULL);
	//�滭�ƶ�����
	bool DrawMoveSprite();
public:
	//����ͼƬ���
	int GetWidth()	{return m_ImageInfo.Width;}
	//����ͼƬ�߶�
	int GetHeight(){return m_ImageInfo.Height;}
	//���㲽��
	void Setoffset();
	//�����ƶ�·��
	void SetMovePath(int StartX,int StartY,int StopX,int StopY);

	bool IsLoaded(){return m_Sprite!=NULL;}
};


////////////////////////////////////////////////////////////////////////////////
#endif