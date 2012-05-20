#pragma once
//-------------------------
//
//������������
//
//���ߣ�����
//
//ʱ�䣺2005��11��17��
//
//���ܣ����ƶ�����ͼƬ����ײ���
//
//-------------------------


#include "d3d9.h"
#include <d3dx9.h>
#include "d3dx9math.h"

//������
class CSprite
{
	//�ӿ�
public:
	LPDIRECT3DTEXTURE9		m_pTexture;								//λͼ���ε��������ӿ�ָ��
	LPD3DXSPRITE            m_pSprite;								//����ӿ�ָ��

	//����
private:
	int						m_RateModi;								//֡����

	RECT srcRect;
	D3DXVECTOR3 vCenter;
	D3DXVECTOR3 vPosition;
public:
	bool					m_bActive;								//�Ƿ����
	bool					m_bSingleFrame;							//�Ƿ��ǵ�֡
	bool					m_bAutoLoop;							//ѭ������
	bool					m_bMultiPic;							//��̬����

	char					m_sType[25];
	char					m_sActiveLoadPath[30];

	int						m_nFrameRate;							//֡����
	int						m_nFrameWidth;							//֡�Ŀ��
	int						m_nFrameHeight;							//֡�ĸ߶�
	int						m_nFrameBegin;							//��ʼ֡
	int						m_nFrameEnd;							//����֡
	int						m_nLoopCount;							//ѭ��������0Ϊ����ѭ��
	int						m_nWidthCount;							//�������
	int						m_nFrameCount;							//����֡����
	int						m_nCurrentFrame;						//��ǰ֡
	int						m_nSrcPosX;								//ͼƬ��ʼX����
	int						m_nSrcPosY;								//ͼƬ��ʼY����
	float					m_nPosX;								//��Ļx����
	float					m_nPosY;								//��Ļy����

	//u\v
	float					u;
	float					v;
	float					nu;
	float					nv;
	//����
public:
	CSprite(void);
	~CSprite(void);

	CSprite & operator = (const CSprite & s);
	
	void zeroSpriteValues(void);									// �������б���ֵ
	bool SetTexture(LPDIRECT3DTEXTURE9 pTexture);		// ������
	
	void DrawSprite(LPDIRECT3DDEVICE9 pDevice,float a = 1.0f);		// ���ƾ���, aΪ���Ƶİ�͸����
	// ��ײ����
	bool CollisionTest(CSprite & pOtherSprite);
	// ѭ��һ��
	void LoopIt(void);
	// ��̬�������ѭ��
	void ActiveLoopIt(LPDIRECT3DDEVICE9 pDevice);
	// �Ƿ����
	bool IsNull(void);
};
