#ifndef MISSION_LIST_HEAD_FILE
#define MISSION_LIST_HEAD_FILE

#pragma once

#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////

//数组说明
typedef CMap<WORD,WORD,WORD,WORD>	CKindIDMap;							//类型标识

//////////////////////////////////////////////////////////////////////////////////

//列表任务
class CMissionList : public CWnd, public CMissionItem
{
	//变量定义
protected:
	bool							m_bUpdateOnLine;					//更新人数
	CKindIDMap						m_KindIDMission;					//游戏标识
	CKindIDMap						m_KindIDPending;					//待决标识

	//静态变量
protected:
	static CMissionList *			m_pMissionList;						//对象指针

	//函数定义
public:
	//构造函数
	CMissionList();
	//析构函数
	virtual ~CMissionList();

	//重载函数
public:
	//连接事件
	virtual bool OnEventMissionLink(INT nErrorCode);
	//关闭事件
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//功能函数
public:
	//更新人数
	VOID UpdateOnLineInfo();
	//更新类型
	VOID UpdateServerInfo(WORD wKindID);

	//静态函数
public:
	//获取对象
	static CMissionList * GetInstance() { return m_pMissionList; }

	//消息映射
protected:
	//时间消息
	VOID OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif