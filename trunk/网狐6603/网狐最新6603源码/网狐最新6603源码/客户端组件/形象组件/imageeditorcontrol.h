#ifndef IMAGE_EDITOR_CONTROL_HEAD_FILE
#define IMAGE_EDITOR_CONTROL_HEAD_FILE

#pragma once

#include "CameraControl.h"
#include "AvatarControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//旋转类型
#define ROUND_TYPE_LEFT				0									//向左旋转
#define ROUND_TYPE_RIGHT			1									//向右旋转

//////////////////////////////////////////////////////////////////////////////////

//接口定义
interface IImageEditorEvent
{
	//图像事件
	virtual VOID OnEventImageSelect(bool bLoadImage, bool bOperateing)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//编辑控件
class AVATAR_CONTROL_CLASS CImageEditorControl : public CWnd
{
	//操作变量
protected:
	BYTE							m_cbHitTest;						//鼠标测试
	CPoint							m_ptLockPoint;						//锁定位置
	CPoint							m_ptDragOrigin;						//拖拉原点

	//状态变量
protected:
	bool							m_bVShowLine;						//线框模式
	BYTE							m_cbConsultType;					//锁定方式
	BYTE							m_cbOperateType;					//操作类型
	CRect							m_rcImageSelect;					//选择区域

	//状态变量
protected:
	CSize							m_SizeSource;						//原图大小
	CSize							m_SizeTarget;						//目标大小
	DOUBLE							m_dZoomPercent;						//缩放比例

	//内核变量
protected:
	CImage							m_ImageSource;						//原图图片
	IImageEditorEvent *				m_pIImageEditorEvent;				//回调接口

	//函数定义
public:
	//构造函数
	CImageEditorControl();
	//析构函数
	virtual ~CImageEditorControl();

	//功能函数
public:
	//重置图片
	bool DestoryImage();
	//加载图片
	bool LoadEditImage(CImage & Image);
	//加载图片
	bool LoadEditImage(LPCTSTR pszImage);

	//控制函数
public:
	//加载判断
	bool IsNull();
	//旋转图片
	VOID SetRoundImage(BYTE cbRoundType);
	//获取图片
	VOID DrawEditImage(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight);

	//配置函数
public:
	//设置接口
	VOID SetImageEditorEvent(IImageEditorEvent * pIImageEditorEvent) { m_pIImageEditorEvent=pIImageEditorEvent; }

	//虚线控制
public:
	//设置虚线
	VOID SetVLineStatus(bool bShowVLine);
	//虚线状态
	bool GetVLineStatus() { return m_bVShowLine; }

	//辅助函数
protected:
	//绘画函数
	VOID DrawLineFrame(CDC * pDC);
	//点击测试
	BYTE OnEventHitTest(CPoint & Point);

	//消息响应
public:
	//绘制背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//鼠标移动
	VOID OnMouseMove(UINT nFlags, CPoint MousePoint);
	//左键弹起
	VOID OnLButtonUp(UINT nFlags, CPoint MousePoint);
	//左键按下
	VOID OnLButtonDown(UINT nFlags, CPoint MousePoint);
	//设置光标
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif

