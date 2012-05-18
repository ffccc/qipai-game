#ifndef APPLY_USER_LIST_HEAD_FILE
#define APPLY_USER_LIST_HEAD_FILE

#pragma once

#include "Stdafx.h"


//////////////////////////////////////////////////////////////////////////
//
struct tagApplyUser
{
	//玩家信息
	DWORD							dwUserID;							//玩家ID号
	TCHAR							szUserName[NAME_LEN];				//玩家帐号
	__int64							iUserScore;							//玩家金币
};

//////////////////////////////////////////////////////////////////////////

//游戏记录
class CApplyUser 
{
	//控件变量
private:
	struct tagApplyBankerUser
	{
		int							nSortNo;
		DWORD						dwUserID;							//玩家ID号
		__int64						iUserScore;							//玩家金币
		TCHAR						szUserName[NAME_LEN];				//玩家帐号
	};


	tagApplyBankerUser  m_BankerUserArray[GAME_PLAYER];	//申请上庄玩家列表
	int  m_nApplyCount;									//当前申请上庄的人数
	int	 m_nPos;										//读写的前面位置
	//函数定义
public:
	//构造函数
	CApplyUser();
	//析构函数
	virtual ~CApplyUser();

	//功能函数
public:
	//插入列表
	void InserUser(tagApplyUser & ApplyUser);
	//删除列表
	void DeleteUser(tagApplyUser & ApplyUser);
	//更新列表
	void UpdateUser( tagApplyUser & ApplyUser );
	//清空列表
	void ClearAll();
	CString FormatNumber(__int64 i64);
	int GetApplayBankerUserCount(){return m_nApplyCount;}
	void ShowApplayBankerUser(CDC * pDC, CRect * pSortNoRect, CRect * pNameRect, CRect * pScoreRect);
	int ScrollUp();
	int ScrollDown();
};

//////////////////////////////////////////////////////////////////////////

#endif
