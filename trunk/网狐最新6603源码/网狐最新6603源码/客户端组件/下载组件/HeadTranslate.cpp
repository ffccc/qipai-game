#include "StdAfx.h"
#include "HeadTranslate.h"

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CHeadTranslate::CHeadTranslate()
{
	//���ñ���
	m_wDataLength=0;
	m_pcbHeadData=NULL;

	//������Ϣ
	m_dwTotalFileSize=0L;
	ZeroMemory(m_szFileEntityTag,sizeof(m_szFileEntityTag));

	return;
}

//��������
CHeadTranslate::~CHeadTranslate()
{
}

//�������
VOID CHeadTranslate::CleanData()
{
	//���ñ���
	m_wDataLength=0;
	m_pcbHeadData=NULL;

	//������Ϣ
	m_dwTotalFileSize=0L;
	ZeroMemory(m_szFileEntityTag,sizeof(m_szFileEntityTag));

	return;
}

//���ò���
VOID CHeadTranslate::SetHeadData(BYTE * pcbHeadData, WORD wDataLength)
{
	//���ñ���
	m_wDataLength=wDataLength;
	m_pcbHeadData=pcbHeadData;

	//��������
	WORD wStartIndex=0;
	LPCSTR pszHeadString=(LPCSTR)m_pcbHeadData;

	//�����ַ�
	LPCSTR pszEntityTag="ETag: \"";
	LPCSTR pszFileLength="Content-Length: ";

	//��������
	for (WORD i=0;i<m_wDataLength-1;i++)
	{
		//�����ж�
		if ((pszHeadString[i]=='\r')&&(pszHeadString[i+1]=='\n'))
		{
			//��������
			WORD wLineLength=i-wStartIndex,wFlagLength=0;
			LPCSTR pszLineData=pszHeadString+wStartIndex;

			//���ñ���
			wStartIndex=(i+2);

			//�ļ���ʶ
			wFlagLength=(WORD)strlen(pszEntityTag);
			if ((wLineLength>wFlagLength)&&(memcmp(pszLineData,pszEntityTag,wFlagLength*sizeof(CHAR))==0))
			{
				//��������
				CHAR szFileEntityTag[CountArray(m_szFileEntityTag)]="";
				WORD wCopyCount=__min(CountArray(m_szFileEntityTag),wLineLength-wFlagLength-1);

				//��������
				ASSERT((wLineLength-wFlagLength-1)<=CountArray(szFileEntityTag));
				CopyMemory(szFileEntityTag,pszLineData+wFlagLength,wCopyCount*sizeof(CHAR));

				//��������
				szFileEntityTag[wCopyCount]=0;
				lstrcpyn(m_szFileEntityTag,(LPCTSTR)(CA2CT(szFileEntityTag)),CountArray(m_szFileEntityTag));

				continue;
			}

			//�ļ�����
			wFlagLength=(WORD)strlen(pszFileLength);
			if ((wLineLength>wFlagLength)&&(memcmp(pszLineData,pszFileLength,wFlagLength*sizeof(CHAR))==0))
			{
				//��������
				m_dwTotalFileSize=atol(pszLineData+wFlagLength);

				continue;
			}
		}
	}

	return;
}

//��ȡ����
WORD CHeadTranslate::GetLineString(WORD wLineIndex, TCHAR szDataBuffer[], WORD wCharCount)
{
	//Ч��״̬
	ASSERT((m_pcbHeadData!=NULL)&&(m_wDataLength>0));
	if ((m_pcbHeadData==NULL)||(m_wDataLength==0)) return 0;

	//��������
	WORD wStartIndex=0;
	WORD wLineSearch=0;
	LPCSTR pszHeadString=(LPCSTR)m_pcbHeadData;

	//��ȡ����
	for (WORD i=0;i<m_wDataLength-1;i++)
	{
		//�����ж�
		if ((pszHeadString[i]=='\r')&&(pszHeadString[i+1]=='\n'))
		{
			//�����ж�
			if (wLineSearch==wLineIndex)
			{
				//���ó���
				ASSERT(wCharCount>=(i-wStartIndex));
				WORD wCopyCount=__min(wCharCount,i-wStartIndex);

				//���ý��
				szDataBuffer[wCopyCount]=0;
				CopyMemory(szDataBuffer,pszHeadString+wStartIndex,wCopyCount*sizeof(CHAR));

				return wCopyCount;
			}

			//���ñ���
			wLineSearch++;
			wStartIndex=(i+2);
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
