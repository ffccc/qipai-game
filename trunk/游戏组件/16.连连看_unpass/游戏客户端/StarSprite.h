#pragma once

struct SingleStar
{
	D3DXVECTOR3 posit;
    D3DCOLOR    color;

	enum FVF
	{
		FVF_Flags = D3DFVF_XYZ|D3DFVF_DIFFUSE
	};
};


class CStarSprite
{
private:
	CPoint		m_SrcPoint;
	CPoint		m_DesPoint;

public:
	CStarSprite(void);
	~CStarSprite(void);

	LPDIRECT3DDEVICE9		m_pDevice;		 //�豸�ӿ�ָ��
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer ;//���㻺����
	LPDIRECT3DTEXTURE9      m_pTexture      ;//����ӿ�

	// �˶�����
	void SetRunPos(CPoint src, CPoint src);

	// ���ƽӿ�
	void DrawPointSprite(void);
	// ��ʼ
	void zeroSpriteValues(void);
	// ���ö��㾫�����
	void SetPointSprite(LPDIRECT3DDEVICE9		pDevice, LONG RCID);
	void initPointSprites( LONG RCID )
};
