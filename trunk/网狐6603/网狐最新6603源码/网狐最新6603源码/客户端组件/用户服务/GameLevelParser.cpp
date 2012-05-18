#include "StdAfx.h"
#include "GameLevelParser.h"

//////////////////////////////////////////////////////////////////////////////////

//��̬����
CLevelDescribeArray		CGameLevelParser::m_LevelDescribeStorage;		//�洢����

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameLevelParser::CGameLevelParser()
{
	//���ñ���
	ZeroMemory(m_szKindName,sizeof(m_szKindName));

	return;
}

//��������
CGameLevelParser::~CGameLevelParser()
{
}

//�ӿڲ�ѯ
VOID * CGameLevelParser::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameLevelParser,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameLevelParser,Guid,dwQueryVer);
	return NULL;
}

//���صȼ�
bool CGameLevelParser::LoadGameLevelItem(LPCTSTR pszKindName, LPCTSTR pszDirectory, WORD wGameGenre)
{
	//Ч�����
	ASSERT(m_LevelDescribeActive.GetCount()==0);
	if (m_LevelDescribeActive.GetCount()!=0) return 0;

	//���ñ���
	lstrcpyn(m_szKindName,pszKindName,CountArray(m_szKindName));

	//��ҵȼ�
	if (wGameGenre==GAME_GENRE_GOLD)
	{
		//�ȼ�����
		LONG lLevelScore[]=
		{	
			0L,2000L,4000L,8000L,20000L,40000L,80000L,150000L,300000L,500000L,1000000L,
				2000000L,5000000L,10000000L,50000000L,100000000L,500000000L,1000000000L,
		};	

		//�ȼ�����
		LPCTSTR pszLevelName[]=
		{		
			TEXT("��ũ"),TEXT("�軧"),TEXT("�͹�"),TEXT("������"),TEXT("ũ����"),TEXT("����"),
				TEXT("�����"),TEXT("����"),TEXT("����"),TEXT("����"),TEXT("С����"),TEXT("�����"),			
				TEXT("�Ĺ�"),TEXT("����"),TEXT("����"),TEXT("��ʥ"),TEXT("����"),TEXT("ְҵ����")	
		};

		//Ч������	
		ASSERT(CountArray(pszLevelName)==(CountArray(lLevelScore)));

		//���صȼ�
		for (WORD i=0;i<CountArray(lLevelScore);i++)
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
			pLevelDescribe->lLevelScore=lLevelScore[i];
			lstrcpyn(pLevelDescribe->szLevelName,pszLevelName[i],CountArray(pLevelDescribe->szLevelName));

			//����ȼ�
			m_LevelDescribeActive.Add(pLevelDescribe);
		}

		return true;
	}

	//���ֵȼ�
	if (wGameGenre&(GAME_GENRE_SCORE|(GAME_GENRE_EDUCATE|GAME_GENRE_MATCH)))
	{
		//����Ŀ¼
		TCHAR szWorkDirectory[MAX_PATH]=TEXT("");
		CWHService::GetWorkDirectory(szWorkDirectory,CountArray(szWorkDirectory));

		//�ļ�Ŀ¼
		TCHAR szGameLevelName[MAX_PATH]=TEXT("");
		_sntprintf(szGameLevelName,CountArray(szGameLevelName),TEXT("%s\\%s\\GameLevel.ini"),szWorkDirectory,pszDirectory);

		//��������
		WORD wItemIndex=0;
		TCHAR szItemName[16]=TEXT("");
		TCHAR szReadData[256]=TEXT("");

		//��ȡ����
		do
		{
			//��ȡ�ַ�
			_sntprintf(szItemName,CountArray(szItemName),TEXT("LevelItem%d"),wItemIndex+1);
			GetPrivateProfileString(TEXT("LevelDescribe"),szItemName,TEXT(""),szReadData,sizeof(szReadData),szGameLevelName);

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
				for (WORD i=0;i<CountArray(pLevelDescribe->szLevelName)-1;i++)
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
		if (m_LevelDescribeActive.GetCount()==0L)
		{
			ASSERT(FALSE);
			return false;
		}

		return true;
	}

	return false;
}

//��ȡ�ȼ�
WORD CGameLevelParser::GetGameLevelItem(tagLevelItem LevelItem[], WORD wMaxCount)
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
		LevelItem[i].lLevelScore=pLevelDescribe->lLevelScore;
		lstrcpyn(LevelItem[i].szLevelName,pLevelDescribe->szLevelName,CountArray(LevelItem[i].szLevelName));
	}

	return (WORD)m_LevelDescribeActive.GetCount();
}

//���õȼ�
bool CGameLevelParser::SetGameLevelItem(tagLevelItem LevelItem[], WORD wItemCount)
{
	//Ч�����
	ASSERT(m_LevelDescribeActive.GetCount()==0);
	if (m_LevelDescribeActive.GetCount()!=0) return 0;

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
		pLevelDescribe->lLevelScore=LevelItem[i].lLevelScore;
		lstrcpyn(pLevelDescribe->szLevelName,LevelItem[i].szLevelName,CountArray(pLevelDescribe->szLevelName));

		//���ñ���
		m_LevelDescribeActive.Add(pLevelDescribe);
	}

	//���ñ���
	if (m_LevelDescribeActive.GetCount()==0)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//�ȼ�����
LPCTSTR CGameLevelParser::GetLevelDescribe(SCORE lScore)
{
	//״̬Ч��
	ASSERT(m_LevelDescribeActive.GetCount()>0L);
	if (m_LevelDescribeActive.GetCount()==0) return GetDefaultLevel(lScore);

	//��������
	WORD wItemCount=(WORD)m_LevelDescribeActive.GetCount();

	//�ȼ�����
	for (WORD i=0;i<wItemCount;i++)
	{
		tagLevelDescribe * pLevelDescribe=m_LevelDescribeActive[i];
		if ((lScore>=pLevelDescribe->lLevelScore)||(i==(wItemCount-1))) return pLevelDescribe->szLevelName;
	}

	return NULL;
}

//��Ϸ����
VOID CGameLevelParser::SetKindName(LPCTSTR pszKindName)
{
	//��������
	lstrcpyn(m_szKindName,pszKindName,CountArray(m_szKindName));

	return;
}

//�ȼ�����
LPCTSTR CGameLevelParser::GetLevelDescribe(IClientUserItem * pIClientUserItem)
{
	//״̬Ч��
	ASSERT(m_LevelDescribeActive.GetCount()>0L);
	if (m_LevelDescribeActive.GetCount()==0) return GetDefaultLevel(pIClientUserItem->GetUserScore());

	//��������
	SCORE lUserScore=pIClientUserItem->GetUserScore();
	INT_PTR nItemCount=m_LevelDescribeActive.GetCount();

	//�ȼ�����
	for (INT_PTR i=0;i<nItemCount;i++)
	{
		tagLevelDescribe * pLevelDescribe=m_LevelDescribeActive[nItemCount-i-1];
		if ((lUserScore>=pLevelDescribe->lLevelScore)||(i==(nItemCount-1))) return pLevelDescribe->szLevelName;
	}

	//�������
	ASSERT(FALSE);

	return NULL;
}

//Ĭ�ϵȼ�
LPCTSTR CGameLevelParser::GetDefaultLevel(SCORE lScore)
{
	//�ȼ�����
	static SCORE lLevelScore[]=
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
tagLevelDescribe * CGameLevelParser::CreateLevelDescribe()
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

//////////////////////////////////////////////////////////////////////////////////

//�����������
DECLARE_CREATE_MODULE(GameLevelParser);

//////////////////////////////////////////////////////////////////////////////////
