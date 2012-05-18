#ifndef D3D_SOUND_THREAD_HEAD_FILE
#define D3D_SOUND_THREAD_HEAD_FILE

#pragma once

#include "GameEngineHead.h"

//////////////////////////////////////////////////////////////////////////////////

//类说明
class CD3DSound;

//声音线程
class GAME_ENGINE_CLASS CD3DSoundThread : public CWHThread
{
	//变量定义
protected:
	CD3DSound *						m_pD3DSound;						//声音对象

	//函数定义
public:
	//构造函数
	CD3DSoundThread();
	//析构函数
	virtual ~CD3DSoundThread();

	//事件函数
protected:
	//运行事件
	virtual bool OnEventThreadRun();

	//功能函数
public:
	//设置对象
	VOID SetD3DSound(CD3DSound * pD3DSound) { m_pD3DSound=pD3DSound; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif