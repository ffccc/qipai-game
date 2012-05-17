#ifndef DLG_CAMERA_HEAD_FILE
#define DLG_CAMERA_HEAD_FILE

#pragma once

#include "AvatarControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//摄像头类
class AVATAR_CONTROL_CLASS CCameraControl : public CSkinDialog
{
	//友元定义
	friend class CFaceItemCustomWnd;

	//变量定义
protected:
	bool							m_bInit;							//配置标识
	HWND							m_hWndCamera;						//显示窗口
	CImage							m_ImageResult;						//结果图像
	
	//设备信息
protected:
	CAPSTATUS						m_CapStatus;						//当前状态
	CAPTUREPARMS					m_CapTureparms;						//捕捉参数
	CAPDRIVERCAPS					m_CapDrivercaps;					//驱动性能

	//控件变量
protected:
	CSkinButton						m_btOk;								//保存编辑
	CSkinButton						m_btRepeat;							//重新拍摄
	CSkinButton						m_btCameraDict;						//捕获单帧

	//其他控件
protected:
	CStatic							m_StringControl;					//提示控件
	CStatic							m_CameraControl;					//视频窗口

	//函数定义
public:
	//构造函数
	CCameraControl(CWnd * pParent);
	//析构函数
	virtual ~CCameraControl();

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//配置函数
	virtual BOOL OnInitDialog();
	
	//消息函数
protected:
	//重新拍摄
	VOID OnBnClickedRepeat();
	//拍摄图片
	VOID OnBnClickedCameraDict();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif