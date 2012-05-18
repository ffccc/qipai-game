#ifndef D3D_SURFACE_HEAD_FILE
#define D3D_SURFACE_HEAD_FILE

#pragma once

#include "D3DDevice.h"
#include "GameEngineHead.h"

//////////////////////////////////////////////////////////////////////////////////

//表面对象
class GAME_ENGINE_CLASS CD3DSurface
{
	//图片信息
public:
	CSize							m_ImageSize;						//图片大小

	//接口变量
public:
	LPDIRECT3DSURFACE9				m_pISurface;						//纹理接口

	//函数定义
public:
	//构造函数
	CD3DSurface();
	//析构函数
	virtual ~CD3DSurface();

	//图片信息
public:
	//加载判断
	bool IsNull();
	//图片大小
	bool GetSurfaceSize(CSize & ImageSize);

	//功能函数
public:
	//销毁纹理
	bool Destory();
	//加载表面
	bool LoadImage(CD3DDevice * pD3DDevice, HINSTANCE hInstance, LPCTSTR pszResource);

	//绘画函数
public:
	//绘画图片
	bool DrawImage(CD3DDevice * pD3DDevice, INT nXDest, INT nYDest);
	//绘画图片
	bool DrawImage(CD3DDevice * pD3DDevice, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXSource, INT nYSource);
	//绘画图片
	bool DrawImage(CD3DDevice * pD3DDevice, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXSource, INT nYSource, INT nSourceWidth, INT nSourceHeight);
};

//////////////////////////////////////////////////////////////////////////////////

#endif