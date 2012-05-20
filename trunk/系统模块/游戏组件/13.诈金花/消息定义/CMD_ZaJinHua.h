#ifndef CMD_HK_FIVE_CARD_HEAD_FILE
#define CMD_HK_FIVE_CARD_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID							3									//��Ϸ I D
#define GAME_PLAYER						5									//��Ϸ����
#define GAME_NAME						TEXT("թ��")						//��Ϸ����
#define GAME_GENRE						(GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//��Ϸ����
#define MAX_COUNT						3									//�˿���Ŀ

//����ԭ��
#define GER_NO_PLAYER					0x10								//û�����
#define GER_COMPARECARD					0x20								//���ƽ���
#define GER_OPENCARD					0x30								//���ƽ���

//��Ϸ״̬
#define GS_T_FREE					GS_FREE									//�ȴ���ʼ
#define GS_T_SCORE					GS_PLAYING								//�з�״̬
#define GS_T_PLAYING				GS_PLAYING+1							//��Ϸ����

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START				100									//��Ϸ��ʼ
#define SUB_S_ADD_SCORE					101									//��ע���
#define SUB_S_GIVE_UP					102									//������ע
#define SUB_S_COMPARE_CARD				105									//���Ƹ�ע
#define SUB_S_LOOK_CARD					106									//���Ƹ�ע
#define SUB_S_SEND_CARD					103									//������Ϣ
#define SUB_S_GAME_END					104									//��Ϸ����
#define SUB_S_PLAYER_EXIT				107									//�û�ǿ��
#define SUB_S_OPEN_CARD					108									//������Ϣ
#define SUB_S_WAIT_COMPARE				109									//�ȴ�����

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONG								lCellScore;							//��������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��ע��Ϣ
	LONG								lMaxCellScore;						//��Ԫ����
	LONG								lCellScore;							//��Ԫ��ע
	LONG								lCurrentTimes;						//��ǰ����
	LONG								lUserMaxScore;						//�û���������

	//״̬��Ϣ
	WORD								wBankerUser;						//ׯ���û�
	WORD				 				wCurrentUser;						//��ǰ���
	BYTE								cbPlayStatus[GAME_PLAYER];			//��Ϸ״̬
	bool								bMingZhu[GAME_PLAYER];				//����״̬
	LONG								lTableScore[GAME_PLAYER];			//��ע��Ŀ

	//�˿���Ϣ
	BYTE								cbHandCardData[3];					//�˿�����

	//״̬��Ϣ
	bool								bCompareState;						//����״̬								
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	//��ע��Ϣ
	LONG								lMaxScore;							//�����ע
	LONG								lCellScore;							//��Ԫ��ע
	LONG								lCurrentTimes;						//��ǰ����
	LONG								lUserMaxScore;						//��������

	//�û���Ϣ
	WORD								wBankerUser;						//ׯ���û�
	WORD				 				wCurrentUser;						//��ǰ���
};

//�û���ע
struct CMD_S_AddScore
{
	WORD								wCurrentUser;						//��ǰ�û�
	WORD								wAddScoreUser;						//��ע�û�
	WORD								wCompareState;						//����״̬
	LONG								lAddScoreCount;						//��ע��Ŀ
	LONG								lCurrentTimes;						//��ǰ����
};

//�û�����
struct CMD_S_GiveUp
{
	WORD								wGiveUpUser;						//�����û�
};

//�������ݰ�
struct CMD_S_CompareCard
{
	WORD								wCurrentUser;						//��ǰ�û�
	WORD								wCompareUser[2];					//�����û�
	WORD								wLostUser;							//�����û�
};

//�������ݰ�
struct CMD_S_LookCard
{
	WORD								wLookCardUser;						//�����û�
	BYTE								cbCardData[MAX_COUNT];				//�û��˿�
};

//�������ݰ�
struct CMD_S_OpenCard
{
	WORD								wWinner;							//ʤ���û�
};

//��Ϸ����
struct CMD_S_GameEnd
{
	LONG								lGameTax;							//��Ϸ˰��
	LONG								lGameScore[GAME_PLAYER];			//��Ϸ�÷�
	BYTE								cbCardData[GAME_PLAYER][3];			//�û��˿�
	WORD								wCompareUser[GAME_PLAYER][4];		//�����û�
	WORD								wEndState;							//����״̬
};

//�û��˳�
struct CMD_S_PlayerExit
{
	WORD								wPlayerID;							//�˳��û�
};

//�ȴ�����
struct CMD_S_WaitCompare
{
	WORD								wCompareUser;						//�����û�
};

//////////////////////////////////////////////////////////////////////////

//�ͻ�������ṹ
#define SUB_C_ADD_SCORE					1									//�û���ע
#define SUB_C_GIVE_UP					2									//������Ϣ
#define SUB_C_COMPARE_CARD				3									//������Ϣ
#define SUB_C_LOOK_CARD					4									//������Ϣ
#define SUB_C_OPEN_CARD					5									//������Ϣ
#define SUB_C_WAIT_COMPARE				6									//�ȴ�����
#define SUB_C_FINISH_FLASH				7									//��ɶ���

//�û���ע
struct CMD_C_AddScore
{
	LONG								lScore;								//��ע��Ŀ
	WORD								wState;								//��ǰ״̬
};

//�������ݰ�
struct CMD_C_CompareCard
{	
	WORD								wCompareUser;						//�����û�
};

//////////////////////////////////////////////////////////////////////////

#endif