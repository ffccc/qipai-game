#include "StdAfx.h"
#include "GameLevelParser.h"

//////////////////////////////////////////////////////////////////////////////////

//静态变量
CLevelDescribeArray		CGameLevelParser::m_LevelDescribeStorage;		//存储数组

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameLevelParser::CGameLevelParser()
{
	//设置变量
	ZeroMemory(m_szKindName,sizeof(m_szKindName));

	return;
}

//析构函数
CGameLevelParser::~CGameLevelParser()
{
}

//接口查询
VOID * CGameLevelParser::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameLevelParser,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameLevelParser,Guid,dwQueryVer);
	return NULL;
}

//加载等级
bool CGameLevelParser::LoadGameLevelItem(LPCTSTR pszKindName, LPCTSTR pszDirectory, WORD wGameGenre)
{
	//效验参数
	ASSERT(m_LevelDescribeActive.GetCount()==0);
	if (m_LevelDescribeActive.GetCount()!=0) return 0;

	//设置变量
	lstrcpyn(m_szKindName,pszKindName,CountArray(m_szKindName));

	//金币等级
	if (wGameGenre==GAME_GENRE_GOLD)
	{
		//等级积分
		LONG lLevelScore[]=
		{	
			0L,2000L,4000L,8000L,20000L,40000L,80000L,150000L,300000L,500000L,1000000L,
				2000000L,5000000L,10000000L,50000000L,100000000L,500000000L,1000000000L,
		};	

		//等级名称
		LPCTSTR pszLevelName[]=
		{		
			TEXT("务农"),TEXT("佃户"),TEXT("雇工"),TEXT("作坊主"),TEXT("农场主"),TEXT("地主"),
				TEXT("大地主"),TEXT("财主"),TEXT("富翁"),TEXT("大富翁"),TEXT("小财神"),TEXT("大财神"),			
				TEXT("赌棍"),TEXT("赌侠"),TEXT("赌王"),TEXT("赌圣"),TEXT("赌神"),TEXT("职业赌神")	
		};

		//效验数据	
		ASSERT(CountArray(pszLevelName)==(CountArray(lLevelScore)));

		//加载等级
		for (WORD i=0;i<CountArray(lLevelScore);i++)
		{
			//获取子项
			tagLevelDescribe * pLevelDescribe=CreateLevelDescribe();

			//错误处理
			if (pLevelDescribe==NULL)
			{
				ASSERT(FALSE);
				return false;
			}

			//设置变量
			pLevelDescribe->lLevelScore=lLevelScore[i];
			lstrcpyn(pLevelDescribe->szLevelName,pszLevelName[i],CountArray(pLevelDescribe->szLevelName));

			//插入等级
			m_LevelDescribeActive.Add(pLevelDescribe);
		}

		return true;
	}

	//积分等级
	if (wGameGenre&(GAME_GENRE_SCORE|(GAME_GENRE_EDUCATE|GAME_GENRE_MATCH)))
	{
		//工作目录
		TCHAR szWorkDirectory[MAX_PATH]=TEXT("");
		CWHService::GetWorkDirectory(szWorkDirectory,CountArray(szWorkDirectory));

		//文件目录
		TCHAR szGameLevelName[MAX_PATH]=TEXT("");
		_sntprintf(szGameLevelName,CountArray(szGameLevelName),TEXT("%s\\%s\\GameLevel.ini"),szWorkDirectory,pszDirectory);

		//变量定义
		WORD wItemIndex=0;
		TCHAR szItemName[16]=TEXT("");
		TCHAR szReadData[256]=TEXT("");

		//读取资料
		do
		{
			//读取字符
			_sntprintf(szItemName,CountArray(szItemName),TEXT("LevelItem%d"),wItemIndex+1);
			GetPrivateProfileString(TEXT("LevelDescribe"),szItemName,TEXT(""),szReadData,sizeof(szReadData),szGameLevelName);

			//读取字符
			if (szReadData[0]!=0)
			{
				//获取子项
				tagLevelDescribe * pLevelDescribe=CreateLevelDescribe();

				//错误处理
				if (pLevelDescribe==NULL)
				{
					ASSERT(FALSE);
					return false;
				}

				//设置变量
				pLevelDescribe->lLevelScore=0L;
				pLevelDescribe->szLevelName[0]=0;

				//读取等级
				WORD wStringIndex=0;
				for (WORD i=0;i<CountArray(pLevelDescribe->szLevelName)-1;i++)
				{
					//过虑处理
					if ((szReadData[i]==TEXT(','))||(szReadData[i]==0)) break;
					if ((wStringIndex==0)&&(szReadData[i]==TEXT(' '))) continue;

					//设置字符
					pLevelDescribe->szLevelName[wStringIndex++]=szReadData[i];
				}
				pLevelDescribe->szLevelName[wStringIndex]=0;

				//寻找开始
				LPCTSTR pszScore=&szReadData[i];
				while (((pszScore[0]>0)&&(pszScore[0]<TEXT('0')))||(pszScore[0]>TEXT('9'))) pszScore++;

                //读取积分
				while ((pszScore[0]>=TEXT('0'))&&(pszScore[0]<=TEXT('9')))
				{
					pLevelDescribe->lLevelScore=pLevelDescribe->lLevelScore*10L+pszScore[0]-TEXT('0');
					++pszScore;
				}

				//设置变量
				wItemIndex++;
				m_LevelDescribeActive.Add(pLevelDescribe);
			}
			else break;

		} while (true);

		//设置变量
		if (m_LevelDescribeActive.GetCount()==0L)
		{
			ASSERT(FALSE);
			return false;
		}

		return true;
	}

	return false;
}

//获取等级
WORD CGameLevelParser::GetGameLevelItem(tagLevelItem LevelItem[], WORD wMaxCount)
{
	//效验参数
	ASSERT(wMaxCount>=m_LevelDescribeActive.GetCount());
	if (wMaxCount<m_LevelDescribeActive.GetCount()) return 0;

	//拷贝数据
	for (INT_PTR i=0;i<m_LevelDescribeActive.GetCount();i++)
	{
		//获取数据
		tagLevelDescribe * pLevelDescribe=m_LevelDescribeActive[i];

		//拷贝数据
		LevelItem[i].lLevelScore=pLevelDescribe->lLevelScore;
		lstrcpyn(LevelItem[i].szLevelName,pLevelDescribe->szLevelName,CountArray(LevelItem[i].szLevelName));
	}

	return (WORD)m_LevelDescribeActive.GetCount();
}

//设置等级
bool CGameLevelParser::SetGameLevelItem(tagLevelItem LevelItem[], WORD wItemCount)
{
	//效验参数
	ASSERT(m_LevelDescribeActive.GetCount()==0);
	if (m_LevelDescribeActive.GetCount()!=0) return 0;

	//设置等级
	for (WORD i=0;i<wItemCount;i++)
	{
		//获取子项
		tagLevelDescribe * pLevelDescribe=CreateLevelDescribe();

		//错误处理
		if (pLevelDescribe==NULL)
		{
			ASSERT(FALSE);
			return false;
		}

		//设置变量
		pLevelDescribe->lLevelScore=LevelItem[i].lLevelScore;
		lstrcpyn(pLevelDescribe->szLevelName,LevelItem[i].szLevelName,CountArray(pLevelDescribe->szLevelName));

		//设置变量
		m_LevelDescribeActive.Add(pLevelDescribe);
	}

	//设置变量
	if (m_LevelDescribeActive.GetCount()==0)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//等级描述
LPCTSTR CGameLevelParser::GetLevelDescribe(SCORE lScore)
{
	//状态效验
	ASSERT(m_LevelDescribeActive.GetCount()>0L);
	if (m_LevelDescribeActive.GetCount()==0) return GetDefaultLevel(lScore);

	//变量定义
	WORD wItemCount=(WORD)m_LevelDescribeActive.GetCount();

	//等级搜索
	for (WORD i=0;i<wItemCount;i++)
	{
		tagLevelDescribe * pLevelDescribe=m_LevelDescribeActive[i];
		if ((lScore>=pLevelDescribe->lLevelScore)||(i==(wItemCount-1))) return pLevelDescribe->szLevelName;
	}

	return NULL;
}

//游戏名字
VOID CGameLevelParser::SetKindName(LPCTSTR pszKindName)
{
	//设置名字
	lstrcpyn(m_szKindName,pszKindName,CountArray(m_szKindName));

	return;
}

//等级描述
LPCTSTR CGameLevelParser::GetLevelDescribe(IClientUserItem * pIClientUserItem)
{
	//状态效验
	ASSERT(m_LevelDescribeActive.GetCount()>0L);
	if (m_LevelDescribeActive.GetCount()==0) return GetDefaultLevel(pIClientUserItem->GetUserScore());

	//变量定义
	SCORE lUserScore=pIClientUserItem->GetUserScore();
	INT_PTR nItemCount=m_LevelDescribeActive.GetCount();

	//等级搜索
	for (INT_PTR i=0;i<nItemCount;i++)
	{
		tagLevelDescribe * pLevelDescribe=m_LevelDescribeActive[nItemCount-i-1];
		if ((lUserScore>=pLevelDescribe->lLevelScore)||(i==(nItemCount-1))) return pLevelDescribe->szLevelName;
	}

	//错误断言
	ASSERT(FALSE);

	return NULL;
}

//默认等级
LPCTSTR CGameLevelParser::GetDefaultLevel(SCORE lScore)
{
	//等级积分
	static SCORE lLevelScore[]=
	{	0L,			2000L,		4000L,		8000L,		20000L,
		40000L,		80000L,		150000L,	300000L,	500000L,
		1000000L,	2000000L,	5000000L,	10000000L,	50000000L,
		100000000L,	500000000L,	1000000000L,
	};	
	
	//等级名称
	static LPCTSTR pszLevelName[]=	
	{		
		TEXT("务农"),TEXT("佃户"),TEXT("雇工"),TEXT("作坊主"),			
			TEXT("农场主"),TEXT("地主"),TEXT("大地主"),TEXT("财主"),			
			TEXT("富翁"),TEXT("大富翁"),TEXT("小财神"),TEXT("大财神"),			
			TEXT("赌棍"),TEXT("赌侠"),TEXT("赌王"),TEXT("赌圣"),			
			TEXT("赌神"),TEXT("职业赌神")	
	};

	//效验数据	
	ASSERT(CountArray(pszLevelName)==(CountArray(lLevelScore)));	

	//获取等级	
	for (INT i=0;i<CountArray(lLevelScore);i++)	
	{		
		if (lScore>=lLevelScore[CountArray(lLevelScore)-i-1]) return pszLevelName[i];	
	}	
	
	return pszLevelName[0];
}

//获取子项
tagLevelDescribe * CGameLevelParser::CreateLevelDescribe()
{
	//获取子项
	tagLevelDescribe * pLevelDescribe=NULL;
	INT_PTR nStorageCount=m_LevelDescribeStorage.GetCount();
	if (nStorageCount>0)
	{
		pLevelDescribe=m_LevelDescribeStorage[nStorageCount-1];
		ASSERT(pLevelDescribe!=NULL);
		m_LevelDescribeStorage.RemoveAt(nStorageCount-1);
	}
	else
	{
		pLevelDescribe=new tagLevelDescribe;
		ASSERT(pLevelDescribe!=NULL);
		if (pLevelDescribe==NULL) return NULL;
	}

	return pLevelDescribe;
}

//////////////////////////////////////////////////////////////////////////////////

//组件创建函数
DECLARE_CREATE_MODULE(GameLevelParser);

//////////////////////////////////////////////////////////////////////////////////
