#ifndef SKIN_RENDER_MANAGER_HEAD_FILE
#define SKIN_RENDER_MANAGER_HEAD_FILE

#pragma once

#include "SkinControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//Ĭ��ɫ��
#define DEF_COLOR_H					203									//Ĭ��ɫ��
#define DEF_COLOR_S					0.0									//Ĭ�ϱ���
#define DEF_COLOR_B					0.0									//Ĭ������

//RGB ��ɫ
struct COLORRGB
{
	BYTE							R;									//��ɫɫ��
	BYTE							G;									//��ɫ����
	BYTE							B;									//��ɫ����
};

//HSB ��ɫ
struct COLORHSB
{
	WORD							H;									//��ɫɫ��
	DOUBLE							S;									//��ɫ����
	DOUBLE							B;									//��ɫ����
};

//��Ⱦ��Ϣ
struct tagSkinRenderInfo
{
	WORD							wColorH;							//��ɫɫ��
	DOUBLE							dRectifyS;							//��ɫ����
	DOUBLE							dRectifyB;							//��ɫ����
};

//////////////////////////////////////////////////////////////////////////////////

//��Ⱦ����
class SKIN_CONTROL_CLASS CSkinRenderManager
{
	//��������
protected:
	tagSkinRenderInfo				m_SkinRenderInfo;					//��Ⱦ��Ϣ

	//��������
public:
	//���캯��
	CSkinRenderManager();
	//��������
	virtual ~CSkinRenderManager();

	//��̬����
protected:
	static CSkinRenderManager *		m_pSkinRenderManager;				//����ָ��

	//��Դת��
public:
	//��Ⱦ����
	VOID RenderImage(CDC * pDC);
	//��Ⱦ��Դ
	VOID RenderImage(CImage & ImageSource);
	//��Ⱦ��ɫ
	COLORREF RenderColor(DOUBLE S, DOUBLE B);

	//��ɫת��
public:
	//��ɫת��
	inline COLORHSB RGBToHSB(COLORREF crColor);
	//��ɫת��
	inline COLORRGB HSBToRGB(WORD H, DOUBLE S, DOUBLE B);
	//��ɫ�ϳ�
	inline COLORRGB CompoundColor(WORD H, DOUBLE S, DOUBLE B);

	//��Ⱦ״̬
public:
	//��ȡ��ɫ
	tagSkinRenderInfo GetSkinRenderInfo() { return m_SkinRenderInfo; }
	//������ɫ
	VOID SetSkinRenderInfo(const tagSkinRenderInfo & SkinRenderInfo) { m_SkinRenderInfo=SkinRenderInfo; }

	//��̬����
public:
	//��ȡʵ��
	static CSkinRenderManager * GetInstance() { return m_pSkinRenderManager; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif