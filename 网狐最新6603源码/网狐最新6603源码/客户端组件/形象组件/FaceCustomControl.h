#ifndef FACE_CUSTOM_CONTROL_HEAD_FILE
#define FACE_CUSTOM_CONTROL_HEAD_FILE

#pragma once

#include "FaceItemControl.h"
#include "AvatarControlHead.h"
#include "ImageEditorControl.h"

//////////////////////////////////////////////////////////////////////////////////

//自定头像
class AVATAR_CONTROL_CLASS CFaceItemCustomWnd : public CWnd
{
	//控制按钮
protected:
	CSkinButton			 			m_btTurnLeft;						//左转按钮		
	CSkinButton			 			m_btTurnRight;						//右转按钮
	CSkinButton			 			m_btShowVLine;						//虚线按钮

	//控制按钮
protected:
	CSkinButton			 			m_btLoadImage;						//加载图片
	CSkinButton			 			m_btLoadCamera;						//开启摄像

	//控件变量
protected:
	CImageEditorControl    			m_ImageEditorControl;				//编辑控件

	//函数定义
public:
	//构造函数
	CFaceItemCustomWnd();
	//析构函数
	virtual ~CFaceItemCustomWnd();

	//配置函数
public:
	//设置接口
	VOID SetImageEditorEvent(IImageEditorEvent * pIImageEditorEvent);

	//图片管理
public:
	//加载判断
	bool IsNull();
	//重置图片
	VOID DestoryImage();
	//获取图片
	VOID DrawEditImage(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight);

	//消息映射
protected:
	//绘画消息
	VOID OnPaint();
	//位置信息
	VOID OnSize(UINT nType, INT cx, INT cy);
	//创建消息
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);

	//按钮消息
protected:
	//旋转图像
	VOID OnBnClickedTurnLeft();
	//旋转图像
	VOID OnBnClickedTurnRight();
	//显示虚线
	VOID OnBnClickedShowVLine();
	//加载图像
	VOID OnBnClickedLoadImage();
	//加载图像
	VOID OnBnClickedLoadCamera();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif