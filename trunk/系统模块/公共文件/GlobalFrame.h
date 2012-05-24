#ifndef GLOBAL_FRAME_HEAD_FILE
#define GLOBAL_FRAME_HEAD_FILE

#pragma once

#include "GlobalDef.h"
#include "GlobalProperty.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��Ϸ״̬
#define GS_FREE							0								//����״̬
#define GS_PLAYING						100								//��Ϸ״̬

//////////////////////////////////////////////////////////////////////////
//IPC �����¼�

#define IPC_MAIN_SOCKET					1								//������Ϣ

#define IPC_SUB_SOCKET_SEND				1								//���緢��
#define IPC_SUB_SOCKET_RECV				2								//�������

//IPC ������ṹ
struct IPC_SocketPackage
{
	CMD_Command							Command;						//������Ϣ
	BYTE								cbBuffer[SOCKET_PACKET];		//���ݻ���
};

//////////////////////////////////////////////////////////////////////////
//IPC ������Ϣ

#define IPC_MAIN_CONFIG					2								//������Ϣ

#define IPC_SUB_SERVER_INFO				1								//������Ϣ
#define IPC_SUB_COLUMN_INFO				2								//�б���Ϣ

//��Ϸ��Ϣ
struct IPC_GF_ServerInfo
{
	DWORD								dwUserID;						//�û� I D
	WORD								wTableID;						//���Ӻ���
	WORD								wChairID;						//���Ӻ���
	WORD								wKindID;						//���ͱ�ʶ
	WORD								wServerID;						//�����ʶ
	WORD								wGameGenre;						//��Ϸ����
	WORD								wChairCount;					//������Ŀ
	BYTE								cbHideUserInfo;					//������Ϣ
	DWORD								dwVideoAddr;					//��Ƶ��ַ
	TCHAR								szKindName[KIND_LEN];			//��������
	TCHAR								szServerName[SERVER_LEN];		//��������
};

//////////////////////////////////////////////////////////////////////////
//IPC �û���Ϣ

#define IPC_MAIN_USER					3								//�û���Ϣ

#define IPC_SUB_USER_COME				1								//�û���Ϣ
#define IPC_SUB_USER_STATUS				2								//�û�״̬
#define IPC_SUB_USER_SCORE				3								//�û�����
#define IPC_SUB_GAME_START				4								//��Ϸ��ʼ
#define IPC_SUB_GAME_FINISH				5								//��Ϸ����
#define IPC_SUB_UPDATE_FACE				6								//����ͷ��
#define IPC_SUB_MEMBERORDER				7								//����ͷ��

//�û�״̬
struct IPC_UserStatus
{
	DWORD								dwUserID;						//�û� I D
	WORD								wNetDelay;						//������ʱ
	BYTE								cbUserStatus;					//�û�״̬
};

//�û�����
struct IPC_UserScore
{
	LONG								lLoveliness;					//�û�����
	DWORD								dwUserID;						//�û� I D
	tagUserScore						UserScore;						//�û�����
};

//��Ա�ȼ�
struct IPC_MemberOrder
{
	BYTE								cbMember;					//��Ա�ȼ�
	DWORD								dwUserID;						//�û� I D
};

//�û�����
struct IPC_UpdateFace
{
	DWORD								dwCustomFace;					//�û� I D
};

//////////////////////////////////////////////////////////////////////////
//IPC ������Ϣ

#define IPC_MAIN_CONCTROL				4								//������Ϣ

#define IPC_SUB_START_FINISH			1								//�������
#define IPC_SUB_CLOSE_FRAME				2								//�رտ��
#define IPC_SUB_JOIN_IN_GAME			3								//������Ϸ

//������Ϸ
struct IPC_JoinInGame
{
	WORD								wTableID;						//���Ӻ���
	WORD								wChairID;						//���Ӻ���
};

//////////////////////////////////////////////////////////////////////////
//����������

#define MDM_GF_GAME						100								//��Ϸ��Ϣ
#define MDM_GF_FRAME					101								//�����Ϣ
#define	MDM_GF_PRESENT					102								//������Ϣ
#define	MDM_GF_BANK						103								//������Ϣ

#define SUB_GF_INFO						1								//��Ϸ��Ϣ
#define SUB_GF_USER_READY				2								//�û�ͬ��
#define SUB_GF_LOOKON_CONTROL			3								//�Թۿ���
#define SUB_GF_KICK_TABLE_USER			4								//�����û�

#define SUB_GF_OPTION					100								//��Ϸ����
#define SUB_GF_SCENE					101								//������Ϣ

#define SUB_GF_USER_CHAT				200								//�û�����

#define SUB_GF_MESSAGE					300								//ϵͳ��Ϣ

//#define SUB_GF_GIFT					400								//������Ϣ

#define SUB_GF_BANK_STORAGE				450								//���д洢
#define SUB_GF_BANK_GET					451								//������ȡ
#define SUB_GF_CHANGE_PASSWORD			452								//������ȡ
#define SUB_GF_TRANSFER					453								//������ȡ

#define SUB_GF_FLOWER_ATTRIBUTE			500								//�ʻ�����
#define SUB_GF_FLOWER					501								//�ʻ���Ϣ
#define SUB_GF_EXCHANGE_CHARM			502								//�һ�����

#define SUB_GF_PROPERTY					550								//������Ϣ
#define SUB_GF_PROPERTY_RESULT			551								//���߽��
#define SUB_GF_RESIDUAL_PROPERTY		552								//ʣ�����
#define SUB_GF_PROP_ATTRIBUTE			553								//��������
#define SUB_GF_PROP_BUGLE				554								//���ȵ���

//�汾��Ϣ
struct CMD_GF_Info
{
	BYTE								bAllowLookon;					//�Թ۱�־
};

//��Ϸ����
struct CMD_GF_Option
{
	BYTE								bGameStatus;					//��Ϸ״̬
	BYTE								bAllowLookon;					//�����Թ�
};

//�Թۿ���
struct CMD_GF_LookonControl
{
	DWORD								dwUserID;						//�û���ʶ
	BYTE								bAllowLookon;					//�����Թ�
};

//�����û�
struct CMD_GF_KickTableUser
{
	DWORD								dwUserID;						//�û� I D
};

//����ṹ
struct CMD_GF_UserChat
{
	WORD								wChatLength;					//��Ϣ����
	COLORREF							crFontColor;					//��Ϣ��ɫ
	DWORD								dwSendUserID;					//�����û�
	DWORD								dwTargetUserID;					//Ŀ���û�
	TCHAR								szChatMessage[MAX_CHAT_LEN];	//������Ϣ
};
//////////////////////////////////////////////////////////////////////////

//��Ϣ����
#define SMT_INFO						0x0001							//��Ϣ��Ϣ
#define SMT_EJECT						0x0002							//������Ϣ
#define SMT_GLOBAL						0x0004							//ȫ����Ϣ
#define SMT_CLOSE_GAME					0x1000							//�ر���Ϸ

//��Ϣ���ݰ�
struct CMD_GF_Message
{
	WORD								wMessageType;					//��Ϣ����
	WORD								wMessageLength;					//��Ϣ����
	TCHAR								szContent[1024];				//��Ϣ����
};

//////////////////////////////////////////////////////////////////////////

//�����ڴ涨��
struct tagShareMemory
{
	WORD								wDataSize;						//���ݴ�С
	HWND								hWndGameFrame;					//��ܾ��
	HWND								hWndGamePlaza;					//�㳡���
	HWND								hWndGameServer;					//������
};

//////////////////////////////////////////////////////////////////////////

//���ͳ���
#define	LOCATION_GAME_ROOM				1								//��Ϸ����
#define	LOCATION_PLAZA_ROOM				2								//��������

//////////////////////////////////////////////////////////////////////////

//���ͽṹ
struct CMD_GF_Gift
{
	BYTE								cbSendLocation;					//���ͳ���
	DWORD								dwSendUserID;					//������ID
	DWORD								dwRcvUserID;					//������ID
	WORD								wGiftID;						//����	ID
	WORD								wFlowerCount;					//�ʻ���Ŀ
};

//���߽ṹ
struct CMD_GF_Property
{
	BYTE								cbSendLocation;					//���ͳ���
	int									nPropertyID;					//����ID
	DWORD								dwPachurseCount;				//������Ŀ
	DWORD								dwOnceCount;					//������Ŀ
	DWORD								dwSourceUserID;					//�������
	DWORD								dwTargetUserID;					//Ŀ�����
	TCHAR								szRcvUserName[32];				//�û�����
};

//���Ƚṹ
struct CMD_GF_BugleProperty
{
	BYTE								cbSendLocation;					//���ͳ���
	TCHAR								szUserName[32];					//����ʺ�
	COLORREF							crText;							//������ɫ
	TCHAR								szContext[BUGLE_MAX_CHAR];		//��������
};

//�һ��ṹ
struct CMD_GF_ExchangeCharm
{
	BYTE								cbSendLocation;					//���ͳ���
	LONG								lLoveliness;					//������ֵ
	DWORD								lGoldValue;						//�����ֵ
};

//����֪ͨ
struct CMD_GF_GiftNotify
{
	BYTE								cbSendLocation;					//���ͳ���
	DWORD								dwSendUserID;					//������ID
	DWORD								dwRcvUserID;					//������ID
	WORD								wGiftID;						//����	ID
	WORD								wFlowerCount;					//�ʻ���Ŀ
};

//�洢���
struct CMD_GF_BankStorage
{
	LONG								lStorageValue;					//�洢���
	BYTE								cbGameAction;					//��Ϸ����
	TCHAR								szPassword[PASS_LEN];			// add by peter
};

//��ȡ���
struct CMD_GF_BankGet
{
	LONG								lGetValue;						//��ȡ���
	TCHAR								szPassword[PASS_LEN];			//�û�����
	BYTE								cbGameAction;					//��Ϸ����
};

//��ȡ���
struct CMD_GF_ChangePassword
{
	TCHAR								szOriginPassword[PASS_LEN];			//�û�����
	TCHAR								szNewPassword[PASS_LEN];			//�û�����
};

//��ȡ���
struct CMD_GF_Transfer
{
	LONGLONG							lInputCount;					//��ȡ���
	DWORD								dwUserID;
	TCHAR								szPassword[PASS_LEN];			//�û�����
	TCHAR								szNickname[NAME_LEN];			//�û�����
};

// add by yijian
struct DBR_GR_Transfer
{
	DWORD	dwClientIP;
	DWORD	dwSourceUserID;
	DWORD	dwTargetUserID;
	long	lRevenue;
	long	lTransferCount;
};

//ʣ�����
struct CMD_GF_ResidualProperty
{
	DWORD								dwResidualTime[PROPERTY_COUNT];	//ʣ��ʱ��
};



//////////////////////////////////////////////////////////////////////////
#endif