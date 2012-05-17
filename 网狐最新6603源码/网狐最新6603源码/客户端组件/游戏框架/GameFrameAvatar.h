#ifndef GAME_FRAME_AVATAR_HEAD_FILE
#define GAME_FRAME_AVATAR_HEAD_FILE

#pragma once

#include "GameFrameHead.h"

//////////////////////////////////////////////////////////////////////////////////

//游戏头像
class GAME_FRAME_CLASS CGameFrameAvatar
{
	//资源变量
protected:
	WORD							m_wD3DLineCount;					//头像数目
	WORD							m_wD3DFullCount;					//头像数目
	CD3DTexture						m_D3DTextureAvatar;					//头像纹理

	//静态变量
protected:
	static CGameFrameAvatar *		m_pGameFrameAvatar;					//引擎接口

	//函数定义
public:
	//构造函数
	CGameFrameAvatar();
	//析构函数
	virtual ~CGameFrameAvatar();

	//配置接口
public:
	//配置函数
	VOID Initialization(CD3DDevice * pD3DDevice);

	//绘画函数
public:
	//绘画头像
	VOID DrawUserAvatar(CDC * pDC, INT nXPos, INT nYPos, WORD wFaceID);
	//绘画头像
	VOID DrawUserAvatar(CDC * pDC, INT nXPos, INT nYPos, IClientUserItem * pIClientUserItem);

	//绘画函数
public:
	//绘画头像
	VOID DrawUserAvatar(CD3DDevice * pD3DDevice, INT nXPos, INT nYPos, WORD wFaceID);
	//绘画头像
	VOID DrawUserAvatar(CD3DDevice * pD3DDevice, INT nXPos, INT nYPos, IClientUserItem * pIClientUserItem);

	//静态函数
public:
	//获取实例
	static CGameFrameAvatar * GetInstance() { return m_pGameFrameAvatar; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif