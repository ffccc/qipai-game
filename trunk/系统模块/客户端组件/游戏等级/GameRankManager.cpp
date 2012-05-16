#include "StdAfx.h"
#include "GameRankManager.h"

//typedef LPCTSTR (*PRANK)(LONG lScore);
//
////////////////////////////////////////////////////////////////////////////
//#include "StdAfx.h"
//#include "GameLevelParser.h"

//////////////////////////////////////////////////////////////////////////

//��̬����
CLevelDescribeArray		CGameRankManager::m_LevelDescribeStorage;		//�洢����

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameRankManager::CGameRankManager()
{
	//���ñ���
	m_bLoadLevel=false;

	return;
}

//��������
CGameRankManager::~CGameRankManager()
{
	//�ͷ�����
	UnLoadGameLevel();

	return;
}

//�ӿڲ�ѯ
VOID * __cdecl CGameRankManager::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameRankManager,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameRankManager,Guid,dwQueryVer);
	return NULL;
}

//���صȼ�
bool __cdecl CGameRankManager::LoadGameLevel(LPCTSTR pszDirectory, WORD wGameGenre)
{
	//�ͷŵȼ�
	ASSERT(m_bLoadLevel==false);
	if (m_bLoadLevel==true) UnLoadGameLevel();

	//�ļ�Ŀ¼
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\GameLevel.ini"),pszDirectory);

	//��������
	WORD wItemIndex=0;
	TCHAR szItemName[16]=TEXT("");
	TCHAR szReadData[256]=TEXT("");

	//��Ϸ�ȼ�
	//if (wGameGenre&(GAME_GENRE_SCORE|(GAME_GENRE_EDUCATE|GAME_GENRE_MATCH|GAME_GENRE_GOLD)))

	//��ȡ����
	do
	{
		//��ȡ�ַ�
		_sntprintf(szItemName,CountArray(szItemName),TEXT("LevelItem%d"),wItemIndex+1);
		GetPrivateProfileString(TEXT("LevelDescribe"),szItemName,TEXT(""),szReadData,sizeof(szReadData),szFileName);

		//��ȡ�ַ�
		if (szReadData[0]!=0)
		{
			//��ȡ����
			tagLevelDescribe * pLevelDescribe=CreateLevelDescribe();

			//������
			if (pLevelDescribe==NULL)
			{
				ASSERT(FALSE);
				return false;
			}

			//���ñ���
			pLevelDescribe->lLevelScore=0L;
			pLevelDescribe->szLevelName[0]=0;

			//��ȡ�ȼ�
			WORD wStringIndex=0;
			WORD i=0;
			for (;i<CountArray(pLevelDescribe->szLevelName)-1;i++)
			{
				//���Ǵ���
				if ((szReadData[i]==TEXT(','))||(szReadData[i]==0)) break;
				if ((wStringIndex==0)&&(szReadData[i]==TEXT(' '))) continue;

				//�����ַ�
				pLevelDescribe->szLevelName[wStringIndex++]=szReadData[i];
			}
			pLevelDescribe->szLevelName[wStringIndex]=0;

			//Ѱ�ҿ�ʼ
			LPCTSTR pszScore=&szReadData[i];
			while (((pszScore[0]>0)&&(pszScore[0]<TEXT('0')))||(pszScore[0]>TEXT('9'))) pszScore++;

			//��ȡ����
			while ((pszScore[0]>=TEXT('0'))&&(pszScore[0]<=TEXT('9')))
			{
				pLevelDescribe->lLevelScore=pLevelDescribe->lLevelScore*10L+pszScore[0]-TEXT('0');
				++pszScore;
			}

			//���ñ���
			wItemIndex++;
			m_LevelDescribeActive.Add(pLevelDescribe);
		}
		else break;

	} while (true);

	//���ñ���
	if (m_LevelDescribeActive.GetCount()>0L) m_bLoadLevel=true;

	return (m_LevelDescribeActive.GetCount()>0L);
}

//��ȡ�ȼ�
WORD __cdecl CGameRankManager::GetGameLevelItem(tagLevelItem LevelItem[], WORD wMaxCount)
{
	//Ч�����
	ASSERT(wMaxCount>=m_LevelDescribeActive.GetCount());
	if (wMaxCount<m_LevelDescribeActive.GetCount()) return 0;

	//��������
	for (INT_PTR i=0;i<m_LevelDescribeActive.GetCount();i++)
	{
		//��ȡ����
		tagLevelDescribe * pLevelDescribe=m_LevelDescribeActive[i];

		//��������
		CT2CW strLevelName(pLevelDescribe->szLevelName);
		LevelItem[i].lLevelScore=pLevelDescribe->lLevelScore;
		lstrcpynW(LevelItem[i].szLevelName,(LPCWSTR)strLevelName,CountArray(LevelItem[i].szLevelName));
	}

	return (WORD)m_LevelDescribeActive.GetCount();
}

//���õȼ�
bool __cdecl CGameRankManager::SetGameLevelItem(tagLevelItem LevelItem[], WORD wItemCount)
{
	//�ͷŵȼ�
	ASSERT(m_bLoadLevel==false);
	if (m_bLoadLevel==true) UnLoadGameLevel();

	//���õȼ�
	for (WORD i=0;i<wItemCount;i++)
	{
		//��ȡ����
		tagLevelDescribe * pLevelDescribe=CreateLevelDescribe();

		//������
		if (pLevelDescribe==NULL)
		{
			ASSERT(FALSE);
			return false;
		}

		//���ñ���
		CW2CT strLevelName(LevelItem[i].szLevelName);
		pLevelDescribe->lLevelScore=LevelItem[i].lLevelScore;
		lstrcpyn(pLevelDescribe->szLevelName,(LPCTSTR)strLevelName,CountArray(pLevelDescribe->szLevelName));

		//���ñ���
		m_LevelDescribeActive.Add(pLevelDescribe);
	}

	//���ñ���
	if (m_LevelDescribeActive.GetCount()>0) m_bLoadLevel=true;

	return true;
}

//�ͷŵȼ�
bool __cdecl CGameRankManager::UnLoadGameLevel()
{
	//���ñ���
	m_bLoadLevel=false;

	//�ͷŶ���
	m_LevelDescribeStorage.Append(m_LevelDescribeActive);
	m_LevelDescribeActive.RemoveAll();

	return true;
}

//�ȼ�����
LPCTSTR __cdecl CGameRankManager::GetLevelDescribe(LONGLONG lScore)
{
	//״̬Ч��
	//ASSERT(m_LevelDescribeActive.GetCount()>0L);
	if (m_LevelDescribeActive.GetCount()==0) return GetDefaultLevel(lScore);

	//��������
	WORD wItemCount=(WORD)m_LevelDescribeActive.GetCount();

	//�ȼ�����
	for (INT i=wItemCount-1;i>=0;i--)
	{
		tagLevelDescribe * pLevelDescribe=m_LevelDescribeActive[i];
		if ((lScore>=pLevelDescribe->lLevelScore)||(i==0)) return pLevelDescribe->szLevelName;
	}

	return NULL;
}

//Ĭ�ϵȼ�
LPCTSTR CGameRankManager::GetDefaultLevel(LONGLONG lScore)
{
	//�ȼ�����
	static LONGLONG lLevelScore[]=
	{	0L,			2000L,		4000L,		8000L,		20000L,
		40000L,		80000L,		150000L,	300000L,	500000L,
		1000000L,	2000000L,	5000000L,	10000000L,	50000000L,
		100000000L,	500000000L,	1000000000L,
	};	
	
	//�ȼ�����
	static LPCTSTR pszLevelName[]=	
	{		
		TEXT("��ũ"),TEXT("�軧"),TEXT("�͹�"),TEXT("������"),			
			TEXT("ũ����"),TEXT("����"),TEXT("�����"),TEXT("����"),			
			TEXT("����"),TEXT("����"),TEXT("С����"),TEXT("�����"),			
			TEXT("�Ĺ�"),TEXT("����"),TEXT("����"),TEXT("��ʥ"),			
			TEXT("����"),TEXT("ְҵ����")	
	};

	//Ч������	
	ASSERT(CountArray(pszLevelName)==(CountArray(lLevelScore)));	

	//��ȡ�ȼ�	
	for (INT i=0;i<CountArray(lLevelScore);i++)	
	{		
		if (lScore>=lLevelScore[CountArray(lLevelScore)-i-1]) return pszLevelName[i];	
	}	
	
	return pszLevelName[0];
}

//��ȡ����
tagLevelDescribe * CGameRankManager::CreateLevelDescribe()
{
	//��ȡ����
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

//////////////////////////////////////////////////////////////////////////

extern "C" __declspec(dllexport) void * __cdecl CreateGameRankManager(const GUID & Guid, DWORD dwInterfaceVer)
{
	//��������
	CGameRankManager *pGameRankManager=NULL;
	try
	{
		pGameRankManager = new CGameRankManager();
		if(pGameRankManager==NULL) 
		{
			throw _T("����ʧ��");
		}

		void *pObject = pGameRankManager->QueryInterface(Guid,dwInterfaceVer);
		if(pObject==NULL) 
		{
			throw _T("�ӿڲ�ѯʧ��");
		}
		return pObject;
	}
	catch (...) 
	{
	}

	//�������
	SafeDelete(pGameRankManager);

	return NULL;
}

//////////////////////////////////////////////////////////////////////////

