#ifndef CMD_UPGRADE_HEAD_FILE
#define CMD_UPGRADE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#ifdef	VIDEO_GAME
#define KIND_ID						500									//��Ƶ��Ϸ I D
#else
#define KIND_ID						103									//����Ƶ��Ϸ I D

#endif
#define GAME_PLAYER					4									//��Ϸ����
#define GAME_NAME					TEXT("������Ϸ")					//��Ϸ����

//��Ϸ״̬
#define GS_UG_FREE					GS_FREE								//����״̬
#define GS_UG_CALL					(GS_PLAYING+1)						//����״̬
#define GS_UG_BACK					(GS_PLAYING+2)						//����״̬
#define GS_UG_PLAY					(GS_PLAYING+3)						//��Ϸ״̬
#define GS_UG_WAIT					(GS_PLAYING+4)						//�ȴ�״̬


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
#define SUB_S_STRUSTEE				109									//�û��й�
#define SUB_S_REQUEST_LEAVE			110									//�����뿪
#define SUB_S_BATCH_CARD			111									//��������

//�Ƹ�����
#define PACK_COUNT					2									//�����
#define CELL_PACK					54									//��Ԫ��Ŀ
#define MAX_COUNT					PACK_COUNT*CELL_PACK/GAME_PLAYER+8	//�����Ŀ

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONG							lBaseScore;							//��������
	//�йܱ���
	bool							bStrustee[GAME_PLAYER];				//�йܱ�־
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
	BYTE							cbCardData[MAX_COUNT];				//�˿��б�

	//������Ϣ
	BYTE							cbComplete;							//��ɱ�־
	BYTE							cbCallCard;							//�����˿�
	BYTE							cbCallCount;						//������Ŀ
	WORD							wCallCardUser;						//�����û�

	//�йܱ���
	bool							bStrustee[GAME_PLAYER];				//�йܱ�־
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
	BYTE							cbCardData[MAX_COUNT];				//�˿��б�

	//������Ϣ
	BYTE							cbConcealCount;						//������Ŀ
	BYTE							cbConcealCard[8];					//�����˿�

	//�йܱ���
	bool							bStrustee[GAME_PLAYER];				//�йܱ�־
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
	BYTE							cbCardData[MAX_COUNT];				//�˿��б�

	//������Ϣ
	BYTE							cbConcealCount;						//������Ŀ
	BYTE							cbConcealCard[8];					//�����˿�

	//���Ʊ���
	BYTE							cbOutCardCount[GAME_PLAYER];				//������Ŀ
	BYTE							cbOutCardData[GAME_PLAYER][MAX_COUNT];		//�����б�
	BYTE							cbLastTurnCard[GAME_PLAYER][MAX_COUNT];		//�ϴγ���
	BYTE							cbLastTurnCount[GAME_PLAYER];				//�ϴγ�����

	//�÷ֱ���
	BYTE							cbScoreCardCount;					//�˿���Ŀ
	BYTE							cbScoreCardData[PACK_COUNT*12];		//�÷��˿�

	//�йܱ���
	bool							bStrustee[GAME_PLAYER];				//�йܱ�־
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
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbCardData[MAX_COUNT];				//�˿��б�
};

//�û�����
struct CMD_S_CallCard
{
	BYTE							cbCallCard;							//�����˿�
	BYTE							cbCallCount;						//������Ŀ
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
	BYTE							cbCardData[MAX_COUNT];				//�˿��б�
};

//˦�ƽ��
struct CMD_S_ThrowResult
{
	WORD							wOutCardUser;						//�������
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							cbThrowCardCount;					//�˿���Ŀ
	BYTE							cbResultCardCount;					//�˿���Ŀ
	BYTE							cbCardDataArray[2*MAX_COUNT];		//�˿�����
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
	LONG							lScore[GAME_PLAYER];				//�û��÷�
	WORD							wGameScore;							//��Ϸ�÷�
	WORD							wConcealTime;						//�۵ױ���
	WORD							wConcealScore;						//���ƻ���
	BYTE							cbConcealCount;						//������Ŀ
	BYTE							cbConcealCard[8];					//�����˿�
};

//�û��й�
struct CMD_S_Strustee
{
	WORD							wChairId;							//�й��û�
	bool							bStrustee;							//�Ƿ��й�
};

//�����뿪
struct CMD_S_RequestLeave
{
	WORD							wChairID;							//�뿪�û�
	TCHAR							szLeaveReason[128];					//�뿪ԭ��
};

//��������
struct CMD_S_BatchCard
{
	BYTE							cbCardCount;						//�˿���Ŀ
	WORD							wOutCardUser;						//�������
	WORD							wTurnWinner;						//һ��ʤ��
	BYTE							cbCardData[PACK_COUNT*CELL_PACK];	//�˿��б�
};


//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_CALL_CARD				1									//�û�����
#define SUB_C_CALL_FINISH			2									//�������
#define SUB_C_CONCEAL_CARD			3									//�����˿�
#define SUB_C_OUT_CARD				4									//�û�����
#define SUB_C_STRUSTEE				5									//�û��й�
#define SUB_C_REQUEST_LEAVE			6									//�����뿪
#define SUB_C_RESPONSES_LEAVE		7									//��Ӧ�뿪

//�û�����
struct CMD_C_CallCard
{
	BYTE							cbCallCard;							//�����˿�
	BYTE							cbCallCount;						//������Ŀ
};

//�����˿�
struct CMD_C_ConcealCard
{
	BYTE							cbConcealCount;						//������Ŀ
	BYTE							cbConcealCard[8];					//�����˿�
};

//�û�����
struct CMD_C_OutCard
{
	BYTE							cbCardCount;						//������Ŀ
	BYTE							cbCardData[MAX_COUNT];				//�˿��б�
};

//�û��й�
struct CMD_C_Strustee
{
	bool bStrustee;														//�Ƿ��й�
};

//�����뿪
struct CMD_C_RequestLeave
{
	TCHAR							szLeaveReason[128];					//�뿪ԭ��
};

//�����Ӧ
struct CMD_C_ResponsesLeave
{
	WORD							wChairID;							//�뿪�û�
	BYTE							cbAgreeLeave;						//ͬ���뿪
};

//////////////////////////////////////////////////////////////////////////

#endif