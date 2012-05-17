#ifndef FACE_ITEM_VIEW_HEAD_FILE
#define FACE_ITEM_VIEW_HEAD_FILE

#pragma once

#include "AvatarControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//头像窗口
class AVATAR_CONTROL_CLASS CFaceItemView : public CStatic
{
	//变量定义
protected:
	WORD							m_wFaceID;							//头像标识
	DWORD							m_dwCustonID;						//自定标识
	DWORD							m_dwCustomFace[FACE_CX*FACE_CY];	//图片信息

	//颜色变量
protected:
	COLORREF						m_rcBackGround;						//背景颜色

	//函数定义
public:
	//构造函数
	CFaceItemView();
	//析构函数
	virtual ~CFaceItemView();

	//配置函数
public:
	//设置颜色
	VOID SetControlColor(COLORREF rcBackGround);
	
	//头像标识
public:
	//设置头像
	VOID SetSystemFace(WORD wFaceID);
	//设置头像
	VOID SetCustomFace(DWORD dwCustonID, DWORD dwCustomFace[FACE_CX*FACE_CY]);

	//消息函数
protected:
	//重画函数
	VOID OnPaint();
	//创建消息
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif