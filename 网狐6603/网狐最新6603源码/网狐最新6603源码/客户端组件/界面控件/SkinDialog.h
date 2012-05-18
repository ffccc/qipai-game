#ifndef SKIN_DIALOG_HEAD_FILE
#define SKIN_DIALOG_HEAD_FILE

#pragma once

#include "SkinEncircle.h"
#include "SkinControlHead.h"
#include "SkinRenderManager.h"
#include "SkinResourceManager.h"

//////////////////////////////////////////////////////////////////////////////////

//窗口资源
class SKIN_CONTROL_CLASS CSkinDialogAttribute
{
	//颜色定义
public:
	COLORREF						m_crTitleText;						//标题颜色
	COLORREF						m_crTitleFrame;						//标题颜色
	COLORREF						m_crBackGround;						//背景颜色
	COLORREF						m_crControlText;					//控制颜色

	//资源变量
public:
	CSize							m_SizeButton;						//按钮大小
	CBrush							m_brBackGround;						//背景画刷
	CEncirclePNG					m_PNGEncircleFrame;					//框架资源

	//位置变量
public:
	tagEncircleInfo					m_EncircleInfoView;					//视图环绕
	tagEncircleInfo					m_EncircleInfoFrame;				//框架环绕

	//函数定义
public:
	//构造函数
	CSkinDialogAttribute();
	//析构函数
	virtual ~CSkinDialogAttribute();

	//管理函数
public:
	//配置资源
	bool Initialization(CSkinRenderManager * pSkinRenderManager);
};

//////////////////////////////////////////////////////////////////////////////////

//分层窗口
class SKIN_CONTROL_CLASS CSkinLayered : public CWnd
{
	//变量定义
protected:
	CWnd *							m_pWndControl;						//控件窗口

	//函数定义
public:
	//构造函数
	CSkinLayered();
	//析构函数
	virtual ~CSkinLayered();

	//功能函数
public:
	//创建窗口
	VOID CreateLayered(CWnd * pWndControl, CRect & rcWindow);
	//设置区域
	VOID InitLayeredArea(CDC * pDCImage, BYTE cbAlpha, CRect & rcUnLayered, CPoint & PointRound, bool bUnLayeredChild);
	//设置区域
	VOID InitLayeredArea(CPngImage & Image, BYTE cbAlpha, CRect & rcUnLayered, CPoint & PointRound, bool bUnLayeredChild);

	//消息映射
protected:
	//关闭消息
	VOID OnClose();
	//焦点消息
	VOID OnSetFocus(CWnd * pOldWnd);

	//静态函数
protected:
	//枚举函数
	static BOOL CALLBACK EnumChildProc(HWND hWndChild, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//对话框类
class SKIN_CONTROL_CLASS CSkinDialog : public CDialog
{
	//按钮状态
private:
	BYTE							m_cbNowHotIndex;					//现在焦点
	BYTE							m_cbNowHitIndex;					//按下按钮
	BYTE							m_cbButtonState[3];					//按钮状态

	//过渡状态
private:
	bool							m_bTransition;						//过渡状态
	WORD							m_wImageIndex[3];					//过渡索引

	//窗口变量
private:
	CRect							m_rcButton[3];						//按钮位置
	CSkinLayered					m_SkinLayered;						//分层窗口

	//资源变量
public:
	static CSkinDialogAttribute		m_SkinAttribute;					//资源变量

	//函数定义
public:
	//构造函数
	CSkinDialog(UINT nIDTemplate, CWnd * pParentWnd=NULL);
	//析构函数
	virtual ~CSkinDialog();

	//重载函数
public:
	//配置消息
	virtual BOOL OnInitDialog();
	//绘画消息
	virtual VOID OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight);

	//辅助函数
private:
	//按键测试
	UINT ControlHitTest(CPoint Point);
	//调整资源
	VOID RectifyResource(INT nWidth, INT nHeight);
	//调整按钮
	VOID CaleTitleButton(INT nWidth, INT nHeight);

	//绘画函数
private:
	//绘画控件
	VOID DrawControlView(CDC * pDC, INT nWdith, INT nHeight);
	//绘画标题
	VOID DrawSystemTitle(CDC * pDC, INT nWdith, INT nHeight);
	//绘画按钮
	VOID DrawSystemButton(CDC * pDC, INT nWdith, INT nHeight);

	//消息映射
protected:
	//鼠标消息
	VOID OnMouseMove(UINT nFlags, CPoint Point);
	//鼠标消息
	VOID OnLButtonUp(UINT nFlags, CPoint Point);
	//鼠标消息
	VOID OnLButtonDown(UINT nFlags, CPoint Point);

	//消息映射
protected:
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//时间消息
	VOID OnTimer(UINT_PTR nIDEvent);
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);
	//显示消息
	VOID OnShowWindow(BOOL bShow, UINT nStatus);
	//位置改变
	VOID OnWindowPosChanged(WINDOWPOS * lpWndPos);
	//控件颜色
	HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);

	//自定消息
protected:
	//标题消息
	LRESULT	OnSetTextMesage(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif