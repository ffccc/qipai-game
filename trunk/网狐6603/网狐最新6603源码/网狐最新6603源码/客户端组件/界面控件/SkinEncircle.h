#ifndef SKIN_ENCIRCLE_HEAD_FILE
#define SKIN_ENCIRCLE_HEAD_FILE

#pragma once

#include "SkinControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//环绕信息
struct tagEncircleInfo
{
	INT								nTBorder;							//上边框高
	INT								nBBorder;							//下边框高
	INT								nLBorder;							//左边框宽
	INT								nRBorder;							//右边框宽
};

//环绕位图
struct tagEncirclePNG
{
	CPngImage						ImageTL;							//位图对象
	CPngImage						ImageTM;							//位图对象
	CPngImage						ImageTR;							//位图对象
	CPngImage						ImageML;							//位图对象
	CPngImage						ImageMR;							//位图对象
	CPngImage						ImageBL;							//位图对象
	CPngImage						ImageBM;							//位图对象
	CPngImage						ImageBR;							//位图对象
};

//环绕位图
struct tagEncircleBMP
{
	CBitImage						ImageTL;							//位图对象
	CBitImage						ImageTM;							//位图对象
	CBitImage						ImageTR;							//位图对象
	CBitImage						ImageML;							//位图对象
	CBitImage						ImageMR;							//位图对象
	CBitImage						ImageBL;							//位图对象
	CBitImage						ImageBM;							//位图对象
	CBitImage						ImageBR;							//位图对象
};

//环绕资源
struct tagEncircleResource
{
	LPCTSTR							pszImageTL;							//资源标识
	LPCTSTR							pszImageTM;							//资源标识
	LPCTSTR							pszImageTR;							//资源标识
	LPCTSTR							pszImageML;							//资源标识
	LPCTSTR							pszImageMR;							//资源标识
	LPCTSTR							pszImageBL;							//资源标识
	LPCTSTR							pszImageBM;							//资源标识
	LPCTSTR							pszImageBR;							//资源标识
};

//////////////////////////////////////////////////////////////////////////////////

//环绕界面
class SKIN_CONTROL_CLASS CEncircleBMP
{
	//资源信息
protected:
	HINSTANCE						m_hResInstance;						//资源句柄
	tagEncircleInfo					m_EncircleInfo;						//环绕信息
	tagEncircleResource				m_EncircleResource;					//环绕资源

	//函数定义
public:
	//构造函数
	CEncircleBMP();
	//析构函数
	virtual ~CEncircleBMP();

	//重载函数
protected:
	//加载资源
	virtual bool LoadEncircleImage(tagEncircleBMP & EncircleImage);
	//绘画处理
	virtual bool PreDrawEncircleImage(tagEncircleBMP & EncircleImage) { return true; }

	//资源管理
public:
	//默认资源
	bool InitDefaultResource();
	//设置资源
	bool InitEncircleResource(tagEncircleResource & EncircleResource, HINSTANCE hResInstance);

	//环绕绘画
public:
	//绘画环绕
	bool DrawEncircleFrame(CDC * pDC, CRect & rcEncircle);
	//绘画环绕
	bool DrawEncircleFrame(CDC * pDC, CRect & rcEncircle, COLORREF crTrans);
	//绘画环绕
	bool DrawEncircleFrame(CDC * pDC, CRect & rcEncircle, COLORREF crTrans, BYTE cbAlphaDepth);

	//位置函数
public:
	//获取位置
	bool GetEncircleInfo(tagEncircleInfo & EncircleInfo);
	//获取位置
	bool GetEncircleRect(CRect & rcFrameBorad, CRect & rcControl);

	//环绕调整
public:
	//环绕调整
	bool SetWindowPos(CWnd * pWnd, CRect & rcEncircleBorad);
	//环绕调整
	bool DeferWindowPos(CWnd * pWnd, HDWP hDwp, CRect & rcEncircleBorad);
};

//////////////////////////////////////////////////////////////////////////////////

//环绕界面
class SKIN_CONTROL_CLASS CEncirclePNG
{
	//资源信息
protected:
	HINSTANCE						m_hResInstance;						//资源句柄
	tagEncircleInfo					m_EncircleInfo;						//环绕信息
	tagEncircleResource				m_EncircleResource;					//环绕资源

	//函数定义
public:
	//构造函数
	CEncirclePNG();
	//析构函数
	virtual ~CEncirclePNG();

	//重载函数
protected:
	//加载资源
	virtual bool LoadEncircleImage(tagEncirclePNG & EncircleImage);
	//绘画处理
	virtual bool PreDrawEncircleImage(tagEncirclePNG & EncircleImage) { return true; }

	//资源管理
public:
	//默认资源
	bool InitDefaultResource();
	//设置资源
	bool InitEncircleResource(tagEncircleResource & EncircleResource, HINSTANCE hResInstance);

	//环绕绘画
public:
	//绘画环绕
	bool DrawEncircleFrame(CDC * pDC, CRect & rcEncircle);

	//位置函数
public:
	//获取位置
	bool GetEncircleInfo(tagEncircleInfo & EncircleInfo);
	//获取位置
	bool GetEncircleRect(CRect & rcFrameBorad, CRect & rcControl);

	//环绕调整
public:
	//环绕调整
	bool SetWindowPos(CWnd * pWnd, CRect & rcEncircleBorad);
	//环绕调整
	bool DeferWindowPos(CWnd * pWnd, HDWP hDwp, CRect & rcEncircleBorad);
};

//////////////////////////////////////////////////////////////////////////////////

#endif