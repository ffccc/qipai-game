#ifndef RECORD_GAME_LIST_HEAD_FILE
#define RECORD_GAME_LIST_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//游戏记录
struct tagGameRecord
{
	__int64	cbGameTimes;	//第几局
	BYTE	cbGameRecord;	//本局结果
};

//////////////////////////////////////////////////////////////////////////

//游戏记录
class CGameRecord
{
	//函数定义
public:
	//构造函数
	CGameRecord();
	//析构函数
	virtual ~CGameRecord();
private:
	int		m_nRecordCount;		//记录个数
	int		m_nReadPos;			//当前读的位置
	int		m_nWritePos;		//当前写的位置
	tagGameRecord  m_RecordArray[MAX_SCORE_HISTORY];
	enum{RECORD_COUNT_LIMIT = 2100000000};

	//功能函数
public:
	//插入列表
	void FillGameRecord(tagGameRecord * pGameRecord, int nRecordNum);
	int GetRecordCount(){return m_nRecordCount;}
	int GetCurReadPos(){return m_nReadPos;}
	int GetCurWritePos(){return m_nWritePos;}
	void ClearAllRecord();
	int	ScrollLeft();
	int ScrollRight();
	void ShowHistory(CDC * pDC, CSkinImage skinImage, CRect * pRectArray, int nRectCount);
};

//////////////////////////////////////////////////////////////////////////

#endif
