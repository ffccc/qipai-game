#ifndef USER_INFORMATION_HEAD_FILE
#define USER_INFORMATION_HEAD_FILE

#pragma once

#include "ShareControlHead.h"

//////////////////////////////////////////////////////////////////////////////////
//�ṹ����

//��˵��
typedef CWHArray<tagCompanionInfo *> CCompanionInfoArray;				//�û���ϵ

//////////////////////////////////////////////////////////////////////////////////

//�û���Ϣ
class SHARE_CONTROL_CLASS CUserInformation : public IUserInformation
{
	//�ӿڱ���
protected:
	IUserCompanionSink *			m_pIUserCompanionSink;				//��ϵ�ص�

	//��ϵ��Ϣ
protected:
	CCompanionInfoArray				m_CompanionInfoActive;				//��������
	CCompanionInfoArray				m_CompanionInfoBuffer;				//�ͷ�����

	//��̬����
protected:
	static CUserInformation *		m_pUserInformation;					//��ϵ�ӿ�

	//��������
public:
	//���캯��
	CUserInformation();
	//��������
	virtual ~CUserInformation();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//������Ϣ
	virtual bool LoadInformation();
	//���ýӿ�
	virtual bool SetUserCompanionSink(IUnknownEx * pIUnknownEx);

	//��ϵ�ӿ�
public:
	//ö�ٹ�ϵ
	virtual tagCompanionInfo * EmunCompanionInfo(WORD wIndex);
	//Ѱ�ҹ�ϵ
	virtual tagCompanionInfo * SearchCompanionInfo(DWORD dwUserID);
	//�����ϵ
	virtual tagCompanionInfo * InsertCompanionInfo(IClientUserItem * pIClientUserItem, BYTE cbCompanion);

	//�ڲ�����
private:
	//������ϵ
	tagCompanionInfo * CreateCompanionItem();
	//�ͷŹ�ϵ
	bool DeleteCompanionItem(tagCompanionInfo * pCompanionInfo);

	//��̬����
public:
	//��ȡʵ��
	static CUserInformation * GetInstance() { return m_pUserInformation; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif