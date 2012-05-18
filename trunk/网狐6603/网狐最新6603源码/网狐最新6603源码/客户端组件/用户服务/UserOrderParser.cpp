#include "StdAfx.h"
#include "UserOrderParser.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CUserOrderParser::CUserOrderParser()
{
	//��������
	m_MapMemberDescribe.InitHashTable(23);
	m_MapMasterDescribe.InitHashTable(23);

	//Ĭ������
	m_MapMemberDescribe[1]=TEXT("��ͨ��Ա");
	m_MapMemberDescribe[2]=TEXT("�м���Ա");
	m_MapMemberDescribe[3]=TEXT("�߼���Ա");

	//Ĭ������
	m_MapMasterDescribe[1]=TEXT("��Ϸ����Ա");
	m_MapMasterDescribe[2]=TEXT("�м�����Ա");
	m_MapMasterDescribe[3]=TEXT("�߼�����Ա");

	return;
}

//��������
CUserOrderParser::~CUserOrderParser()
{
}

//�ӿڲ�ѯ
VOID * CUserOrderParser::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IUserOrderParser,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IUserOrderParser,Guid,dwQueryVer);
	return NULL;
}

//������Ŀ
BYTE CUserOrderParser::GetMemberParserItemCount()
{
	return (BYTE)m_MapMemberDescribe.GetCount();
}

//������Ŀ
BYTE CUserOrderParser::GetMasterParserItemCount()
{
	return (BYTE)m_MapMasterDescribe.GetCount();
}

//���ý���
VOID CUserOrderParser::SetMemberParser(tagMemberItem MemberItem[], WORD wItemCount)
{
	//��������
	m_MapMemberDescribe.RemoveAll();

	//���ý���
	for (WORD i=0;i<wItemCount;i++) 
	{
		LPCTSTR pszMemberName(MemberItem[i].szMemberName);
		m_MapMemberDescribe[MemberItem[i].cbMemberOrder]=pszMemberName;
	}

	return;
}

//���ý���
VOID CUserOrderParser::SetMasterParser(tagMasterItem MasterItem[], WORD wItemCount)
{
	//��������
	m_MapMasterDescribe.RemoveAll();

	//���ý���
	for (WORD i=0;i<wItemCount;i++) 
	{
		LPCTSTR pszMasterName(MasterItem[i].szMasterName);
		m_MapMasterDescribe[MasterItem[i].cbMasterOrder]=pszMasterName;
	}

	return;
}

//��ȡ����
LPCTSTR CUserOrderParser::GetMemberDescribe(BYTE cbMemberOrder)
{
	CMapMemberDescribe::CPair * pPair=m_MapMemberDescribe.PLookup(cbMemberOrder);
	return (pPair!=NULL)?((LPCTSTR)pPair->value):TEXT("");
}

//��ȡ����
LPCTSTR CUserOrderParser::GetMasterDescribe(BYTE cbMasterOrder)
{
	CMapMasterDescribe::CPair * pPair=m_MapMasterDescribe.PLookup(cbMasterOrder);
	return (pPair!=NULL)?((LPCTSTR)pPair->value):TEXT("");
}

//��ȡ����
BYTE CUserOrderParser::GetMemberParser(tagMemberItem MemberItem[], WORD wItemCount)
{
	//��������
	BYTE cbCopyCount=0;
	CString strMemberName;
	POSITION Position=m_MapMemberDescribe.GetStartPosition();

	//���ñ���
	while (Position!=NULL)
	{
		//����ж�
		if (cbCopyCount>=wItemCount) break;

		//��ȡ����
		BYTE cbMemberOrder=0;
		m_MapMemberDescribe.GetNextAssoc(Position,cbMemberOrder,strMemberName);

		//���ý���
		BYTE cbIndex=cbCopyCount++;
		MemberItem[cbIndex].cbMemberOrder=cbMemberOrder;
		lstrcpyn(MemberItem[cbIndex].szMemberName,strMemberName,CountArray(MemberItem[cbIndex].szMemberName));
	}

	return cbCopyCount;
}

//��ȡ����
BYTE CUserOrderParser::GetMasterParser(tagMasterItem MasterItem[], WORD wItemCount)
{
	//��������
	BYTE cbCopyCount=0;
	CString strMasterName;
	POSITION Position=m_MapMasterDescribe.GetStartPosition();

	//���ñ���
	while (Position!=NULL)
	{
		//����ж�
		if (cbCopyCount>=wItemCount) break;

		//��ȡ����
		BYTE cbMasterOrder=0;
		m_MapMasterDescribe.GetNextAssoc(Position,cbMasterOrder,strMasterName);

		//���ý���
		BYTE cbIndex=cbCopyCount++;
		MasterItem[cbIndex].cbMasterOrder=cbMasterOrder;
		lstrcpyn(MasterItem[cbIndex].szMasterName,strMasterName,CountArray(MasterItem[cbIndex].szMasterName));
	}

	return cbCopyCount;
}

//////////////////////////////////////////////////////////////////////////

//�����������
DECLARE_CREATE_MODULE(UserOrderParser);

//////////////////////////////////////////////////////////////////////////
