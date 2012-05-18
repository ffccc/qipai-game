#ifndef USER_ORDER_PARSER_HEAD_FILE
#define USER_ORDER_PARSER_HEAD_FILE

#pragma once

#include "AfxTempl.h"
#include "UserServiceHead.h"

//////////////////////////////////////////////////////////////////////////////////

//����˵��
typedef CMap<BYTE,BYTE,CString,CString &> CMapMemberDescribe;			//��������
typedef CMap<BYTE,BYTE,CString,CString &> CMapMasterDescribe;			//��������

//��Ա����
class USER_SERVICE_CLASS CUserOrderParser : public IUserOrderParser
{
	//��������
protected:
	CMapMemberDescribe				m_MapMemberDescribe;				//��Ա����
	CMapMasterDescribe				m_MapMasterDescribe;				//��������

	//��������
public:
	//���캯��
	CUserOrderParser();
	//��������
	virtual ~CUserOrderParser();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ýӿ�
public:
	//������Ŀ
	virtual BYTE GetMemberParserItemCount();
	//������Ŀ
	virtual BYTE GetMasterParserItemCount();
	//���ý���
	virtual VOID SetMemberParser(tagMemberItem MemberItem[], WORD wItemCount);
	//���ý���
	virtual VOID SetMasterParser(tagMasterItem MasterItem[], WORD wItemCount);

	//���ܽӿ�
public:
	//��ȡ����
	virtual LPCTSTR GetMemberDescribe(BYTE cbMemberOrder);
	//��ȡ����
	virtual LPCTSTR GetMasterDescribe(BYTE cbMasterOrder);
	//��ȡ����
	virtual BYTE GetMemberParser(tagMemberItem MemberItem[], WORD wItemCount);
	//��ȡ����
	virtual BYTE GetMasterParser(tagMasterItem MasterItem[], WORD wItemCount);
};

//////////////////////////////////////////////////////////////////////////////////

#endif