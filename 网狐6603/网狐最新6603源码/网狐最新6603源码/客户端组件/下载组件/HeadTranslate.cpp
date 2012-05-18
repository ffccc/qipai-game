#include "StdAfx.h"
#include "HeadTranslate.h"

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CHeadTranslate::CHeadTranslate()
{
	//设置变量
	m_wDataLength=0;
	m_pcbHeadData=NULL;

	//数据信息
	m_dwTotalFileSize=0L;
	ZeroMemory(m_szFileEntityTag,sizeof(m_szFileEntityTag));

	return;
}

//析构函数
CHeadTranslate::~CHeadTranslate()
{
}

//清理参数
VOID CHeadTranslate::CleanData()
{
	//设置变量
	m_wDataLength=0;
	m_pcbHeadData=NULL;

	//数据信息
	m_dwTotalFileSize=0L;
	ZeroMemory(m_szFileEntityTag,sizeof(m_szFileEntityTag));

	return;
}

//设置参数
VOID CHeadTranslate::SetHeadData(BYTE * pcbHeadData, WORD wDataLength)
{
	//设置变量
	m_wDataLength=wDataLength;
	m_pcbHeadData=pcbHeadData;

	//变量定义
	WORD wStartIndex=0;
	LPCSTR pszHeadString=(LPCSTR)m_pcbHeadData;

	//解释字符
	LPCSTR pszEntityTag="ETag: \"";
	LPCSTR pszFileLength="Content-Length: ";

	//解释数据
	for (WORD i=0;i<m_wDataLength-1;i++)
	{
		//换行判断
		if ((pszHeadString[i]=='\r')&&(pszHeadString[i+1]=='\n'))
		{
			//数据属性
			WORD wLineLength=i-wStartIndex,wFlagLength=0;
			LPCSTR pszLineData=pszHeadString+wStartIndex;

			//设置变量
			wStartIndex=(i+2);

			//文件标识
			wFlagLength=(WORD)strlen(pszEntityTag);
			if ((wLineLength>wFlagLength)&&(memcmp(pszLineData,pszEntityTag,wFlagLength*sizeof(CHAR))==0))
			{
				//变量定义
				CHAR szFileEntityTag[CountArray(m_szFileEntityTag)]="";
				WORD wCopyCount=__min(CountArray(m_szFileEntityTag),wLineLength-wFlagLength-1);

				//拷贝数据
				ASSERT((wLineLength-wFlagLength-1)<=CountArray(szFileEntityTag));
				CopyMemory(szFileEntityTag,pszLineData+wFlagLength,wCopyCount*sizeof(CHAR));

				//设置数据
				szFileEntityTag[wCopyCount]=0;
				lstrcpyn(m_szFileEntityTag,(LPCTSTR)(CA2CT(szFileEntityTag)),CountArray(m_szFileEntityTag));

				continue;
			}

			//文件长度
			wFlagLength=(WORD)strlen(pszFileLength);
			if ((wLineLength>wFlagLength)&&(memcmp(pszLineData,pszFileLength,wFlagLength*sizeof(CHAR))==0))
			{
				//设置数据
				m_dwTotalFileSize=atol(pszLineData+wFlagLength);

				continue;
			}
		}
	}

	return;
}

//获取数据
WORD CHeadTranslate::GetLineString(WORD wLineIndex, TCHAR szDataBuffer[], WORD wCharCount)
{
	//效验状态
	ASSERT((m_pcbHeadData!=NULL)&&(m_wDataLength>0));
	if ((m_pcbHeadData==NULL)||(m_wDataLength==0)) return 0;

	//变量定义
	WORD wStartIndex=0;
	WORD wLineSearch=0;
	LPCSTR pszHeadString=(LPCSTR)m_pcbHeadData;

	//提取数据
	for (WORD i=0;i<m_wDataLength-1;i++)
	{
		//换行判断
		if ((pszHeadString[i]=='\r')&&(pszHeadString[i+1]=='\n'))
		{
			//结束判断
			if (wLineSearch==wLineIndex)
			{
				//设置长度
				ASSERT(wCharCount>=(i-wStartIndex));
				WORD wCopyCount=__min(wCharCount,i-wStartIndex);

				//设置结果
				szDataBuffer[wCopyCount]=0;
				CopyMemory(szDataBuffer,pszHeadString+wStartIndex,wCopyCount*sizeof(CHAR));

				return wCopyCount;
			}

			//设置变量
			wLineSearch++;
			wStartIndex=(i+2);
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
