#ifndef GAME_FRAME_MEDAL_HEAD_FILE
#define GAME_FRAME_MEDAL_HEAD_FILE

#pragma once

#include "GameFrameHead.h"

//////////////////////////////////////////////////////////////////////////////////

//用户信息
class GAME_FRAME_CLASS CGameFrameMedalView : public CWnd
{
	//变量定义
protected:
	DWORD							m_dwUserMedal;						//用户奖牌

	//函数定义
public:
	//构造函数
	CGameFrameMedalView();
	//析构函数
	virtual ~CGameFrameMedalView();

	//功能函数
public:
	//设置奖牌
	VOID SetUserMedal(DWORD dwUserMedal);

	//消息映射
protected:
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif