#ifndef CMD_SHOWHAND_HEAD_FILE
#define CMD_SHOWHAND_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID							201									//��Ϸ��ʶ
#define GAME_PLAYER						4									//��Ϸ����
#define GAME_NAME						TEXT("�����Ϸ")					//��Ϸ����

//////////////////////////////////////////////////////////////////////////
//�����붨��

#define SUB_C_ADD_GOLD					1									//�û���ע
#define SUB_C_GIVE_UP					2									//������ע

#define SUB_S_GAME_START				100									//��Ϸ��ʼ
#define SUB_S_ADD_GOLD					101									//��ע���
#define SUB_S_GIVE_UP					102									//������ע
#define SUB_S_SEND_CARD					103									//������Ϣ
#define SUB_S_GAME_END					104									//��Ϸ����

//////////////////////////////////////////////////////////////////////////
//��Ϣ�ṹ��

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONG								dwBasicGold;						//�������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	WORD				 				wCurrentUser;						//��ǰ���
	BYTE								bPlayStatus[GAME_PLAYER];			//��Ϸ״̬
	BYTE								bTableCardCount[GAME_PLAYER];		//�˿���Ŀ
	BYTE								bTableCardArray[GAME_PLAYER][5];	//�˿�����
	LONG								lMaxGold;							//�����ע
	LONG								lBasicGold;							//�������
	LONG								lTurnMaxGold;						//�����ע
	LONG								lTurnBasicGold;						//������ע
	LONG								lTableGold[2*GAME_PLAYER];			//������
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	BYTE								bFundusCard;						//�����˿�
	WORD				 				wCurrentUser;						//��ǰ���
	LONG								lMaxGold;							//�����ע
	LONG								lTurnMaxGold;						//�����ע
	LONG								lTurnBasicGold;						//������ע
	LONG								lBasicGold;							//��Ԫ��ע
	BYTE								bCardData[GAME_PLAYER];				//�û��˿�
};

//�û�����
struct CMD_S_GiveUp
{
	WORD								wUserChairID;						//�����û�
};

//�û���ע
struct CMD_C_AddGold
{
	LONG								lGold;								//��ע��Ŀ
};

//��ע���
struct CMD_S_AddGold
{
	WORD								wLastChairID;						//��һ�û�
	WORD								wCurrentUser;						//��ǰ�û�
	LONG								lLastAddGold;						//��ע��Ŀ
	LONG								lCurrentLessGold;					//���ټ�ע
};

//�������ݰ�
struct CMD_R_SendCard
{
	LONG								lMaxGold;							//�����ע
	WORD								wCurrentUser;						//��ǰ�û�
	WORD								wStartChairId;						//
	BYTE								cbSendCardCount;					//
	BYTE								bUserCard[GAME_PLAYER][2];			//�û��˿�
};

//��Ϸ����
struct CMD_S_GameEnd
{
	LONG								lTax;								//��Ϸ˰��
	LONG								lGameGold[4];						//��Ϸ�÷�
	BYTE								bUserCard[4];						//�û��˿�
};

//////////////////////////////////////////////////////////////////////////

#endif