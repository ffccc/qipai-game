#pragma once
#include "d3d9.h"
#include <d3dx9.h>
#include "d3dx9math.h"


//λͼ��FVF��ʽ��RHW���꣬������ɫ����������
struct CUSTOMVERTEX
{
	float x,y,z,rhw;
	DWORD color;
	float tu,tv;			//��������
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)


class CD3DImagePro
{
public:
	CD3DImagePro(void);
	~CD3DImagePro(void);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVB; //ͼƬ���㻺����ָ��
	LPDIRECT3DTEXTURE9 m_pTexture;	//λͼ���ε��������ӿ�ָ��
	
	CUSTOMVERTEX vertices[4];     //���㻺��
	//����
public:
	int x, y;//��¼��ǰͼƬ���λ��
public:
	//���ļ�����ͼƬ
	void CreateVertexBuffer(LPDIRECT3DDEVICE9 m_ppDevice,float nx,float ny,float Width,float Height);
	//����ͼƬλ��
	void MoveImage(float x, float y, float width ,float height);
	// ���»���
	void Update(void);
	// ����
	void BitBlt(LPDIRECT3DDEVICE9 m_ppDevice);
	//��������
	void SetTexture(LPDIRECT3DTEXTURE9 pTexture,float u, float v, float nu, float nv);
	//�������� u ���Ͻ�x, v ���Ͻ�y, nu ���½�x ,nv ���½�y
	void SetTexture(float u, float v, float nu, float nv);
	//��������
	void CopyTexture(CD3DImagePro &img);
	// ��ȡ��������ָ��
	LPDIRECT3DTEXTURE9 GetTexture(void);
	// ��������
	void ClearTexture(void);
	// ͼ���Ƿ�Ϊ��
	bool IsNull(void);
};
