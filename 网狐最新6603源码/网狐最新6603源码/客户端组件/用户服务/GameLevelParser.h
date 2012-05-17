#ifndef GAME_LEVEL_PARSER_HEAD_FILE
#define GAME_LEVEL_PARSER_HEAD_FILE

#pragma once

#include "UserServiceHead.h"

//////////////////////////////////////////////////////////////////////////////////

//等级子项
struct tagLevelDescribe
{
	LONG							lLevelScore;						//等级积分
	TCHAR							szLevelName[16];					//等级描述
};

//等级数组
typedef CWHArray<tagLevelDescribe *> CLevelDescribeArray;				//等级数组	

//////////////////////////////////////////////////////////////////////////////////

//等级解释
class USER_SERVICE_CLASS CGameLevelParser : public IGameLevelParser
{
	//变量定义
protected:
	TCHAR							m_szKindName[LEN_KIND];				//游戏名字

	//子项变量
protected:
	CLevelDescribeArray				m_LevelDescribeActive;				//活动数组
	static CLevelDescribeArray		m_LevelDescribeStorage;				//存储数组

	//函数定义
public:
	//构造函数
	CGameLevelParser();
	//析构函数
	virtual ~CGameLevelParser();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//配置接口
public:
	//游戏名字
	virtual VOID SetKindName(LPCTSTR pszKindName);
	//游戏名字
	virtual LPCTSTR GetKindName() { return m_szKindName; }

	//描述接口
public:
	//等级描述
	virtual LPCTSTR GetLevelDescribe(SCORE lScore);
	//等级描述
	virtual LPCTSTR GetLevelDescribe(IClientUserItem * pIClientUserItem);

	//数据接口
public:
	//获取等级
	virtual WORD GetGameLevelItem(tagLevelItem LevelItem[], WORD wMaxCount);
	//设置等级
	virtual bool SetGameLevelItem(tagLevelItem LevelItem[], WORD wItemCount);
	//加载等级
	virtual bool LoadGameLevelItem(LPCTSTR pszKindName, LPCTSTR pszDirectory, WORD wGameGenre);

	//内部函数
private:
	//默认等级
	LPCTSTR GetDefaultLevel(SCORE lScore);
	//获取子项
	tagLevelDescribe * CreateLevelDescribe();
};

//////////////////////////////////////////////////////////////////////////////////

#endif