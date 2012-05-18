#ifndef GAME_FRAME_VIEW_D3D_HEAD_FILE
#define GAME_FRAME_VIEW_D3D_HEAD_FILE

#pragma once

#include "GameFrameHead.h"
#include "GameFrameView.h"

//////////////////////////////////////////////////////////////////////////////////

//视图框架
class GAME_FRAME_CLASS CGameFrameViewD3D : public CGameFrameView, public ID3DDeviceSink
{
	//状态变量
protected:
	bool							m_bInitD3D;							//创建标志
	HANDLE 							m_hRenderThread;					//渲染线程

	//辅助变量
protected:
	CRect							m_RectDirtySurface;					//重画区域

	//帧数统计
protected:
	DWORD							m_dwDrawBenchmark;					//基准起点
	DWORD							m_dwDrawLastCount;					//绘画次数
	DWORD							m_dwDrawCurrentCount;				//绘画次数

	//资源变量
protected:
	CD3DTexture						m_D3DTextureReady;					//准备纹理
	CD3DTexture						m_D3DTextureMember;					//会员纹理
	CD3DTexture						m_D3DTextureClockItem;				//时间纹理
	CD3DTexture						m_D3DTextureClockBack;				//时间纹理

	//组件变量
public:
	CD3DFont						m_D3DFont;							//字体对象
	CD3DDirect						m_D3DDirect;						//环境对象
	CD3DDevice						m_D3DDevice;						//设备对象
	CVirtualEngine					m_VirtualEngine;					//虚拟引擎

	//函数定义
public:
	//构造函数
	CGameFrameViewD3D();
	//析构函数
	virtual ~CGameFrameViewD3D();

	//重载函数
public:
	//重置界面
	virtual VOID ResetGameView();
	//调整控件
	virtual VOID RectifyControl(INT nWidth, INT nHeight);
	//界面更新
	virtual VOID InvalidGameView(INT nXPos, INT nYPos, INT nWidth, INT nHeight);

	//界面函数
private:
	//动画驱动
	virtual VOID CartoonMovie()=NULL;
	//配置界面
	virtual VOID InitGameView(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight)=NULL;
	//绘画界面
	virtual VOID DrawGameView(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight)=NULL;

	//设备接口
protected:
	//配置设备
	virtual VOID OnInitDevice(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight);
	//丢失设备
	virtual VOID OnLostDevice(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight);
	//重置设备
	virtual VOID OnResetDevice(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight);
	//渲染窗口
	virtual VOID OnRenderWindow(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight);

	//重载函数
protected:
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//默认回调
	virtual LRESULT DefWindowProc(UINT nMessage, WPARAM wParam, LPARAM lParam);

	//功能函数
public:
	//渲染线程
	VOID StartRenderThread();

	//游戏元素
public:
	//绘画准备
	VOID DrawUserReady(CD3DDevice * pD3DDevice, INT nXPos, INT nYPos);
	//绘画时间
	VOID DrawUserClock(CD3DDevice * pD3DDevice, INT nXPos, INT nYPos, WORD wUserClock);
	//绘画标志
	VOID DrawOrderFlag(CD3DDevice * pD3DDevice, INT nXPos, INT nYPos, BYTE cbImageIndex);
	//绘画头像
	VOID DrawUserAvatar(CD3DDevice * pD3DDevice, INT nXPos, INT nYPos, IClientUserItem * pIClientUserItem);

	//画图函数
public:
	//绘画背景
	VOID DrawViewImage(CD3DDevice * pD3DDevice, CD3DTexture & D3DTexture, BYTE cbDrawMode);
	//绘画数字
	VOID DrawNumberString(CD3DDevice * pD3DDevice, CD3DTexture & D3DTexture, LONG lNumber, INT nXPos, INT nYPos);

	//画图函数
public:
	//绘画背景
	VOID DrawViewImage(CD3DDevice * pD3DDevice, CD3DSprite & D3DSprite, BYTE cbDrawMode);
	//绘画数字
	VOID DrawNumberString(CD3DDevice * pD3DDevice, CD3DSprite & D3DSprite, LONG lNumber, INT nXPos, INT nYPos);

	//输出字体
public:
	//输出字体
	VOID DrawTextString(CD3DDevice * pD3DDevice, LPCTSTR pszString, CRect rcDraw,UINT nFormat, D3DCOLOR D3DColor);
	//输出字体
	VOID DrawTextString(CD3DDevice * pD3DDevice, LPCTSTR pszString, INT nXPos, INT nYPos, UINT nFormat, D3DCOLOR D3DColor);
	//绘画字符
	VOID DrawTextString(CD3DDevice * pD3DDevice, LPCTSTR pszString, CRect rcDraw, UINT nFormat, D3DCOLOR crText, D3DCOLOR crFrame);
	//绘画字符
	VOID DrawTextString(CD3DDevice * pD3DDevice, LPCTSTR pszString, INT nXPos, INT nYPos, UINT nFormat, D3DCOLOR crText, D3DCOLOR crFrame);

	//消息映射
protected:
	//绘画函数
	VOID OnPaint();
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);

	//自定消息
protected:
	//数据消息
	LRESULT OnMessageD3DStatus(WPARAM wParam, LPARAM lParam);
	//渲染消息
	LRESULT OnMessageD3DRender(WPARAM wParam, LPARAM lParam);

	//静态函数
private:
	//渲染线程
	static VOID D3DRenderThread(LPVOID pThreadData);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif