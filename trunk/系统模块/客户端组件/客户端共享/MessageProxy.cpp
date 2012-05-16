#include "StdAfx.h"
#include "MessageProxy.h"

//////////////////////////////////////////////////////////////////////////


//���캯��
CMessageProxy::CMessageProxy(void)
{
	m_pSkinRichEdit=NULL;
	m_MessageOption.crName=RGB(0,0,200);
	m_MessageOption.crSystemHead=RGB(200,0,0);
	m_MessageOption.crSystemString=RGB(0,0,200);
	lstrcpyn(m_MessageOption.szDefaultFont,TEXT("����"),CountArray(m_MessageOption.szDefaultFont));
	
	//��ȡĿ¼
	GetCurrentDirectory(sizeof(m_szPath),m_szPath);
}

//��������
CMessageProxy::~CMessageProxy(void)
{
}

//�ӿڲ�ѯ
void * __cdecl CMessageProxy::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IMessageProxy,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IMessageProxy,Guid,dwQueryVer);
	return NULL;
}

//���þ��
bool __cdecl CMessageProxy::SetRichEditHwnd(CSkinRichEdit * pSkinRichEdit)
{
	//Ч�����
	ASSERT(pSkinRichEdit!=NULL);
	if (pSkinRichEdit==NULL) return false;

	//���ñ���
	m_pSkinRichEdit=pSkinRichEdit;

	return true;
}

//������ɫ
bool __cdecl CMessageProxy::SetMessageOption(tagMessageOption & MessageOption)
{
	//���ò���
	m_MessageOption.crName=MessageOption.crName;
	m_MessageOption.crSystemHead=MessageOption.crSystemHead;
	m_MessageOption.crSystemString=MessageOption.crSystemString;
	if (MessageOption.szDefaultFont[0]==0) lstrcpyn(m_MessageOption.szDefaultFont,TEXT("����"),CountArray(m_MessageOption.szDefaultFont));
	else lstrcpyn(m_MessageOption.szDefaultFont,MessageOption.szDefaultFont,CountArray(m_MessageOption.szDefaultFont));

	return true;
}

//�����Ļ
bool __cdecl CMessageProxy::CleanScreen()
{
	if (m_pSkinRichEdit==NULL) return false;
	m_pSkinRichEdit->CleanScreen();
	return true;
}

//���ù���
bool __cdecl CMessageProxy::SetAutoScroll(bool bScroll)
{
	if (m_pSkinRichEdit==NULL) return false;
	//m_pSkinRichEdit->SetAutoScroll(bScroll);
	return true;
}

//������Ϣ
bool __cdecl CMessageProxy::LoadMessage(LPCTSTR pszFileName)
{
	if (m_pSkinRichEdit==NULL) return false;
	return m_pSkinRichEdit->LoadMessage(pszFileName);
}

//������Ϣ
bool __cdecl CMessageProxy::SaveMessage(LPCTSTR pszFileName)
{
	if (m_pSkinRichEdit==NULL) return false;
	return m_pSkinRichEdit->SaveMessage(pszFileName);
}

//����ͼƬ
bool __cdecl CMessageProxy::InsertImage(CBitmap * pBitmap)
{
	if (m_pSkinRichEdit==NULL) return false;
	return m_pSkinRichEdit->InsertImage(pBitmap);
}

//����ʱ��
bool __cdecl CMessageProxy::InsertSystemTime(COLORREF crColor)
{
	//Ч�����
	ASSERT(m_pSkinRichEdit!=NULL);
	if (m_pSkinRichEdit==NULL) return false;

	//��ȡʱ��
	SYSTEMTIME SystemTime;
	GetLocalTime(&SystemTime);
	TCHAR szTimeBuffer[64]=TEXT("");
	_snprintf(szTimeBuffer,sizeof(szTimeBuffer),TEXT("��%02d:%02d:%02d��"),SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond);

	//������Ϣ
	m_pSkinRichEdit->InsertString(szTimeBuffer,crColor);

	return true;
}

//�û�����
bool __cdecl CMessageProxy::InsertUserName(LPCTSTR pszUserName)
{
	//Ч�����
	ASSERT(pszUserName!=NULL);
	ASSERT(m_pSkinRichEdit!=NULL);
	if (pszUserName==NULL) return false;
	if (m_pSkinRichEdit==NULL) return false;

	POINT ptCusor;
	GetCursorPos(&ptCusor);

	//��������
	m_pSkinRichEdit->InsertString(TEXT("��"),m_MessageOption.crName);
	m_pSkinRichEdit->InsertString(pszUserName,m_MessageOption.crName);
	m_pSkinRichEdit->InsertString(TEXT("��"),m_MessageOption.crName);

	SetCursorPos(ptCusor.x, ptCusor.y);

	return true;
}

//ϵͳ��Ϣ
bool __cdecl CMessageProxy::InsertSystemString(LPCTSTR pszString, UINT uSize)
{
	//Ч�����
	ASSERT(pszString!=NULL);
	ASSERT(m_pSkinRichEdit!=NULL);
	if (pszString==NULL) return false;
	if (m_pSkinRichEdit==NULL) return false;

	POINT ptCusor;
	GetCursorPos(&ptCusor);

	//������Ϣ
	LPCTSTR pszSystem=TEXT("��ϵͳ��Ϣ��");
	m_pSkinRichEdit->InsertString(pszSystem,m_MessageOption.crSystemHead);
	m_pSkinRichEdit->InsertString(pszString,m_MessageOption.crSystemString);
	m_pSkinRichEdit->InsertString(TEXT("\n"),m_MessageOption.crSystemString);

	SetCursorPos(ptCusor.x, ptCusor.y);

	return true;
}

//��ɫ��Ϣ
bool __cdecl CMessageProxy::InsertShtickString(LPCTSTR pszString, COLORREF crColor, UINT uSize, bool bNewLine)
{
	//Ч�����
	ASSERT(pszString!=NULL);
	ASSERT(m_pSkinRichEdit!=NULL);
	if (pszString==NULL) return false;
	if (m_pSkinRichEdit==NULL) return false;

	POINT ptCusor;
	GetCursorPos(&ptCusor);

	//������Ϣ
	TranslateInsert(pszString,crColor,uSize,m_MessageOption.szDefaultFont);
	if (bNewLine) m_pSkinRichEdit->InsertString(TEXT("\n"),crColor);

	SetCursorPos(ptCusor.x, ptCusor.y);

	return true;
}

//��ͨ��Ϣ
bool __cdecl CMessageProxy::InsertGeneralString(LPCTSTR pszString, COLORREF crColor, UINT uSize, bool bNewLine)
{
	//Ч�����
	ASSERT(pszString!=NULL);
	ASSERT(m_pSkinRichEdit!=NULL);
	if (pszString==NULL) return false;
	if (m_pSkinRichEdit==NULL) return false;

	POINT ptCusor;
	GetCursorPos(&ptCusor);

	//������Ϣ
	m_pSkinRichEdit->InsertString(pszString,crColor);
	if (bNewLine) m_pSkinRichEdit->InsertString(TEXT("\n"),crColor);

	SetCursorPos(ptCusor.x, ptCusor.y);

	return true;
}

//�û�����
bool __cdecl CMessageProxy::InsertUserChat(LPCTSTR pszSendName, LPCTSTR pszString, COLORREF crColor, UINT uSize)
{
	//Ч�����
	ASSERT(pszString!=NULL);
	ASSERT(pszSendName!=NULL);
	ASSERT(m_pSkinRichEdit!=NULL);
	if (pszString==NULL) return false;
	if (pszSendName==NULL) return false;
	if (m_pSkinRichEdit==NULL) return false;

	POINT ptCusor;
	GetCursorPos(&ptCusor);

	//��������
	m_pSkinRichEdit->InsertString(TEXT("��"),m_MessageOption.crName);
	m_pSkinRichEdit->InsertString(pszSendName,m_MessageOption.crName);
	m_pSkinRichEdit->InsertString(TEXT("��˵��"),m_MessageOption.crName);

	//������Ϣ
	TranslateInsert(pszString,crColor,uSize,m_MessageOption.szDefaultFont);
	m_pSkinRichEdit->InsertString(TEXT("\n"),crColor);

	SetCursorPos(ptCusor.x, ptCusor.y);

	return true;
}

//�û�����
bool __cdecl CMessageProxy::InsertUserChat(LPCTSTR pszSendName, LPCTSTR pszRecvName, LPCTSTR pszString, COLORREF crColor, UINT uSize)
{
	//Ч�����
	ASSERT(pszString!=NULL);
	ASSERT(pszSendName!=NULL);
	ASSERT(pszRecvName!=NULL);
	ASSERT(m_pSkinRichEdit!=NULL);
	if (pszString==NULL) return false;
	if (pszSendName==NULL) return false;
	if (pszRecvName==NULL) return false;
	if (m_pSkinRichEdit==NULL) return false;

	POINT ptCusor;
	GetCursorPos(&ptCusor);

	//��������
	m_pSkinRichEdit->InsertString(TEXT("��"),m_MessageOption.crName);
	m_pSkinRichEdit->InsertString(pszSendName,m_MessageOption.crName);
	m_pSkinRichEdit->InsertString(TEXT("�ݶԣ�"),m_MessageOption.crName);
	m_pSkinRichEdit->InsertString(pszRecvName,m_MessageOption.crName);
	m_pSkinRichEdit->InsertString(TEXT("��˵��"),m_MessageOption.crName);

	//������Ϣ
	TranslateInsert(pszString,crColor,uSize,m_MessageOption.szDefaultFont);
	m_pSkinRichEdit->InsertString(TEXT("\n"),crColor);

	SetCursorPos(ptCusor.x, ptCusor.y);

	return true;
}

//���Ͳ���
bool CMessageProxy::TranslateInsert(LPCTSTR pszString, COLORREF crColor, UINT uSize, LPCTSTR pszFontName)
{
	POINT ptCusor;
	GetCursorPos(&ptCusor);

	int nFaceIndex = -1;
	CString strBuffer=pszString;
	LPTSTR pszStringTemp=strBuffer.GetBuffer();
	int iCharLength=lstrlen(pszStringTemp),iHandleLength=0;
	TCHAR szFileName[MAX_PATH]=TEXT("");
	do
	{
		int iPassCharCount=0;
		nFaceIndex = TranslateChar(pszStringTemp+iHandleLength,iCharLength-iHandleLength,iPassCharCount);
		if (nFaceIndex!=-1)
		{
			//��ʾ����
			pszStringTemp[iHandleLength+iPassCharCount]=0;
			m_pSkinRichEdit->InsertString(pszStringTemp+iHandleLength,crColor);
			iHandleLength+=iPassCharCount+CExpression::m_ExpreesionItem[nFaceIndex].m_wTranceLength;

			//װ��ͼƬ			
			_snprintf(szFileName,sizeof(szFileName),TEXT("%s\\Face\\%d.gif"),m_szPath, nFaceIndex);
			m_pSkinRichEdit->InsertImage(szFileName);
		}
		else 
		{
			m_pSkinRichEdit->InsertString(pszStringTemp+iHandleLength,crColor);
			break;
		}
	} while (nFaceIndex!=-1);
	strBuffer.ReleaseBuffer();

	SetCursorPos(ptCusor.x, ptCusor.y);

	return true;
}

//�����ַ�
int CMessageProxy::TranslateChar(LPCTSTR szCharString, int iCharLength, int & iPassCharCount)
{
	POINT ptCusor;
	GetCursorPos(&ptCusor);

	CExpressionItem * pItem=NULL;
	int iItemLength=0,iInputLenth=0;
	for (int i=0;i<iCharLength-2;i++)
	{
		if ((szCharString[i]==TEXT('/'))&&(szCharString[i+1]==TEXT(':')))
		{
			for (int j=MAX_EXPRESS_ITEM-1;0<=j;j--)
			{
				pItem=&CExpression::m_ExpreesionItem[j];
				iInputLenth=iCharLength-i;
				iItemLength=pItem->m_wTranceLength;
				if (iItemLength>iInputLenth) continue;
				int k=2;
				for (;k<iItemLength;k++)
				{
					if ((szCharString[i+k]!=pItem->m_szTrancelate[k])&&(szCharString[i+k]!=(pItem->m_szTrancelate[k]+32))) break;
				}
				if (k==iItemLength) 
				{
					iPassCharCount=i;
					return j;
				}
			}
			i++;
		}
	}

	SetCursorPos(ptCusor.x, ptCusor.y);

	return -1;
}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * __cdecl CreateMessageProxy(const GUID & Guid, DWORD dwInterfaceVer)
{
	//��������
	CMessageProxy * pMessageProxy=NULL;
	try
	{
		pMessageProxy=new CMessageProxy();
		if (pMessageProxy==NULL) throw TEXT("����ʧ��");
		void * pObject=pMessageProxy->QueryInterface(Guid,dwInterfaceVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	catch (...) {}

	//�������
	SafeDelete(pMessageProxy);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
