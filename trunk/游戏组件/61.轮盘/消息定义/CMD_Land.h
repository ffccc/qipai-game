#ifndef CMD_LAND_HEAD_FILE
#define CMD_LAND_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						404									//��Ϸ I D
#define GAME_PLAYER					100									//��Ϸ����
#define GAME_NAME					TEXT("����")						//��Ϸ����

#define	HISTORY_NUMBER				13									//��ʷ��¼����
#define	MAX_BET_POS					256									//�����עλ��
//��Ϸ״̬
#define GS_WK_FREE					GS_FREE								//�ȴ���ʼ


//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_USER_CHIP				101									//�û���ע
#define SUB_S_STOP_ROLL				102									//ֹͣ��ת
#define SUB_S_GAME_END				105									//��Ϸ����

//����״̬
struct CMD_S_StatusFree
{
	LONG							lCellScore;		//��������
	LONG							lUserScore;		//�û�����
	BYTE							cbTimeLeave;	//ʣ��ʱ��
	BYTE							bHistoryInfo[HISTORY_NUMBER];		//��ʷ��Ϣ
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	BYTE							bWinNumber;	//ʤ������
};

//�û���ע
struct CMD_S_UserChipIn
{
	LONG							lAllChips[256];	//������ע
};

//ʤ������
struct CMD_S_StopRoll
{
	BYTE							bWinNumber;	//ʤ������
};

//��Ϸ����
struct CMD_S_GameEnd
{
	BYTE							bWinNumber;						//ʤ������
	LONG							lGameScore[GAME_PLAYER];		//ʤ������
	LONG							lMaxBetWinScore[GAME_PLAYER];		//�����ע
	LONG							lMaxUserScore[GAME_PLAYER];		//�û�����
	BYTE							bHistoryInfo[HISTORY_NUMBER];	//��ʷ��Ϣ
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_CHIP_IN			10									//�û��з�
#define SUB_C_ROLL_END			20									//ʤ������
#define SUB_C_GET_STOP			30									//�õ�ֹͣ��Ϣ

//�û��з�
struct CMD_C_ChipIn
{
	int							ChipsInfo[256];							//��������
};

//��ת����
struct CMD_C_RollEnd
{
	BYTE						WinNumber;	
};


//////////////////////////////////////////////////////////////////////////

#endif