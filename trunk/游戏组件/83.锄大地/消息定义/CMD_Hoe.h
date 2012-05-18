#ifndef CMD_HOE_HEAD_FILE
#define CMD_HOE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#ifdef	VIDEO_GAME
#define KIND_ID						513									//��Ƶ��Ϸ I D
#else
#define KIND_ID						105									//����Ƶ��Ϸ I D
#endif

#define GAME_PLAYER					4									//��Ϸ����
#define GAME_NAME					TEXT("�����")						//��Ϸ����

//��Ϸ״̬
#define GS_RF_FREE					GS_FREE								//�ȴ���ʼ
#define GS_RF_PLAYING				GS_PLAYING							//�з�״̬

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START			101									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				102									//�û�����
#define SUB_S_PASS_CARD				103									//��������
#define SUB_S_GAME_END				104									//��Ϸ����
#define SUB_S_STRUSTEE				105									//�û��й�

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONG							lBaseScore;							//��������
	//�йܱ���
	bool							bStrustee[GAME_PLAYER];				//�йܱ�־
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	LONG							lBaseScore;							//��������
	WORD							wBankerUser;						//ׯ���û�
	WORD							wLastOutUser;						//���Ƶ���
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bCardData[13*GAME_PLAYER];			//�����˿�
	BYTE							bCardCount[4];						//�˿���Ŀ
	BYTE							bTurnCardCount;						//��������
	BYTE							bTurnCardData[13];					//�����б�
	LONG			                lAllTurnScore[4];					//�ֵܾ÷�
	LONG	                        lLastTurnScore[4];					//�Ͼֵ÷�
	//�йܱ���
	bool							bStrustee[GAME_PLAYER];				//�йܱ�־
};

//�����˿�
struct CMD_S_GameStart
{
	BYTE							cbOutCard3;							//������־
	WORD							wBankerUser;						//ׯ���û�
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							cbCardData[13*GAME_PLAYER];			//�˿��б�
};

//�û�����
struct CMD_S_OutCard
{
	BYTE							bCardCount;							//�˿���Ŀ
	WORD				 			wCurrentUser;						//��ǰ���
	WORD							wOutCardUser;						//�������
	BYTE							bCardData[13];						//�˿��б�
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
	LONG							lGameScore[4];						//��Ϸ����
	BYTE							bCardCount[4];						//�˿���Ŀ
	BYTE							bCardData[52];						//�˿��б� 
};

//�û��й�
struct CMD_S_Strustee
{
	WORD							wChairId;							//�й��û�
	bool							bStrustee;							//�Ƿ��й�
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_OUT_CART				2									//�û�����
#define SUB_C_PASS_CARD				3									//��������
#define SUB_C_STRUSTEE				4									//�û��й�

//�������ݰ�
struct CMD_C_OutCard
{
	BYTE							bCardCount;							//������Ŀ
	BYTE							bCardData[13];						//�˿��б�
};

//�û��й�
struct CMD_C_Strustee
{
	bool bStrustee;														//�Ƿ��й�
};

//////////////////////////////////////////////////////////////////////////

#endif
