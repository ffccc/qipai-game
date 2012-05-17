#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////	
//���Ժ���
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


//��Ϸ�߼�
class CGameLogic
{
	//��������
private:
	static const BYTE				m_cbCardListData[52*8];				//�˿˶���

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

	//�߼�����
public:
	//��ȡ�Ƶ�
	BYTE GetCardPip(BYTE cbCardData);
	//��ȡ�Ƶ�
	BYTE GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount);
};

//////////////////////////////////////////////////////////////////////////

#endif
