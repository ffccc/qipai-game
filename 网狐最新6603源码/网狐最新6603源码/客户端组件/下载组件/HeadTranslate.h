#ifndef HEAD_TRANSLATE_HEAD_FILE
#define HEAD_TRANSLATE_HEAD_FILE

#pragma once

#include "DownLoadHead.h"

//////////////////////////////////////////////////////////////////////////////////

//协议翻译
class DOWN_LOAD_CLASS CHeadTranslate
{
	//变量定义
protected:
	WORD							m_wDataLength;						//数据长度
	BYTE *							m_pcbHeadData;						//数据指针

	//数据信息
protected:
	DWORD							m_dwTotalFileSize;					//文件大小
	TCHAR							m_szFileEntityTag[32];				//文件标识

	//函数定义
public:
	//构造函数
	CHeadTranslate();
	//析构函数
	virtual ~CHeadTranslate();

	//数据信息
public:
	//文件大小
	DWORD GetTotalFileSize() { return m_dwTotalFileSize; }
	//文件标识
	LPCTSTR GetFileEntityTag() { return m_szFileEntityTag; }
	
	//功能函数
public:
	//清理参数
	VOID CleanData();
	//设置参数
	VOID SetHeadData(BYTE * pcbHeadData, WORD wDataLength);
	//获取数据
	WORD GetLineString(WORD wLineIndex, TCHAR szDataBuffer[], WORD wCharCount);
};

//////////////////////////////////////////////////////////////////////////////////

#endif