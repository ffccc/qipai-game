#ifndef CMD_LAND_HEAD_FILE
#define CMD_LAND_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						1155							//��Ϸ I D
#define GAME_PLAYER					3									//��Ϸ����
#define GAME_NAME					TEXT("��ţ")						//��Ϸ����
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_GOLD)	//��Ϸ����

//��Ϸ״̬
#define GS_WK_FREE					GS_FREE								//�ȴ���ʼ
#define GS_WK_SCORE					GS_PLAYING							//�з�״̬
#define GS_WK_PLAYING				GS_PLAYING+1						//��Ϸ����

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_SEND_CARD				100									//��������
#define SUB_S_USER_COUPAI			101									//�з�����
#define SUB_S_GAME_START			102									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				103									//�û�����
#define SUB_S_PASS_CARD				104									//��������
#define SUB_S_GAME_END				105									//��Ϸ����
#define SUB_S_USER_XIANPAI			106									//��������
#define	SUB_S_USER_QIANGCI			107									//����
#define	SUB_S_USER_GIVEUP_QIANGCI	108									//��������

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONG							lBaseScore;							//��������
};

//��Ϸ״̬
struct CMD_S_StatusScore
{
	BYTE							bLandScore;							//��������
	LONG							lBaseScore;							//��������
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bScoreInfo[3];						//�з���Ϣ
	BYTE							bCardData[17];						//�����˿�
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	WORD							wLandUser;							//�������
	WORD							wBombTime;							//ը������
	LONG							lBaseScore;							//��������
	BYTE							bLandScore;							//��������
	WORD							wLastOutUser;						//���Ƶ���
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bBackCard[3];						//�����˿�
	BYTE							bCardData[16];						//�����˿�
	BYTE							bCardCount[3];						//�˿���Ŀ
	BYTE							bTurnCardCount;						//��������
	BYTE							bTurnCardData[16];					//�����б�
};

//�����˿�
struct CMD_S_SendCard
{
	WORD				 			wBankUser;						//��ǰ���
	BYTE							bCardData[16];						//�˿��б�
};

//�û��з�
struct CMD_S_LandScore
{
	WORD							bLandUser;							//�з����
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bLandScore;							//�ϴνз�
	BYTE							bCurrentScore;						//��ǰ�з�
};
//�û�����
struct CMD_S_User_CouPai
{
	WORD							wCouPaiUserID;						//�������
	WORD							wCurrentUser;						//��ǰ���
};
//�û�����
struct CMD_S_User_XianPai
{
	WORD							wXianPaiUserID;						//�������
	WORD							wCurrentUser;						//��ǰ���
};
//�û�����
struct CMD_S_User_QiangCi
{
	WORD							wQiangCiUserID;						//�������
};
//��������
struct CMD_S_GiveUp_QiangCi
{
	WORD							wGiveUpQiangCiUserID;				//��������
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD				 			wBankUser;							//�������
	//BYTE							bLandScore;							//��������
	//WORD				 			wCurrentUser;						//��ǰ���
	//BYTE							bBackCard[3];						//�����˿�
	BYTE							bCardData[16];						//�˿��б�
};

//�û�����
struct CMD_S_OutCard
{
	bool							bNewTurn;							//һ�ֿ�ʼ
	bool							bMostCard;							//���
	BYTE							bCardCount;							//������Ŀ
	WORD				 			wCurrentUser;						//��ǰ���
	WORD							wOutCardUser;						//�������
	BYTE							bCardData[16];						//�˿��б�
	BYTE							bZhanPaiData[16];					//һ�ֵ����б�
};

//��������
struct CMD_S_PassCard
{
	BYTE							bZhanpaiUser;						//ռ���û�
	BYTE							bNewTurn;							//һ�ֿ�ʼ
	WORD				 			wPassUser;							//�������
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bZhanPaiData[16];					//һ�ֵ����б�
};

//��Ϸ����
struct CMD_S_GameEnd
{
	LONG							lGameTax;							//��Ϸ˰��
	LONG							lGameScore[3];						//��Ϸ����
	BYTE							bCardCount[3];						//�˿���Ŀ
	BYTE							bCardData[48];						//�˿��б� 
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_COUPAI				1									//�û�����
#define SUB_C_OUT_CART				2									//�û�����
#define SUB_C_PASS_CARD				3									//��������
#define SUB_C_XIANPAI				4									//�û�����
#define SUB_C_QIANGCI				5									//�û�����
#define SUB_C_GIVEUP_QIANGCI		6									//��������

////���ƽṹ
//struct CMD_C_CouPai
//{
//	BYTE							bLandScore;							//��������
//};
//���ƽṹ
struct CMD_C_XianPai
{
	
};
//���̽ṹ
struct CMD_C_QiangCi
{
};

//�������ݰ�
struct CMD_C_OutCard
{
	BYTE							bCardCount;							//������Ŀ
	BYTE							bCardData[16];						//�˿��б�
};

//////////////////////////////////////////////////////////////////////////

#endif