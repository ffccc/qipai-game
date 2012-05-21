#ifndef CMD_THREEKING_HEAD_FILE
#define CMD_THREEKING_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID							9									//��Ϸ I D
#define GAME_PLAYER						6									//��Ϸ����
#define GAME_NAME						TEXT("������Ϸ")					//��Ϸ����

//��Ϸ״̬
#define GS_TK_FREE						GS_FREE								//����״̬
#define GS_TK_PLAYING					GS_PLAYING							//��ʼ״̬

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_TIMER_INFO				100									//ʱ����Ϣ
#define SUB_S_SCORE_INFO				101									//��ע��Ϣ
#define SUB_S_BANKER_USER				102									//ׯ���û�
#define SUB_S_CHANGE_INFO				103									//��ׯ��Ϣ

#define SUB_S_GAME_START				200									//��Ϸ��ʼ
#define SUB_S_USER_CHIPIN				201									//�û���ע
#define SUB_S_SEND_CARD					202									//������Ϣ
#define SUB_S_GAME_END					203									//��Ϸ����

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONG								lMaxScore;							//�����ע
	LONG								lLessScore;							//������ע
	WORD								wLeaveTime;							//ʣ��ʱ��
	WORD								wBankerUser;						//ׯ���û�

	BOOL								bLongBanker;						//��ׯ��־
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��ע��Ϣ
	LONG								lMaxScore;							//�����ע
	LONG								lLessScore;							//������ע
	LONG								lUserChipIn[GAME_PLAYER];			//��ע��Ϣ

	//��Ϸ��Ϣ
	WORD								wBankerUser;						//ׯ���û�
	BYTE								cbPlayStatus[GAME_PLAYER];			//��Ϸ״̬

	//�˿���Ϣ
	BYTE								cbSendCard;							//���Ʊ�־
	BYTE								cbNeedFinish;						//�������
	BYTE								cbUserCardData[GAME_PLAYER][3];		//�û��˿�

	BOOL								bLongBanker;						//��ׯ��־
};

//ʱ����Ϣ
struct CMD_S_TimerInfo
{
	WORD								wLeaveTime;							//ʣ��ʱ��
};

//��ע��Ϣ
struct CMD_S_ScoreInfo
{
	LONG								lMaxScore;							//�����ע
	LONG								lLessScore;							//������ע
};

//ׯ����Ϣ
struct CMD_S_BankerUser
{
	BOOL								bLongBanker;						//��ׯ��־
	LONG								lMaxScore;							//�����ע
	LONG								lLessScore;							//������ע
	WORD								wBankerUser;						//ׯ���û�
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	LONG								lMaxScore;							//�����ע
	LONG								lLessScore;							//������ע
	BYTE								cbPlayStatus[GAME_PLAYER];			//�û�״̬
};

//��ע��Ϣ
struct CMD_S_UserChipin
{
	WORD								wChairID;							//�û�λ��
	LONG								lUserChipin;						//��ע��Ŀ
};

//��Ϸ����
struct CMD_S_SendCard
{
	BYTE								cbUserCardData[GAME_PLAYER][3];		//�û��˿�
};

//��Ϸ����
struct CMD_S_GameEnd
{
	LONG								lGameTax[GAME_PLAYER];				//��Ϸ˰��
	LONG								lGameScore[GAME_PLAYER];			//��Ϸ�÷�
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_USER_CHIPIN				1									//�û���ע
#define SUB_C_SEND_FINISH				2									//�������
#define SUB_C_CHANGE_INFO				3									//��ׯ��Ϣ
#define SUB_C_CONTINUE_GAME				4									//������Ϸ
#define SUB_C_CONTROL_MAX				5

//��ׯ��Ϣ
struct CMD_C_ChangeInfo
{
	BYTE								cbChange;							//��ׯ��־
};

//��ע��Ϣ
struct CMD_C_UserChipin
{
	LONG								lScore;								//��ע��Ŀ
};

//////////////////////////////////////////////////////////////////////////

#endif
