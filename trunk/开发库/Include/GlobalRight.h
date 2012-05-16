#ifndef GLOBAL_RIGHT_HEAD_FILE
#define GLOBAL_RIGHT_HEAD_FILE

//////////////////////////////////////////////////////////////////////////

//���Ȩ�޶���
#define UR_CANNOT_PLAY					0x00000001L				//���ܽ�����Ϸ
#define UR_CANNOT_LOOKON				0x00000002L				//�����Թ���Ϸ
#define UR_CANNOT_WISPER				0x00000004L				//���ܷ���˽��
#define UR_CANNOT_ROOM_CHAT				0x00000008L				//���ܴ�������
#define UR_CANNOT_GAME_CHAT				0x00000010L				//������Ϸ����

//��־Ȩ�޶���
#define UR_MATCH_USER					0x10000000L				//�����û���־

//����Ȩ�޶���
#define UR_CAN_LIMIT_PLAY				0x00000001L				//�����ֹ��Ϸ
#define UR_CAN_LIMIT_LOOKON				0x00000002L				//�����ֹ�Թ�
#define UR_CAN_LIMIT_WISPER				0x00000004L				//�����ֹ˽��
#define UR_CAN_LIMIT_ROOM_CHAT			0x00000008L				//�����ֹ����
#define UR_CAN_LIMIT_GAME_CHAT			0x00000010L				//�����ֹ����
#define UR_CAN_CUT_USER					0x00000020L				//�����߳��û�
#define UR_CAN_FORBID_ACCOUNTS			0x00000040L				//��������ʺ�
#define UR_CAN_CONFINE_IP				0x00000080L				//�����ֹ��ַ
#define UR_CAN_SEE_USER_IP				0x00000100L				//����鿴��ַ
#define UR_CAN_SEND_WARNING				0x00000200L				//�����;���
#define UR_CAN_ISSUE_MESSAGE			0x00000400L				//��������Ϣ
#define UR_CAN_BIND_GAME				0x00000800L				//������Ϸ��
#define UR_CAN_BIND_GLOBAL				0x00001000L				//����ȫ�ְ�
#define UR_CAN_SERVER_OPTION			0x00002000L				//�������÷���

//////////////////////////////////////////////////////////////////////////

//Ȩ�޸�����
class CUserRight
{
	//���Ȩ��
public:
	//��ϷȨ��
	static bool CanPlay(LONG lUserRight) { return (lUserRight&UR_CANNOT_PLAY)==0; }
	//�Թ�Ȩ��
	static bool CanLookon(LONG lUserRight) { return (lUserRight&UR_CANNOT_LOOKON)==0; }
	//˽��Ȩ��
	static bool CanWisper(LONG lUserRight) { return (lUserRight&UR_CANNOT_WISPER)==0; }
	//��������
	static bool CanRoomChat(LONG lUserRight) { return (lUserRight&UR_CANNOT_ROOM_CHAT)==0; }
	//��Ϸ����
	static bool CanGameChat(LONG lUserRight) { return (lUserRight&UR_CANNOT_GAME_CHAT)==0; }

	//�û���ʶ
public:
	//������ʶ
	static bool IsMatchUser(LONG lUserRight) { return (lUserRight&UR_MATCH_USER)!=0; }

	//����Ȩ��
public:
	//��ֹ��Ϸ
	static bool CanLimitPlay(LONG lMasterRight) { return ((lMasterRight&UR_CAN_LIMIT_PLAY)!=0); }
	//��ֹ�Թ�
	static bool CanLimitLookon(LONG lMasterRight) { return ((lMasterRight&UR_CAN_LIMIT_LOOKON)!=0); }
	//��ֹ˽��
	static bool CanLimitWisper(LONG lMasterRight) { return ((lMasterRight&UR_CAN_LIMIT_WISPER)!=0); }
	//��ֹ����
	static bool CanLimitRoomChat(LONG lMasterRight) { return ((lMasterRight&UR_CAN_LIMIT_ROOM_CHAT)!=0); }
	//��ֹ����
	static bool CanLimitGameChat(LONG lMasterRight) { return ((lMasterRight&UR_CAN_LIMIT_GAME_CHAT)!=0); }
	//�߳��û�
	static bool CanCutUser(LONG lMasterRight) { return ((lMasterRight&UR_CAN_CUT_USER)!=0); }
	//�����ʺ�
	static bool CanForbidAccounts(LONG lMasterRight) { return ((lMasterRight&UR_CAN_FORBID_ACCOUNTS)!=0); }
	//��ֹ��ַ
	static bool CanConfineIP(LONG lMasterRight) { return ((lMasterRight&UR_CAN_CONFINE_IP)!=0); }
	//�鿴��ַ
	static bool CanSeeUserIP(LONG lMasterRight) { return ((lMasterRight&UR_CAN_SEE_USER_IP)!=0); }
	//������Ϣ
	static bool CanIssueMessage(LONG lMasterRight) { return ((lMasterRight&UR_CAN_ISSUE_MESSAGE)!=0); }
	//���;���
	static bool CanSendWarning(LONG lMasterRight) { return ((lMasterRight&UR_CAN_SEND_WARNING)!=0); }
	//��Ϸ��
	static bool CanBindGame(LONG lMasterRight) { return ((lMasterRight&UR_CAN_BIND_GAME)!=0); }
	//ȫ�ְ�
	static bool CanBindGlobal(LONG lMasterRight) { return ((lMasterRight&UR_CAN_BIND_GLOBAL)!=0); }
	//���÷���
	static bool CanServerOption(LONG lMasterRight) { return ((lMasterRight&UR_CAN_SERVER_OPTION)!=0); }
};

//////////////////////////////////////////////////////////////////////////

#endif
