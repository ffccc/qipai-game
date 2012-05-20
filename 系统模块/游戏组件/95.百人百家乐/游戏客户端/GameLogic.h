#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////	
//调试函数
static void MyMsgBox(LPCSTR lpszFormat, ...)
{
	va_list args;
	int		nBuf;
	TCHAR	szBuffer[512];

	va_start(args, lpszFormat);
	nBuf = _vsntprintf(szBuffer, sizeof(szBuffer)-sizeof(TCHAR), lpszFormat, args);
#ifdef _DEBUG
	AfxMessageBox(szBuffer);
#else
	TCHAR	szFormat[512];
	_stprintf(szFormat, _T("[JJ][BJL]: %s"), szBuffer);
	OutputDebugString(szFormat);
#endif

	va_end(args);
}

static bool MyDebug(LPCSTR lpszFormat, ...)
{
	va_list args;
	int		nBuf;
	TCHAR	szBuffer[512];
	TCHAR	szFormat[512];

	va_start(args, lpszFormat);
	nBuf = _vsntprintf(szBuffer, sizeof(szBuffer)-sizeof(TCHAR), lpszFormat, args);
	_stprintf(szFormat, _T("[JJ][BJL]: %s"), szBuffer);
	OutputDebugString(szFormat);

	va_end(args);

	return true;
}
//////////////////////////////////////////////////////////////////////////


//游戏逻辑
class CGameLogic
{
	//变量定义
private:
	static const BYTE				m_cbCardListData[52*8];				//扑克定义

	//函数定义
public:
	//构造函数
	CGameLogic();
	//析构函数
	virtual ~CGameLogic();

	//类型函数
public:
	//获取数值
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//获取花色
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&LOGIC_MASK_COLOR; }

	//控制函数
public:
	//混乱扑克
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);

	//逻辑函数
public:
	//获取牌点
	BYTE GetCardPip(BYTE cbCardData);
	//获取牌点
	BYTE GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount);
};

//////////////////////////////////////////////////////////////////////////

#endif
