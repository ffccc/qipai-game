#ifndef CMD_EATINGALONE_HEAD_FILE
#define CMD_EATINGALONE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						601								//��Ϸ I D
#define GAME_PLAYER					5									//��Ϸ����
#define GAME_NAME					TEXT("������Ϸ")					//��Ϸ����
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_GOLD)	//��Ϸ����

//��Ϸ״̬
#define GS_RF_FREE					GS_FREE								//�ȴ���ʼ
#define GS_RF_BANKER				GS_PLAYING+1						//ѡׯ״̬
#define GS_RF_PLAYING				GS_PLAYING							//��Ϸ״̬


//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START			101									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				102									//�û�����
#define SUB_S_PASS_CARD				103									//��������
#define SUB_S_GAME_END				104									//��Ϸ����
#define SUB_S_DISPLAYTEN            105                                 //�û�����
#define SUB_S_JIU                   106                                 //�û�����
#define SUB_S_BANK					107									//ѡׯ
#define SUB_S_RECRUIT_CARD			108									//����

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONG							lBaseScore;							//��������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	LONG							lBaseScore;							//��������
	WORD							wBankerUser;						//ׯ���û�
	WORD							wLastOutUser;						//���Ƶ���
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bCardData[10];						//�����˿�
	BYTE							bCardCount[4];						//�˿���Ŀ
	BYTE							bTurnCardCount;						//��������
	BYTE							bTurnCardData[10];					//�����б�
	LONG                            lAllTurnScore[GAME_PLAYER];			//�ֵܾ÷�
	LONG                            lLastTurnScore[GAME_PLAYER];		//�Ͼֵ÷�
};
//�û���ʮ
struct CMD_S_DisplayTen
{
	WORD                            wChairID;                           //��ʮ�û�
	BYTE                            bTenStatus;                         //��ʮ����
																
};

//�����˿�
struct CMD_S_GameStart
{
	WORD							wBanker	;								//����˿�
	WORD							wCurrentUser;							//��ǰ���
	BYTE							bCardData[6];							//�˿��б�
};
struct CMD_S_Bank
{
	BYTE							Moster;								//����˿�
	BYTE							bCardData[GAME_PLAYER];				//�˿��б�
};
//�û�����
struct CMD_S_OutCard
{
	BYTE							bCardCount;							//�˿���Ŀ
	WORD				 			wCurrentUser;						//��ǰ���
	WORD							wOutCardUser;						//�������
	BYTE							bCardData[10];						//�˿��б�
};

//��������
struct CMD_S_PassCard
{
	BYTE							bNewTurn;							//һ�ֿ�ʼ
	WORD				 			wPassUser;							//�������
	WORD				 			wCurrentUser;						//��ǰ���
};

//��Ϸ����
struct CMD_S_GameEnd
{
	LONG							lGameTax;							//��Ϸ˰��
	LONG							lGameScore[GAME_PLAYER];			//��Ϸ����
	BYTE							bCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							bCardData[50];						//�˿��б� 
};
//���ƽṹ
struct CMD_S_RecruitCard
{
	WORD							wCurrentUser;						//��ǰ�û�
	BYTE							bRecruitCard[5];					//��������
	BYTE							bRecruitCount;
	BYTE							bOneTurnScoreCard[12];				//һ�ֵ���ֵ��
	BYTE							bOneTurnScoreCardCount;				//��ֵ����
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_OUT_CART				2									//�û�����
#define SUB_C_PASS_CARD				3									//��������
#define SUB_C_BAIPAI                4                                    //�û�����                  
#define SUB_C_TOUXIANG              5                                   //�û�Ͷ��
#define SUB_C_JIU					6									//�û�����
#define SUB_C_FOURTEN               7									//�ĸ�ʮ
#define SUB_C_DISPLAYTEN            8                                   //��ʮ

//�������ݰ�
struct CMD_C_OutCard
{
	BYTE							bCardCount;							//������Ŀ
	BYTE							bCardData[10];						//�˿��б�
};
struct CMD_C_DisplayTen
{
	BYTE                            bDisplayStatus;                          //��ʮ������
};

//////////////////////////////////////////////////////////////////////////

#endif