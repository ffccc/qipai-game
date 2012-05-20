#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////

//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码

#define NUM_ONE_DOT                 0x01            // 一点

// 比较结果
enum E_COMPARE_RESULT
{
	enCompResult_Illegal=(int)-2,      // 无法比较
	enCompResult_Less=(int)-1,         // 小于
	enCompResult_Equal=(int)0,         // 等于
	enCompResult_Greater=(int)1        // 大于
};

//////////////////////////////////////////////////////////////////////////

class CDebugString
{
public:
	CDebugString(const TCHAR *pszFunctionName, int iLineNo)
		:m_pszFunctionName(pszFunctionName),m_iLineNo(iLineNo)
	{
	}
public:
	virtual ~CDebugString(void){}

	void __cdecl operator()(const TCHAR *pszFmt, ...) const
	{
		TCHAR szData[1024]={0};
		TCHAR szMsg[1024]={0};
		va_list args;
		va_start(args, pszFmt);
		_snprintf(szData, sizeof(szData) - 1, pszFmt, args);
		va_end(args);
		_snprintf(szMsg, sizeof(szMsg), _T("qlog:[line: %04d][%s] %s\n"), m_iLineNo, m_pszFunctionName, szData);
		OutputDebugString(szMsg);
	}
protected:
	const TCHAR *m_pszFunctionName;
	const int    m_iLineNo;
};

//////////////////////////////////////////////////////////////////////////
#ifdef _UNICODE
#define __STR2WSTR(str)  L##str
#define _STR2WSTR(str)   __STR2WSTR(str)
#define  __YYSKIN_FUCNTION__    _STR2WSTR(__FUNCTION__)
#else
#define  __YYSKIN_FUCNTION__    __FUNCTION__
#endif // _UNICODE
#define OUTPUT_DEBUG_STRING  CDebugString(__YYSKIN_FUCNTION__, __LINE__)

//游戏逻辑
class CGameLogic
{
	//变量定义
private:
	static const E_SICBO_NUMBER				m_enSicboData[MAX_SICBO_NUMBER];				//扑克定义

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

	// 排序
	void SortCards(BYTE byCardsBuffer[], BYTE byCardCount);
	
	//逻辑函数
public:
	// 52个区域
	// 获取牌型
	bool GetCardType(E_SICBO_NUMBER enCardsBuffer[], E_CARD_TYPE enCardType[], int &iCardTypeCount);

	// 单双
	E_CARD_TYPE IsSicboSingleDouble(const E_SICBO_NUMBER enSicboNumber[]);

	// 3个骰子点数和
	E_CARD_TYPE IsSicboNumber(const E_SICBO_NUMBER enSicboNumber[]);

	int GetSicboCountByNumber(const E_SICBO_NUMBER enSicboBuffer[], E_SICBO_NUMBER enSicboNumber);
public:
	static const __int64	m_i64Loss_Percent[COUNT_AZIMUTH];				// 每个区域的赔率
};

//////////////////////////////////////////////////////////////////////////

#endif
