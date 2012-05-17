#ifndef GLOBAL_USER_INFO_HEAD_FILE
#define GLOBAL_USER_INFO_HEAD_FILE

#pragma once

#include "PlatformDataHead.h"

//////////////////////////////////////////////////////////////////////////////////
//�ṹ����

//�û���Ϣ
struct tagGlobalUserData
{
	//��������
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwUserMedal;						//�û�����
	DWORD							dwExperience;						//�û�����
	DWORD							dwLoveLiness;						//�û�����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����

	//��չ����
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMoorMachine;						//��������
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��

	//��������
	DWORD							dwGroupID;							//��������
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������

	//��Ա����
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	SYSTEMTIME						MemberOverDate;						//����ʱ��

	//ͷ����Ϣ
	WORD							wFaceID;							//ͷ������
	DWORD							dwCustomID;							//�Զ���ʶ
	tagCustomFaceInfo				CustomFaceInfo;						//�Զ�ͷ��
};

//��չ����
struct tagIndividualUserData
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szUserNote[LEN_USER_NOTE];			//�û�˵��
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����

	//�绰����
	TCHAR							szSeatPhone[LEN_SEAT_PHONE];		//�̶��绰
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ƶ��绰

	//��ϵ����
	TCHAR							szQQ[LEN_QQ];						//Q Q ����
	TCHAR							szEMail[LEN_EMAIL];					//�����ʼ�
	TCHAR							szDwellingPlace[LEN_DWELLING_PLACE];//��ϵ��ַ
};

//////////////////////////////////////////////////////////////////////////////////

//�û���Ϣ
class PLATFORM_DATA_CLASS CGlobalUserInfo
{
	//�û���Ϣ
protected:
	tagGlobalUserData				m_GlobalUserData;						//�û�����
	tagIndividualUserData			m_IndividualUserData;					//��չ����

	//��̬����
protected:
	static CGlobalUserInfo *		m_pGlobalUserInfo;						//�û���Ϣ

	//��������
public:
	//���캯��
	CGlobalUserInfo();
	//��������
	virtual ~CGlobalUserInfo();

	//���ܺ���
public:
	//��������
	VOID ResetUserInfoData();
	//�û�����
	tagGlobalUserData * GetGlobalUserData() { return &m_GlobalUserData; }
	//��չ����
	tagIndividualUserData * GetIndividualUserData() { return &m_IndividualUserData; }

	//��̬����
public:
	//��ȡ����
	static CGlobalUserInfo * GetInstance() { return m_pGlobalUserInfo; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif