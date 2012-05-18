#ifndef DLG_CUSTOM_FACE_HEAD_FILE
#define DLG_CUSTOM_FACE_HEAD_FILE

#pragma once

#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////

//头像选择
class CDlgCustomFace : public CSkinDialog, public CMissionItem, public IImageEditorEvent, public IFaceItemSelectEvent
{
	//状态变量
protected:
	BYTE							m_cbMode;							//编辑模式
	WORD							m_wFaceID;							//选择头像
	tagCustomFaceInfo				m_CustomFaceInfo;					//自定头像

	//接口变量
protected:
	ICustomFaceEvent *				m_pICustomFaceEvent;				//头像事件

	//功能按钮
protected:
	CSkinButton			 			m_btOk;								//存储退出
	CSkinButton			 			m_btCancel;							//放弃按钮

	//控件变量
protected:
	CSkinTabCtrl		 			m_TabControl;						//选择类型
	CMissionManager					m_MissionManager;					//任务管理
	CFaceItemCustomWnd				m_FaceItemCustomWnd;				//头像自定
	CFaceItemSelectWnd	 			m_FaceItemSelectWnd;				//头像选择

	//函数定义
public:
	//构造函数
	CDlgCustomFace();
	//析构函数
	virtual ~CDlgCustomFace();

	//编辑接口
public:
	//图像事件
	virtual VOID OnEventFaceSelect(WORD wFaceID);
	//图像事件
	virtual VOID OnEventImageSelect(bool bLoadImage, bool bOperateing);

	//重置函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//创建函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();

	//窗口函数
protected:
	//绘画消息
	virtual VOID OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight);

	//重载函数
protected:
	//连接事件
	virtual bool OnEventMissionLink(INT nErrorCode);
	//关闭事件
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//功能函数
public:
	//设置接口
	bool SetCustomFaceEvent(IUnknownEx * pIUnknownEx);

	//内部函数
private:
	//控件控制
	VOID EnableControl(bool bEnableControl);

	//消息响应
protected:
	//类型改变
	VOID OnTcnSelAlterFaceMode(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif