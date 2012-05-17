#include "StdAfx.h"
#include "GlobalUserInfo.h"

//////////////////////////////////////////////////////////////////////////////////

//��̬����
CGlobalUserInfo * CGlobalUserInfo::m_pGlobalUserInfo=NULL;				//�û���Ϣ

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGlobalUserInfo::CGlobalUserInfo()
{
	//���ö���
	ASSERT(m_pGlobalUserInfo==NULL);
	if (m_pGlobalUserInfo==NULL) m_pGlobalUserInfo=this;

	//���ñ���
	ZeroMemory(&m_GlobalUserData,sizeof(m_GlobalUserData));
	ZeroMemory(&m_IndividualUserData,sizeof(m_IndividualUserData));

	return;
}

//��������
CGlobalUserInfo::~CGlobalUserInfo()
{
	//�ͷŶ���
	ASSERT(m_pGlobalUserInfo==this);
	if (m_pGlobalUserInfo==this) m_pGlobalUserInfo=NULL;

	return;
}

//��������
VOID CGlobalUserInfo::ResetUserInfoData()
{
	//���ñ���
	ZeroMemory(&m_GlobalUserData,sizeof(m_GlobalUserData));
	ZeroMemory(&m_IndividualUserData,sizeof(m_IndividualUserData));

	return;
}

//////////////////////////////////////////////////////////////////////////////////
