#ifndef OPTION_PARAMETER_HEAD_FILE
#define OPTION_PARAMETER_HEAD_FILE

#pragma once

#include "AfxTempl.h"
#include "GlobalServer.h"
#include "PlatformDataHead.h"

//////////////////////////////////////////////////////////////////////////////////
//ע�����

//ϵͳ��Ϣ
#define REG_USER_INFO				TEXT("UserInfo")					//�û���Ϣ
#define REG_GAME_OPTION				TEXT("ServerOption")				//��Ϸ��Ϣ

//������Ϣ
#define REG_GOBAL_OPTION			TEXT("Option\\Golbal")				//ȫ����Ϣ
#define REG_INTERFACE_INFO			TEXT("Option\\Interface")			//������Ϣ
#define REG_PROXY_SERVER_INFO		TEXT("Option\\ProxyServer")			//������Ϣ

//////////////////////////////////////////////////////////////////////////////////
//ö�ٶ���

//����ģʽ
#define INVITE_MODE_ALL				0									//ȫ����ʾ
#define INVITE_MODE_NONE			1									//ȫ����ʾ
#define INVITE_MODE_FRIEND			2									//������ʾ

//��Ϣģʽ
#define MESSAGE_MODE_ALL			0									//ȫ����ʾ
#define MESSAGE_MODE_DETEST			1									//�������
#define MESSAGE_MODE_FRIEND			2									//���ź���

//���涯��
#define ACTION_ORIENTATION			0									//��λ�û�
#define ACTION_SEND_WHISPER			1									//����˽��
#define ACTION_SHOW_USER_INFO		2									//�û���Ϣ
#define ACTION_SEARCH_TABLE			3									//Ѱ��λ��
#define ACTION_SHOW_SEARCH_DLG		4									//���ҽ���

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ����
class PLATFORM_DATA_CLASS CParameterGame
{
	//��Ԫ����
	friend class CParameterGlobal;

	//ʤ������
public:
	WORD							m_wMinWinRate;						//���ʤ��
	bool							m_bLimitWinRate;					//����ʤ��

	//��������
public:
	WORD							m_wMaxFleeRate;						//�������
	bool							m_bLimitFleeRate;					//���ƶ���

	//��������
public:
	LONG							m_lMaxGameScore;					//��߷��� 
	LONG							m_lMinGameScore;					//��ͷ���
	bool							m_bLimitGameScore;					//���Ʒ���

	//���Ա���
protected:
	TCHAR							m_szRegKeyName[16];					//ע������

	//��������
public:
	//���캯��
	CParameterGame();
	//��������
	virtual ~CParameterGame();

	//���ܺ���
public:
	//���ز���
	VOID LoadParameter();
	//�������
	VOID SaveParameter();
	//Ĭ�ϲ���
	VOID DefaultParameter();

	//���ú���
protected:
	//���ò���
	bool InitParameter(LPCTSTR pszProcessName);
};

//////////////////////////////////////////////////////////////////////////////////

//�������
class PLATFORM_DATA_CLASS CParameterServer
{
	//���ñ���
public:
	bool							m_bTakePassword;					//�����־
	TCHAR							m_szPassword[LEN_PASSWORD];			//��������

	//��������
public:
	//���캯��
	CParameterServer();
	//��������
	virtual ~CParameterServer();

	//���ú���
public:
	//Ĭ�ϲ���
	VOID DefaultParameter();
};

//////////////////////////////////////////////////////////////////////////////////

//���鶨��
typedef CMap<WORD,WORD,CParameterGame *,CParameterGame *> CParameterGameMap;
typedef CMap<WORD,WORD,CParameterServer *,CParameterServer *> CParameterServerMap;

//ȫ�ֲ���
class PLATFORM_DATA_CLASS CParameterGlobal
{
	//��¼����
public:
	bool							m_bLogonAuto;						//�Զ���¼
	bool							m_bWriteCookie;						//д�����
	bool							m_bRemberPassword;					//��ס����

	//ģʽ����
public:
	BYTE							m_cbInviteMode;						//����ģʽ
	BYTE							m_cbMessageMode;					//��Ϣģʽ
	BYTE							m_cbActionHitAutoJoin;				//�Զ�����
	BYTE							m_cbActionLeftDoubleList;			//˫���б�

	//ʱ������
public:
	WORD							m_wIntermitTime;					//�ж�ʱ��
	WORD							m_wOnLineCountTime;					//����ʱ��

	//��������
public:
	bool							m_bLimitDetest;						//�������
	bool							m_bLimitSameIP;						//���Ƶ�ַ
	bool							m_bNotifyUserInOut;					//������Ϣ
	bool							m_bNotifyFriendCome;				//������ʾ

	//��������
public:
	bool							m_bSalienceTable;					//ͻ������
	bool							m_bFullScreenBrowse;				//ȫ�����

	//ϵͳ����
public:
	bool							m_bAllowSound;						//��������
	bool							m_bAutoSitDown;						//�Զ�����
	bool							m_bAutoShowWhisper;					//��ʾ˽��
	bool							m_bSaveWhisperChat;					//����˽��
	bool							m_bSendWhisperByEnter;				//�س�����

	//�ȼ�����
public:
	WORD							m_wBossHotKey;						//�ϰ��ȼ�
	WORD							m_wWhisperHotKey;					//˽���ȼ�

	//��ɫ����
public:
	COLORREF						m_crChatTX;							//������ɫ
	COLORREF						m_crWhisperTX;						//˽����ɫ
	tagSkinRenderInfo				m_SkinRenderInfo;					//��Ⱦ��ɫ

	//��������
public:
	BYTE							m_cbProxyType;						//��������
	tagProxyServer					m_ProxyServer;						//������Ϣ

	//�������
protected:
	CParameterGameMap				m_ParameterGameMap;					//��Ϸ����
	CParameterServerMap				m_ParameterServerMap;				//��������

	//��̬����
protected:
	static CParameterGlobal *		m_pParameterGlobal;					//ȫ������

	//��������
public:
	//���캯��
	CParameterGlobal();
	//��������
	virtual ~CParameterGlobal();

	//���ú���
public:
	//���ز���
	VOID LoadParameter();
	//�������
	VOID SaveParameter();
	//Ĭ�ϲ���
	VOID DefaultParameter();

	//��Ϸ����
public:
	//��Ϸ����
	CParameterGame * GetParameterGame(tagGameKind * pGameKind);
	//��������
	CParameterServer * GetParameterServer(tagGameServer * pGameServer);

	//��̬����
public:
	//��ȡ����
	static CParameterGlobal * GetInstance() { return m_pParameterGlobal; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif