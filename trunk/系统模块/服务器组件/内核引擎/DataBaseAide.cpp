#include "StdAfx.h"
#include "DataBaseAide.h"
#include "DataBase.h"

//���캯��
CDataBaseAide::CDataBaseAide(IUnknownEx * pIUnknownEx)
{
}
//��������
CDataBaseAide::~CDataBaseAide()
{
}

bool CDataBaseAide::SetDataBase(IUnknownEx *pIUnKnownEx)
{
	m_pIDataBase = (IDataBase *)pIUnKnownEx;
	if (m_pIDataBase != NULL)
		return true;
	else
		return false;
}

VOID *CDataBaseAide::GetDataBase(REFGUID Guid, DWORD dwQueryVer)
{
	if (m_pIDataBase != NULL)
		return m_pIDataBase;
	return NULL;
}
/////////////////////////////////
////ȡ����

INT CDataBaseAide::GetValue_INT(LPCTSTR pszItem)
{
	INT nValue;
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetFieldValue(pszItem, nValue);
		return nValue;
	}
	return 0;
}

UINT CDataBaseAide::GetValue_UINT(LPCTSTR pszItem)
{
	UINT ulValue;
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetFieldValue(pszItem, ulValue);
		return ulValue;

	}
	return 0;
}

DOUBLE CDataBaseAide::GetValue_DOUBLE(LPCTSTR pszItem)
{
	DOUBLE  dbValue;
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetFieldValue(pszItem, dbValue);
		return dbValue;

	}
	return 0;
}

BYTE CDataBaseAide::GetValue_BYTE(LPCTSTR pszItem)
{
	BYTE  bValue;
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetFieldValue(pszItem, bValue);
		return bValue;
	}

	return 0;
}

LONG CDataBaseAide::GetValue_LONG(LPCTSTR pszItem)
{
	LONG  lValue;
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetFieldValue(pszItem, lValue);
		return lValue;

	}
	return 0;

}

WORD CDataBaseAide::GetValue_WORD(LPCTSTR pszItem)
{
	WORD  wValue;
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetFieldValue(pszItem, wValue);
		return wValue;

	}
	return 0;

}

DWORD CDataBaseAide::GetValue_DWORD(LPCTSTR pszItem)
{
	DWORD  ulValue;
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetFieldValue(pszItem, ulValue);
		return ulValue;

	}
	return 0;
}

FLOAT CDataBaseAide::GetValue_FLOAT(LPCTSTR pszItem)
{

	return 0;

}

LONGLONG CDataBaseAide::GetValue_LONGLONG(LPCTSTR pszItem)
{
	__int64 llValue;
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetFieldValue(pszItem, llValue);
		return llValue;

	}
	return 0;
}

VOID CDataBaseAide::GetValue_VarValue(LPCTSTR pszItem, CDBVarValue & DBVarValue)
{
	return;

}

VOID CDataBaseAide::GetValue_SystemTime(LPCTSTR pszItem, SYSTEMTIME & SystemTime)
{
	return;
}


//��ȡ����
VOID CDataBaseAide::GetParameter(LPCTSTR pszItem, CDBVarValue & DBVarValue)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetParameter(pszItem, DBVarValue);
	}

}

//���ò���
VOID CDataBaseAide::ResetParameter()
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->ClearParameters();
		((CDataBase*)m_pIDataBase)->AddParameter(TEXT("RETURN_VALUE"), adInteger, adParamReturnValue, sizeof(long), _variant_t((long)0));
	}

}

//��ȡ�ַ�
VOID CDataBaseAide::GetValue_String(LPCTSTR pszItem, LPSTR pszString, UINT uMaxCount)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetFieldValue(pszItem, (char *)pszString, uMaxCount);
	}
}

//��ȡ�ַ�
VOID CDataBaseAide::GetValue_String(LPCTSTR pszItem, LPWSTR pszString, UINT uMaxCount)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetFieldValue(pszItem, (char *)pszString, uMaxCount);
	}
}

//�洢����
LONG CDataBaseAide::ExecuteProcess(LPCTSTR pszSPName, bool bRecordset)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->ExecuteProcess(pszSPName, bRecordset);
		return ((CDataBase*)m_pIDataBase)->GetReturnValue();
	}
	return -1;

}

//�������
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, INT nValue, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adInteger, adParamInput, sizeof(long), _variant_t((long)nValue));
	}
}

//�������
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, UINT uValue, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adInteger, adParamInput, sizeof(long), _variant_t((long)uValue));
	}

}

//�������
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, LONG lValue, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adInteger, adParamInput, sizeof(long), _variant_t((long)lValue));
	}
}

//�������
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, LONGLONG lValue, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adInteger, adParamInput, sizeof(long), _variant_t((long)lValue));
	}
}

//�������
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, BYTE cbValue, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adInteger, adParamInput, sizeof(long), _variant_t((long)cbValue));
	}
}

//�������
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, WORD wValue, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adInteger, adParamInput, sizeof(long), _variant_t((long)wValue));
	}
}

//�������
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, DWORD dwValue, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adInteger, adParamInput, sizeof(long), _variant_t((long)dwValue));
	}
}

//�������
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, FLOAT fValue, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adInteger, adParamInput, sizeof(long), _variant_t((long)fValue));
	}
}

//�������
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, DOUBLE dValue, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adInteger, adParamInput, sizeof(long), _variant_t((long)dValue));
	}
}

//�������
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, LPCTSTR pszString, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		if (strlen(pszString) != 0)
		{
			((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adChar, adParamInput, lstrlen(pszString), _variant_t(pszString));
		}
		else
		{
			//TCHAR szString[64]=" ";
			((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adChar, adParamInput, lstrlen(pszString) + 1, _variant_t(pszString));
		}
	}
}

#if 0  //
//�������
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, LPCWSTR pszString, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adParamInput, adVarChar, lstrlen(pszString), _variant_t(pszString));
	}
}
#endif

//�������
VOID CDataBaseAide::AddParameterOutput(LPCTSTR pszItem, LPSTR pszString, UINT uSize, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		return;
	}
}
//�������
VOID CDataBaseAide::AddParameterOutput(LPCTSTR pszItem, LPWSTR pszString, UINT uSize, ParameterDirectionEnum ParameterDirection)
{

	if (m_pIDataBase != NULL)
	{
		return;
	}
}












