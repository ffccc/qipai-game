#ifndef GAME_FRAME_USER_VIEW_HEAD_FILE
#define GAME_FRAME_USER_VIEW_HEAD_FILE

#pragma once

#include "GameFrameHead.h"
#include "GameFramePublicize.h"

//////////////////////////////////////////////////////////////////////////////////

//用户信息
class GAME_FRAME_CLASS CGameFrameUserView : public CWnd
{
	//接口变量
protected:
	IClientKernel *					m_pIClientKernel;					//内核接口
	IClientUserItem *				m_pICurrentUserItem;				//当前用户

	//控件变量
protected:
	CSkinButton						m_btUserMedal;						//用户奖牌
	CSkinButton						m_btLoveLiness;						//用户魅力
	CGameFramePublicize				m_GameFramePublicize;				//广告控件

	//函数定义
public:
	//构造函数
	CGameFrameUserView();
	//析构函数
	virtual ~CGameFrameUserView();

	//功能函数
public:
	//设置广告
	VOID SetGameBillInfo(WORD wKindID, WORD wServerID);
	//设置用户
	VOID SetUserViewInfo(IClientUserItem * pIClientUserItem);

	//界面函数
private:
	//调整控件
	VOID RectifyControl(INT nWidth, INT nHeight);

	//按钮消息
protected:
	//奖牌兑换
	VOID OnBnClickedUserMedal();
	//魅力兑换
	VOID OnBnClickedLoveLiness();

	//消息映射
protected:
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);
	//创建消息
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif