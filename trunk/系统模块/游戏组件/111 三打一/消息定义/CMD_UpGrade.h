#ifndef CMD_UPGRADE_HEAD_FILE
#define CMD_UPGRADE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��
#define TAX_MIN_NUM	10 //Ҫ��˰�����ٵ�

#define KIND_ID						ID_3V1Card							//��Ϸ I D
#define GAME_PLAYER					4									//��Ϸ����
#define GAME_NAME					TEXT("����һ��Ϸ")					//��Ϸ����
#define GAME_GENRE					(GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//��Ϸ����

//��Ϸ״̬
#define GS_UG_FREE					GS_FREE								//����״̬
#define GS_UG_CALL					(GS_PLAYING+1)						//����״̬
#define GS_UG_BACK					(GS_PLAYING+2)						//����״̬
#define GS_UG_PLAY					(GS_PLAYING+3)						//��Ϸ״̬
#define GS_UG_WAIT					(GS_PLAYING+4)						//�ȴ�״̬
#define GS_UG_SCORE					(GS_PLAYING+5)						//�з�״̬

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_SEND_CARD				101									//�����˿�
#define SUB_S_CALL_CARD				102									//�û�����
#define SUB_S_SEND_CONCEAL			103									//�����˿�
#define SUB_S_GAME_PLAY				104									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				105									//�û�����
#define SUB_S_THROW_RESULT			106									//˦�ƽ��
#define SUB_S_TURN_BALANCE			107									//һ��ͳ��
#define SUB_S_GAME_END				108									//��Ϸ����
#define SUB_S_CALL_SCORE			109									//�з�����

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONG							lBaseScore;							//��������
};

//����״̬
struct CMD_S_StatusCall
{
	//��Ϸ����
	WORD							wBankerUser;						//ׯ���û�
	BYTE							cbPackCount;						//������Ŀ
	BYTE							cbMainValue;						//������ֵ
	BYTE							cbValueOrder[2];					//������ֵ

	//���Ʊ���
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbCardData[52];						//�˿��б�

	//������Ϣ
	BYTE							cbComplete;							//��ɱ�־
	BYTE							cbCallCard;							//�����˿�
	BYTE							cbCallCount;						//������Ŀ
	WORD							wCallCardUser;						//�����û�
};


//�з�״̬
struct CMD_S_StatusScore
{
	//��Ϸ����
	WORD							wBankerUser;						//ׯ���û�
	BYTE							bBossScore;							//ׯ�ҷ���
	BYTE							cbPackCount;						//������Ŀ
	BYTE							cbMainValue;						//������ֵ
	BYTE							cbValueOrder[2];					//������ֵ

	//���Ʊ���
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbCardData[52];						//�˿��б�

	
	//�з���Ϣ
	BYTE							bScoreInfo[4];						//�з���Ϣ
	WORD							wCallScoreUser;						//�з��û�
	WORD				 			wCurrentUser;						//��ǰ���

	//������Ϣ
	BYTE							cbCallCard;							//�����˿�
	BYTE							cbCallCount;						//������Ŀ
	WORD							wCallCardUser;						//�����û�

	
};

//����״̬
struct CMD_S_StatusBack
{
	//��Ϸ����
	BYTE							cbPackCount;						//������Ŀ
	BYTE							cbMainValue;						//������ֵ
	BYTE							cbMainColor;						//������ֵ
	BYTE							cbValueOrder[2];					//������ֵ

	//������Ϣ
	BYTE							cbCallCard;							//�����˿�
	BYTE							cbCallCount;						//������Ŀ
	WORD							wCallCardUser;						//�����û�

	//�û�����
	WORD							wBankerUser;						//ׯ���û�
	WORD							wCurrentUser;						//��ǰ�û�

	//�˿˱���
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbCardData[60];						//�˿��б�

	//������Ϣ
	BYTE							cbConcealCount;						//������Ŀ
	BYTE							cbConcealCard[8];					//�����˿�
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��Ϸ����
	BYTE							cbPackCount;						//������Ŀ
	BYTE							cbMainValue;						//������ֵ
	BYTE							cbMainColor;						//������ֵ
	BYTE							cbValueOrder[2];					//������ֵ

	//�û�����
	WORD							wBankerUser;						//ׯ���û�
	WORD							wCurrentUser;						//��ǰ�û�
	WORD							wFirstOutUser;						//�����û�

	//������Ϣ
	BYTE							cbCallCard;							//�����˿�
	BYTE							cbCallCount;						//������Ŀ
	WORD							wCallCardUser;						//�����û�

	//�˿˱���
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbCardData[52];						//�˿��б�

	//������Ϣ
	BYTE							cbConcealCount;						//������Ŀ
	BYTE							cbConcealCard[8];					//�����˿�

	//���Ʊ���
	BYTE							cbOutCardCount[4];					//������Ŀ
	BYTE							cbOutCardData[4][52];				//�����б�

	//�÷ֱ���
	BYTE							cbScoreCardCount;					//�˿���Ŀ
	BYTE							cbScoreCardData[48];				//�÷��˿�
};

//�ȴ�״̬
struct CMD_S_StatusWait
{
	//��Ϸ����
	BYTE							cbPackCount;						//������Ŀ
	BYTE							cbMainValue;						//������ֵ
	BYTE							cbValueOrder[2];					//������ֵ

	//�û�����
	WORD							wBankerUser;						//ׯ���û�
};


//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD							wBankerUser;						//ׯ���û�
	BYTE							cbPackCount;						//������Ŀ
	BYTE							cbMainValue;						//������ֵ
	BYTE							cbValueOrder[2];					//������ֵ
};

//�����˿�
struct CMD_S_SendCard
{
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbCardData[52];						//�˿��б�
};

//�û��з�
struct CMD_S_CallScore
{
	WORD							bCallScoreUser;						//�з����
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bPrevScore;							//�ϴνз�
	BYTE							bCurrentScore;						//��ǰ�з�
};

//�û�����
struct CMD_S_CallCard
{
	//BYTE							cbCallCard;							//�����˿�
	BYTE							cbCallCount;						//������Ŀ
	BYTE							cbCallColor;						//���ƻ�ɫ
	WORD							wCallCardUser;						//�����û�
};

//�����˿�
struct CMD_S_SendConceal
{
	BYTE							cbMainColor;						//������ֵ
	WORD							wBankerUser;						//ׯ���û�
	WORD							wCurrentUser;						//��ǰ�û�
	BYTE							cbConcealCount;						//������Ŀ
	BYTE							cbConcealCard[8];					//�����˿�
};

//��Ϸ��ʼ
struct CMD_S_GamePlay
{
	WORD							wCurrentUser;						//��ǰ�û�
	BYTE							cbConcealCount;						//������Ŀ
	BYTE							cbConcealCard[8];					//�����˿�
};

//�û�����
struct CMD_S_OutCard
{
	BYTE							cbCardCount;						//�˿���Ŀ
	WORD							wOutCardUser;						//�������
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							cbCardData[52];						//�˿��б�
};

//˦�ƽ��
struct CMD_S_ThrowResult
{
	WORD							wOutCardUser;						//�������
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							cbThrowCardCount;					//�˿���Ŀ
	BYTE							cbResultCardCount;					//�˿���Ŀ
	BYTE							cbCardDataArray[104];				//�˿�����
};

//һ��ͳ��
struct CMD_TurnBalance
{
	WORD							wTurnWinner;						//һ��ʤ��
	WORD				 			wCurrentUser;						//��ǰ���
};

//��Ϸ����
struct CMD_S_GameEnd
{
	LONG							lScore[4];							//�û��÷�
	WORD							wGameScore;							//��Ϸ�÷�
	WORD							wConcealTime;						//�۵ױ���
	WORD							wConcealScore;						//���ƻ���
	BYTE							cbConcealCount;						//������Ŀ
	BYTE							cbConcealCard[8];					//�����˿�
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_CALL_CARD				1									//�û�����
#define SUB_C_CALL_FINISH			2									//�������
#define SUB_C_CONCEAL_CARD			3									//�����˿�
#define SUB_C_OUT_CARD				4									//�û�����
#define SUB_C_CALL_SCORE			5									//�û��з�

//�û�����
struct CMD_C_CallCard
{
	//BYTE							cbCallCard;							//�����˿�
	//BYTE							cbCallCount;						//������Ŀ
	BYTE							cbCallColor;						//���ƻ�ɫ
};

//�����˿�
struct CMD_C_ConcealCard
{
	BYTE							cbConcealCount;						//������Ŀ
	BYTE							cbConcealCard[8];					//�����˿�
};

//�û��з�
struct CMD_C_CallScore
{
	BYTE							bBossScore;							//��������
};

 
//�û�����
struct CMD_C_OutCard
{
	BYTE							cbCardCount;						//������Ŀ
	BYTE							cbCardData[52];						//�˿��б�
};

//////////////////////////////////////////////////////////////////////////

#endif