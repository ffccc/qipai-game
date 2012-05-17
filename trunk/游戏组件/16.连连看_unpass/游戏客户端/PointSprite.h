#pragma once
////////------------------------------------------------------------
#include "d3d9.h"
#include <d3dx9.h>
#include "d3dx9math.h"

struct Vertex
{
    D3DXVECTOR3 posit;
    D3DCOLOR    color;

	enum FVF
	{
		FVF_Flags = D3DFVF_XYZ|D3DFVF_DIFFUSE
	};
};

struct Particle
{
    D3DXVECTOR3 m_vCurPos;
    D3DXVECTOR3 m_vCurVel;
	D3DCOLOR    m_vColor;
};

const int MAX_PARTICLES = 100;

inline DWORD FtoDW( FLOAT f ) { return *((DWORD*)&f); }
class CPointSprite
{
public:
	bool	m_bActive;
	bool	m_bFirstFrame;
	Particle g_particles[MAX_PARTICLES];	 //���㾫������

	//--------------------
	LPDIRECT3DDEVICE9		m_pDevice;		 //�豸�ӿ�ָ��
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer ;//���㻺����
	LPDIRECT3DTEXTURE9      m_pTexture      ;//����ӿ�

public:
	CPointSprite(void);
	~CPointSprite(void);

	//----------------
	// �������
	float getRandomMinMax( float fMin, float fMax );
	// �������
	D3DXVECTOR3 getRandomVector( void );
	// ��ʼ�����㾫��
	void initPointSprites(LONG RCID);
	// ���¶���λ��
	bool updatePointSprites( void );
	// ����
	void renderPointSprites( void );
	// ���ƽӿ�
	void DrawPointSprite(void);
	// ��ʼ
	void zeroSpriteValues(void);
	// ���ö��㾫�����
	void SetPointSprite(LPDIRECT3DDEVICE9		pDevice, LONG RCID);
	// ����ť
	void On(void);
	// �ذ�ť
	void Off(void);
};
