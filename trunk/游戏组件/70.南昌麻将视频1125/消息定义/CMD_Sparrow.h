#ifndef CMD_SPARROW_HEAD_FILE
#define CMD_SPARROW_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						557									//��Ϸ I D
#define GAME_PLAYER					4									//��Ϸ����
#define GAME_NAME					TEXT("�ϲ��齫")					//��Ϸ����
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_MATCH|GAME_GENRE_GOLD)	//��Ϸ����

//��Ϸ״̬
#define GS_MJ_FREE					GS_FREE								//����״̬
#define GS_MJ_BATCH					(GS_PLAYING+1)						//ϴ��״̬
#define GS_MJ_SICE					(GS_PLAYING+2)						//ɸ��״̬	
#define GS_MJ_PLAY					(GS_PLAYING+3)						//��Ϸ״̬

#define MAX_COUNT					14  								//�����Ŀ
#define MUST_LEFT_COUNT				36									//Ԥ����Ŀ	

//////////////////////////////////////////////////////////////////////////

//�������
struct CMD_WeaveItem
{
	BYTE							cbWeaveKind;						//�������
	BYTE							cbCenterCard;						//�����˿�
	BYTE							cbPublicCard;						//������־
	WORD							wProvideUser;						//��Ӧ�û�
};

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				101									//��������
#define SUB_S_SEND_CARD				102									//�����˿�
#define SUB_S_OPERATE_NOTIFY		104									//������ʾ
#define SUB_S_OPERATE_RESULT		105									//��������
#define SUB_S_GAME_END				106									//��Ϸ����
#define SUB_S_BATCHRESULT			107									//ϴ��
#define SUB_S_TRUSTEE				108									//�û��й�
#define SUB_S_BATCH					109									//ϴ������
//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONG							lCellScore;							//�������
	WORD							wBankerUser;						//ׯ���û�
	bool							bTrustee[GAME_PLAYER];						//�Ƿ��й�
	LONG                            lAllTurnScore;					//�ֵܾ÷�
	LONG                            lLastTurnScore;					//�Ͼֵ÷�

};

//��Ϸ״̬
struct CMD_S_StatusBatch
{
	LONG							lCellScore;							//�������
	BYTE							bComplete;							//��ɱ�־
	bool							bTrustee[GAME_PLAYER];						//�Ƿ��й�

	//��ʷ����
	LONG                            lAllTurnScore[4];					//�ֵܾ÷�
	LONG                            lLastTurnScore[4];					//�Ͼֵ÷�

};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��Ϸ����
	LONG							lCellScore;							//��Ԫ����
	WORD							wSiceCount;							//���ӵ���
	WORD							wSiceCount2;						//���ӵ���
	WORD							wSiceCount3;						//���ӵ���
	WORD							wBankerUser;						//ׯ���û�
	WORD							wCurrentUser;						//��ǰ�û�
	BYTE							cbChip[GAME_PLAYER];				//ϴ�����
	bool							bTrustee[GAME_PLAYER];						//�Ƿ��й�

	
	//���Ƽ�¼
	BYTE							cbFengCardData[8];					//���Ƽ�¼
	BYTE							cbFengCardCount;					//���Ƽ�¼

	//״̬����
	BYTE							cbActionCard;						//�����˿�
	WORD							wActionMask;						//��������
	BYTE							cbLeftCardCount;					//ʣ����Ŀ

	//������Ϣ
	WORD							wOutCardUser;						//�����û�
	BYTE							cbOutCardData;						//�����˿�
	BYTE							cbDiscardCount[4];					//������Ŀ
	BYTE							cbDiscardCard[4][55];				//������¼

	//�˿�����
	BYTE							cbCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							cbKingCardIndex[2];						//�����˿�
	BYTE							cbCardData[GAME_PLAYER][MAX_COUNT];				//�˿��б�
	BYTE							cbSendCardData;								//�����˿�

	//����˿�
	BYTE							cbWeaveCount[4];					//�����Ŀ
	CMD_WeaveItem					WeaveItemArray[4][4];				//����˿�
	
	//��ʷ����
	LONG                            lAllTurnScore[4];					//�ֵܾ÷�
	LONG                            lLastTurnScore[4];					//�Ͼֵ÷�
	
	//���·�
	LONG							lUpDownScore[GAME_PLAYER];			//���·��÷�	
	BYTE							cbUpDownKingCardData;				//���·�����
	BYTE							cbFloor;							//¥��	

};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD							wSiceCount;							//���ӵ���
	WORD							wSiceCount2;						//���ӵ���2
	WORD							wSiceCount3;						//���ӵ���3
	WORD							wBankerUser;						//ׯ���û�
	WORD							wCurrentUser;						//��ǰ�û�
	BYTE							wUserAction;						//�û�����
	BYTE							cbChip[GAME_PLAYER];				//ϴ�����
	BYTE							cbKingCardIndex[2];					//�����˿�
	BYTE							cbCardData[GAME_PLAYER][MAX_COUNT];				//�˿��б�
	bool							bTrustee[GAME_PLAYER];				//�Ƿ��й�
	LONG							lUpDownScore[GAME_PLAYER];			//���·��÷�	
	BYTE							cbUpDownKingCardData;				//���·�����
	BYTE							cbFloor;							//¥��	
};
struct CMD_S_BatchResult
{
	BYTE							cbBatchResult[GAME_PLAYER];						//ׯ���û�
};

//��������
struct CMD_S_OutCard
{
	WORD							wOutCardUser;						//�����û�
	BYTE							cbOutCardData;						//�����˿�
};

//�����˿�
struct CMD_S_SendCard
{
	BYTE							cbCardData;							//�˿�����
	WORD							wActionMask;						//��������
	WORD							wCurrentUser;						//��ǰ�û�
	bool							cbIsNotGang;							//�Ƿ����
};

//��������
struct CMD_S_ListenCard
{
	WORD							wListenUser;						//�����û�
};

//������ʾ
struct CMD_S_OperateNotify
{
	WORD							wResumeUser;						//��ԭ�û�
	WORD							wActionMask;						//��������
	BYTE							cbActionCard;						//�����˿�
};

//��������
struct CMD_S_OperateResult
{
	WORD							wOperateUser;						//�����û�
	WORD							wProvideUser;						//��Ӧ�û�
	WORD							wOperateCode;						//��������
	BYTE							cbOperateCard;						//�����˿�
};

//��Ϸ����
struct CMD_S_GameEnd
{

	//˰��
	LONG							lGameTax;							//��Ϸ˰��

	//���
	BYTE							cbChongguan[GAME_PLAYER];			//�����Ŀ	

	//������
	BYTE							cbBaWangKing[GAME_PLAYER];			//������

	//������Ϣ
	WORD							wProvideUser;						//��Ӧ�û�
	BYTE							cbChiHuCard;						//��Ӧ�˿�
	DWORD							dwChiHuKind[GAME_PLAYER];			//��������
	DWORD							dwChiHuRight[GAME_PLAYER];			//��������

	//������Ϣ
	LONG							lGameScore[GAME_PLAYER];			//��Ϸ����
	LONG							lUpDownScore[GAME_PLAYER];			//���·�����
	LONG							lGameScoreEx[GAME_PLAYER];			//������
	LONG							lGameHuScore[GAME_PLAYER];			//���Ʒ�

	//�˿���Ϣ
	BYTE							cbCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							cbCardData[GAME_PLAYER][MAX_COUNT];	//�˿�����


	LONG							lAllTurnScore[GAME_PLAYER];			//�ֵܾ÷�
	LONG							lLastTurnScore[GAME_PLAYER];		//�Ͼֵ÷�	

};
//�û��й�
struct CMD_S_Trustee
{
	bool							bTrustee;							//�Ƿ��й�
	WORD							wChairID;							//�й��û�
};
//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_OUT_CARD				1									//��������
#define SUB_C_BATCH					2									//ϴ�迪ʼ
#define SUB_C_OPERATE_CARD			3									//�����˿�
#define SUB_C_TRUSTEE				4									//�û��й�
#define SUB_C_BATCH_END				5									//ϴ�����	
#define SUB_C_PASS_KING				6									//��������							

//��������
struct CMD_C_OutCard
{
	BYTE							cbCardData;							//�˿�����
};

//ϴ��
struct CMD_C_BATCH
{
	BYTE							cbChipTimes;						//�򶥱���
	WORD							wCurrentUser;						//��ǰ�û�
};

//��������
struct CMD_C_OperateCard
{
	WORD							wOperateCode;						//��������
	BYTE							cbOperateCard;						//�����˿�
};
//�û��й�
struct CMD_C_Trustee
{
	bool							bTrustee;							//�Ƿ��й�	
};

//////////////////////////////////////////////////////////////////////////

#endif