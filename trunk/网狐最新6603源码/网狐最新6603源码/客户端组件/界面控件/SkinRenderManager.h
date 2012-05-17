#ifndef SKIN_RENDER_MANAGER_HEAD_FILE
#define SKIN_RENDER_MANAGER_HEAD_FILE

#pragma once

#include "SkinControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//默认色相
#define DEF_COLOR_H					203									//默认色相
#define DEF_COLOR_S					0.0									//默认饱和
#define DEF_COLOR_B					0.0									//默认亮度

//RGB 颜色
struct COLORRGB
{
	BYTE							R;									//颜色色相
	BYTE							G;									//颜色饱和
	BYTE							B;									//颜色亮度
};

//HSB 颜色
struct COLORHSB
{
	WORD							H;									//颜色色相
	DOUBLE							S;									//颜色饱和
	DOUBLE							B;									//颜色亮度
};

//渲染信息
struct tagSkinRenderInfo
{
	WORD							wColorH;							//颜色色相
	DOUBLE							dRectifyS;							//颜色饱和
	DOUBLE							dRectifyB;							//颜色亮度
};

//////////////////////////////////////////////////////////////////////////////////

//渲染管理
class SKIN_CONTROL_CLASS CSkinRenderManager
{
	//变量定义
protected:
	tagSkinRenderInfo				m_SkinRenderInfo;					//渲染信息

	//函数定义
public:
	//构造函数
	CSkinRenderManager();
	//析构函数
	virtual ~CSkinRenderManager();

	//静态变量
protected:
	static CSkinRenderManager *		m_pSkinRenderManager;				//对象指针

	//资源转换
public:
	//渲染界面
	VOID RenderImage(CDC * pDC);
	//渲染资源
	VOID RenderImage(CImage & ImageSource);
	//渲染颜色
	COLORREF RenderColor(DOUBLE S, DOUBLE B);

	//颜色转换
public:
	//颜色转换
	inline COLORHSB RGBToHSB(COLORREF crColor);
	//颜色转换
	inline COLORRGB HSBToRGB(WORD H, DOUBLE S, DOUBLE B);
	//颜色合成
	inline COLORRGB CompoundColor(WORD H, DOUBLE S, DOUBLE B);

	//渲染状态
public:
	//获取颜色
	tagSkinRenderInfo GetSkinRenderInfo() { return m_SkinRenderInfo; }
	//设置颜色
	VOID SetSkinRenderInfo(const tagSkinRenderInfo & SkinRenderInfo) { m_SkinRenderInfo=SkinRenderInfo; }

	//静态函数
public:
	//获取实例
	static CSkinRenderManager * GetInstance() { return m_pSkinRenderManager; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif