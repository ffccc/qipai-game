#pragma once

#include "CardControl.h"

extern CCardResource g_CardResource;

//////////////////
// 注册的拖放的消息
extern const UINT WM_DD_DRAGENTER; // 开始拖
extern const UINT WM_DD_DRAGOVER;  // 正在拖
extern const UINT WM_DD_DRAGDROP;  // 放
extern const UINT WM_DD_DRAGABORT; // 放弃拖

// 定义窗口的类型
const UINT DDW_SOURCE = 0x01;   // 拖放的源窗口
const UINT DDW_TARGET = 0x02;	// 拖放的目标窗口

///////////////////////////////////////////////
// 这是拖放对象
class CDragDropData {
protected:
	CBitmap			m_bitmap;	   // 用于画图的位图

	BYTE            m_cbCardData;  //被拖牌的数据
	//构造函数与析构函数
public:
	CDragDropData(BYTE cbCardData) { m_cbCardData = cbCardData; }
	~CDragDropData() { }

	BYTE GetDragData() { return m_cbCardData; }

	//创建一个位图列表
	CImageList* CreateDragImage(CWnd* pWnd, CRect& rc);

	//返回加拖放时显示图片的大小
	CSize OnGetDragSize(CDC& dc);

	//实现具体图片的显示
	void  OnDrawData(CDC& dc, CRect& rc);
};
/////////////////////////////////////////////////////////////
//保存拖放信息的结构体
struct DRAGDROPINFO {
	HWND hwndSource;		// 源窗口
	HWND hwndTarget;		// 目标窗口
	CDragDropData* data;	// 被拖放的数据
	POINT pt;				// 当前的光标所在的点(客户区的坐标)
};
class CDragDropManager
{
protected:
	//定义拖放管理器的状态
	enum { NONE=0, CAPTURED, DRAGGING };
	CWnd*			m_pMainWnd;				 // 主窗口
	HWND            m_hWnd[9];               // 保存9个操作区的窗口句柄
	HCURSOR			m_hCursorDrop;			 // 放时的光标
	HCURSOR			m_hCursorNo;			 // 没放时的光标

	DRAGDROPINFO	m_info;					 // 拖放时的数据
	UINT			m_iState;				 // 拖放管理器=所处的状态
	HWND			m_hwndTracking;		     // 源窗口
	CPoint			m_ptOrg;				 // 开始拖的起始点
	CImageList*		m_pDragImage;			 // 显示拖拉时的图像列表
	HCURSOR			m_hCursorSave;			 // 保存的光标

public:
	CDragDropManager(void);
	~CDragDropManager(void);

	//处理鼠标消息
	BOOL OnLButtonDown(const MSG& msg);
	BOOL OnMouseMove(const MSG& msg);
	BOOL OnLButtonUp(const MSG& msg);

	void SetState(UINT iState);
	BOOL IsCaptured() { return m_iState>=CAPTURED; }
	BOOL IsDragging() { return m_iState>=DRAGGING; }
	BOOL IsDragDropWnd(HWND hWnd);

	void SetCursors(HCURSOR hCursorDrop, HCURSOR hCursorNo)
	{
		m_hCursorDrop = hCursorDrop;
		m_hCursorNo = hCursorNo;
	}

	void InitDragDropManager(CWnd *pMainWnd,HWND hWnd[9]);

	BOOL PreTranslateMessage(MSG* pMsg);
};
