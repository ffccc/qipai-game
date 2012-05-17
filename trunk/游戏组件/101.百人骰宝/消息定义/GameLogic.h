#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////

//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����

#define NUM_ONE_DOT                 0x01            // һ��

// �ȽϽ��
enum E_COMPARE_RESULT
{
	enCompResult_Illegal=(int)-2,      // �޷��Ƚ�
	enCompResult_Less=(int)-1,         // С��
	enCompResult_Equal=(int)0,         // ����
	enCompResult_Greater=(int)1        // ����
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

//��Ϸ�߼�
class CGameLogic
{
	//��������
private:
	static const E_SICBO_NUMBER				m_enSicboData[MAX_SICBO_NUMBER];				//�˿˶���

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//���ͺ���
public:
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&LOGIC_MASK_COLOR; }

	//���ƺ���
public:
	//�����˿�
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);

	// ����
	void SortCards(BYTE byCardsBuffer[], BYTE byCardCount);
	
	//�߼�����
public:
	// 52������
	// ��ȡ����
	bool GetCardType(E_SICBO_NUMBER enCardsBuffer[], E_CARD_TYPE enCardType[], int &iCardTypeCount);

	// ��˫
	E_CARD_TYPE IsSicboSingleDouble(const E_SICBO_NUMBER enSicboNumber[]);

	// 3�����ӵ�����
	E_CARD_TYPE IsSicboNumber(const E_SICBO_NUMBER enSicboNumber[]);

	int GetSicboCountByNumber(const E_SICBO_NUMBER enSicboBuffer[], E_SICBO_NUMBER enSicboNumber);
public:
	static const __int64	m_i64Loss_Percent[COUNT_AZIMUTH];				// ÿ�����������
};

//////////////////////////////////////////////////////////////////////////

#endif
