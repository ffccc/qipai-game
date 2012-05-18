#ifndef WND_VALIDATE_CODE_HEAD_FILE
#define WND_VALIDATE_CODE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////////////

#define VALIDATE_COUNT				4									//验证长度

//////////////////////////////////////////////////////////////////////////////////

//验证控件
class CWndValidateCode : public CStatic
{
	//变量定义
protected:
	bool							m_bValidateCode;					//验证标志
	TCHAR							m_szValidateCode[VALIDATE_COUNT];	//验证字符

	//控件变量
protected:
	CImage							m_ImageValidateCode;				//验证图片

	//函数定义
public:
	//构造函数
	CWndValidateCode();
	//析构函数
	virtual ~CWndValidateCode();

	//功能函数
public:
	//创建验证
	VOID RandValidateCode();
	//效验验证
	bool CheckValidateCode(LPCTSTR pszValidateCode);
	//构造图片
	VOID CreateValidateCode(CImage & ImageValidateCode, INT nWidth, INT nHeight);

	//消息函数
protected:
	//重画函数	
	VOID OnPaint();
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif