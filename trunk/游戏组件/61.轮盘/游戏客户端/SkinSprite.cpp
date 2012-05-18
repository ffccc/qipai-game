#include "StdAfx.h"
#include "SkinSprite.h"


//////////////////////////////////////////////////////////////////////////////////////

//���캯��
CSkinSprite::CSkinSprite()
{
	m_Sprite=NULL;
	m_pTexture=NULL;
	offsetX=0;offsetY=0;
	m_StartX=0;m_StartY=0;m_StopX=0;m_StopY=0;//�ƶ�·��
	ZeroMemory(&m_ImageInfo,sizeof(m_ImageInfo));
}

//��������
CSkinSprite::~CSkinSprite()
{
	SAFE_RELEASE(m_Sprite);
	SAFE_RELEASE(m_pTexture);
}

//////////////////////////////////////////////////////////////////////////////////////
//�ͷź���
void CSkinSprite::Release()
{
	SAFE_RELEASE(m_Sprite);
	SAFE_RELEASE(m_pTexture);
}

//���غ���
void CSkinSprite::SetLoadInfo(LPDIRECT3DDEVICE9	pD3DDevice,LPCTSTR szFileName,D3DCOLOR dwKeyColor)
{
	//��������
	D3DXCreateSprite( pD3DDevice, &m_Sprite);
	//��������
	D3DXCreateTextureFromFileEx(pD3DDevice,	szFileName, 0, 0, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_FILTER_NONE,D3DX_DEFAULT,	dwKeyColor, NULL, NULL,
		&m_pTexture);

	if(m_Sprite==NULL||m_pTexture==NULL) throw 0 ;
	//��ȡ��Ϣ
	D3DXGetImageInfoFromFile(szFileName, &m_ImageInfo);
}
//���غ���
void CSkinSprite::SetLoadInfo(LPDIRECT3DDEVICE9	pD3DDevice,UINT uResourceID,D3DCOLOR dwKeyColor)
{
	//��������
	D3DXCreateSprite( pD3DDevice, &m_Sprite);
	//��������
	D3DXCreateTextureFromResourceEx(pD3DDevice,	NULL,MAKEINTRESOURCE(uResourceID), 0, 0, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_FILTER_NONE,D3DX_DEFAULT,	dwKeyColor, NULL, NULL,
		&m_pTexture);
	if(m_Sprite==NULL||m_pTexture==NULL) throw 0 ;
	//��ȡ��Ϣ
	D3DXGetImageInfoFromResource(GetModuleHandle(NULL),MAKEINTRESOURCE(uResourceID),&m_ImageInfo);
}

//����ͼƬ����·��
void CSkinSprite::SetMovePath(int StartX,int StartY,int StopX,int StopY)
{
	//���ñ���
	m_StartX=StartX;	m_StartY=StartY;
	m_StopX=StopX;		m_StopY=StopY;
	Setoffset();
	return ;
}
//�����ƶ�����
void CSkinSprite::Setoffset()
{
	//����Ĭ��ֵ
	offsetX=5;
	offsetY=5;	

	//����ƫ����
	int xp=abs(m_StopX-m_StartX);//xƫ����ֵ
	int yp=abs(m_StopY-m_StartY);//yƫ����ֵ

	//��x��y�����ϵ��ƶ�����ͬʱ�����趨����
	if(xp!=yp&&xp!=0&&yp!=0)
	{
		//ASSERT(FALSE);
		int longRange=xp>yp?xp:yp;//������
		int shortRange=xp>yp?yp:xp;//�̾���

		int longTime=longRange/5;//�ƶ�����������ʱ��

		int xspeed=xp/longTime;//x�ٶ�
		int yspeed=yp/longTime;//y�ٶ�

		offsetX=xspeed;
		offsetY=yspeed;
	}
}
//�滭�ƶ�����
bool CSkinSprite::DrawMoveSprite()
{
	//ͼƬδ�ƶ���ָ��λ�� ����false
	if(m_StartX!=m_StopX||m_StartY!=m_StopY)
	{
		if(m_StartX<m_StopX) m_StartX+=offsetX;
		if(m_StartX>m_StopX) m_StartX-=offsetX;
		if(m_StartY<m_StopY) m_StartY+=offsetY;
		if(m_StartY>m_StopY) m_StartY-=offsetY;
		DrawSprite(m_StartX,m_StartY);
		return false;
	}
	//��Ⱦ����λ��
	DrawSprite(m_StopX,m_StopY);
	//��Ⱦ��ɷ���true
	return true;
}
//�滭����
void CSkinSprite::DrawSprite(int x,int y,D3DCOLOR channelColor)
{
	//�滭λ��
	D3DXVECTOR3 drawAtPoint((float)x,(float)y,0);
	m_Sprite->Begin(D3DXSPRITE_ALPHABLEND);
	m_Sprite->Draw(m_pTexture, CRect(0,0,m_ImageInfo.Width,m_ImageInfo.Height), NULL, &drawAtPoint, channelColor);
	m_Sprite->Flush();
	m_Sprite->End();
}

//�滭����
void CSkinSprite::DrawSprite(int x,int y,D3DXVECTOR3 vCenterPoint,CRect drawRect,D3DXMATRIX *matRot)
{
	//�滭λ��
	D3DXVECTOR3 drawAtPoint((float)x,(float)y,0);
	m_Sprite->Begin(D3DXSPRITE_ALPHABLEND);
	m_Sprite->SetTransform(matRot);
	m_Sprite->Draw(m_pTexture, drawRect, &vCenterPoint, &drawAtPoint, 0xffffffff);
	m_Sprite->Flush();
	m_Sprite->End();
}

//HRESULT Draw(
//����LPDIRECT3DTEXTURE9 pTexture,
//����CONST RECT *pSrcRect,
//����CONST D3DXVECTOR3 *pCenter,
//����CONST D3DXVECTOR3 *pPosition,
//����D3DCOLOR Color );
//
//pTexture ����Ҫ���Ƶ���ͼ
//pSrcRect ����Ҫ���Ƶ���ͼ�ϵ�һ����������, ����������ͼ����ָ��ΪNULL.
//pCenter �ǻ��Ƶ���������(��תʱ���Դ˵�Ϊ����), ָ��NULL��ʾ��������Ϊ(0,0,0)
//pPosition �ǻ��Ƶ�λ������, Ҳ����ָ��NULL��ʾ(0,0,0)
//Color �ǻ��Ƶ���ɫ, һ�������ָ��Ϊ 0xffffffff. ���λ����λ�ĸ�8�ֽ�ΪAlpha, ��, ��, ��, ���ָ�� 0x80ffffff���ǰ�͸����ͼ. 
//���0xffff0000��ֻ������ͼ��ĺ�ɫ����, ���幦���Լ����. ��Ȼ��ͼ������԰���Alpha��Ϣ.
//��Ҫ��ת�ȹ��ܿ���ʹ�� pSprite->SetTransform(), ����ԭ��Ϊ:
//HRESULT SetTransform( CONST D3DXMATRIX *pTransform );
//////////////////////////////////////////////////////////////////////////////////////

