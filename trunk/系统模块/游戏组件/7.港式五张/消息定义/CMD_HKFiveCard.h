#ifndef CMD_HK_FIVE_CARD_HEAD_FILE
#define CMD_HK_FIVE_CARD_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��


#define KIND_ID						2										//����Ƶ��Ϸ I D

#define GAME_PLAYER						5									//��Ϸ����
#define GAME_NAME						TEXT("��ʽ����")					//��Ϸ����
#define GAME_GENRE						(GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//��Ϸ����

//����ԭ��
#define GER_NO_PLAYER					0x10								//û�����

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START				100									//��Ϸ��ʼ
#define SUB_S_ADD_SCORE					101									//��ע���
#define SUB_S_GIVE_UP					102									//������ע
#define SUB_S_SEND_CARD					103									//������Ϣ
#define SUB_S_GAME_END					104									//��Ϸ����

#define SUB_S_SCORE_OPTION				500									//������Ϣ

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONG								lCellScore;							//��������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��־����
	BYTE								bShowHand;							//�����־
	BYTE								bAddScore;							//��ע��־

	//��ע��Ϣ
	LONG								lMaxScore;							//�����ע
	LONG								lCellScore;							//��Ԫ��ע
	LONG								lTurnMaxScore;						//�����ע
	LONG								lTurnLessScore;						//��С��ע

	//״̬��Ϣ
	WORD				 				wCurrentUser;						//��ǰ���
	BYTE								cbPlayStatus[GAME_PLAYER];			//��Ϸ״̬
	LONG								lTableScore[GAME_PLAYER*2];			//��ע��Ŀ

	//�˿���Ϣ
	BYTE								cbCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE								cbHandCardData[GAME_PLAYER][5];		//�����˿�
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	//��ע��Ϣ
	LONG								lMaxScore;							//�����ע
	LONG								lCellScore;							//��Ԫ��ע
	LONG								lTurnMaxScore;						//�����ע
	LONG								lTurnLessScore;						//��С��ע

	//�û���Ϣ
	WORD				 				wCurrentUser;						//��ǰ���

	//�˿�����
	BYTE								cbObscureCard;						//�����˿�
	BYTE								cbCardData[GAME_PLAYER];			//�û��˿�
};

//�û���ע
struct CMD_S_AddScore
{
	WORD								wCurrentUser;						//��ǰ�û�
	WORD								wAddScoreUser;						//��ע�û�
	LONG								lAddScoreCount;						//��ע��Ŀ
	LONG								lTurnLessScore;						//���ټ�ע
};

//�û�����
struct CMD_S_GiveUp
{
	WORD								wGiveUpUser;						//�����û�
};

//�������ݰ�
struct CMD_S_SendCard
{
	WORD								wCurrentUser;						//��ǰ�û�
	WORD								wLastMostUser;						//�ϴ�����û�
	LONG								lTurnMaxScore;						//�����ע
	LONG								lTurnLessScore;						//��С��ע
	BYTE								cbSendCardCount;					//������Ŀ
	BYTE								cbCardData[GAME_PLAYER][2];			//�û��˿�
};

//��Ϸ����
struct CMD_S_GameEnd
{
	LONG								lGameTax[GAME_PLAYER];				//��Ϸ˰��
	LONG								lGameScore[GAME_PLAYER];			//��Ϸ�÷�
	BYTE								cbCardData[GAME_PLAYER];			//�û��˿�
};

//��������
struct CMD_S_ScoreOption
{
	//״̬��Ϣ
	LONG								lUserCount;							//�û���Ŀ
	LONG								lBlackCount;						//�û���Ŀ

	//������Ϣ
	__int64								lMaxScore;							//���仯
	__int64								lMaxWinScore;						//���Ӯ��
	__int64								lMaxLoseScore;						//������
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_ADD_SCORE					1									//�û���ע
#define SUB_C_GIVE_UP					2									//������ע
#define SUB_C_GET_WINNER				500									//��ȡʤ��

#define SUB_C_GET_OPTION				650									//��ȡ����
#define SUB_C_SET_MAX_SCORE				651									//���û���
#define SUB_C_SET_MAX_WIN_SCORE			652									//���û���
#define SUB_C_SET_MAX_LOSE_SCORE		653									//���û���

#define SUB_C_SET_BLACKLIST				700									//�����û�
#define SUB_C_REMOVE_BLACKLIST			701									//ɾ���û�

//�û���ע
struct CMD_C_AddScore
{
	LONG								lScore;								//��ע��Ŀ
};

//��ȡʤ��
struct CMD_C_GetWinner
{
	DWORD								dwUserID;							//�û���ʶ
};

//������Ϣ
struct CMD_C_ScoreInfo
{
	__int64								lScore;								//������Ϣ
};

//�û���Ϣ
struct CMD_C_UserIDInfo
{
	DWORD								dwUserID;							//�û���ʶ
};

//////////////////////////////////////////////////////////////////////////

#endif