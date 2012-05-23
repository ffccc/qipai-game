#ifndef COMMONALITY_HEAD_FILE
#define COMMONALITY_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//���ݿ������

//���ݿ������ʶ
#define	DBR_GP_LOGON_BY_ACCOUNTS	1									//�ʺŵ�¼
#define	DBR_GP_LOGON_BY_USERID		2									//I D ��¼
#define DBR_GP_REGISTER_ACCOUNTS	3									//ע���ʺ�
#define DBR_GP_USER_UPLOAD_FACE		4									//�ϴ�ͷ��
#define DBR_GP_USER_LEAVE			5									//����뿪
#define DBR_GP_DOWNLOAD_FACE		6									//����ͷ��
#define DBR_GP_DELETE_FACE			7									//ɾ��ͷ��
#define DBR_GP_MODIFY_INDIVIDUAL	8									//�޸�����
//#define DBR_GR_TRANSFER				9									//ת��


//���ݿ������ʶ
#define DBR_GP_LOGON_SUCCESS		100									//��¼�ɹ�
#define DBR_GP_LOGON_ERROR			101									//��¼ʧ��
#define DBR_GP_DOWNLOADFACE_SUCCESS	102									//���سɹ�
#define DBR_GP_UPLOAD_FACE_RESULT	103									//�ϴ����
#define DBR_GP_DELETE_FACE_RESULT	104									//ɾ�����
#define DBR_GP_MODIFY_RESULT		105									//�޸Ľ��

//�ʺŵ�¼
struct DBR_GP_LogonByAccounts
{
	DWORD							dwClientIP;							//���ӵ�ַ
	TCHAR							szAccounts[NAME_LEN];				//��¼�ʺ�
	TCHAR							szPassWord[PASS_LEN];				//��¼����
	TCHAR							szComputerID[COMPUTER_ID_LEN];		//��������
};

//ID ��¼
struct DBR_GP_LogonByUserID
{
	DWORD							dwClientIP;							//���ӵ�ַ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassWord[PASS_LEN];				//��¼����
	TCHAR							szComputerID[COMPUTER_ID_LEN];		//��������
};

//�ʺ�ע��
struct DBR_GP_RegisterAccounts
{
	WORD							wFaceID;							//ע��ͷ��
	BYTE							cbGender;							//�û��Ա�
	DWORD							dwClientIP;							//���ӵ�ַ
	TCHAR							szSpreader[NAME_LEN];				//�Ƽ�����
	TCHAR							szAccounts[NAME_LEN];				//��¼�ʺ�
	TCHAR							szPassWord[PASS_LEN];				//��¼����
	TCHAR							szPassWordBank[PASS_LEN];			//��¼����
	TCHAR							szComputerID[COMPUTER_ID_LEN];		//��������
};

//��¼�ɹ�
struct DBR_GP_LogonSuccess
{
	//��������
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwGameID;							//��Ϸ��ʶ
	DWORD							dwExperience;						//������ֵ
	TCHAR							szPassword[PASS_LEN];				//��¼����
	TCHAR							szAccounts[NAME_LEN];				//��¼�ʺ�
	TCHAR							szGroupName[GROUP_LEN];				//��������
	BYTE							cbMoorMachine;						//�̶�����

	//�û�����
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	DWORD							dwCustomFaceVer;					//ͷ��汾
	BYTE							cbMember;							//��Ա�ȼ�
	TCHAR							szUnderWrite[UNDER_WRITE_LEN];		//����ǩ��

	//������Ϣ
	TCHAR							szDescribeString[128];				//������Ϣ
	TCHAR							szInsurePwd[PASS_LEN];				//�������� add by yijian
};

//��¼ʧ��
struct DBR_GP_LogonError
{
	LONG							lErrorCode;							//�������
	TCHAR							szErrorDescribe[128];				//������Ϣ
};

//�ϴ�ͷ��
struct DBR_GP_UploadCustomFace
{
	DWORD							dwUserID;							//��� ID
};

//�ϴ����
struct DBR_GP_UploadFaceResult
{
	TCHAR							szDescribeMsg[128];					//������Ϣ
	DWORD							dwFaceVer;							//ͷ��汾
	bool							bOperateSuccess;					//�ɹ���ʶ
};

//ɾ�����
struct DBR_GP_DeleteFaceResult
{
	TCHAR							szDescribeMsg[128];					//������Ϣ
	DWORD							dwFaceVer;							//ͷ��汾
	bool							bOperateSuccess;					//�ɹ���ʶ
};

//����ͷ��
struct DBR_GP_DownloadCustomFace
{
	DWORD							dwUserID;							//��� ID
};

//���سɹ�
struct DBR_GP_DownloadFaceSuccess
{
	DWORD							dwToltalSize;						//�ܹ���С
	DWORD							dwCurrentSize;						//��ǰ��С
	DWORD							dwUserID;							//��� ID
	BYTE							bFaceData[2048];					//ͷ������
};

//ɾ����Ϣ
struct DBR_GP_CustomFaceDelete
{
	DWORD							dwUserID;							//��� ID
	DWORD							dwFaceVer;							//ͷ��汾
};

//��������
struct DBR_GP_ModifyIndividual
{
	DWORD							dwUserID;							//��� ID
	TCHAR							szNickname[NAME_LEN];				//����ǳ�
	int								nGender;							//����Ա�
	int								nAge;								//�������
	TCHAR							szAddress[64];						//��ҵ�ַ
	TCHAR							szUnderWrite[32];					//����ǩ��
	TCHAR							szPassword[33];						//�������
};

//�޸Ľ��
struct DBR_GP_ModifyIndividualResult
{
	TCHAR							szDescribeMsg[128];					//������Ϣ
	bool							bOperateSuccess;					//�ɹ���ʶ
};

//////////////////////////////////////////////////////////////////////////

#endif